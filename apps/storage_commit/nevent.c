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
**                              DINPACS 97
**                   Electronic Radiology Laboratory
**                 Mallinckrodt Institute of Radiology
**              Washington University School of Medicine
**
** Module Name(s):
** Author, Date:        Stephen Moore, 23-Mar-1993
** Intent:              Handles DICOM N-Action requests as part of the image
**                      archive.
**
** Last Update:         $Author: smm $, $Date: 2001-12-21 16:43:43 $
** Source File:         $RCSfile: nevent.c,v $
** Revision:            $Revision: 1.2 $
** Status:              $State: Exp $
*/
static char rcsid[] = "$Revision: 1.2 $ $RCSfile: nevent.c,v $";

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
#include "idb.h"
#include "fis.h"
#include "dicom_sq.h"

#include "storage_commit.h"
#include "nevent.h"


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

#if 0
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
	return 0;
    else
	return APP_NORMAL;

}
#endif




static CONDITION
storageCommitmentRequest(MSG_N_EVENT_REPORT_REQ * request,
			 MSG_N_EVENT_REPORT_RESP * response,
			 CALLBACK_CTX * ctx)
{
    CONDITION cond;
    static FIS_STORAGECOMMITREQRECORD storageCommit;
    long flag;

    response->dataSetType = DCM_CMDDATANULL;
    strcpy(response->classUID, request->classUID);
    strcpy(response->affectedInstanceUID, request->affectedInstanceUID);
    response->conditionalFields = MSG_K_N_EVENTREPORTRESP_REQUESTEDINSTANCEUID;
    response->status = MSG_K_SUCCESS;

    if (request->dataSetType != DCM_CMDDATANULL) {
	(void) DCM_DumpElements(&request->dataSet, 0);
    } else {
	response->status = MSG_K_PROCESSINGFAILURE;
	goto ExitPoint;
    }

    memset(&storageCommit, 0, sizeof(storageCommit));
    storageCommit.Type = FIS_K_STORAGECOMMITREQ;

    {
	DCM_ELEMENT e = {DCM_IDTRANSACTIONUID, DCM_UI, "", 1,
	    sizeof(storageCommit.TraUID),
	(void *) storageCommit.TraUID};

	cond = DCM_ParseObject(&request->dataSet, &e, 1, NULL, 0, NULL);
	if (cond != DCM_NORMAL) {
	    response->status = MSG_K_PROCESSINGFAILURE;
	    goto ExitPoint;
	}
    }

    /* First look for the successes.  There may not be any */
    {
	LST_HEAD *l;
	DCM_ELEMENT e;
	SQ_TYPE type;
	SQ_REFERENCEDSOPSEQUENCE *ref;
	e.tag = DCM_IDREFERENCEDSOPSEQUENCE;

	l = LST_Create();
	cond = DCM_GetSequenceList(&request->dataSet,
				   e.tag, &e.d.sq);
	if (cond == DCM_NORMAL) {
	    cond = SQ_ParseSequence(&e, &type, &l);
	    if (cond != SQ_NORMAL) {
		response->status = MSG_K_PROCESSINGFAILURE;
		goto ExitPoint;
	    }
	    printf("Transaction UID: %s\n   SOP Instances commited: %d\n",
		   storageCommit.TraUID,
		   LST_Count(&l));

	    ref = LST_Head(&l);
	    LST_Position(&l, ref);
	    while (ref != NULL) {
		FIS_COMMITSOPREFRECORD sopReference;

		memset(&sopReference, 0, sizeof(sopReference));
		sopReference.Type = FIS_K_COMMITSOPREF;
		strcpy(sopReference.TraUID, storageCommit.TraUID);
		strcpy(sopReference.SOPClaUID, ref->referencedSOPClassUID);
		strcpy(sopReference.SOPInsUID, ref->referencedSOPInstanceUID);

		ref = LST_Next(&l);
	    }
	} else if (cond == DCM_ELEMENTNOTFOUND) {
	    (void) COND_PopCondition(FALSE);
	} else {
	    response->status = MSG_K_PROCESSINGFAILURE;
	    goto ExitPoint;
	}
    }

    /* Now look for the failures.  There may not be any */
    {
	LST_HEAD *l;
	DCM_ELEMENT e;
	SQ_TYPE type;
	SQ_FAILEDSOPSEQUENCE *ref;
	e.tag = DCM_IDFAILEDSOPSEQUENCE;

	l = LST_Create();
	cond = DCM_GetSequenceList(&request->dataSet,
				   e.tag, &e.d.sq);
	if (cond == DCM_NORMAL) {
	    cond = SQ_ParseSequence(&e, &type, &l);
	    if (cond != SQ_NORMAL) {
		response->status = MSG_K_PROCESSINGFAILURE;
		goto ExitPoint;
	    }
	    printf("Transaction UID: %s\n   SOP Instances failed: %d\n",
		   storageCommit.TraUID,
		   LST_Count(&l));

	    ref = LST_Head(&l);
	    LST_Position(&l, ref);
	    while (ref != NULL) {
		FIS_COMMITSOPREFRECORD sopReference;

		memset(&sopReference, 0, sizeof(sopReference));
		sopReference.Type = FIS_K_COMMITSOPREF;
		strcpy(sopReference.TraUID, storageCommit.TraUID);
		strcpy(sopReference.SOPClaUID, ref->referencedSOPClassUID);
		strcpy(sopReference.SOPInsUID, ref->referencedSOPInstanceUID);

		ref = LST_Next(&l);
	    }
	} else if (cond == DCM_ELEMENTNOTFOUND) {
	    (void) COND_PopCondition(FALSE);
	} else {
	    response->status = MSG_K_PROCESSINGFAILURE;
	    goto ExitPoint;
	}
    }

    UTL_GetDicomDate(storageCommit.ResDat);
    UTL_GetDicomTime(storageCommit.ResTim);

    storageCommit.Flag = FIS_K_SCOMMIT_RESDAT | FIS_K_SCOMMIT_RESTIM;

    cond = FIS_Update(ctx->fis, FIS_K_STORAGECOMMITREQ, &storageCommit);
    if (cond != FIS_NORMAL) {
	response->status = MSG_K_PROCESSINGFAILURE;
	COND_DumpConditions();
	goto ExitPoint;
    }
ExitPoint:
    COND_DumpConditions();
    return 1;
}


