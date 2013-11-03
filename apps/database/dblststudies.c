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
** Module Name(s):	dblststudies (main())
**			printstudy
** Author, Date:	David E. Beecher, 4-Jul-93
** Intent:		List the studies in a DICOM database
** Last Update:		$Author: smm $, $Date: 1994-12-30 18:22:21 $
** Source File:		$RCSfile: dblststudies.c,v $
** Revision:		$Revision: 1.7 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.7 $ $RCSfile: dblststudies.c,v $";
#include <stdio.h>
#include "dicom.h"
#include "dbquery.h"

void printstudy(StudyLevel study, int i);

main(int argc, char *argv[])
{

    CONDITION
	ret;
    int
        i;
    short
        id;
    StudyLevel
	study;
    char
        patid[50],
        dbname[50];

    if (argc != 3) {
	printf("Usage %s PatID dbname\n", argv[0]);
	exit(0);
    }
    strcpy(patid, argv[1]);
    strcpy(dbname, argv[2]);

    if (DB_Open(dbname, &id) == DB_NORMAL)
	printf("DB_Open succeeded\n");
    else {
	printf("DB_Open failed\n");
	exit(1);
    }
    i = 1;
    if (DB_GetStudy(id, patid, &study) == DB_NORMAL) {
	printstudy(study, i);
	while (DB_GetNextStudy(id, &study) == DB_NORMAL) {
	    i++;
	    printstudy(study, i);
	}
    }
    if (DB_Close(id) == DB_NORMAL)
	printf("DB_Close succeeded\n");
    else
	exit(0);
}

/* printstudy
**
** Purpose:
**	Dump the study level record characteristics
**
** Parameter Dictionary:
**	study		Study level record whose characteristics are to
**			be dumped
**	i		Study number
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
printstudy(StudyLevel study, int i)
{

    printf("Study #%d\n", i);
    printf("      Study Date: %s\n", study.StudyDate);
    printf("      Study Time: %s\n", study.StudyTime);
    printf("        Study ID: %s\n", study.StudyID);
    printf("Accession Number: %s\n", study.AccessionNumber);
    printf("       Study UID: %s\n", study.StudyUID);
    printf("       Referring: %s\n", study.ReferringPhysName);
    printf("    Interpreting: %s\n", study.InterpretingPhysName);
    printf("       Procedure: %s\n", study.ProcedureDescription);
    printf("       Diagnosis: %s\n\n", study.AdmittingDiagnosedDescription);
    return;
}
