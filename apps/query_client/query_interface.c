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
+-+-+-+-+-+-+-+-+-
*/
/*
**				DICOM 93
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:
** Intent:
** Last Update:		$Author: smm $, $Date: 2002-02-11 16:38:04 $
** Source File:		$Source: /home/smm/ctn/ctn/apps/query_client/query_interface.c,v $
** Revision:		$Revision: 1.13 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.13 $ $RCSfile: query_interface.c,v $";

#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef _MSC_VER
#include <io.h>
#include <fcntl.h>
#include <winsock.h>
#endif
#endif

#include "dicom.h"
#include "dicom_uids.h"
#include "lst.h"
#include "condition.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"

#include "query_interface.h"
#include "format.h"

static DUL_NETWORKKEY		/* Used to initialize our network */
*   network = NULL;
static DUL_ASSOCIATIONKEY	/* Describes the Association with the
				 * Acceptor */
*   association = NULL;

static DUL_ASSOCIATESERVICEPARAMETERS	/* The items which describe this
					 * Association */
    params = {
    DICOM_STDAPPLICATIONCONTEXT, "Calling AP Title", "DICOM_STORAGE",
    "", 16384, 0, 0, 0,
    "calling presentation addr", "called presentation addr",
    NULL, NULL, 0, 0,
    MIR_IMPLEMENTATIONCLASSUID, MIR_IMPLEMENTATIONVERSIONNAME,
    "", ""
};
static int
requestAssociation(DUL_NETWORKKEY ** network,
		   DUL_ASSOCIATIONKEY ** association,
		   DUL_ASSOCIATESERVICEPARAMETERS * params,
		   char *SOPClass);
static CONDITION
sendPatientQuery(DUL_ASSOCIATIONKEY ** association,
		 DUL_ASSOCIATESERVICEPARAMETERS * params,
		 PATIENT_QUERY_MODULE * query, LST_HEAD ** patientList);
static CONDITION
buildPatientQuery(PATIENT_QUERY_MODULE * query,
		  DCM_OBJECT ** queryObject);
static CONDITION
queryCallback(MSG_C_FIND_REQ * findRequest,
	      MSG_C_FIND_RESP * findResponse, int responseCount,
/*	      char *SOPClass, char *queryLevel, LST_HEAD ** lst); */
	      char *SOPClass, char *queryLevel, void *lst);

static CONDITION
sendStudyQuery(DUL_ASSOCIATIONKEY ** association,
	       DUL_ASSOCIATESERVICEPARAMETERS * params,
	       PATIENT_QUERY_MODULE * query, LST_HEAD ** studyList);
static CONDITION
buildStudyQuery(PATIENT_QUERY_MODULE * query,
		DCM_OBJECT ** queryObject);
static CONDITION
queryStudyCallback(MSG_C_FIND_REQ * findRequest,
		   MSG_C_FIND_RESP * findResponse, int responseCount,
/*		   char *SOPClass, char *queryLevel, LST_HEAD ** lst); */
		   char *SOPClass, char *queryLevel, void *lst);

static CONDITION
sendSeriesQuery(DUL_ASSOCIATIONKEY ** association,
		DUL_ASSOCIATESERVICEPARAMETERS * params,
       PATIENT_QUERY_MODULE * patientQuery, STUDY_QUERY_MODULE * studyQuery,
		LST_HEAD ** seriesList);
static CONDITION
buildSeriesQuery(PATIENT_QUERY_MODULE * patientModule,
		 STUDY_QUERY_MODULE * studyModule,
		 DCM_OBJECT ** queryObject);
static CONDITION
querySeriesCallback(MSG_C_FIND_REQ * findRequest,
		    MSG_C_FIND_RESP * findResponse, int responseCount,
/*		    char *SOPClass, char *queryLevel, LST_HEAD ** lst); */
		    char *SOPClass, char *queryLevel, void *lst);

static CONDITION
sendMoveRequest(DUL_ASSOCIATIONKEY ** association,
	  DUL_ASSOCIATESERVICEPARAMETERS * params, char *destinationAPTitle,
		PATIENT_QUERY_MODULE * patient, STUDY_QUERY_MODULE * study);
static CONDITION moveCallback();
static CONDITION
buildMoveQuery(PATIENT_QUERY_MODULE * patientModule,
	       STUDY_QUERY_MODULE * studyModule, DCM_OBJECT ** queryObject);

