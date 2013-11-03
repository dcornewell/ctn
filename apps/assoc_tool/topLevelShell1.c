
/*******************************************************************************
	topLevelShell1.c

       Associated Header file: topLevelShell1.h
*******************************************************************************/

#include "ctn_os.h"
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/Text.h>
#include <Xm/ToggleBG.h>
#include <Xm/TextF.h>
#include <Xm/LabelG.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/BulletinB.h>
#include <Xm/CascadeB.h>
#include <Xm/PushB.h>
#include <Xm/SeparatoG.h>
#include <Xm/RowColumn.h>
#include <Xm/MainW.h>
#include <X11/Shell.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

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
**                     Electronic Radiology Laboratory
**                   Mallinckrodt Institute of Radiology
**                Washington University School  of Medicine
**
** Module Name(s):		main
**			activateCB_menu1_p1_b2
**			browseSelectionCB_scrolledList1
**			browseSelectionCB_scrolledList2
**			activateCB_pushButton1
**			activateCB_pushButton2
**			activateCB_pushButton3
**			activateCB_pushButton4
**			network
**			copyWtext
**			copyWtext2
**			load
**			load_sopClasses
**			addsop_scrolledList2
**			selected_sop_fromList1
**			update
**			set_options
**			set_options_toDefault
**
** Author:			Chander L. Sabharwal,  9-September-94
**
** Intent:			This program lets the user establish a  network
**			 connection with a server and make an Association
**			with it for the SOP service classes. It allows the
**			user to select options as Service Class User SCU,
**			SCP or SCU/SCP. It also lets the user select options
**			for transfer syntaxes as LITTLEENDIAN, LITTLEENDIAN
**			EXPLICIT and BIGENDIAN EXPLICIT.
**
** Last Update:		$Author: smm $, $Date: 2002-10-31 23:17:53 $
**
** Source File:		$Source: /home/smm/ctn/ctn/apps/assoc_tool/topLevelShell1.c,v $
**
** Revision:		$Revision: 1.6 $
**
** Status:			$State: Exp $
**
*/
static char rcsid[] = "$Revisin$ $RCSfile: topLevelShell1.c,v $";

#include "dicom.h"
#include "dicom_uids.h"
#include "lst.h"
#include "condition.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "dicom_ie.h"
#include "mut.h"

#include "structures.h"
#include "prototypes.h"


#define	MAXLEN	5000

/* transfer syntaxes initialized to default */
char tSyntaxes[3][DUL_LEN_UID + 1] = {
    DICOM_TRANSFERLITTLEENDIAN,
    "",
    "",
};

DUL_SC_ROLE			/* identifying the application as requestor,
				 * acceptor or both, initialized to default */
role = DUL_SC_ROLE_DEFAULT;

/* structure for linked list sopclassList  */
typedef struct {
    void *reserved[2];
    char sopclass[65];
    char sopclassW[65];
}   SOP_ELEMENT;

LST_HEAD *sopList = NULL;

LST_HEAD *selectedsopList = NULL;

SELECTED_SOP *se = NULL;

#include "format.h"

static int itemNo_selected1;	/* item number selected fromlist1 */
static int itemNo_selected2;	/* item number selected fromlist2 */

static char *sopstr2;		/* character string containing sop classname
				 * from list2 */

static char *text_typed;	/* text typed in the textfield */

static char node[40];		/* the node we are calling */

static char calledAPTitle[40];	/* called application Title */

static char callingAPTitle[40];	/* calling application title */

static char port[40];		/* TCP port to establish Association */

static char buf_mes[65];


static Widget topLevelShell1;
static Widget mainWindow1;
static Widget menu1;
static Widget menu1_p1;
static Widget menu1_p1_b1;
static Widget menu1_p1_b2;
static Widget menu1_top_b1;
static Widget bulletinBoard1;
static Widget scrolledWindowList1;
static Widget scrolledList1;
static Widget scrolledWindowList2;
static Widget scrolledWindowList4;
static Widget scrolledList2;
static Widget separatorGadget1;
static Widget labelGadget1;
static Widget textField1;
static Widget rowColumn1;
static Widget toggleButtonGadget1;
static Widget toggleButtonGadget2;
static Widget toggleButtonGadget3;
static Widget toggleButtonGadget4;
static Widget toggleButtonGadget5;
static Widget toggleButtonGadget6;
static Widget toggleButtonGadget7;
static Widget separatorGadget2;
static Widget scrolledWindowText1;
static Widget scrolledText1;
static Widget pushButton1;
static Widget pushButton2;
static Widget labelGadget2;
static Widget labelGadget3;
static Widget labelGadget4;
static Widget labelGadget5;
static Widget labelGadget6;
static Widget separatorGadget3;
static Widget textField2;
static Widget textField3;
static Widget textField4;
static Widget textField5;
static Widget pushButton3;
static Widget pushButton4;
static Widget textField6;
static swidget UxParent;

