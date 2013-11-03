! UIMX ascii 2.5 key: 3277                                                      

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
**                     Electronic Radiology Laboratory\
**                   Mallinckrodt Institute of Radiology\
**                Washington University School of Medicine\
**\
** Module Name(s):        main\
**                        openfile\
**                        format\
**                        browseSelectionCB_scrolledList1\
**                        browseSelectionCB_scrolledList2\
**                        browseSelectionCB_scrolledList3\
**\
** Author, Date:          Chander L. Sabharwal, 27-June-94\
**\
** Intent:                This program lets the user view all the data-\
**                        items in the patient data file.  \
**\
** Last Updates:	  $Author: smm $, $Date: 1995-12-19 21:58:34 $\
**\
** Source File:           $RCSfile: topLevelShell1.i,v $\
**\
** Revision:              $Revision: 1.5 $\
**             \
** Status:		  $State: Exp $\
*/\
\
static char rcsid[] = "$Revision: 1.5 $ $RCSfile: topLevelShell1.i,v $";\
\
#include <stdio.h>\
\
#include "dicom.h"\
#include "condition.h"\
#include "lst.h"\
#include "mut.h"\
\
#include "dicom_uids.h"\
#include "dicom_objects.h"\
#include "dicom_ie.h"\
#include "dicom_messages.h"\
\
#include "format.h"\
\
extern swidget  fileSelectionBoxDialog1;\
swidget create_fileSelectionBoxDialog1();\
\
void  openfile();\
void  copyWtext();\
\
CTNBOOLEAN		\
         errorstackP(unsigned long v, char *mes); 	\
\
CTNBOOLEAN					/* to clear the stack\
					 * of errors */\
         clearStack = TRUE;\
\
/* byte order in data stream */\
unsigned  long  options = DCM_ORDERLITTLEENDIAN;\
\
/* Handle to the information object */\
DCM_OBJECT    *queryObject = NULL;\
\
/* Handle to the IE_OBJECT object */\
IE_OBJECT     *ieObject = NULL;\
\
/* Handle to IE_MODULE */\
IE_MODULE     *ieModule = NULL;\
\
/* Handle to IE_ATTRIBUTE */\
IE_ATTRIBUTE  *ieAttr = NULL;\
\
/* Handle to LST_HEAD */\
LST_HEAD *ie_head,  *mod_head, *attr_head;\
\
/* Handle to the IE_INFORMATIONENTITY */\
IE_INFORMATIONENTITY    *ieIE = NULL;\
\
/* Handle to the DCM_ELEMENT */\
DCM_ELEMENT  element;\
\
/**char      info[500];**/\
char      *info;\
int         kount=0;\

*topLevelShell1.ispecdecl:
*topLevelShell1.funcdecl: swidget create_topLevelShell1(swidget UxParent)
*topLevelShell1.funcname: create_topLevelShell1
*topLevelShell1.funcdef: "swidget", "<create_topLevelShell1>(%)"
*topLevelShell1.argdecl: swidget UxParent;
*topLevelShell1.arglist: UxParent
*topLevelShell1.arglist.UxParent: "swidget", "%UxParent%"
*topLevelShell1.icode:
*topLevelShell1.fcode: create_fileSelectionBoxDialog1(NO_PARENT);\
return(rtrn);\

