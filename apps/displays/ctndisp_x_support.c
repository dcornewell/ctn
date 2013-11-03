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
** Module Name(s):	CTNDISP_UnDisplayImageData,CTNDISP_LoadGrayMap,
**			CTNDISP_InitializeX, CTNDISP_RefreshImageData,
**			CTNDISP_DisplayImageData, CTNDISP_CreateIncomingOutgoing,
**			CTNDISP_RefreshStatus, CTNDISP_ProcessIncoming,
**			CTNDISP_ProcessOutgoing, CTNDISP_CreateGC(void)
** Author, Date:	David E. Beecher, 30-Jun-93
** Intent:		X support routines for ctndisp.
** Last Update:		$Author: smm $, $Date: 1999-12-23 17:07:03 $
** Source File:		$RCSfile: ctndisp_x_support.c,v $
** Revision:		$Revision: 1.9 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.9 $ $RCSfile: ctndisp_x_support.c,v $";
#ifdef X11
#include	<X11/Xlib.h>
#include	<X11/Xutil.h>
#include	<X11/Xos.h>
#include	<X11/Xatom.h>
#include	<stdio.h>
#include	"dicom.h"
#include	"condition.h"
#include	"lst.h"
#include	"xutl.h"
#include	"iqueues.h"
#include	"common_x.h"
#include	"ctndisp.h"

#ifdef SMM
typedef struct _XImageStruct {
    XImage *x_image;		/* the X image struct		 */
    Window image_window;	/* the window for display	 */
}   XImageStruct;
#endif

unsigned long graylut[256];
char *display_name = NULL;

/*
 * These variables used mostly for the status buttons.
 */

Window active_window_IN,
    active_window_OUT;
/*
    counter_window_IN,
    counter_window_OUT;
*/

static int but_width,
    but_height,
    counter_val_IN = 0,
    counter_val_OUT = 0,
    active_IN = FALSE,
    active_OUT = FALSE;

static unsigned long
    G_black_color,
    G_red_color,
    G_green_color;

XFontStruct *fontSM,
   *fontLG;

#define		BUT_SIZE_PERCT		.14
#define		BUT_SIZE_OFFSET		10
#define		NODE_X_OFFSET		10
#define		NODE_Y_OFFSET		30
#define		TITLE_ERASE_HEIGHT	40

/*
** Global variables for the X Server
*/

extern int
    G_display_width,
    G_display_height,
    G_screen,
    G_pixel_depth,
    G_store_depth,
    G_byte_order,
    G_ncolors;

extern GC
    G_gclargeT,
    G_gcsmallT,
    G_gclargeTred,
    G_gclargeTgreen,
    G_gcblack,
    G_gcred,
    G_gcgreen;

#if 0
extern Display
*   G_display;
#endif

extern Window
    G_big_window;

extern Visual
*   G_visual;

#if 0
extern Colormap
    G_cmap;
#endif

extern XUTL_STRUCT* G_xStruct;

/*
 * Other globals...
 */
extern int
    G_FirstTime;
extern char
    G_nodename[MAX_STRING_LENGTH],
    G_dpnname[MAX_STRING_LENGTH];
/*
 * End globals...
 */

#if 0
GC G_gcimage;
#endif

/*******************************************************************/
void
CTNDISP_UnDisplayImageData(ImageStruct * img, XImageStruct * ximg)
{
    if (G_FirstTime == TRUE) {
	G_FirstTime = FALSE;
	return;
    }
    if (img->error_condition == FALSE) {
	XUnmapWindow(G_xStruct->display, ximg->image_window);
	XDestroyWindow(G_xStruct->display, ximg->image_window);
	ximg->x_image->data = (char *) 0;	/* so X doesn't delete the
						 * pixel data */
	(void) XDestroyImage(ximg->x_image);
	free(img->data);
    }
    XClearArea(G_xStruct->display, G_big_window, 0, NODE_Y_OFFSET + 10, 0, 0, False);
    XFlush(G_xStruct->display);

    return;
}



/*******************************************************************/
#define LOOKUP_ENTRIES 128
#define SHIFT_FACTOR 8
#define MIN_USEFUL_AMT	16
#define	WHITE 255

