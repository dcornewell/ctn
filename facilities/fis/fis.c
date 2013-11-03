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
** Module Name(s):	FIS_Open
**			FIS_Close
**			FIS_Debug
** Author, Date:	Stephen Moore, 12-Jul-94
** Intent:		This file contains the open and close functions
**			for the FIS (Fake Information System) facility.
** Last Update:		$Author: smm $, $Date: 1997-05-30 18:53:36 $
** Source File:		$RCSfile: fis.c,v $
** Revision:		$Revision: 1.12 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.12 $ $RCSfile: fis.c,v $";

#include <stdio.h>
#include <stdlib.h>

#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "dicom_objects.h"
#include "tbl.h"
#include "manage.h"
#include "fis.h"

#include "fis_private.h"

CTNBOOLEAN __FIS_verbose__ = FALSE;

/* Define the private entries so we don't have to forward declare them */

#define DUMP(s1, s2, f1, f2, p) \
  if (f1 & f2) fprintf(p, "%-20s: %s\n", s1, s2);

static void
dumpStudy(FIS_STUDYRECORD * record, FILE * f)
{
    DUMP("Patient UID", record->PatUID, record->Flag, FIS_K_STU_PATUID, f);
    DUMP("Study Instance UID", record->StuInsUID, record->Flag, FIS_K_STU_STUINSUID, f);
    DUMP("Visit UID", record->VisUID, record->Flag, FIS_K_STU_VISUID, f);
    DUMP("Accession Number", record->AccNum, record->Flag, FIS_K_STU_ACCNUM, f);
}

static void
dumpResults(FIS_RESULTSRECORD * record, FILE * f)
{
    DUMP("Results UID", record->ResUID, record->Flag, FIS_K_RESULTS_RESUID, f);
    DUMP("Study Ins UID", record->StuInsUID, record->Flag, FIS_K_RESULTS_STUINSUID, f);
    DUMP("Results ID", record->ResID, record->Flag, FIS_K_RESULTS_RESID, f);
    DUMP("Impressions", record->Imp, record->Flag, FIS_K_RESULTS_IMP, f);
    DUMP("Ins Creation Date", record->InsCreDat, record->Flag, FIS_K_RESULTS_INSCREDAT, f);
    DUMP("Ins Creation Time", record->InsCreTim, record->Flag, FIS_K_RESULTS_INSCRETIM, f);
    DUMP("Ins Creator UID", record->InsCreUID, record->Flag, FIS_K_RESULTS_INSCREUID, f);
}

static void
dumpInterpretation(FIS_INTERPRETATIONRECORD * record, FILE * f)
{
    DUMP("Interpretation UID", record->IntUID, record->Flag, FIS_K_INTERP_INTUID, f);
    DUMP("Results UID", record->ResUID, record->Flag, FIS_K_INTERP_RESUID, f);
    DUMP("Interp ID", record->IntID, record->Flag, FIS_K_INTERP_INTID, f);
    DUMP("Interp Type", record->IntTypID, record->Flag, FIS_K_INTERP_INTTYPID, f);
    DUMP("Interp Status", record->IntStaID, record->Flag, FIS_K_INTERP_INTSTAID, f);
    DUMP("Phys Approving Int", record->PhyAppInt, record->Flag, FIS_K_INTERP_PHYAPPINT, f);
    DUMP("Interp approval date", record->IntAppDat, record->Flag, FIS_K_INTERP_INTAPPDAT, f);
    DUMP("Interp approval time", record->IntAppTim, record->Flag, FIS_K_INTERP_INTAPPTIM, f);
    DUMP("Interp Text", record->IntTex, record->Flag, FIS_K_INTERP_INTTEX, f);
    DUMP("Interp Diag Des", record->IntDiaDes, record->Flag, FIS_K_INTERP_INTDIADES, f);
}

/* Define the public entries below this point */

