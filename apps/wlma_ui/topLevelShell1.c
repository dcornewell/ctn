
/*******************************************************************************
	topLevelShell1.c

       Associated Header file: topLevelShell1.h
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/Form.h>
#include <Xm/CascadeB.h>
#include <Xm/SeparatoG.h>
#include <Xm/PushB.h>
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
**                   Electronic Radiology Laboratory
**                 Mallinckrodt Institute of Radiology
**              Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	Steve Moore, Summer 1994
** Intent:		This file contains the top level widget/interface
**			for the WLMA application.  It serves as a parent
**			for the other widget hierarchies that actually
**			hold the interface.  This interface contains the
**			"Control" pulldown menu that allows the user to
**			select which interface is to be activated for
**			work list management (the database or demonstration
**			interface).
** Last Update:         $Author: smm $, $Date: 1996-01-22 20:23:18 $
** Source File:         $RCSfile: topLevelShell1.c,v $
** Revision:            $Revision: 1.8 $
** Status:              $State: Exp $
*/

static char rcsid[] = "$Revision: 1.8 $ $RCSfile: topLevelShell1.c,v $";

/*global*/

#include "dicom.h"

static Widget wDatabaseForm;
static CTNBOOLEAN databaseActive = FALSE;
static Widget wDemonstrationForm;
static CTNBOOLEAN demonstrationActive = FALSE;


static Widget topLevelShell1;
static Widget mainWindow1;
static Widget menu1;
static Widget menu1_p1;
static Widget menu1_p1_b2;
static Widget menu1_p1_b3;
static Widget menu1_p1_b5;
static Widget menu1_p1_b4;
static Widget menu1_p1_b1;
static Widget menu1_top_b1;
static Widget form1;
static swidget UxParent;

#define CONTEXT_MACRO_ACCESS 1
#include "topLevelShell1.h"
#undef CONTEXT_MACRO_ACCESS


/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

void
errorText(char *txt)
{
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
    {
#ifdef RUNTIME
	if (demonstrationActive)
	    (void) XtUnmanageChild(wDemonstrationForm);

	if (!databaseActive) {
	    wDatabaseForm = (Widget) create_databaseForm(form1);
	    databaseActive = TRUE;
	} else {
	    XtManageChild(wDatabaseForm);
	}

	loadPatientList();
#endif
    }
}

static void
activateCB_menu1_p1_b3(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
#ifdef RUNTIME
	if (databaseActive) {
	    (void) XtUnmanageChild(wDatabaseForm);
	}
	if (!demonstrationActive) {
	    wDemonstrationForm = (Widget) create_demonstrationForm(form1);
	    demonstrationActive = TRUE;
	} else {
	    XtManageChild(wDemonstrationForm);
	}

	loadPatientStudyList();
	loadWorkList();
#endif
    }
}

static void
activateCB_menu1_p1_b5(
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
activateCB_menu1_p1_b1(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	exit(0);
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
					  XmNheight, 780,
					  XmNx, 107,
					  XmNy, 225,
					  NULL);


    /* Creation of mainWindow1 */
    mainWindow1 = XtVaCreateManagedWidget("mainWindow1",
					  xmMainWindowWidgetClass,
					  topLevelShell1,
					  XmNwidth, 1070,
					  XmNheight, 780,
					  XmNx, 190,
					  XmNy, 210,
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


    /* Creation of menu1_p1_b2 */
    menu1_p1_b2 = XtVaCreateManagedWidget("menu1_p1_b2",
					  xmPushButtonWidgetClass,
					  menu1_p1,
			       RES_CONVERT(XmNlabelString, "Edit Database"),
					  NULL);
    XtAddCallback(menu1_p1_b2, XmNactivateCallback,
		  (XtCallbackProc) activateCB_menu1_p1_b2,
		  (XtPointer) NULL);



    /* Creation of menu1_p1_b3 */
    menu1_p1_b3 = XtVaCreateManagedWidget("menu1_p1_b3",
					  xmPushButtonWidgetClass,
					  menu1_p1,
		     RES_CONVERT(XmNlabelString, "Demonstration Interface"),
					  NULL);
    XtAddCallback(menu1_p1_b3, XmNactivateCallback,
		  (XtCallbackProc) activateCB_menu1_p1_b3,
		  (XtPointer) NULL);



    /* Creation of menu1_p1_b5 */
    menu1_p1_b5 = XtVaCreateManagedWidget("menu1_p1_b5",
					  xmPushButtonWidgetClass,
					  menu1_p1,
			       RES_CONVERT(XmNlabelString, "Configuration"),
					  NULL);
    XtAddCallback(menu1_p1_b5, XmNactivateCallback,
		  (XtCallbackProc) activateCB_menu1_p1_b5,
		  (XtPointer) NULL);



    /* Creation of menu1_p1_b4 */
    menu1_p1_b4 = XtVaCreateManagedWidget("menu1_p1_b4",
					  xmSeparatorGadgetClass,
					  menu1_p1,
					  NULL);


    /* Creation of menu1_p1_b1 */
    menu1_p1_b1 = XtVaCreateManagedWidget("menu1_p1_b1",
					  xmPushButtonWidgetClass,
					  menu1_p1,
					RES_CONVERT(XmNlabelString, "Quit"),
					  NULL);
    XtAddCallback(menu1_p1_b1, XmNactivateCallback,
		  (XtCallbackProc) activateCB_menu1_p1_b1,
		  (XtPointer) NULL);



    /* Creation of menu1_top_b1 */
    menu1_top_b1 = XtVaCreateManagedWidget("menu1_top_b1",
					   xmCascadeButtonWidgetClass,
					   menu1,
				     RES_CONVERT(XmNlabelString, "Control"),
					   XmNsubMenuId, menu1_p1,
					   NULL);


    /* Creation of form1 */
    form1 = XtVaCreateManagedWidget("form1",
				    xmFormWidgetClass,
				    mainWindow1,
				    NULL);


    XmMainWindowSetAreas(mainWindow1, menu1, (Widget) NULL,
			 (Widget) NULL, (Widget) NULL, form1);

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

    {
	/* initial code */
	rtrn = _Uxbuild_topLevelShell1();

	/* final code */
	return (rtrn);
    }
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/
