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
** Module Name(s):	formatPatientRecord
** Author, Date:	Steve Moore, Summer 1994
** Intent:		This file contains a number of subroutines that format
**			database records for printing in a list (or for
**			printing any place for that matter).  Each function
**			receives a structure and index value (0 ... N-1)
**			as input.  The output is expected to be an ASCIZ string
**			that can be presented to a user to allow the user
**			to identify (select) one or more records.
** Last Update:		$Author: smm $, $Date: 1995-12-21 20:45:56 $
** Source File:		$RCSfile: format.c,v $
** Revision:		$Revision: 1.4 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.4 $ $RCSfile: format.c,v $";

#include <stdio.h>
#include <string.h>
#ifdef GCCSUNOS
#include <stddef.h>
#endif

#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "dicom_objects.h"
#include "tbl.h"
#include "manage.h"
#include "lst.h"
#include "fis.h"

#include "rsa.h"
#include "format.h"

void
formatPatientRecord(FIS_PATIENTRECORD * p, int index, char *buf)
{

    (void) sprintf(buf, "%-16s %-30s", p->PatID, p->PatNam);
    while (*buf != '\0') {
	if (*buf == '^')
	    *buf++ = ' ';
	else
	    buf++;
    }
}

int
comparePatientRecord(FIS_PATIENTRECORD * p1, FIS_PATIENTRECORD * p2)
{
    return 1;
}

void
formatStudyRecord(FIS_STUDYRECORD * s, int index, char *buf)
{

    (void) sprintf(buf, "%-16s %-16s %-8s %-16s %-16s",
	     s->AccNum, s->StuID, s->SchStuStaDat, s->SchStuLoc, s->ProDes);
}

int
compareStudyRecord(FIS_STUDYRECORD * s1, FIS_STUDYRECORD * s2)
{
    return 1;
}

void
formatPatientStudy(RSA_PATIENTSTUDY * ps, int index, char *buf)
{
    int month,
        day,
        year;
    char *tbl[] = {"???", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    if (sscanf(ps->Study.SchStuStaDat, "%4d%2d%2d", &year, &month, &day) == 3) {
	if (month < 1)
	    month = 1;
	else if (month > 12)
	    month = 12;
	(void) sprintf(buf, "%-30s %-16s %2d-%-3s-%04d",
		       ps->Patient.PatNam, ps->Study.AccNum,
		       day, tbl[month], year);
    } else {
	(void) sprintf(buf, "%-30s %-16s %-11s ",
		       ps->Patient.PatNam, ps->Study.AccNum,
		       ps->Study.SchStuStaDat);
    }

    if (ps->Study.Flag & FIS_K_STU_PRODES)
	strcat(buf, ps->Study.ProDes);

    while (*buf != '\0') {
	if (*buf == '^')
	    *buf++ = ' ';
	else
	    buf++;
    }
}
