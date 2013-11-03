! UIMX ascii 2.5 key: 3914                                                      

*databaseForm.class: form
*databaseForm.gbldecl: /*\
          Copyright (C) 1993, 1994, RSNA and Washington University\
 \
          The software and supporting documentation for the Radiological\
          Society of North America (RSNA) 1993, 1994 Digital Imaging and\
          Communications in Medicine (DICOM) Demonstration were developed\
          at the\
                  Electronic Radiology Laboratory\
                  Mallinckrodt Institute of Radiology\
                  Washington University School of Medicine\
                  510 S. Kingshighway Blvd.\
                  St. Louis, MO 63110\
          as part of the 1993, 1994 DICOM Central Test Node project for, and\
          under contract with, the Radiological Society of North America.\
 \
          THIS SOFTWARE IS MADE AVAILABLE, AS IS, AND NEITHER RSNA NOR\
          WASHINGTON UNIVERSITY MAKE ANY WARRANTY ABOUT THE SOFTWARE, ITS\
          PERFORMANCE, ITS MERCHANTABILITY OR FITNESS FOR ANY PARTICULAR\
          USE, FREEDOM FROM ANY COMPUTER DISEASES OR ITS CONFORMITY TO ANY\
          SPECIFICATION. THE ENTIRE RISK AS TO QUALITY AND PERFORMANCE OF\
          THE SOFTWARE IS WITH THE USER.\
 \
          Copyright of the software and supporting documentation is\
          jointly owned by RSNA and Washington University, and free access\
          is hereby granted as a license to use this software, copy this\
          software and prepare derivative works based upon this software.\
          However, any distribution of this software source code or\
          supporting documentation or derivative works (source code and\
          supporting documentation) must include the three paragraphs of\
          the copyright notice.\
*/\
/* Copyright marker.  Copyright will be inserted above.  Do not remove */\
\
/*\
**                   Electronic Radiology Laboratory\
**                 Mallinckrodt Institute of Radiology\
**              Washington University School of Medicine\
**\
** Module Name(s):	create_databaseForm\
**			loadPatientList\
** Author, Date:	Steve Moore, Summer 1994\
** Intent:		This file contains the functions that provide the\
**			user interface for the database functions for the\
**			WLMA application.  These database functions include\
**			adding patients and studies to the system and\
**			modifying values for patients and studies.  When\
**			patients and studies are added, these interface\
**			functions do not send any DICOM events (this is\
**			a very structured demonstration).\
**			The public entry points are routines to create\
**			the widget hierarchy for this interface and\
**			to load the patient list from data in the\
**			existing database.\
** Last Update:         $Author: smm $, $Date: 1995-12-22 17:27:24 $\
** Source File:         $RCSfile: databaseForm.i,v $\
** Revision:            $Revision: 1.7 $\
** Status:              $State: Exp $\
*/\
 \
static char rcsid[] = "$Revision: 1.7 $ $RCSfile: databaseForm.i,v $";\
\
\
#include <stdio.h>\
\
#include "dicom.h"\
#include "condition.h"\
#include "lst.h"\
#include "dicom_objects.h"\
#include "tbl.h"\
#include "manage.h"\
#include "fis.h"\
#include "mut.h"\
\
#include "wlma.h"\
#include "format.h"\
\
extern FIS_HANDLE *fisHandle;\
static CONDITION readPatientInformation(FIS_PATIENTRECORD *p);\
static void displayPatientInformation(FIS_PATIENTRECORD *p);\
\
static CONDITION readStudyInformation(FIS_STUDYRECORD *s);\
static void readStudyStatusID(char *status);\
static void displayStudyInformation(FIS_STUDYRECORD *s);\
static void displayStudyStatusID(char *status);\
\
static LST_HEAD *patientList = NULL;\
static LST_HEAD *studyList = NULL;\
static char studyStatusID[DICOM_CS_LENGTH+1] = "CREATED";
*databaseForm.ispecdecl:
*databaseForm.funcdecl: swidget create_databaseForm(swidget UxParent)
*databaseForm.funcname: create_databaseForm
*databaseForm.funcdef: "swidget", "<create_databaseForm>(%)"
*databaseForm.argdecl: swidget UxParent;
*databaseForm.arglist: UxParent
*databaseForm.arglist.UxParent: "swidget", "%UxParent%"
*databaseForm.icode:
*databaseForm.fcode: return(rtrn);\

