/*
+-+-+-+-+-+-+-+-+-
*/
/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	Steve Moore, 31-Mar-1997
** Intent:		This application sweeps through the image database
**			and deletes studies that are older than a time
**			threshold entered by the user.  The process deletes
**			study, series and image information in the database
**			and the actual image files.  It does not delete
**			the patient information.
**
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:37:36 $
** Source File:		$RCSfile: archive_cleaner.c,v $
** Revision:		$Revision: 1.5 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.5 $ $RCSfile: archive_cleaner.c,v $";

#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#endif

#include "dicom.h"
#include "condition.h"
#include "lst.h"
#ifdef CTN_MULTIBYTE
#include "tblmb.h"
#include "idbmb.h"
#else
#include "tbl.h"
#include "idb.h"
#endif
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "manage.h"
#include "fis.h"
#include "utility.h"

#include "image_archive.h"
#include "commit_result.h"

typedef struct {
    void *reserved[2];
    IDB_Query q;
}   QUERYITEM;

CTNBOOLEAN silent = TRUE;

/* selectCallback
**
** Purpose:
**	This is the callback function for the select calls that finds
**	studies to be deleted from the database.  This callback function
**	makes a copy of the study record and places it in the list which
**	is passed as a reference in the context variable.
**
** Parameter Dictionary:
**	q	A pointer to the query/data structure.  We will make
**		a copy of this data and place it in the caller's list.
**	count	The record number.  A housekeeping variable.  There is
**		no order implied in the select retrieval.
**	ctx	Context variable.  Contains a pointer to a LST_HEAD
**		used to store the records we have found.
**
** Return Values:
**	IDB_NORMAL	Success
**	0		Could not allocate a new structure or insert
**			into the list.
**
** Notes:
**
** Algorithm:
*/

static CONDITION
selectCallback(IDB_Query * q, long count, void *ctx)
{
    QUERYITEM *copy;
    LST_HEAD **l = 0;

    copy = malloc(sizeof(*copy));
    if (copy == NULL)
	return 0;

    copy->q = *q;
    l = (LST_HEAD **) ctx;
    if (LST_Enqueue(l, copy) != LST_NORMAL)
	return 0;

    return IDB_NORMAL;
}


/* cleanArchiveIDB
**
** Purpose:
**	This is the driving function for this program.  It takes
**	a time threshold passed by the caller and deletes all studies
**	in the database whose insertion date/time are before the
**	time threshold.
**
** Parameter Dictionary:
**	threshold	Unix time for the threshold.  We will delete
**			all studies inserted before this time.
**     	idbHandle	Handle for the image database.
**	deleteFlag	Only perform the delete if TRUE.  If not, just
**			list candidate studies.
**
** Return Values:
**	0		success
**	1		failure (unix exit status)
**
** Notes:
**	We have to make two passes through the image database to get
**	the right studies.  This is because we have separate insert date
**	and time fields, and we have to be careful in formulating the date/time
**	that we want.
**	We generate a (potentially long) list of studies, and then delete
**	them one by one.
**	We make no provisions for database locking.  We assume the time
**	delta is long enough that no one is trying to insert images
**	for this study while we are deleting them.
**
** Algorithm:
*/

