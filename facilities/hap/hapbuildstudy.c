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
**                              DICOM 93
**                   Electronic Radiology Laboratory
**                 Mallinckrodt Institute of Radiology
**              Washington University School of Medicine
**
** Module Name(s):	HAP_BuildStudy
** Author, Date:	John T. O'Neill, 14-June-93
** Intent:              This module translates a HIS level structure to a
**                      DCM_OBJECT or a DCM_OBJECT to a HIS level structure.
** Last Update:         $Author: smm $, $Date: 1995-04-07 20:45:18 $
** Source File:         $RCSfile: hapbuildstudy.c,v $
** Revision:            $Revision: 1.18 $
** Status:              $State: Exp $
*/

static char rcsid[] = "$Revision: 1.18 $ $RCSfile: hapbuildstudy.c,v $";

#define BUILD
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "dicom.h"
#include "lst.h"
#include "dicom_objects.h"
#include "hisdb.h"
#include "hap.h"
#include "dicom_uids.h"
#include "dicom_sq.h"


#define HAP_X_VARIABLE_EVENT	HAP_S_VARIABLE_EVENT

static HIS_StudyLevel
    p;

static DCM_FLAGGED_ELEMENT list[] = {
    {DCM_IDINSTANCECREATEDATE, DCM_DA, "", 1,
	sizeof p.ICDate, (void *) &p.ICDate[0],
    HIS_K_STUICDATE, &p.Update_Flag},
    {DCM_IDINSTANCECREATETIME, DCM_TM, "", 1,
	sizeof p.ICTime, (void *) &p.ICTime[0],
    HIS_K_STUICTIME, &p.Update_Flag},
    {DCM_IDINSTANCECREATORUID, DCM_UI, "", 1,
	sizeof p.ICUID, (void *) &p.ICUID[0],
    HIS_K_STUICUID, &p.Update_Flag},
    {DCM_SDYSCHEDULEDSTARTDATE, DCM_DA, "", 1,
	sizeof p.SSSDate, (void *) &p.SSSDate[0],
    HIS_K_STUSSSDATE, &p.Update_Flag},
    {DCM_SDYSCHEDULEDSTARTTIME, DCM_TM, "", 1,
	sizeof p.SSSTime, (void *) &p.SSSTime[0],
    HIS_K_STUSSSTIME, &p.Update_Flag},
    {DCM_SDYSCHEDULEDLOCATION, DCM_LO, "", 1,
	sizeof p.SSLocation, (void *) &p.SSLocation[0],
    HIS_K_STUSSLOCATION, &p.Update_Flag},
    {DCM_SDYSCHEDULEDLOCATIONAETITLE, DCM_AE, "", 1,
	sizeof p.SSLATitle, (void *) &p.SSLATitle[0],
    HIS_K_STUSSLATITLE, &p.Update_Flag},
    {DCM_SDYREQUESTEDPRODESCRIPTION, DCM_LO, "", 1,
	sizeof p.RPDescription, (void *) &p.RPDescription[0],
    HIS_K_STURPDESCRIPTION, &p.Update_Flag},
#ifdef SMM
    {DCM_SDYREQUESTEDPROCODINGUID, DCM_UI, "", 1,
	sizeof p.RPCSUID, (void *) &p.RPCSUID[0],
    HIS_K_STURPCSUID, &p.Update_Flag},
    {DCM_SDYREQUESTEDPROCODE, DCM_SH, "", 1,
	sizeof p.RPCode, (void *) &p.RPCode[0],
    HIS_K_STURPCODE, &p.Update_Flag},
#endif
    {DCM_SDYARRIVALDATE, DCM_DA, "", 1,
	sizeof p.SADate, (void *) &p.SADate[0],
    HIS_K_STUSADATE, &p.Update_Flag},
    {DCM_SDYARRIVALTIME, DCM_TM, "", 1,
	sizeof p.SATime, (void *) &p.SATime[0],
    HIS_K_STUSATIME, &p.Update_Flag},
    {DCM_IDSTUDYDATE, DCM_DA, "", 1,
	sizeof p.SDate, (void *) &p.SDate[0],
    HIS_K_STUSDATE, &p.Update_Flag},
    {DCM_IDSTUDYTIME, DCM_TM, "", 1,
	sizeof p.STime, (void *) &p.STime[0],
    HIS_K_STUSTIME, &p.Update_Flag},
    {DCM_SDYCOMPLETIONDATE, DCM_DA, "", 1,
	sizeof p.SCDate, (void *) &p.SCDate[0],
    HIS_K_STUSCDATE, &p.Update_Flag},
    {DCM_SDYCOMPLETIONTIME, DCM_TM, "", 1,
	sizeof p.SCTime, (void *) &p.SCTime[0],
    HIS_K_STUSCTIME, &p.Update_Flag},
    {DCM_SDYVERIFIEDDATE, DCM_DA, "", 1,
	sizeof p.SVDate, (void *) &p.SVDate[0],
    HIS_K_STUSVDATE, &p.Update_Flag},
    {DCM_SDYVERIFIEDTIME, DCM_TM, "", 1,
	sizeof p.SVTime, (void *) &p.SVTime[0],
    HIS_K_STUSVTIME, &p.Update_Flag},
    {DCM_SDYREADDATE, DCM_DA, "", 1,
	sizeof p.SRDate, (void *) &p.SRDate[0],
    HIS_K_STUSRDATE, &p.Update_Flag},
    {DCM_SDYREADTIME, DCM_TM, "", 1,
	sizeof p.SRTime, (void *) &p.SRTime[0],
    HIS_K_STUSRTIME, &p.Update_Flag},
    {DCM_RELSTUDYID, DCM_SH, "", 1,
	sizeof p.SID, (void *) &p.SID[0],
    HIS_K_STUSID, &p.Update_Flag},
    {DCM_SDYIDISSUER, DCM_LO, "", 1,
	sizeof p.SIIssuer, (void *) &p.SIIssuer[0],
    HIS_K_STUSIISSUER, &p.Update_Flag},
#ifdef SMM
    {DCM_IDPROCEDURECODINGSCHEMEUID, DCM_UI, "", 1,
	sizeof p.PCSUID, (void *) &p.PCSUID[0],
    HIS_K_STUPCSUID, &p.Update_Flag},
#endif
#ifdef SMM
    {DCM_IDPROCEDURECODE, DCM_SH, "", 1,
	sizeof p.PCode, (void *) &p.PCode[0],
    HIS_K_STUPCODE, &p.Update_Flag},
#endif
    {DCM_SDYPRIORITYID, DCM_CS, "", 1,
	sizeof p.StudyPriorityID, (void *) &p.StudyPriorityID[0],
    HIS_K_STUSPID, &p.Update_Flag},
    {DCM_SDYREASON, DCM_LO, "", 1,
	sizeof p.RfStudy, (void *) &p.RfStudy[0],
    HIS_K_STURFSTUDY, &p.Update_Flag},
    {DCM_SDYREQUESTINGPHYSICIAN, DCM_PN, "", 1,
	sizeof p.RPhysician, (void *) &p.RPhysician[0],
    HIS_K_STURPHYSICIAN, &p.Update_Flag},
    {DCM_IDACCESSIONNUMBER, DCM_SH, "", 1,
	sizeof p.ANumber, (void *) &p.ANumber[0],
    HIS_K_STUANUMBER, &p.Update_Flag},
    {DCM_SDYSTATUSID, DCM_CS, "", 1,
	sizeof p.StudyStatusID, (void *) &p.StudyStatusID[0],
    HIS_K_STUSSID, &p.Update_Flag},
};

