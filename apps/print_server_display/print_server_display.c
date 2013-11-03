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
#include <stdio.h>
#include <stdlib.h>
#ifdef ATHENA
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/AsciiText.h>

#include "dicom.h"
#include "ctnthread.h"
#include "lst.h"
#include "condition.h"
#include "dicom_objects.h"
#include "dicom_print.h"
#include "display_prn.h"
#include "print_queue.h"
#include "gq.h"

#define MIN_HEIGHT 300
#define MIN_WIDTH 300
#define MAX_HEIGHT DisplayHeight(XtDisplay(toplevel), DefaultScreen(XtDisplay(toplevel)))
#define MAX_WIDTH DisplayWidth(XtDisplay(toplevel), DefaultScreen(XtDisplay(toplevel)))

Widget
toplevel;
static XtAppContext
    app_ctx;
static int
    queue_id;
static LST_HEAD
*   session_list = NULL;
static Widget
    num_client_txt;
static int
    num_client = 0;

static CONDITION
kill_queue(int qID)
{
    GQ_ELEM
    elem;
    CONDITION
	cond;

    cond = GQ_NORMAL;
    while (cond == GQ_NORMAL) {
	cond = GQ_Dequeue(qID, &elem);	/* dequeue the next element */
	/* check if there is some error */
	if ((cond != GQ_NORMAL) && (cond != GQ_QUEUEEMPTY)) {
	    return cond;
	} else if (cond == GQ_NORMAL) {	/* no error and Q not empty */
	    if (strlen(elem.objFile) != 0)	/* file exists */
		unlink(elem.objFile);
	}
    }
    strcpy(elem.objFile, KILL_DISPLAY);
    cond = GQ_Enqueue(qID, (void *) &elem);
    if (cond != GQ_NORMAL) {
	return cond;
    }
    (void) sleep(5);
    cond = GQ_KillQueue(qID);
    if (cond != GQ_NORMAL) {
	return cond;
    }
    return GQ_NORMAL;
}

static XtCallbackProc
quit(Widget w, XtPointer client_data, XtPointer call_data)
{
    CONDITION
    cond;

    if (session_list != NULL)
	DISP_DeleteAllSession(&session_list);

    /* we also kill the created GQ */
    cond = kill_queue(queue_id);
    if (cond != GQ_NORMAL) {
	fprintf(stderr, "Error removing the GQ ID : %d\n", queue_id);
	COND_DumpConditions();
	exit(1);
    }
    exit(0);
}

void
createMainWin()
{
    Widget
    form,
    label1,
    label2,
    btn;

    form = XtVaCreateManagedWidget("print_server_display_mainform",
				   formWidgetClass, toplevel, NULL);
/*
	label1 = XtVaCreateManagedWidget("print_lbl1", labelWidgetClass, form,
		XtNborderWidth, 0,
		XtNlabel, "Print Spooler Display",
		NULL);
*/
    label2 = XtVaCreateManagedWidget("print_lbl2", labelWidgetClass, form,
				     XtNborderWidth, 0,
				     XtNlabel, "Number of Clients",
/*
		XtNfromVert, label1,
*/
				     NULL);
    num_client_txt = XtVaCreateManagedWidget("print_num_client_txt",
					     asciiTextWidgetClass, form,
					     XtNstring, "0",
					     XtNfromVert, label2,
					     NULL);
    btn = XtVaCreateManagedWidget("print_quit_btn", commandWidgetClass, form,
				  XtNlabel, "Quit",
				  XtNborderWidth, 3,
				  XtNfromVert, num_client_txt,
				  NULL);
/*lint -e64*/
    XtAddCallback(btn, XtNcallback, quit, NULL);
/*lint +e64*/
}

#define TIMEOUT 2000

XtTimerCallbackProc
delayedDelete(XtPointer client_data, XtIntervalId * id)
{
    char
       *UID;
    static char
        num[5];

    UID = (char *) client_data;
    if (DISP_DestroySession(UID, &session_list) == False)
	fprintf(stderr, "Fatal Error: DISP_DestorySession failed\n");
    else {
	sprintf(num, "%d", --num_client);
	XtVaSetValues(num_client_txt,
		      XtNstring, num,
		      NULL);
    }
    free(UID);
    return NULL;
}

