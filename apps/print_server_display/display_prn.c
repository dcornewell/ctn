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

#ifdef ATHENA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include        <X11/Xlib.h>
#include        <X11/Xutil.h>
/*#include        <X11/Xos.h>*/
#include        <X11/Xatom.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Viewport.h>
#include <X11/Xaw/Label.h>

#include "dicom.h"
#include "lst.h"
#include "condition.h"
#include "dicom_objects.h"
#include "dicom_sq.h"
#include "dicom_print.h"
#include "display_prn.h"

#define new(x) (x*) malloc (sizeof(x))

typedef struct {
    unsigned short
        position,
        rows,
        cols;
        U32
        size;
    void
       *data;
}   FILE_IMAGE;

typedef struct {
    void
       *reserved[2];
    int
        position;
    char
        filename[256];
        Widget
        label;
        Pixmap
        pixmap;
        Boolean
        has_pixmap;
}   DISP_IMAGE;

typedef struct {
    void
       *reserved[2];
    char
        UID[64],
        filename[256];
    int
        rows,
        cols,
        image_width,
        image_height;
        Widget
        form;
        PRN_BASICFILMBOX
        bfb;
        LST_HEAD
    *   img_box_list;
}   DISP_BOX;

typedef struct {
    void
       *reserved[2];
    char
        UID[64],
        filename[256];
        Widget
        shell,
        viewport,
        form;
        LST_HEAD
    *   film_box_list;
}   DISP_SESSION;

static unsigned long
    graylut[256];
static ncolor;
static Widget
    toplevel;
static Colormap
    cmap;
static GC
    gc;
static int
    box_height,
    box_width;

#define U_S unsigned short
#define U_L unsigned long
#define U_C unsigned char

static void
resizePixel8bit(FILE_IMAGE * img, unsigned short new_rows, unsigned short new_cols)
{
    U_S
    i, j, k, l;
    U_S
	avg;
    U_S
	factor;
    unsigned char
       *new_data,
       *old_data,
        old;

    old_data = (unsigned char *) img->data;
    if (new_rows > img->rows && new_cols > img->cols) {
	factor = new_rows / img->rows;
	if ((int) factor > (int) (new_cols / img->cols))
	    factor = new_cols / img->cols;
	if (factor == 1)
	    return;
	new_rows = factor * img->rows;
	new_cols = factor * img->cols;
	new_data = (unsigned char *) malloc(sizeof(unsigned char) *
					    new_rows * new_cols);
	for (i = 0; i < new_rows; i++)
	    for (j = 0; j < new_cols; j++) {
		old = old_data[(i / factor) * img->cols + j / factor];
		new_data[i * new_rows + j] = old;
	    }
    } else {
	factor = img->rows / new_rows + 1;
	if ((int) factor < (int) (img->cols / new_cols + 1))
	    factor = img->cols / new_cols + 1;
	if (factor == 1)
	    return;
	new_rows = img->rows / factor;
	new_cols = img->cols / factor;
	new_data = (unsigned char *) malloc(sizeof(unsigned char) *
					    new_rows * new_cols);
	for (i = 0; i < new_rows; i++)
	    for (j = 0; j < new_cols; j++) {
		int
		    x,
		    y;
		avg = 0;
		for (k = 0; k < factor; k++)
		    for (l = 0; l < factor; l++) {
			x = j * factor + l;
			y = i * factor + k;
			avg += (U_S) old_data[x + y * img->cols];
		    }
		new_data[i * new_cols + j] = (U_C) ((int) avg / (int) (factor * factor));
	    }
    }
    img->data = (void *) new_data;
    img->size = new_rows * new_cols * sizeof(unsigned char);
    img->rows = new_rows;
    img->cols = new_cols;
    free(old_data);
}


