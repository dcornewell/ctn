/*
          Copyright (C) 1993, 1994, RSNA and Washington University

          The software and supporting documentation for the Radiological
          Society of North America (RSNA) 1993, 1994 Digital Imaging and
          Communications in Medicine (DICOM) Demonstration were developed
          at the
                  Electronic Radiology Laboratory
                  Mallinckrodt Institute of Radiology
                  Washington University School of Medicine
                  510 S. Kingshighway Blvd.
                  St. Louis, MO 63110
          as part of the 1993, 1994 DICOM Central Test Node project for, and
          under contract with, the Radiological Society of North America.

          THIS SOFTWARE IS MADE AVAILABLE, AS IS, AND NEITHER RSNA NOR
          WASHINGTON UNIVERSITY MAKE ANY WARRANTY ABOUT THE SOFTWARE, ITS
          PERFORMANCE, ITS MERCHANTABILITY OR FITNESS FOR ANY PARTICULAR
          USE, FREEDOM FROM ANY COMPUTER DISEASES OR ITS CONFORMITY TO ANY
          SPECIFICATION. THE ENTIRE RISK AS TO QUALITY AND PERFORMANCE OF
          THE SOFTWARE IS WITH THE USER.

          Copyright of the software and supporting documentation is
          jointly owned by RSNA and Washington University, and free access
          is hereby granted as a license to use this software, copy this
          software and prepare derivative works based upon this software.
          However, any distribution of this software source code or
          supporting documentation or derivative works (source code and
          supporting documentation) must include the three paragraphs of
          the copyright notice.
*/
/* Copyright marker.  Copyright will be inserted above.  Do not remove */

/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	TBL_Open
**			TBL_Close
**			TBL_Select
**			TBL_Update
**			TBL_Insert
**			TBL_Delete
**			TBL_Debug
** Author, Date:	David E. Beecher, 28-Feb-94
** Intent:		Provide a general set of functions to be performed
**			on tables in a relational database.
** Last Update:		$Author: smm $, $Date: 2006-06-07 21:07:23 $
** Source File:		$RCSfile: tbl_sybase.c,v $
** Revision:		$Revision: 1.43 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.43 $ $RCSfile: tbl_sybase.c,v $";

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#ifdef MALLOC_DEBUG
#include "malloc.h"
#endif
#include "dicom.h"
#include "condition.h"
#include "tblprivate.h"
#include "tbl.h"

#ifdef SYBASE

#include "sybfront.h"
#include "sybdb.h"

#include "tbl_sybase.h"
#ifdef CTN_USE_THREADS
#include "ctnthread.h"
#endif



/*
** Static Globals for this file...
*/
static int
    G_SybaseError = 0;
static TBL_CONTEXT
*   G_ContextHead = (TBL_CONTEXT *) NULL;

static LOGINREC
*   G_LoginRecord;

static DBPROCESS
*   G_DBOpen,
   *G_DBSelect,
   *G_DBInsert,
   *G_DBUpdate,
   *G_DBDelete,
   *G_DBNextUnique,
   *G_DBLayout;

static int
    G_OpenFlag = 0;

static CTNBOOLEAN
    G_Verbose = FALSE;

static CTNBOOLEAN G_ConsistencyTestsOnOpen = TRUE;

CONDITION
TBL_SetOption(const char *string)
{
    if (strcmp(string, "OPEN_SPEEDUP") == 0)
	G_ConsistencyTestsOnOpen = FALSE;

    return TBL_NORMAL;
}

int
TBL_HasViews(void)
{
    return 1;
}

int
TBL_HasUpdateIncrement(void)
{
    return 1;
}

/* TBL_Debug
**
** Purpose:
**	Simple function to switch on/off Sybase debug messages.
**
** Parameter Dictionary:
**	CTNBOOLEAN flag: the flag that controls the messages
**
** Return Values:
**	TBL_NORMAL: normal termination.
**
** Notes:
**	The initial state of the debugging messages is off (FALSE).
**
** Algorithm:
**	If flag evaluates to true, the global variable G_Verbose is
*	set (to TRUE) otherwise it is reset (to FALSE);
*/
CONDITION
TBL_Debug(CTNBOOLEAN flag)
{

    if (flag == TRUE)
	G_Verbose = TRUE;
    else
	G_Verbose = FALSE;

    return TBL_NORMAL;

}

/* TBL_Open
**
** Purpose:
**	This function "opens" the specified table in the specified
**	database.  It creates a new handle for this particular table
**	and passes that identifier back to the user.
**
** Parameter Dictionary:
**	char *databaseName: The name of the database to open.
**	char *tableName: The name of the table to open in the
**		aforementioned database.
**	TBL_HANDLE **handle: The pointer for the new identifier
**		created for this database/table pair is returned
**		through handle.
**
** Return Values:
**	TBL_NORMAL: normal termination.
**	TBL_DBINITFAILED: The initial database open failed.
**	TBL_ALREADYOPENED: The table/database pair has been opened
**		previously and may not be opened again without
**		first closing it.
**	TBL_DBNOEXIST: The database specified as a calling parameter
**		does not exist.
**	TBL_TBLNOEXIST: The table specified as a calling parameter
**		does not exist within the specified database.
**	TBL_NOMEMORY: There is no memory available from the system.
**
** Notes:
**	Nothing unusual.
**
** Algorithm:
**	The first time TBL_Open is invoked, special Sybase routines
**	are called to allocate the communication structures needed
**	for subsequent operations.  A check is made to ensure that
**	this table/database pair has not already been opened.  A
**	unique handle(address) is then created for this pair and
**	returned to the user for subsequent table operations.
*/

