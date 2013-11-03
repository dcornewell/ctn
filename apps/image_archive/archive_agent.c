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
** Intent:		This program serves as an agent for the image archive.
**			It reads entries from a work queue and performs
**			operations necessary to complete the work.  In this
**			implementation, we handle:
**				storage commitment requests
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:37:36 $
** Source File:		$RCSfile: archive_agent.c,v $
** Revision:		$Revision: 1.11 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.11 $ $RCSfile: archive_agent.c,v $";

#include "ctn_os.h"

#if 0
#ifdef _WIN32
#include "io.h"
#include "winsock.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#endif

#include "dicom.h"
#include "condition.h"
#include "lst.h"
#ifdef CTN_MULTIBYTE
#include "tblmb.h"
#include "idbmb.h"
#else
#include "tbl.h"
#include "idb.h"
#endif
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "manage.h"
#include "fis.h"
#include "ctnthread.h"

#include "image_archive.h"
#include "commit_result.h"

CTNBOOLEAN silent = TRUE;
static CTNBOOLEAN verbose = FALSE;

static void
cleanupWorkList(LST_HEAD ** workList)
{
    FIS_ClearList(*workList);
    LST_Destroy(workList);
}


/* runAgentWorkLoop
**
** Purpose:
**	This is the driving function for this program.  It is an infinite
**	loop that reads the work queue and dispatches events.  If it
**	ever returns, it means that some type of error has occurred.
**
** Parameter Dictionary:
**	networkKey	A DICOM network key that allows us to initiate
**			associations.
**	dmanHandle	Handle for control database to be passed on
**			to the functions that do the work.
**	idbHandle	Handle for the image database to be passed on
**			to the functions that do the work.
**	fisHandle	Handle for the Information System.  We use this
**			to find the work records and also pass this
**			along to the dispatched functions.
**
** Return Values:
**	void
**
** Notes:
**
** Algorithm:
*/

static void
runAgentWorkLoop(DUL_NETWORKKEY ** networkKey,
		 DMAN_HANDLE ** dmanHandle, IDB_HANDLE ** idbHandle,
		 FIS_HANDLE ** fisHandle)
{
    LST_HEAD *workList;
    FIS_WORKQUEUERECORD *workQueue;
    CTNBOOLEAN done = FALSE;
    CONDITION cond;
    DUL_ASSOCIATIONKEY *associationKey;
    DUL_ASSOCIATESERVICEPARAMETERS params;

    workList = LST_Create();
    if (workList == NULL) {
	fprintf(stderr, "Could not create an initial work list. Exiting now.\n");
	return;
    }
    while (!done) {
	cond = FIS_GetWorkQueue(fisHandle, FIS_WQ_STORAGECOMMIT, NULL, 1,
				workList);
	if (cond != FIS_NORMAL) {
	    COND_DumpConditions();
	    cleanupWorkList(&workList);
	    return;
	}
	while ((workQueue = LST_Dequeue(&workList)) != NULL) {
	    printf("%s %s\n", workQueue->TasNam, workQueue->TasID);

	    cond = establishCommitResultAssociation(workQueue->TasID,
						networkKey, &associationKey,
					    &params, dmanHandle, fisHandle);
	    if (cond == APP_NORMAL)
		cond = processStorageCommitRequest(workQueue->TasID,
						   &associationKey, &params,
						   idbHandle, fisHandle);
	    (void) DUL_ReleaseAssociation(&associationKey);
	    if (cond == APP_NORMAL) {
		cond = FIS_DeleteWorkQueueRecord(fisHandle, workQueue->TasID);
		if (cond != FIS_NORMAL) {
		    COND_DumpConditions();
		    cleanupWorkList(&workList);
		    free(workQueue);
		    return;
		}
	    } else {
		COND_DumpConditions();
		free(workQueue);
	    }
	}
#ifdef _MSC_VER
	Sleep(15);
#else
	sleep(15);
#endif
    }
    cleanupWorkList(&workList);
}

static void
usageerror()
{
    char msg[] = "\
Usage: archive_agent [-d <dbname>] [-f <control>] [-h] [-i fis] \n\
\n\
  -d  Set image database name (default LTA_IDB)\n\
  -f  Set control database name (default CTNControl)\n\
  -h  Print help message and exit\n\
  -i  Set FIS database name (default LTA_FIS)\n";

    fprintf(stderr, msg);
    exit(1);
}

