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
** Module Name(s):	IRS_Open
**			IRS_Close
**			IRS_Debug
** Author, Date:	Stephen Moore, 12-Jul-94
** Intent:		This file contains the open and close functions
**			for the IRS (Fake Information System) facility.
** Last Update:		$Author: smm $, $Date: 1998-10-22 17:46:23 $
** Source File:		$RCSfile: irs.c,v $
** Revision:		$Revision: 1.1 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.1 $ $RCSfile: irs.c,v $";

#include <stdio.h>
#include <stdlib.h>

#include "dicom.h"
#include "condition.h"
#include "utility.h"
#include "lst.h"
#include "dicom_objects.h"
#include "tbl.h"
#include "dicom_irs.h"

#include "irs_private.h"

CTNBOOLEAN __IRS_verbose__ = FALSE;

/* Define the private entries so we don't have to forward declare them */

#define DUMP(s1, s2, f1, f2, p) \
  if (f1 & f2) fprintf(p, "%-20s: %s\n", s1, s2);
typedef struct {
    IRS_WORKRECORD *w;
    LST_HEAD *l;
}   WORKRECORD_CALLBACK_STRUCT;

static CONDITION
workRecordCallback(TBL_FIELD * field, int count, void *v)
{
    IRS_WORKRECORD *w;
    int i;
    WORKRECORD_CALLBACK_STRUCT *s;

    s = (WORKRECORD_CALLBACK_STRUCT *) v;

    if (s->l != NULL) {
	w = calloc(1, sizeof(*w));
	if (w == NULL)
	    return TBL_EARLYEXIT;
    } else {
	w = s->w;
    }

    /*
     * The order that we process these fields must match the * order in which
     * they were declared.
     */

    i = 0;
    strcpy(w->PatID, field[i].Value.Value.String);
    i++;
    strcpy(w->PatNam, field[i].Value.Value.String);
    i++;
    strcpy(w->StuUID, field[i].Value.Value.String);
    i++;
    strcpy(w->AccNum, field[i].Value.Value.String);
    i++;
    strcpy(w->InsAET, field[i].Value.Value.String);
    i++;
    strcpy(w->UpdPatID, field[i].Value.Value.String);
    i++;
    strcpy(w->UpdPatNam, field[i].Value.Value.String);
    i++;
    strcpy(w->UpdAccNum, field[i].Value.Value.String);
    i++;
    strcpy(w->Operator, field[i].Value.Value.String);
    i++;
    strcpy(w->Comments, field[i].Value.Value.String);
    i++;
    strcpy(w->Status, field[i].Value.Value.String);
    i++;
    UTL_ConvertLongtoDate(*field[i].Value.Value.Signed4,
			  w->UpdReqDat);
    i++;
    UTL_ConvertFloattoTime(*field[i].Value.Value.Float4,
			   w->UpdReqTim);

    i++;
    if (field[i].Value.IsNull) {
	w->UpdComDat[0] = '\0';
	w->UpdComTim[0] = '\0';
    } else {
	UTL_ConvertLongtoDate(*field[i].Value.Value.Signed4,
			      w->UpdComDat);
	UTL_ConvertFloattoTime(*field[i + 1].Value.Value.Float4,
			       w->UpdComTim);
    }

    i += 2;
    if (*field[i].Value.Value.Signed4 == 0)
	strcpy(w->StuDat, "");
    else
	UTL_ConvertLongtoDate(*field[i].Value.Value.Signed4,
			      w->StuDat);

    if (s->l != NULL) {
	if (LST_Enqueue(&s->l, w) != LST_NORMAL)
	    return TBL_EARLYEXIT;
    }
    return TBL_NORMAL;
}

/* Define the public entries below this point */

