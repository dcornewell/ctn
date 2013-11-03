! UIMX ascii 2.5 key: 2374                                                      

*BasicFilmBox_Standard.class: formDialog
*BasicFilmBox_Standard.gbldecl: /*\
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
/*\
**                   Electronic Radiology Laboratory\
**                 Mallinckrodt Institute of Radiology\
**              Washington University School of Medicine\
**\
** Module Name(s):	create_BasicFilmBox_Standard\
**			loadBasicFilmBox_Standard\
**			printSuccessMes\
** Author, Date:	Chander Sabharwal\
** Intent:		This module contains the widget that will be used as\
**			the container for icons for images being arranged\
**			on a piece of film using the standard (grid) layout.\
** Last Update:		$Author: smm $, $Date: 1995-04-10 17:31:39 $\
** Source File:		$RCSfile: BasicFilmBox_Standard.i,v $\
** Revision:		$Revision: 1.8 $\
** Status:		$State: Exp $\
*/\
 \
static char rcsid[] = "$Revision: 1.8 $ $RCSfile: BasicFilmBox_Standard.i,v $";\
\
#include <stdio.h>\
\
void createStandardBFBWindow();\
void printSuccessMes();\
\
extern Widget wBasicFilmBox_Standard;\
extern Widget  wPrintMessage;\
\

*BasicFilmBox_Standard.ispecdecl:
*BasicFilmBox_Standard.funcdecl: swidget create_BasicFilmBox_Standard(swidget UxParent)
*BasicFilmBox_Standard.funcname: create_BasicFilmBox_Standard
*BasicFilmBox_Standard.funcdef: "swidget", "<create_BasicFilmBox_Standard>(%)"
*BasicFilmBox_Standard.argdecl: swidget UxParent;
*BasicFilmBox_Standard.arglist: UxParent
*BasicFilmBox_Standard.arglist.UxParent: "swidget", "%UxParent%"
*BasicFilmBox_Standard.icode:
*BasicFilmBox_Standard.fcode: return(rtrn);\

*BasicFilmBox_Standard.auxdecl: loadBasicFilmBox_Standard(int r1, int c1)\
{\
\
   createStandardBFBWindow(r1,c1,wBasicFilmBox_Standard);\
\
}\
\
void printSuccessMes()\
{\
\
#ifdef RUNTIME\
    if (wPrintMessage == 0)\
	wPrintMessage = (Widget) create_PrintMessage(NULL);\
\
    XtManageChild(wPrintMessage);\
#endif\
}
*BasicFilmBox_Standard.static: true
*BasicFilmBox_Standard.name: BasicFilmBox_Standard
*BasicFilmBox_Standard.parent: NO_PARENT
*BasicFilmBox_Standard.parentExpression: UxParent
*BasicFilmBox_Standard.defaultShell: topLevelShell
*BasicFilmBox_Standard.width: 330
*BasicFilmBox_Standard.height: 337
*BasicFilmBox_Standard.isCompound: "true"
*BasicFilmBox_Standard.compoundIcon: "formD.xpm"
*BasicFilmBox_Standard.compoundName: "form_Dialog"
*BasicFilmBox_Standard.x: 490
*BasicFilmBox_Standard.y: 420
*BasicFilmBox_Standard.unitType: "pixels"
*BasicFilmBox_Standard.noResize: "false"
*BasicFilmBox_Standard.allowShellResize: "true"
*BasicFilmBox_Standard.autoUnmanage: "false"

