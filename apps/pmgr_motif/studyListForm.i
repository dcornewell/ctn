! UIMX ascii 2.5 key: 567                                                       

*studyListForm.class: formDialog
*studyListForm.gbldecl: /*\
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
** Module Name(s):	loadStudyList\
**			create_studyListForm\
** Author, Date:	Chander Sabharwal\
** Intent:		This module contains the user interface for displaying the\
**			set of studies that are known by the print manager.  It\
**			contains a public interface for creating the widget to\
**			display the study list.\
** Last Update:		$Author: smm $, $Date: 1995-04-10 17:31:10 $\
** Source File:		$RCSfile: studyListForm.i,v $\
** Revision:		$Revision: 1.10 $\
** Status:		$State: Exp $\
*/\
 \
static char rcsid[] = "$Revision: 1.10 $ $RCSfile: studyListForm.i,v $";\
\
\
#include <stdio.h>\
#include "dicom.h"\
#include "condition.h"\
#include "lst.h"\
#include "tbl.h"\
#include "dicom_uids.h"\
#include "dulprotocol.h"\
#include "dicom_objects.h"\
#include "manage.h"\
#include "mut.h"\
\
#include "display.h"\
#include "format.h"\
\
extern char *icon_file, *icon_index, *db_file;\
extern LST_HEAD *lst_studylist;\
\
void  PreviewStudy();
*studyListForm.ispecdecl:
*studyListForm.funcdecl: swidget create_studyListForm(swidget UxParent)
*studyListForm.funcname: create_studyListForm
*studyListForm.funcdef: "swidget", "<create_studyListForm>(%)"
*studyListForm.argdecl: swidget UxParent;
*studyListForm.arglist: UxParent
*studyListForm.arglist.UxParent: "swidget", "%UxParent%"
*studyListForm.icode:
*studyListForm.fcode: return(rtrn);\

*studyListForm.auxdecl: void\
loadStudyList(LST_HEAD * lst_studylist)\
{\
\
    char buf[65];\
\
    MUT_LoadList(wStudyList, lst_studylist, formatstudylist, buf);\
\
}\

*studyListForm.static: true
*studyListForm.name: studyListForm
*studyListForm.parent: NO_PARENT
*studyListForm.parentExpression: UxParent
*studyListForm.defaultShell: topLevelShell
*studyListForm.width: 330
*studyListForm.height: 320
*studyListForm.isCompound: "true"
*studyListForm.compoundIcon: "formD.xpm"
*studyListForm.compoundName: "form_Dialog"
*studyListForm.x: 550
*studyListForm.y: 140
*studyListForm.unitType: "pixels"

*labelGadget1.class: labelGadget
*labelGadget1.static: true
*labelGadget1.name: labelGadget1
*labelGadget1.parent: studyListForm
*labelGadget1.isCompound: "true"
*labelGadget1.compoundIcon: "labelG.xpm"
*labelGadget1.compoundName: "labelG_"
*labelGadget1.x: 10
*labelGadget1.y: 10
*labelGadget1.width: 120
*labelGadget1.height: 30
*labelGadget1.labelString: "List Of Studies"

*scrolledWindowList1.class: scrolledWindow
*scrolledWindowList1.static: true
*scrolledWindowList1.name: scrolledWindowList1
*scrolledWindowList1.parent: studyListForm
*scrolledWindowList1.scrollingPolicy: "application_defined"
*scrolledWindowList1.visualPolicy: "variable"
*scrolledWindowList1.scrollBarDisplayPolicy: "static"
*scrolledWindowList1.shadowThickness: 0
*scrolledWindowList1.isCompound: "true"
*scrolledWindowList1.compoundIcon: "scrllist.xpm"
*scrolledWindowList1.compoundName: "scrolled_List"
*scrolledWindowList1.x: 20
*scrolledWindowList1.y: 60

*wStudyList.class: scrolledList
*wStudyList.static: true
*wStudyList.name: wStudyList
*wStudyList.parent: scrolledWindowList1
*wStudyList.width: 260
*wStudyList.height: 240
*wStudyList.browseSelectionCallback: {\
  XmListCallbackStruct *l;\
  LIST_ITEM *item;\
\
    l = (XmListCallbackStruct *) UxCallbackArg;\
\
    item = LST_Index(&lst_studylist, l->item_position);\
    if (item == NULL)\
	return;\
\
   PreviewStudy(icon_index, icon_file, item->AccessionNumber);\
}
*wStudyList.automaticSelection: "false"