#define CONTEXT_MACRO_ACCESS 1
#include "topLevelShell1.h"
#undef CONTEXT_MACRO_ACCESS


/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

/* load
**
** Purpose:
**	This function loads the list of SOP class in scrolled List1
**
** Parameter Dictionary:
**	sopList	list containing the SOP class names
**
** Return Values:
**	None
**
** Notes:
**
**Algorithm:
**	Description of the algorithm (optional) and any other notes.
**
*/
void
load(LST_HEAD * sops)
{
    char buf[65];

    MUT_LoadList(scrolledList1, sops, formatsop, buf);
}


/* addsop_scrolledList2
**
** Purpose:
**	This function adds the name of SOP class selected to
**	scrolledList2
**
** Parameter Dictionary:
**	sopstr	input character string containg the SOP class name
**
** Return Values:
**	None
**
** Notes:
**
**Algorithm:
**	Description of the algorithm (optional) and any other notes.
**
*/
void
addsop_scrolledList2(char *sopstr)
{
    XmString item;

    item = XmStringCreateSimple(sopstr);

    XmListAddItem(scrolledList2, item, 0);
    XtFree(item);
}


/* copyWtext2
**
** Purpose:
**	This function writes the information and error messages
**	in the text widget.
**
** Parameter Dictionary:
**	buffer	input character string containing the message
**
** Return Values:
**	None
**
** Notes:
**
**Algorithm:
**	Description of the algorithm (optional) and any other notes.
**
*/

void
copyWtext2(char *buffer)
{
    XmTextSetString(scrolledText1, buffer);
}

/* copyWtext
**
** Purpose:
**	This function writes the name of the selected SOP class
**	in the textField window1 inorder to update the options to
**	to be applied to this class or to delete this class from the
**	selected list.
**
** Parameter Dictionary:
**	buffer	input character string containg the SOP class name
**
** Return Values:
**	None
**
** Notes:
**
**Algorithm:
**	Description of the algorithm (optional) and any other notes.
**
*/

void
copyWtext(char *buffer)
{
    XmTextSetString(textField1, buffer);
}


/* update
**
** Purpose:
**	This function updates the service class role and transfer
**	syntaxes for the selected class by checking the state of
**	the toggle buttons.
**
** Parameter Dictionary:
**	None
**
** Return Values:
**	None
**
** Notes:
**
**Algorithm:
**	Description of the algorithm (optional) and any other notes.
**
*/

void
update()
{
    int i;
    SELECTED_SOP *s;

    s = LST_Index(&selectedsopList, itemNo_selected2);
    if (s == NULL)
	return;

    s->tSyntaxes[0][0] = '\0';
    s->tSyntaxes[1][0] = '\0';
    s->tSyntaxes[2][0] = '\0';

    if (XmToggleButtonGadgetGetState(toggleButtonGadget2))
	s->role = DUL_SC_ROLE_SCU;
    else if (XmToggleButtonGadgetGetState(toggleButtonGadget3))
	s->role = DUL_SC_ROLE_SCP;
    else if (XmToggleButtonGadgetGetState(toggleButtonGadget4))
	s->role = DUL_SC_ROLE_SCUSCP;
    else if (XmToggleButtonGadgetGetState(toggleButtonGadget1))
	s->role = DUL_SC_ROLE_DEFAULT;

/* set transfer syntaxes */

    i = 0;
    if (XmToggleButtonGetState(toggleButtonGadget5))
	strcpy(s->tSyntaxes[i++], DICOM_TRANSFERLITTLEENDIAN);

    if (XmToggleButtonGadgetGetState(toggleButtonGadget6))
	strcpy(s->tSyntaxes[i++], DICOM_TRANSFERLITTLEENDIANEXPLICIT);

    if (XmToggleButtonGadgetGetState(toggleButtonGadget7))
	strcpy(s->tSyntaxes[i++], DICOM_TRANSFERBIGENDIANEXPLICIT);
}

/* set_options
**
** Purpose:
**	This function sets up the state of the toggle buttons
**	according to the the service class role and transfer
**	syntaxes already set for the selected class by checking
**	their options.
**
** Parameter Dictionary:
**	None
**
** Return Values:
**	None
**
** Notes:
**
**Algorithm:
**	Description of the algorithm (optional) and any other notes.
**
*/

