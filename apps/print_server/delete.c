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
** Module Name(s):	deleteRequest
**
**	Private Routines
**			deleteCallback
** Author, Date:
** Intent:
** Last Update:		$Author: smm $, $Date: 1996-08-07 21:49:54 $
** Source File:		$RCSfile: delete.c,v $
** Revision:		$Revision: 1.8 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.8 $ $RCSfile: delete.c,v $";

#include "print_server.h"

extern CTNBOOLEAN traceFlag,
    gqueueFlag,
    sendBack;
extern int gqID;
extern BFS_NODE bfsInstance;
extern BFB_NODE *bfbInstance;

static CONDITION
deleteCallback(MSG_N_DELETE_REQ * deleteRequest,
	       MSG_N_DELETE_RESP * deleteResponse,
	       void *ctx, DUL_PRESENTATIONCONTEXT * pc);


/* deleteRequest
**
** Purpose:
**	Build a N-DELETE response message and send it to the requesting SCU
**
** Parameter Dictionary:
**	association	Key which describes an association
**	ctx		Pointer to context for this command
**	request		Pointer to N_DELETE request message
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
**
*/
CONDITION
deleteRequest(DUL_ASSOCIATIONKEY ** association,
	      DUL_PRESENTATIONCONTEXT * ctx, MSG_N_DELETE_REQ ** request)
{
    CONDITION
	cond;
    MSG_N_DELETE_RESP
	deleteResponse;

    memset(&deleteResponse, 0, sizeof(deleteResponse));
    deleteResponse.type = MSG_K_N_DELETE_RESP;

    cond = SRV_NDeleteResponse(association, ctx, request, &deleteResponse,
			       deleteCallback, NULL, ".");
    if (cond != SRV_NORMAL) {
	printf("In deleteRequest : SRV_NDeleteResponse failed\n");
	return cond;
    }
    return SRV_NORMAL;
}
/* deleteCallback
**
** Purpose:
**	To delete the hierarchy staring at FILM SESSION or FILM BOX instance
**	as specified by the request. This routine is invoked by
**	SRV_NDeleteResponse as a callback routine.
**
** Parameter Dictionary:
**	ndeleteRequest	Pointer to N-DELETE request message
**	ndeleteResponse	Pointer to N-DELETE response message
**	ctx		Pointer to context for this command
**	pc		Presentation context which describes this SOP class
**
** Return Values:
**	SRV_NORMAL under error free conditions
**
**	SRV_CALLBACKABORTEDSERVICE
**	SRV_NORMAL
**
** Algorithm:
**
*/
static CONDITION
deleteCallback(MSG_N_DELETE_REQ * ndeleteRequest,
	       MSG_N_DELETE_RESP * ndeleteResponse,
	       void *ctx, DUL_PRESENTATIONCONTEXT * pc)
{
    GQ_ELEM
    gqElement;
    CONDITION
	cond;


    /* we construct the response message */
    ndeleteResponse->type = MSG_K_N_DELETE_RESP;
    ndeleteResponse->messageIDRespondedTo = ndeleteRequest->messageID;
    ndeleteResponse->dataSetType = DCM_CMDDATANULL;
    strcpy(ndeleteResponse->classUID, ndeleteRequest->classUID);
    strcpy(ndeleteResponse->instanceUID, ndeleteRequest->instanceUID);
    ndeleteResponse->conditionalFields |=
	MSG_K_N_DELETERESP_AFFECTEDCLASSUID |
	MSG_K_N_DELETERESP_AFFECTEDINSTANCEUID;
    cond = deleteHierarchy(ndeleteRequest->classUID);
    if (cond != APP_NORMAL) {
	cond = COND_PushCondition(APP_ERROR(cond), "deleteHierarchy",
				  "ndeleteCallback");
	updateOnProcessingFailure((void **) &ndeleteResponse,
	      APP_ERROR(APP_FAILURE), "deleteHierarchy", "ndeleteCallback");
	goto ERROR_RETURN;
    }
    if (strcmp(ndeleteResponse->classUID,
	       DICOM_SOPCLASSBASICFILMSESSION) == 0) {
	/* enqueue the information only if it is a Film Session SOP Class */
	if (gqueueFlag) {
	    gqElement.prnMessage = PRN_ENDSESSION;
	    (void) strcpy(gqElement.instanceUID,
			  bfsInstance.bfsInstanceUID);
	    (void) strcpy(gqElement.objFile, "\0");
	    cond = GQ_Enqueue(gqID, (void *) &gqElement);
	    if (cond != GQ_NORMAL) {
		cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
					  "GQ_Enqueue", "ndeleteCallback");
		updateOnProcessingFailure((void **) &ndeleteResponse,
		   APP_ERROR(APP_FAILURE), "GQ_Enqueue", "ndeleteCallback");
		goto ERROR_RETURN;
	    }
	}
    }
    printf("*** After N-DELETE request for SOP class %s\n",
	   ndeleteRequest->classUID);
    showHierarchy();

    ndeleteResponse->status = MSG_K_SUCCESS;
    return SRV_NORMAL;

ERROR_RETURN:
    COND_DumpConditions();
    return SRV_CALLBACKABORTEDSERVICE;
}
