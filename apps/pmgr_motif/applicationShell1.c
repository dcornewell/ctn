
/*******************************************************************************
	applicationShell1.c

       Associated Header file: applicationShell1.h
*******************************************************************************/

#include "ctn_os.h"
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/Text.h>
#include <Xm/ScrolledW.h>
#include <Xm/Frame.h>
#include <Xm/Form.h>
#include <Xm/CascadeB.h>
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
** Author, Date:	Chander Sabharwal
** Intent:		Define global widgets used throughout the interface.
** Last Update:         $Author: smm $, $Date: 2002-10-31 23:36:05 $
** Source File:         $RCSfile: applicationShell1.c,v $
** Revision:            $Revision: 1.21 $
** Status:              $State: Exp $
*/

static char rcsid[] = "$Revision: 1.21 $ $RCSfile: applicationShell1.c,v $";


#include "dicom.h"
#include "lst.h"
#include "condition.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "dbquery.h"
#include "dicom_uids.h"
#include "dicom_sq.h"
#include "dicom_print.h"
#include "icon.h"
#include "print_shared.h"



extern char *serverGroup;

void freeREsources();

static Widget wSelectPrinterDialog = 0;
Widget wFilmInformationForm = 0;
Widget wFilmSessionForm = 0;
Widget wFilmBoxForm = 0;
Widget wStudyListForm = 0;
Widget wPreviewStudyForm = 0;
Widget wPreviewPrintForm = 0;
Widget wBasicFilmBox_Standard = 0;
Widget wPrintMessage = 0;
Widget wMultipleSelection = 0;
Widget wImageAttribute = 0;


extern DUL_NETWORKKEY *networkKey;
extern DUL_ASSOCIATIONKEY *associationKey;
extern DUL_ASSOCIATESERVICEPARAMETERS associationParameters;


/*******************************************************************************
       The following header file defines the context structure.
*******************************************************************************/

#define CONTEXT_MACRO_ACCESS 1
#include "applicationShell1.h"
#undef CONTEXT_MACRO_ACCESS


/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

void
clearMessage()
{
}

void
setMessage(char *txt)
{
    XmTextSetString(wMessageTxt, txt);
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void
activateCB_menu1_p1_b1(
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
	CONDITION cond = DUL_NORMAL;


	if (associationKey != NULL)
	    cond = DUL_ReleaseAssociation(&associationKey);
	if (cond != DUL_RELEASECONFIRMED) {
	    cond = DUL_AbortAssociation(&associationKey);

	}
	(void) DUL_DropAssociation(&associationKey);



#ifdef RUNTIME
	if (wSelectPrinterDialog == 0)
	    wSelectPrinterDialog = (Widget) create_selectPrinterDialog(NULL);

	XtManageChild(wSelectPrinterDialog);

	loadPrintServerList(serverGroup);
#endif
    }
    UxApplicationShell1Context = UxSaveCtx;
}

static void
activateCB_menu1_p1_b2(
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
	freeResources();
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


    /* Creation of applicationShell1 */
    _UxParent = UxParent;
    if (_UxParent == NULL) {
	_UxParent = UxTopLevel;
    }
    applicationShell1 = XtVaCreatePopupShell("applicationShell1",
					     applicationShellWidgetClass,
					     _UxParent,
					     XmNwidth, 650,
					     XmNheight, 260,
					     XmNx, 0,
					     XmNy, 200,
					     NULL);
    UxPutContext(applicationShell1, (char *) UxApplicationShell1Context);


    /* Creation of mainWindow1 */
    mainWindow1 = XtVaCreateManagedWidget("mainWindow1",
					  xmMainWindowWidgetClass,
					  applicationShell1,
					  XmNwidth, 370,
					  XmNheight, 160,
					  XmNx, 130,
					  XmNy, 10,
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


    /* Creation of menu1_p1_b1 */
    menu1_p1_b1 = XtVaCreateManagedWidget("menu1_p1_b1",
					  xmPushButtonWidgetClass,
					  menu1_p1,
				     RES_CONVERT(XmNlabelString, "Connect"),
					  NULL);
    XtAddCallback(menu1_p1_b1, XmNactivateCallback,
		  (XtCallbackProc) activateCB_menu1_p1_b1,
		  (XtPointer) UxApplicationShell1Context);

    UxPutContext(menu1_p1_b1, (char *) UxApplicationShell1Context);


    /* Creation of menu1_p1_b2 */
    menu1_p1_b2 = XtVaCreateManagedWidget("menu1_p1_b2",
					  xmPushButtonWidgetClass,
					  menu1_p1,
					RES_CONVERT(XmNlabelString, "Quit"),
					  NULL);
    XtAddCallback(menu1_p1_b2, XmNactivateCallback,
		  (XtCallbackProc) activateCB_menu1_p1_b2,
		  (XtPointer) UxApplicationShell1Context);

    UxPutContext(menu1_p1_b2, (char *) UxApplicationShell1Context);


    /* Creation of menu1_top_b1 */
    menu1_top_b1 = XtVaCreateManagedWidget("menu1_top_b1",
					   xmCascadeButtonWidgetClass,
					   menu1,
				     RES_CONVERT(XmNlabelString, "Control"),
					   XmNsubMenuId, menu1_p1,
					   NULL);
    UxPutContext(menu1_top_b1, (char *) UxApplicationShell1Context);


    /* Creation of form1 */
    form1 = XtVaCreateManagedWidget("form1",
				    xmFormWidgetClass,
				    mainWindow1,
				    NULL);
    UxPutContext(form1, (char *) UxApplicationShell1Context);


    /* Creation of frame1 */
    frame1 = XtVaCreateManagedWidget("frame1",
				     xmFrameWidgetClass,
				     form1,
				     XmNwidth, 610,
				     XmNheight, 200,
				     XmNx, 20,
				     XmNy, 10,
				     NULL);
    UxPutContext(frame1, (char *) UxApplicationShell1Context);


    /* Creation of scrolledWindowText1 */
    scrolledWindowText1 = XtVaCreateManagedWidget("scrolledWindowText1",
						xmScrolledWindowWidgetClass,
						  frame1,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNx, 60,
						  XmNy, 50,
						  NULL);
    UxPutContext(scrolledWindowText1, (char *) UxApplicationShell1Context);


    /* Creation of wMessageTxt */
    wMessageTxt = XtVaCreateManagedWidget("wMessageTxt",
					  xmTextWidgetClass,
					  scrolledWindowText1,
					  XmNwidth, 210,
					  XmNheight, 40,
					  NULL);
    UxPutContext(wMessageTxt, (char *) UxApplicationShell1Context);


    XtAddCallback(applicationShell1, XmNdestroyCallback,
		  (XtCallbackProc) UxDestroyContextCB,
		  (XtPointer) UxApplicationShell1Context);

    XmMainWindowSetAreas(mainWindow1, menu1, (Widget) NULL,
			 (Widget) NULL, (Widget) NULL, form1);

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
