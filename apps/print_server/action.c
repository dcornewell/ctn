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
+-+-+-+-+-+-+-+-+-
*/
/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	actionRequest
**
**	Private Routines:
**			nactionBFBCallback
**			unsupportedCallback
** Author, Date:	Aniruddha S. Gokhale, 5-July-1994
** Intent:		This module implements routines that service the
**			N-ACTION service request.
** Last Update:		$Author: smm $, $Date: 1998-01-28 05:05:24 $
** Source File:		$RCSfile: action.c,v $
** Revision:		$Revision: 1.15 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.15 $ $RCSfile: action.c,v $";

#include "print_server.h"

extern CTNBOOLEAN traceFlag,
    gqueueFlag,
    sendBackFlag;
extern int gqID;
extern BFS_NODE bfsInstance;
extern BFB_NODE *bfbInstance;


static CONDITION
nactionBFBCallback(MSG_N_ACTION_REQ * nactionRequest,
		   MSG_N_ACTION_RESP * nactionResponse,
		   void *sendBack, DUL_PRESENTATIONCONTEXT * pc);
static CONDITION
unsupportedCallback(MSG_N_ACTION_REQ * nactionRequest,
		    MSG_N_ACTION_RESP * nactionResponse,
		    void *ctx, DUL_PRESENTATIONCONTEXT * pc);


/* actionRequest
**
** Purpose:
**	To send the N-ACTION response message back to the requesting SCU
**
** Parameter Dictionary:
**	association	Key which describes the association
**	ctx		Pointer to the context for this command
**	request		Pointer to N-ACTION request message
**
** Return Values:
**	SRV_NORMAL under error free conditions
**
**	SRV_CALLBACKABORTEDSERVICE
**	SRV_ILLEGALPARAMETER
**	SRV_NOCALLBACK
**	SRV_NORMAL
**	SRV_OBJECTBUILDFAILED
**	SRV_RESPONSEFAILED
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
actionRequest(DUL_ASSOCIATIONKEY ** association,
	      DUL_PRESENTATIONCONTEXT * ctx, MSG_N_ACTION_REQ ** request,
	      CTNBOOLEAN * sendBack)
{
    MSG_N_ACTION_RESP
	actionResponse = {
	MSG_K_N_ACTION_RESP, 0, 0, 0, 0, 0, NULL, "", "", "", 0
    };
    CONDITION
	cond;

    /* determine for what SOP class is the request made */
    if (!strcmp((*request)->classUID, DICOM_SOPCLASSBASICFILMSESSION)) {
	cond = SRV_NActionResponse(association, ctx, request, &actionResponse,
				   unsupportedCallback, sendBack, ".");
	if (cond != SRV_NORMAL) {
	    printf("In actionRequest : SRV_NActionResponse failed\n");
	}
    } else if (!strcmp((*request)->classUID, DICOM_SOPCLASSBASICFILMBOX)) {
	cond = SRV_NActionResponse(association, ctx, request, &actionResponse,
				   nactionBFBCallback, sendBack, ".");
	if (cond != SRV_NORMAL) {
	    printf("In actionRequest : SRV_NActionResponse failed\n");
	}
    } else {
	/* unsupported */
	cond = SRV_NActionResponse(association, ctx, request, &actionResponse,
				   unsupportedCallback, sendBack, ".");
	if (cond != SRV_NORMAL) {
	    printf("In actionRequest : SRV_NActionResponse failed\n");
	}
    }
    return cond;

}

/* nactionBFBCallback
**
** Purpose:
**	Callback routine called by SRV_NActionresponse used to build the
**	N-ACTION response message.
**
** Parameter Dictionary:
**	nactionRequest		Pointer to N-ACTION request message
**	nactionResponse		Pointer to N-ACTION response message
**	ctx			Pointer to context for this command
**	pc			Presentation context which describes SOP class
**
** Return Values:
**	SRV_NORMAL under error free conditions
**
**	SRV_CALLBACKABORTEDSERVICE
**	SRV_NORMAL
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
nactionBFBCallback(MSG_N_ACTION_REQ * nactionRequest,
		   MSG_N_ACTION_RESP * nactionResponse,
		   void *sendBack, DUL_PRESENTATIONCONTEXT * pc)
{
    CONDITION
    cond;
    PRN_BASICFILMBOX
	filmBox;		/* structure holding the Basic Film Box
				 * instance */
    BIB_NODE
	* bibNode;		/* image Box SOP instance */
    int
        index;
    GQ_ELEM
	gqElement;

    /* we build a response message */
    nactionResponse->type = MSG_K_N_ACTION_RESP;
    nactionResponse->messageIDRespondedTo = nactionRequest->messageID;
    strcpy(nactionResponse->classUID, nactionRequest->classUID);
    strcpy(nactionResponse->instanceUID, nactionRequest->instanceUID);
    nactionResponse->dataSetType = DCM_CMDDATANULL;
#ifdef ASG
    nactionResponse->actionReply = NULL;
