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
+-+-+-+-+-+-+-+-+-
*/
/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	print_server.c
** Author, Date:	Aniruddha S. Gokhale, 23-June-94
** Intent:		This module implements the main program of the
**			print_server i.e. The SCP side of the PRINT
**			Management Service. This module implements
**			a concurrent server, but provides a switch that
**			can make the server iterative.
** Last Update:		$Author: smm $, $Date: 1998-08-03 21:33:41 $
** Source File:		$RCSfile: print_server.c,v $
** Revision:		$Revision: 1.28 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.28 $ $RCSfile: print_server.c,v $";
#include "print_server.h"
#ifdef _MSC_VER
#define MAXHOSTNAMELEN 256
#endif

static void usageError();
static void signalHandler();
static CONDITION
addChildProcess(DUL_ASSOCIATESERVICEPARAMETERS * service,
		int pid, LST_HEAD ** list);
static CONDITION harvestChildrenProcesses(LST_HEAD ** list);
static CONDITION openGQ(int gqID);
void exitApplication(CONDITION cond);
void clearAssociationKeyAndServiceParameters(CONDITION cond);

int
    maxPDU = 16384;
char *controlDatabase = "CTNControl";	/* name of database holding
					 * configuration information */
CTNBOOLEAN
sendBack = FALSE,		/* send attributes back in response messages */
traceFlag = FALSE,		/* whether or not silent mode is to be used */
gqueueFlag = FALSE,		/* indicates if GQ queue will be used */
verboseDUL = FALSE,		/* these three flags put the three facilities */
verboseSRV = FALSE,		/* in debug mode */
verboseDCM = FALSE;
static CTNBOOLEAN
    forgiveFlag = FALSE;	/* to indicate that some inconsistencies in
				 * the Association request may be tolerated */
BFS_NODE
bfsInstance;			/* track the single FILM SESSION instance */
BFB_NODE
* bfbInstance = NULL;		/* track last created film box */
PRINT_SERVER_HANDLES
appHandles = {
    -1, NULL, NULL, NULL, NULL
};
int gqID = -1;			/* unique ID identifying the Generalized */
 /* Queue */

#ifdef _MSC_VER
/* fork
**
** Purpose:
**	Dummy function in the W32 environment.  This "fork" just returns
**	0 to make it look like this is the child process in a Unix fork.
**
** Parameter Dictionary:
**	None
**
** Return Values:
**	0
**
** Notes:
**
*/

static int
fork()
{
    return 0;
}
#endif
 /*
  * The structure holding pointers to all the handles used by the application
  */

