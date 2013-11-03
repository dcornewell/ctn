! UIMX ascii 2.5 key: 1713                                                      

*selectPrinterDialog.class: selectionBoxDialog
*selectPrinterDialog.gbldecl: /*\
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
** Module Name(s):	loadPrintServerList\
**			create_selectPrinterDialog\
** Author, Date:	Chander Sabharwal\
** Intent:		This module contains the user interface\
**			that displays a list of printers in a dialog\
**			box and allows the user to select one.\
** Last Update:		$Author: smm $, $Date: 1995-04-10 17:31:04 $\
** Source File:		$RCSfile: selectPrinterDialog.i,v $\
** Revision:		$Revision: 1.9 $\
** Status:		$State: Exp $\
*/\
 \
static char rcsid[] = "$Revision: 1.9 $ $RCSfile: selectPrinterDialog.i,v $";\
 \
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
#include "mut.h"\
\
extern DMAN_HANDLE *dmanHandle;\
static *printGroupList = NULL;\
\
#include "format.h"\
\
extern void clearMessage();\
extern void setMessage(char *txt);\
\
extern Widget wFilmInformationForm;\
extern Widget wFilmSessionForm;\
extern Widget wFilmBoxForm;\
extern Widget wMessageTxt;\
extern Widget wStudyListBoard;\
extern Widget  wImageAttribute;\
\
extern DUL_NETWORKKEY *networkKey;\
extern DUL_ASSOCIATIONKEY *associationKey;\
extern DUL_ASSOCIATESERVICEPARAMETERS associationParameters;\
\
extern  char applicationTitle[20];\
extern CTNBOOLEAN associationActive;
*selectPrinterDialog.ispecdecl:
*selectPrinterDialog.funcdecl: swidget create_selectPrinterDialog(swidget UxParent)
*selectPrinterDialog.funcname: create_selectPrinterDialog
*selectPrinterDialog.funcdef: "swidget", "<create_selectPrinterDialog>(%)"
*selectPrinterDialog.argdecl: swidget UxParent;
*selectPrinterDialog.arglist: UxParent
*selectPrinterDialog.arglist.UxParent: "swidget", "%UxParent%"
*selectPrinterDialog.icode:
*selectPrinterDialog.fcode: return(rtrn);\

*selectPrinterDialog.auxdecl: /* loadPrintServerList\
**\
** Purpose:\
**      Search the control table for the set of (print server)\
**	applications that belong in this group and place the\
**	list in a list widget so that one can be selected.\
**\
** Parameter Dictionary:\
**      serverGroup	The name of the group which owns a\
**			set of print servers\
**\
** Return Values:\
**	none\
**\
** Notes:\
**	dmanHandle is passed globally.  It is the handle which\
**	allows access to all management tables.\
** Algorithm:\
*/\
\
void loadPrintServerList(char *serverGroup)\
{\
  CONDITION\
    cond;\
  DMAN_GROUPNAMES\
    g,\
    criteria;\
  char\
    b[256];\
  Widget\
    w;\
\
    if (printGroupList == NULL)\
	printGroupList = LST_Create();\
    if (printGroupList == NULL)\
	return;\
\
/* Search the database for the set of applications in this group.\
*/\
\
    memset(&g, 0, sizeof(g));\
    g.Type = DMAN_K_GROUPNAMES;\
    criteria.Type = DMAN_K_GROUPNAMES;\
    criteria.Flag = DMAN_K_GROUP_GROUP;\
    strcpy(criteria.GroupName, serverGroup);\
    cond = DMAN_Select(&dmanHandle, (DMAN_GENERICRECORD *)&g,\
	(DMAN_GENERICRECORD *)&criteria,\
	printGroupList, NULL, NULL, NULL);\
   if (cond != DMAN_NORMAL) {\
           return;\
    }\
\
/* Find the list widget from the dialog box and then populate entries\
** using the general function found in the MUT facility.\
*/\
\
    w = XmSelectionBoxGetChild(selectPrinterDialog, XmDIALOG_LIST);\
    MUT_LoadList(w, printGroupList, formatGroupTitle, b);\
}\
\

*selectPrinterDialog.static: true
*selectPrinterDialog.name: selectPrinterDialog
*selectPrinterDialog.parent: NO_PARENT
*selectPrinterDialog.parentExpression: UxParent
*selectPrinterDialog.defaultShell: topLevelShell
*selectPrinterDialog.width: 360
*selectPrinterDialog.height: 210
*selectPrinterDialog.dialogType: "dialog_selection"
*selectPrinterDialog.isCompound: "true"
*selectPrinterDialog.compoundIcon: "selectD.xpm"
*selectPrinterDialog.compoundName: "selBox_Dialog"
*selectPrinterDialog.x: 510
*selectPrinterDialog.y: 34
*selectPrinterDialog.unitType: "pixels"
*selectPrinterDialog.applyCallback: {\
  Widget w;\
  char buf[256];\
  CONDITION cond;\
  CTNBOOLEAN nullFlag;\
\
    w = XmSelectionBoxGetChild(selectPrinterDialog, XmDIALOG_TEXT);\
\
    cond = MUT_ScanWidget(w, MUT_TEXT, &nullFlag, buf);\
    if (cond != MUT_NORMAL || nullFlag)\
	return;\
\
\
}
*selectPrinterDialog.okCallback: {\
/* This callback function is invoked when the user selects the\
** OK button when selecting one print server.  We use the\
** application title of that server to request an association.\
** If successful, we instantiate two more dialog boxes which\
** allow the user to obtain printer status and to manage a\
** film session (and then we are done.\
*/\
\
  Widget w;\
  char buf[256], errBuf[1024];\
  CONDITION cond;\
  CTNBOOLEAN nullFlag;\
\
    w = XmSelectionBoxGetChild(selectPrinterDialog, XmDIALOG_TEXT);\
    cond = MUT_ScanWidget(w, MUT_TEXT, &nullFlag, buf);\
    if (cond != MUT_NORMAL || nullFlag)\
	return;\
\
\
    cond = requestPrintAssociation(applicationTitle, buf, &dmanHandle,\
	&networkKey, &associationKey, &associationParameters);\
    if (cond != 1) {\
	associationActive = FALSE;\
	COND_CopyText(errBuf, sizeof(errBuf));\
	setMessage(errBuf);\
	COND_DumpConditions();\
	return;\
    }\
\
    associationActive = TRUE;\
#ifdef RUNTIME\
    if (wFilmInformationForm == 0)\
	wFilmInformationForm = (Widget)create_filmInformationForm(NULL);\
    if (wFilmSessionForm == 0)\
	wFilmSessionForm = (Widget)create_filmSessionForm(NULL);\
#endif\
    XtManageChild(wFilmInformationForm);\
    XtManageChild(wFilmSessionForm);\
}

