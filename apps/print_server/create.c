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
** Module Name(s):	createRequest
**
**	Private Routines
**			ncreateBFSCallback
**			ncreateBFBCallback
**			unsupportedCallback
** Author, Date:	Aniruddha S. Gokhale, 29-June-94
** Intent:		This module defines the routine that serves the
**			N-CREATE request for either the Basic Film Session or
**			the Basic Film Box SOP Classes.
** Last Update:		$Author: smm $, $Date: 1996-08-07 19:28:51 $
** Source File:		$RCSfile: create.c,v $
** Revision:		$Revision: 1.39 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.39 $ $RCSfile: create.c,v $";
/*lint -e528*/
#include "print_server.h"
#include "private.h"
#include "static_variables.h"

extern CTNBOOLEAN traceFlag,
    gqueueFlag;
extern int gqID;
extern BFS_NODE bfsInstance;
extern BFB_NODE *bfbInstance;

static CONDITION
ncreateBFSCallback(MSG_N_CREATE_REQ * ncreateRequest,
		   MSG_N_CREATE_RESP * ncreateResponse,
/*		   CTNBOOLEAN * sendBack, */
		   void *sendBack,
		   DUL_PRESENTATIONCONTEXT * presentationCtx);
static CONDITION
ncreateBFBCallback(MSG_N_CREATE_REQ * ncreateRequest,
		   MSG_N_CREATE_RESP * ncreateResponse,
/*		   CTNBOOLEAN * sendBack, */
		   void *sendBack,
		   DUL_PRESENTATIONCONTEXT * presentationCtx);
static CONDITION
unsupportedCallback(MSG_N_CREATE_REQ * ncreateRequest,
		    MSG_N_CREATE_RESP * ncreateResponse,
		    void *ctx,
		    DUL_PRESENTATIONCONTEXT * presentationCtx);
static CONDITION
createBIBList(PRN_BASICFILMBOX * bfbPtr,
	      DUL_PRESENTATIONCONTEXT * presentationCtx);
/* createRequest
**
** Purpose:
**	Receive the N-CREATE Request and send the N-CREATE Response
**	to the requesting SCU. The routine also checks for which SOP class
**	is the request being made i.e. BASIC FILM SESSION or BASIC FILM BOX.
**
** Parameter Dictionary:
**	association	Key which describes the association
**	ctx		Pointer to context for this command
**	request		Pointer to N-CREATE request message
**
** Return Values:
**	SRV_NORMAL under error free conditions.
**
**	SRV_CALLBACKABORTEDSERVICE
**	SRV_ILLEGALPARAMETER
**	SRV_NOCALLBACK
**	SRV_NORMAL
**	SRV_OBJECTBUILDFAILED
**	SRV_RESPONSEFAILED
**
** Algorithm:
**	Determine the SOP Class for which the request is made.
**	Accordingly call SRV_NCreateResponse with the appropriate call back
**		function.
*/
CONDITION
createRequest(DUL_ASSOCIATIONKEY ** association,
	      DUL_PRESENTATIONCONTEXT * ctx, MSG_N_CREATE_REQ ** request,
	      CTNBOOLEAN * sendBack)
{
    MSG_N_CREATE_RESP
	createResponse;		/* N-CREATE response message to be sent */
    CONDITION
	cond = SRV_NORMAL;

    /*
     * Here we need to find what SOP class has been requested to be created.
     * This information is obtained from the classUID field
     */
    memset(&createResponse, 0, sizeof(MSG_N_CREATE_RESP));
    if (strcmp((*request)->classUID, DICOM_SOPCLASSBASICFILMSESSION) == 0) {
	cond = SRV_NCreateResponse(association, ctx, request, &createResponse,
				   ncreateBFSCallback, sendBack, ".");
	if (cond != SRV_NORMAL) {
	    printf("In createRequest : SRV_NCreateResponse failed\n");
	    return cond;
	}
	/* print current hierarchy */
	if (traceFlag) {
	    printf("AFTER creation of BASIC FILM SESSION \n");
	    showHierarchy();
	}
    } else if (strcmp((*request)->classUID, DICOM_SOPCLASSBASICFILMBOX) == 0) {
	cond = SRV_NCreateResponse(association, ctx, request, &createResponse,
				   ncreateBFBCallback, sendBack, ".");
	if (cond != SRV_NORMAL) {
	    printf("In createRequest : SRV_NCreateResponse failed\n");
	    return cond;
	}
	/* print current hierarchy */
	if (traceFlag) {
	    printf("AFTER creation of BASIC FILM BOX\n");
	    showHierarchy();
	}
    } else {
	/* unsupported service */
	cond = SRV_NCreateResponse(association, ctx, request, &createResponse,
				   unsupportedCallback, sendBack, ".");
	if (cond != SRV_NORMAL) {
	    printf("In createRequest : SRV_NCreateResponse failed\n");
	    return cond;
	}
    }
    return cond;
}


