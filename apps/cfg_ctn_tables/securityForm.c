
/*******************************************************************************
	securityForm.c

       Associated Header file: securityForm.h
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <X11/Shell.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/Frame.h>
#include <Xm/PushB.h>
#include <Xm/TextF.h>
#include <Xm/Label.h>
#include <Xm/Form.h>

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
** Module Name(s):	create_securityForm
**			loadSecurityMatrixList
** Author, Date:	Steve Moore, Summer 1994
** Intent:		This file provides the user interface
**                      for defining and modifying entries in
**			the security matrix table.
**			The public entry points are
**			call to create the widget hierarchy
**			and to populate the list application
**			entities.  All other functions are
**			local to this specific interface.
** Last Update:		$Author: smm $, $Date: 1995-04-10 13:20:03 $
** Source File:		$RCSfile: securityForm.c,v $
** Revision:		$Revision: 1.6 $
** Status:		$State: Exp $
*/


#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "tbl.h"
#include "manage.h"
#include "mut.h"

#include "format.h"

extern DMAN_HANDLE *dmanHandle;
static LST_HEAD *securityMatrixList = NULL;

void loadSecurityMatrixList();
static void displaySecurityMatrix(DMAN_SECURITYMATRIX * a);
static CONDITION readSecurityMatrix(DMAN_SECURITYMATRIX * a);


/*******************************************************************************
       The following header file defines the context structure.
*******************************************************************************/

#define CONTEXT_MACRO_ACCESS 1
#include "securityForm.h"
#undef CONTEXT_MACRO_ACCESS


/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

void
loadSecurityMatrixList()
{
    CONDITION
    cond;
    DMAN_SECURITYMATRIX
	a,
	criteria;
    char
        b[256];

    if (securityMatrixList == NULL)
	securityMatrixList = LST_Create();

    if (securityMatrixList == NULL)
	return;

    memset(&a, 0, sizeof(a));
    a.Type = DMAN_K_SECURITYMATRIX;
    criteria.Type = DMAN_K_SECURITYMATRIX;
    criteria.Flag = 0;
    cond = DMAN_Select(&dmanHandle,
		       (DMAN_GENERICRECORD *) & a,
		       (DMAN_GENERICRECORD *) & criteria,
		       securityMatrixList, NULL, NULL, NULL);
    if (cond != DMAN_NORMAL) {
	COND_DumpConditions();
	return;
    }
    (void) LST_Sort(&securityMatrixList, sizeof(a),
		    compareSecurityMatrix);
    MUT_LoadList(wSecurityMatrixList, securityMatrixList,
		 formatSecurityMatrix, b);
}

static void
displaySecurityMatrix(DMAN_SECURITYMATRIX * a)
{
    XmTextSetString(wRequestingApplicationTxt, a->RequestingTitle);
    XmTextSetString(wRespondingApplicationTxt, a->RespondingTitle);
}

