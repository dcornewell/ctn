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
** Module Name(s):	CTNNETWORK_InitializeX, CTNNETWORK_RefreshStatus,
**			CTNNETWORK_CreateGC, CTNNETWORK_CreateNetworkWindow,
**			CTNNETWORK_CloseConnection, CTNNETWORK_ShowPercentage,
**			CTNNETWORK_OpenConnection, CTNNETWORK_RefreshStatus
** Author, Date:	David E. Beecher, 30-Jun-93
** Intent:		X support routines for ctnnetwork.
** Last Update:		$Author: smm $, $Date: 1999-11-02 21:36:31 $
** Source File:		$RCSfile: ctnnetwork_x_support.c,v $
** Revision:		$Revision: 1.20 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.20 $ $RCSfile: ctnnetwork_x_support.c,v $";
#ifdef X11
#include	<X11/Xlib.h>
#include	<X11/Xutil.h>
#include	<X11/Xos.h>
#include	<X11/Xatom.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	"dicom.h"
#include	"condition.h"
#include	"iqueues.h"
#include	"ctnnetwork.h"

char *display_name = NULL;

/*
 * These variables used mostly for the status buttons.
 */

static Window
   *Bar;

static int
    bar_width,
    bar_height;

static unsigned long
    G_black_color,
    G_white_color,
    G_red_color,
    G_green_color;

XFontStruct
*fontLG;

#define 	BAR_SPACING		5
#define		BORDER_WIDTH		2
#define 	ARROW_PERCENTAGE	.060
#define		BAR_PERCENTAGE		.60

/*
** Global variables for the X Server
*/

extern int
    G_display_width,
    G_display_height,
    G_screen;

extern GC
    G_gclargeT,
    G_gcgreen;

extern Display
   *G_display;

extern Window
    G_big_window;

/*static Visual
   *G_visual; smm, do I really need this variable?*/

extern Colormap
    G_cmap;

/*
 * Other globals...
 */
extern char
    G_nodename[MAX_STRING_LENGTH],
    G_dpnname[MAX_STRING_LENGTH];

extern CTNNETWORK_Queue
   *G_Array;
extern int
    G_Number,
   *G_Used;
/*
 * End globals...
 */


/*******************************************************************/
void
CTNNETWORK_CreateNetworkWindow(void)
{
    unsigned long valuemask;
    XSetWindowAttributes
	attributes;

    int
        ypos,
        i;

    char
        temp[MAX_STRING_LENGTH];
    int
        w,
        h;

    int
        dir,
        fa,
        fd;

    XCharStruct
	or;

    Bar = (Window *) malloc(G_Number * sizeof(Window));

    bar_width = G_display_width - (BORDER_WIDTH * 3);
    bar_height = ((G_display_height - (G_Number * BAR_SPACING)) / G_Number);

    ypos = 0;
    for (i = 0; i < G_Number; i++) {
	valuemask = CWBackPixel;
	attributes.background_pixel = G_black_color;
	Bar[i] = XCreateWindow(G_display,
			       G_big_window,
			       BORDER_WIDTH,
			       ypos,
			       bar_width, bar_height,
			       0,
			       CopyFromParent,
			       InputOutput,
			       CopyFromParent,
			       valuemask,
			       &attributes);
	XMapWindow(G_display, Bar[i]);
	XClearWindow(G_display, Bar[i]);
	sprintf(temp, "No Traffic");
	XTextExtents(fontLG, temp, strlen(temp), &dir, &fa, &fd, &or);
	w = or.width;
	h = or.ascent;
	XDrawString(G_display, Bar[i], G_gclargeT,
		    (bar_width - w) / 2,
		    ((bar_height - h) / 2) + h, temp, strlen(temp));
	ypos += bar_height + BAR_SPACING;
    }
    XFlush(G_display);

    return;
}

