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
** Module Name(s):	FIS_Get
**			getPatientRecord
**			patientCallback
**			getStudyRecord
**			studyCallback
** Author, Date:	Stephen Moore, 12-Jul-94
** Intent:		This file contains functions for getting FIS records.
**			These functions implement specific get functions that
**			are useful for implementations of DICOM gets for
**			information systems (management classes).
** Last Update:		$Author: smm $, $Date: 1997-05-30 18:54:22 $
** Source File:		$RCSfile: fisget.c,v $
** Revision:		$Revision: 1.13 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.13 $ $RCSfile: fisget.c,v $";

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
getPatientRecord(PRIVATE_HANDLE * h,
		 FIS_DATA_TYPE criteriaType, char *uid, long listFlag,
		 LST_HEAD * getList, FIS_PATIENTRECORD * r, long *count);
static CONDITION patientCallback(TBL_FIELD * field, int count, LST_HEAD * l);
static CONDITION
getVisitRecord(PRIVATE_HANDLE * h,
	       FIS_DATA_TYPE criteriaType, char *uid, long listFlag,
	       LST_HEAD * getList, FIS_VISITRECORD * r, long *count);
static CONDITION visitCallback(TBL_FIELD * field, int count, LST_HEAD * l);
static CONDITION
getStudyRecord(PRIVATE_HANDLE * h,
	       FIS_DATA_TYPE criteriaType, char *uid, long listFlag,
	       LST_HEAD * getList, FIS_STUDYRECORD * r, long *count);
static CONDITION studyCallback(TBL_FIELD * field, int count, LST_HEAD * l);

static CONDITION
getStudyComponentRecord(PRIVATE_HANDLE * h,
			FIS_DATA_TYPE criteriaType, char *uid, long listFlag,
			LST_HEAD * getList, FIS_STUDYCOMPONENTRECORD * r,
			long *count);
static CONDITION studyComponentCallback(TBL_FIELD * field, int count, LST_HEAD * l);

static CONDITION
getSCSeriesRecord(PRIVATE_HANDLE * h,
		  FIS_DATA_TYPE criteriaType, char *uid, long listFlag,
		  LST_HEAD * getList, FIS_SCSERIESRECORD * r,
		  long *count);
static CONDITION scSeriesCallback(TBL_FIELD * field, int count, LST_HEAD * l);

static CONDITION
getSCImageRecord(PRIVATE_HANDLE * h,
		 FIS_DATA_TYPE criteriaType, char *uid, long listFlag,
		 LST_HEAD * getList, FIS_SCIMAGERECORD * r,
		 long *count);
static CONDITION scImageCallback(TBL_FIELD * field, int count, LST_HEAD * l);


static CONDITION
getResultsRecord(PRIVATE_HANDLE * h,
		 FIS_DATA_TYPE criteriaType, char *uid, long listFlag,
		 LST_HEAD * getList, FIS_RESULTSRECORD * r, long *count);
static CONDITION resultsCallback(TBL_FIELD * field, int count, LST_HEAD * l);
static CONDITION
getInterpretationRecord(PRIVATE_HANDLE * h,
			FIS_DATA_TYPE criteriaType, char *uid, long listFlag,
			LST_HEAD * getList, FIS_INTERPRETATIONRECORD * r,
			long *count);
static CONDITION interpretationCallback(TBL_FIELD * field, int count, LST_HEAD * l);


/* getPatientRecord
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

static FIS_PATIENTRECORD
    patient;
static S32 patientBD;

static CONDITION
getPatientRecord(PRIVATE_HANDLE * h,
   FIS_DATA_TYPE criteriaType, char *uid, long listFlag, LST_HEAD * getList,
		 FIS_PATIENTRECORD * r, long *count)
{
    CONDITION
    cond;
    TBL_CRITERIA
	criteria[2];
    FIS_PATIENTRECORD
	* p;

    static TBL_FIELD fields[] = {
	{"PatID", TBL_STRING, sizeof(patient.PatID), sizeof(patient.PatID),
	0, (void *) &patient.PatID[0]},
	{"PatUID", TBL_STRING, sizeof(patient.PatUID), sizeof(patient.PatUID),
	0, (void *) &patient.PatUID[0]},
	{"PatNam", TBL_STRING, sizeof(patient.PatNam), sizeof(patient.PatNam),
	0, (void *) &patient.PatNam[0]},
	{"PatSex", TBL_STRING, sizeof(patient.PatSex), sizeof(patient.PatSex),
	0, (void *) &patient.PatSex[0]},
	{"PatBirDat", TBL_SIGNED4, sizeof(patientBD), sizeof(patientBD),
	0, (void *) &patientBD},
	{NULL},
    };

    criteria[0].FieldName = criteria[1].FieldName = NULL;
    if (uid != NULL) {
	if (uid[0] != '\0') {
	    TBL_CRITERIA_LOAD_BYTE(criteria[0], "PatUID", uid, TBL_STRING, TBL_EQUAL);
	}
    }
    memset(&patient, 0, sizeof(patient));
    patient.Type = FIS_K_PATIENT;
    cond = TBL_Select(&h->patientHandle, criteria, fields, count, patientCallback,
		      getList);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_PATIENTGETFAILED),
				  "(FIS)getPatientRecord");

    if (r != NULL)
	*r = patient;

    if (listFlag & FIS_K_PAT_STUDYLIST) {
	if (getList != NULL) {
	    p = LST_Head(&getList);
	    (void) LST_Position(&getList, p);
	} else
	    p = r;
	while (p != NULL) {
	    p->StudyList = LST_Create();
	    p->Flag |= FIS_K_PAT_STUDYLIST;
	    cond = getStudyRecord(h, FIS_K_PATIENT, p->PatUID, 0, p->StudyList, NULL, NULL);
	    if (cond != FIS_NORMAL)
		return cond;
	    if (getList != NULL)
		p = LST_Next(&getList);
	    else
		p = NULL;
	}
    }
/*
    if (listFlag & FIS_K_PAT_VISITLIST) {
	if (getList != NULL) {
	    p = LST_Head(&getList);
	    (void) LST_Position(&getList, p);
	} else
	    p = r;
	while (p != NULL) {
	    p->VisitList = LST_Create();
	    p->Flag |= FIS_K_PAT_VISITLIST;
	    cond = getVisitRecord(h, FIS_K_PATIENT, p->PatUID, 0, p->VisitList, NULL, NULL);
	    if (cond != FIS_NORMAL)
		return cond;
	    if (getList != NULL)
		p = LST_Next(&getList);
	    else
		p = NULL;
	}
    }
*/
    return FIS_NORMAL;
}

static CONDITION
patientCallback(TBL_FIELD * field, int count, LST_HEAD * l)
{
    FIS_PATIENTRECORD *p;
    int i;
    static FLAG_MAP map[] = {
	{"PatID", FIS_K_PAT_PATID},
	{"PatUID", FIS_K_PAT_PATUID},
	{"PatNam", FIS_K_PAT_PATNAM},
	{"PatSex", FIS_K_PAT_SEX},
	{"PatBirDat", FIS_K_PAT_BIRDAT},
    };

    patient.Flag = 0;
    while (field->FieldName != NULL) {
	for (i = 0; i < (int) DIM_OF(map); i++) {
	    if (strcmp(field->FieldName, map[i].FieldName) == 0) {
		if (!field->Value.IsNull)
		    patient.Flag |= map[i].Flag;
		break;
	    }
	}
	field++;
    }

    patient.StudyList = patient.VisitList = patient.PatientAliasList = NULL;
    UTL_ConvertLongtoDate((long) patientBD, patient.PatBirDat);
    if (l != NULL) {
	p = malloc(sizeof(*p));
	if (p == NULL)
	    return TBL_EARLYEXIT;

	*p = patient;

	if (LST_Enqueue(&l, p) != LST_NORMAL)
	    return TBL_EARLYEXIT;
    }
    return TBL_NORMAL;
}