XtTimerCallbackProc
pollQueue(XtPointer client_data, XtIntervalId * id)
{
    GQ_ELEM
	element;
    CONDITION
	cond;
    char
       *temp;
    static char
        num[5];


    cond = GQ_Dequeue(queue_id, &element);
    switch (cond) {
    case GQ_NORMAL:
	break;
    case GQ_QUEUEEMPTY:
/*lint -e64*/
	XtAppAddTimeOut(app_ctx, TIMEOUT, pollQueue, NULL);
/*lint +e64*/
	return NULL;
    case GQ_SEMAPHOREFAIL:
    case GQ_NOPENQUEUE:
    case GQ_UNIMPLEMENTED:
    default:
/*
	fprintf(stderr, "GQ_Dequeue failed\n");
*/
/*lint -e64*/
	XtAppAddTimeOut(app_ctx, TIMEOUT, pollQueue, NULL);
/*lint +e64*/
	return NULL;
    }
    switch (element.prnMessage) {
    case PRN_NEWFILMSESSION:
	DISP_DeleteAllSession(&session_list);
	if (DISP_CreateSession(element.instanceUID, element.objFile, &session_list) != True)
	    fprintf(stderr, "Error: Failed to create new session\n");
	else {
	    sprintf(num, "%d", ++num_client);
	    XtVaSetValues(num_client_txt,
			  XtNstring, num,
			  NULL);
	}
	break;
    case PRN_NEWFILMBOX:
	if (DISP_AddBox(element.instanceUID, element.objFile, &session_list) != True)
	    fprintf(stderr, "Error: Failed to create new film box\n");
	break;
    case PRN_NEWIMAGEBOX:
	if (DISP_SetImage(element.instanceUID, element.objFile, &session_list) != True)
	    fprintf(stderr, "Error: Failed to set image box\n");
	break;
    case PRN_PRINTCOMMAND:
	break;
    case PRN_ENDSESSION:
	temp = (char *) malloc(sizeof(char) * strlen(element.instanceUID) +1);
	strcpy(temp, element.instanceUID);
/*lint -e64*/
	XtAppAddTimeOut(app_ctx, 5000, delayedDelete, temp);
/*lint +e64*/
	break;
    case PRN_TOTALFAILURE:
	(void) quit(NULL, NULL, NULL);
	break;
    default:
	break;
    }
/*lint -e64*/
    XtAppAddTimeOut(app_ctx, 1, pollQueue, NULL);
/*lint +e64*/
    return NULL;
}

void
UsageError()
{
    static char *help[] = {
	"|--------------------------------------------------------------------------|",
	"| print_server_display                                      | Version 1.0  |",
	"|--------------------------------------------------------------------------|",
	"| USAGE:                                                                   |",
	"|    print_server_display [SWITCHES] qid                                   |",
	"|                                                                          |",
	"|            qid        the spooler number of the shared queue.            |",
	"|                                                                          |",
	"| SWITCHES:                                            DEFAULTS:           |",
	"|-----------                                           --------------------|",
	"|    -w width      Sets the width of the window        width of display    |",
	"|    -h height     Sets the height of the window       height of display   |",
	"|    -v            Sets Verbose mode                                       |",
	"|                                                                          |",
	"|--------------------------------------------------------------------------|"
    };

    int
        i;

    for (i = 0; i < DIM_OF(help); i++)
	fprintf(stderr, "%s\n", help[i]);
}

static void
cond_CB(int value, char *message)
{
    fprintf(stderr, "Error value:%d\nError message:%s\n\n", value, message);
}