*topLevelShell1.auxdecl: /* openfile\
**\
** Purpose:\
**      This subroutine opens DICOM files that contain DICOM information\
**      object, examines the object for information entity(IE),\
**      and calls a function to load the information entity list \
**      in scrolledList1(window) for viewing and selection. It also\
**      writes the selected patient-data filename in a text widget.\
**\
** Parameter Dictionary:\
**      filename       input, DICOM filename to be viewed\
**\
** Return Values:\
**      list of information entity loaded in scrolledList1\
**\
** Notes:\
**\
** Algorithm:\
**      Description of the algorithm (optional) and any other notes.\
*/\
\
void  openfile(char *filename)\
{\
 char  buff[80], bname[80];\
 int    nitems;\
 int     cnt=0;\
 XmString    *strlist;\
 CONDITION  cond;\
\
\
/**memset(info, 0 ,sizeof(info));\
copyWtext(info);**/\
\
/* show selected patient filename in text widget */\
  sprintf(bname, "File Selected:  %s",filename);\
  XmTextSetString(text3,bname);\
\
/* clear the scrolledlist windows, befor opening a new file*/\
   XtVaGetValues(scrolledList1,\
                           XmNitemCount, &cnt,\
                           XmNitems, &strlist,\
                           NULL);\
  if(cnt != 0) {\
    XmListDeleteAllItems(scrolledList1);\
/*lint -e64*/\
    XtFree(strlist);\
/*lint +e64*/\
  }\
\
  XtVaGetValues(scrolledList2, \
                         XmNitemCount, &cnt,\
                         XmNitems, &strlist,\
                         NULL);\
  if(cnt != 0) {\
    XmListDeleteAllItems(scrolledList2);\
/*lint -e64*/\
    XtFree(strlist);\
/*lint +e64*/\
  }\
   XtVaGetValues(scrolledList3,\
                         XmNitemCount, &cnt,\
                         XmNitems, &strlist,\
                         NULL);\
  if(cnt != 0) {\
    XmListDeleteAllItems(scrolledList3);\
/*lint -e64*/\
    XtFree(strlist);\
/*lint +e64*/\
  }\
\
/* Clear text widgets */\
  if(strlen(XmTextGetString(text1) )!= 0)\
    XmTextSetString(text1,NULL);\
  if(strlen(XmTextGetString(text2) )!= 0)\
    XmTextSetString(text2,NULL);\
\
  if(strlen(XmTextGetString(scrolledText1)) != 0)\
    XmTextSetString(scrolledText1,NULL);\
\
  if(ieObject != NULL)  {\
    cond =  IE_Free((void **)&ieObject);\
    if(cond != IE_NORMAL) {\
      (void) COND_ExtractConditions(errorstackP);\
      copyWtext(info);\
      (void) COND_PopCondition(clearStack);\
      free(info);\
    }\
  }\
  free(ieObject);\
\
  if(ieIE != NULL) {\
    cond = IE_Free((void **)&ieIE);\
    if(cond != IE_NORMAL) {\
      (void) COND_ExtractConditions(errorstackP);\
      copyWtext(info);\
      (void) COND_PopCondition(clearStack);\
      free(info);\
    }\
  }\
\
  free(ieIE);\
  if(ieModule != NULL) {\
    cond =  IE_Free((void **)&ieModule);\
    if(cond != IE_NORMAL) {\
      (void) COND_ExtractConditions(errorstackP);\
      copyWtext(info);\
      (void) COND_PopCondition(clearStack);\
      free(info);\
    }\
  }\
  free(ieModule);\
\
  if(ieAttr != NULL) {\
    cond = IE_Free((void **)&ieAttr);\
    if(cond != IE_NORMAL) {\
      (void) COND_ExtractConditions(errorstackP);\
      copyWtext(info);\
      (void) COND_PopCondition(clearStack);\
      free(info);\
    }\
  }\
  free(ieAttr);\
\
  if(queryObject != NULL) {\
    cond = DCM_CloseObject(&queryObject);\
    if(cond != DCM_NORMAL) {\
      (void) COND_ExtractConditions(errorstackP);\
      copyWtext(info);\
      (void) COND_PopCondition(clearStack);\
      free(info);\
    }\
  }\
  free(queryObject);\
\
  kount  = kount + 1;\
  printf(" %d, %s\n", kount, filename);\
\
  cond = DCM_OpenFile(filename, options, &queryObject);\
  if(cond != DCM_NORMAL) {\
    (void) COND_ExtractConditions(errorstackP);\
    copyWtext(info);\
    (void) COND_PopCondition(clearStack);\
    free(info);\
  } else {\
    cond = IE_ExamineObject(&queryObject, &ieObject);\
    if(cond != IE_NORMAL) {\
      (void) COND_ExtractConditions(errorstackP);\
      copyWtext(info);\
      (void) COND_PopCondition(clearStack);\
      free(info);\
    }\
\
    ie_head = ieObject->ieList;\
 \
    MUT_LoadList(scrolledList1, ie_head, formatieList, buff);\
\
  }\
  free(filename); \
}\
\
\
\
CTNBOOLEAN\
errorstackP(unsigned long v, char *mes)\
{\
 \
 char  buff[20];\
\
  if( ( info = (char *) malloc(500)) == NULL)\
    printf(" malloc info failed\n");\
\
  sprintf(buff, "%8x", v);\
  printf(" %s\n",mes);\
  strcat(info, buff);\
  strcat(info, "        ");\
  strcat(info, mes);\
  strcat(info, "\n");\
  return(TRUE);\
} \
\
\
\
void  copyWtext(char  *ptr)\
{\
 XmTextSetString(scrolledText1, ptr);\
}\

