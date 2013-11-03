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
** Author, Date:	Stephen M. Moore, 12-Jun-93
** Intent:		This program is used to create new UIDS in an
**			image.
** Usage:		new_uids -i patientid file.in file.out
** Last Update:		$Author: smm $, $Date: 2002-04-18 21:13:59 $
** Source File:		$RCSfile: new_uids.c,v $
** Revision:		$Revision: 1.3 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.3 $ $RCSfile: new_uids.c,v $";

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
#include "dicom_uids.h"

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
Usage: dcm_modify_elements [-b] [-t] [-v] [-p pix] filein fileout\n\
\n\
  -b  Assume big endian transfer syntax (non-standard)\n\
  -t  Assume DICOM Part 10 file format\n\
  -p  Add pixels from file pix\n\
\n\
  filein fileout  Input and output files\n";

    fprintf(stderr, msg);
    exit(1);
}


/* addElement
**
** Purpose:
**	Add an element to the DICOM object
**
** Parameter Dictionary:
**	group		Group number of the element to be added
**	element		Element number
**	data		Actual data to be added
**	object		Handle to the object to which the data is added
**
** Return Values:
**
**	DCM_ILLEGALOBJECT
**	DCM_ILLEGALREPRESENTATION
**	DCM_INSERTFAILED
**	DCM_NORMAL
**	DCM_NULLOBJECT
**	DCM_UNRECOGNIZEDELEMENT
**	DCM_UNRECOGNIZEDGROUP
**
** Notes:
**
** Algorithm:
**	Instead of using DCM_AddElements, we use DCM_ModifyElements to
**	achieve our purpose
*/

static CONDITION
addElement(int group, int element, char *data,
	   DCM_OBJECT ** object)
{
    CONDITION
    cond;
    DCM_ELEMENT
	e;
    int
        i;
    U16
	ush;
    U32
	ulo;
    S16
	ssh;
    S32
	slo;

    e.tag = DCM_MAKETAG(group, element);
    cond = DCM_LookupElement(&e);
    if (cond != DCM_NORMAL)
	return cond;

    switch (e.representation) {
    case DCM_AE:		/* Application Entity */
    case DCM_AT:		/* Attribute tag */
    case DCM_DD:		/* Data set */
    case DCM_FD:		/* Floating double */
    case DCM_FL:		/* Float */
    case DCM_OT:		/* Other binary value */
    case DCM_SQ:		/* Sequence of items */
    case DCM_UN:		/* Unknown */
    case DCM_RET:		/* Retired */
    case DCM_CTX:		/* Context sensitive */
	printf("The representation for (%04x %04x) is unimplemented\n",
	       group, element);
	e.d.ot = NULL;
	break;
    case DCM_AS:		/* Age String */
    case DCM_CS:		/* Control string */
    case DCM_DA:		/* Date */
    case DCM_DS:		/* Decimal string */
    case DCM_IS:		/* Integer string */
    case DCM_LT:		/* Long text */
    case DCM_LO:		/* Long string */
    case DCM_PN:		/* Person Name */
    case DCM_SH:		/* Short string */
    case DCM_ST:		/* Short text */
    case DCM_TM:		/* Time */
    case DCM_UI:		/* Unique identifier (UID) */
	if (strcmp(data, "#") == 0) {
	    e.length = 0;
	    e.d.string = "";
	} else {
	    e.length = strlen(data);
	    e.d.string = data;
	}
	break;
    case DCM_SL:		/* Signed long */
	if (strcmp(data, "#") == 0)
	    e.length = 0;
	else {
	    if (sscanf(data, "%d", &i) != 1) {
		fprintf(stderr, "Error parsing %s as signed long\n", data);
		return 0;
	    }
	    e.length = sizeof(slo);
	    slo = (S32) i;
	}
	e.d.sl = &slo;
	break;
    case DCM_SS:		/* Signed short */
	if (strcmp(data, "#") == 0)
	    e.length = 0;
	else {
	    if (sscanf(data, "%d", &i) != 1) {
		fprintf(stderr, "Error parsing %s as signed short\n", data);
		return 0;
	    }
	    e.length = sizeof(ssh);
	    ssh = (S16) i;
	}
	e.d.ss = &ssh;
	break;
    case DCM_UL:		/* Unsigned long */
	if (strcmp(data, "#") == 0)
	    e.length = 0;
	else {
	    if (sscanf(data, "%d", &i) != 1) {
		fprintf(stderr, "Error parsing %s as unsigned long\n", data);
		return 0;
	    }
	    e.length = sizeof(ulo);
	    ulo = (U32) i;
	}
	e.d.ul = &ulo;
	break;
    case DCM_US:		/* Unsigned short */
	if (strcmp(data, "#") == 0)
	    e.length = 0;
	else {
	    if (sscanf(data, "%d", &i) != 1) {
		fprintf(stderr, "Error parsing %s as unsigned short\n", data);
		return 0;
	    }
	    e.length = sizeof(ush);
	    ush = (U16) i;
	}
	e.d.us = &ush;
	break;
    default:
	fprintf(stderr,
		"Should not reach default for element (%04x %04x)\n",
		group, element);
	e.d.ot = NULL;
    }

    if (e.d.ot != NULL)
	cond = DCM_ModifyElements(object, &e, 1, NULL, 0, NULL);
    else
	cond = 0;

    return cond;
}



