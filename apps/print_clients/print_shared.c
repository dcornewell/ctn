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
** @$=@$=@$=
*/
/*
**				DICOM 93
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	initNetwork
**			establishAssociation
**			decodeImageDisplayFormat
**			getPrinterInstance
**			ngetCallback
**			createFilmSession
**			ncreateBFSCallback
**			createFilmBox
**			ncreateBFBCallback
**			setImageBox
**			setImageBoxCallback
**			sendFilmBoxPrint
**			filmBoxPrintCallback
**			sendDeleteRequest
**			ndeleteCallback
**			getImageData
**			displayErrorStatus
**
** Author, Date:	Aniruddha S. Gokhale, August 4,1993.
** Intent:		This module conatains routines that are used by
**			both the print_mgr and the print_client utilities.
**
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:47:23 $
** Source File:		$RCSfile: print_shared.c,v $
** Revision:		$Revision: 1.46 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.46 $ $RCSfile: print_shared.c,v $";

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
#include "dbquery.h"
#include "dicom_uids.h"
#include "dicom_sq.h"
#include "dicom_print.h"

#include "print_client_app.h"

static CONDITION
ngetCallback(MSG_N_GET_REQ * ngetRequest, MSG_N_GET_RESP * ngetResponse,
/*	     PRN_PRINTER ** printerAttributes); */
	     void *printerAttributes);
static CONDITION
ncreateBFSCallback(MSG_N_CREATE_REQ * createRequest,
		   MSG_N_CREATE_RESP * createResponse,
/*		   PRN_BASICFILMSESSION * bfsAttrib); */
		   void *bfsAttrib);
static CONDITION
ncreateBFBCallback(MSG_N_CREATE_REQ * createRequest,
		   MSG_N_CREATE_RESP * createResponse,
/*		   PRN_BASICFILMBOX * bfbAttrib); */
		   void *bfbAttrib);
static CONDITION
setImageBoxCallback(MSG_N_SET_REQ * setRequest,
		    MSG_N_SET_RESP * setResponse,
/*		    PRN_BASICIMAGEBOX * bibAttrib); */
		    void *bibAttrib);
static CONDITION
filmBoxPrintCallback(MSG_N_ACTION_REQ * actionRequest,
/*	    MSG_N_ACTION_RESP * actionResponse, PRN_BASICFILMBOX * filmBox); */
		     MSG_N_ACTION_RESP * actionResponse, void *filmBox);
static CONDITION
ndeleteCallback(MSG_N_DELETE_REQ * deleteRequest,
		MSG_N_DELETE_RESP * deleteResponse, void *ctx);

/* Extern Declarations */
extern CTNBOOLEAN silent;

/* initNetwork
**
** Purpose:
**	Initializes the network and obtains the network key on which the
**	association is to be established.
**
** Parameter Dictionary:
**	params		Association service parameters
**	network		Network key
**
** Return Values:
**
**	DUL_UNSUPPORTEDNETWORK
**	DUL_UNRECOGNIZEDAE
**	DUL_KEYCREATEFAILURE
**	DUL_NETWORKINITIALIZED
**	DUL_NORMAL if initialization is successful. As a side effect, it also
**	returns the network key.
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
initNetwork(DUL_ASSOCIATESERVICEPARAMETERS * params,
	    DUL_NETWORKKEY ** network)
{
    CONDITION
	cond;

    /* Initialize the association parameter structure */
    (void) strcpy(params->applicationContextName, DICOM_STDAPPLICATIONCONTEXT);
    (void) strcpy(params->callingAPTitle, "PRINT_CLIENT");
    (void) strcpy(params->calledAPTitle, "PRINT_SERVER");
    (void) strcpy(params->respondingAPTitle, "");
    params->maxPDU = 16384;
    params->result = 0;
    params->resultSource = 0;
    params->diagnostic = 0;
    (void) strcpy(params->callingPresentationAddress, "calling presentation addr");
    (void) strcpy(params->calledPresentationAddress, "called presentation addr");
    params->requestedPresentationContext = NULL;
    params->acceptedPresentationContext = NULL;
    params->maximumOperationsInvoked = 0;
    params->maximumOperationsPerformed = 0;
    (void) strcpy(params->callingImplementationClassUID,
		  MIR_IMPLEMENTATIONCLASSUID);
    (void) strcpy(params->callingImplementationVersionName,
		  MIR_IMPLEMENTATIONVERSIONNAME);
    (void) strcpy(params->calledImplementationClassUID, "");
    (void) strcpy(params->calledImplementationVersionName, "");

    cond = DUL_InitializeNetwork(DUL_NETWORK_TCP, DUL_AEREQUESTOR,
			    NULL, DUL_TIMEOUT, DUL_ORDERBIGENDIAN, network);
    if (cond != DUL_NORMAL)
	printf("In initNetwork : DUL_InitializeNetwork failed\n");

    return cond;
}

/* establishAssociation
**
** Purpose:
**	Establishes an association with the given node and port number. In
**	addition, it also negotiates for the BASIC PRINT MANAGEMENT META
**	SOP Class.
**
** Parameter Dictionary:
**	params		Association service parameters
**	network		The network key
**	SOPClassList	The list of SOPClasses which need to be negotiated
**			over the association
**	association	The association key
**
** Return Values:
**	DUL_NORMAL if association is established. As a side effect, it
**	also returns the association key.
**
**	DUL_ASSOCIATIONREJECTED
**	DUL_ILLEGALKEY
**	DUL_ILLEGALREQUEST
**	DUL_NOCONNECTION
**	DUL_NORMAL
**	DUL_NULLKEY
**	DUL_REQUESTASSOCIATIONFAILED
**	PRN_MALLOCFAILURE
**	SRV_LISTFAILURE
**	SRV_NORMAL
**	SRV_PRESENTATIONCONTEXTERROR
**	SRV_UNSUPPORTEDSERVICE
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
establishAssociation(DUL_ASSOCIATESERVICEPARAMETERS * params,
		     DUL_NETWORKKEY ** network,
		     char **SOPClassList,
		     DUL_ASSOCIATIONKEY ** association)
{
    CONDITION
	cond;


    /* negotiate for the requested SOP Class and request an association */
    while (strlen(*SOPClassList) > 0) {
	cond = SRV_RequestServiceClass(*SOPClassList, DUL_SC_ROLE_DEFAULT,
				       params);
	if (cond != SRV_NORMAL) {
	    printf("%s SOP class not supported\n", *SOPClassList);
	    COND_DumpConditions();
	    return cond;
	}
	SOPClassList++;
    }
#ifdef ASG
    /* now negotiate for Gray Scale Print Management Meta SOP class */
    strcpy(SOPClass, DICOM_SOPCLASSGREYSCALEPRINTMGMTMETA);
    /* negotiate for the requested SOP Class and request an association */
    cond = SRV_RequestServiceClass(SOPClass, DUL_SC_ROLE_DEFAULT, params);
    if (cond != SRV_NORMAL) {
	printf("Basic Gray Print Management not supported\n");
	COND_DumpConditions();
	return cond;
    }
    /* negotiate for Color Print Management Meta SOP class */
    strcpy(SOPClass, DICOM_SOPCLASSCOLORPRINTMGMTMETA);
    cond = SRV_RequestServiceClass(SOPClass, DUL_SC_ROLE_DEFAULT, params);
    if (cond != SRV_NORMAL) {
	printf("Basic Color Print Management not supported\n");
	COND_DumpConditions();
	return cond;
    }
    /* now negotiate for Print Job SOP class */
    strcpy(SOPClass, DICOM_SOPCLASSPRINTJOB);
    cond = SRV_RequestServiceClass(SOPClass, DUL_SC_ROLE_DEFAULT, params);
    if (cond != SRV_NORMAL) {
	printf("Print Job not supported\n");
	printf("SCU : Allowed to proceed without PRINT JOB SOP class\n");
	COND_DumpConditions();
	return cond;
    }
