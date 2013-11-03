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
** Module Name(s):
**			main
**			usageerror
**			addElement
**			addpixels
** Author, Date:	Stephen M. Moore, 12-Jun-93
** Intent:		This program strips odd groups from a DICOM file.
** Usage:		dcm_modify_elements [-b] [-v] [-p pixels] filein fileout
** Last Update:		$Author: smm $, $Date: 2004-08-27 16:38:34 $
** Source File:		$RCSfile: dcm_strip_odd_groups.c,v $
** Revision:		$Revision: 1.1 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.1 $ $RCSfile: dcm_strip_odd_groups.c,v $";

/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _MSC_VER
#include <io.h>
#include <fcntl.h>
#else
#include <sys/file.h>
#endif
#ifdef SOLARIS
#include <sys/fcntl.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include "dicom.h"
#include "ctnthread.h"
#include "condition.h"
#include "lst.h"
#include "dicom_objects.h"
*/
#include "ctn_os.h"
#include "dicom.h"
#include "ctnthread.h"
#include "condition.h"
#include "lst.h"
#include "dicom_objects.h"


/* usageError
**
** Purpose:
**	Displays the correct usage of the command
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
Usage: dcm_strip_odd_groups [-b] [-t] [-T] [-v] filein fileout\n\
\n\
  -b  Use Big-Endian order to store the object (not Part 10) \n\
  -t  Files are processed in DICOM Part 10 format \n\
  -T  Write output in Part 10 format\n\
  -v  Verbose mode; place DCM facility in verbose mode \n\
\n\
  filein   File that contains original DICOM object \n\
  fileout  File that stores the modified DICOM object \n";

  fprintf(stdout, msg);
  exit(1);
}

int
main(int argc, char **argv)
{
    DCM_OBJECT
    * object;
    CONDITION
	cond;
    char
        buffer[120],
        data[120];
    int
        group,
        element;
    char
       *pixelfile = NULL;
    CTNBOOLEAN
	verbose = FALSE;
    long
        options = DCM_ORDERLITTLEENDIAN;
    unsigned long
	outputOptions = DCM_ORDERLITTLEENDIAN;

    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'b':
	    options &= ~DCM_ORDERMASK;
	    options |= DCM_ORDERBIGENDIAN;
	    break;
	case 'p':
	case 'P':
	    argc--;
	    argv++;
	    pixelfile = *argv;
	    break;
	case 't':
	    options &= ~DCM_FILEFORMATMASK;
	    options |= DCM_PART10FILE;
	    break;
	case 'T':
	    outputOptions = DCM_PART10FILE;
	    break;
	case 'v':
	case 'V':
	    verbose = TRUE;
	    break;
	default:
	    fprintf(stderr, "Unrecognized option: %c\n", *(argv[0] + 1));
	    break;
	}
    }
    if (argc < 2)
	usageerror();

    if (strcmp(argv[0], argv[1]) == 0) {
	fprintf(stderr,
		"This program must have different input and output files\n");
	exit(1);
    }
    THR_Init();
    DCM_Debug(verbose);
    cond = DCM_OpenFile(*argv, options, &object);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }

    cond = DCM_StripOddGroups(&object);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	return 1;
    }

    (void) DCM_DumpElements(&object, 0);
    cond = DCM_WriteFile(&object, outputOptions, *++argv);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	return 1;
    }
    (void) DCM_CloseObject(&object);
    THR_Shutdown();
    return 0;
}
