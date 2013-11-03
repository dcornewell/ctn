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
** Module Name(s):	HAP_BuildPatient
**			HAP_ParstPatient
** Author, Date:	John T. O'Neill, 14-June-93
** Intent:              This module translates a HIS level structure to a
**                      DCM_OBJECT or a DCM_OBJECT to a HIS level structure.
** Last Update:         $Author: smm $, $Date: 1995-04-07 20:45:02 $
** Source File:         $RCSfile: hapbuildpatient.c,v $
** Revision:            $Revision: 1.16 $
** Status:              $State: Exp $
*/

static char rcsid[] = "$Revision: 1.16 $ $RCSfile: hapbuildpatient.c,v $";

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

#define HAP_X_VARIABLE_EVENT	HAP_P_VARIABLE_EVENT

static HIS_PatientLevel
    p;

static DCM_FLAGGED_ELEMENT list[] = {
    {DCM_IDINSTANCECREATEDATE, DCM_DA, "", 1,
	sizeof p.ICDate, (void *) &p.ICDate[0],
    HIS_K_PATICDATE, &p.Update_Flag},
    {DCM_IDINSTANCECREATETIME, DCM_TM, "", 1,
	sizeof p.ICTime, (void *) &p.ICTime[0],
    HIS_K_PATICTIME, &p.Update_Flag},
    {DCM_IDINSTANCECREATORUID, DCM_UI, "", 1,
	sizeof p.ICUID, (void *) &p.ICUID[0],
    HIS_K_PATICUID, &p.Update_Flag},
    {DCM_PATNAME, DCM_PN, "", 1,
	sizeof p.PName, (void *) &p.PName[0],
    HIS_K_PATPNAME, &p.Update_Flag},
    {DCM_PATID, DCM_LO, "", 1,
	sizeof p.PID, (void *) &p.PID[0],
    HIS_K_PATPID, &p.Update_Flag},
    {DCM_PATBIRTHDATE, DCM_DA, "", 1,
	sizeof p.PBDate, (void *) &p.PBDate[0],
    HIS_K_PATPBDATE, &p.Update_Flag},
    {DCM_PATSEX, DCM_CS, "", 1,
	sizeof p.PSex, (void *) &p.PSex[0],
    HIS_K_PATPSEX, &p.Update_Flag},
    {DCM_ISSUERPATIENTID, DCM_LO, "", 1,
	sizeof p.IoPID, (void *) &p.IoPID[0],
    HIS_K_PATIOPID, &p.Update_Flag},
    {DCM_PATOTHERIDS, DCM_LO, "", 1,
	sizeof p.OPIDs, (void *) &p.OPIDs[0],
    HIS_K_PATOPIDS, &p.Update_Flag},
    {DCM_PATOTHERNAMES, DCM_PN, "", 1,
	sizeof p.OPNames, (void *) &p.OPNames[0],
    HIS_K_PATOPNAMES, &p.Update_Flag},
    {DCM_PATADDRESS, DCM_LO, "", 1,
	sizeof p.PAddress, (void *) &p.PAddress[0],
    HIS_K_PATPADDRESS, &p.Update_Flag},
    {DCM_PATTELEPHONENUMBER, DCM_SH, "", 1,
	sizeof p.PTNumbers, (void *) &p.PTNumbers[0],
    HIS_K_PATPTNUMBERS, &p.Update_Flag},
#if STANDARD_VERSION < VERSION_JUL1993
    {DCM_PATRACE, DCM_SH, "", 1,
#else
    {DCM_PATETHNICGROUP, DCM_SH, "", 1,
#endif
	sizeof p.EGroup, (void *) &p.EGroup[0],
    HIS_K_PATEGROUP, &p.Update_Flag},
    {DCM_PATRELIGIOUSPREFERENCE, DCM_LO, "", 1,
	sizeof p.RPreference, (void *) &p.RPreference[0],
    HIS_K_PATRPREFERENCE, &p.Update_Flag},
    {DCM_PATCONTRASTALLERGIES, DCM_LO, "", 1,
	sizeof p.CAllergies, (void *) &p.CAllergies[0],
    HIS_K_PATCALLERGIES, &p.Update_Flag}
};

typedef struct {
    HAP_PATIENT_EVENT event;
    long required_event_attrib;
}   EVENT_MAP;

static EVENT_MAP array[] = {
    {HAP_PATIENT_CREATED, HAP_REQUIRED_PCREATED_ATTRIB},
    {HAP_PATIENT_UPDATED, HAP_REQUIRED_PUPDATED_ATTRIB},
    {HAP_PATIENT_DELETED, HAP_REQUIRED_PDELETED_ATTRIB}
};

int p_parseSequence(DCM_OBJECT ** object);
int p_buildSequence(DCM_OBJECT ** object, SQ_TYPE type);

/* HAP_BuildPatient
**
** Purpose:
**      HAP_BuildPatient adds attributes to the DCM_OBJECT passed to it. It
**      gets the attribute values from the HIS_PatientLevel structure.
**      It knows which attributes from within the structure to add from the
**      Update_Flag element of the HIS_InterpretationLevel structure. The
**      event argument should be either 0 or an enum constant of
**      type HAP_PATIENT_EVENT. If it is a enum constant, the function checks
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
HAP_BuildPatient(DCM_OBJECT ** object,
		 HIS_PatientLevel * xl, HAP_PATIENT_EVENT event)
{
    CONDITION
	cond;
    int
        index,
        x;
    CTNBOOLEAN
	eventFound = FALSE;

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

    if (p.Update_Flag & HIS_K_PATRVSEQUENCE) {
	x = p_buildSequence(object, SQ_K_REFVISITSOPINSTANCEUID);
	if (x < 0)
	    return 0;
    }
    if (p.Update_Flag & HIS_K_PATRSSEQUENCE) {
	x = p_buildSequence(object, SQ_K_REFSTUDYSOPINSTANCEUID);
	if (x < 0)
	    return 0;
    }
    return HAP_NORMAL;
}

/* HAP_ParsePatient
**
** Purpose:
**      HAP_ParsePatient extracts from a DCM_OBJECT all the attributes present
**      that are specified in the Detached Patient Management Service
**      Class. The attibute values are placed in the HIS_PatientLevel
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
HAP_ParsePatient(DCM_OBJECT ** object, HIS_PatientLevel * xl)
{

    CONDITION cond;
    int x;

    memset(&p, 0, sizeof p);

    cond = DCM_ParseObject(object, NULL, 0, list, (int) DIM_OF(list), NULL);

    if (cond != DCM_NORMAL)
	return HAP_ILLEGALOBJECT;

    x = p_parseSequence(object);
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

int
p_buildSequence(DCM_OBJECT ** object, SQ_TYPE type)
{

    int index = 0;
    int i;
    LST_HEAD *l;
    DCM_ELEMENT *e;
    CONDITION cond;
    SQ_REFVISITSOPINSTANCEUID *visit;
    SQ_REFSTUDYSOPINSTANCEUID *study;
    void *pointer;

    static SQ_MAP_SOP sops[] = {
	{SQ_K_REFVISITSOPINSTANCEUID,
	    DICOM_SOPCLASSDETACHEDVISITMGMT, &(p.RVSequenceCount),
	sizeof(SQ_REFVISITSOPINSTANCEUID)},
	{SQ_K_REFSTUDYSOPINSTANCEUID,
	    DICOM_SOPCLASSDETACHEDSTUDYMGMT, &(p.RSSequenceCount),
	sizeof(SQ_REFSTUDYSOPINSTANCEUID)},
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
	case SQ_K_REFVISITSOPINSTANCEUID:
	    visit = (SQ_REFVISITSOPINSTANCEUID *) pointer;
	    visit->type = type;
	    sprintf(visit->referencedSOPClassUID, "%s", sops[i].sopclass);
	    sprintf(visit->referencedSOPInstanceUID, "%s", p.RVSequence[index]);
	    break;
	case SQ_K_REFSTUDYSOPINSTANCEUID:
	    study = (SQ_REFSTUDYSOPINSTANCEUID *) pointer;
	    study->type = type;
	    sprintf(study->referencedSOPClassUID, "%s", sops[i].sopclass);
	    sprintf(study->referencedSOPInstanceUID, "%s", p.RSSequence[index]);
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
    if (!strcmp(DICOM_SOPCLASSDETACHEDVISITMGMT, sopclass)) {
	while (index < p.RVSequenceCount) {
	    visit = malloc(sizeof(*visit));
	    if (visit == NULL)
		return -1;
	    visit->type = SQ_K_REFVISITSOPINSTANCEUID;
	    sprintf(visit->referencedSOPClassUID, "%s", sopclass);
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
    if (!strcmp(DICOM_SOPCLASSDETACHEDSTUDYMGMT, sopclass)) {
	while (index < p.RSSequenceCount) {
	    study = malloc(sizeof(*study));
	    if (study == NULL)
		return -1;
	    study->type = SQ_K_REFSTUDYSOPINSTANCEUID;
	    sprintf(study->referencedSOPClassUID, "%s", sopclass);
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
p_parseSequence(DCM_OBJECT ** object)
{

    DCM_ELEMENT e;
    LST_HEAD *l;
    CONDITION cond;
    int index;
    SQ_TYPE type;
    SQ_REFVISITSOPINSTANCEUID *visit;
    SQ_REFSTUDYSOPINSTANCEUID *study;
    void *pointer;

    static DCM_MAP_SQ sequences[] = {
	{DCM_IDREFERENCEDVISITSEQ, SQ_K_REFVISITSOPINSTANCEUID,
	HIS_K_PATRVSEQUENCE},
	{DCM_IDREFERENCEDSTUDYSEQ, SQ_K_REFSTUDYSOPINSTANCEUID,
	HIS_K_PATRSSEQUENCE},
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
		case DCM_IDREFERENCEDVISITSEQ:
		    visit = (SQ_REFVISITSOPINSTANCEUID *) pointer;
		    strcpy(p.RVSequence[p.RVSequenceCount],
			   visit->referencedSOPInstanceUID);
		    ++p.RVSequenceCount;
		    break;
		case DCM_IDREFERENCEDSTUDYSEQ:
		    study = (SQ_REFSTUDYSOPINSTANCEUID *) pointer;
		    strcpy(p.RSSequence[p.RSSequenceCount],
			   study->referencedSOPInstanceUID);
		    ++p.RSSequenceCount;
		    break;
		}
		pointer = LST_Next(&l);
	    }
	}
    }
    return 0;
/*
    e[0].tag = DCM_IDREFERENCEDVISITSEQ;
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

	if (type == SQ_K_REFVISITSOPINSTANCEUID) {
	    p.Update_Flag |= HIS_K_PATRVSEQUENCE;
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
	if (type == SQ_K_REFSTUDYSOPINSTANCEUID) {
	    p.Update_Flag |= HIS_K_PATRSSEQUENCE;
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
