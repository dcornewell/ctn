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
**			(image database) and transmits the study
**			to a DICOM Storage SCP.
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:44:34 $
** Source File:		$RCSfile: send_study.c,v $
** Revision:		$Revision: 1.5 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.5 $ $RCSfile: send_study.c,v $";

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

#include "send_study.h"

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
static char *callingAETitle = "CTN_INSTRUMENT";
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

/* sendCallback
**
** Purpose:
**	Callback routine for the C-SEND Response primitive
**
** Parameter Dictionary:
**	request		Pointer to request message
**	response	Pointer to response message
**	transmitted	Number of bytes sent
**	total		Total number of bytes to be sent
**	string		Context Information
**
** Return Values:
**	SRV_NORMAL
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
sendCallback(MSG_C_STORE_REQ * request, MSG_C_STORE_RESP * response,
	     unsigned long transmitted, unsigned long total,
	     char *string)
{
    if (transmitted == 0) {
	if (!silent)
	    printf("Initial call to sendCallback\n");
    }
    if (!silent)
	printf("%8d bytes transmitted of %8d (%s)\n",
	       transmitted, total, string);

    if (response != NULL) {
	MSG_DumpMessage(response, stdout);
    }
    return SRV_NORMAL;
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

static CONDITION
sendOneImage(DUL_ASSOCIATIONKEY ** association,
	     DUL_ASSOCIATESERVICEPARAMETERS * params,
	     char *fileName, char *moveAETitle, unsigned short moveMessageID,
	     CONDITION(*sendCallback) (), void *callbackCtx,
	     CTNBOOLEAN updateHeader, IDB_Query * query)
{
    static MSG_C_STORE_REQ
        request;
    CONDITION
	cond;
    DCM_OBJECT
	* object;
    static DCM_ELEMENT list[] = {
	{DCM_IDSOPCLASSUID, DCM_UI, "", 1,
	sizeof(request.classUID), (void *) &request.classUID[0]},
	{DCM_IDSOPINSTANCEUID, DCM_UI, "", 1,
	sizeof(request.instanceUID), (void *) &request.instanceUID[0]}
    };
    U32
	l;
    void
       *ctx;
    int
        index;

    if (fileName == NULL)
	return APP_NORMAL;
    if (fileName[0] == '\0')
	return APP_NORMAL;

    cond = DCM_OpenFile(fileName, DCM_ORDERLITTLEENDIAN | DCM_NOGROUPLENGTH,
			&object);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);		/* repair */
    }
    if (updateHeader) {
	updateHeaderEntries(&object, query);
    }
    for (index = 0; index < (int) DIM_OF(list); index++) {
	ctx = NULL;
	cond = DCM_GetElementValue(&object, &list[index], &l, &ctx);
	if (cond != DCM_NORMAL) {
	    (void) DCM_CloseObject(&object);
	    COND_DumpConditions();
	    THR_Shutdown();
	    exit(1);		/* repair */
	}
	list[index].d.string[l] = '\0';
    }

    request.type = MSG_K_C_STORE_REQ;
    request.messageID = SRV_MessageIDOut();
    request.priority = DCM_PRIORITYMEDIUM;
    request.dataSetType = DCM_CMDDATAIMAGE;
    request.dataSet = object;
    request.fileName = NULL;
    request.conditionalFields = 0;
    if (moveAETitle != NULL) {
	strcpy(request.moveAETitle, moveAETitle);
	request.moveMessageID = moveMessageID;
	request.conditionalFields |=
	    MSG_K_C_STORE_MOVEMESSAGEID | MSG_K_C_STORE_MOVEAETITLE;
    } else
	request.moveAETitle[0] = '\0';

    cond = SRV_CStoreRequest(association, params, &request, NULL,
			     sendCallback, callbackCtx, "");

    (void) DCM_CloseObject(&object);
    if (cond != SRV_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);		/* repair */
    }
    return APP_NORMAL;
}

