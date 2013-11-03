! UIMX ascii 2.5 key: 1763                                                      

*demonstrationForm.class: form
*demonstrationForm.gbldecl: /*\
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
** Module Name(s):	create_demonstrationForm\
**			loadPatientStudyList\
** Author, Date:	Steve Moore, Summer 1994\
** Intent:		This file contains the user interface for the\
**			demonstration of the work list manager.  This\
**			includes the code for creating the widget\
**			hierarchy and the callback functions which\
**			actually do the work.  The public entry points\
**			are the functions to create the widget\
**			hierarchy and to populate the list of patients\
**			and studies with entries from the database.\
** Last Update:		$Author: smm $, $Date: 1995-12-22 17:27:39 $\
** Source File:		$RCSfile: demonstrationForm.i,v $\
** Revision:		$Revision: 1.11 $\
** Status:		$State: Exp $\
*/\
 \
static char rcsid[] = "$Revision: 1.11 $ $RCSfile: demonstrationForm.i,v $";\
\
\
#include <stdio.h>\
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
extern DMAN_HANDLE *dmanHandle;\
extern char applicationTitle[];\
static LST_HEAD *patientList = NULL;\
static LST_HEAD *patientStudyList = NULL;\
static LST_HEAD *studyComponentList = NULL;\
static LST_HEAD *studyComponentSeriesList = NULL;\
static LST_HEAD *workList = NULL;
*demonstrationForm.ispecdecl:
*demonstrationForm.funcdecl: swidget create_demonstrationForm(swidget UxParent)
*demonstrationForm.funcname: create_demonstrationForm
*demonstrationForm.funcdef: "swidget", "<create_demonstrationForm>(%)"
*demonstrationForm.argdecl: swidget UxParent;
*demonstrationForm.arglist: UxParent
*demonstrationForm.arglist.UxParent: "swidget", "%UxParent%"
*demonstrationForm.icode:
*demonstrationForm.fcode: return(rtrn);\

*demonstrationForm.auxdecl: void loadPatientStudyList()\
{\
  CONDITION\
    cond;\
  char\
    b[256];\
  WLMA_PATIENTSTUDY\
    *p;\
\
    if (patientStudyList == NULL)\
	patientStudyList = LST_Create();\
    if (patientList == NULL)\
	patientList = LST_Create();\
\
    cond = FIS_Get(&fisHandle, FIS_K_PATIENT, FIS_K_PATIENT, NULL,\
	FIS_K_PAT_STUDYLIST, patientList);\
    if (cond != FIS_NORMAL) {\
	COND_DumpConditions();\
	return;\
    }\
    expandPatientStudy(patientList, patientStudyList);\
    MUT_LoadList(wPatientStudyList, patientStudyList, formatPatientStudy, b);\
    p = LST_Head(&patientStudyList);\
    if (p != NULL)\
	LST_Position(&patientStudyList, p);\
    XmTextSetString(wPatientStudyTxt, "");\
}\
\
void loadStudyComponentList()\
{\
  CONDITION cond;\
  char b[256];\
  WLMA_PATIENTSTUDY *p;\
\
    if (studyComponentList == NULL)\
	studyComponentList = LST_Create();\
    if (studyComponentSeriesList == NULL)\
	studyComponentSeriesList = LST_Create();\
\
    p = LST_Current(&patientStudyList);\
    if (p == NULL)\
	FIS_ClearList(studyComponentList);\
    else {\
	cond = FIS_Get(&fisHandle, FIS_K_STUDYCOMPONENT,\
		FIS_K_STUDY, p->Study.StuInsUID,\
		FIS_K_STUDYCOMP_SERIESLIST,\
		studyComponentList);\
	if (cond != FIS_NORMAL) {\
	    COND_DumpConditions();\
	    return;\
	}\
    }\
    expandStudyComponentSCSeries(studyComponentList,\
	studyComponentSeriesList);\
\
    MUT_LoadList(wStudyComponentList, studyComponentSeriesList,\
	formatStudyComponentSCSeries, b);\
}\
\
void loadWorkList()\
{\
  CONDITION cond;\
  char b[256];\
  WLMA_PATIENTSTUDY *p, *q, *save;\
\
    if (workList == NULL)\
	workList = LST_Create();\
    if (workList == NULL)\
	return;\
\
    save = LST_Current(&patientStudyList);\
\
    while ((q = LST_Dequeue(&workList)) != NULL)\
	free(q);\
\
    p = LST_Head(&patientStudyList);\
    (void) LST_Position(&patientStudyList, p);\
    while (p != NULL) {\
	if (strcmp(p->Study.StuStaID, "SCHEDULED") == 0) {\
	    q = (WLMA_PATIENTSTUDY *)malloc(sizeof(*q));\
	    if (q == NULL)\
		return;\
\
	    *q = *p;\
	    (void) LST_Enqueue(&workList, q);\
	}\
	p = LST_Next(&patientStudyList);\
   }\
    (void) LST_Position(&patientStudyList, save);\
\
    (void) LST_Sort(&workList, sizeof(*p), comparePatientStudyScheduled);\
   MUT_LoadList(wWorkListList, workList,\
	formatPatientStudyScheduled, b);\
}
*demonstrationForm.static: true
*demonstrationForm.name: demonstrationForm
*demonstrationForm.parent: NO_PARENT
*demonstrationForm.parentExpression: UxParent
*demonstrationForm.defaultShell: topLevelShell
*demonstrationForm.width: 952
*demonstrationForm.height: 771
*demonstrationForm.resizePolicy: "resize_none"
*demonstrationForm.isCompound: "true"
*demonstrationForm.compoundIcon: "form.xpm"
*demonstrationForm.compoundName: "form_"
*demonstrationForm.x: 66
*demonstrationForm.y: 29
*demonstrationForm.unitType: "pixels"

