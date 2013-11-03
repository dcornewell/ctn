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
** Module Name(s):	setRequest
**
**	Private Routines:
**			nsetImageBoxCallback
**			nsetBFSCallback
**			nsetBFBCallback
**			unsupportedCallback
** Author, Date:	Aniruddha S. Gokhale, 5-July-1994
** Intent:		This module implements the routines that set the
**			attributes of an Image Box.
** Last Update:		$Author: smm $, $Date: 1995-12-29 04:13:11 $
** Source File:		$RCSfile: set.c,v $
** Revision:		$Revision: 1.21 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.21 $ $RCSfile: set.c,v $";

#include "print_server.h"
#include "private.h"
#include "static_variables.h"

/*lint -e528*/
extern CTNBOOLEAN traceFlag,
    gqueueFlag,
    sendBackFlag;
extern int gqID;
extern BFS_NODE bfsInstance;
extern BFB_NODE *bfbInstance;

static CONDITION
nsetImageBoxCallback(MSG_N_SET_REQ * nsetRequest,
		     MSG_N_SET_RESP * nsetResponse,
		     void *sendBack, DUL_PRESENTATIONCONTEXT * pc);
static CONDITION
nsetBFSCallback(MSG_N_SET_REQ * nsetRequest,
		MSG_N_SET_RESP * nsetResponse,
		void *sendBack, DUL_PRESENTATIONCONTEXT * pc);
static CONDITION
nsetBFBCallback(MSG_N_SET_REQ * nsetRequest,
		MSG_N_SET_RESP * nsetResponse,
		void *sendBack, DUL_PRESENTATIONCONTEXT * pc);
static CONDITION
unsupportedCallback(MSG_N_SET_REQ * nsetRequest,
		    MSG_N_SET_RESP * nsetResponse,
		    void *ctx, DUL_PRESENTATIONCONTEXT * pc);

/* setRequest
**
** Purpose:
**	To build the N-SET response message and send it back to the
**	requesting SCU.
**
** Parameter Dictionary:
**	association	The key which describes the association
**	ctx		Pointer to the context for this command
**	request		Pointer to the N-SET request message
**
** Return Values:
**	SRV_NORMAL under error free conditions
**
**	SRV_CALLBACKABORTEDSERVICE
**	SRV_ILLEGALPARAMETER
**	SRV_NOCALLBACK
**	SRV_NORMAL
**	SRV_OBJECTBUILDFAILED
**	SRV_RESPONSEFAILED
**
** Algorithm:
**	First determine for which SOP Class, the request has arrived.
**	Accordingly call SRV_NSetResponse with the appropriate
**	call back function.
*/
CONDITION
setRequest(DUL_ASSOCIATIONKEY ** association,
	   DUL_PRESENTATIONCONTEXT * ctx, MSG_N_SET_REQ ** request,
	   CTNBOOLEAN * sendBack)
{
    MSG_N_SET_RESP
	setResponse;
    CONDITION
	cond = SRV_NORMAL;

    /* clear the contents of the response message */
    memset((void *) &setResponse, 0, sizeof(MSG_N_SET_RESP));

    /*
     * Here we first need to find out for which SOP class, the SET request
     * has arrived
     */
    if (strcmp((*request)->classUID,
	       DICOM_SOPCLASSBASICGREYSCALEIMAGEBOX) == 0) {
	(void) COND_PopCondition(TRUE);
	cond = SRV_NSetResponse(association, ctx, request, &setResponse,
				nsetImageBoxCallback, sendBack, ".");
	if (cond != SRV_NORMAL) {
	    printf("In setRequest : SRV_NSetResponse failed\n");
	    return cond;
	}
	if (traceFlag) {
	    printf("AFTER setting attributes of Grayscale IMAGE BOX\n");
	    showHierarchy();
	}
	return cond;
    } else if (strcmp((*request)->classUID,
		      DICOM_SOPCLASSBASICCOLORIMAGEBOX) == 0) {
	(void) COND_PopCondition(TRUE);
	cond = SRV_NSetResponse(association, ctx, request, &setResponse,
				nsetImageBoxCallback, sendBack, ".");
	if (cond != SRV_NORMAL) {
	    printf("In setRequest : SRV_NSetResponse failed\n");
	    return cond;
	}
	if (traceFlag) {
	    printf("AFTER setting attributes of Color IMAGE BOX\n");
	    showHierarchy();
	}
	return cond;
    } else if (strcmp((*request)->classUID,
		      DICOM_SOPCLASSBASICFILMSESSION) == 0) {
	(void) COND_PopCondition(TRUE);
	cond = SRV_NSetResponse(association, ctx, request, &setResponse,
				nsetBFSCallback, sendBack, ".");
	if (cond != SRV_NORMAL) {
	    printf("In setRequest : SRV_NSetResponse failed\n");
	    return cond;
	}
	if (traceFlag) {
	    printf("AFTER setting attributes of BASIC FILM SESSION\n");
	    showHierarchy();
	}
	return cond;
#ifdef ASG
    } else if (strcmp((*request)->classUID,
		      DICOM_SOPCLASSBASICFILMBOX) == 0) {
	(void) COND_PopCondition(TRUE);
	cond = SRV_NSetResponse(association, ctx, request, &setResponse,
				nsetBFBCallback, sendBack, ".");
	if (cond != SRV_NORMAL) {
	    printf("In setRequest : SRV_NSetResponse failed\n");
	    return cond;
	}
	if (traceFlag) {
	    printf("AFTER setting attributes of BASIC FILM BOX\n");
	    showHierarchy();
	}
	return cond;
#endif
    } else {
	/* unsupported command for the given SOP class */
	cond = SRV_NSetResponse(association, ctx, request, &setResponse,
				unsupportedCallback, sendBack, ".");
	if (cond != SRV_NORMAL) {
	    printf("In setRequest : SRV_NSetResponse failed\n");
	    return cond;
	}
	return cond;
    }
}

