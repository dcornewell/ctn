! UIMX ascii 2.5 key: 9309                                                      

*imageStorageControlForm.class: form
*imageStorageControlForm.gbldecl: /*\
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
** Module Name(s):	create_imageStorageControlForm\
**			loadImageStorageControlList\
** Author, Date:	Steve Moore, Summer 1994\
** Intent:		This file provides the user interface\
** 			for defining and modifying entries in\
** 			the Image Storage Control table.  This table\
**			is used to define the directories used to\
**			store images as they are received from external\
**			applications.\
**			The public entry points are\
**			call to create the widget hierarchy\
** 			and to populate the list application\
**			entities.  All other functions are\
**			local to this specific interface.\
** Last Update:		$Author: smm $, $Date: 1995-04-10 13:19:55 $\
** Source File:		$RCSfile: imageStorageControlForm.i,v $\
** Revision:		$Revision: 1.4 $\
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
\
static LST_HEAD *storageControlList = NULL;\

*imageStorageControlForm.ispecdecl:
*imageStorageControlForm.funcdecl: swidget create_imageStorageControlForm(swidget UxParent)
*imageStorageControlForm.funcname: create_imageStorageControlForm
*imageStorageControlForm.funcdef: "swidget", "<create_imageStorageControlForm>(%)"
*imageStorageControlForm.argdecl: swidget UxParent;
*imageStorageControlForm.arglist: UxParent
*imageStorageControlForm.arglist.UxParent: "swidget", "%UxParent%"
*imageStorageControlForm.icode:
*imageStorageControlForm.fcode: return(rtrn);\

*imageStorageControlForm.auxdecl: void loadImageStorageControlList()\
{\
  CONDITION cond;\
  DMAN_STORAGECONTROL s, criteria;\
\
  char buf[120];\
\
    if (storageControlList == NULL)\
	storageControlList = LST_Create();\
    if (storageControlList == NULL)\
	return;\
\
    memset(&s, 0, sizeof(s));\
    memset(&criteria, 0, sizeof(criteria));\
    s.Type = criteria.Type = DMAN_K_STORAGECONTROL;\
\
    cond = DMAN_Select(&dmanHandle, (DMAN_GENERICRECORD *)&s,\
	(DMAN_GENERICRECORD *)&criteria,\
	storageControlList, NULL, NULL, NULL);\
    if (cond != DMAN_NORMAL) {\
	COND_DumpConditions();\
	return;\
    }\
\
    MUT_LoadList(wStorageControlList, storageControlList,\
	formatStorageControl, buf);\
}\
\
static CONDITION\
readStorageControl(DMAN_STORAGECONTROL *s)\
{\
  CONDITION cond;\
  CTNBOOLEAN nullFlag;\
\
    if (s == NULL)\
	return 0;\
    memset(s, 0, sizeof(*s));\
    s->Type = DMAN_K_STORAGECONTROL;\
    s->Flag = 0;\
\
    cond = MUT_ScanWidget(wRequestingApplicationTxt, MUT_TEXT,\
	&nullFlag, s->RequestingTitle);\
    if (cond != MUT_NORMAL || nullFlag)\
	return 0;\
    s->Flag |= DMAN_K_STORAGECONTROL_REQUESTING;\
\
    cond = MUT_ScanWidget(wRespondingApplicationTxt, MUT_TEXT,\
	&nullFlag, s->RespondingTitle);\
    if (cond != MUT_NORMAL || nullFlag)\
	return 0;\
    s->Flag |= DMAN_K_STORAGECONTROL_RESPONDING;\
\
    cond = MUT_ScanWidget(wMediumTxt, MUT_TEXT, &nullFlag,\
	s->Medium);\
    if (cond != MUT_NORMAL)\
	return 0;\
    if (!nullFlag)\
	s->Flag |= DMAN_K_STORAGECONTROL_MEDIUM;\
\
    cond = MUT_ScanWidget(wRootTxt, MUT_TEXT, &nullFlag, s->Root);\
    if (cond != MUT_NORMAL || nullFlag) {\
	return 0;\
    }\
    s->Flag |= DMAN_K_STORAGECONTROL_ROOT;\
\
    return 1;\
}\
\
static void displayStorageControl(DMAN_STORAGECONTROL *s)\
{\
    if (s == NULL)\
	return;\
\
    if (s->Flag & DMAN_K_STORAGECONTROL_REQUESTING)\
	XmTextSetString(wRequestingApplicationTxt, s->RequestingTitle);\
    else\
	XmTextSetString(wRequestingApplicationTxt, "");\
\
    if (s->Flag & DMAN_K_STORAGECONTROL_RESPONDING)\
	XmTextSetString(wRespondingApplicationTxt, s->RespondingTitle);\
    else\
	XmTextSetString(wRespondingApplicationTxt, "");\
\
    if (s->Flag & DMAN_K_STORAGECONTROL_MEDIUM)\
	XmTextSetString(wMediumTxt, s->Medium);\
    else\
	XmTextSetString(wMediumTxt, "");\
\
    if (s->Flag & DMAN_K_STORAGECONTROL_ROOT)\
	XmTextSetString(wRootTxt, s->Root);\
    else\
	XmTextSetString(wRootTxt, "");\
}
*imageStorageControlForm.static: true
*imageStorageControlForm.name: imageStorageControlForm
*imageStorageControlForm.parent: NO_PARENT
*imageStorageControlForm.parentExpression: UxParent
*imageStorageControlForm.defaultShell: topLevelShell
*imageStorageControlForm.resizePolicy: "resize_none"
*imageStorageControlForm.unitType: "pixels"
*imageStorageControlForm.x: 260
*imageStorageControlForm.y: 310
*imageStorageControlForm.width: 770
*imageStorageControlForm.height: 490

*frame7.class: frame
*frame7.static: true
*frame7.name: frame7
*frame7.parent: imageStorageControlForm
*frame7.width: 740
*frame7.height: 140
*frame7.isCompound: "true"
*frame7.compoundIcon: "frame.xpm"
*frame7.compoundName: "frame_"
*frame7.x: 20
*frame7.y: 10
*frame7.resizable: "false"
*frame7.resizeRecursion: "up"

*scrolledWindowList8.class: scrolledWindow
*scrolledWindowList8.static: true
*scrolledWindowList8.name: scrolledWindowList8
*scrolledWindowList8.parent: frame7
*scrolledWindowList8.scrollingPolicy: "application_defined"
*scrolledWindowList8.visualPolicy: "variable"
*scrolledWindowList8.scrollBarDisplayPolicy: "static"
*scrolledWindowList8.shadowThickness: 0
*scrolledWindowList8.isCompound: "true"
*scrolledWindowList8.compoundIcon: "scrllist.xpm"
*scrolledWindowList8.compoundName: "scrolled_List"
*scrolledWindowList8.x: 50
*scrolledWindowList8.y: 30

*wStorageControlList.class: scrolledList
*wStorageControlList.static: true
*wStorageControlList.name: wStorageControlList
*wStorageControlList.parent: scrolledWindowList8
*wStorageControlList.width: 260
*wStorageControlList.height: 40
*wStorageControlList.browseSelectionCallback: {\
  XmListCallbackStruct *l;\
  DMAN_STORAGECONTROL *s;\
\
    l = (XmListCallbackStruct *)UxCallbackArg;\
    s = LST_Index(&storageControlList, l->item_position);\
    displayStorageControl(s);\
}

*label22.class: label
*label22.static: true
*label22.name: label22
*label22.parent: imageStorageControlForm
*label22.isCompound: "true"
*label22.compoundIcon: "label.xpm"
*label22.compoundName: "label_"
*label22.x: 30
*label22.y: 170
*label22.width: 250
*label22.height: 50
*label22.labelString: "Requesting Application"

*wRequestingApplicationTxt.class: textField
*wRequestingApplicationTxt.static: true
*wRequestingApplicationTxt.name: wRequestingApplicationTxt
*wRequestingApplicationTxt.parent: imageStorageControlForm
*wRequestingApplicationTxt.width: 440
*wRequestingApplicationTxt.isCompound: "true"
*wRequestingApplicationTxt.compoundIcon: "textfield.xpm"
*wRequestingApplicationTxt.compoundName: "text_Field"
*wRequestingApplicationTxt.x: 320
*wRequestingApplicationTxt.y: 170
*wRequestingApplicationTxt.height: 50
*wRequestingApplicationTxt.leftAttachment: "attach_widget"
*wRequestingApplicationTxt.leftOffset: 15
*wRequestingApplicationTxt.leftWidget: "label22"
*wRequestingApplicationTxt.topAttachment: "attach_opposite_widget"
*wRequestingApplicationTxt.topOffset: 0
*wRequestingApplicationTxt.topWidget: "label22"

*label23.class: label
*label23.static: true
*label23.name: label23
*label23.parent: imageStorageControlForm
*label23.isCompound: "true"
*label23.compoundIcon: "label.xpm"
*label23.compoundName: "label_"
*label23.x: 30
*label23.y: 220
*label23.width: 250
*label23.height: 50
*label23.leftAttachment: "attach_opposite_widget"
*label23.leftOffset: 0
*label23.leftWidget: "label22"
*label23.topAttachment: "attach_widget"
*label23.topOffset: 10
*label23.topWidget: "label22"
*label23.labelString: "Responding Application"

*label24.class: label
*label24.static: true
*label24.name: label24
*label24.parent: imageStorageControlForm
*label24.isCompound: "true"
*label24.compoundIcon: "label.xpm"
*label24.compoundName: "label_"
*label24.x: 30
*label24.y: 260
*label24.width: 250
*label24.height: 50
*label24.leftAttachment: "attach_opposite_widget"
*label24.leftWidget: "label22"
*label24.topAttachment: "attach_widget"
*label24.topOffset: 10
*label24.topWidget: "label23"
*label24.labelString: "Medium"

*label25.class: label
*label25.static: true
*label25.name: label25
*label25.parent: imageStorageControlForm
*label25.isCompound: "true"
*label25.compoundIcon: "label.xpm"
*label25.compoundName: "label_"
*label25.x: 30
*label25.y: 300
*label25.width: 250
*label25.height: 50
*label25.leftAttachment: "attach_opposite_widget"
*label25.leftOffset: 0
*label25.leftWidget: "label22"
*label25.topAttachment: "attach_widget"
*label25.topOffset: 10
*label25.topWidget: "label24"
*label25.labelString: "(File System) Root"

*wRespondingApplicationTxt.class: textField
*wRespondingApplicationTxt.static: true
*wRespondingApplicationTxt.name: wRespondingApplicationTxt
*wRespondingApplicationTxt.parent: imageStorageControlForm
*wRespondingApplicationTxt.width: 440
*wRespondingApplicationTxt.isCompound: "true"
*wRespondingApplicationTxt.compoundIcon: "textfield.xpm"
*wRespondingApplicationTxt.compoundName: "text_Field"
*wRespondingApplicationTxt.x: 320
*wRespondingApplicationTxt.y: 220
*wRespondingApplicationTxt.height: 50
*wRespondingApplicationTxt.leftAttachment: "attach_widget"
*wRespondingApplicationTxt.leftOffset: 15
*wRespondingApplicationTxt.leftWidget: "label22"
*wRespondingApplicationTxt.topAttachment: "attach_opposite_widget"
*wRespondingApplicationTxt.topOffset: 0
*wRespondingApplicationTxt.topWidget: "label23"

*wMediumTxt.class: textField
*wMediumTxt.static: true
*wMediumTxt.name: wMediumTxt
*wMediumTxt.parent: imageStorageControlForm
*wMediumTxt.width: 440
*wMediumTxt.isCompound: "true"
*wMediumTxt.compoundIcon: "textfield.xpm"
*wMediumTxt.compoundName: "text_Field"
*wMediumTxt.x: 320
*wMediumTxt.y: 270
*wMediumTxt.height: 50
*wMediumTxt.leftAttachment: "attach_widget"
*wMediumTxt.leftOffset: 15
*wMediumTxt.leftWidget: "label22"
*wMediumTxt.topAttachment: "attach_opposite_widget"
*wMediumTxt.topOffset: 0
*wMediumTxt.topWidget: "label24"

*wRootTxt.class: textField
*wRootTxt.static: true
*wRootTxt.name: wRootTxt
*wRootTxt.parent: imageStorageControlForm
*wRootTxt.width: 440
*wRootTxt.isCompound: "true"
*wRootTxt.compoundIcon: "textfield.xpm"
*wRootTxt.compoundName: "text_Field"
*wRootTxt.x: 320
*wRootTxt.y: 320
*wRootTxt.height: 50
*wRootTxt.leftAttachment: "attach_widget"
*wRootTxt.leftOffset: 15
*wRootTxt.leftWidget: "label22"
*wRootTxt.topAttachment: "attach_opposite_widget"
*wRootTxt.topOffset: 0
*wRootTxt.topWidget: "label25"

*pushButton18.class: pushButton
*pushButton18.static: true
*pushButton18.name: pushButton18
*pushButton18.parent: imageStorageControlForm
*pushButton18.isCompound: "true"
*pushButton18.compoundIcon: "push.xpm"
*pushButton18.compoundName: "push_Button"
*pushButton18.x: 60
*pushButton18.y: 410
*pushButton18.width: 150
*pushButton18.height: 50
*pushButton18.labelString: "Add"
*pushButton18.activateCallback: {\
  CONDITION cond;\
  DMAN_STORAGECONTROL s;\
\
    cond = readStorageControl(&s);\
    if (cond != 1) {\
	return;\
    }\
\
    cond = DMAN_Insert(&dmanHandle, (DMAN_GENERICRECORD *)&s);\
    if (cond != DMAN_NORMAL) {\
	COND_DumpConditions();\
    }\
    loadImageStorageControlList();\
}

*pushButton19.class: pushButton
*pushButton19.static: true
*pushButton19.name: pushButton19
*pushButton19.parent: imageStorageControlForm
*pushButton19.isCompound: "true"
*pushButton19.compoundIcon: "push.xpm"
*pushButton19.compoundName: "push_Button"
*pushButton19.x: 310
*pushButton19.y: 410
*pushButton19.width: 150
*pushButton19.height: 50
*pushButton19.labelString: "Update"
*pushButton19.sensitive: "false"

*pushButton20.class: pushButton
*pushButton20.static: true
*pushButton20.name: pushButton20
*pushButton20.parent: imageStorageControlForm
*pushButton20.isCompound: "true"
*pushButton20.compoundIcon: "push.xpm"
*pushButton20.compoundName: "push_Button"
*pushButton20.x: 560
*pushButton20.y: 410
*pushButton20.width: 150
*pushButton20.height: 50
*pushButton20.labelString: "Delete"
*pushButton20.activateCallback: {\
  CONDITION cond;\
  DMAN_STORAGECONTROL s;\
\
    cond = readStorageControl(&s);\
    if (cond != 1) {\
	COND_DumpConditions();\
	return;\
    }\
    cond = DMAN_Delete(&dmanHandle, (DMAN_GENERICRECORD *)&s);\
    if (cond != DMAN_NORMAL) {\
	COND_DumpConditions();\
    }\
    loadImageStorageControlList();\
}

