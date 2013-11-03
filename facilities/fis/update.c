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
** Module Name(s):	FIS_Update
** Author, Date:	Stephen Moore, 12-Jul-94
** Intent:		This file contains functions for inserting FIS records
** Last Update:		$Author: smm $, $Date: 2000-11-03 17:04:23 $
** Source File:		$RCSfile: update.c,v $
** Revision:		$Revision: 1.8 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.8 $ $RCSfile: update.c,v $";

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
#include "utility.h"
#include "dicom_uids.h"

#include "fis_private.h"

static CONDITION updatePatientRecord(PRIVATE_HANDLE * h, FIS_PATIENTRECORD * r);
static CONDITION updateStudyRecord(PRIVATE_HANDLE * h, FIS_STUDYRECORD * r);
static CONDITION
updateStudyComponentRecord(PRIVATE_HANDLE * h,
			   FIS_STUDYCOMPONENTRECORD * r);
static CONDITION updateResultsRecord(PRIVATE_HANDLE * h, FIS_RESULTSRECORD * r);
static CONDITION
updateInterpretationRecord(PRIVATE_HANDLE * h,
			   FIS_INTERPRETATIONRECORD * r);
static CONDITION
updateStorageCommitReqRecord(PRIVATE_HANDLE * h,
			     FIS_STORAGECOMMITREQRECORD * r);

