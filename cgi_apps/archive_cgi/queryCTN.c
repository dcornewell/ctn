
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "dicom_uids.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#ifdef CTN_MULTIBYTE
#include "tblmb.h"
#include "idbmb.h"
#include "dicom_chr.h"
#else
#include "tbl.h"
#include "idb.h"
#endif
#include "manage.h"

#include "queryCTN.h"
#include "dicomServerInterface.h"

static int compareStudyEntryByName(QUERY_LIST_ITEM* q1,  QUERY_LIST_ITEM *q2)
{
  int result;

  result = strcmp(q1->query.patient.PatNam, q2->query.patient.PatNam);
  if (result != 0)
    return result;

  result = strcmp(q1->query.study.StuDat, q2->query.study.StuDat);
  return result;
}

static int compareSeriesEntryByNumber(QUERY_LIST_ITEM* q1,
				      QUERY_LIST_ITEM *q2)
{
  int result;
  int series1;
  int series2;

  series1 = atoi(q1->query.series.SerNum);
  series2 = atoi(q2->query.series.SerNum);

  if (series1 < series2)
    return -1;
  else if (series1 > series2)
    return 1;
  else
    return 0;
}

static int compareImageEntryByNumber(QUERY_LIST_ITEM* q1,
				     QUERY_LIST_ITEM *q2)
{
  int result;
  int image1;
  int image2;

  image1 = atoi(q1->query.image.ImaNum);
  image2 = atoi(q2->query.image.ImaNum);

  if (image1 < image2)
    return -1;
  else if (image1 > image2)
    return 1;
  else
    return 0;
}

#ifdef CTN_MULTIBYTE
static void xlatePatientISO13toISO2022JP(LST_HEAD* l)
{
  QUERY_LIST_ITEM* p;

  p = LST_Head(&l);
  (void)LST_Position(&l, p);

  while (p != NULL) {
    char buffer[512];
    int length = 0;
    FILE* f;


#if 0
    f = fopen("/tmp/pname", "w");
    fprintf(f, "%s", p->query.patient.patNam.personName);
    fclose(f);
#endif

    CHR_Encode(p->query.patient.patNam.personName,
	       strlen(p->query.patient.patNam.personName),
	       CHR_DEFAULTISOIR13,
	       buffer, sizeof(buffer),
	       CHR_ISO2022JP,
	       &length);
    strcpy(p->query.patient.patNam.personName, buffer);
#if 0
    f = fopen("/tmp/smm1", "w");
    fprintf(f, "%s", p->query.patient.patNam.personName);
    fclose(f);
#endif

    p = LST_Next(&l);
  }

}
#endif

CONDITION openCTN(DMAN_HANDLE** controlHandle)
{
  CONDITION cond;

  cond = DMAN_Open("CTNControl", "", "", controlHandle);

  return cond;
}

CONDITION closeCTN(DMAN_HANDLE** controlHandle)
{
  CONDITION cond;

  cond = DMAN_Close(controlHandle);

  return cond;
}