CONDITION
TBL_Open(const char *databaseName, const char *tableName, TBL_HANDLE ** handle)
{
    TBL_CONTEXT
	* tc;
    char
       *sn,
        server_name[50],
       *tdb,
       *ttb;
    CTNBOOLEAN openSucceeded = TRUE;

#ifdef CTN_USE_THREADS
    THR_ObtainMutex(FAC_TBL);
#endif

    (*handle) = (void *) NULL;

    if (G_OpenFlag == 0) {
	if (dbinit() == FAIL) {
#ifdef CTN_USE_THREADS
	    THR_ReleaseMutex(FAC_TBL);
#endif
	    return COND_PushCondition(TBL_ERROR(TBL_DBINITFAILED),
				      DATABASE, "TBL_Open");
	}
	dberrhandle(syb_err_handler);
	dbmsghandle(syb_msg_handler);
	if ((G_LoginRecord = dblogin()) == NULL) {
#ifdef CTN_USE_THREADS
	    THR_ReleaseMutex(FAC_TBL);
#endif
	    return COND_PushCondition(TBL_ERROR(TBL_DBINITFAILED),
				      DATABASE, "TBL_Open");
	}
	DBSETLUSER(G_LoginRecord, SYBASE_USER);
	DBSETLPWD(G_LoginRecord, SYBASE_PASSWORD);
	DBSETLAPP(G_LoginRecord, SYBASE_APP);

	if ((sn = getenv("CTN_SYBASE_SERVER")) == NULL)
	    if ((sn = getenv("DSQUERY")) == NULL)
		strcpy(server_name, "SYBASE");
	    else
		strcpy(server_name, sn);
	else
	    strcpy(server_name, sn);

	if ((G_DBOpen = dbopen(G_LoginRecord, server_name)) == NULL)
	    openSucceeded = FALSE;
	if ((G_DBInsert = dbopen(G_LoginRecord, server_name)) == NULL)
	    openSucceeded = FALSE;
	if ((G_DBSelect = dbopen(G_LoginRecord, server_name)) == NULL)
	    openSucceeded = FALSE;
	if ((G_DBDelete = dbopen(G_LoginRecord, server_name)) == NULL)
	    openSucceeded = FALSE;
	if ((G_DBUpdate = dbopen(G_LoginRecord, server_name)) == NULL)
	    openSucceeded = FALSE;
	if ((G_DBNextUnique = dbopen(G_LoginRecord, server_name)) == NULL)
	    openSucceeded = FALSE;
	if ((G_DBLayout = dbopen(G_LoginRecord, server_name)) == NULL)
	    openSucceeded = FALSE;

	if (!openSucceeded) {
#ifdef CTN_USE_THREADS
	    THR_ReleaseMutex(FAC_TBL);
#endif
	    return COND_PushCondition(TBL_ERROR(TBL_DBINITFAILED),
				      DATABASE, "TBL_Open");
	}
    }
    tc = G_ContextHead;
    while (tc != (TBL_CONTEXT *) NULL) {
	if ((strcmp(tc->databaseName, databaseName) == 0) &&
	    (strcmp(tc->tableName, tableName) == 0)) {
	    (*handle) = (void *) tc;
	    tc->refCount++;
	    G_OpenFlag++;
#ifdef CTN_USE_THREADS
	    THR_ReleaseMutex(FAC_TBL);
#endif
	    return TBL_NORMAL;
#if 0
	    /* Large change on 22-May-1998.  We used to return an error */
	    /* Now we use reference counting on the table */
	    return COND_PushCondition(TBL_ERROR(TBL_ALREADYOPENED),
				      "TBL_Open",
				      databaseName, tableName);
#endif
	}
	tc = tc->next;
    }
    /*
     * The database/table pair has not been found...so now we check to see if
     * we can access it before adding it to our list....currently, we skip
     * this check because it takes a lot of time.  A bad Database name will
     * be caught in subseqnent calls, but a bad table spec is not handled so
     * well...this should be addressed at a later point.
     */
/*
 * Profile to see if this speeds things up much...
 * (YES, it does speed things up but we need the error checking...)
 *
*/
    if (getenv("TBL_SYBASE_SPEEDUP") == NULL && G_ConsistencyTestsOnOpen) {
	char localDBName[1024];
	strcpy(localDBName, databaseName);
	if (dbuse(G_DBOpen, localDBName) != SUCCEED) {
#ifdef CTN_USE_THREADS
	    THR_ReleaseMutex(FAC_TBL);
#endif
	    return COND_PushCondition(TBL_ERROR(TBL_DBNOEXIST), databaseName, "TBL_Open");
	}
	dbcmd(G_DBOpen, "sp_help ");
	dbcmd(G_DBOpen, tableName);
	G_SybaseError = 0;
	if ((dbsqlexec(G_DBOpen) != SUCCEED) || G_SybaseError) {
#ifdef CTN_USE_THREADS
	    THR_ReleaseMutex(FAC_TBL);
#endif
	    return COND_PushCondition(TBL_ERROR(TBL_TBLNOEXIST), tableName, "TBL_Open");
	}
	dbcancel(G_DBOpen);
    }
    /*
     * Both the Database and the Table exist, so we can add the new entry to
     * our list.
     */
    if ((tc = (TBL_CONTEXT *) malloc(sizeof(TBL_CONTEXT))) == (TBL_CONTEXT *) NULL) {
#ifdef CTN_USE_THREADS
	THR_ReleaseMutex(FAC_TBL);
#endif
	return COND_PushCondition(TBL_ERROR(TBL_NOMEMORY), "TBL_Open");
    }
    tc->refCount = 1;
    if ((tdb = (char *) malloc(strlen(databaseName) + 1)) == (char *) NULL) {
	free(tc);
#ifdef CTN_USE_THREADS
	THR_ReleaseMutex(FAC_TBL);
#endif
	return COND_PushCondition(TBL_ERROR(TBL_NOMEMORY), "TBL_Open");
    }
    if ((ttb = (char *) malloc(strlen(tableName) + 1)) == (char *) NULL) {
	free(tc);
	free(tdb);
#ifdef CTN_USE_THREADS
	THR_ReleaseMutex(FAC_TBL);
#endif
	return COND_PushCondition(TBL_ERROR(TBL_NOMEMORY), "TBL_Open");
    }
    strcpy(tdb, databaseName);
    strcpy(ttb, tableName);
    tc->databaseName = tdb;
    tc->tableName = ttb;
    tc->next = G_ContextHead;
    G_ContextHead = tc;
    G_OpenFlag++;

    (*handle) = (void *) G_ContextHead;

#ifdef CTN_USE_THREADS
    THR_ReleaseMutex(FAC_TBL);
#endif
    return TBL_NORMAL;
}



/* TBL_Close
**
** Purpose:
**	This function "closes" the specified table in the specified
**	database.
**
** Parameter Dictionary:
**	TBL_HANDLE **handle: The pointer for the database/table pair
**		to be closed.
**
** Return Values:
**	TBL_NORMAL: normal termination.
**	TBL_CLOSERROR: The handle to be closed could not be located
**		in the internal list or no database/table pairs
**		had been opened up to this point.
**
** Notes:
**	Nothing unusual.
**
** Algorithm:
**	Locates the handle specified in the call and removes that
**	entry from the internal list maintained by this	facility.
*/
CONDITION
TBL_Close(TBL_HANDLE ** handle)
{
    TBL_CONTEXT
	* prevtc,
	*tc,
	*hc;

#ifdef CTN_USE_THREADS
    THR_ObtainMutex(FAC_TBL);
#endif

    G_OpenFlag--;
    if (G_OpenFlag == 0) {
	dbclose(G_DBOpen);
	dbclose(G_DBInsert);
	dbclose(G_DBSelect);
	dbclose(G_DBDelete);
	dbclose(G_DBUpdate);
	dbclose(G_DBNextUnique);
	dbclose(G_DBLayout);
	dbexit();
    }
    hc = (TBL_CONTEXT *) (*handle);
    prevtc = tc = G_ContextHead;
    while (tc != (TBL_HANDLE *) NULL) {
	if (hc == tc) {
	    /* Large change 22-May-1998.  We use reference counting to keep */
	    /* tables open */
	    tc->refCount--;
	    if (tc->refCount > 0) {
		/* (*handle) = (TBL_HANDLE *) NULL; */
#ifdef CTN_USE_THREADS
		THR_ReleaseMutex(FAC_TBL);
#endif
		return TBL_NORMAL;
	    }
	    free(tc->databaseName);
	    free(tc->tableName);
	    if (tc == G_ContextHead)
		G_ContextHead = tc->next;
	    else
		prevtc->next = tc->next;
	    free(tc);
	    (*handle) = (TBL_HANDLE *) NULL;
#ifdef CTN_USE_THREADS
	    THR_ReleaseMutex(FAC_TBL);
#endif
	    return TBL_NORMAL;
	}
	prevtc = tc;
	tc = tc->next;
    }

#ifdef CTN_USE_THREADS
    THR_ReleaseMutex(FAC_TBL);
#endif
    return COND_PushCondition(TBL_ERROR(TBL_CLOSERROR), "TBL_Close");
}