*wPatientStudyTxt.class: textField
*wPatientStudyTxt.static: true
*wPatientStudyTxt.name: wPatientStudyTxt
*wPatientStudyTxt.parent: demonstrationForm
*wPatientStudyTxt.width: 890
*wPatientStudyTxt.isCompound: "true"
*wPatientStudyTxt.compoundIcon: "textfield.xpm"
*wPatientStudyTxt.compoundName: "text_Field"
*wPatientStudyTxt.x: 40
*wPatientStudyTxt.y: 220
*wPatientStudyTxt.height: 30

*label18.class: label
*label18.static: true
*label18.name: label18
*label18.parent: demonstrationForm
*label18.isCompound: "true"
*label18.compoundIcon: "label.xpm"
*label18.compoundName: "label_"
*label18.x: 60
*label18.y: 410
*label18.width: 210
*label18.height: 30
*label18.labelString: "Remote Application (Title)"

*wRemoteApplicationTxt.class: textField
*wRemoteApplicationTxt.static: true
*wRemoteApplicationTxt.name: wRemoteApplicationTxt
*wRemoteApplicationTxt.parent: demonstrationForm
*wRemoteApplicationTxt.width: 210
*wRemoteApplicationTxt.isCompound: "true"
*wRemoteApplicationTxt.compoundIcon: "textfield.xpm"
*wRemoteApplicationTxt.compoundName: "text_Field"
*wRemoteApplicationTxt.x: 300
*wRemoteApplicationTxt.y: 410
*wRemoteApplicationTxt.height: 40
*wRemoteApplicationTxt.topAttachment: "attach_opposite_widget"
*wRemoteApplicationTxt.topOffset: 0
*wRemoteApplicationTxt.topWidget: "label18"

