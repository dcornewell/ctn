/*
          Copyright (C) 2001 RSNA and Washington University

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
**                              DICOM 93
**                   Electronic Radiology Laboratory
**                 Mallinckrodt Institute of Radiology
**              Washington University School of Medicine
**
** Module Name(s):      main
**                      usageerror
** Author, Date:        Stephen M. Moore, 13-Feb-2001
** Intent:		This program creates test images with specific pixel patterns
**			to test Transfer syntaxes.
**   Usage:
**	dcm_vr_tests [-t] <test> <output file>
**   Options:
**	t	Write art 10 file
**   Descriptions:
**	test		Which pattern to create (0, 1)
**	output file	Binary DICOM file
**
** Last Update:         $Author: smm $, $Date: 2001-02-19 15:47:37 $
** Source File:         $RCSfile: dcm_vr_patterns.c,v $
** Revision:            $Revision: 1.2 $
** Status:              $State: Exp $
*/

static char rcsid[] = "$Revision: 1.2 $ $RCSfile: dcm_vr_patterns.c,v $";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dicom.h"
#include "ctnthread.h"
#include "lst.h"
#include "condition.h"
#include "dicom_objects.h"
#include "dicom_uids.h"


static void addTextElement(DCM_OBJECT** object, DCM_TAG tag,
			   const char *f)
{
  DCM_ELEMENT e;
  CONDITION cond;
  char txt[1024];

  e.tag = tag;
  DCM_LookupElement(&e);

  strcpy(txt, f);
  e.length = strlen(txt);
  e.d.ot = txt;
  cond = DCM_AddElement(object, &e);
  if (cond != DCM_NORMAL) {
    COND_DumpConditions();
    exit(2);
  }
}

static void
usageerror()
{
    char msg[] = "\
Usage: dcm_vr_tests <test (0, 1)> <output file>\n\
\n\
   test            0 -> 8 bit data, 1-> 16 bit data\n\
   output file     Binary DICOM file\n";

    fprintf(stderr, msg);
    exit(1);
}

static void
addPatientModule(DCM_OBJECT* obj, const char* name)
{
  addTextElement(&obj, DCM_PATNAME, name);
  addTextElement(&obj, DCM_PATID, "");
  addTextElement(&obj, DCM_PATBIRTHDATE, "");
  addTextElement(&obj, DCM_PATSEX, "");
}

#define UID_BASE "1.2.840.113654.2.3.1995.3.0.0"

static void
addGeneralStudyModule(DCM_OBJECT* obj)
{
  addTextElement(&obj, DCM_RELSTUDYINSTANCEUID, (UID_BASE  ".1"));
  addTextElement(&obj, DCM_IDSTUDYDATE, "20010213");
  addTextElement(&obj, DCM_IDSTUDYTIME, "1235");
  addTextElement(&obj, DCM_IDREFERRINGPHYSICIAN, "Referring^The");
  addTextElement(&obj, DCM_RELSTUDYID, "STUDYID");
  addTextElement(&obj, DCM_IDACCESSIONNUMBER, "ACCESSION");
}

static void
addGeneralSeriesModule(DCM_OBJECT* obj)
{
  addTextElement(&obj, DCM_IDMODALITY, "OT");
  addTextElement(&obj, DCM_RELSERIESINSTANCEUID, (UID_BASE  ".1.1"));
  addTextElement(&obj, DCM_RELSERIESNUMBER, "1");
}

static void
addSCEquipmentModule(DCM_OBJECT* obj)
{
  addTextElement(&obj, DCM_IDCONVERSIONTYPE, "WSD");
}

static void
addSOPCommonModule(DCM_OBJECT* obj, const char* sopInstanceUID)
{
  addTextElement(&obj, DCM_IDSOPCLASSUID, DICOM_SOPCLASSSECONDARYCAPTURE);
  addTextElement(&obj, DCM_IDSOPINSTANCEUID, sopInstanceUID);
}

static void
addGeneralImageModule(DCM_OBJECT* obj, const char* imageNumber)
{
  addTextElement(&obj, DCM_RELIMAGENUMBER, imageNumber);
}

