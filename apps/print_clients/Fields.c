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
** Module Name(s):	UpdateFIELDwTEXT
**			UpdateFIELDwRBTN
**			UpdateAllEntries
**			ALLRBTNcb
**			CreateRBTNwLABEL
**			CreateRBTNwTEXTFIELD
**			CreateRBTNw2TEXTFIELD
**			CreateFIELDwRBTN
**			CreateFIELDwTEXT
**			CreateField
**			CreateEntryForm
**			CheckRequiredFields
**
** Author, Date:	Tze Khiong Lee, SEPTEMBER 14th, 1993
** Intent:		To provide a set of routines to help build
**			forms of text fields or enumerated fields with
**			buttons.
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:37:48 $
** Source File:		$RCSfile: Fields.c,v $
** Revision:		$Revision: 1.16 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.16 $ $RCSfile: Fields.c,v $";
#ifdef ATHENA
#include "ctn_os.h"

#if 0
#include <stdio.h>
#endif

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/List.h>
#include <X11/Xaw/Viewport.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Toggle.h>
#include "Buttons.h"
#include "Fields.h"
#define MAXENTRIES 100
#define isNULL(x)	(x == NULL)
#define notNULL(x)	(x != NULL)

static ENTRY
*   AllEntries[MAXENTRIES];

static int
    NumEntries = 0;

extern Widget toplevel;

static char text_translations_table[] =
"	<FocusIn>:	display-caret(on)\n\
	<FocusOut>:	display-caret(off)\n\
	Ctrl<Key>M: no-op(RingBell)\n\
	Ctrl<Key>J: no-op(RingBell)\n\
	Ctrl<Key>O: no-op(RingBell)\n\
        <Key>Linefeed: no-op(RingBell)\n\
        <Key>Return: no-op(RingBell)\n\
        <Key>Up: no-op(RingBell)\n\
        <Key>Down: no-op(RingBell)";

static XtTranslations text_translations;
static Boolean text_translated = FALSE;


