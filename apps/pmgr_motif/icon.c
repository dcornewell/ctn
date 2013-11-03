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
** Module Name(s):	LoadGrayMap
**			initializeColor
**			selectImage
**			highlight_sel
**			cancelMulti_select
**			Multi_select
**			createPreviewStudyIcons
**			MapX
**			PreviewStudy
**			createStandardBFBWindow
**			printBFB
**			MoreBFB
**			selectBIB
**			sendBIB
**			cancelPrint
**			freeResources
**
** Author, Date:	Chander L. Sabharwal, 7-Nov-94
**
** Intent:		This programs opens Preview Study Icons
**			window, Standard BFB Window, study list
**			scrolled window. It provides routines
**			that deal with loading, displaying and
**			tranfering icons in windows.It lets the user
**			select a patient, it displays images and lets
**			the user select images of choice and displays
**			those images in the standard BFB window to
**			send those images for printing.
**
** Last Update:		$Author: smm $, $Date: 2002-10-31 23:36:06 $
** Source File:		$Source: /home/smm/ctn/ctn/apps/pmgr_motif/icon.c,v $
** Revision:		$Revision: 1.29 $
** Status:		$State: Exp $
*/

#include "ctn_os.h"
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>

#include "UxXt.h"

#include <Xm/ScrolledW.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xm/ToggleB.h>
#include <Xm/RowColumn.h>

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
#include "icon.h"
#include "print_shared.h"

static Widget wRowColumn = 0;
Widget wBasicFilmBox_Standard;
Widget wPreviewStudyForm;
Widget wFilmBoxForm;
Widget wFilmSessionForm;
Widget wFilmInformationForm;
Widget wStudyListForm;
Widget wMultipleSelection;
Widget wImageAttribute;

static LST_HEAD *icon_list = NULL;
static unsigned long graylut[256];
static int ncolor;
static Display *display;
static GC gc,
    mygc;
static Visual *visual;
static Screen *screen;
static Colormap colormap;
static Widget texxt;
static LST_HEAD *image_list = NULL;
static Widget printbutton = 0;
static Widget cancelbutton = 0;
extern int filmboxes,
    filmboxcount;

#define LOOKUP_ENTRIES 128
#define SHIFT_FACTOR 8
#define MIN_USEFUL_AMT  16

static void printBFB();
void printSuccessMes();
static void selectBIB();
void displayMessage();
static void highlight_sel();
void UpdateEntries_in_ImageAttWindow();
static void cancelPrint();
void freeResources();
void makeCreateButtonSen();
void makeFilmScreateSen();

typedef struct {
    Widget button;
    Pixmap pixmap;
    XImage *image;
    char UID[DICOM_UI_LENGTH + 1];
}   ICON_GRID_ITEM;

static ICON_GRID_ITEM
    icons[100][8];

typedef struct {
    Widget button;
    Pixmap pixmap;
    XImage *image;
    char UID[DICOM_UI_LENGTH + 1];
    PRN_BASICIMAGEBOX bib;
}   BASICIMAGEBOX;

BASICIMAGEBOX
bib[8][8];

int
    selected_bib = -1;

extern PRN_BASICIMAGEBOX
    bibAttrib;

extern char
    imagePositionC[64];

extern CTNBOOLEAN
    Send_BIB_when_print;

extern PRN_BASICFILMBOX
    filmBox;

static int
    bfbROW,			/* number of rows in the basic film box */
    bfbCOL;			/* number of columns in the basic film box */

static Boolean
    multi_select_on = FALSE;
static int
    start_selected_image;


extern XtAppContext UxAppContext;

extern char
   *db_file;

extern DUL_NETWORKKEY *networkKey;
extern DUL_ASSOCIATIONKEY *associationKey;
extern DUL_ASSOCIATESERVICEPARAMETERS associationParameters;

char
   *SOPClass = DICOM_SOPCLASSGREYSCALEPRINTMGMTMETA;

static CTNBOOLEAN sendBIB(int row, int col);


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

