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
** Module Name(s):	gqinitq (main())
** Author, Date:	David E. Beecher, 4-Jul-93
** Intent:		GQ queue creation utility.
** Last Update:		$Author: smm $, $Date: 1998-08-03 21:29:57 $
** Source File:		$RCSfile: gqinitq.c,v $
** Revision:		$Revision: 1.8 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.8 $ $RCSfile: gqinitq.c,v $";
#include <stdio.h>
#include <stdlib.h>
#include "dicom.h"
#include "ctnthread.h"
#include "condition.h"
#include "gq.h"

int
main(int argc, char *argv[])
{

    if (argc != 4) {
	printf("\nUsage: %s  <id> <queue size> <element size>\n\n", argv[0]);
	return (0);
    }
    THR_Init();
    if (GQ_InitQueue(atoi(argv[1]), atoi(argv[2]), atoi(argv[3])) != GQ_NORMAL) {
	printf("\nGQ_InitQueue failed!\n\n");
	COND_DumpConditions();
	THR_Shutdown();
	return (-1);
    }
    THR_Shutdown();
    return (0);
}
