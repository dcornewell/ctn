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
** Module Name(s):	main
**			usageError
**			myExit
**			makeDBEntry
**			dump_query
** Author, Date:	Aniruddha S. Gokhale, August 5th, 1993.
** Intent:		This module contains routines to create a database
**			entry from a given image file.
** Usage
**			create_db_entry [-v] [-p] dbfile imagefile prefmtImgFile
**
**	dbfile		Name of the database to be created
**	imagefile	Name of the file containing the image data
**	prefmtImgFile	Name of the preformatted image file i.e. one in which
**			the original image file is compressed to the 8-bit
**			format and then a greyscale transformation is made
**			on it.
**
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:37:36 $
** Source File:		$RCS: /dicom1a/projects/dicom93/apps/RCS/create_print_entry.c,v $
** Revision:		$Revision: 1.14 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.14 $ $RCSfile: create_print_entry.c,v $";

#include "ctn_os.h"

#if 0
#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <time.h>
#include    <stdarg.h>
#ifndef MACH
#include    <unistd.h>
#else
#include    <sys/file.h>
#endif
#endif

#include    "dicom.h"
#include    "ctnthread.h"
#include    "lst.h"
#include    "cfg.h"
#include    "condition.h"
#include    "dulprotocol.h"
#include    "dicom_objects.h"
#include    "dicom_messages.h"
#include    "dicom_services.h"
#include    "dicom_uids.h"
#include    "dbquery.h"
#include    "iap.h"

#define MAXPATHLEN	150
#define MAXFILENAME	200

static char cwd[MAXPATHLEN];	/* current working directory */
CTNBOOLEAN verbose = FALSE;

typedef enum {
    GETWDERROR,			/* getcwd command failed */
    NOREADACCESSERROR,		/* no read access to image file */
    UNKNOWNCMDLINEARG,		/* unknown command line argument */
    DBCREATEERROR,		/* data base create error */
    DBOPENERROR,		/* data base open error */
    DCMOPENFILEERROR,		/* DCM open file error */
    IAPOBJTOQUERYERROR,		/* IAP object to query conversion error */
    DCMCLOSEOBJERROR,		/* DCM close object error */
    DBADDPATERROR,		/* DB add patient error */
    DBADDSTUDYERROR,		/* DB add study error */
    DBADDSERIESERROR,		/* DB add series error */
    DBDELIMGERROR,		/* DB delete image error */
    DBADDIMGERROR,		/* DB add image error */
    DBCLOSEERROR		/* DB database close error */
}   DBERRORCODES;

/* prototype declarations */
void usageError();
void myExit(DBERRORCODES code);
void makeDBEntry(short dbid, char *imgfile, char *prefmtImgfile);
static void dump_query(Query * pq);

/* Main program.*/
int
main(int argc, char **argv)
{
    CONDITION
    cond;			/* condition return code */
    short
        dbid;			/* DB database id */
    CTNBOOLEAN
	prepend = FALSE;	/* for prepending current working directory */
    static char dbfile[MAXFILENAME];	/* name of the DB database file */
    static char imgfile[MAXFILENAME];	/* name of the image file */
    static char prefmtImgfile[MAXFILENAME];	/* name of the preformatted
						 * image */
    THR_Init();
    (void) strcpy(cwd, "\0");	/* initialize current working directory */

    /* get the current working directory */

    if (!getcwd(cwd, sizeof(cwd)))
	myExit(GETWDERROR);

    /* parse the command line arguments */
    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'v':
	    verbose = TRUE;
	    break;
	case 'p':
	    prepend = TRUE;
	    break;
	default:
	    myExit(UNKNOWNCMDLINEARG);
	}
    }
    if (argc < 3)
	usageError();

    if (sscanf(*argv++, "%s", dbfile) != 1)
	usageError();

    /* prepend the current working directory before the filenames */
    if (prepend) {
	(void) sprintf(imgfile, "%s/%s", cwd, *argv++);
	(void) sprintf(prefmtImgfile, "%s/%s", cwd, *argv++);
    } else {
	(void) sprintf(imgfile, "%s", *argv++);
	(void) sprintf(prefmtImgfile, "%s", *argv++);
    }

    if (verbose)
	printf("DB file : %s, imgfile : %s, prefmtfile : %s\n", dbfile,
	       imgfile, prefmtImgfile);

    /*
     * The first step is to open the DB database file, if it exists, else
     * newly create it.
     */

    cond = DB_Open(dbfile, &dbid);
    if (cond != DB_NORMAL) {	/* DB database file doesnot exist */
	/* create the DB data base file */
	cond = DB_CreateDB(dbfile);
	if (cond != DB_NORMAL)
	    myExit(DBCREATEERROR);
	/* now open the file */
	cond = DB_Open(dbfile, &dbid);
	if (cond != DB_NORMAL)
	    myExit(DBOPENERROR);
    }
    /* now add the image and make an entry into the data base */
    if (!access(imgfile, R_OK))
	makeDBEntry(dbid, imgfile, prefmtImgfile);
    else
	myExit(NOREADACCESSERROR);

    THR_Shutdown();
    return (0);			/* success */
}