/* TBL_Select
**
** Purpose:
**	This function selects some number of records (possibly zero),
**	that match the criteria specifications given in the input
**	parameter criteriaList.
**
** Parameter Dictionary:
**	TBL_HANDLE **handle: The pointer for the database/table pair
**		to be accessed.  This table must be open.
**	TBL_CRITERIA *criteriaList: Contains a list of the criteria
**		to use when selecting records from the specified table.
**		A null list implies that all records will be selected.
**	TBL_FIELD *fieldList: Contains a list of the fields to be
**		retreived from each record that matches the criteria
**		specification.  It is an error to specify a null
**		fieldList.
**	int *count: Contains a number that represents the total number
**		of records retreived by this particular select.  If this
**		parameter is null, then an internal counter is used and
**		the final count will not be returned when the select
**		finishes.
**	CONDITION (*callback)(): The callback function invoked whenever
**		a new record is retreived from the database.  It is
**		invoked with parameters as described below.
**	void *ctx: Ancillary data passed through to the callback function
**		and untouched by this routine.
**
** Return Values:
**	TBL_NORMAL: normal termination.
**	TBL_BADHANDLE: The handle passed to the routine was invalid.
**	TBL_DBNOEXIST: The database specified does not exist.
**	TBL_DBINITFAILED: The attempt to allocate another Sybase
**		process record failed.
**	TBL_NOFIELDLIST: A null field list pointer (fieldList *) was
**		specified.
**	TBL_SELECTFAILED: The select operation failed most probably from
**		a bad specification in the fieldList or criteriaList.  This
**		return is not the same as a valid query returning no records.
**		This error return could result from a misspelled keyword, etc.
**	TBL_EARLYEXIT: The callback routine returned something other than
**		TBL_NORMAL which caused this routine to cancel the remainder
**		of the database operation and return early.
**
** Algorithm:
**	As each record is retreived from the
**	database, the fields requested by the user (contained in
**	fieldList), are filled with the informatiton retreived from
**	the database and a pointer to the list is passed to the
**	callback routine designated by the input parameter callback.
**	The callback routine is invoked as follows:
**
**		callback(fieldList *fieldList, long count, void *ctx)
**
**	The count contains the number of records retreived to this point.
**	ctx contains any additional information the user originally passed
**	to this select function.  If callback returns any value other
**	than TBL_NORMAL, it is assumed that this function should terminate
**	(i.e. cancel the current db operation), and return an abnormal
**	termination message (TBL_EARLYEXIT) to the routine which
**	originally invoked the select.
*/
CONDITION
TBL_Select(TBL_HANDLE ** handle, const TBL_CRITERIA * criteriaList,
     TBL_FIELD * fieldList, long *count, CONDITION(*callback) (), void *ctx)
{
    TBL_CONTEXT
	* tc;
    TBL_FIELD
	* fp;
    const TBL_CRITERIA
    *   cp;
    char
       *dbName = "",
       *tableName = "";
    int
        i,
        foundit;
    long
        realcount,
       *lp;
    static int selCount = 1;

#ifdef CTN_USE_THREADS
    THR_ObtainMutex(FAC_TBL);
#endif

    tc = G_ContextHead;
    foundit = 0;
    while (tc != (TBL_CONTEXT *) NULL) {
	if (tc == (TBL_CONTEXT *) (*handle)) {
	    dbName = tc->databaseName;
	    tableName = tc->tableName;
	    foundit = 1;
	}
	tc = tc->next;
    }
    selCount++;
    if (!foundit) {
#ifdef CTN_USE_THREADS
	THR_ReleaseMutex(FAC_TBL);
#endif
	return COND_PushCondition(TBL_ERROR(TBL_BADHANDLE), "TBL_Select");
    }
    /*
     * We found the names we need...now make sure we can access it. Actually,
     * given that we found it, we probably already know the outcome of this
     * command...but we do need to set up the correct database for this
     * command anyway...
     */
    if (dbuse(G_DBSelect, dbName) != SUCCEED) {
#ifdef CTN_USE_THREADS
	THR_ReleaseMutex(FAC_TBL);
#endif
	return COND_PushCondition(TBL_ERROR(TBL_DBNOEXIST), dbName, "TBL_Select");
    }
    if ((fieldList == (TBL_FIELD *) NULL) || (fieldList->FieldName == 0)) {
	return COND_PushCondition(TBL_ERROR(TBL_NOFIELDLIST), "TBL_Select");
    }
    dbfreebuf(G_DBSelect);
    /*
     * Set up the select statement
     */
    dbcmd(G_DBSelect, "SELECT ");
    fp = fieldList;
    Add_Field_to_Select(G_DBSelect, fp);
    fp++;
    while (fp->FieldName != NULL) {
	dbcmd(G_DBSelect, " , ");
	Add_Field_to_Select(G_DBSelect, fp);
	fp++;
    }
    dbfcmd(G_DBSelect, " FROM %s ", tableName);
    cp = criteriaList;
    if ((cp != (TBL_CRITERIA *) NULL) && (cp->FieldName != 0)) {
	dbcmd(G_DBSelect, " WHERE ");
	Add_Criteria_to_Sybuf(G_DBSelect, cp);
	cp++;
	while (cp->FieldName != NULL) {
	    dbcmd(G_DBSelect, " AND ");
	    Add_Criteria_to_Sybuf(G_DBSelect, cp);
	    cp++;
	}
    }
    if (count != (long *) NULL)
	lp = count;
    else
	lp = &realcount;
    *lp = 0;

    if (dbsqlexec(G_DBSelect) == FAIL) {
	dbfreebuf(G_DBSelect);
#ifdef CTN_USE_THREADS
	THR_ReleaseMutex(FAC_TBL);
#endif
	return COND_PushCondition(TBL_ERROR(TBL_SELECTFAILED), DATABASE, "TBL_Select");
    }
    while (dbresults(G_DBSelect) != NO_MORE_RESULTS) {
	if (DBROWS(G_DBSelect) == SUCCEED) {
	    for (fp = fieldList, i = 1; fp->FieldName != NULL; fp++, i++) {
		switch (fp->Value.Type) {
		case TBL_SIGNED2:
		    dbbind(G_DBSelect, i, INTBIND,
			   (DBINT) fp->Value.AllocatedSize, (BYTE *) (fp->Value.Value.Signed2));
		    break;
		case TBL_UNSIGNED2:
		    dbbind(G_DBSelect, i, INTBIND,
			   (DBINT) fp->Value.AllocatedSize, (BYTE *) (fp->Value.Value.Unsigned2));
		    break;
		case TBL_SIGNED4:
		    dbbind(G_DBSelect, i, INTBIND,
			   (DBINT) fp->Value.AllocatedSize, (BYTE *) (fp->Value.Value.Signed4));
		    break;
		case TBL_UNSIGNED4:
		    dbbind(G_DBSelect, i, INTBIND,
			   (DBINT) fp->Value.AllocatedSize, (BYTE *) (fp->Value.Value.Unsigned4));
		    break;
		case TBL_FLOAT4:
		    dbbind(G_DBSelect, i, REALBIND,
			   (DBINT) fp->Value.AllocatedSize, (BYTE *) (fp->Value.Value.Float4));
		    break;
		case TBL_FLOAT8:
		    dbbind(G_DBSelect, i, FLT8BIND,
			   (DBINT) fp->Value.AllocatedSize, (BYTE *) (fp->Value.Value.Float8));
		    break;
		case TBL_STRING:
		    dbbind(G_DBSelect, i, NTBSTRINGBIND,
			   (DBINT) fp->Value.AllocatedSize, (BYTE *) (fp->Value.Value.String));
		    break;
		case TBL_TEXT:
		    dbbind(G_DBSelect, i, NTBSTRINGBIND,
			   (DBINT) fp->Value.AllocatedSize, (BYTE *) (fp->Value.Value.Text));
		    break;
		case TBL_BINARYDATA:
		    dbbind(G_DBSelect, i, BINARYBIND,
			   (DBINT) fp->Value.AllocatedSize, (BYTE *) (fp->Value.Value.BinaryData));
		    break;
		}
	    }
	    while (dbnextrow(G_DBSelect) != NO_MORE_ROWS) {
		(*lp)++;
		for (fp = fieldList, i = 1; fp->FieldName != NULL; fp++, i++) {
		    fp->Value.IsNull = 0;
		    switch (fp->Value.Type) {
		    case TBL_SIGNED2:
			fp->Value.Size = 2;
			if (*(fp->Value.Value.Signed2) == BIG_2) {
			    fp->Value.IsNull = 1;
			    fp->Value.Size = 0;
			}
			break;
		    case TBL_UNSIGNED2:
			fp->Value.Size = 2;
			if (*(fp->Value.Value.Unsigned2) == BIG_2) {
			    fp->Value.IsNull = 1;
			    fp->Value.Size = 0;
			}
			break;
		    case TBL_SIGNED4:
			fp->Value.Size = 4;
			if (*(fp->Value.Value.Signed4) == BIG_4) {
			    fp->Value.IsNull = 1;
			    fp->Value.Size = 0;
			}
			break;
		    case TBL_UNSIGNED4:
			fp->Value.Size = 4;
			if (*(fp->Value.Value.Unsigned4) == BIG_4) {
			    fp->Value.IsNull = 1;
			    fp->Value.Size = 0;
			}
			break;
		    case TBL_FLOAT4:
			fp->Value.Size = 4;
			if (*(fp->Value.Value.Float4) == BIG_4) {
			    fp->Value.IsNull = 1;
			    fp->Value.Size = 0;
			}
			break;
		    case TBL_FLOAT8:
			fp->Value.Size = 8;
			if (*(fp->Value.Value.Float8) == BIG_4) {
			    fp->Value.IsNull = 1;
			    fp->Value.Size = 0;
			}
			break;
		    case TBL_STRING:
			fp->Value.Size = fp->Value.AllocatedSize;
			if (strcmp(fp->Value.Value.String, "") == 0) {
			    fp->Value.IsNull = 1;
			    fp->Value.Size = 0;
			}
			break;
		    case TBL_TEXT:
		    case TBL_BINARYDATA:
			fp->Value.Size = fp->Value.AllocatedSize;
			if (((DBBINARY *) dbtxptr(G_DBSelect, i)) == ((DBBINARY *) NULL)) {
			    fp->Value.IsNull = 1;
			    fp->Value.Size = 0;
			}
			break;
		    }
		}
		if (callback != NULL) {
		    if (callback(fieldList, *lp, ctx) != TBL_NORMAL) {
			dbcancel(G_DBSelect);
			dbfreebuf(G_DBSelect);
#ifdef CTN_USE_THREADS
			THR_ReleaseMutex(FAC_TBL);
#endif
			return COND_PushCondition(TBL_ERROR(TBL_EARLYEXIT), "TBL_Select");
		    }
		}
	    }
	}
    }

#ifdef CTN_USE_THREADS
    THR_ReleaseMutex(FAC_TBL);
#endif
    return TBL_NORMAL;
}