static void upperCase(char *s);
static void listClear(LST_HEAD ** l);

CONDITION
patientQuery(CONNECTION_MODULE * connection,
	     PATIENT_QUERY_MODULE * query, LST_HEAD ** patientList)
{
    char
        localHost[80];
    CONDITION
	cond;

    if (network == NULL) {
	cond = DUL_InitializeNetwork(DUL_NETWORK_TCP, DUL_AEREQUESTOR,
			   NULL, DUL_TIMEOUT, DUL_ORDERBIGENDIAN, &network);
	if (cond != DUL_NORMAL)
	    return 0;
    }
    (void) gethostname(localHost, sizeof(localHost));
    sprintf(params.calledPresentationAddress, "%s:%s", connection->NodeName,
	    connection->PortNumber);
    strcpy(params.callingPresentationAddress, localHost);
    strcpy(params.calledAPTitle, connection->CalledAPTitle);
    strcpy(params.callingAPTitle, connection->CallingAPTitle);
    if (requestAssociation(&network, &association, &params,
			   DICOM_SOPPATIENTQUERY_FIND) == 0) {
	return 0;
    }
    cond = sendPatientQuery(&association, &params, query, patientList);

    (void) DUL_ReleaseAssociation(&association);
    (void) DUL_DropAssociation(&association);

    return 1;
}