/* nsetImageBoxCallback
**
** Purpose:
**	Call back routine called by SRV_NSetResponse used to build the
**	N-SET response message.
**
** Parameter Dictionary:
**	nsetRequest	Pointer to N-SET Request message
**	nsetResponse	Pointer to N-SET Response message
**	ctx		Pointer to context for this command.
**	pc		Presentation context that describes this SOP Class
**
** Return Values:
**	SRV_NORMAL under error free conditions.
**
**	SRV_CALLBACKABORTEDSERVICE
**	SRV_NORMAL
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
nsetImageBoxCallback(MSG_N_SET_REQ * nsetRequest,
		     MSG_N_SET_RESP * nsetResponse,
		     void *sendBack, DUL_PRESENTATIONCONTEXT * pc)
{

    CONDITION
    cond;			/* condition code returned by various
				 * facilities */
    int
        index;			/* loop index */
    char
       *flagNameList = NULL;	/* holds attribute names that gave errors
				 * during verification process */
    BIB_NODE
	* bibNode;		/* pointer to instance of Image Box */
    char
       *fileName;		/* filename holding attributes of image */
    GQ_ELEM
	gqElement;		/* The GQ element that is to be enqueued */
    PRN_BASICIMAGEBOX
	bibAttrib,		/* structure holding attributes of image box */
	*bibPtr;		/* pointer to the above structure */
    CTNBOOLEAN
	flag = FALSE;		/* to check if the image is present */

    /*
     * For Image Position, we do a runtime check depending on the value of
     * Image Display Format specified by the Film Box attribute.
     */

    /* Now define a table of all supported attributes */
    static SUPPORTEDATTRIBUTES supportedAttributeList[] = {
	{PRN_BIB_K_IMAGEPOSITION, "BIB Image Position ", NULL, 0},
	{PRN_BIB_K_IMAGEMODULE, "BIB Preformatted Image Sequence ",
	NULL, 0},
	{PRN_BIB_K_POLARITY, "BIB Polarity ", bibPolarity,
	(int) DIM_OF(bibPolarity)},
	{PRN_BIB_K_MAGNIFICATIONTYPE, "BIB Magnification type ",
	bibMagnificationType, (int) DIM_OF(bibMagnificationType)},
#ifdef ASG
	{PRN_BIB_K_SMOOTHINGTYPE, "BIB Smooothing Type ",
	bibSmoothingType, (int) DIM_OF(bibSmoothingType)},
	{PRN_BIB_K_REQUESTEDIMAGESIZE, "BIB Requested Image Size ",
	bibRequestedImageSize, (int) DIM_OF(bibRequestedImageSize)}
#endif
    };

    /* we now build up the response message */
    nsetResponse->type = MSG_K_N_SET_RESP;
    nsetResponse->messageIDRespondedTo = nsetRequest->messageID;
    strcpy(nsetResponse->classUID, nsetRequest->classUID);
    strcpy(nsetResponse->instanceUID, nsetRequest->instanceUID);

    /* default */
    nsetResponse->dataSetType = DCM_CMDDATANULL;
#ifdef ASG
    nsetResponse->dataSet = NULL;	/* no data set is sent */
