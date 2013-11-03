
/*******************************************************************************
	BasicFilmBox_Standard.c

       Associated Header file: BasicFilmBox_Standard.h
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
** Module Name(s):	create_BasicFilmBox_Standard
**			loadBasicFilmBox_Standard
**			printSuccessMes
** Author, Date:	Chander Sabharwal
** Intent:		This module contains the widget that will be used as
**			the container for icons for images being arranged
**			on a piece of film using the standard (grid) layout.
** Last Update:		$Author: smm $, $Date: 1995-02-22 05:19:54 $
** Source File:		$RCSfile: BasicFilmBox_Standard.c,v $
** Revision:		$Revision: 1.7 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.7 $ $RCSfile: BasicFilmBox_Standard.c,v $";


void createStandardBFBWindow();
void printSuccessMes();

extern Widget wBasicFilmBox_Standard;
extern Widget wPrintMessage;


/*******************************************************************************
       The following header file defines the context structure.
*******************************************************************************/

#define CONTEXT_MACRO_ACCESS 1
#include "BasicFilmBox_Standard.h"
#undef CONTEXT_MACRO_ACCESS


/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

loadBasicFilmBox_Standard(int r1, int c1)
{

    createStandardBFBWindow(r1, c1, wBasicFilmBox_Standard);

}

void
printSuccessMes()
{

#ifdef RUNTIME
    if (wPrintMessage == 0)
	wPrintMessage = (Widget) create_PrintMessage(NULL);

    XtManageChild(wPrintMessage);
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
_Uxbuild_BasicFilmBox_Standard()
{
    Widget _UxParent;


    /* Creation of BasicFilmBox_Standard */
    _UxParent = UxParent;
    if (_UxParent == NULL) {
	_UxParent = UxTopLevel;
    }
    _UxParent = XtVaCreatePopupShell("BasicFilmBox_Standard_shell",
				     xmDialogShellWidgetClass, _UxParent,
				     XmNx, 490,
				     XmNy, 420,
				     XmNwidth, 330,
				     XmNheight, 337,
				     XmNallowShellResize, TRUE,
				     XmNshellUnitType, XmPIXELS,
				     XmNtitle, "BasicFilmBox_Standard",
				     NULL);

    BasicFilmBox_Standard = XtVaCreateWidget("BasicFilmBox_Standard",
					     xmFormWidgetClass,
					     _UxParent,
					     XmNwidth, 330,
					     XmNheight, 337,
					     XmNunitType, XmPIXELS,
					     XmNnoResize, FALSE,
					     XmNautoUnmanage, FALSE,
					     NULL);
    UxPutContext(BasicFilmBox_Standard, (char *) UxBasicFilmBox_StandardContext);


    XtAddCallback(BasicFilmBox_Standard, XmNdestroyCallback,
		  (XtCallbackProc) UxDestroyContextCB,
		  (XtPointer) UxBasicFilmBox_StandardContext);


    return (BasicFilmBox_Standard);
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget
create_BasicFilmBox_Standard(swidget _UxUxParent)
{
    Widget rtrn;
    _UxCBasicFilmBox_Standard *UxContext;

    UxBasicFilmBox_StandardContext = UxContext =
	(_UxCBasicFilmBox_Standard *) UxNewContext(sizeof(_UxCBasicFilmBox_Standard), False);

    UxParent = _UxUxParent;

    rtrn = _Uxbuild_BasicFilmBox_Standard();

    return (rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/
