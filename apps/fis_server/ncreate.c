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
** Module Name(s):
** Author, Date:        Stephen Moore, 27-Oct-94
** Intent:              Handles DICOM NCREATE requests for HIS functions
**
** Last Update:         $Author: smm $, $Date: 2001-12-21 16:37:33 $
** Source File:         $RCSfile: ncreate.c,v $
** Revision:            $Revision: 1.8 $
** Status:              $State: Exp $
*/
static char rcsid[] = "$Revision: 1.8 $ $RCSfile: ncreate.c,v $";

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
ncreateCallback(MSG_N_CREATE_REQ * request, MSG_N_CREATE_RESP * response,
/*		CALLBACK_CTX * ctx, DUL_PRESENTATIONCONTEXT *pc); */
		void *ctx, DUL_PRESENTATIONCONTEXT * pc);
static CONDITION
studyComponentCreate(MSG_N_CREATE_REQ * request, MSG_N_CREATE_RESP * response,
		     CALLBACK_CTX * ctx);

/* ncreateRequest
**
*/

CONDITION
ncreateRequest(DUL_ASSOCIATESERVICEPARAMETERS * params,
	       DUL_ASSOCIATIONKEY ** association, DMAN_HANDLE ** dman,
	       DUL_PRESENTATIONCONTEXT * ctx, MSG_N_CREATE_REQ ** request,
	       FIS_HANDLE ** fis)
{
    MSG_N_CREATE_RESP response;
    CONDITION cond;
    CALLBACK_CTX cb;

    memset(&response, 0, sizeof(response));
    response.type = MSG_K_N_CREATE_RESP;
    response.conditionalFields = 0;
    response.dataSetType = DCM_CMDDATANULL;

    cb.abstractSyntax = ctx->abstractSyntax;
    cb.fis = fis;

    cond = SRV_NCreateResponse(association, ctx, request, &response,
			       ncreateCallback, &cb, "");
    if (cond != SRV_NORMAL)
	return 0;

    return APP_NORMAL;
}

/* ncreateCallback
**
** Purpose:
**
** Parameter Dictionary:
**	request		structure containing the instance to look up in the
**			database and the attributes to include in the
**			DCM_OBJECT.
**	response	structure filled in by ngetCallback; contains
**			the DCM_OBJECT built.
**	ctx		Pointer to context structure which contains abstract
**			syntax for this class and FIS handle.
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
ncreateCallback(MSG_N_CREATE_REQ * request, MSG_N_CREATE_RESP * response,
/*		CALLBACK_CTX * ctx, DUL_PRESENTATIONCONTEXT *pc) */
		void *ctxPtr, DUL_PRESENTATIONCONTEXT * pc)
{
    CONDITION cond;

/*  Following definition and assignment satisfy prototypes for dicom_services.h*/
    CALLBACK_CTX *ctx;
    ctx = (CALLBACK_CTX *) ctxPtr;

    strcpy(response->classUID, request->classUID);
    response->messageIDRespondedTo = request->messageID;


    if (strcmp(ctx->abstractSyntax, DICOM_SOPCLASSDETACHEDPATIENTMGMT) == 0) {
	cond = 0;

    } else if (strcmp(ctx->abstractSyntax, DICOM_SOPCLASSDETACHEDVISITMGMT) == 0) {
	cond = 0;
    } else if (strcmp(ctx->abstractSyntax, DICOM_SOPCLASSDETACHEDSTUDYMGMT) == 0) {
	cond = 0;
    } else if (strcmp(ctx->abstractSyntax, DICOM_SOPCLASSSTUDYCOMPONENTMGMT) == 0) {
	cond = studyComponentCreate(request, response, ctx);
    } else if (strcmp(ctx->abstractSyntax, DICOM_SOPCLASSDETACHEDRESULTSMGMT) == 0) {
	cond = 0;

    } else if (strcmp(ctx->abstractSyntax, DICOM_SOPCLASSDETACHEDINTERPRETMGMT) == 0) {
	cond = 0;

    } else
	return SRV_UNSUPPORTEDTRANSFERSYNTAX;

    if (cond == 1)
	return SRV_NORMAL;
    else
	return 0;
}

#define STUDYCOMPONENT_CREATE_FLAGS (FIS_K_STUDYCOMP_MOD | \
        FIS_K_STUDYCOMP_STUINSUID | \
        FIS_K_STUDYCOMP_PROCODVAL | \
        FIS_K_STUDYCOMP_PROCODSCHDES | FIS_K_STUDYCOMP_PROCODMEA | \
        FIS_K_STUDYCOMP_STUCOMSTAID | \
        FIS_K_STUDYCOMP_STUDES | \
        FIS_K_STUDYCOMP_STUID | \
        FIS_K_STUDYCOMP_SERIESLIST )

static CONDITION
studyComponentCreate(MSG_N_CREATE_REQ * request, MSG_N_CREATE_RESP * response,
		     CALLBACK_CTX * ctx)
{
    CONDITION cond;
    FIS_STUDYCOMPONENTRECORD sc;
    long flag;
    FIS_SCSERIESRECORD *series;
    FIS_SCIMAGERECORD *image;
    FIS_STUDYRECORD study;

    response->dataSetType = DCM_CMDDATANULL;
    strcpy(response->classUID, request->classUID);
    response->conditionalFields = MSG_K_N_CREATERESP_AFFECTEDCLASSUID;
    response->status = MSG_K_SUCCESS;

    if (request->dataSetType != DCM_CMDDATANULL) {
	(void) DCM_DumpElements(&request->dataSet, 0);
    } else {
	response->status = MSG_K_PROCESSINGFAILURE;
	goto ExitPoint;
    }

    cond = FIS_ParseObject(&request->dataSet, FIS_K_STUDYCOMPONENT, &sc);
    if (cond != FIS_NORMAL) {
	response->status = MSG_K_PROCESSINGFAILURE;
	goto ExitPoint;
    }
    flag = sc.Flag & STUDYCOMPONENT_CREATE_FLAGS;
    if (flag != STUDYCOMPONENT_CREATE_FLAGS) {
	response->status = MSG_K_MISSINGATTRIBUTE;
	goto ExitPoint;
    }
    if (request->conditionalFields & MSG_K_N_CREATEREQ_INSTANCEUID) {
	strcpy(sc.StuComUID, request->instanceUID);
    } else {
	cond = FIS_NewRecord(ctx->fis, FIS_K_STUDYCOMPONENT, &sc);
	if (cond != FIS_NORMAL) {
	    response->status = MSG_K_PROCESSINGFAILURE;
	    goto ExitPoint;
	}
    }
    sc.Flag |= FIS_K_STUDYCOMP_STUCOMUID;

    strcpy(response->instanceUID, sc.StuComUID);
    response->conditionalFields |= MSG_K_N_CREATERESP_AFFECTEDINSTANCEUID;

    cond = FIS_Insert(ctx->fis, FIS_K_STUDYCOMPONENT, &sc);
    if (cond != FIS_NORMAL) {
	response->status = MSG_K_PROCESSINGFAILURE;
	COND_DumpConditions();
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

	memset(&study, 0, sizeof(study));
	study.Type = FIS_K_STUDY;
	strcpy(study.StuInsUID, sc.StuInsUID);
	strcpy(study.StuStaID, "VERIFIED");
	study.Flag = FIS_K_STU_STUINSUID | FIS_K_STU_STUSTAID;
	(void) FIS_Update(ctx->fis, FIS_K_STUDY, &study);
    }
    (void) LST_Destroy(&sc.SeriesList);

ExitPoint:
    COND_DumpConditions();
    return 1;
}
