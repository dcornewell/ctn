/*
          Copyright (C) 1993 - 1995, RSNA and Washington University

          The software and supporting documentation for the Radiological
          Society of North America (RSNA) 1993, 1994 Digital Imaging and
          Communications in Medicine (DICOM) Demonstration were developed
          at the
                  Electronic Radiology Laboratory
                  Mallinckrodt Institute of Radiology
                  Washington University School of Medicine
                  510 S. Kingshighway Blvd.
                  St. Louis, MO 63110
          as part of the 1993 - 1995 DICOM Central Test Node project for, and
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
+-+-+-+-+-+-+-+-+-
*/
/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	work
** Author, Date:	Stephen Moore, 11-Feb-94
** Intent:		This module contains the functions which read
**			information from the standard input and update
**			the Results and Interpretation reocrds to be
**			used for the Results Storage Application.  The
**			public entry is "work".
** Last Update:		$Author: smm $, $Date: 1997-05-30 19:56:40 $
** Source File:		$RCSfile: workImpInterp.c,v $
** Revision:		$Revision: 1.5 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.5 $ $RCSfile: workImpInterp.c,v $";

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#ifdef MACH
#include <unistd.h>
#endif

#include "dicom.h"
#include "condition.h"
#include "tbl.h"
#include "lst.h"
#include "dicom_objects.h"
#include "tbl.h"
#include "manage.h"
#include "idb.h"
#include "fis.h"

extern char *retrieveAETitle;

typedef struct {
    void *reserved[2];
    IDB_Query query;
}   QUERY_LIST_ITEM;

/* work
**
** Purpose:
**
** Parameter Dictionary:
**	idb	The handle for accessing the image database.
**	fis	The handle for accessing the FIS.
**
** Return Values:
**	1 for success
**	2 reached EOF
**	0 for failure
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
work(FIS_HANDLE ** fis)
{
    CONDITION cond;
    LST_HEAD *lst = NULL;
    char vendor[256];
    char patientName[256];
    char accessionNumber[256];
    char line[256];
    char studyInstanceUID[DICOM_UI_LENGTH + 1];
    FIS_RESULTSRECORD results;
    FIS_INTERPRETATIONRECORD interpretation;

    if (fgets(vendor, sizeof(vendor), stdin) == NULL)
	return 2;
    if (fgets(patientName, sizeof(patientName), stdin) == NULL)
	return 0;
    if (fgets(accessionNumber, sizeof(accessionNumber), stdin) == NULL)
	return 0;
    printf("%s %s %s\n", vendor, patientName, accessionNumber);

    if (fgets(line, sizeof(line), stdin) == NULL)
	return 0;
    if (sscanf(line, "%s", studyInstanceUID) != 1)
	return 0;

    memset(&results, 0, sizeof(results));
    results.Type = FIS_K_RESULTS;
    cond = FIS_GetOne(fis, FIS_K_RESULTS, FIS_K_STUDY, studyInstanceUID,
		      0, &results);
    if (cond != FIS_NORMAL)
	return 0;

    results.Imp[0] = '\0';
    while (1) {
	if (fgets(line, sizeof(line), stdin) == NULL)
	    return 0;
	if (strncmp(line, "****", 4) == 0)
	    break;
	if (strlen(results.Imp) + strlen(line) <= sizeof(results.Imp) - 1)
	    strcat(results.Imp, line);
    }

    results.Imp[strlen(results.Imp) - 1] = '\0';
    cond = FIS_Update(fis, FIS_K_RESULTS, &results);
    if (cond != FIS_NORMAL)
	return 0;

    memset(&interpretation, 0, sizeof(interpretation));
    interpretation.Type = FIS_K_INTERPRETATION;

    cond = FIS_GetOne(fis, FIS_K_INTERPRETATION, FIS_K_RESULTS,
		      results.ResUID, 0, &interpretation);
    if (cond != FIS_NORMAL) {
	return 0;
    }
    interpretation.IntTex[0] = '\0';

    while (1) {
	if (fgets(line, sizeof(line), stdin) == NULL)
	    break;
	if (strncmp(line, "****", 4) == 0)
	    break;
	if (strlen(interpretation.IntTex) + strlen(line) <=
	    sizeof(interpretation.IntTex) - 10) {
	    strcat(interpretation.IntTex, line);
	}
    }
    printf("%d\n", strlen(interpretation.IntTex));

    UTL_GetDicomDate(interpretation.IntAppDat);
    UTL_GetDicomTime(interpretation.IntAppTim);
    strcpy(interpretation.PhyAppInt, "STAFF");
    strcpy(interpretation.IntStaID, "APPROVED");
    interpretation.Flag = FIS_K_INTERP_INTTEX;

    cond = FIS_Update(fis, FIS_K_INTERPRETATION, &interpretation);
    if (cond != FIS_NORMAL)
	return 0;

    interpretation.Flag = FIS_K_INTERP_INTAPPDAT |
	FIS_K_INTERP_INTAPPTIM | FIS_K_INTERP_PHYAPPINT |
	FIS_K_INTERP_INTSTAID;

    cond = FIS_Update(fis, FIS_K_INTERPRETATION, &interpretation);
    if (cond != FIS_NORMAL)
	return 0;

    strcpy(interpretation.IntDiaDes, interpretation.IntTex);
    interpretation.Flag = FIS_K_INTERP_INTDIADES;
    cond = FIS_Update(fis, FIS_K_INTERPRETATION, &interpretation);
    if (cond != FIS_NORMAL)
	return 0;

    return 1;
}