*topLevelShell1.static: true
*topLevelShell1.name: topLevelShell1
*topLevelShell1.parent: NO_PARENT
*topLevelShell1.parentExpression: UxParent
*topLevelShell1.width: 1000
*topLevelShell1.height: 662
*topLevelShell1.isCompound: "true"
*topLevelShell1.compoundIcon: "toplevelS.xpm"
*topLevelShell1.compoundName: "topLevel_Shell"
*topLevelShell1.x: 49
*topLevelShell1.y: 161
*topLevelShell1.iconName: "OBJECT-VIEWER"

*mainWindow1.class: mainWindow
*mainWindow1.static: true
*mainWindow1.name: mainWindow1
*mainWindow1.parent: topLevelShell1
*mainWindow1.width: 1000
*mainWindow1.height: 662
*mainWindow1.isCompound: "true"
*mainWindow1.compoundIcon: "main.xpm"
*mainWindow1.compoundName: "main_Window"
*mainWindow1.compoundEditor: {\
extern swidget UxGUIMePopup UXPROTO((swidget, swidget, int, int));\
UxGUIMwePopup(UxThisWidget, UxGetParent(UxThisWidget));\
}
*mainWindow1.x: 0
*mainWindow1.y: 0
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

*menu1_p1_b1.class: separator
*menu1_p1_b1.static: true
*menu1_p1_b1.name: menu1_p1_b1
*menu1_p1_b1.parent: menu1_p1

*menu1_p1_b2.class: pushButton
*menu1_p1_b2.static: true
*menu1_p1_b2.name: menu1_p1_b2
*menu1_p1_b2.parent: menu1_p1
*menu1_p1_b2.labelString: "Open"
*menu1_p1_b2.mnemonic: "O"
*menu1_p1_b2.activateCallback: UxPopupInterface(fileSelectionBoxDialog1,no_grab);

*menu1_p1_b3.class: separator
*menu1_p1_b3.static: true
*menu1_p1_b3.name: menu1_p1_b3
*menu1_p1_b3.parent: menu1_p1

*menu1_p1_b4.class: pushButton
*menu1_p1_b4.static: true
*menu1_p1_b4.name: menu1_p1_b4
*menu1_p1_b4.parent: menu1_p1
*menu1_p1_b4.labelString: "Quit"
*menu1_p1_b4.mnemonic: "Q"
*menu1_p1_b4.activateCallback: /* activateCP_menu1_p1_b4\
**\
** Purpose:\
**   This subroutine closes all the open structures and dicom Object\
**   before quitting the program\
**\
** Parameter Dictinary:\
**      none\
** Return Value:\
**      none\
**\
** Notes:\
**\
** Algorithm:\
**      Description of the algorithm (optional) and ant other notes.\
**\
*/\
\
  \
