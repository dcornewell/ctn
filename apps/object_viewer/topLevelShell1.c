
/*******************************************************************************
	topLevelShell1.c

       Associated Header file: topLevelShell1.h
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/Text.h>
#include <Xm/Text.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/Label.h>
#include <Xm/BulletinB.h>
#include <Xm/CascadeB.h>
#include <Xm/PushB.h>
#include <Xm/Separator.h>
#include <Xm/RowColumn.h>
#include <Xm/MainW.h>
#include <X11/Shell.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
          Copyright (C) 1993, 1994, RSNA and Washington University

          The software and supporting documentation for the Radiological
          Society of North America (RSNA) 1993, 1994 Digital Imaging and
          Communications in Medicine (DICOM) Demonstration were developed
          at the
                  Electronic Radiology Laboratory
                  Mallinckrodt Institute of Radiology
                  Washington University School of Medicine
                  510 S. Kingshighway Blvd.
                  St. Louis, MO 63110
          as part of the 1993, 1994 DICOM Central Test Node project for, and
          under contract with, the Radiological Society of North America.

          THIS SOFTWARE IS MADE AVAILABLE, AS IS, AND NEITHER RSNA NOR
          WASHINGTON UNIVERSITY MAKE ANY WARRANTY ABOUT THE SOFTWARE, ITS
          PERFORMANCE, ITS MERCHANTABILITY OR FITNESS FOR ANY PARTICULAR
          USE, FREEDOM FROM ANY COMPUTER DISEASES OR ITS CONFORMITY TO ANY
          SPECIFICATION. THE ENTIRE RISK AS TO QUALITY AND PERFORMANCE OF
          THE SOFTWARE IS WITH THE USER.

          Copyright of the software and supporting documentation is
          jointly owned by RSNA and Washington University, and free access
          is hereby granted as a license to use this software, copy this
          software and prepare derivative works based upon this software.
          However, any distribution of this software source code or
          supporting documentation or derivative works (source code and
          supporting documentation) must include the three paragraphs of
          the copyright notice.
*/
/* Copyright marker.  Copyright will be inserted above.  Do not remove */

/*
**                     Electronic Radiology Laboratory
**                   Mallinckrodt Institute of Radiology
**                Washington University School of Medicine
**
** Module Name(s):        main
**                        openfile
**                        format
**                        browseSelectionCB_scrolledList1
**                        browseSelectionCB_scrolledList2
**                        browseSelectionCB_scrolledList3
**
** Author, Date:          Chander L. Sabharwal, 27-June-94
**
** Intent:                This program lets the user view all the data-
**                        items in the patient data file.
**
** Last Updates:	  $Author: smm $, $Date: 1995-12-19 21:58:31 $
**
** Source File:           $RCSfile: topLevelShell1.c,v $
**
** Revision:              $Revision: 1.4 $
**
** Status:		  $State: Exp $
*/

static char rcsid[] = "$Revision: 1.4 $ $RCSfile: topLevelShell1.c,v $";


#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "mut.h"

#include "dicom_uids.h"
#include "dicom_objects.h"
#include "dicom_ie.h"
#include "dicom_messages.h"

#include "format.h"

extern swidget fileSelectionBoxDialog1;
swidget create_fileSelectionBoxDialog1();

void openfile();
void copyWtext();

CTNBOOLEAN
errorstackP(unsigned long v, char *mes);

CTNBOOLEAN			/* to clear the stack of errors */
clearStack = TRUE;

/* byte order in data stream */
unsigned long options = DCM_ORDERLITTLEENDIAN;

/* Handle to the information object */
DCM_OBJECT *queryObject = NULL;

/* Handle to the IE_OBJECT object */
IE_OBJECT *ieObject = NULL;

/* Handle to IE_MODULE */
IE_MODULE *ieModule = NULL;

/* Handle to IE_ATTRIBUTE */
IE_ATTRIBUTE *ieAttr = NULL;

/* Handle to LST_HEAD */
LST_HEAD *ie_head,
   *mod_head,
   *attr_head;

/* Handle to the IE_INFORMATIONENTITY */
IE_INFORMATIONENTITY *ieIE = NULL;