static FIS_VISITRECORD
    visit;

static CONDITION
getVisitRecord(PRIVATE_HANDLE * h,
   FIS_DATA_TYPE criteriaType, char *uid, long listFlag, LST_HEAD * getList,
	       FIS_VISITRECORD * r, long *count)
{
    CONDITION cond;
    TBL_CRITERIA criteria[2];
    FIS_VISITRECORD *p;

    static TBL_FIELD fields[] = {
	{TBL_FIELD_STRING("VisUID", visit.VisUID)},
	{TBL_FIELD_STRING("PatUID", visit.PatUID)},
	{TBL_FIELD_STRING("RefPhyNam", visit.RefPhyNam)},
	{NULL},
    };

    criteria[0].FieldName = criteria[1].FieldName = NULL;
    if (uid != NULL) {
	if (uid[0] != '\0') {
	    if (criteriaType == FIS_K_PATIENT) {
		TBL_CRITERIA_LOAD_BYTE(criteria[0], "PatUID", uid, TBL_STRING, TBL_EQUAL);
	    } else {
		TBL_CRITERIA_LOAD_BYTE(criteria[0], "VisUID", uid, TBL_STRING, TBL_EQUAL);
	    }
	}
    }
    memset(&visit, 0, sizeof(visit));
    visit.Type = FIS_K_VISIT;
    cond = TBL_Select(&h->visitHandle, criteria, fields, count, visitCallback,
		      getList);
    if (cond != TBL_NORMAL)
	return 0;

    if (r != NULL)
	*r = visit;

    if (listFlag & FIS_K_VISIT_STUDYLIST) {
	if (getList != NULL) {
	    p = LST_Head(&getList);
	    (void) LST_Position(&getList, p);
	} else
	    p = r;
	while (p != NULL) {
	    p->StudyList = LST_Create();
	    cond = getStudyRecord(h, FIS_K_VISIT, p->VisUID, 0, p->StudyList, NULL, NULL);
	    if (cond != FIS_NORMAL)
		return cond;
	    if (getList != NULL)
		p = LST_Next(&getList);
	    else
		p = NULL;
	}
    }
    return FIS_NORMAL;
}

static CONDITION
visitCallback(TBL_FIELD * field, int count, LST_HEAD * l)
{
    FIS_VISITRECORD *v;

    int i;
    static FLAG_MAP map[] = {
	{"VisUID", FIS_K_VISIT_VISUID},
	{"PatUID", FIS_K_VISIT_PATUID},
	{"RefPhyNam", FIS_K_VISIT_REFPHYNAM}
    };


    visit.Flag = 0;
    while (field->FieldName != NULL) {
	for (i = 0; i < (int) DIM_OF(map); i++) {
	    if (strcmp(field->FieldName, map[i].FieldName) == 0) {
		if (!field->Value.IsNull)
		    visit.Flag |= map[i].Flag;
		break;
	    }
	}
	field++;
    }

    if (l != NULL) {
	v = malloc(sizeof(*v));
	if (v == NULL)
	    return TBL_EARLYEXIT;

	*v = visit;
	if (LST_Enqueue(&l, v) != LST_NORMAL)
	    return TBL_EARLYEXIT;
    }
    return TBL_NORMAL;
}

/* getStudyRecord
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

static FIS_STUDYRECORD
    study;
static S32 studyDate;
static float studyTime;

static CONDITION
getStudyRecord(PRIVATE_HANDLE * h,
   FIS_DATA_TYPE criteriaType, char *uid, long listFlag, LST_HEAD * getList,
	       FIS_STUDYRECORD * r, long *count)
{
    CONDITION cond;
    TBL_CRITERIA criteria[2];
    FIS_STUDYRECORD *p;

    static TBL_FIELD fields[] = {
	{TBL_FIELD_STRING("PatUID", study.PatUID)},
	{TBL_FIELD_STRING("StuInsUID", study.StuInsUID)},
	{TBL_FIELD_STRING("VisUID", study.VisUID)},
	{TBL_FIELD_STRING("AccNum", study.AccNum)},
	{TBL_FIELD_STRING("StuID", study.StuID)},
	{"SchStuStaDat", TBL_SIGNED4, sizeof(studyDate), sizeof(studyDate),
	0, (void *) &studyDate},
	{"SchStuStaTim", TBL_FLOAT4, sizeof(studyTime), sizeof(studyTime),
	0, (void *) &studyTime},
	{TBL_FIELD_STRING("SchStuLoc", study.SchStuLoc)},
	{TBL_FIELD_STRING("ProDes", study.ProDes)},
	{TBL_FIELD_STRING("StuStaID", study.StuStaID)},
	{NULL},
    };

    criteria[0].FieldName = criteria[1].FieldName = NULL;
    if (uid != NULL) {
	if (uid[0] != '\0') {
	    if (criteriaType == FIS_K_PATIENT) {
		TBL_CRITERIA_LOAD_BYTE(criteria[0], "PatUID", uid, TBL_STRING, TBL_EQUAL);
	    } else {
		TBL_CRITERIA_LOAD_BYTE(criteria[0], "StuInsUID", uid, TBL_STRING, TBL_EQUAL);
	    }
	}
    }
    (void) memset(&study, 0, sizeof(study));
    study.Type = FIS_K_STUDY;
    cond = TBL_Select(&h->studyHandle, criteria, fields, count, studyCallback,
		      getList);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_STUDYGETFAILED),
				  "(FIS)getStudyRecord");

    if (r != NULL)
	*r = study;

    if (listFlag & FIS_K_STU_STUDYCOMPONENTLIST) {
	if (getList != NULL) {
	    p = LST_Head(&getList);
	    (void) LST_Position(&getList, p);
	} else
	    p = r;
	while (p != NULL) {
	    p->StudyComponentList = LST_Create();
	    p->Flag |= FIS_K_STU_STUDYCOMPONENTLIST;
	    cond = getStudyComponentRecord(h, FIS_K_STUDY, p->StuInsUID,
				      0, p->StudyComponentList, NULL, NULL);
	    if (cond != FIS_NORMAL)
		return cond;
	    if (getList != NULL)
		p = LST_Next(&getList);
	    else
		p = NULL;
	}
    }
    if (listFlag & FIS_K_STU_RESULTSLIST) {
	if (getList != NULL) {
	    p = LST_Head(&getList);
	    (void) LST_Position(&getList, p);
	} else
	    p = r;
	while (p != NULL) {
	    p->ResultsList = LST_Create();
	    p->Flag |= FIS_K_STU_RESULTSLIST;
	    cond = getResultsRecord(h, FIS_K_STUDY, p->StuInsUID, 0,
				    p->ResultsList, NULL, NULL);
	    if (cond != FIS_NORMAL)
		return cond;
	    if (getList != NULL)
		p = LST_Next(&getList);
	    else
		p = NULL;
	}
    }
    return FIS_NORMAL;
}


static CONDITION
selectStudyRecord(PRIVATE_HANDLE * h,
		  FIS_CRITERIA_TYPE criteriaType, const char *criteriaString,
		  long listFlag, LST_HEAD * getList,
		  FIS_STUDYRECORD * r, long *count)
{
    CONDITION cond = FIS_NORMAL;
    TBL_CRITERIA criteria[2];
    char localString[1024];
    FIS_STUDYRECORD *p;

    static TBL_FIELD fields[] = {
	{TBL_FIELD_STRING("PatUID", study.PatUID)},
	{TBL_FIELD_STRING("StuInsUID", study.StuInsUID)},
	{TBL_FIELD_STRING("VisUID", study.VisUID)},
	{TBL_FIELD_STRING("AccNum", study.AccNum)},
	{TBL_FIELD_STRING("StuID", study.StuID)},
	{"SchStuStaDat", TBL_SIGNED4, sizeof(studyDate), sizeof(studyDate),
	0, (void *) &studyDate},
	{"SchStuStaTim", TBL_FLOAT4, sizeof(studyTime), sizeof(studyTime),
	0, (void *) &studyTime},
	{TBL_FIELD_STRING("SchStuLoc", study.SchStuLoc)},
	{TBL_FIELD_STRING("ProDes", study.ProDes)},
	{TBL_FIELD_STRING("StuStaID", study.StuStaID)},
	{NULL},
    };

    criteria[0].FieldName = criteria[1].FieldName = NULL;
    strcpy(localString, criteriaString);
    switch (criteriaType) {
    case FIS_K_ACCESSION:
	TBL_CRITERIA_LOAD_BYTE(criteria[0], "AccNum", localString, TBL_STRING, TBL_EQUAL);
	break;
    default:
	cond = 0;		/* Repair */
    }

    if (cond != FIS_NORMAL)
	return cond;

    (void) memset(&study, 0, sizeof(study));
    study.Type = FIS_K_STUDY;
    cond = TBL_Select(&h->studyHandle, criteria, fields, count, studyCallback,
		      getList);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_STUDYGETFAILED),
				  "(FIS)getStudyRecord");

    if (r != NULL)
	*r = study;

    if (listFlag & FIS_K_STU_STUDYCOMPONENTLIST) {
	if (getList != NULL) {
	    p = LST_Head(&getList);
	    (void) LST_Position(&getList, p);
	} else
	    p = r;
	while (p != NULL) {
	    p->StudyComponentList = LST_Create();
	    p->Flag |= FIS_K_STU_STUDYCOMPONENTLIST;
	    cond = getStudyComponentRecord(h, FIS_K_STUDY, p->StuInsUID,
				      0, p->StudyComponentList, NULL, NULL);
	    if (cond != FIS_NORMAL)
		return cond;
	    if (getList != NULL)
		p = LST_Next(&getList);
	    else
		p = NULL;
	}
    }
    if (listFlag & FIS_K_STU_RESULTSLIST) {
	if (getList != NULL) {
	    p = LST_Head(&getList);
	    (void) LST_Position(&getList, p);
	} else
	    p = r;
	while (p != NULL) {
	    p->ResultsList = LST_Create();
	    p->Flag |= FIS_K_STU_RESULTSLIST;
	    cond = getResultsRecord(h, FIS_K_STUDY, p->StuInsUID, 0,
				    p->ResultsList, NULL, NULL);
	    if (cond != FIS_NORMAL)
		return cond;
	    if (getList != NULL)
		p = LST_Next(&getList);
	    else
		p = NULL;
	}
    }
    return FIS_NORMAL;
}