*databaseForm.auxdecl: void errorText(char *txt);\
\
static CONDITION readPatientInformation (FIS_PATIENTRECORD *p)\
{\
  CTNBOOLEAN\
    nullFlag;\
  CONDITION\
    cond;\
  char\
    lastName[60],\
    firstName[60],\
    middleInitial[10];\
\
    memset(p, 0, sizeof(*p));\
    p->Type = FIS_K_PATIENT;\
\
    cond = MUT_ScanWidget(wPatientIDTxt, MUT_TEXT, &nullFlag, p->PatID);\
    if (cond != MUT_NORMAL || nullFlag) {\
	(void) COND_PopCondition(TRUE);\
	p->PatID[0] = '\0';\
    }\
    cond = MUT_ScanWidget(wPatientUIDTxt, MUT_TEXT, &nullFlag, p->PatUID);\
    if (cond != MUT_NORMAL || nullFlag) {\
	(void) COND_PopCondition(TRUE);\
	p->PatUID[0] = '\0';\
    }\
\
    cond = MUT_ScanWidget(wLastNameTxt, MUT_TEXT, &nullFlag, lastName);\
    if (cond != MUT_NORMAL || nullFlag) {\
	errorText("No last name entered");\
	return 0;\
    }\
    cond = MUT_ScanWidget(wFirstNameTxt, MUT_TEXT, &nullFlag, firstName);\
    if (cond != MUT_NORMAL || nullFlag) {\
	errorText("No first name entered");\
	return 0;\
    }\
    cond = MUT_ScanWidget(wMiddleInitialTxt, MUT_TEXT, &nullFlag, middleInitial);\
    if (cond != MUT_NORMAL || nullFlag) \
	middleInitial[0] = '\0';\
    (void)sprintf(p->PatNam, "%-s^%-s^%c", lastName, firstName, middleInitial[0]);\
\
    cond = MUT_ScanWidget(wBirthDateTxt, MUT_TEXT, &nullFlag, p->PatBirDat);\
    if (cond != MUT_NORMAL || nullFlag) {\
	errorText("No birthdate entered");\
	return 0;\
    }\
\
    if (XmToggleButtonGetState(wMaleSexToggle))\
	strcpy(p->PatSex, "M");\
    else\
	strcpy(p->PatSex, "F");\
\
    errorText("");\
    return 1;\
}\
\
static void displayPatientInformation(FIS_PATIENTRECORD *p)\
{\
  char\
    lastName[60] ,\
    firstName[60] ,\
    middleInitial[60];\
  int\
    index = 0,\
    charsScanned = 0;\
\
    if (sscanf(&p->PatNam[index], "%[^^] %n", lastName, &charsScanned) == 1) {\
	index += charsScanned + 1;\
    } else\
	strcpy(lastName, p->PatNam);\
\
    if (sscanf(&p->PatNam[index], "%[^^] %n", firstName, &charsScanned) == 1) {\
	index += charsScanned + 1;\
    } else\
	firstName[0] = '\0';\
\
    if (sscanf(&p->PatNam[index], "%[^^]", middleInitial) != 1)\
	middleInitial[0] = '\0';\
\
    XmTextSetString(wLastNameTxt, lastName);\
    XmTextSetString(wFirstNameTxt, firstName);\
    XmTextSetString(wMiddleInitialTxt, middleInitial);\
    XmTextSetString(wPatientUIDTxt, p->PatUID);\
    XmTextSetString(wPatientIDTxt, p->PatID);\
    XmTextSetString(wBirthDateTxt, p->PatBirDat);\
    if (p->PatSex[0] == 'M') {\
	XmToggleButtonSetState(wMaleSexToggle, True, False);\
	XmToggleButtonSetState(wFemaleSexToggle, False, False);\
    } else {\
	XmToggleButtonSetState(wFemaleSexToggle, True, False);\
	XmToggleButtonSetState(wMaleSexToggle, False, False);\
    }\
}\
void loadPatientList()\
{\
  CONDITION\
    cond;\
  char\
    b[80];\
\
    if (patientList == NULL)\
	patientList = LST_Create();\
\
    cond = FIS_Get(&fisHandle, FIS_K_PATIENT, FIS_K_PATIENT, NULL, 0, patientList);\
    if (cond != FIS_NORMAL) {\
	COND_DumpConditions();\
	return;\
    }\
    MUT_LoadList(wPatientList, patientList, formatPatientRecord, b);\
}\
\
\
\
static CONDITION readStudyInformation(FIS_STUDYRECORD *s)\
{\
  CTNBOOLEAN\
    nullFlag;\
  CONDITION\
    cond;\
\
    s->Flag = 0;\
\
    cond = MUT_ScanWidget(wStudyUIDTxt, MUT_TEXT, &nullFlag, s->StuInsUID);\
    if (cond != MUT_NORMAL || nullFlag) {\
	(void) COND_PopCondition(TRUE);\
	s->StuInsUID[0] = '\0';\
    } else\
	s->Flag |= FIS_K_STU_STUINSUID;\
\
    cond = MUT_ScanWidget(wAccessionNumberTxt, MUT_TEXT, &nullFlag, s->AccNum);\
    if (cond != MUT_NORMAL || nullFlag) {\
	(void)COND_PopCondition(TRUE);\
	s->AccNum[0] = '\0';\
    }\
    s->Flag |= FIS_K_STU_ACCNUM;\
\
    cond = MUT_ScanWidget(wStudyIDTxt, MUT_TEXT, &nullFlag, s->StuID);\
    if (cond != MUT_NORMAL || nullFlag) {\
	(void) COND_PopCondition(TRUE);\
	s->StuID[0] = '\0';\
    }\
    s->Flag |= FIS_K_STU_STUID;\
\
    cond = MUT_ScanWidget(wStudyDateTxt, MUT_TEXT, &nullFlag, s->SchStuStaDat);\
    if (cond != MUT_NORMAL || nullFlag)\
	return 0;\
    s->Flag |= FIS_K_STU_SCHSTUSTADAT;\
\
    cond = MUT_ScanWidget(wStudyTimeTxt, MUT_TEXT, &nullFlag, s->SchStuStaTim);\
    if (cond != MUT_NORMAL || nullFlag)\
	return 0;\
    s->Flag |= FIS_K_STU_SCHSTUSTATIM;\
\
    cond = MUT_ScanWidget(wStudyLocationTxt, MUT_TEXT, &nullFlag, s->SchStuLoc);\
    if (cond != MUT_NORMAL || nullFlag)\
	return 0;\
    s->Flag |= FIS_K_STU_SCHSTULOC;\
\
    cond = MUT_ScanWidget(wProcedureDescriptionTxt, MUT_TEXT, &nullFlag, s->ProDes);\
    if (cond != MUT_NORMAL || nullFlag)\
	return 0;\
    s->Flag |= FIS_K_STU_PRODES;\
\
    readStudyStatusID(s->StuStaID);\
    s->Flag |= FIS_K_STU_STUSTAID;\
\
    return 1;\
}\
\
typedef struct {\
    Widget *w;\
    char *s;\
} W_MAP;\
\
static void readStudyStatusID(char *s)\
{\
  Arg arg;\
  Widget w;\
\
    XtSetArg(arg, XmNmenuHistory, &w);\
    XtGetValues(optionMenu_p1, &arg, 1);\
\
#ifdef RUNTIME\
    if (w == wStudyStatusCreatedButton)\
	strcpy(s, "CREATED");\
    else if (w == wStudyStatusScheduledButton)\
	strcpy(s, "SCHEDULED");\
    else if (w == wStudyStatusArrivedButton)\
	strcpy(s, "ARRIVED");\
    else if (w == wStudyStatusCompletedButton)\
	strcpy(s, "COMPLETED");\
    else if (w == wStudyStatusVerifiedButton)\
	strcpy(s, "VERIFIED");\
    else if (w == wStudyStatusReadButton)\
	strcpy(s, "READ");\
    else\
	strcpy(s, "CREATED");\
#endif\
\
}\
\
static void displayStudyInformation(FIS_STUDYRECORD *s)\
{\
    XmTextSetString(wStudyUIDTxt, s->StuInsUID);\
    XmTextSetString(wAccessionNumberTxt, s->AccNum);\
    XmTextSetString(wStudyIDTxt, s->StuID);\
    XmTextSetString(wStudyDateTxt, s->SchStuStaDat);\
    XmTextSetString(wStudyTimeTxt, s->SchStuStaTim);\
    XmTextSetString(wStudyLocationTxt, s->SchStuLoc);\
    XmTextSetString(wProcedureDescriptionTxt, s->ProDes);\
    displayStudyStatusID(s->StuStaID);\
}\
\
static void displayStudyStatusID(char *s)\
{\
  Widget w;\
  XButtonEvent event;\
  Arg arg;\
\
#ifdef RUNTIME\
    if (strcmp(s, "CREATED") == 0)\
	w = wStudyStatusCreatedButton;\
    else if (strcmp(s, "SCHEDULED") == 0)\
	w = wStudyStatusScheduledButton;\
    else if (strcmp(s, "ARRIVED") == 0)\
	w = wStudyStatusArrivedButton;\
    else if (strcmp(s, "STARTED") == 0)\
	w = wStudyStatusStartedButton;\
    else if (strcmp(s, "COMPLETED") == 0)\
	w = wStudyStatusCompletedButton;\
    else if (strcmp(s, "VERIFIED") == 0)\
	w = wStudyStatusVerifiedButton;\
    else if (strcmp(s, "READ") == 0)\
	w = wStudyStatusReadButton;\
    else\
	w = wStudyStatusCreatedButton;\
\
    XtSetArg(arg, XmNmenuHistory, w);\
    XtSetValues(optionMenu_p1, &arg, 1);\
    XtSetValues(wStudyStatusMenu, &arg, 1);\
/*\
    XtCallActionProc(w, "ArmAndActivate", event, NULL, 0);\
*/\
\
#endif\
}\
\
\
static void loadStudyList()\
{\
  CONDITION\
    cond;\
  char\
    b[80];\
  FIS_PATIENTRECORD\
    p;\
\
    if (studyList == NULL)\
	studyList = LST_Create();\
\
   FIS_ClearList(studyList);\
    cond = readPatientInformation(&p);\
    if (cond == 1) {\
	if (p.PatUID[0] != '\0') {\
	    cond = FIS_Get(&fisHandle, FIS_K_STUDY, FIS_K_PATIENT,\
		p.PatUID, 0, studyList);\
	    if (cond != FIS_NORMAL) {\
		COND_DumpConditions();\
		FIS_ClearList(studyList);\
	    }\
	}\
    }\
    MUT_LoadList(wStudyList, studyList, formatStudyRecord, b);\
}
*databaseForm.static: true
*databaseForm.name: databaseForm
*databaseForm.parent: NO_PARENT
*databaseForm.parentExpression: UxParent
*databaseForm.defaultShell: topLevelShell
*databaseForm.width: 925
*databaseForm.height: 759
*databaseForm.resizePolicy: "resize_none"
*databaseForm.isCompound: "true"
*databaseForm.compoundIcon: "form.xpm"
*databaseForm.compoundName: "form_"
*databaseForm.x: 185
*databaseForm.y: 171
*databaseForm.unitType: "pixels"

