/*
** The software and supporting documentation for the RSNA 1993
** DICOM Demonstration were developed at
** 	Electronic Radiology Laboratory
** 	Mallinckrodt Institute of Radiology
** 	Washington University School of Medicine
**	510 S. Kingshighway Blvd.
**	St. Louis, MO  63110
** as part of the 1993 DICOM Central Test Node
** project for, and under contract with, RSNA.  Copyright in the
** software and supporting documentation is jointly owned by RSNA
** and Washington University, and access is hereby granted
** to participating exhibitors as a limited license to
** use the software and supporting documentation only to prepare for
** and participate in the 1993 RSNA DICOM Demonstration, and all rights
** to make such use shall expire at 12:00 pm, Friday, December 3, 1993.
** Any other use of the software or supporting documentation, or any
** disclosure or transfer of the software or supporting documentation to
** any other party, is strictly prohibited.  The exhibitor may not at any
** time make any claim or representation that the exhibitor has any
** proprietary rights in the software or supporting documentation or
** that neither RSNA or Washington University has evaluated, approved
** or endorsed the exhibitor or any of its products or services.
*/
/*
+-+-+-+-+-+-+-+-+-
*/
/*
**				DICOM 93
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	LoadGrayMap, InitializeX, RefreshImageData,
**			DisplayImageData, CreateGC
** Author, Date:	David E. Beecher, 30-Jun-93
** Intent:		X utility routines for dcm_x_disp, a general
**			purpose display utility for dicom images.
** Last Update:		$Author: beech $, $Date: 1993-07-21 13:38:56 $
** Source File:		$Source: /home/smm/ctn/ctn/apps/displays/x_utils.c,v $
** Revision:		$Revision: 1.10 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.10 $ $RCSfile: x_utils.c,v $";
#ifdef X11
#include	<X11/Xlib.h>
#include	<X11/Xutil.h>
#include	<X11/Xos.h>
#include	<X11/Xatom.h>
#include	<stdio.h>
#include	"dicom.h"
#include	"lst.h"
#include	"dicom_objects.h"
#include	"dcm_x_disp.h"

typedef struct _XImageStruct {
    XImage *x_image;		/* the X image struct            */
    Window image_window;	/* the window for display        */
}   XImageStruct;

unsigned long
    graylut[256];
char
   *display_name = NULL;

/*
** Global variables for the X Server
*/

extern int
    G_display_width,
    G_display_height,
    G_screen,
    G_ncolors;
extern GC
    G_gclargeT,
    G_gcsmallT,
    G_gcblack,
    G_gcred,
    G_gcgreen;

extern Display *G_display;

extern Window G_big_window;

extern Visual *G_visual;

extern Colormap G_cmap;

/*
 * End globals...
 */

/*******************************************************************/
#define LOOKUP_ENTRIES 128
#define SHIFT_FACTOR 8
#define MIN_USEFUL_AMT	16
#define	WHITE 255

CONDITION
LoadGrayMap(int n)
{
    int i;
    XColor colors;
    unsigned long grayvalue;
    unsigned long graypixels[256];

    colors.flags = DoRed | DoGreen | DoBlue;

    for (i = 0; i < n; i++) {
	grayvalue = i * WHITE / (n - 1);
	colors.pixel = i;
	colors.red = colors.green = colors.blue =
	    grayvalue << SHIFT_FACTOR;
	if (!XAllocColor(G_display, G_cmap, &colors)) {
	    if (n <= MIN_USEFUL_AMT) {
		COND_PushCondition(DICOMXDISP_NOCOLORS, "%s",
				   "DICOMXDISP: Cannot get enough colors");
		return (DICOMXDISP_NOCOLORS);
	    }
	    XFreeColors(G_display, G_cmap, graypixels, i, 0);
	    return (LoadGrayMap(i));
	} else {
	    graypixels[i] = colors.pixel;
	}
    }
    graylut[0] = graypixels[0];
    for (i = 1; i < LOOKUP_ENTRIES; i++)
	graylut[i] = graypixels[i / (LOOKUP_ENTRIES / n)];
    G_ncolors = n;
    return (DICOMXDISP_NORMAL);
}



