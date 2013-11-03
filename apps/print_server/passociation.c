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
** Module Name(s):	nextAssociation
**
**	Private Routines
**			associationCheck
**			checkDICOMVersion
**			checkCalledNode
**			checkCallingNode
**			checkApplicationConnection
** Author, Date:	Aniruddha S. Gokhale, 23-June-94
** Intent:		This module implements routines that accept arriving
**			Association requests and verify their validity.
**			These routines are always called by the Parent
**			server process that listens on a specified port.
** Last Update:		$Author: smm $, $Date: 1999-02-07 18:55:56 $
** Source File:		$RCSfile: passociation.c,v $
** Revision:		$Revision: 1.1 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.1 $ $RCSfile: passociation.c,v $";

#include "print_server.h"

extern CTNBOOLEAN verboseDUL,
    gqueueFlag;
extern char *controlDatabase;

static CONDITION
associationCheck(char *node, DUL_ASSOCIATESERVICEPARAMETERS * params,
		 DMAN_HANDLE ** manageHandle, CTNBOOLEAN forgive,
		 DUL_ABORTITEMS * abortItems);
static CONDITION
checkDICOMVersion(char *node, DUL_ASSOCIATESERVICEPARAMETERS * params,
    CTNBOOLEAN forgive, DMAN_HANDLE ** handle, DUL_ABORTITEMS * abortItems);
static CONDITION
checkCalledNode(char *node, DUL_ASSOCIATESERVICEPARAMETERS * params,
    CTNBOOLEAN forgive, DMAN_HANDLE ** handle, DUL_ABORTITEMS * abortItems);
static CONDITION
checkCallingNode(char *node, DUL_ASSOCIATESERVICEPARAMETERS * params,
    CTNBOOLEAN forgive, DMAN_HANDLE ** handle, DUL_ABORTITEMS * abortItems);
static CONDITION
checkApplicationConnection(char *node, DUL_ASSOCIATESERVICEPARAMETERS * params,
    CTNBOOLEAN forgive, DMAN_HANDLE ** handle, DUL_ABORTITEMS * abortItems);
static CONDITION
    lookupCFGtable(DMAN_HANDLE **, DUL_ASSOCIATESERVICEPARAMETERS *, int *);

char *classArray[] = {		/* we support only the Print Management SOP
				 * meta classes and the verification class */
    DICOM_SOPCLASSVERIFICATION,
    DICOM_SOPCLASSGREYSCALEPRINTMGMTMETA,
/*    DICOM_SOPCLASSCOLORPRINTMGMTMETA, */
    /* DICOM_SOPCLASSPRINTJOB, */
};