/* Handle to the DCM_ELEMENT */
DCM_ELEMENT element;

/**char      info[500];**/
char *info;
int kount = 0;


static Widget topLevelShell1;
static Widget mainWindow1;
static Widget menu1;
static Widget menu1_p1;
static Widget menu1_p1_b1;
static Widget menu1_p1_b2;
static Widget menu1_p1_b3;
static Widget menu1_p1_b4;
static Widget menu1_top_b1;
static Widget bulletinBoard1;
static Widget label1;
static Widget label2;
static Widget scrolledWindowList1;
static Widget scrolledList1;
static Widget scrolledWindowList2;
static Widget scrolledList2;
static Widget label3;
static Widget text1;
static Widget scrolledWindowList3;
static Widget scrolledList3;
static Widget label4;
static Widget text2;
static Widget label5;
static Widget scrolledWindowText1;
static Widget scrolledText1;
static Widget text3;
static swidget UxParent;

#define CONTEXT_MACRO_ACCESS 1
#include "topLevelShell1.h"
#undef CONTEXT_MACRO_ACCESS


/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

/* openfile
**
** Purpose:
**      This subroutine opens DICOM files that contain DICOM information
**      object, examines the object for information entity(IE),
**      and calls a function to load the information entity list
**      in scrolledList1(window) for viewing and selection. It also
**      writes the selected patient-data filename in a text widget.
**
** Parameter Dictionary:
**      filename       input, DICOM filename to be viewed
**
** Return Values:
**      list of information entity loaded in scrolledList1
**
** Notes:
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/

void
openfile(char *filename)
{
    char buff[80],
        bname[80];
    int nitems;
    int cnt = 0;
    XmString *strlist;
    CONDITION cond;


/**memset(info, 0 ,sizeof(info));
copyWtext(info);**/

/* show selected patient filename in text widget */
    sprintf(bname, "File Selected:  %s", filename);
    XmTextSetString(text3, bname);

/* clear the scrolledlist windows, befor opening a new file*/
    XtVaGetValues(scrolledList1,
		  XmNitemCount, &cnt,
		  XmNitems, &strlist,
		  NULL);
    if (cnt != 0) {
	XmListDeleteAllItems(scrolledList1);
/*lint -e64*/
	XtFree(strlist);
/*lint +e64*/
    }
    XtVaGetValues(scrolledList2,
		  XmNitemCount, &cnt,
		  XmNitems, &strlist,
		  NULL);
    if (cnt != 0) {
	XmListDeleteAllItems(scrolledList2);
/*lint -e64*/
	XtFree(strlist);
/*lint +e64*/
    }
    XtVaGetValues(scrolledList3,
		  XmNitemCount, &cnt,
		  XmNitems, &strlist,
		  NULL);
    if (cnt != 0) {
	XmListDeleteAllItems(scrolledList3);
/*lint -e64*/
	XtFree(strlist);
/*lint +e64*/
    }
