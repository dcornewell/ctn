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
** @$=@$=@$=
*/
/*
**				DICOM 93
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	main
**			usageerrror
**			serviceRequests
**			serviceThisCommand
**			echoRequest
**			echoCallback
**			eventReportRequest
**			eventReportCallback
**			supportedClass
**			associationCheck
** Author, Date:	Stephen M. Moore, 17-Apr-93
** Intent:		This program is an example and test program
**			which demonstrates the DICOM Upper Layer (DUL)
**			software and service classes for the DICOM 93 software.
**			It provides the storage and verification service
**			classes.
** Usage:		simple_storage [-a] [-f] [-p] [-t trips] [-v] port
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:45:53 $
** Source File:		$RCSfile: simple_pacs.c,v $
** Revision:		$Revision: 1.15 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.15 $ $RCSfile: simple_pacs.c,v $";

#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#ifndef MACH
#include <unistd.h>
#endif
#endif

#include "dicom.h"
#include "ctnthread.h"
#include "dicom_uids.h"
#include "lst.h"
#include "condition.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "simple_pacs.h"

typedef struct {
    DUL_NETWORKKEY **network;
    DUL_ASSOCIATESERVICEPARAMETERS *params;
}   MOVE_PARAMS;

static void usageerror();
static CONDITION
serviceRequests(DUL_NETWORKKEY ** network, DUL_ASSOCIATIONKEY ** association,
		DUL_ASSOCIATESERVICEPARAMETERS * service);
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
	     DUL_PRESENTATIONCONTEXT * pci);
static CONDITION
eventReportRequest(DUL_ASSOCIATIONKEY ** association,
	  DUL_PRESENTATIONCONTEXT * ctx, MSG_N_EVENT_REPORT_REQ ** request);
static CONDITION
eventReportCallback(MSG_N_EVENT_REPORT_REQ * request,
		    MSG_N_EVENT_REPORT_RESP * response, void *string,
		    DUL_PRESENTATIONCONTEXT * pc);

static int supportedClass(char *abstractSyntax, char **classArray);
static CONDITION
associationCheck(DUL_ASSOCIATESERVICEPARAMETERS * params, CTNBOOLEAN forgive,
		 DUL_ABORTITEMS * abortItems);

static CTNBOOLEAN silent = FALSE;

main(int argc, char **argv)
{
    CONDITION
	cond;
    DUL_NETWORKKEY
	* network = NULL;
    DUL_ASSOCIATIONKEY
	* association = NULL;
    DUL_ASSOCIATESERVICEPARAMETERS
	service;
    DUL_PRESENTATIONCONTEXT
	* requestedCtx;
    DUL_ABORTITEMS
	abortItems;
    int
        port,
        trips = 2048,		/* Trips through the main loop */
        classCount = 0;
    CTNBOOLEAN
	verboseDUL = FALSE,
	verboseSRV = FALSE,
	verboseDCM = FALSE,
	abortFlag = FALSE,
	forgiveFlag = FALSE,
	paramsFlag = FALSE,
	acceptFlag,
	drop;
    DUL_SC_ROLE
	scRole;

    char *classArray[] = {
	DICOM_SOPCLASSVERIFICATION,
	DICOM_SOPCLASSDETACHEDPATIENTMGMT,
	DICOM_SOPCLASSDETACHEDVISITMGMT,
	DICOM_SOPCLASSDETACHEDPATIENTMGMTMETA,
	DICOM_SOPCLASSDETACHEDSTUDYMGMT,
	DICOM_SOPCLASSDETACHEDRESULTSMGMT,
	DICOM_SOPCLASSDETACHEDINTERPRETMGMT,
	DICOM_SOPCLASSDETACHEDRESULTSMGMTMETA,
    ""};


    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'a':
	    abortFlag = TRUE;
	    break;
	case 'd':
	    argc--;
	    argv++;
	    if (argc < 1)
		usageerror();
	    if (strcmp(*argv, "DCM") == 0)
		verboseDCM = TRUE;
	    else if (strcmp(*argv, "DUL") == 0)
		verboseDUL = TRUE;
	    else if (strcmp(*argv, "SRV") == 0)
		verboseSRV = TRUE;
	    else
		usageerror();
	    break;
	case 'f':
	    forgiveFlag = TRUE;
	    break;
	case 'p':
	    paramsFlag = TRUE;
	    break;
	case 's':
	    silent = TRUE;
	    break;
	case 't':
	    argc--;
	    argv++;
	    if (argc < 1)
		usageerror();
	    if (sscanf(*argv, "%d", &trips) != 1)
		usageerror();
	    break;
	case 'v':
	    verboseDUL = TRUE;
	    verboseSRV = TRUE;
	    break;
	default:
	    (void) fprintf(stderr, "Unrecognized option: %c\n", *(argv[0] + 1));
	    break;
	}
    }

    if (argc < 1)
	usageerror();
    if (sscanf(*argv, "%d", &port) != 1)
	usageerror();

    THR_Init();
    DCM_Debug(verboseDCM);
    DUL_Debug(verboseDUL);
    SRV_Debug(verboseSRV);

    if (port < 1024) {
	if (geteuid() != 0) {
	    char errmsg[] =
	    "To use this port (%d), you must run as root or the application must be\n\
setuid root (see chmod)\n";

	    fprintf(stderr, errmsg, port);
	    perror("");
	    THR_Shutdown();
	    return (1);
	}
    }
    cond = DUL_InitializeNetwork(DUL_NETWORK_TCP, DUL_AEBOTH,
		 (void *) &port, DUL_TIMEOUT, DUL_ORDERBIGENDIAN, &network);
    if (cond != DUL_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	return (1);
    }
