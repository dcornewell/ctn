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
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	FIS_SendEvent
**			FIS_SendSet
** Author, Date:	Stephen Moore, 22-Jul-94
** Intent:		This file contains functions for
**			information systems (management classes).
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:52:04 $
** Source File:		$RCSfile: event.c,v $
** Revision:		$Revision: 1.12 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.12 $ $RCSfile: event.c,v $";

#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "dicom_objects.h"
#include "tbl.h"
#include "manage.h"
#include "fis.h"
#include "dicom_uids.h"
#include "dulprotocol.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "utility.h"

#include "fis_private.h"
extern CTNBOOLEAN __FIS_verbose__;

static CONDITION
requestAssociation(DMAN_HANDLE ** dman, char *local,
		   char *application, FIS_DATA_TYPE type, DUL_SC_ROLE role,
      DUL_ASSOCIATIONKEY ** assoc, DUL_ASSOCIATESERVICEPARAMETERS * params);
static void
releaseAssociation(DUL_ASSOCIATIONKEY ** assoc,
		   DUL_ASSOCIATESERVICEPARAMETERS * params);
static CONDITION
sendEvent(FIS_DATA_TYPE type, void *d, DCM_OBJECT ** obj, FIS_EVENT event,
      DUL_ASSOCIATIONKEY ** assoc, DUL_ASSOCIATESERVICEPARAMETERS * params);
static CONDITION
sendSet(FIS_DATA_TYPE type, void *d, DCM_OBJECT ** obj,
      DUL_ASSOCIATIONKEY ** assoc, DUL_ASSOCIATESERVICEPARAMETERS * params);

/* FIS_SendEvent
**
** Purpose:
**	Describe the purpose of the function
**
** Parameter Dictionary:
**	Define the parameters to the function
**
** Return Values:
**	FIS_NORMAL
**	FIS_SENDEVENTFAILED
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
FIS_SendEvent(FIS_HANDLE ** fis, DMAN_HANDLE ** dman,
	      FIS_DATA_TYPE type, void *d, FIS_EVENT event,
	      char *localApplication, char *dst)
{
    CONDITION
	cond = FIS_NORMAL;
    DUL_ASSOCIATIONKEY
	* assoc;
    DUL_ASSOCIATESERVICEPARAMETERS
	params;
    DCM_OBJECT
	* obj;
    char application[20];
    int i;

    if (dst == NULL)
	return FIS_NORMAL;

    cond = DCM_CreateObject(&obj, 0);
    if (cond != DCM_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_SENDEVENTFAILED), dst,
				  "FIS_SendEvent");
    cond = FIS_BuildObject(&obj, type, d, event);
    if (cond != FIS_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_SENDEVENTFAILED), dst,
				  "FIS_SendEvent");

    while (dst[0] != '\0') {
	(void) sscanf(dst, "%[^:]", application);
	cond = requestAssociation(dman, localApplication, application, type,
				  DUL_SC_ROLE_SCP, &assoc, &params);
	if (cond != FIS_NORMAL)
	    return COND_PushCondition(FIS_COND(FIS_SENDEVENTFAILED), dst,
				      "FIS_SendEvent");
	cond = sendEvent(type, d, &obj, event, &assoc, &params);
	releaseAssociation(&assoc, &params);
	if (cond != FIS_NORMAL)
	    return COND_PushCondition(FIS_COND(FIS_SENDEVENTFAILED), dst,
				      "FIS_SendEvent");

	while (*dst != '\0' && *dst != ':')
	    dst++;
    }

    return FIS_NORMAL;
}

CONDITION
FIS_SendSet(FIS_HANDLE ** fis, DMAN_HANDLE ** dman,
	    FIS_DATA_TYPE type, void *d,
	    char *localApplication, char *dst)
{
    CONDITION
	cond = FIS_NORMAL;
    DUL_ASSOCIATIONKEY
	* assoc;
    DUL_ASSOCIATESERVICEPARAMETERS
	params;
    DCM_OBJECT
	* obj;
    char application[20];
    int i;

    if (dst == NULL)
	return FIS_NORMAL;

    cond = DCM_CreateObject(&obj, 0);
    if (cond != DCM_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_SENDSETFAILED), dst,
				  "FIS_SendSet");
    cond = FIS_BuildObject(&obj, type, d, FIS_K_NO_EVENT);
    if (cond != FIS_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_SENDSETFAILED), dst,
				  "FIS_SendSet");

    while (dst[0] != '\0') {
	(void) sscanf(dst, "%[^:]", application);
	cond = requestAssociation(dman, localApplication, application, type,
				  DUL_SC_ROLE_SCU, &assoc, &params);
	if (cond != FIS_NORMAL)
	    return COND_PushCondition(FIS_COND(FIS_SENDSETFAILED), dst,
				      "FIS_SendSet");
	cond = sendSet(type, d, &obj, &assoc, &params);
	releaseAssociation(&assoc, &params);
	if (cond != FIS_NORMAL)
	    return COND_PushCondition(FIS_COND(FIS_SENDSETFAILED), dst,
				      "FIS_SendSet");

	while (*dst != '\0' && *dst != ':')
	    dst++;
    }

    return FIS_NORMAL;
}


/* --------------------------------------------------
**	private functions
*/

