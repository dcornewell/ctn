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
**                              DICOM 93
**                   Electronic Radiology Laboratory
**                 Mallinckrodt Institute of Radiology
**              Washington University School of Medicine
**
** Module Name(s):      processEvents
** Author, Date:        John O'Neill, Stephen Moore, 8-Sep-94
** Intent:              Handles DICOM NGET requests for HIS functions
**
** Last Update:         $Author: smm $, $Date: 2001-12-21 16:44:51 $
** Source File:         $RCSfile: procevent.c,v $
** Revision:            $Revision: 1.6 $
** Status:              $State: Exp $
*/
static char rcsid[] = "$Revision: 1.6 $ $RCSfile: procevent.c,v $";

#include "ctn_os.h"

#if 0
#include    <stdio.h>
#include    <stdlib.h>
#include    <sys/types.h>
#endif

#include "dicom.h"
#include "tbl.h"
#include "lst.h"
#include "dicom_uids.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "tbl.h"
#include "manage.h"
#include "fis.h"

#include "ris_gateway.h"
extern LST_HEAD *eventList;
extern int logLevel;


static CONDITION
eventCallback(MSG_N_GET_REQ * req, MSG_N_GET_RESP * resp,
	      void *ctx)
{
    return SRV_NORMAL;
}

static CONDITION
patientNGet(DUL_ASSOCIATIONKEY ** assoc,
  DUL_ASSOCIATESERVICEPARAMETERS * params, char *uid, FIS_PATIENTRECORD * p)
{
    MSG_N_GET_REQ request;
    MSG_N_GET_RESP response;
    CONDITION cond;

    request.type = MSG_K_N_GET_REQ;
    request.conditionalFields = 0;
    request.dataSetType = DCM_CMDDATANULL;
    strcpy(request.classUID, DICOM_SOPCLASSDETACHEDPATIENTMGMT);
    strcpy(request.requestedInstanceUID, uid);
    request.attributeList = NULL;
    request.attributeCount = 0;

    cond = SRV_NGetRequest(assoc, params, DICOM_SOPCLASSDETACHEDPATIENTMGMT,
			   &request, &response, eventCallback, p, "");

    if (cond == SRV_NORMAL && response.dataSetType != DCM_CMDDATANULL) {
	if (logLevel > 1) {
	    fprintf(stdout, "Just performed Patient N-Get: \n");
	    (void) DCM_DumpElements(&response.dataSet, 0);
	}
	cond = FIS_ParseObject(&response.dataSet, FIS_K_PATIENT, p);
	(void) DCM_CloseObject(&response.dataSet);
	if (cond != FIS_NORMAL)
	    return 0;
    } else
	return 0;

    return 1;
}

static CONDITION
studyNGet(DUL_ASSOCIATIONKEY ** assoc,
    DUL_ASSOCIATESERVICEPARAMETERS * params, char *uid, FIS_STUDYRECORD * s)
{
    MSG_N_GET_REQ request;
    MSG_N_GET_RESP response;
    CONDITION cond;

    request.type = MSG_K_N_GET_REQ;
    request.conditionalFields = 0;

    strcpy(request.classUID, DICOM_SOPCLASSDETACHEDSTUDYMGMT);
    strcpy(request.requestedInstanceUID, uid);
    request.attributeList = NULL;
    request.attributeCount = 0;

    cond = SRV_NGetRequest(assoc, params, DICOM_SOPCLASSDETACHEDSTUDYMGMT,
			   &request, &response, eventCallback, s, "");

    if (cond == SRV_NORMAL && response.dataSetType != DCM_CMDDATANULL) {
	if (logLevel > 1) {
	    fprintf(stdout, "Just performed Study N-Get: \n");
	    (void) DCM_DumpElements(&response.dataSet, 0);
	}
	cond = FIS_ParseObject(&response.dataSet, FIS_K_STUDY, s);
	(void) DCM_CloseObject(&response.dataSet);
	if (cond != FIS_NORMAL)
	    return 0;
    } else
	return 0;

    return 1;
}

