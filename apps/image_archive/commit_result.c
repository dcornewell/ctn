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
** Author, Date:	Steve Moore, 20-Mar-97
** Intent:
**
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:37:38 $
** Source File:		$RCSfile: commit_result.c,v $
** Revision:		$Revision: 1.6 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.6 $ $RCSfile: commit_result.c,v $";

#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _WIN32
#include <sys/file.h>
#endif
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
#ifdef CTN_MULTIBYTE
#include "tblmb.h"
#include "idbmb.h"
#else
#include "tbl.h"
#include "idb.h"
#endif
#include "manage.h"
#include "fis.h"

#include "image_archive.h"
#include "commit_result.h"

/* First part of the file defines all of the private functions. */

/* eventRequestCallback
**
** Purpose:
**	Callback function to deal with the N-EVENT REPORT response message
**
** Parameter Dictionary:
**	eventReportRequest	Pointer to the N-EVENT REPORT request message
**	eventReportResponse	Pointer to the N-EVENT REPORT response message
**	ctx
**
** Return Values:
**
**	SRV_CALLBACKABORTEDSERVICE
**	SRV_NORMAL
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
eventRequestCallback(MSG_N_EVENT_REPORT_REQ * eventReportRequest,
		     MSG_N_EVENT_REPORT_RESP * eventReportResponse,
		     void *ctx)
{
    return SRV_NORMAL;
}

/* addToSuccesses
**
** Purpose:
**	This function adds one SOP Reference record to a list of records
**	that have been successfully committed to storage.  The caller
**	passes an FIS record with the reference information.  We allocate
**	a sequence element, fill it with values from the FIS record, and
**	place the sequence element at the end of the caller's list.
**
** Parameter Dictionary:
**	sopRefRecord	Caller's FIS record which references the SOP Instance
**	successes	The current list of successful records.
**	ctx
**
** Return Values:
**
**	APP_NORMAL
**	APP_FAILURE
**
** Notes:
**
** Algorithm:
*/

static CONDITION
addToSuccesses(FIS_COMMITSOPREFRECORD * sopRefRecord,
	       LST_HEAD ** successes)
{
    SQ_REFERENCEDSOPSEQUENCE *sq;

    sq = calloc(1, sizeof(*sq));
    if (sq == NULL)
	return COND_PushCondition(APP_FAILURE, APP_Message(APP_FAILURE),
				  "Failed to allocate a referenced SOP seq",
				  "addToSuccesses");

    sq->type = SQ_K_REFERENCEDSOPSEQUENCE;
    strcpy(sq->referencedSOPClassUID, sopRefRecord->SOPClaUID);
    strcpy(sq->referencedSOPInstanceUID, sopRefRecord->SOPInsUID);
    if (LST_Enqueue(successes, sq) != LST_NORMAL)
	return COND_PushCondition(APP_FAILURE, APP_Message(APP_FAILURE),
				  "Failed to enqueue a referenced SOP seq",
				  "addToSuccesses");

    return APP_NORMAL;

}

/* addToFailures
**
** Purpose:
**	This function adds one SOP Reference record to a list of records
**	that have not been committed to storage.  The caller
**	passes an FIS record with the reference information.  We allocate
**	a sequence element, fill it with values from the FIS record, and
**	place the sequence element at the end of the caller's list.
**
** Parameter Dictionary:
**	sopRefRecord	Caller's FIS record which references the SOP Instance
**	failues	The current list of failure records.
**	ctx
**
** Return Values:
**
**	APP_NORMAL
**	APP_FAILURE
**
** Notes:
**	We currently hard code the failure reason to 0x110, general processing
**	failure.  We could do better.
**
** Algorithm:
*/

static CONDITION
addToFailures(FIS_COMMITSOPREFRECORD * sopRefRecord,
	      LST_HEAD ** failures)
{
    SQ_FAILEDSOPSEQUENCE *failedSOPSeq;

    failedSOPSeq = calloc(1, sizeof(*failedSOPSeq));
    if (failedSOPSeq == NULL)
	return COND_PushCondition(APP_FAILURE, APP_Message(APP_FAILURE),
				  "Failed to allocate a failed SOP seq",
				  "addToFailures");

    failedSOPSeq->type = SQ_K_FAILEDSOPSEQUENCE;
    strcpy(failedSOPSeq->referencedSOPClassUID, sopRefRecord->SOPClaUID);
    strcpy(failedSOPSeq->referencedSOPInstanceUID, sopRefRecord->SOPInsUID);
    failedSOPSeq->failureReason = 0x110;	/* General processing failure */
    if (LST_Enqueue(failures, failedSOPSeq) != LST_NORMAL)
	return COND_PushCondition(APP_FAILURE, APP_Message(APP_FAILURE),
				  "Failed to enqueue a failed SOP seq",
				  "addToFailures");
    return APP_NORMAL;
}

