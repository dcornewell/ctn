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
** Last Update:		$Author: smm $, $Date: 1995-12-15 21:12:19 $
** Source File:		$RCSfile: services.h,v $
** Revision:		$Revision: 1.3 $
** Status:		$State: Exp $
*/

#ifndef _SERVICES_IS_IN
#define _SERVICES_IS_IN 1

CONDITION
ngetRequest(DUL_ASSOCIATESERVICEPARAMETERS * params,
	    DUL_ASSOCIATIONKEY ** association, DMAN_HANDLE ** dman,
	    DUL_PRESENTATIONCONTEXT * ctx, MSG_N_GET_REQ ** request,
	    FIS_HANDLE ** fis);
CONDITION
nsetRequest(DUL_ASSOCIATESERVICEPARAMETERS * params,
	    DUL_ASSOCIATIONKEY ** association, DMAN_HANDLE ** dman,
	    DUL_PRESENTATIONCONTEXT * ctx, MSG_N_SET_REQ ** request,
	    FIS_HANDLE ** fis);
CONDITION
ncreateRequest(DUL_ASSOCIATESERVICEPARAMETERS * params,
	       DUL_ASSOCIATIONKEY ** association, DMAN_HANDLE ** dman,
	       DUL_PRESENTATIONCONTEXT * ctx, MSG_N_CREATE_REQ ** request,
	       FIS_HANDLE ** fis);

#endif
