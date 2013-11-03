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
**                              DICOM 94
**                   Electronic Radiology Laboratory
**                 Mallinckrodt Institute of Radiology
**              Washington University School of Medicine
**
** Module Name(s):      handleNGetRequest
**			ngetCallback
**			convertTags
** Author, Date:        John O'Neill, Stephen Moore, 8-Sep-94
** Intent:              Handles DICOM NGET requests for HIS functions
**
** Last Update:         $Author: smm $, $Date: 2001-12-21 16:37:34 $
** Source File:         $RCSfile: nget.c,v $
** Revision:            $Revision: 1.11 $
** Status:              $State: Exp $
*/
static char rcsid[] = "$Revision: 1.11 $ $RCSfile: nget.c,v $";

#include "ctn_os.h"

#if 0
#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <sys/types.h>
#endif

#include "dicom.h"
#include "tbl.h"
#include "lst.h"
#include "dicom_uids.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "tbl.h"
#include "manage.h"
#include "fis.h"

#include "fis_server.h"
#include "services.h"

typedef struct {
    char *abstractSyntax;
    FIS_HANDLE **fis;
}   CALLBACK_CTX;

typedef struct {
    DCM_TAG tag;
    unsigned long flag;
}   TAG_MAP;

static CONDITION
ngetCallback(MSG_N_GET_REQ * request, MSG_N_GET_RESP * response,
/*	     CALLBACK_CTX * ctx, DUL_PRESENTATIONCONTEXT *pc); */
	     void *ctxPtr, DUL_PRESENTATIONCONTEXT * pc);
static void
convertTags(int attributeCount, DCM_TAG * attributeList,
	    TAG_MAP * map, unsigned long *flagAddress, int size);
static CONDITION
patientGet(MSG_N_GET_REQ * request, MSG_N_GET_RESP * response,
	   CALLBACK_CTX * ctx);
static CONDITION
visitGet(MSG_N_GET_REQ * request, MSG_N_GET_RESP * response,
	 CALLBACK_CTX * ctx);
static CONDITION
studyGet(MSG_N_GET_REQ * request, MSG_N_GET_RESP * response,
	 CALLBACK_CTX * ctx);
static CONDITION
resultsGet(MSG_N_GET_REQ * request, MSG_N_GET_RESP * response,
	   CALLBACK_CTX * ctx);
static CONDITION
interpretationGet(MSG_N_GET_REQ * request, MSG_N_GET_RESP * response,
		  CALLBACK_CTX * ctx);
static CONDITION
studyComponentGet(MSG_N_GET_REQ * request, MSG_N_GET_RESP * response,
		  CALLBACK_CTX * ctx);

/* ngetRequest
**
*/

CONDITION
ngetRequest(DUL_ASSOCIATESERVICEPARAMETERS * params,
	    DUL_ASSOCIATIONKEY ** association, DMAN_HANDLE ** dman,
	    DUL_PRESENTATIONCONTEXT * ctx, MSG_N_GET_REQ ** request,
	    FIS_HANDLE ** fis)
{
    MSG_N_GET_RESP response = {
	MSG_K_N_GET_RESP,	/* type */
	0,			/* conditionalFields */
	0,			/* messageIDRespondedTo */
	DCM_CMDDATANULL,	/* dataSetType */
	0,			/* status */
	"",			/* SOP Class UID */
	"",			/* Affected instance UID */
	NULL,			/* dataSet object */
    };
    CONDITION cond;
    CALLBACK_CTX cb;

    cb.abstractSyntax = ctx->abstractSyntax;
    cb.fis = fis;

    cond = SRV_NGetResponse(association, ctx, request, &response, ngetCallback,
			    &cb, "");
    if (cond != SRV_NORMAL)
	return 0;

    return APP_NORMAL;
}

