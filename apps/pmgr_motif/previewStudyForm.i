! UIMX ascii 2.5 key: 2470                                                      

*previewStudyForm.class: formDialog
*previewStudyForm.gbldecl: /*\
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
** Module Name(s):      create_previewStudyForm\
**			loadPreviewStudyForm\
**			displayMessage\
** Author, Date:        Chander Sabharwal\
** Intent:              This module provides a container for the\
**			widget that will be used to hold image icons\
**			(to allow the user to select images to be\
**			placed on film and printed.)\
** Last Update:         $Author: smm $, $Date: 1995-04-10 17:30:56 $\
** Source File:         $RCSfile: previewStudyForm.i,v $\
** Revision:            $Revision: 1.10 $\
** Status:              $State: Exp $\
*/\
 \
static char rcsid[] = "$Revision: 1.10 $ $RCSfile: previewStudyForm.i,v $";\
\
#include <stdio.h>\
#include <Xm/DrawnB.h>\
#include <Xm/MessageB.h>\
\
extern Widget wPreviewStudyForm;\
extern Widget  wMultipleSelection;\
\
void  displayMessage();\
\
\
\

*previewStudyForm.ispecdecl:
*previewStudyForm.funcdecl: swidget create_previewStudyForm(swidget UxParent)
*previewStudyForm.funcname: create_previewStudyForm
*previewStudyForm.funcdef: "swidget", "<create_previewStudyForm>(%)"
*previewStudyForm.argdecl: swidget UxParent;
*previewStudyForm.arglist: UxParent
*previewStudyForm.arglist.UxParent: "swidget", "%UxParent%"
*previewStudyForm.icode:
*previewStudyForm.fcode: return(rtrn);\

*previewStudyForm.auxdecl: void loadPreviewStudyForm(int cols, int rows)\
{\
    createPreviewStudyIcons(cols, rows, wPreviewStudyForm);\
 \
}\
\
void  displayMessage()\
{\
#ifdef RUNTIME\
  if(wMultipleSelection == 0)\
     wMultipleSelection = (Widget) create_MultipleSelection(NULL);\
     XtManageChild(wMultipleSelection);\
#endif\
}
*previewStudyForm.static: true
*previewStudyForm.name: previewStudyForm
*previewStudyForm.parent: NO_PARENT
*previewStudyForm.parentExpression: UxParent
*previewStudyForm.defaultShell: topLevelShell
*previewStudyForm.width: 590
*previewStudyForm.height: 602
*previewStudyForm.isCompound: "true"
*previewStudyForm.compoundIcon: "formD.xpm"
*previewStudyForm.compoundName: "form_Dialog"
*previewStudyForm.x: 350
*previewStudyForm.y: 258
*previewStudyForm.unitType: "pixels"
*previewStudyForm.autoUnmanage: "false"