/* UpdateFIELDwTEXT
**
** Purpose:
**	Updates the display of the text field.
**
** Parameter Dictionary:
**	ftext
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
UpdateFIELDwTEXT(FIELDwTEXT * ftext)
{
    XtVaSetValues(ftext->text, XtNstring, ftext->storage, NULL);
}


/* UpdateFIELDwRBTN
**
** Purpose:
**	To update the button states of a given field with buttons.
**
** Parameter Dictionary:
**	frbtn
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
UpdateFIELDwRBTN(FIELDwRBTN * frbtn)
{
    int
        i;
    char *data;

    Boolean
	set_one_on = FALSE;


    for (i = frbtn->numrbtns - 1; i >= 0; i--) {
	switch (frbtn->rbtns[i].type) {
	case (BTN_RBTNwTEXTFIELD):
	    data = frbtn->rbtns[i].btns.rbtf->data;
	    break;
	case (BTN_RBTNw2TEXTFIELD):
	    data = frbtn->rbtns[i].btns.rb2tf->data;
	    break;
	case (BTN_RBTNwLABEL):
	    data = frbtn->rbtns[i].btns.rblbl->data;
	    break;
	default:
	    data = "";
	    break;
	}
	if (!strncmp(data, frbtn->storage, strlen(data))) {
	    XtVaSetValues(frbtn->rbtns[i].button, XtNstate, TRUE,
			  NULL);
	    set_one_on = TRUE;
	} else
	    XtVaSetValues(frbtn->rbtns[i].button, XtNstate, FALSE,
			  NULL);
    }
    if (set_one_on == FALSE)
	XtVaSetValues(frbtn->rbtns[0].button, XtNstate, TRUE,
		      NULL);
}


/* UpdateAllEntries
**
** Purpose:
**	Updates the displays of all fields.
**
** Parameter Dictionary:
**	None
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
UpdateAllEntries()
{
    int
        i;
    String
	text;
    for (i = 0; i < NumEntries; i++)
	switch (AllEntries[i]->type) {
	case (FLD_FIELDwTEXT):
	    UpdateFIELDwTEXT(AllEntries[i]->field.ftext);
	    break;
	case (FLD_FIELDwRBTN):
	    UpdateFIELDwRBTN(AllEntries[i]->field.frbtn);
	    break;
	default:
	    break;
	}
}


/* ALLRBTNcb
**
** Purpose:
**	This function is called when any buttons in a field is pressed.
**
** Parameter Dictionary:
**	w
**	client_data
**	call_data
**
** Return Values:
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static void
ALLRBTNcb(Widget w, XtPointer client_data, XtPointer call_data)
{

    FIELDwRBTN
    * fld;
    int
        i,
        j;

    fld = (FIELDwRBTN *) client_data;
    if (call_data == NULL)
	return;
    for (i = 0; (i < fld->numrbtns) && (w != fld->rbtns[i].button); i++);
    switch (fld->rbtns[i].type) {
    case (BTN_RBTNwLABEL):
	strcpy(fld->storage, fld->rbtns[i].btns.rblbl->data);
	break;
    case (BTN_RBTNwTEXTFIELD):
	strcpy(fld->storage, fld->rbtns[i].btns.rbtf->data);
	break;
    case (BTN_RBTNw2TEXTFIELD):
	strcpy(fld->storage, fld->rbtns[i].btns.rb2tf->data);
	break;
    }
}


/* CreateRBTNwLABEL
**
** Purpose:
**	Creates a radio button with a label.
**
** Parameter Dictionary:
**	parent
**	top
**	group
**	rbl
**	client_data
**	state
**
** Return Values:
**	Handle to a widget
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static Widget
CreateRBTNwLABEL(Widget parent, Widget top, Widget group, RBTNwLABEL * rbl, XtPointer client_data, Boolean state)
{
    Widget
    form,
    box,
    rbtn,
    label;
    form = XtVaCreateManagedWidget("radio_button_form", formWidgetClass,
			 parent, XtNfromVert, top, XtNborderWidth, 0, NULL);
    box = XtVaCreateManagedWidget("radio_button_box", formWidgetClass,
				  form, XtNdefaultDistance, 2, NULL);
    rbtn = XtVaCreateManagedWidget("radio_button", toggleWidgetClass,
				 box, XtNradioGroup, group, XtNstate, state,
			   XtNlabel, "", XtNwidth, 10, XtNheight, 10, NULL);
    XtAddCallback(rbtn, XtNcallback, ALLRBTNcb, client_data);
    label = XtVaCreateManagedWidget("radio_button_label", labelWidgetClass,
			       form, XtNfromHoriz, box, XtNhorizDistance, 5,
		       XtNlabel, rbl->buttonlabel, XtNborderWidth, 0, NULL);
    if (isNULL(form) || isNULL(rbtn) || isNULL(label))
	return (NULL);

    return (rbtn);
}


/* CreateRBTNwTEXTFIELD
**
** Purpose:
**	Creates a radio button with a text field.
**
** Parameter Dictionary:
**	parent
**	top
**	group
**	rbl
**	client_data
**	state
**
** Return Values:
**	Handle to a widget
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static Widget
CreateRBTNwTEXTFIELD(Widget parent, Widget top, Widget group, RBTNwTEXTFIELD * rbl, XtPointer client_data, Boolean state)
{
    Widget
    form,
    box,
    rbtn,
    label,
    label2 = 0,
    before,
    text;
    if (text_translated == FALSE) {
	text_translations = XtParseTranslationTable(text_translations_table);
	text_translated = TRUE;
    }
    form = XtVaCreateManagedWidget("radio_button_form", formWidgetClass,
			 parent, XtNfromVert, top, XtNborderWidth, 0, NULL);
    box = XtVaCreateManagedWidget("radio_button_box", formWidgetClass,
				  form, XtNdefaultDistance, 2, NULL);
    rbtn = XtVaCreateManagedWidget("radio_button", toggleWidgetClass,
				 box, XtNradioGroup, group, XtNstate, state,
			   XtNlabel, "", XtNwidth, 10, XtNheight, 10, NULL);
    XtAddCallback(rbtn, XtNcallback, ALLRBTNcb, client_data);
    label = NULL;
    if (rbl->buttonlabel != NULL) {
	label = XtVaCreateManagedWidget("radio_button_label", labelWidgetClass,
			       form, XtNfromHoriz, box, XtNhorizDistance, 5,
		       XtNlabel, rbl->buttonlabel, XtNborderWidth, 0, NULL);
	before = label;
    } else
	before = box;
    text = XtVaCreateManagedWidget("text_field", asciiTextWidgetClass,
				   form, XtNfromHoriz, before,
				   XtNwidth, 50,
				   XtNuseStringInPlace, TRUE,
				   XtNstring, rbl->storage,
/*
				   XtNdisplayCaret, FALSE,
*/
				   XtNlength, rbl->storagesize - 1,
				   XtNeditType, XawtextEdit, NULL);
    if (rbl->suffix != NULL)
	label2 = XtVaCreateManagedWidget("radio_button_label", labelWidgetClass,
			      form, XtNfromHoriz, text, XtNhorizDistance, 5,
			    XtNlabel, rbl->suffix, XtNborderWidth, 0, NULL);
    if (isNULL(form) || isNULL(rbtn) || (isNULL(label) && rbl->buttonlabel) ||
	isNULL(text) || (isNULL(label2) && rbl->suffix))
	return (NULL);
    XtOverrideTranslations(text, text_translations);
    return (rbtn);
}