if(ieIE != NULL)\
  (void) IE_Free((void **)&ieIE);\
/**  cond = IE_Free(ieIE);\
  if(cond != IE_NORMAL)\
    (void) COND_DumpConditions();**/\
\
if(ieModule != NULL)\
  (void) IE_Free((void **)&ieModule);\
/**  cond = IE_Free(ieModule);\
  if(cond != IE_NORMAL)\
    (void) COND_DumpConditions();**/\
\
if(ieAttr != NULL)\
  (void) IE_Free((void **)&ieAttr);\
/**  cond = IE_Free(ieAttr);\
  if(cond != IE_NORMAL)\
    (void) COND_DumpConditions();**/\
\
if(ieObject != NULL)\
  (void) IE_Free((void **)&ieObject);\
/**  cond = IE_Free(ieObject);\
  if(cond != IE_NORMAL)\
    (void) COND_DumpConditions();**/\
\
(void) DCM_CloseObject(&queryObject);\
\
exit(0);

*menu1_top_b1.class: cascadeButton
*menu1_top_b1.static: true
*menu1_top_b1.name: menu1_top_b1
*menu1_top_b1.parent: menu1
*menu1_top_b1.labelString: "File"
*menu1_top_b1.mnemonic: "F"
*menu1_top_b1.subMenuId: "menu1_p1"
*menu1_top_b1.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"

*bulletinBoard1.class: bulletinBoard
*bulletinBoard1.static: true
*bulletinBoard1.name: bulletinBoard1
*bulletinBoard1.parent: mainWindow1

*label1.class: label
*label1.static: true
*label1.name: label1
*label1.parent: bulletinBoard1
*label1.isCompound: "true"
*label1.compoundIcon: "label.xpm"
*label1.compoundName: "label_"
*label1.x: 10
*label1.y: 80
*label1.width: 176
*label1.height: 28
*label1.labelString: "INFORMATION ENTITY"
*label1.fontList: "-adobe-helvetica-bold-r-normal--12-120-75-75-p-70-iso8859-1"

*label2.class: label
*label2.static: true
*label2.name: label2
*label2.parent: bulletinBoard1
*label2.isCompound: "true"
*label2.compoundIcon: "label.xpm"
*label2.compoundName: "label_"
*label2.x: 320
*label2.y: 80
*label2.width: 88
*label2.height: 24
*label2.labelString: "MODULES"
*label2.fontList: "-adobe-helvetica-bold-r-normal--12-120-75-75-p-70-iso8859-1"

*scrolledWindowList1.class: scrolledWindow
*scrolledWindowList1.static: true
*scrolledWindowList1.name: scrolledWindowList1
*scrolledWindowList1.parent: bulletinBoard1
*scrolledWindowList1.scrollingPolicy: "application_defined"
*scrolledWindowList1.visualPolicy: "variable"
*scrolledWindowList1.scrollBarDisplayPolicy: "static"
*scrolledWindowList1.shadowThickness: 0
*scrolledWindowList1.isCompound: "true"
*scrolledWindowList1.compoundIcon: "scrllist.xpm"
*scrolledWindowList1.compoundName: "scrolled_List"
*scrolledWindowList1.x: 10
*scrolledWindowList1.y: 110