typedef struct {
    HAP_STUDY_EVENT event;
    unsigned long required_event_attrib;
}   EVENT_MAP;

static EVENT_MAP array[] = {
    {HAP_STUDY_CREATED, HAP_REQUIRED_SCREATED_ATTRIB},
    {HAP_STUDY_SCHEDULED, HAP_REQUIRED_SSCHEDULED_ATTRIB},
    {HAP_PATIENT_ARRIVED, HAP_REQUIRED_SARRIVED_ATTRIB},
    {HAP_STUDY_STARTED, HAP_REQUIRED_SSTARTED_ATTRIB},
    {HAP_STUDY_COMPLETED, HAP_REQUIRED_SCOMPLETED_ATTRIB},
    {HAP_STUDY_VERIFIED, HAP_REQUIRED_SVERIFIED_ATTRIB},
    {HAP_STUDY_READ, HAP_REQUIRED_SREAD_ATTRIB},
    {HAP_STUDY_DELETED, HAP_REQUIRED_SUPDATED_ATTRIB},
    {HAP_STUDY_UPDATED, HAP_REQUIRED_SDELETED_ATTRIB}
};

static int s_buildSequence(DCM_OBJECT ** object, SQ_TYPE type);
static int s_parseSequence(DCM_OBJECT ** object);
static int
    sEmptySequence(DCM_OBJECT ** object, SQ_TYPE type);

