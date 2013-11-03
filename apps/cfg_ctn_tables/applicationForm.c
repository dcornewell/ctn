
/*******************************************************************************
	applicationForm.c

       Associated Header file: applicationForm.h
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
** Module Name(s):	create_applicationForm
**			loadApplicationList
** Author, Date:	Steve Moore, Summer 1994
** Intent:		This file provides the user interface
**			for defining and modifying application
**			entities.  The public entry points are
**			call to create the widget hierarchy
**			and to populate the list application
**			entities.  All other functions are
**			local to this specific interface.
** Last Update:         $Author: smm $, $Date: 1995-04-10 13:19:25 $
** Source File:         $RCSfile: applicationForm.c,v $
** Revision:            $Revision: 1.6 $
** Status:              $State: Exp $
*/

static char rcsid[] = "$Revision: 1.6 $ $RCSfile: applicationForm.c,v $";


#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "tbl.h"
#include "manage.h"
#include "mut.h"

#include "format.h"

extern DMAN_HANDLE *dmanHandle;
static LST_HEAD *applicationList = NULL;


/*******************************************************************************
       The following header file defines the context structure.
*******************************************************************************/

#define CONTEXT_MACRO_ACCESS 1
#include "applicationForm.h"
#undef CONTEXT_MACRO_ACCESS


/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

void
loadApplicationList()
{
    CONDITION
    cond;
    DMAN_APPLICATIONENTITY
	ae,
	criteria;
    char
        b[256];

    if (applicationList == NULL)
	applicationList = LST_Create();

    if (applicationList == NULL)
	return;

    memset(&ae, 0, sizeof(ae));
    ae.Type = DMAN_K_APPLICATIONENTITY;
    criteria.Type = DMAN_K_APPLICATIONENTITY;
    criteria.Flag = 0;
    cond = DMAN_Select(&dmanHandle, (DMAN_GENERICRECORD *) & ae,
		       (DMAN_GENERICRECORD *) & criteria,
		       applicationList, NULL, NULL, NULL);
    if (cond != DMAN_NORMAL)
	return;

    (void) LST_Sort(&applicationList, sizeof(ae), compareAE);
    MUT_LoadList(wApplicationList, applicationList, formatAE, b);
}

static void
displayAE(DMAN_APPLICATIONENTITY * ae)
{
    char
        b[20];

    XmTextSetString(wApplicationTitleTxt, ae->Title);
    XmTextSetString(wNodeTxt, ae->Node);
    XmTextSetString(wOrganizationTxt, ae->Organization);
    if (ae->Flag & DMAN_K_APPLICATION_COMMENT)
	XmTextSetString(wCommentTxt, ae->Comment);
    else
	XmTextSetString(wCommentTxt, "");

    if (!ae->Flag & DMAN_K_APPLICATION_PORT)
	b[0] = '\0';
    else
	sprintf(b, "%-d", ae->Port);
    XmTextSetString(wPortTxt, b);
}