#endif

    bibPtr = &bibAttrib;
    bibPtr->bibAttributeFlag = 0x0;
    (void) strcpy(bibPtr->imageBoxSOPClassUID, nsetRequest->classUID);
    (void) strcpy(bibPtr->imageBoxSOPInstanceUID, nsetRequest->instanceUID);
    /* Parse the data set sent by the SCU */
    cond = PRN_ParseObject(&nsetRequest->dataSet, PRN_K_BASICGREYSCALEIMAGEBOX,
			   (void **) &bibPtr);
    if (cond != PRN_NORMAL) {
	cond = COND_PushCondition(APP_ERROR(APP_FAILURE), "PRN_ParseObject",
				  "nsetImageBoxCallback");
	updateOnProcessingFailure((void **) &nsetResponse,
				  APP_ERROR(APP_FAILURE), "PRN_ParseObject",
				  "nsetImageBoxCallback");
	goto ERROR_RETURN;
    }
    /* PRN_DumpAttributes(bibPtr, stdout); */
    cond = supportedAttributes(MSG_K_N_SET_REQ, bibPtr, supportedAttributeList,
			(int) DIM_OF(supportedAttributeList), &flagNameList,
			       (void **) &nsetResponse);
    if (cond != APP_NORMAL) {
	switch (cond) {
	case APP_MISSINGREQUIREDATTRIBUTE:
	    nsetResponse->status = MSG_K_MISSINGATTRIBUTE;
	    break;
	case APP_UNSUPPORTEDMANDATORYATTRIBUTE:
	case APP_UNSUPPORTEDOPTIONALATTRIBUTE:
	    nsetResponse->status = MSG_K_NOSUCHATTRIBUTE;
	    break;
	case APP_UNSUPPORTEDATTRIBUTEVALUE:
	    nsetResponse->status = MSG_K_INVALIDATTRIBUTEVALUE;
	    break;
	}
	cond = COND_PushCondition(APP_ERROR(cond), flagNameList,
				  "nsetImageBoxCallback");
	goto ERROR_RETURN;
    }
    /*
     * Now we update the hierarchy. We note here that attributes are set for
     * the image boxes belonging to the current BASIC FILM BOX instance which
     * is given by bfbInstance
     */
    bibNode = (BIB_NODE *) LST_Position(&bfbInstance->bibList,
					LST_Head(&bfbInstance->bibList));
    if (bibNode == NULL) {
	cond = COND_PushCondition(APP_ERROR(APP_MISSINGIMAGES),
				  "nsetImageBoxCallback");
	updateOnProcessingFailure((void **) &nsetResponse,
		      APP_ERROR(APP_MISSINGIMAGES), "nsetImageBoxCallback");
	goto ERROR_RETURN;
    }
    flag = FALSE;
    /*
     * go thru the list of Image SOP instance UIDs maintained by the Film Box
     * instance
     */
    for (index = 0; index < (int) LST_Count(&bfbInstance->bibList)
	 && !flag; index++) {
	/* now see if the instance UID matches */
	if (strcmp(nsetRequest->instanceUID, bibNode->bibInstanceUID) == 0) {
	    flag = TRUE;
	}
	if (!flag)
	    bibNode = (BIB_NODE *) LST_Next(&bfbInstance->bibList);
    }
    if (flag == FALSE) {
	cond = COND_PushCondition(APP_ERROR(APP_NOSUCHINSTANCE),
			  nsetRequest->instanceUID, "nsetImageBoxCallback");
	nsetResponse->status = MSG_K_NOSUCHOBJECTINSTANCE;
	goto ERROR_RETURN;
    }
    /* we now store the attributes of the image box */
    /*
     * To avoid copying so much data, we just make a pointer point to the
     * dataSet
     */
    bibNode->bibAttrib = nsetRequest->dataSet;
    nsetRequest->dataSet = NULL;/* remove the data set from the request
				 * message */

    /*
     * If the server's terminal has x-capability and wishes to enqueue the
     * image in the GQueue
     */
    if (gqueueFlag) {
	gqElement.prnMessage = PRN_NEWIMAGEBOX;
	(void) strcpy(gqElement.instanceUID, bibNode->bibInstanceUID);
	/* get a new file name to store the object */
	fileName = getNewFileName();
	(void) strcpy(gqElement.objFile, fileName);

	/* now write the object into the file and enqueue it */
	cond = DCM_WriteFile(&bibNode->bibAttrib, DCM_ORDERLITTLEENDIAN,
			     fileName);
	if (cond != DCM_NORMAL) {
	    unlink(gqElement.objFile);	/* delete the file before exiting */
	    cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				   "DCM_WriteFile", "nsetImageBoxCallback");
	    updateOnProcessingFailure((void **) &nsetResponse,
				    APP_ERROR(APP_FAILURE), "DCM_WriteFile",
				      "nsetImageBoxCallback");
	    goto ERROR_RETURN;
	}
	cond = GQ_Enqueue(gqID, (void *) &gqElement);
	if (cond != GQ_NORMAL) {
	    unlink(gqElement.objFile);
	    cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				      "GQ_Enqueue", "nsetImageBoxCallback");
	    updateOnProcessingFailure((void **) &nsetResponse,
				      APP_ERROR(APP_FAILURE), "GQ_Enqueue",
				      "nsetImageBoxCallback");
	    goto ERROR_RETURN;
	}
    }
    /* send attributes back, but do not send back the image pixel data */
    /* temporarily we reset the FLAG for the pixel data */
    nsetResponse->dataSetType = DCM_CMDDATAOTHER;
    bibPtr->bibAttributeFlag ^= PRN_BIB_K_IMAGEMODULE;
    cond = PRN_BuildObject((void *) bibPtr, &nsetResponse->dataSet);
    if (cond != PRN_NORMAL) {
	cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				  "PRN_BuildObject", "nsetImageBoxCallback");
	updateOnProcessingFailure((void **) &nsetResponse,
				  APP_ERROR(APP_FAILURE), "PRN_BuildObject",
				  "nsetImageBoxCallback");
	goto ERROR_RETURN;
    }
    /* set back the FLAG */
    bibPtr->bibAttributeFlag |= PRN_BIB_K_IMAGEMODULE;
    /* everything is successful */
    nsetResponse->status = MSG_K_SUCCESS;
    return SRV_NORMAL;