static CONDITION
studyComponentNGet(DUL_ASSOCIATIONKEY ** assoc,
		   DUL_ASSOCIATESERVICEPARAMETERS * params, char *uid,
		   FIS_STUDYCOMPONENTRECORD * s)
{
    MSG_N_GET_REQ request;
    MSG_N_GET_RESP response;
    CONDITION cond;

    request.type = MSG_K_N_GET_REQ;
    request.conditionalFields = 0;

    strcpy(request.classUID, DICOM_SOPCLASSSTUDYCOMPONENTMGMT);
    strcpy(request.requestedInstanceUID, uid);
    request.attributeList = NULL;
    request.attributeCount = 0;

    cond = SRV_NGetRequest(assoc, params, DICOM_SOPCLASSSTUDYCOMPONENTMGMT,
			   &request, &response, eventCallback, s, "");

    if (cond == SRV_NORMAL && response.dataSetType != DCM_CMDDATANULL) {
	if (logLevel > 1) {
	    fprintf(stdout, "Just performed Study Component N-Get: \n");
	    (void) DCM_DumpElements(&response.dataSet, 0);
	}
	cond = FIS_ParseObject(&response.dataSet, FIS_K_STUDYCOMPONENT, s);
	(void) DCM_CloseObject(&response.dataSet);
	if (cond != FIS_NORMAL)
	    return 0;
    } else
	return 0;

    return 1;
}

static CONDITION
resultsNGet(DUL_ASSOCIATIONKEY ** assoc,
  DUL_ASSOCIATESERVICEPARAMETERS * params, char *uid, FIS_RESULTSRECORD * s)
{
    MSG_N_GET_REQ request;
    MSG_N_GET_RESP response;
    CONDITION cond;
    DCM_TAG attributeList[] = {DCM_RESREFERENCEDINTERPSEQ};

/*  This first Get request is for debugging */

    request.type = MSG_K_N_GET_REQ;
    request.conditionalFields = 0;
    request.dataSetType = DCM_CMDDATANULL;
    strcpy(request.classUID, DICOM_SOPCLASSDETACHEDRESULTSMGMT);
    strcpy(request.requestedInstanceUID, uid);
    request.attributeList = attributeList;
    request.attributeCount = DIM_OF(attributeList);

    cond = SRV_NGetRequest(assoc, params, DICOM_SOPCLASSDETACHEDRESULTSMGMT,
			   &request, &response, eventCallback, s, "");

    if (cond == SRV_NORMAL && response.dataSetType != DCM_CMDDATANULL) {
	if (logLevel > 1) {
	    fprintf(stdout, "Just performed Results Mgmt N-Get: \n");
	    (void) DCM_DumpElements(&response.dataSet, 0);
	}
    }
    request.type = MSG_K_N_GET_REQ;
    request.conditionalFields = 0;
    request.dataSetType = DCM_CMDDATANULL;
    strcpy(request.classUID, DICOM_SOPCLASSDETACHEDRESULTSMGMT);
    strcpy(request.requestedInstanceUID, uid);
    request.attributeList = NULL;
    request.attributeCount = 0;

    cond = SRV_NGetRequest(assoc, params, DICOM_SOPCLASSDETACHEDRESULTSMGMT,
			   &request, &response, eventCallback, s, "");

    if (cond == SRV_NORMAL && response.dataSetType != DCM_CMDDATANULL) {
	if (logLevel > 1) {
	    fprintf(stdout, "Just performed Results Mgmt N-Get: \n");
	    (void) DCM_DumpElements(&response.dataSet, 0);
	}
	cond = FIS_ParseObject(&response.dataSet, FIS_K_RESULTS, s);
	(void) DCM_CloseObject(&response.dataSet);
	if (cond != FIS_NORMAL)
	    return 0;
    } else
	return 0;

    return 1;
}

