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
** Last Update:		$Author: smm $, $Date: 2007-04-09 16:09:24 $
** Source File:		$RCSfile: tbl_sqlserver.c,v $
** Revision:		$Revision: 1.17 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.17 $ $RCSfile: tbl_sqlserver.c,v $";

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>

#include "dicom.h"
#include "condition.h"
#include "tblprivate.h"
#include "tbl.h"
#include "ctnthread.h"


#ifdef TBL_SQLSERVER

#include <sql.h>
#include <sqlext.h>

#include "tbl_sqlserver.h"

/*
** Static Globals for this file...
*/
#if 0
static int
    G_SybaseError = 0;


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
#endif

static TBL_CONTEXT* G_ContextHead = (TBL_CONTEXT *) NULL;
static int
    G_OpenFlag = 0;

static CTNBOOLEAN
    G_Verbose = FALSE;

static CTNBOOLEAN G_ConsistencyTestsOnOpen = TRUE;

HENV odbcEnvironment;
HDBC odbcConnect;

static void addFieldName(char *sqlStatement,
						 const char* punctuation,
						 const char* fieldName)
{
  char temp[512];

  sprintf(temp, "%s %s", punctuation, fieldName);
  strcat(sqlStatement, temp);
}

static void addFieldToSelect(char *sqlStatement,
				 const char* punctuation,
				 const TBL_FIELD *fp)
{
	addFieldName(sqlStatement, punctuation, fp->FieldName);
}
static void addFieldValue(char *sqlStatement,
						  const char * punctuation,
						  const TBL_FIELD *fp)
{
  char temp[512];
  char quote[2];
  char *pQuote;
  char *pField;
  char *pData;

  switch(fp->Value.Type) {
  case TBL_OTHER:
	  fprintf(stderr, "Cannot add field value: TBL_OTHER\n");
	  exit(1);
	  break;
  case TBL_UNSIGNED2:
	  sprintf(temp, "%s %d", punctuation, *fp->Value.Value.Unsigned2);
	  break;
  case TBL_UNSIGNED4:
	  sprintf(temp, "%s %d", punctuation, *fp->Value.Value.Unsigned4);
	  break;
  case TBL_SIGNED2:
	  sprintf(temp, "%s %d", punctuation, *fp->Value.Value.Signed2);
	  break;
  case TBL_SIGNED4:
	  sprintf(temp, "%s %d", punctuation, *fp->Value.Value.Signed4);
	  break;
  case TBL_FLOAT4:
	  sprintf(temp, "%s %f", punctuation, *fp->Value.Value.Float4);
	  break;
  case TBL_FLOAT8:
	  sprintf(temp, "%s %f", punctuation, *fp->Value.Value.Float8);
	  break;
  case TBL_STRING:
	  sprintf(temp, "%s \'", punctuation);
	  strcat(sqlStatement, temp);

	  strcpy(quote, "'");
	  pQuote = quote;
	  pData = sqlStatement + strlen(sqlStatement);
	  pField = fp->Value.Value.String;
	  while (*pField != '\0') {
	    if (*pField == *pQuote) {
	      *pData++ = *pQuote;
	      *pData++ = *pField++;
	    } else {
	      *pData++ = *pField++;
	    }
	  }
	  *pData = '\0';

	  /*strcat(sqlStatement, fp->Value.Value.String);*/

	  sprintf(temp, "\' ");
	  break;
  case TBL_TEXT:			/* To be filled in at execution time */
  case TBL_BINARYDATA:
	  sprintf(temp, "%s ?", punctuation);
	  break;
  }

  strcat(sqlStatement, temp);
}

static void addCriteria(char *sqlStatement,
						const char *punctuation,
						const TBL_CRITERIA *cp)
{
	char temp[512];

	sprintf(temp, " %s ", punctuation);
	strcat(sqlStatement, temp);

	if (cp->Operator != TBL_NOP) {
		sprintf(temp, "%s", cp->FieldName);
		strcat(sqlStatement, temp);
	}

    switch (cp->Operator) {
    case TBL_NULL:
		strcat(sqlStatement, " is null ");
	break;
    case TBL_NOT_NULL:
		strcat(sqlStatement, " is not null ");
	break;
    case TBL_EQUAL:
		strcat(sqlStatement, " = ");
	break;
    case TBL_LIKE:
		strcat(sqlStatement, " like ");
	break;
    case TBL_NOT_EQUAL:
		strcat(sqlStatement, " != ");
	break;
    case TBL_GREATER:
		strcat(sqlStatement, " > ");
	break;
    case TBL_GREATER_EQUAL:
		strcat(sqlStatement, " >= ");
	break;
    case TBL_LESS:
		strcat(sqlStatement, " < ");
	break;
    case TBL_LESS_EQUAL:
		strcat(sqlStatement, " <= ");
	break;
    case TBL_NOP:
		sprintf(sqlStatement, "%s", cp->Value.Value.String);
	break;
    }
    if ((cp->Operator != TBL_NULL) &&
		(cp->Operator != TBL_NOT_NULL) &&
		(cp->Operator != TBL_NOP)) {
	switch (cp->Value.Type) {
	case TBL_SIGNED2:
	    sprintf(temp, " %d ", *(cp->Value.Value.Signed2));
	    break;
	case TBL_UNSIGNED2:
	    sprintf(temp, " %d ", *(cp->Value.Value.Unsigned2));
	    break;
	case TBL_SIGNED4:
	    sprintf(temp, " %d ", *(cp->Value.Value.Signed4));
	    break;
	case TBL_UNSIGNED4:
	    sprintf(temp, " %d ", *(cp->Value.Value.Unsigned4));
	    break;
	case TBL_FLOAT4:
	    sprintf(temp, " %f ", *(cp->Value.Value.Float4));
	    break;
	case TBL_FLOAT8:
	    sprintf(temp, " %f ", *(cp->Value.Value.Float8));
	    break;
	case TBL_STRING:
	    sprintf(temp, "\'%s\'", cp->Value.Value.String);
	    break;
	}
	strcat(sqlStatement, temp);
    }
}

static SWORD mapToSQLType(const TBL_FIELD *fp)
{
	SWORD ret = 0;

	switch (fp->Value.Type) {
	case TBL_OTHER:
		break;
	case TBL_UNSIGNED2:
		ret = SQL_C_SHORT;
		break;
	case TBL_UNSIGNED4:
		ret = SQL_C_LONG;
		break;
	case TBL_SIGNED2:
		ret = SQL_C_SHORT;
		break;
	case TBL_SIGNED4:
		ret = SQL_C_LONG;
		break;
	case TBL_FLOAT4:
		ret = SQL_C_FLOAT;
		break;
	case TBL_FLOAT8:
		ret = SQL_C_DOUBLE;
		break;
	case TBL_STRING:
		ret = SQL_C_CHAR;
		break;
	case TBL_TEXT:
		ret = SQL_C_CHAR;
		break;
	case TBL_BINARYDATA:
		break;
	}
	return ret;
}

static SDWORD sqlNTS = SQL_NTS;

static void bindInsertColumns(const TBL_FIELD * fieldList, HSTMT stmtHandle)
{
  const TBL_FIELD *fp;
  UWORD parameterNumber = 1;

    fp = fieldList;
	while (fp->FieldName != NULL) {
		switch (fp->Value.Type) {
		case TBL_TEXT:
			SQLBindParameter(stmtHandle,
							 parameterNumber,
							 SQL_PARAM_INPUT,
							 SQL_C_CHAR,
							 SQL_CHAR,
							 strlen(fp->Value.Value.Text),
							 0,
							 fp->Value.Value.Text,
							 strlen(fp->Value.Value.Text),
							 &sqlNTS);
			parameterNumber++;
			break;
		default:
			break;
		}
		fp++;
    }
}

static void bindUpdateColumns(const TBL_UPDATE * updateList, HSTMT stmtHandle)
{
  const TBL_UPDATE *up;
  UWORD parameterNumber = 1;
  
  up = updateList;
  while (up->FieldName != NULL) {
	if (up->Value.Type == TBL_TEXT) {
		SQLBindParameter(stmtHandle,
						parameterNumber,
						SQL_PARAM_INPUT,
						SQL_C_CHAR,
						SQL_CHAR,
						strlen(up->Value.Value.Text),
						0,
						up->Value.Value.Text,
						strlen(up->Value.Value.Text),
						&sqlNTS);
		parameterNumber++;	
	}
	up++;
  }
}

static void bindSelectColumns(HSTMT stmtHandle, TBL_FIELD *fp,
				SDWORD FAR *lengthField)
{
	SQLSMALLINT columnToBind = 1;
	SWORD dataType;
	int retVal;

	while (fp->FieldName != NULL) {
		dataType = mapToSQLType(fp);
		retVal = SQLBindCol(stmtHandle, columnToBind, dataType,
					(PTR)fp->Value.Value.Other,
					fp->Value.AllocatedSize,
					lengthField);

		if( retVal != SQL_SUCCESS && retVal != SQL_SUCCESS_WITH_INFO ) {
			printf("SQLBindCol::Failure\n");
			exit(1);
		}
		fp++;

		columnToBind++;
	}

}

static void processSelectRow(TBL_FIELD *fp)
{
  int i;
  char *str;

	for (; fp->FieldName != NULL; fp++) {
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
		case TBL_TEXT:
			str = fp->Value.Value.String;
			/*for (i = fp->Value.AllocatedSize-1; i >= 0; i--) {*/
			for (i = strlen(str); i >= 0; i--) {
				if ((str[i] == ' ') || (str[i] == '\0'))
					str[i] = '\0';
				else
					break;
			}
			fp->Value.Size = i+1;

			/*fp->Value.Size = fp->Value.AllocatedSize;*/
			if (strcmp(fp->Value.Value.String, "") == 0) {
			    fp->Value.IsNull = 1;
			    fp->Value.Size = 0;
			}
			break;
		/*case TBL_TEXT:*/
		case TBL_BINARYDATA:
			fprintf(stderr, "Not ready to process TBL_TEXT or BINARYDATA\n");
			exit(1);
			/*
			fp->Value.Size = fp->Value.AllocatedSize;
			if (((DBBINARY *) dbtxptr(G_DBSelect, i)) == ((DBBINARY *) NULL)) {
			    fp->Value.IsNull = 1;
			    fp->Value.Size = 0;
			}
			*/
			break;
		}
	}
}

