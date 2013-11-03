/*
          Copyright (C) 1998 Washington University

          The software and supporting documentation for the Mallinckrodt
	  Institute of Radiology Digital Imaging and Communications in
	  Medicine (DICOM) Project were developed at the
                  Electronic Radiology Laboratory
                  Mallinckrodt Institute of Radiology
                  Washington University School of Medicine
                  510 S. Kingshighway Blvd.
                  St. Louis, MO 63110

          THIS SOFTWARE IS MADE AVAILABLE, AS IS, AND WASHINGTON UNIVERSITY
	  MAKES NO WARRANTY ABOUT THE SOFTWARE, ITS
          PERFORMANCE, ITS MERCHANTABILITY OR FITNESS FOR ANY PARTICULAR
          USE, FREEDOM FROM ANY COMPUTER DISEASES OR ITS CONFORMITY TO ANY
          SPECIFICATION. THE ENTIRE RISK AS TO QUALITY AND PERFORMANCE OF
          THE SOFTWARE IS WITH THE USER.

          Copyright of the software and supporting documentation is
          owned by Washington University, and free access
          is hereby granted as a license to use this software, copy this
          software and prepare derivative works based upon this software.
          However, any distribution of this software source code or
          supporting documentation or derivative works (source code and
          supporting documentation) must include the three paragraphs of
          the copyright notice.
*/

/* Copyright marker.  Copyright will be inserted above.  Do not remove */

/*
**				DICOM 98
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
**	private functions
** Author, Date:	Stephen M. Moore, 25-Oct-1997
** Intent:		Provide functions for DICOM Directory services
** Last Update:		$Author: smm $, $Date: 2004-09-23 03:44:23 $
** Source File:		$RCSfile: ddr.c,v $
** Revision:		$Revision: 1.3 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.3 $ $RCSfile: ddr.c,v $";

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#ifdef MALLOC_DEBUG
#include "malloc.h"
#endif
#ifdef GCCSUNOS
#include <unistd.h>
#endif

#ifdef _MSC_VER
#else
#include <sys/file.h>
#endif

#ifdef SOLARIS
#include <sys/fcntl.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "dicom_uids.h"
#include "dicom_objects.h"
#include "dicom_ddr.h"

static CTNBOOLEAN debug = FALSE;/* Flag for debugging messages to stdout */

/* First part of the file contains functions that are private to this facility.
** The public entry points will be defined after the private functions.
*/

static CONDITION
addPatientRecord(DCM_OBJECT ** patObject, LST_HEAD ** lst)
{
    CONDITION cond;
    DDR_PATIENT patientRecord,
       *patientPtr;
    DCM_ELEMENT required[] = {
	{DCM_PATNAME, DCM_PN, "", 1, sizeof(patientRecord.PatientName), NULL},
	{DCM_PATID, DCM_LO, "", 1, sizeof(patientRecord.PatientID), NULL},
	{DCM_DIRLOWERLEVELOFFSET, DCM_UL, "", 1,
	sizeof(patientRecord.StudyLinkOffset), NULL},
    };
    DCM_FLAGGED_ELEMENT optional[] = {
	{DCM_PATBIRTHDATE, DCM_DA, "", 1, sizeof(patientRecord.BirthDate), NULL, 0, NULL},
	{DCM_PATSEX, DCM_CS, "", 1, sizeof(patientRecord.Sex), NULL, 0, NULL}
    };
    required[0].d.string = patientRecord.PatientName;
    required[1].d.string = patientRecord.PatientID;
    required[2].d.ul = &patientRecord.StudyLinkOffset;
    optional[0].e.d.string = patientRecord.BirthDate;
    optional[0].flagAddress = &patientRecord.optionFlag;
    optional[1].e.d.string = patientRecord.Sex;
    optional[1].flagAddress = &patientRecord.optionFlag;

    memset(&patientRecord, 0, sizeof(patientRecord));
    cond = DCM_ParseObject(patObject, required, 3, optional, 2, NULL);
    if (cond != DCM_NORMAL) {
	return 0;		/* repair */
    }
    patientPtr = malloc(sizeof(*patientPtr));
    if (patientPtr == NULL) {
	return 0;		/* repair */
    }
    memcpy(patientPtr, &patientRecord, sizeof(patientRecord));
    LST_Enqueue(lst, patientPtr);
    return DDR_NORMAL;
}