static CONDITION
interpretationNGet(DUL_ASSOCIATIONKEY ** assoc,
		   DUL_ASSOCIATESERVICEPARAMETERS * params, char *uid,
		   FIS_INTERPRETATIONRECORD * s)
{
    MSG_N_GET_REQ request;
    MSG_N_GET_RESP response;
    CONDITION cond;

    request.type = MSG_K_N_GET_REQ;
    request.conditionalFields = 0;
    request.dataSetType = DCM_CMDDATANULL;
    strcpy(request.classUID, DICOM_SOPCLASSDETACHEDINTERPRETMGMT);
    strcpy(request.requestedInstanceUID, uid);
    request.attributeList = NULL;
    request.attributeCount = 0;

    cond = SRV_NGetRequest(assoc, params, DICOM_SOPCLASSDETACHEDINTERPRETMGMT,
			   &request, &response, eventCallback, s, "");

    if (cond == SRV_NORMAL && response.dataSetType != DCM_CMDDATANULL) {
	if (logLevel > 1) {
	    fprintf(stdout, "Just performed Interpretation Mgmt N-Get: \n");
	    (void) DCM_DumpElements(&response.dataSet, 0);
	}
	cond = FIS_ParseObject(&response.dataSet, FIS_K_INTERPRETATION, s);
	(void) DCM_CloseObject(&response.dataSet);
	if (cond != FIS_NORMAL)
	    return 0;
    } else
	return 0;

    return 1;
}


static CONDITION
insertStudyComponent(FIS_HANDLE ** f,
		     FIS_STUDYCOMPONENTRECORD * sc)
{
    CONDITION cond;
    FIS_SCSERIESRECORD *series;
    FIS_SCIMAGERECORD *image;

    cond = FIS_Insert(f, FIS_K_STUDYCOMPONENT, sc);
    if (cond != FIS_NORMAL)
	return 0;

    if (sc->SeriesList == NULL)
	return 0;
    series = LST_Head(&sc->SeriesList);
    (void) LST_Position(&sc->SeriesList, series);
    while (series != NULL) {
	if (series->ImageList == NULL)
	    return 0;

	(void) strcpy(series->StuComUID, sc->StuComUID);
	series->Flag |= FIS_K_SCSERIES_STUCOMUID;
	cond = FIS_Insert(f, FIS_K_SCSERIES, series);
	if (cond != FIS_NORMAL)
	    return 0;

	image = LST_Head(&series->ImageList);
	(void) LST_Position(&series->ImageList, image);
	while (image != NULL) {
	    (void) strcpy(image->StuComUID, sc->StuComUID);
	    image->Flag |= FIS_K_SCIMAGE_STUCOMUID;
	    cond = FIS_Insert(f, FIS_K_SCIMAGE, image);
	    if (cond != FIS_NORMAL)
		return 0;

	    image = LST_Next(&series->ImageList);
	}
	series = LST_Next(&sc->SeriesList);
    }
    return 1;
}


static CONDITION
retrieveInsertInterp(FIS_RESULTSRECORD * results, DUL_ASSOCIATIONKEY ** assoc,
		   DUL_ASSOCIATESERVICEPARAMETERS * params, FIS_HANDLE ** f)
{
    FIS_INTERPRETATIONRECORD *interpPtr,
        interp;
    CONDITION cond,
        rtnCond = 1;

    if (results->Flag & FIS_K_RESULTS_INTERPRETATIONLIST) {
	interpPtr = LST_Head(&results->InterpretationList);
	LST_Position(&results->InterpretationList, interpPtr);
	while (interpPtr != NULL) {
	    printf("%s\n", interpPtr->IntUID);
	    cond = interpretationNGet(assoc, params, interpPtr->IntUID,
				      &interp);
	    if (cond != 1) {
		rtnCond = 0;
		goto ExitPoint;
	    }
	    strcpy(interp.IntUID, interpPtr->IntUID);
	    strcpy(interp.ResUID, results->ResUID);
	    interp.Flag |= (FIS_K_INTERP_RESUID | FIS_K_INTERP_INTUID);
	    if (FIS_Insert(f, FIS_K_INTERPRETATION, &interp) != FIS_NORMAL)
		COND_DumpConditions();

	    interpPtr = LST_Next(&results->InterpretationList);
	}
    }
ExitPoint:
    return rtnCond;
}

