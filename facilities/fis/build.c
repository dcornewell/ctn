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
** Module Name(s):	FIS_BuildObject
**			FIS_ParseObject
** Author, Date:	Stephen Moore, 22-Jul-94
** Intent:		This file contains functions for
**			information systems (management classes).
** Last Update:		$Author: smm $, $Date: 2000-11-03 17:03:38 $
** Source File:		$RCSfile: build.c,v $
** Revision:		$Revision: 1.15 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.15 $ $RCSfile: build.c,v $";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "dicom_objects.h"
#include "dicom_sq.h"
#include "tbl.h"
#include "manage.h"
#include "fis.h"
#include "dicom_uids.h"
#include "utility.h"

#include "fis_private.h"

static CONDITION
    buildPatient(DCM_OBJECT ** obj, FIS_PATIENTRECORD * p, FIS_EVENT event);
static CONDITION
    buildVisit(DCM_OBJECT ** o, FIS_VISITRECORD * v, FIS_EVENT e);
static CONDITION
    buildStudy(DCM_OBJECT ** obj, FIS_STUDYRECORD * s, FIS_EVENT event);
static CONDITION
    buildStudyComponent(DCM_OBJECT ** o, FIS_STUDYCOMPONENTRECORD * s, FIS_EVENT e);
static CONDITION
    buildResults(DCM_OBJECT ** o, FIS_RESULTSRECORD * r, FIS_EVENT e);
static CONDITION
    buildInterpretation(DCM_OBJECT ** o, FIS_INTERPRETATIONRECORD * r, FIS_EVENT e);
static CONDITION addRefPatientUID(DCM_OBJECT ** obj, char *patientUID);
static CONDITION addRefVisitUID(DCM_OBJECT ** obj, char *visitUID);
static CONDITION addRefStudyUID(DCM_OBJECT ** obj, char *studyUID);
static CONDITION addRefSCList(DCM_OBJECT ** obj, LST_HEAD * scList);
static CONDITION
    addRefResultsUID(DCM_OBJECT ** obj, char *resultsUID);
static CONDITION
addProcedureCodeSeq(DCM_OBJECT ** obj, char *codeValue, char *codingScheme,
		    char *codeMeaning);

static CONDITION
    addRefSeriesList(DCM_OBJECT ** obj, LST_HEAD * seriesList);
static CONDITION addRefImageList(LST_HEAD * imagesFIS, LST_HEAD * imagesSQ);
static CONDITION
    addRefResultsList(DCM_OBJECT ** obj, LST_HEAD * resultsList);
static CONDITION
    addRefInterpList(DCM_OBJECT ** obj, LST_HEAD * interpList);

static CONDITION
    parsePatient(DCM_OBJECT ** obj, FIS_PATIENTRECORD * s);
static CONDITION
    parseStudy(DCM_OBJECT ** obj, FIS_STUDYRECORD * s);
static CONDITION
    parseStudyComponent(DCM_OBJECT ** obj, FIS_STUDYCOMPONENTRECORD * s);
static CONDITION
    parseResults(DCM_OBJECT ** obj, FIS_RESULTSRECORD * s);
static CONDITION
    parseInterpretation(DCM_OBJECT ** obj, FIS_INTERPRETATIONRECORD * s);
static void clearLlst(LST_HEAD ** l);
static CONDITION
    parseSeq(DCM_OBJECT ** obj, DCM_TAG tag, SQ_TYPE * type, LST_HEAD ** l);

static CONDITION
addInterpretationApprover(DCM_OBJECT ** obj, FIS_INTERPRETATIONRECORD * record)
{
    CONDITION cond;
    LST_HEAD *l;
    SQ_REQINTERPRETATIONAPPROVER *approver;
    DCM_ELEMENT *e;

    l = LST_Create();
    if (l == NULL) {
	return 0;
    }
    approver = malloc(sizeof(*approver));
    if (approver == NULL) {
	(void) LST_Destroy(&l);
	return 0;
    }
    approver->type = SQ_K_REQINTERPRETATIONAPPROVER;
    strcpy(approver->approvalDates, record->IntAppDat);
    strcpy(approver->approvalTimes, record->IntAppTim);
    strcpy(approver->physiciansApproving, record->PhyAppInt);

    if (LST_Enqueue(&l, approver) != LST_NORMAL) {
	(void) LST_Destroy(l);
	free(approver);
	return 0;
    }
    cond = SQ_BuildSequence(&l, SQ_K_REQINTERPRETATIONAPPROVER, &e);
    if (cond != SQ_NORMAL) {
	(void) LST_Destroy(l);
	free(approver);
	return 0;
    }
    cond = DCM_AddElement(obj, e);
    if (cond != DCM_NORMAL) {
	(void) LST_Destroy(l);
	return 0;
    }
    return FIS_NORMAL;
}

static CONDITION
addDiagnosisCodes(DCM_OBJECT ** obj, FIS_INTERPRETATIONRECORD * record)
{
    CONDITION cond;
    LST_HEAD *l;
    DCM_ELEMENT *e;

    l = LST_Create();
    if (l == NULL) {
	return 0;
    }
    cond = SQ_BuildSequence(&l, SQ_K_REQDIAGNOSISCODE, &e);
    if (cond != SQ_NORMAL) {
	(void) LST_Destroy(l);
	return 0;
    }
    cond = DCM_AddElement(obj, e);
    if (cond != DCM_NORMAL) {
	(void) LST_Destroy(l);
	return 0;
    }
    return FIS_NORMAL;
}