void
set_options()
{
    int i;

    se = LST_Head(&selectedsopList);
    (void) LST_Position(&selectedsopList, se);

    for (i = 2; i <= itemNo_selected2; i++) {
	se = LST_Next(&selectedsopList);
    }


    if (se->flag == 0) {
	set_options_toDefault();
    } else if (se->flag == 1) {
	if (se->role == DUL_SC_ROLE_DEFAULT) {
	    XmToggleButtonGadgetSetState(toggleButtonGadget1, TRUE, 0);
	} else {
	    XmToggleButtonGadgetSetState(toggleButtonGadget1, FALSE, 0);
	}

	if (se->role == DUL_SC_ROLE_SCU) {
	    XmToggleButtonGadgetSetState(toggleButtonGadget2, TRUE, 0);
	} else {
	    XmToggleButtonGadgetSetState(toggleButtonGadget2, FALSE, 0);
	}

	if (se->role == DUL_SC_ROLE_SCP) {
	    XmToggleButtonGadgetSetState(toggleButtonGadget3, TRUE, 0);
	} else {
	    XmToggleButtonGadgetSetState(toggleButtonGadget3, FALSE, 0);
	}

	if (se->role == DUL_SC_ROLE_SCUSCP) {
	    XmToggleButtonGadgetSetState(toggleButtonGadget4, TRUE, 0);
	} else {
	    XmToggleButtonGadgetSetState(toggleButtonGadget4, FALSE, 0);
	}

/* set syntaxes */

	if (strlen(se->tSyntaxes[0]) != 0) {
	    XmToggleButtonGadgetSetState(toggleButtonGadget5, TRUE, 0);
	} else {
	    XmToggleButtonGadgetSetState(toggleButtonGadget5, FALSE, 0);
	}

	if (strlen(se->tSyntaxes[1]) != 0) {
	    XmToggleButtonGadgetSetState(toggleButtonGadget6, TRUE, 0);
	} else {
	    XmToggleButtonGadgetSetState(toggleButtonGadget6, FALSE, 0);
	}

	if (strlen(se->tSyntaxes[2]) != 0) {
	    XmToggleButtonGadgetSetState(toggleButtonGadget7, TRUE, 0);
	} else {
	    XmToggleButtonGadgetSetState(toggleButtonGadget7, FALSE, 0);
	}
    }
    update();
}

/*delete_sop_fromList2
**
** Purpose:
**	This function deletes the name of SOP class from
**	scrolledList2, clears the textwindow from classname.
**
** Parameter Dictionary:
**	None
**
** Return Values:
**	None
**
** Notes:
**
**Algorithm:
**	Description of the algorithm (optional) and any other notes.
**
*/
void
delete_sop_fromList2()
{
    memset(buf_mes, 0, sizeof(buf_mes));
    XmTextSetString(textField6, buf_mes);

    if (itemNo_selected2 != 0) {
	XmListDeletePos(scrolledList2, itemNo_selected2);
	strcpy(buf_mes, "");
	copyWtext(buf_mes);

	update_selected_sop_inList2(itemNo_selected2);
	itemNo_selected2 = 0;
	free(sopstr2);

	if (strcmp(XmTextGetString(textField6), " SELECT AN ITEM FROM SCROLLEDLIST2 BEFORE APPLYING DELETE") == 0) {
	    strcpy(buf_mes, "");
	    XmTextSetString(textField6, buf_mes);
	}
	set_options_toDefault();
    } else {
	memset(buf_mes, 0, sizeof(buf_mes));
	strcpy(buf_mes, " SELECT AN ITEM FROM SCROLLEDLIST2 BEFORE APPLYING DELETE");

	XmTextSetString(textField6, buf_mes);
    }
}
/*set_options_toDefault
**
** Purpose:
**	This function sets the state of the toggle buttons
**	to default.
**
** Parameter Dictionary:
**	None
**
** Return Values:
**	None
**
** Notes:
**
**Algorithm:
**	Description of the algorithm (optional) and any other notes.
**
*/