#endif
    /* Now request for association */
    cond = DUL_RequestAssociation(network, params, association);
    if (cond != DUL_NORMAL)
	if (cond == DUL_ASSOCIATIONREJECTED) {
	    fprintf(stderr, "Association Rejected\n");
	    fprintf(stderr, " Result: %2x Source %2x Reason %2x\n",
		    params->result, params->resultSource,
		    params->diagnostic);
	}
    return cond;
}

/* decodeImageDisplayFormat
**
** Purpose:
**	Given the image display format for an IMAGE BOX, decode the
**	format type and determine the number of row and/or columns of
**	images.
** Parameter Dictionary:
**      displayFormat	contains the image display format
**
** Return Values:
**	A pointer to a data structure that holds the "type" of the
**	image display e.g. STANDARD and the number of images arranged
**	in row-major or column major order.
**
**	NULL on failure.
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/
IMG_DISPLAYFORMAT
* decodeImageDisplayFormat(char *displayFormat)
{
    char
        string[DICOM_LO_LENGTH + 1];	/* scratch variable */
    int
        index;
    int
        index2 = 0;
    int
        i;
    int
        numList[20];		/* scratch variable */
    IMG_DISPLAYFORMAT
	* imgDisplay;		/* the data structure to be returned */


    if ((imgDisplay = malloc(sizeof(IMG_DISPLAYFORMAT))) == NULL) {
	printf("Out of memory in decodeImageDisplayFormat\n");
	return NULL;
    }
    index = 0;
    while ((index < strlen(displayFormat))
	   && (displayFormat[index] != '\\')) {
	string[index] = displayFormat[index];
	index++;
    }
    string[index] = '\0';

    /* decode the display format type first */
    if (strcmp(string, "STANDARD") == 0)
	strcpy(imgDisplay->displayFormat, STANDARD);
    else if (strcmp(string, "ROW") == 0)
	strcpy(imgDisplay->displayFormat, ROW);
    else if (strcmp(string, "COL") == 0)
	strcpy(imgDisplay->displayFormat, COL);
    else if (strcmp(string, "SLIDE") == 0)
	strcpy(imgDisplay->displayFormat, SLIDE);
    else if (strcmp(string, "SUPERSLIDE") == 0)
	strcpy(imgDisplay->displayFormat, SUPERSLIDE);
    else if (strcmp(string, "CUSTOM") == 0)
	strcpy(imgDisplay->displayFormat, CUSTOM);
    else
	return NULL;

    /*
     * now obtain the number of images present in the film box as specified
     * by the attribute
     */
    strcpy(string, "\0");
    index++;
    index2 = 0;
    imgDisplay->count = 1;
    while (index < strlen(displayFormat)) {
	i = 0;
	while ((index < strlen(displayFormat))
	       && (displayFormat[index] != ',')) {
	    string[i] = displayFormat[index];
	    i++;
	    index++;
	}
	string[i] = '\0';
	numList[index2] = atoi(string);
	imgDisplay->count *= numList[index2];
	index++;
	index2++;
    }
    if ((imgDisplay->numList
	 = malloc(index2 * sizeof(int))) != NULL) {
	for (i = 0; i < index2; i++)
	    imgDisplay->numList[i] = numList[i];
	return imgDisplay;
    }
    printf("Out of memory in decodeImageDisplayFormat\n");
    return NULL;		/* otherwise */

}

#ifdef SMM
/* getImageBoxInstanceUID
**
** Purpose:
**	To get the "i" th instance UID from the chain of instance
**	UIDs present in the response data set.
**
** Parameter Dictionary:
**	e	a DCM element structure holding the chain of UIDs
**	index	the position in the chain which is desired
** Return Values:
**	the UID value
**
** Algorithm:
**	First skip all (i-1) UIDS in the list.
**	Then obtain the ith UID by scanning characters till you
**	   reach a \ or end of list.
*/
char *
getImageBoxInstanceUID(DCM_ELEMENT * e, int index)
{
    /* all local variables are scratch variables */
    int
        i,
        j = 0;
    int
        count = 0;
    static char
        uid[DICOM_UI_LENGTH + 1];


    while (count != index + 1) {
	i = 0;
	while ((e->d.string[j] != '\0') && (e->d.string[j] != '\\')) {
	    uid[i] = e->d.string[j];
	    i++;
	    j++;
	}
	uid[i] = '\0';
	j++;
	count++;
    }
    return uid;
}

/* convertObjectToStruct
**
** Purpose:
**	Converts the DICOM Object into the Printer attribute structure.
**
** Parameter Dictionary:
**	object		Address of pointer to DICOM object containing the
**			data set.
**	printAttrib	Pointer to printer attribute structure.
**
** Return Values:
**	DCM_NORMAL on success.
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
convertObjectToStruct(DCM_OBJECT ** object, PRINTER_ATTRIBUTES * printAttrib)
{

    static PRINTER_ATTRIBUTES
        prnAttrib;		/* needed for compiler to work properly */
    static long
        printerFlag = 0x0;
    int
        pc;
    CONDITION
	cond;
    static DCM_FLAGGED_ELEMENT prnAttribElem[] = {
	{DCM_PRINTERSTATUS, DCM_CS, "Status Normal", 1,
	    sizeof(prnAttrib.printerStatus), (void *) prnAttrib.printerStatus,
	PRINTER_STATUS, &printerFlag},
	{DCM_PRINTERSTATUSINFOCODE, DCM_CS, "Info code", 1,
	    sizeof(prnAttrib.printerStatusInfoCode),
	    (void *) prnAttrib.printerStatusInfoCode,
	PRINTER_STATUSINFO, &printerFlag},
	{DCM_PRINTERNAME, DCM_LO, "Printer name", 1,
	    sizeof(prnAttrib.printerName), (void *) prnAttrib.printerName,
	PRINTER_NAME, &printerFlag},
	{DCM_IDMANUFACTURER, DCM_LO, "", 1,
	    sizeof(prnAttrib.printerManufacturer),
	    (void *) prnAttrib.printerManufacturer,
	PRINTER_MANUFACTURER, &printerFlag},
	{DCM_IDMANUFACTURERMODEL, DCM_LO, "Manufacturer Model", 1,
	    sizeof(prnAttrib.printerManufacturerModelName),
	    (void *) prnAttrib.printerManufacturerModelName,
	PRINTER_MODELNUMBER, &printerFlag},
	{DCM_ACQDEVICESERIALNUM, DCM_LO, "Device Serial #", 1,
	    sizeof(prnAttrib.printerDeviceSerialNumber),
	    (void *) prnAttrib.printerDeviceSerialNumber,
	PRINTER_SERIALNUMBER, &printerFlag},
	{DCM_ACQSOFTWAREVERSION, DCM_LO, "Sofware version", 1,
	    sizeof(prnAttrib.printerSoftwareVersion),
	    (void *) prnAttrib.printerSoftwareVersion,
	PRINTER_SOFTWAREVERSION, &printerFlag},
	{DCM_ACQDATELASTCALIBRATION, DCM_DA, "Calibration date", 1,
	    sizeof(prnAttrib.printerDateOfLastCalibration),
	    (void *) prnAttrib.printerDateOfLastCalibration,
	PRINTER_DATEOFCALIBRATION, &printerFlag},
	{DCM_ACQTIMELASTCALIBRATION, DCM_TM, "Calibration time", 1,
	    sizeof(prnAttrib.printerTimeOfLastCalibration),
	    (void *) prnAttrib.printerTimeOfLastCalibration,
	PRINTER_TIMEOFCALIBRATION, &printerFlag}
    };				/* we define all possible attributes of the
				 * printer and retrieve as many as arrived on
				 * the response message */

    /* Now convert the data set into a DCM_FLAGGED_ELEMENT */
    cond = DCM_ParseObject(object, NULL, 0, prnAttribElem,
			   DIM_OF(prnAttribElem), &pc);
    if (cond != DCM_NORMAL) {
	printf("In convertObjectToStruct, DCM_ParseObject Failed \n");
	return cond;
    }
    printf("PARSE COUNT = %d\n", pc);
    *printAttrib = prnAttrib;

    return DCM_NORMAL;
}

