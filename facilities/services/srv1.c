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
** Module Name(s):	SRV_AcceptServiceClass
**			SRV_MessageIDOut
**			SRV_MessageIDIn
**			SRV_RejectServiceClass
**			SRV_RequestServiceClass
**	private modules
**			SRVPRV_ReadNextPDV
**			writeCallback
**			verifyCommandValidity
**			dequeCommand
**			enqueCommand
**			findPresentationContext
**			createFile
** Author, Date:	Stephen M. Moore, 15-Apr-93
** Intent:		This module contains general routines which are used
**			in our implementation of service classes.  These
**			routines allow users to request and accept service
**			classes, build and manipulate the public DUL
**			structures, send and receive messages, and request
**			unique Message IDs.
** Last Update:		$Author: smm $, $Date: 2008-03-19 20:41:26 $
** Source File:		$RCSfile: srv1.c,v $
** Revision:		$Revision: 1.48 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.48 $ $RCSfile: srv1.c,v $";

#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#ifdef _MSC_VER
#include <io.h>
#include <fcntl.h>
#else
#include <sys/file.h>
#endif
#ifdef MALLOC_DEBUG
#include "malloc.h"
#endif
#ifdef SOLARIS
#include <sys/fcntl.h>
#endif
#endif

#include "dicom.h"
#include "dicom_uids.h"
#include "condition.h"
#include "lst.h"
#include "utility.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#ifdef CTN_USE_THREADS
#ifndef _MSC_VER
#include <synch.h>
#endif
#include "ctnthread.h"
#endif
#include "private.h"

#define FRAGMENTMAX 65536

typedef unsigned long SRVPERMITTED;

typedef struct {
    char classUID[DICOM_UI_LENGTH + 1];
    SRVPERMITTED *permittedSrvList;	/* list of permitted services */
    unsigned short permittedSrvListSize;
}   SOPCLASSPERMITTEDSRV;	/* defines the various services permitted for
				 * a given SOP class */

/*
static char
    fragmentBuffer[FRAGMENTMAX + 1000];
DUL_PDVLIST pdvList =
{0, fragmentBuffer, sizeof(fragmentBuffer), {0x00, 0x00, 0x00}, NULL};
*/