/* nextAssociationRequest
**
** Purpose:
**	Get the next Association request from the client side.
**
** Parameter Dictionary:
**	node		Server application entity name
**	network		Handle to the network key for this association
**	service		Pointer to the service parameters structure to
**			be negotiated
**	maxPDU		Maximum size of the PDU
**	forgiveFlag	To indicate if certain errors in the Association
**			establishment could be tolerated or not
**	gqID		The Generalized Queue ID that is present in the
**			Print Server Config database for a given calling
**			and called application title.
**	association	Handle to the Association to be returned on success
**
** Return Values:
**
** Notes:
**	In case of the concurrent server, the parent server process
**	invokes this function.
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
nextAssociationRequest(char *node, DUL_NETWORKKEY ** network,
	     DUL_ASSOCIATESERVICEPARAMETERS * service, unsigned long maxPDU,
       CTNBOOLEAN forgiveFlag, int *gqID, DUL_ASSOCIATIONKEY ** association)
{
    CTNBOOLEAN
	drop,			/* indicates whether the association is to be
				 * dropped */
	acceptFlag,		/* indicates if association is accepted */
	acceptPresentationContext;
    /*
     * indicates whether the presentation context is accepted
     */
    CONDITION
	cond,			/* return condition codes */
	rtnCond = APP_NORMAL;
    DUL_PRESENTATIONCONTEXT
	* requestedCtx;		/* Handle to requested presentation context */
    int
        classCount;		/* holds the number of classes supported */
    DUL_SC_ROLE
	scRole;			/* indicates the role that the application
				 * plays */
    DUL_ABORTITEMS
	abortItems;		/* structure maintaining information on why
				 * an association is rejected */
    DMAN_HANDLE
	* manageHandle = NULL;	/* Handle to the DMAN structure. Needed to
				 * validate the access rights */

    (void) memset(service, 0, sizeof(*service));
    service->maxPDU = maxPDU;
    strcpy(service->calledImplementationClassUID,
	   MIR_IMPLEMENTATIONCLASSUID);
    strcpy(service->calledImplementationVersionName,
	   MIR_IMPLEMENTATIONVERSIONNAME);

    /* Block until an association request is received */
    cond = DUL_ReceiveAssociationRQ(network, DUL_BLOCK,
				    service, association);
    if (cond != DUL_NORMAL)
	return COND_PushCondition(APP_ERROR(APP_ASSOCIATIONRQFAILED));

    acceptFlag = TRUE;
    drop = FALSE;

    cond = DMAN_Open(controlDatabase, service->callingAPTitle,
		     service->calledAPTitle, &manageHandle);
    if (cond != DMAN_NORMAL) {
	acceptFlag = FALSE;
	COND_DumpConditions();
	rtnCond = COND_PushCondition(APP_ERROR(APP_ASSOCIATIONRQFAILED));
    }
    if (acceptFlag) {
	/* validate all the access rights for the incoming association */
	cond = associationCheck(node, service, &manageHandle, forgiveFlag,
				&abortItems);
	if (CTN_ERROR(cond)) {
	    acceptFlag = FALSE;
	    fprintf(stderr, "Incorrect Association Request\n");
	    (void) DUL_RejectAssociationRQ(association, &abortItems);
	    rtnCond = COND_PushCondition(APP_ERROR(APP_ASSOCIATIONRQFAILED));
	} else if (!CTN_SUCCESS(cond)) {
	    COND_DumpConditions();
	}
    }
    if (acceptFlag) {		/* at this point, the acceptFlag being TRUE
				 * implies that the association check was
				 * successful */
	if (verboseDUL)
	    DUL_DumpParams(service);


	/* Get the GQ ID entry from the Print Server Config Table */
	if (gqueueFlag) {
	    cond = lookupCFGtable(&manageHandle, service, gqID);
	    if (cond != APP_NORMAL) {
		printf("Error getting GQ ID. Queue facility ignored\n");
	    }
	}
	/*
	 * Go thru the list of requested presentation contexts and count the
	 * number of supported SOP classes
	 */
	requestedCtx = LST_Head(&service->requestedPresentationContext);
	if (requestedCtx != NULL)
	    (void) LST_Position(&service->requestedPresentationContext,
				requestedCtx);
	classCount = 0;
	while (requestedCtx != NULL) {
	    int i;

	    acceptPresentationContext = FALSE;
	    /* find if the SOP class requested is supported */
	    for (i = 0; i < DIM_OF(classArray); i++) {
		if (strcmp(classArray[i], requestedCtx->abstractSyntax) == 0) {
		    acceptPresentationContext = TRUE;
		    break;
		}
	    }
	    if (acceptPresentationContext) {
		/*
		 * at this point, the current requested presentation context
		 * is accepted. Define the role that this application will
		 * play by looking at the "proposedSCRole" parameter
		 */
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
		/*
		 * Accept this class and add it to the list of accepted
		 * presentation contexts
		 */
		cond = SRV_AcceptServiceClass(requestedCtx, scRole,
					      service);
		if (cond == SRV_NORMAL) {
		    classCount++;
		} else {
		    drop = TRUE;
		    printf("SRV Facility rejected SOP Class: %s\n",
			   requestedCtx->abstractSyntax);
		    COND_DumpConditions();
		}
	    } else {
		printf("Print server rejects: %s\n",
		       requestedCtx->abstractSyntax);
		cond = SRV_RejectServiceClass(requestedCtx,
				     DUL_PRESENTATION_REJECT_USER, service);
		if (cond != SRV_NORMAL) {
		    drop = TRUE;
		}
	    }
	    requestedCtx = LST_Next(&service->requestedPresentationContext);
	}
	printf("Supported classes: %d\n", classCount);

	if (drop) {		/* drop is TRUE implies something went wrong
				 * before and the received association is to
				 * be dropped. We set the return condition to
				 * be abnormal return and later drop the
				 * association */
	    rtnCond = COND_PushCondition(APP_ERROR(APP_ASSOCIATIONRQFAILED));
	}
    }
    if (manageHandle)
	(void) DMAN_Close(&manageHandle);	/* free the handle to the
						 * DMAN structure */
    if (rtnCond != APP_NORMAL) {
	(void) DUL_DropAssociation(association);
    }
    return rtnCond;
}


