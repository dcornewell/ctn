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
**				DINPACS 97
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	Stephen Moore, 22-Mar-1997
** Intent:		commit_agent is a server process that listens
**			for storage commit event notifications.
** Last Update:		$Author: smm $, $Date: 2001-12-27 19:30:47 $
** Source File:		$RCSfile: commit_agent.c,v $
** Revision:		$Revision: 1.5 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.5 $ $RCSfile: commit_agent.c,v $";

#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/param.h>
#ifdef SOLARIS
#include <netdb.h>
#endif
#include <stdlib.h>
#include <string.h>
#ifdef MACH
#include <unistd.h>
#endif
/*lint -e46*/
#include <sys/wait.h>
/*lint +e46*/
#ifdef MALLOC_DEBUG
#include "malloc.h"
#endif
#endif

#include "dicom.h"
#include "ctnthread.h"
#include "condition.h"
#include "tbl.h"
#include "lst.h"
#include "dicom_uids.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "tbl.h"
#include "manage.h"
#include "idb.h"
#include "utility.h"

#include "storage_commit.h"

static void usageerror();
static CONDITION
    addChildProcess(PROCESS_ELEMENT * processElement, LST_HEAD ** list);
static CONDITION harvestChildrenProcesses(LST_HEAD ** list);
static CONDITION
checkOrganizationConnections(char *organization,
			     LST_HEAD ** l, int connections);
int maxPDU = 16384;
CTNBOOLEAN verboseDUL = FALSE;
CTNBOOLEAN verboseTBL = FALSE;
CTNBOOLEAN verboseSRV = FALSE;
CTNBOOLEAN silent = FALSE;
char *controlDatabase = "CTNControl";
static char *logFile = NULL;
static CTNBOOLEAN forgiveFlag = FALSE;

CTNBOOLEAN doVerification = FALSE;

main(int argc, char **argv)
{
    CONDITION cond;
    DUL_NETWORKKEY *network;
    DUL_ASSOCIATIONKEY *association = NULL;
    DUL_ASSOCIATESERVICEPARAMETERS service;
    DUL_PRESENTATIONCONTEXT *requestedCtx;
    int pid;
    int port;
    char node[MAXHOSTNAMELEN + 1] = "";
    CTNBOOLEAN singleUserMode = FALSE;
    LST_HEAD *processList = NULL;
    DMAN_HANDLE *dmanHandle;
    char uidError[] = "\
To use this port (%d), you must run as root or the application\n\
must be setuid root (see chmod)\n";
    char currentDate[DICOM_DA_LENGTH + 1];
    char currentTime[DICOM_TM_LENGTH + 1];
    char *logString = "%-10s (%s %s %s) (%s %s) %s %s %d\n";
    int maxPerOrganization = 1000;
    PROCESS_ELEMENT processElement;

    (void) gethostname(node, MAXHOSTNAMELEN);
    while (--argc > 0 && *(++argv)[0] == '-') {
	switch ((*argv)[1]) {
	case 'f':
	    if (argc < 1)
		usageerror();
	    argc--;
	    argv++;
	    controlDatabase = *argv;
	    break;
	case 'l':
	    if (argc < 1)
		usageerror();
	    argc--;
	    argv++;
	    logFile = *argv;
	    break;
	case 'i':
	    forgiveFlag = TRUE;
	    break;
	case 'n':
	    if (argc < 1)
		usageerror();
	    argc--;
	    argv++;
	    strcpy(node, *argv);
	    break;
	case 'q':
	    silent = TRUE;
	    break;
	case 's':
	    singleUserMode = TRUE;
	    break;
	case 'v':
	    verboseDUL = TRUE;
	    break;
	case 'x':
	    if (--argc < 1)
		usageerror();
	    argv++;
	    if (strcmp(*argv, "TBL") == 0)
		verboseTBL = TRUE;
	    else if (strcmp(*argv, "SRV") == 0)
		verboseSRV = TRUE;
	    else
		usageerror();
	    break;
	default:
	    printf("Unrecognized option: %s\n", *argv);
	    break;
	}
    }

    if (argc < 1)
	usageerror();

    if (sscanf(*argv++, "%d", &port) != 1)
	usageerror();

    (void) signal(SIGUSR1, SIG_IGN);
    if (port < 1024) {
	if (geteuid() != 0) {
	    fprintf(stderr, uidError, port);
	    return (1);
	}
    }
    THR_Init();
    (void) TBL_Debug(verboseTBL);
    DUL_Debug(verboseDUL);
    SRV_Debug(verboseSRV);

    cond = DUL_InitializeNetwork(DUL_NETWORK_TCP, DUL_AEACCEPTOR,
				 (void *) &port, DUL_TIMEOUT,
			 DUL_ORDERBIGENDIAN | DUL_FULLDOMAINNAME, &network);
    if (cond != DUL_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	return (2);
    }
    setuid(getuid());		/* Return to proper uid */

    if ((processList = LST_Create()) == NULL) {
	fprintf(stderr, "Unable to create list for process elements\n");
	THR_Shutdown();
	return (3);
    }
    while (!CTN_ERROR(cond)) {
	cond = nextAssociationRequest(node, &network, &service, maxPDU,
				      forgiveFlag, &association,
				      &processElement);
	UTL_GetDicomDate(currentDate);
	UTL_GetDicomTime(currentTime);

	if (!singleUserMode) {
	    (void) harvestChildrenProcesses(&processList);
	}
	if (cond == APP_NORMAL) {
	    DUL_ABORTITEMS abortItems;
	    cond = checkOrganizationConnections(processElement.organization,
					  &processList, maxPerOrganization);
	    if (cond != APP_NORMAL) {
		abortItems.result = DUL_REJECT_TRANSIENT;
		abortItems.source = DUL_ULSU_REJECT;
		abortItems.reason = DUL_ULSU_REJ_NOREASON;
		(void) DUL_RejectAssociationRQ(&association, &abortItems);
	    }
	}
	if (cond == APP_NORMAL) {
	    if (!singleUserMode) {
		pid = fork();
	    } else
		pid = 0;

	    if (pid == 0) {
		if (!silent)
		    printf("Forked child\n");
		cond = DUL_AcknowledgeAssociationRQ(&association, &service);
		if (cond != DUL_NORMAL)
		    continue;

		cond = serviceRequests(&network, &association, &service);
		if (cond == SRV_PEERREQUESTEDRELEASE)
		    cond = SRV_NORMAL;
		if (!singleUserMode) {
		    break;
		}
	    } else {
		logMessage(logString, "ACCEPTED", service.callingAPTitle,
			   service.callingPresentationAddress,
			   processElement.organization,
			   service.calledAPTitle,
			   service.calledPresentationAddress,
			   currentDate, currentTime,
			   pid);

		(void) DUL_DropAssociation(&association);
		processElement.pid = pid;
		(void) addChildProcess(&processElement, &processList);
		if (!silent)
		    printf("Parent\n");
	    }
	} else {
	    logMessage(logString, "REJECTED", service.callingAPTitle,
		       service.callingPresentationAddress,
		       processElement.organization,
		       service.calledAPTitle,
		       service.calledPresentationAddress,
		       currentDate, currentTime,
		       0);
	    COND_DumpConditions();
	}
    }
    COND_DumpConditions();
    if (!silent)
	printf("Exiting\n");
#ifdef MALLOC_DEBUG
    malloc_verify(0);
    malloc_shutdown();
#endif
    THR_Shutdown();
    return (0);
}

