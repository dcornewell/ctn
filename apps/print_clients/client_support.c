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
**			sendGetPrinterInstance
**			ngetCallback
**			sendCreateFilmSession
**			ncreateBFSCallback
**			sendCreateFilmBox
**			ncreateBFBCallback
**			sendSetImageBox
**			nsetBIBCallback
**			sendActionRequest
**			nactionBFBCallback
**			sendDeleteRequest
**			ndeleteCallback
**			getImageBoxInstanceUID
**			decodeImageDisplayFormat
**
** Author, Date:	Aniruddha S. Gokhale, June 9, 1993
** Intent:
**			This module implements the SCU of the BASIC
**			Print Management SOP Class.
** Usage:
** Options:
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:51:25 $
** Source File:		$Source: /home/smm/ctn/ctn/apps/print_clients/client_support.c,v $
** Revision:		$Revision: 1.3 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.3 $ $RCSfile: client_support.c,v $";

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
#include "dicom_n_printer.h"
#include "print_client.h"

/* prototype declarations of private functions used */

static CONDITION
ngetCallback(MSG_N_GET_REQ * ngetRequest,
	     MSG_N_GET_RESP * ngetResponse,
	     void *ctx);
static CONDITION
ncreateBFSCallback(MSG_N_CREATE_REQ * createRequest,
		   MSG_N_CREATE_RESP * createResponse,
		   void *ctx);
static CONDITION
ncreateBFBCallback(MSG_N_CREATE_REQ * createRequest,
		   MSG_N_CREATE_RESP * createResponse,
		   void *ctx);
static int
sendSetImageBox(DUL_ASSOCIATIONKEY ** association,
		DUL_ASSOCIATESERVICEPARAMETERS * params,
		char *SOPClass,
		char *uid,
		int position,
		MSG_N_SET_REQ * request,
		MSG_N_SET_RESP ** response);
static CONDITION
nsetBIBCallback(MSG_N_SET_REQ * setRequest,
		MSG_N_SET_RESP * setResponse,
		void *ctx);
static int
sendActionRequest(DUL_ASSOCIATIONKEY ** association,
		  DUL_ASSOCIATESERVICEPARAMETERS * params,
		  char *SOPClass,
		  MSG_N_CREATE_RESP * responseFilmBox,
		  MSG_N_ACTION_REQ * request,
		  MSG_N_ACTION_RESP ** response);
static CONDITION
nactionBFBCallback(MSG_N_ACTION_REQ * actionRequest,
		   MSG_N_ACTION_RESP * actionResponse,
		   void *ctx);
static int
sendDeleteRequest(DUL_ASSOCIATIONKEY ** association,
		  DUL_ASSOCIATESERVICEPARAMETERS * params,
		  char *SOPClass,
		  char *classUID,
		  char *instanceUID,
		  MSG_N_DELETE_REQ * request,
		  MSG_N_DELETE_RESP ** response);
static CONDITION
ndeleteCallback(MSG_N_DELETE_REQ * deleteRequest,
		MSG_N_DELETE_RESP * deleteResponse,
		void *ctx);
char
   *getImageBoxInstanceUID(DCM_ELEMENT * e, int index);
IMG_DISPLAYFORMAT
* decodeImageDisplayFormat(char *displayFormat);
static int
    freeAll(void **message);
static CONDITION
convertObjectToStruct(DCM_OBJECT ** object,
		      PRINTER_ATTRIBUTES * printAttrib);
/* GLOBAL DATA STRUCTURES USED */
static char bfsInstanceUID[DICOM_UI_LENGTH + 1];	/* stores the instance
							 * UID of the parent
							 * FILM SESSION instance */