static char *syntaxList[] = {
    DICOM_SOPCLASSVERIFICATION,
    /* Storage Classes Ordered according to PS 3.4, Table B.5-1 */

    DICOM_SOPCLASSCOMPUTEDRADIOGRAPHY,	/* "1.2.840.10008.5.1.4.1.1.1" */
	"1.2.840.10008.5.1.4.1.1.1.1",			/* Digital X-Ray Image Storage - For Presentation */
	"1.2.840.10008.5.1.4.1.1.1.1.1",		/* Digital X-Ray Image Storage - For Processing */
	"1.2.840.10008.5.1.4.1.1.1.2",			/* Digital Mammography Im Storage - For Presentation */
	"1.2.840.10008.5.1.4.1.1.1.2.1",		/* Digital Mammography Im Storage - For Processing */
	"1.2.840.10008.5.1.4.1.1.1.3",			/* Digital Intra-oral X-Ray Im Storage - Presentation */
	"1.2.840.10008.5.1.4.1.1.1.3.1",		/* Digital Intra-oral X-Ray Im Storage - Processing */
    DICOM_SOPCLASSCT,						/* "1.2.840.10008.5.1.4.1.1.2" */
    DICOM_SOPCLASSENHANCEDCT,				/* "1.2.840.10008.5.1.4.1.1.2.1" */
	"1.2.840.10008.5.1.4.1.1.3.1",			/* Ultrasound Multii-frame Im Storage */
    DICOM_SOPCLASSMR,						/* "1.2.840.10008.5.1.4.1.1.4" */
    DICOM_SOPCLASSENHANCEDMR,				/* "1.2.840.10008.5.1.4.1.1.4.1" */	
    "1.2.840.10008.5.1.4.1.1.4.2",			/* MR Spectroscopy data */
    "1.2.840.10008.5.1.4.1.1.4.3",			/* Enhanced MR Color Im Storage */
    DICOM_SOPCLASSUS,						/* "1.2.840.10008.5.1.4.1.1.6.1" */	
	"1.2.840.10008.5.1.4.1.1.6.2",			/* Enhanced US Volume Storage */
    DICOM_SOPCLASSSECONDARYCAPTURE,			/* "1.2.840.10008.5.1.4.1.1.7" */
	"1.2.840.10008.5.1.4.1.1.7.1",			/* Multi-frame Single Bit Secondary Capture Im Storage */
	"1.2.840.10008.5.1.4.1.1.7.2",			/* Multi-frame Grayscale Byte Secondary Capture Im Storage */
	"1.2.840.10008.5.1.4.1.1.7.3",			/* Multi-frame Grayscalw Word Secondary Capture Im Storage */
	"1.2.840.10008.5.1.4.1.1.7.4",			/* Multi-frame True Color Secondary Capture Im Storage */
	"1.2.840.10008.5.1.4.1.1.9.1.1",		/* 12-lead ECG Waveform Storage */
	"1.2.840.10008.5.1.4.1.1.9.1.2",		/* General ECG Waveform Storage */
	"1.2.840.10008.5.1.4.1.1.9.1.3",		/* Ambulatory ECG Waveform Storage */
	"1.2.840.10008.5.1.4.1.1.9.2.1",		/* Hemodynamic Waveform Storage */
	"1.2.840.10008.5.1.4.1.1.9.3.1",		/* Cardiac Electrophysiology Waveform Storage */
	"1.2.840.10008.5.1.4.1.1.9.4.1",		/* Basic Voice Audio Waveform Storage */
	"1.2.840.10008.5.1.4.1.1.9.4.2",		/* General Audio Waveform Storage */
	"1.2.840.10008.5.1.4.1.1.9.5.1",		/* Arterial Pulse Waveform Storage */
	"1.2.840.10008.5.1.4.1.1.9.6.1",		/* Respiratory Waveform Storage */
	"1.2.840.10008.5.1.4.1.1.11.1",			/* Grayscale Softcopy Presentation State Storage */
	"1.2.840.10008.5.1.4.1.1.11.2",			/* Color Softcopy Presentation State Storage */
	"1.2.840.10008.5.1.4.1.1.11.3",			/* Pseudo-Color Softcopy Presentation State Storage */
	"1.2.840.10008.5.1.4.1.1.11.4",			/* Blending Softcopy Presentation State Storage */
	"1.2.840.10008.5.1.4.1.1.11.5",			/* XA/XRF Grayscale Softcopy Presentation State Storage */
    DICOM_SOPCLASSXRAYANGIO,				/* "1.2.840.10008.5.1.4.1.1.12.1" */
	"1.2.840.10008.5.1.4.1.1.12.1.1",		/* Enhanced XA Image Storage */
    DICOM_SOPCLASSXRAYFLUORO,				/* "1.2.840.10008.5.1.4.1.1.12.2" */
	"1.2.840.10008.5.1.4.1.1.12.2.1",		/* Enhanced XRF Image Storage */
	"1.2.840.10008.5.1.4.1.1.13.1.1",		/* X-Ray 3D Angiographic Image Storage */
	"1.2.840.10008.5.1.4.1.1.13.1.2",		/* X-Ray 3D Craniofacial Image Storage */
	"1.2.840.10008.5.1.4.1.1.13.1.3",		/* Breast Tomosynthesis Image Storage */
    DICOM_SOPCLASSNM,						/* "1.2.840.10008.5.1.4.1.1.20" */
    "1.2.840.10008.5.1.4.1.1.66",			/* Raw data */
	"1.2.840.10008.5.1.4.1.1.66.1",			/* Spatial Registration Storage */
	"1.2.840.10008.5.1.4.1.1.66.2",			/* Spatial Fiducials Storage */
	"1.2.840.10008.5.1.4.1.1.66.3",			/* Deformable Spatial Registration Storage */
	"1.2.840.10008.5.1.4.1.1.66.4",			/* Segmentation Storage */
	"1.2.840.10008.5.1.4.1.1.66.5",			/* Surface Segmentation Storage */
	"1.2.840.10008.5.1.4.1.1.67",			/* Real World Value Mapping Storage */
    DICOM_SOPCLASSVLENDOSCOPIC,				/* "1.2.840.10008.5.1.4.1.1.77.1.1" */
	"1.2.840.10008.5.1.4.1.1.77.1.1.1",		/* Video Endoscopic Image Storage */
    DICOM_SOPCLASSVLMICROSCOPIC,			/* "1.2.840.10008.5.1.4.1.1.77.1.2" */
	"1.2.840.10008.5.1.4.1.1.77.1.2.1",		/* Video Microscopic Image Storage */
    DICOM_SOPCLASSVLSLIDEMICROSCOPIC,		/* "1.2.840.10008.5.1.4.1.1.77.1.3" */
    DICOM_SOPCLASSVLPHOTOGRAPHIC,			/* "1.2.840.10008.5.1.4.1.1.77.1.4" */
	"1.2.840.10008.5.1.4.1.1.77.1.4.1",		/* Video Photographic Image Storage */
    "1.2.840.10008.5.1.4.1.1.77.1.5.1",		/* Opthalmic Photo 8 Bit Image */
    "1.2.840.10008.5.1.4.1.1.77.1.5.2",		/* Opthalmic Photo 16 Bit Image */
    "1.2.840.10008.5.1.4.1.1.77.1.5.3",		/* Stereometric Relationship Storage*/
	"1.2.840.10008.5.1.4.1.1.77.1.5.4",		/* Ophthalmic Tomography Image Storage */
	"1.2.840.10008.5.1.4.1.1.78.1",			/* Lensometry Measurements Storage */
	"1.2.840.10008.5.1.4.1.1.78.2",			/* Autorefraction Measurements Storage */
	"1.2.840.10008.5.1.4.1.1.78.3",			/* Keratometry Measurements Storage */
	"1.2.840.10008.5.1.4.1.1.78.4",			/* Subjective Refraction Measurements Storage */
	"1.2.840.10008.5.1.4.1.1.78.5",			/* Visual Acuity Measurements Storage */
	"1.2.840.10008.5.1.4.1.1.78.6",			/* Spectacle Prescription  Report Storage */
	"1.2.840.10008.5.1.4.1.1.79.1",			/* Macular Grid Thichness and Volume Report */
    DICOM_SOPCLASSBASICTEXTSR,				/* "1.2.840.10008.5.1.4.1.1.88.11" */
    DICOM_SOPCLASSENHANCEDSR,				/* "1.2.840.10008.5.1.4.1.1.88.22" */
    DICOM_SOPCLASSCOMPREHENSIVESR,			/* "1.2.840.10008.5.1.4.1.1.88.33" */
    "1.2.840.10008.5.1.4.1.1.88.40",		/* Procedure Log */
    "1.2.840.10008.5.1.4.1.1.88.50",		/* Mammo CAD SR */
    "1.2.840.10008.5.1.4.1.1.88.59",		/* Key Object Selection */
	"1.2.840.10008.5.1.4.1.1.88.65",		/* Chest CAD SR */
	"1.2.840.10008.5.1.4.1.1.88.67",		/* X-Ray Radiation Dose SR */
	"1.2.840.10008.5.1.4.1.1.88.69",		/* Colon CAD SR */
	DICOM_SOPCLASSENCAPSULATEDPDF,			/* "1.2.840.10008.5.1.4.1.1.104.1" */
	"1.2.840.10008.5.1.4.1.1.104.2",		/* Encapsulated CDA Storage */
    DICOM_SOPCLASSPET,						/* "1.2.840.10008.5.1.4.1.1.128" */
	"1.2.840.10008.5.1.4.1.1.130",			/* Enhanced PET Image Storage */
	"1.2.840.10008.5.1.4.1.1.131",			/* Basic Structured Display Storage */
    DICOM_SOPRTIMAGESTORAGE,				/* "1.2.840.10008.5.1.4.1.1.481.1" */
    DICOM_SOPRTDOSESTORAGE,					/* "1.2.840.10008.5.1.4.1.1.481.2" */
    DICOM_SOPRTSTRUCTURESETSTORAGE,			/* "1.2.840.10008.5.1.4.1.1.481.3" */
    DICOM_SOPRTBREAMS,						/* "1.2.840.10008.5.1.4.1.1.481.4" */
    DICOM_SOPRTPLANSTORAGE,					/* "1.2.840.10008.5.1.4.1.1.481.5" */
    DICOM_SOPRTBRACHYTREATMENT,				/* "1.2.840.10008.5.1.4.1.1.481.6" */
    DICOM_SOPRTTREATMENTSUMMARY,			/* "1.2.840.10008.5.1.4.1.1.481.7" */
	"1.2.840.10008.5.1.4.1.1.481.8",		/* RT Ion Plan Storage */
	"1.2.840.10008.5.1.4.1.1.481.9",		/* RT Ion Beams Treatment Record */

    DICOM_SOPCLASSHARDCOPYCOLORIMAGE,	/* "1.2.840.10008.5.1.1.30" */
    DICOM_SOPCLASSHARDCOPYGRAYSCALEIMAGE, /* "1.2.840.10008.5.1.1.29" */
    DICOM_SOPCLASSSTANDALONECURVE,	/* "1.2.840.10008.5.1.4.1.1.9" */
    DICOM_SOPCLASSSTANDALONEMODALITYLUT, /* "1.2.840.10008.5.1.4.1.1.10" */
    DICOM_SOPCLASSSTANDALONEOVERLAY,	/* "1.2.840.10008.5.1.4.1.1.8" */
    DICOM_SOPCLASSSTANDALONEVOILUT,	/* "1.2.840.10008.5.1.4.1.1.11" */
    DICOM_SOPCLASSSTANDALONEPETCURVE,	/* "1.2.840.10008.5.1.4.1.1.129" */
    DICOM_SOPCLASSSTOREDPRINT,		/* "1.2.840.10008.5.1.1.27" */

    /* Some additional storage classes */
    DICOM_SOPCLASSUSMULTIFRAMEIMAGE1993,/* "1.2.840.10008.5.1.4.1.1.3" */
    DICOM_SOPCLASSNM1993,		/* "1.2.840.10008.5.1.4.1.1.5" */
    DICOM_SOPCLASSUS1993,		/* "1.2.840.10008.5.1.4.1.1.6" */

    DICOM_SOPCLASSXRAYANGIOBIPLANE_RET, /* "1.2.840.10008.5.1.4.1.1.12.3" */
    "1.3.12.2.1107.5.9.1",		/* SMS Raw CT data */

    DICOM_SOPPATIENTQUERY_FIND,
    DICOM_SOPPATIENTQUERY_MOVE,
    DICOM_SOPPATIENTQUERY_GET,
    DICOM_SOPSTUDYQUERY_FIND,
    DICOM_SOPSTUDYQUERY_MOVE,
    DICOM_SOPSTUDYQUERY_GET,
    DICOM_SOPPATIENTSTUDYQUERY_FIND,
    DICOM_SOPPATIENTSTUDYQUERY_MOVE,
    DICOM_SOPPATIENTSTUDYQUERY_GET,
    DICOM_SOPCLASSGREYSCALEPRINTMGMTMETA,
    DICOM_SOPCLASSCOLORPRINTMGMTMETA,
    DICOM_SOPCLASSDETACHEDPATIENTMGMT,
    DICOM_SOPCLASSDETACHEDVISITMGMT,
    DICOM_SOPCLASSDETACHEDPATIENTMGMTMETA,
    DICOM_SOPCLASSDETACHEDSTUDYMGMT,
    DICOM_SOPCLASSDETACHEDRESULTSMGMT,
    DICOM_SOPCLASSDETACHEDINTERPRETMGMT,
    DICOM_SOPCLASSDETACHEDRESULTSMGMTMETA,
    DICOM_SOPCLASSSTUDYCOMPONENTMGMT,
    DICOM_SOPCLASSDETACHEDSTUDYMGMTMETA,
    DICOM_SOPCLASSSTORAGECOMMITMENTPUSHMODEL,
    DICOM_SOPMODALITYWORKLIST_FIND,
    DICOM_SOPGPWORKLIST_FIND, /* "1.2.840.10008.5.1.4.32.1" */
    DICOM_SOPGPSPS, 		/* "1.2.840.10008.5.1.4.32.2" */
    DICOM_SOPGPPPS, 		/* "1.2.840.10008.5.1.4.32.3" */
    DICOM_SOPCLASSMPPS,

    "1.2.840.10008.5.1.4.33",		/* Instance Availability Notification SOP Class */

    "1.2.840.10008.5.1.4.34.4.3",	/* UPS Pull SOP Class */
    MIR_SOPCLASSKILLSERVER,		/* Private MIR SOP Class */
    MIR_SOPCLASSRESETSERVER		/* Private MIR SOP Class */

};