static void addUpdateValue(char *sqlStatement,
						   const char* punctuation,
						   TBL_UPDATE* up)
{
  char temp[512];

  sprintf(temp, "%s %s = ", punctuation, up->FieldName);
  strcat(sqlStatement, temp);

	if (up->Function == TBL_SET) {
		switch (up->Value.Type) {
	    case TBL_SIGNED2:
			sprintf(temp, "%d", *(up->Value.Value.Signed2));
			break;
		case TBL_UNSIGNED2:
			sprintf(temp, "%d", *(up->Value.Value.Unsigned2));
			break;
		case TBL_SIGNED4:
			sprintf(temp, "%d", *(up->Value.Value.Signed4));
			break;
		case TBL_UNSIGNED4:
			sprintf(temp, "%d", *(up->Value.Value.Unsigned4));
			break;
		case TBL_FLOAT4:
			sprintf(temp, "%f", *(up->Value.Value.Float4));
			break;
		case TBL_FLOAT8:
			sprintf(temp, "%f", *(up->Value.Value.Float8));
			break;
		case TBL_STRING:
			sprintf(temp, "\'%s\'", up->Value.Value.String);
			break;
		case TBL_TEXT:
			sprintf(temp, "?");		/* To be bound later */
			break;
		case TBL_BINARYDATA:
			fprintf(stderr, "Cannot handle binary data\n");
			exit(1);
			break;
		}
	} else if (up->Function == TBL_ZERO)
		sprintf(temp, " 0 ");
    else if (up->Function == TBL_INCREMENT)
		sprintf(temp, " %s + 1 ", up->FieldName);
    else if (up->Function == TBL_DECREMENT)
		sprintf(temp, " %s - 1 ", up->FieldName);
    else if (up->Function == TBL_ADD) {
		switch (up->Value.Type) {
		case TBL_SIGNED2:
			sprintf(temp, " %s + %d ", up->FieldName, *(up->Value.Value.Signed2));
			break;
		case TBL_SIGNED4:
			sprintf(temp, " %s + %d ", up->FieldName, *(up->Value.Value.Signed4));
			break;
		case TBL_UNSIGNED2:
			sprintf(temp, " %s + %d ", up->FieldName, *(up->Value.Value.Unsigned2));
			break;
		case TBL_UNSIGNED4:
			sprintf(temp, " %s + %d ", up->FieldName, *(up->Value.Value.Unsigned4));
			break;
		case TBL_FLOAT4:
			sprintf(temp, " %s + %.6f ", up->FieldName, *(up->Value.Value.Float4));
			break;
		case TBL_FLOAT8:
			sprintf(temp, " %s + %.6f ", up->FieldName, *(up->Value.Value.Float8));
			break;
		}
    } else if (up->Function == TBL_SUBTRACT) {
		switch (up->Value.Type) {
		case TBL_SIGNED2:
			sprintf(temp, " %s - %d ", up->FieldName, *(up->Value.Value.Signed2));
			break;
		case TBL_SIGNED4:
			sprintf(temp, " %s - %d ", up->FieldName, *(up->Value.Value.Signed4));
			break;
		case TBL_UNSIGNED2:
			sprintf(temp, " %s - %d ", up->FieldName, *(up->Value.Value.Unsigned2));
			break;
		case TBL_UNSIGNED4:
			sprintf(temp, " %s - %d ", up->FieldName, *(up->Value.Value.Unsigned4));
			break;
		case TBL_FLOAT4:
			sprintf(temp, " %s - %.6f ", up->FieldName, *(up->Value.Value.Float4));
			break;
		case TBL_FLOAT8:
			sprintf(temp, " %s - %.6f ", up->FieldName, *(up->Value.Value.Float8));
			break;
		}
    }
	strcat(sqlStatement, temp);
}

