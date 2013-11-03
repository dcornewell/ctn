! UIMX ascii 2.5 key: 2326                                                      

*FISAccessForm.class: form
*FISAccessForm.gbldecl: /*\
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
** Module Name(s):      create_FISAccessForm\
**                      loadFISAccessList\
** Author, Date:        Steve Moore, Summer 1994\
** Intent:              This file provides the user interface\
**                      for defining and modifying entries in\
**                      the FIS Access table.\
**                      The public entry points are\
**                      call to create the widget hierarchy\
**                      and to populate the list application\
**                      entities.  All other functions are\
**                      local to this specific interface.\
** Last Update:         $Author: smm $, $Date: 1995-04-10 13:19:11 $\
** Source File:         $RCSfile: FISAccessForm.i,v $\
** Revision:            $Revision: 1.5 $\
** Status:              $State: Exp $\
*/\
\
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
static LST_HEAD *FISAccessList = NULL;\
\
void loadFISAccessList();\
static void displayFISAccess(DMAN_FISACCESS *f);\
static CONDITION readFISAccess(DMAN_FISACCESS *f);
*FISAccessForm.ispecdecl:
*FISAccessForm.funcdecl: swidget create_FISAccessForm(swidget UxParent)
*FISAccessForm.funcname: create_FISAccessForm
*FISAccessForm.funcdef: "swidget", "<create_FISAccessForm>(%)"
*FISAccessForm.argdecl: swidget UxParent;
*FISAccessForm.arglist: UxParent
*FISAccessForm.arglist.UxParent: "swidget", "%UxParent%"
*FISAccessForm.icode:
*FISAccessForm.fcode: return(rtrn);\

*FISAccessForm.auxdecl: void loadFISAccessList()\
{\
  CONDITION cond;\
  DMAN_FISACCESS f, criteria;\
  char b[256];\
\
    if (FISAccessList == NULL)\
	FISAccessList = LST_Create();\
    if (FISAccessList == NULL)\
	return;\
\
    memset(&f, 0, sizeof(f));\
    f.Type = DMAN_K_FISACCESS;\
    criteria.Type = DMAN_K_FISACCESS;\
    criteria.Flag = 0;\
    cond = DMAN_Select(&dmanHandle,\
	(DMAN_GENERICRECORD *)&f,\
	(DMAN_GENERICRECORD *)&criteria,\
	FISAccessList, NULL, NULL, NULL);\
    if (cond != DMAN_NORMAL)\
	return;\
\
    MUT_LoadList(wFISAccessList, FISAccessList,\
	formatFISAccess, b);\
}\
\
static void displayFISAccess(DMAN_FISACCESS *f)\
{\
    if (f == NULL)\
	return;\
\
    XmTextSetString(wTitleTxt, f->Title);\
    XmTextSetString(wDatabaseKeyTxt, f->DbKey);\
    XmTextSetString(wGroupTxt, f->GroupName);\
    XmTextSetString(wCommentTxt, f->Comment);\
}\
\
static CONDITION readFISAccess (DMAN_FISACCESS *f)\
{\
  CONDITION cond;\
  CTNBOOLEAN nullFlag;\
\
    memset(f, 0, sizeof(*f));\
    f->Type = DMAN_K_FISACCESS;\
\
    cond = MUT_ScanWidget(wTitleTxt, MUT_TEXT, &nullFlag,\
	f->Title);\
    if (cond != MUT_NORMAL || nullFlag)\
	return 0;\
    f->Flag |= DMAN_K_FISACCESS_TITLE;\
\
    cond = MUT_ScanWidget(wDatabaseKeyTxt, MUT_TEXT, &nullFlag,\
	f->DbKey);\
    if (cond != MUT_NORMAL || nullFlag)\
	return 0;\
    f->Flag |= DMAN_K_FISACCESS_DBKEY;\
\
    cond = MUT_ScanWidget(wGroupTxt, MUT_TEXT, &nullFlag,\
	f->GroupName);\
    if (cond != MUT_NORMAL || nullFlag)\
	return 0;\
    f->Flag |= DMAN_K_FISACCESS_GROUPNAME;\
\
    cond = MUT_ScanWidget(wCommentTxt, MUT_TEXT, &nullFlag,\
	f->Comment);\
    if (cond != MUT_NORMAL || nullFlag)\
	return 0;\
    f->Flag |= DMAN_K_FISACCESS_COMMENT;\
\
    return 1;\
}
*FISAccessForm.static: true
*FISAccessForm.name: FISAccessForm
*FISAccessForm.parent: NO_PARENT
*FISAccessForm.parentExpression: UxParent
*FISAccessForm.defaultShell: topLevelShell
*FISAccessForm.width: 655
*FISAccessForm.height: 480
*FISAccessForm.resizePolicy: "resize_none"
*FISAccessForm.isCompound: "true"
*FISAccessForm.compoundIcon: "form.xpm"
*FISAccessForm.compoundName: "form_"
*FISAccessForm.x: 180
*FISAccessForm.y: 306
*FISAccessForm.unitType: "pixels"

