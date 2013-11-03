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
** Module Name(s):	openTables
**			expandPatientStudy
**			clearPatientStudy
**			expandStudyComponentSCSeries
**			loadDatabase
** Author, Date:	Steve Moore, Summer 1994
** Intent:		This file contains database support functions for
**			the wlma_ui application.  These functions open
**			the database and provide some record manipulation
**			which combines some records to give the user a
**			simpler view of the data.
** Last Update:		$Author: smm $, $Date: 1995-12-22 17:27:49 $
** Source File:		$RCSfile: support.c,v $
** Revision:		$Revision: 1.8 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.8 $ $RCSfile: support.c,v $";

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "dicom_objects.h"
#include "tbl.h"
#include "manage.h"
#include "fis.h"

#include "wlma.h"

static void clearPatientStudy(LST_HEAD * list);

CONDITION
openTables(char *databaseName, char *application, DMAN_HANDLE ** databaseHandle,
	   FIS_HANDLE ** FISHandle)
{
    CONDITION cond;
    DMAN_FISACCESS
	s,
	criteria;

    cond = DMAN_Open(databaseName, "", "", databaseHandle);
    if (cond != DMAN_NORMAL)
	return 0;

    cond = DMAN_LookupFISAccess(databaseHandle, application, &s);
    if (cond != DMAN_NORMAL)
	return 0;

    cond = FIS_Open(s.DbKey, FISHandle);
    if (cond != FIS_NORMAL)
	return 0;

    return 1;
}

void
expandPatientStudy(LST_HEAD * patientList, LST_HEAD * patientStudyList)
{
    FIS_PATIENTRECORD
    * p;
    FIS_STUDYRECORD
	* s;
    WLMA_PATIENTSTUDY
	* ps;

    clearPatientStudy(patientStudyList);
    p = LST_Head(&patientList);
    if (p != NULL)
	(void) LST_Position(&patientList, p);
    while (p != NULL) {
	s = LST_Head(&p->StudyList);
	if (s != NULL)
	    (void) LST_Position(&p->StudyList, s);
	while (s != NULL) {
	    ps = malloc(sizeof(*ps));
	    ps->Patient = *p;
	    ps->Study = *s;
	    (void) LST_Enqueue(&patientStudyList, ps);
	    s = LST_Next(&p->StudyList);
	}
	p = LST_Next(&patientList);
    }
}

static void
clearPatientStudy(LST_HEAD * list)
{
    WLMA_PATIENTSTUDY
    * ps;

    while ((ps = LST_Pop(&list)) != NULL)
	free(ps);
}

void
expandStudyComponentSCSeries(LST_HEAD * scList, LST_HEAD * scSeriesList)
{
    FIS_STUDYCOMPONENTRECORD *p;
    FIS_SCSERIESRECORD *s;
    WLMA_STUDYCOMPONENTSCSERIES *ps;

    clearPatientStudy(scSeriesList);
    p = LST_Head(&scList);
    if (p != NULL)
	(void) LST_Position(&scList, p);
    while (p != NULL) {
	s = LST_Head(&p->SeriesList);
	if (s != NULL)
	    (void) LST_Position(&p->SeriesList, s);
	while (s != NULL) {
	    ps = malloc(sizeof(*ps));
	    ps->StudyComponent = *p;
	    ps->SCSeries = *s;
	    (void) LST_Enqueue(&scSeriesList, ps);
	    s = LST_Next(&p->SeriesList);
	}
	p = LST_Next(&scList);
    }
}

CONDITION
loadDatabase(FIS_HANDLE ** handle)
{
    static FIS_PATIENTRECORD p[] = {
	{0, 0, FIS_K_PATIENT, 0, "", "Moore^Charlie^J", "", "19931020", "M", NULL, NULL, NULL},
	{0, 0, FIS_K_PATIENT, 0, "", "Fitzsimmons^Teresa^A", "", "19581216", "F", NULL, NULL, NULL},
	{0, 0, FIS_K_PATIENT, 0, "", "Barnholtz^Theodore^J", "", "19220101", "M", NULL, NULL, NULL},
	{0, 0, FIS_K_PATIENT, 0, "", "Simpson^Erin", "", "18900704", "F", NULL, NULL, NULL},
	{0, 0, FIS_K_PATIENT, 0, "", "Myers^Lisa", "", "19631006", "F", NULL, NULL, NULL}
    };
    static FIS_STUDYRECORD s[] = {
	{0, 0, FIS_K_STUDY, 0, "", "", "", "", "", "19941127", "120000", "McCormick-East",
	"Head CT", "", "SCHEDULED", "", "", "", NULL, NULL},
	{0, 0, FIS_K_STUDY, 0, "", "", "", "", "", "19941127", "130000", "McCormick-North",
	"Knee MR", "", "SCHEDULED", "", "", "", NULL, NULL},
	{0, 0, FIS_K_STUDY, 0, "", "", "", "", "", "19941127", "140000", "McCormick-Admitting",
	"Chest XRay", "", "SCHEDULED", "", "", "", NULL, NULL},
	{0, 0, FIS_K_STUDY, 0, "", "", "", "", "", "19941127", "150000", "McCormick-East",
	"Thalium Scan", "", "SCHEDULED", "", "", "", NULL, NULL},
	{0, 0, FIS_K_STUDY, 0, "", "", "", "", "", "19941127", "160000", "McCormick-East",
	"Fetal US", "", "SCHEDULED", "", "", "", NULL, NULL},

    };
    CONDITION cond;
    int i;

    for (i = 0; i < (int) DIM_OF(p); i++) {
	cond = FIS_NewRecord(handle, FIS_K_PATIENT, &p[i]);
	if (cond != FIS_NORMAL)
	    return 0;
	cond = FIS_NewRecord(handle, FIS_K_STUDY, &s[i]);
	if (cond != FIS_NORMAL)
	    return 0;

	p[i].Flag = FIS_K_PAT_PATID | FIS_K_PAT_PATNAM | FIS_K_PAT_PATUID |
	    FIS_K_PAT_BIRDAT | FIS_K_PAT_SEX;
	cond = FIS_Insert(handle, FIS_K_PATIENT, &p[i]);
	if (cond != FIS_NORMAL)
	    return 0;

	strcpy(s[i].PatUID, p[i].PatUID);
	s[i].Flag = FIS_K_STU_PATUID | FIS_K_STU_STUINSUID |
	    FIS_K_STU_VISUID | FIS_K_STU_ACCNUM |
	    FIS_K_STU_STUID | FIS_K_STU_SCHSTUSTADAT |
	    FIS_K_STU_SCHSTUSTATIM | FIS_K_STU_SCHSTULOC |
	    FIS_K_STU_PRODES | FIS_K_STU_STUSTAID;
	cond = FIS_Insert(handle, FIS_K_STUDY, &s[i]);
	if (cond != FIS_NORMAL)
	    return 0;
    }

    return 1;
}
