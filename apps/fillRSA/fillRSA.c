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
+-+-+-+-+-+-+-+-+-
*/
/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	main
**			usageerror()
** Author, Date:	Stephen Moore, 15-Oct-94
** Intent:		This is the "main" program used to fill the
**			information system for the Results Storage
**			Application (RSA).  It does this by extracting the
**			set of studies and patients from an image database
**			and placing appropriate entries in our FIS.
** Last Update:		$Author: smm $, $Date: 1998-08-03 21:29:30 $
** Source File:		$RCSfile: fillRSA.c,v $
** Revision:		$Revision: 1.7 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.7 $ $RCSfile: fillRSA.c,v $";

#include <stdio.h>
#include <sys/types.h>
#include <sys/param.h>
#ifdef SOLARIS
#include <netdb.h>
#endif
#include <stdlib.h>
#include <string.h>
#ifdef MACH
#include <unistd.h>
#endif
#ifdef MALLOC_DEBUG
#include "malloc.h"
#endif

#include "dicom.h"
#include "ctnthread.h"
#include "condition.h"
#include "tbl.h"
#include "lst.h"
#include "tbl.h"
#include "manage.h"
#include "idb.h"
#include "dicom_objects.h"
#include "fis.h"

extern CONDITION work(IDB_HANDLE ** idb, FIS_HANDLE ** fis);
static void usageerror();

char *retrieveAETitle = NULL;

int
main(int argc, char **argv)
{
    CONDITION cond;
    static IDB_HANDLE *IDBHandle;
    static FIS_HANDLE *FISHandle;
    CTNBOOLEAN verboseTBL = FALSE;

    while (--argc > 0 && *(++argv)[0] == '-') {
	switch ((*argv)[1]) {
	case 'v':
	    break;
	case 'r':
	    if (argc < 1)
		usageerror();
	    argc--;
	    retrieveAETitle = *++argv;
	    break;
	case 'x':
	    if (argc < 1)
		usageerror();
	    argc--;
	    argv++;
	    if (strcmp(*argv, "TBL") == 0)
		verboseTBL = TRUE;
	    else
		usageerror();
	    break;
	default:
	    printf("Unrecognized option: %s\n", *argv);
	    break;
	}
    }

    if (argc < 2)
	usageerror();

/*  Open the image database and FIS tables.  Once they are open, call
**  a high-level function which does the work.
**/

    THR_Init();
    (void) TBL_Debug(verboseTBL);
    cond = IDB_Open(*argv, &IDBHandle);
    if (cond != IDB_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	return (1);
    }
    cond = FIS_Open(*++argv, &FISHandle);
    if (cond != FIS_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	return (1);
    }
    cond = work(&IDBHandle, &FISHandle);
    if (cond != 1)
	COND_DumpConditions();

    THR_Shutdown();
    return 0;
}

static void
usageerror()
{
    char msg[] = "\
Usage: [-r title] [-x fac] image-database FIS-database\n\
\n\
    -r title    Enter title as retrieve AE title at series level\n\
    -x fac      Place fac in verbose mode (TBL)\n";

    fprintf(stderr, msg);
    exit(1);
}
