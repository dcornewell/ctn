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
** Module Name(s):	getRequest
**
**	Private Routines
**			ngetCallback
**			findElem
**			unsupportedCallback
** Author, Date:	Aniruddha S. Gokhale, 29-June-1994
** Intent:		This module defines the functions necessary to
**			service the incoming N-GET request for printer
**			attributes.
** Last Update:		$Author: smm $, $Date: 1995-12-29 04:12:45 $
** Source File:		$RCSfile: get.c,v $
** Revision:		$Revision: 1.17 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.17 $ $RCSfile: get.c,v $";
#include "print_server.h"

extern CTNBOOLEAN
    traceFlag;
static CONDITION
ngetCallback(MSG_N_GET_REQ * ngetRequest,
	     MSG_N_GET_RESP * ngetResponse,
	     void *ctx, DUL_PRESENTATIONCONTEXT * pc);
static CONDITION
unsupportedCallback(MSG_N_GET_REQ * ngetRequest,
		    MSG_N_GET_RESP * ngetResponse,
		    void *ctx, DUL_PRESENTATIONCONTEXT * pc);
static int findElem(DCM_ELEMENT * elem, DCM_TAG tag, int size);

/* getRequest
**
** Purpose:
**	Invokes the SRV_NGetResponse to	send the requested attributes of the
**	printer to the SCU
** Parameter Dictionary:
**	association	The key which is used to access the association
**                      on which requests are received.
**	ctx		Pointer to the presentation context for this command.
**	request		Pointer to the N-GET Request message.
**
** Return Values:
**	A condition which should be SRV_NORMAL if no errors are encountered.
**
**	SRV_CALLBACKABORTEDSERVICE
**	SRV_ILLEGALPARAMETER
**	SRV_NOCALLBACK
**	SRV_NORMAL
**	SRV_OBJECTBUILDFAILED
**	SRV_RESPONSEFAILED
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
getRequest(DUL_ASSOCIATIONKEY ** association,
	   DUL_PRESENTATIONCONTEXT * ctx, MSG_N_GET_REQ ** request)
{
    MSG_N_GET_RESP
	getResponse;		/* N-GET Response to be generated and sent to
				 * the SCU */
    CONDITION
	cond;


    memset(&getResponse, 0, sizeof(MSG_N_GET_RESP));
    if (strcmp((*request)->classUID, DICOM_SOPCLASSPRINTER) == 0) {
	cond = SRV_NGetResponse(association, ctx, request, &getResponse,
				ngetCallback, ctx, NULL);
	if (cond != SRV_NORMAL) {
	    printf("In getRequest : SRV_NGetResponse failed\n");
	}
    } else {
	/* unsupported */
	cond = SRV_NGetResponse(association, ctx, request, &getResponse,
				unsupportedCallback, ctx, NULL);
	if (cond != SRV_NORMAL) {
	    printf("In getRequest : SRV_NGetResponse failed\n");
	}
    }
    return cond;
}

typedef struct {
    long prnFlag;
    DCM_TAG dcmFlag;
}   PRINTER_FLAG;

/* ngetCallback
**
** Purpose:
**	The callback routine which is called by SRV_NGetResponse. This
**	routine s responsible for actually building up the N-GET response
**	message.
**
** Parameter Dictionary:
**	ngetRequest	Pointer to the N-GET Request message
**	ngetResponse	Pointer to the N-GET Response message to be returned
**	ctx		Pointer to the context for this command
**	pc		Presentation context which describes SOP Class
**
** Return Values:
**	In normal circumstances, should return SRV_NORMAL
**
**	DCM_ELEMENTNOTFOUND
**	DCM_GETINCOMPLETE
**	DCM_ILLEGALOBJECT
**	DCM_NULLOBJECT
**	SRV_CALLBACKABORTEDSERVICE
**	SRV_NORMAL
**
** Algorithm:
**	Look at the various tags of the attribute list field of the request
**		message.
**	Depending on the attributes requested (if none are requested,
**		return all), return the values of those attributes as a
**		data set in the response message.
**
*/

