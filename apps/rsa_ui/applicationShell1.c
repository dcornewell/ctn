
/*******************************************************************************
	applicationShell1.c

       Associated Header file: applicationShell1.h
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/List.h>
#include <Xm/Text.h>
#include <Xm/ScrolledW.h>
#include <Xm/Frame.h>
#include <Xm/TextF.h>
#include <Xm/Separator.h>
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
** Module Name(s):	create_applicationShell1
**			loadPatientStudyList
** Author, Date:	Steve Moore, Summer 1994
** Intent:		This file contains the code that defines the
**			user interface for the rsa_ui application.  This
**			includes the code that creates the UI (by creating
**			widgets), the code that loads data into the
**			interface and the callback functions for user
**			events (like list selection and button presses).
**			The public interfaces create the widget
**			hierarchy and loads the scrolled list of
**			studies (for user selection from the event loop).
** Last Update:         $Author: smm $, $Date: 1995-12-21 20:45:43 $
** Source File:         $RCSfile: applicationShell1.c,v $
** Revision:            $Revision: 1.6 $
** Status:              $State: Exp $
*/

static char rcsid[] = "$Revision: 1.6 $ $RCSfile: applicationShell1.c,v $";


#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "dicom_objects.h"
#include "tbl.h"
#include "manage.h"
#include "fis.h"
#include "mut.h"

#include "rsa.h"
#include "format.h"

extern FIS_HANDLE *fisHandle;
extern DMAN_HANDLE *dmanHandle;
extern char applicationTitle[];
extern char *eventDestinations;
static LST_HEAD *patientList = NULL;
static LST_HEAD *patientStudyList = NULL;
static FIS_RESULTSRECORD results;
static FIS_INTERPRETATIONRECORD interpretation;


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
loadPatientStudyList()
{
    CONDITION cond;
    char b[256];
    RSA_PATIENTSTUDY *p;

    if (patientStudyList == NULL)
	patientStudyList = LST_Create();
    if (patientList == NULL)
	patientList = LST_Create();

    cond = FIS_Get(&fisHandle, FIS_K_PATIENT, FIS_K_PATIENT,
		   NULL, FIS_K_PAT_STUDYLIST, patientList);
    if (cond != FIS_NORMAL) {
	COND_DumpConditions();
	return;
    }
    expandPatientStudy(patientList, patientStudyList);
    MUT_LoadList(wPatientStudyList, patientStudyList, formatPatientStudy, b);
    p = LST_Head(&patientStudyList);
    if (p != NULL)
	(void) LST_Position(&patientStudyList, p);
}

static void
displayImpressionsInterpretation()
{
    CONDITION cond;
    RSA_PATIENTSTUDY *p;

    XmTextSetString(text1, "");
    p = LST_Current(&patientStudyList);
    if (p == NULL)
	return;

    memset(&results, 0, sizeof(results));
    results.Type = FIS_K_RESULTS;
    memset(&interpretation, 0, sizeof(interpretation));
    interpretation.Type = FIS_K_INTERPRETATION;

    cond = FIS_GetOne(&fisHandle, FIS_K_RESULTS, FIS_K_STUDY,
		      p->Study.StuInsUID, 0, &results);
    if (cond != FIS_NORMAL) {
	XmTextSetString(text1, "Failed to retrieve results record");
	COND_DumpConditions();
	return;
    }
    if (results.Flag & FIS_K_RESULTS_IMP)
	XmTextSetString(wImpressionsTxt, results.Imp);
    else
	XmTextSetString(wImpressionsTxt, "<None>");

    cond = FIS_GetOne(&fisHandle, FIS_K_INTERPRETATION, FIS_K_RESULTS,
		      results.ResUID, 0, &interpretation);
    if (cond != FIS_NORMAL) {
	XmTextSetString(text1, "Failed to retrieve interpretation record");
	COND_DumpConditions();
	return;
    }
    if (interpretation.Flag & FIS_K_INTERP_INTTEX)
	XmTextSetString(wReportTxt, interpretation.IntTex);
    else
	XmTextSetString(wReportTxt, "<None>");
}