int
main(int argc, char **argv)
{
    CONDITION
    cond = APP_NORMAL;		/* condition code returned by various
				 * facilities */
    DUL_NETWORKKEY
	* network;		/* The handle to the network */
    DUL_ASSOCIATIONKEY
	* association = NULL;	/* The handle to the Association */
    DUL_ASSOCIATESERVICEPARAMETERS
	service;		/* Presentation parameters */
    int
        pid,			/* process id */
        port;			/* port number on which the server listens */
    char
        node[MAXHOSTNAMELEN + 1] = "";	/* name of node */
    CTNBOOLEAN
	singleUserMode = FALSE;	/* indicates if the server will run in single
				 * user mode i.e. iterative */
    LST_HEAD
	* processList = NULL;	/* maintains a list of children processes */
    CTNBOOLEAN paramsFlag = FALSE;	/* Dump association parameters? */

    (void) gethostname(node, MAXHOSTNAMELEN);
    /* Parse the command line arguments. First process all the switches */
    while (--argc > 0 && *(++argv)[0] == '-') {
	switch ((*argv)[1]) {
	case 'd':		/* option to put a specific facility in debug
				 * mode */
	    argc--;
	    argv++;
	    if (!argc)
		usageError();
	    if (strcmp(*argv, "DCM") == 0)
		verboseDCM = TRUE;
	    else if (strcmp(*argv, "DUL") == 0)
		verboseDUL = TRUE;
	    else if (strcmp(*argv, "SRV") == 0)
		verboseSRV = TRUE;
	    else
		usageError();
	    break;
	case 'f':		/* database selection option */
	    argc--;
	    argv++;
	    if (!argc)
		usageError();
	    controlDatabase = *argv;
	    break;
	case 'i':		/* set the forgive option */
	    forgiveFlag = TRUE;
	    break;
	case 'n':		/* use node as name rather than hostname */
	    if (argc < 1)
		usageError();
	    argc--;
	    argv++;
	    strcpy(node, *argv);
	    break;
	case 'p':
	    paramsFlag = TRUE;	/* Dump the association parameters to stdout */
	    break;
	case 'r':
	    sendBack = TRUE;	/* send optional attributes back in the
				 * response messages */
	    break;
	case 's':		/* set single user mode. Useful for debugging */
	    singleUserMode = TRUE;
	    break;
	case 't':
	    traceFlag = TRUE;	/* tracing ON (non silent operation) */
	    break;
	case 'v':		/* set verbose mode ON */
	    verboseDUL = TRUE;
	    verboseSRV = TRUE;
	    verboseDCM = TRUE;
	    break;
	case 'x':		/* for maintaining a Generalized Queue. This
				 * option is useful for terminals having X
				 * capability */
	    gqueueFlag = TRUE;
#ifdef ASG
	    argc--;
	    argv++;
	    if (sscanf(*argv, "%d", &gqID) != 1)
		usageError();
#endif
	    break;
	default:
	    printf("Unrecognized option: %s\n", *argv);
	    break;
	}
    }

    if (argc < 1)
	usageError();

    if (sscanf(*argv++, "%d", &port) != 1)
	usageError();

#ifdef _MSC_VER
    singleUserMode = TRUE;
#endif

    THR_Init();
    DCM_Debug(verboseDCM);
    DUL_Debug(verboseDUL);
    SRV_Debug(verboseSRV);

    /* Handle interrupts */
    (void) signal(SIGINT, signalHandler);

    /* Initialize a network connection and listen on the specified port */
    cond = DUL_InitializeNetwork(DUL_NETWORK_TCP, DUL_AEBOTH,
		 (void *) &port, DUL_TIMEOUT, DUL_ORDERBIGENDIAN, &network);
    if (cond != DUL_NORMAL) {
	exitApplication(cond);
    }
    appHandles.network = network;	/* set the network key field */
    if (!singleUserMode) {
	/*
	 * initialize a list that will hold information about all the
	 * children processes that are active
	 */
	if ((processList = LST_Create()) == NULL) {
	    cond = COND_PushCondition(APP_ERROR(APP_FAILURE), "LST_Create",
				      "main");
	    exitApplication(cond);
	}
	appHandles.processList = processList;	/* set the process list field */
    }
    /* The server loops forever accepting new requests */
    while (1) {
	/* get the next Association request */
	cond = nextAssociationRequest(node, &network, &service, maxPDU,
				      forgiveFlag, &gqID, &association);
	if (cond == APP_NORMAL) {
	    appHandles.association = association;
	    appHandles.service = &service;
	    if (!singleUserMode) {
		/*
		 * remove all those child processes that have exited so that
		 * zombie processes are not created
		 */
		(void) harvestChildrenProcesses(&processList);
		/*
		 * server continues to accept new requests, whereas the child
		 * services the requests on that association. To create a
		 * child process, we fork
		 */
		pid = fork();
	    } else
		pid = 0;	/* setting pid to 0 in this else clause is
				 * essential due to the manner in which the
				 * following if condition is coded */

	    if (pid < 0) {
		/* fork failed */
		printf("Cannot spawn child process. Request rejected\n");
		clearAssociationKeyAndServiceParameters(APP_FAILURE);
		continue;
	    } else if (pid == 0) {	/* This is the child process unless
					 * the single user mode is ON in
					 * which case this is the parent
					 * process itself. For this part of
					 * the code to be activated when the
					 * singleUserMode is TRUE, we need to
					 * set pid = 0 as done above */
		if (!singleUserMode)
		    printf("Forked child\n");
		cond = DUL_AcknowledgeAssociationRQ(&association, &service);
		if (cond != DUL_NORMAL) {
		    if (!singleUserMode) {
			/* child process exits with a status of -1 */
			exitApplication(cond);
		    } else {
			/*
			 * In the single user mode, the parent just discards
			 * the current association and service parameters and
			 * continues to listen to new requests
			 */
			clearAssociationKeyAndServiceParameters(cond);
			continue;	/* go to next iteration of main loop */
		    }
		}
		/*
		 * open a GQ queue, if the GQ facility is to be used
		 */
		if (gqueueFlag) {
		    if (gqID == -1) {	/* we were unsuccessful retrieving
					 * the GQ ID info from the database.
					 * Hence we decide to ignore the
					 * display */
			fprintf(stderr,
				"Failure to retrieve GQ ID information\n");
			fprintf(stderr,
				"GQ facility ignored\n");
			gqueueFlag = FALSE;
		    } else {
			/* open a GQ */
			cond = openGQ(gqID);
			if (cond != APP_NORMAL) {
			    if (!singleUserMode) {
				/* child process exits with a status of -1 */
				exitApplication(cond);
			    } else {
				clearAssociationKeyAndServiceParameters(cond);
				continue;	/* go to next iteration of
						 * main loop */
			    }
			}
		    }
		}
		if (paramsFlag)
		    DUL_DumpParams(&service);
		cond = serviceRequests(&network, &association, &service);
		if (cond == SRV_PEERREQUESTEDRELEASE)
		    cond = SRV_NORMAL;
		if (CTN_ERROR(cond)) {
		    if (!singleUserMode) {
			fprintf(stderr, "child failed to service request\n");
			COND_DumpConditions();
			exitApplication(cond);
		    } else {
			/*
			 * In the single user mode, the parent just discards
			 * the current association and service parameters and
			 * continues to listen to new requests
			 */
			fprintf(stderr,
			      "Iterative server failed to serve request\n");
			clearAssociationKeyAndServiceParameters(cond);
			continue;	/* go to next iteration of main loop */
		    }
		}
		if (!singleUserMode) {
		    /* graceful exit by the child process (return status 0) */
		    exitApplication(SRV_NORMAL);
		}
	    } else {
		/*
		 * Parent has to drop the Association that was created for
		 * the child when an Association request arrived from the
		 * client. If this is not done, both the parent and the child
		 * are capable of receiving on the same association
		 */
		cond = DUL_DropAssociation(&association);
		if (cond != DUL_NORMAL) {
		    exitApplication(cond);
		}
		/*
		 * Add information of the process to the list maintained by
		 * the parent
		 */
		(void) addChildProcess(&service, pid, &processList);
		/*
		 * Now clear the service parameters so as to accept a new set
		 * of parameters on the association
		 */
		(void) DUL_ClearServiceParameters(&service);
		printf("Parent ready to accept new request\n");
	    }
	} else {
	    /*
	     * something went wrong accepting the next Association request
	     * Parent server issues an error message and continues to serve
	     * the next request
	     */
	    fprintf(stderr, "!!!! Error getting next Request.\n");
	    fprintf(stderr, "Discarding this request\n");
	    COND_DumpConditions();
	}
    }
#ifdef MALLOC_DEBUG
    malloc_verify(0);
    malloc_shutdown();
#endif
    return 0;
}

