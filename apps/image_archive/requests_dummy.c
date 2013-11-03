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
**				DICOM 94
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	Stephen Moore, 11-Feb-94
** Intent:
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:37:42 $
** Source File:		$RCSfile: requests_dummy.c,v $
** Revision:		$Revision: 1.3 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.3 $ $RCSfile: requests_dummy.c,v $";

#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#ifdef MACH
#include <unistd.h>
#endif
#endif

#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "dicom_uids.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_ie.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#ifdef CTN_MULTIBYTE
#include "tblmb.h"
#include "idbmb.h"
#else
#include "tbl.h"
#include "idb.h"
#endif
#include "manage.h"
#include "fis.h"
#include "copy.h"

#include "image_archive.h"
#include "find.h"
#include "move.h"
#include "cget.h"
#include "naction.h"

extern char *controlDatabase;
extern CTNBOOLEAN doVerification;

typedef struct {
    char *fileName;
    char *transferSyntax;
    char *owner;
    char *groupName;
    char *priv;
    DMAN_HANDLE **handle;
}   STORAGE_PARAMS;

static IDB_HANDLE *IDBHandle;
static FIS_HANDLE *fisHandle;

static CONDITION
serviceThisCommand(DUL_NETWORKKEY ** network, DUL_ASSOCIATIONKEY ** association,
		   DUL_PRESENTATIONCONTEXT * ctx, MSG_TYPE messageType,
		   void **message, DUL_ASSOCIATESERVICEPARAMETERS * params,
		   DMAN_HANDLE ** handle);
static CONDITION
echoRequest(DUL_ASSOCIATIONKEY ** association,
	    DUL_PRESENTATIONCONTEXT * ctx, MSG_C_ECHO_REQ ** message);
static CONDITION
storeRequest(DUL_ASSOCIATESERVICEPARAMETERS * params,
	     DUL_ASSOCIATIONKEY ** association, DMAN_HANDLE ** handle,
	     DUL_PRESENTATIONCONTEXT * ctx, MSG_C_STORE_REQ ** message);
static CONDITION
echoCallback(MSG_C_ECHO_REQ * echoRequest,
	     MSG_C_ECHO_RESP * echoResponse, void *ctx,
	     DUL_PRESENTATIONCONTEXT * pc);
static CONDITION
storageCallback(MSG_C_STORE_REQ * request, MSG_C_STORE_RESP * response,
		unsigned long received, unsigned long estimate,
/*		DCM_OBJECT ** object, STORAGE_PARAMS * params, */
		DCM_OBJECT ** object, void *paramsPtr,
		DUL_PRESENTATIONCONTEXT * pc);
static CONDITION
storeImage(DMAN_HANDLE ** handle, char *origFileName,
	   char *transferSyntax, char *SOPClass,
	   char *owner, char *groupName, char *priv,
	   DCM_OBJECT ** object);

extern CTNBOOLEAN silent;

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
**	reducedCapability If true, we are operating with reduced capability.
**			This means we will not use FIS for anything.
**
** Return Values:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
serviceRequests(DUL_NETWORKKEY ** network, DUL_ASSOCIATIONKEY ** association,
		DUL_ASSOCIATESERVICEPARAMETERS * service,
		CTNBOOLEAN reducedCapability)
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
	networkLink = TRUE,
	commandServiced;
    DMAN_HANDLE
	* handle;
    DMAN_STORAGEACCESS
	storage;
    DMAN_FISACCESS fisAccess;

#if 0
    cond = DMAN_Open(controlDatabase, service->callingAPTitle,
		     service->calledAPTitle, &handle);

    if (cond == DMAN_NORMAL) {
	cond = DMAN_LookupStorage(&handle, service->calledAPTitle,
				  &storage);
    }
    if (cond == DMAN_NORMAL)
	cond = IDB_Open(storage.DbKey, &IDBHandle);

    if (!reducedCapability && !CTN_ERROR(cond)) {
	cond = DMAN_LookupFISAccess(&handle, service->calledAPTitle, &fisAccess);
	if (!CTN_ERROR(cond))
	    cond = FIS_Open(fisAccess.DbKey, &fisHandle);
    }
