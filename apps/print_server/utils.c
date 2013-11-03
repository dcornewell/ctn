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
** Module Name(s):	decodeImageDisplayFormat
**			subset
**			getNewFileName
**			showHierarchy
**			deleteHierarchy
**			updateFlagName
**			updateOnProcessingFailure
**			updateAttributeIdentifierList
**	Private Routines
**			freeBFBinstance
** Author, Date:	Aniruddha S. Gokhale, 30-June-1994
** Intent:		This module provides support routines that are used
**			in other modules.
** Last Update:		$Author: smm $, $Date: 1998-08-03 16:22:05 $
** Source File:		$RCSfile: utils.c,v $
** Revision:		$Revision: 1.29 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.29 $ $RCSfile: utils.c,v $";
#include "print_server.h"

extern CTNBOOLEAN traceFlag,
    gqueueFlag,
    sendBack;
extern BFS_NODE bfsInstance;
extern BFB_NODE *bfbInstance;

static int freeBFBinstance(BFB_NODE ** bfbNode);

/* decodeImageDisplayFormat
**
** Purpose:
**      Given the image display format for an IMAGE BOX, decode the
**      format type and determine the number of row and/or columns of
**      images.
** Parameter Dictionary:
**      imageDisplayFormat   contains the image display format
**	displayFormat	Pointer to structure that holds values after
**			parsing the image display format attribute
**
** Return Values:
**	TRUE if the format is supported and all values of rows/columns
**	or number of images per row/col are within the accepted range
**
**	FALSE, otherwise
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/
#ifdef ASG
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


    if ((imgDisplay = malloc(sizeof(*imgDisplay))) == NULL) {
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
#endif
CTNBOOLEAN
decodeImageDisplayFormat(char *imageDisplayFormat,
			 APP_BFBIMAGEDISPLAYFORMAT * displayFormat)
{
    int
        t[MAXIMAGES];		/* for a possible max 4 images */
    int
        rows,
        cols;			/* total rows and cols in STANDARD format */
    int
        count;			/* for #rows or #columns */
    int
        i;

    /*
     * scan the Basic Film Box image display format attribute and retrieve
     * the TYPE of format and other relevent information in the argument
     * displayFormat
     */
    if (sscanf(imageDisplayFormat, "STANDARD\\%d,%d", &cols, &rows) > 0) {
	if ((cols < 0) || (cols > 8) || (rows < 0) || (rows > 8))
	    return FALSE;

	displayFormat->type = STANDARD_FORMAT;
	displayFormat->format.standard.numRows = rows;
	displayFormat->format.standard.numColumns = cols;
    } else if ((count =
		sscanf(imageDisplayFormat, "ROW\\%d,%d,%d,%d,%d,%d,%d,%d",
	     &t[0], &t[1], &t[2], &t[3], &t[4], &t[5], &t[6], &t[7])) > 0) {
	for (i = 0; i < count; i++) {
	    if ((t[i] < 0) || (t[i] > 8))
		return FALSE;
	}

	displayFormat->type = ROW_FORMAT;
	displayFormat->format.row.numRows = count;
	for (i = 0; i < count; i++)
	    displayFormat->format.row.imagesPerRow[i] = t[i];
    } else if ((count =
		sscanf(imageDisplayFormat, "COL\\%d,%d,%d,%d,%d,%d,%d,%d",
	     &t[0], &t[1], &t[2], &t[3], &t[4], &t[5], &t[6], &t[7])) > 0) {
	for (i = 0; i < count; i++) {
	    if ((t[i] < 0) || (t[i] > 8))
		return FALSE;
	}

	displayFormat->type = COL_FORMAT;
	displayFormat->format.col.numColumns = count;
	for (i = 0; i < count; i++)
	    displayFormat->format.col.imagesPerColumn[i] = t[i];
    } else {
	return FALSE;
    }
    return TRUE;
}
/* subset
**
** Purpose:
**	Find if the first flag argument is completely covered by the
**	second flag argument
**
** Parameter Dictionary:
**	flag
**	superflag
**
** Return Values:
**	failure or success
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
short
subset(unsigned long flag, unsigned short superflag)
{
    while (flag != 0) {
	if (flag & 0x1) {
	    if (!(superflag & 0x1))
		return 0;
	}
	flag /= 2;
	superflag /= 2;
    }
    return 1;
}

/* getNewFileName
**
** Purpose:
**	Generate a new file name for the current process.
**
** Parameter Dictionary:
**	None
**
** Return Values:
**	File name
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
char *
getNewFileName(void)
{

    static int
        fileExtension = 1;
    int
        pid;
    static char
        fileName[256];

    /* generate a unique file name */
    pid = getpid();
    sprintf(fileName, "PRN_%d.%d", pid, fileExtension);
    strcat(fileName, "\0");
    fileExtension++;

    return fileName;
}

