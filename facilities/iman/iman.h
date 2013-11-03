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
** Author, Date:	Steve Moore, April 1994
** Intent:		Define typedefs and function prototypes for
**			IMAN facility (for image management functions).
** Last Update:		$Author: smm $, $Date: 1996-08-23 19:40:32 $
** Source File:		$RCSfile: iman.h,v $
** Revision:		$Revision: 1.4 $
** Status:		$State: Exp $
*/


#ifndef _IMAN_IS_IN
#define _IMAN_IS_IN 1

#ifdef  __cplusplus
extern "C" {
#endif

CONDITION
IMAN_RequestTemp(TBL_HANDLE ** tblHandle, char *srcApp, char *destApp,
		 char *SOPClass, char *fileName, size_t fileLength);
CONDITION
IMAN_StoreImage(IDB_HANDLE ** idbHandle, DCM_OBJECT ** obj,
		char *tempFile);

/* Define condition values */

#define	IMAN_NORMAL		FORM_COND(FAC_IMAN, SEV_SUCC, 1)

#ifdef  __cplusplus
}
#endif

#endif
