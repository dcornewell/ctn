
/*******************************************************************************
	demonstrationForm.c

       Associated Header file: demonstrationForm.h
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
#include <Xm/Label.h>
#include <Xm/TextF.h>
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
** Module Name(s):	create_demonstrationForm
**			loadPatientStudyList
** Author, Date:	Steve Moore, Summer 1994
** Intent:		This file contains the user interface for the
**			demonstration of the work list manager.  This
**			includes the code for creating the widget
**			hierarchy and the callback functions which
**			actually do the work.  The public entry points
**			are the functions to create the widget
**			hierarchy and to populate the list of patients
**			and studies with entries from the database.
** Last Update:		$Author: smm $, $Date: 1996-01-22 20:23:11 $
** Source File:		$RCSfile: demonstrationForm.c,v $
** Revision:		$Revision: 1.12 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.12 $ $RCSfile: demonstrationForm.c,v $";


#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "dicom_objects.h"
#include "tbl.h"
#include "manage.h"
#include "fis.h"
#include "mut.h"

#include "wlma.h"
#include "format.h"

extern FIS_HANDLE *fisHandle;
extern DMAN_HANDLE *dmanHandle;
extern char applicationTitle[];
static LST_HEAD *patientList = NULL;
static LST_HEAD *patientStudyList = NULL;
static LST_HEAD *studyComponentList = NULL;
static LST_HEAD *studyComponentSeriesList = NULL;
static LST_HEAD *workList = NULL;


static Widget demonstrationForm;
static Widget wPatientStudyTxt;
static Widget label18;
static Widget wRemoteApplicationTxt;
static Widget pushButton7;
static Widget pushButton9;
static Widget frame4;
static Widget scrolledWindowList4;
static Widget wStudyComponentList;
static Widget pushButton10;
static Widget frame5;
static Widget scrolledWindowList3;
static Widget wPatientStudyList;
static Widget frame6;
static Widget scrolledWindowList5;
static Widget wWorkListList;
static swidget UxParent;

#define CONTEXT_MACRO_ACCESS 1
#include "demonstrationForm.h"
#undef CONTEXT_MACRO_ACCESS


/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

void
loadPatientStudyList()
{
    CONDITION
    cond;
    char
        b[256];
    WLMA_PATIENTSTUDY
	* p;

    if (patientStudyList == NULL)
	patientStudyList = LST_Create();
    if (patientList == NULL)
	patientList = LST_Create();

    cond = FIS_Get(&fisHandle, FIS_K_PATIENT, FIS_K_PATIENT, NULL,
		   FIS_K_PAT_STUDYLIST, patientList);
    if (cond != FIS_NORMAL) {
	COND_DumpConditions();
	return;
    }
    expandPatientStudy(patientList, patientStudyList);
    MUT_LoadList(wPatientStudyList, patientStudyList, formatPatientStudy, b);
    p = LST_Head(&patientStudyList);
    if (p != NULL)
	LST_Position(&patientStudyList, p);
    XmTextSetString(wPatientStudyTxt, "");
}

void
loadStudyComponentList()
{
    CONDITION cond;
    char b[256];
    WLMA_PATIENTSTUDY *p;

    if (studyComponentList == NULL)
	studyComponentList = LST_Create();
    if (studyComponentSeriesList == NULL)
	studyComponentSeriesList = LST_Create();

    p = LST_Current(&patientStudyList);
    if (p == NULL)
	FIS_ClearList(studyComponentList);
    else {
	cond = FIS_Get(&fisHandle, FIS_K_STUDYCOMPONENT,
		       FIS_K_STUDY, p->Study.StuInsUID,
		       FIS_K_STUDYCOMP_SERIESLIST,
		       studyComponentList);
	if (cond != FIS_NORMAL) {
	    COND_DumpConditions();
	    return;
	}
    }
    expandStudyComponentSCSeries(studyComponentList,
				 studyComponentSeriesList);

    MUT_LoadList(wStudyComponentList, studyComponentSeriesList,
		 formatStudyComponentSCSeries, b);
}

void
loadWorkList()
{
    CONDITION cond;
    char b[256];
    WLMA_PATIENTSTUDY *p,
       *q,
       *save;

    if (workList == NULL)
	workList = LST_Create();
    if (workList == NULL)
	return;

    save = LST_Current(&patientStudyList);

    while ((q = LST_Dequeue(&workList)) != NULL)
	free(q);

    p = LST_Head(&patientStudyList);
    (void) LST_Position(&patientStudyList, p);
    while (p != NULL) {
	if (strcmp(p->Study.StuStaID, "SCHEDULED") == 0) {
	    q = (WLMA_PATIENTSTUDY *) malloc(sizeof(*q));
	    if (q == NULL)
		return;

	    *q = *p;
	    (void) LST_Enqueue(&workList, q);
	}
	p = LST_Next(&patientStudyList);
    }
    (void) LST_Position(&patientStudyList, save);

    (void) LST_Sort(&workList, sizeof(*p), comparePatientStudyScheduled);
    MUT_LoadList(wWorkListList, workList,
		 formatPatientStudyScheduled, b);
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
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	CONDITION cond;
	char destinationApplication[20];
	CTNBOOLEAN nullFlag;
	WLMA_PATIENTSTUDY *ps;
	FIS_STUDYRECORD study;
	FIS_STUDYCOMPONENTRECORD *studyComponent;

	ps = LST_Current(&patientStudyList);
	if (ps == NULL)
	    return;

	cond = MUT_ScanWidget(wRemoteApplicationTxt, MUT_TEXT, &nullFlag,
			      destinationApplication);
	if (cond != MUT_NORMAL)
	    return;

	strcpy(ps->Study.StuStaID, "SCHEDULED");
	UTL_GetDicomDate(ps->Study.SchStuStaDat);
	UTL_GetDicomTime(ps->Study.SchStuStaTim);
	ps->Study.Flag |= FIS_K_STU_STUSTAID |
	    FIS_K_STU_SCHSTUSTADAT |
	    FIS_K_STU_SCHSTUSTATIM;

	study = ps->Study;
	study.Flag = FIS_K_STU_STUINSUID | FIS_K_STU_STUSTAID |
	    FIS_K_STU_SCHSTUSTADAT |
	    FIS_K_STU_SCHSTUSTATIM;
	cond = FIS_Update(&fisHandle, FIS_K_STUDY, &study);
	if (cond != FIS_NORMAL)
	    COND_DumpConditions();
	studyComponent = LST_Head(&studyComponentList);
	(void) LST_Position(&studyComponentList, studyComponent);
	while (studyComponent != NULL) {
	    cond = FIS_Delete(&fisHandle, FIS_K_STUDYCOMPONENT,
			   FIS_K_STUDYCOMPONENT, studyComponent->StuComUID);
	    if (cond != FIS_NORMAL)
		COND_DumpConditions();
	    cond = FIS_Delete(&fisHandle, FIS_K_SCSERIES,
			   FIS_K_STUDYCOMPONENT, studyComponent->StuComUID);
	    if (cond != FIS_NORMAL)
		COND_DumpConditions();
	    cond = FIS_Delete(&fisHandle, FIS_K_SCIMAGE,
			   FIS_K_STUDYCOMPONENT, studyComponent->StuComUID);
	    if (cond != FIS_NORMAL)
		COND_DumpConditions();

	    studyComponent = LST_Next(&studyComponentList);
	}

	if (!nullFlag) {
	    cond = FIS_SendEvent(&fisHandle, &dmanHandle, FIS_K_STUDY,
			&ps->Study, FIS_K_STUDY_SCHEDULED, applicationTitle,
				 destinationApplication);
	    if (cond != FIS_NORMAL)
		COND_DumpConditions();
	}
	loadPatientStudyList();
	loadStudyComponentList();
	loadWorkList();
    }
}

static void
activateCB_pushButton9(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	CONDITION cond;
	char destinationApplication[20];
	CTNBOOLEAN nullFlag;
	WLMA_PATIENTSTUDY *ps;
	FIS_STUDYRECORD study;

	ps = LST_Current(&patientStudyList);
	if (ps == NULL)
	    return;

	cond = MUT_ScanWidget(wRemoteApplicationTxt, MUT_TEXT, &nullFlag,
			      destinationApplication);
	if (cond != MUT_NORMAL)
	    return;

	strcpy(ps->Study.StuStaID, "READ");
	UTL_GetDicomDate(ps->Study.StuReaDat);
	UTL_GetDicomTime(ps->Study.StuReaTim);
	ps->Study.Flag |= FIS_K_STU_STUSTAID | FIS_K_STU_STUREADAT |
	    FIS_K_STU_STUREATIM;

	study = ps->Study;
	study.Flag = FIS_K_STU_STUINSUID | FIS_K_STU_STUSTAID;
	cond = FIS_Update(&fisHandle, FIS_K_STUDY, &study);
	if (cond != FIS_NORMAL)
	    COND_DumpConditions();

	if (!nullFlag) {
	    cond = FIS_SendEvent(&fisHandle, &dmanHandle, FIS_K_STUDY,
			     &ps->Study, FIS_K_STUDY_READ, applicationTitle,
				 destinationApplication);
	    if (cond != FIS_NORMAL)
		COND_DumpConditions();
	}
	loadPatientStudyList();
	loadWorkList();
    }
}

static void
activateCB_pushButton10(
			Widget wgt,
			XtPointer cd,
			XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	loadPatientStudyList();
	loadStudyComponentList();
	loadWorkList();
    }
}

static void
browseSelectionCB_wPatientStudyList(
				    Widget wgt,
				    XtPointer cd,
				    XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	XmListCallbackStruct *listStruct;
	WLMA_PATIENTSTUDY *p;
	int i;
	char b[256];


	listStruct = (XmListCallbackStruct *) UxCallbackArg;
	p = LST_Index(&patientStudyList, listStruct->item_position);

	formatPatientStudy(p, 0, b);
	XmTextSetString(wPatientStudyTxt, b);
	loadStudyComponentList();
    }
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget
_Uxbuild_demonstrationForm()
{
    Widget _UxParent;


    /* Creation of demonstrationForm */
    _UxParent = UxParent;
    if (_UxParent == NULL) {
	_UxParent = XtVaCreatePopupShell("demonstrationForm_shell",
				       topLevelShellWidgetClass, UxTopLevel,
					 XmNx, 66,
					 XmNy, 29,
					 XmNwidth, 952,
					 XmNheight, 771,
					 XmNshellUnitType, XmPIXELS,
					 XmNtitle, "demonstrationForm",
					 XmNiconName, "demonstrationForm",
					 NULL);

    }
    demonstrationForm = XtVaCreateManagedWidget("demonstrationForm",
						xmFormWidgetClass,
						_UxParent,
						XmNwidth, 952,
						XmNheight, 771,
					     XmNresizePolicy, XmRESIZE_NONE,
						XmNunitType, XmPIXELS,
						NULL);


    /* Creation of wPatientStudyTxt */
    wPatientStudyTxt = XtVaCreateManagedWidget("wPatientStudyTxt",
					       xmTextFieldWidgetClass,
					       demonstrationForm,
					       XmNwidth, 890,
					       XmNx, 40,
					       XmNy, 220,
					       XmNheight, 30,
					       NULL);


    /* Creation of label18 */
    label18 = XtVaCreateManagedWidget("label18",
				      xmLabelWidgetClass,
				      demonstrationForm,
				      XmNx, 60,
				      XmNy, 410,
				      XmNwidth, 210,
				      XmNheight, 30,
		  RES_CONVERT(XmNlabelString, "Remote Application (Title)"),
				      NULL);


    /* Creation of wRemoteApplicationTxt */
    wRemoteApplicationTxt = XtVaCreateManagedWidget("wRemoteApplicationTxt",
						    xmTextFieldWidgetClass,
						    demonstrationForm,
						    XmNwidth, 210,
						    XmNx, 300,
						    XmNy, 410,
						    XmNheight, 40,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
						    XmNtopOffset, 0,
						    XmNtopWidget, label18,
						    NULL);


    /* Creation of pushButton7 */
    pushButton7 = XtVaCreateManagedWidget("pushButton7",
					  xmPushButtonWidgetClass,
					  demonstrationForm,
					  XmNx, 50,
					  XmNy, 480,
					  XmNwidth, 240,
					  XmNheight, 50,
			     RES_CONVERT(XmNlabelString, "Study Scheduled"),
					  NULL);
    XtAddCallback(pushButton7, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton7,
		  (XtPointer) NULL);



    /* Creation of pushButton9 */
    pushButton9 = XtVaCreateManagedWidget("pushButton9",
					  xmPushButtonWidgetClass,
					  demonstrationForm,
					  XmNx, 350,
					  XmNy, 480,
					  XmNwidth, 240,
					  XmNheight, 50,
				  RES_CONVERT(XmNlabelString, "Study Read"),
					  NULL);
    XtAddCallback(pushButton9, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton9,
		  (XtPointer) NULL);



    /* Creation of frame4 */
    frame4 = XtVaCreateManagedWidget("frame4",
				     xmFrameWidgetClass,
				     demonstrationForm,
				     XmNwidth, 890,
				     XmNheight, 120,
				     XmNx, 40,
				     XmNy, 260,
				     XmNresizable, FALSE,
				     NULL);


    /* Creation of scrolledWindowList4 */
    scrolledWindowList4 = XtVaCreateManagedWidget("scrolledWindowList4",
						xmScrolledWindowWidgetClass,
						  frame4,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNshadowThickness, 0,
						  XmNx, 140,
						  XmNy, 40,
						  NULL);


    /* Creation of wStudyComponentList */
    wStudyComponentList = XtVaCreateManagedWidget("wStudyComponentList",
						  xmListWidgetClass,
						  scrolledWindowList4,
						  XmNwidth, 290,
						  XmNheight, 40,
						  NULL);


    /* Creation of pushButton10 */
    pushButton10 = XtVaCreateManagedWidget("pushButton10",
					   xmPushButtonWidgetClass,
					   demonstrationForm,
					   XmNx, 670,
					   XmNy, 480,
					   XmNwidth, 240,
					   XmNheight, 50,
				     RES_CONVERT(XmNlabelString, "Refresh"),
					   XmNsensitive, TRUE,
					   NULL);
    XtAddCallback(pushButton10, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton10,
		  (XtPointer) NULL);



    /* Creation of frame5 */
    frame5 = XtVaCreateManagedWidget("frame5",
				     xmFrameWidgetClass,
				     demonstrationForm,
				     XmNwidth, 890,
				     XmNheight, 150,
				     XmNx, 40,
				     XmNy, 40,
				     XmNresizable, FALSE,
				     NULL);


    /* Creation of scrolledWindowList3 */
    scrolledWindowList3 = XtVaCreateManagedWidget("scrolledWindowList3",
						xmScrolledWindowWidgetClass,
						  frame5,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNshadowThickness, 0,
						  XmNx, 0,
						  XmNy, 0,
						  NULL);


    /* Creation of wPatientStudyList */
    wPatientStudyList = XtVaCreateManagedWidget("wPatientStudyList",
						xmListWidgetClass,
						scrolledWindowList3,
						XmNwidth, 890,
						XmNheight, 160,
						NULL);
    XtAddCallback(wPatientStudyList, XmNbrowseSelectionCallback,
		  (XtCallbackProc) browseSelectionCB_wPatientStudyList,
		  (XtPointer) NULL);



    /* Creation of frame6 */
    frame6 = XtVaCreateManagedWidget("frame6",
				     xmFrameWidgetClass,
				     demonstrationForm,
				     XmNwidth, 890,
				     XmNheight, 120,
				     XmNx, 36,
				     XmNy, 576,
				     XmNresizable, FALSE,
				     NULL);


    /* Creation of scrolledWindowList5 */
    scrolledWindowList5 = XtVaCreateManagedWidget("scrolledWindowList5",
						xmScrolledWindowWidgetClass,
						  frame6,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNshadowThickness, 0,
						  XmNx, 260,
						  XmNy, 20,
						  NULL);


    /* Creation of wWorkListList */
    wWorkListList = XtVaCreateManagedWidget("wWorkListList",
					    xmListWidgetClass,
					    scrolledWindowList5,
					    XmNwidth, 230,
					    XmNheight, 10,
					    NULL);



    return (demonstrationForm);
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget
create_demonstrationForm(swidget _UxUxParent)
{
    Widget rtrn;

    UxParent = _UxUxParent;

    rtrn = _Uxbuild_demonstrationForm();

    return (rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/
