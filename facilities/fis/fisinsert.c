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
** Module Name(s):	FIS_Insert
** Author, Date:	Stephen Moore, 12-Jul-94
** Intent:		This file contains functions for inserting FIS records
** Last Update:		$Author: smm $, $Date: 1997-05-30 18:55:18 $
** Source File:		$RCSfile: fisinsert.c,v $
** Revision:		$Revision: 1.11 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.11 $ $RCSfile: fisinsert.c,v $";

#include <stdio.h>
#include <stdlib.h>

#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "dicom_objects.h"
#include "tbl.h"
#include "manage.h"
#include "fis.h"
#include "utility.h"
#include "dicom_uids.h"

#include "fis_private.h"

static CONDITION insertPatientRecord(PRIVATE_HANDLE * h, FIS_PATIENTRECORD * record);
static CONDITION
    insertStudyRecord(PRIVATE_HANDLE * h, FIS_STUDYRECORD * record);
static CONDITION
insertStudyComponentRecord(PRIVATE_HANDLE * h,
			   FIS_STUDYCOMPONENTRECORD * record);
static CONDITION
    insertSCSeriesRecord(PRIVATE_HANDLE * h, FIS_SCSERIESRECORD * r);
static CONDITION
    insertSCImageRecord(PRIVATE_HANDLE * h, FIS_SCIMAGERECORD * r);
static CONDITION
    insertResultsRecord(PRIVATE_HANDLE * h, FIS_RESULTSRECORD * record);
static CONDITION
    insertInterpretationRecord(PRIVATE_HANDLE * h, FIS_INTERPRETATIONRECORD * record);
static CONDITION
    insertStorageCommitReq(PRIVATE_HANDLE * h, FIS_STORAGECOMMITREQRECORD * record);
static CONDITION
    insertCommitSOPRef(PRIVATE_HANDLE * h, FIS_COMMITSOPREFRECORD * s);
static CONDITION
    insertWorkQueueRecord(PRIVATE_HANDLE * h, FIS_WORKQUEUERECORD * r);