*form2.class: form
*form2.static: true
*form2.name: form2
*form2.parent: databaseForm
*form2.width: 531
*form2.height: 150
*form2.resizePolicy: "resize_none"
*form2.isCompound: "true"
*form2.compoundIcon: "form.xpm"
*form2.compoundName: "form_"
*form2.x: 373
*form2.y: 30

*frame2.class: frame
*frame2.static: true
*frame2.name: frame2
*frame2.parent: form2
*frame2.width: 516
*frame2.height: 150
*frame2.isCompound: "true"
*frame2.compoundIcon: "frame.xpm"
*frame2.compoundName: "frame_"
*frame2.x: 15
*frame2.y: 0

*scrolledWindowList2.class: scrolledWindow
*scrolledWindowList2.static: true
*scrolledWindowList2.name: scrolledWindowList2
*scrolledWindowList2.parent: frame2
*scrolledWindowList2.scrollingPolicy: "application_defined"
*scrolledWindowList2.visualPolicy: "variable"
*scrolledWindowList2.scrollBarDisplayPolicy: "static"
*scrolledWindowList2.shadowThickness: 0
*scrolledWindowList2.isCompound: "true"
*scrolledWindowList2.compoundIcon: "scrllist.xpm"
*scrolledWindowList2.compoundName: "scrolled_List"
*scrolledWindowList2.x: 6
*scrolledWindowList2.y: 4

*wStudyList.class: scrolledList
*wStudyList.static: true
*wStudyList.name: wStudyList
*wStudyList.parent: scrolledWindowList2
*wStudyList.width: 510
*wStudyList.height: 146
*wStudyList.browseSelectionCallback: {\
  XmListCallbackStruct *listStruct;\
  FIS_STUDYRECORD *s;\
  int i;\
\
    listStruct = (XmListCallbackStruct *)UxCallbackArg;\
    s = LST_Head(&studyList);\
    (void) LST_Position(&studyList, s);\
    for (i = 2; i <= listStruct->item_position; i++)\
	s = LST_Next(&studyList);\
\
    displayStudyInformation(s);\
\
}

*label8.class: label
*label8.static: true
*label8.name: label8
*label8.parent: databaseForm
*label8.isCompound: "true"
*label8.compoundIcon: "label.xpm"
*label8.compoundName: "label_"
*label8.x: 470
*label8.y: 210
*label8.width: 140
*label8.height: 30
*label8.labelString: "Study UID"

*wStudyUIDTxt.class: text
*wStudyUIDTxt.static: true
*wStudyUIDTxt.name: wStudyUIDTxt
*wStudyUIDTxt.parent: databaseForm
*wStudyUIDTxt.width: 180
*wStudyUIDTxt.isCompound: "true"
*wStudyUIDTxt.compoundIcon: "text.xpm"
*wStudyUIDTxt.compoundName: "text_"
*wStudyUIDTxt.x: 560
*wStudyUIDTxt.y: 200
*wStudyUIDTxt.height: 30
*wStudyUIDTxt.editable: "false"
*wStudyUIDTxt.leftAttachment: "attach_widget"
*wStudyUIDTxt.leftOffset: 10
*wStudyUIDTxt.leftWidget: "label8"
*wStudyUIDTxt.topAttachment: "attach_opposite_widget"
*wStudyUIDTxt.topOffset: 0
*wStudyUIDTxt.topWidget: "label8"

*pushButton4.class: pushButton
*pushButton4.static: true
*pushButton4.name: pushButton4
*pushButton4.parent: databaseForm
*pushButton4.isCompound: "true"
*pushButton4.compoundIcon: "push.xpm"
*pushButton4.compoundName: "push_Button"
*pushButton4.x: 490
*pushButton4.y: 570
*pushButton4.width: 110
*pushButton4.height: 50
*pushButton4.labelString: "Add"
*pushButton4.activateCallback: {\
  CONDITION\
    cond;\
  FIS_PATIENTRECORD\
    p;\
  FIS_STUDYRECORD\
    s;\
\
    XtCallActionProc(wStudyStatusCreatedButton, "ArmAndActivate", NULL, \
	NULL, 0);\
    p.Type = FIS_K_PATIENT;\
    cond = readPatientInformation(&p);\
    if (cond != 1)\
	return;\
\
    s.Type = FIS_K_STUDY;\
    cond = readStudyInformation(&s);\
    if (cond != 1)\
	return;\
\
    cond = FIS_NewRecord(&fisHandle, FIS_K_STUDY, &s);\
    if (cond != FIS_NORMAL) {\
	COND_DumpConditions();\
	return;\
    }\
\
    strcpy(s.PatUID, p.PatUID);\
\
    cond = FIS_Insert(&fisHandle, FIS_K_STUDY, &s);\
    if (cond != FIS_NORMAL) {\
	COND_DumpConditions();\
	return;\
    }\
\
    loadStudyList(); \
}

*pushButton5.class: pushButton
*pushButton5.static: true
*pushButton5.name: pushButton5
*pushButton5.parent: databaseForm
*pushButton5.isCompound: "true"
*pushButton5.compoundIcon: "push.xpm"
*pushButton5.compoundName: "push_Button"
*pushButton5.x: 600
*pushButton5.y: 570
*pushButton5.width: 110
*pushButton5.height: 50
*pushButton5.labelString: "Delete"
*pushButton5.activateCallback: {\
  CONDITION\
    cond;\
  FIS_STUDYRECORD\
    s;\
\
    s.Type = FIS_K_STUDY;\
    cond = readStudyInformation(&s);\
    if (cond != 1)\
	return;\
\
    cond = FIS_Delete(&fisHandle, FIS_K_STUDY, FIS_K_STUDY, s.StuInsUID);\
    if (cond != FIS_NORMAL) {\
	COND_DumpConditions();\
    }\
\
    loadStudyList();\
}