/* FIS_BuildObject
**
** Purpose:
**	Describe the purpose of the function
**
** Parameter Dictionary:
**	Define the parameters to the function
**
** Return Values:
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
FIS_BuildObject(DCM_OBJECT ** obj, FIS_DATA_TYPE type, void *d,
		FIS_EVENT event)
{
    CONDITION cond = FIS_NORMAL;
    FIS_PATIENTRECORD *p;

    p = (FIS_PATIENTRECORD *) d;
    if (p->Type != type)
	return COND_PushCondition(FIS_COND(FIS_ILLEGALRECORDTYPE),
				  "", "FIS_BuildObject");

    switch (type) {
    case FIS_K_PATIENT:
	cond = buildPatient(obj, (FIS_PATIENTRECORD *) d, event);
	break;
    case FIS_K_VISIT:
	cond = buildVisit(obj, (FIS_VISITRECORD *) d, event);
	break;
    case FIS_K_STUDY:
	cond = buildStudy(obj, (FIS_STUDYRECORD *) d, event);
	break;
    case FIS_K_STUDYCOMPONENT:
	cond = buildStudyComponent(obj, (FIS_STUDYCOMPONENTRECORD *) d, event);
	break;
    case FIS_K_RESULTS:
	cond = buildResults(obj, (FIS_RESULTSRECORD *) d, event);
	break;
    case FIS_K_INTERPRETATION:
	cond = buildInterpretation(obj, (FIS_INTERPRETATIONRECORD *) d, event);
	break;
    }
    if (cond != FIS_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_BUILDFAILED),
				  "", "FIS_BuildObject");
    else
	return FIS_NORMAL;
}

CONDITION
FIS_ParseObject(DCM_OBJECT ** obj, FIS_DATA_TYPE type, void *d)
{
    CONDITION
	cond = FIS_NORMAL;

    switch (type) {
    case FIS_K_PATIENT:
	cond = parsePatient(obj, (FIS_PATIENTRECORD *) d);
	break;
    case FIS_K_VISIT:
	break;
    case FIS_K_STUDY:
	cond = parseStudy(obj, (FIS_STUDYRECORD *) d);
	break;
    case FIS_K_STUDYCOMPONENT:
	cond = parseStudyComponent(obj, (FIS_STUDYCOMPONENTRECORD *) d);
	break;
    case FIS_K_RESULTS:
	cond = parseResults(obj, (FIS_RESULTSRECORD *) d);
	break;
    case FIS_K_INTERPRETATION:
	cond = parseInterpretation(obj, (FIS_INTERPRETATIONRECORD *) d);
	break;
    }
    return cond;
}


typedef struct {
    FIS_EVENT Event;
    long Flag;
}   EVENT_MAP;

static FIS_STUDYRECORD
    study;
static DCM_FLAGGED_ELEMENT studyMap[] = {
    {DCM_SDYSCHEDULEDSTARTDATE, DCM_DA, "", 1, sizeof(study.SchStuStaTim),
    (void *) &study.SchStuStaDat[0], FIS_K_STU_SCHSTUSTADAT, &study.Flag},
    {DCM_SDYSCHEDULEDSTARTTIME, DCM_TM, "", 1, sizeof(study.SchStuStaTim),
    (void *) &study.SchStuStaTim[0], FIS_K_STU_SCHSTUSTATIM, &study.Flag},
    {DCM_SDYSCHEDULEDLOCATION, DCM_LO, "", 1, sizeof(study.SchStuLoc),
    (void *) &study.SchStuLoc[0], FIS_K_STU_SCHSTULOC, &study.Flag},
    {DCM_SDYSCHEDULEDLOCATIONAETITLE, DCM_AE, "", 1, sizeof(study.SchStuLocAE),
    (void *) &study.SchStuLocAE[0], FIS_K_STU_SCHSTULOCAE, &study.Flag},
    {DCM_SDYREQUESTEDPRODESCRIPTION, DCM_LO, "", 1, sizeof(study.ProDes),
    (void *) &study.ProDes[0], FIS_K_STU_PRODES, &study.Flag},
    {DCM_SDYSTATUSID, DCM_CS, "", 1, sizeof(study.StuStaID),
    (void *) &study.StuStaID[0], FIS_K_STU_STUSTAID, &study.Flag},
    {DCM_RELSTUDYID, DCM_SH, "", 1, sizeof(study.StuID),
    (void *) &study.StuID[0], FIS_K_STU_STUID, &study.Flag},
    {DCM_IDACCESSIONNUMBER, DCM_SH, "", 1, sizeof(study.AccNum),
    (void *) &study.AccNum[0], FIS_K_STU_ACCNUM, &study.Flag},
    {DCM_SDYREADDATE, DCM_DA, "", 1, sizeof(study.StuReaDat),
    (void *) &study.StuReaDat[0], FIS_K_STU_STUREADAT, &study.Flag},
    {DCM_SDYREADTIME, DCM_TM, "", 1, sizeof(study.StuReaTim),
    (void *) &study.StuReaTim[0], FIS_K_STU_STUREATIM, &study.Flag},
};

#define	STUDY_SCHEDULED_FLAGS	(FIS_K_STU_PATUID | \
				 FIS_K_STU_VISUID | \
				FIS_K_STU_SCHSTUSTADAT | \
				FIS_K_STU_SCHSTUSTATIM | \
				FIS_K_STU_SCHSTULOC | \
				FIS_K_STU_SCHSTULOCAE | \
				FIS_K_STU_REQPRO )

#define	STUDY_READ_FLAGS	(FIS_K_STU_STUREADAT | \
				FIS_K_STU_STUREATIM )

static EVENT_MAP studyEventMap[] = {
    {FIS_K_STUDY_CREATED, 0},
    {FIS_K_STUDY_SCHEDULED, STUDY_SCHEDULED_FLAGS},
    {FIS_K_STUDY_READ, STUDY_READ_FLAGS},
};


static CONDITION
buildStudy(DCM_OBJECT ** obj, FIS_STUDYRECORD * s,
	   FIS_EVENT event)
{
    CONDITION
    cond;
    int
        i;

    study = *s;
    if (event != FIS_K_NO_EVENT) {
	for (i = 0; i < (int) DIM_OF(studyEventMap); i++) {
	    if (studyEventMap[i].Event == event)
		study.Flag = studyEventMap[i].Flag;
	}
    }
    cond = DCM_ModifyElements(obj, NULL, 0, studyMap, (int) DIM_OF(studyMap),
			      NULL);
    if (cond != DCM_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_BUILDFAILED),
				  "study", "(FIS)buildStudy");

    if (study.Flag & FIS_K_STU_PATUID) {
	cond = addRefPatientUID(obj, s->PatUID);
	if (cond != FIS_NORMAL) {
	    (void) DCM_CloseObject(obj);
	    return COND_PushCondition(FIS_COND(FIS_BUILDFAILED),
				      "study", "(FIS)buildStudy");
	}
    }
    if (study.Flag & FIS_K_STU_VISUID) {
	cond = addRefVisitUID(obj, s->VisUID);
	if (cond != FIS_NORMAL) {
	    (void) DCM_CloseObject(obj);
	    return COND_PushCondition(FIS_COND(FIS_BUILDFAILED),
				      "study", "(FIS)buildStudy");
	}
    }
    if (study.Flag & FIS_K_STU_RESULTSLIST) {
	cond = addRefResultsList(obj, s->ResultsList);
	if (cond != FIS_NORMAL) {
	    (void) DCM_CloseObject(obj);
	    return COND_PushCondition(FIS_COND(FIS_BUILDFAILED),
				      "study", "(FIS)buildStudy");
	}
    }
    if (study.Flag & FIS_K_STU_STUDYCOMPONENTLIST) {
	cond = addRefSCList(obj, s->StudyComponentList);
	if (cond != FIS_NORMAL) {
	    (void) DCM_CloseObject(obj);
	    return COND_PushCondition(FIS_COND(FIS_BUILDFAILED),
				      "study", "(FIS)buildStudy");
	}
    }
    return FIS_NORMAL;
}

static CONDITION
parseStudy(DCM_OBJECT ** obj, FIS_STUDYRECORD * s)
{
    CONDITION cond;
    LST_HEAD *l;
    SQ_REFPATIENTSOPINSTANCEUID *patientSQ;
    SQ_REFRESULTSSOPINSTANCEUID *resultsSQ;
    SQ_REFSTUDYCOMPONENT *compSQ;
    SQ_TYPE sqType;
    FIS_RESULTSRECORD *results;
    FIS_STUDYCOMPONENTRECORD *studyComponent;

    memset(&study, 0, sizeof(study));
    study.Flag = 0;
    cond = DCM_ParseObject(obj, NULL, 0, studyMap, (int) DIM_OF(studyMap), NULL);
    if (cond != DCM_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
				  "study", "(FIS)parseStudy");

    *s = study;
    s->Type = FIS_K_STUDY;

    l = LST_Create();
    if (l == NULL)
	return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
				  "study", "(FIS)parseStudy");

    cond = parseSeq(obj, DCM_IDREFERENCEDPATIENTSEQ, &sqType, &l);
    if (cond == FIS_NORMAL) {
	if (sqType != SQ_K_REFPATIENTSOPINSTANCEUID)
	    return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
				      "study", "(FIS)parseStudy");
	patientSQ = LST_Head(&l);
	if (patientSQ == NULL)
	    return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
				      "study", "(FIS)parseStudy");
	strcpy(s->PatUID, patientSQ->referencedSOPInstanceUID);
	s->Flag |= FIS_K_STU_PATUID;
	clearLlst(&l);
    } else
	(void) COND_PopCondition(TRUE);

    cond = parseSeq(obj, DCM_IDREFERENCEDRESULTSSEQ, &sqType, &l);
    if (cond == FIS_NORMAL) {
	if (sqType != SQ_K_REFRESULTSSOPINSTANCEUID)
	    return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
				      "study", "(FIS)parseStudy");
	s->ResultsList = LST_Create();
	if (s->ResultsList == NULL)
	    return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
				      "study", "(FIS)parseStudy");
	while ((resultsSQ = LST_Dequeue(&l)) != NULL) {
	    if ((results = calloc(1, sizeof(*results))) == NULL)
		return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
					  "study", "(FIS)parseStudy");
	    results->Type = FIS_K_RESULTS;
	    results->Flag = FIS_K_RESULTS_RESUID;
	    strcpy(results->ResUID, resultsSQ->referencedSOPInstanceUID);
	    if ((LST_Enqueue(&s->ResultsList, results)) != LST_NORMAL)
		return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
					  "study", "(FIS)parseStudy");
	}
	s->Flag |= FIS_K_STU_RESULTSLIST;
    } else
	(void) COND_PopCondition(TRUE);

    cond = parseSeq(obj, DCM_IDREFERENCEDSTUDYCOMPONENTSEQ, &sqType, &l);
    if (cond == FIS_NORMAL) {
	if (sqType != SQ_K_REFSTUDYCOMPONENT)
	    return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
				      "study", "(FIS)parseStudy");
	s->StudyComponentList = LST_Create();
	if (s->StudyComponentList == NULL)
	    return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
				      "study", "(FIS)parseStudy");
	while ((compSQ = LST_Dequeue(&l)) != NULL) {
	    if ((studyComponent = calloc(1, sizeof(*studyComponent))) == NULL)
		return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
					  "study", "(FIS)parseStudy");
	    studyComponent->Type = FIS_K_STUDYCOMPONENT;
	    studyComponent->Flag = FIS_K_STUDYCOMP_STUCOMUID;
	    strcpy(studyComponent->StuComUID, compSQ->refSOPInstanceUID);
	    if ((LST_Enqueue(&s->StudyComponentList, studyComponent)) !=
		LST_NORMAL)
		return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
					  "study", "(FIS)parseStudy");
	}
	s->Flag |= FIS_K_STU_STUDYCOMPONENTLIST;
    } else
	(void) COND_PopCondition(TRUE);

    return FIS_NORMAL;
}



static FIS_PATIENTRECORD
    patient;
static DCM_FLAGGED_ELEMENT patientMap[] = {
    {DCM_PATID, DCM_LO, "", 1, sizeof(patient.PatID),
    (void *) &patient.PatID[0], FIS_K_PAT_PATID, &patient.Flag},
    {DCM_PATNAME, DCM_PN, "", 1, sizeof(patient.PatNam),
    (void *) &patient.PatNam[0], FIS_K_PAT_PATNAM, &patient.Flag},
    {DCM_PATBIRTHDATE, DCM_DA, "", 1, sizeof(patient.PatBirDat),
    (void *) &patient.PatBirDat[0], FIS_K_PAT_BIRDAT, &patient.Flag},
    {DCM_PATSEX, DCM_CS, "", 1, sizeof(patient.PatSex),
    (void *) &patient.PatSex[0], FIS_K_PAT_SEX, &patient.Flag},
};

#define	PATIENT_CREATED_FLAGS	(FIS_K_PAT_PATNAM | \
				 FIS_K_PAT_PATID | \
				FIS_K_PAT_BIRDAT| \
				FIS_K_PAT_SEX)

static EVENT_MAP patientEventMap[] = {
    {FIS_K_PATIENT_CREATED, PATIENT_CREATED_FLAGS},
};

static CONDITION
buildPatient(DCM_OBJECT ** obj, FIS_PATIENTRECORD * p, FIS_EVENT event)
{
    CONDITION
    cond;
    int
        i;

    patient = *p;
    if (event != FIS_K_NO_EVENT) {
	for (i = 0; i < (int) DIM_OF(patientEventMap); i++) {
	    if (patientEventMap[i].Event == event)
		patient.Flag = patientEventMap[i].Flag;
	}
    }
    cond = DCM_ModifyElements(obj, NULL, 0, patientMap, (int) DIM_OF(patientMap),
			      NULL);
    if (cond != DCM_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_BUILDFAILED),
				  "patient", "(FIS)buildPatient");

/*    if (study.Flag & FIS_K_STU_PATUID) {
	cond = addRefPatientUID(obj, s->PatUID);
	if (cond != FIS_NORMAL) {
	    (void) DCM_CloseObject(obj);
	    return COND_PushCondition(FIS_COND(FIS_BUILDFAILED),
				  "patient", "(FIS)buildPatient");
	}
    }
    if (study.Flag & FIS_K_STU_VISUID) {
	cond = addRefVisitUID(obj, s->VisUID);
	if (cond != FIS_NORMAL) {
	    (void) DCM_CloseObject(obj);
	    return COND_PushCondition(FIS_COND(FIS_BUILDFAILED),
				  "patient", "(FIS)buildPatient");
	}
    }
*/
    return FIS_NORMAL;
}