typedef struct {
    void *reserved[2];
    DUL_ASSOCIATIONKEY **association;
    DUL_PRESENTATIONCONTEXTID ctxID;
    unsigned short command;
    MSG_TYPE messageType;
    void *message;
}   COMMAND_ENTRY;

extern CTNBOOLEAN PRVSRV_debug;
static DUL_PRESENTATIONCONTEXT
*
findPresentationCtx(DUL_ASSOCIATESERVICEPARAMETERS * params,
		    DUL_PRESENTATIONCONTEXTID ctxid);

/*
static char *xx[4] = { DICOM_TRANSFERLITTLEENDIANEXPLICIT,
	DICOM_TRANSFERBIGENDIANEXPLICIT,
	DICOM_TRANSFERLITTLEENDIAN, 0 };
static char** supportedTransferSyntax = xx;
*/

/*
static char**
mapAbstractSyntaxToXferSyntax(const char* SOPClass,
			     int* xferSyntaxCount, int* singleMode)
{
  *xferSyntaxCount = 3;
  *singleMode = 0;

  return xx;
}
*/

/*
static char**
mapStorageSOPToXferSyntax(const char* SOPClass,
		       int* xferSyntaxCount, int* singleMode)
{
  char* paramValue;
  char paramName[1024] = "PROPOSE/XFER/STORAGE/";

  strcat(paramName, SOPClass);

  paramValue = UTL_GetConfigParameter(paramName);
  if (paramValue == NULL)
    paramValue = DICOM_TRANSFERLITTLEENDIAN;

  *singleMode = 1;
  return UTL_ExpandToPointerArray(paramValue, ";", xferSyntaxCount);
}
*/

static char**
mapProposedSOPToXferSyntax(const char* SOPClass, const char* prefix,
		   int *xferSyntaxCount, int* singleMode)
{
  char paramName[1024];
  char* paramValue;

  paramValue = UTL_GetConfigParameter("PROPOSE/SINGLE_PRESENTATION");
  if (paramValue == NULL) {
    *singleMode = 0;
  } else if (strcmp(paramValue, "1") == 0) {
    *singleMode = 1;
  } else {
    *singleMode = 0;
  }

  strcpy(paramName, prefix);
  strcat(paramName, "/");
  strcat(paramName, SOPClass);

  paramValue = UTL_GetConfigParameter(paramName);
  if (paramValue == NULL)
    paramValue = DICOM_TRANSFERLITTLEENDIAN;

  return UTL_ExpandToPointerArray(paramValue, ";", xferSyntaxCount);
}

static DUL_TRANSFERSYNTAX*
matchProposedXferSyntax(DUL_PRESENTATIONCONTEXT* requestedCtx,
			const char* prefix,
			char** xferSyntaxes, int xferSyntaxCount)
{
  char** xferSyntaxesLocal = 0;
  int singleMode = 0;
  DUL_TRANSFERSYNTAX* transfer;
  int found = 0;
  int idx;

  if (xferSyntaxCount == 0) {
    char* paramValue;
    char paramName[1024] = "";
    strcpy(paramName, prefix);
    strcat(paramName, "/");
    strcat(paramName, requestedCtx->abstractSyntax);

    paramValue = UTL_GetConfigParameter(paramName);
    if (paramValue == NULL)
      paramValue = DICOM_TRANSFERLITTLEENDIAN;

    xferSyntaxesLocal = UTL_ExpandToPointerArray(paramValue, ";", &xferSyntaxCount);
  } else {
    xferSyntaxesLocal = xferSyntaxes;
  }

  for (idx = 0; (idx < xferSyntaxCount) && !found; idx++) {
    transfer = (DUL_TRANSFERSYNTAX*)LST_Head(&requestedCtx->proposedTransferSyntax);
    (void) LST_Position(&requestedCtx->proposedTransferSyntax, transfer);
    while (transfer != NULL) {
//	  COND_PushCondition(0, "%s %s", transfer->transferSyntax, xferSyntaxesLocal[idx]); /*repair*/
      if (strcmp(transfer->transferSyntax, xferSyntaxesLocal[idx]) == 0) {
	found = 1;
	break;
      }
      else
	transfer = LST_Next(&requestedCtx->proposedTransferSyntax);
    }
  }

  if (!found) {
	  DUL_TRANSFERSYNTAX* x;
	  x = (DUL_TRANSFERSYNTAX*)LST_Head(&requestedCtx->proposedTransferSyntax);
	  (void)LST_Position(&requestedCtx->proposedTransferSyntax, x);
	  while (x != NULL) {
		  COND_PushCondition(SRV_REQUESTEDXFER, "SRV REQUESTED transfer syntax: %s",
			  x->transferSyntax);
		  x = (DUL_TRANSFERSYNTAX*)LST_Next(&requestedCtx->proposedTransferSyntax);
	  }
	  for (idx = 0; (idx < xferSyntaxCount) && !found; idx++) {
		  COND_PushCondition(SRV_SUPPORTEDXFER, "SRV Supported transfer syntax: %s",
			  xferSyntaxesLocal[idx]);
	  }
	  transfer = 0;
  }

  if (xferSyntaxesLocal != xferSyntaxes) {
    CTN_FREE(xferSyntaxesLocal);
  }
  return transfer;
}