/* ncreateBFSCallback
**
** Purpose:
**	Call back function called by SRV_NCreateResponse to actually build the
**	BASIC FILM SESSION SOPinstance and its attributes and the response
**	message.
**
** Parameter Dictionary:
**	ncreateRequest	Pointer to the N-CREATE request message
**	ncreateResponse	Pointer to the N-CREATE response message
**	sendBack	Pointer to boolean flag (ignored here)
**	presentationCtx	Presentation context for this SOP class
**
** Return Values:
**	SRV_NORMAL under error free conditions
**
**	SRV_CALLBACKABORTEDSERVICE
**	SRV_NORMAL
**
** Algorithm:
**	Check that the SCU has sent all mandatory attributes and if any
**	optional attributes.
**	Further, verify that the SCP supports the optional attributes.
**	If all conditions are satisfied, then create a data set with attributes
**	requested by the SCU and additional optional attributes which the
**	SCP provides.
*/
static CONDITION
ncreateBFSCallback(MSG_N_CREATE_REQ * ncreateRequest,
		   MSG_N_CREATE_RESP * ncreateResponse,
/*	   CTNBOOLEAN * sendBack, */
		   void *sendBack,
		   DUL_PRESENTATIONCONTEXT * presentationCtx)
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
	{PRN_BFS_K_FILMSESSIONLABEL, "BFS Memory Allocation ",
	    bfsSupportedFilmSessionLabel,
	(int) DIM_OF(bfsSupportedFilmSessionLabel)}
    };

    /* we build up the response message */
    ncreateResponse->type = MSG_K_N_CREATE_RESP;
    ncreateResponse->messageIDRespondedTo = ncreateRequest->messageID;
    strcpy(ncreateResponse->classUID, ncreateRequest->classUID);
    ncreateResponse->conditionalFields |= MSG_K_N_CREATERESP_AFFECTEDCLASSUID;
    ncreateResponse->dataSetType = DCM_CMDDATANULL;
    ncreateResponse->conditionalFields = 0;
#ifdef ASG
    ncreateResponse->dataSet = NULL;
