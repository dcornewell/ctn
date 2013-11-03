/*
+-+-+-+-+-+-+-+-+-
*/
/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	nextAssociation
** Author, Date:	Stephen Moore, 18-Mar-94
** Intent:
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:44:51 $
** Source File:		$RCSfile: association.c,v $
** Revision:		$Revision: 1.4 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.4 $ $RCSfile: association.c,v $";

#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#ifdef MACH
#include <unistd.h>
#endif
#include <sys/wait.h>
#endif

#include "dicom.h"
#include "tbl.h"
#include "lst.h"
#include "dicom_uids.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "manage.h"
#include "fis.h"

#include "ris_gateway.h"

extern CTNBOOLEAN verboseDUL;
extern char *controlDatabase;

#define	ACCESS_READ	0x01
#define	ACCESS_WRITE	0x02

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


typedef struct {
    char *SOPClassUID;
    int accessRights;
}   ACCESS_MAP;

static ACCESS_MAP map[] = {
    {DICOM_SOPCLASSVERIFICATION, 0},
    {DICOM_SOPCLASSDETACHEDPATIENTMGMT, 0},
    {DICOM_SOPCLASSDETACHEDPATIENTMGMTMETA, 0},
    {DICOM_SOPCLASSDETACHEDVISITMGMT, 0},
    {DICOM_SOPCLASSDETACHEDSTUDYMGMT, 0},
    {DICOM_SOPCLASSSTUDYCOMPONENTMGMT, 0},
    {DICOM_SOPCLASSDETACHEDRESULTSMGMT, 0},
    {DICOM_SOPCLASSDETACHEDRESULTSMGMTMETA, 0},
    {DICOM_SOPCLASSDETACHEDSTUDYMGMTMETA, 0},
    {DICOM_SOPCLASSDETACHEDINTERPRETMGMT, 0}
};

CONDITION
nextAssociationRequest(char *node, DUL_NETWORKKEY ** network,
	     DUL_ASSOCIATESERVICEPARAMETERS * service, unsigned long maxPDU,
		  CTNBOOLEAN forgiveFlag, DUL_ASSOCIATIONKEY ** association)
{
    CTNBOOLEAN
	drop,
	acceptFlag,
	acceptPresentationContext,
	readAccess,
	writeAccess;
    CONDITION
	cond,
	rtnCond = APP_NORMAL;
    DUL_PRESENTATIONCONTEXT
	* requestedCtx;
    int
        classCount;
    DUL_SC_ROLE
	scRole;
    DUL_ABORTITEMS
	abortItems;
    DMAN_HANDLE
	* manageHandle;

    (void) memset(service, 0, sizeof(*service));
    service->maxPDU = maxPDU;
    strcpy(service->calledImplementationClassUID,
	   MIR_IMPLEMENTATIONCLASSUID);
    strcpy(service->calledImplementationVersionName,
	   MIR_IMPLEMENTATIONVERSIONNAME);
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
	rtnCond = COND_PushCondition(APP_ERROR(APP_ASSOCIATIONRQFAILED));
    }
    if (acceptFlag) {

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
	if (acceptFlag) {
	    cond = DMAN_StorageAccess(&manageHandle, service->callingAPTitle,
			 service->calledAPTitle, &readAccess, &writeAccess);
	    if (cond != DMAN_NORMAL) {
		readAccess = writeAccess = FALSE;
		COND_DumpConditions();
	    }
	}
    }
    if (acceptFlag) {
	if (verboseDUL)
	    DUL_DumpParams(service);


	requestedCtx = LST_Head(&service->requestedPresentationContext);
	if (requestedCtx != NULL)
	    (void) LST_Position(&service->requestedPresentationContext,
				requestedCtx);
	classCount = 0;
	while (requestedCtx != NULL) {
	    ACCESS_MAP *p;
	    int i,
	        grantedAccess;

	    acceptPresentationContext = FALSE;
	    for (p = map, i = 0; i < DIM_OF(map) && !acceptPresentationContext; i++) {
		if (strcmp(p->SOPClassUID, requestedCtx->abstractSyntax) == 0)
		    acceptPresentationContext = TRUE;
		else
		    p++;
	    }
	    if (acceptPresentationContext) {
		if ((p->accessRights & ACCESS_WRITE)) {
		    if (!writeAccess)
			acceptPresentationContext = FALSE;
		}
		if ((p->accessRights & ACCESS_READ)) {
		    if (!readAccess)
			acceptPresentationContext = FALSE;
		}
	    }
	    if (acceptPresentationContext) {
		switch (requestedCtx->proposedSCRole) {
		case DUL_SC_ROLE_DEFAULT:
		    acceptPresentationContext = FALSE;
		    break;
		case DUL_SC_ROLE_SCU:
		    acceptPresentationContext = FALSE;
		    break;
		case DUL_SC_ROLE_SCP:
		    scRole = DUL_SC_ROLE_SCP;
		    break;
		case DUL_SC_ROLE_SCUSCP:
		    scRole = DUL_SC_ROLE_SCP;
		    break;
		default:
		    acceptPresentationContext = FALSE;
		    break;
		}
	    }
	    if (acceptPresentationContext) {
		cond = SRV_AcceptServiceClass(requestedCtx, scRole,
					      service);
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
				     DUL_PRESENTATION_REJECT_USER, service);
		if (cond != SRV_NORMAL) {
		    drop = TRUE;
		}
	    }
	    requestedCtx = LST_Next(&service->requestedPresentationContext);
	}
	printf("Supported classes: %d\n", classCount);

	if (drop) {
	    (void) DUL_DropAssociation(association);
	    rtnCond = COND_PushCondition(APP_ERROR(APP_ASSOCIATIONRQFAILED));
	}
    }
    (void) DMAN_Close(&manageHandle);
    return rtnCond;
}


