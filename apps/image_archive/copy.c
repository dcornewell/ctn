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
**				DICOM 94
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:
** Intent:
** Last Update:		$Author: smm $, $Date: 2001-12-21 22:30:58 $
** Source File:		$Source: /home/smm/ctn/ctn/apps/image_archive/copy.c,v $
** Revision:		$Revision: 1.5 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.5 $ $RCSfile: copy.c,v $";

#include "ctn_os.h"
#if 0
#include <stdio.h>
#include <stdlib.h>
#endif

#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "dicom_objects.h"
#include "dulprotocol.h"
#include "icpy.h"
#ifdef CTN_MULTIBYTE
#include "tblmb.h"
#include "idbmb.h"
#else
#include "tbl.h"
#include "idb.h"
#endif
#include "manage.h"

#include "image_archive.h"
#include "copy.h"

typedef struct {
    LST_HEAD **lst;
    void *record;
}   X_CONTEXT;

#ifdef COPY
static CTNBOOLEAN openFlag = FALSE;
static TBL_HANDLE *imageForwardTable;
static TBL_HANDLE *imageCopyTable;
static LST_HEAD *forwardList = NULL;
static CONDITION
selectForwardRecords(LST_HEAD ** l, char *callingAE,
		     char *calledAE);
static CONDITION
    callbackForward(TBL_FIELD * field, int count, X_CONTEXT * ctx);
static CONDITION insertCopyRecord(ICPY_COPYRECORD * copy);

CONDITION
copyOpen(char *database, char *callingAE, char *calledAE)
{
    CONDITION
	cond;

    if (openFlag)
	return APP_NORMAL;

    forwardList = LST_Create();
    if (forwardList == NULL)
	return 0;

    cond = TBL_Open(database, "ImageForward", &imageForwardTable);
    if (cond != TBL_NORMAL)
	return 0;

    cond = TBL_Open(database, "ImageCopy", &imageCopyTable);
    if (cond != TBL_NORMAL) {
	(void) TBL_Close(&imageForwardTable);
	return 0;
    }
    cond = selectForwardRecords(&forwardList, callingAE, calledAE);
    if (cond != APP_NORMAL)
	return 0;

    openFlag = TRUE;

}

CONDITION
copyClose(void)
{
    if (openFlag) {
	(void) LST_Destroy(&forwardList);
	(void) TBL_Close(&imageForwardTable);
	(void) TBL_Close(&imageCopyTable);
    }
    openFlag = FALSE;
    return APP_NORMAL;
}

CONDITION
copyEnqueue(DCM_OBJECT ** obj)
{
    ICPY_FORWARDRECORD
	* f;
    static ICPY_COPYRECORD
        copy;
    DCM_ELEMENT e[] = {
	{DCM_IDACCESSIONNUMBER, DCM_SH, "", 1, sizeof(copy.Accession),
	(void *) copy.Accession},
	{DCM_IDMODALITY, DCM_CS, "", 1, sizeof(copy.Modality),
	(void *) copy.Modality},
	{DCM_IDSOPINSTANCEUID, DCM_UI, "", 1, sizeof(copy.SOPInstanceUID),
	(void *) copy.SOPInstanceUID}
    };
    CONDITION
	cond;

    if (openFlag) {
	cond = DCM_ParseObject(obj, e, (int) DIM_OF(e), NULL, 0, NULL);

	f = LST_Head(&forwardList);
	(void) LST_Position(&forwardList, f);

	while (f != NULL && !ERROR(cond)) {
	    strcpy(copy.Source, f->SourceApplication);
	    strcpy(copy.Destination, f->ForwardingApplication);
	    copy.CopyNumber = 0;
	    copy.RetryCount = 0;
	    copy.FailureStatus = 0;
	    cond = insertCopyRecord(&copy);
	    if (cond != APP_NORMAL)
		break;
	    f = LST_Next(&forwardList);
	}
    }
    return APP_NORMAL;
}