/* usageError
**
** Purpose:
**	Displays the correct mode in which the application is to be invoked
**
** Parameter Dictionary:
**	None
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
usageError()
{
    fprintf(stderr,
	    "Usage : create_db_entry -v dbfile imagefile prefmtImgFile\n");
    exit(0);
}

/* myExit
**
** Purpose:
**	Issue an appropriate error message and exit with a failure status
**
** Parameter Dictionary:
**	errCode		Error code
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
myExit(DBERRORCODES errCode)
{
    switch (errCode) {
	case GETWDERROR:
	fprintf(stderr,
		"getcwd command failed\n");
	break;
    case NOREADACCESSERROR:
	fprintf(stderr,
		"no read access to image file\n");
	break;
    case UNKNOWNCMDLINEARG:
	fprintf(stderr,
		"unknown command line argument\n");
	break;
    case DBCREATEERROR:
	fprintf(stderr,
		"DB data base create error\n");
	break;
    case DBOPENERROR:
	fprintf(stderr,
		"DB data base open error\n");
	break;
    case DCMOPENFILEERROR:
	fprintf(stderr,
		"DCM open file error\n");
	break;
    case IAPOBJTOQUERYERROR:
	fprintf(stderr,
		"IAP object to query conversion error\n");
	break;
    case DCMCLOSEOBJERROR:
	fprintf(stderr,
		"DCM close object error\n");
	break;
    case DBADDPATERROR:
	fprintf(stderr,
		"DB add patient error\n");
	break;
    case DBADDSTUDYERROR:
	fprintf(stderr,
		"DB add study error\n");
	break;
    case DBADDSERIESERROR:
	fprintf(stderr,
		"DB add series error\n");
	break;
    case DBDELIMGERROR:
	fprintf(stderr,
		"DB delete image error\n");
	break;
    case DBADDIMGERROR:
	fprintf(stderr,
		"DB add image error\n");
	break;
    case DBCLOSEERROR:
	fprintf(stderr,
		"DB database close error\n");
	break;
    }

    if (verbose)
	COND_DumpConditions();

    THR_Shutdown();
    exit(1);
}

/* makeDBEntry
**
** Purpose:
**	Adds an image file to the database.
**      The patient, study, series are created in the database if they don't
**      already exist.  If the image UID matches an existing one, the
**      existing one is deleted before the new one is added.
**
** Parameter Dictionary:
**	dbid		DB database id
**	imgfile		Image file name
**	prefmtImgfile	Preformatted image file name
**
** Return Values:
**	NONE
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
void DB_DumpDB(short dbid);
void
makeDBEntry(short dbid, char *imgfile, char *prefmtImgfile)
{
    CONDITION
    cond;			/* return condition status */
    Query
	query;			/* The Query structure */
    DCM_OBJECT
	* object;		/* Pointer to a DICOM object */
    int
        parseCount;		/* keeps track of the number of elements
				 * parsed while converting from DCM object to
				 * Query */

    /* Initialize the query structure to all null characters */
    (void) memset((void *) &query, '\0', sizeof(query));

    /* Read the image file and convert it into an object representation */
    cond = DCM_OpenFile(imgfile, (long) DCM_ORDERLITTLEENDIAN, &object);
    if (cond != DCM_NORMAL)
	myExit(DCMOPENFILEERROR);

    if (verbose)
	(void) DCM_DumpElements(&object, 0);

    /* Convert the object into the query structure */
    cond = IAP_ObjectToQuery(&object, "", &query, &parseCount);
    if (verbose) {
	printf("Number of elements parsed = %d\n", parseCount);
	COND_DumpConditions();
	printf("Query Structure created :- \n");
	dump_query(&query);
    }
    if (CTN_ERROR(cond))
	myExit(IAPOBJTOQUERYERROR);


    /* free the memory ocupied by the object */
    cond = DCM_CloseObject(&object);
    if (cond != DCM_NORMAL)
	myExit(DCMCLOSEOBJERROR);

    /*
     * So far everything is fine. Now store the name of the preformatted
     * image file in the FileName field in the ImageLevel structure
     */
    (void) sprintf(query.Image.FileName,
		   "%.*s", sizeof(query.Image.FileName) - 2, prefmtImgfile);

    /* now create the PATIENT, STUDY and SERIES, if they don't already exist */
    cond = DB_AddPatient(dbid, &query.Patient);
    if (cond != DB_NORMAL && cond != DB_DUPATIENT)
	myExit(DBADDPATERROR);

    cond = DB_AddStudy(dbid, query.Patient.PatID, &query.Study);
    if (cond != DB_NORMAL && cond != DB_DUPSTUDY)
	myExit(DBADDSTUDYERROR);

    cond = DB_AddSeries(dbid, query.Patient.PatID, query.Study.StudyUID,
			&query.Series);
    if (cond != DB_NORMAL && cond != DB_DUPSERIES)
	myExit(DBADDSERIESERROR);

    cond = DB_AddImage(dbid, query.Patient.PatID, query.Study.StudyUID,
		       query.Series.SeriesUID, &query.Image);
    if (cond == DB_DUPIMAGE) {
	/* we just delete previous image and replace it by new one */
	cond = DB_DelImage(dbid, query.Patient.PatID, query.Study.StudyUID,
			   query.Series.SeriesUID, query.Image.ImageUID);
	if (cond != DB_NORMAL)
	    myExit(DBDELIMGERROR);

	/* Now add this new image */
	cond = DB_AddImage(dbid, query.Patient.PatID, query.Study.StudyUID,
			   query.Series.SeriesUID, &query.Image);
	if (cond != DB_NORMAL)
	    myExit(DBADDIMGERROR);
    }
    if (verbose) {
	dump_query(&query);
	DB_DumpDB(dbid);
    }
    /* As a last step, we close the data base */
    cond = DB_Close(dbid);
    if (cond != DB_NORMAL)
	myExit(DBCLOSEERROR);
}

/* dump_query
**
** Purpose:
**	dumps the contents of the query structure if the verbose flag is
**	set.
**
** Parameter Dictionary:
**	pq	Pointer to the query structure
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