/* Clear text widgets */
    if (strlen(XmTextGetString(text1)) != 0)
	XmTextSetString(text1, NULL);
    if (strlen(XmTextGetString(text2)) != 0)
	XmTextSetString(text2, NULL);

    if (strlen(XmTextGetString(scrolledText1)) != 0)
	XmTextSetString(scrolledText1, NULL);

    if (ieObject != NULL) {
	cond = IE_Free((void **) &ieObject);
	if (cond != IE_NORMAL) {
	    (void) COND_ExtractConditions(errorstackP);
	    copyWtext(info);
	    (void) COND_PopCondition(clearStack);
	    free(info);
	}
    }
    free(ieObject);

    if (ieIE != NULL) {
	cond = IE_Free((void **) &ieIE);
	if (cond != IE_NORMAL) {
	    (void) COND_ExtractConditions(errorstackP);
	    copyWtext(info);
	    (void) COND_PopCondition(clearStack);
	    free(info);
	}
    }
    free(ieIE);
    if (ieModule != NULL) {
	cond = IE_Free((void **) &ieModule);
	if (cond != IE_NORMAL) {
	    (void) COND_ExtractConditions(errorstackP);
	    copyWtext(info);
	    (void) COND_PopCondition(clearStack);
	    free(info);
	}
    }
    free(ieModule);

    if (ieAttr != NULL) {
	cond = IE_Free((void **) &ieAttr);
	if (cond != IE_NORMAL) {
	    (void) COND_ExtractConditions(errorstackP);
	    copyWtext(info);
	    (void) COND_PopCondition(clearStack);
	    free(info);
	}
    }
    free(ieAttr);

    if (queryObject != NULL) {
	cond = DCM_CloseObject(&queryObject);
	if (cond != DCM_NORMAL) {
	    (void) COND_ExtractConditions(errorstackP);
	    copyWtext(info);
	    (void) COND_PopCondition(clearStack);
	    free(info);
	}
    }
    free(queryObject);

    kount = kount + 1;
    printf(" %d, %s\n", kount, filename);

    cond = DCM_OpenFile(filename, options, &queryObject);
    if (cond != DCM_NORMAL) {
	(void) COND_ExtractConditions(errorstackP);
	copyWtext(info);
	(void) COND_PopCondition(clearStack);
	free(info);
    } else {
	cond = IE_ExamineObject(&queryObject, &ieObject);
	if (cond != IE_NORMAL) {
	    (void) COND_ExtractConditions(errorstackP);
	    copyWtext(info);
	    (void) COND_PopCondition(clearStack);
	    free(info);
	}
	ie_head = ieObject->ieList;

	MUT_LoadList(scrolledList1, ie_head, formatieList, buff);

    }
    free(filename);
}



CTNBOOLEAN
errorstackP(unsigned long v, char *mes)
{

    char buff[20];

    if ((info = (char *) malloc(500)) == NULL)
	printf(" malloc info failed\n");

    sprintf(buff, "%8x", v);
    printf(" %s\n", mes);
    strcat(info, buff);
    strcat(info, "        ");
    strcat(info, mes);
    strcat(info, "\n");
    return (TRUE);
}



