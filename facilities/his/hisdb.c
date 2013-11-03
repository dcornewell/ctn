/*
          Copyright (C) 1993, RSNA and Washington University

          The software and supporting documentation for the Radiological
          Society of North America (RSNA) 1993 Digital Imaging and
          Communications in Medicine (DICOM) Demonstration were developed
          at the
                  Electronic Radiology Laboratory
                  Mallinckrodt Institute of Radiology
                  Washington University School of Medicine
                  510 S. Kingshighway Blvd.
                  St. Louis, MO 63110
          as part of the 1993 DICOM Central Test Node project for, and
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
/*
** @$=@$=@$=
*/
/*
**				DICOM 93
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s): 	HIS_CreateDB, HIS_DeleteDB, HIS_Init, HIS_Open, HIS_Close,
**
**			HIS_AddPatient, HIS_AddVisit, HIS_AddStudy, HIS_AddResults,
**			HIS_AddInterpretation,
**
**			HIS_DelPatient, HIS_DelVisit, HIS_DelStudy, HIS_DelResults,
**			HIS_DelInterpretation,
**
**			HIS_UpdatePatient, HIS_UpdateVisit, HIS_UpdateStudy,
**			HIS_UpdateResults, HIS_UpdateInterpretation,
**
**			HIS_GetPatient, HIS_GetNextPatient, HIS_GetVisit,
**			HIS_GetNextVisit, HIS_GetStudy, HIS_GetNextStudy,
**			HIS_GetResults, HIS_GetNextResults, HIS_GetInterpretation,
**			HIS_GetNextInterpretation
**
** Author, Date:	David E. Beecher, 15-June-93
**
** Intent:		These routines provide access to the HIS databases for the DICOM
**			project.  They rely heavily on the hunk file facility for
**			low-level file handling functions.  All routines in this
**			file are not meant for public use, but
**			rather just the routines which have prototypes in the file
**			hisdb.h.  hisprivate.h contains the prototypes for all other
**			functions to be used only by the database implementor.
**
** Last Update:		$Author: smm $, $Date: 1997-08-29 18:44:33 $
** Source File:		$RCSfile: hisdb.c,v $
** Revision:		$Revision: 1.24 $
** Status:		$State: Exp $
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
/*#include <sys/time.h> */
#include "dicom.h"
#include "condition.h"
#include "hunk_man.h"
#include "hisdb.h"
#include "hisprivate.h"
static char rcsid[] = "$Revision: 1.24 $ $RCSfile: hisdb.c,v $";

static Root
    GS_root;

static HISidstruct
   *GS_context = (HISidstruct *) NULL;

static short
    GS_dbid = 0;

/* HIS_CreateDB
**
** Purpose:
**	This routine creates a new HIS database for use by the system.
**
** Parameter Dictionary:
**	char *dbkey:
**		a null terminated string indicating the file name to use for the db.
**
**
** Return Values:
**
**	HIS_NORMAL: 	The creation command succeeded
**	HIS_EXISTS: 	This database exists and must be removed first
**	HIS_CREATERROR: The hunk file could not be created
**	HIS_OPENERROR:	The newly created file could not be opened
**	HIS_ALLOCATERROR:The Root record could not be allocated
**	HIS_WRITERROR:	The Root record could not be written to disk
**
** Algorithm:
**	A database file can only be created if it represents a filename which is
**	located in a directory to which the caller has write access.  The file name
**	specified by dbkey must not already exist, or the routine returns HIS_EXISTS
**	and the existing database is retained.  The user may use HIS_DeleteDB to first
**	remove this database and it may then be created with HIS_CreateDB.  More
**	information concerning database creation will be explained in the HIS_Init call.
*/
CONDITION
HIS_CreateDB(char *dbkey)
{
    short
        dbid;

    if (HIS_Open(dbkey, &dbid) == HIS_NORMAL) {
	HIS_Close(dbid);
	COND_PushCondition(HIS_EXISTS, "HIS_CreateDB: Database %s is already open", dbkey);
	return (HIS_EXISTS);
    }
    return (HIS_Init(dbkey));
}

/* HIS_DeleteDB
**
** Purpose:
**	This routine deletes an existing database.
**
** Parameter Dictionary:
**	char *dbkey:
**		a null terminated string indicating the file name to use for the db.
**
**
** Return Values:
**
**	HIS_NORMAL: 	The deletion command succeeded
**
** Algorithm:
**	The deletion algorithm always assumes it succeeds.  It just deletes the file
**	name passed in dbkey.  This routine may be smartened up if the need arises.
*/
CONDITION
HIS_DeleteDB(char *dbkey)
{

    unlink(dbkey);

    return (HIS_NORMAL);

}

/* HIS_Init
**
** Purpose:
**	This routine initializes a new database..
**
** Parameter Dictionary:
**	char *dbkey:
**		a null terminated string indicating the file name to use for the db.
**
**
** Return Values:
**
**	HIS_NORMAL: 	The initialization command succeeded
**	HIS_CREATERROR:	The database file could not be created
**	HIS_OPENERROR:	The newly created database could not be opened
**	HIS_ALLOCATERROR:The root node record could not be allocated
**	HIS_WRITERROR:	The root node record could not be written
**
** Algorithm:
**	The initialization relies on the hunk_man facility to perform most of the
**	work.  Default parameters are used to set up the hunk file to be used
**	by this database, namely, HIS_HUNKINITIALALLOCATION for the number of hunks
**	to initially allocate to this file, HIS_HUNKRECORDSIZE, for the hunk record
**	size, and HIS_NUMRECSPERHUNK, for the number of records per hunk.  These
**	parameters can be tuned for optimal performance.  The root node is then
**	initialized and written to the hunk.
*/
CONDITION
HIS_Init(char *dbkey)
{
    int i;

    CONDITION retval;

    if (HF_Create(dbkey, HIS_HUNKINITIALALLOCATION,
		  HIS_HUNKRECORDSIZE, HIS_NUMRECSPERHUNK) != HF_NORMAL) {
	COND_PushCondition(HIS_CREATERROR, "HIS_Init: HF_Create error");
	return (HIS_CREATERROR);
    }
    if (retval = HF_Open(dbkey) != HF_NORMAL) {
	COND_PushCondition(HIS_OPENERROR, "HIS_Init: HF_Open error");
	return (HIS_OPENERROR);
    }
    strcpy(GS_root.dbkey, dbkey);
    GS_root.num_patients = 0;
    GS_root.num_visits = 0;
    GS_root.num_studies = 0;
    GS_root.num_results = 0;
    GS_root.num_interps = 0;

    GS_root.used_p = GS_root.used_v = GS_root.used_s =
	GS_root.used_r = GS_root.used_i = HIS_NULL;

    GS_root.free_p = GS_root.free_v = GS_root.free_s =
	GS_root.free_r = GS_root.free_i = 0;
    for (i = 0; i < HIS_MAXPATIENTS - 1; i++) {
	GS_root.patient[i].uid[0] = '\000';
	GS_root.patient[i].badd.hunk_number = HUNK_PTR_NULL;
	GS_root.patient[i].badd.node_number = HUNK_PTR_NULL;
	GS_root.patient[i].next = i + 1;
    }
    GS_root.patient[HIS_MAXPATIENTS - 1].next = HIS_NULL;

    for (i = 0; i < HIS_MAXVISITS - 1; i++) {
	GS_root.visit[i].uid[0] = '\000';
	GS_root.visit[i].badd.hunk_number = HUNK_PTR_NULL;
	GS_root.visit[i].badd.node_number = HUNK_PTR_NULL;
	GS_root.visit[i].next = i + 1;
    }
    GS_root.visit[HIS_MAXVISITS - 1].next = HIS_NULL;

    for (i = 0; i < HIS_MAXSTUDIES - 1; i++) {
	GS_root.study[i].uid[0] = '\000';
	GS_root.study[i].badd.hunk_number = HUNK_PTR_NULL;
	GS_root.study[i].badd.node_number = HUNK_PTR_NULL;
	GS_root.study[i].next = i + 1;
    }
    GS_root.study[HIS_MAXSTUDIES - 1].next = HIS_NULL;

    for (i = 0; i < HIS_MAXRESULTS - 1; i++) {
	GS_root.results[i].uid[0] = '\000';
	GS_root.results[i].badd.hunk_number = HUNK_PTR_NULL;
	GS_root.results[i].badd.node_number = HUNK_PTR_NULL;
	GS_root.results[i].next = i + 1;
    }
    GS_root.results[HIS_MAXRESULTS - 1].next = HIS_NULL;

    for (i = 0; i < HIS_MAXINTERPS - 1; i++) {
	GS_root.interps[i].uid[0] = '\000';
	GS_root.interps[i].badd.hunk_number = HUNK_PTR_NULL;
	GS_root.interps[i].badd.node_number = HUNK_PTR_NULL;
	GS_root.interps[i].next = i + 1;
    }
    GS_root.interps[HIS_MAXINTERPS - 1].next = HIS_NULL;

    if (HF_AllocateStaticRecord(0, sizeof(GS_root)) != HF_NORMAL) {
	COND_PushCondition(HIS_ALLOCATERROR, "HIS_Init: HF_AllocateStaticRecord error");
	return (HIS_ALLOCATERROR);
    }
    if (HF_WriteStaticRecord(0, sizeof(GS_root), (void *) &GS_root)
	!= HF_NORMAL) {
	COND_PushCondition(HIS_WRITERROR, "HIS_Init: HF_WriteStaticRecord error");
	return (HIS_WRITERROR);
    }
    HF_Close(dbkey);

    return (HIS_NORMAL);
}
/* HIS_Close
**
** Purpose:
**	This routine closes a previously opened database.
**
** Parameter Dictionary:
**	short dbid:
**		the database identifier received upon opening the database.
**
**
** Return Values:
**
**	HIS_NORMAL: 	The close command succeeded
**	HIS_CLOSERROR:	The database file could not be closed
**
** Algorithm:
**	The close simply performs any writes needed at the hunk_file level and
**	removes this database id from the list of active dbids.
*/
CONDITION
HIS_Close(short dbid)
{

    CONDITION
	retval;
    char
        ts[HIS_MAXKEYLENGTH];
    if ((retval = HIS_Removeid(dbid, ts)) != HIS_NORMAL) {
	COND_PushCondition(retval, "HIS_Close: dbid handling error");
	return (retval);
    }
    if (HF_Close(ts) != HF_NORMAL) {
	COND_PushCondition(HIS_CLOSERROR, "HIS_Close: HF_Close error");
	return (HIS_CLOSERROR);
    }
    return (HIS_NORMAL);
}
/* HIS_Addid
**
** Purpose:
**	This routine adds a new database access id to an internal list which
**	also stores contextual information for sequential database searches.
**
** Parameter Dictionary:
**	short dbid:
**		the database identifier to be generated by this routine.
**
**
** Return Values:
**
**	HIS_NORMAL: 	The command succeeded and generated a new dbid.
**	HIS_NOMEMORY:	No memory is available for list allocation
**
** Algorithm:
**	The routine simply increments a static global variable, GS_dbid, to
**	create the next dbid.  This dbid will be used in all subsequent access
**	calls to this database.  Then, another HISidstruct is allocated and added
**	to the list of existing structs which may be opened by other users.
**	Currently, each process may only have a single database open at one time.
**	This can easily be modified if the need arises.
*/
CONDITION
HIS_Addid(short *dbid)
{

    HISidstruct
       *temp;

    *dbid = ++GS_dbid;

    if (GS_context == (HISidstruct *) NULL) {
	if ((GS_context = (HISidstruct *) malloc(sizeof(HISidstruct))) ==
	    (HISidstruct *) NULL) {
	    return (HIS_NOMEMORY);
	}
	GS_context->next = (HISidstruct *) NULL;
    } else {
	if ((temp = (HISidstruct *) malloc(sizeof(HISidstruct))) ==
	    (HISidstruct *) NULL) {
	    return (HIS_NOMEMORY);
	}
	temp->next = GS_context;
	GS_context = temp;
    }
    return (HIS_NORMAL);
}
/* HIS_Removeid
**
** Purpose:
**	This routine removes the HISidstruct from the existing list if a match
**	is found with the dbid passed.
**
** Parameter Dictionary:
**	short dbid:
**		the database identifier to be removed by this routine.
**
**
** Return Values:
**
**	HIS_NORMAL: 	The command succeeded and generated a new dbid.
**	HIS_IDREMERROR:	The HISidstruct list is null, there are no entries.
**			Or, the dbid could not be found in the list.
**
** Algorithm:
**	The routine searches through the HISidstruct list pointed to by the
**	global variable GS_context, and deletes the entry which contains the
**	the dbid passed as input to the routine.
*/
CONDITION
HIS_Removeid(short dbid, char *dbkey)
{

    HISidstruct
       *pt,
       *temp;

    if (GS_context == (HISidstruct *) NULL)
	return (HIS_IDREMERROR);

    temp = GS_context;
    while (temp != (HISidstruct *) NULL) {
	if (temp->dbid == dbid) {
	    strcpy(dbkey, temp->dbkey);
	    if (temp == GS_context) {
		GS_context = temp->next;
		free(temp);
	    } else {
		pt->next = temp->next;
		free(temp);
	    }
	    return (HIS_NORMAL);
	}
	pt = temp;
	temp = temp->next;
    }

    return (HIS_IDREMERROR);
}
/* HIS_Open
**
** Purpose:
**	This routine attempts to open for access the database pointed to by
**	the input string dbkey.
**
** Parameter Dictionary:
**	char *dbkey:
**		the name of the database to open
**	short dbid:
**		will contain the newly created dbid upon successful return
**
**
** Return Values:
**
**	HIS_NORMAL: 	The open succeeded and a new database has been opened.
**	HIS_OPENERROR:	The corresponding hunk_file could not be opened.
**	HIS_NOMEMORY:	No local memory available for storage.
**	HIS_READERROR:	The root record of the database could not be read.
**
** Algorithm:
**	HIS_Open first attempts to open the corresponding hunk file for access. It
**	then attempts to generate a new dbid and context structure.  Failure for
**	either of these two functions results in an open failure.
**	If successful, the context structure is effectively set to null, and the
**	root index record of the database is read.
*/
CONDITION
HIS_Open(char *dbkey, short *dbid)
{

    CONDITION retval;

    if (HF_Open(dbkey) != HF_NORMAL) {
	COND_PushCondition(HIS_OPENERROR, "HIS_Open: HF_Open error");
	return (HIS_OPENERROR);
    }
    if ((retval = HIS_Addid(dbid)) != HIS_NORMAL) {
	COND_PushCondition(retval, "HIS_Open: id handling error");
	HF_Close(dbkey);
	return (retval);
    }
    strcpy(GS_context->dbkey, dbkey);
    GS_context->dbid = *dbid;
    GS_context->patient_context = HIS_NULL;
    GS_context->visit_context = HIS_NULL;
    GS_context->study_context = HIS_NULL;
    GS_context->results_context = HIS_NULL;
    GS_context->interpretation_context = HIS_NULL;

    if (HF_ReadStaticRecord(0, sizeof(GS_root), (void *) &GS_root)
	!= HF_NORMAL) {
	COND_PushCondition(HIS_READERROR, "HIS_Open: HF_ReadStaticRecord error");
	return (HIS_READERROR);
    }
    return (HIS_NORMAL);
}
/* HIS_AddPatient
**
** Purpose:
**	This routine adds a new patient to the database.
**
** Parameter Dictionary:
**	short dbid:
**		Contains the database identifier.
**	HIS_PatientLevel *pat:
**		Contains the patient information to add to the database.
**
** Return Values:
**
**	HIS_NORMAL: 	The patient add succeeded.
**	HIS_NOTOPENED:	The specified dbid has not been opened.
**	HIS_LOCKERROR:	Could not acquire a requested lock
**	HIS_TOOMANYPATS:Only HIS_MAXPATIENTS are allowed, this add would result
**		in one more (HIS_MAXPATIENTS is currently 20).
**	HIS_NOFREENODES:No more internal nodes are available.
**	HIS_BADSEQCOUNT:Cannot specify a study or visit at this level.
**	HIS_ALLOCATERROR:The new record in the hunk file could not be allocated.
**	HIS_WRITERROR:	The new record could not be written to the hunk file.
**
** Algorithm:
**	The dbid must be found in the struct list.  Then, the check is made for the
**	maximum number of patients .  The new records are allocated, the root node
**	is updated, and all new information is written out to the database file.
*/
CONDITION
HIS_AddPatient(short dbid, HIS_PatientLevel *pat)
{

    int temp;
    HunkBufAdd hadd;

    if (HIS_Findid(dbid) != HIS_NORMAL) {
	COND_PushCondition(HIS_NOTOPENED, "HIS_AddPatient: DB not opened");
	return (HIS_NOTOPENED);
    }
    if (GS_root.num_patients >= HIS_MAXPATIENTS) {
	COND_PushCondition(HIS_TOOMANYPATS, "HIS_AddPatient: Too many patients");
	return (HIS_TOOMANYPATS);
    }
    if (GS_root.free_p == HIS_NULL) {
	COND_PushCondition(HIS_NOFREENODES, "HIS_AddPatient: No free nodes");
	return (HIS_NOFREENODES);
    }
    if ((pat->RVSequenceCount != 0) || (pat->RSSequenceCount != 0)) {
	COND_PushCondition(HIS_BADSEQCOUNT, "HIS_AddPatient: Bad sequence count");
	return (HIS_BADSEQCOUNT);
    }
    if (HF_ExclusiveLock() != HF_NORMAL) {
	COND_PushCondition(HIS_LOCKERROR, "HIS_AddPatient: Lock Error");
	return (HIS_LOCKERROR);
    }
    /*
     * We might want to check for duplicate entries here...but not now.
     */
    if (HF_AllocateRecord(sizeof(HIS_PatientLevel), &hadd) != HF_NORMAL) {
	COND_PushCondition(HIS_ALLOCATERROR, "HIS_AddPatient: HF_AllocateRecord error");
	return (HIS_UnLock(HIS_ALLOCATERROR));
    }
    GS_root.patient[GS_root.free_p].badd.hunk_number = hadd.hunk_number;
    GS_root.patient[GS_root.free_p].badd.node_number = hadd.node_number;
    strcpy(GS_root.patient[GS_root.free_p].uid, pat->PIUID);

    if (HF_WriteRecord(&hadd, sizeof(HIS_PatientLevel), (void *) pat)
	!= HF_NORMAL) {
	COND_PushCondition(HIS_WRITERROR, "HIS_AddPatient: HF_WriteRecord error");
	return (HIS_UnLock(HIS_WRITERROR));
    }
    temp = GS_root.patient[GS_root.free_p].next;
    GS_root.patient[GS_root.free_p].next = GS_root.used_p;
    GS_root.used_p = GS_root.free_p;
    GS_root.free_p = temp;
    GS_root.num_patients++;

    if (HF_WriteStaticRecord(0, sizeof(GS_root), (void *) &GS_root)
	!= HF_NORMAL) {
	COND_PushCondition(HIS_WRITERROR, "HIS_AddPatient: HF_WriteStaticRecord error");
	return (HIS_UnLock(HIS_WRITERROR));
    }
    return (HIS_UnLock(HIS_NORMAL));
}
/* HIS_FindID
**
** Purpose:
**	This routine attempts to locate the dbid in the maintained list.
**
** Parameter Dictionary:
**	short dbid:
**		Contains the database identifier.
**
** Return Values:
**
**	HIS_NORMAL: 	The dbid was found.
**	HIS_IDNOTHERE:	The dbid was not found.
**
** Algorithm:
**	The dbid is search for and if found, HIS_NORMAL is returned, otherwise,
**	HIS_IDNOTHERE is returned.
*/
CONDITION
HIS_Findid(short dbid)
{

    HISidstruct
       *temp;

    if (GS_context == (HISidstruct *) NULL)
	return (HIS_IDNOTHERE);

    temp = GS_context;
    while (temp != (HISidstruct *) NULL) {
	if (temp->dbid == dbid) {
	    return (HIS_NORMAL);
	}
	temp = temp->next;
    }
    return (HIS_IDNOTHERE);
}

