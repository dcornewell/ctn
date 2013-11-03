/*
+-+-+-+-+-+-+-+-+-
*/
/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	Steve Moore, 21-Mar-1997
** Intent:		This program dumps the storage commit requests
**			in the FIS to the standard output.  It generates
**			a simple report with all events in the system.
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:37:38 $
** Source File:		$RCSfile: dump_commit_requests.c,v $
** Revision:		$Revision: 1.6 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.6 $ $RCSfile: dump_commit_requests.c,v $";

#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <stdlib.h>
#endif

#include "dicom.h"
#include "condition.h"
#include "lst.h"
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
#include "fis.h"
#include "ctnthread.h"

#include "image_archive.h"
#include "commit_result.h"

/* dumpCommitRequests
**
** Purpose:
**	This is the function that does the real work in this program.
**	It reads the storage commitment requests from the FIS and prints
**	a formatted report.
**
** Parameter Dictionary:
**	fisHandle	Handle to FIS to read the storage commitment requests.
**
** Return Values:
**
**	void
**
** Notes:
**
** Algorithm:
*/


static void
dumpCommitRequests(FIS_HANDLE ** fisHandle)
{
    LST_HEAD *requestList;
    FIS_STORAGECOMMITREQRECORD *commitReq;
    CONDITION cond;

    requestList = LST_Create();
    if (requestList == NULL) {
	fprintf(stderr, "Could not create an initial request list. Exiting now.\n");
	return;
    }
    cond = FIS_Get(fisHandle, FIS_K_STORAGECOMMITREQ, FIS_K_STORAGECOMMITREQ,
		   NULL, 0, requestList);
    if (cond != FIS_NORMAL) {
	COND_DumpConditions();
	return;
    }
    printf("%64s %16s %16s %12s %14s %12s %14s\n",
	   "Transaction UID",
	   "Requesting App",
	   "Responding App",
	   "Request Date",
	   "Request Time",
	   "Response Date",
	   "Response Time");

    while ((commitReq = LST_Dequeue(&requestList)) != NULL) {
	if (!(commitReq->Flag & FIS_K_SCOMMIT_REQDAT))
	    commitReq->ReqDat[0] = '\0';
	if (!(commitReq->Flag & FIS_K_SCOMMIT_REQTIM))
	    commitReq->ReqTim[0] = '\0';
	if (!(commitReq->Flag & FIS_K_SCOMMIT_RESDAT))
	    commitReq->ResDat[0] = '\0';
	if (!(commitReq->Flag & FIS_K_SCOMMIT_RESTIM))
	    commitReq->ResTim[0] = '\0';
	printf("%64s %16s %16s %12s %14s %12s %14s\n",
	       commitReq->TraUID,
	       commitReq->ReqAE,
	       commitReq->ResAE,
	       commitReq->ReqDat,
	       commitReq->ReqTim,
	       commitReq->ResDat,
	       commitReq->ResTim);
	free(commitReq);
    }
    LST_Destroy(&requestList);
}



static void
usageerror()
{
    char msg[] = "\
Usage: dump_commit_requests [-h] <fis database> \n\
\n\
  -h            Print help message\n\
\n\
  fis database  Name of the database to query\n";

    fprintf(stderr, msg);
    exit(1);
}

/* openFIS
**
** Purpose:
**	Open an FIS and return a handle to the caller.  If the open fails,
**	dump the error stack and exit.
**
** Parameter Dictionary:
**	fisDatabase	Name of the database to open
**	fisHandle	Handle to FIS returned to caller.
**
** Return Values:
**
**	void
**
** Notes:
**
** Algorithm:
*/

static void
openFIS(char *fisDatabase, FIS_HANDLE ** fisHandle)
{
    CONDITION cond = 0;

    cond = FIS_Open(fisDatabase, fisHandle);
    if (cond != FIS_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}

/* closeFIS
**
** Purpose:
**	Close an FIS.  The caller passes a reference that we use to close
**	the database.  If the close fails, dump the error stack and exit.
**
** Parameter Dictionary:
**	fisHandle	Handle to FIS passed by caller.
**
** Return Values:
**
**	void
**
** Notes:
**
** Algorithm:
*/

static void
closeFIS(FIS_HANDLE ** fisHandle)
{
    CONDITION cond = 0;

    cond = FIS_Close(fisHandle);
    if (cond != FIS_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}

main(int argc, char **argv)
{
    FIS_HANDLE *fisHandle = NULL;
    char *fisDatabase;

    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'h':
	    usageerror();
	    break;
	default:
	    break;
	}
    }

    if (argc < 1)
	usageerror();

    THR_Init();
    fisDatabase = *argv++;
    openFIS(fisDatabase, &fisHandle);

    dumpCommitRequests(&fisHandle);

    closeFIS(&fisHandle);
    THR_Shutdown();

    return 0;
}