void
copyWtext(char *ptr)
{
    XmTextSetString(scrolledText1, ptr);
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void
activateCB_menu1_p1_b2(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    UxPopupInterface(fileSelectionBoxDialog1, no_grab);
}

static void
activateCB_menu1_p1_b4(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    /*
     * activateCP_menu1_p1_b4 *
     * 
     * Purpose: *   This subroutine closes all the open structures and dicom
     * Object *   before quitting the program *
     * 
     * Parameter Dictinary: *      none * Return Value: *      none *
     * 
     * Notes: *
     * 
     * Algorithm: *      Description of the algorithm (optional) and ant other
     * notes. *
     * 
     */


    if (ieIE != NULL)
	(void) IE_Free((void **) &ieIE);
    /**  cond = IE_Free(ieIE);
      if(cond != IE_NORMAL)
        (void) COND_DumpConditions();**/

    if (ieModule != NULL)
	(void) IE_Free((void **) &ieModule);
    /**  cond = IE_Free(ieModule);
      if(cond != IE_NORMAL)
        (void) COND_DumpConditions();**/

    if (ieAttr != NULL)
	(void) IE_Free((void **) &ieAttr);
    /**  cond = IE_Free(ieAttr);
      if(cond != IE_NORMAL)
        (void) COND_DumpConditions();**/

    if (ieObject != NULL)
	(void) IE_Free((void **) &ieObject);
    /**  cond = IE_Free(ieObject);
      if(cond != IE_NORMAL)
        (void) COND_DumpConditions();**/

    (void) DCM_CloseObject(&queryObject);

    exit(0);
}

static void
browseSelectionCB_scrolledList1(
				Widget wgt,
				XtPointer cd,
				XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	/*
	 * scrolledList1 *
	 * 
	 * Purpose: *   This subrouitne examines the selected information
	 * entity(IE), *   finds a pointer to module-list structure and loads
	 * the list of *   modules in scrolledList2(window). *
	 * 
	 * Parameter Dictionary: *      cbs             input, pointer to the
	 * selected IE structure *
	 * 
	 * Return Value: *      list of modules loaded in the scrolledList2 *
	 * 
	 * Notes: *
	 * 
	 * Algorithm: *      Description of the algorithm (optional) and any
	 * other notes. *
	 * 
	 */
	XmListCallbackStruct *cbs;

	char *iename;
	int i;
	char buff[80];

	CONDITION cond;

	int cnt3 = 0;
	XmString *strlist3;

	cbs = (XmListCallbackStruct *) UxCallbackArg;


	/* if selected a new item from List1(IE), then clear list3 */

	XtVaGetValues(scrolledList2,
		      XmNitemCount, &cnt3,
		      XmNitems, &strlist3,
		      NULL);

	if (cnt3 != 0) {
	    XmListDeleteAllItems(scrolledList2);
	    /* lint -e64 */
	    XtFree(strlist3);
	    /* lint +e64 */
	}
	XtVaGetValues(scrolledList3,
		      XmNitemCount, &cnt3,
		      XmNitems, &strlist3,
		      NULL);

	if (cnt3 != 0) {
	    XmListDeleteAllItems(scrolledList3);
	    /* lint -e64 */
	    XtFree(strlist3);
	    /* lint +e64 */
	}
	/* clear any previosly open structure IE */
	if (ieIE != NULL) {
	    cond = IE_Free((void **) &ieIE);
	    if (cond != IE_NORMAL) {
		(void) COND_ExtractConditions(errorstackP);
		copyWtext(info);
		(void) COND_PopCondition(clearStack);
		free(info);
	    }
	}
	free(ieIE);

	/* clear text widgets */
	if (strlen(XmTextGetString(text1)) != 0)
	    XmTextSetString(text1, NULL);
	if (strlen(XmTextGetString(text2)) != 0)
	    XmTextSetString(text2, NULL);

	/* pointer to the head-end of the list, ieList */
	ieIE = LST_Head(&ie_head);
	/* makes current the node */
	(void) LST_Position(&ie_head, ieIE);

	/* pointer to the next item  */
	for (i = 2; i <= cbs->item_position; i++)
	    ieIE = LST_Next(&ie_head);

	cond = IE_ExamineInformationEntity(&queryObject, ieIE->ieType, &ieIE);
	if (cond != IE_NORMAL) {
	    (void) COND_ExtractConditions(errorstackP);
	    copyWtext(info);
	    (void) COND_PopCondition(clearStack);
	    free(info);
	}
	mod_head = ieIE->moduleList;

	MUT_LoadList(scrolledList2, ieIE->moduleList, formatmodieList, buff);



    }
}

static void
browseSelectionCB_scrolledList2(
				Widget wgt,
				XtPointer cd,
				XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	/*
	 * scrolledList2 *
	 * 
	 * Purpose: *   This subroutine examines the selected module, finds a
	 * pointer *   to the atrribute-list structure and loads the list of
	 * attributes *   in scrolledList3(window) *
	 * 
	 * Parameter Dictinary: *      cbs            input, pointer to the
	 * selected module structure *
	 * 
	 * Return Value: *      list of attributes loaded in scrolledList3 *
	 * 
	 * Notes: *
	 * 
	 * Algorithm: *      Description of the algorithm (optional) and any
	 * other notes. *
	 * 
	 */
	XmListCallbackStruct *cbs;

	char *modname;
	char buff[80];
	int i;
	int cnt = 0;
	XmString *strlist;

	CONDITION cond;

	cbs = (XmListCallbackStruct *) UxCallbackArg;

	/* clear scrolledlist3 */
	XtVaGetValues(scrolledList3,
		      XmNitemCount, &cnt,
		      XmNitems, &strlist,
		      NULL);
	if (cnt != 0) {
	    XmListDeleteAllItems(scrolledList3);
	    /* lint -e64 */
	    XtFree(strlist);
	    /* lint +e64 */
	}
	if (strlen(XmTextGetString(text1)) != 0)
	    XmTextSetString(text1, NULL);
	if (strlen(XmTextGetString(text2)) != 0)
	    XmTextSetString(text2, NULL);

	/* if any previous module structure is open, free it */
	if (ieModule != NULL) {
	    cond = IE_Free((void **) &ieModule);
	    if (cond != IE_NORMAL) {
		(void) COND_ExtractConditions(errorstackP);
		copyWtext(info);
		(void) COND_PopCondition(clearStack);
		free(info);
	    }
	}
	free(ieModule);

	/* load scrolledList3 with attributeList */

	ieModule = LST_Head(&mod_head);
	(void) LST_Position(&mod_head, ieModule);
	for (i = 2; i <= cbs->item_position; i++)
	    ieModule = LST_Next(&mod_head);

	cond = IE_ExamineModule(&queryObject, ieIE->ieType, ieModule->moduleType, &ieModule);
	if (cond != IE_NORMAL) {
	    (void) COND_ExtractConditions(errorstackP);
	    copyWtext(info);
	    (void) COND_PopCondition(clearStack);
	    free(info);
	}
	attr_head = ieModule->attributeList;

	MUT_LoadList(scrolledList3, ieModule->attributeList, formatattrieList, buff);


    }
}

