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
**                              DINPACS 1997
**                   Electronic Radiology Laboratory
**                 Mallinckrodt Institute of Radiology
**              Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	Stephen Moore, 27-Oct-94
** Intent:		Handles DICOM N-Action requests as part of the image
**			archive.  These functions respond to requests
**			made by an SCU of one or more SOP Classes
** Last Update:         $Author: smm $, $Date: 2001-12-21 16:37:40 $
** Source File:         $RCSfile: naction.c,v $
** Revision:            $Revision: 1.7 $
** Status:              $State: Exp $
*/
static char rcsid[] = "$Revision: 1.7 $ $RCSfile: naction.c,v $";

#include "ctn_os.h"

#if 0
#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <sys/types.h>
#endif

#include "dicom.h"
#include "condition.h"
#include "utility.h"
#include "lst.h"
#include "dicom_uids.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
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
#include "dicom_sq.h"

#include "image_archive.h"
#include "naction.h"


typedef struct {
    char *abstractSyntax;
    FIS_HANDLE **fis;
    IDB_HANDLE **idb;
    DUL_ASSOCIATESERVICEPARAMETERS *params;
}   CALLBACK_CTX;

typedef struct {
    DCM_TAG tag;
    unsigned long flag;
}   TAG_MAP;

static char transactionUID[DICOM_UI_LENGTH + 1];

/* The first part of the file will define the private functions. */

/* insertWorkQueueRecord
**
** Purpose:
**	Inserts one Storage Commitment work queue record into the FIS.
**	This function uses a hard-coded task name and the transaction UID
**	passed by the caller as the task ID.
**
** Parameter Dictionary:
**	fis		Handle for the FIS database
**	transactionUID	Of the storage commitment request.  We will use this
**			as the task ID
**
** Return Values:
**
**	APP_NORMAL
**	SRV_FAILURE
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
insertWorkQueueRecord(FIS_HANDLE ** fis,
		      const char *transactionUID)
{
    FIS_WORKQUEUERECORD workQueue;
    CONDITION cond;

    memset(&workQueue, 0, sizeof(workQueue));
    workQueue.Type = FIS_K_WORKQUEUE;
    workQueue.Flag = FIS_K_WORKQUEUE_TASNAM | FIS_K_WORKQUEUE_TASID;
    strcpy(workQueue.TasNam, FIS_WQ_STORAGECOMMIT);
    strcpy(workQueue.TasID, transactionUID);
    cond = FIS_Insert(fis, FIS_K_WORKQUEUE, &workQueue);
    if (cond != FIS_NORMAL)
	return COND_PushCondition(APP_FAILURE, APP_Message(APP_FAILURE),
				  "Unable to insert work queue record",
				  "insertWorkQueueRecord");
    else
	return APP_NORMAL;

}

/* storageCommitmentRequest
**
** Purpose:
**	This function performs the first part of the archive's response to
**	a storage commitment request.  We make a note of the request in the
**	FIS and create a work queue record for someone else to process.
**	When we return from this function, the SRV facility will pass a
**	positive response to the N-ACTION requester.  This means that we
**	have seen/recorded the request.  It does not imply that we have
**	committed the images to storage.
**
** Parameter Dictionary:
**	request		An SRV structure which contains the request
**			parameters.
**	response	An SRV structure with the response parameters.
**	ctx		Context information.  A structure with pointers
**			to various handlers to allow us to do DB operations.
**
** Return Values:
**
**	1		Which means the mechanics of the process work.  We
**			can always place a failure status code in the
**			response message.
**
** Notes:
**
** Algorithm:
**	Parse the N-ACTION data set.
**	Insert one FIS record for each SOP Instance UID to be committed.
**	Insert one FIS record with the Storage Commitment request
**	Call another function to insert a work queue record to ask
**		someone else to determine if the images where stored.
*/

