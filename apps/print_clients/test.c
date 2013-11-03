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
** @$=@$=@$=
*/
#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <string.h>
#endif

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>
#include "dicom.h"
#include "ctnthread.h"
#include "dicom_uids.h"
#include "lst.h"
#include "condition.h"
#include "icon.h"
#include "display_icon.h"
#include "display_study.h"

XtAppContext
app_context;

Widget
toplevel,
mainwin,
studyshell,
bibshell,
previewshell;

char
   *icon_file,
   *icon_index,
   *db_file;

void
main(int argc, char *argv[])
{
    Widget
    label;
    toplevel = XtAppInitialize(	/* create application context */
			       &app_context,
			       "Print Manager",
			       NULL, 0,
			       &argc, argv,
			       NULL,
			       NULL, 0);
    if (argc != 4) {
	printf("Usage: test db_file icon_index icon_file\n");
	exit(0);
    }
    THR_Init();
    db_file = strdup(argv[1]);
    icon_index = strdup(argv[2]);
    icon_file = strdup(argv[3]);
    printf("%s\n%s\n%s\n", db_file, icon_index, icon_file);
    mainwin = XtVaCreateManagedWidget("mainwin", formWidgetClass, toplevel, NULL);
    label = XtVaCreateManagedWidget("label", labelWidgetClass, mainwin,
				    XtNlabel, "Main Window", NULL);
    if (!(previewshell = CreatePreviewStudyWindow())) {
	printf("Error: CreatePreviewStudyWindow failed\n");
	THR_Shutdown();
	exit(0);
    }
    if (!(studyshell = CreateStudyListWindow())) {
	printf("Error: CreateStudyListWindow failed\n");
	THR_Shutdown();
	exit(0);
    }
    if ((bibshell = CreateStandardBFBWindow(3, 5)) == NULL) {
	printf("Error: CreateStandardBFBWindow failed\n");
	THR_Shutdown();
	exit(0);
    }
    XtRealizeWidget(toplevel);
    XtPopup(previewshell, XtGrabNone);
    XtPopup(studyshell, XtGrabNone);
    XtPopup(bibshell, XtGrabNone);
/*
    PreviewStudy(icon_index, icon_file, db_file);
*/
    XtAppMainLoop(app_context);
    THR_Shutdown();
}
