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
** Module Name(s):	HAP_BuildVisit
** Author, Date:	John T. O'Neill, 14-June-93
** Intent:              This module translates a HIS level structure to a
**                      DCM_OBJECT or a DCM_OBJECT to a HIS level structure.
** Last Update:         $Author: smm $, $Date: 1995-04-07 20:45:10 $
** Source File:         $RCSfile: hapbuildvisit.c,v $
** Revision:            $Revision: 1.18 $
** Status:              $State: Exp $
*/

static char rcsid[] = "$Revision: 1.18 $ $RCSfile: hapbuildvisit.c,v $";

#define BUILD
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "dicom.h"
#include "lst.h"
#include "dicom_objects.h"
#include "hisdb.h"
#include "hap.h"
#include "dicom_sq.h"
#include "dicom_uids.h"

#define HAP_X_VARIABLE_EVENT	HAP_V_VARIABLE_EVENT

static HIS_VisitLevel
    p;

static DCM_FLAGGED_ELEMENT list[] = {
    {DCM_IDINSTANCECREATEDATE, DCM_DA, "", 1,
	sizeof p.ICDate, (void *) &p.ICDate[0],
    HIS_K_VISICDATE, &p.Update_Flag},
    {DCM_IDINSTANCECREATETIME, DCM_TM, "", 1,
	sizeof p.ICTime, (void *) &p.ICTime[0],
    HIS_K_VISICTIME, &p.Update_Flag},
    {DCM_IDINSTANCECREATORUID, DCM_UI, "", 1,
	sizeof p.ICUID, (void *) &p.ICUID[0],
    HIS_K_VISICUID, &p.Update_Flag},
    {DCM_VISSTATUSID, DCM_CS, "", 1,
	sizeof p.VSID, (void *) &p.VSID[0],
    HIS_K_VISVSID, &p.Update_Flag},
    {DCM_VISADMISSIONID, DCM_LO, "", 1,
	sizeof p.AID, (void *) &p.AID[0],
    HIS_K_VISAID, &p.Update_Flag},
    {DCM_VISSCHEDULEDADMISSIONDATE, DCM_DA, "", 1,
	sizeof p.SADate, (void *) &p.SADate[0],
    HIS_K_VISSADATE, &p.Update_Flag},
    {DCM_VISSCHEDULEDADMISSIONTIME, DCM_TM, "", 1,
	sizeof p.SATime, (void *) &p.SATime[0],
    HIS_K_VISSATIME, &p.Update_Flag},
    {DCM_VISSCHEDULEDPATINSTRESIDENCE, DCM_LO, "", 1,
	sizeof p.SPIResidence, (void *) &p.SPIResidence[0],
    HIS_K_VISSPIRESIDENCE, &p.Update_Flag},
    {DCM_VISADMITTINGDATE, DCM_DA, "", 1,
	sizeof p.ADate, (void *) &p.ADate[0],
    HIS_K_VISADATE, &p.Update_Flag},
    {DCM_VISADMITTINGTIME, DCM_TM, "", 1,
	sizeof p.ATime, (void *) &p.ATime[0],
    HIS_K_VISATIME, &p.Update_Flag},
    {DCM_VISROUTEOFADMISSION, DCM_LO, "", 1,
	sizeof p.RoAdmissions, (void *) &p.RoAdmissions[0],
    HIS_K_VISROADMISSIONS, &p.Update_Flag},

    {DCM_IDREFERRINGPHYSICIAN, DCM_PN, "", 1, sizeof p.RPhysician,
    (void *) &p.RPhysician[0], HIS_K_VISRPHYSICIAN, &p.Update_Flag},

    {DCM_VISPATIENTSINSTRESIDENCE, DCM_LO, "", 1,
	sizeof p.PIResidence, (void *) &p.PIResidence[0],
    HIS_K_VISPIRESIDENCE, &p.Update_Flag},
    {DCM_VISCURRENTPATIENTLOCATION, DCM_LO, "", 1,
	sizeof p.CPLocation, (void *) &p.CPLocation[0],
    HIS_K_VISCPLOCATION, &p.Update_Flag},
    {DCM_VISDISCHARGEDATE, DCM_DA, "", 1,
	sizeof p.DDate, (void *) &p.DDate[0],
    HIS_K_VISDDATE, &p.Update_Flag},
    {DCM_VISDISCHARGETIME, DCM_TM, "", 1,
	sizeof p.DTime, (void *) &p.DTime[0],
    HIS_K_VISDTIME, &p.Update_Flag},
    {DCM_IDINSTITUTIONNAME, DCM_LO, "", 1,
	sizeof p.IIDName, (void *) &p.IIDName[0],
    HIS_K_VISIIDNAME, &p.Update_Flag},
    {DCM_IDADMITTINGDIAGDESCR, DCM_LO, "", 1,
	sizeof p.ADDescription, (void *) &p.ADDescription[0],
    HIS_K_VISADDESCRIPTION, &p.Update_Flag},
    {DCM_VISDISCHARGEDIAGDESCRIPTION, DCM_LO, "", 1,
	sizeof p.DDDescription, (void *) &p.DDDescription[0],
    HIS_K_VISDDDESCRIPTION, &p.Update_Flag},
/* {DCM_VISCOMMENTS, DCM_LT, "", 1,
 sizeof p.comments, (void *) p.comments,
 V_COMMENTS, &p.Update_Flag} */
};