*label12.class: label
*label12.static: true
*label12.name: label12
*label12.parent: FISAccessForm
*label12.isCompound: "true"
*label12.compoundIcon: "label.xpm"
*label12.compoundName: "label_"
*label12.x: 40
*label12.y: 160
*label12.width: 170
*label12.height: 40
*label12.labelString: "Title"

*label13.class: label
*label13.static: true
*label13.name: label13
*label13.parent: FISAccessForm
*label13.isCompound: "true"
*label13.compoundIcon: "label.xpm"
*label13.compoundName: "label_"
*label13.x: 36
*label13.y: 186
*label13.width: 170
*label13.height: 40
*label13.labelString: "Database Key"
*label13.leftAttachment: "attach_opposite_widget"
*label13.leftOffset: 0
*label13.leftWidget: "label12"
*label13.topAttachment: "attach_widget"
*label13.topOffset: 15
*label13.topWidget: "label12"

*label14.class: label
*label14.static: true
*label14.name: label14
*label14.parent: FISAccessForm
*label14.isCompound: "true"
*label14.compoundIcon: "label.xpm"
*label14.compoundName: "label_"
*label14.x: 23
*label14.y: 306
*label14.width: 170
*label14.height: 40
*label14.labelString: "Group"
*label14.leftAttachment: "attach_opposite_widget"
*label14.leftOffset: 0
*label14.leftWidget: "label12"
*label14.topAttachment: "attach_widget"
*label14.topOffset: 15
*label14.topWidget: "label13"

*label15.class: label
*label15.static: true
*label15.name: label15
*label15.parent: FISAccessForm
*label15.isCompound: "true"
*label15.compoundIcon: "label.xpm"
*label15.compoundName: "label_"
*label15.x: 23
*label15.y: 361
*label15.width: 170
*label15.height: 40
*label15.labelString: "Comment"
*label15.leftAttachment: "attach_opposite_widget"
*label15.leftOffset: 0
*label15.leftWidget: "label12"
*label15.topAttachment: "attach_widget"
*label15.topOffset: 15
*label15.topWidget: "label14"

*wGroupTxt.class: textField
*wGroupTxt.static: true
*wGroupTxt.name: wGroupTxt
*wGroupTxt.parent: FISAccessForm
*wGroupTxt.width: 375
*wGroupTxt.isCompound: "true"
*wGroupTxt.compoundIcon: "textfield.xpm"
*wGroupTxt.compoundName: "text_Field"
*wGroupTxt.x: 213
*wGroupTxt.y: 306
*wGroupTxt.height: 40
*wGroupTxt.leftAttachment: "attach_widget"
*wGroupTxt.leftOffset: 15
*wGroupTxt.leftWidget: "label12"
*wGroupTxt.topAttachment: "attach_opposite_widget"
*wGroupTxt.topOffset: 0
*wGroupTxt.topWidget: "label14"

*wCommentTxt.class: textField
*wCommentTxt.static: true
*wCommentTxt.name: wCommentTxt
*wCommentTxt.parent: FISAccessForm
*wCommentTxt.width: 380
*wCommentTxt.isCompound: "true"
*wCommentTxt.compoundIcon: "textfield.xpm"
*wCommentTxt.compoundName: "text_Field"
*wCommentTxt.x: 263
*wCommentTxt.y: 361
*wCommentTxt.height: 40
*wCommentTxt.leftAttachment: "attach_widget"
*wCommentTxt.leftOffset: 15
*wCommentTxt.leftWidget: "label12"
*wCommentTxt.topAttachment: "attach_opposite_widget"
*wCommentTxt.topOffset: 0
*wCommentTxt.topWidget: "label15"

*wTitleTxt.class: textField
*wTitleTxt.static: true
*wTitleTxt.name: wTitleTxt
*wTitleTxt.parent: FISAccessForm
*wTitleTxt.width: 380
*wTitleTxt.isCompound: "true"
*wTitleTxt.compoundIcon: "textfield.xpm"
*wTitleTxt.compoundName: "text_Field"
*wTitleTxt.x: 191
*wTitleTxt.y: 210
*wTitleTxt.height: 40
*wTitleTxt.leftAttachment: "attach_widget"
*wTitleTxt.leftOffset: 15
*wTitleTxt.leftWidget: "label12"
*wTitleTxt.topAttachment: "attach_opposite_widget"
*wTitleTxt.topOffset: 0
*wTitleTxt.topWidget: "label12"

