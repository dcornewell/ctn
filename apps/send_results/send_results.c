/*
+-+-+-+-+-+-+-+-+-
*/
/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	Steve Moore
** Intent:		This program sends an interpretation event
**			request indicating the interpretation was approved.
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:45:57 $
** Source File:		$RCSfile: send_results.c,v $
** Revision:		$Revision: 1.3 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.3 $ $RCSfile: send_results.c,v $";

#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <stdlib.h>
#endif

#include "dicom.h"
#include "ctnthread.h"
#include "condition.h"
#include "tbl.h"
#include "lst.h"
#include "manage.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "idb.h"
#include "fis.h"

#include "send_results.h"

CTNBOOLEAN silent = TRUE;


typedef struct {
    void *reserved[2];
    IDB_Query query;
}   QUERY_LIST_ITEM;

static CONDITION
selectCallback(IDB_Query * queryResponse,
	       long count, LST_HEAD * lst);
static CTNBOOLEAN verbose = FALSE;

static void usageerror();
static void openDatabase(const char *dbname, IDB_HANDLE ** handle);
static void closeDatabase(IDB_HANDLE ** handle);
static void openControl(char *controlDatabase, DMAN_HANDLE ** dmanHandle);
static LST_HEAD *
selectImages(IDB_HANDLE ** h, const char *patientID,
	     const char *studyInstanceUID, const char *accessionNumber);

static DUL_ASSOCIATESERVICEPARAMETERS *
createAssociationParameters()
{
    DUL_ASSOCIATESERVICEPARAMETERS *p;
    char localHost[128];

    p = malloc(sizeof(*p));
    if (p == NULL) {
	fprintf(stderr, "Could not allocate Service Parameters structure\n");
	THR_Shutdown();
	exit(1);
    }
    memset(p, 0, sizeof(*p));
    DUL_DefaultServiceParameters(p);
    (void) gethostname(localHost, sizeof(localHost));
    strcpy(p->callingPresentationAddress, localHost);
    p->maxPDU = 16384;

    return p;
}

static void
updateHeaderEntries(DCM_OBJECT ** object, const IDB_Query * query)
{
    static char patientName[DICOM_PN_LENGTH + 1];
    static char patientID[DICOM_LO_LENGTH + 1];
    static char accessionNumber[DICOM_SH_LENGTH + 1];
    static char studyInstanceUID[DICOM_UI_LENGTH + 1];
    static char seriesInstanceUID[DICOM_UI_LENGTH + 1];
    static char sopInstanceUID[DICOM_UI_LENGTH + 1];
    CONDITION cond;

    DCM_ELEMENT l[] = {
	{DCM_PATNAME, DCM_PN, "", 1,
	sizeof(patientName), (void *) patientName},
	{DCM_PATID, DCM_LO, "", 1,
	sizeof(patientID), (void *) patientID},
	{DCM_IDACCESSIONNUMBER, DCM_SH, "", 1,
	sizeof(accessionNumber), (void *) accessionNumber},
	{DCM_RELSTUDYINSTANCEUID, DCM_UI, "", 1,
	sizeof(studyInstanceUID), (void *) studyInstanceUID},
	{DCM_RELSERIESINSTANCEUID, DCM_UI, "", 1,
	sizeof(seriesInstanceUID), (void *) seriesInstanceUID},
	{DCM_IDSOPINSTANCEUID, DCM_UI, "", 1,
	sizeof(sopInstanceUID), (void *) sopInstanceUID}
    };

    strcpy(patientName, query->patient.PatNam);
    strcpy(patientID, query->patient.PatID);
    strcpy(accessionNumber, query->study.AccNum);
    strcpy(studyInstanceUID, query->study.StuInsUID);
    strcpy(seriesInstanceUID, query->series.SerInsUID);
    strcpy(sopInstanceUID, query->image.SOPInsUID);

    cond = DCM_ModifyElements(object, l, (int) DIM_OF(l),
			      NULL, 0, NULL);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);		/* repair */
    }
}

