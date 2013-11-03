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
** Author, Date:	Phillip DiCorpo, 16-Feb-2001
** Intent:		This program converts a DICOM file to an XML
**			representation.
**	
**   Usage:
**			dcm_iterator [-g] [-t] [-v] [-z] file [file ...]
** Last Update:		$Author: pd3 $, $Date: 2001-03-26 17:19:49 $
** Source File:		$RCSfile: dcm_to_xml.c,v $
** Revision:		$Revision: 1.1 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.1 $ $RCSfile: dcm_to_xml.c,v $";

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
#include "utility.h"

static void usageerror();
typedef struct {
  char path[1024];
} PATH_STRUCT;


static void
indentXML(size_t NL) {
	/*Indent with number of TABS equal to
		NL (Nesting Level)*/
	while(NL > 0) {
		printf("\t");
		NL--;
	}
}


static void dumpElement(DCM_ELEMENT* e)
{
  char txt[1024];
  int l;

  if (DCM_IsString(e->representation)) {
    l = (e->length < sizeof(txt)-1)? e->length: sizeof(txt)-1;
    strncpy(txt, e->d.string, l);
    txt[l] = '\0';
    printf("%s", txt);
  } else {
    printf("%-32s", e->description);
  }

	/* NEED TO INSERT CASE STATEMENT FOR VALUE REPRESENTATION ?*/
}

static void startXMLTag(DCM_ELEMENT* e, size_t NL)
{
  char tag[1024];
	char* space;

  /* Generate start tag based on element description */
  tag[0]='<';
  tag[1]='\0';
  strcat(tag, e->description);
  strcat(tag, ">");

	/* Substitute invalid characters in tag */
  space = strpbrk(tag, " ");
  while(space != 0) {
    space[0] = '_';
    space = strpbrk(space, " ");
  }
	space = strpbrk(tag, "'");
  while(space != 0) {
    space[0] = '_';
    space = strpbrk(space, "'");
  }

	/* If not a sequence element, then indent according to NL(Nesting Level)*/
	if(e->representation != DCM_SQ)
		indentXML(NL);

  /* Print the start tag */
  printf("%s", tag);

}

static void endXMLTag(DCM_ELEMENT* e)
{
  char tag[1024];
	char* space;

  /* Generate end tag based on element description */
  tag[0]='<';
	tag[1]='/';
  tag[2]='\0';
  strcat(tag, e->description);
  strcat(tag, ">");
  
	/* Substitute invalid characters in tag */
	space = strpbrk(tag, " ");
  while(space != 0) {
    space[0] = '_';
    space = strpbrk(space, " ");
  }
	space = strpbrk(tag, "'");
  while(space != 0) {
    space[0] = '_';
    space = strpbrk(space, "'");
  }

  /* Print the start tag */
  printf("%s", tag);
}


static void
iterateThroughElements(DCM_OBJECT** obj, size_t NL)
{
  CONDITION cond;
  DCM_ELEMENT	*e;
	DCM_SEQUENCE_ITEM *sq;

  cond = DCM_GetFirstElement(obj, &e);
  if (cond != DCM_NORMAL) {
    COND_DumpConditions();
    return;
  }

	if(e->representation == DCM_SQ){
		indentXML(NL);
		startXMLTag(e, NL);
		printf("\n");
		sq = (DCM_SEQUENCE_ITEM*)LST_Head(&e->d.sq);
		if (sq != NULL)
			(void)LST_Position(&e->d.sq, sq);

		while (sq != NULL) {
			iterateThroughElements(&sq->object, NL+1);
			sq = (DCM_SEQUENCE_ITEM*)LST_Next(&e->d.sq);
		}

		indentXML(NL);
		endXMLTag(e);
		printf("\n");
	}
	else{
		startXMLTag(e, NL);
		dumpElement(e);
		endXMLTag(e);
		printf("\n");
	}

  while (cond == DCM_NORMAL) {
    cond = DCM_GetNextElement(obj, &e);
    if (cond == DCM_NORMAL) {
			if(e->representation == DCM_SQ){
				indentXML(NL);
				startXMLTag(e, NL);
				printf("\n");
				sq = (DCM_SEQUENCE_ITEM*)LST_Head(&e->d.sq);
				if (sq != NULL)
					(void)LST_Position(&e->d.sq, sq);

				while (sq != NULL) {
					iterateThroughElements(&sq->object, NL+1);
					sq = (DCM_SEQUENCE_ITEM*)LST_Next(&e->d.sq);
				}

				indentXML(NL);
				endXMLTag(e);
				printf("\n");
			}
			else{
				startXMLTag(e, NL);
				dumpElement(e);
				endXMLTag(e);
				printf("\n");
			}
    }
  }
  if (cond != DCM_GETNEXTELEMENTCOMPLETE) {
    fprintf(stderr, "Unable to get all elements\n");
  }
}

main(int argc, char **argv)
{
  DCM_OBJECT
	* object;
    CONDITION
	cond;
		char *
	   fileInput;
    CTNBOOLEAN
	verbose = FALSE,
	exitFlag = FALSE,
	formatFlag = FALSE;
    unsigned long
        options = DCM_ORDERLITTLEENDIAN;
    long vmLimit = 0;
    LST_HEAD* fileNames = 0;
    UTL_FILEITEM* p = NULL;

		if (argc < 2)
			usageerror();
		else {
			argv++;
			fileInput = *argv;
		}

    THR_Init();
    DCM_Debug(verbose);

    cond = DCM_OpenFile(fileInput, options, &object);
    if (cond != DCM_NORMAL && ((options & DCM_PART10FILE) == 0)) {
	    COND_DumpConditions();
	    (void) DCM_CloseObject(&object);
	    (void) COND_PopCondition(TRUE);
	    fprintf(stderr, "Could not open %s as expected.  Trying Part 10 format.\n", p->path);
	    cond = DCM_OpenFile(p->path, options | DCM_PART10FILE, &object);
    }
    if (cond == DCM_NORMAL) {
			printf("<?xml version=\"1.0\" ?>\n");
			printf("<Structured_Report>\n");
	    iterateThroughElements(&object, 1);
			printf("</Structured_Report>\n");
    }
    COND_DumpConditions();
    (void) DCM_CloseObject(&object);
    (void) COND_PopCondition(TRUE);

    if (cond != DCM_NORMAL && exitFlag) {
	    THR_Shutdown();
	    exit(1);
    }

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
Usage: dcm_to_file file\n\
\n\
    file      The information object to be converted to XML\n";

    fprintf(stderr, msg);
    exit(1);
}
