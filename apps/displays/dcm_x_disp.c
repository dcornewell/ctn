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
** Module Name(s):	main(), (dcm_x_disp)
** Author, Date:	David E. Beecher, 24-June-93
** Intent:		dcm_x_disp is a simple routine to display an ACR-NEMA
**			image file on an X display
** Last Update:		$Author: smm $, $Date: 2002-12-13 15:18:25 $
** Source File:		$RCSfile: dcm_x_disp.c,v $
** Revision:		$Revision: 1.14 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.14 $ $RCSfile: dcm_x_disp.c,v $";

#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<sys/time.h>
#ifdef X11
#include        <X11/Xlib.h>
#include        <X11/Xutil.h>
#include        <X11/Xos.h>
#include        <X11/Xatom.h>

#include	"dicom.h"
#include	"ctnthread.h"
#include	"condition.h"
#include	"lst.h"
#include	"dicom_objects.h"
#include	"xutl.h"
#include	"common_x.h"
#include 	"dcm_x_disp.h"

char *DICOMXDISP_usage =
"Usage: dcm_x_disp [-b] [-t] [-w display_width -h display_height] [-W width] [-C center] <dicom image file>\n";
/*
 * A macro to compute the change in time for timing values...
 */
#define DELTATIME(A,B) ((double)((B).tv_sec-(A).tv_sec)) + \
	((double)((B).tv_usec-(A).tv_usec)) /1000000.

#ifdef SMM
typedef struct _XImageStruct {
    XImage *x_image;		/* the X image struct            */
    Window image_window;	/* the window for display        */
}   XImageStruct;
#endif

void DisplayImageData(ImageStruct * img, XImageStruct * ximg);
void RefreshImageData(ImageStruct * img, XImageStruct * ximg);

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
#if 0
  G_gclargeT,
    G_gcsmallT,
#endif
    G_gcblack,			/* Graphics contexts needed...	 */
    G_gcred,
    G_gcgreen;

int G_display_width,
    G_display_height,		/* Well know screen stuff...	 */
    G_pixel_depth,
    G_store_depth,
    G_byte_order,
    G_screen,
    G_ncolors;

int G_FirstTime = TRUE;

char G_nodename[MAX_STRING_LENGTH];

XUTL_STRUCT* G_xStruct = 0;
/*
 * End Globals...
 */

ImageStruct cur_img,
    next_img;

XImageStruct xcur_img,
    xnext_img;