/* associationCheck
**
** Purpose:
**	Check the validity of the Association service parameters
**
** Parameter Dictionary:
**	node		Host name for which the authorization is to be
**			verified.
**	params		Service parameters describing the Association
**	manageHandle	Handle to the DMAN structure that has pointers to
**			various access lists.
**	forgive		Decides whether to return a warning or a failure status
**	abortItems	Structure specifying reasons for aborting the
**			Association
**
** Return Values:
**
**	APP_NORMAL
**	APP_PARAMETERFAILURE
**	APP_PARAMETERWARNINGS
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
**	- Search the "Application Entity" table by title to see if the
**	called AE title identifies an application that exists on this
**	node.
*/

typedef struct {
    CONDITION(*function) ();
}   FUNCTION;

/* define the local functions to be invoked as part of the Association check*/
static FUNCTION testVector[] = {{checkDICOMVersion}, {checkCalledNode},
{checkCallingNode}, {checkApplicationConnection}};

static CONDITION
associationCheck(char *node, DUL_ASSOCIATESERVICEPARAMETERS * params,
		 DMAN_HANDLE ** manageHandle, CTNBOOLEAN forgive,
		 DUL_ABORTITEMS * abortItems)
{
    CONDITION
    cond,
    rtnCond = APP_NORMAL;
    int
        index;


    cond = APP_NORMAL;
    /* invoke each validation function one by one */
    for (index = 0;
	 index < DIM_OF(testVector) && CTN_SUCCESS(cond); index++) {
	cond = testVector[index].function(node, params, forgive, manageHandle,
					  abortItems);
	if (cond != APP_NORMAL) {
	    if (!CTN_FATAL(cond) && forgive) {
		cond = APP_NORMAL;
	    }
	}
    }

    if (cond != APP_NORMAL) {
	if (forgive)
	    rtnCond = COND_PushCondition(APP_PARAMETERWARNINGS,
					 APP_Message(APP_PARAMETERWARNINGS));
	else
	    rtnCond = COND_PushCondition(APP_PARAMETERFAILURE,
					 APP_Message(APP_PARAMETERFAILURE));
    }
    return rtnCond;
}