static CONDITION
addStudyRecord(DCM_OBJECT ** studyObject, LST_HEAD ** lst)
{
    CONDITION cond;
    DDR_STUDY record,
       *studyPtr;
    DCM_ELEMENT required[] = {
	{DCM_IDSTUDYDATE, DCM_DA, "", 1, sizeof(record.StudyDate), NULL},
	{DCM_IDSTUDYTIME, DCM_TM, "", 1, sizeof(record.StudyTime), NULL},
	{DCM_IDSTUDYDESCRIPTION, DCM_LO, "", 1,
	sizeof(record.StudyDescription), NULL},
	{DCM_RELSTUDYINSTANCEUID, DCM_UI, "", 1,
	sizeof(record.StudyInstanceUID), NULL},
	{DCM_RELSTUDYID, DCM_SH, "", 1, sizeof(record.StudyID), NULL},
	{DCM_IDACCESSIONNUMBER, DCM_SH, "", 1,
	sizeof(record.AccessionNumber), NULL},
	{DCM_DIRLOWERLEVELOFFSET, DCM_UL, "", 1,
	sizeof(record.SeriesLinkOffset), NULL}
    };

    required[0].d.string = record.StudyDate;
    required[1].d.string = record.StudyTime;
    required[2].d.string = record.StudyDescription;
    required[3].d.string = record.StudyInstanceUID;
    required[4].d.string = record.StudyID;
    required[5].d.string = record.AccessionNumber;
    required[6].d.ul = &record.SeriesLinkOffset;

    memset(&record, 0, sizeof(record));
    cond = DCM_ParseObject(studyObject, required,
			   (int) DIM_OF(required), NULL, 0, NULL);
    if (cond != DCM_NORMAL) {
	return 0;		/* repair */
    }
    studyPtr = malloc(sizeof(*studyPtr));
    if (studyPtr == NULL) {
	return 0;		/* repair */
    }
    memcpy(studyPtr, &record, sizeof(record));
    LST_Enqueue(lst, studyPtr);
    return DDR_NORMAL;
}

static CONDITION
addSeriesRecord(DCM_OBJECT ** seriesObject, LST_HEAD ** lst)
{
    CONDITION cond;
    DDR_SERIES record,
       *seriesPtr;
    DCM_ELEMENT required[] = {
	{DCM_IDMODALITY, DCM_CS, "", 1, sizeof(record.Modality), NULL},
	{DCM_RELSERIESINSTANCEUID, DCM_UI, "", 1,
	sizeof(record.SeriesInstanceUID), NULL},
	{DCM_RELSERIESNUMBER, DCM_IS, "", 1,
	sizeof(record.SeriesNumber), NULL},
	{DCM_DIRLOWERLEVELOFFSET, DCM_UL, "", 1,
	sizeof(record.LeafLinkOffset), NULL}
    };

    required[0].d.string = record.Modality;
    required[1].d.string = record.SeriesInstanceUID;
    required[2].d.string = record.SeriesNumber;
    required[3].d.ul = &record.LeafLinkOffset;

    memset(&record, 0, sizeof(record));
    cond = DCM_ParseObject(seriesObject, required,
			   (int) DIM_OF(required), NULL, 0, NULL);
    if (cond != DCM_NORMAL) {
	return 0;		/* repair */
    }
    seriesPtr = malloc(sizeof(*seriesPtr));
    if (seriesPtr == NULL) {
	return 0;		/* repair */
    }
    memcpy(seriesPtr, &record, sizeof(record));
    LST_Enqueue(lst, seriesPtr);
    return DDR_NORMAL;
}

