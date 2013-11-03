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
** Module Name(s):	GetDICOMData, ScaleImageData,
**			pixelavg16signed, pixelavg8unsigned,
**			pixelexp16signed, pixelexp8unsigned,
**			pixelResize
** Author, Date:	David E. Beecher, Stephen M. Moore, 30-Jun-93
** Intent:		Utility routines to retreive and scale dicom image
**			data for display purposes.
** Last Update:		$Author: smm $, $Date: 1999-03-10 17:44:49 $
** Source File:		$RCSfile: image_utils.c,v $
** Revision:		$Revision: 1.1 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.1 $ $RCSfile: image_utils.c,v $";
#include	<stdio.h>
#include 	<stdlib.h>
#include	<fcntl.h>
#include 	<sys/types.h>
#ifndef _MSC_VER
#include 	<sys/time.h>
#endif
#include	<math.h>

#if 0
#include        <X11/Xlib.h>
#include        <X11/Xutil.h>
#include        <X11/Xos.h>
#include        <X11/Xatom.h>
#endif

#include	"dicom.h"
#include	"condition.h"
#include	"lst.h"
#include	"dicom_objects.h"
#include	"ctndisp.h"

#define	SIGNED_16BIT	1
#define	SIGNED_8BIT	2
#define	UNSIGNED_16BIT	3
#define	UNSIGNED_8BIT	4

static short
pixelexp16signed(short *pixels, unsigned long rows,
		 unsigned long cols, unsigned long factor,
	    short **pixout, unsigned long *newRows, unsigned long *newCols);

static short
pixelexp8unsigned(unsigned char *pixels, unsigned long rows,
		  unsigned long cols, unsigned long factor,
	    short **pixout, unsigned long *newRows, unsigned long *newCols);

static short
pixelavg16signed(short *pixels, unsigned long rows,
		 unsigned long cols, unsigned long factor,
	    short **pixout, unsigned long *newRows, unsigned long *newCols);

static short
pixelavg8unsigned(unsigned char *pixels, unsigned long rows,
		  unsigned long cols, unsigned long factor,
	    short **pixout, unsigned long *newRows, unsigned long *newCols);

short
pixelResize(void *pixels, unsigned long rows, unsigned long cols,
	    int type, unsigned long desiredRows, unsigned long desiredCols,
	    void **pixout, unsigned long *newRows, unsigned long *newCols, int expand_data);


extern int
    G_display_height,
    G_display_width,
    G_ncolors;

extern unsigned long
    graylut[];