/* deleteHierarchy
**
** Purpose:
**	Delete the hierarchy starting at the requested root.
**
** Parameter Dictionary:
**	classUID	SOP class UID of the instance to be deleted
**
** Return Values:
**	Success or failure
**	DCM_STATUS_FAILED
**
** Algorithm:
**	Determine which SOP Class instance is to be deleted.
**	If it is a film session instance,
**		Delete each film box in the film session hierarchy first
**		Delete the Film session node
**	Else it is a film box to be deleted
**		Search for the film box in the hierarchy
**		Delete the film box by deleteing all its children first
**
*/
CONDITION
deleteHierarchy(char *classUID)
{
    int
        count;
    int
        index;
    CONDITION
	cond;
    BFB_NODE
	* bfbNode;
    short
        failOperation = 0;

    /*
     * here we need to find out for which SOP class has the request being
     * made.
     */
    if (strcmp(classUID, DICOM_SOPCLASSBASICFILMSESSION) == 0) {
	/* BASIC FILM SESSION instance to be deleted */
	(void) strcpy(bfsInstance.bfsInstanceUID, "\0");
	cond = DCM_CloseObject(&bfsInstance.bfsAttrib);
	/* remove attribute object */
	if (cond != DCM_NORMAL)
	    failOperation++;

	if (bfsInstance.bfbList == NULL) {
	    /* already deleted */
	    return APP_NORMAL;
	}
	bfbNode = LST_Position(&bfsInstance.bfbList,
			       LST_Head(&bfsInstance.bfbList));
	if (bfbNode == NULL) {
	    printf("No BASIC FILM BOX instances in the list to delete\n");
	}
	count = LST_Count(&bfsInstance.bfbList);
	for (index = 0; index < count; index++) {
	    if (traceFlag)
		printf("Deleting Child FILM BOX\n");
	    bfbNode = LST_Remove(&bfsInstance.bfbList, LST_K_AFTER);
	    /* now free the bfbNode and its list of image boxes */
	    if (bfbNode == NULL)
		failOperation++;
	    else
		failOperation += freeBFBinstance(&bfbNode);
	}
	cond = LST_Destroy(&bfsInstance.bfbList);
	if (cond != LST_NORMAL)
	    failOperation++;
	if (failOperation > 0) {
	    printf("Total delete operations failed = %d\n", failOperation);
	    return APP_FAILURE;
	}
	if (traceFlag)
	    printf("FILM SESSION INSTANCE DELETED\n");
	return APP_NORMAL;	/* otherwise */
    } else if (strcmp(classUID, DICOM_SOPCLASSBASICFILMBOX) == 0) {
	/* last created BASIC FILM BOX instance to be deleted */
	/*
	 * Here we first search the FILM BOX list in the FILM SESSION to
	 * first get to the current FILM BOX. Then we remove it and delete
	 * it.
	 */
	count = LST_Count(&bfsInstance.bfbList);
	bfbNode = LST_Position(&bfsInstance.bfbList,
			       LST_Head(&bfsInstance.bfbList));
	if (bfbNode == NULL) {
	    printf("No BASIC FILM BOX instance in the list to delete\n");
	    return APP_FAILURE;
	}
	for (index = 0; index < count; index++) {
	    bfbNode = LST_Remove(&bfsInstance.bfbList, LST_K_AFTER);
	    if (bfbNode == NULL) {
		printf("Desired FILM BOX instance not in FILM SESSION list\n");
		return APP_FAILURE;
	    }
	    if (strcmp(bfbInstance->bfbInstanceUID,
		       bfbNode->bfbInstanceUID) == 0) {	/* this node to be
							 * removed */
		failOperation += freeBFBinstance(&bfbNode);
	    }
	}
	count = LST_Count(&bfsInstance.bfbList);
	if (failOperation > 0) {
	    return APP_FAILURE;
	}
	return APP_NORMAL;	/* otherwise */
    }
    return APP_FAILURE;		/* if there is no match */
}