static CONDITION
retrieveStudyPatient(GATEWAY_EVENT * e, DUL_NETWORKKEY ** network,
	       DMAN_HANDLE ** d, FIS_HANDLE ** f, char *local, char *remote)
{
    CONDITION cond,
        rtnCond = 1;
    FIS_STUDYRECORD study;
    FIS_PATIENTRECORD patient;
    DUL_ASSOCIATESERVICEPARAMETERS params;
    DUL_ASSOCIATIONKEY *assoc;

    cond = requestAssociationHIS(d, local, remote, network, &assoc,
				 &params);
    if (cond != 1)
	return 0;

    cond = studyNGet(&assoc, &params, e->r.study.StuInsUID, &study);
    if (cond != 1)
	rtnCond = 0;

    if (rtnCond == 1) {
	cond = patientNGet(&assoc, &params, study.PatUID, &patient);
	if (cond != 1)
	    rtnCond = 0;
    }
    strcpy(patient.PatUID, study.PatUID);
    patient.Flag |= FIS_K_PAT_PATUID;
    (void) FIS_Delete(f, FIS_K_PATIENT, FIS_K_PATIENT, patient.PatUID);
    (void) FIS_Insert(f, FIS_K_PATIENT, &patient);

    strcpy(study.StuInsUID, e->r.study.StuInsUID);
    study.Flag |= FIS_K_STU_STUINSUID;
    (void) FIS_Delete(f, FIS_K_STUDY, FIS_K_STUDY, study.StuInsUID);
    (void) FIS_Insert(f, FIS_K_STUDY, &study);

    cond = releaseAssociation(&assoc, &params);
    if (cond != 1)
	rtnCond = 0;

    return rtnCond;
}

static CONDITION
retrieveAllStudy(GATEWAY_EVENT * e, DUL_NETWORKKEY ** network,
	       DMAN_HANDLE ** d, FIS_HANDLE ** f, char *local, char *remote)
{
    CONDITION cond,
        rtnCond = 1;
    FIS_STUDYRECORD study;
    FIS_PATIENTRECORD patient;
    FIS_RESULTSRECORD *resultsPtr,
        results;
    FIS_STUDYCOMPONENTRECORD *scPtr,
        studyComponent;
    DUL_ASSOCIATESERVICEPARAMETERS params;
    DUL_ASSOCIATIONKEY *assoc;

    cond = requestAssociationHIS(d, local, remote, network, &assoc,
				 &params);
    if (cond != 1)
	return 0;

    cond = studyNGet(&assoc, &params, e->r.study.StuInsUID, &study);
    if (cond != 1) {
	rtnCond = 0;
	goto ExitPoint;
    }
    cond = patientNGet(&assoc, &params, study.PatUID, &patient);
    if (cond != 1) {
	rtnCond = 0;
	goto ExitPoint;
    }
    if (study.Flag & FIS_K_STU_RESULTSLIST) {
	resultsPtr = LST_Head(&study.ResultsList);
	LST_Position(&study.ResultsList, resultsPtr);
	while (resultsPtr != NULL) {
	    printf("%s\n", resultsPtr->ResUID);
	    cond = resultsNGet(&assoc, &params, resultsPtr->ResUID, &results);
	    if (cond != 1) {
		rtnCond = 0;
		goto ExitPoint;
	    }
	    strcpy(results.StuInsUID, e->r.study.StuInsUID);
	    strcpy(results.ResUID, resultsPtr->ResUID);
	    results.Flag |= (FIS_K_RESULTS_RESUID | FIS_K_RESULTS_STUINSUID);
	    if (FIS_Insert(f, FIS_K_RESULTS, &results) != FIS_NORMAL)
		COND_DumpConditions();

	    cond = retrieveInsertInterp(&results, &assoc, &params, f);
	    if (cond != 1)
		COND_DumpConditions();

	    resultsPtr = LST_Next(&study.ResultsList);
	}
    }
    if (study.Flag & FIS_K_STU_STUDYCOMPONENTLIST) {
	scPtr = LST_Head(&study.StudyComponentList);
	LST_Position(&study.StudyComponentList, scPtr);
	while (scPtr != NULL) {
	    printf("%s\n", scPtr->StuComUID);
	    cond = studyComponentNGet(&assoc, &params, scPtr->StuComUID,
				      &studyComponent);
	    if (cond != 1) {
		rtnCond = 0;
		goto ExitPoint;
	    }
	    strcpy(studyComponent.StuComUID, scPtr->StuComUID);
	    studyComponent.Flag |= FIS_K_STUDYCOMP_STUCOMUID;
	    cond = insertStudyComponent(f, &studyComponent);
	    if (cond != 1) {
		rtnCond = 0;
		goto ExitPoint;
	    }
	    scPtr = LST_Next(&study.StudyComponentList);
	}
    }
    strcpy(patient.PatUID, study.PatUID);
    patient.Flag |= FIS_K_PAT_PATUID;
    (void) FIS_Insert(f, FIS_K_PATIENT, &patient);

    strcpy(study.StuInsUID, e->r.study.StuInsUID);
    study.Flag |= FIS_K_STU_STUINSUID;
    (void) FIS_Insert(f, FIS_K_STUDY, &study);

ExitPoint:
    cond = releaseAssociation(&assoc, &params);
    if (cond != 1)
	rtnCond = 0;

    return rtnCond;
}