static CONDITION
studyCallback(TBL_FIELD * field, int count, LST_HEAD * l)
{
    FIS_STUDYRECORD
    * s;

    int i;
    static FLAG_MAP map[] = {
	{"PatUID", FIS_K_STU_PATUID},
	{"StuInsUID", FIS_K_STU_STUINSUID},
	{"VisUID", FIS_K_STU_VISUID},
	{"AccNum", FIS_K_STU_ACCNUM},
	{"StuID", FIS_K_STU_STUID},
	{"SchStuStaDat", FIS_K_STU_SCHSTUSTADAT},
	{"SchStuStaTim", FIS_K_STU_SCHSTUSTATIM},
	{"SchStuLoc", FIS_K_STU_SCHSTULOC},
	{"ProDes", FIS_K_STU_PRODES},
	{"StuStaID", FIS_K_STU_STUSTAID}
    };


    study.Flag = 0;
    while (field->FieldName != NULL) {
	for (i = 0; i < (int) DIM_OF(map); i++) {
	    if (strcmp(field->FieldName, map[i].FieldName) == 0) {
		if (!field->Value.IsNull)
		    study.Flag |= map[i].Flag;
		break;
	    }
	}
	field++;
    }

    if (study.Flag & FIS_K_STU_SCHSTUSTADAT)
	UTL_ConvertLongtoDate((long) studyDate, study.SchStuStaDat);
    if (study.Flag & FIS_K_STU_SCHSTUSTATIM)
	UTL_ConvertFloattoTime(studyTime, study.SchStuStaTim);
    if (l != NULL) {
	s = malloc(sizeof(*s));
	if (s == NULL)
	    return TBL_EARLYEXIT;

	*s = study;
	if (LST_Enqueue(&l, s) != LST_NORMAL)
	    return TBL_EARLYEXIT;
    }
    return TBL_NORMAL;
}


static FIS_STUDYCOMPONENTRECORD
    studyComponent;

static CONDITION
getStudyComponentRecord(PRIVATE_HANDLE * h,
   FIS_DATA_TYPE criteriaType, char *uid, long listFlag, LST_HEAD * getList,
			FIS_STUDYCOMPONENTRECORD * r, long *count)
{
    CONDITION cond;
    TBL_CRITERIA criteria[2];
    FIS_STUDYCOMPONENTRECORD *p;

    static TBL_FIELD fields[] = {
	{TBL_FIELD_STRING("StuComUID", studyComponent.StuComUID)},
	{TBL_FIELD_STRING("StuInsUID", studyComponent.StuInsUID)},
	{TBL_FIELD_STRING("Mod", studyComponent.Mod)},
	{TBL_FIELD_STRING("StuDes", studyComponent.StuDes)},
	{TBL_FIELD_STRING("ProCodVal", studyComponent.ProCodVal)},
	{TBL_FIELD_STRING("ProCodSchDes", studyComponent.ProCodSchDes)},
	{TBL_FIELD_STRING("ProCodMea", studyComponent.ProCodMea)},
	{TBL_FIELD_STRING("StuComStaID", studyComponent.StuComStaID)},
	{TBL_FIELD_STRING("StuID", studyComponent.StuID)},
	{NULL},
    };

    criteria[0].FieldName = criteria[1].FieldName = NULL;
    if (uid != NULL) {
	if (uid[0] != '\0') {
	    if (criteriaType == FIS_K_STUDY) {
		TBL_CRITERIA_LOAD_BYTE(criteria[0], "StuInsUID", uid, TBL_STRING, TBL_EQUAL);
	    } else {
		TBL_CRITERIA_LOAD_BYTE(criteria[0], "StuComUID", uid, TBL_STRING, TBL_EQUAL);
	    }
	}
    }
    (void) memset(&studyComponent, 0, sizeof(studyComponent));
    studyComponent.Type = FIS_K_STUDYCOMPONENT;
    cond = TBL_Select(&h->studyComponentHandle, criteria, fields, count,
		      studyComponentCallback, getList);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_STUDYGETFAILED),
				  "(FIS)getStudyRecord");

    if (r != NULL)
	*r = studyComponent;

    if (listFlag & FIS_K_STUDYCOMP_SERIESLIST) {
	if (getList != NULL) {
	    p = LST_Head(&getList);
	    (void) LST_Position(&getList, p);
	} else
	    p = r;
	while (p != NULL) {
	    p->SeriesList = LST_Create();
	    p->Flag |= FIS_K_STUDYCOMP_SERIESLIST;
	    cond = getSCSeriesRecord(h, FIS_K_STUDYCOMPONENT, p->StuComUID,
				     FIS_K_SCSERIES_IMAGELIST, p->SeriesList,
				     NULL, NULL);
	    if (cond != FIS_NORMAL)
		return cond;
	    if (getList != NULL)
		p = LST_Next(&getList);
	    else
		p = NULL;
	}
    }
    return FIS_NORMAL;
}

