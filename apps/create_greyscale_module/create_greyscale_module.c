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
** Author, Date:	Stephen M. Moore, 2-Aug-93
** Intent:
** Usage:		create_greyscale_module input output
** Last Update:		$Author: smm $, $Date: 1998-08-03 16:08:51 $
** Source File:		$RCSfile: create_greyscale_module.c,v $
** Revision:		$Revision: 1.10 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.10 $ $RCSfile: create_greyscale_module.c,v $";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dicom.h"
#include "ctnthread.h"
#include "lst.h"
#include "dicom_objects.h"
#include "condition.h"

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
        options = DCM_ORDERLITTLEENDIAN;
    U32
	pixelLength,
	elementLength;
    short
       *pixels;
    static unsigned short
        rows,
        columns,
        bitsAllocated,
        bitsStored,
        highBit,
        pixelRepresentation,
        samplesPerPixel;
    static char
        photometricInterp[DICOM_CS_LENGTH + 1];
    static char
        aspectRatio[] = "1\\1";
    void
       *ctx;

    DCM_ELEMENT
	p2 = {
	DCM_PXLPIXELDATA, DCM_OT, "", 1, 0, NULL
    };
    DCM_ELEMENT elementAspectRatio =
    {DCM_IMGPIXELASPECTRATIO, DCM_IS, "",
    2, sizeof(aspectRatio), (void *) aspectRatio};

    static DCM_ELEMENT list[] = {
	{DCM_IMGSAMPLESPERPIXEL, DCM_US, "",
	1, sizeof(samplesPerPixel), (void *) &samplesPerPixel},
	{DCM_IMGPHOTOMETRICINTERP, DCM_CS, "",
	1, sizeof(photometricInterp), (void *) photometricInterp},
	{DCM_IMGROWS, DCM_US, "",
	1, sizeof(rows), (void *) &rows},
	{DCM_IMGCOLUMNS, DCM_US, "",
	1, sizeof(columns), (void *) &columns},
	{DCM_IMGBITSALLOCATED, DCM_US, "",
	1, sizeof(bitsAllocated), (void *) &bitsAllocated},
	{DCM_IMGBITSSTORED, DCM_US, "",
	1, sizeof(bitsStored), (void *) &bitsStored},
	{DCM_IMGHIGHBIT, DCM_US, "",
	1, sizeof(highBit), (void *) &highBit},
	{DCM_IMGPIXELREPRESENTATION, DCM_US, "",
	1, sizeof(pixelRepresentation), (void *) &pixelRepresentation},
    };

    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'b':
	    options &= ~DCM_ORDERMASK;
	    options |= DCM_ORDERBIGENDIAN;
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
    if (argc < 2)
	usageerror();

    printf("DCM File: %s\n", *argv);
    cond = DCM_OpenFile(*argv++, options, &object);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    cond = DCM_ParseObject(&object, list, (int) DIM_OF(list), NULL, 0, NULL);
    if (cond != DCM_NORMAL) {
	fprintf(stderr, "Error parsing input image\n");
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    if (bitsAllocated != 8) {
	fprintf(stderr, "Expected 8 bits allocated, got %d\n", bitsAllocated);
	THR_Shutdown();
	exit(1);
    }
    if (highBit != 7) {
	fprintf(stderr, "Expected high bit to be 7, got %d\n", highBit);
	THR_Shutdown();
	exit(1);
    }
    if (pixelRepresentation != 0) {
	fprintf(stderr, "Expected pixel representation to be 0, got %d\n",
		pixelRepresentation);
	THR_Shutdown();
	exit(1);
    }
    if (samplesPerPixel != 1) {
	fprintf(stderr, "Expected samples/pixel to be 1, got %d\n",
		samplesPerPixel);
	THR_Shutdown();
	exit(1);
    }
    if ((strcmp(photometricInterp, "MONOCHROME1") != 0) &&
	(strcmp(photometricInterp, "MONOCHROME2") != 0)) {
	fprintf(stderr, "Photometric interpretation cannot be %s\n",
		photometricInterp);
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
    p2.length = pixelLength;
    p2.d.ot = pixels;
    ctx = NULL;
    if (DCM_GetElementValue(&object, &p2, &elementLength, &ctx) != DCM_NORMAL) {
	fprintf(stderr, "Error retrieving pixels from image\n");
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    (void) DCM_CloseObject(&object);
    cond = DCM_CreateObject(&object, 0);
    if (cond != DCM_NORMAL) {
	fprintf(stderr, "Error creating new object\n");
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    cond = DCM_ModifyElements(&object, list, (int) DIM_OF(list), NULL, 0, NULL);
    if (cond != DCM_NORMAL) {
	fprintf(stderr, "Error adding elements to new object\n");
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    cond = DCM_AddElement(&object, &elementAspectRatio);
    if (cond != DCM_NORMAL) {
	fprintf(stderr, "Error adding aspect ratio to new object\n");
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    if (DCM_AddElement(&object, &p2) != DCM_NORMAL) {
	fprintf(stderr, "Error adding new pixel data to object\n");
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    if (DCM_WriteFile(&object, DCM_ORDERLITTLEENDIAN, *argv) != DCM_NORMAL) {
	fprintf(stderr, "Error writing new DCM image file\n");
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    THR_Shutdown();
    exit(0);
}

/* usageerror
**
** Purpose:
**	Display the correct usage of the command along with all options
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
	    "Usage: create_greyscale_module input output\n");
    exit(1);
}