/* neventReportRequestCallback
**
** Purpose:
**
** Parameter Dictionary:
**
** Return Values:
**
**	SRV_NORMAL
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/

static CONDITION
neventReportRequestCallback(MSG_N_EVENT_REPORT_REQ * request,
			    MSG_N_EVENT_REPORT_RESP * response,
			    void *ctxPtr, DUL_PRESENTATIONCONTEXT * pc)
{
    CONDITION cond = 0;

/*  Following definition and assignment satisfy prototypes for dicom_services.h*/
    CALLBACK_CTX *ctx;
    ctx = (CALLBACK_CTX *) ctxPtr;

    strcpy(response->classUID, request->classUID);
    response->messageIDRespondedTo = request->messageID;


    if (strcmp(ctx->abstractSyntax, DICOM_SOPCLASSSTORAGECOMMITMENTPUSHMODEL) == 0) {
	cond = storageCommitmentRequest(request, response, ctx);
    } else
	return SRV_UNSUPPORTEDSERVICE;

    if (cond == 1)
	return SRV_NORMAL;
    else
	return 0;
}

#if 0
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
#endif

/* Entry points for public functions start here.
 */

/* neventReportRequest
**
*/

CONDITION
neventReportRequest(DUL_ASSOCIATESERVICEPARAMETERS * params,
		    DUL_ASSOCIATIONKEY ** association, DMAN_HANDLE ** dman,
	   DUL_PRESENTATIONCONTEXT * ctx, MSG_N_EVENT_REPORT_REQ ** request,
		    FIS_HANDLE ** fis)
{
    MSG_N_EVENT_REPORT_RESP response;
    CONDITION cond;
    CALLBACK_CTX cb;

    memset(&response, 0, sizeof(response));
    response.type = MSG_K_N_EVENT_REPORT_RESP;
    response.conditionalFields = 0;
    response.dataSetType = DCM_CMDDATANULL;

    cb.abstractSyntax = ctx->abstractSyntax;
    cb.fis = fis;
    cb.idb = NULL;
    cb.params = params;

    (void) MSG_DumpMessage(*request, stdout);

    cond = SRV_NEventReportResponse(association, ctx, request, &response,
				    neventReportRequestCallback, &cb, "");
    if (cond != SRV_NORMAL)
	return 0;

    return APP_NORMAL;
}