static CONDITION
studyEvent(GATEWAY_EVENT * e, DUL_NETWORKKEY ** network,
	   DMAN_HANDLE ** d, FIS_HANDLE ** f, char *local, char *remote)
{
    CONDITION cond = 1;

    switch (e->Event) {
    case MSG_K_N_STUDY_CREATED:
	break;
    case MSG_K_N_STUDY_SCHEDULED:
	cond = retrieveStudyPatient(e, network, d, f, local, remote);
	break;
    case MSG_K_N_PATIENT_ARRIVED:
    case MSG_K_N_STUDY_STARTED:
    case MSG_K_N_STUDY_COMPLETED:
    case MSG_K_N_STUDY_VERIFIED:
	break;
    case MSG_K_N_STUDY_READ:
	cond = retrieveAllStudy(e, network, d, f, local, remote);
	break;
    case MSG_K_N_STUDY_DELETED:
    case MSG_K_N_STUDY_UPDATED:
	break;
    }
    return cond;
}



/* The next set of functions will handle results events */

static CONDITION
insertNewResults(GATEWAY_EVENT * e, DUL_NETWORKKEY ** network,
	       DMAN_HANDLE ** d, FIS_HANDLE ** f, char *local, char *remote)
{
    CONDITION cond,
        rtnCond = 1;
    FIS_RESULTSRECORD *resultsPtr,
        results;

    resultsPtr = &(e->r.results);
    if (logLevel > 0) {
	fprintf(stdout, "\nAbout to insert new results record from %s\n", remote);
	FIS_DumpRecord(resultsPtr, stdout);
    }
    /* Test to see if we already have these results.  If so, just return */
    memset(&results, 0, sizeof(results));
    cond = FIS_GetOne(f, FIS_K_RESULTS, FIS_K_RESULTS,
		      resultsPtr->ResUID, 0, &results);
    if (cond == FIS_NORMAL) {
	if (logLevel > 0) {
	    fprintf(stdout, "Did not need to insert new results; already exists\n");
	}
	return rtnCond;
    }
    COND_PopCondition();

    cond = FIS_Insert(f, FIS_K_RESULTS, resultsPtr);
    if (cond != FIS_NORMAL) {
	COND_DumpConditions();
	rtnCond = 0;
    }
    return rtnCond;
}

static CONDITION
resultsEvent(GATEWAY_EVENT * e, DUL_NETWORKKEY ** network,
	     DMAN_HANDLE ** d, FIS_HANDLE ** f, char *local, char *remote)
{
    CONDITION cond = 1;

    switch (e->Event) {
    case MSG_K_N_RESULTS_CREATED:
	cond = insertNewResults(e, network, d, f, local, remote);
	break;
    case MSG_K_N_RESULTS_UPDATED:
    case MSG_K_N_RESULTS_DELETED:
	break;
    default:
	break;
    }
    return cond;
}

/* Interpretation events handled here */