/* HIS_AddVisit
**
** Purpose:
**	HIS_AddVisit adds a visit node to the his data base
**
** Parameter Dictionary:
**	short dbid:
**		The database identifer.
**	HIS_VisitLevel *visit:
**		The visit level struct to add.
**
** Return Values:
**
**	HIS_NORMAL: 	The add visit succeeded.
**	HIS_LOCKERROR:	Could not acquire a requested lock
**	HIS_NOTOPENED:	The database has not been opened.
**	HIS_NOFREENODES:No more internal nodes are available.
**	HIS_BADPATIENT:	The patient specified in the visit counld not be found.
**	HIS_BADSEQCOUNT:A study was specified in the visit node.
**	HIS_READERROR:	The hunk record could not be read.
**	HIS_TOOMANYVISITS:Adding this visit would exceed the maximum allowed which
**		is set at HIS_MAXVISITS.  This value is currently 15.
**	HIS_ALLOCATERROR:The new record could not be allocated.
**	HIS_WRITERROR:	The new record could not be written.
**
** Algorithm:
**	We perform the normal checks for database open, and the maximum number
**	of visits allowed.  We check to make sure that a study has not been
**	named as part of the visit structure, as this is an error at this point.
**	We then make sure we can locate the patient specified in the visit
**	structure as this is necessary to link this visit with the appropriate
**	patient.  Once all these checks have been performed, it is a relatively
**	simple process to actually add the visit to the database.
**
**
*/
CONDITION
HIS_AddVisit(short dbid, HIS_VisitLevel *visit)
{

    int i,
        temp;
    HunkBufAdd
        vadd;
    CONDITION
	retval,
	retvalfinal;

    if (HIS_Findid(dbid) != HIS_NORMAL) {
	COND_PushCondition(HIS_NOTOPENED, "HIS_AddVisit: HIS_Findid failed");
	return (HIS_NOTOPENED);
    }
    if (GS_root.num_visits >= HIS_MAXVISITS) {
	COND_PushCondition(HIS_TOOMANYVISITS, "HIS_AddVisit: Too many visits");
	return (HIS_TOOMANYVISITS);
    }
    if (GS_root.free_v == HIS_NULL) {
	COND_PushCondition(HIS_NOFREENODES, "HIS_AddVisit: No free nodes");
	return (HIS_NOFREENODES);
    }
    if (visit->RSSequenceCount != 0) {
	COND_PushCondition(HIS_BADSEQCOUNT, "HIS_AddVisit: Bad sequence count");
	return (HIS_BADSEQCOUNT);
    }
    /*
     * Check to make sure that any up pointers to patients exist...
     */
    for (i = 0; i < visit->RPSequenceCount; i++) {
	if (HIS_FindPatient(visit->RPSequence[i]) == HIS_NULL) {
	    COND_PushCondition(HIS_BADPATIENT, "HIS_AddVisit: Patient not found");
	    return (HIS_BADPATIENT);
	}
    }
    if (HF_ExclusiveLock() != HF_NORMAL) {
	COND_PushCondition(HIS_LOCKERROR, "HIS_AddVisit: HF_ExclusiveLock failed");
	return (HIS_LOCKERROR);
    }
    /*
     * We may want to check for a duplicate visit at some point, but not
     * now... just allocate the record needed.
     */

    if (HF_AllocateRecord(sizeof(HIS_VisitLevel), &vadd) != HF_NORMAL) {
	COND_PushCondition(HIS_ALLOCATERROR, "HIS_AddVisit: HF_AllocateRecord failed");
	return (HIS_UnLock(HIS_ALLOCATERROR));
    }
    retvalfinal = HIS_NORMAL;
    for (i = 0; i < visit->RPSequenceCount; i++) {
	if ((retval = HIS_AddVisittoPatient(visit->PVIUID, visit->RPSequence[i]))
	    != HIS_NORMAL) {
	    retvalfinal = retval;
	}
    }

    if (retvalfinal != HIS_NORMAL) {
	COND_PushCondition(retvalfinal, "HIS_AddVisit: Couldn't update patient");
	HF_DeallocateRecord(&vadd);
	return (HIS_UnLock(retvalfinal));
    }
    GS_root.visit[GS_root.free_v].badd.hunk_number = vadd.hunk_number;
    GS_root.visit[GS_root.free_v].badd.node_number = vadd.node_number;
    strcpy(GS_root.visit[GS_root.free_v].uid, visit->PVIUID);

    if (HF_WriteRecord(&vadd, sizeof(HIS_VisitLevel), (void *) visit)
	!= HF_NORMAL) {
	COND_PushCondition(HIS_WRITERROR, "HIS_AddVisit: HF_WriteRecord error");
	return (HIS_UnLock(HIS_WRITERROR));
    }
    temp = GS_root.visit[GS_root.free_v].next;
    GS_root.visit[GS_root.free_v].next = GS_root.used_v;
    GS_root.used_v = GS_root.free_v;
    GS_root.free_v = temp;
    GS_root.num_visits++;

    if (HF_WriteStaticRecord(0, sizeof(GS_root), (void *) &GS_root)
	!= HF_NORMAL) {
	COND_PushCondition(HIS_WRITERROR, "HIS_AddVisit: HF_WriteStaticRecord error");
	return (HIS_UnLock(HIS_WRITERROR));
    }
    return (HIS_UnLock(HIS_NORMAL));
}

/* HIS_AddStudy
**
** Purpose:
**	HIS_AddStudy adds a study node to an existing patient
**
** Parameter Dictionary:
**	short dbid:
**		The database identifer.
**	HIS_StudyLevel *study:
**		The study level struct to add to the specified patient.
**
** Return Values:
**
**	HIS_NORMAL: 	The add study succeeded.
**	HIS_LOCKERROR:	Could not acquire a requested lock
**	HIS_NOTOPENED:	The database has not been opened.
**	HIS_BADPATIENT:	The patient specified counld not be found.
**	HIS_READERROR:	The hunk record could not be read.
**	HIS_TOOMANYSTUDS:Adding this study would exceed the maximum allowed which
**		is set at HIS_MAXSTUDIES.  This value is currently 20.
**	HIS_ALLOCATERROR:The new record could not be allocated.
**	HIS_WRITERROR:	The new record could not be written.
**
** Algorithm:
**	We perform the normal checks for database open, and the maximum number
**	of studies allowed.  We check to make sure that a result has not been
**	named as part of the study structure, as this is an error at this point.
**	We then make sure we can locate the patient and visit specified in the
**	study structure as this is necessary to link this study with the appropriate
**	patient and visit.  Once all these checks have been performed, it is a
**	relatively simple process to actually add the study to the database.
**
*/
CONDITION
HIS_AddStudy(short dbid, HIS_StudyLevel *study)
{

    int i,
        temp;
    HunkBufAdd
        sadd;
    CONDITION
	retval,
	retvalfinal;

    if (HIS_Findid(dbid) != HIS_NORMAL) {
	COND_PushCondition(HIS_NOTOPENED, "HIS_AddStudy: HIS_Findid failed");
	return (HIS_NOTOPENED);
    }
    if (GS_root.num_studies >= HIS_MAXSTUDIES) {
	COND_PushCondition(HIS_TOOMANYSTUDS, "HIS_AddStudy: Too many studies");
	return (HIS_TOOMANYSTUDS);
    }
    if (GS_root.free_s == HIS_NULL) {
	COND_PushCondition(HIS_NOFREENODES, "HIS_AddStudy: No free nodes");
	return (HIS_NOFREENODES);
    }
    if (study->RRSequenceCount != 0) {
	COND_PushCondition(HIS_BADSEQCOUNT, "HIS_AddStudy: Bad sequence count");
	return (HIS_BADSEQCOUNT);
    }
    /*
     * Check to make sure that any up pointers to patients and/or visits
     * exist...
     */
    for (i = 0; i < study->RPSequenceCount; i++) {
	if (HIS_FindPatient(study->RPSequence[i]) == HIS_NULL) {
	    COND_PushCondition(HIS_BADPATIENT, "HIS_AddStudy: Patient not found");
	    return (HIS_BADPATIENT);
	}
    }
    for (i = 0; i < study->RVSequenceCount; i++) {
	if (HIS_FindVisit(study->RVSequence[i]) == HIS_NULL) {
	    COND_PushCondition(HIS_BADVISIT, "HIS_AddStudy: Visit not found");
	    return (HIS_BADVISIT);
	}
    }
    if (HF_ExclusiveLock() != HF_NORMAL) {
	COND_PushCondition(HIS_LOCKERROR, "HIS_AddStudy: HF_ExclusiveLock failed");
	return (HIS_LOCKERROR);
    }
    /*
     * We may want to check for a duplicate study at some point, but not
     * now... just allocate the record needed.
     */

    if (HF_AllocateRecord(sizeof(HIS_StudyLevel), &sadd) != HF_NORMAL) {
	COND_PushCondition(HIS_ALLOCATERROR, "HIS_AddStudy: HF_AllocateRecord failed");
	return (HIS_UnLock(HIS_ALLOCATERROR));
    }
    retvalfinal = HIS_NORMAL;
    for (i = 0; i < study->RPSequenceCount; i++) {
	if ((retval = HIS_AddStudytoPatient(study->SIUID, study->RPSequence[i]))
	    != HIS_NORMAL) {
	    retvalfinal = retval;
	}
    }
    for (i = 0; i < study->RVSequenceCount; i++) {
	if ((retval = HIS_AddStudytoVisit(study->SIUID, study->RVSequence[i]))
	    != HIS_NORMAL) {
	    retvalfinal = retval;
	}
    }

    if (retvalfinal != HIS_NORMAL) {
	COND_PushCondition(retvalfinal, "HIS_AddStudy: Couldn't update patient/visit");
	HF_DeallocateRecord(&sadd);
	return (HIS_UnLock(retvalfinal));
    }
    GS_root.study[GS_root.free_s].badd.hunk_number = sadd.hunk_number;
    GS_root.study[GS_root.free_s].badd.node_number = sadd.node_number;
    strcpy(GS_root.study[GS_root.free_s].uid, study->SIUID);

    if (HF_WriteRecord(&sadd, sizeof(HIS_StudyLevel), (void *) study)
	!= HF_NORMAL) {
	COND_PushCondition(HIS_WRITERROR, "HIS_AddStudy: HF_WriteRecord error");
	return (HIS_UnLock(HIS_WRITERROR));
    }
    temp = GS_root.study[GS_root.free_s].next;
    GS_root.study[GS_root.free_s].next = GS_root.used_s;
    GS_root.used_s = GS_root.free_s;
    GS_root.free_s = temp;
    GS_root.num_studies++;

    if (HF_WriteStaticRecord(0, sizeof(GS_root), (void *) &GS_root)
	!= HF_NORMAL) {
	COND_PushCondition(HIS_WRITERROR, "HIS_AddStudy: HF_WriteStaticRecord error");
	return (HIS_UnLock(HIS_WRITERROR));
    }
    return (HIS_UnLock(HIS_NORMAL));
}

/* HIS_AddResults
**
** Purpose:
**	HIS_AddResults adds a results node to a HIS database.
**
** Parameter Dictionary:
**	short dbid:
**		The database identifer.
**	HIS_ResultsLevel *results:
**		The results level struct.
**
** Return Values:
**
**	HIS_NORMAL: 	The add results succeeded.
**	HIS_LOCKERROR:	Could not acquire a requested lock
**	HIS_NOTOPENED:	The database has not been opened.
**	HIS_BADPATIENT:	The patient specified counld not be found.
**	HIS_READERROR:	The hunk record could not be read.
**	HIS_TOOMANYRESULTS:Adding this results would exceed the maximum
**		allowed which is set at HIS_MAXRESULTS.
**	HIS_ALLOCATERROR:The new record could not be allocated.
**	HIS_WRITERROR:	The new record could not be written.
**
** Algorithm:
**	We perform the normal checks for database open, and the maximum number
**	of results allowed.  We check to make sure that a interpretation has not
**	been named as part of the results structure, as this is an error at
**	this point.  We then make sure we can locate the study specified in the
**	results structure as this is necessary to link this results with the
**	appropriate study.  Once all these checks have been performed, it is a
**	relatively simple process to actually add the results to the database.
**
*/
CONDITION
HIS_AddResults(short dbid, HIS_ResultsLevel *results)
{

    int i,
        temp;
    HunkBufAdd
        radd;
    CONDITION
	retval,
	retvalfinal;

    if (HIS_Findid(dbid) != HIS_NORMAL) {
	COND_PushCondition(HIS_NOTOPENED, "HIS_AddResults: HIS_Findid failed");
	return (HIS_NOTOPENED);
    }
    if (GS_root.num_results >= HIS_MAXRESULTS) {
	COND_PushCondition(HIS_TOOMANYRESULTS, "HIS_AddResults: Too many results");
	return (HIS_TOOMANYRESULTS);
    }
    if (GS_root.free_r == HIS_NULL) {
	COND_PushCondition(HIS_NOFREENODES, "HIS_AddResults: No free nodes");
	return (HIS_NOFREENODES);
    }
    if (results->RISequenceCount != 0) {
	COND_PushCondition(HIS_BADSEQCOUNT, "HIS_AddResults: Bad sequence count");
	return (HIS_BADSEQCOUNT);
    }
    /*
     * Check to make sure that any up pointers to studies exist...
     */
    for (i = 0; i < results->RSSequenceCount; i++) {
	if (HIS_FindStudy(results->RSSequence[i]) == HIS_NULL) {
	    COND_PushCondition(HIS_BADSTUDY, "HIS_AddResults: Study not found");
	    return (HIS_BADSTUDY);
	}
    }
    if (HF_ExclusiveLock() != HF_NORMAL) {
	COND_PushCondition(HIS_LOCKERROR, "HIS_AddResults: HF_ExclusiveLock failed");
	return (HIS_LOCKERROR);
    }
    /*
     * We may want to check for a duplicate results at some point, but not
     * now... just allocate the record needed.
     */

    if (HF_AllocateRecord(sizeof(HIS_ResultsLevel), &radd) != HF_NORMAL) {
	COND_PushCondition(HIS_ALLOCATERROR, "HIS_AddResults: HF_AllocateRecord failed");
	return (HIS_UnLock(HIS_ALLOCATERROR));
    }
    retvalfinal = HIS_NORMAL;
    for (i = 0; i < results->RSSequenceCount; i++) {
	if ((retval = HIS_AddResultstoStudy(results->RIUID, results->RSSequence[i]))
	    != HIS_NORMAL) {
	    retvalfinal = retval;
	}
    }

    if (retvalfinal != HIS_NORMAL) {
	COND_PushCondition(retvalfinal, "HIS_AddResults: Couldn't update study");
	HF_DeallocateRecord(&radd);
	return (HIS_UnLock(retvalfinal));
    }
    GS_root.results[GS_root.free_r].badd.hunk_number = radd.hunk_number;
    GS_root.results[GS_root.free_r].badd.node_number = radd.node_number;
    strcpy(GS_root.results[GS_root.free_r].uid, results->RIUID);

    if (HF_WriteRecord(&radd, sizeof(HIS_ResultsLevel), (void *) results)
	!= HF_NORMAL) {
	COND_PushCondition(HIS_WRITERROR, "HIS_AddResults: HF_WriteRecord error");
	return (HIS_UnLock(HIS_WRITERROR));
    }
    temp = GS_root.results[GS_root.free_r].next;
    GS_root.results[GS_root.free_r].next = GS_root.used_r;
    GS_root.used_r = GS_root.free_r;
    GS_root.free_r = temp;
    GS_root.num_results++;

    if (HF_WriteStaticRecord(0, sizeof(GS_root), (void *) &GS_root)
	!= HF_NORMAL) {
	COND_PushCondition(HIS_WRITERROR, "HIS_AddResults: HF_WriteStaticRecord error");
	return (HIS_UnLock(HIS_WRITERROR));
    }
    return (HIS_UnLock(HIS_NORMAL));
}

