
/*******************************************************************************
	topLevelShell1.c

       Associated Header file: topLevelShell1.h
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/CascadeB.h>
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>
#include <Xm/TextF.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/DrawingA.h>
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
** Module Name(s):	create_topLevelShell1
** Author, Date:	Steve Moore
** Intent:		This file contains the code the creates and
**			implements the user interface for the query_client
**			program.  This consists of functions that create
**			the widgets that provide the user interface and
**			the (private) callback functions that are executed
**			to service an event (like a button push).
** Last Update:         $Author: smm $, $Date: 1995-03-30 21:44:56 $
** Source File:         $RCSfile: topLevelShell1.c,v $
** Revision:            $Revision: 1.4 $
** Status:              $State: Exp $
*/

static char rcsid[] = "$Revision: 1.4 $ $RCSfile: topLevelShell1.c,v $";

#include <Xm/Text.h>


#include "dicom.h"
#include "lst.h"
#include "condition.h"
#include "mut.h"

#include "query_interface.h"
#include "format.h"

static LST_HEAD *patientList = NULL;
static LST_HEAD *studyList = NULL;
static PATIENT_QUERY_MODULE selectedPatient;
static STUDY_QUERY_MODULE selectedStudy;

static void copyWidgetText(Widget w, char *txt);
static void listClear(LST_HEAD ** lst);


/*******************************************************************************
       The following header file defines the context structure.
*******************************************************************************/

#define CONTEXT_MACRO_ACCESS 1
#include "topLevelShell1.h"
#undef CONTEXT_MACRO_ACCESS


/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

static void
copyWidgetText(Widget w, char *txt)
{
    char
       *t;

    *txt = '\0';
    t = XmTextGetString(w);
    if (t == NULL)
	return;
    (void) sscanf(t, "%s", txt);
}

