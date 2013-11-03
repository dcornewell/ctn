
/*******************************************************************************
	databaseForm.c

       Associated Header file: databaseForm.h
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <X11/Shell.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/Text.h>
#include <Xm/ToggleB.h>
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>
#include <Xm/Text.h>
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
** Module Name(s):	create_databaseForm
**			loadPatientList
** Author, Date:	Steve Moore, Summer 1994
** Intent:		This file contains the functions that provide the
**			user interface for the database functions for the
**			WLMA application.  These database functions include
**			adding patients and studies to the system and
**			modifying values for patients and studies.  When
**			patients and studies are added, these interface
**			functions do not send any DICOM events (this is
**			a very structured demonstration).
**			The public entry points are routines to create
**			the widget hierarchy for this interface and
**			to load the patient list from data in the
**			existing database.
** Last Update:         $Author: smm $, $Date: 1996-01-22 20:23:00 $
** Source File:         $RCSfile: databaseForm.c,v $
** Revision:            $Revision: 1.7 $
** Status:              $State: Exp $
*/

static char rcsid[] = "$Revision: 1.7 $ $RCSfile: databaseForm.c,v $";



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
static CONDITION readPatientInformation(FIS_PATIENTRECORD * p);
static void displayPatientInformation(FIS_PATIENTRECORD * p);

static CONDITION readStudyInformation(FIS_STUDYRECORD * s);
static void readStudyStatusID(char *status);
static void displayStudyInformation(FIS_STUDYRECORD * s);
static void displayStudyStatusID(char *status);

static LST_HEAD *patientList = NULL;
static LST_HEAD *studyList = NULL;
static char studyStatusID[DICOM_CS_LENGTH + 1] = "CREATED";


static Widget databaseForm;
static Widget form2;
static Widget frame2;
static Widget scrolledWindowList2;
static Widget wStudyList;
static Widget label8;
static Widget wStudyUIDTxt;
static Widget pushButton4;
static Widget pushButton5;
static Widget pushButton6;
static Widget form4;
static Widget frame1;
static Widget scrolledWindowList1;
static Widget wPatientList;
static Widget label1;
static Widget label2;
static Widget label3;
static Widget label4;
static Widget label5;
static Widget label6;
static Widget label7;
static Widget wPatientUIDTxt;
static Widget wPatientIDTxt;
static Widget wLastNameTxt;
static Widget wFirstNameTxt;
static Widget wMiddleInitialTxt;
static Widget wBirthDateTxt;
static Widget rowColumn1;
static Widget wMaleSexToggle;
static Widget wFemaleSexToggle;
static Widget pushButton1;
static Widget pushButton2;
static Widget pushButton3;
static Widget pushButton8;
static Widget label16;
static Widget label9;
static Widget label10;
static Widget label11;
static Widget label12;
static Widget label13;
static Widget label14;
static Widget label15;
static Widget wStudyIDTxt;
static Widget wStudyDateTxt;
static Widget wStudyTimeTxt;
static Widget wStudyLocationTxt;
static Widget wProcedureDescriptionTxt;
static Widget wRequestedProcedureTxt;
static Widget wAccessionNumberTxt;
static Widget optionMenu_p1;
static Widget wStudyStatusCreatedButton;
static Widget wStudyStatusScheduledButton;
static Widget wStudyStatusArrivedButton;
static Widget wStudyStatusStartedButton;
static Widget wStudyStatusCompletedButton;
static Widget wStudyStatusVerifiedButton;
static Widget wStudyStatusReadButton;
static Widget wStudyStatusMenu;
static Widget frame3;
static Widget scrolledWindowText1;
static Widget wDatabaseMessageTxt;
static swidget UxParent;

#define CONTEXT_MACRO_ACCESS 1
#include "databaseForm.h"
#undef CONTEXT_MACRO_ACCESS


/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

void errorText(char *txt);

static CONDITION
readPatientInformation(FIS_PATIENTRECORD * p)
{
    CTNBOOLEAN
    nullFlag;
    CONDITION
	cond;
    char
        lastName[60],
        firstName[60],
        middleInitial[10];

    memset(p, 0, sizeof(*p));
    p->Type = FIS_K_PATIENT;

    cond = MUT_ScanWidget(wPatientIDTxt, MUT_TEXT, &nullFlag, p->PatID);
    if (cond != MUT_NORMAL || nullFlag) {
	(void) COND_PopCondition(TRUE);
	p->PatID[0] = '\0';
    }
    cond = MUT_ScanWidget(wPatientUIDTxt, MUT_TEXT, &nullFlag, p->PatUID);
    if (cond != MUT_NORMAL || nullFlag) {
	(void) COND_PopCondition(TRUE);
	p->PatUID[0] = '\0';
    }
    cond = MUT_ScanWidget(wLastNameTxt, MUT_TEXT, &nullFlag, lastName);
    if (cond != MUT_NORMAL || nullFlag) {
	errorText("No last name entered");
	return 0;
    }
    cond = MUT_ScanWidget(wFirstNameTxt, MUT_TEXT, &nullFlag, firstName);
    if (cond != MUT_NORMAL || nullFlag) {
	errorText("No first name entered");
	return 0;
    }
    cond = MUT_ScanWidget(wMiddleInitialTxt, MUT_TEXT, &nullFlag, middleInitial);
    if (cond != MUT_NORMAL || nullFlag)
	middleInitial[0] = '\0';
    (void) sprintf(p->PatNam, "%-s^%-s^%c", lastName, firstName, middleInitial[0]);

    cond = MUT_ScanWidget(wBirthDateTxt, MUT_TEXT, &nullFlag, p->PatBirDat);
    if (cond != MUT_NORMAL || nullFlag) {
	errorText("No birthdate entered");
	return 0;
    }
    if (XmToggleButtonGetState(wMaleSexToggle))
	strcpy(p->PatSex, "M");
    else
	strcpy(p->PatSex, "F");

    errorText("");
    return 1;
}