static int
cleanArchiveIDB(time_t threshold, IDB_HANDLE ** idbHandle,
		CTNBOOLEAN deleteFlag)
{
    struct tm *theTime = 0;
    char dateString[DICOM_DA_LENGTH + 2];
    char timeString[DICOM_TM_LENGTH + 2];
    LST_HEAD *studyList = 0;
    CONDITION cond;
    long count = 0;
    IDB_Query query;
    QUERYITEM *ptr;
    void *t1;
    void *t2;
    double deltaTime;

    t1 = UTL_GetTimeStamp();

    studyList = LST_Create();
    if (studyList == NULL) {
	fprintf(stderr, "Could not create a new study list\n");
	return 1;
    }
    /*
     * Perform two selects to find the studies to be deleted.  The callback *
     * function will add studies to one common list.
     */

    /* Find all studies on the threshold date before the threshold time */
    theTime = localtime(&threshold);
    sprintf(dateString, "%04d%02d%02d",
	    theTime->tm_year + 1900,
	    theTime->tm_mon + 1,
	    theTime->tm_mday);
    sprintf(timeString, "-%02d%02d",
	    theTime->tm_hour,
	    theTime->tm_min);

    memset(&query, 0, sizeof(query));
    strcpy(query.study.InsertDate, dateString);
    strcpy(query.study.InsertTime, timeString);
    query.StudyQFlag = QF_STU_InsertDate | QF_STU_InsertTime;

    cond = IDB_Select(idbHandle, PATIENT_ROOT, IDB_STUDY_LEVEL, IDB_STUDY_LEVEL,
		      &query, &count, selectCallback, &studyList);
    if (cond != IDB_NORMAL && cond != IDB_NOMATCHES)
	return 1;

    (void) COND_PopCondition(TRUE);

    /*
     * Now just find all studies before the threshold date.  We don't need to *
     * worry about the time because we are 24 hours before the threshold
     * date.
     */

    threshold -= 24 * 60 * 60;
    theTime = localtime(&threshold);
    sprintf(dateString, "-%04d%02d%02d",
	    theTime->tm_year + 1900,
	    theTime->tm_mon + 1,
	    theTime->tm_mday);
    memset(&query, 0, sizeof(query));
    strcpy(query.study.InsertDate, dateString);
    query.StudyQFlag = QF_STU_InsertDate;

    cond = IDB_Select(idbHandle, PATIENT_ROOT, IDB_STUDY_LEVEL, IDB_STUDY_LEVEL,
		      &query, &count, selectCallback, &studyList);
    if (cond != IDB_NORMAL && cond != IDB_NOMATCHES)
	return 1;

    (void) COND_PopCondition(TRUE);


    printf("Found %6d studies to delete\n", LST_Count(&studyList));

    /* Go through each study in the list and delete it from the database. */

    while ((ptr = LST_Dequeue(&studyList)) != NULL) {
	printf("%s %s %16s %s\n",
	       ptr->q.study.InsertDate,
	       ptr->q.study.InsertTime,
	       ptr->q.study.AccNum,
	       ptr->q.study.StuInsUID);

	if (deleteFlag) {
	    t2 = UTL_GetTimeStamp();
	    cond = IDB_Delete(idbHandle, IDB_STUDY_LEVEL, ptr->q.study.StuInsUID, TRUE);
	    if (cond != IDB_NORMAL) {
		COND_DumpConditions();
		return 1;
	    }
	    deltaTime = UTL_DeltaTime(t2);
	    printf("Study delete time (sec): %f\n", deltaTime);
	    UTL_ReleaseTimeStamp(t2);
	    printf("There are %6d studies to be deleted\n", LST_Count(&studyList));
	}
	free(ptr);
    }

    LST_Destroy(&studyList);
    deltaTime = UTL_DeltaTime(t1);
    printf("Elapsed time to process all studies (sec): %f\n", deltaTime);
    UTL_ReleaseTimeStamp(t1);
    return 0;
}

static int
cleanArchiveIDBPatients(time_t threshold, IDB_HANDLE ** idbHandle,
			CTNBOOLEAN deleteFlag)
{
    struct tm *theTime = 0;
    char dateString[DICOM_DA_LENGTH + 2];
    char timeString[DICOM_TM_LENGTH + 2];
    LST_HEAD *patientList = 0;
    CONDITION cond;
    long count = 0;
    IDB_Query query;
    QUERYITEM *ptr;
    void *t1;
    void *t2;
    double deltaTime;

    t1 = UTL_GetTimeStamp();

    patientList = LST_Create();
    if (patientList == NULL) {
	fprintf(stderr, "Could not create a new patient list\n");
	return 1;
    }
    /*
     * Perform two selects to find the studies to be deleted.  The callback *
     * function will add studies to one common list.
     */

    /* Find all studies on the threshold date before the threshold time */
    theTime = localtime(&threshold);
    sprintf(dateString, "%04d%02d%02d",
	    theTime->tm_year + 1900,
	    theTime->tm_mon + 1,
	    theTime->tm_mday);
    sprintf(timeString, "-%02d%02d",
	    theTime->tm_hour,
	    theTime->tm_min);

    memset(&query, 0, sizeof(query));
    strcpy(query.patient.InsertDate, dateString);
    strcpy(query.patient.InsertTime, timeString);
    query.PatientQFlag = QF_PAT_InsertDate | QF_PAT_InsertTime;

    cond = IDB_Select(idbHandle, PATIENT_ROOT, IDB_PATIENT_LEVEL,
		      IDB_PATIENT_LEVEL,
		      &query, &count, selectCallback, &patientList);
    if (cond != IDB_NORMAL && cond != IDB_NOMATCHES)
	return 1;

    (void) COND_PopCondition(TRUE);

    /*
     * Now just find all patients before the threshold date.  We don't need
     * to * worry about the time because we are 24 hours before the threshold
     * date.
     */

    threshold -= 24 * 60 * 60;
    theTime = localtime(&threshold);
    sprintf(dateString, "-%04d%02d%02d",
	    theTime->tm_year + 1900,
	    theTime->tm_mon + 1,
	    theTime->tm_mday);
    memset(&query, 0, sizeof(query));
    strcpy(query.patient.InsertDate, dateString);
    query.PatientQFlag = QF_PAT_InsertDate;

    cond = IDB_Select(idbHandle, PATIENT_ROOT, IDB_PATIENT_LEVEL, IDB_PATIENT_LEVEL,
		      &query, &count, selectCallback, &patientList);
    if (cond != IDB_NORMAL && cond != IDB_NOMATCHES)
	return 1;

    (void) COND_PopCondition(TRUE);


    printf("Found %6d patients to delete\n", LST_Count(&patientList));

    /* Go through each patient in the list and delete it from the database. */

    while ((ptr = LST_Dequeue(&patientList)) != NULL) {
	printf("%s %s %16s %4d %s\n",
	       ptr->q.patient.InsertDate,
	       ptr->q.patient.InsertTime,
	       ptr->q.patient.PatID,
	       ptr->q.patient.NumPatRelStu,
	       ptr->q.patient.PatNam);

	if (deleteFlag && (ptr->q.patient.NumPatRelStu == 0)) {
	    t2 = UTL_GetTimeStamp();
	    cond = IDB_Delete(idbHandle, IDB_PATIENT_LEVEL, ptr->q.patient.PatID, TRUE);
	    if (cond != IDB_NORMAL) {
		COND_DumpConditions();
		return 1;
	    }
	    deltaTime = UTL_DeltaTime(t2);
	    printf("Patient delete time (sec): %f\n", deltaTime);
	    UTL_ReleaseTimeStamp(t2);
	    printf("There are %6d patients to be deleted\n", LST_Count(&patientList));
	}
	free(ptr);
    }

    LST_Destroy(&patientList);
    deltaTime = UTL_DeltaTime(t1);
    printf("Elapsed time to process all patients (sec): %f\n", deltaTime);
    UTL_ReleaseTimeStamp(t1);
    return 0;
}

