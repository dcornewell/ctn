
/*******************************************************************************
	printServerCFGForm.c

       Associated Header file: printServerCFGForm.h
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
** Module Name(s):	create_printServerCFGForm
**			loadPrintServerCFGList
** Author, Date:	Steve Moore, Summer 1994
** Intent:		This file provides the user interface
**			for defining and modifying the print server
**			configuration table.  The public entry points are
**			call to create the widget hierarchy
**			and to populate the list of print configuration
**			entries.  All other functions are
** 			local to this specific interface.
** Last Update:         $Author: smm $, $Date: 1995-04-10 13:19:58 $
** Source File:         $RCSfile: printServerCFGForm.c,v $
** Revision:            $Revision: 1.3 $
** Status:              $State: Exp $
*/



#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "manage.h"
#include "mut.h"

#include "format.h"

extern DMAN_HANDLE *dmanHandle;
static LST_HEAD *printServerCFGList = NULL;

void loadPrintServerCFGList();
static void displayPrintServerCFG(DMAN_PRINTSERVERCFG * p);
static CONDITION readPrintServerCFG(DMAN_PRINTSERVERCFG * p);


/*******************************************************************************
       The following header file defines the context structure.
*******************************************************************************/

#define CONTEXT_MACRO_ACCESS 1
#include "printServerCFGForm.h"
#undef CONTEXT_MACRO_ACCESS


/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

void
loadPrintServerCFGList()
{
    CONDITION
    cond;
    DMAN_PRINTSERVERCFG
	p,
	criteria;
    char
        b[128];

    if (printServerCFGList == NULL)
	printServerCFGList = LST_Create();
    if (printServerCFGList == NULL)
	return;

    memset(&p, 0, sizeof(p));
    p.Type = DMAN_K_PRINTSERVERCFG;
    criteria.Type = DMAN_K_PRINTSERVERCFG;
    criteria.Flag = 0;
    cond = DMAN_Select(&dmanHandle, (DMAN_GENERICRECORD *) & p,
		       (DMAN_GENERICRECORD *) & criteria,
		       printServerCFGList, NULL, NULL, NULL);
    if (cond != DMAN_NORMAL) {
	COND_DumpConditions();
	return;
    }
    MUT_LoadList(wPrintServerCFGList, printServerCFGList,
		 formatPrintServerCFG, b);
}

static void
displayPrintServerCFG(DMAN_PRINTSERVERCFG * p)
{
    char
        buf[16];

    if (p == NULL)
	return;
    if (p->Type != DMAN_K_PRINTSERVERCFG)
	return;

    XmTextSetString(wRequestingTxt, p->RequestingTitle);
    XmTextSetString(wRespondingTxt, p->RespondingTitle);
    (void) sprintf(buf, "%-6d", p->GQId);
    XmTextSetString(wQueueIDTxt, buf);
}

