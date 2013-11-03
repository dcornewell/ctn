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
+-+-+-+-+-+-+-+-+-
*/
/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	work
** Author, Date:	Stephen Moore, 11-Feb-94
** Intent:		This module contains the functions which read
**			information from an image database and populate a FIS
**			to be used for the Results Storage Application.  The
**			public entry is "work".
** Last Update:		$Author: smm $, $Date: 1997-05-30 19:54:30 $
** Source File:		$RCSfile: work.c,v $
** Revision:		$Revision: 1.8 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.8 $ $RCSfile: work.c,v $";

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#ifdef MACH
#include <unistd.h>
#endif

#include "dicom.h"
#include "condition.h"
#include "tbl.h"
#include "lst.h"
#include "dicom_objects.h"
#include "tbl.h"
#include "manage.h"
#include "idb.h"
#include "fis.h"

extern char *retrieveAETitle;

typedef struct {
    void *reserved[2];
    IDB_Query query;
}   QUERY_LIST_ITEM;

static CONDITION
    selectCallback(IDB_Query * queryResponse, long count, LST_HEAD * lst);
static CONDITION
insertPatient(IDB_PatientQuery * p, IDB_HANDLE ** idb,
	      FIS_HANDLE ** fis);
static CONDITION
insertStudy(char *patUID, IDB_Query * q, IDB_HANDLE ** idb,
	    FIS_HANDLE ** fis);
static CONDITION
insertStudyComponent(char *patUID, IDB_Query * q, IDB_HANDLE ** idb,
		     FIS_HANDLE ** fis);
static void trimcopy(char *dst, const char *src, unsigned long nullFlag);


/* work
**
** Purpose:
**	Provide a high level driving function for loading data into the FIS.
**	This function extracts all of the patients from the image database
**	and calls insertPatient to insert that patient into the FIS.
**
** Parameter Dictionary:
**	idb	The handle for accessing the image database.
**	fis	The handle for accessing the FIS.
**
** Return Values:
**	1 for success
**	0 for failure
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
work(IDB_HANDLE ** idb, FIS_HANDLE ** fis)
{
    CONDITION cond;
    LST_HEAD *lst = NULL;
    long selectCount;

    IDB_Query
	queryStructure;
    QUERY_LIST_ITEM
	* queryItem;

    if (lst == NULL) {
	lst = LST_Create();
	if (lst == NULL)
	    return 0;
    }
    memset(&queryStructure, 0, sizeof(queryStructure));
    queryStructure.PatientQFlag = QF_PAT_PatID;
    queryStructure.StudyQFlag = 0;
    queryStructure.SeriesQFlag = 0;
    queryStructure.ImageQFlag = 0;
    queryStructure.PatientNullFlag = QF_PAT_PatID;
    queryStructure.StudyNullFlag = 0;
    queryStructure.SeriesNullFlag = 0;
    queryStructure.ImageNullFlag = 0;
    cond = IDB_Select(idb, PATIENT_ROOT, IDB_PATIENT_LEVEL, IDB_PATIENT_LEVEL,
		      &queryStructure, &selectCount, selectCallback, lst);
    if ((cond != IDB_NORMAL) && (cond != IDB_NOMATCHES)) {
	COND_DumpConditions();
	return 0;
    }
    while ((queryItem = LST_Dequeue(&lst)) != NULL) {
	printf("%s\n", queryItem->query.patient.PatNam);
	cond = insertPatient(&queryItem->query.patient, idb, fis);
	if (cond != 1)
	    return 0;
	(void) free(queryItem);
    }

    return 1;
}

/* selectCallback
**
** Purpose:
**	Callback function called for each record found when searching the
**	image database.  Make a copy of the record and append it to the
**	list.
**
** Parameter Dictionary:
**	queryResponse	Pointer to a query response structure.  We don't own
**			it, but we can copy it.
**	count		The record number for this record (although records
**			are not stored in any particular order).
**	lst		The handle for the list.  Add the query response to
**			this list.
**
** Return Values:
**	IDB_NORMAL	if we can add the structure to the list
**	0		if some failure occurred
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
selectCallback(IDB_Query * queryResponse, long count, LST_HEAD * lst)
{
    QUERY_LIST_ITEM
    * item;

    item = malloc(sizeof(*item));
    if (item == NULL)
	return 0;

    item->query = *queryResponse;
    (void) LST_Enqueue(&lst, item);
    return IDB_NORMAL;
}

/* insertPatient
**
** Purpose:
**	The driving function for inserting one patient into the FIS with
**	the associated studies for that patient.  This function reads the
**	set of studies for the patient from the image database and inserts
**	each study into the FIS by calling insertStudy.
**
** Parameter Dictionary:
**	p	A patient query structure which contains informat needed
**		to insert the patient into the FIS and to select studies from
**		the image database.
**	idb	The handle for the image database so we can select studies.
**	fis	The handle for the FIS used for inserting data.
** Return Values:
**	1 if we can insert the patient and studies successfully
**	0 if some failure occurs
** Notes:
**
** Algorithm:
*/