/* selectCallback
**
** Purpose:
**	selectCallback is a callback function for an IDB_Select call.  It is
**	called for each record that matches the select below.  In this case,
**	all we care about is whether the select found a record, so we don't
**	bother to process the record.
**
** Parameter Dictionary:
**	q	Query structure with data filled in
**	count	The number of the current record.  No order is implied.
**	ctx	Context information, unused here.
**
** Return Values:
**
**	IDB_NORMAL
**
** Notes:
**
** Algorithm:
*/

static CONDITION
selectCallback(IDB_Query * q, long count, void *ctx)
{
    return IDB_NORMAL;
}

/* determineCommitSuccessesFailures
**
** Purpose:
**	determineCommitSuccessesFailures examines the image database (IDB)
**	to determine if images have been committed to storage.  The caller
**	passes a transaction UID which can be used to find a set of images
**	that are to be committed.  This function searches the image database
**	for each image in the set.  For each image that is found or not found,
**	an entry is made in the successes or failures list (as appropriate).
**
** Parameter Dictionary:
**	transactionUID	Transaction UID of a commit request.
**	idb		Handle to image database used for image queries.
**	fis		Handle to FIS used to get the set of SOP references.
**	successes	List which will hold the references to the images
**			were stored successfully by the server.
**	failures	List which will hold the references to the images
**			which were not stored by the server.
**
** Return Values:
**
**	APP_NORMAL
**	APP_FAILURE
**
** Notes:
**
** Algorithm:
*/

static CONDITION
determineCommitSuccessesFailures(char *transactionUID,
				 IDB_HANDLE ** idb, FIS_HANDLE ** fis,
				 LST_HEAD * successes,
				 LST_HEAD * failures)
{
    LST_HEAD *l;
    CONDITION cond;
    FIS_COMMITSOPREFRECORD *sopRefRecord;

    l = LST_Create();

    if (l == NULL || successes == NULL || failures == NULL) {
	fprintf(stderr, "Could not allocate a new list head in nactionCleanup\n");
	COND_DumpConditions();
	exit(1);
    }
    cond = FIS_Get(fis, FIS_K_COMMITSOPREF, FIS_K_STORAGECOMMITREQ,
		   transactionUID, 1, l);
    if (cond != FIS_NORMAL) {
	COND_DumpConditions();
	return 0;
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
	free(sopRefRecord);
    }
    LST_Destroy(&l);
    return APP_NORMAL;
}

/* createNEventDataSet
**
** Purpose:
**	createNEventDataSet creates and fills a DICOM object which will be the
**	data set in an N EVENT REPORT REQUEST message.  This dataset contains
**	the Transaction UID for a storage commitment request, the retrieve AE
**	title and the sequences of successful and failed SOP references.
**
** Parameter Dictionary:
**	transactionUID	Transaction UID for this storage commitment request.
**	retrieveAETitle	AE title of the server which can be queried for the images.
**	successes	List which holds the references to the images
**			were stored successfully by the server.
**	failures	List which holds the references to the images
**			which were not stored by the server.
**	dataSet		Reference to a DCM OBJECT in caller's space.  We will
**			create the object and fill with the proper attributes.
**
** Return Values:
**
**	APP_NORMAL
**	APP_FAILURE
**
** Notes:
**	This function assumes that the lists already contain the sets of
**	successfully stored and unstored SOP Instance UIDs.  That means this
**	function merely needs to assemble the proper DICOM attributes into
**	the dataset.  No further searching is required.
**
** Algorithm:
*/