/* associationCheck
**
** Purpose:
**	Check the validity of the Association service parameters
**
** Parameter Dictionary:
**	params		Service parameters describing the Association
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

static FUNCTION testVector[] = {{checkDICOMVersion}, {checkCalledNode},
{checkCallingNode}, {checkApplicationConnection}};

CONDITION
associationCheck(char *node, DUL_ASSOCIATESERVICEPARAMETERS * params,
		 DMAN_HANDLE ** manageHandle, CTNBOOLEAN forgive,
		 DUL_ABORTITEMS * abortItems)
{
    CONDITION
	cond,
	rtnCond = APP_NORMAL;
    DMAN_APPLICATIONENTITY
	AEWork, AECriteria;
    long
        count;
    int
        index;


    cond = APP_NORMAL;
    for (index = 0;
	 index < DIM_OF(testVector) && CTN_SUCCESS(cond); index++) {
	cond = testVector[index].function(node, params, forgive, manageHandle,
					  abortItems);
	if (cond != APP_NORMAL) {
	    rtnCond = 0;
	    if (!CTN_FATAL(cond) && forgive) {
		cond = APP_NORMAL;
	    }
	}
    }

    if (rtnCond != APP_NORMAL) {
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
**	Check the validity of the Association service parameters
**
** Parameter Dictionary:
**	params		Service parameters describing the Association
**	forgive		Decides whether to return a warning or a failure status
**	abortItems	Structure specifying reasons for aborting the
**			Association
**
** Return Values:
**
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
**	Check the validity of the Association service parameters
**
** Parameter Dictionary:
**	params		Service parameters describing the Association
**	forgive		Decides whether to return a warning or a failure status
**	abortItems	Structure specifying reasons for aborting the
**			Association
**
** Return Values:
**
**	APP_NORMAL
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
**	Check the validity of the Association service parameters
**
** Parameter Dictionary:
**	params		Service parameters describing the Association
**	forgive		Decides whether to return a warning or a failure status
**	abortItems	Structure specifying reasons for aborting the
**			Association
**
** Return Values:
**
**	APP_NORMAL
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
**
**
** Parameter Dictionary:
**	params		Service parameters describing the Association
**	forgive		Decides whether to return a warning or a failure status
**	abortItems	Structure specifying reasons for aborting the
**			Association
**
** Return Values:
**
**	APP_NORMAL
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

CONDITION
requestAssociationHIS(DMAN_HANDLE ** dman, char *local,
		      char *application, DUL_NETWORKKEY ** network,
		      DUL_ASSOCIATIONKEY ** assoc,
		      DUL_ASSOCIATESERVICEPARAMETERS * params)
{
    CONDITION
	cond;
    char
        localHost[100];
    DMAN_APPLICATIONENTITY
	ae;
    static char *SOPClasses[] = {
	DICOM_SOPCLASSDETACHEDPATIENTMGMT,
	DICOM_SOPCLASSDETACHEDPATIENTMGMTMETA,
	DICOM_SOPCLASSDETACHEDVISITMGMT,
	DICOM_SOPCLASSDETACHEDSTUDYMGMT,
	DICOM_SOPCLASSSTUDYCOMPONENTMGMT,
	DICOM_SOPCLASSDETACHEDRESULTSMGMT,
	DICOM_SOPCLASSDETACHEDRESULTSMGMTMETA,
	DICOM_SOPCLASSDETACHEDSTUDYMGMTMETA,
	DICOM_SOPCLASSDETACHEDINTERPRETMGMT
    };
    int
        i;

    cond = DMAN_LookupApplication(dman, application, &ae);
    if (cond != DMAN_NORMAL)
	return 0;

    DUL_DefaultServiceParameters(params);
    sprintf(params->calledPresentationAddress, "%s:%-d", ae.Node, ae.Port);
    strcpy(params->calledAPTitle, ae.Title);
    strcpy(params->callingAPTitle, local);
    (void) gethostname(localHost, sizeof(localHost));
    strcpy(params->callingPresentationAddress, localHost);

    for (i = 0; i < (int) DIM_OF(SOPClasses); i++) {
	cond = SRV_RequestServiceClass(SOPClasses[i], DUL_SC_ROLE_DEFAULT,
				       params);
	if (cond != SRV_NORMAL)
	    return 0;
    }

    cond = DUL_RequestAssociation(network, params, assoc);
    if (cond != DUL_NORMAL)
	return 0;

    return 1;
}

CONDITION
releaseAssociation(DUL_ASSOCIATIONKEY ** assoc,
		   DUL_ASSOCIATESERVICEPARAMETERS * params)
{
    CONDITION
	cond;

    cond = DUL_ReleaseAssociation(assoc);
    if (cond != DUL_RELEASECONFIRMED)
	(void) DUL_AbortAssociation(assoc);

    (void) DUL_DropAssociation(assoc);
    (void) DUL_ClearServiceParameters(params);

    return 1;
}
