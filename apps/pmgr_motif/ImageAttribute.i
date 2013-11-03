! UIMX ascii 2.5 key: 4483                                                      

*ImageAttribute.class: formDialog
*ImageAttribute.gbldecl: /*\
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
** Module Name(s):	create_ImageAttribute\
**			UpdateEntries_in_ImageAttWindow\
** Author, Date:	Chander Sabharwal\
** Intent:		This module contains the widget that allows the\
**			user to modify the attributes of a single image\
**			on the print "film".  The user can modify\
**			polarity and the algorithm for magnification.\
** Last Update:		$Author: smm $, $Date: 1995-04-10 17:32:14 $\
** Source File:		$RCSfile: ImageAttribute.i,v $\
** Revision:		$Revision: 1.7 $\
** Status:		$State: Exp $\
*/\
 \
static char rcsid[] = "$Revision: 1.7 $ $RCSfile: ImageAttribute.i,v $";\
\
#include <stdio.h>\
\
#include "dicom.h"\
#include "condition.h"\
#include "lst.h"\
#include "tbl.h"\
#include "dicom_uids.h"\
#include "dulprotocol.h"\
#include "dicom_objects.h"\
#include "dicom_print.h"\
#include "manage.h"\
\
\
#include "print_shared.h"\
#include "print_client_app.h"\
\
extern Widget wMessageTxt;\
\
void  UpdateEntries_in_ImageAttWindow();\
\
extern char\
    imagePositionC[64];\
\
extern PRN_BASICIMAGEBOX\
    bibAttrib;\
\
extern int\
    selected_bib;\
\
typedef struct {\
    Widget button;\
    Pixmap pixmap;\
    XImage *image;\
    char UID[DICOM_UI_LENGTH + 1];\
    PRN_BASICIMAGEBOX bib;\
}   BASICIMAGEBOX;\
\
\
BASICIMAGEBOX\
    bib[8][8];\

*ImageAttribute.ispecdecl:
*ImageAttribute.funcdecl: swidget create_ImageAttribute(swidget UxParent)
*ImageAttribute.funcname: create_ImageAttribute
*ImageAttribute.funcdef: "swidget", "<create_ImageAttribute>(%)"
*ImageAttribute.argdecl: swidget UxParent;
*ImageAttribute.arglist: UxParent
*ImageAttribute.arglist.UxParent: "swidget", "%UxParent%"
*ImageAttribute.icode:
*ImageAttribute.fcode: return(rtrn);\

*ImageAttribute.auxdecl: void UpdateEntries_in_ImageAttWindow()\
{\
 int  imgPos;\
  CONDITION  cond;\
  CTNBOOLEAN nullFlag; \
  char  bufchar[64];\
\
printf( " in Update\n");\
\
printf("bib.imgP = %d\n", bibAttrib.imagePosition);\
\
sprintf(bufchar,"%d", bibAttrib.imagePosition);\
XmTextFieldSetString(wImagePositionTxt, bufchar);\
\
 printf("bib.Pol = %s,%s\n",bibAttrib.polarity,\
                  bib[selected_bib / 8][selected_bib % 8].bib.polarity);\
\
if(strcmp(bibAttrib.polarity, "NORMAL") == 0)\
  XmToggleButtonGadgetSetState(wNormalTBG, TRUE,TRUE);\
else if(strcmp(bibAttrib.polarity, "REVERSE") == 0)\
  XmToggleButtonGadgetSetState(wReverseTBG, TRUE,TRUE);\
\
\
printf("bib.mag = %s,%s\n",bibAttrib.magnificationType,\
	bib[selected_bib / 8][selected_bib % 8].bib.magnificationType);\
\
if(strcmp(bibAttrib.magnificationType,"REPLICATE") == 0)\
   XmToggleButtonGadgetSetState(wReplicateTBG,TRUE,TRUE);\
else if(strcmp(bibAttrib.magnificationType, "BILINEAR") == 0)\
  XmToggleButtonGadgetSetState(wBilinearTBG, TRUE,TRUE);\
else if(strcmp(bibAttrib.magnificationType, "CUBIC") == 0)\
  XmToggleButtonGadgetSetState(wCubicTBG, TRUE,TRUE);\
}
*ImageAttribute.static: true
*ImageAttribute.name: ImageAttribute
*ImageAttribute.parent: NO_PARENT
*ImageAttribute.parentExpression: UxParent
*ImageAttribute.defaultShell: topLevelShell
*ImageAttribute.width: 270
*ImageAttribute.height: 313
*ImageAttribute.isCompound: "true"
*ImageAttribute.compoundIcon: "formD.xpm"
*ImageAttribute.compoundName: "form_Dialog"
*ImageAttribute.x: 510
*ImageAttribute.y: 397
*ImageAttribute.unitType: "pixels"

*labelGadget2.class: labelGadget
*labelGadget2.static: true
*labelGadget2.name: labelGadget2
*labelGadget2.parent: ImageAttribute
*labelGadget2.isCompound: "true"
*labelGadget2.compoundIcon: "labelG.xpm"
*labelGadget2.compoundName: "labelG_"
*labelGadget2.x: 0
*labelGadget2.y: 0
*labelGadget2.width: 110
*labelGadget2.height: 30
*labelGadget2.labelString: "Image Position:"