#endif

    /* Assign the instance SOP UID if necessary */
    if ((ncreateRequest->conditionalFields & MSG_K_N_CREATEREQ_INSTANCEUID)
	== 0) {
	/* invoking SCU had not supplied an instance UID for the SOP class */
	cond = UID_NewUID(UID_PRINTER, ncreateResponse->instanceUID);
	if (cond != UID_NORMAL) {
	    cond = COND_PushCondition(APP_ERROR(APP_FAILURE), "UID_NewUID",
				      "ncreateBFSCallback");
	    updateOnProcessingFailure((void **) &ncreateResponse,
		APP_ERROR(APP_FAILURE), "UID_NewUID", "ncreateBFSCallback");
	    goto ERROR_RETURN;
	}
	ncreateResponse->conditionalFields |= MSG_K_N_CREATERESP_AFFECTEDINSTANCEUID;
    } else {
	/*
	 * here we need to make sure that the instance UID provided by the
	 * invoking SCU doesn't conflict with any instances already
	 * registered with us
	 */
	ncreateResponse->conditionalFields |= MSG_K_N_CREATERESP_AFFECTEDINSTANCEUID;
	strcpy(ncreateResponse->instanceUID, ncreateRequest->instanceUID);
    }
    bfsPtr = &bfsAttrib;
    memset(bfsPtr, 0, sizeof(*bfsPtr));
    bfsPtr->bfsAttributeFlag = 0x0;
    (void) strcpy(bfsPtr->filmSessionSOPClassUID, ncreateResponse->classUID);
    (void) strcpy(bfsPtr->filmSessionSOPInstanceUID,
		  ncreateResponse->instanceUID);
    /* Parse the data set sent by SCU */
    if (ncreateRequest->dataSet != NULL) {
	cond = PRN_ParseObject(&ncreateRequest->dataSet, PRN_K_BASICFILMSESSION,
			       (void **) &bfsPtr);
	if (cond != PRN_NORMAL) {
	    cond = COND_PushCondition(APP_ERROR(APP_FAILURE), "PRN_ParseObject",
				      "ncreateBFSCallback");
	    updateOnProcessingFailure((void **) &ncreateResponse,
	    APP_ERROR(APP_FAILURE), "PRN_ParseObject", "ncreateBFSCallback");
	    goto ERROR_RETURN;
	}
    }
    if (traceFlag) {
	printf("Film Session Attributes Specified by SCU:\n");
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
    cond = supportedAttributes(MSG_K_N_CREATE_REQ, bfsPtr,
	       supportedAttributeList, (int) DIM_OF(supportedAttributeList),
			       &flagNameList, (void **) &ncreateResponse);
    if (cond != APP_NORMAL) {
	/*
	 * depending upon the condition returned, set the message status
	 * value
	 */
	switch (cond) {
	case APP_MISSINGREQUIREDATTRIBUTE:
	    ncreateResponse->status = MSG_K_MISSINGATTRIBUTE;
	    cond = COND_PushCondition(APP_ERROR(cond), flagNameList,
				      "ncreateBFSCallback");
	    goto ERROR_RETURN;
	case APP_UNSUPPORTEDMANDATORYATTRIBUTE:
	    ncreateResponse->status = MSG_K_NOSUCHATTRIBUTE;
	    cond = COND_PushCondition(APP_ERROR(cond), flagNameList,
				      "ncreateBFSCallback");
	    goto ERROR_RETURN;
	    /* for the next two cases we do not return error */
	case APP_UNSUPPORTEDOPTIONALATTRIBUTE:
	    /* ncreateResponse->status = MSG_K_NOSUCHATTRIBUTE; */
	    ncreateResponse->status = MSG_K_SUCCESS;
	    break;
	case APP_UNSUPPORTEDATTRIBUTEVALUE:
	    /* ncreateResponse->status = MSG_K_INVALIDATTRIBUTEVALUE; */
	    ncreateResponse->status = MSG_K_SUCCESS;
	    break;
	}
    }
    /*
     * At this point the verification process is successful Proceed by
     * creating the Basic Film Session
     */
    /* UPDATE hierarchy */
    cond = PRN_BuildObject((void *) bfsPtr, &bfsInstance.bfsAttrib);
    if (cond != PRN_NORMAL) {
	cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				  "PRN_BuildObject", "ncreateBFSCallback");
	updateOnProcessingFailure((void **) &ncreateResponse,
	   APP_ERROR(APP_FAILURE), "PRN_BuildObject", "ncreateBFSCallback");
	goto ERROR_RETURN;
    }
    strcpy(bfsInstance.bfsInstanceUID, ncreateResponse->instanceUID);
    /* acquire space to store basic film boxes */
    bfsInstance.bfbList = LST_Create();
    if (bfsInstance.bfbList == NULL) {
	cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				  "LST_Create", "ncreateBFSCallback");
	updateOnProcessingFailure((void **) &ncreateResponse,
		APP_ERROR(APP_FAILURE), "LST_Create", "ncreateBFSCallback");
	goto ERROR_RETURN;
    }
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
				      "DCM_WriteFile", "ncreateBFSCallback");
	    updateOnProcessingFailure((void **) &ncreateResponse,
	     APP_ERROR(APP_FAILURE), "DCM_WriteFile", "ncreateBFSCallback");
	    goto ERROR_RETURN;
	}
	cond = GQ_Enqueue(gqID, (void *) &gqElement);
	if (cond != GQ_NORMAL) {
	    unlink(gqElement.objFile);
	    cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				      "GQ_Enqueue", "ncreateBFSCallback");
	    updateOnProcessingFailure((void **) &ncreateResponse,
				      APP_ERROR(APP_FAILURE),
				      "GQ_Enqueue", "ncreateBFSCallback");
	    goto ERROR_RETURN;
	}
    }
    /* Attributes need to be sent back */
    cond = PRN_BuildObject((void *) bfsPtr, &ncreateResponse->dataSet);
    if (cond != PRN_NORMAL) {
	cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				  "PRN_BuildObject", "ncreateBFSCallback");
	updateOnProcessingFailure((void **) &ncreateResponse,
	   APP_ERROR(APP_FAILURE), "PRN_BuildObject", "ncreateBFSCallback");
	goto ERROR_RETURN;
    }
    ncreateResponse->dataSetType = DCM_CMDDATAOTHER;
    /* everything is succesful */
    ncreateResponse->status = MSG_K_SUCCESS;
    if (traceFlag) {
	printf("Returning SRV_NORMAL in ncreateBFSCallback\n");
	MSG_DumpMessage(ncreateResponse, stdout);
    }
    return SRV_NORMAL;

    /*
     * This is the exit point for all errors that may have occurred in the
     * callback routine
     */
