/*
          Copyright (C) 1993, RSNA and Washington University

          The software and supporting documentation for the Radiological
          Society of North America (RSNA) 1993 Digital Imaging and
          Communications in Medicine (DICOM) Demonstration were developed
          at the
                  Electronic Radiology Laboratory
                  Mallinckrodt Institute of Radiology
                  Washington University School of Medicine
                  510 S. Kingshighway Blvd.
                  St. Louis, MO 63110
          as part of the 1993 DICOM Central Test Node project for, and
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
/*
** @$=@$=@$=
*/
/*
**				DICOM 93
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:
** Intent:		Defines return values for application success and
**			failure.
** Last Update:		$Author: chander $, $Date: 1994-10-10 20:41:15 $
** Source File:		$RCSfile: print_client_app.h,v $
** Revision:		$Revision: 1.1 $
** Status:		$State: Exp $
*/

#ifndef PRINT_CLIENT_APPS
#define PRINT_CLIENT_APPS 1

#define	APP_NORMAL		FORM_COND(FAC_APP, SEV_SUCC, 1)
#define	APP_FAILED		FORM_COND(FAC_APP, SEV_ERROR, 2)

#endif