static void
addImageLeafAttributes(DCM_OBJECT ** leafObject, DDR_SERIES_LEAF * leaf)
{
    CONDITION cond;
    DCM_ELEMENT required[] = {
	{DCM_RELIMAGENUMBER, DCM_IS, "", 1,
	sizeof(leaf->specific.image.ImageNumber), NULL}
    };
    required[0].d.string = leaf->specific.image.ImageNumber;
    cond = DCM_ParseObject(leafObject, required,
			   (int) DIM_OF(required), NULL, 0, NULL);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	exit(1);		/* repair */
    }
}

static CONDITION
addLeafRecord(DCM_OBJECT ** leafObject, LST_HEAD ** lst)
{
    CONDITION cond;
    DDR_SERIES_LEAF record,
       *leafPtr;
    DCM_ELEMENT required[] = {
	{DCM_DIRRECORDTYPE, DCM_CS, "", 1, sizeof(record.RecordType), NULL},
	{DCM_DIRREFERENCEDFILEID, DCM_CS, "", 1,
	sizeof(record.FileID), NULL},
	{DCM_DIRREFSOPCLASSUID, DCM_UI, "", 1,
	sizeof(record.SOPClassUID), NULL},
	{DCM_DIRREFSOPINSTANCEUID, DCM_UI, "", 1,
	sizeof(record.SOPInstanceUID), NULL},
	{DCM_DIRREFTRANSFERSYNTAXUID, DCM_UI, "", 1,
	sizeof(record.TransferSyntaxUID), NULL}
    };

    required[0].d.string = record.RecordType;
    required[1].d.string = record.FileID;
    required[2].d.string = record.SOPClassUID;
    required[3].d.string = record.SOPInstanceUID;
    required[4].d.string = record.TransferSyntaxUID;

    memset(&record, 0, sizeof(record));
    cond = DCM_ParseObject(leafObject, required,
			   (int) DIM_OF(required), NULL, 0, NULL);
    if (cond != DCM_NORMAL) {
	return 0;		/* repair */
    }
    if (strcmp(record.RecordType, "IMAGE") == 0)
	addImageLeafAttributes(leafObject, &record);

    leafPtr = malloc(sizeof(*leafPtr));
    if (leafPtr == NULL) {
	return 0;		/* repair */
    }
    memcpy(leafPtr, &record, sizeof(record));
    LST_Enqueue(lst, leafPtr);

    return DDR_NORMAL;
}

static U32
offsetNextRecord(DCM_OBJECT ** obj)
{
    CONDITION cond;
    U32 offset;
    DCM_ELEMENT e = {DCM_DIRNEXTRECORDOFFSET, DCM_UL, "", 1,
    sizeof(offset), NULL};

    e.d.ul = &offset;
    cond = DCM_ParseObject(obj, &e, 1, NULL, 0, NULL);
    if (cond != DCM_NORMAL) {
	return 0;		/* repair */
    }
    return offset;
}

/* Public DDR functions are defined below this point. */


/* DDR_GetFileSetID
**
** Purpose:
**	DDR_GetFileSetID examines a DICOM object which contains DICOMDIR
**	information and returns the information in the File-Set Identification
**	Module to the caller. The attributes in the File-Set Identification
**	Module are defined in PS 3.3, Table F.3-2.  All attributes are
**	returned as ASCIZ strings in the caller's DDR_FILESETID argument.
**	If an attribute is missing in in the DICOMDIR information (a type 3
**	attribute), that attribute is returned to the caller as a zero-length
**	string.
**
** Parameter Dictionary:
**	obj		A DICOM Object that represents a DICOMDIR object.
**	fileSetID	DDR_FILESETID structure allocated by the caller.  Fields
**			will be filled in by this function.
**
** Return Values:
**	DDR_NORMAL
**	DDR_ERROR
*/


