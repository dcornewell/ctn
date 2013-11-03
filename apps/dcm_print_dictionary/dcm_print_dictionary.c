/*
          Copyright (C) 1993-1995 RSNA and Washington University

          The software and supporting documentation for the Radiological
          Society of North America (RSNA) 1993 Digital Imaging and
          Communications in Medicine (DICOM) Demonstration were developed
          at the
                  Electronic Radiology Laboratory
                  Mallinckrodt Institute of Radiology
                  Washington University School of Medicine
                  510 S. Kingshighway Blvd.
                  St. Louis, MO 63110
          as part of the 1993-1995 DICOM Central Test Node project for, and
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
/*
** @$=@$=@$=
*/
/*
**				DICOM 95
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	main
**			usageerror
** Author, Date:	Stephen M. Moore, 9-May-95
** Intent:		This program dumps the DICOM data dictionary to
**			the standard output.
** Usage:
** Last Update:		$Author: smm $, $Date: 2002-04-16 21:27:43 $
** Source File:		$RCSfile: dcm_print_dictionary.c,v $
** Revision:		$Revision: 1.8 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.8 $ $RCSfile: dcm_print_dictionary.c,v $";

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
#include "dicom_uids.h"


static void groupCallback(unsigned short group, char *description, void *ctx);
static void
elementCallback(DCM_TAG tag, char *description,
		DCM_VALUEREPRESENTATION representation, void *ctx);
static char *translate(DCM_VALUEREPRESENTATION r);

void
uidCallback(const UID_DESCRIPTION * d1, void *ctx)
{
    char *classDescription = "Unknown class";

    switch (d1->UIDclass) {
    case UID_CLASS_K_APPLICATIONCONTEXT:
	classDescription = "Application Context";
	break;
    case UID_CLASS_K_IMPLEMENTATION:
	classDescription = "Implementation";
	break;
    case UID_CLASS_K_SOPCLASS:
	classDescription = "SOP Class";
	break;
    case UID_CLASS_K_METASOPCLASS:
	classDescription = "Meta SOP Class";
	break;
    case UID_CLASS_K_TRANSFERSYNTAX:
	classDescription = "Transfer Syntax";
	break;
    case UID_CLASS_K_WELLKNOWNUID:
	classDescription = "Well Known UID";
	break;
    }

    printf("%s\n  %-20s: %s\n  %s\n",
	   d1->UID,
	   classDescription,
	   d1->description,
	   d1->originator);
}

main(int argc, char **argv)
{
    CONDITION cond;
    LST_HEAD *l,
       *elementList;
    DCM_ELEMENT_NODE *n,
       *elementNode;
    unsigned short group;
    CTNBOOLEAN uidDictionary = FALSE;

    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'u':
	    uidDictionary = TRUE;
	    break;
	default:
	    fprintf(stderr, "Unrecognized option: %s\n", *argv);
	    exit(1);
	    break;
	}
    }

    THR_Init();
    if (uidDictionary) {
	UID_ScanDictionary(uidCallback, 0);
	THR_Shutdown();
	return 0;
    }
    l = LST_Create();
    if (l == NULL) {
	THR_Shutdown();
	return 1;
    }
    elementList = LST_Create();
    if (elementList == NULL) {
	THR_Shutdown();
	return 1;
    }
    cond = DCM_GroupDictionary(0xffff, &l, groupCallback);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    n = LST_Dequeue(&l);
    while (n != NULL) {
	group = DCM_TAG_GROUP(n->e.tag);
	printf("%04x  %s\n", group, n->e.description);

	cond = DCM_ElementDictionary(DCM_MAKETAG(group, 0xffff), &elementList,
				     elementCallback);
	if (cond != DCM_NORMAL) {
	    COND_DumpConditions();
	    THR_Shutdown();
	    exit(1);
	}
	elementNode = LST_Dequeue(&elementList);
	while (elementNode != NULL) {
	    printf("  %04x %04x %s %s\n",
		   DCM_TAG_GROUP(elementNode->e.tag),
		   DCM_TAG_ELEMENT(elementNode->e.tag),
		   translate(elementNode->e.representation),
		   elementNode->e.description);

	    free(elementNode);
	    elementNode = LST_Dequeue(&elementList);
	}

	free(n);
	n = LST_Dequeue(&l);
    }
    THR_Shutdown();
    return 0;
}

static void
groupCallback(unsigned short group, char *description, void *ctx)
{
    DCM_ELEMENT_NODE *n;
    LST_HEAD **l;

    l = (LST_HEAD **) ctx;
    n = calloc(1, sizeof(*n));
    if (n == NULL)
	return;

    n->e.tag = DCM_MAKETAG(group, 0);
    strcpy(n->e.description, description);

    (void) LST_Enqueue(l, n);
}

static void
elementCallback(DCM_TAG tag, char *description,
		DCM_VALUEREPRESENTATION representation, void *ctx)
{
    DCM_ELEMENT_NODE *n;
    LST_HEAD **l;

    l = (LST_HEAD **) ctx;
    n = calloc(1, sizeof(*n));
    if (n == NULL)
	return;

    n->e.tag = tag;
    strcpy(n->e.description, description);
    n->e.representation = representation;

    (void) LST_Enqueue(l, n);
}

typedef struct {
    DCM_VALUEREPRESENTATION r;
    char *translation;
}   MAP;

static char *
translate(DCM_VALUEREPRESENTATION r)
{
    static MAP map[] = {
	{DCM_AE, "AE"},
	{DCM_AS, "AS"},
	{DCM_AT, "AT"},
	{DCM_CS, "CS"},
	{DCM_DA, "DA"},
	{DCM_DD, "DD"},
	{DCM_DS, "DS"},
	{DCM_FD, "FD"},
	{DCM_FL, "FL"},
	{DCM_IS, "IS"},
	{DCM_LO, "LO"},
	{DCM_LT, "LT"},
	{DCM_OT, "OT"},
	{DCM_SH, "SH"},
	{DCM_SL, "SL"},
	{DCM_SQ, "SQ"},
	{DCM_SS, "SS"},
	{DCM_ST, "ST"},
	{DCM_TM, "TM"},
	{DCM_UI, "UI"},
	{DCM_UL, "UL"},
	{DCM_US, "US"},
	{DCM_UT, "UT"},
	{DCM_UN, "UN"},
	{DCM_RET, "RET"},
	{DCM_CTX, "CTX"},
	{DCM_PN, "PN"},
	{DCM_OB, "OB"},
	{DCM_OW, "OW"},
	{DCM_DT, "DT"}
    };
    int i;

    for (i = 0; i < (int) DIM_OF(map); i++) {
	if (r == map[i].r)
	    return map[i].translation;
    }
    return "??";
}