*pushButton6.class: pushButton
*pushButton6.static: true
*pushButton6.name: pushButton6
*pushButton6.parent: databaseForm
*pushButton6.isCompound: "true"
*pushButton6.compoundIcon: "push.xpm"
*pushButton6.compoundName: "push_Button"
*pushButton6.x: 710
*pushButton6.y: 570
*pushButton6.width: 110
*pushButton6.height: 50
*pushButton6.labelString: "Update"
*pushButton6.activateCallback: {\
  CONDITION\
    cond;\
  FIS_STUDYRECORD\
    s;\
\
    s.Type = FIS_K_STUDY;\
    cond = readStudyInformation(&s);\
    if (cond != 1)\
	return;\
\
    cond = FIS_Update(&fisHandle, FIS_K_STUDY, &s);\
    if (cond != FIS_NORMAL) {\
	COND_DumpConditions();\
	return;\
    }\
\
    loadStudyList();\
}

*form4.class: form
*form4.static: true
*form4.name: form4
*form4.parent: databaseForm
*form4.width: 390
*form4.height: 660
*form4.resizePolicy: "resize_none"
*form4.isCompound: "true"
*form4.compoundIcon: "form.xpm"
*form4.compoundName: "form_"
*form4.x: 0
*form4.y: 10

*frame1.class: frame
*frame1.static: true
*frame1.name: frame1
*frame1.parent: form4
*frame1.width: 310
*frame1.height: 150
*frame1.isCompound: "true"
*frame1.compoundIcon: "frame.xpm"
*frame1.compoundName: "frame_"
*frame1.x: 42
*frame1.y: 22

*scrolledWindowList1.class: scrolledWindow
*scrolledWindowList1.static: true
*scrolledWindowList1.name: scrolledWindowList1
*scrolledWindowList1.parent: frame1
*scrolledWindowList1.scrollingPolicy: "application_defined"
*scrolledWindowList1.visualPolicy: "variable"
*scrolledWindowList1.scrollBarDisplayPolicy: "static"
*scrolledWindowList1.shadowThickness: 0
*scrolledWindowList1.isCompound: "true"
*scrolledWindowList1.compoundIcon: "scrllist.xpm"
*scrolledWindowList1.compoundName: "scrolled_List"
*scrolledWindowList1.x: 2
*scrolledWindowList1.y: 2

*wPatientList.class: scrolledList
*wPatientList.static: true
*wPatientList.name: wPatientList
*wPatientList.parent: scrolledWindowList1
*wPatientList.width: 306
*wPatientList.height: 140
*wPatientList.browseSelectionCallback: {\
  XmListCallbackStruct *listStruct;\
  FIS_PATIENTRECORD *p;\
  int i;\
\
    listStruct = (XmListCallbackStruct *)UxCallbackArg;\
    p = LST_Head(&patientList);\
    (void)LST_Position(&patientList, p);\
    for (i = 2; i <= listStruct->item_position; i++)\
	p = LST_Next(&patientList);\
\
    displayPatientInformation(p);\
    loadStudyList();\
\
}

*label1.class: label
*label1.static: true
*label1.name: label1
*label1.parent: form4
*label1.isCompound: "true"
*label1.compoundIcon: "label.xpm"
*label1.compoundName: "label_"
*label1.x: 20
*label1.y: 190
*label1.width: 140
*label1.height: 30
*label1.labelString: "Patient UID"

*label2.class: label
*label2.static: true
*label2.name: label2
*label2.parent: form4
*label2.isCompound: "true"
*label2.compoundIcon: "label.xpm"
*label2.compoundName: "label_"
*label2.x: 40
*label2.y: 220
*label2.width: 140
*label2.height: 30
*label2.labelString: "Patient ID"
*label2.leftAttachment: "attach_opposite_widget"
*label2.leftOffset: 0
*label2.leftWidget: "label1"
*label2.topAttachment: "attach_widget"
*label2.topOffset: 10
*label2.topWidget: "label1"

*label3.class: label
*label3.static: true
*label3.name: label3
*label3.parent: form4
*label3.isCompound: "true"
*label3.compoundIcon: "label.xpm"
*label3.compoundName: "label_"
*label3.x: 40
*label3.y: 260
*label3.width: 140
*label3.height: 30
*label3.labelString: "Last Name"
*label3.leftAttachment: "attach_opposite_widget"
*label3.leftOffset: 0
*label3.leftWidget: "label1"
*label3.topAttachment: "attach_widget"
*label3.topOffset: 10
*label3.topWidget: "label2"

*label4.class: label
*label4.static: true
*label4.name: label4
*label4.parent: form4
*label4.isCompound: "true"
*label4.compoundIcon: "label.xpm"
*label4.compoundName: "label_"
*label4.x: 40
*label4.y: 300
*label4.width: 140
*label4.height: 30
*label4.labelString: "First Name"
*label4.leftAttachment: "attach_opposite_widget"
*label4.leftOffset: 0
*label4.leftWidget: "label1"
*label4.topAttachment: "attach_widget"
*label4.topOffset: 10
*label4.topWidget: "label3"

*label5.class: label
*label5.static: true
*label5.name: label5
*label5.parent: form4
*label5.isCompound: "true"
*label5.compoundIcon: "label.xpm"
*label5.compoundName: "label_"
*label5.x: 40
*label5.y: 340
*label5.width: 140
*label5.height: 30
*label5.labelString: "Middle Initial"
*label5.leftAttachment: "attach_opposite_widget"
*label5.leftOffset: 0
*label5.leftWidget: "label1"
*label5.topAttachment: "attach_widget"
*label5.topOffset: 10
*label5.topWidget: "label4"

*label6.class: label
*label6.static: true
*label6.name: label6
*label6.parent: form4
*label6.isCompound: "true"
*label6.compoundIcon: "label.xpm"
*label6.compoundName: "label_"
*label6.x: 40
*label6.y: 380
*label6.width: 140
*label6.height: 30
*label6.labelString: "Birth (yyyymmdd)"
*label6.leftAttachment: "attach_opposite_widget"
*label6.leftOffset: 0
*label6.leftWidget: "label1"
*label6.topAttachment: "attach_widget"
*label6.topOffset: 10
*label6.topWidget: "label5"

*label7.class: label
*label7.static: true
*label7.name: label7
*label7.parent: form4
*label7.isCompound: "true"
*label7.compoundIcon: "label.xpm"
*label7.compoundName: "label_"
*label7.x: 40
*label7.y: 420
*label7.width: 140
*label7.height: 30
*label7.labelString: "Sex"
*label7.leftAttachment: "attach_opposite_widget"
*label7.leftOffset: 0
*label7.leftWidget: "label1"
*label7.topAttachment: "attach_widget"
*label7.topOffset: 10
*label7.topWidget: "label6"

