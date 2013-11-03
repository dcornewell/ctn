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
**				DINPACS 97
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	Steve Moore
** Intent:		Define typedefs and function prototypes for
**			send_study program.
** Last Update:		$Author: smm $, $Date: 1997-05-14 15:47:04 $
** Source File:		$RCSfile: clone_study.h,v $
** Revision:		$Revision: 1.1 $
** Status:		$State: Exp $
*/

#ifndef _APP_IS_IN
#define _APP_IS_IN 1

#define	K_APPLICATION_ENTITY	0
#define	K_GROUP_NAMES		1
#define	K_DATABASE_ACCESS	2


#define	APP_NORMAL	FORM_COND(FAC_APP, SEV_SUCC, 1)

/*
#define	APP_ILLEGALSERVICEPARAMETER	FORM_COND(FAC_APP, SEV_ERROR, 2)
#define	APP_MISSINGSERVICEPARAMETER	FORM_COND(FAC_APP, SEV_ERROR, 3)
#define	APP_PARAMETERWARNINGS		FORM_COND(FAC_APP, SEV_WARN,  4)
#define	APP_PARAMETERFAILURE		FORM_COND(FAC_APP, SEV_ERROR, 5)
#define	APP_FAILURE			FORM_COND(FAC_APP, SEV_ERROR, 6)
#define	APP_ASSOCIATIONRQFAILED		FORM_COND(FAC_APP, SEV_WARN, 7)
#define	APP_IMAGEPARSEFAILED		FORM_COND(FAC_APP, SEV_ERROR, 8)
#define	APP_GENERALMSG			FORM_COND(FAC_APP, SEV_WARN, 9)
*/

#define APP_ERROR(a) (a), APP_Message((a))

#endif
