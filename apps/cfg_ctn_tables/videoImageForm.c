
/*******************************************************************************
	videoImageForm.c

       Associated Header file: videoImageForm.h
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
** Module Name(s):	create_videoImagelForm
** 			loadVideoImageList
** Author, Date:	Steve Moore, Summer 1994
** Intent:		This file provides the user interface
** 			for defining and modifying entries in
**			the Video Image Control table.  This table
**			is used to define destinations for video
**			images that are captured with the Photo ID
**			system.
**			The public entry points are
**			call to create the widget hierarchy
**			and to populate the list application
**			entities.  All other functions are
** 			local to this specific interface.
** Last Update:		$Author: smm $, $Date: 1995-04-10 13:20:08 $
** Source File:		$RCSfile: videoImageForm.c,v $
** Revision:		$Revision: 1.5 $
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
static LST_HEAD *videoImageList = NULL;


/*******************************************************************************
       The following header file defines the context structure.
*******************************************************************************/

#define CONTEXT_MACRO_ACCESS 1
#include "videoImageForm.h"
#undef CONTEXT_MACRO_ACCESS


/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

void
loadVideoImageList()
{
    CONDITION cond;
    DMAN_VIDEOIMAGEDEST a,
        criteria;
    char b[256];

    if (videoImageList == NULL)
	videoImageList = LST_Create();
    if (videoImageList == NULL)
	return;

    memset(&a, 0, sizeof(a));
    a.Type = DMAN_K_VIDEOIMAGEDEST;
    memset(&criteria, 0, sizeof(criteria));
    criteria.Type = DMAN_K_VIDEOIMAGEDEST;

    cond = DMAN_Select(&dmanHandle,
		       (DMAN_GENERICRECORD *) & a,
		       (DMAN_GENERICRECORD *) & criteria,
		       videoImageList, NULL, NULL, NULL);
    if (cond != DMAN_NORMAL) {
	COND_DumpConditions();
	return;
    }
    MUT_LoadList(wVideoImageList, videoImageList,
		 formatVideoImage, b);
}

static void
displayVideoImage(DMAN_VIDEOIMAGEDEST * d)
{
    if (d == NULL)
	return;

    XmTextSetString(wRequestingTitleTxt, d->RequestingTitle);
    XmTextSetString(wRespondingTitleTxt, d->RespondingTitle);
    XmTextSetString(wImageTypeTxt, d->ImageType);
}

