
/*******************************************************************************
	imageStorageAccessForm.c

       Associated Header file: imageStorageAccessForm.h
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
** Module Name(s):      create_imageStorageAccessForm
**                      loadImageStorageAccessList
** Author, Date:        Steve Moore, Summer 1994
** Intent:              This file provides the user interface
**                      for defining and modifying entries in
**                      the Image Storage Access table.  This table
**			is used to define which (external) applications
**			have access rights to storage areas manged
**			by the image server or other applications.
**                      The public entry points are
**                      call to create the widget hierarchy
**                      and to populate the list application
**                      entities.  All other functions are
**                      local to this specific interface.
** Last Update:         $Author: smm $, $Date: 1995-04-10 13:19:48 $
** Source File:         $RCSfile: imageStorageAccessForm.c,v $
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

static LST_HEAD *storageAccessList = NULL;
extern DMAN_HANDLE *dmanHandle;
void loadImageStorageAccessList();


/*******************************************************************************
       The following header file defines the context structure.
*******************************************************************************/

#define CONTEXT_MACRO_ACCESS 1
#include "imageStorageAccessForm.h"
#undef CONTEXT_MACRO_ACCESS


/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

void
loadImageStorageAccessList()
{
    CONDITION cond;
    DMAN_STORAGEACCESS s,
        criteria;
    char buf[120];

    if (storageAccessList == NULL)
	storageAccessList = LST_Create();

    if (storageAccessList == NULL)
	return;

    memset(&s, 0, sizeof(s));
    memset(&criteria, 0, sizeof(criteria));
    s.Type = DMAN_K_STORAGEACCESS;
    criteria.Type = DMAN_K_STORAGEACCESS;

    cond = DMAN_Select(&dmanHandle, (DMAN_GENERICRECORD *) & s,
		       (DMAN_GENERICRECORD *) & criteria,
		       storageAccessList, NULL, NULL, NULL);
    if (cond != DMAN_NORMAL) {
	COND_DumpConditions();
	return;
    }
    MUT_LoadList(wStorageAccessList, storageAccessList,
		 formatStorageAccess, buf);
}

static CONDITION
readStorageAccess(DMAN_STORAGEACCESS * s)
{
    CONDITION cond;
    CTNBOOLEAN nullFlag;

    memset(s, 0, sizeof(*s));
    s->Type = DMAN_K_STORAGEACCESS;
    s->Flag = 0;
    cond = MUT_ScanWidget(wApplicationTitleTxt, MUT_TEXT, &nullFlag, s->Title);
    if (cond != MUT_NORMAL || nullFlag)
	return 0;
    s->Flag |= DMAN_K_STORAGEACCESS_TITLE;

    cond = MUT_ScanWidget(wDatabaseKeyTxt, MUT_TEXT, &nullFlag, s->DbKey);
    if (cond != MUT_NORMAL || nullFlag) {
	return 0;
    }
    s->Flag |= DMAN_K_STORAGEACCESS_DBKEY;

    cond = MUT_ScanWidget(wOwnerTxt, MUT_TEXT, &nullFlag, s->Owner);
    if (cond != MUT_NORMAL) {
	return 0;
    }
    if (!nullFlag)
	s->Flag |= DMAN_K_STORAGEACCESS_OWNER;

    cond = MUT_ScanWidget(wGroupNameTxt, MUT_TEXT, &nullFlag, s->GroupName);
    if (cond != MUT_NORMAL) {
	return 0;
    }
    if (!nullFlag)
	s->Flag |= DMAN_K_STORAGEACCESS_GROUPNAME;

    s->Access = 0xffffff;
    s->Flag |= DMAN_K_STORAGEACCESS_ACCESS;

    cond = MUT_ScanWidget(wCommentTxt, MUT_TEXT, &nullFlag, s->Comment);
    if (cond != MUT_NORMAL) {
	return 0;
    }
    if (!nullFlag)
	s->Flag |= DMAN_K_STORAGEACCESS_COMMENT;

    return 1;
}