/* HIS_AddInterpretation
**
** Purpose:
**	HIS_AddInterpretation adds a interpretaions node to a HIS database.
**
** Parameter Dictionary:
**	short dbid:
**		The database identifer.
**	HIS_InterpretationLevel *interps:
**		The interpretation level struct.
**
** Return Values:
**
**	HIS_NORMAL: 	The add interpretation succeeded.
**	HIS_LOCKERROR:	Could not acquire a requested lock
**	HIS_NOTOPENED:	The database has not been opened.
**	HIS_BADPATIENT:	The patient specified counld not be found.
**	HIS_READERROR:	The hunk record could not be read.
**	HIS_TOOMANYINTERPS:Adding this interpretation would exceed the
**		maximum allowed which is set at HIS_MAXINTERPS.
**	HIS_ALLOCATERROR:The new record could not be allocated.
**	HIS_WRITERROR:	The new record could not be written.
**
** Algorithm:
**	We perform the normal checks for database open, and the maximum number
**	of interpretations allowed.  We then make sure we can locate the
**	result specified in the interpretations structure as this is
**	necessary to link this interpretations with the appropriate result.
**	Once all these checks have been performed, it is a relatively simple
**	process to actually add the interpretations to the database.
**
*/
CONDITION
HIS_AddInterpretation(short dbid, HIS_InterpretationLevel *interps)
{

    int i,
        temp;
    HunkBufAdd
        iadd;
    CONDITION
	retval,
	retvalfinal;

    if (HIS_Findid(dbid) != HIS_NORMAL) {
	COND_PushCondition(HIS_NOTOPENED, "HIS_AddInterpretation: HIS_Findid failed");
	return (HIS_NOTOPENED);
    }
    if (GS_root.num_interps >= HIS_MAXINTERPS) {
	COND_PushCondition(HIS_TOOMANYINTERPS,
			 "HIS_AddInterpretation: Too many interpretations");
	return (HIS_TOOMANYINTERPS);
    }
    if (GS_root.free_i == HIS_NULL) {
	COND_PushCondition(HIS_NOFREENODES, "HIS_AddInterpretation: No free nodes");
	return (HIS_NOFREENODES);
    }
    /*
     * Check to make sure that any up pointers to results exist...
     */
    for (i = 0; i < interps->RRSequenceCount; i++) {
	if (HIS_FindResults(interps->RRSequence[i]) == HIS_NULL) {
	    COND_PushCondition(HIS_BADRESULT, "HIS_AddInterpretation: Result not found");
	    return (HIS_BADRESULT);
	}
    }
    if (HF_ExclusiveLock() != HF_NORMAL) {
	COND_PushCondition(HIS_LOCKERROR, "HIS_AddInterpretation: HF_ExclusiveLock failed");
	return (HIS_LOCKERROR);
    }
    /*
     * We may want to check for a duplicate results at some point, but not
     * now... just allocate the record needed.
     */

    if (HF_AllocateRecord(sizeof(HIS_InterpretationLevel), &iadd) != HF_NORMAL) {
	COND_PushCondition(HIS_ALLOCATERROR,
			 "HIS_AddInterpretation: HF_AllocateRecord failed");
	return (HIS_UnLock(HIS_ALLOCATERROR));
    }
    retvalfinal = HIS_NORMAL;
    for (i = 0; i < interps->RRSequenceCount; i++) {
	if ((retval = HIS_AddInterpretationtoResult(interps->IIUID, interps->RRSequence[i]))
	    != HIS_NORMAL) {
	    retvalfinal = retval;
	}
    }

    if (retvalfinal != HIS_NORMAL) {
	COND_PushCondition(retvalfinal, "HIS_AddInterpretation: Couldn't update study");
	HF_DeallocateRecord(&iadd);
	return (HIS_UnLock(retvalfinal));
    }
    GS_root.interps[GS_root.free_i].badd.hunk_number = iadd.hunk_number;
    GS_root.interps[GS_root.free_i].badd.node_number = iadd.node_number;
    strcpy(GS_root.interps[GS_root.free_i].uid, interps->IIUID);

    if (HF_WriteRecord(&iadd, sizeof(HIS_InterpretationLevel), (void *) interps)
	!= HF_NORMAL) {
	COND_PushCondition(HIS_WRITERROR, "HIS_AddInterpretation: HF_WriteRecord error");
	return (HIS_UnLock(HIS_WRITERROR));
    }
    temp = GS_root.interps[GS_root.free_i].next;
    GS_root.interps[GS_root.free_i].next = GS_root.used_i;
    GS_root.used_i = GS_root.free_i;
    GS_root.free_i = temp;
    GS_root.num_interps++;

    if (HF_WriteStaticRecord(0, sizeof(GS_root), (void *) &GS_root)
	!= HF_NORMAL) {
	COND_PushCondition(HIS_WRITERROR, "HIS_AddInterpretation: HF_WriteStaticRecord error");
	return (HIS_UnLock(HIS_WRITERROR));
    }
    return (HIS_UnLock(HIS_NORMAL));
}

/* HIS_GetPatient
**
** Purpose:
**	HIS_GetPatient gets patient information from the database
**
** Parameter Dictionary:
**	short dbid:
**		The database identifer.
**	HIS_PatientLevel *patient:
**		The patient level struct to fill with information.
**
** Return Values:
**
**	HIS_NORMAL: 	The add interpretation succeeded.
**	HIS_LOCKERROR:	Could not acquire a requested lock
**	HIS_NOTOPENED:	The database has not been opened.
**	HIS_NOPATIENTS:	There are no patients in the database
**	HIS_PATNOTFOUND:The patient specified could not be located.
**	HIS_READERROR:	The record could not be read from the hunk file.
**
** Algorithm:
**	The normal checks are made for database open.  This routine is
**	typically used with HIS_GetNextPatient to retreive all the
**	patients from the database for some type of list processing.
**	HIS_GetPatient initializes the context and returns the first
**	patient found.  Subsequent calls to HIS_GetNextPatient can then
**	be used to retreive other patients until HIS_NOMORE is returned
**	indicating that no more patients exist.  HIS_GetPatient can be used
**	in one other way.  If a non-null patientUID is specified in  the
**	patient level structure, this routine will attempt to find and
**	return the information on that patient.  It is not recommended
**	to use HIS_GetPatient in this manner and then used HIS_GetNextPatient.
**	Unexpected results could occur.
**
*/
CONDITION
HIS_GetPatient(short dbid, HIS_PatientLevel *patient)
{

    int
        pindex;
    HunkBufAdd
        padd;

    if (HIS_Findid(dbid) != HIS_NORMAL) {
	COND_PushCondition(HIS_NOTOPENED, "HIS_GetPatient: HIS_Findid failed");
	return (HIS_NOTOPENED);
    }
    if (GS_root.num_patients <= 0) {
	COND_PushCondition(HIS_NOPATIENTS, "HIS_GetPatient: No patients to retreive");
	return (HIS_NOPATIENTS);
    }
    if (HF_SharedLock() != HF_NORMAL) {
	COND_PushCondition(HIS_LOCKERROR, "HIS_GetPatient: HF_SharedLock failed");
	return (HIS_LOCKERROR);
    }
    if (strcmp(patient->PIUID, "") != 0) {
	if ((pindex = HIS_FindPatient(patient->PIUID)) == HIS_NULL) {
	    COND_PushCondition(HIS_PATNOTFOUND, "HIS_GetPatient: Patient not found");
	    return (HIS_UnLock(HIS_PATNOTFOUND));
	}
	padd = GS_root.patient[pindex].badd;
	if (HF_ReadRecord(&padd, sizeof(HIS_PatientLevel), (void *) patient) != HF_NORMAL) {
	    COND_PushCondition(HIS_READERROR, "HIS_GetPatient: HF_ReadRecord failed");
	    return (HIS_UnLock(HIS_READERROR));
	}
	return (HIS_UnLock(HIS_NORMAL));
    }
    GS_context->patient_context = GS_root.used_p;
    padd = GS_root.patient[GS_root.used_p].badd;

    if (HF_ReadRecord(&padd, sizeof(HIS_PatientLevel), (void *) patient) != HF_NORMAL) {
	COND_PushCondition(HIS_READERROR, "HIS_GetPatient: HF_ReadRecord failed");
	GS_context->patient_context = HIS_NULL;
	return (HIS_UnLock(HIS_READERROR));
    }
    return (HIS_UnLock(HIS_NORMAL));
}

/* HIS_GetNextPatient
**
** Purpose:
**	HIS_GetNextPatient gets patient information from the database
**
** Parameter Dictionary:
**	short dbid:
**		The database identifer.
**	HIS_PatientLevel *patient:
**		The patient level struct to fill with information.
**
** Return Values:
**
**	HIS_NORMAL: 	The add interpretation succeeded.
**	HIS_LOCKERROR:	Could not acquire a requested lock
**	HIS_BADCONTEXT:	Could not pick up the old context
**	HIS_NOTOPENED:	The database has not been opened.
**	HIS_NOMORE:	There are no more patients to be retreived
**	HIS_READERROR:	The record could not be read from the hunk file.
**
** Algorithm:
**	The normal checks are made for database open.  This routine is
**	typically used repeatedly following HIS_GetPatient to retreive
**	all the patient records for some type of list processing.
**	HIS_GetPatient must be called first to establish the context, and
**	then this routine can be called to retreive all records until
**	it returns HIS_NOMORE.
**
*/
CONDITION
HIS_GetNextPatient(short dbid, HIS_PatientLevel *patient)
{

    int
        pindex;

    HunkBufAdd
        padd;

    if (HIS_Findid(dbid) != HIS_NORMAL) {
	COND_PushCondition(HIS_NOTOPENED, "HIS_GetNextPatient: HIS_Findid failed");
	return (HIS_NOTOPENED);
    }
    if (GS_context->patient_context == HIS_NULL) {
	COND_PushCondition(HIS_BADCONTEXT, "HIS_GetNextPatient: Bad context");
	return (HIS_BADCONTEXT);
    }
    pindex = GS_root.patient[GS_context->patient_context].next;
    if (pindex == HIS_NULL) {
	GS_context->patient_context = HIS_NULL;
	return (HIS_NOMORE);
    }
    if (HF_SharedLock() != HF_NORMAL) {
	COND_PushCondition(HIS_LOCKERROR, "HIS_GetNextPatient: HF_SharedLock failed");
	return (HIS_LOCKERROR);
    }
    GS_context->patient_context = pindex;
    padd = GS_root.patient[pindex].badd;

    if (HF_ReadRecord(&padd, sizeof(HIS_PatientLevel), (void *) patient) != HF_NORMAL) {
	COND_PushCondition(HIS_READERROR, "HIS_GetNextPatient: HF_ReadRecord failed");
	GS_context->patient_context = HIS_NULL;
	return (HIS_UnLock(HIS_READERROR));
    }
    return (HIS_UnLock(HIS_NORMAL));
}

/* HIS_GetVisit
**
** Purpose:
**	HIS_GetVisit gets visit information from the database
**
** Parameter Dictionary:
**	short dbid:
**		The database identifer.
**	HIS_VisitLevel *visit:
**		The visit level struct to fill with information.
**
** Return Values:
**
**	HIS_NORMAL: 	The add interpretation succeeded.
**	HIS_LOCKERROR:	Could not acquire a requested lock
**	HIS_NOTOPENED:	The database has not been opened.
**	HIS_NOVISITS:	There are no visits in the database
**	HIS_VISITNOTFOUND:The visit specified could not be located.
**	HIS_READERROR:	The record could not be read from the hunk file.
**
** Algorithm:
**	The normal checks are made for database open.  This routine is
**	typically used with HIS_GetNextVisit to retreive all the
**	visits from the database for some type of list processing.
**	HIS_GetVisit initializes the context and returns the first
**	visit found.  Subsequent calls to HIS_GetNextVisit can then
**	be used to retreive other visits until HIS_NOMORE is returned
**	indicating that no more visits exist.  HIS_GetVisit can be used
**	in one other way.  If a non-null visitUID is specified in  the
**	visit level structure, this routine will attempt to find and
**	return the information on that visit.  It is not recommended
**	to use HIS_GetVisit in this manner and then used HIS_GetNextVisit.
**	Unexpected results could occur.
**
*/
CONDITION
HIS_GetVisit(short dbid, HIS_VisitLevel *visit)
{

    int
        vindex;
    HunkBufAdd
        vadd;

    if (HIS_Findid(dbid) != HIS_NORMAL) {
	COND_PushCondition(HIS_NOTOPENED, "HIS_GetVisit: HIS_Findid failed");
	return (HIS_NOTOPENED);
    }
    if (GS_root.num_visits <= 0) {
	COND_PushCondition(HIS_NOVISITS, "HIS_GetVisit: No visits to retreive");
	return (HIS_NOVISITS);
    }
    if (HF_SharedLock() != HF_NORMAL) {
	COND_PushCondition(HIS_LOCKERROR, "HIS_GetVisit: HF_SharedLock failed");
	return (HIS_LOCKERROR);
    }
    if (strcmp(visit->PVIUID, "") != 0) {
	if ((vindex = HIS_FindVisit(visit->PVIUID)) == HIS_NULL) {
	    COND_PushCondition(HIS_VISITNOTFOUND, "HIS_GetVisit: Visit not found");
	    return (HIS_UnLock(HIS_VISITNOTFOUND));
	}
	vadd = GS_root.visit[vindex].badd;
	if (HF_ReadRecord(&vadd, sizeof(HIS_VisitLevel), (void *) visit) != HF_NORMAL) {
	    COND_PushCondition(HIS_READERROR, "HIS_GetVisit: HF_ReadRecord failed");
	    return (HIS_UnLock(HIS_READERROR));
	}
	return (HIS_UnLock(HIS_NORMAL));
    }
    GS_context->visit_context = GS_root.used_v;
    vadd = GS_root.visit[GS_root.used_v].badd;

    if (HF_ReadRecord(&vadd, sizeof(HIS_VisitLevel), (void *) visit) != HF_NORMAL) {
	COND_PushCondition(HIS_READERROR, "HIS_GetVisit: HF_ReadRecord failed");
	GS_context->visit_context = HIS_NULL;
	return (HIS_UnLock(HIS_READERROR));
    }
    return (HIS_UnLock(HIS_NORMAL));
}

/* HIS_GetNextVisit
**
** Purpose:
**	HIS_GetNextVisit gets visit information from the database
**
** Parameter Dictionary:
**	short dbid:
**		The database identifer.
**	HIS_VisitLevel *visit:
**		The visit level struct to fill with information.
**
** Return Values:
**
**	HIS_NORMAL: 	The add interpretation succeeded.
**	HIS_LOCKERROR:	Could not acquire a requested lock
**	HIS_BADCONTEXT:	Could not pick up the old context
**	HIS_NOTOPENED:	The database has not been opened.
**	HIS_NOMORE:	There are no more visits to be retreived
**	HIS_READERROR:	The record could not be read from the hunk file.
**
** Algorithm:
**	The normal checks are made for database open.  This routine is
**	typically used repeatedly following HIS_GetVisit to retreive
**	all the visit records for some type of list processing.
**	HIS_GetVisit must be called first to establish the context, and
**	then this routine can be called to retreive all records until
**	it returns HIS_NOMORE.
**
*/
CONDITION
HIS_GetNextVisit(short dbid, HIS_VisitLevel *visit)
{

    int
        vindex;

    HunkBufAdd
        vadd;

    if (HIS_Findid(dbid) != HIS_NORMAL) {
	COND_PushCondition(HIS_NOTOPENED, "HIS_GetNextVisit: HIS_Findid failed");
	return (HIS_NOTOPENED);
    }
    if (GS_context->visit_context == HIS_NULL) {
	COND_PushCondition(HIS_BADCONTEXT, "HIS_GetNextVisit: Bad context");
	return (HIS_BADCONTEXT);
    }
    vindex = GS_root.visit[GS_context->visit_context].next;
    if (vindex == HIS_NULL) {
	GS_context->visit_context = HIS_NULL;
	return (HIS_NOMORE);
    }
    if (HF_SharedLock() != HF_NORMAL) {
	COND_PushCondition(HIS_LOCKERROR, "HIS_GetNextVisit: HF_SharedLock failed");
	return (HIS_LOCKERROR);
    }
    GS_context->visit_context = vindex;
    vadd = GS_root.visit[vindex].badd;

    if (HF_ReadRecord(&vadd, sizeof(HIS_VisitLevel), (void *) visit) != HF_NORMAL) {
	COND_PushCondition(HIS_READERROR, "HIS_GetNextVisit: HF_ReadRecord failed");
	GS_context->visit_context = HIS_NULL;
	return (HIS_UnLock(HIS_READERROR));
    }
    return (HIS_UnLock(HIS_NORMAL));
}

/* HIS_GetStudy
**
** Purpose:
**	HIS_GetStudy gets study information from the database
**
** Parameter Dictionary:
**	short dbid:
**		The database identifer.
**	HIS_StudyLevel *study:
**		The study level struct to fill with information.
**
** Return Values:
**
**	HIS_NORMAL: 	The add interpretation succeeded.
**	HIS_LOCKERROR:	Could not acquire a requested lock
**	HIS_NOTOPENED:	The database has not been opened.
**	HIS_NOVISITS:	There are no studies in the database
**	HIS_STUDYNOTFOUND:The study specified could not be located.
**	HIS_READERROR:	The record could not be read from the hunk file.
**
** Algorithm:
**	The normal checks are made for database open.  This routine is
**	typically used with HIS_GetNextStudy to retreive all the
**	studies from the database for some type of list processing.
**	HIS_GetStudy initializes the context and returns the first
**	study found.  Subsequent calls to HIS_GetNextStudy can then
**	be used to retreive other studies until HIS_NOMORE is returned
**	indicating that no more studies exist.  HIS_GetStudy can be used
**	in one other way.  If a non-null study UID is specified in  the
**	study level structure, this routine will attempt to find and
**	return the information on that visit.  It is not recommended
**	to use HIS_GetStudy in this manner and then used HIS_GetNextStudy.
**	Unexpected results could occur.
**
*/
CONDITION
HIS_GetStudy(short dbid, HIS_StudyLevel *study)
{

    int
        sindex;
    HunkBufAdd
        sadd;

    if (HIS_Findid(dbid) != HIS_NORMAL) {
	COND_PushCondition(HIS_NOTOPENED, "HIS_GetStudy: HIS_Findid failed");
	return (HIS_NOTOPENED);
    }
    if (GS_root.num_studies <= 0) {
	COND_PushCondition(HIS_NOSTUDIES, "HIS_GetStudy: No studies to retreive");
	return (HIS_NOSTUDIES);
    }
    if (HF_SharedLock() != HF_NORMAL) {
	COND_PushCondition(HIS_LOCKERROR, "HIS_GetStudy: HF_SharedLock failed");
	return (HIS_LOCKERROR);
    }
    if (strcmp(study->SIUID, "") != 0) {
	if ((sindex = HIS_FindStudy(study->SIUID)) == HIS_NULL) {
	    COND_PushCondition(HIS_STUDYNOTFOUND, "HIS_GetStudy: Study not found");
	    return (HIS_UnLock(HIS_STUDYNOTFOUND));
	}
	sadd = GS_root.study[sindex].badd;
	if (HF_ReadRecord(&sadd, sizeof(HIS_StudyLevel), (void *) study) != HF_NORMAL) {
	    COND_PushCondition(HIS_READERROR, "HIS_GetStudy: HF_ReadRecord failed");
	    return (HIS_UnLock(HIS_READERROR));
	}
	return (HIS_UnLock(HIS_NORMAL));
    }
    GS_context->study_context = GS_root.used_s;
    sadd = GS_root.study[GS_root.used_s].badd;

    if (HF_ReadRecord(&sadd, sizeof(HIS_StudyLevel), (void *) study) != HF_NORMAL) {
	COND_PushCondition(HIS_READERROR, "HIS_GetStudy: HF_ReadRecord failed");
	GS_context->study_context = HIS_NULL;
	return (HIS_UnLock(HIS_READERROR));
    }
    return (HIS_UnLock(HIS_NORMAL));
}
/* HIS_GetNextStudy
**
** Purpose:
**	HIS_GetNextStudy gets study information from the database
**
** Parameter Dictionary:
**	short dbid:
**		The database identifer.
**	HIS_StudyLevel *study:
**		The study level struct to fill with information.
**
** Return Values:
**
**	HIS_NORMAL: 	The add interpretation succeeded.
**	HIS_LOCKERROR:	Could not acquire a requested lock
**	HIS_BADCONTEXT:	Could not pick up the old context
**	HIS_NOTOPENED:	The database has not been opened.
**	HIS_NOMORE:	There are no more studies to be retreived
**	HIS_READERROR:	The record could not be read from the hunk file.
**
** Algorithm:
**	The normal checks are made for database open.  This routine is
**	typically used repeatedly following HIS_GetStudy to retreive
**	all the study records for some type of list processing.
**	HIS_GetStudy must be called first to establish the context, and
**	then this routine can be called to retreive all records until
**	it returns HIS_NOMORE.
**
*/
CONDITION
HIS_GetNextStudy(short dbid, HIS_StudyLevel *study)
{
    int
        sindex;

    HunkBufAdd
        sadd;

    if (HIS_Findid(dbid) != HIS_NORMAL) {
	COND_PushCondition(HIS_NOTOPENED, "HIS_GetNextStudy: HIS_Findid failed");
	return (HIS_NOTOPENED);
    }
    if (GS_context->study_context == HIS_NULL) {
	COND_PushCondition(HIS_BADCONTEXT, "HIS_GetNextStudy: Bad context");
	return (HIS_BADCONTEXT);
    }
    sindex = GS_root.study[GS_context->study_context].next;
    if (sindex == HIS_NULL) {
	GS_context->study_context = HIS_NULL;
	return (HIS_NOMORE);
    }
    if (HF_SharedLock() != HF_NORMAL) {
	COND_PushCondition(HIS_LOCKERROR, "HIS_GetNextStudy: HF_SharedLock failed");
	return (HIS_LOCKERROR);
    }
    GS_context->study_context = sindex;
    sadd = GS_root.study[sindex].badd;

    if (HF_ReadRecord(&sadd, sizeof(HIS_StudyLevel), (void *) study) != HF_NORMAL) {
	COND_PushCondition(HIS_READERROR, "HIS_GetNextStudy: HF_ReadRecord failed");
	GS_context->study_context = HIS_NULL;
	return (HIS_UnLock(HIS_READERROR));
    }
    return (HIS_UnLock(HIS_NORMAL));
}

