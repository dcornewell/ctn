
/*******************************************************************************
	imageStorageControlForm.c

       Associated Header file: imageStorageControlForm.h
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <X11/Shell.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/PushB.h>
#include <Xm/TextF.h>
#include <Xm/Label.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/Frame.h>
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
** Module Name(s):	create_imageStorageControlForm
**			loadImageStorageControlList
** Author, Date:	Steve Moore, Summer 1994
** Intent:		This file provides the user interface
** 			for defining and modifying entries in
** 			the Image Storage Control table.  This table
**			is used to define the directories used to
**			store images as they are received from external
**			applications.
**			The public entry points are
**			call to create the widget hierarchy
** 			and to populate the list application
**			entities.  All other functions are
**			local to this specific interface.
** Last Update:		$Author: smm $, $Date: 1995-04-10 13:19:53 $
** Source File:		$RCSfile: imageStorageControlForm.c,v $
** Revision:		$Revision: 1.3 $
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

static LST_HEAD *storageControlList = NULL;


/*******************************************************************************
       The following header file defines the context structure.
*******************************************************************************/

#define CONTEXT_MACRO_ACCESS 1
#include "imageStorageControlForm.h"
#undef CONTEXT_MACRO_ACCESS


/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

void
loadImageStorageControlList()
{
    CONDITION cond;
    DMAN_STORAGECONTROL s,
        criteria;

    char buf[120];

    if (storageControlList == NULL)
	storageControlList = LST_Create();
    if (storageControlList == NULL)
	return;

    memset(&s, 0, sizeof(s));
    memset(&criteria, 0, sizeof(criteria));
    s.Type = criteria.Type = DMAN_K_STORAGECONTROL;

    cond = DMAN_Select(&dmanHandle, (DMAN_GENERICRECORD *) & s,
		       (DMAN_GENERICRECORD *) & criteria,
		       storageControlList, NULL, NULL, NULL);
    if (cond != DMAN_NORMAL) {
	COND_DumpConditions();
	return;
    }
    MUT_LoadList(wStorageControlList, storageControlList,
		 formatStorageControl, buf);
}

static CONDITION
readStorageControl(DMAN_STORAGECONTROL * s)
{
    CONDITION cond;
    CTNBOOLEAN nullFlag;

    if (s == NULL)
	return 0;
    memset(s, 0, sizeof(*s));
    s->Type = DMAN_K_STORAGECONTROL;
    s->Flag = 0;

    cond = MUT_ScanWidget(wRequestingApplicationTxt, MUT_TEXT,
			  &nullFlag, s->RequestingTitle);
    if (cond != MUT_NORMAL || nullFlag)
	return 0;
    s->Flag |= DMAN_K_STORAGECONTROL_REQUESTING;

    cond = MUT_ScanWidget(wRespondingApplicationTxt, MUT_TEXT,
			  &nullFlag, s->RespondingTitle);
    if (cond != MUT_NORMAL || nullFlag)
	return 0;
    s->Flag |= DMAN_K_STORAGECONTROL_RESPONDING;

    cond = MUT_ScanWidget(wMediumTxt, MUT_TEXT, &nullFlag,
			  s->Medium);
    if (cond != MUT_NORMAL)
	return 0;
    if (!nullFlag)
	s->Flag |= DMAN_K_STORAGECONTROL_MEDIUM;

    cond = MUT_ScanWidget(wRootTxt, MUT_TEXT, &nullFlag, s->Root);
    if (cond != MUT_NORMAL || nullFlag) {
	return 0;
    }
    s->Flag |= DMAN_K_STORAGECONTROL_ROOT;

    return 1;
}