CONDITION TBL_SetOption(const char* string)
{
#if 0
    if (strcmp(string, "OPEN_SPEEDUP") == 0)
	G_ConsistencyTestsOnOpen = FALSE;

    return TBL_NORMAL;
#endif
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
#if 0
    if (flag == TRUE)
	G_Verbose = TRUE;
    else
	G_Verbose = FALSE;
#endif

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
TBL_Open(const char *databaseName, char *tableName, TBL_HANDLE ** handle)
{
    TBL_CONTEXT	* tc;
    char *tdb;
    char *ttb;
	int retVal;
	int ret;
	char szdsn[32],szuid[32],szpwd[20];

	THR_ObtainMutex(FAC_TBL);

    (*handle) = (void *) NULL;

    if (G_OpenFlag == 0) {
		char *nextToken = 0;
		char *accessString;
		char accessScratch[1024];

		retVal = SQLAllocEnv(&odbcEnvironment);

		if( retVal != SQL_SUCCESS && retVal != SQL_SUCCESS_WITH_INFO ) {
			THR_ReleaseMutex(FAC_TBL);
			(void) COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
				"MS SQL", "Could not allocate ODBC environment", "TBL_Open");
		    return COND_PushCondition(TBL_ERROR(TBL_OPENFAILED), tableName, "TBL_Open");
		}

		/*
		 * The Connection handle 
		 */
		retVal = SQLAllocConnect(odbcEnvironment, &odbcConnect);

		if( retVal != SQL_SUCCESS && retVal != SQL_SUCCESS_WITH_INFO ) {
			THR_ReleaseMutex(FAC_TBL);
			(void) COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
				"MS SQL", "Could not allocate connection handle", "TBL_Open");
		    return COND_PushCondition(TBL_ERROR(TBL_OPENFAILED), tableName, "TBL_Open");
		}

		accessString = getenv("SQL_ACCESS");
		if (accessString == NULL) {
		  accessString = "LocalServer:ctn:ctn";
		}
		if (accessString == NULL) {
			THR_ReleaseMutex(FAC_TBL);
			(void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
				"Environment variable SQL_ACCESS not set","", "TBL_Open");
			return COND_PushCondition(TBL_ERROR(TBL_OPENFAILED),
				tableName, "TBL_Open");
		}
		strcpy(accessScratch, accessString);

		nextToken = strtok(accessScratch, ":");
		if (nextToken == NULL) {
			THR_ReleaseMutex(FAC_TBL);
			(void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
				"SQL Server", "SQL_ACCESS env variable formatted illegally",
				"TBL_Open");
			return COND_PushCondition(TBL_ERROR(TBL_OPENFAILED),
				tableName, "TBL_Open");
		}
		strcpy(szdsn, nextToken);

		nextToken = strtok(NULL, ":");
		if (nextToken == NULL)
				nextToken = "";
		strcpy(szuid, nextToken);

		nextToken = strtok(NULL, ":");
		if (nextToken == NULL)
			nextToken = "";
		strcpy(szpwd, nextToken);

#if 0
		fprintf(stderr, "TBL_SQLServer:TBL_Open: server name: %s, login: %s, passwd: %s\n",
			szdsn, szuid, szpwd);
#endif

		/*
		 * This connects to the data source which must be defined
		 * in ODBC...look in the control panel to see how this one
		 * is defined.
		 */
		retVal = SQLConnect(odbcConnect,
			szdsn,(SWORD)strlen(szdsn),
			szuid,(SWORD)strlen(szuid),
			szpwd,(SWORD)strlen(szpwd));

		if( retVal != SQL_SUCCESS && retVal != SQL_SUCCESS_WITH_INFO ) {
			char msg[512];
			char s1[256];
			char s2[256];
			DWORD nativeError;
			SWORD s2Len = 0;
			HSTMT stmtHandle;

			stmtHandle = SQL_NULL_HSTMT;

			SQLError(odbcEnvironment, odbcConnect, stmtHandle,
				s1, &nativeError, s2, 255, &s2Len);

			s2[128] = '\0';
			(void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC), DATABASE,
				s2, "TBL_Open");

			THR_ReleaseMutex(FAC_TBL);
			sprintf(msg, "Could not connect to ODBC data server %s", szdsn);
			(void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
				"SQL Server", msg, "TBL_Open");
			sprintf(msg, "Used the login:password (%s:%s)", szuid, szpwd);
			(void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
				"SQL Server", msg, "TBL_Open");
			return COND_PushCondition(TBL_ERROR(TBL_OPENFAILED), tableName, "TBL_Open");
		}
	}
	
    tc = G_ContextHead;
    while (tc != (TBL_CONTEXT *) NULL) {
	if ((strcmp(tc->databaseName, databaseName) == 0) &&
	    (strcmp(tc->tableName, tableName) == 0)) {
		tc->refCount++;
		*handle = tc;
		G_OpenFlag++;
		THR_ReleaseMutex(FAC_TBL);
		return TBL_NORMAL;
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
     * Both the Database and the Table exist, so we can add the new entry to
     * our list.
     */
    if ((tc = (TBL_CONTEXT *) malloc(sizeof(TBL_CONTEXT))) == (TBL_CONTEXT *) NULL) {
		THR_ReleaseMutex(FAC_TBL);
		return COND_PushCondition(TBL_ERROR(TBL_NOMEMORY), "TBL_Open");
	}
    if ((tdb = (char *) malloc(strlen(databaseName) + 1)) == (char *) NULL) {
		free(tc);
		THR_ReleaseMutex(FAC_TBL);
		return COND_PushCondition(TBL_ERROR(TBL_NOMEMORY), "TBL_Open");
    }
    if ((ttb = (char *) malloc(strlen(tableName) + 1)) == (char *) NULL) {
		free(tc);
		free(tdb);
		THR_ReleaseMutex(FAC_TBL);
		return COND_PushCondition(TBL_ERROR(TBL_NOMEMORY), "TBL_Open");
    }
    strcpy(tdb, databaseName);
    strcpy(ttb, tableName);
    tc->databaseName = tdb;
    tc->tableName = ttb;
	tc->refCount = 1;
    tc->next = G_ContextHead;
    G_ContextHead = tc;

    (*handle) = (void *) G_ContextHead;

    G_OpenFlag++;
	THR_ReleaseMutex(FAC_TBL);
    return TBL_NORMAL;

}

CONDITION
TBL_OpenDB(const char *databaseName, TBL_HANDLE ** handle)
{
  TBL_CONTEXT	* tc;
  char *tdb;
  char *ttb;
  int retVal;
  int ret;
  char szdsn[32],szuid[32],szpwd[20];

  THR_ObtainMutex(FAC_TBL);

  (*handle) = (void *) NULL;

  if (G_OpenFlag == 0) {
    char *nextToken = 0;
    char *accessString;
    char accessScratch[1024];

    retVal = SQLAllocEnv(&odbcEnvironment);

    if( retVal != SQL_SUCCESS && retVal != SQL_SUCCESS_WITH_INFO ) {
      THR_ReleaseMutex(FAC_TBL);
      (void) COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
		"MS SQL", "Could not allocate ODBC environment", "TBL_Open");
      return COND_PushCondition(TBL_ERROR(TBL_OPENFAILED), databaseName, "TBL_Open");
    }

    /*
     * The Connection handle 
     */
    retVal = SQLAllocConnect(odbcEnvironment, &odbcConnect);

    if( retVal != SQL_SUCCESS && retVal != SQL_SUCCESS_WITH_INFO ) {
      THR_ReleaseMutex(FAC_TBL);
      (void) COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
		"MS SQL", "Could not allocate connection handle", "TBL_Open");
      return COND_PushCondition(TBL_ERROR(TBL_OPENFAILED), databaseName, "TBL_Open");
    }

    accessString = getenv("SQL_ACCESS");
    if (accessString == NULL) {
      accessString = "LocalServer:ctn:ctn";
    }
    if (accessString == NULL) {
      THR_ReleaseMutex(FAC_TBL);
      (void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
		"Environment variable SQL_ACCESS not set","", "TBL_Open");
      return COND_PushCondition(TBL_ERROR(TBL_OPENFAILED),
				databaseName, "TBL_Open");
    }
    strcpy(accessScratch, accessString);

    nextToken = strtok(accessScratch, ":");
    if (nextToken == NULL) {
      THR_ReleaseMutex(FAC_TBL);
      (void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
			"SQL Server", "SQL_ACCESS env variable formatted illegally",
			"TBL_Open");
      return COND_PushCondition(TBL_ERROR(TBL_OPENFAILED),
				databaseName, "TBL_Open");
    }
    strcpy(szdsn, nextToken);

    nextToken = strtok(NULL, ":");
    if (nextToken == NULL)
      nextToken = "";
    strcpy(szuid, nextToken);

    nextToken = strtok(NULL, ":");
    if (nextToken == NULL)
      nextToken = "";
    strcpy(szpwd, nextToken);

#if 0
    fprintf(stderr, "TBL_SQLServer:TBL_Open: server name: %s, login: %s, passwd: %s\n",
			szdsn, szuid, szpwd);
#endif

    /*
     * This connects to the data source which must be defined
     * in ODBC...look in the control panel to see how this one
     * is defined.
     */
    retVal = SQLConnect(odbcConnect,
			szdsn,(SWORD)strlen(szdsn),
			szuid,(SWORD)strlen(szuid),
			szpwd,(SWORD)strlen(szpwd));

    if( retVal != SQL_SUCCESS && retVal != SQL_SUCCESS_WITH_INFO ) {
	char msg[512];
	char s1[256];
	char s2[256];
	DWORD nativeError;
	SWORD s2Len = 0;
	HSTMT stmtHandle;

	stmtHandle = SQL_NULL_HSTMT;

	SQLError(odbcEnvironment, odbcConnect, stmtHandle,
		s1, &nativeError, s2, 255, &s2Len);

	s2[128] = '\0';
	(void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC), DATABASE,
				s2, "TBL_Open");

	THR_ReleaseMutex(FAC_TBL);
	sprintf(msg, "Could not connect to ODBC data server %s", szdsn);
	(void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
				"SQL Server", msg, "TBL_Open");
	sprintf(msg, "Used the login:password (%s:%s)", szuid, szpwd);
	(void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
				"SQL Server", msg, "TBL_Open");
	return COND_PushCondition(TBL_ERROR(TBL_OPENFAILED), databaseName, "TBL_Open");
    }
  }
	
  tc = G_ContextHead;
  while (tc != (TBL_CONTEXT *) NULL) {
    if ((strcmp(tc->databaseName, databaseName) == 0)) {
      tc->refCount++;
      *handle = tc;
      G_OpenFlag++;
      THR_ReleaseMutex(FAC_TBL);
      return TBL_NORMAL;
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
     * Both the Database and the Table exist, so we can add the new entry to
     * our list.
     */
  if ((tc = (TBL_CONTEXT *) malloc(sizeof(TBL_CONTEXT))) == (TBL_CONTEXT *) NULL) {
    THR_ReleaseMutex(FAC_TBL);
    return COND_PushCondition(TBL_ERROR(TBL_NOMEMORY), "TBL_Open");
  }
  if ((tdb = (char *) malloc(strlen(databaseName) + 1)) == (char *) NULL) {
    free(tc);
    THR_ReleaseMutex(FAC_TBL);
    return COND_PushCondition(TBL_ERROR(TBL_NOMEMORY), "TBL_Open");
  }

  strcpy(tdb, databaseName);

  tc->databaseName = tdb;
  tc->tableName = 0;
  tc->refCount = 1;
  tc->next = G_ContextHead;
  G_ContextHead = tc;

  (*handle) = (void *) G_ContextHead;

  G_OpenFlag++;
  THR_ReleaseMutex(FAC_TBL);
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
**	The TBL functions now can have multiple references to one
**  open table.  When the reference count reaches 0, it is time
**  to close the table.
**
** Algorithm:
**	Locates the handle specified in the call and removes that
**	entry from the internal list maintained by this	facility.
*/
CONDITION
TBL_Close(TBL_HANDLE ** handle)
{
    TBL_CONTEXT* prevtc;
	TBL_CONTEXT* tc;
	TBL_CONTEXT* hc;

	THR_ObtainMutex(FAC_TBL);

    G_OpenFlag--;
    if (G_OpenFlag == 0) {
		;
    }
    hc = (TBL_CONTEXT *) (*handle);
    prevtc = tc = G_ContextHead;
    while (tc != (TBL_HANDLE *) NULL) {
	if (hc == tc) {
		tc->refCount--;
		if (tc->refCount > 0) {
			THR_ReleaseMutex(FAC_TBL);
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
		THR_ReleaseMutex(FAC_TBL);
	    return TBL_NORMAL;
	}
	prevtc = tc;
	tc = tc->next;
    }

	/* If we reach this point, we did not find the user's handle. */
	THR_ReleaseMutex(FAC_TBL);
    return COND_PushCondition(TBL_ERROR(TBL_CLOSERROR), "TBL_Close");
}

CONDITION TBL_CloseDB()
{
  return TBL_NORMAL;
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
     TBL_FIELD * fieldList, long *count,
     CONDITION(*callback) (TBL_FIELD*, int, void *), void *ctx)
{
    TBL_CONTEXT* tc;
    TBL_FIELD* fp;
    const TBL_CRITERIA* cp;
    char *dbName;
    char *tableName;
    int foundit;
    long realcount;
    long *lp;
	HSTMT stmtHandle;
	char sqlStatement[8192];
	int iStatementLength = 0;
	int ret;
	SDWORD lengthField;

	THR_ObtainMutex(FAC_TBL);

    tc = G_ContextHead;
    foundit = 0;
    while ((tc != (TBL_CONTEXT *) NULL) && (foundit == 0)) {
		if (tc == (TBL_CONTEXT *) (*handle)) {
		    dbName = tc->databaseName;
			tableName = tc->tableName;
			foundit = 1;
		}
		tc = tc->next;
    }
    if (!foundit) {
		THR_ReleaseMutex(FAC_TBL);
		return COND_PushCondition(TBL_ERROR(TBL_BADHANDLE), "TBL_Select");
    }

	ret = SQLAllocStmt(odbcConnect, &stmtHandle);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
		THR_ReleaseMutex(FAC_TBL);
		(void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
				DATABASE, "Could not allocate statement handle", "TBL_Select");
		return COND_PushCondition(TBL_ERROR(TBL_SELECTFAILED),
				"allocate", "TBL_Select");
	}

    /*
     * We found the names we need...now make sure we can access it. Actually,
     * given that we found it, we probably already know the outcome of this
     * command...but we do need to set up the correct database for this
     * command anyway...
     */

	sprintf(sqlStatement, "USE %s", dbName);
	iStatementLength = strlen(sqlStatement);
	ret = SQLExecDirect(stmtHandle, (unsigned char *)sqlStatement, iStatementLength);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
		char s1[256];
		char s2[256];
		DWORD nativeError;
		SWORD s2Len = 0;
		SQLError(odbcEnvironment, odbcConnect, stmtHandle,
				s1, &nativeError, s2, 255, &s2Len);
		SQLFreeStmt(stmtHandle, SQL_DROP);
		THR_ReleaseMutex(FAC_TBL);

		s2[128] = '\0';
		(void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC), DATABASE,
			s2, "TBL_Select");
		return COND_PushCondition(TBL_ERROR(TBL_SELECTFAILED), "", "TBL_Select");
	}

	sprintf(sqlStatement, "SELECT ");
	fp = fieldList;
	addFieldToSelect(sqlStatement, "", fp);
	fp++;
	while (fp->FieldName != NULL) {
		addFieldToSelect(sqlStatement, ",", fp);
		fp++;
	}
	{ char temp[512];
		sprintf(temp, " FROM %s ", tableName);
		strcat(sqlStatement, temp);
	}
	cp = criteriaList;
	if ((cp != NULL) && (cp->FieldName != 0)) {
		strcat(sqlStatement, " WHERE ");
		addCriteria(sqlStatement, "", cp);
		cp++;
		while(cp->FieldName != NULL) {
			addCriteria(sqlStatement, " AND ", cp);
			cp++;
		}
	}
	if (count != (long *) NULL)
		lp = count;
    else
		lp = &realcount;

	*lp = 0;

	iStatementLength = strlen(sqlStatement);
	ret = SQLExecDirect(stmtHandle,
						(unsigned char *)sqlStatement,
						iStatementLength);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
		char s1[256];
		char s2[256];
		DWORD nativeError;
		SWORD s2Len = 0;
		SQLError(odbcEnvironment, odbcConnect, stmtHandle,
				s1, &nativeError, s2, 255, &s2Len);
		SQLFreeStmt(stmtHandle, SQL_DROP);
		THR_ReleaseMutex(FAC_TBL);

		s2[128] = '\0';
		(void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC), DATABASE,
			s2, "TBL_Select");
		return COND_PushCondition(TBL_ERROR(TBL_SELECTFAILED), "", "TBL_Select");
	}

    bindSelectColumns(stmtHandle, fieldList, &lengthField);

	/*
	 * The fetch will fetch all rows (like a cursor) from the 
	 * lastest statement.
	 */
	while( (ret = SQLFetch(stmtHandle)) != SQL_NO_DATA_FOUND ) {
		(*lp)++;
		processSelectRow(fieldList);

		if (callback != NULL) {
		    if (callback(fieldList, *lp, ctx) != TBL_NORMAL) {
				/*
				dbcancel(G_DBSelect);
				dbfreebuf(G_DBSelect);
				*/
				SQLFreeStmt(stmtHandle, SQL_DROP);
				THR_ReleaseMutex(FAC_TBL);
				return COND_PushCondition(TBL_ERROR(TBL_EARLYEXIT), "TBL_Select");
		    }
		}
	}
	
	if( ret != SQL_SUCCESS &&
		ret != SQL_SUCCESS_WITH_INFO &&
		ret != SQL_NO_DATA_FOUND ) {
		char s1[256];
		char s2[256];
		DWORD nativeError;
		SWORD s2Len = 0;
		SQLError(odbcEnvironment, odbcConnect, stmtHandle,
				s1, &nativeError, s2, 255, &s2Len);
		SQLFreeStmt(stmtHandle, SQL_DROP);
		THR_ReleaseMutex(FAC_TBL);
		s2[128] = '\0';
		(void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC), DATABASE,
			s2, "TBL_Select");
		return COND_PushCondition(TBL_ERROR(TBL_SELECTFAILED), "", "TBL_Select");
	}

	SQLFreeStmt(stmtHandle, SQL_DROP);
	THR_ReleaseMutex(FAC_TBL);
    return TBL_NORMAL;
}