/* freeBFBinstance
**
** Purpose:
**	Free a single Film box instance
**
** Parameter Dictionary:
**	bfbNode		Address of pointer to film box node to be deleted
**
** Return Values:
**	success or failure. 0 => success. Any positive value indicates
**	that many errors.
**
** Algorithm:
**	First delete all Image box instances and Annotation box instances (if
**	any) and then delete the Film box node.
*/
static int
freeBFBinstance(BFB_NODE ** bfbNode)
{
    int
        count;
    int
        index;
    BIB_NODE
	* bibNode = NULL;
    BAB_NODE
	* babNode = NULL;
    int
        failOperation = 0;
    CONDITION
	cond;

    (void) strcpy((*bfbNode)->bfbInstanceUID, "\0");
    /* first free the attribute object */
    cond = DCM_CloseObject(&(*bfbNode)->bfbAttrib);
    if (cond != DCM_NORMAL) {
	printf("In freeBFBInstance, DCM_CloseObject of BFB attrib failed\n");
	failOperation++;
    }
    count = LST_Count(&(*bfbNode)->bibList);
    bibNode = LST_Position(&(*bfbNode)->bibList,
			   LST_Head(&(*bfbNode)->bibList));
    if (bibNode == NULL) {
	printf("No Image Boxes in the Film Box\n");
	failOperation++;
    }
    /* destroy individual elements in the list first */
    for (index = 0; index < count; index++) {
	bibNode = LST_Remove(&(*bfbNode)->bibList, LST_K_AFTER);
	(void) strcpy(bibNode->bibInstanceUID, "\0");
	if (bibNode->bibAttrib == NULL)
	    continue;
	cond = DCM_CloseObject(&bibNode->bibAttrib);
	free(bibNode);
	if (cond != DCM_NORMAL) {
	    printf("In freeBFBInstance, DCM_CloseObject of BIB attrib failed\n");
	    failOperation++;
	}
    }
    /* now free babNode if any */
    if ((*bfbNode)->babList != NULL) {
	count = LST_Count(&(*bfbNode)->babList);
	babNode = LST_Position(&(*bfbNode)->babList,
			       LST_Head(&(*bfbNode)->bibList));
	if (babNode == NULL) {
	    printf("No Annotation Boxes in the Film Box\n");
	}
	/* destroy individual elements in the list first */
	for (index = 0; index < count; index++) {
	    babNode = LST_Remove(&(*bfbNode)->babList, LST_K_AFTER);
	    (void) strcpy(babNode->babInstanceUID, "\0");
	    if (babNode->babAttrib == NULL)
		continue;
	    cond = DCM_CloseObject(&babNode->babAttrib);
	    free(babNode);
	    if (cond != DCM_NORMAL)
		failOperation++;
	}
    }
    cond = LST_Destroy(&(*bfbNode)->bibList);
    if (cond != LST_NORMAL)
	failOperation++;
    if ((*bfbNode)->babList != NULL) {
	cond = LST_Destroy(&(*bfbNode)->babList);
	if (cond != LST_NORMAL)
	    failOperation++;
    }
    return failOperation;
}
/* showHierarchy
**
** Purpose:
**	prints the current print management hierarchy
**
** Parameter Dictionary:
**	None
**
** Return Values:
**	None
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
void
showHierarchy()
{
    int
        index;
    int
        index2;
    CONDITION
	cond;
    LST_NODE
	* lstNode;
    BFB_NODE
	* bfbNode;
    BIB_NODE
	* bibNode;

    printf("PRINT MANAGEMENT SOP CLASS HIERARCHY\n");
    printf("------------------------------------\n\n");

    if (strcmp(bfsInstance.bfsInstanceUID, "\0") == 0) {
	printf("Hierarchy Doesnot exists\n");
	return;
    }
    printf("Basic Film Session SOP Instance UID : %s\n",
	   bfsInstance.bfsInstanceUID);
    printf("  Attributes : \n");
    cond = DCM_DumpElements(&bfsInstance.bfsAttrib, 0);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	return;
    }
    printf("\n\n");

    printf("\tChildren Basic Film Box Instances : \n");
    printf("\t---------------------------------\n");
    lstNode = LST_Head(&bfsInstance.bfbList);
    if (lstNode == NULL) {
	printf("\tChildren Basic Film Boxes do not exist currently\n\n");
	return;
    }
    if ((bfbNode = LST_Position(&bfsInstance.bfbList, lstNode)) ==
	NULL) {
	printf("LST_Position failed in showHierarchy\n");
	COND_DumpConditions();
    }
    for (index = 0; index < (int) LST_Count(&bfsInstance.bfbList); index++) {
	if (strlen(bfbNode->bfbInstanceUID) == 0) {
	    printf("Hierarchy Doesnot exists\n");
	    continue;
	}
	printf("\tBasic Film Box SOP instance UID : %s\n",
	       bfbNode->bfbInstanceUID);
	printf("\t  Attributes : \n");
	cond = DCM_DumpElements(&bfbNode->bfbAttrib, 0);
	if (cond != DCM_NORMAL) {
	    COND_DumpConditions();
	    return;
	}
	printf("\n\t\tChildren Basic Image Boxes\n");
	printf("\t\t--------------------------\n");
	bibNode = (BIB_NODE *) LST_Position(&bfbNode->bibList,
					    LST_Head(&bfbNode->bibList));
	if (bibNode == NULL) {
	    printf("LST_Position returned NULL for bibNode\n");
	    return;
	}
	for (index2 = 0; index2 < (int) LST_Count(&bfbNode->bibList); index2++) {
	    printf("\t\tBasic Image Box SOP instance UID : %s\n",
		   bibNode->bibInstanceUID);
	    printf("\t\t  Attributes :\n\n");
	    cond = DCM_DumpElements(&bibNode->bibAttrib, 0);
	    if (cond != DCM_NORMAL)
		if (cond == DCM_NULLOBJECT)
		    printf("\t\t\tCurrently No Attributes present\n");
		else {
		    COND_DumpConditions();
		    return;
		}
	    bibNode = (BIB_NODE *) LST_Next(&bfbNode->bibList);
	}
	printf("\n");
	bfbNode = (BFB_NODE *) LST_Next(&bfsInstance.bfbList);
    }
    printf("\n\n###############################################################################\n\n");
}

/* kill_queue
**
** Purpose:
**	Describe the purpose of the function
**
** Parameter Dictionary:
**	qID	: The qID of the GQ that needs to be killed
**
** Return Values:
**	GQ_NORMAL if successful
**
**	GQ_NOPENQUEUE
**	GQ_NORMAL
**	GQ_QUEUEFULL
**	GQ_SEMAPHOREFAIL
**	GQ_SHAREDMEMORYFAIL
**	GQ_UNIMPLEMENTED
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
kill_queue(int qID)
{
    GQ_ELEM
	elem;
    CONDITION
	cond;

    cond = GQ_NORMAL;
    while (cond == GQ_NORMAL) {
	cond = GQ_Dequeue(qID, &elem);	/* dequeue the next element */
	/* check if there is some error */
	if ((cond != GQ_NORMAL) && (cond != GQ_QUEUEEMPTY)) {
	    return cond;
	} else if (cond == GQ_NORMAL) {	/* no error and Q not empty */
	    if (strlen(elem.objFile) != 0)	/* file exists */
		unlink(elem.objFile);
	}
    }
    strcpy(elem.objFile, KILL_DISPLAY);
    cond = GQ_Enqueue(qID, (void *) &elem);
    if (cond != GQ_NORMAL) {
	return cond;
    }
