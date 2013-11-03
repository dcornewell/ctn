
/*******************************************************************************
	studyListForm.c

       Associated Header file: studyListForm.h
*******************************************************************************/

#include "ctn_os.h"
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/LabelG.h>
#include <Xm/Form.h>

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
**                   Electronic Radiology Laboratory
**                 Mallinckrodt Institute of Radiology
**              Washington University School of Medicine
**
** Module Name(s):	loadStudyList
**			create_studyListForm
** Author, Date:	Chander Sabharwal
** Intent:		This module contains the user interface for displaying the
**			set of studies that are known by the print manager.  It
**			contains a public interface for creating the widget to
**			display the study list.
** Last Update:		$Author: smm $, $Date: 2002-10-31 23:36:07 $
** Source File:		$RCSfile: studyListForm.c,v $
** Revision:		$Revision: 1.7 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.7 $ $RCSfile: studyListForm.c,v $";


#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "tbl.h"
#include "dicom_uids.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "manage.h"
#include "mut.h"

#include "display.h"
#include "format.h"

extern char *icon_file,
   *icon_index,
   *db_file;
extern LST_HEAD *lst_studylist;

void PreviewStudy();


/*******************************************************************************
       The following header file defines the context structure.
*******************************************************************************/

#define CONTEXT_MACRO_ACCESS 1
#include "studyListForm.h"
#undef CONTEXT_MACRO_ACCESS


/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

void
loadStudyList(LST_HEAD * lst_studylist)
{

    char buf[65];

    MUT_LoadList(wStudyList, lst_studylist, formatstudylist, buf);

}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void
browseSelectionCB_wStudyList(
			     Widget wgt,
			     XtPointer cd,
			     XtPointer cb)
{
    _UxCstudyListForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxStudyListFormContext;
    UxStudyListFormContext = UxContext =
	(_UxCstudyListForm *) UxGetContext(UxWidget);
    {
	XmListCallbackStruct *l;
	LIST_ITEM *item;

	l = (XmListCallbackStruct *) UxCallbackArg;

	item = LST_Index(&lst_studylist, l->item_position);
	if (item == NULL)
	    return;

	PreviewStudy(icon_index, icon_file, item->AccessionNumber);
    }
    UxStudyListFormContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget
_Uxbuild_studyListForm()
{
    Widget _UxParent;


    /* Creation of studyListForm */
    _UxParent = UxParent;
    if (_UxParent == NULL) {
	_UxParent = UxTopLevel;
    }
    _UxParent = XtVaCreatePopupShell("studyListForm_shell",
				     xmDialogShellWidgetClass, _UxParent,
				     XmNx, 550,
				     XmNy, 140,
				     XmNwidth, 330,
				     XmNheight, 320,
				     XmNshellUnitType, XmPIXELS,
				     XmNtitle, "studyListForm",
				     NULL);

    studyListForm = XtVaCreateWidget("studyListForm",
				     xmFormWidgetClass,
				     _UxParent,
				     XmNwidth, 330,
				     XmNheight, 320,
				     XmNunitType, XmPIXELS,
				     NULL);
    UxPutContext(studyListForm, (char *) UxStudyListFormContext);


    /* Creation of labelGadget1 */
    labelGadget1 = XtVaCreateManagedWidget("labelGadget1",
					   xmLabelGadgetClass,
					   studyListForm,
					   XmNx, 10,
					   XmNy, 10,
					   XmNwidth, 120,
					   XmNheight, 30,
			     RES_CONVERT(XmNlabelString, "List Of Studies"),
					   NULL);
    UxPutContext(labelGadget1, (char *) UxStudyListFormContext);


    /* Creation of scrolledWindowList1 */
    scrolledWindowList1 = XtVaCreateManagedWidget("scrolledWindowList1",
						xmScrolledWindowWidgetClass,
						  studyListForm,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNshadowThickness, 0,
						  XmNx, 20,
						  XmNy, 60,
						  NULL);
    UxPutContext(scrolledWindowList1, (char *) UxStudyListFormContext);


    /* Creation of wStudyList */
    wStudyList = XtVaCreateManagedWidget("wStudyList",
					 xmListWidgetClass,
					 scrolledWindowList1,
					 XmNwidth, 260,
					 XmNheight, 240,
					 XmNautomaticSelection, FALSE,
					 NULL);
    XtAddCallback(wStudyList, XmNbrowseSelectionCallback,
		  (XtCallbackProc) browseSelectionCB_wStudyList,
		  (XtPointer) UxStudyListFormContext);

    UxPutContext(wStudyList, (char *) UxStudyListFormContext);


    XtAddCallback(studyListForm, XmNdestroyCallback,
		  (XtCallbackProc) UxDestroyContextCB,
		  (XtPointer) UxStudyListFormContext);


    return (studyListForm);
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget
create_studyListForm(swidget _UxUxParent)
{
    Widget rtrn;
    _UxCstudyListForm *UxContext;

    UxStudyListFormContext = UxContext =
	(_UxCstudyListForm *) UxNewContext(sizeof(_UxCstudyListForm), False);

    UxParent = _UxUxParent;

    rtrn = _Uxbuild_studyListForm();

    return (rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/