static int
LoadGrayMap(int n, Colormap * cmap)
{
    int i;
    XColor colors;
    unsigned long grayvalue;
    unsigned long graypixels[256];

    colors.flags = DoRed | DoGreen | DoBlue;

    memset(graypixels, 0, sizeof(graypixels));

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

/* initializeColor
**
** Purpose:
**      This function will allocate the colormap required for displaying
**      the images.
**
** Parameter Dictionary:
**      None
**
** Return Values:
**
** Notes:
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/

static void
initializeColor()
{
    static CTNBOOLEAN initialized = FALSE;
    unsigned long valuemask;
    XColor color;
    XGCValues values;

    if (initialized)
	return;

    initialized = TRUE;

    display = XtDisplay(UxTopLevel);
    screen = XtScreen(UxTopLevel);
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
    gc = XtGetGC(UxTopLevel, valuemask, &values);

    valuemask = GCForeground | GCBackground | GCFillStyle;
    XParseColor(display, colormap, "black", &color);
    XAllocColor(display, colormap, &color);
    values.foreground = color.pixel;
    XParseColor(display, colormap, "white", &color);
    XAllocColor(display, colormap, &color);
    values.background = color.pixel;
    values.fill_style = FillSolid;
    mygc = XtGetGC(UxTopLevel, valuemask, &values);


    if ((ncolor = LoadGrayMap(128, &colormap)) < 32) {
	setMessage("Failed to create color map.");
	return;
    }
}

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

static void
selectImage(Widget w, XtPointer client_data, XtPointer call_data)
{
    int idx,
        x,
        y;

    XImage
	* image;
    char
        temp[255];

    LIST_ICON_STUDYICON
	* icon_item;

    idx = (int) client_data;
    strcpy(temp, icons[idx / 8][idx % 8].UID);
/*    XtVaSetValues(text, XtNstring, temp, NULL);*/
    if (icons[idx / 8][idx % 8].pixmap == (Pixmap) 0)
	return;
    if (selected_bib == -1)
	return;


    icon_item = LST_Position(&image_list, LST_Head(&image_list));
    if (icon_item == NULL) {

	setMessage("Error: LST_Position failed in selectImage\n");
	return;
    }
    while (strcmp(icons[idx / 8][idx % 8].UID, icon_item->studyicon.UID)) {
	icon_item = LST_Next(&image_list);
	if (icon_item == NULL) {
	    setMessage("Error: LST_Next failed in selectImage\n");
	    return;
	}
    }

    if (icons[idx / 8][idx % 8].pixmap == (Pixmap) 0)
	icons[idx / 8][idx % 8].pixmap = XCreatePixmap(
						       display,
				   XtWindow(icons[idx / 8][idx % 8].button),
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
		   XtWindow(bib[selected_bib / 8][selected_bib % 8].button),
					ICON_XSIZE, ICON_YSIZE, ICON_DEPTH);

    XPutImage(display, bib[selected_bib / 8][selected_bib % 8].pixmap, gc,
	      image, 0, 0, 0, 0, ICON_XSIZE, ICON_YSIZE);

    if (bib[selected_bib / 8][selected_bib % 8].image != NULL) {
	bib[selected_bib / 8][selected_bib % 8].image->data = 0;
	XDestroyImage(bib[selected_bib / 8][selected_bib % 8].image);
	bib[selected_bib / 8][selected_bib % 8].image = NULL;
    }
    bib[selected_bib / 8][selected_bib % 8].image = image;

/*    XDestroyImage(image); */

    XtVaSetValues(bib[selected_bib / 8][selected_bib % 8].button,
		  XmNlabelType, XmPIXMAP,
	     XmNlabelPixmap, bib[selected_bib / 8][selected_bib % 8].pixmap,
		  NULL);

    strcpy(bib[selected_bib / 8][selected_bib % 8].UID,
	   icons[idx / 8][idx % 8].UID);

    x = selected_bib % 8;
    y = selected_bib / 8;
    if (Send_BIB_when_print == FALSE)
	sendBIB(y, x);
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

    if (bib[selected_bib / 8][selected_bib % 8].image == (XImage *) 0) {
	XFillRectangle(display,
		       bib[selected_bib / 8][selected_bib % 8].pixmap, mygc,
		       0, 0, ICON_XSIZE, ICON_YSIZE);
    }
    XmToggleButtonSetState(bib[selected_bib / 8][selected_bib % 8].button,
			   TRUE, TRUE);
#ifdef CHANDER
    bibAttrib = bib[selected_bib / 8][selected_bib % 8].bib;
    sprintf(imagePositionC, "%i", bibAttrib.imagePosition);
    UpdateEntries_in_ImageAttWindow();
#endif
}
/* my_highlight
**
** Purpose:
**      This function highlights the selected images from
**	Preview window.
**
** Parameter Dictionary:
**      w
**      start_selection
**      end_selection
**
** Return Values:
**
** Notes:
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/

static void
highlight_sel(Widget w, int start_selection, int end_selection)
{
    int
        idx;

    if (multi_select_on == FALSE) {
	for (idx = 0; idx < 64; idx++)
	    XtVaSetValues(icons[idx / 8][idx % 8].button,
			  XmNdefaultButtonShadowThickness, 0,
			  XmNset, FALSE, NULL);

	return;
    }
    for (idx = 0; idx < 64 && icons[idx / 8][idx % 8].button != w;
	 idx++);
    if (idx >= 64) {

	setMessage("Major Code Error #2: Please Report to MIR");
	return;
    }
    for (idx = 0; idx < 64; idx++)
	XtVaSetValues(icons[idx / 8][idx % 8].button,
		      XmNdefaultButtonShadowThickness, 0,
		      XmNset, FALSE, NULL);

    if (end_selection - start_selection < 0)
	for (idx = end_selection; idx <= start_selection;
	     idx++) {
	    XtVaSetValues(icons[idx / 8][idx % 8].button,
			  XmNdefaultButtonShadowThickness, 1,
			  XmNset, TRUE,
			  NULL);
	}
    else
	for (idx = start_selection; idx <= end_selection;
	     idx++) {
	    XtVaSetValues(icons[idx / 8][idx % 8].button,
			  XmNdefaultButtonShadowThickness, 1,
			  XmNset, TRUE,
			  NULL);
	}

    for (idx = 0; idx < 64; idx++)
	XtVaSetValues(icons[idx / 8][idx % 8].button,
		      XmNdefaultButtonShadowThickness, 0,
		      XmNset, FALSE, NULL);

}

/* cancelMulti_select
**
** Purpose:
**	Cancel the multiple select option
**
** Parameter Dictionary:
**	None
** Return Values:
**	None
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

void
cancelMulti_select()
{
    int idx;

    if (multi_select_on)
	multi_select_on = FALSE;
    for (idx = 0; idx < 64; idx++)
	XtVaSetValues(icons[idx / 8][idx % 8].button,
		      XmNdefaultButtonShadowThickness, 0,
		      XmNset, FALSE, NULL);

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

static void
Multi_select(Widget w, XEvent * event, char *params[],
	     Cardinal * num_params)
{

    int
        end_selected_image,
        idx = 0;

    for (idx = 0; idx < 64 && icons[idx / 8][idx % 8].button != w;
	 idx++);
    if (idx >= 64) {

	setMessage("Major Code Error #2: Please Report to MIR");
	return;
    }
    if (multi_select_on == FALSE) {
	start_selected_image = idx;
	XtVaSetValues(icons[idx / 8][idx % 8].button,
		      XmNdefaultButtonShadowThickness, 1,
		      XmNset, TRUE,
		      NULL);

	multi_select_on = TRUE;
	displayMessage();
	return;
    }
    XtUnmanageChild(wMultipleSelection);
    end_selected_image = idx;

    highlight_sel(w, start_selected_image, end_selected_image);

    multi_select_on = FALSE;


    if (end_selected_image - start_selected_image < 0)
	for (idx = end_selected_image; idx <= start_selected_image;
	     idx++) {
	    XtVaSetValues(icons[idx / 8][idx % 8].button,
			  XmNset, FALSE, NULL);
	    selectImage(w, (XtPointer) idx, NULL);
	}
    else
	for (idx = start_selected_image; idx <= end_selected_image;
	     idx++) {
	    XtVaSetValues(icons[idx / 8][idx % 8].button,
			  XmNset, FALSE, NULL);
	    selectImage(w, (XtPointer) idx, NULL);
	}
}

/*CreatePreviewStudyWindow
**
** Purpose:
**	This will create a window with icons that
**	will display the images in a study.
**
** Parameter Dictionary:
**	cols		number of columns of icons
**	rows		number of rows of icons
**	Widget		parent widget to contain icons
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
createPreviewStudyIcons(int cols, int rows, Widget parent)
{

    int i,
        j;
    static XtActionsRec Multi_selection[] = {
	{"Multi_select", Multi_select},
    };


    static char button_translation_table[] =
    "<Btn2Down>:    Multi_select()\n\
    <Btn2Up>:      Disarm()";


    static XtTranslations translations;

    static CTNBOOLEAN initialized = FALSE;


    if (!initialized) {
	initialized = TRUE;

	initializeColor();
	XtAppAddActions(UxAppContext, Multi_selection, XtNumber(Multi_selection));
	translations = XtParseTranslationTable(button_translation_table);

	cols = (cols <= 8) ? cols : 8;
	rows = (rows <= 100) ? rows : 100;

	for (i = 0; i < rows; i++) {
	    for (j = 0; j < cols; j++) {
		icons[i][j].button = (Widget) XtVaCreateManagedWidget("icon",
					    xmPushButtonWidgetClass, parent,
						  XmNwidth, ICON_XSIZE + 10,
						 XmNheight, ICON_YSIZE + 10,
						XmNx, j * (ICON_XSIZE + 10),
						XmNy, i * (ICON_YSIZE + 10),
							  XmNborderWidth, 0,
				       RES_CONVERT(XmNlabelString, "Empty"),
								      NULL);


		XtAddCallback(icons[i][j].button, XmNactivateCallback, selectImage,
			      (XtPointer) (i * 8 + j));
		XtOverrideTranslations(icons[i][j].button, translations);
#ifdef SMM
		if (icons[i][j].pixmap != (Pixmap) 0)
		    icons[i][j].pixmap = (Pixmap) 0;
		if (icons[i][j].image != (XImage *) 0)
		    icons[i][j].image = (XImage *) 0;
#endif
		icons[i][j].pixmap = (Pixmap) 0;
		icons[i][j].image = (XImage *) 0;
	    }
	}
    }
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

static void
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
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CTNBOOLEAN
PreviewStudy(char *idx_file, char *icon_file, char *accessionNumber)
{
    CONDITION cond;
    ICON_STUDYOFFSET studyoffset;
    LIST_ICON_STUDYICON *icon_item;
    int idx;
    unsigned char data[ICON_SIZE];
    XImage *image;


    if ((cond = ICON_GetStudyOffset(idx_file, accessionNumber, &studyoffset))
	!= ICON_NORMAL)
	return (FALSE);
    if ((cond = ICON_GetStudyIconLst(icon_file, &studyoffset, &icon_list))
	!= ICON_NORMAL)
	return (FALSE);


    for (idx = 0; idx < 64; idx++) {
	strcpy(icons[idx / 8][idx % 8].UID, "");

	if (icons[idx / 8][idx % 8].pixmap != (Pixmap) 0)
	    XFreePixmap(display, icons[idx / 8][idx % 8].pixmap);
	icons[idx / 8][idx % 8].pixmap = (Pixmap) 0;

	if (icons[idx / 8][idx % 8].image != (XImage *) 0) {
	    image = icons[idx / 8][idx % 8].image;
	    image->data = NULL;
	    XDestroyImage(icons[idx / 8][idx % 8].image);
	    icons[idx / 8][idx % 8].image = (XImage *) 0;
	}
	if (icons[idx / 8][idx % 8].pixmap == (Pixmap) 0) {
	    icons[idx / 8][idx % 8].pixmap = XCreatePixmap(
							   display,
				   XtWindow(icons[idx / 8][idx % 8].button),
					ICON_XSIZE, ICON_YSIZE, ICON_DEPTH);
	}
	if (icons[idx / 8][idx % 8].pixmap != (Pixmap) 0) {
	    XFillRectangle(display, icons[idx / 8][idx % 8].pixmap, gc,
			   0, 0, ICON_XSIZE, ICON_YSIZE);

	    XtVaSetValues(icons[idx / 8][idx % 8].button,
			  XmNlabelPixmap, icons[idx / 8][idx % 8].pixmap,
			  XmNlabelType, XmPIXMAP,
			  NULL);
	}
    }

    idx = 0;
    if (image_list == NULL)
	image_list = LST_Create();
    if (image_list == NULL) {
	setMessage("Error: LST_Create Failed.");
	return (FALSE);
    }
    while ((icon_item = LST_Dequeue(&image_list)) != NULL)
	free(icon_item);

    while (((icon_item = LST_Dequeue(&icon_list)) != NULL) && (idx < 64)) {
#ifdef CHANDER
	printf(" UID = %s\n", icon_item->studyicon.UID);
#endif
	strcpy(icons[idx / 8][idx % 8].UID, icon_item->studyicon.UID);


	MapX(icon_item->studyicon.Icon);

	image = XCreateImage(display,
			     visual,
			     ICON_DEPTH,
			     ZPixmap,
			     0,
			     (char *) icon_item->studyicon.Icon,
			     ICON_XSIZE, ICON_YSIZE,
			     8, ICON_XSIZE);

	icons[idx / 8][idx % 8].image = image;

	XPutImage(display, icons[idx / 8][idx % 8].pixmap, gc, image, 0, 0, 0,
		  0, ICON_XSIZE, ICON_YSIZE);


	XtVaSetValues(icons[idx / 8][idx % 8].button,
		      XmNlabelType, XmPIXMAP,
		      XmNlabelPixmap, icons[idx / 8][idx % 8].pixmap,
		      NULL);

	idx++;
	LST_Enqueue(&image_list, icon_item);

    }
    while ((icon_item = LST_Dequeue(&icon_list)) != NULL)
	free(icon_item);

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

void
createStandardBFBWindow(int row, int col, Widget parent)
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

    int i,
        j;
    int x = 0,
        y = 0;
    XImage *image;


    static char button_translations_table[] =
    "<EnterWindow>:     Enter()\n\
	<LeaveWindow>:		Leave()\n\
    <Btn1Down>,<Btn1Up>:	Arm() Disarm()";

    static XtTranslations button_translations;

    static CTNBOOLEAN button_translated = FALSE;

    SQ_REFPRINT *refPrint;


    initializeColor();

    if (button_translated == FALSE)
	button_translations = XtParseTranslationTable(button_translations_table);
    bfbROW = row;
    bfbCOL = col;

    if (wRowColumn == 0) {	/* First time through */
	for (i = 0; i < row; i++) {
	    for (j = 0; j < col; j++) {
		bib[i][j].button = (Widget) 0;
		bib[i][j].pixmap = (Pixmap) 0;
		bib[i][j].image = (XImage *) 0;
		strcpy(bib[i][j].UID, "");
	    }
	}
    } else {


	for (i = 0; i < 8; i++) {
	    for (j = 0; j < 8; j++) {
		if (bib[i][j].pixmap != (Pixmap) 0) {
		    XFreePixmap(display, bib[i][j].pixmap);
		    bib[i][j].pixmap = (Pixmap) 0;
		}
		if (bib[i][j].image != (XImage *) 0) {
		    image = bib[i][j].image;
		    image->data = NULL;
		    bib[i][j].image = (XImage *) 0;
		}
		if (bib[i][j].button != (Widget) 0) {
		    XtDestroyWidget(bib[i][j].button);
		    bib[i][j].button = (Widget) 0;
		}
		strcpy(bib[i][j].UID, "");
	    }
	}
	XtDestroyWidget(wRowColumn);
	XtDestroyWidget(printbutton);
	XtDestroyWidget(cancelbutton);
    }

    refPrint = LST_Head(&filmBox.referencedBIBSeq);
    if (refPrint != NULL)
	(void) LST_Position(&filmBox.referencedBIBSeq, refPrint);
    else {
	setMessage("empty UID list for BIBs");
	return;
    }

    XClearWindow(display, XtWindow(parent));

    wRowColumn = XtVaCreateManagedWidget("rc", xmRowColumnWidgetClass, parent,
					 XmNnumColumns, row,
					 XmNpacking, XmPACK_NONE,
					 XmNradioBehavior, TRUE,
					 XmNorientation, XmHORIZONTAL,
					 XmNborderWidth, 1,
					 NULL);

    for (i = 0; i < row; i++) {
	for (j = 0; j < col; j++) {
	    bib[i][j].button = (Widget) XtVaCreateManagedWidget("bib",
						  xmToggleButtonWidgetClass,
								wRowColumn,
						  XmNwidth, ICON_XSIZE + 10,
						 XmNheight, ICON_YSIZE + 10,
						XmNx, j * (ICON_XSIZE + 10),
						XmNy, i * (ICON_YSIZE + 10),
							  XmNborderWidth, 0,
						      XmNindicatorOn, FALSE,
				       RES_CONVERT(XmNlabelString, "Empty"),
								NULL);
	    bib[i][j].pixmap = (Pixmap) 0;

	    if (bib[i][j].pixmap == (Pixmap) 0) {
		bib[i][j].pixmap = XCreatePixmap(
						 display,
						 XtWindow(bib[i][j].button),
					ICON_XSIZE, ICON_YSIZE, ICON_DEPTH);
	    }
	    if (bib[i][j].pixmap != (Pixmap) 0) {
		XFillRectangle(display, bib[i][j].pixmap, gc,
			       x, y, ICON_XSIZE, ICON_YSIZE);
	    }
	    XtVaSetValues(bib[i][j].button,
			  XmNlabelType, XmPIXMAP,
			  XmNlabelPixmap, bib[i][j].pixmap,
			  NULL);



	    bib[i][j].bib = default_imageBox;
	    strcpy(bib[i][j].bib.imageBoxSOPClassUID, refPrint->refSOPClassUID);
	    strcpy(bib[i][j].bib.imageBoxSOPInstanceUID,
		   refPrint->refSOPInstanceUID);


	    bib[i][j].bib.imagePosition = (i * col) + j + 1;

	    bib[i][j].bib.bibAttributeFlag = PRN_BIB_K_IMAGEPOSITION |
		PRN_BIB_K_POLARITY | PRN_BIB_K_MAGNIFICATIONTYPE;

	    XtAddCallback(bib[i][j].button, XmNarmCallback, selectBIB,
			  (XtPointer) (i * 8 + j));

	    refPrint = LST_Next(&filmBox.referencedBIBSeq);
	}
    }

    XFillRectangle(display, bib[0][0].pixmap, mygc,
		   0, 0, ICON_XSIZE, ICON_YSIZE);

    XmToggleButtonSetState(bib[0][0].button, TRUE, TRUE);

    selected_bib = 0;

    printbutton = (Widget) XtVaCreateManagedWidget("pbutton",
					    xmPushButtonWidgetClass, parent,
						   XmNwidth, ICON_XSIZE,
						   XmNheight, ICON_YSIZE / 2,
						   XmNx, 5,
					  XmNy, row * (ICON_XSIZE + 10) + 5,
	     XmNborderWidth, 0, RES_CONVERT(XmNlabelString, "Print"), NULL);

    XtAddCallback(printbutton, XmNactivateCallback, printBFB, NULL);

    cancelbutton = (Widget) XtVaCreateManagedWidget("cbutton",
					    xmPushButtonWidgetClass, parent,
						    XmNwidth, ICON_XSIZE,
						  XmNheight, ICON_YSIZE / 2,
						    XmNx, ICON_XSIZE + 7,
					  XmNy, row * (ICON_XSIZE + 10) + 5,
	    XmNborderWidth, 0, RES_CONVERT(XmNlabelString, "Cancel"), NULL);

    XtAddCallback(cancelbutton, XmNactivateCallback, cancelPrint, NULL);


    bibAttrib = bib[0][0].bib;

    sprintf(imagePositionC, "%i", bibAttrib.imagePosition);
