! UIMX ascii 2.5 key: 121                                                       

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
** Module Name(s):\
** Author, Date:	Steve Moore, Summer 1994\
** Intent:		This file contains the generated code that\
**			defines the top level shell for the\
**			cfg_ctn_tables application.  This shell serves\
**			as a parent for the interesting widgets that\
**			actually provide the user interface.  This file\
**			contains the callback functions for push buttons\
**			which activate the various interfaces provided\
**			under the umbrella of this single program.\
** Last Update:         $Author: smm $, $Date: 1995-04-10 13:19:36 $\
** Source File:         $RCSfile: applicationShell1.i,v $\
** Revision:            $Revision: 1.9 $\
** Status:              $State: Exp $\
*/\
 \
static char rcsid[] = "$Revision: 1.9 $ $RCSfile: applicationShell1.i,v $";\
\
#include <stdio.h>\
\
#include "dicom.h"\
\
static Widget wApplicationForm = 0;\
static CTNBOOLEAN applicationActive = FALSE;\
static Widget wSecurityForm = 0;\
static CTNBOOLEAN securityActive = FALSE;\
static Widget wPrintServerCFGForm = 0;\
static CTNBOOLEAN printServerCFGActive = FALSE;\
static Widget wGroupNamesForm = 0;\
static Widget wFISAccessForm = 0;\
static Widget wImageStorageAccessForm = 0;\
static Widget wImageStorageControlForm = 0;\
static Widget wVideoImageForm = 0;\
\
static void clearInterfaces();
*applicationShell1.ispecdecl:
*applicationShell1.funcdecl: swidget create_applicationShell1(swidget UxParent)
*applicationShell1.funcname: create_applicationShell1
*applicationShell1.funcdef: "swidget", "<create_applicationShell1>(%)"
*applicationShell1.argdecl: swidget UxParent;
*applicationShell1.arglist: UxParent
*applicationShell1.arglist.UxParent: "swidget", "%UxParent%"
*applicationShell1.icode:
*applicationShell1.fcode: return(rtrn);\

*applicationShell1.auxdecl: static void clearInterfaces()\
{\
    if (wApplicationForm != 0)\
	(void) XtUnmanageChild(wApplicationForm);\
    if (wSecurityForm != 0)\
	(void) XtUnmanageChild(wSecurityForm);\
    if (wPrintServerCFGForm != 0)\
	(void) XtUnmanageChild(wPrintServerCFGForm);\
    if (wGroupNamesForm != 0)\
	(void) XtUnmanageChild(wGroupNamesForm);\
    if (wFISAccessForm != 0)\
	(void)XtUnmanageChild(wFISAccessForm);\
    if (wImageStorageAccessForm != 0)\
	(void) XtUnmanageChild(wImageStorageAccessForm);\
    if (wImageStorageControlForm != 0)\
	(void) XtUnmanageChild(wImageStorageControlForm);\
    if (wVideoImageForm != 0)\
	(void) XtUnmanageChild(wVideoImageForm);\
}
*applicationShell1.static: true
*applicationShell1.name: applicationShell1
*applicationShell1.parent: NO_PARENT
*applicationShell1.parentExpression: UxParent
*applicationShell1.width: 830
*applicationShell1.height: 650
*applicationShell1.isCompound: "true"
*applicationShell1.compoundIcon: "applS.xpm"
*applicationShell1.compoundName: "appl_Shell"
*applicationShell1.x: 209
*applicationShell1.y: 241

*mainWindow1.class: mainWindow
*mainWindow1.static: true
*mainWindow1.name: mainWindow1
*mainWindow1.parent: applicationShell1
*mainWindow1.width: 830
*mainWindow1.height: 650
*mainWindow1.isCompound: "true"
*mainWindow1.compoundIcon: "main.xpm"
*mainWindow1.compoundName: "main_Window"
*mainWindow1.compoundEditor: {\
extern swidget UxGUIMePopup UXPROTO((swidget, swidget, int, int));\
UxGUIMwePopup(UxThisWidget, UxGetParent(UxThisWidget));\
}
*mainWindow1.x: 130
*mainWindow1.y: 90
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