CONDITION
CTNDISP_LoadGrayMap(int n)
{
    int i;
    XColor colors;
    unsigned long grayvalue;
    unsigned long graypixels[256];

    if (n <= MIN_USEFUL_AMT) {
	COND_PushCondition(CTNDISP_NOCOLORS, "%s",
		"CTNDISP: You may need to close some other X-applications");
	COND_PushCondition(CTNDISP_NOCOLORS, "%s",
			   "CTNDISP: Cannot get enough colors");
	return (CTNDISP_NOCOLORS);
    }
    colors.flags = DoRed | DoGreen | DoBlue;

    if ((G_visual->class == DirectColor) ||
	(G_visual->class == TrueColor)) {
	for (i = 0; i < n; i++)
	    graypixels[i] = i * WHITE / (n - 1);
    } else {
	for (i = 0; i < n; i++) {
	    grayvalue = i * WHITE / (n - 1);
	    colors.pixel = i;
	    colors.red = colors.green = colors.blue =
		grayvalue << SHIFT_FACTOR;
	    if (!XAllocColor(G_xStruct->display,
			     G_xStruct->colormap,
			     &colors)) {
		if (n <= MIN_USEFUL_AMT) {
		    COND_PushCondition(CTNDISP_NOCOLORS, "%s",
				       "CTNDISP: Cannot get enough colors");
		    return (CTNDISP_NOCOLORS);
		}
		XFreeColors(G_xStruct->display,
			    G_xStruct->colormap,
			    graypixels, i, 0);
		return (CTNDISP_LoadGrayMap(i));
	    } else {
		/*
		 * printf ("Color %d: %d -> %d %d %d\n", i, colors.pixel,
		 * colors.red, colors.green, colors.blue);
		 */
		graypixels[i] = colors.pixel;
	    }
	}
    }

    graylut[0] = graypixels[0];
    for (i = 1; i < LOOKUP_ENTRIES; i++)
	graylut[i] = graypixels[i / (LOOKUP_ENTRIES / n)];
    G_ncolors = n;
    return (CTNDISP_NORMAL);
}



/*******************************************************************/

CONDITION
CTNDISP_InitializeX(int ncolors)
{
    unsigned long valuemask;
    XSetWindowAttributes
	attributes;
    int width,
        height;

/*
 * Connect the client to the X server...the default display is the
 * contents of the DISPLAY environment variable
 */
#if 0
    if ((G_display = XOpenDisplay(display_name)) == NULL) {
	COND_PushCondition(CTNDISP_NOSERVER, "%s",
			   "CTNDISP: Cannot connect to X server");
	return (CTNDISP_NOSERVER);
    }
#endif
    XUTL_Open(ncolors, &G_xStruct);

    G_screen = DefaultScreen(G_xStruct->display);

    G_pixel_depth = DefaultDepth(G_xStruct->display, G_screen);

    /* right now only supports 8-bit, 16-bit or 24-bit screens */
    if (G_pixel_depth % 8) {
	COND_PushCondition(CTNDISP_NO8BIT, "%s",
			 "CTNDISP: Cannot display to Default Screen depth");
	return (CTNDISP_NO8BIT);
    }
    switch (G_pixel_depth) {
    case 8:
	G_store_depth = 1;
	break;
    case 16:
	G_store_depth = 2;
	break;
    default:
	G_store_depth = 4;
    }
    G_byte_order = ImageByteOrder(G_xStruct->display);

    height = DisplayHeight(G_xStruct->display, G_screen);
    width = DisplayWidth(G_xStruct->display, G_screen);
    if ((G_display_height > height) || (G_display_height < 0))
	G_display_height = height;
    if ((G_display_width > width) || (G_display_width < 0))
	G_display_width = width;

    G_visual = DefaultVisual(G_xStruct->display, G_screen);
#if 0
    G_cmap = DefaultColormap(G_xStruct->display, G_screen);
#endif
    /* create the graphics context */
    if ((CTNDISP_CreateGC()) != CTNDISP_NORMAL) {
	COND_PushCondition(CTNDISP_NOGRAPHICS, "%s",
			   "CTNDISP: Cannot get graphics context");
	return (CTNDISP_NOGRAPHICS);
    }
/*
 * Allocate a gray-scale colormap
 */
    if (CTNDISP_LoadGrayMap(ncolors) != CTNDISP_NORMAL) {
	COND_PushCondition(CTNDISP_NOCOLORS, "%s",
			   "CTNDISP: Cannot get enough colors");
	return (CTNDISP_NOCOLORS);
    }
    valuemask = CWBackPixel;
    attributes.background_pixel = graylut[G_ncolors / 2];
    G_big_window = XCreateWindow(G_xStruct->display,
				 RootWindow(G_xStruct->display, G_screen),
				 0, 0,
				 G_display_width,
				 G_display_height,
				 0, CopyFromParent,
				 InputOutput,
				 CopyFromParent,
				 valuemask,
				 &attributes);

    XSetStandardProperties(G_xStruct->display, G_big_window,
			   "ctn_display", "ctn_display",
			   None, (char **) NULL, 0, (XSizeHints *) NULL);

    XSelectInput(G_xStruct->display, G_big_window,
	       SubstructureNotifyMask | ExposureMask | StructureNotifyMask);
/* DEB
	       ButtonReleaseMask | SubstructureNotifyMask | ExposureMask | StructureNotifyMask);
*/

    XMapWindow(G_xStruct->display, G_big_window);
    XFlush(G_xStruct->display);
    return (CTNDISP_NORMAL);
}



