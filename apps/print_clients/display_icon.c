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
/*
**				DICOM 93
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
**			ReverseX
**			MapX
**			LoadGrayMap
**			selectImage
**			cancelMulti_select
**			my_highlight
**			Multi_select
**			CreatePreviewStudyWindow
**			PreviewStudy
**			selectBIB
**			sendBIB
**			moreBFB
**			printBFB
**			cancelPrint
**			CreateStandardBFBWindow
** Author, Date:	Tze Khiong Lee, SEPTEMBER 14th 1993
** Intent:		To provide routines that will deal with
**			loading, displaying and tranfering icons
**			in windows. There are routines for building
**			a Basic Film Box Window,  a window that displays
**			the images in a study and also to select images
**			from.
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:48:50 $
** Source File:		$RCSfile: display_icon.c,v $
** Revision:		$Revision: 1.31 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.31 $ $RCSfile: display_icon.c,v $";
#ifdef ATHENA

#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <stdlib.h>
#endif

#include        <X11/Xlib.h>
#include        <X11/Xutil.h>
#include        <X11/Xos.h>
#include        <X11/Xatom.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Toggle.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/AsciiText.h>
#include "dicom.h"
#include "lst.h"
#include "condition.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "dbquery.h"
#include "dicom_uids.h"
#include "dicom_sq.h"
#include "dicom_print.h"

#include "print_client_app.h"


#include "icon.h"
#include "Buttons.h"
#include "display_icon.h"
#include "print_shared.h"
#include "display_error.h"
#include "display_message.h"

extern char
   *db_file;

extern Widget
    mainwin,
    toplevel;
extern PRN_BASICFILMBOX
    bfbAttrib;

typedef struct {
    Widget toggle;
    Pixmap pixmap;
    XImage *image;
    char UID[DICOM_UI_LENGTH + 1];
}   ICON_GRID_ITEM;

static ICON_GRID_ITEM
    icons[8][8];

typedef struct {
    Widget toggle;
    Pixmap pixmap;
    char UID[DICOM_UI_LENGTH + 1];
    PRN_BASICIMAGEBOX bib;
}   BASICIMAGEBOX;

BASICIMAGEBOX
bib[8][8];
static Widget
    bibform[8];
static Widget
    bibshell = 0;
int
    selected_bib = -1;

static Widget
    shell;

static unsigned long
    graylut[256];

static int
    ncolor;

static Display
*   display;
static GC
    gc;
static Visual
*   visual;
static Screen
*   screen;
static Colormap
    colormap;
static Widget
    text;
static LST_HEAD
*   image_list = NULL;

#define LOOKUP_ENTRIES 128
#define SHIFT_FACTOR 8
#define MIN_USEFUL_AMT  16

static Boolean sendBIB(int row, int col);
void UpdateAllEntries();

/* ReverseX
**
** Purpose:
**	Reverse the color of the pixel
**
** Parameter Dictionary:
**	pixel		Pixel value
**
** Return Values:
**	New value of the pixel in reverse video
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
unsigned long
ReverseX(unsigned long pixel)
{
    int
        i,
        j;

    for (j = 0; j < ncolor && pixel != graylut[j]; j++);
    if (j >= ncolor) {
	printf("Fatal Error in ReverseX\n");
	exit(1);
    }
    return ((unsigned long) graylut[ncolor - j]);
}


/* MapX
**
** Purpose:
**	This function will take in the raw data of an image
**	and maps each pixel to the closest pixel value available
**	from the X colormaps.
**
** Parameter Dictionary:
**	data		Raw image data
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
void
MapX(unsigned char data[])
{
    unsigned char
        max,
        min,
        range;
    int
        i;
    float
        ratio,
        temp;

    max = 0;
    min = 255;
    for (i = 0; i < ICON_SIZE; i++) {
	if (data[i] > max)
	    max = data[i];
	if (data[i] < min)
	    min = data[i];
    }
    range = max - min;
    ratio = ((float) ncolor) / ((float) range);
    for (i = 0; i < ICON_SIZE; i++) {
	temp = ((float) data[i]) - ((float) min);
	data[i] = (unsigned char) graylut[(int) (temp * ratio)];
    }
}


/* LoadGrayMap
**
** Purpose:
**	This function will allocate the colormap required for displaying
**	the images.
**
** Parameter Dictionary:
**	n
**	cmap		Color map required to display the image
**
** Return Values:
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
int
LoadGrayMap(int n, Colormap * cmap)
{
    int i;
    XColor colors;
    unsigned long grayvalue;
    unsigned long graypixels[256];


    colors.flags = DoRed | DoGreen | DoBlue;

    for (i = 0; i < n; i++) {
	grayvalue = i * 255 / (n - 1);
	colors.pixel = i;
	colors.red = colors.green = colors.blue =
	    grayvalue << SHIFT_FACTOR;
	if (!XAllocColor(display, *cmap, &colors)) {
	    if (n <= MIN_USEFUL_AMT) {
		fprintf(stderr, "cannot allocate enough colors\n");
		return (0);
	    }
	    XFreeColors(display, *cmap, graypixels, i, 0);
	    return (LoadGrayMap(i, cmap));
	} else {
	    graypixels[i] = colors.pixel;
	}
    }
    graylut[0] = graypixels[0];
    for (i = 1; i < LOOKUP_ENTRIES; i++)
	graylut[i] = graypixels[i / (LOOKUP_ENTRIES / n)];
    return (n);
}

static int
    bfbROW,			/* number of rows in the basic film box */
    bfbCOL;			/* number of columns in the basic film box */