static CONDITION
createNEventDataSet(char *transactionUID, char *retrieveAETitle,
		    LST_HEAD * successes, LST_HEAD * failures,
		    DCM_OBJECT ** dataSet)
{
    CONDITION cond;

    DCM_ELEMENT *commitRefSOPSeq;
    DCM_ELEMENT *failedSOPSeq;

    cond = DCM_CreateObject(dataSet, 0);
    if (cond != DCM_NORMAL)
	return COND_PushCondition(APP_FAILURE, APP_Message(APP_FAILURE),
				  "Failed to allocate a new DCM object",
				  "createNEventDataSet");

    if (LST_Count(&successes) != 0) {
	cond = SQ_BuildSequence(&successes, SQ_K_REFERENCEDSOPSEQUENCE,
				&commitRefSOPSeq);
	if (cond != SQ_NORMAL)
	    return COND_PushCondition(APP_FAILURE, APP_Message(APP_FAILURE),
				  "Failed to build referenced SOP sequence",
				      "createNEventDataSet");

	cond = DCM_AddElement(dataSet, commitRefSOPSeq);
	if (cond != DCM_NORMAL)
	    return COND_PushCondition(APP_FAILURE, APP_Message(APP_FAILURE),
		      "Failed to add referenced SOP sequence to DCM object",
				      "createNEventDataSet");
    }
    if (LST_Count(&failures) != 0) {
	cond = SQ_BuildSequence(&failures, SQ_K_FAILEDSOPSEQUENCE,
				&failedSOPSeq);
	if (cond != SQ_NORMAL)
	    if (cond != SQ_NORMAL)
		return COND_PushCondition(APP_FAILURE, APP_Message(APP_FAILURE),
				      "Failed to build failed SOP sequence",
					  "createNEventDataSet");

	cond = DCM_AddElement(dataSet, failedSOPSeq);
	if (cond != DCM_NORMAL)
	    return COND_PushCondition(APP_FAILURE, APP_Message(APP_FAILURE),
			  "Failed to add failed SOP sequence to DCM object",
				      "createNEventDataSet");
    } {
	DCM_ELEMENT e = {DCM_IDTRANSACTIONUID, DCM_UI, "", 1, 0, NULL};
	e.length = strlen(transactionUID);
	e.d.string = transactionUID;
	cond = DCM_AddElement(dataSet, &e);
	if (cond != DCM_NORMAL)
	    return COND_PushCondition(APP_FAILURE, APP_Message(APP_FAILURE),
			      "Failed to add transaction UID to DCM object",
				      "createNEventDataSet");

    }
    {
	DCM_ELEMENT e = {DCM_IDRETRIEVEAETITLE, DCM_AE, "", 1, 0, NULL};
	e.length = strlen(retrieveAETitle);
	e.d.string = retrieveAETitle;
	cond = DCM_AddElement(dataSet, &e);
	if (cond != DCM_NORMAL)
	    return COND_PushCondition(APP_FAILURE, APP_Message(APP_FAILURE),
			    "Failed to add retrieve AE title to DCM object",
				      "createNEventDataSet");

    }

    return APP_NORMAL;
}

/* The section below contains the public functions for this file. */

/* processStorageCommitRequest
**
** Purpose:
**	processStorageCommitRequest examines one storage commitment request and
**	sends an N-EVENT REPORT REQUEST message with the status of the storage
**	request.  It invokes several other functions to determine if the storage
**	commitment was successful, and then packages the results and sends them
**	via a DICOM N-EVENT REPORT Request.
**
** Parameter Dictionary:
**	transactionUID	Transaction UID for this storage commitment request.
**	association	A key for an existing association.  We will use this
**			to send the event report.
**	params		The association parameters which contain the
**			presentation contexts and other useful information.
**	idbHandle	Handle for the image database.  Used for queries.
**	fisHandle	Handle for the information system.  Used for queries.
**
** Return Values:
**
**	APP_NORMAL
**	APP_FAILURE
**
** Notes:
**
** Algorithm:
*/

CONDITION
processStorageCommitRequest(char *transactionUID,
			    DUL_ASSOCIATIONKEY ** association,
			    DUL_ASSOCIATESERVICEPARAMETERS * params,
			    IDB_HANDLE ** idbHandle, FIS_HANDLE ** fisHandle)
{
    LST_HEAD *successes;
    LST_HEAD *failures;
    CONDITION cond;
    MSG_N_EVENT_REPORT_REQ request;
    MSG_N_EVENT_REPORT_RESP response;
    void *ctx = NULL;
    DCM_OBJECT *dataSet;

    successes = LST_Create();
    failures = LST_Create();
    if (successes == NULL || failures == NULL)
	return COND_PushCondition(APP_FAILURE, APP_Message(APP_FAILURE),
				  "Could not allocate a new list head",
				  "processStorageCommitRequest");

    cond = determineCommitSuccessesFailures(transactionUID,
					    idbHandle, fisHandle,
					    successes, failures);
    if (cond != APP_NORMAL)
	return 0;

    cond = createNEventDataSet(transactionUID, params->calledAPTitle,
			       successes, failures, &dataSet);
    if (cond != APP_NORMAL)
	return COND_PushCondition(APP_FAILURE, APP_Message(APP_FAILURE),
				  "Could not create N-EVENT REPORT data set",
				  "processStorageCommitRequest");

    DCM_DumpElements(&dataSet, 0);

    request.messageID = SRV_MessageIDOut();
    request.dataSetType = DCM_CMDDATAOTHER;
    request.eventTypeID = (LST_Count(&failures) == 0) ? 1 : 2;
    request.type = MSG_K_N_EVENT_REPORT_REQ;
    request.dataSet = dataSet;
    strcpy(request.classUID, DICOM_SOPCLASSSTORAGECOMMITMENTPUSHMODEL);
    strcpy(request.affectedInstanceUID, DICOM_WELLKNOWNSTORAGECOMMITMENTPUSHMODEL);

    cond = SRV_NEventReportRequest(association, params,
				   &request, &response, eventRequestCallback,
				   NULL, ".");
    if (cond != SRV_NORMAL)
	return COND_PushCondition(APP_FAILURE, APP_Message(APP_FAILURE),
				  "N Event Report Request failed",
				  "processStorageCommitRequest");

    /**stat = response.status;*/
    return APP_NORMAL;
}

