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
** Author, Date:	Stephen M. Moore, 25-Oct-1997
** Intent:		This file contains definitions and function prototypes
**			for the DDR facility which allows the user to
**			manipulate DICOM Directory Objects.
** Last Update:		$Author: smm $, $Date: 1998-04-17 14:20:40 $
** Source File:		$RCSfile: dicom_ddr.h,v $
** Revision:		$Revision: 1.2 $
** Status:		$State: Exp $
*/

#ifndef DCM_DDR_IS_IN
#define DCM_DDR_IS_IN 1

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct _DDR_FILESETID {
  unsigned long optionFlag;
  char FileSetID[DICOM_CS_LENGTH+1];
  char DescriptorFileID[DICOM_CS_LENGTH+1];
  char SpecificCharacterSet[DICOM_CS_LENGTH+1];
} DDR_FILESETID;

typedef struct _DDR_PATIENT {
  void *reserved[2];
  long optionFlag;
  U32 StudyLinkOffset;
  char PatientName[DICOM_PN_LENGTH+1];
  char PatientID[DICOM_LO_LENGTH+1];
  char BirthDate[DICOM_DA_LENGTH+1];
  char Sex[DICOM_CS_LENGTH+1];
  } DDR_PATIENT;

typedef struct _DDR_STUDY {
  void *reserved[2];
  long optionFlag;
  U32 SeriesLinkOffset;
  char StudyDate[DICOM_DA_LENGTH+1];
  char StudyTime[DICOM_TM_LENGTH+1];
  char StudyDescription[DICOM_LO_LENGTH+1];
  char StudyInstanceUID[DICOM_UI_LENGTH+1];
  char StudyID[DICOM_SH_LENGTH+1];
  char AccessionNumber[DICOM_SH_LENGTH+1];
  } DDR_STUDY;

typedef struct _DDR_SERIES {
  void *reserved[2];
  long optionFlag;
  U32 LeafLinkOffset;
  char Modality[DICOM_CS_LENGTH+1];
  char SeriesInstanceUID[DICOM_UI_LENGTH+1];
  char SeriesNumber[DICOM_IS_LENGTH+1];
} DDR_SERIES;

typedef struct _DDR_IMAGE {
  char ImageNumber[DICOM_IS_LENGTH+1];
} DDR_IMAGE;

typedef struct _DDR_SERIES_LEAF {
  void *reserved[2];
  long optionFlag;
  char RecordType[DICOM_CS_LENGTH+1];
  char FileID[DICOM_CS_LENGTH*8 + 1];
  char SOPClassUID[DICOM_UI_LENGTH+1];
  char SOPInstanceUID[DICOM_UI_LENGTH+1];
  char TransferSyntaxUID[DICOM_UI_LENGTH+1];
  union {
    DDR_IMAGE image;
  } specific;
} DDR_SERIES_LEAF;


CONDITION DDR_GetFileSetID(DCM_OBJECT** obj, DDR_FILESETID* fileSetID);
CONDITION DDR_GetPatientList(DCM_OBJECT**obj, LST_HEAD** lst);
CONDITION DDR_GetStudyList(DCM_OBJECT **object, const char* patientID,
			   LST_HEAD** lst);
CONDITION DDR_GetSeriesList(DCM_OBJECT **object, const char* patientID,
			    const char* studyInstanceUID, LST_HEAD **seriesList);
CONDITION DDR_GetSeriesLeafList(DCM_OBJECT **object, const char *patientID,
			const char* studyInstanceUID,
			const char* seriesInstanceUID,
			LST_HEAD **leafList);

#define DDR_NORMAL	FORM_COND(FAC_DDR, SEV_SUCC, 1)
#define DDR_ERROR	FORM_COND(FAC_DDR, SEV_ERROR, 2)

#ifdef  __cplusplus
}
#endif

#endif