CONDITION
TBL_SelectTable(TBL_HANDLE ** handle, const TBL_CRITERIA * criteriaList,
     TBL_FIELD * fieldList, long *count, CONDITION(*callback) (), void *ctx,
     const char* tableName)
{
  TBL_CONTEXT* tc;
  TBL_FIELD* fp;
  const TBL_CRITERIA* cp;
  char *dbName;

  int foundit;
  long realcount;
  long *lp;
  HSTMT stmtHandle;
  char sqlStatement[8192];
  int iStatementLength = 0;
  int ret;
  SDWORD lengthField;

  THR_ObtainMutex(FAC_TBL);

  tc = G_ContextHead;
  foundit = 0;
  while ((tc != (TBL_CONTEXT *) NULL) && (foundit == 0)) {
    if (tc == (TBL_CONTEXT *) (*handle)) {
      dbName = tc->databaseName;
      foundit = 1;
    }
    tc = tc->next;
  }
  if (!foundit) {
    THR_ReleaseMutex(FAC_TBL);
    return COND_PushCondition(TBL_ERROR(TBL_BADHANDLE), "TBL_Select");
  }

  ret = SQLAllocStmt(odbcConnect, &stmtHandle);

  if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
    THR_ReleaseMutex(FAC_TBL);
    (void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
			DATABASE, "Could not allocate statement handle", "TBL_Select");
    return COND_PushCondition(TBL_ERROR(TBL_SELECTFAILED),
				"allocate", "TBL_Select");
  }

    /*
     * We found the names we need...now make sure we can access it. Actually,
     * given that we found it, we probably already know the outcome of this
     * command...but we do need to set up the correct database for this
     * command anyway...
     */

  sprintf(sqlStatement, "USE %s", dbName);
  iStatementLength = strlen(sqlStatement);
  ret = SQLExecDirect(stmtHandle, (unsigned char *)sqlStatement, iStatementLength);

  if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
    char s1[256];
    char s2[256];
    DWORD nativeError;
    SWORD s2Len = 0;
    SQLError(odbcEnvironment, odbcConnect, stmtHandle,
		s1, &nativeError, s2, 255, &s2Len);
    SQLFreeStmt(stmtHandle, SQL_DROP);
    THR_ReleaseMutex(FAC_TBL);

    s2[128] = '\0';
    (void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC), DATABASE,
			s2, "TBL_Select");
    return COND_PushCondition(TBL_ERROR(TBL_SELECTFAILED), "", "TBL_Select");
  }

  sprintf(sqlStatement, "SELECT ");
  fp = fieldList;
  addFieldToSelect(sqlStatement, "", fp);
  fp++;
  while (fp->FieldName != NULL) {
    addFieldToSelect(sqlStatement, ",", fp);
    fp++;
  }
  { char temp[512];
    sprintf(temp, " FROM %s ", tableName);
    strcat(sqlStatement, temp);
  }
  cp = criteriaList;
  if ((cp != NULL) && (cp->FieldName != 0)) {
    strcat(sqlStatement, " WHERE ");
    addCriteria(sqlStatement, "", cp);
    cp++;
    while(cp->FieldName != NULL) {
      addCriteria(sqlStatement, " AND ", cp);
      cp++;
    }
  }
  if (count != (long *) NULL)
    lp = count;
  else
    lp = &realcount;

  *lp = 0;

  iStatementLength = strlen(sqlStatement);
  ret = SQLExecDirect(stmtHandle,
		(unsigned char *)sqlStatement,
		iStatementLength);

  if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
    char s1[256];
    char s2[256];
    DWORD nativeError;
    SWORD s2Len = 0;
    SQLError(odbcEnvironment, odbcConnect, stmtHandle,
		s1, &nativeError, s2, 255, &s2Len);
    SQLFreeStmt(stmtHandle, SQL_DROP);
    THR_ReleaseMutex(FAC_TBL);

    s2[128] = '\0';
    (void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC), DATABASE,
			s2, "TBL_Select");
    return COND_PushCondition(TBL_ERROR(TBL_SELECTFAILED), "", "TBL_Select");
  }

  bindSelectColumns(stmtHandle, fieldList, &lengthField);

	/*
	 * The fetch will fetch all rows (like a cursor) from the 
	 * lastest statement.
	 */
  while( (ret = SQLFetch(stmtHandle)) != SQL_NO_DATA_FOUND ) {
    (*lp)++;
    processSelectRow(fieldList);

    if (callback != NULL) {
      if (callback(fieldList, *lp, ctx) != TBL_NORMAL) {
				/*
				dbcancel(G_DBSelect);
				dbfreebuf(G_DBSelect);
				*/
	SQLFreeStmt(stmtHandle, SQL_DROP);
	THR_ReleaseMutex(FAC_TBL);
	return COND_PushCondition(TBL_ERROR(TBL_EARLYEXIT), "TBL_Select");
      }
    }
  }
	
  if( ret != SQL_SUCCESS &&
	    ret != SQL_SUCCESS_WITH_INFO &&
	    ret != SQL_NO_DATA_FOUND ) {
    char s1[256];
    char s2[256];
    DWORD nativeError;
    SWORD s2Len = 0;
    SQLError(odbcEnvironment, odbcConnect, stmtHandle,
				s1, &nativeError, s2, 255, &s2Len);
    SQLFreeStmt(stmtHandle, SQL_DROP);
    THR_ReleaseMutex(FAC_TBL);
    s2[128] = '\0';
    (void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC), DATABASE,
		s2, "TBL_Select");
    return COND_PushCondition(TBL_ERROR(TBL_SELECTFAILED), "", "TBL_Select");
  }

  SQLFreeStmt(stmtHandle, SQL_DROP);
  THR_ReleaseMutex(FAC_TBL);
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
    TBL_CONTEXT* tc;
    TBL_UPDATE* up;
    const TBL_CRITERIA* cp;
    /*char tabcol[100];*/
    char *dbName;
    char *tableName;
    int ret;
    int first;
    int foundTextorBinary;
    int foundit;
	HSTMT stmtHandle;
	char sqlStatement[8192];
	int iStatementLength = 0;
    /*DBBINARY* bin_ptr;*/

	THR_ObtainMutex(FAC_TBL);

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
		THR_ReleaseMutex(FAC_TBL);
		(void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
				DATABASE, "Could not allocate statement handle", "TBL_Update");
		return COND_PushCondition(TBL_ERROR(TBL_UPDATEFAILED),
				"allocate", "TBL_Update");
    }

	ret = SQLAllocStmt(odbcConnect, &stmtHandle);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
		THR_ReleaseMutex(FAC_TBL);
		(void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
				DATABASE, "Could not allocate statement handle", "TBL_Update");
		return COND_PushCondition(TBL_ERROR(TBL_UPDATEFAILED),
				"allocate", "TBL_Update");
	}

    /*
     * We found the names we need...now make sure we can access it. Actually,
     * given that we found it, we probably already know the outcome of this
     * command...but we do need to set up the correct database for this
     * command anyway...
     */

	sprintf(sqlStatement, "USE %s", dbName);
	iStatementLength = strlen(sqlStatement);
	ret = SQLExecDirect(stmtHandle,
		(unsigned char *)sqlStatement, iStatementLength);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
		char s1[256];
		char s2[256];
		DWORD nativeError = 0;
		SWORD s2Len = 0;

		SQLError(odbcEnvironment, odbcConnect, stmtHandle,
				s1, &nativeError, s2, 255, &s2Len);
		SQLFreeStmt(stmtHandle, SQL_DROP);
		THR_ReleaseMutex(FAC_TBL);
		s2[128] = '\0';
		(void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC), s2, "TBL_Update");
		return COND_PushCondition(TBL_ERROR(TBL_UPDATEFAILED),
				"", "TBL_Update");
	}

	if (updateList == (TBL_UPDATE *) NULL) {
		SQLFreeStmt(stmtHandle, SQL_DROP);
		THR_ReleaseMutex(FAC_TBL);
		return COND_PushCondition(TBL_ERROR(TBL_NOFIELDLIST), "TBL_Update");
    }

	up = updateList;
	sprintf(sqlStatement, "UPDATE %s SET ", tableName);
    first = 1;
    foundit = 0;
    foundTextorBinary = 0;
    while (up->FieldName != NULL) {
		if (up->Value.Type != TBL_BINARYDATA) {
			if (first)
				addUpdateValue(sqlStatement, "", up);
			else
				addUpdateValue(sqlStatement, " , ", up);

			first = 0;
			foundit = 1;
		} else {
			foundTextorBinary = 1;
			fprintf(stderr, "Update cannot handle BINARY data\n");
			exit(1);
		}
		up++;
    }

	if (foundit) {
		if ((cp = criteriaList) != (TBL_CRITERIA *) NULL) {
			strcat(sqlStatement, " WHERE ");

			addCriteria(sqlStatement, "", cp);
			cp++;

		    while (cp->FieldName != NULL) {
				addCriteria(sqlStatement, " AND ", cp);
				cp++;
			}
		}
	}

	bindUpdateColumns(updateList, stmtHandle);

	iStatementLength = strlen(sqlStatement);
	ret = SQLExecDirect(stmtHandle, (unsigned char *)sqlStatement, iStatementLength);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
		char s1[256];
		char s2[256];
		DWORD nativeError = 0;
		SWORD s2Len = 0;

		SQLError(odbcEnvironment, odbcConnect, stmtHandle,
				s1, &nativeError, s2, 255, &s2Len);
		SQLFreeStmt(stmtHandle, SQL_DROP);
		THR_ReleaseMutex(FAC_TBL);
		s2[128] = '\0';
		(void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC), s2, "TBL_Update");
		return COND_PushCondition(TBL_ERROR(TBL_UPDATEFAILED),
				"", "TBL_Update");
	}

	SQLFreeStmt(stmtHandle, SQL_DROP);
	THR_ReleaseMutex(FAC_TBL);
    return TBL_NORMAL;
}