static CONDITION
parsePatient(DCM_OBJECT ** obj, FIS_PATIENTRECORD * p)
{
    CONDITION cond;

    memset(&patient, 0, sizeof(patient));
    patient.Flag = 0;
    cond = DCM_ParseObject(obj, NULL, 0, patientMap,
			   (int) DIM_OF(patientMap), NULL);
    if (cond != DCM_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
				  "patient", "(FIS)parsePatient");

    *p = patient;
    p->Type = FIS_K_PATIENT;
    return FIS_NORMAL;
}

/* Fill this in when we have something implemented for visits
static FIS_VISITRECORD
    visit;

static DCM_FLAGGED_ELEMENT visitMap[] = {
    {0, DCM_LO, "", 1, 0, NULL, 0, NULL}
};

#define	VISIT_CREATED_FLAGS	0

static EVENT_MAP visitEventMap[] = {
    {FIS_K_VISIT_CREATED, VISIT_CREATED_FLAGS},
};
*/

static CONDITION
buildVisit(DCM_OBJECT ** obj, FIS_VISITRECORD * v, FIS_EVENT event)
{

/*  Implement this code when we have something implemented for visit

    visit = *v;
    if (event != FIS_K_NO_EVENT) {
	for (i = 0; i < (int) DIM_OF(visitEventMap); i++) {
	    if (visitEventMap[i].Event == event)
		visit.Flag = visitEventMap[i].Flag;
	}
    }
    cond = DCM_ModifyElements(obj, NULL, 0, visitMap, (int) DIM_OF(visitMap),
			      NULL);
    if (cond != DCM_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_BUILDFAILED),
				  "visit", "(FIS)buildVisit");
*/

/*    if (study.Flag & FIS_K_STU_PATUID) {
	cond = addRefPatientUID(obj, s->PatUID);
	if (cond != FIS_NORMAL) {
	    (void) DCM_CloseObject(obj);
	    return COND_PushCondition(FIS_COND(FIS_BUILDFAILED),
				  "visit", "(FIS)buildVisit");
	}
    }
    if (study.Flag & FIS_K_STU_VISUID) {
	cond = addRefVisitUID(obj, s->VisUID);
	if (cond != FIS_NORMAL) {
	    (void) DCM_CloseObject(obj);
	    return COND_PushCondition(FIS_COND(FIS_BUILDFAILED),
				  "visit", "(FIS)buildVisit");
	}
    }
*/
    return FIS_NORMAL;
}

static FIS_STUDYCOMPONENTRECORD
    studyComponent;
static DCM_FLAGGED_ELEMENT studyComponentMap[] = {
    {DCM_IDMODALITY, DCM_CS, "", 1, sizeof(studyComponent.Mod),
    (void *) &studyComponent.Mod[0], FIS_K_STUDYCOMP_MOD, &studyComponent.Flag},
    {DCM_IDSTUDYDESCRIPTION, DCM_LO, "", 1, sizeof(studyComponent.StuDes),
    (void *) &studyComponent.StuDes[0], FIS_K_STUDYCOMP_STUDES, &studyComponent.Flag},
    {DCM_SDYSTUDYCOMPONENTSTATUSID, DCM_CS, "", 1,
	sizeof(studyComponent.StuComStaID), (void *) &studyComponent.StuComStaID[0],
    FIS_K_STUDYCOMP_STUCOMSTAID, &studyComponent.Flag},
    {DCM_RELSTUDYID, DCM_SH, "", 1,
	sizeof(studyComponent.StuID), (void *) &studyComponent.StuID[0],
    FIS_K_STUDYCOMP_STUID, &studyComponent.Flag},
};

