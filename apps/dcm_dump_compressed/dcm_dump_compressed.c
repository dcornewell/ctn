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
** Author, Date:        Stephen M. Moore, 29-June-93
** Intent:		This program uses the DICOM package to open an DICOM
**			file and write one element to a file.
**   Usage:
**	dicom_dump_compressed [-v] [-b] group element filein fileout
**   Options:
**	b	Big Endian mode
**	v	verbose mode
**   Descriptions:
**	group		group number of the element
**	element		element number of the element
**	filein		input file name
**	fileout		output file name
**
** Last Update:         $Author: smm $, $Date: 2002-02-26 20:30:18 $
** Source File:         $RCSfile: dcm_dump_compressed.c,v $
** Revision:            $Revision: 1.4 $
** Status:              $State: Exp $
*/

static char rcsid[] = "$Revision: 1.4 $ $RCSfile: dcm_dump_compressed.c,v $";

#include "ctn_os.h"

#include "dicom.h"
#include "ctnthread.h"
#include "lst.h"
#include "condition.h"
#include "dicom_objects.h"

static void usageerror();

static CONDITION
dumpCallback(void *buf, U32 bytesExported, int index, int startFlag,
	     int lastFlag, int startOfFragment, void *ctx)
{
    int fd;
    char fileName[1024];
    int flags = 0;

    if (index == 0) {
	U32 i;
	U32 *p;
	p = buf;
	for (i = 0; i < bytesExported; i += 4, p++)
	    printf("%5d %8x %8d\n", i, *p, *p);
    } else if (lastFlag) {
	printf("This is the last call. Should be no bytes exported\n");
	if (bytesExported != 0) {
	    printf("Hmmm, we really don't expect the %d bytes you exported to me\n",
		bytesExported);
	    printf("I don't know what to do with these\n");
	}
    } else {
	sprintf(fileName, "%s.%-d", (char *) ctx, index);
	if (startOfFragment == 1) {
	  flags = O_WRONLY | O_CREAT | O_TRUNC;
	} else {
	  flags = O_WRONLY | O_APPEND;
	}

	fd = open(fileName, flags, 0666);
	if (fd < 0) {
	    perror(fileName);
	    exit(1);
	}
	printf(" Index: %5d, Bytes exported: %8d\n", index, bytesExported);
	if (startFlag) {
	    unsigned char *c;
	    c = buf;
	    printf("            %2x %2x %2x %2x\n", c[0], c[1], c[2], c[3]);
	}
	write(fd, buf, bytesExported);
	close(fd);
    }
    return DCM_NORMAL;
}

main(int argc, char **argv)
{
    DCM_OBJECT
	* object;
    CONDITION
	cond;
    CTNBOOLEAN
	verbose = FALSE;
    DCM_TAG tag = DCM_PXLPIXELDATA;
    int g = 0,
        e = 0;
    unsigned long
        options = DCM_ORDERLITTLEENDIAN;
    unsigned char buf[150000];

    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'a':
	    argc--;
	    argv++;
	    if (argc < 2)
		usageerror();
	    if (sscanf(*argv, "%x", &g) != 1)
		usageerror();
	    argc--;
	    argv++;
	    if (sscanf(*argv, "%x", &e) != 1)
		usageerror();
	    argc--;
	    argv++;
	    tag = DCM_MAKETAG(g, e);
	    break;
	case 'b':
	    options &= ~DCM_ORDERMASK;
	    options |= DCM_ORDERBIGENDIAN;
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

    if (argc < 2)
	usageerror();

    THR_Init();
    DCM_Debug(verbose);
    printf("DCM File: %s\n", *argv);
    cond = DCM_OpenFile(*argv++, options, &object);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }

    cond = DCM_GetCompressedValue(&object, tag, buf, sizeof buf, dumpCallback,
				  *argv);

    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }

    (void) DCM_CloseObject(&object);
    THR_Shutdown();
    return 0;
}

/* usageError
**
** Purpose:
**	Prints information on how to invoke the command with a description
**	of various options
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
    char *msg = "\
Usage: dcm_dump_compressed [-a group element] [-b] [-t] [-v] filein fileout\n\
\n\
    -a    Specify group, element for attribute (default 7fe0 0010)\n\
    -b    Read file assuming big endian transfer syntax\n\
    -t    Read file assuming DICOM Part 10 format\n\
    -v    Place DCM facility in verbose mode\n\
\n\
    group element   The tag of the element to be dumped\n\
    filein fileout  Input and output files\n";

    fprintf(stderr, msg);

    exit(1);
}