/* TBL_Update
**
** Purpose:
**	This updates existing records in the named table.
**
** Parameter Dictionary:
**	TBL_HANDLE **handle: The pointer for the database/table pair
**		to be accessed for modification.  This table must be open.
**	TBL_CRITERIA *criteriaList: Contains the list of criteria to
**		select those records that should be updated.
**	TBL_FIELD *fieldList: Contains a list of the keyword/value
**		pairs to be used to modify the selected records.
**
** Return Values:
**	TBL_NORMAL: normal termination.
**	TBL_BADHANDLE: The handle passed to the routine was invalid.
**	TBL_DBNOEXIST: The database specified does not exist.
**	TBL_NOFIELDLIST: No keyword/value pairs were specified for update.
**	TBL_UPDATEFAILED: The insert operation failed most probably from
**		a bad specification in the fieldList.  This error
**		return could result from a misspelled keyword, etc.
**
** Notes:
**	Nothing unusual.
**
** Algorithm:
**	The records which match the (ANDED) criteria in criteriaList
**	are retreived and updated with the information contained in
**	fieldList.  Only the fields contained in fieldList will be
**	updated by this call.
*/
CONDITION
TBL_Update(TBL_HANDLE ** handle, const TBL_CRITERIA * criteriaList,
	   TBL_UPDATE * updateList)
{
    TBL_CONTEXT
	* tc;
    TBL_UPDATE
	* up;
    const TBL_CRITERIA
    *   cp;
    char
        tabcol[100],
       *dbName,
       *tableName;
    int
        i,
        ret,
        first,
        FoundTextorBinary,
        foundit;
    DBBINARY
	* bin_ptr;

    tc = G_ContextHead;
    foundit = 0;
    while (tc != (TBL_CONTEXT *) NULL) {
	if (tc == (TBL_CONTEXT *) (*handle)) {
	    dbName = tc->databaseName;
	    tableName = tc->tableName;
	    foundit = 1;
	}
	tc = tc->next;
    }
    if (!foundit) {
	return COND_PushCondition(TBL_ERROR(TBL_BADHANDLE), "TBL_Update");
    }
    /*
     * We found the names we need...now make sure we can access it. Actually,
     * given that we found it, we probably already know the outcome of this
     * command...but we do need to set up the correct database for this
     * update...
     */
    if (dbuse(G_DBUpdate, dbName) != SUCCEED) {
	return COND_PushCondition(TBL_ERROR(TBL_DBNOEXIST), dbName, "TBL_Update");
    }
    if (updateList == (TBL_UPDATE *) NULL) {
	return COND_PushCondition(TBL_ERROR(TBL_NOFIELDLIST), "TBL_Update");
    }
    dbfreebuf(G_DBUpdate);
    /*
     * Set up the update statement
     */
    up = updateList;
    dbfcmd(G_DBUpdate, "UPDATE %s SET ", tableName);
    first = 1;
    foundit = 0;
    FoundTextorBinary = 0;
    while (up->FieldName != NULL) {
	if (up->Value.Type != TBL_TEXT && up->Value.Type != TBL_BINARYDATA) {
	    if (!first)
		dbcmd(G_DBUpdate, " , ");
	    dbfcmd(G_DBUpdate, " %s = ", up->FieldName);
	    Add_UpdateValue_to_Sybuf(G_DBUpdate, up);
	    first = 0;
	    foundit = 1;
	} else {
	    FoundTextorBinary = 1;
	}
	up++;
    }
    if (foundit) {
	if ((cp = criteriaList) != (TBL_CRITERIA *) NULL) {
	    dbcmd(G_DBUpdate, " WHERE ");
	    Add_Criteria_to_Sybuf(G_DBUpdate, cp);
	    cp++;
	    while (cp->FieldName != NULL) {
		dbcmd(G_DBUpdate, " AND ");
		Add_Criteria_to_Sybuf(G_DBUpdate, cp);
		cp++;
	    }
	}
	if (dbsqlexec(G_DBUpdate) == FAIL) {
	    dbfreebuf(G_DBUpdate);
	    return COND_PushCondition(TBL_ERROR(TBL_UPDATEFAILED), DATABASE, "TBL_Update");
	}
    }
    dbfreebuf(G_DBUpdate);
    if (FoundTextorBinary) {
	up = updateList;
	dbcmd(G_DBUpdate, "SELECT ");
	first = 1;
	while (up->FieldName != NULL) {
	    if (up->Value.Type == TBL_TEXT || up->Value.Type == TBL_BINARYDATA) {
		if (!first)
		    dbcmd(G_DBUpdate, " , ");
		dbfcmd(G_DBUpdate, " %s ", up->FieldName);
		first = 0;
	    }
	    up++;
	}
	first = 1;
	dbfcmd(G_DBUpdate, " FROM %s ", tableName);
	if ((cp = criteriaList) != (TBL_CRITERIA *) NULL) {
	    dbcmd(G_DBUpdate, " WHERE ");
	    Add_Criteria_to_Sybuf(G_DBUpdate, cp);
	    cp++;
	    while (cp->FieldName != NULL) {
		dbcmd(G_DBUpdate, " AND ");
		Add_Criteria_to_Sybuf(G_DBUpdate, cp);
		cp++;
	    }
	}
	if (dbsqlexec(G_DBUpdate) == FAIL) {
	    dbcancel(G_DBUpdate);
	    return COND_PushCondition(TBL_ERROR(TBL_UPDATEFAILED), DATABASE, "TBL_Update");
	}
	/*
	 * Now we have done the select on the TEXT and BINARYDATA fields...so
	 * we need to rewrite that data...
	 */
	/*
	 * Now retreive the rows..get the text pointer...and copy the data
	 * from the users buffer to the database.
	 */
	if (dbresults(G_DBUpdate) != NO_MORE_RESULTS) {
	    if (DBROWS(G_DBUpdate) == SUCCEED) {
		while (dbnextrow(G_DBUpdate) != NO_MORE_ROWS) {
		    i = 1;
		    ret = SUCCEED;
		    for (up = updateList; up->FieldName != 0; up++) {
			if (up->Value.Type == TBL_TEXT) {
			    sprintf(tabcol, "%s.%s", tableName, up->FieldName);
			    bin_ptr = dbtxptr(G_DBUpdate, i);
			    ret = dbwritetext(G_DBUpdate, tabcol, bin_ptr, DBTXPLEN,
					 dbtxtimestamp(G_DBUpdate, i), TRUE,
					    (DBINT) up->Value.AllocatedSize,
					   (BYTE *) (up->Value.Value.Text));
			    i++;
			} else if (up->Value.Type == TBL_BINARYDATA) {
			    sprintf(tabcol, "%s.%s", tableName, up->FieldName);
			    bin_ptr = dbtxptr(G_DBUpdate, i);
			    ret = dbwritetext(G_DBUpdate, tabcol, bin_ptr, DBTXPLEN,
					 dbtxtimestamp(G_DBUpdate, i), TRUE,
					    (DBINT) up->Value.AllocatedSize,
				     (BYTE *) (up->Value.Value.BinaryData));
			    i++;
			}
			if (ret != SUCCEED) {
			    dbcancel(G_DBUpdate);
			    return COND_PushCondition(TBL_ERROR(TBL_UPDATEFAILED), DATABASE, "TBL_Update");
			}
		    }
		}
	    }
	}
    }
    return TBL_NORMAL;
}