#ifdef _MSC_VER
    Sleep(5000);
#else
    sleep(5);
#endif
    cond = GQ_KillQueue(qID);
    if (cond != GQ_NORMAL) {
	return cond;
    }
    return GQ_NORMAL;
}

void
updateFlagName(char **flagName,...)
{
    char
       *args[10];
    int
        argno = 0;
    va_list
	ap;
    int
        orgLen,
        newLen;

/*lint -e40*/
/*lint -e50*/
    va_start(ap, flagName);
/*lint +e40*/
/*lint +e50*/

    if (*flagName) {
	orgLen = strlen(*flagName);
	newLen = orgLen + 2;
	*flagName = realloc(*flagName, newLen);
	memset(&((*flagName)[orgLen]), 0, (newLen - orgLen));
	strcat(*flagName, ",");
    }
    while ((args[argno++] = va_arg(ap, char *)) != (char *) 0) {
	if (*flagName) {
	    orgLen = strlen(*flagName);
	    newLen = orgLen + strlen(args[argno - 1]) + 1;
	    *flagName = realloc(*flagName, newLen);
	} else {
	    orgLen = 0;
	    newLen = orgLen + strlen(args[argno - 1]) + 1;
	    *flagName = malloc(newLen);
	}
	memset(&((*flagName)[orgLen]), 0, (newLen - orgLen));
	strcat(*flagName, args[argno - 1]);
    }
    va_end(ap);
}

