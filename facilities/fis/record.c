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
**				DICOM 94
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	FIS_Record
** Author, Date:	Stephen Moore, 12-Jul-94
** Intent:		This file contains functions for creating and
**			verifying new FIS records;
** Last Update:		$Author: smm $, $Date: 2001-01-03 21:38:37 $
** Source File:		$RCSfile: record.c,v $
** Revision:		$Revision: 1.10 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.10 $ $RCSfile: record.c,v $";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "utility.h"
#include "dicom_objects.h"
#include "tbl.h"
#include "manage.h"
#include "fis.h"
#include "dicom_uids.h"

#include "fis_private.h"

static CONDITION newPatientRecord(PRIVATE_HANDLE * h, FIS_PATIENTRECORD * record);
static CONDITION newStudyRecord(PRIVATE_HANDLE * h, FIS_STUDYRECORD * record);
static CONDITION
newStudyComponentRecord(PRIVATE_HANDLE * h,
			FIS_STUDYCOMPONENTRECORD * r);
static CONDITION newResultsRecord(PRIVATE_HANDLE * h, FIS_RESULTSRECORD * record);
static CONDITION newInterpretationRecord(PRIVATE_HANDLE * h, FIS_INTERPRETATIONRECORD * r);
static CONDITION
newStorageCommitReqRecord(PRIVATE_HANDLE * h,
			  FIS_STORAGECOMMITREQRECORD * r);
static CONDITION
newCommitSOPRefRecord(PRIVATE_HANDLE * h,
		      FIS_COMMITSOPREFRECORD * record);

/* FIS_NewRecord
**
** Purpose:
**
** Parameter Dictionary:
**
** Return Values:
**	FIS_NORMAL
**	FIS_NEWRECORDFAILED
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
FIS_NewRecord(FIS_HANDLE ** handle, FIS_DATA_TYPE type,
	      void *record)
{
    CONDITION
	cond = 0;
    PRIVATE_HANDLE
	* h;

    h = *(PRIVATE_HANDLE **) handle;

    switch (type) {
    case FIS_K_PATIENT:
	cond = newPatientRecord(h, (FIS_PATIENTRECORD *) record);
	break;
    case FIS_K_VISIT:
	break;
    case FIS_K_STUDY:
	cond = newStudyRecord(h, (FIS_STUDYRECORD *) record);
	break;
    case FIS_K_STUDYCOMPONENT:
	cond = newStudyComponentRecord(h, (FIS_STUDYCOMPONENTRECORD *) record);
	break;
    case FIS_K_RESULTS:
	cond = newResultsRecord(h, (FIS_RESULTSRECORD *) record);
	break;
    case FIS_K_INTERPRETATION:
	cond = newInterpretationRecord(h, (FIS_INTERPRETATIONRECORD *) record);
	break;
    case FIS_K_STORAGECOMMITREQ:
	cond = newStorageCommitReqRecord(h, (FIS_STORAGECOMMITREQRECORD *) record);
	break;
    case FIS_K_COMMITSOPREF:
	cond = newCommitSOPRefRecord(h, (FIS_COMMITSOPREFRECORD *) record);
	break;
    }
    if (cond != FIS_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_NEWRECORDFAILED),
				  "FIS_NewRecord");
    else
	return FIS_NORMAL;
}

/* newPatientRecord
**
** Purpose:
**
** Parameter Dictionary:
**
** Return Values:
**	FIS_NORMAL
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
newPatientRecord(PRIVATE_HANDLE * h, FIS_PATIENTRECORD * record)
{
    CONDITION
    cond;
    int patientNumber;

    if (record->Type != FIS_K_PATIENT)
	return COND_PushCondition(FIS_COND(FIS_ILLEGALRECORDTYPE),
				  "Patient Record", "(FIS)newPatientRecord");

    cond = UID_NewUID(UID_PATIENT, record->PatUID);
    if (cond != UID_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_UIDFAILED), "PATIENT",
				  "(FIS)newPatientRecord");

    cond = TBL_NextUnique(&h->uniquesHandle, "PatientID", &patientNumber);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_IDFAILED), "PATIENT",
				  "(FIS)newPatientRecord");

/*lint -e566*/
    sprintf(record->PatID, "ID%-08d", patientNumber);
/*lint +e566*/
    return FIS_NORMAL;
}

/* newStudyRecord
**
** Purpose:
**
** Parameter Dictionary:
**
** Return Values:
**	FIS_NORMAL
**	FIS_UIDFAILED
**	FIS_IDFAILED
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
newStudyRecord(PRIVATE_HANDLE * h, FIS_STUDYRECORD * record)
{
    CONDITION
    cond;
    int
        id;			/* Value returned from TBL for unique number */

    if (record->Type != FIS_K_STUDY)
	return COND_PushCondition(FIS_COND(FIS_ILLEGALRECORDTYPE),
				  "Study Record", "(FIS)newStudyRecord");

    cond = UID_NewUID(UID_STUDY, record->StuInsUID);
    if (cond != UID_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_UIDFAILED), "STUDY",
				  "(FIS)newStudyRecord");

    cond = UID_NewUID(UID_VISIT, record->VisUID);
    if (cond != UID_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_UIDFAILED), "VISIT",
				  "(FIS)newStudyRecord");

    cond = TBL_NextUnique(&h->uniquesHandle, "AccessionNumber", &id);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_ACCESSIONNUMBERFAILED),
				  "(FIS)newStudyRecord");
/*lint -e566*/
    sprintf(record->AccNum, "ID%-08d", id);
