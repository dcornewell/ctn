
/*******************************************************************************
	fileSelectionBoxDialog1.c

       Associated Header file: fileSelectionBoxDialog1.h
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/FileSB.h>

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
** Module Name(s):        fileSelectionBoxDialog1
**                        okCallback_fileSelectionBoxDialog1
**                        cancelCB_fileSelectionBoxDialog1
**
** Author, Date:          Chander L. Sabharwal, 27-June-94
**
** Intenet:               This program pops up a file selection box
**                        and lets the user select a file.
**
** Last Updates:          $Author: smm $, $Date: 1995-03-28 23:24:38 $
**
** Source File:           $RCSfile: fileSelectionBoxDialog1.c,v $
**
** Revision:              $Revision: 1.2 $
**
** Status:                $State: Exp $
*/

static char rcsid[] = "$Revision: 1.2 $ $RCSfile: fileSelectionBoxDialog1.c,v $";


/*char    info[500];*/

void copyWtext();
void openfile();


static swidget UxParent;

#define CONTEXT_MACRO_ACCESS 1
#include "fileSelectionBoxDialog1.h"
#undef CONTEXT_MACRO_ACCESS

Widget fileSelectionBoxDialog1;

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void
okCallback_fileSelectionBoxDialog1(
				   Widget wgt,
				   XtPointer cd,
				   XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	/*
	 * okCallback_fileSelectionBoxDialog1 *
	 * 
	 * Purpose: *      This subrouitne selects the DICOM filename from *
	 * list of files in the selectionbox, calls a suroutine * openfile
	 * and unmanages the selectionbox. *
	 * 
	 * Parameter Dictionary: *      cbs             input, pointer to the
	 * selected DICOM file *      filename        the selected DICOM
	 * filename *
	 * 
	 * Return Values: *      none *
	 * 
	 * Notes: *
	 * 
	 * Algorithm: *      Description of the algorithm (optional) and any
	 * other notes. *
	 * 
	 */

	XmFileSelectionBoxCallbackStruct *cbs;

	char *filename;

	char *info;

	cbs = (XmFileSelectionBoxCallbackStruct *) UxCallbackArg;

	if ((filename = (char *) malloc(100)) == NULL)
	    printf(" malloc filename failed\n");

	if ((info = (char *) malloc(500)) == NULL)
	    printf(" malloc info failed\n");

	if (!XmStringGetLtoR(cbs->value, XmSTRING_DEFAULT_CHARSET, &filename)) {
	    strcpy(info, "No DICOM filename selected from fileSelectionBoxDialog1, ERROR\n");
	    copyWtext(info);
	    free(info);
	}
	UxPopdownInterface(fileSelectionBoxDialog1);

	openfile(filename);


    }
}

static void
cancelCB_fileSelectionBoxDialog1(
				 Widget wgt,
				 XtPointer cd,
				 XtPointer cb)
{
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;
    {
	/*
	 * cancelCB_fileSelectionBOxDialog1 *
	 * 
	 * Purpose: *      This subroutine lets the user close the file
	 * selection box *
	 * 
	 * Parameter Dictionary: *      None *
	 * 
	 * Return Values: *      None *
	 * 
	 * Notes: *
	 * 
	 * Algorithm: *      Description of the algorithm (optional) and any
	 * other notes. *
	 * 
	 */
	UxPopdownInterface(fileSelectionBoxDialog1);
    }
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget
_Uxbuild_fileSelectionBoxDialog1()
{
    Widget _UxParent;


    /* Creation of fileSelectionBoxDialog1 */
    _UxParent = UxParent;
    if (_UxParent == NULL) {
	_UxParent = UxTopLevel;
    }
    _UxParent = XtVaCreatePopupShell("fileSelectionBoxDialog1_shell",
				     xmDialogShellWidgetClass, _UxParent,
				     XmNx, 420,
				     XmNy, 272,
				     XmNwidth, 568,
				     XmNheight, 548,
				     XmNshellUnitType, XmPIXELS,
				     XmNtitle, "fileSelectionBoxDialog1",
				     NULL);

    fileSelectionBoxDialog1 = XtVaCreateWidget("fileSelectionBoxDialog1",
					       xmFileSelectionBoxWidgetClass,
					       _UxParent,
					       XmNwidth, 568,
					       XmNheight, 548,
				     XmNdialogType, XmDIALOG_FILE_SELECTION,
					       XmNunitType, XmPIXELS,
					       NULL);
    XtAddCallback(fileSelectionBoxDialog1, XmNokCallback,
		  (XtCallbackProc) okCallback_fileSelectionBoxDialog1,
		  (XtPointer) NULL);
    XtAddCallback(fileSelectionBoxDialog1, XmNcancelCallback,
		  (XtCallbackProc) cancelCB_fileSelectionBoxDialog1,
		  (XtPointer) NULL);




    return (fileSelectionBoxDialog1);
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget
create_fileSelectionBoxDialog1(swidget _UxUxParent)
{
    Widget rtrn;

    UxParent = _UxUxParent;

    rtrn = _Uxbuild_fileSelectionBoxDialog1();

    return (rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/