/*******************************************************************/
void
CTNDISP_RefreshImageData(ImageStruct * img, XImageStruct * ximg)
{

    char patient_string[256];
    int w;

    if (img->error_condition == TRUE) {
#if 0
	w = XTextWidth(fontSM, img->error_message, strlen(img->error_message));
	XDrawString(G_xStruct->display, G_big_window, G_gcsmallT,
		    (G_display_width / 2) - (w / 2), G_display_height / 2,
		    img->error_message, strlen(img->error_message));
#endif
	XUTL_DisplayString(&G_xStruct,
			   G_big_window, 0,
			   G_display_width / 8, G_display_height / 2,
			   img->error_message);


    } else {
	XPutImage(G_xStruct->display, ximg->image_window, G_gclargeT,
		  ximg->x_image, 0, 0, 0, 0, img->ncols, img->nrows);
	strcpy(patient_string, img->patname);
	strcat(patient_string, " - ");
	strcat(patient_string, img->date);
	strcat(patient_string, " - ");
	strcat(patient_string, img->modality);
	strcat(patient_string, " - ");
	strcat(patient_string, img->study);

#ifdef CTN_MULTIBYTE
	XUTL_DisplayMBString(&G_xStruct,
			     G_big_window, 0,
			     img->xpos, img->ypos - TEXT_POSITION,
			     patient_string, img->encoding);
#else
	XUTL_DisplayString(&G_xStruct,
			   G_big_window, 0,
			   img->xpos, img->ypos - TEXT_POSITION,
			   patient_string);

#endif

#if 0
	w = XTextWidth(fontSM, patient_string, strlen(patient_string));
	XDrawString(G_xStruct->display, G_big_window, G_gcsmallT,
		 (G_display_width / 2) - (w / 2), img->ypos - TEXT_POSITION,
		    patient_string, strlen(patient_string));
#endif

    }
    XFlush(G_xStruct->display);
    return;
}


/*******************************************************************/
void
CTNDISP_AdjustImageDepth(ImageStruct * img)
{

    unsigned int npels;
    unsigned char *new_data,
       *dptr,
       *ndptr;

    /* for non-8-bit color masks */
    unsigned int depths[3],
        shifts[3],
        masks[3],
        bt,
        sz,
        r,
        g,
        b;

    /* 8-bit */
    if (G_store_depth == 1)
	return;

    npels = img->nrows * img->ncols;
    new_data = (unsigned char *) malloc(npels * G_store_depth);

    if (new_data == NULL)
	return;

    ndptr = new_data;
    dptr = img->data;

    /* 24-bit */
    if (G_store_depth == 4) {
	while (npels--) {
	    *ndptr++ = *dptr;
	    *ndptr++ = *dptr;
	    *ndptr++ = *dptr;
	    *ndptr++ = *dptr++;
	}
	free(img->data);
	img->data = new_data;
	return;
    }
    /* 16-bit */
    sz = sizeof(G_visual->red_mask) * 8;

    /* RED */
    bt = 0;
    while ((bt < sz) && !(G_visual->red_mask & (1 << bt)))
	bt++;
    shifts[0] = bt;
    depths[0] = 0;
    while ((bt < sz) && (G_visual->red_mask & (1 << bt))) {
	depths[0]++;
	bt++;
    }
    depths[0] = 8 - depths[0];
    masks[0] = G_visual->red_mask;
    /* GREEN */
    bt = 0;
    while ((bt < sz) && !(G_visual->green_mask & (1 << bt)))
	bt++;
    shifts[1] = bt;
    depths[1] = 0;
    while ((bt < sz) && (G_visual->green_mask & (1 << bt))) {
	depths[1]++;
	bt++;
    }
    depths[1] = 8 - depths[1];
    masks[1] = G_visual->green_mask;
    /* BLUE */
    bt = 0;
    while ((bt < sz) && !(G_visual->blue_mask & (1 << bt)))
	bt++;
    shifts[2] = bt;
    depths[2] = 0;
    while ((bt < sz) && (G_visual->blue_mask & (1 << bt))) {
	depths[2]++;
	bt++;
    }
    depths[2] = 8 - depths[2];
    masks[2] = G_visual->blue_mask;

    /* expand */
    while (npels--) {
	r = g = b = *dptr++;
	r = ((r >> depths[0]) << shifts[0]) & masks[0];
	g = ((g >> depths[1]) << shifts[1]) & masks[1];
	b = ((b >> depths[2]) << shifts[2]) & masks[2];
	r = r | g | b;
	if (G_byte_order == LSBFirst) {
	    *ndptr++ = (unsigned char) (r & 0xff);
	    *ndptr++ = (unsigned char) ((r >> 8) & 0xff);
	} else {
	    *ndptr++ = (unsigned char) ((r >> 8) & 0xff);
	    *ndptr++ = (unsigned char) (r & 0xff);
	}
    }

    free(img->data);
    img->data = new_data;

    return;
}


