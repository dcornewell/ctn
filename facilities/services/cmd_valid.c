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
**	private modules
** Author, Date:	Stephen M. Moore, 15-Apr-93
** Intent:		This module contains general routines which are used
**			in our implementation of service classes.  These
**			routines allow users to request and accept service
**			classes, build and manipulate the public DUL
**			structures, send and receive messages, and request
**			unique Message IDs.
** Last Update:		$Author: smm $, $Date: 2008-03-19 20:41:26 $
** Source File:		$RCSfile: cmd_valid.c,v $
** Revision:		$Revision: 1.32 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.32 $ $RCSfile: cmd_valid.c,v $";

#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#ifdef _MSC_VER
#else
#include <sys/file.h>
#endif
#ifdef SOLARIS
#include <sys/fcntl.h>
#endif
#endif

#include "dicom.h"
#include "dicom_uids.h"
#include "condition.h"
#include "lst.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "private.h"

#define FRAGMENTMAX 65536

typedef unsigned long SRVPERMITTED;

typedef struct {
    char classUID[DICOM_UI_LENGTH + 1];
    SRVPERMITTED *permittedSrvList;	/* list of permitted services */
    unsigned short permittedSrvListSize;
}   SOPCLASSPERMITTEDSRV;	/* defines the various services permitted for
				 * a given SOP class */

typedef struct {
    void *reserved[2];
    DUL_ASSOCIATIONKEY **association;
    DUL_PRESENTATIONCONTEXTID ctxID;
    unsigned short command;
    MSG_TYPE messageType;
    void *message;
}   COMMAND_ENTRY;


CTNBOOLEAN PRVSRV_debug = 0;
static DUL_PRESENTATIONCONTEXT
*
findPresentationCtx(DUL_ASSOCIATESERVICEPARAMETERS * params,
		    DUL_PRESENTATIONCONTEXTID ctxid);

/* verification SOP classes */
static SRVPERMITTED verifySOPClass[] = {
    DCM_ECHO_REQUEST,
    DCM_ECHO_RESPONSE
};
/* Storage SOP classes */
static SRVPERMITTED storageSOPClass[] = {
    DCM_STORE_REQUEST,
    DCM_STORE_RESPONSE
};

