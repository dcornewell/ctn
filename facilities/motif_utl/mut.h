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
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	include file
** Author, Date:	Steve Moore, Summer 1994
** Intent:		Provide prototypes and constants for MUT facility.
** Last Update:		$Author: smm $, $Date: 1996-08-23 20:00:55 $
** Source File:		$RCSfile: mut.h,v $
** Revision:		$Revision: 1.8 $
** Status:		$State: Exp $
*/

#ifdef  __cplusplus
extern "C" {
#endif

typedef enum {
    MUT_TEXT, MUT_INT, MUT_FLOAT, MUT_US
}   MUT_DATATYPE;

void MUT_LoadList(Widget w, LST_HEAD * lst, void (*format) (), char *buf);
CONDITION MUT_ScanWidget(Widget w, MUT_DATATYPE type, CTNBOOLEAN * nullFlag, void *d);
CONDITION MUT_DisplayConditionText(Widget w);

#define	MUT_NORMAL		/* Normal return from MUT package */ \
	FORM_COND(FAC_MUT, SEV_SUCC, 1)
#define	MUT_SCANFAILURE		/* Failed to scan text from a widget */ \
	FORM_COND(FAC_MUT, SEV_ERROR, 2)

#ifdef  __cplusplus
}
#endif
