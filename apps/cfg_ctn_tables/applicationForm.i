! UIMX ascii 2.5 key: 739                                                       

*applicationForm.class: form
*applicationForm.gbldecl: /*\
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
** Module Name(s):	create_applicationForm\
**			loadApplicationList\
** Author, Date:	Steve Moore, Summer 1994\
** Intent:		This file provides the user interface\
**			for defining and modifying application\
**			entities.  The public entry points are\
**			call to create the widget hierarchy\
**			and to populate the list application\
**			entities.  All other functions are\
**			local to this specific interface.\
** Last Update:         $Author: smm $, $Date: 1995-04-10 13:19:28 $\
** Source File:         $RCSfile: applicationForm.i,v $\
** Revision:            $Revision: 1.6 $\
** Status:              $State: Exp $\
*/\
 \
static char rcsid[] = "$Revision: 1.6 $ $RCSfile: applicationForm.i,v $";\
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
static LST_HEAD *applicationList = NULL;
*applicationForm.ispecdecl:
*applicationForm.funcdecl: swidget create_applicationForm(swidget UxParent)
*applicationForm.funcname: create_applicationForm
*applicationForm.funcdef: "swidget", "<create_applicationForm>(%)"
*applicationForm.argdecl: swidget UxParent;
*applicationForm.arglist: UxParent
*applicationForm.arglist.UxParent: "swidget", "%UxParent%"
*applicationForm.icode:
*applicationForm.fcode: return(rtrn);\

*applicationForm.auxdecl: void loadApplicationList()\
{\
  CONDITION\
    cond;\
  DMAN_APPLICATIONENTITY\
    ae,\
    criteria;\
  char\
    b[256];\
\
    if (applicationList == NULL)\
	applicationList = LST_Create();\
\
    if (applicationList == NULL)\
	return;\
\
    memset(&ae, 0, sizeof(ae));\
    ae.Type = DMAN_K_APPLICATIONENTITY;\
    criteria.Type = DMAN_K_APPLICATIONENTITY;\
    criteria.Flag = 0;\
    cond = DMAN_Select(&dmanHandle, (DMAN_GENERICRECORD *)&ae,\
	(DMAN_GENERICRECORD *)&criteria,\
	applicationList, NULL, NULL, NULL);\
    if (cond != DMAN_NORMAL)\
	return;\
\
    (void) LST_Sort(&applicationList, sizeof(ae), compareAE);\
    MUT_LoadList(wApplicationList, applicationList, formatAE, b);\
}\
\
static void displayAE(DMAN_APPLICATIONENTITY *ae)\
{\
  char\
    b[20];\
\
    XmTextSetString(wApplicationTitleTxt, ae->Title);\
    XmTextSetString(wNodeTxt, ae->Node);\
    XmTextSetString(wOrganizationTxt, ae->Organization);\
    if (ae->Flag & DMAN_K_APPLICATION_COMMENT)\
	XmTextSetString(wCommentTxt, ae->Comment);\
    else\
	XmTextSetString(wCommentTxt, "");\
\
    if (!ae->Flag & DMAN_K_APPLICATION_PORT)\
	b[0] = '\0';\
    else\
	sprintf(b, "%-d", ae->Port);\
    XmTextSetString(wPortTxt, b);\
}\
\
static CONDITION readAE(DMAN_APPLICATIONENTITY *ae)\
{\
  CONDITION\
    cond;\
  CTNBOOLEAN\
    nullFlag;\
\
    memset(ae, 0, sizeof(*ae));\
    ae->Type = DMAN_K_APPLICATIONENTITY;\
    ae->Flag = 0;\
\
    cond = MUT_ScanWidget(wApplicationTitleTxt, MUT_TEXT, &nullFlag, ae->Title);\
    if (cond != MUT_NORMAL || nullFlag)\
	return 0;\
    ae->Flag |= DMAN_K_APPLICATION_TITLE;\
\
    cond = MUT_ScanWidget(wNodeTxt, MUT_TEXT, &nullFlag, ae->Node);\
    if (cond != MUT_NORMAL || nullFlag)\
	return 0;\
    ae->Flag |= DMAN_K_APPLICATION_NODE;\
\
    cond = MUT_ScanWidget(wOrganizationTxt, MUT_TEXT, &nullFlag,\
	ae->Organization);\
    if (cond != MUT_NORMAL || nullFlag)\
	return 0;\
    ae->Flag |= DMAN_K_APPLICATION_ORGANIZATION;\
\
    cond = MUT_ScanWidget(wPortTxt, MUT_INT, &nullFlag, &ae->Port);\
    if (cond != MUT_NORMAL)\
	return 0;\
    ae->Flag |= DMAN_K_APPLICATION_PORT;\
\
    cond = MUT_ScanWidget(wCommentTxt, MUT_TEXT, &nullFlag, ae->Comment);\
    if (cond != MUT_NORMAL)\
	return 0;\
    ae->Flag |= DMAN_K_APPLICATION_COMMENT;\
\
    return 1;\
}
*applicationForm.static: true
*applicationForm.name: applicationForm
*applicationForm.parent: NO_PARENT
*applicationForm.parentExpression: UxParent
*applicationForm.defaultShell: topLevelShell
*applicationForm.width: 746
*applicationForm.height: 549
*applicationForm.resizePolicy: "resize_none"
*applicationForm.isCompound: "true"
*applicationForm.compoundIcon: "form.xpm"
*applicationForm.compoundName: "form_"
*applicationForm.x: 74
*applicationForm.y: 139
*applicationForm.unitType: "pixels"