/* SRV_RequestServiceClass
**
** Purpose:
**	This function is called by an application which is proposing an
**	Association and wishes to request a service class.  The application
**	can request the Service Class as an SCU or as an SCP.  This function
**	determines if the SERVICES library supports the service class
**	(table lookup).  If so, it builds a Presentation Context for the
**	service and adds it to the list of Presentation Contexts for the
**	Association which is to be requested.
**
** Parameter Dictionary:
**	SOPClass	UID of the abstract syntrax which defines the
**			service class
**	role		Role the application wishes to propose for this
**			class (SCU, SCP, both, default)
**	params		Parameter list for the Association to be requested.
**			This includes the list of Presentation Contexts
**			for the Association.
**
** Return Values:
**
**	SRV_LISTFAILURE
**	SRV_MALLOCFAILURE
**	SRV_NORMAL
**	SRV_PRESENTATIONCONTEXTERROR
**	SRV_UNSUPPORTEDSERVICE
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
SRV_RequestServiceClass(const char *SOPClass, DUL_SC_ROLE role,
			DUL_ASSOCIATESERVICEPARAMETERS * params)
{
    DUL_PRESENTATIONCONTEXTID
	contextID = 1;
    int
        index;
    CTNBOOLEAN
	found = FALSE;
    CONDITION
	cond;
    DUL_PRESENTATIONCONTEXT
	* ctx;

    if (params->requestedPresentationContext == NULL) {
	params->requestedPresentationContext = LST_Create();
	if (params->requestedPresentationContext == NULL) {
	    return COND_PushCondition(SRV_LISTFAILURE,
		   SRV_Message(SRV_LISTFAILURE), "SRV_RequestServiceClass");
	}
    }
    ctx = LST_Head(&params->requestedPresentationContext);
    if (ctx != NULL)
	(void) LST_Position(&params->requestedPresentationContext, ctx);
    while (ctx != NULL) {
	contextID += 2;
	if (strcmp(SOPClass, ctx->abstractSyntax) == 0)
	    return SRV_NORMAL;

	ctx = LST_Next(&params->requestedPresentationContext);
    }

    for (index = 0; index < (int) DIM_OF(syntaxList) && !found; index++) {
	if (strcmp(SOPClass, syntaxList[index]) == 0) {
	    found = TRUE;
	}
    }
    if (found) {
	char **xferSyntaxes;

	int singleMode = 1;
	int xferSyntaxCount = 0;

	xferSyntaxes = mapProposedSOPToXferSyntax(SOPClass,
		"PROPOSE/XFER", &xferSyntaxCount, &singleMode);

	if (singleMode == 1) {
	    cond = DUL_AddSinglePresentationCtx(params,
			role, DUL_SC_ROLE_DEFAULT, contextID,
			0, SOPClass, (const char**)xferSyntaxes,
			xferSyntaxCount);
	} else {
	    cond = DUL_AddMultiplePresentationCtx(params,
			role, DUL_SC_ROLE_DEFAULT, contextID,
			0, SOPClass, (const char**)xferSyntaxes,
			xferSyntaxCount);
	}
	if (cond != DUL_NORMAL) {
	    return COND_PushCondition(SRV_PRESENTATIONCONTEXTERROR,
			SRV_Message(SRV_PRESENTATIONCONTEXTERROR),
			"SRV_RequestServiceClass");
	}
/* Legacy version removed 1/3/2001, smm
	    cond = DUL_MakePresentationCtx(&ctx, role, DUL_SC_ROLE_DEFAULT,
				       contextID, 0, SOPClass, "",
				       DICOM_TRANSFERLITTLEENDIAN, NULL);
	if (cond != DUL_NORMAL)
	    return COND_PushCondition(SRV_PRESENTATIONCONTEXTERROR,
				  SRV_Message(SRV_PRESENTATIONCONTEXTERROR),
				      "SRV_RequestServiceClass");
	else {
	    cond = LST_Enqueue(&params->requestedPresentationContext, ctx);
	    if (cond != LST_NORMAL)
		return COND_PushCondition(SRV_LISTFAILURE,
		   SRV_Message(SRV_LISTFAILURE), "SRV_RequestServiceClass");

	    contextID += 2;
	}
*/

    } else
	return COND_PushCondition(SRV_UNSUPPORTEDSERVICE,
			      SRV_Message(SRV_UNSUPPORTEDSERVICE), SOPClass,
				  "SRV_RequestServiceClass");

    return SRV_NORMAL;
}

CONDITION
SRV_RegisterSOPClass(const char *SOPClass, DUL_SC_ROLE role,
		     DUL_ASSOCIATESERVICEPARAMETERS * params)
{
  DUL_PRESENTATIONCONTEXTID contextID = 1;
  CTNBOOLEAN found = FALSE;
  CONDITION cond;
  DUL_PRESENTATIONCONTEXT* ctx;

  if (params->requestedPresentationContext == NULL) {
    params->requestedPresentationContext = LST_Create();
    if (params->requestedPresentationContext == NULL) {
      return COND_PushCondition(SRV_LISTFAILURE,
		   SRV_Message(SRV_LISTFAILURE), "SRV_RegisterSOPClass");
    }
  }
  ctx = LST_Head(&params->requestedPresentationContext);
  if (ctx != NULL)
    (void) LST_Position(&params->requestedPresentationContext, ctx);
  while (ctx != NULL) {
    contextID += 2;
    if (strcmp(SOPClass, ctx->abstractSyntax) == 0)
      return SRV_NORMAL;

    ctx = LST_Next(&params->requestedPresentationContext);
  }

  cond = DUL_MakePresentationCtx(&ctx, role, DUL_SC_ROLE_DEFAULT,
				       contextID, 0, SOPClass, "",
				       DICOM_TRANSFERLITTLEENDIAN, NULL);
  if (cond != DUL_NORMAL) {
    return COND_PushCondition(SRV_PRESENTATIONCONTEXTERROR,
				  SRV_Message(SRV_PRESENTATIONCONTEXTERROR),
				      "SRV_RegisterSOPClass");
  }

  cond = LST_Enqueue(&params->requestedPresentationContext, ctx);
  if (cond != LST_NORMAL) {
    return COND_PushCondition(SRV_LISTFAILURE,
		   SRV_Message(SRV_LISTFAILURE), "SRV_RegisterSOPClass");
  }

  return SRV_NORMAL;
}

CONDITION
SRV_RegisterSOPClassXfer(const char *SOPClass, const char* xferSyntax,
		     DUL_SC_ROLE role,
		     DUL_ASSOCIATESERVICEPARAMETERS * params)
{
  DUL_PRESENTATIONCONTEXTID contextID = 1;
  CTNBOOLEAN found = FALSE;
  CONDITION cond;
  DUL_PRESENTATIONCONTEXT* ctx;

  if (params->requestedPresentationContext == NULL) {
    params->requestedPresentationContext = LST_Create();
    if (params->requestedPresentationContext == NULL) {
      return COND_PushCondition(SRV_LISTFAILURE,
		   SRV_Message(SRV_LISTFAILURE), "SRV_RegisterSOPClassXfer");
    }
  }
  ctx = LST_Head(&params->requestedPresentationContext);
  if (ctx != NULL)
    (void) LST_Position(&params->requestedPresentationContext, ctx);
  while (ctx != NULL) {
    DUL_TRANSFERSYNTAX* xferItem;
    contextID += 2;
    xferItem = (DUL_TRANSFERSYNTAX*) LST_Head(&ctx->proposedTransferSyntax);
    if ((strcmp(SOPClass, ctx->abstractSyntax) == 0) &&
	(strcmp(xferSyntax, xferItem->transferSyntax) == 0) &&
	(role == ctx->proposedSCRole) )
      return SRV_NORMAL;

    ctx = LST_Next(&params->requestedPresentationContext);
  }

  cond = DUL_MakePresentationCtx(&ctx, role, DUL_SC_ROLE_DEFAULT,
				       contextID, 0, SOPClass, "",
				       xferSyntax, NULL);
  if (cond != DUL_NORMAL) {
    return COND_PushCondition(SRV_PRESENTATIONCONTEXTERROR,
				  SRV_Message(SRV_PRESENTATIONCONTEXTERROR),
				      "SRV_RegisterSOPClassXfer");
  }

  cond = LST_Enqueue(&params->requestedPresentationContext, ctx);
  if (cond != LST_NORMAL) {
    return COND_PushCondition(SRV_LISTFAILURE,
		   SRV_Message(SRV_LISTFAILURE), "SRV_RegisterSOPClassXfer");
  }

  return SRV_NORMAL;
}

