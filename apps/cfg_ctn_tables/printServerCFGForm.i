! UIMX ascii 2.5 key: 2829                                                      

*printServerCFGForm.class: form
*printServerCFGForm.gbldecl: /*\
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
** Module Name(s):	create_printServerCFGForm\
**			loadPrintServerCFGList\
** Author, Date:	Steve Moore, Summer 1994\
** Intent:		This file provides the user interface\
**			for defining and modifying the print server\
**			configuration table.  The public entry points are\
**			call to create the widget hierarchy\
**			and to populate the list of print configuration\
**			entries.  All other functions are\
** 			local to this specific interface.\
** Last Update:         $Author: smm $, $Date: 1995-04-10 13:20:00 $\
** Source File:         $RCSfile: printServerCFGForm.i,v $\
** Revision:            $Revision: 1.4 $\
** Status:              $State: Exp $\
*/\
\
\
#include <stdio.h>\
\
#include "dicom.h"\
#include "condition.h"\
#include "lst.h"\
#include "manage.h"\
#include "mut.h"\
\
#include "format.h"\
\
extern DMAN_HANDLE *dmanHandle;\
static LST_HEAD *printServerCFGList = NULL;\
\
void loadPrintServerCFGList();\
static void displayPrintServerCFG(DMAN_PRINTSERVERCFG *p);\
static CONDITION readPrintServerCFG(DMAN_PRINTSERVERCFG *p);
*printServerCFGForm.ispecdecl:
*printServerCFGForm.funcdecl: swidget create_printServerCFGForm(swidget UxParent)
*printServerCFGForm.funcname: create_printServerCFGForm
*printServerCFGForm.funcdef: "swidget", "<create_printServerCFGForm>(%)"
*printServerCFGForm.argdecl: swidget UxParent;
*printServerCFGForm.arglist: UxParent
*printServerCFGForm.arglist.UxParent: "swidget", "%UxParent%"
*printServerCFGForm.icode:
*printServerCFGForm.fcode: return(rtrn);\

*printServerCFGForm.auxdecl: void loadPrintServerCFGList()\
{\
  CONDITION\
    cond;\
  DMAN_PRINTSERVERCFG\
    p,\
    criteria;\
  char\
    b[128];\
\
    if (printServerCFGList == NULL)\
	printServerCFGList = LST_Create();\
    if (printServerCFGList == NULL)\
	return;\
\
    memset(&p, 0, sizeof(p));\
    p.Type = DMAN_K_PRINTSERVERCFG;\
    criteria.Type = DMAN_K_PRINTSERVERCFG;\
    criteria.Flag = 0;\
    cond = DMAN_Select(&dmanHandle, (DMAN_GENERICRECORD *)&p,\
	(DMAN_GENERICRECORD *)&criteria,\
	printServerCFGList, NULL, NULL, NULL);\
    if (cond != DMAN_NORMAL) {\
	COND_DumpConditions();\
	return;\
    }\
    MUT_LoadList(wPrintServerCFGList, printServerCFGList,\
	formatPrintServerCFG, b);\
}\
\
static void displayPrintServerCFG(DMAN_PRINTSERVERCFG *p)\
{\
  char\
    buf[16];\
\
    if (p == NULL)\
	return;\
    if (p->Type != DMAN_K_PRINTSERVERCFG)\
	return;\
\
    XmTextSetString(wRequestingTxt, p->RequestingTitle);\
    XmTextSetString(wRespondingTxt, p->RespondingTitle);\
    (void)sprintf(buf, "%-6d", p->GQId);\
    XmTextSetString(wQueueIDTxt, buf);\
}\
\
static CONDITION readPrintServerCFG(DMAN_PRINTSERVERCFG *p)\
{\
  CONDITION\
    cond;\
  CTNBOOLEAN\
    nullFlag;\
\
    memset(p, 0, sizeof(*p));\
    p->Type = DMAN_K_PRINTSERVERCFG;\
\
    cond = MUT_ScanWidget(wRequestingTxt, MUT_TEXT, &nullFlag,\
	p->RequestingTitle);\
    if (cond != MUT_NORMAL || nullFlag)\
	return 0;\
    p->Flag |= DMAN_K_PRINTSERVER_REQUESTING;\
\
    cond = MUT_ScanWidget(wRespondingTxt, MUT_TEXT, &nullFlag,\
	p->RespondingTitle);\
    if (cond != MUT_NORMAL ||  nullFlag)\
	return 0;\
    p->Flag |= DMAN_K_PRINTSERVER_RESPONDING;\
\
    cond = MUT_ScanWidget(wQueueIDTxt, MUT_INT, &nullFlag,\
	&p->GQId);\
    if (cond != MUT_NORMAL || nullFlag)\
	return 0;\
    p->Flag |= DMAN_K_PRINTSERVER_GQID;\
\
    return 1;\
}
*printServerCFGForm.static: true
*printServerCFGForm.name: printServerCFGForm
*printServerCFGForm.parent: NO_PARENT
*printServerCFGForm.parentExpression: UxParent
*printServerCFGForm.defaultShell: topLevelShell
*printServerCFGForm.width: 570
*printServerCFGForm.height: 460
*printServerCFGForm.resizePolicy: "resize_none"
*printServerCFGForm.isCompound: "true"
*printServerCFGForm.compoundIcon: "form.xpm"
*printServerCFGForm.compoundName: "form_"
*printServerCFGForm.x: 240
*printServerCFGForm.y: 360
*printServerCFGForm.unitType: "pixels"