ERROR_RETURN:
    COND_DumpConditions();
    return SRV_CALLBACKABORTEDSERVICE;
}
/* nsetBFSCallback
**
** Purpose:
**	Callback function invoked by SRV_NSetResponse to set the attributes
**	of Basic Film Session
**
** Parameter Dictionary:
**	nsetRequest	Pointer to the set request
**	nsetResponse	Pointer to the set response
**	sendBack	Indicates whether attributes are to be sent back
**			to the requesting SCU
**	pc		Presentation context describes this SOP class
**
** Return Values:
**	SRV_NORMAL on success
**	SRV_CALLBACKABORTEDSERVICE on failure
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
nsetBFSCallback(MSG_N_SET_REQ * nsetRequest,
		MSG_N_SET_RESP * nsetResponse,
		void *sendBack, DUL_PRESENTATIONCONTEXT * pc)
{
    CONDITION
    cond;			/* condition code returned by various
				 * facilities used by the callback */
    PRN_BASICFILMSESSION bfsAttrib,	/* holds attributes of the Basic Film
					 * Session received from the
					 * requesting SCU */
       *bfsPtr;			/* a pointer to the above structure */
    char
       *fileName;		/* filename holding attributes of Basic Film
				 * Session instance */
    GQ_ELEM
	gqElement;		/* The GQ element to be enqueued */
    char
       *flagNameList = NULL;	/* holds attribute names for which the
				 * verification service caused an error */

    /* define a table of all those attributes that need to be verified */
    static SUPPORTEDATTRIBUTES supportedAttributeList[] = {
	{PRN_BFS_K_COPIES, "BFS Copies ", bfsSupportedCopies,
	(int) DIM_OF(bfsSupportedCopies)},
	{PRN_BFS_K_PRIORITY, "BFS Priority ", bfsSupportedPriority,
	(int) DIM_OF(bfsSupportedPriority)},
	{PRN_BFS_K_MEDIUMTYPE, "BFS Medium type ", bfsSupportedMediumType,
	(int) DIM_OF(bfsSupportedMediumType)},
	{PRN_BFS_K_FILMDESTINATION, "BFS Film Destination ",
	bfsSupportedFilmDestination, (int) DIM_OF(bfsSupportedFilmDestination)},
	{PRN_BFS_K_FILMSESSIONLABEL, "BFS Film Session Label ",
	    bfsSupportedFilmSessionLabel,
	(int) DIM_OF(bfsSupportedFilmSessionLabel)},
#ifdef ASG
	{PRN_BFS_K_MEMORYALLOCATION, "BFS Memory Allocation ",
	NULL, 0}
#endif
    };

    /* we build up the response message */
    nsetResponse->type = MSG_K_N_SET_RESP;
    nsetResponse->messageIDRespondedTo = nsetRequest->messageID;
    strcpy(nsetResponse->classUID, nsetRequest->classUID);
    strcpy(nsetResponse->instanceUID, nsetRequest->instanceUID);
    nsetResponse->dataSetType = DCM_CMDDATANULL;
    nsetResponse->conditionalFields = 0;
#ifdef ASG
    nsetResponse->dataSet = NULL;