CONDITION
TBL_UpdateTable(TBL_HANDLE ** handle, const TBL_CRITERIA * criteriaList,
	   TBL_UPDATE * updateList, const char* tableName)
{
  TBL_CONTEXT* tc;
  TBL_UPDATE* up;
  const TBL_CRITERIA* cp;
    /*char tabcol[100];*/
  char *dbName;

  int ret;
  int first;
  int foundTextorBinary;
  int foundit;
  HSTMT stmtHandle;
  char sqlStatement[8192];
  int iStatementLength = 0;
    /*DBBINARY* bin_ptr;*/

  THR_ObtainMutex(FAC_TBL);

  tc = G_ContextHead;
  foundit = 0;
  while (tc != (TBL_CONTEXT *) NULL) {
    if (tc == (TBL_CONTEXT *) (*handle)) {
      dbName = tc->databaseName;
      foundit = 1;
    }
    tc = tc->next;
  }
  if (!foundit) {
    THR_ReleaseMutex(FAC_TBL);
    (void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
				DATABASE, "Could not allocate statement handle", "TBL_Update");
    return COND_PushCondition(TBL_ERROR(TBL_UPDATEFAILED),
				"allocate", "TBL_Update");
  }

  ret = SQLAllocStmt(odbcConnect, &stmtHandle);

  if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
    THR_ReleaseMutex(FAC_TBL);
    (void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
				DATABASE, "Could not allocate statement handle", "TBL_Update");
    return COND_PushCondition(TBL_ERROR(TBL_UPDATEFAILED),
				"allocate", "TBL_Update");
  }

    /*
     * We found the names we need...now make sure we can access it. Actually,
     * given that we found it, we probably already know the outcome of this
     * command...but we do need to set up the correct database for this
     * command anyway...
     */

  sprintf(sqlStatement, "USE %s", dbName);
  iStatementLength = strlen(sqlStatement);
  ret = SQLExecDirect(stmtHandle,
		(unsigned char *)sqlStatement, iStatementLength);

  if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
    char s1[256];
    char s2[256];
    DWORD nativeError = 0;
    SWORD s2Len = 0;

    SQLError(odbcEnvironment, odbcConnect, stmtHandle,
				s1, &nativeError, s2, 255, &s2Len);
    SQLFreeStmt(stmtHandle, SQL_DROP);
    THR_ReleaseMutex(FAC_TBL);
    s2[128] = '\0';
    (void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC), s2, "TBL_Update");
    return COND_PushCondition(TBL_ERROR(TBL_UPDATEFAILED),
				"", "TBL_Update");
  }

  if (updateList == (TBL_UPDATE *) NULL) {
    SQLFreeStmt(stmtHandle, SQL_DROP);
    THR_ReleaseMutex(FAC_TBL);
    return COND_PushCondition(TBL_ERROR(TBL_NOFIELDLIST), "TBL_Update");
  }

  up = updateList;
  sprintf(sqlStatement, "UPDATE %s SET ", tableName);
  first = 1;
  foundit = 0;
  foundTextorBinary = 0;
  while (up->FieldName != NULL) {
    if (up->Value.Type != TBL_BINARYDATA) {
      if (first)
	addUpdateValue(sqlStatement, "", up);
      else
	addUpdateValue(sqlStatement, " , ", up);

      first = 0;
      foundit = 1;
    } else {
      foundTextorBinary = 1;
      fprintf(stderr, "Update cannot handle BINARY data\n");
      exit(1);
    }
    up++;
  }

  if (foundit) {
    if ((cp = criteriaList) != (TBL_CRITERIA *) NULL) {
      strcat(sqlStatement, " WHERE ");

      addCriteria(sqlStatement, "", cp);
      cp++;

      while (cp->FieldName != NULL) {
	addCriteria(sqlStatement, " AND ", cp);
	cp++;
      }
    }
  }

  bindUpdateColumns(updateList, stmtHandle);

  iStatementLength = strlen(sqlStatement);
  ret = SQLExecDirect(stmtHandle, (unsigned char *)sqlStatement, iStatementLength);

  if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
    char s1[256];
    char s2[256];
    DWORD nativeError = 0;
    SWORD s2Len = 0;

    SQLError(odbcEnvironment, odbcConnect, stmtHandle,
				s1, &nativeError, s2, 255, &s2Len);
    SQLFreeStmt(stmtHandle, SQL_DROP);
    THR_ReleaseMutex(FAC_TBL);
    s2[128] = '\0';
    (void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC), s2, "TBL_Update");
    return COND_PushCondition(TBL_ERROR(TBL_UPDATEFAILED),
				"", "TBL_Update");
  }

  SQLFreeStmt(stmtHandle, SQL_DROP);
  THR_ReleaseMutex(FAC_TBL);
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

#define CHECKRETURN(x, y) \
  if( (x) != SQL_SUCCESS && (x) != SQL_SUCCESS_WITH_INFO ) {	\
    char s1[256];						\
    char s2[256];						\
    DWORD nativeError = 0;					\
    SWORD s2Len = 0;						\
    SQLError(odbcEnvironment, odbcConnect, stmtHandle,		\
		s1, &nativeError, s2, 255, &s2Len);		\
    SQLFreeStmt(stmtHandle, SQL_DROP);				\
    SQLFreeStmt(updateHandle, SQL_DROP);			\
    SQLFreeStmt(transactionHandle, SQL_DROP);			\
    THR_ReleaseMutex(FAC_TBL);					\
    s2[128] = '\0';						\
    (void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),		\
	DATABASE, s2, (y));			\
    return TBL_DBSPECIFIC;					\
  }


CONDITION
TBL_NextUnique(TBL_HANDLE ** handle, char *name, int *unique)
{
  TBL_CONTEXT* tc;
  char *dbName;
  char *tableName;
  int ret;
  int num;
  int foundit;

  HSTMT stmtHandle;
  HSTMT updateHandle;
  HSTMT transactionHandle;
  char sqlStatement[8192];
  int iStatementLength = 0;
  int retVal;
  DWORD rtnLength = 0;

  THR_ObtainMutex(FAC_TBL);

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
    THR_ReleaseMutex(FAC_TBL);
    return COND_PushCondition(TBL_ERROR(TBL_BADHANDLE), "TBL_NextUnique");
  }

  ret = SQLAllocStmt(odbcConnect, &stmtHandle);

  if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
    THR_ReleaseMutex(FAC_TBL);
    (void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
	DATABASE, "Could not allocate statement handle", "TBL_NextUnique");
    return TBL_DBSPECIFIC;
  }

  ret = SQLAllocStmt(odbcConnect, &updateHandle);

  if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
    THR_ReleaseMutex(FAC_TBL);
    (void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
	DATABASE, "Could not allocate statement handle", "TBL_NextUnique");
    return TBL_DBSPECIFIC;
  }

  ret = SQLAllocStmt(odbcConnect, &transactionHandle);

  if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
    THR_ReleaseMutex(FAC_TBL);
    (void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
	DATABASE, "Could not allocate statement handle", "TBL_NextUnique");
    return TBL_DBSPECIFIC;
  }

  sprintf(sqlStatement, "USE %s", dbName);
  iStatementLength = strlen(sqlStatement);
  ret = SQLExecDirect(stmtHandle, (unsigned char *)sqlStatement, iStatementLength);
  CHECKRETURN(ret, "TBL_NextUnique");