#define STORAGE_COMMIT_CREATE_FLAGS (FIS_K_SCOMMIT_TRAUID | \
        FIS_K_SCOMMIT_REQAE | \
        FIS_K_SCOMMIT_RESAE | \
        FIS_K_SCOMMIT_REQDAT | \
        FIS_K_SCOMMIT_REQTIM | \
        FIS_K_SCOMMIT_RESDAT | \
        FIS_K_SCOMMIT_RESTIM )

static CONDITION
storageCommitmentRequest(MSG_N_ACTION_REQ * request,
			 MSG_N_ACTION_RESP * response,
			 CALLBACK_CTX * ctx)
{
    CONDITION cond;
    static FIS_STORAGECOMMITREQRECORD storageCommit;

    response->dataSetType = DCM_CMDDATANULL;
    strcpy(response->classUID, request->classUID);
    strcpy(response->instanceUID, request->instanceUID);
    response->conditionalFields = MSG_K_N_ACTIONRESP_AFFECTEDINSTANCEUID;
    response->status = MSG_K_SUCCESS;

    if (request->dataSetType != DCM_CMDDATANULL) {
	(void) DCM_DumpElements(&request->actionInformation, 0);
    } else {
	response->status = MSG_K_PROCESSINGFAILURE;
	fprintf(stderr, "Storage commit request did not have required dataset\n");
	goto ExitPoint;
    }

    memset(&storageCommit, 0, sizeof(storageCommit));
    storageCommit.Type = FIS_K_STORAGECOMMITREQ;

    {
	DCM_ELEMENT e = {DCM_IDTRANSACTIONUID, DCM_UI, "", 1,
	    sizeof(storageCommit.TraUID),
	(void *) storageCommit.TraUID};

	cond = DCM_ParseObject(&request->actionInformation, &e, 1, NULL, 0, NULL);
	if (cond != DCM_NORMAL) {
	    response->status = MSG_K_PROCESSINGFAILURE;
	    fprintf(stderr, "Was not able to parse action information in request\n");
	    goto ExitPoint;
	}
    }

    strcpy(transactionUID, storageCommit.TraUID);

    strcpy(storageCommit.ReqAE, ctx->params->callingAPTitle);
    strcpy(storageCommit.ResAE, ctx->params->calledAPTitle);
    UTL_GetDicomDate(storageCommit.ReqDat);
    UTL_GetDicomTime(storageCommit.ReqTim);

    fprintf(stderr, "Req AE %s\n Res AE %s", storageCommit.ReqAE, storageCommit.ResAE);
    /*
     * A lot of code to insert one FIS record for each SOP instance to * be
     * committed.
     */
    {
	LST_HEAD *l;
	DCM_ELEMENT e;
	SQ_TYPE type;
	SQ_REFERENCEDSOPSEQUENCE *ref;
	e.tag = DCM_IDREFERENCEDSOPSEQUENCE;

	l = LST_Create();
	cond = DCM_GetSequenceList(&request->actionInformation,
				   e.tag, &e.d.sq);
	if (cond != DCM_NORMAL) {
	    response->status = MSG_K_PROCESSINGFAILURE;
	    fprintf(stderr, "Could not get sequence list\n");
	    goto ExitPoint;
	}
	cond = SQ_ParseSequence(&e, &type, &l);
	if (cond != SQ_NORMAL) {
	    response->status = MSG_K_PROCESSINGFAILURE;
	    fprintf(stderr, "Could not parse sequence\n");
	    goto ExitPoint;
	}
	ref = LST_Head(&l);
	LST_Position(&l, ref);
	fprintf(stderr, "Address of first sequence element: %x\n", ref);
	while (ref != NULL) {
	    FIS_COMMITSOPREFRECORD sopReference;

	    memset(&sopReference, 0, sizeof(sopReference));
	    sopReference.Type = FIS_K_COMMITSOPREF;
	    strcpy(sopReference.TraUID, storageCommit.TraUID);
	    strcpy(sopReference.SOPClaUID, ref->referencedSOPClassUID);
	    strcpy(sopReference.SOPInsUID, ref->referencedSOPInstanceUID);
	    fprintf(stderr, "Tra UID: %s, SOP Class: %s, SOP Ins: %s\n",
		    sopReference.TraUID, sopReference.SOPClaUID, sopReference.SOPInsUID);

	    sopReference.Flag = FIS_K_COMMITSOP_TRAUID |
		FIS_K_COMMITSOP_SOPCLAUID |
		FIS_K_COMMITSOP_SOPINSUID;
	    fprintf(stderr, "About to insert COMMITSOPREF into FIS\n");
	    cond = FIS_Insert(ctx->fis, FIS_K_COMMITSOPREF, &sopReference);
	    if (cond != FIS_NORMAL) {
		fprintf(stderr, "Could not insert record into FIS\n");
		response->status = MSG_K_PROCESSINGFAILURE;
		SQ_ReleaseList(&l);
		/* LST_Destroy(&l); */
		COND_DumpConditions();
		goto ExitPoint;
	    }
	    ref = LST_Next(&l);
	}
	SQ_ReleaseList(&l);
	/* LST_Destroy(&l); */
    }

    UTL_GetDicomDate(storageCommit.ResDat);
    UTL_GetDicomTime(storageCommit.ResTim);

    storageCommit.Flag = STORAGE_COMMIT_CREATE_FLAGS;

    /*
     * Insert the FIS record with the Storage Commitment Request
     */
    fprintf(stderr, "About to insert storage commit req into FIS\n");
    cond = FIS_Insert(ctx->fis, FIS_K_STORAGECOMMITREQ, &storageCommit);
    if (cond != FIS_NORMAL) {
	fprintf(stderr, "Could not insert storage commit req\n");
	response->status = MSG_K_PROCESSINGFAILURE;
	COND_DumpConditions();
	goto ExitPoint;
    }
    /*
     * Insert a work queue record so that someone will process the request
     */
    fprintf(stderr, "About to insert work queue record into FIS\n");
    cond = insertWorkQueueRecord(ctx->fis, storageCommit.TraUID);
    if (cond != APP_NORMAL) {
	fprintf(stderr, "Could not insert work queue record\n");
	response->status = MSG_K_PROCESSINGFAILURE;
	COND_DumpConditions();
	goto ExitPoint;
    }
ExitPoint:
    COND_DumpConditions();
    return 1;
}


