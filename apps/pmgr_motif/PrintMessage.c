
/*******************************************************************************
	PrintMessage.c

       Associated Header file: PrintMessage.h
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
** Module Name(s):      create_PrintMessage
** Author, Date:        Chander Sabharwal
** Intent:              This module contains a widget and code that allows
**			the developer to pup up a message saying that
**			printing completed successfully.
** Last Update:         $Author: smm $, $Date: 1995-02-22 05:19:59 $
** Source File:         $RCSfile: PrintMessage.c,v $
** Revision:            $Revision: 1.5 $
** Status:              $State: Exp $
*/

static char rcsid[] = "$Revision: 1.5 $ $RCSfile: PrintMessage.c,v $";


void MoreBFB();


/*******************************************************************************
       The following header file defines the context structure.
*******************************************************************************/

#define CONTEXT_MACRO_ACCESS 1
#include "PrintMessage.h"
#undef CONTEXT_MACRO_ACCESS


/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void
okCallback_PrintMessage(
			Widget wgt,
			XtPointer cd,
			XtPointer cb)
{
    _UxCPrintMessage *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxPrintMessageContext;
    UxPrintMessageContext = UxContext =
	(_UxCPrintMessage *) UxGetContext(UxWidget);
    {
	MoreBFB();
    }
    UxPrintMessageContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget
_Uxbuild_PrintMessage()
{
    Widget _UxParent;


    /* Creation of PrintMessage */
    _UxParent = UxParent;
    if (_UxParent == NULL) {
	_UxParent = UxTopLevel;
    }
    _UxParent = XtVaCreatePopupShell("PrintMessage_shell",
				     xmDialogShellWidgetClass, _UxParent,
				     XmNx, 640,
				     XmNy, 320,
				     XmNwidth, 300,
				     XmNheight, 100,
				     XmNshellUnitType, XmPIXELS,
				     XmNtitle, "PrintMessage",
				     NULL);

    PrintMessage = XtVaCreateWidget("PrintMessage",
				    xmMessageBoxWidgetClass,
				    _UxParent,
				    XmNwidth, 300,
				    XmNheight, 100,
				    XmNdialogType, XmDIALOG_MESSAGE,
				    XmNunitType, XmPIXELS,
				    RES_CONVERT(XmNmessageString, " 'Printed!' , ' Print Operation Completed'"),
				    XmNtextFontList, UxConvertFontList("-adobe-helvetica-bold-r-normal--17-120-100-100-p-92-iso8859-1"),
				    NULL);
    XtAddCallback(PrintMessage, XmNokCallback,
		  (XtCallbackProc) okCallback_PrintMessage,
		  (XtPointer) UxPrintMessageContext);

    UxPutContext(PrintMessage, (char *) UxPrintMessageContext);


    XtAddCallback(PrintMessage, XmNdestroyCallback,
		  (XtCallbackProc) UxDestroyContextCB,
		  (XtPointer) UxPrintMessageContext);


    return (PrintMessage);
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget
create_PrintMessage(swidget _UxUxParent)
{
    Widget rtrn;
    _UxCPrintMessage *UxContext;

    UxPrintMessageContext = UxContext =
	(_UxCPrintMessage *) UxNewContext(sizeof(_UxCPrintMessage), False);

    UxParent = _UxUxParent;

    rtrn = _Uxbuild_PrintMessage();

    return (rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/
