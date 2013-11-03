/*
          Copyright (C) 1993, RSNA and Washington University

          The software and supporting documentation for the Radiological
          Society of North America (RSNA) 1993 Digital Imaging and
          Communications in Medicine (DICOM) Demonstration were developed
          at the
                  Electronic Radiology Laboratory
                  Mallinckrodt Institute of Radiology
                  Washington University School of Medicine
                  510 S. Kingshighway Blvd.
                  St. Louis, MO 63110
          as part of the 1993 DICOM Central Test Node project for, and
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
/*
** @$=@$=@$=
*/
/*
**				DICOM 93
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	main
**			usageerror
**			myExit
**			requestAssociation
**			sendQuery
**			queryCallback
** Author, Date:	Stephen M. Moore, Long Duan 6-May-93
** Intent:		This program is an example and test program which
**			demonstrates the DICOM packages developed at MIR.
**			It establishes an Association with a server and sends
**			a query request to the server.
**  Usage:
**	patient_query [-c calling title] [-t title] [-v] [-x] node port query
**  Options:
**	-c	Calling AP title to use during Association setup
**	-t	Called AP title to use during Association setup
**	-x	Cancel find request after first callback
**	-v	Verbose mode for DUL facility
**	node	Host node name where server is running
**	port	TCP/IP Port number of server
**	query	DICOM IOD file with query
**
** Last Update:		$Author: smm $, $Date: 1997-04-17 18:44:16 $
** Source File:		$RCSfile: patient_query.c,v $
** Revision:		$Revision: 1.2 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.2 $ $RCSfile: patient_query.c,v $";

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "dicom.h"
#include "dicom_uids.h"
#include "lst.h"
#include "condition.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"

static void usageerror();
static void myExit(DUL_ASSOCIATIONKEY ** association);
static int
requestAssociation(DUL_NETWORKKEY ** network,
		   DUL_ASSOCIATIONKEY ** association,
		   DUL_ASSOCIATESERVICEPARAMETERS * params,
		   char *SOPClass);
static void
sendQuery(DUL_NETWORKKEY ** network,
	DUL_ASSOCIATESERVICEPARAMETERS * params, DCM_OBJECT ** queryObject);
static CONDITION
queryCallback(MSG_C_FIND_REQ * findRequest,
	      MSG_C_FIND_RESP * findResponse, int responseCount,
	      char *SOPClass, char *queryLevel, void *ctx);
static BOOLEAN cancelFlag = FALSE;

main(int argc, char **argv)
{
    CONDITION			/* Return values from DUL and ACR routines */
	cond;
    DUL_NETWORKKEY		/* Used to initialize our network */
	* network = NULL;
    DUL_ASSOCIATESERVICEPARAMETERS	/* The items which describe this
					 * Association */
	params = {
	DICOM_STDAPPLICATIONCONTEXT, "Calling AP Title", "DICOM_STORAGE",
	    "", 16384, 0, 0, 0,
	    "calling presentation addr", "called presentation addr",
	    NULL, NULL, 0, 0,
	    MIR_IMPLEMENTATIONCLASSUID, MIR_IMPLEMENTATIONVERSIONNAME,
	    "", ""
    };
    char
       *calledAPTitle = "DICOM_STORAGE",
       *callingAPTitle = "PATIENT_QUERY",
        localHost[40],
       *node,			/* The node we are calling */
       *port;			/* ASCIIZ representation of TCP port */
    int
        scratch;		/* Used to check syntax of port number */

    BOOLEAN
	verbose = FALSE;
    DCM_OBJECT
	* queryObject;

    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'c':
	    argc--;
	    argv++;
	    if (argc <= 0)
		usageerror();
	    callingAPTitle = *argv;
	    break;
	case 't':
	    argc--;
	    argv++;
	    if (argc <= 0)
		usageerror();
	    calledAPTitle = *argv;
	    break;
	case 'v':
	    verbose = TRUE;
	    break;
	case 'x':
	    cancelFlag = TRUE;
	    break;
	default:
	    break;
	}
    }
    if (argc < 3)
	usageerror();

    DCM_Debug(verbose);
    DUL_Debug(verbose);
    SRV_Debug(verbose);
    node = *argv++;
    argc--;
    port = *argv++;
    argc--;
    if (sscanf(port, "%d", &scratch) != 1)
	usageerror();
    cond = DCM_OpenFile(*argv, DCM_ORDERLITTLEENDIAN, &queryObject);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
    argv++;
    argc--;

    cond = DUL_InitializeNetwork(DUL_NETWORK_TCP, DUL_AEREQUESTOR,
			   NULL, DUL_TIMEOUT, DUL_ORDERBIGENDIAN, &network);
    if (cond != DUL_NORMAL)
	myExit(NULL);

    (void) gethostname(localHost, sizeof(localHost));
    sprintf(params.calledPresentationAddress, "%s:%s", node, port);
    strcpy(params.callingPresentationAddress, localHost);
    strcpy(params.calledAPTitle, calledAPTitle);
    strcpy(params.callingAPTitle, callingAPTitle);

    sendQuery(&network, &params, &queryObject);
    exit(0);
}


