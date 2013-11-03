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
**                              DICOM 93
**                   Electronic Radiology Laboratory
**                 Mallinckrodt Institute of Radiology
**              Washington University School of Medicine
**
** Module Name(s):      main
**			add_image
**			dump_query
**			Cprintf
**			callback
**			Cprint_errors
** Author, Date:        Sheldon Hoffman, 14 Jun 93
** Intent:              Utility program for DB files
** Last Update:         $Author: smm $, $Date: 2001-12-21 16:37:31 $
** Source File:         $RCSfile: dbfill.c,v $
** Revision:            $Revision: 1.14 $
** Status:              $State: Exp $
*/

static char rcsid[] = "$Revision: 1.14 $ $RCSfile: dbfill.c,v $";

#include "ctn_os.h"

#if 0
#include    <stdio.h>
#include    <stdlib.h>
#include    <memory.h>
#include    <time.h>
#include    <stdarg.h>

#if MSDOS
#else
#include    <sys/param.h>
#endif
#endif

#include    "dbfill.h"

static short add_image(short, char *);
static void dump_query(Query *);
static void Cprintf(int, char *,...);
static void Cprint_errors(int);

static char *help[] = {
    "---------------------------------------------------------------------------",
    "| DBFILL - Utility program for creating/populating DB databases.     V 1.2|",
    "---------------------------------------------------------------------------",
    "| Usage: dbfill [switches] DBFILE [files..]                               |",
    "|                      Duplicate images are first deleted.                |",
    "|                      Patient, Study and Series are added as needed.     |",
    "|                                                                         |",
    "| <DBFILE>             Name of a DB database file                         |",
    "| Switches:                                                               |",
    "|    -p                Prepend current directory to the file's path       |",
    "|   files..            One or more DICOM (DCM) image files                |",
    "|                                                                         |",
    "| The  DBFILE  is created, if not already present.                        |",
    "---------------------------------------------------------------------------",
0};

static char *dbfile;		/* File name of DB file */
static short addflag = 0;	/* =1 iff -a specified */
static char current_dir[MAXPATHLEN];	/* Current directory */

/*------------------------------------------------------------------------*/
int
main(int ac, char *av[])
{
    CONDITION cond;		/* Condition return code */
    short dbid;			/* A DB Database id */
    short arg;			/* Index to command line arguments */

    current_dir[0] = '\0';
    addflag = 1;		/* multiple image files can be input */
    for (arg = 1; arg < ac; arg++) {
	if (av[arg][0] != '-')
	    break;

	switch (tolower(av[arg][1])) {
	case 'p':

	    if (!getcwd(current_dir, sizeof(current_dir))) {
		Cprintf(DIAG_ALWAYS,
			"Unable to determine current working directory\n");
		return (0);
	    }
	    strcat(current_dir, "/");
	    break;
	default:
	    Cprintf(DIAG_ALWAYS,
		    "Unknown switch on command line '%s'\n", av[arg]);
	    goto QUIT;
	}
    }

    if ((ac - arg) <= 1) {
	char **hlp;

	for (hlp = help; *hlp; hlp++)
	    printf("%s\n", *hlp);
	return (0);
    }
    dbfile = av[arg++];		/* Remember dbfile name for later */

/* OPEN/CREATE DATABASE */

    cond = DB_Open(dbfile, &dbid);
    if (cond != DB_NORMAL) {
	cond = DB_CreateDB(dbfile);
	if (cond != DB_NORMAL) {
	    Cprintf(DIAG_SEVERE, "Error creating database %s\n", dbfile);
	    goto QUIT;
	}
	Cprintf(DIAG_ALWAYS, "DB file %s created.\n", dbfile);
	cond = DB_Open(dbfile, &dbid);
	if (cond != DB_NORMAL) {
	    Cprintf(DIAG_SEVERE, "Error opening database %s\n", dbfile);
	    goto QUIT;
	}
	COND_PopCondition(TRUE);
    }
    if (addflag) {
	short i;

	for (i = 0; arg < ac; arg++) {
	    char fileName[MAXPATHLEN];

	    sprintf(fileName, "%s%s", current_dir, av[arg]);
	    if (access(fileName, 04))	/* We we have read access ? */
		Cprintf(DIAG_SEVERE, "No such file: %s\n", fileName);
	    else if (!add_image(dbid, fileName))
		i++;
	    else {
		Cprintf(DIAG_SEVERE, "Error adding image %s\n", fileName);
		Cprint_errors(DIAG_SEVERE);
	    }
	}
	Cprintf(DIAG_ALWAYS, "%d image files added to %s\n", i, dbfile);
    }
QUIT:
    if (dbid) {
	cond = DB_Close(dbid);	/* Close database, if opened */
	if (cond != DB_NORMAL) {
	    Cprintf(DIAG_SEVERE, "Error closing database %s\n", dbfile);
	    Cprint_errors(DIAG_SEVERE);
	}
    }
    COND_DumpConditions();
    return (0);
}