*wPatientUIDTxt.class: text
*wPatientUIDTxt.static: true
*wPatientUIDTxt.name: wPatientUIDTxt
*wPatientUIDTxt.parent: form4
*wPatientUIDTxt.width: 180
*wPatientUIDTxt.isCompound: "true"
*wPatientUIDTxt.compoundIcon: "text.xpm"
*wPatientUIDTxt.compoundName: "text_"
*wPatientUIDTxt.x: 190
*wPatientUIDTxt.y: 180
*wPatientUIDTxt.height: 30
*wPatientUIDTxt.editable: "false"
*wPatientUIDTxt.leftAttachment: "attach_widget"
*wPatientUIDTxt.leftOffset: 10
*wPatientUIDTxt.leftWidget: "label1"
*wPatientUIDTxt.topAttachment: "attach_opposite_widget"
*wPatientUIDTxt.topOffset: 0
*wPatientUIDTxt.topWidget: "label1"

*wPatientIDTxt.class: text
*wPatientIDTxt.static: true
*wPatientIDTxt.name: wPatientIDTxt
*wPatientIDTxt.parent: form4
*wPatientIDTxt.width: 180
*wPatientIDTxt.isCompound: "true"
*wPatientIDTxt.compoundIcon: "text.xpm"
*wPatientIDTxt.compoundName: "text_"
*wPatientIDTxt.x: 190
*wPatientIDTxt.y: 220
*wPatientIDTxt.height: 30
*wPatientIDTxt.editable: "false"
*wPatientIDTxt.leftAttachment: "attach_opposite_widget"
*wPatientIDTxt.leftOffset: 0
*wPatientIDTxt.leftWidget: "wPatientUIDTxt"
*wPatientIDTxt.topAttachment: "attach_opposite_widget"
*wPatientIDTxt.topOffset: 0
*wPatientIDTxt.topWidget: "label2"

*wLastNameTxt.class: text
*wLastNameTxt.static: true
*wLastNameTxt.name: wLastNameTxt
*wLastNameTxt.parent: form4
*wLastNameTxt.width: 180
*wLastNameTxt.isCompound: "true"
*wLastNameTxt.compoundIcon: "text.xpm"
*wLastNameTxt.compoundName: "text_"
*wLastNameTxt.x: 190
*wLastNameTxt.y: 260
*wLastNameTxt.height: 30
*wLastNameTxt.leftAttachment: "attach_opposite_widget"
*wLastNameTxt.leftOffset: 0
*wLastNameTxt.leftWidget: "wPatientUIDTxt"
*wLastNameTxt.topAttachment: "attach_opposite_widget"
*wLastNameTxt.topOffset: 0
*wLastNameTxt.topWidget: "label3"

*wFirstNameTxt.class: text
*wFirstNameTxt.static: true
*wFirstNameTxt.name: wFirstNameTxt
*wFirstNameTxt.parent: form4
*wFirstNameTxt.width: 180
*wFirstNameTxt.isCompound: "true"
*wFirstNameTxt.compoundIcon: "text.xpm"
*wFirstNameTxt.compoundName: "text_"
*wFirstNameTxt.x: 190
*wFirstNameTxt.y: 300
*wFirstNameTxt.height: 30
*wFirstNameTxt.leftAttachment: "attach_opposite_widget"
*wFirstNameTxt.leftOffset: 0
*wFirstNameTxt.leftWidget: "wPatientUIDTxt"
*wFirstNameTxt.topAttachment: "attach_opposite_widget"
*wFirstNameTxt.topOffset: 0
*wFirstNameTxt.topWidget: "label4"

*wMiddleInitialTxt.class: text
*wMiddleInitialTxt.static: true
*wMiddleInitialTxt.name: wMiddleInitialTxt
*wMiddleInitialTxt.parent: form4
*wMiddleInitialTxt.width: 180
*wMiddleInitialTxt.isCompound: "true"
*wMiddleInitialTxt.compoundIcon: "text.xpm"
*wMiddleInitialTxt.compoundName: "text_"
*wMiddleInitialTxt.x: 190
*wMiddleInitialTxt.y: 340
*wMiddleInitialTxt.height: 30
*wMiddleInitialTxt.leftAttachment: "attach_opposite_widget"
*wMiddleInitialTxt.leftOffset: 0
*wMiddleInitialTxt.leftWidget: "wPatientUIDTxt"
*wMiddleInitialTxt.topAttachment: "attach_opposite_widget"
*wMiddleInitialTxt.topOffset: 0
*wMiddleInitialTxt.topWidget: "label5"

*wBirthDateTxt.class: text
*wBirthDateTxt.static: true
*wBirthDateTxt.name: wBirthDateTxt
*wBirthDateTxt.parent: form4
*wBirthDateTxt.width: 180
*wBirthDateTxt.isCompound: "true"
*wBirthDateTxt.compoundIcon: "text.xpm"
*wBirthDateTxt.compoundName: "text_"
*wBirthDateTxt.x: 190
*wBirthDateTxt.y: 380
*wBirthDateTxt.height: 30
*wBirthDateTxt.leftAttachment: "attach_opposite_widget"
*wBirthDateTxt.leftOffset: 0
*wBirthDateTxt.leftWidget: "wPatientUIDTxt"
*wBirthDateTxt.topAttachment: "attach_opposite_widget"
*wBirthDateTxt.topOffset: 0
*wBirthDateTxt.topWidget: "label6"

*rowColumn1.class: rowColumn
*rowColumn1.static: true
*rowColumn1.name: rowColumn1
*rowColumn1.parent: form4
*rowColumn1.width: 180
*rowColumn1.height: 80
*rowColumn1.isCompound: "true"
*rowColumn1.compoundIcon: "row.xpm"
*rowColumn1.compoundName: "row_Column"
*rowColumn1.x: 190
*rowColumn1.y: 420
*rowColumn1.radioBehavior: "true"
*rowColumn1.leftAttachment: "attach_opposite_widget"
*rowColumn1.leftOffset: 10
*rowColumn1.leftWidget: "wPatientUIDTxt"
*rowColumn1.topAttachment: "attach_opposite_widget"
*rowColumn1.topOffset: 0
*rowColumn1.topWidget: "label7"

*wMaleSexToggle.class: toggleButton
*wMaleSexToggle.static: true
*wMaleSexToggle.name: wMaleSexToggle
*wMaleSexToggle.parent: rowColumn1
*wMaleSexToggle.isCompound: "true"
*wMaleSexToggle.compoundIcon: "toggle.xpm"
*wMaleSexToggle.compoundName: "toggle_Button"
*wMaleSexToggle.x: 3
*wMaleSexToggle.y: 3
*wMaleSexToggle.width: 177
*wMaleSexToggle.height: 26
*wMaleSexToggle.labelString: "Male"

*wFemaleSexToggle.class: toggleButton
*wFemaleSexToggle.static: true
*wFemaleSexToggle.name: wFemaleSexToggle
*wFemaleSexToggle.parent: rowColumn1
*wFemaleSexToggle.isCompound: "true"
*wFemaleSexToggle.compoundIcon: "toggle.xpm"
*wFemaleSexToggle.compoundName: "toggle_Button"
*wFemaleSexToggle.x: 3
*wFemaleSexToggle.y: 3
*wFemaleSexToggle.width: 177
*wFemaleSexToggle.height: 26
*wFemaleSexToggle.labelString: "Female"