/* CreateRBTNw2TEXTFIELD
**
** Purpose:
** 	Creates a radio button with two text fields.
**
** Parameter Dictionary:
**	parent
**	top
**	group
**	rbl
**	client_data
**	state
**
** Return Values:
**	Handle to a widget
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static Widget
CreateRBTNw2TEXTFIELD(Widget parent, Widget top, Widget group, RBTNw2TEXTFIELD
		      * rbl, XtPointer client_data, Boolean state)
{
    Widget
    form,
    box,
    rbtn,
    label,
    label1,
    label2,
    text1,
    text2;

    if (text_translated == FALSE) {
	text_translations = XtParseTranslationTable(text_translations_table);
	text_translated = TRUE;
    }
    form = XtVaCreateManagedWidget("radio_button_form", formWidgetClass,
			 parent, XtNfromVert, top, XtNborderWidth, 0, NULL);
    box = XtVaCreateManagedWidget("radio_button_box", formWidgetClass,
				  form, XtNdefaultDistance, 2, NULL);
    rbtn = XtVaCreateManagedWidget("radio_button", toggleWidgetClass,
				 box, XtNradioGroup, group, XtNstate, state,
			   XtNlabel, "", XtNwidth, 10, XtNheight, 10, NULL);
    XtAddCallback(rbtn, XtNcallback, ALLRBTNcb, client_data);
    label = XtVaCreateManagedWidget("radio_button_label", labelWidgetClass,
			       form, XtNfromHoriz, box, XtNhorizDistance, 5,
		       XtNlabel, rbl->buttonlabel, XtNborderWidth, 0, NULL);
    label1 = XtVaCreateManagedWidget("radio_button_label", labelWidgetClass,
			    form, XtNfromHoriz, label, XtNhorizDistance, 10,
			    XtNlabel, rbl->label1, XtNborderWidth, 0, NULL);
    text1 = XtVaCreateManagedWidget("text_field", asciiTextWidgetClass,
				    form, XtNfromHoriz, label1,
				    XtNwidth, 50,
				    XtNuseStringInPlace, TRUE,
				    XtNstring, rbl->storage1,
/*
				    XtNdisplayCaret, FALSE,
*/
				    XtNlength, rbl->storagesize1 - 1,
				    XtNeditType, XawtextEdit, NULL);
    label2 = XtVaCreateManagedWidget("radio_button_label", labelWidgetClass,
			    form, XtNfromHoriz, text1, XtNhorizDistance, 10,
			    XtNlabel, rbl->label2, XtNborderWidth, 0, NULL);
    text2 = XtVaCreateManagedWidget("text_field", asciiTextWidgetClass,
				    form, XtNfromHoriz, label2,
				    XtNwidth, 50,
				    XtNuseStringInPlace, TRUE,
				    XtNstring, rbl->storage2,
/*
				    XtNdisplayCaret, FALSE,
*/
				    XtNlength, rbl->storagesize2 - 1,
				    XtNeditType, XawtextEdit, NULL);
    if (isNULL(form) || isNULL(rbtn) || isNULL(label) || isNULL(text1)
	|| isNULL(text2) || isNULL(label1) || isNULL(label2))
	return (NULL);
    XtOverrideTranslations(text1, text_translations);
    XtOverrideTranslations(text2, text_translations);
    return (rbtn);
}


