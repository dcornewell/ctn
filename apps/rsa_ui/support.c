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
**			clearPatientStudy
**			expandPatientStudy
**			clearPatientStudy
**			loadDatabase
**			distributeEvent
** Author, Date:	Steve Moore, Summer 1994
** Intent:		This file contains database support functions for
**			the rsa_ui application.  These functions open the
**			database and provide some record merging so that
**			the user is presented with a collapsed view of the
**			data in the database.
** Last Update:		$Author: smm $, $Date: 1995-04-03 18:46:11 $
** Source File:		$RCSfile: support.c,v $
** Revision:		$Revision: 1.2 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.2 $ $RCSfile: support.c,v $";

#include <stdio.h>
#include <stdlib.h>

#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "dicom_objects.h"
#include "tbl.h"
#include "manage.h"
#include "fis.h"

#include "rsa.h"

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
    RSA_PATIENTSTUDY
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
    RSA_PATIENTSTUDY
    * ps;

    while ((ps = LST_Pop(&list)) != NULL)
	free(ps);
}

CONDITION
loadDatabase(FIS_HANDLE ** handle)
{
    printf("load\n");
    return 1;
}

CONDITION
distributeEvent(FIS_HANDLE ** fis, DMAN_HANDLE ** dman,
       FIS_DATA_TYPE type, void *d, FIS_EVENT event, char *localApplication,
		char *dst)
{
    CONDITION cond;

    if (dst == NULL)
	dst = "";

    cond = FIS_SendEvent(fis, dman, type, d, event, localApplication,
			 dst);
    if (cond != FIS_NORMAL)
	return 0;
    else
	return 1;
}