static void
updateImpressions()
{
    CONDITION cond;
    FIS_RESULTSRECORD localResults;

    XmTextSetString(text1, "");
    memcpy(&localResults, &results, sizeof(results));
    localResults.Flag = FIS_K_RESULTS_IMP;
    cond = FIS_Update(&fisHandle, FIS_K_RESULTS, &localResults);
    if (cond != FIS_NORMAL) {
	(void) MUT_DisplayConditionText(text1);
	(void) COND_PopCondition(TRUE);
    }
}

static void
updateInterpretation()
{
    CONDITION cond;
    FIS_INTERPRETATIONRECORD localInterpretation;

    XmTextSetString(text1, "");
    memcpy(&localInterpretation, &interpretation, sizeof(interpretation));
    localInterpretation.Flag = FIS_K_INTERP_INTTEX;
    cond = FIS_Update(&fisHandle, FIS_K_INTERPRETATION, &localInterpretation);
    if (cond != FIS_NORMAL) {
	(void) MUT_DisplayConditionText(text1);
	(void) COND_PopCondition(TRUE);
    }
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
	exit(0);
    }
    UxApplicationShell1Context = UxSaveCtx;
}

static void
activateCB_wClearReportButton(
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
	XmTextSetString(wReportTxt, "");
	interpretation.IntTex[0] = '\0';
	updateInterpretation();
    }
    UxApplicationShell1Context = UxSaveCtx;
}

static void
activateCB_wUpdateReportButton(
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
	CONDITION cond;
	CTNBOOLEAN nullFlag;

	cond = MUT_ScanWidget(wReportTxt, MUT_TEXT, &nullFlag,
			      interpretation.IntTex);
	if (cond != MUT_NORMAL) {
	    MUT_DisplayConditionText(text1);
	    (void) COND_PopCondition(TRUE);
	    return;
	}
	if (nullFlag)
	    interpretation.IntTex[0] = '\0';
	updateInterpretation();
    }
    UxApplicationShell1Context = UxSaveCtx;
}

static void
activateCB_pushButton3(
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
	CONDITION cond;
	RSA_PATIENTSTUDY *p;

	XmTextSetString(text1, "");
	p = LST_Current(&patientStudyList);
	if (p == NULL) {
	    XmTextSetString(text1, "No current study selected");
	    return;
	}
	cond = distributeEvent(&fisHandle, &dmanHandle, FIS_K_STUDY,
			 &p->Study, FIS_K_STUDY_SCHEDULED, applicationTitle,
			       eventDestinations);
	if (cond != 1) {
	    (void) MUT_DisplayConditionText(text1);
	    (void) COND_PopCondition(TRUE);
	} else {
	    XmTextSetString(text1, "Study Scheduled Event delivered");
	}
    }
    UxApplicationShell1Context = UxSaveCtx;
}

static void
activateCB_pushButton4(
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
	CONDITION cond;
	RSA_PATIENTSTUDY *p;

	XmTextSetString(text1, "");
	p = LST_Current(&patientStudyList);
	if (p == NULL) {
	    XmTextSetString(text1, "No current study selected");
	    return;
	}
    }
    UxApplicationShell1Context = UxSaveCtx;
}

static void
activateCB_pushButton5(
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
	CONDITION cond;
	RSA_PATIENTSTUDY *p;

	XmTextSetString(text1, "");
	p = LST_Current(&patientStudyList);
	if (p == NULL) {
	    XmTextSetString(text1, "No current study selected");
	    return;
	}
	UTL_GetDicomDate(p->Study.StuReaDat);
	UTL_GetDicomTime(p->Study.StuReaTim);
	p->Study.Flag |= FIS_K_STU_STUREADAT | FIS_K_STU_STUREATIM;

	cond = distributeEvent(&fisHandle, &dmanHandle, FIS_K_STUDY,
			       &p->Study, FIS_K_STUDY_READ, applicationTitle,
			       eventDestinations);
	if (cond != 1) {
	    (void) MUT_DisplayConditionText(text1);
	    (void) COND_PopCondition(TRUE);
	} else {
	    XmTextSetString(text1, "Study Read Event delivered");
	}

    }
    UxApplicationShell1Context = UxSaveCtx;
}