void
set_options_toDefault()
{
    XmToggleButtonGadgetSetState(toggleButtonGadget1, TRUE, 0);
    XmToggleButtonGadgetSetState(toggleButtonGadget5, TRUE, 0);

    XmToggleButtonGadgetSetState(toggleButtonGadget2, FALSE, 0);
    XmToggleButtonGadgetSetState(toggleButtonGadget3, FALSE, 0);
    XmToggleButtonGadgetSetState(toggleButtonGadget4, FALSE, 0);
    XmToggleButtonGadgetSetState(toggleButtonGadget6, FALSE, 0);
    XmToggleButtonGadgetSetState(toggleButtonGadget7, FALSE, 0);
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void
activateCB_menu1_p1_b2(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    /*
     * activateCB_menu1_p1_b2 *
     * 
     * Purpose: *	This routine calls a function to close the network connection *
     * 
     * Parameter Dictionary: *	None *
     * 
     * Return Values: *	None *
     * 
     * Notes: *
     * 
     * Algorithm: *	Description of the algorithm (optional) and any other
     * notes. *
     * 
     */
    {
	closeNetworkConnection();
	exit(0);
    }
}

static void
browseSelectionCB_scrolledList1(
				Widget wgt,
				XtPointer cd,
				XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	/*
	 * browseSelectionCB_scrolledList1 *
	 * 
	 * Purpose: *	This function selects an SOP class from scrolled
	 * List1 *	and calls functions to add it in scrolledList2, and *
	 * selectedsopList. *
	 * 
	 * Parameter Dictionary: *	None *
	 * 
	 * Return Values: *	None *
	 * 
	 * Notes: *
	 * 
	 * Algorithm: *	Description of the algorithm (optional) and any other
	 * notes. *
	 * 
	 */

	XmListCallbackStruct *cbs;
	char *sopstr1;
	int one;

	cbs = (XmListCallbackStruct *) UxCallbackArg;

	strcpy(buf_mes, "");

	if ((sopstr1 = (char *) malloc(65)) == NULL) {
	    strcpy(buf_mes, "malloc sopstr1 failed in scrolledList1");
	    copyWtext2(buf_mes);
	    strcpy(buf_mes, "");
	}
	if (!XmStringGetLtoR(cbs->item, XmSTRING_DEFAULT_CHARSET, &sopstr1)) {
	    strcpy(buf_mes, "No SOP class selected from scrolledList1");
	    copyWtext2(buf_mes);
	    strcpy(buf_mes, "");
	}
	itemNo_selected1 = cbs->item_position;
	one = itemNo_selected1;

	addsop_scrolledList2(sopstr1);
	selected_sop_fromList1(one);
	free(sopstr1);
    }
}

static void
browseSelectionCB_scrolledList2(
				Widget wgt,
				XtPointer cd,
				XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	/*
	 * browseSelectionCB_scrolledList2 *
	 * 
	 * Purpose: *	This function selects an SOP class from scrolledList2 *
	 * and calls function update to update the options on *	the
	 * toggleButtons. *
	 * 
	 * Parameter Dictionary: *	None *
	 * 
	 * Return Values: *	None *
	 * 
	 * Notes: *
	 * 
	 * Algorithm: *	Description of the algorithm (optional) and any other
	 * notes. *
	 * 
	 */
	XmListCallbackStruct *cbs;

	cbs = (XmListCallbackStruct *) UxCallbackArg;

	strcpy(buf_mes, "");

	if ((sopstr2 = (char *) malloc(65)) == NULL) {
	    strcpy(buf_mes, " malloc sopstr2 failed in scrolledList2");
	    copyWtext2(buf_mes);
	    strcpy(buf_mes, "");
	}
	if (!XmStringGetLtoR(cbs->item, XmSTRING_DEFAULT_CHARSET, &sopstr2)) {
	    strcpy(buf_mes, "No sopclass selected from scrolledList2");
	    copyWtext2(buf_mes);
	    strcpy(buf_mes, "");
	}
	itemNo_selected2 = cbs->item_position;

	set_options();

	strcpy(buf_mes, sopstr2);
	copyWtext(buf_mes);
	strcpy(buf_mes, "");

    }
}

static void
activateCB_scrolledText1(
			 Widget wgt,
			 XtPointer cd,
			 XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {

    }
}

static void
activateCB_pushButton1(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	/*
	 * activateCB_pushButton1 *
	 * 
	 * Purpose: *	This function reads association request parameters
	 * and *	requests an association *
	 * 
	 * Parameter Dictionary: *	None *
	 * 
	 * Return Values: *	None *
	 * 
	 * Notes: *
	 * 
	 * Algorithm: *	Description of the algorithm (optional) and any other
	 * notes. *
	 * 
	 */

	char buffer[512] = "";

	text_typed = XmTextFieldGetString(textField2);
	if (strlen(text_typed) != 0) {
	    strcpy(node, text_typed);
	    XtFree(text_typed);
	} else {
	    strcat(buffer, "Association Request Parameters,  Node  not supplied !\n");
	}

	text_typed = XmTextFieldGetString(textField3);
	if (strlen(text_typed) != 0) {
	    strcpy(port, text_typed);
	    XtFree(text_typed);
	} else {
	    strcat(buffer, "Association Request Parameter,  PORT not supplied !\n");
	}

	text_typed = XmTextFieldGetString(textField4);
	if (strlen(text_typed) != 0) {
	    strcpy(callingAPTitle, text_typed);
	    XtFree(text_typed);
	} else {
	    strcat(buffer, "Association Request Parameter,  Calling App Title not supplied !\n");
	}

	text_typed = XmTextFieldGetString(textField5);
	if (strlen(text_typed) != 0) {
	    strcpy(calledAPTitle, text_typed);
	    XtFree(text_typed);
	} else {
	    strcat(buffer, "Association Request Parameter,  called App Title not supplied !\n");
	}

	if (strlen(buffer) != 0) {
	    copyWtext2(buffer);
	} else {
	    request_association(node, port, calledAPTitle, callingAPTitle);
	}
    }
}