/*******************************************************************/
CONDITION
InitializeX(int ncolors)
{
    unsigned long valuemask;
    XSetWindowAttributes
	attributes;
    int width,
        height;

    /*
     * DICOMXDISP_DWM ("DICOMXDISP_InitializeX: Initialize X system\n",
     * DIAG_INFORM);
     */
/*
 * Connect the client to the X server...the default display is the
 * contents of the DISPLAY environment variable
 */
    if ((G_display = XOpenDisplay(display_name)) == NULL) {
	COND_PushCondition(DICOMXDISP_NOSERVER, "DICOMXDISP: Cannot connect to X server");
	return (DICOMXDISP_NOSERVER);
    }
    G_screen = DefaultScreen(G_display);
    height = DisplayHeight(G_display, G_screen);
    width = DisplayWidth(G_display, G_screen);
    if ((G_display_height > height) || (G_display_height < 0))
	G_display_height = height;
    if ((G_display_width > width) || (G_display_width < 0))
	G_display_width = width;
    G_visual = DefaultVisual(G_display, G_screen);
    G_cmap = DefaultColormap(G_display, G_screen);

    /* create the graphics context */
    if ((CreateGC()) != DICOMXDISP_NORMAL) {
	COND_PushCondition(DICOMXDISP_NOGRAPHICS, "%s",
			   "DICOMXDISP: Cannot get graphics context");
	return (DICOMXDISP_NOGRAPHICS);
    }
/*
 * Allocate a gray-scale colormap
 */
    if (LoadGrayMap(ncolors) != DICOMXDISP_NORMAL) {
	COND_PushCondition(DICOMXDISP_NOCOLORS, "%s",
			   "DICOMXDISP: Cannot get enough colors");
	return (DICOMXDISP_NOCOLORS);
    }
/*
 * G_ncolors now contains the number of colors we acually got!
 */
    valuemask = CWBackPixel;
    attributes.background_pixel = graylut[G_ncolors / 2];
    G_big_window = XCreateWindow(G_display,
				 RootWindow(G_display, G_screen),
				 0, 0,
				 G_display_width,
				 G_display_height,
				 0, CopyFromParent,
				 InputOutput,
				 CopyFromParent,
				 valuemask,
				 &attributes);

    XSelectInput(G_display, G_big_window,
		 ButtonPressMask | SubstructureNotifyMask | ExposureMask | StructureNotifyMask);

    XMapWindow(G_display, G_big_window);
    XFlush(G_display);
    return (DICOMXDISP_NORMAL);
}



/*******************************************************************/
void
RefreshImageData(ImageStruct * img, XImageStruct * ximg)
{

    char patient_string[256];

    if (img->error_condition == TRUE) {
	XDrawString(G_display, G_big_window, G_gcsmallT,
		    G_display_width / 8, G_display_height / 2,
		    img->error_message, strlen(img->error_message));
    } else {
	XPutImage(G_display, ximg->image_window, G_gclargeT,
		  ximg->x_image, 0, 0, 0, 0, img->ncols, img->nrows);
	strcpy(patient_string, img->patname);
	strcat(patient_string, " | ");
	strcat(patient_string, img->date);
	strcat(patient_string, " | ");
	strcat(patient_string, img->modality);
	strcat(patient_string, " | ");
	strcat(patient_string, img->study);
	XDrawString(G_display, G_big_window, G_gcsmallT,
		    img->xpos, img->ypos - TEXT_POSITION,
		    patient_string, strlen(patient_string));
    }
    XFlush(G_display);
    return;
}



