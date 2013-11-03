! UIMX ascii 2.5 key: 9765                                                      

*MultipleSelection.class: messageBoxDialog
*MultipleSelection.gbldecl: /*\
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
** Module Name(s):	create_MultipleSelection\
** Author, Date:	Chander Sabharwal\
** Intent:		This module has the dialog widget that is displayed\
**			when prompting the user to select the last image\
**			in a set of images for printing (select first,\
**			select last).\
** Last Update:		$Author: smm $, $Date: 1995-04-10 17:32:09 $\
** Source File:		$RCSfile: MultipleSelection.i,v $\
** Revision:		$Revision: 1.6 $\
** Status:		$State: Exp $\
*/\
 \
static char rcsid[] = "$Revision: 1.6 $ $RCSfile: MultipleSelection.i,v $";\
\
#include <stdio.h>\
\
void cancelMulti_select();\
\

*MultipleSelection.ispecdecl:
*MultipleSelection.funcdecl: swidget create_MultipleSelection(swidget UxParent)
*MultipleSelection.funcname: create_MultipleSelection
*MultipleSelection.funcdef: "swidget", "<create_MultipleSelection>(%)"
*MultipleSelection.argdecl: swidget UxParent;
*MultipleSelection.arglist: UxParent
*MultipleSelection.arglist.UxParent: "swidget", "%UxParent%"
*MultipleSelection.icode:
*MultipleSelection.fcode: return(rtrn);\

*MultipleSelection.auxdecl:
*MultipleSelection.static: true
*MultipleSelection.name: MultipleSelection
*MultipleSelection.parent: NO_PARENT
*MultipleSelection.parentExpression: UxParent
*MultipleSelection.defaultShell: topLevelShell
*MultipleSelection.width: 270
*MultipleSelection.height: 100
*MultipleSelection.msgDialogType: "dialog_message"
*MultipleSelection.isCompound: "true"
*MultipleSelection.compoundIcon: "messageD.xpm"
*MultipleSelection.compoundName: "msgBox_Dialog"
*MultipleSelection.x: 750
*MultipleSelection.y: 180
*MultipleSelection.unitType: "pixels"
*MultipleSelection.messageString: "Select last image with Button 2."
*MultipleSelection.okCallback: {\
\
}
*MultipleSelection.cancelCallback: {\
cancelMulti_select();\
}
*MultipleSelection.allowShellResize: "false"

