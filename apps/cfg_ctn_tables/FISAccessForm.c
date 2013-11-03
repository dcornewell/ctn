
/*******************************************************************************
	FISAccessForm.c

       Associated Header file: FISAccessForm.h
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
** Module Name(s):      create_FISAccessForm
**                      loadFISAccessList
** Author, Date:        Steve Moore, Summer 1994
** Intent:              This file provides the user interface
**                      for defining and modifying entries in
**                      the FIS Access table.
**                      The public entry points are
**                      call to create the widget hierarchy
**                      and to populate the list application
**                      entities.  All other functions are
**                      local to this specific interface.
** Last Update:         $Author: smm $, $Date: 1995-04-10 13:19:01 $
** Source File:         $RCSfile: FISAccessForm.c,v $
** Revision:            $Revision: 1.3 $
** Status:              $State: Exp $
*/



#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "tbl.h"
#include "manage.h"
#include "mut.h"

#include "format.h"

extern DMAN_HANDLE *dmanHandle;
static LST_HEAD *FISAccessList = NULL;

void loadFISAccessList();
static void displayFISAccess(DMAN_FISACCESS * f);
static CONDITION readFISAccess(DMAN_FISACCESS * f);


/*******************************************************************************
       The following header file defines the context structure.
*******************************************************************************/

#define CONTEXT_MACRO_ACCESS 1
#include "FISAccessForm.h"
#undef CONTEXT_MACRO_ACCESS


/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

void
loadFISAccessList()
{
    CONDITION cond;
    DMAN_FISACCESS f,
        criteria;
    char b[256];

    if (FISAccessList == NULL)
	FISAccessList = LST_Create();
    if (FISAccessList == NULL)
	return;

    memset(&f, 0, sizeof(f));
    f.Type = DMAN_K_FISACCESS;
    criteria.Type = DMAN_K_FISACCESS;
    criteria.Flag = 0;
    cond = DMAN_Select(&dmanHandle,
		       (DMAN_GENERICRECORD *) & f,
		       (DMAN_GENERICRECORD *) & criteria,
		       FISAccessList, NULL, NULL, NULL);
    if (cond != DMAN_NORMAL)
	return;

    MUT_LoadList(wFISAccessList, FISAccessList,
		 formatFISAccess, b);
}

static void
displayFISAccess(DMAN_FISACCESS * f)
{
    if (f == NULL)
	return;

    XmTextSetString(wTitleTxt, f->Title);
    XmTextSetString(wDatabaseKeyTxt, f->DbKey);
    XmTextSetString(wGroupTxt, f->GroupName);
    XmTextSetString(wCommentTxt, f->Comment);
}