/*******************************************************************/
void
DisplayImageData(ImageStruct * img, XImageStruct * ximg)
{
    unsigned long valuemask;
    XSetWindowAttributes
	attributes;

    char patient_string[256];

    if (img->error_condition == TRUE) {
	XDrawString(G_display, G_big_window, G_gcsmallT,
		    G_display_width / 8, G_display_height / 2,
		    img->error_message, strlen(img->error_message));
	XFlush(G_display);
	return;
    }
    valuemask = 0;
    ximg->image_window = XCreateWindow(G_display,
				       G_big_window,
				       img->xpos, img->ypos,
				       img->ncols, img->nrows,
				       1, CopyFromParent,
				       InputOutput,
				       CopyFromParent,
				       valuemask,
				       &attributes);
    XSetStandardProperties(G_display, G_big_window, "dcm_x_disp", "dcm_x_disp",
			   None, (char **) NULL, 0, (XSizeHints *) NULL);

    XSelectInput(G_display, ximg->image_window, ButtonPressMask);

    XMapWindow(G_display, ximg->image_window);

    ximg->x_image = XCreateImage(G_display,
				 G_visual,
				 8,
				 ZPixmap,
				 0,
				 (char *) img->data,
				 img->ncols, img->nrows,
				 8, img->ncols);

    XPutImage(G_display, ximg->image_window, G_gclargeT,
	      ximg->x_image, 0, 0, 0, 0, img->ncols, img->nrows);
    strcpy(patient_string, img->patname);
    strcat(patient_string, " | ");
    strcat(patient_string, img->date);
    strcat(patient_string, " | ");
    strcat(patient_string, img->modality);
    strcat(patient_string, " | ");
    strcat(patient_string, img->study);
    XDrawString(G_display, G_big_window, G_gcsmallT,
		img->xpos, img->ypos - TEXT_POSITION,
		patient_string, strlen(patient_string));
    XFlush(G_display);

    return;
}


/*
 * Remember this for future reference
 *
	char    *fontname1 =
			"-adobe-helvetica-medium-r-normal--14-*";
	char	*fontname2 =
		"-adobe-helvetica-bold-r-normal-*-24-*-*-*-*-*-*-*";
*/

#ifdef ULTRIX

#define FONT_SM_A	"-adobe-helvetica-*-r-normal--14-*"
#define FONT_LG_A	"-adobe-*-*-r-normal--24-*"

#define FONT_SM_B	"-*-*-*-*-*--10-*"
#define FONT_LG_B	"-*-*-*-*-*--16-*"

#else

#define FONT_SM_A 	"-adobe-helvetica-bold-r-normal--14-*"
#define FONT_LG_A	"-adobe-helvetica-bold-r-normal--24-*"

#define FONT_SM_B 	"-adobe-helvetica-medium-r-normal--10-*"
#define FONT_LG_B	"-adobe-helvetica-medium-r-normal--18-*"