/* freeAll
**
** Purpose:
**	Free all occupied memory
**
** Parameter Dictionary:
**	message		address of pointer to DIMSE-N meesage
**
** Return Values:
**	success or failure
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CTNBOOLEAN
freeAll(void **message)
{
    CONDITION
	cond;
    MSG_TYPE
	* type;

    type = (MSG_TYPE *) (*message);
    switch (*type) {
    case MSG_K_N_EVENT_REPORT_REQ:
	SRV_MessageIDIn(((MSG_N_EVENT_REPORT_REQ *) (*message))->messageID);
	if (((MSG_N_EVENT_REPORT_REQ *) (*message))->dataSetType !=
	    DCM_CMDDATANULL) {
	    cond = DCM_CloseObject(
			 &((MSG_N_EVENT_REPORT_REQ *) (*message))->dataSet);
	    if (cond != DCM_NORMAL) {
		printf("In freeAll, DCM_CloseObject failed\n");
		return FALSE;
	    }
	}
	break;
    case MSG_K_N_GET_REQ:
	SRV_MessageIDIn(((MSG_N_GET_REQ *) (*message))->messageID);
	break;
    case MSG_K_N_SET_REQ:
	SRV_MessageIDIn(((MSG_N_SET_REQ *) (*message))->messageID);
	if (((MSG_N_SET_REQ *) (*message))->dataSetType !=
	    DCM_CMDDATANULL) {
	    cond = DCM_CloseObject(
				   &((MSG_N_SET_REQ *) (*message))->dataSet);
	    if (cond != DCM_NORMAL) {
		printf("In freeAll, DCM_CloseObject failed\n");
		return FALSE;
	    }
	}
	break;
    case MSG_K_N_ACTION_REQ:
	SRV_MessageIDIn(((MSG_N_ACTION_REQ *) (*message))->messageID);
	if (((MSG_N_ACTION_REQ *) (*message))->dataSetType !=
	    DCM_CMDDATANULL) {
	    cond = DCM_CloseObject(
		     &((MSG_N_ACTION_REQ *) (*message))->actionInformation);
	    if (cond != DCM_NORMAL) {
		printf("In freeAll, DCM_CloseObject failed\n");
		return FALSE;
	    }
	}
	break;
    case MSG_K_N_CREATE_REQ:
	SRV_MessageIDIn(((MSG_N_CREATE_REQ *) (*message))->messageID);
	if (((MSG_N_CREATE_REQ *) (*message))->dataSetType !=
	    DCM_CMDDATANULL) {
	    cond = DCM_CloseObject(
			       &((MSG_N_CREATE_REQ *) (*message))->dataSet);
	    if (cond != DCM_NORMAL) {
		printf("In freeAll, DCM_CloseObject failed\n");
		return FALSE;
	    }
	}
	break;
    case MSG_K_N_DELETE_REQ:
	SRV_MessageIDIn(((MSG_N_DELETE_REQ *) (*message))->messageID);
	break;
    case MSG_K_N_DELETE_RESP:
	break;
    case MSG_K_N_EVENT_REPORT_RESP:
	if (((MSG_N_EVENT_REPORT_RESP *) (*message))->dataSetType !=
	    DCM_CMDDATANULL) {
	    cond = DCM_CloseObject(
			&((MSG_N_EVENT_REPORT_RESP *) (*message))->dataSet);
	    if (cond != DCM_NORMAL) {
		printf("In freeAll, DCM_CloseObject failed\n");
		return FALSE;
	    }
	}
	break;
    case MSG_K_N_GET_RESP:
	if (((MSG_N_GET_RESP *) (*message))->dataSetType !=
	    DCM_CMDDATANULL) {
	    cond = DCM_CloseObject(
				 &((MSG_N_GET_RESP *) (*message))->dataSet);
	    if (cond != DCM_NORMAL) {
		printf("In freeAll, DCM_CloseObject failed\n");
		return FALSE;
	    }
	}
	break;
    case MSG_K_N_SET_RESP:
	if (((MSG_N_SET_RESP *) (*message))->dataSetType !=
	    DCM_CMDDATANULL) {
	    cond = DCM_CloseObject(
				 &((MSG_N_SET_RESP *) (*message))->dataSet);
	    if (cond != DCM_NORMAL) {
		printf("In freeAll, DCM_CloseObject failed\n");
		return FALSE;
	    }
	}
	break;
    case MSG_K_N_ACTION_RESP:
	if (((MSG_N_ACTION_RESP *) (*message))->dataSetType !=
	    DCM_CMDDATANULL) {
	    cond = DCM_CloseObject(
			  &((MSG_N_ACTION_RESP *) (*message))->actionReply);
	    if (cond != DCM_NORMAL) {
		printf("In freeAll, DCM_CloseObject failed\n");
		return FALSE;
	    }
	}
	break;
    case MSG_K_N_CREATE_RESP:
	if (((MSG_N_CREATE_RESP *) (*message))->dataSetType !=
	    DCM_CMDDATANULL) {
	    cond = DCM_CloseObject(
			      &((MSG_N_CREATE_RESP *) (*message))->dataSet);
	    if (cond != DCM_NORMAL) {
		printf("In freeAll, DCM_CloseObject failed\n");
		return FALSE;
	    }
	}
	break;
    default:
	printf("In freeAll, Invalid message type\n");
	return FALSE;
    }

    /* now free the rest of the structure */
    (void) MSG_Free(message);
    return TRUE;
}
#endif


/* getPrinterInstance
**
** Purpose:
**      Send the N-GET  request to the SCP to get the attributes of the
**      PRINTER SOP instance.
** Parameter Dictionary:
**      association     the key which describes the association
**      params          parameters describing the association
**      SOPClass        negotiated meta SOP class UID
**	tags		array of tags which forms the attribute list
** Return Values:
**
**	APP_FAILED
**	APP_NORMAL
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/
CONDITION
getPrinterInstance(DUL_ASSOCIATIONKEY ** association,
		   DUL_ASSOCIATESERVICEPARAMETERS * params, char *SOPClass,
		   PRN_PRINTER ** printerAttributes,
		   unsigned short *status)
{
    MSG_N_GET_REQ
	request;
    MSG_N_GET_RESP
	response;
    CONDITION
	cond;			/* condition code returned by various
				 * facilities */
    DCM_TAG
	tags[] = {		/* We request the server to provide us with
				 * the following attributes */
	DCM_PRINTERSTATUS,
	    DCM_PRINTERSTATUSINFO,
	    DCM_PRINTERNAME,
	    DCM_IDMANUFACTURER,
	    DCM_IDMANUFACTURERMODEL,
	    DCM_ACQDEVICESERIALNUM,
	    DCM_ACQSOFTWAREVERSION,
	    DCM_ACQDATELASTCALIBRATION,
	    DCM_ACQTIMELASTCALIBRATION
    };				/* we ask for all the attributes of the
				 * printer */

    /* build the N-GET request message */
    request.type = MSG_K_N_GET_REQ;
    request.messageID = SRV_MessageIDOut();
    strcpy(request.classUID, DICOM_SOPCLASSPRINTER);
    strcpy(request.requestedInstanceUID, DICOM_SOPPRINTERINSTANCE);
    strcpy(request.requestedInstanceUID, DICOM_SOPPRINTERINSTANCE);
    request.attributeList = tags;
    request.attributeCount = (int) DIM_OF(tags);

    /* invoke the N_GET service */
    cond = SRV_NGetRequest(association, params, SOPClass,
		 &request, &response, ngetCallback, printerAttributes, ".");

    if (cond != SRV_NORMAL) {
	printf("In getPrinterInstance : SRV_NGetRequest failed\n");
	return APP_FAILED;
    }
    *status = response.status;
    return APP_NORMAL;		/* on success */
}

