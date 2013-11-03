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
** Module Name(s):	dbquerytest (main())
**			printpat
**			printstudy
**			printseries
**			printimage
** Author, Date:	David E. Beecher, 4-Jul-93
** Intent:		Code example for using the query mechanism.
** Last Update:		$Author: smm $, $Date: 1994-12-30 18:22:28 $
** Source File:		$RCSfile: dbquerytest.c,v $
** Revision:		$Revision: 1.7 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.7 $ $RCSfile: dbquerytest.c,v $";
#include <stdio.h>
#include "dicom.h"
#include "dbquery.h"

void printpat(PatientLevel pat, int i);
void printstudy(StudyLevel study, int i);
void printseries(SeriesLevel series, int i);
void printimage(ImageLevel image, int i);

main(int argc, char *argv[])
{

    CONDITION
	ret;
    int
        i;
    short
        id;
    PatientLevel
	pat;
    char
        patid[50],
        dbname[50];

    Query
	spec, info;

    spec.QueryState = DB_K_CLASSPAT;
    spec.QueryState |= DB_K_LEVELIMAGE;

    spec.Patient.Query_Flag = 0;
/*
    spec.Patient.Query_Flag |= DB_K_QNAME;
    spec.Patient.Query_Flag |= DB_K_QBIRTHDATE;
    spec.Patient.Query_Flag |= DB_K_QID;
*/
    spec.Study.Query_Flag = 0;
/*
    spec.Study.Query_Flag |= DB_K_QSTUDYDATE;
    spec.Study.Query_Flag |= DB_K_QSTUDYTIME;
    spec.Study.Query_Flag |= DB_K_QSTUDYID;
    spec.Study.Query_Flag |= DB_K_QACCESSIONNUMBER;
    spec.Study.Query_Flag |= DB_K_QSTUDYUID;
    spec.Study.Query_Flag |= DB_K_QREFERRINGPHYSNAME;
    spec.Study.Query_Flag |= DB_K_QINTERPRETINGPHYSNAME;
    spec.Study.Query_Flag |= DB_K_QPROCEDUREDESCRIPTION;
*/
    spec.Study.Query_Flag |= DB_K_QADMITTINGDIAGNOSEDDESCRIPTION;

    spec.Series.Query_Flag = 0;
/*
    spec.Series.Query_Flag |= DB_K_QMODALITY;
    spec.Series.Query_Flag |= DB_K_QSERIESNUMBER;
    spec.Series.Query_Flag |= DB_K_QSERIESUID;
*/
    spec.Image.Query_Flag = 0;
/*
    spec.Image.Query_Flag |= DB_K_QIMAGENUMBER;
    spec.Image.Query_Flag |= DB_K_QIMAGEUID;
    spec.Image.Query_Flag |= DB_K_QCLASSUID;
    spec.Image.Query_Flag |= DB_K_QIMAGEMULTUID;
*/
    strcpy(spec.Patient.Name, "*");
    strcpy(spec.Patient.PatID, "");
    strcpy(spec.Patient.BirthDate, "");

    strcpy(spec.Study.StudyDate, "");
    strcpy(spec.Study.StudyTime, "");
    strcpy(spec.Study.StudyID, "");
    strcpy(spec.Study.AccessionNumber, "");
    strcpy(spec.Study.StudyUID, "");
    strcpy(spec.Study.ReferringPhysName, "");
    strcpy(spec.Study.InterpretingPhysName, "");
    strcpy(spec.Study.ProcedureDescription, "");
    strcpy(spec.Study.AdmittingDiagnosedDescription, "*D*");

    strcpy(spec.Series.Modality, "");
    strcpy(spec.Series.SeriesNumber, "");
    strcpy(spec.Series.SeriesUID, "");

    strcpy(spec.Image.ImageNumber, "");
    strcpy(spec.Image.ImageUID, "");
    strcpy(spec.Image.ClassUID, "");

    if (DB_Open("db.NEW.DEC", &id) == DB_NORMAL)
	printf("DB_Open succeeded\n");
    else {
	printf("DB_Open failed\n");
	exit(1);
    }
    if (DB_Query(id, &spec, &info) == DB_NORMAL) {
	i = 1;
	printpat(info.Patient, i);
	if (!(spec.QueryState & DB_K_LEVELPAT)) {
	    printstudy(info.Study, i);
	    if (!(spec.QueryState & DB_K_LEVELSTUDY)) {
		printseries(info.Series, i);
		if (!(spec.QueryState & DB_K_LEVELSERIES)) {
		    printimage(info.Image, i);
		}
	    }
	}
	while (DB_NextQuery(id, &spec, &info) == DB_NORMAL) {
	    i++;
	    printpat(info.Patient, i);
	    if (!(spec.QueryState & DB_K_LEVELPAT)) {
		printstudy(info.Study, i);
		if (!(spec.QueryState & DB_K_LEVELSTUDY)) {
		    printseries(info.Series, i);
		    if (!(spec.QueryState & DB_K_LEVELSERIES)) {
			printimage(info.Image, i);
		    }
		}
	    }
	}
    }
    if (DB_Close(id) == DB_NORMAL)
	printf("DB_Close succeeded\n");
    else
	exit(0);
}

/* printpat
**
** Purpose:
**	Print characteristics of patient record
**
** Parameter Dictionary:
**	pat		Patient level record
**	i		Patient number
**
** Return Values:
**	NOne
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
void
printpat(PatientLevel pat, int i)
{

    printf("Patient #%d\n", i);
    printf("            Birth: %s\n", pat.BirthDate);
    printf("             Name: %s\n", pat.Name);
    printf("            PatID: %s\n\n", pat.PatID);
    return;
}

/* printstudy
**
** Purpose:
**	Print characteristics of the study record
**
** Parameter Dictionary:
**	study		Study level record
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
    printf("       Diagnosed: %s\n\n", study.AdmittingDiagnosedDescription);
    return;
}

/* printseries
**
** Purpose:
**	Dump the characteristics of series level record
**
** Parameter Dictionary:
**	series		Series level record
**	i		Series number
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

/* printimage
**
** Purpose:
**	Dump the image level record characteristics
**
** Parameter Dictionary:
**	image		Image level record
**	i		Image number
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