#endif

    /*
     * Verify that the instance UID specified in the Request message is valid
     */
    if (strcmp(bfsInstance.bfsInstanceUID, nsetRequest->instanceUID) != 0) {
	/* instance UID conflict */
	nsetResponse->status = MSG_K_CLASSINSTANCECONFLICT;
	strcpy(nsetResponse->requestedClassUID, nsetRequest->classUID);
	strcpy(nsetResponse->requestedInstanceUID, nsetRequest->instanceUID);
	nsetResponse->conditionalFields |= MSG_K_N_SETRESP_REQUESTEDCLASSUID |
	    MSG_K_N_SETRESP_REQUESTEDINSTANCEUID;
	goto ERROR_RETURN;
    }
    bfsPtr = &bfsAttrib;
    bfsPtr->bfsAttributeFlag = 0x0;
    (void) strcpy(bfsPtr->filmSessionSOPClassUID, nsetResponse->classUID);
    (void) strcpy(bfsPtr->filmSessionSOPInstanceUID,
		  nsetResponse->instanceUID);
    /* Parse the data set sent by SCU */
    cond = PRN_ParseObject(&nsetRequest->dataSet, PRN_K_BASICFILMSESSION,
			   (void **) &bfsPtr);
    if (cond != PRN_NORMAL) {
	cond = COND_PushCondition(APP_ERROR(APP_FAILURE), "PRN_ParseObject",
				  "nsetBFSCallback");
	updateOnProcessingFailure((void **) &nsetResponse,
	      APP_ERROR(APP_FAILURE), "PRN_ParseObject", "nsetBFSCallback");
	goto ERROR_RETURN;
    }
    if (traceFlag) {
	printf("Film Session Attributes Specified by SCU (NSet Request):\n");
	PRN_DumpAttributes(bfsPtr, stdout);
    }
    /*
     * We verify if the performing SCU has provided all mandatory attributes
     * and the requesting SCU has specified values for the mandatory
     * attributes. Further, we also verify if the optional attributes
     * requested by the SCU are supported by the performing SCU. Finally, we
     * verify if the values supplied by the requesting SCU are supported by
     * the performing SCU.
     * 
     * At any point, the function finds a discrepancy, it returns the
     * appropriate error condition
     */
    cond = supportedAttributes(MSG_K_N_SET_REQ, bfsPtr,
	       supportedAttributeList, (int) DIM_OF(supportedAttributeList),
			       &flagNameList, (void **) &nsetResponse);
    if (cond != APP_NORMAL) {
	/*
	 * depending upon the condition returned, set the message status
	 * value
	 */
	switch (cond) {
	case APP_MISSINGREQUIREDATTRIBUTE:
	    nsetResponse->status = MSG_K_MISSINGATTRIBUTE;
	    break;
	case APP_UNSUPPORTEDMANDATORYATTRIBUTE:
	case APP_UNSUPPORTEDOPTIONALATTRIBUTE:
	    nsetResponse->status = MSG_K_NOSUCHATTRIBUTE;
	    break;
	case APP_UNSUPPORTEDATTRIBUTEVALUE:
	    nsetResponse->status = MSG_K_INVALIDATTRIBUTEVALUE;
	    break;
	}
	cond = COND_PushCondition(APP_ERROR(cond), flagNameList,
				  "nsetBFSCallback");
	goto ERROR_RETURN;
    }
    /*
     * At this point the verification process is successful Proceed by
     * creating the Basic Film Session
     */
    /* UPDATE hierarchy */
    cond = PRN_BuildObject((void *) bfsPtr, &bfsInstance.bfsAttrib);
    if (cond != PRN_NORMAL) {
	cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				  "PRN_BuildObject", "nsetBFSCallback");
	updateOnProcessingFailure((void **) &nsetResponse,
	      APP_ERROR(APP_FAILURE), "PRN_BuildObject", "nsetBFSCallback");
	goto ERROR_RETURN;
    }
    strcpy(bfsInstance.bfsInstanceUID, nsetResponse->instanceUID);
    /*
     * If the server's terminal has x-capability and wishes to enqueue the
     * image in the GQueue
     */
    if (gqueueFlag) {
	gqElement.prnMessage = PRN_NEWFILMSESSION;	/* element type to be
							 * enqueued */
	(void) strcpy(gqElement.instanceUID, bfsInstance.bfsInstanceUID);
	/* get a new file name to store the object */
	fileName = getNewFileName();
	(void) strcpy(gqElement.objFile, fileName);

	/* now write the object into the file and enqueue it */
	cond = DCM_WriteFile(&bfsInstance.bfsAttrib, DCM_ORDERLITTLEENDIAN,
			     fileName);
	if (cond != DCM_NORMAL) {
	    unlink(gqElement.objFile);	/* delete the file before exiting */
	    cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				      "DCM_WriteFile", "nsetBFSCallback");
	    updateOnProcessingFailure((void **) &nsetResponse,
		APP_ERROR(APP_FAILURE), "DCM_WriteFile", "nsetBFSCallback");
	    goto ERROR_RETURN;
	}
	cond = GQ_Enqueue(gqID, (void *) &gqElement);
	if (cond != GQ_NORMAL) {
	    unlink(gqElement.objFile);
	    cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				      "GQ_Enqueue", "nsetBFSCallback");
	    updateOnProcessingFailure((void **) &nsetResponse,
		   APP_ERROR(APP_FAILURE), "GQ_Enqueue", "nsetBFSCallback");
	    goto ERROR_RETURN;
	}
    }
    /* Attributes need to be sent back */
    cond = PRN_BuildObject((void *) bfsPtr, &nsetResponse->dataSet);
    if (cond != PRN_NORMAL) {
	cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				  "PRN_BuildObject", "nsetBFSCallback");
	updateOnProcessingFailure((void **) &nsetResponse,
	      APP_ERROR(APP_FAILURE), "PRN_BuildObject", "nsetBFSCallback");
	goto ERROR_RETURN;
    }
    nsetResponse->dataSetType = DCM_CMDDATAOTHER;
    /* everything is successful */
    nsetResponse->status = MSG_K_SUCCESS;
    return SRV_NORMAL;

    /*
     * This is the exit point for all errors that may have occurred in the
     * callback routine
     */
ERROR_RETURN:
    COND_DumpConditions();
    return SRV_CALLBACKABORTEDSERVICE;
}

