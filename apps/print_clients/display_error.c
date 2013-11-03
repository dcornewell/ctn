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
** Module Name(s):	OkCB
**			displayError
** Author, Date:	Tze Khiong Lee, SEPTEMBER 14th 1993
** Intent:		To create a set of routines that will display
**			error prompts in a window.
**
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:47:04 $
** Source File:		$RCSfile: display_error.c,v $
** Revision:		$Revision: 1.10 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.10 $ $RCSfile: display_error.c,v $";
#ifdef ATHENA

#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <string.h>
#endif

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Form.h>
#include "Buttons.h"
#include "dicom.h"
#include "dicom_uids.h"
#include "lst.h"
#include "condition.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"

extern Widget
    toplevel;

typedef struct {
    void
       *reserved[2];
        Widget
        widget;
    char
       *error;
}   ERROR_ITEM;

static LST_HEAD
*   prompts = NULL;


/* OkCB
**
** Purpose:
**	This is the function that is called when the "OK" button
**	is pressed in a Error Window.
**
** Parameter Dictionary:
**	w
**	client_data
**	call_data
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static void
OkCB(Widget w, XtPointer client_data, XtPointer call_data)
{
    Widget
    shell;
    ERROR_ITEM
	* item;

    shell = (Widget) client_data;
    XtPopdown(shell);
    XtDestroyWidget(shell);
/*
	item = LST_Position(&prompts, LST_Head(&prompts));
	if(item == NULL) {
		printf("Error: LST_Position failure OkCB\n");
		return;
	}
	while(item != NULL && w != item->widget)
		item = LST_Next(&prompts);
	if(item == NULL) {
		printf("Error: LST_Next failure OkCB\n");
		return;
	}
	item = LST_Remove(&prompts, LST_K_BEFORE);
	if(item == NULL) {
		printf("Error: LST_Remove failure OkCB\n");
		return;
	}
	free(item);
*/
}


/* displayError
**
** Purpose:
**	This function creates a window with an error prompt.
**
** Parameter Dictionary:
**	error 	Error string.
**
** Return Values:
**	TRUE	Success.
**	FALSE	Failure.
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
Boolean
displayError(char *error)
{
    Widget
	shell,
	form,
	label,
	oldlabel,
	button;

    int
        len,
        i,
        start;

    Screen
	* screen;
    int
        screen_height,
        screen_width;

/*
	ERROR_ITEM
		*item;
	
   	if(prompts == NULL) {
		prompts = LST_Create();
		if(prompts == NULL){
			printf("Error: LST_Create failed in displayError\n");
			return(FALSE);
		}
	} else {
		item = LST_Position(&prompts, LST_Head(&prompts));
		if(item == NULL) {
			printf("Error: LST_Position failed in displayError\n");
			return(FALSE);
		}
		while(item != NULL && strcmp(error, item->error))
			item = LST_Next(&prompts);
		if(item != NULL)
			return;
	}
*/
    screen = XtScreen(toplevel);
    screen_width = XWidthOfScreen(screen);
    screen_height = XHeightOfScreen(screen);
    shell = XtVaCreatePopupShell("Error!", transientShellWidgetClass,
		  toplevel, XtNx, screen_width / 3, XtNy, screen_height / 3,
				 NULL);
    form = XtVaCreateManagedWidget("error_form", formWidgetClass,
				   shell, XtNborderWidth, 0, NULL);
    oldlabel = NULL;
    len = strlen(error);
    for (i = 0, start = 0; i < len + 1; i++) {
	if (error[i] == '\n' || error[i] == '\0') {
	    error[i] = '\0';
	    label = XtVaCreateManagedWidget("error_label",
					    labelWidgetClass, form, XtNlabel,
					 &(error[start]), XtNborderWidth, 0,
					    XtNfromVert, oldlabel, NULL);
	    oldlabel = label;
	    start = i + 1;
	}
    }
    button = CreateButtonTop(form, label, "Ok", OkCB, (XtPointer) shell);
    XtPopup(shell, XtGrabNone);
/*
	item = (ERROR_ITEM*) malloc(sizeof(ERROR_ITEM));
	item->error = strdup(error);
	item->widget = button;
	if(item == NULL || item->error == NULL) {
		printf("Error: Malloc failed in displayError\n");
		return(FALSE);
	}
	if(LST_Enqueue(&prompts, item) != LST_NORMAL) {
		printf("Error: LST_Enqueue in displayError\n");
		return(FALSE);
	}
*/
    return (TRUE);
}

