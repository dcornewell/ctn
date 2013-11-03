! UIMX ascii 2.5 key: 9324                                                      

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
/*\
**                   Electronic Radiology Laboratory\
**                 Mallinckrodt Institute of Radiology\
**              Washington University School of Medicine\
**\
** Module Name(s):\
** Author, Date:	Chander Sabharwal\
** Intent:		Define global widgets used throughout the interface.\
** Last Update:         $Author: smm $, $Date: 1998-04-17 15:19:40 $\
** Source File:         $RCSfile: applicationShell1.i,v $\
** Revision:            $Revision: 1.15 $\
** Status:              $State: Exp $\
*/\
 \
static char rcsid[] = "$Revision: 1.15 $ $RCSfile: applicationShell1.i,v $";\
\
#include <stdio.h>\
\
#include "dicom.h"\
#include "lst.h"\
#include "condition.h"\
#include "dulprotocol.h"\
#include "dicom_objects.h"\
#include "dicom_messages.h"\
#include "dicom_services.h"\
#include "dbquery.h"\
#include "dicom_uids.h"\
#include "dicom_sq.h"\
#include "dicom_print.h"\
#include "icon.h"\
#include "print_shared.h"\
\
\
\
extern char *serverGroup;\
\
void  freeREsources();\
\
static Widget wSelectPrinterDialog = 0;\
Widget wFilmInformationForm = 0;\
Widget wFilmSessionForm = 0;\
Widget wFilmBoxForm = 0;\
Widget wStudyListForm = 0;\
Widget wPreviewStudyForm = 0;\
Widget wPreviewPrintForm = 0;\
Widget wBasicFilmBox_Standard = 0;\
Widget  wPrintMessage = 0;\
Widget  wMultipleSelection = 0;\
Widget  wImageAttribute = 0;\
\
\
extern DUL_NETWORKKEY *networkKey;\
extern DUL_ASSOCIATIONKEY *associationKey;\
extern DUL_ASSOCIATESERVICEPARAMETERS associationParameters;\

*applicationShell1.ispecdecl:
*applicationShell1.funcdecl: swidget create_applicationShell1(swidget UxParent)
*applicationShell1.funcname: create_applicationShell1
*applicationShell1.funcdef: "swidget", "<create_applicationShell1>(%)"
*applicationShell1.argdecl: swidget UxParent;
*applicationShell1.arglist: UxParent
*applicationShell1.arglist.UxParent: "swidget", "%UxParent%"
*applicationShell1.icode:
*applicationShell1.fcode: return(rtrn);\

*applicationShell1.auxdecl: void clearMessage()\
{\
}\
\
void setMessage(char *txt)\
{\
  XmTextSetString(wMessageTxt, txt);\
}\
\
\
\
\

*applicationShell1.static: true
*applicationShell1.name: applicationShell1
*applicationShell1.parent: NO_PARENT
*applicationShell1.parentExpression: UxParent
*applicationShell1.width: 650
*applicationShell1.height: 260
*applicationShell1.isCompound: "true"
*applicationShell1.compoundIcon: "applS.xpm"
*applicationShell1.compoundName: "appl_Shell"
*applicationShell1.x: 0
*applicationShell1.y: 200

*mainWindow1.class: mainWindow
*mainWindow1.static: true
*mainWindow1.name: mainWindow1
*mainWindow1.parent: applicationShell1
*mainWindow1.width: 370
*mainWindow1.height: 160
*mainWindow1.isCompound: "true"
*mainWindow1.compoundIcon: "main.xpm"
*mainWindow1.compoundName: "main_Window"
*mainWindow1.compoundEditor: {\
extern swidget UxGUIMePopup UXPROTO((swidget, swidget, int, int));\
UxGUIMwePopup(UxThisWidget, UxGetParent(UxThisWidget));\
}
*mainWindow1.x: 130
*mainWindow1.y: 10
*mainWindow1.unitType: "pixels"

*menu1.class: rowColumn
*menu1.static: true
*menu1.name: menu1
*menu1.parent: mainWindow1
*menu1.rowColumnType: "menu_bar"
*menu1.menuAccelerator: "<KeyUp>F10"

*menu1_p1.class: rowColumn
*menu1_p1.static: true
*menu1_p1.name: menu1_p1
*menu1_p1.parent: menu1
*menu1_p1.rowColumnType: "menu_pulldown"

*menu1_p1_b1.class: pushButton
*menu1_p1_b1.static: true
*menu1_p1_b1.name: menu1_p1_b1
*menu1_p1_b1.parent: menu1_p1
*menu1_p1_b1.labelString: "Connect"
*menu1_p1_b1.activateCallback: {\
CONDITION  cond = DUL_NORMAL;\
\
\
if(associationKey != NULL)\
 cond = DUL_ReleaseAssociation(&associationKey);\
        if (cond != DUL_RELEASECONFIRMED) {\
        cond = DUL_AbortAssociation(&associationKey);\
 \
}\
    (void) DUL_DropAssociation(&associationKey);\
\
\
\
#ifdef RUNTIME\
    if (wSelectPrinterDialog == 0)\
	wSelectPrinterDialog = (Widget)create_selectPrinterDialog(NULL);\
\
    XtManageChild(wSelectPrinterDialog);\
\
    loadPrintServerList(serverGroup);\
#endif\
}

*menu1_p1_b2.class: pushButton
*menu1_p1_b2.static: true
*menu1_p1_b2.name: menu1_p1_b2
*menu1_p1_b2.parent: menu1_p1
*menu1_p1_b2.labelString: "Quit"
*menu1_p1_b2.activateCallback: {\
 freeResources();\
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

*frame1.class: frame
*frame1.static: true
*frame1.name: frame1
*frame1.parent: form1
*frame1.width: 610
*frame1.height: 200
*frame1.isCompound: "true"
*frame1.compoundIcon: "frame.xpm"
*frame1.compoundName: "frame_"
*frame1.x: 20
*frame1.y: 10

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
*scrolledWindowText1.x: 60
*scrolledWindowText1.y: 50

*wMessageTxt.class: scrolledText
*wMessageTxt.static: true
*wMessageTxt.name: wMessageTxt
*wMessageTxt.parent: scrolledWindowText1
*wMessageTxt.width: 210
*wMessageTxt.height: 40

