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
** Author, Date:	Stephen Moore, 12-Jul-94
** Intent:		This file contains private definitions for the
**			FIS facility.
** Last Update:		$Author: smm $, $Date: 1998-10-22 17:47:28 $
** Source File:		$Source: /home/smm/ctn/ctn/facilities/irs/irs_private.h,v $
** Revision:		$Revision: 1.1 $
** Status:		$State: Exp $
*/

#ifndef _IRS_PRIVATE_IS_IN
#define _IRS_PRIVATE_IS_IN 1

#ifdef  __cplusplus
extern "C" {
#endif

#define	IRS_COND(a) (a), IRS_Message((a))

typedef struct {
    TBL_HANDLE *workQueueHandle;
}   PRIVATE_HANDLE;

typedef struct {
    char *FieldName;
    long Flag;
}   FLAG_MAP;

char *IRS_Message(CONDITION cond);

#ifdef  __cplusplus
}
#endif

#endif