/* HAP_BuildStudy
**
** Purpose:
**      HAP_BuildStudy adds attributes to the DCM_OBJECT passed to it. It
**      gets the attribute values from the HIS_StudyLevel structure.
**      It knows which attributes from within the structure to add from the
**      Update_Flag element of the HIS_InterpretationLevel structure. The
**      event argument should be either 0 or an enum constant of
**      type HAP_STUDY_EVENT. If it is a enum constant, the function checks
**      the bits of Update_Flag so that the mandatory attributes for that event
**      are present and returns an error if they are not. For 0, there is no
**      checking.
**
** Parameter Dictionary:
**      object          The existing DCM_OBJECT that is to be built.
**      xl              The structure containing attributes to add to the
**                      DCM_OBJECT.
**      event           The event type.
**
** Return Values:
**      HAP_NORMAL
**      HAP_ILLEGALOBJECT
**      HAP_ILLEGALEVENT
**      HAP_INCOMPLETEEVENT
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/


CONDITION
HAP_BuildStudy(DCM_OBJECT ** object,
	       HIS_StudyLevel * xl, HAP_STUDY_EVENT event)
{

    CONDITION
	cond;
    int
        index,
        x;
    CTNBOOLEAN
	eventFound = FALSE;

    static unsigned long his_flags[] =
    {HIS_K_STURPSEQUENCE, HIS_K_STURVSEQUENCE, HIS_K_STURRSEQUENCE,
    HIS_K_STUPCSEQUENCE, HIS_K_STURPCSEQUENCE, HIS_K_STURSCSEQUENCE};
    static SQ_TYPE types[] =
    {SQ_K_REFPATIENTSOPINSTANCEUID, SQ_K_REFVISITSOPINSTANCEUID,
	SQ_K_REFRESULTSSOPINSTANCEUID, SQ_K_PROCEDURECODE,
    SQ_K_REQPROCEDURECODE, SQ_K_REFSTUDYCOMPONENT};

    p = *xl;

    if (event != 0) {
	for (index = 0; index < (int) DIM_OF(array) && !eventFound; index++) {
	    if (event == array[index].event) {
		eventFound = TRUE;
		if ((event & HAP_X_VARIABLE_EVENT) != 0) {
		    if ((p.Update_Flag & array[index].required_event_attrib) !=
			array[index].required_event_attrib)
			return HAP_INCOMPLETEEVENT;
		} else {
		    if (p.Update_Flag != array[index].required_event_attrib)
			return HAP_INCOMPLETEEVENT;
		}
	    }
	}

	if (!eventFound)
	    return HAP_ILLEGALEVENT;
    }
    cond = DCM_ModifyElements(object, NULL, 0, list, (int) DIM_OF(list),
			      NULL);
    if (cond != DCM_NORMAL)
	return HAP_ILLEGALOBJECT;

    for (index = 0; index < (int) DIM_OF(his_flags); index++)
	if (p.Update_Flag & his_flags[index]) {
	    x = s_buildSequence(object, types[index]);
	    if (x < 0)
		return 0;
	}
/*
    if (p.Update_Flag & HIS_K_STURPSEQUENCE) {
	x = s_buildSequence(object, REFPATIENTSQ);
	if (x < 0)
	    return 0;
    }
    if (p.Update_Flag & HIS_K_STURVSEQUENCE) {
	x = s_buildSequence(object, REFVISITSQ);
	if (x < 0)
	    return 0;
    }
    if (p.Update_Flag2 & HIS_K_STURRSEQUENCE) {
	x = s_buildSequence(object, REFRESULTSSQ);
	if (x < 0)
	    return 0;
    }
    if (event == HAP_STUDY_SCHEDULED) {
	x = sEmptySequence(object, SQ_K_REQPROCEDURECODE);
	if (x < 0)
	    return 0;
    } else if (event == HAP_STUDY_COMPLETED) {
	x = sEmptySequence(object, SQ_K_REFSTUDYCOMPONENT);
	if (x < 0)
	    return 0;
    } else if (event == HAP_STUDY_VERIFIED) {
	x = sEmptySequence(object, SQ_K_REFSTUDYCOMPONENT);
	if (x < 0)
	    return 0;
    }
*/
    return HAP_NORMAL;
}

