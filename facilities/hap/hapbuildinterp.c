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
** Module Name(s):	HAP_BuildInterp
**			HAP_ParseInterp
** Author, Date:	John T. O'Neill, 14-June-93
** Intent:		This module translates a HIS level structure to a
**			DCM_OBJECT or a DCM_OBJECT to a HIS level structure.
** Last Update:         $Author: smm $, $Date: 1995-04-07 20:45:23 $
** Source File:         $RCSfile: hapbuildinterp.c,v $
** Revision:            $Revision: 1.13 $
** Status:              $State: Exp $
*/

static char rcsid[] = "$Revision: 1.13 $ $RCSfile: hapbuildinterp.c,v $";

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

#define HAP_X_VARIABLE_EVENT	HAP_I_VARIABLE_EVENT

static HIS_InterpretationLevel
    p;

static DCM_FLAGGED_ELEMENT list[] = {
    {DCM_IDINSTANCECREATEDATE, DCM_DA, "", 1,
	sizeof p.ICDate, (void *) &p.ICDate[0],
    HIS_K_INTICDATE, &p.Update_Flag},
    {DCM_IDINSTANCECREATETIME, DCM_TM, "", 1,
	sizeof p.ICTime, (void *) &p.ICTime[0],
    HIS_K_INTICTIME, &p.Update_Flag},
    {DCM_IDINSTANCECREATORUID, DCM_UI, "", 1,
	sizeof p.ICUID, (void *) &p.ICUID[0],
    HIS_K_INTICUID, &p.Update_Flag},
    {DCM_RESINTERPTYPEID, DCM_CS, "", 1,
	sizeof p.ITID, (void *) &p.ITID[0],
    HIS_K_INTITID, &p.Update_Flag},
    {DCM_RESINTERPSTATUSID, DCM_CS, "", 1,
	sizeof p.ISID, (void *) &p.ISID[0],
    HIS_K_INTISID, &p.Update_Flag},
    {DCM_RESINTERPRECORDEDDATE, DCM_DA, "", 1,
	sizeof p.IRDate, (void *) &p.IRDate[0],
    HIS_K_INTIRDATE, &p.Update_Flag},
    {DCM_RESINTERPRECORDEDTIME, DCM_TM, "", 1,
	sizeof p.IRTime, (void *) &p.IRTime[0],
    HIS_K_INTIRTIME, &p.Update_Flag},
    {DCM_RESINTERPRECORDER, DCM_PN, "", 1,
	sizeof p.IRecorder, (void *) &p.IRecorder[0],
    HIS_K_INTIRECORDER, &p.Update_Flag},
    {DCM_RESINTERPTRANSCRIPTIONDATE, DCM_DA, "", 1,
	sizeof p.ITDate, (void *) &p.ITDate[0],
    HIS_K_INTITDATE, &p.Update_Flag},
    {DCM_RESINTERPTRANSCRIPTIONTIME, DCM_TM, "", 1,
	sizeof p.ITTime, (void *) &p.ITTime[0],
    HIS_K_INTITTIME, &p.Update_Flag},
    {DCM_RESINTERPAUTHOR, DCM_PN, "", 1,
	sizeof p.IAuthor, (void *) &p.IAuthor[0],
    HIS_K_INTIAUTHOR, &p.Update_Flag},
/*
    {DCM_RESINTERPAPPROVALDATE, DCM_DA, "", 1,
	sizeof p.IADates, (void *) p.IADates,
    HIS_K_INTIADATES, &p.Update_Flag},
    {DCM_RESINTERPAPPROVALTIME, DCM_TM, "", 1,
	sizeof p.IATimes, (void *) p.IATimes,
    HIS_K_INTIATIMES, &p.Update_Flag},
    {DCM_RESPHYSICIANAPPROVINGINTERP, DCM_PN, "", 1,
	sizeof p.PAInterpretation, (void *) p.PAInterpretation,
    HIS_K_INTPAINTERPRETATION, &p.Update_Flag},
*/
    {DCM_RESDIAGNOSIS, DCM_LT, "", 1,
	sizeof p.IDiagnosis, (void *) &p.IDiagnosis[0],
    HIS_K_INTIDIAGNOSIS, &p.Update_Flag},
#ifdef SMM
    {DCM_RESDIAGNOSISCODINGSCHEMEUID, DCM_UI, "", 1,
	sizeof p.IDCSUID, (void *) &p.IDCSUID[0],
    HIS_K_INTIDCSUID, &p.Update_Flag},
#endif
#ifdef SMM
    {DCM_RESDIAGNOSISCODE, DCM_SH, "", 1,
	sizeof p.IDCodes, (void *) &p.IDCodes[0],
    HIS_K_INTIDCODES, &p.Update_Flag},
#endif
    {DCM_RESINTERPTEXT, DCM_ST, "", 1,
	sizeof p.IText, (void *) &p.IText[0],
    HIS_K_INTITEXT, &p.Update_Flag},
    {DCM_RESINTERPIDISSUER, DCM_LO, "", 1,
	sizeof p.IIIssuer, (void *) &p.IIIssuer[0],
    HIS_K_INTIIISSUER, &p.Update_Flag},
    {DCM_RESINTERPID, DCM_SH, "", 1,
	sizeof p.IID, (void *) &p.IID[0],
    HIS_K_INTIID, &p.Update_Flag},
    {DCM_RESINTERPTRANSCRIBER, DCM_PN, "", 1,
	sizeof p.ITranscriber, (void *) &p.ITranscriber[0],
    HIS_K_INTITRANSCRIBER, &p.Update_Flag}
};