static CONDITION
readPrintServerCFG(DMAN_PRINTSERVERCFG * p)
{
    CONDITION
    cond;
    CTNBOOLEAN
	nullFlag;

    memset(p, 0, sizeof(*p));
    p->Type = DMAN_K_PRINTSERVERCFG;

    cond = MUT_ScanWidget(wRequestingTxt, MUT_TEXT, &nullFlag,
			  p->RequestingTitle);
    if (cond != MUT_NORMAL || nullFlag)
	return 0;
    p->Flag |= DMAN_K_PRINTSERVER_REQUESTING;

    cond = MUT_ScanWidget(wRespondingTxt, MUT_TEXT, &nullFlag,
			  p->RespondingTitle);
    if (cond != MUT_NORMAL || nullFlag)
	return 0;
    p->Flag |= DMAN_K_PRINTSERVER_RESPONDING;

    cond = MUT_ScanWidget(wQueueIDTxt, MUT_INT, &nullFlag,
			  &p->GQId);
    if (cond != MUT_NORMAL || nullFlag)
	return 0;
    p->Flag |= DMAN_K_PRINTSERVER_GQID;

    return 1;
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void
activateCB_pushButton7(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    _UxCprintServerCFGForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxPrintServerCFGFormContext;
    UxPrintServerCFGFormContext = UxContext =
	(_UxCprintServerCFGForm *) UxGetContext(UxWidget);
    {
	DMAN_PRINTSERVERCFG
	    p;
	CONDITION
	    cond;

	cond = readPrintServerCFG(&p);
	if (cond != 1) {
	    COND_DumpConditions();
	    return;
	}
	cond = DMAN_Insert(&dmanHandle, (DMAN_GENERICRECORD *) & p);
	if (cond != DMAN_NORMAL)
	    COND_DumpConditions();

	loadPrintServerCFGList();
    }
    UxPrintServerCFGFormContext = UxSaveCtx;
}

static void
activateCB_pushButton9(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    _UxCprintServerCFGForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxPrintServerCFGFormContext;
    UxPrintServerCFGFormContext = UxContext =
	(_UxCprintServerCFGForm *) UxGetContext(UxWidget);
    {
	DMAN_PRINTSERVERCFG
	    p;
	CONDITION
	    cond;

	cond = readPrintServerCFG(&p);
	if (cond != 1) {
	    COND_DumpConditions();
	    return;
	}
	cond = DMAN_Delete(&dmanHandle, (DMAN_GENERICRECORD *) & p);
	if (cond != DMAN_NORMAL)
	    COND_DumpConditions();

	loadPrintServerCFGList();
    }
    UxPrintServerCFGFormContext = UxSaveCtx;
}

static void
browseSelectionCB_wPrintServerCFGList(
				      Widget wgt,
				      XtPointer cd,
				      XtPointer cb)
{
    _UxCprintServerCFGForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxPrintServerCFGFormContext;
    UxPrintServerCFGFormContext = UxContext =
	(_UxCprintServerCFGForm *) UxGetContext(UxWidget);
    {
	XmListCallbackStruct *l;
	DMAN_PRINTSERVERCFG *p;

	l = (XmListCallbackStruct *) UxCallbackArg;
	p = LST_Index(&printServerCFGList, l->item_position);
	displayPrintServerCFG(p);
    }
    UxPrintServerCFGFormContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget
_Uxbuild_printServerCFGForm()
{
    Widget _UxParent;


    /* Creation of printServerCFGForm */
    _UxParent = UxParent;
    if (_UxParent == NULL) {
	_UxParent = XtVaCreatePopupShell("printServerCFGForm_shell",
				       topLevelShellWidgetClass, UxTopLevel,
					 XmNx, 240,
					 XmNy, 360,
					 XmNwidth, 570,
					 XmNheight, 460,
					 XmNshellUnitType, XmPIXELS,
					 XmNtitle, "printServerCFGForm",
					 XmNiconName, "printServerCFGForm",
					 NULL);

    }
    printServerCFGForm = XtVaCreateManagedWidget("printServerCFGForm",
						 xmFormWidgetClass,
						 _UxParent,
						 XmNwidth, 570,
						 XmNheight, 460,
					     XmNresizePolicy, XmRESIZE_NONE,
						 XmNunitType, XmPIXELS,
						 NULL);
    UxPutContext(printServerCFGForm, (char *) UxPrintServerCFGFormContext);


    /* Creation of label7 */
    label7 = XtVaCreateManagedWidget("label7",
				     xmLabelWidgetClass,
				     printServerCFGForm,
				     XmNx, 10,
				     XmNy, 140,
				     XmNwidth, 220,
				     XmNheight, 30,
		       RES_CONVERT(XmNlabelString, "Requesting (external)"),
				     NULL);
    UxPutContext(label7, (char *) UxPrintServerCFGFormContext);


    /* Creation of label8 */
    label8 = XtVaCreateManagedWidget("label8",
				     xmLabelWidgetClass,
				     printServerCFGForm,
				     XmNx, 10,
				     XmNy, 170,
				     XmNwidth, 220,
				     XmNheight, 30,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				     XmNleftOffset, 0,
				     XmNleftWidget, label7,
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 15,
				     XmNtopWidget, label7,
			    RES_CONVERT(XmNlabelString, "Responding (CTN)"),
				     NULL);
    UxPutContext(label8, (char *) UxPrintServerCFGFormContext);


    /* Creation of label9 */
    label9 = XtVaCreateManagedWidget("label9",
				     xmLabelWidgetClass,
				     printServerCFGForm,
				     XmNx, 10,
				     XmNy, 220,
				     XmNwidth, 220,
				     XmNheight, 30,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				     XmNleftOffset, 0,
				     XmNleftWidget, label7,
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 15,
				     XmNtopWidget, label8,
			 RES_CONVERT(XmNlabelString, "Queue ID (a number)"),
				     NULL);
    UxPutContext(label9, (char *) UxPrintServerCFGFormContext);


    /* Creation of wRequestingTxt */
    wRequestingTxt = XtVaCreateManagedWidget("wRequestingTxt",
					     xmTextFieldWidgetClass,
					     printServerCFGForm,
					     XmNwidth, 290,
					     XmNx, 260,
					     XmNy, 100,
					     XmNheight, 40,
					 XmNleftAttachment, XmATTACH_WIDGET,
					     XmNleftOffset, 15,
					     XmNleftWidget, label7,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					     XmNtopOffset, 0,
					     XmNtopWidget, label7,
					     NULL);
    UxPutContext(wRequestingTxt, (char *) UxPrintServerCFGFormContext);


    /* Creation of wRespondingTxt */
    wRespondingTxt = XtVaCreateManagedWidget("wRespondingTxt",
					     xmTextFieldWidgetClass,
					     printServerCFGForm,
					     XmNwidth, 290,
					     XmNx, 260,
					     XmNy, 150,
					     XmNheight, 40,
					 XmNleftAttachment, XmATTACH_WIDGET,
					     XmNleftOffset, 15,
					     XmNleftWidget, label8,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					     XmNtopOffset, 0,
					     XmNtopWidget, label8,
					     NULL);
    UxPutContext(wRespondingTxt, (char *) UxPrintServerCFGFormContext);


    /* Creation of wQueueIDTxt */
    wQueueIDTxt = XtVaCreateManagedWidget("wQueueIDTxt",
					  xmTextFieldWidgetClass,
					  printServerCFGForm,
					  XmNwidth, 290,
					  XmNx, 260,
					  XmNy, 210,
					  XmNheight, 40,
					  XmNleftAttachment, XmATTACH_WIDGET,
					  XmNleftOffset, 15,
					  XmNleftWidget, label7,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					  XmNtopOffset, 0,
					  XmNtopWidget, label9,
					  NULL);
    UxPutContext(wQueueIDTxt, (char *) UxPrintServerCFGFormContext);


    /* Creation of pushButton7 */
    pushButton7 = XtVaCreateManagedWidget("pushButton7",
					  xmPushButtonWidgetClass,
					  printServerCFGForm,
					  XmNx, 40,
					  XmNy, 310,
					  XmNwidth, 130,
					  XmNheight, 40,
					  RES_CONVERT(XmNlabelString, "Add"),
					  NULL);
    XtAddCallback(pushButton7, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton7,
		  (XtPointer) UxPrintServerCFGFormContext);

    UxPutContext(pushButton7, (char *) UxPrintServerCFGFormContext);


    /* Creation of pushButton8 */
    pushButton8 = XtVaCreateManagedWidget("pushButton8",
					  xmPushButtonWidgetClass,
					  printServerCFGForm,
					  XmNx, 230,
					  XmNy, 310,
					  XmNwidth, 130,
					  XmNheight, 40,
				      RES_CONVERT(XmNlabelString, "Update"),
					  XmNsensitive, FALSE,
					  NULL);
    UxPutContext(pushButton8, (char *) UxPrintServerCFGFormContext);


    /* Creation of pushButton9 */
    pushButton9 = XtVaCreateManagedWidget("pushButton9",
					  xmPushButtonWidgetClass,
					  printServerCFGForm,
					  XmNx, 410,
					  XmNy, 310,
					  XmNwidth, 130,
					  XmNheight, 40,
				      RES_CONVERT(XmNlabelString, "Delete"),
					  NULL);
    XtAddCallback(pushButton9, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton9,
		  (XtPointer) UxPrintServerCFGFormContext);

    UxPutContext(pushButton9, (char *) UxPrintServerCFGFormContext);


    /* Creation of frame3 */
    frame3 = XtVaCreateManagedWidget("frame3",
				     xmFrameWidgetClass,
				     printServerCFGForm,
				     XmNwidth, 540,
				     XmNheight, 120,
				     XmNx, 10,
				     XmNy, 10,
				     NULL);
    UxPutContext(frame3, (char *) UxPrintServerCFGFormContext);


    /* Creation of scrolledWindowList3 */
    scrolledWindowList3 = XtVaCreateManagedWidget("scrolledWindowList3",
						xmScrolledWindowWidgetClass,
						  frame3,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNshadowThickness, 0,
						  XmNx, 60,
						  XmNy, 40,
						  NULL);
    UxPutContext(scrolledWindowList3, (char *) UxPrintServerCFGFormContext);


    /* Creation of wPrintServerCFGList */
    wPrintServerCFGList = XtVaCreateManagedWidget("wPrintServerCFGList",
						  xmListWidgetClass,
						  scrolledWindowList3,
						  XmNwidth, 280,
						  XmNheight, 30,
						  NULL);
    XtAddCallback(wPrintServerCFGList, XmNbrowseSelectionCallback,
		  (XtCallbackProc) browseSelectionCB_wPrintServerCFGList,
		  (XtPointer) UxPrintServerCFGFormContext);

    UxPutContext(wPrintServerCFGList, (char *) UxPrintServerCFGFormContext);


    XtAddCallback(printServerCFGForm, XmNdestroyCallback,
		  (XtCallbackProc) UxDestroyContextCB,
		  (XtPointer) UxPrintServerCFGFormContext);


    return (printServerCFGForm);
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget
create_printServerCFGForm(swidget _UxUxParent)
{
    Widget rtrn;
    _UxCprintServerCFGForm *UxContext;

    UxPrintServerCFGFormContext = UxContext =
	(_UxCprintServerCFGForm *) UxNewContext(sizeof(_UxCprintServerCFGForm), False);

    UxParent = _UxUxParent;

    rtrn = _Uxbuild_printServerCFGForm();

    return (rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/