static void
displayStorageAccess(DMAN_STORAGEACCESS * s)
{
    if (s == NULL)
	return;

    if (s->Flag & DMAN_K_STORAGEACCESS_TITLE)
	XmTextSetString(wApplicationTitleTxt, s->Title);
    else
	XmTextSetString(wApplicationTitleTxt, "");

    if (s->Flag & DMAN_K_STORAGEACCESS_DBKEY)
	XmTextSetString(wDatabaseKeyTxt, s->DbKey);
    else
	XmTextSetString(wDatabaseKeyTxt, "");

    if (s->Flag & DMAN_K_STORAGEACCESS_OWNER)
	XmTextSetString(wOwnerTxt, s->Owner);
    else
	XmTextSetString(wOwnerTxt, "");

    if (s->Flag & DMAN_K_STORAGEACCESS_GROUPNAME)
	XmTextSetString(wGroupNameTxt, s->GroupName);
    else
	XmTextSetString(wGroupNameTxt, "");

    if (s->Flag & DMAN_K_STORAGEACCESS_COMMENT)
	XmTextSetString(wCommentTxt, s->Comment);
    else
	XmTextSetString(wCommentTxt, "");
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void
browseSelectionCB_wStorageAccessList(
				     Widget wgt,
				     XtPointer cd,
				     XtPointer cb)
{
    _UxCimageStorageAccessForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxImageStorageAccessFormContext;
    UxImageStorageAccessFormContext = UxContext =
	(_UxCimageStorageAccessForm *) UxGetContext(UxWidget);
    {
	XmListCallbackStruct *l;
	DMAN_STORAGEACCESS *s;

	l = (XmListCallbackStruct *) UxCallbackArg;
	s = LST_Index(&storageAccessList, l->item_position);

	displayStorageAccess(s);
    }
    UxImageStorageAccessFormContext = UxSaveCtx;
}

static void
activateCB_pushButton15(
			Widget wgt,
			XtPointer cd,
			XtPointer cb)
{
    _UxCimageStorageAccessForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxImageStorageAccessFormContext;
    UxImageStorageAccessFormContext = UxContext =
	(_UxCimageStorageAccessForm *) UxGetContext(UxWidget);
    {
	CONDITION cond;
	DMAN_STORAGEACCESS s;

	cond = readStorageAccess(&s);
	if (cond != 1)
	    return;

	cond = DMAN_Insert(&dmanHandle, (DMAN_GENERICRECORD *) & s);
	if (cond != DMAN_NORMAL)
	    COND_DumpConditions();

	loadImageStorageAccessList();
    }
    UxImageStorageAccessFormContext = UxSaveCtx;
}

static void
activateCB_pushButton17(
			Widget wgt,
			XtPointer cd,
			XtPointer cb)
{
    _UxCimageStorageAccessForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxImageStorageAccessFormContext;
    UxImageStorageAccessFormContext = UxContext =
	(_UxCimageStorageAccessForm *) UxGetContext(UxWidget);
    {
	CONDITION cond;
	DMAN_STORAGEACCESS s;

	cond = readStorageAccess(&s);
	if (cond != 1)
	    return;

	cond = DMAN_Delete(&dmanHandle, (DMAN_GENERICRECORD *) & s);
	if (cond != DMAN_NORMAL)
	    COND_DumpConditions();

	loadImageStorageAccessList();
    }
    UxImageStorageAccessFormContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget
_Uxbuild_imageStorageAccessForm()
{
    Widget _UxParent;


    /* Creation of imageStorageAccessForm */
    _UxParent = UxParent;
    if (_UxParent == NULL) {
	_UxParent = XtVaCreatePopupShell("imageStorageAccessForm_shell",
				       topLevelShellWidgetClass, UxTopLevel,
					 XmNx, 275,
					 XmNy, 249,
					 XmNwidth, 795,
					 XmNheight, 647,
					 XmNshellUnitType, XmPIXELS,
					 XmNtitle, "imageStorageAccessForm",
				      XmNiconName, "imageStorageAccessForm",
					 NULL);

    }
    imageStorageAccessForm = XtVaCreateManagedWidget("imageStorageAccessForm",
						     xmFormWidgetClass,
						     _UxParent,
						     XmNwidth, 795,
						     XmNheight, 647,
					     XmNresizePolicy, XmRESIZE_NONE,
						     XmNunitType, XmPIXELS,
						     NULL);
    UxPutContext(imageStorageAccessForm, (char *) UxImageStorageAccessFormContext);


    /* Creation of frame5 */
    frame5 = XtVaCreateManagedWidget("frame5",
				     xmFrameWidgetClass,
				     imageStorageAccessForm,
				     XmNwidth, 782,
				     XmNheight, 170,
				     XmNx, 8,
				     XmNy, 8,
				     XmNresizable, FALSE,
				     NULL);
    UxPutContext(frame5, (char *) UxImageStorageAccessFormContext);


    /* Creation of scrolledWindowList7 */
    scrolledWindowList7 = XtVaCreateManagedWidget("scrolledWindowList7",
						xmScrolledWindowWidgetClass,
						  frame5,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNshadowThickness, 0,
						  XmNx, 62,
						  XmNy, 12,
						  NULL);
    UxPutContext(scrolledWindowList7, (char *) UxImageStorageAccessFormContext);


    /* Creation of wStorageAccessList */
    wStorageAccessList = XtVaCreateManagedWidget("wStorageAccessList",
						 xmListWidgetClass,
						 scrolledWindowList7,
						 XmNwidth, 330,
						 XmNheight, 140,
						 NULL);
    XtAddCallback(wStorageAccessList, XmNbrowseSelectionCallback,
		  (XtCallbackProc) browseSelectionCB_wStorageAccessList,
		  (XtPointer) UxImageStorageAccessFormContext);

    UxPutContext(wStorageAccessList, (char *) UxImageStorageAccessFormContext);


    /* Creation of label16 */
    label16 = XtVaCreateManagedWidget("label16",
				      xmLabelWidgetClass,
				      imageStorageAccessForm,
				      XmNx, 50,
				      XmNy, 190,
				      XmNwidth, 250,
				      XmNheight, 40,
		     RES_CONVERT(XmNlabelString, "Application Title (CTN)"),
				      NULL);
    UxPutContext(label16, (char *) UxImageStorageAccessFormContext);


    /* Creation of label17 */
    label17 = XtVaCreateManagedWidget("label17",
				      xmLabelWidgetClass,
				      imageStorageAccessForm,
				      XmNx, 50,
				      XmNy, 280,
				      XmNwidth, 250,
				      XmNheight, 40,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				      XmNleftOffset, 0,
				      XmNleftWidget, label16,
				      XmNtopAttachment, XmATTACH_WIDGET,
				      XmNtopOffset, 5,
				      XmNtopWidget, label16,
			 RES_CONVERT(XmNlabelString, "Database Key (Name)"),
				      NULL);
    UxPutContext(label17, (char *) UxImageStorageAccessFormContext);


    /* Creation of label18 */
    label18 = XtVaCreateManagedWidget("label18",
				      xmLabelWidgetClass,
				      imageStorageAccessForm,
				      XmNx, 50,
				      XmNy, 330,
				      XmNwidth, 250,
				      XmNheight, 40,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				      XmNleftOffset, 0,
				      XmNleftWidget, label16,
				      XmNtopAttachment, XmATTACH_WIDGET,
				      XmNtopOffset, 5,
				      XmNtopWidget, label17,
		 RES_CONVERT(XmNlabelString, "Owner (AE Title - optional)"),
				      NULL);
    UxPutContext(label18, (char *) UxImageStorageAccessFormContext);


    /* Creation of label19 */
    label19 = XtVaCreateManagedWidget("label19",
				      xmLabelWidgetClass,
				      imageStorageAccessForm,
				      XmNx, 50,
				      XmNy, 380,
				      XmNwidth, 250,
				      XmNheight, 40,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				      XmNleftOffset, 0,
				      XmNleftWidget, label16,
				      XmNtopAttachment, XmATTACH_WIDGET,
				      XmNtopOffset, 5,
				      XmNtopWidget, label18,
		       RES_CONVERT(XmNlabelString, "Group Name (optional)"),
				      NULL);
    UxPutContext(label19, (char *) UxImageStorageAccessFormContext);


    /* Creation of label20 */
    label20 = XtVaCreateManagedWidget("label20",
				      xmLabelWidgetClass,
				      imageStorageAccessForm,
				      XmNx, 50,
				      XmNy, 450,
				      XmNwidth, 250,
				      XmNheight, 40,
				      XmNtopAttachment, XmATTACH_WIDGET,
				      XmNtopOffset, 30,
				      XmNtopWidget, label19,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				      XmNleftOffset, 0,
				      XmNleftWidget, label16,
				      RES_CONVERT(XmNlabelString, "Access"),
				      NULL);
    UxPutContext(label20, (char *) UxImageStorageAccessFormContext);


    /* Creation of label21 */
    label21 = XtVaCreateManagedWidget("label21",
				      xmLabelWidgetClass,
				      imageStorageAccessForm,
				      XmNx, 50,
				      XmNy, 530,
				      XmNwidth, 250,
				      XmNheight, 40,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				      XmNleftOffset, 0,
				      XmNleftWidget, label16,
				      XmNtopAttachment, XmATTACH_WIDGET,
				      XmNtopOffset, 35,
				      XmNtopWidget, label20,
				      RES_CONVERT(XmNlabelString, "Comment"),
				      NULL);
    UxPutContext(label21, (char *) UxImageStorageAccessFormContext);


    /* Creation of wApplicationTitleTxt */
    wApplicationTitleTxt = XtVaCreateManagedWidget("wApplicationTitleTxt",
						   xmTextFieldWidgetClass,
						   imageStorageAccessForm,
						   XmNwidth, 390,
						   XmNx, 400,
						   XmNy, 210,
						   XmNheight, 40,
					 XmNleftAttachment, XmATTACH_WIDGET,
						   XmNleftOffset, 15,
						   XmNleftWidget, label16,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
						   XmNtopOffset, 0,
						   XmNtopWidget, label16,
						   NULL);
    UxPutContext(wApplicationTitleTxt, (char *) UxImageStorageAccessFormContext);


    /* Creation of wDatabaseKeyTxt */
    wDatabaseKeyTxt = XtVaCreateManagedWidget("wDatabaseKeyTxt",
					      xmTextFieldWidgetClass,
					      imageStorageAccessForm,
					      XmNwidth, 390,
					      XmNx, 400,
					      XmNy, 250,
					      XmNheight, 40,
					 XmNleftAttachment, XmATTACH_WIDGET,
					      XmNleftOffset, 15,
					      XmNleftWidget, label16,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					      XmNtopOffset, 0,
					      XmNtopWidget, label17,
					      NULL);
    UxPutContext(wDatabaseKeyTxt, (char *) UxImageStorageAccessFormContext);


    /* Creation of wOwnerTxt */
    wOwnerTxt = XtVaCreateManagedWidget("wOwnerTxt",
					xmTextFieldWidgetClass,
					imageStorageAccessForm,
					XmNwidth, 390,
					XmNx, 400,
					XmNy, 290,
					XmNheight, 40,
					XmNleftAttachment, XmATTACH_WIDGET,
					XmNleftOffset, 15,
					XmNleftWidget, label16,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					XmNtopOffset, 0,
					XmNtopWidget, label18,
					NULL);
    UxPutContext(wOwnerTxt, (char *) UxImageStorageAccessFormContext);


    /* Creation of wGroupNameTxt */
    wGroupNameTxt = XtVaCreateManagedWidget("wGroupNameTxt",
					    xmTextFieldWidgetClass,
					    imageStorageAccessForm,
					    XmNwidth, 390,
					    XmNx, 400,
					    XmNy, 340,
					    XmNheight, 40,
					 XmNleftAttachment, XmATTACH_WIDGET,
					    XmNleftOffset, 15,
					    XmNleftWidget, label16,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					    XmNtopOffset, 0,
					    XmNtopWidget, label19,
					    NULL);
    UxPutContext(wGroupNameTxt, (char *) UxImageStorageAccessFormContext);


    /* Creation of wCommentTxt */
    wCommentTxt = XtVaCreateManagedWidget("wCommentTxt",
					  xmTextFieldWidgetClass,
					  imageStorageAccessForm,
					  XmNwidth, 390,
					  XmNx, 400,
					  XmNy, 480,
					  XmNheight, 40,
					  XmNleftAttachment, XmATTACH_WIDGET,
					  XmNleftOffset, 15,
					  XmNleftWidget, label16,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					  XmNtopOffset, 0,
					  XmNtopWidget, label21,
					  NULL);
    UxPutContext(wCommentTxt, (char *) UxImageStorageAccessFormContext);


    /* Creation of pushButton15 */
    pushButton15 = XtVaCreateManagedWidget("pushButton15",
					   xmPushButtonWidgetClass,
					   imageStorageAccessForm,
					   XmNx, 40,
					   XmNy, 540,
					   XmNwidth, 180,
					   XmNheight, 50,
					 RES_CONVERT(XmNlabelString, "Add"),
					   NULL);
    XtAddCallback(pushButton15, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton15,
		  (XtPointer) UxImageStorageAccessFormContext);

    UxPutContext(pushButton15, (char *) UxImageStorageAccessFormContext);


    /* Creation of pushButton16 */
    pushButton16 = XtVaCreateManagedWidget("pushButton16",
					   xmPushButtonWidgetClass,
					   imageStorageAccessForm,
					   XmNx, 310,
					   XmNy, 540,
					   XmNwidth, 180,
					   XmNheight, 50,
				      RES_CONVERT(XmNlabelString, "Update"),
					   XmNsensitive, FALSE,
					   NULL);
    UxPutContext(pushButton16, (char *) UxImageStorageAccessFormContext);


    /* Creation of pushButton17 */
    pushButton17 = XtVaCreateManagedWidget("pushButton17",
					   xmPushButtonWidgetClass,
					   imageStorageAccessForm,
					   XmNx, 580,
					   XmNy, 540,
					   XmNwidth, 180,
					   XmNheight, 50,
				      RES_CONVERT(XmNlabelString, "Delete"),
					   NULL);
    XtAddCallback(pushButton17, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton17,
		  (XtPointer) UxImageStorageAccessFormContext);

    UxPutContext(pushButton17, (char *) UxImageStorageAccessFormContext);


    XtAddCallback(imageStorageAccessForm, XmNdestroyCallback,
		  (XtCallbackProc) UxDestroyContextCB,
		  (XtPointer) UxImageStorageAccessFormContext);


    return (imageStorageAccessForm);
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget
create_imageStorageAccessForm(swidget _UxUxParent)
{
    Widget rtrn;
    _UxCimageStorageAccessForm *UxContext;

    UxImageStorageAccessFormContext = UxContext =
	(_UxCimageStorageAccessForm *) UxNewContext(sizeof(_UxCimageStorageAccessForm), False);

    UxParent = _UxUxParent;

    rtrn = _Uxbuild_imageStorageAccessForm();

    return (rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/
