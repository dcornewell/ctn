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
** Module Name(s):	CreateButtonTop
**			CreateButtonLeft
**			SetButtonSensitive
**			CreateButtonList
**			CreateLabel
**			CreateSimpleList
**			SimpleListGetBox
** Author, Date:	Tze Khiong Lee, June 9th 1993
** Intent:		This modules assists applications in creating
**			buttons in Form Widgets.  Each button
**			actually consists of a command Widget
**			surrounded by a box Widget.  Another function
**			let's applications make a button sensitive
**			or insensitive.
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:37:48 $
** Source File:		$RCSfile: Buttons.c,v $
** Revision:		$Revision: 1.7 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.7 $ $RCSfile: Buttons.c,v $";
#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <stdlib.h>
#endif

#ifdef ATHENA
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Viewport.h>
#include <X11/Xaw/List.h>
#include "Buttons.h"
#define	isNULL(x)	(x==NULL)
#define notNULL(x)	(x!=NULL)

/* CreateButtonTop
**
** Purpose:
**      This function creates a button. The button will consists of
**      an outer box , an inner box with a label that displays a
**      string.  The button will call a callback function when it
**      being clicked on. Parent must be a Form Widget.
**
**
** Parameter Dictionary:
**      parent          The parent that will contain this button.
**	top		The Button that is to the top of this button.
**      label           The string that is displayed in the button.
**      callback        The function that is called when the button
**                      is clicked on.
**      client_data     The data that is passed to the function.
**
** Return Values:
**      Widget handle of a boxWidgetClass is returned if button is sucessfully
**      created, otherwise NULL is returned.
**
** Notes:
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/

Widget
CreateButtonTop(Widget parent, Widget top, String label, void (*callback) (),
		XtPointer client_data) {
    int
        i;
    Widget
	box,
	command;
    Arg args[16];

    /* Create the outer border */
    i = 0;
    XtSetArg(args[i], XtNhorizDistance, 10);
    i++;
    XtSetArg(args[i], XtNborderWidth, 2);
    i++;
    XtSetArg(args[i], XtNfromVert, top);
    i++;
    XtSetArg(args[i], XtNvertDistance, 10);
    i++;
    box = XtCreateManagedWidget("button_box", formWidgetClass,
				parent, args, i);

    if (box == NULL)
	return (NULL);

    /* Create the inner box with display */
    i = 0;
    XtSetArg(args[i], XtNlabel, label);
    i++;
    XtSetArg(args[i], XtNresizable, TRUE);
    i++;
    command = XtCreateManagedWidget("button_command", commandWidgetClass,
				    box, args, i);

    if (command == NULL)
	return (NULL);

    /* Add the call back function to the button */
    XtAddCallback(command, XtNcallback, callback, client_data);
    return (command);
}
/* CreateButtonTop
**
** Purpose:
**      This function creates a button. The button will consists of
**      an outer box , an inner box with a label that displays a
**      string.  The button will call a callback function when it
**      being clicked on. Parent must be a Form Widget.
**
**
** Parameter Dictionary:
**      parent          The parent that will contain this button.
**	left		The Button that is to the left of it.
**      label           The string that is displayed in the button.
**      callback        The function that is called when the button
**                      is clicked on.
**      client_data     The data that is passed to the function.
**
** Return Values:
**      Widget handle of a boxWidgetClass is returned if button is sucessfully
**      created, otherwise NULL is returned.
**
** Notes:
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/

Widget
CreateButtonLeft(Widget parent, Widget left, String label, void (*callback) (),
		 XtPointer client_data) {
    int
        i;
    Widget
	box,
	command;
    Arg args[16];

    /* Create the outer border */
    i = 0;
    XtSetArg(args[i], XtNhorizDistance, 40);
    i++;
    XtSetArg(args[i], XtNborderWidth, 2);
    i++;
    XtSetArg(args[i], XtNfromHoriz, left);
    i++;
    XtSetArg(args[i], XtNvertDistance, 10);
    i++;
    box = XtCreateManagedWidget("button_box", formWidgetClass,
				parent, args, i);

    if (box == NULL)
	return (NULL);

    /* Create the inner box with display */
    i = 0;
    XtSetArg(args[i], XtNlabel, label);
    i++;
    XtSetArg(args[i], XtNresizable, FALSE);
    i++;
    command = XtCreateManagedWidget("button_command", commandWidgetClass,
				    box, args, i);

    if (command == NULL)
	return (NULL);

    /* Add the call back function to the button */
    if (callback)
	XtAddCallback(command, XtNcallback, callback, client_data);
    return (command);
}