#endif

    if (bfbInstance == NULL) {
	cond = COND_PushCondition(APP_ERROR(APP_MISSINGFILMBOX),
				  "nactionBFBCallback");
	nactionResponse->status = MSG_K_BFS_NOFILMBOXSOPINSTANCES;
	goto ERROR_RETURN;
    }
    /*
     * we check if the current instance of Basic Film Box is to be printed
     */
    if (strcmp(bfbInstance->bfbInstanceUID, nactionRequest->instanceUID) != 0) {
	cond = COND_PushCondition(APP_ERROR(APP_MISSINGFILMBOX),
				  "nactionBFBCallback");
	nactionResponse->status = MSG_K_NOSUCHOBJECTINSTANCE;
	goto ERROR_RETURN;
    }
    /* The SCP creates a PRINT JOB SOP instance */
    filmBox.type = PRN_K_BASICFILMBOX;
    strcpy(filmBox.refPrintJobClassUID, DICOM_SOPCLASSPRINTJOB);

    cond = UID_NewUID(UID_PRINTER, filmBox.refPrintJobInstanceUID);
    if (cond != UID_NORMAL) {
	cond = COND_PushCondition(APP_ERROR(APP_FAILURE), "UID_NewUID",
				  "nactionBFBCallback");
	nactionResponse->status = MSG_K_BFB_UNABLETOCREATEPRINTJOBSOPINSTANCE;
	goto ERROR_RETURN;
    }
    filmBox.bfbAttributeFlag = PRN_BFB_K_REFPRINTJOBSEQ;

#if 0
    nactionResponse->dataSetType = DCM_CMDDATAOTHER;
    cond = PRN_BuildObject(&filmBox, &nactionResponse->actionReply);
    if (cond != PRN_NORMAL) {
	cond = COND_PushCondition(APP_ERROR(APP_FAILURE), "PRN_BuildObject",
				  "nactionBFBCallback");
	updateOnProcessingFailure((void **) &nactionResponse,
	   APP_ERROR(APP_FAILURE), "PRN_BuildObject", "nactionBFBCallback");
	goto ERROR_RETURN;
    }
#else
    nactionResponse->dataSetType = DCM_CMDDATANULL;
#endif
    /*
     * Store the Referenced Print Job instance in the current Basic Film Box
     * instance
     */
    cond = PRN_BuildObject(&filmBox, &bfbInstance->bfbAttrib);
    if (cond != PRN_NORMAL) {
	cond = COND_PushCondition(APP_ERROR(APP_FAILURE), "PRN_BuildObject",
				  "nactionBFBCallback");
	updateOnProcessingFailure((void **) &nactionResponse,
	   APP_ERROR(APP_FAILURE), "PRN_BuildObject", "nactionBFBCallback");
	goto ERROR_RETURN;
    }
    /*
     * The next step in N-ACTION is to send the images of the current film
     * box for printing
     */
    bibNode = (BIB_NODE *) LST_Position(&bfbInstance->bibList,
					LST_Head(&bfbInstance->bibList));
    if (bibNode == NULL) {
	cond = COND_PushCondition(APP_ERROR(APP_MISSINGIMAGES),
				  "nactionBFBCallback");
	nactionResponse->status = MSG_K_BFB_NOIMAGEBOXSOPINSTANCES;
	goto ERROR_RETURN;
    }
    for (index = 0; index < (int) LST_Count(&bfbInstance->bibList);
	 index++) {
	/* send the image to the printing device */
	if (bibNode->bibAttrib == NULL) {
	    cond = COND_PushCondition(APP_ERROR(APP_MISSINGIMAGES),
				      "nactionBFBCallback");
	    nactionResponse->status = MSG_K_BFB_NOIMAGEBOXSOPINSTANCES;
	    fprintf(stderr, "WARNING!!!!!!!! Image position %d\n", index + 1);
	    COND_DumpConditions();
	}			/* else sendToEvb(bibNode->bibAttrib); */
	bibNode = (BIB_NODE *) LST_Next(&bfbInstance->bibList);
    }

    if (gqueueFlag) {
	gqElement.prnMessage = PRN_PRINTCOMMAND;
	(void) strcpy(gqElement.instanceUID, bfbInstance->bfbInstanceUID);
	(void) strcpy(gqElement.objFile, "\0");
	cond = GQ_Enqueue(gqID, (void *) &gqElement);
	if (cond != GQ_NORMAL) {
	    cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				      "GQ_Enqueue", "nactionBFBCallback");
	    updateOnProcessingFailure((void **) &nactionResponse,
		APP_ERROR(APP_FAILURE), "GQ_Enqueue", "nactionBFBCallback");
	    goto ERROR_RETURN;
	}
    }
    /* send back the action reply */
/******** ADD CODE HERE *******/
    /* everything is a success */
    nactionResponse->status = MSG_K_SUCCESS;
    return SRV_NORMAL;

ERROR_RETURN:
    COND_DumpConditions();
    return SRV_CALLBACKABORTEDSERVICE;
}
/* unsupportedCallback
**
** Purpose:
**	This callback routine is invoked by the SRV_NSetResponse
**	whenever service for the requested SOP class is unsupported
**
** Parameter Dictionary:
**	nactionRequest	Pointer to request message
**	nactionResponse	Pointer to response message
**	ctx		Context information which we ignore
**	pc		Presentation context which describes this SOP class
**
** Return Values:
**	SRV_CALLBACKABORTEDSERVICE
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
unsupportedCallback(MSG_N_ACTION_REQ * nactionRequest,
		    MSG_N_ACTION_RESP * nactionResponse,
		    void *ctx, DUL_PRESENTATIONCONTEXT * pc)
{
    nactionResponse->status = MSG_K_UNRECOGNIZEDOPERATION;
    return SRV_CALLBACKABORTEDSERVICE;
}
