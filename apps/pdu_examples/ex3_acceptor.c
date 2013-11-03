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
**			storeRequest
**			findRequest
**			findCallback
**			moveRequest
**			moveCallback
**			void fakeDBQuery
**			establishSendAssociation
** Author, Date:	Stephen M. Moore, 17-Apr-93
** Intent:		This program is an example and test program
**			which demonstrates the DICOM Upper Layer (DUL)
**			software and service classes for the DICOM 93 software.
**			It provides the storage and verification service
**			classes.
** Usage:		ex3_acceptor [-f] [-p] [-v] port
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:37:46 $
** Source File:		$RCSfile: ex3_acceptor.c,v $
** Revision:		$Revision: 1.8 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.8 $ $RCSfile: ex3_acceptor.c,v $";

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
#include "dbquery.h"
#include "iap.h"
#include "ex3_acceptor.h"

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

static int supportedClass(char *abstractSyntax, char **classArray);
static CONDITION
associationCheck(DUL_ASSOCIATESERVICEPARAMETERS * params, CTNBOOLEAN forgive,
		 DUL_ABORTITEMS * abortItems);
CONDITION
EXP_AcceptServiceClass(DUL_PRESENTATIONCONTEXT * requestedCtx,
		 DUL_SC_ROLE role, DUL_ASSOCIATESERVICEPARAMETERS * params);
static void
    appError(char *messageType);

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
        classCount = 0;
    CTNBOOLEAN
	verbose = FALSE,
	forgiveFlag = FALSE,
	paramsFlag = FALSE,
	acceptFlag,
	drop;
    DUL_SC_ROLE
	scRole;

    char *classArray[] = {
	DICOM_SOPCLASSVERIFICATION,
	DICOM_SOPCLASSCOMPUTEDRADIOGRAPHY,
	DICOM_SOPCLASSCT,
	DICOM_SOPCLASSMR,
	DICOM_SOPCLASSNM,
	DICOM_SOPCLASSUS,
	DICOM_SOPPATIENTQUERY_FIND,
	DICOM_SOPPATIENTQUERY_MOVE,
	DICOM_SOPSTUDYQUERY_FIND,
	DICOM_SOPSTUDYQUERY_MOVE,
	DICOM_SOPPATIENTSTUDYQUERY_FIND,
	DICOM_SOPPATIENTSTUDYQUERY_MOVE,
    ""};


    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'f':
	    forgiveFlag = TRUE;
	    break;
	case 'p':
	    paramsFlag = TRUE;
	    break;
	case 'v':
	    verbose = TRUE;
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
    DUL_Debug(verbose);
    SRV_Debug(verbose);

    if (port < 1024) {
	if (geteuid() != 0) {
	    char errmsg[] =
	    "To use this port (%d), you must run as root or the application must be\n\
setuid root (see chmod)\n";

	    fprintf(stderr, errmsg, port);
	    perror("");
	    THR_Shutdown();
	    exit(1);
	}
    }
    cond = DUL_InitializeNetwork(DUL_NETWORK_TCP, DUL_AEBOTH,
			   (void *) &port, 0, DUL_ORDERBIGENDIAN, &network);
    if (cond != DUL_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
/*  Go back to the proper uid so we don't mess with things we don't own.
*/

    (void) setuid(getuid());

    while (!CTN_ERROR(cond)) {
	drop = FALSE;
	(void) memset(&service, 0, sizeof(service));
	service.maxPDU = 16384;
	strcpy(service.calledImplementationClassUID,
	       MIR_IMPLEMENTATIONCLASSUID);
	strcpy(service.calledImplementationVersionName,
	       MIR_IMPLEMENTATIONVERSIONNAME);
	cond = DUL_ReceiveAssociationRQ(&network, DUL_BLOCK,
					&service, &association);
	if (cond != DUL_NORMAL) {
	    COND_DumpConditions();
	    THR_Shutdown();
	    exit(1);
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
	    if (verbose || paramsFlag)
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
		    cond = EXP_AcceptServiceClass(requestedCtx, scRole,
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
		    exit(1);
		}
		if (verbose)
		    DUL_DumpParams(&service);
		cond = serviceRequests(&network, &association, &service);
		if (cond == SRV_PEERREQUESTEDRELEASE)
		    cond = SRV_NORMAL;
	    }
	}
	(void) DUL_ClearServiceParameters(&service);
    }
    (void) DUL_DropNetwork(&network);
    THR_Shutdown();
    exit(0);
}