/* SetButtonSensitive
**
** Purpose:
**	This procedure sets the corresponding buttons sensitive resource
**	to that of the value.  This allows other applications to
**	make any buttons sensitive or insensitive to inputs.
**
** Parameter Dictionary:
**	w	The handle to the button.
**	value	The value that is to be set as.
**
** Return Values:
**	None.
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
void
SetButtonSensitive(Widget w, Boolean value)
{
    int
        i;
    Arg args[16];

    i = 0;
    XtSetArg(args[i], XtNsensitive, value);
    i++;
    XtSetValues(w, args, i);
}


/* CreateButtonList
**
** Purpose:
**	Creates a column of buttons, the appearance of the buttons
**	and the number of buttons is determined by the list of ButtonList
**	passed to it.
**
** Parameter Dictionary:
**	parent		Handle to parent widget
**	list		List of buttons
**	size		Size of list
**	header
**	formargs
**	formi
**
** Return Values:
**	Handle to a widget.
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
Widget
CreateButtonList(Widget parent, ButtonList list[], int size, String header,
		 Arg formargs[], int formi)
{
    int
        i,
        j;
    Widget
	form,
	label;
    Arg args[16];

    /* Create a box to contain the buttons */
    form = XtCreateManagedWidget("buttonlistform", formWidgetClass,
				 parent, formargs, formi);
    i = 0;
    XtSetArg(args[i], XtNborderWidth, 0);
    i++;
    XtSetArg(args[i], XtNdefaultDistance, 4);
    i++;
    XtSetValues(form, args, i);

    if (form == NULL)
	return (NULL);		/* exit if failed to create the box */

    /* Create a label ontop of the buttons */
    i = 0;
    XtSetArg(args[i], XtNlabel, header);
    i++;
    XtSetArg(args[i], XtNborderWidth, 0);
    i++;
    XtSetArg(args[i], XtNresizable, FALSE);
    i++;
    label = XtCreateManagedWidget("buttonlistlabel", labelWidgetClass,
				  form, args, i);

    if (label == NULL)
	return (NULL);

    /* Create the first button */
    if ((list[0].button = CreateButtonTop(form, label, list[0].label,
			    list[0].callback, list[0].client_data)) == NULL)
	return (NULL);

    /* Create the rest of the other buttons */
    i = 0;
    XtSetArg(args[i], XtNsensitive, list[0].sensitive);
    i++;
    XtSetValues(list[0].button, args, i);

    for (j = 1; j < size; j++) {
	list[j].button = CreateButtonTop(form, XtParent(list[j - 1].button),
		      list[j].label, list[j].callback, list[j].client_data);

	if (list[j].button == NULL)
	    return (NULL);

	SetButtonSensitive(list[j].button, list[j].sensitive);
    }

    return (form);
}


/* CreateLabel
**
** Purpose:
**	This function creates a label.
**
** Parameter Dictionary:
**	parent		Parent widget
**	top
**	left
**	string
**	vdist
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
CreateLabel(Widget parent, Widget top, Widget left, char *string, int vdist)
{
    Widget
	label;
    int
        i;
    Arg
	args[16];

    i = 0;
    XtSetArg(args[i], XtNfromVert, top);
    i++;
    XtSetArg(args[i], XtNvertDistance, vdist);
    i++;
    XtSetArg(args[i], XtNfromHoriz, left);
    i++;
    XtSetArg(args[i], XtNresizable, FALSE);
    i++;
    XtSetArg(args[i], XtNborderWidth, 0);
    i++;
    XtSetArg(args[i], XtNlabel, string);
    i++;
    if ((label = XtCreateManagedWidget("visitpatientlisterlabel",
				labelWidgetClass, parent, args, i)) == NULL)
	printf("error creating visit-patient lister\n");
    return (label);
}


/* CreateSimpleList
**
** Purpose:
**	This function creates a list.
**
** Parameter Dictionary:
**	parent
**	top
**	left
**	xsize
**	ysize
**	callback
**	client_data
**	list
**	listsize
**	allowHoriz
**
** Return Values:
**	Handle to a widget (on success)
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
Widget
CreateSimpleList(Widget parent, Widget top, Widget left, int xsize, int ysize,
    void (*callback) (), XtPointer client_data, String List[], int listsize,
		 Boolean allowHoriz)
{
    int
        i;
    Arg
	args[16];
    Widget
	view,
	viewbox,
	list;

    i = 0;
    XtSetArg(args[i], XtNfromVert, top);
    i++;
    XtSetArg(args[i], XtNfromHoriz, left);
    i++;
    if ((viewbox = XtCreateManagedWidget(
					 "simplelistviewbox",
			       formWidgetClass, parent, args, i)) == NULL) {
	printf("error creating simple list\n");
	return (NULL);
    }
    /* create the scrollers for the list */
    i = 0;
    XtSetArg(args[i], XtNallowVert, TRUE);
    i++;
    XtSetArg(args[i], XtNallowHoriz, allowHoriz);
    i++;
    XtSetArg(args[i], XtNheight, ysize);
    i++;
    XtSetArg(args[i], XtNwidth, xsize);
    i++;
    XtSetArg(args[i], XtNuseBottom, TRUE);
    i++;
    XtSetArg(args[i], XtNuseRight, TRUE);
    i++;
    if ((view = XtCreateManagedWidget("simplelistviewport",
			  viewportWidgetClass, viewbox, args, i)) == NULL) {
	printf("error creating simple lister\n");
	return (NULL);
    }
    /* create the list */
    i = 0;
    XtSetArg(args[i], XtNverticalList, TRUE);
    i++;
    XtSetArg(args[i], XtNforceColumns, TRUE);
    i++;
    XtSetArg(args[i], XtNdefaultColumns, 1);
    i++;
    if ((list = XtCreateManagedWidget("simplelistlist",
				 listWidgetClass, view, args, i)) == NULL) {
	printf("error creating simple lister\n");
	return NULL;
    }
    if (notNULL(callback))
	XtAddCallback(list, XtNcallback, callback, client_data);
    if (notNULL(List) && (listsize != 0))
	XawListChange(list, List, listsize, xsize - 30, TRUE);
    return (list);
}


/* SimpleListGetBox
**
** Purpose:
**	This function finds the box that encapsulates the list.
**
** Parameter Dictionary:
**	list
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
SimpleListGetBox(Widget list)
{
    return (XtParent(XtParent(list)));
}
#endif
get_wmShellWidgetClass()
{
    return 1;
}
get_applicationShellWidgetClass()
{
    return 1;
}