/* HIS_GetResults
**
** Purpose:
**	HIS_GetResults gets results information from the database
**
** Parameter Dictionary:
**	short dbid:
**		The database identifer.
**	HIS_ResultsLevel *results:
**		The results level struct to fill with information.
**
** Return Values:
**
**	HIS_NORMAL: 	The add interpretation succeeded.
**	HIS_LOCKERROR:	Could not acquire a requested lock
**	HIS_NOTOPENED:	The database has not been opened.
**	HIS_NOVISITS:	There are no results in the database
**	HIS_RESULTSNOTFOUND:The results specified could not be located.
**	HIS_READERROR:	The record could not be read from the hunk file.
**
** Algorithm:
**	The normal checks are made for database open.  This routine is
**	typically used with HIS_GetNextResults to retreive all the
**	results from the database for some type of list processing.
**	HIS_GetResults initializes the context and returns the first
**	results found.  Subsequent calls to HIS_GetNextResults can then
**	be used to retreive other results until HIS_NOMORE is returned
**	indicating that no more results exist.  HIS_GetResults can be used
**	in one other way.  If a non-null resultsUID is specified in  the
**	result level structure, this routine will attempt to find and
**	return the information on that result.  It is not recommended
**	to use HIS_GetResults in this manner and then used HIS_GetNextResults.
**	Unexpected results could occur.
**
*/
CONDITION
HIS_GetResults(short dbid, HIS_ResultsLevel *results)
{
    int
        rindex;
    HunkBufAdd
        radd;

    if (HIS_Findid(dbid) != HIS_NORMAL) {
	COND_PushCondition(HIS_NOTOPENED, "HIS_GetResults: HIS_Findid failed");
	return (HIS_NOTOPENED);
    }
    if (GS_root.num_results <= 0) {
	COND_PushCondition(HIS_NORESULTS, "HIS_GetResults: No results to retreive");
	return (HIS_NORESULTS);
    }
    if (HF_SharedLock() != HF_NORMAL) {
	COND_PushCondition(HIS_LOCKERROR, "HIS_GetResults: HF_SharedLock failed");
	return (HIS_LOCKERROR);
    }
    if (strcmp(results->RIUID, "") != 0) {
	if ((rindex = HIS_FindResults(results->RIUID)) == HIS_NULL) {
	    COND_PushCondition(HIS_RESULTSNOTFOUND, "HIS_GetResults: Results not found");
	    return (HIS_UnLock(HIS_RESULTSNOTFOUND));
	}
	radd = GS_root.results[rindex].badd;
	if (HF_ReadRecord(&radd, sizeof(HIS_ResultsLevel), (void *) results) != HF_NORMAL) {
	    COND_PushCondition(HIS_READERROR, "HIS_GetResults: HF_ReadRecord failed");
	    return (HIS_UnLock(HIS_READERROR));
	}
	return (HIS_UnLock(HIS_NORMAL));
    }
    GS_context->results_context = GS_root.used_r;
    radd = GS_root.results[GS_root.used_r].badd;

    if (HF_ReadRecord(&radd, sizeof(HIS_ResultsLevel), (void *) results) != HF_NORMAL) {
	COND_PushCondition(HIS_READERROR, "HIS_GetResults: HF_ReadRecord failed");
	GS_context->results_context = HIS_NULL;
	return (HIS_UnLock(HIS_READERROR));
    }
    return (HIS_UnLock(HIS_NORMAL));
}
/* HIS_GetNextResults
**
** Purpose:
**	HIS_GetNextResults gets results information from the database
**
** Parameter Dictionary:
**	short dbid:
**		The database identifer.
**	HIS_ResultsLevel *results:
**		The results level struct to fill with information.
**
** Return Values:
**
**	HIS_NORMAL: 	The add interpretation succeeded.
**	HIS_LOCKERROR:	Could not acquire a requested lock
**	HIS_BADCONTEXT:	Could not pick up the old context
**	HIS_NOTOPENED:	The database has not been opened.
**	HIS_NOMORE:	There are no more results to be retreived
**	HIS_READERROR:	The record could not be read from the hunk file.
**
** Algorithm:
**	The normal checks are made for database open.  This routine is
**	typically used repeatedly following HIS_GetResults to retreive
**	all the results records for some type of list processing.
**	HIS_GetResults must be called first to establish the context, and
**	then this routine can be called to retreive all records until
**	it returns HIS_NOMORE.
**
*/
CONDITION
HIS_GetNextResults(short dbid, HIS_ResultsLevel *results)
{
    int
        rindex;

    HunkBufAdd
        radd;

    if (HIS_Findid(dbid) != HIS_NORMAL) {
	COND_PushCondition(HIS_NOTOPENED, "HIS_GetNextResults: HIS_Findid failed");
	return (HIS_NOTOPENED);
    }
    if (GS_context->results_context == HIS_NULL) {
	COND_PushCondition(HIS_BADCONTEXT, "HIS_GetNextResults: Bad context");
	return (HIS_BADCONTEXT);
    }
    rindex = GS_root.results[GS_context->results_context].next;
    if (rindex == HIS_NULL) {
	GS_context->results_context = HIS_NULL;
	return (HIS_NOMORE);
    }
    if (HF_SharedLock() != HF_NORMAL) {
	COND_PushCondition(HIS_LOCKERROR, "HIS_GetNextResults: HF_SharedLock failed");
	return (HIS_LOCKERROR);
    }
    GS_context->results_context = rindex;
    radd = GS_root.results[rindex].badd;

    if (HF_ReadRecord(&radd, sizeof(HIS_ResultsLevel), (void *) results) != HF_NORMAL) {
	COND_PushCondition(HIS_READERROR, "HIS_GetNextResults: HF_ReadRecord failed");
	GS_context->results_context = HIS_NULL;
	return (HIS_UnLock(HIS_READERROR));
    }
    return (HIS_UnLock(HIS_NORMAL));
}

/* HIS_GetInterpretation
**
** Purpose:
**	HIS_GetInterpretation gets interps information from the database
**
** Parameter Dictionary:
**	short dbid:
**		The database identifer.
**	HIS_InterpretationLevel *interps:
**		The interps level struct to fill with information.
**
** Return Values:
**
**	HIS_NORMAL: 	The add interpretation succeeded.
**	HIS_LOCKERROR:	Could not acquire a requested lock
**	HIS_NOTOPENED:	The database has not been opened.
**	HIS_NOVISITS:	There are no interps in the database
**	HIS_INTERPSNOTFOUND:The interps specified could not be located.
**	HIS_READERROR:	The record could not be read from the hunk file.
**
** Algorithm:
**	The normal checks are made for database open.  This routine is
**	typically used with HIS_GetNextInterpretation to retreive all the
**	interps from the database for some type of list processing.
**	HIS_GetInterpretation initializes the context and returns the first
**	interps found.  Subsequent calls to HIS_GetNextInterpretation can then
**	be used to retreive other interps until HIS_NOMORE is returned
**	indicating that no more interps exist.  HIS_GetInterpretation can be used
**	in one other way.  If a non-null interpsUID is specified in  the
**	interps level structure, this routine will attempt to find and
**	return the information on that interps.  It is not recommended
**	to use HIS_GetInterpretation in this manner and then used
**	HIS_GetNextInterpretation.  Unexpected results could occur.
**
*/
CONDITION
HIS_GetInterpretation(short dbid, HIS_InterpretationLevel *interps)
{

    int
        iindex;
    HunkBufAdd
        iadd;

    if (HIS_Findid(dbid) != HIS_NORMAL) {
	COND_PushCondition(HIS_NOTOPENED, "HIS_GetInterpretation: HIS_Findid failed");
	return (HIS_NOTOPENED);
    }
    if (GS_root.num_interps <= 0) {
	COND_PushCondition(HIS_NOINTERPS,
		   "HIS_GetInterpretation: No interpretations to retreive");
	return (HIS_NOINTERPS);
    }
    if (HF_SharedLock() != HF_NORMAL) {
	COND_PushCondition(HIS_LOCKERROR, "HIS_GetInterpretation: HF_SharedLock failed");
	return (HIS_LOCKERROR);
    }
    if (strcmp(interps->IIUID, "") != 0) {
	if ((iindex = HIS_FindInterpretation(interps->IIUID)) == HIS_NULL) {
	    COND_PushCondition(HIS_INTERPSNOTFOUND,
			 "HIS_GetInterpretation: Interpretation not found");
	    return (HIS_UnLock(HIS_INTERPSNOTFOUND));
	}
	iadd = GS_root.interps[iindex].badd;
	if (HF_ReadRecord(&iadd, sizeof(HIS_InterpretationLevel),
			  (void *) interps) != HF_NORMAL) {
	    COND_PushCondition(HIS_READERROR,
			     "HIS_GetInterpretation: HF_ReadRecord failed");
	    return (HIS_UnLock(HIS_READERROR));
	}
	return (HIS_UnLock(HIS_NORMAL));
    }
    GS_context->interpretation_context = GS_root.used_i;
    iadd = GS_root.interps[GS_root.used_i].badd;

    if (HF_ReadRecord(&iadd, sizeof(HIS_InterpretationLevel),
		      (void *) interps) != HF_NORMAL) {
	COND_PushCondition(HIS_READERROR, "HIS_GetInterpretation: HF_ReadRecord failed");
	GS_context->interpretation_context = HIS_NULL;
	return (HIS_UnLock(HIS_READERROR));
    }
    return (HIS_UnLock(HIS_NORMAL));
}
/* HIS_GetNextInterpretation
**
** Purpose:
**	HIS_GetNextInterpretation gets interps information from the database
**
** Parameter Dictionary:
**	short dbid:
**		The database identifer.
**	HIS_InterpretationLevel *interps:
**		The interps level struct to fill with information.
**
** Return Values:
**
**	HIS_NORMAL: 	The add interpretation succeeded.
**	HIS_LOCKERROR:	Could not acquire a requested lock
**	HIS_BADCONTEXT:	Could not pick up the old context
**	HIS_NOTOPENED:	The database has not been opened.
**	HIS_NOMORE:	There are no more interps to be retreived
**	HIS_READERROR:	The record could not be read from the hunk file.
**
** Algorithm:
**	The normal checks are made for database open.  This routine is
**	typically used repeatedly following HIS_GetInterpretation to retreive
**	all the interps records for some type of list processing.
**	HIS_GetInterpretation must be called first to establish the context, and
**	then this routine can be called to retreive all records until
**	it returns HIS_NOMORE.
**
*/
CONDITION
HIS_GetNextInterpretation(short dbid, HIS_InterpretationLevel *interps)
{
    int
        iindex;

    HunkBufAdd
        iadd;

    if (HIS_Findid(dbid) != HIS_NORMAL) {
	COND_PushCondition(HIS_NOTOPENED, "HIS_GetNextInterpretation: HIS_Findid failed");
	return (HIS_NOTOPENED);
    }
    if (GS_context->interpretation_context == HIS_NULL) {
	COND_PushCondition(HIS_BADCONTEXT, "HIS_GetNextInterpretation: Bad context");
	return (HIS_BADCONTEXT);
    }
    iindex = GS_root.interps[GS_context->interpretation_context].next;
    if (iindex == HIS_NULL) {
	GS_context->interpretation_context = HIS_NULL;
	return (HIS_NOMORE);
    }
    if (HF_SharedLock() != HF_NORMAL) {
	COND_PushCondition(HIS_LOCKERROR, "HIS_GetNextInterpretation: HF_SharedLock failed");
	return (HIS_LOCKERROR);
    }
    GS_context->interpretation_context = iindex;
    iadd = GS_root.interps[iindex].badd;

    if (HF_ReadRecord(&iadd, sizeof(HIS_InterpretationLevel), (void *) interps) != HF_NORMAL) {
	COND_PushCondition(HIS_READERROR, "HIS_GetNextInterpretation: HF_ReadRecord failed");
	GS_context->interpretation_context = HIS_NULL;
	return (HIS_UnLock(HIS_READERROR));
    }
    return (HIS_UnLock(HIS_NORMAL));
}

/* CODE_SEQUENCE_cpy
**
** Purpose:
*	General Purpose function for copying contents of a CODE_SEQUNCE structure to another.
**
** Parameter Dictionary:
**	CODE_SEQUENCE dest[]
**		The structure where things are copied to.
**	CODE_SEQUENCE source[]
**		The structure where things are copied from.
**	int count
**		The size of the structures.
**
** Return Values:
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
void
CODE_SEQUENCE_cpy(CODE_SEQUENCE dest[], CODE_SEQUENCE source[], int count)
{
    int i;
    for (i = 0; i < count; i++) {
	strcpy(dest[i].CodeValue, source[i].CodeValue);
	strcpy(dest[i].CodingSchemeDesignator, source[i].CodingSchemeDesignator);
	strcpy(dest[i].CodeMeaning, source[i].CodeMeaning);
    }
}

/* HIS_UpdatePatient
**
** Purpose:
**	HIS_UpdatePatient updates information for a particular record
**
** Parameter Dictionary:
**	short dbid:
**		The database identifer.
**	HIS_PatientLevel *patient:
**		The patient level struct to use for update information
**
** Return Values:
**
**	HIS_NORMAL: 	The add interpretation succeeded.
**	HIS_LOCKERROR:	Could not acquire a requested lock
**	HIS_NOTOPENED:	The database has not been opened.
**	HIS_NOPATIENTS:	No patients to update
**	HIS_PATNOTFOUND:Specified patient to update was not found
**	HIS_READERROR:	The record could not be read from the hunk file.
**	HIS_WRITERROR:	The modified record could not be rewritten to the hunk file
**
** Algorithm:
**	The normal checks are made for database open. This routine is used to
**	update the record specified by the uid with the parameters marked
**	for update.  The Update_Flag in the patient level structure is used
**	as a bitmask to determine which fields to replace.  The update flags
**	can be found in hisdb.h.
**
*/
CONDITION
HIS_UpdatePatient(short dbid, HIS_PatientLevel *patient)
{

    int
        pindex;
    HunkBufAdd
        padd;
    HIS_PatientLevel
	newpat;

    if (HIS_Findid(dbid) != HIS_NORMAL) {
	COND_PushCondition(HIS_NOTOPENED, "HIS_UpdatePatient: HIS_Findid failed");
	return (HIS_NOTOPENED);
    }
    if (GS_root.num_patients <= 0) {
	COND_PushCondition(HIS_NOPATIENTS, "HIS_UpdatePatient: No patients to update");
	return (HIS_NOPATIENTS);
    }
    if (HF_ExclusiveLock() != HF_NORMAL) {
	COND_PushCondition(HIS_LOCKERROR, "HIS_UpdatePatient: HF_ExclusiveLock failed");
	return (HIS_LOCKERROR);
    }
    if (strcmp(patient->PIUID, "") != 0) {
	if ((pindex = HIS_FindPatient(patient->PIUID)) == HIS_NULL) {
	    COND_PushCondition(HIS_PATNOTFOUND, "HIS_UpdatePatient: Patient not specified");
	    return (HIS_UnLock(HIS_PATNOTFOUND));
	}
	padd = GS_root.patient[pindex].badd;
	if (HF_ReadRecord(&padd, sizeof(HIS_PatientLevel), (void *) &newpat) != HF_NORMAL) {
	    COND_PushCondition(HIS_READERROR, "HIS_UpdatePatient: HF_ReadRecord failed");
	    return (HIS_UnLock(HIS_READERROR));
	}
    } else {
	COND_PushCondition(HIS_PATNOTFOUND, "HIS_UpdatePatient: Patient not specified");
	return (HIS_UnLock(HIS_PATNOTFOUND));
    }

    /*
     * Now check each bit and perform the updates is necessary.
     */

    if (patient->Update_Flag & HIS_K_PATICDATE) {
	strcpy(newpat.ICDate, patient->ICDate);
    }
    if (patient->Update_Flag & HIS_K_PATICTIME) {
	strcpy(newpat.ICTime, patient->ICTime);
    }
    if (patient->Update_Flag & HIS_K_PATICUID) {
	strcpy(newpat.ICUID, patient->ICUID);
    }
    if (patient->Update_Flag & HIS_K_PATRVSEQUENCE) {
	/* Can't do this...too dangerous */
    }
    if (patient->Update_Flag & HIS_K_PATRSSEQUENCE) {
	/* Can't do this...too dangerous */
    }
    if (patient->Update_Flag & HIS_K_PATPNAME) {
	strcpy(newpat.PName, patient->PName);
    }
    if (patient->Update_Flag & HIS_K_PATPID) {
	strcpy(newpat.PID, patient->PID);
    }
    if (patient->Update_Flag & HIS_K_PATIOPID) {
	strcpy(newpat.IoPID, patient->IoPID);
    }
    if (patient->Update_Flag & HIS_K_PATOPIDS) {
	strcpy(newpat.OPIDs, patient->OPIDs);
    }
    if (patient->Update_Flag & HIS_K_PATOPNAMES) {
	strcpy(newpat.OPNames, patient->OPNames);
    }
    if (patient->Update_Flag & HIS_K_PATPADDRESS) {
	strcpy(newpat.PAddress, patient->PAddress);
    }
    if (patient->Update_Flag & HIS_K_PATPTNUMBERS) {
	strcpy(newpat.PTNumbers, patient->PTNumbers);
    }
    if (patient->Update_Flag & HIS_K_PATPBDATE) {
	strcpy(newpat.PBDate, patient->PBDate);
    }
    if (patient->Update_Flag & HIS_K_PATPSEX) {
	strcpy(newpat.PSex, patient->PSex);
    }
    if (patient->Update_Flag & HIS_K_PATEGROUP) {
	strcpy(newpat.EGroup, patient->EGroup);
    }
    if (patient->Update_Flag & HIS_K_PATRPREFERENCE) {
	strcpy(newpat.RPreference, patient->RPreference);
    }
    if (patient->Update_Flag & HIS_K_PATCALLERGIES) {
	strcpy(newpat.CAllergies, patient->CAllergies);
    }
    if (HF_WriteRecord(&padd, sizeof(HIS_PatientLevel), (void *) &newpat) != HF_NORMAL) {
	COND_PushCondition(HIS_WRITERROR, "HIS_UpdatePatient: HF_WriteRecord failed");
	return (HIS_UnLock(HIS_WRITERROR));
    }
    return (HIS_UnLock(HIS_NORMAL));
}

