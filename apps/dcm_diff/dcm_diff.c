/*
          Copyright (C) 1997, Washington University

	  This application was developed at the
                  Electronic Radiology Laboratory
                  Mallinckrodt Institute of Radiology
                  Washington University School of Medicine
                  510 S. Kingshighway Blvd.
                  St. Louis, MO 63110

          THIS SOFTWARE IS MADE AVAILABLE, AS IS, AND NEITHER RSNA NOR
          WASHINGTON UNIVERSITY MAKE ANY WARRANTY ABOUT THE SOFTWARE, ITS
          PERFORMANCE, ITS MERCHANTABILITY OR FITNESS FOR ANY PARTICULAR
          USE, FREEDOM FROM ANY COMPUTER DISEASES OR ITS CONFORMITY TO ANY
          SPECIFICATION. THE ENTIRE RISK AS TO QUALITY AND PERFORMANCE OF
          THE SOFTWARE IS WITH THE USER.

          Copyright of the software and supporting documentation is
          owned by and Washington University, and free access
          is hereby granted as a license to use this software, copy this
          software and prepare derivative works based upon this software.
          However, any distribution of this software source code or
          supporting documentation or derivative works (source code and
          supporting documentation) must include the three paragraphs of
          the copyright notice.
*/
/* Copyright marker.  Copyright will be inserted above.  Do not remove */
/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	main
**			usageerror
** Author, Date:	Stephen M. Moore, 10-May-97
** Intent:		This program uses the DICOM OBJECTS package to open
**			two DICOM files and compare the attributes in the
**			files.
**   Usage:
**			dcm_diff [-b] [-g] [-v] [-z] file1 file2
** Last Update:		$Author: smm $, $Date: 2002-04-16 21:27:03 $
** Source File:		$RCSfile: dcm_diff.c,v $
** Revision:		$Revision: 1.4 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.4 $ $RCSfile: dcm_diff.c,v $";

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

static CTNBOOLEAN verboseOutput = FALSE;

static void usageerror();

static void
compareTextAttribute(const DCM_ELEMENT * e1,
		     const DCM_ELEMENT * e2, void *ctx)
{
    char *c1;
    char *c2;
    char temp1[1024];
    char temp2[1024];
    int l;

    l = (e1->length < 1023) ? e1->length : 1023;
    strncpy(temp1, e1->d.string, l);
    temp1[l] = '\0';

    l = (e2->length < 1023) ? e2->length : 1023;
    strncpy(temp2, e2->d.string, l);
    temp2[l] = '\0';

    c1 = temp1;
    c2 = temp2;

    while (*c1 == ' ')
	c1++;

    while (*c2 == ' ')
	c2++;

    if (strcmp(c1, c2) == 0) {
#if 0
	if (verboseOutput)
	    printf("       %04x %04x %32x\n",
		   DCM_TAG_GROUP(e1->tag),
		   DCM_TAG_GROUP(e2->tag),
		   e1->description);
#endif
	return;
    }
    printf("  TEXT %04x %04x %32s\n        %4d %s\n        %4d %s\n",
	   DCM_TAG_GROUP(e1->tag),
	   DCM_TAG_ELEMENT(e2->tag),
	   e1->description,
	   strlen(c1), c1, strlen(c2), c2);
    (*(long *) ctx)++;

}

static void
compareULAttribute(const DCM_ELEMENT * e1, const DCM_ELEMENT * e2, void *ctx)
{
    const U32 *p1,
       *p2;
    U32 l1;

    p1 = e1->d.ul;
    p2 = e2->d.ul;
    if (e1->length != e2->length) {
	printf("LENGTH %04x %04x %32s %d %d\n",
	       DCM_TAG_GROUP(e1->tag),
	       DCM_TAG_ELEMENT(e2->tag),
	       e1->description,
	       e1->length, e2->length);
	(*(long *) ctx)++;
	return;
    }
    l1 = e1->length;

    while (l1 > 0) {
	if (*p1 != *p2) {
	    printf("BINARY %04x %04x %32s %d %d\n",
		   DCM_TAG_GROUP(e1->tag),
		   DCM_TAG_ELEMENT(e2->tag),
		   e1->description,
		   *p1, *p2);
	    (*(long *) ctx)++;
	    return;
	}
	p1++;
	p2++;
	l1 -= sizeof(U32);
    }
}

static void
compareSLAttribute(const DCM_ELEMENT * e1, const DCM_ELEMENT * e2, void *ctx)
{
    const S32 *p1,
       *p2;
    U32 l1;

    p1 = e1->d.sl;
    p2 = e2->d.sl;
    if (e1->length != e2->length) {
	printf("LENGTH %04x %04x %32s %d %d\n",
	       DCM_TAG_GROUP(e1->tag),
	       DCM_TAG_ELEMENT(e2->tag),
	       e1->description,
	       e1->length, e2->length);
	(*(long *) ctx)++;
	return;
    }
    l1 = e1->length;

    while (l1 > 0) {
	if (*p1 != *p2) {
	    printf("BINARY %04x %04x %32s %d %d\n",
		   DCM_TAG_GROUP(e1->tag),
		   DCM_TAG_ELEMENT(e2->tag),
		   e1->description,
		   *p1, *p2);
	    (*(long *) ctx)++;
	    return;
	}
	p1++;
	p2++;
	l1 -= sizeof(S32);
    }
}