static CONDITION
readSecurityMatrix(DMAN_SECURITYMATRIX * a)
{
    CONDITION
    cond;
    CTNBOOLEAN
	nullFlag;

    memset(a, 0, sizeof(*a));
    a->Type = DMAN_K_SECURITYMATRIX;

    cond = MUT_ScanWidget(wRequestingApplicationTxt, MUT_TEXT, &nullFlag,
			  a->RequestingTitle);
    if (cond != MUT_NORMAL || nullFlag)
	return 0;
    a->Flag |= DMAN_K_SECURITY_REQUESTING;

    cond = MUT_ScanWidget(wRespondingApplicationTxt, MUT_TEXT, &nullFlag,
			  a->RespondingTitle);
    if (cond != MUT_NORMAL || nullFlag)
	return 0;
    a->Flag |= DMAN_K_SECURITY_RESPONDING;

    return 1;
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void
activateCB_pushButton4(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    _UxCsecurityForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxSecurityFormContext;
    UxSecurityFormContext = UxContext =
	(_UxCsecurityForm *) UxGetContext(UxWidget);
    {
	DMAN_SECURITYMATRIX
	    a;
	CONDITION
	    cond;

	cond = readSecurityMatrix(&a);
	if (cond != 1)
	    return;

	cond = DMAN_Insert(&dmanHandle, (DMAN_GENERICRECORD *) & a);
	if (cond != DMAN_NORMAL)
	    COND_DumpConditions();

	loadSecurityMatrixList();
    }
    UxSecurityFormContext = UxSaveCtx;
}

static void
activateCB_pushButton5(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    _UxCsecurityForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxSecurityFormContext;
    UxSecurityFormContext = UxContext =
	(_UxCsecurityForm *) UxGetContext(UxWidget);
    {
	/*
	 * DMAN_APPLICATIONENTITY ae; CONDITION cond;
	 * 
	 * cond = readAE(&ae); if (cond != 1) return;
	 * 
	 * cond = DMAN_Insert(&dmanHandle, (DMAN_GENERICRECORD *)&ae); if (cond
	 * != DMAN_NORMAL) COND_DumpConditions();
	 * 
	 * loadApplicationList();
	 */
    }
    UxSecurityFormContext = UxSaveCtx;
}

static void
activateCB_pushButton6(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    _UxCsecurityForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxSecurityFormContext;
    UxSecurityFormContext = UxContext =
	(_UxCsecurityForm *) UxGetContext(UxWidget);
    {
	DMAN_SECURITYMATRIX
	    a;
	CONDITION
	    cond;

	cond = readSecurityMatrix(&a);
	if (cond != 1)
	    return;

	cond = DMAN_Delete(&dmanHandle, (DMAN_GENERICRECORD *) & a);
	if (cond != DMAN_NORMAL)
	    COND_DumpConditions();

	loadSecurityMatrixList();
    }
    UxSecurityFormContext = UxSaveCtx;
}

static void
browseSelectionCB_wSecurityMatrixList(
				      Widget wgt,
				      XtPointer cd,
				      XtPointer cb)
{
    _UxCsecurityForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxSecurityFormContext;
    UxSecurityFormContext = UxContext =
	(_UxCsecurityForm *) UxGetContext(UxWidget);
    {
	XmListCallbackStruct *l;
	DMAN_SECURITYMATRIX
	    * a;
	int i;

	l = (XmListCallbackStruct *) UxCallbackArg;

	a = LST_Head(&securityMatrixList);
	(void) LST_Position(&securityMatrixList, a);
	for (i = 2; i <= l->item_position; i++)
	    a = LST_Next(&securityMatrixList);

	displaySecurityMatrix(a);
    }
    UxSecurityFormContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget
_Uxbuild_securityForm()
{
    Widget _UxParent;


    /* Creation of securityForm */
    _UxParent = UxParent;
    if (_UxParent == NULL) {
	_UxParent = XtVaCreatePopupShell("securityForm_shell",
				       topLevelShellWidgetClass, UxTopLevel,
					 XmNx, 494,
					 XmNy, 120,
					 XmNwidth, 690,
					 XmNheight, 460,
					 XmNshellUnitType, XmPIXELS,
					 XmNtitle, "securityForm",
					 XmNiconName, "securityForm",
					 NULL);

    }
    securityForm = XtVaCreateManagedWidget("securityForm",
					   xmFormWidgetClass,
					   _UxParent,
					   XmNwidth, 690,
					   XmNheight, 460,
					   XmNresizePolicy, XmRESIZE_NONE,
					   XmNunitType, XmPIXELS,
					   NULL);
    UxPutContext(securityForm, (char *) UxSecurityFormContext);


    /* Creation of label5 */
    label5 = XtVaCreateManagedWidget("label5",
				     xmLabelWidgetClass,
				     securityForm,
				     XmNx, 50,
				     XmNy, 200,
				     XmNwidth, 200,
				     XmNheight, 40,
	     RES_CONVERT(XmNlabelString, "Requesting Application (Vendor)"),
				     NULL);
    UxPutContext(label5, (char *) UxSecurityFormContext);


    /* Creation of label6 */
    label6 = XtVaCreateManagedWidget("label6",
				     xmLabelWidgetClass,
				     securityForm,
				     XmNx, 50,
				     XmNy, 240,
				     XmNwidth, 200,
				     XmNheight, 40,
		RES_CONVERT(XmNlabelString, "Responding Application (CTN)"),
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				     XmNleftOffset, 0,
				     XmNleftWidget, label5,
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 15,
				     XmNtopWidget, label5,
				     NULL);
    UxPutContext(label6, (char *) UxSecurityFormContext);


    /* Creation of wRequestingApplicationTxt */
    wRequestingApplicationTxt = XtVaCreateManagedWidget("wRequestingApplicationTxt",
						     xmTextFieldWidgetClass,
							securityForm,
							XmNwidth, 370,
							XmNx, 280,
							XmNy, 210,
							XmNheight, 40,
					 XmNleftAttachment, XmATTACH_WIDGET,
							XmNleftOffset, 15,
						      XmNleftWidget, label5,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
							XmNtopOffset, 0,
							XmNtopWidget, label5,
							NULL);
    UxPutContext(wRequestingApplicationTxt, (char *) UxSecurityFormContext);


    /* Creation of wRespondingApplicationTxt */
    wRespondingApplicationTxt = XtVaCreateManagedWidget("wRespondingApplicationTxt",
						     xmTextFieldWidgetClass,
							securityForm,
							XmNwidth, 370,
							XmNx, 280,
							XmNy, 250,
							XmNheight, 40,
					 XmNleftAttachment, XmATTACH_WIDGET,
							XmNleftOffset, 15,
						      XmNleftWidget, label5,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
							XmNtopOffset, 0,
							XmNtopWidget, label6,
							NULL);
    UxPutContext(wRespondingApplicationTxt, (char *) UxSecurityFormContext);


    /* Creation of pushButton4 */
    pushButton4 = XtVaCreateManagedWidget("pushButton4",
					  xmPushButtonWidgetClass,
					  securityForm,
					  XmNx, 50,
					  XmNy, 350,
					  XmNwidth, 160,
					  XmNheight, 50,
					  RES_CONVERT(XmNlabelString, "Add"),
					  NULL);
    XtAddCallback(pushButton4, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton4,
		  (XtPointer) UxSecurityFormContext);

    UxPutContext(pushButton4, (char *) UxSecurityFormContext);


    /* Creation of pushButton5 */
    pushButton5 = XtVaCreateManagedWidget("pushButton5",
					  xmPushButtonWidgetClass,
					  securityForm,
					  XmNx, 270,
					  XmNy, 350,
					  XmNwidth, 160,
					  XmNheight, 50,
				      RES_CONVERT(XmNlabelString, "Update"),
					  XmNsensitive, FALSE,
					  NULL);
    XtAddCallback(pushButton5, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton5,
		  (XtPointer) UxSecurityFormContext);

    UxPutContext(pushButton5, (char *) UxSecurityFormContext);


    /* Creation of pushButton6 */
    pushButton6 = XtVaCreateManagedWidget("pushButton6",
					  xmPushButtonWidgetClass,
					  securityForm,
					  XmNx, 500,
					  XmNy, 350,
					  XmNwidth, 160,
					  XmNheight, 50,
				      RES_CONVERT(XmNlabelString, "Delete"),
					  NULL);
    XtAddCallback(pushButton6, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton6,
		  (XtPointer) UxSecurityFormContext);

    UxPutContext(pushButton6, (char *) UxSecurityFormContext);


    /* Creation of frame2 */
    frame2 = XtVaCreateManagedWidget("frame2",
				     xmFrameWidgetClass,
				     securityForm,
				     XmNwidth, 610,
				     XmNheight, 150,
				     XmNx, 40,
				     XmNy, 20,
				     XmNresizable, FALSE,
				     NULL);
    UxPutContext(frame2, (char *) UxSecurityFormContext);


    /* Creation of scrolledWindowList2 */
    scrolledWindowList2 = XtVaCreateManagedWidget("scrolledWindowList2",
						xmScrolledWindowWidgetClass,
						  frame2,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNshadowThickness, 0,
						  XmNx, 110,
						  XmNy, 60,
						  NULL);
    UxPutContext(scrolledWindowList2, (char *) UxSecurityFormContext);


    /* Creation of wSecurityMatrixList */
    wSecurityMatrixList = XtVaCreateManagedWidget("wSecurityMatrixList",
						  xmListWidgetClass,
						  scrolledWindowList2,
						  XmNwidth, 170,
						  XmNheight, 40,
						  NULL);
    XtAddCallback(wSecurityMatrixList, XmNbrowseSelectionCallback,
		  (XtCallbackProc) browseSelectionCB_wSecurityMatrixList,
		  (XtPointer) UxSecurityFormContext);

    UxPutContext(wSecurityMatrixList, (char *) UxSecurityFormContext);


    XtAddCallback(securityForm, XmNdestroyCallback,
		  (XtCallbackProc) UxDestroyContextCB,
		  (XtPointer) UxSecurityFormContext);


    return (securityForm);
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget
create_securityForm(swidget _UxUxParent)
{
    Widget rtrn;
    _UxCsecurityForm *UxContext;

    UxSecurityFormContext = UxContext =
	(_UxCsecurityForm *) UxNewContext(sizeof(_UxCsecurityForm), False);

    UxParent = _UxUxParent;

    rtrn = _Uxbuild_securityForm();

    return (rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/