/*
CONDITION
SRV_RegisterStorageSOPClass(const char *SOPClass, DUL_SC_ROLE role,
			    DUL_ASSOCIATESERVICEPARAMETERS * params)
{
  DUL_PRESENTATIONCONTEXTID contextID = 1;
  CTNBOOLEAN found = FALSE;
  CONDITION cond;
  DUL_PRESENTATIONCONTEXT* ctx;
  char **xferSyntaxes;
  int singleMode = 1;
  int xferSyntaxCount = 0;

  if (params->requestedPresentationContext == NULL) {
    params->requestedPresentationContext = LST_Create();
    if (params->requestedPresentationContext == NULL) {
      return COND_PushCondition(SRV_LISTFAILURE,
		   SRV_Message(SRV_LISTFAILURE), "SRV_RequestServiceClass");
    }
  }
  ctx = LST_Head(&params->requestedPresentationContext);
  if (ctx != NULL)
    (void) LST_Position(&params->requestedPresentationContext, ctx);
  while (ctx != NULL) {
    contextID += 2;
    if (strcmp(SOPClass, ctx->abstractSyntax) == 0)
      return SRV_NORMAL;

    ctx = LST_Next(&params->requestedPresentationContext);
  }

  xferSyntaxes = mapStorageSOPToXferSyntax(SOPClass,
		&xferSyntaxCount, &singleMode);

  if (singleMode == 1) {
    cond = DUL_AddSinglePresentationCtx(params,
			role, role, contextID,
			0, SOPClass, xferSyntaxes,
			xferSyntaxCount);
  } else {
    cond = DUL_AddMultiplePresentationCtx(params,
			role, role, contextID,
			0, SOPClass, xferSyntaxes,
			xferSyntaxCount);
  }
  CTN_FREE(xferSyntaxes);

  if (cond != DUL_NORMAL) {
    return COND_PushCondition(SRV_PRESENTATIONCONTEXTERROR,
				  SRV_Message(SRV_PRESENTATIONCONTEXTERROR),
				      "SRV_RegisterStorageSOPClass");
  }

  return SRV_NORMAL;
}
*/

CONDITION
SRV_ProposeSOPClassWithXfer(const char*SOPClass, DUL_SC_ROLE role,
			     char** xferSyntaxes, int xferSyntaxCount,
			     int isStorageClass,
			     DUL_ASSOCIATESERVICEPARAMETERS* params)
{
  DUL_PRESENTATIONCONTEXTID contextID = 1;
  CTNBOOLEAN found = FALSE;
  CONDITION cond;
  DUL_PRESENTATIONCONTEXT* ctx;
  char** xferSyntaxesLocal = 0;
  char* prefix = "PROPOSE/XFER";
  int singleMode = 1;

  if (params->requestedPresentationContext == NULL) {
    params->requestedPresentationContext = LST_Create();
    if (params->requestedPresentationContext == NULL) {
      return COND_PushCondition(SRV_LISTFAILURE,
	   SRV_Message(SRV_LISTFAILURE), "SRV_ProposeSOPClassWithXfer");
    }
  }

  ctx = LST_Head(&params->requestedPresentationContext);
  if (ctx != NULL)
    (void) LST_Position(&params->requestedPresentationContext, ctx);
  while (ctx != NULL) {
    contextID += 2;
    if (strcmp(SOPClass, ctx->abstractSyntax) == 0)
      return SRV_NORMAL;

    ctx = LST_Next(&params->requestedPresentationContext);
  }

  if (xferSyntaxCount == 0) {
    if (isStorageClass)
      prefix = "PROPOSE/XFER/STORAGE";

    xferSyntaxesLocal = mapProposedSOPToXferSyntax(SOPClass,
		prefix, &xferSyntaxCount, &singleMode);
  } else {
    xferSyntaxesLocal = xferSyntaxes;
    singleMode = 1;
  }

  if (singleMode == 1) {
    cond = DUL_AddSinglePresentationCtx(params,
			role, DUL_SC_ROLE_DEFAULT, contextID,
			0, SOPClass, (const char**)xferSyntaxesLocal,
			xferSyntaxCount);
  } else {
    cond = DUL_AddMultiplePresentationCtx(params,
			role, DUL_SC_ROLE_DEFAULT, contextID,
			0, SOPClass, (const char**)xferSyntaxesLocal,
			xferSyntaxCount);
  }
  if (xferSyntaxesLocal != xferSyntaxes) {
    CTN_FREE(xferSyntaxesLocal);
  }

  if (cond != DUL_NORMAL) {
    return COND_PushCondition(SRV_PRESENTATIONCONTEXTERROR,
			SRV_Message(SRV_PRESENTATIONCONTEXTERROR),
			"SRV_ProposeSOPClassWithXfer");
  }

  if (cond != DUL_NORMAL)
    return COND_PushCondition(SRV_PRESENTATIONCONTEXTERROR,
				  SRV_Message(SRV_PRESENTATIONCONTEXTERROR),
				      "SRV_ProposeSOPClassWithXfer");


  return SRV_NORMAL;
}

