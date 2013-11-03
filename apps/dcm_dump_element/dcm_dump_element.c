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
**	dicom_dump_file [-v] [-b] group element filein fileout
**   Options:
**	b	Big Endian mode
**	v	verbose mode
**   Descriptions:
**	group		group number of the element
**	element		element number of the element
**	filein		input file name
**	fileout		output file name
**
** Last Update:         $Author: smm $, $Date: 2008-03-19 20:54:19 $
** Source File:         $RCSfile: dcm_dump_element.c,v $
** Revision:            $Revision: 1.26 $
** Status:              $State: Exp $
*/

static char rcsid[] = "$Revision: 1.26 $ $RCSfile: dcm_dump_element.c,v $";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef _MSC_VER
#include <io.h>
#endif
#include <fcntl.h>
#include "dicom.h"
#include "ctnthread.h"
#include "lst.h"
#include "condition.h"
#include "dicom_objects.h"

static void usageerror();
CTNBOOLEAN verbose = FALSE;

static int
dumpSequence(DCM_OBJECT ** object, DCM_ELEMENT * element, int index, char *fileName)
{
    CONDITION cond;
    int count;
    DCM_SEQUENCE_ITEM *item;

    if (verbose) {
      fprintf(stdout, "Element (%04x %04x %s) is a sequence and is dumped to a CTN file\n",
	    DCM_TAG_GROUP(element->tag),
	    DCM_TAG_ELEMENT(element->tag),
	    element->description);
    }

    cond = DCM_GetSequenceList(object, element->tag, &element->d.sq);
    if (cond != DCM_NORMAL)
	return 1;

    count = LST_Count(&element->d.sq);
    if (index > count) {
      fprintf(stderr, "Sequence element had %d entries. You requested item %d\n",
		      count, index);
      return 1;
    }
 /*
    if (count != 1) {
	fprintf(stderr, "Sequence element had %d entries.  We expect 1\n",
		count);
	return 1;
    }
*/
    item = LST_Head(&element->d.sq);
    if (item == NULL) {
	fprintf(stderr, "Some kind of internal error.  Could not get first item from sequence list\n");
	return 1;
    }

    /* Index sequentially through the list until we hit the right entry */
    LST_Position(&element->d.sq, item);
    while (item != NULL && --index > 0) {
      item = (LST_NODE*)LST_Next(&element->d.sq);
    }
    if (item == NULL) {
      fprintf(stderr, "Some kind of internal error.  Could not get item from sequence list\n");
      fprintf(stderr, "This is after the first node was found.\n");
      return 1;
    }

    cond = DCM_WriteFile(&item->object, DCM_ORDERLITTLEENDIAN, fileName);
    if (cond != DCM_NORMAL)
	return 1;

    return 0;
}

static void
printASCIIRepresentation(DCM_OBJECT ** object, DCM_TAG tag,
			 const char *fileName)
{
    DCM_ELEMENT element;
    CONDITION cond;
    FILE *f;

    memset(&element, 0, sizeof(element));

    cond = DCM_GetElement(object, tag, &element);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    if (element.representation == DCM_SQ) {
	fprintf(stderr, "Cannot dump sequence in ASCII mode\n");
	THR_Shutdown();
	exit(1);
    }
    element.d.ot = (unsigned char *) malloc(element.length + 1);
    if (element.d.ot == NULL) {
	perror("Mallocate element buffer");
	THR_Shutdown();
	exit(1);
    }
    cond = DCM_ParseObject(object, &element, 1, NULL, 0, NULL);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    f = fopen(fileName, "w");
    if (f == NULL) {
	perror(fileName);
	exit(1);
    }
    switch (element.representation) {
    case DCM_AE:
    case DCM_AS:
	break;
    case DCM_AT:
	break;
    case DCM_CS:
    case DCM_DA:
	break;
    case DCM_DD:
	break;
    case DCM_DS:
	break;
    case DCM_FD:
    case DCM_FL:
	break;
    case DCM_IS:
    case DCM_LO:
    case DCM_LT:
	break;
    case DCM_OT:
	break;
    case DCM_SH:
	break;
    case DCM_SL:
	break;
    case DCM_SQ:
	break;
    case DCM_SS:
	break;
    case DCM_ST:
    case DCM_TM:
    case DCM_UI:
	break;
    case DCM_UL:
	break;
    case DCM_US:
	fprintf(f, "%d\n", *element.d.us);
	break;
    case DCM_UN:
	break;
    case DCM_RET:
	break;
    case DCM_CTX:
	break;
    case DCM_PN:
	fprintf(f, "%s\n", element.d.string);
	break;
    case DCM_OB:
	break;
    case DCM_OW:
	break;
    case DCM_DT:
	break;
    case DCM_DLM:
	break;
    default:
	break;
    }
    fclose(f);
}