/* ngetCallback
**
** Purpose:
**	This function fills the response structure based on the request
**	structure. It opens and calls a database to find information
**	about the requested instance in the request structure. Then, from
** 	the database information, a DCM_OBJECT is constructed.
**
** Parameter Dictionary:
**	request		structure containing the instance to look up in the
**			database and the attributes to include in the
**			DCM_OBJECT.
**	response	structure filled in by ngetCallback; contains
**			the DCM_OBJECT built.
**	ctx		Pointer to a structure that contains the abstract
**			syntax for this class and the FIS handle.
**	pc		Presentation context that describes this SOP Class
**
** Return Values:
**
**	SRV_NORMAL
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/



static CONDITION
ngetCallback(MSG_N_GET_REQ * request, MSG_N_GET_RESP * response,
/*	     CALLBACK_CTX * ctx, DUL_PRESENTATIONCONTEXT *pc) */
	     void *ctxPtr, DUL_PRESENTATIONCONTEXT * pc)
{
    CONDITION cond;
/*  Following definition and assignment satisfy prototypes for dicom_services.h*/
    CALLBACK_CTX *ctx;
    ctx = (CALLBACK_CTX *) ctxPtr;

    strcpy(response->classUID, request->classUID);
    strcpy(response->affectedInstanceUID, request->requestedInstanceUID);
    response->messageIDRespondedTo = request->messageID;

    if (request->attributeCount < 0)
	return 0;

    /*
     * IF attributeCount == 0, THEN THE attributeList IS NULL. THUS, DICOM
     * V3.0 SAYS ALL ATTRIBUTES SHOULD BE INCLUDED IN RESPONSE.
     */
    /*
     * These five IF-statements get information from the database and build a
     * DCM_OBJECT from that information. Each IF-statement corresponds to the
     * detached patient, visit, study, results, or interpretation service
     * class.
     */

    if (strcmp(ctx->abstractSyntax, DICOM_SOPCLASSDETACHEDPATIENTMGMT) == 0) {
	cond = patientGet(request, response, ctx);

    } else if (strcmp(ctx->abstractSyntax, DICOM_SOPCLASSDETACHEDVISITMGMT) == 0) {
	cond = visitGet(request, response, ctx);

#ifdef SMM
#endif
	return cond = SRV_NORMAL;
    } else if (strcmp(ctx->abstractSyntax, DICOM_SOPCLASSDETACHEDSTUDYMGMT) == 0) {
	cond = studyGet(request, response, ctx);
    } else if (strcmp(ctx->abstractSyntax, DICOM_SOPCLASSSTUDYCOMPONENTMGMT) == 0) {
	cond = studyComponentGet(request, response, ctx);
    } else if (strcmp(ctx->abstractSyntax, DICOM_SOPCLASSDETACHEDRESULTSMGMT) == 0) {
	cond = resultsGet(request, response, ctx);

    } else if (strcmp(ctx->abstractSyntax, DICOM_SOPCLASSDETACHEDINTERPRETMGMT) == 0) {
	cond = interpretationGet(request, response, ctx);

    } else
	return SRV_UNSUPPORTEDTRANSFERSYNTAX;

    if (cond == 1)
	return SRV_NORMAL;
    else
	return 0;
}



static CONDITION
patientGet(MSG_N_GET_REQ * request, MSG_N_GET_RESP * response,
	   CALLBACK_CTX * ctx)
{
    CONDITION cond;
    FIS_PATIENTRECORD patient;
    unsigned long flag;

    static TAG_MAP patient_supported[] = {
	{DCM_PATNAME, FIS_K_PAT_PATNAM},
	{DCM_PATID, FIS_K_PAT_PATID},
	{DCM_PATBIRTHDATE, FIS_K_PAT_BIRDAT},
	{DCM_PATSEX, FIS_K_PAT_SEX},
	{DCM_IDREFERENCEDSTUDYSEQ, FIS_K_PAT_STUDYLIST},
/*    {DCM_IDREFERENCEDVISITSEQ , FIS_K_PAT_VISITLIST}, */
    };

    if (request->attributeCount == 0)
	flag = 0xffffffff;
    else
	convertTags(request->attributeCount, request->attributeList,
		 patient_supported, &flag, (int) DIM_OF(patient_supported));

    patient.Type = FIS_K_PATIENT;
    strcpy(patient.PatUID, request->requestedInstanceUID);

    cond = FIS_GetOne(ctx->fis, FIS_K_PATIENT, FIS_K_PATIENT,
		      request->requestedInstanceUID, flag, &patient);
    if (cond != FIS_NORMAL) {
	response->dataSetType = DCM_CMDDATANULL;
	response->status = MSG_K_NOSUCHSOPCLASS;
	return 1;
    }
    /* Reset patient attribute flag based on what the caller requested */

    patient.Flag &= flag;
    cond = FIS_BuildObject(&response->dataSet, FIS_K_PATIENT, &patient,
			   FIS_K_NO_EVENT);
    if (cond != FIS_NORMAL) {
	response->dataSetType = DCM_CMDDATANULL;
	response->status = MSG_K_PROCESSINGFAILURE;
	return 1;
    }
    response->dataSetType = DCM_CMDDATAOTHER;
    (void) DCM_DumpElements(&response->dataSet, 0);

    return 1;
}