ERROR_RETURN:
    COND_DumpConditions();
    if (traceFlag) {
	printf("Returning SRV_CALLBACKABORTEDSERVICE in ncreateBFSCallback\n");
	MSG_DumpMessage(ncreateResponse, stdout);
    }
    return SRV_CALLBACKABORTEDSERVICE;
}

/* ncreateBFBCallback
**
** Purpose:
**	Call back function called by SRV_NCreateResponse to build the
**	N-CREATE Response message for the BASIC FILM BOX SOP instance
**
** Parameter Dictionary:
**	ncreateRequest	Pointer to the N-CREATE request message
**	ncreateResponse	Pointer to the N-CREATE response message
**	sendBack	Pointer to boolean variable (ignored here)
**	presentationCtx	Pointer to presentation context describing this class
**
** Return Values:
**	SRV_NORMAL in error free conditions
**
**	SRV_CALLBACKABORTEDSERVICE
**	SRV_NORMAL
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
ncreateBFBCallback(MSG_N_CREATE_REQ * ncreateRequest,
		   MSG_N_CREATE_RESP * ncreateResponse,
/*	   CTNBOOLEAN * sendBack, */
		   void *sendBack,
		   DUL_PRESENTATIONCONTEXT * presentationCtx)
{
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

    /*
     * We do not have a structure for imageDisplayFormat since it is
     * impossible to list all combinations statically. We do a run-time check
     * on it. Instead, we use "supportedAttributes" to achieve a side-effect
     * in which we pass a pointer to the APP_BFBIMAGEDISPLAYFORMAT structure
     * and get the format structure in this variable
     */
    static APP_BFBIMAGEDISPLAYFORMAT
        bfbImageDisplayFormat;	/* this structure gets filled by the
				 * verification service as a side-effect */


    /* define the table of all attributes that need to be verified */
    static SUPPORTEDATTRIBUTES supportedAttributeList[] = {
	{PRN_BFB_K_IMAGEDISPLAYFORMAT, "BFB Image Display Format ",
	&bfbImageDisplayFormat, 0},
	{PRN_BFB_K_FILMORIENTATION, "BFB Film Orientation ",
	bfbFilmOrientation, (int) DIM_OF(bfbFilmOrientation)},
	{PRN_BFB_K_FILMSIZEID, "BFB Film Size ID ",
	bfbFilmSizeID, (int) DIM_OF(bfbFilmSizeID)},
	{PRN_BFB_K_MAGNIFICATIONTYPE, "BFB Magnification Type ",
	bfbMagnificationType, (int) DIM_OF(bfbMagnificationType)},
	{PRN_BFB_K_MAXDENSITY, "BFB Max Density ", NULL, 0},
	{PRN_BFB_K_CONFIGURATIONINFO, "BFB Configuration Information ",
	bfbConfigurationInfo, (int) DIM_OF(bfbConfigurationInfo)},
	{PRN_BFB_K_BORDERDENSITY, "BFB Border Density ",
	bfbBorderDensity, (int) DIM_OF(bfbBorderDensity)},
	{PRN_BFB_K_EMPTYIMAGEDENSITY, "BFB Empty Image Density ",
	bfbEmptyImageDensity, (int) DIM_OF(bfbEmptyImageDensity)},
	{PRN_BFB_K_MINDENSITY, "BFB Min Density ", NULL, 0},
	{PRN_BFB_K_TRIM, "BFB Trim ", bfbTrim, (int) DIM_OF(bfbTrim)},
	{PRN_BFB_K_REFERENCEDBASICFILMSESSIONSEQ,
	    "BFB Referenced Film Session Sequence ", bfbRefFilmSessionSequence,
	(int) DIM_OF(bfbRefFilmSessionSequence)},
	{PRN_BFB_K_REFERENCEDBASICIMAGEBOXSEQ,
	"BFB Referenced Image Box Sequence ", NULL, 0}
    };

    /* we build up the response message */
    ncreateResponse->type = MSG_K_N_CREATE_RESP;
    ncreateResponse->status = MSG_K_SUCCESS;
    ncreateResponse->messageIDRespondedTo = ncreateRequest->messageID;
    ncreateResponse->dataSetType = DCM_CMDDATANULL;
    ncreateResponse->conditionalFields = 0;
    strcpy(ncreateResponse->classUID, ncreateRequest->classUID);
#ifdef ASG
    ncreateResponse->dataSet = NULL;
#endif
    /* Assign the instance SOP UID if necessary */
    if ((ncreateRequest->conditionalFields & MSG_K_N_CREATEREQ_INSTANCEUID)
	== 0) {
	/*
	 * requesting SCU has not provided the instance UID. The SCP assigns
	 * an instance UID
	 */
	cond = UID_NewUID(UID_PRINTER, ncreateResponse->instanceUID);
	if (cond != UID_NORMAL) {
	    cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				      "UID_NewUID", "ncreateBFBCallback");
	    updateOnProcessingFailure((void **) &ncreateResponse,
		APP_ERROR(APP_FAILURE), "UID_NewUID", "ncreateBFBCallback");
	    goto ERROR_RETURN;
	}
	ncreateResponse->conditionalFields |= MSG_K_N_CREATERESP_AFFECTEDINSTANCEUID;
    } else {
	/*
	 * here we need to make sure that the instance UID provided by the
	 * invoking SCU doesn't conflict with any instances already
	 * registered with us
	 */
	ncreateResponse->conditionalFields |= MSG_K_N_CREATERESP_AFFECTEDINSTANCEUID;
	strcpy(ncreateResponse->instanceUID, ncreateRequest->instanceUID);
    }

    /* Parse the received data set */
    memset(&bfbAttrib, 0, sizeof(bfbAttrib));
    bfbPtr = &bfbAttrib;
    bfbPtr->bfbAttributeFlag = 0x0;
    (void) strcpy(bfbPtr->filmBoxSOPClassUID, ncreateRequest->classUID);
    (void) strcpy(bfbPtr->filmBoxSOPInstanceUID, ncreateResponse->instanceUID);
    if (ncreateRequest->dataSet != NULL) {
	cond = PRN_ParseObject(&ncreateRequest->dataSet, PRN_K_BASICFILMBOX,
			       (void **) &bfbPtr);
	if (cond != PRN_NORMAL) {
	    cond = COND_PushCondition(APP_ERROR(APP_FAILURE), "PRN_ParseObject",
				      "ncreateBFBCallback");
	    updateOnProcessingFailure((void **) &ncreateResponse,
	    APP_ERROR(APP_FAILURE), "PRN_ParseObject", "ncreateBFBCallback");
	    goto ERROR_RETURN;
	}
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
			       &flagNameList, (void **) &ncreateResponse);
    if (cond != APP_NORMAL) {
	/*
	 * depending upon the condition returned, set the message status
	 * value
	 */
	switch (cond) {
	case APP_MISSINGREQUIREDATTRIBUTE:
	    ncreateResponse->status = MSG_K_MISSINGATTRIBUTE;
	    cond = COND_PushCondition(APP_ERROR(cond), flagNameList,
				      "ncreateBFBCallback");
	    goto ERROR_RETURN;
	case APP_UNSUPPORTEDMANDATORYATTRIBUTE:
	    cond = COND_PushCondition(APP_ERROR(cond), flagNameList,
				      "ncreateBFBCallback");
	    goto ERROR_RETURN;
	case APP_UNSUPPORTEDOPTIONALATTRIBUTE:
	    /* ncreateResponse->status = MSG_K_NOSUCHATTRIBUTE; */
	    ncreateResponse->status = MSG_K_SUCCESS;
	    break;
	case APP_UNSUPPORTEDATTRIBUTEVALUE:
	    /* ncreateResponse->status = MSG_K_INVALIDATTRIBUTEVALUE; */
	    ncreateResponse->status = MSG_K_SUCCESS;
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
				  "LST_Create", "ncreateBFBCallback");
	updateOnProcessingFailure((void **) &ncreateResponse,
		APP_ERROR(APP_FAILURE), "LST_Create", "ncreateBFBCallback");
	goto ERROR_RETURN;
    }
    /*
     * We also update the hierarchy. Create a copy of the BFB instance
     */
    if ((bfbInstance = malloc(sizeof(*bfbInstance))) == NULL) {
	cond = COND_PushCondition(APP_ERROR(APP_MALLOCFAILURE),
				  "ncreateBFBCallback");
	updateOnProcessingFailure((void **) &ncreateResponse,
			APP_ERROR(APP_MALLOCFAILURE), "ncreateBFBCallback");
	goto ERROR_RETURN;
    }
    (void) strcpy(bfbInstance->bfbInstanceUID, ncreateResponse->instanceUID);
    bfbInstance->bfbAttrib = NULL;

    /* now acquire space to store the list of image boxes */
    bfbInstance->babList = NULL;/* we do not have an annotation box */
    bfbInstance->bibList = LST_Create();
    if (bfbInstance->bibList == NULL) {
	cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				  "LST_Create", "ncreateBFBCallback");
	updateOnProcessingFailure((void **) &ncreateResponse,
		APP_ERROR(APP_FAILURE), "LST_Create", "ncreateBFBCallback");
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
		cond = createBIBList(bfbPtr, presentationCtx);
		if (cond != APP_NORMAL) {
		    cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				     "createBIBList", "ncreateBFBCallback");
		    updateOnProcessingFailure((void **) &ncreateResponse,
				    APP_ERROR(APP_FAILURE), "createBIBList",
					      "ncreateBFBCallback");
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
		cond = createBIBList(bfbPtr, presentationCtx);
		if (cond != APP_NORMAL) {
		    cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				     "createBIBList", "ncreateBFBCallback");
		    updateOnProcessingFailure((void **) &ncreateResponse,
				    APP_ERROR(APP_FAILURE), "createBIBList",
					      "ncreateBFBCallback");
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
		cond = createBIBList(bfbPtr, presentationCtx);
		if (cond != APP_NORMAL) {
		    cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				     "createBIBList", "ncreateBFBCallback");
		    updateOnProcessingFailure((void **) &ncreateResponse,
				    APP_ERROR(APP_FAILURE), "createBIBList",
					      "ncreateBFBCallback");
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
    ncreateResponse->dataSetType = DCM_CMDDATAOTHER;
    cond = PRN_BuildObject((void *) bfbPtr, &ncreateResponse->dataSet);
    if (cond != PRN_NORMAL) {
	cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				  "PRN_BuildObject", "ncreateBFBCallback");
	updateOnProcessingFailure((void **) &ncreateResponse,
	   APP_ERROR(APP_FAILURE), "PRN_BuildObject", "ncreateBFBCallback");
	goto ERROR_RETURN;
    }
    /* also add this to the hierarchy */
    cond = PRN_BuildObject((void *) bfbPtr, &bfbInstance->bfbAttrib);
    if (cond != PRN_NORMAL) {
	cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				  "PRN_BuildObject", "ncreateBFBCallback");
	updateOnProcessingFailure((void **) &ncreateResponse,
	   APP_ERROR(APP_FAILURE), "PRN_BuildObject", "ncreateBFBCallback");
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
				      "DCM_WriteFile", "ncreateBFBCallback");
	    updateOnProcessingFailure((void **) &ncreateResponse,
	     APP_ERROR(APP_FAILURE), "DCM_WriteFile", "ncreateBFBCallback");
	    goto ERROR_RETURN;
	}
	cond = GQ_Enqueue(gqID, (void *) &gqElement);
	if (cond != GQ_NORMAL) {
	    unlink(gqElement.objFile);
	    cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				      "GQ_Enqueue", "ncreateBFBCallback");
	    updateOnProcessingFailure((void **) &ncreateResponse,
				      APP_ERROR(APP_FAILURE),
				      "GQ_Enqueue", "ncreateBFBCallback");
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
				  "LST_Enqueue", "ncreateBFBCallback");
	updateOnProcessingFailure((void **) &ncreateResponse,
	       APP_ERROR(APP_FAILURE), "LST_Enqueue", "ncreateBFBCallback");
	goto ERROR_RETURN;
    }
    /* Attributes need to be sent back */
    cond = PRN_BuildObject((void *) bfbPtr, &ncreateResponse->dataSet);
    if (cond != PRN_NORMAL) {
	cond = COND_PushCondition(APP_ERROR(APP_FAILURE),
				  "PRN_BuildObject", "ncreateBFBCallback");
	updateOnProcessingFailure((void **) &ncreateResponse,
	   APP_ERROR(APP_FAILURE), "PRN_BuildObject", "ncreateBFBCallback");
	goto ERROR_RETURN;
    }
    /* everything is successful (may have warnings) */
    ncreateResponse->dataSetType = DCM_CMDDATAOTHER;