CONDITION
DDR_GetFileSetID(DCM_OBJECT ** obj, DDR_FILESETID * fileSetID)
{
    static long flag;
    DCM_ELEMENT e = {DCM_DIRFILESETID, DCM_CS, "", 1,
    DICOM_CS_LENGTH + 1, NULL};
    DCM_FLAGGED_ELEMENT optionalElements[] = {
	{DCM_DIRFILESETDESCRFILEID, DCM_CS, "", 1,
	DICOM_CS_LENGTH + 1, NULL, 0, &flag},
	{DCM_DIRSPECIFICCHARACTER, DCM_CS, "", 1,
	DICOM_CS_LENGTH + 1, NULL, 0, &flag}
    };

    CONDITION cond;

    memset(fileSetID, 0, sizeof(fileSetID));
    e.d.string = fileSetID->FileSetID;
    optionalElements[0].e.d.string = fileSetID->DescriptorFileID;
    optionalElements[1].e.d.string = fileSetID->SpecificCharacterSet;

    cond = DCM_ParseObject(obj, &e, 1,
		    optionalElements, (int) DIM_OF(optionalElements), NULL);
    if (cond != DCM_NORMAL)
	return DDR_ERROR;

    return DDR_NORMAL;
}

/* DDR_GetPatientList
**
** Purpose:
**	DDR_GetPatientList retrieves the list of patients from a DICOMDIR object
**	passed in by the caller.  For each patient in the DICOMDIR input,
**	DDR_GetFileSetID creates a new DDR_PATIENT record and adds the record
**	to the list passed by the caller.
**
** Parameter Dictionary:
**	obj		A DICOM Object that represents a DICOMDIR object.
**	lst		The list head used to hold the list of patient records
**			returned by this function.
**
** Return Values:
**	DDR_NORMAL
**	DDR_ERROR
**
** Notes:
**	The caller owns the lst argument and must have created the list before
**	calling this function.  The caller is responsible for freeing the
**	structures allocated and placed on the list by this function.  We
**	assume the caller passes this function an empty list, but that is
**	not a requirement.
*/


CONDITION
DDR_GetPatientList(DCM_OBJECT ** obj, LST_HEAD ** lst)
{
    U32 offset = 0;
    DCM_ELEMENT e = {DCM_DIRFIRSTOFFSET, DCM_UL, "", 1, sizeof(offset), NULL};
    DCM_OBJECT *patObject;
    CONDITION cond;

    e.d.ul = &offset;


    /*
     * Find the first offset in the DICOMDIR object.  That should point to a
     * patient
     */

    cond = DCM_ParseObject(obj, &e, 1, NULL, 0, NULL);
    if (cond != DCM_NORMAL)
	return DDR_ERROR;


    /*
     * Walk through the chain of patient records.  Each time we find one,
     * call another * function to parse that patient record and add it to the
     * caller's list.
     */

    while (offset != 0) {
	cond = DCM_GetSequenceByOffset(obj, DCM_DIRRECORDSEQUENCE, offset,
				       &patObject);
	if (cond != DCM_NORMAL) {
	    char errTxt[128];
	    sprintf(errTxt, "DDR_GetPatientList could not find patient with offset: %d", offset);
	    return COND_PushCondition(DDR_ERROR, errTxt);
	}
	addPatientRecord(&patObject, lst);
	offset = offsetNextRecord(&patObject);
    }
    return DDR_NORMAL;
}

