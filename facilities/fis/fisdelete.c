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
** Module Name(s):	FIS_Delete
** Author, Date:	Stephen Moore, 19-Jul-94
** Intent:		This file contains functions for deleting FIS records.
** Last Update:		$Author: smm $, $Date: 1997-04-11 22:00:30 $
** Source File:		$RCSfile: fisdelete.c,v $
** Revision:		$Revision: 1.8 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.8 $ $RCSfile: fisdelete.c,v $";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "dicom_objects.h"
#include "tbl.h"
#include "manage.h"
#include "fis.h"
#include "dicom_uids.h"
#include "utility.h"

#include "fis_private.h"

static CONDITION
deletePatientRecord(PRIVATE_HANDLE * h,
		    FIS_DATA_TYPE criteriaType, char *uid);
static CONDITION
deleteStudyRecord(PRIVATE_HANDLE * h,
		  FIS_DATA_TYPE criteriaType, char *uid);
static CONDITION
deleteStudyComponentRecord(PRIVATE_HANDLE * h,
			   FIS_DATA_TYPE criteriaType, char *uid);
static CONDITION
deleteSCSeriesRecord(PRIVATE_HANDLE * h,
		     FIS_DATA_TYPE criteriaType, char *uid);
static CONDITION
deleteSCImageRecord(PRIVATE_HANDLE * h,
		    FIS_DATA_TYPE criteriaType, char *uid);
static CONDITION
deleteResultsRecord(PRIVATE_HANDLE * h,
		    FIS_DATA_TYPE criteriaType, char *uid);
static CONDITION
deleteInterpretationRecord(PRIVATE_HANDLE * h,
			   FIS_DATA_TYPE criteriaType, char *uid);