static CONDITION
studyComponentCallback(TBL_FIELD * field, int count, LST_HEAD * l)
{
    FIS_STUDYCOMPONENTRECORD
    * s;

    int i;
    static FLAG_MAP map[] = {
	{"StuComUID", FIS_K_STUDYCOMP_STUCOMUID},
	{"StuInsUID", FIS_K_STUDYCOMP_STUINSUID},
	{"Mod", FIS_K_STUDYCOMP_MOD},
	{"StuDes", FIS_K_STUDYCOMP_STUDES},
	{"ProCodVal", FIS_K_STUDYCOMP_PROCODVAL},
	{"ProCodSch", FIS_K_STUDYCOMP_PROCODSCHDES},
	{"ProCodMea", FIS_K_STUDYCOMP_PROCODMEA},
	{"StuComStaID", FIS_K_STUDYCOMP_STUCOMSTAID},
	{"StuID", FIS_K_STUDYCOMP_STUID},
    };


    studyComponent.Flag = 0;
    while (field->FieldName != NULL) {
	for (i = 0; i < (int) DIM_OF(map); i++) {
	    if (strcmp(field->FieldName, map[i].FieldName) == 0) {
		if (!field->Value.IsNull)
		    studyComponent.Flag |= map[i].Flag;
		break;
	    }
	}
	field++;
    }

    if (l != NULL) {
	s = malloc(sizeof(*s));
	if (s == NULL)
	    return TBL_EARLYEXIT;

	*s = studyComponent;
	if (LST_Enqueue(&l, s) != LST_NORMAL)
	    return TBL_EARLYEXIT;
    }
    return TBL_NORMAL;
}

static FIS_SCSERIESRECORD
    scSeries;

static CONDITION
getSCSeriesRecord(PRIVATE_HANDLE * h,
   FIS_DATA_TYPE criteriaType, char *uid, long listFlag, LST_HEAD * getList,
		  FIS_SCSERIESRECORD * r, long *count)
{
    CONDITION cond;
    TBL_CRITERIA criteria[2];
    FIS_SCSERIESRECORD *p;

    static TBL_FIELD fields[] = {
	{TBL_FIELD_STRING("SerInsUID", scSeries.SerInsUID)},
	{TBL_FIELD_STRING("StuComUID", scSeries.StuComUID)},
	{TBL_FIELD_STRING("SerDat", scSeries.SerDat)},
	{TBL_FIELD_STRING("SerTim", scSeries.SerTim)},
	{TBL_FIELD_STRING("RetAETit", scSeries.RetAETit)},
	{TBL_FIELD_STRING("StoMedFilSetID", scSeries.StoMedFilSetID)},
	{TBL_FIELD_STRING("StoMedFilSetUID", scSeries.StoMedFilSetUID)},
	{NULL},
    };

    criteria[0].FieldName = criteria[1].FieldName = NULL;
    if (uid != NULL) {
	if (uid[0] != '\0') {
	    if (criteriaType == FIS_K_STUDYCOMPONENT) {
		TBL_CRITERIA_LOAD_BYTE(criteria[0], "StuComUID", uid, TBL_STRING, TBL_EQUAL);
	    } else {
		TBL_CRITERIA_LOAD_BYTE(criteria[0], "SerInsUID", uid, TBL_STRING, TBL_EQUAL);
	    }
	}
    }
    (void) memset(&scSeries, 0, sizeof(scSeries));
    scSeries.Type = FIS_K_SCSERIES;
    cond = TBL_Select(&h->studyComponentSeriesHandle, criteria, fields, count,
		      scSeriesCallback, getList);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_STUDYGETFAILED),
				  "(FIS)getSCSeriesRecord");

    if (r != NULL)
	*r = scSeries;


    if (listFlag & FIS_K_SCSERIES_IMAGELIST) {
	if (getList != NULL) {
	    p = LST_Head(&getList);
	    (void) LST_Position(&getList, p);
	} else
	    p = r;
	while (p != NULL) {
	    p->ImageList = LST_Create();
	    cond = getSCImageRecord(h, FIS_K_SCSERIES, p->SerInsUID, 0,
				    p->ImageList, NULL, NULL);
	    if (cond != FIS_NORMAL)
		return cond;
	    if (getList != NULL)
		p = LST_Next(&getList);
	    else
		p = NULL;
	}
    }
    return FIS_NORMAL;
}

static CONDITION
scSeriesCallback(TBL_FIELD * field, int count, LST_HEAD * l)
{
    FIS_SCSERIESRECORD
    * s;

    int i;
    static FLAG_MAP map[] = {
	{"SerInsUID", FIS_K_SCSERIES_SERINSUID},
	{"StuComUID", FIS_K_SCSERIES_STUCOMUID},
	{"SerDat", FIS_K_SCSERIES_SERDAT},
	{"SerTim", FIS_K_SCSERIES_SERTIM},
	{"RetAETit", FIS_K_SCSERIES_RETAETIT},
	{"StoMedFilSetID", FIS_K_SCSERIES_STOMEDFILSETID},
	{"StoMedFileSetUID", FIS_K_SCSERIES_STOMEDFILSETUID},
    };


    scSeries.Flag = 0;
    while (field->FieldName != NULL) {
	for (i = 0; i < (int) DIM_OF(map); i++) {
	    if (strcmp(field->FieldName, map[i].FieldName) == 0) {
		if (!field->Value.IsNull)
		    scSeries.Flag |= map[i].Flag;
		break;
	    }
	}
	field++;
    }

    if (l != NULL) {
	s = malloc(sizeof(*s));
	if (s == NULL)
	    return TBL_EARLYEXIT;

	*s = scSeries;
	if (LST_Enqueue(&l, s) != LST_NORMAL)
	    return TBL_EARLYEXIT;
    }
    return TBL_NORMAL;
}

static FIS_SCIMAGERECORD
    scImage;

static CONDITION
getSCImageRecord(PRIVATE_HANDLE * h,
   FIS_DATA_TYPE criteriaType, char *uid, long listFlag, LST_HEAD * getList,
		 FIS_SCIMAGERECORD * r, long *count)
{
    CONDITION cond;
    TBL_CRITERIA criteria[2];
    FIS_SCIMAGERECORD *p;

    static TBL_FIELD fields[] = {
	{TBL_FIELD_STRING("SOPInsUID", scImage.SOPInsUID)},
	{TBL_FIELD_STRING("SerInsUID", scImage.SerInsUID)},
	{TBL_FIELD_STRING("StuComUID", scImage.StuComUID)},
	{TBL_FIELD_STRING("SOPClaUID", scImage.SOPClaUID)},
	{TBL_FIELD_STRING("RetAETit", scImage.RetAETit)},
	{TBL_FIELD_STRING("StoMedFilSetID", scImage.StoMedFilSetID)},
	{TBL_FIELD_STRING("StoMedFilSetUID", scImage.StoMedFilSetUID)},
	{NULL},
    };

    criteria[0].FieldName = criteria[1].FieldName = NULL;
    if (uid != NULL) {
	if (uid[0] != '\0') {
	    if (criteriaType == FIS_K_SCSERIES) {
		TBL_CRITERIA_LOAD_BYTE(criteria[0], "SerInsUID", uid, TBL_STRING, TBL_EQUAL);
	    } else if (criteriaType == FIS_K_STUDYCOMPONENT) {
		TBL_CRITERIA_LOAD_BYTE(criteria[0], "StuComUID", uid, TBL_STRING, TBL_EQUAL);
	    } else {
		TBL_CRITERIA_LOAD_BYTE(criteria[0], "SOPInsUID", uid, TBL_STRING, TBL_EQUAL);
	    }
	}
    }
    (void) memset(&scImage, 0, sizeof(scImage));
    scImage.Type = FIS_K_SCIMAGE;
    cond = TBL_Select(&h->studyComponentImageHandle, criteria, fields, count,
		      scImageCallback, getList);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_STUDYGETFAILED),
				  "(FIS)getSCImageRecord");

    if (r != NULL)
	*r = scImage;

    return FIS_NORMAL;
}

