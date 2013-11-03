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
** Module Name(s):	main
** Author, Date:	Steve Moore
** Intent:		This file contains the initialization code
**			for the query_client application.  It
**			performs some initialization functions, calls
**			one function to create the user interface
**			and then kicks off the X event loop.
**			The query_client application is used to
**			perform DICOM queries on SCPs of the
**			DICOM Query/Retrieve Patient Root SOP Class.
** Last Update:		$Author: smm $, $Date: 1998-08-03 16:24:17 $
** Source File:		$RCSfile: query_client.c,v $
** Revision:		$Revision: 1.4 $
** Status:		$State: Exp $
*/


/*-----------------------------------------------------------
 * This is the project main program file for Xt generated
 * code. You may add application dependent source code
 * at the appropriate places.
 *
 * Do not modify the statements preceded by the dollar
 * sign ($), these statements will be replaced with
 * the appropriate source code when the main program is
 * generated.
 *
 * $Date: 1998-08-03 16:24:17 $  		$Revision: 1.4 $
 *-----------------------------------------------------------*/

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xlib.h>

#include <Xm/Xm.h>

/*----------------------------------------------------
 * UxXt.h needs to be included only when compiling a
 * stand-alone application.
 *---------------------------------------------------*/
#ifndef DESIGN_TIME
#include "UxXt.h"
#endif				/* DESIGN_TIME */

XtAppContext UxAppContext;
Widget UxTopLevel;
Display *UxDisplay;
int UxScreen;

/*----------------------------------------------
 * Insert application global declarations here
 *---------------------------------------------*/
#include "dicom.h"
#include "ctnthread.h"


#ifdef _NO_PROTO
main(argc, argv)
int argc;
char *argv[];
#else
main(int argc, char *argv[])
#endif				/* _NO_PROTO */
{
    /*-----------------------------------------------------------
     * Declarations.
     * The default identifier - mainIface will only be declared
     * if the interface function is global and of type swidget.
     * To change the identifier to a different name, modify the
     * string mainIface in the file "xtmain.dat". If "mainIface"
     * is declared, it will be used below where the return value
     * of  PJ_INTERFACE_FUNCTION_CALL will be assigned to it.
     *----------------------------------------------------------*/

    Widget mainIface;

    /*---------------------------------
     * Interface function declaration
     *--------------------------------*/

    Widget create_topLevelShell1(swidget);

    swidget UxParent = NULL;


    /*---------------------
     * Initialize program
     *--------------------*/

#ifdef XOPEN_CATALOG
    if (XSupportsLocale()) {
	XtSetLanguageProc(NULL, (XtLanguageProc) NULL, NULL);
    }
#endif

    SgePreInitialize(&argc, argv);
    UxTopLevel = XtAppInitialize(&UxAppContext, "query_client",
				 NULL, 0, &argc, argv, NULL, NULL, 0);

    UxDisplay = XtDisplay(UxTopLevel);
    UxScreen = XDefaultScreen(UxDisplay);

    /*
     * We set the geometry of UxTopLevel so that dialogShells that are
     * parented on it will get centered on the screen (if defaultPosition is
     * true).
     */

    XtVaSetValues(UxTopLevel,
		  XtNx, 0,
		  XtNy, 0,
		  XtNwidth, DisplayWidth(UxDisplay, UxScreen),
		  XtNheight, DisplayHeight(UxDisplay, UxScreen),
		  NULL);

    /*-------------------------------------------------------
     * Insert initialization code for your application here
     *------------------------------------------------------*/


    /*----------------------------------------------------------------
     * Create and popup the first window of the interface.  The
     * return value can be used in the popdown or destroy functions.
     * The Widget return value of  PJ_INTERFACE_FUNCTION_CALL will
     * be assigned to "mainIface" from  PJ_INTERFACE_RETVAL_TYPE.
     *---------------------------------------------------------------*/

    mainIface = create_topLevelShell1(UxParent);

    UxPopupInterface(mainIface, no_grab);

    /*-----------------------
     * Enter the event loop
     *----------------------*/

    THR_Init();
    XtAppMainLoop(UxAppContext);
    THR_Shutdown();
    return 0;
}
