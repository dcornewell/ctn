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
** Module Name(s):      handleNEventRequest
**			eventCallback
**			convertTags
** Author, Date:        John O'Neill, Stephen Moore, 8-Sep-94
** Intent:              Handles DICOM NGET requests for HIS functions
**
** Last Update:         $Author: smm $, $Date: 2001-12-21 16:44:51 $
** Source File:         $RCSfile: event.c,v $
** Revision:            $Revision: 1.4 $
** Status:              $State: Exp $
*/
static char rcsid[] = "$Revision: 1.4 $ $RCSfile: event.c,v $";

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

typedef struct {
    char *abstractSyntax;
    FIS_HANDLE **fis;
}   CALLBACK_CTX;

typedef struct {
    DCM_TAG tag;
    unsigned long flag;
}   TAG_MAP;

static CONDITION
eventCallback(MSG_N_EVENT_REPORT_REQ * request,
	      MSG_N_EVENT_REPORT_RESP * response, void *ctx,
	      DUL_PRESENTATIONCONTEXT * pc);
static void
convertTags(int attributeCount, DCM_TAG * attributeList,
	    TAG_MAP * map, long *flagAddress, int size);
static CONDITION
patientEvent(MSG_N_EVENT_REPORT_REQ * request, MSG_N_EVENT_REPORT_RESP * response,
	     CALLBACK_CTX * ctx);
static CONDITION
visitEvent(MSG_N_EVENT_REPORT_REQ * request, MSG_N_EVENT_REPORT_RESP * response,
	   CALLBACK_CTX * ctx);
static CONDITION
studyEvent(MSG_N_EVENT_REPORT_REQ * request, MSG_N_EVENT_REPORT_RESP * response,
	   CALLBACK_CTX * ctx);
static CONDITION
resultsEvent(MSG_N_EVENT_REPORT_REQ * request, MSG_N_EVENT_REPORT_RESP * response,
	     CALLBACK_CTX * ctx);
static CONDITION
interpretationEvent(MSG_N_EVENT_REPORT_REQ * request, MSG_N_EVENT_REPORT_RESP * response,
		    CALLBACK_CTX * ctx);
static CONDITION
studyComponentEvent(MSG_N_EVENT_REPORT_REQ * request, MSG_N_EVENT_REPORT_RESP * response,
		    CALLBACK_CTX * ctx);

/* ngetRequest
**
*/

CONDITION
NEventReportRequest(DUL_ASSOCIATESERVICEPARAMETERS * params,
		    DUL_ASSOCIATIONKEY ** association, DMAN_HANDLE ** dman,
	   DUL_PRESENTATIONCONTEXT * ctx, MSG_N_EVENT_REPORT_REQ ** request,
		    FIS_HANDLE ** fis)
{
    MSG_N_EVENT_REPORT_RESP response;
    CONDITION cond;
    CALLBACK_CTX cb;

    memset(&response, 0, sizeof(response));
    response.type = MSG_K_N_EVENT_REPORT_RESP;
    cb.abstractSyntax = ctx->abstractSyntax;
    cb.fis = fis;

    cond = SRV_NEventReportResponse(association, ctx, request, &response,
				    eventCallback, &cb, "");

    return cond;
}

/* eventCallback
**
** Purpose:
**	This function fills the response structure based on the request
**	structure. It opens and calls a database to find information
**	about the requested instance in the request structure. Then, from
** 	the database information, a DCM_OBJECT is constructed.
**
** Parameter Dictionary:
**	request		structure containing the instance to look up in the
**			database and the attributes to include in the
**			DCM_OBJECT.
**	response	structure filled in by ngetCallback; contains
**			the DCM_OBJECT built.
**	ctxPtr		Pointer to a structure with some parameters we need
**			to implement this callback.
**	pc		Presentation context for this SOP class.
**
** Return Values:
**
**	SRV_NORMAL
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/

static CONDITION
eventCallback(MSG_N_EVENT_REPORT_REQ * request,
	      MSG_N_EVENT_REPORT_RESP * response, void *ctxPtr,
	      DUL_PRESENTATIONCONTEXT * pc)
{
    CONDITION cond = SRV_NORMAL;
    GATEWAY_EVENT *e;
    CALLBACK_CTX *ctx;

    ctx = (CALLBACK_CTX *) ctxPtr;

    e = malloc(sizeof(*e));
    if (e == NULL)
	return 0;

    if (logLevel > 1) {
	fprintf(stdout, "\nA new event request was received:\n");
	MSG_DumpMessage(request, stdout);
    }
    if (request->dataSetType != DCM_CMDDATANULL) {
	if (logLevel > 1)
	    (void) DCM_DumpElements(&request->dataSet, 0);

	if (strcmp(request->classUID, DICOM_SOPCLASSDETACHEDPATIENTMGMT) == 0) {
	    ;
	} else if (strcmp(request->classUID, DICOM_SOPCLASSDETACHEDVISITMGMT) == 0) {
	    ;
	} else if (strcmp(request->classUID, DICOM_SOPCLASSDETACHEDSTUDYMGMT) == 0) {
	    e->Type = FIS_K_STUDY;
	    cond = FIS_ParseObject((DCM_OBJECT **) & (request->dataSet),
				   FIS_K_STUDY, &e->r.study);
	    strcpy(e->r.study.StuInsUID, request->affectedInstanceUID);
	    e->r.study.Flag |= FIS_K_STU_STUINSUID;
	} else if (strcmp(request->classUID, DICOM_SOPCLASSDETACHEDRESULTSMGMT) == 0) {
	    e->Type = FIS_K_RESULTS;
	    cond = FIS_ParseObject((DCM_OBJECT **) & (request->dataSet),
				   FIS_K_RESULTS, &e->r.results);
	    strcpy(e->r.results.ResUID, request->affectedInstanceUID);
	    e->r.results.Flag |= FIS_K_RESULTS_RESUID;
	} else if (strcmp(request->classUID, DICOM_SOPCLASSDETACHEDINTERPRETMGMT) == 0) {
	    e->Type = FIS_K_INTERPRETATION;
	    cond = FIS_ParseObject((DCM_OBJECT **) & (request->dataSet),
				FIS_K_INTERPRETATION, &e->r.interpretation);
	    strcpy(e->r.interpretation.IntUID, request->affectedInstanceUID);
	    e->r.interpretation.Flag |= FIS_K_INTERP_INTUID;
	}
    }
    if (cond != FIS_NORMAL)
	return 0;

    e->Event = request->eventTypeID;
    (void) LST_Enqueue(&eventList, e);

    response->dataSetType = DCM_CMDDATANULL;
    response->eventTypeID = request->eventTypeID;
    response->conditionalFields = MSG_K_N_EVENTREPORTRESP_EVENTTYPEID;
    return SRV_NORMAL;
}
