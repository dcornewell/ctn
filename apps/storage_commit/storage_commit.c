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
** Intent:		This program finds a study in the CTN IDB
**			(image database) and sends a storage commit
**
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:43:43 $
** Source File:		$RCSfile: storage_commit.c,v $
** Revision:		$Revision: 1.5 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.5 $ $RCSfile: storage_commit.c,v $";

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
#include "idb.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "manage.h"
#include "fis.h"

#include "storage_commit.h"

CTNBOOLEAN silent = TRUE;

static char
   *dbname = "INSTRUMENT_IDB";

typedef struct {
    void *reserved[2];
    IDB_Query query;
}   QUERY_LIST_ITEM;

static CONDITION
selectCallback(IDB_Query * queryResponse,
	       long count, LST_HEAD * lst);

static CTNBOOLEAN verbose = FALSE;
CTNBOOLEAN verboseDUL = FALSE;
char *controlDatabase = "CTNControl";

static void usageerror();
static void openDatabase(const char *dbname, IDB_HANDLE ** handle);
static void closeDatabase(IDB_HANDLE ** handle);
static void openControl(char *controlDatabase, DMAN_HANDLE ** dmanHandle);
static void openFIS(char *fisDatabase, FIS_HANDLE ** fisHandle);
static LST_HEAD *
selectImages(IDB_HANDLE ** h, const char *patientID,
	     const char *studyInstanceUID, const char *accessionNumber);
static void
insertDBRecords(FIS_HANDLE ** handle, const char *callingAETitle,
		const char *destinationAETitle, LST_HEAD ** imageLlist,
		char *transactionUID);
static void
sendImages(LST_HEAD * imageList, char *destinationAETitle,
	   DMAN_HANDLE ** dmanHandle);


static void
loadTitleAddressParameters(DUL_ASSOCIATESERVICEPARAMETERS * params,
			   const char *callingAPTitle,
			   const char *calledAPTitle)
{
    char localHost[256];

    (void) gethostname(localHost, sizeof(localHost));
    strcpy(params->callingPresentationAddress, localHost);
    strcpy(params->calledAPTitle, calledAPTitle);
    strcpy(params->callingAPTitle, callingAPTitle);
}

