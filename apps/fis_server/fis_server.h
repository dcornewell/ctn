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
**				DICOM 94
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	Steve Moore
** Intent:		Define typedefs and function prototypes for
**			TBL facility (for handling table operations).
** Last Update:		$Author: smm $, $Date: 1995-04-10 16:36:03 $
** Source File:		$RCSfile: fis_server.h,v $
** Revision:		$Revision: 1.4 $
** Status:		$State: Exp $
*/

#ifndef _APP_IS_IN
#define _APP_IS_IN 1

typedef struct {
    char callingAPTitle[DUL_LEN_TITLE + 1];
    char calledAPTitle[DUL_LEN_TITLE + 1];
    int pid;
}   PROCESS_ELEMENT;

#define	K_APPLICATION_ENTITY	0
#define	K_GROUP_NAMES		1
#define	K_DATABASE_ACCESS	2

#define	MAXIMUM_TABLES 10

#define	SIMPLE_STORAGE_AE_TITLE	"DICOM_STORAGE"
CONDITION
associationCheck(char *node, DUL_ASSOCIATESERVICEPARAMETERS * params,
    DMAN_HANDLE ** handle, CTNBOOLEAN forgive, DUL_ABORTITEMS * abortItems);
char *APP_Message(CONDITION cond);
CONDITION
serviceRequests(DUL_NETWORKKEY ** network, DUL_ASSOCIATIONKEY ** association,
		DUL_ASSOCIATESERVICEPARAMETERS * service);
CONDITION
nextAssociationRequest(char *node, DUL_NETWORKKEY ** network,
	     DUL_ASSOCIATESERVICEPARAMETERS * service, unsigned long maxPDU,
		 CTNBOOLEAN forgiveFlag, DUL_ASSOCIATIONKEY ** association);

#define	APP_NORMAL	FORM_COND(FAC_APP, SEV_SUCC, 1)
#define	APP_ILLEGALSERVICEPARAMETER	FORM_COND(FAC_APP, SEV_ERROR, 2)
#define	APP_MISSINGSERVICEPARAMETER	FORM_COND(FAC_APP, SEV_ERROR, 3)
#define	APP_PARAMETERWARNINGS		FORM_COND(FAC_APP, SEV_WARN,  4)
#define	APP_PARAMETERFAILURE		FORM_COND(FAC_APP, SEV_ERROR, 5)
#define	APP_FAILURE			FORM_COND(FAC_APP, SEV_ERROR, 6)
#define	APP_ASSOCIATIONRQFAILED		FORM_COND(FAC_APP, SEV_WARN, 7)

#define APP_ERROR(a) (a), APP_Message((a))

#endif