static CONDITION
readVideoImage(DMAN_VIDEOIMAGEDEST * d)
{
    CONDITION cond;
    CTNBOOLEAN nullFlag;

    memset(d, 0, sizeof(*d));
    d->Type = DMAN_K_VIDEOIMAGEDEST;

    cond = MUT_ScanWidget(wRequestingTitleTxt, MUT_TEXT,
			  &nullFlag, d->RequestingTitle);
    if (cond != MUT_NORMAL || nullFlag)
	return 0;
    d->Flag |= DMAN_K_VIDEOIMAGE_REQUESTING;

    cond = MUT_ScanWidget(wRespondingTitleTxt, MUT_TEXT,
			  &nullFlag, d->RespondingTitle);
    if (cond != MUT_NORMAL || nullFlag)
	return 0;
    d->Flag |= DMAN_K_VIDEOIMAGE_RESPONDING;

    cond = MUT_ScanWidget(wImageTypeTxt, MUT_TEXT,
			  &nullFlag, d->ImageType);
    if (cond != MUT_NORMAL || nullFlag)
	return 0;
    d->Flag |= DMAN_K_VIDEOIMAGE_IMAGETYPE;

    return 1;
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void
browseSelectionCB_wVideoImageList(
				  Widget wgt,
				  XtPointer cd,
				  XtPointer cb)
{
    _UxCvideoImageForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxVideoImageFormContext;
    UxVideoImageFormContext = UxContext =
	(_UxCvideoImageForm *) UxGetContext(UxWidget);
    {
	XmListCallbackStruct *l;
	DMAN_VIDEOIMAGEDEST *video;

	l = (XmListCallbackStruct *) UxCallbackArg;

	video = LST_Index(&videoImageList, l->item_position);
	displayVideoImage(video);
    }
    UxVideoImageFormContext = UxSaveCtx;
}

static void
activateCB_pushButton21(
			Widget wgt,
			XtPointer cd,
			XtPointer cb)
{
    _UxCvideoImageForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxVideoImageFormContext;
    UxVideoImageFormContext = UxContext =
	(_UxCvideoImageForm *) UxGetContext(UxWidget);
    {
	DMAN_VIDEOIMAGEDEST v;
	CONDITION cond;

	cond = readVideoImage(&v);
	if (cond != 1)
	    return;

	cond = DMAN_Insert(&dmanHandle, (DMAN_GENERICRECORD *) & v);
	if (cond != DMAN_NORMAL)
	    COND_DumpConditions();

	loadVideoImageList();
    }
    UxVideoImageFormContext = UxSaveCtx;
}

static void
activateCB_pushButton22(
			Widget wgt,
			XtPointer cd,
			XtPointer cb)
{
    _UxCvideoImageForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxVideoImageFormContext;
    UxVideoImageFormContext = UxContext =
	(_UxCvideoImageForm *) UxGetContext(UxWidget);
    {
	DMAN_VIDEOIMAGEDEST v;
	CONDITION cond;

	cond = readVideoImage(&v);
	if (cond != 1)
	    return;

	cond = DMAN_Delete(&dmanHandle, (DMAN_GENERICRECORD *) & v);
	if (cond != DMAN_NORMAL)
	    COND_DumpConditions();

	loadVideoImageList();
    }
    UxVideoImageFormContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget
_Uxbuild_videoImageForm()
{
    Widget _UxParent;


    /* Creation of videoImageForm */
    _UxParent = UxParent;
    if (_UxParent == NULL) {
	_UxParent = XtVaCreatePopupShell("videoImageForm_shell",
				       topLevelShellWidgetClass, UxTopLevel,
					 XmNx, 335,
					 XmNy, 159,
					 XmNwidth, 520,
					 XmNheight, 351,
					 XmNshellUnitType, XmPIXELS,
					 XmNtitle, "videoImageForm",
					 XmNiconName, "videoImageForm",
					 NULL);

    }
    videoImageForm = XtVaCreateManagedWidget("videoImageForm",
					     xmFormWidgetClass,
					     _UxParent,
					     XmNwidth, 520,
					     XmNheight, 351,
					     XmNresizePolicy, XmRESIZE_NONE,
					     XmNunitType, XmPIXELS,
					     NULL);
    UxPutContext(videoImageForm, (char *) UxVideoImageFormContext);


    /* Creation of frame6 */
    frame6 = XtVaCreateManagedWidget("frame6",
				     xmFrameWidgetClass,
				     videoImageForm,
				     XmNwidth, 490,
				     XmNheight, 110,
				     XmNx, 20,
				     XmNy, 20,
				     XmNresizable, FALSE,
				     NULL);
    UxPutContext(frame6, (char *) UxVideoImageFormContext);


    /* Creation of scrolledWindowList9 */
    scrolledWindowList9 = XtVaCreateManagedWidget("scrolledWindowList9",
						xmScrolledWindowWidgetClass,
						  frame6,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNshadowThickness, 0,
						  XmNx, 70,
						  XmNy, 20,
						  NULL);
    UxPutContext(scrolledWindowList9, (char *) UxVideoImageFormContext);


    /* Creation of wVideoImageList */
    wVideoImageList = XtVaCreateManagedWidget("wVideoImageList",
					      xmListWidgetClass,
					      scrolledWindowList9,
					      XmNwidth, 190,
					      XmNheight, 30,
					      NULL);
    XtAddCallback(wVideoImageList, XmNbrowseSelectionCallback,
		  (XtCallbackProc) browseSelectionCB_wVideoImageList,
		  (XtPointer) UxVideoImageFormContext);

    UxPutContext(wVideoImageList, (char *) UxVideoImageFormContext);


    /* Creation of label26 */
    label26 = XtVaCreateManagedWidget("label26",
				      xmLabelWidgetClass,
				      videoImageForm,
				      XmNx, 30,
				      XmNy, 150,
				      XmNwidth, 130,
				      XmNheight, 30,
			    RES_CONVERT(XmNlabelString, "Requesting Title"),
				      NULL);
    UxPutContext(label26, (char *) UxVideoImageFormContext);


    /* Creation of label27 */
    label27 = XtVaCreateManagedWidget("label27",
				      xmLabelWidgetClass,
				      videoImageForm,
				      XmNx, 30,
				      XmNy, 190,
				      XmNwidth, 130,
				      XmNheight, 30,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				      XmNleftOffset, 0,
				      XmNleftWidget, label26,
				      XmNtopAttachment, XmATTACH_WIDGET,
				      XmNtopOffset, 20,
				      XmNtopWidget, label26,
			    RES_CONVERT(XmNlabelString, "Responding Title"),
				      NULL);
    UxPutContext(label27, (char *) UxVideoImageFormContext);


    /* Creation of label28 */
    label28 = XtVaCreateManagedWidget("label28",
				      xmLabelWidgetClass,
				      videoImageForm,
				      XmNx, 30,
				      XmNy, 240,
				      XmNwidth, 130,
				      XmNheight, 30,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				      XmNleftOffset, 0,
				      XmNleftWidget, label26,
				      XmNtopAttachment, XmATTACH_WIDGET,
				      XmNtopOffset, 20,
				      XmNtopWidget, label27,
				  RES_CONVERT(XmNlabelString, "Image Type"),
				      NULL);
    UxPutContext(label28, (char *) UxVideoImageFormContext);


    /* Creation of wRequestingTitleTxt */
    wRequestingTitleTxt = XtVaCreateManagedWidget("wRequestingTitleTxt",
						  xmTextFieldWidgetClass,
						  videoImageForm,
						  XmNwidth, 320,
						  XmNx, 190,
						  XmNy, 150,
						  XmNheight, 40,
					 XmNleftAttachment, XmATTACH_WIDGET,
						  XmNleftOffset, 15,
						  XmNleftWidget, label26,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
						  XmNtopOffset, 0,
						  XmNtopWidget, label26,
						  NULL);
    UxPutContext(wRequestingTitleTxt, (char *) UxVideoImageFormContext);


    /* Creation of wRespondingTitleTxt */
    wRespondingTitleTxt = XtVaCreateManagedWidget("wRespondingTitleTxt",
						  xmTextFieldWidgetClass,
						  videoImageForm,
						  XmNwidth, 320,
						  XmNx, 190,
						  XmNy, 190,
						  XmNheight, 40,
					 XmNleftAttachment, XmATTACH_WIDGET,
						  XmNleftOffset, 15,
						  XmNleftWidget, label26,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
						  XmNtopOffset, 0,
						  XmNtopWidget, label27,
						  NULL);
    UxPutContext(wRespondingTitleTxt, (char *) UxVideoImageFormContext);


    /* Creation of wImageTypeTxt */
    wImageTypeTxt = XtVaCreateManagedWidget("wImageTypeTxt",
					    xmTextFieldWidgetClass,
					    videoImageForm,
					    XmNwidth, 320,
					    XmNx, 190,
					    XmNy, 230,
					    XmNheight, 40,
					 XmNleftAttachment, XmATTACH_WIDGET,
					    XmNleftOffset, 15,
					    XmNleftWidget, label26,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					    XmNtopOffset, 0,
					    XmNtopWidget, label28,
					    NULL);
    UxPutContext(wImageTypeTxt, (char *) UxVideoImageFormContext);


    /* Creation of pushButton21 */
    pushButton21 = XtVaCreateManagedWidget("pushButton21",
					   xmPushButtonWidgetClass,
					   videoImageForm,
					   XmNx, 60,
					   XmNy, 300,
					   XmNwidth, 140,
					   XmNheight, 40,
					 RES_CONVERT(XmNlabelString, "Add"),
					   NULL);
    XtAddCallback(pushButton21, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton21,
		  (XtPointer) UxVideoImageFormContext);

    UxPutContext(pushButton21, (char *) UxVideoImageFormContext);


    /* Creation of pushButton22 */
    pushButton22 = XtVaCreateManagedWidget("pushButton22",
					   xmPushButtonWidgetClass,
					   videoImageForm,
					   XmNx, 320,
					   XmNy, 300,
					   XmNwidth, 140,
					   XmNheight, 40,
				      RES_CONVERT(XmNlabelString, "Delete"),
					   NULL);
    XtAddCallback(pushButton22, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton22,
		  (XtPointer) UxVideoImageFormContext);

    UxPutContext(pushButton22, (char *) UxVideoImageFormContext);


    XtAddCallback(videoImageForm, XmNdestroyCallback,
		  (XtCallbackProc) UxDestroyContextCB,
		  (XtPointer) UxVideoImageFormContext);


    return (videoImageForm);
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget
create_videoImageForm(swidget _UxUxParent)
{
    Widget rtrn;
    _UxCvideoImageForm *UxContext;

    UxVideoImageFormContext = UxContext =
	(_UxCvideoImageForm *) UxNewContext(sizeof(_UxCvideoImageForm), False);

    UxParent = _UxUxParent;

    rtrn = _Uxbuild_videoImageForm();

    return (rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/