/* SRV_AcceptServiceClass
**
** Purpose:
**	Determine if the SRV facility can accept a proposed service class and
**	and build the appropriate response for the Association Accept message.
**
** Parameter Dictionary:
**	requestedCtx	The presentation context for the sevice which has been
**			requested by the Requesting Application. This context
**			includes the UID of the service class as well as the
**			proposed transfer syntax UIDs.
**	role		Role proposed by the application for this service
**			class.
**	params		The list of service parameters for the Association
**			which is being negotiated.
**
** Return Values:
**
**	SRV_LISTFAILURE
**	SRV_NORMAL
**	SRV_PRESENTATIONCONTEXTERROR
**	SRV_PRESENTATIONCTXREJECTED
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
SRV_AcceptServiceClass(DUL_PRESENTATIONCONTEXT * requestedCtx,
		  DUL_SC_ROLE role, DUL_ASSOCIATESERVICEPARAMETERS * params)
{
    int
        index;
    CTNBOOLEAN
	abstractFound = FALSE,
	transferFound = FALSE;

    CONDITION
	cond,
	rtnCond = SRV_NORMAL;
    DUL_PRESENTATIONCONTEXT
	* ctx;
    DUL_TRANSFERSYNTAX
	* transfer;

    if (params->acceptedPresentationContext == NULL) {
	params->acceptedPresentationContext = LST_Create();
	if (params->acceptedPresentationContext == NULL) {
	    return COND_PushCondition(SRV_LISTFAILURE,
		    SRV_Message(SRV_LISTFAILURE), "SRV_AcceptServiceClass");
	}
    }
    for (index = 0; index < (int) DIM_OF(syntaxList) && !abstractFound; index++) {
	if (strcmp(requestedCtx->abstractSyntax, syntaxList[index]) == 0) {
	    abstractFound = TRUE;
	}
    }
    if (abstractFound) {
	char* xferSyntaxes[] =
	{ "1.2.840.10008.1.2.4.50",	/* JPEG Baseline Process 1 */
	"1.2.840.10008.1.2.4.51",	/* JPEG Extended Process 2 & 4 */
	"1.2.840.10008.1.2.1",		/* Explicit VR Little Endian */
	"1.2.840.10008.1.2.1.99",	/* Deflated Explicit VR Little Endian */
	"1.2.840.10008.1.2.2",		/* Explicit VR Big Endian Endian */
	"1.2.840.10008.1.2.4.70",
	"1.2.840.10008.1.2.4.90",
	DICOM_TRANSFERLITTLEENDIAN };

	transfer = matchProposedXferSyntax(requestedCtx,
			"ACCEPT/XFER", xferSyntaxes, 8);
	if (transfer == NULL) {
	  cond = DUL_MakePresentationCtx(&ctx,
				       requestedCtx->proposedSCRole,
				       DUL_SC_ROLE_DEFAULT,
				       requestedCtx->presentationContextID,
				    DUL_PRESENTATION_REJECT_TRANSFER_SYNTAX,
				       requestedCtx->abstractSyntax,
	      DICOM_TRANSFERLITTLEENDIAN, DICOM_TRANSFERLITTLEENDIAN, NULL);
	  if (cond != DUL_NORMAL)
	    return COND_PushCondition(SRV_PRESENTATIONCONTEXTERROR,
				  SRV_Message(SRV_PRESENTATIONCONTEXTERROR),
				      "SRV_AcceptServiceClass");
	  (void) COND_PushCondition(SRV_UNSUPPORTEDTRANSFERSYNTAX,
				  "SRV No transfer syntaxes supported for SOP class (%s) in %s",
		    requestedCtx->abstractSyntax, "SRV_AcceptServiceClass");
	  rtnCond = COND_PushCondition(SRV_PRESENTATIONCTXREJECTED,
				   "SRV Presentation Context rejected for SOP class (%s) in %s",
		    requestedCtx->abstractSyntax, "SRV_AcceptServiceClass");
	} else {
	    cond = DUL_MakePresentationCtx(&ctx,
					 requestedCtx->proposedSCRole, role,
					requestedCtx->presentationContextID,
					   DUL_PRESENTATION_ACCEPT,
					   requestedCtx->abstractSyntax,
					   transfer->transferSyntax,
					   transfer->transferSyntax,
					   NULL);
	    if (cond != DUL_NORMAL)
		return COND_PushCondition(SRV_PRESENTATIONCONTEXTERROR,
				  SRV_Message(SRV_PRESENTATIONCONTEXTERROR),
					  "SRV_AcceptServiceClass");
	}
#if 0
	if ((transfer = LST_Head(&requestedCtx->proposedTransferSyntax)) == NULL)
	    return COND_PushCondition(SRV_LISTFAILURE,
		    SRV_Message(SRV_LISTFAILURE), "SRV_AcceptServiceClass");
	(void) LST_Position(&requestedCtx->proposedTransferSyntax, transfer);
	while (!transferFound && (transfer != NULL)) {
	    if (strcmp(transfer->transferSyntax, DICOM_TRANSFERLITTLEENDIAN) == 0)
		transferFound = TRUE;
	    else
		transfer = LST_Next(&requestedCtx->proposedTransferSyntax);
	}
	if (transferFound) {
	    cond = DUL_MakePresentationCtx(&ctx,
					 requestedCtx->proposedSCRole, role,
					requestedCtx->presentationContextID,
					   DUL_PRESENTATION_ACCEPT,
					   requestedCtx->abstractSyntax,
	      DICOM_TRANSFERLITTLEENDIAN, DICOM_TRANSFERLITTLEENDIAN, NULL);
	    if (cond != DUL_NORMAL)
		return COND_PushCondition(SRV_PRESENTATIONCONTEXTERROR,
				  SRV_Message(SRV_PRESENTATIONCONTEXTERROR),
					  "SRV_AcceptServiceClass");
	} else {
	    cond = DUL_MakePresentationCtx(&ctx,
					   requestedCtx->proposedSCRole,
					   DUL_SC_ROLE_DEFAULT,
					requestedCtx->presentationContextID,
				    DUL_PRESENTATION_REJECT_TRANSFER_SYNTAX,
					   requestedCtx->abstractSyntax,
	      DICOM_TRANSFERLITTLEENDIAN, DICOM_TRANSFERLITTLEENDIAN, NULL);
	    if (cond != DUL_NORMAL)
		return COND_PushCondition(SRV_PRESENTATIONCONTEXTERROR,
				  SRV_Message(SRV_PRESENTATIONCONTEXTERROR),
					  "SRV_AcceptServiceClass");

	    (void) COND_PushCondition(SRV_UNSUPPORTEDTRANSFERSYNTAX,
				 SRV_Message(SRV_UNSUPPORTEDTRANSFERSYNTAX),
		    requestedCtx->abstractSyntax, "SRV_AcceptServiceClass");
	    rtnCond = COND_PushCondition(SRV_PRESENTATIONCTXREJECTED,
				   SRV_Message(SRV_PRESENTATIONCTXREJECTED),
		    requestedCtx->abstractSyntax, "SRV_AcceptServiceClass");
	}
#endif
    } else {
	cond = DUL_MakePresentationCtx(&ctx,
				       requestedCtx->proposedSCRole,
				       DUL_SC_ROLE_DEFAULT,
				       requestedCtx->presentationContextID,
				    DUL_PRESENTATION_REJECT_ABSTRACT_SYNTAX,
				       requestedCtx->abstractSyntax,
	      DICOM_TRANSFERLITTLEENDIAN, DICOM_TRANSFERLITTLEENDIAN, NULL);
	if (cond != DUL_NORMAL)
	    return COND_PushCondition(SRV_PRESENTATIONCONTEXTERROR,
				  SRV_Message(SRV_PRESENTATIONCONTEXTERROR),
				      "SRV_AcceptServiceClass");
	(void) COND_PushCondition(SRV_UNSUPPORTEDSERVICE,
				  SRV_Message(SRV_UNSUPPORTEDSERVICE),
		    requestedCtx->abstractSyntax, "SRV_AcceptServiceClass");
	rtnCond = COND_PushCondition(SRV_PRESENTATIONCTXREJECTED,
				   SRV_Message(SRV_PRESENTATIONCTXREJECTED),
		    requestedCtx->abstractSyntax, "SRV_AcceptServiceClass");
    }
    cond = LST_Enqueue(&params->acceptedPresentationContext, ctx);
    if (cond != LST_NORMAL)
	return COND_PushCondition(SRV_LISTFAILURE, SRV_Message(SRV_LISTFAILURE),
				  "SRV_AcceptServiceClass");

/*    pdvList.count = 0; */

    return rtnCond;
}

