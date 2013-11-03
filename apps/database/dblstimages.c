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
** Module Name(s):	dblstimages (main())
**			printimage
** Author, Date:	David E. Beecher, 4-Jul-93
** Intent:		List the images in a DICOM database
** Last Update:		$Author: smm $, $Date: 1994-12-30 18:22:15 $
** Source File:		$RCSfile: dblstimages.c,v $
** Revision:		$Revision: 1.7 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.7 $ $RCSfile: dblstimages.c,v $";
#include <stdio.h>
#include "dicom.h"
#include "dbquery.h"

void printimage(ImageLevel image, int i);

main(int argc, char *argv[])
{

    CONDITION
	ret;
    int
        i;
    short
        id;
    ImageLevel
	image;
    char
        patid[50],
        studyuid[50],
        seriesuid[50],
        dbname[50];

    if (argc != 5) {
	printf("Usage %s PatID StudyUID SeriesUID dbname\n", argv[0]);
	exit(0);
    }
    strcpy(patid, argv[1]);
    strcpy(studyuid, argv[2]);
    strcpy(seriesuid, argv[3]);
    strcpy(dbname, argv[4]);

    if (DB_Open(dbname, &id) == DB_NORMAL)
	printf("DB_Open succeeded\n");
    else {
	printf("DB_Open failed\n");
	exit(1);
    }
    i = 1;
    if (DB_GetImage(id, patid, studyuid, seriesuid, &image) == DB_NORMAL) {
	printimage(image, i);
	while (DB_GetNextImage(id, &image) == DB_NORMAL) {
	    i++;
	    printimage(image, i);
	}
    }
    if (DB_Close(id) == DB_NORMAL)
	printf("DB_Close succeeded\n");
    else
	exit(0);
}

/* printimage
**
** Purpose:
**	Dump the image characteristics
**
** Parameter Dictionary:
**	image		Characteristics of the image to be dumped
**	i		image number
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
printimage(ImageLevel image, int i)
{

    printf("Image #%d\n", i);
    printf("Image Number: %s\n", image.ImageNumber);
    printf("   Image UID: %s\n", image.ImageUID);
    printf("   Class UID: %s\n", image.ClassUID);
    printf("    FileName: %s\n\n", image.FileName);
    return;
}