static void
browseSelectionCB_scrolledList3(
				Widget wgt,
				XtPointer cd,
				XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	/*
	 * scrolledList3 *
	 * 
	 * Purpose: *   This subroutine lists the description of the selected
	 * attribute *   in the text widgets. *
	 * 
	 * Parameter Dictinary: *      cbs            input, pointer to the
	 * selected attribute *
	 * 
	 * Return Value: *      Description of the element *
	 * 
	 * Notes: *
	 * 
	 * Algorithm: *      Description of the algorithm (optional) and any
	 * other notes. *
	 * 
	 */

	XmListCallbackStruct *cbs;

	char *atname;
	int i;
	char buf2[64],
	    buf1[64];

	CONDITION cond;

	unsigned short gg,
	    ee;

	cbs = (XmListCallbackStruct *) UxCallbackArg;

	if ((atname = (char *) malloc(64)) == NULL)
	    printf(" malloc atname failed\n");

	XmStringGetLtoR(cbs->item, XmSTRING_DEFAULT_CHARSET, &atname);


	if (strlen(XmTextGetString(text1)) != 0)
	    XmTextSetString(text1, NULL);
	if (strlen(XmTextGetString(text2)) != 0)
	    XmTextSetString(text2, NULL);

	ieAttr = LST_Head(&attr_head);
	(void) LST_Position(&attr_head, ieAttr);
	for (i = 2; i <= cbs->item_position; i++)
	    ieAttr = LST_Next(&attr_head);

	cond = DCM_LookupElement(&ieAttr->element);
	if (cond != DCM_NORMAL) {
	    (void) COND_ExtractConditions(errorstackP);
	    copyWtext(info);
	    (void) COND_PopCondition(clearStack);
	    free(info);
	}
	sprintf(buf1, " %s", atname);
	XmTextSetString(text1, buf1);

	if (ieAttr->element.length == 0) {
	    strcpy(buf2, "<None>");
	    XmTextSetString(text2, buf2);
	} else {
	    switch (ieAttr->element.representation) {

	    case DCM_AS:	/* Age String */
	    case DCM_CS:	/* control string */
	    case DCM_DA:	/* date */
	    case DCM_DS:	/* decimal string */
	    case DCM_IS:	/* integer string */
	    case DCM_LO:	/* long string */
	    case DCM_LT:	/* long text */
	    case DCM_ST:	/* short text */
	    case DCM_SH:	/* short string */
	    case DCM_TM:	/* time */
	    case DCM_UI:	/* uid */
	    case DCM_PN:	/* person name */

		gg = DCM_TAG_GROUP(ieAttr->element.tag);
		ee = DCM_TAG_ELEMENT(ieAttr->element.tag);

		sprintf(buf2, "%04x,  %04x,   %s", gg, ee, ieAttr->element.d.string);
		XmTextSetString(text2, buf2);

		break;

	    case DCM_SS:	/* signed short */
		gg = DCM_TAG_GROUP(ieAttr->element.tag);
		ee = DCM_TAG_ELEMENT(ieAttr->element.tag);
		sprintf(buf2, "%04x,  %04x,  %d", gg, ee, *(ieAttr->element.d.us));
		XmTextSetString(text2, buf2);
		break;

	    case DCM_SL:	/* signed long */
		gg = DCM_TAG_GROUP(ieAttr->element.tag);
		ee = DCM_TAG_ELEMENT(ieAttr->element.tag);
		sprintf(buf2, " %04x,  %04x,  %d", gg, ee, *(ieAttr->element.d.sl));
		XmTextSetString(text2, buf2);
		break;

	    case DCM_US:	/* unsigned short */
		gg = DCM_TAG_GROUP(ieAttr->element.tag);
		ee = DCM_TAG_ELEMENT(ieAttr->element.tag);
		sprintf(buf2, " %04x, %04x, %d", gg, ee, *(ieAttr->element.d.us));
		XmTextSetString(text2, buf2);
		break;

	    case DCM_UL:	/* unsigned long */
		gg = DCM_TAG_GROUP(ieAttr->element.tag);
		ee = DCM_TAG_ELEMENT(ieAttr->element.tag);
		sprintf(buf2, " %04x, %04x, %d", gg, ee, *(ieAttr->element.d.ul));
		XmTextSetString(text2, buf2);
		break;

	    default:
		break;
	    }
	}
	free(atname);

    }
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget
_Uxbuild_topLevelShell1()
{
    Widget _UxParent;
    Widget menu1_p1_shell;


    /* Creation of topLevelShell1 */
    _UxParent = UxParent;
    if (_UxParent == NULL) {
	_UxParent = UxTopLevel;
    }
    topLevelShell1 = XtVaCreatePopupShell("topLevelShell1",
					  topLevelShellWidgetClass,
					  _UxParent,
					  XmNwidth, 1000,
					  XmNheight, 662,
					  XmNx, 49,
					  XmNy, 161,
					  XmNiconName, "OBJECT-VIEWER",
					  NULL);


    /* Creation of mainWindow1 */
    mainWindow1 = XtVaCreateManagedWidget("mainWindow1",
					  xmMainWindowWidgetClass,
					  topLevelShell1,
					  XmNwidth, 1000,
					  XmNheight, 662,
					  XmNx, 0,
					  XmNy, 0,
					  XmNunitType, XmPIXELS,
					  NULL);


    /* Creation of menu1 */
    menu1 = XtVaCreateManagedWidget("menu1",
				    xmRowColumnWidgetClass,
				    mainWindow1,
				    XmNrowColumnType, XmMENU_BAR,
				    XmNmenuAccelerator, "<KeyUp>F10",
				    NULL);


    /* Creation of menu1_p1 */
    menu1_p1_shell = XtVaCreatePopupShell("menu1_p1_shell",
					  xmMenuShellWidgetClass, menu1,
					  XmNwidth, 1,
					  XmNheight, 1,
					  XmNallowShellResize, TRUE,
					  XmNoverrideRedirect, TRUE,
					  NULL);

    menu1_p1 = XtVaCreateWidget("menu1_p1",
				xmRowColumnWidgetClass,
				menu1_p1_shell,
				XmNrowColumnType, XmMENU_PULLDOWN,
				NULL);


    /* Creation of menu1_p1_b1 */
    menu1_p1_b1 = XtVaCreateManagedWidget("menu1_p1_b1",
					  xmSeparatorWidgetClass,
					  menu1_p1,
					  NULL);


    /* Creation of menu1_p1_b2 */
    menu1_p1_b2 = XtVaCreateManagedWidget("menu1_p1_b2",
					  xmPushButtonWidgetClass,
					  menu1_p1,
					RES_CONVERT(XmNlabelString, "Open"),
					  RES_CONVERT(XmNmnemonic, "O"),
					  NULL);
    XtAddCallback(menu1_p1_b2, XmNactivateCallback,
		  (XtCallbackProc) activateCB_menu1_p1_b2,
		  (XtPointer) NULL);



    /* Creation of menu1_p1_b3 */
    menu1_p1_b3 = XtVaCreateManagedWidget("menu1_p1_b3",
					  xmSeparatorWidgetClass,
					  menu1_p1,
					  NULL);


    /* Creation of menu1_p1_b4 */
    menu1_p1_b4 = XtVaCreateManagedWidget("menu1_p1_b4",
					  xmPushButtonWidgetClass,
					  menu1_p1,
					RES_CONVERT(XmNlabelString, "Quit"),
					  RES_CONVERT(XmNmnemonic, "Q"),
					  NULL);
    XtAddCallback(menu1_p1_b4, XmNactivateCallback,
		  (XtCallbackProc) activateCB_menu1_p1_b4,
		  (XtPointer) NULL);



    /* Creation of menu1_top_b1 */
    menu1_top_b1 = XtVaCreateManagedWidget("menu1_top_b1",
					   xmCascadeButtonWidgetClass,
					   menu1,
					RES_CONVERT(XmNlabelString, "File"),
					   RES_CONVERT(XmNmnemonic, "F"),
					   XmNsubMenuId, menu1_p1,
					   XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
					   NULL);


    /* Creation of bulletinBoard1 */
    bulletinBoard1 = XtVaCreateManagedWidget("bulletinBoard1",
					     xmBulletinBoardWidgetClass,
					     mainWindow1,
					     NULL);


    /* Creation of label1 */
    label1 = XtVaCreateManagedWidget("label1",
				     xmLabelWidgetClass,
				     bulletinBoard1,
				     XmNx, 10,
				     XmNy, 80,
				     XmNwidth, 176,
				     XmNheight, 28,
			  RES_CONVERT(XmNlabelString, "INFORMATION ENTITY"),
				     XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--12-120-75-75-p-70-iso8859-1"),
				     NULL);


    /* Creation of label2 */
    label2 = XtVaCreateManagedWidget("label2",
				     xmLabelWidgetClass,
				     bulletinBoard1,
				     XmNx, 320,
				     XmNy, 80,
				     XmNwidth, 88,
				     XmNheight, 24,
				     RES_CONVERT(XmNlabelString, "MODULES"),
				     XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--12-120-75-75-p-70-iso8859-1"),
				     NULL);


    /* Creation of scrolledWindowList1 */
    scrolledWindowList1 = XtVaCreateManagedWidget("scrolledWindowList1",
						xmScrolledWindowWidgetClass,
						  bulletinBoard1,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNshadowThickness, 0,
						  XmNx, 10,
						  XmNy, 110,
						  NULL);


    /* Creation of scrolledList1 */
    scrolledList1 = XtVaCreateManagedWidget("scrolledList1",
					    xmListWidgetClass,
					    scrolledWindowList1,
					    XmNwidth, 280,
					    XmNheight, 324,
					    XmNx, 0,
					    XmNy, 110,
					    XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-100-100-100-p-82-iso8859-1"),
					    NULL);
    XtAddCallback(scrolledList1, XmNbrowseSelectionCallback,
		  (XtCallbackProc) browseSelectionCB_scrolledList1,
		  (XtPointer) NULL);



    /* Creation of scrolledWindowList2 */
    scrolledWindowList2 = XtVaCreateManagedWidget("scrolledWindowList2",
						xmScrolledWindowWidgetClass,
						  bulletinBoard1,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNshadowThickness, 0,
						  XmNx, 310,
						  XmNy, 110,
						  NULL);


    /* Creation of scrolledList2 */
    scrolledList2 = XtVaCreateManagedWidget("scrolledList2",
					    xmListWidgetClass,
					    scrolledWindowList2,
					    XmNwidth, 316,
					    XmNheight, 324,
					    XmNx, 0,
					    XmNy, 110,
					    XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-100-100-100-p-82-iso8859-1"),
					    NULL);
    XtAddCallback(scrolledList2, XmNbrowseSelectionCallback,
		  (XtCallbackProc) browseSelectionCB_scrolledList2,
		  (XtPointer) NULL);



    /* Creation of label3 */
    label3 = XtVaCreateManagedWidget("label3",
				     xmLabelWidgetClass,
				     bulletinBoard1,
				     XmNx, 20,
				     XmNy, 440,
				     XmNwidth, 92,
				     XmNheight, 28,
				  RES_CONVERT(XmNlabelString, "ATTRIBUTE "),
				     XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--12-120-75-75-p-70-iso8859-1"),
				     NULL);


    /* Creation of text1 */
    text1 = XtVaCreateManagedWidget("text1",
				    xmTextWidgetClass,
				    bulletinBoard1,
				    XmNwidth, 400,
				    XmNx, 10,
				    XmNy, 470,
				    XmNheight, 70,
				    XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-100-100-100-p-82-iso8859-1"),
				    NULL);


    /* Creation of scrolledWindowList3 */
    scrolledWindowList3 = XtVaCreateManagedWidget("scrolledWindowList3",
						xmScrolledWindowWidgetClass,
						  bulletinBoard1,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNshadowThickness, 0,
						  XmNx, 650,
						  XmNy, 110,
						  NULL);


    /* Creation of scrolledList3 */
    scrolledList3 = XtVaCreateManagedWidget("scrolledList3",
					    xmListWidgetClass,
					    scrolledWindowList3,
					    XmNwidth, 304,
					    XmNheight, 324,
					    XmNx, 0,
					    XmNy, 110,
					    XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-100-100-100-p-82-iso8859-1"),
					    NULL);
    XtAddCallback(scrolledList3, XmNbrowseSelectionCallback,
		  (XtCallbackProc) browseSelectionCB_scrolledList3,
		  (XtPointer) NULL);



    /* Creation of label4 */
    label4 = XtVaCreateManagedWidget("label4",
				     xmLabelWidgetClass,
				     bulletinBoard1,
				     XmNx, 660,
				     XmNy, 80,
				     XmNwidth, 104,
				     XmNheight, 24,
				  RES_CONVERT(XmNlabelString, "ATTRIBUTES"),
				     XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--12-120-75-75-p-70-iso8859-1"),
				     NULL);


    /* Creation of text2 */
    text2 = XtVaCreateManagedWidget("text2",
				    xmTextWidgetClass,
				    bulletinBoard1,
				    XmNwidth, 560,
				    XmNx, 410,
				    XmNy, 470,
				    XmNheight, 70,
				    XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
				    XmNwordWrap, FALSE,
				    NULL);


    /* Creation of label5 */
    label5 = XtVaCreateManagedWidget("label5",
				     xmLabelWidgetClass,
				     bulletinBoard1,
				     XmNx, 420,
				     XmNy, 440,
				     XmNwidth, 196,
				     XmNheight, 28,
		       RES_CONVERT(XmNlabelString, "ATTRIBUTE INFORMATION"),
				     XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--12-120-75-75-p-70-iso8859-1"),
				     NULL);


    /* Creation of scrolledWindowText1 */
    scrolledWindowText1 = XtVaCreateManagedWidget("scrolledWindowText1",
						xmScrolledWindowWidgetClass,
						  bulletinBoard1,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNx, 30,
						  XmNy, 550,
						  NULL);


    /* Creation of scrolledText1 */
    scrolledText1 = XtVaCreateManagedWidget("scrolledText1",
					    xmTextWidgetClass,
					    scrolledWindowText1,
					    XmNwidth, 890,
					    XmNheight, 50,
					    XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
					    NULL);


    /* Creation of text3 */
    text3 = XtVaCreateManagedWidget("text3",
				    xmTextWidgetClass,
				    bulletinBoard1,
				    XmNwidth, 880,
				    XmNx, 90,
				    XmNy, 20,
				    XmNheight, 40,
				    XmNfontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"),
				    NULL);


    XmMainWindowSetAreas(mainWindow1, menu1, (Widget) NULL,
			 (Widget) NULL, (Widget) NULL, bulletinBoard1);

    return (topLevelShell1);
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget
create_topLevelShell1(swidget _UxUxParent)
{
    Widget rtrn;

    UxParent = _UxUxParent;

    rtrn = _Uxbuild_topLevelShell1();

    create_fileSelectionBoxDialog1(NO_PARENT);
    return (rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/
