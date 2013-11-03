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
** Module Name(s):	main
**			usageerror
**			pixel_clip
**			pixel_rescale_slope
**			pixel_window_center
** Author, Date:	Stephen M. Moore, 30-Jul-93
** Intent:		This program reads a DICOM image and converts the
**			pixel data to 8-bit pixels.  Pixel values are mapped
**			using existing window/center and slope/intercept
**			values found in the data.  It is not guaranteed that
**			this mapping to 8-bits will be optimal.
** Last Update:		$Author: smm $, $Date: 2006-11-11 05:07:25 $
** Source File:		$RCSfile: dcm_map_to_8.c,v $
** Revision:		$Revision: 1.19 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.19 $ $RCSfile: dcm_map_to_8.c,v $";
/*
**   Usage:
**	dcm_map_to_8 [-b] [-v] W C input output
**		-b	big endian data
**		-v	verbose mode
**		W	window value (override value in image)
**		C	center value (override value in image)
**
**  Author:
**	Stephen M. Moore (smm@wuerl.wustl.edu)
**	Electronic Radiology Lab
**	Mallinckrodt Institute of Radiology
**	Washington University School of Medicine
**	7-Sep-1992
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dicom.h"
#include "ctnthread.h"
#include "condition.h"
#include "lst.h"
#include "dicom_objects.h"

static void
pixel_clip(DCM_OBJECT ** object, void *pixels, int count,
   int bitsAllocated, int bitsStored, int highBit, int pixelRepresentation);
static void
pixel_rescale_slope(DCM_OBJECT ** object, void *pixels, int count,
    int bitsAllocated, int bitsStored, int highBit, int pixelRepresentation,
		    float *interceptValue);
static void
pixel_window_center(DCM_OBJECT ** object, void *pixels,
		    unsigned char *mappedPixels, int count,
		    int bitsAllocated, int bitsStored, int highBit,
		    int pixelRepresentation,
		    int window, int center);
static void
map_color_to_grey(DCM_OBJECT ** object, void *pixels,
		  unsigned char *mappedPixels, int count,
		  char *photometricInterpretation,
		  int bitsAllocated, int bitsStored, int highBit,
		  int pixelRepresentation);
static void
map_palette_color(DCM_OBJECT ** object, void *pixels,
		  unsigned char *mappedPixels, int count,
		  char *photometricInterpretation,
		  int bitsAllocated, int bitsStored, int highBit,
		  int pixelRepresentation);
static void usageerror();

main(int argc, char **argv)
{
    DCM_OBJECT
	* object;
    CONDITION
	cond;
    CTNBOOLEAN
	verbose = FALSE;
    unsigned long
        options = DCM_ORDERLITTLEENDIAN,
        i;
    U32
	pixelLength,
	elementLength,
	frameLength;
    int
        W = 0x10000,
        C = 0x10000;
    void
       *pixels;
    unsigned char
       *newPixels;
    static unsigned short
        bitsAllocated,
        bitsStored,
        highBit,
        pixelRepresentation,
        samplesPerPixel,
        rows,
        columns;
    float
        intercept;
    int pixelCount;
    static char
        photometricInterpretation[DICOM_CS_LENGTH + 1];
    void
       *ctx;

    DCM_ELEMENT
	p2 = {
	DCM_PXLPIXELDATA, DCM_OT, "", 1, 0, NULL
    };

    static DCM_ELEMENT list[] = {
	{DCM_IMGBITSALLOCATED, DCM_US, "",
	1, sizeof(bitsAllocated), (void *) &bitsAllocated},
	{DCM_IMGBITSSTORED, DCM_US, "",
	1, sizeof(bitsStored), (void *) &bitsStored},
	{DCM_IMGHIGHBIT, DCM_US, "",
	1, sizeof(highBit), (void *) &highBit},
	{DCM_IMGPIXELREPRESENTATION, DCM_US, "",
	1, sizeof(pixelRepresentation), (void *) &pixelRepresentation},
	{DCM_IMGSAMPLESPERPIXEL, DCM_US, "",
	1, sizeof(samplesPerPixel), (void *) &samplesPerPixel},
	{DCM_IMGROWS, DCM_US, "",
	1, sizeof(rows), (void *) &rows},
	{DCM_IMGCOLUMNS, DCM_US, "",
	1, sizeof(columns), (void *) &columns},
	{DCM_IMGPHOTOMETRICINTERP, DCM_CS, "",
	1, sizeof(photometricInterpretation), photometricInterpretation}
    };
    static DCM_TAG deleteTags[] = {
	DCM_IMGRESCALEINTERCEPT,
	DCM_IMGRESCALESLOPE,
	DCM_IMGWINDOWCENTER,
	DCM_IMGWINDOWWIDTH,
	DCM_IMGPLANARCONFIGURATION
    };

    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'b':
	    options &= ~DCM_ORDERMASK;
	    options |= DCM_ORDERBIGENDIAN;
	    break;
	case 't':
	    options &= ~DCM_FILEFORMATMASK;
	    options |= DCM_PART10FILE;
	    break;
	case 'v':
	    verbose = TRUE;
	    break;
	case 'Z':
	    options |= DCM_ACCEPTVRMISMATCH;
            break;

	    break;
	case 'C':
	    argc--;
	    argv++;
	    if (sscanf(*argv, "%d", &C) != 1)
		usageerror();
	    break;
	case 'W':
	    argc--;
	    argv++;
	    if (sscanf(*argv, "%d", &W) != 1)
		usageerror();
	    break;
	default:
	    break;
	}
    }

    THR_Init();
    DCM_Debug(verbose);
    if (argc < 2)
	usageerror();

    printf("DCM File: %s\n", *argv);
    cond = DCM_OpenFile(*argv, options, &object);
    if (cond != DCM_NORMAL) {
	if ((options & DCM_PART10FILE) == 0) {
	    options |= DCM_PART10FILE;
	    cond = DCM_OpenFile(*argv, options, &object);
        }
	if (cond != DCM_NORMAL) {
	    COND_DumpConditions();
	    THR_Shutdown();
	    exit(1);
	}
    }
    argv++;

    cond = DCM_ParseObject(&object, list, (int) DIM_OF(list), NULL, 0, NULL);
    if (cond != DCM_NORMAL) {
	fprintf(stderr, "Error retrieving bits allocated data element\n");
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    cond = DCM_GetElementSize(&object, p2.tag, &pixelLength);
    if (cond != DCM_NORMAL) {
	fprintf(stderr, "Error finding length of pixel data\n");
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
/*
    frameLength = (U32) samplesPerPixel *(U32) rows *(U32) columns *(U32) (bitsAllocated / 8);
    if (pixelLength != frameLength) {
	fprintf(stderr, "Computed pixel length differs from actual: %s\n", *argv);
	THR_Shutdown();
	exit(2);
    }
*/
    pixels = malloc(pixelLength);
    if (pixels == NULL) {
	perror("Malloc of pixel data");
	THR_Shutdown();
	exit(1);
    }
    /*pixelCount = (int) rows *(int) columns;*/
    pixelCount = pixelLength/2;
    newPixels = malloc(pixelCount + 1);
    if (newPixels == NULL) {
	perror("Malloc of pixel data");
	THR_Shutdown();
	exit(1);
    }
    p2.length = pixelLength;
    p2.d.ot = pixels;
    ctx = NULL;
    if (DCM_GetElementValue(&object, &p2, &elementLength, &ctx) != DCM_NORMAL) {
	fprintf(stderr, "Error retrieving pixels from image\n");
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    if (strcmp(photometricInterpretation, "MONOCHROME2") == 0 ||
	strcmp(photometricInterpretation, "MONOCHROME1") == 0) {

	pixel_clip(&object, pixels, pixelCount, bitsAllocated, bitsStored,
		   highBit, pixelRepresentation);
	pixel_rescale_slope(&object, pixels, pixelCount, bitsAllocated,
			    bitsStored, highBit, pixelRepresentation,
			    &intercept);
	if (intercept < 0.)
	    pixelRepresentation = DCM_PIXELTWOSCOMPLEMENT;

	pixel_window_center(&object, pixels, newPixels, pixelCount,
	     bitsAllocated, bitsStored, highBit, pixelRepresentation, W, C);
    } else {
	map_color_to_grey(&object, pixels, newPixels, pixelCount,
		       photometricInterpretation, bitsAllocated, bitsStored,
			  highBit, pixelRepresentation);
    }

    if (DCM_RemoveElement(&object, p2.tag) != DCM_NORMAL) {
	fprintf(stderr, "Error removing old pixel data from object\n");
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    p2.length = pixelCount;
    if (p2.length & 1)
	p2.length++;

    p2.d.ot = newPixels;
    if (DCM_AddElement(&object, &p2) != DCM_NORMAL) {
	fprintf(stderr, "Error adding new pixel data to object\n");
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    bitsAllocated = 8;
    bitsStored = 8;
    highBit = 7;
    pixelRepresentation = 0;
    samplesPerPixel = 1;
    strcpy(photometricInterpretation, "MONOCHROME2");

    if (DCM_ModifyElements(&object, list, (int) DIM_OF(list), NULL, 0, NULL) !=
	DCM_NORMAL) {
	fprintf(stderr, "Error adding new pixel data to object\n");
	COND_DumpConditions();
    }
    for (i = 0; i < DIM_OF(deleteTags); i++) {
	(void) DCM_RemoveElement(&object, deleteTags[i]);
    }

    if (DCM_WriteFile(&object, DCM_ORDERLITTLEENDIAN, *argv) != DCM_NORMAL) {
	fprintf(stderr, "Error writing new DCM image file\n");
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    THR_Shutdown();
    return 0;
}

/* usageError
**
** Purpose:
**	Displays the correct way to invoke the command along with various
**	options
**
** Parameter Dictionary:
**	None
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
usageerror()
{
    fprintf(stderr,
	    "Usage: dcm_map_to_8 [-b] [-t] [-v] [-W width] [-C center] input output\n");
    exit(1);
}

/* pixel_clip
**
** Purpose:
**
** Parameter Dictionary:
**	object		Handle to the DICOM object
**	pixels		stream of pixels
**	count		Number of pixels
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
pixel_clip(DCM_OBJECT ** object, void *pixels, int count,
    int bitsAllocated, int bitsStored, int highBit, int pixelRepresentation)
{
    unsigned short mask;
    unsigned short signBit;
    unsigned short *p16;
    unsigned char *p8;

    if (bitsAllocated == 16) {
	p16 = (unsigned short *) pixels;
	if (pixelRepresentation == DCM_PIXELUNSIGNED) {
	    mask = 0xffff << (highBit + 1);
	    while (count-- > 0) {
		*(p16++) &= ~mask;
	    }
	} else {
	    signBit = 1 << highBit;
	    mask = 0xffff << (highBit + 1);
	    while (count-- > 0) {
		if ((*p16 & signBit) != 0)
		    *(p16++) |= mask;
		else
		    *(p16++) &= ~mask;
	    }
	}
    } else {
	p8 = (unsigned char *) pixels;
	if (pixelRepresentation == 0) {
	    mask = 0xff << (highBit + 1);
	    while (count-- > 0) {
		*(p8++) &= ~mask;
	    }

	} else {
	    fprintf(stderr, "Expected pixel representation of 0 for 8 bit data\n");
	    exit(1);
	}
    }
}

/* pixel_rescale_slope
**
** Purpose:
**
** Parameter Dictionary:
**	object		Handle to the DICOM object
**	pixels		stream of pixels
**	count		Number of pixels
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
pixel_rescale_slope(DCM_OBJECT ** object, void *pixels, int count,
    int bitsAllocated, int bitsStored, int highBit, int pixelRepresentation,
		    float *interceptValue)
{
    static char
        slopeString[256],
        interceptString[256];
    int i;
    void *ctx;
    U32 elementLength;
    float
        slope,
        intercept;
    long value;
    short *p16;
    unsigned char *p8;

    static DCM_ELEMENT list[] = {
	{DCM_IMGRESCALEINTERCEPT, DCM_IS, "",
	1, sizeof(interceptString), (void *) interceptString},
	{DCM_IMGRESCALESLOPE, DCM_IS, "",
	1, sizeof(slopeString), (void *) slopeString}
    };

    *interceptValue = (float) 0.;

    for (i = 0; i < DIM_OF(list); i++) {
	ctx = NULL;
	if (DCM_GetElementValue(object, &list[i], &elementLength, &ctx) !=
	    DCM_NORMAL) {
	    (void) COND_PopCondition(TRUE);
	    return;
	}
    }
    if (sscanf(slopeString, "%e", &slope) != 1) {
	fprintf(stderr, "Error converting slope: %s\n", slopeString);
	return;
    }
    if (sscanf(interceptString, "%e", &intercept) != 1) {
	fprintf(stderr, "Error converting intercept: %s\n", interceptString);
	return;
    }
    *interceptValue = intercept;

    if (bitsAllocated == 16) {
	p16 = (short *) pixels;
	while (count-- > 0) {
	    value = (long) (((float) *p16) * slope + intercept);
	    *p16++ = (short) value;
	}
    } else {
	p8 = (unsigned char *) pixels;
	while (count-- > 0) {
	    value = (long) (((float) *p8) * slope + intercept);
	    *p8++ = (unsigned char) value;
	}
    }
}

/* pixel_window_center
**
** Purpose:
**	Describe the purpose of the function
**
** Parameter Dictionary:
**	object		Handle to DICOM object
**	pixels		Stream of pixel data
**	mappedPixels
**	count		Pixel count
**	window
**	center
**
** Return Values:
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static void
pixel_window_center(DCM_OBJECT ** object, void *pixels,
		    unsigned char *mappedPixels, int count,
		    int bitsAllocated, int bitsStored, int highBit,
		    int pixelRepresentation,
		    int window, int center)
{
    static char
        windowString[256] = "",
        centerString[256] = "";
    int
        w = 0,
        c = 0;
    int i;
    float slope;
    int shift;
    long value;
    short *p16 = NULL;
    unsigned short *p16u = NULL;
    unsigned char *p8 = NULL;

    static DCM_ELEMENT list[] = {
	{DCM_IMGWINDOWWIDTH, DCM_IS, "",
	1, sizeof(windowString), (void *) windowString},
	{DCM_IMGWINDOWCENTER, DCM_IS, "",
	1, sizeof(centerString), (void *) centerString}
    };

    (void) DCM_ParseObject(object, list, (int) DIM_OF(list), NULL, 0, NULL);
    (void) COND_PopCondition(TRUE);

    if (sscanf(windowString, "%d", &i) == 1)
	w = i;
    else
	w = 1 << highBit;

    if (sscanf(centerString, "%d", &i) == 1)
	c = i;
    else
	c = 1 << (highBit - 1);

    if (window != 0x10000)
	w = window;
    if (center != 0x10000)
	c = center;

    shift = c - w / 2;
    slope = (float) 256. / (float) w;

    if (bitsAllocated == 16) {
	if (pixelRepresentation == 0) {
	    p16u = (unsigned short *) pixels;
	    while (count-- > 0) {
		value = *p16u++;
		value = (long) (((value) - shift) * slope);
		if (value < 0)
		    *mappedPixels++ = 0;
		else if (value > 255)
		    *mappedPixels++ = 255;
		else
		    *mappedPixels++ = (unsigned char) value;
	    }
	} else {
	    p16 = (short *) pixels;
	    while (count-- > 0) {
		value = (long) (((*p16++) - shift) * slope);
		if (value < 0)
		    *mappedPixels++ = 0;
		else if (value > 255)
		    *mappedPixels++ = 255;
		else
		    *mappedPixels++ = (unsigned char) value;
	    }
	}
    } else {
	p8 = (unsigned char *) pixels;
	while (count-- > 0) {
	    value = (long) (((short) (*p8++) - shift) * slope);
	    if (value < 0)
		*mappedPixels++ = 0;
	    else if (value > 255)
		*mappedPixels++ = 255;
	    else
		*mappedPixels++ = (unsigned char) value;
	}
    }
}

#define RED_COMP        0.299
#define GREEN_COMP      0.587
#define BLUE_COMP       0.114

static void
map_color_to_grey(DCM_OBJECT ** object, void *pixels,
		  unsigned char *mappedPixels, int count,
		  char *photometricInterpretation,
		  int bitsAllocated, int bitsStored, int highBit,
		  int pixelRepresentation)
{
    static unsigned short planarConfig;
    DCM_ELEMENT l[] = {
	{DCM_IMGPLANARCONFIGURATION, DCM_US, "",
	1, sizeof(planarConfig), (void *) &planarConfig}
    };
    CONDITION cond;
    int i,
        intensity;

    if (strcmp(photometricInterpretation, "RGB") == 0) {
	cond = DCM_ParseObject(object, l, (int) DIM_OF(l), NULL, 0, NULL);
	if (cond != DCM_NORMAL) {
	    COND_DumpConditions();
	    exit(1);
	}
	if (bitsAllocated == 8) {
	    unsigned char *p8;
	    p8 = (unsigned char *) pixels;
	    if (planarConfig == 0) {	/* R1G1B1  R2G2B2 ... */
		for (i = 0; i < count; i++) {
		    intensity =
			(int) ((RED_COMP * (float) p8[0]) +
			       (GREEN_COMP * (float) p8[1]) +
			       (BLUE_COMP * (float) p8[2]));
		    if (intensity > 255)
			intensity = 255;
		    *mappedPixels++ = (unsigned char) intensity;
		    p8 += 3;
		}
	    } else {		/* R1R2R3.. G1G2G3... B1B2B3... */
		for (i = 0; i < count; i++) {
		    intensity =
			(int) ((RED_COMP * (float) p8[0]) +
			       (GREEN_COMP * (float) p8[count]) +
			       (BLUE_COMP * (float) p8[count]));
		    if (intensity > 255)
			intensity = 255;
		    *mappedPixels++ = (unsigned char) intensity;
		    p8++;
		}
	    }
	} else if (strcmp(photometricInterpretation, "PALETTE COLOR") == 0) {
	    map_palette_color(object, pixels, mappedPixels, count,
			      photometricInterpretation,
			      bitsAllocated, bitsStored, highBit,
			      pixelRepresentation);
	} else {
	    fprintf(stderr, "Can not handle 16 bits allocated for RGB image\n");
	    exit(1);
	}
    } else if (strcmp(photometricInterpretation, "PALETTE COLOR") == 0) {
	map_palette_color(object, pixels, mappedPixels, count,
		       photometricInterpretation, bitsAllocated, bitsStored,
			  highBit, pixelRepresentation);
    } else {
	fprintf(stderr, "Could not deal with %s\n", photometricInterpretation);
	exit(1);
    }
}
static void
map_palette_color(DCM_OBJECT ** object, void *pixels,
		  unsigned char *mappedPixels, int count,
		  char *photometricInterpretation,
		  int bitsAllocated, int bitsStored, int highBit,
		  int pixelRepresentation)
{
    static char
        WindowWidth[128] = "",	/* strings for TEXT (DCM_IS) elements */
        WindowCenter[128] = "",
        Slope[128] = "",
        Intercept[128] = "";

    static unsigned short
        sampsperpixel,
        nrows,
        ncols,
        bitsallocated,
        bitsstored,
        highbit,
        pixrep,
        planarconfig,
        s;			/* Utility var used for shorts	 */

    static short
        red_entries,		/* For PALETTE COLOR LUTS	 */
        red_first_map,
        blue_entries,
        blue_first_map,
        green_entries,
        green_first_map;

    static unsigned short	/* These changed to unsigned shorts 	 */
       *red_table,		/* to address a problem with 8 bit data */
       *blue_table,
       *green_table;

    U32
	rtnLength;		/* Needed for Get Element calls	 */
    void
       *ctx;

    short
       *stemp;

/*
 * Initialize the Required Elements to prepare for retrieval...
 */
    static DCM_ELEMENT required[] = {
	{DCM_IMGSAMPLESPERPIXEL, DCM_US, "", 1,
	sizeof(sampsperpixel), (void *) &(sampsperpixel)},
	{DCM_IMGROWS, DCM_US, "", 1,
	sizeof(nrows), (void *) &(nrows)},
	{DCM_IMGCOLUMNS, DCM_US, "", 1,
	sizeof(ncols), (void *) &(ncols)},
	{DCM_IMGBITSALLOCATED, DCM_US, "", 1,
	sizeof(bitsallocated), (void *) &(bitsallocated)},
	{DCM_IMGBITSSTORED, DCM_US, "", 1,
	sizeof(bitsstored), (void *) &(bitsstored)},
	{DCM_IMGHIGHBIT, DCM_US, "", 1,
	sizeof(highbit), (void *) &(highbit)},
	{DCM_IMGPIXELREPRESENTATION, DCM_US, "", 1,
	sizeof(pixrep), (void *) &(pixrep)}
    };

    /*
     * These elements are optional...(except for pixel data) (Certain
     * elements are required depending on the photointerpretation) (i.e. if
     * photointerp is PALETTECOLOR, then the palette color lookup tables must
     * be present.)
     */
    DCM_ELEMENT
	redlutdesc = {
	DCM_IMGLUTDESCRIPTRED, DCM_OT, "",
	    1, 0, NULL
    },
	bluelutdesc = {
	DCM_IMGLUTDESCRIPTBLUE, DCM_OT, "",
	    1, 0, NULL
    },
	greenlutdesc = {
	DCM_IMGLUTDESCRIPTGREEN, DCM_OT, "",
	    1, 0, NULL
    },
	redlut = {
	DCM_IMGLOOKUPDATARED, DCM_OT, "",
	    1, 0, NULL
    },
	bluelut = {
	DCM_IMGLOOKUPDATABLUE, DCM_OT, "",
	    1, 0, NULL
    },
	greenlut = {
	DCM_IMGLOOKUPDATAGREEN, DCM_OT, "",
	    1, 0, NULL
    },
	slope = {
	DCM_IMGRESCALESLOPE, DCM_IS, "",
	    1, sizeof(Slope), (void *) Slope
    },
	intercept = {
	DCM_IMGRESCALEINTERCEPT, DCM_IS, "",
	    1, sizeof(Intercept), (void *) Intercept
    },
	window_center = {
	DCM_IMGWINDOWCENTER, DCM_IS, "",
	    1, sizeof(WindowCenter), (void *) WindowCenter
    },
	window_width = {
	DCM_IMGWINDOWWIDTH, DCM_IS, "",
	    1, sizeof(WindowWidth), (void *) WindowWidth
    },
	pc = {
	DCM_IMGPLANARCONFIGURATION, DCM_US, "",
	    1, sizeof(planarconfig), (void *) &(planarconfig)
    };

    /*
     * Get the required elements...
     */
    if (DCM_ParseObject(object, required, (int) DIM_OF(required), NULL, 0, NULL) != DCM_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
/*
 * The planar configuration tells us if we need to intrepret multiple samples per pixel as
 * R1G1B1 R2G2B2.... or R1R2R3...G1G2G3...B1B2B3...
 */
    planarconfig = 0xfff;
    ctx = NULL;
    if (sampsperpixel > 1) {
	if (DCM_GetElementValue(object, &pc, &rtnLength, &ctx) != DCM_NORMAL) {
	    COND_DumpConditions();
	    exit(1);
	}
    } {
	/*
	 * Read in the color palette tables now b/c we need them for either
	 * of the preceding methods
	 */
	/*
	 * Get the Look up table descriptors
	 */
	if (DCM_GetElementSize(object, redlutdesc.tag, &rtnLength) != DCM_NORMAL) {
	    COND_DumpConditions();
	    exit(1);
	}
	/*
	 * Check the of the LUT descriptor to make sure we have the correct
	 * amount...
	 */
	if (rtnLength != 3 * sizeof(short)) {
	    COND_DumpConditions();
	    exit(1);
	}
	/*
	 * Allocate the memory to hold the LUT descriptor data and get it
	 * from the DICOM file...
	 */
	stemp = (void *) malloc(rtnLength);
	if (stemp == NULL) {
	    COND_DumpConditions();
	    exit(1);
	}
	redlutdesc.length = rtnLength;
	redlutdesc.d.ot = stemp;

	ctx = NULL;
	if (DCM_GetElementValue(object, &redlutdesc, &rtnLength, &ctx) != DCM_NORMAL) {
	    COND_DumpConditions();
	    exit(1);
	}
	red_entries = *stemp;
	red_first_map = *(stemp + 1);
/*	red_bits_per_entry = *(stemp + 2); */

	if (DCM_GetElementSize(object, bluelutdesc.tag, &rtnLength) != DCM_NORMAL) {
	    COND_DumpConditions();
	    exit(1);
	}
	/*
	 * Check the of the LUT descriptor to make sure we have the correct
	 * amount...
	 */
	if (rtnLength != 3 * sizeof(short)) {
	    COND_DumpConditions();
	    exit(1);
	}
	bluelutdesc.length = rtnLength;
	bluelutdesc.d.ot = stemp;

	ctx = NULL;
	if (DCM_GetElementValue(object, &bluelutdesc, &rtnLength, &ctx) != DCM_NORMAL) {
	    COND_DumpConditions();
	    exit(1);
	}
	blue_entries = *stemp;
	blue_first_map = *(stemp + 1);
/*	blue_bits_per_entry = *(stemp + 2); */

	if (DCM_GetElementSize(object, greenlutdesc.tag, &rtnLength) != DCM_NORMAL) {
	    COND_DumpConditions();
	    exit(1);
	}
	/*
	 * Check the of the LUT descriptor to make sure we have the correct
	 * amount...
	 */
	if (rtnLength != 3 * sizeof(short)) {
	    COND_DumpConditions();
	    exit(1);
	}
	greenlutdesc.length = rtnLength;
	greenlutdesc.d.ot = stemp;

	ctx = NULL;
	if (DCM_GetElementValue(object, &greenlutdesc, &rtnLength, &ctx) != DCM_NORMAL) {
	    COND_DumpConditions();
	    exit(1);
	}
	green_entries = *stemp;
	green_first_map = *(stemp + 1);
/*	green_bits_per_entry = *(stemp + 2); */

	free(stemp);
	/*
	 * Check to make sure all the luts have the same number of entries...
	 */
	if ((red_entries != blue_entries) ||
	    (red_entries != green_entries)) {
	    exit(1);
	}
	/*
	 * Get the RED LUT...
	 */
	if (DCM_GetElementSize(object, redlut.tag, &rtnLength) != DCM_NORMAL) {
	    COND_DumpConditions();
	    exit(1);
	}
	/*
	 * Check the of the LUT descriptor to make sure we have the correct
	 * amount...
	 */
	if (rtnLength != (red_entries * sizeof(short))) {
	    exit(1);
	}
	/*
	 * Allocate the memory to hold the RED LUT data and get it from the
	 * DICOM file...
	 */
	stemp = (void *) malloc(rtnLength);
	if (stemp == NULL) {
	    exit(1);
	}
	redlut.length = rtnLength;
	redlut.d.ot = stemp;

	ctx = NULL;
	if (DCM_GetElementValue(object, &redlut, &rtnLength, &ctx) != DCM_NORMAL) {
	    exit(1);
	}
	red_table = (unsigned short *) stemp;

	/*
	 * Get the BLUE LUT
	 */
	if (DCM_GetElementSize(object, bluelut.tag, &rtnLength) != DCM_NORMAL) {
	    exit(1);
	}
	/*
	 * Check the of the LUT descriptor to make sure we have the correct
	 * amount...
	 */
	if (rtnLength != (blue_entries * sizeof(short))) {
	    exit(1);
	}
	/*
	 * Allocate the memory to hold the BLUE LUT data and get it from the
	 * DICOM file...
	 */
	stemp = (void *) malloc(rtnLength);
	if (stemp == NULL) {
	    exit(1);
	}
	bluelut.length = rtnLength;
	bluelut.d.ot = stemp;

	ctx = NULL;
	if (DCM_GetElementValue(object, &bluelut, &rtnLength, &ctx) != DCM_NORMAL) {
	    exit(1);
	}
	blue_table = (unsigned short *) stemp;

	/*
	 * Get the GREEN LUT
	 */
	if (DCM_GetElementSize(object, greenlut.tag, &rtnLength) != DCM_NORMAL) {
	    exit(1);
	}
	/*
	 * Check the GREEN LUT descriptor to make sure we have the correct
	 * amount...
	 */
	if (rtnLength != (green_entries * sizeof(short))) {
	    exit(1);
	}
	/*
	 * Allocate the memory to hold the LUT data and get it from the DICOM
	 * file...
	 */
	stemp = (void *) malloc(rtnLength);
	if (stemp == NULL) {
	    exit(1);
	}
	greenlut.length = rtnLength;
	greenlut.d.ot = stemp;

	ctx = NULL;
	if (DCM_GetElementValue(object, &greenlut, &rtnLength, &ctx) != DCM_NORMAL) {
	    exit(1);
	}
	green_table = (unsigned short *) stemp;
	/*
	 * Now we check to see if we have 8 bit data, if so we will rescale
	 * the LUTS from 0 to 255...we're going to lose it somewhere, so we
	 * may as well rescale the smallest amount of data possible...
	 */
	if (bitsAllocated == 8) {
	    int max,
	        index;
	    float sf;
	    max = 0;
	    for (index = 0; index < red_entries; index++) {
		if ((int) red_table[index] > max)
		    max = red_table[index];
		if ((int) green_table[index] > max)
		    max = green_table[index];
		if ((int) blue_table[index] > max)
		    max = blue_table[index];
	    }
	    if (max > 0)
		sf = (float) 255.0 / (float) max;
	    else
		sf = (float) 1.;
	    for (index = 0; index < red_entries; index++) {
/*lint -e524 */
#ifdef _WIN32
#pragma warning(disable:4244)
#endif
		red_table[index] *= sf;
		green_table[index] *= sf;
		blue_table[index] *= sf;
/*lint +e524 */
	    }
	} {
	    short *cs,
	       *ncs;
	    unsigned char *bs,
	       *nbs;
	    int index;

	    if (bitsAllocated == 16) {	/* 16 bits per pixel and PALETTE
					 * COLOR */
		ncs = (short *) mappedPixels;
		cs = (short *) pixels;
		for (index = 0; index < count; index++) {
		    if (*cs < red_first_map) {
			*ncs = (short) (
					(RED_COMP * (float) (*red_table)) +
				     (GREEN_COMP * (float) (*green_table)) +
					(BLUE_COMP * (float) (*blue_table))
			    );
		    } else if (*cs > (red_first_map + red_entries - 1)) {
			*ncs = (short) (
					(RED_COMP * (float) (*(red_table + red_entries - 1))) +
					(GREEN_COMP * (float) (*(green_table + green_entries - 1))) +
					(BLUE_COMP * (float) (*(blue_table + blue_entries - 1)))
			    );
		    } else {
			*ncs = (short) (
					(RED_COMP * (float) (*(red_table + (*cs - red_first_map)))) +
					(GREEN_COMP * (float) (*(green_table + (*cs - green_first_map)))) +
					(BLUE_COMP * (float) (*(blue_table + (*cs - blue_first_map))))
			    );
		    }
		    ncs++;
		    cs++;
		}
	    } else {		/* 8 bits per pixel and PALETTE COLOR */
		nbs = (unsigned char *) mappedPixels;
		bs = (unsigned char *) pixels;
		for (index = 0; index < count; index++) {
		    if (*bs < (unsigned char) red_first_map) {
			*nbs = (unsigned char) (
					 (RED_COMP * (float) (*red_table)) +
				     (GREEN_COMP * (float) (*green_table)) +
					 (BLUE_COMP * (float) (*blue_table))
			    );
		    } else if (*bs > (unsigned char) (red_first_map + red_entries - 1)) {
			*nbs = (unsigned char) (
						(RED_COMP * (float) (*(red_table + red_entries - 1))) +
						(GREEN_COMP * (float) (*(green_table + green_entries - 1))) +
						(BLUE_COMP * (float) (*(blue_table + blue_entries - 1)))
			    );
		    } else {
			*nbs = (unsigned char) (
						(RED_COMP * (float) (*(red_table + (*bs - red_first_map)))) +
						(GREEN_COMP * (float) (*(green_table + (*bs - green_first_map)))) +
						(BLUE_COMP * (float) (*(blue_table + (*bs - blue_first_map))))
			    );
		    }
		    nbs++;
		    bs++;
		}
	    }
	}
    }
}