*menu1_p1_b7.class: cascadeButton
*menu1_p1_b7.static: true
*menu1_p1_b7.name: menu1_p1_b7
*menu1_p1_b7.parent: menu1_p1
*menu1_p1_b7.labelString: "General"
*menu1_p1_b7.subMenuId: "menu1_p4"

*menu1_p1_b5.class: cascadeButton
*menu1_p1_b5.static: true
*menu1_p1_b5.name: menu1_p1_b5
*menu1_p1_b5.parent: menu1_p1
*menu1_p1_b5.labelString: "Image Server"
*menu1_p1_b5.subMenuId: "menu1_p6"

*menu1_p1_b9.class: cascadeButton
*menu1_p1_b9.static: true
*menu1_p1_b9.name: menu1_p1_b9
*menu1_p1_b9.parent: menu1_p1
*menu1_p1_b9.labelString: "Print Server"
*menu1_p1_b9.subMenuId: "menu1_p2"

*menu1_p1_b6.class: cascadeButton
*menu1_p1_b6.static: true
*menu1_p1_b6.name: menu1_p1_b6
*menu1_p1_b6.parent: menu1_p1
*menu1_p1_b6.labelString: "FIS"
*menu1_p1_b6.subMenuId: "menu1_p5"

*menu1_p1_b8.class: cascadeButton
*menu1_p1_b8.static: true
*menu1_p1_b8.name: menu1_p1_b8
*menu1_p1_b8.parent: menu1_p1
*menu1_p1_b8.labelString: "Video Image"
*menu1_p1_b8.subMenuId: "menu1_p7"

*menu1_p1_b3.class: pushButton
*menu1_p1_b3.static: true
*menu1_p1_b3.name: menu1_p1_b3
*menu1_p1_b3.parent: menu1_p1
*menu1_p1_b3.labelString: "Quit"
*menu1_p1_b3.activateCallback: {\
    exit(0);\
}

*menu1_p2.class: rowColumn
*menu1_p2.static: true
*menu1_p2.name: menu1_p2
*menu1_p2.parent: menu1_p1
*menu1_p2.rowColumnType: "menu_pulldown"

*menu1_p2_b1.class: pushButton
*menu1_p2_b1.static: true
*menu1_p2_b1.name: menu1_p2_b1
*menu1_p2_b1.parent: menu1_p2
*menu1_p2_b1.labelString: "Print Server CFG"
*menu1_p2_b1.activateCallback: {\
#ifdef RUNTIME\
    clearInterfaces();\
\
    if (wPrintServerCFGForm == 0)\
	wPrintServerCFGForm = (Widget) create_printServerCFGForm(bulletinBoard3);\
\
    XtManageChild(wPrintServerCFGForm);\
\
    loadPrintServerCFGList();\
#endif\
}

*menu1_p4.class: rowColumn
*menu1_p4.static: true
*menu1_p4.name: menu1_p4
*menu1_p4.parent: menu1_p1
*menu1_p4.rowColumnType: "menu_pulldown"

*menu1_p4_b2.class: pushButton
*menu1_p4_b2.static: true
*menu1_p4_b2.name: menu1_p4_b2
*menu1_p4_b2.parent: menu1_p4
*menu1_p4_b2.labelString: "Applications"
*menu1_p4_b2.activateCallback: {\
#ifdef RUNTIME\
    clearInterfaces();\
    if (wApplicationForm == 0)\
	wApplicationForm = (Widget) create_applicationForm(bulletinBoard3);\
\
    XtManageChild(wApplicationForm);\
\
    loadApplicationList();\
#endif\
\
\
\
}

*menu1_p4_b3.class: pushButton
*menu1_p4_b3.static: true
*menu1_p4_b3.name: menu1_p4_b3
*menu1_p4_b3.parent: menu1_p4
*menu1_p4_b3.labelString: "Security Matrix"
*menu1_p4_b3.activateCallback: {\
#ifdef RUNTIME\
    clearInterfaces();\
\
    if (wSecurityForm == 0)\
	wSecurityForm = (Widget) create_securityForm(bulletinBoard3);\
\
    XtManageChild(wSecurityForm);\
\
    loadSecurityMatrixList();\
#endif\
}

