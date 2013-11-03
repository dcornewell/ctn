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
** Module Name(s):	LoadGrayMap, InitializeX, RefreshImageData,
**			DisplayImageData, CreateGC
** Author, Date:	David E. Beecher, 30-Jun-93
** Intent:		X utility routines for dcm_x_disp, a general
**			purpose display utility for dicom images.
** Last Update:		$Author: smm $, $Date: 1999-12-23 17:06:07 $
** Source File:		$RCSfile: dcm_x_utils.c,v $
** Revision:		$Revision: 1.9 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.9 $ $RCSfile: dcm_x_utils.c,v $";
#ifdef X11
#include	<X11/Xlib.h>
#include	<X11/Xutil.h>
#include	<X11/Xos.h>
#include	<X11/Xatom.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	"dicom.h"
#include	"condition.h"
#include	"lst.h"
#include	"dicom_objects.h"
#include	"common_x.h"
#include	"dcm_x_disp.h"
#include	"xutl.h"


#ifdef SMM
typedef struct _XImageStruct {
    XImage *x_image;		/* the X image struct            */
    Window image_window;	/* the window for display        */
}   XImageStruct;
#endif

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
    G_pixel_depth,
    G_store_depth,
    G_byte_order,
    G_ncolors;
extern GC
/*    G_gclargeT,
    G_gcsmallT,
*/
    G_gcblack,
    G_gcred,
    G_gcgreen;

GC G_gcimage;

extern XUTL_STRUCT* G_xStruct;

/*extern Display *G_display;*/

extern Window G_big_window;

extern Visual *G_visual;

/*extern Colormap G_cmap;*/

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

    if (n <= MIN_USEFUL_AMT) {
	(void) COND_PushCondition(DICOMXDISP_NOCOLORS, "%s",
	     "DICOMXDISP: You may need to close some other X-applications");
	(void) COND_PushCondition(DICOMXDISP_NOCOLORS, "%s",
				  "DICOMXDISP: Cannot get enough colors");
	return (DICOMXDISP_NOCOLORS);
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
		    COND_PushCondition(DICOMXDISP_NOCOLORS, "%s",
				    "DICOMXDISP: Cannot get enough colors");
		    return (DICOMXDISP_NOCOLORS);
		}
		XFreeColors(G_xStruct->display, G_xStruct->colormap,
			    graypixels, i, 0);
		return (LoadGrayMap(i));
	    } else {
		graypixels[i] = colors.pixel;
	    }
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
    XUTL_Open(ncolors, &G_xStruct);

    G_screen = DefaultScreen(G_xStruct->display);

    G_pixel_depth = DefaultDepth(G_xStruct->display, G_screen);
    /* right now only supports 8-bit, 16-bit or 24-bit screens */
    if (G_pixel_depth % 8) {
	COND_PushCondition(DICOMXDISP_NO8BIT, "%s",
		      "DICOMXDISP: Cannot display to Default Screen depth");
	return (DICOMXDISP_NO8BIT);
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

    XSelectInput(G_xStruct->display, G_big_window,
		 ButtonPressMask | SubstructureNotifyMask |
		 ExposureMask | StructureNotifyMask);

    XMapWindow(G_xStruct->display, G_big_window);
    XFlush(G_xStruct->display);
    return (DICOMXDISP_NORMAL);
}



/*******************************************************************/
void
RefreshImageData(ImageStruct * img, XImageStruct * ximg)
{

    char patient_string[256];

    if (img->error_condition == TRUE) {
      XUTL_DisplayString(&G_xStruct,
			 G_big_window, 0,
			 G_display_width / 8, G_display_height / 2,
			 img->error_message);
    } else {
	XPutImage(G_xStruct->display, ximg->image_window, G_gcimage,
		  ximg->x_image, 0, 0, 0, 0, img->ncols, img->nrows);
	strcpy(patient_string, img->patname);
	strcat(patient_string, " | ");
	strcat(patient_string, img->date);
	strcat(patient_string, " | ");
	strcat(patient_string, img->modality);
	strcat(patient_string, " | ");
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
    }
    XFlush(G_xStruct->display);
    return;
}