static void
usageerror()
{
    char msg[] = "\
Usage: [-f db] [-i] [-l logfile] [-n node] [-q] [-v] [-x <fac>] port\n\
\n\
    -f    Set control database (default CTNControl)\n\
    -i    Ignore some problems in Association Request\n\
    -l    Place log of association requests in <logfile>\n\
    -n    Use node as name rather than hostname\n\
    -q    Quiet mode, don't dump a lot of messages to terminal\n\
    -v    Place DUL facility in verbose mode\n\
    -x    Place <fac> in verbose mode (<fac> = TBL, SRV)\n\
\n\
    port  TCP/IP port address\n";

    fprintf(stderr, msg);
    exit(5);
}

static CONDITION
addChildProcess(PROCESS_ELEMENT * processElement, LST_HEAD ** list)
{
    PROCESS_ELEMENT
    * e;

    if ((e = malloc(sizeof(*e))) == NULL)
	return 0;
    *e = *processElement;

    if (LST_Enqueue(list, e) != LST_NORMAL)
	return 0;

    return APP_NORMAL;
}

static CONDITION
harvestChildrenProcesses(LST_HEAD ** list)
{
    int
        pid;
    PROCESS_ELEMENT
	* e;

    while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
	e = LST_Head(list);
	if (e != NULL)
	    (void) LST_Position(list, e);
	while (e != NULL) {
	    if (!silent)
		printf("%5d %s\n", e->pid, e->calledAPTitle);
	    if (e->pid == pid) {
		(void) LST_Remove(list, LST_K_BEFORE);
		free(e);
		e = NULL;
	    } else {
		e = LST_Next(list);
	    }
	}
    }
    return 1;
}

static CONDITION
checkOrganizationConnections(char *organization,
			     LST_HEAD ** l, int connections)
{
    PROCESS_ELEMENT *e;

    e = LST_Head(l);
    (void) LST_Position(l, e);

    while (e != NULL && connections > 0) {
	if (strcmp(e->organization, organization) == 0)
	    connections--;

	e = LST_Next(l);
    }

    if (connections > 0)
	return APP_NORMAL;
    else
	return 0;
}

void
logMessage(char *controlString,...)
{
    FILE *fp;
    va_list args;

    if (logFile == NULL)
	fp = stdout;
    else
	fp = fopen(logFile, "a");

    if (fp == NULL)
	return;

/*lint -e40 -e50 */
    va_start(args, controlString);
    if (controlString == NULL)
	controlString = "NULL Control string passedto PushCondition";
    (void) vfprintf(fp, controlString, args);
    (void) fflush(fp);
    va_end(args);
/*lint +e40 +e50 */

    if (logFile != NULL)
	(void) fclose(fp);
}
