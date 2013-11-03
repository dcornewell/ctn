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
**				DICOM 94
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	Stephen Moore, 11-Feb-94
** Intent:
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:37:45 $
** Source File:		$RCSfile: test_storage.c,v $
** Revision:		$Revision: 1.3 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.3 $ $RCSfile: test_storage.c,v $";

#include "ctn_os.h"

#if 0
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
#include <sys/wait.h>
#ifdef MALLOC_DEBUG
#include "malloc.h"
#endif
#endif

#include "dicom.h"
#include "tbl.h"
#include "lst.h"
#include "dicom_uids.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "tbl.h"
#include "manage.h"
#include "idb.h"

#include "image_server.h"

static void usageerror();
main(int argc, char **argv)
{
    CONDITION cond;
    DCM_OBJECT *object;
    IDB_Insertion ins;

    while (--argc > 0 && *(++argv)[0] == '-') {
	switch ((*argv)[1]) {
	default:
	    printf("Unrecognized option: %s\n", *argv);
	    break;
	}
    }

    if (argc < 1)
	usageerror();

    cond = DCM_OpenFile(*argv, DCM_ORDERLITTLEENDIAN | DCM_FORMATCONVERSION,
			&object);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
    cond = parseImageForInsert(&object, &ins);
    COND_DumpConditions();
}

static void
usageerror()
{
    char msg[] = "\
Usage: test_storage file\n\
\n\
    file  File containing DICOM image\n";

    fprintf(stderr, msg);
    exit(1);
}