/*******************************************************************/
void
CTNDISP_DisplayImageData(ImageStruct * img, XImageStruct * ximg)
{
    unsigned long valuemask;
    XSetWindowAttributes
	attributes;

    char patient_string[256];
    int w;

    if (img->error_condition == TRUE) {
      XUTL_DisplayString(&G_xStruct,
			 G_big_window, 0,
			 G_display_width / 8, G_display_height / 2,
			 img->error_message);
#if 0
	w = XTextWidth(fontSM, img->error_message, strlen(img->error_message));
	XDrawString(G_xStruct->display, G_big_window, G_gcsmallT,
		    (G_display_width / 2) - (w / 2), G_display_height / 2,
		    img->error_message, strlen(img->error_message));
#endif

	XFlush(G_xStruct->display);
	return;
    }
    valuemask = 0;
    ximg->image_window = XCreateWindow(G_xStruct->display,
				       G_big_window,
				       img->xpos, img->ypos,
				       img->ncols, img->nrows,
				       1, CopyFromParent,
				       InputOutput,
				       CopyFromParent,
				       valuemask,
				       &attributes);

    XMapWindow(G_xStruct->display, ximg->image_window);

    ximg->x_image = XCreateImage(G_xStruct->display,
				 G_visual,
				 G_pixel_depth,
				 ZPixmap,
				 0,
				 (char *) img->data,
				 img->ncols, img->nrows,
				 G_store_depth * 8,
				 img->ncols * G_store_depth);

    XPutImage(G_xStruct->display, ximg->image_window, G_gclargeT,
	      ximg->x_image, 0, 0, 0, 0, img->ncols, img->nrows);
    strcpy(patient_string, img->patname);
    strcat(patient_string, " - ");
    strcat(patient_string, img->date);
    strcat(patient_string, " - ");
    strcat(patient_string, img->modality);
    strcat(patient_string, " - ");
    strcat(patient_string, img->study);

#ifdef CTN_MULTIBYTE
    XUTL_DisplayMBString(&G_xStruct,
			 G_big_window, 0,
			 img->xpos, img->ypos - TEXT_POSITION,
			 patient_string, img->encoding);
#else
    XUTL_DisplayString(&G_xStruct,
		       G_big_window, 0,
		       img->xpos, img->ypos - TEXT_POSITION,
		       patient_string);

#endif

#if 0
    w = XTextWidth(fontSM, patient_string, strlen(patient_string));
    XDrawString(G_xStruct->display, G_big_window, G_gcsmallT,
		(G_display_width / 2) - (w / 2), img->ypos - TEXT_POSITION,
		patient_string, strlen(patient_string));
#endif

    XFlush(G_xStruct->display);

    return;
}