/* ngetCallback
**
** Purpose:
**	The call back routine which should take the appropriate
**	actions on receiving the N-GET response message
**
** Parameter Dictionary:
**	ngetRequest	the N-GET request message
**	ngetResponse	the N-GET response message
**	printerAttributes
**			Address of pointer to the structure containing
**			printer attributes.
**
** Return Values:
**
**	PRN_MALLOCFAILURE
**	PRN_NORMAL
**	PRN_PARSEFAILED
**	SRV_NORMAL
**
** Notes:
**	We pass printerAttributes as void* and later cast to PRN_PRINTER**
**	to satisfy protype defined for n-get callback in dicom_services.h.
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
ngetCallback(MSG_N_GET_REQ * ngetRequest,
	     MSG_N_GET_RESP * ngetResponse,
/*	     PRN_PRINTER ** printerAttributes) */
	     void *printerAttributesPtr)
{
    CONDITION
    cond;
/* Definition and cast help satisfy prototype for n-get callback defined
** in dicom_services.h.
*/
    PRN_PRINTER **printerAttributes;
    printerAttributes = (PRN_PRINTER **) printerAttributesPtr;

    /* Now print the attributes of the printer that we have received */
    if (!silent) {
	printf("  Attributes of the printer are : - \n");
	cond = DCM_DumpElements(&ngetResponse->dataSet, 0);
	if (cond != DCM_NORMAL) {
	    printf(" In ngetCallback : DCM_DumpElements failed\n");
	    return cond;
	}
    }
    /* convert the response message received into a structure */
    cond = PRN_ParseObject(&ngetResponse->dataSet, PRN_K_BASICPRINTER,
			   (void **) printerAttributes);
    if (cond != PRN_NORMAL) {
	printf("In ngetCallback, parse failed\n");
	return cond;
    }
    return SRV_NORMAL;
}

/* createFilmSession
**
** Purpose:
**      used to send the N-CREATE request message to create a BASIC
**      FILM SESSION SOP instance.
**
** Parameter Dictionary:
**      association     key which describes the association
**      params          parameters describing the association
**      bfsAttributes   pointer to structure containing values of various
**                      attributes chosen by the user.
**      SOPClass        meta SOP class UID
**
** Return Values:
**
**	PRN_BUILDFAILED
**	PRN_ILLEGALUIDLENGTH
**	PRN_LISTCREATEFAILURE
**	PRN_LISTFAILURE
**	PRN_MALLOCFAILURE
**	PRN_NORMAL
**	SRV_CALLBACKABORTEDSERVICE
**	SRV_ILLEGALPARAMETER
**	SRV_NOCALLBACK
**	SRV_NORMAL
**	SRV_OBJECTBUILDFAILED
**	SRV_REQUESTFAILED
**	SRV_UNEXPECTEDCOMMAND
**	SRV_UNSUPPORTEDSERVICE
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/
CONDITION
createFilmSession(DUL_ASSOCIATIONKEY ** association,
		  DUL_ASSOCIATESERVICEPARAMETERS * params,
		  PRN_BASICFILMSESSION * bfsAttributes, char *SOPClass,
		  unsigned short *status)
{
    MSG_N_CREATE_REQ
	request;
    MSG_N_CREATE_RESP
	response;
    CONDITION
	cond;

    /*
     * we now build up a N-CREATE message to be sent to the server to request
     * for the creation of a BASIC FILM_SESSION SOP instance
     */
    memset(&request, 0, sizeof(MSG_N_CREATE_REQ));
    request.messageID = SRV_MessageIDOut();
    request.type = MSG_K_N_CREATE_REQ;
    strcpy(request.classUID, DICOM_SOPCLASSBASICFILMSESSION);
    strcpy(request.instanceUID, "\0");	/* Let SCP provide it */
    request.dataSetType = DCM_CMDDATAOTHER;

    /* The attribute list is stored as an object */
    cond = PRN_BuildObject(bfsAttributes, &request.dataSet);
    if (cond != PRN_NORMAL) {
	printf("In createFilmSession: failed to build Data Set\n");
	return cond;
    };

    /*
     * now use the DIMSE N-CREATE service to create the desired Basic Film
     * Session SOP instance
     */
    cond = SRV_NCreateRequest(association, params, SOPClass,
			      &request, &response, ncreateBFSCallback,
			      bfsAttributes, ".");
    if (cond != SRV_NORMAL) {
	printf("In createFilmSession : SRV_NCreateRequest failed\n");
	return cond;
    }
    *status = response.status;
    return SRV_NORMAL;		/* success */
}

/* ncreateBFSCallback
**
** Purpose:
**	Call back function for the N-CREATE request for BAsic Film Session
**
** Parameter Dictionary:
**	createRequest	N-CREATE request message for Basic Film Session
**	createResponse	N-CREATE response message for Basic Film Session
**	bfsAttrib	Pointer to structure holding attributes of
**			Basic Film Session.
**
** Return Values:
**
**	DCM_ILLEGALOBJECT
**	DCM_NORMAL
**	DCM_NULLOBJECT
**	PRN_MALLOCFAILURE
**	PRN_NORMAL
**	PRN_PARSEFAILED
**	SRV_NORMAL
**
** Notes:
**	We pass bfsAttrib as a void* and later cast to PRN_BASICFILMSESSION*
**	to satisfy prototype for n-create callback defined in dicom_services.h
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
ncreateBFSCallback(MSG_N_CREATE_REQ * createRequest,
		   MSG_N_CREATE_RESP * createResponse,
/*		   PRN_BASICFILMSESSION * bfsAttrib) */
		   void *bfsAttribPtr)
{
    CONDITION
    cond;
    static long filmSessionFlagList[] = {
	PRN_BFS_K_COPIES,
	PRN_BFS_K_PRIORITY,
	PRN_BFS_K_MEDIUMTYPE,
	PRN_BFS_K_FILMDESTINATION,
	PRN_BFS_K_FILMSESSIONLABEL,
	PRN_BFS_K_MEMORYALLOCATION,
	PRN_BFS_K_REFERENCEDBFBSEQ
    };
    static PRN_BASICFILMSESSION
        filmSession;		/* stores the copy of the attributes which
				 * SCU has */
    int
        index;
/* Definition and cast assignment help satisfy prototype for n-create callback
** defined in dicom_services.h.
*/
    PRN_BASICFILMSESSION *bfsAttrib;
    bfsAttrib = (PRN_BASICFILMSESSION *) bfsAttribPtr;

    /* save a copy of the attributes */
    filmSession = *bfsAttrib;


    /* print attribute list, status and SOP instance UID */
    if (createResponse->dataSetType != DCM_CMDDATANULL) {
	if (!silent) {
	    printf("SCU : Received the following updated BFS attributes : \n");
	    printf("SCU : Received Basic Film Session instance UID : %s\n",
		   createResponse->instanceUID);

	    cond = DCM_DumpElements(&createResponse->dataSet, 0);
	    if (cond != DCM_NORMAL) {
		printf("In ncreateBFSCallback : DCM_DumpElements failed\n");
		return cond;
	    }
	}
	(void) PRN_ParseObject(&createResponse->dataSet,
			       PRN_K_BASICFILMSESSION, (void **) &bfsAttrib);
    } else
	printf("NO DATA SET RECEIVED\n");

    MSG_DumpMessage(createResponse, stdout);
    (void) strcpy(filmSession.filmSessionSOPInstanceUID,
		  createResponse->instanceUID);

    /* find which extra attributes have been provided by the SCP */
    for (index = 0; index < (int) DIM_OF(filmSessionFlagList); index++) {
	if (bfsAttrib->bfsAttributeFlag & filmSessionFlagList[index]) {
	    /* attribute was present in the data set sent by SCP */
	    if (!(filmSession.bfsAttributeFlag &
		  filmSessionFlagList[index])) {
		/*
		 * attribute was not provided by SCU. Copy that attribute and
		 * update the flag
		 */
		filmSession.bfsAttributeFlag |= filmSessionFlagList[index];

		switch (filmSessionFlagList[index]) {
		case PRN_BFS_K_COPIES:
		    (void) strcpy(filmSession.copies,
				  bfsAttrib->copies);
		    break;
		case PRN_BFS_K_PRIORITY:
		    (void) strcpy(filmSession.priority,
				  bfsAttrib->priority);
		    break;
		case PRN_BFS_K_MEDIUMTYPE:
		    (void) strcpy(filmSession.mediumType,
				  bfsAttrib->mediumType);
		    break;
		case PRN_BFS_K_FILMDESTINATION:
		    (void) strcpy(filmSession.filmDestination,
				  bfsAttrib->filmDestination);
		    break;
		case PRN_BFS_K_FILMSESSIONLABEL:
		    (void) strcpy(filmSession.filmSessionLabel,
				  bfsAttrib->filmSessionLabel);
		    break;
		case PRN_BFS_K_MEMORYALLOCATION:
		    (void) strcpy(filmSession.memoryAllocation,
				  bfsAttrib->memoryAllocation);
		    break;
		case PRN_BFS_K_REFERENCEDBFBSEQ:
		    filmSession.referencedBFBSeq
			= bfsAttrib->referencedBFBSeq;
		    break;
		}
	    }
	}
    }

    /* copy the temporary copy back into the real structure */
    *bfsAttrib = filmSession;

    return SRV_NORMAL;
}