extern CTNBOOLEAN Send_BIB_when_print;


/* selectImage
**
** Purpose:
**	This function is called when the user selects an image from
**	from a study.  The function will then transfer the image
**	to the currently selected BIB, and make another BIB current.
**
** Parameter Dictionary:
**	w
**	client_data
**	call_data
**
** Return Values:
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

void
selectImage(Widget w, XtPointer client_data, XtPointer call_data)
{
    int
        x,
        y,
        idx;
    XImage
	* image;
    char
        temp[255];
    LIST_ICON_STUDYICON
	* icon_item;

    idx = (int) client_data;
    strcpy(temp, icons[idx / 8][idx % 8].UID);
    XtVaSetValues(text, XtNstring, temp, NULL);
    if (icons[idx / 8][idx % 8].pixmap == (Pixmap) 0)
	return;
    if (selected_bib == -1)
	return;
    icon_item = LST_Position(&image_list, LST_Head(&image_list));
    if (icon_item == NULL) {
	printf("Error: LST_Position failed in selectImage\n");
	return;
    }
    while (strcmp(icons[idx / 8][idx % 8].UID, icon_item->studyicon.UID)) {
	icon_item = LST_Next(&image_list);
	if (icon_item == NULL) {
	    printf("Error: LST_Next failed in selectImage\n");
	    return;
	}
    }
    if (icons[idx / 8][idx % 8].pixmap == (Pixmap) 0)
	icons[idx / 8][idx % 8].pixmap = XCreatePixmap(
						       display,
				   XtWindow(icons[idx / 8][idx % 8].toggle),
					ICON_XSIZE, ICON_YSIZE, ICON_DEPTH);
    image = XCreateImage(display,
			 visual,
			 ICON_DEPTH,
			 ZPixmap,
			 0,
			 (char *) icon_item->studyicon.Icon,
			 ICON_XSIZE, ICON_YSIZE,
			 8, ICON_XSIZE);
    if (bib[selected_bib / 8][selected_bib % 8].pixmap == (Pixmap) 0)
	bib[selected_bib / 8][selected_bib % 8].pixmap = XCreatePixmap(display,
		   XtWindow(bib[selected_bib / 8][selected_bib % 8].toggle),
					ICON_XSIZE, ICON_YSIZE, ICON_DEPTH);
    XPutImage(display, bib[selected_bib / 8][selected_bib % 8].pixmap, gc,
	      image, 0, 0, 0, 0, ICON_XSIZE, ICON_YSIZE);
/*    XDestroyImage(image); */
    XtVaSetValues(bib[selected_bib / 8][selected_bib % 8].toggle, XtNbitmap,
		  bib[selected_bib / 8][selected_bib % 8].pixmap, NULL);
    strcpy(bib[selected_bib / 8][selected_bib % 8].UID,
	   icons[idx / 8][idx % 8].UID);
    x = selected_bib % 8;
    y = selected_bib / 8;
    if (Send_BIB_when_print == FALSE)
	(void) sendBIB(y, x);
    selected_bib++;
    x = selected_bib % 8;
    y = selected_bib / 8;
    if (x >= bfbCOL) {
	x = 0;
	y++;
    }
    if (y >= bfbROW)
	y = 0;
    selected_bib = y * 8 + x;
    XtVaSetValues(bib[selected_bib / 8][selected_bib % 8].toggle, XtNstate,
		  TRUE, NULL);
}