static void
usageerror()
{
    char msg[] = "\
Usage: archive_cleaner [-d <dbname>] [-h] [-n] days [hours [minutes] ]\n\
\n\
  -d  Set image database name (default LTA_IDB)\n\
  -h  Print help message and exit\n\
  -n  Do not actually perform the delete\n";

    fprintf(stderr, msg);
    exit(1);
}

/* openDatabase
**
** Purpose:
**	Open the image database and pass a reference back to the caller.

static void
usageerror()
{
    char msg[] = "\
Usage: archive_cleaner [-d <dbname>] [-h] [-n] [-v] days [hours] [minutes]\n\
\n\
  -d  Set image database name (default LTA_IDB)\n\
  -h  Print help message\n\
  -n  Do not actually perform the delete\n\
  -v  Place in verbose mode\n";

    fprintf(stderr, msg);
    exit(1);
}

/* openDatabase
**
** Purpose:
**	Open the image database and pass a reference back to the caller.
**	If we are unsuccessful, dump the error stack and exit.
**
** Parameter Dictionary:
**	dbname		Name of the image database.
**	handle		Reference to the database passed back to caller
**
** Return Values:
**	void
**
** Notes:
**
** Algorithm:
*/

static void
openDatabase(const char *dbname, IDB_HANDLE ** handle)
{
    CONDITION cond = 0;

    cond = IDB_Open(dbname, handle);
    if (cond != IDB_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}



/* closeDatabase
**
** Purpose:
**	Close the image database referenced by the caller's handle.
**
** Parameter Dictionary:
**	handle		A reference to an image database.
**
** Return Values:
**	void
**
** Notes:
**
** Algorithm:
*/

static void
closeDatabase(IDB_HANDLE ** handle)
{
    CONDITION cond = 0;

    cond = IDB_Close(handle);
    if (cond != IDB_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}


main(int argc, char **argv)
{
    CONDITION cond;
    long i,
        count;

    CTNBOOLEAN verbose = FALSE;
    CTNBOOLEAN deleteFlag = TRUE;
    IDB_HANDLE *idbHandle = NULL;
    static char *dbname = "LTA_IDB";
    int deltaDays,
        deltaHours = 0,
        deltaMinutes = 0;
    time_t deltaTime;
    time_t timeNow;
    time_t timeThreshold;
    int status;

    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'd':
	    argc--;
	    argv++;
	    if (argc < 1)
		usageerror();
	    dbname = *argv;
	    break;
	case 'h':
	    usageerror();
	    break;
	case 'n':
	    deleteFlag = FALSE;
	    break;
	case 'v':
	    verbose = TRUE;
	    break;
	default:
	    break;
	}
    }

    if (argc < 1)
	usageerror();

    if (sscanf(*argv, "%d", &deltaDays) != 1)
	usageerror();

    argc--;
    argv++;
    if (argc > 0) {
	if (sscanf(*argv, "%d", &deltaHours) != 1)
	    usageerror();
    }
    argc--;
    argv++;
    if (argc > 0) {
	if (sscanf(*argv, "%d", &deltaMinutes) != 1)
	    usageerror();
    }
    deltaTime = (deltaDays * 24 * 60 * 60) + (deltaHours * 60 * 60) +
	(deltaMinutes * 60);
    if (deltaTime <= 0) {
	fprintf(stderr, "This application needs the delta time to be >= 0\n");
	exit(1);
    }
    timeNow = time(0);
    timeThreshold = timeNow - deltaTime;

    openDatabase(dbname, &idbHandle);

    status = cleanArchiveIDB(timeThreshold, &idbHandle, deleteFlag);

    if (status == 0)
	status = cleanArchiveIDBPatients(timeThreshold, &idbHandle, deleteFlag);

    closeDatabase(&idbHandle);

    COND_DumpConditions();
    return status;
}