/* establishCommitResultAssociation
**
** Purpose:
**	establishCommitResultAssociation requests an Association with a server
**	process for the purpose of sending an Event Report in the Storage
**	Commitment PUSH Model class.  We propose one presentation context as
**	an SCP of the SOP class.
**
** Parameter Dictionary:
**	transactionUID	Transaction UID for this storage commitment request.
**			We use this to find the name of the requesting device.
**	networkKey	An active network key used to make a new association key
**	association	The key for the association that we request.  We make
**			this key as part of this function.  The caller uses the
**			key in other functions to send messages to the peer.
**	params		Address of a association parameters structure in the
**			caller's space.  We place some values in there to
**			request the association.
**	dmanHandle	Handle for the control database.  We use this to
**			find information about the remote application
**			(node name, port number).
**	fisHandle	Handle for the Information System.  We use this to
**			map the transaction ID to a particular event.
**
** Return Values:
**
**	APP_NORMAL
**	APP_FAILURE
**
** Notes:
**
** Algorithm:
**	Find the transaction.  This gives you name of the requesting app.
**	Lookup the requesting app in the control database.
**	Place host name and AE titles in the params structure.
**	Request the Storage Commitment Push Model SOP Class (SCP).
**	Make the association request.
*/

CONDITION
establishCommitResultAssociation(char *transactionUID,
				 DUL_NETWORKKEY ** networkKey,
				 DUL_ASSOCIATIONKEY ** association,
				 DUL_ASSOCIATESERVICEPARAMETERS * params,
				 DMAN_HANDLE ** dmanHandle,
				 FIS_HANDLE ** fisHandle)
{
    CONDITION cond;
    FIS_STORAGECOMMITREQRECORD commitRequest;
    char localHost[128];
    DMAN_APPLICATIONENTITY ae;

    cond = FIS_GetOne(fisHandle, FIS_K_STORAGECOMMITREQ, FIS_K_STORAGECOMMITREQ,
		      transactionUID, 0, &commitRequest);
    if (cond != FIS_NORMAL)
	return COND_PushCondition(APP_FAILURE, APP_Message(APP_FAILURE),
			      "Lookup failed on storage commitment request",
				  "establishCommitResultsAssociation");

    DUL_DefaultServiceParameters(params);
#ifdef _WIN32
    strcpy(localHost, "");
#else
    (void) gethostname(localHost, sizeof(localHost));
#endif
    cond = DMAN_LookupApplication(dmanHandle, commitRequest.ReqAE, &ae);
    if (cond != DMAN_NORMAL)
	return COND_PushCondition(APP_FAILURE, APP_Message(APP_FAILURE),
				  "Lookup failed on application entity",
				  "establishCommitResultsAssociation");

    sprintf(params->calledPresentationAddress, "%s:%-d", ae.Node, ae.Port);
    strcpy(params->callingPresentationAddress, localHost);
    strcpy(params->calledAPTitle, commitRequest.ReqAE);
    strcpy(params->callingAPTitle, commitRequest.ResAE);

    cond = SRV_RequestServiceClass(DICOM_SOPCLASSSTORAGECOMMITMENTPUSHMODEL,
				   DUL_SC_ROLE_SCP, params);
    if (CTN_INFORM(cond))
	(void) COND_PopCondition(FALSE);
    else if (cond != SRV_NORMAL)
	return COND_PushCondition(APP_FAILURE, APP_Message(APP_FAILURE),
		 "Unable to get Storage Commitment Class from SRV facility",
				  "establishCommitResultsAssociation");

    cond = DUL_RequestAssociation(networkKey, params, association);
    if (cond != DUL_NORMAL)
	return COND_PushCondition(APP_ASSOCIATIONINITIATIONFAILED,
			       APP_Message(APP_ASSOCIATIONINITIATIONFAILED),
				  params->callingAPTitle,
				  params->calledAPTitle,
				  params->calledPresentationAddress);

    return APP_NORMAL;

}
