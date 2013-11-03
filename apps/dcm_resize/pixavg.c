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
** Module Name(s):	pixelResize
**			pixelavg16signed
**			pixelavg8unsigned
** Author, Date:	Stephen M. More, 18-June-1992
** Intent:		Subroutines to resize images.
**
** Last Update:		$Author: smm $, $Date: 1997-10-15 04:02:42 $
** Source File:		$RCSfile: pixavg.c,v $
** Revision:		$Revision: 1.7 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.7 $ $RCSfile: pixavg.c,v $";

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#ifndef _MSC_VER
#include <sys/time.h>
#endif
#include "dicom.h"
#include "pixavg.h"

static int Mask_Data = 0;
static int And_Flag = 0;

/* pixelResize
**
** Purpose:
**	Resize the image to specified size
**
** Parameter Dictionary:
**	pixels		Pixel data
**	rows		Number of rows
**	cols		Number of columns
**	type		Image type (e.g. signed 16bit)
**	desiredRows	Desired number of rows in resized image
**	desiredCols	Desired number of columns in resized image
**	mask_data	Data mask
**	and_flag	Flag mask
**	pixout		Resized pixel data
**	newRows		Actual number of rows in the resized image
**	newCols		Actual number of columns in resized image
**
** Return Values:
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
pixelResize(void *pixels, unsigned long rows, unsigned long cols,
	    int type, unsigned long desiredRows, unsigned long desiredCols, int mask_data, int and_flag,
	    void **pixout, unsigned long *newRows, unsigned long *newCols)
{
    unsigned long
        factorRows,
        factorCols,
        factor;
    CONDITION
	cond;

    factorRows = rows / desiredRows;
    factorCols = cols / desiredCols;
    if (factorRows == 0)
	factorRows++;
    if (factorCols == 0)
	factorCols++;

    factor = (factorRows > factorCols) ? factorRows : factorCols;

    if (rows / factor > desiredRows)
	factor++;
    if (cols / factor > desiredCols)
	factor++;

    Mask_Data = mask_data;
    And_Flag = and_flag;
    switch (type) {
    case SIGNED_16BIT:
	cond = pixelavg16signed((short *) pixels, rows, cols, factor,
				(short **) pixout, newRows, newCols);
	break;
    case UNSIGNED_8BIT:
	cond = pixelavg8unsigned((unsigned char *) pixels, rows, cols, factor,
				 (short **) pixout, newRows, newCols);
	break;
    default:
	printf("Unimplemented type %d\n", type);
	exit(1);
    }
    return cond;
}

/* pixelavg16signed
**
** Purpose:
**	Resize a signed 16 bit image
**
** Parameter Dictionary:
**	pixels		Stream of pixel data
**	rows		Number of rows in the image
**	cols		Number of columns in the image
**	factor		Resizing factor
**	pixout		Resized pixel data
**	newRows		Actual number of rows in resized image
**	newCols		Actual number of columns in resized image
**
** Return Values:
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
pixelavg16signed(short *pixels, unsigned long rows,
		 unsigned long cols, unsigned long factor,
	     short **pixout, unsigned long *newRows, unsigned long *newCols)
{
    unsigned long
        i,
        j,
        k,
        l;
    short
       *p,
       *t;
    unsigned long
        r,
        c;
    float
        sum,
        divisor;

    r = rows / factor;
    c = cols / factor;
    divisor = (float) (factor * factor);

    p = malloc(r * c * sizeof(short));
    if (p == NULL)
	return 0;

    *pixout = p;
    for (i = 0; i < r; i++) {
	for (j = 0; j < c; j++) {
	    sum = 0;
	    t = pixels + (i * factor * cols) + (j * factor);
	    for (k = 0; k < factor; k++) {
		for (l = 0; l < factor; l++) {
		    if (Mask_Data) {
			if (And_Flag) {
			    t[l] &= Mask_Data;
			} else {
			    if (t[l] & Mask_Data) {
				t[l] |= Mask_Data;
			    }
			}
		    }
		    sum += (float) t[l];
		}
		t += cols;
	    }
	    *p++ = (short) (sum / divisor);
	}
    }
    *newRows = rows / factor;
    *newCols = cols / factor;
    return 1;
}

/* function name
**
** Purpose:
**	Resize an unsigned 8 bit type image
**
** Parameter Dictionary:
**	pixels		Stream of pixel data
**	rows		Number of rows in the image
**	cols		Number of columns in the image
**	factor		Resizing factor
**	pixout		Resized pixel data
**	newRows		Actual number of rows in resized image
**	newCols		Actual number of columns in resized image
**
** Return Values:
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
pixelavg8unsigned(unsigned char *pixels, unsigned long rows,
		  unsigned long cols, unsigned long factor,
	     short **pixout, unsigned long *newRows, unsigned long *newCols)
{
    unsigned long
        i,
        j,
        k,
        l;
    short
       *p;
    unsigned char
       *t;
    unsigned long
        r,
        c;
    float
        sum,
        divisor;

    r = rows / factor;
    c = cols / factor;
    divisor = (float) (factor * factor);

    p = malloc(r * c * sizeof(short));
    if (p == NULL)
	return 0;

    *pixout = p;
    for (i = 0; i < r; i++) {
	for (j = 0; j < c; j++) {
	    sum = 0;
	    t = pixels + (i * factor * cols) + (j * factor);
	    for (k = 0; k < factor; k++) {
		for (l = 0; l < factor; l++) {
		    sum += (float) t[l];
		}
		t += cols;
	    }
	    *p++ = (short) (sum / divisor);
	}
    }
    *newRows = rows / factor;
    *newCols = cols / factor;
    return 1;
}