/*  Go back to the proper uid so we don't mess with things we don't own.
*/

    (void) setuid(getuid());

    while (!CTN_ERROR(cond) && (trips-- > 0)) {
	drop = FALSE;
	(void) memset(&service, 0, sizeof(service));
	service.maxPDU = 8192;
	strcpy(service.calledImplementationClassUID,
	       MIR_IMPLEMENTATIONCLASSUID);
	strcpy(service.calledImplementationVersionName,
	       MIR_IMPLEMENTATIONVERSIONNAME);
	cond = DUL_ReceiveAssociationRQ(&network, DUL_BLOCK,
					&service, &association);
	if (cond != DUL_NORMAL) {
	    COND_DumpConditions();
	    THR_Shutdown();
	    return (1);
	}
	acceptFlag = TRUE;
	cond = associationCheck(&service, forgiveFlag, &abortItems);
	if (CTN_ERROR(cond)) {
	    acceptFlag = FALSE;
	    fprintf(stderr, "Incorrect Association Request\n");
	    COND_DumpConditions();
	    cond = DUL_RejectAssociationRQ(&association, &abortItems);
	    if (cond != DUL_NORMAL) {
		fprintf(stderr, "Unable to cleanly reject Association\n");
		COND_DumpConditions();
	    }
	} else if (!CTN_SUCCESS(cond)) {
	    COND_DumpConditions();
	}
	if (acceptFlag) {
	    if (verboseDUL || paramsFlag)
		DUL_DumpParams(&service);
	    requestedCtx = LST_Head(&service.requestedPresentationContext);
	    if (requestedCtx != NULL)
		(void) LST_Position(&service.requestedPresentationContext,
				    requestedCtx);
	    classCount = 0;
	    while (requestedCtx != NULL) {
		if (supportedClass(requestedCtx->abstractSyntax, classArray)) {
		    switch (requestedCtx->proposedSCRole) {
		    case DUL_SC_ROLE_DEFAULT:
			scRole = DUL_SC_ROLE_DEFAULT;
			break;
		    case DUL_SC_ROLE_SCU:
			scRole = DUL_SC_ROLE_SCU;
			break;
		    case DUL_SC_ROLE_SCP:
			scRole = DUL_SC_ROLE_DEFAULT;
			break;
		    case DUL_SC_ROLE_SCUSCP:
			scRole = DUL_SC_ROLE_SCU;
			break;
		    default:
			scRole = DUL_SC_ROLE_DEFAULT;
			break;
		    }
		    cond = SRV_AcceptServiceClass(requestedCtx, scRole,
						  &service);
		    if (cond == SRV_NORMAL) {
			classCount++;
		    } else {
			printf("SRV Facility rejected SOP Class: %s\n",
			       requestedCtx->abstractSyntax);
			COND_DumpConditions();
		    }
		} else {
		    printf("Simple server rejects: %s\n",
			   requestedCtx->abstractSyntax);
		    cond = SRV_RejectServiceClass(requestedCtx,
				    DUL_PRESENTATION_REJECT_USER, &service);
		    if (cond != SRV_NORMAL) {
			drop = TRUE;
		    }
		}
		requestedCtx = LST_Next(&service.requestedPresentationContext);
	    }
	    printf("Supported classes: %d\n", classCount);
	    if (paramsFlag)
		DUL_DumpParams(&service);

	    if (drop) {
		(void) DUL_DropAssociation(&association);
		COND_DumpConditions();
	    } else {
		cond = DUL_AcknowledgeAssociationRQ(&association, &service);
		if (cond != DUL_NORMAL) {
		    COND_DumpConditions();
		    THR_Shutdown();
		    return (1);
		}
		if (verboseDUL)
		    DUL_DumpParams(&service);
		cond = serviceRequests(&network, &association, &service);
		if (cond == SRV_PEERREQUESTEDRELEASE)
		    cond = SRV_NORMAL;
		if (!abortFlag) {
		    (void) DUL_DropAssociation(&association);
		}
	    }
	}
	(void) DUL_ClearServiceParameters(&service);
    }
    if (abortFlag) {
	THR_Shutdown();
	return (1);
    }
    (void) DUL_DropNetwork(&network);
    THR_Shutdown();
    return (0);
}


