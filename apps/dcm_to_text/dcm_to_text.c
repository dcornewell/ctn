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
** Intent:		This program converts a DICOM file to a text file that
**						can later be read by dcm_create_object ot recreate the object
**	
**   Usage:
**			dcm_to text file
** Last Update:		$Author: pd3 $, $Date: 2001-03-26 17:18:12 $
** Source File:		$RCSfile: dcm_to_text.c,v $
** Revision:		$Revision: 1.1 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.1 $ $RCSfile: dcm_to_text.c,v $";

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
indentText(size_t NL) {
	/*Indent with number of TABS equal to
		NL (Nesting Level)*/
	while(NL > 0) {
		printf("\t");
		NL--;
	}
}


static void dumpElement(DCM_ELEMENT* e)
{
	char line[1024];
  char txt[1024];
  int l;

	line[0]='"';
	line[1]='\0';
  if (DCM_IsString(e->representation)) {
    l = (e->length < sizeof(txt)-1)? e->length: sizeof(txt)-1;
    strncpy(txt, e->d.string, l);
    txt[l] = '\0';

		/* Remove spaces from the end */
		l = strlen(txt);
		l--;
		while( (l>0) && (txt[l] == ' '))
		{
			txt[l] = '\0';
			l--;
		}
		/* Check for spaces in string, and add quotes */
		if(strpbrk(txt, " ") != 0) {
			strcat(line, txt);
			strcat(line, "\"");
			printf("%s", line);
		}
		else
			printf("%s", txt);
  } else {
    printf("%-32s", e->description);
  }

/* NEED TO INSERT CASE STATEMENT FOR VALUE REPRESENTATION ? */

}

static void startTextLine(DCM_ELEMENT* e, size_t NL)
{
  char line[1024];
	line[0] = '\0';

  /* Generate text line based on element description */
  strcat(line, "\t");

	/* If not a sequence element, then indent according to NL(Nesting Level)*/
	if((e->representation != DCM_SQ))
		indentText(NL);

  /* Print the attribute tag */
	printf("%04x", DCM_TAG_GROUP(e->tag));
	printf(" ");
	printf("%04x", DCM_TAG_ELEMENT(e->tag));
  printf("%s", line);
}

static void endTextLine(DCM_ELEMENT* e)
{
  char line[1024];
	line[0] = '\0';

  /* Generate end of text line (comment) based on element description */
	strcat(line, "\t//");
  strcat(line, e->description);
  
  /* Print the start tag */
  printf("%s", line);
	printf("\n");
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
		indentText(NL);
		startTextLine(e, NL);
		sq = (DCM_SEQUENCE_ITEM*)LST_Head(&e->d.sq);
		if (sq != NULL)
			(void)LST_Position(&e->d.sq, sq);
		if (sq == NULL){
			printf("####\t");
			endTextLine(e);
		}
		else {
			while (sq != NULL) {
				indentText(NL);
				printf("(\t");
				endTextLine(e);
				iterateThroughElements(&sq->object, NL+1);
				sq = (DCM_SEQUENCE_ITEM*)LST_Next(&e->d.sq);
				indentText(NL);
				printf("%s",")\n");
			}
		}
	}
	else{
		startTextLine(e, NL);
		dumpElement(e);
		endTextLine(e);
	}

  while (cond == DCM_NORMAL) {
    cond = DCM_GetNextElement(obj, &e);
    if (cond == DCM_NORMAL) {
			if(e->representation == DCM_SQ){
				indentText(NL);
				startTextLine(e, NL);
				sq = (DCM_SEQUENCE_ITEM*)LST_Head(&e->d.sq);
			if (sq != NULL)
				(void)LST_Position(&e->d.sq, sq);
			if (sq == NULL){
				printf("####\t");
				endTextLine(e);
			}
			else {
				while (sq != NULL) {
					indentText(NL);
					printf("(\t");
					endTextLine(e);
					iterateThroughElements(&sq->object, NL+1);
					sq = (DCM_SEQUENCE_ITEM*)LST_Next(&e->d.sq);
					indentText(NL);
					printf("%s",")\n");
				}
			}
		}
		else{
			startTextLine(e, NL);
			dumpElement(e);
			endTextLine(e);
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
	    iterateThroughElements(&object, 0);
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
Usage: dcm_to_text file\n\
\n\
    file      The information object to be converted to text\n";

    fprintf(stderr, msg);
    exit(1);
}