static CONDITION
selectForwardRecords(LST_HEAD ** l, char *callingAE,
		     char *calledAE)
{
    CONDITION
    cond;
    static ICPY_FORWARDRECORD
        forward;
    static TBL_FIELD
        fields[] = {
	{"SourceApplication", TBL_STRING,
	    sizeof(forward.SourceApplication), 0, 0,
	(void *) forward.SourceApplication},
	{"DestinationApplication", TBL_STRING,
	    sizeof(forward.DestinationApplication), 0, 0,
	(void *) forward.DestinationApplication},
	{"ForwardingApplication", TBL_STRING,
	    sizeof(forward.ForwardingApplication), 0, 0,
	(void *) forward.ForwardingApplication},
	{NULL, TBL_STRING, 0, 0, 0, NULL}
    };
    TBL_CRITERIA
	criteria[3];
    int
        i;
    X_CONTEXT
	context;

    context.lst = l;
    context.record = &forward;

    i = 0;

    criteria[i].FieldName = "SourceApplication";
    criteria[i].Operator = TBL_EQUAL;
    TBL_LOAD_STRING(&criteria[i].Value, callingAE);

    i++;
    criteria[i].FieldName = "DestinationApplication";
    criteria[i].Operator = TBL_EQUAL;
    TBL_LOAD_STRING(&criteria[i].Value, calledAE);

    i++;
    criteria[i].FieldName = NULL;

    cond = TBL_Select(&imageForwardTable, criteria, fields, NULL,
		      callbackForward, &context);
    if (cond != TBL_NORMAL)
	return 0;

    return APP_NORMAL;
}
static CONDITION
callbackForward(TBL_FIELD * field, int count, CONTEXT * ctx)
{
    ICPY_FORWARDRECORD
    * forward,
    *localForward;
    CONDITION
	cond;
    int
        index;

    localForward = malloc(sizeof(*localForward));
    if (localForward == NULL)
	return 0;

    forward = (ICPY_FORWARDRECORD *) ctx->record;

    *localForward = *forward;
    if (*ctx->lst != NULL) {
	cond = LST_Enqueue(ctx->lst, localForward);
	if (cond != LST_NORMAL)
	    return 0;
    }
    return TBL_NORMAL;
}

static CONDITION
insertCopyRecord(ICPY_COPYRECORD * copy)
{
    static ICPY_COPYRECORD
        c;
    int
        i;
    CONDITION
	cond;

    TBL_FIELD fields[] = {
	{"Accession", TBL_STRING, sizeof(c.Accession),
	sizeof(c.Accession), 0, (void *) c.Accession},
	{"Source", TBL_STRING, sizeof(c.Source),
	sizeof(c.Source), 0, (void *) c.Source},
	{"Destination", TBL_STRING, sizeof(c.Destination),
	sizeof(c.Destination), 0, (void *) c.Destination},
	{"Modality", TBL_STRING, sizeof(c.Modality),
	sizeof(c.Modality), 0, (void *) c.Modality},
	{"SOPInstanceUID", TBL_STRING, sizeof(c.SOPInstanceUID),
	sizeof(c.SOPInstanceUID), 0, (void *) c.SOPInstanceUID},
	{"CopyNumber", TBL_SIGNED4, sizeof(c.CopyNumber),
	sizeof(c.CopyNumber), 0, (void *) &c.CopyNumber},
	{"RetryCount", TBL_SIGNED4, sizeof(c.RetryCount),
	sizeof(c.RetryCount), 0, (void *) &c.RetryCount},
	{"FailureStatus", TBL_SIGNED4, sizeof(c.FailureStatus),
	sizeof(c.FailureStatus), 0, (void *) &c.FailureStatus},
	{NULL},
    };

    c = *copy;
    for (i = 0; fields[i].FieldName != NULL; i++) {
	if (fields[i].Value.Type == TBL_STRING)
	    fields[i].Value.Size = strlen(fields[i].Value.Value.String);
    }
    cond = TBL_Insert(&imageCopyTable, fields);
    if (cond != TBL_NORMAL)
	COND_DumpConditions();

    return APP_NORMAL;
}
#endif