main(int argc, char **argv)
{
    CONDITION
	cond;
    IDB_HANDLE
	* handle;
    IDB_Query
	recs;
    long
        i,
        count;
    static char *callingAETitle = "CTN_INSTRUMENT";
    static char *calledAETitle = "";
    char *patientID = "";
    char *studyInstanceUID = "";
    char *accessionNumber = "";
    LST_HEAD *imageList = NULL;
    DMAN_HANDLE *dmanHandle = NULL;
    FIS_HANDLE *fisHandle = NULL;
    char *fisDatabase = "INSTRUMENT_FIS";
    char transactionUID[DICOM_UI_LENGTH + 1];
    DUL_NETWORKKEY *networkKey;
    DUL_ASSOCIATIONKEY *associationKey = NULL;
    DUL_ASSOCIATESERVICEPARAMETERS serviceParameters;

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
	case 'p':
	    argc--;
	    argv++;
	    if (argc < 1)
		usageerror();
	    patientID = *argv;
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

    calledAETitle = *argv;

    THR_Init();
    openDatabase(dbname, &handle);
    openControl(controlDatabase, &dmanHandle);
    openFIS(fisDatabase, &fisHandle);

    imageList = selectImages(&handle, patientID, studyInstanceUID, accessionNumber);
    if (LST_Count(&imageList) == 0) {
	fprintf(stderr, "No images found to match the criteria you specified.\n");
	THR_Shutdown();
	exit(1);
    }
    insertDBRecords(&fisHandle, callingAETitle, calledAETitle, &imageList,
		    transactionUID);

    cond = DUL_InitializeNetwork(DUL_NETWORK_TCP, DUL_AEREQUESTOR,
			NULL, DUL_TIMEOUT, DUL_ORDERBIGENDIAN, &networkKey);
    if (cond != DUL_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    DUL_DefaultServiceParameters(&serviceParameters);
    loadTitleAddressParameters(&serviceParameters, callingAETitle, calledAETitle);

    if (establishAssociation(&networkKey,
			     calledAETitle,
			     &dmanHandle,
			     &associationKey,
			     &serviceParameters) != APP_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    sendStorageCommitRequest(&fisHandle, transactionUID,
			     &associationKey, &serviceParameters);

    /* sendImages(imageList, destinationAETitle, &dmanHandle); */
    closeDatabase(&handle);

    (void) DUL_ReleaseAssociation(&associationKey);
    (void) DUL_DropAssociation(&associationKey);
    (void) DUL_ClearServiceParameters(&serviceParameters);

    THR_Shutdown();
    return 0;
}

static void
usageerror()
{
    char msg[] = "\
Usage: storage_commit [-a title] [-d <dbname>] [-f <control>] [-i <fis name>] [-p <pat id>] [-r <req num>] [-u <study UID>] destinationAETitle\n\
\n\
  -a  Set calling AE title (default CTN_INSTRUMENT)\n\
  -d  Set image database name (default INSTRUMENT_IDB)\n\
  -f  Set control database name (default CTNControl)\n\
  -i  Set FIS database name (default INSTRUMENT_FIS)\n\
  -p  Send storage commit req for images with patient ID <pat id>\n\
  -r  Send storage commit req for images with accession number <req num>\n\
  -u  Send storage commit req for images with study instance UID <study UID>\n\
\n\
  destinationAETitle	An AE title found in the control database\n";

    fprintf(stderr, msg);
    exit(1);
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
openFIS(char *fisDatabase, FIS_HANDLE ** fisHandle)
{
    CONDITION cond = 0;

    cond = FIS_Open(fisDatabase, fisHandle);
    if (cond != FIS_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
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
    recs.PatientNullFlag = recs.StudyNullFlag =
	recs.SeriesNullFlag = recs.ImageNullFlag = 0;


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
    return imageList;
}

static void
insertCommitSOPRef(FIS_HANDLE ** fisHandle,
	   FIS_STORAGECOMMITREQRECORD * commit, QUERY_LIST_ITEM * queryItem)
{
    FIS_COMMITSOPREFRECORD commitSOP;
    CONDITION cond;

    commitSOP.Type = FIS_K_COMMITSOPREF;
    cond = FIS_NewRecord(fisHandle, FIS_K_COMMITSOPREF, &commitSOP);
    if (cond != FIS_NORMAL) {
	fprintf(stderr, "Could not make a new Storage Commitment SOP Reference database record\n");
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    strcpy(commitSOP.TraUID, commit->TraUID);
    strcpy(commitSOP.SOPClaUID, queryItem->query.image.SOPClaUID);
    strcpy(commitSOP.SOPInsUID, queryItem->query.image.SOPInsUID);

    commitSOP.Flag |= FIS_K_COMMITSOP_TRAUID |
	FIS_K_COMMITSOP_SOPCLAUID | FIS_K_COMMITSOP_SOPINSUID;

    cond = FIS_Insert(fisHandle, FIS_K_COMMITSOPREF, &commitSOP);
    if (cond != FIS_NORMAL) {
	fprintf(stderr, "Could not insert new FIS Storage Commit SOP Reference into DB\n");
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
}

static void
insertDBRecords(FIS_HANDLE ** fisHandle, const char *callingAETitle,
		const char *destinationAETitle, LST_HEAD ** imageList,
		char *transactionUID)
{
    FIS_STORAGECOMMITREQRECORD commit;
    CONDITION cond;
    QUERY_LIST_ITEM *queryItem;

    commit.Type = FIS_K_STORAGECOMMITREQ;
    cond = FIS_NewRecord(fisHandle, FIS_K_STORAGECOMMITREQ, &commit);
    if (cond != FIS_NORMAL) {
	fprintf(stderr, "Could not make a new Storage Commitment database record\n");
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    strcpy(transactionUID, commit.TraUID);

    strcpy(commit.ReqAE, callingAETitle);
    strcpy(commit.ResAE, destinationAETitle);
    commit.Flag |= FIS_K_SCOMMIT_REQAE | FIS_K_SCOMMIT_RESAE;

    cond = FIS_Insert(fisHandle, FIS_K_STORAGECOMMITREQ, &commit);
    if (cond != FIS_NORMAL) {
	fprintf(stderr, "Could not insert new FIS Storage Commit Request into DB\n");
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    queryItem = LST_Head(imageList);
    if (queryItem != NULL)
	(void) LST_Position(imageList, queryItem);

    while ((queryItem = LST_Dequeue(imageList)) != NULL) {
	insertCommitSOPRef(fisHandle, &commit, queryItem);
    }
}



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