/* FIS_Open
**
** Purpose:
**	Open the Fake Information System set of tables so they can
**	be accessed by other functions in the FIS facility.
**
** Parameter Dictionary:
**	databaseName	The name of the database which holds the FIS tables.
**	handle		The FIS_HANDLE which is created by this function
**			and returned to the caller.  This handle will be used
**			by later FIS functions for access to FIS tables.
**
** Return Values:
**	FIS_NORMAL
**	FIS_OPENFAILED
**	FIS_MALLOCFAILURE
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
FIS_Open(char *databaseName, FIS_HANDLE ** handle)
{
    CONDITION
	cond;
    PRIVATE_HANDLE
	* h;

    h = malloc(sizeof(*h));
    if (h == NULL) {
	(void) COND_PushCondition(FIS_COND(FIS_MALLOCFAILURE),
				  sizeof(*h), "FIS_Open");
	return COND_PushCondition(FIS_COND(FIS_OPENFAILED), databaseName,
				  "FIS_Open");
    }
    cond = TBL_Open(databaseName, "PatientTable", &h->patientHandle);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_OPENFAILED), "PatientTable",
				  databaseName, "FIS_Open");

    cond = TBL_Open(databaseName, "StudyTable", &h->studyHandle);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_OPENFAILED), "StudyTable",
				  databaseName, "FIS_Open");

    cond = TBL_Open(databaseName, "StudyCmpTable",
		    &h->studyComponentHandle);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_OPENFAILED),
				  "StudyCmpTable",
				  databaseName, "FIS_Open");

    cond = TBL_Open(databaseName, "StudyCmpSeriesTable",
		    &h->studyComponentSeriesHandle);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_OPENFAILED),
			   "StudyCmpSeriesTable", databaseName, "FIS_Open");

    cond = TBL_Open(databaseName, "StudyCmpImageTable",
		    &h->studyComponentImageHandle);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_OPENFAILED),
			    "StudyCmpImageTable", databaseName, "FIS_Open");

    cond = TBL_Open(databaseName, "ResultsTable", &h->resultsHandle);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_OPENFAILED), "ResultsTable",
				  databaseName, "FIS_Open");

    cond = TBL_Open(databaseName, "InterpretationTable", &h->interpretationHandle);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_OPENFAILED), "InterpretationTable",
				  databaseName, "FIS_Open");

    cond = TBL_Open(databaseName, "UniqueNumbers", &h->uniquesHandle);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_OPENFAILED), "UniqueNumbers",
				  databaseName, "FIS_Open");

    cond = TBL_Open(databaseName, "StorageCommitReq", &h->storageCommitReqHandle);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_OPENFAILED), "StorageCommitReq",
				  databaseName, "FIS_Open");

    cond = TBL_Open(databaseName, "CommitSOPRef", &h->commitSOPRefHandle);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_OPENFAILED), "commitSOPRefHandle",
				  databaseName, "FIS_Open");

    cond = TBL_Open(databaseName, "WorkQueue", &h->workQueueHandle);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_OPENFAILED), "workQueueHandle",
				  databaseName, "FIS_Open");


    *handle = h;
    return FIS_NORMAL;
}

/* FIS_Close
**
** Purpose:
**	Close the Fake Information System set of tables so.
**
** Parameter Dictionary:
**	handle		The FIS_HANDLE which was created by FIS_Open.
**
** Return Values:
**	FIS_NORMAL
**	FIS_CLOSEFAILED
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
FIS_Close(FIS_HANDLE ** handle)
{
    CONDITION
	cond;
    PRIVATE_HANDLE
	* h;

    h = *(PRIVATE_HANDLE **) handle;

    cond = TBL_Close(&h->patientHandle);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_CLOSEFAILED), "FIS_Close");

    cond = TBL_Close(&h->studyHandle);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_CLOSEFAILED), "FIS_Close");

    cond = TBL_Close(&h->studyComponentHandle);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_CLOSEFAILED), "FIS_Close");

    cond = TBL_Close(&h->studyComponentSeriesHandle);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_CLOSEFAILED), "FIS_Close");

    cond = TBL_Close(&h->studyComponentImageHandle);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_CLOSEFAILED), "FIS_Close");

    cond = TBL_Close(&h->resultsHandle);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_CLOSEFAILED), "FIS_Close");

    cond = TBL_Close(&h->interpretationHandle);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_CLOSEFAILED), "FIS_Close");

    cond = TBL_Close(&h->uniquesHandle);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_CLOSEFAILED), "FIS_Close");

    cond = TBL_Close(&h->storageCommitReqHandle);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_CLOSEFAILED), "FIS_Close");

    cond = TBL_Close(&h->commitSOPRefHandle);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_CLOSEFAILED), "FIS_Close");

    cond = TBL_Close(&h->workQueueHandle);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(FIS_COND(FIS_CLOSEFAILED), "FIS_Close");

    free(h);
    *handle = NULL;
    return FIS_NORMAL;
}

void
FIS_Debug(CTNBOOLEAN flag)
{
    __FIS_verbose__ = flag;
}

void
FIS_DumpRecord(void *record, FILE * f)
{
    switch (((FIS_GENERICRECORD *) record)->Type) {
	case FIS_K_STUDY:
	dumpStudy((FIS_STUDYRECORD *) record, f);
	break;
    case FIS_K_RESULTS:
	dumpResults((FIS_RESULTSRECORD *) record, f);
	break;
    case FIS_K_INTERPRETATION:
	dumpInterpretation((FIS_INTERPRETATIONRECORD *) record, f);
	break;
    default:
	break;
    }
}
