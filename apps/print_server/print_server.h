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
** Module Name(s):	print_server.h
** Author, Date:	Aniruddha S. Gokhale, 23-June-94
** Intent:		Defines prototypes of functions and conditions
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:46:21 $
** Source File:		$RCSfile: print_server.h,v $
** Revision:		$Revision: 1.28 $
** Status:		$State: Exp $
*/

#ifndef _APP_IS_IN
#define _APP_IS_IN 1

#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#ifndef _MSC_VER
#include <sys/param.h>
/*lint -e46*/
#include <sys/wait.h>
/*lint +e46*/
#endif
#ifdef SOLARIS
#include <netdb.h>
#endif
#ifdef _MSC_VER
#include <winsock.h>
#endif

#include <stdlib.h>
#include <signal.h>
#include <string.h>
#ifdef MACH
#include <unistd.h>
#endif
#include <stdarg.h>
#endif

#include "dicom.h"
#include "ctnthread.h"
#include "condition.h"
#include "lst.h"
#include "dicom_uids.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "dicom_sq.h"
#include "gq.h"
#include "dicom_print.h"
#include "print_queue.h"
#include "tbl.h"
#include "manage.h"

typedef struct {
    void *reserved[2];
    char callingAPTitle[DUL_LEN_TITLE + 1];
    char calledAPTitle[DUL_LEN_TITLE + 1];
    int pid;
}   PROCESS_ELEMENT;

typedef struct {
    unsigned long flag;
    char *listName;
    void *valueList;
    int listCount;
}   SUPPORTEDATTRIBUTES;

#define MAXIMAGES	8
typedef enum {
    STANDARD_FORMAT,
    ROW_FORMAT,
    COL_FORMAT
}   DISPLAYFORMATTYPES;

typedef struct {
    DISPLAYFORMATTYPES type;	/* type of image display format */
    union {
	struct {		/* The STANDARD format */
	    int numRows;
	    int numColumns;
	}   standard;
	struct {		/* The ROW format */
	    int numRows;
	    int imagesPerRow[MAXIMAGES];
	}   row;
	struct {		/* The COL format */
	    int numColumns;
	    int imagesPerColumn[MAXIMAGES];
	}   col;
    }   format;
}   APP_BFBIMAGEDISPLAYFORMAT;

/* Define all possible application specific error codes */

#define	APP_NORMAL			FORM_COND(FAC_APP, SEV_SUCC, 1)
#define	APP_ILLEGALSERVICEPARAMETER	FORM_COND(FAC_APP, SEV_ERROR, 2)
#define	APP_MISSINGSERVICEPARAMETER	FORM_COND(FAC_APP, SEV_ERROR, 3)
#define	APP_PARAMETERWARNINGS		FORM_COND(FAC_APP, SEV_WARN,  4)
#define	APP_PARAMETERFAILURE		FORM_COND(FAC_APP, SEV_ERROR, 5)
#define	APP_FAILURE			FORM_COND(FAC_APP, SEV_ERROR, 6)
#define	APP_ASSOCIATIONRQFAILED		FORM_COND(FAC_APP, SEV_WARN, 7)
#define APP_LISTFAILURE			FORM_COND(FAC_APP, SEV_ERROR, 8)
#define APP_MALLOCFAILURE		FORM_COND(FAC_APP, SEV_ERROR, 9)
#define APP_GQOPERATIONFAILED		FORM_COND(FAC_APP, SEV_ERROR, 10)
#define APP_OPERATIONINTERRUPTED	FORM_COND(FAC_APP, SEV_ERROR, 11)
#define APP_MISSINGREQUIREDATTRIBUTE	FORM_COND(FAC_APP, SEV_ERROR, 12)
#define APP_UNSUPPORTEDMANDATORYATTRIBUTE FORM_COND(FAC_APP, SEV_ERROR, 13)
#define APP_UNSUPPORTEDOPTIONALATTRIBUTE  FORM_COND(FAC_APP, SEV_ERROR, 14)
#define APP_UNSUPPORTEDATTRIBUTEVALUE	FORM_COND(FAC_APP, SEV_ERROR, 15)
#define APP_MISSINGIMAGES		FORM_COND(FAC_APP, SEV_ERROR, 16)
#define APP_MISSINGFILMBOX		FORM_COND(FAC_APP, SEV_ERROR, 17)
#define APP_NOSUCHINSTANCE		FORM_COND(FAC_APP, SEV_ERROR, 18)

char *APP_Message(CONDITION cond);
#define APP_ERROR(a) (a), APP_Message((a))

/* define a structure that keeps track of all those handles that need to be
   freed on normal or abnormal termination of the application
*/
typedef struct {
    int gqID;
    DUL_NETWORKKEY *network;
    DUL_ASSOCIATIONKEY *association;
    DUL_ASSOCIATESERVICEPARAMETERS *service;
    LST_HEAD *processList;
    /* DMAN_HANDLE *dmanHandle; */
}   PRINT_SERVER_HANDLES;

CONDITION
serviceRequests(DUL_NETWORKKEY ** network,
		DUL_ASSOCIATIONKEY ** association,
		DUL_ASSOCIATESERVICEPARAMETERS * service);
CONDITION
nextAssociationRequest(char *node, DUL_NETWORKKEY ** network,
		       DUL_ASSOCIATESERVICEPARAMETERS * service,
		       unsigned long maxPDU, CTNBOOLEAN forgiveFlag,
		       int *gqID,
		       DUL_ASSOCIATIONKEY ** association);
CONDITION
getRequest(DUL_ASSOCIATIONKEY ** association,
	   DUL_PRESENTATIONCONTEXT * ctx,
	   MSG_N_GET_REQ ** request);
CONDITION
createRequest(DUL_ASSOCIATIONKEY ** association,
	      DUL_PRESENTATIONCONTEXT * ctx,
	      MSG_N_CREATE_REQ ** request, CTNBOOLEAN * sendBack);
CONDITION
setRequest(DUL_ASSOCIATIONKEY ** association,
	   DUL_PRESENTATIONCONTEXT * ctx,
	   MSG_N_SET_REQ ** request,
	   CTNBOOLEAN * sendBack);
CONDITION
actionRequest(DUL_ASSOCIATIONKEY ** association,
	      DUL_PRESENTATIONCONTEXT * ctx,
	      MSG_N_ACTION_REQ ** request,
	      CTNBOOLEAN * sendBack);
CONDITION
deleteRequest(DUL_ASSOCIATIONKEY ** association,
	      DUL_PRESENTATIONCONTEXT * ctx,
	      MSG_N_DELETE_REQ ** request);
CONDITION
kill_queue(int qID);

/* utilities (support) routines */
CTNBOOLEAN
decodeImageDisplayFormat(char *, APP_BFBIMAGEDISPLAYFORMAT *);
char
   *getNewFileName(void);
short
    subset(unsigned long flag, unsigned short superflag);
CONDITION
deleteHierarchy(char *classUID);
void
    showHierarchy();
void
    updateFlagName(char **,...);
void
    updateOnProcessingFailure(void **response, unsigned long id, char *,...);
void
    updateAttributeIdentifierList(DCM_TAG tag, void **response);
CONDITION
supportedAttributes(MSG_TYPE, void *, SUPPORTEDATTRIBUTES *, int, char **,
		    void **);
#endif