*menu1_p4_b1.class: pushButton
*menu1_p4_b1.static: true
*menu1_p4_b1.name: menu1_p4_b1
*menu1_p4_b1.parent: menu1_p4
*menu1_p4_b1.labelString: "Group"
*menu1_p4_b1.activateCallback: {\
#ifdef RUNTIME\
    clearInterfaces();\
\
    if (wGroupNamesForm == 0)\
	wGroupNamesForm = (Widget) create_groupNamesForm(bulletinBoard3);\
\
    XtManageChild(wGroupNamesForm);\
\
    loadGroupGroupList();\
#endif\
}

*menu1_p5.class: rowColumn
*menu1_p5.static: true
*menu1_p5.name: menu1_p5
*menu1_p5.parent: menu1_p1
*menu1_p5.rowColumnType: "menu_pulldown"

*menu1_p5_b1.class: pushButton
*menu1_p5_b1.static: true
*menu1_p5_b1.name: menu1_p5_b1
*menu1_p5_b1.parent: menu1_p5
*menu1_p5_b1.labelString: "FIS Access"
*menu1_p5_b1.activateCallback: {\
#ifdef RUNTIME\
    clearInterfaces();\
\
    if (wFISAccessForm == 0)\
	wFISAccessForm = (Widget)create_FISAccessForm(bulletinBoard3);\
\
    XtManageChild(wFISAccessForm);\
\
    loadFISAccessList();\
#endif\
}

*menu1_p6.class: rowColumn
*menu1_p6.static: true
*menu1_p6.name: menu1_p6
*menu1_p6.parent: menu1_p1
*menu1_p6.rowColumnType: "menu_pulldown"

*menu1_p6_b1.class: pushButton
*menu1_p6_b1.static: true
*menu1_p6_b1.name: menu1_p6_b1
*menu1_p6_b1.parent: menu1_p6
*menu1_p6_b1.labelString: "Storage Access"
*menu1_p6_b1.activateCallback: {\
#ifdef RUNTIME\
    clearInterfaces();\
    if (wImageStorageAccessForm == 0)\
	wImageStorageAccessForm = (Widget) create_imageStorageAccessForm(bulletinBoard3);\
\
    XtManageChild(wImageStorageAccessForm);\
\
    loadImageStorageAccessList();\
#endif\
}

*menu1_p6_b2.class: pushButton
*menu1_p6_b2.static: true
*menu1_p6_b2.name: menu1_p6_b2
*menu1_p6_b2.parent: menu1_p6
*menu1_p6_b2.labelString: "Storage Control"
*menu1_p6_b2.activateCallback: {\
#ifdef RUNTIME\
    clearInterfaces();\
    if (wImageStorageControlForm == 0)\
	wImageStorageControlForm = (Widget) create_imageStorageControlForm(bulletinBoard3);\
\
    XtManageChild(wImageStorageControlForm);\
    loadImageStorageControlList();\
#endif\
}

*menu1_p7.class: rowColumn
*menu1_p7.static: true
*menu1_p7.name: menu1_p7
*menu1_p7.parent: menu1_p1
*menu1_p7.rowColumnType: "menu_pulldown"

*menu1_p7_b1.class: pushButton
*menu1_p7_b1.static: true
*menu1_p7_b1.name: menu1_p7_b1
*menu1_p7_b1.parent: menu1_p7
*menu1_p7_b1.labelString: "Destinations"
*menu1_p7_b1.activateCallback: {\
#ifdef RUNTIME\
    clearInterfaces();\
    if (wVideoImageForm == 0)\
	wVideoImageForm = (Widget) create_videoImageForm(bulletinBoard3);\
\
    XtManageChild(wVideoImageForm);\
    loadVideoImageList();\
#endif\
}

*menu1_top_b1.class: cascadeButton
*menu1_top_b1.static: true
*menu1_top_b1.name: menu1_top_b1
*menu1_top_b1.parent: menu1
*menu1_top_b1.labelString: "Control"
*menu1_top_b1.subMenuId: "menu1_p1"

*bulletinBoard3.class: bulletinBoard
*bulletinBoard3.static: true
*bulletinBoard3.name: bulletinBoard3
*bulletinBoard3.parent: mainWindow1