static Boolean
    multi_select_on = FALSE;

/* cancelMulti_select
**
** Purpose:
**	Cancel the multiple select option
**
** Parameter Dictionary:
**	w
**	client_data
**	call_data
**
** Return Values:
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static XtCallbackProc
cancelMulti_select(Widget w, XtPointer client_data, XtPointer call_data)
{
    multi_select_on = FALSE;
    return NULL;
}

static int
    start_selected_image;

/* my_highlight
**
** Purpose:
**	Describe the purpose of the function
**
** Parameter Dictionary:
**	w
**	event
**	params
**	num_params
**
** Return Values:
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
void
my_highlight(Widget w, XEvent * event, char *params[], Cardinal * num_params)
{
    int
        current_selection,
        idx;

    if (multi_select_on == FALSE)
	return;
    for (idx = 0; idx < 64 && icons[idx / 8][idx % 8].toggle != w;
	 idx++);
    if (idx >= 64) {
	(void) displayError("Major Code Error #2: Please Report to MIR");
	return;
    }
    current_selection = idx;
    for (idx = 0; idx < 64; idx++)
	XtVaSetValues(icons[idx / 8][idx % 8].toggle, XtNstate, FALSE, NULL);
    if (current_selection - start_selected_image < 0)
	for (idx = current_selection; idx <= start_selected_image;
	     idx++)
	    XtVaSetValues(icons[idx / 8][idx % 8].toggle, XtNstate, TRUE, NULL);
    else
	for (idx = start_selected_image; idx <= current_selection;
	     idx++)
	    XtVaSetValues(icons[idx / 8][idx % 8].toggle, XtNstate, TRUE, NULL);
}

/* Multi_select
**
** Purpose:
**	Select multiple images at one time.
**
** Parameter Dictionary:
**	w
**	event
**	params
**	num_params
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
void
Multi_select(Widget w, XEvent * event, char *params[], Cardinal * num_params)
{
    static Widget
        cancelshell = 0;
    int
        end_selected_image,
        idx = 0;

    for (idx = 0; idx < 64 && icons[idx / 8][idx % 8].toggle != w;
	 idx++);
    if (idx >= 64) {
	(void) displayError("Major Code Error #2: Please Report to MIR");
	return;
    }
    if (multi_select_on == FALSE) {
	start_selected_image = idx;
	multi_select_on = TRUE;
	cancelshell = displayMessage("Multiple Selection",
				     "Select last image with Buttton 2.",
				     cancelMulti_select, NULL, "Cancel");
	return;
    }
    if (cancelshell == 0) {
	(void) displayError("Major Code Error #2");
	return;
    }
    XtPopdown(cancelshell);
    XtDestroyWidget(cancelshell);
    cancelshell = 0;
    end_selected_image = idx;
    multi_select_on = FALSE;
    if (end_selected_image - start_selected_image < 0)
	for (idx = end_selected_image; idx <= start_selected_image;
	     idx++) {
	    XtVaSetValues(icons[idx / 8][idx % 8].toggle, XtNstate, FALSE, NULL);
	    selectImage(w, (XtPointer) idx, NULL);
	}
    else
	for (idx = start_selected_image; idx <= end_selected_image;
	     idx++) {
	    XtVaSetValues(icons[idx / 8][idx % 8].toggle, XtNstate, FALSE, NULL);
	    selectImage(w, (XtPointer) idx, NULL);
	}
}

extern XtAppContext
    app_context;

/*CreatePreviewStudyWindow
**
** Purpose:
**	This will create a window that will display the images in a study.
**
** Parameter Dictionary:
**	None
**
** Return Values:
**	Handle to a widget
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
Widget
CreatePreviewStudyWindow()
{
    Widget
	label,
	mainform,
	oldform,
	newform,
	oldicon;
    int
        i,
        j;
    unsigned long valuemask;
    XGCValues values;
    XColor color;
    static XtActionsRec Multi_selection[] = {
	{"Multi_select", Multi_select},
	{"My_highlight", my_highlight},
    };
    static char toggle_translation_table[] =
    "<EnterWindow>:	highlight() My_highlight()\n\
	 <Btn2Down>,<Btn2Up>:	Multi_select()";
    static XtTranslations
        translations;

    XtAppAddActions(app_context, Multi_selection, XtNumber(Multi_selection));
    translations = XtParseTranslationTable(toggle_translation_table);
    display = XtDisplay(toplevel);
    screen = XtScreen(toplevel);
    visual = XDefaultVisualOfScreen(screen);
    colormap = XDefaultColormapOfScreen(screen);
    valuemask = GCForeground | GCBackground | GCFillStyle;
    XParseColor(display, colormap, "white", &color);
    XAllocColor(display, colormap, &color);
    values.foreground = color.pixel;
    XParseColor(display, colormap, "black", &color);
    XAllocColor(display, colormap, &color);
    values.background = color.pixel;
    values.fill_style = FillSolid;
    gc = XtGetGC(toplevel, valuemask, &values);

    shell = XtVaCreatePopupShell("Select Image From Study",
				 topLevelShellWidgetClass, toplevel, NULL);

    if ((ncolor = LoadGrayMap(128, &colormap)) < 32) {
	printf("Failed to create color map.\n");
	return (0);
    }
    mainform = XtVaCreateManagedWidget("icon_box", formWidgetClass, shell,
     XtNcolormap, colormap, XtNborderWidth, 0, XtNdefaultDistance, 0, NULL);

    oldform = 0;
    for (i = 0; i < 8; i++) {
	oldicon = 0;
	newform = XtVaCreateManagedWidget("stack_icon", formWidgetClass,
			  mainform, XtNfromVert, oldform, XtNborderWidth, 0,
					  XtNdefaultDistance, 0, NULL);
	for (j = 0; j < 8; j++) {
	    icons[i][j].toggle = XtVaCreateManagedWidget("icon",
							 commandWidgetClass,
			      newform, XtNwidth, ICON_XSIZE + 10, XtNheight,
		  ICON_YSIZE + 10, XtNborderWidth, 0, XtNfromHoriz, oldicon,
						   XtNlabel, "Empty", NULL);
	    XtAddCallback(icons[i][j].toggle, XtNcallback, selectImage,
			  (XtPointer) (i * 8 + j));
	    XtOverrideTranslations(icons[i][j].toggle, translations);
	    oldicon = icons[i][j].toggle;
	    icons[i][j].pixmap = (Pixmap) 0;
	    icons[i][j].image = (XImage *) 0;
	}
	oldform = newform;
    }
    label = XtVaCreateManagedWidget("icon_label", labelWidgetClass, mainform,
	   XtNfromVert, oldform, XtNlabel, "UID:", XtNborderWidth, 0, NULL);
    text = XtVaCreateManagedWidget("icon_text", asciiTextWidgetClass, mainform,
	    XtNfromVert, oldform, XtNfromHoriz, label, XtNwidth, 550, NULL);
    return (shell);
}



/* PreviewStudy
**
** Purpose:
**	This function will put the images of a study into the
**	PreviewStudy Window.
**
** Parameter Dictionary:
**	idx_file	The icon idx pathname.
**	icon_file	The icon file pathname.
**	accessionNumber The Accession Number of the study to preview.
**
** Return Values:
**	TRUE	Success.
**	FALSE	Failure.
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
Boolean
PreviewStudy(char *idx_file, char *icon_file, char *accessionNumber)
{
    CONDITION
	cond;
    ICON_STUDYOFFSET
	studyoffset;
    LIST_ICON_STUDYICON
	* icon_item;
    static LST_HEAD
    *   icon_list = NULL;
    int
        idx;
    unsigned char
        data[ICON_SIZE];
    XImage
	* image;

    XtManageChild(shell);
    if (ICON_GetStudyOffset(idx_file, accessionNumber, &studyoffset) != ICON_NORMAL)
	return (FALSE);
    if (ICON_GetStudyIconLst(icon_file, &studyoffset, &icon_list) != ICON_NORMAL)
	return (FALSE);

    for (idx = 0; idx < 64; idx++) {
	strcpy(icons[idx / 8][idx % 8].UID, "");
	if (icons[idx / 8][idx % 8].pixmap != (Pixmap) 0) {
	    XFillRectangle(display, icons[idx / 8][idx % 8].pixmap, gc,
			   0, 0, ICON_XSIZE, ICON_YSIZE);
	    XtVaSetValues(icons[idx / 8][idx % 8].toggle, XtNlabel, "Empty",
			  XtNbitmap, icons[idx / 8][idx % 8].pixmap, NULL);
	}
	if (icons[idx / 8][idx % 8].image != (XImage *) 0) {
	    image = icons[idx / 8][idx % 8].image;
	    image->data = NULL;
	    (void) XDestroyImage(icons[idx / 8][idx % 8].image);
	    icons[idx / 8][idx % 8].image = (XImage *) 0;
	}
    }
    idx = 0;
    if (image_list == NULL)
	image_list = LST_Create();
    if (image_list == NULL) {
	printf("Error: LST_Create Failed\n");
	return (FALSE);
    }
    while ((icon_item = LST_Dequeue(&image_list)) != NULL)
	free(icon_item);
    while (((icon_item = LST_Dequeue(&icon_list)) != NULL) && (idx < 64)) {
	printf("UID = %s\n", icon_item->studyicon.UID);
	strcpy(icons[idx / 8][idx % 8].UID, icon_item->studyicon.UID);
	MapX(icon_item->studyicon.Icon);
	if (icons[idx / 8][idx % 8].pixmap == (Pixmap) 0)
	    icons[idx / 8][idx % 8].pixmap = XCreatePixmap(
							   display,
				   XtWindow(icons[idx / 8][idx % 8].toggle),
					ICON_XSIZE, ICON_YSIZE, ICON_DEPTH);
	image = XCreateImage(display,
			     visual,
			     ICON_DEPTH,
			     ZPixmap,
			     0,
			     (char *) icon_item->studyicon.Icon,
			     ICON_XSIZE, ICON_YSIZE,
			     8, ICON_XSIZE);
	icons[idx / 8][idx % 8].image = image;
	XPutImage(display, icons[idx / 8][idx % 8].pixmap, gc, image, 0, 0, 0, 0,
		  ICON_XSIZE, ICON_YSIZE);
	XtVaSetValues(icons[idx / 8][idx % 8].toggle, XtNbitmap,
		      icons[idx / 8][idx % 8].pixmap, NULL);
/*	XDestroyImage(image); */
	idx++;
	(void) LST_Enqueue(&image_list, icon_item);
    }
    while ((icon_item = LST_Dequeue(&icon_list)) != NULL)
	free(icon_item);
    return (TRUE);
}

