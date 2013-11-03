! UIMX ascii 2.5 key: 5065                                                      

*PrintMessage.class: messageBoxDialog
*PrintMessage.gbldecl: /*\
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
** Module Name(s):      create_PrintMessage\
** Author, Date:        Chander Sabharwal\
** Intent:              This module contains a widget and code that allows\
**			the developer to pup up a message saying that\
**			printing completed successfully.\
** Last Update:         $Author: smm $, $Date: 1995-04-10 17:32:05 $\
** Source File:         $RCSfile: PrintMessage.i,v $\
** Revision:            $Revision: 1.7 $\
** Status:              $State: Exp $\
*/\
 \
static char rcsid[] = "$Revision: 1.7 $ $RCSfile: PrintMessage.i,v $";\
\
#include <stdio.h>\
\
void  MoreBFB();
*PrintMessage.ispecdecl:
*PrintMessage.funcdecl: swidget create_PrintMessage(swidget UxParent)
*PrintMessage.funcname: create_PrintMessage
*PrintMessage.funcdef: "swidget", "<create_PrintMessage>(%)"
*PrintMessage.argdecl: swidget UxParent;
*PrintMessage.arglist: UxParent
*PrintMessage.arglist.UxParent: "swidget", "%UxParent%"
*PrintMessage.icode:
*PrintMessage.fcode: return(rtrn);\

*PrintMessage.auxdecl:
*PrintMessage.static: true
*PrintMessage.name: PrintMessage
*PrintMessage.parent: NO_PARENT
*PrintMessage.parentExpression: UxParent
*PrintMessage.defaultShell: topLevelShell
*PrintMessage.width: 300
*PrintMessage.height: 100
*PrintMessage.msgDialogType: "dialog_message"
*PrintMessage.isCompound: "true"
*PrintMessage.compoundIcon: "messageD.xpm"
*PrintMessage.compoundName: "msgBox_Dialog"
*PrintMessage.x: 640
*PrintMessage.y: 320
*PrintMessage.unitType: "pixels"
*PrintMessage.messageString: " 'Printed!' , ' Print Operation Completed'"
*PrintMessage.textFontList: "-adobe-helvetica-bold-r-normal--17-120-100-100-p-92-iso8859-1"
*PrintMessage.okCallback: {\
MoreBFB();\
}