static CONDITION
readAE(DMAN_APPLICATIONENTITY * ae)
{
    CONDITION
    cond;
    CTNBOOLEAN
	nullFlag;

    memset(ae, 0, sizeof(*ae));
    ae->Type = DMAN_K_APPLICATIONENTITY;
    ae->Flag = 0;

    cond = MUT_ScanWidget(wApplicationTitleTxt, MUT_TEXT, &nullFlag, ae->Title);
    if (cond != MUT_NORMAL || nullFlag)
	return 0;
    ae->Flag |= DMAN_K_APPLICATION_TITLE;

    cond = MUT_ScanWidget(wNodeTxt, MUT_TEXT, &nullFlag, ae->Node);
    if (cond != MUT_NORMAL || nullFlag)
	return 0;
    ae->Flag |= DMAN_K_APPLICATION_NODE;

    cond = MUT_ScanWidget(wOrganizationTxt, MUT_TEXT, &nullFlag,
			  ae->Organization);
    if (cond != MUT_NORMAL || nullFlag)
	return 0;
    ae->Flag |= DMAN_K_APPLICATION_ORGANIZATION;

    cond = MUT_ScanWidget(wPortTxt, MUT_INT, &nullFlag, &ae->Port);
    if (cond != MUT_NORMAL)
	return 0;
    ae->Flag |= DMAN_K_APPLICATION_PORT;

    cond = MUT_ScanWidget(wCommentTxt, MUT_TEXT, &nullFlag, ae->Comment);
    if (cond != MUT_NORMAL)
	return 0;
    ae->Flag |= DMAN_K_APPLICATION_COMMENT;

    return 1;
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void
activateCB_pushButton1(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    _UxCapplicationForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxApplicationFormContext;
    UxApplicationFormContext = UxContext =
	(_UxCapplicationForm *) UxGetContext(UxWidget);
    {
	DMAN_APPLICATIONENTITY
	    ae;
	CONDITION
	    cond;

	cond = readAE(&ae);
	if (cond != 1)
	    return;

	cond = DMAN_Insert(&dmanHandle, (DMAN_GENERICRECORD *) & ae);
	if (cond != DMAN_NORMAL)
	    COND_DumpConditions();

	loadApplicationList();
    }
    UxApplicationFormContext = UxSaveCtx;
}

static void
activateCB_pushButton3(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    _UxCapplicationForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxApplicationFormContext;
    UxApplicationFormContext = UxContext =
	(_UxCapplicationForm *) UxGetContext(UxWidget);
    {
	DMAN_APPLICATIONENTITY
	    ae;
	CONDITION
	    cond;

	cond = readAE(&ae);
	if (cond != 1)
	    return;

	ae.Flag = DMAN_K_APPLICATION_TITLE;
	cond = DMAN_Delete(&dmanHandle, (DMAN_GENERICRECORD *) & ae);
	if (cond != DMAN_NORMAL)
	    COND_DumpConditions();

	loadApplicationList();
    }
    UxApplicationFormContext = UxSaveCtx;
}

static void
browseSelectionCB_wApplicationList(
				   Widget wgt,
				   XtPointer cd,
				   XtPointer cb)
{
    _UxCapplicationForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxApplicationFormContext;
    UxApplicationFormContext = UxContext =
	(_UxCapplicationForm *) UxGetContext(UxWidget);
    {
	XmListCallbackStruct *l;
	DMAN_APPLICATIONENTITY *ae;

	l = (XmListCallbackStruct *) UxCallbackArg;
	ae = LST_Index(&applicationList, l->item_position);

	displayAE(ae);
    }
    UxApplicationFormContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget
_Uxbuild_applicationForm()
{
    Widget _UxParent;


    /* Creation of applicationForm */
    _UxParent = UxParent;
    if (_UxParent == NULL) {
	_UxParent = XtVaCreatePopupShell("applicationForm_shell",
				       topLevelShellWidgetClass, UxTopLevel,
					 XmNx, 74,
					 XmNy, 139,
					 XmNwidth, 746,
					 XmNheight, 549,
					 XmNshellUnitType, XmPIXELS,
					 XmNtitle, "applicationForm",
					 XmNiconName, "applicationForm",
					 NULL);

    }
    applicationForm = XtVaCreateManagedWidget("applicationForm",
					      xmFormWidgetClass,
					      _UxParent,
					      XmNwidth, 746,
					      XmNheight, 549,
					      XmNresizePolicy, XmRESIZE_NONE,
					      XmNunitType, XmPIXELS,
					      NULL);
    UxPutContext(applicationForm, (char *) UxApplicationFormContext);


    /* Creation of label1 */
    label1 = XtVaCreateManagedWidget("label1",
				     xmLabelWidgetClass,
				     applicationForm,
				     XmNx, 50,
				     XmNy, 200,
				     XmNwidth, 200,
				     XmNheight, 40,
			   RES_CONVERT(XmNlabelString, "Application Title"),
				     NULL);
    UxPutContext(label1, (char *) UxApplicationFormContext);


    /* Creation of label2 */
    label2 = XtVaCreateManagedWidget("label2",
				     xmLabelWidgetClass,
				     applicationForm,
				     XmNx, 50,
				     XmNy, 240,
				     XmNwidth, 200,
				     XmNheight, 40,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				     XmNleftOffset, 0,
				     XmNleftWidget, label1,
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 15,
				     XmNtopWidget, label1,
				     RES_CONVERT(XmNlabelString, "Node"),
				     NULL);
    UxPutContext(label2, (char *) UxApplicationFormContext);


    /* Creation of wApplicationTitleTxt */
    wApplicationTitleTxt = XtVaCreateManagedWidget("wApplicationTitleTxt",
						   xmTextFieldWidgetClass,
						   applicationForm,
						   XmNwidth, 370,
						   XmNx, 280,
						   XmNy, 210,
						   XmNheight, 40,
					 XmNleftAttachment, XmATTACH_WIDGET,
						   XmNleftOffset, 10,
						   XmNleftWidget, label1,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
						   XmNtopOffset, 0,
						   XmNtopWidget, label1,
						   NULL);
    UxPutContext(wApplicationTitleTxt, (char *) UxApplicationFormContext);


    /* Creation of wNodeTxt */
    wNodeTxt = XtVaCreateManagedWidget("wNodeTxt",
				       xmTextFieldWidgetClass,
				       applicationForm,
				       XmNwidth, 370,
				       XmNx, 280,
				       XmNy, 250,
				       XmNheight, 40,
				       XmNleftAttachment, XmATTACH_WIDGET,
				       XmNleftOffset, 10,
				       XmNleftWidget, label1,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
				       XmNtopOffset, 0,
				       XmNtopWidget, label2,
				       NULL);
    UxPutContext(wNodeTxt, (char *) UxApplicationFormContext);


    /* Creation of pushButton1 */
    pushButton1 = XtVaCreateManagedWidget("pushButton1",
					  xmPushButtonWidgetClass,
					  applicationForm,
					  XmNx, 50,
					  XmNy, 480,
					  XmNwidth, 160,
					  XmNheight, 50,
					  RES_CONVERT(XmNlabelString, "Add"),
					  NULL);
    XtAddCallback(pushButton1, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton1,
		  (XtPointer) UxApplicationFormContext);

    UxPutContext(pushButton1, (char *) UxApplicationFormContext);


    /* Creation of pushButton2 */
    pushButton2 = XtVaCreateManagedWidget("pushButton2",
					  xmPushButtonWidgetClass,
					  applicationForm,
					  XmNx, 300,
					  XmNy, 480,
					  XmNwidth, 160,
					  XmNheight, 50,
				      RES_CONVERT(XmNlabelString, "Update"),
					  XmNsensitive, FALSE,
					  NULL);
    UxPutContext(pushButton2, (char *) UxApplicationFormContext);


    /* Creation of pushButton3 */
    pushButton3 = XtVaCreateManagedWidget("pushButton3",
					  xmPushButtonWidgetClass,
					  applicationForm,
					  XmNx, 530,
					  XmNy, 480,
					  XmNwidth, 160,
					  XmNheight, 50,
				      RES_CONVERT(XmNlabelString, "Delete"),
					  NULL);
    XtAddCallback(pushButton3, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton3,
		  (XtPointer) UxApplicationFormContext);

    UxPutContext(pushButton3, (char *) UxApplicationFormContext);


    /* Creation of frame1 */
    frame1 = XtVaCreateManagedWidget("frame1",
				     xmFrameWidgetClass,
				     applicationForm,
				     XmNwidth, 710,
				     XmNheight, 160,
				     XmNx, 30,
				     XmNy, 20,
				     XmNresizable, FALSE,
				     NULL);
    UxPutContext(frame1, (char *) UxApplicationFormContext);


    /* Creation of scrolledWindowList1 */
    scrolledWindowList1 = XtVaCreateManagedWidget("scrolledWindowList1",
						xmScrolledWindowWidgetClass,
						  frame1,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNshadowThickness, 0,
						  XmNx, 90,
						  XmNy, 60,
						  NULL);
    UxPutContext(scrolledWindowList1, (char *) UxApplicationFormContext);


    /* Creation of wApplicationList */
    wApplicationList = XtVaCreateManagedWidget("wApplicationList",
					       xmListWidgetClass,
					       scrolledWindowList1,
					       XmNwidth, 250,
					       XmNheight, 50,
					       NULL);
    XtAddCallback(wApplicationList, XmNbrowseSelectionCallback,
		  (XtCallbackProc) browseSelectionCB_wApplicationList,
		  (XtPointer) UxApplicationFormContext);

    UxPutContext(wApplicationList, (char *) UxApplicationFormContext);


    /* Creation of label29 */
    label29 = XtVaCreateManagedWidget("label29",
				      xmLabelWidgetClass,
				      applicationForm,
				      XmNx, 50,
				      XmNy, 310,
				      XmNwidth, 200,
				      XmNheight, 40,
				RES_CONVERT(XmNlabelString, "Organization"),
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				      XmNleftOffset, 0,
				      XmNleftWidget, label1,
				      XmNtopAttachment, XmATTACH_WIDGET,
				      XmNtopOffset, 15,
				      XmNtopWidget, label2,
				      NULL);
    UxPutContext(label29, (char *) UxApplicationFormContext);


    /* Creation of label3 */
    label3 = XtVaCreateManagedWidget("label3",
				     xmLabelWidgetClass,
				     applicationForm,
				     XmNx, 50,
				     XmNy, 270,
				     XmNwidth, 200,
				     XmNheight, 40,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				     XmNleftOffset, 0,
				     XmNleftWidget, label1,
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 15,
				     XmNtopWidget, label29,
			     RES_CONVERT(XmNlabelString, "Port (Optional)"),
				     NULL);
    UxPutContext(label3, (char *) UxApplicationFormContext);


    /* Creation of label4 */
    label4 = XtVaCreateManagedWidget("label4",
				     xmLabelWidgetClass,
				     applicationForm,
				     XmNx, 50,
				     XmNy, 310,
				     XmNwidth, 200,
				     XmNheight, 40,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				     XmNleftOffset, 0,
				     XmNleftWidget, label1,
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 15,
				     XmNtopWidget, label3,
			  RES_CONVERT(XmNlabelString, "Comment (Optional)"),
				     NULL);
    UxPutContext(label4, (char *) UxApplicationFormContext);


    /* Creation of wPortTxt */
    wPortTxt = XtVaCreateManagedWidget("wPortTxt",
				       xmTextFieldWidgetClass,
				       applicationForm,
				       XmNwidth, 370,
				       XmNx, 280,
				       XmNy, 290,
				       XmNheight, 40,
				       XmNleftAttachment, XmATTACH_WIDGET,
				       XmNleftOffset, 10,
				       XmNleftWidget, label1,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
				       XmNtopOffset, 0,
				       XmNtopWidget, label3,
				       NULL);
    UxPutContext(wPortTxt, (char *) UxApplicationFormContext);


    /* Creation of wCommentTxt */
    wCommentTxt = XtVaCreateManagedWidget("wCommentTxt",
					  xmTextFieldWidgetClass,
					  applicationForm,
					  XmNwidth, 370,
					  XmNx, 280,
					  XmNy, 330,
					  XmNheight, 40,
					  XmNleftAttachment, XmATTACH_WIDGET,
					  XmNleftOffset, 10,
					  XmNleftWidget, label1,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					  XmNtopOffset, 0,
					  XmNtopWidget, label4,
					  NULL);
    UxPutContext(wCommentTxt, (char *) UxApplicationFormContext);


    /* Creation of wOrganizationTxt */
    wOrganizationTxt = XtVaCreateManagedWidget("wOrganizationTxt",
					       xmTextFieldWidgetClass,
					       applicationForm,
					       XmNwidth, 370,
					       XmNx, 240,
					       XmNy, 300,
					       XmNheight, 40,
					 XmNleftAttachment, XmATTACH_WIDGET,
					       XmNleftOffset, 10,
					       XmNleftWidget, label1,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					       XmNtopOffset, 0,
					       XmNtopWidget, label29,
					       NULL);
    UxPutContext(wOrganizationTxt, (char *) UxApplicationFormContext);


    XtAddCallback(applicationForm, XmNdestroyCallback,
		  (XtCallbackProc) UxDestroyContextCB,
		  (XtPointer) UxApplicationFormContext);


    return (applicationForm);
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget
create_applicationForm(swidget _UxUxParent)
{
    Widget rtrn;
    _UxCapplicationForm *UxContext;

    UxApplicationFormContext = UxContext =
	(_UxCapplicationForm *) UxNewContext(sizeof(_UxCapplicationForm), False);

    UxParent = _UxUxParent;

    rtrn = _Uxbuild_applicationForm();

    return (rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/
