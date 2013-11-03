/*
          Copyright (C) 1993 - 1995, RSNA and Washington University

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
**				DICOM 95
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
**			main
**			usageerror
** Author, Date:	Stephen M. Moore, 12-May-95
** Intent:		This program provides a utility for converting
**			files in original CTN file format to DICOM part 10
**			files.
** Last Update:		$Author: smm $, $Date: 1998-08-03 21:25:49 $
** Source File:		$RCSfile: dcm_ctnto10.c,v $
** Revision:		$Revision: 1.6 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.6 $ $RCSfile: dcm_ctnto10.c,v $";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
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
#include "dicom_uids.h"

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
    long
        options = DCM_ORDERLITTLEENDIAN | DCM_PART10FILE;
    long
        openOptions = DCM_ORDERLITTLEENDIAN;
    DCM_FILE_META *fileMeta;

    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'b':
	    options &= ~DCM_ORDERMASK;
	    options |= DCM_ORDERBIGENDIAN;
	    break;
	case 'B':
	    options &= ~DCM_ORDERMASK;
	    options |= DCM_EXPLICITBIGENDIAN;
	    break;
	case 'L':
	    options &= ~DCM_ORDERMASK;
	    options |= DCM_EXPLICITLITTLEENDIAN;
	    break;
	case 't':
	    openOptions |= DCM_PART10FILE;
	    break;
	case 'v':
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
	return 1;
    }
    THR_Init();
    DCM_Debug(verbose);

    cond = DCM_OpenFile(*argv, openOptions, &object);
    if (cond != DCM_NORMAL)
	goto abort;

    if ((options & DCM_PART10FILE) != 0)
	(void) DCM_RemoveGroup(&object, 0x0002);

    cond = DCM_DefaultFileMeta(&object, &fileMeta);
    if (cond != DCM_NORMAL)
	goto abort;

    if ((options & DCM_ORDERMASK) == DCM_EXPLICITBIGENDIAN)
	strcpy(fileMeta->transferSyntaxUID, DICOM_TRANSFERBIGENDIANEXPLICIT);

    if ((options & DCM_ORDERMASK) == DCM_EXPLICITLITTLEENDIAN)
	strcpy(fileMeta->transferSyntaxUID, DICOM_TRANSFERLITTLEENDIANEXPLICIT);

    cond = DCM_SetFileMeta(&object, fileMeta);
    if (cond != DCM_NORMAL)
	goto abort;

    cond = DCM_FreeFileMeta(&fileMeta);
    if (cond != DCM_NORMAL)
	goto abort;

    cond = DCM_WriteFile(&object, options, *++argv);
    (void) DCM_CloseObject(&object);
    if (cond != DCM_NORMAL)
	goto abort;

    THR_Shutdown();
    return 0;

abort:
    COND_DumpConditions();
    THR_Shutdown();
    return 1;
}


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
Usage: [-bBL] [-v] filein fileout\n\
    -b  Big endian output (non standard)\n\
    -B  Big endian explicit transfer syntax\n\
    -L  Little endian explicit transfer syntax\n";

    fprintf(stderr, msg);
    exit(1);
}
