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
**                              DICOM 93
**                   Electronic Radiology Laboratory
**                 Mallinckrodt Institute of Radiology
**              Washington University School of Medicine
**
** Module Name(s):      main
**                      usageerror
** Author, Date:        Stephen M. Moore, 7-Jul-93
** Intent:		This program uses the DICOM package to open an DICOM
**			file and remove one group or more groups
**   Usage:
**	dcm_rm_group [-v] [-b] filein fileout group [group...]
**   Options:
**	b	Big Endian mode
**	v	verbose mode
**   Descriptions:
**	group		group number of the element
**	filein		input file name
**	fileout		output file name
**
** Last Update:         $Author: smm $, $Date: 2007-03-12 18:24:16 $
** Source File:         $RCSfile: dcm_rm_group.c,v $
** Revision:            $Revision: 1.15 $
** Status:              $State: Exp $
*/

static char rcsid[] = "$Revision: 1.15 $ $RCSfile: dcm_rm_group.c,v $";

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "dicom.h"
#include "ctnthread.h"
#include "lst.h"
#include "condition.h"
#include "dicom_objects.h"

static void usageerror();

main(int argc, char **argv)
{
    DCM_OBJECT
	* object;
    CONDITION
	cond;
    CTNBOOLEAN
	verbose = FALSE;
    int
        group;
    char
       *outputFile;

    unsigned long
        options = DCM_ORDERLITTLEENDIAN;
    unsigned long
	    outputOptions = DCM_ORDERLITTLEENDIAN;

    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'b':
	    options &= ~DCM_ORDERMASK;
	    options |= DCM_ORDERBIGENDIAN;
	    break;
	case 't':
	    options &= ~DCM_FILEFORMATMASK;
	    options |= DCM_PART10FILE;
	    break;
	case 'T':
	    outputOptions = DCM_PART10FILE;
	    break;
	case 'v':
	    verbose = TRUE;
	    break;
	case 'Z':
	    options |= DCM_ACCEPTVRMISMATCH;
            break;
	default:
	    break;
	}
    }

    if (argc < 3)
	usageerror();

    THR_Init();
    DCM_Debug(verbose);
    printf("DCM File: %s\n", *argv);
    argc--;
    cond = DCM_OpenFile(*argv++, options, &object);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	return (1);
    }
    outputFile = (*argv++);
    argc--;

    while (argc-- > 0) {
	if (sscanf(*argv++, "%x", &group) != 1)
	    usageerror();
	cond = DCM_RemoveGroup(&object, (unsigned short)group);
	if (cond == DCM_GROUPNOTFOUND) {
	  COND_PopCondition(TRUE);
	} else if (cond != DCM_NORMAL) {
	    COND_DumpConditions();
	    THR_Shutdown();
	    return (1);
	}
    }
    cond = DCM_WriteFile(&object, outputOptions, outputFile);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	return (1);
    }
    (void) DCM_CloseObject(&object);
    THR_Shutdown();
    return 0;
}

/* usageError
**
** Purpose:
**	Display the usage of the command with all options
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
Usage: dcm_rm_group [-b] [-t] [-T] [-v] filein fileout group [group...]\n\
\n\
  -b  Read file with big endian transfer syntax\n\
  -t  Read file with DICOM Part 10 file format\n\
  -T  Write output file in DICOM Part 10 format \n\
  -v  Place DCM facility in verbose mode\n\
\n\
  filein fileout    Input and output files\n\
  group [group ..]  One or more groups to be removed from filein\n";

    fprintf(stderr, msg);
    exit(1);
}
