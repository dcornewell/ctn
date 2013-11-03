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
**			displayPrompt
** Author, Date:	Tze Khiong Lee, SEPTEMBER 16th 1993
** Intent:		To create a set of routines that will display
**			prompts in a window.
**
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:48:41 $
** Source File:		$RCSfile: display_message.c,v $
** Revision:		$Revision: 1.8 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.8 $ $RCSfile: display_message.c,v $";
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
#include "condition.h"
#include "lst.h"

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
**	void
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
}


/* displayMessage
**
** Purpose:
**	This function creates a window with a message.
**
** Parameter Dictionary:
**	string 	Message string.
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
Widget
displayMessage(char *title, char *string, XtCallbackProc(*cb) (),
	       XtPointer client_data, char *btnlabel) {
    Widget
	shell,
	form,
	label = 0,
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

    screen = XtScreen(toplevel);
    screen_width = XWidthOfScreen(screen);
    screen_height = XHeightOfScreen(screen);
    shell = XtVaCreatePopupShell(title, transientShellWidgetClass,
	   toplevel, XtNx, screen_width / 3, XtNy, screen_height / 3, NULL);
    form = XtVaCreateManagedWidget("message_form", formWidgetClass,
				   shell, XtNborderWidth, 0, NULL);
    oldlabel = NULL;
    len = strlen(string);
    for (i = 0, start = 0; i < len + 1; i++) {
	if (string[i] == '\n' || string[i] == '\0') {
	    string[i] = '\0';
	    label = XtVaCreateManagedWidget("error_label",
					    labelWidgetClass, form, XtNlabel,
					&(string[start]), XtNborderWidth, 0,
					    XtNfromVert, oldlabel, NULL);
	    oldlabel = label;
	    start = i + 1;
	}
    }
    if (btnlabel == NULL)
	button = CreateButtonTop(form, label, "Ok", OkCB, (XtPointer) shell);
    else
	button = CreateButtonTop(form, label, btnlabel, OkCB, (XtPointer) shell);
/*lint -e64*/
    if (cb != NULL)
	XtAddCallback(button, XtNcallback, cb, client_data);
/*lint +e64*/
    XtPopup(shell, XtGrabNone);
    return (shell);
}


#endif