#ifdef ASG
    ncreateResponse->status = MSG_K_SUCCESS;
#endif
    if (traceFlag) {
	printf("Returning SRV_NORMAL in ncreateBFBCallback\n");
	MSG_DumpMessage(ncreateResponse, stdout);
    }
    return SRV_NORMAL;

ERROR_RETURN:
    COND_DumpConditions();
    if (traceFlag) {
	printf("Returning SRV_CALLBACKABORTEDSERVICE in ncreateBFBCallback\n");
	MSG_DumpMessage(ncreateResponse, stdout);
    }
    return SRV_CALLBACKABORTEDSERVICE;
}

/* createBIBList
**
** Purpose:
**	Creates a sequence of Basic Image Boxes depending on the number of
**	times it is invoked by the parent "ncreateBFBCallback" routine.
**
** Parameter Dictionary:
**	bfbPtr		Pointer to a structure that holds attributes of
**			Basic Film Box.
**
** Return Values:
**	APP_NORMAL if there are no errors.
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
createBIBList(PRN_BASICFILMBOX * bfbPtr,
	      DUL_PRESENTATIONCONTEXT * presentationCtx)
{
    CONDITION
    cond;
    SQ_REFPRINT
	* refItem = NULL;	/* holds the basic image box ref item */
    BIB_NODE
	* bibInstance = NULL;	/* holds an instance of basic image box */

    if ((refItem = malloc(sizeof(SQ_REFPRINT))) == NULL) {
	return COND_PushCondition(APP_ERROR(APP_MALLOCFAILURE),
				  "createBIBList");
    }
    /* generate new instance UID */
    (void) UID_NewUID(UID_PRINTER, refItem->refSOPInstanceUID);
    refItem->type = SQ_K_REFBASICIMAGEBOX;
