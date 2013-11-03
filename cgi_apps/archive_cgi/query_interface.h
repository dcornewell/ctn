/*
+-+-+-+-+-+-+-+-+-
*/
/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:
** Intent:
** Last Update:		$Author: smm $, $Date: 1999-11-03 17:08:05 $
** Source File:		$Source: /home/smm/ctn/ctn/cgi_apps/archive_cgi/query_interface.h,v $
** Revision:		$Revision: 1.1 $
** Status:		$State: Exp $
*/

#ifndef _QUERY_INTERFACE_IS_IN
#define _QUERY_INTERFACE_IS_IN 1

typedef struct {
    char NodeName[65];
    char PortNumber[65];
    char CalledAPTitle[65];
    char CallingAPTitle[65];
}   CONNECTION_MODULE;

#define	PATIENT_BIRTHDATE 0x01
#define	PATIENT_NUMBERRELATEDSTUDIES	0x02
#define	PATIENT_NUMBERRELATEDSERIES	0x04
#define	PATIENT_NUMBERRELATEDIMAGES	0x08

typedef struct {
    void *reserved[2];
    long OptionalFlags;
    char PatientID[65];
    char LastName[65];
    char BirthDate[65];
    char PatientName[65];
    char NumberRelatedStudies[DICOM_IS_LENGTH + 1];
    char NumberRelatedSeries[DICOM_IS_LENGTH + 1];
    char NumberRelatedImages[DICOM_IS_LENGTH + 1];
}   PATIENT_QUERY_MODULE;

#define STUDY_STUDYDESCRIPTION		0x01
typedef struct {
    void *reserved[2];
    long OptionalFlags;
    char AccessionNumber[65];
    char StudyID[17];
    char StudyDate[65];
    char StudyInstanceUID[65];
    char StudyDescription[65];
    char SeriesModalities[10][10];
}   STUDY_QUERY_MODULE;

typedef struct {
    void *reserved[2];
    char SeriesInstanceUID[65];
    char Modality[17];
}   SERIES_QUERY_MODULE;

CONDITION
patientQuery(CONNECTION_MODULE * connection,
	     PATIENT_QUERY_MODULE * query, LST_HEAD ** patientList);
CONDITION
studyQuery(CONNECTION_MODULE * connection,
	   PATIENT_QUERY_MODULE * query, LST_HEAD ** studyList);
CONDITION
moveRequest(CONNECTION_MODULE * connection,
	    char *destinationAPTitle, PATIENT_QUERY_MODULE * patient,
	    STUDY_QUERY_MODULE * study);
#endif