typedef struct {
    HAP_INTERP_EVENT event;
    long required_event_attrib;
}   EVENT_MAP;

static EVENT_MAP array[] = {
    {HAP_INTERP_CREATED, HAP_REQUIRED_ICREATED_ATTRIB},
    {HAP_INTERP_RECORDED, HAP_REQUIRED_IRECORDED_ATTRIB},
    {HAP_INTERP_TRANSCRIBED, HAP_REQUIRED_ITRANSCRIBED_ATTRIB},
    {HAP_INTERP_APPROVED, HAP_REQUIRED_IAPPROVED_ATTRIB},
    {HAP_INTERP_UPDATED, HAP_REQUIRED_IUPDATED_ATTRIB},
    {HAP_INTERP_DELETED, HAP_REQUIRED_IDELETED_ATTRIB}
};

int i_buildSequence(DCM_OBJECT ** object, SQ_TYPE type);
int i_parseSequence(DCM_OBJECT ** object);

/* HAP_BuildInterp
**
** Purpose:
** 	HAP_BuildInterp adds attributes to the DCM_OBJECT passed to it. It
**	gets the attribute values from the HIS_InterpretationLevel structure.
**	It knows which attributes from within the structure to add from the
**	Update_Flag element of the HIS_InterpretationLevel structure. The
** 	event argument should be either 0 or an enum constant of
**	type HAP_INTERP_EVENT. If it is a enum constant, the function checks
**	the bits of Update_Flag so that the mandatory attributes for that event
**	are present and returns an error if they are not. For 0, there is no
**	checking.
**
** Parameter Dictionary:
**      object  	The existing DCM_OBJECT that is to be built.
**	xl		The structure containing attributes to add to the
**			DCM_OBJECT.
**	event		The event type.
**
** Return Values:
**	HAP_NORMAL
**	HAP_ILLEGALOBJECT
**	HAP_ILLEGALEVENT
**	HAP_INCOMPLETEEVENT
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/

CONDITION
HAP_BuildInterp(DCM_OBJECT ** object,
		HIS_InterpretationLevel * xl, HAP_INTERP_EVENT event)
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

    if (p.Update_Flag & HIS_K_INTRRSEQUENCE) {
	x = i_buildSequence(object, SQ_K_REFRESULTSSOPINSTANCEUID);
	if (x < 0)
	    return 0;		/* needs repair */
    }
    if (p.Update_Flag & HIS_K_INTIAPPSEQUENCE) {
	x = i_buildSequence(object, SQ_K_REQINTERPRETATIONAPPROVER);
	if (x < 0)
	    return 0;		/* needs repair */
    }
    if (p.Update_Flag & HIS_K_INTIDCSEQUENCE) {
	x = i_buildSequence(object, SQ_K_REQDIAGNOSISCODE);
	if (x < 0)
	    return 0;		/* needs repair */
    }
    return HAP_NORMAL;
}