static char bfbInstanceUID[DICOM_UI_LENGTH + 1];	/* stores the instance
							 * UID of the last
							 * created FILM BOX
							 * instance */


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
				 NULL, 10, DUL_ORDERBIGENDIAN, network);
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
**	association	The association key
**
** Return Values:
**	DUL_NORMAL if association is established. As a side effect, it
**	also returns the association key.
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
establishAssociation(DUL_ASSOCIATESERVICEPARAMETERS * params,
		     DUL_NETWORKKEY ** network,
		     char *SOPClass,
		     DUL_ASSOCIATIONKEY ** association)
{
    CONDITION
	cond;

    /* now negotiate for Gray Scale Print Management Meta SOP class */
    strcpy(SOPClass, DICOM_SOPCLASSGREYPRINTMGMTMETA);
    cond = SRV_RequestServiceClass(SOPClass, DUL_SC_ROLE_DEFAULT, params);
    if (cond != SRV_NORMAL) {
	printf("Basic Gray Print Management not supported\n");
	COND_DumpConditions();
	return cond;
    }
#ifdef ASG
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
#ifdef ASG
/* printMgmtSCU
**
** Purpose:
**	Actual operation of an SCU of the Print Management SOP Class
** Parameter Dictionary:
**	network		key which describes the network connection
**	params		describes the association parameters
**	association	key which describes the association
**	SOPClass	SOP class UID of the meta SOP class for which the
**			association is established.
**	films		number of films i.e. FILM BOX instances
**	format		image display format
**
** Return Values:
**	None.
** Algorithm:
**	If association established then
**	   Get Printer attributes
**	   Create BASIC FILM SESSION SOP instance
**         For (each film of the film session)
**		Create a BASIC FILM BOX SOP instance
**		For (every image in the FILM BOX)
**		   Set attributes of the BASIC IMAGE BOX SOP instance
**		If (films not collated)
**		   Send action request
**		   Delete the film box instance
**	   If (films are collated)
**		Send action request
**		Delete the film session instance
**	   Service any event-report requests
*/

static void
printMgmtSCU(DUL_NETWORKKEY ** network,
	     DUL_ASSOCIATESERVICEPARAMETERS * params,
	     DUL_ASSOCIATIONKEY ** association,
	     char *SOPClass,
	     int films,
	     char *format)
{
    CONDITION
    cond;
    MSG_N_GET_REQ
	getRequest;
    MSG_N_GET_RESP
	* getResponse;
    MSG_N_CREATE_REQ
	createRequest, createRequestFilmBox;
    MSG_N_CREATE_RESP
	* createResponse, *createResponseFilmBox;
    MSG_N_SET_REQ
	setRequest;
    MSG_N_SET_RESP
	* setResponse;
    MSG_N_ACTION_REQ
	actionRequest;
    MSG_N_ACTION_RESP
	* actionResponse;
    MSG_N_DELETE_REQ
	deleteRequest;
    MSG_N_DELETE_RESP
	* deleteResponse;
    int
        index;
    int
        position;
    IMG_DISPLAYFORMAT
	* imgDisplayFormat;
    DCM_ELEMENT
	e[] = {
	{
	    DCM_BFBREFBASICIMAGEBOXUIDS, DCM_UI, "", 1, DICOM_UI_LENGTH, ""
	}
    };

    /*
     * the next step is to get the SOP instance UID of the Printer SOP class
     * from the SCP using the DIMSE N_GET service
     */

    printf("\n\n");
    printf("SCU : Sending N-GET Request to get printer attributes\n\n");
    if (sendGetPrinterInstance(association, params, SOPClass,
			       &getRequest, &getResponse) == 0) {
	printf("In printMgmtSCU : sendGetPrinterInstance failed\n");
	myExit(association);
    }
    /* Now create a film session SOP instance */
    printf("SCU : Sending N-CREATE Request to create BASIC FILM SESSION\n\n");
    if (sendCreateFilmSession(association, params, SOPClass,
			      &createRequest, &createResponse) == 0) {
	printf("IN printMgmtSCU, sendCreateFilmSession failed\n");
	myExit(association);
    }
    /*
     * It is necessary for the FILM BOXES to know who their parent FILM
     * SESSION instance is. So we pass this information as a parameter to the
     * sendCreateFilmBox routine.
     */

    /* for every film in the film session, create a Film Box SOP instance */
    for (index = 0; index < films; index++) {
	/* create a Film Box SOP instance */
	printf("SCU : Sending N-CREATE Request to create BASIC FILM BOX\n\n");
	if (sendCreateFilmBox(association, params, SOPClass,
			      createResponse->instanceUID, format,
		      &createRequestFilmBox, &createResponseFilmBox) == 0) {
	    printf("In printMgmtSCU, sendCreateFilmBox failed\n");
	    myExit(association);
	}
	/*
	 * we now acquire the instance UIDs of the children Basic Image Box
	 * instances from the response message received by sendCreateFilmBox
	 */
	e[0].length =
	    999 * (DICOM_UI_LENGTH + 2);	/* assign enough space */
	cond = DCM_ParseObject(&createResponseFilmBox->dataSet,
			       e, 1, NULL, 0, NULL);	/* look for UID list in
							 * the response data set */
	if (cond != DCM_NORMAL) {
	    printf("In printMgmtSCU, DCM_ParseObject failed\n");
	    myExit(association);
	}
	/*
	 * now find the image display format from the command line argument
	 */
	imgDisplayFormat = decodeImageDisplayFormat(format);
	if (imgDisplayFormat == NULL) {
	    printf("In printMgmtSCU, decodeImageDisplayFormat failed\n");
	    myExit(association);
	}
	/*
	 * for every image box created by the film box, we set the attributes
	 * of the image box
	 */
	for (position = 0; position < imgDisplayFormat->count; position++) {
	    printf("SCU : Sending N-SET Request to set attributes of BASIC ");
	    printf("IMAGE BOX\n\n");
	    if (sendSetImageBox(association, params, SOPClass,
				getImageBoxInstanceUID(e, position),
				position + 1,
				&setRequest, &setResponse) == 0) {
		printf("In printMgmtSCU, sendSetImageBox failed\n");
		myExit(association);
	    }
	}
	/*
	 * Now we ask the SCP to print the images in the current film box
	 * instance
	 */
	printf("SCU : Sending N-ACTION request to print the BASIC FILM BOX\n\n");
	if (sendActionRequest(association, params, SOPClass,
	     createResponseFilmBox, &actionRequest, &actionResponse) == 0) {
	    printf("In printMgmtSCU, sendActionRequest failed\n");
	    myExit(association);
	}
	/* now we ask the SCP to delete the current Film Box instance */
	printf("SCU : Sending N-DELETE Request to delete current FILM BOX\n\n");
	if (sendDeleteRequest(association, params, SOPClass,
			      createResponseFilmBox->classUID,
			      createResponseFilmBox->instanceUID,
			      &deleteRequest, &deleteResponse) == 0) {
	    printf("In printMgmtSCU, sendDeleteRequest failed\n");
	    myExit(association);
	}
    }

    /* service any event-report requests asynchronously */
}
#endif
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
**	a pointer to aa data structure that holds the "type" of the
**	image display e.g. STANDARD and the number of images arranged
**	in row-major or column major order.
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/
IMG_DISPLAYFORMAT
* decodeImageDisplayFormat(char *displayFormat)
{
    char
        string[DICOM_LO_LENGTH + 1];	/* scratch variable */
    unsigned int
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
/* sendGetPrinterInstance
**
** Purpose:
**	Send the N-GET  request to the SCP to get the attributes of the
**	PRINTER SOP instance.
** Parameter Dictionary:
**	association	the key which describes the association
**	params		parameters describing the association
**	SOPClass	negotiated meta SOP class UID
**	printAttrib	Pointer to structure containing PRINTER attributes
**
** Return Values:
**	success or failure
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
sendGetPrinterInstance(DUL_ASSOCIATIONKEY ** association,
		       DUL_ASSOCIATESERVICEPARAMETERS * params,
		       char *SOPClass,
		       PRINTER_ATTRIBUTES * printAttrib)
{
    MSG_N_GET_REQ
	request;
    MSG_N_GET_RESP
	* response;
    CONDITION
	cond;			/* condition code returned by various
				 * facilities */
    void
       *ctx = NULL;
    DCM_TAG
	tags[] = {		/* We request the server to provide us with
				 * the following attributes */
	DCM_PRINTERSTATUS,
	    DCM_PRINTERSTATUSINFOCODE,
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
    request.attributeList = tags;
    request.attributeCount = DIM_OF(tags);

    /* invoke the N_GET service */
    cond = SRV_NGetRequest(association, params, SOPClass,
			   &request, &response, ngetCallback, ctx);
    if (cond != SRV_NORMAL) {
	printf("In sendGetPrinterInstance : SRV_NGetRequest failed\n");
	return cond;
    }
    cond = convertObjectToStruct(&response->dataSet, printAttrib);
    if (cond != DCM_NORMAL) {
	printf("In sendGetPrinterInstance, convertObjectToStruct failed\n");
	return cond;
    }
    if (freeAll((void **) &response))
	printf("N-GET Response PROPERLY FREED\n\n");
    return SRV_NORMAL;		/* on success */
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
**	ctx		the associated context variable
**
** Return Values:
**	should return SRV_NORMAL in normal conditions
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
ngetCallback(MSG_N_GET_REQ * ngetRequest,
	     MSG_N_GET_RESP * ngetResponse,
	     void *ctx)
{
    CONDITION
    cond;

    /* Now print the attributes of the printer that we have received */
    printf("SCU : N-GET response from the printer\n");
    printf("  Attributes of the printer are : - \n");
    cond = DCM_DumpElements(&ngetResponse->dataSet);
    if (cond != DCM_NORMAL) {
	printf(" In ngetCallback : DCM_DumpElements failed\n");
	return cond;
    }
    return SRV_NORMAL;
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
static CONDITION
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
/* sendCreateFilmSession
**
** Purpose:
**	used to send the N-CREATE request message to create a BASIC
**	FILM SESSION SOP instance.
**
** Parameter Dictionary:
**	association	key which describes the association
**	params		parameters describing the association
**	SOPClass	meta SOP class UID
**	request		N-CREATE request message for FILM SESSION instance
**	response	N-CREATE response message obtained from SCP
**
** Return Values:
**	success or failure
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
sendCreateFilmSession(DUL_ASSOCIATIONKEY ** association,
		      DUL_ASSOCIATESERVICEPARAMETERS * params,
		      char *SOPClass,
		      BFS_ATTRIBUTES * bfsAttributes)
{
    MSG_N_CREATE_REQ
	request;
    MSG_N_CREATE_RESP
	* response;
    CONDITION
	cond;
    int
        uc;			/* update count */
    void
       *ctx = NULL;

    /*
     * The following are the values for the BasicFilmSession attributes
     * provided by the SCU and which are to be set by the SCP during the
     * N-CREATE service
     */
    static BFS_ATTRIBUTES bfsAttrib;
    static long
        presetFlags;

    static DCM_FLAGGED_ELEMENT map[] = {
	{DCM_BFSCOPIES, DCM_IS,
	    "#copies", 1, sizeof(bfsAttrib.copies), (void *) bfsAttrib.copies,
	BFS_COPIES, &presetFlags},
	{DCM_BFSPRINTPRIORITY, DCM_CS,
	    "priority", 1, sizeof(bfsAttrib.priority), (void *) bfsAttrib.priority,
	BFS_PRIORITY, &presetFlags},
	{DCM_BFSMEDIUMTYPE, DCM_CS,
	    "medium type", 1, sizeof(bfsAttrib.mediumType),
	    (void *) bfsAttrib.mediumType, BFS_MEDIUMTYPE,
	&presetFlags},
	{DCM_BFSFILMDESTINATION, DCM_CS,
	    "film dest", 1, sizeof(bfsAttrib.filmDestination),
	    (void *) bfsAttrib.filmDestination, BFS_FILMDESTINATION,
	&presetFlags},
	{DCM_BFSFILMSESSIONLABEL, DCM_SH,
	    "label", 1, sizeof(bfsAttrib.filmSessionLabel),
	    (void *) bfsAttrib.filmSessionLabel, BFS_FILMSESSIONLABEL,
	&presetFlags},
	{DCM_BFSMEMORYALLOCATION, DCM_IS,
	    "mem alloc", 1, sizeof(bfsAttrib.memoryAllocation),
	    (void *) bfsAttrib.memoryAllocation, BFS_MEMORYALLOCATION,
	&presetFlags},
    };

    presetFlags = bfsAttributes->bfsAttribFlag;
    bfsAttrib = *bfsAttributes;
    bfsAttrib.bfsAttribFlag = presetFlags;


    /*
     * we now build up a N-CREATE message to be sent to the server to request
     * for the creation of a BASIC FILM_SESSION SOP instance
     */
    request.messageID = SRV_MessageIDOut();
    request.type = MSG_K_N_CREATE_REQ;
    strcpy(request.classUID, DICOM_SOPCLASSBASICFILMSESSION);
    strcpy(request.instanceUID, "\0");	/* Let SCP provide it */
    request.dataSetType = DCM_CMDDATAOTHER;

    /* The attribute list is stored as an object */
    cond = DCM_CreateObject(&request.dataSet, 0);
    if (cond != DCM_NORMAL) {
	printf("In sendCreateFilmSession : DCM_CreateObject failed\n");
	return cond;
    };
    /* update the object with the given map elements */
    cond = DCM_ModifyElements(&request.dataSet, NULL, 0, map, DIM_OF(map), &uc);
    if (cond != DCM_NORMAL) {
	printf("In sendCreateFilmSession : DCM_ModifyElements failed\n");
	return cond;
    }
    /*
     * now use the DIMSE N-CREATE service to create the desired Basic Film
     * Session SOP instance
     */
    cond = SRV_NCreateRequest(association, params, SOPClass,
			      &request, &response, ncreateBFSCallback, ctx);
    if (cond != SRV_NORMAL) {
	printf("In sendCreateFilmSession : SRV_NCreateRequest failed\n");
	return cond;
    }
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
**	ctx		context variable
**
** Return Values:
**	Should return SRV_NORMAL under normal circumstances.
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
ncreateBFSCallback(MSG_N_CREATE_REQ * createRequest,
		   MSG_N_CREATE_RESP * createResponse,
		   void *ctx)
{
    CONDITION
    cond;

    /* print attribute list, status and SOP instance UID */
    printf("SCU : Received N-CREATE Response message\n");
    printf("SCU : Received BASIC FILM SESSION instance UID : %s\n",
	   createResponse->instanceUID);
    printf("SCU : Received the following updated BFS attributes : \n");
    if (createResponse->dataSetType != DCM_CMDDATANULL) {
	cond = DCM_DumpElements(&createResponse->dataSet);
	if (cond != DCM_NORMAL) {
	    printf("In sendCreateFilmSession : DCM_DumpElements failed\n");
	    return cond;
	}
    } else
	printf("NO DATA SET RECEIVED\n");

    (void) strcpy(bfsInstanceUID, createResponse->instanceUID);
    return SRV_NORMAL;
}
/* sendCreateFilmBox
**
** Purpose:
**	Describe the purpose of the function
**
** Parameter Dictionary:
**	association	key which describes the association
**	params		parameters of the association
**	SOPClass	negotiated meta SOP class
**	bfbAttributes	pointer to structure containing attributes of the
**			BASIC FILM BOX to be created.
**
** Return Values:
**	success or failure
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
sendCreateFilmBox(DUL_ASSOCIATIONKEY ** association,
		  DUL_ASSOCIATESERVICEPARAMETERS * params,
		  char *SOPClass,
		  BFB_ATTRIBUTES * bfbAttributes)
{
    MSG_N_CREATE_REQ
	request;
    MSG_N_CREATE_RESP
	* response;
    static BFB_ATTRIBUTES
        bfbAttrib;
    static DCM_FLAGGED_ELEMENT map[] = {
	{DCM_BFBIMAGEDISPLAYFORMAT, DCM_CS, "Image display format", 1,
	    sizeof(bfbAttrib.imageDisplayFormat),
	    (void *) bfbAttrib.imageDisplayFormat, BFB_IMAGEDISPLAYFORMAT,
	&bfbAttrib.bfbAttribFlag},
	{DCM_BFBFILMORIENTATION, DCM_CS, "Film orientation", 1,
	    sizeof(bfbAttrib.filmOrientation), (void *) bfbAttrib.filmOrientation,
	BFB_FILMORIENTATION, &bfbAttrib.bfbAttribFlag},
	{DCM_BFBFILMSIZEID, DCM_CS, "Film size", 1, sizeof(bfbAttrib.filmSizeID),
	(void *) bfbAttrib.filmSizeID, BFB_FILMSIZEID, &bfbAttrib.bfbAttribFlag},
	{DCM_BFBMAGNIFICATIONTYPE, DCM_CS, "Magnification type", 1,
	    sizeof(bfbAttrib.magnificationType),
	(void *) bfbAttrib.magnificationType, BFB_MAGNIFICATIONTYPE, &bfbAttrib.bfbAttribFlag},
	{DCM_BFBBORDERDENSITY, DCM_CS, "Border density", 1,
	    sizeof(bfbAttrib.borderDensity), (void *) bfbAttrib.borderDensity,
	BFB_BORDERDENSITY, &bfbAttrib.bfbAttribFlag},
	{DCM_BFBEMPTYIMAGEDENSITY, DCM_CS, "Empty image density", 1,
	    sizeof(bfbAttrib.emptyImageDensity),
	(void *) bfbAttrib.emptyImageDensity, BFB_EMPTYIMAGEDENSITY, &bfbAttrib.bfbAttribFlag},
	{DCM_BFBMINDENSITY, DCM_IS, "min density", 1,
	    sizeof(bfbAttrib.minDensity), (void *) bfbAttrib.minDensity,
	BFB_MINDENSITY, &bfbAttrib.bfbAttribFlag},
	{DCM_BFBMAXDENSITY, DCM_IS, "Max density", 1,
	    sizeof(bfbAttrib.maxDensity), (void *) bfbAttrib.maxDensity,
	BFB_MAXDENSITY, &bfbAttrib.bfbAttribFlag},
	{DCM_BFBTRIM, DCM_CS, "Trim", 1, sizeof(bfbAttrib.trim),
	(void *) bfbAttrib.trim, BFB_TRIM, &bfbAttrib.bfbAttribFlag},
	{DCM_BFBREFPARENTFILMSESSIONUID, DCM_UI, "parent film session", 1,
	    sizeof(bfbAttrib.referencedParentFilmSessionUID),
	    (void *) bfbAttrib.referencedParentFilmSessionUID,
	BFB_REFERENCEDPARENTFILMSESSION, &bfbAttrib.bfbAttribFlag}
    };
    CONDITION
	cond;
    int
        uc;
    void
       *ctx = NULL;

    bfbAttrib = *bfbAttributes;
    /* set the parent film session SOP instance UID */
    strcpy(bfbAttrib.referencedParentFilmSessionUID, bfsInstanceUID);
    bfbAttrib.bfbAttribFlag |= BFB_REFERENCEDPARENTFILMSESSION;

    /*
     * we now build up a N-CREATE message to be sent to the server to request
     * for the creation of a BASIC FILM BOX SOP instance
     */
    request.messageID = SRV_MessageIDOut();
    request.type = MSG_K_N_CREATE_REQ;
    strcpy(request.classUID, DICOM_SOPCLASSBASICFILMBOX);
    strcpy(request.instanceUID, "\0");	/* Let SCP provide it */
    request.dataSetType = DCM_CMDDATAOTHER;

    /* The attribute list is stored as an object */
    cond = DCM_CreateObject(&request.dataSet, 0);
    if (cond != DCM_NORMAL) {
	printf("In sendCreateFilmBox : DCM_CreateObject failed\n");
	return cond;
    };
    /* update the object with the given map elements */
    cond = DCM_ModifyElements(&request.dataSet, NULL, 0, map, DIM_OF(map), &uc);
    if (cond != DCM_NORMAL) {
	printf("In sendCreateFilmBox : DCM_ModifyElements failed\n");
	return cond;
    }
    /*
     * now use the DIMSE N-CREATE service to create the desired Basic Film
     * Session SOP instance
     */
    cond = SRV_NCreateRequest(association, params, SOPClass,
			      &request, &response, ncreateBFBCallback, ctx);
    if (cond != SRV_NORMAL) {
	printf("In sendCreateFilmBox : SRV_NCreateRequest failed\n");
	return cond;
    }
    /*
     * before returning, we store the instance UID of the last created BASIC
     * FILM BOX SESSION
     */
    (void) strcpy(bfbInstanceUID, response->instanceUID);

    return SRV_NORMAL;		/* success */

}

/* ncreateBFBCallback
**
** Purpose:
**	Call back function for N-CREATE request for BASIC FILM BOX
**
** Parameter Dictionary:
**	createRequest	N-CREATE request for Basic Film Box
**	createResponse	N-CREATE response for Basic Film Box
**	ctx		context variable
**
** Return Values:
**	should return SRV_NORMAL in normal circumstances
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
ncreateBFBCallback(MSG_N_CREATE_REQ * createRequest,
		   MSG_N_CREATE_RESP * createResponse,
		   void *ctx)
{
    CONDITION
    cond;

    /* print attribute list, status and SOP instance UID */
    printf("SCU : Received N-CREATE Response message\n");
    printf("SCU : Received Basic Film Box instance UID : %s\n",
	   createResponse->instanceUID);
    printf("SCU : Attributes received via Response message:-\n");
    cond = DCM_DumpElements(&createResponse->dataSet);
    if (cond != DCM_NORMAL) {
	printf("In ncreateBFBCallback : DCM_DumpElements failed\n");
	return cond;
    }
    return SRV_NORMAL;
}
/* sendSetImageBox
**
** Purpose:
**	sends a request to set the attributes of the Basic Image Box
**
** Parameter Dictionary:
**	association	key which descibes the association
**	params		parameters which describe the association
**	SOPClass	meta SOP class
**	uid		Basic Image Box SOP instance UID whose attributes
**			are to be set
**	position	position of the image in the row/column major order
**	request		pointer to N-SET request message
**	response	address of pointer to response message
**
** Return Values:
**	success or failure
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static int
sendSetImageBox(DUL_ASSOCIATIONKEY ** association,
		DUL_ASSOCIATESERVICEPARAMETERS * params,
		char *SOPClass,
		char *uid,
		int position,
		MSG_N_SET_REQ * request,
		MSG_N_SET_RESP ** response)
{
    static BIB_ATTRIBUTES bibAttrib = {
	0x0,
	""			/* SCU only provides the one mandatory
				 * attribute viz. image position */
    };
    static long
        presetFlags = BIB_IMAGEPOSITION;
    static DCM_FLAGGED_ELEMENT map[] = {
	{DCM_BIBIMAGEPOSITION, DCM_IS, "Image position", 1,
	    sizeof(bibAttrib.imagePosition), (void *) bibAttrib.imagePosition,
	BIB_IMAGEPOSITION, &presetFlags}
    };
    CONDITION
	cond;
    int
        uc;
    void
       *ctx = NULL;

    sprintf(bibAttrib.imagePosition, "%d", position);
    bibAttrib.bibAttribFlag = presetFlags;

    /*
     * we now build up a N_SET message to be sent to the server to request
     * for setting up the default attributes of the basic image box instance
     */
    request->messageID = SRV_MessageIDOut();
    request->type = MSG_K_N_SET_REQ;
    strcpy(request->classUID, DICOM_SOPCLASSBASICIMAGEBOX);
    strcpy(request->instanceUID, uid);
    request->dataSetType = DCM_CMDDATAOTHER;
    /* The modifications list is stored as an object */
    cond = DCM_CreateObject(&request->dataSet, 0);
    if (cond != DCM_NORMAL) {
	printf("In sendSetImageBox : DCM_CreateObject failed\n");
	return 0;
    }
    /* update the object with the given map elements */
    cond = DCM_ModifyElements(&request->dataSet, NULL, 0, map, DIM_OF(map), &uc);
    if (cond != DCM_NORMAL) {
	printf("In sendSetImageBox : DCM_ModifyElements failed\n");
	return 0;
    }
    /*
     * now use the DIMSE N-SET service to set the mandatory attribute and ask
     * the server (SCP) to set the other default values for the Basic Image
     * Box SOP instance
     */
    cond = SRV_NSetRequest(association, params, SOPClass,
			   request, response, nsetBIBCallback, ctx);
    if (cond != SRV_NORMAL) {
	printf("In sendSetImageBox : SRV_NSetRequest failed\n");
	return 0;
    }
    return 1;
}

