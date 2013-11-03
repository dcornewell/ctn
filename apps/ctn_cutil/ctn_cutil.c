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
** Intent:		This program uses the DICOM OBJECTS package to open
**			DICOM files and dump their contents to stdout.  Each
**			argument to the program is expected to be the name
**			of a file containing a DICOM stream.
**   Usage:
**			dcm_dump_file [-b] [-g] [-v] [-z] file [file ...]
** Last Update:		$Author: smm $, $Date: 1999-11-22 17:00:13 $
** Source File:		$RCSfile: ctn_cutil.c,v $
** Revision:		$Revision: 1.1 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.1 $ $RCSfile: ctn_cutil.c,v $";

#include <stdio.h>
#include <stdlib.h>
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
Usage: ctn_cutil <input format> <output format>\n\
\n\
    formats are one of:\n\
              ISO2022JP\n\
              ISOIR13\n\
              EUCJP\n\
              EUCJPROMAJI\n\
              ROMAJI\n
              ISO2022KR\n\
              EUCKR\n";

    fprintf(stderr, msg);
    exit(1);
}

typedef struct {
  const char* codeName;
  CHR_ENCODING codeValue;
} CODE_MAP;

CHR_ENCODING mapCodeName(const char* n)
{
  CODE_MAP lMap[] = {
    { "ISO2022JP", CHR_ISO2022JP },
    { "ISOIR13", CHR_DEFAULTISOIR13},
    { "EUCJP", CHR_EUC_JP},
    { "EUCJPROMAJI", CHR_EUC_JPROMAJI},
    { "ROMAJI", CHR_EUC_JPROMAJI},
    { "ISO2022KR", CHR_ISO2022KR},
    { "EUCKR", CHR_EUC_KR }
  };

  int i;

  for (i = 0; i < (int)DIM_OF(lMap); i++) {
    if (strcmp(n, lMap[i].codeName) == 0)
      return lMap[i].codeValue;
  }
  fprintf(stderr, "Could not determine a code set for: %s\n", n);
  exit(1);
}

static void translate(const char* inputCode, const char* outputCode)
{
  char x1[10240] = "";
  char x2[10240] = "";
  int x1Length = 0;
  int x2Length = 0;

  CHR_ENCODING code1;
  CHR_ENCODING code2;

  code1 = mapCodeName(inputCode);
  code2 = mapCodeName(outputCode);

  while (fgets(x1, sizeof(x1), stdin) != NULL) {
    x1Length = strlen(x1);
    CHR_Encode(x1, x1Length, code1,
		  x2, sizeof(x2), code2,
		  &x2Length);
    fprintf(stdout, "%s", x2);
  }
}

main(int argc, char **argv)
{
    DCM_OBJECT * object;
    CONDITION cond;
    CTNBOOLEAN verbose = FALSE;

    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'v':
	    verbose = TRUE;
	    break;
	default:
	    break;
	}
    }

    if (argc < 2)
	usageerror();

    THR_Init();

    translate(argv[0], argv[1]);

    THR_Shutdown();
    return 0;
}