#ifdef ASG
    (void) strcpy(refItem->refSOPClassUID, DICOM_SOPCLASSBASICGREYSCALEIMAGEBOX);
#endif
    (void) strcpy(refItem->refSOPClassUID, presentationCtx->abstractSyntax);

    /* insert the reference item to the list */
    cond = LST_Enqueue(&bfbPtr->referencedBIBSeq, refItem);
    if (cond != LST_NORMAL) {
	return COND_PushCondition(APP_ERROR(APP_FAILURE),
				  "LST_Enqueue", "createBIBList");
    }
    /* update hierarchy */
    if ((bibInstance = malloc(sizeof(BIB_NODE))) == NULL) {
	return COND_PushCondition(APP_ERROR(APP_MALLOCFAILURE), "createBIBList");
    }
    (void) strcpy(bibInstance->bibInstanceUID, refItem->refSOPInstanceUID);
    bibInstance->bibAttrib = NULL;
    cond = LST_Enqueue(&bfbInstance->bibList, bibInstance);
    if (cond != LST_NORMAL) {
	return COND_PushCondition(APP_ERROR(APP_FAILURE),
				  "LST_Enqueue", "createBIBList");
    }
    return APP_NORMAL;
}
/* unsupportedCallback
**
** Purpose:
**	This callback routine is invoked by the SRV_NSetResponse
**	whenever service for the requested SOP class is unsupported
**
** Parameter Dictionary:
**	ncreateRequest	Pointer to request message
**	ncreateResponse	Pointer to response message
**	ctx		Application context
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
unsupportedCallback(MSG_N_CREATE_REQ * ncreateRequest,
		    MSG_N_CREATE_RESP * ncreateResponse,
		    void *ctx, DUL_PRESENTATIONCONTEXT * presentationCtx)
{
    ncreateResponse->status = MSG_K_UNRECOGNIZEDOPERATION;
    return SRV_CALLBACKABORTEDSERVICE;
}
