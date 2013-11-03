
/*******************************************************************************
	MultipleSelection.c

       Associated Header file: MultipleSelection.h
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/MessageB.h>

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
** Module Name(s):	create_MultipleSelection
** Author, Date:	Chander Sabharwal
** Intent:		This module has the dialog widget that is displayed
**			when prompting the user to select the last image
**			in a set of images for printing (select first,
**			select last).
** Last Update:		$Author: smm $, $Date: 1995-02-22 05:20:02 $
** Source File:		$RCSfile: MultipleSelection.c,v $
** Revision:		$Revision: 1.4 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.4 $ $RCSfile: MultipleSelection.c,v $";


void cancelMulti_select();


/*******************************************************************************
       The following header file defines the context structure.
*******************************************************************************/

#define CONTEXT_MACRO_ACCESS 1
#include "MultipleSelection.h"
#undef CONTEXT_MACRO_ACCESS


/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void
okCallback_MultipleSelection(
			     Widget wgt,
			     XtPointer cd,
			     XtPointer cb)
{
    _UxCMultipleSelection *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxMultipleSelectionContext;
    UxMultipleSelectionContext = UxContext =
	(_UxCMultipleSelection *) UxGetContext(UxWidget);
    {

    }
    UxMultipleSelectionContext = UxSaveCtx;
}

static void
cancelCB_MultipleSelection(
			   Widget wgt,
			   XtPointer cd,
			   XtPointer cb)
{
    _UxCMultipleSelection *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxMultipleSelectionContext;
    UxMultipleSelectionContext = UxContext =
	(_UxCMultipleSelection *) UxGetContext(UxWidget);
    {
	cancelMulti_select();
    }
    UxMultipleSelectionContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget
_Uxbuild_MultipleSelection()
{
    Widget _UxParent;


    /* Creation of MultipleSelection */
    _UxParent = UxParent;
    if (_UxParent == NULL) {
	_UxParent = UxTopLevel;
    }
    _UxParent = XtVaCreatePopupShell("MultipleSelection_shell",
				     xmDialogShellWidgetClass, _UxParent,
				     XmNx, 750,
				     XmNy, 180,
				     XmNwidth, 270,
				     XmNheight, 100,
				     XmNallowShellResize, FALSE,
				     XmNshellUnitType, XmPIXELS,
				     XmNtitle, "MultipleSelection",
				     NULL);

    MultipleSelection = XtVaCreateWidget("MultipleSelection",
					 xmMessageBoxWidgetClass,
					 _UxParent,
					 XmNwidth, 270,
					 XmNheight, 100,
					 XmNdialogType, XmDIALOG_MESSAGE,
					 XmNunitType, XmPIXELS,
	  RES_CONVERT(XmNmessageString, "Select last image with Button 2."),
					 NULL);
    XtAddCallback(MultipleSelection, XmNokCallback,
		  (XtCallbackProc) okCallback_MultipleSelection,
		  (XtPointer) UxMultipleSelectionContext);
    XtAddCallback(MultipleSelection, XmNcancelCallback,
		  (XtCallbackProc) cancelCB_MultipleSelection,
		  (XtPointer) UxMultipleSelectionContext);

    UxPutContext(MultipleSelection, (char *) UxMultipleSelectionContext);


    XtAddCallback(MultipleSelection, XmNdestroyCallback,
		  (XtCallbackProc) UxDestroyContextCB,
		  (XtPointer) UxMultipleSelectionContext);


    return (MultipleSelection);
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget
create_MultipleSelection(swidget _UxUxParent)
{
    Widget rtrn;
    _UxCMultipleSelection *UxContext;

    UxMultipleSelectionContext = UxContext =
	(_UxCMultipleSelection *) UxNewContext(sizeof(_UxCMultipleSelection), False);

    UxParent = _UxUxParent;

    rtrn = _Uxbuild_MultipleSelection();

    return (rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/