/* HAP_ParseInterp
**
** Purpose:
**      HAP_ParseInterp extracts from a DCM_OBJECT all the attributes present
**	that are specified in the Detached Interpretation Management Service
**	Class. The attibute values are placed in the HIS_InterpretationLevel
** 	structure, and the Update_Flag element of this structure is set
**	according to what attributes were found.
**
** Parameter Dictionary:
**      object		Existing DCM_OBJECT to be parsed.
**	xl		The structure that holds the attribute values parsed
**			from object.
**
** Return Values:
**	HAP_NORMAL
**	HAP_ILLEGALOBJECT
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/


CONDITION
HAP_ParseInterp(DCM_OBJECT ** object,
		HIS_InterpretationLevel * xl)
{

    CONDITION cond;
    int x;

    memset(&p, 0, sizeof p);

    cond = DCM_ParseObject(object, NULL, 0, list, (int) DIM_OF(list), NULL);
    if (cond != DCM_NORMAL)
	return HAP_ILLEGALOBJECT;

    x = i_parseSequence(object);
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
i_buildSequence(DCM_OBJECT ** object, SQ_TYPE type)
{

    int index = 0;
    int i;
    LST_HEAD *l;
    DCM_ELEMENT *e;
    CONDITION cond;
    SQ_REFRESULTSSOPINSTANCEUID *results;
    SQ_REQINTERPRETATIONAPPROVER *approver;
    SQ_REQPROCEDURECODE *diagnosis;
    void *pointer;

    static SQ_MAP_SOP sops[] = {
	{SQ_K_REFRESULTSSOPINSTANCEUID,
	    DICOM_SOPCLASSDETACHEDRESULTSMGMT, &(p.RRSequenceCount),
	sizeof(SQ_REFRESULTSSOPINSTANCEUID)},
	{SQ_K_REQINTERPRETATIONAPPROVER,
	    "", &(p.IApprovalSequenceCount),
	sizeof(SQ_REQINTERPRETATIONAPPROVER)},
	{SQ_K_REQDIAGNOSISCODE,
	    "", &(p.IApprovalSequenceCount),
	sizeof(SQ_REQPROCEDURECODE)},
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
	case SQ_K_REFRESULTSSOPINSTANCEUID:
	    results = (SQ_REFRESULTSSOPINSTANCEUID *) pointer;
	    results->type = type;
	    sprintf(results->referencedSOPClassUID, "%s",
		    DICOM_SOPCLASSDETACHEDRESULTSMGMT);
	    sprintf(results->referencedSOPInstanceUID, "%s",
		    p.RRSequence[index]);
	    break;
	case SQ_K_REQINTERPRETATIONAPPROVER:
	    approver = (SQ_REQINTERPRETATIONAPPROVER *) pointer;
	    approver->type = type;
	    sprintf(approver->physiciansApproving, "%s",
		    p.IApprover[index]);
	    sprintf(approver->approvalDates, "%s",
		    p.IADates[index]);
	    sprintf(approver->approvalTimes, "%s",
		    p.IATimes[index]);
	    break;
	case SQ_K_REQDIAGNOSISCODE:
	    diagnosis = (SQ_REQPROCEDURECODE *) pointer;
	    diagnosis->type = type;
	    sprintf(diagnosis->codeValue, "%s",
		    p.IDCSequence[index].CodeValue);
	    sprintf(diagnosis->codingSchemeDesignator, "%s",
		    p.IDCSequence[index].CodingSchemeDesignator);
	    sprintf(diagnosis->codeMeaning, "%s",
		    p.IDCSequence[index].CodeMeaning);
	    break;
	}
	LST_Enqueue(&l, pointer);
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
}

typedef struct {
    DCM_TAG tag;
    SQ_TYPE type;
    unsigned long his_field;
}   DCM_MAP_SQ;

int
i_parseSequence(DCM_OBJECT ** object)
{

    DCM_ELEMENT e;
    LST_HEAD *l;
    CONDITION cond;
    int index;
    SQ_TYPE type;
    SQ_REFRESULTSSOPINSTANCEUID *results;
    SQ_REQINTERPRETATIONAPPROVER *approver;
    SQ_REQPROCEDURECODE *diagnosis;
    void *pointer;

    static DCM_MAP_SQ sequences[] = {
	{DCM_IDREFERENCEDRESULTSSEQ, SQ_K_REFRESULTSSOPINSTANCEUID,
	HIS_K_INTRRSEQUENCE},
	{DCM_RESINTERPAPPROVERSEQUENCE, SQ_K_REQINTERPRETATIONAPPROVER,
	HIS_K_INTIAPPSEQUENCE},
	{DCM_RESDIAGNOSISCODESEQ, SQ_K_REQDIAGNOSISCODE,
	HIS_K_INTIDCSEQUENCE},
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
		case DCM_IDREFERENCEDRESULTSSEQ:
		    strcpy(p.RRSequence[p.RRSequenceCount],
			   ((SQ_REFRESULTSSOPINSTANCEUID *)
			    pointer)->referencedSOPInstanceUID);
		    p.RRSequenceCount++;
		    break;
		case DCM_RESINTERPAPPROVERSEQUENCE:
		    strcpy(p.IApprover[p.IApprovalSequenceCount],
			   ((SQ_REQINTERPRETATIONAPPROVER *)
			    pointer)->physiciansApproving);
		    strcpy(p.IADates[p.IApprovalSequenceCount],
			   ((SQ_REQINTERPRETATIONAPPROVER *)
			    pointer)->approvalDates);
		    strcpy(p.IATimes[p.IApprovalSequenceCount],
			   ((SQ_REQINTERPRETATIONAPPROVER *)
			    pointer)->approvalTimes);
		    p.IApprovalSequenceCount++;
		    break;
		case DCM_RESDIAGNOSISCODESEQ:
		    strcpy(p.IDCSequence[p.IDCSequenceCount].
			   CodeValue,
			   ((SQ_REQPROCEDURECODE *) pointer)->codeValue);
		    strcpy(p.IDCSequence[p.IDCSequenceCount].
			   CodingSchemeDesignator,
			   ((SQ_REQPROCEDURECODE *) pointer)->codingSchemeDesignator);
		    strcpy(p.IDCSequence[p.IDCSequenceCount].
			   CodeMeaning,
			   ((SQ_REQPROCEDURECODE *) pointer)->codeMeaning);
		    p.IDCSequenceCount++;
		    break;
		}
		pointer = LST_Next(&l);
	    }
	}
    }
    return 0;
/*
    e.tag = DCM_IDREFERENCEDRESULTSSEQ;

    cond = DCM_GetSequenceList(object, e.tag, &e.d.sq);
    if (cond == DCM_ELEMENTNOTFOUND)
	return 0;
    else if (cond != DCM_NORMAL)
	return -1;

    cond = SQ_ParseSequence(&e, &type, &l);
    if (cond != SQ_NORMAL)
	return -2;
    if (type == SQ_K_REFRESULTSSOPINSTANCEUID) {
	p.Update_Flag |= HIS_K_INTRRSEQUENCE;
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
*/
}