static CONDITION patientCallback(IDB_Query *queryResponse, long count,
				 LST_HEAD *lst)
{
    QUERY_LIST_ITEM
    * item;
    IDB_InstanceListElement
	* e1, *e2;

    item = malloc(sizeof(*item));
    if (item == NULL)
	return 0;

    item->query = *queryResponse;

    if ((item->query.PatientNullFlag & QF_PAT_PatNam) != 0)
      item->query.patient.PatNam[0] = '\0';
    if ((item->query.PatientNullFlag & QF_PAT_PatID) != 0)
      item->query.patient.PatID[0] = '\0';
    if ((item->query.PatientNullFlag & QF_PAT_PatBirDat) != 0)
      item->query.patient.PatBirDat[0] = '\0';
    if ((item->query.PatientNullFlag & QF_PAT_PatSex) != 0)
      item->query.patient.PatSex[0] = '\0';

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


static CONDITION studyCallback(IDB_Query *queryResponse, long count,
			       LST_HEAD *lst)
{
    QUERY_LIST_ITEM
    * item;
    IDB_InstanceListElement
	* e1, *e2;

    item = malloc(sizeof(*item));
    if (item == NULL)
	return 0;

    item->query = *queryResponse;

    if ((item->query.StudyNullFlag & QF_STU_AccNum) != 0)
      item->query.study.AccNum[0] = '\0';
    if ((item->query.StudyNullFlag & QF_STU_StuDes) != 0)
      item->query.study.StuDes[0] = '\0';
    if ((item->query.StudyNullFlag & QF_STU_StuDat) != 0)
      item->query.study.StuDat[0] = '\0';

    if ((item->query.SeriesNullFlag & QF_SER_BodParExa) != 0)
      item->query.series.BodParExa[0] = '\0';
    if ((item->query.SeriesNullFlag & QF_SER_SerDes) != 0)
      item->query.series.SerDes[0] = '\0';
    if ((item->query.SeriesNullFlag & QF_SER_ProNam) != 0)
      item->query.series.ProNam[0] = '\0';

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

#if 0
  QUERY_LIST_ITEM *item;

  item = malloc(sizeof(*item));
  if (item == NULL)
    return 0;

  item->query = *queryResponse;
  (void) LST_Enqueue(&lst, item);
  return IDB_NORMAL;
#endif
}

static CONDITION seriesCallback(IDB_Query *queryResponse, long count,
			       LST_HEAD *lst)
{
    QUERY_LIST_ITEM
    * item;
    IDB_InstanceListElement
	* e1, *e2;

    item = malloc(sizeof(*item));
    if (item == NULL)
	return 0;

    item->query = *queryResponse;

#if 0
    if ((item->query.StudyNullFlag & QF_STU_AccNum) != 0)
      item->query.study.AccNum[0] = '\0';
    if ((item->query.StudyNullFlag & QF_STU_StuDes) != 0)
      item->query.study.StuDes[0] = '\0';
    if ((item->query.StudyNullFlag & QF_STU_StuDat) != 0)
      item->query.study.StuDat[0] = '\0';
#endif

    if ((item->query.SeriesNullFlag & QF_SER_BodParExa) != 0)
      item->query.series.BodParExa[0] = '\0';
    if ((item->query.SeriesNullFlag & QF_SER_SerDes) != 0)
      item->query.series.SerDes[0] = '\0';
    if ((item->query.SeriesNullFlag & QF_SER_ProNam) != 0)
      item->query.series.ProNam[0] = '\0';

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

#if 0
  QUERY_LIST_ITEM *item;

  item = malloc(sizeof(*item));
  if (item == NULL)
    return 0;

  item->query = *queryResponse;
  (void) LST_Enqueue(&lst, item);
  return IDB_NORMAL;
#endif
}

static CONDITION imageCallback(IDB_Query *queryResponse, long count,
			       LST_HEAD *lst)
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



CONDITION buildMoveObject(const char *studyUID, const char *seriesUID,
		DCM_OBJECT **moveObject)
{
  char queryLevel[] = DCM_QUERYLEVELSERIES;
  DCM_ELEMENT e[] = {
    {DCM_IDQUERYLEVEL, DCM_CS, "", 1, 0, NULL},
    {DCM_RELSTUDYINSTANCEUID, DCM_UI, "", 1, 0, NULL},
    {DCM_RELSERIESINSTANCEUID, DCM_UI, "", 1, 0, NULL}
  };
  CONDITION cond;

  e[0].d.string = queryLevel;
  e[1].d.string = (char*)studyUID;
  e[2].d.string = (char*)seriesUID;

  cond = DCM_ModifyElements(moveObject, e, (int)DIM_OF(e), NULL, 0, NULL);
  if (cond != DCM_NORMAL)
    return 0;

  return 1;
}


/* Public functions beneath here */


CONDITION listOfArchives(DMAN_HANDLE** controlHandle, LST_HEAD* lst)
{
  DMAN_STORAGEACCESS criteria;
  DMAN_STORAGEACCESS work;
  CONDITION cond;

  memset(&criteria, 0, sizeof(criteria));
  criteria.Type = DMAN_K_STORAGEACCESS;
  work = criteria;

  cond = DMAN_Select(controlHandle,
		     (DMAN_GENERICRECORD*)&work,
		     (DMAN_GENERICRECORD*)&criteria,
		     lst, NULL, NULL, NULL);

  return cond;
}

CONDITION listOfDestinations(DMAN_HANDLE** controlHandle, LST_HEAD *lst)
{
  CONDITION cond;

  cond = DMAN_SelectImageDestinations(controlHandle, "CTN", lst);

  return cond;
}

CONDITION listOfPatients(const char* dbKey,
			 const char* codeSet,
			 const char* name,
			 LST_HEAD *patientList)
{
  IDB_HANDLE *IDBHandle = 0;
  CONDITION cond;
  IDB_Query query;
  long count = 0;

  memset(&query, 0, sizeof(query));	/* Says to match on everything */

  cond = IDB_Open(dbKey, &IDBHandle);
#ifdef CTN_MULTIBYTE
  IDB_SetCharSet(&IDBHandle, codeSet);
#endif

  cond = IDB_Select(&IDBHandle,
		      STUDY_ROOT,		/* Query Model */
		      IDB_PATIENT_LEVEL,	/* Begin level */
		      IDB_PATIENT_LEVEL,	/* End level */
		      &query,			/* The query specification */
		      &count,			/* Number of records found */
		      patientCallback,		/* Called for each DB hit */
		      patientList);		/* Context info for query */

#ifdef CTN_MULTIBYTE
  if (strcmp(codeSet, "EUCJPROMAJI") == 0)
    xlatePatientISO13toISO2022JP(patientList);
#endif

  cond = IDB_Close(&IDBHandle);
  return cond;
}


CONDITION listOfPatientStudies(const char* dbKey,
			       const char* codeSet,
			       const char* patientID,
			       LST_HEAD *studyList)
{
  IDB_HANDLE *IDBHandle = 0;
  CONDITION cond;
  IDB_Query query;
  long count = 0;

  memset(&query, 0, sizeof(query));	/* Says to match on everything */
#ifdef CTN_MULTIBYTE
  strcpy(query.patient.InternalID, patientID);
  strcpy(query.patient.patID.text, patientID);
  query.patient.patID.length = strlen(patientID);
  query.PatientQFlag = QF_PAT_InternalID;
#else
  strcpy(query.patient.PatID, patientID);
  query.PatientQFlag = QF_PAT_PatID;
#endif

#if 0
  { FILE *f;
    f = fopen("/tmp/patid", "a");
    fprintf(f, "%d %s\n", query.patient.patID.length,
		query.patient.patID.text);
    fclose(f);
  }
#endif

  cond = IDB_Open(dbKey, &IDBHandle);
#ifdef CTN_MULTIBYTE
  IDB_SetCharSet(&IDBHandle, codeSet);
#endif
  cond = IDB_Select(&IDBHandle,
		      PATIENT_ROOT,		/* Query Model */
		      IDB_PATIENT_LEVEL,	/* Begin level */
		      IDB_STUDY_LEVEL,	/* End level */
		      &query,			/* The query specification */
		      &count,			/* Number of records found */
		      studyCallback,		/* Called for each DB hit */
		      studyList);		/* Context info for query */

#ifdef CTN_MULTIBYTE
  if (strcmp(codeSet, "EUCJPROMAJI") == 0)
    xlatePatientISO13toISO2022JP(studyList);
#endif

  cond = IDB_Close(&IDBHandle);
  return cond;
}


CONDITION listOfStudies(const char* dbKey, 
			LST_HEAD *studyList)
{
  IDB_HANDLE *IDBHandle = 0;
  CONDITION cond;
  IDB_Query query;
  long count = 0;

  memset(&query, 0, sizeof(query));	/* Says to match on everything */

  cond = IDB_Open(dbKey, &IDBHandle);

  cond = IDB_Select(&IDBHandle,
		    STUDY_ROOT,		/* Query Model */
		    IDB_PATIENT_LEVEL,	/* Begin level */
		    IDB_STUDY_LEVEL,	/* End level */
		    &query,		/* The query specification */
		    &count,		/* The number of records found */
		    studyCallback,	/* Called for each DB hit */
		    studyList);		/* Context info for query */

  cond = IDB_Close(&IDBHandle);
  return cond;
}

CONDITION searchOnePatient(const char* dbKey,
			 const char* codeSet,
			 const char* internalID,
			 IDB_Query* response)
{
  LST_HEAD *lst;

  IDB_HANDLE *IDBHandle = 0;
  CONDITION cond;
  IDB_Query query;
  QUERY_LIST_ITEM *item;
  long count = 0;

  memset(&query, 0, sizeof(query));
#ifdef CTN_MULTIBYTE
  strcpy(query.patient.InternalID, internalID);
  query.PatientQFlag = QF_PAT_InternalID;
#else
  strcpy(query.patient.PatID, internalID);
  query.PatientQFlag = QF_PAT_PatID;
#endif

  cond = IDB_Open(dbKey, &IDBHandle);
#ifdef CTN_MULTIBYTE
  IDB_SetCharSet(&IDBHandle, codeSet);
#endif

  lst = LST_Create();

  cond = IDB_Select(&IDBHandle,
		    PATIENT_ROOT,		/* Query Model */
		    IDB_PATIENT_LEVEL,	/* Begin level */
		    IDB_PATIENT_LEVEL,	/* End level */
		    &query,		/* The query specification */
		    &count,		/* The number of records found */
		    patientCallback,	/* Called for each DB hit */
		    lst);		/* Context info for query */

  cond = IDB_Close(&IDBHandle);

#ifdef CTN_MULTIBYTE
  if (strcmp(codeSet, "EUCJPROMAJI") == 0)
    xlatePatientISO13toISO2022JP(lst);
#endif

  item = LST_Dequeue(&lst);
  *response = item->query;

  while (item != NULL) {
    free(item);
    item = LST_Dequeue(&lst);
  }
  LST_Destroy(&lst);

  return cond;
}

CONDITION searchOneStudy(const char* dbKey, const char* codeSet,
			 const char* studyUID,
			 IDB_Query* response)
{
  LST_HEAD *lst;

  IDB_HANDLE *IDBHandle = 0;
  CONDITION cond;
  IDB_Query query;
  QUERY_LIST_ITEM *item;
  long count = 0;

  memset(&query, 0, sizeof(query));
  strcpy(query.study.StuInsUID, studyUID);	/* Match on Study UID */
  query.StudyQFlag = QF_STU_StuInsUID;

  cond = IDB_Open(dbKey, &IDBHandle);
#ifdef CTN_MULTIBYTE
  IDB_SetCharSet(&IDBHandle, codeSet);
#endif

  lst = LST_Create();

  cond = IDB_Select(&IDBHandle,
		    STUDY_ROOT,		/* Query Model */
		    IDB_PATIENT_LEVEL,	/* Begin level */
		    IDB_STUDY_LEVEL,	/* End level */
		    &query,		/* The query specification */
		    &count,		/* The number of records found */
		    studyCallback,	/* Called for each DB hit */
		    lst);		/* Context info for query */

  cond = IDB_Close(&IDBHandle);

#ifdef CTN_MULTIBYTE
  if (strcmp(codeSet, "EUCJPROMAJI") == 0)
    xlatePatientISO13toISO2022JP(lst);
#endif

  item = LST_Dequeue(&lst);
  *response = item->query;

  while (item != NULL) {
    free(item);
    item = LST_Dequeue(&lst);
  }
  LST_Destroy(&lst);

  return cond;
}

CONDITION searchOneSeries(const char* dbKey, const char* studyUID,
			 const char *seriesUID, IDB_Query* response)
{
  LST_HEAD *lst;

  IDB_HANDLE *IDBHandle = 0;
  CONDITION cond;
  IDB_Query query;
  QUERY_LIST_ITEM *item;
  long count = 0;

  memset(&query, 0, sizeof(query));
  strcpy(query.study.StuInsUID, studyUID);	/* Match on Study UID */
  query.StudyQFlag = QF_STU_StuInsUID;

  strcpy(query.series.SerInsUID, seriesUID);	/* Match on Series UID */
  query.SeriesQFlag = QF_SER_SerInsUID;

  cond = IDB_Open(dbKey, &IDBHandle);

  lst = LST_Create();

  cond = IDB_Select(&IDBHandle,
		    STUDY_ROOT,		/* Query Model */
		    IDB_PATIENT_LEVEL,	/* Begin level */
		    IDB_SERIES_LEVEL,	/* End level */
		    &query,		/* The query specification */
		    &count,		/* The number of records found */
		    seriesCallback,	/* Called for each DB hit */
		    lst);		/* Context info for query */

  cond = IDB_Close(&IDBHandle);

  item = LST_Dequeue(&lst);
  *response = item->query;

  while (item != NULL) {
    free(item);
    item = LST_Dequeue(&lst);
  }
  LST_Destroy(&lst);

  return cond;
}

CONDITION searchOneImage(const char* dbKey, const char* codeSet,
			 const char* seriesUID,
			 const char *imageUID, IDB_Query* response)
{
  LST_HEAD *lst;

  IDB_HANDLE *IDBHandle = 0;
  CONDITION cond;
  IDB_Query query;
  QUERY_LIST_ITEM *item;
  long count = 0;

  memset(&query, 0, sizeof(query));

  strcpy(query.series.SerInsUID, seriesUID);
  query.SeriesQFlag = QF_SER_SerInsUID;

  strcpy(query.image.SOPInsUID, imageUID);	/* Match on SOP Ins UID */
  query.ImageQFlag = QF_IMA_SOPInsUID;

  cond = IDB_Open(dbKey, &IDBHandle);
#ifdef CTN_MULTIBYTE
  IDB_SetCharSet(&IDBHandle, codeSet);
#endif

  lst = LST_Create();

  cond = IDB_Select(&IDBHandle,
		    STUDY_ROOT,		/* Query Model */
		    IDB_SERIES_LEVEL,	/* Begin level */
		    IDB_IMAGE_LEVEL,	/* End level */
		    &query,		/* The query specification */
		    &count,		/* The number of records found */
		    imageCallback,	/* Called for each DB hit */
		    lst);		/* Context info for query */


  cond = IDB_Close(&IDBHandle);

  item = LST_Dequeue(&lst);
  *response = item->query;

  while (item != NULL) {
    free(item);
    item = LST_Dequeue(&lst);
  }
  LST_Destroy(&lst);

  return cond;
}


CONDITION deleteOneStudy(const char* dbKey, const char* studyUID)
{
  IDB_HANDLE *IDBHandle = 0;
  CONDITION cond;

  cond = IDB_Open(dbKey, &IDBHandle);

  cond = IDB_Delete(&IDBHandle,
		    IDB_STUDY_LEVEL,
		    studyUID,
		    TRUE);

  cond = IDB_Close(&IDBHandle);

  return cond;
}


CONDITION searchSeries(const char* dbKey, const char* codeSet,
		       const char* studyUID,
		       LST_HEAD *seriesList)
{
  IDB_HANDLE *IDBHandle = 0;
  CONDITION cond;
  IDB_Query query;
  QUERY_LIST_ITEM *item;
  long count = 0;

  memset(&query, 0, sizeof(query));
  strcpy(query.study.StuInsUID, studyUID);	/* Match on Study UID */
  query.StudyQFlag = QF_STU_StuInsUID;

  cond = IDB_Open(dbKey, &IDBHandle);
#ifdef CTN_MULTIBYTE
  IDB_SetCharSet(&IDBHandle, codeSet);
#endif

  cond = IDB_Select(&IDBHandle,
		      STUDY_ROOT,		/* Query Model */
		      IDB_STUDY_LEVEL,	/* Begin level */
		      IDB_SERIES_LEVEL,	/* End level */
		      &query,		/* The query specification */
		      &count,		/* The number of records found */
		      studyCallback,	/* Called for each DB hit */
		      seriesList);	/* Context info for query */

  cond = IDB_Close(&IDBHandle);

  return cond;
}

CONDITION moveSeriesRequest(const char* dbKey, const char* studyUID,
		     const char *seriesUID, const char* destinationTitle,
		     long *successes, long *warnings, long *failures)
{
  DMAN_HANDLE *handle;
  CONDITION cond;
  DCM_OBJECT *moveObject;
  DUL_NETWORKKEY *network = NULL;
  DUL_ASSOCIATIONKEY *association = NULL;
  DUL_ASSOCIATESERVICEPARAMETERS params;

  openCTN(&handle);

  cond = connectToDICOMServer(dbKey, destinationTitle, &handle,
				&network, &association,
				&params, DICOM_SOPSTUDYQUERY_MOVE);


  cond =DCM_CreateObject(&moveObject, 0);
  cond = buildMoveObject(studyUID, seriesUID, &moveObject);

  cond = sendMoveRequest(&association, &params, destinationTitle,
          moveObject, successes, warnings, failures);

  (void)DCM_CloseObject(&moveObject);

  cond = releaseDICOMServer(&network, &association);

  closeCTN(&handle);

#if 0
  IDB_HANDLE *IDBHandle = 0;
  CONDITION cond;
  IDB_Query query;
  QUERY_LIST_ITEM *item;
  long count = 0;

  memset(&query, 0, sizeof(query));
  strcpy(query.study.StuInsUID, studyUID);	/* Match on Study UID */
  query.StudyQFlag = QF_STU_StuInsUID;

  cond = IDB_Open(dbKey, &IDBHandle);

  cond = IDB_Select(&IDBHandle,
		    STUDY_ROOT,		/* Query Model */
		    IDB_PATIENT_LEVEL,	/* Begin level */
		    IDB_SERIES_LEVEL,	/* End level */
		    &query,		/* The query specification */
		    &count,		/* The number of records found */
		    studyCallback,	/* Called for each DB hit */
		    seriesList);	/* Context info for query */

  cond = IDB_Close(&IDBHandle);

  return cond;
#endif
  return 1;
}

CONDITION searchImages(const char* dbKey, const char* studyUID,
                        LST_HEAD *imageList)
{
  IDB_HANDLE *IDBHandle = 0;
  CONDITION cond;
  IDB_Query query;
  QUERY_LIST_ITEM *item;
  long count = 0;

  memset(&query, 0, sizeof(query));
  strcpy(query.study.StuInsUID, studyUID);	/* Match on Study UID */
  query.StudyQFlag = QF_STU_StuInsUID;

  cond = IDB_Open(dbKey, &IDBHandle);

  cond = IDB_Select(&IDBHandle,
		    STUDY_ROOT,		/* Query Model */
		    IDB_PATIENT_LEVEL,	/* Begin level */
		    IDB_IMAGE_LEVEL,	/* End level */
		    &query,		/* The query specification */
		    &count,		/* The number of records found */
		    studyCallback,	/* Called for each DB hit */
		    imageList);		/* Context info for query */

  cond = IDB_Close(&IDBHandle);

  return cond;
}

CONDITION searchImagesBySeries(const char* dbKey, const char* codeSet,
			       const char* seriesUID, LST_HEAD *imageList)
{
  IDB_HANDLE *IDBHandle = 0;
  CONDITION cond;
  IDB_Query query;
  QUERY_LIST_ITEM *item;
  long count = 0;

  memset(&query, 0, sizeof(query));
  strcpy(query.series.SerInsUID, seriesUID);	/* Match on Study UID */
  query.SeriesQFlag = QF_SER_SerInsUID;

  cond = IDB_Open(dbKey, &IDBHandle);
#ifdef CTN_MULTIBYTE
  IDB_SetCharSet(&IDBHandle, codeSet);
#endif

  cond = IDB_Select(&IDBHandle,
		    PATIENT_ROOT,		/* Query Model */
		    IDB_SERIES_LEVEL,	/* Begin level */
		    IDB_IMAGE_LEVEL,	/* End level */
		    &query,		/* The query specification */
		    &count,		/* The number of records found */
		    seriesCallback,	/* Called for each DB hit */
		    imageList);		/* Context info for query */

  cond = IDB_Close(&IDBHandle);

  return cond;
}


CONDITION dbLimits(const char*dbKey, IDB_Limits *limits)
{
  IDB_HANDLE *IDBHandle = 0;
  CONDITION cond;

  cond = IDB_Open(dbKey, &IDBHandle);

#if 0
  cond = IDB_SelectLimits(&IDBHandle,
			  limits);
#endif
  cond = IDB_Close(&IDBHandle);

  return cond;
}

int passwordCompare(const char *dbKey, const char *password)
{
  DMAN_STORAGEACCESS criteria;
  DMAN_STORAGEACCESS work;
  CONDITION cond;
  LST_HEAD *lst;
  DMAN_HANDLE *handle;
  DMAN_STORAGEACCESS *p;
  int returnValue = 0;


  openCTN(&handle);

  memset(&criteria, 0, sizeof(criteria));
  criteria.Type = DMAN_K_STORAGEACCESS;
  work = criteria;

  lst = LST_Create();

  cond = DMAN_Select(&handle,
		     (DMAN_GENERICRECORD*)&work,
		     (DMAN_GENERICRECORD*)&criteria,
		     lst, NULL, NULL, NULL);

  closeCTN(&handle);

  while ((p = LST_Dequeue(&lst)) != NULL) {
    if ((strcmp(p->DbKey, dbKey) == 0) && 
	(strcmp(p->Owner, password) == 0))
      returnValue = 1;

    free(p);
  }

  LST_Destroy(&lst);

  return returnValue;
}

int dbCodeSet(const char *dbKey, char *codeSet)
{
  DMAN_STORAGEACCESS criteria;
  DMAN_STORAGEACCESS work;
  CONDITION cond;
  LST_HEAD *lst;
  DMAN_HANDLE *handle;
  DMAN_STORAGEACCESS *p;
  int returnValue = 0;


  openCTN(&handle);

  memset(&criteria, 0, sizeof(criteria));
  criteria.Type = DMAN_K_STORAGEACCESS;
  work = criteria;

  lst = LST_Create();

  cond = DMAN_Select(&handle,
		     (DMAN_GENERICRECORD*)&work,
		     (DMAN_GENERICRECORD*)&criteria,
		     lst, NULL, NULL, NULL);

  closeCTN(&handle);

  while ((p = LST_Dequeue(&lst)) != NULL) {
    if (strcmp(p->DbKey, dbKey) == 0)
      strcpy(codeSet, p->Comment);

    free(p);
  }

  LST_Destroy(&lst);

  return returnValue;
}

void orderStudyListByName(LST_HEAD *lst)
{
  LST_Sort(&lst, sizeof(QUERY_LIST_ITEM), compareStudyEntryByName);
}

void orderSeriesListByNumber(LST_HEAD *lst)
{
  LST_Sort(&lst, sizeof(QUERY_LIST_ITEM), compareSeriesEntryByNumber);
}

void orderImageListByNumber(LST_HEAD *lst)
{
  LST_Sort(&lst, sizeof(QUERY_LIST_ITEM), compareImageEntryByNumber);
}