static CONDITION
buildStudyComponent(DCM_OBJECT ** obj, FIS_STUDYCOMPONENTRECORD * s,
		    FIS_EVENT event)
{
    CONDITION cond;

    studyComponent = *s;
    cond = DCM_ModifyElements(obj, NULL, 0, studyComponentMap,
			      (int) DIM_OF(studyComponentMap), NULL);
    if (cond != DCM_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_BUILDFAILED),
			     "study component", "(FIS)buildStudyComponent");

    if (studyComponent.Flag & FIS_K_STUDYCOMP_STUINSUID) {
	cond = addRefStudyUID(obj, s->StuInsUID);
	if (cond != FIS_NORMAL) {
	    (void) DCM_CloseObject(obj);
	    return COND_PushCondition(FIS_COND(FIS_BUILDFAILED),
			     "study component", "(FIS)buildStudyComponent");
	}
    }
    if (studyComponent.Flag & FIS_K_STUDYCOMP_PROCODVAL) {
	cond = addProcedureCodeSeq(obj, s->ProCodVal, s->ProCodSchDes,
				   s->ProCodMea);
	if (cond != FIS_NORMAL) {
	    (void) DCM_CloseObject(obj);
	    return COND_PushCondition(FIS_COND(FIS_BUILDFAILED),
			     "study component", "(FIS)buildStudyComponent");
	}
    }
    if (studyComponent.Flag & FIS_K_STUDYCOMP_SERIESLIST) {
	cond = addRefSeriesList(obj, s->SeriesList);
	if (cond != FIS_NORMAL) {
	    (void) DCM_CloseObject(obj);
	    return COND_PushCondition(FIS_COND(FIS_BUILDFAILED),
			     "study component", "(FIS)buildStudyComponent");
	}
    }
    return FIS_NORMAL;
}

static CONDITION
parseStudyComponent(DCM_OBJECT ** obj, FIS_STUDYCOMPONENTRECORD * s)
{
    CONDITION cond;
    LST_HEAD *l;
    SQ_TYPE sqType;
    SQ_REFSTUDYSOPINSTANCEUID *studySQ;
    SQ_PROCEDURECODE *procedureSQ;
    SQ_REFSERIESSEQUENCE *seriesSQ;
    SQ_REFIMAGESEQUENCE *imageSQ;
    FIS_SCSERIESRECORD *scSeries;
    FIS_SCIMAGERECORD *scImage;

    memset(&studyComponent, 0, sizeof(studyComponent));
    studyComponent.Flag = 0;
    cond = DCM_ParseObject(obj, NULL, 0, studyComponentMap,
			   (int) DIM_OF(studyComponentMap), NULL);
    if (cond != DCM_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
			     "study component", "(FIS)parseStudyComponent");

    *s = studyComponent;
    s->Type = FIS_K_STUDYCOMPONENT;

    l = LST_Create();
    if (l == NULL)
	return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
			     "study component", "(FIS)parseStudyComponent");

    cond = parseSeq(obj, DCM_IDREFERENCEDSTUDYSEQ, &sqType, &l);
    if (cond == FIS_NORMAL) {
	if (sqType != SQ_K_REFSTUDYSOPINSTANCEUID)
	    return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
			     "study component", "(FIS)parseStudyComponent");
	studySQ = LST_Head(&l);
	if (studySQ == NULL)
	    return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
			     "study component", "(FIS)parseStudyComponent");
	strcpy(s->StuInsUID, studySQ->referencedSOPInstanceUID);
	s->Flag |= FIS_K_STUDYCOMP_STUINSUID;
	clearLlst(&l);
    } else
	(void) COND_PopCondition(TRUE);

    cond = parseSeq(obj, DCM_IDPROCEDURECODESEQUENCE, &sqType, &l);
    if (cond == FIS_NORMAL) {
	if (sqType != SQ_K_PROCEDURECODE)
	    return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
			     "study component", "(FIS)parseStudyComponent");
	procedureSQ = LST_Head(&l);
	if (procedureSQ == NULL)
	    return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
			     "study component", "(FIS)parseStudyComponent");
	strcpy(s->ProCodVal, procedureSQ->codeValue);
	strcpy(s->ProCodSchDes, procedureSQ->codingSchemeDesignator);
	strcpy(s->ProCodMea, procedureSQ->codeMeaning);
	s->Flag |= FIS_K_STUDYCOMP_PROCODVAL |
	    FIS_K_STUDYCOMP_PROCODSCHDES | FIS_K_STUDYCOMP_PROCODMEA;
	clearLlst(&l);
    } else
	(void) COND_PopCondition(TRUE);

    cond = parseSeq(obj, DCM_IDREFERENCEDSERIESSEQ, &sqType, &l);
    if (cond == FIS_NORMAL) {
	if (sqType != SQ_K_REFSERIESSEQUENCE)
	    return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
			     "study component", "(FIS)parseStudyComponent");
	s->SeriesList = LST_Create();
	if (s->SeriesList == NULL)
	    return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
			     "study component", "(FIS)parseStudyComponent");
	while ((seriesSQ = LST_Dequeue(&l)) != NULL) {
	    if ((scSeries = calloc(1, sizeof(*scSeries))) == NULL)
		return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
			     "study component", "(FIS)parseStudyComponent");
	    scSeries->Type = FIS_K_SCSERIES;
	    scSeries->Flag = FIS_K_SCSERIES_SERINSUID |
		FIS_K_SCSERIES_SERDAT | FIS_K_SCSERIES_SERTIM |
		FIS_K_SCSERIES_IMAGELIST;

	    strcpy(scSeries->SerInsUID, seriesSQ->seriesInstanceUID);
	    strcpy(scSeries->SerDat, seriesSQ->seriesDate);
	    strcpy(scSeries->SerTim, seriesSQ->seriesTime);
	    if (seriesSQ->conditionalFields & SQ_K_REFSERIES_RETRIEVEAETITLE) {
		strcpy(scSeries->RetAETit, seriesSQ->retrieveAETitle);
		scSeries->Flag |= FIS_K_SCSERIES_RETAETIT;
	    }
	    if (seriesSQ->conditionalFields & SQ_K_REFSERIES_STORAGEMEDIAFILESETID) {
		strcpy(scSeries->StoMedFilSetID, seriesSQ->storageMediaFileSetID);
		scSeries->Flag |= FIS_K_SCSERIES_STOMEDFILSETID;
	    }
	    if (seriesSQ->conditionalFields & SQ_K_REFSERIES_STORAGEMEDIAFILESETUID) {
		strcpy(scSeries->StoMedFilSetUID, seriesSQ->storageMediaFileSetUID);
		scSeries->Flag |= FIS_K_SCSERIES_STOMEDFILSETUID;
	    }
	    scSeries->ImageList = LST_Create();
	    if (scSeries->ImageList == NULL)
		return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
			     "study component", "(FIS)parseStudyComponent");
	    while ((imageSQ = LST_Dequeue(&seriesSQ->imageList)) != NULL) {
		if ((scImage = calloc(1, sizeof(*scImage))) == NULL)
		    return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
			     "study component", "(FIS)parseStudyComponent");
		scImage->Type = FIS_K_SCIMAGE;
		scImage->Flag = FIS_K_SCIMAGE_SOPINSUID |
		    FIS_K_SCIMAGE_SERINSUID |
		    FIS_K_SCIMAGE_SOPCLAUID;
		strcpy(scImage->SOPInsUID, imageSQ->refSOPInstanceUID);
		strcpy(scImage->SerInsUID, scSeries->SerInsUID);
		strcpy(scImage->SOPClaUID, imageSQ->refSOPClassUID);

		if (imageSQ->conditionalFields & SQ_K_REFIMAGE_RETRIEVEAETITLE) {
		    strcpy(scImage->RetAETit, imageSQ->retrieveAETitle);
		    scImage->Flag |= FIS_K_SCIMAGE_RETAETIT;
		}
		if (imageSQ->conditionalFields & SQ_K_REFIMAGE_STORAGEMEDIAFILESETID) {
		    strcpy(scImage->StoMedFilSetID, imageSQ->storageMediaFileSetID);
		    scImage->Flag |= FIS_K_SCIMAGE_STOMEDFILSETID;
		}
		if (imageSQ->conditionalFields & SQ_K_REFIMAGE_STORAGEMEDIAFILESETUID) {
		    strcpy(scImage->StoMedFilSetUID, imageSQ->storageMediaFileSetUID);
		    scImage->Flag |= FIS_K_SCIMAGE_STOMEDFILSETUID;
		}
		if (LST_Enqueue(&scSeries->ImageList, scImage) != LST_NORMAL)
		    return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
			     "study component", "(FIS)parseStudyComponent");
	    }

	    if ((LST_Enqueue(&s->SeriesList, scSeries)) != LST_NORMAL)
		return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
			     "study component", "(FIS)parseStudyComponent");
	}
	s->Flag |= FIS_K_STUDYCOMP_SERIESLIST;
    } else
	(void) COND_PopCondition(TRUE);

    (void) LST_Destroy(&l);

    return FIS_NORMAL;
}


static FIS_RESULTSRECORD
    results;
