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
**			nsetCallback
** Author, Date:        John O'Neill, Stephen Moore, 8-Sep-94
** Intent:              Handles DICOM NGET requests for HIS functions
**
** Last Update:         $Author: smm $, $Date: 2001-12-21 16:37:35 $
** Source File:         $RCSfile: nset.c,v $
** Revision:            $Revision: 1.7 $
** Status:              $State: Exp $
*/
static char rcsid[] = "$Revision: 1.7 $ $RCSfile: nset.c,v $";

#include "ctn_os.h"

#if 0
#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <sys/types.h>
#endif

#include "dicom.h"
#include "condition.h"
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
nsetCallback(MSG_N_SET_REQ * request, MSG_N_SET_RESP * response,
/*	     CALLBACK_CTX * ctx, DUL_PRESENTATIONCONTEXT *pc); */
	     void *ctxPtr, DUL_PRESENTATIONCONTEXT * pc);
static CONDITION
patientSet(MSG_N_SET_REQ * request, MSG_N_SET_RESP * response,
	   CALLBACK_CTX * ctx);
static CONDITION
visitSet(MSG_N_SET_REQ * request, MSG_N_SET_RESP * response,
	 CALLBACK_CTX * ctx);
static CONDITION
studySet(MSG_N_SET_REQ * request, MSG_N_SET_RESP * response,
	 CALLBACK_CTX * ctx);
static CONDITION
resultsSet(MSG_N_SET_REQ * request, MSG_N_SET_RESP * response,
	   CALLBACK_CTX * ctx);
static CONDITION
interpretationSet(MSG_N_SET_REQ * request, MSG_N_SET_RESP * response,
		  CALLBACK_CTX * ctx);
static CONDITION
studyComponentSet(MSG_N_SET_REQ * request, MSG_N_SET_RESP * response,
		  CALLBACK_CTX * ctx);

/* nsetRequest
**
*/

CONDITION
nsetRequest(DUL_ASSOCIATESERVICEPARAMETERS * params,
	    DUL_ASSOCIATIONKEY ** association, DMAN_HANDLE ** dman,
	    DUL_PRESENTATIONCONTEXT * ctx, MSG_N_SET_REQ ** request,
	    FIS_HANDLE ** fis)
{
    MSG_N_SET_RESP response;
    CONDITION cond;
    CALLBACK_CTX cb;

    memset(&response, 0, sizeof(response));
    response.type = MSG_K_N_SET_RESP;
    response.conditionalFields = 0;
    response.dataSetType = DCM_CMDDATANULL;

    cb.abstractSyntax = ctx->abstractSyntax;
    cb.fis = fis;

    cond = SRV_NSetResponse(association, ctx, request, &response, nsetCallback,
			    &cb, "");
    if (cond != SRV_NORMAL)
	return 0;

    return APP_NORMAL;
}