/* HIS_UpdateVisit
**
** Purpose:
**	HIS_UpdateVisit updates information for a particular record
**
** Parameter Dictionary:
**	short dbid:
**		The database identifer.
**	HIS_VisitLevel *visit:
**		The visit level struct to use for update information
**
** Return Values:
**
**	HIS_NORMAL: 	The add interpretation succeeded.
**	HIS_LOCKERROR:	Could not acquire a requested lock
**	HIS_NOTOPENED:	The database has not been opened.
**	HIS_NOVISITS:	No visits to update
**	HIS_VISITNOTFOUND:Specified visit to update was not found
**	HIS_READERROR:	The record could not be read from the hunk file.
**	HIS_WRITERROR:	The modified record could not be rewritten to the hunk file
**
** Algorithm:
**	The normal checks are made for database open. This routine is used to
**	update the record specified by the uid with the parameters marked
**	for update.  The Update_Flag in the visit level structure is used
**	as a bitmask to determine which fields to replace.  The update flags
**	can be found in hisdb.h.
**
*/
CONDITION
HIS_UpdateVisit(short dbid, HIS_VisitLevel *visit)
{

    int
        vindex;
    HunkBufAdd
        vadd;
    HIS_VisitLevel
	newvis;

    if (HIS_Findid(dbid) != HIS_NORMAL) {
	COND_PushCondition(HIS_NOTOPENED, "HIS_UpdateVisit: HIS_Findid failed");
	return (HIS_NOTOPENED);
    }
    if (GS_root.num_visits <= 0) {
	COND_PushCondition(HIS_NOVISITS, "HIS_UpdateVisit: No visits to update");
	return (HIS_NOVISITS);
    }
    if (HF_ExclusiveLock() != HF_NORMAL) {
	COND_PushCondition(HIS_LOCKERROR, "HIS_UpdateVisit: HF_ExclusiveLock failed");
	return (HIS_LOCKERROR);
    }
    if (strcmp(visit->PVIUID, "") != 0) {
	if ((vindex = HIS_FindVisit(visit->PVIUID)) == HIS_NULL) {
	    COND_PushCondition(HIS_VISITNOTFOUND, "HIS_UpdateVisit: Visit not specified");
	    return (HIS_UnLock(HIS_VISITNOTFOUND));
	}
	vadd = GS_root.visit[vindex].badd;
	if (HF_ReadRecord(&vadd, sizeof(HIS_VisitLevel), (void *) &newvis) != HF_NORMAL) {
	    COND_PushCondition(HIS_READERROR, "HIS_UpdateVisit: HF_ReadRecord failed");
	    return (HIS_UnLock(HIS_READERROR));
	}
    } else {
	COND_PushCondition(HIS_VISITNOTFOUND, "HIS_UpdateVisit: Visit not specified");
	return (HIS_UnLock(HIS_VISITNOTFOUND));
    }

    /*
     * Now check each bit and perform the updates is necessary.
     */

    if (visit->Update_Flag & HIS_K_VISICDATE) {
	strcpy(newvis.ICDate, visit->ICDate);
    }
    if (visit->Update_Flag & HIS_K_VISICTIME) {
	strcpy(newvis.ICTime, visit->ICTime);
    }
    if (visit->Update_Flag & HIS_K_VISICUID) {
	strcpy(newvis.ICUID, visit->ICUID);
    }
    if (visit->Update_Flag & HIS_K_VISRPSEQUENCE) {
	/* Can't do this...too dangerous */
    }
    if (visit->Update_Flag & HIS_K_VISRSSEQUENCE) {
	/* Can't do this...too dangerous */
    }
    if (visit->Update_Flag & HIS_K_VISIIDNAME) {
	strcpy(newvis.IIDName, visit->IIDName);
    }
    if (visit->Update_Flag & HIS_K_VISIOIID) {
	strcpy(newvis.IoIID, visit->IoIID);
    }
    if (visit->Update_Flag & HIS_K_VISAID) {
	strcpy(newvis.AID, visit->AID);
    }
    if (visit->Update_Flag & HIS_K_VISSADATE) {
	strcpy(newvis.SADate, visit->SADate);
    }
    if (visit->Update_Flag & HIS_K_VISSATIME) {
	strcpy(newvis.SATime, visit->SATime);
    }
    if (visit->Update_Flag & HIS_K_VISADATE) {
	strcpy(newvis.ADate, visit->ADate);
    }
    if (visit->Update_Flag & HIS_K_VISATIME) {
	strcpy(newvis.ATime, visit->ATime);
    }
    if (visit->Update_Flag & HIS_K_VISROADMISSIONS) {
	strcpy(newvis.RoAdmissions, visit->RoAdmissions);
    }
    if (visit->Update_Flag & HIS_K_VISRPHYSICIAN) {
	strcpy(newvis.RPhysician, visit->RPhysician);
    }
    if (visit->Update_Flag & HIS_K_VISADDESCRIPTION) {
	strcpy(newvis.ADDescription, visit->ADDescription);
    }
/*
    if (visit->Update_Flag & HIS_K_VISADCSUID) {
	strcpy(newvis.ADCSUID, visit->ADCSUID);
    }
    if (visit->Update_Flag & HIS_K_VISADCODE) {
	strcpy(newvis.ADcode, visit->ADcode);
    }
*/
    if (visit->Update_Flag & HIS_K_VISADCSEQUENCE) {
	if (visit->ADCSequenceCount > HIS_MAXVISADCSEQUENCE ||
	    visit->ADCSequenceCount < 0) {
	    COND_PushCondition(HIS_BADSEQUENCECOUNT, "HIS_UpdateVisit: ADC sequence Count is out of range");
	    return (HIS_UnLock(HIS_BADSEQUENCECOUNT));
	}
	CODE_SEQUENCE_cpy(newvis.ADCSequence, visit->ADCSequence, visit->ADCSequenceCount);
	newvis.ADCSequenceCount = visit->ADCSequenceCount;
    }
    if (visit->Update_Flag & HIS_K_VISVSID) {
	strcpy(newvis.VSID, visit->VSID);
    }
    if (visit->Update_Flag & HIS_K_VISCPLOCATION) {
	strcpy(newvis.CPLocation, visit->CPLocation);
    }
    if (visit->Update_Flag & HIS_K_VISSPIRESIDENCE) {
	strcpy(newvis.SPIResidence, visit->SPIResidence);
    }
    if (visit->Update_Flag & HIS_K_VISPIRESIDENCE) {
	strcpy(newvis.PIResidence, visit->PIResidence);
    }
    if (visit->Update_Flag & HIS_K_VISDDATE) {
	strcpy(newvis.DDate, visit->DDate);
    }
    if (visit->Update_Flag & HIS_K_VISDTIME) {
	strcpy(newvis.DTime, visit->DTime);
    }
    if (visit->Update_Flag & HIS_K_VISDDDESCRIPTION) {
	strcpy(newvis.DDDescription, visit->DDDescription);
    }
/*
    if (visit->Update_Flag & HIS_K_VISDDCSUID) {
	strcpy(newvis.DDCSUID, visit->DDCSUID);
    }
    if (visit->Update_Flag & HIS_K_VISDDCODE) {
	strcpy(newvis.DDCode, visit->DDCode);
    }
*/
    if (visit->Update_Flag & HIS_K_VISDDCSEQUENCE) {
	if (visit->DDCSequenceCount > HIS_MAXVISDDCSEQUENCE ||
	    visit->DDCSequenceCount < 0) {
	    COND_PushCondition(HIS_BADSEQUENCECOUNT, "HIS_UpdateVisit: DDC sequence Count is out of range");
	    return (HIS_UnLock(HIS_BADSEQUENCECOUNT));
	}
	CODE_SEQUENCE_cpy(newvis.DDCSequence, visit->DDCSequence, visit->DDCSequenceCount);
	newvis.DDCSequenceCount = visit->DDCSequenceCount;
    }
    if (HF_WriteRecord(&vadd, sizeof(HIS_VisitLevel), (void *) &newvis) != HF_NORMAL) {
	COND_PushCondition(HIS_WRITERROR, "HIS_UpdateVisit: HF_WriteRecord failed");
	return (HIS_UnLock(HIS_WRITERROR));
    }
    return (HIS_UnLock(HIS_NORMAL));
}

/* HIS_UpdateStudy
**
** Purpose:
**	HIS_UpdateStudy updates information for a particular record
**
** Parameter Dictionary:
**	short dbid:
**		The database identifer.
**	HIS_StudyLevel *study:
**		The study level struct to use for update information
**
** Return Values:
**
**	HIS_NORMAL: 	The add interpretation succeeded.
**	HIS_LOCKERROR:	Could not acquire a requested lock
**	HIS_NOTOPENED:	The database has not been opened.
**	HIS_NOSTUDIES:	No studies to update
**	HIS_STUDYNOTFOUND:Specified study to update was not found
**	HIS_READERROR:	The record could not be read from the hunk file.
**	HIS_WRITERROR:	The modified record could not be rewritten to the hunk file
**
** Algorithm:
**	The normal checks are made for database open. This routine is used to
**	update the record specified by the uid with the parameters marked
**	for update.  The Update_Flag in the study level structure is used
**	as a bitmask to determine which fields to replace.  The update flags
**	can be found in hisdb.h.
**
*/
CONDITION
HIS_UpdateStudy(short dbid, HIS_StudyLevel *study)
{

    int
        sindex;
    HunkBufAdd
        sadd;
    HIS_StudyLevel
	newstudy;

    if (HIS_Findid(dbid) != HIS_NORMAL) {
	COND_PushCondition(HIS_NOTOPENED, "HIS_UpdateStudy: HIS_Findid failed");
	return (HIS_NOTOPENED);
    }
    if (GS_root.num_studies <= 0) {
	COND_PushCondition(HIS_NOPATIENTS, "HIS_UpdateStudy: No studies to update");
	return (HIS_NOPATIENTS);
    }
    if (HF_ExclusiveLock() != HF_NORMAL) {
	COND_PushCondition(HIS_LOCKERROR, "HIS_UpdateStudy: HF_ExclusiveLock failed");
	return (HIS_LOCKERROR);
    }
    if (strcmp(study->SIUID, "") != 0) {
	if ((sindex = HIS_FindStudy(study->SIUID)) == HIS_NULL) {
	    COND_PushCondition(HIS_STUDYNOTFOUND, "HIS_UpdateStudy: Study not found");
	    return (HIS_UnLock(HIS_STUDYNOTFOUND));
	}
	sadd = GS_root.study[sindex].badd;
	if (HF_ReadRecord(&sadd, sizeof(HIS_StudyLevel), (void *) &newstudy) != HF_NORMAL) {
	    COND_PushCondition(HIS_READERROR, "HIS_UpdateStudy: HF_ReadRecord failed");
	    return (HIS_UnLock(HIS_READERROR));
	}
    } else {
	COND_PushCondition(HIS_STUDYNOTFOUND, "HIS_UpdateStudy: Study not specified");
	return (HIS_UnLock(HIS_STUDYNOTFOUND));
    }

    /*
     * Now check each bit and perform the updates is necessary.
     */

    if (study->Update_Flag & HIS_K_STUICDATE) {
	strcpy(newstudy.ICDate, study->ICDate);
    }
    if (study->Update_Flag & HIS_K_STUICTIME) {
	strcpy(newstudy.ICTime, study->ICTime);
    }
    if (study->Update_Flag & HIS_K_STUICUID) {
	strcpy(newstudy.ICUID, study->ICUID);
    }
    if (study->Update_Flag & HIS_K_STUANUMBER) {
	strcpy(newstudy.ANumber, study->ANumber);
    }
    if (study->Update_Flag & HIS_K_STURPSEQUENCE) {
	/* Can't do this...too dangerous */
    }
    if (study->Update_Flag & HIS_K_STURVSEQUENCE) {
	/* Can't do this...too dangerous */
    }
    if (study->Update_Flag & HIS_K_STUSID) {
	strcpy(newstudy.SID, study->SID);
    }
    if (study->Update_Flag & HIS_K_STUSIISSUER) {
	strcpy(newstudy.SIIssuer, study->SIIssuer);
    }
    if (study->Update_Flag & HIS_K_STUDESCRIPTION) {
	strcpy(newstudy.Description, study->Description);
    }
/*
    if (study->Update_Flag & HIS_K_STUPCSUID) {
	strcpy(newstudy.PCSUID, study->PCSUID);
    }
    if (study->Update_Flag & HIS_K_STUPCODE) {
	strcpy(newstudy.PCode, study->PCode);
    }
*/
    if (study->Update_Flag & HIS_K_STUPCSEQUENCE) {
	if (study->PCSequenceCount > HIS_MAXSTUPCSEQUENCE ||
	    study->PCSequenceCount < 0) {
	    COND_PushCondition(HIS_BADSEQUENCECOUNT, "HIS_UpdateStudy: PC sequence Count is out of range");
	    return (HIS_UnLock(HIS_BADSEQUENCECOUNT));
	}
	CODE_SEQUENCE_cpy(newstudy.PCSequence, study->PCSequence, study->PCSequenceCount);
	newstudy.PCSequenceCount = study->PCSequenceCount;
    }
    if (study->Update_Flag & HIS_K_STUSSID) {
	strcpy(newstudy.StudyStatusID, study->StudyStatusID);
    }
    if (study->Update_Flag & HIS_K_STUSPID) {
	strcpy(newstudy.StudyPriorityID, study->StudyPriorityID);
    }
    if (study->Update_Flag & HIS_K_STUSSSDATE) {
	strcpy(newstudy.SSSDate, study->SSSDate);
    }
    if (study->Update_Flag & HIS_K_STUSSSTIME) {
	strcpy(newstudy.SSSTime, study->SSSTime);
    }
    if (study->Update_Flag & HIS_K_STUSSLOCATION) {
	strcpy(newstudy.SSLocation, study->SSLocation);
    }
    if (study->Update_Flag & HIS_K_STUSSLATITLE) {
	strcpy(newstudy.SSLATitle, study->SSLATitle);
    }
    if (study->Update_Flag & HIS_K_STURFSTUDY) {
	strcpy(newstudy.RfStudy, study->RfStudy);
    }
    if (study->Update_Flag & HIS_K_STURPHYSICIAN) {
	strcpy(newstudy.RPhysician, study->RPhysician);
    }
    if (study->Update_Flag & HIS_K_STURPDESCRIPTION) {
	strcpy(newstudy.RPDescription, study->RPDescription);
    }
/*
    if (study->Update_Flag & HIS_K_STURPCSUID) {
	strcpy(newstudy.RPCSUID, study->RPCSUID);
    }
    if (study->Update_Flag & HIS_K_STURPCODE) {
	strcpy(newstudy.RPCode, study->RPCode);
    }
*/
    if (study->Update_Flag & HIS_K_STURPCSEQUENCE) {
	if (study->RPCSequenceCount > HIS_MAXSTURPCSEQUENCE ||
	    study->RPCSequenceCount < 0) {
	    COND_PushCondition(HIS_BADSEQUENCECOUNT, "HIS_UpdateStudy: RPC sequence Count is out of range");
	    return (HIS_UnLock(HIS_BADSEQUENCECOUNT));
	}
	CODE_SEQUENCE_cpy(newstudy.RPCSequence, study->RPCSequence, study->RPCSequenceCount);
	newstudy.RPCSequenceCount = study->RPCSequenceCount;
    }
    if (study->Update_Flag & HIS_K_STUSADATE) {
	strcpy(newstudy.SADate, study->SADate);
    }
    if (study->Update_Flag & HIS_K_STUSATIME) {
	strcpy(newstudy.SATime, study->SATime);
    }
    if (study->Update_Flag & HIS_K_STUSDATE) {
	strcpy(newstudy.SDate, study->SDate);
    }
    if (study->Update_Flag & HIS_K_STUSTIME) {
	strcpy(newstudy.STime, study->STime);
    }
    if (study->Update_Flag & HIS_K_STURSCSEQUENCE) {
	int i;
	if (study->RSCSequenceCount > HIS_MAXSTURSCSEQUENCE ||
	    study->RSCSequenceCount < 0) {
	    COND_PushCondition(HIS_BADSEQUENCECOUNT,
		     "HIS_UpdateStudy: RSC sequence Count is out of range");
	    return (HIS_UnLock(HIS_BADSEQUENCECOUNT));
	}
	for (i = 0; i < study->RSCSequenceCount; i++) {
	    strcpy(newstudy.RSCRSCUIDSequence[i], study->RSCRSCUIDSequence[i]);
	    strcpy(newstudy.RSCRSIUIDSequence[i], study->RSCRSIUIDSequence[i]);
	}
	newstudy.RSCSequenceCount = study->RSCSequenceCount;
    }
    if (study->Update_Flag & HIS_K_STUSCDATE) {
	strcpy(newstudy.SCDate, study->SCDate);
    }
    if (study->Update_Flag & HIS_K_STUSCTIME) {
	strcpy(newstudy.SCTime, study->SCTime);
    }
    if (study->Update_Flag & HIS_K_STUSVDATE) {
	strcpy(newstudy.SVDate, study->SVDate);
    }
    if (study->Update_Flag & HIS_K_STUSVTIME) {
	strcpy(newstudy.SVTime, study->SVTime);
    }
    if (study->Update_Flag & HIS_K_STUSRDATE) {
	strcpy(newstudy.SRDate, study->SRDate);
    }
    if (study->Update_Flag & HIS_K_STUSRTIME) {
	strcpy(newstudy.SRTime, study->SRTime);
    }
    if (study->Update_Flag2 & HIS_K_STURRSEQUENCE) {
	/* Can't do this...too dangerous */
    }
    if (HF_WriteRecord(&sadd, sizeof(HIS_StudyLevel), (void *) &newstudy) != HF_NORMAL) {
	COND_PushCondition(HIS_WRITERROR, "HIS_UpdateStudy: HF_WriteRecord failed");
	return (HIS_UnLock(HIS_WRITERROR));
    }
    return (HIS_UnLock(HIS_NORMAL));
}