static CONDITION
insertPatient(IDB_PatientQuery * p, IDB_HANDLE ** idb,
	      FIS_HANDLE ** fis)
{
    CONDITION cond;
    FIS_PATIENTRECORD patientFIS;
    IDB_Query queryStructure;
    QUERY_LIST_ITEM *queryItem;
    static LST_HEAD *lst = NULL;
    long selectCount;

    patientFIS.Type = FIS_K_PATIENT;
    cond = FIS_NewRecord(fis, FIS_K_PATIENT, &patientFIS);
    if (cond != FIS_NORMAL)
	return 0;
    trimcopy(patientFIS.PatID, p->PatID, 0);
    trimcopy(patientFIS.PatNam, p->PatNam, 0);
    trimcopy(patientFIS.PatBirDat, p->PatBirDat, 0);
    trimcopy(patientFIS.PatSex, p->PatSex, 0);
    patientFIS.Flag = FIS_K_PAT_PATID | FIS_K_PAT_PATNAM |
	FIS_K_PAT_PATUID | FIS_K_PAT_BIRDAT | FIS_K_PAT_SEX;
    cond = FIS_Insert(fis, FIS_K_PATIENT, &patientFIS);
    if (cond != FIS_NORMAL)
	return 0;

    if (lst == NULL) {
	lst = LST_Create();
	if (lst == NULL)
	    return 0;
    }
    memset(&queryStructure, 0, sizeof(queryStructure));
    queryStructure.PatientQFlag = QF_PAT_PatID;
    queryStructure.StudyQFlag = 0;
    queryStructure.SeriesQFlag = 0;
    queryStructure.ImageQFlag = 0;
    queryStructure.PatientNullFlag = 0;
    queryStructure.StudyNullFlag = 0;
    queryStructure.SeriesNullFlag = 0;
    queryStructure.ImageNullFlag = 0;
    strcpy(queryStructure.patient.PatID, p->PatID);
    cond = IDB_Select(idb, PATIENT_ROOT, IDB_PATIENT_LEVEL, IDB_STUDY_LEVEL,
		      &queryStructure, &selectCount, selectCallback, lst);
    if ((cond != IDB_NORMAL) && (cond != IDB_NOMATCHES)) {
	COND_DumpConditions();
	return 0;
    }
    while ((queryItem = LST_Dequeue(&lst)) != NULL) {
	printf("%s\n", queryItem->query.study.AccNum);
	cond = insertStudy(patientFIS.PatUID, &queryItem->query, idb, fis);
	if (cond != 1)
	    return 0;

	(void) free(queryItem);
    }
    return 1;
}

/* insertStudy
**
** Purpose:
**	The driving function for inserting one study into the FIS.  This
**	function inserts the study and then calls other functions to insert
**	other records that are related to the study (like study component,
**	interpretation).
**
** Parameter Dictionary:
**	patUID	So we can link this study to the right patient
**	q	A query structure that contains some study information needed
**		to make a study record for the FIS
**	idb	The handle for the image database so we can select studies.
**	fis	The handle for the FIS used for inserting data.
** Return Values:
**	1 if we can insert the patient and studies successfully
**	0 if some failure occurs
**
** Notes:
**	Given that we call this function once for each image, it is possible
**	(likely) that the study will already exist in the FIS.  If it does
**	exist, that is not an error.  We just go on and insert other pieces
**	of the study.
** Algorithm:
*/