static void
displayPatientInformation(FIS_PATIENTRECORD * p)
{
    char
        lastName[60],
        firstName[60],
        middleInitial[60];
    int
        index = 0,
        charsScanned = 0;

    if (sscanf(&p->PatNam[index], "%[^^] %n", lastName, &charsScanned) == 1) {
	index += charsScanned + 1;
    } else
	strcpy(lastName, p->PatNam);

    if (sscanf(&p->PatNam[index], "%[^^] %n", firstName, &charsScanned) == 1) {
	index += charsScanned + 1;
    } else
	firstName[0] = '\0';

    if (sscanf(&p->PatNam[index], "%[^^]", middleInitial) != 1)
	middleInitial[0] = '\0';

    XmTextSetString(wLastNameTxt, lastName);
    XmTextSetString(wFirstNameTxt, firstName);
    XmTextSetString(wMiddleInitialTxt, middleInitial);
    XmTextSetString(wPatientUIDTxt, p->PatUID);
    XmTextSetString(wPatientIDTxt, p->PatID);
    XmTextSetString(wBirthDateTxt, p->PatBirDat);
    if (p->PatSex[0] == 'M') {
	XmToggleButtonSetState(wMaleSexToggle, True, False);
	XmToggleButtonSetState(wFemaleSexToggle, False, False);
    } else {
	XmToggleButtonSetState(wFemaleSexToggle, True, False);
	XmToggleButtonSetState(wMaleSexToggle, False, False);
    }
}
void
loadPatientList()
{
    CONDITION
    cond;
    char
        b[80];

    if (patientList == NULL)
	patientList = LST_Create();

    cond = FIS_Get(&fisHandle, FIS_K_PATIENT, FIS_K_PATIENT, NULL, 0, patientList);
    if (cond != FIS_NORMAL) {
	COND_DumpConditions();
	return;
    }
    MUT_LoadList(wPatientList, patientList, formatPatientRecord, b);
}



static CONDITION
readStudyInformation(FIS_STUDYRECORD * s)
{
    CTNBOOLEAN
    nullFlag;
    CONDITION
	cond;

    s->Flag = 0;

    cond = MUT_ScanWidget(wStudyUIDTxt, MUT_TEXT, &nullFlag, s->StuInsUID);
    if (cond != MUT_NORMAL || nullFlag) {
	(void) COND_PopCondition(TRUE);
	s->StuInsUID[0] = '\0';
    } else
	s->Flag |= FIS_K_STU_STUINSUID;

    cond = MUT_ScanWidget(wAccessionNumberTxt, MUT_TEXT, &nullFlag, s->AccNum);
    if (cond != MUT_NORMAL || nullFlag) {
	(void) COND_PopCondition(TRUE);
	s->AccNum[0] = '\0';
    }
    s->Flag |= FIS_K_STU_ACCNUM;

    cond = MUT_ScanWidget(wStudyIDTxt, MUT_TEXT, &nullFlag, s->StuID);
    if (cond != MUT_NORMAL || nullFlag) {
	(void) COND_PopCondition(TRUE);
	s->StuID[0] = '\0';
    }
    s->Flag |= FIS_K_STU_STUID;

    cond = MUT_ScanWidget(wStudyDateTxt, MUT_TEXT, &nullFlag, s->SchStuStaDat);
    if (cond != MUT_NORMAL || nullFlag)
	return 0;
    s->Flag |= FIS_K_STU_SCHSTUSTADAT;

    cond = MUT_ScanWidget(wStudyTimeTxt, MUT_TEXT, &nullFlag, s->SchStuStaTim);
    if (cond != MUT_NORMAL || nullFlag)
	return 0;
    s->Flag |= FIS_K_STU_SCHSTUSTATIM;

    cond = MUT_ScanWidget(wStudyLocationTxt, MUT_TEXT, &nullFlag, s->SchStuLoc);
    if (cond != MUT_NORMAL || nullFlag)
	return 0;
    s->Flag |= FIS_K_STU_SCHSTULOC;

    cond = MUT_ScanWidget(wProcedureDescriptionTxt, MUT_TEXT, &nullFlag, s->ProDes);
    if (cond != MUT_NORMAL || nullFlag)
	return 0;
    s->Flag |= FIS_K_STU_PRODES;

    readStudyStatusID(s->StuStaID);
    s->Flag |= FIS_K_STU_STUSTAID;

    return 1;
}

typedef struct {
    Widget *w;
    char *s;
}   W_MAP;

static void
readStudyStatusID(char *s)
{
    Arg arg;
    Widget w;

    XtSetArg(arg, XmNmenuHistory, &w);
    XtGetValues(optionMenu_p1, &arg, 1);

#ifdef RUNTIME
    if (w == wStudyStatusCreatedButton)
	strcpy(s, "CREATED");
    else if (w == wStudyStatusScheduledButton)
	strcpy(s, "SCHEDULED");
    else if (w == wStudyStatusArrivedButton)
	strcpy(s, "ARRIVED");
    else if (w == wStudyStatusCompletedButton)
	strcpy(s, "COMPLETED");
    else if (w == wStudyStatusVerifiedButton)
	strcpy(s, "VERIFIED");
    else if (w == wStudyStatusReadButton)
	strcpy(s, "READ");
    else
	strcpy(s, "CREATED");
#endif

}

static void
displayStudyInformation(FIS_STUDYRECORD * s)
{
    XmTextSetString(wStudyUIDTxt, s->StuInsUID);
    XmTextSetString(wAccessionNumberTxt, s->AccNum);
    XmTextSetString(wStudyIDTxt, s->StuID);
    XmTextSetString(wStudyDateTxt, s->SchStuStaDat);
    XmTextSetString(wStudyTimeTxt, s->SchStuStaTim);
    XmTextSetString(wStudyLocationTxt, s->SchStuLoc);
    XmTextSetString(wProcedureDescriptionTxt, s->ProDes);
    displayStudyStatusID(s->StuStaID);
}

static void
displayStudyStatusID(char *s)
{
    Widget w;
    XButtonEvent event;
    Arg arg;

#ifdef RUNTIME
    if (strcmp(s, "CREATED") == 0)
	w = wStudyStatusCreatedButton;
    else if (strcmp(s, "SCHEDULED") == 0)
	w = wStudyStatusScheduledButton;
    else if (strcmp(s, "ARRIVED") == 0)
	w = wStudyStatusArrivedButton;
    else if (strcmp(s, "STARTED") == 0)
	w = wStudyStatusStartedButton;
    else if (strcmp(s, "COMPLETED") == 0)
	w = wStudyStatusCompletedButton;
    else if (strcmp(s, "VERIFIED") == 0)
	w = wStudyStatusVerifiedButton;
    else if (strcmp(s, "READ") == 0)
	w = wStudyStatusReadButton;
    else
	w = wStudyStatusCreatedButton;

    XtSetArg(arg, XmNmenuHistory, w);
    XtSetValues(optionMenu_p1, &arg, 1);
    XtSetValues(wStudyStatusMenu, &arg, 1);
/*
    XtCallActionProc(w, "ArmAndActivate", event, NULL, 0);
*/

#endif
}


