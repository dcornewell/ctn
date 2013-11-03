/*
          Copyright (C) 1993, RSNA and Washington University

          The software and supporting documentation for the Radiological
          Society of North America (RSNA) 1993 Digital Imaging and
          Communications in Medicine (DICOM) Demonstration were developed
          at the
                  Electronic Radiology Laboratory
                  Mallinckrodt Institute of Radiology
                  Washington University School of Medicine
                  510 S. Kingshighway Blvd.
                  St. Louis, MO 63110
          as part of the 1993 DICOM Central Test Node project for, and
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
/*
** @$=@$=@$=
*/
/*
**				DINPACS 97
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
**
** Author, Date:	Steve Moore, 13-Mar-97
** Intent:
**
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:43:42 $
** Source File:		$RCSfile: commit_request.c,v $
** Revision:		$Revision: 1.2 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.2 $ $RCSfile: commit_request.c,v $";

#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include "dicom.h"
#include "lst.h"
#include "condition.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "dicom_uids.h"
#include "dicom_sq.h"
#include "manage.h"
#include "fis.h"


static void
createNActionDataSet(FIS_HANDLE ** fisHandle, char *transactionUID,
		     DCM_OBJECT ** dataSet)
{
    LST_HEAD *l;
    CONDITION cond;
    DCM_OBJECT *obj;
    FIS_COMMITSOPREFRECORD *sopRefRecord;
    SQ_REFERENCEDSOPSEQUENCE *refSopSeq;
    LST_HEAD *sqList;

    DCM_ELEMENT *commitRefSOPSeq;

    l = LST_Create();
    sqList = LST_Create();
    if (l == NULL || sqList == NULL) {
	fprintf(stderr, "Could not allocate a new list head in createNActionDataSet\n");
	COND_DumpConditions();
	exit(1);
    }
    cond = DCM_CreateObject(dataSet, 0);
    if (cond != DCM_NORMAL) {
	fprintf(stderr, "Could not allocate a new DCM object in createNActionDataSet\n");
	COND_DumpConditions();
	exit(1);
    }
    cond = FIS_Get(fisHandle, FIS_K_COMMITSOPREF, FIS_K_STORAGECOMMITREQ,
		   transactionUID, 1, l);
    if (cond != FIS_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
    while ((sopRefRecord = LST_Dequeue(&l)) != NULL) {
	refSopSeq = calloc(1, sizeof(*refSopSeq));
	if (refSopSeq == NULL) {
	    fprintf(stderr, "Could not calloc a new ref SOP Sequence struct\n");
	    exit(1);
	}
	refSopSeq->type = SQ_K_REFERENCEDSOPSEQUENCE;
	strcpy(refSopSeq->referencedSOPClassUID, sopRefRecord->SOPClaUID);
	strcpy(refSopSeq->referencedSOPInstanceUID, sopRefRecord->SOPInsUID);

	LST_Enqueue(&sqList, refSopSeq);

	free(sopRefRecord);
    }

    cond = SQ_BuildSequence(&sqList, SQ_K_REFERENCEDSOPSEQUENCE, &commitRefSOPSeq);
    if (cond != SQ_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
    cond = DCM_AddElement(dataSet, commitRefSOPSeq);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	exit(1);
    } {
	DCM_ELEMENT e = {DCM_IDTRANSACTIONUID, DCM_UI, "", 1, 0, NULL};
	e.length = strlen(transactionUID);
	e.d.string = transactionUID;
	cond = DCM_AddElement(dataSet, &e);
	if (cond != DCM_NORMAL) {
	    COND_DumpConditions();
	    exit(1);
	}
    }
}

/* filmBoxPrintCallback
**
** Purpose:
**	Callback function to deal with the N-ACTION response message
**
** Parameter Dictionary:
**	actionRequest	Pointer to the N-ACTION request message
**	actionResponse	Pointer to the N-ACTION response message
**	ctx
**
** Return Values:
**
**	SRV_CALLBACKABORTEDSERVICE
**	SRV_NORMAL
**
** Notes:
**	We pass the filmBox parameter as a void* and later cast to
**	PRN_BASICFILMBOX* to satisfy prototypes for n-action callbacks
**	defined in dicom_services.h.
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
actionRequestCallback(MSG_N_ACTION_REQ * actionRequest,
		      MSG_N_ACTION_RESP * actionResponse,
		      void *ctx)
{
    CONDITION cond;

#if 0
/*  Definition and cast assignment below help satisfy prototypes defined
**  for n-action callbacks defined in dicom_services.h.
*/
    PRN_BASICFILMBOX *filmBox;
    filmBox = (PRN_BASICFILMBOX *) filmBoxVoid;

    if (actionResponse->status == MSG_K_SUCCESS) {
	printf("SCU : Received N-ACTION Response, status success\n");
	/* verify if the SCP supports a PRINT JOB SOP class */
	if (actionResponse->dataSetType != DCM_CMDDATANULL) {
	    if (!silent) {
		/* now dump the information */
		cond = DCM_DumpElements(&actionResponse->actionReply);
		if (cond != DCM_NORMAL) {
		    printf("In filmBoxPrintCallback, DCM_DumpElement failed\n");
		    return SRV_CALLBACKABORTEDSERVICE;
		}
	    }
	    filmBoxPtr = &localFilmBox;
	    cond = PRN_ParseObject(&actionResponse->actionReply,
				 PRN_K_BASICFILMBOX, (void **) &filmBoxPtr);
	    if (cond != PRN_NORMAL) {
		printf("In filmBoxPrintCallback, PRN Parse Failed\n");
		return SRV_CALLBACKABORTEDSERVICE;
	    }
	    if (!(localFilmBox.bfbAttributeFlag & PRN_BFB_K_REFPRINTJOBSEQ)) {
		printf("In filmBoxPrintCallback, PRN Parse Failed\n");
		return SRV_CALLBACKABORTEDSERVICE;
	    }
	    strcpy(filmBox->refPrintJobClassUID,
		   localFilmBox.refPrintJobClassUID);
	    strcpy(filmBox->refPrintJobInstanceUID,
		   localFilmBox.refPrintJobInstanceUID);
	    filmBox->bfbAttributeFlag |= PRN_BFB_K_REFPRINTJOBSEQ;

	} else {
	    printf(" NO ACTION REPLY RECEIVED\n");
	}
    }