/* HAP_ParseStudy
**
** Purpose:
**      HAP_ParseStudy extracts from a DCM_OBJECT all the attributes present
**      that are specified in the Detached Study Management Service
**      Class. The attibute values are placed in the HIS_StudyLevel
**      structure, and the Update_Flag element of this structure is set
**      according to what attributes were found.
**
** Parameter Dictionary:
**      object          Existing DCM_OBJECT to be parsed.
**      xl              The structure that holds the attribute values parsed
**                      from object.
**
** Return Values:
**      HAP_NORMAL
**      HAP_ILLEGALOBJECT
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/


CONDITION
HAP_ParseStudy(DCM_OBJECT ** object, HIS_StudyLevel * xl)
{

    CONDITION cond;
    int x;

    memset(&p, 0, sizeof(p));

    cond = DCM_ParseObject(object, NULL, 0, list, (int) DIM_OF(list), NULL);

    if (cond != DCM_NORMAL)
	return HAP_ILLEGALOBJECT;

    x = s_parseSequence(object);
    if (x < 0)
	return 0;

    *xl = p;

    return HAP_NORMAL;
}

typedef struct {
    SQ_TYPE type;
    char *sopclass;
    long *counter;
    long size;
}   SQ_MAP_SOP;

static int
s_buildSequence(DCM_OBJECT ** object, SQ_TYPE type)
{

    int index = 0;
    int i;
    LST_HEAD *l;
    DCM_ELEMENT *e;
    CONDITION cond;
    SQ_REFPATIENTSOPINSTANCEUID *patient;
    SQ_REFVISITSOPINSTANCEUID *visit;
    SQ_REFRESULTSSOPINSTANCEUID *results;
    SQ_REQPROCEDURECODE *code;
    SQ_REFSTUDYCOMPONENT *component;
    void *pointer;

    static SQ_MAP_SOP sops[] = {
	{SQ_K_REFPATIENTSOPINSTANCEUID,
	    DICOM_SOPCLASSDETACHEDPATIENTMGMT, &(p.RPSequenceCount),
	sizeof(SQ_REFPATIENTSOPINSTANCEUID)},
	{SQ_K_REFVISITSOPINSTANCEUID,
	    DICOM_SOPCLASSDETACHEDVISITMGMT, &(p.RVSequenceCount),
	sizeof(SQ_REFVISITSOPINSTANCEUID)},
	{SQ_K_REFRESULTSSOPINSTANCEUID,
	    DICOM_SOPCLASSDETACHEDRESULTSMGMT, &(p.RRSequenceCount),
	sizeof(SQ_REFRESULTSSOPINSTANCEUID)},
	{SQ_K_PROCEDURECODE, "",
	&(p.PCSequenceCount), sizeof(SQ_REQPROCEDURECODE)},
	{SQ_K_REQPROCEDURECODE, "",
	&(p.RPCSequenceCount), sizeof(SQ_REQPROCEDURECODE)},
	{SQ_K_REFSTUDYCOMPONENT, "",
	&(p.RSCSequenceCount), sizeof(SQ_REFSTUDYCOMPONENT)},
    };

    l = LST_Create();
    if (l == NULL)
	return -1;

    for (i = 0; i < (int) DIM_OF(sops) && sops[i].type != type; i++);
    if (i >= (int) DIM_OF(sops))
	return -1;

    while (index < *(sops[i].counter)) {
	if ((pointer = malloc(sops[i].size)) == NULL)
	    return -1;
	switch (type) {
	case SQ_K_REFPATIENTSOPINSTANCEUID:
	    patient = (SQ_REFPATIENTSOPINSTANCEUID *) pointer;
	    patient->type = type;
	    sprintf(patient->referencedSOPClassUID, "%s", sops[i].sopclass);
	    sprintf(patient->referencedSOPInstanceUID, "%s",
		    p.RPSequence[index]);
	    break;
	case SQ_K_REFVISITSOPINSTANCEUID:
	    visit = (SQ_REFVISITSOPINSTANCEUID *) pointer;
	    visit->type = type;
	    sprintf(visit->referencedSOPClassUID, "%s", sops[i].sopclass);
	    sprintf(visit->referencedSOPInstanceUID, "%s", p.RVSequence[index]);
	    break;
	case SQ_K_REFRESULTSSOPINSTANCEUID:
	    results = (SQ_REFRESULTSSOPINSTANCEUID *) pointer;
	    results->type = type;
	    sprintf(results->referencedSOPClassUID, "%s", sops[i].sopclass);
	    sprintf(results->referencedSOPInstanceUID, "%s",
		    p.RRSequence[index]);
	    break;
	case SQ_K_PROCEDURECODE:
	    code = (SQ_REQPROCEDURECODE *) pointer;
	    code->type = type;
	    sprintf(code->codeValue, "%s", p.PCSequence[index].CodeValue);
	    sprintf(code->codingSchemeDesignator, "%s",
		    p.PCSequence[index].CodingSchemeDesignator);
	    sprintf(code->codeMeaning, "%s", p.PCSequence[index].CodeMeaning);
	    break;
	case SQ_K_REQPROCEDURECODE:
	    code = (SQ_REQPROCEDURECODE *) pointer;
	    code->type = type;
	    sprintf(code->codeValue, "%s", p.RPCSequence[index].CodeValue);
	    sprintf(code->codingSchemeDesignator, "%s",
		    p.RPCSequence[index].CodingSchemeDesignator);
	    sprintf(code->codeMeaning, "%s", p.RPCSequence[index].CodeMeaning);
	    break;
	case SQ_K_REFSTUDYCOMPONENT:
	    component = (SQ_REFSTUDYCOMPONENT *) pointer;
	    component->type = type;
	    sprintf(component->refSOPClassUID, "%s",
		    p.RSCRSCUIDSequence[index]);
	    sprintf(component->refSOPInstanceUID, "%s",
		    p.RSCRSIUIDSequence[index]);
	    break;
	}
	if (LST_Enqueue(&l, pointer) != LST_NORMAL)
	    return -2;
	index++;
    }
    cond = SQ_BuildSequence(&l, sops[i].type, &e);
    if (cond != SQ_NORMAL) {
	COND_DumpConditions();
	fprintf(stderr, "SQ_BuildSequence failed \n");
	return -1;
    }
    cond = DCM_AddElement(object, e);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	fprintf(stderr, "DCM_AddElement failed \n");
	return -1;
    }
    return 0;

/*
    if (type == REFPATIENTSQ) {
	while (index < p.RPSequenceCount) {
	    patient = malloc(sizeof(*patient));
	    if (patient == NULL)
		return -1;
	    patient->type = SQ_K_REFPATIENTSOPINSTANCEUID;
	    sprintf(patient->referencedSOPClassUID, "%s",
		    DICOM_SOPCLASSDETACHEDPATIENTMGMT);
	    sprintf(patient->referencedSOPInstanceUID, "%s",
		    p.RPSequence[index]);
	    LST_Enqueue(&l, patient);
	    index++;
	}

	cond = SQ_BuildSequence(&l, SQ_K_REFPATIENTSOPINSTANCEUID, &e);
	if (cond != SQ_NORMAL) {
	    COND_DumpConditions();
	    fprintf(stderr, "SQ_BuildSequence failed \n");
	    return -1;
	}
	cond = DCM_AddElement(object, e);
	if (cond != DCM_NORMAL) {
	    COND_DumpConditions();
	    fprintf(stderr, "DCM_AddElement failed \n");
	    return -1;
	}
	return 0;
    }
    if (type == REFVISITSQ) {
	while (index < p.RVSequenceCount) {
	    visit = malloc(sizeof(*visit));
	    if (visit == NULL)
		return -1;
	    visit->type = SQ_K_REFVISITSOPINSTANCEUID;
	    sprintf(visit->referencedSOPClassUID, "%s",
		    DICOM_SOPCLASSDETACHEDVISITMGMT);
	    sprintf(visit->referencedSOPInstanceUID, "%s", p.RVSequence[index]);
	    LST_Enqueue(&l, visit);
	    index++;
	}

	cond = SQ_BuildSequence(&l, SQ_K_REFVISITSOPINSTANCEUID, &e);
	if (cond != SQ_NORMAL) {
	    COND_DumpConditions();
	    fprintf(stderr, "SQ_BuildSequence failed \n");
	    return -1;
	}
	cond = DCM_AddElement(object, e);
	if (cond != DCM_NORMAL) {
	    COND_DumpConditions();
	    fprintf(stderr, "DCM_AddElement failed \n");
	    return -1;
	}
	return 1;
    }
    if (type == REFRESULTSSQ) {
	while (index < p.RRSequenceCount) {
	    results = malloc(sizeof(*results));
	    if (results == NULL)
		return -1;
	    results->type = SQ_K_REFRESULTSSOPINSTANCEUID;
	    sprintf(results->referencedSOPClassUID, "%s",
		    DICOM_SOPCLASSDETACHEDRESULTSMGMT);
	    sprintf(results->referencedSOPInstanceUID, "%s", p.RRSequence[index]);
	    LST_Enqueue(&l, results);
	    index++;
	}

	cond = SQ_BuildSequence(&l, SQ_K_REFRESULTSSOPINSTANCEUID, &e);
	if (cond != SQ_NORMAL) {
	    COND_DumpConditions();
	    fprintf(stderr, "SQ_BuildSequence failed \n");
	    return -1;
	}
	cond = DCM_AddElement(object, e);
	if (cond != DCM_NORMAL) {
	    COND_DumpConditions();
	    fprintf(stderr, "DCM_AddElement failed \n");
	    return -1;
	}
	return 1;
    }
*/
}

