! UIMX ascii 2.5 key: 6510                                                      

*applicationShell1.class: applicationShell
*applicationShell1.gbldecl: /*\
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
** Module Name(s):	create_applicationShell1\
**			loadPatientStudyList\
** Author, Date:	Steve Moore, Summer 1994\
** Intent:		This file contains the code that defines the\
**			user interface for the rsa_ui application.  This\
**			includes the code that creates the UI (by creating\
**			widgets), the code that loads data into the\
**			interface and the callback functions for user\
**			events (like list selection and button presses).\
**			The public interfaces create the widget\
**			hierarchy and loads the scrolled list of\
**			studies (for user selection from the event loop).\
** Last Update:         $Author: smm $, $Date: 1995-12-21 20:45:50 $\
** Source File:         $RCSfile: applicationShell1.i,v $\
** Revision:            $Revision: 1.7 $\
** Status:              $State: Exp $\
*/\
 \
static char rcsid[] = "$Revision: 1.7 $ $RCSfile: applicationShell1.i,v $";\
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
#include "rsa.h"\
#include "format.h"\
\
extern FIS_HANDLE *fisHandle;\
extern DMAN_HANDLE *dmanHandle;\
extern char applicationTitle[];\
extern char *eventDestinations;\
static LST_HEAD *patientList = NULL;\
static LST_HEAD *patientStudyList = NULL;\
static FIS_RESULTSRECORD results;\
static FIS_INTERPRETATIONRECORD interpretation;
*applicationShell1.ispecdecl:
*applicationShell1.funcdecl: swidget create_applicationShell1(swidget UxParent)
*applicationShell1.funcname: create_applicationShell1
*applicationShell1.funcdef: "swidget", "<create_applicationShell1>(%)"
*applicationShell1.argdecl: swidget UxParent;
*applicationShell1.arglist: UxParent
*applicationShell1.arglist.UxParent: "swidget", "%UxParent%"
*applicationShell1.icode:
*applicationShell1.fcode: return(rtrn);\

*applicationShell1.auxdecl: void loadPatientStudyList()\
{\
  CONDITION cond;\
  char b[256];\
    RSA_PATIENTSTUDY *p;\
\
    if (patientStudyList == NULL)\
	patientStudyList = LST_Create();\
    if (patientList == NULL)\
	patientList = LST_Create();\
\
    cond = FIS_Get(&fisHandle, FIS_K_PATIENT, FIS_K_PATIENT,\
	NULL, FIS_K_PAT_STUDYLIST, patientList);\
    if (cond != FIS_NORMAL) {\
	COND_DumpConditions();\
	return;\
    }\
    expandPatientStudy(patientList, patientStudyList);\
    MUT_LoadList(wPatientStudyList, patientStudyList, formatPatientStudy, b);\
    p = LST_Head(&patientStudyList);\
    if (p != NULL)\
	(void) LST_Position(&patientStudyList, p);\
}\
\
static void displayImpressionsInterpretation()\
{\
  CONDITION cond;\
    RSA_PATIENTSTUDY *p;\
\
    XmTextSetString(text1, "");\
    p = LST_Current(&patientStudyList);\
    if (p == NULL)\
	return;\
\
    memset(&results, 0, sizeof(results));\
    results.Type = FIS_K_RESULTS;\
    memset(&interpretation, 0, sizeof(interpretation));\
    interpretation.Type = FIS_K_INTERPRETATION;\
\
    cond = FIS_GetOne(&fisHandle, FIS_K_RESULTS, FIS_K_STUDY,\
	p->Study.StuInsUID, 0, &results);\
    if (cond != FIS_NORMAL) {\
	XmTextSetString(text1, "Failed to retrieve results record");\
	COND_DumpConditions();\
	return;\
    }\
    if (results.Flag & FIS_K_RESULTS_IMP)\
	XmTextSetString(wImpressionsTxt, results.Imp);\
    else\
	XmTextSetString(wImpressionsTxt, "<None>");\
\
    cond = FIS_GetOne(&fisHandle, FIS_K_INTERPRETATION, FIS_K_RESULTS,\
	results.ResUID, 0, &interpretation);\
    if (cond != FIS_NORMAL) {\
	XmTextSetString(text1, "Failed to retrieve interpretation record");\
	COND_DumpConditions();\
	return;\
    }\
    if (interpretation.Flag & FIS_K_INTERP_INTTEX)\
	XmTextSetString(wReportTxt, interpretation.IntTex);\
    else\
	XmTextSetString(wReportTxt, "<None>");\
}\
\
static void updateImpressions()\
{\
  CONDITION cond;\
  FIS_RESULTSRECORD localResults;\
\
    XmTextSetString(text1, "");\
    memcpy(&localResults, &results, sizeof(results));\
    localResults.Flag = FIS_K_RESULTS_IMP;\
    cond = FIS_Update(&fisHandle, FIS_K_RESULTS, &localResults);\
    if (cond != FIS_NORMAL) {\
	(void)MUT_DisplayConditionText(text1);\
	(void) COND_PopCondition(TRUE);\
    }\
}\
\
static void updateInterpretation()\
{\
  CONDITION cond;\
  FIS_INTERPRETATIONRECORD localInterpretation;\
\
    XmTextSetString(text1, "");\
    memcpy(&localInterpretation, &interpretation, sizeof(interpretation));\
    localInterpretation.Flag = FIS_K_INTERP_INTTEX;\
    cond = FIS_Update(&fisHandle, FIS_K_INTERPRETATION, &localInterpretation);\
    if (cond != FIS_NORMAL) {\
	(void) MUT_DisplayConditionText(text1);\
	(void) COND_PopCondition(TRUE);\
    }\
}
*applicationShell1.static: true
*applicationShell1.name: applicationShell1
*applicationShell1.parent: NO_PARENT
*applicationShell1.parentExpression: UxParent
*applicationShell1.width: 900
*applicationShell1.height: 780
*applicationShell1.isCompound: "true"
*applicationShell1.compoundIcon: "applS.xpm"
*applicationShell1.compoundName: "appl_Shell"
*applicationShell1.x: 67
*applicationShell1.y: 92

