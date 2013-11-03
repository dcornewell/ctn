/*
+-+-+-+-+-+-+-+-+-
*/
/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	Stephen Moore, 11-Feb-94
** Intent:
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:44:52 $
** Source File:		$RCSfile: requests.c,v $
** Revision:		$Revision: 1.5 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.5 $ $RCSfile: requests.c,v $";

#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#ifdef MACH
#include <unistd.h>
#endif
#endif

#include "dicom.h"
#include "tbl.h"
#include "lst.h"
#include "dicom_uids.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_ie.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "tbl.h"
#include "manage.h"
#include "fis.h"

#include "ris_gateway.h"

extern char *controlDatabase;
extern int logLevel;

static CONDITION
serviceThisCommand(DUL_NETWORKKEY ** network, DUL_ASSOCIATIONKEY ** association,
		   DUL_PRESENTATIONCONTEXT * ctx, MSG_TYPE messageType,
		   void **message, DUL_ASSOCIATESERVICEPARAMETERS * params,
		   DMAN_HANDLE ** handle, FIS_HANDLE ** fis);
static CONDITION
echoRequest(DUL_ASSOCIATIONKEY ** association,
	    DUL_PRESENTATIONCONTEXT * ctx, MSG_C_ECHO_REQ ** message);
static CONDITION
echoCallback(MSG_C_ECHO_REQ * echoRequest,
	     MSG_C_ECHO_RESP * echoResponse, void *ctx,
	     DUL_PRESENTATIONCONTEXT * pc);
static CTNBOOLEAN silent = FALSE;
static CTNBOOLEAN waitFlag = FALSE;


/* serviceRequests
**
** Purpose:
**	This function reads requests from the network and services those
**	requests.
**
** Parameter Dictionary:
**	network		The key which is used to access the network.
**	association	They key which is used to access the association
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
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
serviceRequests(DUL_NETWORKKEY ** network, DUL_ASSOCIATIONKEY ** association,
		DUL_ASSOCIATESERVICEPARAMETERS * service)
{
    CONDITION cond,
        processCond;
    DUL_PRESENTATIONCONTEXT
	* ctx;
    DUL_PRESENTATIONCONTEXTID
	ctxID;
    void
       *message;
    MSG_TYPE
	messageType;
    CTNBOOLEAN
	networkLink = TRUE,
	commandServiced;
    DMAN_HANDLE
	* handle;
    DMAN_FISACCESS
	FISAccess;
    FIS_HANDLE
	* FISHandle;
    char
        localApplication[DUL_LEN_TITLE + 1],
        remoteApplication[DUL_LEN_TITLE + 1];

    strcpy(localApplication, service->calledAPTitle);
    strcpy(remoteApplication, service->callingAPTitle);
    cond = DMAN_Open(controlDatabase, service->callingAPTitle,
		     service->calledAPTitle, &handle);

    if (cond == DMAN_NORMAL) {
	cond = DMAN_LookupFISAccess(&handle, service->calledAPTitle,
				    &FISAccess);
    }
    if (cond == DMAN_NORMAL) {
	cond = FIS_Open(FISAccess.DbKey, &FISHandle);
    }
    while ((networkLink == TRUE) && !CTN_ERROR(cond)) {
	cond = SRV_ReceiveCommand(association, service, DUL_BLOCK, 0, &ctxID,
				  NULL, &messageType, &message);
	if (cond == SRV_PEERREQUESTEDRELEASE) {
	    networkLink = FALSE;
	    (void) DUL_AcknowledgeRelease(association);
	    (void) DUL_DropAssociation(association);
	} else if (cond == SRV_PEERABORTEDASSOCIATION) {
	    networkLink = FALSE;
	    (void) DUL_DropAssociation(association);
	} else if (cond != SRV_NORMAL) {
	    (void) DUL_DropAssociation(association);
	    COND_DumpConditions();
	    cond = 0;
	} else {
	    ctx = LST_Head(&service->acceptedPresentationContext);
	    if (ctx != NULL)
		(void) LST_Position(&service->acceptedPresentationContext, ctx);
	    commandServiced = FALSE;
	    while (ctx != NULL) {
		if (ctx->presentationContextID == ctxID) {
		    if (commandServiced) {
			fprintf(stderr,
			      "Context ID Repeat in serviceRequests (%d)\n",
				ctxID);
		    } else {
			cond = serviceThisCommand(network, association, ctx,
					     messageType, &message, service,
						  &handle, &FISHandle);
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
    processCond = processEvents(network, &handle, &FISHandle, localApplication,
				remoteApplication);
    (void) DMAN_Close(&handle);
    (void) FIS_Close(&FISHandle);
    if (processCond != 1)
	COND_DumpConditions();
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
**	association	They key which is used to access the association
**			on which requests are received.
**	ctx		Pointer to the presentation context for this command
**	messageType	The type of message that we are to recognize.
**	message		Pointer to a structure which contains the message.
**			We will use "messageType" to get the proper type.
**
** Return Values:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
serviceThisCommand(DUL_NETWORKKEY ** network, DUL_ASSOCIATIONKEY ** association,
		   DUL_PRESENTATIONCONTEXT * ctx, MSG_TYPE messageType,
		   void **message, DUL_ASSOCIATESERVICEPARAMETERS * params,
		   DMAN_HANDLE ** handle, FIS_HANDLE ** fis)
{
    CONDITION
    cond;
    MSG_GENERAL
	* general;

    general = *(MSG_GENERAL **) message;

    if (logLevel > 0) {
	fprintf(stdout, "\nA new request was received:\n");
	MSG_DumpMessage((void *) general, stdout);
    }
    switch (messageType) {
    case MSG_K_C_ECHO_REQ:
	cond = echoRequest(association, ctx, (MSG_C_ECHO_REQ **) message);
	break;
    case MSG_K_N_EVENT_REPORT_REQ:
	cond = NEventReportRequest(params, association, handle,
			     ctx, (MSG_N_EVENT_REPORT_REQ **) message, fis);
	break;
    default:
	fprintf(stderr, "Unimplemented message type: %d\n", messageType);
	cond = 1;
	break;
    }
    return cond;
}


/* echoRequest
**
** Purpose:
**	This function responds to an echo request from the network.
**	It creates an echo response message with a status of success
**	and sends the message to the peer application.
**
** Parameter Dictionary:
**	association	They key which is used to access the association
**			on which requests are received.
**	ctx		Pointer to the presentation context for this command
**	message		Pointer to the MSG_C_ECHO_REQ message that was
**			received by the server.
**
** Return Values:
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
**	Call back routine provided by the service provider. It is invoked
**	by the SRV Echo Response function.
**
** Parameter Dictionary:
**	request		Pointer to C-Echo Request Message
**	response	Pointer to C-Echo Response Message
**	ctx		Pointer to a context structure which is not used
**	pc		Pointer to presentation context for this SOP class
**
** Return Values:
**	SRV_NORMAL
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
echoCallback(MSG_C_ECHO_REQ * request,
	     MSG_C_ECHO_RESP * response, void *ctx,
	     DUL_PRESENTATIONCONTEXT * pc)
{
    if (!silent)
	printf("Echo Request Received/Acknowledged\n");

    response->dataSetType = DCM_CMDDATANULL;
    response->messageIDRespondedTo = request->messageID;
    response->status = MSG_K_SUCCESS;
    strcpy(response->classUID, request->classUID);

    return SRV_NORMAL;
}