/* SRV_AcceptServiceClassWithOneXferSyntax
**
** Purpose:
**	Determine if the SRV facility can accept a proposed service class and
**	and build the appropriate response for the Association Accept message.
**	This function is similar to other functions, but the caller
**	specifies one acceptable transfer syntax to be supported.
**
** Parameter Dictionary:
**	requestedCtx	The presentation context for the sevice which has been
**			requested by the Requesting Application. This context
**			includes the UID of the service class as well as the
**			proposed transfer syntax UIDs.
**	role		Role proposed by the application for this service
**			class.
**	params		The list of service parameters for the Association
**			which is being negotiated.
**	singleXferSyntax	Character string listing the single
			transfer syntax to be supported.
**
** Return Values:
**
**	SRV_LISTFAILURE
**	SRV_NORMAL
**	SRV_PRESENTATIONCONTEXTERROR
**	SRV_PRESENTATIONCTXREJECTED
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
SRV_AcceptServiceClassWithOneXferSyntax(DUL_PRESENTATIONCONTEXT * requestedCtx,
		  DUL_SC_ROLE role, DUL_ASSOCIATESERVICEPARAMETERS * params,
		  const char* singleXferSyntax)
{
    int
        index;
    CTNBOOLEAN
	abstractFound = FALSE,
	transferFound = FALSE;

    CONDITION
	cond,
	rtnCond = SRV_NORMAL;
    DUL_PRESENTATIONCONTEXT
	* ctx;
    DUL_TRANSFERSYNTAX
	* transfer;

    if (params->acceptedPresentationContext == NULL) {
	params->acceptedPresentationContext = LST_Create();
	if (params->acceptedPresentationContext == NULL) {
	    return COND_PushCondition(SRV_LISTFAILURE,
		    SRV_Message(SRV_LISTFAILURE), "SRV_AcceptServiceClass");
	}
    }
    for (index = 0; index < (int) DIM_OF(syntaxList) && !abstractFound; index++) {
	if (strcmp(requestedCtx->abstractSyntax, syntaxList[index]) == 0) {
	    abstractFound = TRUE;
	}
    }
    if (abstractFound) {
#if 0
	char* xferSyntaxes[] =
	{ "1.2.840.10008.1.2.4.50",	/* JPEG Baseline Process 1 */
	"1.2.840.10008.1.2.4.51",	/* JPEG Extended Process 2 & 4 */
	"1.2.840.10008.1.2.1",		/* Explicit VR Little Endian */
	"1.2.840.10008.1.2.1.99",	/* Deflated Explicit VR Little Endian */
	"1.2.840.10008.1.2.2",		/* Explicit VR Big Endian Endian */
	DICOM_TRANSFERLITTLEENDIAN };
#endif

	transfer = matchProposedXferSyntax(requestedCtx,
			"ACCEPT/XFER", &singleXferSyntax, 1);
	if (transfer == NULL) {
	  cond = DUL_MakePresentationCtx(&ctx,
				       requestedCtx->proposedSCRole,
				       DUL_SC_ROLE_DEFAULT,
				       requestedCtx->presentationContextID,
				    DUL_PRESENTATION_REJECT_TRANSFER_SYNTAX,
				       requestedCtx->abstractSyntax,
	      DICOM_TRANSFERLITTLEENDIAN, DICOM_TRANSFERLITTLEENDIAN, NULL);
	  if (cond != DUL_NORMAL)
	    return COND_PushCondition(SRV_PRESENTATIONCONTEXTERROR,
				  SRV_Message(SRV_PRESENTATIONCONTEXTERROR),
				      "SRV_AcceptServiceClass");
	  (void) COND_PushCondition(SRV_UNSUPPORTEDSERVICE,
				  SRV_Message(SRV_UNSUPPORTEDSERVICE),
		    requestedCtx->abstractSyntax, "SRV_AcceptServiceClass");
	  rtnCond = COND_PushCondition(SRV_PRESENTATIONCTXREJECTED,
				   SRV_Message(SRV_PRESENTATIONCTXREJECTED),
		    requestedCtx->abstractSyntax, "SRV_AcceptServiceClass");
	} else {
	    cond = DUL_MakePresentationCtx(&ctx,
					 requestedCtx->proposedSCRole, role,
					requestedCtx->presentationContextID,
					   DUL_PRESENTATION_ACCEPT,
					   requestedCtx->abstractSyntax,
					   transfer->transferSyntax,
					   transfer->transferSyntax,
					   NULL);
	    if (cond != DUL_NORMAL)
		return COND_PushCondition(SRV_PRESENTATIONCONTEXTERROR,
				  SRV_Message(SRV_PRESENTATIONCONTEXTERROR),
					  "SRV_AcceptServiceClass");
	}
#if 0
	if ((transfer = LST_Head(&requestedCtx->proposedTransferSyntax)) == NULL)
	    return COND_PushCondition(SRV_LISTFAILURE,
		    SRV_Message(SRV_LISTFAILURE), "SRV_AcceptServiceClass");
	(void) LST_Position(&requestedCtx->proposedTransferSyntax, transfer);
	while (!transferFound && (transfer != NULL)) {
	    if (strcmp(transfer->transferSyntax, DICOM_TRANSFERLITTLEENDIAN) == 0)
		transferFound = TRUE;
	    else
		transfer = LST_Next(&requestedCtx->proposedTransferSyntax);
	}
	if (transferFound) {
	    cond = DUL_MakePresentationCtx(&ctx,
					 requestedCtx->proposedSCRole, role,
					requestedCtx->presentationContextID,
					   DUL_PRESENTATION_ACCEPT,
					   requestedCtx->abstractSyntax,
	      DICOM_TRANSFERLITTLEENDIAN, DICOM_TRANSFERLITTLEENDIAN, NULL);
	    if (cond != DUL_NORMAL)
		return COND_PushCondition(SRV_PRESENTATIONCONTEXTERROR,
				  SRV_Message(SRV_PRESENTATIONCONTEXTERROR),
					  "SRV_AcceptServiceClass");
	} else {
	    cond = DUL_MakePresentationCtx(&ctx,
					   requestedCtx->proposedSCRole,
					   DUL_SC_ROLE_DEFAULT,
					requestedCtx->presentationContextID,
				    DUL_PRESENTATION_REJECT_TRANSFER_SYNTAX,
					   requestedCtx->abstractSyntax,
	      DICOM_TRANSFERLITTLEENDIAN, DICOM_TRANSFERLITTLEENDIAN, NULL);
	    if (cond != DUL_NORMAL)
		return COND_PushCondition(SRV_PRESENTATIONCONTEXTERROR,
				  SRV_Message(SRV_PRESENTATIONCONTEXTERROR),
					  "SRV_AcceptServiceClass");

	    (void) COND_PushCondition(SRV_UNSUPPORTEDTRANSFERSYNTAX,
				 SRV_Message(SRV_UNSUPPORTEDTRANSFERSYNTAX),
		    requestedCtx->abstractSyntax, "SRV_AcceptServiceClass");
	    rtnCond = COND_PushCondition(SRV_PRESENTATIONCTXREJECTED,
				   SRV_Message(SRV_PRESENTATIONCTXREJECTED),
		    requestedCtx->abstractSyntax, "SRV_AcceptServiceClass");
	}
#endif
    } else {
	cond = DUL_MakePresentationCtx(&ctx,
				       requestedCtx->proposedSCRole,
				       DUL_SC_ROLE_DEFAULT,
				       requestedCtx->presentationContextID,
				    DUL_PRESENTATION_REJECT_ABSTRACT_SYNTAX,
				       requestedCtx->abstractSyntax,
	      DICOM_TRANSFERLITTLEENDIAN, DICOM_TRANSFERLITTLEENDIAN, NULL);
	if (cond != DUL_NORMAL)
	    return COND_PushCondition(SRV_PRESENTATIONCONTEXTERROR,
				  SRV_Message(SRV_PRESENTATIONCONTEXTERROR),
				      "SRV_AcceptServiceClass");
	(void) COND_PushCondition(SRV_UNSUPPORTEDSERVICE,
				  SRV_Message(SRV_UNSUPPORTEDSERVICE),
		    requestedCtx->abstractSyntax, "SRV_AcceptServiceClass");
	rtnCond = COND_PushCondition(SRV_PRESENTATIONCTXREJECTED,
				   SRV_Message(SRV_PRESENTATIONCTXREJECTED),
		    requestedCtx->abstractSyntax, "SRV_AcceptServiceClass");
    }
    cond = LST_Enqueue(&params->acceptedPresentationContext, ctx);
    if (cond != LST_NORMAL)
	return COND_PushCondition(SRV_LISTFAILURE, SRV_Message(SRV_LISTFAILURE),
				  "SRV_AcceptServiceClass");

