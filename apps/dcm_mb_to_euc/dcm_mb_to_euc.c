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
** Author, Date:	Stephen M. Moore, 9-May-93
** Intent:		This program reads DICOM Images and validates
**			character encodings (for multibyte character sets).
** Usage:
**			dcm_mb_to_euc [-b] [-g] [-v] [-z] file [file ...]
** Last Update:		$Author: smm $, $Date: 1999-12-23 16:58:22 $
** Source File:		$RCSfile: dcm_mb_to_euc.c,v $
** Revision:		$Revision: 1.1 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.1 $ $RCSfile: dcm_mb_to_euc.c,v $";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef GCCSUNOS
#include <sys/types.h>
#endif
#ifdef MALLOC_DEBUG
#include "malloc.h"
#endif
#include "dicom.h"
#include "ctnthread.h"
#include "lst.h"
#include "condition.h"
#include "dicom_objects.h"
#include "dicom_chr.h"


static void
usageerror()
{
    char msg[] = "\
Usage: dcm_mb_to_euc [-e <code>] [-t] [-v] file [file ...]\n\
\n\
    -e <code>  Indicate which character encoding to use.  Supported values:\n\
                eucjp\n\
                eucjpromaji\n\
    -t        Part 10 file\n\
    -v        Place DCM facility in verbose mode\n\
\n\
    file      One or more information objects to be dumped\n";

    fprintf(stderr, msg);
    exit(1);
}

static char* getString(DCM_OBJECT** o, DCM_TAG t)
{
  DCM_ELEMENT e;
  CONDITION cond;
  char* s;

  e.tag = t;
  cond = DCM_GetElement(o, t, &e);
  if (cond != DCM_NORMAL) {
    COND_PopCondition(TRUE);
    return 0;
  }
  s = malloc(e.length + 1);
  e.d.string = s;
  cond = DCM_ParseObject(o, &e, 1, 0, 0, 0);
  if (cond != DCM_NORMAL) {
    COND_DumpConditions();
    free(s);
    exit(1);
  }

  return s;
}

typedef struct {
  char* c1;
  char* c2;
} STRING_MATRIX;

static int validateSpecificCharSet(const char* specificCharSet,
				   const char* group)
{
  char imageCharSet[1024];
  int index;
  int flag;

  STRING_MATRIX firstElement[] = {
    { "eucjp", "" },
    { "eucjp", "ISO_IR 100"},
    { "eucjp", "ISO_IR 100 "},
    { "eucjp", "ISO 2022 IR 6"},
    { "eucjp", "ISO 2022 IR 6 "},
    { "eucjp", "ISO 2022 IR 100"},
    { "eucjp", "ISO 2022 IR 100 "},
    { "eucjpromaji", "ISO_IR 13"},
    { "eucjpromaji", "ISO_IR 13 "},
    { "eucjpromaji", "ISO 2022 IR 13"},
    { "eucjpromaji", "ISO 2022 IR 13 "}
  };

  STRING_MATRIX secondaryElements[] = {
    { "eucjp", "ISO 2022 IR 87"},
    { "eucjp", "ISO 2022 IR 87 "},
    { "eucjp", "ISO 2022 IR 159"},
    { "eucjp", "ISO 2022 IR 159 "},
    { "eucjpromaji", "ISO 2022 IR 87"},
    { "eucjpromaji", "ISO 2022 IR 87 "},
    { "eucjpromaji", "ISO 2022 IR 159"},
    { "eucjpromaji", "ISO 2022 IR 159 "}
  };

  char* tok1;
  char* tok2;

  strcpy(imageCharSet, specificCharSet);

  if (imageCharSet[0] == '\\') {
    tok1 = "";
    tok2 = strtok(imageCharSet, "\\");
  } else if (imageCharSet[0] == '\0') {
    tok1 = "";
    tok2 = 0;
  } else {
    tok1 = strtok(imageCharSet, "\\");
    tok2 = strtok(NULL, "\\");
  }

  for (flag = 0, index = 0;
       index < (int)DIM_OF(firstElement) && flag == 0;
       index++) {
    if ((strcmp(firstElement[index].c1, group) == 0) &&
	(strcmp(firstElement[index].c2, tok1) == 0)) {
      flag = 1;
    }
  }

  if (flag == 0) {
    fprintf(stderr,
	    "MBCTN does not support group (%s) and specific character set (%s)\n",
	    group,
	    specificCharSet);
    exit(1);
  }
}