static void
cvtData8bit(unsigned char data[], unsigned long size)
{
    unsigned char
        max,
        min,
        range;
    unsigned long
        i;
    float
        ratio,
        temp;

    max = 0;
    min = 255;
    for (i = 0; i < size; i++) {
	if (data[i] > max)
	    max = data[i];
	if (data[i] < min)
	    min = data[i];
    }
    range = max - min;
    ratio = ((float) ncolor) / ((float) range);
    for (i = 0; i < size; i++) {
	temp = ((float) data[i]) - ((float) min);
	data[i] = (unsigned char) graylut[(int) (temp * ratio)];
    }
}

#define LOOKUP_ENTRIES 128
#define SHIFT_FACTOR 8
#define MIN_USEFUL_AMT  16

static int
LoadGrayMap(int n, Colormap * map)
{
    int i;
    XColor colors;
    unsigned long grayvalue;
    unsigned long graypixels[256];

    if (n <= MIN_USEFUL_AMT) {
	fprintf(stderr, "You may need to close some other X-applications\n");
	fprintf(stderr, "cannot allocate enough colors\n");
	return (0);
    }
    colors.flags = DoRed | DoGreen | DoBlue;

    for (i = 0; i < n; i++) {
	grayvalue = i * 255 / (n - 1);
	colors.pixel = i;
	colors.red = colors.green = colors.blue =
	    grayvalue << SHIFT_FACTOR;
	if (!XAllocColor(XtDisplay(toplevel), *map, &colors)) {
	    if (n <= MIN_USEFUL_AMT) {
		fprintf(stderr, "cannot allocate enough colors\n");
		return (0);
	    }
	    XFreeColors(XtDisplay(toplevel), *map, graypixels, i, 0);
	    return (LoadGrayMap(i, map));
	} else {
	    graypixels[i] = colors.pixel;
	}
    }
    graylut[0] = graypixels[0];
    for (i = 1; i < LOOKUP_ENTRIES; i++)
	graylut[i] = graypixels[i / (LOOKUP_ENTRIES / n)];
    return (n);
}

static CONDITION
getElement(DCM_OBJECT ** object, DCM_TAG tag, void **storage, U32 * size)
{
    DCM_ELEMENT
    element;
    U32
	rtnlength;
    void
       *ctx = NULL;
    CONDITION
	cond;

    if (*size == 0) {
	cond = DCM_GetElementSize(object, tag, size);
	if (cond != DCM_NORMAL) {
	    fprintf(stderr, "Error: Failed to get data size\n");
	    return (cond);
	}
	*storage = (void *) malloc(sizeof(char) * (*size));
    }
    element.tag = tag;
    element.d.string = (char *) *storage;
    element.length = *size;
    element.multiplicity = 1;

    cond = DCM_LookupElement(&element);
    if (cond != DCM_NORMAL)
	return (cond);

    cond = DCM_GetElementValue(object, &element, &rtnlength, &ctx);
    return (cond);
}

static DISP_BOX
*
findBOXofBIB(char *UID, LST_HEAD ** session_list)
{
    SQ_REFPRINT
    * item;
    DISP_SESSION
	* session;
    DISP_BOX
	* box;
    DISP_IMAGE
	* img;
    Boolean
	found = False;

    session = LST_Position(session_list, LST_Head(session_list));
    while (session != NULL && found == False) {
	box = LST_Position(&(session->film_box_list), LST_Head(&(session->film_box_list)));
	while (box != NULL && found == False) {
	    item = LST_Position(&(box->bfb.referencedBIBSeq), LST_Head(&(box->bfb.referencedBIBSeq)));
	    while (item != NULL && found == False)
		if (strcmp(item->refSOPInstanceUID, UID) == 0)
		    found = True;
		else
		    item = LST_Next(&(box->bfb.referencedBIBSeq));
	    if (found == False)
		box = LST_Next(&(session->film_box_list));
	}
	if (found == False)
	    session = LST_Next(session_list);
    }
    if (found == False) {
	fprintf(stderr, "Incoming BIB has a UID that does not exist in list\n");
	return (NULL);
    }
    return (box);
}