*scrolledList1.class: scrolledList
*scrolledList1.static: true
*scrolledList1.name: scrolledList1
*scrolledList1.parent: scrolledWindowList1
*scrolledList1.width: 280
*scrolledList1.height: 324
*scrolledList1.x: 0
*scrolledList1.y: 110
*scrolledList1.browseSelectionCallback: {\
/* scrolledList1\
**\
** Purpose:\
**   This subrouitne examines the selected information entity(IE),\
**   finds a pointer to module-list structure and loads the list of \
**   modules in scrolledList2(window).\
**  \
** Parameter Dictionary:\
**      cbs             input, pointer to the selected IE structure\
**\
** Return Value:\
**      list of modules loaded in the scrolledList2\
**\
** Notes:\
**\
** Algorithm:\
**      Description of the algorithm (optional) and any other notes.\
**\
*/\
XmListCallbackStruct    *cbs;\
\
char   *iename;\
int       i;\
char    buff[80];\
\
CONDITION    cond;\
\
 int  cnt3 = 0;\
 XmString  *strlist3;\
\
  cbs = (XmListCallbackStruct *)UxCallbackArg;\
\
\
/* if selected a new item from List1(IE), then clear list3 */\
\
  XtVaGetValues(scrolledList2,\
                         XmNitemCount, &cnt3,\
                         XmNitems,&strlist3,\
                         NULL);\
\
  if(cnt3 != 0) {\
    XmListDeleteAllItems(scrolledList2);\
/*lint -e64*/\
    XtFree(strlist3);\
/*lint +e64*/\
  }\
\
  XtVaGetValues(scrolledList3,\
                         XmNitemCount, &cnt3,\
                         XmNitems, &strlist3,\
                         NULL);\
\
  if(cnt3 != 0) {\
    XmListDeleteAllItems(scrolledList3);\
/*lint -e64*/\
   XtFree(strlist3);\
/*lint +e64*/\
  }\
\
/* clear any previosly open structure IE */\
  if(ieIE != NULL) {\
    cond =  IE_Free((void **)&ieIE);\
    if(cond != IE_NORMAL) {\
      (void) COND_ExtractConditions(errorstackP);\
      copyWtext(info);\
      (void) COND_PopCondition(clearStack);\
      free(info);\
    }\
  }\
  free(ieIE);\
\
/* clear text widgets */\
  if(strlen(XmTextGetString(text1)) != 0)\
    XmTextSetString(text1,NULL);\
  if(strlen(XmTextGetString(text2)) != 0)\
    XmTextSetString(text2,NULL);\
\
/* pointer to the head-end of the list, ieList */\
  ieIE = LST_Head(&ie_head);\
/* makes current the node */\
  (void)LST_Position(&ie_head,ieIE);\
\
/* pointer to the next item  */\
  for(i = 2; i <= cbs->item_position; i++)\
    ieIE = LST_Next(&ie_head);\
\
  cond = IE_ExamineInformationEntity(&queryObject, ieIE->ieType, &ieIE);\
  if(cond != IE_NORMAL) {\
    (void) COND_ExtractConditions(errorstackP);\
    copyWtext(info);\
    (void) COND_PopCondition(clearStack);\
    free(info);\
  }\
\
  mod_head = ieIE->moduleList;\
\
  MUT_LoadList(scrolledList2, ieIE->moduleList, formatmodieList, buff);\
\
\
\
}
*scrolledList1.fontList: "-adobe-helvetica-bold-r-normal--14-100-100-100-p-82-iso8859-1"

*scrolledWindowList2.class: scrolledWindow
*scrolledWindowList2.static: true
*scrolledWindowList2.name: scrolledWindowList2
*scrolledWindowList2.parent: bulletinBoard1
*scrolledWindowList2.scrollingPolicy: "application_defined"
*scrolledWindowList2.visualPolicy: "variable"
*scrolledWindowList2.scrollBarDisplayPolicy: "static"
*scrolledWindowList2.shadowThickness: 0
*scrolledWindowList2.isCompound: "true"
*scrolledWindowList2.compoundIcon: "scrllist.xpm"
*scrolledWindowList2.compoundName: "scrolled_List"
*scrolledWindowList2.x: 310
*scrolledWindowList2.y: 110