*label1.class: label
*label1.static: true
*label1.name: label1
*label1.parent: applicationForm
*label1.isCompound: "true"
*label1.compoundIcon: "label.xpm"
*label1.compoundName: "label_"
*label1.x: 50
*label1.y: 200
*label1.width: 200
*label1.height: 40
*label1.labelString: "Application Title"

*label2.class: label
*label2.static: true
*label2.name: label2
*label2.parent: applicationForm
*label2.isCompound: "true"
*label2.compoundIcon: "label.xpm"
*label2.compoundName: "label_"
*label2.x: 50
*label2.y: 240
*label2.width: 200
*label2.height: 40
*label2.leftAttachment: "attach_opposite_widget"
*label2.leftOffset: 0
*label2.leftWidget: "label1"
*label2.topAttachment: "attach_widget"
*label2.topOffset: 15
*label2.topWidget: "label1"
*label2.labelString: "Node"

*wApplicationTitleTxt.class: textField
*wApplicationTitleTxt.static: true
*wApplicationTitleTxt.name: wApplicationTitleTxt
*wApplicationTitleTxt.parent: applicationForm
*wApplicationTitleTxt.width: 370
*wApplicationTitleTxt.isCompound: "true"
*wApplicationTitleTxt.compoundIcon: "textfield.xpm"
*wApplicationTitleTxt.compoundName: "text_Field"
*wApplicationTitleTxt.x: 280
*wApplicationTitleTxt.y: 210
*wApplicationTitleTxt.height: 40
*wApplicationTitleTxt.leftAttachment: "attach_widget"
*wApplicationTitleTxt.leftOffset: 10
*wApplicationTitleTxt.leftWidget: "label1"
*wApplicationTitleTxt.topAttachment: "attach_opposite_widget"
*wApplicationTitleTxt.topOffset: 0
*wApplicationTitleTxt.topWidget: "label1"

*wNodeTxt.class: textField
*wNodeTxt.static: true
*wNodeTxt.name: wNodeTxt
*wNodeTxt.parent: applicationForm
*wNodeTxt.width: 370
*wNodeTxt.isCompound: "true"
*wNodeTxt.compoundIcon: "textfield.xpm"
*wNodeTxt.compoundName: "text_Field"
*wNodeTxt.x: 280
*wNodeTxt.y: 250
*wNodeTxt.height: 40
*wNodeTxt.leftAttachment: "attach_widget"
*wNodeTxt.leftOffset: 10
*wNodeTxt.leftWidget: "label1"
*wNodeTxt.topAttachment: "attach_opposite_widget"
*wNodeTxt.topOffset: 0
*wNodeTxt.topWidget: "label2"

*pushButton1.class: pushButton
*pushButton1.static: true
*pushButton1.name: pushButton1
*pushButton1.parent: applicationForm
*pushButton1.isCompound: "true"
*pushButton1.compoundIcon: "push.xpm"
*pushButton1.compoundName: "push_Button"
*pushButton1.x: 50
*pushButton1.y: 480
*pushButton1.width: 160
*pushButton1.height: 50
*pushButton1.labelString: "Add"
*pushButton1.activateCallback: {\
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
}

*pushButton2.class: pushButton
*pushButton2.static: true
*pushButton2.name: pushButton2
*pushButton2.parent: applicationForm
*pushButton2.isCompound: "true"
*pushButton2.compoundIcon: "push.xpm"
*pushButton2.compoundName: "push_Button"
*pushButton2.x: 300
*pushButton2.y: 480
*pushButton2.width: 160
*pushButton2.height: 50
*pushButton2.labelString: "Update"
*pushButton2.sensitive: "false"

*pushButton3.class: pushButton
*pushButton3.static: true
*pushButton3.name: pushButton3
*pushButton3.parent: applicationForm
*pushButton3.isCompound: "true"
*pushButton3.compoundIcon: "push.xpm"
*pushButton3.compoundName: "push_Button"
*pushButton3.x: 530
*pushButton3.y: 480
*pushButton3.width: 160
*pushButton3.height: 50
*pushButton3.labelString: "Delete"
*pushButton3.activateCallback: {\
  DMAN_APPLICATIONENTITY\
    ae;\
  CONDITION\
    cond;\
\
    cond = readAE(&ae);\
    if (cond != 1)\
	return;\
\
    ae.Flag = DMAN_K_APPLICATION_TITLE;\
    cond = DMAN_Delete(&dmanHandle, (DMAN_GENERICRECORD *)&ae);\
    if (cond != DMAN_NORMAL)\
	COND_DumpConditions();\
\
    loadApplicationList();\
}