/* updateOnProcessingFailure
**
** Purpose:
**	This routine updates the optional fields of a response message on
**	confronting a general processing failure
**
** Parameter Dictionary:
**	response	Address of pointer to response message whose optional
**			fields are to be modified
**	id		The error ID to be set
**	...		Should be a variable number of strings terminated by
**			a NULL
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Updates fields such as errorID, errorComment and status field.
*/
void
updateOnProcessingFailure(void **response, unsigned long id,
			  char *controlStr,...)
{
    va_list
    ap;
    MSG_GENERAL
	* msg = (MSG_GENERAL *) (*response);	/* we assume this is not NULL */
    char
        comment[DICOM_LO_LENGTH + 1];

    /* the response parameter must be one of this */
    MSG_N_EVENT_REPORT_RESP
	* event;
    MSG_N_GET_RESP
	* get;
    MSG_N_SET_RESP
	* set;
    MSG_N_ACTION_RESP
	* action;
    MSG_N_CREATE_RESP
	* create;
    MSG_N_DELETE_RESP
	* delete;
/*lint -e40*/
/*lint -e50*/
    va_start(ap, controlStr);
/*lint +e40*/
/*lint +e50*/
    switch (msg->type) {
    case MSG_K_N_EVENT_REPORT_RESP:
	event = (MSG_N_EVENT_REPORT_RESP *) msg;
	event->status = MSG_K_PROCESSINGFAILURE;
	event->errorID = id;
	vsprintf(event->errorComment,
		 controlStr, ap);
	event->conditionalFields |= MSG_K_N_EVENTREPORTRESP_ERRORCOMMENT |
	    MSG_K_N_EVENTREPORTRESP_ERRORID;
	break;
    case MSG_K_N_GET_RESP:
	get = (MSG_N_GET_RESP *) msg;
	get->status = MSG_K_PROCESSINGFAILURE;
	get->errorID = id;
	vsprintf(get->errorComment,
		 controlStr, ap);
	get->conditionalFields |= MSG_K_N_GETRESP_ERRORCOMMENT |
	    MSG_K_N_GETRESP_ERRORID;
	break;
    case MSG_K_N_SET_RESP:
	set = (MSG_N_SET_RESP *) msg;
	set->status = MSG_K_PROCESSINGFAILURE;
	set->errorID = id;
	vsprintf(set->errorComment,
		 controlStr, ap);
	set->conditionalFields |= MSG_K_N_SETRESP_ERRORCOMMENT |
	    MSG_K_N_SETRESP_ERRORID;
	break;
    case MSG_K_N_ACTION_RESP:
	action = (MSG_N_ACTION_RESP *) msg;
	action->status = MSG_K_PROCESSINGFAILURE;
	action->errorID = id;
	vsprintf(action->errorComment,
		 controlStr, ap);
	action->conditionalFields |= MSG_K_N_ACTIONRESP_ERRORCOMMENT |
	    MSG_K_N_ACTIONRESP_ERRORID;
	break;
    case MSG_K_N_CREATE_RESP:
	create = (MSG_N_CREATE_RESP *) msg;
	create->status = MSG_K_PROCESSINGFAILURE;
	create->errorID = id;
	vsprintf(create->errorComment,
		 controlStr, ap);
	create->conditionalFields |= MSG_K_N_CREATERESP_ERRORCOMMENT |
	    MSG_K_N_CREATERESP_ERRORID;
	break;
    case MSG_K_N_DELETE_RESP:
	delete = (MSG_N_DELETE_RESP *) msg;
	delete->status = MSG_K_PROCESSINGFAILURE;
	delete->errorID = id;
	vsprintf(delete->errorComment,
		 controlStr, ap);
	delete->conditionalFields |= MSG_K_N_DELETERESP_ERRORCOMMENT |
	    MSG_K_N_DELETERESP_ERRORID;
	break;
    default:
	fprintf(stderr, "Unknown message type\n");
    }
    va_end(ap);
}