static FILE_IMAGE
*
getImageFromFile(char *filename)
{
    static FILE_IMAGE
        file_img;
    DCM_OBJECT
	* object;
    FILE_IMAGE
	* ret_img;
    CONDITION
	cond;
    U32
	size;
    PRN_BASICIMAGEBOX
	bib,
	*pbib;
    void
       *pointer;

    pbib = &bib;
    cond = DCM_OpenFile(filename, DCM_ORDERLITTLEENDIAN, &object);
    if (cond != DCM_NORMAL) {
	fprintf(stderr, "Failed to open %s\n", filename);
	return (NULL);
    }
    bib.bibAttributeFlag = PRN_BIB_K_IMAGEPOSITION || PRN_BIB_K_IMAGEMODULE;
    cond = PRN_ParseObject(&object, PRN_K_BASICGREYSCALEIMAGEBOX, (void **) &pbib);
    if (cond != PRN_NORMAL) {
	fprintf(stderr, "Failed to parse PRN object\n");
	return (NULL);
    }
    file_img.position = bib.imagePosition;
    size = 2;
    pointer = &(file_img.rows);
    cond = getElement(&(bib.object), DCM_IMGROWS, &pointer, &size);
    if (cond == DCM_NORMAL) {
	pointer = &(file_img.cols);
	cond = getElement(&(bib.object), DCM_IMGCOLUMNS, &pointer, &size);
	if (cond == DCM_NORMAL) {
	    file_img.size = 0;
	    cond = getElement(&(bib.object), DCM_PXLPIXELDATA, &(file_img.data), &(file_img.size));
	}
    }
    if (cond != DCM_NORMAL) {
	fprintf(stderr, "Error: Failed to get pixel data in %s\n", filename);
	return (NULL);
    }
    ret_img = new(FILE_IMAGE);
    *ret_img = file_img;
    cond = DCM_CloseObject(&object);
#ifdef ASG
/* This is temporarily commented till the bug is removed */
    cond = DCM_CloseObject(&(bib.object));
#endif
    return (ret_img);
}

static DISP_IMAGE
*
findIMGofFILE(FILE_IMAGE * file_img, DISP_BOX * box)
{
    CONDITION
    cond;
    DISP_IMAGE
	* img;

    img = LST_Position(&(box->img_box_list), LST_Head(&(box->img_box_list)));
    while (img != NULL && img->position != (int) file_img->position)
	img = LST_Next(&(box->img_box_list));
    if (img == NULL) {
	fprintf(stderr, "Error: Failed to find position #%d in box %s\n",
		file_img->position, box->UID);
	return (NULL);
    }
    return (img);
}

Boolean
DISP_SetImage(char *UID, char *filename, LST_HEAD ** session_list)
{
    char
       *data;
    unsigned short
        size,
        rows,
        cols;
    DISP_BOX
	* box;
    FILE_IMAGE
	* file_img;
    DISP_IMAGE
	* img;
    XImage
	* ximg;
    Pixmap
	pixmap;


    box = findBOXofBIB(UID, session_list);
    if (box == NULL) {
	fprintf(stderr, "Error: findBOXofBIB failed\n");
	return (False);
    }
    file_img = getImageFromFile(filename);
    if (file_img == NULL) {
	fprintf(stderr, "Error: getImageFromFile failed\n");
	return (False);
    }
    img = findIMGofFILE(file_img, box);
    if (img == NULL) {
	fprintf(stderr, "Error: findIMGofBIB failed\n");
	return (False);
    }
    printf("successfull\n");

    resizePixel8bit(file_img, (unsigned short) box->image_height, (unsigned short) box->image_width);
    cvtData8bit(file_img->data, file_img->size);
    pixmap = XCreatePixmap(XtDisplay(toplevel), XtWindow(img->label),
			   file_img->cols, file_img->rows, 8);
    ximg = XCreateImage(XtDisplay(toplevel),
			XDefaultVisualOfScreen(XtScreen(toplevel)),
			8,
			ZPixmap,
			0,
			(char *) file_img->data,
			file_img->cols,
			file_img->rows,
			8,
			file_img->cols);
    XPutImage(XtDisplay(toplevel), pixmap, gc, ximg, 0, 0, 0, 0, file_img->cols, file_img->rows);
    (void) XDestroyImage(ximg);
    XtVaSetValues(img->label, XtNbitmap, pixmap, NULL);
    if (img->has_pixmap == True)
	XFreePixmap(XtDisplay(toplevel), img->pixmap);
    else
	img->has_pixmap = True;
    img->pixmap = pixmap;
    strcpy(img->filename, filename);
    free(file_img);

    return True;
}

