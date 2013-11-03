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
** Module Name(s):	fdFileSize
**			getFileSize
**			extractAccessionNumber
**			strdup
**			main
** Author, Date:	Tze Khiong Lee, Aug 4th 1993
** Intent:		This program will allow for appending a
**			new record in an icon index file.  A record
**			consists of an Accession Number and an offset
**			location of the icon existing in the icon table file.
** Last Update:		$Author: smm $, $Date: 1998-08-03 21:30:04 $
** Source File:		$RCSfile: icon_append_index.c,v $
** Revision:		$Revision: 1.15 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.15 $ $RCSfile: icon_append_index.c,v $";
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

/* fileSize
**
** Purpose:
**      Determine the file size of a file on an open descriptor.
**
** Parameter Dictionary:
**      fd      File descriptor for an open file.
**
** Return Values:
**      the size of the open file in bytes (nonnegative)
**      a negative status value returned by fstat
**
** Algorithm:
**      call unix fstat system call to get file size.
**      if successful call
**          return file size
**      else
**          return status returned by fstat call (-1)
*/

static int
fdFileSize(int fd)
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


/* getFileSize
**
** Purpose:
**	Get the size of a file.
**
** Parameter Dictionary:
**	file		Name of the file
**
** Return Values:
**	Size of the file
**	-1 on error
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static int
getFileSize(char *file)
{
    int
        fd,
        size;
    if ((fd = open(file, O_RDONLY)) < 0)
	fd = open(file, O_RDONLY | O_CREAT, 0666);
    if (fd < 0) {
	printf("Failed to open/create %s\n", file);
	return (-1);
    }
    size = fdFileSize(fd);
    close(fd);
    if (size < 0) {
	printf("Failed to get size of %s\n", file);
	return (-1);
    }
    return (size);
}


/* extractAccessionNumber
**
** Purpose:
**	Extract the Accession number from a DICOM image file.
**
** Parameter Dictionary:
**	file		Name of the file
**
** Return Values:
**	Accession number if found, else NULL
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
	printf("DCM Parse Object failed\n");
	if (verbose == TRUE)
	    COND_DumpConditions();
	return (NULL);
    }
    return (element.d.string);
}

int
main(int argc, char *argv[])
{
    int
        iconfilesize;
    char
       *iconindex,
       *iconfile,
       *imagefile;
    char
       *accessionNumber;
    CONDITION
	cond;
    ICON_STUDYOFFSET
	studyoffset;

    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'v':
	    verbose = TRUE;
	    break;
	default:
	    break;
	}
    }
    DCM_Debug(verbose);
    if (argc != 3) {
	printf("Usage: append_icon_index ICONCindex ICONfile imagefilei\n");
	return (0);
    }
    THR_Init();
    iconindex = strdup(argv[0]);
    iconfile = strdup(argv[1]);
    imagefile = strdup(argv[2]);
    iconfilesize = getFileSize(iconfile);
    if (iconfilesize < 0) {
	THR_Shutdown();
	return (1);
    }
    printf("Size of %s is %d\n", iconfile, iconfilesize);
    accessionNumber = extractAccessionNumber(imagefile);
    if (accessionNumber == NULL) {
	THR_Shutdown();
	return (1);
    }
    printf("Accession Number = %s\n", accessionNumber);
    strcpy(studyoffset.accessionNumber, accessionNumber);
    studyoffset.Offset = iconfilesize;
    cond = ICON_AppendStudyOffset(iconindex, &studyoffset);
    if (cond != ICON_NORMAL) {
	printf("ICON_AppendStudyOffset failed\n");
	if (verbose == TRUE)
	    COND_DumpConditions();
	THR_Shutdown();
	return (1);
    }
    (void) ICON_DumpStudyOffset(iconindex);
    THR_Shutdown();
    return 0;
}
#ifdef NO_STRDUP
/* strdup
**
** Purpose:
**      Duplicate a string
**
** Parameter Dictionary:
**      src             Source string to be duplicated
**
** Return Values:
**      Duplicate copy
**      NULL on any error condition
**
** Notes:
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
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