/* TBL_NextUnique
**
** Purpose:
**	This routine retrieves the next unique number from a predefined table.
**
** Parameter Dictionary:
**	TBL_HANDLE **handle: The pointer for the database/table pair
**		to be accessed for modification.  This table must be open.
**	char *name : The name of the unique variable to access.
**	int *unique: Contains the next unique number in the sequence upon
**			return.
**
** Return Values:
**	TBL_NORMAL: normal termination.
**	TBL_BADHANDLE: The handle passed to the routine was invalid.
**	TBL_DBNOEXIST: The database specified does not exist.
**	TBL_SELECTFAILED: The unique number name could not be found.
**	TBL_UPDATEFAILED: The unique number could not be incremented.
**
** Notes:
**	Nothing unusual.
**
** Algorithm:
**	The unique number associated with  "name" in the opened table is
**	retrieved and passed back to the user in "unique".  This number is
**	then incremented (by one) and placed back in the table in preparation
**	for the next call to this routine.
*/
CONDITION
TBL_NextUnique(TBL_HANDLE ** handle, char *name, int *unique)
{
    TBL_CONTEXT
	* tc;
    char
       *dbName,
       *tableName;
    int
        i,
        ret,
        num,
        foundit;

    tc = G_ContextHead;
    foundit = 0;
    while (tc != (TBL_CONTEXT *) NULL) {
	if (tc == (TBL_CONTEXT *) (*handle)) {
	    dbName = tc->databaseName;
	    tableName = tc->tableName;
	    foundit = 1;
	}
	tc = tc->next;
    }
    if (!foundit) {
	return COND_PushCondition(TBL_ERROR(TBL_BADHANDLE), "TBL_NextUnique");
    }
    /*
     * We found the names we need...now make sure we can access it. Actually,
     * given that we found it, we probably already know the outcome of this
     * command...but we do need to set up the correct database for this
     * update...
     */
    if (dbuse(G_DBNextUnique, dbName) != SUCCEED) {
	return COND_PushCondition(TBL_ERROR(TBL_DBNOEXIST), dbName, "TBL_NextUnique");
    }
    dbfreebuf(G_DBNextUnique);

    dbcmd(G_DBNextUnique, "BEGIN TRANSACTION UNIQUEUPDATE");
    if (dbsqlexec(G_DBNextUnique) == FAIL) {
	dbcancel(G_DBNextUnique);
	return COND_PushCondition(TBL_ERROR(TBL_SELECTFAILED), DATABASE, "TBL_NextUnique");
    }
    dbfcmd(G_DBNextUnique, "SELECT UniqueNumber FROM %s WHERE NumberName=\"%s\" ", tableName, name);
    if (dbsqlexec(G_DBNextUnique) == FAIL) {
	dbcancel(G_DBNextUnique);
	return COND_PushCondition(TBL_ERROR(TBL_SELECTFAILED), DATABASE, "TBL_NextUnique");
    }
    *unique = -1;
    if (dbresults(G_DBNextUnique) != NO_MORE_RESULTS) {
	dbbind(G_DBNextUnique, 1, INTBIND, (DBINT) 4, (BYTE *) & num);
	if (DBROWS(G_DBNextUnique) == SUCCEED) {
	    while (dbnextrow(G_DBNextUnique) != NO_MORE_ROWS) {
		*unique = num;
	    }
	} else {
	    dbcancel(G_DBNextUnique);
	    return COND_PushCondition(TBL_ERROR(TBL_SELECTFAILED), DATABASE, "TBL_NextUnique");
	}
    } else {
	dbcancel(G_DBNextUnique);
	return COND_PushCondition(TBL_ERROR(TBL_SELECTFAILED), DATABASE, "TBL_NextUnique");
    }
    /*
     * Now perform the update...
     */
    dbfcmd(G_DBNextUnique, "UPDATE %s SET UniqueNumber=UniqueNumber+1 WHERE NumberName=\"%s\"", tableName, name);

    if (dbsqlexec(G_DBNextUnique) == FAIL) {
	dbcancel(G_DBNextUnique);
	dbfreebuf(G_DBNextUnique);
	return COND_PushCondition(TBL_ERROR(TBL_UPDATEFAILED), DATABASE, "TBL_NextUnique");
    }
    dbcmd(G_DBNextUnique, "COMMIT TRANSACTION UNIQUEUPDATE");
    if (dbsqlexec(G_DBNextUnique) == FAIL) {
	dbcancel(G_DBNextUnique);
	dbfreebuf(G_DBNextUnique);
	return COND_PushCondition(TBL_ERROR(TBL_SELECTFAILED), DATABASE, "TBL_NextUnique");
    }
    return TBL_NORMAL;
}

CONDITION
TBL_NextSequence(TBL_HANDLE ** handle, char *name, long *unique)
{
    return COND_PushCondition(TBL_ERROR(TBL_UNIMPLEMENTED), "TBL_NextSequence");
}


/* TBL_Insert
**
** Purpose:
**	This inserts records into the named table.
**
** Parameter Dictionary:
**	TBL_HANDLE **handle: The pointer for the database/table pair
**		to be accessed for deletion.  This table must be open.
**	TBL_FIELD *fieldList: Contains a list of the keyword/value
**		pairs to be inserted into the specified table.
**
** Return Values:
**	TBL_NORMAL: normal termination.
**	TBL_BADHANDLE: The handle passed to the routine was invalid.
**	TBL_DBNOEXIST: The database specified does not exist.
**	TBL_NOFIELDLIST: No keyword/value pairs were specified to
**		insert.
**	TBL_INSERTFAILED: The insert operation failed most probably from
**		a bad specification in the fieldList.  This error
**		return could result from a misspelled keyword, etc.
**
** Notes:
**	Nothing unusual.
**
** Algorithm:
**	The table values contained in fieldList are added to the
**	database and table specified by handle.  Each call inserts
**	exactly one record.  It is the users responsibility to ensure
**	that the correct number of values are supplied for the
**	particular table, and that any values which need to be
**	unique (i.e.for the unique key field in a table) are
**	in-fact unique.
*/
CONDITION
TBL_Insert(TBL_HANDLE ** handle, TBL_FIELD * fieldList)
{
    TBL_CONTEXT
	* tc;
    TBL_FIELD
	* fp;
    char
        tabcol[100],
       *dbName,
       *tableName;
    int
        i,
        ret,
        FoundTextorBinary,
        foundit;
    DBBINARY
	* bin_ptr;

    tc = G_ContextHead;
    foundit = 0;
    while (tc != (TBL_CONTEXT *) NULL) {
	if (tc == (TBL_CONTEXT *) (*handle)) {
	    dbName = tc->databaseName;
	    tableName = tc->tableName;
	    foundit = 1;
	}
	tc = tc->next;
    }
    if (!foundit) {
	return COND_PushCondition(TBL_ERROR(TBL_BADHANDLE), "TBL_Insert");
    }
    /*
     * We found the names we need...now make sure we can access it. Actually,
     * given that we found it, we probably already know the outcome of this
     * command...but we do need to set up the correct database for this
     * insert...
     */
    if (dbuse(G_DBInsert, dbName) != SUCCEED) {
	return COND_PushCondition(TBL_ERROR(TBL_DBNOEXIST), dbName, "TBL_Insert");
    }
    fp = fieldList;
    if (fp == (TBL_FIELD *) NULL) {
	return COND_PushCondition(TBL_ERROR(TBL_NOFIELDLIST), "TBL_Insert");
    }
    dbfreebuf(G_DBInsert);
    /*
     * Set up the insert statement
     */
    FoundTextorBinary = 0;
    dbfcmd(G_DBInsert, "INSERT %s ( ", tableName);
    dbcmd(G_DBInsert, fp->FieldName);
    if (fp->Value.Type == TBL_TEXT || fp->Value.Type == TBL_BINARYDATA)
	FoundTextorBinary = 1;
    fp++;
    while (fp->FieldName != NULL) {
	dbcmd(G_DBInsert, " , ");
	dbcmd(G_DBInsert, fp->FieldName);
	if (fp->Value.Type == TBL_TEXT || fp->Value.Type == TBL_BINARYDATA)
	    FoundTextorBinary = 1;
	fp++;
    }
    dbcmd(G_DBInsert, " ) VALUES ( ");

    fp = fieldList;
    Add_Value_to_Sybuf(G_DBInsert, fp);
    fp++;
    while (fp->FieldName != NULL) {
	dbcmd(G_DBInsert, " , ");
	Add_Value_to_Sybuf(G_DBInsert, fp);
	fp++;
    }

    dbcmd(G_DBInsert, " )");

    if (dbsqlexec(G_DBInsert) == FAIL) {
	dbfreebuf(G_DBInsert);
	return COND_PushCondition(TBL_ERROR(TBL_INSERTFAILED), DATABASE, "TBL_Insert");
    }
    dbfreebuf(G_DBInsert);

    if (FoundTextorBinary) {
	int first;
	first = 1;
	dbcmd(G_DBInsert, "SELECT ");
	for (fp = fieldList; fp->FieldName != 0; fp++) {
	    if (fp->Value.Type == TBL_TEXT || fp->Value.Type == TBL_BINARYDATA) {
		if (!first)
		    dbcmd(G_DBInsert, " , ");
		dbfcmd(G_DBInsert, " %s ", fp->FieldName);
		first = 0;
	    }
	}
	first = 1;
	dbfcmd(G_DBInsert, " FROM %s WHERE ", tableName);
	for (fp = fieldList; fp->FieldName != 0; fp++) {
	    if (fp->Value.Type != TBL_TEXT && fp->Value.Type != TBL_BINARYDATA) {
		if (!first)
		    dbcmd(G_DBInsert, " AND ");
		dbfcmd(G_DBInsert, " %s = ", fp->FieldName);
		Add_Value_to_Sybuf(G_DBInsert, fp);
		first = 0;
	    }
	}
	/*
	 * Now retreive the rows..get the text pointer...and copy the data
	 * from the users buffer to the database.
	 */
	if (dbsqlexec(G_DBInsert) == FAIL) {
	    dbcancel(G_DBInsert);
	    return COND_PushCondition(TBL_ERROR(TBL_INSERTFAILED), DATABASE, "TBL_Insert");
	}
	if (dbresults(G_DBInsert) != NO_MORE_RESULTS) {
	    if (DBROWS(G_DBInsert) == SUCCEED) {
		while (dbnextrow(G_DBInsert) != NO_MORE_ROWS) {
		    i = 1;
		    ret = SUCCEED;
		    for (fp = fieldList; fp->FieldName != 0; fp++) {
			if (fp->Value.Type == TBL_TEXT) {
			    sprintf(tabcol, "%s.%s", tableName, fp->FieldName);
			    bin_ptr = dbtxptr(G_DBInsert, i);
			    ret = dbwritetext(G_DBInsert, tabcol, bin_ptr, DBTXPLEN,
					 dbtxtimestamp(G_DBInsert, i), TRUE,
					    (DBINT) fp->Value.AllocatedSize,
					   (BYTE *) (fp->Value.Value.Text));
			    i++;
			} else if (fp->Value.Type == TBL_BINARYDATA) {
			    sprintf(tabcol, "%s.%s", tableName, fp->FieldName);
			    bin_ptr = dbtxptr(G_DBInsert, i);
			    ret = dbwritetext(G_DBInsert, tabcol, bin_ptr, DBTXPLEN,
					 dbtxtimestamp(G_DBInsert, i), TRUE,
					    (DBINT) fp->Value.AllocatedSize,
				     (BYTE *) (fp->Value.Value.BinaryData));
			    i++;
			}
			if (ret != SUCCEED) {
			    dbcancel(G_DBInsert);
			    return COND_PushCondition(TBL_ERROR(TBL_INSERTFAILED), DATABASE, "TBL_Insert");
			}
		    }
		}
	    }
	}
    }
    return TBL_NORMAL;
}
/*
** INTERNAL USE FUNCTIONS **
*/
void
TBL_BeginInsertTransaction(void)
{

    dbfreebuf(G_DBInsert);
    /*
     * Set up the transaction statement
     */
    dbcmd(G_DBInsert, "BEGIN TRANSACTION TBLINSERT");
    dbsqlexec(G_DBInsert);

    return;
}
void
TBL_CommitInsertTransaction(void)
{

    dbfreebuf(G_DBInsert);
    /*
     * Set up the transaction statement
     */
    dbcmd(G_DBInsert, "COMMIT TRANSACTION TBLINSERT");
    dbsqlexec(G_DBInsert);

    return;
}
void
TBL_RollbackInsertTransaction(void)
{

    dbfreebuf(G_DBInsert);
    /*
     * Set up the transaction statement
     */
    dbcmd(G_DBInsert, "ROLLBACK TRANSACTION TBLINSERT");
    dbsqlexec(G_DBInsert);

    return;
}