static CONDITION
ngetCallback(MSG_N_GET_REQ * ngetRequest,
	     MSG_N_GET_RESP * ngetResponse,
	     void *ctx, DUL_PRESENTATIONCONTEXT * pc)
{
    static PRINTER_FLAG printFlag[] = {
	{PRN_PRINTER_K_STATUS, DCM_PRINTERSTATUS},
	{PRN_PRINTER_K_STATUSINFO, DCM_PRINTERSTATUSINFO},
	{PRN_PRINTER_K_NAME, DCM_PRINTERNAME},
	{PRN_PRINTER_K_MANUFACTURER, DCM_IDMANUFACTURER},
	{PRN_PRINTER_K_MODELNUMBER, DCM_IDMANUFACTURERMODEL},
	{PRN_PRINTER_K_SERIALNUMBER, DCM_ACQDEVICESERIALNUM},
	{PRN_PRINTER_K_SOFTWAREVERSION, DCM_ACQSOFTWAREVERSION},
	{PRN_PRINTER_K_DATEOFCALIBRATION, DCM_ACQDATELASTCALIBRATION},
	{PRN_PRINTER_K_TIMEOFCALIBRATION, DCM_ACQTIMELASTCALIBRATION}
    };

    static PRN_PRINTER printAttrib = {	/* attributes of the printer
					 * maintained by the SCP */
	NULL, NULL,
	PRN_K_BASICPRINTER,	/* type */
	0,			/* flag */
	DICOM_SOPCLASSPRINTER,	/* SOP class UID */
	DICOM_SOPPRINTERINSTANCE,	/* instance UID */
	PRINTERNORMAL,		/* status */
	NO_INFO,		/* info code */
	"XYZ Laser",		/* name */
	"ABXY Inc.",		/* manufacturer */
	"ABXY-LPF",		/* manufacturer model name */
	"pq34mn.V8",		/* serial number */
	"las-pri v3.2",		/* software version */
	"19930721",		/* date of last calibration */
	"141550"		/* time of last calibratiom */
    };

    static SUPPORTEDATTRIBUTES
        supportedAttributeList[] = {
	{PRN_PRINTER_K_STATUS, "PRINTER Status", NULL, 0},
	{PRN_PRINTER_K_STATUSINFO, "PRINTER Status Information", NULL, 0},
	{PRN_PRINTER_K_NAME, "PRINTER Name", NULL, 0},
	{PRN_PRINTER_K_MANUFACTURER, "PRINTER Manufacturer", NULL, 0},
	{PRN_PRINTER_K_MODELNUMBER, "PRINTER Model number", NULL, 0},
	{PRN_PRINTER_K_SERIALNUMBER, "PRINTER Serial Number", NULL, 0},
	{PRN_PRINTER_K_SOFTWAREVERSION, "PRINTER Software Version", NULL, 0},
	{PRN_PRINTER_K_DATEOFCALIBRATION, "PRINTER Date of last calibration",
	NULL, 0},
	{PRN_PRINTER_K_TIMEOFCALIBRATION, "PRINTER Time of last calibration",
	NULL, 0}
    };

    static DCM_ELEMENT prnAttribElem[] = {
	{DCM_PRINTERSTATUS, DCM_CS, "Status Normal", 1,
	sizeof(printAttrib.status), (void *) printAttrib.status},
	{DCM_PRINTERSTATUSINFO, DCM_CS, "Info code", 1,
	    sizeof(printAttrib.statusInfoCode),
	(void *) printAttrib.statusInfoCode},
	{DCM_PRINTERNAME, DCM_LO, "Printer name", 1,
	sizeof(printAttrib.name), (void *) printAttrib.name},
	{DCM_IDMANUFACTURER, DCM_LO, "", 1,
	    sizeof(printAttrib.manufacturer),
	(void *) printAttrib.manufacturer},
	{DCM_IDMANUFACTURERMODEL, DCM_LO, "Manufacturer Model", 1,
	    sizeof(printAttrib.manufacturerModelName),
	(void *) printAttrib.manufacturerModelName},
	{DCM_ACQDEVICESERIALNUM, DCM_LO, "Device Serial #", 1,
	    sizeof(printAttrib.deviceSerialNumber),
	(void *) printAttrib.deviceSerialNumber},
	{DCM_ACQSOFTWAREVERSION, DCM_LO, "Sofware version", 1,
	    sizeof(printAttrib.softwareVersion),
	(void *) printAttrib.softwareVersion},
	{DCM_ACQDATELASTCALIBRATION, DCM_DA, "Calibration date", 1,
	    sizeof(printAttrib.dateOfLastCalibration),
	(void *) printAttrib.dateOfLastCalibration},
	{DCM_ACQTIMELASTCALIBRATION, DCM_TM, "Calibration time", 1,
	    sizeof(printAttrib.timeOfLastCalibration),
	(void *) printAttrib.timeOfLastCalibration}
    };
    int
        index,
        index2;
    CONDITION
	cond;
    DCM_ELEMENT
	element;
    PRN_PRINTER
	printer;
    char
       *flagNameList;
    int
        unknownAttribCount = 0;	/* holds the number of attributes of the
				 * printer requested by the SCU but unknown
				 * to the SCP */

    /* we build up the response message */
    ngetResponse->type = MSG_K_N_GET_RESP;
    ngetResponse->messageIDRespondedTo = ngetRequest->messageID;
    strcpy(ngetResponse->classUID, ngetRequest->classUID);
    strcpy(ngetResponse->affectedInstanceUID,
	   ngetRequest->requestedInstanceUID);
    ngetResponse->dataSetType = DCM_CMDDATANULL;	/* initialize */

    /* find which attributes are requested */
    printer.type = PRN_K_BASICPRINTER;
    printer.attributeFlag = 0x0;
    for (index = 0; index < ngetRequest->attributeCount; index++) {
	for (index2 = 0; index2 < (int) DIM_OF(printFlag); index2++) {
	    if (ngetRequest->attributeList[index] ==
		printFlag[index2].dcmFlag)
		printer.attributeFlag |= printFlag[index2].prnFlag;
	}
    }
    /* verify if requested attributes are supported */
    cond = supportedAttributes(MSG_K_N_GET_REQ, &printer,
	       supportedAttributeList, (int) DIM_OF(supportedAttributeList),
			       &flagNameList, (void **) &ngetResponse);
    if (cond != APP_NORMAL) {
	switch (cond) {
	case APP_MISSINGREQUIREDATTRIBUTE:
	    ngetResponse->status = MSG_K_MISSINGATTRIBUTE;
	    cond = COND_PushCondition(APP_ERROR(cond), "ngetCallback");
	    goto ERROR_RETURN;
	case APP_UNSUPPORTEDMANDATORYATTRIBUTE:
	    cond = COND_PushCondition(APP_ERROR(cond), "ngetCallback");
	    goto ERROR_RETURN;
	case APP_UNSUPPORTEDOPTIONALATTRIBUTE:
	    /* ngetResponse->status = MSG_K_NOSUCHATTRIBUTE; */
	    ngetResponse->status = MSG_K_SUCCESS;
	    break;
	}
    }
    /*
     * depending on the attribute list passed by the client, we now need to
     * build a data set containing the requested attributes The SRV routine
     * reserves space for the data set. We need to explicitly set the data
     * set type to DCM_CMDDATAOTHER
     */
    ngetResponse->dataSetType = DCM_CMDDATAOTHER;	/* as a data set has to
							 * be present */
    if (ngetRequest->attributeCount == 0) {
	/*
	 * if attribute count is 0, return all attributes supported by the
	 * SCP in the response message
	 */
	for (index = 0; index < (int) DIM_OF(prnAttribElem); index++) {
	    if (traceFlag) {
		element.tag = prnAttribElem[index].tag;
		element.description[0] = '\0';
		(void) DCM_LookupElement(&element);
		printf("N-GET for (%08x) %s\n",
		       prnAttribElem[index].tag, element.description);
	    }
	    cond = DCM_ModifyElements(&ngetResponse->dataSet,
				   &prnAttribElem[index], 1, NULL, 0, NULL);
	    if (cond != DCM_NORMAL) {
		cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				      "DCM_ModifyElements", "ngetCallback");
		updateOnProcessingFailure((void **) &ngetResponse,
					  APP_ERROR(APP_FAILURE), "DCM_ModifyElements", "ngetCallback");
		goto ERROR_RETURN;
	    }
	}
	ngetResponse->status = MSG_K_SUCCESS;
	return SRV_NORMAL;
    }
    for (index = 0; index < ngetRequest->attributeCount; index++) {

	/*
	 * go through the attribute tag list and find which attributes are
	 * requested. Accordingly insert that attribute value in the data set
	 * of the response message
	 */
	index2 = findElem(prnAttribElem, ngetRequest->attributeList[index],
			  DIM_OF(prnAttribElem));
	if (index2 == -1) {
	    printf("Server Not aware of such an attribute : %x\n",
		   ngetRequest->attributeList[index]);
	    unknownAttribCount++;
	    continue;		/* proceed with the next request */
	}
	if (traceFlag) {
	    element.tag = prnAttribElem[index2].tag;
	    element.description[0] = '\0';
	    (void) DCM_LookupElement(&element);
	    printf("N-GET for (%08x) %s\n",
		   prnAttribElem[index2].tag, element.description);
	}
	cond = DCM_ModifyElements(&ngetResponse->dataSet,
				  &prnAttribElem[index2], 1, NULL, 0, NULL);
	if (cond != DCM_NORMAL) {
	    cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				      "DCM_ModifyElements", "ngetCallback");
	    updateOnProcessingFailure((void **) &ngetResponse,
	      APP_ERROR(APP_FAILURE), "DCM_ModifyElements", "ngetCallback");
	    goto ERROR_RETURN;
	}
    }
    if (unknownAttribCount) {
	ngetResponse->status = MSG_K_ATTRIBUTELISTERRORR;
	goto ERROR_RETURN;
    }
    ngetResponse->status = MSG_K_SUCCESS;
    return SRV_NORMAL;