/* displayErrorStatus
**
** Purpose:
**	Display the error status that has been returned in the response
**	message. After pressing the OK button, the application returns
**	to the toplevel menu
**
** Parameter Dictionary:
**	name		A character string indicating the SOP class
**	op		The service that was carried out
**	status		The status code returned in the response message
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
typedef struct {
    unsigned short status;
    char *description;
}   APP_STATUS_DESCRIPTION;

static APP_STATUS_DESCRIPTION statDesc[] = {
    {MSG_K_SUCCESS, "APP Success"},
    {MSG_K_CANCEL, "APP Operation cancelled"},
    {MSG_K_ATTRIBUTELISTERRORR, "APP Attribute List Error"},
    {MSG_K_CLASSINSTANCECONFLICT, "APP Class Instance Conflict"},
    {MSG_K_DUPLICATESOPINSTANCE, "APP Duplicate SOP Instance"},
    {MSG_K_DUPLICATEINVOCATION, "APP Duplicate Invocation"},
    {MSG_K_INVALIDARGUMENTVALUE, "APP Invalid Argument Value"},
    {MSG_K_INVALIDATTRIBUTEVALUE, "APP Invalid Attribute Value"},
    {MSG_K_INVALIDOBJECTINSTANCE, "APP Invalied Object Instance"},
    {MSG_K_MISSINGATTRIBUTE, "APP Missing Attribute"},
    {MSG_K_MISSINGATTRIBUTEVALUE, "APP Missing Attribute Value"},
    {MSG_K_MISTYPEDARGUMENT, "APP Mistyped Argument"},
    {MSG_K_NOSUCHARGUMENT, "APP No Such Argument"},
    {MSG_K_NOSUCHATTRIBUTE, "APP No Such Attribute"},
    {MSG_K_NOSUCHEVENTTYPE, "APP No Such Event Type"},
    {MSG_K_NOSUCHOBJECTINSTANCE, "APP No Such Object Instance"},
    {MSG_K_NOSUCHSOPCLASS, "APP No Such SOP Class"},
    {MSG_K_PROCESSINGFAILURE, "APP Processing Failure"},
    {MSG_K_RESOURCELIMITATION, "APP Resource Limitation"},
    {MSG_K_UNRECOGNIZEDOPERATION, "APP Unrecognized Operation"},
    {MSG_K_BFS_MEMORYALLOCATIONUNSUPPORTED, "APP BFS Memory Alloc Unsupported"},
    {MSG_K_BFS_FILMACCEPTEDFORPRINTING, "APP BFS Film Accepted for Printing"},
    {MSG_K_BFS_FILMCOLLATIONUNSUPPORTED, "APP BFS Film Collation Unsupported"},
    {MSG_K_BFS_NOIMAGEBOXSOPINSTANCES, "APP BFS No Image Box SOP Instances"},
    {MSG_K_BFS_NOFILMBOXSOPINSTANCES, "APP BFS No Film Box SOP Instances"},
    {MSG_K_BFS_UNABLETOCREATEPRINTJOBSOPINSTANCE,
    "APP BFS Unable to create PRINT JOB Instance"},
    {MSG_K_BFS_IMAGEPOSITIONCOLLISION, "APP BFS Image Position Collision"},
    {MSG_K_BFS_IMAGESIZELARGERTHANIMAGEBOXSIZE, "APP BFS Image Size Too Large"},
    {MSG_K_BFB_FILMACCEPTEDFORPRINTING, "APP BFB Film Accepted for Printing"},
    {MSG_K_BFB_NOIMAGEBOXSOPINSTANCES, "APP BFB No Image Box SOP Instances"},
    {MSG_K_BFB_UNABLETOCREATEPRINTJOBSOPINSTANCE,
    "APP BFB Unable to create PRINT JOB Instance"},
    {MSG_K_BFB_IMAGEPOSITIONCOLLISION, "APP BFB Image Position Collision"},
    {MSG_K_BFB_IMAGESIZELARGERTHANIMAGEBOXSIZE, "APP BFB Image too Large"},
    {MSG_K_BIB_INSUFFICIENTMEMORYINPRINTER, "APP BIB Printer Memory Over"},
    {MSG_K_BIB_MORETHANONEVOILUTBOXINIMAGE,
    "APP BIB More than one VOILUT Box in Image"}
};

void
displayErrorStatus(char *name, char *op, unsigned short status)
{
    char
        displayStr[100];
    int
        idx;

    for (idx = 0; idx < DIM_OF(statDesc); idx++) {
	if (status == statDesc[idx].status) {
	    strcpy(displayStr, name);
	    strcat(displayStr, " : ");
	    strcat(displayStr, op);
	    strcat(displayStr, " - ");
	    strcat(displayStr, statDesc[idx].description);
	    displayError(displayStr);
	    return;
	}
    }
    displayError("UNKNOWN ERROR");
    return;
}
#endif