/* createFilmBox
**
** Purpose:
**      Describe the purpose of the function
**
** Parameter Dictionary:
**      association     key which describes the association
**      params          parameters of the association
**      bfbAttributes   pointer to structure containing attributes of the
**                      BASIC FILM BOX to be created.
**      SOPClass        negotiated meta SOP class
**
** Return Values:
**
**	PRN_BUILDFAILED
**	PRN_ILLEGALUIDLENGTH
**	PRN_LISTCREATEFAILURE
**	PRN_LISTFAILURE
**	PRN_MALLOCFAILURE
**	PRN_NORMAL
**	SRV_CALLBACKABORTEDSERVICE
**	SRV_ILLEGALPARAMETER
**	SRV_NOCALLBACK
**	SRV_NORMAL
**	SRV_OBJECTBUILDFAILED
**	SRV_REQUESTFAILED
**	SRV_UNEXPECTEDCOMMAND
**	SRV_UNSUPPORTEDSERVICE
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/
CONDITION
createFilmBox(DUL_ASSOCIATIONKEY ** association,
	      DUL_ASSOCIATESERVICEPARAMETERS * params,
	      PRN_BASICFILMBOX * bfbAttributes,
	      char *SOPClass,
	      unsigned short *status)
{
    MSG_N_CREATE_REQ
	request;
    MSG_N_CREATE_RESP
	response;
    CONDITION
	cond;
    int
        uc;


    /*
     * we now build up a N-CREATE message to be sent to the server to request
     * for the creation of a BASIC FILM BOX SOP instance
     */
    memset(&request, 0, sizeof(MSG_N_CREATE_REQ));
    request.messageID = SRV_MessageIDOut();
    request.type = MSG_K_N_CREATE_REQ;
    strcpy(request.classUID, DICOM_SOPCLASSBASICFILMBOX);
    strcpy(request.instanceUID, "\0");	/* Let SCP provide it */
    request.dataSetType = DCM_CMDDATAOTHER;

    cond = PRN_BuildObject(bfbAttributes, &request.dataSet);
    if (cond != PRN_NORMAL) {
	printf("In createFilmBox: failed to build Data Set\n");
	return cond;
    }
    /*
     * now use the DIMSE N-CREATE service to create the desired Basic Film
     * Session SOP instance
     */
    cond = SRV_NCreateRequest(association, params, SOPClass,
			      &request, &response, ncreateBFBCallback,
			      bfbAttributes, ".");
    if (cond != SRV_NORMAL) {
	printf("In createFilmBox : SRV_NCreateRequest failed\n");
	return cond;
    }
    *status = response.status;
    return SRV_NORMAL;		/* success */

}