/* usageerror
**
** Purpose:
**	Print the usage text for this program and exit
**
** Parameter Dictionary:
**	None
**
** Return Values:
**	None
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static void
usageerror()
{
    char usage[] =
    "Usage: simple_storage [-a] [-d FAC] [-p] [-t trips] [-v] port\n";

    (void) fprintf(stderr, usage);
    exit(1);
}


/* serviceRequests
**
** Purpose:
**	This function reads requests from the network and services those
**	requests.
**
** Parameter Dictionary:
**	association	They key which is used to access the association
**			on which requests are received.
**	service		The parameter list which describes the association.
**			This list includes the list of presentation contexts
**			for the association.
**
** Return Values:
**
**	SRV_CALLBACKABORTEDSERVICE
**	SRV_ILLEGALPARAMETER
**	SRV_NOCALLBACK
**	SRV_NORMAL
**	SRV_OBJECTBUILDFAILED
**	SRV_PEERABORTEDASSOCIATION
**	SRV_PEERREQUESTEDRELEASE
**	SRV_RESPONSEFAILED
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
serviceRequests(DUL_NETWORKKEY ** network, DUL_ASSOCIATIONKEY ** association,
		DUL_ASSOCIATESERVICEPARAMETERS * service)
{
    CONDITION
    cond;
    DUL_PRESENTATIONCONTEXT
	* ctx;
    DUL_PRESENTATIONCONTEXTID
	ctxID;
    void
       *message;
    MSG_TYPE
	messageType;
    CTNBOOLEAN
	networkLink = TRUE;

    cond = SRV_NORMAL;
    while ((networkLink == TRUE) && !CTN_ERROR(cond)) {
	cond = SRV_ReceiveCommand(association, service, DUL_BLOCK, 0, &ctxID,
				  NULL, &messageType, &message);
	if (cond == SRV_PEERREQUESTEDRELEASE) {
	    networkLink = FALSE;
	    (void) DUL_AcknowledgeRelease(association);
	} else if (cond == SRV_PEERABORTEDASSOCIATION) {
	    networkLink = FALSE;
	} else if (cond != SRV_NORMAL) {
	    COND_DumpConditions();
	    cond = 0;
	} else {
	    ctx = LST_Head(&service->acceptedPresentationContext);
	    if (ctx != NULL)
		(void) LST_Position(&service->acceptedPresentationContext, ctx);
	    while (ctx != NULL) {
		if (ctx->presentationContextID == ctxID) {
		    cond = serviceThisCommand(network, association, ctx,
					    messageType, &message, service);
		    if (cond != SRV_NORMAL)
			COND_DumpConditions();
		}
		ctx = LST_Next(&service->acceptedPresentationContext);
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
**	association	They key which is used to access the association
**			on which requests are received.
**	ctx		Pointer to the presentation context for this command
**	messageType	The type of message that we are to recognize.
**	message		Pointer to a structure which contains the message.
**			We will use "messageType" to get the proper type.
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
serviceThisCommand(DUL_NETWORKKEY ** network, DUL_ASSOCIATIONKEY ** association,
		   DUL_PRESENTATIONCONTEXT * ctx, MSG_TYPE messageType,
		   void **message, DUL_ASSOCIATESERVICEPARAMETERS * params)
{
    CONDITION
    cond;

    MSG_DumpMessage(message, stdout);
    switch (messageType) {
    case MSG_K_C_ECHO_REQ:
	cond = echoRequest(association, ctx, (MSG_C_ECHO_REQ **) message);
	break;
    case MSG_K_N_EVENT_REPORT_REQ:
	cond = eventReportRequest(association, ctx,
				  (MSG_N_EVENT_REPORT_REQ **) message);
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
**	Application specific call back routine called by the C-ECHO primitive
**
** Parameter Dictionary:
**	request		Handle to C-ECHO request message
**	response	Handle to C-ECHO response message
**	ctx		Context inormation which we ignore
**	pci		The presentation context for this call (ignored)
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
	     DUL_PRESENTATIONCONTEXT * pci)
{
    if (!silent)
	printf("Echo Request Received/Acknowledged\n");

    response->dataSetType = DCM_CMDDATANULL;
    response->messageIDRespondedTo = request->messageID;
    response->status = MSG_K_SUCCESS;
    strcpy(response->classUID, request->classUID);

    return SRV_NORMAL;
}


/* eventReportRequest
**
** Purpose:
**
** Parameter Dictionary:
**	association	They key which is used to access the association
**			on which requests are received.
**	ctx		Pointer to the presentation context for this command
**	message
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
eventReportRequest(DUL_ASSOCIATIONKEY ** association,
	   DUL_PRESENTATIONCONTEXT * ctx, MSG_N_EVENT_REPORT_REQ ** request)
{
    MSG_N_EVENT_REPORT_RESP
    response;

    response.type = MSG_K_N_EVENT_REPORT_RESP;
    return SRV_NEventReportResponse(association, ctx, request, &response,
				eventReportCallback, "callback string", "");
}

/* eventReportCallback
**
** Purpose:
**	Application specific callback routine called by the N-EVENT-REPORT
**	primitive
**
** Parameter Dictionary:
**	request		Handle to the N-EVENT-REPORT request message
**	response	Handle to the N-EVENT-REPORT response message
**	string		Context specific information
**	pc		Presentaton context which describes current SOP Class
**
** Return Values:
**	SRV_NORMAL
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
eventReportCallback(MSG_N_EVENT_REPORT_REQ * request,
		    MSG_N_EVENT_REPORT_RESP * response, void *string,
		    DUL_PRESENTATIONCONTEXT * pc)
{
    if (!silent)
	(void) DCM_DumpElements(&request->dataSet, 0);

    response->status = MSG_K_SUCCESS;
    response->dataSetType = DCM_CMDDATANULL;

    return SRV_NORMAL;
}

/* supportedClass
**
** Purpose:
**	This function determines if an SOP class is supported by this
**	application.  The caller supplies an abstract syntax which defines
**	an SOP Class that is proposed for an Association.  The caller
**	also provides a static list of supported classes which this
**	routine can search.  The function returns 1 if the requested
**	class is found in the list and 0 if not.
**
** Parameter Dictionary:
**	Define the parameters to the function
**
** Return Values:
**	1 => success (class is supported)
**	0 => failure
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static int
supportedClass(char *abstractSyntax, char **classArray)
{
    while (strlen(*classArray) != 0) {
	if (strcmp(abstractSyntax, *classArray) == 0)
	    return 1;
	else
	    classArray++;
    }
    return 0;
}

/* associationCheck
**
** Purpose:
**	Verify the validity of the Association service parameters.
**
** Parameter Dictionary:
**	params		Service parameters for the Association
**	forgive		Decides whether to issue a warning or an error
**	abortItems	Handle to s structure indicating reasons for
**			invalid Association parameters
**
** Return Values:
**
**	APP_ILLEGALSERVICEPARAMETER
**	APP_MISSINGSERVICEPARAMETER
**	APP_NORMAL
**	APP_PARAMETERFAILURE
**	APP_PARAMETERWARNINGS
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
associationCheck(DUL_ASSOCIATESERVICEPARAMETERS * params, CTNBOOLEAN forgive,
		 DUL_ABORTITEMS * abortItems)
{
    CONDITION
    cond = APP_NORMAL;

    if (strcmp(params->applicationContextName, DICOM_STDAPPLICATIONCONTEXT) != 0) {
	abortItems->result = DUL_REJECT_PERMANENT;
	abortItems->source = DUL_ULSU_REJECT;
	abortItems->reason = DUL_ULSU_REJ_UNSUP_APP_CTX_NAME;
	cond = COND_PushCondition(APP_ILLEGALSERVICEPARAMETER,
				  APP_Message(APP_ILLEGALSERVICEPARAMETER),
		"Application Context Name", params->applicationContextName);
    }
    if (strlen(params->callingImplementationClassUID) == 0) {
	abortItems->result = DUL_REJECT_PERMANENT;
	abortItems->source = DUL_ULSU_REJECT;
	abortItems->reason = DUL_ULSU_REJ_NOREASON;

	cond = COND_PushCondition(APP_MISSINGSERVICEPARAMETER,
				  APP_Message(APP_MISSINGSERVICEPARAMETER),
				  "Requestor's ImplementationClassUID");
    }
    if (strcmp(params->calledAPTitle, SIMPLE_STORAGE_AE_TITLE) != 0) {
	abortItems->result = DUL_REJECT_PERMANENT;
	abortItems->source = DUL_ULSU_REJECT;
	abortItems->reason = DUL_ULSU_REJ_UNREC_CALLED_TITLE;
	cond = COND_PushCondition(APP_ILLEGALSERVICEPARAMETER,
				  APP_Message(APP_ILLEGALSERVICEPARAMETER),
				  "AE Title", params->calledAPTitle);
    }
    if (cond != APP_NORMAL) {
	if (forgive)
	    cond = COND_PushCondition(APP_PARAMETERWARNINGS,
				      APP_Message(APP_PARAMETERWARNINGS));
	else
	    cond = COND_PushCondition(APP_PARAMETERFAILURE,
				      APP_Message(APP_PARAMETERFAILURE));
    }
    return cond;
}