main(int argc, char **argv)
{
    DCM_OBJECT
	* object;
    CONDITION
	cond;
    unsigned long
        ng,
        ne;
    U32
	rtnSize;
    void
       *ctx;
    DCM_ELEMENT
	element;
    DCM_TAG tag;
    int
        fd;
    unsigned long
        options = DCM_ORDERLITTLEENDIAN;
    int exitStatus;
    CTNBOOLEAN asciiFlag = FALSE;
    int fileOptions;
    int sequenceIndex = 1;

    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'a':
	    asciiFlag = TRUE;
	    break;
	case 'b':
	    options &= ~DCM_ORDERMASK;
	    options |= DCM_ORDERBIGENDIAN;
	    break;
	case 'i':
	    argc--; argv++;
	    if (argc < 1) {
	      usageerror();
	    }
	    sequenceIndex = atoi(*argv);
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

    if (argc < 4)
	usageerror();
    if (sscanf(*argv++, "%x", &ng) != 1)
	usageerror();
    if (sscanf(*argv++, "%x", &ne) != 1)
	usageerror();
    tag = DCM_MAKETAG(ng, ne);
/*    element.tag = DCM_MAKETAG(ng, ne);*/

    THR_Init();
    DCM_Debug(verbose);
    if (verbose) {
      printf("DCM File: %s\n", *argv);
    }
    cond = DCM_OpenFile(*argv, options, &object);
    /*Try open the file as part 10 format*/
    if (cond != DCM_NORMAL && ((options & DCM_PART10FILE) == 0)) {
	    /*COND_DumpConditions();*/
	    (void) DCM_CloseObject(&object);
	    if (verbose) {
	      fprintf(stderr, "Could not open %s as expected.  Trying Part 10 format.\n", *argv);
	    }
	    cond = DCM_OpenFile(*argv, options | DCM_PART10FILE, &object);
    }
    *argv++;
    COND_PopCondition(TRUE);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    if (asciiFlag) {
	printASCIIRepresentation(&object, tag, *argv);
	return 0;
    }
    cond = DCM_GetElement(&object, tag, &element);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    if (element.representation == DCM_SQ) {
	exitStatus = dumpSequence(&object, &element, sequenceIndex, *argv);
	(void) DCM_CloseObject(&object);
	COND_DumpConditions();
	THR_Shutdown();
	return exitStatus;
    }
    element.d.ot = (unsigned char *) malloc(element.length);
    if (element.d.ot == NULL) {
	perror("Mallocate element buffer");
	THR_Shutdown();
	exit(1);
    }
    ctx = NULL;
    cond = DCM_GetElementValue(&object, &element, &rtnSize, &ctx);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }

    fileOptions = O_WRONLY | O_CREAT | O_TRUNC;
#ifdef _WIN32
    fileOptions |= O_BINARY | _S_IREAD | _S_IWRITE;
#endif

    fd = open(*argv, fileOptions, 0666);
    if (fd < 0) {
	perror(*argv);
	THR_Shutdown();
	exit(1);
    }
    (void) write(fd, element.d.ot, rtnSize);
    close(fd);
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
Usage: dcm_dump_element [-b] [-i index] [-t] [-v] group element filein fileout\n\
\n\
    -b    Read file assuming big endian transfer syntax\n\
    -i    Index of sequence item to dump (1, 2, ...N)\n\
    -t    Read file assuming DICOM Part 10 format\n\
    -v    Place DCM facility in verbose mode\n\
\n\
    group element   The tag of the element to be dumped\n\
    filein fileout  Input and output files\n";

    fprintf(stderr, msg);

    exit(1);
}