#if 0
  ret = SQLSetConnectAttr(odbcConnect,
			  SQL_ATTR_AUTOCOMMIT, SQL_AUTOCOMMIT_OFF,
			  SQL_NTS);
  CHECKRETURN(ret, "TBL_NextUnique");
#endif

  sprintf(sqlStatement, "BEGIN TRANSACTION UNIQUEUPDATE");
  iStatementLength = strlen(sqlStatement);
  ret = SQLExecDirect(transactionHandle, (unsigned char *)sqlStatement, iStatementLength);
  CHECKRETURN(ret, "TBL_NextUnique");

  sprintf(sqlStatement,
		"SELECT UniqueNumber FROM %s WHERE NumberName = \'%s\' ",
		tableName, name);

  iStatementLength = strlen(sqlStatement);
  ret = SQLExecDirect(stmtHandle, (unsigned char *)sqlStatement, iStatementLength);
  CHECKRETURN(ret, "TBL_NextUnique");

  *unique = -1;

  retVal = SQLBindCol(stmtHandle, 1, SQL_C_LONG,
			(PTR)&num, 4, &rtnLength);
	
#if 1
  while( (ret = SQLFetch(stmtHandle)) != SQL_NO_DATA_FOUND ) {
    *unique = num;
  }
#else
  ret = SQLFetch(stmtHandle);
  if (ret == SQL_NO_DATA_FOUND) {
    fprintf(stderr, "no data found\n");
  }
  *unique = num;
#endif

  //SQLFreeStmt(stmtHandle, SQL_CLOSE);

    /*
     * Now perform the update...
     */
  sprintf(sqlStatement,
	"UPDATE %s SET UniqueNumber=UniqueNumber+1 WHERE NumberName = \'%s\'",
	tableName, name);

  iStatementLength = strlen(sqlStatement);
  ret = SQLExecDirect(updateHandle, (unsigned char *)sqlStatement, iStatementLength);
  CHECKRETURN(ret, "TBL_Insert");

  strcpy(sqlStatement, "COMMIT TRANSACTION UNIQUEUPDATE");

  iStatementLength = strlen(sqlStatement);
  ret = SQLExecDirect(transactionHandle, (unsigned char *)sqlStatement, iStatementLength);
  CHECKRETURN(ret, "TBL_NextUnique");

#if 0
  ret = SQLSetConnectAttr(odbcConnect,
			  SQL_ATTR_AUTOCOMMIT, SQL_AUTOCOMMIT_ON,
			  SQL_NTS);
  CHECKRETURN(ret, "TBL_NextUnique");
#endif

  SQLFreeStmt(stmtHandle, SQL_DROP);
  SQLFreeStmt(updateHandle, SQL_DROP);
  SQLFreeStmt(transactionHandle, SQL_DROP);
  THR_ReleaseMutex(FAC_TBL);
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
    TBL_CONTEXT* tc;
    TBL_FIELD* fp;
    char *dbName;
    char *tableName;
    int ret;
    int foundTextorBinary;
    int foundit;
	HSTMT stmtHandle;
	char sqlStatement[8192];
	int iStatementLength = 0;
    /*DBBINARY* bin_ptr;*/

	THR_ObtainMutex(FAC_TBL);
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
		THR_ReleaseMutex(FAC_TBL);
		return COND_PushCondition(TBL_ERROR(TBL_BADHANDLE), "TBL_Insert");
    }

	ret = SQLAllocStmt(odbcConnect, &stmtHandle);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
		THR_ReleaseMutex(FAC_TBL);
		(void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
				DATABASE, "Could not allocate statement handle", "TBL_Insert");
		return COND_PushCondition(TBL_ERROR(TBL_INSERTFAILED),
			"allocate", "TBL_Insert");
	}

	/*
	 * This is the first statement passed to the server instructing
	 * it what database to use.
	 * Note: This overrides any database you may have specicified
	 * in the ODBC data source.
	 */
	sprintf(sqlStatement, "USE %s", dbName);
	iStatementLength = strlen(sqlStatement);
	ret = SQLExecDirect(stmtHandle, (unsigned char *)sqlStatement, iStatementLength);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
		char s1[256];
		char s2[256];
		DWORD nativeError = 0;
		SWORD s2Len = 0;
		SQLError(odbcEnvironment, odbcConnect, stmtHandle,
				s1, &nativeError, s2, 255, &s2Len);
		SQLFreeStmt(stmtHandle, SQL_DROP);
		THR_ReleaseMutex(FAC_TBL);

		s2[128] = '\0';
		COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC), s2, "TBL_Insert");
		return COND_PushCondition(TBL_ERROR(TBL_INSERTFAILED),
			"", "TBL_Insert");
	}


	fp = fieldList;
	if (fp == (TBL_FIELD *) NULL) {
		THR_ReleaseMutex(FAC_TBL);
		return COND_PushCondition(TBL_ERROR(TBL_NOFIELDLIST), "TBL_Insert");
    }

	/*
     * Set up the insert statement
     */
    foundTextorBinary = 0;
	sprintf(sqlStatement, "INSERT %s ( ", tableName);
	addFieldName(sqlStatement, " ", fp->FieldName);

    if (fp->Value.Type == TBL_TEXT || fp->Value.Type == TBL_BINARYDATA)
		foundTextorBinary = 1;
    fp++;
    while (fp->FieldName != NULL) {
		addFieldName(sqlStatement, ",", fp->FieldName);

#if 0
		if (fp->Value.Type == TBL_TEXT || fp->Value.Type == TBL_BINARYDATA) {
			foundTextorBinary = 1;
			fprintf(stderr, "Found a text or binary value in TBL; exiting\n");
			exit(1);
		}
#endif
		fp++;
    }
	strcat(sqlStatement, " ) VALUES ( " );

    fp = fieldList;
    addFieldValue(sqlStatement, " ", fp);
    fp++;
    while (fp->FieldName != NULL) {
		addFieldValue(sqlStatement, ",", fp);
		fp++;
    }

	strcat(sqlStatement, " )");

	bindInsertColumns(fieldList, stmtHandle);

	iStatementLength = strlen(sqlStatement);
	ret = SQLExecDirect(stmtHandle, (unsigned char *)sqlStatement, iStatementLength);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
		char s1[256];
		char s2[256];
		DWORD nativeError = 0;
		SWORD s2Len = 0;

		SQLError(odbcEnvironment, odbcConnect, stmtHandle,
				s1, &nativeError, s2, 255, &s2Len);
		SQLFreeStmt(stmtHandle, SQL_DROP);
		THR_ReleaseMutex(FAC_TBL);
		s2[128] = '\0';
		COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC), s2, "TBL_Insert");
		return COND_PushCondition(TBL_ERROR(TBL_INSERTFAILED),
			"", "TBL_Insert");
	}

	SQLFreeStmt(stmtHandle, SQL_DROP);
	THR_ReleaseMutex(FAC_TBL);
    return TBL_NORMAL;
}