static DCM_FLAGGED_ELEMENT resultsMap[] = {
    {DCM_RESID, DCM_SH, "", 1, sizeof(results.ResID),
    (void *) &results.ResID[0], FIS_K_RESULTS_RESID, &results.Flag},
    {DCM_RESIMPRESSIONS, DCM_ST, "", 1, sizeof(results.Imp),
    (void *) &results.Imp[0], FIS_K_RESULTS_IMP, &results.Flag},
    {DCM_IDINSTANCECREATEDATE, DCM_DA, "", 1, sizeof(results.InsCreDat),
    (void *) &results.InsCreDat[0], FIS_K_RESULTS_INSCREDAT, &results.Flag},
    {DCM_IDINSTANCECREATETIME, DCM_TM, "", 1, sizeof(results.InsCreTim),
    (void *) &results.InsCreTim[0], FIS_K_RESULTS_INSCRETIM, &results.Flag},
    {DCM_IDINSTANCECREATORUID, DCM_UI, "", 1, sizeof(results.InsCreUID),
    (void *) &results.InsCreUID[0], FIS_K_RESULTS_INSCREUID, &results.Flag}
};

#define	RESULTS_CREATED_FLAGS	( FIS_K_RESULTS_STUINSUID | \
				FIS_K_RESULTS_RESID | \
				FIS_K_RESULTS_INTERPRETATIONLIST | \
				FIS_K_RESULTS_INSCREDAT | \
				FIS_K_RESULTS_INSCRETIM | \
				FIS_K_RESULTS_INSCREUID )

static EVENT_MAP resultsEventMap[] = {
    {FIS_K_RESULTS_CREATED, RESULTS_CREATED_FLAGS}
};

static CONDITION
buildResults(DCM_OBJECT ** obj, FIS_RESULTSRECORD * r, FIS_EVENT event)
{
    CONDITION cond;
    int i;

    results = *r;
    if (event != FIS_K_NO_EVENT) {
	for (i = 0; i < (int) DIM_OF(resultsEventMap); i++) {
	    if (resultsEventMap[i].Event == event)
		results.Flag = resultsEventMap[i].Flag;
	}
    }
    cond = DCM_ModifyElements(obj, NULL, 0, resultsMap, (int) DIM_OF(resultsMap),
			      NULL);
    if (cond != DCM_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_BUILDFAILED),
				  "results", "(FIS)buildResults");

    if (results.Flag & FIS_K_RESULTS_STUINSUID) {
	cond = addRefStudyUID(obj, r->StuInsUID);
	if (cond != FIS_NORMAL) {
	    (void) DCM_CloseObject(obj);
	    return COND_PushCondition(FIS_COND(FIS_BUILDFAILED),
				      "results", "(FIS)buildResults");
	}
    }
    if (results.Flag & FIS_K_RESULTS_INTERPRETATIONLIST) {
	cond = addRefInterpList(obj, r->InterpretationList);
	if (cond != FIS_NORMAL) {
	    (void) DCM_CloseObject(obj);
	    return COND_PushCondition(FIS_COND(FIS_BUILDFAILED),
				      "results", "(FIS)buildResults");
	}
    }
    return FIS_NORMAL;
}

static CONDITION
parseResults(DCM_OBJECT ** obj, FIS_RESULTSRECORD * s)
{
    CONDITION cond;
    LST_HEAD *l;
    SQ_REFINTERPRETATIONSOPINSTANCEUID *interpSQ;
    SQ_REFSTUDYSOPINSTANCEUID *studySQ;
    SQ_TYPE sqType;
    FIS_INTERPRETATIONRECORD *interp;

    memset(&results, 0, sizeof(results));
    results.Flag = 0;
    cond = DCM_ParseObject(obj, NULL, 0, resultsMap, (int) DIM_OF(resultsMap),
			   NULL);
    if (cond != DCM_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
				  "results", "(FIS)parseResults");

    *s = results;
    s->Type = FIS_K_RESULTS;

    l = LST_Create();
    if (l == NULL)
	return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
				  "results", "(FIS)parseResults");

    cond = parseSeq(obj, DCM_RESREFERENCEDINTERPSEQ, &sqType, &l);
    if (cond == 0)
	return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
				  "results", "(FIS)parseResults");
    if (cond == FIS_NORMAL) {
	if (sqType != SQ_K_REFINTERPRETATIONSOPINSTANCEUID)
	    return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
				      "results", "(FIS)parseResults");
	if ((s->InterpretationList = LST_Create()) == NULL)
	    return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
				      "results", "(FIS)parseResults");
	while ((interpSQ = LST_Dequeue(&l)) != NULL) {
	    if ((interp = calloc(1, sizeof(*interp))) == NULL)
		return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
					  "results", "(FIS)parseResults");
	    interp->Type = FIS_K_INTERPRETATION;
	    strcpy(interp->IntUID, interpSQ->referencedSOPInstanceUID);
	    interp->Flag = FIS_K_INTERP_INTUID;
	    if (LST_Enqueue(&s->InterpretationList, interp) != LST_NORMAL)
		return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
					  "results", "(FIS)parseResults");
	}
	s->Flag |= FIS_K_RESULTS_INTERPRETATIONLIST;
    }
    cond = parseSeq(obj, DCM_IDREFERENCEDSTUDYSEQ, &sqType, &l);
    if (cond == FIS_NORMAL) {
	if (sqType != SQ_K_REFSTUDYSOPINSTANCEUID)
	    return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
				      "results", "(FIS)parseResults");
	studySQ = LST_Head(&l);
	if (studySQ == NULL)
	    return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
				      "results", "(FIS)parseResults");
	strcpy(s->StuInsUID, studySQ->referencedSOPInstanceUID);
	s->Flag |= FIS_K_RESULTS_STUINSUID;
	clearLlst(&l);
    } else
	(void) COND_PopCondition(TRUE);

    return FIS_NORMAL;
}

static FIS_INTERPRETATIONRECORD
    interp;
static DCM_FLAGGED_ELEMENT interpretationMap[] = {
    {DCM_RESINTERPID, DCM_SH, "", 1, sizeof(interp.IntID),
    (void *) &interp.IntID[0], FIS_K_INTERP_INTID, &interp.Flag},
    {DCM_RESINTERPTEXT, DCM_ST, "", 1, sizeof(interp.IntTex),
    (void *) &interp.IntTex[0], FIS_K_INTERP_INTTEX, &interp.Flag},
    {DCM_RESINTERPTYPEID, DCM_CS, "", 1, sizeof(interp.IntTypID),
    (void *) &interp.IntTypID[0], FIS_K_INTERP_INTTYPID, &interp.Flag},
    {DCM_RESINTERPSTATUSID, DCM_CS, "", 1, sizeof(interp.IntStaID),
    (void *) &interp.IntStaID[0], FIS_K_INTERP_INTSTAID, &interp.Flag},
    {DCM_IDINSTANCECREATEDATE, DCM_DA, "", 1, sizeof(interp.InsCreDat),
    (void *) &interp.InsCreDat[0], FIS_K_INTERP_INSCREDAT, &interp.Flag},
    {DCM_IDINSTANCECREATETIME, DCM_TM, "", 1, sizeof(interp.InsCreTim),
    (void *) &interp.InsCreTim[0], FIS_K_INTERP_INSCRETIM, &interp.Flag},
    {DCM_IDINSTANCECREATORUID, DCM_UI, "", 1, sizeof(interp.InsCreUID),
    (void *) &interp.InsCreUID[0], FIS_K_INTERP_INSCREUID, &interp.Flag},
    {DCM_RESDIAGNOSIS, DCM_LT, "", 1, sizeof(interp.IntDiaDes),
    (void *) &interp.IntDiaDes[0], FIS_K_INTERP_INTDIADES, &interp.Flag},

};

#define	INTERPRETATION_CREATED_FLAGS	( FIS_K_INTERP_RESUID | \
					  FIS_K_INTERP_INSCREDAT | \
					  FIS_K_INTERP_INSCRETIM | \
					  FIS_K_INTERP_INSCREUID | \
					  FIS_K_INTERP_INTSTAID )