/* usageError
**
** Purpose:
**	Dumps the optional and mandatory switches required for executing
**	the command
**
** Parameter Dictionary:
**	None
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static void
usageError()
{
    char msg[] = "\
Usage: print_server [-d <facility>] [-f database] [-i] [-n node] [-p] [-r] [-s] [-t] [-x] [-v] port \n\n\
    -d  Put the specified facility in verbose mode\n\
    -f  Use database as control database rather than default (CTNControl)\n\
    -i  Ignore some problems in Association Request\n\
    -n  Use node as name rather than hostname\n\
    -p  Print association parameters\n\
    -r  Send optional attributes back in the response messages\n\
    -s  Use single user mode i.e. iterative server\n\
    -t  Tracing ON\n\
    -x  Use the GQ facility to maintain a queue\n\
    -v  Place DUL and SRV facilities in verbose mode\n\
\n\
    port  TCP/IP port address\n";

    fprintf(stderr, msg);
    exit(1);
}

/* addChildProcess
**
** Purpose:
**	Adds information about a child to the process list maintained by
**	the parent
**
** Parameter Dictionary:
**	service		Handle to the service parameters
**	pid		Process ID of the child
**	list		Handle to the list of processes
**
** Return Values:
**	APP_NORMAL
**	APP_MALLOCFAILURE
**	APP_FAILURE
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
addChildProcess(DUL_ASSOCIATESERVICEPARAMETERS * service,
		int pid, LST_HEAD ** list)
{
    PROCESS_ELEMENT
    * e;

    if ((e = malloc(sizeof(PROCESS_ELEMENT))) == NULL) {
	return COND_PushCondition(APP_ERROR(APP_MALLOCFAILURE));
    }
    strcpy(e->callingAPTitle, service->callingAPTitle);
    strcpy(e->calledAPTitle, service->calledAPTitle);
    e->pid = pid;

    if (LST_Enqueue(list, e) != LST_NORMAL) {
	return COND_PushCondition(APP_ERROR(APP_FAILURE), "LST_Enqueue",
				  "addChildProcess");
    }
    return APP_NORMAL;
}

/* harvestChildrenProcesses
**
** Purpose:
**	The purpose of this function is to make sure that the exiting
**	child processes do not become zombies. Secondly, we also remove
**	the entry of the exiting process from the list we maintain.
**
** Parameter Dictionary:
**	list		Handle to the list of processes
**
** Return Values:
**	APP_NORMAL
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
harvestChildrenProcesses(LST_HEAD ** list)
{
#ifndef _MSC_VER
    int
        pid;
    int
        statusp;
    PROCESS_ELEMENT
	* e;

    /* Continue as long as status is available from any of the child proceses */
    while ((pid = waitpid(-1, &statusp, WNOHANG)) > 0) {
	e = LST_Head(list);
	if (e != NULL)
	    (void) LST_Position(list, e);
	while (e != NULL) {
	    if (e->pid == pid) {
		printf("\n\n");
		printf("***Child Exit Status*****************************\n");
		printf("\tProcess ID #%d\n", pid);
		printf("\tCalling APP Title : %s\n", e->callingAPTitle);
		printf("\tCalled APP Title : %s\n", e->calledAPTitle);
		printf("\tExit status : %s\n", (statusp ? "ERROR" : "SUCCESS"));
		printf("*************************************************\n");
		printf("\n\n");
		(void) LST_Remove(list, LST_K_BEFORE);
		free(e);
		e = NULL;
	    } else {
		e = LST_Next(list);
	    }
	}
    }