static CONDITION
visitGet(MSG_N_GET_REQ * request, MSG_N_GET_RESP * response,
	 CALLBACK_CTX * ctx)
{
    CONDITION cond;

    return 1;
}


/* Commented out by smm, 12/30/95.  Not referenced */
/*
static DCM_TAG study_unsupported[] = {
    DCM_IDINSTANCECREATEDATE,
    DCM_IDINSTANCECREATETIME,
    DCM_IDINSTANCECREATORUID,
    DCM_SDYREQUESTEDPROCODESEQ,
    DCM_SDYARRIVALDATE,
    DCM_SDYARRIVALTIME,
    DCM_IDSTUDYDATE,
    DCM_IDSTUDYTIME,
    DCM_SDYCOMPLETIONDATE,
    DCM_SDYCOMPLETIONTIME,
    DCM_SDYVERIFIEDDATE,
    DCM_SDYVERIFIEDTIME,
    DCM_SDYREADDATE,
    DCM_SDYREADTIME,
    DCM_SDYIDISSUER,
    DCM_IDSTUDYDESCRIPTION,
    DCM_IDPROCEDURECODESEQUENCE,
    DCM_SDYPRIORITYID,
    DCM_SDYREASON,
    DCM_SDYREQUESTINGPHYSICIAN,
    DCM_IDREFERENCEDSTUDYCOMPONENTSEQ
};
*/

static CONDITION
studyGet(MSG_N_GET_REQ * request, MSG_N_GET_RESP * response,
	 CALLBACK_CTX * ctx)
{
    CONDITION cond;
    FIS_STUDYRECORD study;
    unsigned long flag;

    static TAG_MAP study_supported[] = {
	{DCM_SDYSCHEDULEDSTARTDATE, FIS_K_STU_SCHSTUSTADAT},
	{DCM_SDYSCHEDULEDSTARTTIME, FIS_K_STU_SCHSTUSTATIM},
	{DCM_SDYSCHEDULEDLOCATION, FIS_K_STU_SCHSTULOC},
	{DCM_SDYREQUESTEDPRODESCRIPTION, FIS_K_STU_PRODES},
	{DCM_RELSTUDYID, FIS_K_STU_STUID},
	{DCM_IDACCESSIONNUMBER, FIS_K_STU_ACCNUM},
	{DCM_SDYSTATUSID, FIS_K_STU_STUSTAID},
	{DCM_IDREFERENCEDPATIENTSEQ, FIS_K_STU_PATUID},
	{DCM_IDREFERENCEDVISITSEQ, FIS_K_STU_VISUID},
	{DCM_IDREFERENCEDPATIENTSEQ, FIS_K_STU_PATUID},
	{DCM_IDREFERENCEDSTUDYCOMPONENTSEQ, FIS_K_STU_STUDYCOMPONENTLIST},
	{DCM_IDREFERENCEDVISITSEQ, FIS_K_STU_VISUID},
	{DCM_IDREFERENCEDRESULTSSEQ, FIS_K_STU_RESULTSLIST},
    };

    /* Set retrieval flags based on attribute list */

    if (request->attributeCount == 0)
	flag = 0xffffffff;
    else
	convertTags(request->attributeCount, request->attributeList,
		    study_supported, &flag, (int) DIM_OF(study_supported));

    study.Type = FIS_K_STUDY;
    strcpy(study.StuInsUID, request->requestedInstanceUID);

    cond = FIS_GetOne(ctx->fis, FIS_K_STUDY, FIS_K_STUDY,
		      request->requestedInstanceUID, flag, &study);
    if (cond != FIS_NORMAL) {
	response->dataSetType = DCM_CMDDATANULL;
	response->status = MSG_K_NOSUCHSOPCLASS;
	return 1;
    }
    /* Reset study attribute flag based on what the caller requested */
    study.Flag &= flag;
    cond = FIS_BuildObject(&response->dataSet, FIS_K_STUDY, &study,
			   FIS_K_NO_EVENT);
    if (cond != FIS_NORMAL) {
	response->dataSetType = DCM_CMDDATANULL;
	response->status = MSG_K_PROCESSINGFAILURE;
	return 1;
    }
    response->dataSetType = DCM_CMDDATAOTHER;
    (void) DCM_DumpElements(&response->dataSet, 0);

    return 1;
}