/* Query/Retrieve SOP classes */
static SRVPERMITTED patientQueryRetrieveFindSOPClass[] = {
    DCM_FIND_REQUEST,
    DCM_FIND_RESPONSE,
    DCM_CANCEL_REQUEST
};
static SRVPERMITTED patientQueryRetrieveMoveSOPClass[] = {
    DCM_MOVE_REQUEST,
    DCM_MOVE_RESPONSE,
    DCM_CANCEL_REQUEST
};
static SRVPERMITTED patientQueryRetrieveGetSOPClass[] = {
    DCM_GET_REQUEST,
    DCM_GET_RESPONSE,
    DCM_CANCEL_REQUEST
};
static SRVPERMITTED studyQueryRetrieveFindSOPClass[] = {
    DCM_FIND_REQUEST,
    DCM_FIND_RESPONSE,
    DCM_CANCEL_REQUEST
};
static SRVPERMITTED studyQueryRetrieveMoveSOPClass[] = {
    DCM_MOVE_REQUEST,
    DCM_MOVE_RESPONSE,
    DCM_CANCEL_REQUEST
};
static SRVPERMITTED studyQueryRetrieveGetSOPClass[] = {
    DCM_GET_REQUEST,
    DCM_GET_RESPONSE,
    DCM_CANCEL_REQUEST
};
static SRVPERMITTED patientStudyQueryRetrieveFindSOPClass[] = {
    DCM_FIND_REQUEST,
    DCM_FIND_RESPONSE,
    DCM_CANCEL_REQUEST
};
static SRVPERMITTED patientStudyQueryRetrieveMoveSOPClass[] = {
    DCM_MOVE_REQUEST,
    DCM_MOVE_RESPONSE,
    DCM_CANCEL_REQUEST
};
static SRVPERMITTED patientStudyQueryRetrieveGetSOPClass[] = {
    DCM_GET_REQUEST,
    DCM_GET_RESPONSE,
    DCM_CANCEL_REQUEST
};
/* Notification Service classes */
static SRVPERMITTED basicStudyContentNotificationSOPClass[] = {
    DCM_STORE_REQUEST,
    DCM_STORE_RESPONSE
};
/* Patient Management Service SOP classes */
static SRVPERMITTED detachedPatientManagementSOPClass[] = {
    DCM_N_EVENT_REPORT_REQUEST,
    DCM_N_EVENT_REPORT_RESPONSE,
    DCM_N_GET_REQUEST,
    DCM_N_GET_RESPONSE
};
static SRVPERMITTED detachedVisitManagementSOPClass[] = {
    DCM_N_EVENT_REPORT_REQUEST,
    DCM_N_EVENT_REPORT_RESPONSE,
    DCM_N_GET_REQUEST,
    DCM_N_GET_RESPONSE,
    DCM_N_SET_REQUEST,
    DCM_N_SET_RESPONSE
};
static SRVPERMITTED detachedPatientManagementMetaSOPClass[] = {
    DCM_N_EVENT_REPORT_REQUEST,
    DCM_N_EVENT_REPORT_RESPONSE,
    DCM_N_GET_REQUEST,
    DCM_N_GET_RESPONSE,
    DCM_N_SET_REQUEST,
    DCM_N_SET_RESPONSE
};
/* Study Management Service SOP classes */
static SRVPERMITTED detachedStudyManagementSOPClass[] = {
    DCM_N_EVENT_REPORT_REQUEST,
    DCM_N_EVENT_REPORT_RESPONSE,
    DCM_N_GET_REQUEST,
    DCM_N_GET_RESPONSE,
    DCM_N_SET_REQUEST,
    DCM_N_SET_RESPONSE
};
static SRVPERMITTED studyComponentManagementSOPClass[] = {
    DCM_N_CREATE_REQUEST,
    DCM_N_CREATE_RESPONSE,
    DCM_N_GET_REQUEST,
    DCM_N_GET_RESPONSE,
    DCM_N_SET_REQUEST,
    DCM_N_SET_RESPONSE
};
static SRVPERMITTED studyManagementMetaSOPClass[] = {
    DCM_N_EVENT_REPORT_REQUEST,
    DCM_N_EVENT_REPORT_RESPONSE,
    DCM_N_CREATE_REQUEST,
    DCM_N_CREATE_RESPONSE,
    DCM_N_GET_REQUEST,
    DCM_N_GET_RESPONSE,
    DCM_N_SET_REQUEST,
    DCM_N_SET_RESPONSE
};
/* Results Management Service SOP classes */
static SRVPERMITTED detachedResultsManagementSOPClass[] = {
    DCM_N_EVENT_REPORT_REQUEST,
    DCM_N_EVENT_REPORT_RESPONSE,
    DCM_N_GET_REQUEST,
    DCM_N_GET_RESPONSE
};
static SRVPERMITTED detachedInterpretationManagementSOPClass[] = {
    DCM_N_EVENT_REPORT_REQUEST,
    DCM_N_EVENT_REPORT_RESPONSE,
    DCM_N_GET_REQUEST,
    DCM_N_GET_RESPONSE,
    DCM_N_SET_REQUEST,
    DCM_N_SET_RESPONSE
};
static SRVPERMITTED detachedResultsManagementMetaSOPClass[] = {
    DCM_N_EVENT_REPORT_REQUEST,
    DCM_N_EVENT_REPORT_RESPONSE,
    DCM_N_GET_REQUEST,
    DCM_N_GET_RESPONSE,
    DCM_N_SET_REQUEST,
    DCM_N_SET_RESPONSE
};
/* Print Management Service SOP Classes */
static SRVPERMITTED basicGreyscalePrintManagementMetaSOPClass[] = {
    DCM_N_CREATE_REQUEST,
    DCM_N_CREATE_RESPONSE,
    DCM_N_GET_REQUEST,
    DCM_N_GET_RESPONSE,
    DCM_N_SET_REQUEST,
    DCM_N_SET_RESPONSE,
    DCM_N_EVENT_REPORT_REQUEST,
    DCM_N_EVENT_REPORT_RESPONSE,
    DCM_N_ACTION_REQUEST,
    DCM_N_ACTION_RESPONSE,
    DCM_N_DELETE_REQUEST,
    DCM_N_DELETE_RESPONSE
};
static SRVPERMITTED basicColorPrintManagementMetaSOPClass[] = {
    DCM_N_CREATE_REQUEST,
    DCM_N_CREATE_RESPONSE,
    DCM_N_GET_REQUEST,
    DCM_N_GET_RESPONSE,
    DCM_N_SET_REQUEST,
    DCM_N_SET_RESPONSE,
    DCM_N_EVENT_REPORT_REQUEST,
    DCM_N_EVENT_REPORT_RESPONSE,
    DCM_N_ACTION_REQUEST,
    DCM_N_ACTION_RESPONSE,
    DCM_N_DELETE_REQUEST,
    DCM_N_DELETE_RESPONSE
};
static SRVPERMITTED referencedGreyscalePrintManagementMetaSOPClass[] = {
    DCM_N_CREATE_REQUEST,
    DCM_N_CREATE_RESPONSE,
    DCM_N_GET_REQUEST,
    DCM_N_GET_RESPONSE,
    DCM_N_SET_REQUEST,
    DCM_N_SET_RESPONSE,
    DCM_N_EVENT_REPORT_REQUEST,
    DCM_N_EVENT_REPORT_RESPONSE,
    DCM_N_ACTION_REQUEST,
    DCM_N_ACTION_RESPONSE,
    DCM_N_DELETE_REQUEST,
    DCM_N_DELETE_RESPONSE
};
static SRVPERMITTED referencedColorPrintManagementMetaSOPClass[] = {
    DCM_N_CREATE_REQUEST,
    DCM_N_CREATE_RESPONSE,
    DCM_N_GET_REQUEST,
    DCM_N_GET_RESPONSE,
    DCM_N_SET_REQUEST,
    DCM_N_SET_RESPONSE,
    DCM_N_EVENT_REPORT_REQUEST,
    DCM_N_EVENT_REPORT_RESPONSE,
    DCM_N_ACTION_REQUEST,
    DCM_N_ACTION_RESPONSE,
    DCM_N_DELETE_REQUEST,
    DCM_N_DELETE_RESPONSE
};
static SRVPERMITTED basicFilmSessionSOPClass[] = {
    DCM_N_CREATE_REQUEST,
    DCM_N_CREATE_RESPONSE,
    DCM_N_SET_REQUEST,
    DCM_N_SET_RESPONSE,
    DCM_N_ACTION_REQUEST,
    DCM_N_ACTION_RESPONSE,
    DCM_N_DELETE_REQUEST,
    DCM_N_DELETE_RESPONSE
};
static SRVPERMITTED basicFilmBoxSOPClass[] = {
    DCM_N_CREATE_REQUEST,
    DCM_N_CREATE_RESPONSE,
    DCM_N_SET_REQUEST,
    DCM_N_SET_RESPONSE,
    DCM_N_ACTION_REQUEST,
    DCM_N_ACTION_RESPONSE,
    DCM_N_DELETE_REQUEST,
    DCM_N_DELETE_RESPONSE
};
static SRVPERMITTED basicGreyscaleImageBoxSOPClass[] = {
    DCM_N_SET_REQUEST,
    DCM_N_SET_RESPONSE
};
static SRVPERMITTED basicColorImageBoxSOPClass[] = {
    DCM_N_SET_REQUEST,
    DCM_N_SET_RESPONSE
};
static SRVPERMITTED referencedImageBoxSOPClass[] = {
    DCM_N_SET_REQUEST,
    DCM_N_SET_RESPONSE
};
static SRVPERMITTED basicAnnotationBoxSOPClass[] = {
    DCM_N_SET_REQUEST,
    DCM_N_SET_RESPONSE
};
static SRVPERMITTED printJobSOPClass[] = {
    DCM_N_EVENT_REPORT_REQUEST,
    DCM_N_EVENT_REPORT_RESPONSE,
    DCM_N_GET_REQUEST,
    DCM_N_GET_RESPONSE
};
static SRVPERMITTED printerSOPClass[] = {
    DCM_N_EVENT_REPORT_REQUEST,
    DCM_N_EVENT_REPORT_RESPONSE,
    DCM_N_GET_REQUEST,
    DCM_N_GET_RESPONSE
};
static SRVPERMITTED basicVOILUTSOPClass[] = {
    DCM_N_CREATE_REQUEST,
    DCM_N_CREATE_RESPONSE,
    DCM_N_SET_REQUEST,
    DCM_N_SET_RESPONSE,
    DCM_N_DELETE_REQUEST,
    DCM_N_DELETE_RESPONSE
};
static SRVPERMITTED imageOverlayBoxSOPClass[] = {
    DCM_N_CREATE_REQUEST,
    DCM_N_CREATE_RESPONSE,
    DCM_N_SET_REQUEST,
    DCM_N_SET_RESPONSE,
    DCM_N_DELETE_REQUEST,
    DCM_N_DELETE_RESPONSE
};