void *
GetDICOMData(const char *filename, unsigned long options,
	     ImageStruct * idata, int overrideWidth, int overrideCenter)
{
    DCM_OBJECT
    * object;			/* generic object		 */
    void
       *new_pixels,
       *pixels;			/* will hold address of pixel data */

    static char
        WindowWidth[128] = "",	/* strings for TEXT (DCM_IS) elements */
        WindowCenter[128] = "",
        Slope[128] = "",
        Intercept[128] = "",
        temp[128] = "";

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
/*        red_bits_per_entry, */
        blue_entries,
        blue_first_map,
/*        blue_bits_per_entry, */
        green_entries,
        green_first_map;
/*        green_bits_per_entry; */

    static unsigned short	/* These changed to unsigned shorts 	 */
       *red_table,		/* to address a problem with 8 bit data */
       *blue_table,
       *green_table;

    U32
	real_length,
	rtnLength;		/* Needed for Get Element calls	 */
    void
       *ctx;

    CONDITION
	cond,			/* various condition vars	 */
	cond1,
	cond2;
    char
       *cptr;			/* temps...			 */
    short
       *stemp;
    int
        idx;
    U32
	check_length_in_bytes;

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
	p = {
	DCM_PXLPIXELDATA, DCM_OT, "",
	    1, 0, NULL
    },
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
	date = {
	DCM_IDSTUDYDATE, DCM_AT, "",
	    1, sizeof(temp), (void *) temp
    },
	modality = {
	DCM_IDMODALITY, DCM_AT, "",
	    1, sizeof(temp), (void *) temp
    },
	name = {
	DCM_PATNAME, DCM_AT, "",
	    1, sizeof(temp), (void *) temp
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
    },
	photointerp = {
	DCM_IMGPHOTOMETRICINTERP, DCM_CS, "",
	    1, sizeof(temp), (void *) temp
    };


    DCM_Debug(FALSE);
    cond = DCM_OpenFile(filename, options, &object);
    if (cond != DCM_NORMAL) {
	idata->error_condition = TRUE;
	sprintf(idata->error_message, "Can't open ACR file: %s", filename);
	COND_PushCondition(DICOMXDISP, "Can't open ACR file: %s", filename);
	return ((void *) NULL);
    }
    /*
     * Get the required elements...
     */
    if (DCM_ParseObject(&object, required, 7, NULL, 0, NULL) != DCM_NORMAL) {
	idata->error_condition = TRUE;
	sprintf(idata->error_message, "Required parameter(s) missing: %s", filename);
	COND_PushCondition(DICOMXDISP, "Required parameter(s) missing: %s", filename);
	(void) DCM_CloseObject(&object);
	return ((void *) NULL);
    }
    idata->sampsperpixel = sampsperpixel;
    idata->nrows = nrows;
    idata->ncols = ncols;
    idata->bitsallocated = bitsallocated;
    idata->bitsstored = bitsstored;
    idata->highbit = highbit;
    idata->pixrep = pixrep;
/*
 * Get the actual  pixel data (the only other required element)
 */
    if (DCM_GetElementSize(&object, p.tag, &rtnLength) != DCM_NORMAL) {
	idata->error_condition = TRUE;
	sprintf(idata->error_message, "Can't get pixel data size for: %s", filename);
	COND_PushCondition(DICOMXDISP, "Can't get pixel data size for: %s", filename);
	(void) DCM_CloseObject(&object);
	return ((void *) NULL);
    }
/*
 * Check the size of the pixel data to make sure we have the correct amount...
 */
    real_length = rtnLength;
    check_length_in_bytes = nrows * ncols * sampsperpixel;
    if (bitsallocated == 16)
	check_length_in_bytes *= 2;
    if (check_length_in_bytes > real_length) {
	idata->error_condition = TRUE;
	sprintf(idata->error_message, "Too little pixel data supplied");
	COND_PushCondition(DICOMXDISP, "Too little pixel data supplied");
	(void) DCM_CloseObject(&object);
	return ((void *) NULL);
    }
/*
 * Now allocate the memory to hold the pixel data and get it from the DICOM file...
 */
    pixels = (void *) malloc(check_length_in_bytes);
    if (pixels == NULL) {
	idata->error_condition = TRUE;
	sprintf(idata->error_message, "Can't allocate memory for: %s", filename);
	COND_PushCondition(DICOMXDISP, "Can't allocate memory for: %s", filename);
	(void) DCM_CloseObject(&object);
	return ((void *) NULL);
    }
    p.length = check_length_in_bytes;
    p.d.ot = pixels;

    ctx = NULL;
    cond = DCM_GetElementValue(&object, &p, &rtnLength, &ctx);
    if ((cond != DCM_NORMAL) && (cond != DCM_GETINCOMPLETE)) {
	idata->error_condition = TRUE;
	sprintf(idata->error_message, "Can't get pixel data for: %s", filename);
	COND_PushCondition(DICOMXDISP, "Can't get pixel data for: %s", filename);
	free(pixels);
	(void) DCM_CloseObject(&object);
	return ((void *) NULL);
    }
    if ((check_length_in_bytes < real_length) && (cond != DCM_GETINCOMPLETE)) {
	idata->error_condition = TRUE;
	sprintf(idata->error_message, "Can't get pixel data for: %s", filename);
	COND_PushCondition(DICOMXDISP, "Can't get pixel data for: %s", filename);
	free(pixels);
	(void) DCM_CloseObject(&object);
	return ((void *) NULL);
    }
    COND_PushCondition(DICOMXDISP, " Samples per pixel: %d", sampsperpixel);
    COND_PushCondition(DICOMXDISP, "              Rows: %d", nrows);
    COND_PushCondition(DICOMXDISP, "           Columns: %d", ncols);
    COND_PushCondition(DICOMXDISP, "    Bits Allocated: %d", bitsallocated);
    COND_PushCondition(DICOMXDISP, "       Bits Stored: %d", bitsstored);
    COND_PushCondition(DICOMXDISP, "          High Bit: %d", highbit);
    COND_PushCondition(DICOMXDISP, "         Pixel Rep: %d", pixrep);
/*
 * Go get the photmetric interpretation...(this should be required but is optional
 * for this particular demonstration)
 */
    ctx = NULL;
    idata->photointerp = NOT_IMPLEMENTED;
    if (DCM_GetElementValue(&object, &photointerp, &rtnLength, &ctx) == DCM_NORMAL) {
	cptr = photointerp.d.string;
	cptr[rtnLength] = '\0';
	if (strncmp(cptr, DCM_IMGPHOTOINTERPMONOCHROME1, 11) == 0) {
	    idata->photointerp = MONOCHROME1;
	    COND_PushCondition(DICOMXDISP, "Photometric Interp: MONOCHROME1");
	} else if (strncmp(cptr, DCM_IMGPHOTOINTERPMONOCHROME2, 11) == 0) {
	    idata->photointerp = MONOCHROME2;
	    COND_PushCondition(DICOMXDISP, "Photometric Interp: MONOCHROME2");
	} else if (strncmp(cptr, DCM_IMGPHOTOINTERPPALETTECOLOR, 13) == 0) {
	    idata->photointerp = PALETTECOLOR;
	    COND_PushCondition(DICOMXDISP, "Photometric Interp: PALETTE COLOR");
	} else if (strncmp(cptr, DCM_IMGPHOTOINTERPRGB, 3) == 0) {
	    if (strncmp(cptr, DCM_IMGPHOTOINTERPRGBA, 4) == 0) {
		idata->photointerp = CTNRGBA;
		COND_PushCondition(DICOMXDISP, "Photometric Interp: RGBA");
	    } else {
		idata->photointerp = CTNRGB;
		COND_PushCondition(DICOMXDISP, "Photometric Interp: RGB");
	    }
	} else if (strncmp(cptr, DCM_IMGPHOTOINTERPHSV, 3) == 0) {
	    idata->photointerp = HSV;
	    COND_PushCondition(DICOMXDISP, "Photometric Interp: HSV");
	} else if (strncmp(cptr, DCM_IMGPHOTOINTERPCMYK, 4) == 0) {
	    idata->photointerp = CTNCMYK;
	    COND_PushCondition(DICOMXDISP, "Photometric Interp: CMYK");
	} else
	    COND_PushCondition(DICOMXDISP, "Photometric Interp: NOT SPECIFIED/NOT SUPPORTED");
    }
/*
 * Do preliminary shifting and scaling if necessary
 */
    if (sampsperpixel == 1) {
	int
	    mask_data,
	    and_flag,
	    shift_num;
	short
	   *cs;
	unsigned char
	   *bs;
	/*
	 * Mask the 16 bit data if necessary
	 */
	mask_data = 0;
	and_flag = 0;
	if ((idata->bitsallocated == 16) &&
	    (idata->pixrep == DCM_PIXELTWOSCOMPLEMENT)) {
	    if (idata->highbit > 0) {
		mask_data = 0xFFFFFFFF << (idata->highbit);
	    }
	} else if ((idata->bitsallocated == 16) &&
		   (idata->pixrep == DCM_PIXELUNSIGNED)) {
	    if (idata->highbit > 0) {
		and_flag = 1;
		mask_data = 0x0FFFF >> (15 - idata->highbit);
	    }
	}
	/*
	 * Check to see if the data was shifted...
	 */
	shift_num = 0;
	if ((idata->bitsstored - 1) != idata->highbit) {	/* Need to shift it? */
	    shift_num = idata->highbit - idata->bitsstored + 1;
	}
	if (mask_data | shift_num) {
	    if (idata->bitsallocated == 16) {
		cs = (short *) pixels;
		for (idx = 0; idx < (idata->nrows * idata->ncols); idx++) {
		    if (shift_num)
			*cs >>= shift_num;
		    if (mask_data) {
			if (and_flag) {
			    *cs &= mask_data;
			} else {
			    if (*cs & mask_data)
				*cs |= mask_data;
			}
		    }
		    cs++;
		}
	    } else if (idata->bitsallocated == 8) {
		bs = (unsigned char *) pixels;
		for (idx = 0; idx < (idata->nrows * idata->ncols); idx++) {
		    if (shift_num)
			*bs >>= shift_num;
		    if (mask_data) {
			if (and_flag) {
			    *bs &= mask_data;
			} else {
			    if (*bs & mask_data)
				*bs |= mask_data;
			}
		    }
		    bs++;
		}
	    }
	    idata->highbit = idata->bitsstored - 1;
	}
    }
/*
 * The planar configuration tells us if we need to intrepret multiple samples per pixel as
 * R1G1B1 R2G2B2.... or R1R2R3...G1G2G3...B1B2B3...
 */
    planarconfig = 0xffff;
    ctx = NULL;
    if (sampsperpixel > 1) {
	if (DCM_GetElementValue(&object, &pc, &rtnLength, &ctx) != DCM_NORMAL) {
	    idata->error_condition = TRUE;
	    sprintf(idata->error_message, "Planar Configuration not specified");
	    COND_PushCondition(DICOMXDISP, "Planar Configuration not specified");
	    free(pixels);
	    (void) DCM_CloseObject(&object);
	    return ((void *) NULL);
	}
    }
    idata->smethod = DEFAULT;

    switch (idata->photointerp) {
    case MONOCHROME1:
    case MONOCHROME2:
	break;
    case PALETTECOLOR:
    case CTNRGBA:
	/*
	 * Read in the color palette tables now b/c we need them for either
	 * of the preceding methods
	 */
	/*
	 * Get the Look up table descriptors
	 */
	if (DCM_GetElementSize(&object, redlutdesc.tag, &rtnLength) != DCM_NORMAL) {
	    idata->error_condition = TRUE;
	    sprintf(idata->error_message, "Can't get RED LUT descriptor");
	    COND_PushCondition(DICOMXDISP, "Can't get RED LUT descriptor");
	    free(pixels);
	    (void) DCM_CloseObject(&object);
	    return ((void *) NULL);
	}
	/*
	 * Check the of the LUT descriptor to make sure we have the correct
	 * amount...
	 */
	if (rtnLength != 3 * sizeof(short)) {
	    idata->error_condition = TRUE;
	    sprintf(idata->error_message, "Incorrect LUT descriptor supplied");
	    COND_PushCondition(DICOMXDISP, "Incorrect LUT descriptor supplied");
	    free(pixels);
	    (void) DCM_CloseObject(&object);
	    return ((void *) NULL);
	}
	/*
	 * Allocate the memory to hold the LUT descriptor data and get it
	 * from the DICOM file...
	 */
	stemp = (void *) malloc(rtnLength);
	if (stemp == NULL) {
	    idata->error_condition = TRUE;
	    sprintf(idata->error_message, "Can't allocate memory for RED LUT descriptor");
	    COND_PushCondition(DICOMXDISP, "Can't allocate memory for RED LUT descriptor");
	    free(pixels);
	    (void) DCM_CloseObject(&object);
	    return ((void *) NULL);
	}
	redlutdesc.length = rtnLength;
	redlutdesc.d.ot = stemp;

	ctx = NULL;
	if (DCM_GetElementValue(&object, &redlutdesc, &rtnLength, &ctx) != DCM_NORMAL) {
	    idata->error_condition = TRUE;
	    sprintf(idata->error_message, "Can't get RED LUT descriptor");
	    COND_PushCondition(DICOMXDISP, "Can't get RED LUT descriptor");
	    free(stemp);
	    free(pixels);
	    (void) DCM_CloseObject(&object);
	    return ((void *) NULL);
	}
	red_entries = *stemp;
	red_first_map = *(stemp + 1);
/*	red_bits_per_entry = *(stemp + 2); */

	if (DCM_GetElementSize(&object, bluelutdesc.tag, &rtnLength) != DCM_NORMAL) {
	    idata->error_condition = TRUE;
	    sprintf(idata->error_message, "Can't get BLUE LUT descriptor");
	    COND_PushCondition(DICOMXDISP, "Can't get BLUE LUT descriptor");
	    (void) DCM_CloseObject(&object);
	    return ((void *) NULL);
	}
	/*
	 * Check the of the LUT descriptor to make sure we have the correct
	 * amount...
	 */
	if (rtnLength != 3 * sizeof(short)) {
	    idata->error_condition = TRUE;
	    sprintf(idata->error_message, "Incorrect LUT descriptor supplied");
	    COND_PushCondition(DICOMXDISP, "Incorrect LUT descriptor supplied");
	    free(pixels);
	    (void) DCM_CloseObject(&object);
	    return ((void *) NULL);
	}
	bluelutdesc.length = rtnLength;
	bluelutdesc.d.ot = stemp;

	ctx = NULL;
	if (DCM_GetElementValue(&object, &bluelutdesc, &rtnLength, &ctx) != DCM_NORMAL) {
	    idata->error_condition = TRUE;
	    sprintf(idata->error_message, "Can't get BLUE LUT descriptor");
	    COND_PushCondition(DICOMXDISP, "Can't get BLUE LUT descriptor");
	    free(stemp);
	    free(pixels);
	    (void) DCM_CloseObject(&object);
	    return ((void *) NULL);
	}
	blue_entries = *stemp;
	blue_first_map = *(stemp + 1);
/*	blue_bits_per_entry = *(stemp + 2); */

	if (DCM_GetElementSize(&object, greenlutdesc.tag, &rtnLength) != DCM_NORMAL) {
	    idata->error_condition = TRUE;
	    sprintf(idata->error_message, "Can't get GREEN LUT descriptor");
	    COND_PushCondition(DICOMXDISP, "Can't get GREEN LUT descriptor");
	    (void) DCM_CloseObject(&object);
	    return ((void *) NULL);
	}
	/*
	 * Check the of the LUT descriptor to make sure we have the correct
	 * amount...
	 */
	if (rtnLength != 3 * sizeof(short)) {
	    idata->error_condition = TRUE;
	    sprintf(idata->error_message, "Incorrect LUT descriptor supplied");
	    COND_PushCondition(DICOMXDISP, "Incorrect LUT descriptor supplied");
	    free(pixels);
	    (void) DCM_CloseObject(&object);
	    return ((void *) NULL);
	}
	greenlutdesc.length = rtnLength;
	greenlutdesc.d.ot = stemp;

	ctx = NULL;
	if (DCM_GetElementValue(&object, &greenlutdesc, &rtnLength, &ctx) != DCM_NORMAL) {
	    idata->error_condition = TRUE;
	    sprintf(idata->error_message, "Can't get GREEN LUT descriptor");
	    COND_PushCondition(DICOMXDISP, "Can't get GREEN LUT descriptor");
	    free(stemp);
	    free(pixels);
	    (void) DCM_CloseObject(&object);
	    return ((void *) NULL);
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
	    idata->error_condition = TRUE;
	    sprintf(idata->error_message, "LUT's have different number of entries");
	    COND_PushCondition(DICOMXDISP, "LUT's have different number of entries");
	    free(pixels);
	    (void) DCM_CloseObject(&object);
	    return ((void *) NULL);
	}
	/*
	 * Get the RED LUT...
	 */
	if (DCM_GetElementSize(&object, redlut.tag, &rtnLength) != DCM_NORMAL) {
	    idata->error_condition = TRUE;
	    sprintf(idata->error_message, "Can't get RED LUT");
	    COND_PushCondition(DICOMXDISP, "Can't get RED LUT");
	    free(pixels);
	    (void) DCM_CloseObject(&object);
	    return ((void *) NULL);
	}
	/*
	 * Check the of the LUT descriptor to make sure we have the correct
	 * amount...
	 */
	if (rtnLength != (red_entries * sizeof(short))) {
	    idata->error_condition = TRUE;
	    sprintf(idata->error_message, "Incorrect RED LUT supplied");
	    COND_PushCondition(DICOMXDISP, "Incorrect RED LUT supplied");
	    free(pixels);
	    (void) DCM_CloseObject(&object);
	    return ((void *) NULL);
	}
	/*
	 * Allocate the memory to hold the RED LUT data and get it from the
	 * DICOM file...
	 */
	stemp = (void *) malloc(rtnLength);
	if (stemp == NULL) {
	    idata->error_condition = TRUE;
	    sprintf(idata->error_message, "Can't allocate memory for RED LUT");
	    COND_PushCondition(DICOMXDISP, "Can't allocate memory for RED LUT");
	    free(pixels);
	    (void) DCM_CloseObject(&object);
	    return ((void *) NULL);
	}
	redlut.length = rtnLength;
	redlut.d.ot = stemp;

	ctx = NULL;
	if (DCM_GetElementValue(&object, &redlut, &rtnLength, &ctx) != DCM_NORMAL) {
	    idata->error_condition = TRUE;
	    sprintf(idata->error_message, "Can't get RED LUT descriptor");
	    COND_PushCondition(DICOMXDISP, "Can't get RED LUT descriptor");
	    free(stemp);
	    free(pixels);
	    (void) DCM_CloseObject(&object);
	    return ((void *) NULL);
	}
	red_table = (unsigned short *) stemp;

	/*
	 * Get the BLUE LUT
	 */
	if (DCM_GetElementSize(&object, bluelut.tag, &rtnLength) != DCM_NORMAL) {
	    idata->error_condition = TRUE;
	    sprintf(idata->error_message, "Can't get BLUE LUT");
	    COND_PushCondition(DICOMXDISP, "Can't get BLUE LUT");
	    free(pixels);
	    free(red_table);
	    (void) DCM_CloseObject(&object);
	    return ((void *) NULL);
	}
	/*
	 * Check the of the LUT descriptor to make sure we have the correct
	 * amount...
	 */
	if (rtnLength != (blue_entries * sizeof(short))) {
	    idata->error_condition = TRUE;
	    sprintf(idata->error_message, "Incorrect BLUE LUT supplied");
	    COND_PushCondition(DICOMXDISP, "Incorrect BLUE LUT supplied");
	    free(pixels);
	    free(red_table);
	    (void) DCM_CloseObject(&object);
	    return ((void *) NULL);
	}
	/*
	 * Allocate the memory to hold the BLUE LUT data and get it from the
	 * DICOM file...
	 */
	stemp = (void *) malloc(rtnLength);
	if (stemp == NULL) {
	    idata->error_condition = TRUE;
	    sprintf(idata->error_message, "Can't allocate memory for BLUE LUT");
	    COND_PushCondition(DICOMXDISP, "Can't allocate memory for BLUE LUT");
	    free(pixels);
	    free(red_table);
	    (void) DCM_CloseObject(&object);
	    return ((void *) NULL);
	}
	bluelut.length = rtnLength;
	bluelut.d.ot = stemp;

	ctx = NULL;
	if (DCM_GetElementValue(&object, &bluelut, &rtnLength, &ctx) != DCM_NORMAL) {
	    idata->error_condition = TRUE;
	    sprintf(idata->error_message, "Can't get BLUE LUT descriptor");
	    COND_PushCondition(DICOMXDISP, "Can't get BLUE LUT descriptor");
	    free(stemp);
	    free(red_table);
	    free(pixels);
	    (void) DCM_CloseObject(&object);
	    return ((void *) NULL);
	}
	blue_table = (unsigned short *) stemp;

	/*
	 * Get the GREEN LUT
	 */
	if (DCM_GetElementSize(&object, greenlut.tag, &rtnLength) != DCM_NORMAL) {
	    idata->error_condition = TRUE;
	    sprintf(idata->error_message, "Can't get GREEN LUT");
	    COND_PushCondition(DICOMXDISP, "Can't get GREEN LUT");
	    free(red_table);
	    free(blue_table);
	    free(pixels);
	    (void) DCM_CloseObject(&object);
	    return ((void *) NULL);
	}
	/*
	 * Check the GREEN LUT descriptor to make sure we have the correct
	 * amount...
	 */
	if (rtnLength != (green_entries * sizeof(short))) {
	    idata->error_condition = TRUE;
	    sprintf(idata->error_message, "Incorrect GREEN LUT supplied");
	    COND_PushCondition(DICOMXDISP, "Incorrect GREEN LUT supplied");
	    free(red_table);
	    free(blue_table);
	    free(pixels);
	    (void) DCM_CloseObject(&object);
	    return ((void *) NULL);
	}
	/*
	 * Allocate the memory to hold the LUT data and get it from the DICOM
	 * file...
	 */
	stemp = (void *) malloc(rtnLength);
	if (stemp == NULL) {
	    idata->error_condition = TRUE;
	    sprintf(idata->error_message, "Can't allocate memory for GREEN LUT");
	    COND_PushCondition(DICOMXDISP, "Can't allocate memory for GREEN LUT");
	    free(red_table);
	    free(blue_table);
	    free(pixels);
	    (void) DCM_CloseObject(&object);
	    return ((void *) NULL);
	}
	greenlut.length = rtnLength;
	greenlut.d.ot = stemp;

	ctx = NULL;
	if (DCM_GetElementValue(&object, &greenlut, &rtnLength, &ctx) != DCM_NORMAL) {
	    idata->error_condition = TRUE;
	    sprintf(idata->error_message, "Can't get GREEN LUT descriptor");
	    COND_PushCondition(DICOMXDISP, "Can't get GREEN LUT descriptor");
	    free(stemp);
	    free(red_table);
	    free(blue_table);
	    free(pixels);
	    (void) DCM_CloseObject(&object);
	    return ((void *) NULL);
	}
	green_table = (unsigned short *) stemp;
	/*
	 * Now we check to see if we have 8 bit data, if so we will rescale
	 * the LUTS from 0 to 255...we're going to lose it somewhere, so we
	 * may as well rescale the smallest amount of data possible...
	 */
	if (idata->bitsallocated == 8) {
	    int max,
	        j;
	    float sf;
	    max = 0;
	    for (j = 0; j < red_entries; j++) {
		if ((int) red_table[j] > max)
		    max = red_table[j];
		if ((int) green_table[j] > max)
		    max = green_table[j];
		if ((int) blue_table[j] > max)
		    max = blue_table[j];
	    }
	    if (max > 0)
		sf = 255.0 / (float) max;
	    else
		sf = 1;
	    for (j = 0; j < red_entries; j++) {
/*lint -e524*/
		red_table[j] *= sf;
		green_table[j] *= sf;
		blue_table[j] *= sf;
/*lint +e524*/
	    }
	}
	if (idata->photointerp == PALETTECOLOR) {
	    short *cs,
	       *ncs;
	    unsigned char *bs,
	       *nbs;
	    int j,
	        xbyy;
	    if (idata->bitsallocated == 16)
		new_pixels = (void *) malloc(idata->nrows * idata->ncols * 2);
	    else
		new_pixels = (void *) malloc(idata->nrows * idata->ncols);
	    if (new_pixels == (void *) NULL) {
		idata->error_condition = TRUE;
		sprintf(idata->error_message, "Can't allocate enough local memory");
		COND_PushCondition(DICOMXDISP, "Can't allocate enough local memory");
		free(red_table);
		free(blue_table);
		free(green_table);
		free(pixels);
		(void) DCM_CloseObject(&object);
		return ((void *) NULL);
	    }
	    if (idata->bitsallocated == 16) {	/* 16 bits per pixel and
						 * PALETTE COLOR */
		ncs = (short *) new_pixels;
		cs = (short *) pixels;
		for (j = 0; j < (idata->nrows * idata->ncols); j++) {
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
		nbs = (unsigned char *) new_pixels;
		bs = (unsigned char *) pixels;
		for (j = 0; j < (idata->nrows * idata->ncols); j++) {
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
	    bs = (unsigned char *) pixels;
	    pixels = new_pixels;
	    free(bs);

	} else {		/* photointerp must be RGBA */
	    short *cs,
	       *ncs;
	    unsigned char *bs,
	       *nbs;
	    int j,
	        xbyy;

	    if (idata->bitsallocated == 16)
		new_pixels = (void *) malloc(idata->nrows * idata->ncols * 2);
	    else
		new_pixels = (void *) malloc(idata->nrows * idata->ncols);
	    if (new_pixels == (void *) NULL) {
		idata->error_condition = TRUE;
		sprintf(idata->error_message, "Can't allocate enough local memory");
		COND_PushCondition(DICOMXDISP, "Can't allocate enough local memory");
		free(pixels);
		(void) DCM_CloseObject(&object);
		return ((void *) NULL);
	    }
	    if (planarconfig == 0) {	/* R1G1B1A1 R2G2B2A2 ... */
		if (idata->bitsallocated == 16) {	/* 16 bits per pixel and
							 * RGBA */
		    ncs = (short *) new_pixels;
		    cs = (short *) pixels;
		    for (j = 0; j < (idata->nrows * idata->ncols); j++) {
			if (*(cs + 3) <= 0) {	/* use the RGB value */
			    *ncs = (short) (
					  (RED_COMP * (float) (*(cs + 0))) +
					(GREEN_COMP * (float) (*(cs + 1))) +
					    (BLUE_COMP * (float) (*(cs + 2)))
				);
			} else {/* use the lookup table value */
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
						(RED_COMP * (float) (*(red_table + ((*(cs + 0)) - red_first_map)))) +
						(GREEN_COMP * (float) (*(green_table + ((*(cs + 1)) - green_first_map)))) +
						(BLUE_COMP * (float) (*(blue_table + ((*(cs + 2)) - blue_first_map))))
				    );
			    }
			}
			ncs++;
			cs += 4;
		    }
		} else {	/* 8 bits per pixel and RGBA */
		    nbs = (unsigned char *) new_pixels;
		    bs = (unsigned char *) pixels;
		    for (j = 0; j < (idata->nrows * idata->ncols); j++) {
			if (*(bs + 3) <= 0) {
			    *nbs = (unsigned char) (
					  (RED_COMP * (float) (*(bs + 0))) +
					(GREEN_COMP * (float) (*(bs + 1))) +
					   (BLUE_COMP * (float) (*(bs + 2)))
				);
			} else {/* use the lookup table value */
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
							(RED_COMP * (float) (*(red_table + ((*bs + 0) - red_first_map)))) +
							(GREEN_COMP * (float) (*(green_table + ((*bs + 1) - green_first_map)))) +
							(BLUE_COMP * (float) (*(blue_table + ((*bs + 2) - blue_first_map))))
				    );
			    }
			}
			nbs++;
			bs += 4;
		    }
		}
	    } else {		/* R1R2R3...G1G2G3...B1B2B3...A1A2A3... */
		xbyy = idata->nrows * idata->ncols;
		if (idata->bitsallocated == 16) {	/* 16 bits per pixel and
							 * RGBA */
		    ncs = (short *) new_pixels;
		    cs = (short *) pixels;
		    for (j = 0; j < (idata->nrows * idata->ncols); j++) {
			if (*(cs + (3 * xbyy)) <= 0) {	/* use the RGB value */
			    *ncs = (short) (
					  (RED_COMP * (float) (*(cs + 0))) +
				     (GREEN_COMP * (float) (*(cs + xbyy))) +
				  (BLUE_COMP * (float) (*(cs + (2 * xbyy))))
				);
			} else {/* use the lookup table value */
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
						(RED_COMP * (float) (*(red_table + ((*(cs + 0)) - red_first_map)))) +
						(GREEN_COMP * (float) (*(green_table + ((*(cs + xbyy)) - green_first_map)))) +
						(BLUE_COMP * (float) (*(blue_table + ((*(cs + (2 * xbyy))) - blue_first_map))))
				    );
			    }
			}
			ncs++;
			cs++;
		    }
		} else {	/* 8 bits per pixel and RGBA */
		    nbs = (unsigned char *) new_pixels;
		    bs = (unsigned char *) pixels;
		    for (j = 0; j < (idata->nrows * idata->ncols); j++) {
			if (*(bs + (3 * xbyy)) <= 0) {
			    *nbs = (unsigned char) (
					  (RED_COMP * (float) (*(bs + 0))) +
				     (GREEN_COMP * (float) (*(bs + xbyy))) +
				  (BLUE_COMP * (float) (*(bs + (2 * xbyy))))
				);
			} else {/* use the lookup table value */
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
							(RED_COMP * (float) (*(red_table + ((*(bs + 0)) - red_first_map)))) +
							(GREEN_COMP * (float) (*(green_table + ((*(bs + xbyy)) - green_first_map)))) +
							(BLUE_COMP * (float) (*(blue_table + ((*(bs + (2 * xbyy))) - blue_first_map))))
				    );
			    }
			}
			nbs++;
			bs += 4;
		    }
		}
	    }
	    bs = (unsigned char *) pixels;
	    pixels = new_pixels;
	    free(bs);
	}
	free(red_table);
	free(blue_table);
	free(green_table);
	/*
	 * This is a giant hack to compensate for the sun displays so the
	 * intensity data looks right.  We (really)  need to apply another
	 * transform here...and that will be done when more time
	 * exists...most likely for the September 15th trial....DEBeecher.
	 * idata->smethod = WINDOW_LEVEL; idata->window = 200; idata->level =
	 * 50;
	 */
	break;

    case HSV:
    case CTNCMYK:
	idata->error_condition = TRUE;
	sprintf(idata->error_message, "Unimplemented Photo Interpretation");
	COND_PushCondition(DICOMXDISP, "Unimplemented Photo Interpretation");
	free(pixels);
	(void) DCM_CloseObject(&object);
	return ((void *) NULL);

    case CTNRGB:
	{
	    short *cs,
	       *ncs;
	    unsigned char *bs,
	       *nbs;
	    int i,
	        xbyy;
	    if (idata->bitsallocated == 16)
		new_pixels = (void *) malloc(idata->nrows * idata->ncols * 2);
	    else
		new_pixels = (void *) malloc(idata->nrows * idata->ncols);
	    if (new_pixels == (void *) NULL) {
		idata->error_condition = TRUE;
		sprintf(idata->error_message, "Can't allocate enough local memory");
		COND_PushCondition(DICOMXDISP, "Can't allocate enough local memory");
		free(pixels);
		(void) DCM_CloseObject(&object);
		return ((void *) NULL);
	    }
	    if (planarconfig == 0) {	/* R1G1B1 R2G2B2 ... */
		if (idata->bitsallocated == 16) {
		    ncs = (short *) new_pixels;
		    cs = (short *) pixels;
		    for (i = 0; i < (idata->nrows * idata->ncols); i++) {
			*ncs = (short) (
					(RED_COMP * (float) (*(cs + 0))) +
					(GREEN_COMP * (float) (*(cs + 1))) +
					(BLUE_COMP * (float) (*(cs + 2)))
			    );
			ncs++;
			cs += 3;
		    }
		} else {
		    nbs = (unsigned char *) new_pixels;
		    bs = (unsigned char *) pixels;
		    for (i = 0; i < (idata->nrows * idata->ncols); i++) {
			*nbs = (unsigned char) (
					  (RED_COMP * (float) (*(bs + 0))) +
					(GREEN_COMP * (float) (*(bs + 1))) +
					   (BLUE_COMP * (float) (*(bs + 2)))
			    );
			nbs++;
			bs += 3;
		    }
		}
	    } else {		/* R1R2R3...G1G2G3...B1B2B3... */
		xbyy = idata->nrows * idata->ncols;
		if (idata->bitsallocated == 16) {
		    ncs = (short *) new_pixels;
		    cs = (short *) pixels;
		    for (i = 0; i < (idata->nrows * idata->ncols); i++) {
			*ncs = (short) (
					(RED_COMP * (float) (*(cs + 0))) +
				     (GREEN_COMP * (float) (*(cs + xbyy))) +
				  (BLUE_COMP * (float) (*(cs + (2 * xbyy))))
			    );
			ncs++;
			cs++;
		    }
		} else {
		    nbs = (unsigned char *) new_pixels;
		    bs = (unsigned char *) pixels;
		    for (i = 0; i < (idata->nrows * idata->ncols); i++) {
			*nbs = (unsigned char) (
					  (RED_COMP * (float) (*(bs + 0))) +
				     (GREEN_COMP * (float) (*(bs + xbyy))) +
				  (BLUE_COMP * (float) (*(bs + (2 * xbyy))))
			    );
			nbs++;
			bs++;
		    }
		}
	    }
	    bs = (unsigned char *) pixels;
	    pixels = new_pixels;
	    free(bs);
	    /*
	     * This is a giant hack to compensate for the sun displays so the
	     * intensity data looks right.  We (really)  need to apply
	     * another transform here...and that will be done when more time
	     * exists...most likely for the September 15th
	     * trial....DEBeecher.
	     */
	    idata->smethod = WINDOW_LEVEL;
	    idata->window = 125;
	    idata->level = 25;

	    break;
	}
    }