*scrolledList2.class: scrolledList
*scrolledList2.static: true
*scrolledList2.name: scrolledList2
*scrolledList2.parent: scrolledWindowList2
*scrolledList2.width: 316
*scrolledList2.height: 324
*scrolledList2.x: 0
*scrolledList2.y: 110
*scrolledList2.browseSelectionCallback: {\
/* scrolledList2\
**\
** Purpose:\
**   This subroutine examines the selected module, finds a pointer \
**   to the atrribute-list structure and loads the list of attributes\
**   in scrolledList3(window)\
**\
** Parameter Dictinary:\
**      cbs            input, pointer to the selected module structure\
**\
** Return Value:\
**      list of attributes loaded in scrolledList3\
**\
** Notes:\
** \
** Algorithm:\
**      Description of the algorithm (optional) and any other notes.\
**\
*/\
XmListCallbackStruct    *cbs;\
\
char    *modname;\
char     buff[80];\
int        i;\
int        cnt=0;\
XmString  *strlist;\
\
CONDITION    cond;\
\
  cbs = (XmListCallbackStruct *)UxCallbackArg;\
\
/* clear scrolledlist3 */\
  XtVaGetValues(scrolledList3,\
                        XmNitemCount, &cnt,\
                        XmNitems, &strlist,\
                        NULL);\
  if(cnt != 0) {\
    XmListDeleteAllItems(scrolledList3);\
/*lint -e64*/\
    XtFree(strlist);\
/*lint +e64*/\
  }\
\
  if(strlen(XmTextGetString(text1)) != 0)\
    XmTextSetString(text1,NULL);\
  if(strlen(XmTextGetString(text2)) != 0)\
    XmTextSetString(text2,NULL);\
\
/* if any previous module structure is open, free it */\
  if(ieModule != NULL) {\
    cond = IE_Free((void **)&ieModule);\
    if(cond != IE_NORMAL) {\
      (void) COND_ExtractConditions(errorstackP);\
      copyWtext(info);\
      (void) COND_PopCondition(clearStack);\
      free(info);\
    }\
  }\
  free(ieModule);\
\
/* load scrolledList3 with attributeList */\
 \
  ieModule = LST_Head(&mod_head);\
  (void)LST_Position(&mod_head,ieModule);\
  for(i=2; i<= cbs->item_position; i++)\
    ieModule = LST_Next(&mod_head);\
\
  cond = IE_ExamineModule(&queryObject, ieIE->ieType, ieModule->moduleType, &ieModule);\
  if(cond != IE_NORMAL) {\
    (void) COND_ExtractConditions(errorstackP);\
    copyWtext(info);\
    (void) COND_PopCondition(clearStack);\
    free(info);\
  }\
  attr_head = ieModule->attributeList;\
\
  MUT_LoadList(scrolledList3, ieModule->attributeList, formatattrieList, buff);\
\
\
}
*scrolledList2.fontList: "-adobe-helvetica-bold-r-normal--14-100-100-100-p-82-iso8859-1"

*label3.class: label
*label3.static: true
*label3.name: label3
*label3.parent: bulletinBoard1
*label3.isCompound: "true"
*label3.compoundIcon: "label.xpm"
*label3.compoundName: "label_"
*label3.x: 20
*label3.y: 440
*label3.width: 92
*label3.height: 28
*label3.labelString: "ATTRIBUTE "
*label3.fontList: "-adobe-helvetica-bold-r-normal--12-120-75-75-p-70-iso8859-1"

*text1.class: text
*text1.static: true
*text1.name: text1
*text1.parent: bulletinBoard1
*text1.width: 400
*text1.isCompound: "true"
*text1.compoundIcon: "text.xpm"
*text1.compoundName: "text_"
*text1.x: 10
*text1.y: 470
*text1.height: 70
*text1.fontList: "-adobe-helvetica-bold-r-normal--14-100-100-100-p-82-iso8859-1"

*scrolledWindowList3.class: scrolledWindow
*scrolledWindowList3.static: true
*scrolledWindowList3.name: scrolledWindowList3
*scrolledWindowList3.parent: bulletinBoard1
*scrolledWindowList3.scrollingPolicy: "application_defined"
*scrolledWindowList3.visualPolicy: "variable"
*scrolledWindowList3.scrollBarDisplayPolicy: "static"
*scrolledWindowList3.shadowThickness: 0
*scrolledWindowList3.isCompound: "true"
*scrolledWindowList3.compoundIcon: "scrllist.xpm"
*scrolledWindowList3.compoundName: "scrolled_List"
*scrolledWindowList3.x: 650
*scrolledWindowList3.y: 110