static void
sendImages(LST_HEAD * imageList, char *callingAETitle,
	   char *destinationAETitle,
	   DMAN_HANDLE ** dmanHandle,
	   CTNBOOLEAN updateHeader)
{
    CONDITION cond = 1;
    int i;
    CTNBOOLEAN done;
    QUERY_LIST_ITEM *queryItem;
    IDB_InstanceListElement *instance;
    static DUL_NETWORKKEY *network = NULL;
    static DUL_ASSOCIATIONKEY
    *   sendAssociation = NULL;
    DUL_ASSOCIATESERVICEPARAMETERS
	* params;

    if (network == NULL) {
	cond = DUL_InitializeNetwork(DUL_NETWORK_TCP, DUL_AEREQUESTOR,
			   NULL, DUL_TIMEOUT, DUL_ORDERBIGENDIAN, &network);
	if (cond != DUL_NORMAL) {
	    fprintf(stderr, "Could not allocate network key\n");
	    COND_DumpConditions();
	    THR_Shutdown();
	    exit(1);
	}
    }
    params = createAssociationParameters();
    strcpy(params->callingAPTitle, callingAETitle);
    strcpy(params->calledAPTitle, destinationAETitle);
    establishSendAssociation(&network, imageList, destinationAETitle,
			     dmanHandle, &sendAssociation, params);


    queryItem = LST_Head(&imageList);
    if (queryItem != NULL)
	(void) LST_Position(&imageList, queryItem);

    while ((queryItem = LST_Dequeue(&imageList)) != NULL) {
	if (queryItem->query.image.InstanceList == NULL) {
	    instance = NULL;
	} else {
	    instance = LST_Head(&queryItem->query.image.InstanceList);
	}
	if (instance != NULL && sendAssociation != NULL) {
	    printf("%s\n", instance->Path);
	    cond = sendOneImage(&sendAssociation, params,
				instance->Path,
				"", 1, sendCallback, "X",
				updateHeader, &queryItem->query);
#if 0
	    cond = IAP_SendImage(&sendAssociation, params,
				 instance->Path,
				 "", 1, sendCallback, "X");
#endif
	} else
	    cond = 0;

	if (CTN_SUCCESS(cond)) {

	} else if (CTN_WARNING(cond)) {
	    COND_DumpConditions();
	} else {
	    COND_DumpConditions();
	}
    }
    (void) DUL_ReleaseAssociation(&sendAssociation);
    (void) DUL_DropAssociation(&sendAssociation);
    (void) DUL_ClearServiceParameters(params);
    free(params);
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
    char *patientID = "";
    char *studyInstanceUID = "";
    char *accessionNumber = "";
    LST_HEAD *imageList = NULL;
    DMAN_HANDLE *dmanHandle = NULL;
    char *controlDatabase = "CTNControl";
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
	case 'n':
	    updateHeader = TRUE;
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

    destinationAETitle = *argv;

    THR_Init();
    openDatabase(dbname, &handle);
    openControl(controlDatabase, &dmanHandle);
    imageList = selectImages(&handle, patientID, studyInstanceUID, accessionNumber);
    sendImages(imageList, callingAETitle, destinationAETitle, &dmanHandle,
	       updateHeader);
    closeDatabase(&handle);


    THR_Shutdown();
    return 0;
}

static void
usageerror()
{
    char msg[] = "\
Usage: send_study [-a title] [-d <dbname>] [-f <control>] [-p <pat id>] [-r <req num>] [-u <study UID>] destinationAETitle\n\
\n\
  -a  Set calling AE title (default CTN_INSTRUMENT)\n\
  -d  Set image database name (default INSTRUMENT_IDB)\n\
  -f  Set control database name (default CTNControl)\n\
  -n  Update image headers from database\n\
  -p  Send study (or studies) for patient ID <pat id>\n\
  -r  Send study with accession number <req num>\n\
  -u  Send study with study instance UID <study UID>\n\
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