/* IRS_Open
**
** Purpose:
**	Open the Image Recycle System set of tables so they can
**	be accessed by other functions in the IRS facility.
**
** Parameter Dictionary:
**	databaseName	The name of the database which holds the IRS tables.
**	handle		The IRS_HANDLE which is created by this function
**			and returned to the caller.  This handle will be used
**			by later IRS functions for access to IRS tables.
**
** Return Values:
**	IRS_NORMAL
**	IRS_OPENFAILED
**	IRS_MALLOCFAILURE
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
IRS_Open(char *databaseName, IRS_HANDLE ** handle)
{
    CONDITION
	cond;
    PRIVATE_HANDLE
	* h;

    h = malloc(sizeof(*h));
    if (h == NULL) {
	(void) COND_PushCondition(IRS_COND(IRS_MALLOCFAILURE),
				  sizeof(*h), "IRS_Open");
	return COND_PushCondition(IRS_COND(IRS_OPENFAILED), databaseName,
				  "IRS_Open");
    }
    cond = TBL_Open(databaseName, "IRSWorkQueue", &h->workQueueHandle);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(IRS_COND(IRS_OPENFAILED), "workQueueHandle",
				  databaseName, "IRS_Open");

    *handle = h;
    return IRS_NORMAL;
}

/* IRS_Close
**
** Purpose:
**	Close the Fake Information System set of tables so.
**
** Parameter Dictionary:
**	handle		The IRS_HANDLE which was created by IRS_Open.
**
** Return Values:
**	IRS_NORMAL
**	IRS_CLOSEFAILED
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
IRS_Close(IRS_HANDLE ** handle)
{
    CONDITION
	cond;
    PRIVATE_HANDLE
	* h;

    h = *(PRIVATE_HANDLE **) handle;

    cond = TBL_Close(&h->workQueueHandle);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(IRS_COND(IRS_CLOSEFAILED), "IRS_Close");

    free(h);
    *handle = NULL;
    return IRS_NORMAL;
}

void
IRS_Debug(CTNBOOLEAN flag)
{
    __IRS_verbose__ = flag;
}


CONDITION
IRS_EnterWorkRecord(IRS_HANDLE ** handle, const IRS_WORKRECORD * p)
{
    CONDITION cond;
    TBL_FIELD fields[20];
    int i;
    S32 requestDate;
    S32 studyDate;
    float requestTime;
    char dicomDate[DICOM_DA_LENGTH + 1];
    char dicomTime[DICOM_TM_LENGTH + 1];
    char status[41];
    PRIVATE_HANDLE *h;
    IRS_WORKRECORD r;

    h = *(PRIVATE_HANDLE **) handle;
    r = *p;

    i = 0;
    TBL_FIELD_LOAD_BYTE(fields[i], "PatID", r.PatID, TBL_STRING);
    i++;
    TBL_FIELD_LOAD_BYTE(fields[i], "PatNam", r.PatNam, TBL_STRING);
    i++;
    TBL_FIELD_LOAD_BYTE(fields[i], "StuUID", r.StuUID, TBL_STRING);
    i++;
    TBL_FIELD_LOAD_BYTE(fields[i], "AccNum", r.AccNum, TBL_STRING);
    i++;
    TBL_FIELD_LOAD_BYTE(fields[i], "InsAET", r.InsAET, TBL_STRING);
    i++;
    TBL_FIELD_LOAD_BYTE(fields[i], "UpdPatID", r.UpdPatID, TBL_STRING);
    i++;
    TBL_FIELD_LOAD_BYTE(fields[i], "UpdPatNam", r.UpdPatNam, TBL_STRING);
    i++;
    TBL_FIELD_LOAD_BYTE(fields[i], "UpdAccNum", r.UpdAccNum, TBL_STRING);
    i++;
    TBL_FIELD_LOAD_BYTE(fields[i], "Operator", r.Operator, TBL_STRING);
    i++;
    TBL_FIELD_LOAD_BYTE(fields[i], "Comments", r.Comments, TBL_STRING);
    i++;
    TBL_FIELD_LOAD_BYTE(fields[i], "Status", r.Status, TBL_STRING);
    i++;

    UTL_GetDicomDate(dicomDate);
    requestDate = UTL_ConvertDatetoLong(dicomDate);
    TBL_FIELD_LOAD_NUM(fields[i], "UpdReqDat", requestDate, TBL_SIGNED4);
    i++;
    UTL_GetDicomTime(dicomTime);
    requestTime = UTL_ConvertTimetoFloat(dicomTime);
    TBL_FIELD_LOAD_NUM(fields[i], "UpdReqTim", requestTime, TBL_FLOAT4);

    i++;
    if (strlen(r.StuDat) != 0)
	studyDate = UTL_ConvertDatetoLong(r.StuDat);
    else
	studyDate = 0;
    TBL_FIELD_LOAD_NUM(fields[i], "StuDat", studyDate, TBL_SIGNED4);

    i++;
    fields[i].FieldName = NULL;

    cond = TBL_Insert(&h->workQueueHandle, fields);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(IRS_COND(IRS_INSERTFAILED),
				  "workQueue", "IRS_EnterWorkRecord");


    return IRS_NORMAL;
}

CONDITION
IRS_GetWorkQueue(IRS_HANDLE ** handle, LST_HEAD * lst)
{
    CONDITION cond;
    TBL_CRITERIA criteria[2];
    IRS_WORKRECORD r;
    int i;

    TBL_FIELD fields[20];
    U32 completedDate = 0;
    U32 requestDate;
    U32 requestCompleteDate;
    U32 studyDate;
    float requestTime;
    float requestCompleteTime;
    WORKRECORD_CALLBACK_STRUCT s;

    PRIVATE_HANDLE *h;
    U32 count = 0;

    h = *(PRIVATE_HANDLE **) handle;

    i = 0;
    TBL_FIELD_DECLARE_STRING(fields[i], "PatID", r.PatID, sizeof(r.PatID));
    i++;
    TBL_FIELD_DECLARE_STRING(fields[i], "PatNam", r.PatNam, sizeof(r.PatNam));
    i++;
    TBL_FIELD_DECLARE_STRING(fields[i], "StuUID", r.StuUID, sizeof(r.StuUID));
    i++;
    TBL_FIELD_DECLARE_STRING(fields[i], "AccNum", r.AccNum, sizeof(r.AccNum));
    i++;
    TBL_FIELD_DECLARE_STRING(fields[i], "InsAET", r.InsAET, sizeof(r.InsAET));
    i++;
    TBL_FIELD_DECLARE_STRING(fields[i], "UpdPatID", r.UpdPatID, sizeof(r.UpdPatID));
    i++;
    TBL_FIELD_DECLARE_STRING(fields[i], "UpdPatNam", r.UpdPatNam, sizeof(r.UpdPatNam));
    i++;
    TBL_FIELD_DECLARE_STRING(fields[i], "UpdAccNum", r.UpdAccNum, sizeof(r.UpdAccNum));
    i++;
    TBL_FIELD_DECLARE_STRING(fields[i], "Operator", r.Operator, sizeof(r.Operator));
    i++;
    TBL_FIELD_DECLARE_STRING(fields[i], "Comments", r.Comments, sizeof(r.Comments));
    i++;
    TBL_FIELD_DECLARE_STRING(fields[i], "Status", r.Status, sizeof(r.Status));
    i++;
    TBL_FIELD_DECLARE_NUM(fields[i], "UpdReqDat", TBL_SIGNED4,
			  requestDate, sizeof(requestDate));
    i++;
    TBL_FIELD_DECLARE_NUM(fields[i], "UpdReqTim", TBL_FLOAT4,
			  requestTime, sizeof(requestTime));
    i++;
    TBL_FIELD_DECLARE_NUM(fields[i], "UpdComDat", TBL_SIGNED4,
			  requestCompleteDate, sizeof(requestCompleteDate));
    i++;
    TBL_FIELD_DECLARE_NUM(fields[i], "UpdComTim", TBL_FLOAT4,
			  requestCompleteTime, sizeof(requestCompleteTime));
    i++;
    TBL_FIELD_DECLARE_NUM(fields[i], "StuDat", TBL_SIGNED4,
			  studyDate, sizeof(studyDate))
	i++;

    fields[i].FieldName = NULL;
    fields[i].Value.Type = TBL_STRING;
    fields[i].Value.AllocatedSize = 0;
    fields[i].Value.Size = 0;
    fields[i].Value.IsNull = 1;
    fields[i].Value.Value.String = NULL;

    TBL_CRITERIA_LOAD_NUM(criteria[0], "UpdComDat", completedDate,
			  TBL_SIGNED4, TBL_NULL);
    criteria[1].FieldName = NULL;

    memset(&r, 0, sizeof(r));
    s.l = lst;
    s.w = NULL;
    cond = TBL_Select(&h->workQueueHandle, criteria, fields,
		      NULL, workRecordCallback, &s);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(IRS_COND(IRS_SELECTFAILED),
				  "IRS_GetWorkQueue");

    return IRS_NORMAL;
}

CONDITION
IRS_GetWorkRecord(IRS_HANDLE ** handle, const char *studyUID,
		  IRS_WORKRECORD * rec)
{
    CONDITION cond;
    TBL_CRITERIA criteria[2];
    IRS_WORKRECORD r;
    int i;

    TBL_FIELD fields[20];
    U32 requestDate;
    U32 requestCompleteDate;
    U32 studyDate;
    float requestTime;
    float requestCompleteTime;
    PRIVATE_HANDLE *h;
    long count = 0;
    WORKRECORD_CALLBACK_STRUCT s;

    h = *(PRIVATE_HANDLE **) handle;

    i = 0;
    TBL_FIELD_DECLARE_STRING(fields[i], "PatID", r.PatID, sizeof(r.PatID));
    i++;
    TBL_FIELD_DECLARE_STRING(fields[i], "PatNam", r.PatNam, sizeof(r.PatNam));
    i++;
    TBL_FIELD_DECLARE_STRING(fields[i], "StuUID", r.StuUID, sizeof(r.StuUID));
    i++;
    TBL_FIELD_DECLARE_STRING(fields[i], "AccNum", r.AccNum, sizeof(r.AccNum));
    i++;
    TBL_FIELD_DECLARE_STRING(fields[i], "InsAET", r.InsAET, sizeof(r.InsAET));
    i++;
    TBL_FIELD_DECLARE_STRING(fields[i], "UpdPatID", r.UpdPatID, sizeof(r.UpdPatID));
    i++;
    TBL_FIELD_DECLARE_STRING(fields[i], "UpdPatNam", r.UpdPatNam, sizeof(r.UpdPatNam));
    i++;
    TBL_FIELD_DECLARE_STRING(fields[i], "UpdAccNum", r.UpdAccNum, sizeof(r.UpdAccNum));
    i++;
    TBL_FIELD_DECLARE_STRING(fields[i], "Operator", r.Operator, sizeof(r.Operator));
    i++;
    TBL_FIELD_DECLARE_STRING(fields[i], "Comments", r.Comments, sizeof(r.Comments));
    i++;
    TBL_FIELD_DECLARE_STRING(fields[i], "Status", r.Status, sizeof(r.Status));
    i++;

    TBL_FIELD_DECLARE_NUM(fields[i], "UpdReqDat", TBL_SIGNED4,
			  requestDate, sizeof(requestDate));
    i++;
    TBL_FIELD_DECLARE_NUM(fields[i], "UpdReqTim", TBL_FLOAT4,
			  requestTime, sizeof(requestTime));
    i++;
    TBL_FIELD_DECLARE_NUM(fields[i], "UpdComDat", TBL_SIGNED4,
			  requestCompleteDate, sizeof(requestCompleteDate));
    i++;
    TBL_FIELD_DECLARE_NUM(fields[i], "UpdComTim", TBL_FLOAT4,
			  requestCompleteTime, sizeof(requestCompleteTime));
    i++;
    TBL_FIELD_DECLARE_NUM(fields[i], "StuDat", TBL_SIGNED4,
			  studyDate, sizeof(studyDate));
    i++;

    fields[i].FieldName = NULL;
    fields[i].Value.Type = TBL_STRING;
    fields[i].Value.AllocatedSize = 0;
    fields[i].Value.Size = 0;
    fields[i].Value.IsNull = 1;
    fields[i].Value.Value.String = NULL;

    TBL_CRITERIA_LOAD_BYTE(criteria[0], "StuUID", (char *) studyUID,
			   TBL_STRING, TBL_EQUAL);
    criteria[1].FieldName = NULL;

    memset(&r, 0, sizeof(r));
    s.l = NULL;
    s.w = &r;
    cond = TBL_Select(&h->workQueueHandle, criteria, fields,
		      &count, workRecordCallback, &s);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(IRS_COND(IRS_SELECTFAILED),
				  "IRS_GetWorkRecord");

    if (count == 1) {
	*rec = r;
	return IRS_NORMAL;
    } else {
	memset(rec, 0, sizeof(*rec));
	return COND_PushCondition(IRS_COND(IRS_SELECTFAILED),
				  "IRS_GetWorkRecord");
    }
}

CONDITION
IRS_SetWorkRecordComplete(IRS_HANDLE ** handle, const char *studyUID)
{
    TBL_UPDATE update[10];
    TBL_CRITERIA criteria[2];
    int i;
    S32 completeDate;
    float completeTime;
    char dateText[128];
    char timeText[128];
    char status[41];
    CONDITION cond;

    PRIVATE_HANDLE *h;

    h = *(PRIVATE_HANDLE **) handle;

    TBL_CRITERIA_LOAD_BYTE(criteria[0], "StuUID", (char *) studyUID,
			   TBL_STRING, TBL_EQUAL);
    criteria[1].FieldName = NULL;

    i = 0;

    UTL_GetDicomDate(dateText);
    completeDate = UTL_ConvertDatetoLong(dateText);
    TBL_UPDATE_LOAD_NUM(update[i], "UpdComDat", completeDate,
			TBL_SIGNED4, TBL_SET);
    i++;
    UTL_GetDicomTime(timeText);
    completeTime = UTL_ConvertTimetoFloat(timeText);
    TBL_UPDATE_LOAD_NUM(update[i], "UpdComTim", completeTime,
			TBL_FLOAT4, TBL_SET);
    i++;
    strcpy(status, "COMPLETE");
    TBL_UPDATE_LOAD_BYTE(update[i], "Status", status,
			 TBL_STRING, TBL_SET);

    i++;
    update[i].FieldName = NULL;

    cond = TBL_Update(&h->workQueueHandle, criteria, update);
    if (cond != TBL_NORMAL)
	return COND_PushCondition(IRS_COND(IRS_UPDATEFAILED),
				  "IRS_SetWorkRecordComplete");

    return IRS_NORMAL;
}

CONDITION
IRS_DeleteCompletedEntries(IRS_HANDLE ** handle, int deltaDays)
{
    return IRS_NORMAL;
}