/* nsetBIBCallback
**
** Purpose:
**	Callback function for the N-SET request for the Basic Image Box
**
** Parameter Dictionary:
**	setRequest	N-SET request to set attributes for Image Box
**	setResponse	N-SET response received from SCP
**	ctx		context information
**
** Return Values:
**	should return SRV_NORMAL under normal conditions
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
nsetBIBCallback(MSG_N_SET_REQ * setRequest,
		MSG_N_SET_RESP * setResponse,
		void *ctx)
{
    CONDITION
    cond;

    /* print attribute list */
    printf("SCU : Received N-SET Response message\n");
    printf("SCU : For the Image box instance UID : %s\n",
	   setResponse->instanceUID);
    printf("SCU : Received updated Image Box attributes :- \n");
    cond = DCM_DumpElements(&setResponse->dataSet);
    if (cond != DCM_NORMAL) {
	printf("In nsetBIBCallback : DCM_DumpElements failed\n");
	return cond;
    }
    return SRV_NORMAL;
}
/* sendActionRequest
**
** Purpose:
**	send N-ACTION request to request SCP to print the images
**
** Parameter Dictionary:
**	association	key which descibes the association
**	params		parameters which describe the association
**	SOPClass	meta SOP class
**	responseFilmBox	pointer to N-CREATE response message for parent
**			basic film box instance
**	request		pointer to N-ACTION request message
**	response	address of pointer to N-ACTION response message
**
** Return Values:
**	success or failure
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static int
sendActionRequest(DUL_ASSOCIATIONKEY ** association,
		  DUL_ASSOCIATESERVICEPARAMETERS * params,
		  char *SOPClass,
		  MSG_N_CREATE_RESP * responseFilmBox,
		  MSG_N_ACTION_REQ * request,
		  MSG_N_ACTION_RESP ** response)
{
    CONDITION
    cond;
    void
       *ctx = NULL;

    /* there is no data set to be sent to the SCP */
    request->dataSetType = DCM_CMDDATANULL;
    request->messageID = SRV_MessageIDOut();
    request->type = MSG_K_N_ACTION_REQ;
    request->actionInformation = NULL;
    strcpy(request->classUID, responseFilmBox->classUID);
    strcpy(request->instanceUID, responseFilmBox->instanceUID);

    /*
     * now use the DIMSE N-ACTION service to print the images in the Film Box
     * instance
     */
    cond = SRV_NActionRequest(association, params, SOPClass,
			      request, response, nactionBFBCallback, ctx);
    if (cond != SRV_NORMAL) {
	printf("In sendActionRequest : SRV_NActionRequest failed\n");
	return 0;
    }
    return 1;
}