/*******************************************************************/
int
main(int argc, char **argv)
{
    XEvent
    report;
    CONDITION
	retval;
    void
       *data;
    unsigned char
       *new_data;
    int count_args,
        file_arg;
    int study_Q_id,
        status_Q_id;

    struct timeval
        t_start,		/* Variables to compute timings... */
        t_end;
    struct timezone
        t_zone;

    double
        deltaTime;

    char dicom_file[MAX_STRING_LENGTH];

    unsigned long options = DCM_ORDERLITTLEENDIAN;
    int overrideWidth = NO_OVERRIDE,
        overrideCenter = NO_OVERRIDE;

    CTNBOOLEAN verbose = FALSE;
    CTNBOOLEAN useImport = FALSE;

/*
 * Get the input paramaters from command line
 */
    if (argc < 2) {
	COND_PushCondition(DICOMXDISP_CMDLINE, DICOMXDISP_usage);
	COND_DumpConditions();
	return (-1);
    }
    count_args = file_arg = 0;
    argc--;
    argv++;
    while (argc > 0) {
	if (strcmp(*argv, "-b") == 0) {
	    argc--;
	    argv++;
	    options &= ~DCM_ORDERMASK;
	    options |= DCM_ORDERBIGENDIAN;
	} else if ((strcmp(*argv, "-i")) == 0) {
	    argc--;
	    argv++;
	    useImport = TRUE;
	} else if ((strcmp(*argv, "-t")) == 0) {
	    argc--;
	    argv++;
	    options &= ~DCM_FILEFORMATMASK;
	    options |= DCM_PART10FILE;
	} else if ((strcmp(*argv, "-v")) == 0) {
	    verbose = TRUE;
	} else if ((strcmp(*argv, "-w")) == 0) {
	    argc--;
	    argv++;
	    G_display_width = atoi(*argv);
	    if (G_display_width < MIN_DISPLAY_WIDTH) {
		COND_PushCondition(DICOMXDISP_CMDLINE,
		  "%s: Display width must be > MIN_DISPLAY_WIDTH", argv[0]);
		COND_DumpConditions();
		return (-1);
	    }
	    count_args++;
	    argc--;
	    argv++;
	} else if ((strcmp(*argv, "-h")) == 0) {
	    argc--;
	    argv++;
	    G_display_height = atoi(*argv);
	    if (G_display_height < MIN_DISPLAY_HEIGHT) {
		COND_PushCondition(DICOMXDISP_CMDLINE,
		"%s: Display height must be > MIN_DISPLAY_HEIGHT", argv[0]);
		COND_DumpConditions();
		return (-1);
	    }
	    count_args++;
	    argc--;
	    argv++;
	} else if ((strcmp(*argv, "-W")) == 0) {
	    argc--;
	    argv++;
	    overrideWidth = atoi(*argv);
	    argc--;
	    argv++;
	} else if ((strcmp(*argv, "-C")) == 0) {
	    argc--;
	    argv++;
	    overrideCenter = atoi(*argv);
	    argc--;
	    argv++;
	} else {
	    strcpy(dicom_file, *argv);
	    file_arg++;
	    argc--;
	    argv++;
	}
    }
    if (count_args != 2)	/* -1's indicate to set the width and height
				 * to full screen */
	G_display_width = G_display_height = -1;

    THR_Init();
    if (file_arg != 1) {
	COND_PushCondition(DICOMXDISP_CMDLINE, "No filename specified");
	COND_DumpConditions();
	THR_Shutdown();
	return (-1);
    }
    /* Initialize the X stuff, load gray-scale color table */
    if ((InitializeX(128)) != DICOMXDISP_NORMAL) {
	COND_PushCondition(DICOMXDISP_NOXWINDOWS,
			   "No Connection to X-Server.");
	COND_DumpConditions();
	THR_Shutdown();
	return (-1);
    }
/*
 * Main Control loop for acrxdisp
 */
    next_img.error_condition = FALSE;

    gettimeofday(&t_start, &t_zone);

    data = GetDICOMData(dicom_file, options, &next_img, overrideWidth, overrideCenter, useImport);

    gettimeofday(&t_end, &t_zone);
    deltaTime = DELTATIME(t_start, t_end);

    if (data != NULL) {
	gettimeofday(&t_start, &t_zone);

	if (overrideWidth != NO_OVERRIDE) {
	    COND_PushCondition(DICOMXDISP, "  New Window Width: %d", overrideWidth);
	    next_img.window = overrideWidth;
	}
	if (overrideCenter != NO_OVERRIDE) {
	    COND_PushCondition(DICOMXDISP, " New Window Center: %d", overrideCenter);
	    next_img.level = overrideCenter;
	}
	new_data = ScaleImageData(data, &next_img);
	if (new_data == NULL)
	    fprintf(stderr, "ScaleImageData returned null pointer\n");

	gettimeofday(&t_end, &t_zone);
	deltaTime = DELTATIME(t_start, t_end);
    }
    gettimeofday(&t_start, &t_zone);
    /* 8-bit data must be converted if G_pixel_depth != 8 */
    AdjustImageDepth(&next_img);

    DisplayImageData(&next_img, &xnext_img);
    COND_DumpConditions();
    cur_img = next_img;
    xcur_img = xnext_img;

    gettimeofday(&t_end, &t_zone);
    deltaTime = DELTATIME(t_start, t_end);
    if (verbose)
	printf("Time to get to display is: %f\n", deltaTime);

    Refresh();

    THR_Shutdown();
    return 0;
}



/*******************************************************************/
void
Refresh(void)
{

    XEvent report;
    unsigned long mask_event;
    int count = 0;

/*
 * Check for X events that require redrawing the screen...
 */
    while (1) {
	mask_event =
	    ButtonPressMask | StructureNotifyMask | ExposureMask;
	while (XCheckMaskEvent(G_xStruct->display,
			       mask_event,
			       &report) == True) {
	    /* put it back first before checking for ButtonPress */
	    XPutBackEvent(G_xStruct->display, &report);
	    if (cur_img.error_condition == FALSE) {
		if (XCheckTypedWindowEvent(G_xStruct->display,
		       xcur_img.image_window, ButtonPress, &report) == True)
		    return;
	    } else {
		if (XCheckTypedWindowEvent(G_xStruct->display,
				G_big_window, ButtonPress, &report) == True)
		    return;
	    }
	    RefreshImageData(&cur_img, &xcur_img);
	}
	(void) sleep(1);
    }

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