*mainWindow1.class: mainWindow
*mainWindow1.static: true
*mainWindow1.name: mainWindow1
*mainWindow1.parent: applicationShell1
*mainWindow1.width: 900
*mainWindow1.height: 780
*mainWindow1.isCompound: "true"
*mainWindow1.compoundIcon: "main.xpm"
*mainWindow1.compoundName: "main_Window"
*mainWindow1.compoundEditor: {\
extern swidget UxGUIMePopup UXPROTO((swidget, swidget, int, int));\
UxGUIMwePopup(UxThisWidget, UxGetParent(UxThisWidget));\
}
*mainWindow1.x: 0
*mainWindow1.y: 0
*mainWindow1.unitType: "pixels"

*menu1.class: rowColumn
*menu1.static: true
*menu1.name: menu1
*menu1.parent: mainWindow1
*menu1.rowColumnType: "menu_bar"

*menu1_p1.class: rowColumn
*menu1_p1.static: true
*menu1_p1.name: menu1_p1
*menu1_p1.parent: menu1
*menu1_p1.rowColumnType: "menu_pulldown"

*menu1_p1_b1.class: pushButton
*menu1_p1_b1.static: true
*menu1_p1_b1.name: menu1_p1_b1
*menu1_p1_b1.parent: menu1_p1
*menu1_p1_b1.labelString: "Quit"
*menu1_p1_b1.activateCallback: {\
    exit(0);\
}

*menu1_top_b1.class: cascadeButton
*menu1_top_b1.static: true
*menu1_top_b1.name: menu1_top_b1
*menu1_top_b1.parent: menu1
*menu1_top_b1.labelString: "Control"
*menu1_top_b1.subMenuId: "menu1_p1"

*form1.class: form
*form1.static: true
*form1.name: form1
*form1.parent: mainWindow1

*wClearReportButton.class: pushButton
*wClearReportButton.static: true
*wClearReportButton.name: wClearReportButton
*wClearReportButton.parent: form1
*wClearReportButton.isCompound: "true"
*wClearReportButton.compoundIcon: "push.xpm"
*wClearReportButton.compoundName: "push_Button"
*wClearReportButton.x: 770
*wClearReportButton.y: 370
*wClearReportButton.width: 130
*wClearReportButton.height: 40
*wClearReportButton.labelString: "Clear Report"
*wClearReportButton.sensitive: "false"
*wClearReportButton.activateCallback: {\
    XmTextSetString(wReportTxt, "");\
    interpretation.IntTex[0] = '\0';\
    updateInterpretation();\
}