/* FIS_Update
**
** Purpose:
**
** Parameter Dictionary:
**
** Return Values:
**	FIS_NORMAL
**	FIS_UPDATEFAILED
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
FIS_Update(FIS_HANDLE ** handle, FIS_DATA_TYPE type, void *r)
{
    CONDITION
	cond = FIS_NORMAL;
    PRIVATE_HANDLE
	* h;

    h = *(PRIVATE_HANDLE **) handle;

    switch (type) {
    case FIS_K_PATIENT:
	cond = updatePatientRecord(h, (FIS_PATIENTRECORD *) r);
	break;
    case FIS_K_VISIT:
	break;
    case FIS_K_STUDY:
	cond = updateStudyRecord(h, (FIS_STUDYRECORD *) r);
	break;
    case FIS_K_STUDYCOMPONENT:
	cond = updateStudyComponentRecord(h, (FIS_STUDYCOMPONENTRECORD *) r);
	break;
    case FIS_K_RESULTS:
	cond = updateResultsRecord(h, (FIS_RESULTSRECORD *) r);
	break;
    case FIS_K_INTERPRETATION:
	cond = updateInterpretationRecord(h, (FIS_INTERPRETATIONRECORD *) r);
	break;
    case FIS_K_STORAGECOMMITREQ:
	cond = updateStorageCommitReqRecord(h, (FIS_STORAGECOMMITREQRECORD *) r);
	break;
    }
    if (cond != FIS_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_UPDATEFAILED), "FIS_Update");
    return cond;
}

/* updatePatientRecord
**
** Purpose:
**
** Parameter Dictionary:
**
** Return Values:
**	FIS_NORMAL
**	FIS_ILLEGALRECORDTYPE
**	FIS_PATIENTUPDATEFAILED
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
updatePatientRecord(PRIVATE_HANDLE * h, FIS_PATIENTRECORD * p)
{
    CONDITION
    cond;
    TBL_UPDATE
	update[10];
    TBL_CRITERIA
	criteria[2];
    int
        i;
    S32
	date;

    if (p->Type != FIS_K_PATIENT)
	return COND_PushCondition(FIS_COND(FIS_ILLEGALRECORDTYPE),
			      "Patient Record", "(FIS)updatePatientRecord");

    if (p->PatUID[0] == '\0')
	return COND_PushCondition(FIS_COND(FIS_NULLSTRING), "Patient UID",
				  "(FIS)updatePatientRecord");

    TBL_CRITERIA_LOAD_BYTE(criteria[0], "PatUID", p->PatUID, TBL_STRING, TBL_EQUAL);
    criteria[1].FieldName = NULL;

    i = 0;
    TBL_UPDATE_LOAD_BYTE(update[i], "PatID", p->PatID, TBL_STRING, TBL_SET);
    i++;
    TBL_UPDATE_LOAD_BYTE(update[i], "PatUID", p->PatUID, TBL_STRING, TBL_SET);
    i++;
    TBL_UPDATE_LOAD_BYTE(update[i], "PatNam", p->PatNam, TBL_STRING, TBL_SET);
    i++;
    TBL_UPDATE_LOAD_BYTE(update[i], "PatSex", p->PatSex, TBL_STRING, TBL_SET);

    i++;
    date = UTL_ConvertDatetoLong(p->PatBirDat);
    TBL_UPDATE_LOAD_NUM(update[i], "PatBirDat", date, TBL_SIGNED4, TBL_SET);

    i++;
    update[i].FieldName = NULL;

    cond = TBL_Update(&h->patientHandle, criteria, update);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_PATIENTUPDATEFAILED),
				  "(FIS)updatePatientRecord");

    return FIS_NORMAL;
}

/* updateStudyRecord
**
** Purpose:
**
** Parameter Dictionary:
**
** Return Values:
**	FIS_NORMAL
**	FIS_ILLEGALRECORDTYPE
**	FIS_STUDYUPDATEFAILED
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
updateStudyRecord(PRIVATE_HANDLE * h, FIS_STUDYRECORD * s)
{
    CONDITION
    cond;
    TBL_UPDATE
	update[15];
    TBL_CRITERIA
	criteria[2];
    int
        i;
    S32
	date;
    double
        time;

    if (s->Type != FIS_K_STUDY)
	return COND_PushCondition(FIS_COND(FIS_ILLEGALRECORDTYPE),
				  "Study Record", "(FIS)updateStudyRecord");

    if (s->StuInsUID[0] == '\0')
	return COND_PushCondition(FIS_COND(FIS_NULLSTRING), "Study UID",
				  "(FIS)updateStudyRecord");

    TBL_CRITERIA_LOAD_BYTE(criteria[0], "StuInsUID", s->StuInsUID, TBL_STRING, TBL_EQUAL);
    criteria[1].FieldName = NULL;

    i = 0;
    if (s->Flag & FIS_K_STU_SCHSTUSTADAT) {
	date = UTL_ConvertDatetoLong(s->SchStuStaDat);
	TBL_UPDATE_LOAD_NUM(update[i], "SchStuStaDat", date, TBL_SIGNED4, TBL_SET);
	i++;
    }
    if (s->Flag & FIS_K_STU_SCHSTUSTATIM) {
	time = UTL_ConvertTimetoFloat(s->SchStuStaTim);
	TBL_UPDATE_LOAD_NUM(update[i], "SchStuStaTim", time, TBL_FLOAT4, TBL_SET);
	i++;
    }
    if (s->Flag & FIS_K_STU_SCHSTULOC) {
	TBL_UPDATE_LOAD_BYTE(update[i], "SchStuLoc", s->SchStuLoc, TBL_STRING, TBL_SET);
	i++;
    }
    if (s->Flag & FIS_K_STU_PRODES) {
	TBL_UPDATE_LOAD_BYTE(update[i], "ProDes", s->ProDes, TBL_STRING, TBL_SET);
	i++;
    }
    if (s->Flag & FIS_K_STU_STUSTAID) {
	TBL_UPDATE_LOAD_BYTE(update[i], "StuStaID", s->StuStaID, TBL_STRING, TBL_SET);
	i++;
    }
    if (i == 0)
	return FIS_NORMAL;

    update[i].FieldName = NULL;
    cond = TBL_Update(&h->studyHandle, criteria, update);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_STUDYUPDATEFAILED),
				  "(FIS)updateStudyRecord");

    return FIS_NORMAL;
}

static CONDITION
updateStudyComponentRecord(PRIVATE_HANDLE * h, FIS_STUDYCOMPONENTRECORD * s)
{
    CONDITION cond;
    TBL_UPDATE update[15];
    TBL_CRITERIA criteria[2];
    int i;

    if (s->Type != FIS_K_STUDYCOMPONENT)
	return COND_PushCondition(FIS_COND(FIS_ILLEGALRECORDTYPE),
				  "Study Componetn Record",
				  "(FIS)updateStudyComponentRecord");

    if (s->StuComUID[0] == '\0')
	return COND_PushCondition(FIS_COND(FIS_NULLSTRING), "Study Component UID",
				  "(FIS)updateStudyComponentRecord");

    TBL_CRITERIA_LOAD_BYTE(criteria[0], "StuComUID", s->StuComUID, TBL_STRING, TBL_EQUAL);
    criteria[1].FieldName = NULL;

    i = 0;
    if (s->Flag & FIS_K_STUDYCOMP_MOD) {
	TBL_UPDATE_LOAD_BYTE(update[i], "Mod", s->Mod, TBL_STRING, TBL_SET);
	i++;
    }
    if (s->Flag & FIS_K_STUDYCOMP_PROCODVAL) {
	TBL_UPDATE_LOAD_BYTE(update[i], "ProCodVal", s->ProCodVal, TBL_STRING, TBL_SET);
	i++;
    }
    if (s->Flag & FIS_K_STUDYCOMP_PROCODSCHDES) {
	TBL_UPDATE_LOAD_BYTE(update[i], "ProCodSchDes", s->ProCodSchDes,
			     TBL_STRING, TBL_SET);
	i++;
    }
    if (s->Flag & FIS_K_STUDYCOMP_PROCODMEA) {
	TBL_UPDATE_LOAD_BYTE(update[i], "ProCodMea", s->ProCodMea, TBL_STRING, TBL_SET);
	i++;
    }
    if (s->Flag & FIS_K_STUDYCOMP_STUDES) {
	TBL_UPDATE_LOAD_BYTE(update[i], "StuDes", s->StuDes, TBL_STRING, TBL_SET);
	i++;
    }
    if (s->Flag & FIS_K_STUDYCOMP_STUID) {
	TBL_UPDATE_LOAD_BYTE(update[i], "StuID", s->StuID, TBL_STRING, TBL_SET);
	i++;
    }
    if (s->Flag & FIS_K_STUDYCOMP_STUCOMSTAID) {
	TBL_UPDATE_LOAD_BYTE(update[i], "StuComStaID", s->StuComStaID,
			     TBL_STRING, TBL_SET);
	i++;
    }
    if (i == 0)
	return FIS_NORMAL;

    update[i].FieldName = NULL;
    cond = TBL_Update(&h->studyComponentHandle, criteria, update);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_UPDATEFAILED),
				  "(FIS)updateStudyComponentRecord");

    return FIS_NORMAL;
}

/* updateResultsRecord
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
updateResultsRecord(PRIVATE_HANDLE * h, FIS_RESULTSRECORD * r)
{
    CONDITION
    cond;
    TBL_UPDATE
	update[10];
    TBL_CRITERIA
	criteria[2];
    int
        i;

    if (r->Type != FIS_K_RESULTS)
	return COND_PushCondition(FIS_COND(FIS_ILLEGALRECORDTYPE),
			      "Results Record", "(FIS)updateResultsRecord");

    if (r->ResUID[0] == '\0')
	return COND_PushCondition(FIS_COND(FIS_NULLSTRING), "Results UID",
				  "(FIS)updateResultsRecord");

    TBL_CRITERIA_LOAD_BYTE(criteria[0], "ResUID", r->ResUID, TBL_STRING, TBL_EQUAL);
    criteria[1].FieldName = NULL;

    i = 0;
    if ((r->Flag & FIS_K_RESULTS_STUINSUID) && (strlen(r->StuInsUID) != 0)) {
	TBL_UPDATE_LOAD_BYTE(update[i], "StuInsUID", r->StuInsUID, TBL_STRING, TBL_SET);
	i++;
    }
    if (r->Flag & FIS_K_RESULTS_RESID) {
	TBL_UPDATE_LOAD_BYTE(update[i], "ResID", r->ResID, TBL_STRING, TBL_SET);
	i++;
    }
    if (r->Flag & FIS_K_RESULTS_IMP) {
	TBL_UPDATE_LOAD_BYTE(update[i], "Imp", r->Imp, TBL_TEXT, TBL_SET);
	i++;
    }
    update[i].FieldName = NULL;

    cond = TBL_Update(&h->resultsHandle, criteria, update);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_RESULTSUPDATEFAILED),
				  "(FIS)updateResultsRecord");

    return FIS_NORMAL;
}

/* updateInterpretatioinRecord
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
updateInterpretationRecord(PRIVATE_HANDLE * h, FIS_INTERPRETATIONRECORD * r)
{
    CONDITION
    cond;
    TBL_UPDATE
	update[10];
    TBL_CRITERIA
	criteria[2];
    int
        i;

    if (r->Type != FIS_K_INTERPRETATION)
	return COND_PushCondition(FIS_COND(FIS_ILLEGALRECORDTYPE),
		"Interpretation Record", "(FIS)updateInterpretationRecord");

    if (r->IntUID[0] == '\0')
	return COND_PushCondition(FIS_COND(FIS_NULLSTRING), "Interpretation UID",
				  "(FIS)updateInterpretationRecord");

    TBL_CRITERIA_LOAD_BYTE(criteria[0], "IntUID", r->IntUID, TBL_STRING, TBL_EQUAL);
    criteria[1].FieldName = NULL;

    i = 0;
    if ((r->Flag & FIS_K_INTERP_RESUID) && (strlen(r->ResUID) != 0)) {
	TBL_UPDATE_LOAD_BYTE(update[i], "ResUID", r->ResUID, TBL_STRING, TBL_SET);
	i++;
    }
    if (r->Flag & FIS_K_INTERP_INTID) {
	TBL_UPDATE_LOAD_BYTE(update[i], "IntID", r->IntID, TBL_STRING, TBL_SET);
	i++;
    }
    if (r->Flag & FIS_K_INTERP_INTTEX) {
	TBL_UPDATE_LOAD_BYTE(update[i], "IntTex", r->IntTex, TBL_TEXT, TBL_SET);
	i++;
    }
    if (r->Flag & FIS_K_INTERP_INTTYPID) {
	TBL_UPDATE_LOAD_BYTE(update[i], "IntTypID", r->IntTypID, TBL_STRING, TBL_SET);
	i++;
    }
    if (r->Flag & FIS_K_INTERP_INTSTAID) {
	TBL_UPDATE_LOAD_BYTE(update[i], "IntStaID", r->IntStaID, TBL_STRING, TBL_SET);
	i++;
    }
    if (r->Flag & FIS_K_INTERP_INTDIADES) {
	TBL_UPDATE_LOAD_BYTE(update[i], "IntDiaDes", r->IntDiaDes, TBL_TEXT, TBL_SET);
	i++;
    }
    if (r->Flag & FIS_K_INTERP_PHYAPPINT) {
	TBL_UPDATE_LOAD_BYTE(update[i], "PhyAppInt", r->PhyAppInt, TBL_STRING, TBL_SET);
	i++;
    }
    if (r->Flag & FIS_K_INTERP_INTAPPDAT) {
	TBL_UPDATE_LOAD_BYTE(update[i], "IntAppDat", r->IntAppDat, TBL_STRING, TBL_SET);
	i++;
    }
    if (r->Flag & FIS_K_INTERP_INTAPPTIM) {
	TBL_UPDATE_LOAD_BYTE(update[i], "IntAppTim", r->IntAppTim, TBL_STRING, TBL_SET);
	i++;
    }
    update[i].FieldName = NULL;

    cond = TBL_Update(&h->interpretationHandle, criteria, update);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_INTERPRETATIONUPDATEFAILED),
				  "(FIS)updateInterpretationRecord");

    return FIS_NORMAL;
}

static CONDITION
updateStorageCommitReqRecord(PRIVATE_HANDLE * h,
			     FIS_STORAGECOMMITREQRECORD * r)
{
    CONDITION
    cond;
    TBL_UPDATE
	update[10];
    TBL_CRITERIA
	criteria[2];
    int
        i;
    long scratchDate2;
    double scratchTime2;

    if (r->Type != FIS_K_STORAGECOMMITREQ)
	return COND_PushCondition(FIS_COND(FIS_ILLEGALRECORDTYPE),
	 "Storage Commitment Request", "(FIS)updateStorageCommitReqRecord");

    if (r->TraUID[0] == '\0')
	return COND_PushCondition(FIS_COND(FIS_NULLSTRING), "Transaction UID",
				  "(FIS)updateStorageCommitReqRecord");

    TBL_CRITERIA_LOAD_BYTE(criteria[0], "TraUID", r->TraUID, TBL_STRING, TBL_EQUAL);
    criteria[1].FieldName = NULL;

#if 0
#define FIS_K_SCOMMIT_REQDAT  (1 << 3)
#define FIS_K_SCOMMIT_REQTIM  (1 << 4)
#define FIS_K_SCOMMIT_RESDAT  (1 << 5)
#define FIS_K_SCOMMIT_RESTIM  (1 << 6)
#endif

    i = 0;
    if ((r->Flag & FIS_K_SCOMMIT_REQAE) && (strlen(r->ReqAE) != 0)) {
	TBL_UPDATE_LOAD_BYTE(update[i], "ReqAE", r->ReqAE, TBL_STRING, TBL_SET);
	i++;
    }
    if ((r->Flag & FIS_K_SCOMMIT_RESAE) && (strlen(r->ResAE) != 0)) {
	TBL_UPDATE_LOAD_BYTE(update[i], "ResAE", r->ResAE, TBL_STRING, TBL_SET);
	i++;
    }
    if (r->Flag & FIS_K_SCOMMIT_RESDAT) {
	scratchDate2 = UTL_ConvertDatetoLong(r->ResDat);
	TBL_UPDATE_LOAD_NUM(update[i], "ResDat", scratchDate2, TBL_SIGNED4, TBL_SET);
	i++;
    }
    if (r->Flag & FIS_K_SCOMMIT_RESTIM) {
	scratchTime2 = UTL_ConvertTimetoFloat(r->ResTim);
	TBL_UPDATE_LOAD_NUM(update[i], "ResTim", scratchTime2, TBL_FLOAT4, TBL_SET);
	i++;
    }
    update[i].FieldName = NULL;

    cond = TBL_Update(&h->storageCommitReqHandle, criteria, update);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_STORAGECOMMITREQUPDATEFAILED),
				  "(FIS)updateStorageCommitReqRecord");

    return FIS_NORMAL;
}
