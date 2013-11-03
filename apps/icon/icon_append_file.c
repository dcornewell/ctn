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
** Module Name(s):	extractAccessionNumber
**			extractSOPInstanceUID
**			extractPixelData
**			strdup
**			main
** Author, Date:	August 4th, 1993
** Intent:		This program allows for appending new
**			icon records to a icon table file.
**			A record consists of an Accession Number,
**			a SOPInstanceUID, and an image(icon).
** Last Update:		$Author: smm $, $Date: 2002-04-16 21:27:59 $
** Source File:		$RCSfile: icon_append_file.c,v $
** Revision:		$Revision: 1.17 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.17 $ $RCSfile: icon_append_file.c,v $";
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/file.h>
#ifdef SOLARIS
#include <sys/fcntl.h>
#endif
#include <sys/stat.h>
#include <string.h>
#include "dicom.h"
#include "ctnthread.h"
#include "lst.h"
#include "condition.h"
#include "dicom_objects.h"
#include "icon.h"
CTNBOOLEAN
verbose = FALSE;

#ifdef NO_STRDUP
static char *strdup(const char *src);
#endif

/* extractAccessionNumber
**
** Purpose
**	Extract the Accession Number from a DICOM image file.
**
** Parameter Dictionary:
**	file		Name of the file
**
** Return Values:
**	Accession Number if found, else NULL
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static
char *
extractAccessionNumber(char *file)
{
    DCM_OBJECT
    * object = NULL;
    DCM_ELEMENT
	element;
    CONDITION
	cond;

    cond = DCM_OpenFile(file, DCM_ORDERLITTLEENDIAN, &object);
    if (cond != DCM_NORMAL) {
	printf("DCM_Openfile failed on %s\n", file);
	if (verbose == TRUE)
	    COND_DumpConditions();
	return (NULL);
    }
    element.tag = DCM_IDACCESSIONNUMBER;
    element.d.string = (char *) malloc(DICOM_CS_LENGTH + 1);
    element.length = DICOM_CS_LENGTH + 1;
    cond = DCM_LookupElement(&element);
    if (cond != DCM_NORMAL) {
	printf("DCM_LookupElement failed\n");
	if (verbose == TRUE)
	    COND_DumpConditions();
	return (NULL);
    }
    cond = DCM_ParseObject(&object, &element, 1, NULL, 0, NULL);
    if (cond != DCM_NORMAL) {
	printf("DCM_ParseObject failed\n");
	if (verbose == TRUE)
	    COND_DumpConditions();
	return (NULL);
    }
    return (element.d.string);
}


/* extractSOPInstanceUID
**
** Purpose:
**	Extract the SOPInstanceUID from a DICOM image file.
**
** Parameter Dictionary:
**	file		Name of the file
**
** Return Values:
**	SOP Instance UID if found, else NULL
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static
char *
extractSOPInstanceUID(char *file)
{
    DCM_OBJECT
    * object = NULL;
    DCM_ELEMENT
	element;
    void
       *ctx = NULL;
    U32
	rtnlength;
    CONDITION
	cond;

    cond = DCM_OpenFile(file, DCM_ORDERLITTLEENDIAN, &object);
    if (cond != DCM_NORMAL) {
	printf("DCM_Openfile failed on %s\n", file);
	if (verbose == TRUE)
	    COND_DumpConditions();
	return (NULL);
    }
    element.tag = DCM_IDSOPINSTANCEUID;
    element.d.string = (char *) malloc(DICOM_UI_LENGTH + 1);
    element.length = DICOM_UI_LENGTH + 1;
    cond = DCM_LookupElement(&element);
    if (cond != DCM_NORMAL) {
	printf("DCM_LookupElement failed\n");
	if (verbose == TRUE)
	    COND_DumpConditions();
	return (NULL);
    }
    cond = DCM_GetElementValue(&object, &element, &rtnlength, &ctx);
    if (cond != DCM_NORMAL) {
	printf("DCM_GetElementValue failed\n");
	printf("rntlength = %d", rtnlength);
	if (verbose == TRUE)
	    COND_DumpConditions();
	return (NULL);
    }
    element.d.string[rtnlength] = '\0';
    return (element.d.string);
}


/* extractPixelData
**
** Purpose:
**	Extract the Pixel Data from a DICOM image file.
**
** Parameter Dictionary:
**	file		Name of the file
**
** Return Values:
**	Pixel data as a stream of unsigned characters
**	NULL if not found.
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static unsigned char
   *
extractPixelData(char *file)
{
    DCM_OBJECT
    * object = NULL;
    DCM_ELEMENT
	element;
    void
       *ctx = NULL;
    unsigned long
        rtnlength;
    CONDITION
	cond;
    static unsigned short
        r,
        c;
    static unsigned char
        pixels[ICON_SIZE],
       *p;
    static DCM_ELEMENT l[] = {
	{DCM_IMGROWS, DCM_US, "", 1, sizeof(r), (void *) &r},
	{DCM_IMGCOLUMNS, DCM_US, "", 1, sizeof(c), (void *) &c},
	{DCM_PXLPIXELDATA, DCM_UN, "", 1, sizeof(pixels), (void *) pixels}
    };
    unsigned short
        i,
        j;

    cond = DCM_OpenFile(file, DCM_ORDERLITTLEENDIAN, &object);
    if (cond != DCM_NORMAL) {
	printf("DCM_Openfile failed on %s\n", file);
	if (verbose == TRUE)
	    COND_DumpConditions();
	return (NULL);
    }
    cond = DCM_ParseObject(&object, l, (int) DIM_OF(l), NULL, 0, NULL);
    if (cond != DCM_NORMAL) {
	printf("DCM Parse Failed when getting pixel data\n");
	if (verbose == TRUE)
	    COND_DumpConditions();
	return (NULL);
    }
    p = (unsigned char *) malloc(ICON_SIZE);
    (void) memset(p, 0, ICON_SIZE);
    for (i = 0; i < r; i++) {
	for (j = 0; j < c; j++)
	    p[i * ICON_XSIZE + j] = pixels[i * c + j];
    }

    return p;
}

int
main(int argc, char *argv[])
{
    int
        iconfilesize;
    char
       *iconfile,
       *icon;
    char
       *accessionNumber,
       *SOPInstanceUID;
    unsigned char
       *pixeldata;
    CONDITION
	cond;
    ICON_STUDYICON
	studyicon;

    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'v':
	    verbose = TRUE;
	    break;
	default:
	    break;
	}
    }

    if (argc != 2) {
	printf("Usage: append_icon_file ICONfile icon\n");
	return (0);
    }
    THR_Init();
    DCM_Debug(verbose);
    iconfile = strdup(argv[0]);
    icon = strdup(argv[1]);
    accessionNumber = extractAccessionNumber(icon);
    SOPInstanceUID = extractSOPInstanceUID(icon);
    pixeldata = extractPixelData(icon);
    if (!accessionNumber || !SOPInstanceUID || !pixeldata) {
	THR_Shutdown();
	return (1);
    }
    printf("Accession Number = %s\n", accessionNumber);
    printf("SOPInstanceUID = %s\n", SOPInstanceUID);
    strcpy(studyicon.accessionNumber, accessionNumber);
    strcpy(studyicon.UID, SOPInstanceUID);
    memcpy(studyicon.Icon, pixeldata, ICON_SIZE);
    cond = ICON_AppendStudyIcon(iconfile, &studyicon);
    if (cond != ICON_NORMAL) {
	printf("ICON_AppendStudyIcon failed\n");
	if (verbose == TRUE)
	    COND_DumpConditions();
	THR_Shutdown();
	return (1);
    }
    (void) ICON_DumpStudyIcon(iconfile);
    THR_Shutdown();
    return 0;
}

#ifdef NO_STRDUP
/* strdup
**
** Purpose:
**	Duplicate a string
**
** Parameter Dictionary:
**	src		Source string to be duplicated
**
** Return Values:
**	Duplicate copy
**	NULL on any error condition
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static char *
strdup(const char *src)
{
    char
       *rslt;
    int
        l;

    l = strlen(src) + 1;
    rslt = malloc(l);
    if (rslt == NULL)
	return NULL;

    (void) strcpy(rslt, src);
    return rslt;
}
#endif