*pushButton7.class: pushButton
*pushButton7.static: true
*pushButton7.name: pushButton7
*pushButton7.parent: demonstrationForm
*pushButton7.isCompound: "true"
*pushButton7.compoundIcon: "push.xpm"
*pushButton7.compoundName: "push_Button"
*pushButton7.x: 50
*pushButton7.y: 480
*pushButton7.width: 240
*pushButton7.height: 50
*pushButton7.labelString: "Study Scheduled"
*pushButton7.activateCallback: {\
  CONDITION cond;\
  char  destinationApplication[20];\
  CTNBOOLEAN  nullFlag;\
  WLMA_PATIENTSTUDY  *ps;\
  FIS_STUDYRECORD study;\
  FIS_STUDYCOMPONENTRECORD *studyComponent;\
\
    ps = LST_Current(&patientStudyList);\
    if (ps == NULL)\
	return;\
\
    cond = MUT_ScanWidget(wRemoteApplicationTxt, MUT_TEXT, &nullFlag,\
	destinationApplication);\
    if (cond != MUT_NORMAL)\
	return;\
\
    strcpy(ps->Study.StuStaID, "SCHEDULED");\
    UTL_GetDicomDate(ps->Study.SchStuStaDat);\
    UTL_GetDicomTime(ps->Study.SchStuStaTim);\
    ps->Study.Flag |= FIS_K_STU_STUSTAID |\
		FIS_K_STU_SCHSTUSTADAT |\
		FIS_K_STU_SCHSTUSTATIM;\
\
    study = ps->Study;\
    study.Flag = FIS_K_STU_STUINSUID | FIS_K_STU_STUSTAID |\
		FIS_K_STU_SCHSTUSTADAT |\
		FIS_K_STU_SCHSTUSTATIM;\
    cond = FIS_Update(&fisHandle, FIS_K_STUDY, &study);\
    if (cond != FIS_NORMAL)\
	COND_DumpConditions();\
    studyComponent = LST_Head(&studyComponentList);\
    (void) LST_Position(&studyComponentList, studyComponent);\
    while(studyComponent != NULL) {\
	cond = FIS_Delete(&fisHandle, FIS_K_STUDYCOMPONENT,\
	    FIS_K_STUDYCOMPONENT, studyComponent->StuComUID);\
	if (cond != FIS_NORMAL)\
	    COND_DumpConditions();\
	cond = FIS_Delete(&fisHandle, FIS_K_SCSERIES,\
	    FIS_K_STUDYCOMPONENT, studyComponent->StuComUID);\
	if (cond != FIS_NORMAL)\
	    COND_DumpConditions();\
	cond = FIS_Delete(&fisHandle, FIS_K_SCIMAGE,\
	    FIS_K_STUDYCOMPONENT, studyComponent->StuComUID);\
	if (cond != FIS_NORMAL)\
	    COND_DumpConditions();\
\
	studyComponent = LST_Next(&studyComponentList);\
    }\
\
    if (!nullFlag) {\
	cond = FIS_SendEvent(&fisHandle, &dmanHandle, FIS_K_STUDY,\
		&ps->Study, FIS_K_STUDY_SCHEDULED, applicationTitle,\
		destinationApplication);\
	if (cond != FIS_NORMAL)\
	    COND_DumpConditions();\
    }\
\
    loadPatientStudyList();\
    loadStudyComponentList();\
    loadWorkList();\
}

*pushButton9.class: pushButton
*pushButton9.static: true
*pushButton9.name: pushButton9
*pushButton9.parent: demonstrationForm
*pushButton9.isCompound: "true"
*pushButton9.compoundIcon: "push.xpm"
*pushButton9.compoundName: "push_Button"
*pushButton9.x: 350
*pushButton9.y: 480
*pushButton9.width: 240
*pushButton9.height: 50
*pushButton9.labelString: "Study Read"
*pushButton9.activateCallback: {\
  CONDITION cond;\
  char  destinationApplication[20];\
  CTNBOOLEAN  nullFlag;\
  WLMA_PATIENTSTUDY  *ps;\
  FIS_STUDYRECORD study;\
\
    ps = LST_Current(&patientStudyList);\
    if (ps == NULL)\
	return;\
\
    cond = MUT_ScanWidget(wRemoteApplicationTxt, MUT_TEXT, &nullFlag,\
	destinationApplication);\
    if (cond != MUT_NORMAL)\
	return;\
\
    strcpy(ps->Study.StuStaID, "READ");\
    UTL_GetDicomDate(ps->Study.StuReaDat);\
    UTL_GetDicomTime(ps->Study.StuReaTim);\
    ps->Study.Flag |= FIS_K_STU_STUSTAID | FIS_K_STU_STUREADAT |\
		FIS_K_STU_STUREATIM;\
\
    study = ps->Study;\
    study.Flag = FIS_K_STU_STUINSUID | FIS_K_STU_STUSTAID;\
    cond = FIS_Update(&fisHandle, FIS_K_STUDY, &study);\
    if (cond != FIS_NORMAL)\
	COND_DumpConditions();\
\
    if (!nullFlag) {\
	cond = FIS_SendEvent(&fisHandle, &dmanHandle, FIS_K_STUDY,\
		&ps->Study, FIS_K_STUDY_READ, applicationTitle,\
		destinationApplication);\
	if (cond != FIS_NORMAL)\
	    COND_DumpConditions();\
    }\
\
    loadPatientStudyList();\
    loadWorkList();\
}

*frame4.class: frame
*frame4.static: true
*frame4.name: frame4
*frame4.parent: demonstrationForm
*frame4.width: 890
*frame4.height: 120
*frame4.isCompound: "true"
*frame4.compoundIcon: "frame.xpm"
*frame4.compoundName: "frame_"
*frame4.x: 40
*frame4.y: 260
*frame4.resizable: "false"
*frame4.resizeRecursion: "up"