static CONDITION
requestAssociation(DMAN_HANDLE ** dman, char *local,
		   char *application, FIS_DATA_TYPE type, DUL_SC_ROLE role,
       DUL_ASSOCIATIONKEY ** assoc, DUL_ASSOCIATESERVICEPARAMETERS * params)
{
    CONDITION
    cond;
    static CTNBOOLEAN
        first = TRUE;
    static DUL_NETWORKKEY
    *   network;
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

    if (first) {
	first = FALSE;
	cond = DUL_InitializeNetwork(DUL_NETWORK_TCP, DUL_AEREQUESTOR,
			   NULL, DUL_TIMEOUT, DUL_ORDERBIGENDIAN, &network);
	if (cond != DUL_NORMAL)
	    return 0;
    }
    DUL_DefaultServiceParameters(params);
    sprintf(params->calledPresentationAddress, "%s:%-d", ae.Node, ae.Port);
    strcpy(params->calledAPTitle, ae.Title);
    strcpy(params->callingAPTitle, local);
    (void) gethostname(localHost, sizeof(localHost));
    strcpy(params->callingPresentationAddress, localHost);

    for (i = 0; i < (int) DIM_OF(SOPClasses); i++) {
	cond = SRV_RequestServiceClass(SOPClasses[i], role, params);
	if (cond != SRV_NORMAL)
	    return 0;
    }

    cond = DUL_RequestAssociation(&network, params, assoc);
    if (cond != DUL_NORMAL)
	return 0;

    return FIS_NORMAL;
}

static void
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
}

typedef struct {
    FIS_DATA_TYPE Type;
    FIS_EVENT Event;
    int ID;
}   EVENT_ID_MAP;

static EVENT_ID_MAP IDMap[] = {
    {FIS_K_PATIENT, FIS_K_PATIENT_CREATED, 1},
    {FIS_K_PATIENT, FIS_K_PATIENT_DELETED, 2},
    {FIS_K_PATIENT, FIS_K_PATIENT_UPDATED, 3},
    {FIS_K_STUDY, FIS_K_STUDY_SCHEDULED, 2},
    {FIS_K_STUDY, FIS_K_STUDY_READ, 7},
    {FIS_K_RESULTS, FIS_K_RESULTS_CREATED, 1},
    {FIS_K_INTERPRETATION, FIS_K_INTERPRETATION_CREATED, 1},
    {FIS_K_INTERPRETATION, FIS_K_INTERPRETATION_APPROVED, 4}
};

static CONDITION
sendEventReportCB(MSG_N_EVENT_REPORT_REQ * request,
		  MSG_N_EVENT_REPORT_RESP * response, void *ctx)
{
    return SRV_NORMAL;
}