Boolean
DISP_AddBox(char *UID, char *filename, LST_HEAD ** session_list)
{
    DISP_SESSION
	* session;
    CONDITION
	cond;
    int
        i,
        j;
    DCM_OBJECT
	* object;
    DISP_BOX
	* box,
	*previuos_box;
    DISP_IMAGE
	* img;
    Widget
	prev_form = NULL;
    Widget
	old_img,
	row_form,
	old_row_form;
    int
        x_interval,
        y_interval;
    char
        imageFormat[DICOM_ST_LENGTH + 1],
        film_session_UID[DICOM_UI_LENGTH + 1];
    PRN_BASICFILMBOX
	* bfb;


    cond = DCM_OpenFile(filename, DCM_ORDERLITTLEENDIAN, &object);
    if (cond != DCM_NORMAL) {
	fprintf(stderr, "Failed to open %s\n", filename);
	return (False);
    }
    box = new(DISP_BOX);
    bfb = &(box->bfb);
    bfb->bfbAttributeFlag = PRN_BFB_K_IMAGEDISPLAYFORMAT ||
	PRN_BFB_K_REFERENCEDBASICIMAGEBOXSEQ ||
	PRN_BFB_K_REFERENCEDBASICFILMSESSIONSEQ;
    cond = PRN_ParseObject(&object, PRN_K_BASICFILMBOX, (void **) &bfb);
    if (cond != PRN_NORMAL) {
	fprintf(stderr, "PRN_ParseObject failed in PRN_AddBox\n");
	free(box);
	return (False);
    }
/*
	cond = getElement(&object, DCM_BFBIMAGEDISPLAYFORMAT,
		(void*) imageFormat, sizeof(imageFormat));
	if(cond != DCM_NORMAL) {
		fprintf(stderr, "Failed to get Image format from %s\n", filename);
		return(False);
	}
	cond = getElement(&object, DCM_BFBIMAGEDISPLAYFORMAT,
		(void*) imageFormat, sizeof(imageFormat));
	if(cond != DCM_NORMAL) {
		fprintf(stderr, "Failed to get Image format from %s\n", filename);
		return(False);
	}
*/
    if (DCM_CloseObject(&object) != DCM_NORMAL) {
	fprintf(stderr, "DCM_CloseObject Failed in DISP_AddBox\n");
	return (False);
    }
    session = LST_Position(session_list, LST_Head(session_list));
    while (session != NULL &&
	strcmp(session->UID, bfb->referencedFilmSessionSOPInstanceUID) != 0)
	session = LST_Next(session_list);
    if (session == NULL) {
	fprintf(stderr, "Fatal search Error in session list\n");
	return (FALSE);
    }
    strcpy(box->UID, UID);
    printf("image format = %s\n", bfb->imageDisplayFormat);
    if (strncmp("STANDARD", bfb->imageDisplayFormat, strlen("STANDARD")) != 0) {
	fprintf(stderr, "Image Format:%s not supported\n", imageFormat);
	return (False);
    }
    sscanf(bfb->imageDisplayFormat, "STANDARD\\%d,%d", &(box->cols), &(box->rows));
    printf("rows = %d, cols = %d\n", box->rows, box->cols);
    if (LST_Count(&(session->film_box_list)) > 0) {
	previuos_box = LST_Tail(&(session->film_box_list));
	prev_form = previuos_box->form;
    }
    if ((box->img_box_list = LST_Create()) == NULL) {
	fprintf(stderr, "LST_Create Failed in DISP_AddBox\n");
	return (False);
    }
    strcpy(box->filename, filename);
    box->form = XtVaCreateManagedWidget("box_form", formWidgetClass,
					session->form,
					XtNfromVert, prev_form,
					XtNwidth, box_width,
					XtNheight, box_height,
					NULL);
    box->image_width = box_width / box->cols;
    box->image_height = box_height / box->rows;
    x_interval = (box_width % box->cols) / (box->cols + 1);
    y_interval = (box_height % box->rows) / (box->rows + 1);
    for (i = 0; i < box->rows; i++) {
	for (j = 0; j < box->cols; j++) {
	    int
	        x,
	        y;
	    char
	        temp[60];

	    x = j * box->image_width + (j + 1) * x_interval;
	    y = i * box->image_height + (i + 1) * y_interval;
	    img = new(DISP_IMAGE);
	    img->position = i * box->cols + j + 1;
	    img->has_pixmap = False;
	    sprintf(temp, "Image %d is empty", img->position);
	    img->label = XtVaCreateManagedWidget("image_lbl",
						 labelWidgetClass, box->form,
						 XtNlabel, temp,
						 XtNwidth, box->image_width,
					       XtNheight, box->image_height,
						 XtNhorizDistance, x,
						 XtNvertDistance, y,
						 NULL);
	    if (LST_Enqueue(&(box->img_box_list), img) != LST_NORMAL) {
		fprintf(stderr, "1:LST_Enqueue Failed in DISP_AddBox\n");
		return (False);
	    }
	}
    }
    if (LST_Enqueue(&(session->film_box_list), box) != LST_NORMAL) {
	fprintf(stderr, "2:LST_Enqueue Failed in DISP_AddBox\n");
	return (False);
    }
    return (True);
}