*wImagePositionTxt.class: textField
*wImagePositionTxt.static: true
*wImagePositionTxt.name: wImagePositionTxt
*wImagePositionTxt.parent: ImageAttribute
*wImagePositionTxt.width: 230
*wImagePositionTxt.isCompound: "true"
*wImagePositionTxt.compoundIcon: "textfield.xpm"
*wImagePositionTxt.compoundName: "text_Field"
*wImagePositionTxt.x: 10
*wImagePositionTxt.y: 30
*wImagePositionTxt.height: 30
*wImagePositionTxt.text: "1"

*labelGadget3.class: labelGadget
*labelGadget3.static: true
*labelGadget3.name: labelGadget3
*labelGadget3.parent: ImageAttribute
*labelGadget3.isCompound: "true"
*labelGadget3.compoundIcon: "labelG.xpm"
*labelGadget3.compoundName: "labelG_"
*labelGadget3.x: 0
*labelGadget3.y: 60
*labelGadget3.width: 90
*labelGadget3.height: 30
*labelGadget3.labelString: "Polarity:"

*rowColumn9.class: rowColumn
*rowColumn9.static: true
*rowColumn9.name: rowColumn9
*rowColumn9.parent: ImageAttribute
*rowColumn9.width: 80
*rowColumn9.height: 80
*rowColumn9.isCompound: "true"
*rowColumn9.compoundIcon: "row.xpm"
*rowColumn9.compoundName: "row_Column"
*rowColumn9.x: 10
*rowColumn9.y: 90
*rowColumn9.radioBehavior: "true"

*wNormalTBG.class: toggleButtonGadget
*wNormalTBG.static: true
*wNormalTBG.name: wNormalTBG
*wNormalTBG.parent: rowColumn9
*wNormalTBG.isCompound: "true"
*wNormalTBG.compoundIcon: "toggleG.xpm"
*wNormalTBG.compoundName: "toggleB_Gadget"
*wNormalTBG.x: 0
*wNormalTBG.y: 0
*wNormalTBG.width: 80
*wNormalTBG.height: 30
*wNormalTBG.labelString: "Normal"
*wNormalTBG.set: "true"

*wReverseTBG.class: toggleButtonGadget
*wReverseTBG.static: true
*wReverseTBG.name: wReverseTBG
*wReverseTBG.parent: rowColumn9
*wReverseTBG.isCompound: "true"
*wReverseTBG.compoundIcon: "toggleG.xpm"
*wReverseTBG.compoundName: "toggleB_Gadget"
*wReverseTBG.x: 13
*wReverseTBG.y: 13
*wReverseTBG.width: 80
*wReverseTBG.height: 30
*wReverseTBG.labelString: "Reverse"

*labelGadget4.class: labelGadget
*labelGadget4.static: true
*labelGadget4.name: labelGadget4
*labelGadget4.parent: ImageAttribute
*labelGadget4.isCompound: "true"
*labelGadget4.compoundIcon: "labelG.xpm"
*labelGadget4.compoundName: "labelG_"
*labelGadget4.x: 0
*labelGadget4.y: 160
*labelGadget4.width: 140
*labelGadget4.height: 30
*labelGadget4.labelString: "Magnification Type:"

*rowColumn10.class: rowColumn
*rowColumn10.static: true
*rowColumn10.name: rowColumn10
*rowColumn10.parent: ImageAttribute
*rowColumn10.width: 130
*rowColumn10.height: 90
*rowColumn10.isCompound: "true"
*rowColumn10.compoundIcon: "row.xpm"
*rowColumn10.compoundName: "row_Column"
*rowColumn10.x: 10
*rowColumn10.y: 200
*rowColumn10.radioBehavior: "true"

*wReplicateTBG.class: toggleButtonGadget
*wReplicateTBG.static: true
*wReplicateTBG.name: wReplicateTBG
*wReplicateTBG.parent: rowColumn10
*wReplicateTBG.isCompound: "true"
*wReplicateTBG.compoundIcon: "toggleG.xpm"
*wReplicateTBG.compoundName: "toggleB_Gadget"
*wReplicateTBG.x: 0
*wReplicateTBG.y: 0
*wReplicateTBG.width: 130
*wReplicateTBG.height: 30
*wReplicateTBG.labelString: "Replicate"
*wReplicateTBG.set: "true"

*wBilinearTBG.class: toggleButtonGadget
*wBilinearTBG.static: true
*wBilinearTBG.name: wBilinearTBG
*wBilinearTBG.parent: rowColumn10
*wBilinearTBG.isCompound: "true"
*wBilinearTBG.compoundIcon: "toggleG.xpm"
*wBilinearTBG.compoundName: "toggleB_Gadget"
*wBilinearTBG.x: 13
*wBilinearTBG.y: 13
*wBilinearTBG.width: 130
*wBilinearTBG.height: 30
*wBilinearTBG.labelString: "Bilinear"

*wCubicTBG.class: toggleButtonGadget
*wCubicTBG.static: true
*wCubicTBG.name: wCubicTBG
*wCubicTBG.parent: rowColumn10
*wCubicTBG.isCompound: "true"
*wCubicTBG.compoundIcon: "toggleG.xpm"
*wCubicTBG.compoundName: "toggleB_Gadget"
*wCubicTBG.x: 13
*wCubicTBG.y: 42
*wCubicTBG.width: 130
*wCubicTBG.height: 30
*wCubicTBG.labelString: "Cubic"