/*
 * Get the scaling information (all optional)...and we don't use these for color.
 */
    if (idata->photointerp == NOT_IMPLEMENTED ||
	idata->photointerp == MONOCHROME1 ||
	idata->photointerp == MONOCHROME2) {
	ctx = NULL;
	cond1 = DCM_GetElementValue(&object, &intercept, &rtnLength, &ctx);
	if (cond1 == DCM_NORMAL)
	    Intercept[rtnLength] = '\0';
	ctx = NULL;
	cond2 = DCM_GetElementValue(&object, &slope, &rtnLength, &ctx);
	if (cond2 == DCM_NORMAL)
	    Slope[rtnLength] = '\0';
	if ((cond1 == DCM_NORMAL) && (cond2 == DCM_NORMAL)) {
	    idata->smethod = SLOPE_INTERCEPT;
	    sscanf(Slope, "%e", &(idata->m));
	    sscanf(Intercept, "%e", &(idata->b));
	    if (idata->m == 0.0)
		idata->m = 1.0;
	}
	idata->window = 0;
	idata->level = 0;
	if (overrideCenter == NO_OVERRIDE) {
	    ctx = NULL;
	    cond1 = DCM_GetElementValue(&object, &window_center, &rtnLength, &ctx);
	    if (cond1 == DCM_NORMAL) {
		WindowCenter[rtnLength] = '\0';
		sscanf(WindowCenter, "%e", &(idata->level));
	    }
	} else {
	    idata->level = overrideCenter;
	    cond1 = DCM_NORMAL;
	}

	if (overrideWidth == NO_OVERRIDE) {
	    ctx = NULL;
	    cond2 = DCM_GetElementValue(&object, &window_width, &rtnLength, &ctx);
	    if (cond2 == DCM_NORMAL) {
		WindowWidth[rtnLength] = '\0';
		sscanf(WindowWidth, "%e", &(idata->window));
	    }
	} else {
	    idata->window = overrideWidth;
	    cond2 = DCM_NORMAL;
	}
	if ((cond1 == DCM_NORMAL) && (cond2 == DCM_NORMAL)) {

	    if ((idata->smethod == SLOPE_INTERCEPT) &&
		(idata->level != 0) && (idata->window != 0))
		idata->smethod = S_I_W_L;
	    else if ((idata->level != 0) && (idata->window != 0))
		idata->smethod = WINDOW_LEVEL;
	}
	if (idata->smethod == DEFAULT)
	    COND_PushCondition(DICOMXDISP, "    Scaling Method: DEFAULT");
	else if (idata->smethod == WINDOW_LEVEL) {
	    COND_PushCondition(DICOMXDISP, "    Scaling Method: WINDOW/LEVEL");
	    COND_PushCondition(DICOMXDISP, "      Window/Level: %.2f %.2f",
			       idata->window, idata->level);
	} else if (idata->smethod == SLOPE_INTERCEPT) {
	    COND_PushCondition(DICOMXDISP, "    Scaling Method: SLOPE/INTERCEPT");
	    COND_PushCondition(DICOMXDISP, "   Slope/Intercept: %.2f %.2f",
			       idata->m, idata->b);
	} else {
	    COND_PushCondition(DICOMXDISP, "    Scaling Method: SLOPE/INTERCEPT-WINDOW/LEVEL");
	    COND_PushCondition(DICOMXDISP, "   Slope/Intercept: %.2f %.2f",
			       idata->m, idata->b);
	    COND_PushCondition(DICOMXDISP, "      Window/Level: %.2f %.2f",
			       idata->window, idata->level);
	}
    }