static CONDITION
studyComponentGet(MSG_N_GET_REQ * request, MSG_N_GET_RESP * response,
		  CALLBACK_CTX * ctx)
{
    CONDITION cond;
    FIS_STUDYCOMPONENTRECORD studyComponent;
    unsigned long flag;

    static TAG_MAP studyComponent_supported[] = {
	{DCM_IDMODALITY, FIS_K_STUDYCOMP_MOD},
	{DCM_IDSTUDYDESCRIPTION, FIS_K_STUDYCOMP_STUDES},
	{DCM_IDREFERENCEDSTUDYSEQ, FIS_K_STUDYCOMP_STUINSUID},
	{DCM_IDPROCEDURECODESEQUENCE, FIS_K_STUDYCOMP_PROCODVAL |
	    FIS_K_STUDYCOMP_PROCODSCHDES |
	FIS_K_STUDYCOMP_PROCODMEA},
	{DCM_SDYSTUDYCOMPONENTSTATUSID, FIS_K_STUDYCOMP_STUCOMSTAID},
	{DCM_RELSTUDYID, FIS_K_STUDYCOMP_STUID},
	{DCM_IDREFERENCEDSERIESSEQ, FIS_K_STUDYCOMP_SERIESLIST}
    };

    /* Set retrieval flags based on attribute list */

    if (request->attributeCount == 0)
	flag = 0xffffffff;
    else
	convertTags(request->attributeCount, request->attributeList,
		    studyComponent_supported, &flag,
		    (int) DIM_OF(studyComponent_supported));

    studyComponent.Type = FIS_K_STUDYCOMPONENT;
    strcpy(studyComponent.StuComUID, request->requestedInstanceUID);

    cond = FIS_GetOne(ctx->fis, FIS_K_STUDYCOMPONENT, FIS_K_STUDYCOMPONENT,
		      request->requestedInstanceUID, flag, &studyComponent);
    if (cond != FIS_NORMAL) {
	response->dataSetType = DCM_CMDDATANULL;
	response->status = MSG_K_NOSUCHSOPCLASS;
	return 1;
    }
    /* Reset study component attribute flag based on what caller requested */
    studyComponent.Flag &= flag;
    cond = FIS_BuildObject(&response->dataSet, FIS_K_STUDYCOMPONENT,
			   &studyComponent, FIS_K_NO_EVENT);
    if (cond != FIS_NORMAL) {
	response->dataSetType = DCM_CMDDATANULL;
	response->status = MSG_K_PROCESSINGFAILURE;
	return 1;
    }
    response->dataSetType = DCM_CMDDATAOTHER;
    (void) DCM_DumpElements(&response->dataSet, 0);

    return 1;
}