/* nsetBFBCallback
**
** Purpose:
**	Describe the purpose of the function
**
** Parameter Dictionary:
**	Define the parameters to the function
**
** Return Values:
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
nsetBFBCallback(MSG_N_SET_REQ * nsetRequest,
		MSG_N_SET_RESP * nsetResponse,
		void *sendBack, DUL_PRESENTATIONCONTEXT * pc)
{
#ifdef ASG
    CONDITION
    cond;			/* various condition codes returned by the
				 * facilities */

    PRN_BASICFILMBOX bfbAttrib,	/* structure to hold the attributes of BASIC
				 * FILM BOX */
       *bfbPtr = NULL;		/* pointer to the above structure */
    int
        index;			/* loop indices */
    int
        index2;
    char
       *fileName;		/* filename holding attributes of Basic Film
				 * Box instance that is to be enqueued */
    GQ_ELEM
	gqElement;		/* A GQ element that shall hold the instance
				 * of the Basic Film Box */
    char
       *flagNameList = NULL;	/* holds values and names of attributes that
				 * cause any error during verification */

    static APP_BFBIMAGEDISPLAYFORMAT
        bfbImageDisplayFormat;	/* this structure gets filled by the
				 * verification service as a side-effect */

    /* define the table of all attributes that need to be verified */
    static SUPPORTEDATTRIBUTES supportedAttributeList[] = {
	{PRN_BFB_K_MAGNIFICATIONTYPE, "BFB Magnification Type ",
	bfbMagnificationType, (int) DIM_OF(bfbMagnificationType)},
	{PRN_BFB_K_MAXDENSITY, "BFB Max Density ", NULL, 0},
	{PRN_BFB_K_CONFIGURATIONINFO, "BFB Configuration Information ",
	bfbConfigurationInfo, (int) DIM_OF(bfbConfigurationInfo)},
	{PRN_BFB_K_SMOOTHINGTYPE, "BFB Smoothing Type", NULL, 0},
	{PRN_BFB_K_BORDERDENSITY, "BFB Border Density ",
	bfbBorderDensity, (int) DIM_OF(bfbBorderDensity)},
	{PRN_BFB_K_EMPTYIMAGEDENSITY, "BFB Empty Image Density ",
	bfbEmptyImageDensity, (int) DIM_OF(bfbEmptyImageDensity)},
	{PRN_BFB_K_MINDENSITY, "BFB Min Density ", NULL, 0},
	{PRN_BFB_K_TRIM, "BFB Trim ", bfbTrim, (int) DIM_OF(bfbTrim)}
    };

    /* we build up the response message */
    nsetResponse->type = MSG_K_N_SET_RESP;
    nsetResponse->status = MSG_K_SUCCESS;
    nsetResponse->messageIDRespondedTo = nsetRequest->messageID;
    nsetResponse->dataSetType = DCM_CMDDATANULL;
    nsetResponse->conditionalFields = 0;
    strcpy(nsetResponse->classUID, nsetRequest->classUID);
#ifdef ASG
    nsetResponse->dataSet = NULL;