#define INTERPRETATION_APPROVED_FLAGS	( FIS_K_INTERP_INTDIADES | \
					  FIS_K_INTERP_INTSTAID | \
					  FIS_K_INTERP_PHYAPPINT | \
					  FIS_K_INTERP_DIACODSEQ )

static EVENT_MAP interpretationEventMap[] = {
    {FIS_K_INTERPRETATION_CREATED, INTERPRETATION_CREATED_FLAGS},
    {FIS_K_INTERPRETATION_APPROVED, INTERPRETATION_APPROVED_FLAGS}
};

static CONDITION
buildInterpretation(DCM_OBJECT ** obj, FIS_INTERPRETATIONRECORD * r,
		    FIS_EVENT event)
{
    CONDITION cond;
    int i;

    interp = *r;
    if (event != FIS_K_NO_EVENT) {
	for (i = 0; i < (int) DIM_OF(interpretationEventMap); i++) {
	    if (interpretationEventMap[i].Event == event)
		interp.Flag = interpretationEventMap[i].Flag;
	}
    }
    cond = DCM_ModifyElements(obj, NULL, 0, interpretationMap,
			      (int) DIM_OF(interpretationMap),
			      NULL);
    if (cond != DCM_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_BUILDFAILED),
			      "interpretation", "(FIS)buildInterpretation");

    if (interp.Flag & FIS_K_INTERP_RESUID) {
	cond = addRefResultsUID(obj, r->ResUID);
	if (cond != FIS_NORMAL) {
	    (void) DCM_CloseObject(obj);
	    return COND_PushCondition(FIS_COND(FIS_BUILDFAILED),
			      "interpretation", "(FIS)buildInterpretation");
	}
    }
    if (interp.Flag & FIS_K_INTERP_PHYAPPINT) {
	cond = addInterpretationApprover(obj, r);
	if (cond != FIS_NORMAL) {
	    (void) DCM_CloseObject(obj);
	    return COND_PushCondition(FIS_COND(FIS_BUILDFAILED),
			      "interpretation", "(FIS)buildInterpretation");
	}
    }
    if (interp.Flag & FIS_K_INTERP_DIACODSEQ) {
	cond = addDiagnosisCodes(obj, r);
	if (cond != FIS_NORMAL) {
	    (void) DCM_CloseObject(obj);
	    return COND_PushCondition(FIS_COND(FIS_BUILDFAILED),
			      "interpretation", "(FIS)buildInterpretation");
	}
    }
    return FIS_NORMAL;
}

static CONDITION
parseInterpretation(DCM_OBJECT ** obj, FIS_INTERPRETATIONRECORD * s)
{
    CONDITION cond;
    SQ_REFRESULTSSOPINSTANCEUID *resultsSQ = NULL;
    SQ_REQINTERPRETATIONAPPROVER *approver = NULL;
    SQ_TYPE sqType;
    LST_HEAD *l;

    memset(&interp, 0, sizeof(interp));
    interp.Flag = 0;
    cond = DCM_ParseObject(obj, NULL, 0, interpretationMap,
			   (int) DIM_OF(interpretationMap), NULL);
    if (cond != DCM_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
			      "interpretation", "(FIS)parseInterpretation");

    *s = interp;
    s->Type = FIS_K_INTERPRETATION;

    cond = parseSeq(obj, DCM_IDREFERENCEDRESULTSSEQ, &sqType, &l);
    if (cond == FIS_NORMAL) {
	if (sqType != SQ_K_REFRESULTSSOPINSTANCEUID)
	    return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
			      "interpretation", "(FIS)parseInterpretation");
	resultsSQ = LST_Head(&l);
	if (resultsSQ == NULL)
	    return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
			      "interpretation", "(FIS)parseInterpretation");
	strcpy(s->ResUID, resultsSQ->referencedSOPInstanceUID);
	s->Flag |= FIS_K_INTERP_RESUID;
	clearLlst(&l);
    } else
	(void) COND_PopCondition(TRUE);

    cond = parseSeq(obj, DCM_RESINTERPAPPROVERSEQUENCE, &sqType, &l);
    if (cond == FIS_NORMAL) {
	if (sqType != SQ_K_REQINTERPRETATIONAPPROVER)
	    if (sqType != SQ_K_REFRESULTSSOPINSTANCEUID)
		return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
			      "interpretation", "(FIS)parseInterpretation");
	approver = LST_Head(&l);
	if (approver == NULL)
	    return COND_PushCondition(FIS_COND(FIS_PARSEFAILED),
			      "interpretation", "(FIS)parseInterpretation");
	strcpy(s->IntAppDat, approver->approvalDates);
	strcpy(s->IntAppTim, approver->approvalTimes);
	strcpy(s->PhyAppInt, approver->physiciansApproving);

	s->Flag |= FIS_K_INTERP_INTAPPDAT | FIS_K_INTERP_INTAPPTIM | FIS_K_INTERP_PHYAPPINT;
	clearLlst(&l);
    } else
	(void) COND_PopCondition(TRUE);

    return FIS_NORMAL;
}


static CONDITION
addRefPatientUID(DCM_OBJECT ** obj, char *patientUID)
{
    CONDITION
    cond;
    LST_HEAD
	* l;
    SQ_REFPATIENTSOPINSTANCEUID
	* refPatient;
    DCM_ELEMENT
	* e;


    l = LST_Create();
    if (l == NULL) {
	return 0;
    }
    refPatient = malloc(sizeof(*refPatient));
    if (refPatient == NULL) {
	(void) LST_Destroy(&l);
	return 0;
    }
    refPatient->type = SQ_K_REFPATIENTSOPINSTANCEUID;
    refPatient->conditionalFields = 0;
    strcpy(refPatient->referencedSOPClassUID,
	   DICOM_SOPCLASSDETACHEDPATIENTMGMT);
    strcpy(refPatient->referencedSOPInstanceUID, patientUID);
    if (LST_Enqueue(&l, refPatient) != LST_NORMAL) {
	(void) LST_Destroy(l);
	free(refPatient);
	return 0;
    }
    cond = SQ_BuildSequence(&l, SQ_K_REFPATIENTSOPINSTANCEUID, &e);
    if (cond != SQ_NORMAL) {
	(void) DCM_CloseObject(obj);
	(void) LST_Destroy(l);
	free(refPatient);
	return 0;
    }
    cond = DCM_AddElement(obj, e);
    if (cond != DCM_NORMAL) {
	(void) DCM_CloseObject(obj);
	(void) LST_Destroy(l);
	return 0;
    }
    return FIS_NORMAL;
}


static CONDITION
addRefVisitUID(DCM_OBJECT ** obj, char *visitUID)
{
    CONDITION
    cond;
    LST_HEAD
	* l;
    SQ_REFVISITSOPINSTANCEUID
	* refVisit;
    DCM_ELEMENT
	* e;

    l = LST_Create();
    if (l == NULL) {
	return 0;
    }
    refVisit = malloc(sizeof(*refVisit));
    if (refVisit == NULL) {
	(void) LST_Destroy(&l);
	return 0;
    }
    refVisit->type = SQ_K_REFVISITSOPINSTANCEUID;
    refVisit->conditionalFields = 0;
    strcpy(refVisit->referencedSOPClassUID,
	   DICOM_SOPCLASSDETACHEDVISITMGMT);
    strcpy(refVisit->referencedSOPInstanceUID, visitUID);
    if (LST_Enqueue(&l, refVisit) != LST_NORMAL) {
	(void) LST_Destroy(l);
	free(refVisit);
	return 0;
    }
    cond = SQ_BuildSequence(&l, SQ_K_REFVISITSOPINSTANCEUID, &e);
    if (cond != SQ_NORMAL) {
	(void) LST_Destroy(l);
	free(refVisit);
	return 0;
    }
    cond = DCM_AddElement(obj, e);
    if (cond != DCM_NORMAL) {
	(void) LST_Destroy(l);
	return 0;
    }
    return FIS_NORMAL;
}
static CONDITION
addRefStudyUID(DCM_OBJECT ** obj, char *studyUID)
{
    CONDITION cond;
    LST_HEAD *l;
    SQ_REFSTUDYSOPINSTANCEUID *refStudy;
    DCM_ELEMENT *e;

    l = LST_Create();
    if (l == NULL) {
	return 0;
    }
    refStudy = malloc(sizeof(*refStudy));
    if (refStudy == NULL) {
	(void) LST_Destroy(&l);
	return 0;
    }
    refStudy->type = SQ_K_REFSTUDYSOPINSTANCEUID;
    refStudy->conditionalFields = 0;
    strcpy(refStudy->referencedSOPClassUID,
	   DICOM_SOPCLASSDETACHEDSTUDYMGMT);
    strcpy(refStudy->referencedSOPInstanceUID, studyUID);
    if (LST_Enqueue(&l, refStudy) != LST_NORMAL) {
	(void) LST_Destroy(l);
	free(refStudy);
	return 0;
    }
    cond = SQ_BuildSequence(&l, SQ_K_REFSTUDYSOPINSTANCEUID, &e);
    if (cond != SQ_NORMAL) {
	(void) LST_Destroy(l);
	free(refStudy);
	return 0;
    }
    cond = DCM_AddElement(obj, e);
    if (cond != DCM_NORMAL) {
	(void) LST_Destroy(l);
	return 0;
    }
    return FIS_NORMAL;
}