/* HIS_UpdateResults
**
** Purpose:
**	HIS_UpdateResults updates information for a particular record
**
** Parameter Dictionary:
**	short dbid:
**		The database identifer.
**	HIS_ResultsLevel *results:
**		The results level struct to use for update information
**
** Return Values:
**
**	HIS_NORMAL: 	The add interpretation succeeded.
**	HIS_LOCKERROR:	Could not acquire a requested lock
**	HIS_NOTOPENED:	The database has not been opened.
**	HIS_NORESULTS:	No results to update
**	HIS_RESULTSNOTFOUND:Specified results to update was not found
**	HIS_READERROR:	The record could not be read from the hunk file.
**	HIS_WRITERROR:	The modified record could not be rewritten to the hunk file
**
** Algorithm:
**	The normal checks are made for database open. This routine is used to
**	update the record specified by the uid with the parameters marked
**	for update.  The Update_Flag in the results level structure is used
**	as a bitmask to determine which fields to replace.  The update flags
**	can be found in hisdb.h.
**
*/
CONDITION
HIS_UpdateResults(short dbid, HIS_ResultsLevel *results)
{

    int
        rindex;
    HunkBufAdd
        radd;
    HIS_ResultsLevel
	newres;

    if (HIS_Findid(dbid) != HIS_NORMAL) {
	COND_PushCondition(HIS_NOTOPENED, "HIS_UpdateResults: HIS_Findid failed");
	return (HIS_NOTOPENED);
    }
    if (GS_root.num_results <= 0) {
	COND_PushCondition(HIS_NORESULTS, "HIS_UpdateResults: No results to update");
	return (HIS_NORESULTS);
    }
    if (HF_ExclusiveLock() != HF_NORMAL) {
	COND_PushCondition(HIS_LOCKERROR, "HIS_UpdateResults: HF_ExclusiveLock failed");
	return (HIS_LOCKERROR);
    }
    if (strcmp(results->RIUID, "") != 0) {
	if ((rindex = HIS_FindResults(results->RIUID)) == HIS_NULL) {
	    COND_PushCondition(HIS_RESULTSNOTFOUND, "HIS_UpdateResults: Result not specified");
	    return (HIS_UnLock(HIS_RESULTSNOTFOUND));
	}
	radd = GS_root.results[rindex].badd;
	if (HF_ReadRecord(&radd, sizeof(HIS_ResultsLevel), (void *) &newres) != HF_NORMAL) {
	    COND_PushCondition(HIS_READERROR, "HIS_UpdateResults: HF_ReadRecord failed");
	    return (HIS_UnLock(HIS_READERROR));
	}
    } else {
	COND_PushCondition(HIS_RESULTSNOTFOUND, "HIS_UpdateResults: Patient not specified");
	return (HIS_UnLock(HIS_RESULTSNOTFOUND));
    }

    /*
     * Now check each bit and perform the updates is necessary.
     */

    if (results->Update_Flag & HIS_K_RESICDATE) {
	strcpy(newres.ICDate, results->ICDate);
    }
    if (results->Update_Flag & HIS_K_RESICTIME) {
	strcpy(newres.ICTime, results->ICTime);
    }
    if (results->Update_Flag & HIS_K_RESICUID) {
	strcpy(newres.ICUID, results->ICUID);
    }
    if (results->Update_Flag & HIS_K_RESRSSEQUENCE) {
	/* Can't do this...too dangerous */
    }
    if (results->Update_Flag & HIS_K_RESRISEQUENCE) {
	/* Can't do this...too dangerous */
    }
    if (results->Update_Flag & HIS_K_RESRID) {
	strcpy(newres.RID, results->RID);
    }
    if (results->Update_Flag & HIS_K_RESRIISSUER) {
	strcpy(newres.RIIssuer, results->RIIssuer);
    }
    if (results->Update_Flag & HIS_K_RESIMPRESSIONS) {
	strcpy(newres.Impressions, results->Impressions);
    }
    if (results->Update_Flag & HIS_K_RESCOMMENTS) {
	strcpy(newres.Comments, results->Comments);
    }
    if (HF_WriteRecord(&radd, sizeof(HIS_ResultsLevel), (void *) &newres) != HF_NORMAL) {
	COND_PushCondition(HIS_WRITERROR, "HIS_UpdateResults: HF_WriteRecord failed");
	return (HIS_UnLock(HIS_WRITERROR));
    }
    return (HIS_UnLock(HIS_NORMAL));
}

/* HIS_UpdateInterpretation
**
** Purpose:
**	HIS_UpdateInterpretation updates information for a particular record
**
** Parameter Dictionary:
**	short dbid:
**		The database identifer.
**	HIS_InterpretationLevel *interps:
**		The interps level struct to use for update information
**
** Return Values:
**
**	HIS_NORMAL: 	The add interpretation succeeded.
**	HIS_LOCKERROR:	Could not acquire a requested lock
**	HIS_NOTOPENED:	The database has not been opened.
**	HIS_NOINTERPS:	No interps to update
**	HIS_INTERPSNOTFOUND:Specified interps to update was not found
**	HIS_READERROR:	The record could not be read from the hunk file.
**	HIS_WRITERROR:	The modified record could not be rewritten to the hunk file
**
** Algorithm:
**	The normal checks are made for database open. This routine is used to
**	update the record specified by the uid with the parameters marked
**	for update.  The Update_Flag in the interps level structure is used
**	as a bitmask to determine which fields to replace.  The update flags
**	can be found in hisdb.h.
**
*/
CONDITION
HIS_UpdateInterpretation(short dbid, HIS_InterpretationLevel *interps)
{

    int
        iindex;
    HunkBufAdd
        iadd;
    HIS_InterpretationLevel
	newinterps;

    if (HIS_Findid(dbid) != HIS_NORMAL) {
	COND_PushCondition(HIS_NOTOPENED, "HIS_UpdateInterpretation: HIS_Findid failed");
	return (HIS_NOTOPENED);
    }
    if (GS_root.num_interps <= 0) {
	COND_PushCondition(HIS_NOINTERPS,
			   "HIS_UpdateInterpretation: No interps to update");
	return (HIS_NOINTERPS);
    }
    if (HF_ExclusiveLock() != HF_NORMAL) {
	COND_PushCondition(HIS_LOCKERROR,
		       "HIS_UpdateInterpretation: HF_ExclusiveLock failed");
	return (HIS_LOCKERROR);
    }
    if (strcmp(interps->IIUID, "") != 0) {
	if ((iindex = HIS_FindInterpretation(interps->IIUID)) == HIS_NULL) {
	    COND_PushCondition(HIS_INTERPSNOTFOUND,
			 "HIS_UpdateInterpretation: Interps not specified");
	    return (HIS_UnLock(HIS_INTERPSNOTFOUND));
	}
	iadd = GS_root.interps[iindex].badd;
	if (HF_ReadRecord(&iadd, sizeof(HIS_InterpretationLevel), (void *) &newinterps) !=
	    HF_NORMAL) {
	    COND_PushCondition(HIS_READERROR,
			  "HIS_UpdateInterpretation: HF_ReadRecord failed");
	    return (HIS_UnLock(HIS_READERROR));
	}
    } else {
	COND_PushCondition(HIS_INTERPSNOTFOUND,
			 "HIS_UpdateInterpretation: Interps not specified");
	return (HIS_UnLock(HIS_INTERPSNOTFOUND));
    }

    /*
     * Now check each bit and perform the updates is necessary.
     */

    if (interps->Update_Flag & HIS_K_INTICDATE) {
	strcpy(newinterps.ICDate, interps->ICDate);
    }
    if (interps->Update_Flag & HIS_K_INTICTIME) {
	strcpy(newinterps.ICTime, interps->ICTime);
    }
    if (interps->Update_Flag & HIS_K_INTICUID) {
	strcpy(newinterps.ICUID, interps->ICUID);
    }
    if (interps->Update_Flag & HIS_K_INTRRSEQUENCE) {
	/* Can't do this...too dangerous */
    }
    if (interps->Update_Flag & HIS_K_INTIID) {
	strcpy(newinterps.IID, interps->IID);
    }
    if (interps->Update_Flag & HIS_K_INTIIISSUER) {
	strcpy(newinterps.IIIssuer, interps->IIIssuer);
    }
    if (interps->Update_Flag & HIS_K_INTITID) {
	strcpy(newinterps.ITID, interps->ITID);
    }
    if (interps->Update_Flag & HIS_K_INTISID) {
	strcpy(newinterps.ISID, interps->ISID);
    }
    if (interps->Update_Flag & HIS_K_INTIRDATE) {
	strcpy(newinterps.IRDate, interps->IRDate);
    }
    if (interps->Update_Flag & HIS_K_INTIRTIME) {
	strcpy(newinterps.IRTime, interps->IRTime);
    }
    if (interps->Update_Flag & HIS_K_INTIRECORDER) {
	strcpy(newinterps.IRecorder, interps->IRecorder);
    }
    if (interps->Update_Flag & HIS_K_INTITRANSCRIBER) {
	strcpy(newinterps.ITranscriber, interps->ITranscriber);
    }
    if (interps->Update_Flag & HIS_K_INTITDATE) {
	strcpy(newinterps.ITDate, interps->ITDate);
    }
    if (interps->Update_Flag & HIS_K_INTITTIME) {
	strcpy(newinterps.ITTime, interps->ITTime);
    }
    if (interps->Update_Flag & HIS_K_INTIAUTHOR) {
	strcpy(newinterps.IAuthor, interps->IAuthor);
    }
    if (interps->Update_Flag & HIS_K_INTIAPPSEQUENCE) {
	int i;
	if (interps->IApprovalSequenceCount > HIS_MAXINTERPAPPROVSEQUENCE ||
	    interps->IApprovalSequenceCount < 0) {
	    COND_PushCondition(HIS_BADSEQUENCECOUNT, "HIS_UpdateInterp: Approval sequence Count is out of range");
	    return (HIS_UnLock(HIS_BADSEQUENCECOUNT));
	}
	for (i = 0; i < interps->IApprovalSequenceCount; i++) {
	    strcpy(newinterps.IApprover[i], interps->IApprover[i]);
	    strcpy(newinterps.IADates[i], interps->IADates[i]);
	    strcpy(newinterps.IATimes[i], interps->IATimes[i]);
	}
	newinterps.IApprovalSequenceCount = interps->IApprovalSequenceCount;
    }
/*
    if (interps->Update_Flag & HIS_K_INTIADATES) {
	strcpy(newinterps.IADates, interps->IADates);
    }
    if (interps->Update_Flag & HIS_K_INTIATIMES) {
	strcpy(newinterps.IATimes, interps->IATimes);
    }
    if (interps->Update_Flag & HIS_K_INTPAINTERPRETATION) {
	strcpy(newinterps.PAInterpretation, interps->PAInterpretation);
    }
*/
    if (interps->Update_Flag & HIS_K_INTIDIAGNOSIS) {
	strcpy(newinterps.IDiagnosis, interps->IDiagnosis);
    }
/*
    if (interps->Update_Flag & HIS_K_INTIDCSUID) {
	strcpy(newinterps.IDCSUID, interps->IDCSUID);
    }
    if (interps->Update_Flag & HIS_K_INTIDCODES) {
	strcpy(newinterps.IDCodes, interps->IDCodes);
    }
*/
    if (interps->Update_Flag & HIS_K_INTIDCSEQUENCE) {
	if (interps->IDCSequenceCount > HIS_MAXINTERPDCSEQUENCE ||
	    interps->IDCSequenceCount < 0) {
	    COND_PushCondition(HIS_BADSEQUENCECOUNT, "HIS_UpdateInterp: DC sequence Count is out of range");
	    return (HIS_UnLock(HIS_BADSEQUENCECOUNT));
	}
	CODE_SEQUENCE_cpy(newinterps.IDCSequence, interps->IDCSequence, interps->IDCSequenceCount);
	newinterps.IDCSequenceCount = interps->IDCSequenceCount;
    }
    if (interps->Update_Flag & HIS_K_INTITEXT) {
	strcpy(newinterps.IText, interps->IText);
    }
    if (interps->Update_Flag & HIS_K_INTRDLSEQUENCE) {
	strcpy(newinterps.RDLSequence, interps->RDLSequence);
    }
    if (HF_WriteRecord(&iadd, sizeof(HIS_InterpretationLevel), (void *) &newinterps) !=
	HF_NORMAL) {
	COND_PushCondition(HIS_WRITERROR,
			 "HIS_UpdateInterpretation: HF_WriteRecord failed");
	return (HIS_UnLock(HIS_WRITERROR));
    }
    return (HIS_UnLock(HIS_NORMAL));
}

/* HIS_DelPatient
**
** Purpose:
**	HIS_DelPatient deletes a particular patient record
**
** Parameter Dictionary:
**	short dbid:
**		The database identifer.
**	char *patuid
**		The uid identifier for the record to find and delete
**
** Return Values:
**
**	HIS_NORMAL: 	The add interpretation succeeded.
**	HIS_LOCKERROR:	Could not acquire a requested lock
**	HIS_NOTOPENED:	The database has not been opened.
**	HIS_NOPATIENTS:	No patients to delete
**	HIS_PATNOTFOUND:Specified patient to delete could not be found
**	HIS_READERROR:	The record could not be read from the hunk file.
**	HIS_WRITERROR:	The modified record could not be rewritten to the hunk file
**
** Algorithm:
**	The normal checks are made for database open. This routine is used to
**	delete the patient record specified by the uid passed.  Not being
**	able to find this record is an error.  The routine works by first
**	deleting everything below this node with utility routines, and then
**	deletes the patient node specified.
**
*/
CONDITION
HIS_DelPatient(short dbid, char *patuid)
{

    int
        i,
        temp,
        p,
        lastp,
        pindex;
    HunkBufAdd
        padd;
    HIS_PatientLevel
        patient;

    if (HIS_Findid(dbid) != HIS_NORMAL) {
	COND_PushCondition(HIS_NOTOPENED, "HIS_DelPatient: HIS_Findid failed");
	return (HIS_NOTOPENED);
    }
    if (GS_root.num_patients <= 0) {
	COND_PushCondition(HIS_NOPATIENTS,
			   "HIS_DelPatient: No patients to delete");
	return (HIS_NOPATIENTS);
    }
    if (HF_ExclusiveLock() != HF_NORMAL) {
	COND_PushCondition(HIS_LOCKERROR,
			   "HIS_DelPatient: HF_ExclusiveLock failed");
	return (HIS_LOCKERROR);
    }
    if ((pindex = HIS_FindPatient(patuid)) == HIS_NULL) {
	COND_PushCondition(HIS_PATNOTFOUND,
			   "HIS_DelPatient: Patient not found");
	return (HIS_UnLock(HIS_PATNOTFOUND));
    }
    padd = GS_root.patient[pindex].badd;
    if (HF_ReadRecord(&padd, sizeof(HIS_PatientLevel), (void *) &patient) !=
	HF_NORMAL) {
	COND_PushCondition(HIS_READERROR,
			   "HIS_DelPatient: HF_ReadRecord failed");
	return (HIS_UnLock(HIS_READERROR));
    }
    /*
     * Next we need to delete all the visits (and studies).
     */
    for (i = 0; i < patient.RVSequenceCount; i++) {
	if ((p = HIS_FindVisit(patient.RVSequence[i])) != HIS_NULL)
	    HIS_RemoveVLocal(p);
    }

    HF_DeallocateRecord(&padd);

    if (GS_root.used_p == pindex)
	GS_root.used_p = GS_root.patient[GS_root.used_p].next;
    else {
	lastp = GS_root.used_p;
	p = GS_root.patient[GS_root.used_p].next;
	while (p != HIS_NULL) {
	    if (p == pindex) {
		GS_root.patient[lastp].next = GS_root.patient[p].next;
		break;
	    }
	    lastp = p;
	    p = GS_root.patient[p].next;
	}
    }
    temp = GS_root.free_p;
    GS_root.free_p = pindex;
    GS_root.patient[GS_root.free_p].next = temp;

    GS_root.num_patients--;

    if (HF_WriteStaticRecord(0, sizeof(GS_root), (void *) &GS_root)
	!= HF_NORMAL) {
	COND_PushCondition(HIS_WRITERROR,
			   "HIS_DelPatient: HF_WriteStaticRecord error");
	return (HIS_UnLock(HIS_WRITERROR));
    }
    return (HIS_UnLock(HIS_NORMAL));
}

/* HIS_DelVisit
**
** Purpose:
**	HIS_DelVisit deletes a particular visit record
**
** Parameter Dictionary:
**	short dbid:
**		The database identifer.
**	char *visituid
**		The uid identifier for the record to find and delete
**
** Return Values:
**
**	HIS_NORMAL: 	The add interpretation succeeded.
**	HIS_LOCKERROR:	Could not acquire a requested lock
**	HIS_NOTOPENED:	The database has not been opened.
**	HIS_NOVISITS:	No visit to delete
**	HIS_VISITNOTFOUND:Specified visit to delete could not be found
**	HIS_READERROR:	The record could not be read from the hunk file.
**	HIS_WRITERROR:	The modified record could not be rewritten to the hunk file
**
** Algorithm:
**	The normal checks are made for database open. This routine is used to
**	delete the visit record specified by the uid passed.  Not being
**	able to find this record is an error.  The routine works by first
**	deleting everything below this node with utility routines, and then
**	deletes the visit node specified.
**
*/
CONDITION
HIS_DelVisit(short dbid, char *visituid)
{

    int
        i,
        temp,
        p,
        lastp,
        vindex;
    HunkBufAdd
        vadd;
    HIS_VisitLevel
        visit;

    if (HIS_Findid(dbid) != HIS_NORMAL) {
	COND_PushCondition(HIS_NOTOPENED, "HIS_DelVisit: HIS_Findid failed");
	return (HIS_NOTOPENED);
    }
    if (GS_root.num_visits <= 0) {
	COND_PushCondition(HIS_NOVISITS,
			   "HIS_DelVisit: No visits to delete");
	return (HIS_NOVISITS);
    }
    if (HF_ExclusiveLock() != HF_NORMAL) {
	COND_PushCondition(HIS_LOCKERROR,
			   "HIS_DelVisit: HF_ExclusiveLock failed");
	return (HIS_LOCKERROR);
    }
    if ((vindex = HIS_FindVisit(visituid)) == HIS_NULL) {
	COND_PushCondition(HIS_VISITNOTFOUND,
			   "HIS_DelVisit: Visit not found");
	return (HIS_UnLock(HIS_VISITNOTFOUND));
    }
    vadd = GS_root.visit[vindex].badd;
    if (HF_ReadRecord(&vadd, sizeof(HIS_VisitLevel), (void *) &visit) !=
	HF_NORMAL) {
	COND_PushCondition(HIS_READERROR,
			   "HIS_DelVisit: HF_ReadRecord failed");
	return (HIS_UnLock(HIS_READERROR));
    }
    /*
     * Before we delete this record, we need to remove references to this
     * node from any Patient nodes which point to it.
     */
    for (i = 0; i < visit.RPSequenceCount; i++) {
	HIS_RemoveVisitfromPatient(visituid, visit.RPSequence[i]);
    }

    /*
     * Next we need to delete all the studies.
     */
    for (i = 0; i < visit.RSSequenceCount; i++) {
	if ((p = HIS_FindStudy(visit.RSSequence[i])) != HIS_NULL)
	    HIS_RemoveSLocal(p);
    }

    HF_DeallocateRecord(&vadd);

    if (GS_root.used_v == vindex)
	GS_root.used_v = GS_root.visit[GS_root.used_v].next;
    else {
	lastp = GS_root.used_v;
	p = GS_root.visit[GS_root.used_v].next;
	while (p != HIS_NULL) {
	    if (p == vindex) {
		GS_root.visit[lastp].next = GS_root.visit[p].next;
		break;
	    }
	    lastp = p;
	    p = GS_root.visit[p].next;
	}
    }
    temp = GS_root.free_v;
    GS_root.free_v = vindex;
    GS_root.visit[GS_root.free_v].next = temp;

    GS_root.num_visits--;

    if (HF_WriteStaticRecord(0, sizeof(GS_root), (void *) &GS_root)
	!= HF_NORMAL) {
	COND_PushCondition(HIS_WRITERROR,
			   "HIS_DelVisit: HF_WriteStaticRecord error");
	return (HIS_UnLock(HIS_WRITERROR));
    }
    return (HIS_UnLock(HIS_NORMAL));
}