#endif
/*******************************************************************/
CONDITION
CreateGC(void)
{
    unsigned long valuemask;
    XColor color;
    XGCValues values;
    char tc_small[MAX_STRING_LENGTH],
        tc_large[MAX_STRING_LENGTH];
    Font fontsmall,
        fontlarge;

    if ((G_display_height < 400) || (G_display_width < 400)) {
	strcpy(tc_small, FONT_SM_B);
	strcpy(tc_large, FONT_LG_B);
    } else {
	strcpy(tc_small, FONT_SM_A);
	strcpy(tc_large, FONT_LG_A);
    }

    if ((fontsmall = XLoadFont(G_display, tc_small)) == 0) {
	/*
	 * DICOMXDISP_DWM ("DICOMXDISP_CreateGC: Server does not support
	 * small font.\n", DIAG_SEVERE);
	 */
	return (DICOMXDISP_NOFONT);
    }
    if ((fontlarge = XLoadFont(G_display, tc_large)) == 0) {
	/*
	 * DICOMXDISP_DWM ("DICOMXDISP_CreateGC: Server does not support
	 * large font.\n", DIAG_SEVERE);
	 */
	return (DICOMXDISP_NOFONT);
    }
    valuemask = GCForeground | GCBackground | GCFont | GCFillStyle;
    values.font = fontlarge;

    XParseColor(G_display, G_cmap, "white", &color);
    XAllocColor(G_display, G_cmap, &color);
    values.foreground = color.pixel;

    XParseColor(G_display, G_cmap, "black", &color);
    XAllocColor(G_display, G_cmap, &color);
    values.background = color.pixel;

    values.fill_style = FillSolid;

    if ((G_gclargeT = XCreateGC(G_display, RootWindow(G_display, G_screen),
				valuemask, &values)) == NULL) {
	/*
	 * DICOMXDISP_DWM ("DICOMXDISP_CreateGC: Cannot create Large Text
	 * Graphics Context\n", DIAG_SEVERE);
	 */
	COND_PushCondition(DICOMXDISP_NOGC, "%s",
		       "DICOMXDISP: Cannot get requested graphics context");
	return (DICOMXDISP_NOGC);
    }
    values.font = fontsmall;
    if ((G_gcsmallT = XCreateGC(G_display, RootWindow(G_display, G_screen),
				valuemask, &values)) == NULL) {
	/*
	 * DICOMXDISP_DWM ("DICOMXDISP_CreateGC: Cannot create Small Text
	 * Graphics Context\n", DIAG_SEVERE);
	 */
	COND_PushCondition(DICOMXDISP_NOGC, "%s",
		       "DICOMXDISP: Cannot get requested graphics context");
	return (DICOMXDISP_NOGC);
    }
/*
 * Create the colors we need...
 */
    values.font = fontlarge;
    values.background = values.foreground;
    XParseColor(G_display, G_cmap, "black", &color);
    XAllocColor(G_display, G_cmap, &color);
    values.foreground = color.pixel;
    if ((G_gcblack = XCreateGC(G_display, RootWindow(G_display, G_screen),
			       valuemask, &values)) == NULL) {
	/*
	 * DICOMXDISP_DWM ("DICOMXDISP_CreateGC: Cannot create Black Graphics
	 * Context\n", DIAG_SEVERE);
	 */
	COND_PushCondition(DICOMXDISP_NOGC, "%s",
		       "DICOMXDISP: Cannot get requested graphics context");
	return (DICOMXDISP_NOGC);
    }
    valuemask = GCForeground | GCBackground | GCFont | GCFillStyle;
    values.font = fontlarge;

    XParseColor(G_display, G_cmap, "red", &color);
    XAllocColor(G_display, G_cmap, &color);
    values.foreground = color.pixel;

    XParseColor(G_display, G_cmap, "black", &color);
    XAllocColor(G_display, G_cmap, &color);
    values.background = color.pixel;

    values.fill_style = FillSolid;

    if ((G_gcred = XCreateGC(G_display, RootWindow(G_display, G_screen),
			     valuemask, &values)) == NULL) {
	/*
	 * DICOMXDISP_DWM ("DICOMXDISP_CreateGC: Cannot create Red Graphics
	 * Context\n", DIAG_SEVERE);
	 */
	COND_PushCondition(DICOMXDISP_NOGC, "%s",
		       "DICOMXDISP: Cannot get requested graphics context");
	return (DICOMXDISP_NOGC);
    }
    XParseColor(G_display, G_cmap, "green", &color);
    XAllocColor(G_display, G_cmap, &color);
    values.foreground = color.pixel;

    if ((G_gcgreen = XCreateGC(G_display, RootWindow(G_display, G_screen),
			       valuemask, &values)) == NULL) {
	/*
	 * DICOMXDISP_DWM ("DICOMXDISP_CreateGC: Cannot create Green Graphics
	 * Context\n", DIAG_SEVERE);
	 */
	COND_PushCondition(DICOMXDISP_NOGC, "%s",
		       "DICOMXDISP: Cannot get requested graphics context");
	return (DICOMXDISP_NOGC);
    }
    return (DICOMXDISP_NORMAL);
}
#endif
