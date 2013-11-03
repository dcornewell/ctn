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
** Module Name(s):	main()
**			usageerror();
** Author, Date:	Stephen M. More, 18-June-1992
** Intent:		This program is an example and test progra which
**			demonstrates the DICOM packages developed at MIR.
**			This program uses the DICOM package to open ACR
**			files which contain pixel data and resize the data
**			using a simple pixel averaging scheme.
**			Pixels are assumed to be 16 bit signed integers
**			or unsigned 8 bit integers.
**  Usage:
**	dicom_resize [-b] [-v] [-c cols] [-r rows] input output
**
**  Description:
**	input	Input ACR-NEMA file
**	output	Output ACR-NEMA file
**  Options:
**	-b	big endian data
**	-v	verbose mode
**	-c	New columns in image
**	-r	New rows in image
**
** Last Update:		$Author: smm $, $Date: 2000-11-09 19:52:33 $
** Source File:		$RCSfile: dcm_resize.c,v $
** Revision:		$Revision: 1.15 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.15 $ $RCSfile: dcm_resize.c,v $";

#include <stdio.h>
#include <stdlib.h>
#include "dicom.h"
#include "ctnthread.h"
#include "lst.h"
#include "condition.h"
#include "dicom_objects.h"
#include "pixavg.h"

static void usageerror();

int
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
	elementLength;
    int
        r = 0,
        c = 0;
    short
       *pixelPtr;
    void
       *newPixels;
    static unsigned short
        bitsAllocated,
        rows,
        cols;
    void
       *ctx;
    unsigned long
        newRows,
        newCols;

    DCM_ELEMENT list[] = {
	{DCM_IMGBITSALLOCATED, DCM_US, "",
	1, sizeof(bitsAllocated), (void *) (&bitsAllocated)},
	{DCM_IMGCOLUMNS, DCM_US, "",
	1, sizeof(cols), (void *) (&cols)},
	{DCM_IMGROWS, DCM_US, "",
	1, sizeof(rows), (void *) (&rows)}
    },
        p2 = {DCM_PXLPIXELDATA, DCM_OW, "",
    1, 0, NULL};

    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'b':
	    options &= ~DCM_ORDERMASK;
	    options |= DCM_ORDERBIGENDIAN;
	    break;

	case 'c':
	    argc--;
	    argv++;
	    if (argc < 1)
		usageerror();
	    if (sscanf(*argv, "%d", &c) != 1)
		usageerror();
	    break;
	case 'r':
	    argc--;
	    argv++;
	    if (argc < 1)
		usageerror();
	    if (sscanf(*argv, "%d", &r) != 1)
		usageerror();
	    break;

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

    if (r == 0 && c == 0) {
	fprintf(stderr,
		"You have to specify a new value for rows and/or cols\n");
	usageerror();
    }
    if (argc < 2)
	usageerror();

    THR_Init();
    DCM_Debug(verbose);

    printf("DICOM File: %s\n", *argv);
    cond = DCM_OpenFile(*argv++, options, &object);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	return (1);
    }
    if (DCM_ParseObject(&object, list, DIM_OF(list),
			NULL, 0, NULL) != DCM_NORMAL) {
	fprintf(stderr, "Error retrieving part of the image\n");
	COND_DumpConditions();
	THR_Shutdown();
	return (1);
    }
    if ((bitsAllocated != 16) && (bitsAllocated != 8)) {
	fprintf(stderr, "Expected 8 or 16 bits allocated, got %d\n",
		bitsAllocated);
	THR_Shutdown();
	return (1);
    }
    if (r == 0)
	r = rows;

    if (c == 0)
	c = cols;

    cond = DCM_GetElementSize(&object, p2.tag, &elementLength);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	return (1);
    }
    pixelPtr = malloc(elementLength);
    if (pixelPtr == NULL) {
	perror("Allocating area for pixels");
	THR_Shutdown();
	return (1);
    }
    ctx = NULL;
    p2.length = elementLength;
    p2.d.ot = pixelPtr;
    cond = DCM_GetElementValue(&object, &p2, &elementLength, &ctx);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	return (1);
    }
    cond = pixelResize(pixelPtr, (unsigned long) rows, (unsigned long) cols,
		       (bitsAllocated == 16) ? SIGNED_16BIT : UNSIGNED_8BIT,
		       (unsigned long) r, (unsigned long) c,
		       0xffff, 1,
		       &newPixels, &newRows, &newCols);
    if (cond != 1) {
	fprintf(stderr, "Error resizing pixels");
	THR_Shutdown();
	return (1);
    }
    bitsAllocated = 16;
    p2.length = newRows * newCols * (bitsAllocated / 8);
    p2.d.ot = newPixels;
    p2.representation = DCM_OW;
    if (DCM_RemoveElement(&object, p2.tag) != DCM_NORMAL) {
	fprintf(stderr, "Error removing old pixel data from object\n");
	COND_DumpConditions();
	THR_Shutdown();
	return (1);
    }
    if (DCM_AddElement(&object, &p2) != DCM_NORMAL) {
	fprintf(stderr, "Error adding new pixel data to object\n");
	COND_DumpConditions();
	THR_Shutdown();
	return (1);
    }
    rows = (unsigned short)newRows;
    cols = (unsigned short)newCols;
    for (i = 0; i < DIM_OF(list); i++) {
	if (DCM_RemoveElement(&object, list[i].tag) != DCM_NORMAL) {
	    fprintf(stderr, "Error removing data element from object\n");
	    COND_DumpConditions();
	    THR_Shutdown();
	    return (1);
	}
	if (DCM_AddElement(&object, &list[i]) != DCM_NORMAL) {
	    fprintf(stderr, "Error adding new data element to object\n");
	    COND_DumpConditions();
	    THR_Shutdown();
	    return (1);
	}
    }

    if (verbose)
	(void) DCM_DumpElements(&object, 0);
    if (DCM_WriteFile(&object, options, *argv) != DCM_NORMAL) {
	fprintf(stderr, "Error writing new DICOM image file\n");
	COND_DumpConditions();
	THR_Shutdown();
	return (1);
    }
    THR_Shutdown();
    return 0;
}

/* usageError
**
** Purpose:
**	Displays the correct usage of the command invocation
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
Usage: dcm_resize [-b] [-c cols] [-r rows] [-t] [-v] input output\n\
\n\
  -b  Read file with big endian transfer syntax\n\
  -c  New number of columns in image\n\
  -r  New number of rows in image\n\
  -t  Read file with DICOM Part 10 file format\n\
  -v  Place DCM facility in verbose mode\n";

    fprintf(stderr, msg);
    exit(1);
}