#endif

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
	    cond = 2;
	} else {
	    ctx = LST_Head(&service->acceptedPresentationContext);
	    if (ctx != NULL)
		(void) LST_Position(&service->acceptedPresentationContext, ctx);
	    commandServiced = FALSE;
	    while (ctx != NULL && !commandServiced) {
		if (ctx->presentationContextID == ctxID) {
		    if (commandServiced) {
			if (!silent)
			    fprintf(stderr,
			      "Context ID Repeat in serviceRequests (%d)\n",
				    ctxID);
		    } else {
			cond = serviceThisCommand(network, association, ctx,
					     messageType, &message, service,
						  &handle);
			if (cond == SRV_OPERATIONCANCELLED) {
			    if (!silent)
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
    if (!silent)
	COND_DumpConditions();
#if 0
    (void) DMAN_Close(&handle);
    (void) IDB_Close(&IDBHandle);
    if (!reducedCapability)
	(void)FIS_Close(&fisHandle);
#endif
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
		   DMAN_HANDLE ** handle)
{
    CONDITION
    cond;
    MSG_GENERAL
	* general;

    general = *(MSG_GENERAL **) message;

    if (!silent)
	MSG_DumpMessage((void *) general, stdout);
    switch (messageType) {
    case MSG_K_C_ECHO_REQ:
	cond = echoRequest(association, ctx, (MSG_C_ECHO_REQ **) message);
	break;
    case MSG_K_C_STORE_REQ:
	cond = storeRequest(params, association, handle, ctx,
			    (MSG_C_STORE_REQ **) message);
	break;
    case MSG_K_C_FIND_REQ:
	cond = findRequest(association, params, ctx,
			   (MSG_C_FIND_REQ **) message, &IDBHandle);
	break;
    case MSG_K_C_MOVE_REQ:
	cond = moveRequest(network, association, ctx,
			   (MSG_C_MOVE_REQ **) message, params, &IDBHandle,
			   handle);
	break;
    case MSG_K_C_GET_REQ:
	cond = cgetRequest(network, association, ctx,
			   (MSG_C_GET_REQ **) message, params, &IDBHandle,
			   handle);
	break;
    case MSG_K_C_CANCEL_REQ:
	cond = SRV_NORMAL;	/* This must have happened after we were done */
	break;
    case MSG_K_N_ACTION_REQ:
	cond = nactionRequest(params, association, handle, ctx,
			      (MSG_N_ACTION_REQ **) message, &fisHandle,
			      &IDBHandle);
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
**	ctx		Context information that we ignore
**	pc		Presentation context pointer that we ignore
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


/* storeRequest
**
** Purpose:
**	This function responds to a request to store an image.
**
** Parameter Dictionary:
**	association	They key which is used to access the association
**			on which requests are received.
**	ctx		Pointer to the presentation context for this command
**	message		Pointer to the MSG_C_STORE_REQ message that was
**			received by the server.
**
** Return Values:
**
**	SRV_FILECREATEFAILED
**	SRV_NORMAL
**	SRV_OBJECTBUILDFAILED
**	SRV_RESPONSEFAILED
**	SRV_UNEXPECTEDPDVTYPE
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/


static CONDITION
storeRequest(DUL_ASSOCIATESERVICEPARAMETERS * params,
	     DUL_ASSOCIATIONKEY ** association, DMAN_HANDLE ** handle,
	     DUL_PRESENTATIONCONTEXT * ctx, MSG_C_STORE_REQ ** request)
{
    int
        index;
    char
        fileName[1024];
    MSG_C_STORE_RESP
	response;
    CONDITION
	cond;
    int
        fd;
    STORAGE_PARAMS
	storageParams;

#if 0
    cond = DMAN_TempImageFile(handle, (*request)->classUID,
			      fileName, sizeof(fileName));
    if (cond != DMAN_NORMAL)
	fileName[0] = '\0';
#endif
    strcpy(fileName, (*request)->instanceUID);

    storageParams.fileName = fileName;
    storageParams.transferSyntax = ctx->acceptedTransferSyntax;
    storageParams.handle = handle;
    storageParams.owner = params->callingAPTitle;
    storageParams.groupName = "";
    storageParams.priv = "";

    cond = SRV_CStoreResponse(association, ctx, request, &response,
			      fileName, storageCallback, &storageParams, "");
    (void)unlink(fileName);
    return cond;
}

/* storageCallback
**
** Purpose:
**	Call back routine provided by the service provider and invoked by
**	the C-STORE response routine
**
** Parameter Dictionary:
**	request		Pointer to C-STORE request message
**	response	Pointer to C-STORE response message
**	received	Number of bytes received so far
**	estimate	Estimated number of bytes in the data set
**	object		Handle to DICOM object
**	params		Context information which is a set of pointers we
**			use for storing the image and data in the database.
**	pc		Presentation context which describes this SOP class.
**
** Return Values:
**	SRV_NORMAL
**
** Notes:
**	We pass the hand
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
storageCallback(MSG_C_STORE_REQ * request, MSG_C_STORE_RESP * response,
		unsigned long received, unsigned long estimate,
/*		DCM_OBJECT ** object, STORAGE_PARAMS * params, */
		DCM_OBJECT ** object, void *paramsPtr,
		DUL_PRESENTATIONCONTEXT * pc)
{
    CONDITION
    cond = 0;
    IE_OBJECT
	* ieObject;
/*  The definition and assignment help satisfy prototypes for this callback
**  function (defined by dicom_services.h)
*/
    STORAGE_PARAMS *params;
    params = (STORAGE_PARAMS *) paramsPtr;

    if (!silent)
	printf("%8d bytes received of %8d estimated \n", received, estimate);
    if (!silent && (object != NULL))
	(void) DCM_DumpElements(object, 0);

    if (object != NULL) {
	if (doVerification) {
	    cond = IE_ExamineObject(object, &ieObject);
	    (void) IE_Free((void **) &ieObject);
	    if (cond != IE_NORMAL) {	/* The image does not satisfy */
		/* Part 3 requirements */
		response->status = MSG_K_C_STORE_DATASETNOTMATCHSOPCLASSERROR;
		return SRV_NORMAL;
	    }
	}
#if 0
	cond = storeImage(params->handle, params->fileName,
			  params->transferSyntax, request->classUID,
			  params->owner, params->groupName, params->priv,
			  object);
#endif
	cond = APP_NORMAL;
    }
    if (response != NULL) {
	if (cond == APP_NORMAL)
	    response->status = MSG_K_SUCCESS;
	else
	    response->status = MSG_K_C_STORE_OUTOFRESOURCES;
    }
    return SRV_NORMAL;
}

static void
logImageError(const char *msg, const char *fileName)
{
    FILE *log;
    time_t t;
    char *timeString;

    t = time(0);
    timeString = ctime(&t);

    log = fopen("archive_logfile", "a");
    if (log == NULL)
	fprintf(stderr, "%s %s %s\n", timeString, msg, fileName);
    else {
	fprintf(log, "%s %s %s\n", timeString, msg, fileName);
	fclose(log);
    }
}

static CONDITION
storeImage(DMAN_HANDLE ** handle, char *origFileName,
	   char *transferSyntax, char *SOPClass,
	   char *owner, char *groupName, char *priv, DCM_OBJECT ** object)
{
    IDB_Insertion Insertion;
    IDB_InstanceListElement imageInstance;

    CONDITION cond;
    char fileName[256];
    char *accession;
    int i;

    memset(&Insertion, 0, sizeof(Insertion));

    cond = parseImageForInsert(object, &Insertion);
    if (cond != APP_NORMAL)
	return 0;

    strcpy(Insertion.patient.Owner, owner);
    strcpy(Insertion.patient.GroupName, groupName);
    strcpy(Insertion.patient.Priv, priv);

    if (strlen(Insertion.study.AccNum) != 0)
	accession = Insertion.study.AccNum;
    else
	accession = Insertion.study.StuInsUID;

    cond = DMAN_PermImageFile(handle, SOPClass, accession,
			      Insertion.series.SerInsUID,
			      fileName, sizeof(fileName));
    if (cond != DMAN_NORMAL)
	return 0;

    i = rename(origFileName, fileName);
    if (i != 0)
	return 0;

    strcpy(Insertion.image.Path, fileName);
    strcpy(Insertion.image.Transfer, transferSyntax);

    if (strlen(Insertion.patient.PatNam) == 0) {
	logImageError("Zero length patient name", fileName);
	(void) COND_PopCondition(TRUE);
	return APP_NORMAL;
    }
    if (strlen(Insertion.patient.PatID) == 0) {
	logImageError("Zero length patient ID", fileName);
	(void) COND_PopCondition(TRUE);
	return APP_NORMAL;
    }
    cond = IDB_InsertImage(&IDBHandle, &Insertion);
    if (cond != IDB_NORMAL) {
	COND_DumpConditions();
	return 0;
    }
    (void) COND_PopCondition(TRUE);

    return APP_NORMAL;
}