extern PRN_BASICIMAGEBOX
    bibAttrib;
extern char
    imagePositionC[64];


/* selectBIB
**
** Purpose:
**	This function is called when a BIB is selected.
**	It will update the Image Attribute Window to display the correct
**	information for each BIB.
**
** Parameter Dictionary:
**	w
**	client_data
**	call_data
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static void
selectBIB(Widget w, XtPointer client_data, XtPointer call_data)
{
    int
        selected;
    Boolean
	set,
	send = FALSE;

    XtVaGetValues(w, XtNstate, &set, NULL);
    if (set == TRUE) {
	if (Send_BIB_when_print == FALSE) {
	    if (strcmp(bibAttrib.polarity,
		    bib[selected_bib / 8][selected_bib % 8].bib.polarity) ||
		strcmp(bibAttrib.magnificationType,
	     bib[selected_bib / 8][selected_bib % 8].bib.magnificationType))
		send = TRUE;
	}
	selected = (int) client_data;
	bib[selected_bib / 8][selected_bib % 8].bib = bibAttrib;
	bibAttrib = bib[selected / 8][selected % 8].bib;
	if (send == TRUE)
	    (void) sendBIB(selected_bib / 8, selected_bib % 8);
	selected_bib = selected;
    } else {
	selected = (int) client_data;
	if (Send_BIB_when_print == FALSE) {
	    if (strcmp(bibAttrib.polarity,
		       bib[selected / 8][selected % 8].bib.polarity) ||
		strcmp(bibAttrib.magnificationType,
		     bib[selected / 8][selected % 8].bib.magnificationType))
		send = TRUE;
	}
	bib[selected / 8][selected % 8].bib = bibAttrib;
	bibAttrib = bib[selected_bib / 8][selected_bib % 8].bib;
	if (send == TRUE)
	    (void) sendBIB(selected / 8, selected % 8);
    }
    sprintf(imagePositionC, "%i", bibAttrib.imagePosition);
    UpdateAllEntries();
}

extern DUL_ASSOCIATESERVICEPARAMETERS
    params;

extern DUL_ASSOCIATIONKEY
*   association;

extern char
    SOPClass[DICOM_UI_LENGTH + 1];

extern Widget
    imageattshell,
    previewshell,
    studyshell,
    bibshell;


/* sendBIB
**
** Purpose:
**	This function will retrive the dcm_object for a BIB
**	and send the BIB to the print server.
**
** Parameter Dictionary:
**	row		Row number
**	col		Column number
**
** Return Values:
**	TRUE
**	FALSE
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static Boolean
sendBIB(int row, int col)
{
    unsigned short
        retStat;
    bib[row][col].bib.object = NULL;
    if (getImageData(db_file, bib[row][col].UID, &(bib[row][col].bib.object)) !=
	DCM_NORMAL) {
	(void) displayError("Error:Failed to open image file");
	return (FALSE);
    }
    bib[row][col].bib.bibAttributeFlag |= PRN_BIB_K_IMAGEMODULE;
    if (setImageBox(&association, &params, SOPClass,
		    &bib[row][col].bib, &retStat)
	!= SRV_NORMAL) {
	COND_DumpConditions();
	(void) displayError("Error:Failed to set Image Box");
	return (FALSE);
    }
    if (retStat != MSG_K_SUCCESS) {
	displayErrorStatus("BIB", "NSet", retStat);
	return FALSE;
    }
    return (TRUE);
}

extern Widget bfbshell;
extern int
    filmboxes,
    filmboxcount;


/* moreBFB
**
** Purpose:
**	Describe the purpose of the function
**
** Parameter Dictionary:
**	w
**	client_data
**	call_data
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
XtCallbackProc
MoreBFB(Widget w, XtPointer client_data, XtPointer call_data)
{
    XtPopdown(imageattshell);
    XtPopdown(previewshell);
    XtPopdown(studyshell);
    XtPopdown(bibshell);
    filmboxcount++;
    if (filmboxcount <= filmboxes)
	XtPopup(bfbshell, XtGrabNone);
    return NULL;
}


/* printBFB
**
** Purpose:
**	This function is called when the user clicks on the Print
**	button. The function do the neccessary number of n-set for
**	each selected BIB and then send the print command.
**
** Parameter Dictionary:
**	w
**	client_data
**	call_data
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
void
printBFB(Widget w, XtPointer client_data, XtPointer call_data)
{
    int
        i,
        j;
    char
       *filename;
    unsigned short
        retStat;

    if (Send_BIB_when_print == TRUE) {
	for (i = 0; i < bfbROW; i++)
	    for (j = 0; j < bfbCOL; j++) {
		if (strlen(bib[i][j].UID) > 0)
		    if (sendBIB(i, j) == FALSE)
			return;
	    }
    } else if (strcmp(bibAttrib.polarity,
		    bib[selected_bib / 8][selected_bib % 8].bib.polarity) ||
	       strcmp(bibAttrib.magnificationType,
	   bib[selected_bib / 8][selected_bib % 8].bib.magnificationType)) {
	bib[selected_bib / 8][selected_bib % 8].bib = bibAttrib;
	(void) sendBIB(selected_bib / 8, selected_bib % 8);
    }
    if (sendFilmBoxPrint(&association, &params, SOPClass,
	  DICOM_SOPCLASSBASICFILMBOX, &bfbAttrib, &retStat) != SRV_NORMAL) {
	(void) displayError("Error: sendFilmBoxPrint");
	return;
    }
    if (retStat != MSG_K_SUCCESS) {
	displayErrorStatus("BFB", "NAction", retStat);
	return;
    }
    if (sendDeleteRequest(&association, &params, SOPClass,
			  DICOM_SOPCLASSBASICFILMBOX,
		 bfbAttrib.filmBoxSOPInstanceUID, &retStat) != SRV_NORMAL) {
	printf("In printMgmtSCU, sendDeleteRequest failed\n");
	printf("while deleting FILM BOX instance \n");
	return;
    }
    if (retStat != MSG_K_SUCCESS) {
	displayErrorStatus("BFB", "NDelete", retStat);
	return;
    }
    (void) displayMessage("Printed!", "Print Operation Completed", MoreBFB, NULL, NULL);
}


/* cancelPrint
**
** Purpose:
**	This function is called when the user clicks on the cancel
**	on the BFB window.
**
** Parameter Dictionary:
**	w
**	client_data
**	call_data
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static void
cancelPrint(Widget w, XtPointer client_data, XtPointer call_data)
{
    XtPopdown(imageattshell);
    XtPopdown(previewshell);
    XtPopdown(studyshell);
    XtPopdown(bibshell);
}

/* CreateStandardBFBWindow
**
** Purpose:
**	This function creates a window to represent the film box.
**
** Parameter Dictionary:
**	row		Row number
**	col		Column number
**
** Return Values:
**	Handle to a widget
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
Widget
CreateStandardBFBWindow(int row, int col)
{
    static PRN_BASICIMAGEBOX default_imageBox = {
	NULL, NULL,		/* Reserved for LST */
	PRN_K_BASICGREYSCALEIMAGEBOX,	/* Type */
	0,			/* Flag for conditional fields */
	DICOM_SOPCLASSBASICGREYSCALEIMAGEBOX,
	"",			/* SOP Instance UID */
	1,			/* Image Position */
	NORMAL,			/* Polarity */
	REPLICATE,		/* Magnification type */
	"",			/* Smoothing type */
	"",			/* Requested Image Size */
	NULL,			/* Preformatted Image sequence */
	NULL, NULL, NULL	/* Pointers to other sequences */
    };

    Widget
	shellW,
	mainform,
	iconform,
	buttonform,
	form,
	firsticon,
	oldicon,
	oldform,
	newform,
	printbutton,
	label;
    char
        temp[256];
    int
        i,
        j;
    static char toggle_translations_table[] =
    "<EnterWindow>:		highlight(Always)\n\
	<LeaveWindow>:		unhighlight()\n\
	<Btn1Down>,<Btn1Up>:	set() notify()";

    static XtTranslations
        toggle_translations;

    static Boolean
        toggle_translated = FALSE;

    SQ_REFPRINT
	* refPrint;

    if (toggle_translated == FALSE)
	toggle_translations = XtParseTranslationTable(toggle_translations_table);

    bfbROW = row;
    bfbCOL = col;
    if (bibshell != 0) {
	XtDestroyWidget(bibshell);
	for (i = 0; i < 8; i++)
	    for (j = 0; j < 8; j++) {
		if (bib[i][j].pixmap != (Pixmap) 0)
		    XFreePixmap(display, bib[i][j].pixmap);
		bib[i][j].pixmap = (Pixmap) 0;
		strcpy(bib[i][j].UID, "");
	    }
    }
    sprintf(temp, "Basic Film Box - Standard %d x %d", row, col);
    shellW = XtVaCreatePopupShell(temp,
				  topLevelShellWidgetClass, toplevel, NULL);

    mainform = XtVaCreateManagedWidget("BFB_form", formWidgetClass, shellW,
				       XtNborderWidth, 0, NULL);
    iconform = XtVaCreateManagedWidget("BFB_icon_form", formWidgetClass,
				       mainform,
     XtNcolormap, colormap, XtNborderWidth, 1, XtNdefaultDistance, 0, NULL);

    oldform = 0;
    firsticon = 0;
    refPrint = LST_Head(&bfbAttrib.referencedBIBSeq);
    if (refPrint != NULL)
	(void) LST_Position(&bfbAttrib.referencedBIBSeq, refPrint);
    else {
	(void) displayError("empty UID list for BIBs");
	return (0);
    }
    for (i = 0; i < row; i++) {
	oldicon = 0;
	newform = XtVaCreateManagedWidget("stack_icon", formWidgetClass,
			  iconform, XtNfromVert, oldform, XtNborderWidth, 0,
					  XtNdefaultDistance, 0, NULL);
	bibform[i] = newform;
	if (bibform[i] == 0) {
	    printf("Could not create stack icon %d\n", i);
	    exit(1);
	}
	for (j = 0; j < col; j++) {
	    if (refPrint == NULL) {
		(void) displayError("Not Enough BIB UIDs");
		return (0);
	    }
	    bib[i][j].toggle = XtVaCreateManagedWidget("BIB",
						       toggleWidgetClass,
			      newform, XtNwidth, ICON_XSIZE + 10, XtNheight,
		  ICON_YSIZE + 10, XtNborderWidth, 0, XtNfromHoriz, oldicon,
				XtNradioGroup, firsticon, XtNlabel, "Empty",
				XtNtranslations, toggle_translations, NULL);
	    bib[i][j].pixmap = (Pixmap) 0;
	    bib[i][j].bib = default_imageBox;
	    strcpy(bib[i][j].bib.imageBoxSOPClassUID, refPrint->refSOPClassUID);
	    strcpy(bib[i][j].bib.imageBoxSOPInstanceUID,
		   refPrint->refSOPInstanceUID);
	    bib[i][j].bib.imagePosition = (i * col) + j + 1;
	    bib[i][j].bib.bibAttributeFlag = PRN_BIB_K_IMAGEPOSITION |
		PRN_BIB_K_POLARITY | PRN_BIB_K_MAGNIFICATIONTYPE;
	    XtAddCallback(bib[i][j].toggle, XtNcallback, selectBIB,
			  (XtPointer) (i * 8 + j));
	    oldicon = bib[i][j].toggle;
	    if ((i == 0) && (j == 0))
		firsticon = bib[i][j].toggle;
	    refPrint = LST_Next(&bfbAttrib.referencedBIBSeq);
	}
	oldform = newform;
    }
    bibshell = shellW;
    XtVaSetValues(firsticon, XtNstate, TRUE, NULL);
    selected_bib = 0;
    buttonform = XtVaCreateManagedWidget("button_form", formWidgetClass,
		  mainform, XtNfromVert, iconform, XtNborderWidth, 0, NULL);
    printbutton = CreateButtonTop(buttonform, NULL, "Print", printBFB, NULL);
    (void) CreateButtonLeft(buttonform, XtParent(printbutton), "Cancel", cancelPrint, NULL);
    bibAttrib = bib[0][0].bib;
    sprintf(imagePositionC, "%i", bibAttrib.imagePosition);
    UpdateAllEntries();
    return (shellW);
}

#endif