/* DDR_GetStudyList
**
** Purpose:
**	DDR_GetStudyList retrieves the list of studies for a specific patient
**	from a DICOMDIR object passed in by the caller.  The function scans
**	the DICOMDIR object for patient records which match the patientID
**	string passed by the caller.  When a matching patient record is found,
**	each study record under that patient is parsed and a corresponding
**	DDR_STUDY structure added to the study list passed by the caller.
**
** Parameter Dictionary:
**	obj		A DICOM Object that represents a DICOMDIR object.
**	patientID	Character string which identifies the specific patient
**			for which to scan study records.
**	lst		The list head used to hold the list of study records
**			returned by this function.
**
** Return Values:
**	DDR_NORMAL
**	DDR_ERROR
**
** Notes:
**	The caller owns the lst argument and must have created the list before
**	calling this function.  The caller is responsible for freeing the
**	structures allocated and placed on the list by this function.
**	We assume the caller passes this function an empty list, but that is
**	not a requirement.
*/


CONDITION
DDR_GetStudyList(DCM_OBJECT ** object, const char *patientID,
		 LST_HEAD ** lst)
{
    LST_HEAD *patientList;
    DDR_PATIENT *pPtr;
    CONDITION cond;
    U32 offset;
    DCM_OBJECT *studyObject;

    /*
     * Create a patient list and get all of the patients in the DICOMDIR
     * object
     */

    patientList = LST_Create();
    cond = DDR_GetPatientList(object, &patientList);
    pPtr = LST_Dequeue(&patientList);

    /*
     * Scan through the patient list for a patient whose patient ID matches
     * that passed * by the caller.  When we find that, walk through the
     * study list for that patient * and fill the caller's list with
     * DDR_STUDY records.
     */

    while (pPtr != NULL) {
	if (strcmp(pPtr->PatientID, patientID) == 0) {
	    offset = pPtr->StudyLinkOffset;
	    while (offset != 0) {
		cond = DCM_GetSequenceByOffset(object, DCM_DIRRECORDSEQUENCE,
					       offset, &studyObject);
		if (cond != DCM_NORMAL) {
		    exit(1);	/* repair */
		}
		addStudyRecord(&studyObject, lst);
		offset = offsetNextRecord(&studyObject);
	    }
	}
	free(pPtr);
	pPtr = LST_Dequeue(&patientList);
    }

    LST_Destroy(&patientList);
    return DDR_NORMAL;
}


/* DDR_GetSeriesList
**
** Purpose:
**	DDR_GetSeriesList retrieves the list of series for a specific patient
**	and study from a DICOMDIR object passed in by the caller.  The function
**	scans the input DICOMDIR object to find the specific patient and study
**	requested by the caller.  When the desired study is located, the
**	function scans through the series records for that study.
**	A DDR_SERIES record is allocated for each series and placed in the
**	caller's list.
**
** Parameter Dictionary:
**	obj		A DICOM Object that represents a DICOMDIR object.
**	patientID	Character string which identifies the specific patient
**			for which to scan study records.
**	studyInstanceUID Study Instance UID which identifies the specific
**			study to be scanned for series.
**	lst		The list head used to hold the list of series records
**			returned by this function.
**
** Return Values:
**	DDR_NORMAL
**	DDR_ERROR
**
** Notes:
**	The caller owns the lst argument and must have created the list before
**	calling this function.  The caller is responsible for freeing the
**	structures allocated and placed on the list by this function.
**	We assume the caller passes this function an empty list, but that is
**	not a requirement.
*/

CONDITION
DDR_GetSeriesList(DCM_OBJECT ** object, const char *patientID,
		  const char *studyInstanceUID, LST_HEAD ** seriesList)
{
    LST_HEAD *studyList;
    DDR_STUDY *sPtr;
    CONDITION cond;
    U32 offset;
    DCM_OBJECT *seriesObject;

    studyList = LST_Create();
    cond = DDR_GetStudyList(object, patientID, &studyList);
    if (cond != DDR_NORMAL)
	return cond;

    sPtr = LST_Dequeue(&studyList);
    while (sPtr != NULL) {
	if (strcmp(sPtr->StudyInstanceUID, studyInstanceUID) == 0) {
	    offset = sPtr->SeriesLinkOffset;
	    while (offset != 0) {
		cond = DCM_GetSequenceByOffset(object, DCM_DIRRECORDSEQUENCE,
					       offset, &seriesObject);
		if (cond != DCM_NORMAL) {
		    return DDR_ERROR;
		}
		addSeriesRecord(&seriesObject, seriesList);
		offset = offsetNextRecord(&seriesObject);
	    }
	}
	free(sPtr);
	sPtr = LST_Dequeue(&studyList);
    }
    LST_Destroy(&studyList);
    return DDR_NORMAL;
}