static CONDITION
scImageCallback(TBL_FIELD * field, int count, LST_HEAD * l)
{
    FIS_SCIMAGERECORD
    * s;

    int i;
    static FLAG_MAP map[] = {
	{"SOPInsUID", FIS_K_SCIMAGE_SOPINSUID},
	{"SerInsUID", FIS_K_SCIMAGE_SERINSUID},
	{"StuComUID", FIS_K_SCIMAGE_STUCOMUID},
	{"SOPClaUID", FIS_K_SCIMAGE_SOPCLAUID},
	{"RetAETit", FIS_K_SCIMAGE_RETAETIT},
	{"StoMedFilSetID", FIS_K_SCIMAGE_STOMEDFILSETID},
	{"StoMedFileSetUID", FIS_K_SCIMAGE_STOMEDFILSETUID},
    };


    scImage.Flag = 0;
    while (field->FieldName != NULL) {
	for (i = 0; i < (int) DIM_OF(map); i++) {
	    if (strcmp(field->FieldName, map[i].FieldName) == 0) {
		if (!field->Value.IsNull)
		    scImage.Flag |= map[i].Flag;
		break;
	    }
	}
	field++;
    }

    if (l != NULL) {
	s = malloc(sizeof(*s));
	if (s == NULL)
	    return TBL_EARLYEXIT;

	*s = scImage;
	if (LST_Enqueue(&l, s) != LST_NORMAL)
	    return TBL_EARLYEXIT;
    }
    return TBL_NORMAL;
}




static FIS_RESULTSRECORD results;

static CONDITION
getResultsRecord(PRIVATE_HANDLE * h,
   FIS_DATA_TYPE criteriaType, char *uid, long listFlag, LST_HEAD * getList,
		 FIS_RESULTSRECORD * r, long *count)
{
    CONDITION cond;
    TBL_CRITERIA criteria[2];
    FIS_RESULTSRECORD *p;

    static TBL_FIELD fields[] = {
	{"ResUID", TBL_STRING, sizeof(results.ResUID), sizeof(results.ResUID),
	0, (void *) &results.ResUID[0]},
	{"StuInsUID", TBL_STRING, sizeof(results.StuInsUID),
	sizeof(results.StuInsUID), 0, (void *) &results.StuInsUID[0]},
	{"ResID", TBL_STRING, sizeof(results.ResID), sizeof(results.ResID),
	0, (void *) &results.ResID[0]},
	{"Imp", TBL_TEXT, sizeof(results.Imp), sizeof(results.Imp),
	0, (void *) &results.Imp[0]},
	{"InsCreDat", TBL_STRING, sizeof(results.InsCreDat),
	sizeof(results.InsCreDat), 0, (void *) &results.InsCreDat[0]},
	{"InsCreTim", TBL_STRING, sizeof(results.InsCreTim),
	sizeof(results.InsCreTim), 0, (void *) &results.InsCreTim[0]},
	{"InsCreUID", TBL_STRING, sizeof(results.InsCreUID),
	sizeof(results.InsCreUID), 0, (void *) &results.InsCreUID},
	{NULL}
    };

    criteria[0].FieldName = criteria[1].FieldName = NULL;
    if (uid != NULL) {
	if (uid[0] != '\0') {
	    if (criteriaType == FIS_K_STUDY) {
		TBL_CRITERIA_LOAD_BYTE(criteria[0], "StuInsUID", uid, TBL_STRING, TBL_EQUAL);
	    } else {
		TBL_CRITERIA_LOAD_BYTE(criteria[0], "ResUID", uid, TBL_STRING, TBL_EQUAL);
	    }
	}
    }
    (void) memset(&results, 0, sizeof(results));
    results.Type = FIS_K_RESULTS;
    cond = TBL_Select(&h->resultsHandle, criteria, fields, count,
		      resultsCallback, getList);
    if (cond != TBL_NORMAL)
	return FIS_GETFAILED;

    if (r != NULL)
	*r = results;

    if (listFlag & FIS_K_RESULTS_INTERPRETATIONLIST) {
	if (getList != NULL) {
	    p = LST_Head(&getList);
	    (void) LST_Position(&getList, p);
	} else
	    p = r;
	while (p != NULL) {
	    p->InterpretationList = LST_Create();
	    p->Flag |= FIS_K_RESULTS_INTERPRETATIONLIST;
	    cond = getInterpretationRecord(h, FIS_K_RESULTS, p->ResUID,
				      0, p->InterpretationList, NULL, NULL);
	    if (cond != FIS_NORMAL)
		return cond;

	    if (getList != NULL)
		p = LST_Next(&getList);
	    else
		p = NULL;
	}
    }
    return FIS_NORMAL;
}

static CONDITION
resultsCallback(TBL_FIELD * field, int count, LST_HEAD * l)
{
    FIS_RESULTSRECORD *r;
    CONDITION cond;
    int i;
    static FLAG_MAP map[] = {
	{"ResUID", FIS_K_RESULTS_RESUID},
	{"StuInsUID", FIS_K_RESULTS_STUINSUID},
	{"ResID", FIS_K_RESULTS_RESID},
	{"Imp", FIS_K_RESULTS_IMP},
	{"InsCreDat", FIS_K_RESULTS_INSCREDAT},
	{"InsCreTim", FIS_K_RESULTS_INSCRETIM},
	{"InsCreUID", FIS_K_RESULTS_INSCREUID}
    };

    results.Flag = 0;
    while (field->FieldName != NULL) {
	for (i = 0; i < (int) DIM_OF(map); i++) {
	    if (strcmp(field->FieldName, map[i].FieldName) == 0) {
		if (!field->Value.IsNull)
		    results.Flag |= map[i].Flag;
		break;
	    }
	}
	field++;
    }

    if (l != NULL) {
	r = malloc(sizeof(*r));
	if (r == NULL)
	    return TBL_EARLYEXIT;

	*r = results;
	if (LST_Enqueue(&l, r) != LST_NORMAL)
	    return TBL_EARLYEXIT;
    }
    return TBL_NORMAL;
}


static FIS_INTERPRETATIONRECORD interpretation;