/*------------------------------------------------------------------------*/
/* add_image
**
** Purpose:
**      Add an image file to the database.
**      The patient, study, series are created in the database if they don't
**      already exist.  If the image UID matches an existing one, the
**      existing one is deleted before the new one is added.
**
** Parameter Dictionary:
**      short dbid;                 DB Database id
**      char *file;                 Name of image file (with complete path)
**
** Return Values:
**      0 if everything went well.  Otherwise, a non-zero value.
**
** Algorithm:
**      1. Extract information required to add image to database and
**         store into DB structures PatientLevel, StudyLevel, SeriesLevel
**         and ImageLevel.  This requires using the DICOM OBJECT package
**         (DCM) to parse the image file (using DCM_OpenFile,
**         DCM_GetElementValue and DCM_CloseObject)
**
**      2. If the patient doesn't exist, create one (using DB_GetPatient,
**         and DB_AddPatient)
**      3. If the study doesn't exist, create it (using DB_GetStudy,
**         DB_AddStudy)
**      4. If the series doesn't exist, create it (using DB_GetSeries,
**         DB_AddSeries)
**      5. Add the image to the series (using DB_GetImage, DB_AddImage).
**         If the image already exists, we delete the previous one with
**         DB_DelImage and add the new one.  We do not treat overwriting
**         an image as an error.
**
** Edit History:
**      14 Jun 93 sh    Initial edit
*/

static short
add_image(short dbid, char *file)
{
    CONDITION cond1,
        cond;			/* Condition codes */
    Query query;		/* Database query structure */
    DCM_OBJECT *object;		/* A DICOM object */
    int elementCount;		/* # of objects parsed */

/* PARSE IMAGE FILE. EXTRACT INFORMATION NEEDED TO ADD IMAGE TO DATABASE. */

    memset((void *) &query, '\0', sizeof(query));	/* Start with clean
							 * slate */

    cond = DCM_OpenFile(file, (long) DCM_ORDERLITTLEENDIAN, &object);
    if (cond != DCM_NORMAL) {
	Cprintf(DIAG_SEVERE, "Error opening image file %s\n", file);
	Cprint_errors(DIAG_SEVERE);
	return (-1);
    }
    cond = IAP_ObjectToQuery(&object, "", &query, &elementCount);

    cond1 = DCM_CloseObject(&object);
    if (!CTN_SUCCESS(DCM_NORMAL)) {
	Cprintf(DIAG_SEVERE, "Error parsing image file %s\n", file);
	Cprint_errors(DIAG_SEVERE);
	return (-1);
    }
    if (cond1 != DCM_NORMAL) {
	Cprintf(DIAG_SEVERE, "Error closing image file %s\n", file);
	Cprint_errors(DIAG_SEVERE);
	return (-1);
    }
/* LOAD NAME OF IMAGE FILE INTO DB STRUCTURE */

    sprintf(query.Image.FileName, "%.*s", sizeof(query.Image.FileName) - 2, file);

    dump_query(&query);

/* CREATE PATIENT, STUDY AND SERIES, IF THEY DON'T ALREADY EXIST. */

    cond = DB_AddPatient(dbid, &query.Patient);
    if (cond != DB_NORMAL && cond != DB_DUPATIENT) {
	Cprintf(DIAG_SEVERE, "Unable to add patient (cond=%lX) (image file %s)\n", cond, file);
	Cprint_errors(DIAG_SEVERE);
	return (-1);
    }
    cond = DB_AddStudy(dbid, query.Patient.PatID, &query.Study);
    if (cond != DB_NORMAL && cond != DB_DUPSTUDY) {
	Cprintf(DIAG_SEVERE, "Unable to add study (cond=%lX) (image file %s)\n", cond, file);
	Cprint_errors(DIAG_SEVERE);
	return (-1);
    }
    cond = DB_AddSeries(dbid, query.Patient.PatID, query.Study.StudyUID, &query.Series);
    if (cond != DB_NORMAL && cond != DB_DUPSERIES) {
	Cprintf(DIAG_SEVERE, "Unable to add series (cond=%lX) (image file %s)\n", cond, file);
	Cprint_errors(DIAG_SEVERE);
	return (-1);
    }
    (void) COND_PopCondition(TRUE);

    cond = DB_AddImage(dbid, query.Patient.PatID, query.Study.StudyUID,
		       query.Series.SeriesUID, &query.Image);
    if (cond == DB_DUPIMAGE) {
	Cprintf(DIAG_INFORM, "Deleting duplicate image (file %s)\n", file);
	cond = DB_DelImage(dbid, query.Patient.PatID, query.Study.StudyUID,
			   query.Series.SeriesUID, query.Image.ImageUID);
	if (cond != DB_NORMAL) {
	    Cprintf(DIAG_SEVERE, "Error deleting image %s\n", file);
	    Cprint_errors(DIAG_SEVERE);
	    return (-1);
	}
	/*
	 * SHOULD WE DELETE IMAGE FILE?????? IF SO, MAKE SURE ITS NOT THE
	 * SAME NAME AS THE ONE WE'RE TRYING TO ADD!
	 */
	cond = DB_AddImage(dbid, query.Patient.PatID, query.Study.StudyUID,
			   query.Series.SeriesUID, &query.Image);
    }
    if (cond != DB_NORMAL) {
	Cprintf(DIAG_SEVERE, "Unable to add image (cond=%lX) (image file %s)\n", cond, file);
	Cprint_errors(DIAG_SEVERE);
	return (-1);
    }
    return (0);			/* Image successfully added to db */
}