*label7.class: label
*label7.static: true
*label7.name: label7
*label7.parent: printServerCFGForm
*label7.isCompound: "true"
*label7.compoundIcon: "label.xpm"
*label7.compoundName: "label_"
*label7.x: 10
*label7.y: 140
*label7.width: 220
*label7.height: 30
*label7.labelString: "Requesting (external)"

*label8.class: label
*label8.static: true
*label8.name: label8
*label8.parent: printServerCFGForm
*label8.isCompound: "true"
*label8.compoundIcon: "label.xpm"
*label8.compoundName: "label_"
*label8.x: 10
*label8.y: 170
*label8.width: 220
*label8.height: 30
*label8.leftAttachment: "attach_opposite_widget"
*label8.leftOffset: 0
*label8.leftWidget: "label7"
*label8.topAttachment: "attach_widget"
*label8.topOffset: 15
*label8.topWidget: "label7"
*label8.labelString: "Responding (CTN)"

*label9.class: label
*label9.static: true
*label9.name: label9
*label9.parent: printServerCFGForm
*label9.isCompound: "true"
*label9.compoundIcon: "label.xpm"
*label9.compoundName: "label_"
*label9.x: 10
*label9.y: 220
*label9.width: 220
*label9.height: 30
*label9.leftAttachment: "attach_opposite_widget"
*label9.leftOffset: 0
*label9.leftWidget: "label7"
*label9.topAttachment: "attach_widget"
*label9.topOffset: 15
*label9.topWidget: "label8"
*label9.labelString: "Queue ID (a number)"

*wRequestingTxt.class: textField
*wRequestingTxt.static: true
*wRequestingTxt.name: wRequestingTxt
*wRequestingTxt.parent: printServerCFGForm
*wRequestingTxt.width: 290
*wRequestingTxt.isCompound: "true"
*wRequestingTxt.compoundIcon: "textfield.xpm"
*wRequestingTxt.compoundName: "text_Field"
*wRequestingTxt.x: 260
*wRequestingTxt.y: 100
*wRequestingTxt.height: 40
*wRequestingTxt.leftAttachment: "attach_widget"
*wRequestingTxt.leftOffset: 15
*wRequestingTxt.leftWidget: "label7"
*wRequestingTxt.topAttachment: "attach_opposite_widget"
*wRequestingTxt.topOffset: 0
*wRequestingTxt.topWidget: "label7"

*wRespondingTxt.class: textField
*wRespondingTxt.static: true
*wRespondingTxt.name: wRespondingTxt
*wRespondingTxt.parent: printServerCFGForm
*wRespondingTxt.width: 290
*wRespondingTxt.isCompound: "true"
*wRespondingTxt.compoundIcon: "textfield.xpm"
*wRespondingTxt.compoundName: "text_Field"
*wRespondingTxt.x: 260
*wRespondingTxt.y: 150
*wRespondingTxt.height: 40
*wRespondingTxt.leftAttachment: "attach_widget"
*wRespondingTxt.leftOffset: 15
*wRespondingTxt.leftWidget: "label8"
*wRespondingTxt.topAttachment: "attach_opposite_widget"
*wRespondingTxt.topOffset: 0
*wRespondingTxt.topWidget: "label8"

