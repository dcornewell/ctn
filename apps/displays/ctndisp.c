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
**				DICOM 93
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	ctndisp (main())
** Author, Date:	David E. Beecher, 4-Jul-93
** Intent:		DICOM Image display server.
** Last Update:		$Author: smm $, $Date: 2002-12-13 15:18:24 $
** Source File:		$RCSfile: ctndisp.c,v $
** Revision:		$Revision: 1.13 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.13 $ $RCSfile: ctndisp.c,v $";

#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#ifdef X11
#include        <X11/Xlib.h>
#include        <X11/Xutil.h>
#include        <X11/Xos.h>
#include        <X11/Xatom.h>

#include	"dicom.h"
#include	"condition.h"
#include	"lst.h"
#include	"dicom_objects.h"
#include	"iqueues.h"
#include 	"gq.h"
#include	"xutl.h"
#include 	"common_x.h"
#include 	"ctndisp.h"
#include 	"common_disp.h"

char *CTNDISP_usage = "CTNDISP Usage: ctndisp -n node -q queue_number [-w display_width -h display_height]\n";
/*
 * This typedef and these function prototypes occur here because
 * gcc can't handlel the X include libraries....this is the short
 * answer...
 */
#ifdef SMM
typedef struct _XImageStruct {
    XImage *x_image;		/* the X image struct		 */
    Window image_window;	/* the window for display	 */
}   XImageStruct;
#endif

void CTNDISP_AdjustImageDepth(ImageStruct * img);
void CTNDISP_DisplayImageData(ImageStruct * img, XImageStruct * ximg);
void CTNDISP_RefreshImageData(ImageStruct * img, XImageStruct * ximg);

/*
 * Globals (preceded with G_)...declared extern in other files
 */
Window G_big_window;

#if 0
Display *G_display;		/* Xstuff used all over...	 */
#endif

Visual *G_visual;

#if 0
Colormap G_cmap;
#endif

GC
    G_gclargeT,
    G_gcsmallT,
    G_gclargeTred,
    G_gclargeTgreen,
    G_gcblack,			/* Graphics contexts needed...	 */
    G_gcred,
    G_gcgreen;

int G_display_width,
    G_display_height,		/* Well known screen stuff...	 */
    G_screen,
    G_pixel_depth,
    G_store_depth,
    G_byte_order,
    G_ncolors;

int G_FirstTime = TRUE;

char G_nodename[MAX_STRING_LENGTH];
char G_dpnname[MAX_STRING_LENGTH];

XUTL_STRUCT* G_xStruct = 0;
/*
 * End Globals...
 */

ImageStruct cur_img,
    next_img;

XImageStruct xcur_img,
    xnext_img;