#endif
    return SRV_NORMAL;
}

CONDITION
sendStorageCommitRequest(FIS_HANDLE ** fisHandle, char *transactionUID,
			 DUL_ASSOCIATIONKEY ** association,
			 DUL_ASSOCIATESERVICEPARAMETERS * params)
{
    CONDITION cond;
    MSG_N_ACTION_REQ request;
    MSG_N_ACTION_RESP response;
    void *ctx = NULL;
    DCM_OBJECT *dataSet;

    createNActionDataSet(fisHandle, transactionUID, &dataSet);
    DCM_DumpElements(&dataSet, 0);

    request.messageID = SRV_MessageIDOut();
    request.dataSetType = DCM_CMDDATAOTHER;
    request.actionTypeID = 1;
    request.type = MSG_K_N_ACTION_REQ;
    request.actionInformation = dataSet;
    strcpy(request.classUID, DICOM_SOPCLASSSTORAGECOMMITMENTPUSHMODEL);
    strcpy(request.instanceUID, DICOM_WELLKNOWNSTORAGECOMMITMENTPUSHMODEL);

    cond = SRV_NActionRequest(association, params,
			      DICOM_SOPCLASSSTORAGECOMMITMENTPUSHMODEL,
			      &request, &response, actionRequestCallback,
			      NULL, ".");
    if (cond != SRV_NORMAL) {
	printf("In sendStorageCommitRequest : SRV_NActionRequest failed\n");
	return cond;
    }
    /**stat = response.status;*/
    return SRV_NORMAL;
}