static void
listClear(LST_HEAD ** lst)
{
    LST_NODE
    * node;

    while ((node = LST_Dequeue(lst)) != NULL)
	free(node);
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void
browseSelectionCB_studyListWidget(
				  Widget wgt,
				  XtPointer cd,
				  XtPointer cb)
{
    _UxCtopLevelShell1 *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxTopLevelShell1Context;
    UxTopLevelShell1Context = UxContext =
	(_UxCtopLevelShell1 *) UxGetContext(UxWidget);
    {
	XmListCallbackStruct *listStruct;
	STUDY_QUERY_MODULE
	    * s;
	int
	    i;

	listStruct = (XmListCallbackStruct *) UxCallbackArg;
	s = LST_Head(&studyList);
	(void) LST_Position(&studyList, s);

	for (i = 2; i <= listStruct->item_position; i++)
	    s = LST_Next(&studyList);

	selectedStudy = *s;
    }
    UxTopLevelShell1Context = UxSaveCtx;
}

static void
browseSelectionCB_patientListWidget(
				    Widget wgt,
				    XtPointer cd,
				    XtPointer cb)
{
    _UxCtopLevelShell1 *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxTopLevelShell1Context;
    UxTopLevelShell1Context = UxContext =
	(_UxCtopLevelShell1 *) UxGetContext(UxWidget);
    {
	XmListCallbackStruct *listStruct;
	PATIENT_QUERY_MODULE
	    * p;
	int
	    i;
	CONDITION
	    cond;
	char
	   *txt;
	CONNECTION_MODULE
	    connection;
	char
	    buf[80];

	listStruct = (XmListCallbackStruct *) UxCallbackArg;
	p = LST_Head(&patientList);
	(void) LST_Position(&patientList, p);

	for (i = 2; i <= listStruct->item_position; i++)
	    p = LST_Next(&patientList);

	printf("%s\n", p->PatientID);

	copyWidgetText(imageServerNodeTxt, connection.NodeName);
	copyWidgetText(imageServerPortTxt, connection.PortNumber);
	copyWidgetText(calledAPTitleTxt, connection.CalledAPTitle);
	copyWidgetText(callingAPTitleTxt, connection.CallingAPTitle);

	if (studyList == NULL)
	    studyList = LST_Create();

	if (studyList == NULL)
	    return;

	listClear(&studyList);
	cond = studyQuery(&connection, p, &studyList);
	COND_DumpConditions();
	MUT_LoadList(studyListWidget, studyList, formatStudy, buf);

	selectedPatient = *p;
    }
    UxTopLevelShell1Context = UxSaveCtx;
}

static void
activateCB_moveButton(
		      Widget wgt,
		      XtPointer cd,
		      XtPointer cb)
{
    _UxCtopLevelShell1 *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxTopLevelShell1Context;
    UxTopLevelShell1Context = UxContext =
	(_UxCtopLevelShell1 *) UxGetContext(UxWidget);
    {
	CONNECTION_MODULE
	    connection;
	char
	    moveDestination[20];
	CONDITION
	    cond;

	copyWidgetText(imageServerNodeTxt, connection.NodeName);
	copyWidgetText(imageServerPortTxt, connection.PortNumber);
	copyWidgetText(calledAPTitleTxt, connection.CalledAPTitle);
	copyWidgetText(callingAPTitleTxt, connection.CallingAPTitle);

	copyWidgetText(destinationAPTitleTxt, moveDestination);

	cond = moveRequest(&connection, moveDestination,
			   &selectedPatient, &selectedStudy);
	COND_DumpConditions();
    }
    UxTopLevelShell1Context = UxSaveCtx;
}

static void
activateCB_pushButton1(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    _UxCtopLevelShell1 *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxTopLevelShell1Context;
    UxTopLevelShell1Context = UxContext =
	(_UxCtopLevelShell1 *) UxGetContext(UxWidget);
    {
	CONDITION
	    cond;
	char
	   *txt;
	CONNECTION_MODULE
	    connection;
	PATIENT_QUERY_MODULE
	    query;
	char
	    buf[80];

	copyWidgetText(imageServerNodeTxt, connection.NodeName);
	copyWidgetText(imageServerPortTxt, connection.PortNumber);
	copyWidgetText(calledAPTitleTxt, connection.CalledAPTitle);
	copyWidgetText(callingAPTitleTxt, connection.CallingAPTitle);

	copyWidgetText(patientIDTxt, query.PatientID);
	copyWidgetText(lastNameTxt, query.LastName);
	copyWidgetText(birthDateTxt, query.BirthDate);

	if (patientList == NULL)
	    patientList = LST_Create();
	if (studyList == NULL)
	    studyList = LST_Create();

	listClear(&patientList);
	cond = patientQuery(&connection, &query, &patientList);
	COND_DumpConditions();
	MUT_LoadList(patientListWidget, patientList, formatPatient, buf);

	listClear(&studyList);
	MUT_LoadList(studyListWidget, studyList, formatStudy, buf);
    }
    UxTopLevelShell1Context = UxSaveCtx;
}

static void
activateCB_menu1_p1_b1(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    _UxCtopLevelShell1 *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxTopLevelShell1Context;
    UxTopLevelShell1Context = UxContext =
	(_UxCtopLevelShell1 *) UxGetContext(UxWidget);
    {
	exit(0);
    }
    UxTopLevelShell1Context = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget
_Uxbuild_topLevelShell1()
{
    Widget _UxParent;
    Widget menu1_p1_shell;


    /* Creation of topLevelShell1 */
    _UxParent = UxParent;
    if (_UxParent == NULL) {
	_UxParent = UxTopLevel;
    }
    topLevelShell1 = XtVaCreatePopupShell("topLevelShell1",
					  topLevelShellWidgetClass,
					  _UxParent,
					  XmNwidth, 980,
					  XmNheight, 470,
					  XmNx, 53,
					  XmNy, 223,
					  NULL);
    UxPutContext(topLevelShell1, (char *) UxTopLevelShell1Context);


    /* Creation of mainWindow1 */
    mainWindow1 = XtVaCreateManagedWidget("mainWindow1",
					  xmMainWindowWidgetClass,
					  topLevelShell1,
					  XmNwidth, 980,
					  XmNheight, 470,
					  XmNx, 120,
					  XmNy, 20,
					  XmNunitType, XmPIXELS,
					  NULL);
    UxPutContext(mainWindow1, (char *) UxTopLevelShell1Context);


    /* Creation of drawingArea1 */
    drawingArea1 = XtVaCreateManagedWidget("drawingArea1",
					   xmDrawingAreaWidgetClass,
					   mainWindow1,
					   NULL);
    UxPutContext(drawingArea1, (char *) UxTopLevelShell1Context);


    /* Creation of scrolledWindowList2 */
    scrolledWindowList2 = XtVaCreateManagedWidget("scrolledWindowList2",
						xmScrolledWindowWidgetClass,
						  drawingArea1,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNshadowThickness, 0,
						  XmNx, 520,
						  XmNy, 220,
						  NULL);
    UxPutContext(scrolledWindowList2, (char *) UxTopLevelShell1Context);


    /* Creation of studyListWidget */
    studyListWidget = XtVaCreateManagedWidget("studyListWidget",
					      xmListWidgetClass,
					      scrolledWindowList2,
					      XmNwidth, 420,
					      XmNheight, 180,
					      XmNlistSizePolicy, XmCONSTANT,
					      XmNfontList, UxConvertFontList("-misc-fixed-medium-r-normal--14-130-75-75-c-70-iso8859-1"),
					      NULL);
    XtAddCallback(studyListWidget, XmNbrowseSelectionCallback,
		  (XtCallbackProc) browseSelectionCB_studyListWidget,
		  (XtPointer) UxTopLevelShell1Context);

    UxPutContext(studyListWidget, (char *) UxTopLevelShell1Context);


    /* Creation of scrolledWindowList1 */
    scrolledWindowList1 = XtVaCreateManagedWidget("scrolledWindowList1",
						xmScrolledWindowWidgetClass,
						  drawingArea1,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNshadowThickness, 0,
						  XmNx, 10,
						  XmNy, 220,
						  NULL);
    UxPutContext(scrolledWindowList1, (char *) UxTopLevelShell1Context);


    /* Creation of patientListWidget */
    patientListWidget = XtVaCreateManagedWidget("patientListWidget",
						xmListWidgetClass,
						scrolledWindowList1,
						XmNwidth, 420,
						XmNheight, 180,
					      XmNlistSizePolicy, XmCONSTANT,
						XmNfontList, UxConvertFontList("-misc-fixed-medium-r-normal--14-130-75-75-c-70-iso8859-1"),
						NULL);
    XtAddCallback(patientListWidget, XmNbrowseSelectionCallback,
		  (XtCallbackProc) browseSelectionCB_patientListWidget,
		  (XtPointer) UxTopLevelShell1Context);

    UxPutContext(patientListWidget, (char *) UxTopLevelShell1Context);


    /* Creation of frame3 */
    frame3 = XtVaCreateManagedWidget("frame3",
				     xmFrameWidgetClass,
				     drawingArea1,
				     XmNwidth, 320,
				     XmNheight, 190,
				     XmNx, 650,
				     XmNy, 0,
				     NULL);
    UxPutContext(frame3, (char *) UxTopLevelShell1Context);


    /* Creation of form3 */
    form3 = XtVaCreateManagedWidget("form3",
				    xmFormWidgetClass,
				    frame3,
				    XmNwidth, 250,
				    XmNheight, 140,
				    XmNresizePolicy, XmRESIZE_NONE,
				    XmNx, 20,
				    XmNy, 20,
				    NULL);
    UxPutContext(form3, (char *) UxTopLevelShell1Context);


    /* Creation of label11 */
    label11 = XtVaCreateManagedWidget("label11",
				      xmLabelWidgetClass,
				      form3,
				      XmNx, 20,
				      XmNy, 50,
				      XmNwidth, 150,
				      XmNheight, 20,
			RES_CONVERT(XmNlabelString, "Destination AP Title"),
				      NULL);
    UxPutContext(label11, (char *) UxTopLevelShell1Context);


    /* Creation of destinationAPTitleTxt */
    destinationAPTitleTxt = XtVaCreateManagedWidget("destinationAPTitleTxt",
						    xmTextFieldWidgetClass,
						    form3,
						    XmNwidth, 130,
						    XmNx, 180,
						    XmNy, 40,
						    XmNheight, 40,
						    XmNfontList, UxConvertFontList("-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1"),
						    NULL);
    UxPutContext(destinationAPTitleTxt, (char *) UxTopLevelShell1Context);


    /* Creation of moveButton */
    moveButton = XtVaCreateManagedWidget("moveButton",
					 xmPushButtonWidgetClass,
					 form3,
					 XmNx, 30,
					 XmNy, 150,
					 XmNwidth, 280,
					 XmNheight, 30,
					 RES_CONVERT(XmNlabelString, "Move"),
					 NULL);
    XtAddCallback(moveButton, XmNactivateCallback,
		  (XtCallbackProc) activateCB_moveButton,
		  (XtPointer) UxTopLevelShell1Context);

    UxPutContext(moveButton, (char *) UxTopLevelShell1Context);


    /* Creation of frame1 */
    frame1 = XtVaCreateManagedWidget("frame1",
				     xmFrameWidgetClass,
				     drawingArea1,
				     XmNwidth, 320,
				     XmNheight, 190,
				     XmNx, 330,
				     XmNy, 0,
				     NULL);
    UxPutContext(frame1, (char *) UxTopLevelShell1Context);


    /* Creation of form2 */
    form2 = XtVaCreateManagedWidget("form2",
				    xmFormWidgetClass,
				    frame1,
				    XmNwidth, 180,
				    XmNheight, 120,
				    XmNresizePolicy, XmRESIZE_NONE,
				    XmNx, 40,
				    XmNy, 20,
				    NULL);
    UxPutContext(form2, (char *) UxTopLevelShell1Context);


    /* Creation of label6 */
    label6 = XtVaCreateManagedWidget("label6",
				     xmLabelWidgetClass,
				     form2,
				     XmNx, 0,
				     XmNy, 30,
				     XmNwidth, 150,
				     XmNheight, 20,
				  RES_CONVERT(XmNlabelString, "Patient ID"),
				     NULL);
    UxPutContext(label6, (char *) UxTopLevelShell1Context);


    /* Creation of label7 */
    label7 = XtVaCreateManagedWidget("label7",
				     xmLabelWidgetClass,
				     form2,
				     XmNx, 22,
				     XmNy, 82,
				     XmNwidth, 150,
				     XmNheight, 20,
				   RES_CONVERT(XmNlabelString, "Last Name"),
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 20,
				     XmNtopWidget, label6,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				     XmNleftOffset, 0,
				     XmNleftWidget, label6,
				     NULL);
    UxPutContext(label7, (char *) UxTopLevelShell1Context);


    /* Creation of label8 */
    label8 = XtVaCreateManagedWidget("label8",
				     xmLabelWidgetClass,
				     form2,
				     XmNx, 20,
				     XmNy, 110,
				     XmNwidth, 150,
				     XmNheight, 20,
				  RES_CONVERT(XmNlabelString, "Birth Date"),
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 20,
				     XmNtopWidget, label7,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				     XmNleftOffset, 0,
				     XmNleftWidget, label6,
				     NULL);
    UxPutContext(label8, (char *) UxTopLevelShell1Context);


    /* Creation of patientIDTxt */
    patientIDTxt = XtVaCreateManagedWidget("patientIDTxt",
					   xmTextFieldWidgetClass,
					   form2,
					   XmNwidth, 140,
					   XmNx, 150,
					   XmNy, 30,
					   XmNheight, 35,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					   XmNtopOffset, 0,
					   XmNtopWidget, label6,
					   XmNfontList, UxConvertFontList("-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1"),
					   NULL);
    UxPutContext(patientIDTxt, (char *) UxTopLevelShell1Context);


    /* Creation of lastNameTxt */
    lastNameTxt = XtVaCreateManagedWidget("lastNameTxt",
					  xmTextFieldWidgetClass,
					  form2,
					  XmNwidth, 140,
					  XmNx, 150,
					  XmNy, 79,
					  XmNheight, 35,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					  XmNtopOffset, 0,
					  XmNtopWidget, label7,
					  XmNfontList, UxConvertFontList("-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1"),
					  NULL);
    UxPutContext(lastNameTxt, (char *) UxTopLevelShell1Context);


    /* Creation of birthDateTxt */
    birthDateTxt = XtVaCreateManagedWidget("birthDateTxt",
					   xmTextFieldWidgetClass,
					   form2,
					   XmNwidth, 140,
					   XmNx, 150,
					   XmNy, 128,
					   XmNheight, 35,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					   XmNtopOffset, 0,
					   XmNtopWidget, label8,
					   XmNfontList, UxConvertFontList("-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1"),
					   NULL);
    UxPutContext(birthDateTxt, (char *) UxTopLevelShell1Context);


    /* Creation of pushButton1 */
    pushButton1 = XtVaCreateManagedWidget("pushButton1",
					  xmPushButtonWidgetClass,
					  form2,
					  XmNx, 38,
					  XmNy, 148,
					  XmNwidth, 250,
					  XmNheight, 30,
				      RES_CONVERT(XmNlabelString, "Search"),
					  NULL);
    XtAddCallback(pushButton1, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton1,
		  (XtPointer) UxTopLevelShell1Context);

    UxPutContext(pushButton1, (char *) UxTopLevelShell1Context);


    /* Creation of frame2 */
    frame2 = XtVaCreateManagedWidget("frame2",
				     xmFrameWidgetClass,
				     drawingArea1,
				     XmNwidth, 320,
				     XmNheight, 190,
				     XmNx, 10,
				     XmNy, 0,
				     NULL);
    UxPutContext(frame2, (char *) UxTopLevelShell1Context);


    /* Creation of form1 */
    form1 = XtVaCreateManagedWidget("form1",
				    xmFormWidgetClass,
				    frame2,
				    XmNwidth, 290,
				    XmNheight, 166,
				    XmNresizePolicy, XmRESIZE_NONE,
				    XmNx, 10,
				    XmNy, 20,
				    NULL);
    UxPutContext(form1, (char *) UxTopLevelShell1Context);


    /* Creation of label2 */
    label2 = XtVaCreateManagedWidget("label2",
				     xmLabelWidgetClass,
				     form1,
				     XmNx, 10,
				     XmNy, 20,
				     XmNwidth, 120,
				     XmNheight, 20,
			  RES_CONVERT(XmNlabelString, "Image Server Node:"),
				     NULL);
    UxPutContext(label2, (char *) UxTopLevelShell1Context);


    /* Creation of label3 */
    label3 = XtVaCreateManagedWidget("label3",
				     xmLabelWidgetClass,
				     form1,
				     XmNx, 10,
				     XmNy, 55,
				     XmNwidth, 120,
				     XmNheight, 20,
			  RES_CONVERT(XmNlabelString, "Image Server Port:"),
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				     XmNleftOffset, 0,
				     XmNleftWidget, label2,
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 20,
				     XmNtopWidget, label2,
				     NULL);
    UxPutContext(label3, (char *) UxTopLevelShell1Context);


    /* Creation of label4 */
    label4 = XtVaCreateManagedWidget("label4",
				     xmLabelWidgetClass,
				     form1,
				     XmNx, 10,
				     XmNy, 90,
				     XmNwidth, 120,
				     XmNheight, 20,
			    RES_CONVERT(XmNlabelString, "Called AP Title:"),
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				     XmNleftOffset, 0,
				     XmNleftWidget, label2,
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 20,
				     XmNtopWidget, label3,
				     NULL);
    UxPutContext(label4, (char *) UxTopLevelShell1Context);


    /* Creation of label5 */
    label5 = XtVaCreateManagedWidget("label5",
				     xmLabelWidgetClass,
				     form1,
				     XmNx, 10,
				     XmNy, 125,
				     XmNwidth, 120,
				     XmNheight, 20,
			   RES_CONVERT(XmNlabelString, "Calling AP Title:"),
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				     XmNleftOffset, 0,
				     XmNleftWidget, label2,
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 20,
				     XmNtopWidget, label4,
				     NULL);
    UxPutContext(label5, (char *) UxTopLevelShell1Context);


    /* Creation of callingAPTitleTxt */
    callingAPTitleTxt = XtVaCreateManagedWidget("callingAPTitleTxt",
						xmTextFieldWidgetClass,
						form1,
						XmNwidth, 160,
						XmNx, 150,
						XmNy, 140,
						XmNheight, 40,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
						XmNtopOffset, 0,
						XmNtopWidget, label5,
						XmNfontList, UxConvertFontList("-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1"),
						NULL);
    UxPutContext(callingAPTitleTxt, (char *) UxTopLevelShell1Context);


    /* Creation of calledAPTitleTxt */
    calledAPTitleTxt = XtVaCreateManagedWidget("calledAPTitleTxt",
					       xmTextFieldWidgetClass,
					       form1,
					       XmNwidth, 160,
					       XmNx, 150,
					       XmNy, 100,
					       XmNheight, 40,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					       XmNtopOffset, 0,
					       XmNtopWidget, label4,
					       XmNfontList, UxConvertFontList("-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1"),
					       NULL);
    UxPutContext(calledAPTitleTxt, (char *) UxTopLevelShell1Context);


    /* Creation of imageServerPortTxt */
    imageServerPortTxt = XtVaCreateManagedWidget("imageServerPortTxt",
						 xmTextFieldWidgetClass,
						 form1,
						 XmNwidth, 160,
						 XmNx, 150,
						 XmNy, 60,
						 XmNheight, 40,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
						 XmNtopOffset, 0,
						 XmNtopWidget, label3,
						 XmNfontList, UxConvertFontList("-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1"),
						 NULL);
    UxPutContext(imageServerPortTxt, (char *) UxTopLevelShell1Context);


    /* Creation of imageServerNodeTxt */
    imageServerNodeTxt = XtVaCreateManagedWidget("imageServerNodeTxt",
						 xmTextFieldWidgetClass,
						 form1,
						 XmNwidth, 160,
						 XmNx, 150,
						 XmNy, 20,
						 XmNheight, 40,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
						 XmNtopOffset, 0,
						 XmNtopWidget, label2,
						 XmNfontList, UxConvertFontList("-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1"),
						 NULL);
    UxPutContext(imageServerNodeTxt, (char *) UxTopLevelShell1Context);


    /* Creation of label1 */
    label1 = XtVaCreateManagedWidget("label1",
				     xmLabelWidgetClass,
				     mainWindow1,
				     NULL);
    UxPutContext(label1, (char *) UxTopLevelShell1Context);


    /* Creation of menu1 */
    menu1 = XtVaCreateManagedWidget("menu1",
				    xmRowColumnWidgetClass,
				    mainWindow1,
				    XmNrowColumnType, XmMENU_BAR,
				    NULL);
    UxPutContext(menu1, (char *) UxTopLevelShell1Context);


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
    UxPutContext(menu1_p1, (char *) UxTopLevelShell1Context);


    /* Creation of menu1_p1_b1 */
    menu1_p1_b1 = XtVaCreateManagedWidget("menu1_p1_b1",
					  xmPushButtonWidgetClass,
					  menu1_p1,
					RES_CONVERT(XmNlabelString, "Quit"),
					  NULL);
    XtAddCallback(menu1_p1_b1, XmNactivateCallback,
		  (XtCallbackProc) activateCB_menu1_p1_b1,
		  (XtPointer) UxTopLevelShell1Context);

    UxPutContext(menu1_p1_b1, (char *) UxTopLevelShell1Context);


    /* Creation of menu1_top_b1 */
    menu1_top_b1 = XtVaCreateManagedWidget("menu1_top_b1",
					   xmCascadeButtonWidgetClass,
					   menu1,
					RES_CONVERT(XmNlabelString, "File"),
					   XmNsubMenuId, menu1_p1,
					   NULL);
    UxPutContext(menu1_top_b1, (char *) UxTopLevelShell1Context);


    XtAddCallback(topLevelShell1, XmNdestroyCallback,
		  (XtCallbackProc) UxDestroyContextCB,
		  (XtPointer) UxTopLevelShell1Context);

    XmMainWindowSetAreas(mainWindow1, menu1, (Widget) NULL,
			 (Widget) NULL, (Widget) NULL, drawingArea1);
    XtVaSetValues(mainWindow1, XmNmessageWindow, label1, NULL);

    return (topLevelShell1);
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget
create_topLevelShell1(swidget _UxUxParent)
{
    Widget rtrn;
    _UxCtopLevelShell1 *UxContext;

    UxTopLevelShell1Context = UxContext =
	(_UxCtopLevelShell1 *) UxNewContext(sizeof(_UxCtopLevelShell1), False);

    UxParent = _UxUxParent;

    rtrn = _Uxbuild_topLevelShell1();

    return (rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/