/*
 * Go get patient information (all optional)
 */
    ctx = NULL;
    if (DCM_GetElementValue(&object, &date, &rtnLength, &ctx) == DCM_NORMAL) {
	cptr = date.d.string;
	cptr[rtnLength] = '\0';
	strcpy(idata->date, cptr);
    } else
	strcpy(idata->date, "NODATE");

    ctx = NULL;
    if (DCM_GetElementValue(&object, &modality, &rtnLength, &ctx) == DCM_NORMAL) {
	cptr = modality.d.string;
	cptr[rtnLength] = '\0';
	strcpy(idata->modality, cptr);
    } else
	strcpy(idata->modality, "NOMODALITY");

    ctx = NULL;
    if (DCM_GetElementValue(&object, &name, &rtnLength, &ctx) == DCM_NORMAL) {
	cptr = name.d.string;
	cptr[rtnLength] = '\0';
	strcpy(idata->patname, cptr);
    } else
	strcpy(idata->patname, "NONAME");

    cond = DCM_CloseObject(&object);
    if (cond != DCM_NORMAL) {
    }
    return (pixels);
}


/*******************************************************************/
unsigned char *
ScaleImageData(void *data, ImageStruct * idata)
{
    int
        factor,
        new_nrow,
        new_ncol;
    short
       *new_data;
    int i,
        j,
        k;
    short
       *short_data;
    unsigned char
       *uc_data,
       *return_data;

    unsigned long
        nl_rows,
        nl_cols;
    short
        temp_ret;

    int
        expand_data,
        max,
        min,
        xmin,
        xmax;

    float
        m,
        b;			/* temps for slope and intercept	 */

    short
       *cs;
    char
       *bs;



/*
 * Figure out how much downsampling (or expansion) is needed...
 */
    factor = 1;
    expand_data = 1;
    new_nrow = idata->nrows;
    new_ncol = idata->ncols;
    while ((new_nrow > (G_display_height * MAX_PERCT)) ||
	   (new_ncol > (G_display_width * MAX_PERCT))) {
	factor = factor * 2;
	new_nrow = (int) (((float) new_nrow / 2.0) + 0.51);
	new_ncol = (int) (((float) new_ncol / 2.0) + 0.51);
	expand_data = 0;
    }
    if (expand_data) {		/* Try to expand the data set...(if needed) */
	factor = 1;
	expand_data = 0;
	new_nrow = idata->nrows;
	new_ncol = idata->ncols;
#if 0
	while (((new_nrow * 2) < (G_display_height * MAX_PERCT)) &&
	       ((new_ncol * 2) < (G_display_width * MAX_PERCT))) {
	    factor = factor * 2;
	    new_nrow = (int) ((float) new_nrow * 2.0);
	    new_ncol = (int) ((float) new_ncol * 2.0);
	    expand_data = 1;
	}
#endif
    }
    COND_PushCondition(DICOMXDISP, "     Original Size: %5d %5d",
		       idata->nrows, idata->ncols);
    COND_PushCondition(DICOMXDISP, "          New Size: %5d %5d",
		       new_nrow, new_ncol);

    if (idata->bitsallocated == 8) {
	temp_ret = pixelResize(data, (unsigned long) idata->nrows,
			       (unsigned long) idata->ncols, UNSIGNED_8BIT,
			 (unsigned long) new_nrow, (unsigned long) new_ncol,
		      (void **) &new_data, &nl_rows, &nl_cols, expand_data);
    } else {
	temp_ret = pixelResize(data, (unsigned long) idata->nrows,
			       (unsigned long) idata->ncols, SIGNED_16BIT,
			 (unsigned long) new_nrow, (unsigned long) new_ncol,
		      (void **) &new_data, &nl_rows, &nl_cols, expand_data);
    }
    if (temp_ret == 0) {
	idata->error_condition = TRUE;
	sprintf(idata->error_message, "Can't allocate memory for pixel resize");
	free(data);		/* Free the existing pixel data */
	idata->data = NULL;
	return ((unsigned char *) NULL);
    }
    new_nrow = nl_rows;
    new_ncol = nl_cols;

    if ((return_data = (unsigned char *)
	 malloc(new_nrow * new_ncol * sizeof(unsigned char))) == NULL) {
	idata->error_condition = TRUE;
	sprintf(idata->error_message, "Can't allocate memory for scaling");
	free(data);		/* Free the existing pixel data */
	free(new_data);		/* Free the new pixel data */
	idata->data = NULL;
	return ((unsigned char *) NULL);
    }
    idata->xpos = (G_display_width - new_ncol) / 2;
    idata->ypos = (G_display_height - new_nrow) / 2 - 10;

    idata->nrows = new_nrow;
    idata->ncols = new_ncol;
    free(data);
/*
 * We now have the a 16 bit representation in new_data, we need to examine the
 * scaling method and scale appropriately
 */
    switch (idata->smethod) {
    case DEFAULT:
    default:
	max = -65000;
	min = 65000;
	for (i = 0; i < new_nrow * new_ncol; i++) {
	    if (new_data[i] > max)
		max = new_data[i];
	    if (new_data[i] < min)
		min = new_data[i];
	}
	m = (float) (G_ncolors) / (max - min);
	b = (float) (G_ncolors) - (m * max);
	xmin = min;
	xmax = max;
	break;

    case SLOPE_INTERCEPT:
	m = (idata->m) / (256.0 / (G_ncolors));
	b = (idata->b) / (256.0 / (G_ncolors));
	xmin = (int) (-b / m);
	xmax = (int) (((G_ncolors) - b) / m);
	break;

    case WINDOW_LEVEL:
	m = (float) (G_ncolors) / (idata->window);
	b = (G_ncolors) - m * (idata->level + (idata->window / 2));
	xmin = (int) ((idata->level) - (idata->window) / 2);
	xmax = (int) ((idata->level) + (idata->window) / 2);
	break;

    case S_I_W_L:
	{
	    register long temp;
	    for (i = 0; i < new_nrow * new_ncol; i++) {
		temp = (long) ((idata->m) * new_data[i] + idata->b);
		if (temp <= -32768)
		    new_data[i] = -32768;
		else if (temp <= 32767)
		    new_data[i] = (short) temp;
		else
		    new_data[i] = 32767;
	    }
	    m = (float) (G_ncolors) / (idata->window);
	    b = (G_ncolors) - m * (idata->level + (idata->window / 2));
	    xmin = (int) (idata->level - idata->window / 2);
	    xmax = (int) (idata->level + idata->window / 2);
	}
	break;
    }

    /*
     * MONOCHROME1 and MONOCHROME2 are handled here...
     */
    if (idata->photointerp == MONOCHROME1) {	/* flip image to display
						 * white->black */
	short idx;
	for (i = 0; i < new_nrow * new_ncol; i++) {
	    if (new_data[i] < xmin)
		return_data[i] = graylut[G_ncolors - 1];
	    else if (new_data[i] > xmax)
		return_data[i] = graylut[0];
	    else {
		idx = (int) (((new_data[i] * m) + b) + 0.5);
		return_data[i] = graylut[(G_ncolors - 1) - idx];
	    }
	}
    } else {			/* Assume it is MONOCHROME2 or anything else
				 * for that matter... */
	for (i = 0; i < new_nrow * new_ncol; i++) {
		int y, z;
	    if (new_data[i] < xmin)
		return_data[i] = graylut[0];
	    else if (new_data[i] > xmax)
		return_data[i] = graylut[G_ncolors - 1];
	    else {
		return_data[i] = graylut[(int) (((new_data[i] * m) + b) + 0.5)];
		y = graylut[10];
		z = return_data[i];
		}
	}
    }

    free(new_data);
    idata->data = return_data;
    return (return_data);
}