/* updateAttributeIdentifierList
**
** Purpose:
**	Update the attribute tag list with the newly supplied DCM_TAG value and
**	increment the count by 1
**
** Parameter Dictionary:
**	tag	The Tag value to be inserted in the list
**	response
**		Address of the pointer to the response message
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
void
updateAttributeIdentifierList(DCM_TAG tag, void **response)
{
    MSG_N_GET_RESP
    * get;
    MSG_N_SET_RESP
	* set;
    MSG_N_CREATE_RESP
	* create;
    MSG_GENERAL
	* msg = (MSG_GENERAL *) (*response);
    int
        newsize;

    switch (msg->type) {
    case MSG_K_N_GET_RESP:
	get = (MSG_N_GET_RESP *) msg;
	if (get->attributeCount == 0) {
	    get->attributeIdentifierList = (DCM_TAG *) malloc(sizeof(DCM_TAG));
	    if (get->attributeIdentifierList != NULL) {
		get->conditionalFields =
		    MSG_K_N_GETRESP_ATTRIBUTEIDENTIFIERLIST;
		*get->attributeIdentifierList = tag;
		get->attributeCount = 1;
	    }
	} else {
	    newsize = sizeof(DCM_TAG) * (get->attributeCount + 1);
	    get->attributeIdentifierList =
		(DCM_TAG *) realloc(get->attributeIdentifierList, newsize);
	    get->attributeCount++;
	}
	break;
    case MSG_K_N_SET_RESP:
	set = (MSG_N_SET_RESP *) msg;
	if (set->attributeCount == 0) {
	    set->attributeIdentifierList = (DCM_TAG *) malloc(sizeof(DCM_TAG));
	    if (set->attributeIdentifierList != NULL) {
		set->conditionalFields =
		    MSG_K_N_GETRESP_ATTRIBUTEIDENTIFIERLIST;
		*set->attributeIdentifierList = tag;
		set->attributeCount = 1;
	    }
	} else {
	    newsize = sizeof(DCM_TAG) * (set->attributeCount + 1);
	    set->attributeIdentifierList =
		(DCM_TAG *) realloc(set->attributeIdentifierList, newsize);
	    set->attributeCount++;
	}
	break;
    case MSG_K_N_CREATE_RESP:
	create = (MSG_N_CREATE_RESP *) msg;
	if (create->attributeCount == 0) {
	    create->attributeIdentifierList =
		(DCM_TAG *) malloc(sizeof(DCM_TAG));
	    if (create->attributeIdentifierList != NULL) {
		create->conditionalFields =
		    MSG_K_N_GETRESP_ATTRIBUTEIDENTIFIERLIST;
		*create->attributeIdentifierList = tag;
		create->attributeCount = 1;
	    }
	} else {
	    newsize = sizeof(DCM_TAG) * (create->attributeCount + 1);
	    create->attributeIdentifierList =
		(DCM_TAG *) realloc(create->attributeIdentifierList, newsize);
	    create->attributeIdentifierList[create->attributeCount] = tag;
	    create->attributeCount++;
	}
	break;
    }
}
