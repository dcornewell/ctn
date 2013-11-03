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
** Intent:		This file provides the "main" function for
**			the rsa_ui application.  This application allows
**			a user to examine impressions and results
**			from studies, to modify the impressions and
**			results and to sent DICOM events to server
**			applications to indicate changes.
** Last Update:         $Author: smm $, $Date: 1998-08-03 16:25:30 $
** Source File:         $RCSfile: rsa_ui.c,v $
** Revision:            $Revision: 1.7 $
** Status:              $State: Exp $
*/

static char rcsid[] = "$Revision: 1.7 $ $RCSfile: rsa_ui.c,v $";

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
 * $Date: 1998-08-03 16:25:30 $  		$Revision: 1.7 $
 *-----------------------------------------------------------*/

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xlib.h>

#include <Xm/Xm.h>

#include <stdio.h>

#include "dicom.h"
#include "ctnthread.h"
#include "condition.h"
#include "lst.h"
#include "dicom_objects.h"
#include "tbl.h"
#include "manage.h"
#include "fis.h"
#include "rsa.h"

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

extern void loadPatientStudyList();

FIS_HANDLE *fisHandle;
DMAN_HANDLE *dmanHandle;
char applicationTitle[20] = "RSA_MIR";
char *eventDestinations = NULL;
static void usageerror();

#ifdef _NO_PROTO
main(argc, argv)
int argc;
char *argv[];
#else
main(int argc, char *argv[])
#endif				/* _NO_PROTO */
{
    char *databaseName = "CTNControl";
    CONDITION cond;
    CTNBOOLEAN verboseTBL = FALSE;

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

    Widget create_applicationShell1(swidget);

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
    UxTopLevel = XtAppInitialize(&UxAppContext, "rsa_ui",
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
    while (--argc > 0 && *(++argv)[0] == '-') {
	switch ((*argv)[1]) {
	case 'a':
	    if (argc-- < 1)
		usageerror();
	    strncpy(applicationTitle, *++argv, sizeof(applicationTitle) - 1);
	    applicationTitle[sizeof(applicationTitle) - 1] = '\0';
	    break;
	case 'c':
	    if (argc-- < 1)
		usageerror();
	    eventDestinations = *++argv;
	    break;
	case 'f':
	    if (argc-- < 1)
		usageerror();
	    databaseName = *++argv;
	    break;
	case 'h':
	    usageerror();
	    break;
	case 'x':
	    if (argc-- < 1)
		usageerror();
	    argv++;
	    if (strcmp(*argv, "TBL") == 0)
		verboseTBL = TRUE;
	    break;
	default:
	    printf("Unrecognized option: %s\n", *argv);
	    break;
	}
    }
    THR_Init();
    (void) TBL_Debug(verboseTBL);
#ifdef RUNTIME
    cond = openTables(databaseName, applicationTitle, &dmanHandle,
		      &fisHandle);
#endif
    if (cond != 1) {
	COND_DumpConditions();
	exit(1);
    }
    if (eventDestinations == NULL) {
	printf("Warning: No event destinations specified\n");
	printf("         Events will not be delivered.\n");
    }
    /*----------------------------------------------------------------
     * Create and popup the first window of the interface.  The
     * return value can be used in the popdown or destroy functions.
     * The Widget return value of  PJ_INTERFACE_FUNCTION_CALL will
     * be assigned to "mainIface" from  PJ_INTERFACE_RETVAL_TYPE.
     *---------------------------------------------------------------*/

    mainIface = create_applicationShell1(UxParent);

    UxPopupInterface(mainIface, no_grab);

    /*-----------------------
     * Enter the event loop
     *----------------------*/

    loadPatientStudyList();
    XtAppMainLoop(UxAppContext);
    THR_Shutdown();
    return 0;
}

static void
usageerror()
{
    char msg[] = "\
Usage: [-a application] [-c remote] [-d databaseName] [-v] [-x <facility>] \n\
\n\
    -a  Use application as application title instead of default (RSA_MIR)\n\
    -c  Send event notifications to application remote (no default)\n\
    -f  Use database databaseName rather than default (CTNControl)\n\
    -h  Display help page\n\
    -x  Place <facility> in verbose mode (TBL)\n";

    fprintf(stderr, msg);
    exit(1);
}
