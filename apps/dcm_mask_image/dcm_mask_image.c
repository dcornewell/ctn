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
** Author, Date:	Stephen M. Moore, 22-May-2001
** Intent:		This program reads a DICOM image and masks the image
**			with a pattern specified by the user.
** Last Update:		$Author: smm $, $Date: 2001-06-20 21:12:01 $
** Source File:		$RCSfile: dcm_mask_image.c,v $
** Revision:		$Revision: 1.4 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.4 $ $RCSfile: dcm_mask_image.c,v $";
/*
**   Usage:
**	dcm_mask_image [-p pattern] input output
**
**  Author:
**	Stephen M. Moore (smm@wuerl.wustl.edu)
**	Electronic Radiology Lab
**	Mallinckrodt Institute of Radiology
**	Washington University School of Medicine
**	22-May-2002
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
usageerror()
{
  char msg[] = "\
Usage: dcm_mask_image [-p pattern] [-x value] input output\n\
  -p  Pattern is one of left, right, top, bottom\n\
  -x  Set the value to replace pixels; default is 0 \n";

  fprintf(stderr, msg);
  exit(1);
}

typedef struct {
  unsigned short bitsAllocated;
  unsigned short bitsStored;
  unsigned short highBit;
  unsigned short pixelRepresentation;
  unsigned short samplesPerPixel;
  unsigned short rows;
  unsigned short columns;
  char photometricInterpretation[DICOM_CS_LENGTH + 1];
} PIXEL_PARAMS;

static void*
getPixels(DCM_OBJECT** obj, PIXEL_PARAMS* params)
{
  U32
    pixelLength,
    elementLength,
    frameLength;
  void *pixels;
  int pixelCount;
  void *ctx;

  DCM_ELEMENT
    p2 = {
	DCM_PXLPIXELDATA, DCM_OW, "", 1, 0, NULL
    };
  CONDITION cond = 0;

  cond = DCM_GetElementSize(obj, p2.tag, &pixelLength);
  if (cond != DCM_NORMAL) {
    fprintf(stderr, "Error finding length of pixel data\n");
    COND_DumpConditions();
    THR_Shutdown();
    exit(1);
  }
  frameLength = ((U32) params->samplesPerPixel) *
		((U32) params->rows) *
		((U32) params->columns) *
		((U32) (params->bitsAllocated) / 8);
  if (pixelLength != frameLength) {
    fprintf(stderr, "Computed pixel length differs from actual: %d %d\n",
		pixelLength, frameLength);
    THR_Shutdown();
    exit(2);
  }
  pixels = malloc(pixelLength);
  if (pixels == NULL) {
    perror("Malloc of pixel data");
    THR_Shutdown();
    exit(1);
  }
  pixelCount = (int) params->rows * (int) params->columns;
  p2.length = pixelLength;
  p2.d.ot = pixels;
  ctx = NULL;
  if (DCM_GetElementValue(obj, &p2, &elementLength, &ctx) != DCM_NORMAL) {
    fprintf(stderr, "Error retrieving pixels from image\n");
    COND_DumpConditions();
    THR_Shutdown();
    exit(1);
  }
  return pixels;
}

static 
maskLeft8(void* p, int value, PIXEL_PARAMS* params)
{
  unsigned char* newP = 0;

  int r = 0;
  int c = 0;

  newP = (unsigned char*)p;

  for (r = 0; r < params->rows; r++) {
    for (c = params->columns/2; c < params->columns; c++) {
      newP[(r*params->columns) + c] = (unsigned char)value;
    }
  }
}

static 
maskLeft16(void* p, int value, PIXEL_PARAMS* params)
{
  U16* newP = 0;

  int r = 0;
  int c = 0;

  newP = (U16*)p;

  for (r = 0; r < params->rows; r++) {
    for (c = params->columns/2; c < params->columns; c++) {
      newP[(r*params->columns) + c] = value;
    }
  }
}

static 
maskRight8(void* p, int value, PIXEL_PARAMS* params)
{
  unsigned char* newP = 0;

  int r = 0;
  int c = 0;

  newP = (unsigned char*)p;

  for (r = 0; r < params->rows; r++) {
    for (c = 0; c < params->columns/2; c++) {
      newP[(r*params->columns) + c] = (unsigned char)value;
    }
  }
}

static 
maskRight16(void* p, int value, PIXEL_PARAMS* params)
{
  U16* newP = 0;

  int r = 0;
  int c = 0;

  newP = (U16*)p;

  for (r = 0; r < params->rows; r++) {
    for (c = 0; c < params->columns/2; c++) {
      newP[(r*params->columns) + c] = value;
    }
  }
}

static 
maskTop8(void* p, int value, PIXEL_PARAMS* params)
{
  unsigned char* newP = 0;

  int r = 0;
  int c = 0;

  newP = (unsigned char*)p;

  for (r = params->rows/2; r < params->rows; r++) {
    for (c = 0; c < params->columns; c++) {
      newP[(r*params->columns) + c] = (unsigned char)value;
    }
  }
}

static 
maskTop16(void* p, int value, PIXEL_PARAMS* params)
{
  U16* newP = 0;

  int r = 0;
  int c = 0;

  newP = (U16*)p;

  for (r = params->rows/2; r < params->rows; r++) {
    for (c = 0; c < params->columns; c++) {
      newP[(r*params->columns) + c] = value;
    }
  }
}

static 
maskBottom8(void* p, int value, PIXEL_PARAMS* params)
{
  unsigned char* newP = 0;

  int r = 0;
  int c = 0;

  newP = (unsigned char*)p;

  for (r = 0; r < params->rows/2; r++) {
    for (c = 0; c < params->columns; c++) {
      newP[(r*params->columns) + c] = (unsigned char)value;
    }
  }
}

static 
maskBottom16(void* p, int value, PIXEL_PARAMS* params)
{
  U16* newP = 0;

  int r = 0;
  int c = 0;

  newP = (U16*)p;

  for (r = 0; r < params->rows/2; r++) {
    for (c = 0; c < params->columns; c++) {
      newP[(r*params->columns) + c] = value;
    }
  }
}

static
maskLeft(void* p, int value, PIXEL_PARAMS* params)
{
  if (params->bitsAllocated == 8)
    maskLeft8(p, value, params);
  if (params->bitsAllocated == 16)
    maskLeft16(p, value, params);
}

static
maskRight(void* p, int value, PIXEL_PARAMS* params)
{
  if (params->bitsAllocated == 8)
    maskRight8(p, value, params);
  if (params->bitsAllocated == 16)
    maskRight16(p, value, params);
}

static
maskTop(void* p, int value, PIXEL_PARAMS* params)
{
  if (params->bitsAllocated == 8)
    maskTop8(p, value, params);
  if (params->bitsAllocated == 16)
    maskTop16(p, value, params);
}

static
maskBottom(void* p, int value, PIXEL_PARAMS* params)
{
  if (params->bitsAllocated == 8)
    maskBottom8(p, value, params);
  if (params->bitsAllocated == 16)
    maskBottom16(p, value, params);
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
        options = DCM_ORDERLITTLEENDIAN;
  U32 pixelLength = 0;
  unsigned char
    *pixels;
  char* pattern = "";
  char* value = "0";
  int valueInteger = 0;

  static PIXEL_PARAMS params;
  DCM_ELEMENT
    p2 = {
	DCM_PXLPIXELDATA, DCM_OW, "", 1, 0, NULL
    };

  static DCM_ELEMENT list[] = {
	{DCM_IMGBITSALLOCATED, DCM_US, "",
	1, sizeof(params.bitsAllocated), (void *) &params.bitsAllocated},
	{DCM_IMGBITSSTORED, DCM_US, "",
	1, sizeof(params.bitsStored), (void *) &params.bitsStored},
	{DCM_IMGHIGHBIT, DCM_US, "",
	1, sizeof(params.highBit), (void *) &params.highBit},
	{DCM_IMGPIXELREPRESENTATION, DCM_US, "",
	1, sizeof(params.pixelRepresentation), (void *) &params.pixelRepresentation},
	{DCM_IMGSAMPLESPERPIXEL, DCM_US, "",
	1, sizeof(params.samplesPerPixel), (void *) &params.samplesPerPixel},
	{DCM_IMGROWS, DCM_US, "",
	1, sizeof(params.rows), (void *) &params.rows},
	{DCM_IMGCOLUMNS, DCM_US, "",
	1, sizeof(params.columns), (void *) &params.columns},
	{DCM_IMGPHOTOMETRICINTERP, DCM_CS, "",
	1, sizeof(params.photometricInterpretation), params.photometricInterpretation}
  };

  while (--argc > 0 && (*++argv)[0] == '-') {
    switch (*(argv[0] + 1)) {
    case 'p':
      argc--; argv++;
      if (argc < 1)
	usageerror();

      pattern = *argv;
      break;
    case 't':
      options &= ~DCM_FILEFORMATMASK;
      options |= DCM_PART10FILE;
      break;
    case 'v':
      verbose = TRUE;
      break;
    case 'x':
      argc--; argv++;
      if (argc < 1)
	usageerror();

      value = *argv;
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
    options &= ~DCM_FILEFORMATMASK;
    options |= DCM_PART10FILE;
    cond = DCM_OpenFile(*argv++, options, &object);
    if (cond != DCM_NORMAL) {
      COND_DumpConditions();
      THR_Shutdown();
      exit(1);
    }
  }

  cond = DCM_ParseObject(&object, list, (int) DIM_OF(list), NULL, 0, NULL);
  if (cond != DCM_NORMAL) {
    fprintf(stderr, "Error retrieving bits allocated data element\n");
    COND_DumpConditions();
    THR_Shutdown();
    exit(1);
  }
  pixels = getPixels(&object, &params);

  valueInteger = atoi(value);

  if (strcmp(pattern, "left") == 0) {
    maskLeft(pixels, valueInteger, &params);
  } else if (strcmp(pattern, "right") == 0) {
    maskRight(pixels, valueInteger, &params);
  } else if (strcmp(pattern, "top") == 0) {
    maskTop(pixels, valueInteger, &params);
  } else if (strcmp(pattern, "bottom") == 0) {
    maskBottom(pixels, valueInteger, &params);
  } else {
    fprintf(stderr, "Could not determine pattern for masking image\n");
    exit(1);
  }


  cond = DCM_GetElementSize(&object, p2.tag, &pixelLength);

  if (DCM_RemoveElement(&object, DCM_PXLPIXELDATA) != DCM_NORMAL) {
    fprintf(stderr, "Error removing old pixel data from object\n");
    COND_DumpConditions();
    THR_Shutdown();
    exit(1);
  }

  p2.length = pixelLength;

  p2.d.ot = pixels;
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
  return 0;
}