*wUpdateReportButton.class: pushButton
*wUpdateReportButton.static: true
*wUpdateReportButton.name: wUpdateReportButton
*wUpdateReportButton.parent: form1
*wUpdateReportButton.isCompound: "true"
*wUpdateReportButton.compoundIcon: "push.xpm"
*wUpdateReportButton.compoundName: "push_Button"
*wUpdateReportButton.x: 770
*wUpdateReportButton.y: 460
*wUpdateReportButton.width: 130
*wUpdateReportButton.height: 40
*wUpdateReportButton.labelString: "Update Report"
*wUpdateReportButton.sensitive: "false"
*wUpdateReportButton.activateCallback: {\
  CONDITION cond;\
  CTNBOOLEAN nullFlag;\
\
    cond = MUT_ScanWidget(wReportTxt, MUT_TEXT, &nullFlag,\
	interpretation.IntTex);\
    if (cond != MUT_NORMAL) {\
	MUT_DisplayConditionText(text1);\
	(void) COND_PopCondition(TRUE);\
	return;\
    }\
    if (nullFlag)\
	interpretation.IntTex[0] = '\0';\
    updateInterpretation();\
}

*pushButton3.class: pushButton
*pushButton3.static: true
*pushButton3.name: pushButton3
*pushButton3.parent: form1
*pushButton3.isCompound: "true"
*pushButton3.compoundIcon: "push.xpm"
*pushButton3.compoundName: "push_Button"
*pushButton3.x: 80
*pushButton3.y: 590
*pushButton3.width: 120
*pushButton3.height: 40
*pushButton3.labelString: "Study Scheduled"
*pushButton3.activateCallback: {\
  CONDITION cond;\
  RSA_PATIENTSTUDY *p;\
\
    XmTextSetString(text1, "");\
    p = LST_Current(&patientStudyList);\
    if (p == NULL) {\
	XmTextSetString(text1, "No current study selected");\
	return;\
    }\
\
    cond = distributeEvent(&fisHandle, &dmanHandle, FIS_K_STUDY,\
	&p->Study, FIS_K_STUDY_SCHEDULED, applicationTitle,\
	eventDestinations);\
    if (cond != 1) {\
	(void) MUT_DisplayConditionText(text1);\
	(void) COND_PopCondition(TRUE);\
    } else {\
	XmTextSetString(text1, "Study Scheduled Event delivered");\
    }\
}

*pushButton4.class: pushButton
*pushButton4.static: true
*pushButton4.name: pushButton4
*pushButton4.parent: form1
*pushButton4.isCompound: "true"
*pushButton4.compoundIcon: "push.xpm"
*pushButton4.compoundName: "push_Button"
*pushButton4.x: 360
*pushButton4.y: 590
*pushButton4.width: 120
*pushButton4.height: 40
*pushButton4.labelString: "Study Completed"
*pushButton4.activateCallback: {\
  CONDITION cond;\
  RSA_PATIENTSTUDY *p;\
\
    XmTextSetString(text1, "");\
    p = LST_Current(&patientStudyList);\
    if (p == NULL) {\
	XmTextSetString(text1, "No current study selected");\
	return;\
    }\
}
*pushButton4.sensitive: "false"

*pushButton5.class: pushButton
*pushButton5.static: true
*pushButton5.name: pushButton5
*pushButton5.parent: form1
*pushButton5.isCompound: "true"
*pushButton5.compoundIcon: "push.xpm"
*pushButton5.compoundName: "push_Button"
*pushButton5.x: 600
*pushButton5.y: 590
*pushButton5.width: 120
*pushButton5.height: 40
*pushButton5.labelString: "Study Read"
*pushButton5.activateCallback: {\
  CONDITION cond;\
  RSA_PATIENTSTUDY *p;\
\
    XmTextSetString(text1, "");\
    p = LST_Current(&patientStudyList);\
    if (p == NULL) {\
	XmTextSetString(text1, "No current study selected");\
	return;\
    }\
    UTL_GetDicomDate(p->Study.StuReaDat);\
    UTL_GetDicomTime(p->Study.StuReaTim);\
    p->Study.Flag |= FIS_K_STU_STUREADAT | FIS_K_STU_STUREATIM;\
\
    cond = distributeEvent(&fisHandle, &dmanHandle, FIS_K_STUDY,\
	&p->Study, FIS_K_STUDY_READ,  applicationTitle,\
	eventDestinations);\
    if (cond != 1) {\
	(void) MUT_DisplayConditionText(text1);\
	(void) COND_PopCondition(TRUE);\
    } else {\
	XmTextSetString(text1, "Study Read Event delivered");\
    }\
\
}

*separator1.class: separator
*separator1.static: true
*separator1.name: separator1
*separator1.parent: form1
*separator1.width: 840
*separator1.height: 10
*separator1.isCompound: "true"
*separator1.compoundIcon: "sep.xpm"
*separator1.compoundName: "separator_"
*separator1.x: 20
*separator1.y: 560

