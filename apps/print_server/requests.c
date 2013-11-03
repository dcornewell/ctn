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
** Module Name(s):	serviceRequests
**
**	Private routines:
**			serviceThisCommand
** Author, Date:	Aniruddha S. Gokhale, 27-June-1994
** Intent:		This module implements routines that service
**			the incoming requests on the association
**			that is already established.
** Last Update:		$Author: smm $, $Date: 1998-01-28 05:04:41 $
** Source File:		$RCSfile: requests.c,v $
** Revision:		$Revision: 1.11 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.11 $ $RCSfile: requests.c,v $";
#include "print_server.h"

static CONDITION
serviceThisCommand(DUL_NETWORKKEY ** network, DUL_ASSOCIATIONKEY ** association,
		   DUL_PRESENTATIONCONTEXT * ctx, MSG_TYPE messageType,
		   void **message, DUL_ASSOCIATESERVICEPARAMETERS * params);

static CONDITION
echoRequest(DUL_ASSOCIATIONKEY ** association,
	    DUL_PRESENTATIONCONTEXT * ctx, MSG_C_ECHO_REQ ** message);
static CONDITION
echoCallback(MSG_C_ECHO_REQ * echoRequest,
	     MSG_C_ECHO_RESP * echoResponse, void *ctx,
	     DUL_PRESENTATIONCONTEXT * pc);

extern CTNBOOLEAN traceFlag,
    sendBack;


/* serviceRequests
**
** Purpose:
**	This function reads requests from the network and services those
**	requests.
**
** Parameter Dictionary:
**	network		The key which is used to access the network.
**	association	The key which is used to access the association
**			on which requests are received.
**	service		The parameter list which describes the association.
**			This list includes the list of presentation contexts
**			for the association.
**	abortFlag	A test flag which tells this function to abort the
**			Association after it receives one image.
**	releaseDelay	Amount of time to delay (sleep) after release
**			is requested before acknowledging release.
**
** Return Values:
**
** Notes
**	In the concurrent server case, this function is always invoked
**	by the child server process.
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
serviceRequests(DUL_NETWORKKEY ** network, DUL_ASSOCIATIONKEY ** association,
		DUL_ASSOCIATESERVICEPARAMETERS * service)
{
    CONDITION
	cond = 1;
    DUL_PRESENTATIONCONTEXT
	* ctx;			/* handle to the accepted presentation
				 * context whose id matches that of the
				 * arriving command request */
    DUL_PRESENTATIONCONTEXTID
	ctxID;			/* presentation context id for the arriving
				 * command request */
    void
       *message;		/* handle to the received request */
    MSG_TYPE
	messageType;		/* the "TYPE" of the message received */
    CTNBOOLEAN
	networkLink = TRUE,
	commandServiced;

    while ((networkLink == TRUE) && !CTN_ERROR(cond)) {
	/* block until a request is received */
	cond = SRV_ReceiveCommand(association, service, DUL_BLOCK, 0, &ctxID,
				  NULL, &messageType, &message);
	if (cond == SRV_PEERREQUESTEDRELEASE) {	/* release association
						 * request */
	    networkLink = FALSE;
	    (void) DUL_AcknowledgeRelease(association);
	    (void) DUL_DropAssociation(association);
	} else if (cond == SRV_PEERABORTEDASSOCIATION) {
	    printf("Peer aborted Association\n");
	    networkLink = FALSE;
	    (void) DUL_DropAssociation(association);
	} else if (cond != SRV_NORMAL) {
	    (void) DUL_DropAssociation(association);
	    COND_DumpConditions();
	} else {
	    /*
	     * go thru the list of accepted presentation contexts and select
	     * one for which the context ID matches with the one received
	     * along with the command request
	     */
	    ctx = LST_Head(&service->acceptedPresentationContext);
	    if (ctx != NULL)
		(void) LST_Position(&service->acceptedPresentationContext, ctx);
	    commandServiced = FALSE;
	    while (ctx != NULL) {
		if (ctx->presentationContextID == ctxID) {
		    if (commandServiced) {
			/*
			 * the command has already been serviced and there is
			 * a context ID repeated in the list of presentation
			 * contexts
			 */
			fprintf(stderr,
			      "Context ID Repeat in serviceRequests (%d)\n",
				ctxID);
		    } else {
			/* command not yet serviced. So service it */
			cond = serviceThisCommand(network, association, ctx,
					    messageType, &message, service);
			if (cond == SRV_OPERATIONCANCELLED) {
			    printf("Operation cancelled\n");
			    (void) COND_PopCondition(TRUE);
			} else if (cond != SRV_NORMAL)
			    COND_DumpConditions();
			commandServiced = TRUE;
		    }
		}
		ctx = LST_Next(&service->acceptedPresentationContext);
	    }
	    if (!commandServiced) {
		fprintf(stderr, "In serviceRequests, context ID %d not found\n",
			ctxID);
		(void) DUL_DropAssociation(association);
		networkLink = FALSE;
	    }
	}
    }
    return cond;
}