/*******************************************************************/
void
AdjustImageDepth(ImageStruct * img)
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
DisplayImageData(ImageStruct * img, XImageStruct * ximg)
{
    unsigned long valuemask;
    XSetWindowAttributes
	attributes;

    char patient_string[256];

    if (img->error_condition == TRUE) {
      XUTL_DisplayString(&G_xStruct,
			 G_big_window, 0,
			 G_display_width / 8, G_display_height / 2,
			 img->error_message);
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
    XSetStandardProperties(G_xStruct->display, G_big_window,
			   "dcm_x_disp", "dcm_x_disp",
			   None, (char **) NULL, 0, (XSizeHints *) NULL);

    XSelectInput(G_xStruct->display, ximg->image_window, ButtonPressMask);

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

    XPutImage(G_xStruct->display, ximg->image_window, G_gcimage,
	      ximg->x_image, 0, 0, 0, 0, img->ncols, img->nrows);
    strcpy(patient_string, img->patname);
    strcat(patient_string, " | ");
    strcat(patient_string, img->date);
    strcat(patient_string, " | ");
    strcat(patient_string, img->modality);
    strcat(patient_string, " | ");
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



    XFlush(G_xStruct->display);

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

#if 0
    if ((fontsmall = XLoadFont(G_xStruct->display, tc_small)) == 0) {
	/*
	 * DICOMXDISP_DWM ("DICOMXDISP_CreateGC: Server does not support
	 * small font.\n", DIAG_SEVERE);
	 */
	return (DICOMXDISP_NOFONT);
    }
    if ((fontlarge = XLoadFont(G_xStruct->display, tc_large)) == 0) {
	/*
	 * DICOMXDISP_DWM ("DICOMXDISP_CreateGC: Server does not support
	 * large font.\n", DIAG_SEVERE);
	 */
	return (DICOMXDISP_NOFONT);
    }
#endif

    valuemask = GCForeground | GCBackground | GCFillStyle;
    /* values.font = fontlarge; */

    XParseColor(G_xStruct->display, G_xStruct->colormap, "white", &color);
    XAllocColor(G_xStruct->display, G_xStruct->colormap, &color);
    values.foreground = color.pixel;

    XParseColor(G_xStruct->display, G_xStruct->colormap, "black", &color);
    XAllocColor(G_xStruct->display, G_xStruct->colormap, &color);
    values.background = color.pixel;

    values.fill_style = FillSolid;

    if ((G_gcimage = XCreateGC(G_xStruct->display,
				RootWindow(G_xStruct->display, G_screen),
				valuemask, &values)) == NULL) {
	/*
	 * DICOMXDISP_DWM ("DICOMXDISP_CreateGC: Cannot create Large Text
	 * Graphics Context\n", DIAG_SEVERE);
	 */
	COND_PushCondition(DICOMXDISP_NOGC, "%s",
		       "DICOMXDISP: Cannot get requested graphics context");
	return (DICOMXDISP_NOGC);
    }
#if 0
    values.font = fontsmall;
    if ((G_gcsmallT = XCreateGC(G_xStruct->display,
				RootWindow(G_xStruct->display, G_screen),
				valuemask, &values)) == NULL) {
	/*
	 * DICOMXDISP_DWM ("DICOMXDISP_CreateGC: Cannot create Small Text
	 * Graphics Context\n", DIAG_SEVERE);
	 */
	COND_PushCondition(DICOMXDISP_NOGC, "%s",
		       "DICOMXDISP: Cannot get requested graphics context");
	return (DICOMXDISP_NOGC);
    }
#endif

/*
 * Create the colors we need...
 */
    /*values.font = fontlarge;*/

    values.background = values.foreground;
    XParseColor(G_xStruct->display, G_xStruct->colormap, "black", &color);
    XAllocColor(G_xStruct->display, G_xStruct->colormap, &color);
    values.foreground = color.pixel;
    G_gcblack = XCreateGC(G_xStruct->display,
			  RootWindow(G_xStruct->display, G_screen),
			  valuemask, &values);
    if (G_gcblack == NULL) {
	/*
	 * DICOMXDISP_DWM ("DICOMXDISP_CreateGC: Cannot create Black Graphics
	 * Context\n", DIAG_SEVERE);
	 */
	COND_PushCondition(DICOMXDISP_NOGC, "%s",
		       "DICOMXDISP: Cannot get requested graphics context");
	return (DICOMXDISP_NOGC);
    }
    valuemask = GCForeground | GCBackground | GCFillStyle;
    /*values.font = fontlarge; */

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
	/*
	 * DICOMXDISP_DWM ("DICOMXDISP_CreateGC: Cannot create Red Graphics
	 * Context\n", DIAG_SEVERE);
	 */
	COND_PushCondition(DICOMXDISP_NOGC, "%s",
		       "DICOMXDISP: Cannot get requested graphics context");
	return (DICOMXDISP_NOGC);
    }
    XParseColor(G_xStruct->display, G_xStruct->colormap, "green", &color);
    XAllocColor(G_xStruct->display, G_xStruct->colormap, &color);
    values.foreground = color.pixel;

    G_gcgreen = XCreateGC(G_xStruct->display,
			  RootWindow(G_xStruct->display, G_screen),
			  valuemask, &values);
    if (G_gcgreen == NULL) {
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