/* FIS_Insert
**
** Purpose:
**
** Parameter Dictionary:
**
** Return Values:
**	FIS_NORMAL
**	FIS_INSERTFAILED
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
FIS_Insert(FIS_HANDLE ** handle, FIS_DATA_TYPE type, void *record)
{
    CONDITION
	cond = 0;
    PRIVATE_HANDLE
	* h;

    h = *(PRIVATE_HANDLE **) handle;

    switch (type) {
    case FIS_K_PATIENT:
	cond = insertPatientRecord(h, (FIS_PATIENTRECORD *) record);
	break;
    case FIS_K_VISIT:
	break;
    case FIS_K_STUDY:
	cond = insertStudyRecord(h, (FIS_STUDYRECORD *) record);
	break;
    case FIS_K_STUDYCOMPONENT:
	cond = insertStudyComponentRecord(h, (FIS_STUDYCOMPONENTRECORD *) record);
	break;
    case FIS_K_SCSERIES:
	cond = insertSCSeriesRecord(h, (FIS_SCSERIESRECORD *) record);
	break;
    case FIS_K_SCIMAGE:
	cond = insertSCImageRecord(h, (FIS_SCIMAGERECORD *) record);
	break;
    case FIS_K_RESULTS:
	cond = insertResultsRecord(h, (FIS_RESULTSRECORD *) record);
	break;
    case FIS_K_INTERPRETATION:
	cond = insertInterpretationRecord(h, (FIS_INTERPRETATIONRECORD *) record);
	break;
    case FIS_K_STORAGECOMMITREQ:
	cond = insertStorageCommitReq(h,
				      (FIS_STORAGECOMMITREQRECORD *) record);
	break;
    case FIS_K_COMMITSOPREF:
	cond = insertCommitSOPRef(h, (FIS_COMMITSOPREFRECORD *) record);
	break;
    case FIS_K_WORKQUEUE:
	cond = insertWorkQueueRecord(h, (FIS_WORKQUEUERECORD *) record);
	break;
    }
    if (cond != FIS_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_INSERTFAILED),
				  "", "FIS_Insert");
    return cond;
}

/* insertPatientRecord
**
** Purpose:
**
** Parameter Dictionary:
**
** Return Values:
**	FIS_NORMAL
**	FIS_ILLEGALRECORDTYPE
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
insertPatientRecord(PRIVATE_HANDLE * h, FIS_PATIENTRECORD * p)
{
    CONDITION
    cond;
    TBL_FIELD
	fields[10];
    int
        i;
    S32
	date;

    if (p->Type != FIS_K_PATIENT)
	return COND_PushCondition(FIS_COND(FIS_ILLEGALRECORDTYPE),
			      "Patient Record", "(FIS)insertPatientRecord");

    i = 0;
    TBL_FIELD_LOAD_BYTE(fields[i], "PatID", p->PatID, TBL_STRING);
    i++;
    TBL_FIELD_LOAD_BYTE(fields[i], "PatUID", p->PatUID, TBL_STRING);
    i++;
    TBL_FIELD_LOAD_BYTE(fields[i], "PatNam", p->PatNam, TBL_STRING);
    i++;
    TBL_FIELD_LOAD_BYTE(fields[i], "PatSex", p->PatSex, TBL_STRING);

    i++;
    date = UTL_ConvertDatetoLong(p->PatBirDat);
    TBL_FIELD_LOAD_NUM(fields[i], "PatBirDat", date, TBL_SIGNED4);

    i++;
    fields[i].FieldName = NULL;

    cond = TBL_Insert(&h->patientHandle, fields);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_INSERTFAILED),
				  "patient", "(FIS)insertPatientRecord");

    return FIS_NORMAL;
}

/* insertStudyRecord
**
** Purpose:
**
** Parameter Dictionary:
**
** Return Values:
**	FIS_NORMAL
**	FIS_ILLEGALRECORDTYPE
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
insertStudyRecord(PRIVATE_HANDLE * h, FIS_STUDYRECORD * s)
{
    CONDITION
    cond;
    TBL_FIELD
	fields[20];
    int
        i;
    S32
	date,
	readDate;
    float
        time,
        readTime;

    if (s->Type != FIS_K_STUDY)
	return COND_PushCondition(FIS_COND(FIS_ILLEGALRECORDTYPE),
				  "Study Record", "(FIS)insertStudyRecord");

    i = 0;
    TBL_FIELD_LOAD_BYTE(fields[i], "PatUID", s->PatUID, TBL_STRING);
    i++;
    TBL_FIELD_LOAD_BYTE(fields[i], "StuInsUID", s->StuInsUID, TBL_STRING);
    i++;
    TBL_FIELD_LOAD_BYTE(fields[i], "VisUID", s->VisUID, TBL_STRING);
    i++;
    TBL_FIELD_LOAD_BYTE(fields[i], "AccNum", s->AccNum, TBL_STRING);
    i++;
    TBL_FIELD_LOAD_BYTE(fields[i], "StuID", s->StuID, TBL_STRING);

    i++;
    date = UTL_ConvertDatetoLong(s->SchStuStaDat);
    TBL_FIELD_LOAD_NUM(fields[i], "SchStuStaDat", date, TBL_SIGNED4);
    i++;
    time = UTL_ConvertTimetoFloat(s->SchStuStaTim);
    TBL_FIELD_LOAD_NUM(fields[i], "SchStuStaTim", time, TBL_FLOAT4);

    i++;
    TBL_FIELD_LOAD_BYTE(fields[i], "SchStuLoc", s->SchStuLoc, TBL_STRING);
    i++;
    TBL_FIELD_LOAD_BYTE(fields[i], "ProDes", s->ProDes, TBL_STRING);
    i++;
    TBL_FIELD_LOAD_BYTE(fields[i], "StuStaID", s->StuStaID, TBL_STRING);
    i++;

    if (s->Flag & FIS_K_STU_STUREADAT) {
	readDate = UTL_ConvertDatetoLong(s->StuReaDat);
	TBL_FIELD_LOAD_NUM(fields[i], "StuReaDat", readDate, TBL_SIGNED4);
	i++;
    }
    if (s->Flag & FIS_K_STU_STUREATIM) {
	readTime = UTL_ConvertTimetoFloat(s->StuReaTim);
	TBL_FIELD_LOAD_NUM(fields[i], "StuReaTim", readTime, TBL_FLOAT4);
	i++;
    }
    fields[i].FieldName = NULL;

    cond = TBL_Insert(&h->studyHandle, fields);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_INSERTFAILED),
				  "study", "(FIS)insertStudyRecord");

    return FIS_NORMAL;
}

static CONDITION
insertStudyComponentRecord(PRIVATE_HANDLE * h, FIS_STUDYCOMPONENTRECORD * s)
{
    CONDITION cond;
    TBL_FIELD fields[12];
    int i;

    if (s->Type != FIS_K_STUDYCOMPONENT)
	return COND_PushCondition(FIS_COND(FIS_ILLEGALRECORDTYPE),
				  "Study Component Record",
				  "(FIS)insertStudyComponentRecord");

    i = 0;
    if ((s->Flag & FIS_K_STUDYCOMP_STUCOMUID) && (strlen(s->StuComUID) != 0)) {
	TBL_FIELD_LOAD_BYTE(fields[i], "StuComUID", s->StuComUID, TBL_STRING);
    } else
	return COND_PushCondition(FIS_COND(FIS_MISSINGMANDATORYELEMENT),
				  "Study Component UID",
				  "(FIS)insertStudyComponentRecord");
    i++;

    if ((s->Flag & FIS_K_STUDYCOMP_STUINSUID) && (strlen(s->StuInsUID) != 0)) {
	TBL_FIELD_LOAD_BYTE(fields[i], "StuInsUID", s->StuInsUID, TBL_STRING);
    } else
	return COND_PushCondition(FIS_COND(FIS_MISSINGMANDATORYELEMENT),
		   "Study Instance UID", "(FIS)insertStudyComponentRecord");
    i++;

    if (s->Flag & FIS_K_STUDYCOMP_MOD) {
	TBL_FIELD_LOAD_BYTE(fields[i], "Mod", s->Mod, TBL_STRING);
	i++;
    }
    if (s->Flag & FIS_K_STUDYCOMP_PROCODVAL) {
	TBL_FIELD_LOAD_BYTE(fields[i], "ProCodVal", s->ProCodVal, TBL_STRING);
	i++;
    }
    if (s->Flag & FIS_K_STUDYCOMP_PROCODSCHDES) {
	TBL_FIELD_LOAD_BYTE(fields[i], "ProCodSchDes", s->ProCodSchDes, TBL_STRING);
	i++;
    }
    if (s->Flag & FIS_K_STUDYCOMP_PROCODMEA) {
	TBL_FIELD_LOAD_BYTE(fields[i], "ProCodMea", s->ProCodMea, TBL_STRING);
	i++;
    }
    if (s->Flag & FIS_K_STUDYCOMP_STUDES) {
	TBL_FIELD_LOAD_BYTE(fields[i], "StuDes", s->StuDes, TBL_STRING);
	i++;
    }
    if (s->Flag & FIS_K_STUDYCOMP_STUID) {
	TBL_FIELD_LOAD_BYTE(fields[i], "StuID", s->StuID, TBL_STRING);
	i++;
    }
    if (s->Flag & FIS_K_STUDYCOMP_STUCOMSTAID) {
	TBL_FIELD_LOAD_BYTE(fields[i], "StuComStaID", s->StuComStaID, TBL_STRING);
	i++;
    }
    fields[i].FieldName = NULL;

    cond = TBL_Insert(&h->studyComponentHandle, fields);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_INSERTFAILED),
				  "study component",
				  "(FIS)insertStudyComponentRecord");

    return FIS_NORMAL;
}

static CONDITION
insertSCSeriesRecord(PRIVATE_HANDLE * h, FIS_SCSERIESRECORD * s)
{
    CONDITION cond;
    TBL_FIELD fields[12];
    int i;

    if (s->Type != FIS_K_SCSERIES)
	return COND_PushCondition(FIS_COND(FIS_ILLEGALRECORDTYPE),
				  "Study Component Series Record",
				  "(FIS)insertSCSeriesRecord");

    i = 0;
    if ((s->Flag & FIS_K_SCSERIES_SERINSUID) && (strlen(s->SerInsUID) != 0)) {
	TBL_FIELD_LOAD_BYTE(fields[i], "SerInsUID", s->SerInsUID, TBL_STRING);
    } else
	return COND_PushCondition(FIS_COND(FIS_MISSINGMANDATORYELEMENT),
				  "Series Instance UID",
				  "(FIS)insertSCSeriesRecord");
    i++;

    if ((s->Flag & FIS_K_SCSERIES_STUCOMUID) && (strlen(s->StuComUID) != 0)) {
	TBL_FIELD_LOAD_BYTE(fields[i], "StuComUID", s->StuComUID, TBL_STRING);
    } else
	return COND_PushCondition(FIS_COND(FIS_MISSINGMANDATORYELEMENT),
			"Study Component UID", "(FIS)insertSCSeriesRecord");
    i++;

    if (s->Flag & FIS_K_SCSERIES_SERDAT) {
	TBL_FIELD_LOAD_BYTE(fields[i], "SerDat", s->SerDat, TBL_STRING);
	i++;
    }
    if (s->Flag & FIS_K_SCSERIES_SERTIM) {
	TBL_FIELD_LOAD_BYTE(fields[i], "SerTim", s->SerTim, TBL_STRING);
	i++;
    }
    if (s->Flag & FIS_K_SCSERIES_RETAETIT) {
	TBL_FIELD_LOAD_BYTE(fields[i], "RetAETit", s->RetAETit, TBL_STRING);
	i++;
    }
    if (s->Flag & FIS_K_SCSERIES_STOMEDFILSETID) {
	TBL_FIELD_LOAD_BYTE(fields[i], "StoMedFilSetID", s->StoMedFilSetID, TBL_STRING);
	i++;
    }
    if (s->Flag & FIS_K_SCSERIES_STOMEDFILSETUID) {
	TBL_FIELD_LOAD_BYTE(fields[i], "StoMedFilSetUID", s->StoMedFilSetUID, TBL_STRING);
	i++;
    }
    fields[i].FieldName = NULL;

    cond = TBL_Insert(&h->studyComponentSeriesHandle, fields);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_INSERTFAILED),
	 "study component series", "(FIS)insertStudyComponentSeriesRecord");

    return FIS_NORMAL;
}

static CONDITION
insertSCImageRecord(PRIVATE_HANDLE * h, FIS_SCIMAGERECORD * s)
{
    CONDITION cond;
    TBL_FIELD fields[12];
    int i;

    if (s->Type != FIS_K_SCIMAGE)
	return COND_PushCondition(FIS_COND(FIS_ILLEGALRECORDTYPE),
				  "Study Component Image Record",
				  "(FIS)insertSCImageRecord");

    i = 0;
    if ((s->Flag & FIS_K_SCIMAGE_SOPINSUID) && (strlen(s->SOPInsUID) != 0)) {
	TBL_FIELD_LOAD_BYTE(fields[i], "SOPInsUID", s->SOPInsUID, TBL_STRING);
    } else
	return COND_PushCondition(FIS_COND(FIS_MISSINGMANDATORYELEMENT),
				  "SOP Instance UID",
				  "(FIS)insertSCImageRecord");
    i++;


    if ((s->Flag & FIS_K_SCIMAGE_SERINSUID) && (strlen(s->SerInsUID) != 0)) {
	TBL_FIELD_LOAD_BYTE(fields[i], "SerInsUID", s->SerInsUID, TBL_STRING);
    } else
	return COND_PushCondition(FIS_COND(FIS_MISSINGMANDATORYELEMENT),
				  "Series Instance UID",
				  "(FIS)insertSCImageRecord");
    i++;

    if ((s->Flag & FIS_K_SCIMAGE_STUCOMUID) && (strlen(s->StuComUID) != 0)) {
	TBL_FIELD_LOAD_BYTE(fields[i], "StuComUID", s->StuComUID, TBL_STRING);
    } else
	return COND_PushCondition(FIS_COND(FIS_MISSINGMANDATORYELEMENT),
			 "Study Component UID", "(FIS)insertSCImageRecord");
    i++;
    if ((s->Flag & FIS_K_SCIMAGE_SOPCLAUID) && (strlen(s->SOPClaUID) != 0)) {
	TBL_FIELD_LOAD_BYTE(fields[i], "SOPClaUID", s->SOPClaUID, TBL_STRING);
    } else
	return COND_PushCondition(FIS_COND(FIS_MISSINGMANDATORYELEMENT),
				  "SOP Class UID",
				  "(FIS)insertSCImageRecord");
    i++;

    if (s->Flag & FIS_K_SCIMAGE_RETAETIT) {
	TBL_FIELD_LOAD_BYTE(fields[i], "RetAETit", s->RetAETit, TBL_STRING);
	i++;
    }
    if (s->Flag & FIS_K_SCIMAGE_STOMEDFILSETID) {
	TBL_FIELD_LOAD_BYTE(fields[i], "StoMedFilSetID", s->StoMedFilSetID, TBL_STRING);
	i++;
    }
    if (s->Flag & FIS_K_SCIMAGE_STOMEDFILSETUID) {
	TBL_FIELD_LOAD_BYTE(fields[i], "StoMedFilSetUID", s->StoMedFilSetUID, TBL_STRING);
	i++;
    }
    fields[i].FieldName = NULL;

    cond = TBL_Insert(&h->studyComponentImageHandle, fields);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_INSERTFAILED),
	   "Study Component Image", "(FIS)insertStudyComponentImageRecord");

    return FIS_NORMAL;
}


/* insertResultsRecord
**
** Purpose:
**
** Parameter Dictionary:
**
** Return Values:
**	FIS_NORMAL
**	FIS_ILLEGALRECORDTYPE
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
insertResultsRecord(PRIVATE_HANDLE * h, FIS_RESULTSRECORD * s)
{
    CONDITION
    cond;
    TBL_FIELD
	fields[15];
    int
        i;

    if (s->Type != FIS_K_RESULTS)
	return COND_PushCondition(FIS_COND(FIS_ILLEGALRECORDTYPE),
			      "Results Record", "(FIS)insertResultsRecord");

    i = 0;
    if ((s->Flag & FIS_K_RESULTS_RESUID) && (strlen(s->ResUID) != 0)) {
	TBL_FIELD_LOAD_BYTE(fields[i], "ResUID", s->ResUID, TBL_STRING);
    } else
	return COND_PushCondition(FIS_COND(FIS_MISSINGMANDATORYELEMENT),
				  "Results UID", "(FIS)insertResultsRecord");
    i++;

    if ((s->Flag & FIS_K_RESULTS_STUINSUID) && (strlen(s->StuInsUID) != 0)) {
	TBL_FIELD_LOAD_BYTE(fields[i], "StuInsUID", s->StuInsUID, TBL_STRING);
    } else
	return COND_PushCondition(FIS_COND(FIS_MISSINGMANDATORYELEMENT),
			  "Study Instance UID", "(FIS)insertResultsRecord");
    i++;

    if (s->Flag & FIS_K_RESULTS_RESID) {
	TBL_FIELD_LOAD_BYTE(fields[i], "ResID", s->ResID, TBL_STRING);
	i++;
    }
    if (s->Flag & FIS_K_RESULTS_IMP) {
	TBL_FIELD_LOAD_BYTE(fields[i], "Imp", s->Imp, TBL_TEXT);
	i++;
    }
    if (s->Flag & FIS_K_RESULTS_INSCREDAT) {
	TBL_FIELD_LOAD_BYTE(fields[i], "InsCreDat", s->InsCreDat, TBL_STRING);
	i++;
    }
    if (s->Flag & FIS_K_RESULTS_INSCRETIM) {
	TBL_FIELD_LOAD_BYTE(fields[i], "InsCreTim", s->InsCreTim, TBL_STRING);
	i++;
    }
    if (s->Flag & FIS_K_RESULTS_INSCREUID) {
	TBL_FIELD_LOAD_BYTE(fields[i], "InsCreUID", s->InsCreUID, TBL_STRING);
	i++;
    }
    fields[i].FieldName = NULL;

    cond = TBL_Insert(&h->resultsHandle, fields);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_INSERTFAILED),
				  "results", "(FIS)insertResultsRecord");

    return FIS_NORMAL;
}

/* insertInterpretationRecord
**
** Purpose:
**
** Parameter Dictionary:
**
** Return Values:
**	FIS_NORMAL
**	FIS_ILLEGALRECORDTYPE
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
insertInterpretationRecord(PRIVATE_HANDLE * h, FIS_INTERPRETATIONRECORD * s)
{
    CONDITION
    cond;
    TBL_FIELD
	fields[15];
    int
        i;

    if (s->Type != FIS_K_INTERPRETATION)
	return COND_PushCondition(FIS_COND(FIS_ILLEGALRECORDTYPE),
				  "Interpretation Record",
				  "(FIS)insertInterpretationRecord");

    i = 0;
    if ((s->Flag & FIS_K_INTERP_INTUID) && (strlen(s->IntUID) != 0)) {
	TBL_FIELD_LOAD_BYTE(fields[i], "IntUID", s->IntUID, TBL_STRING);
    } else
	return COND_PushCondition(FIS_COND(FIS_MISSINGMANDATORYELEMENT),
		   "Interpretation UID", "(FIS)insertInterpretationRecord");
    i++;

    if ((s->Flag & FIS_K_INTERP_RESUID) && (strlen(s->ResUID) != 0)) {
	TBL_FIELD_LOAD_BYTE(fields[i], "ResUID", s->ResUID, TBL_STRING);
    } else
	return COND_PushCondition(FIS_COND(FIS_MISSINGMANDATORYELEMENT),
			  "Results UID", "(FIS)insertInterpretationRecord");
    i++;

    if (s->Flag & FIS_K_INTERP_INTTEX) {
	TBL_FIELD_LOAD_BYTE(fields[i], "IntTex", s->IntTex, TBL_TEXT);
	i++;
    }
    if (s->Flag & FIS_K_INTERP_INTID) {
	TBL_FIELD_LOAD_BYTE(fields[i], "IntID", s->IntID, TBL_STRING);
	i++;
    }
    if (s->Flag & FIS_K_INTERP_INTTYPID) {
	TBL_FIELD_LOAD_BYTE(fields[i], "IntTypID", s->IntTypID, TBL_STRING);
	i++;
    }
    if (s->Flag & FIS_K_INTERP_INTSTAID) {
	TBL_FIELD_LOAD_BYTE(fields[i], "IntStaID", s->IntStaID, TBL_STRING);
	i++;
    }
    if (s->Flag & FIS_K_INTERP_INSCREDAT) {
	TBL_FIELD_LOAD_BYTE(fields[i], "InsCreDat", s->InsCreDat, TBL_STRING);
	i++;
    }
    if (s->Flag & FIS_K_INTERP_INSCRETIM) {
	TBL_FIELD_LOAD_BYTE(fields[i], "InsCreTim", s->InsCreTim, TBL_STRING);
	i++;
    }
    if (s->Flag & FIS_K_INTERP_INSCREUID) {
	TBL_FIELD_LOAD_BYTE(fields[i], "InsCreUID", s->InsCreUID, TBL_STRING);
	i++;
    }
    if (s->Flag & FIS_K_INTERP_PHYAPPINT) {
	TBL_FIELD_LOAD_BYTE(fields[i], "PhyAppInt", s->PhyAppInt, TBL_STRING);
	i++;
    }
    if (s->Flag & FIS_K_INTERP_INTAPPDAT) {
	TBL_FIELD_LOAD_BYTE(fields[i], "IntAppDat", s->IntAppDat, TBL_STRING);
	i++;
    }
    if (s->Flag & FIS_K_INTERP_INTAPPTIM) {
	TBL_FIELD_LOAD_BYTE(fields[i], "IntAppTim", s->IntAppTim, TBL_STRING);
	i++;
    }
    fields[i].FieldName = NULL;

    cond = TBL_Insert(&h->interpretationHandle, fields);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_INSERTFAILED),
		       "interpretation", "(FIS)insertInterpretationRecord");

    return FIS_NORMAL;
}

static CONDITION
insertStorageCommitReq(PRIVATE_HANDLE * h, FIS_STORAGECOMMITREQRECORD * s)
{
    CONDITION cond;
    TBL_FIELD fields[10];
    int i;
    long scratchDate1,
        scratchDate2;
    float scratchTime1,
        scratchTime2;

    if (s->Type != FIS_K_STORAGECOMMITREQ)
	return COND_PushCondition(FIS_COND(FIS_ILLEGALRECORDTYPE),
				  "Storage Commitment Request",
				  "(FIS)insertStorageCommitReq");

    i = 0;
    if ((s->Flag & FIS_K_SCOMMIT_TRAUID) && (strlen(s->TraUID) != 0)) {
	TBL_FIELD_LOAD_BYTE(fields[i], "TraUID", s->TraUID, TBL_STRING);
    } else
	return COND_PushCondition(FIS_COND(FIS_MISSINGMANDATORYELEMENT),
			  "Transaction UID", "(FIS)insertStorageCommitReq");
    i++;

    if ((s->Flag & FIS_K_SCOMMIT_REQAE) && (strlen(s->ReqAE) != 0)) {
	TBL_FIELD_LOAD_BYTE(fields[i], "ReqAE", s->ReqAE, TBL_STRING);
    } else
	return COND_PushCondition(FIS_COND(FIS_MISSINGMANDATORYELEMENT),
			    "Requesting AE", "(FIS)insertStorageCommitReq");
    i++;

    if (s->Flag & FIS_K_SCOMMIT_RESAE) {
	TBL_FIELD_LOAD_BYTE(fields[i], "ResAE", s->ResAE, TBL_STRING);
	i++;
    }
    if (s->Flag & FIS_K_SCOMMIT_REQDAT) {
	scratchDate1 = UTL_ConvertDatetoLong(s->ReqDat);
	TBL_FIELD_LOAD_NUM(fields[i], "ReqDat", scratchDate1, TBL_SIGNED4);
	i++;
    }
    if (s->Flag & FIS_K_SCOMMIT_REQTIM) {
	scratchTime1 = UTL_ConvertTimetoFloat(s->ReqTim);
	TBL_FIELD_LOAD_NUM(fields[i], "ReqTim", scratchTime1, TBL_FLOAT4);
	i++;
    }
    if (s->Flag & FIS_K_SCOMMIT_RESDAT) {
	scratchDate2 = UTL_ConvertDatetoLong(s->ResDat);
	TBL_FIELD_LOAD_NUM(fields[i], "ResDat", scratchDate2, TBL_SIGNED4);
	i++;
    }
    if (s->Flag & FIS_K_SCOMMIT_RESTIM) {
	scratchTime2 = UTL_ConvertTimetoFloat(s->ResTim);
	TBL_FIELD_LOAD_NUM(fields[i], "ResTim", scratchTime2, TBL_FLOAT4);
	i++;
    }
    fields[i].FieldName = NULL;

    cond = TBL_Insert(&h->storageCommitReqHandle, fields);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_INSERTFAILED),
				  "Storage Commitment Request",
				  "(FIS)insertStorageCommitReq");

    return FIS_NORMAL;
}

static CONDITION
insertCommitSOPRef(PRIVATE_HANDLE * h, FIS_COMMITSOPREFRECORD * s)
{
    CONDITION cond;
    TBL_FIELD fields[10];
    int i;

    if (s->Type != FIS_K_COMMITSOPREF)
	return COND_PushCondition(FIS_COND(FIS_ILLEGALRECORDTYPE),
				  "Commit SOP Reference Record",
				  "(FIS)insertCommitSOPRef");

    i = 0;
    if ((s->Flag & FIS_K_COMMITSOP_TRAUID) && (strlen(s->TraUID) != 0)) {
	TBL_FIELD_LOAD_BYTE(fields[i], "TraUID", s->TraUID, TBL_STRING);
    } else
	return COND_PushCondition(FIS_COND(FIS_MISSINGMANDATORYELEMENT),
			      "Transaction UID", "(FIS)insertCommitSOPRef");
    i++;

    if ((s->Flag & FIS_K_COMMITSOP_SOPCLAUID) && (strlen(s->SOPClaUID) != 0)) {
	TBL_FIELD_LOAD_BYTE(fields[i], "SOPClaUID", s->SOPClaUID, TBL_STRING);
    } else
	return COND_PushCondition(FIS_COND(FIS_MISSINGMANDATORYELEMENT),
				"SOP Class UID", "(FIS)insertCommitSOPRef");
    i++;

    if ((s->Flag & FIS_K_COMMITSOP_SOPINSUID) && (strlen(s->SOPInsUID) != 0)) {
	TBL_FIELD_LOAD_BYTE(fields[i], "SOPInsUID", s->SOPInsUID, TBL_STRING);
    } else
	return COND_PushCondition(FIS_COND(FIS_MISSINGMANDATORYELEMENT),
			     "SOP Instance UID", "(FIS)insertCommitSOPRef");
    i++;

    fields[i].FieldName = NULL;

    cond = TBL_Insert(&h->commitSOPRefHandle, fields);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_INSERTFAILED),
		     "Commitment SOP Reference", "(FIS)insertCommitSOPRef");

    return FIS_NORMAL;
}

static CONDITION
insertWorkQueueRecord(PRIVATE_HANDLE * h, FIS_WORKQUEUERECORD * r)
{
    CONDITION cond;
    TBL_FIELD fields[3];
    int i;

    if (r->Type != FIS_K_WORKQUEUE)
	return COND_PushCondition(FIS_COND(FIS_ILLEGALRECORDTYPE),
				  "Work Queue Record",
				  "(FIS)insertWorkQueueRecord");

    i = 0;
    if ((r->Flag & FIS_K_WORKQUEUE_TASNAM) && (strlen(r->TasNam) != 0)) {
	TBL_FIELD_LOAD_BYTE(fields[i], "TasNam", r->TasNam, TBL_STRING);
    } else
	return COND_PushCondition(FIS_COND(FIS_MISSINGMANDATORYELEMENT),
				  "Task Name", "(FIS)insertWorkQueueRecord");
    i++;

    if ((r->Flag & FIS_K_WORKQUEUE_TASID) && (strlen(r->TasID) != 0)) {
	TBL_FIELD_LOAD_BYTE(fields[i], "TasID", r->TasID, TBL_STRING);
    } else
	return COND_PushCondition(FIS_COND(FIS_MISSINGMANDATORYELEMENT),
				  "Task ID", "(FIS)insertWorkQueueRecord");
    i++;

    fields[i].FieldName = NULL;

    cond = TBL_Insert(&h->workQueueHandle, fields);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_INSERTFAILED),
				"Work Queue", "(FIS)insertWorkQueueRecord");

    return FIS_NORMAL;
}