/* nactionCallback
**
** Purpose:
**
** Parameter Dictionary:
**	request		e
**			d
**	response	ss
**			the
**	ctxPtr		Pointer to context structure which contains abstract
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
nactionCallback(MSG_N_ACTION_REQ * request, MSG_N_ACTION_RESP * response,
/*		CALLBACK_CTX * ctx, DUL_PRESENTATIONCONTEXT *pc) */
		void *ctxPtr, DUL_PRESENTATIONCONTEXT * pc)
{
    CONDITION cond = 0;

/*  Following definition and assignment satisfy prototypes for dicom_services.h*/
    CALLBACK_CTX *ctx;
    ctx = (CALLBACK_CTX *) ctxPtr;

    strcpy(response->classUID, request->classUID);
    response->messageIDRespondedTo = request->messageID;


    if (strcmp(ctx->abstractSyntax, DICOM_SOPCLASSSTORAGECOMMITMENTPUSHMODEL) == 0) {
	fprintf(stderr, "Archive server: N-Action Request, Storage Commit Push\n");
	cond = storageCommitmentRequest(request, response, ctx);
    } else
	return SRV_UNSUPPORTEDSERVICE;

    if (cond == 1)
	return SRV_NORMAL;
    else
	return 0;
}

static CONDITION
selectCallback(IDB_Query * q, long count, LST_HEAD * lst)
{
    return IDB_NORMAL;
}