static SRVPERMITTED storageCommitmentPushModelClass[] = {
    DCM_N_EVENT_REPORT_REQUEST,
    DCM_N_EVENT_REPORT_RESPONSE,
    DCM_N_ACTION_REQUEST,
    DCM_N_ACTION_RESPONSE
};

static SRVPERMITTED generalCFindSOPClass[] = {
    DCM_FIND_REQUEST,
    DCM_FIND_RESPONSE,
    DCM_CANCEL_REQUEST
};

static SRVPERMITTED mppsSOPClass[] = {
    DCM_N_CREATE_REQUEST,
    DCM_N_CREATE_RESPONSE,
    DCM_N_SET_REQUEST,
    DCM_N_SET_RESPONSE
};

static SRVPERMITTED ianSOPClass[] = {
    DCM_N_CREATE_REQUEST,
    DCM_N_CREATE_RESPONSE
};

static SRVPERMITTED gpspsSOPClass[] = {
    DCM_N_ACTION_REQUEST,
    DCM_N_ACTION_RESPONSE
};

static SRVPERMITTED gpppsSOPClass[] = {
    DCM_N_CREATE_REQUEST,
    DCM_N_CREATE_RESPONSE,
    DCM_N_SET_REQUEST,
    DCM_N_SET_RESPONSE
};