static CONDITION
resultsGet(MSG_N_GET_REQ * request, MSG_N_GET_RESP * response,
	   CALLBACK_CTX * ctx)
{
    CONDITION cond;
    FIS_RESULTSRECORD results;
    unsigned long flag;

    static TAG_MAP results_supported[] = {
	{DCM_IDREFERENCEDSTUDYSEQ, FIS_K_RESULTS_STUINSUID},
	{DCM_RESID, FIS_K_RESULTS_RESID},
	{DCM_RESIMPRESSIONS, FIS_K_RESULTS_IMP},
	{DCM_RESREFERENCEDINTERPSEQ, FIS_K_RESULTS_INTERPRETATIONLIST},
    };

    /* Set retrieval flags based on attribute list */

    if (request->attributeCount == 0)
	flag = 0xffffffff;
    else
	convertTags(request->attributeCount, request->attributeList,
		 results_supported, &flag, (int) DIM_OF(results_supported));

    results.Type = FIS_K_RESULTS;
    strcpy(results.ResUID, request->requestedInstanceUID);

    cond = FIS_GetOne(ctx->fis, FIS_K_RESULTS, FIS_K_RESULTS,
		      request->requestedInstanceUID, flag, &results);
    if (cond != FIS_NORMAL) {
	response->dataSetType = DCM_CMDDATANULL;
	response->status = MSG_K_NOSUCHSOPCLASS;
	return 1;
    }
/*  Reset results attribute flag based on what caller requested */
    results.Flag &= flag;
    cond = FIS_BuildObject(&response->dataSet, FIS_K_RESULTS, &results,
			   FIS_K_NO_EVENT);
    if (cond != FIS_NORMAL) {
	response->dataSetType = DCM_CMDDATANULL;
	response->status = MSG_K_PROCESSINGFAILURE;
	return 1;
    }
    response->dataSetType = DCM_CMDDATAOTHER;
    (void) DCM_DumpElements(&response->dataSet, 0);

    return 1;
}


static CONDITION
interpretationGet(MSG_N_GET_REQ * request, MSG_N_GET_RESP * response,
		  CALLBACK_CTX * ctx)
{
    CONDITION cond;
    FIS_INTERPRETATIONRECORD interpretation;
    unsigned long flag;

    static TAG_MAP interpretation_supported[] = {
	{DCM_IDREFERENCEDRESULTSSEQ, FIS_K_INTERP_RESUID},
	{DCM_RESINTERPTEXT, FIS_K_INTERP_INTTEX},
	{DCM_RESINTERPID, FIS_K_INTERP_INTID},
	{DCM_RESINTERPTYPEID, FIS_K_INTERP_INTTYPID},
	{DCM_RESINTERPSTATUSID, FIS_K_INTERP_INTSTAID}
    };

    /* Set retrieval flags based on attribute list */

    if (request->attributeCount == 0)
	flag = 0xffffffff;
    else
	convertTags(request->attributeCount, request->attributeList,
		    interpretation_supported, &flag,
		    (int) DIM_OF(interpretation_supported));

    interpretation.Type = FIS_K_INTERPRETATION;
    strcpy(interpretation.IntUID, request->requestedInstanceUID);

    cond = FIS_GetOne(ctx->fis, FIS_K_INTERPRETATION, FIS_K_INTERPRETATION,
		      request->requestedInstanceUID, flag, &interpretation);
    if (cond != FIS_NORMAL) {
	response->dataSetType = DCM_CMDDATANULL;
	response->status = MSG_K_NOSUCHSOPCLASS;
	return 1;
    }
/*  Reset interpretation attribute flag based on what caller requested */
    interpretation.Flag &= flag;
    cond = FIS_BuildObject(&response->dataSet, FIS_K_INTERPRETATION,
			   &interpretation, FIS_K_NO_EVENT);
    if (cond != FIS_NORMAL) {
	response->dataSetType = DCM_CMDDATANULL;
	response->status = MSG_K_PROCESSINGFAILURE;
	return 1;
    }
    response->dataSetType = DCM_CMDDATAOTHER;
    (void) DCM_DumpElements(&response->dataSet, 0);

    return 1;
}

static void
convertTags(int attributeCount, DCM_TAG * attributeList,
	    TAG_MAP * map, unsigned long *flagAddress, int size)
{
    int index;

    *flagAddress = 0;
    while (--attributeCount >= 0) {
	for (index = 0; index < size; index++)
	    if (*attributeList == map[index].tag)
		*flagAddress |= map[index].flag;

	attributeList++;
    }
}
