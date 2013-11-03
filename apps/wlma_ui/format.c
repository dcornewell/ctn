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
** Module Name(s):
** Author, Date:	Steve Moore, Summer 1994
** Intent:		This file contains functions which are used to format
**			records to be presented to users.  Each function is
**			called with an input record and an index number (0-...).
**			The output is an ASCIZ string that will be presented
**			to a user, to allow the user to select one or more
**			entries from a list.
** Last Update:		$Author: smm $, $Date: 1995-12-22 17:27:44 $
** Source File:		$RCSfile: format.c,v $
** Revision:		$Revision: 1.7 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.7 $ $RCSfile: format.c,v $";

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
#include "utility.h"

#include "wlma.h"
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

    (void) sprintf(buf, "%-16s %-16s %.4s-%.2s-%.2s %-16s %-16s",
		   s->AccNum,
		   s->StuID,
		   s->SchStuStaDat,
		   &s->SchStuStaDat[4],
		   &s->SchStuStaDat[6],
		   s->SchStuLoc,
		   s->ProDes);
}

int
compareStudyRecord(FIS_STUDYRECORD * s1, FIS_STUDYRECORD * s2)
{
    return 1;
}

void
formatPatientStudy(WLMA_PATIENTSTUDY * ps, int index, char *buf)
{
    (void) sprintf(buf, "%-30s %-16s %.4s-%.2s-%.2s %-20s %-16s %-16s",
		   ps->Patient.PatNam,
		   ps->Study.StuID,
		   ps->Study.SchStuStaDat,
		   &ps->Study.SchStuStaDat[4],
		   &ps->Study.SchStuStaDat[6],
		   ps->Study.SchStuLoc,
		   ps->Study.ProDes,
		   ps->Study.StuStaID);

    while (*buf != '\0') {
	if (*buf == '^')
	    *buf++ = ' ';
	else
	    buf++;
    }
}
void
formatPatientStudyScheduled(WLMA_PATIENTSTUDY * ps, int index, char *buf)
{
    (void) sprintf(buf, "%-30s %-16s %.4s-%.2s-%.2s %.2s:%.2s %-20s %-16s",
		   ps->Patient.PatNam,
		   ps->Study.StuID,
		   ps->Study.SchStuStaDat,
		   &ps->Study.SchStuStaDat[4],
		   &ps->Study.SchStuStaDat[6],
		   ps->Study.SchStuStaTim,
		   &ps->Study.SchStuStaTim[2],
		   ps->Study.SchStuLoc,
		   ps->Study.ProDes);

    while (*buf != '\0') {
	if (*buf == '^')
	    *buf++ = ' ';
	else
	    buf++;
    }
}

int
comparePatientStudyScheduled(WLMA_PATIENTSTUDY * ps1, WLMA_PATIENTSTUDY * ps2)
{
    long date1,
        date2;
    double time1,
        time2;

    date1 = UTL_ConvertDatetoLong(ps1->Study.SchStuStaDat);
    date2 = UTL_ConvertDatetoLong(ps2->Study.SchStuStaDat);
    if (date1 < date2)
	return -1;
    else if (date1 > date2)
	return 1;

    time1 = UTL_ConvertTimetoFloat(ps1->Study.SchStuStaTim);
    time2 = UTL_ConvertTimetoFloat(ps2->Study.SchStuStaTim);
    if (time1 < time2)
	return -1;
    else
	return 1;
}


void
formatStudyComponent(FIS_STUDYCOMPONENTRECORD * s, int index, char *buf)
{
    (void) sprintf(buf, "%-16s %-4s %-20s %-12s",
		   s->StuID,
		   s->Mod,
		   s->StuDes,
		   s->StuComStaID);
}

void
formatStudyComponentSCSeries(WLMA_STUDYCOMPONENTSCSERIES * p,
			     int index, char *buf)
{
    char
        dat[DICOM_DA_LENGTH + 1] = "",
        tim[DICOM_TM_LENGTH + 1] = "",
        aeTitle[DICOM_AE_LENGTH + 1] = "";
    int
        count = 0;

    if (p->SCSeries.Flag & FIS_K_SCSERIES_SERDAT)
	strcpy(dat, p->SCSeries.SerDat);
    if (p->SCSeries.Flag & FIS_K_SCSERIES_SERTIM)
	strcpy(dat, p->SCSeries.SerTim);
    if (p->SCSeries.Flag & FIS_K_SCSERIES_RETAETIT)
	strcpy(aeTitle, p->SCSeries.RetAETit);

    if (p->SCSeries.ImageList != NULL) {
	count = LST_Count(&p->SCSeries.ImageList);
    }
    (void) sprintf(buf, "%-16s %-4s %20s %-12s %-10s %-12s %-16s Images: %4d",
		   p->StudyComponent.StuID,
		   p->StudyComponent.Mod,
		   p->StudyComponent.StuDes,
		   p->StudyComponent.StuComStaID,
		   dat,
		   tim,
		   aeTitle,
		   count);
}