/* Initialization of the permitted services table for each defined
   SOP class
*/
static SOPCLASSPERMITTEDSRV classPermittedSrvTable[] = {
    /* verification service */
    {DICOM_SOPCLASSVERIFICATION, verifySOPClass, DIM_OF(verifySOPClass)},
    /* storage service */



    { DICOM_SOPCLASSCOMPUTEDRADIOGRAPHY,	storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.1" */
    { "1.2.840.10008.5.1.4.1.1.1.1",		storageSOPClass, 2 },	/* Digital X-Ray Image Storage - For Presentation */
    { "1.2.840.10008.5.1.4.1.1.1.1.1",		storageSOPClass, 2 },	/* Digital X-Ray Image Storage - For Processing */
    { "1.2.840.10008.5.1.4.1.1.1.2",		storageSOPClass, 2 },	/* Digital Mammography Im Storage - For Presentation */
    { "1.2.840.10008.5.1.4.1.1.1.2.1",		storageSOPClass, 2 },	/* Digital Mammography Im Storage - For Processing */
    { "1.2.840.10008.5.1.4.1.1.1.3",		storageSOPClass, 2 },	/* Digital Intra-oral X-Ray Im Storage - Presentation */
	{ "1.2.840.10008.5.1.4.1.1.1.3.1",		storageSOPClass, 2 },	/* Digital Intra-oral X-Ray Im Storage - Processing */
    { DICOM_SOPCLASSCT,						storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.2" */
    { DICOM_SOPCLASSENHANCEDCT,				storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.2.1" */
	{ "1.2.840.10008.5.1.4.1.1.3.1",		storageSOPClass, 2 },	/* Ultrasound Multii-frame Im Storage */
    { DICOM_SOPCLASSMR,						storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.4" */
    { DICOM_SOPCLASSENHANCEDMR,				storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.4.1" */	
    { "1.2.840.10008.5.1.4.1.1.4.2",		storageSOPClass, 2 },	/* MR Spectroscopy data */
    { "1.2.840.10008.5.1.4.1.1.4.3",		storageSOPClass, 2 },	/* Enhanced MR Color Im Storage */
    { DICOM_SOPCLASSUS,						storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.6.1" */	
	{ "1.2.840.10008.5.1.4.1.1.6.2",		storageSOPClass, 2 },	/* Enhanced US Volume Storage */
    { DICOM_SOPCLASSSECONDARYCAPTURE,		storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.7" */
	{ "1.2.840.10008.5.1.4.1.1.7.1",		storageSOPClass, 2 },	/* Multi-frame Single Bit Secondary Capture Im Storage */
	{ "1.2.840.10008.5.1.4.1.1.7.2",		storageSOPClass, 2 },	/* Multi-frame Grayscale Byte Secondary Capture Im Storage */
	{ "1.2.840.10008.5.1.4.1.1.7.3",		storageSOPClass, 2 },	/* Multi-frame Grayscalw Word Secondary Capture Im Storage */
	{ "1.2.840.10008.5.1.4.1.1.7.4",		storageSOPClass, 2 },	/* Multi-frame True Color Secondary Capture Im Storage */
	{ "1.2.840.10008.5.1.4.1.1.9.1.1",		storageSOPClass, 2 },	/* 12-lead ECG Waveform Storage */
	{ "1.2.840.10008.5.1.4.1.1.9.1.2",		storageSOPClass, 2 },	/* General ECG Waveform Storage */
	{ "1.2.840.10008.5.1.4.1.1.9.1.3",		storageSOPClass, 2 },	/* Ambulatory ECG Waveform Storage */
	{ "1.2.840.10008.5.1.4.1.1.9.2.1",		storageSOPClass, 2 },	/* Hemodynamic Waveform Storage */
	{ "1.2.840.10008.5.1.4.1.1.9.3.1",		storageSOPClass, 2 },	/* Cardiac Electrophysiology Waveform Storage */
	{ "1.2.840.10008.5.1.4.1.1.9.4.1",		storageSOPClass, 2 },	/* Basic Voice Audio Waveform Storage */
	{ "1.2.840.10008.5.1.4.1.1.9.4.2",		storageSOPClass, 2 },	/* General Audio Waveform Storage */
	{ "1.2.840.10008.5.1.4.1.1.9.5.1",		storageSOPClass, 2 },	/* Arterial Pulse Waveform Storage */
	{ "1.2.840.10008.5.1.4.1.1.9.6.1",		storageSOPClass, 2 },	/* Respiratory Waveform Storage */
	{ "1.2.840.10008.5.1.4.1.1.11.1",		storageSOPClass, 2 },	/* Grayscale Softcopy Presentation State Storage */
	{ "1.2.840.10008.5.1.4.1.1.11.2",		storageSOPClass, 2 },	/* Color Softcopy Presentation State Storage */
	{ "1.2.840.10008.5.1.4.1.1.11.3",		storageSOPClass, 2 },	/* Pseudo-Color Softcopy Presentation State Storage */
	{ "1.2.840.10008.5.1.4.1.1.11.4",		storageSOPClass, 2 },	/* Blending Softcopy Presentation State Storage */
	{ "1.2.840.10008.5.1.4.1.1.11.5",		storageSOPClass, 2 },	/* XA/XRF Grayscale Softcopy Presentation State Storage */
    { DICOM_SOPCLASSXRAYANGIO,				storageSOPClass, 2 },/* "1.2.840.10008.5.1.4.1.1.12.1" */
	{ "1.2.840.10008.5.1.4.1.1.12.1.1",		storageSOPClass, 2 },/* Enhanced XA Image Storage */
    { DICOM_SOPCLASSXRAYFLUORO,				storageSOPClass, 2 },/* "1.2.840.10008.5.1.4.1.1.12.2" */
	{ "1.2.840.10008.5.1.4.1.1.12.2.1",		storageSOPClass, 2 },/* Enhanced XRF Image Storage */
	{ "1.2.840.10008.5.1.4.1.1.13.1.1",		storageSOPClass, 2 },/* X-Ray 3D Angiographic Image Storage */
	{ "1.2.840.10008.5.1.4.1.1.13.1.2",		storageSOPClass, 2 },/* X-Ray 3D Craniofacial Image Storage */
	{ "1.2.840.10008.5.1.4.1.1.13.1.3",		storageSOPClass, 2 },/* Breast Tomosynthesis Image Storage */
    { DICOM_SOPCLASSNM,						storageSOPClass, 2 },/* "1.2.840.10008.5.1.4.1.1.20" */
    { "1.2.840.10008.5.1.4.1.1.66",			storageSOPClass, 2 },/* Raw data */
	{ "1.2.840.10008.5.1.4.1.1.66.1",		storageSOPClass, 2 },	/* Spatial Registration Storage */
	{ "1.2.840.10008.5.1.4.1.1.66.2",		storageSOPClass, 2 },	/* Spatial Fiducials Storage */
	{ "1.2.840.10008.5.1.4.1.1.66.3",		storageSOPClass, 2 },	/* Deformable Spatial Registration Storage */
	{ "1.2.840.10008.5.1.4.1.1.66.4",		storageSOPClass, 2 },	/* Segmentation Storage */
	{ "1.2.840.10008.5.1.4.1.1.66.5",		storageSOPClass, 2 },	/* Surface Segmentation Storage */
	{ "1.2.840.10008.5.1.4.1.1.67",			storageSOPClass, 2 },/* Real World Value Mapping Storage */
    { DICOM_SOPCLASSVLENDOSCOPIC,			storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.77.1.1" */
	{ "1.2.840.10008.5.1.4.1.1.77.1.1.1",	storageSOPClass, 2 },	/* Video Endoscopic Image Storage */
    { DICOM_SOPCLASSVLMICROSCOPIC,			storageSOPClass, 2 },/* "1.2.840.10008.5.1.4.1.1.77.1.2" */
	{ "1.2.840.10008.5.1.4.1.1.77.1.2.1",	storageSOPClass, 2 },	/* Video Microscopic Image Storage */
    { DICOM_SOPCLASSVLSLIDEMICROSCOPIC,		storageSOPClass, 2 },/* "1.2.840.10008.5.1.4.1.1.77.1.3" */
    { DICOM_SOPCLASSVLPHOTOGRAPHIC,			storageSOPClass, 2 },/* "1.2.840.10008.5.1.4.1.1.77.1.4" */
	{ "1.2.840.10008.5.1.4.1.1.77.1.4.1",	storageSOPClass, 2 },	/* Video Photographic Image Storage */
    { "1.2.840.10008.5.1.4.1.1.77.1.5.1",	storageSOPClass, 2 },	/* Opthalmic Photo 8 Bit Image */
    { "1.2.840.10008.5.1.4.1.1.77.1.5.2",	storageSOPClass, 2 },	/* Opthalmic Photo 16 Bit Image */
    { "1.2.840.10008.5.1.4.1.1.77.1.5.3",	storageSOPClass, 2 },	/* Stereometric Relationship Storage*/
	{ "1.2.840.10008.5.1.4.1.1.77.1.5.4",	storageSOPClass, 2 },	/* Ophthalmic Tomography Image Storage */
	{ "1.2.840.10008.5.1.4.1.1.78.1",		storageSOPClass, 2 },	/* Lensometry Measurements Storage */
	{ "1.2.840.10008.5.1.4.1.1.78.2",		storageSOPClass, 2 },	/* Autorefraction Measurements Storage */
	{ "1.2.840.10008.5.1.4.1.1.78.3",		storageSOPClass, 2 },	/* Keratometry Measurements Storage */
	{ "1.2.840.10008.5.1.4.1.1.78.4",		storageSOPClass, 2 },	/* Subjective Refraction Measurements Storage */
	{ "1.2.840.10008.5.1.4.1.1.78.5",		storageSOPClass, 2 },	/* Visual Acuity Measurements Storage */
	{ "1.2.840.10008.5.1.4.1.1.78.6",		storageSOPClass, 2 },	/* Spectacle Prescription  Report Storage */
	{ "1.2.840.10008.5.1.4.1.1.79.1",		storageSOPClass, 2 },	/* Macular Grid Thichness and Volume Report */
    { DICOM_SOPCLASSBASICTEXTSR,			storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.88.11" */
    { DICOM_SOPCLASSENHANCEDSR,				storageSOPClass, 2 },/* "1.2.840.10008.5.1.4.1.1.88.22" */
    { DICOM_SOPCLASSCOMPREHENSIVESR,		storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.88.33" */
    { "1.2.840.10008.5.1.4.1.1.88.40",		storageSOPClass, 2 },/* Procedure Log */
    { "1.2.840.10008.5.1.4.1.1.88.50",		storageSOPClass, 2 },/* Mammo CAD SR */
    { "1.2.840.10008.5.1.4.1.1.88.59",		storageSOPClass, 2 },/* Key Object Selection */
	{ "1.2.840.10008.5.1.4.1.1.88.65",		storageSOPClass, 2 },/* Chest CAD SR */
	{ "1.2.840.10008.5.1.4.1.1.88.67",		storageSOPClass, 2 },/* X-Ray Radiation Dose SR */
	{ "1.2.840.10008.5.1.4.1.1.88.69",		storageSOPClass, 2 },/* Colon CAD SR */
	{ DICOM_SOPCLASSENCAPSULATEDPDF,		storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.104.1" */
	{ "1.2.840.10008.5.1.4.1.1.104.2",		storageSOPClass, 2 },/* Encapsulated CDA Storage */
    { DICOM_SOPCLASSPET,					storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.128" */
	{ "1.2.840.10008.5.1.4.1.1.130",		storageSOPClass, 2 },	/* Enhanced PET Image Storage */
	{ "1.2.840.10008.5.1.4.1.1.131",		storageSOPClass, 2 },	/* Basic Structured Display Storage */
    { DICOM_SOPRTIMAGESTORAGE,				storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.481.1" */
    { DICOM_SOPRTDOSESTORAGE,				storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.481.2" */
    { DICOM_SOPRTSTRUCTURESETSTORAGE,		storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.481.3" */
    { DICOM_SOPRTBREAMS,					storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.481.4" */
    { DICOM_SOPRTPLANSTORAGE,				storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.481.5" */
    { DICOM_SOPRTBRACHYTREATMENT,			storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.481.6" */
    { DICOM_SOPRTTREATMENTSUMMARY,			storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.481.7" */
	{ "1.2.840.10008.5.1.4.1.1.481.8",		storageSOPClass, 2 },	/* RT Ion Plan Storage */
	{ "1.2.840.10008.5.1.4.1.1.481.9",		storageSOPClass, 2 },	/* RT Ion Beams Treatment Record */

    { DICOM_SOPCLASSHARDCOPYCOLORIMAGE,		storageSOPClass, 2 },	/* "1.2.840.10008.5.1.1.30" */
    { DICOM_SOPCLASSHARDCOPYGRAYSCALEIMAGE, storageSOPClass, 2 },	/* "1.2.840.10008.5.1.1.29" */
    { DICOM_SOPCLASSSTANDALONECURVE,		storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.9" */
    { DICOM_SOPCLASSSTANDALONEMODALITYLUT, 	storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.10" */
    { DICOM_SOPCLASSSTANDALONEOVERLAY,		storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.8" */
    { DICOM_SOPCLASSSTANDALONEVOILUT,		storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.11" */
    { DICOM_SOPCLASSSTANDALONEPETCURVE,		storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.129" */
    { DICOM_SOPCLASSSTOREDPRINT,			storageSOPClass, 2 },	/* "1.2.840.10008.5.1.1.27" */

    /* Some additional storage classes */
    { DICOM_SOPCLASSUSMULTIFRAMEIMAGE1993,	storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.3" */
    { DICOM_SOPCLASSNM1993,					storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.5" */
    { DICOM_SOPCLASSUS1993,					storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.6" */

    { DICOM_SOPCLASSXRAYANGIOBIPLANE_RET, 	storageSOPClass, 2 },	/* "1.2.840.10008.5.1.4.1.1.12.3" */
    { "1.3.12.2.1107.5.9.1",				storageSOPClass, 2 },	/* SMS Raw CT data */


#if 0
    { DICOM_SOPCLASSCOMPUTEDRADIOGRAPHY, storageSOPClass, 2 },
    { DICOM_SOPCLASSCT, storageSOPClass, 2 },
    { DICOM_SOPCLASSENHANCEDCT, storageSOPClass, 2 },
    { DICOM_SOPCLASSHARDCOPYCOLORIMAGE, storageSOPClass, 2 },
    { DICOM_SOPCLASSHARDCOPYGRAYSCALEIMAGE, storageSOPClass, 2 },
    { DICOM_SOPCLASSMR, storageSOPClass, 2 },
    { DICOM_SOPCLASSENHANCEDMR, storageSOPClass, 2 },
    { DICOM_SOPCLASSNM, storageSOPClass, 2 },
    { DICOM_SOPCLASSPET, storageSOPClass, 2 },
    { DICOM_SOPRTDOSESTORAGE, storageSOPClass, 2 },
    { DICOM_SOPRTIMAGESTORAGE, storageSOPClass, 2 },
    { DICOM_SOPRTPLANSTORAGE, storageSOPClass, 2 },
    { DICOM_SOPRTSTRUCTURESETSTORAGE, storageSOPClass, 2 },
    { DICOM_SOPRTBREAMS, storageSOPClass, 2 },
    { DICOM_SOPRTBRACHYTREATMENT, storageSOPClass, 2 },
    { DICOM_SOPRTTREATMENTSUMMARY, storageSOPClass, 2 },
    { DICOM_SOPCLASSSECONDARYCAPTURE, storageSOPClass, 2 },
    { DICOM_SOPCLASSSTANDALONECURVE, storageSOPClass, 2 },
    { DICOM_SOPCLASSSTANDALONEMODALITYLUT, storageSOPClass, 2 },
    { DICOM_SOPCLASSSTANDALONEOVERLAY, storageSOPClass, 2 },
    { DICOM_SOPCLASSSTANDALONEVOILUT, storageSOPClass, 2 },
    { DICOM_SOPCLASSSTANDALONEPETCURVE, storageSOPClass, 2 },
    { DICOM_SOPCLASSSTOREDPRINT, storageSOPClass, 2 },
    { DICOM_SOPCLASSUS, storageSOPClass, 2 },
    { DICOM_SOPCLASSUSMULTIFRAMEIMAGE, storageSOPClass, 2 },
    { DICOM_SOPCLASSXRAYANGIO, storageSOPClass, 2 },
    { DICOM_SOPCLASSXRAYFLUORO, storageSOPClass, 2 },
    { DICOM_SOPCLASSDIGXRAYPRESENTATION, storageSOPClass, 2 },
    { DICOM_SOPCLASSDIGXRAYPROCESSING, storageSOPClass, 2 },
    { DICOM_SOPCLASSMAMMOXRPRESENTATION, storageSOPClass, 2 },
    { DICOM_SOPCLASSMAMMOXRPROCESSING, storageSOPClass, 2 },
    { DICOM_SOPCLASSINTRAORALPRESENTATION, storageSOPClass, 2 },
    { DICOM_SOPCLASSINTRAORALPROCESSING, storageSOPClass, 2 },
    { DICOM_SOPCLASSVLENDOSCOPIC, storageSOPClass, 2 },
    { DICOM_SOPCLASSVLMICROSCOPIC, storageSOPClass, 2 },
    { DICOM_SOPCLASSVLSLIDEMICROSCOPIC, storageSOPClass, 2 },
    { DICOM_SOPCLASSVLPHOTOGRAPHIC, storageSOPClass, 2 },

    /* More storage SOP Classes */
    { DICOM_SOPCLASSUSMULTIFRAMEIMAGE1993, storageSOPClass, 2 },
    { DICOM_SOPCLASSNM1993, storageSOPClass, 2 },
    { DICOM_SOPCLASSUS1993, storageSOPClass, 2 },
    { DICOM_SOPCLASSWAVEFORMSTORAGE, storageSOPClass, 2 },
    { DICOM_SOPCLASSECGWAVEFORMSTORAGE, storageSOPClass, 2 },
    { DICOM_SOPCLASSXRAYANGIOBIPLANE_RET, storageSOPClass, 2 },
    { DICOM_SOPCLASSGREYSCALEPS, storageSOPClass, 2 },
    { DICOM_SOPCLASSCOLORPS, storageSOPClass, 2 },
    { DICOM_SOPCLASSPSEUDOCOLORPS, storageSOPClass, 2 },
    { DICOM_SOPCLASSBLENDINGPS, storageSOPClass, 2 },

    { "1.2.840.10008.5.1.4.1.1.88.11", storageSOPClass, 2 },	/* Basic Text SR */
    { "1.2.840.10008.5.1.4.1.1.88.22", storageSOPClass, 2 },	/* Enhanced SR Storage */
    { "1.2.840.10008.5.1.4.1.1.88.33", storageSOPClass, 2 },	/* Comprehensive SR */
    { "1.2.840.10008.5.1.4.1.1.88.40", storageSOPClass, 2 },	/* Procedure Log */
    { "1.2.840.10008.5.1.4.1.1.88.50", storageSOPClass, 2 },	/* Mammography CAD SR */
    { "1.2.840.10008.5.1.4.1.1.88.59", storageSOPClass, 2 },	/* Key Object Selection Document */
    { "1.2.840.10008.5.1.4.1.1.88.65", storageSOPClass, 2 },	/* Chest CAD SR */
    { "1.2.840.10008.5.1.4.1.1.88.67", storageSOPClass, 2 },	/* X-Ray Radiation Dose SR */
    { "1.2.840.10008.5.1.4.1.1.104.1", storageSOPClass, 2 },	/* Encapsulated PDF */
    { "1.2.840.10008.5.1.4.1.1.104.2", storageSOPClass, 2 },	/* Encapsuldated CDA */
    { "1.3.12.2.1107.5.9.1", storageSOPClass, 2 },	/* SMS Raw CT Data*/
    { "1.2.840.10008.5.1.4.1.1.66", storageSOPClass, 2 },	/* Raw Data*/
    { "1.2.840.10008.5.1.4.1.1.4.2", storageSOPClass, 2 },	/* Spectroscopy Data*/
    { "1.2.840.10008.5.1.4.1.1.66.1", storageSOPClass, 2 },	/* Spatial Registration*/
    { "1.2.840.10008.5.1.4.1.1.1.2", storageSOPClass, 2 },    /* "for presentation" mammo image */
    { "1.2.840.10008.5.1.4.1.1.1.2.1", storageSOPClass, 2 },   /* "for processing" mammo image */
    { "1.2.840.10008.5.1.4.1.1.77.1.5.1", storageSOPClass, 2 },   /* Opthalmic Phto 8 Bit Image Storage */
    { "1.2.840.10008.5.1.4.1.1.77.1.5.2", storageSOPClass, 2 },   /* Opthalmic Phto 16 Bit Image Storage */
    { "1.2.840.10008.5.1.4.1.1.77.1.5.3", storageSOPClass, 2 },   /* Stereometric Relationship Storage */
    { "1.2.840.10008.5.1.4.1.1.9.1.2", storageSOPClass, 2 },   /* ECG Data*/
    { "1.2.840.10008.5.1.4.1.1.9.2.1", storageSOPClass, 2 },   /* Hemodynamic Data*/
#endif



    /* Query/Retrieve Service */
    {DICOM_SOPPATIENTQUERY_FIND, patientQueryRetrieveFindSOPClass,
    DIM_OF(patientQueryRetrieveFindSOPClass)},
    {DICOM_SOPPATIENTQUERY_MOVE, patientQueryRetrieveMoveSOPClass,
    DIM_OF(patientQueryRetrieveMoveSOPClass)},
    {DICOM_SOPPATIENTQUERY_GET, patientQueryRetrieveGetSOPClass,
    DIM_OF(patientQueryRetrieveGetSOPClass)},
    {DICOM_SOPSTUDYQUERY_FIND, studyQueryRetrieveFindSOPClass,
    DIM_OF(studyQueryRetrieveFindSOPClass)},
    {DICOM_SOPSTUDYQUERY_MOVE, studyQueryRetrieveMoveSOPClass,
    DIM_OF(studyQueryRetrieveMoveSOPClass)},
    {DICOM_SOPSTUDYQUERY_GET, studyQueryRetrieveGetSOPClass,
    DIM_OF(studyQueryRetrieveGetSOPClass)},
    {DICOM_SOPPATIENTSTUDYQUERY_FIND, patientStudyQueryRetrieveFindSOPClass,
    DIM_OF(patientStudyQueryRetrieveFindSOPClass)},
    {DICOM_SOPPATIENTSTUDYQUERY_MOVE, patientStudyQueryRetrieveMoveSOPClass,
    DIM_OF(patientStudyQueryRetrieveMoveSOPClass)},
    {DICOM_SOPPATIENTSTUDYQUERY_GET, patientStudyQueryRetrieveGetSOPClass,
    DIM_OF(patientStudyQueryRetrieveGetSOPClass)},
    /* Notification Service */
    {DICOM_SOPCLASSBASICSTUDYCONTENTNOTIFICATION,
	basicStudyContentNotificationSOPClass,
    DIM_OF(basicStudyContentNotificationSOPClass)},
    /* Patient Management Service */
    {DICOM_SOPCLASSDETACHEDPATIENTMGMT, detachedPatientManagementSOPClass,
    DIM_OF(detachedPatientManagementSOPClass)},
    {DICOM_SOPCLASSDETACHEDVISITMGMT, detachedVisitManagementSOPClass,
    DIM_OF(detachedVisitManagementSOPClass)},
    {DICOM_SOPCLASSDETACHEDPATIENTMGMTMETA,
	detachedPatientManagementMetaSOPClass,
    DIM_OF(detachedPatientManagementMetaSOPClass)},
    /* Study Management Service */
    {DICOM_SOPCLASSDETACHEDSTUDYMGMT, detachedStudyManagementSOPClass,
    DIM_OF(detachedStudyManagementSOPClass)},
    {DICOM_SOPCLASSSTUDYCOMPONENTMGMT, studyComponentManagementSOPClass,
    DIM_OF(studyComponentManagementSOPClass)},
    {DICOM_SOPCLASSDETACHEDSTUDYMGMTMETA, studyManagementMetaSOPClass,
    DIM_OF(studyManagementMetaSOPClass)},
    /* Results Management Services */
    {DICOM_SOPCLASSDETACHEDRESULTSMGMT, detachedResultsManagementSOPClass,
    DIM_OF(detachedResultsManagementSOPClass)},
    {DICOM_SOPCLASSDETACHEDINTERPRETMGMT,
	detachedInterpretationManagementSOPClass,
    DIM_OF(detachedInterpretationManagementSOPClass)},
    {DICOM_SOPCLASSDETACHEDRESULTSMGMTMETA,
	detachedResultsManagementMetaSOPClass,
    DIM_OF(detachedResultsManagementMetaSOPClass)},
    /* Print Management services */
    {DICOM_SOPCLASSGREYSCALEPRINTMGMTMETA,
	basicGreyscalePrintManagementMetaSOPClass,
    DIM_OF(basicGreyscalePrintManagementMetaSOPClass)},
    {DICOM_SOPCLASSCOLORPRINTMGMTMETA,
	basicColorPrintManagementMetaSOPClass,
    DIM_OF(basicColorPrintManagementMetaSOPClass)},
    {DICOM_SOPCLASSREFGREYSCALEPRINTMGMTMETA,
	referencedGreyscalePrintManagementMetaSOPClass,
    DIM_OF(referencedGreyscalePrintManagementMetaSOPClass)},
    {DICOM_SOPCLASSREFCOLORPRINTMGMTMETA,
	referencedColorPrintManagementMetaSOPClass,
    DIM_OF(referencedColorPrintManagementMetaSOPClass)},
    {DICOM_SOPCLASSBASICFILMSESSION, basicFilmSessionSOPClass,
    DIM_OF(basicFilmSessionSOPClass)},
    {DICOM_SOPCLASSBASICFILMBOX, basicFilmBoxSOPClass,
    DIM_OF(basicFilmBoxSOPClass)},
    {DICOM_SOPCLASSBASICGREYSCALEIMAGEBOX, basicGreyscaleImageBoxSOPClass,
    DIM_OF(basicGreyscaleImageBoxSOPClass)},
    {DICOM_SOPCLASSBASICCOLORIMAGEBOX, basicColorImageBoxSOPClass,
    DIM_OF(basicColorImageBoxSOPClass)},
    {DICOM_SOPCLASSREFERENCEDIMAGEBOX, referencedImageBoxSOPClass,
    DIM_OF(referencedImageBoxSOPClass)},
    {DICOM_SOPCLASSBASICANNOTATIONBOX, basicAnnotationBoxSOPClass,
    DIM_OF(basicAnnotationBoxSOPClass)},
    {DICOM_SOPCLASSPRINTJOB, printJobSOPClass, DIM_OF(printJobSOPClass)},
    {DICOM_SOPCLASSPRINTER, printerSOPClass, DIM_OF(printerSOPClass)},
    {DICOM_SOPCLASSVOILUT, basicVOILUTSOPClass, DIM_OF(basicVOILUTSOPClass)},
    {DICOM_SOPCLASSIMAGEOVERLAYBOX, imageOverlayBoxSOPClass,
    DIM_OF(imageOverlayBoxSOPClass)},
    {DICOM_SOPCLASSSTORAGECOMMITMENTPUSHMODEL, storageCommitmentPushModelClass,
    DIM_OF(storageCommitmentPushModelClass)},
    {DICOM_SOPMODALITYWORKLIST_FIND, generalCFindSOPClass,
    DIM_OF(generalCFindSOPClass)},
    {DICOM_SOPCLASSMPPS, mppsSOPClass,
    DIM_OF(mppsSOPClass)},
    {DICOM_SOPGPWORKLIST_FIND, generalCFindSOPClass, DIM_OF(generalCFindSOPClass)},
     /* UPS Pull SOP Class, Supplement 96 */
    {"1.2.840.10008.5.1.4.34.4.3", generalCFindSOPClass, DIM_OF(generalCFindSOPClass)},
    {DICOM_SOPGPSPS, gpspsSOPClass, DIM_OF(gpspsSOPClass)},
    {DICOM_SOPGPPPS, gpppsSOPClass, DIM_OF(gpppsSOPClass)},
    {"1.2.840.10008.5.1.4.33", ianSOPClass, DIM_OF(ianSOPClass)},
};

/* verifyCommandValidity
**
** Purpose:
**	Verify if the command is valid for the proposed abstract syntax
**
** Parameter Dictionary:
**	params		The association service parameters which contain the
**			list of all the abstract syntaxes accepted on
**			the association.
**	ctxid		The context id which determines the current abstract
**			syntax for which the command is received
**	command		The DICOM normalized/composite service being requested
**			for the abstract syntax.
**
** Return Values:
**	SRV_NORMAL
**	SRV_UNSUPPORTEDCOMMAND
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
PRVSRV_verifyCommandValidity(DUL_ASSOCIATESERVICEPARAMETERS * params,
			     DUL_PRESENTATIONCONTEXTID ctxid,
			     unsigned short command)
{

    /*
     * verify if the command is valid for the given abstract syntax i.e. SOP
     * class
     */
    int
        index1,
        index2;
    DUL_PRESENTATIONCONTEXT
	* ctx;

    /*
     * using the ctxid, we first extract the abstract syntax for which the
     * command has been requested.
     */

    ctx = findPresentationCtx(params, ctxid);
    if (ctx == NULL)
	return SRV_UNSUPPORTEDCOMMAND;

    /*
     * From the current presentation context we have obtained, extract the
     * abstract syntax and find the list of valid commands for that abstract
     * syntax and then verify if the requested command is valid or not
     */
    for (index1 = 0; index1 < DIM_OF(classPermittedSrvTable); index1++) {
	if (strcmp(classPermittedSrvTable[index1].classUID,
		   ctx->abstractSyntax) == 0) {	/* they matched */
	    for (index2 = 0;
		 index2 <
		 (int) classPermittedSrvTable[index1].permittedSrvListSize;
		 index2++) {
		if (classPermittedSrvTable[index1].permittedSrvList[index2]
		    == command)
		    return SRV_NORMAL;
	    }
	    return SRV_UNSUPPORTEDCOMMAND;
	}
    }
    return SRV_UNSUPPORTEDCOMMAND;
}

/* findPresentationCtx
**
** Purpose:
**	Find the presentation context in the service parameters using the
**	context ID
**
** Parameter Dictionary:
**	params		Service parameters
**	ctxid		Context ID using which the presentation context
**			is to be returned
**
** Return Values:
**	Handle to the presentation context, if found, else NULL.
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static DUL_PRESENTATIONCONTEXT
*
findPresentationCtx(DUL_ASSOCIATESERVICEPARAMETERS * params,
		    DUL_PRESENTATIONCONTEXTID ctxid)
{
    DUL_PRESENTATIONCONTEXT
    * ctx;

    if (params->acceptedPresentationContext == NULL)
	return NULL;
    ctx = LST_Head(&params->acceptedPresentationContext);
    if (ctx == NULL)
	return NULL;
    (void) LST_Position(&params->acceptedPresentationContext, ctx);
    while (ctx != NULL) {
	if (ctx->presentationContextID == ctxid)	/* context id found */
	    break;
	ctx = LST_Next(&params->acceptedPresentationContext);
    }

    return ctx;
}

