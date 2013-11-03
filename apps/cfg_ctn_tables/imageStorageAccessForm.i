! UIMX ascii 2.5 key: 5270                                                      

*imageStorageAccessForm.class: form
*imageStorageAccessForm.gbldecl: /*\
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
** Module Name(s):      create_imageStorageAccessForm\
**                      loadImageStorageAccessList\
** Author, Date:        Steve Moore, Summer 1994\
** Intent:              This file provides the user interface\
**                      for defining and modifying entries in\
**                      the Image Storage Access table.  This table\
**			is used to define which (external) applications\
**			have access rights to storage areas manged\
**			by the image server or other applications.\
**                      The public entry points are\
**                      call to create the widget hierarchy\
**                      and to populate the list application\
**                      entities.  All other functions are\
**                      local to this specific interface.\
** Last Update:         $Author: smm $, $Date: 1995-04-10 13:19:51 $\
** Source File:         $RCSfile: imageStorageAccessForm.i,v $\
** Revision:            $Revision: 1.4 $\
** Status:              $State: Exp $\
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
static LST_HEAD *storageAccessList = NULL;\
extern DMAN_HANDLE *dmanHandle;\
void loadImageStorageAccessList();
*imageStorageAccessForm.ispecdecl:
*imageStorageAccessForm.funcdecl: swidget create_imageStorageAccessForm(swidget UxParent)
*imageStorageAccessForm.funcname: create_imageStorageAccessForm
*imageStorageAccessForm.funcdef: "swidget", "<create_imageStorageAccessForm>(%)"
*imageStorageAccessForm.argdecl: swidget UxParent;
*imageStorageAccessForm.arglist: UxParent
*imageStorageAccessForm.arglist.UxParent: "swidget", "%UxParent%"
*imageStorageAccessForm.icode:
*imageStorageAccessForm.fcode: return(rtrn);\

*imageStorageAccessForm.auxdecl: void loadImageStorageAccessList()\
{\
  CONDITION cond;\
  DMAN_STORAGEACCESS s, criteria;\
  char buf[120];\
\
    if (storageAccessList == NULL)\
	storageAccessList = LST_Create();\
\
    if (storageAccessList == NULL)\
	return;\
\
    memset(&s, 0, sizeof(s));\
    memset(&criteria, 0, sizeof(criteria));\
    s.Type = DMAN_K_STORAGEACCESS;\
    criteria.Type = DMAN_K_STORAGEACCESS;\
\
    cond = DMAN_Select(&dmanHandle, (DMAN_GENERICRECORD *)&s,\
	(DMAN_GENERICRECORD *)&criteria,\
	storageAccessList, NULL, NULL, NULL);\
    if (cond != DMAN_NORMAL) {\
	COND_DumpConditions();\
	return;\
    }\
\
    MUT_LoadList(wStorageAccessList, storageAccessList,\
	formatStorageAccess, buf);\
}\
\
static CONDITION readStorageAccess(DMAN_STORAGEACCESS *s)\
{\
  CONDITION cond;\
  CTNBOOLEAN nullFlag;\
\
    memset(s, 0, sizeof(*s));\
    s->Type = DMAN_K_STORAGEACCESS;\
    s->Flag = 0;\
    cond = MUT_ScanWidget(wApplicationTitleTxt, MUT_TEXT, &nullFlag, s->Title);\
    if (cond != MUT_NORMAL || nullFlag)\
	return 0;\
    s->Flag |= DMAN_K_STORAGEACCESS_TITLE;\
\
    cond = MUT_ScanWidget(wDatabaseKeyTxt, MUT_TEXT, &nullFlag, s->DbKey);\
    if (cond != MUT_NORMAL || nullFlag) {\
	return 0;\
    }\
    s->Flag |= DMAN_K_STORAGEACCESS_DBKEY;\
\
    cond = MUT_ScanWidget(wOwnerTxt, MUT_TEXT, &nullFlag, s->Owner);\
    if (cond != MUT_NORMAL) {\
	return 0;\
    }\
    if (!nullFlag)\
	s->Flag |= DMAN_K_STORAGEACCESS_OWNER;\
\
    cond = MUT_ScanWidget(wGroupNameTxt, MUT_TEXT, &nullFlag, s->GroupName);\
    if (cond != MUT_NORMAL) {\
	return 0;\
    }\
    if (!nullFlag)\
	s->Flag |= DMAN_K_STORAGEACCESS_GROUPNAME;\
\
    s->Access = 0xffffff;\
    s->Flag |= DMAN_K_STORAGEACCESS_ACCESS;\
\
    cond = MUT_ScanWidget(wCommentTxt, MUT_TEXT, &nullFlag, s->Comment);\
    if (cond != MUT_NORMAL) {\
	return 0;\
    }\
    if (!nullFlag)\
	s->Flag |= DMAN_K_STORAGEACCESS_COMMENT;\
\
    return 1;\
}\
\
static void displayStorageAccess(DMAN_STORAGEACCESS *s)\
{\
    if (s == NULL)\
	return;\
\
    if (s->Flag & DMAN_K_STORAGEACCESS_TITLE)\
	XmTextSetString(wApplicationTitleTxt, s->Title);\
    else\
	XmTextSetString(wApplicationTitleTxt, "");\
\
    if (s->Flag & DMAN_K_STORAGEACCESS_DBKEY)\
	XmTextSetString(wDatabaseKeyTxt, s->DbKey);\
    else\
	XmTextSetString(wDatabaseKeyTxt, "");\
\
    if (s->Flag & DMAN_K_STORAGEACCESS_OWNER)\
	XmTextSetString(wOwnerTxt, s->Owner);\
    else\
	XmTextSetString(wOwnerTxt, "");\
\
    if (s->Flag & DMAN_K_STORAGEACCESS_GROUPNAME)\
	XmTextSetString(wGroupNameTxt, s->GroupName);\
    else\
	XmTextSetString(wGroupNameTxt, "");\
\
    if (s->Flag & DMAN_K_STORAGEACCESS_COMMENT)\
	XmTextSetString(wCommentTxt, s->Comment);\
    else\
	XmTextSetString(wCommentTxt, "");\
}\

*imageStorageAccessForm.static: true
*imageStorageAccessForm.name: imageStorageAccessForm
*imageStorageAccessForm.parent: NO_PARENT
*imageStorageAccessForm.parentExpression: UxParent
*imageStorageAccessForm.defaultShell: topLevelShell
*imageStorageAccessForm.width: 795
*imageStorageAccessForm.height: 647
*imageStorageAccessForm.resizePolicy: "resize_none"
*imageStorageAccessForm.isCompound: "true"
*imageStorageAccessForm.compoundIcon: "form.xpm"
*imageStorageAccessForm.compoundName: "form_"
*imageStorageAccessForm.x: 275
*imageStorageAccessForm.y: 249
*imageStorageAccessForm.unitType: "pixels"

*frame5.class: frame
*frame5.static: true
*frame5.name: frame5
*frame5.parent: imageStorageAccessForm
*frame5.width: 782
*frame5.height: 170
*frame5.isCompound: "true"
*frame5.compoundIcon: "frame.xpm"
*frame5.compoundName: "frame_"
*frame5.x: 8
*frame5.y: 8
*frame5.resizable: "false"
*frame5.resizeRecursion: "up"

*scrolledWindowList7.class: scrolledWindow
*scrolledWindowList7.static: true
*scrolledWindowList7.name: scrolledWindowList7
*scrolledWindowList7.parent: frame5
*scrolledWindowList7.scrollingPolicy: "application_defined"
*scrolledWindowList7.visualPolicy: "variable"
*scrolledWindowList7.scrollBarDisplayPolicy: "static"
*scrolledWindowList7.shadowThickness: 0
*scrolledWindowList7.isCompound: "true"
*scrolledWindowList7.compoundIcon: "scrllist.xpm"
*scrolledWindowList7.compoundName: "scrolled_List"
*scrolledWindowList7.x: 62
*scrolledWindowList7.y: 12

*wStorageAccessList.class: scrolledList
*wStorageAccessList.static: true
*wStorageAccessList.name: wStorageAccessList
*wStorageAccessList.parent: scrolledWindowList7
*wStorageAccessList.width: 330
*wStorageAccessList.height: 140
*wStorageAccessList.browseSelectionCallback: {\
  XmListCallbackStruct *l;\
  DMAN_STORAGEACCESS *s;\
\
    l = (XmListCallbackStruct *)UxCallbackArg;\
    s = LST_Index(&storageAccessList, l->item_position);\
\
    displayStorageAccess(s);\
}

*label16.class: label
*label16.static: true
*label16.name: label16
*label16.parent: imageStorageAccessForm
*label16.isCompound: "true"
*label16.compoundIcon: "label.xpm"
*label16.compoundName: "label_"
*label16.x: 50
*label16.y: 190
*label16.width: 250
*label16.height: 40
*label16.labelString: "Application Title (CTN)"

*label17.class: label
*label17.static: true
*label17.name: label17
*label17.parent: imageStorageAccessForm
*label17.isCompound: "true"
*label17.compoundIcon: "label.xpm"
*label17.compoundName: "label_"
*label17.x: 50
*label17.y: 280
*label17.width: 250
*label17.height: 40
*label17.leftAttachment: "attach_opposite_widget"
*label17.leftOffset: 0
*label17.leftWidget: "label16"
*label17.topAttachment: "attach_widget"
*label17.topOffset: 5
*label17.topWidget: "label16"
*label17.labelString: "Database Key (Name)"

*label18.class: label
*label18.static: true
*label18.name: label18
*label18.parent: imageStorageAccessForm
*label18.isCompound: "true"
*label18.compoundIcon: "label.xpm"
*label18.compoundName: "label_"
*label18.x: 50
*label18.y: 330
*label18.width: 250
*label18.height: 40
*label18.leftAttachment: "attach_opposite_widget"
*label18.leftOffset: 0
*label18.leftWidget: "label16"
*label18.topAttachment: "attach_widget"
*label18.topOffset: 5
*label18.topWidget: "label17"
*label18.labelString: "Owner (AE Title - optional)"

*label19.class: label
*label19.static: true
*label19.name: label19
*label19.parent: imageStorageAccessForm
*label19.isCompound: "true"
*label19.compoundIcon: "label.xpm"
*label19.compoundName: "label_"
*label19.x: 50
*label19.y: 380
*label19.width: 250
*label19.height: 40
*label19.leftAttachment: "attach_opposite_widget"
*label19.leftOffset: 0
*label19.leftWidget: "label16"
*label19.topAttachment: "attach_widget"
*label19.topOffset: 5
*label19.topWidget: "label18"
*label19.labelString: "Group Name (optional)"

*label20.class: label
*label20.static: true
*label20.name: label20
*label20.parent: imageStorageAccessForm
*label20.isCompound: "true"
*label20.compoundIcon: "label.xpm"
*label20.compoundName: "label_"
*label20.x: 50
*label20.y: 450
*label20.width: 250
*label20.height: 40
*label20.topAttachment: "attach_widget"
*label20.topOffset: 30
*label20.topWidget: "label19"
*label20.leftAttachment: "attach_opposite_widget"
*label20.leftOffset: 0
*label20.leftWidget: "label16"
*label20.labelString: "Access"

*label21.class: label
*label21.static: true
*label21.name: label21
*label21.parent: imageStorageAccessForm
*label21.isCompound: "true"
*label21.compoundIcon: "label.xpm"
*label21.compoundName: "label_"
*label21.x: 50
*label21.y: 530
*label21.width: 250
*label21.height: 40
*label21.leftAttachment: "attach_opposite_widget"
*label21.leftOffset: 0
*label21.leftWidget: "label16"
*label21.topAttachment: "attach_widget"
*label21.topOffset: 35
*label21.topWidget: "label20"
*label21.labelString: "Comment"

*wApplicationTitleTxt.class: textField
*wApplicationTitleTxt.static: true
*wApplicationTitleTxt.name: wApplicationTitleTxt
*wApplicationTitleTxt.parent: imageStorageAccessForm
*wApplicationTitleTxt.width: 390
*wApplicationTitleTxt.isCompound: "true"
*wApplicationTitleTxt.compoundIcon: "textfield.xpm"
*wApplicationTitleTxt.compoundName: "text_Field"
*wApplicationTitleTxt.x: 400
*wApplicationTitleTxt.y: 210
*wApplicationTitleTxt.height: 40
*wApplicationTitleTxt.leftAttachment: "attach_widget"
*wApplicationTitleTxt.leftOffset: 15
*wApplicationTitleTxt.leftWidget: "label16"
*wApplicationTitleTxt.topAttachment: "attach_opposite_widget"
*wApplicationTitleTxt.topOffset: 0
*wApplicationTitleTxt.topWidget: "label16"

*wDatabaseKeyTxt.class: textField
*wDatabaseKeyTxt.static: true
*wDatabaseKeyTxt.name: wDatabaseKeyTxt
*wDatabaseKeyTxt.parent: imageStorageAccessForm
*wDatabaseKeyTxt.width: 390
*wDatabaseKeyTxt.isCompound: "true"
*wDatabaseKeyTxt.compoundIcon: "textfield.xpm"
*wDatabaseKeyTxt.compoundName: "text_Field"
*wDatabaseKeyTxt.x: 400
*wDatabaseKeyTxt.y: 250
*wDatabaseKeyTxt.height: 40
*wDatabaseKeyTxt.leftAttachment: "attach_widget"
*wDatabaseKeyTxt.leftOffset: 15
*wDatabaseKeyTxt.leftWidget: "label16"
*wDatabaseKeyTxt.topAttachment: "attach_opposite_widget"
*wDatabaseKeyTxt.topOffset: 0
*wDatabaseKeyTxt.topWidget: "label17"

*wOwnerTxt.class: textField
*wOwnerTxt.static: true
*wOwnerTxt.name: wOwnerTxt
*wOwnerTxt.parent: imageStorageAccessForm
*wOwnerTxt.width: 390
*wOwnerTxt.isCompound: "true"
*wOwnerTxt.compoundIcon: "textfield.xpm"
*wOwnerTxt.compoundName: "text_Field"
*wOwnerTxt.x: 400
*wOwnerTxt.y: 290
*wOwnerTxt.height: 40
*wOwnerTxt.leftAttachment: "attach_widget"
*wOwnerTxt.leftOffset: 15
*wOwnerTxt.leftWidget: "label16"
*wOwnerTxt.topAttachment: "attach_opposite_widget"
*wOwnerTxt.topOffset: 0
*wOwnerTxt.topWidget: "label18"

*wGroupNameTxt.class: textField
*wGroupNameTxt.static: true
*wGroupNameTxt.name: wGroupNameTxt
*wGroupNameTxt.parent: imageStorageAccessForm
*wGroupNameTxt.width: 390
*wGroupNameTxt.isCompound: "true"
*wGroupNameTxt.compoundIcon: "textfield.xpm"
*wGroupNameTxt.compoundName: "text_Field"
*wGroupNameTxt.x: 400
*wGroupNameTxt.y: 340
*wGroupNameTxt.height: 40
*wGroupNameTxt.leftAttachment: "attach_widget"
*wGroupNameTxt.leftOffset: 15
*wGroupNameTxt.leftWidget: "label16"
*wGroupNameTxt.topAttachment: "attach_opposite_widget"
*wGroupNameTxt.topOffset: 0
*wGroupNameTxt.topWidget: "label19"

*wCommentTxt.class: textField
*wCommentTxt.static: true
*wCommentTxt.name: wCommentTxt
*wCommentTxt.parent: imageStorageAccessForm
*wCommentTxt.width: 390
*wCommentTxt.isCompound: "true"
*wCommentTxt.compoundIcon: "textfield.xpm"
*wCommentTxt.compoundName: "text_Field"
*wCommentTxt.x: 400
*wCommentTxt.y: 480
*wCommentTxt.height: 40
*wCommentTxt.leftAttachment: "attach_widget"
*wCommentTxt.leftOffset: 15
*wCommentTxt.leftWidget: "label16"
*wCommentTxt.topAttachment: "attach_opposite_widget"
*wCommentTxt.topOffset: 0
*wCommentTxt.topWidget: "label21"

*pushButton15.class: pushButton
*pushButton15.static: true
*pushButton15.name: pushButton15
*pushButton15.parent: imageStorageAccessForm
*pushButton15.x: 40
*pushButton15.y: 540
*pushButton15.width: 180
*pushButton15.height: 50
*pushButton15.labelString: "Add"
*pushButton15.activateCallback: {\
  CONDITION cond;\
  DMAN_STORAGEACCESS s;\
\
    cond = readStorageAccess(&s);\
    if (cond != 1)\
	return;\
\
    cond = DMAN_Insert(&dmanHandle, (DMAN_GENERICRECORD *)&s);\
    if (cond != DMAN_NORMAL)\
	COND_DumpConditions();\
\
    loadImageStorageAccessList();\
}

*pushButton16.class: pushButton
*pushButton16.static: true
*pushButton16.name: pushButton16
*pushButton16.parent: imageStorageAccessForm
*pushButton16.x: 310
*pushButton16.y: 540
*pushButton16.width: 180
*pushButton16.height: 50
*pushButton16.labelString: "Update"
*pushButton16.sensitive: "false"

*pushButton17.class: pushButton
*pushButton17.static: true
*pushButton17.name: pushButton17
*pushButton17.parent: imageStorageAccessForm
*pushButton17.x: 580
*pushButton17.y: 540
*pushButton17.width: 180
*pushButton17.height: 50
*pushButton17.labelString: "Delete"
*pushButton17.activateCallback: {\
  CONDITION cond;\
  DMAN_STORAGEACCESS s;\
\
    cond = readStorageAccess(&s);\
    if (cond != 1)\
	return;\
\
    cond = DMAN_Delete(&dmanHandle, (DMAN_GENERICRECORD *)&s);\
    if (cond != DMAN_NORMAL)\
	COND_DumpConditions();\
\
    loadImageStorageAccessList();\
}

