
/*******************************************************************************
	applicationShell1.c

       Associated Header file: applicationShell1.h
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/BulletinB.h>
#include <Xm/PushB.h>
#include <Xm/CascadeB.h>
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
** Intent:		This file contains the generated code that
**			defines the top level shell for the
**			cfg_ctn_tables application.  This shell serves
**			as a parent for the interesting widgets that
**			actually provide the user interface.  This file
**			contains the callback functions for push buttons
**			which activate the various interfaces provided
**			under the umbrella of this single program.
** Last Update:         $Author: smm $, $Date: 1995-04-10 13:19:31 $
** Source File:         $RCSfile: applicationShell1.c,v $
** Revision:            $Revision: 1.9 $
** Status:              $State: Exp $
*/

static char rcsid[] = "$Revision: 1.9 $ $RCSfile: applicationShell1.c,v $";


#include "dicom.h"

static Widget wApplicationForm = 0;
static CTNBOOLEAN applicationActive = FALSE;
static Widget wSecurityForm = 0;
static CTNBOOLEAN securityActive = FALSE;
static Widget wPrintServerCFGForm = 0;
static CTNBOOLEAN printServerCFGActive = FALSE;
static Widget wGroupNamesForm = 0;
static Widget wFISAccessForm = 0;
static Widget wImageStorageAccessForm = 0;
static Widget wImageStorageControlForm = 0;
static Widget wVideoImageForm = 0;

static void clearInterfaces();


/*******************************************************************************
       The following header file defines the context structure.
*******************************************************************************/

#define CONTEXT_MACRO_ACCESS 1
#include "applicationShell1.h"
#undef CONTEXT_MACRO_ACCESS


/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