void
main(int argc, char *argv[])
{
    int
        height,
        width,
        temp;
    Boolean
	GOT_QID = False;
    CONDITION
	cond;

    toplevel = XtAppInitialize(	/* create application context */
			       &app_ctx,
			       "print_server_display",
			       NULL, 0,
#ifdef SOLARIS
			       (Cardinal *) & argc,
#else
			       &argc,
#endif
			       argv,
			       NULL,
			       NULL, 0);

    width = MAX_WIDTH - 10;
    height = MAX_HEIGHT - 10;
    argc--;
    argv++;
    while (argc > 0) {
	if (strcmp(*argv, "-w") == 0) {
	    argc--;
	    argv++;
	    temp = atoi(*argv);
	    if (temp < MIN_WIDTH) {
		fprintf(stderr, "Height must be > %d\n", MIN_WIDTH);
		UsageError();
		exit(0);
	    }
	    if (temp > MAX_WIDTH) {
		fprintf(stderr, "Height must be < %d\n", MAX_WIDTH);
		UsageError();
		exit(0);
	    }
	    width = temp;
	    argc--;
	    argv++;
	} else if (strcmp(*argv, "-h") == 0) {
	    argc--;
	    argv++;
	    temp = atoi(*argv);
	    if (temp < MIN_HEIGHT) {
		fprintf(stderr, "Height must be > %d\n", MIN_HEIGHT);
		UsageError();
		exit(0);
	    }
	    if (temp > MAX_HEIGHT) {
		fprintf(stderr, "Height must be < %d\n", MAX_HEIGHT);
		UsageError();
		exit(0);
	    }
	    height = temp;
	    argc--;
	    argv++;
	} else if (strcmp(*argv, "-v") == 0) {
	    argc--;
	    argv++;
	    (void) COND_EstablishCallback(cond_CB);
	} else {
	    if (argc != 1) {
		fprintf(stderr, "Only the last parameter is a none switch\n");
		UsageError();
		exit(0);
	    }
	    queue_id = atoi(*argv);
	    if (queue_id == 0) {
		fprintf(stderr, "Invalid queueu ID\n");
		UsageError();
		exit(0);
	    }
	    argc--;
	    argv++;
	    GOT_QID = True;
	}
    }
    if (GOT_QID == False) {
	fprintf(stderr, "Error: Missing QID\n");
	UsageError();
	exit(0);
    }
    THR_Init();
    /* The print_server_display creates a GQ with the specified ID */
    cond = GQ_InitQueue(queue_id, 128, sizeof(GQ_ELEM));
    if (cond != GQ_NORMAL) {
	fprintf(stderr, "GQ_InitQueue failed to create GQ with ID : %d\n",
		queue_id);
	COND_DumpConditions();
	exit(1);
    }
    /* now get hold of the just created GQ */
    cond = GQ_GetQueue(queue_id, sizeof(GQ_ELEM));
    switch (cond) {
    case GQ_SHAREDMEMORYFAIL:
	fprintf(stderr, "GQ Shared Memory failed\n");
	exit(0);
    case GQ_FILEACCESSFAIL:
	fprintf(stderr, "GQ File Access failed\n");
	exit(0);
    case GQ_BADELEMSIZE:
	fprintf(stderr, "GQ Bad Element Size\n");
	exit(0);
    case GQ_UNIMPLEMENTED:
	fprintf(stderr, "GQ Unimplemented\n");
	exit(0);
    case GQ_NORMAL:
	break;
    }
    session_list = LST_Create();
    printf("width = %d, height = %d\n", width, height);
    createMainWin();
    XtRealizeWidget(toplevel);
    DISP_Initialize(toplevel, width, height);
/*
    DISP_CreateSession("1.2.840.113654.2.3.1993.9.123.9.3221", "test", &session_list);
    DISP_AddBox("test", "PRN_13674.2", &session_list);
    DISP_AddBox("test2", "PRN_13674.2", &session_list);
    DISP_SetImage("1.2.840.113654.2.3.1993.9.123.9.3226", "PRN_13674.5", &session_list);
    DISP_SetImage("1.2.840.113654.2.3.1993.9.123.9.3225", "PRN_13674.4", &session_list);
    DISP_SetImage("1.2.840.113654.2.3.1993.9.123.9.3226", "PRN_13674.3", &session_list);
*/
/*lint -e64*/
    XtAppAddTimeOut(app_ctx, TIMEOUT, pollQueue, NULL);
/*lint +e64*/
    XtAppMainLoop(app_ctx);
    THR_Shutdown();
}
#else

int
main()
{
    fprintf(stderr, "This application needs the Athena toolkit to compile/run\n");
    return 1;
}
#endif