typedef struct {
  char* c1;
  CHR_ENCODING code;
} CODE_MAP;


CHR_ENCODING translateGroup(const char* group)
{
  CODE_MAP m[] = {
    { "eucjp", CHR_EUC_JP },
    { "eucjpromaji", CHR_EUC_JPROMAJI}
  };

  int index;

  for (index = 0; index < (int)DIM_OF(m); index++) {
    if (strcmp(group, m[index].c1) == 0)
      return m[index].code;
  }

  fprintf(stderr, "Could not translate group (%s) to a supported encoding\n",
	  group);
  exit(1);
}

static CONDITION callback1(const DCM_ELEMENT* e, void* ctx)
{
  CHR_ENCODING* coding;
  CONDITION cond;
  char xlate[1024];
  coding = (CHR_ENCODING*)ctx;

  if ((e->representation == DCM_CS) ||
	(e->representation == DCM_LO) ||
	(e->representation == DCM_ST) ||
	(e->representation == DCM_LT) ||
	(e->representation == DCM_PN) )
  {
    int length = 0;
    cond = CHR_Encode(e->d.string, e->length, CHR_ISO2022JP,
			xlate, sizeof(xlate), *coding,
			&length);

    if (cond != CHR_NORMAL)
      fprintf(stderr, "Failed to encode for: %08x\n", e->tag);
    printf("%04x %04x %32s %s\n",
	   DCM_TAG_GROUP(e->tag),
	   DCM_TAG_ELEMENT(e->tag),
	   e->description,
	   xlate);
  }
  return DCM_NORMAL;
}

static CONDITION printTranslated(DCM_OBJECT** object, const char* group)
{
  char* specificCharSet;
  CHR_ENCODING coding;
  CONDITION cond;
  CONDITION rtnCond = DCM_NORMAL;

  specificCharSet = getString(object, DCM_IDSPECIFICCHARACTER);

  validateSpecificCharSet(specificCharSet, group);
  coding = translateGroup(group);

  DCM_ScanParseObject(object, 0, 0, 0, 0, callback1, &coding);

  return rtnCond;
}

main(int argc, char **argv)
{
  DCM_OBJECT * object;
  CONDITION cond;
  CTNBOOLEAN verbose = FALSE,
    exitFlag = FALSE;
  unsigned long options = DCM_ORDERLITTLEENDIAN;
  char* group = "eucjp";

  while (--argc > 0 && (*++argv)[0] == '-') {
    switch (*(argv[0] + 1)) {
    case 'b':
      options &= ~DCM_ORDERMASK;
      options |= DCM_ORDERBIGENDIAN;
      break;
    case 'e':
      argc--; argv++;
      group = *argv;
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

  THR_Init();
  if (argc < 1)
    usageerror();

  while (argc-- > 0) {
    printf("DICOM File: %s\n", *argv);
    cond = DCM_OpenFile(*argv, options, &object);
    if (cond != DCM_NORMAL && ((options & DCM_PART10FILE) == 0)) {
      COND_DumpConditions();
      (void) DCM_CloseObject(&object);
      (void) COND_PopCondition(TRUE);
      fprintf(stderr, "Could not open %s as expected.  Trying Part 10 format.\n", *argv);
      cond = DCM_OpenFile(*argv, options | DCM_PART10FILE, &object);
    }
    if (cond == DCM_NORMAL) {
      printTranslated(&object, group);
    }
    COND_DumpConditions();
    (void) DCM_CloseObject(&object);
    (void) COND_PopCondition(TRUE);

    if (cond != DCM_NORMAL && exitFlag) {
      THR_Shutdown();
      exit(1);
    }
    argv++;
  }

  THR_Shutdown();
  return 0;
}