#endif
    /*
     * verify that the arriving NSet request for the Basic Film Box is for
     * the last created film box instance
     */

    /* Parse the received data set */
    bfbPtr = &bfbAttrib;
    bfbPtr->bfbAttributeFlag = 0x0;
    (void) strcpy(bfbPtr->filmBoxSOPClassUID, nsetRequest->classUID);
    (void) strcpy(bfbPtr->filmBoxSOPInstanceUID, nsetResponse->instanceUID);
    cond = PRN_ParseObject(&nsetRequest->dataSet, PRN_K_BASICFILMBOX,
			   (void **) &bfbPtr);
    if (cond != PRN_NORMAL) {
	cond = COND_PushCondition(APP_ERROR(APP_FAILURE), "PRN_ParseObject",
				  "nsetBFBCallback");
	updateOnProcessingFailure((void **) &nsetResponse,
	      APP_ERROR(APP_FAILURE), "PRN_ParseObject", "nsetBFBCallback");
	goto ERROR_RETURN;
    }
    if (traceFlag) {
	printf("Basic Film Box Attributes specified by SCU:\n");
	PRN_DumpAttributes(bfbPtr, stdout);
    }
    /*
     * We verify if the performing SCU has provided all mandatory attributes
     * and the requesting SCU has specified values for the mandatory
     * attributes. Further, we also verify if the optional attributes
     * requested by the SCU are supported by the performing SCU. Finally, we
     * verify if the values supplied by the requesting SCU are supported by
     * the performing SCU.
     * 
     * At any point, the function finds a discrepancy, it returns the
     * appropriate error condition
     */
    cond = supportedAttributes(MSG_K_N_CREATE_REQ, bfbPtr,
	       supportedAttributeList, (int) DIM_OF(supportedAttributeList),
			       &flagNameList, (void **) &nsetResponse);
    if (cond != APP_NORMAL) {
	/*
	 * depending upon the condition returned, set the message status
	 * value
	 */
	switch (cond) {
	case APP_MISSINGREQUIREDATTRIBUTE:
	    nsetResponse->status = MSG_K_MISSINGATTRIBUTE;
	    cond = COND_PushCondition(APP_ERROR(cond), flagNameList,
				      "nsetBFBCallback");
	    goto ERROR_RETURN;
	    break;
	case APP_UNSUPPORTEDMANDATORYATTRIBUTE:
	    cond = COND_PushCondition(APP_ERROR(cond), flagNameList,
				      "nsetBFBCallback");
	    goto ERROR_RETURN;
	    break;
	case APP_UNSUPPORTEDOPTIONALATTRIBUTE:
	    /* nsetResponse->status = MSG_K_NOSUCHATTRIBUTE; */
	    nsetResponse->status = MSG_K_SUCCESS;
	    break;
	case APP_UNSUPPORTEDATTRIBUTEVALUE:
	    /* nsetResponse->status = MSG_K_INVALIDATTRIBUTEVALUE; */
	    nsetResponse->status = MSG_K_SUCCESS;
	    break;
	}
    }
    /*
     * we need to build the referenced image box sequence to be included in
     * the data set. We do not support sequence of annotation boxes.
     */
    bfbPtr->referencedBABSeq = NULL;	/* no seq of annotation boxes */
    bfbPtr->referencedBIBSeq = LST_Create();
    if (bfbPtr->referencedBIBSeq == NULL) {
	cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				  "LST_Create", "nsetBFBCallback");
	updateOnProcessingFailure((void **) &nsetResponse,
		   APP_ERROR(APP_FAILURE), "LST_Create", "nsetBFBCallback");
	goto ERROR_RETURN;
    }
    /*
     * We also update the hierarchy. Create a copy of the BFB instance
     */
    if ((bfbInstance = malloc(sizeof(*bfbInstance))) == NULL) {
	cond = COND_PushCondition(APP_ERROR(APP_MALLOCFAILURE),
				  "nsetBFBCallback");
	updateOnProcessingFailure((void **) &nsetResponse,
			   APP_ERROR(APP_MALLOCFAILURE), "nsetBFBCallback");
	goto ERROR_RETURN;
    }
    (void) strcpy(bfbInstance->bfbInstanceUID, nsetResponse->instanceUID);
    bfbInstance->bfbAttrib = NULL;

    /* now acquire space to store the list of image boxes */
    bfbInstance->babList = NULL;/* we do not have an annotation box */
    bfbInstance->bibList = LST_Create();
    if (bfbInstance->bibList == NULL) {
	cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				  "LST_Create", "nsetBFBCallback");
	updateOnProcessingFailure((void **) &nsetResponse,
		   APP_ERROR(APP_FAILURE), "LST_Create", "nsetBFBCallback");
	goto ERROR_RETURN;
    }
    /*
     * The SCP now creates instances of the BASIC IMAGE BOX SOP class
     * depending on the attribute value viz. imageDisplayFormat
     * 
     */
    switch (bfbImageDisplayFormat.type) {
    case STANDARD_FORMAT:
	/* images are stored in row-major order */
	for (index = 0; index < bfbImageDisplayFormat.format.standard.numRows;
	     index++) {
	    for (index2 = 0; index2 <
	       bfbImageDisplayFormat.format.standard.numColumns; index2++) {
		cond = createBIBList(bfbPtr);
		if (cond != APP_NORMAL) {
		    cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
					"createBIBList", "nsetBFBCallback");
		    updateOnProcessingFailure((void **) &nsetResponse,
				    APP_ERROR(APP_FAILURE), "createBIBList",
					      "nsetBFBCallback");
		    goto ERROR_RETURN;
		}
	    }
	}
	break;
    case ROW_FORMAT:
	for (index = 0; index < bfbImageDisplayFormat.format.row.numRows;
	     index++) {
	    for (index2 = 0; index2 <
		 bfbImageDisplayFormat.format.row.imagesPerRow[index];
		 index2++) {
		cond = createBIBList(bfbPtr);
		if (cond != APP_NORMAL) {
		    cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
					"createBIBList", "nsetBFBCallback");
		    updateOnProcessingFailure((void **) &nsetResponse,
				    APP_ERROR(APP_FAILURE), "createBIBList",
					      "nsetBFBCallback");
		    goto ERROR_RETURN;
		}
	    }
	}
	break;
    case COL_FORMAT:
	for (index = 0; index < bfbImageDisplayFormat.format.standard.numRows;
	     index++) {
	    for (index2 = 0; index2 <
		 bfbImageDisplayFormat.format.col.imagesPerColumn[index];
		 index2++) {
		cond = createBIBList(bfbPtr);
		if (cond != APP_NORMAL) {
		    cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
					"createBIBList", "nsetBFBCallback");
		    updateOnProcessingFailure((void **) &nsetResponse,
				    APP_ERROR(APP_FAILURE), "createBIBList",
					      "nsetBFBCallback");
		    goto ERROR_RETURN;
		}
	    }
	}
	break;
    }
    /*
     * We need to update the following flag since, the SCP provides this
     * attribute.
     */
    bfbPtr->bfbAttributeFlag |= PRN_BFB_K_REFERENCEDBASICIMAGEBOXSEQ;
    /* we now build the response data set */
    nsetResponse->dataSetType = DCM_CMDDATAOTHER;
    cond = PRN_BuildObject((void *) bfbPtr, &nsetResponse->dataSet);
    if (cond != PRN_NORMAL) {
	cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				  "PRN_BuildObject", "nsetBFBCallback");
	updateOnProcessingFailure((void **) &nsetResponse,
	      APP_ERROR(APP_FAILURE), "PRN_BuildObject", "nsetBFBCallback");
	goto ERROR_RETURN;
    }
    /* also add this to the hierarchy */
    cond = PRN_BuildObject((void *) bfbPtr, &bfbInstance->bfbAttrib);
    if (cond != PRN_NORMAL) {
	cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				  "PRN_BuildObject", "nsetBFBCallback");
	updateOnProcessingFailure((void **) &nsetResponse,
	      APP_ERROR(APP_FAILURE), "PRN_BuildObject", "nsetBFBCallback");
	goto ERROR_RETURN;
    }
    /*
     * If the server's terminal has x-capability and wishes to enqueue the
     * film box instance in the GQueue
     */
    if (gqueueFlag) {
	gqElement.prnMessage = PRN_NEWFILMBOX;
	(void) strcpy(gqElement.instanceUID, bfbInstance->bfbInstanceUID);
	/* get a new file name to store the object */
	fileName = getNewFileName();
	(void) strcpy(gqElement.objFile, fileName);

	/* now write the object into the file and enqueue it */
	cond = DCM_WriteFile(&bfbInstance->bfbAttrib, DCM_ORDERLITTLEENDIAN,
			     fileName);
	if (cond != DCM_NORMAL) {
	    unlink(gqElement.objFile);	/* delete the file before exiting */
	    cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				      "DCM_WriteFile", "nsetBFBCallback");
	    updateOnProcessingFailure((void **) &nsetResponse,
		APP_ERROR(APP_FAILURE), "DCM_WriteFile", "nsetBFBCallback");
	    goto ERROR_RETURN;
	}
	cond = GQ_Enqueue((void *) &gqElement);
	if (cond != GQ_NORMAL) {
	    unlink(gqElement.objFile);
	    cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				      "GQ_Enqueue", "nsetBFBCallback");
	    updateOnProcessingFailure((void **) &nsetResponse,
				      APP_ERROR(APP_FAILURE),
				      "GQ_Enqueue", "nsetBFBCallback");
	    goto ERROR_RETURN;
	}
    }
    /*
     * add this bfbInstance to the list of basic film boxes maintained by the
     * the parent Basic Film Session instance
     */
    cond = LST_Enqueue(&bfsInstance.bfbList, bfbInstance);
    if (cond != LST_NORMAL) {
	cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				  "LST_Enqueue", "nsetBFBCallback");
	updateOnProcessingFailure((void **) &nsetResponse,
		  APP_ERROR(APP_FAILURE), "LST_Enqueue", "nsetBFBCallback");
	goto ERROR_RETURN;
    }
    /* Attributes need to be sent back */
    cond = PRN_BuildObject((void *) bfbPtr, &nsetResponse->dataSet);
    if (cond != PRN_NORMAL) {
	cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				  "PRN_BuildObject", "nsetBFBCallback");
	updateOnProcessingFailure((void **) &nsetResponse,
	      APP_ERROR(APP_FAILURE), "PRN_BuildObject", "nsetBFBCallback");
	goto ERROR_RETURN;
    }
    /* everything is successful (may have warnings) */
    nsetResponse->dataSetType = DCM_CMDDATAOTHER;