static CONDITION
insertStudy(char *patUID, IDB_Query * q, IDB_HANDLE ** idb,
	    FIS_HANDLE ** fis)
{
    CONDITION cond;
    FIS_STUDYRECORD studyFIS;
    FIS_RESULTSRECORD resultsFIS;
    FIS_INTERPRETATIONRECORD interpretationFIS;
    IDB_Query queryStructure;
    QUERY_LIST_ITEM *queryItem;
    static LST_HEAD *lst = NULL;
    long selectCount;

/*  If the study does not already exist in the FIS, insert it.
*/
    memset(&studyFIS, 0, sizeof(studyFIS));
    studyFIS.Type = FIS_K_STUDY;
    cond = FIS_GetOne(fis, FIS_K_STUDY, FIS_K_STUDY, q->study.StuInsUID,
		      0, &studyFIS);
    if (cond != FIS_NORMAL) {	/* Assume the study does not yet exist */

	(void) COND_PopCondition(TRUE);

	memset(&studyFIS, 0, sizeof(studyFIS));
	studyFIS.Type = FIS_K_STUDY;
	trimcopy(studyFIS.PatUID, patUID, 0);
	trimcopy(studyFIS.StuInsUID, q->study.StuInsUID, 0);
	trimcopy(studyFIS.AccNum, q->study.AccNum,
		 q->StudyNullFlag & QF_STU_AccNum);
	trimcopy(studyFIS.StuID, q->study.StuID,
		 q->StudyNullFlag & QF_STU_StuID);
	trimcopy(studyFIS.SchStuStaDat, q->study.StuDat, 0);
	trimcopy(studyFIS.SchStuStaTim, q->study.StuTim, 0);
	trimcopy(studyFIS.StuStaID, "VERIFIED", 0);

	studyFIS.Flag = FIS_K_STU_PATUID | FIS_K_STU_STUINSUID |
	    FIS_K_STU_VISUID | FIS_K_STU_ACCNUM |
	    FIS_K_STU_STUID | FIS_K_STU_SCHSTUSTADAT |
	    FIS_K_STU_SCHSTUSTATIM | FIS_K_STU_STUSTAID;

	cond = FIS_Insert(fis, FIS_K_STUDY, &studyFIS);
	if (cond != FIS_NORMAL)
	    return 0;

	memset(&resultsFIS, 0, sizeof(resultsFIS));
	resultsFIS.Type = FIS_K_RESULTS;
	cond = FIS_NewRecord(fis, FIS_K_RESULTS, &resultsFIS);
	if (cond != FIS_NORMAL)
	    return 0;

	trimcopy(resultsFIS.StuInsUID, studyFIS.StuInsUID, 0);
	trimcopy(resultsFIS.Imp, "No impressions", 0);

	resultsFIS.Flag |= FIS_K_RESULTS_RESUID | FIS_K_RESULTS_STUINSUID |
	    FIS_K_RESULTS_RESID | FIS_K_RESULTS_IMP;

	cond = FIS_Insert(fis, FIS_K_RESULTS, &resultsFIS);
	if (cond != FIS_NORMAL)
	    return 0;

	memset(&interpretationFIS, 0, sizeof(interpretationFIS));
	interpretationFIS.Type = FIS_K_INTERPRETATION;
	cond = FIS_NewRecord(fis, FIS_K_INTERPRETATION, &interpretationFIS);
	if (cond != FIS_NORMAL)
	    return 0;

	trimcopy(interpretationFIS.ResUID, resultsFIS.ResUID, 0);
	trimcopy(interpretationFIS.IntTex, "No interpretation at this time", 0);
	trimcopy(interpretationFIS.IntTypID, "REPORT", 0);
	trimcopy(interpretationFIS.IntStaID, "TRANSCRIBED", 0);

	interpretationFIS.Flag |= FIS_K_INTERP_INTUID | FIS_K_INTERP_RESUID |
	    FIS_K_INTERP_INTTEX | FIS_K_INTERP_INTID |
	    FIS_K_INTERP_INTTYPID | FIS_K_INTERP_INTSTAID;

	cond = FIS_Insert(fis, FIS_K_INTERPRETATION, &interpretationFIS);
	if (cond != FIS_NORMAL)
	    return 0;
    }
    if (lst == NULL) {
	lst = LST_Create();
	if (lst == NULL)
	    return 0;
    }
/*  Find the set of series for this study.
*/

    memset(&queryStructure, 0, sizeof(queryStructure));
    queryStructure.PatientQFlag = QF_PAT_PatID;
    queryStructure.StudyQFlag = QF_STU_StuInsUID;
    queryStructure.SeriesQFlag = 0;
    queryStructure.ImageQFlag = 0;
    queryStructure.PatientNullFlag = 0;
    queryStructure.StudyNullFlag = 0;
    queryStructure.SeriesNullFlag = 0;
    queryStructure.ImageNullFlag = 0;
    strcpy(queryStructure.patient.PatID, q->patient.PatID);
    strcpy(queryStructure.study.StuInsUID, q->study.StuInsUID);
    cond = IDB_Select(idb, PATIENT_ROOT, IDB_PATIENT_LEVEL, IDB_SERIES_LEVEL,
		      &queryStructure, &selectCount, selectCallback, lst);
    if ((cond != IDB_NORMAL) && (cond != IDB_NOMATCHES)) {
	COND_DumpConditions();
	return 0;
    }
/*  For each series, insert a study component fragment.
*/

    while ((queryItem = LST_Dequeue(&lst)) != NULL) {
	printf("%s\n", queryItem->query.series.SerInsUID);
	cond = insertStudyComponent(patUID, &queryItem->query, idb, fis);
	if (cond != 1)
	    return 0;

	(void) free(queryItem);
    }
    return 1;
}