CONDITION
TBL_InsertTable(TBL_HANDLE ** handle, TBL_FIELD * fieldList, const char* tableName)
{
  TBL_CONTEXT* tc;
  TBL_FIELD* fp;
  char *dbName;

  int ret;
  int foundTextorBinary;
  int foundit;
  HSTMT stmtHandle;
  char sqlStatement[8192];
  int iStatementLength = 0;
    /*DBBINARY* bin_ptr;*/

  THR_ObtainMutex(FAC_TBL);
  tc = G_ContextHead;
  foundit = 0;
  while (tc != (TBL_CONTEXT *) NULL) {
    if (tc == (TBL_CONTEXT *) (*handle)) {
      dbName = tc->databaseName;
      foundit = 1;
    }
    tc = tc->next;
  }
  if (!foundit) {
    THR_ReleaseMutex(FAC_TBL);
    return COND_PushCondition(TBL_ERROR(TBL_BADHANDLE), "TBL_Insert");
  }

  ret = SQLAllocStmt(odbcConnect, &stmtHandle);

  if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
    THR_ReleaseMutex(FAC_TBL);
    (void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
				DATABASE, "Could not allocate statement handle", "TBL_Insert");
    return COND_PushCondition(TBL_ERROR(TBL_INSERTFAILED),
			"allocate", "TBL_Insert");
  }

	/*
	 * This is the first statement passed to the server instructing
	 * it what database to use.
	 * Note: This overrides any database you may have specicified
	 * in the ODBC data source.
	 */
  sprintf(sqlStatement, "USE %s", dbName);
  iStatementLength = strlen(sqlStatement);
  ret = SQLExecDirect(stmtHandle, (unsigned char *)sqlStatement, iStatementLength);

  if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
    char s1[256];
    char s2[256];
    DWORD nativeError = 0;
    SWORD s2Len = 0;

    fprintf(stderr, "%s\n", sqlStatement);

    SQLError(odbcEnvironment, odbcConnect, stmtHandle,
				s1, &nativeError, s2, 255, &s2Len);
    SQLFreeStmt(stmtHandle, SQL_DROP);
    THR_ReleaseMutex(FAC_TBL);

    s2[128] = '\0';
    COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC), s2, "TBL_Insert");
    return COND_PushCondition(TBL_ERROR(TBL_INSERTFAILED),
			"", "TBL_Insert");
  }


  fp = fieldList;
  if (fp == (TBL_FIELD *) NULL) {
    THR_ReleaseMutex(FAC_TBL);
    return COND_PushCondition(TBL_ERROR(TBL_NOFIELDLIST), "TBL_Insert");
  }

	/*
     * Set up the insert statement
     */
  foundTextorBinary = 0;
  sprintf(sqlStatement, "INSERT %s ( ", tableName);
  addFieldName(sqlStatement, " ", fp->FieldName);

  if (fp->Value.Type == TBL_TEXT || fp->Value.Type == TBL_BINARYDATA)
    foundTextorBinary = 1;
  fp++;
  while (fp->FieldName != NULL) {
    addFieldName(sqlStatement, ",", fp->FieldName);

#if 0
		if (fp->Value.Type == TBL_TEXT || fp->Value.Type == TBL_BINARYDATA) {
			foundTextorBinary = 1;
			fprintf(stderr, "Found a text or binary value in TBL; exiting\n");
			exit(1);
		}
#endif
  fp++;
  }
  strcat(sqlStatement, " ) VALUES ( " );

  fp = fieldList;
  addFieldValue(sqlStatement, " ", fp);
  fp++;
  while (fp->FieldName != NULL) {
    addFieldValue(sqlStatement, ",", fp);
    fp++;
  }

  strcat(sqlStatement, " )");

  bindInsertColumns(fieldList, stmtHandle);

  iStatementLength = strlen(sqlStatement);
  ret = SQLExecDirect(stmtHandle, (unsigned char *)sqlStatement, iStatementLength);

  if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
    char s1[256];
    char s2[256];
    DWORD nativeError = 0;
    SWORD s2Len = 0;

    fprintf(stderr, "%s\n", sqlStatement);

    SQLError(odbcEnvironment, odbcConnect, stmtHandle,
				s1, &nativeError, s2, 255, &s2Len);
    SQLFreeStmt(stmtHandle, SQL_DROP);
    THR_ReleaseMutex(FAC_TBL);
    s2[128] = '\0';
    COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC), s2, "TBL_Insert");
    return COND_PushCondition(TBL_ERROR(TBL_INSERTFAILED),
			"", "TBL_Insert");
  }

  SQLFreeStmt(stmtHandle, SQL_DROP);
  THR_ReleaseMutex(FAC_TBL);
  return TBL_NORMAL;
}
/*
** INTERNAL USE FUNCTIONS **
*/
void
TBL_BeginInsertTransaction(void)
{
	HSTMT stmtHandle;
	char sqlStatement[8192];
	int iStatementLength = 0;
	int ret = 0;

	THR_ObtainMutex(FAC_TBL);
	ret = SQLAllocStmt(odbcConnect, &stmtHandle);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
		THR_ReleaseMutex(FAC_TBL);
		return;  /* repair */
	}

	strcpy(sqlStatement, "BEGIN TRANSACTION TBLINSERT");
	iStatementLength = strlen(sqlStatement);
	ret = SQLExecDirect(stmtHandle, (unsigned char *)sqlStatement, iStatementLength);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
		char s1[256];
		char s2[256];
		DWORD nativeError;
		SWORD s2Len = 0;
		SQLError(odbcEnvironment, odbcConnect, stmtHandle,
				s1, &nativeError, s2, 255, &s2Len);
		SQLFreeStmt(stmtHandle, SQL_DROP);

		s2[128] = '\0';
		COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
			s2, "TBL_BeginInsertTransaction");
		THR_ReleaseMutex(FAC_TBL);
		return;  /* repair */
	}

    return;
}
void
TBL_CommitInsertTransaction(void)
{
	HSTMT stmtHandle;
	char sqlStatement[8192];
	int iStatementLength = 0;
	int ret = 0;

	ret = SQLAllocStmt(odbcConnect, &stmtHandle);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
		THR_ReleaseMutex(FAC_TBL);
		return;
	}

	strcpy(sqlStatement, "COMMIT TRANSACTION TBLINSERT");
	iStatementLength = strlen(sqlStatement);
	ret = SQLExecDirect(stmtHandle, (unsigned char *)sqlStatement, iStatementLength);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
		char s1[256];
		char s2[256];
		DWORD nativeError = 0;
		SWORD s2Len = 0;
		SQLError(odbcEnvironment, odbcConnect, stmtHandle,
				s1, &nativeError, s2, 255, &s2Len);
		SQLFreeStmt(stmtHandle, SQL_DROP);
		THR_ReleaseMutex(FAC_TBL);
		s2[128] = '\0';
		COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
			s2, "TBL_CommitInsertTransaction");
		return;  /* repair */
	}

	SQLFreeStmt(stmtHandle, SQL_DROP);
	THR_ReleaseMutex(FAC_TBL);
    return;
}
void
TBL_RollbackInsertTransaction(void)
{
	HSTMT stmtHandle;
	char sqlStatement[8192];
	int iStatementLength = 0;
	int ret = 0;

	ret = SQLAllocStmt(odbcConnect, &stmtHandle);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
		THR_ReleaseMutex(FAC_TBL);
		return;
	}

	strcpy(sqlStatement, "ROLLBACK TRANSACTION TBLINSERT");
	iStatementLength = strlen(sqlStatement);
	ret = SQLExecDirect(stmtHandle, (unsigned char *)sqlStatement, iStatementLength);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
		char s1[256];
		char s2[256];
		DWORD nativeError = 0;
		SWORD s2Len = 0;

		SQLError(odbcEnvironment, odbcConnect, stmtHandle,
				s1, &nativeError, s2, 255, &s2Len);
		SQLFreeStmt(stmtHandle, SQL_DROP);
		THR_ReleaseMutex(FAC_TBL);

		s2[128] = '\0';
		COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
			s2, "TBL_RollbackInsertTransaction");
		return;  /* repair */
	}

	THR_ReleaseMutex(FAC_TBL);
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
    TBL_CONTEXT* tc;
    const TBL_CRITERIA* cp;
    char* dbName;
    char* tableName;
    int foundit;
	int ret;
	HSTMT stmtHandle;
	char sqlStatement[8192];
	int iStatementLength = 0;
    /*DBBINARY* bin_ptr;*/


	THR_ObtainMutex(FAC_TBL);

    tc = G_ContextHead;
    foundit = 0;
    while ((tc != (TBL_CONTEXT *) NULL) && (foundit == 0)) {
		if (tc == (TBL_CONTEXT *) (*handle)) {
		    dbName = tc->databaseName;
			tableName = tc->tableName;
			foundit = 1;
		}
		tc = tc->next;
    }
    if (!foundit) {
		THR_ReleaseMutex(FAC_TBL);
		return COND_PushCondition(TBL_ERROR(TBL_BADHANDLE), "TBL_Delete");
    }

	ret = SQLAllocStmt(odbcConnect, &stmtHandle);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
		THR_ReleaseMutex(FAC_TBL);
		(void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
				DATABASE, "Could not allocate statement handle", "TBL_Delete");
		return COND_PushCondition(TBL_ERROR(TBL_DELETEFAILED),
			"allocate", "TBL_Delete");
	}

    /*
     * We found the names we need...now make sure we can access it. Actually,
     * given that we found it, we probably already know the outcome of this
     * command...but we do need to set up the correct database for this
     * command anyway...
     */

	sprintf(sqlStatement, "USE %s", dbName);
	iStatementLength = strlen(sqlStatement);
	ret = SQLExecDirect(stmtHandle, (unsigned char *)sqlStatement, iStatementLength);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
		char s1[256];
		char s2[256];
		DWORD nativeError = 0;
		SWORD s2Len = 0;

		SQLError(odbcEnvironment, odbcConnect, stmtHandle,
				s1, &nativeError, s2, 255, &s2Len);
		SQLFreeStmt(stmtHandle, SQL_DROP);
		THR_ReleaseMutex(FAC_TBL);

		s2[128] = '\0';
		(void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
				DATABASE, s2, "TBL_Delete");
		return COND_PushCondition(TBL_ERROR(TBL_DELETEFAILED),
			"", "TBL_Delete");
	}

	sprintf(sqlStatement, "DELETE FROM %s", tableName);

	cp = criteriaList;
    if (cp == (TBL_CRITERIA *) NULL || cp->FieldName == NULL) {
		;
    } else {
		strcat(sqlStatement, " WHERE ");
		addCriteria(sqlStatement, "", cp);
		cp++;
		while (cp != NULL && cp->FieldName != NULL) {
			addCriteria(sqlStatement, " AND ", cp);
		    cp++;
		}
    }

	iStatementLength = strlen(sqlStatement);
	ret = SQLExecDirect(stmtHandle, (unsigned char *)sqlStatement, iStatementLength);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
		char s1[256];
		char s2[256];
		DWORD nativeError = 0;
		SWORD s2Len = 0;

		SQLError(odbcEnvironment, odbcConnect, stmtHandle,
				s1, &nativeError, s2, 255, &s2Len);
		SQLFreeStmt(stmtHandle, SQL_DROP);
		THR_ReleaseMutex(FAC_TBL);
		s2[128] = '\0';
		(void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
				DATABASE, s2, "TBL_Delete");
		return COND_PushCondition(TBL_ERROR(TBL_DELETEFAILED),
			"", "TBL_Delete");
	}

	SQLFreeStmt(stmtHandle, SQL_DROP);
	THR_ReleaseMutex(FAC_TBL);

    return TBL_NORMAL;
}

