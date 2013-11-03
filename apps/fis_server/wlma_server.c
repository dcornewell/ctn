/*
+-+-+-+-+-+-+-+-+-
*/
/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	Stephen Moore, 11-Feb-94
** Intent:		The main program for the WLMA server.  This application
**			accepts Association requests from nodes and performs
**			some HIS queries and updates.
** Last Update:		$Author: smm $, $Date: 1994-09-23 20:01:49 $
** Source File:		$RCSfile: wlma_server.c,v $
** Revision:		$Revision: 1.2 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.2 $ $RCSfile: wlma_server.c,v $";

#include <stdio.h>
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
#include <sys/wait.h>
#ifdef MALLOC_DEBUG
#include "malloc.h"
#endif

#include "dicom.h"
#include "tbl.h"
#include "lst.h"
#include "dicom_uids.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "tbl.h"
#include "manage.h"

#include "wlma_server.h"

static void usageerror();
static CONDITION
addChildProcess(DUL_ASSOCIATESERVICEPARAMETERS * service,
		int pid, LST_HEAD ** list);
static CONDITION harvestChildrenProcesses(LST_HEAD ** list);
int
    maxPDU = 16384;
BOOLEAN
verboseDUL = FALSE;
char *controlDatabase = "CTNControl";
static BOOLEAN
    forgiveFlag = FALSE;

main(int argc, char **argv)
{
    CONDITION
	cond;
    DUL_NETWORKKEY
	* network;
    DUL_ASSOCIATIONKEY
	* association = NULL;
    DUL_ASSOCIATESERVICEPARAMETERS
	service;
    DUL_PRESENTATIONCONTEXT
	* requestedCtx;
    int
        pid,
        port;
    char
        node[MAXHOSTNAMELEN + 1] = "";
    BOOLEAN
	singleUserMode = FALSE;
    LST_HEAD
	* processList = NULL;
    DMAN_HANDLE
	* dmanHandle;

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
	case 's':
	    singleUserMode = TRUE;
	    break;
	case 'v':
	    verboseDUL = TRUE;
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

    cond = DUL_InitializeNetwork(DUL_NETWORK_TCP, DUL_AEBOTH,
				 (void *) &port, DUL_TIMEOUT,
			 DUL_ORDERBIGENDIAN | DUL_FULLDOMAINNAME, &network);
    if (cond != DUL_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
    if ((processList = LST_Create()) == NULL) {
	fprintf(stderr, "Unable to create list for process elements\n");
	exit(1);
    }
    while (!ERROR(cond)) {
	cond = nextAssociationRequest(node, &network, &service, maxPDU,
				      forgiveFlag, &association);
	if (cond == APP_NORMAL) {
	    if (!singleUserMode) {
		(void) harvestChildrenProcesses(&processList);
		pid = fork();
	    } else
		pid = 0;

	    if (pid == 0) {
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
		(void) DUL_DropAssociation(&association);
		(void) addChildProcess(&service, pid, &processList);
		printf("Parent\n");
	    }
	} else {
	    COND_DumpConditions();
	}
    }
    COND_DumpConditions();
    printf("Exiting\n");
#ifdef MALLOC_DEBUG
    malloc_verify(0);
    malloc_shutdown();
#endif
    exit(0);
}

static void
usageerror()
{
    char msg[] = "\
Usage: wlma_server [-f db] [-i] [-n node] [-v] port\n\
\n\
    -f    Use db as control database instead of default (CTNControl)\n\
    -i    Ignore some problems in Association Request\n\
    -n    Use node as name rather than hostname\n\
    -v    Place DUL and SRV facilities in verbose mode\n\
\n\
    port  TCP/IP port address\n";

    fprintf(stderr, msg);
    exit(1);
}

static CONDITION
addChildProcess(DUL_ASSOCIATESERVICEPARAMETERS * service,
		int pid, LST_HEAD ** list)
{
    PROCESS_ELEMENT
    * e;

    if ((e = malloc(sizeof(*e))) == NULL)
	return 0;
    strcpy(e->callingAPTitle, service->callingAPTitle);
    strcpy(e->calledAPTitle, service->calledAPTitle);
    e->pid = pid;

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
}