/* DDR_GetSeriesLeafList
**
** Purpose:
**	DDR_GetSeriesLeafList retrieves the list of leaf for a specific
**	patient/study/series in the DICOMDIR object passed in by the caller.
**	The function scans the input DICOMDIR object to find the specific
**	patient/study/series requested by the caller.  When the desired series
**	is located, the function scans through the leaf node records for that
**	series.  **	A DDR_SERIES_LEAF record is allocated for each leaf
**	and placed in the caller's list.
**
** Parameter Dictionary:
**	obj		A DICOM Object that represents a DICOMDIR object.
**	patientID	Character string which identifies the specific patient
**			for which to scan study records.
**	studyInstanceUID Study Instance UID which identifies the specific
**			study to be scanned for series.
**	seriesInstanceUID Series Instance UID which identifies the specific
**			series to be scanned for leaf nodes.
**	lst		The list head used to hold the list of leaf records
**			returned by this function.
**
** Return Values:
**	DDR_NORMAL
**	DDR_ERROR
**
** Notes:
**	The caller owns the lst argument and must have created the list before
**	calling this function.  The caller is responsible for freeing the
**	structures allocated **	and placed on the list by this function.
**	We assume the caller passes this function an empty list, but that is
**	not a requirement.
*/


CONDITION
DDR_GetSeriesLeafList(DCM_OBJECT ** object, const char *patientID,
		      const char *studyInstanceUID,
		      const char *seriesInstanceUID,
		      LST_HEAD ** leafList)
{
    LST_HEAD *seriesList;
    DDR_SERIES *sPtr;
    CONDITION cond;
    U32 offset;
    DCM_OBJECT *leafObject;

    seriesList = LST_Create();
    cond = DDR_GetSeriesList(object, patientID, studyInstanceUID, &seriesList);
    if (cond != DDR_NORMAL)
	return cond;

    sPtr = LST_Dequeue(&seriesList);
    while (sPtr != NULL) {
	if (strcmp(sPtr->SeriesInstanceUID, seriesInstanceUID) == 0) {
	    offset = sPtr->LeafLinkOffset;
	    while (offset != 0) {
		cond = DCM_GetSequenceByOffset(object, DCM_DIRRECORDSEQUENCE,
					       offset, &leafObject);
		if (cond != DCM_NORMAL) {
		    return DDR_ERROR;
		}
		addLeafRecord(&leafObject, leafList);
		offset = offsetNextRecord(&leafObject);
	    }
	}
	free(sPtr);
	sPtr = LST_Dequeue(&seriesList);
    }
    LST_Destroy(&seriesList);
    return DDR_NORMAL;
}


#if 0
CONDITION
DDR_RootDirectory(DCM_OBJECT ** obj)
{
    U32 offset = 0;
    DCM_ELEMENT e = {DCM_DIRFIRSTOFFSET, DCM_UL, "", 1, sizeof(offset), NULL};

    CONDITION cond;

#if 0
    e.d.ul = &offset;

    cond = DCM_ParseObject(obj, &e, 1, NULL, 0, NULL);
    if (cond != DCM_NORMAL)
	return DDR_ERROR;

    cond = DCM_GetSequenceByOffset(obj, DCM_DIRRECORDSEQUENCE, offset);
#endif

    return DDR_NORMAL;
}
#endif