static void
loadStudyList()
{
    CONDITION
    cond;
    char
        b[80];
    FIS_PATIENTRECORD
	p;

    if (studyList == NULL)
	studyList = LST_Create();

    FIS_ClearList(studyList);
    cond = readPatientInformation(&p);
    if (cond == 1) {
	if (p.PatUID[0] != '\0') {
	    cond = FIS_Get(&fisHandle, FIS_K_STUDY, FIS_K_PATIENT,
			   p.PatUID, 0, studyList);
	    if (cond != FIS_NORMAL) {
		COND_DumpConditions();
		FIS_ClearList(studyList);
	    }
	}
    }
    MUT_LoadList(wStudyList, studyList, formatStudyRecord, b);
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void
browseSelectionCB_wStudyList(
			     Widget wgt,
			     XtPointer cd,
			     XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	XmListCallbackStruct *listStruct;
	FIS_STUDYRECORD *s;
	int i;

	listStruct = (XmListCallbackStruct *) UxCallbackArg;
	s = LST_Head(&studyList);
	(void) LST_Position(&studyList, s);
	for (i = 2; i <= listStruct->item_position; i++)
	    s = LST_Next(&studyList);

	displayStudyInformation(s);

    }
}

static void
activateCB_pushButton4(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	CONDITION
	    cond;
	FIS_PATIENTRECORD
	    p;
	FIS_STUDYRECORD
	    s;

	XtCallActionProc(wStudyStatusCreatedButton, "ArmAndActivate", NULL,
			 NULL, 0);
	p.Type = FIS_K_PATIENT;
	cond = readPatientInformation(&p);
	if (cond != 1)
	    return;

	s.Type = FIS_K_STUDY;
	cond = readStudyInformation(&s);
	if (cond != 1)
	    return;

	cond = FIS_NewRecord(&fisHandle, FIS_K_STUDY, &s);
	if (cond != FIS_NORMAL) {
	    COND_DumpConditions();
	    return;
	}
	strcpy(s.PatUID, p.PatUID);

	cond = FIS_Insert(&fisHandle, FIS_K_STUDY, &s);
	if (cond != FIS_NORMAL) {
	    COND_DumpConditions();
	    return;
	}
	loadStudyList();
    }
}

static void
activateCB_pushButton5(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	CONDITION
	    cond;
	FIS_STUDYRECORD
	    s;

	s.Type = FIS_K_STUDY;
	cond = readStudyInformation(&s);
	if (cond != 1)
	    return;

	cond = FIS_Delete(&fisHandle, FIS_K_STUDY, FIS_K_STUDY, s.StuInsUID);
	if (cond != FIS_NORMAL) {
	    COND_DumpConditions();
	}
	loadStudyList();
    }
}

static void
activateCB_pushButton6(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	CONDITION
	    cond;
	FIS_STUDYRECORD
	    s;

	s.Type = FIS_K_STUDY;
	cond = readStudyInformation(&s);
	if (cond != 1)
	    return;

	cond = FIS_Update(&fisHandle, FIS_K_STUDY, &s);
	if (cond != FIS_NORMAL) {
	    COND_DumpConditions();
	    return;
	}
	loadStudyList();
    }
}

static void
browseSelectionCB_wPatientList(
			       Widget wgt,
			       XtPointer cd,
			       XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	XmListCallbackStruct *listStruct;
	FIS_PATIENTRECORD *p;
	int i;

	listStruct = (XmListCallbackStruct *) UxCallbackArg;
	p = LST_Head(&patientList);
	(void) LST_Position(&patientList, p);
	for (i = 2; i <= listStruct->item_position; i++)
	    p = LST_Next(&patientList);

	displayPatientInformation(p);
	loadStudyList();

    }
}

static void
activateCB_pushButton1(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	CONDITION
	    cond;
	FIS_PATIENTRECORD
	    p;

	p.Type = FIS_K_PATIENT;
	cond = readPatientInformation(&p);
	if (cond != 1)
	    return;

	cond = FIS_NewRecord(&fisHandle, FIS_K_PATIENT, &p);
	if (cond != FIS_NORMAL) {
	    COND_DumpConditions();
	    return;
	}
	cond = FIS_Insert(&fisHandle, FIS_K_PATIENT, &p);
	if (cond != FIS_NORMAL) {
	    COND_DumpConditions();
	    return;
	}
	loadPatientList();
    }
}

static void
activateCB_pushButton2(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	CONDITION
	    cond;
	FIS_PATIENTRECORD
	    p;

	p.Type = FIS_K_PATIENT;
	cond = readPatientInformation(&p);
	if (cond != 1)
	    return;

	cond = FIS_Delete(&fisHandle, FIS_K_PATIENT, FIS_K_PATIENT, p.PatUID);
	if (cond != FIS_NORMAL)
	    COND_DumpConditions();

	loadPatientList();
    }
}

static void
activateCB_pushButton3(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	CONDITION
	    cond;
	FIS_PATIENTRECORD
	    p;

	p.Type = FIS_K_PATIENT;
	cond = readPatientInformation(&p);
	if (cond != 1)
	    return;

	cond = FIS_Update(&fisHandle, FIS_K_PATIENT, &p);
	if (cond != FIS_NORMAL) {
	    COND_DumpConditions();
	    return;
	}
	loadPatientList();
    }
}

static void
activateCB_pushButton8(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	CONDITION cond;

	XmTextSetString(wDatabaseMessageTxt, "");
	cond = loadDatabase(&fisHandle);
	if (cond != 1) {
	    MUT_DisplayConditionText(wDatabaseMessageTxt);
	    (void) COND_PopCondition(TRUE);
	}
	loadPatientList();
    }
}