typedef struct {
    HAP_VISIT_EVENT event;
    long required_event_attrib;
}   EVENT_MAP;

static EVENT_MAP array[] = {
    {HAP_VISIT_CREATED, HAP_REQUIRED_VCREATED_ATTRIB},
    {HAP_VISIT_UPDATED, HAP_REQUIRED_VUPDATED_ATTRIB},
    {HAP_VISIT_DELETED, HAP_REQUIRED_VDELETED_ATTRIB},
    {HAP_VISIT_SCHEDULED, HAP_REQUIRED_VSCHEDULED_ATTRIB},
    {HAP_PATIENT_ADMITTED, HAP_REQUIRED_VADMITTED_ATTRIB},
    {HAP_PATIENT_TRANSFERRED, HAP_REQUIRED_VTRANSFERRED_ATTRIB},
    {HAP_PATIENT_DISCHARGED, HAP_REQUIRED_VDISCHARGED_ATTRIB}
};

int v_buildSequence(DCM_OBJECT ** object, SQ_TYPE type);
int v_parseSequence(DCM_OBJECT ** object);

/* HAP_BuildVisit
**
** Purpose:
**      HAP_BuildVisit adds attributes to the DCM_OBJECT passed to it. It
**      gets the attribute values from the HIS_VisitLevel structure.
**      It knows which attributes from within the structure to add from the
**      Update_Flag element of the HIS_InterpretationLevel structure. The
**      event argument should be either 0 or an enum constant of
**      type HAP_VISIT_EVENT. If it is a enum constant, the function checks
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
HAP_BuildVisit(DCM_OBJECT ** object,
	       HIS_VisitLevel * xl, HAP_VISIT_EVENT event)
{

    int index,
        x;

    CONDITION
	cond;

    CTNBOOLEAN
	eventFound = FALSE;

    static unsigned long his_flags[] =
    {HIS_K_VISRPSEQUENCE, HIS_K_VISRSSEQUENCE,
    HIS_K_VISADCSEQUENCE, HIS_K_VISDDCSEQUENCE};
    static SQ_TYPE types[] =
    {SQ_K_REFPATIENTSOPINSTANCEUID, SQ_K_REFSTUDYSOPINSTANCEUID,
    SQ_K_ADMITDIAGNOSISCODE, SQ_K_DISCHARGEDIAGNOSISCODE};

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
	    x = v_buildSequence(object, types[index]);
	    if (x < 0)
		return 0;	/* needs repair */
	}
/*
    if (p.Update_Flag & HIS_K_VISRPSEQUENCE) {
	x = v_buildSequence(object, SQ_K_REFPATIENTSOPINSTANCEUID);
	if (x < 0)
	    return 0;
    }
    if (p.Update_Flag & HIS_K_VISRSSEQUENCE) {
	x = v_buildSequence(object, SQ_K_REFSTUDYSOPINSTANCEUID);
	if (x < 0)
	    return 0;
    }
*/
    return HAP_NORMAL;
}