/* requestAssociation
**
** Purpose:
**      This subroutine implements association request
**
** Parameter Dictionary:
**      networkKey	the handle key of the network
**      assoc		the association key of the communication
**      assocParams	the parameters for the association request
**      SOPClass	the SOP Class for specifying the association reqeust
**
** Return Values:
**      0               association request failed
**      1               association request successed
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/

static int
requestAssociation(DUL_NETWORKKEY ** networkKey,
		   DUL_ASSOCIATIONKEY ** assoc,
		   DUL_ASSOCIATESERVICEPARAMETERS * assocParams,
		   char *SOPClass)
{
    CONDITION
    cond;

    cond = SRV_RequestServiceClass(SOPClass, DUL_SC_ROLE_SCU, assocParams);
    if (cond != SRV_NORMAL) {
	COND_DumpConditions();
	return 0;
    }
    cond = DUL_RequestAssociation(networkKey, assocParams, assoc);
    if (cond != DUL_NORMAL) {
	if (cond == DUL_ASSOCIATIONREJECTED) {
	    fprintf(stderr, "Association Rejected\n");
	    fprintf(stderr, " Result: %2x Source %2x Reason %2x\n",
		    assocParams->result, assocParams->resultSource,
		    assocParams->diagnostic);
	}
	return 0;
    }
    (void) printf("Association accepted, parameters:-------------\n");
    DUL_DumpParams(assocParams);

    return 1;
}

static CONDITION
sendPatientQuery(DUL_ASSOCIATIONKEY ** assoc,
		 DUL_ASSOCIATESERVICEPARAMETERS * assocParams,
		 PATIENT_QUERY_MODULE * query, LST_HEAD ** patientList)
{
    MSG_C_FIND_REQ
    request;
    MSG_C_FIND_RESP
	response;
    CONDITION
	cond;
    DCM_OBJECT
	* queryObject;

    cond = DCM_CreateObject(&queryObject, 0);

    cond = buildPatientQuery(query, &queryObject);

    request.type = MSG_K_C_FIND_REQ;
    request.dataSetType = DCM_CMDDATAIDENTIFIER;
    request.messageID = SRV_MessageIDOut();
    request.priority = 0;
    strcpy(request.classUID, DICOM_SOPPATIENTQUERY_FIND);
    request.identifier = queryObject;
    cond = SRV_CFindRequest(assoc, assocParams, &request,
			    &response, queryCallback, patientList, "");

    if (cond == SRV_OPERATIONCANCELLED)
	printf("Find operation cancelled\n");
    else if (cond != SRV_NORMAL)
	COND_DumpConditions();

    MSG_DumpMessage(&response, stdout);
    (void) DCM_CloseObject(&queryObject);
    return 1;
}



static PATIENT_QUERY_MODULE
    patient;
static DCM_ELEMENT patientTable[] = {
    {DCM_PATNAME, DCM_PN, "", 1, sizeof(patient.PatientName),
    (void *) patient.PatientName},
    {DCM_PATID, DCM_LO, "", 1, sizeof(patient.PatientID),
    (void *) patient.PatientID},
    {DCM_PATBIRTHDATE, DCM_DA, "", 1, sizeof(patient.BirthDate),
    (void *) patient.BirthDate}
};

static DCM_ELEMENT requiredPatientAttributes[] = {
    {DCM_PATNAME, DCM_PN, "", 1, sizeof(patient.PatientName),
    (void *) patient.PatientName},
    {DCM_PATID, DCM_LO, "", 1, sizeof(patient.PatientID),
    (void *) patient.PatientID}
};


static DCM_FLAGGED_ELEMENT optionalPatientAttributes[] = {
    {DCM_PATBIRTHDATE, DCM_DA, "", 1, sizeof(patient.BirthDate),
    (void *) patient.BirthDate, PATIENT_BIRTHDATE, &patient.OptionalFlags},

    {DCM_RELNUMBERPATRELATEDSTUDIES, DCM_IS, "", 1,
	sizeof(patient.NumberRelatedStudies),
	(void *) patient.NumberRelatedStudies,
    PATIENT_NUMBERRELATEDSTUDIES, &patient.OptionalFlags},

    {DCM_RELNUMBERPATRELATEDSERIES, DCM_IS, "", 1,
	sizeof(patient.NumberRelatedSeries),
	(void *) patient.NumberRelatedSeries,
    PATIENT_NUMBERRELATEDSERIES, &patient.OptionalFlags},

    {DCM_RELNUMBERPATRELATEDIMAGES, DCM_IS, "", 1,
	sizeof(patient.NumberRelatedImages),
	(void *) patient.NumberRelatedImages,
    PATIENT_NUMBERRELATEDIMAGES, &patient.OptionalFlags}
};

static CONDITION
buildPatientQuery(PATIENT_QUERY_MODULE * query,
		  DCM_OBJECT ** queryObject)
{
    static char
        queryLevel[] = DCM_QUERYLEVELPATIENT;
    static DCM_ELEMENT
        e = {DCM_IDQUERYLEVEL, DCM_CS, "", 1, sizeof(queryLevel),
    (void *) queryLevel};

    CONDITION
	cond;

    patient = *query;
    upperCase(patient.PatientID);

    strcpy(patient.PatientName, patient.LastName);
    upperCase(patient.PatientName);
    strcat(patient.PatientName, "*");

    cond = DCM_ModifyElements(queryObject, patientTable,
			      (int) DIM_OF(patientTable), NULL, 0, NULL);
    if (cond != DCM_NORMAL)
	return 0;

    cond = DCM_ModifyElements(queryObject, &e, 1, NULL, 0, NULL);
    if (cond != DCM_NORMAL)
	return 0;

    return 1;
}

static CONDITION
queryCallback(MSG_C_FIND_REQ * findRequest,
	      MSG_C_FIND_RESP * findResponse, int responseCount,
/*	      char *SOPClass, char *queryLevel, LST_HEAD ** lst) */
	      char *SOPClass, char *queryLevel, void *lstPtr)
{
    static PATIENT_QUERY_MODULE
    *   p;
    CONDITION
	cond;
/*  Definition and cast assignment below help satisfy prototype for c-find
**  callback defined in dicom_services.h.
*/
    LST_HEAD **lst;
    lst = (LST_HEAD **) lstPtr;

    if (findResponse->status == MSG_K_C_FIND_MATCHCONTINUING ||
	findResponse->status == MSG_K_C_FIND_MATCHCONTINUINGWARN) {
	memset(&patient, 0, sizeof(patient));
	cond = DCM_ParseObject(&findResponse->identifier,
			       requiredPatientAttributes,
			       (int) DIM_OF(requiredPatientAttributes),
			       optionalPatientAttributes,
			     (int) DIM_OF(optionalPatientAttributes), NULL);
	if (cond != DCM_NORMAL)
	    return 0;

	if ((patient.OptionalFlags & PATIENT_BIRTHDATE) == 0)
	    patient.BirthDate[0] = '\0';

	p = malloc(sizeof(*p));
	if (p == NULL)
	    return 0;

	*p = patient;
	(void) LST_Enqueue(lst, p);
    }
    printf("Query Response: %5d\n", responseCount);
    printf("SOP Class:   %s\n", SOPClass);
    printf("Query Level: %s\n", queryLevel);

    MSG_DumpMessage(findResponse, stdout);
    return SRV_NORMAL;
}