static void
addImagePixelModule(DCM_OBJECT* obj, const char* imageNumber, U16 bitsAll, U16 bitsStored,
	U16 highBit, U16 pixelRep)
{
  CONDITION cond;
  int idx;

  static U16 samplesPerPixel = 1;
  static U16 rows = 4;
  static U16 cols = 8;
  static U16 myBitsAllocated = 0;
  static U16 myBitsStored = 0;
  static U16 myHighBit = 0;
  static U16 myPixelRep = 0;

  DCM_ELEMENT e[] = {
    { DCM_IMGSAMPLESPERPIXEL, DCM_US, "", 0, sizeof(samplesPerPixel), (void*)&samplesPerPixel },
    { DCM_IMGROWS, DCM_US, "", 0, sizeof(rows), (void*)&rows },
    { DCM_IMGCOLUMNS, DCM_US, "", 0, sizeof(cols), (void*)&cols },
    { DCM_IMGBITSALLOCATED, DCM_US, "", 0, sizeof(myBitsAllocated), (void*)&myBitsAllocated },
    { DCM_IMGBITSSTORED, DCM_US, "", 0, sizeof(myBitsStored), (void*)&myBitsStored },
    { DCM_IMGHIGHBIT, DCM_US, "", 0, sizeof(myHighBit), (void*)&myHighBit },
    { DCM_IMGPIXELREPRESENTATION, DCM_US, "", 0, sizeof(myPixelRep), (void*)&myPixelRep }

  };

  myBitsAllocated = bitsAll;
  myBitsStored = bitsStored;
  myHighBit = highBit;
  myPixelRep = pixelRep;

  for (idx = 0; idx < (int)DIM_OF(e); idx++) {
    cond = DCM_AddElement(&obj, &e[idx]);
    if (cond != DCM_NORMAL) {
      COND_DumpConditions();
      exit(2);
    }
  }

  addTextElement(&obj, DCM_IMGPHOTOMETRICINTERP, "MONOCHROME2");
}

static void
add8BitPixels(DCM_OBJECT* obj, void* pixels, int length)
{
  CONDITION cond;

  DCM_ELEMENT e = { DCM_PXLPIXELDATA, DCM_OB, "", 0, 0, 0 };

  e.length = length;
  e.d.ot = pixels;

  cond = DCM_AddElement(&obj, &e);
  if (cond != DCM_NORMAL) {
    COND_DumpConditions();
    exit(2);
  }
}

static void
add16BitPixels(DCM_OBJECT* obj, void* pixels, int length)
{
  CONDITION cond;

  DCM_ELEMENT e = { DCM_PXLPIXELDATA, DCM_OW, "", 0, 0, 0 };

  e.length = length;
  e.d.ot = pixels;

  cond = DCM_AddElement(&obj, &e);
  if (cond != DCM_NORMAL) {
    COND_DumpConditions();
    exit(2);
  }
}

static DCM_OBJECT* test0()
{
  DCM_OBJECT* obj;
  CONDITION cond;

  cond = DCM_CreateObject(&obj, 0);

  addPatientModule(obj, "PIXELS^EIGHTBIT");
  addGeneralStudyModule(obj);
  addGeneralSeriesModule(obj);
  addSCEquipmentModule(obj);
  addSOPCommonModule(obj, (UID_BASE  ".1.1.8"));
  addGeneralImageModule(obj, "8");

  addImagePixelModule(obj, "8", 8, 8, 7, 0);

  add8BitPixels(obj, "ABCDEFGH" "IJKLMNOP" "QRSTUVWX" "YZ012345", 32);

  return obj;
}

static DCM_OBJECT* test1()
{
  DCM_OBJECT* obj;
  CONDITION cond;
  U16 pixels[32];
  int idx;

  cond = DCM_CreateObject(&obj, 0);

  addPatientModule(obj, "PIXELS^SIXTEENBIT");
  addGeneralStudyModule(obj);
  addGeneralSeriesModule(obj);
  addSCEquipmentModule(obj);
  addSOPCommonModule(obj, (UID_BASE  ".1.1.16"));
  addGeneralImageModule(obj, "16");

  addImagePixelModule(obj, "16", 16, 16, 15, 0);

  for (idx = 0; idx < 26; idx++)
    pixels[idx] = 'A' + idx;

  for (idx = 0; idx < 6; idx++)
    pixels[idx+26] = 'a' + idx;


  add16BitPixels(obj, pixels, 64);

  return obj;
}

static void
addFileMeta(DCM_OBJECT* obj)
{
  DCM_FILE_META* fileMeta = 0;
  CONDITION cond;

  cond = DCM_DefaultFileMeta(&obj, &fileMeta);
  if (cond != DCM_NORMAL)
    goto abort;

  strcpy(fileMeta->transferSyntaxUID, DICOM_TRANSFERLITTLEENDIAN);

  cond = DCM_SetFileMeta(&obj, fileMeta);
  if (cond != DCM_NORMAL)
    goto abort;

  cond = DCM_FreeFileMeta(&fileMeta);
  if (cond != DCM_NORMAL)
    goto abort;

  return;

abort:
  fprintf(stderr, "Unable to complete File Meta\n");
  exit(1);
}