/* fileSize
**
** Purpose:
**	Returns file size on success
**
** Parameter Dictionary:
**	fd	File descriptor
**
** Return Values:
**	file size on success, else error status code
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static int
fileSize(int fd)
{
    int
        status;
    struct stat
        im_stat;

    status = fstat(fd, &im_stat);
    if (status < 0) {
	return status;
    } else
	return im_stat.st_size;
}

typedef struct {
	U32 tag;
	UID_TYPE type;
} MY_TABLE;

int
main(int argc, char **argv)
{
    DCM_OBJECT * object;
    CONDITION cond;

    char
       *pixelfile = NULL;
    CTNBOOLEAN
	verbose = FALSE;
    long
        options = DCM_ORDERLITTLEENDIAN;
    char* patientID = NULL;

    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'i':
	    argc--; argv++;
	    patientID = *argv;
	    break;
	case 't':
	    options &= ~DCM_FILEFORMATMASK;
	    options |= DCM_PART10FILE;
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
    {
	MY_TABLE t[] = {
		{ DCM_RELSTUDYINSTANCEUID, UID_STUDY},
		{ DCM_RELSERIESINSTANCEUID, UID_SERIES},
		{ DCM_IDSOPINSTANCEUID, UID_IMAGE}
	};

	int i;
	char uid[1024];

	for (i = 0; i < (int)DIM_OF(t); i++) {
	    cond = UID_NewUID(t[i].type, uid);
	    if (cond != UID_NORMAL) {
		COND_DumpConditions();
		return 1;
	    }
	    cond = addElement(DCM_TAG_GROUP(t[i].tag),
				DCM_TAG_ELEMENT(t[i].tag),
				uid, &object);
	    if (cond != DCM_NORMAL) {
		COND_DumpConditions();
		THR_Shutdown();
		exit(1);
	    }
	}
    }
    if (patientID != NULL) {
	cond = addElement(DCM_TAG_GROUP(DCM_PATID),
				DCM_TAG_ELEMENT(DCM_PATID),
				patientID, &object);
	if (cond != DCM_NORMAL) {
	    COND_DumpConditions();
	    THR_Shutdown();
	    exit(1);
	}
    }

    (void) DCM_DumpElements(&object, 0);
    /* cond = DCM_WriteFile(&object, options, *++argv); */
    cond = DCM_WriteFile(&object, DCM_ORDERLITTLEENDIAN, *++argv);
    if (cond != DCM_NORMAL)
	COND_DumpConditions();
    (void) DCM_CloseObject(&object);
    THR_Shutdown();
    return 0;
}