/*******************************************************************/
void
CTNDISP_CreateIncomingOutgoing(void)
{
    unsigned long valuemask;
    XSetWindowAttributes
	attributes;

    char temp[MAX_STRING_LENGTH];
    int w;

    but_width = (int) (G_display_width * BUT_SIZE_PERCT);
    but_height = (int) (G_display_height * BUT_SIZE_PERCT);
    valuemask = CWBackPixel;
    attributes.background_pixel = graylut[0];

    valuemask = CWBackPixel;
    attributes.background_pixel = graylut[0];
    active_window_IN = XCreateWindow(G_xStruct->display,
				     G_big_window,
				     BUT_SIZE_OFFSET,
			    G_display_height - but_height - BUT_SIZE_OFFSET,
				     but_width, but_height,
				     0,
				     CopyFromParent,
				     InputOutput,
				     CopyFromParent,
				     valuemask,
				     &attributes);

    valuemask = CWBackPixel;
    attributes.background_pixel = graylut[0];
    active_window_OUT = XCreateWindow(G_xStruct->display,
				      G_big_window,
			      G_display_width - but_width - BUT_SIZE_OFFSET,
			    G_display_height - but_height - BUT_SIZE_OFFSET,
				      but_width, but_height,
				      0,
				      CopyFromParent,
				      InputOutput,
				      CopyFromParent,
				      valuemask,
				      &attributes);

    XMapWindow(G_xStruct->display, active_window_IN);
    XMapWindow(G_xStruct->display, active_window_OUT);
/*
 * And...display the nodename and labels while we are here...
 */
    XClearArea(G_xStruct->display, G_big_window, 0, 0, G_display_width,
	       TITLE_ERASE_HEIGHT, False);
    strcpy(temp, G_nodename);
    strcat(temp, ":");
    strcat(temp, G_dpnname);
    w = XTextWidth(fontLG, temp, strlen(temp));
    XDrawString(G_xStruct->display, G_big_window, G_gclargeT,
		(G_display_width / 2) - (w / 2), NODE_Y_OFFSET,
		temp, strlen(temp));

    sprintf(temp, "Incoming");
    w = XTextWidth(fontSM, temp, strlen(temp));
    XDrawString(G_xStruct->display, G_big_window, G_gcsmallT,
		BUT_SIZE_OFFSET + (but_width / 2) - (w / 2),
		G_display_height - (but_height + BUT_SIZE_OFFSET + 5),
		temp, strlen(temp));

    sprintf(temp, "Outgoing");
    w = XTextWidth(fontSM, temp, strlen(temp));
    XDrawString(G_xStruct->display, G_big_window, G_gcsmallT,
	      G_display_width - (but_width / 2) - BUT_SIZE_OFFSET - (w / 2),
		G_display_height - (but_height + BUT_SIZE_OFFSET + 5),
		temp, strlen(temp));

    XFlush(G_xStruct->display);

    return;
}

/*******************************************************************/
void
CTNDISP_RefreshStatus(void)
{
    char
        temp[MAX_STRING_LENGTH];
    int
        w;

    if (active_IN == TRUE) {
	CTNDISP_ProcessIncoming((short) CONN_INOPEN, (short) 0);
	CTNDISP_ProcessIncoming((short) CONN_INXFER, (short) counter_val_IN);
    } else
	CTNDISP_ProcessIncoming((short) CONN_INCLOSE, (short) 0);


    if (active_OUT == TRUE) {
	CTNDISP_ProcessOutgoing((short) CONN_OUTOPEN, (short) 0);
	CTNDISP_ProcessOutgoing((short) CONN_OUTXFER, (short) counter_val_OUT);
    } else
	CTNDISP_ProcessOutgoing((short) CONN_OUTCLOSE, (short) 0);


/*
 * Update the node name....
 */
    XClearArea(G_xStruct->display, G_big_window, 0, 0, G_display_width,
	       TITLE_ERASE_HEIGHT, False);
    strcpy(temp, G_nodename);
    strcat(temp, ":");
    strcat(temp, G_dpnname);
    w = XTextWidth(fontLG, temp, strlen(temp));
    XDrawString(G_xStruct->display, G_big_window, G_gclargeT,
		(G_display_width / 2) - (w / 2), NODE_Y_OFFSET,
		temp, strlen(temp));

    sprintf(temp, "Incoming");
    w = XTextWidth(fontSM, temp, strlen(temp));
    XDrawString(G_xStruct->display, G_big_window, G_gcsmallT,
		BUT_SIZE_OFFSET + (but_width / 2) - (w / 2),
		G_display_height - (but_height + BUT_SIZE_OFFSET + 5),
		temp, strlen(temp));

    sprintf(temp, "Outgoing");
    w = XTextWidth(fontSM, temp, strlen(temp));
    XDrawString(G_xStruct->display, G_big_window, G_gcsmallT,
	      G_display_width - (but_width / 2) - BUT_SIZE_OFFSET - (w / 2),
		G_display_height - (but_height + BUT_SIZE_OFFSET + 5),
		temp, strlen(temp));

    XFlush(G_xStruct->display);
    return;
}