static CONDITION
insertNewInterpretation(GATEWAY_EVENT * e, DUL_NETWORKKEY ** network,
	       DMAN_HANDLE ** d, FIS_HANDLE ** f, char *local, char *remote)
{
    CONDITION cond,
        rtnCond = 1;
    FIS_INTERPRETATIONRECORD *interpretationPtr,
        interpretation;

    interpretationPtr = &(e->r.interpretation);
    if (logLevel > 0) {
	fprintf(stdout, "\nAbout to insert new interpretation record from %s\n", remote);
	FIS_DumpRecord(interpretationPtr, stdout);
    }
    /*
     * Test to see if we already have this interpretation.  If so, just
     * return
     */
    memset(&interpretation, 0, sizeof(interpretation));
    cond = FIS_GetOne(f, FIS_K_INTERPRETATION, FIS_K_INTERPRETATION,
		      interpretationPtr->IntUID, 0, &interpretation);
    if (cond == FIS_NORMAL) {
	if (logLevel > 0) {
	    fprintf(stdout, "No need to insert new interpretation record; already exists\n");
	}
	return rtnCond;
    }
    COND_PopCondition();

    if ((interpretationPtr->Flag & FIS_K_INTERP_INTTYPID) == 0) {
	strcpy(interpretationPtr->IntTypID, "REPORT");
	interpretationPtr->Flag |= FIS_K_INTERP_INTTYPID;
    }
    if ((interpretationPtr->Flag & FIS_K_INTERP_INTSTAID) == 0) {
	strcpy(interpretationPtr->IntStaID, "CREATED");
	interpretationPtr->Flag |= FIS_K_INTERP_INTSTAID;
    }
    cond = FIS_Insert(f, FIS_K_INTERPRETATION, interpretationPtr);
    if (cond != FIS_NORMAL) {
	COND_DumpConditions();
	rtnCond = 0;
    }
    return rtnCond;
}

static CONDITION
updateInterpretation(GATEWAY_EVENT * e, DUL_NETWORKKEY ** network,
	       DMAN_HANDLE ** d, FIS_HANDLE ** f, char *local, char *remote)
{
    CONDITION cond,
        rtnCond = 1;
    FIS_INTERPRETATIONRECORD *interpretationPtr,
        interpretation;

    interpretationPtr = &(e->r.interpretation);
    if (logLevel > 0) {
	fprintf(stdout, "\nAbout to update interpretation record from %s\n", remote);
	FIS_DumpRecord(interpretationPtr, stdout);
    }
    /* Test to see if we already have this interpretation. */
    memset(&interpretation, 0, sizeof(interpretation));
    cond = FIS_GetOne(f, FIS_K_INTERPRETATION, FIS_K_INTERPRETATION,
		      interpretationPtr->IntUID, 0, &interpretation);
    if (cond != FIS_NORMAL) {
	fprintf(stderr, "Could not update interpretation record: %s from %s\n",
		interpretationPtr->IntUID, remote);
	COND_DumpConditions();
	return 0;
    }
    cond = FIS_Update(f, FIS_K_INTERPRETATION, interpretationPtr);
    if (cond != FIS_NORMAL) {
	COND_DumpConditions();
	rtnCond = 0;
    }
    return rtnCond;
}

static CONDITION
interpretationEvent(GATEWAY_EVENT * e, DUL_NETWORKKEY ** network,
	       DMAN_HANDLE ** d, FIS_HANDLE ** f, char *local, char *remote)
{
    CONDITION cond = 1;

    switch (e->Event) {
    case MSG_K_N_INTERPRETATION_CREATED:
	cond = insertNewInterpretation(e, network, d, f, local, remote);
	break;
    case MSG_K_N_INTERPRETATION_APPROVED:
	cond = updateInterpretation(e, network, d, f, local, remote);
	break;
    default:
	break;
    }
    return cond;
}


/* Public entry points are below this comment */


CONDITION
processEvents(DUL_NETWORKKEY ** network,
	  DMAN_HANDLE ** dman, FIS_HANDLE ** fis, char *local, char *remote)
{
    GATEWAY_EVENT *e;
    CONDITION cond = 1;

    while ((e = LST_Dequeue(&eventList)) != NULL && (cond == 1)) {
	switch (e->Type) {
	case FIS_K_PATIENT:
	    break;
	case FIS_K_VISIT:
	    break;
	case FIS_K_STUDY:
	    cond = studyEvent(e, network, dman, fis, local, remote);
	    break;
	case FIS_K_RESULTS:
	    cond = resultsEvent(e, network, dman, fis, local, remote);
	    break;
	case FIS_K_INTERPRETATION:
	    cond = interpretationEvent(e, network, dman, fis, local, remote);
	    break;
	default:
	    break;
	}
	free(e);
    }

    return cond;
}
