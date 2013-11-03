! UIMX ascii 2.5 key: 8736                                                      

*securityForm.class: form
*securityForm.gbldecl: /*\
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
** Module Name(s):	create_securityForm\
**			loadSecurityMatrixList\
** Author, Date:	Steve Moore, Summer 1994\
** Intent:		This file provides the user interface\
**                      for defining and modifying entries in\
**			the security matrix table.\
**			The public entry points are\
**			call to create the widget hierarchy\
**			and to populate the list application\
**			entities.  All other functions are\
**			local to this specific interface.\
** Last Update:		$Author: smm $, $Date: 1995-04-10 13:20:05 $\
** Source File:		$RCSfile: securityForm.i,v $\
** Revision:		$Revision: 1.6 $\
** Status:		$State: Exp $\
*/\
\
#include <stdio.h>\
\
#include "dicom.h"\
#include "condition.h"\
#include "lst.h"\
#include "tbl.h"\
#include "manage.h"\
#include "mut.h"\
\
#include "format.h"\
\
extern DMAN_HANDLE *dmanHandle;\
static LST_HEAD *securityMatrixList = NULL;\
\
void loadSecurityMatrixList();\
static void displaySecurityMatrix(DMAN_SECURITYMATRIX *a);\
static CONDITION readSecurityMatrix(DMAN_SECURITYMATRIX *a);
*securityForm.ispecdecl:
*securityForm.funcdecl: swidget create_securityForm(swidget UxParent)
*securityForm.funcname: create_securityForm
*securityForm.funcdef: "swidget", "<create_securityForm>(%)"
*securityForm.argdecl: swidget UxParent;
*securityForm.arglist: UxParent
*securityForm.arglist.UxParent: "swidget", "%UxParent%"
*securityForm.icode:
*securityForm.fcode: return(rtrn);\

*securityForm.auxdecl: void loadSecurityMatrixList()\
{\
  CONDITION\
    cond;\
  DMAN_SECURITYMATRIX\
    a,\
    criteria;\
  char\
    b[256];\
\
    if (securityMatrixList == NULL)\
	securityMatrixList = LST_Create();\
\
    if (securityMatrixList == NULL)\
	return;\
\
    memset(&a, 0, sizeof(a));\
    a.Type = DMAN_K_SECURITYMATRIX;\
    criteria.Type = DMAN_K_SECURITYMATRIX;\
    criteria.Flag = 0;\
    cond = DMAN_Select(&dmanHandle,\
	(DMAN_GENERICRECORD *)&a,\
	(DMAN_GENERICRECORD *)&criteria,\
	securityMatrixList, NULL, NULL, NULL);\
    if (cond != DMAN_NORMAL) {\
	COND_DumpConditions();\
	return;\
    }\
    (void) LST_Sort(&securityMatrixList, sizeof(a),\
	compareSecurityMatrix);\
    MUT_LoadList(wSecurityMatrixList, securityMatrixList,\
	formatSecurityMatrix, b);\
}\
\
static void displaySecurityMatrix(DMAN_SECURITYMATRIX *a)\
{\
    XmTextSetString(wRequestingApplicationTxt, a->RequestingTitle);\
    XmTextSetString(wRespondingApplicationTxt, a->RespondingTitle);\
}\
\
static CONDITION readSecurityMatrix(DMAN_SECURITYMATRIX *a)\
{\
  CONDITION\
    cond;\
  CTNBOOLEAN\
    nullFlag;\
\
    memset(a, 0, sizeof(*a));\
    a->Type = DMAN_K_SECURITYMATRIX;\
\
    cond = MUT_ScanWidget(wRequestingApplicationTxt, MUT_TEXT, &nullFlag,\
	a->RequestingTitle);\
    if (cond != MUT_NORMAL || nullFlag)\
	return 0;\
    a->Flag |= DMAN_K_SECURITY_REQUESTING;\
\
    cond = MUT_ScanWidget(wRespondingApplicationTxt, MUT_TEXT, &nullFlag,\
	a->RespondingTitle);\
    if (cond != MUT_NORMAL || nullFlag)\
	return 0;\
    a->Flag |= DMAN_K_SECURITY_RESPONDING;\
\
    return 1;\
}
*securityForm.static: true
*securityForm.name: securityForm
*securityForm.parent: NO_PARENT
*securityForm.parentExpression: UxParent
*securityForm.defaultShell: topLevelShell
*securityForm.width: 690
*securityForm.height: 460
*securityForm.resizePolicy: "resize_none"
*securityForm.isCompound: "true"
*securityForm.compoundIcon: "form.xpm"
*securityForm.compoundName: "form_"
*securityForm.x: 494
*securityForm.y: 120
*securityForm.unitType: "pixels"