*pushButton1.class: pushButton
*pushButton1.static: true
*pushButton1.name: pushButton1
*pushButton1.parent: form4
*pushButton1.isCompound: "true"
*pushButton1.compoundIcon: "push.xpm"
*pushButton1.compoundName: "push_Button"
*pushButton1.x: 20
*pushButton1.y: 530
*pushButton1.width: 110
*pushButton1.height: 50
*pushButton1.labelString: "Add"
*pushButton1.activateCallback: {\
  CONDITION\
    cond;\
  FIS_PATIENTRECORD\
    p;\
\
    p.Type = FIS_K_PATIENT;\
    cond = readPatientInformation(&p);\
    if (cond != 1)\
	return;\
\
    cond = FIS_NewRecord(&fisHandle, FIS_K_PATIENT, &p);\
    if (cond != FIS_NORMAL) {\
	COND_DumpConditions();\
	return;\
    }\
\
    cond = FIS_Insert(&fisHandle, FIS_K_PATIENT, &p);\
    if (cond != FIS_NORMAL) {\
	COND_DumpConditions();\
	return;\
    }\
\
    loadPatientList();\
}

*pushButton2.class: pushButton
*pushButton2.static: true
*pushButton2.name: pushButton2
*pushButton2.parent: form4
*pushButton2.isCompound: "true"
*pushButton2.compoundIcon: "push.xpm"
*pushButton2.compoundName: "push_Button"
*pushButton2.x: 130
*pushButton2.y: 530
*pushButton2.width: 110
*pushButton2.height: 50
*pushButton2.labelString: "Delete"
*pushButton2.activateCallback: {\
  CONDITION\
    cond;\
  FIS_PATIENTRECORD\
    p;\
\
    p.Type = FIS_K_PATIENT;\
    cond = readPatientInformation(&p);\
    if (cond != 1)\
	return;\
\
    cond = FIS_Delete(&fisHandle, FIS_K_PATIENT, FIS_K_PATIENT,  p.PatUID);\
    if (cond != FIS_NORMAL)\
	COND_DumpConditions();\
\
    loadPatientList();\
}

*pushButton3.class: pushButton
*pushButton3.static: true
*pushButton3.name: pushButton3
*pushButton3.parent: form4
*pushButton3.isCompound: "true"
*pushButton3.compoundIcon: "push.xpm"
*pushButton3.compoundName: "push_Button"
*pushButton3.x: 240
*pushButton3.y: 530
*pushButton3.width: 110
*pushButton3.height: 50
*pushButton3.labelString: "Update"
*pushButton3.activateCallback: {\
  CONDITION\
    cond;\
  FIS_PATIENTRECORD\
    p;\
\
    p.Type = FIS_K_PATIENT;\
    cond = readPatientInformation(&p);\
    if (cond != 1)\
	return;\
\
    cond = FIS_Update(&fisHandle, FIS_K_PATIENT, &p);\
    if (cond != FIS_NORMAL) {\
	COND_DumpConditions();\
	return;\
    }\
\
    loadPatientList();\
}

*pushButton8.class: pushButton
*pushButton8.static: true
*pushButton8.name: pushButton8
*pushButton8.parent: form4
*pushButton8.isCompound: "true"
*pushButton8.compoundIcon: "push.xpm"
*pushButton8.compoundName: "push_Button"
*pushButton8.x: 20
*pushButton8.y: 600
*pushButton8.width: 330
*pushButton8.height: 50
*pushButton8.labelString: "Load Database"
*pushButton8.activateCallback: {\
  CONDITION cond;\
\
    XmTextSetString(wDatabaseMessageTxt, "");\
    cond = loadDatabase(&fisHandle);\
    if (cond != 1) {\
	MUT_DisplayConditionText(wDatabaseMessageTxt);\
	(void) COND_PopCondition(TRUE);\
    }\
    loadPatientList();\
}

*label16.class: label
*label16.static: true
*label16.name: label16
*label16.parent: databaseForm
*label16.isCompound: "true"
*label16.compoundIcon: "label.xpm"
*label16.compoundName: "label_"
*label16.x: 390
*label16.y: 240
*label16.width: 140
*label16.height: 30
*label16.labelString: "AccessionNumber"
*label16.leftAttachment: "attach_opposite_widget"
*label16.leftWidget: "label8"
*label16.topAttachment: "attach_widget"
*label16.topOffset: 10
*label16.topWidget: "label8"

*label9.class: label
*label9.static: true
*label9.name: label9
*label9.parent: databaseForm
*label9.isCompound: "true"
*label9.compoundIcon: "label.xpm"
*label9.compoundName: "label_"
*label9.x: 390
*label9.y: 240
*label9.width: 140
*label9.height: 30
*label9.labelString: "Study ID"
*label9.leftAttachment: "attach_opposite_widget"
*label9.leftOffset: 0
*label9.leftWidget: "label8"
*label9.topAttachment: "attach_widget"
*label9.topOffset: 10
*label9.topWidget: "label16"

*label10.class: label
*label10.static: true
*label10.name: label10
*label10.parent: databaseForm
*label10.isCompound: "true"
*label10.compoundIcon: "label.xpm"
*label10.compoundName: "label_"
*label10.x: 390
*label10.y: 270
*label10.width: 140
*label10.height: 30
*label10.labelString: "Study Date"
*label10.leftAttachment: "attach_opposite_widget"
*label10.leftOffset: 0
*label10.leftWidget: "label8"
*label10.topAttachment: "attach_widget"
*label10.topOffset: 10
*label10.topWidget: "label9"

*label11.class: label
*label11.static: true
*label11.name: label11
*label11.parent: databaseForm
*label11.isCompound: "true"
*label11.compoundIcon: "label.xpm"
*label11.compoundName: "label_"
*label11.x: 390
*label11.y: 300
*label11.width: 140
*label11.height: 30
*label11.labelString: "Study Time"
*label11.leftAttachment: "attach_opposite_widget"
*label11.leftOffset: 0
*label11.leftWidget: "label8"
*label11.topAttachment: "attach_widget"
*label11.topOffset: 10
*label11.topWidget: "label10"

*label12.class: label
*label12.static: true
*label12.name: label12
*label12.parent: databaseForm
*label12.isCompound: "true"
*label12.compoundIcon: "label.xpm"
*label12.compoundName: "label_"
*label12.x: 400
*label12.y: 420
*label12.width: 140
*label12.height: 30
*label12.labelString: "Study Location"
*label12.leftAttachment: "attach_opposite_widget"
*label12.leftOffset: 0
*label12.leftWidget: "label8"
*label12.topAttachment: "attach_widget"
*label12.topOffset: 10
*label12.topWidget: "label11"

