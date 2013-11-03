
/*******************************************************************************
	previewStudyForm.c

       Associated Header file: previewStudyForm.h
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

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
** Module Name(s):      create_previewStudyForm
**			loadPreviewStudyForm
**			displayMessage
** Author, Date:        Chander Sabharwal
** Intent:              This module provides a container for the
**			widget that will be used to hold image icons
**			(to allow the user to select images to be
**			placed on film and printed.)
** Last Update:         $Author: smm $, $Date: 1995-02-22 05:19:40 $
** Source File:         $RCSfile: previewStudyForm.c,v $
** Revision:            $Revision: 1.8 $
** Status:              $State: Exp $
*/

static char rcsid[] = "$Revision: 1.8 $ $RCSfile: previewStudyForm.c,v $";

#include <Xm/DrawnB.h>
#include <Xm/MessageB.h>

extern Widget wPreviewStudyForm;
extern Widget wMultipleSelection;

void displayMessage();


/*******************************************************************************
       The following header file defines the context structure.
*******************************************************************************/

#define CONTEXT_MACRO_ACCESS 1
#include "previewStudyForm.h"
#undef CONTEXT_MACRO_ACCESS


/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

void
loadPreviewStudyForm(int cols, int rows)
{
    createPreviewStudyIcons(cols, rows, wPreviewStudyForm);

}

void
displayMessage()
{
#ifdef RUNTIME
    if (wMultipleSelection == 0)
	wMultipleSelection = (Widget) create_MultipleSelection(NULL);
    XtManageChild(wMultipleSelection);
#endif
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget
_Uxbuild_previewStudyForm()
{
    Widget _UxParent;


    /* Creation of previewStudyForm */
    _UxParent = UxParent;
    if (_UxParent == NULL) {
	_UxParent = UxTopLevel;
    }
    _UxParent = XtVaCreatePopupShell("previewStudyForm_shell",
				     xmDialogShellWidgetClass, _UxParent,
				     XmNx, 350,
				     XmNy, 258,
				     XmNwidth, 590,
				     XmNheight, 602,
				     XmNshellUnitType, XmPIXELS,
				     XmNtitle, "previewStudyForm",
				     NULL);

    previewStudyForm = XtVaCreateWidget("previewStudyForm",
					xmFormWidgetClass,
					_UxParent,
					XmNwidth, 590,
					XmNheight, 602,
					XmNunitType, XmPIXELS,
					XmNautoUnmanage, FALSE,
					NULL);
    UxPutContext(previewStudyForm, (char *) UxPreviewStudyFormContext);


    XtAddCallback(previewStudyForm, XmNdestroyCallback,
		  (XtCallbackProc) UxDestroyContextCB,
		  (XtPointer) UxPreviewStudyFormContext);


    return (previewStudyForm);
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget
create_previewStudyForm(swidget _UxUxParent)
{
    Widget rtrn;
    _UxCpreviewStudyForm *UxContext;

    UxPreviewStudyFormContext = UxContext =
	(_UxCpreviewStudyForm *) UxNewContext(sizeof(_UxCpreviewStudyForm), False);

    UxParent = _UxUxParent;

    rtrn = _Uxbuild_previewStudyForm();

    return (rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/