CONDITION
studyQuery(CONNECTION_MODULE * connection,
	   PATIENT_QUERY_MODULE * query, LST_HEAD ** studyList)
{
#ifdef SMM
    DUL_ASSOCIATIONKEY		/* Describes the Association with the
				 * Acceptor */
	* association = NULL;

    DUL_ASSOCIATESERVICEPARAMETERS	/* The items which describe this
					 * Association */
	params = {
	DICOM_STDAPPLICATIONCONTEXT, "Calling AP Title", "DICOM_STORAGE",
	    "", 16384, 0, 0, 0,
	    "calling presentation addr", "called presentation addr",
	    NULL, NULL, 0, 0,
	    MIR_IMPLEMENTATIONCLASSUID, MIR_IMPLEMENTATIONVERSIONNAME,
	    "", ""
    };
#endif
    char
        localHost[80];
    CONDITION
	cond;
    int
        i;
    STUDY_QUERY_MODULE
	* studyModule;
    SERIES_QUERY_MODULE
	* seriesModule;
    LST_HEAD
	* seriesList;

    if (network == NULL) {
	cond = DUL_InitializeNetwork(DUL_NETWORK_TCP, DUL_AEREQUESTOR,
			   NULL, DUL_TIMEOUT, DUL_ORDERBIGENDIAN, &network);
	if (cond != DUL_NORMAL)
	    return 0;
    }
    (void) gethostname(localHost, sizeof(localHost));
    sprintf(params.calledPresentationAddress, "%s:%s", connection->NodeName,
	    connection->PortNumber);
    strcpy(params.callingPresentationAddress, localHost);
    strcpy(params.calledAPTitle, connection->CalledAPTitle);
    strcpy(params.callingAPTitle, connection->CallingAPTitle);
    if (requestAssociation(&network, &association, &params,
			   DICOM_SOPPATIENTQUERY_FIND) == 0) {
	return 0;
    }
    cond = sendStudyQuery(&association, &params, query, studyList);

    studyModule = LST_Head(studyList);
    (void) LST_Position(studyList, studyModule);

    seriesList = LST_Create();
    if (seriesList == NULL)
	return 0;

    while (studyModule != NULL) {
	cond = sendSeriesQuery(&association, &params, query, studyModule,
			       &seriesList);

	seriesModule = LST_Head(&seriesList);
	(void) LST_Position(&seriesList, seriesModule);
	for (i = 0; i < 10 && seriesModule != NULL; i++) {
	    strcpy(studyModule->SeriesModalities[i], seriesModule->Modality);

	    seriesModule = LST_Next(&seriesList);
	}

	studyModule = LST_Next(studyList);
	listClear(&seriesList);
    }

    (void) LST_Destroy(&seriesList);
    (void) DUL_ReleaseAssociation(&association);
    (void) DUL_DropAssociation(&association);

    return 1;
}

static CONDITION
sendStudyQuery(DUL_ASSOCIATIONKEY ** assoc,
	       DUL_ASSOCIATESERVICEPARAMETERS * assocParams,
	       PATIENT_QUERY_MODULE * query, LST_HEAD ** studyList)
{
    MSG_C_FIND_REQ
    request;
    MSG_C_FIND_RESP
	response;
    CONDITION
	cond;
    DCM_OBJECT
	* queryObject;

    cond = DCM_CreateObject(&queryObject, 0);

    cond = buildStudyQuery(query, &queryObject);

    request.type = MSG_K_C_FIND_REQ;
    request.dataSetType = DCM_CMDDATAIDENTIFIER;
    request.messageID = SRV_MessageIDOut();
    request.priority = 0;
    strcpy(request.classUID, DICOM_SOPPATIENTQUERY_FIND);
    request.identifier = queryObject;
    cond = SRV_CFindRequest(assoc, assocParams, &request,
			    &response, queryStudyCallback, studyList, "");

    if (cond == SRV_OPERATIONCANCELLED)
	printf("Find operation cancelled\n");
    else if (cond != SRV_NORMAL)
	COND_DumpConditions();

    MSG_DumpMessage(&response, stdout);
    (void) DCM_CloseObject(&queryObject);
    return 1;
}