/* nactionBFBCallback
**
** Purpose:
**	Callback function for N-ACTION request
**
** Parameter Dictionary:
**	actionRequest	pointer to N-ACTION request message
**	actionResponse	address of pointer to N-ACTION response message
**	ctx		context information
**
** Return Values:
**	should return SRV_NORMAL in normal circumstances
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
nactionBFBCallback(MSG_N_ACTION_REQ * actionRequest,
		   MSG_N_ACTION_RESP * actionResponse,
		   void *ctx)
{
    CONDITION
    cond;

    if (actionResponse->status == DCM_STATUS_SUCCESS) {
	printf("SCU : Received N-ACTION Response, status success\n");
	/* verify if the SCP supports a PRINT JOB SOP class */
	if (actionResponse->dataSetType != DCM_CMDDATANULL) {
	    /* now dump the information */
	    cond = DCM_DumpElements(&actionResponse->actionReply);
	    if (cond != DCM_NORMAL) {
		printf("In sendActionRequest, DCM_DumpElement failed\n");
		return cond;
	    }
	}
    }
    return SRV_NORMAL;
}
/* sendDeleteRequest
**
** Purpose:
**	send the N-DELETE request
**
** Parameter Dictionary:
**	Define the parameters to the function
**
** Return Values:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static int
sendDeleteRequest(DUL_ASSOCIATIONKEY ** association,
		  DUL_ASSOCIATESERVICEPARAMETERS * params,
		  char *SOPClass,
		  char *classUID,
		  char *instanceUID,
		  MSG_N_DELETE_REQ * request,
		  MSG_N_DELETE_RESP ** response)
{
    CONDITION
    cond;
    void
       *ctx = NULL;

    /* there is no data set to be sent to the SCP */
    request->dataSetType = DCM_CMDDATANULL;
    request->messageID = SRV_MessageIDOut();
    request->type = MSG_K_N_DELETE_REQ;
    /* delete request could be for Film Session or Film Box */
    strcpy(request->classUID, classUID);
    strcpy(request->instanceUID, instanceUID);

    /*
     * now use the DIMSE N-DELETE service to delete the current Film Box
     * instance
     */
    cond = SRV_NDeleteRequest(association, params, SOPClass,
			      request, response, ndeleteCallback, ctx);
    if (cond != SRV_NORMAL) {
	printf("In sendDeleteRequest : SRV_NDeleteRequest failed\n");
	return 0;
    }
    return 1;			/* otherwise */

}