#endif
    return APP_NORMAL;
}

/* signalHandler
**
** Purpose:
**	Handle any kill interrupts i.e. pressing control-C
**
** Parameter Dictionary:
**	None
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static void
signalHandler()
{
    CONDITION
    cond;

    fprintf(stderr, "\nProcess Killed\n");
    cond = COND_PushCondition(APP_ERROR(APP_OPERATIONINTERRUPTED));
    exitApplication(cond);
}

/* exitApplication
**
** Purpose:
**	Exit the application in presence of some errors. Before exiting,
**	free all the handles which were allocated to the application
**
** Parameter Dictionary:
**	cond	The condition that caused the app to exit
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
void
exitApplication(CONDITION cond)
{
    CONDITION
    rtnCond;

    fprintf(stderr, "Freeing all handles\n");
    /* free all handles here */
#ifdef ASG
    if (appHandles.gqID != -1) {
	rtnCond = kill_queue(appHandles.gqID);
	if (CTN_ERROR(rtnCond)) {
	    fprintf(stderr, "APP Error freeing generalized queue\n");
	    COND_DumpConditions();
	}
    }
#endif
    if (appHandles.network) {
	rtnCond = DUL_DropNetwork(&appHandles.network);
	if (CTN_ERROR(rtnCond)) {
	    fprintf(stderr, "APP Error freeing network key\n");
	    COND_DumpConditions();
	}
    }
    if (appHandles.association) {
	rtnCond = DUL_DropAssociation(&appHandles.association);
	if (CTN_ERROR(rtnCond)) {
	    fprintf(stderr, "APP Error freeing association key\n");
	    COND_DumpConditions();
	}
    }
    if (appHandles.service) {
	rtnCond = DUL_ClearServiceParameters(appHandles.service);
	if (CTN_ERROR(rtnCond)) {
	    fprintf(stderr, "APP Error clearing service parameters\n");
	    COND_DumpConditions();
	}
    }
    if (appHandles.processList) {
	PROCESS_ELEMENT
	    * e = NULL;
	e = LST_Head(&appHandles.processList);
	if (e != NULL)
	    (void) LST_Position(&appHandles.processList, e);
	while (e != NULL) {
	    free(e);
	    e = LST_Dequeue(&appHandles.processList);
	}
	rtnCond = LST_Destroy(&appHandles.processList);
	if (CTN_ERROR(rtnCond)) {
	    fprintf(stderr, "APP Error freeing Process List\n");
	    COND_DumpConditions();
	}
    }
    /* Now the final exit */
    if (CTN_ERROR(cond)) {
	fprintf(stderr, "\n\n");
	fprintf(stderr, "APP Abnormal Exit\n");
	COND_DumpConditions();
	fprintf(stderr, "\n\n");
	exit(1);		/* error exit status */
    }
    fprintf(stderr, "\n\n");
    exit(0);			/* normal exit */
}