/*    pdvList.count = 0; */

    return rtnCond;
}

CONDITION
SRV_AcceptSOPClass(DUL_PRESENTATIONCONTEXT * requestedCtx,
		   DUL_SC_ROLE role, DUL_ASSOCIATESERVICEPARAMETERS * params,
		   char** xferSyntaxes, int xferSyntaxCount,
		   int isStorageClass)
{
  CTNBOOLEAN
    abstractFound = FALSE,
    transferFound = FALSE;

  CONDITION
    cond,
    rtnCond = SRV_NORMAL;
  DUL_PRESENTATIONCONTEXT* ctx;
  DUL_TRANSFERSYNTAX* transfer;
  char* prefix = "ACCEPT/XFER";

  if (params->acceptedPresentationContext == NULL) {
    params->acceptedPresentationContext = LST_Create();
    if (params->acceptedPresentationContext == NULL) {
      return COND_PushCondition(SRV_LISTFAILURE,
		    SRV_Message(SRV_LISTFAILURE), "SRV_AcceptServiceClass");
    }
  }

  if (isStorageClass)
    prefix = "ACCEPT/XFER/STORAGE";

  transfer = matchProposedXferSyntax(requestedCtx,
		prefix, xferSyntaxes, xferSyntaxCount);
  if (transfer == NULL) {
    DUL_TRANSFERSYNTAX* proposedXfer;
    proposedXfer = (DUL_TRANSFERSYNTAX*)LST_Head(&requestedCtx->proposedTransferSyntax);

    cond = DUL_MakePresentationCtx(&ctx,
		requestedCtx->proposedSCRole,
		DUL_SC_ROLE_DEFAULT,
		requestedCtx->presentationContextID,
		DUL_PRESENTATION_REJECT_ABSTRACT_SYNTAX,
		requestedCtx->abstractSyntax,
		proposedXfer->transferSyntax,
		proposedXfer->transferSyntax,
		NULL);
    if (cond != DUL_NORMAL) {
      return COND_PushCondition(SRV_PRESENTATIONCONTEXTERROR,
				  SRV_Message(SRV_PRESENTATIONCONTEXTERROR),
				      "SRV_AcceptSOPClass");
    }
    (void) COND_PushCondition(SRV_UNSUPPORTEDSERVICE,
		SRV_Message(SRV_UNSUPPORTEDSERVICE),
		requestedCtx->abstractSyntax, "SRV_AcceptSOPClass");
    rtnCond = COND_PushCondition(SRV_PRESENTATIONCTXREJECTED,
		SRV_Message(SRV_PRESENTATIONCTXREJECTED),
		requestedCtx->abstractSyntax, "SRV_AcceptSOPClass");
  } else {
    cond = DUL_MakePresentationCtx(&ctx,
		requestedCtx->proposedSCRole, role,
		requestedCtx->presentationContextID,
		DUL_PRESENTATION_ACCEPT,
		requestedCtx->abstractSyntax,
		transfer->transferSyntax,
		transfer->transferSyntax,
		NULL);
    if (cond != DUL_NORMAL) {
      return COND_PushCondition(SRV_PRESENTATIONCONTEXTERROR,
				  SRV_Message(SRV_PRESENTATIONCONTEXTERROR),
					  "SRV_AcceptSOPClass");
    }
  }
  cond = LST_Enqueue(&params->acceptedPresentationContext, ctx);
  if (cond != LST_NORMAL)
    return COND_PushCondition(SRV_LISTFAILURE, SRV_Message(SRV_LISTFAILURE),
				  "SRV_AcceptSOPClass");
  return rtnCond;
}





CONDITION
SRV_AcceptSOPClassAsStorageClass(DUL_PRESENTATIONCONTEXT* requestedCtx,
		    DUL_SC_ROLE role, DUL_ASSOCIATESERVICEPARAMETERS* params,
		    char**xferSyntaxes, int xferSyntaxCount) {

  return 0;
}

/* SRV_RejectServiceClass
**
** Purpose:
**	Reject an SOP class proposed by a calling application.
**
** Parameter Dictionary:
**	requestedCtx	Pointerto requested Presentation Context which user
**			is rejecting.
**	result		One of the defined DUL results which provide reasons
**			for rejecting a Presentation Context.
**	params		The structure which contains parameters which defines
**			the association.
**
** Return Values:
**
**	SRV_LISTFAILURE
**	SRV_NORMAL
**	SRV_PRESENTATIONCONTEXTERROR
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
SRV_RejectServiceClass(DUL_PRESENTATIONCONTEXT * requestedCtx,
		       unsigned short result,
		       DUL_ASSOCIATESERVICEPARAMETERS * params)
{
    CONDITION
	cond;
    DUL_PRESENTATIONCONTEXT
	* ctx;

    if (params->acceptedPresentationContext == NULL) {
	params->acceptedPresentationContext = LST_Create();
	if (params->acceptedPresentationContext == NULL) {
	    return COND_PushCondition(SRV_LISTFAILURE,
		    SRV_Message(SRV_LISTFAILURE), "SRV_RejectServiceClass");
	}
    }
    cond = DUL_MakePresentationCtx(&ctx,
				   requestedCtx->proposedSCRole,
				   DUL_SC_ROLE_DEFAULT,
				   requestedCtx->presentationContextID,
				   (unsigned char)result,
				   requestedCtx->abstractSyntax,
				   DICOM_TRANSFERLITTLEENDIAN,
				   DICOM_TRANSFERLITTLEENDIAN, NULL);
    if (cond != DUL_NORMAL)
	return COND_PushCondition(SRV_PRESENTATIONCONTEXTERROR,
				  SRV_Message(SRV_PRESENTATIONCONTEXTERROR),
				  "SRV_RejectServiceClass");

    cond = LST_Enqueue(&params->acceptedPresentationContext, ctx);
    if (cond != LST_NORMAL)
	return COND_PushCondition(SRV_LISTFAILURE, SRV_Message(SRV_LISTFAILURE),
				  "SRV_RejectServiceClass");

    return SRV_NORMAL;
}

/* SRV_MessageIDOut
**
** Purpose:
**	Get a unique message ID which can be used in a DICOM command.
**
** Parameter Dictionary:
**	NONE
**
** Return Values:
**	Unique message ID.
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

unsigned short
SRV_MessageIDOut()
{
    static unsigned short
        messageID = 0;

#ifdef CTN_USE_THREADS
    if (THR_ObtainMutex(FAC_SRV) != THR_NORMAL) {
	COND_DumpConditions();
	fprintf(stderr, " SRV_MessageIDOut unable to obtain mutex, exiting\n");
	exit(1);
    }
#endif
    messageID++;

#ifdef CTN_USE_THREADS
    if (THR_ReleaseMutex(FAC_SRV) != THR_NORMAL) {
	COND_DumpConditions();
	fprintf(stderr, " SRV_MessageIDOut unable to release mutex, exiting\n");
	exit(1);
    }
#endif
    return messageID;
}

/* SRV_MessageIDIn
**
** Purpose:
**	Function to reclaim ID messages after they have been used.
**
** Parameter Dictionary:
**	messageID	ID to be reclaimed.
**
** Return Values:
**	NONE
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

void
SRV_MessageIDIn(unsigned short messageID)
{
}

/* SRV_Debug
**
** Purpose:
**	Set debug flag in this module and in the other modules.
**
** Parameter Dictionary:
**	flag	Boolean flag indicating if debug mode is to be ON or OFF.
**
** Return Values:
**	NONE
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

void
SRV_Debug(CTNBOOLEAN flag)
{
    PRVSRV_debug = flag;
}