/*******************************************************************/
void
CTNNETWORK_CloseConnection(int slot)
{

    char
        temp[100];
    int
        w,
        h;
    int
        dir,
        fa,
        fd;
    XCharStruct
	or;

    if (slot >= 0 && slot < G_Number) {
	XSetWindowBackground(G_display, Bar[slot], G_red_color);
	XClearWindow(G_display, Bar[slot]);
	XSetWindowBorderWidth(G_display, Bar[slot], BORDER_WIDTH);
	XSetWindowBorder(G_display, Bar[slot], G_white_color);
	if (G_Array[slot].connection == CONN_INCLOSE) {
	    sprintf(temp, "Close [%s] ==> CTN (%d)",
		    G_Array[slot].dpnid,
		    G_Array[slot].percentage);
	} else {
	    sprintf(temp, "Close [%s] <== CTN (%d)",
		    G_Array[slot].dpnid,
		    G_Array[slot].percentage);
	}
	XTextExtents(fontLG, temp, strlen(temp), &dir, &fa, &fd, &or);
	w = or.width;
	h = or.ascent;
	XDrawString(G_display, Bar[slot], G_gclargeT,
		    (bar_width - w) / 2,
		    ((bar_height - h) / 2) + h,
		    temp, strlen(temp));
	XFlush(G_display);
    }
}

/*******************************************************************/
void
CTNNETWORK_ShowPercentage(int slot)
{

    char
        temp[100];
    int
        w,
        h;
    int
        dir,
        fa,
        fd;
    XCharStruct
	or;
    XPoint
	pt[3];
    int
        sp,
        sph;

    sp = (bar_height - (int) (BAR_PERCENTAGE * bar_height)) / 2 + 1;
    if (G_Array[slot].percentage > 100)
	G_Array[slot].percentage = 100;
    if (slot >= 0 && slot < G_Number) {
	XSetWindowBorderWidth(G_display, Bar[slot], 0);
	XSetWindowBackground(G_display, Bar[slot], G_black_color);
	XClearWindow(G_display, Bar[slot]);
	if (G_Array[slot].connection == CONN_INXFER) {
	    XFillRectangle(G_display, Bar[slot], G_gcgreen,
			   0, 0 + sp,
			   (G_Array[slot].percentage * bar_width) / 100,
			   (unsigned int) (bar_height * BAR_PERCENTAGE));
	    pt[0].x = pt[1].x = (G_Array[slot].percentage * bar_width) / 100;
	    pt[0].y = 0;
	    pt[1].y = bar_height;
	    pt[2].x = pt[0].x + (short) (bar_width * ARROW_PERCENTAGE);
	    pt[2].y = bar_height / 2;
	    XFillPolygon(G_display, Bar[slot], G_gcgreen, pt, 3, Convex, CoordModeOrigin);
	    sprintf(temp, "[%s] ==> CTN", G_Array[slot].dpnid);
	} else {
	    XFillRectangle(G_display, Bar[slot], G_gcgreen,
			   bar_width - ((G_Array[slot].percentage * bar_width) / 100), 0 + sp,
			   (G_Array[slot].percentage * bar_width) / 100,
			   (unsigned int) (bar_height * BAR_PERCENTAGE));
	    pt[0].x = pt[1].x = bar_width - ((G_Array[slot].percentage * bar_width) / 100);
	    pt[0].y = 0;
	    pt[1].y = bar_height;
	    pt[2].x = pt[0].x - (short) (bar_width * ARROW_PERCENTAGE);
	    pt[2].y = bar_height / 2;
	    XFillPolygon(G_display, Bar[slot], G_gcgreen, pt, 3, Convex, CoordModeOrigin);
	    sprintf(temp, "[%s] <== CTN", G_Array[slot].dpnid);
	}
	XTextExtents(fontLG, temp, strlen(temp), &dir, &fa, &fd, &or);
	w = or.width;
	h = or.ascent;
	XDrawString(G_display, Bar[slot], G_gclargeT,
		    (bar_width - w) / 2,
		    ((bar_height - h) / 2) + h,
		    temp, strlen(temp));
	XFlush(G_display);
    }
}