/* insertStudyComponent
**
** Purpose:
**	This function inserts the study component information into the
**	FIS for one series in a given study.  This function tries to insert
**	the top level study component record.  Then, it inserts the
**	series record and tries to insert records for each image in
**	the series.
**
** Parameter Dictionary:
**	patUID	Patient UID which is not used, but kept around to keep
**		the calling sequences the same.
**	q	A query structure used for yanking other information from
**		the image database.
**	idb	The handle for the image database so we can select studies.
**	fis	The handle for the FIS used for inserting data.
** Return Values:
**	1 if we can insert the patient and studies successfully
**	0 if some failure occurs
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
insertStudyComponent(char *patUID, IDB_Query * q, IDB_HANDLE ** idb,
		     FIS_HANDLE ** fis)
{
    CONDITION cond;
    FIS_STUDYCOMPONENTRECORD studyComponentFIS;
    FIS_SCSERIESRECORD scSeries;
    FIS_SCIMAGERECORD scImage;
    IDB_Query queryStructure;
    QUERY_LIST_ITEM *queryItem;
    static LST_HEAD *lst = NULL;
    long selectCount;

/*  See if the study component record already exists.  If not, insert it.
*/
    memset(&studyComponentFIS, 0, sizeof(studyComponentFIS));
    studyComponentFIS.Type = FIS_K_STUDYCOMPONENT;
    cond = FIS_GetOne(fis, FIS_K_STUDYCOMPONENT, FIS_K_STUDY,
		      q->study.StuInsUID, 0, &studyComponentFIS);
    if (cond != FIS_NORMAL) {	/* Assume study component does not yet exist */
	(void) COND_PopCondition(TRUE);

	memset(&studyComponentFIS, 0, sizeof(studyComponentFIS));
	studyComponentFIS.Type = FIS_K_STUDYCOMPONENT;
	cond = FIS_NewRecord(fis, FIS_K_STUDYCOMPONENT, &studyComponentFIS);
	if (cond != FIS_NORMAL)
	    return 0;

	trimcopy(studyComponentFIS.StuInsUID, q->study.StuInsUID, 0);
	trimcopy(studyComponentFIS.Mod, q->series.Mod, 0);
	if (q->StudyNullFlag & QF_STU_StuDes)
	    trimcopy(studyComponentFIS.StuDes, "No Description Available", 0);
	else
	    trimcopy(studyComponentFIS.StuDes, q->study.StuDes, 0);
	trimcopy(studyComponentFIS.ProCodVal, "87", 0);
	trimcopy(studyComponentFIS.ProCodSchDes, "I9C", 0);
	trimcopy(studyComponentFIS.ProCodMea, "Diagnostic Radiology", 0);
	trimcopy(studyComponentFIS.StuComStaID, "VERIFIED", 0);
	trimcopy(studyComponentFIS.StuID, q->study.StuID,
		 q->StudyNullFlag & QF_STU_StuID);

	studyComponentFIS.Flag = FIS_K_STUDYCOMP_STUCOMUID |
	    FIS_K_STUDYCOMP_STUINSUID | FIS_K_STUDYCOMP_MOD |
	    FIS_K_STUDYCOMP_STUDES | FIS_K_STUDYCOMP_PROCODVAL |
	    FIS_K_STUDYCOMP_PROCODSCHDES | FIS_K_STUDYCOMP_PROCODMEA |
	    FIS_K_STUDYCOMP_STUCOMSTAID | FIS_K_STUDYCOMP_STUID;

	cond = FIS_Insert(fis, FIS_K_STUDYCOMPONENT, &studyComponentFIS);
	if (cond != FIS_NORMAL)
	    return 0;
    }