static STUDY_QUERY_MODULE
    study;
static DCM_ELEMENT studyTable[] = {
    {DCM_IDSTUDYDATE, DCM_DA, "", 1, sizeof(study.StudyDate),
    (void *) study.StudyDate},
    {DCM_IDACCESSIONNUMBER, DCM_SH, "", 1, sizeof(study.AccessionNumber),
    (void *) study.AccessionNumber},
    {DCM_RELSTUDYID, DCM_SH, "", 1, sizeof(study.StudyID),
    (void *) study.StudyID},
    {DCM_RELSTUDYINSTANCEUID, DCM_UI, "", 1,
    sizeof(study.StudyInstanceUID), (void *) study.StudyInstanceUID}
};

static CONDITION
buildStudyQuery(PATIENT_QUERY_MODULE * query,
		DCM_OBJECT ** queryObject)
{
    static char
        queryLevel[] = DCM_QUERYLEVELSTUDY;
    static DCM_ELEMENT
        e = {DCM_IDQUERYLEVEL, DCM_CS, "", 1, sizeof(queryLevel),
    (void *) queryLevel};
    static DCM_ELEMENT patientIDElement =
    {DCM_PATID, DCM_LO, "", 1, sizeof(patient.PatientID),
    (void *) patient.PatientID};

    CONDITION
	cond;


    patient = *query;
    memset(&study, 0, sizeof(study));

    cond = DCM_ModifyElements(queryObject, studyTable,
			      (int) DIM_OF(studyTable), NULL, 0, NULL);

    cond = DCM_ModifyElements(queryObject, &e, 1, NULL, 0, NULL);
    if (cond != DCM_NORMAL)
	return 0;

    cond = DCM_ModifyElements(queryObject, &patientIDElement, 1, NULL, 0, NULL);
    if (cond != DCM_NORMAL)
	return 0;

    return 1;
}

static CONDITION
queryStudyCallback(MSG_C_FIND_REQ * findRequest,
		   MSG_C_FIND_RESP * findResponse, int responseCount,
/*		   char *SOPClass, char *queryLevel, LST_HEAD ** lst) */
		   char *SOPClass, char *queryLevel, void *lstPtr)
{
    static STUDY_QUERY_MODULE
    *   s;
    CONDITION
	cond;
/*  Definition and cast assignment help satisfy prototype for c-find callback
**  defined in dicom_services.h.
*/
    LST_HEAD **lst;
    lst = (LST_HEAD **) lstPtr;

    if (findResponse->status == MSG_K_C_FIND_MATCHCONTINUING ||
	findResponse->status == MSG_K_C_FIND_MATCHCONTINUINGWARN) {
	cond = DCM_ParseObject(&findResponse->identifier, studyTable,
			       (int) DIM_OF(studyTable), NULL, 0, NULL);
	if (cond != DCM_NORMAL)
	    return 0;

	s = malloc(sizeof(*s));
	if (s == NULL)
	    return 0;

	*s = study;
	memset(s->SeriesModalities, 0, sizeof(s->SeriesModalities));

	(void) LST_Enqueue(lst, s);
    }
    printf("Query Response: %5d\n", responseCount);
    printf("SOP Class:   %s\n", SOPClass);
    printf("Query Level: %s\n", queryLevel);

    MSG_DumpMessage(findResponse, stdout);
    return SRV_NORMAL;
}