CONDITION
TBL_DeleteTable(TBL_HANDLE ** handle, const TBL_CRITERIA * criteriaList,
	const char* tableName)
{
  TBL_CONTEXT* tc;
  const TBL_CRITERIA* cp;
  char* dbName;

  int foundit;
  int ret;
  HSTMT stmtHandle;
  char sqlStatement[8192];
  int iStatementLength = 0;
    /*DBBINARY* bin_ptr;*/


  THR_ObtainMutex(FAC_TBL);

  tc = G_ContextHead;
  foundit = 0;
  while ((tc != (TBL_CONTEXT *) NULL) && (foundit == 0)) {
    if (tc == (TBL_CONTEXT *) (*handle)) {
      dbName = tc->databaseName;
      foundit = 1;
    }
    tc = tc->next;
  }
  if (!foundit) {
    THR_ReleaseMutex(FAC_TBL);
    return COND_PushCondition(TBL_ERROR(TBL_BADHANDLE), "TBL_Delete");
  }

  ret = SQLAllocStmt(odbcConnect, &stmtHandle);

  if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
    THR_ReleaseMutex(FAC_TBL);
    (void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
				DATABASE, "Could not allocate statement handle", "TBL_Delete");
    return COND_PushCondition(TBL_ERROR(TBL_DELETEFAILED),
			"allocate", "TBL_Delete");
  }

    /*
     * We found the names we need...now make sure we can access it. Actually,
     * given that we found it, we probably already know the outcome of this
     * command...but we do need to set up the correct database for this
     * command anyway...
     */

  sprintf(sqlStatement, "USE %s", dbName);
  iStatementLength = strlen(sqlStatement);
  ret = SQLExecDirect(stmtHandle, (unsigned char *)sqlStatement, iStatementLength);

  if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
    char s1[256];
    char s2[256];
    DWORD nativeError = 0;
    SWORD s2Len = 0;

    SQLError(odbcEnvironment, odbcConnect, stmtHandle,
				s1, &nativeError, s2, 255, &s2Len);
    SQLFreeStmt(stmtHandle, SQL_DROP);
    THR_ReleaseMutex(FAC_TBL);

    s2[128] = '\0';
    (void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
				DATABASE, s2, "TBL_Delete");
    return COND_PushCondition(TBL_ERROR(TBL_DELETEFAILED),
			"", "TBL_Delete");
  }

  sprintf(sqlStatement, "DELETE FROM %s", tableName);

  cp = criteriaList;
  if (cp == (TBL_CRITERIA *) NULL || cp->FieldName == NULL) {
    ;
  } else {
    strcat(sqlStatement, " WHERE ");
    addCriteria(sqlStatement, "", cp);
    cp++;
    while (cp != NULL && cp->FieldName != NULL) {
      addCriteria(sqlStatement, " AND ", cp);
      cp++;
    }
  }

  iStatementLength = strlen(sqlStatement);
  ret = SQLExecDirect(stmtHandle, (unsigned char *)sqlStatement, iStatementLength);

  if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
    char s1[256];
    char s2[256];
    DWORD nativeError = 0;
    SWORD s2Len = 0;

    SQLError(odbcEnvironment, odbcConnect, stmtHandle,
				s1, &nativeError, s2, 255, &s2Len);
    SQLFreeStmt(stmtHandle, SQL_DROP);
    THR_ReleaseMutex(FAC_TBL);
    s2[128] = '\0';
    (void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
				DATABASE, s2, "TBL_Delete");
    return COND_PushCondition(TBL_ERROR(TBL_DELETEFAILED),
			"", "TBL_Delete");
  }

  SQLFreeStmt(stmtHandle, SQL_DROP);
  THR_ReleaseMutex(FAC_TBL);

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
TBL_Layout(char *databaseName, char *tableName, CONDITION(*callback) (), void *ctx)
{
    TBL_FIELD field;
    char column_name[SQL_MAX_COLUMN_NAME_LEN + 1];
    long tableid;
	DWORD sizeRtn;
    long system_type;
    long user_length;
	DWORD column_name_length = 0;
	DWORD system_type_length = 0;
	DWORD user_length_length = 0;
	HSTMT stmtHandle;
	char sqlStatement[8192];
	int iStatementLength = 0;
	int ret;

    if (callback == NULL) {
		return COND_PushCondition(TBL_ERROR(TBL_NOCALLBACK), "TBL_Layout");
    }

	THR_ObtainMutex(FAC_TBL);
	ret = SQLAllocStmt(odbcConnect, &stmtHandle);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
		THR_ReleaseMutex(FAC_TBL);
		(void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
				DATABASE, "Could not allocate statement handle", "TBL_Layout");
		return TBL_DBSPECIFIC;
	}

	sprintf(sqlStatement, "USE %s", databaseName);
	iStatementLength = strlen(sqlStatement);
	ret = SQLExecDirect(stmtHandle, (unsigned char *)sqlStatement, iStatementLength);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
		char s1[256];
		char s2[256];
		DWORD nativeError = 0;
		SWORD s2Len = 0;

		SQLError(odbcEnvironment, odbcConnect, stmtHandle,
				s1, &nativeError, s2, 255, &s2Len);
		SQLFreeStmt(stmtHandle, SQL_DROP);
		THR_ReleaseMutex(FAC_TBL);

		s2[128] = '\0';
		return COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
			s2, "TBL_Layout");
	}

	sprintf(sqlStatement,
		"SELECT id from sysobjects where name = \'%s\' and type = \'U\'",
		tableName);
	iStatementLength = strlen(sqlStatement);
	ret = SQLExecDirect(stmtHandle, (unsigned char *)sqlStatement, iStatementLength);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
		char s1[256];
		char s2[256];
		DWORD nativeError = 0;
		SWORD s2Len = 0;

		SQLError(odbcEnvironment, odbcConnect, stmtHandle,
				s1, &nativeError, s2, 255, &s2Len);
		SQLFreeStmt(stmtHandle, SQL_DROP);
		THR_ReleaseMutex(FAC_TBL);

		s2[128] = '\0';
		(void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
			s2, "TBL_Layout");
		return COND_PushCondition(TBL_ERROR(TBL_SELECTFAILED),
			"", "TBL_Layout");
	}

	ret = SQLBindCol(stmtHandle, 1, SQL_C_LONG, (PTR)&tableid, sizeof(tableid),
						&sizeRtn);
	while( (ret = SQLFetch(stmtHandle)) != SQL_NO_DATA_FOUND ) {
		;
	}

	if( ret != SQL_SUCCESS &&
		ret != SQL_SUCCESS_WITH_INFO &&
		ret != SQL_NO_DATA_FOUND ) {
		char s1[256];
		char s2[256];
		DWORD nativeError = 0;
		SWORD s2Len = 0;

		SQLError(odbcEnvironment, odbcConnect, stmtHandle,
				s1, &nativeError, s2, 255, &s2Len);
		THR_ReleaseMutex(FAC_TBL);
		s2[128] = '\0';
		(void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
			s2, "TBL_Layout");
		return COND_PushCondition(TBL_ERROR(TBL_SELECTFAILED),
			"fetch", "TBL_Layout");
	}


	SQLFreeStmt(stmtHandle, SQL_CLOSE);

	sprintf(sqlStatement, "SELECT name,type,length from syscolumns where id = %d",
			tableid);
	iStatementLength = strlen(sqlStatement);
	ret = SQLExecDirect(stmtHandle, (unsigned char *)sqlStatement, iStatementLength);

	if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
		char s1[256];
		char s2[256];
		DWORD nativeError = 0;
		SWORD s2Len = 0;

		SQLError(odbcEnvironment, odbcConnect, stmtHandle,
				s1, &nativeError, s2, 255, &s2Len);
		SQLFreeStmt(stmtHandle, SQL_DROP);
		THR_ReleaseMutex(FAC_TBL);
		s2[128] = '\0';
		(void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
			s2, "TBL_Layout");
		return COND_PushCondition(TBL_ERROR(TBL_SELECTFAILED),
			"", "TBL_Layout");
	}

	ret = SQLBindCol(stmtHandle, 1, SQL_C_CHAR, (PTR)&column_name,
						sizeof(column_name), &column_name_length);
	ret = SQLBindCol(stmtHandle, 2, SQL_C_LONG, (PTR)&system_type,
						sizeof(system_type), &system_type_length);
	ret = SQLBindCol(stmtHandle, 3, SQL_C_LONG, (PTR)&user_length,
						sizeof(user_length), &user_length_length);

    field.FieldName = column_name;
	while( (ret = SQLFetch(stmtHandle)) != SQL_NO_DATA_FOUND ) {

		if( ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO ) {
			char s1[256];
			char s2[256];
			DWORD nativeError = 0;
			SWORD s2Len = 0;
			SQLError(odbcEnvironment, odbcConnect, stmtHandle,
				s1, &nativeError, s2, 255, &s2Len);
			SQLFreeStmt(stmtHandle, SQL_DROP);
			THR_ReleaseMutex(FAC_TBL);
			s2[128] = '\0';
			(void)COND_PushCondition(TBL_ERROR(TBL_DBSPECIFIC),
				s2, "TBL_Layout");
			return COND_PushCondition(TBL_ERROR(TBL_SELECTFAILED),
				"", "TBL_Layout");
		}

		field.Value.AllocatedSize = user_length;
		switch (system_type) {
		case 45:			/* binary */
		case 37:			/* varbinary */
		case 34:			/* image */
		    field.Value.Type = TBL_BINARYDATA;
		    break;
		case 35:			/* text */
		    field.Value.Type = TBL_TEXT;
		    break;
		case 39:			/* varchar */
		case 47:			/* char */
		    field.Value.Type = TBL_STRING;
		    break;
		case 52:			/* smallint */
		    field.Value.Type = TBL_SIGNED2;
		    break;
		case 38:			/* intn */
		case 56:			/* int */
		    field.Value.Type = TBL_SIGNED4;
		    break;
		case 59:			/* real */
		    field.Value.Type = TBL_FLOAT4;
		    break;
		case 109:			/* floatn */
		case 62:			/* float */
		    field.Value.Type = TBL_FLOAT8;
		    break;


		default:
		    field.Value.Type = TBL_OTHER;
		    break;
		}
		if (callback != NULL) {
		    if (callback(&field, ctx) != TBL_NORMAL) {
				SQLFreeStmt(stmtHandle, SQL_DROP);
				THR_ReleaseMutex(FAC_TBL);
				return COND_PushCondition(TBL_ERROR(TBL_EARLYEXIT), "TBL_Layout");
			}
		}
	}

	SQLFreeStmt(stmtHandle, SQL_DROP);
	THR_ReleaseMutex(FAC_TBL);
    return TBL_NORMAL;

}


#else				/* If TBL_SQLSERVER is not defined...just return the
				 * unimplemented code. */

CONDITION
TBL_Open(const char *databaseName, char *tableName, TBL_HANDLE ** handle)
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
    return 0;
}
int
TBL_HasUpdateIncrement(void)
{
    return 0;
}
#endif