/* ncreateBFBCallback
**
** Purpose:
**      Call back function for N-CREATE request for BASIC FILM BOX
**
** Parameter Dictionary:
**      createRequest   N-CREATE request for Basic Film Box
**      createResponse  N-CREATE response for Basic Film Box
**	bfbAttrib	Pointer to structure containing the attributes
**			of the Basic Film Box.
**
** Return Values:
**
**	DCM_ILLEGALOBJECT
**	DCM_NORMAL
**	DCM_NULLOBJECT
**	PRN_MALLOCFAILURE
**	PRN_NORMAL
**	PRN_PARSEFAILED
**	SRV_NORMAL
**
**
** Notes:
**	We pass bfbAttrib as a void* and laster cast to PRN_BASICFILMBOX*
**	to satisfy protytpe for n-crate callback defined in dicom_services.h.
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/
static CONDITION
ncreateBFBCallback(MSG_N_CREATE_REQ * createRequest,
		   MSG_N_CREATE_RESP * createResponse,
/*		   PRN_BASICFILMBOX * bfbAttrib) */
		   void *bfbAttribPtr)
{
    CONDITION
    cond;
    static filmBoxFlagList[] = {
	PRN_BFB_K_IMAGEDISPLAYFORMAT,
	PRN_BFB_K_ANNOTATIONDISPLAYFORMATID,
	PRN_BFB_K_FILMORIENTATION,
	PRN_BFB_K_FILMSIZEID,
	PRN_BFB_K_MAGNIFICATIONTYPE,
	PRN_BFB_K_SMOOTHINGTYPE,
	PRN_BFB_K_BORDERDENSITY,
	PRN_BFB_K_EMPTYIMAGEDENSITY,
	PRN_BFB_K_MINDENSITY,
	PRN_BFB_K_MAXDENSITY,
	PRN_BFB_K_TRIM,
	PRN_BFB_K_CONFIGURATIONINFO,
	PRN_BFB_K_REFERENCEDBASICFILMSESSIONSEQ,
	PRN_BFB_K_REFERENCEDBASICIMAGEBOXSEQ,
	PRN_BFB_K_REFERENCEDBASICANNOTATIONBOXSEQ,
	PRN_BFB_K_REFPRINTJOBSEQ
    };
    static PRN_BASICFILMBOX
        filmBox;		/* stores copy of SCU's copy of attributes */
    int
        index;
/* Definition and cast assignment below help satisfy prototype for n-create
** callback defined in dicom_services.h.
*/
    PRN_BASICFILMBOX *bfbAttrib;
    bfbAttrib = (PRN_BASICFILMBOX *) bfbAttribPtr;

    /* make a copy of the attributes */
    filmBox = *bfbAttrib;

    /* print attribute list, status and SOP instance UID */
    if (createResponse->dataSetType != DCM_CMDDATANULL) {
	if (!silent) {
	    printf("SCU : Received N-CREATE Response message\n");
	    printf("SCU : Received Basic Film Box instance UID : %s\n",
		   createResponse->instanceUID);
	    printf("SCU : Attributes received via Response message:-\n");
	    cond = DCM_DumpElements(&createResponse->dataSet, 0);
	    if (cond != DCM_NORMAL) {
		printf("In ncreateBFBCallback : DCM_DumpElements failed\n");
		return cond;
	    }
	}
	cond = PRN_ParseObject(&createResponse->dataSet,
			       PRN_K_BASICFILMBOX, (void **) &bfbAttrib);
	if (cond != PRN_NORMAL) {
	    printf("In ncreateBFBCallback, PRN_ParseObject failed \n");
	    return cond;
	}
    } else
	printf("NO DATASET received for Basic Film Box\n");

    MSG_DumpMessage(createResponse, stdout);

    (void) strcpy(filmBox.filmBoxSOPClassUID, createResponse->classUID);
    (void) strcpy(filmBox.filmBoxSOPInstanceUID,
		  createResponse->instanceUID);

    /* find which extra attributes have been provided by the SCP */
    for (index = 0; index < (int) DIM_OF(filmBoxFlagList); index++) {
	if (bfbAttrib->bfbAttributeFlag & filmBoxFlagList[index]) {
	    /* attribute was present in the data set sent by SCP */
	    if (!(filmBox.bfbAttributeFlag &
		  filmBoxFlagList[index])) {
		/*
		 * attribute was not provided by SCU. Copy that attribute and
		 * update the flag
		 */
		filmBox.bfbAttributeFlag |= filmBoxFlagList[index];

		switch (filmBoxFlagList[index]) {
		case PRN_BFB_K_IMAGEDISPLAYFORMAT:
		    (void) strcpy(filmBox.imageDisplayFormat,
				  bfbAttrib->imageDisplayFormat);
		    break;
		case PRN_BFB_K_ANNOTATIONDISPLAYFORMATID:
		    (void) strcpy(filmBox.annotationDisplayFormatID,
				  bfbAttrib->annotationDisplayFormatID);
		    break;
		case PRN_BFB_K_FILMORIENTATION:
		    (void) strcpy(filmBox.filmOrientation,
				  bfbAttrib->filmOrientation);
		    break;
		case PRN_BFB_K_FILMSIZEID:
		    (void) strcpy(filmBox.filmSizeID,
				  bfbAttrib->filmSizeID);
		    break;
		case PRN_BFB_K_MAGNIFICATIONTYPE:
		    (void) strcpy(filmBox.magnificationType,
				  bfbAttrib->magnificationType);
		    break;
		case PRN_BFB_K_SMOOTHINGTYPE:
		    (void) strcpy(filmBox.smoothingType,
				  bfbAttrib->smoothingType);
		    break;
		case PRN_BFB_K_BORDERDENSITY:
		    (void) strcpy(filmBox.borderDensity,
				  bfbAttrib->borderDensity);
		    break;
		case PRN_BFB_K_EMPTYIMAGEDENSITY:
		    (void) strcpy(filmBox.emptyImageDensity,
				  bfbAttrib->emptyImageDensity);
		    break;
#if STANDARD_VERSION < VERSION_AUG1993
		case PRN_BFB_K_MINDENSITY:
		    (void) strcpy(filmBox.minDensity, bfbAttrib->minDensity);
		    break;
		case PRN_BFB_K_MAXDENSITY:
		    (void) strcpy(filmBox.maxDensity, bfbAttrib->maxDensity);
		    break;
#else
		case PRN_BFB_K_MINDENSITY:
		    filmBox.minDensity = bfbAttrib->minDensity;
		    break;
		case PRN_BFB_K_MAXDENSITY:
		    filmBox.maxDensity = bfbAttrib->maxDensity;
		    break;
#endif
		case PRN_BFB_K_TRIM:
		    (void) strcpy(filmBox.trim,
				  bfbAttrib->trim);
		    break;
		case PRN_BFB_K_CONFIGURATIONINFO:
		    (void) strcpy(filmBox.configurationInfo,
				  bfbAttrib->configurationInfo);
		    break;
		case PRN_BFB_K_REFERENCEDBASICFILMSESSIONSEQ:
		    (void) strcpy(filmBox.referencedFilmSessionSOPClassUID,
			       bfbAttrib->referencedFilmSessionSOPClassUID);
		    (void) strcpy(filmBox.referencedFilmSessionSOPInstanceUID,
			    bfbAttrib->referencedFilmSessionSOPInstanceUID);
		    break;
		case PRN_BFB_K_REFERENCEDBASICIMAGEBOXSEQ:
		    filmBox.referencedBIBSeq = bfbAttrib->referencedBIBSeq;
		    break;
		case PRN_BFB_K_REFERENCEDBASICANNOTATIONBOXSEQ:
		    filmBox.referencedBABSeq = bfbAttrib->referencedBABSeq;
		    break;
		case PRN_BFB_K_REFPRINTJOBSEQ:
		    (void) strcpy(filmBox.refPrintJobClassUID,
				  bfbAttrib->refPrintJobClassUID);
		    (void) strcpy(filmBox.refPrintJobInstanceUID,
				  bfbAttrib->refPrintJobInstanceUID);
		    break;
		}
	    }
	}
    }

    /* copy back the attributes */
    *bfbAttrib = filmBox;

    return SRV_NORMAL;
}

/* setImageBox
**
** Purpose:
**      sends a request to set the attributes of the Basic Image Box
**
** Parameter Dictionary:
**      association     key which descibes the association
**      params          parameters which describe the association
**      SOPClass        meta SOP class
**	imageBox	Pointer to atructure containing the image attributes.
**
** Return Values:
**
**	DCM_ILLEGALOBJECT
**	DCM_LISTFAILURE
**	DCM_NORMAL
**	DCM_NULLOBJECT
**	PRN_BUILDFAILED
**	PRN_ILLEGALUIDLENGTH
**	PRN_LISTCREATEFAILURE
**	PRN_LISTFAILURE
**	PRN_MALLOCFAILURE
**	PRN_NORMAL
**	SRV_CALLBACKABORTEDSERVICE
**	SRV_ILLEGALPARAMETER
**	SRV_NOCALLBACK
**	SRV_NORMAL
**	SRV_OBJECTBUILDFAILED
**	SRV_REQUESTFAILED
**	SRV_UNEXPECTEDCOMMAND
**	SRV_UNSUPPORTEDSERVICE
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/

CONDITION
setImageBox(DUL_ASSOCIATIONKEY ** association,
	    DUL_ASSOCIATESERVICEPARAMETERS * params,
	    char *SOPClass,
	    PRN_BASICIMAGEBOX * imageBox,
	    unsigned short *status)
{
    MSG_N_SET_REQ
	request;
    MSG_N_SET_RESP
	response;
    DCM_ELEMENT
	* e;
    DCM_SEQUENCE_ITEM
	* item;
    CONDITION
	cond;
    int
        uc;
    void
       *ctx = NULL;
    unsigned long
        length;


    /*
     * we now build up a N_SET message to be sent to the server to request
     * for setting up the default attributes of the basic image box instance
     */
    request.messageID = SRV_MessageIDOut();
    request.type = MSG_K_N_SET_REQ;
    strcpy(request.classUID, DICOM_SOPCLASSBASICGREYSCALEIMAGEBOX);
    strcpy(request.instanceUID, imageBox->imageBoxSOPInstanceUID);
    request.dataSetType = DCM_CMDDATAOTHER;

    cond = PRN_BuildObject(imageBox, &request.dataSet);
    if (cond != PRN_NORMAL) {
	printf("In setImageBox : PRN_CreateObject failed\n");
	return cond;
    }
    if (!silent) {
	printf("Dumping Request data set\n");
	(void) DCM_DumpElements(&request.dataSet, 0);
    }
    /*
     * now use the DIMSE N-SET service to set the mandatory attribute and ask
     * the server (SCP) to set the other default values for the Basic Image
     * Box SOP instance
     */
    cond = SRV_NSetRequest(association, params, SOPClass,
		   &request, &response, setImageBoxCallback, imageBox, ".");
    if (cond != SRV_NORMAL) {
	printf("In setImageBox : SRV_NSetRequest failed\n");
	return cond;
    }
    cond = DCM_CloseObject(&request.dataSet);
    if (cond != DCM_NORMAL) {
	printf("In setImageBox : DCM_CloseObject failed\n");
	return cond;
    }
    *status = response.status;
    return SRV_NORMAL;
}