static CONDITION
readFISAccess(DMAN_FISACCESS * f)
{
    CONDITION cond;
    CTNBOOLEAN nullFlag;

    memset(f, 0, sizeof(*f));
    f->Type = DMAN_K_FISACCESS;

    cond = MUT_ScanWidget(wTitleTxt, MUT_TEXT, &nullFlag,
			  f->Title);
    if (cond != MUT_NORMAL || nullFlag)
	return 0;
    f->Flag |= DMAN_K_FISACCESS_TITLE;

    cond = MUT_ScanWidget(wDatabaseKeyTxt, MUT_TEXT, &nullFlag,
			  f->DbKey);
    if (cond != MUT_NORMAL || nullFlag)
	return 0;
    f->Flag |= DMAN_K_FISACCESS_DBKEY;

    cond = MUT_ScanWidget(wGroupTxt, MUT_TEXT, &nullFlag,
			  f->GroupName);
    if (cond != MUT_NORMAL || nullFlag)
	return 0;
    f->Flag |= DMAN_K_FISACCESS_GROUPNAME;

    cond = MUT_ScanWidget(wCommentTxt, MUT_TEXT, &nullFlag,
			  f->Comment);
    if (cond != MUT_NORMAL || nullFlag)
	return 0;
    f->Flag |= DMAN_K_FISACCESS_COMMENT;

    return 1;
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void
activateCB_pushButton12(
			Widget wgt,
			XtPointer cd,
			XtPointer cb)
{
    _UxCFISAccessForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxFISAccessFormContext;
    UxFISAccessFormContext = UxContext =
	(_UxCFISAccessForm *) UxGetContext(UxWidget);
    {
	DMAN_FISACCESS f;
	CONDITION cond;

	cond = readFISAccess(&f);
	if (cond != 1) {
	    COND_DumpConditions();
	    return;
	}
	cond = DMAN_Insert(&dmanHandle, (DMAN_GENERICRECORD *) & f);
	if (cond != DMAN_NORMAL)
	    COND_DumpConditions();

	loadFISAccessList();
    }
    UxFISAccessFormContext = UxSaveCtx;
}

static void
activateCB_pushButton14(
			Widget wgt,
			XtPointer cd,
			XtPointer cb)
{
    _UxCFISAccessForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxFISAccessFormContext;
    UxFISAccessFormContext = UxContext =
	(_UxCFISAccessForm *) UxGetContext(UxWidget);
    {
	DMAN_FISACCESS f;
	CONDITION cond;

	cond = readFISAccess(&f);
	if (cond != 1) {
	    COND_DumpConditions();
	    return;
	}
	cond = DMAN_Delete(&dmanHandle, (DMAN_GENERICRECORD *) & f);
	if (cond != DMAN_NORMAL)
	    COND_DumpConditions();

	loadFISAccessList();
    }
    UxFISAccessFormContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget
_Uxbuild_FISAccessForm()
{
    Widget _UxParent;


    /* Creation of FISAccessForm */
    _UxParent = UxParent;
    if (_UxParent == NULL) {
	_UxParent = XtVaCreatePopupShell("FISAccessForm_shell",
				       topLevelShellWidgetClass, UxTopLevel,
					 XmNx, 180,
					 XmNy, 306,
					 XmNwidth, 655,
					 XmNheight, 480,
					 XmNshellUnitType, XmPIXELS,
					 XmNtitle, "FISAccessForm",
					 XmNiconName, "FISAccessForm",
					 NULL);

    }
    FISAccessForm = XtVaCreateManagedWidget("FISAccessForm",
					    xmFormWidgetClass,
					    _UxParent,
					    XmNwidth, 655,
					    XmNheight, 480,
					    XmNresizePolicy, XmRESIZE_NONE,
					    XmNunitType, XmPIXELS,
					    NULL);
    UxPutContext(FISAccessForm, (char *) UxFISAccessFormContext);


    /* Creation of label12 */
    label12 = XtVaCreateManagedWidget("label12",
				      xmLabelWidgetClass,
				      FISAccessForm,
				      XmNx, 40,
				      XmNy, 160,
				      XmNwidth, 170,
				      XmNheight, 40,
				      RES_CONVERT(XmNlabelString, "Title"),
				      NULL);
    UxPutContext(label12, (char *) UxFISAccessFormContext);


    /* Creation of label13 */
    label13 = XtVaCreateManagedWidget("label13",
				      xmLabelWidgetClass,
				      FISAccessForm,
				      XmNx, 36,
				      XmNy, 186,
				      XmNwidth, 170,
				      XmNheight, 40,
				RES_CONVERT(XmNlabelString, "Database Key"),
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				      XmNleftOffset, 0,
				      XmNleftWidget, label12,
				      XmNtopAttachment, XmATTACH_WIDGET,
				      XmNtopOffset, 15,
				      XmNtopWidget, label12,
				      NULL);
    UxPutContext(label13, (char *) UxFISAccessFormContext);


    /* Creation of label14 */
    label14 = XtVaCreateManagedWidget("label14",
				      xmLabelWidgetClass,
				      FISAccessForm,
				      XmNx, 23,
				      XmNy, 306,
				      XmNwidth, 170,
				      XmNheight, 40,
				      RES_CONVERT(XmNlabelString, "Group"),
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				      XmNleftOffset, 0,
				      XmNleftWidget, label12,
				      XmNtopAttachment, XmATTACH_WIDGET,
				      XmNtopOffset, 15,
				      XmNtopWidget, label13,
				      NULL);
    UxPutContext(label14, (char *) UxFISAccessFormContext);


    /* Creation of label15 */
    label15 = XtVaCreateManagedWidget("label15",
				      xmLabelWidgetClass,
				      FISAccessForm,
				      XmNx, 23,
				      XmNy, 361,
				      XmNwidth, 170,
				      XmNheight, 40,
				      RES_CONVERT(XmNlabelString, "Comment"),
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				      XmNleftOffset, 0,
				      XmNleftWidget, label12,
				      XmNtopAttachment, XmATTACH_WIDGET,
				      XmNtopOffset, 15,
				      XmNtopWidget, label14,
				      NULL);
    UxPutContext(label15, (char *) UxFISAccessFormContext);


    /* Creation of wGroupTxt */
    wGroupTxt = XtVaCreateManagedWidget("wGroupTxt",
					xmTextFieldWidgetClass,
					FISAccessForm,
					XmNwidth, 375,
					XmNx, 213,
					XmNy, 306,
					XmNheight, 40,
					XmNleftAttachment, XmATTACH_WIDGET,
					XmNleftOffset, 15,
					XmNleftWidget, label12,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					XmNtopOffset, 0,
					XmNtopWidget, label14,
					NULL);
    UxPutContext(wGroupTxt, (char *) UxFISAccessFormContext);


    /* Creation of wCommentTxt */
    wCommentTxt = XtVaCreateManagedWidget("wCommentTxt",
					  xmTextFieldWidgetClass,
					  FISAccessForm,
					  XmNwidth, 380,
					  XmNx, 263,
					  XmNy, 361,
					  XmNheight, 40,
					  XmNleftAttachment, XmATTACH_WIDGET,
					  XmNleftOffset, 15,
					  XmNleftWidget, label12,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					  XmNtopOffset, 0,
					  XmNtopWidget, label15,
					  NULL);
    UxPutContext(wCommentTxt, (char *) UxFISAccessFormContext);


    /* Creation of wTitleTxt */
    wTitleTxt = XtVaCreateManagedWidget("wTitleTxt",
					xmTextFieldWidgetClass,
					FISAccessForm,
					XmNwidth, 380,
					XmNx, 191,
					XmNy, 210,
					XmNheight, 40,
					XmNleftAttachment, XmATTACH_WIDGET,
					XmNleftOffset, 15,
					XmNleftWidget, label12,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					XmNtopOffset, 0,
					XmNtopWidget, label12,
					NULL);
    UxPutContext(wTitleTxt, (char *) UxFISAccessFormContext);


    /* Creation of wDatabaseKeyTxt */
    wDatabaseKeyTxt = XtVaCreateManagedWidget("wDatabaseKeyTxt",
					      xmTextFieldWidgetClass,
					      FISAccessForm,
					      XmNwidth, 380,
					      XmNx, 211,
					      XmNy, 260,
					      XmNheight, 40,
					 XmNleftAttachment, XmATTACH_WIDGET,
					      XmNleftOffset, 15,
					      XmNleftWidget, label12,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					      XmNtopOffset, 0,
					      XmNtopWidget, label13,
					      NULL);
    UxPutContext(wDatabaseKeyTxt, (char *) UxFISAccessFormContext);


    /* Creation of pushButton12 */
    pushButton12 = XtVaCreateManagedWidget("pushButton12",
					   xmPushButtonWidgetClass,
					   FISAccessForm,
					   XmNx, 41,
					   XmNy, 420,
					   XmNwidth, 140,
					   XmNheight, 40,
					 RES_CONVERT(XmNlabelString, "Add"),
					   NULL);
    XtAddCallback(pushButton12, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton12,
		  (XtPointer) UxFISAccessFormContext);

    UxPutContext(pushButton12, (char *) UxFISAccessFormContext);


    /* Creation of pushButton13 */
    pushButton13 = XtVaCreateManagedWidget("pushButton13",
					   xmPushButtonWidgetClass,
					   FISAccessForm,
					   XmNx, 231,
					   XmNy, 420,
					   XmNwidth, 140,
					   XmNheight, 40,
				      RES_CONVERT(XmNlabelString, "Update"),
					   XmNsensitive, FALSE,
					   NULL);
    UxPutContext(pushButton13, (char *) UxFISAccessFormContext);


    /* Creation of pushButton14 */
    pushButton14 = XtVaCreateManagedWidget("pushButton14",
					   xmPushButtonWidgetClass,
					   FISAccessForm,
					   XmNx, 421,
					   XmNy, 420,
					   XmNwidth, 140,
					   XmNheight, 40,
				      RES_CONVERT(XmNlabelString, "Delete"),
					   NULL);
    XtAddCallback(pushButton14, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton14,
		  (XtPointer) UxFISAccessFormContext);

    UxPutContext(pushButton14, (char *) UxFISAccessFormContext);


    /* Creation of frame4 */
    frame4 = XtVaCreateManagedWidget("frame4",
				     xmFrameWidgetClass,
				     FISAccessForm,
				     XmNwidth, 620,
				     XmNheight, 110,
				     XmNx, 20,
				     XmNy, 20,
				     NULL);
    UxPutContext(frame4, (char *) UxFISAccessFormContext);


    /* Creation of scrolledWindowList6 */
    scrolledWindowList6 = XtVaCreateManagedWidget("scrolledWindowList6",
						xmScrolledWindowWidgetClass,
						  frame4,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNshadowThickness, 0,
						  XmNx, 180,
						  XmNy, 10,
						  NULL);
    UxPutContext(scrolledWindowList6, (char *) UxFISAccessFormContext);


    /* Creation of wFISAccessList */
    wFISAccessList = XtVaCreateManagedWidget("wFISAccessList",
					     xmListWidgetClass,
					     scrolledWindowList6,
					     XmNwidth, 130,
					     XmNheight, 90,
					     NULL);
    UxPutContext(wFISAccessList, (char *) UxFISAccessFormContext);


    XtAddCallback(FISAccessForm, XmNdestroyCallback,
		  (XtCallbackProc) UxDestroyContextCB,
		  (XtPointer) UxFISAccessFormContext);


    return (FISAccessForm);
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget
create_FISAccessForm(swidget _UxUxParent)
{
    Widget rtrn;
    _UxCFISAccessForm *UxContext;

    UxFISAccessFormContext = UxContext =
	(_UxCFISAccessForm *) UxNewContext(sizeof(_UxCFISAccessForm), False);

    UxParent = _UxUxParent;

    rtrn = _Uxbuild_FISAccessForm();

    return (rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/