static void
addToSuccesses(FIS_COMMITSOPREFRECORD * sopRefRecord,
	       LST_HEAD ** successes)
{
    SQ_REFERENCEDSOPSEQUENCE *sq;

    sq = calloc(1, sizeof(*sq));
    sq->type = SQ_K_REFERENCEDSOPSEQUENCE;
    strcpy(sq->referencedSOPClassUID, sopRefRecord->SOPClaUID);
    strcpy(sq->referencedSOPInstanceUID, sopRefRecord->SOPInsUID);
    LST_Enqueue(successes, sq);


}
static void
addToFailures(FIS_COMMITSOPREFRECORD * sopRefRecord,
	      LST_HEAD ** failures)
{
    SQ_FAILEDSOPSEQUENCE *failedSOPSeq;

    failedSOPSeq = calloc(1, sizeof(*failedSOPSeq));
    failedSOPSeq->type = SQ_K_FAILEDSOPSEQUENCE;
    strcpy(failedSOPSeq->referencedSOPClassUID, sopRefRecord->SOPClaUID);
    strcpy(failedSOPSeq->referencedSOPInstanceUID, sopRefRecord->SOPInsUID);
    LST_Enqueue(failures, failedSOPSeq);
}

/* Entry points for public functions start here.
 */

/* nactionRequest
**
*/

CONDITION
nactionRequest(DUL_ASSOCIATESERVICEPARAMETERS * params,
	       DUL_ASSOCIATIONKEY ** association, DMAN_HANDLE ** dman,
	       DUL_PRESENTATIONCONTEXT * ctx, MSG_N_ACTION_REQ ** request,
	       FIS_HANDLE ** fis, IDB_HANDLE ** idb)
{
    MSG_N_ACTION_RESP response;
    CONDITION cond;
    CALLBACK_CTX cb;

    memset(&response, 0, sizeof(response));
    response.type = MSG_K_N_ACTION_RESP;
    response.conditionalFields = 0;
    response.dataSetType = DCM_CMDDATANULL;

    cb.abstractSyntax = ctx->abstractSyntax;
    cb.fis = fis;
    cb.idb = idb;
    cb.params = params;

    cond = SRV_NActionResponse(association, ctx, request, &response,
			       nactionCallback, &cb, "");
    if (cond != SRV_NORMAL)
	return 0;

    return APP_NORMAL;
}


/* nactionCleanup
**
*/

CONDITION
nactionCleanup(DUL_NETWORKKEY ** network, DUL_ASSOCIATESERVICEPARAMETERS * params,
	       DUL_ASSOCIATIONKEY ** association, DMAN_HANDLE ** dman,
	       DUL_PRESENTATIONCONTEXT * ctx,
	       FIS_HANDLE ** fis, IDB_HANDLE ** idb)
{
    LST_HEAD *l;
    LST_HEAD *successes;
    LST_HEAD *failures;
    CONDITION cond;
    FIS_COMMITSOPREFRECORD *sopRefRecord;

    l = LST_Create();
    successes = LST_Create();
    failures = LST_Create();
    if (l == NULL || successes == NULL || failures == NULL) {
	fprintf(stderr, "Could not allocate a new list head in nactionCleanup\n");
	COND_DumpConditions();
	exit(1);
    }
    cond = FIS_Get(fis, FIS_K_COMMITSOPREF, FIS_K_STORAGECOMMITREQ,
		   transactionUID, 1, l);
    if (cond != FIS_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
    while ((sopRefRecord = LST_Dequeue(&l)) != NULL) {
	long count = 0;
	IDB_Query query;


	printf("%s\n%s\n%s\n\n",
	       sopRefRecord->TraUID,
	       sopRefRecord->SOPClaUID,
	       sopRefRecord->SOPInsUID);

	memset(&query, 0, sizeof(query));
	strcpy(query.image.SOPInsUID, sopRefRecord->SOPInsUID);
	query.ImageQFlag = QF_IMA_SOPInsUID;

	cond = IDB_Select_ImageLevel(idb, &query, &count, selectCallback, NULL);
	if (cond == IDB_NORMAL && count == 1) {
	    addToSuccesses(sopRefRecord, &successes);
	} else {
	    addToFailures(sopRefRecord, &failures);
	}
    }

#if 0
    sendCommitResult(network, params, dman, &successes, &failures);
#endif

    return APP_NORMAL;
}
