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
** Last Update:		$Author: smm $, $Date: 1998-10-22 17:47:39 $
** Source File:		$Source: /home/smm/ctn/ctn/facilities/irs/dicom_irs.h,v $
** Revision:		$Revision: 1.1 $
** Status:		$State: Exp $
*/

#ifndef _IRS_IS_IN
#define _IRS_IS_IN 1

#ifdef  __cplusplus
extern "C" {
#endif

typedef enum {
    IRS_K_WORKQUEUE
}   IRS_DATA_TYPE;

/* Define a set of critiera that can be used for searching the database */

typedef enum {
    IRS_K_ACCESSION
} IRS_CRITERIA_TYPE;

typedef void IRS_HANDLE;

typedef struct {
    void *reserved[2];
    IRS_DATA_TYPE Type;
    long Flag;
} IRS_GENERICRECORD;

#define	IRS_K_PAT_PATID		(1 << 0)
#define	IRS_K_PAT_PATNAM	(1 << 1)
#define	IRS_K_PAT_PATUID	(1 << 2)
#define	IRS_K_PAT_BIRDAT	(1 << 3)
#define	IRS_K_PAT_SEX		(1 << 4)
#define	IRS_K_PAT_STUDYLIST	(1 << 5)
#define	IRS_K_PAT_VISITLIST	(1 << 6)
#define	IRS_K_PAT_ALIASLIST	(1 << 7)

typedef struct {
    void *reserved[2];
    IRS_DATA_TYPE Type;
    long Flag;
    char PatID[DICOM_LO_LENGTH + 1];
    char PatNam[DICOM_PN_LENGTH + 1];
    char StuUID[DICOM_UI_LENGTH + 1];
    char AccNum[DICOM_SH_LENGTH + 1];
    char StuDat[DICOM_DA_LENGTH + 1];
    char InsAET[DICOM_AE_LENGTH + 1];

    char UpdPatID[DICOM_LO_LENGTH + 1];
    char UpdPatNam[DICOM_PN_LENGTH + 1];
    char UpdAccNum[DICOM_SH_LENGTH + 1];

    char Operator[65];
    char Comments[81];
    char Status[41];

    char UpdReqDat[DICOM_DA_LENGTH + 1];
    char UpdReqTim[DICOM_TM_LENGTH + 1];
    char UpdComDat[DICOM_DA_LENGTH + 1];
    char UpdComTim[DICOM_TM_LENGTH + 1];
}   IRS_WORKRECORD;


CONDITION IRS_Open(char *databaseName, IRS_HANDLE ** handle);
CONDITION IRS_Close(IRS_HANDLE ** handle);
CONDITION IRS_EnterWorkRecord(IRS_HANDLE** handle, const IRS_WORKRECORD *rec);
CONDITION IRS_GetWorkQueue(IRS_HANDLE** handle, LST_HEAD* lst);
CONDITION IRS_GetWorkRecord(IRS_HANDLE** handle, const char* studyUID,
			    IRS_WORKRECORD* rec);
CONDITION IRS_SetWorkRecordComplete(IRS_HANDLE** handle, const char* studyUID);
CONDITION IRS_DeleteCompletedEntries(IRS_HANDLE** handle, int deltaDays);

#if 0
void IRS_DumpRecord(void* record, FILE* f);

CONDITION
IRS_BuildObject(DCM_OBJECT ** obj, IRS_DATA_TYPE type, void *d,
		IRS_EVENT event);
CONDITION
IRS_ParseObject(DCM_OBJECT ** obj, IRS_DATA_TYPE type, void *d);
void IRS_Debug(CTNBOOLEAN flag);
#endif

#define	IRS_NORMAL	/* Normal return from IRS package */ \
	FORM_COND(FAC_IRS, SEV_SUCC, 1)
#define	IRS_ERROR	/* General error from IRS package */ \
	FORM_COND(FAC_IRS, SEV_ERROR, 2)
#define	IRS_OPENFAILED	/* General error when opening IRS tables */ \
	FORM_COND(FAC_IRS, SEV_ERROR, 3)
#define	IRS_MALLOCFAILURE	/* Failure to allocate memory */ \
	FORM_COND(FAC_IRS, SEV_ERROR, 4)
#define	IRS_CLOSEFAILED	/* General error when closing IRS tables */ \
	FORM_COND(FAC_IRS, SEV_ERROR, 5)
#define	IRS_ILLEGALRECORDTYPE	/* Caller passed illegal record to IRS */ \
	FORM_COND(FAC_IRS, SEV_ERROR, 6)
#define	IRS_NEWRECORDFAILED	/* Failed to create a new record */ \
	FORM_COND(FAC_IRS, SEV_ERROR, 7)
#define	IRS_INSERTFAILED	/* Failed to insert a record into IRS */ \
	FORM_COND(FAC_IRS, SEV_ERROR, 9)
#define	IRS_PATIENTINSERTFAILED	/* Failed to insert patient record into IRS */ \
	FORM_COND(FAC_IRS, SEV_ERROR, 10)
#define	IRS_SELECTFAILED	/* Failed to get a record from IRS */ \
	FORM_COND(FAC_IRS, SEV_ERROR, 11)
#define	IRS_PATIENTGETFAILED	/* Failed to get patient record from IRS */ \
	FORM_COND(FAC_IRS, SEV_ERROR, 12)
#define	IRS_DELETEFAILED	/* Failed to delete a record from IRS */ \
	FORM_COND(FAC_IRS, SEV_ERROR, 13)
#define	IRS_PATIENTDELETEFAILED	/* Failed to delete patient record from IRS */ \
	FORM_COND(FAC_IRS, SEV_ERROR, 14)
#define	IRS_NULLSTRING		/* Null string passed to IRS function */ \
	FORM_COND(FAC_IRS, SEV_ERROR, 15)
#define	IRS_UPDATEFAILED	/* Failed to update a record in IRS */ \
	FORM_COND(FAC_IRS, SEV_ERROR, 16)
#define	IRS_PATIENTUPDATEFAILED	/* Failed to update patient record in IRS */ \
	FORM_COND(FAC_IRS, SEV_ERROR, 17)
#define	IRS_STUDYGETFAILED	/* Failed to get study record from IRS */ \
	FORM_COND(FAC_IRS, SEV_ERROR, 19)
#define	IRS_STUDYDELETEFAILED	/* Failed to delete study record from IRS */ \
	FORM_COND(FAC_IRS, SEV_ERROR, 20)
#define	IRS_STUDYUPDATEFAILED	/* Failed to update study record in IRS */ \
	FORM_COND(FAC_IRS, SEV_ERROR, 21)
#define	IRS_ACCESSIONNUMBERFAILED	/* Failed to create accession number*/ \
	FORM_COND(FAC_IRS, SEV_ERROR, 24)
#define	IRS_MISSINGMANDATORYELEMENT	/* Record missing mandatory element */ \
	FORM_COND(FAC_IRS, SEV_ERROR, 26)
#define	IRS_RESULTSUPDATEFAILED	/* Failed to update results record in IRS */ \
	FORM_COND(FAC_IRS, SEV_ERROR, 27)
#define	IRS_INTERPRETATIONUPDATEFAILED	/* Failed to update interp record in IRS */ \
	FORM_COND(FAC_IRS, SEV_ERROR, 28)
#define	IRS_IDFAILED	/* Failed to create a new ID */ \
	FORM_COND(FAC_IRS, SEV_ERROR, 31)
#define	IRS_UIDFAILED	/* Failed to create a new UID */ \
	FORM_COND(FAC_IRS, SEV_ERROR, 32)
#define	IRS_REQUESTEDATTRIBUTEMISSING	/* Failed to parse a requested attribute */ \
	FORM_COND(FAC_IRS, SEV_WARN, 33)
#define	IRS_ILLEGALDELETECRITERIA	/* Illegal criteria for delete op */ \
	FORM_COND(FAC_IRS, SEV_ERROR, 34)
#define	IRS_SINGLEGETFAILED	/* Failed to retrieve req'sted single record */\
	FORM_COND(FAC_IRS, SEV_ERROR, 35)
#define	IRS_BUILDFAILED	/* Failed to build a record of a given type */\
	FORM_COND(FAC_IRS, SEV_ERROR, 36)
#define	IRS_SENDEVENTFAILED	/* Failed to send an event report*/\
	FORM_COND(FAC_IRS, SEV_ERROR, 37)
#define	IRS_SENDSETFAILED	/* Failed to send a set request */\
	FORM_COND(FAC_IRS, SEV_ERROR, 38)
#define	IRS_PARSEFAILED	/* Failed to parse a DICOM object into IRS structure */\
	FORM_COND(FAC_IRS, SEV_ERROR, 39)
#define	IRS_STORAGECOMMITREQUPDATEFAILED /* Failed to update storage commitment req record in IRS */ \
	FORM_COND(FAC_IRS, SEV_ERROR, 40)
#define IRS_WORKQUEUERECORDDELETEFAILED /* Failed to delete a work queue record */ \
	FORM_COND(FAC_IRS, SEV_ERROR, 41)

#ifdef  __cplusplus
}
#endif

#endif
