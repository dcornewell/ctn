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
** Intent:		Define prototypes for format functions.
** Last Update:		$Author: smm $, $Date: 1995-04-03 18:45:57 $
** Source File:		$RCSfile: format.h,v $
** Revision:		$Revision: 1.2 $
** Status:		$State: Exp $
*/

void formatPatientRecord(FIS_PATIENTRECORD * p, int index, char *buf);
int comparePatientRecord(FIS_PATIENTRECORD * p1, FIS_PATIENTRECORD * p2);
void formatStudyRecord(FIS_STUDYRECORD * s, int index, char *buf);
int compareStudyRecord(FIS_STUDYRECORD * s1, FIS_STUDYRECORD * s2);
void formatPatientStudy(RSA_PATIENTSTUDY * ps, int index, char *buf);
int comparePatientStudy(RSA_PATIENTSTUDY * ps1, RSA_PATIENTSTUDY * ps2);