*wPatientStudyTxt.class: textField
*wPatientStudyTxt.static: true
*wPatientStudyTxt.name: wPatientStudyTxt
*wPatientStudyTxt.parent: form1
*wPatientStudyTxt.width: 790
*wPatientStudyTxt.isCompound: "true"
*wPatientStudyTxt.compoundIcon: "textfield.xpm"
*wPatientStudyTxt.compoundName: "text_Field"
*wPatientStudyTxt.x: 10
*wPatientStudyTxt.y: 0
*wPatientStudyTxt.height: 40
*wPatientStudyTxt.editable: "false"
*wPatientStudyTxt.fontList: "courier-bold"

*frame1.class: frame
*frame1.static: true
*frame1.name: frame1
*frame1.parent: form1
*frame1.width: 740
*frame1.height: 200
*frame1.isCompound: "true"
*frame1.compoundIcon: "frame.xpm"
*frame1.compoundName: "frame_"
*frame1.x: 20
*frame1.y: 340

*scrolledWindowText1.class: scrolledWindow
*scrolledWindowText1.static: true
*scrolledWindowText1.name: scrolledWindowText1
*scrolledWindowText1.parent: frame1
*scrolledWindowText1.scrollingPolicy: "application_defined"
*scrolledWindowText1.visualPolicy: "variable"
*scrolledWindowText1.scrollBarDisplayPolicy: "static"
*scrolledWindowText1.isCompound: "true"
*scrolledWindowText1.compoundIcon: "scrltext.xpm"
*scrolledWindowText1.compoundName: "scrolled_Text"
*scrolledWindowText1.x: 200
*scrolledWindowText1.y: 60

*wReportTxt.class: scrolledText
*wReportTxt.static: true
*wReportTxt.name: wReportTxt
*wReportTxt.parent: scrolledWindowText1
*wReportTxt.width: 260
*wReportTxt.height: 50
*wReportTxt.editMode: "multi_line_edit"
*wReportTxt.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"

*frame2.class: frame
*frame2.static: true
*frame2.name: frame2
*frame2.parent: form1
*frame2.width: 790
*frame2.height: 160
*frame2.isCompound: "true"
*frame2.compoundIcon: "frame.xpm"
*frame2.compoundName: "frame_"
*frame2.x: 10
*frame2.y: 50
*frame2.resizable: "false"
*frame2.resizeRecursion: "up"

*scrolledWindowList1.class: scrolledWindow
*scrolledWindowList1.static: true
*scrolledWindowList1.name: scrolledWindowList1
*scrolledWindowList1.parent: frame2
*scrolledWindowList1.scrollingPolicy: "application_defined"
*scrolledWindowList1.visualPolicy: "variable"
*scrolledWindowList1.scrollBarDisplayPolicy: "static"
*scrolledWindowList1.shadowThickness: 0
*scrolledWindowList1.isCompound: "true"
*scrolledWindowList1.compoundIcon: "scrllist.xpm"
*scrolledWindowList1.compoundName: "scrolled_List"
*scrolledWindowList1.x: 330
*scrolledWindowList1.y: 0

*wPatientStudyList.class: scrolledList
*wPatientStudyList.static: true
*wPatientStudyList.name: wPatientStudyList
*wPatientStudyList.parent: scrolledWindowList1
*wPatientStudyList.width: 90
*wPatientStudyList.height: 156
*wPatientStudyList.browseSelectionCallback: {\
  XmListCallbackStruct *listStruct;\
  RSA_PATIENTSTUDY *p;\
  char buf[256];\
\
    listStruct = (XmListCallbackStruct *)UxCallbackArg;\
    p = LST_Index(&patientStudyList, listStruct->item_position);\
    formatPatientStudy(p, 0, buf);\
    XmTextSetString(wPatientStudyTxt, buf);\
\
    displayImpressionsInterpretation(p);\
    XtSetSensitive(wClearImpressionsButton, TRUE);\
    XtSetSensitive(wUpdateImpressionsButton, TRUE);\
    XtSetSensitive(wClearReportButton, TRUE);\
    XtSetSensitive(wUpdateReportButton, TRUE);\
}
*wPatientStudyList.fontList: "courier-bold"

*frame3.class: frame
*frame3.static: true
*frame3.name: frame3
*frame3.parent: form1
*frame3.width: 740
*frame3.height: 90
*frame3.isCompound: "true"
*frame3.compoundIcon: "frame.xpm"
*frame3.compoundName: "frame_"
*frame3.x: 20
*frame3.y: 230