static void
openDatabase(const char *dbname, IDB_HANDLE ** handle)
{
    CONDITION cond = 0;

    cond = IDB_Open(dbname, handle);
    if (cond != IDB_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}

/* openControl
**
** Purpose:
**	Open the control database and pass a reference back to the caller.
**	If we are unsuccessful, dump the error stack and exit.
**
** Parameter Dictionary:
**	controlDatabase		Name of the control database.
**	dmanHandle		Reference to the database passed back to caller
**
** Return Values:
**	void
**
** Notes:
**
** Algorithm:
*/


static void
openControl(char *controlDatabase, DMAN_HANDLE ** dmanHandle)
{
    CONDITION cond = 0;

    cond = DMAN_Open(controlDatabase, "", "", dmanHandle);
    if (cond != DMAN_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}

/* openFIS
**
** Purpose:
**	Open the FIS database and pass a reference back to the caller.
**	If we are unsuccessful, dump the error stack and exit.
**
** Parameter Dictionary:
**	fisDatabase		Name of the FIS database.
**	fisHandle		Reference to the database passed back to caller
**
** Return Values:
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


/* closeDatabase
**
** Purpose:
**	Close the image database referenced by the caller's handle.
**
** Parameter Dictionary:
**	handle		A reference to an image database.
**
** Return Values:
**	void
**
** Notes:
**
** Algorithm:
*/

static void
closeDatabase(IDB_HANDLE ** handle)
{
    CONDITION cond = 0;

    cond = IDB_Close(handle);
    if (cond != IDB_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}

/* closeFIS
**
** Purpose:
**	Close the FIS database referenced by the caller's handle.
**
** Parameter Dictionary:
**	handle		A reference to an FIS database.
**
** Return Values:
**	void
**
** Notes:
**
** Algorithm:
*/

static void
closeFIS(FIS_HANDLE ** handle)
{
    CONDITION cond = 0;

    cond = FIS_Close(handle);
    if (cond != FIS_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}

/* closeControl
**
** Purpose:
**	Close the control database referenced by the caller's handle.
**
** Parameter Dictionary:
**	handle		A reference to a control database.
**
** Return Values:
**	void
**
** Notes:
**
** Algorithm:
*/

static void
closeControl(DMAN_HANDLE ** handle)
{
    CONDITION cond = 0;

    cond = DMAN_Close(handle);
    if (cond != DMAN_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}




main(int argc, char **argv)
{
    CONDITION
	cond;

    DMAN_HANDLE *dmanHandle = NULL;
    IDB_HANDLE *idbHandle = NULL;
    FIS_HANDLE *fisHandle = NULL;
    char *controlDatabase = "CTNControl";
    static char *dbname = "LTA_IDB";
    char *fisDatabase = "LTA_FIS";
    DUL_NETWORKKEY *networkKey;

    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'd':
	    argc--;
	    argv++;
	    if (argc < 1)
		usageerror();
	    dbname = *argv;
	    break;
	case 'f':
	    argc--;
	    argv++;
	    if (argc < 1)
		usageerror();
	    controlDatabase = *argv;
	    break;
	case 'h':
	    usageerror();
	    break;
	case 'i':
	    argc--;
	    argv++;
	    if (argc < 1)
		usageerror();
	    fisDatabase = *argv;
	    break;
	case 'v':
	    verbose = TRUE;
	    break;
	default:
	    break;
	}
    }

    THR_Init();
    openDatabase(dbname, &idbHandle);
    openControl(controlDatabase, &dmanHandle);
    openFIS(fisDatabase, &fisHandle);

    cond = DUL_InitializeNetwork(DUL_NETWORK_TCP, DUL_AEREQUESTOR,
			NULL, DUL_TIMEOUT, DUL_ORDERBIGENDIAN, &networkKey);
    if (cond != DUL_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
    /*
     * We don't expect to return from this function except when an * error
     * occurs.
     */
    runAgentWorkLoop(&networkKey, &dmanHandle, &idbHandle, &fisHandle);

    closeDatabase(&idbHandle);
    closeFIS(&fisHandle);
    closeControl(&dmanHandle);
    THR_Shutdown();

    COND_DumpConditions();
    return 0;
}