/*******************************************************************/
void
CTNDISP_ProcessIncoming(short flag, short num)
{

    XSetWindowAttributes
    attributes;
    unsigned long
        valuemask;
    XColor
	color;

    char
        temp[MAX_STRING_LENGTH];
    int
        w;

    valuemask = CWBackPixel;
    switch (flag) {
    case CONN_INOPEN:
	attributes.background_pixel = G_green_color;
	XChangeWindowAttributes(G_xStruct->display,
				active_window_IN,
				valuemask,
				&attributes);
	XClearArea(G_xStruct->display, active_window_IN, 0, 0, 0, 0, False);
	active_IN = TRUE;
	break;
    case CONN_INCLOSE:
	attributes.background_pixel = G_red_color;
	XChangeWindowAttributes(G_xStruct->display, active_window_IN, valuemask, &attributes);
	XClearArea(G_xStruct->display, active_window_IN, 0, 0, 0, 0, False);
	active_IN = FALSE;
	break;
    case CONN_INXFER:
	XClearArea(G_xStruct->display, active_window_IN, 0, 0, 0, 0, False);
	sprintf(temp, "%d", num);
	w = XTextWidth(fontLG, temp, strlen(temp));
	XDrawString(G_xStruct->display, active_window_IN, G_gclargeTgreen,
		    (but_width / 2) - (w / 2), but_height - but_height / 3,
		    temp, strlen(temp));
	counter_val_IN = num;
	break;
    default:
	break;
    }
    return;
}
/*******************************************************************/
void
CTNDISP_ProcessOutgoing(short flag, short num)
{

    XSetWindowAttributes
    attributes;
    unsigned long
        valuemask;
    XColor
	color;
    char
        temp[MAX_STRING_LENGTH];
    int
        w;

    valuemask = CWBackPixel;
    switch (flag) {
    case CONN_OUTOPEN:
	attributes.background_pixel = G_green_color;
	XChangeWindowAttributes(G_xStruct->display,
				active_window_OUT,
				valuemask,
				&attributes);
	XClearArea(G_xStruct->display, active_window_OUT, 0, 0, 0, 0, False);
	active_OUT = TRUE;
	break;
    case CONN_OUTCLOSE:
	attributes.background_pixel = G_red_color;
	XChangeWindowAttributes(G_xStruct->display,
				active_window_OUT,
				valuemask,
				&attributes);
	XClearArea(G_xStruct->display, active_window_OUT, 0, 0, 0, 0, False);
	active_OUT = FALSE;
	break;
    case CONN_OUTXFER:
	XClearArea(G_xStruct->display, active_window_OUT, 0, 0, 0, 0, False);
	sprintf(temp, "%d", num);
	w = XTextWidth(fontLG, temp, strlen(temp));
	XDrawString(G_xStruct->display, active_window_OUT, G_gclargeTgreen,
		    (but_width / 2) - (w / 2), but_height - but_height / 3,
		    temp, strlen(temp));
	counter_val_OUT = num;
	break;
    default:
	break;
    }
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
CTNDISP_CreateGC(void)
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

    if ((fontsmall = XLoadFont(G_xStruct->display, tc_small)) == 0) {
	return (CTNDISP_NOFONT);
    }
    if ((fontlarge = XLoadFont(G_xStruct->display, tc_large)) == 0) {
	return (CTNDISP_NOFONT);
    }
    fontSM = XQueryFont(G_xStruct->display, fontsmall);
    fontLG = XQueryFont(G_xStruct->display, fontlarge);

    valuemask = GCForeground | GCBackground | GCFont | GCFillStyle;
    values.font = fontlarge;

    XParseColor(G_xStruct->display, G_xStruct->colormap, "white", &color);
    XAllocColor(G_xStruct->display, G_xStruct->colormap, &color);
    values.foreground = color.pixel;

    XParseColor(G_xStruct->display, G_xStruct->colormap, "black", &color);
    XAllocColor(G_xStruct->display, G_xStruct->colormap, &color);
    G_black_color = values.background = color.pixel;

    values.fill_style = FillSolid;

    if ((G_gclargeT = XCreateGC(G_xStruct->display,
			       RootWindow(G_xStruct->display, G_screen),
			       valuemask, &values)) == NULL) {
	COND_PushCondition(CTNDISP_NOGC, "%s",
			   "CTNDISP: Cannot get requested graphics context");
	return (CTNDISP_NOGC);
    }

    values.font = fontsmall;
    if ((G_gcsmallT = XCreateGC(G_xStruct->display,
				RootWindow(G_xStruct->display, G_screen),
				valuemask, &values)) == NULL) {
	COND_PushCondition(CTNDISP_NOGC, "%s",
			   "CTNDISP: Cannot get requested graphics context");
	return (CTNDISP_NOGC);
    }


    valuemask = GCForeground | GCBackground | GCFont | GCFillStyle;
    values.font = fontlarge;
    XParseColor(G_xStruct->display, G_xStruct->colormap, "red", &color);
    XAllocColor(G_xStruct->display, G_xStruct->colormap, &color);
    G_red_color = values.background = color.pixel;
    G_gclargeTred = XCreateGC(G_xStruct->display,
			      RootWindow(G_xStruct->display, G_screen),
			      valuemask, &values);
    if (G_gclargeTred == NULL) {
	COND_PushCondition(CTNDISP_NOGC, "%s",
			   "CTNDISP: Cannot get requested graphics context");
	return (CTNDISP_NOGC);
    }
    XParseColor(G_xStruct->display, G_xStruct->colormap, "green", &color);
    XAllocColor(G_xStruct->display, G_xStruct->colormap, &color);
    G_green_color = values.background = color.pixel;
    values.foreground = G_black_color;
    if ((G_gclargeTgreen = XCreateGC(G_xStruct->display,
				     RootWindow(G_xStruct->display, G_screen),
				     valuemask, &values)) == NULL) {
	COND_PushCondition(CTNDISP_NOGC, "%s",
			   "CTNDISP: Cannot get requested graphics context");
	return (CTNDISP_NOGC);
    }
/*
 * Create the colors we need...
 */
    values.font = fontlarge;
    values.background = values.foreground;
    XParseColor(G_xStruct->display, G_xStruct->colormap, "black", &color);
    XAllocColor(G_xStruct->display, G_xStruct->colormap, &color);
    values.foreground = color.pixel;
    G_gcblack = XCreateGC(G_xStruct->display,
			  RootWindow(G_xStruct->display, G_screen),
			  valuemask, &values);
    if (G_gcblack == NULL) {
	COND_PushCondition(CTNDISP_NOGC, "%s",
			   "CTNDISP: Cannot get requested graphics context");
	return (CTNDISP_NOGC);
    }
    valuemask = GCForeground | GCBackground | GCFont | GCFillStyle;
    values.font = fontlarge;

    XParseColor(G_xStruct->display, G_xStruct->colormap, "red", &color);
    XAllocColor(G_xStruct->display, G_xStruct->colormap, &color);
    values.foreground = color.pixel;

    XParseColor(G_xStruct->display, G_xStruct->colormap, "black", &color);
    XAllocColor(G_xStruct->display, G_xStruct->colormap, &color);
    values.background = color.pixel;

    values.fill_style = FillSolid;

    G_gcred = XCreateGC(G_xStruct->display,
			RootWindow(G_xStruct->display, G_screen),
			valuemask, &values);
    if (G_gcred == NULL) {
	COND_PushCondition(CTNDISP_NOGC, "%s",
			   "CTNDISP: Cannot get requested graphics context");
	return (CTNDISP_NOGC);
    }
    XParseColor(G_xStruct->display, G_xStruct->colormap, "green", &color);
    XAllocColor(G_xStruct->display, G_xStruct->colormap, &color);
    values.foreground = color.pixel;

    G_gcgreen = XCreateGC(G_xStruct->display,
			  RootWindow(G_xStruct->display, G_screen),
			  valuemask, &values);
    if (G_gcgreen == NULL) {
	COND_PushCondition(CTNDISP_NOGC, "%s",
			   "CTNDISP: Cannot get requested graphics context");
	return (CTNDISP_NOGC);
    }
    return (CTNDISP_NORMAL);
}
#endif