Boolean
DISP_CreateSession(char *UID, char *filename, LST_HEAD ** session_list)
{
    DISP_SESSION
	* session;

    if (*session_list == NULL) {
	*session_list = LST_Create();
	if (*session_list == NULL) {
	    fprintf(stderr, "LST_Create failed in  DISP_CreateSession\n");
	    return (False);
	}
    }
    session = new(DISP_SESSION);
    session->shell = XtVaCreatePopupShell(UID, topLevelShellWidgetClass,
					  toplevel,
					  XtNcolormap, cmap,
					  NULL);
    session->viewport = XtVaCreateManagedWidget("session_viewport",
					viewportWidgetClass, session->shell,
						XtNwidth, box_width,
						XtNheight, box_height,
						XtNallowVert, True,
						XtNallowHoriz, True,
						NULL);
    session->form = XtVaCreateManagedWidget("session_form",
					 formWidgetClass, session->viewport,
					    XtNwidth, box_width,
					    XtNheight, box_height,
					    NULL);
    strcpy(session->UID, UID);
    strcpy(session->filename, filename);
    if ((session->film_box_list = LST_Create()) == NULL) {
	fprintf(stderr, "LST_Create Failed in DISP_CreateSession\n");
	return (False);
    }
    XtPopup(session->shell, XtGrabNone);
    if (LST_Enqueue(session_list, session) != LST_NORMAL) {
	fprintf(stderr, "LST_Enqueue failed in DISP_CreateSession\n");
	return (False);
    }
    return (True);
}

void
DISP_Initialize(Widget top, int width, int height)
{
    unsigned long valuemask;
    XGCValues values;
    XColor color;

    toplevel = top;
    box_height = height;
    box_width = width;
    cmap = XDefaultColormapOfScreen(XtScreen(toplevel));
    valuemask = GCForeground | GCBackground | GCFillStyle;
    XParseColor(XtDisplay(toplevel), cmap, "white", &color);
    XAllocColor(XtDisplay(toplevel), cmap, &color);
    values.foreground = color.pixel;
    XParseColor(XtDisplay(toplevel), cmap, "black", &color);
    XAllocColor(XtDisplay(toplevel), cmap, &color);
    values.background = color.pixel;
    values.fill_style = FillSolid;
    gc = XtGetGC(toplevel, valuemask, &values);
    ncolor = LoadGrayMap(128, &cmap);
    if (ncolor == 0) {
	fprintf(stderr, "Failed to Allocate Enough colors\n");
	exit(0);
    }
}