/* CreateFIELDwRBTN
**
** Purpose:
**	Create an entry field with buttons.
**
** Parameter Dictionary:
**	parent
**	top
**	fld
**
** Return Values:
**	Handle to a widget
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static Widget
CreateFIELDwRBTN(Widget parent, Widget top, FIELDwRBTN * fld)
{
    Widget
    form,
    label,
    rbtn2,
    previous;
    int
        i;
    Boolean
	state;

    static char toggle_translations_table[] =
    "<EnterWindow>:         highlight(Always)\n\
        <LeaveWindow>:          unhighlight()\n\
        <Btn1Down>,<Btn1Up>:    set() notify()";

    static XtTranslations
        toggle_translations;

    static Boolean
        toggle_translated = FALSE;

    if (toggle_translated == FALSE)
	toggle_translations = XtParseTranslationTable(toggle_translations_table);
    form = XtVaCreateManagedWidget("field_rbtn_form", formWidgetClass,
				parent, XtNfromVert, top, XtNborderWidth, 0,
				   XtNdefaultDistance, 2, NULL);
    label = XtVaCreateManagedWidget("field_rbtn_label", labelWidgetClass,
		  form, XtNlabel, fld->fieldlabel, XtNborderWidth, 0, NULL);
    rbtn2 = NULL;
    previous = label;
    for (i = 0; i < fld->numrbtns; i++) {
	if (i == 0) {
	    state = TRUE;
	    strcpy(fld->storage, fld->rbtns[i].btns.rblbl->data);
	} else
	    state = FALSE;
	switch (fld->rbtns[i].type) {
	case (BTN_RBTNwLABEL):
	    if (isNULL((fld->rbtns[i].button = CreateRBTNwLABEL(form, previous,
		 rbtn2, fld->rbtns[i].btns.rblbl, (XtPointer) fld, state))))
		return (NULL);
	    rbtn2 = fld->rbtns[i].button;
	    previous = GetRbtnForm(fld->rbtns[i].button);
	    break;
	case (BTN_RBTNwTEXTFIELD):
	    if (isNULL((fld->rbtns[i].button = CreateRBTNwTEXTFIELD(form,
		  previous, rbtn2, fld->rbtns[i].btns.rbtf, (XtPointer) fld,
								    state))))
		return (NULL);
	    rbtn2 = fld->rbtns[i].button;
	    previous = GetRbtnForm(fld->rbtns[i].button);
	    break;
	case (BTN_RBTNw2TEXTFIELD):
	    if (isNULL((fld->rbtns[i].button = CreateRBTNw2TEXTFIELD(form,
		 previous, rbtn2, fld->rbtns[i].btns.rb2tf, (XtPointer) fld,
								   state))))
		return (NULL);
	    rbtn2 = fld->rbtns[i].button;
	    previous = GetRbtnForm(fld->rbtns[i].button);
	    break;
	default:
	    break;
	}
	XtOverrideTranslations(fld->rbtns[i].button, toggle_translations);
    }
    return (form);
}


/* CreateFIELDwTEXT
**
** Purpose:
**	Creates a text entry field.
**
** Parameter Dictionary:
**	parent
**	top
**	fld
**
** Return Values:
**	Handle to a widget
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static Widget
CreateFIELDwTEXT(Widget parent, Widget top, FIELDwTEXT * fld)
{
    Widget
    form,
    label,
    text;

    if (text_translated == FALSE) {
	text_translations = XtParseTranslationTable(text_translations_table);
	text_translated = TRUE;
    }
    form = XtVaCreateManagedWidget("field_text_form", formWidgetClass,
			 parent, XtNfromVert, top, XtNborderWidth, 0, NULL);
    label = CreateLabel(form, NULL, NULL, (char *) fld->fieldlabel, 2);
    if (fld->editable == TRUE)
	text = XtVaCreateManagedWidget("text_field", asciiTextWidgetClass,
				       form, XtNfromVert, label,
				       XtNwidth, 300,
				       XtNuseStringInPlace, TRUE,
				       XtNstring, fld->storage,
				       XtNeditType, XawtextEdit,
/*
				       XtNdisplayCaret, FALSE,
*/
				     XtNlength, fld->storagesize - 1, NULL);
    else
	text = XtVaCreateManagedWidget("text_field", asciiTextWidgetClass,
				       form, XtNfromVert, label,
				       XtNwidth, 300,
				       XtNuseStringInPlace, TRUE,
				       XtNstring, fld->storage,
/*
				       XtNdisplayCaret, FALSE,
*/
				     XtNlength, fld->storagesize - 1, NULL);
    if (isNULL(form) || isNULL(label) || isNULL(text))
	return (NULL);
    fld->text = text;
    XtOverrideTranslations(fld->text, text_translations);
    return (form);
}