*wQueueIDTxt.class: textField
*wQueueIDTxt.static: true
*wQueueIDTxt.name: wQueueIDTxt
*wQueueIDTxt.parent: printServerCFGForm
*wQueueIDTxt.width: 290
*wQueueIDTxt.isCompound: "true"
*wQueueIDTxt.compoundIcon: "textfield.xpm"
*wQueueIDTxt.compoundName: "text_Field"
*wQueueIDTxt.x: 260
*wQueueIDTxt.y: 210
*wQueueIDTxt.height: 40
*wQueueIDTxt.leftAttachment: "attach_widget"
*wQueueIDTxt.leftOffset: 15
*wQueueIDTxt.leftWidget: "label7"
*wQueueIDTxt.topAttachment: "attach_opposite_widget"
*wQueueIDTxt.topOffset: 0
*wQueueIDTxt.topWidget: "label9"

*pushButton7.class: pushButton
*pushButton7.static: true
*pushButton7.name: pushButton7
*pushButton7.parent: printServerCFGForm
*pushButton7.isCompound: "true"
*pushButton7.compoundIcon: "push.xpm"
*pushButton7.compoundName: "push_Button"
*pushButton7.x: 40
*pushButton7.y: 310
*pushButton7.width: 130
*pushButton7.height: 40
*pushButton7.labelString: "Add"
*pushButton7.activateCallback: {\
  DMAN_PRINTSERVERCFG\
    p;\
  CONDITION\
    cond;\
\
    cond = readPrintServerCFG(&p);\
    if (cond != 1) {\
	COND_DumpConditions();\
	return;\
    }\
\
    cond = DMAN_Insert(&dmanHandle, (DMAN_GENERICRECORD *)&p);\
    if (cond != DMAN_NORMAL)\
	COND_DumpConditions();\
\
   loadPrintServerCFGList();\
}

*pushButton8.class: pushButton
*pushButton8.static: true
*pushButton8.name: pushButton8
*pushButton8.parent: printServerCFGForm
*pushButton8.isCompound: "true"
*pushButton8.compoundIcon: "push.xpm"
*pushButton8.compoundName: "push_Button"
*pushButton8.x: 230
*pushButton8.y: 310
*pushButton8.width: 130
*pushButton8.height: 40
*pushButton8.labelString: "Update"
*pushButton8.sensitive: "false"

*pushButton9.class: pushButton
*pushButton9.static: true
*pushButton9.name: pushButton9
*pushButton9.parent: printServerCFGForm
*pushButton9.isCompound: "true"
*pushButton9.compoundIcon: "push.xpm"
*pushButton9.compoundName: "push_Button"
*pushButton9.x: 410
*pushButton9.y: 310
*pushButton9.width: 130
*pushButton9.height: 40
*pushButton9.labelString: "Delete"
*pushButton9.activateCallback: {\
  DMAN_PRINTSERVERCFG\
    p;\
  CONDITION\
    cond;\
\
    cond = readPrintServerCFG(&p);\
    if (cond != 1) {\
	COND_DumpConditions();\
	return;\
    }\
\
    cond = DMAN_Delete(&dmanHandle, (DMAN_GENERICRECORD *)&p);\
    if (cond != DMAN_NORMAL)\
	COND_DumpConditions();\
\
    loadPrintServerCFGList();\
}

*frame3.class: frame
*frame3.static: true
*frame3.name: frame3
*frame3.parent: printServerCFGForm
*frame3.width: 540
*frame3.height: 120
*frame3.isCompound: "true"
*frame3.compoundIcon: "frame.xpm"
*frame3.compoundName: "frame_"
*frame3.x: 10
*frame3.y: 10

*scrolledWindowList3.class: scrolledWindow
*scrolledWindowList3.static: true
*scrolledWindowList3.name: scrolledWindowList3
*scrolledWindowList3.parent: frame3
*scrolledWindowList3.scrollingPolicy: "application_defined"
*scrolledWindowList3.visualPolicy: "variable"
*scrolledWindowList3.scrollBarDisplayPolicy: "static"
*scrolledWindowList3.shadowThickness: 0
*scrolledWindowList3.isCompound: "true"
*scrolledWindowList3.compoundIcon: "scrllist.xpm"
*scrolledWindowList3.compoundName: "scrolled_List"
*scrolledWindowList3.x: 60
*scrolledWindowList3.y: 40

*wPrintServerCFGList.class: scrolledList
*wPrintServerCFGList.static: true
*wPrintServerCFGList.name: wPrintServerCFGList
*wPrintServerCFGList.parent: scrolledWindowList3
*wPrintServerCFGList.width: 280
*wPrintServerCFGList.height: 30
*wPrintServerCFGList.browseSelectionCallback: {\
  XmListCallbackStruct *l;\
  DMAN_PRINTSERVERCFG *p;\
\
    l = (XmListCallbackStruct *)UxCallbackArg;\
    p = LST_Index(&printServerCFGList, l->item_position);\
    displayPrintServerCFG(p);\
}