*scrolledList3.class: scrolledList
*scrolledList3.static: true
*scrolledList3.name: scrolledList3
*scrolledList3.parent: scrolledWindowList3
*scrolledList3.width: 304
*scrolledList3.height: 324
*scrolledList3.browseSelectionCallback: {\
/* scrolledList3\
**\
** Purpose:\
**   This subroutine lists the description of the selected attribute\
**   in the text widgets.\
**\
** Parameter Dictinary:\
**      cbs            input, pointer to the selected attribute\
**\
** Return Value:\
**      Description of the element\
**\
** Notes:\
**\
** Algorithm:\
**      Description of the algorithm (optional) and any other notes.\
**\
*/\
\
XmListCallbackStruct     *cbs;\
\
char          *atname;\
int              i;\
char           buf2[64], buf1[64];\
\
CONDITION    cond;\
\
unsigned short   gg,ee;\
\
  cbs = (XmListCallbackStruct *)UxCallbackArg;\
\
  if( (atname = (char *) malloc(64)) == NULL)\
    printf(" malloc atname failed\n");\
\
  XmStringGetLtoR(cbs->item, XmSTRING_DEFAULT_CHARSET, &atname);\
\
\
  if(strlen(XmTextGetString(text1)) != 0)\
    XmTextSetString(text1,NULL);\
  if(strlen(XmTextGetString(text2)) != 0)\
    XmTextSetString(text2,NULL);\
\
  ieAttr = LST_Head(&attr_head);\
  (void)LST_Position(&attr_head,ieAttr);\
  for(i=2; i<= cbs->item_position; i++)\
    ieAttr = LST_Next(&attr_head);\
\
  cond = DCM_LookupElement(&ieAttr->element);\
  if(cond != DCM_NORMAL) {\
    (void) COND_ExtractConditions(errorstackP);\
    copyWtext(info);\
    (void) COND_PopCondition(clearStack);\
    free(info);\
  }\
\
  sprintf(buf1, " %s", atname);\
  XmTextSetString(text1,buf1);\
\
  if(ieAttr->element.length == 0) {\
    strcpy(buf2, "<None>");\
    XmTextSetString(text2,buf2);\
  } else {\
    switch(ieAttr->element.representation)\
    {\
 \
    case DCM_AS:		/* Age String */\
    case DCM_CS:		/* control string */\
    case DCM_DA:		/* date */\
    case DCM_DS:		/* decimal string */\
    case DCM_IS:		/* integer string */   \
    case DCM_LO:		/* long string */\
    case DCM_LT:		/* long text */		\
    case DCM_ST:		/* short text */\
    case DCM_SH:		/* short string */\
    case DCM_TM:		/* time */\
    case DCM_UI:		/* uid */\
    case DCM_PN:		/* person name */\
 \
      gg = DCM_TAG_GROUP(ieAttr->element.tag);\
      ee = DCM_TAG_ELEMENT(ieAttr->element.tag);\
\
      sprintf(buf2, "%04x,  %04x,   %s", gg, ee,  ieAttr->element.d.string);\
      XmTextSetString(text2,buf2);\
 \
      break;\
 \
    case DCM_SS:		/* signed short */\
      gg = DCM_TAG_GROUP(ieAttr->element.tag);\
      ee = DCM_TAG_ELEMENT(ieAttr->element.tag);\
      sprintf(buf2, "%04x,  %04x,  %d",  gg, ee, *(ieAttr->element.d.us));\
      XmTextSetString(text2,buf2);\
      break;\
\
    case DCM_SL:		/* signed long */\
      gg = DCM_TAG_GROUP(ieAttr->element.tag);\
      ee = DCM_TAG_ELEMENT(ieAttr->element.tag);\
      sprintf(buf2, " %04x,  %04x,  %d",  gg, ee, *(ieAttr->element.d.sl));\
      XmTextSetString(text2,buf2);\
      break;\
\
    case DCM_US:		/* unsigned short */\
      gg = DCM_TAG_GROUP(ieAttr->element.tag);\
      ee = DCM_TAG_ELEMENT(ieAttr->element.tag);\
      sprintf(buf2, " %04x, %04x, %d", gg, ee, *(ieAttr->element.d.us));\
      XmTextSetString(text2,buf2);\
      break;\
\
    case DCM_UL:		/* unsigned long */\
      gg = DCM_TAG_GROUP(ieAttr->element.tag);\
      ee = DCM_TAG_ELEMENT(ieAttr->element.tag);\
      sprintf(buf2, " %04x, %04x, %d", gg, ee,  *(ieAttr->element.d.ul));\
      XmTextSetString(text2,buf2);\
      break;\
 \
    default:\
      break;\
    }\
  }\
  free(atname);\
\
}
*scrolledList3.x: 0
*scrolledList3.y: 110
*scrolledList3.fontList: "-adobe-helvetica-bold-r-normal--14-100-100-100-p-82-iso8859-1"