/* nsetCallback
**
** Purpose:
**
** Parameter Dictionary:
**	request		structure containing the instance to look up in the
**			database and the attributes to include in the
**			DCM_OBJECT.
**	response	structure filled in by ngetCallback; contains
**			the DCM_OBJECT built.
**	ctx		Context variable which points at a structure with
**			abstract syntax for this SOP class and FIS handle
**	pc		Presentation context which describes this SOP class.
**
** Return Values:
**
**	SRV_NORMAL
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/



static CONDITION
nsetCallback(MSG_N_SET_REQ * request, MSG_N_SET_RESP * response,
/*	     CALLBACK_CTX * ctx, DUL_PRESENTATIONCONTEXT *pc) */
	     void *ctxPtr, DUL_PRESENTATIONCONTEXT * pc)
{
    CONDITION cond;
/*  Following definition and assignment satisfy prototypes for services */
    CALLBACK_CTX *ctx;
    ctx = (CALLBACK_CTX *) ctxPtr;

    strcpy(response->classUID, request->classUID);
    strcpy(response->instanceUID, request->instanceUID);
    response->messageIDRespondedTo = request->messageID;


    if (strcmp(ctx->abstractSyntax, DICOM_SOPCLASSDETACHEDPATIENTMGMT) == 0) {
	cond = patientSet(request, response, ctx);

    } else if (strcmp(ctx->abstractSyntax, DICOM_SOPCLASSDETACHEDVISITMGMT) == 0) {
	cond = visitSet(request, response, ctx);
	return cond = SRV_NORMAL;
    } else if (strcmp(ctx->abstractSyntax, DICOM_SOPCLASSDETACHEDSTUDYMGMT) == 0) {
	cond = studySet(request, response, ctx);
    } else if (strcmp(ctx->abstractSyntax, DICOM_SOPCLASSSTUDYCOMPONENTMGMT) == 0) {
	cond = studyComponentSet(request, response, ctx);

    } else if (strcmp(ctx->abstractSyntax, DICOM_SOPCLASSDETACHEDRESULTSMGMT) == 0) {
	cond = resultsSet(request, response, ctx);

    } else if (strcmp(ctx->abstractSyntax, DICOM_SOPCLASSDETACHEDINTERPRETMGMT) == 0) {
	cond = interpretationSet(request, response, ctx);

    } else
	return SRV_UNSUPPORTEDTRANSFERSYNTAX;

    if (cond == 1)
	return SRV_NORMAL;
    else
	return 0;
}

/* Commented out by smm, 12/30/95.  Not referenced in code */
/*
static TAG_MAP patient_supported[] = {
    {DCM_PATNAME, FIS_K_PAT_PATNAM},
    {DCM_PATID, FIS_K_PAT_PATID},
    {DCM_PATBIRTHDATE, FIS_K_PAT_BIRDAT},
    {DCM_PATSEX, FIS_K_PAT_SEX},
};
*/


static CONDITION
patientSet(MSG_N_SET_REQ * request, MSG_N_SET_RESP * response,
	   CALLBACK_CTX * ctx)
{
    CONDITION cond;
    FIS_PATIENTRECORD patient;

#ifdef SMM
    patient.Type = FIS_K_PATIENT;
    strcpy(patient.PatUID, request->requestedInstanceUID);

    cond = FIS_GetOne(ctx->fis, FIS_K_PATIENT, FIS_K_PATIENT,
		      request->requestedInstanceUID, 0, &patient);
    if (cond != FIS_NORMAL)
	return 0;

    /* SET FLAGS FOR HAP FACILITY BASED ON attributeList */

    if (request->attributeCount == 0)
	patient.Flag = 0xffffffff;
    else
	convertTags(request->attributeCount, request->attributeList,
		    patient_supported, &patient.Flag,
		    (int) DIM_OF(patient_supported));

    cond = FIS_BuildObject(&response->dataSet, FIS_K_PATIENT, &patient,
			   FIS_K_NO_EVENT);
    if (cond != FIS_NORMAL)
	return 0;
    response->dataSetType = DCM_CMDDATAOTHER;
    (void) DCM_DumpElements(&response->dataSet, 0);
#endif
    return 1;
}

static CONDITION
visitSet(MSG_N_SET_REQ * request, MSG_N_SET_RESP * response,
	 CALLBACK_CTX * ctx)
{
    CONDITION cond;

    return 1;
}

#define	STUDY_SET_FLAGS (FIS_K_STU_STUSTAID)
static CONDITION
studySet(MSG_N_SET_REQ * request, MSG_N_SET_RESP * response,
	 CALLBACK_CTX * ctx)
{
    CONDITION cond;
    FIS_STUDYRECORD study;
    long flag;

    if (request->dataSetType != DCM_CMDDATANULL) {
	(void) DCM_DumpElements(&request->dataSet, 0);
    }
    cond = FIS_ParseObject(&request->dataSet, FIS_K_STUDY, &study);
    if (cond != FIS_NORMAL)
	return 0;

    study.Flag &= STUDY_SET_FLAGS;
    if (study.Flag == 0)
	return 1;

    strcpy(study.StuInsUID, request->instanceUID);
    study.Flag |= FIS_K_STU_STUINSUID;
    cond = FIS_Update(ctx->fis, FIS_K_STUDY, &study);
    if (cond != FIS_NORMAL) {
	COND_DumpConditions();
    } else {
	response->status = MSG_K_SUCCESS;
    }
    response->dataSetType = DCM_CMDDATANULL;
    strcpy(response->classUID, request->classUID);
    strcpy(response->instanceUID, request->instanceUID);
    response->conditionalFields = MSG_K_N_SETRESP_REQUESTEDCLASSUID |
	MSG_K_N_SETRESP_REQUESTEDINSTANCEUID;

    return 1;
}