static CONDITION
sendEvent(FIS_DATA_TYPE type, void *d, DCM_OBJECT ** obj,
	  FIS_EVENT event, DUL_ASSOCIATIONKEY ** assoc,
	  DUL_ASSOCIATESERVICEPARAMETERS * params)
{
    CONDITION
    cond = FIS_NORMAL;
    MSG_N_EVENT_REPORT_REQ
	request;
    MSG_N_EVENT_REPORT_RESP
	response;
    int
        i;
    FIS_STUDYRECORD
	* study;
    FIS_PATIENTRECORD
	* patient;
    FIS_RESULTSRECORD *results = NULL;
    FIS_INTERPRETATIONRECORD *interp = NULL;

    request.type = MSG_K_N_EVENT_REPORT_REQ;
    request.messageID = SRV_MessageIDOut();
    request.eventTypeID = 0;
    for (i = 0; i < (int) DIM_OF(IDMap); i++) {
	if (type == IDMap[i].Type && event == IDMap[i].Event) {
	    request.eventTypeID = IDMap[i].ID;
	    break;
	}
    }
    if (request.eventTypeID == 0)
	return 0;

    switch (type) {
    case FIS_K_PATIENT:
	patient = (FIS_PATIENTRECORD *) d;
	strcpy(request.classUID, DICOM_SOPCLASSDETACHEDPATIENTMGMT);
	strcpy(request.affectedInstanceUID, patient->PatUID);
	break;
    case FIS_K_VISIT:
	break;
    case FIS_K_STUDY:
	study = (FIS_STUDYRECORD *) d;
	strcpy(request.classUID, DICOM_SOPCLASSDETACHEDSTUDYMGMT);
	strcpy(request.affectedInstanceUID, study->StuInsUID);
	break;
    case FIS_K_RESULTS:
	results = (FIS_RESULTSRECORD *) d;
	strcpy(request.classUID, DICOM_SOPCLASSDETACHEDRESULTSMGMT);
	strcpy(request.affectedInstanceUID, results->ResUID);
	break;
    case FIS_K_INTERPRETATION:
	interp = (FIS_INTERPRETATIONRECORD *) d;
	strcpy(request.classUID, DICOM_SOPCLASSDETACHEDINTERPRETMGMT);
	strcpy(request.affectedInstanceUID, interp->IntUID);
	break;
    }
    if (obj == NULL) {
	request.dataSet = NULL;
	request.dataSetType = DCM_CMDDATANULL;
    } else {
	request.dataSet = *obj;
	request.dataSetType = DCM_CMDDATAOTHER;
    }

    if (__FIS_verbose__)
	MSG_DumpMessage(&request, stdout);

    cond = SRV_NEventReportRequest(assoc, params, &request, &response,
				   sendEventReportCB, NULL, "");
    if (cond != SRV_NORMAL)
	return 0;

    return FIS_NORMAL;
}

static CONDITION
sendSetCB(MSG_N_SET_REQ * request, MSG_N_SET_RESP * response, void *ctx)
{
    return SRV_NORMAL;
}

static CONDITION
sendSet(FIS_DATA_TYPE type, void *d, DCM_OBJECT ** obj,
	DUL_ASSOCIATIONKEY ** assoc,
	DUL_ASSOCIATESERVICEPARAMETERS * params)
{
    CONDITION cond = FIS_NORMAL;
    MSG_N_SET_REQ request;
    MSG_N_SET_RESP response;
    int i;
    FIS_STUDYRECORD *study;
    FIS_PATIENTRECORD *patient;

    memset(&request, 0, sizeof(request));
    request.type = MSG_K_N_SET_REQ;
    request.messageID = SRV_MessageIDOut();

    switch (type) {
    case FIS_K_PATIENT:
	patient = (FIS_PATIENTRECORD *) d;
	strcpy(request.classUID, DICOM_SOPCLASSDETACHEDPATIENTMGMT);
	strcpy(request.instanceUID, patient->PatUID);
	break;
    case FIS_K_VISIT:
	break;
    case FIS_K_STUDY:
	study = (FIS_STUDYRECORD *) d;
	strcpy(request.classUID, DICOM_SOPCLASSDETACHEDSTUDYMGMT);
	strcpy(request.instanceUID, study->StuInsUID);
	break;
    case FIS_K_RESULTS:
	break;
    case FIS_K_INTERPRETATION:
	break;
    }
    if (obj == NULL) {
	request.dataSet = NULL;
	request.dataSetType = DCM_CMDDATANULL;
    } else {
	request.dataSet = *obj;
	request.dataSetType = DCM_CMDDATAOTHER;
    }

    cond = SRV_NSetRequest(assoc, params, request.classUID, &request, &response,
			   sendSetCB, NULL, "");
    if (cond != SRV_NORMAL)
	return 0;

    return FIS_NORMAL;
}