*label4.class: label
*label4.static: true
*label4.name: label4
*label4.parent: bulletinBoard1
*label4.isCompound: "true"
*label4.compoundIcon: "label.xpm"
*label4.compoundName: "label_"
*label4.x: 660
*label4.y: 80
*label4.width: 104
*label4.height: 24
*label4.labelString: "ATTRIBUTES"
*label4.fontList: "-adobe-helvetica-bold-r-normal--12-120-75-75-p-70-iso8859-1"

*text2.class: text
*text2.static: true
*text2.name: text2
*text2.parent: bulletinBoard1
*text2.width: 560
*text2.isCompound: "true"
*text2.compoundIcon: "text.xpm"
*text2.compoundName: "text_"
*text2.x: 410
*text2.y: 470
*text2.height: 70
*text2.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"
*text2.wordWrap: "false"

*label5.class: label
*label5.static: true
*label5.name: label5
*label5.parent: bulletinBoard1
*label5.isCompound: "true"
*label5.compoundIcon: "label.xpm"
*label5.compoundName: "label_"
*label5.x: 420
*label5.y: 440
*label5.width: 196
*label5.height: 28
*label5.labelString: "ATTRIBUTE INFORMATION"
*label5.fontList: "-adobe-helvetica-bold-r-normal--12-120-75-75-p-70-iso8859-1"

*scrolledWindowText1.class: scrolledWindow
*scrolledWindowText1.static: true
*scrolledWindowText1.name: scrolledWindowText1
*scrolledWindowText1.parent: bulletinBoard1
*scrolledWindowText1.scrollingPolicy: "application_defined"
*scrolledWindowText1.visualPolicy: "variable"
*scrolledWindowText1.scrollBarDisplayPolicy: "static"
*scrolledWindowText1.isCompound: "true"
*scrolledWindowText1.compoundIcon: "scrltext.xpm"
*scrolledWindowText1.compoundName: "scrolled_Text"
*scrolledWindowText1.x: 30
*scrolledWindowText1.y: 550

*scrolledText1.class: scrolledText
*scrolledText1.static: true
*scrolledText1.name: scrolledText1
*scrolledText1.parent: scrolledWindowText1
*scrolledText1.width: 890
*scrolledText1.height: 50
*scrolledText1.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"

*text3.class: text
*text3.static: true
*text3.name: text3
*text3.parent: bulletinBoard1
*text3.width: 880
*text3.isCompound: "true"
*text3.compoundIcon: "text.xpm"
*text3.compoundName: "text_"
*text3.x: 90
*text3.y: 20
*text3.height: 40
*text3.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"