static CONDITION
getInterpretationRecord(PRIVATE_HANDLE * h,
   FIS_DATA_TYPE criteriaType, char *uid, long listFlag, LST_HEAD * getList,
			FIS_INTERPRETATIONRECORD * r, long *count)
{
    CONDITION
    cond;
    TBL_CRITERIA
	criteria[2];

    static TBL_FIELD fields[] = {
	{"IntUID", TBL_STRING, sizeof(interpretation.IntUID),
	sizeof(interpretation.IntUID), 0, (void *) &interpretation.IntUID[0]},
	{"ResUID", TBL_STRING, sizeof(interpretation.ResUID),
	sizeof(interpretation.ResUID), 0, (void *) &interpretation.ResUID[0]},
	{"IntID", TBL_STRING, sizeof(interpretation.IntID),
	sizeof(interpretation.IntID), 0, (void *) &interpretation.IntID[0]},
	{"IntTex", TBL_TEXT, sizeof(interpretation.IntTex),
	sizeof(interpretation.IntTex), 0, (void *) &interpretation.IntTex[0]},
	{"IntDiaDes", TBL_TEXT, sizeof(interpretation.IntDiaDes),
	    sizeof(interpretation.IntDiaDes), 0,
	(void *) &interpretation.IntDiaDes[0]},
	{"IntTypID", TBL_STRING, sizeof(interpretation.IntTypID),
	sizeof(interpretation.IntTypID), 0, (void *) &interpretation.IntTypID[0]},
	{"IntStaID", TBL_STRING, sizeof(interpretation.IntStaID),
	sizeof(interpretation.IntStaID), 0, (void *) &interpretation.IntStaID[0]},

	{"InsCreDat", TBL_STRING, sizeof(interpretation.InsCreDat),
	sizeof(interpretation.InsCreDat), 0, (void *) &interpretation.InsCreDat[0]},
	{"InsCreTim", TBL_STRING, sizeof(interpretation.InsCreTim),
	sizeof(interpretation.InsCreTim), 0, (void *) &interpretation.InsCreTim[0]},
	{"InsCreUID", TBL_STRING, sizeof(interpretation.InsCreUID),
	sizeof(interpretation.InsCreUID), 0, (void *) &interpretation.InsCreUID},

	{"PhyAppInt", TBL_STRING, sizeof(interpretation.PhyAppInt),
	sizeof(interpretation.PhyAppInt), 0, (void *) &interpretation.PhyAppInt},
	{"IntAppDat", TBL_STRING, sizeof(interpretation.IntAppDat),
	sizeof(interpretation.IntAppDat), 0, (void *) &interpretation.IntAppDat},
	{"IntAppTim", TBL_STRING, sizeof(interpretation.IntAppTim),
	sizeof(interpretation.IntAppTim), 0, (void *) &interpretation.IntAppTim},
	{NULL},
    };

    criteria[0].FieldName = criteria[1].FieldName = NULL;
    if (uid != NULL) {
	if (uid[0] != '\0') {
	    if (criteriaType == FIS_K_RESULTS) {
		TBL_CRITERIA_LOAD_BYTE(criteria[0], "ResUID", uid, TBL_STRING, TBL_EQUAL);
	    } else {
		TBL_CRITERIA_LOAD_BYTE(criteria[0], "IntUID", uid, TBL_STRING, TBL_EQUAL);
	    }
	}
    }
    (void) memset(&interpretation, 0, sizeof(interpretation));
    interpretation.Type = FIS_K_INTERPRETATION;
    cond = TBL_Select(&h->interpretationHandle, criteria, fields, count,
		      interpretationCallback, getList);
    if (cond != TBL_NORMAL)
	return FIS_GETFAILED;

    if (r != NULL)
	*r = interpretation;
    return FIS_NORMAL;
}

static CONDITION
interpretationCallback(TBL_FIELD * field, int count, LST_HEAD * l)
{
    FIS_INTERPRETATIONRECORD *r;
    CONDITION cond;
    int i;
    static FLAG_MAP map[] = {
	{"IntUID", FIS_K_INTERP_INTUID},
	{"ResUID", FIS_K_INTERP_RESUID},
	{"IntID", FIS_K_INTERP_INTID},
	{"IntTex", FIS_K_INTERP_INTTEX},
	{"IntDiaDes", FIS_K_INTERP_INTDIADES},
	{"IntTypID", FIS_K_INTERP_INTTYPID},
	{"IntStaID", FIS_K_INTERP_INTSTAID},
	{"InsCreDat", FIS_K_INTERP_INSCREDAT},
	{"InsCreTim", FIS_K_INTERP_INSCRETIM},
	{"InsCreUID", FIS_K_INTERP_INSCREUID},
	{"PhyAppInt", FIS_K_INTERP_PHYAPPINT},
	{"IntAppDat", FIS_K_INTERP_INTAPPDAT},
	{"IntAppTim", FIS_K_INTERP_INTAPPTIM}
    };

    interpretation.Flag = 0;
    while (field->FieldName != NULL) {
	for (i = 0; i < (int) DIM_OF(map); i++) {
	    if (strcmp(field->FieldName, map[i].FieldName) == 0) {
		if (!field->Value.IsNull)
		    interpretation.Flag |= map[i].Flag;
		break;
	    }
	}
	field++;
    }

    if (l != NULL) {
	r = malloc(sizeof(*r));
	if (r == NULL)
	    return TBL_EARLYEXIT;

	*r = interpretation;
	if (LST_Enqueue(&l, r) != LST_NORMAL)
	    return TBL_EARLYEXIT;
    }
    return TBL_NORMAL;
}


static FIS_COMMITSOPREFRECORD commitSOPRef;

static CONDITION
commitSOPRefCallback(TBL_FIELD * field, int count, LST_HEAD * l)
{
    FIS_COMMITSOPREFRECORD *r;
    CONDITION cond;
    int i;
    static FLAG_MAP map[] = {
	{"TraUID", FIS_K_COMMITSOP_TRAUID},
	{"SOPClaUID", FIS_K_COMMITSOP_SOPCLAUID},
	{"SOPInsUID", FIS_K_COMMITSOP_SOPINSUID}
    };

    commitSOPRef.Flag = 0;
    while (field->FieldName != NULL) {
	for (i = 0; i < (int) DIM_OF(map); i++) {
	    if (strcmp(field->FieldName, map[i].FieldName) == 0) {
		if (!field->Value.IsNull)
		    commitSOPRef.Flag |= map[i].Flag;
		break;
	    }
	}
	field++;
    }

    if (l != NULL) {
	r = malloc(sizeof(*r));
	if (r == NULL)
	    return TBL_EARLYEXIT;

	*r = commitSOPRef;
	if (LST_Enqueue(&l, r) != LST_NORMAL)
	    return TBL_EARLYEXIT;
    }
    return TBL_NORMAL;
}

static CONDITION
getCommitSOPRecord(PRIVATE_HANDLE * h,
   FIS_DATA_TYPE criteriaType, char *uid, long listFlag, LST_HEAD * getList,
		   FIS_COMMITSOPREFRECORD * r, long *count)
{
    CONDITION
    cond;
    TBL_CRITERIA
	criteria[2];

    static TBL_FIELD fields[] = {
	{"TraUID", TBL_STRING, sizeof(commitSOPRef.TraUID),
	sizeof(commitSOPRef.TraUID), 0, (void *) &commitSOPRef.TraUID[0]},
	{"SOPClaUID", TBL_STRING, sizeof(commitSOPRef.SOPClaUID),
	sizeof(commitSOPRef.SOPClaUID), 0, (void *) &commitSOPRef.SOPClaUID[0]},
	{"SOPInsUID", TBL_STRING, sizeof(commitSOPRef.SOPInsUID),
	sizeof(commitSOPRef.SOPInsUID), 0, (void *) &commitSOPRef.SOPInsUID[0]},

	{NULL}
    };

    criteria[0].FieldName = criteria[1].FieldName = NULL;
    if (uid != NULL) {
	if (uid[0] != '\0') {
	    if (criteriaType == FIS_K_STORAGECOMMITREQ) {
		TBL_CRITERIA_LOAD_BYTE(criteria[0], "TraUID", uid, TBL_STRING, TBL_EQUAL);
	    }
	}
    }
    (void) memset(&commitSOPRef, 0, sizeof(commitSOPRef));
    commitSOPRef.Type = FIS_K_COMMITSOPREF;
    cond = TBL_Select(&h->commitSOPRefHandle, criteria, fields, count,
		      commitSOPRefCallback, getList);
    if (cond != TBL_NORMAL)
	return FIS_GETFAILED;

    if (r != NULL)
	*r = commitSOPRef;

    return FIS_NORMAL;
}


static FIS_STORAGECOMMITREQRECORD storageCommitRequest;
static S32 commitRequestDate;
static float commitRequestTime;
static S32 commitResponseDate;
static float commitResponseTime;