/* TBL_Delete
**
** Purpose:
**	This deletes the records specified from the indicated table.
**
** Parameter Dictionary:
**	TBL_HANDLE **handle: The pointer for the database/table pair
**		to be accessed for deletion.  This table must be open.
**	TBL_CRITERIA *criteriaList: Contains a list of the criteria
**		to use when deleting records from the specified table.
**		A null list implies that all records will be deleted.
**
** Return Values:
**	TBL_NORMAL: normal termination.
**	TBL_BADHANDLE: The handle passed to the routine was invalid.
**	TBL_DBNOEXIST: The database specified does not exist.
**	TBL_DELETEFAILED: The delete operation failed most probably from
**		a bad specification in the criteriaList.  This error
**		return could result from a misspelled keyword, etc.
**
** Notes:
**	Nothing unusual.
**
** Algorithm:
**	The records selected by criteriaList are removed from the
**	database/table indicated by handle.
*/
CONDITION
TBL_Delete(TBL_HANDLE ** handle, const TBL_CRITERIA * criteriaList)
{
    TBL_CONTEXT
	* tc;
    const TBL_CRITERIA
    *   cp;
    char
       *dbName,
       *tableName;
    int
        i,
        foundit;

    tc = G_ContextHead;
    foundit = 0;
    while (tc != (TBL_CONTEXT *) NULL) {
	if (tc == (TBL_CONTEXT *) (*handle)) {
	    dbName = tc->databaseName;
	    tableName = tc->tableName;
	    foundit = 1;
	}
	tc = tc->next;
    }
    if (!foundit) {
	return COND_PushCondition(TBL_ERROR(TBL_BADHANDLE), "TBL_Delete");
    }
    /*
     * We found the names we need...now make sure we can access it. Actually,
     * given that we found it, we probably already know the outcome of this
     * command...but we do need to set up the correct database for this
     * delete...
     */
    if (dbuse(G_DBDelete, dbName) != SUCCEED) {
	return COND_PushCondition(TBL_ERROR(TBL_DBNOEXIST), dbName, "TBL_Delete");
    }
    dbfreebuf(G_DBDelete);
    /*
     * Set up the delete statement
     */
    dbcmd(G_DBDelete, "DELETE ");
    cp = criteriaList;
    if (cp == (TBL_CRITERIA *) NULL || cp->FieldName == NULL) {
	dbcmd(G_DBDelete, tableName);
    } else {
	dbfcmd(G_DBDelete, " FROM %s WHERE ", tableName);
	Add_Criteria_to_Sybuf(G_DBDelete, cp);
	cp++;
	while (cp != NULL && cp->FieldName != NULL) {
	    dbcmd(G_DBDelete, " AND ");
	    Add_Criteria_to_Sybuf(G_DBDelete, cp);
	    cp++;
	}
    }
    if (dbsqlexec(G_DBDelete) == FAIL) {
	dbfreebuf(G_DBDelete);
	return COND_PushCondition(TBL_ERROR(TBL_DELETEFAILED), DATABASE, "TBL_Delete");
    }
    dbfreebuf(G_DBDelete);
    return TBL_NORMAL;
}


