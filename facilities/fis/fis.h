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
** Module Name(s):
** Author, Date:
** Intent:
** Last Update:		$Author: smm $, $Date: 1997-05-30 19:08:12 $
** Source File:		$Source: /home/smm/ctn/ctn/facilities/fis/fis.h,v $
** Revision:		$Revision: 1.16 $
** Status:		$State: Exp $
*/

#ifndef _FIS_IS_IN
#define _FIS_IS_IN 1

#ifdef  __cplusplus
extern "C" {
#endif

typedef enum {
    FIS_K_PATIENT, FIS_K_VISIT, FIS_K_STUDY, FIS_K_INTERPRETATION,
    FIS_K_RESULTS, FIS_K_STUDYCOMPONENT, FIS_K_SCSERIES,
    FIS_K_SCIMAGE,
    FIS_K_STORAGECOMMITREQ, FIS_K_COMMITSOPREF,
    FIS_K_WORKQUEUE
}   FIS_DATA_TYPE;

/* Define a set of critiera that can be used for searching the database */

typedef enum {
    FIS_K_ACCESSION
} FIS_CRITERIA_TYPE;

typedef void FIS_HANDLE;

typedef struct {
    void *reserved[2];
    FIS_DATA_TYPE Type;
    long Flag;
} FIS_GENERICRECORD;

#define	FIS_K_PAT_PATID		(1 << 0)
#define	FIS_K_PAT_PATNAM	(1 << 1)
#define	FIS_K_PAT_PATUID	(1 << 2)
#define	FIS_K_PAT_BIRDAT	(1 << 3)
#define	FIS_K_PAT_SEX		(1 << 4)
#define	FIS_K_PAT_STUDYLIST	(1 << 5)
#define	FIS_K_PAT_VISITLIST	(1 << 6)
#define	FIS_K_PAT_ALIASLIST	(1 << 7)

typedef struct {
    void *reserved[2];
    FIS_DATA_TYPE Type;
    long Flag;
    char PatID[DICOM_LO_LENGTH + 1];
    char PatNam[DICOM_PN_LENGTH + 1];
    char PatUID[DICOM_UI_LENGTH + 1];
    char PatBirDat[DICOM_DA_LENGTH + 1];
    char PatSex[2];
    LST_HEAD *StudyList;
    LST_HEAD *VisitList;
    LST_HEAD *PatientAliasList;
}   FIS_PATIENTRECORD;

#define	FIS_K_VISIT_VISUID	(1 << 0)
#define	FIS_K_VISIT_PATUID	(1 << 1)
#define	FIS_K_VISIT_REFPHYNAM	(1 << 2)
#define	FIS_K_VISIT_STUDYLIST	(1 << 3)

typedef struct {
    void *reserved[2];
    FIS_DATA_TYPE Type;
    long Flag;
    char VisUID[DICOM_UI_LENGTH + 1];
    char PatUID[DICOM_UI_LENGTH + 1];
    char RefPhyNam[DICOM_PN_LENGTH + 1];
    LST_HEAD *StudyList;
}   FIS_VISITRECORD;

#define	FIS_K_STU_PATUID	(1 << 0)
#define	FIS_K_STU_STUINSUID	(1 << 1)
#define	FIS_K_STU_VISUID	(1 << 2)
#define	FIS_K_STU_ACCNUM	(1 << 3)
#define	FIS_K_STU_STUID		(1 << 4)
#define	FIS_K_STU_SCHSTUSTADAT	(1 << 5)
#define	FIS_K_STU_SCHSTUSTATIM	(1 << 6)
#define	FIS_K_STU_SCHSTULOC	(1 << 7)
#define	FIS_K_STU_PRODES	(1 << 9)
#define	FIS_K_STU_REQPRO	(1 << 9)
#define	FIS_K_STU_STUSTAID	(1 << 10)
#define	FIS_K_STU_SCHSTULOCAE	(1 << 11)
#define	FIS_K_STU_STUREADAT	(1 << 12)
#define	FIS_K_STU_STUREATIM	(1 << 13)

#define	FIS_K_STU_STUDYCOMPONENTLIST	(1 << 25)
#define	FIS_K_STU_RESULTSLIST		(1 << 26)

typedef struct {
    void *reserved[2];
    FIS_DATA_TYPE Type;
    long Flag;
    char PatUID[DICOM_UI_LENGTH + 1];
    char StuInsUID[DICOM_UI_LENGTH + 1];
    char VisUID[DICOM_UI_LENGTH + 1];
    char AccNum[DICOM_SH_LENGTH + 1];
    char StuID[DICOM_SH_LENGTH + 1];
    char SchStuStaDat[DICOM_DA_LENGTH + 1];
    char SchStuStaTim[DICOM_TM_LENGTH + 1];
    char SchStuLoc[DICOM_LO_LENGTH + 1];
    char ProDes[DICOM_LO_LENGTH + 1];
    char ReqPro[DICOM_SH_LENGTH + 1];
    char StuStaID[DICOM_CS_LENGTH + 1];
    char SchStuLocAE[DICOM_AE_LENGTH + 1];
    char StuReaDat[DICOM_DA_LENGTH + 1];
    char StuReaTim[DICOM_TM_LENGTH + 1];
    LST_HEAD *StudyComponentList;
    LST_HEAD *ResultsList;
}   FIS_STUDYRECORD;

#define	FIS_K_STUDYCOMP_STUCOMUID	(1 << 0)
#define	FIS_K_STUDYCOMP_STUINSUID	(1 << 1)
#define	FIS_K_STUDYCOMP_MOD		(1 << 2)
#define	FIS_K_STUDYCOMP_STUDES		(1 << 3)
#define	FIS_K_STUDYCOMP_PROCODVAL	(1 << 4)
#define	FIS_K_STUDYCOMP_PROCODSCHDES	(1 << 5)
#define	FIS_K_STUDYCOMP_PROCODMEA	(1 << 6)
#define	FIS_K_STUDYCOMP_STUCOMSTAID	(1 << 7)
#define	FIS_K_STUDYCOMP_STUID		(1 << 8)
#define	FIS_K_STUDYCOMP_SERIESLIST	(1 << 9)

typedef struct {
    void *reserved[2];
    FIS_DATA_TYPE Type;
    long Flag;
    char StuComUID[DICOM_UI_LENGTH + 1];
    char StuInsUID[DICOM_UI_LENGTH + 1];
    char Mod[DICOM_CS_LENGTH + 1];
    char StuDes[DICOM_LO_LENGTH + 1];
    char ProCodVal[DICOM_SH_LENGTH + 1];
    char ProCodSchDes[DICOM_SH_LENGTH + 1];
    char ProCodMea[DICOM_LO_LENGTH + 1];
    char StuComStaID[DICOM_CS_LENGTH + 1];
    char StuID[DICOM_SH_LENGTH + 1];
    LST_HEAD *SeriesList;
}   FIS_STUDYCOMPONENTRECORD;

#define	FIS_K_SCSERIES_SERINSUID	(1 << 0)
#define	FIS_K_SCSERIES_STUCOMUID	(1 << 1)
#define	FIS_K_SCSERIES_SERDAT		(1 << 2)
#define	FIS_K_SCSERIES_SERTIM		(1 << 3)
#define	FIS_K_SCSERIES_RETAETIT		(1 << 4)
#define	FIS_K_SCSERIES_STOMEDFILSETID	(1 << 5)
#define	FIS_K_SCSERIES_STOMEDFILSETUID	(1 << 6)
#define	FIS_K_SCSERIES_IMAGELIST	(1 << 7)

typedef struct {
    void *reserved[2];
    FIS_DATA_TYPE Type;
    long Flag;
    char SerInsUID[DICOM_UI_LENGTH + 1];
    char StuComUID[DICOM_UI_LENGTH + 1];
    char SerDat[DICOM_DA_LENGTH + 1];
    char SerTim[DICOM_TM_LENGTH + 1];
    char RetAETit[DICOM_AE_LENGTH + 1];
    char StoMedFilSetID[DICOM_SH_LENGTH + 1];
    char StoMedFilSetUID[DICOM_UI_LENGTH + 1];
    LST_HEAD *ImageList;
}   FIS_SCSERIESRECORD;

#define	FIS_K_SCIMAGE_SOPINSUID		(1 << 0)
#define	FIS_K_SCIMAGE_SERINSUID		(1 << 1)
#define	FIS_K_SCIMAGE_STUCOMUID		(1 << 2)
#define	FIS_K_SCIMAGE_SOPCLAUID		(1 << 3)
#define	FIS_K_SCIMAGE_RETAETIT		(1 << 4)
#define	FIS_K_SCIMAGE_STOMEDFILSETID	(1 << 5)
#define	FIS_K_SCIMAGE_STOMEDFILSETUID	(1 << 6)

typedef struct {
    void *reserved[2];
    FIS_DATA_TYPE Type;
    long Flag;
    char SOPInsUID[DICOM_UI_LENGTH + 1];
    char SerInsUID[DICOM_UI_LENGTH + 1];
    char StuComUID[DICOM_UI_LENGTH + 1];
    char SOPClaUID[DICOM_UI_LENGTH + 1];
    char RetAETit[DICOM_AE_LENGTH + 1];
    char StoMedFilSetID[DICOM_SH_LENGTH + 1];
    char StoMedFilSetUID[DICOM_UI_LENGTH + 1];
}   FIS_SCIMAGERECORD;

#define	FIS_K_RESULTS_RESUID	(1 << 0)
#define	FIS_K_RESULTS_STUINSUID	(1 << 1)
#define	FIS_K_RESULTS_RESID	(1 << 2)
#define	FIS_K_RESULTS_IMP	(1 << 3)
#define	FIS_K_RESULTS_INTERPRETATIONLIST	(1 << 4)
#define	FIS_K_RESULTS_INSCREDAT	(1 << 5)
#define	FIS_K_RESULTS_INSCRETIM	(1 << 6)
#define	FIS_K_RESULTS_INSCREUID	(1 << 7)

typedef struct {
    void *reserved[2];
    FIS_DATA_TYPE Type;
    long Flag;
    char ResUID[DICOM_UI_LENGTH + 1];
    char StuInsUID[DICOM_UI_LENGTH + 1];
    char ResID[DICOM_SH_LENGTH + 1];
    char Imp[DICOM_ST_LENGTH + 1];
    char InsCreDat[DICOM_DA_LENGTH+1];		/* Instance Creation Date */
    char InsCreTim[DICOM_TM_LENGTH+1];		/* Instance Creation Time */
    char InsCreUID[DICOM_UI_LENGTH+1];		/* Instance Creator UID */
    LST_HEAD *InterpretationList;
}   FIS_RESULTSRECORD;

#define	FIS_K_INTERP_INTUID	(1 << 0)
#define	FIS_K_INTERP_RESUID	(1 << 1)
#define	FIS_K_INTERP_INTTEX	(1 << 2)
#define	FIS_K_INTERP_INTID	(1 << 3)
#define	FIS_K_INTERP_INTTYPID	(1 << 4)
#define	FIS_K_INTERP_INTSTAID	(1 << 5)
#define	FIS_K_INTERP_INTDIADES	(1 << 6)
#define	FIS_K_INTERP_INSCREDAT	(1 << 7)
#define	FIS_K_INTERP_INSCRETIM	(1 << 8)
#define	FIS_K_INTERP_INSCREUID	(1 << 9)
#define	FIS_K_INTERP_PHYAPPINT	(1 << 10)
#define	FIS_K_INTERP_INTAPPDAT	(1 << 11)
#define	FIS_K_INTERP_INTAPPTIM	(1 << 12)

#define	FIS_K_INTERP_DIACODSEQ	(1 << 30)

typedef struct {
    void *reserved[2];
    FIS_DATA_TYPE Type;
    long Flag;
    char IntUID[DICOM_UI_LENGTH + 1];
    char ResUID[DICOM_UI_LENGTH + 1];
    char IntID[DICOM_SH_LENGTH + 1];
    char IntTex[DICOM_ST_LENGTH + 1];
    char IntTypID[DICOM_CS_LENGTH + 1];
    char IntStaID[DICOM_CS_LENGTH + 1];
    char IntDiaDes[DICOM_LT_LENGTH + 1];
    char InsCreDat[DICOM_DA_LENGTH+1];		/* Instance Creation Date */
    char InsCreTim[DICOM_TM_LENGTH+1];		/* Instance Creation Time */
    char InsCreUID[DICOM_UI_LENGTH+1];		/* Instance Creator UID */
    char PhyAppInt[DICOM_PN_LENGTH+1];		/* Physician Approving Interp */
    char IntAppDat[DICOM_DA_LENGTH+1];		/* Interp approval date */
    char IntAppTim[DICOM_TM_LENGTH+1];		/* Interp approval time */
}   FIS_INTERPRETATIONRECORD;

/* Define the two records that are used for storage commitment.  The
** first record is the high level records for the commit request/response.
** The second record lists the actual SOP Instance UIDs found in the
** request.
*/

#define FIS_K_SCOMMIT_TRAUID  (1 << 0)
#define FIS_K_SCOMMIT_REQAE   (1 << 1)
#define FIS_K_SCOMMIT_RESAE   (1 << 2)
#define FIS_K_SCOMMIT_REQDAT  (1 << 3)
#define FIS_K_SCOMMIT_REQTIM  (1 << 4)
#define FIS_K_SCOMMIT_RESDAT  (1 << 5)
#define FIS_K_SCOMMIT_RESTIM  (1 << 6)

typedef struct {
  void* reserved[2];
  FIS_DATA_TYPE Type;
  long Flag;
  char TraUID[DICOM_UI_LENGTH+1];     /* Transaction UID */
  char ReqAE[DICOM_AE_LENGTH+1];      /* Requesting AE title */
  char ResAE[DICOM_AE_LENGTH+1];      /* Responding AE title */
  char ReqDat[DICOM_DT_LENGTH+1];     /* Date of the request */
  char ReqTim[DICOM_TM_LENGTH+1];     /* Time of the request */
  char ResDat[DICOM_DT_LENGTH+1];     /* Date of the response */
  char ResTim[DICOM_TM_LENGTH+1];     /* Time of the response */
} FIS_STORAGECOMMITREQRECORD;

#define FIS_K_COMMITSOP_TRAUID    (1 << 0)
#define FIS_K_COMMITSOP_SOPCLAUID (1 << 1)
#define FIS_K_COMMITSOP_SOPINSUID (1 << 2)

typedef struct {
  void* reserved[2];
  FIS_DATA_TYPE Type;
  long Flag;
  char TraUID[DICOM_UI_LENGTH+1];     /* Transaction UID */
  char SOPClaUID[DICOM_UI_LENGTH+1];  /* SOP Class UID */
  char SOPInsUID[DICOM_UI_LENGTH+1];  /* SOP Instance UID */
} FIS_COMMITSOPREFRECORD;

#define FIS_K_WORKQUEUE_TASNAM    (1 << 0)
#define FIS_K_WORKQUEUE_TASID     (1 << 1)

/* Legal values for Task Name (TasNam) attribute */
#define FIS_WQ_STORAGECOMMIT "WQ_STORAGECOMMIT"

typedef struct {
  void* reserved[2];
  FIS_DATA_TYPE Type;
  long Flag;
  char TasNam[101];                   /* Task Name */
  char TasID[101];                    /* Task Identifier */
} FIS_WORKQUEUERECORD;


typedef enum {
    FIS_K_NO_EVENT,
    FIS_K_PATIENT_CREATED, FIS_K_PATIENT_DELETED,
    FIS_K_PATIENT_UPDATED,

    FIS_K_STUDY_CREATED, FIS_K_STUDY_SCHEDULED,
    FIS_K_STUDY_PATIENTARRIVED, FIS_K_STUDY_STARTED,
    FIS_K_STUDY_COMPLETED, FIS_K_STUDY_VERIFIED,
    FIS_K_STUDY_READ, FIS_K_STUDY_DELETED,
    FIS_K_STUDY_UPDATED,

    FIS_K_VISIT_CREATED, FIS_K_VISIT_SCHEDULED,
    FIS_K_VISIT_PATIENTADMITTED, FIS_K_VISIT_PATIENTTRANSFERRED,
    FIS_K_VISIT_PATIENTDISCHARGED, FIS_K_VISIT_DELETED,
    FIS_K_VISIT_UPDATED,

    FIS_K_RESULTS_CREATED, FIS_K_RESULTS_DELETED,
    FIS_K_RESULTS_UPDATED,

    FIS_K_INTERPRETATION_CREATED, FIS_K_INTERPRETATION_RECORDED,
    FIS_K_INTERPRETATION_TRANSCRIBED, FIS_K_INTERPRETATION_APPROVED,
    FIS_K_INTERPRETATION_DELETED, FIS_K_INTERPRETATION_UPDATED
}   FIS_EVENT;

void FIS_ClearList(LST_HEAD * l);
CONDITION FIS_Open(char *databaseName, FIS_HANDLE ** handle);
CONDITION FIS_Close(FIS_HANDLE ** handle);
CONDITION
FIS_Delete(FIS_HANDLE ** handle, FIS_DATA_TYPE type,
	   FIS_DATA_TYPE criteriaType, char *uid);
CONDITION
FIS_Get(FIS_HANDLE ** handle, FIS_DATA_TYPE type,
  FIS_DATA_TYPE criteriaType, char *uid, long listFlag, LST_HEAD * getList);
CONDITION
FIS_GetOne(FIS_HANDLE ** handle, FIS_DATA_TYPE type,
	FIS_DATA_TYPE criteriaType, const char *uid, long listFlag, void *record);
CONDITION
FIS_SelectOne(FIS_HANDLE ** handle, FIS_DATA_TYPE type,
	FIS_CRITERIA_TYPE criteriaType, const char *criteriaString,
	long listFlag, void *record);
CONDITION
FIS_GetWorkQueue(FIS_HANDLE ** handle, char* taskName, char* taskID,
		 long listFlag, LST_HEAD * getList);
CONDITION
FIS_DeleteWorkQueueRecord(FIS_HANDLE** handle, char* taskID);
CONDITION
FIS_Insert(FIS_HANDLE ** handle, FIS_DATA_TYPE type, void *record);
CONDITION
FIS_Update(FIS_HANDLE ** handle, FIS_DATA_TYPE type, void *r);
CONDITION
FIS_NewRecord(FIS_HANDLE ** handle, FIS_DATA_TYPE type, void *record);
void FIS_DumpRecord(void* record, FILE* f);

/* Define prototypes for functions for building/parsing objects
** for event notifications.
*/

CONDITION
FIS_SendEvent(FIS_HANDLE ** fis, DMAN_HANDLE ** dman,
       FIS_DATA_TYPE type, void *d, FIS_EVENT event, char *localApplication,
	      char *dst);
CONDITION
FIS_SendSet(FIS_HANDLE ** fis, DMAN_HANDLE ** dman,
	    FIS_DATA_TYPE type, void *d, char *localApplication, char *dst);

CONDITION
FIS_BuildObject(DCM_OBJECT ** obj, FIS_DATA_TYPE type, void *d,
		FIS_EVENT event);
CONDITION
FIS_ParseObject(DCM_OBJECT ** obj, FIS_DATA_TYPE type, void *d);
void FIS_Debug(CTNBOOLEAN flag);

#define	FIS_NORMAL	/* Normal return from FIS package */ \
	FORM_COND(FAC_FIS, SEV_SUCC, 1)
#define	FIS_ERROR	/* General error from FIS package */ \
	FORM_COND(FAC_FIS, SEV_ERROR, 2)
#define	FIS_OPENFAILED	/* General error when opening FIS tables */ \
	FORM_COND(FAC_FIS, SEV_ERROR, 3)
#define	FIS_MALLOCFAILURE	/* Failure to allocate memory */ \
	FORM_COND(FAC_FIS, SEV_ERROR, 4)
#define	FIS_CLOSEFAILED	/* General error when closing FIS tables */ \
	FORM_COND(FAC_FIS, SEV_ERROR, 5)
#define	FIS_ILLEGALRECORDTYPE	/* Caller passed illegal record to FIS */ \
	FORM_COND(FAC_FIS, SEV_ERROR, 6)
#define	FIS_NEWRECORDFAILED	/* Failed to create a new record */ \
	FORM_COND(FAC_FIS, SEV_ERROR, 7)
#define	FIS_INSERTFAILED	/* Failed to insert a record into FIS */ \
	FORM_COND(FAC_FIS, SEV_ERROR, 9)
#define	FIS_PATIENTINSERTFAILED	/* Failed to insert patient record into FIS */ \
	FORM_COND(FAC_FIS, SEV_ERROR, 10)
#define	FIS_GETFAILED	/* Failed to get a record from FIS */ \
	FORM_COND(FAC_FIS, SEV_ERROR, 11)
#define	FIS_PATIENTGETFAILED	/* Failed to get patient record from FIS */ \
	FORM_COND(FAC_FIS, SEV_ERROR, 12)
#define	FIS_DELETEFAILED	/* Failed to delete a record from FIS */ \
	FORM_COND(FAC_FIS, SEV_ERROR, 13)
#define	FIS_PATIENTDELETEFAILED	/* Failed to delete patient record from FIS */ \
	FORM_COND(FAC_FIS, SEV_ERROR, 14)
#define	FIS_NULLSTRING		/* Null string passed to FIS function */ \
	FORM_COND(FAC_FIS, SEV_ERROR, 15)
#define	FIS_UPDATEFAILED	/* Failed to update a record in FIS */ \
	FORM_COND(FAC_FIS, SEV_ERROR, 16)
#define	FIS_PATIENTUPDATEFAILED	/* Failed to update patient record in FIS */ \
	FORM_COND(FAC_FIS, SEV_ERROR, 17)
#define	FIS_STUDYGETFAILED	/* Failed to get study record from FIS */ \
	FORM_COND(FAC_FIS, SEV_ERROR, 19)
#define	FIS_STUDYDELETEFAILED	/* Failed to delete study record from FIS */ \
	FORM_COND(FAC_FIS, SEV_ERROR, 20)
#define	FIS_STUDYUPDATEFAILED	/* Failed to update study record in FIS */ \
	FORM_COND(FAC_FIS, SEV_ERROR, 21)
#define	FIS_ACCESSIONNUMBERFAILED	/* Failed to create accession number*/ \
	FORM_COND(FAC_FIS, SEV_ERROR, 24)
#define	FIS_MISSINGMANDATORYELEMENT	/* Record missing mandatory element */ \
	FORM_COND(FAC_FIS, SEV_ERROR, 26)
#define	FIS_RESULTSUPDATEFAILED	/* Failed to update results record in FIS */ \
	FORM_COND(FAC_FIS, SEV_ERROR, 27)
#define	FIS_INTERPRETATIONUPDATEFAILED	/* Failed to update interp record in FIS */ \
	FORM_COND(FAC_FIS, SEV_ERROR, 28)
#define	FIS_IDFAILED	/* Failed to create a new ID */ \
	FORM_COND(FAC_FIS, SEV_ERROR, 31)
#define	FIS_UIDFAILED	/* Failed to create a new UID */ \
	FORM_COND(FAC_FIS, SEV_ERROR, 32)
#define	FIS_REQUESTEDATTRIBUTEMISSING	/* Failed to parse a requested attribute */ \
	FORM_COND(FAC_FIS, SEV_WARN, 33)
#define	FIS_ILLEGALDELETECRITERIA	/* Illegal criteria for delete op */ \
	FORM_COND(FAC_FIS, SEV_ERROR, 34)
#define	FIS_SINGLEGETFAILED	/* Failed to retrieve req'sted single record */\
	FORM_COND(FAC_FIS, SEV_ERROR, 35)
#define	FIS_BUILDFAILED	/* Failed to build a record of a given type */\
	FORM_COND(FAC_FIS, SEV_ERROR, 36)
#define	FIS_SENDEVENTFAILED	/* Failed to send an event report*/\
	FORM_COND(FAC_FIS, SEV_ERROR, 37)
#define	FIS_SENDSETFAILED	/* Failed to send a set request */\
	FORM_COND(FAC_FIS, SEV_ERROR, 38)
#define	FIS_PARSEFAILED	/* Failed to parse a DICOM object into FIS structure */\
	FORM_COND(FAC_FIS, SEV_ERROR, 39)
#define	FIS_STORAGECOMMITREQUPDATEFAILED /* Failed to update storage commitment req record in FIS */ \
	FORM_COND(FAC_FIS, SEV_ERROR, 40)
#define FIS_WORKQUEUERECORDDELETEFAILED /* Failed to delete a work queue record */ \
	FORM_COND(FAC_FIS, SEV_ERROR, 41)

#ifdef  __cplusplus
}
#endif

#endif