*scrolledWindowList4.class: scrolledWindow
*scrolledWindowList4.static: true
*scrolledWindowList4.name: scrolledWindowList4
*scrolledWindowList4.parent: frame4
*scrolledWindowList4.scrollingPolicy: "application_defined"
*scrolledWindowList4.visualPolicy: "variable"
*scrolledWindowList4.scrollBarDisplayPolicy: "static"
*scrolledWindowList4.shadowThickness: 0
*scrolledWindowList4.isCompound: "true"
*scrolledWindowList4.compoundIcon: "scrllist.xpm"
*scrolledWindowList4.compoundName: "scrolled_List"
*scrolledWindowList4.x: 140
*scrolledWindowList4.y: 40

*wStudyComponentList.class: scrolledList
*wStudyComponentList.static: true
*wStudyComponentList.name: wStudyComponentList
*wStudyComponentList.parent: scrolledWindowList4
*wStudyComponentList.width: 290
*wStudyComponentList.height: 40

*pushButton10.class: pushButton
*pushButton10.static: true
*pushButton10.name: pushButton10
*pushButton10.parent: demonstrationForm
*pushButton10.isCompound: "true"
*pushButton10.compoundIcon: "push.xpm"
*pushButton10.compoundName: "push_Button"
*pushButton10.x: 670
*pushButton10.y: 480
*pushButton10.width: 240
*pushButton10.height: 50
*pushButton10.labelString: "Refresh"
*pushButton10.activateCallback: {\
    loadPatientStudyList();\
    loadStudyComponentList();\
    loadWorkList();\
}
*pushButton10.sensitive: "true"

*frame5.class: frame
*frame5.static: true
*frame5.name: frame5
*frame5.parent: demonstrationForm
*frame5.width: 890
*frame5.height: 150
*frame5.isCompound: "true"
*frame5.compoundIcon: "frame.xpm"
*frame5.compoundName: "frame_"
*frame5.x: 40
*frame5.y: 40
*frame5.resizable: "false"
*frame5.resizeRecursion: "up"

*scrolledWindowList3.class: scrolledWindow
*scrolledWindowList3.static: true
*scrolledWindowList3.name: scrolledWindowList3
*scrolledWindowList3.parent: frame5
*scrolledWindowList3.scrollingPolicy: "application_defined"
*scrolledWindowList3.visualPolicy: "variable"
*scrolledWindowList3.scrollBarDisplayPolicy: "static"
*scrolledWindowList3.shadowThickness: 0
*scrolledWindowList3.isCompound: "true"
*scrolledWindowList3.compoundIcon: "scrllist.xpm"
*scrolledWindowList3.compoundName: "scrolled_List"
*scrolledWindowList3.x: 0
*scrolledWindowList3.y: 0

*wPatientStudyList.class: scrolledList
*wPatientStudyList.static: true
*wPatientStudyList.name: wPatientStudyList
*wPatientStudyList.parent: scrolledWindowList3
*wPatientStudyList.width: 890
*wPatientStudyList.height: 160
*wPatientStudyList.browseSelectionCallback: {\
  XmListCallbackStruct *listStruct;\
  WLMA_PATIENTSTUDY *p;\
  int i;\
  char b[256];\
\
\
    listStruct = (XmListCallbackStruct *)UxCallbackArg;\
    p = LST_Index(&patientStudyList, listStruct->item_position);\
\
    formatPatientStudy(p, 0, b);\
    XmTextSetString(wPatientStudyTxt, b);\
    loadStudyComponentList();\
}

*frame6.class: frame
*frame6.static: true
*frame6.name: frame6
*frame6.parent: demonstrationForm
*frame6.width: 890
*frame6.height: 120
*frame6.isCompound: "true"
*frame6.compoundIcon: "frame.xpm"
*frame6.compoundName: "frame_"
*frame6.x: 36
*frame6.y: 576
*frame6.resizeRecursion: "none"
*frame6.resizable: "false"

*scrolledWindowList5.class: scrolledWindow
*scrolledWindowList5.static: true
*scrolledWindowList5.name: scrolledWindowList5
*scrolledWindowList5.parent: frame6
*scrolledWindowList5.scrollingPolicy: "application_defined"
*scrolledWindowList5.visualPolicy: "variable"
*scrolledWindowList5.scrollBarDisplayPolicy: "static"
*scrolledWindowList5.shadowThickness: 0
*scrolledWindowList5.isCompound: "true"
*scrolledWindowList5.compoundIcon: "scrllist.xpm"
*scrolledWindowList5.compoundName: "scrolled_List"
*scrolledWindowList5.x: 260
*scrolledWindowList5.y: 20

*wWorkListList.class: scrolledList
*wWorkListList.static: true
*wWorkListList.name: wWorkListList
*wWorkListList.parent: scrolledWindowList5
*wWorkListList.width: 230
*wWorkListList.height: 10

