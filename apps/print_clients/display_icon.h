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
** @$=@$=@$=
*/
/*
**				DICOM 93
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	Tze Khiong Lee
** Intent:		Header file for displaying icons
** Last Update:		$Author: smm $, $Date: 1995-02-22 05:17:58 $
** Source File:		$RCSfile: display_icon.h,v $
** Revision:		$Revision: 1.5 $
** Status:		$State: Exp $
*/

#define DICOMXDISP_NORMAL       FORM_COND (FAC_APP, SEV_SUCC,  1)
#define DICOMXDISP_NOXWINDOWS   FORM_COND (FAC_APP, SEV_FATAL, 2)
#define DICOMXDISP_NOCOLORS     FORM_COND (FAC_APP, SEV_FATAL, 3)
#define DICOMXDISP_NOSERVER     FORM_COND (FAC_APP, SEV_FATAL, 4)
#define DICOMXDISP_NOFONT       FORM_COND (FAC_APP, SEV_WARN,  5)
#define DICOMXDISP_NOGC         FORM_COND (FAC_APP, SEV_FATAL, 6)
#define DICOMXDISP_NOGRAPHICS   FORM_COND (FAC_APP, SEV_FATAL, 7)
#define DICOMXDISP_CMDLINE      FORM_COND (FAC_APP, SEV_FATAL, 8)
#define DICOMXDISP              FORM_COND (FAC_APP, SEV_INFORM, 8)

Widget
CreatePreviewStudyWindow();
Widget
CreateStandardBFBWindow();
Boolean
PopupStandardBFBWindow(int row, int col);

Boolean
PreviewStudy(char *index_file, char *icon_file, char *accessionNumber);
