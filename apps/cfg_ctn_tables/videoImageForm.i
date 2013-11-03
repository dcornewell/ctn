! UIMX ascii 2.5 key: 9591                                                      

*videoImageForm.class: form
*videoImageForm.gbldecl: /*\
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
** Module Name(s):	create_videoImagelForm\
** 			loadVideoImageList\
** Author, Date:	Steve Moore, Summer 1994\
** Intent:		This file provides the user interface\
** 			for defining and modifying entries in\
**			the Video Image Control table.  This table\
**			is used to define destinations for video\
**			images that are captured with the Photo ID\
**			system.\
**			The public entry points are\
**			call to create the widget hierarchy\
**			and to populate the list application\
**			entities.  All other functions are\
** 			local to this specific interface.\
** Last Update:		$Author: smm $, $Date: 1995-04-10 13:20:11 $\
** Source File:		$RCSfile: videoImageForm.i,v $\
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
static LST_HEAD *videoImageList = NULL;
*videoImageForm.ispecdecl:
*videoImageForm.funcdecl: swidget create_videoImageForm(swidget UxParent)
*videoImageForm.funcname: create_videoImageForm
*videoImageForm.funcdef: "swidget", "<create_videoImageForm>(%)"
*videoImageForm.argdecl: swidget UxParent;
*videoImageForm.arglist: UxParent
*videoImageForm.arglist.UxParent: "swidget", "%UxParent%"
*videoImageForm.icode:
*videoImageForm.fcode: return(rtrn);\

*videoImageForm.auxdecl: void loadVideoImageList()\
{\
  CONDITION cond;\
  DMAN_VIDEOIMAGEDEST a, criteria;\
  char b[256];\
\
    if (videoImageList == NULL)\
	videoImageList = LST_Create();\
    if (videoImageList == NULL)\
	return;\
\
    memset(&a, 0, sizeof(a));\
    a.Type = DMAN_K_VIDEOIMAGEDEST;\
    memset(&criteria, 0, sizeof(criteria));\
    criteria.Type = DMAN_K_VIDEOIMAGEDEST;\
\
    cond = DMAN_Select(&dmanHandle,\
	(DMAN_GENERICRECORD *) &a,\
	(DMAN_GENERICRECORD *) &criteria,\
	videoImageList, NULL, NULL, NULL);\
    if (cond != DMAN_NORMAL) {\
	COND_DumpConditions();\
	return;\
    }\
    MUT_LoadList(wVideoImageList, videoImageList,\
	formatVideoImage, b);\
}\
\
static void displayVideoImage(DMAN_VIDEOIMAGEDEST *d)\
{\
    if (d == NULL)\
	return;\
\
    XmTextSetString(wRequestingTitleTxt, d->RequestingTitle);\
    XmTextSetString(wRespondingTitleTxt, d->RespondingTitle);\
    XmTextSetString(wImageTypeTxt, d->ImageType);\
}\
\
static CONDITION\
readVideoImage(DMAN_VIDEOIMAGEDEST *d)\
{\
  CONDITION cond;\
  CTNBOOLEAN nullFlag;\
\
    memset(d, 0, sizeof(*d));\
    d->Type = DMAN_K_VIDEOIMAGEDEST;\
\
    cond = MUT_ScanWidget(wRequestingTitleTxt, MUT_TEXT,\
	&nullFlag, d->RequestingTitle);\
    if (cond != MUT_NORMAL || nullFlag)\
	return 0;\
    d->Flag |= DMAN_K_VIDEOIMAGE_REQUESTING;\
\
    cond = MUT_ScanWidget(wRespondingTitleTxt, MUT_TEXT,\
	&nullFlag, d->RespondingTitle);\
    if (cond != MUT_NORMAL || nullFlag)\
	return 0;\
    d->Flag |= DMAN_K_VIDEOIMAGE_RESPONDING;\
\
    cond = MUT_ScanWidget(wImageTypeTxt, MUT_TEXT,\
	&nullFlag, d->ImageType);\
    if (cond != MUT_NORMAL || nullFlag)\
	return 0;\
    d->Flag |= DMAN_K_VIDEOIMAGE_IMAGETYPE;\
\
    return 1;\
}
*videoImageForm.static: true
*videoImageForm.name: videoImageForm
*videoImageForm.parent: NO_PARENT
*videoImageForm.parentExpression: UxParent
*videoImageForm.defaultShell: topLevelShell
*videoImageForm.width: 520
*videoImageForm.height: 351
*videoImageForm.resizePolicy: "resize_none"
*videoImageForm.isCompound: "true"
*videoImageForm.compoundIcon: "form.xpm"
*videoImageForm.compoundName: "form_"
*videoImageForm.x: 335
*videoImageForm.y: 159
*videoImageForm.unitType: "pixels"

*frame6.class: frame
*frame6.static: true
*frame6.name: frame6
*frame6.parent: videoImageForm
*frame6.width: 490
*frame6.height: 110
*frame6.isCompound: "true"
*frame6.compoundIcon: "frame.xpm"
*frame6.compoundName: "frame_"
*frame6.x: 20
*frame6.y: 20
*frame6.resizable: "false"
*frame6.resizeRecursion: "up"

*scrolledWindowList9.class: scrolledWindow
*scrolledWindowList9.static: true
*scrolledWindowList9.name: scrolledWindowList9
*scrolledWindowList9.parent: frame6
*scrolledWindowList9.scrollingPolicy: "application_defined"
*scrolledWindowList9.visualPolicy: "variable"
*scrolledWindowList9.scrollBarDisplayPolicy: "static"
*scrolledWindowList9.shadowThickness: 0
*scrolledWindowList9.isCompound: "true"
*scrolledWindowList9.compoundIcon: "scrllist.xpm"
*scrolledWindowList9.compoundName: "scrolled_List"
*scrolledWindowList9.x: 70
*scrolledWindowList9.y: 20

*wVideoImageList.class: scrolledList
*wVideoImageList.static: true
*wVideoImageList.name: wVideoImageList
*wVideoImageList.parent: scrolledWindowList9
*wVideoImageList.width: 190
*wVideoImageList.height: 30
*wVideoImageList.browseSelectionCallback: {\
  XmListCallbackStruct *l;\
  DMAN_VIDEOIMAGEDEST *video;\
\
    l = (XmListCallbackStruct *) UxCallbackArg;\
\
    video = LST_Index(&videoImageList, l->item_position);\
    displayVideoImage(video);\
}

*label26.class: label
*label26.static: true
*label26.name: label26
*label26.parent: videoImageForm
*label26.isCompound: "true"
*label26.compoundIcon: "label.xpm"
*label26.compoundName: "label_"
*label26.x: 30
*label26.y: 150
*label26.width: 130
*label26.height: 30
*label26.labelString: "Requesting Title"

*label27.class: label
*label27.static: true
*label27.name: label27
*label27.parent: videoImageForm
*label27.isCompound: "true"
*label27.compoundIcon: "label.xpm"
*label27.compoundName: "label_"
*label27.x: 30
*label27.y: 190
*label27.width: 130
*label27.height: 30
*label27.leftAttachment: "attach_opposite_widget"
*label27.leftOffset: 0
*label27.leftWidget: "label26"
*label27.topAttachment: "attach_widget"
*label27.topOffset: 20
*label27.topWidget: "label26"
*label27.labelString: "Responding Title"

*label28.class: label
*label28.static: true
*label28.name: label28
*label28.parent: videoImageForm
*label28.isCompound: "true"
*label28.compoundIcon: "label.xpm"
*label28.compoundName: "label_"
*label28.x: 30
*label28.y: 240
*label28.width: 130
*label28.height: 30
*label28.leftAttachment: "attach_opposite_widget"
*label28.leftOffset: 0
*label28.leftWidget: "label26"
*label28.topAttachment: "attach_widget"
*label28.topOffset: 20
*label28.topWidget: "label27"
*label28.labelString: "Image Type"

*wRequestingTitleTxt.class: textField
*wRequestingTitleTxt.static: true
*wRequestingTitleTxt.name: wRequestingTitleTxt
*wRequestingTitleTxt.parent: videoImageForm
*wRequestingTitleTxt.width: 320
*wRequestingTitleTxt.isCompound: "true"
*wRequestingTitleTxt.compoundIcon: "textfield.xpm"
*wRequestingTitleTxt.compoundName: "text_Field"
*wRequestingTitleTxt.x: 190
*wRequestingTitleTxt.y: 150
*wRequestingTitleTxt.height: 40
*wRequestingTitleTxt.leftAttachment: "attach_widget"
*wRequestingTitleTxt.leftOffset: 15
*wRequestingTitleTxt.leftWidget: "label26"
*wRequestingTitleTxt.topAttachment: "attach_opposite_widget"
*wRequestingTitleTxt.topOffset: 0
*wRequestingTitleTxt.topWidget: "label26"

*wRespondingTitleTxt.class: textField
*wRespondingTitleTxt.static: true
*wRespondingTitleTxt.name: wRespondingTitleTxt
*wRespondingTitleTxt.parent: videoImageForm
*wRespondingTitleTxt.width: 320
*wRespondingTitleTxt.isCompound: "true"
*wRespondingTitleTxt.compoundIcon: "textfield.xpm"
*wRespondingTitleTxt.compoundName: "text_Field"
*wRespondingTitleTxt.x: 190
*wRespondingTitleTxt.y: 190
*wRespondingTitleTxt.height: 40
*wRespondingTitleTxt.leftAttachment: "attach_widget"
*wRespondingTitleTxt.leftOffset: 15
*wRespondingTitleTxt.leftWidget: "label26"
*wRespondingTitleTxt.topAttachment: "attach_opposite_widget"
*wRespondingTitleTxt.topOffset: 0
*wRespondingTitleTxt.topWidget: "label27"

*wImageTypeTxt.class: textField
*wImageTypeTxt.static: true
*wImageTypeTxt.name: wImageTypeTxt
*wImageTypeTxt.parent: videoImageForm
*wImageTypeTxt.width: 320
*wImageTypeTxt.isCompound: "true"
*wImageTypeTxt.compoundIcon: "textfield.xpm"
*wImageTypeTxt.compoundName: "text_Field"
*wImageTypeTxt.x: 190
*wImageTypeTxt.y: 230
*wImageTypeTxt.height: 40
*wImageTypeTxt.leftAttachment: "attach_widget"
*wImageTypeTxt.leftOffset: 15
*wImageTypeTxt.leftWidget: "label26"
*wImageTypeTxt.topAttachment: "attach_opposite_widget"
*wImageTypeTxt.topOffset: 0
*wImageTypeTxt.topWidget: "label28"

*pushButton21.class: pushButton
*pushButton21.static: true
*pushButton21.name: pushButton21
*pushButton21.parent: videoImageForm
*pushButton21.isCompound: "true"
*pushButton21.compoundIcon: "push.xpm"
*pushButton21.compoundName: "push_Button"
*pushButton21.x: 60
*pushButton21.y: 300
*pushButton21.width: 140
*pushButton21.height: 40
*pushButton21.labelString: "Add"
*pushButton21.activateCallback: {\
  DMAN_VIDEOIMAGEDEST v;\
  CONDITION cond;\
\
    cond = readVideoImage(&v);\
    if (cond != 1)\
	return;\
\
    cond = DMAN_Insert(&dmanHandle, (DMAN_GENERICRECORD *) &v);\
    if (cond != DMAN_NORMAL)\
	COND_DumpConditions();\
\
    loadVideoImageList();\
}

*pushButton22.class: pushButton
*pushButton22.static: true
*pushButton22.name: pushButton22
*pushButton22.parent: videoImageForm
*pushButton22.isCompound: "true"
*pushButton22.compoundIcon: "push.xpm"
*pushButton22.compoundName: "push_Button"
*pushButton22.x: 320
*pushButton22.y: 300
*pushButton22.width: 140
*pushButton22.height: 40
*pushButton22.labelString: "Delete"
*pushButton22.activateCallback: {\
  DMAN_VIDEOIMAGEDEST v;\
  CONDITION cond;\
\
    cond = readVideoImage(&v);\
    if (cond != 1)\
	return;\
\
    cond = DMAN_Delete(&dmanHandle, (DMAN_GENERICRECORD *) & v);\
    if (cond != DMAN_NORMAL)\
	COND_DumpConditions();\
\
    loadVideoImageList();\
}