/*******************************************************************/
main(int argc, char **argv)
{
    CONDITION
	retval;
    CTNDISP_Queue
	element;
    void
       *data;
    unsigned char
       *new_data;
    unsigned long
        options;
    int
        count_args,
        node_arg;
    int
        QueueElementSize,
        diag_level,
        image_Q_id = 0;

    char
        log_file[MAX_STRING_LENGTH];

    QueueElementSize = sizeof(CTNDISP_Queue);
/*
 * Get the input paramaters from command line
 */
    if (argc < 3) {
	COND_PushCondition(CTNDISP_CMDLINE, "%s", CTNDISP_usage);
	COND_DumpConditions();
	exit(-1);
    }
    count_args = node_arg = 0;
    argc--;
    argv++;
    while (argc > 0) {
	if ((strcmp(*argv, "-q")) == 0) {
	    argc--;
	    argv++;
	    image_Q_id = atoi(*argv);
	    if (image_Q_id < 0) {
		COND_PushCondition(CTNDISP_CMDLINE,
				   "CTNDISP: Queue number must > 0");
		COND_DumpConditions();
		exit(-1);
	    }
	    argc--;
	    argv++;
	} else if ((strcmp(*argv, "-w")) == 0) {
	    argc--;
	    argv++;
	    G_display_width = atoi(*argv);
	    if (G_display_width < MIN_DISPLAY_WIDTH) {
		COND_PushCondition(CTNDISP_CMDLINE,
		      "CTNDISP: Display width must be > MIN_DISPLAY_WIDTH");
		COND_DumpConditions();
		exit(-1);
	    }
	    count_args++;
	    argc--;
	    argv++;
	} else if ((strcmp(*argv, "-h")) == 0) {
	    argc--;
	    argv++;
	    G_display_height = atoi(*argv);
	    if (G_display_height < MIN_DISPLAY_HEIGHT) {
		COND_PushCondition(CTNDISP_CMDLINE,
		    "CTNDISP: Display height must be > MIN_DISPLAY_HEIGHT");
		COND_DumpConditions();
		exit(-1);
	    }
	    count_args++;
	    argc--;
	    argv++;
	} else if ((strcmp(*argv, "-n")) == 0) {
	    argc--;
	    argv++;
	    strcpy(G_nodename, *argv);
	    node_arg++;
	    argc--;
	    argv++;
	} else {
	    COND_PushCondition(CTNDISP_CMDLINE, CTNDISP_usage);
	    COND_DumpConditions();
	    exit(-1);
	}
    }

    /*
     * -1's indicate to set the width and height to full screen
     */
    if (count_args != 2)
	G_display_width = G_display_height = -1;

    if (node_arg == 0)
	strcpy(G_nodename, "UNKNOWN");

/*
 * Initialize the Generalized Queue for images and status messages
 */
    if (GQ_GetQueue(image_Q_id, QueueElementSize) != GQ_NORMAL) {
	if ((GQ_InitQueue(image_Q_id, QUEUE_SIZE, QueueElementSize)) != GQ_NORMAL) {
	    COND_PushCondition(CTNDISP_NOGQ, "CTNDISP: Not able to acess Image Queue.");
	    COND_DumpConditions();
	    exit(-1);
	}
	if (GQ_GetQueue(image_Q_id, QueueElementSize) != GQ_NORMAL) {
	    COND_PushCondition(CTNDISP_NOGQ, "CTNDISP: Not able to get Image Queue.");
	    COND_DumpConditions();
	    exit(-1);
	}
    }
/*
 * Initialize the X stuff, load gray-scale color table
 */
    if ((CTNDISP_InitializeX(128)) != CTNDISP_NORMAL) {
	COND_PushCondition(CTNDISP_NOXWINDOWS,
			   "CTNDISP: No Connection to X-Server.");
	(void) GQ_KillQueue(image_Q_id);
	COND_DumpConditions();
	exit(-1);
    }
/*
 * Display node name and the status buttons...
 */
    strcpy(G_dpnname, "none");
    CTNDISP_CreateIncomingOutgoing();
    CTNDISP_CheckStatus();

/*
 * Main Control loop for ctndisp
 */
    retval = GQ_Dequeue(image_Q_id, (void *) &element);
    if (retval == GQ_NORMAL)
	strcpy(G_dpnname, element.dpnid);
    while (strcmp(element.imagefile, KILL_DISPLAY) != 0) {
	if (retval == GQ_NORMAL) {
	    switch (element.connection) {
	    case CONN_INDISPLAY:
	    case CONN_OUTDISPLAY:
		next_img.error_condition = FALSE;
		options = DCM_ORDERLITTLEENDIAN;
		data = GetDICOMData(element.imagefile, options, &next_img,
				    NO_OVERRIDE, NO_OVERRIDE, FALSE);

		if (next_img.error_condition)
		  fprintf(stderr, "Error set after GetDICOMData: %s\n",
			element.imagefile);

		CTNDISP_CheckStatus();

		if (data != NULL) {
		    new_data = ScaleImageData(data, &next_img);
		    if (new_data == NULL)
			fprintf(stderr, "ScaleImageData returned NULL ptr\n");
		    COND_DumpConditions();

		if (next_img.error_condition)
		  fprintf(stderr, "Error set after ScaleImageData: %s\n",
			element.imagefile);
		}
		CTNDISP_CheckStatus();

		CTNDISP_UnDisplayImageData(&cur_img, &xcur_img);
		CTNDISP_AdjustImageDepth(&next_img);
		if (next_img.error_condition)
		  fprintf(stderr, "Error set after AdjustImageDepth: %s\n",
			element.imagefile);
		CTNDISP_DisplayImageData(&next_img, &xnext_img);
		cur_img = next_img;
		xcur_img = xnext_img;

		CTNDISP_CheckStatus();
		break;
	    case CONN_INOPEN:
	    case CONN_INCLOSE:
	    case CONN_INXFER:
		CTNDISP_ProcessIncoming(element.connection, element.inumber);
		break;
	    case CONN_OUTOPEN:
	    case CONN_OUTCLOSE:
	    case CONN_OUTXFER:
		CTNDISP_ProcessOutgoing(element.connection, element.inumber);
		break;
	    default:
		break;
	    }
	}
	retval = GQ_Dequeue(image_Q_id, (void *) &element);
	if (retval == GQ_NORMAL)
	    strcpy(G_dpnname, element.dpnid);
	else {
#ifdef IRIX
	    (void) sginap(50);
#elif defined (USLEEP)
	    (void) usleep(100000);
#else
	    (void) sleep(1);
#endif
	}

	CTNDISP_CheckStatus();
	/*
	 * Clear the COND stack so it doesn't overflow...
	COND_DumpConditions();
	 */
	COND_PopCondition(TRUE);
    }
#ifdef MALLOC_DEBUG
    malloc_verify(0);
    malloc_shutdown();
#endif

    return 0;
}



/*******************************************************************/
void
CTNDISP_CheckStatus()
{

    CTNDISP_Queue
    element;

    XEvent
	report;

    unsigned long
        mask_event;

    CONDITION
	val;

/*
 * Check for X events that require redrawing the screen...
 */
    mask_event = StructureNotifyMask | SubstructureNotifyMask | ExposureMask;
    while (XCheckMaskEvent(G_xStruct->display, mask_event, &report) == True) {

	CTNDISP_RefreshStatus();
	if (G_FirstTime == FALSE)
	    CTNDISP_RefreshImageData(&cur_img, &xcur_img);
    }

    return;
}

#else

main()
{
    fprintf(stderr, "ctndisp requires the X Window System\n");
    fprintf(stderr, "This version was compiled without the proper MACRO\n");
    fprintf(stderr, "defined to use X11.  Please refer to the source code\n");
    fprintf(stderr, "(and recompile) to use this application\n");
}
#endif