*label13.class: label
*label13.static: true
*label13.name: label13
*label13.parent: databaseForm
*label13.isCompound: "true"
*label13.compoundIcon: "label.xpm"
*label13.compoundName: "label_"
*label13.x: 390
*label13.y: 450
*label13.width: 140
*label13.height: 30
*label13.labelString: "Procedure Description"
*label13.leftAttachment: "attach_opposite_widget"
*label13.leftOffset: 0
*label13.leftWidget: "label8"
*label13.topAttachment: "attach_widget"
*label13.topOffset: 10
*label13.topWidget: "label12"

*label14.class: label
*label14.static: true
*label14.name: label14
*label14.parent: databaseForm
*label14.isCompound: "true"
*label14.compoundIcon: "label.xpm"
*label14.compoundName: "label_"
*label14.x: 400
*label14.y: 480
*label14.width: 140
*label14.height: 30
*label14.labelString: "Requested Procedure"
*label14.leftAttachment: "attach_opposite_widget"
*label14.leftOffset: 0
*label14.leftWidget: "label8"
*label14.topAttachment: "attach_widget"
*label14.topOffset: 10
*label14.topWidget: "label13"

*label15.class: label
*label15.static: true
*label15.name: label15
*label15.parent: databaseForm
*label15.isCompound: "true"
*label15.compoundIcon: "label.xpm"
*label15.compoundName: "label_"
*label15.x: 410
*label15.y: 510
*label15.width: 140
*label15.height: 30
*label15.labelString: "Study Status"
*label15.leftAttachment: "attach_opposite_widget"
*label15.leftOffset: 0
*label15.leftWidget: "label8"
*label15.topAttachment: "attach_widget"
*label15.topOffset: 10
*label15.topWidget: "label14"

*wStudyIDTxt.class: text
*wStudyIDTxt.static: true
*wStudyIDTxt.name: wStudyIDTxt
*wStudyIDTxt.parent: databaseForm
*wStudyIDTxt.width: 180
*wStudyIDTxt.isCompound: "true"
*wStudyIDTxt.compoundIcon: "text.xpm"
*wStudyIDTxt.compoundName: "text_"
*wStudyIDTxt.x: 560
*wStudyIDTxt.y: 230
*wStudyIDTxt.height: 30
*wStudyIDTxt.editable: "false"
*wStudyIDTxt.leftAttachment: "attach_widget"
*wStudyIDTxt.leftOffset: 10
*wStudyIDTxt.leftWidget: "label8"
*wStudyIDTxt.topAttachment: "attach_opposite_widget"
*wStudyIDTxt.topOffset: 0
*wStudyIDTxt.topWidget: "label9"

*wStudyDateTxt.class: text
*wStudyDateTxt.static: true
*wStudyDateTxt.name: wStudyDateTxt
*wStudyDateTxt.parent: databaseForm
*wStudyDateTxt.width: 180
*wStudyDateTxt.isCompound: "true"
*wStudyDateTxt.compoundIcon: "text.xpm"
*wStudyDateTxt.compoundName: "text_"
*wStudyDateTxt.x: 560
*wStudyDateTxt.y: 260
*wStudyDateTxt.height: 30
*wStudyDateTxt.editable: "true"
*wStudyDateTxt.leftAttachment: "attach_widget"
*wStudyDateTxt.leftOffset: 10
*wStudyDateTxt.leftWidget: "label8"
*wStudyDateTxt.topAttachment: "attach_opposite_widget"
*wStudyDateTxt.topOffset: 0
*wStudyDateTxt.topWidget: "label10"

*wStudyTimeTxt.class: text
*wStudyTimeTxt.static: true
*wStudyTimeTxt.name: wStudyTimeTxt
*wStudyTimeTxt.parent: databaseForm
*wStudyTimeTxt.width: 180
*wStudyTimeTxt.isCompound: "true"
*wStudyTimeTxt.compoundIcon: "text.xpm"
*wStudyTimeTxt.compoundName: "text_"
*wStudyTimeTxt.x: 560
*wStudyTimeTxt.y: 290
*wStudyTimeTxt.height: 30
*wStudyTimeTxt.editable: "true"
*wStudyTimeTxt.leftAttachment: "attach_widget"
*wStudyTimeTxt.leftOffset: 10
*wStudyTimeTxt.leftWidget: "label8"
*wStudyTimeTxt.topAttachment: "attach_opposite_widget"
*wStudyTimeTxt.topOffset: 0
*wStudyTimeTxt.topWidget: "label11"

*wStudyLocationTxt.class: text
*wStudyLocationTxt.static: true
*wStudyLocationTxt.name: wStudyLocationTxt
*wStudyLocationTxt.parent: databaseForm
*wStudyLocationTxt.width: 180
*wStudyLocationTxt.isCompound: "true"
*wStudyLocationTxt.compoundIcon: "text.xpm"
*wStudyLocationTxt.compoundName: "text_"
*wStudyLocationTxt.x: 560
*wStudyLocationTxt.y: 320
*wStudyLocationTxt.height: 30
*wStudyLocationTxt.editable: "true"
*wStudyLocationTxt.leftAttachment: "attach_widget"
*wStudyLocationTxt.leftOffset: 10
*wStudyLocationTxt.leftWidget: "label8"
*wStudyLocationTxt.topAttachment: "attach_opposite_widget"
*wStudyLocationTxt.topOffset: 0
*wStudyLocationTxt.topWidget: "label12"

*wProcedureDescriptionTxt.class: text
*wProcedureDescriptionTxt.static: true
*wProcedureDescriptionTxt.name: wProcedureDescriptionTxt
*wProcedureDescriptionTxt.parent: databaseForm
*wProcedureDescriptionTxt.width: 180
*wProcedureDescriptionTxt.isCompound: "true"
*wProcedureDescriptionTxt.compoundIcon: "text.xpm"
*wProcedureDescriptionTxt.compoundName: "text_"
*wProcedureDescriptionTxt.x: 560
*wProcedureDescriptionTxt.y: 350
*wProcedureDescriptionTxt.height: 30
*wProcedureDescriptionTxt.editable: "true"
*wProcedureDescriptionTxt.leftAttachment: "attach_widget"
*wProcedureDescriptionTxt.leftOffset: 10
*wProcedureDescriptionTxt.leftWidget: "label8"
*wProcedureDescriptionTxt.topAttachment: "attach_opposite_widget"
*wProcedureDescriptionTxt.topOffset: 0
*wProcedureDescriptionTxt.topWidget: "label13"

*wRequestedProcedureTxt.class: text
*wRequestedProcedureTxt.static: true
*wRequestedProcedureTxt.name: wRequestedProcedureTxt
*wRequestedProcedureTxt.parent: databaseForm
*wRequestedProcedureTxt.width: 180
*wRequestedProcedureTxt.isCompound: "true"
*wRequestedProcedureTxt.compoundIcon: "text.xpm"
*wRequestedProcedureTxt.compoundName: "text_"
*wRequestedProcedureTxt.x: 560
*wRequestedProcedureTxt.y: 380
*wRequestedProcedureTxt.height: 30
*wRequestedProcedureTxt.editable: "true"
*wRequestedProcedureTxt.leftAttachment: "attach_widget"
*wRequestedProcedureTxt.leftOffset: 10
*wRequestedProcedureTxt.leftWidget: "label8"
*wRequestedProcedureTxt.topAttachment: "attach_opposite_widget"
*wRequestedProcedureTxt.topOffset: 0
*wRequestedProcedureTxt.topWidget: "label14"