/* usageerror
**
** Purpose:
**	Print the usage text for this program and exit
**
** Parameter Dictionary:
**
** Return Values:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static void
usageerror()
{
    char msg[] = "\
Usage: ex3_acceptor [-f] [-p] [-v] port\n\
\n\
   -f    Forgiving mode;  allow Associations with some incorrect parameters\n\
   -p    Dump Association RQ parameters\n\
   -v    Place DCM, DUL and SRV facilities in verbose mode\n";

    (void) fprintf(stderr, msg);
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
	appError("C-Echo");
	cond = 0;
	break;
    case MSG_K_C_STORE_REQ:
	appError("C-Store");
	cond = 0;
	break;
    case MSG_K_C_FIND_REQ:
	appError("C-Find");
	cond = 0;
	break;
    case MSG_K_C_MOVE_REQ:
	appError("C-Move");
	cond = 0;
	break;

    default:
	fprintf(stderr, "Unimplemented message type: %d\n", messageType);
	cond = 1;
	break;
    }
    return cond;
}


static void
appError(char *messageType)
{
    char msg[] = "\
This application received a %s Request.  It is only supposed to\
process Associations Requests and Releases\n";

    fprintf(stderr, msg, messageType);
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
    if (strcmp(params->calledAPTitle, "RSNA_STORAGE_93") != 0) {
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

/* the modification part */
static char *SyntaxList[] = {
    DICOM_SOPCLASSVERIFICATION,
    DICOM_SOPCLASSCOMPUTEDRADIOGRAPHY,
    DICOM_SOPCLASSCT,
    DICOM_SOPCLASSMR,
    DICOM_SOPCLASSNM,
    DICOM_SOPCLASSUS,
    DICOM_SOPCLASSSECONDARYCAPTURE,
    DICOM_SOPPATIENTQUERY_FIND,
    DICOM_SOPPATIENTQUERY_MOVE,
    DICOM_SOPSTUDYQUERY_FIND,
    DICOM_SOPSTUDYQUERY_MOVE,
    DICOM_SOPPATIENTSTUDYQUERY_FIND,
    DICOM_SOPPATIENTSTUDYQUERY_MOVE,
    DICOM_SOPCLASSDETACHEDPATIENTMGMT,
    DICOM_SOPCLASSDETACHEDVISITMGMT,
    DICOM_SOPCLASSDETACHEDPATIENTMGMTMETA,
    DICOM_SOPCLASSDETACHEDSTUDYMGMT,
    DICOM_SOPCLASSDETACHEDRESULTSMGMT,
    DICOM_SOPCLASSDETACHEDINTERPRETMGMT,
    DICOM_SOPCLASSDETACHEDRESULTSMGMTMETA
};

