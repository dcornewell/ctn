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
**				DINPACS 1997
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	Stephen Moore, March-1997
** Intent:		The archive_server application is an extension of
**			the CTN image_server application.  This application
**			adds support for the DICOM Storage Commitment Push Model
**			SOP class.
** Last Update:		$Author: smm $, $Date: 2001-05-30 03:50:41 $
** Source File:		$RCSfile: idb_dump.c,v $
** Revision:		$Revision: 1.1 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.1 $ $RCSfile: idb_dump.c,v $";

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <signal.h>
#include <errno.h>
#ifndef _MSC_VER
#include <sys/types.h>
#include <sys/param.h>
#endif
#ifdef SOLARIS
#include <netdb.h>
#endif
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
/*lint -e46*/
#include <sys/wait.h>
/*lint +e46*/
#endif
#ifdef _MSC_VER
#include <windows.h>
#include <process.h>
#include <winsock.h>
#endif

#ifdef CTN_USE_THREADS
#ifdef _MSC_VER
#include <process.h>
#else
#include <pthread.h>
#endif
#endif

#include "dicom.h"
#include "condition.h"
#include "tbl.h"
#include "lst.h"
#include "dicom_uids.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#ifdef CTN_MULTIBYTE
#include "tblmb.h"
#include "idbmb.h"
#else
#include "tbl.h"
#include "idb.h"
#endif
#include "manage.h"
#include "utility.h"
#include "ctnthread.h"
#include "gq.h"

static void
usageerror()
{
    char msg[] = "\
Usage: <database name> <object type>\n\
\n\
<object type> is PATIENT, STUDY, SERIES, INSTANCE\n";

    fprintf(stderr, msg);
    exit(5);
}


typedef struct {
    void *reserved[2];
    IDB_Query query;
}   QUERY_LIST_ITEM;

static CONDITION
selectCallback(IDB_Query * queryResponse, long count, LST_HEAD * lst)
{
  QUERY_LIST_ITEM * item;

  item = malloc(sizeof(*item));
  if (item == NULL)
    return 0;

  item->query = *queryResponse;
  (void) LST_Enqueue(&lst, item);
  return IDB_NORMAL;
}

static void runSeriesQuery(IDB_HANDLE** h)
{
  CONDITION cond;
  LST_HEAD* lst = 0;
  IDB_Query queryStructure;
  QUERY_LIST_ITEM * queryItem;
  long selectCount = 0;

  lst = LST_Create();

  memset(&queryStructure, 0, sizeof(queryStructure));
  cond = IDB_Select(h, PATIENT_ROOT, IDB_SERIES_LEVEL, IDB_SERIES_LEVEL,
			&queryStructure, &selectCount, selectCallback, lst);
  if (cond != IDB_NORMAL) {
    COND_DumpConditions();
    return;
  }

  queryItem = LST_Dequeue(&lst);
  while (queryItem != NULL) {
    printf ("%s %s \n",
	queryItem->query.series.Mod,
	queryItem->query.series.SerInsUID);
    queryItem = LST_Dequeue(&lst);
  }
}

static void runQuery(IDB_HANDLE** h, const char* objType)
{
  if (strcmp(objType, "SERIES") == 0)
    runSeriesQuery(h);
}

main(int argc, char **argv)
{
  CONDITION cond;
  CTNBOOLEAN verbose = FALSE;
  IDB_HANDLE* h = 0;
  char* dbName = "";
  char* objType = "";

  while (--argc > 0 && *(++argv)[0] == '-') {
    switch ((*argv)[1]) {
    case 'v':
      verbose = TRUE;
      break;
    default:
      printf("Unrecognized option: %s\n", *argv);
      break;
    }
  }

  if (argc < 2)
    usageerror();

  dbName = argv[0];
  objType = argv[1];

  cond = IDB_Open(dbName, &h);
  if (cond != IDB_NORMAL) {
    COND_DumpConditions();
    exit(1);
  }

  runQuery(&h, objType);

  cond = IDB_Close(&h);
  if (cond != IDB_NORMAL) {
    COND_DumpConditions();
    exit(1);
  }

  return 0;
}