/* setImageBoxCallback
**
** Purpose:
**	Callback routine to deal with the N-SET response message.
**
** Parameter Dictionary:
**	setRequest	Pointer to the N-SET request message
**	setResponse	Pointer to the N-SET response message
**	bibAttrib	Pointer to structure containing the attributes
**			of the Basic Image Box.
**
** Return Values:
**
**	DCM_ILLEGALOBJECT
**	DCM_NORMAL
**	DCM_NULLOBJECT
**	PRN_MALLOCFAILURE
**	PRN_NORMAL
**	PRN_PARSEFAILED
**	SRV_NORMAL
**
** Notes:
**	We pass bibAttrib as a void* and later cast to PRN_BASICIMAGEBOX*
**	to satisfy the prototype defined for an n-set operation in
**	dicom_services.h.
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
setImageBoxCallback(MSG_N_SET_REQ * setRequest,
		    MSG_N_SET_RESP * setResponse,
/*		    PRN_BASICIMAGEBOX * bibAttrib) */
		    void *bibAttribPtr)
{
    CONDITION
    cond;
    static long imageBoxFlagList[] = {
	PRN_BIB_K_IMAGEPOSITION,
	PRN_BIB_K_POLARITY,
	PRN_BIB_K_MAGNIFICATIONTYPE,
	PRN_BIB_K_SMOOTHINGTYPE,
	PRN_BIB_K_REQUESTEDIMAGESIZE,
	PRN_BIB_K_IMAGEMODULE,
	/* These are needed for older versions */
	PRN_BIB_K_MAGNIFICATIONFACTOR,
	PRN_BIB_K_IMAGEBOXSOPCLASSUID
    };
    static PRN_BASICIMAGEBOX
        imageBox;		/* to store a copy of attributes */
    int
        index;
/* Definition and cast assignment help satisfy prototype defined for n-set
** operation in dicom_services.h.
*/
    PRN_BASICIMAGEBOX *bibAttrib;
    bibAttrib = (PRN_BASICIMAGEBOX *) bibAttribPtr;

    /* make a copy of the attributes */
    imageBox = *bibAttrib;

    /* print the attribute list, status, SOP instance UID */
    if (setResponse->dataSetType != DCM_CMDDATANULL) {
	if (!silent) {
	    printf("Image box instance UID : %s\n", setResponse->instanceUID);
	    printf("Attributes :\n");
	    cond = DCM_DumpElements(&setResponse->dataSet, 0);
	    if (cond != DCM_NORMAL) {
		printf("In setImageBoxCallback : DCM_DumpElements failed\n");
		return cond;
	    }
	}
	(void) PRN_ParseObject(&setResponse->dataSet, PRN_K_BASICIMAGEBOX,
			       (void **) &bibAttrib);
    } else
	printf("NO DATA SET RECEIVED in N-SET response\n");
    MSG_DumpMessage((void *) setResponse, stdout);

    (void) strcpy(imageBox.imageBoxSOPClassUID,
		  DICOM_SOPCLASSBASICGREYSCALEIMAGEBOX);
    (void) strcpy(imageBox.imageBoxSOPInstanceUID,
		  setRequest->instanceUID);
    /* find which extra attributes have been provided by the SCP */
    for (index = 0; index < (int) DIM_OF(imageBoxFlagList); index++) {
	if (bibAttrib->bibAttributeFlag & imageBoxFlagList[index]) {
	    /* attribute was present in the data set sent by SCP */
	    if (!(imageBox.bibAttributeFlag &
		  imageBoxFlagList[index])) {
		/*
		 * attribute was not provided by SCU. Copy that attribute and
		 * update the flag
		 */
		imageBox.bibAttributeFlag |= imageBoxFlagList[index];

		switch (imageBoxFlagList[index]) {
		case PRN_BIB_K_IMAGEPOSITION:
		    imageBox.imagePosition =
			bibAttrib->imagePosition;
		    break;
		case PRN_BIB_K_POLARITY:
		    (void) strcpy(imageBox.polarity,
				  bibAttrib->polarity);
		    break;
		case PRN_BIB_K_MAGNIFICATIONTYPE:
		    (void) strcpy(imageBox.magnificationType,
				  bibAttrib->magnificationType);
		    break;
		case PRN_BIB_K_SMOOTHINGTYPE:
		    (void) strcpy(imageBox.smoothingType,
				  bibAttrib->smoothingType);
		    break;
#if STANDARD_VERSION < VERSION_AUG1993
		case PRN_BIB_K_MAGNIFICATIONFACTOR:
		    (void) strcpy(imageBox.magnificationFactor,
				  bibAttrib->magnificationFactor);
		    break;
		case PRN_BIB_K_IMAGEBOXSOPCLASSUID:
		    (void) strcpy(imageBox.imageBoxSOPClassUID,
				  bibAttrib->imageBoxSOPClassUID);
		    break;
#else
		case PRN_BIB_K_REQUESTEDIMAGESIZE:
		    (void) strcpy(imageBox.requestedImageSize,
				  bibAttrib->requestedImageSize);
		    break;
#endif
		case PRN_BIB_K_IMAGEMODULE:
		    imageBox.referencedImageSequence
			= bibAttrib->referencedImageSequence;
		    imageBox.object = bibAttrib->object;
		    break;

		}
	    }
	}
    }
    /* copy back the attributes */
    *bibAttrib = imageBox;

    return SRV_NORMAL;
}

/* sendFilmBoxPrint
**
** Purpose:
**      send N-ACTION request to request SCP to print the images
**
** Parameter Dictionary:
**      association     key which descibes the association
**      params          parameters which describe the association
**      SOPClass        meta SOP class
**      classUID        instance of that classUID sends action request
**	filmBox		Pointer to structure holding attributes of
**			Basic Film Box.
**
** Return Values:
**
**	SRV_CALLBACKABORTEDSERVICE
**	SRV_ILLEGALPARAMETER
**	SRV_NOCALLBACK
**	SRV_NORMAL
**	SRV_OBJECTBUILDFAILED
**	SRV_REQUESTFAILED
**	SRV_UNEXPECTEDCOMMAND
**	SRV_UNSUPPORTEDSERVICE
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/

CONDITION
sendFilmBoxPrint(DUL_ASSOCIATIONKEY ** association,
		 DUL_ASSOCIATESERVICEPARAMETERS * params,
		 char *SOPClass, char *classUID, PRN_BASICFILMBOX * filmBox,
		 unsigned short *status)
{
    CONDITION
	cond;
    MSG_N_ACTION_REQ
	request;
    MSG_N_ACTION_RESP
	response;
    void
       *ctx = NULL;

    /* there is no data set to be sent to the SCP */
    request.messageID = SRV_MessageIDOut();
    request.dataSetType = DCM_CMDDATANULL;
    request.actionTypeID = PRN_K_FILMBOX_ACTION_PRINT;
    request.type = MSG_K_N_ACTION_REQ;
    request.actionInformation = NULL;
    strcpy(request.classUID, DICOM_SOPCLASSBASICFILMBOX);
    strcpy(request.instanceUID, filmBox->filmBoxSOPInstanceUID);

    /*
     * now use the DIMSE N-ACTION service to print the images in the Film Box
     * instance
     */
    cond = SRV_NActionRequest(association, params, SOPClass,
			      &request, &response, filmBoxPrintCallback,
			      filmBox, ".");
    if (cond != SRV_NORMAL) {
	printf("In sendFilmBoxPrint : SRV_NActionRequest failed\n");
	return cond;
    }
    *status = response.status;
    return SRV_NORMAL;
}