/* serviceThisCommand
**
** Purpose:
**	This function serves as a dispatch routine for the commands
**	that can be received from the network.  It uses a case statement
**	to identify the command and call the function which will
**	respond to the request.
**
** Parameter Dictionary:
**	network		Handle to the network connection
**	association	The key which is used to access the association
**			on which requests are received.
**	ctx		Pointer to the presentation context for this command
**	messageType	The message type
**	message		Pointer to a structure which contains the message.
**			We will use "messageType" to get the proper type.
**	params		Handle to the presentation service parameters
**
** Return Values:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
serviceThisCommand(DUL_NETWORKKEY ** network, DUL_ASSOCIATIONKEY ** association,
		   DUL_PRESENTATIONCONTEXT * ctx, MSG_TYPE messageType,
		   void **message, DUL_ASSOCIATESERVICEPARAMETERS * params)
{
    CONDITION
    cond;

    if (traceFlag) {
	fprintf(stdout, "Received message is :\n");
	MSG_DumpMessage(*message, stdout);
    }
    switch (messageType) {
    case MSG_K_C_ECHO_REQ:
	if (traceFlag)
	    printf("SCP : Received C-Echo request for class %s\n",
		   ((MSG_N_GET_REQ *) * message)->classUID);
	cond = echoRequest(association, ctx, (MSG_C_ECHO_REQ **) message);
	if (traceFlag && (cond == SRV_NORMAL))
	    printf("SCP : C-Echo Response Sent\n");
	break;
    case MSG_K_N_GET_REQ:	/* N-GET request */
	if (traceFlag)
	    printf("SCP : Received N-GET request for class %s\n",
		   ((MSG_N_GET_REQ *) * message)->classUID);
	cond = getRequest(association, ctx, (MSG_N_GET_REQ **) message);
	if (traceFlag && (cond == SRV_NORMAL))
	    printf("SCP : N-GET Response Sent\n");
	break;

    case MSG_K_N_CREATE_REQ:	/* N-CREATE Request */
	if (traceFlag)
	    printf("SCP : Received N-CREATE request for class %s\n",
		   ((MSG_N_CREATE_REQ *) * message)->classUID);
	cond = createRequest(association, ctx, (MSG_N_CREATE_REQ **) message,
			     &sendBack);
	if (traceFlag && (cond == SRV_NORMAL))
	    printf("SCP : N-CREATE Response Sent\n");
	break;

    case MSG_K_N_SET_REQ:	/* N-SET request */
	if (traceFlag)
	    printf("SCP : Received N-SET request for class %s\n",
		   ((MSG_N_SET_REQ *) * message)->classUID);
	cond = setRequest(association, ctx, (MSG_N_SET_REQ **) message,
			  &sendBack);
	if (traceFlag && (cond == SRV_NORMAL))
	    printf("SCP : N-SET Response Sent\n");
	break;

    case MSG_K_N_ACTION_REQ:
	if (traceFlag)
	    printf("SCP : Received N-ACTION request for class %s\n",
		   ((MSG_N_ACTION_REQ *) * message)->classUID);
	cond = actionRequest(association, ctx, (MSG_N_ACTION_REQ **) message,
			     &sendBack);
	if (traceFlag && (cond == SRV_NORMAL))
	    printf("SCP : N-ACTION Response Sent\n");
	break;

    case MSG_K_N_DELETE_REQ:
	if (traceFlag)
	    printf("SCP : Received N-DELETE request for class %s\n",
		   ((MSG_N_DELETE_REQ *) * message)->classUID);
	cond = deleteRequest(association, ctx, (MSG_N_DELETE_REQ **) message);
	if (traceFlag && (cond == SRV_NORMAL))
	    printf("SCP : N-DELETE Response Sent\n");
	break;

    default:
	fprintf(stderr, "Unimplemented message type: %d\n", messageType);
	cond = COND_PushCondition(APP_FAILURE, (char *) APP_ERROR(APP_FAILURE));
	break;
    }
    return cond;
}
/* echoRequest
**
** Purpose:
**      This function responds to an echo request from the network.
**      It creates an echo response message with a status of success
**      and sends the message to the peer application.
**
** Parameter Dictionary:
**      association     They key which is used to access the association
**                      on which requests are received.
**      ctx             Pointer to the presentation context for this command
**      message         Pointer to the MSG_C_ECHO_REQ message that was
**                      received by the server.
**
** Return Values:
**
**      SRV_CALLBACKABORTEDSERVICE
**      SRV_ILLEGALPARAMETER
**      SRV_NOCALLBACK
**      SRV_NORMAL
**      SRV_OBJECTBUILDFAILED
**      SRV_RESPONSEFAILED
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/

static CONDITION
echoRequest(DUL_ASSOCIATIONKEY ** association,
	    DUL_PRESENTATIONCONTEXT * ctx, MSG_C_ECHO_REQ ** echoReq)
{
    MSG_C_ECHO_RESP
    echoResponse = {
	MSG_K_C_ECHO_RESP, 0, 0, DCM_CMDDATANULL, 0, ""
    };

    return SRV_CEchoResponse(association, ctx, echoReq,
			     &echoResponse, echoCallback, NULL, "");
}

/* echoCallback
**
** Purpose:
**      Call back routine provided by the service provider. It is invoked
**      by the SRV Echo Response function.
**
** Parameter Dictionary:
**      request         Pointer to C-Echo Request Message
**      response        Pointer to C-Echo Response Message
**	ctx		My context, ignored here
**	pc		Presentation context describes this SOP Class
**
** Return Values:
**      SRV_NORMAL
**
** Notes:
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/
static CONDITION
echoCallback(MSG_C_ECHO_REQ * request,
	MSG_C_ECHO_RESP * response, void *ctx, DUL_PRESENTATIONCONTEXT * pc)
{
    if (traceFlag)
	printf("Echo Request Received/Acknowledged\n");

    response->dataSetType = DCM_CMDDATANULL;
    response->messageIDRespondedTo = request->messageID;
    response->status = MSG_K_SUCCESS;
    strcpy(response->classUID, request->classUID);

    return SRV_NORMAL;
}