static void
compareUSAttribute(const DCM_ELEMENT * e1, const DCM_ELEMENT * e2, void *ctx)
{
    const U16 *p1,
       *p2;
    U32 l1;

    p1 = e1->d.us;
    p2 = e2->d.us;
    if (e1->length != e2->length) {
	printf("LENGTH %04x %04x %32s %d %d\n",
	       DCM_TAG_GROUP(e1->tag),
	       DCM_TAG_ELEMENT(e2->tag),
	       e1->description,
	       e1->length, e2->length);
	(*(long *) ctx)++;
	return;
    }
    l1 = e1->length;

    while (l1 > 0) {
	if (*p1 != *p2) {
	    printf("BINARY %04x %04x %32s %d %d\n",
		   DCM_TAG_GROUP(e1->tag),
		   DCM_TAG_ELEMENT(e2->tag),
		   e1->description,
		   *p1, *p2);
	    (*(long *) ctx)++;
	    return;
	}
	p1++;
	p2++;
	l1 -= sizeof(U16);
    }
}

static void
compareSSAttribute(const DCM_ELEMENT * e1, const DCM_ELEMENT * e2, void *ctx)
{
    const S16 *p1,
       *p2;
    S32 l1;

    p1 = e1->d.ss;
    p2 = e2->d.ss;
    if (e1->length != e2->length) {
	printf("LENGTH %04x %04x %32s %d %d\n",
	       DCM_TAG_GROUP(e1->tag),
	       DCM_TAG_ELEMENT(e2->tag),
	       e1->description,
	       e1->length, e2->length);
	(*(long *) ctx)++;
	return;
    }
    l1 = e1->length;

    while (l1 > 0) {
	if (*p1 != *p2) {
	    printf("BINARY %04x %04x %32s %d %d\n",
		   DCM_TAG_GROUP(e1->tag),
		   DCM_TAG_ELEMENT(e2->tag),
		   e1->description,
		   *p1, *p2);
	    (*(long *) ctx)++;
	    return;
	}
	p1++;
	p2++;
	l1 -= sizeof(S16);
    }
}




static void
compareAttribute(const DCM_ELEMENT * e1, const DCM_ELEMENT * e2,
		 void *ctx)
{

    if (verboseOutput)
	printf("       %04x %04x %32s\n", DCM_TAG_GROUP(e1->tag),
	       DCM_TAG_ELEMENT(e1->tag), e1->description);
    if (e1->representation != e2->representation) {
	printf(" VR    %04x %04x %32s\n",
	       DCM_TAG_GROUP(e1->tag),
	       DCM_TAG_ELEMENT(e1->tag), e1->description);
	return;
    }
    switch (e1->representation) {
    case DCM_AE:		/* Application Entity */
    case DCM_AS:		/* Age string */
	compareTextAttribute(e1, e2, ctx);
	break;
    case DCM_AT:		/* Attribute tag */
	break;
    case DCM_CS:		/* Control string */
    case DCM_DA:		/* Date */
	compareTextAttribute(e1, e2, ctx);
	break;
    case DCM_DD:		/* Data set */
	break;
    case DCM_DS:		/* Decimal string */
	compareTextAttribute(e1, e2, ctx);
	break;
    case DCM_FD:		/* Floating double */
    case DCM_FL:		/* Float */
	break;
    case DCM_IS:		/* Integer string */
    case DCM_LO:		/* Long string */
    case DCM_LT:		/* Long text */
	compareTextAttribute(e1, e2, ctx);
	break;
    case DCM_OT:		/* Other binary value */
	break;
    case DCM_SH:		/* Short string */
	compareTextAttribute(e1, e2, ctx);
	break;
    case DCM_SL:		/* Signed long */
	compareSLAttribute(e1, e2, ctx);
	break;
    case DCM_SQ:		/* Sequence of items */
	break;
    case DCM_SS:		/* Signed short */
	compareSSAttribute(e1, e2, ctx);
	break;
    case DCM_ST:		/* Short text */
    case DCM_TM:		/* Time */
    case DCM_UI:		/* Unique identifier (UID) */
	compareTextAttribute(e1, e2, ctx);
	break;
    case DCM_UL:		/* Unsigned long */
	compareULAttribute(e1, e2, ctx);
	break;
    case DCM_US:		/* Unsigned short */
	compareUSAttribute(e1, e2, ctx);
	break;
    case DCM_UN:		/* Unknown/unspecified (non-standard) */
    case DCM_RET:		/* Retired */
    case DCM_CTX:		/* Context sensitive (non-standard) */
	break;
    case DCM_PN:		/* Person Name */
	compareTextAttribute(e1, e2, ctx);
	break;
    case DCM_OB:		/* Other, byte */
    case DCM_OW:		/* Other, word */
	break;
    case DCM_DT:		/* Date/Time */
	compareTextAttribute(e1, e2, ctx);
	break;
    case DCM_DLM:
	break;
    case DCM_UT:		/* Long text */
	compareTextAttribute(e1, e2, ctx);
    }
}