#ifdef CHANDER
    UpdateEntries_in_ImageAttWindow();
#endif

}
/* printBFB
**
** Purpose:
**      This function is called when the user clicks on the Print
**      button. The function do the neccessary number of n-set for
**      each selected BIB and then send the print command.
**
** Parameter Dictionary:
**      w
**      client_data
**      call_data
**
** Return Values:
**      None
**
** Notes:
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/

static void
printBFB(Widget w, XtPointer client_data, XtPointer call_data)
{
    int
        i,
        j;
    char
       *filename;
    unsigned short
        retStat;

    char errorinfo[80];

    if (Send_BIB_when_print == TRUE) {
	for (i = 0; i < bfbROW; i++)
	    for (j = 0; j < bfbCOL; j++) {
		if (strlen(bib[i][j].UID) > 0) {
		    if (sendBIB(i, j) == FALSE)
			return;
		}
	    }
    } else if (strcmp(bibAttrib.polarity,
		    bib[selected_bib / 8][selected_bib % 8].bib.polarity) ||
	       strcmp(bibAttrib.magnificationType,
	   bib[selected_bib / 8][selected_bib % 8].bib.magnificationType)) {
	bib[selected_bib / 8][selected_bib % 8].bib = bibAttrib;
	sendBIB(selected_bib / 8, selected_bib % 8);
    }
    if (sendFilmBoxPrint(&associationKey, &associationParameters, SOPClass,
	    DICOM_SOPCLASSBASICFILMBOX, &filmBox, &retStat) != SRV_NORMAL) {
	setMessage("Error: sendFilmBoxPrint");
	return;
    }
    if (retStat != MSG_K_SUCCESS) {
	sprintf(errorinfo, "BFB, NAction, retStat = %d", retStat);
	setMessage(errorinfo);
	return;
    }
    if (sendDeleteRequest(&associationKey, &associationParameters, SOPClass,
			  DICOM_SOPCLASSBASICFILMBOX,
		   filmBox.filmBoxSOPInstanceUID, &retStat) != SRV_NORMAL) {
	setMessage("In printMgmtSCU, sendDeleteRequest failed while deleting FILM BOX instance \n");
	return;
    }
    if (retStat != MSG_K_SUCCESS) {
	sprintf(errorinfo, "BFB, NDelete, retStat = %d", retStat);
	setMessage(errorinfo);
	return;
    }
    printSuccessMes();
    makeCreateButtonSen();
}

/* moreBFB
**
** Purpose:
**      Describe the purpose of the function
**
** Parameter Dictionary:
**      w
**      client_data
**      call_data
**
** Return Values:
**      None
**
** Notes:
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/

MoreBFB()
{

    XtUnmanageChild(wBasicFilmBox_Standard);
    XtUnmanageChild(wPreviewStudyForm);
    XtUnmanageChild(wFilmBoxForm);
    XtUnmanageChild(wStudyListForm);
#ifdef CHANDER
    XtUnmanageChild(wImageAttribute);
#endif
    filmboxcount++;
    if (filmboxcount <= filmboxes) {
#ifdef RUNTIME
	XtManageChild(wFilmBoxForm);
#endif
    }
    if (filmboxcount > filmboxes) {
	makeFilmScreateSen();
    }
}
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
    int i,
        j;
    Boolean
	set,
	send = FALSE;


    initializeColor();
    selected = (int) client_data;
    selected_bib = selected;


    bibAttrib = bib[selected / 8][selected % 8].bib;

    XFillRectangle(display,
		   bib[selected_bib / 8][selected_bib % 8].pixmap, mygc,
		   0, 0, ICON_XSIZE, ICON_YSIZE);


    XmToggleButtonSetState(bib[selected_bib / 8][selected_bib % 8].button,
			   TRUE, TRUE);

    for (i = 0; i < bfbROW; i++) {
	for (j = 0; j < bfbCOL; j++) {
	    if (i != selected_bib / 8 || j != selected_bib % 8) {

		if (XmToggleButtonGetState(bib[i][j].button))
		    XmToggleButtonSetState(bib[i][j].button, FALSE, FALSE);

		if (bib[i][j].pixmap != (Pixmap) 0) {
		    if (bib[i][j].image == (XImage *) 0) {
			bib[i][j].pixmap = (Pixmap) 0;

			bib[i][j].pixmap = XCreatePixmap(
							 display,
						 XtWindow(bib[i][j].button),
					ICON_XSIZE, ICON_YSIZE, ICON_DEPTH);

			if (bib[i][j].pixmap != (Pixmap) 0) {
			    XFillRectangle(display, bib[i][j].pixmap, gc,
					   0, 0, ICON_XSIZE, ICON_YSIZE);
			}
			XtVaSetValues(bib[i][j].button,
				      XmNlabelType, XmPIXMAP,
				      XmNlabelPixmap, bib[i][j].pixmap,
				      NULL);

		    }
		}
	    }
	}
    }

    if (Send_BIB_when_print == FALSE) {
	if (strcmp(bibAttrib.polarity,
		   bib[selected_bib / 8][selected_bib % 8].bib.polarity) ||
	    strcmp(bibAttrib.magnificationType,
	     bib[selected_bib / 8][selected_bib % 8].bib.magnificationType))
	    send = TRUE;
    }
    if (send == TRUE)
	sendBIB(selected_bib / 8, selected_bib % 8);

    sprintf(imagePositionC, "%i", bibAttrib.imagePosition);
#ifdef CHANDER
    UpdateEntries_in_ImageAttWindow();
#endif
}

/* sendBIB
**
** Purpose:
**      This function will retrive the dcm_object for a BIB
**      and send the BIB to the print server.
**
** Parameter Dictionary:
**      row             Row number
**      col             Column number
**
** Return Values:
**      TRUE
**      FALSE
**
** Notes:
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/
static CTNBOOLEAN
sendBIB(int row, int col)
{
    unsigned short
        retStat;

    char errorinfo[80];

    bib[row][col].bib.object = NULL;
    if (getImageData(db_file, bib[row][col].UID, &(bib[row][col].bib.object)) !=
	DCM_NORMAL) {
	setMessage("Error:Failed to open image file");
	return (FALSE);
    }
    bib[row][col].bib.bibAttributeFlag |= PRN_BIB_K_IMAGEMODULE;

    if (setImageBox(&associationKey, &associationParameters, SOPClass,
		    &bib[row][col].bib, &retStat)
	!= SRV_NORMAL) {
	COND_DumpConditions();
	setMessage("Error:Failed to set Image Box");
	return (FALSE);
    }
    if (retStat != MSG_K_SUCCESS) {
	sprintf(errorinfo, "BIB, NSet, retStat = %d", retStat);
	setMessage(errorinfo);
	return FALSE;
    }
    return (TRUE);
}

/*cancelPrint
**
** Purpose:
**      This function is called when the user clicks on the cancel
**      on the BFB window.
**
** Parameter Dictionary:
**      w
**      client_data
**      call_data
**
** Return Values:
**      None
**
** Notes:
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/
static void
cancelPrint(Widget w, XtPointer client_data, XtPointer call_data)
{
    XtUnmanageChild(wBasicFilmBox_Standard);
    XtUnmanageChild(wPreviewStudyForm);
    XtUnmanageChild(wStudyListForm);
#ifdef CHANDER
    XtUnmanageChild(wImageAttribute);
#endif
}
/*freeResources
**
** Purpose:
**      This function frees the resources used for this program.
**
** Parameter Dictionary:
**     None
**
** Return Values:
**      None
**
** Notes:
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/

void
freeResources()
{
    int i,
        j;

    CONDITION cond;
    LIST_ICON_STUDYICON *icon_item;

    freePatientResources();

    if (icon_list != NULL) {
	while ((icon_item = LST_Dequeue(&icon_list)) != NULL)
	    free(icon_item);

	LST_Destroy(&icon_list);
    }
    if (image_list != NULL) {
	while ((icon_item = LST_Dequeue(&image_list)) != NULL)
	    free(icon_item);

	LST_Destroy(&image_list);
    }
    for (i = 0; i < 8; i++) {
	for (j = 0; j < 8; j++) {
	    if (bib[i][j].pixmap != (Pixmap) 0)
		XFreePixmap(display, bib[i][j].pixmap);
	    if (icons[i][j].pixmap != (Pixmap) 0)
		XFreePixmap(display, icons[i][j].pixmap);
	}
    }
    if (colormap)
	XFreeColormap(display, colormap);
    if (gc)
	XFreeGC(display, gc);
    if (mygc)
	XFreeGC(display, mygc);
    if (display)
	XtCloseDisplay(display);

    cond = DUL_ReleaseAssociation(&associationKey);
    if (cond != DUL_RELEASECONFIRMED) {
	cond = DUL_AbortAssociation(&associationKey);
    }
    (void) DUL_DropAssociation(&associationKey);


    DUL_DropNetwork(&networkKey);
}