/* Commented out by smm, 12/30/95.  Not referenced in code */
/*
static TAG_MAP results_supported[] = {
    {DCM_IDREFERENCEDSTUDYSEQ, FIS_K_RESULTS_STUINSUID},
    {DCM_RESID, FIS_K_RESULTS_RESID},
    {DCM_RESIMPRESSIONS, FIS_K_RESULTS_IMP}
};
*/

static CONDITION
resultsSet(MSG_N_SET_REQ * request, MSG_N_SET_RESP * response,
	   CALLBACK_CTX * ctx)
{
    CONDITION cond;
    FIS_RESULTSRECORD results;
    long flag;

#ifdef SMM
    results.Type = FIS_K_RESULTS;
    strcpy(results.ResUID, request->requestedInstanceUID);

    cond = FIS_GetOne(ctx->fis, FIS_K_RESULTS, FIS_K_RESULTS,
		      request->requestedInstanceUID, flag, &results);
    if (cond != FIS_NORMAL)
	return 0;

    results.Flag &= flag;
    cond = FIS_BuildObject(&response->dataSet, FIS_K_RESULTS, &results,
			   FIS_K_NO_EVENT);
    if (cond != FIS_NORMAL)
	return 0;
    response->dataSetType = DCM_CMDDATAOTHER;
    (void) DCM_DumpElements(&response->dataSet, 0);
#endif
    return 1;
}

/* Commented out by smm 12/30/95.  Not referenced in code */
/*
static TAG_MAP interpretation_supported[] = {
    {DCM_IDREFERENCEDRESULTSSEQ, FIS_K_INTERP_RESUID},
    {DCM_RESINTERPTEXT, FIS_K_INTERP_INTTEX},
    {DCM_RESINTERPID, FIS_K_INTERP_INTID},
    {DCM_RESINTERPTYPEID, FIS_K_INTERP_INTTYPID},
    {DCM_RESINTERPSTATUSID, FIS_K_INTERP_INTSTAID}
};
*/

static CONDITION
interpretationSet(MSG_N_SET_REQ * request, MSG_N_SET_RESP * response,
		  CALLBACK_CTX * ctx)
{
    CONDITION cond;
    FIS_INTERPRETATIONRECORD interpretation;
    long flag;

#ifdef SMM
    interpretation.Type = FIS_K_INTERPRETATION;
    strcpy(interpretation.IntUID, request->requestedInstanceUID);

    cond = FIS_GetOne(ctx->fis, FIS_K_INTERPRETATION, FIS_K_INTERPRETATION,
		      request->requestedInstanceUID, flag, &interpretation);
    if (cond != FIS_NORMAL)
	return 0;

    interpretation.Flag &= flag;
    cond = FIS_BuildObject(&response->dataSet, FIS_K_INTERPRETATION,
			   &interpretation, FIS_K_NO_EVENT);
    if (cond != FIS_NORMAL)
	return 0;
    response->dataSetType = DCM_CMDDATAOTHER;
    (void) DCM_DumpElements(&response->dataSet, 0);
#endif
    return 1;
}

#define STUDYCOMPONENT_SET_FLAGS ( FIS_K_STUDYCOMP_STUDES | \
        FIS_K_STUDYCOMP_PROCODVAL | \
        FIS_K_STUDYCOMP_PROCODSCHDES | FIS_K_STUDYCOMP_PROCODMEA | \
        FIS_K_STUDYCOMP_STUCOMSTAID | \
        FIS_K_STUDYCOMP_STUID | \
        FIS_K_STUDYCOMP_SERIESLIST)

