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
** Last Update:		$Author: pkc $, $Date: 2006-05-11 22:11:36 $
** Source File:		$RCSfile: dcm_pixel_bits.c,v $
** Revision:		$Revision: 1.3 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.3 $ $RCSfile: dcm_pixel_bits.c,v $";
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

static void usageerror();

/* This function applies an operator and a mask value to pixel data*/

static void
applyMask(DCM_OBJECT ** object, void *pixels, int count,
    int bitsAllocated, int bitsStored, int highBit, int pixelRepresentation,
    const char* operator, int mask)
{
    S16 *p16;
    U16 localMask;

    if ((strcmp(operator, "AND") == 0) && (bitsAllocated == 16)) {
	p16 = (S16 *) pixels;
	localMask = (U16)mask;
/*	    printf("localMask: %d\n", localMask);
	    printf("Operator: %s\n", operator);
	    printf("Mask: %x\n", mask);
	    printf("BitsAllocated: %d\n", bitsAllocated);
	    printf("Count: %d\n", count);
	    printf("pixels: %x\n", *p16); */
	while (count-- > 0) {
/*		printf("Count: %d\n", count);
		printf("*p16 Before: %#4x\n", *p16);
		printf("p16 Before: %x\n", p16);*/
	    *p16 = *p16 & mask;

/*	    	printf("*p16 After: %#4x\n\n", *p16); */
		
	    p16++;
	}
    } else if ((strcmp(operator, "OR") == 0) && (bitsAllocated == 16)) {
	p16 = (S16 *) pixels;
	localMask = (U16)mask;
	while (count-- > 0) {
/*		printf("Count: %d\n", count);
		printf("*p16 Before: %#4x\n", *p16);
		printf("p16 Before: %x\n", p16); */
	    *p16 = *p16 | mask;
/*	    	printf("*p16 After: %#4x\n\n", *p16); */

	    p16++;
	}
    } else {
	fprintf(stderr, "Unrecognized combination of bits allocated (%d) and operator (%s)\n",
		bitsAllocated, operator);
	exit(1);
    }
}

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
    char *outFile = "";
    char *operator = "";
    int mask = 0;

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
	case 't':
	    options &= ~DCM_FILEFORMATMASK;
	    options |= DCM_PART10FILE;
	    break;
	case 'v':
	    verbose = TRUE;
	    break;
	default:
	    break;
	}
    }

    THR_Init();
    DCM_Debug(verbose);
    if (argc < 4)
	usageerror();

    printf("DCM File: %s\n", *argv);
    cond = DCM_OpenFile(*argv++, options, &object);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    outFile = *argv++;
    operator = *argv++;
    sscanf(*argv, "%x", &mask);

    printf("Mask: %x\n", mask);

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

    pixels = malloc(pixelLength);
    if (pixels == NULL) {
	perror("Malloc of pixel data");
	THR_Shutdown();
	exit(1);
    }

    pixelCount = pixelLength/2;
    newPixels = malloc(pixelLength);
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

    applyMask(&object, pixels, pixelCount, bitsAllocated, bitsStored,
		   highBit, pixelRepresentation,
		operator, mask);

    if (DCM_RemoveElement(&object, p2.tag) != DCM_NORMAL) {
	fprintf(stderr, "Error removing old pixel data from object\n");
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    p2.length = pixelLength;
    if (p2.length & 1)
	p2.length++;

    swab(pixels, newPixels, pixelLength );
    p2.d.ot = newPixels;
    if (DCM_AddElement(&object, &p2) != DCM_NORMAL) {
	fprintf(stderr, "Error adding new pixel data to object\n");
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }

    if (DCM_WriteFile(&object, DCM_ORDERLITTLEENDIAN, outFile) != DCM_NORMAL) {
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
  char msg[] = "\
Usage: dcm_pixel_bits [-t] [-v] input output operator mask\n\
  -t         Use part 10 files\n\
  -v         Verbose mode\n\
  input      Input file\n\
  output     Output file\n\
  operator   AND, OR \n\
  mask       Hexadecimal mask\n";
    fprintf(stderr, msg);
    exit(1);
}