*scrolledWindowText2.class: scrolledWindow
*scrolledWindowText2.static: true
*scrolledWindowText2.name: scrolledWindowText2
*scrolledWindowText2.parent: frame3
*scrolledWindowText2.scrollingPolicy: "application_defined"
*scrolledWindowText2.visualPolicy: "variable"
*scrolledWindowText2.scrollBarDisplayPolicy: "static"
*scrolledWindowText2.isCompound: "true"
*scrolledWindowText2.compoundIcon: "scrltext.xpm"
*scrolledWindowText2.compoundName: "scrolled_Text"
*scrolledWindowText2.x: 200
*scrolledWindowText2.y: -10
*scrolledWindowText2.height: 156

*wImpressionsTxt.class: scrolledText
*wImpressionsTxt.static: true
*wImpressionsTxt.name: wImpressionsTxt
*wImpressionsTxt.parent: scrolledWindowText2
*wImpressionsTxt.width: 430
*wImpressionsTxt.height: 137
*wImpressionsTxt.editMode: "multi_line_edit"
*wImpressionsTxt.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"

*wClearImpressionsButton.class: pushButton
*wClearImpressionsButton.static: true
*wClearImpressionsButton.name: wClearImpressionsButton
*wClearImpressionsButton.parent: form1
*wClearImpressionsButton.isCompound: "true"
*wClearImpressionsButton.compoundIcon: "push.xpm"
*wClearImpressionsButton.compoundName: "push_Button"
*wClearImpressionsButton.x: 770
*wClearImpressionsButton.y: 230
*wClearImpressionsButton.width: 130
*wClearImpressionsButton.height: 40
*wClearImpressionsButton.labelString: "Clear Impressions"
*wClearImpressionsButton.sensitive: "false"
*wClearImpressionsButton.activateCallback: {\
    XmTextSetString(wImpressionsTxt, "");\
    results.Imp[0] = '\0';\
    updateImpressions();\
}

*wUpdateImpressionsButton.class: pushButton
*wUpdateImpressionsButton.static: true
*wUpdateImpressionsButton.name: wUpdateImpressionsButton
*wUpdateImpressionsButton.parent: form1
*wUpdateImpressionsButton.isCompound: "true"
*wUpdateImpressionsButton.compoundIcon: "push.xpm"
*wUpdateImpressionsButton.compoundName: "push_Button"
*wUpdateImpressionsButton.x: 770
*wUpdateImpressionsButton.y: 280
*wUpdateImpressionsButton.width: 130
*wUpdateImpressionsButton.height: 40
*wUpdateImpressionsButton.labelString: "Update Impressions"
*wUpdateImpressionsButton.sensitive: "false"
*wUpdateImpressionsButton.activateCallback: {\
  CONDITION cond;\
  CTNBOOLEAN nullFlag;\
\
    cond = MUT_ScanWidget(wImpressionsTxt, MUT_TEXT, &nullFlag, results.Imp);\
    if (cond != MUT_NORMAL) {\
	MUT_DisplayConditionText(text1);\
	(void) COND_PopCondition(TRUE);\
	return;\
    }\
    if (nullFlag)\
	results.Imp[0] = '\0';\
    updateImpressions();\
}

*frame4.class: frame
*frame4.static: true
*frame4.name: frame4
*frame4.parent: form1
*frame4.width: 870
*frame4.height: 90
*frame4.isCompound: "true"
*frame4.compoundIcon: "frame.xpm"
*frame4.compoundName: "frame_"
*frame4.x: 6
*frame4.y: 646
*frame4.resizable: "false"
*frame4.resizeRecursion: "up"

*scrolledWindowText3.class: scrolledWindow
*scrolledWindowText3.static: true
*scrolledWindowText3.name: scrolledWindowText3
*scrolledWindowText3.parent: frame4
*scrolledWindowText3.scrollingPolicy: "application_defined"
*scrolledWindowText3.visualPolicy: "variable"
*scrolledWindowText3.scrollBarDisplayPolicy: "static"
*scrolledWindowText3.isCompound: "true"
*scrolledWindowText3.compoundIcon: "scrltext.xpm"
*scrolledWindowText3.compoundName: "scrolled_Text"
*scrolledWindowText3.x: 430
*scrolledWindowText3.y: 10

*text1.class: scrolledText
*text1.static: true
*text1.name: text1
*text1.parent: scrolledWindowText3
*text1.width: 140
*text1.height: 60
*text1.editable: "false"
*text1.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"