*frame1.class: frame
*frame1.static: true
*frame1.name: frame1
*frame1.parent: applicationForm
*frame1.width: 710
*frame1.height: 160
*frame1.isCompound: "true"
*frame1.compoundIcon: "frame.xpm"
*frame1.compoundName: "frame_"
*frame1.x: 30
*frame1.y: 20
*frame1.resizable: "false"
*frame1.resizeRecursion: "up"

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
*scrolledWindowList1.x: 90
*scrolledWindowList1.y: 60

*wApplicationList.class: scrolledList
*wApplicationList.static: true
*wApplicationList.name: wApplicationList
*wApplicationList.parent: scrolledWindowList1
*wApplicationList.width: 250
*wApplicationList.height: 50
*wApplicationList.browseSelectionCallback: {\
  XmListCallbackStruct *l;\
  DMAN_APPLICATIONENTITY *ae;\
\
    l = (XmListCallbackStruct *)UxCallbackArg;\
    ae = LST_Index(&applicationList, l->item_position);\
\
    displayAE(ae);\
}

*label29.class: label
*label29.static: true
*label29.name: label29
*label29.parent: applicationForm
*label29.isCompound: "true"
*label29.compoundIcon: "label.xpm"
*label29.compoundName: "label_"
*label29.x: 50
*label29.y: 310
*label29.width: 200
*label29.height: 40
*label29.labelString: "Organization"
*label29.leftAttachment: "attach_opposite_widget"
*label29.leftOffset: 0
*label29.leftWidget: "label1"
*label29.topAttachment: "attach_widget"
*label29.topOffset: 15
*label29.topWidget: "label2"

*label3.class: label
*label3.static: true
*label3.name: label3
*label3.parent: applicationForm
*label3.isCompound: "true"
*label3.compoundIcon: "label.xpm"
*label3.compoundName: "label_"
*label3.x: 50
*label3.y: 270
*label3.width: 200
*label3.height: 40
*label3.leftAttachment: "attach_opposite_widget"
*label3.leftOffset: 0
*label3.leftWidget: "label1"
*label3.topAttachment: "attach_widget"
*label3.topOffset: 15
*label3.topWidget: "label29"
*label3.labelString: "Port (Optional)"

*label4.class: label
*label4.static: true
*label4.name: label4
*label4.parent: applicationForm
*label4.isCompound: "true"
*label4.compoundIcon: "label.xpm"
*label4.compoundName: "label_"
*label4.x: 50
*label4.y: 310
*label4.width: 200
*label4.height: 40
*label4.leftAttachment: "attach_opposite_widget"
*label4.leftOffset: 0
*label4.leftWidget: "label1"
*label4.topAttachment: "attach_widget"
*label4.topOffset: 15
*label4.topWidget: "label3"
*label4.labelString: "Comment (Optional)"

*wPortTxt.class: textField
*wPortTxt.static: true
*wPortTxt.name: wPortTxt
*wPortTxt.parent: applicationForm
*wPortTxt.width: 370
*wPortTxt.isCompound: "true"
*wPortTxt.compoundIcon: "textfield.xpm"
*wPortTxt.compoundName: "text_Field"
*wPortTxt.x: 280
*wPortTxt.y: 290
*wPortTxt.height: 40
*wPortTxt.leftAttachment: "attach_widget"
*wPortTxt.leftOffset: 10
*wPortTxt.leftWidget: "label1"
*wPortTxt.topAttachment: "attach_opposite_widget"
*wPortTxt.topOffset: 0
*wPortTxt.topWidget: "label3"

*wCommentTxt.class: textField
*wCommentTxt.static: true
*wCommentTxt.name: wCommentTxt
*wCommentTxt.parent: applicationForm
*wCommentTxt.width: 370
*wCommentTxt.isCompound: "true"
*wCommentTxt.compoundIcon: "textfield.xpm"
*wCommentTxt.compoundName: "text_Field"
*wCommentTxt.x: 280
*wCommentTxt.y: 330
*wCommentTxt.height: 40
*wCommentTxt.leftAttachment: "attach_widget"
*wCommentTxt.leftOffset: 10
*wCommentTxt.leftWidget: "label1"
*wCommentTxt.topAttachment: "attach_opposite_widget"
*wCommentTxt.topOffset: 0
*wCommentTxt.topWidget: "label4"

*wOrganizationTxt.class: textField
*wOrganizationTxt.static: true
*wOrganizationTxt.name: wOrganizationTxt
*wOrganizationTxt.parent: applicationForm
*wOrganizationTxt.width: 370
*wOrganizationTxt.isCompound: "true"
*wOrganizationTxt.compoundIcon: "textfield.xpm"
*wOrganizationTxt.compoundName: "text_Field"
*wOrganizationTxt.x: 240
*wOrganizationTxt.y: 300
*wOrganizationTxt.height: 40
*wOrganizationTxt.leftAttachment: "attach_widget"
*wOrganizationTxt.leftOffset: 10
*wOrganizationTxt.leftWidget: "label1"
*wOrganizationTxt.topAttachment: "attach_opposite_widget"
*wOrganizationTxt.topOffset: 0
*wOrganizationTxt.topWidget: "label29"