/* filmBoxPrintCallback
**
** Purpose:
**	Callback function to deal with the N-ACTION response message
**
** Parameter Dictionary:
**	actionRequest	Pointer to the N-ACTION request message
**	actionResponse	Pointer to the N-ACTION response message
**	filmBox		Pointer to the Basic Film Box structure
**
** Return Values:
**
**	SRV_CALLBACKABORTEDSERVICE
**	SRV_NORMAL
**
** Notes:
**	We pass filBox as a void* and last cast to PRN_BASICFILMBOX* to
**	satisfy the prototype for an n-action callback defined in
**	dicom_services.h.
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
filmBoxPrintCallback(MSG_N_ACTION_REQ * actionRequest,
		     MSG_N_ACTION_RESP * actionResponse,
/*		     PRN_BASICFILMBOX * filmBox) */
		     void *filmBoxVoid)
{
    CONDITION
    cond;
    PRN_BASICFILMBOX
	localFilmBox,
	*filmBoxPtr;
/* Definition and cast assignment help satisfy prototype for n-action
** callback defined in dicom_services.h.
*/
    PRN_BASICFILMBOX *filmBox;
    filmBox = (PRN_BASICFILMBOX *) filmBoxVoid;

    if (actionResponse->status == MSG_K_SUCCESS) {
	printf("SCU : Received N-ACTION Response, status success\n");
	/* verify if the SCP supports a PRINT JOB SOP class */
	if (actionResponse->dataSetType != DCM_CMDDATANULL) {
	    if (!silent) {
		/* now dump the information */
		cond = DCM_DumpElements(&actionResponse->actionReply, 0);
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
#if STANDARD_VERSION >= VERSION_AUG1993
	    if (!(localFilmBox.bfbAttributeFlag & PRN_BFB_K_REFPRINTJOBSEQ)) {
		printf("In filmBoxPrintCallback, PRN Parse Failed\n");
		return SRV_CALLBACKABORTEDSERVICE;
	    }
	    strcpy(filmBox->refPrintJobClassUID,
		   localFilmBox.refPrintJobClassUID);
	    strcpy(filmBox->refPrintJobInstanceUID,
		   localFilmBox.refPrintJobInstanceUID);
	    filmBox->bfbAttributeFlag |= PRN_BFB_K_REFPRINTJOBSEQ;
#endif
	} else {
	    printf(" NO ACTION REPLY RECEIVED\n");
	}
    }
    return SRV_NORMAL;
}

/* sendDeleteRequest
**
** Purpose:
**      send the N-DELETE request
**
** Parameter Dictionary:
**      association     key which descibes the association
**      params          parameters which describe the association
**      SOPClass        meta SOP class
**      classUID        classUID sends action request
**	instanceUID	instance of that class which actually requests.
**
** Return Values:
**
**	SRV_CALLBACKABORTEDSERVICE
**	SRV_ILLEGALPARAMETER
**	SRV_NOCALLBACK
**	SRV_NORMAL
**	SRV_OBJECTBUILDFAILED
**	SRV_REQUESTFAILED
**	SRV_UNEXPECTEDCOMMAND
**	SRV_UNSUPPORTEDSERVICE
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/
CONDITION
sendDeleteRequest(DUL_ASSOCIATIONKEY ** association,
		  DUL_ASSOCIATESERVICEPARAMETERS * params,
		  char *SOPClass, char *classUID, char *instanceUID,
		  unsigned short *status)
{
    CONDITION
	cond;
    MSG_N_DELETE_REQ
	request;
    MSG_N_DELETE_RESP
	response;
    void
       *ctx = NULL;

    /* there is no data set to be sent to the SCP */
    request.dataSetType = DCM_CMDDATANULL;
    request.messageID = SRV_MessageIDOut();
    request.type = MSG_K_N_DELETE_REQ;

    /* delete request could be for Film Session or Film Box */
    strcpy(request.classUID, classUID);
    strcpy(request.instanceUID, instanceUID);

    /*
     * now use the DIMSE N-DELETE service to delete the current Film Box
     * instance
     */
    cond = SRV_NDeleteRequest(association, params, SOPClass,
			    &request, &response, ndeleteCallback, ctx, ".");
    if (cond != SRV_NORMAL) {
	printf("In sendDeleteRequest : SRV_NDeleteRequest failed\n");
	return cond;
    }
    *status = response.status;
    return SRV_NORMAL;		/* otherwise */
}

/* ndeleteCallback
**
** Purpose:
**      Callback function for N-DELETE request
**
** Parameter Dictionary:
**      deleteRequest   N-DELETE request message
**      deleteResponse  N-DELETE response message
**      ctx             context information
**
** Return Values:
**	APP_FAILED
**	SRV_NORMAL
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/
static CONDITION
ndeleteCallback(MSG_N_DELETE_REQ * deleteRequest,
		MSG_N_DELETE_RESP * deleteResponse, void *ctx)
{
    printf("SCU : Received N-DELETE Response message\n");
    if (deleteResponse->status == DCM_STATUS_SUCCESS) {
	if (strcmp(deleteResponse->classUID,
		   DICOM_SOPCLASSBASICFILMSESSION) == 0)
	    printf("FILM SESSION DELETED\n");
	else if (strcmp(deleteResponse->classUID,
			DICOM_SOPCLASSBASICFILMBOX) == 0)
	    printf("FILM BOX DELETED\n");
    } else {
	printf("In ndeleteCallback, N-DELETE Failed\n");
	return APP_FAILED;
    }
    return SRV_NORMAL;
}

/* getImageData
**
** Purpose:
**	Describe the purpose of the function
**
** Parameter Dictionary:
**	dbkey		Name of the database to be queried
**	imageUID	UID of the image whose data is to be found.
**	imageObject	The pixel data corresponding to the UID in the
**			form of a DICOM object to be returned.
**
** Return Values:
**
**	DB_BADQUERY
**	DB_LOCKERROR
**	DB_NOMEMORY
**	DB_NOMORE
**	DB_NORMAL
**	DB_NOTOPENED
**	DB_OPENERROR
**	DB_READERROR
**	DCM_CREATEOBJECTFAILED
**	DCM_ELEMENTOUTOFORDER
**	DCM_FILEACCESSERROR
**	DCM_FILEOPENFAILED
**	DCM_ILLEGALOPTION
**	DCM_LISTFAILURE
**	DCM_NORMAL
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
getImageData(char *dbkey, char *imageUID, DCM_OBJECT ** imageObject)
{
    CONDITION
	cond;
    short
        dbid;
    Query
	query, retInfo;


    /* open the data base file for querying */
    cond = DB_Open(dbkey, &dbid);
    if (cond != DB_NORMAL) {
	printf("In getImageData, DB_Open failed\n");
	COND_DumpConditions();
	return cond;
    }
    /*
     * Given the image UID, query the data base to get the required file name
     * storing the image data.
     * 
     * First fill up the pattern to be matched in the query structure.
     */
    query.Image.Query_Flag = DB_K_QIMAGEUID;
    /* clear all flags */
    query.Patient.Query_Flag = 0;
    query.Study.Query_Flag = 0;
    query.Series.Query_Flag = 0;
    query.QueryState = DB_K_LEVELIMAGE;
    (void) strcpy(query.Image.ImageUID, imageUID);
    printf("imageUID = %s\n", query.Image.ImageUID);
    cond = DB_Query(dbid, &query, &retInfo);
    if (cond != DB_NORMAL) {
	(void) DB_Close(dbid);
	printf("In getImageData, DB_Query failed\n");
	COND_DumpConditions();
	return cond;
    }
    (void) DB_Close(dbid);

    /* extract the image data file name and convert it into DICOM Object */
    cond = DCM_OpenFile(retInfo.Image.FileName, DCM_ORDERLITTLEENDIAN,
			imageObject);
    if (cond != DCM_NORMAL) {
	printf("In getImageData, DCM_OpenFile failed\n");
	COND_DumpConditions();
	return cond;
    }
    return DCM_NORMAL;		/* success */
}