static void
clearInterfaces()
{
    if (wApplicationForm != 0)
	(void) XtUnmanageChild(wApplicationForm);
    if (wSecurityForm != 0)
	(void) XtUnmanageChild(wSecurityForm);
    if (wPrintServerCFGForm != 0)
	(void) XtUnmanageChild(wPrintServerCFGForm);
    if (wGroupNamesForm != 0)
	(void) XtUnmanageChild(wGroupNamesForm);
    if (wFISAccessForm != 0)
	(void) XtUnmanageChild(wFISAccessForm);
    if (wImageStorageAccessForm != 0)
	(void) XtUnmanageChild(wImageStorageAccessForm);
    if (wImageStorageControlForm != 0)
	(void) XtUnmanageChild(wImageStorageControlForm);
    if (wVideoImageForm != 0)
	(void) XtUnmanageChild(wVideoImageForm);
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void
activateCB_menu1_p4_b2(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    _UxCapplicationShell1 *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxApplicationShell1Context;
    UxApplicationShell1Context = UxContext =
	(_UxCapplicationShell1 *) UxGetContext(UxWidget);
    {
#ifdef RUNTIME
	clearInterfaces();
	if (wApplicationForm == 0)
	    wApplicationForm = (Widget) create_applicationForm(bulletinBoard3);

	XtManageChild(wApplicationForm);

	loadApplicationList();
#endif



    }
    UxApplicationShell1Context = UxSaveCtx;
}

static void
activateCB_menu1_p4_b3(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    _UxCapplicationShell1 *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxApplicationShell1Context;
    UxApplicationShell1Context = UxContext =
	(_UxCapplicationShell1 *) UxGetContext(UxWidget);
    {
#ifdef RUNTIME
	clearInterfaces();

	if (wSecurityForm == 0)
	    wSecurityForm = (Widget) create_securityForm(bulletinBoard3);

	XtManageChild(wSecurityForm);

	loadSecurityMatrixList();
#endif
    }
    UxApplicationShell1Context = UxSaveCtx;
}

static void
activateCB_menu1_p4_b1(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    _UxCapplicationShell1 *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxApplicationShell1Context;
    UxApplicationShell1Context = UxContext =
	(_UxCapplicationShell1 *) UxGetContext(UxWidget);
    {
#ifdef RUNTIME
	clearInterfaces();

	if (wGroupNamesForm == 0)
	    wGroupNamesForm = (Widget) create_groupNamesForm(bulletinBoard3);

	XtManageChild(wGroupNamesForm);

	loadGroupGroupList();
#endif
    }
    UxApplicationShell1Context = UxSaveCtx;
}

static void
activateCB_menu1_p6_b1(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    _UxCapplicationShell1 *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxApplicationShell1Context;
    UxApplicationShell1Context = UxContext =
	(_UxCapplicationShell1 *) UxGetContext(UxWidget);
    {
#ifdef RUNTIME
	clearInterfaces();
	if (wImageStorageAccessForm == 0)
	    wImageStorageAccessForm = (Widget) create_imageStorageAccessForm(bulletinBoard3);

	XtManageChild(wImageStorageAccessForm);

	loadImageStorageAccessList();
#endif
    }
    UxApplicationShell1Context = UxSaveCtx;
}

static void
activateCB_menu1_p6_b2(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    _UxCapplicationShell1 *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxApplicationShell1Context;
    UxApplicationShell1Context = UxContext =
	(_UxCapplicationShell1 *) UxGetContext(UxWidget);
    {
#ifdef RUNTIME
	clearInterfaces();
	if (wImageStorageControlForm == 0)
	    wImageStorageControlForm = (Widget) create_imageStorageControlForm(bulletinBoard3);

	XtManageChild(wImageStorageControlForm);
	loadImageStorageControlList();
#endif
    }
    UxApplicationShell1Context = UxSaveCtx;
}

static void
activateCB_menu1_p2_b1(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    _UxCapplicationShell1 *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxApplicationShell1Context;
    UxApplicationShell1Context = UxContext =
	(_UxCapplicationShell1 *) UxGetContext(UxWidget);
    {
#ifdef RUNTIME
	clearInterfaces();

	if (wPrintServerCFGForm == 0)
	    wPrintServerCFGForm = (Widget) create_printServerCFGForm(bulletinBoard3);

	XtManageChild(wPrintServerCFGForm);

	loadPrintServerCFGList();
#endif
    }
    UxApplicationShell1Context = UxSaveCtx;
}

static void
activateCB_menu1_p5_b1(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    _UxCapplicationShell1 *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxApplicationShell1Context;
    UxApplicationShell1Context = UxContext =
	(_UxCapplicationShell1 *) UxGetContext(UxWidget);
    {
#ifdef RUNTIME
	clearInterfaces();

	if (wFISAccessForm == 0)
	    wFISAccessForm = (Widget) create_FISAccessForm(bulletinBoard3);

	XtManageChild(wFISAccessForm);

	loadFISAccessList();
#endif
    }
    UxApplicationShell1Context = UxSaveCtx;
}

static void
activateCB_menu1_p7_b1(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    _UxCapplicationShell1 *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxApplicationShell1Context;
    UxApplicationShell1Context = UxContext =
	(_UxCapplicationShell1 *) UxGetContext(UxWidget);
    {
#ifdef RUNTIME
	clearInterfaces();
	if (wVideoImageForm == 0)
	    wVideoImageForm = (Widget) create_videoImageForm(bulletinBoard3);

	XtManageChild(wVideoImageForm);
	loadVideoImageList();
#endif
    }
    UxApplicationShell1Context = UxSaveCtx;
}

static void
activateCB_menu1_p1_b3(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    _UxCapplicationShell1 *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxApplicationShell1Context;
    UxApplicationShell1Context = UxContext =
	(_UxCapplicationShell1 *) UxGetContext(UxWidget);
    {
	exit(0);
    }
    UxApplicationShell1Context = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget
_Uxbuild_applicationShell1()
{
    Widget _UxParent;
    Widget menu1_p1_shell;
    Widget menu1_p4_shell;
    Widget menu1_p6_shell;
    Widget menu1_p2_shell;
    Widget menu1_p5_shell;
    Widget menu1_p7_shell;


    /* Creation of applicationShell1 */
    _UxParent = UxParent;
    if (_UxParent == NULL) {
	_UxParent = UxTopLevel;
    }
    applicationShell1 = XtVaCreatePopupShell("applicationShell1",
					     applicationShellWidgetClass,
					     _UxParent,
					     XmNwidth, 830,
					     XmNheight, 650,
					     XmNx, 209,
					     XmNy, 241,
					     NULL);
    UxPutContext(applicationShell1, (char *) UxApplicationShell1Context);


    /* Creation of mainWindow1 */
    mainWindow1 = XtVaCreateManagedWidget("mainWindow1",
					  xmMainWindowWidgetClass,
					  applicationShell1,
					  XmNwidth, 830,
					  XmNheight, 650,
					  XmNx, 130,
					  XmNy, 90,
					  XmNunitType, XmPIXELS,
					  NULL);
    UxPutContext(mainWindow1, (char *) UxApplicationShell1Context);


    /* Creation of menu1 */
    menu1 = XtVaCreateManagedWidget("menu1",
				    xmRowColumnWidgetClass,
				    mainWindow1,
				    XmNrowColumnType, XmMENU_BAR,
				    XmNmenuAccelerator, "<KeyUp>F10",
				    NULL);
    UxPutContext(menu1, (char *) UxApplicationShell1Context);


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
    UxPutContext(menu1_p1, (char *) UxApplicationShell1Context);


    /* Creation of menu1_p4 */
    menu1_p4_shell = XtVaCreatePopupShell("menu1_p4_shell",
					  xmMenuShellWidgetClass, menu1_p1,
					  XmNwidth, 1,
					  XmNheight, 1,
					  XmNallowShellResize, TRUE,
					  XmNoverrideRedirect, TRUE,
					  NULL);

    menu1_p4 = XtVaCreateWidget("menu1_p4",
				xmRowColumnWidgetClass,
				menu1_p4_shell,
				XmNrowColumnType, XmMENU_PULLDOWN,
				NULL);
    UxPutContext(menu1_p4, (char *) UxApplicationShell1Context);


    /* Creation of menu1_p4_b2 */
    menu1_p4_b2 = XtVaCreateManagedWidget("menu1_p4_b2",
					  xmPushButtonWidgetClass,
					  menu1_p4,
				RES_CONVERT(XmNlabelString, "Applications"),
					  NULL);
    XtAddCallback(menu1_p4_b2, XmNactivateCallback,
		  (XtCallbackProc) activateCB_menu1_p4_b2,
		  (XtPointer) UxApplicationShell1Context);

    UxPutContext(menu1_p4_b2, (char *) UxApplicationShell1Context);


    /* Creation of menu1_p4_b3 */
    menu1_p4_b3 = XtVaCreateManagedWidget("menu1_p4_b3",
					  xmPushButtonWidgetClass,
					  menu1_p4,
			     RES_CONVERT(XmNlabelString, "Security Matrix"),
					  NULL);
    XtAddCallback(menu1_p4_b3, XmNactivateCallback,
		  (XtCallbackProc) activateCB_menu1_p4_b3,
		  (XtPointer) UxApplicationShell1Context);

    UxPutContext(menu1_p4_b3, (char *) UxApplicationShell1Context);


    /* Creation of menu1_p4_b1 */
    menu1_p4_b1 = XtVaCreateManagedWidget("menu1_p4_b1",
					  xmPushButtonWidgetClass,
					  menu1_p4,
				       RES_CONVERT(XmNlabelString, "Group"),
					  NULL);
    XtAddCallback(menu1_p4_b1, XmNactivateCallback,
		  (XtCallbackProc) activateCB_menu1_p4_b1,
		  (XtPointer) UxApplicationShell1Context);

    UxPutContext(menu1_p4_b1, (char *) UxApplicationShell1Context);


    /* Creation of menu1_p1_b7 */
    menu1_p1_b7 = XtVaCreateManagedWidget("menu1_p1_b7",
					  xmCascadeButtonWidgetClass,
					  menu1_p1,
				     RES_CONVERT(XmNlabelString, "General"),
					  XmNsubMenuId, menu1_p4,
					  NULL);
    UxPutContext(menu1_p1_b7, (char *) UxApplicationShell1Context);


    /* Creation of menu1_p6 */
    menu1_p6_shell = XtVaCreatePopupShell("menu1_p6_shell",
					  xmMenuShellWidgetClass, menu1_p1,
					  XmNwidth, 1,
					  XmNheight, 1,
					  XmNallowShellResize, TRUE,
					  XmNoverrideRedirect, TRUE,
					  NULL);

    menu1_p6 = XtVaCreateWidget("menu1_p6",
				xmRowColumnWidgetClass,
				menu1_p6_shell,
				XmNrowColumnType, XmMENU_PULLDOWN,
				NULL);
    UxPutContext(menu1_p6, (char *) UxApplicationShell1Context);


    /* Creation of menu1_p6_b1 */
    menu1_p6_b1 = XtVaCreateManagedWidget("menu1_p6_b1",
					  xmPushButtonWidgetClass,
					  menu1_p6,
			      RES_CONVERT(XmNlabelString, "Storage Access"),
					  NULL);
    XtAddCallback(menu1_p6_b1, XmNactivateCallback,
		  (XtCallbackProc) activateCB_menu1_p6_b1,
		  (XtPointer) UxApplicationShell1Context);

    UxPutContext(menu1_p6_b1, (char *) UxApplicationShell1Context);


    /* Creation of menu1_p6_b2 */
    menu1_p6_b2 = XtVaCreateManagedWidget("menu1_p6_b2",
					  xmPushButtonWidgetClass,
					  menu1_p6,
			     RES_CONVERT(XmNlabelString, "Storage Control"),
					  NULL);
    XtAddCallback(menu1_p6_b2, XmNactivateCallback,
		  (XtCallbackProc) activateCB_menu1_p6_b2,
		  (XtPointer) UxApplicationShell1Context);

    UxPutContext(menu1_p6_b2, (char *) UxApplicationShell1Context);


    /* Creation of menu1_p1_b5 */
    menu1_p1_b5 = XtVaCreateManagedWidget("menu1_p1_b5",
					  xmCascadeButtonWidgetClass,
					  menu1_p1,
				RES_CONVERT(XmNlabelString, "Image Server"),
					  XmNsubMenuId, menu1_p6,
					  NULL);
    UxPutContext(menu1_p1_b5, (char *) UxApplicationShell1Context);


    /* Creation of menu1_p2 */
    menu1_p2_shell = XtVaCreatePopupShell("menu1_p2_shell",
					  xmMenuShellWidgetClass, menu1_p1,
					  XmNwidth, 1,
					  XmNheight, 1,
					  XmNallowShellResize, TRUE,
					  XmNoverrideRedirect, TRUE,
					  NULL);

    menu1_p2 = XtVaCreateWidget("menu1_p2",
				xmRowColumnWidgetClass,
				menu1_p2_shell,
				XmNrowColumnType, XmMENU_PULLDOWN,
				NULL);
    UxPutContext(menu1_p2, (char *) UxApplicationShell1Context);


    /* Creation of menu1_p2_b1 */
    menu1_p2_b1 = XtVaCreateManagedWidget("menu1_p2_b1",
					  xmPushButtonWidgetClass,
					  menu1_p2,
			    RES_CONVERT(XmNlabelString, "Print Server CFG"),
					  NULL);
    XtAddCallback(menu1_p2_b1, XmNactivateCallback,
		  (XtCallbackProc) activateCB_menu1_p2_b1,
		  (XtPointer) UxApplicationShell1Context);

    UxPutContext(menu1_p2_b1, (char *) UxApplicationShell1Context);


    /* Creation of menu1_p1_b9 */
    menu1_p1_b9 = XtVaCreateManagedWidget("menu1_p1_b9",
					  xmCascadeButtonWidgetClass,
					  menu1_p1,
				RES_CONVERT(XmNlabelString, "Print Server"),
					  XmNsubMenuId, menu1_p2,
					  NULL);
    UxPutContext(menu1_p1_b9, (char *) UxApplicationShell1Context);


    /* Creation of menu1_p5 */
    menu1_p5_shell = XtVaCreatePopupShell("menu1_p5_shell",
					  xmMenuShellWidgetClass, menu1_p1,
					  XmNwidth, 1,
					  XmNheight, 1,
					  XmNallowShellResize, TRUE,
					  XmNoverrideRedirect, TRUE,
					  NULL);

    menu1_p5 = XtVaCreateWidget("menu1_p5",
				xmRowColumnWidgetClass,
				menu1_p5_shell,
				XmNrowColumnType, XmMENU_PULLDOWN,
				NULL);
    UxPutContext(menu1_p5, (char *) UxApplicationShell1Context);


    /* Creation of menu1_p5_b1 */
    menu1_p5_b1 = XtVaCreateManagedWidget("menu1_p5_b1",
					  xmPushButtonWidgetClass,
					  menu1_p5,
				  RES_CONVERT(XmNlabelString, "FIS Access"),
					  NULL);
    XtAddCallback(menu1_p5_b1, XmNactivateCallback,
		  (XtCallbackProc) activateCB_menu1_p5_b1,
		  (XtPointer) UxApplicationShell1Context);

    UxPutContext(menu1_p5_b1, (char *) UxApplicationShell1Context);


    /* Creation of menu1_p1_b6 */
    menu1_p1_b6 = XtVaCreateManagedWidget("menu1_p1_b6",
					  xmCascadeButtonWidgetClass,
					  menu1_p1,
					  RES_CONVERT(XmNlabelString, "FIS"),
					  XmNsubMenuId, menu1_p5,
					  NULL);
    UxPutContext(menu1_p1_b6, (char *) UxApplicationShell1Context);


    /* Creation of menu1_p7 */
    menu1_p7_shell = XtVaCreatePopupShell("menu1_p7_shell",
					  xmMenuShellWidgetClass, menu1_p1,
					  XmNwidth, 1,
					  XmNheight, 1,
					  XmNallowShellResize, TRUE,
					  XmNoverrideRedirect, TRUE,
					  NULL);

    menu1_p7 = XtVaCreateWidget("menu1_p7",
				xmRowColumnWidgetClass,
				menu1_p7_shell,
				XmNrowColumnType, XmMENU_PULLDOWN,
				NULL);
    UxPutContext(menu1_p7, (char *) UxApplicationShell1Context);


    /* Creation of menu1_p7_b1 */
    menu1_p7_b1 = XtVaCreateManagedWidget("menu1_p7_b1",
					  xmPushButtonWidgetClass,
					  menu1_p7,
				RES_CONVERT(XmNlabelString, "Destinations"),
					  NULL);
    XtAddCallback(menu1_p7_b1, XmNactivateCallback,
		  (XtCallbackProc) activateCB_menu1_p7_b1,
		  (XtPointer) UxApplicationShell1Context);

    UxPutContext(menu1_p7_b1, (char *) UxApplicationShell1Context);


    /* Creation of menu1_p1_b8 */
    menu1_p1_b8 = XtVaCreateManagedWidget("menu1_p1_b8",
					  xmCascadeButtonWidgetClass,
					  menu1_p1,
				 RES_CONVERT(XmNlabelString, "Video Image"),
					  XmNsubMenuId, menu1_p7,
					  NULL);
    UxPutContext(menu1_p1_b8, (char *) UxApplicationShell1Context);


    /* Creation of menu1_p1_b3 */
    menu1_p1_b3 = XtVaCreateManagedWidget("menu1_p1_b3",
					  xmPushButtonWidgetClass,
					  menu1_p1,
					RES_CONVERT(XmNlabelString, "Quit"),
					  NULL);
    XtAddCallback(menu1_p1_b3, XmNactivateCallback,
		  (XtCallbackProc) activateCB_menu1_p1_b3,
		  (XtPointer) UxApplicationShell1Context);

    UxPutContext(menu1_p1_b3, (char *) UxApplicationShell1Context);


    /* Creation of menu1_top_b1 */
    menu1_top_b1 = XtVaCreateManagedWidget("menu1_top_b1",
					   xmCascadeButtonWidgetClass,
					   menu1,
				     RES_CONVERT(XmNlabelString, "Control"),
					   XmNsubMenuId, menu1_p1,
					   NULL);
    UxPutContext(menu1_top_b1, (char *) UxApplicationShell1Context);


    /* Creation of bulletinBoard3 */
    bulletinBoard3 = XtVaCreateManagedWidget("bulletinBoard3",
					     xmBulletinBoardWidgetClass,
					     mainWindow1,
					     NULL);
    UxPutContext(bulletinBoard3, (char *) UxApplicationShell1Context);


    XtAddCallback(applicationShell1, XmNdestroyCallback,
		  (XtCallbackProc) UxDestroyContextCB,
		  (XtPointer) UxApplicationShell1Context);

    XmMainWindowSetAreas(mainWindow1, menu1, (Widget) NULL,
			 (Widget) NULL, (Widget) NULL, bulletinBoard3);

    return (applicationShell1);
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget
create_applicationShell1(swidget _UxUxParent)
{
    Widget rtrn;
    _UxCapplicationShell1 *UxContext;

    UxApplicationShell1Context = UxContext =
	(_UxCapplicationShell1 *) UxNewContext(sizeof(_UxCapplicationShell1), False);

    UxParent = _UxUxParent;

    rtrn = _Uxbuild_applicationShell1();

    return (rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/
