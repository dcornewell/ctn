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
** Module Name(s):
**			PopdownMessageBox
**			CreateMessageBox
**			CreateMessageBoxPopup
**			PopupMessageBox
** Author, Date:	Tze Khiong Lee, SEPTEMBER 14th 1993
** Intent:		This will provide routines to help
**			build windows that will display text.
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:37:48 $
** Source File:		$RCSfile: MessageBox.c,v $
** Revision:		$Revision: 1.12 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.12 $ $RCSfile: MessageBox.c,v $";

#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <stdlib.h>
#endif

#ifdef ATHENA
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Command.h>
#include "Buttons.h"
#include "MessageBox.h"

#define widgetIsNULL(x)	((x) == 0)
#define widgetIsNotNULL(x)	((x) != 0)
#define isNULL(x)	(x==NULL)
#define notNULL(x)	(x!=NULL)


/* PopdownMessageBox
**
** Purpose:
**	This is callback function used to close the poped up
**	MessageBox.
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
void
PopdownMessageBox(Widget w, XtPointer client_data, XtPointer call_data)
{
    MessageBox
    * mb;

    mb = (MessageBox *) client_data;

    if (mb->shell != 0)
	XtPopdown(mb->shell);
}


/* CreateMessageBox
**
** Purpose:
**	This will create a window with a list of labels.
**
** Parameter Dictionary:
**	mb
**	parent
**	haveButtons
**
** Return Values:
**	Handle to a widget
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
Widget
CreateMessageBox(MessageBox * mb, Widget parent, Boolean haveButtons)
{
    Widget
	form,
	oldlabel,
	label,
	infoform,
	buttonform = 0,
	ok,
	cancel;
    int
        i,
        j;
    Arg
	args[16];
    void
        (*callback) ();

    i = 0;
    XtSetArg(args[i], XtNdefaultDistance, 10);
    i++;
    form = XtCreateManagedWidget("messageboxform", formWidgetClass,
				 parent, args, i);
    if (widgetIsNULL(form))
	return (0);
    i = 0;
    XtSetArg(args[i], XtNdefaultDistance, 10);
    i++;
    XtSetArg(args[i], XtNborderWidth, 0);
    i++;
    infoform = XtCreateManagedWidget("messageboxinfoform",
				     formWidgetClass, form, args, i);
    if (widgetIsNULL(infoform))
	return (0);

    oldlabel = NULL;
    for (j = 0; j < mb->numlabels; j++) {
	i = 0;
	XtSetArg(args[i], XtNlabel, mb->labels[j]);
	i++;
	XtSetArg(args[i], XtNborderWidth, 0);
	i++;
	XtSetArg(args[i], XtNfromVert, oldlabel);
	i++;
	label = XtCreateManagedWidget("messageboxtext",
				      labelWidgetClass, infoform, args, i);
	if (widgetIsNULL(label))
	    return (0);
	oldlabel = label;
    }
    mb->lastlabel = label;
    if (haveButtons == TRUE) {
	i = 0;
	XtSetArg(args[i], XtNfromVert, infoform);
	i++;
	XtSetArg(args[i], XtNvertDistance, 20);
	i++;
	XtSetArg(args[i], XtNhorizDistance, 20);
	i++;
	XtSetArg(args[i], XtNborderWidth, 0);
	i++;
	buttonform = XtCreateManagedWidget("messageboxbuttonbox",
					   formWidgetClass, form, args, i);
	if (widgetIsNULL(buttonform))
	    return (0);
	if (notNULL(mb->okcallback))
	    callback = mb->okcallback;
/*
	    XtAddCallback(ok, XtNcallback, mb->okcallback, NULL);
*/
	else
	    callback = PopdownMessageBox;
	if (notNULL(mb->okstring))
	    ok = CreateButtonTop(buttonform, NULL, mb->okstring,
				 callback, (XtPointer) mb);
	else
	    ok = CreateButtonTop(buttonform, NULL, "Ok",
				 callback, (XtPointer) mb);
	if (mb->haveCancel) {
	    cancel = CreateButtonLeft(buttonform, XtParent(ok), "Cancel",
				      PopdownMessageBox, (XtPointer) mb);
	    if (cancel == 0) {
		printf("Could not create cancel button\n");
		exit(1);
	    }
	}
    }
    mb->form = form;
    mb->workform = infoform;
    mb->buttonform = buttonform;
    return (form);
}

/* CreateMessageBoxPopup
**
** Purpose:
**	Create a Pop Up Message Box
**
** Parameter Dictionary:
**	mb
**	parent
**	haveButtons
**
** Return Values:
**	Handle to a widget
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
Widget
CreateMessageBoxPopup(MessageBox * mb, Widget parent, Boolean haveButtons)
{
    mb->shell = XtCreatePopupShell(mb->windowTitle,
				 topLevelShellWidgetClass, parent, NULL, 0);
    if (widgetIsNULL(mb->shell))
	return (0);
    if (widgetIsNULL(CreateMessageBox(mb, mb->shell, haveButtons)))
	return (0);

    return ((Widget) mb->shell);
}

/* PopupMessageBox
**
** Purpose:
**	This will popup a MessageBox window.
**
** Parameter Dictionary:
**	mb
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
PopupMessageBox(MessageBox * mb)
{
    if (widgetIsNULL((Widget) mb->shell))
	return;
    XtPopdown((Widget) mb->shell);
    XtPopup((Widget) mb->shell, XtGrabNone);
}
#endif