/* TBL_Layout
**
** Purpose:
**	This function returns the columns and types of a particular
**	table specified by handle.
**
** Parameter Dictionary:
**	char *databaseName: The name of the database to use.
**	char *tableName: The name of the table to access.
**	CONDITION (*callback)(): The callback function invoked whenever
**		a new record is retreived from the database.  It is
**		invoked with parameters as described below.
**	void *ctx: Ancillary data passed through to the callback function
**		and untouched by this routine.
**
** Return Values:
**	TBL_NORMAL: normal termination.
**	TBL_BADHANDLE: The handle passed to the routine was invalid.
**	TBL_NOCALLBACK: No callback function was specified.
**	TBL_DBNOEXIST: The database specified does not exist.
**	TBL_SELECTFAILED: The select operation failed most probably from
**		a bad specification in the fieldList or criteriaList.  This
**		return is not the same as a valid query returning no records.
**		This error return could result from a misspelled keyword, etc.
**	TBL_TBLNOEXIST: The table specified did not exist in the correct
**		internal database table...this may indicate some sort
**		of consistency problem withing the database.
**	TBL_NOCOLUMNS: The table specified contains no columnns.
**	TBL_EARLYEXIT: The callback routine returned something other than
**		TBL_NORMAL which caused this routine to cancel the remainder
**		of the database operation and return early.
**
** Notes:
**	It is an error to specify a null callback function.
**
** Algorithm:
**	As each column is retrieved from the specified table, the
**	callback function is invoked as follows:
**
**		callback(fieldList *fieldList, void *ctx)
**
**	fieldList contains the field name and the type of the column from
**	the table specified.
**	ctx contains any additional information the user originally passed
**	to this layout function.  If callback returns any value other
**	than TBL_NORMAL, it is assumed that this function should terminate
**	(i.e. cancel the current db operation), and return an abnormal
**	termination message (TBL_EARLYEXIT) to the routine which
**	originally invoked TBL_Layout.
*/
CONDITION
TBL_Layout(char *databaseName, char *tableName, CONDITION(*callback) (), void *ctx) {
    TBL_FIELD
	field;
    char
        column_name[DBMAXCOLNAME + 1];
    int
        i,
        tableid,
        system_type,
        user_length,
        foundit;

    if (callback == NULL) {
	return COND_PushCondition(TBL_ERROR(TBL_NOCALLBACK), "TBL_Layout");
    }
    /*
     * We found the names we need...now make sure we can access it. Actually,
     * given that we found it, we probably already know the outcome of this
     * command...but we do need to set up the correct database for this
     * command anyway...
     */
    if (dbuse(G_DBLayout, databaseName) != SUCCEED) {
	return COND_PushCondition(TBL_ERROR(TBL_DBNOEXIST), databaseName, "TBL_Layout");
    }
    dbfreebuf(G_DBLayout);
    /*
     * Set up the select statement
     */
    dbfcmd(G_DBLayout, "SELECT id from sysobjects where name=\"%s\" and type=\"U\"", tableName);
    if (dbsqlexec(G_DBLayout) == FAIL) {
	dbfreebuf(G_DBLayout);
	return COND_PushCondition(TBL_ERROR(TBL_SELECTFAILED), DATABASE, "TBL_Layout");
    }
    if (dbresults(G_DBLayout) != NO_MORE_RESULTS) {
	if (DBROWS(G_DBLayout) == SUCCEED) {
	    dbbind(G_DBLayout, 1, INTBIND, (DBINT) 4, (BYTE *) (&tableid));
	    /*
	     * There should only be one here...
	     */
	    while (dbnextrow(G_DBLayout) != NO_MORE_ROWS);
	} else
	    return COND_PushCondition(TBL_ERROR(TBL_TBLNOEXIST), tableName, "TBL_Layout");
    } else
	return COND_PushCondition(TBL_ERROR(TBL_TBLNOEXIST), tableName, "TBL_Layout");
    /*
     * Now lets find each column and type for that table...
     */
    dbfcmd(G_DBLayout, "SELECT name,type,length from syscolumns where id=%d", tableid);
    if (dbsqlexec(G_DBLayout) == FAIL) {
	dbfreebuf(G_DBLayout);
	return COND_PushCondition(TBL_ERROR(TBL_SELECTFAILED), DATABASE, "TBL_Layout");
    }
    field.FieldName = column_name;
    if (dbresults(G_DBLayout) != NO_MORE_RESULTS) {
	if (DBROWS(G_DBLayout) == SUCCEED) {
	    dbbind(G_DBLayout, 1, NTBSTRINGBIND, (DBINT) DBMAXCOLNAME + 1, (BYTE *) (column_name));
	    dbbind(G_DBLayout, 2, INTBIND, (DBINT) 4, (BYTE *) (&system_type));
	    dbbind(G_DBLayout, 3, INTBIND, (DBINT) 4, (BYTE *) (&user_length));

	    while (dbnextrow(G_DBLayout) != NO_MORE_ROWS) {
		field.Value.AllocatedSize = user_length;
		switch (system_type) {
		case SYBBINARY:
		case SYBVARBINARY:
		case SYBIMAGE:
		    field.Value.Type = TBL_BINARYDATA;
		    break;
		case SYBTEXT:
		    field.Value.Type = TBL_TEXT;
		    break;
		case SYBVARCHAR:
		case SYBCHAR:
		    field.Value.Type = TBL_STRING;
		    break;
		case SYBINT2:
		    field.Value.Type = TBL_SIGNED2;
		    break;
		case SYBINTN:
		case SYBINT4:
		    field.Value.Type = TBL_SIGNED4;
		    break;
		case SYBFLTN:
		case SYBREAL:
		    field.Value.Type = TBL_FLOAT4;
		    break;
		case SYBFLT8:
		    field.Value.Type = TBL_FLOAT8;
		    break;
		default:
		    field.Value.Type = TBL_OTHER;
		    break;
		}
		if (callback != NULL) {
		    if (callback(&field, ctx) != TBL_NORMAL) {
			dbcancel(G_DBLayout);
			dbfreebuf(G_DBLayout);
			return COND_PushCondition(TBL_ERROR(TBL_EARLYEXIT), "TBL_Layout");
		    }
		}
	    }
	} else
	    return COND_PushCondition(TBL_ERROR(TBL_NOCOLUMNS), "TBL_Layout");
    } else
	return COND_PushCondition(TBL_ERROR(TBL_NOCOLUMNS), "TBL_Layout");

    return TBL_NORMAL;
}



/*
** INTERNAL USE FUNCTION **
*/
static void
Add_Field_to_Select(DBPROCESS * DBSelect, TBL_FIELD * fp)
{

    if (fp->Value.Type != TBL_BINARYDATA && fp->Value.Type != TBL_TEXT) {
	dbfcmd(DBSelect, " isnull(%s,", fp->FieldName);
	switch (fp->Value.Type) {
	case TBL_SIGNED2:
	case TBL_UNSIGNED2:
	    dbfcmd(DBSelect, "%d)", BIG_2);
	    break;
	case TBL_SIGNED4:
	case TBL_UNSIGNED4:
	case TBL_FLOAT4:
	case TBL_FLOAT8:
	    dbfcmd(DBSelect, "%d)", BIG_4);
	    break;
	case TBL_STRING:
	    dbfcmd(DBSelect, "\"\")");
	    break;
	}
    } else {
	dbfcmd(DBSelect, " %s ", fp->FieldName);
    }
    return;
}
/*
** INTERNAL USE FUNCTION **
*/
static void
Add_UpdateValue_to_Sybuf(DBPROCESS * DBproc, TBL_UPDATE * up)
{

    if (up->Function == TBL_SET) {
	switch (up->Value.Type) {
	    case TBL_SIGNED2:
	    dbfcmd(DBproc, "%d", *(up->Value.Value.Signed2));
	    break;
	case TBL_UNSIGNED2:
	    dbfcmd(DBproc, "%d", *(up->Value.Value.Unsigned2));
	    break;
	case TBL_SIGNED4:
	    dbfcmd(DBproc, "%d", *(up->Value.Value.Signed4));
	    break;
	case TBL_UNSIGNED4:
	    dbfcmd(DBproc, "%d", *(up->Value.Value.Unsigned4));
	    break;
	case TBL_FLOAT4:
	    dbfcmd(DBproc, "%f", *(up->Value.Value.Float4));
	    break;
	case TBL_FLOAT8:
	    dbfcmd(DBproc, "%f", *(up->Value.Value.Float8));
	    break;
	case TBL_STRING:
	    dbfcmd(DBproc, "\"%s\"", up->Value.Value.String);
	    break;
	    /*
	     * These two types are simply initialized with something
	     * (anything) so that the database will initialize the internal
	     * addresses of these fields.
	     */
	case TBL_TEXT:
	    dbcmd(DBproc, "\"FILLER-WILL BE REPLACED\"");
	    break;
	case TBL_BINARYDATA:
	    dbcmd(DBproc, "0xFFFFFFFF");
	    break;
	}
    } else if (up->Function == TBL_ZERO)
	dbfcmd(DBproc, " 0 ");
    else if (up->Function == TBL_INCREMENT)
	dbfcmd(DBproc, " %s + 1 ", up->FieldName);
    else if (up->Function == TBL_DECREMENT)
	dbfcmd(DBproc, " %s - 1 ", up->FieldName);
    else if (up->Function == TBL_ADD) {
	switch (up->Value.Type) {
	case TBL_SIGNED2:
	    dbfcmd(DBproc, " %s + %d ", up->FieldName, *(up->Value.Value.Signed2));
	    break;
	case TBL_SIGNED4:
	    dbfcmd(DBproc, " %s + %d ", up->FieldName, *(up->Value.Value.Signed4));
	    break;
	case TBL_UNSIGNED2:
	    dbfcmd(DBproc, " %s + %d ", up->FieldName, *(up->Value.Value.Unsigned2));
	    break;
	case TBL_UNSIGNED4:
	    dbfcmd(DBproc, " %s + %d ", up->FieldName, *(up->Value.Value.Unsigned4));
	    break;
	case TBL_FLOAT4:
	    dbfcmd(DBproc, " %s + %.6f ", up->FieldName, *(up->Value.Value.Float4));
	    break;
	case TBL_FLOAT8:
	    dbfcmd(DBproc, " %s + %.6f ", up->FieldName, *(up->Value.Value.Float8));
	    break;
	}
    } else if (up->Function == TBL_SUBTRACT) {
	switch (up->Value.Type) {
	case TBL_SIGNED2:
	    dbfcmd(DBproc, " %s - %d ", up->FieldName, *(up->Value.Value.Signed2));
	    break;
	case TBL_SIGNED4:
	    dbfcmd(DBproc, " %s - %d ", up->FieldName, *(up->Value.Value.Signed4));
	    break;
	case TBL_UNSIGNED2:
	    dbfcmd(DBproc, " %s - %d ", up->FieldName, *(up->Value.Value.Unsigned2));
	    break;
	case TBL_UNSIGNED4:
	    dbfcmd(DBproc, " %s - %d ", up->FieldName, *(up->Value.Value.Unsigned4));
	    break;
	case TBL_FLOAT4:
	    dbfcmd(DBproc, " %s - %.6f ", up->FieldName, *(up->Value.Value.Float4));
	    break;
	case TBL_FLOAT8:
	    dbfcmd(DBproc, " %s - %.6f ", up->FieldName, *(up->Value.Value.Float8));
	    break;
	}
    }
    return;
}
/*
** INTERNAL USE FUNCTION **
*/
static void
Add_Value_to_Sybuf(DBPROCESS * DBproc, TBL_FIELD * fp)
{

    switch (fp->Value.Type) {
	case TBL_SIGNED2:
	dbfcmd(DBproc, "%d", *(fp->Value.Value.Signed2));
	break;
    case TBL_UNSIGNED2:
	dbfcmd(DBproc, "%d", *(fp->Value.Value.Unsigned2));
	break;
    case TBL_SIGNED4:
	dbfcmd(DBproc, "%d", *(fp->Value.Value.Signed4));
	break;
    case TBL_UNSIGNED4:
	dbfcmd(DBproc, "%d", *(fp->Value.Value.Unsigned4));
	break;
    case TBL_FLOAT4:
	dbfcmd(DBproc, "%f", *(fp->Value.Value.Float4));
	break;
    case TBL_FLOAT8:
	dbfcmd(DBproc, "%f", *(fp->Value.Value.Float8));
	break;
    case TBL_STRING:
	dbfcmd(DBproc, "\"%s\"", fp->Value.Value.String);
	break;
	/*
	 * These two types are simply initialized with something (anything)
	 * so that the database will initialize the internal addresses of
	 * these fields.
	 */
    case TBL_TEXT:
	dbcmd(DBproc, "\"FILLER-WILL BE REPLACED\"");
	break;
    case TBL_BINARYDATA:
	dbcmd(DBproc, "0xFFFFFFFF");
	break;
    }
    return;
}
/*
** INTERNAL USE FUNCTION **
*/
static void
Add_Criteria_to_Sybuf(DBPROCESS * DBproc, const TBL_CRITERIA * cp)
{

    if (cp->Operator != TBL_NOP)
	dbfcmd(DBproc, "%s", cp->FieldName);
    switch (cp->Operator) {
    case TBL_NULL:
	dbcmd(DBproc, " is null ");
	break;
    case TBL_NOT_NULL:
	dbcmd(DBproc, " is not null ");
	break;
    case TBL_EQUAL:
	dbcmd(DBproc, " = ");
	break;
    case TBL_LIKE:
	dbcmd(DBproc, " like ");
	break;
    case TBL_NOT_EQUAL:
	dbcmd(DBproc, " != ");
	break;
    case TBL_GREATER:
	dbcmd(DBproc, " > ");
	break;
    case TBL_GREATER_EQUAL:
	dbcmd(DBproc, " >= ");
	break;
    case TBL_LESS:
	dbcmd(DBproc, " < ");
	break;
    case TBL_LESS_EQUAL:
	dbcmd(DBproc, " <= ");
	break;
    case TBL_NOP:
	dbfcmd(DBproc, "%s", cp->Value.Value.String);
	break;
    }
    if ((cp->Operator != TBL_NULL) && (cp->Operator != TBL_NOT_NULL) && (cp->Operator != TBL_NOP)) {
	switch (cp->Value.Type) {
	case TBL_SIGNED2:
	    dbfcmd(DBproc, " %d ", *(cp->Value.Value.Signed2));
	    break;
	case TBL_UNSIGNED2:
	    dbfcmd(DBproc, " %d ", *(cp->Value.Value.Unsigned2));
	    break;
	case TBL_SIGNED4:
	    dbfcmd(DBproc, " %d ", *(cp->Value.Value.Signed4));
	    break;
	case TBL_UNSIGNED4:
	    dbfcmd(DBproc, " %d ", *(cp->Value.Value.Unsigned4));
	    break;
	case TBL_FLOAT4:
	    dbfcmd(DBproc, " %f ", *(cp->Value.Value.Float4));
	    break;
	case TBL_FLOAT8:
	    dbfcmd(DBproc, " %f ", *(cp->Value.Value.Float8));
	    break;
	case TBL_STRING:
	    dbfcmd(DBproc, "\"%s\"", cp->Value.Value.String);
	    break;
	}
    }
    return;
}