*wDatabaseKeyTxt.class: textField
*wDatabaseKeyTxt.static: true
*wDatabaseKeyTxt.name: wDatabaseKeyTxt
*wDatabaseKeyTxt.parent: FISAccessForm
*wDatabaseKeyTxt.width: 380
*wDatabaseKeyTxt.isCompound: "true"
*wDatabaseKeyTxt.compoundIcon: "textfield.xpm"
*wDatabaseKeyTxt.compoundName: "text_Field"
*wDatabaseKeyTxt.x: 211
*wDatabaseKeyTxt.y: 260
*wDatabaseKeyTxt.height: 40
*wDatabaseKeyTxt.leftAttachment: "attach_widget"
*wDatabaseKeyTxt.leftOffset: 15
*wDatabaseKeyTxt.leftWidget: "label12"
*wDatabaseKeyTxt.topAttachment: "attach_opposite_widget"
*wDatabaseKeyTxt.topOffset: 0
*wDatabaseKeyTxt.topWidget: "label13"

*pushButton12.class: pushButton
*pushButton12.static: true
*pushButton12.name: pushButton12
*pushButton12.parent: FISAccessForm
*pushButton12.isCompound: "true"
*pushButton12.compoundIcon: "push.xpm"
*pushButton12.compoundName: "push_Button"
*pushButton12.x: 41
*pushButton12.y: 420
*pushButton12.width: 140
*pushButton12.height: 40
*pushButton12.labelString: "Add"
*pushButton12.activateCallback: {\
  DMAN_FISACCESS f;\
  CONDITION cond;\
\
    cond = readFISAccess(&f);\
    if (cond != 1) {\
	COND_DumpConditions();\
	return;\
    }\
\
    cond = DMAN_Insert(&dmanHandle, (DMAN_GENERICRECORD *)&f);\
    if (cond != DMAN_NORMAL)\
	COND_DumpConditions();\
\
    loadFISAccessList();\
}

*pushButton13.class: pushButton
*pushButton13.static: true
*pushButton13.name: pushButton13
*pushButton13.parent: FISAccessForm
*pushButton13.isCompound: "true"
*pushButton13.compoundIcon: "push.xpm"
*pushButton13.compoundName: "push_Button"
*pushButton13.x: 231
*pushButton13.y: 420
*pushButton13.width: 140
*pushButton13.height: 40
*pushButton13.labelString: "Update"
*pushButton13.sensitive: "false"

*pushButton14.class: pushButton
*pushButton14.static: true
*pushButton14.name: pushButton14
*pushButton14.parent: FISAccessForm
*pushButton14.isCompound: "true"
*pushButton14.compoundIcon: "push.xpm"
*pushButton14.compoundName: "push_Button"
*pushButton14.x: 421
*pushButton14.y: 420
*pushButton14.width: 140
*pushButton14.height: 40
*pushButton14.labelString: "Delete"
*pushButton14.activateCallback: {\
  DMAN_FISACCESS f;\
  CONDITION cond;\
\
    cond = readFISAccess(&f);\
    if (cond != 1) {\
	COND_DumpConditions();\
	return;\
    }\
\
    cond = DMAN_Delete(&dmanHandle, (DMAN_GENERICRECORD *)&f);\
    if (cond != DMAN_NORMAL)\
	COND_DumpConditions();\
\
    loadFISAccessList();\
}

*frame4.class: frame
*frame4.static: true
*frame4.name: frame4
*frame4.parent: FISAccessForm
*frame4.width: 620
*frame4.height: 110
*frame4.isCompound: "true"
*frame4.compoundIcon: "frame.xpm"
*frame4.compoundName: "frame_"
*frame4.x: 20
*frame4.y: 20

*scrolledWindowList6.class: scrolledWindow
*scrolledWindowList6.static: true
*scrolledWindowList6.name: scrolledWindowList6
*scrolledWindowList6.parent: frame4
*scrolledWindowList6.scrollingPolicy: "application_defined"
*scrolledWindowList6.visualPolicy: "variable"
*scrolledWindowList6.scrollBarDisplayPolicy: "static"
*scrolledWindowList6.shadowThickness: 0
*scrolledWindowList6.isCompound: "true"
*scrolledWindowList6.compoundIcon: "scrllist.xpm"
*scrolledWindowList6.compoundName: "scrolled_List"
*scrolledWindowList6.x: 180
*scrolledWindowList6.y: 10

*wFISAccessList.class: scrolledList
*wFISAccessList.static: true
*wFISAccessList.name: wFISAccessList
*wFISAccessList.parent: scrolledWindowList6
*wFISAccessList.width: 130
*wFISAccessList.height: 90