static void
activateCB_pushButton2(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	/*
	 * activateCB_pushButton2 *
	 * 
	 * Purpose: *	This function requests for release of association *
	 * 
	 * Parameter Dictionary: *	None *
	 * 
	 * Return Values: *	None *
	 * 
	 * Notes: *
	 * 
	 * Algorithm: *	Description of the algorithm (optional) and any other
	 * notes. *
	 * 
	 */
	request_release();
    }
}

static void
activateCB_pushButton3(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	/*
	 * activateCB_pushButton3 *
	 * 
	 * Purpose: *	This function writes message in textField6 to select
	 * an *	SOP class from scrolledList2 before applying update, if * the
	 * class is not already selected *
	 * 
	 * Parameter Dictionary: *	None *
	 * 
	 * Return Values: *	None *
	 * 
	 * Notes: *
	 * 
	 * Algorithm: *	Description of the algorithm (optional) and any other
	 * notes. *
	 * 
	 */

	if (strlen(XmTextGetString(textField1)) != 0) {
	    XmTextSetString(textField6, "");
	    update();
	} else {
	    XmTextSetString(textField6,
			    "Select an item from list of requested SOP classes before applying UPDATE");
	}


    }
}

static void
activateCB_pushButton4(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	/*
	 * activateCB_pushButton4 *
	 * 
	 * Purpose: *	This function calls routine to delete selected sop
	 * class *	from scrolledList2 *
	 * 
	 * Parameter Dictionary: *	None *
	 * 
	 * Return Values: *	None *
	 * 
	 * Notes: *
	 * 
	 * Algorithm: *	Description of the algorithm (optional) and any other
	 * notes. *
	 * 
	 */

	delete_sop_fromList2();
    }
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget
_Uxbuild_topLevelShell1()
{
    Widget _UxParent;
    Widget menu1_p1_shell;


    /* Creation of topLevelShell1 */
    _UxParent = UxParent;
    if (_UxParent == NULL) {
	_UxParent = UxTopLevel;
    }
    topLevelShell1 = XtVaCreatePopupShell("topLevelShell1",
					  topLevelShellWidgetClass,
					  _UxParent,
					  XmNwidth, 1070,
					  XmNheight, 840,
					  XmNx, 90,
					  XmNy, 50,
					  XmNiconName, "ASSOC_TOOL",
					  NULL);


    /* Creation of mainWindow1 */
    mainWindow1 = XtVaCreateManagedWidget("mainWindow1",
					  xmMainWindowWidgetClass,
					  topLevelShell1,
					  XmNwidth, 1070,
					  XmNheight, 840,
					  XmNx, 0,
					  XmNy, 0,
					  XmNunitType, XmPIXELS,
					  NULL);


    /* Creation of menu1 */
    menu1 = XtVaCreateManagedWidget("menu1",
				    xmRowColumnWidgetClass,
				    mainWindow1,
				    XmNrowColumnType, XmMENU_BAR,
				    XmNmenuAccelerator, "<KeyUp>F10",
				    NULL);


    /* Creation of menu1_p1 */
    menu1_p1_shell = XtVaCreatePopupShell("menu1_p1_shell",
					  xmMenuShellWidgetClass, menu1,
					  XmNwidth, 1,
					  XmNheight, 1,
					  XmNallowShellResize, TRUE,
					  XmNoverrideRedirect, TRUE,
					  NULL);

    menu1_p1 = XtVaCreateWidget("menu1_p1",
				xmRowColumnWidgetClass,
				menu1_p1_shell,
				XmNrowColumnType, XmMENU_PULLDOWN,
				NULL);


    /* Creation of menu1_p1_b1 */
    menu1_p1_b1 = XtVaCreateManagedWidget("menu1_p1_b1",
					  xmSeparatorGadgetClass,
					  menu1_p1,
					  NULL);


    /* Creation of menu1_p1_b2 */
    menu1_p1_b2 = XtVaCreateManagedWidget("menu1_p1_b2",
					  xmPushButtonWidgetClass,
					  menu1_p1,
					RES_CONVERT(XmNlabelString, "Quit"),
					  RES_CONVERT(XmNmnemonic, "Q"),
					  NULL);
    XtAddCallback(menu1_p1_b2, XmNactivateCallback,
		  (XtCallbackProc) activateCB_menu1_p1_b2,
		  (XtPointer) NULL);



    /* Creation of menu1_top_b1 */
    menu1_top_b1 = XtVaCreateManagedWidget("menu1_top_b1",
					   xmCascadeButtonWidgetClass,
					   menu1,
				     RES_CONVERT(XmNlabelString, "Control"),
					   RES_CONVERT(XmNmnemonic, "C"),
					   XmNsubMenuId, menu1_p1,
					   NULL);


    /* Creation of bulletinBoard1 */
    bulletinBoard1 = XtVaCreateManagedWidget("bulletinBoard1",
					     xmBulletinBoardWidgetClass,
					     mainWindow1,
					     NULL);


    /* Creation of scrolledWindowList1 */
    scrolledWindowList1 = XtVaCreateManagedWidget("scrolledWindowList1",
						xmScrolledWindowWidgetClass,
						  bulletinBoard1,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNshadowThickness, 0,
						  XmNx, 60,
						  XmNy, 30,
						  NULL);


    /* Creation of scrolledList1 */
    scrolledList1 = XtVaCreateManagedWidget("scrolledList1",
					    xmListWidgetClass,
					    scrolledWindowList1,
					    XmNwidth, 440,
					    XmNheight, 150,
					    XmNx, 0,
					    XmNy, 30,
					    XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
					    XmNhighlightOnEnter, FALSE,
					XmNselectionPolicy, XmBROWSE_SELECT,
					    NULL);
    XtAddCallback(scrolledList1, XmNbrowseSelectionCallback,
		  (XtCallbackProc) browseSelectionCB_scrolledList1,
		  (XtPointer) NULL);



    /* Creation of scrolledWindowList2 */
    scrolledWindowList2 = XtVaCreateManagedWidget("scrolledWindowList2",
						xmScrolledWindowWidgetClass,
						  bulletinBoard1,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNshadowThickness, 0,
						  XmNx, 610,
						  XmNy, 30,
						  NULL);


    /* Creation of scrolledWindowList4 */
    scrolledWindowList4 = XtVaCreateManagedWidget("scrolledWindowList4",
						xmScrolledWindowWidgetClass,
						  scrolledWindowList2,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNshadowThickness, 0,
						  XmNx, 0,
						  XmNy, 0,
						  NULL);


    /* Creation of scrolledList2 */
    scrolledList2 = XtVaCreateManagedWidget("scrolledList2",
					    xmListWidgetClass,
					    scrolledWindowList4,
					    XmNwidth, 430,
					    XmNheight, 150,
					    XmNx, 0,
					    XmNy, 30,
					    XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
					    XmNhighlightOnEnter, FALSE,
					XmNselectionPolicy, XmBROWSE_SELECT,
					    NULL);
    XtAddCallback(scrolledList2, XmNbrowseSelectionCallback,
		  (XtCallbackProc) browseSelectionCB_scrolledList2,
		  (XtPointer) NULL);



    /* Creation of separatorGadget1 */
    separatorGadget1 = XtVaCreateManagedWidget("separatorGadget1",
					       xmSeparatorGadgetClass,
					       bulletinBoard1,
					       XmNwidth, 1030,
					       XmNx, 20,
					       XmNy, 200,
					       XmNheight, 20,
					       NULL);


    /* Creation of labelGadget1 */
    labelGadget1 = XtVaCreateManagedWidget("labelGadget1",
					   xmLabelGadgetClass,
					   bulletinBoard1,
					   XmNx, 160,
					   XmNy, 230,
					   XmNwidth, 180,
					   XmNheight, 30,
			  RES_CONVERT(XmNlabelString, "Selected SopClass:"),
					   XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
					   NULL);


    /* Creation of textField1 */
    textField1 = XtVaCreateManagedWidget("textField1",
					 xmTextFieldWidgetClass,
					 bulletinBoard1,
					 XmNwidth, 490,
					 XmNx, 330,
					 XmNy, 230,
					 XmNheight, 40,
					 XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
					 NULL);


    /* Creation of rowColumn1 */
    rowColumn1 = XtVaCreateManagedWidget("rowColumn1",
					 xmRowColumnWidgetClass,
					 bulletinBoard1,
					 XmNwidth, 228,
					 XmNheight, 147,
					 XmNx, 200,
					 XmNy, 280,
					 XmNborderWidth, 0,
					 XmNradioBehavior, TRUE,
					 NULL);


    /* Creation of toggleButtonGadget1 */
    toggleButtonGadget1 = XtVaCreateManagedWidget("toggleButtonGadget1",
						  xmToggleButtonGadgetClass,
						  rowColumn1,
						  XmNx, 0,
						  XmNy, 0,
						  XmNwidth, 222,
						  XmNheight, 33,
				 RES_CONVERT(XmNlabelString, "Default SCU"),
						  XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
						  XmNset, TRUE,
						  NULL);


    /* Creation of toggleButtonGadget2 */
    toggleButtonGadget2 = XtVaCreateManagedWidget("toggleButtonGadget2",
						  xmToggleButtonGadgetClass,
						  rowColumn1,
						  XmNx, 0,
						  XmNy, 10,
						  XmNwidth, 222,
						  XmNheight, 33,
		      RES_CONVERT(XmNlabelString, "Service Class User SCU"),
						  XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
						  NULL);


    /* Creation of toggleButtonGadget3 */
    toggleButtonGadget3 = XtVaCreateManagedWidget("toggleButtonGadget3",
						  xmToggleButtonGadgetClass,
						  rowColumn1,
						  XmNx, 0,
						  XmNy, 41,
						  XmNwidth, 222,
						  XmNheight, 33,
		  RES_CONVERT(XmNlabelString, "Service Class Provider SCP"),
						  XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
						  NULL);


    /* Creation of toggleButtonGadget4 */
    toggleButtonGadget4 = XtVaCreateManagedWidget("toggleButtonGadget4",
						  xmToggleButtonGadgetClass,
						  rowColumn1,
						  XmNx, 0,
						  XmNy, 72,
						  XmNwidth, 222,
						  XmNheight, 33,
				     RES_CONVERT(XmNlabelString, "SCU/SCP"),
						  XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
						  NULL);


    /* Creation of toggleButtonGadget5 */
    toggleButtonGadget5 = XtVaCreateManagedWidget("toggleButtonGadget5",
						  xmToggleButtonGadgetClass,
						  bulletinBoard1,
						  XmNx, 620,
						  XmNy, 280,
						  XmNwidth, 210,
						  XmNheight, 30,
		      RES_CONVERT(XmNlabelString, "Implicit LITTLE ENDIAN"),
						  XmNset, TRUE,
						  XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
						  NULL);


    /* Creation of toggleButtonGadget6 */
    toggleButtonGadget6 = XtVaCreateManagedWidget("toggleButtonGadget6",
						  xmToggleButtonGadgetClass,
						  bulletinBoard1,
						  XmNx, 620,
						  XmNy, 330,
						  XmNwidth, 210,
						  XmNheight, 30,
						  XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
		      RES_CONVERT(XmNlabelString, "Explicit LITTLE ENDIAN"),
						  NULL);


    /* Creation of toggleButtonGadget7 */
    toggleButtonGadget7 = XtVaCreateManagedWidget("toggleButtonGadget7",
						  xmToggleButtonGadgetClass,
						  bulletinBoard1,
						  XmNx, 620,
						  XmNy, 370,
						  XmNwidth, 210,
						  XmNheight, 30,
						  XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
			 RES_CONVERT(XmNlabelString, "Explicit BIG ENDIAN"),
						  NULL);


    /* Creation of separatorGadget2 */
    separatorGadget2 = XtVaCreateManagedWidget("separatorGadget2",
					       xmSeparatorGadgetClass,
					       bulletinBoard1,
					       XmNwidth, 1030,
					       XmNx, 20,
					       XmNy, 470,
					       XmNheight, 10,
					       NULL);


    /* Creation of scrolledWindowText1 */
    scrolledWindowText1 = XtVaCreateManagedWidget("scrolledWindowText1",
						xmScrolledWindowWidgetClass,
						  bulletinBoard1,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNx, 90,
						  XmNy, 640,
						  NULL);


    /* Creation of scrolledText1 */
    scrolledText1 = XtVaCreateManagedWidget("scrolledText1",
					    xmTextWidgetClass,
					    scrolledWindowText1,
					    XmNwidth, 900,
					    XmNheight, 100,
					    XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--12-120-75-75-p-70-iso8859-1"),
					    XmNeditMode, XmMULTI_LINE_EDIT,
					    NULL);
    XtAddCallback(scrolledText1, XmNactivateCallback,
		  (XtCallbackProc) activateCB_scrolledText1,
		  (XtPointer) NULL);



    /* Creation of pushButton1 */
    pushButton1 = XtVaCreateManagedWidget("pushButton1",
					  xmPushButtonWidgetClass,
					  bulletinBoard1,
					  XmNx, 270,
					  XmNy, 770,
					  XmNwidth, 130,
					  XmNheight, 30,
					  XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
				     RES_CONVERT(XmNlabelString, "Request"),
					  NULL);
    XtAddCallback(pushButton1, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton1,
		  (XtPointer) NULL);



    /* Creation of pushButton2 */
    pushButton2 = XtVaCreateManagedWidget("pushButton2",
					  xmPushButtonWidgetClass,
					  bulletinBoard1,
					  XmNx, 600,
					  XmNy, 770,
					  XmNwidth, 130,
					  XmNheight, 30,
					  XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
				     RES_CONVERT(XmNlabelString, "Release"),
					  NULL);
    XtAddCallback(pushButton2, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton2,
		  (XtPointer) NULL);



    /* Creation of labelGadget2 */
    labelGadget2 = XtVaCreateManagedWidget("labelGadget2",
					   xmLabelGadgetClass,
					   bulletinBoard1,
					   XmNx, 180,
					   XmNy, 480,
					   XmNwidth, 270,
					   XmNheight, 30,
	      RES_CONVERT(XmNlabelString, "Association Request Parameters"),
					   XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
					   NULL);


    /* Creation of labelGadget3 */
    labelGadget3 = XtVaCreateManagedWidget("labelGadget3",
					   xmLabelGadgetClass,
					   bulletinBoard1,
					   XmNx, 200,
					   XmNy, 530,
					   XmNwidth, 130,
					   XmNheight, 30,
					   XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
				 RES_CONVERT(XmNlabelString, "Remote Node"),
					   NULL);


    /* Creation of labelGadget4 */
    labelGadget4 = XtVaCreateManagedWidget("labelGadget4",
					   xmLabelGadgetClass,
					   bulletinBoard1,
					   XmNx, 590,
					   XmNy, 530,
					   XmNwidth, 140,
					   XmNheight, 30,
					   XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
				 RES_CONVERT(XmNlabelString, "Remote Port"),
					   NULL);


    /* Creation of labelGadget5 */
    labelGadget5 = XtVaCreateManagedWidget("labelGadget5",
					   xmLabelGadgetClass,
					   bulletinBoard1,
					   XmNx, 200,
					   XmNy, 580,
					   XmNwidth, 130,
					   XmNheight, 30,
					   XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
			   RES_CONVERT(XmNlabelString, "Calling App Title"),
					   NULL);


    /* Creation of labelGadget6 */
    labelGadget6 = XtVaCreateManagedWidget("labelGadget6",
					   xmLabelGadgetClass,
					   bulletinBoard1,
					   XmNx, 590,
					   XmNy, 580,
					   XmNwidth, 140,
					   XmNheight, 30,
					   XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
			    RES_CONVERT(XmNlabelString, "Called App Title"),
					   NULL);


    /* Creation of separatorGadget3 */
    separatorGadget3 = XtVaCreateManagedWidget("separatorGadget3",
					       xmSeparatorGadgetClass,
					       bulletinBoard1,
					       XmNwidth, 1050,
					       XmNx, 20,
					       XmNy, 620,
					       XmNheight, 10,
					       NULL);


    /* Creation of textField2 */
    textField2 = XtVaCreateManagedWidget("textField2",
					 xmTextFieldWidgetClass,
					 bulletinBoard1,
					 XmNwidth, 160,
					 XmNx, 360,
					 XmNy, 520,
					 XmNheight, 40,
					 XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
					 NULL);


    /* Creation of textField3 */
    textField3 = XtVaCreateManagedWidget("textField3",
					 xmTextFieldWidgetClass,
					 bulletinBoard1,
					 XmNwidth, 160,
					 XmNx, 740,
					 XmNy, 520,
					 XmNheight, 40,
					 XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
					 NULL);


    /* Creation of textField4 */
    textField4 = XtVaCreateManagedWidget("textField4",
					 xmTextFieldWidgetClass,
					 bulletinBoard1,
					 XmNwidth, 160,
					 XmNx, 360,
					 XmNy, 570,
					 XmNheight, 40,
					 XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
					 NULL);


    /* Creation of textField5 */
    textField5 = XtVaCreateManagedWidget("textField5",
					 xmTextFieldWidgetClass,
					 bulletinBoard1,
					 XmNwidth, 160,
					 XmNx, 740,
					 XmNy, 570,
					 XmNheight, 40,
					 XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
					 NULL);


    /* Creation of pushButton3 */
    pushButton3 = XtVaCreateManagedWidget("pushButton3",
					  xmPushButtonWidgetClass,
					  bulletinBoard1,
					  XmNx, 320,
					  XmNy, 410,
					  XmNwidth, 110,
					  XmNheight, 30,
					  XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
				      RES_CONVERT(XmNlabelString, "Update"),
					  NULL);
    XtAddCallback(pushButton3, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton3,
		  (XtPointer) NULL);



    /* Creation of pushButton4 */
    pushButton4 = XtVaCreateManagedWidget("pushButton4",
					  xmPushButtonWidgetClass,
					  bulletinBoard1,
					  XmNx, 600,
					  XmNy, 410,
					  XmNwidth, 110,
					  XmNheight, 30,
					  XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
				      RES_CONVERT(XmNlabelString, "Delete"),
					  XmNdefaultButtonShadowThickness, 0,
					  NULL);
    XtAddCallback(pushButton4, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton4,
		  (XtPointer) NULL);



    /* Creation of textField6 */
    textField6 = XtVaCreateManagedWidget("textField6",
					 xmTextFieldWidgetClass,
					 bulletinBoard1,
					 XmNwidth, 600,
					 XmNx, 250,
					 XmNy, 440,
					 XmNheight, 30,
					 XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--12-120-75-75-p-70-iso8859-1"),
					 NULL);


    XmMainWindowSetAreas(mainWindow1, menu1, (Widget) NULL,
			 (Widget) NULL, (Widget) NULL, bulletinBoard1);

    return (topLevelShell1);
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget
create_topLevelShell1(swidget _UxUxParent)
{
    Widget rtrn;

    UxParent = _UxUxParent;

    rtrn = _Uxbuild_topLevelShell1();

    return (rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/