/* ndeleteCallback
**
** Purpose:
**	Callback function for N-DELETE request
**
** Parameter Dictionary:
**	deleteRequest	N-DELETE request message
**	deleteResponse	N-DELETE response message
**	ctx		context information
**
** Return Values:
**	should return SRV_NORMAL in normal conditions
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
ndeleteCallback(MSG_N_DELETE_REQ * deleteRequest,
		MSG_N_DELETE_RESP * deleteResponse,
		void *ctx)
{
    printf("SCU : Received N-DELETE Response message\n");
    if (deleteResponse->status == DCM_STATUS_SUCCESS) {
	if (strcmp(deleteResponse->classUID,
		   DICOM_SOPCLASSBASICFILMSESSION) == 0)
	    printf("FILM SESSION DELETED\n");
	else if (strcmp(deleteResponse->classUID,
			DICOM_SOPCLASSBASICFILMBOX) == 0)
	    printf("FILM BOX DELETED\n");

    } else
	printf("N-DELETE Failed\n");
    return SRV_NORMAL;
}

/* usageerror
**
** Purpose:
**	Print the usage message for this program and exit.
**
** Parameter Dictionary:
**	Define the parameters to the function
**
** Return Values:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static void
usageerror()
{
    fprintf(stderr,
	    "Usage: print_client [-c calledAPTitle] [-f films] [-i imageFormat] [-p] [-t callingAPTitle] [-v] node port\n");
    exit(1);
}


/* myExit
**
** Purpose:
**	Exit routines which closes network connections, dumps error
**	messages and exits.
**
** Parameter Dictionary:
**	association	A handle for an association which is possibly open.
**
** Return Values:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static void
myExit(DUL_ASSOCIATIONKEY ** association)
{
    fprintf(stderr, "Abnormal exit\n");
    COND_DumpConditions();

    if (association != NULL)
	if (*association != NULL)
	    (void) DUL_DropAssociation(association);
    exit(1);
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
static int
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
		return 0;
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
		return 0;
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
		return 0;
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
		return 0;
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
		return 0;
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
		return 0;
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
		return 0;
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
		return 0;
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
		return 0;
	    }
	}
	break;
    default:
	printf("In freeAll, Invalid message type\n");
	return 0;
    }

    /* now free the rest of the structure */
    (void) MSG_Free(message);
    return 1;
}