/* EXP_AcceptServiceClass
**
** Purpose:
**      Describe the purpose of the function
**
** Parameter Dictionary:
**      Define the parameters to the function
**
** Return Values:
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/
CONDITION
EXP_AcceptServiceClass(DUL_PRESENTATIONCONTEXT * requestedCtx,
		  DUL_SC_ROLE role, DUL_ASSOCIATESERVICEPARAMETERS * params)
{
    int
        index;
    CTNBOOLEAN
	abstractFound = FALSE,
	transferFound = FALSE;

    CONDITION
	cond,
	rtnCond = SRV_NORMAL;
    DUL_PRESENTATIONCONTEXT
	* ctx;
    DUL_TRANSFERSYNTAX
	* transfer;

    if (params->acceptedPresentationContext == NULL) {
	params->acceptedPresentationContext = LST_Create();
	if (params->acceptedPresentationContext == NULL) {
	    return COND_PushCondition(SRV_LISTFAILURE,
		    SRV_Message(SRV_LISTFAILURE), "SRV_AcceptServiceClass");
	}
    }
    for (index = 0; index < (int) DIM_OF(SyntaxList) && !abstractFound;
	 index++) {
	if (strcmp(requestedCtx->abstractSyntax, SyntaxList[index]) == 0) {
	    abstractFound = TRUE;
	}
    }
    if (abstractFound) {
	if ((transfer = LST_Head(&requestedCtx->proposedTransferSyntax)) == NULL
	    )
	    return COND_PushCondition(SRV_LISTFAILURE,
		    SRV_Message(SRV_LISTFAILURE), "SRV_AcceptServiceClass");
	(void) LST_Position(&requestedCtx->proposedTransferSyntax, transfer);
	while (!transferFound && (transfer != NULL)) {
	    if (strcmp(transfer->transferSyntax,
		       DICOM_TRANSFERLITTLEENDIAN) == 0)
		transferFound = TRUE;
	    else if (strcmp(transfer->transferSyntax,
			    DICOM_TRANSFERLITTLEENDIANEXPLICIT) == 0)
		transferFound = TRUE;
	    else
		transfer = LST_Next(&requestedCtx->proposedTransferSyntax);
	}
	if (transferFound) {
	    cond = DUL_MakePresentationCtx(&ctx,
					 requestedCtx->proposedSCRole, role,
					requestedCtx->presentationContextID,
					   DUL_PRESENTATION_ACCEPT,
					   requestedCtx->abstractSyntax,
		  transfer->transferSyntax, transfer->transferSyntax, NULL);
	    if (cond != DUL_NORMAL)
		return COND_PushCondition(SRV_PRESENTATIONCONTEXTERROR,
				  SRV_Message(SRV_PRESENTATIONCONTEXTERROR),
					  "SRV_AcceptServiceClass");
	} else {
	    cond = DUL_MakePresentationCtx(&ctx,
					   requestedCtx->proposedSCRole,
					   DUL_SC_ROLE_DEFAULT,
					requestedCtx->presentationContextID,
				    DUL_PRESENTATION_REJECT_TRANSFER_SYNTAX,
					   requestedCtx->abstractSyntax,
	      DICOM_TRANSFERLITTLEENDIAN, DICOM_TRANSFERLITTLEENDIAN, NULL);
	    if (cond != DUL_NORMAL)
		return COND_PushCondition(SRV_PRESENTATIONCONTEXTERROR,
				  SRV_Message(SRV_PRESENTATIONCONTEXTERROR),
					  "SRV_AcceptServiceClass");

	    (void) COND_PushCondition(SRV_UNSUPPORTEDTRANSFERSYNTAX,
				 SRV_Message(SRV_UNSUPPORTEDTRANSFERSYNTAX),
		    requestedCtx->abstractSyntax, "SRV_AcceptServiceClass");
	    rtnCond = COND_PushCondition(SRV_PRESENTATIONCTXREJECTED,
				   SRV_Message(SRV_PRESENTATIONCTXREJECTED),
		    requestedCtx->abstractSyntax, "SRV_AcceptServiceClass");
	}
    } else {
	cond = DUL_MakePresentationCtx(&ctx,
				       requestedCtx->proposedSCRole,
				       DUL_SC_ROLE_DEFAULT,
				       requestedCtx->presentationContextID,
				    DUL_PRESENTATION_REJECT_ABSTRACT_SYNTAX,
				       requestedCtx->abstractSyntax,
	      DICOM_TRANSFERLITTLEENDIAN, DICOM_TRANSFERLITTLEENDIAN, NULL);
	if (cond != DUL_NORMAL)
	    return COND_PushCondition(SRV_PRESENTATIONCONTEXTERROR,
				  SRV_Message(SRV_PRESENTATIONCONTEXTERROR),
				      "SRV_AcceptServiceClass");
	(void) COND_PushCondition(SRV_UNSUPPORTEDSERVICE,
				  SRV_Message(SRV_UNSUPPORTEDSERVICE),
		    requestedCtx->abstractSyntax, "SRV_AcceptServiceClass");
	rtnCond = COND_PushCondition(SRV_PRESENTATIONCTXREJECTED,
				   SRV_Message(SRV_PRESENTATIONCTXREJECTED),
		    requestedCtx->abstractSyntax, "SRV_AcceptServiceClass");
    }
    cond = LST_Enqueue(&params->acceptedPresentationContext, ctx);
    if (cond != LST_NORMAL)
	return COND_PushCondition(SRV_LISTFAILURE, SRV_Message(SRV_LISTFAILURE),
				  "SRV_AcceptServiceClass");

/* Warning: these external variables cannot be reached outside the SRV facility
 * module. Therefore the modified program can only get the correct result on
 * the first received association and service request.
 */
/*    pdvList.count = 0;
 *    nextPDV = NULL;
 */
    return rtnCond;
}
