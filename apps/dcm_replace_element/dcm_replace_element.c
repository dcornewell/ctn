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
** Author, Date:        Stephen M. Moore, 7-Jul-93
** Intent:		This program uses the DICOM package to open an DICOM
**			file and replace one element
**   Usage:
**	dcm_replace_element [-v] [-b] group element <element file> filein fileout
**   Options:
**	t	Part 10 file
**	v	verbose mode
**   Descriptions:
**	group		group number of the element
**	element		element number of the element
**	element file   	file containing new element data
**	filein		input file name
**	fileout		output file name
**
** Last Update:         $Author: smm $, $Date: 2000-11-09 19:44:14 $
** Source File:         $RCSfile: dcm_replace_element.c,v $
** Revision:            $Revision: 1.2 $
** Status:              $State: Exp $
*/

static char rcsid[] = "$Revision: 1.2 $ $RCSfile: dcm_replace_element.c,v $";

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifdef _WIN32
#include <io.h>
#endif

#include "dicom.h"
#include "ctnthread.h"
#include "lst.h"
#include "condition.h"
#include "dicom_objects.h"

static void usageerror();

static int fileSize(int fd)
{
  int status;
  struct stat im_stat;

  status = fstat(fd, &im_stat);
  if (status < 0)
    return status;
  else
    return im_stat.st_size;
}

static void replaceElement(DCM_OBJECT** object, DCM_TAG tag,
			   const char *f)
{
  int fd;
  int l;
  int bytesRead;
  unsigned char* p;
  DCM_ELEMENT e;
  CONDITION cond;

  fd = open(f, O_RDONLY);
  l = fileSize(fd);
  p = malloc(l);
  if (p == NULL) {
    perror("");
    exit(1);
  }

  bytesRead = read(fd, p, l);
  if (bytesRead != l) {
    perror(f);
    exit(1);
  }

  e.tag = tag;
  DCM_LookupElement(&e);
  e.length = l;
  e.d.ot = p;
  cond = DCM_AddElement(object, &e);
  if (cond != DCM_NORMAL) {
    COND_DumpConditions();
    exit(2);
  }
  free(p);
}

int
main(int argc, char **argv)
{
    DCM_OBJECT
    * object;
    CONDITION
	cond;
    CTNBOOLEAN
	verbose = FALSE;
    int
        group,
        element;
    char *elementFile;

    unsigned long
        options = DCM_ORDERLITTLEENDIAN;

    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
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

    if (argc < 5)
	usageerror();
    if (sscanf(*argv++, "%x", &group) != 1)
	usageerror();
    if (sscanf(*argv++, "%x", &element) != 1)
	usageerror();

    elementFile = *argv++;

    THR_Init();
    DCM_Debug(verbose);

    cond = DCM_OpenFile(*argv++, options, &object);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	return (1);
    }
    cond = DCM_RemoveElement(&object, DCM_MAKETAG(group, element));
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	return (1);
    }

    replaceElement(&object, DCM_MAKETAG(group, element), elementFile);

    cond = DCM_WriteFile(&object, options, *argv);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	return (1);
    }
    (void) DCM_CloseObject(&object);
    THR_Shutdown();
    return 0;
}

/* usageerror
**
** Purpose:
**	Display correct usage of the command along with all options
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
Usage: dcm_replace_element [-b] [-t] [-v] group element <element file> filein fileout\n\
\n\
  -b  Read file with big endian transfer syntax\n\
  -t  Read file with DICOM Part 10 file format\n\
  -v  Place DCM facility in verbose mode\n\
\n\
   group element   Tag of the element to remove\n\
   <element file>  File containing data to replace element\n\
   filein fileout  Input and output files\n";

    fprintf(stderr, msg);
    exit(1);
}