/*
static int
sEmptySequence(DCM_OBJECT ** object, SQ_TYPE type)
{
    LST_HEAD *l;
    DCM_ELEMENT *e;
    CONDITION cond;
    SQ_REFPATIENTSOPINSTANCEUID *patient;
    SQ_REFVISITSOPINSTANCEUID *visit;
    SQ_REFRESULTSSOPINSTANCEUID *results;

    l = LST_Create();
    if (l == NULL)
	return -1;

    switch (type) {
    case SQ_K_REQPROCEDURECODE:
    case SQ_K_REFSTUDYCOMPONENT:
	cond = SQ_BuildSequence(&l, type, &e);
	if (cond != SQ_NORMAL) {
	    COND_DumpConditions();
	    fprintf(stderr, "SQ_BuildSequence failed \n");
	    return -1;
	}
	cond = DCM_AddElement(object, e);
	if (cond != DCM_NORMAL) {
	    COND_DumpConditions();
	    fprintf(stderr, "DCM_AddElement failed \n");
	    return -1;
	}
	break;
    default:
	break;
    }
    return 1;
}
*/

typedef struct {
    DCM_TAG tag;
    SQ_TYPE type;
    unsigned long his_field;
}   DCM_MAP_SQ;

static int
s_parseSequence(DCM_OBJECT ** object)
{

    DCM_ELEMENT e;
    LST_HEAD *l;
    CONDITION cond;
    int index;
    SQ_TYPE type;
    SQ_REFPATIENTSOPINSTANCEUID *patient;
    SQ_REFVISITSOPINSTANCEUID *visit;
    SQ_REFRESULTSSOPINSTANCEUID *results;
    SQ_REQPROCEDURECODE *code;
    SQ_REFSTUDYCOMPONENT *component;
    void *pointer;

    static DCM_MAP_SQ sequences[] = {
	{DCM_IDREFERENCEDPATIENTSEQ, SQ_K_REFPATIENTSOPINSTANCEUID,
	HIS_K_STURPSEQUENCE},
	{DCM_IDREFERENCEDVISITSEQ, SQ_K_REFVISITSOPINSTANCEUID,
	HIS_K_STURVSEQUENCE},
	{DCM_IDREFERENCEDRESULTSSEQ, SQ_K_REFRESULTSSOPINSTANCEUID,
	HIS_K_STURRSEQUENCE},
	{DCM_IDPROCEDURECODESEQUENCE, SQ_K_PROCEDURECODE,
	HIS_K_STUPCSEQUENCE},
	{DCM_SDYREQUESTEDPROCODESEQ, SQ_K_REQPROCEDURECODE,
	HIS_K_STURPCSEQUENCE},
	{DCM_IDREFERENCEDSTUDYCOMPONENTSEQ, SQ_K_REFSTUDYCOMPONENT,
	HIS_K_STURSCSEQUENCE},
    };

    for (index = 0; index < (int) DIM_OF(sequences); index++) {
	e.tag = sequences[index].tag;
	cond = DCM_GetSequenceList(object, e.tag, &e.d.sq);
	if (cond != DCM_NORMAL) {
	    if (cond != DCM_ELEMENTNOTFOUND)
		return -1;
	} else {
	    cond = SQ_ParseSequence(&e, &type, &l);
	    if (cond != SQ_NORMAL || type != sequences[index].type)
		return -2;
	    p.Update_Flag |= sequences[index].his_field;
	    pointer = LST_Head(&l);
	    if (pointer != NULL)
		LST_Position(&l, pointer);
	    while (pointer != NULL) {
		switch (sequences[index].tag) {
		case DCM_IDREFERENCEDPATIENTSEQ:
		    patient = (SQ_REFPATIENTSOPINSTANCEUID *) pointer;
		    strcpy(p.RPSequence[p.RPSequenceCount],
			   patient->referencedSOPInstanceUID);
		    ++p.RPSequenceCount;
		    break;
		case DCM_IDREFERENCEDVISITSEQ:
		    visit = (SQ_REFVISITSOPINSTANCEUID *) pointer;
		    strcpy(p.RVSequence[p.RVSequenceCount],
			   visit->referencedSOPInstanceUID);
		    ++p.RVSequenceCount;
		    break;
		case DCM_IDREFERENCEDRESULTSSEQ:
		    results = (SQ_REFRESULTSSOPINSTANCEUID *) pointer;
		    strcpy(p.RRSequence[p.RRSequenceCount],
			   results->referencedSOPInstanceUID);
		    ++p.RRSequenceCount;
		    break;
		case DCM_IDPROCEDURECODESEQUENCE:
		    code = (SQ_REQPROCEDURECODE *) pointer;
		    strcpy(p.PCSequence[p.PCSequenceCount].CodeValue,
			   code->codeValue);
		    strcpy(p.PCSequence[p.PCSequenceCount].
		      CodingSchemeDesignator, code->codingSchemeDesignator);
		    strcpy(p.PCSequence[p.PCSequenceCount].CodeMeaning,
			   code->codeMeaning);
		    p.PCSequenceCount++;
		    break;
		case DCM_SDYREQUESTEDPROCODESEQ:
		    code = (SQ_REQPROCEDURECODE *) pointer;
		    strcpy(p.RPCSequence[p.RPCSequenceCount].CodeValue,
			   code->codeValue);
		    strcpy(p.RPCSequence[p.RPCSequenceCount].
		      CodingSchemeDesignator, code->codingSchemeDesignator);
		    strcpy(p.RPCSequence[p.RPCSequenceCount].CodeMeaning,
			   code->codeMeaning);
		    p.RPCSequenceCount++;
		    break;
		case DCM_IDREFERENCEDSTUDYCOMPONENTSEQ:
		    component = (SQ_REFSTUDYCOMPONENT *) pointer;
		    strcpy(p.RSCRSIUIDSequence[p.RSCSequenceCount],
			   component->refSOPInstanceUID);
		    strcpy(p.RSCRSCUIDSequence[p.RSCSequenceCount],
			   component->refSOPClassUID);
		    p.RSCSequenceCount++;
		    break;
		}
		pointer = LST_Next(&l);
	    }
	}
    }
    return 0;
/*
    e[0].tag = DCM_IDREFERENCEDPATIENTSEQ;
    e[1].tag = DCM_IDREFERENCEDVISITSEQ;
    e[2].tag = DCM_IDREFERENCEDRESULTSSEQ;

    for (index = 0; index < (int) DIM_OF(e); index++) {
	cond = DCM_GetSequenceList(object, e[index].tag, &e[index].d.sq);
	if (cond == DCM_ELEMENTNOTFOUND)
	    continue;
	else if (cond != DCM_NORMAL)
	    return -1;

	cond = SQ_ParseSequence(&e[index], &type, &l);
	if (cond != SQ_NORMAL)
	    return -2;

	if (type == SQ_K_REFPATIENTSOPINSTANCEUID) {
	    p.Update_Flag |= HIS_K_STURPSEQUENCE;
	    patient = LST_Head(&l);
	    if (patient != NULL)
		LST_Position(&l, patient);
	    while (patient != NULL) {
		strcpy(p.RPSequence[p.RPSequenceCount],
		       patient->referencedSOPInstanceUID);
		++p.RPSequenceCount;
		patient = LST_Next(&l);
	    }
	}
	if (type == SQ_K_REFVISITSOPINSTANCEUID) {
	    p.Update_Flag |= HIS_K_STURVSEQUENCE;
	    visit = LST_Head(&l);
	    if (visit != NULL)
		LST_Position(&l, visit);
	    while (visit != NULL) {
		strcpy(p.RVSequence[p.RVSequenceCount],
		       visit->referencedSOPInstanceUID);
		++p.RVSequenceCount;
		visit = LST_Next(&l);
	    }
	}
	if (type == SQ_K_REFRESULTSSOPINSTANCEUID) {
	    p.Update_Flag2 |= HIS_K_STURRSEQUENCE;
	    results = LST_Head(&l);
	    if (results != NULL)
		LST_Position(&l, results);
	    while (results != NULL) {
		strcpy(p.RRSequence[p.RRSequenceCount],
		       results->referencedSOPInstanceUID);
		++p.RRSequenceCount;
		results = LST_Next(&l);
	    }
	}
    }

    return 0;
*/
}
