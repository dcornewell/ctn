/*
          Copyright (C) 2002, RSNA and Washington University

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
** Author, Date:	Stephen M. Moore, 12-Feb-2002
** Intent:		This program reads a DICOM object and adds one
**			or more fragments to that object. Fragments are
**			used for encapsulated pixel data.
** Last Update:		$Author: smm $, $Date: 2003-03-11 20:24:15 $
** Source File:		$RCSfile: dcm_add_fragments.c,v $
** Revision:		$Revision: 1.4 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.4 $ $RCSfile: dcm_add_fragments.c,v $";
/*
**   Usage:
**	dcm_add_fragments [-t] [-x xfer] filein fileout f1 [f2 ...]
**
**  Author:
**	Stephen M. Moore (smm@wuerl.wustl.edu)
**	Electronic Radiology Lab
**	Mallinckrodt Institute of Radiology
**	Washington University School of Medicine
**	12-Feb-2002
**
*/

#include "ctn_os.h"

#include "dicom.h"
#include "ctnthread.h"
#include "condition.h"
#include "lst.h"
#include "dicom_objects.h"
#include "utility.h"

/*void cstore(DCM_OBJECT** obj, const char* host, int port);*/

static void
usageerror()
{
  char msg[] = "\
Usage: dcm_add_fragments [-t] [-x xfer] filein fileout f1 [f2...] \n\
\n\
  -t      Open as part 10 file \n\
  -x      Specify transfer syntax for output \n\
  filein  Input file \n\
  fileout Output file \n\
  f1      First fragment to add \n\
  f2      Zero or more fragments to add \n";

  fprintf(stderr, msg);
  exit(1);
}

main(int argc, char **argv)
{
  DCM_OBJECT * object;
  CONDITION cond;
  CTNBOOLEAN verbose = FALSE;
  U32 fileSize = 0;
  unsigned long
    options = DCM_ORDERLITTLEENDIAN,
    i;

  char* xferSyntax = 0;
  char* outputFile = 0;
  unsigned char* buffer;
  U32 bufferLength;
  int fd;
  int bytesRead;

  while (--argc > 0 && (*++argv)[0] == '-') {
    switch (*(argv[0] + 1)) {
    case 't':
      options &= ~DCM_FILEFORMATMASK;
      options |= DCM_PART10FILE;
      break;
    case 'v':
      verbose = TRUE;
      break;
    case 'x':
      argc--; argv++;
      if (argc < 1)
	usageerror();
      xferSyntax = *argv;
      break;
    default:
      break;
    }
  }

  THR_Init();
  DCM_Debug(verbose);
  if (argc < 3)
    usageerror();

  cond = DCM_OpenFile(*argv, options, &object);
  if (cond != DCM_NORMAL) {
    cond = DCM_OpenFile(*argv, DCM_PART10FILE, &object);
    if (cond != DCM_NORMAL) {
      COND_DumpConditions();
      THR_Shutdown();
      exit(1);
    }
  }
  argc--; argv++;
  outputFile = *argv++; argc--;

  cond = DCM_RemoveGroup(&object, 0x7fe0);
  if (cond != DCM_NORMAL) {
    (void)COND_PopCondition(TRUE);
  }

  buffer = 0;
  bufferLength = 0;

  while(argc-- > 0) {
    cond = UTL_FileSize(*argv, &fileSize);
    if (cond != UTL_NORMAL) {
      printf("Could not get file size for: %s\n", *argv);
      return 1;
    }

    printf(" Fragment: %10s %6d \n", *argv, fileSize);
    if (fileSize > bufferLength) {
      if (bufferLength != 0) {
	CTN_FREE(buffer);
      }
      buffer = CTN_MALLOC(fileSize);
      bufferLength = fileSize;
    }
    if (buffer == NULL) {
      fprintf(stderr, "Unable to malloc %d bytes\n", fileSize);
      return 1;
    }
    fd = open(*argv, O_RDONLY);
    if (fd < 0) {
      perror(*argv);
      return 1;
    }
    bytesRead = read(fd, buffer, fileSize);
    if (bytesRead != fileSize) {
      perror(*argv);
      return 1;
    }
    close(fd);

    cond = DCM_AddFragment(&object, buffer, fileSize);
    if (cond != DCM_NORMAL) {
      COND_DumpConditions();
      return 1;
    }
    argv++;
  }
  if (bufferLength != 0)
    CTN_FREE(buffer);

  /*DCM_DumpElements(&object, 1);*/

  if (DCM_WriteFile(&object, DCM_PART10FILE | DCM_ENCAPSULATEDPIXELS, outputFile) != DCM_NORMAL) {
    fprintf(stderr, "Error writing new DCM image file\n");
    COND_DumpConditions();
    THR_Shutdown();
    exit(1);
  }

  /*cstore(&object, "localhost", 2100);*/

  (void) DCM_CloseObject(&object);

  THR_Shutdown();
  return 0;
}