ERROR_RETURN:
    COND_DumpConditions();
    return SRV_CALLBACKABORTEDSERVICE;
}

/* findElem
**
** Purpose:
**	Given a DCM tag, return the matching DCM element from an array of
**	DCM elements
**
** Parameter Dictionary:
**	elem	Array of DCM elements
**	tag	DCM tag to be searched
**	size	size of array
**
** Return Values:
**	the index of the array where the tag matched.
**	-1 returned on error.
**
** Algorithm:
**	Do a linear search.
*/
static int
findElem(DCM_ELEMENT * elem, DCM_TAG tag, int size)
{
    int
        index;

    for (index = 0; index < size; index++) {
	if (tag == elem->tag)
	    return index;
	elem++;
    }
    return -1;
}
/* unsupportedCallback
**
** Purpose:
**	This callback routine is invoked by the SRV_NSetResponse
**	whenever service for the requested SOP class is unsupported
**
** Parameter Dictionary:
**	ngetRequest	Pointer to request message
**	ngetResponse	Pointer to response message
**	ctx		Application context
**	pc		Presentation context for this SOP Class
**
** Return Values:
**	SRV_CALLBACKABORTEDSERVICE
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
unsupportedCallback(MSG_N_GET_REQ * ngetRequest,
		    MSG_N_GET_RESP * ngetResponse,
		    void *ctx, DUL_PRESENTATIONCONTEXT * pc)
{
    ngetResponse->status = MSG_K_UNRECOGNIZEDOPERATION;
    return SRV_CALLBACKABORTEDSERVICE;
}