/*lint +e566*/

    cond = TBL_NextUnique(&h->uniquesHandle, "StudyID", &id);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_IDFAILED), "STUDY"
				  "(FIS)newStudyRecord");
/*lint -e566*/
    sprintf(record->StuID, "ID%-08d", id);
/*lint +e566*/

    return FIS_NORMAL;
}

static CONDITION
newStudyComponentRecord(PRIVATE_HANDLE * h, FIS_STUDYCOMPONENTRECORD * r)
{
    CONDITION cond;

    if (r->Type != FIS_K_STUDYCOMPONENT)
	return COND_PushCondition(FIS_COND(FIS_ILLEGALRECORDTYPE),
				  "Study Component Record",
				  "(FIS)newStudyComponentRecord");

    cond = UID_NewUID(UID_STUDYCOMPONENT, r->StuComUID);
    if (cond != UID_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_UIDFAILED), "STUDYCOMPONENT",
				  "(FIS)newStudyComponentRecord");

    return FIS_NORMAL;
}


/* newResultsRecord
**
** Purpose:
**
** Parameter Dictionary:
**
** Return Values:
**	FIS_NORMAL
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
newResultsRecord(PRIVATE_HANDLE * h, FIS_RESULTSRECORD * record)
{
    CONDITION cond;
    int id;			/* Value returned from TBL for unique number */

    if (record->Type != FIS_K_RESULTS)
	return COND_PushCondition(FIS_COND(FIS_ILLEGALRECORDTYPE),
				  "Results Record", "(FIS)newResultsRecord");

    cond = UID_NewUID(UID_RESULTS, record->ResUID);
    if (cond != UID_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_UIDFAILED), "RESULTS"
				  "(FIS)newResultsRecord");

    cond = TBL_NextUnique(&h->uniquesHandle, "ResultsID", &id);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_IDFAILED), "RESULTS",
				  "(FIS)newResultsRecord");
/*lint -e566*/
    sprintf(record->ResID, "RES%-08d", id);
/*lint +e566*/

    UTL_GetDicomDate(record->InsCreDat);
    UTL_GetDicomTime(record->InsCreTim);
    strcpy(record->InsCreUID, "");

    record->Flag = FIS_K_RESULTS_RESUID | FIS_K_RESULTS_RESID |
	FIS_K_RESULTS_INSCREDAT | FIS_K_RESULTS_INSCRETIM;

    return FIS_NORMAL;
}

/* newInterpretationRecord
**
** Purpose:
**
** Parameter Dictionary:
**
** Return Values:
**	FIS_NORMAL
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
newInterpretationRecord(PRIVATE_HANDLE * h, FIS_INTERPRETATIONRECORD * record)
{
    CONDITION cond;
    int id;			/* Value returned from TBL for unique number */

    if (record->Type != FIS_K_INTERPRETATION)
	return COND_PushCondition(FIS_COND(FIS_ILLEGALRECORDTYPE),
		   "Interpretation Record", "(FIS)newInterpretationRecord");

    cond = UID_NewUID(UID_INTERPRETATION, record->IntUID);
    if (cond != UID_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_UIDFAILED), "INTERPRETATION",
				  "(FIS)newInterpretationRecord");

    cond = TBL_NextUnique(&h->uniquesHandle, "InterpretationID", &id);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_IDFAILED), "INTERPRETATION",
				  "(FIS)newInterpretationRecord");
/*lint -e566*/
    sprintf(record->IntID, "INT%-08d", id);
/*lint +e566*/

    UTL_GetDicomDate(record->InsCreDat);
    UTL_GetDicomTime(record->InsCreTim);
    strcpy(record->InsCreUID, "");

    record->Flag = FIS_K_INTERP_INTUID | FIS_K_INTERP_INTID |
	FIS_K_INTERP_INSCREDAT | FIS_K_INTERP_INSCRETIM;


    return FIS_NORMAL;
}

static CONDITION
newStorageCommitReqRecord(PRIVATE_HANDLE * h,
			  FIS_STORAGECOMMITREQRECORD * record)
{
    CONDITION cond;

    if (record->Type != FIS_K_STORAGECOMMITREQ)
	return COND_PushCondition(FIS_COND(FIS_ILLEGALRECORDTYPE),
				  "Storage Commitment Request Record",
				  "(FIS)newStorageCommitReqRecord");

    memset(record, 0, sizeof(*record));
    record->Type = FIS_K_STORAGECOMMITREQ;

    cond = UID_NewUID(UID_STORAGECOMMITTRANSACTION, record->TraUID);
    if (cond != UID_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_UIDFAILED),
				  "STORAGE COMMITMENT TRANSACTION",
				  "(FIS)newStorageCommitReqRecord");

    UTL_GetDicomDate(record->ReqDat);
    UTL_GetDicomTime(record->ReqTim);

    record->Flag = FIS_K_SCOMMIT_TRAUID |
	FIS_K_SCOMMIT_REQDAT |
	FIS_K_SCOMMIT_REQTIM;

    return FIS_NORMAL;
}

static CONDITION
newCommitSOPRefRecord(PRIVATE_HANDLE * h,
		      FIS_COMMITSOPREFRECORD * record)
{
    if (record->Type != FIS_K_COMMITSOPREF)
	return COND_PushCondition(FIS_COND(FIS_ILLEGALRECORDTYPE),
				  "Commit SOP Reference Record",
				  "(FIS)newCommitSOPRefRecord");

    memset(record, 0, sizeof(*record));
    record->Type = FIS_K_COMMITSOPREF;

    return FIS_NORMAL;
}