/* sendQuery
**
** Purpose:
**	This subroutine is used to send out a C-FIND-REQ command to server
**
** Parameter Dictionary:
**      network         the handle key of the network
**      params          points to the parameters used for association request
**	queryObject	points to the DICOM object need to specify the C-FIND
**			command
**
** Return Values:
**	None
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static void
sendQuery(DUL_NETWORKKEY ** network, DUL_ASSOCIATESERVICEPARAMETERS * params,
	  DCM_OBJECT ** queryObject)
{
    DUL_ASSOCIATIONKEY		/* Describes the Association with the
				 * Acceptor */
    * association = NULL;
    MSG_C_FIND_REQ
	request;
    MSG_C_FIND_RESP
	response;
    CONDITION
	cond;

    if (requestAssociation(network, &association, params,
			   DICOM_SOPPATIENTQUERY_FIND) == 0) {
	myExit(NULL);
    }
    request.type = MSG_K_C_FIND_REQ;
    request.dataSetType = DCM_CMDDATAIDENTIFIER;
    request.messageID = SRV_MessageIDOut();
    request.priority = 0;
    request.identifier = NULL;
    strcpy(request.classUID, DICOM_SOPPATIENTQUERY_FIND);
    request.identifier = *queryObject;
    cond = SRV_CFindRequest(&association, params, &request,
			    &response, queryCallback, NULL, "");

    if (cond == SRV_OPERATIONCANCELLED)
	printf("Find operation cancelled\n");
    else if (cond != SRV_NORMAL)
	COND_DumpConditions();

    MSG_DumpMessage(&response, stdout);
    (void) DUL_ReleaseAssociation(&association);
    (void) DUL_DropNetwork(network);
}


/* queryCallback
**
** Purpose:
**	This subroutine is used to implement some special function
**	when the C-FIND request is sent out and a response is received
**
** Parameter Dictionary:
**      fineRequest     a structure contains C-FIND-REQ parameters
**      findResponse    a structure contains C-FIND-RSP parameters
**	responseCount	the response message count
**      SOPClass        the SOP Class spedified by the C-FIND command
**	queryLevel	specified the Query level
**      ctx             the representation context
**
** Return Values:
**	SRV_NORMAL
**	SRV_OPERATIONCANCELLED
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
queryCallback(MSG_C_FIND_REQ * findRequest,
	      MSG_C_FIND_RESP * findResponse, int responseCount,
	      char *SOPClass, char *queryLevel, void *ctx)
{
    printf("Query Response: %5d\n", responseCount);
    printf("SOP Class:   %s\n", SOPClass);
    printf("Query Level: %s\n", queryLevel);

    MSG_DumpMessage(findResponse, stdout);
    (void) DCM_DumpElements(&findResponse->identifier, 0);
    if (cancelFlag)
	return SRV_OPERATIONCANCELLED;
    else
	return SRV_NORMAL;
}

/* usageerror
**
** Purpose:
**	Print the usage message for this program and exit.
**
** Parameter Dictionary:
**	None
**
** Return Values:
**	None
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static void
usageerror()
{
    char msg[] = "\
Usage: patient_query [-c calling] [-t called] [-v] [-x] node port query\n\
\n\
    -c calling   Set AE Calling Title to calling\n\
    -t called    Set AE Called Title to called\n\
    -v           Place DCM, DUL and SRV facilities in verbose mode\n\
    -x           Cancel query after first callback (debugging tool)\n\
\n\
    node         Node name hosting server\n\
    port         IP port number of server\n\
    query        File containing a DICOM object with the query\n";

    fprintf(stderr, msg);
    exit(1);
}


/* myExit
**
** Purpose:
**	Exit routines which closes network connections, dumps error
**	messages and exits.
**
** Parameter Dictionary:
**	association	A handle for an association which is possibly open.
**
** Return Values:
**	None
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static void
myExit(DUL_ASSOCIATIONKEY ** association)
{
    fprintf(stderr, "Abnormal exit\n");
    COND_DumpConditions();

    if (association != NULL)
	if (*association != NULL)
	    (void) DUL_DropAssociation(association);
    exit(1);
}


/* requestAssociation
**
** Purpose:
**      This subroutine implements association request
**
** Parameter Dictionary:
**      network         the handle key of the network
**      association     the association key of the communication
**      params          the parameters for the association request
**      SOPClass        the SOP Class for specifying the association reqeust
**
** Return Values:
**      0               association request failed
**      1               association request successed
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/

static int
requestAssociation(DUL_NETWORKKEY ** network,
		   DUL_ASSOCIATIONKEY ** association,
		   DUL_ASSOCIATESERVICEPARAMETERS * params,
		   char *SOPClass)
{
    CONDITION
    cond;

    cond = SRV_RequestServiceClass(SOPClass, DUL_SC_ROLE_SCU, params);
    if (cond != SRV_NORMAL) {
	COND_DumpConditions();
	return 0;
    }
    cond = DUL_RequestAssociation(network, params, association);
    if (cond != DUL_NORMAL) {
	if (cond == DUL_ASSOCIATIONREJECTED) {
	    fprintf(stderr, "Association Rejected\n");
	    fprintf(stderr, " Result: %2x Source %2x Reason %2x\n",
		    params->result, params->resultSource,
		    params->diagnostic);
	}
	return 0;
    }
    (void) printf("Association accepted, parameters:-------------\n");
    DUL_DumpParams(params);
    return 1;
}