static CONDITION
storageCommitCallback(TBL_FIELD * field, int count, LST_HEAD * l)
{
    FIS_STORAGECOMMITREQRECORD *r;
    CONDITION cond;
    int i;
    static FLAG_MAP map[] = {
	{"TraUID", FIS_K_SCOMMIT_TRAUID},
	{"ReqAE", FIS_K_SCOMMIT_REQAE},
	{"ResAE", FIS_K_SCOMMIT_RESAE},
	{"ReqDat", FIS_K_SCOMMIT_REQDAT},
	{"ReqTim", FIS_K_SCOMMIT_REQTIM},
	{"ResDat", FIS_K_SCOMMIT_RESDAT},
	{"ResTim", FIS_K_SCOMMIT_RESTIM}
    };

    storageCommitRequest.Flag = 0;
    while (field->FieldName != NULL) {
	for (i = 0; i < (int) DIM_OF(map); i++) {
	    if (strcmp(field->FieldName, map[i].FieldName) == 0) {
		if (!field->Value.IsNull)
		    storageCommitRequest.Flag |= map[i].Flag;
		break;
	    }
	}
	field++;
    }

    if (storageCommitRequest.Flag & FIS_K_SCOMMIT_REQDAT)
	UTL_ConvertLongtoDate((long) commitRequestDate,
			      storageCommitRequest.ReqDat);
    if (storageCommitRequest.Flag & FIS_K_SCOMMIT_REQDAT)
	UTL_ConvertLongtoDate((long) commitResponseDate,
			      storageCommitRequest.ResDat);
    if (storageCommitRequest.Flag & FIS_K_SCOMMIT_REQTIM)
	UTL_ConvertFloattoTime(commitRequestTime,
			       storageCommitRequest.ReqTim);
    if (storageCommitRequest.Flag & FIS_K_SCOMMIT_RESTIM)
	UTL_ConvertFloattoTime(commitResponseTime,
			       storageCommitRequest.ResTim);


    if (l != NULL) {
	r = malloc(sizeof(*r));
	if (r == NULL)
	    return TBL_EARLYEXIT;

	*r = storageCommitRequest;
	if (LST_Enqueue(&l, r) != LST_NORMAL)
	    return TBL_EARLYEXIT;
    }
    return TBL_NORMAL;
}

static CONDITION
getStorageCommitReq(PRIVATE_HANDLE * h,
   FIS_DATA_TYPE criteriaType, char *uid, long listFlag, LST_HEAD * getList,
		    FIS_STORAGECOMMITREQRECORD * r, long *count)
{
    CONDITION
    cond;
    TBL_CRITERIA
	criteria[2];

    static TBL_FIELD fields[] = {
	{"TraUID", TBL_STRING, sizeof(storageCommitRequest.TraUID),
	    sizeof(storageCommitRequest.TraUID), 0,
	(void *) &storageCommitRequest.TraUID[0]},
	{"ReqAE", TBL_STRING, sizeof(storageCommitRequest.ReqAE),
	    sizeof(storageCommitRequest.ReqAE), 0,
	(void *) &storageCommitRequest.ReqAE[0]},
	{"ResAE", TBL_STRING, sizeof(storageCommitRequest.ResAE),
	    sizeof(storageCommitRequest.TraUID), 0,
	(void *) &storageCommitRequest.ResAE[0]},
	{"ReqDat", TBL_SIGNED4, sizeof(commitRequestDate),
	    sizeof(commitRequestDate), 0,
	(void *) &commitRequestDate},
	{"ReqTim", TBL_FLOAT4, sizeof(commitRequestTime),
	    sizeof(commitRequestTime), 0,
	(void *) &commitRequestTime},
	{"ResDat", TBL_SIGNED4, sizeof(commitResponseDate),
	    sizeof(commitResponseDate), 0,
	(void *) &commitResponseDate},
	{"ResTim", TBL_FLOAT4, sizeof(commitResponseTime),
	    sizeof(commitResponseTime), 0,
	(void *) &commitResponseTime},

	{NULL}
    };

    criteria[0].FieldName = criteria[1].FieldName = NULL;
    if (uid != NULL && uid[0] != '\0') {
	if (criteriaType == FIS_K_STORAGECOMMITREQ) {
	    TBL_CRITERIA_LOAD_BYTE(criteria[0], "TraUID", uid, TBL_STRING, TBL_EQUAL);
	}
    }
    (void) memset(&storageCommitRequest, 0, sizeof(storageCommitRequest));
    commitRequestDate = commitResponseDate = 0;
    commitRequestTime = commitResponseTime = 0.;

    storageCommitRequest.Type = FIS_K_STORAGECOMMITREQ;
    cond = TBL_Select(&h->storageCommitReqHandle, criteria, fields, count,
		      storageCommitCallback, getList);
    if (cond != TBL_NORMAL)
	return FIS_GETFAILED;

    if (r != NULL)
	*r = storageCommitRequest;

    return FIS_NORMAL;
}


static FIS_WORKQUEUERECORD workQueue;

static CONDITION
workQueueCallback(TBL_FIELD * field, int count, LST_HEAD * l)
{
    FIS_WORKQUEUERECORD *r;
    CONDITION cond;
    int i;
    static FLAG_MAP map[] = {
	{"TasNam", FIS_K_WORKQUEUE_TASNAM},
	{"TasID", FIS_K_WORKQUEUE_TASID}
    };

    workQueue.Flag = 0;
    while (field->FieldName != NULL) {
	for (i = 0; i < (int) DIM_OF(map); i++) {
	    if (strcmp(field->FieldName, map[i].FieldName) == 0) {
		if (!field->Value.IsNull)
		    workQueue.Flag |= map[i].Flag;
		break;
	    }
	}
	field++;
    }

    if (l != NULL) {
	r = malloc(sizeof(*r));
	if (r == NULL)
	    return TBL_EARLYEXIT;

	*r = workQueue;
	if (LST_Enqueue(&l, r) != LST_NORMAL)
	    return TBL_EARLYEXIT;
    }
    return TBL_NORMAL;
}

static CONDITION
getWorkQueueRecord(PRIVATE_HANDLE * h,
		   char *taskName, char *taskID,
		   long listFlag, LST_HEAD * getList,
		   FIS_WORKQUEUERECORD * r, long *count)
{
    CONDITION
    cond;
    TBL_CRITERIA
	criteria[3];
    int i = 0;

    static TBL_FIELD fields[] = {
	{"TasNam", TBL_STRING, sizeof(workQueue.TasNam),
	sizeof(workQueue.TasNam), 0, (void *) &workQueue.TasNam[0]},
	{"TasID", TBL_STRING, sizeof(workQueue.TasID),
	sizeof(workQueue.TasID), 0, (void *) &workQueue.TasID[0]},
	{NULL}
    };

    if (taskName != NULL && taskName[0] != '\0') {
	TBL_CRITERIA_LOAD_BYTE(criteria[i], "TasNam", taskName, TBL_STRING, TBL_EQUAL);
	i++;
    }
    if (taskID != NULL && taskID[0] != '\0') {
	TBL_CRITERIA_LOAD_BYTE(criteria[i], "TasID", taskID, TBL_STRING, TBL_EQUAL);
	i++;
    }
    criteria[i].FieldName = NULL;

    (void) memset(&workQueue, 0, sizeof(workQueue));
    workQueue.Type = FIS_K_WORKQUEUE;
    cond = TBL_Select(&h->workQueueHandle, criteria, fields, count,
		      workQueueCallback, getList);
    if (cond != TBL_NORMAL)
	return FIS_GETFAILED;

    if (r != NULL)
	*r = workQueue;

    return FIS_NORMAL;
}