/* CreateField
**
** Purpose:
**	Creates an entry field.
**
** Parameter Dictionary:
**	parent
**	top
**	entry
**
** Return Values:
**	Handle to a widget
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static Widget
CreateField(Widget parent, Widget top, ENTRY * entry)
{
    AllEntries[NumEntries++] = entry;
    switch (entry->type) {
    case (FLD_FIELDwRBTN):
	return (CreateFIELDwRBTN(parent, top, entry->field.frbtn));
    case (FLD_FIELDwTEXT):
	return (CreateFIELDwTEXT(parent, top, entry->field.ftext));
    }
    return (NULL);
}


/* CreateEntryForm
**
** Purpose:
**	Creates an entry form.
**
** Parameter Dictionary:
**	parent
**	top
**	left
**	entry
**	num
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
CreateEntryForm(Widget parent, Widget top, Widget left, ENTRY entry[], int num)
{
    int
        i;
    Widget
	form,
	field,
	last;

    form = XtVaCreateManagedWidget("entry_form", formWidgetClass,
			       parent, XtNfromVert, top, XtNfromHoriz, left,
			    XtNdefaultDistance, 1, XtNborderWidth, 0, NULL);
    last = NULL;
    for (i = 0; i < num; i++) {
	field = CreateField(form, last, &(entry[i]));
	if (isNULL(field))
	    return (NULL);
	last = field;
    }
    return (form);
}

/* CheckRequiredFields
**
** Purpose:
**	Check if required fields appear in the entry.
**
** Parameter Dictionary:
**	entry
**	num
**
** Return Values:
**	TRUE
**	FALSE
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
Boolean
CheckRequiredFields(ENTRY entry[], int num)
{
    int
        i;
    int
        count = 0;
    Boolean
	flag = TRUE;
    static Widget
        shell = NULL;
    static Widget
        label,
        form,
        list;
    String
	missinglist[30];

    for (i = 0; i < num; i++) {
	switch (entry[i].type) {
	case (FLD_FIELDwRBTN):
	    if (entry[i].require == REQUIRED &&
	      !XawToggleGetCurrent(entry[i].field.frbtn->rbtns[i].button)) {
		flag = FALSE;
		missinglist[count++] = entry[i].field.frbtn->fieldlabel;
	    }
	}
    }
    if (flag == FALSE) {
	if (isNULL(shell)) {
	    shell = XtVaCreatePopupShell("Error", transientShellWidgetClass,
					 toplevel, NULL);
	    form = XtVaCreateManagedWidget("error_form", formWidgetClass,
					   shell, NULL);
	    label = CreateLabel(form, NULL, NULL, "Missing Required Fields:",
				10);
	    list = CreateSimpleList(form, label, NULL, 300, 200, NULL, NULL,
				    missinglist, 0, FALSE);
	    if (isNULL(shell) || isNULL(form) || isNULL(label) || isNULL(list)) {
		printf("Error: Creation Failure in CheckRequiredFeilds\n");
		return (FALSE);
	    }
	    XtPopup(shell, XtGrabNone);
	    XawListChange(list, missinglist, count, 170, TRUE);
	}
    } else if (notNULL(shell))
	XtPopdown(shell);
    return (flag);
}
#endif