/* checkDICOMVersion
**
** Purpose:
**	Check the DICOM version number
**
** Parameter Dictionary:
**	node		Host name for which the authorization is to be
**			verified.
**	params		Service parameters describing the Association
**	manageHandle	Handle to the DMAN structure that has pointers to
**			various access lists.
**	forgive		Decides whether to return a warning or a failure status
**	abortItems	Structure specifying reasons for aborting the
**			Association
**
** Return Values:
**	APP_NORMAL
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
checkDICOMVersion(char *node, DUL_ASSOCIATESERVICEPARAMETERS * params,
     CTNBOOLEAN forgive, DMAN_HANDLE ** handle, DUL_ABORTITEMS * abortItems)
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
    return cond;
}

/* checkCalledNode
**
** Purpose:
**	Check if the called Title and the "node" match and if they are
**	authorized to accept the association.
**
** Parameter Dictionary:
**	node		Host name for which the authorization is to be
**			verified.
**	params		Service parameters describing the Association
**	manageHandle	Handle to the DMAN structure that has pointers to
**			various access lists.
**	forgive		Decides whether to return a warning or a failure status
**	abortItems	Structure specifying reasons for aborting the
**			Association
**
** Return Values:
**	APP_NORMAL
**	APP_ILLEGALSERVICEPARAMETER
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
checkCalledNode(char *node, DUL_ASSOCIATESERVICEPARAMETERS * params,
     CTNBOOLEAN forgive, DMAN_HANDLE ** handle, DUL_ABORTITEMS * abortItems)
{
    CONDITION
    cond,
    rtnCond = APP_NORMAL;

    cond = DMAN_VerifyApplication(handle, params->calledAPTitle, node);

    if (cond != DMAN_NORMAL) {
	rtnCond = COND_PushCondition(APP_ILLEGALSERVICEPARAMETER,
				   APP_Message(APP_ILLEGALSERVICEPARAMETER),
				  "Called AE Title", params->calledAPTitle);
    }
    if (rtnCond != APP_NORMAL) {
	abortItems->result = DUL_REJECT_PERMANENT;
	abortItems->source = DUL_ULSU_REJECT;
	abortItems->reason = DUL_ULSU_REJ_UNREC_CALLED_TITLE;
    }
    return rtnCond;
}

/* checkCallingNode
**
** Purpose:
**	Verify that the calling presentation address and the calling AE title
**	match
**
** Parameter Dictionary:
**	node		Host name for which the authorization is to be
**			verified.
**	params		Service parameters describing the Association
**	manageHandle	Handle to the DMAN structure that has pointers to
**			various access lists.
**	forgive		Decides whether to return a warning or a failure status
**	abortItems	Structure specifying reasons for aborting the
**			Association
**
** Return Values:
**	APP_NORMAL
**	APP_ILLEGALSERVICEPARAMETER
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
checkCallingNode(char *node, DUL_ASSOCIATESERVICEPARAMETERS * params,
     CTNBOOLEAN forgive, DMAN_HANDLE ** handle, DUL_ABORTITEMS * abortItems)
{
    CONDITION
    cond,
    rtnCond = APP_NORMAL;

    cond = DMAN_VerifyApplication(handle, params->callingAPTitle,
				  params->callingPresentationAddress);
    if (cond != DMAN_NORMAL) {
	rtnCond = COND_PushCondition(APP_ILLEGALSERVICEPARAMETER,
				   APP_Message(APP_ILLEGALSERVICEPARAMETER),
				"Calling AE Title", params->callingAPTitle);
    }
    if (rtnCond != APP_NORMAL) {
	abortItems->result = DUL_REJECT_PERMANENT;
	abortItems->source = DUL_ULSU_REJECT;
	abortItems->reason = DUL_ULSU_REJ_UNREC_CALLED_TITLE;
    }
    return rtnCond;
}

/* checkApplicationConnection
**
** Purpose:
**	Verify that the requesting peer and this application entity are
**	authorized to communicate with each other.
**
** Parameter Dictionary:
**	node		Host name for which the authorization is to be
**			verified.
**	params		Service parameters describing the Association
**	manageHandle	Handle to the DMAN structure that has pointers to
**			various access lists.
**	forgive		Decides whether to return a warning or a failure status
**	abortItems	Structure specifying reasons for aborting the
**			Association
**
** Return Values:
**	APP_NORMAL
**	APP_ILLEGALSERVICEPARAMETER
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
checkApplicationConnection(char *node, DUL_ASSOCIATESERVICEPARAMETERS * params,
     CTNBOOLEAN forgive, DMAN_HANDLE ** handle, DUL_ABORTITEMS * abortItems)
{
    CONDITION
    cond,
    rtnCond = APP_NORMAL;
    CTNBOOLEAN
	accessFlag;

    cond = DMAN_ApplicationAccess(handle, params->callingAPTitle,
				  params->calledAPTitle, &accessFlag);
    if ((cond != DMAN_NORMAL) || !accessFlag) {
	rtnCond = COND_PushCondition(APP_ILLEGALSERVICEPARAMETER,
				   APP_Message(APP_ILLEGALSERVICEPARAMETER),
				"Calling AE Title", params->callingAPTitle);
    }
    if (rtnCond != APP_NORMAL) {
	abortItems->result = DUL_REJECT_PERMANENT;
	abortItems->source = DUL_ULSU_REJECT;
	abortItems->reason = DUL_ULSU_REJ_UNREC_CALLED_TITLE;
    }
    return rtnCond;
}

static CONDITION
lookupCFGtable(DMAN_HANDLE ** handle, DUL_ASSOCIATESERVICEPARAMETERS * params,
	       int *gqID)
{
    CONDITION
    cond;
    DMAN_PRINTSERVERCFG
	criteria,
	work;


    *gqID = -1;
    memset(&work, 0, sizeof(DMAN_PRINTSERVERCFG));
    memset(&criteria, 0, sizeof(DMAN_PRINTSERVERCFG));
    criteria.Type = DMAN_K_PRINTSERVERCFG;
    work.Type = DMAN_K_PRINTSERVERCFG;
    criteria.Flag = DMAN_K_PRINTSERVER_REQUESTING |
	DMAN_K_PRINTSERVER_RESPONDING;
    strcpy(criteria.RequestingTitle, params->callingAPTitle);
    strcpy(criteria.RespondingTitle, params->calledAPTitle);

    cond = DMAN_Select(handle, (DMAN_GENERICRECORD *) & work,
		 (DMAN_GENERICRECORD *) & criteria, NULL, NULL, NULL, NULL);
    if (cond != DMAN_NORMAL) {
	printf("DMAN_Select failed to retrieve Print Server CFG tables\n");
	return cond;
    }
    *gqID = work.GQId;

    return APP_NORMAL;
}