/* HAP_ParseVisit
**
** Purpose:
**      HAP_ParseVisit extracts from a DCM_OBJECT all the attributes present
**      that are specified in the Detached Visit Management Service
**      Class. The attibute values are placed in the HIS_VisitLevel
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
HAP_ParseVisit(DCM_OBJECT ** object, HIS_VisitLevel * xl)
{

    CONDITION cond;
    int x;

    memset(&p, 0, sizeof p);

    cond = DCM_ParseObject(object, NULL, 0, list, (int) DIM_OF(list), NULL);

    if (cond != DCM_NORMAL)
	return HAP_ILLEGALOBJECT;

    x = v_parseSequence(object);
    if (x < 0)
	return 0;		/* needs repair */

    *xl = p;

    return HAP_NORMAL;
}

typedef struct {
    SQ_TYPE type;
    char *sopclass;
    long *counter;
    long size;
}   SQ_MAP_SOP;

int
v_buildSequence(DCM_OBJECT ** object, SQ_TYPE type)
{

    int index = 0;
    int i;
    LST_HEAD *l;
    DCM_ELEMENT *e;
    CONDITION cond;
    SQ_REFPATIENTSOPINSTANCEUID *patient;
    SQ_REFSTUDYSOPINSTANCEUID *study;
    SQ_REQPROCEDURECODE *code;
    void *pointer;

    static SQ_MAP_SOP sops[] = {
	{SQ_K_REFPATIENTSOPINSTANCEUID,
	    DICOM_SOPCLASSDETACHEDPATIENTMGMT, &(p.RPSequenceCount),
	sizeof(SQ_REFPATIENTSOPINSTANCEUID)},
	{SQ_K_REFSTUDYSOPINSTANCEUID,
	    DICOM_SOPCLASSDETACHEDSTUDYMGMT, &(p.RSSequenceCount),
	sizeof(SQ_REFSTUDYSOPINSTANCEUID)},
	{SQ_K_ADMITDIAGNOSISCODE,
	    "", &(p.ADCSequenceCount),
	sizeof(SQ_REQPROCEDURECODE)},
	{SQ_K_DISCHARGEDIAGNOSISCODE,
	    "", &(p.DDCSequenceCount),
	sizeof(SQ_REQPROCEDURECODE)},
    };

    l = LST_Create();
    if (l == NULL)
	return -1;

    for (i = 0; i < (int) DIM_OF(sops) && sops[i].type != type;
	 i++);
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
	case SQ_K_REFSTUDYSOPINSTANCEUID:
	    study = (SQ_REFSTUDYSOPINSTANCEUID *) pointer;
	    study->type = type;
	    sprintf(study->referencedSOPClassUID, "%s", sops[i].sopclass);
	    sprintf(study->referencedSOPInstanceUID, "%s", p.RSSequence[index]);
	    break;
	case SQ_K_ADMITDIAGNOSISCODE:
	    code = (SQ_REQPROCEDURECODE *) pointer;
	    code->type = type;
	    strcpy(code->codeValue, p.ADCSequence[index].CodeValue);
	    strcpy(code->codingSchemeDesignator,
		   p.ADCSequence[index].CodingSchemeDesignator);
	    strcpy(code->codeMeaning, p.ADCSequence[index].CodeMeaning);
	    break;
	case SQ_K_DISCHARGEDIAGNOSISCODE:
	    code = (SQ_REQPROCEDURECODE *) pointer;
	    code->type = type;
	    strcpy(code->codeValue, p.DDCSequence[index].CodeValue);
	    strcpy(code->codingSchemeDesignator,
		   p.DDCSequence[index].CodingSchemeDesignator);
	    strcpy(code->codeMeaning, p.DDCSequence[index].CodeMeaning);
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
	    sprintf(patient->referencedSOPClassUID, "%s", DICOM_SOPCLASSDETACHEDPATIENTMGMT);
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
    if (type == REFSTUDYSQ) {
	while (index < p.RSSequenceCount) {
	    study = malloc(sizeof(*study));
	    if (study == NULL)
		return -1;
	    study->type = SQ_K_REFSTUDYSOPINSTANCEUID;
	    sprintf(study->referencedSOPClassUID, "%s", DICOM_SOPCLASSDETACHEDSTUDYMGMT);
	    sprintf(study->referencedSOPInstanceUID, "%s", p.RSSequence[index]);
	    LST_Enqueue(&l, study);
	    index++;
	}

	cond = SQ_BuildSequence(&l, SQ_K_REFSTUDYSOPINSTANCEUID, &e);
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

typedef struct {
    DCM_TAG tag;
    SQ_TYPE type;
    unsigned long his_field;
}   DCM_MAP_SQ;

int
v_parseSequence(DCM_OBJECT ** object)
{

    DCM_ELEMENT e;
    LST_HEAD *l;
    CONDITION cond;
    int index;
    SQ_TYPE type;
    SQ_REFPATIENTSOPINSTANCEUID *patient;
    SQ_REFSTUDYSOPINSTANCEUID *study;
    SQ_REQPROCEDURECODE *code;
    void *pointer;

    static DCM_MAP_SQ sequences[] = {
	{DCM_IDREFERENCEDPATIENTSEQ, SQ_K_REFPATIENTSOPINSTANCEUID,
	HIS_K_VISRPSEQUENCE},
	{DCM_IDREFERENCEDSTUDYSEQ, SQ_K_REFSTUDYSOPINSTANCEUID,
	HIS_K_VISRSSEQUENCE},
	{DCM_IDADMITDIAGCODESEQUENCE, SQ_K_ADMITDIAGNOSISCODE,
	HIS_K_VISADCSEQUENCE},
	{DCM_VISDISCHARGEDIAGNOSISCODESEQ, SQ_K_DISCHARGEDIAGNOSISCODE,
	HIS_K_VISDDCSEQUENCE},
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
		case DCM_IDREFERENCEDSTUDYSEQ:
		    study = (SQ_REFSTUDYSOPINSTANCEUID *) pointer;
		    strcpy(p.RSSequence[p.RSSequenceCount],
			   study->referencedSOPInstanceUID);
		    ++p.RSSequenceCount;
		    break;
		case DCM_IDADMITDIAGCODESEQUENCE:
		    code = (SQ_REQPROCEDURECODE *) pointer;
		    strcpy(p.ADCSequence[p.ADCSequenceCount].CodeValue,
			   code->codeValue);
		    strcpy(p.ADCSequence[p.ADCSequenceCount].
		      CodingSchemeDesignator, code->codingSchemeDesignator);
		    strcpy(p.ADCSequence[p.ADCSequenceCount].CodeMeaning,
			   code->codeMeaning);
		    p.ADCSequenceCount++;
		    break;
		case DCM_VISDISCHARGEDIAGNOSISCODESEQ:
		    code = (SQ_REQPROCEDURECODE *) pointer;
		    strcpy(p.DDCSequence[p.DDCSequenceCount].CodeValue,
			   code->codeValue);
		    strcpy(p.DDCSequence[p.DDCSequenceCount].
		      CodingSchemeDesignator, code->codingSchemeDesignator);
		    strcpy(p.DDCSequence[p.DDCSequenceCount].CodeMeaning,
			   code->codeMeaning);
		    p.DDCSequenceCount++;
		    break;
		}
		pointer = LST_Next(&l);
	    }
	}
    }
    return 0;

/*
    e[0].tag = DCM_IDREFERENCEDPATIENTSEQ;
    e[1].tag = DCM_IDREFERENCEDSTUDYSEQ;

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
	    p.Update_Flag |= HIS_K_VISRPSEQUENCE;
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
	if (type == SQ_K_REFSTUDYSOPINSTANCEUID) {
	    p.Update_Flag |= HIS_K_VISRSSEQUENCE;
	    study = LST_Head(&l);
	    if (study != NULL)
		LST_Position(&l, study);
	    while (study != NULL) {
		strcpy(p.RSSequence[p.RSSequenceCount],
		       study->referencedSOPInstanceUID);
		++p.RSSequenceCount;
		study = LST_Next(&l);
	    }
	}
    }

    return 0;
*/
}