*wAccessionNumberTxt.class: text
*wAccessionNumberTxt.static: true
*wAccessionNumberTxt.name: wAccessionNumberTxt
*wAccessionNumberTxt.parent: databaseForm
*wAccessionNumberTxt.width: 180
*wAccessionNumberTxt.isCompound: "true"
*wAccessionNumberTxt.compoundIcon: "text.xpm"
*wAccessionNumberTxt.compoundName: "text_"
*wAccessionNumberTxt.x: 560
*wAccessionNumberTxt.y: 230
*wAccessionNumberTxt.height: 30
*wAccessionNumberTxt.editable: "false"
*wAccessionNumberTxt.leftAttachment: "attach_widget"
*wAccessionNumberTxt.leftOffset: 10
*wAccessionNumberTxt.leftWidget: "label8"
*wAccessionNumberTxt.topAttachment: "attach_opposite_widget"
*wAccessionNumberTxt.topOffset: 0
*wAccessionNumberTxt.topWidget: "label16"

*wStudyStatusMenu.class: rowColumn
*wStudyStatusMenu.static: true
*wStudyStatusMenu.name: wStudyStatusMenu
*wStudyStatusMenu.parent: databaseForm
*wStudyStatusMenu.rowColumnType: "menu_option"
*wStudyStatusMenu.subMenuId: "optionMenu_p1"
*wStudyStatusMenu.isCompound: "true"
*wStudyStatusMenu.compoundIcon: "optionM.xpm"
*wStudyStatusMenu.compoundName: "option_Menu"
*wStudyStatusMenu.compoundEditor: {\
extern swidget UxGUIMePopup UXPROTO((swidget, swidget, int, int));\
UxGUIMePopup(UxThisWidget, NULL, 2, 0);\
}
*wStudyStatusMenu.x: 630
*wStudyStatusMenu.y: 530
*wStudyStatusMenu.width: 170
*wStudyStatusMenu.height: 30
*wStudyStatusMenu.orientation: "horizontal"

*optionMenu_p1.class: rowColumn
*optionMenu_p1.static: true
*optionMenu_p1.name: optionMenu_p1
*optionMenu_p1.parent: wStudyStatusMenu
*optionMenu_p1.rowColumnType: "menu_pulldown"

*wStudyStatusCreatedButton.class: pushButton
*wStudyStatusCreatedButton.static: true
*wStudyStatusCreatedButton.name: wStudyStatusCreatedButton
*wStudyStatusCreatedButton.parent: optionMenu_p1
*wStudyStatusCreatedButton.labelString: "CREATED"
*wStudyStatusCreatedButton.activateCallback: {\
    strcpy(studyStatusID, "CREATED");\
}

*wStudyStatusScheduledButton.class: pushButton
*wStudyStatusScheduledButton.static: true
*wStudyStatusScheduledButton.name: wStudyStatusScheduledButton
*wStudyStatusScheduledButton.parent: optionMenu_p1
*wStudyStatusScheduledButton.labelString: "SCHEDULED"
*wStudyStatusScheduledButton.activateCallback: {\
    strcpy(studyStatusID, "SCHEDULED");\
}

*wStudyStatusArrivedButton.class: pushButton
*wStudyStatusArrivedButton.static: true
*wStudyStatusArrivedButton.name: wStudyStatusArrivedButton
*wStudyStatusArrivedButton.parent: optionMenu_p1
*wStudyStatusArrivedButton.labelString: "ARRIVED"
*wStudyStatusArrivedButton.activateCallback: {\
     strcpy(studyStatusID, "ARRIVED");\
}

*wStudyStatusStartedButton.class: pushButton
*wStudyStatusStartedButton.static: true
*wStudyStatusStartedButton.name: wStudyStatusStartedButton
*wStudyStatusStartedButton.parent: optionMenu_p1
*wStudyStatusStartedButton.labelString: "STARTED"
*wStudyStatusStartedButton.activateCallback: {\
    strcpy(studyStatusID, "STARTED");\
}

*wStudyStatusCompletedButton.class: pushButton
*wStudyStatusCompletedButton.static: true
*wStudyStatusCompletedButton.name: wStudyStatusCompletedButton
*wStudyStatusCompletedButton.parent: optionMenu_p1
*wStudyStatusCompletedButton.labelString: "COMPLETED"
*wStudyStatusCompletedButton.activateCallback: {\
    strcpy(studyStatusID, "COMPLETED");\
}

*wStudyStatusVerifiedButton.class: pushButton
*wStudyStatusVerifiedButton.static: true
*wStudyStatusVerifiedButton.name: wStudyStatusVerifiedButton
*wStudyStatusVerifiedButton.parent: optionMenu_p1
*wStudyStatusVerifiedButton.labelString: "VERIFIED"
*wStudyStatusVerifiedButton.activateCallback: {\
    strcpy(studyStatusID, "VERIFIED");\
}

*wStudyStatusReadButton.class: pushButton
*wStudyStatusReadButton.static: true
*wStudyStatusReadButton.name: wStudyStatusReadButton
*wStudyStatusReadButton.parent: optionMenu_p1
*wStudyStatusReadButton.labelString: "READ"
*wStudyStatusReadButton.activateCallback: {\
    strcpy(studyStatusID, "READ");\
}

*frame3.class: frame
*frame3.static: true
*frame3.name: frame3
*frame3.parent: databaseForm
*frame3.width: 900
*frame3.height: 80
*frame3.isCompound: "true"
*frame3.compoundIcon: "frame.xpm"
*frame3.compoundName: "frame_"
*frame3.x: 10
*frame3.y: 670
*frame3.resizable: "false"
*frame3.resizeRecursion: "up"

*scrolledWindowText1.class: scrolledWindow
*scrolledWindowText1.static: true
*scrolledWindowText1.name: scrolledWindowText1
*scrolledWindowText1.parent: frame3
*scrolledWindowText1.scrollingPolicy: "application_defined"
*scrolledWindowText1.visualPolicy: "variable"
*scrolledWindowText1.scrollBarDisplayPolicy: "static"
*scrolledWindowText1.isCompound: "true"
*scrolledWindowText1.compoundIcon: "scrltext.xpm"
*scrolledWindowText1.compoundName: "scrolled_Text"
*scrolledWindowText1.x: 130
*scrolledWindowText1.y: 10

*wDatabaseMessageTxt.class: scrolledText
*wDatabaseMessageTxt.static: true
*wDatabaseMessageTxt.name: wDatabaseMessageTxt
*wDatabaseMessageTxt.parent: scrolledWindowText1
*wDatabaseMessageTxt.width: 320
*wDatabaseMessageTxt.height: 50
*wDatabaseMessageTxt.scrollHorizontal: "false"
*wDatabaseMessageTxt.scrollLeftSide: "false"