/* HIS_DelStudy
**
** Purpose:
**	HIS_DelStudy deletes a particular study record
**
** Parameter Dictionary:
**	short dbid:
**		The database identifer.
**	char *studyuid
**		The uid identifier for the record to find and delete
**
** Return Values:
**
**	HIS_NORMAL: 	The add interpretation succeeded.
**	HIS_LOCKERROR:	Could not acquire a requested lock
**	HIS_NOTOPENED:	The database has not been opened.
**	HIS_NOSTUDIES:	No study to delete
**	HIS_STUDYNOTFOUND:Specified study to delete could not be found
**	HIS_READERROR:	The record could not be read from the hunk file.
**	HIS_WRITERROR:	The modified record could not be rewritten to the hunk file
**
** Algorithm:
**	The normal checks are made for database open. This routine is used to
**	delete the study record specified by the uid passed.  Not being
**	able to find this record is an error.  The routine works by first
**	deleting everything below this node with utility routines, and then
**	deletes the study node specified.
**
*/
CONDITION
HIS_DelStudy(short dbid, char *studyuid)
{

    int
        i,
        temp,
        p,
        lastp,
        sindex;
    HunkBufAdd
        sadd;
    HIS_StudyLevel
        study;

    if (HIS_Findid(dbid) != HIS_NORMAL) {
	COND_PushCondition(HIS_NOTOPENED, "HIS_DelStudy: HIS_Findid failed");
	return (HIS_NOTOPENED);
    }
    if (GS_root.num_studies <= 0) {
	COND_PushCondition(HIS_NOSTUDIES,
			   "HIS_DelStudy: No studies to delete");
	return (HIS_NOSTUDIES);
    }
    if (HF_ExclusiveLock() != HF_NORMAL) {
	COND_PushCondition(HIS_LOCKERROR,
			   "HIS_DelStudy: HF_ExclusiveLock failed");
	return (HIS_LOCKERROR);
    }
    if ((sindex = HIS_FindStudy(studyuid)) == HIS_NULL) {
	COND_PushCondition(HIS_STUDYNOTFOUND,
			   "HIS_DelStudy: Study not found");
	return (HIS_UnLock(HIS_STUDYNOTFOUND));
    }
    sadd = GS_root.study[sindex].badd;
    if (HF_ReadRecord(&sadd, sizeof(HIS_StudyLevel), (void *) &study) !=
	HF_NORMAL) {
	COND_PushCondition(HIS_READERROR,
			   "HIS_DelStudy: HF_ReadRecord failed");
	return (HIS_UnLock(HIS_READERROR));
    }
    /*
     * Before we delete this record, we need to remove references to this
     * node from any Visits or Patient nodes which point to it.
     */
    for (i = 0; i < study.RVSequenceCount; i++) {
	HIS_RemoveStudyfromVisit(studyuid, study.RVSequence[i]);
    }
    for (i = 0; i < study.RPSequenceCount; i++) {
	HIS_RemoveStudyfromPatient(studyuid, study.RPSequence[i]);
    }

    /*
     * Next we need to delete all the results.
     * 
     */
    for (i = 0; i < study.RRSequenceCount; i++) {
	if ((p = HIS_FindResults(study.RRSequence[i])) != HIS_NULL)
	    HIS_RemoveRLocal(p);
    }

    HF_DeallocateRecord(&sadd);

    if (GS_root.used_s == sindex)
	GS_root.used_s = GS_root.study[GS_root.used_s].next;
    else {
	lastp = GS_root.used_s;
	p = GS_root.study[GS_root.used_s].next;
	while (p != HIS_NULL) {
	    if (p == sindex) {
		GS_root.study[lastp].next = GS_root.study[p].next;
		break;
	    }
	    lastp = p;
	    p = GS_root.study[p].next;
	}
    }
    temp = GS_root.free_s;
    GS_root.free_s = sindex;
    GS_root.study[GS_root.free_s].next = temp;

    GS_root.num_studies--;

    if (HF_WriteStaticRecord(0, sizeof(GS_root), (void *) &GS_root)
	!= HF_NORMAL) {
	COND_PushCondition(HIS_WRITERROR,
			   "HIS_DelStudy: HF_WriteStaticRecord error");
	return (HIS_UnLock(HIS_WRITERROR));
    }
    return (HIS_UnLock(HIS_NORMAL));
}

/* HIS_DelResults
**
** Purpose:
**	HIS_DelResults deletes a particular results record
**
** Parameter Dictionary:
**	short dbid:
**		The database identifer.
**	char *resultuid
**		The uid identifier for the record to find and delete
**
** Return Values:
**
**	HIS_NORMAL: 	The add interpretation succeeded.
**	HIS_LOCKERROR:	Could not acquire a requested lock
**	HIS_NOTOPENED:	The database has not been opened.
**	HIS_NORESULTS:	No results to delete
**	HIS_RESULTSNOTFOUND:Specified results to delete could not be found
**	HIS_READERROR:	The record could not be read from the hunk file.
**	HIS_WRITERROR:	The modified record could not be rewritten to the hunk file
**
** Algorithm:
**	The normal checks are made for database open. This routine is used to
**	delete the results record specified by the uid passed.  Not being
**	able to find this record is an error.  The routine works by first
**	deleting everything below this node with utility routines, and then
**	deletes the results node specified.
**
*/
CONDITION
HIS_DelResults(short dbid, char *resultsuid)
{

    int
        i,
        temp,
        p,
        lastp,
        rindex;
    HunkBufAdd
        radd;
    HIS_ResultsLevel
        results;

    if (HIS_Findid(dbid) != HIS_NORMAL) {
	COND_PushCondition(HIS_NOTOPENED, "HIS_DelResults: HIS_Findid failed");
	return (HIS_NOTOPENED);
    }
    if (GS_root.num_results <= 0) {
	COND_PushCondition(HIS_NORESULTS,
			   "HIS_DelResults: No results to delete");
	return (HIS_NORESULTS);
    }
    if (HF_ExclusiveLock() != HF_NORMAL) {
	COND_PushCondition(HIS_LOCKERROR,
			   "HIS_DelResults: HF_ExclusiveLock failed");
	return (HIS_LOCKERROR);
    }
    if ((rindex = HIS_FindResults(resultsuid)) == HIS_NULL) {
	COND_PushCondition(HIS_RESULTSNOTFOUND,
			   "HIS_DelResults: Results not found");
	return (HIS_UnLock(HIS_RESULTSNOTFOUND));
    }
    radd = GS_root.results[rindex].badd;
    if (HF_ReadRecord(&radd, sizeof(HIS_ResultsLevel), (void *) &results) !=
	HF_NORMAL) {
	COND_PushCondition(HIS_READERROR,
			   "HIS_DelResults: HF_ReadRecord failed");
	return (HIS_UnLock(HIS_READERROR));
    }
    /*
     * Before we delete this record, we need to remove references to this
     * node from any Studies nodes which point to it.
     */
    for (i = 0; i < results.RSSequenceCount; i++) {
	HIS_RemoveResultsfromStudy(resultsuid, results.RSSequence[i]);
    }

    /*
     * Next we need to delete all the interpretations.
     * 
     */
    for (i = 0; i < results.RISequenceCount; i++) {
	if ((p = HIS_FindInterpretation(results.RISequence[i])) != HIS_NULL)
	    HIS_RemoveILocal(p);
    }

    HF_DeallocateRecord(&radd);

    if (GS_root.used_r == rindex) {
	GS_root.used_r = GS_root.results[GS_root.used_r].next;
    } else {
	lastp = GS_root.used_r;
	p = GS_root.results[GS_root.used_r].next;
	while (p != HIS_NULL) {
	    if (p == rindex) {
		GS_root.results[lastp].next = GS_root.results[p].next;
		break;
	    }
	    lastp = p;
	    p = GS_root.results[p].next;
	}
    }

    temp = GS_root.free_r;
    GS_root.free_r = rindex;
    GS_root.results[GS_root.free_r].next = temp;

    GS_root.num_results--;

    if (HF_WriteStaticRecord(0, sizeof(GS_root), (void *) &GS_root)
	!= HF_NORMAL) {
	COND_PushCondition(HIS_WRITERROR,
			   "HIS_DelResults: HF_WriteStaticRecord error");
	return (HIS_UnLock(HIS_WRITERROR));
    }
    return (HIS_UnLock(HIS_NORMAL));


}

CONDITION
HIS_RemoveVLocal(int vindex)
{

    int
        i,
        p,
        lastp,
        temp;

    HIS_VisitLevel
        visit;

    HunkBufAdd
        vadd;

    vadd = GS_root.visit[vindex].badd;
    /*
     * Remove all studies first.
     */
    if (HF_ReadRecord(&vadd, sizeof(HIS_VisitLevel), (void *) &visit) ==
	HF_NORMAL) {
	for (i = 0; i < visit.RSSequenceCount; i++) {
	    if ((temp = HIS_FindStudy(visit.RSSequence[i])) != HIS_NULL) {
		HIS_RemoveSLocal(temp);
	    }
	}
    }
    HF_DeallocateRecord(&vadd);

    if (GS_root.used_v == vindex)
	GS_root.used_v = GS_root.visit[GS_root.used_v].next;
    else {
	lastp = GS_root.used_v;
	p = GS_root.visit[GS_root.used_v].next;
	if (p == HIS_NULL) {
	    GS_root.used_v = HIS_NULL;
	} else {
	    while (p != HIS_NULL) {
		if (p == vindex) {
		    GS_root.visit[lastp].next = GS_root.visit[p].next;
		    break;
		}
		lastp = p;
		p = GS_root.visit[p].next;
	    }
	}
    }
    temp = GS_root.free_v;
    GS_root.free_v = vindex;
    GS_root.visit[GS_root.free_v].next = temp;

    GS_root.num_visits--;

    return (HIS_NORMAL);
}

CONDITION
HIS_RemoveSLocal(int sindex)
{

    int
        i,
        p,
        lastp,
        temp;

    HIS_StudyLevel
        study;

    HunkBufAdd
        sadd;

    sadd = GS_root.study[sindex].badd;
    /*
     * Remove all results first.
     */
    if (HF_ReadRecord(&sadd, sizeof(HIS_StudyLevel), (void *) &study) ==
	HF_NORMAL) {
	for (i = 0; i < study.RRSequenceCount; i++) {
	    if ((temp = HIS_FindResults(study.RRSequence[i])) != HIS_NULL) {
		HIS_RemoveRLocal(temp);
	    }
	}
	for (i = 0; i < study.RPSequenceCount; i++) {
	    HIS_RemoveStudyfromPatient(study.SIUID, study.RPSequence[i]);
	}
    }
    HF_DeallocateRecord(&sadd);

    if (GS_root.used_s == sindex)
	GS_root.used_s = GS_root.study[GS_root.used_s].next;
    else {
	lastp = GS_root.used_s;
	p = GS_root.study[GS_root.used_s].next;
	if (p == HIS_NULL) {
	    GS_root.used_s = HIS_NULL;
	} else {
	    while (p != HIS_NULL) {
		if (p == sindex) {
		    GS_root.study[lastp].next = GS_root.study[p].next;
		    break;
		}
		lastp = p;
		p = GS_root.study[p].next;
	    }
	}
    }
    temp = GS_root.free_s;
    GS_root.free_s = sindex;
    GS_root.study[GS_root.free_s].next = temp;

    GS_root.num_studies--;

    return (HIS_NORMAL);
}

CONDITION
HIS_RemoveRLocal(int rindex)
{

    int
        i,
        p,
        lastp,
        temp;

    HIS_ResultsLevel
        results;

    HunkBufAdd
        radd;

    radd = GS_root.results[rindex].badd;
    /*
     * Remove all interpretations first.
     */
    if (HF_ReadRecord(&radd, sizeof(HIS_ResultsLevel), (void *) &results) ==
	HF_NORMAL) {
	for (i = 0; i < results.RISequenceCount; i++) {
	    if ((temp = HIS_FindInterpretation(results.RISequence[i])) != HIS_NULL) {
		HIS_RemoveILocal(temp);
	    }
	}
    }
    HF_DeallocateRecord(&radd);

    if (GS_root.used_r == rindex)
	GS_root.used_r = GS_root.results[GS_root.used_r].next;
    else {
	lastp = GS_root.used_r;
	p = GS_root.results[GS_root.used_r].next;
	if (p == HIS_NULL) {
	    GS_root.used_r = HIS_NULL;
	} else {
	    while (p != HIS_NULL) {
		if (p == rindex) {
		    GS_root.results[lastp].next = GS_root.results[p].next;
		    break;
		}
		lastp = p;
		p = GS_root.results[p].next;
	    }
	}
    }
    temp = GS_root.free_r;
    GS_root.free_r = rindex;
    GS_root.results[GS_root.free_r].next = temp;

    GS_root.num_results--;

    return (HIS_NORMAL);
}

CONDITION
HIS_RemoveILocal(int iindex)
{

    int
        p,
        lastp,
        temp;

    HunkBufAdd
        iadd;

    iadd = GS_root.interps[iindex].badd;
    HF_DeallocateRecord(&iadd);

    if (GS_root.used_i == iindex)
	GS_root.used_i = GS_root.interps[GS_root.used_i].next;
    else {
	lastp = GS_root.used_i;
	p = GS_root.interps[GS_root.used_i].next;
	if (p == HIS_NULL) {
	    GS_root.used_i = HIS_NULL;
	} else {
	    while (p != HIS_NULL) {
		if (p == iindex) {
		    GS_root.interps[lastp].next = GS_root.interps[p].next;
		    break;
		}
		lastp = p;
		p = GS_root.interps[p].next;
	    }
	}
    }
    temp = GS_root.free_i;
    GS_root.free_i = iindex;
    GS_root.interps[GS_root.free_i].next = temp;

    GS_root.num_interps--;

    return (HIS_NORMAL);
}

/* HIS_DelInterpretation
**
** Purpose:
**	HIS_DelInterpretation deletes a particular interpretation record
**
** Parameter Dictionary:
**	short dbid:
**		The database identifer.
**	char *interpsuid
**		The uid identifier for the record to find and delete
**
** Return Values:
**
**	HIS_NORMAL: 	The add interpretation succeeded.
**	HIS_LOCKERROR:	Could not acquire a requested lock
**	HIS_NOTOPENED:	The database has not been opened.
**	HIS_NOINTERPS:	No interps to delete
**	HIS_INTERPSNOTFOUND:Specified interps to delete could not be found
**	HIS_READERROR:	The record could not be read from the hunk file.
**	HIS_WRITERROR:	The modified record could not be rewritten to the hunk file
**
** Algorithm:
**	The normal checks are made for database open. This routine is used to
**	delete the interps record specified by the uid passed.  Not being
**	able to find this record is an error.  The routine works by first
**	deleting everything below this node with utility routines, and then
**	deletes the interps node specified.
**
*/
CONDITION
HIS_DelInterpretation(short dbid, char *interpuid)
{

    int
        i,
        temp,
        p,
        lastp,
        iindex;
    HunkBufAdd
        iadd;
    HIS_InterpretationLevel
        interps;

    if (HIS_Findid(dbid) != HIS_NORMAL) {
	COND_PushCondition(HIS_NOTOPENED, "HIS_DelInterpretation: HIS_Findid failed");
	return (HIS_NOTOPENED);
    }
    if (GS_root.num_interps <= 0) {
	COND_PushCondition(HIS_NOINTERPS,
			   "HIS_DelInterpretation: No interps to delete");
	return (HIS_NOINTERPS);
    }
    if (HF_ExclusiveLock() != HF_NORMAL) {
	COND_PushCondition(HIS_LOCKERROR,
			   "HIS_DelInterpretation: HF_ExclusiveLock failed");
	return (HIS_LOCKERROR);
    }
    if ((iindex = HIS_FindInterpretation(interpuid)) == HIS_NULL) {
	COND_PushCondition(HIS_INTERPSNOTFOUND,
			   "HIS_DelInterpretation: Interps not found");
	return (HIS_UnLock(HIS_INTERPSNOTFOUND));
    }
    iadd = GS_root.interps[iindex].badd;
    if (HF_ReadRecord(&iadd, sizeof(HIS_InterpretationLevel), (void *) &interps) !=
	HF_NORMAL) {
	COND_PushCondition(HIS_READERROR,
			   "HIS_DelInterpretation: HF_ReadRecord failed");
	return (HIS_UnLock(HIS_READERROR));
    }
    /*
     * Before we delete this record, we need to remove references to this
     * node from any Results nodes which point to it.
     */
    for (i = 0; i < interps.RRSequenceCount; i++) {
	HIS_RemoveInterpretationfromResult(interpuid, interps.RRSequence[i]);
    }

    HF_DeallocateRecord(&iadd);

    if (GS_root.used_i == iindex)
	GS_root.used_i = GS_root.interps[GS_root.used_i].next;
    else {
	lastp = GS_root.used_i;
	p = GS_root.interps[GS_root.used_i].next;
	while (p != HIS_NULL) {
	    if (p == iindex) {
		GS_root.interps[lastp].next = GS_root.interps[p].next;
		break;
	    }
	    lastp = p;
	    p = GS_root.interps[p].next;
	}
    }
    temp = GS_root.free_i;
    GS_root.free_i = iindex;
    GS_root.interps[GS_root.free_i].next = temp;

    GS_root.num_interps--;

    if (HF_WriteStaticRecord(0, sizeof(GS_root), (void *) &GS_root)
	!= HF_NORMAL) {
	COND_PushCondition(HIS_WRITERROR,
		       "HIS_DelInterpretation: HF_WriteStaticRecord error");
	return (HIS_UnLock(HIS_WRITERROR));
    }
    return (HIS_UnLock(HIS_NORMAL));


}