static void
activateCB_wStudyStatusCreatedButton(
				     Widget wgt,
				     XtPointer cd,
				     XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	strcpy(studyStatusID, "CREATED");
    }
}

static void
activateCB_wStudyStatusScheduledButton(
				       Widget wgt,
				       XtPointer cd,
				       XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	strcpy(studyStatusID, "SCHEDULED");
    }
}

static void
activateCB_wStudyStatusArrivedButton(
				     Widget wgt,
				     XtPointer cd,
				     XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	strcpy(studyStatusID, "ARRIVED");
    }
}

static void
activateCB_wStudyStatusStartedButton(
				     Widget wgt,
				     XtPointer cd,
				     XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	strcpy(studyStatusID, "STARTED");
    }
}

static void
activateCB_wStudyStatusCompletedButton(
				       Widget wgt,
				       XtPointer cd,
				       XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	strcpy(studyStatusID, "COMPLETED");
    }
}

static void
activateCB_wStudyStatusVerifiedButton(
				      Widget wgt,
				      XtPointer cd,
				      XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	strcpy(studyStatusID, "VERIFIED");
    }
}

static void
activateCB_wStudyStatusReadButton(
				  Widget wgt,
				  XtPointer cd,
				  XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	strcpy(studyStatusID, "READ");
    }
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget
_Uxbuild_databaseForm()
{
    Widget _UxParent;
    Widget optionMenu_p1_shell;


    /* Creation of databaseForm */
    _UxParent = UxParent;
    if (_UxParent == NULL) {
	_UxParent = XtVaCreatePopupShell("databaseForm_shell",
				       topLevelShellWidgetClass, UxTopLevel,
					 XmNx, 185,
					 XmNy, 171,
					 XmNwidth, 925,
					 XmNheight, 759,
					 XmNshellUnitType, XmPIXELS,
					 XmNtitle, "databaseForm",
					 XmNiconName, "databaseForm",
					 NULL);

    }
    databaseForm = XtVaCreateManagedWidget("databaseForm",
					   xmFormWidgetClass,
					   _UxParent,
					   XmNwidth, 925,
					   XmNheight, 759,
					   XmNresizePolicy, XmRESIZE_NONE,
					   XmNunitType, XmPIXELS,
					   NULL);


    /* Creation of form2 */
    form2 = XtVaCreateManagedWidget("form2",
				    xmFormWidgetClass,
				    databaseForm,
				    XmNwidth, 531,
				    XmNheight, 150,
				    XmNresizePolicy, XmRESIZE_NONE,
				    XmNx, 373,
				    XmNy, 30,
				    NULL);


    /* Creation of frame2 */
    frame2 = XtVaCreateManagedWidget("frame2",
				     xmFrameWidgetClass,
				     form2,
				     XmNwidth, 516,
				     XmNheight, 150,
				     XmNx, 15,
				     XmNy, 0,
				     NULL);


    /* Creation of scrolledWindowList2 */
    scrolledWindowList2 = XtVaCreateManagedWidget("scrolledWindowList2",
						xmScrolledWindowWidgetClass,
						  frame2,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNshadowThickness, 0,
						  XmNx, 6,
						  XmNy, 4,
						  NULL);


    /* Creation of wStudyList */
    wStudyList = XtVaCreateManagedWidget("wStudyList",
					 xmListWidgetClass,
					 scrolledWindowList2,
					 XmNwidth, 510,
					 XmNheight, 146,
					 NULL);
    XtAddCallback(wStudyList, XmNbrowseSelectionCallback,
		  (XtCallbackProc) browseSelectionCB_wStudyList,
		  (XtPointer) NULL);



    /* Creation of label8 */
    label8 = XtVaCreateManagedWidget("label8",
				     xmLabelWidgetClass,
				     databaseForm,
				     XmNx, 470,
				     XmNy, 210,
				     XmNwidth, 140,
				     XmNheight, 30,
				   RES_CONVERT(XmNlabelString, "Study UID"),
				     NULL);


    /* Creation of wStudyUIDTxt */
    wStudyUIDTxt = XtVaCreateManagedWidget("wStudyUIDTxt",
					   xmTextWidgetClass,
					   databaseForm,
					   XmNwidth, 180,
					   XmNx, 560,
					   XmNy, 200,
					   XmNheight, 30,
					   XmNeditable, FALSE,
					 XmNleftAttachment, XmATTACH_WIDGET,
					   XmNleftOffset, 10,
					   XmNleftWidget, label8,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					   XmNtopOffset, 0,
					   XmNtopWidget, label8,
					   NULL);


    /* Creation of pushButton4 */
    pushButton4 = XtVaCreateManagedWidget("pushButton4",
					  xmPushButtonWidgetClass,
					  databaseForm,
					  XmNx, 490,
					  XmNy, 570,
					  XmNwidth, 110,
					  XmNheight, 50,
					  RES_CONVERT(XmNlabelString, "Add"),
					  NULL);
    XtAddCallback(pushButton4, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton4,
		  (XtPointer) NULL);



    /* Creation of pushButton5 */
    pushButton5 = XtVaCreateManagedWidget("pushButton5",
					  xmPushButtonWidgetClass,
					  databaseForm,
					  XmNx, 600,
					  XmNy, 570,
					  XmNwidth, 110,
					  XmNheight, 50,
				      RES_CONVERT(XmNlabelString, "Delete"),
					  NULL);
    XtAddCallback(pushButton5, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton5,
		  (XtPointer) NULL);



    /* Creation of pushButton6 */
    pushButton6 = XtVaCreateManagedWidget("pushButton6",
					  xmPushButtonWidgetClass,
					  databaseForm,
					  XmNx, 710,
					  XmNy, 570,
					  XmNwidth, 110,
					  XmNheight, 50,
				      RES_CONVERT(XmNlabelString, "Update"),
					  NULL);
    XtAddCallback(pushButton6, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton6,
		  (XtPointer) NULL);



    /* Creation of form4 */
    form4 = XtVaCreateManagedWidget("form4",
				    xmFormWidgetClass,
				    databaseForm,
				    XmNwidth, 390,
				    XmNheight, 660,
				    XmNresizePolicy, XmRESIZE_NONE,
				    XmNx, 0,
				    XmNy, 10,
				    NULL);


    /* Creation of frame1 */
    frame1 = XtVaCreateManagedWidget("frame1",
				     xmFrameWidgetClass,
				     form4,
				     XmNwidth, 310,
				     XmNheight, 150,
				     XmNx, 42,
				     XmNy, 22,
				     NULL);


    /* Creation of scrolledWindowList1 */
    scrolledWindowList1 = XtVaCreateManagedWidget("scrolledWindowList1",
						xmScrolledWindowWidgetClass,
						  frame1,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNshadowThickness, 0,
						  XmNx, 2,
						  XmNy, 2,
						  NULL);


    /* Creation of wPatientList */
    wPatientList = XtVaCreateManagedWidget("wPatientList",
					   xmListWidgetClass,
					   scrolledWindowList1,
					   XmNwidth, 306,
					   XmNheight, 140,
					   NULL);
    XtAddCallback(wPatientList, XmNbrowseSelectionCallback,
		  (XtCallbackProc) browseSelectionCB_wPatientList,
		  (XtPointer) NULL);



    /* Creation of label1 */
    label1 = XtVaCreateManagedWidget("label1",
				     xmLabelWidgetClass,
				     form4,
				     XmNx, 20,
				     XmNy, 190,
				     XmNwidth, 140,
				     XmNheight, 30,
				 RES_CONVERT(XmNlabelString, "Patient UID"),
				     NULL);


    /* Creation of label2 */
    label2 = XtVaCreateManagedWidget("label2",
				     xmLabelWidgetClass,
				     form4,
				     XmNx, 40,
				     XmNy, 220,
				     XmNwidth, 140,
				     XmNheight, 30,
				  RES_CONVERT(XmNlabelString, "Patient ID"),
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				     XmNleftOffset, 0,
				     XmNleftWidget, label1,
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 10,
				     XmNtopWidget, label1,
				     NULL);


    /* Creation of label3 */
    label3 = XtVaCreateManagedWidget("label3",
				     xmLabelWidgetClass,
				     form4,
				     XmNx, 40,
				     XmNy, 260,
				     XmNwidth, 140,
				     XmNheight, 30,
				   RES_CONVERT(XmNlabelString, "Last Name"),
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				     XmNleftOffset, 0,
				     XmNleftWidget, label1,
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 10,
				     XmNtopWidget, label2,
				     NULL);


    /* Creation of label4 */
    label4 = XtVaCreateManagedWidget("label4",
				     xmLabelWidgetClass,
				     form4,
				     XmNx, 40,
				     XmNy, 300,
				     XmNwidth, 140,
				     XmNheight, 30,
				  RES_CONVERT(XmNlabelString, "First Name"),
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				     XmNleftOffset, 0,
				     XmNleftWidget, label1,
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 10,
				     XmNtopWidget, label3,
				     NULL);


    /* Creation of label5 */
    label5 = XtVaCreateManagedWidget("label5",
				     xmLabelWidgetClass,
				     form4,
				     XmNx, 40,
				     XmNy, 340,
				     XmNwidth, 140,
				     XmNheight, 30,
			      RES_CONVERT(XmNlabelString, "Middle Initial"),
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				     XmNleftOffset, 0,
				     XmNleftWidget, label1,
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 10,
				     XmNtopWidget, label4,
				     NULL);


    /* Creation of label6 */
    label6 = XtVaCreateManagedWidget("label6",
				     xmLabelWidgetClass,
				     form4,
				     XmNx, 40,
				     XmNy, 380,
				     XmNwidth, 140,
				     XmNheight, 30,
			    RES_CONVERT(XmNlabelString, "Birth (yyyymmdd)"),
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				     XmNleftOffset, 0,
				     XmNleftWidget, label1,
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 10,
				     XmNtopWidget, label5,
				     NULL);


    /* Creation of label7 */
    label7 = XtVaCreateManagedWidget("label7",
				     xmLabelWidgetClass,
				     form4,
				     XmNx, 40,
				     XmNy, 420,
				     XmNwidth, 140,
				     XmNheight, 30,
				     RES_CONVERT(XmNlabelString, "Sex"),
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				     XmNleftOffset, 0,
				     XmNleftWidget, label1,
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 10,
				     XmNtopWidget, label6,
				     NULL);


    /* Creation of wPatientUIDTxt */
    wPatientUIDTxt = XtVaCreateManagedWidget("wPatientUIDTxt",
					     xmTextWidgetClass,
					     form4,
					     XmNwidth, 180,
					     XmNx, 190,
					     XmNy, 180,
					     XmNheight, 30,
					     XmNeditable, FALSE,
					 XmNleftAttachment, XmATTACH_WIDGET,
					     XmNleftOffset, 10,
					     XmNleftWidget, label1,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					     XmNtopOffset, 0,
					     XmNtopWidget, label1,
					     NULL);


    /* Creation of wPatientIDTxt */
    wPatientIDTxt = XtVaCreateManagedWidget("wPatientIDTxt",
					    xmTextWidgetClass,
					    form4,
					    XmNwidth, 180,
					    XmNx, 190,
					    XmNy, 220,
					    XmNheight, 30,
					    XmNeditable, FALSE,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
					    XmNleftOffset, 0,
					    XmNleftWidget, wPatientUIDTxt,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					    XmNtopOffset, 0,
					    XmNtopWidget, label2,
					    NULL);


    /* Creation of wLastNameTxt */
    wLastNameTxt = XtVaCreateManagedWidget("wLastNameTxt",
					   xmTextWidgetClass,
					   form4,
					   XmNwidth, 180,
					   XmNx, 190,
					   XmNy, 260,
					   XmNheight, 30,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
					   XmNleftOffset, 0,
					   XmNleftWidget, wPatientUIDTxt,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					   XmNtopOffset, 0,
					   XmNtopWidget, label3,
					   NULL);


    /* Creation of wFirstNameTxt */
    wFirstNameTxt = XtVaCreateManagedWidget("wFirstNameTxt",
					    xmTextWidgetClass,
					    form4,
					    XmNwidth, 180,
					    XmNx, 190,
					    XmNy, 300,
					    XmNheight, 30,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
					    XmNleftOffset, 0,
					    XmNleftWidget, wPatientUIDTxt,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					    XmNtopOffset, 0,
					    XmNtopWidget, label4,
					    NULL);


    /* Creation of wMiddleInitialTxt */
    wMiddleInitialTxt = XtVaCreateManagedWidget("wMiddleInitialTxt",
						xmTextWidgetClass,
						form4,
						XmNwidth, 180,
						XmNx, 190,
						XmNy, 340,
						XmNheight, 30,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
						XmNleftOffset, 0,
					      XmNleftWidget, wPatientUIDTxt,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
						XmNtopOffset, 0,
						XmNtopWidget, label5,
						NULL);


    /* Creation of wBirthDateTxt */
    wBirthDateTxt = XtVaCreateManagedWidget("wBirthDateTxt",
					    xmTextWidgetClass,
					    form4,
					    XmNwidth, 180,
					    XmNx, 190,
					    XmNy, 380,
					    XmNheight, 30,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
					    XmNleftOffset, 0,
					    XmNleftWidget, wPatientUIDTxt,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					    XmNtopOffset, 0,
					    XmNtopWidget, label6,
					    NULL);


    /* Creation of rowColumn1 */
    rowColumn1 = XtVaCreateManagedWidget("rowColumn1",
					 xmRowColumnWidgetClass,
					 form4,
					 XmNwidth, 180,
					 XmNheight, 80,
					 XmNx, 190,
					 XmNy, 420,
					 XmNradioBehavior, TRUE,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
					 XmNleftOffset, 10,
					 XmNleftWidget, wPatientUIDTxt,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					 XmNtopOffset, 0,
					 XmNtopWidget, label7,
					 NULL);


    /* Creation of wMaleSexToggle */
    wMaleSexToggle = XtVaCreateManagedWidget("wMaleSexToggle",
					     xmToggleButtonWidgetClass,
					     rowColumn1,
					     XmNx, 3,
					     XmNy, 3,
					     XmNwidth, 177,
					     XmNheight, 26,
					RES_CONVERT(XmNlabelString, "Male"),
					     NULL);


    /* Creation of wFemaleSexToggle */
    wFemaleSexToggle = XtVaCreateManagedWidget("wFemaleSexToggle",
					       xmToggleButtonWidgetClass,
					       rowColumn1,
					       XmNx, 3,
					       XmNy, 3,
					       XmNwidth, 177,
					       XmNheight, 26,
				      RES_CONVERT(XmNlabelString, "Female"),
					       NULL);


    /* Creation of pushButton1 */
    pushButton1 = XtVaCreateManagedWidget("pushButton1",
					  xmPushButtonWidgetClass,
					  form4,
					  XmNx, 20,
					  XmNy, 530,
					  XmNwidth, 110,
					  XmNheight, 50,
					  RES_CONVERT(XmNlabelString, "Add"),
					  NULL);
    XtAddCallback(pushButton1, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton1,
		  (XtPointer) NULL);



    /* Creation of pushButton2 */
    pushButton2 = XtVaCreateManagedWidget("pushButton2",
					  xmPushButtonWidgetClass,
					  form4,
					  XmNx, 130,
					  XmNy, 530,
					  XmNwidth, 110,
					  XmNheight, 50,
				      RES_CONVERT(XmNlabelString, "Delete"),
					  NULL);
    XtAddCallback(pushButton2, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton2,
		  (XtPointer) NULL);



    /* Creation of pushButton3 */
    pushButton3 = XtVaCreateManagedWidget("pushButton3",
					  xmPushButtonWidgetClass,
					  form4,
					  XmNx, 240,
					  XmNy, 530,
					  XmNwidth, 110,
					  XmNheight, 50,
				      RES_CONVERT(XmNlabelString, "Update"),
					  NULL);
    XtAddCallback(pushButton3, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton3,
		  (XtPointer) NULL);



    /* Creation of pushButton8 */
    pushButton8 = XtVaCreateManagedWidget("pushButton8",
					  xmPushButtonWidgetClass,
					  form4,
					  XmNx, 20,
					  XmNy, 600,
					  XmNwidth, 330,
					  XmNheight, 50,
			       RES_CONVERT(XmNlabelString, "Load Database"),
					  NULL);
    XtAddCallback(pushButton8, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton8,
		  (XtPointer) NULL);



    /* Creation of label16 */
    label16 = XtVaCreateManagedWidget("label16",
				      xmLabelWidgetClass,
				      databaseForm,
				      XmNx, 390,
				      XmNy, 240,
				      XmNwidth, 140,
				      XmNheight, 30,
			     RES_CONVERT(XmNlabelString, "AccessionNumber"),
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				      XmNleftWidget, label8,
				      XmNtopAttachment, XmATTACH_WIDGET,
				      XmNtopOffset, 10,
				      XmNtopWidget, label8,
				      NULL);


    /* Creation of label9 */
    label9 = XtVaCreateManagedWidget("label9",
				     xmLabelWidgetClass,
				     databaseForm,
				     XmNx, 390,
				     XmNy, 240,
				     XmNwidth, 140,
				     XmNheight, 30,
				     RES_CONVERT(XmNlabelString, "Study ID"),
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				     XmNleftOffset, 0,
				     XmNleftWidget, label8,
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 10,
				     XmNtopWidget, label16,
				     NULL);


    /* Creation of label10 */
    label10 = XtVaCreateManagedWidget("label10",
				      xmLabelWidgetClass,
				      databaseForm,
				      XmNx, 390,
				      XmNy, 270,
				      XmNwidth, 140,
				      XmNheight, 30,
				  RES_CONVERT(XmNlabelString, "Study Date"),
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				      XmNleftOffset, 0,
				      XmNleftWidget, label8,
				      XmNtopAttachment, XmATTACH_WIDGET,
				      XmNtopOffset, 10,
				      XmNtopWidget, label9,
				      NULL);


    /* Creation of label11 */
    label11 = XtVaCreateManagedWidget("label11",
				      xmLabelWidgetClass,
				      databaseForm,
				      XmNx, 390,
				      XmNy, 300,
				      XmNwidth, 140,
				      XmNheight, 30,
				  RES_CONVERT(XmNlabelString, "Study Time"),
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				      XmNleftOffset, 0,
				      XmNleftWidget, label8,
				      XmNtopAttachment, XmATTACH_WIDGET,
				      XmNtopOffset, 10,
				      XmNtopWidget, label10,
				      NULL);


    /* Creation of label12 */
    label12 = XtVaCreateManagedWidget("label12",
				      xmLabelWidgetClass,
				      databaseForm,
				      XmNx, 400,
				      XmNy, 420,
				      XmNwidth, 140,
				      XmNheight, 30,
			      RES_CONVERT(XmNlabelString, "Study Location"),
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				      XmNleftOffset, 0,
				      XmNleftWidget, label8,
				      XmNtopAttachment, XmATTACH_WIDGET,
				      XmNtopOffset, 10,
				      XmNtopWidget, label11,
				      NULL);


    /* Creation of label13 */
    label13 = XtVaCreateManagedWidget("label13",
				      xmLabelWidgetClass,
				      databaseForm,
				      XmNx, 390,
				      XmNy, 450,
				      XmNwidth, 140,
				      XmNheight, 30,
		       RES_CONVERT(XmNlabelString, "Procedure Description"),
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				      XmNleftOffset, 0,
				      XmNleftWidget, label8,
				      XmNtopAttachment, XmATTACH_WIDGET,
				      XmNtopOffset, 10,
				      XmNtopWidget, label12,
				      NULL);


    /* Creation of label14 */
    label14 = XtVaCreateManagedWidget("label14",
				      xmLabelWidgetClass,
				      databaseForm,
				      XmNx, 400,
				      XmNy, 480,
				      XmNwidth, 140,
				      XmNheight, 30,
			 RES_CONVERT(XmNlabelString, "Requested Procedure"),
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				      XmNleftOffset, 0,
				      XmNleftWidget, label8,
				      XmNtopAttachment, XmATTACH_WIDGET,
				      XmNtopOffset, 10,
				      XmNtopWidget, label13,
				      NULL);


    /* Creation of label15 */
    label15 = XtVaCreateManagedWidget("label15",
				      xmLabelWidgetClass,
				      databaseForm,
				      XmNx, 410,
				      XmNy, 510,
				      XmNwidth, 140,
				      XmNheight, 30,
				RES_CONVERT(XmNlabelString, "Study Status"),
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				      XmNleftOffset, 0,
				      XmNleftWidget, label8,
				      XmNtopAttachment, XmATTACH_WIDGET,
				      XmNtopOffset, 10,
				      XmNtopWidget, label14,
				      NULL);


    /* Creation of wStudyIDTxt */
    wStudyIDTxt = XtVaCreateManagedWidget("wStudyIDTxt",
					  xmTextWidgetClass,
					  databaseForm,
					  XmNwidth, 180,
					  XmNx, 560,
					  XmNy, 230,
					  XmNheight, 30,
					  XmNeditable, FALSE,
					  XmNleftAttachment, XmATTACH_WIDGET,
					  XmNleftOffset, 10,
					  XmNleftWidget, label8,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					  XmNtopOffset, 0,
					  XmNtopWidget, label9,
					  NULL);


    /* Creation of wStudyDateTxt */
    wStudyDateTxt = XtVaCreateManagedWidget("wStudyDateTxt",
					    xmTextWidgetClass,
					    databaseForm,
					    XmNwidth, 180,
					    XmNx, 560,
					    XmNy, 260,
					    XmNheight, 30,
					    XmNeditable, TRUE,
					 XmNleftAttachment, XmATTACH_WIDGET,
					    XmNleftOffset, 10,
					    XmNleftWidget, label8,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					    XmNtopOffset, 0,
					    XmNtopWidget, label10,
					    NULL);


    /* Creation of wStudyTimeTxt */
    wStudyTimeTxt = XtVaCreateManagedWidget("wStudyTimeTxt",
					    xmTextWidgetClass,
					    databaseForm,
					    XmNwidth, 180,
					    XmNx, 560,
					    XmNy, 290,
					    XmNheight, 30,
					    XmNeditable, TRUE,
					 XmNleftAttachment, XmATTACH_WIDGET,
					    XmNleftOffset, 10,
					    XmNleftWidget, label8,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					    XmNtopOffset, 0,
					    XmNtopWidget, label11,
					    NULL);


    /* Creation of wStudyLocationTxt */
    wStudyLocationTxt = XtVaCreateManagedWidget("wStudyLocationTxt",
						xmTextWidgetClass,
						databaseForm,
						XmNwidth, 180,
						XmNx, 560,
						XmNy, 320,
						XmNheight, 30,
						XmNeditable, TRUE,
					 XmNleftAttachment, XmATTACH_WIDGET,
						XmNleftOffset, 10,
						XmNleftWidget, label8,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
						XmNtopOffset, 0,
						XmNtopWidget, label12,
						NULL);


    /* Creation of wProcedureDescriptionTxt */
    wProcedureDescriptionTxt = XtVaCreateManagedWidget("wProcedureDescriptionTxt",
						       xmTextWidgetClass,
						       databaseForm,
						       XmNwidth, 180,
						       XmNx, 560,
						       XmNy, 350,
						       XmNheight, 30,
						       XmNeditable, TRUE,
					 XmNleftAttachment, XmATTACH_WIDGET,
						       XmNleftOffset, 10,
						       XmNleftWidget, label8,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
						       XmNtopOffset, 0,
						       XmNtopWidget, label13,
						       NULL);


    /* Creation of wRequestedProcedureTxt */
    wRequestedProcedureTxt = XtVaCreateManagedWidget("wRequestedProcedureTxt",
						     xmTextWidgetClass,
						     databaseForm,
						     XmNwidth, 180,
						     XmNx, 560,
						     XmNy, 380,
						     XmNheight, 30,
						     XmNeditable, TRUE,
					 XmNleftAttachment, XmATTACH_WIDGET,
						     XmNleftOffset, 10,
						     XmNleftWidget, label8,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
						     XmNtopOffset, 0,
						     XmNtopWidget, label14,
						     NULL);


    /* Creation of wAccessionNumberTxt */
    wAccessionNumberTxt = XtVaCreateManagedWidget("wAccessionNumberTxt",
						  xmTextWidgetClass,
						  databaseForm,
						  XmNwidth, 180,
						  XmNx, 560,
						  XmNy, 230,
						  XmNheight, 30,
						  XmNeditable, FALSE,
					 XmNleftAttachment, XmATTACH_WIDGET,
						  XmNleftOffset, 10,
						  XmNleftWidget, label8,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
						  XmNtopOffset, 0,
						  XmNtopWidget, label16,
						  NULL);


    /* Creation of optionMenu_p1 */
    optionMenu_p1_shell = XtVaCreatePopupShell("optionMenu_p1_shell",
				       xmMenuShellWidgetClass, databaseForm,
					       XmNwidth, 1,
					       XmNheight, 1,
					       XmNallowShellResize, TRUE,
					       XmNoverrideRedirect, TRUE,
					       NULL);

    optionMenu_p1 = XtVaCreateWidget("optionMenu_p1",
				     xmRowColumnWidgetClass,
				     optionMenu_p1_shell,
				     XmNrowColumnType, XmMENU_PULLDOWN,
				     NULL);


    /* Creation of wStudyStatusCreatedButton */
    wStudyStatusCreatedButton = XtVaCreateManagedWidget("wStudyStatusCreatedButton",
						    xmPushButtonWidgetClass,
							optionMenu_p1,
				     RES_CONVERT(XmNlabelString, "CREATED"),
							NULL);
    XtAddCallback(wStudyStatusCreatedButton, XmNactivateCallback,
		  (XtCallbackProc) activateCB_wStudyStatusCreatedButton,
		  (XtPointer) NULL);



    /* Creation of wStudyStatusScheduledButton */
    wStudyStatusScheduledButton = XtVaCreateManagedWidget("wStudyStatusScheduledButton",
						    xmPushButtonWidgetClass,
							  optionMenu_p1,
				   RES_CONVERT(XmNlabelString, "SCHEDULED"),
							  NULL);
    XtAddCallback(wStudyStatusScheduledButton, XmNactivateCallback,
		  (XtCallbackProc) activateCB_wStudyStatusScheduledButton,
		  (XtPointer) NULL);



    /* Creation of wStudyStatusArrivedButton */
    wStudyStatusArrivedButton = XtVaCreateManagedWidget("wStudyStatusArrivedButton",
						    xmPushButtonWidgetClass,
							optionMenu_p1,
				     RES_CONVERT(XmNlabelString, "ARRIVED"),
							NULL);
    XtAddCallback(wStudyStatusArrivedButton, XmNactivateCallback,
		  (XtCallbackProc) activateCB_wStudyStatusArrivedButton,
		  (XtPointer) NULL);



    /* Creation of wStudyStatusStartedButton */
    wStudyStatusStartedButton = XtVaCreateManagedWidget("wStudyStatusStartedButton",
						    xmPushButtonWidgetClass,
							optionMenu_p1,
				     RES_CONVERT(XmNlabelString, "STARTED"),
							NULL);
    XtAddCallback(wStudyStatusStartedButton, XmNactivateCallback,
		  (XtCallbackProc) activateCB_wStudyStatusStartedButton,
		  (XtPointer) NULL);



    /* Creation of wStudyStatusCompletedButton */
    wStudyStatusCompletedButton = XtVaCreateManagedWidget("wStudyStatusCompletedButton",
						    xmPushButtonWidgetClass,
							  optionMenu_p1,
				   RES_CONVERT(XmNlabelString, "COMPLETED"),
							  NULL);
    XtAddCallback(wStudyStatusCompletedButton, XmNactivateCallback,
		  (XtCallbackProc) activateCB_wStudyStatusCompletedButton,
		  (XtPointer) NULL);



    /* Creation of wStudyStatusVerifiedButton */
    wStudyStatusVerifiedButton = XtVaCreateManagedWidget("wStudyStatusVerifiedButton",
						    xmPushButtonWidgetClass,
							 optionMenu_p1,
				    RES_CONVERT(XmNlabelString, "VERIFIED"),
							 NULL);
    XtAddCallback(wStudyStatusVerifiedButton, XmNactivateCallback,
		  (XtCallbackProc) activateCB_wStudyStatusVerifiedButton,
		  (XtPointer) NULL);



    /* Creation of wStudyStatusReadButton */
    wStudyStatusReadButton = XtVaCreateManagedWidget("wStudyStatusReadButton",
						     xmPushButtonWidgetClass,
						     optionMenu_p1,
					RES_CONVERT(XmNlabelString, "READ"),
						     NULL);
    XtAddCallback(wStudyStatusReadButton, XmNactivateCallback,
		  (XtCallbackProc) activateCB_wStudyStatusReadButton,
		  (XtPointer) NULL);



    /* Creation of wStudyStatusMenu */
    wStudyStatusMenu = XtVaCreateManagedWidget("wStudyStatusMenu",
					       xmRowColumnWidgetClass,
					       databaseForm,
					    XmNrowColumnType, XmMENU_OPTION,
					       XmNsubMenuId, optionMenu_p1,
					       XmNx, 630,
					       XmNy, 530,
					       XmNwidth, 170,
					       XmNheight, 30,
					       XmNorientation, XmHORIZONTAL,
					       NULL);


    /* Creation of frame3 */
    frame3 = XtVaCreateManagedWidget("frame3",
				     xmFrameWidgetClass,
				     databaseForm,
				     XmNwidth, 900,
				     XmNheight, 80,
				     XmNx, 10,
				     XmNy, 670,
				     XmNresizable, FALSE,
				     NULL);


    /* Creation of scrolledWindowText1 */
    scrolledWindowText1 = XtVaCreateManagedWidget("scrolledWindowText1",
						xmScrolledWindowWidgetClass,
						  frame3,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNx, 130,
						  XmNy, 10,
						  NULL);


    /* Creation of wDatabaseMessageTxt */
    wDatabaseMessageTxt = XtVaCreateManagedWidget("wDatabaseMessageTxt",
						  xmTextWidgetClass,
						  scrolledWindowText1,
						  XmNwidth, 320,
						  XmNheight, 50,
						  XmNscrollHorizontal, FALSE,
						  XmNscrollLeftSide, FALSE,
						  NULL);



    return (databaseForm);
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget
create_databaseForm(swidget _UxUxParent)
{
    Widget rtrn;

    UxParent = _UxUxParent;

    rtrn = _Uxbuild_databaseForm();

    return (rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/
