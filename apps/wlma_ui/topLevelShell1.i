! UIMX ascii 2.5 key: 7807                                                      

*topLevelShell1.class: topLevelShell
*topLevelShell1.gbldecl: /*\
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
** Intent:		This file contains the top level widget/interface\
**			for the WLMA application.  It serves as a parent\
**			for the other widget hierarchies that actually\
**			hold the interface.  This interface contains the\
**			"Control" pulldown menu that allows the user to\
**			select which interface is to be activated for\
**			work list management (the database or demonstration\
**			interface).\
** Last Update:         $Author: smm $, $Date: 1995-12-22 17:28:01 $\
** Source File:         $RCSfile: topLevelShell1.i,v $\
** Revision:            $Revision: 1.7 $\
** Status:              $State: Exp $\
*/\
 \
static char rcsid[] = "$Revision: 1.7 $ $RCSfile: topLevelShell1.i,v $";\
\
#include <stdio.h>\
/*global*/\
\
#include "dicom.h"\
\
static Widget wDatabaseForm;\
static CTNBOOLEAN databaseActive = FALSE;\
static Widget wDemonstrationForm;\
static CTNBOOLEAN demonstrationActive = FALSE;
*topLevelShell1.ispecdecl: /*interface*/
*topLevelShell1.funcdecl: swidget create_topLevelShell1(swidget UxParent)\
/*interface function*/
*topLevelShell1.funcname: create_topLevelShell1
*topLevelShell1.funcdef: "swidget", "<create_topLevelShell1>(%)"
*topLevelShell1.argdecl: swidget UxParent;
*topLevelShell1.arglist: UxParent
*topLevelShell1.arglist.UxParent: "swidget", "%UxParent%"
*topLevelShell1.icode: /*initial code*/
*topLevelShell1.fcode: /*final code*/\
return(rtrn);\

*topLevelShell1.auxdecl: void errorText(char *txt)\
{\
}
*topLevelShell1.static: true
*topLevelShell1.name: topLevelShell1
*topLevelShell1.parent: NO_PARENT
*topLevelShell1.parentExpression: UxParent
*topLevelShell1.width: 1070
*topLevelShell1.height: 780
*topLevelShell1.isCompound: "true"
*topLevelShell1.compoundIcon: "toplevelS.xpm"
*topLevelShell1.compoundName: "topLevel_Shell"
*topLevelShell1.x: 107
*topLevelShell1.y: 225

*mainWindow1.class: mainWindow
*mainWindow1.static: true
*mainWindow1.name: mainWindow1
*mainWindow1.parent: topLevelShell1
*mainWindow1.width: 1070
*mainWindow1.height: 780
*mainWindow1.isCompound: "true"
*mainWindow1.compoundIcon: "main.xpm"
*mainWindow1.compoundName: "main_Window"
*mainWindow1.compoundEditor: {\
extern swidget UxGUIMePopup UXPROTO((swidget, swidget, int, int));\
UxGUIMwePopup(UxThisWidget, UxGetParent(UxThisWidget));\
}
*mainWindow1.x: 190
*mainWindow1.y: 210
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

*menu1_p1_b2.class: pushButton
*menu1_p1_b2.static: true
*menu1_p1_b2.name: menu1_p1_b2
*menu1_p1_b2.parent: menu1_p1
*menu1_p1_b2.labelString: "Edit Database"
*menu1_p1_b2.activateCallback: {\
#ifdef RUNTIME\
    if (demonstrationActive)\
	(void) XtUnmanageChild(wDemonstrationForm);\
\
    if (!databaseActive) {\
	wDatabaseForm = (Widget) create_databaseForm(form1);\
	databaseActive = TRUE;\
    } else {\
	XtManageChild(wDatabaseForm);\
    }\
\
    loadPatientList();\
#endif\
}

*menu1_p1_b3.class: pushButton
*menu1_p1_b3.static: true
*menu1_p1_b3.name: menu1_p1_b3
*menu1_p1_b3.parent: menu1_p1
*menu1_p1_b3.labelString: "Demonstration Interface"
*menu1_p1_b3.activateCallback: {\
#ifdef RUNTIME\
    if (databaseActive) {\
	(void) XtUnmanageChild(wDatabaseForm);\
    }\
    if (!demonstrationActive) {\
	wDemonstrationForm = (Widget) create_demonstrationForm(form1);\
	demonstrationActive = TRUE;\
    } else {\
	XtManageChild(wDemonstrationForm);\
    }\
\
    loadPatientStudyList();\
    loadWorkList();\
#endif\
}

*menu1_p1_b5.class: pushButton
*menu1_p1_b5.static: true
*menu1_p1_b5.name: menu1_p1_b5
*menu1_p1_b5.parent: menu1_p1
*menu1_p1_b5.labelString: "Configuration"
*menu1_p1_b5.activateCallback: {\
\
}

*menu1_p1_b4.class: separatorGadget
*menu1_p1_b4.static: true
*menu1_p1_b4.name: menu1_p1_b4
*menu1_p1_b4.parent: menu1_p1

*menu1_p1_b1.class: pushButton
*menu1_p1_b1.static: true
*menu1_p1_b1.name: menu1_p1_b1
*menu1_p1_b1.parent: menu1_p1
*menu1_p1_b1.labelString: "Quit"
*menu1_p1_b1.activateCallback: {\
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