static Boolean
DISP_DestroyDISP_SESSION(DISP_SESSION * session)
{
    DISP_BOX
    * box;
    DISP_IMAGE
	* img;
    CONDITION
	cond;
    SQ_REFPRINT
	* item;

    printf("Destroying session: %s\n", session->UID);
    XtPopdown(session->shell);
    XtDestroyWidget(session->shell);
    for (box = LST_Dequeue(&(session->film_box_list));
	 box != NULL;
	 box = LST_Dequeue(&(session->film_box_list))
	) {
	for (img = LST_Dequeue(&(box->img_box_list));
	     img != NULL;
	     img = LST_Dequeue(&(box->img_box_list))
	    ) {
	    if (img->has_pixmap == True)
		XFreePixmap(XtDisplay(toplevel), img->pixmap);
	    unlink(img->filename);
	    free(img);
	}
	cond = LST_Destroy(&(box->img_box_list));
	if (cond != LST_NORMAL) {
	    fprintf(stderr, "Error: LST_Detroy failed in DISP_DestroySession\n");
	    return (False);
	}
	if (box->bfb.referencedBIBSeq != NULL) {
	    for (item = LST_Dequeue(&(box->bfb.referencedBIBSeq));
		 item != NULL;
		 item = LST_Dequeue(&(box->bfb.referencedBIBSeq))
		)
		free(item);
	    cond = LST_Destroy(&(box->bfb.referencedBIBSeq));
	    if (cond != LST_NORMAL) {
		fprintf(stderr, "Error: LST_Detroy failed in DISP_DestroySession\n");
		return (False);
	    }
	}
	if (box->bfb.referencedBABSeq != NULL) {
	    for (item = LST_Dequeue(&(box->bfb.referencedBABSeq));
		 item != NULL;
		 item = LST_Dequeue(&(box->bfb.referencedBABSeq))
		)
		free(item);
	    cond = LST_Destroy(&(box->bfb.referencedBABSeq));
	    if (cond != LST_NORMAL) {
		fprintf(stderr, "Error: LST_Detroy failed in DISP_DestroySession\n");
		return (False);
	    }
	}
	unlink(box->filename);
	free(box);
    }
    cond = LST_Destroy(&(session->film_box_list));
    if (cond != LST_NORMAL) {
	fprintf(stderr, "Error: LST_Detroy failed in DISP_DestroySession\n");
	return (False);
    }
    unlink(session->filename);
    free(session);
    return (True);
}

Boolean
DISP_DestroySession(char *UID, LST_HEAD ** session_list)
{
    DISP_SESSION
	* session;

    session = LST_Position(session_list, LST_Head(session_list));
    while (session != NULL && strcmp(UID, session->UID) != 0)
	session = LST_Next(session_list);
    if (session == NULL) {
	fprintf(stderr, "Error: Failed to find %s session to destroy\n", UID);
	return (False);
    }
    session = LST_Position(session_list, session);
    if (session == NULL) {
	fprintf(stderr, "Error: Failed to position session list\n");
	return (False);
    }
    session = LST_Remove(session_list, LST_K_BEFORE);
    if (session == NULL) {
	fprintf(stderr, "Error: LST_Remove failed in DISP_DestroySession\n");
	return (False);
    }
    return (DISP_DestroyDISP_SESSION(session));
}

void
DISP_DeleteAllSession(LST_HEAD ** session_list)
{
    DISP_SESSION
    * session;

    session = LST_Dequeue(session_list);
    printf("Ready to delete all sessions\n");
    while (session != NULL) {
	if (DISP_DestroyDISP_SESSION(session) == False)
	    fprintf(stderr, "Error: Failed to destroy session\n");
	session = LST_Dequeue(session_list);
    }
}
#endif