short
pixelResize(void *pixels, unsigned long rows, unsigned long cols,
	    int type, unsigned long desiredRows, unsigned long desiredCols,
	    void **pixout, unsigned long *newRows,
	    unsigned long *newCols, int expand_data)
{
    unsigned long
        factorRows,
        factorCols,
        factor;
    short
        cond;

    if (!expand_data) {
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
    } else {
	factorRows = desiredRows / rows;
	factorCols = desiredCols / cols;
	factor = (factorRows < factorCols) ? factorRows : factorCols;
    }


    switch (type) {
    case SIGNED_16BIT:
	if (expand_data)
	    cond = pixelexp16signed((short *) pixels, rows, cols, factor,
				    (short **) pixout, newRows, newCols);
	else
	    cond = pixelavg16signed((short *) pixels, rows, cols, factor,
				    (short **) pixout, newRows, newCols);
	break;
    case UNSIGNED_8BIT:
	if (expand_data)
	    cond = pixelexp8unsigned((unsigned char *) pixels, rows, cols, factor,
				     (short **) pixout, newRows, newCols);
	else
	    cond = pixelavg8unsigned((unsigned char *) pixels, rows, cols, factor,
				     (short **) pixout, newRows, newCols);
	break;
    default:
	printf("Unimplemented type %d\n", type);
	exit(1);
    }
    return cond;
}
static short
pixelexp16signed(short *pixels, unsigned long rows,
		 unsigned long cols, unsigned long factor,
	     short **pixout, unsigned long *newRows, unsigned long *newCols)
{
    unsigned long
        i,
        j,
        k,
        l,
        bigi;
    short
       *p,
        t;
    unsigned long
        r,
        c;

    r = rows * factor;
    c = cols * factor;

    p = malloc(r * c * sizeof(short));
    if (p == NULL)
	return (short) 0;

    *pixout = p;
    for (i = 0; i < rows; i++) {
	for (j = 0; j < cols; j++) {
	    t = *(pixels + (i * cols) + j);
	    /* Perform the pixel replication here... */
	    bigi = ((i * factor) * c) + (j * factor);
	    for (k = bigi; k < (bigi + factor); k++) {
		for (l = 0; l < factor; l++) {
		    p[k + (l * c)] = t;
		}
	    }
	}
    }
    *newRows = rows * factor;
    *newCols = cols * factor;
    return (short) 1;
}
static short
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
	return (short) 0;

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
    return (short) 1;
}
static short
pixelexp8unsigned(unsigned char *pixels, unsigned long rows,
		  unsigned long cols, unsigned long factor,
	     short **pixout, unsigned long *newRows, unsigned long *newCols)
{
    unsigned long
        i,
        j,
        k,
        l,
        bigi;
    short
       *p;
    unsigned char
        t;
    unsigned long
        r,
        c;

    r = rows * factor;
    c = cols * factor;

    p = malloc(r * c * sizeof(short));
    if (p == NULL)
	return (short) 0;

    *pixout = p;
    for (i = 0; i < rows; i++) {
	for (j = 0; j < cols; j++) {
	    t = *(pixels + (i * cols) + j);
	    /* Perform the pixel replication here... */
	    bigi = ((i * factor) * c) + (j * factor);
	    for (k = bigi; k < (bigi + factor); k++) {
		for (l = 0; l < factor; l++) {
		    p[k + (l * c)] = (short) t;
		}
	    }
	}
    }
    *newRows = rows * factor;
    *newCols = cols * factor;
    return (short) 1;
}
static short
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
	return (short) 0;

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
    return (short) 1;
}

