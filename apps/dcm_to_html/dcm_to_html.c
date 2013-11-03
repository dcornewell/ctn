/*
          Copyright (C) 1998, Washington University

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
** Author, Date:	Stephen M. Moore, 4-Dec-1998
** Intent:		This program reads a DICOM file and creates an HTML
**			representation of the information in the file.
**   Usage:		dcm_to_html [-t] file
** Last Update:		$Author: smm $, $Date: 2002-04-16 21:27:58 $
** Source File:		$RCSfile: dcm_to_html.c,v $
** Revision:		$Revision: 1.3 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.3 $ $RCSfile: dcm_to_html.c,v $";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef GCCSUNOS
#include <sys/types.h>
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
    const char *c1;
    const char *c2;

    c1 = e1->d.string;
    c2 = e2->d.string;

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
    printf("  TEXT %04x %04x %32s\n        %s\n        %s\n",
	   DCM_TAG_GROUP(e1->tag),
	   DCM_TAG_ELEMENT(e2->tag),
	   e1->description,
	   c1, c2);
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
    case DCM_UN:		/* Unknown */
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

static CONDITION callback(const DCM_ELEMENT* e, void* ctx)
{
  char txt[512] = "";
  printf("<TR>\n");
  printf("<TD>%04x %04x</TD>\n", DCM_TAG_GROUP(e->tag), DCM_TAG_ELEMENT(e->tag));
  printf("<TD ALIGN=RIGHT>%6d</TD>\n", e->length);
  printf("<TD ALIGN=CENTER>%s</TD>\n", e->description);
  if (e->d.ot == NULL) {
    strcpy(txt, "No data");
  } else if (e->length == 0) {
    strcpy(txt, "0 length data");
  } else {
    if (DCM_IsString(e->representation)) {
      int stringLength;
      stringLength = MIN(sizeof(txt)-1, e->length);
      strncpy(txt, e->d.string, stringLength);
      txt[stringLength] = '\0';
    } else {
      switch (e->representation) {
      case DCM_AE:
      case DCM_AS:
	break;
      case DCM_AT:
	sprintf(txt, "%04x %04x",
		DCM_TAG_GROUP(*e->d.ul), DCM_TAG_ELEMENT(*e->d.ul));
	break;
      case DCM_CS:
      case DCM_DA:
	break;
      case DCM_DD:
	strcpy(txt, "Unimplemented");
	break;
      case DCM_DS:
	break;
      case DCM_FD:
      case DCM_FL:
	strcpy(txt, "Unimplemented");
	break;
      case DCM_IS:
      case DCM_LO:
      case DCM_LT:
      case DCM_OT:
      case DCM_SH:
	break;
      case DCM_SL:
	sprintf(txt, "%d", *e->d.sl);
	break;
      case DCM_SQ:
	strcpy(txt, "Unimplemented");
	break;
      case DCM_SS:
	sprintf(txt, "%d", *e->d.ss);
	break;
      case DCM_ST:
      case DCM_TM:
      case DCM_UI:
	break;
      case DCM_UL:
	sprintf(txt, "%d", *e->d.ul);
	break;
      case DCM_US:
	sprintf(txt, "%d", *e->d.us);
	break;
      case DCM_UN:
	strcpy(txt, "Unknown");
	break;
      case DCM_RET:
	strcpy(txt, "Retired");
	break;
      case DCM_CTX:
	strcpy(txt, "Context sensitive");
	break;
      case DCM_PN:
	break;
      case DCM_OB:
      case DCM_OW:
	strcpy(txt, "OB or OW");
	break;
      case DCM_DT:
	break;
      case DCM_DLM:
	strcpy(txt, "Delimiter");
	break;
      default:
	break;
      }
    }
  }
  printf("<TD>%s</TD></TR>\n", txt);
  return DCM_NORMAL;
}

main(int argc, char **argv)
{
    DCM_OBJECT *obj1;
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
	case 'm':
	    argc--;
	    argv++;
	    vmLimit = atoi(*argv);
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
    if (argc != 1)
	usageerror();

    cond = DCM_OpenFile(*argv, options, &obj1);
    if (cond != DCM_NORMAL && ((options & DCM_PART10FILE) == 0)) {
	COND_DumpConditions();
	(void) DCM_CloseObject(&obj1);
	(void) COND_PopCondition(TRUE);
	cond = DCM_OpenFile(*argv, options | DCM_PART10FILE, &obj1);
    }
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }

    printf("<HTML>\n");
    printf("<HEAD>\n");
    printf("<META HTTP-EQUIV=""Content-Type"" CONTENT=""text/html; charset=iso-2022-jp"">\n");
    printf("<TITLE>dcm_to_html</TITLE>\n");
    printf("</HEAD>");

    printf("<BODY BGCOLOR=#ffffff FGCOLOR=#ff000000>\n");
    printf("<TABLE BORDER>\n");
    DCM_ScanParseObject(&obj1,
			NULL, 0,           /* Buffer/buffer size */
			NULL, 0,           /* Vector of elements to scan */
			callback, NULL);
    printf("</TABLE>\n");
    printf("</BODY>\n");
    printf("</HTML>");

    (void) DCM_CloseObject(&obj1);;

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
Usage: dcm_to_html [-b] [-g] [-l] [-o] [-t] [-v] [-z] file1 \n\
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