static void evaluate8Bit(DCM_OBJECT* obj)
{
  CONDITION cond;
  static unsigned char pixels[33];
  void* ctx = 0;
  U32 length = 0;

  DCM_ELEMENT e =
	{ DCM_PXLPIXELDATA, DCM_OB, "", 0, sizeof(pixels), (void*)pixels };

  cond = DCM_GetElementValue(&obj, &e, &length, &ctx);
  if (cond != DCM_NORMAL) {
    COND_DumpConditions();
    exit(3);
  }
  if (length != 32) {
    fprintf(stderr, "Expected 32 bytes, retrieved %d bytes\n", length);
    exit(3);
  }

  if (strcmp(pixels, ("ABCDEFGH" "IJKLMNOP" "QRSTUVWX" "YZ012345")) != 0) {
    fprintf(stderr, "Error in retrieving 8 bit pixels\n");
    fprintf(stderr, "Expected: %s\n",
	    ("ABCDEFGH" "IJKLMNOP" "QRSTUVWX" "YZ012345"));
    pixels[32] = '\0';
    fprintf(stderr, "Received: %s\n", pixels);
  }
}

static void evaluate16Bit(DCM_OBJECT* obj)
{
  CONDITION cond;
  static U16 pixels[33];
  void* ctx = 0;
  U32 length = 0;
  int idx = 0;
  int errorFlag = 0;

  DCM_ELEMENT e =
	{ DCM_PXLPIXELDATA, DCM_OW, "", 0, sizeof(pixels), (void*)pixels };

  cond = DCM_GetElementValue(&obj, &e, &length, &ctx);
  if (cond != DCM_NORMAL) {
    COND_DumpConditions();
    exit(3);
  }
  if (length != 64) {
    fprintf(stderr, "Expected 64 bytes, retrieved %d bytes\n", length);
    exit(3);
  }

  for (idx = 0; idx < 26; idx++) {
    U16 expectedValue;

    expectedValue = idx + 'A';
    if (pixels[idx] != expectedValue) {
      fprintf(stderr, "Error in retrieving 16 bit pixels\n");
      fprintf(stderr, "At index: %d, expected %02x, received %02x \n",
		idx, expectedValue, pixels[idx]);
      errorFlag = 1;
    }
  }

  for (idx = 0; idx < 6; idx++) {
    U16 expectedValue;

    expectedValue = idx + 'a';
    if (pixels[idx+26] != expectedValue) {
      fprintf(stderr, "Error in retrieving 16 bit pixels\n");
      fprintf(stderr, "At index: %d, expected %02x, received %02x \n",
		idx+26, expectedValue, pixels[idx+26]);
      errorFlag = 1;
    }
  }

  if (errorFlag)
    exit(3);
}

int
main(int argc, char **argv)
{
  DCM_OBJECT * object;
  CONDITION cond;
  char* elementFile = "";

  unsigned long options = DCM_ORDERLITTLEENDIAN | DCM_PART10FILE;
  int validation = 0;

  while (--argc > 0 && (*++argv)[0] == '-') {
    switch (*(argv[0] + 1)) {
    case 'x':
      validation = 1;
      break;
    default:
      break;
    }
  }

  if (argc < 2)
    usageerror();

  THR_Init();

  if (validation == 0) {

    if (strcmp(*argv, "0") == 0)
      object = test0();
    else
      object = test1();

    addFileMeta(object);

    elementFile = *++argv;

    cond = DCM_WriteFile(&object, options, elementFile);
    if (cond != DCM_NORMAL) {
      COND_DumpConditions();
      THR_Shutdown();
      return 1;
    }

    (void) DCM_CloseObject(&object);
  } else {

    cond = DCM_OpenFile(argv[1], DCM_PART10FILE, &object);
    if (cond != DCM_NORMAL) {
      COND_DumpConditions();
      THR_Shutdown();
      return 1;
    }
    if (strcmp(*argv, "0") == 0)
      evaluate8Bit(object);
    else
      evaluate16Bit(object);

    (void) DCM_CloseObject(&object);
  }
  THR_Shutdown();
  return 0;
}