/*
** INTERNAL USE FUNCTION **
*/
/*
 * The sybase error and message handlers....
 */
static int
syb_err_handler(DBPROCESS * dbproc,
		int severity, int dberr, int oserr,
		char *dberrstr, char *oserrstr)
{

    G_SybaseError = 1;
    if ((dbproc == NULL) || (DBDEAD(dbproc)))
	return (INT_EXIT);
    else {
	if (G_Verbose) {
	    printf("DB-Library error:\n\t%s\n", dberrstr);
	    if (oserr != DBNOERR)
		printf("Operating-system error:\n\t%s\n", oserrstr);
	}
	return (INT_CANCEL);

    }
}

/*
** INTERNAL USE FUNCTION **
*/
static int
syb_msg_handler(DBPROCESS * dbproc, DBINT msgno,
		int msgstate, int severity,
		char *msgtext, char *srvname, char *procname,
		int line)
 /* DBUSMALLINT line) */
{
/*
 * SYBASE DUMPING CODE...
 */
    G_SybaseError = 1;
    if (G_Verbose) {
	printf("Msg %ld, Level %d, State %d\n",
	       msgno, severity, msgstate);
	if (strlen(srvname) > 0)
	    printf("Server '%s', ", srvname);
	if (strlen(procname) > 0)
	    printf("Procedure '%s', ", procname);
	if (line > 0)
	    printf("Line %d", line);
	printf("\n\t%s\n", msgtext);
    }
    return (0);
}
/*
 * SYBASE DUMPING CODE...
 *
{
char sybuf[1024];
dbstrcpy(G_DBxxx,0,-1,sybuf);
printf("%d: %s\n",strlen(sybuf),sybuf);
}
*/
#else				/* If SYBASE is not defined...just return the
				 * unimplemented code. */

CONDITION
TBL_Open(const char *databaseName, const char *tableName, TBL_HANDLE ** handle)
{
    return COND_PushCondition(TBL_ERROR(TBL_UNIMPLEMENTED), "TBL_Open");
}

CONDITION
TBL_Close(TBL_HANDLE ** handle)
{
    return COND_PushCondition(TBL_ERROR(TBL_UNIMPLEMENTED), "TBL_Close");
}

CONDITION
TBL_Select(TBL_HANDLE ** handle, const TBL_CRITERIA * criteriaList,
     TBL_FIELD * fieldList, long *count, CONDITION(*callback) (), void *ctx)
{
    return COND_PushCondition(TBL_ERROR(TBL_UNIMPLEMENTED), "TBL_Select");
}

CONDITION
TBL_Update(TBL_HANDLE ** handle, const TBL_CRITERIA * criteriaList,
	   TBL_UPDATE * updateList)
{
    return COND_PushCondition(TBL_ERROR(TBL_UNIMPLEMENTED), "TBL_Update");
}

CONDITION
TBL_Insert(TBL_HANDLE ** handle, TBL_FIELD * fieldList)
{
    return COND_PushCondition(TBL_ERROR(TBL_UNIMPLEMENTED), "TBL_Insert");
}

CONDITION
TBL_Delete(TBL_HANDLE ** handle, const TBL_CRITERIA * criteriaList)
{
    return COND_PushCondition(TBL_ERROR(TBL_UNIMPLEMENTED), "TBL_Delete");
}

CONDITION
TBL_Layout(char *databaseName, char *tableName, CONDITION(*callback) (), void *ctx) {
    return COND_PushCondition(TBL_ERROR(TBL_UNIMPLEMENTED), "TBL_Layout");
}

CONDITION
TBL_NextUnique(TBL_HANDLE ** handle, char *name, int *unique)
{
    return COND_PushCondition(TBL_ERROR(TBL_UNIMPLEMENTED), "TBL_NextUnique");
}

CONDITION
TBL_NextSequence(TBL_HANDLE ** handle, char *name, long *unique)
{
    return COND_PushCondition(TBL_ERROR(TBL_UNIMPLEMENTED), "TBL_NextSequence");
}


void
TBL_BeginInsertTransaction(void)
{
}

void
TBL_RollbackInsertTransaction(void)
{
}

void
TBL_CommitInsertTransaction(void)
{
}

CONDITION
TBL_Debug(CTNBOOLEAN flag)
{
    return TBL_NORMAL;
}

int
TBL_HasViews(void)
{
    return TBL_UNIMPLEMENTED;
}
#endif