static CONDITION
addRefResultsUID(DCM_OBJECT ** obj, char *resultsUID)
{
    CONDITION cond;
    LST_HEAD *l;
    SQ_REFVISITSOPINSTANCEUID *refResults;
    DCM_ELEMENT *e;

    l = LST_Create();
    if (l == NULL) {
	return 0;
    }
    refResults = malloc(sizeof(*refResults));
    if (refResults == NULL) {
	(void) LST_Destroy(&l);
	return 0;
    }
    refResults->type = SQ_K_REFRESULTSSOPINSTANCEUID;
    refResults->conditionalFields = 0;
    strcpy(refResults->referencedSOPClassUID,
	   DICOM_SOPCLASSDETACHEDRESULTSMGMT);
    strcpy(refResults->referencedSOPInstanceUID, resultsUID);
    if (LST_Enqueue(&l, refResults) != LST_NORMAL) {
	(void) LST_Destroy(l);
	free(refResults);
	return 0;
    }
    cond = SQ_BuildSequence(&l, SQ_K_REFRESULTSSOPINSTANCEUID, &e);
    if (cond != SQ_NORMAL) {
	(void) LST_Destroy(l);
	free(refResults);
	return 0;
    }
    cond = DCM_AddElement(obj, e);
    if (cond != DCM_NORMAL) {
	(void) LST_Destroy(l);
	return 0;
    }
    return FIS_NORMAL;
}

static CONDITION
addProcedureCodeSeq(DCM_OBJECT ** obj, char *codeValue, char *codingScheme,
		    char *codeMeaning)
{
    CONDITION cond;
    LST_HEAD *l;
    SQ_PROCEDURECODE *procedure;
    DCM_ELEMENT *e;

    l = LST_Create();
    if (l == NULL) {
	return 0;
    }
    procedure = malloc(sizeof(*procedure));
    if (procedure == NULL) {
	(void) LST_Destroy(&l);
	return 0;
    }
    procedure->type = SQ_K_PROCEDURECODE;
    strcpy(procedure->codeValue, codeValue);
    strcpy(procedure->codingSchemeDesignator, codingScheme);
    strcpy(procedure->codeMeaning, codeMeaning);
    if (LST_Enqueue(&l, procedure) != LST_NORMAL) {
	(void) LST_Destroy(l);
	free(procedure);
	return 0;
    }
    cond = SQ_BuildSequence(&l, SQ_K_PROCEDURECODE, &e);
    if (cond != SQ_NORMAL) {
	(void) LST_Destroy(l);
	free(procedure);
	return 0;
    }
    cond = DCM_AddElement(obj, e);
    if (cond != DCM_NORMAL) {
	(void) LST_Destroy(l);
	return 0;
    }
    return FIS_NORMAL;
}


#ifdef SMM
static CONDITION
addRefStudyList(DCM_OBJECT ** obj, LST_HEAD * studyList)
{
    CONDITION cond;
    LST_HEAD *l;
    SQ_REFSTUDYSOPINSTANCEUID *refStudy;
    FIS_STUDYRECORD *studyPtr;
    DCM_ELEMENT *e;

    l = LST_Create();
    if (l == NULL) {
	return 0;
    }
    studyPtr = LST_Head(&studyList);
    (void) LST_Position(&studyList, studyPtr);

    while (studyPtr != NULL) {
	refStudy = malloc(sizeof(*refStudy));
	if (refStudy == NULL) {
	    (void) LST_Destroy(&l);	/* memory leak */
	    return 0;
	}
	refStudy->type = SQ_K_REFSTUDYSOPINSTANCEUID;
	refStudy->conditionalFields = 0;
	strcpy(refStudy->referencedSOPClassUID,
	       DICOM_SOPCLASSDETACHEDSTUDYMGMT);
	strcpy(refStudy->referencedSOPInstanceUID, studyPtr->StuInsUID);
	if (LST_Enqueue(&l, refStudy) != LST_NORMAL) {
	    (void) LST_Destroy(l);	/* memory leak */
	    free(refStudy);
	    return 0;
	}
	studyPtr = LST_Next(&studyList);
    }
    cond = SQ_BuildSequence(&l, SQ_K_REFSTUDYSOPINSTANCEUID, &e);
    if (cond != SQ_NORMAL) {
	(void) LST_Destroy(l);	/* memory leak */
	return 0;
    }
    cond = DCM_AddElement(obj, e);
    if (cond != DCM_NORMAL) {
	(void) LST_Destroy(l);
	return 0;
    }
    return FIS_NORMAL;
}
#endif

static CONDITION
addRefSCList(DCM_OBJECT ** obj, LST_HEAD * scList)
{
    CONDITION cond;
    LST_HEAD *l;
    SQ_REFSTUDYCOMPONENT *refStudyComponent;
    FIS_STUDYCOMPONENTRECORD *studyComponentPtr;
    DCM_ELEMENT *e;

    l = LST_Create();
    if (l == NULL) {
	return 0;
    }
    studyComponentPtr = LST_Head(&scList);
    if (studyComponentPtr != NULL)
	(void) LST_Position(&scList, studyComponentPtr);

    while (studyComponentPtr != NULL) {
	refStudyComponent = malloc(sizeof(*refStudyComponent));
	if (refStudyComponent == NULL) {
	    (void) LST_Destroy(&l);	/* memory leak */
	    return 0;
	}
	refStudyComponent->type = SQ_K_REFSTUDYCOMPONENT;
	strcpy(refStudyComponent->refSOPClassUID,
	       DICOM_SOPCLASSSTUDYCOMPONENTMGMT);
	strcpy(refStudyComponent->refSOPInstanceUID,
	       studyComponentPtr->StuComUID);
	if (LST_Enqueue(&l, refStudyComponent) != LST_NORMAL) {
	    (void) LST_Destroy(l);	/* memory leak */
	    free(refStudyComponent);
	    return 0;
	}
	studyComponentPtr = LST_Next(&scList);
    }
    cond = SQ_BuildSequence(&l, SQ_K_REFSTUDYCOMPONENT, &e);
    if (cond != SQ_NORMAL) {
	(void) LST_Destroy(l);	/* memory leak */
	return 0;
    }
    cond = DCM_AddElement(obj, e);
    if (cond != DCM_NORMAL) {
	(void) LST_Destroy(l);
	return 0;
    }
    return FIS_NORMAL;
}