void
clearAssociationKeyAndServiceParameters(CONDITION cond)
{
    CONDITION
    rtnCond;

    if (appHandles.association) {
	rtnCond = DUL_DropAssociation(&appHandles.association);
	if (CTN_ERROR(rtnCond)) {
	    fprintf(stderr, "APP Error freeing association key\n");
	    COND_DumpConditions();
	}
    }
    if (appHandles.service) {
	rtnCond = DUL_ClearServiceParameters(appHandles.service);
	if (CTN_ERROR(rtnCond)) {
	    fprintf(stderr, "APP Error clearing service parameters\n");
	    COND_DumpConditions();
	}
    }
}

static CONDITION
openGQ(int qID)
{
    CONDITION
    cond;

    /* Get a new or existing Queue for the GQ operations */
    if (gqueueFlag) {
#ifdef ASG
	cond = GQ_GetQueue(qID, sizeof(GQ_ELEM));
	if (cond != GQ_NORMAL) {
	    (void) COND_PopCondition(TRUE);	/* necessary to clear the
						 * stack since, this is not
						 * an error and any
						 * conditions dumped later,
						 * unnecessarily displays
						 * this message */
	    /* The Q may not be existing, in which case we create one */
	    cond = GQ_InitQueue(qID, 128, sizeof(GQ_ELEM));
	    if (cond != GQ_NORMAL) {
		return COND_PushCondition(APP_ERROR(APP_FAILURE),
					  "GQ_InitQueue", "main");
	    }
	}
	appHandles.gqID = qID;	/* set the gqID field */

	/* Now again invoke GetQueue */
#endif
	/*
	 * We try to open an existing Queue. It is assumed that an
	 * accompanying program viz. print_server_display will be running
	 * with the desired GQ already created
	 */
	cond = GQ_GetQueue(qID, sizeof(GQ_ELEM));
	if (cond != GQ_NORMAL) {
	    return COND_PushCondition(APP_ERROR(APP_FAILURE),
				      "GQ_GetQueue", "main");
	}
    }
    return APP_NORMAL;
}
