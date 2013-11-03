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
** Module Name(s):	enq_ctnnetwork (main())
** Author, Date:	David E. Beecher, 6-Jul-93
** Intent:		Utility routine to enqueue an element to a ctndisp queue
** Last Update:		$Author: smm $, $Date: 1998-08-03 16:09:10 $
** Source File:		$RCSfile: enq_ctnnetwork.c,v $
** Revision:		$Revision: 1.8 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.8 $ $RCSfile: enq_ctnnetwork.c,v $";
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dicom.h"
#include "ctnthread.h"
#include "condition.h"
#include "iqueues.h"
#include "gq.h"

main(int argc, char *argv[])
{

    CONDITION r;

    CTNNETWORK_Queue e;
    int qid;

    if (argc != 7) {
	printf("\nUsage: %s <qid> <vendorid> <dpnid> <association id> <conn> <percentage>\n", argv[0]);
	exit(1);
    }
    THR_Init();
    qid = atoi(argv[1]);
    if (GQ_GetQueue(qid, sizeof(CTNNETWORK_Queue)) != GQ_NORMAL) {
	printf("\nGQ_GetQueue failed!\n\n");
	COND_DumpConditions();
	THR_Shutdown();
	exit(-1);
    }
    strcpy(e.vendorid, argv[2]);
    strcpy(e.dpnid, argv[3]);
    e.association_id = atoi(argv[4]);
    e.connection = atoi(argv[5]);
    e.percentage = atoi(argv[6]);
    if ((r = GQ_Enqueue(qid, (void *) &e)) != GQ_NORMAL) {
	if (r == GQ_QUEUEFULL)
	    printf("\nThe queue is full!\n\n");
	else {
	    printf("\nGQ_Enqueue failed\n\n");
	    COND_DumpConditions();
	    THR_Shutdown();
	    exit(-1);
	}
    }
    THR_Shutdown();
    return 0;
}