static void
browseSelectionCB_wPatientStudyList(
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
	XmListCallbackStruct *listStruct;
	RSA_PATIENTSTUDY *p;
	char buf[256];

	listStruct = (XmListCallbackStruct *) UxCallbackArg;
	p = LST_Index(&patientStudyList, listStruct->item_position);
	formatPatientStudy(p, 0, buf);
	XmTextSetString(wPatientStudyTxt, buf);

	displayImpressionsInterpretation(p);
	XtSetSensitive(wClearImpressionsButton, TRUE);
	XtSetSensitive(wUpdateImpressionsButton, TRUE);
	XtSetSensitive(wClearReportButton, TRUE);
	XtSetSensitive(wUpdateReportButton, TRUE);
    }
    UxApplicationShell1Context = UxSaveCtx;
}

static void
activateCB_wClearImpressionsButton(
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
	XmTextSetString(wImpressionsTxt, "");
	results.Imp[0] = '\0';
	updateImpressions();
    }
    UxApplicationShell1Context = UxSaveCtx;
}

static void
activateCB_wUpdateImpressionsButton(
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
	CONDITION cond;
	CTNBOOLEAN nullFlag;

	cond = MUT_ScanWidget(wImpressionsTxt, MUT_TEXT, &nullFlag, results.Imp);
	if (cond != MUT_NORMAL) {
	    MUT_DisplayConditionText(text1);
	    (void) COND_PopCondition(TRUE);
	    return;
	}
	if (nullFlag)
	    results.Imp[0] = '\0';
	updateImpressions();
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
					     XmNwidth, 900,
					     XmNheight, 780,
					     XmNx, 67,
					     XmNy, 92,
					     NULL);
    UxPutContext(applicationShell1, (char *) UxApplicationShell1Context);


    /* Creation of mainWindow1 */
    mainWindow1 = XtVaCreateManagedWidget("mainWindow1",
					  xmMainWindowWidgetClass,
					  applicationShell1,
					  XmNwidth, 900,
					  XmNheight, 780,
					  XmNx, 0,
					  XmNy, 0,
					  XmNunitType, XmPIXELS,
					  NULL);
    UxPutContext(mainWindow1, (char *) UxApplicationShell1Context);


    /* Creation of menu1 */
    menu1 = XtVaCreateManagedWidget("menu1",
				    xmRowColumnWidgetClass,
				    mainWindow1,
				    XmNrowColumnType, XmMENU_BAR,
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
					RES_CONVERT(XmNlabelString, "Quit"),
					  NULL);
    XtAddCallback(menu1_p1_b1, XmNactivateCallback,
		  (XtCallbackProc) activateCB_menu1_p1_b1,
		  (XtPointer) UxApplicationShell1Context);

    UxPutContext(menu1_p1_b1, (char *) UxApplicationShell1Context);


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


    /* Creation of wClearReportButton */
    wClearReportButton = XtVaCreateManagedWidget("wClearReportButton",
						 xmPushButtonWidgetClass,
						 form1,
						 XmNx, 770,
						 XmNy, 370,
						 XmNwidth, 130,
						 XmNheight, 40,
				RES_CONVERT(XmNlabelString, "Clear Report"),
						 XmNsensitive, FALSE,
						 NULL);
    XtAddCallback(wClearReportButton, XmNactivateCallback,
		  (XtCallbackProc) activateCB_wClearReportButton,
		  (XtPointer) UxApplicationShell1Context);

    UxPutContext(wClearReportButton, (char *) UxApplicationShell1Context);


    /* Creation of wUpdateReportButton */
    wUpdateReportButton = XtVaCreateManagedWidget("wUpdateReportButton",
						  xmPushButtonWidgetClass,
						  form1,
						  XmNx, 770,
						  XmNy, 460,
						  XmNwidth, 130,
						  XmNheight, 40,
			       RES_CONVERT(XmNlabelString, "Update Report"),
						  XmNsensitive, FALSE,
						  NULL);
    XtAddCallback(wUpdateReportButton, XmNactivateCallback,
		  (XtCallbackProc) activateCB_wUpdateReportButton,
		  (XtPointer) UxApplicationShell1Context);

    UxPutContext(wUpdateReportButton, (char *) UxApplicationShell1Context);


    /* Creation of pushButton3 */
    pushButton3 = XtVaCreateManagedWidget("pushButton3",
					  xmPushButtonWidgetClass,
					  form1,
					  XmNx, 80,
					  XmNy, 590,
					  XmNwidth, 120,
					  XmNheight, 40,
			     RES_CONVERT(XmNlabelString, "Study Scheduled"),
					  NULL);
    XtAddCallback(pushButton3, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton3,
		  (XtPointer) UxApplicationShell1Context);

    UxPutContext(pushButton3, (char *) UxApplicationShell1Context);


    /* Creation of pushButton4 */
    pushButton4 = XtVaCreateManagedWidget("pushButton4",
					  xmPushButtonWidgetClass,
					  form1,
					  XmNx, 360,
					  XmNy, 590,
					  XmNwidth, 120,
					  XmNheight, 40,
			     RES_CONVERT(XmNlabelString, "Study Completed"),
					  XmNsensitive, FALSE,
					  NULL);
    XtAddCallback(pushButton4, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton4,
		  (XtPointer) UxApplicationShell1Context);

    UxPutContext(pushButton4, (char *) UxApplicationShell1Context);


    /* Creation of pushButton5 */
    pushButton5 = XtVaCreateManagedWidget("pushButton5",
					  xmPushButtonWidgetClass,
					  form1,
					  XmNx, 600,
					  XmNy, 590,
					  XmNwidth, 120,
					  XmNheight, 40,
				  RES_CONVERT(XmNlabelString, "Study Read"),
					  NULL);
    XtAddCallback(pushButton5, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton5,
		  (XtPointer) UxApplicationShell1Context);

    UxPutContext(pushButton5, (char *) UxApplicationShell1Context);


    /* Creation of separator1 */
    separator1 = XtVaCreateManagedWidget("separator1",
					 xmSeparatorWidgetClass,
					 form1,
					 XmNwidth, 840,
					 XmNheight, 10,
					 XmNx, 20,
					 XmNy, 560,
					 NULL);
    UxPutContext(separator1, (char *) UxApplicationShell1Context);


    /* Creation of wPatientStudyTxt */
    wPatientStudyTxt = XtVaCreateManagedWidget("wPatientStudyTxt",
					       xmTextFieldWidgetClass,
					       form1,
					       XmNwidth, 790,
					       XmNx, 10,
					       XmNy, 0,
					       XmNheight, 40,
					       XmNeditable, FALSE,
			     XmNfontList, UxConvertFontList("courier-bold"),
					       NULL);
    UxPutContext(wPatientStudyTxt, (char *) UxApplicationShell1Context);


    /* Creation of frame1 */
    frame1 = XtVaCreateManagedWidget("frame1",
				     xmFrameWidgetClass,
				     form1,
				     XmNwidth, 740,
				     XmNheight, 200,
				     XmNx, 20,
				     XmNy, 340,
				     NULL);
    UxPutContext(frame1, (char *) UxApplicationShell1Context);


    /* Creation of scrolledWindowText1 */
    scrolledWindowText1 = XtVaCreateManagedWidget("scrolledWindowText1",
						xmScrolledWindowWidgetClass,
						  frame1,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNx, 200,
						  XmNy, 60,
						  NULL);
    UxPutContext(scrolledWindowText1, (char *) UxApplicationShell1Context);


    /* Creation of wReportTxt */
    wReportTxt = XtVaCreateManagedWidget("wReportTxt",
					 xmTextWidgetClass,
					 scrolledWindowText1,
					 XmNwidth, 260,
					 XmNheight, 50,
					 XmNeditMode, XmMULTI_LINE_EDIT,
					 XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
					 NULL);
    UxPutContext(wReportTxt, (char *) UxApplicationShell1Context);


    /* Creation of frame2 */
    frame2 = XtVaCreateManagedWidget("frame2",
				     xmFrameWidgetClass,
				     form1,
				     XmNwidth, 790,
				     XmNheight, 160,
				     XmNx, 10,
				     XmNy, 50,
				     XmNresizable, FALSE,
				     NULL);
    UxPutContext(frame2, (char *) UxApplicationShell1Context);


    /* Creation of scrolledWindowList1 */
    scrolledWindowList1 = XtVaCreateManagedWidget("scrolledWindowList1",
						xmScrolledWindowWidgetClass,
						  frame2,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNshadowThickness, 0,
						  XmNx, 330,
						  XmNy, 0,
						  NULL);
    UxPutContext(scrolledWindowList1, (char *) UxApplicationShell1Context);


    /* Creation of wPatientStudyList */
    wPatientStudyList = XtVaCreateManagedWidget("wPatientStudyList",
						xmListWidgetClass,
						scrolledWindowList1,
						XmNwidth, 90,
						XmNheight, 156,
			     XmNfontList, UxConvertFontList("courier-bold"),
						NULL);
    XtAddCallback(wPatientStudyList, XmNbrowseSelectionCallback,
		  (XtCallbackProc) browseSelectionCB_wPatientStudyList,
		  (XtPointer) UxApplicationShell1Context);

    UxPutContext(wPatientStudyList, (char *) UxApplicationShell1Context);


    /* Creation of frame3 */
    frame3 = XtVaCreateManagedWidget("frame3",
				     xmFrameWidgetClass,
				     form1,
				     XmNwidth, 740,
				     XmNheight, 90,
				     XmNx, 20,
				     XmNy, 230,
				     NULL);
    UxPutContext(frame3, (char *) UxApplicationShell1Context);


    /* Creation of scrolledWindowText2 */
    scrolledWindowText2 = XtVaCreateManagedWidget("scrolledWindowText2",
						xmScrolledWindowWidgetClass,
						  frame3,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNx, 200,
						  XmNy, -10,
						  XmNheight, 156,
						  NULL);
    UxPutContext(scrolledWindowText2, (char *) UxApplicationShell1Context);


    /* Creation of wImpressionsTxt */
    wImpressionsTxt = XtVaCreateManagedWidget("wImpressionsTxt",
					      xmTextWidgetClass,
					      scrolledWindowText2,
					      XmNwidth, 430,
					      XmNheight, 137,
					      XmNeditMode, XmMULTI_LINE_EDIT,
					      XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
					      NULL);
    UxPutContext(wImpressionsTxt, (char *) UxApplicationShell1Context);


    /* Creation of wClearImpressionsButton */
    wClearImpressionsButton = XtVaCreateManagedWidget("wClearImpressionsButton",
						    xmPushButtonWidgetClass,
						      form1,
						      XmNx, 770,
						      XmNy, 230,
						      XmNwidth, 130,
						      XmNheight, 40,
			   RES_CONVERT(XmNlabelString, "Clear Impressions"),
						      XmNsensitive, FALSE,
						      NULL);
    XtAddCallback(wClearImpressionsButton, XmNactivateCallback,
		  (XtCallbackProc) activateCB_wClearImpressionsButton,
		  (XtPointer) UxApplicationShell1Context);

    UxPutContext(wClearImpressionsButton, (char *) UxApplicationShell1Context);


    /* Creation of wUpdateImpressionsButton */
    wUpdateImpressionsButton = XtVaCreateManagedWidget("wUpdateImpressionsButton",
						    xmPushButtonWidgetClass,
						       form1,
						       XmNx, 770,
						       XmNy, 280,
						       XmNwidth, 130,
						       XmNheight, 40,
			  RES_CONVERT(XmNlabelString, "Update Impressions"),
						       XmNsensitive, FALSE,
						       NULL);
    XtAddCallback(wUpdateImpressionsButton, XmNactivateCallback,
		  (XtCallbackProc) activateCB_wUpdateImpressionsButton,
		  (XtPointer) UxApplicationShell1Context);

    UxPutContext(wUpdateImpressionsButton, (char *) UxApplicationShell1Context);


    /* Creation of frame4 */
    frame4 = XtVaCreateManagedWidget("frame4",
				     xmFrameWidgetClass,
				     form1,
				     XmNwidth, 870,
				     XmNheight, 90,
				     XmNx, 6,
				     XmNy, 646,
				     XmNresizable, FALSE,
				     NULL);
    UxPutContext(frame4, (char *) UxApplicationShell1Context);


    /* Creation of scrolledWindowText3 */
    scrolledWindowText3 = XtVaCreateManagedWidget("scrolledWindowText3",
						xmScrolledWindowWidgetClass,
						  frame4,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNx, 430,
						  XmNy, 10,
						  NULL);
    UxPutContext(scrolledWindowText3, (char *) UxApplicationShell1Context);


    /* Creation of text1 */
    text1 = XtVaCreateManagedWidget("text1",
				    xmTextWidgetClass,
				    scrolledWindowText3,
				    XmNwidth, 140,
				    XmNheight, 60,
				    XmNeditable, FALSE,
				    XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
				    NULL);
    UxPutContext(text1, (char *) UxApplicationShell1Context);


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