/*------------------------------------------------------------------------*/
/* dump_query
**
** Purpose:
** dump_query - Just a handy little debug function for displaying the
** contents of a DB Query structure.
**
** Parameter Dictionary:
**	pq		Handle to the query record
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static void
dump_query(Query * pq)
{
    printf("\n");
    printf("-BEGIN OF QUERY Structures-----------------------------------\n");
    printf("QUERY STATE:    0x%lX\n", pq->QueryState);
    printf("PATIENT:\n");
    printf("    Name:       %s\n", pq->Patient.Name);
    printf("    Birth:      %s\n", pq->Patient.BirthDate);
    printf("    PatID:      %s\n", pq->Patient.PatID);
    printf("    QueryFlag:  0x%lX\n", pq->Patient.Query_Flag);
    printf("STUDY:\n");
    printf("    Date:       %s\n", pq->Study.StudyDate);
    printf("    Time:       %s\n", pq->Study.StudyTime);
    printf("    StudyID:    %s\n", pq->Study.StudyID);
    printf("    StudyUID:   %s\n", pq->Study.StudyUID);
    printf("    Accession#: %s\n", pq->Study.AccessionNumber);
    printf("    Rfrng Doc:  %s\n", pq->Study.ReferringPhysName);
    printf("    Intrp Doc:  %s\n", pq->Study.InterpretingPhysName);
    printf("    Proc Desc:  %s\n", pq->Study.ProcedureDescription);
    printf("    Adm Dignos: %s\n", pq->Study.AdmittingDiagnosedDescription);
    printf("    QueryFlag:  0x%lX\n", pq->Study.Query_Flag);
    printf("SERIES:\n");
    printf("    Modality:   %s\n", pq->Series.Modality);
    printf("    Series #:   %s\n", pq->Series.SeriesNumber);
    printf("    Series UID: %s\n", pq->Series.SeriesUID);
    printf("    QueryFlag:  0x%lX\n", pq->Series.Query_Flag);
    printf("IMAGE\n");
    printf("    Image #:    %s\n", pq->Image.ImageNumber);
    printf("    Class UID:  %s\n", pq->Image.ClassUID);
    printf("    Image UID:  %s\n", pq->Image.ImageUID);
    printf("    Image File: %s\n", pq->Image.FileName);
    printf("    QueryFlag:  0x%lX\n", pq->Image.Query_Flag);
    printf("-END OF QUERY Structures-----------------------------------\n");
    printf("\n");
}

/*------------------------------------------------------------------------*/
/* Cprintf
**
** Purpose:
**      Message printing from the CLIENT portion of the SERVER.
**
** Parameter Dictionary:
**      int diag_level;         One of DIAG_xxx
**      char *fmt;              Just like printf
**      ..args..                Just like printf
** Return Values
**	None
**
** Algorithm:
**      if the diag_level (arg 1) is less than or equal to the current
**      diagnostic level (config.diag_level), format the message and
**      call DIAG_WriteMessage to print it.
*/

static void
Cprintf(int diag_level, char *fmt,...)
{
    char buf[1024];
    char *p;
    va_list arg_ptr;

    diag_level = diag_level;	/* Suppress compiler warning */
    sprintf(buf, "DBFILL -- ");

    p = buf + strlen(buf);

    va_start(arg_ptr, fmt);
    vsprintf(p, fmt, arg_ptr);
    va_end(arg_ptr);
    printf("%s", buf);
}

/*------------------------------------------------------------------------*/
static int d_level = DIAG_VERBOSE;

/* callback
**
** Purpose:
**	Call back routine called to print the condition code and the
**	corresponding error message
**
** Parameter Dictionary:
**	cond		Condition code
**	message		Error message
**
** Return Values:
**	TRUE always
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CTNBOOLEAN
callback(CONDITION cond, char *message)
{
    d_level = d_level;		/* Suppress compiler warning */
    printf("DBFILL -- (COND %lx) %s\n", cond, message);

    return (TRUE);
}

/*------------------------------------------------------------------------*/
/* Cprint_errors
**
** Purpose:
**      Print all CONDITIONS on the CONDITION stack.
**
** Parameter Dictionary:
**      int level;              One of the DIAG_xxx levels
**
** Algorithm:
**      Use the COND_ExtractConditions to print each condition.
**      The CONDITION stack is left empty.
*/

static void
Cprint_errors(int level)
{
    d_level = level;
    COND_ExtractConditions(callback);
    COND_PopCondition((CTNBOOLEAN) TRUE);
}