static void
displayStorageControl(DMAN_STORAGECONTROL * s)
{
    if (s == NULL)
	return;

    if (s->Flag & DMAN_K_STORAGECONTROL_REQUESTING)
	XmTextSetString(wRequestingApplicationTxt, s->RequestingTitle);
    else
	XmTextSetString(wRequestingApplicationTxt, "");

    if (s->Flag & DMAN_K_STORAGECONTROL_RESPONDING)
	XmTextSetString(wRespondingApplicationTxt, s->RespondingTitle);
    else
	XmTextSetString(wRespondingApplicationTxt, "");

    if (s->Flag & DMAN_K_STORAGECONTROL_MEDIUM)
	XmTextSetString(wMediumTxt, s->Medium);
    else
	XmTextSetString(wMediumTxt, "");

    if (s->Flag & DMAN_K_STORAGECONTROL_ROOT)
	XmTextSetString(wRootTxt, s->Root);
    else
	XmTextSetString(wRootTxt, "");
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void
browseSelectionCB_wStorageControlList(
				      Widget wgt,
				      XtPointer cd,
				      XtPointer cb)
{
    _UxCimageStorageControlForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxImageStorageControlFormContext;
    UxImageStorageControlFormContext = UxContext =
	(_UxCimageStorageControlForm *) UxGetContext(UxWidget);
    {
	XmListCallbackStruct *l;
	DMAN_STORAGECONTROL *s;

	l = (XmListCallbackStruct *) UxCallbackArg;
	s = LST_Index(&storageControlList, l->item_position);
	displayStorageControl(s);
    }
    UxImageStorageControlFormContext = UxSaveCtx;
}

static void
activateCB_pushButton18(
			Widget wgt,
			XtPointer cd,
			XtPointer cb)
{
    _UxCimageStorageControlForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxImageStorageControlFormContext;
    UxImageStorageControlFormContext = UxContext =
	(_UxCimageStorageControlForm *) UxGetContext(UxWidget);
    {
	CONDITION cond;
	DMAN_STORAGECONTROL s;

	cond = readStorageControl(&s);
	if (cond != 1) {
	    return;
	}
	cond = DMAN_Insert(&dmanHandle, (DMAN_GENERICRECORD *) & s);
	if (cond != DMAN_NORMAL) {
	    COND_DumpConditions();
	}
	loadImageStorageControlList();
    }
    UxImageStorageControlFormContext = UxSaveCtx;
}

static void
activateCB_pushButton20(
			Widget wgt,
			XtPointer cd,
			XtPointer cb)
{
    _UxCimageStorageControlForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxImageStorageControlFormContext;
    UxImageStorageControlFormContext = UxContext =
	(_UxCimageStorageControlForm *) UxGetContext(UxWidget);
    {
	CONDITION cond;
	DMAN_STORAGECONTROL s;

	cond = readStorageControl(&s);
	if (cond != 1) {
	    COND_DumpConditions();
	    return;
	}
	cond = DMAN_Delete(&dmanHandle, (DMAN_GENERICRECORD *) & s);
	if (cond != DMAN_NORMAL) {
	    COND_DumpConditions();
	}
	loadImageStorageControlList();
    }
    UxImageStorageControlFormContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget
_Uxbuild_imageStorageControlForm()
{
    Widget _UxParent;


    /* Creation of imageStorageControlForm */
    _UxParent = UxParent;
    if (_UxParent == NULL) {
	_UxParent = XtVaCreatePopupShell("imageStorageControlForm_shell",
				       topLevelShellWidgetClass, UxTopLevel,
					 XmNx, 260,
					 XmNy, 310,
					 XmNwidth, 770,
					 XmNheight, 490,
					 XmNshellUnitType, XmPIXELS,
					 XmNtitle, "imageStorageControlForm",
				     XmNiconName, "imageStorageControlForm",
					 NULL);

    }
    imageStorageControlForm = XtVaCreateManagedWidget("imageStorageControlForm",
						      xmFormWidgetClass,
						      _UxParent,
					     XmNresizePolicy, XmRESIZE_NONE,
						      XmNunitType, XmPIXELS,
						      XmNwidth, 770,
						      XmNheight, 490,
						      NULL);
    UxPutContext(imageStorageControlForm, (char *) UxImageStorageControlFormContext);


    /* Creation of frame7 */
    frame7 = XtVaCreateManagedWidget("frame7",
				     xmFrameWidgetClass,
				     imageStorageControlForm,
				     XmNwidth, 740,
				     XmNheight, 140,
				     XmNx, 20,
				     XmNy, 10,
				     XmNresizable, FALSE,
				     NULL);
    UxPutContext(frame7, (char *) UxImageStorageControlFormContext);


    /* Creation of scrolledWindowList8 */
    scrolledWindowList8 = XtVaCreateManagedWidget("scrolledWindowList8",
						xmScrolledWindowWidgetClass,
						  frame7,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNshadowThickness, 0,
						  XmNx, 50,
						  XmNy, 30,
						  NULL);
    UxPutContext(scrolledWindowList8, (char *) UxImageStorageControlFormContext);


    /* Creation of wStorageControlList */
    wStorageControlList = XtVaCreateManagedWidget("wStorageControlList",
						  xmListWidgetClass,
						  scrolledWindowList8,
						  XmNwidth, 260,
						  XmNheight, 40,
						  NULL);
    XtAddCallback(wStorageControlList, XmNbrowseSelectionCallback,
		  (XtCallbackProc) browseSelectionCB_wStorageControlList,
		  (XtPointer) UxImageStorageControlFormContext);

    UxPutContext(wStorageControlList, (char *) UxImageStorageControlFormContext);


    /* Creation of label22 */
    label22 = XtVaCreateManagedWidget("label22",
				      xmLabelWidgetClass,
				      imageStorageControlForm,
				      XmNx, 30,
				      XmNy, 170,
				      XmNwidth, 250,
				      XmNheight, 50,
		      RES_CONVERT(XmNlabelString, "Requesting Application"),
				      NULL);
    UxPutContext(label22, (char *) UxImageStorageControlFormContext);


    /* Creation of wRequestingApplicationTxt */
    wRequestingApplicationTxt = XtVaCreateManagedWidget("wRequestingApplicationTxt",
						     xmTextFieldWidgetClass,
						    imageStorageControlForm,
							XmNwidth, 440,
							XmNx, 320,
							XmNy, 170,
							XmNheight, 50,
					 XmNleftAttachment, XmATTACH_WIDGET,
							XmNleftOffset, 15,
						     XmNleftWidget, label22,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
							XmNtopOffset, 0,
						      XmNtopWidget, label22,
							NULL);
    UxPutContext(wRequestingApplicationTxt, (char *) UxImageStorageControlFormContext);


    /* Creation of label23 */
    label23 = XtVaCreateManagedWidget("label23",
				      xmLabelWidgetClass,
				      imageStorageControlForm,
				      XmNx, 30,
				      XmNy, 220,
				      XmNwidth, 250,
				      XmNheight, 50,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				      XmNleftOffset, 0,
				      XmNleftWidget, label22,
				      XmNtopAttachment, XmATTACH_WIDGET,
				      XmNtopOffset, 10,
				      XmNtopWidget, label22,
		      RES_CONVERT(XmNlabelString, "Responding Application"),
				      NULL);
    UxPutContext(label23, (char *) UxImageStorageControlFormContext);


    /* Creation of label24 */
    label24 = XtVaCreateManagedWidget("label24",
				      xmLabelWidgetClass,
				      imageStorageControlForm,
				      XmNx, 30,
				      XmNy, 260,
				      XmNwidth, 250,
				      XmNheight, 50,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				      XmNleftWidget, label22,
				      XmNtopAttachment, XmATTACH_WIDGET,
				      XmNtopOffset, 10,
				      XmNtopWidget, label23,
				      RES_CONVERT(XmNlabelString, "Medium"),
				      NULL);
    UxPutContext(label24, (char *) UxImageStorageControlFormContext);


    /* Creation of label25 */
    label25 = XtVaCreateManagedWidget("label25",
				      xmLabelWidgetClass,
				      imageStorageControlForm,
				      XmNx, 30,
				      XmNy, 300,
				      XmNwidth, 250,
				      XmNheight, 50,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				      XmNleftOffset, 0,
				      XmNleftWidget, label22,
				      XmNtopAttachment, XmATTACH_WIDGET,
				      XmNtopOffset, 10,
				      XmNtopWidget, label24,
			  RES_CONVERT(XmNlabelString, "(File System) Root"),
				      NULL);
    UxPutContext(label25, (char *) UxImageStorageControlFormContext);


    /* Creation of wRespondingApplicationTxt */
    wRespondingApplicationTxt = XtVaCreateManagedWidget("wRespondingApplicationTxt",
						     xmTextFieldWidgetClass,
						    imageStorageControlForm,
							XmNwidth, 440,
							XmNx, 320,
							XmNy, 220,
							XmNheight, 50,
					 XmNleftAttachment, XmATTACH_WIDGET,
							XmNleftOffset, 15,
						     XmNleftWidget, label22,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
							XmNtopOffset, 0,
						      XmNtopWidget, label23,
							NULL);
    UxPutContext(wRespondingApplicationTxt, (char *) UxImageStorageControlFormContext);


    /* Creation of wMediumTxt */
    wMediumTxt = XtVaCreateManagedWidget("wMediumTxt",
					 xmTextFieldWidgetClass,
					 imageStorageControlForm,
					 XmNwidth, 440,
					 XmNx, 320,
					 XmNy, 270,
					 XmNheight, 50,
					 XmNleftAttachment, XmATTACH_WIDGET,
					 XmNleftOffset, 15,
					 XmNleftWidget, label22,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					 XmNtopOffset, 0,
					 XmNtopWidget, label24,
					 NULL);
    UxPutContext(wMediumTxt, (char *) UxImageStorageControlFormContext);


    /* Creation of wRootTxt */
    wRootTxt = XtVaCreateManagedWidget("wRootTxt",
				       xmTextFieldWidgetClass,
				       imageStorageControlForm,
				       XmNwidth, 440,
				       XmNx, 320,
				       XmNy, 320,
				       XmNheight, 50,
				       XmNleftAttachment, XmATTACH_WIDGET,
				       XmNleftOffset, 15,
				       XmNleftWidget, label22,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
				       XmNtopOffset, 0,
				       XmNtopWidget, label25,
				       NULL);
    UxPutContext(wRootTxt, (char *) UxImageStorageControlFormContext);


    /* Creation of pushButton18 */
    pushButton18 = XtVaCreateManagedWidget("pushButton18",
					   xmPushButtonWidgetClass,
					   imageStorageControlForm,
					   XmNx, 60,
					   XmNy, 410,
					   XmNwidth, 150,
					   XmNheight, 50,
					 RES_CONVERT(XmNlabelString, "Add"),
					   NULL);
    XtAddCallback(pushButton18, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton18,
		  (XtPointer) UxImageStorageControlFormContext);

    UxPutContext(pushButton18, (char *) UxImageStorageControlFormContext);


    /* Creation of pushButton19 */
    pushButton19 = XtVaCreateManagedWidget("pushButton19",
					   xmPushButtonWidgetClass,
					   imageStorageControlForm,
					   XmNx, 310,
					   XmNy, 410,
					   XmNwidth, 150,
					   XmNheight, 50,
				      RES_CONVERT(XmNlabelString, "Update"),
					   XmNsensitive, FALSE,
					   NULL);
    UxPutContext(pushButton19, (char *) UxImageStorageControlFormContext);


    /* Creation of pushButton20 */
    pushButton20 = XtVaCreateManagedWidget("pushButton20",
					   xmPushButtonWidgetClass,
					   imageStorageControlForm,
					   XmNx, 560,
					   XmNy, 410,
					   XmNwidth, 150,
					   XmNheight, 50,
				      RES_CONVERT(XmNlabelString, "Delete"),
					   NULL);
    XtAddCallback(pushButton20, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton20,
		  (XtPointer) UxImageStorageControlFormContext);

    UxPutContext(pushButton20, (char *) UxImageStorageControlFormContext);


    XtAddCallback(imageStorageControlForm, XmNdestroyCallback,
		  (XtCallbackProc) UxDestroyContextCB,
		  (XtPointer) UxImageStorageControlFormContext);


    return (imageStorageControlForm);
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget
create_imageStorageControlForm(swidget _UxUxParent)
{
    Widget rtrn;
    _UxCimageStorageControlForm *UxContext;

    UxImageStorageControlFormContext = UxContext =
	(_UxCimageStorageControlForm *) UxNewContext(sizeof(_UxCimageStorageControlForm), False);

    UxParent = _UxUxParent;

    rtrn = _Uxbuild_imageStorageControlForm();

    return (rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/