void
FIS_ClearList(LST_HEAD * l)
{
    LST_NODE
    * n;

    while ((n = LST_Dequeue(&l)) != NULL)
	free(n);
}

/* FIS_Get
**
** Purpose:
**
** Parameter Dictionary:
**
** Return Values:
**	FIS_NORMAL
**	FIS_GETFAILED
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
FIS_Get(FIS_HANDLE ** handle, FIS_DATA_TYPE type,
   FIS_DATA_TYPE criteriaType, char *uid, long listFlag, LST_HEAD * getList)
{
    CONDITION
	cond = FIS_NORMAL;
    PRIVATE_HANDLE
	* h;

    h = *(PRIVATE_HANDLE **) handle;
    FIS_ClearList(getList);

    switch (type) {
    case FIS_K_PATIENT:
	cond = getPatientRecord(h, criteriaType, uid, listFlag, getList, NULL, NULL);
	break;
    case FIS_K_VISIT:
	cond = getVisitRecord(h, criteriaType, uid, listFlag, getList, NULL, NULL);
	break;
    case FIS_K_STUDY:
	cond = getStudyRecord(h, criteriaType, uid, listFlag, getList, NULL, NULL);
	break;
    case FIS_K_STUDYCOMPONENT:
	cond = getStudyComponentRecord(h, criteriaType, uid, listFlag, getList, NULL, NULL);
	break;
    case FIS_K_SCSERIES:
	cond = getSCSeriesRecord(h, criteriaType, uid, listFlag, getList, NULL, NULL);
	break;
    case FIS_K_SCIMAGE:
	cond = getSCImageRecord(h, criteriaType, uid, listFlag, getList, NULL, NULL);
	break;
    case FIS_K_RESULTS:
	cond = getResultsRecord(h, criteriaType, uid, listFlag, getList, NULL, NULL);
	break;
    case FIS_K_INTERPRETATION:
	cond = getInterpretationRecord(h, criteriaType, uid, listFlag, getList, NULL, NULL);
	break;
    case FIS_K_STORAGECOMMITREQ:
	cond = getStorageCommitReq(h, criteriaType, uid, listFlag, getList, NULL, NULL);
	break;
    case FIS_K_COMMITSOPREF:
	cond = getCommitSOPRecord(h, criteriaType, uid, listFlag, getList,
				  NULL, NULL);
	break;
    }
    if (cond != FIS_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_GETFAILED),
				  "FIS_Get");
    return cond;
}

/* FIS_GetOne
**
** Purpose:
**
** Parameter Dictionary:
**
** Return Values:
**	FIS_NORMAL
**	FIS_GETFAILED
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
FIS_GetOne(FIS_HANDLE ** handle, FIS_DATA_TYPE type,
	FIS_DATA_TYPE criteriaType, const char *uid, long listFlag, void *r)
{
    CONDITION cond = FIS_NORMAL;
    PRIVATE_HANDLE *h;
    long count = 0;
    char localUID[DICOM_UI_LENGTH + 1];
    char *uidPtr = NULL;

    h = *(PRIVATE_HANDLE **) handle;
    if (uid != NULL) {
	strcpy(localUID, uid);
	uidPtr = localUID;
    }
    switch (type) {
    case FIS_K_PATIENT:
	cond = getPatientRecord(h, criteriaType, uidPtr, listFlag, NULL,
				(FIS_PATIENTRECORD *) r, &count);
	break;
    case FIS_K_VISIT:
	cond = getVisitRecord(h, criteriaType, uidPtr, listFlag, NULL,
			      (FIS_VISITRECORD *) r, &count);
	break;
    case FIS_K_STUDY:
	cond = getStudyRecord(h, criteriaType, uidPtr, listFlag, NULL,
			      (FIS_STUDYRECORD *) r, &count);
	break;
    case FIS_K_STUDYCOMPONENT:
	cond = getStudyComponentRecord(h, criteriaType, uidPtr, listFlag, NULL,
				    (FIS_STUDYCOMPONENTRECORD *) r, &count);
	break;
    case FIS_K_SCSERIES:
	cond = getSCSeriesRecord(h, criteriaType, uidPtr, listFlag, NULL,
				 (FIS_SCSERIESRECORD *) r, &count);
	break;
    case FIS_K_SCIMAGE:
	cond = getSCImageRecord(h, criteriaType, uidPtr, listFlag, NULL,
				(FIS_SCIMAGERECORD *) r, &count);
	break;
    case FIS_K_RESULTS:
	cond = getResultsRecord(h, criteriaType, uidPtr, listFlag, NULL,
				(FIS_RESULTSRECORD *) r, &count);
	break;
    case FIS_K_INTERPRETATION:
	cond = getInterpretationRecord(h, criteriaType, uidPtr, listFlag, NULL,
				    (FIS_INTERPRETATIONRECORD *) r, &count);
	break;
    case FIS_K_COMMITSOPREF:
	cond = getCommitSOPRecord(h, criteriaType, uidPtr, listFlag, NULL,
				  (FIS_COMMITSOPREFRECORD *) r, &count);
	break;
    case FIS_K_STORAGECOMMITREQ:
	cond = getStorageCommitReq(h, criteriaType, uidPtr, listFlag, NULL,
				   (FIS_STORAGECOMMITREQRECORD *) r, &count);
	break;
    }
    if (cond != FIS_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_GETFAILED),
				  "FIS_GetOne");
    if (count != 1)
	return COND_PushCondition(FIS_COND(FIS_SINGLEGETFAILED),
				  localUID, "FIS_GetOne");
    return cond;
}


CONDITION
FIS_GetWorkQueue(FIS_HANDLE ** handle, char *taskName, char *taskID,
		 long listFlag, LST_HEAD * getList)
{
    CONDITION
	cond = FIS_NORMAL;
    PRIVATE_HANDLE
	* h;

    h = *(PRIVATE_HANDLE **) handle;
    FIS_ClearList(getList);


    return getWorkQueueRecord(h, taskName, taskID, listFlag, getList,
			      NULL, NULL);
}

/* FIS_GetOne
**
** Purpose:
**
** Parameter Dictionary:
**
** Return Values:
**	FIS_NORMAL
**	FIS_GETFAILED
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
FIS_SelectOne(FIS_HANDLE ** handle, FIS_DATA_TYPE type,
	      FIS_CRITERIA_TYPE criteriaType, const char *criteriaString,
	      long listFlag, void *r)
{
    CONDITION cond = FIS_NORMAL;
    PRIVATE_HANDLE *h;
    long count = 0;

    h = *(PRIVATE_HANDLE **) handle;

    switch (type) {
    case FIS_K_PATIENT:
	cond = 0;
	break;
    case FIS_K_VISIT:
	cond = 0;
	break;
    case FIS_K_STUDY:
	cond = selectStudyRecord(h, criteriaType, criteriaString, listFlag, NULL,
				 (FIS_STUDYRECORD *) r, &count);
	break;
    case FIS_K_STUDYCOMPONENT:
	cond = 0;
	break;
    case FIS_K_SCSERIES:
	cond = 0;
	break;
    case FIS_K_SCIMAGE:
	cond = 0;
	break;
    case FIS_K_RESULTS:
	cond = 0;
	break;
    case FIS_K_INTERPRETATION:
	cond = 0;
	break;
    case FIS_K_COMMITSOPREF:
	cond = 0;
	break;
    case FIS_K_STORAGECOMMITREQ:
	cond = 0;
	break;
    }
    if (cond != FIS_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_GETFAILED),
				  "FIS_SelectOne");
    if (count != 1)
	return COND_PushCondition(FIS_COND(FIS_SINGLEGETFAILED),
				  criteriaString, "FIS_SelectOne");
    return cond;
}