/* FIS_Delete
**
** Purpose:
**
** Parameter Dictionary:
**
** Return Values:
**	FIS_NORMAL
**	FIS_NULLSTRING
**	FIS_DELETEFAILED
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
FIS_Delete(FIS_HANDLE ** handle, FIS_DATA_TYPE type,
	   FIS_DATA_TYPE criteriaType, char *uid)
{
    CONDITION
	cond = FIS_NORMAL;
    PRIVATE_HANDLE
	* h;

    if (uid == NULL)
	return COND_PushCondition(FIS_COND(FIS_NULLSTRING), "UID", "FIS_Delete");
    if (uid[0] == '\0')
	return COND_PushCondition(FIS_COND(FIS_NULLSTRING), "UID", "FIS_Delete");

    h = *(PRIVATE_HANDLE **) handle;

    switch (type) {
    case FIS_K_PATIENT:
	cond = deletePatientRecord(h, criteriaType, uid);
	break;
    case FIS_K_VISIT:
	break;
    case FIS_K_STUDY:
	cond = deleteStudyRecord(h, criteriaType, uid);
	break;
    case FIS_K_STUDYCOMPONENT:
	cond = deleteStudyComponentRecord(h, criteriaType, uid);
	break;
    case FIS_K_SCSERIES:
	cond = deleteSCSeriesRecord(h, criteriaType, uid);
	break;
    case FIS_K_SCIMAGE:
	cond = deleteSCImageRecord(h, criteriaType, uid);
	break;
    case FIS_K_RESULTS:
	cond = deleteResultsRecord(h, criteriaType, uid);
	break;
    case FIS_K_INTERPRETATION:
	cond = deleteInterpretationRecord(h, criteriaType, uid);
	break;
    }
    if (cond != FIS_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_DELETEFAILED),
				  "FIS_Get");
    else
	return FIS_NORMAL;
}

CONDITION
FIS_DeleteWorkQueueRecord(FIS_HANDLE ** handle, char *taskID)
{
    CONDITION cond;
    TBL_CRITERIA criteria[2];
    PRIVATE_HANDLE *h;

    h = *(PRIVATE_HANDLE **) handle;

    TBL_CRITERIA_LOAD_BYTE(criteria[0], "TasID", taskID, TBL_STRING, TBL_EQUAL);
    criteria[1].FieldName = NULL;

    cond = TBL_Delete(&h->workQueueHandle, criteria);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_WORKQUEUERECORDDELETEFAILED),
				  "FIS_DeleteWorkQueueRecord");

    return FIS_NORMAL;
}


/* deletePatientRecord
**
** Purpose:
**
** Parameter Dictionary:
**
** Return Values:
**	FIS_NORMAL
**	FIS_PATIENTDELETEFAILED
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
deletePatientRecord(PRIVATE_HANDLE * h, FIS_DATA_TYPE criteriaType, char *uid)
{
    CONDITION
    cond;
    TBL_CRITERIA
	criteria[2];

    if (criteriaType == FIS_K_PATIENT) {
	TBL_CRITERIA_LOAD_BYTE(criteria[0], "PatUID", uid, TBL_STRING,
			       TBL_EQUAL);
    } else
	return COND_PushCondition(FIS_COND(FIS_ILLEGALDELETECRITERIA),
				  "patient", "(FIS)deletePatientRecord");

    criteria[1].FieldName = NULL;

    cond = TBL_Delete(&h->patientHandle, criteria);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_PATIENTDELETEFAILED),
				  "(FIS)deletePatientRecord");

    return FIS_NORMAL;
}

/* deleteStudyRecord
**
** Purpose:
**
** Parameter Dictionary:
**
** Return Values:
**	FIS_NORMAL
**	FIS_DELETEFAILED
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
deleteStudyRecord(PRIVATE_HANDLE * h, FIS_DATA_TYPE criteriaType, char *uid)
{
    CONDITION
    cond;
    TBL_CRITERIA
	criteria[2];

    if (criteriaType == FIS_K_STUDY) {
	TBL_CRITERIA_LOAD_BYTE(criteria[0], "StuInsUID", uid, TBL_STRING,
			       TBL_EQUAL);
    } else if (criteriaType == FIS_K_PATIENT) {
	TBL_CRITERIA_LOAD_BYTE(criteria[0], "PatUID", uid, TBL_STRING,
			       TBL_EQUAL);
    } else
	return COND_PushCondition(FIS_COND(FIS_ILLEGALDELETECRITERIA),
				  "study", "(FIS)deleteStudyRecord");

    criteria[1].FieldName = NULL;

    cond = TBL_Delete(&h->studyHandle, criteria);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_DELETEFAILED),
				  "(FIS)deleteStudyRecord");

    return FIS_NORMAL;
}

static CONDITION
deleteStudyComponentRecord(PRIVATE_HANDLE * h, FIS_DATA_TYPE criteriaType, char *uid)
{
    CONDITION
    cond;
    TBL_CRITERIA
	criteria[2];

    if (criteriaType == FIS_K_STUDY) {
	TBL_CRITERIA_LOAD_BYTE(criteria[0], "StuInsUID", uid, TBL_STRING,
			       TBL_EQUAL);
    } else if (criteriaType == FIS_K_STUDYCOMPONENT) {
	TBL_CRITERIA_LOAD_BYTE(criteria[0], "StuComUID", uid, TBL_STRING,
			       TBL_EQUAL);
    } else
	return COND_PushCondition(FIS_COND(FIS_ILLEGALDELETECRITERIA),
				"study", "(FIS)deleteStudyComponentRecord");

    criteria[1].FieldName = NULL;

    cond = TBL_Delete(&h->studyComponentHandle, criteria);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_DELETEFAILED),
				  "(FIS)deleteStudyComponentRecord");

    return FIS_NORMAL;
}

static CONDITION
deleteSCSeriesRecord(PRIVATE_HANDLE * h, FIS_DATA_TYPE criteriaType, char *uid)
{
    CONDITION
    cond;
    TBL_CRITERIA
	criteria[2];

    if (criteriaType == FIS_K_STUDYCOMPONENT) {
	TBL_CRITERIA_LOAD_BYTE(criteria[0], "StuComUID", uid, TBL_STRING,
			       TBL_EQUAL);
    } else if (criteriaType == FIS_K_SCSERIES) {
	TBL_CRITERIA_LOAD_BYTE(criteria[0], "SerInsUID", uid, TBL_STRING,
			       TBL_EQUAL);
    } else
	return COND_PushCondition(FIS_COND(FIS_ILLEGALDELETECRITERIA),
				  "study", "(FIS)deleteSCSeriesRecord");

    criteria[1].FieldName = NULL;

    cond = TBL_Delete(&h->studyComponentSeriesHandle, criteria);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_DELETEFAILED),
				  "(FIS)deleteSCSeriesRecord");

    return FIS_NORMAL;
}

static CONDITION
deleteSCImageRecord(PRIVATE_HANDLE * h, FIS_DATA_TYPE criteriaType, char *uid)
{
    CONDITION
    cond;
    TBL_CRITERIA
	criteria[2];

    if (criteriaType == FIS_K_STUDYCOMPONENT) {
	TBL_CRITERIA_LOAD_BYTE(criteria[0], "StuComUID", uid, TBL_STRING,
			       TBL_EQUAL);
    } else if (criteriaType == FIS_K_SCSERIES) {
	TBL_CRITERIA_LOAD_BYTE(criteria[0], "SerInsUID", uid, TBL_STRING,
			       TBL_EQUAL);
    } else if (criteriaType == FIS_K_SCIMAGE) {
	TBL_CRITERIA_LOAD_BYTE(criteria[0], "SOPInsUID", uid, TBL_STRING,
			       TBL_EQUAL);
    } else
	return COND_PushCondition(FIS_COND(FIS_ILLEGALDELETECRITERIA),
				  "study", "(FIS)deleteSCImageRecord");

    criteria[1].FieldName = NULL;

    cond = TBL_Delete(&h->studyComponentImageHandle, criteria);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_DELETEFAILED),
				  "(FIS)deleteSCImageRecord");

    return FIS_NORMAL;
}

/* deleteResultsRecord
**
** Purpose:
**
** Parameter Dictionary:
**
** Return Values:
**	FIS_NORMAL
**	FIS_STUDYDELETEFAILED
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
deleteResultsRecord(PRIVATE_HANDLE * h, FIS_DATA_TYPE criteriaType, char *uid)
{
    CONDITION
    cond;
    TBL_CRITERIA
	criteria[2];

    if (criteriaType == FIS_K_STUDY) {
	TBL_CRITERIA_LOAD_BYTE(criteria[0], "StuInsUID", uid, TBL_STRING,
			       TBL_EQUAL);
    } else if (criteriaType == FIS_K_RESULTS) {
	TBL_CRITERIA_LOAD_BYTE(criteria[0], "ResUID", uid, TBL_STRING,
			       TBL_EQUAL);
    } else
	return COND_PushCondition(FIS_COND(FIS_ILLEGALDELETECRITERIA),
				  "results", "(FIS)deleteResultsRecord");

    criteria[1].FieldName = NULL;

    cond = TBL_Delete(&h->resultsHandle, criteria);
    if (cond != TBL_NORMAL)
	return FIS_DELETEFAILED;

    return FIS_NORMAL;
}

/* deleteInterpretationRecord
**
** Purpose:
**
** Parameter Dictionary:
**
** Return Values:
**	FIS_NORMAL
**	FIS_STUDYDELETEFAILED
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
deleteInterpretationRecord(PRIVATE_HANDLE * h, FIS_DATA_TYPE criteriaType,
			   char *uid)
{
    CONDITION
    cond;
    TBL_CRITERIA
	criteria[2];

    if (criteriaType == FIS_K_RESULTS) {
	TBL_CRITERIA_LOAD_BYTE(criteria[0], "ResUID", uid, TBL_STRING,
			       TBL_EQUAL);
    } else if (criteriaType == FIS_K_INTERPRETATION) {
	TBL_CRITERIA_LOAD_BYTE(criteria[0], "IntUID", uid, TBL_STRING,
			       TBL_EQUAL);
    } else
	return COND_PushCondition(FIS_COND(FIS_ILLEGALDELETECRITERIA),
		       "interpretation", "(FIS)deleteInterpretationRecord");

    criteria[1].FieldName = NULL;

    cond = TBL_Delete(&h->interpretationHandle, criteria);
    if (cond != TBL_NORMAL)
	return FIS_DELETEFAILED;

    return FIS_NORMAL;
}