static void
compareCallback(const DCM_ELEMENT * e1, const DCM_ELEMENT * e2,
		void *ctx)
{
    if (e1 == NULL) {
	printf(" FILE2 %04x %04x %32s\n", DCM_TAG_GROUP(e2->tag),
	       DCM_TAG_ELEMENT(e2->tag), e2->description);
	(*(long *) ctx)++;
    } else if (e2 == NULL) {
	printf(" FILE1 %04x %04x %32s\n", DCM_TAG_GROUP(e1->tag),
	       DCM_TAG_ELEMENT(e1->tag), e1->description);
	(*(long *) ctx)++;
    } else {
	compareAttribute(e1, e2, ctx);
    }
}

main(int argc, char **argv)
{
    DCM_OBJECT *obj1,
       *obj2;
    CONDITION cond;
    CTNBOOLEAN
	verbose = FALSE,
	exitFlag = FALSE;
    unsigned long
        options = DCM_ORDERLITTLEENDIAN;
    long vmLimit = 0;
    long differences = 0;

    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'b':
	    options &= ~DCM_ORDERMASK;
	    options |= DCM_ORDERBIGENDIAN;
	    break;
	case 'e':
	    exitFlag = TRUE;
	    break;
	case 'g':
	    options &= ~DCM_GROUPLENGTHMASK;
	    options |= DCM_NOGROUPLENGTH;
	    break;
	case 'l':
	    options &= ~DCM_LENGTHTOENDMASK;
	    options |= DCM_USELENGTHTOEND;
	    break;
	case 'm':
	    argc--;
	    argv++;
	    vmLimit = atoi(*argv);
	    break;
	case 'o':
	    verboseOutput = TRUE;
	    break;
	case 't':
	    options &= ~DCM_FILEFORMATMASK;
	    options |= DCM_PART10FILE;
	    break;

	case 'v':
	    verbose = TRUE;
	    break;
	case 'z':
	    options |= DCM_FORMATCONVERSION;
	    break;
	default:
	    break;
	}
    }

    THR_Init();
    DCM_Debug(verbose);
    if (argc != 2)
	usageerror();

    printf("DICOM File: %s\n", *argv);
    cond = DCM_OpenFile(*argv, options, &obj1);
    if (cond != DCM_NORMAL && ((options & DCM_PART10FILE) == 0)) {
	COND_DumpConditions();
	(void) DCM_CloseObject(&obj1);
	(void) COND_PopCondition(TRUE);
	fprintf(stderr, "Could not open %s as expected.  Trying Part 10 format.\n", *argv);
	cond = DCM_OpenFile(*argv, options | DCM_PART10FILE, &obj1);
    }
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    argv++;
    printf("DICOM File: %s\n", *argv);
    cond = DCM_OpenFile(*argv, options, &obj2);
    if (cond != DCM_NORMAL && ((options & DCM_PART10FILE) == 0)) {
	COND_DumpConditions();
	(void) DCM_CloseObject(&obj2);
	(void) COND_PopCondition(TRUE);
	fprintf(stderr, "Could not open %s as expected.  Trying Part 10 format.\n", *argv);
	cond = DCM_OpenFile(*argv, options | DCM_PART10FILE, &obj2);
    }
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    DCM_CompareAttributes(&obj1, &obj2, compareCallback, &differences);

    printf("Differences encountered: %5d\n", differences);

    (void) DCM_CloseObject(&obj1);
    (void) DCM_CloseObject(&obj2);

#ifdef MALLOC_DEBUG
    malloc_verify(0);
    malloc_shutdown();
#endif
    THR_Shutdown();
    return 0;
}

/* usageerror
**
** Purpose:
**	Print the usage string for this application and exit.
**
** Parameter Dictionary:
**	None
**
** Return Values:
**	None
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static void
usageerror()
{
    char msg[] = "\
Usage: dcm_diff [-b] [-g] [-l] [-o] [-t] [-v] [-z] file1 file2 \n\
\n\
    -b        Input files are stored in big-endian byte order\n\
    -g        Remove group length elements\n\
    -l        Use (retired) length-to-end attribute for object length\n\
    -o        Place output in verbose mode\n\
    -t        Part 10 file\n\
    -v        Place DCM facility in verbose mode\n\
    -z        Perform format conversion (verification) on data in files\n\
\n\
    file1 file2 \n";

    fprintf(stderr, msg);
    exit(1);
}