/*******************************************************************/
void
CTNNETWORK_OpenConnection(int slot)
{

    char
        temp[100];
    int
        w,
        h;
    int
        dir,
        fa,
        fd;
    XCharStruct
	or;

    if (slot >= 0 && slot < G_Number) {
	XSetWindowBackground(G_display, Bar[slot], G_black_color);
	XClearWindow(G_display, Bar[slot]);
	XSetWindowBorderWidth(G_display, Bar[slot], BORDER_WIDTH);
	XSetWindowBorder(G_display, Bar[slot], G_green_color);
	if (G_Array[slot].connection == CONN_INOPEN) {
	    sprintf(temp, "Open [%s] ==> CTN", G_Array[slot].dpnid);
	} else {
	    sprintf(temp, "Open [%s] <== CTN", G_Array[slot].dpnid);
	}
	XTextExtents(fontLG, temp, strlen(temp), &dir, &fa, &fd, &or);
	w = or.width;
	h = or.ascent;
	XDrawString(G_display, Bar[slot], G_gclargeT,
		    (G_display_width - w) / 2,
		    ((bar_height - h) / 2) + h,
		    temp, strlen(temp));
	XFlush(G_display);
    }
}

/*******************************************************************/
void
CTNNETWORK_RefreshStatus(void)
{
    char
        temp[MAX_STRING_LENGTH];
    int
        i,
        w,
        h;
    int
        dir,
        fa,
        fd;
    XCharStruct
	or;
    XPoint
	pt[3];
    int
        sp;

    sp = (bar_height - (int) (BAR_PERCENTAGE * bar_height)) / 2 + 1;

    for (i = 0; i < G_Number; i++) {
	if ((G_Used[i] == 1) || (G_Array[i].connection == CONN_INCLOSE) ||
	    (G_Array[i].connection == CONN_OUTCLOSE)) {
	    switch (G_Array[i].connection) {
	    case CONN_INOPEN:
		XSetWindowBackground(G_display, Bar[i], G_black_color);
		XClearWindow(G_display, Bar[i]);
		XSetWindowBorderWidth(G_display, Bar[i], BORDER_WIDTH);
		XSetWindowBorder(G_display, Bar[i], G_green_color);
		sprintf(temp, "Open [%s] ==> CTN", G_Array[i].dpnid);
		XTextExtents(fontLG, temp, strlen(temp), &dir, &fa, &fd, &or);
		w = or.width;
		h = or.ascent;
		XDrawString(G_display, Bar[i], G_gclargeT,
			    (G_display_width - w) / 2,
			    ((bar_height - h) / 2) + h,
			    temp, strlen(temp));
		break;
	    case CONN_OUTOPEN:
		XSetWindowBackground(G_display, Bar[i], G_black_color);
		XClearWindow(G_display, Bar[i]);
		XSetWindowBorderWidth(G_display, Bar[i], BORDER_WIDTH);
		XSetWindowBorder(G_display, Bar[i], G_green_color);
		sprintf(temp, "Open [%s] <== CTN", G_Array[i].dpnid);
		XTextExtents(fontLG, temp, strlen(temp), &dir, &fa, &fd, &or);
		w = or.width;
		h = or.ascent;
		XDrawString(G_display, Bar[i], G_gclargeT,
			    (G_display_width - w) / 2,
			    ((bar_height - h) / 2) + h, temp, strlen(temp));
		break;
	    case CONN_INCLOSE:
		XSetWindowBackground(G_display, Bar[i], G_red_color);
		XClearWindow(G_display, Bar[i]);
		sprintf(temp, "Close [%s] ==> CTN (%d)",
			G_Array[i].dpnid,
			G_Array[i].percentage);
		XTextExtents(fontLG, temp, strlen(temp), &dir, &fa, &fd, &or);
		w = or.width;
		h = or.ascent;
		XDrawString(G_display, Bar[i], G_gclargeT,
			    (bar_width - w) / 2,
			    ((bar_height - h) / 2) + h, temp, strlen(temp));
		break;
	    case CONN_OUTCLOSE:
		XSetWindowBackground(G_display, Bar[i], G_red_color);
		XClearWindow(G_display, Bar[i]);
		sprintf(temp, "Close [%s] <== CTN (%d)",
			G_Array[i].dpnid,
			G_Array[i].percentage);
		XTextExtents(fontLG, temp, strlen(temp), &dir, &fa, &fd, &or);
		w = or.width;
		h = or.ascent;
		XDrawString(G_display, Bar[i], G_gclargeT,
			    (bar_width - w) / 2,
			    ((bar_height - h) / 2) + h, temp, strlen(temp));
		break;
	    case CONN_INXFER:
		XSetWindowBorderWidth(G_display, Bar[i], 0);
		XSetWindowBackground(G_display, Bar[i], G_black_color);
		XClearWindow(G_display, Bar[i]);
		XFillRectangle(G_display, Bar[i], G_gcgreen,
			       0, 0 + sp,
			       (G_Array[i].percentage * bar_width) / 100,
			       (unsigned int) (bar_height * BAR_PERCENTAGE));
		pt[0].x = pt[1].x = (G_Array[i].percentage * bar_width) / 100;
		pt[0].y = 0;
		pt[1].y = bar_height;
		pt[2].x = pt[0].x + (short) (bar_width * ARROW_PERCENTAGE);
		pt[2].y = bar_height / 2;
		XFillPolygon(G_display, Bar[i], G_gcgreen, pt, 3, Convex, CoordModeOrigin);
		sprintf(temp, "[%s] ==> CTN", G_Array[i].dpnid);
		XTextExtents(fontLG, temp, strlen(temp), &dir, &fa, &fd, &or);
		w = or.width;
		h = or.ascent;
		XDrawString(G_display, Bar[i], G_gclargeT,
			    (bar_width - w) / 2,
			    ((bar_height - h) / 2) + h, temp, strlen(temp));
		break;
	    case CONN_OUTXFER:
		XSetWindowBorderWidth(G_display, Bar[i], 0);
		XSetWindowBackground(G_display, Bar[i], G_black_color);
		XClearWindow(G_display, Bar[i]);
		XFillRectangle(G_display, Bar[i], G_gcgreen,
		    bar_width - ((G_Array[i].percentage * bar_width) / 100),
			       0 + sp,
			       (G_Array[i].percentage * bar_width) / 100,
			       (unsigned int) (bar_height * BAR_PERCENTAGE));
		pt[0].x = pt[1].x = bar_width - ((G_Array[i].percentage * bar_width) / 100);
		pt[0].y = 0;
		pt[1].y = bar_height;
		pt[2].x = pt[0].x - (short) (bar_width * ARROW_PERCENTAGE);
		pt[2].y = bar_height / 2;
		XFillPolygon(G_display, Bar[i], G_gcgreen, pt, 3, Convex, CoordModeOrigin);
		sprintf(temp, "[%s] <== CTN", G_Array[i].dpnid);
		XTextExtents(fontLG, temp, strlen(temp), &dir, &fa, &fd, &or);
		w = or.width;
		h = or.ascent;
		XDrawString(G_display, Bar[i], G_gclargeT,
			    (bar_width - w) / 2,
			    ((bar_height - h) / 2) + h, temp, strlen(temp));
		break;
	    default:
		break;
	    }
	} else {
	    XSetWindowBackground(G_display, Bar[i], G_black_color);
	    XSetWindowBorderWidth(G_display, Bar[i], 0);
	    XClearWindow(G_display, Bar[i]);
	    sprintf(temp, "No Traffic");
	    XTextExtents(fontLG, temp, strlen(temp), &dir, &fa, &fd, &or);
	    w = or.width;
	    h = or.ascent;
	    XDrawString(G_display, Bar[i], G_gclargeT,
			(bar_width - w) / 2,
			((bar_height - h) / 2) + h, temp, strlen(temp));
	}

    }

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
CTNNETWORK_CreateGC(void)
{
    unsigned long
        valuemask;
    XColor
	color;
    XGCValues
	values;
    char
        tc_large[MAX_STRING_LENGTH];
    Font
	fontlarge;

    if ((G_display_height < 400) || (G_display_width < 400)) {
	strcpy(tc_large, FONT_LG_B);
    } else {
	strcpy(tc_large, FONT_LG_A);
    }

    if ((fontlarge = XLoadFont(G_display, tc_large)) == 0) {
	return (CTNNETWORK_NOFONT);
    }
    fontLG = XQueryFont(G_display, fontlarge);

/*
 * Create the colors we need...
 */
    XParseColor(G_display, G_cmap, "red", &color);
    XAllocColor(G_display, G_cmap, &color);
    G_red_color = values.background = color.pixel;

    XParseColor(G_display, G_cmap, "green", &color);
    XAllocColor(G_display, G_cmap, &color);
    G_green_color = values.background = color.pixel;

    XParseColor(G_display, G_cmap, "white", &color);
    XAllocColor(G_display, G_cmap, &color);
    G_white_color = values.foreground = color.pixel;

    XParseColor(G_display, G_cmap, "black", &color);
    XAllocColor(G_display, G_cmap, &color);
    G_black_color = values.background = color.pixel;

    valuemask = GCForeground | GCBackground | GCFont | GCFillStyle;
    values.font = fontlarge;
    values.fill_style = FillSolid;
    values.foreground = G_white_color;
    values.background = G_black_color;

    if ((G_gclargeT = XCreateGC(G_display, RootWindow(G_display, G_screen),
				valuemask, &values)) == NULL) {
	COND_PushCondition(CTNNETWORK_NOGC, "%s",
		       "CTNNETWORK: Cannot get requested graphics context");
	return (CTNNETWORK_NOGC);
    }
    values.fill_style = FillSolid;
    values.font = fontlarge;
    values.foreground = G_green_color;
    values.background = G_black_color;

    if ((G_gcgreen = XCreateGC(G_display, RootWindow(G_display, G_screen),
			       valuemask, &values)) == NULL) {
	COND_PushCondition(CTNNETWORK_NOGC, "%s",
		       "CTNNETWORK: Cannot get requested graphics context");
	return (CTNNETWORK_NOGC);
    }
    return (CTNNETWORK_NORMAL);
}

/*******************************************************************/
CONDITION
CTNNETWORK_InitializeX(void)
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
    if ((G_display = XOpenDisplay(display_name)) == NULL) {
	COND_PushCondition(CTNNETWORK_NOSERVER, "%s", "CTNNETWORK: Cannot connect to X server");
	return (CTNNETWORK_NOSERVER);
    }
    G_screen = DefaultScreen(G_display);
    height = DisplayHeight(G_display, G_screen);
    width = DisplayWidth(G_display, G_screen);
    if ((G_display_height > height) || (G_display_height < 0))
	G_display_height = height;
    if ((G_display_width > width) || (G_display_width < 0))
	G_display_width = width;
/*    G_visual = DefaultVisual(G_display, G_screen); smm */
    G_cmap = DefaultColormap(G_display, G_screen);

    /* create the graphics context */
    if ((CTNNETWORK_CreateGC()) != CTNNETWORK_NORMAL) {
	COND_PushCondition(CTNNETWORK_NOGRAPHICS, "%s",
			   "CTNNETWORK: Cannot get graphics context");
	return (CTNNETWORK_NOGRAPHICS);
    }
    valuemask = CWBackPixel;
    attributes.background_pixel = G_black_color;
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

    XSetStandardProperties(G_display, G_big_window, "ctn_network", "ctn_network",
			None, (char **) NULL, (int) 0, (XSizeHints *) NULL);

    XSelectInput(G_display, G_big_window,
	       SubstructureNotifyMask | ExposureMask | StructureNotifyMask);
/* DEB
	       ButtonReleaseMask | SubstructureNotifyMask | ExposureMask | StructureNotifyMask);
*/

    XMapWindow(G_display, G_big_window);
    XFlush(G_display);
    return (CTNNETWORK_NORMAL);
}
#endif