static CONDITION
sendSeriesQuery(DUL_ASSOCIATIONKEY ** assoc,
		DUL_ASSOCIATESERVICEPARAMETERS * assocParams,
		PATIENT_QUERY_MODULE * patientQueryModule, STUDY_QUERY_MODULE * studyQueryModule,
		LST_HEAD ** seriesList)
{
    MSG_C_FIND_REQ
    request;
    MSG_C_FIND_RESP
	response;
    CONDITION
	cond;
    DCM_OBJECT
	* queryObject;

    cond = DCM_CreateObject(&queryObject, 0);
    if (cond != DCM_NORMAL)
	(void) COND_DumpConditions;

    cond = buildSeriesQuery(patientQueryModule, studyQueryModule, &queryObject);
    if (cond != 1)
	(void) COND_DumpConditions;

    request.type = MSG_K_C_FIND_REQ;
    request.dataSetType = DCM_CMDDATAIDENTIFIER;
    request.messageID = SRV_MessageIDOut();
    request.priority = 0;
    strcpy(request.classUID, DICOM_SOPPATIENTQUERY_FIND);
    request.identifier = queryObject;
    cond = SRV_CFindRequest(assoc, assocParams, &request,
			    &response, querySeriesCallback, seriesList, "");

    if (cond == SRV_OPERATIONCANCELLED)
	printf("Find operation cancelled\n");
    else if (cond != SRV_NORMAL)
	COND_DumpConditions();

    MSG_DumpMessage(&response, stdout);
    (void) DCM_CloseObject(&queryObject);

    return 1;
}

static SERIES_QUERY_MODULE
    series;
static DCM_ELEMENT seriesTable[] = {
    {DCM_RELSERIESINSTANCEUID, DCM_UI, "", 1,
    sizeof(series.SeriesInstanceUID), (void *) series.SeriesInstanceUID},
    {DCM_IDMODALITY, DCM_CS, "", 1, sizeof(series.Modality),
    (void *) series.Modality}
};

static CONDITION
buildSeriesQuery(PATIENT_QUERY_MODULE * patientModule,
		 STUDY_QUERY_MODULE * studyModule,
		 DCM_OBJECT ** queryObject)
{
    static char
        queryLevel[] = DCM_QUERYLEVELSERIES;
    static DCM_ELEMENT
        e = {DCM_IDQUERYLEVEL, DCM_CS, "", 1, sizeof(queryLevel),
    (void *) queryLevel};
    static DCM_ELEMENT IDElements[] = {
	{DCM_PATID, DCM_LO, "", 1, sizeof(patient.PatientID),
	(void *) patient.PatientID},
	{DCM_RELSTUDYINSTANCEUID, DCM_UI, "", 1,
	sizeof(study.StudyInstanceUID), (void *) study.StudyInstanceUID}
    };

    CONDITION
	cond;


    strcpy(patient.PatientID, patientModule->PatientID);
    strcpy(study.StudyInstanceUID, studyModule->StudyInstanceUID);
    memset(&series, 0, sizeof(series));

    cond = DCM_ModifyElements(queryObject, seriesTable,
			      (int) DIM_OF(seriesTable), NULL, 0, NULL);
    if (cond != DCM_NORMAL)
	return 0;

    cond = DCM_ModifyElements(queryObject, IDElements, (int) DIM_OF(IDElements),
			      NULL, 0, NULL);
    if (cond != DCM_NORMAL)
	return 0;

    cond = DCM_ModifyElements(queryObject, &e, 1, NULL, 0, NULL);
    if (cond != DCM_NORMAL)
	return 0;

    (void) DCM_DumpElements(queryObject, 0);

    return 1;
}

static CONDITION
querySeriesCallback(MSG_C_FIND_REQ * findRequest,
		    MSG_C_FIND_RESP * findResponse, int responseCount,
/*		    char *SOPClass, char *queryLevel, LST_HEAD ** lst) */
		    char *SOPClass, char *queryLevel, void *lstPtr)
{
    static SERIES_QUERY_MODULE
    *   s;
    CONDITION
	cond;
/*  Definition and cast assignment help satisfy prototype for c-find callback
**  defined in dicom_services.h.
*/
    LST_HEAD **lst;
    lst = (LST_HEAD **) lstPtr;

    if (findResponse->status == MSG_K_C_FIND_MATCHCONTINUING ||
	findResponse->status == MSG_K_C_FIND_MATCHCONTINUINGWARN) {
	cond = DCM_ParseObject(&findResponse->identifier, seriesTable,
			       (int) DIM_OF(seriesTable), NULL, 0, NULL);
	if (cond != DCM_NORMAL)
	    return 0;

	s = malloc(sizeof(*s));
	if (s == NULL)
	    return 0;

	*s = series;

	(void) LST_Enqueue(lst, s);
    }
    printf("Query Response: %5d\n", responseCount);
    printf("SOP Class:   %s\n", SOPClass);
    printf("Query Level: %s\n", queryLevel);

    MSG_DumpMessage(findResponse, stdout);
    return SRV_NORMAL;
}