/*  See if the series record already exists in the FIS.  If not, insert it.
*/

    memset(&scSeries, 0, sizeof(scSeries));
    scSeries.Type = FIS_K_SCSERIES;
    cond = FIS_GetOne(fis, FIS_K_SCSERIES, FIS_K_SCSERIES,
		      q->series.SerInsUID, 0, &scSeries);
    if (cond != FIS_NORMAL) {	/* Assume series not part of this Study Comp */
	memset(&scSeries, 0, sizeof(scSeries));
	scSeries.Type = FIS_K_SCSERIES;
	trimcopy(scSeries.SerInsUID, q->series.SerInsUID, 0);
	trimcopy(scSeries.StuComUID, studyComponentFIS.StuComUID, 0);
	trimcopy(scSeries.SerDat, "", 0);
	trimcopy(scSeries.SerTim, "", 0);
	scSeries.Flag =
	    FIS_K_SCSERIES_SERINSUID | FIS_K_SCSERIES_STUCOMUID |
	    FIS_K_SCSERIES_SERDAT | FIS_K_SCSERIES_SERTIM;

	if (retrieveAETitle != NULL) {
	    trimcopy(scSeries.RetAETit, retrieveAETitle, 0);
	    scSeries.Flag |= FIS_K_SCSERIES_RETAETIT;
	}
	cond = FIS_Insert(fis, FIS_K_SCSERIES, &scSeries);
	if (cond != FIS_NORMAL)
	    return 0;
    }
/*  Get all of the images for this series and insert them into the FIS
**  as part of this study component/series.
*/

    if (lst == NULL) {
	lst = LST_Create();
	if (lst == NULL)
	    return 0;
    }
    memset(&queryStructure, 0, sizeof(queryStructure));
    queryStructure.PatientQFlag = QF_PAT_PatID;
    queryStructure.StudyQFlag = QF_STU_StuInsUID;
    queryStructure.SeriesQFlag = QF_SER_SerInsUID;
    queryStructure.ImageQFlag = 0;
    queryStructure.PatientNullFlag = 0;
    queryStructure.StudyNullFlag = 0;
    queryStructure.SeriesNullFlag = 0;
    queryStructure.ImageNullFlag = 0;
    strcpy(queryStructure.patient.PatID, q->patient.PatID);
    strcpy(queryStructure.study.StuInsUID, q->study.StuInsUID);
    strcpy(queryStructure.series.SerInsUID, q->series.SerInsUID);

    cond = IDB_Select(idb, PATIENT_ROOT, IDB_PATIENT_LEVEL, IDB_IMAGE_LEVEL,
		      &queryStructure, &selectCount, selectCallback, lst);
    if ((cond != IDB_NORMAL) && (cond != IDB_NOMATCHES)) {
	COND_DumpConditions();
	return 0;
    }
    while ((queryItem = LST_Dequeue(&lst)) != NULL) {
	printf("%s\n", queryItem->query.image.SOPInsUID);

	memset(&scImage, 0, sizeof(scImage));
	scImage.Type = FIS_K_SCIMAGE;
	trimcopy(scImage.SOPInsUID, queryItem->query.image.SOPInsUID, 0);
	trimcopy(scImage.SOPClaUID, queryItem->query.image.SOPClaUID, 0);
	trimcopy(scImage.SerInsUID, queryItem->query.series.SerInsUID, 0);
	trimcopy(scImage.StuComUID, studyComponentFIS.StuComUID, 0);
	scImage.Flag =
	    FIS_K_SCIMAGE_SOPINSUID | FIS_K_SCIMAGE_SOPCLAUID |
	    FIS_K_SCIMAGE_SERINSUID | FIS_K_SCIMAGE_STUCOMUID;

	cond = FIS_Insert(fis, FIS_K_SCIMAGE, &scImage);
	if (cond != FIS_NORMAL)
	    return 0;

	(void) free(queryItem);
    }

    return 1;
}

/* trimcopy
**
** Purpose:
**	A utility function that copies strings while trimming off
**	trailing spaces.
**
** Parameter Dictionary:
**	dst	The destination string, allocated by the caller.
**	src	The source string, 0 terminated.
** Return Values:
**	none
** Notes:
**
** Algorithm:
*/

static void
trimcopy(char *dst, const char *src, unsigned long nullFlag)
{
    int i;

    if (nullFlag) {
	*dst = '\0';
	return;
    }
    while (*src == ' ' || *src == 0x8)
	src++;

    strcpy(dst, src);
    i = strlen(dst);

    while (--i > 0) {
	if (dst[i] == ' ' || dst[i] == 0x9)
	    dst[i] = '\0';
	else
	    break;
    }
}