#ifdef ASG
    nsetResponse->status = MSG_K_SUCCESS;
#endif
    if (traceFlag) {
	printf("Returning SRV_NORMAL in nsetBFBCallback\n");
	MSG_DumpMessage(nsetResponse, stdout);
    }
    return SRV_NORMAL;

ERROR_RETURN:
    COND_DumpConditions();
    if (traceFlag) {
	printf("Returning SRV_CALLBACKABORTEDSERVICE in nsetBFBCallback\n");
	MSG_DumpMessage(nsetResponse, stdout);
    }
    return SRV_CALLBACKABORTEDSERVICE;
#endif
    return SRV_NORMAL;
}

/* unsupportedCallback
**
** Purpose:
**	This callback routine is invoked by the SRV_NSetResponse
**	whenever service for the requested SOP class is unsupported
**
** Parameter Dictionary:
**	nsetRequest	Pointer to request message
**	nsetResponse	Pointer to response message
**	ctx		Context information which we ignore
**	pc		Presentation context describes this SOP class
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
unsupportedCallback(MSG_N_SET_REQ * nsetRequest,
		    MSG_N_SET_RESP * nsetResponse,
		    void *ctx, DUL_PRESENTATIONCONTEXT * pc)
{
    nsetResponse->status = MSG_K_UNRECOGNIZEDOPERATION;
    return SRV_CALLBACKABORTEDSERVICE;
}