*label5.class: label
*label5.static: true
*label5.name: label5
*label5.parent: securityForm
*label5.isCompound: "true"
*label5.compoundIcon: "label.xpm"
*label5.compoundName: "label_"
*label5.x: 50
*label5.y: 200
*label5.width: 200
*label5.height: 40
*label5.labelString: "Requesting Application (Vendor)"

*label6.class: label
*label6.static: true
*label6.name: label6
*label6.parent: securityForm
*label6.isCompound: "true"
*label6.compoundIcon: "label.xpm"
*label6.compoundName: "label_"
*label6.x: 50
*label6.y: 240
*label6.width: 200
*label6.height: 40
*label6.labelString: "Responding Application (CTN)"
*label6.leftAttachment: "attach_opposite_widget"
*label6.leftOffset: 0
*label6.leftWidget: "label5"
*label6.topAttachment: "attach_widget"
*label6.topOffset: 15
*label6.topWidget: "label5"

*wRequestingApplicationTxt.class: textField
*wRequestingApplicationTxt.static: true
*wRequestingApplicationTxt.name: wRequestingApplicationTxt
*wRequestingApplicationTxt.parent: securityForm
*wRequestingApplicationTxt.width: 370
*wRequestingApplicationTxt.isCompound: "true"
*wRequestingApplicationTxt.compoundIcon: "textfield.xpm"
*wRequestingApplicationTxt.compoundName: "text_Field"
*wRequestingApplicationTxt.x: 280
*wRequestingApplicationTxt.y: 210
*wRequestingApplicationTxt.height: 40
*wRequestingApplicationTxt.leftAttachment: "attach_widget"
*wRequestingApplicationTxt.leftOffset: 15
*wRequestingApplicationTxt.leftWidget: "label5"
*wRequestingApplicationTxt.topAttachment: "attach_opposite_widget"
*wRequestingApplicationTxt.topOffset: 0
*wRequestingApplicationTxt.topWidget: "label5"

*wRespondingApplicationTxt.class: textField
*wRespondingApplicationTxt.static: true
*wRespondingApplicationTxt.name: wRespondingApplicationTxt
*wRespondingApplicationTxt.parent: securityForm
*wRespondingApplicationTxt.width: 370
*wRespondingApplicationTxt.isCompound: "true"
*wRespondingApplicationTxt.compoundIcon: "textfield.xpm"
*wRespondingApplicationTxt.compoundName: "text_Field"
*wRespondingApplicationTxt.x: 280
*wRespondingApplicationTxt.y: 250
*wRespondingApplicationTxt.height: 40
*wRespondingApplicationTxt.leftAttachment: "attach_widget"
*wRespondingApplicationTxt.leftOffset: 15
*wRespondingApplicationTxt.leftWidget: "label5"
*wRespondingApplicationTxt.topAttachment: "attach_opposite_widget"
*wRespondingApplicationTxt.topOffset: 0
*wRespondingApplicationTxt.topWidget: "label6"

