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
**				DICOM 93
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	main
**			usageerror
**			myExit
**			mwlCallback
** Author, Date:	Stephen M. Moore, 1-May-93
** Intent:		This program is an example and test program which
**			demonstrates the DICOM packages developed at MIR.
**			It establishes an Association with a server and uses
**			the verification service class (ECHO command).
**  Usage:
**	mwlQuery [-a title] [-c title] [-f file] [-p] [-v] node port
**  Options:
**	a	Application title of this application
**	c	Called AP title to use during Association setup
**	f	File containing a dcm object with a query specification
**	p	Dump service parameters after Association Request
**	v	Verbose mode for DUL facility
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:37:45 $
** Source File:		$RCSfile: mwlQuery.c,v $
** Revision:		$Revision: 1.2 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.2 $ $RCSfile: mwlQuery.c,v $";

#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
#include <io.h>
#include <winsock.h>
#else
#include <sys/file.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include "dicom.h"
#include "ctnthread.h"
#include "dicom_uids.h"
#include "lst.h"
#include "condition.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"

static void usageerror();
static void myExit(DUL_ASSOCIATIONKEY ** association);

static DCM_OBJECT *
createQueryObject(const char *fileName)
{
    DCM_OBJECT *obj;
    CONDITION cond;
    DCM_ELEMENT e;
    char txt[1024] = "";

    int group = 0;
    int element = 0;
    char textValue[1024];

    if (fileName != NULL) {
	cond = DCM_OpenFile(fileName, DCM_ORDERLITTLEENDIAN, &obj);
	if (cond != DCM_NORMAL) {
	    COND_DumpConditions();
	    exit(1);
	}
	return obj;
    }
    cond = DCM_CreateObject(&obj, 0);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
    while (fgets(txt, sizeof(txt), stdin) != NULL) {
	if (txt[0] == '#' || txt[0] == '\0')
	    continue;
	if (txt[0] == '\n' || txt[0] == '\r')
	    continue;

	if (sscanf(txt, "%x %x %[^\n]", &group, &element, textValue) != 3)
	    continue;

	e.tag = DCM_MAKETAG(group, element);
	DCM_LookupElement(&e);
	if (strncmp(textValue, "#", 1) == 0) {
	    e.length = 0;
	    e.d.string = NULL;
	} else {
	    e.length = strlen(textValue);
	    e.d.string = textValue;
	}

	cond = DCM_AddElement(&obj, &e);
	if (cond != DCM_NORMAL) {
	    COND_DumpConditions();
	    exit(1);
	}
    }

    return obj;
}

static CONDITION
mwlCallback(MSG_C_FIND_REQ * request, MSG_C_FIND_RESP * response,
	    int responseCount, char *SOPClass, char *queryLevel, void *p)
{
    if (response->status == MSG_K_SUCCESS)
	return SRV_NORMAL;

    MSG_DumpMessage(response, stdout);

    return SRV_NORMAL;
}

main(int argc, char **argv)
{

    CONDITION			/* Return values from DUL and ACR routines */
	cond;
    DUL_NETWORKKEY		/* Used to initialize our network */
	* network = NULL;
    DUL_ASSOCIATIONKEY		/* Describes the Association with the
				 * Acceptor */
	* association = NULL;
    DUL_ASSOCIATESERVICEPARAMETERS	/* The items which describe this
					 * Association */
	params;
    char
       *calledAPTitle = "MWL_SCP",
       *callingAPTitle = "MWL_SCU";

    char
        localHost[40];

    int
        port;			/* TCP port used to establish Association */
    char
       *node;			/* The node we are calling */

    CTNBOOLEAN
	verbose = FALSE,
	abortFlag = FALSE,
	paramsFlag = FALSE;
    char *fileName = NULL;
    DUL_SC_ROLE
	role = DUL_SC_ROLE_DEFAULT;
    MSG_C_FIND_RESP response;
    MSG_C_FIND_REQ findRequest = {MSG_K_C_FIND_REQ, 0, 0,
	DCM_CMDDATAIDENTIFIER,
	0, NULL,
    DICOM_SOPMODALITYWORKLIST_FIND};

    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'a':
	    argc--;
	    argv++;
	    if (argc <= 0)
		usageerror();
	    callingAPTitle = *argv;
	    break;
	case 'c':
	    argc--;
	    argv++;
	    if (argc <= 0)
		usageerror();
	    calledAPTitle = *argv;
	    break;
	case 'f':
	    argc--;
	    argv++;
	    if (argc <= 0)
		usageerror();
	    fileName = *argv;
	    break;
	case 'p':
	    paramsFlag = TRUE;
	    break;
	case 'v':
	    verbose = TRUE;
	    break;
	default:
	    break;
	}
    }
    if (argc < 2)
	usageerror();

    THR_Init();
    DUL_Debug(verbose);
    SRV_Debug(verbose);

    node = *argv;
    if (sscanf(*++argv, "%d", &port) != 1)
	usageerror();
    (void) gethostname(localHost, sizeof(localHost));

    cond = DUL_InitializeNetwork(DUL_NETWORK_TCP, DUL_AEREQUESTOR,
				 NULL, 10, DUL_ORDERBIGENDIAN, &network);
    if (cond != DUL_NORMAL)
	myExit(&association);

    DUL_DefaultServiceParameters(&params);
    sprintf(params.calledPresentationAddress, "%s:%s", node, *argv);
    strcpy(params.callingPresentationAddress, localHost);
    strcpy(params.calledAPTitle, calledAPTitle);
    strcpy(params.callingAPTitle, callingAPTitle);
    cond = SRV_RequestServiceClass(DICOM_SOPMODALITYWORKLIST_FIND, role,
				   &params);
    if (cond != SRV_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    cond = DUL_RequestAssociation(&network, &params, &association);
    if (cond != DUL_NORMAL) {
	if (cond == DUL_ASSOCIATIONREJECTED) {
	    fprintf(stderr, "Association Rejected\n");
	    fprintf(stderr, " Result: %2x Source %2x Reason %2x\n",
		    params.result, params.resultSource,
		    params.diagnostic);
	}
	myExit(&association);
    }
    if (verbose || paramsFlag) {
	(void) printf("Association accepted, parameters:\n");
	DUL_DumpParams(&params);
    }
    findRequest.messageID = SRV_MessageIDOut();
    findRequest.identifier = createQueryObject(fileName);

    cond = SRV_CFindRequest(&association, &params,
			    &findRequest, &response,
			    mwlCallback, "Context", "");
    if (!(CTN_SUCCESS(cond))) {
	(void) printf("Verification Request unsuccessful\n");
	COND_DumpConditions();
    } else {
	MSG_DumpMessage(&response, stdout);
    }
    SRV_MessageIDIn(findRequest.messageID);

    cond = DUL_ReleaseAssociation(&association);
    if (cond != DUL_RELEASECONFIRMED) {
	(void) fprintf(stderr, "%x\n", cond);
	COND_DumpConditions();
    }
    (void) DUL_ClearServiceParameters(&params);

    (void) DUL_DropNetwork(&network);
    THR_Shutdown();
    return 0;
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
mwlQuery [-a title] [-c title] [-p] [-v] node port\n\
\n\
    a     Application title of this application\n\
    c     Called AP title to use during Association setup\n\
    f     File containing a dcm object with query\n\
    p     Dump service parameters after Association Request\n\
    v     Verbose mode for DUL/SRV facilities\n\
\n\
    node  Node name of server\n\
    port  Port number of server\n";

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
    THR_Shutdown();
    exit(1);
}