static CONDITION
studyComponentSet(MSG_N_SET_REQ * request, MSG_N_SET_RESP * response,
		  CALLBACK_CTX * ctx)
{
    CONDITION cond;
    FIS_STUDYCOMPONENTRECORD sc;
    long flag;
    FIS_SCSERIESRECORD *series;
    FIS_SCIMAGERECORD *image;

    response->status = MSG_K_SUCCESS;

    if (request->dataSetType != DCM_CMDDATANULL) {
	(void) DCM_DumpElements(&request->dataSet, 0);
    } else {
	response->status = MSG_K_MISSINGATTRIBUTE;
	goto ExitPoint;
    }

    cond = FIS_ParseObject(&request->dataSet, FIS_K_STUDYCOMPONENT, &sc);
    if (cond != FIS_NORMAL) {
	response->status = MSG_K_PROCESSINGFAILURE;
	goto ExitPoint;
    }
    sc.Flag &= STUDYCOMPONENT_SET_FLAGS;
    if (sc.Flag == 0)
	goto ExitPoint;

    strcpy(sc.StuComUID, request->instanceUID);
    sc.Flag |= FIS_K_STUDYCOMP_STUCOMUID;
    cond = FIS_Update(ctx->fis, FIS_K_STUDYCOMPONENT, &sc);
    if (cond != FIS_NORMAL) {
	response->status = MSG_K_PROCESSINGFAILURE;
	goto ExitPoint;
    }
    if (sc.Flag & FIS_K_STUDYCOMP_SERIESLIST) {
/*  Delete existing values in series/image tables and insert new values */
	cond = FIS_Delete(ctx->fis, FIS_K_SCSERIES, FIS_K_STUDYCOMPONENT,
			  request->instanceUID);
	if (cond != FIS_NORMAL) {
	    response->status = MSG_K_PROCESSINGFAILURE;
	    goto ExitPoint;
	}
	cond = FIS_Delete(ctx->fis, FIS_K_SCIMAGE, FIS_K_STUDYCOMPONENT,
			  request->instanceUID);
	if (cond != FIS_NORMAL) {
	    response->status = MSG_K_PROCESSINGFAILURE;
	    goto ExitPoint;
	}
	while ((series = LST_Dequeue(&sc.SeriesList)) != NULL) {
	    strcpy(series->StuComUID, sc.StuComUID);
	    series->Flag |= FIS_K_SCSERIES_STUCOMUID;

	    cond = FIS_Insert(ctx->fis, FIS_K_SCSERIES, series);
	    if (cond != FIS_NORMAL) {
		response->status = MSG_K_PROCESSINGFAILURE;
		COND_DumpConditions();
		goto ExitPoint;
	    }
	    while ((image = LST_Dequeue(&series->ImageList)) != NULL) {
		strcpy(image->SerInsUID, series->SerInsUID);
		strcpy(image->StuComUID, series->StuComUID);
		image->Flag |= FIS_K_SCIMAGE_SERINSUID | FIS_K_SCIMAGE_STUCOMUID;

		cond = FIS_Insert(ctx->fis, FIS_K_SCIMAGE, image);
		if (cond != FIS_NORMAL) {
		    response->status = MSG_K_PROCESSINGFAILURE;
		    COND_DumpConditions();
		    goto ExitPoint;
		}
		free(image);
	    }
	    (void) LST_Destroy(&series->ImageList);
	    free(series);
	}
	(void) LST_Destroy(&sc.SeriesList);


    }
    response->dataSetType = DCM_CMDDATANULL;
    strcpy(response->classUID, request->classUID);
    strcpy(response->instanceUID, request->instanceUID);
    response->conditionalFields = MSG_K_N_SETRESP_REQUESTEDCLASSUID |
	MSG_K_N_SETRESP_REQUESTEDINSTANCEUID;

ExitPoint:
    COND_DumpConditions();
    return 1;
}