*pushButton4.class: pushButton
*pushButton4.static: true
*pushButton4.name: pushButton4
*pushButton4.parent: securityForm
*pushButton4.isCompound: "true"
*pushButton4.compoundIcon: "push.xpm"
*pushButton4.compoundName: "push_Button"
*pushButton4.x: 50
*pushButton4.y: 350
*pushButton4.width: 160
*pushButton4.height: 50
*pushButton4.labelString: "Add"
*pushButton4.activateCallback: {\
  DMAN_SECURITYMATRIX\
    a;\
  CONDITION\
    cond;\
\
    cond = readSecurityMatrix(&a);\
    if (cond != 1)\
	return;\
\
    cond = DMAN_Insert(&dmanHandle, (DMAN_GENERICRECORD *)&a);\
    if (cond != DMAN_NORMAL)\
	COND_DumpConditions();\
\
    loadSecurityMatrixList();\
}

*pushButton5.class: pushButton
*pushButton5.static: true
*pushButton5.name: pushButton5
*pushButton5.parent: securityForm
*pushButton5.isCompound: "true"
*pushButton5.compoundIcon: "push.xpm"
*pushButton5.compoundName: "push_Button"
*pushButton5.x: 270
*pushButton5.y: 350
*pushButton5.width: 160
*pushButton5.height: 50
*pushButton5.labelString: "Update"
*pushButton5.activateCallback: {\
/*\
  DMAN_APPLICATIONENTITY\
    ae;\
  CONDITION\
    cond;\
\
    cond = readAE(&ae);\
    if (cond != 1)\
	return;\
\
    cond = DMAN_Insert(&dmanHandle, (DMAN_GENERICRECORD *)&ae);\
    if (cond != DMAN_NORMAL)\
	COND_DumpConditions();\
\
    loadApplicationList();\
*/\
}
*pushButton5.sensitive: "false"

*pushButton6.class: pushButton
*pushButton6.static: true
*pushButton6.name: pushButton6
*pushButton6.parent: securityForm
*pushButton6.isCompound: "true"
*pushButton6.compoundIcon: "push.xpm"
*pushButton6.compoundName: "push_Button"
*pushButton6.x: 500
*pushButton6.y: 350
*pushButton6.width: 160
*pushButton6.height: 50
*pushButton6.labelString: "Delete"
*pushButton6.activateCallback: {\
  DMAN_SECURITYMATRIX\
    a;\
  CONDITION\
    cond;\
\
    cond = readSecurityMatrix(&a);\
    if (cond != 1)\
	return;\
\
    cond = DMAN_Delete(&dmanHandle, (DMAN_GENERICRECORD *)&a);\
    if (cond != DMAN_NORMAL)\
	COND_DumpConditions();\
\
    loadSecurityMatrixList();\
}

*frame2.class: frame
*frame2.static: true
*frame2.name: frame2
*frame2.parent: securityForm
*frame2.width: 610
*frame2.height: 150
*frame2.isCompound: "true"
*frame2.compoundIcon: "frame.xpm"
*frame2.compoundName: "frame_"
*frame2.x: 40
*frame2.y: 20
*frame2.resizable: "false"
*frame2.resizeRecursion: "up"

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
*scrolledWindowList2.x: 110
*scrolledWindowList2.y: 60

*wSecurityMatrixList.class: scrolledList
*wSecurityMatrixList.static: true
*wSecurityMatrixList.name: wSecurityMatrixList
*wSecurityMatrixList.parent: scrolledWindowList2
*wSecurityMatrixList.width: 170
*wSecurityMatrixList.height: 40
*wSecurityMatrixList.browseSelectionCallback: {\
  XmListCallbackStruct *l;\
  DMAN_SECURITYMATRIX\
    *a;\
  int i;\
\
    l = (XmListCallbackStruct *)UxCallbackArg;\
\
    a = LST_Head(&securityMatrixList);\
    (void) LST_Position(&securityMatrixList, a);\
    for (i = 2; i <= l->item_position; i++)\
	a = LST_Next(&securityMatrixList);\
\
    displaySecurityMatrix(a);\
}

