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
** Module Name(s):
** Author, Date:	David E. Beecher
** Intent:		Test program for Sybase TBL functions
** Last Update:		$Author: smm $, $Date: 1998-08-03 16:28:00 $
** Source File:		$RCSfile: ttunique.c,v $
** Revision:		$Revision: 1.7 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.7 $ $RCSfile: ttunique.c,v $";

#include <stdio.h>
#include <stdlib.h>
#include "dicom.h"
#include "ctnthread.h"
#include "condition.h"
#include "tbl.h"

main()
{
    CONDITION
	cond;
    TBL_HANDLE
	* handle;
    int
        i,
        count;

    THR_Init();
    cond = TBL_Open("TBLTest", "UniqueNumbers", &handle);
    if (cond != TBL_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    for (i = 0; i < 10; i++) {
	cond = TBL_NextUnique(&handle, "UN1", &count);
	if (cond != TBL_NORMAL) {
	    COND_DumpConditions();
	    THR_Shutdown();
	    exit(2);
	}
	printf("UN1 iteration: %d  count: %d\n", i, count);
    }

    for (i = 0; i < 10; i++) {
	cond = TBL_NextUnique(&handle, "UN2", &count);
	if (cond != TBL_NORMAL) {
	    COND_DumpConditions();
	    THR_Shutdown();
	    exit(2);
	}
	printf("UN2 iteration: %d  count: %d\n", i, count);
    }

    for (i = 0; i < 10; i++) {
	cond = TBL_NextUnique(&handle, "UN3", &count);
	if (cond != TBL_NORMAL) {
	    COND_DumpConditions();
	    THR_Shutdown();
	    exit(2);
	}
	printf("UN3 iteration: %d  count: %d\n", i, count);
    }

    THR_Shutdown();
    exit(0);
}
