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
** Module Name(s):	dbaddstudy (main())
** Author, Date:	David E. Beecher, 4-Jul-93
** Intent:		Add a study to a DICOM database
** Last Update:		$Author: smm $, $Date: 1994-12-30 18:21:57 $
** Source File:		$RCSfile: dbaddstudy.c,v $
** Revision:		$Revision: 1.6 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.6 $ $RCSfile: dbaddstudy.c,v $";
#include <stdio.h>
#include "dicom.h"
#include "dbquery.h"

main(int argc, char *argv[])
{

    CONDITION
	ret;
    short
        id;
    StudyLevel
	study;
    char
        patid[50],
        dbname[50];

    if (argc != 12) {
	printf("Usage: %s StudyDate StudyTime StudyID AcNum StudyUID RefP IntP ProcDes AdmDes PatID dbname\n",
	       argv[0]);
	exit(0);
    }
    strcpy(study.StudyDate, argv[1]);
    strcpy(study.StudyTime, argv[2]);
    strcpy(study.StudyID, argv[3]);
    strcpy(study.AccessionNumber, argv[4]);
    strcpy(study.StudyUID, argv[5]);
    strcpy(study.ReferringPhysName, argv[6]);
    strcpy(study.InterpretingPhysName, argv[7]);
    strcpy(study.ProcedureDescription, argv[8]);
    strcpy(study.AdmittingDiagnosedDescription, argv[9]);
    strcpy(patid, argv[10]);
    strcpy(dbname, argv[11]);

    printf("      StudyDate: %s\n", study.StudyDate);
    printf("      StudyTime: %s\n", study.StudyTime);
    printf("        StudyID: %s\n", study.StudyID);
    printf("AccessionNumber: %s\n", study.AccessionNumber);
    printf("       StudyUID: %s\n", study.StudyUID);
    printf("           RefP: %s\n", study.ReferringPhysName);
    printf("           IntP: %s\n", study.InterpretingPhysName);
    printf("        ProcDes: %s\n", study.ProcedureDescription);
    printf("         AdmDes: %s\n", study.AdmittingDiagnosedDescription);
    printf("     patient id: %s\n", patid);
    printf("        DB Name: %s\n", dbname);

    if (DB_Open(dbname, &id) == DB_NORMAL)
	printf("DB_Open succeeded\n");
    else {
	printf("DB_Open failed\n");
	exit(1);
    }

    if ((ret = DB_AddStudy(id, patid, &study)) == DB_NORMAL)
	printf("%s succeeded\n", argv[0]);
    else
	printf("Another error occurred: %08x\n", ret);

    if (DB_Close(id) == DB_NORMAL)
	printf("DB_Close succeeded\n");
    else
	exit(0);
}