static CONDITION
addRefSeriesList(DCM_OBJECT ** obj, LST_HEAD * seriesList)
{
    CONDITION cond;
    LST_HEAD *l;
    SQ_REFSERIESSEQUENCE *refSeries;
    FIS_SCSERIESRECORD *series;
    DCM_ELEMENT *e;

    l = LST_Create();
    if (l == NULL) {
	return 0;
    }
    series = LST_Head(&seriesList);
    (void) LST_Position(&seriesList, series);

    while (series != NULL) {
	refSeries = calloc(1, sizeof(*refSeries));
	if (refSeries == NULL) {
	    (void) LST_Destroy(&l);	/* memory leak */
	    return 0;
	}
	refSeries->type = SQ_K_REFSERIESSEQUENCE;
	refSeries->conditionalFields = 0;
	refSeries->imageList = LST_Create();
	if (refSeries->imageList == NULL) {
	    (void) LST_Destroy(&l);	/* memory leak */
	    return 0;
	}
	strcpy(refSeries->seriesDate, series->SerDat);
	strcpy(refSeries->seriesTime, series->SerTim);
	strcpy(refSeries->seriesInstanceUID, series->SerInsUID);
	if (series->Flag & FIS_K_SCSERIES_RETAETIT) {
	    strcpy(refSeries->retrieveAETitle, series->RetAETit);
	    refSeries->conditionalFields |= SQ_K_REFSERIES_RETRIEVEAETITLE;
	}
	if (series->Flag & FIS_K_SCSERIES_STOMEDFILSETID) {
	    strcpy(refSeries->storageMediaFileSetID, series->StoMedFilSetID);
	    refSeries->conditionalFields |= SQ_K_REFSERIES_STORAGEMEDIAFILESETID;
	}
	if (series->Flag & FIS_K_SCSERIES_STOMEDFILSETUID) {
	    strcpy(refSeries->storageMediaFileSetUID, series->StoMedFilSetUID);
	    refSeries->conditionalFields |= SQ_K_REFSERIES_STORAGEMEDIAFILESETUID;
	}
	cond = addRefImageList(series->ImageList, refSeries->imageList);
	if (cond != FIS_NORMAL)
	    return 0;		/* memory leak */

	if (LST_Enqueue(&l, refSeries) != LST_NORMAL) {
	    (void) LST_Destroy(l);	/* memory leak */
	    free(refSeries);
	    return 0;
	}
	series = LST_Next(&seriesList);
    }
    cond = SQ_BuildSequence(&l, SQ_K_REFSERIESSEQUENCE, &e);
    if (cond != SQ_NORMAL) {
	(void) LST_Destroy(l);	/* memory leak */
	return 0;
    }
    cond = DCM_AddElement(obj, e);
    if (cond != DCM_NORMAL) {
	(void) LST_Destroy(l);
	return 0;
    }
    return FIS_NORMAL;
}

static CONDITION
addRefImageList(LST_HEAD * imagesFIS, LST_HEAD * imagesSQ)
{
    SQ_REFIMAGESEQUENCE *refImage;
    FIS_SCIMAGERECORD *image;

    image = LST_Head(&imagesFIS);
    (void) LST_Position(&imagesFIS, image);

    while (image != NULL) {
	refImage = calloc(1, sizeof(*refImage));
	if (refImage == NULL) {
	    return 0;
	}
	refImage->type = SQ_K_REFIMAGESEQUENCE;
	refImage->conditionalFields = 0;

	strcpy(refImage->refSOPClassUID, image->SOPClaUID);
	strcpy(refImage->refSOPInstanceUID, image->SOPInsUID);
	if (image->Flag & FIS_K_SCIMAGE_RETAETIT) {
	    strcpy(refImage->retrieveAETitle, image->RetAETit);
	    refImage->conditionalFields |= SQ_K_REFIMAGE_RETRIEVEAETITLE;
	}
	if (image->Flag & FIS_K_SCIMAGE_STOMEDFILSETID) {
	    strcpy(refImage->storageMediaFileSetID, image->StoMedFilSetID);
	    refImage->conditionalFields |= SQ_K_REFIMAGE_STORAGEMEDIAFILESETID;
	}
	if (image->Flag & FIS_K_SCIMAGE_STOMEDFILSETUID) {
	    strcpy(refImage->storageMediaFileSetUID, image->StoMedFilSetUID);
	    refImage->conditionalFields |= SQ_K_REFIMAGE_STORAGEMEDIAFILESETUID;
	}
	if (LST_Enqueue(&imagesSQ, refImage) != LST_NORMAL) {
	    return 0;		/* memory leadk */
	}
	image = LST_Next(&imagesFIS);
    }
    return FIS_NORMAL;
}


static CONDITION
addRefResultsList(DCM_OBJECT ** obj, LST_HEAD * resultsList)
{
    CONDITION cond;
    LST_HEAD *l;
    SQ_REFRESULTSSOPINSTANCEUID *refResults;
    FIS_RESULTSRECORD *resultsPtr;
    DCM_ELEMENT *e;

    l = LST_Create();
    if (l == NULL) {
	return 0;
    }
    resultsPtr = LST_Head(&resultsList);
    if (resultsPtr != NULL)
	(void) LST_Position(&resultsList, resultsPtr);

    while (resultsPtr != NULL) {
	refResults = malloc(sizeof(*refResults));
	if (refResults == NULL) {
	    (void) LST_Destroy(&l);	/* memory leak */
	    return 0;
	}
	refResults->type = SQ_K_REFRESULTSSOPINSTANCEUID;
	refResults->conditionalFields = 0;
	strcpy(refResults->referencedSOPClassUID,
	       DICOM_SOPCLASSDETACHEDRESULTSMGMT);
	strcpy(refResults->referencedSOPInstanceUID, resultsPtr->ResUID);
	if (LST_Enqueue(&l, refResults) != LST_NORMAL) {
	    (void) LST_Destroy(l);	/* memory leak */
	    free(refResults);
	    return 0;
	}
	resultsPtr = LST_Next(&resultsList);
    }
    cond = SQ_BuildSequence(&l, SQ_K_REFRESULTSSOPINSTANCEUID, &e);
    if (cond != SQ_NORMAL) {
	(void) LST_Destroy(l);	/* memory leak */
	return 0;
    }
    cond = DCM_AddElement(obj, e);
    if (cond != DCM_NORMAL) {
	(void) LST_Destroy(l);
	return 0;
    }
    return FIS_NORMAL;
}

static CONDITION
addRefInterpList(DCM_OBJECT ** obj, LST_HEAD * interpList)
{
    CONDITION cond;
    LST_HEAD *l;
    SQ_REFINTERPRETATIONSOPINSTANCEUID *refInterpretation;
    FIS_INTERPRETATIONRECORD *interpPtr;
    DCM_ELEMENT *e;

    l = LST_Create();
    if (l == NULL) {
	return 0;
    }
    interpPtr = LST_Head(&interpList);
    (void) LST_Position(&interpList, interpPtr);

    while (interpPtr != NULL) {
	refInterpretation = malloc(sizeof(*refInterpretation));
	if (refInterpretation == NULL) {
	    (void) LST_Destroy(&l);	/* memory leak */
	    return 0;
	}
	refInterpretation->type = SQ_K_REFINTERPRETATIONSOPINSTANCEUID;
	refInterpretation->conditionalFields = 0;
	strcpy(refInterpretation->referencedSOPClassUID,
	       DICOM_SOPCLASSDETACHEDINTERPRETMGMT);
	strcpy(refInterpretation->referencedSOPInstanceUID, interpPtr->IntUID);
	if (LST_Enqueue(&l, refInterpretation) != LST_NORMAL) {
	    (void) LST_Destroy(l);	/* memory leak */
	    free(refInterpretation);
	    return 0;
	}
	interpPtr = LST_Next(&interpList);
    }
    cond = SQ_BuildSequence(&l, SQ_K_REFINTERPRETATIONSOPINSTANCEUID, &e);
    if (cond != SQ_NORMAL) {
	(void) LST_Destroy(l);	/* memory leak */
	return 0;
    }
    cond = DCM_AddElement(obj, e);
    if (cond != DCM_NORMAL) {
	(void) LST_Destroy(l);
	return 0;
    }
    return FIS_NORMAL;
}



static CONDITION
parseSeq(DCM_OBJECT ** obj, DCM_TAG tag, SQ_TYPE * type, LST_HEAD ** l)
{
    DCM_ELEMENT e;
    CONDITION cond;

    e.tag = tag;
    cond = DCM_GetSequenceList(obj, e.tag, &e.d.sq);
    if (cond == DCM_ELEMENTNOTFOUND) {
	(void) COND_PopCondition(TRUE);
	return FIS_REQUESTEDATTRIBUTEMISSING;
    } else if (cond != DCM_NORMAL)
	return 0;

    cond = SQ_ParseSequence(&e, type, l);
    if (cond != SQ_NORMAL)
	return 0;

    return FIS_NORMAL;
}

static void
clearLlst(LST_HEAD ** l)
{
    LST_NODE *n;

    while ((n = LST_Dequeue(l)) != NULL)
	free(n);
}
