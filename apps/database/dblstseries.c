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
** Module Name(s):	dblstseries (main())
**			printseries
** Author, Date:	David E. Beecher, 4-Jul-93
** Intent:		List the series in a DICOM database
** Last Update:		$Author: smm $, $Date: 1994-12-30 18:22:20 $
** Source File:		$RCSfile: dblstseries.c,v $
** Revision:		$Revision: 1.7 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.7 $ $RCSfile: dblstseries.c,v $";
#include <stdio.h>
#include "dicom.h"
#include "dbquery.h"

void printseries(SeriesLevel series, int i);

main(int argc, char *argv[])
{

    CONDITION
	ret;
    int
        i;
    short
        id;
    SeriesLevel
	series;
    char
        patid[50],
        studyuid[50],
        dbname[50];

    if (argc != 4) {
	printf("Usage %s PatID StudyUID dbname\n", argv[0]);
	exit(0);
    }
    strcpy(patid, argv[1]);
    strcpy(studyuid, argv[2]);
    strcpy(dbname, argv[3]);

    if (DB_Open(dbname, &id) == DB_NORMAL)
	printf("DB_Open succeeded\n");
    else {
	printf("DB_Open failed\n");
	exit(1);
    }
    i = 1;
    if (DB_GetSeries(id, patid, studyuid, &series) == DB_NORMAL) {
	printseries(series, i);
	while (DB_GetNextSeries(id, &series) == DB_NORMAL) {
	    i++;
	    printseries(series, i);
	}
    }
    if (DB_Close(id) == DB_NORMAL)
	printf("DB_Close succeeded\n");
    else
	exit(0);
}

/* printseries
**
** Purpose:
**	Dump the characteristics of the series record
**
** Parameter Dictionary:
**	series		Characteristics of the series record to be dumped
**	i		Series record number
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
void
printseries(SeriesLevel series, int i)
{

    printf("Series #%d\n", i);
    printf("        Modality: %s\n", series.Modality);
    printf("   Series Number: %s\n", series.SeriesNumber);
    printf("      Series UID: %s\n", series.SeriesUID);
    return;
}