CONDITION
HIS_RemoveVisitfromPatient(char *visituid, char *patientuid)
{

    int
        i,
        j,
        pindex;
    HIS_PatientLevel
        patient;
    HunkBufAdd
        padd;

    pindex = HIS_FindPatient(patientuid);
    padd = GS_root.patient[pindex].badd;

    if (HF_ReadRecord(&padd, sizeof(HIS_PatientLevel), (void *) &patient) != HF_NORMAL) {
	return (HIS_READERROR);
    }
    if (patient.RVSequenceCount <= 0)
	return (HIS_NOVISITS);

    for (i = 0; i < patient.RVSequenceCount; i++) {
	if (strcmp(patient.RVSequence[i], visituid) == 0) {
	    for (j = i; j < patient.RVSequenceCount - 1; j++) {
		strcpy(patient.RVSequence[j], patient.RVSequence[j + 1]);
	    }
	    break;
	}
    }
    (patient.RVSequenceCount)--;

    if (HF_WriteRecord(&padd, sizeof(HIS_PatientLevel), (void *) &patient) != HF_NORMAL) {
	return (HIS_READERROR);
    }
    return (HIS_NORMAL);
}

CONDITION
HIS_RemoveStudyfromVisit(char *studyuid, char *visituid)
{

    int
        i,
        j,
        vindex;
    HIS_VisitLevel
        visit;
    HunkBufAdd
        vadd;

    vindex = HIS_FindVisit(visituid);
    vadd = GS_root.visit[vindex].badd;

    if (HF_ReadRecord(&vadd, sizeof(HIS_VisitLevel), (void *) &visit) != HF_NORMAL) {
	return (HIS_READERROR);
    }
    if (visit.RSSequenceCount <= 0)
	return (HIS_NOSTUDIES);

    for (i = 0; i < visit.RSSequenceCount; i++) {
	if (strcmp(visit.RSSequence[i], studyuid) == 0) {
	    for (j = i; j < visit.RSSequenceCount - 1; j++) {
		strcpy(visit.RSSequence[j], visit.RSSequence[j + 1]);
	    }
	    break;
	}
    }
    (visit.RSSequenceCount)--;

    if (HF_WriteRecord(&vadd, sizeof(HIS_VisitLevel), (void *) &visit) != HF_NORMAL) {
	return (HIS_READERROR);
    }
    return (HIS_NORMAL);
}

CONDITION
HIS_RemoveStudyfromPatient(char *studyuid, char *patientuid)
{

    int
        i,
        j,
        pindex;
    HIS_PatientLevel
        patient;
    HunkBufAdd
        padd;

    pindex = HIS_FindPatient(patientuid);
    padd = GS_root.patient[pindex].badd;

    if (HF_ReadRecord(&padd, sizeof(HIS_PatientLevel), (void *) &patient) != HF_NORMAL) {
	return (HIS_READERROR);
    }
    if (patient.RSSequenceCount <= 0)
	return (HIS_NOSTUDIES);

    for (i = 0; i < patient.RSSequenceCount; i++) {
	if (strcmp(patient.RSSequence[i], studyuid) == 0) {
	    for (j = i; j < patient.RSSequenceCount - 1; j++) {
		strcpy(patient.RSSequence[j], patient.RSSequence[j + 1]);
	    }
	    break;
	}
    }
    (patient.RSSequenceCount)--;

    if (HF_WriteRecord(&padd, sizeof(HIS_PatientLevel), (void *) &patient) != HF_NORMAL) {
	return (HIS_READERROR);
    }
    return (HIS_NORMAL);
}

CONDITION
HIS_RemoveResultsfromStudy(char *resultsuid, char *studyuid)
{

    int
        i,
        j,
        sindex;
    HIS_StudyLevel
        study;
    HunkBufAdd
        sadd;

    sindex = HIS_FindStudy(studyuid);
    sadd = GS_root.study[sindex].badd;

    if (HF_ReadRecord(&sadd, sizeof(HIS_StudyLevel), (void *) &study) != HF_NORMAL) {
	return (HIS_READERROR);
    }
    if (study.RRSequenceCount <= 0)
	return (HIS_NORESULTS);

    for (i = 0; i < study.RRSequenceCount; i++) {
	if (strcmp(study.RRSequence[i], resultsuid) == 0) {
	    for (j = i; j < study.RRSequenceCount - 1; j++) {
		strcpy(study.RRSequence[j], study.RRSequence[j + 1]);
	    }
	    break;
	}
    }
    (study.RRSequenceCount)--;

    if (HF_WriteRecord(&sadd, sizeof(HIS_StudyLevel), (void *) &study) != HF_NORMAL) {
	return (HIS_READERROR);
    }
    return (HIS_NORMAL);

}

CONDITION
HIS_RemoveInterpretationfromResult(char *interpsuid, char *resultsuid)
{

    int
        i,
        j,
        rindex;
    HIS_ResultsLevel
        results;
    HunkBufAdd
        radd;

    rindex = HIS_FindResults(resultsuid);
    radd = GS_root.results[rindex].badd;

    if (HF_ReadRecord(&radd, sizeof(HIS_ResultsLevel), (void *) &results) != HF_NORMAL) {
	return (HIS_READERROR);
    }
    if (results.RISequenceCount <= 0)
	return (HIS_NOINTERPS);

    for (i = 0; i < results.RISequenceCount; i++) {
	if (strcmp(results.RISequence[i], interpsuid) == 0) {
	    for (j = i; j < results.RISequenceCount - 1; j++) {
		strcpy(results.RISequence[j], results.RISequence[j + 1]);
	    }
	    break;
	}
    }
    (results.RISequenceCount)--;

    if (HF_WriteRecord(&radd, sizeof(HIS_ResultsLevel), (void *) &results) != HF_NORMAL) {
	return (HIS_READERROR);
    }
    return (HIS_NORMAL);

}

CONDITION
HIS_AddInterpretationtoResult(char *interpsuid, char *resultsuid)
{

    int
        rindex;
    HIS_ResultsLevel
        results;
    HunkBufAdd
        radd;

    rindex = HIS_FindResults(resultsuid);
    radd = GS_root.results[rindex].badd;

    if (HF_ReadRecord(&radd, sizeof(HIS_ResultsLevel), (void *) &results) != HF_NORMAL) {
	return (HIS_READERROR);
    }
    if (results.RISequenceCount == HIS_MAXINTERPSPERRES)
	return (HIS_TOOMANYINTERPS);

    strcpy(results.RISequence[results.RISequenceCount], interpsuid);
    (results.RISequenceCount)++;

    if (HF_WriteRecord(&radd, sizeof(HIS_ResultsLevel), (void *) &results) != HF_NORMAL) {
	return (HIS_READERROR);
    }
    return (HIS_NORMAL);

}

CONDITION
HIS_AddResultstoStudy(char *resultsuid, char *studyuid)
{

    int
        sindex;
    HIS_StudyLevel
        study;
    HunkBufAdd
        sadd;

    sindex = HIS_FindStudy(studyuid);
    sadd = GS_root.study[sindex].badd;

    if (HF_ReadRecord(&sadd, sizeof(HIS_StudyLevel), (void *) &study) != HF_NORMAL) {
	return (HIS_READERROR);
    }
    if (study.RRSequenceCount == HIS_MAXRESULTSPERSTUDY)
	return (HIS_TOOMANYRESULTS);

    strcpy(study.RRSequence[study.RRSequenceCount], resultsuid);
    (study.RRSequenceCount)++;

    if (HF_WriteRecord(&sadd, sizeof(HIS_StudyLevel), (void *) &study) != HF_NORMAL) {
	return (HIS_READERROR);
    }
    return (HIS_NORMAL);

}

CONDITION
HIS_AddVisittoPatient(char *visituid, char *patuid)
{

    int
        pindex;
    HIS_PatientLevel
        pat;
    HunkBufAdd
        patadd;

    pindex = HIS_FindPatient(patuid);
    patadd = GS_root.patient[pindex].badd;

    if (HF_ReadRecord(&patadd, sizeof(HIS_PatientLevel), (void *) &pat) != HF_NORMAL) {
	return (HIS_READERROR);
    }
    if (pat.RVSequenceCount == HIS_MAXVISITSPERPAT)
	return (HIS_TOOMANYVISITS);
    strcpy(pat.RVSequence[pat.RVSequenceCount], visituid);
    (pat.RVSequenceCount)++;

    if (HF_WriteRecord(&patadd, sizeof(HIS_PatientLevel), (void *) &pat) != HF_NORMAL) {
	return (HIS_READERROR);
    }
    return (HIS_NORMAL);

}

CONDITION
HIS_AddStudytoPatient(char *studyuid, char *patuid)
{

    int
        pindex;
    HIS_PatientLevel
        pat;
    HunkBufAdd
        patadd;

    pindex = HIS_FindPatient(patuid);
    patadd = GS_root.patient[pindex].badd;

    if (HF_ReadRecord(&patadd, sizeof(HIS_PatientLevel), (void *) &pat) != HF_NORMAL) {
	return (HIS_READERROR);
    }
    if (pat.RSSequenceCount == HIS_MAXSTUDIESPERPAT)
	return (HIS_TOOMANYSTUDS);
    strcpy(pat.RSSequence[pat.RSSequenceCount], studyuid);
    (pat.RSSequenceCount)++;

    if (HF_WriteRecord(&patadd, sizeof(HIS_PatientLevel), (void *) &pat) != HF_NORMAL) {
	return (HIS_READERROR);
    }
    return (HIS_NORMAL);

}
CONDITION
HIS_AddStudytoVisit(char *studyuid, char *visituid)
{

    int
        vindex;
    HIS_VisitLevel
        visit;
    HunkBufAdd
        vadd;

    vindex = HIS_FindVisit(visituid);
    vadd = GS_root.visit[vindex].badd;

    if (HF_ReadRecord(&vadd, sizeof(HIS_VisitLevel), (void *) &visit) != HF_NORMAL) {
	return (HIS_READERROR);
    }
    if (visit.RSSequenceCount == HIS_MAXSTUDIESPERVIS)
	return (HIS_TOOMANYSTUDS);
    strcpy(visit.RSSequence[visit.RSSequenceCount], studyuid);
    (visit.RSSequenceCount)++;

    if (HF_WriteRecord(&vadd, sizeof(HIS_VisitLevel), (void *) &visit) != HF_NORMAL) {
	return (HIS_READERROR);
    }
    return (HIS_NORMAL);
}
int
HIS_FindPatient(char *uid)
{

    int i,
        index;

    index = GS_root.used_p;
    for (i = 0; i < GS_root.num_patients; i++) {
	if (strcmp(uid, GS_root.patient[index].uid) == 0)
	    return (index);
	index = GS_root.patient[index].next;
    }

    return (HIS_NULL);
}
int
HIS_FindVisit(char *uid)
{

    int i,
        index;

    index = GS_root.used_v;
    for (i = 0; i < GS_root.num_visits; i++) {
	if (strcmp(uid, GS_root.visit[index].uid) == 0)
	    return (index);
	index = GS_root.visit[index].next;
    }

    return (HIS_NULL);
}
int
HIS_FindStudy(char *uid)
{

    int i,
        index;

    index = GS_root.used_s;
    for (i = 0; i < GS_root.num_studies; i++) {
	if (strcmp(uid, GS_root.study[index].uid) == 0)
	    return (index);
	index = GS_root.study[index].next;
    }

    return (HIS_NULL);
}
int
HIS_FindResults(char *uid)
{

    int i,
        index;

    index = GS_root.used_r;
    for (i = 0; i < GS_root.num_results; i++) {
	if (strcmp(uid, GS_root.results[index].uid) == 0)
	    return (index);
	index = GS_root.results[index].next;
    }

    return (HIS_NULL);
}
int
HIS_FindInterpretation(char *uid)
{

    int i,
        index;

    index = GS_root.used_i;
    for (i = 0; i < GS_root.num_interps; i++) {
	if (strcmp(uid, GS_root.interps[index].uid) == 0)
	    return (index);
	index = GS_root.interps[index].next;
    }

    return (HIS_NULL);
}

/* HIS_dumpDB
**
** Purpose:
**	HIS_dumpDB is a utility to dump the database to standard out.
**
** Parameter Dictionary:
**	short dbid:
**		The database identifer.
**
** Return Values:
**
**	none
**
** Algorithm:
**	Just a utility do dump the database mostly for diagnostic purposes...not really
**	meant for end users.
*/
void
HIS_DumpDB(short dbid)
{

    HunkBufAdd
        add;
    int
        i,
        j,
        index;

    HIS_PatientLevel
        p;
    HIS_VisitLevel
        v;
    HIS_StudyLevel
        s;
    HIS_ResultsLevel
        r;
    HIS_InterpretationLevel
        in;

    if (HIS_Findid(dbid) != HIS_NORMAL) {
	COND_PushCondition(HIS_NOTOPENED, "HIS_DumpDB: HIS_Findid failed");
	return;
    }
    printf("Database Key: %s\n", GS_root.dbkey);
    printf("Patients: %4d (free: %4d, used: %4d)\n",
	   GS_root.num_patients, GS_root.free_p, GS_root.used_p);
    printf("  Visits: %4d (free: %4d, used: %4d)\n",
	   GS_root.num_visits, GS_root.free_v, GS_root.used_v);
    printf(" Studies: %4d (free: %4d, used: %4d)\n",
	   GS_root.num_studies, GS_root.free_s, GS_root.used_s);
    printf(" Results: %4d (free: %4d, used: %4d)\n",
	   GS_root.num_results, GS_root.free_r, GS_root.used_r);
    printf(" Interps: %4d (free: %4d, used: %4d)\n",
	   GS_root.num_interps, GS_root.free_i, GS_root.used_i);

    printf("\n\n\t\t*** PATIENTS ***\n");
    index = GS_root.used_p;
    for (i = 0; i < GS_root.num_patients; i++) {
	add = GS_root.patient[index].badd;
	if (HF_ReadRecord(&add, sizeof(p), (void *) &p) != HF_NORMAL) {
	    return;
	}
	printf("Patient #%03d  UID: %s\n", i, p.PIUID);
	printf("\t#Visits: %d  #Studies: %d\n", p.RVSequenceCount, p.RSSequenceCount);
	for (j = 0; j < p.RVSequenceCount; j++)
	    printf("\tVisit: %2d  uid: %s\n", j, p.RVSequence[j]);
	for (j = 0; j < p.RSSequenceCount; j++)
	    printf("\tStudy: %2d  uid: %s\n", j, p.RSSequence[j]);

	index = GS_root.patient[index].next;
    }
    printf("\n\n\t\t*** VISITS ***\n");
    index = GS_root.used_v;
    for (i = 0; i < GS_root.num_visits; i++) {
	add = GS_root.visit[index].badd;
	if (HF_ReadRecord(&add, sizeof(v), (void *) &v) != HF_NORMAL) {
	    return;
	}
	printf("Visit #%03d  UID: %s\n", i, v.PVIUID);
	printf("\t#Patients: %d  #Studies: %d\n", v.RPSequenceCount, v.RSSequenceCount);
	for (j = 0; j < v.RPSequenceCount; j++)
	    printf("\tPatient: %2d  uid: %s\n", j, v.RPSequence[j]);
	for (j = 0; j < v.RSSequenceCount; j++)
	    printf("\t  Study: %2d  uid: %s\n", j, v.RSSequence[j]);

	index = GS_root.visit[index].next;
    }
    printf("\n\n\t\t*** STUDIES ***\n");
    index = GS_root.used_s;
    for (i = 0; i < GS_root.num_studies; i++) {
	add = GS_root.study[index].badd;
	if (HF_ReadRecord(&add, sizeof(s), (void *) &s) != HF_NORMAL) {
	    return;
	}
	printf("Study #%03d  UID: %s\n", i, s.SIUID);
	printf("\t#Patients: %d  #Visits: %d  #Results: %d\n",
	       s.RPSequenceCount, s.RVSequenceCount, s.RRSequenceCount);
	for (j = 0; j < s.RPSequenceCount; j++)
	    printf("\tPatient: %2d  uid: %s\n", j, s.RPSequence[j]);
	for (j = 0; j < s.RVSequenceCount; j++)
	    printf("\t  Visit: %2d  uid: %s\n", j, s.RVSequence[j]);
	for (j = 0; j < s.RRSequenceCount; j++)
	    printf("\tResults: %2d  uid: %s\n", j, s.RRSequence[j]);

	index = GS_root.study[index].next;
    }
    printf("\n\n\t\t*** RESULTS ***\n");
    index = GS_root.used_r;
    for (i = 0; i < GS_root.num_results; i++) {
	add = GS_root.results[index].badd;
	if (HF_ReadRecord(&add, sizeof(r), (void *) &r) != HF_NORMAL) {
	    return;
	}
	printf("Result #%03d  UID: %s\n", i, r.RIUID);
	printf("\t#Studies: %d  #Interpretations: %d\n",
	       r.RSSequenceCount, r.RISequenceCount);
	for (j = 0; j < r.RSSequenceCount; j++)
	    printf("\tStudies: %2d  uid: %s\n", j, r.RSSequence[j]);
	for (j = 0; j < r.RISequenceCount; j++)
	    printf("\tInterps: %2d  uid: %s\n", j, r.RISequence[j]);

	index = GS_root.results[index].next;
    }
    printf("\n\n\t\t*** INTERPRETATIONS ***\n");
    index = GS_root.used_i;
    for (i = 0; i < GS_root.num_interps; i++) {
	add = GS_root.interps[index].badd;
	if (HF_ReadRecord(&add, sizeof(in), (void *) &in) != HF_NORMAL) {
	    return;
	}
	printf("Interpretation #%03d  UID: %s\n", i, in.IIUID);
	printf("\t#Results: %d\n", in.RRSequenceCount);
	for (j = 0; j < in.RRSequenceCount; j++)
	    printf("\tResults: %2d  uid: %s\n", j, in.RRSequence[j]);

	index = GS_root.interps[index].next;
    }

    return;
}
/* HIS_UnLock
**
** Purpose:
**	Unlock the db file and return the proper return code.
**
** Parameter Dictionary:
**	CONDITION ret_val
**		The return value to return after unlocking...
**
** Return Values:
**	HIS_?:	The return value will always be what was passed.
**
** Algorithm:
**	Just a utility routine to keep the routines less messy.
*/
CONDITION
HIS_UnLock(CONDITION ret_val)
{

    HF_UnLock();
    return (ret_val);
}