CONDITION
moveRequest(CONNECTION_MODULE * connection,
	    char *destinationAPTitle, PATIENT_QUERY_MODULE * patientModule,
	    STUDY_QUERY_MODULE * studyModule)
{
    char
        localHost[80];
    CONDITION
	cond;

    if (network == NULL) {
	cond = DUL_InitializeNetwork(DUL_NETWORK_TCP, DUL_AEREQUESTOR,
			   NULL, DUL_TIMEOUT, DUL_ORDERBIGENDIAN, &network);
	if (cond != DUL_NORMAL)
	    return 0;
    }
    (void) gethostname(localHost, sizeof(localHost));
    sprintf(params.calledPresentationAddress, "%s:%s", connection->NodeName,
	    connection->PortNumber);
    strcpy(params.callingPresentationAddress, localHost);
    strcpy(params.calledAPTitle, connection->CalledAPTitle);
    strcpy(params.callingAPTitle, connection->CallingAPTitle);
    if (requestAssociation(&network, &association, &params,
			   DICOM_SOPPATIENTQUERY_MOVE) == 0) {
	return 0;
    }
    cond = sendMoveRequest(&association, &params, destinationAPTitle,
			   patientModule, studyModule);

    (void) DUL_ReleaseAssociation(&association);
    (void) DUL_DropAssociation(&association);

    return 1;
}

static CONDITION
sendMoveRequest(DUL_ASSOCIATIONKEY ** assoc,
     DUL_ASSOCIATESERVICEPARAMETERS * assocParams, char *destinationAPTitle,
     PATIENT_QUERY_MODULE * patientModule, STUDY_QUERY_MODULE * studyModule)
{
    MSG_C_MOVE_REQ
    request;
    MSG_C_MOVE_RESP
	response;
    CONDITION
	cond;
    DCM_OBJECT
	* queryObject;

    cond = DCM_CreateObject(&queryObject, 0);

    cond = buildMoveQuery(patientModule, studyModule, &queryObject);

    request.type = MSG_K_C_MOVE_REQ;
    request.dataSetType = DCM_CMDDATAIDENTIFIER;
    request.messageID = SRV_MessageIDOut();
    request.priority = 0;
    strcpy(request.classUID, DICOM_SOPPATIENTQUERY_MOVE);
    strcpy(request.moveDestination, destinationAPTitle);
    request.identifier = queryObject;

    cond = SRV_CMoveRequest(assoc, assocParams, &request,
			    &response, moveCallback, NULL, "");

    if (cond == SRV_OPERATIONCANCELLED)
	printf("Find operation cancelled\n");
    else if (cond != SRV_NORMAL)
	COND_DumpConditions();

    MSG_DumpMessage(&response, stdout);
    (void) DCM_CloseObject(&queryObject);
    return 1;
}

static CONDITION
moveCallback()
{
    return SRV_NORMAL;
}

static CONDITION
buildMoveQuery(PATIENT_QUERY_MODULE * patientModule,
	       STUDY_QUERY_MODULE * studyModule, DCM_OBJECT ** queryObject)
{
    static char
        queryLevel[] = DCM_QUERYLEVELSTUDY;
    static DCM_ELEMENT e[] = {
	{DCM_IDQUERYLEVEL, DCM_CS, "", 1, sizeof(queryLevel),
	(void *) queryLevel},
	{DCM_PATID, DCM_LO, "", 1, sizeof(patient.PatientID),
	(void *) patient.PatientID},
	{DCM_RELSTUDYINSTANCEUID, DCM_UI, "", 1,
	sizeof(study.StudyInstanceUID), (void *) study.StudyInstanceUID}
    };

    CONDITION
	cond;


    patient = *patientModule;
    study = *studyModule;

    cond = DCM_ModifyElements(queryObject, e, (int) DIM_OF(e), NULL, 0, NULL);
    if (cond != DCM_NORMAL)
	return 0;

    return 1;
}

static void
upperCase(char *s)
{
    char
        c;

    while ((c = *s) != '\0') {
	if (isupper(c))
	    *s++ = c;
	else
	    s++;
    }
}

static void
listClear(LST_HEAD ** l)
{
    LST_NODE
    * node;

    while ((node = LST_Dequeue(l)) != NULL)
	free(node);
}