static void
openDatabase(const char *dbname, IDB_HANDLE ** handle)
{
    CONDITION cond = 0;

    cond = IDB_Open(dbname, handle);
    if (cond != IDB_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
}

static void
closeDatabase(IDB_HANDLE ** handle)
{
    CONDITION cond = 0;

    cond = IDB_Close(handle);
    if (cond != IDB_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
}


static void
openControl(char *controlDatabase, DMAN_HANDLE ** dmanHandle)
{
    CONDITION cond = 0;

    cond = DMAN_Open(controlDatabase, "", "", dmanHandle);
    if (cond != DMAN_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
}

static void
openFIS(char *name, FIS_HANDLE ** handle)
{
    CONDITION cond = 0;

    cond = FIS_Open(name, handle);
    if (cond != FIS_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
}

static void
findStudy(IDB_HANDLE ** handle, FIS_HANDLE ** fisHandle,
	  const char *accessionNumber, const char *studyInstanceUID,
	  FIS_STUDYRECORD * studyRecord)
{
    CONDITION cond;

    if (strlen(studyInstanceUID) != 0) {
	cond = FIS_GetOne(fisHandle, FIS_K_STUDY, FIS_K_STUDY,
		      studyInstanceUID, FIS_K_STU_RESULTSLIST, studyRecord);
	if (cond == FIS_NORMAL)
	    return;

    } else if (strlen(accessionNumber) != 0) {
	cond = FIS_SelectOne(fisHandle, FIS_K_STUDY, FIS_K_ACCESSION,
		       accessionNumber, FIS_K_STU_RESULTSLIST, studyRecord);
	if (cond == FIS_NORMAL)
	    return;

    } else {
	fprintf(stderr, "Cannot find a study w/o accession number or study ins uid\n");
	THR_Shutdown();
	exit(1);
    }

    COND_DumpConditions();
    THR_Shutdown();
    exit(1);
}

static void
findResults(FIS_HANDLE ** fisHandle,
	    const char *studyInstanceUID,
	    FIS_RESULTSRECORD * resultsRecord)
{
    CONDITION cond;

    cond = FIS_GetOne(fisHandle, FIS_K_RESULTS, FIS_K_STUDY,
		      studyInstanceUID, FIS_K_RESULTS_INTERPRETATIONLIST,
		      resultsRecord);
    if (cond != FIS_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
}

static void
sendResults(IDB_HANDLE ** handle, FIS_HANDLE ** fisHandle,
	    DMAN_HANDLE ** dmanHandle, const char *accessionNumber,
	    const char *studyInstanceUID,
	    char *requestingAETitle, char *remoteAETitle)
{
    FIS_STUDYRECORD study;
    FIS_RESULTSRECORD results;
    CONDITION cond = 0;
    FIS_INTERPRETATIONRECORD *interp;

    memset(&study, 0, sizeof study);
    study.Type = FIS_K_STUDY;

    findStudy(handle, fisHandle, accessionNumber, studyInstanceUID, &study);
    FIS_DumpRecord(&study, stdout);

    findResults(fisHandle, study.StuInsUID, &results);
    FIS_DumpRecord(&results, stdout);

    cond = FIS_SendEvent(fisHandle, dmanHandle, FIS_K_RESULTS, &results,
		   FIS_K_RESULTS_CREATED, requestingAETitle, remoteAETitle);
    if (cond != FIS_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    interp = LST_Head(&results.InterpretationList);
    LST_Position(&results.InterpretationList, interp);
    while (interp != NULL) {
	FIS_DumpRecord(interp, stdout);
	cond = FIS_SendEvent(fisHandle, dmanHandle, FIS_K_INTERPRETATION, interp,
	    FIS_K_INTERPRETATION_CREATED, requestingAETitle, remoteAETitle);
	if (cond != FIS_NORMAL) {
	    COND_DumpConditions();
	    THR_Shutdown();
	    exit(1);
	}
	cond = FIS_SendEvent(fisHandle, dmanHandle, FIS_K_INTERPRETATION, interp,
	   FIS_K_INTERPRETATION_APPROVED, requestingAETitle, remoteAETitle);
	if (cond != FIS_NORMAL) {
	    COND_DumpConditions();
	    THR_Shutdown();
	    exit(1);
	}
	interp = LST_Next(&results.InterpretationList);
    }

}

main(int argc, char **argv)
{
    CONDITION cond;
    IDB_HANDLE *handle;
    FIS_HANDLE *fisHandle;
    IDB_Query recs;
    long
        i,
        count;
    char *patientID = "";
    char *studyInstanceUID = "";
    char *accessionNumber = "";
    LST_HEAD *imageList = NULL;
    DMAN_HANDLE *dmanHandle = NULL;
    char *controlDatabase = "CTNControl";
    char *dbname = "WKSTN_IDB";
    char *fisDatabase = "WKSTN_FIS";
    char *callingAETitle = "CTN_WKSTN";
    char *destinationAETitle = "";
    CTNBOOLEAN updateHeader = FALSE;

    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'a':
	    argc--;
	    argv++;
	    if (argc < 1)
		usageerror();
	    callingAETitle = *argv;
	    break;
	case 'd':
	    argc--;
	    argv++;
	    if (argc < 1)
		usageerror();
	    dbname = *argv;
	    break;
	case 'f':
	    argc--;
	    argv++;
	    if (argc < 1)
		usageerror();
	    controlDatabase = *argv;
	    break;
	case 'i':
	    argc--;
	    argv++;
	    if (argc < 1)
		usageerror();
	    fisDatabase = *argv;
	    break;
	case 'r':
	    argc--;
	    argv++;
	    if (argc < 1)
		usageerror();
	    accessionNumber = *argv;
	    break;
	case 'u':
	    argc--;
	    argv++;
	    if (argc < 1)
		usageerror();
	    studyInstanceUID = *argv;
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

    destinationAETitle = *argv;

    THR_Init();
    openDatabase(dbname, &handle);
    openFIS(fisDatabase, &fisHandle);
    openControl(controlDatabase, &dmanHandle);

    sendResults(&handle, &fisHandle, &dmanHandle, accessionNumber,
		studyInstanceUID, callingAETitle, destinationAETitle);

    closeDatabase(&handle);

    THR_Shutdown();
    return 0;
}

static void
usageerror()
{
    char msg[] = "\
Usage: send_results [-a title] [-d dbname] [-f control] [-i <FIS db>] [-r <req num>] [-u <study UID>] destinationAETitle\n\
\n\
  -a  Set calling AE title (default CTN_WKSTN)\n\
  -d  Set image database name (default WKSTN_IDB)\n\
  -f  Set control database name (default CTNControl)\n\
  -i  Set FIS database name (default WKSTN_FIS)\n\
  -r  Send results from study with accession number <req num>\n\
  -u  Send results from study with study instance UID <study UID>\n\
\n\
  destinationAETitle	An AE title found in the control database\n";

    fprintf(stderr, msg);
    exit(1);
}

static LST_HEAD *
selectImages(IDB_HANDLE ** h, const char *patientID,
	     const char *studyInstanceUID, const char *accessionNumber)
{
    CONDITION cond;
    IDB_HANDLE *handle;
    IDB_Query recs;
    long count;
    LST_HEAD *imageList = NULL;

    recs.PatientQFlag = recs.StudyQFlag = recs.SeriesQFlag = recs.ImageQFlag = 0;
    recs.PatientNullFlag = recs.StudyNullFlag = recs.SeriesNullFlag = recs.ImageNullFlag = 0;


    if (strlen(patientID) != 0) {
	recs.PatientQFlag |= QF_PAT_PatID;
	strcpy(recs.patient.PatID, patientID);
    }
    if (strlen(studyInstanceUID) != 0) {
	recs.StudyQFlag |= QF_STU_StuInsUID;
	strcpy(recs.study.StuInsUID, studyInstanceUID);
    }
    if (strlen(accessionNumber) != 0) {
	recs.StudyQFlag |= QF_STU_AccNum;
	strcpy(recs.study.AccNum, accessionNumber);
    }
    if (imageList == NULL) {
	imageList = LST_Create();
	if (imageList == NULL) {
	    THR_Shutdown();
	    exit(1);
	}
    }
    cond = IDB_Select(h, STUDY_ROOT, IDB_PATIENT_LEVEL,
		 IDB_IMAGE_LEVEL, &recs, &count, selectCallback, imageList);
    if (cond != IDB_NORMAL) {
	COND_DumpConditions();
    }
    printf("%d\n", LST_Count(&imageList));
    return imageList;
}




static CONDITION
selectCallback(IDB_Query * queryResponse, long count, LST_HEAD * lst)
{
    QUERY_LIST_ITEM
    * item;
    IDB_InstanceListElement
	* e1, *e2;

    item = malloc(sizeof(*item));
    if (item == NULL)
	return 0;

    item->query = *queryResponse;
    if (queryResponse->image.InstanceList != NULL) {
	item->query.image.InstanceList = LST_Create();
	if (item->query.image.InstanceList == NULL)
	    return 0;
	e1 = LST_Head(&queryResponse->image.InstanceList);
	(void) LST_Position(&queryResponse->image.InstanceList, e1);
	while (e1 != NULL) {
	    e2 = malloc(sizeof(*e2));
	    *e2 = *e1;
	    (void) LST_Enqueue(&item->query.image.InstanceList, e2);
	    e1 = LST_Next(&queryResponse->image.InstanceList);
	}
    }
    (void) LST_Enqueue(&lst, item);
    return IDB_NORMAL;
}
