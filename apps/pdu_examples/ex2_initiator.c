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
** Author, Date:	Stephen M. Moore, 11-Nov-93
** Intent:		This program is an example and test program which
**			demonstrates the DICOM packages developed at MIR.
**			This test program is used to generate an Association
**			request to produce an example PDU.  This program
**			requests a single presentation context with multiple
**			transfer syntaxes.
**  Usage:
**	ex2_initiator node port
**
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:37:46 $
** Source File:		$RCSfile: ex2_initiator.c,v $
** Revision:		$Revision: 1.6 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.6 $ $RCSfile: ex2_initiator.c,v $";

#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/file.h>
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
static int
requestAssociation(DUL_NETWORKKEY ** network,
		   DUL_ASSOCIATIONKEY ** association,
		   DUL_ASSOCIATESERVICEPARAMETERS * params,
		   char **SOPClasses, int classCount);
static void
reqReleaseAssociation(DUL_NETWORKKEY ** network,
		      DUL_ASSOCIATESERVICEPARAMETERS * params);

main(int argc, char **argv)
{
    CONDITION			/* Return values from facilities */
	cond;
    DUL_NETWORKKEY		/* Used to initialize our network */
	* network = NULL;
    DUL_ASSOCIATESERVICEPARAMETERS	/* The items which describe this
					 * Association */
	params = {
	DICOM_STDAPPLICATIONCONTEXT, "DEMO_SENDER_93", "DICOM_STORAGE",
	    "", 16384, 0, 0, 0,
	    "calling addr", "called addr", NULL, NULL, 0, 0,
	    MIR_IMPLEMENTATIONCLASSUID, MIR_IMPLEMENTATIONVERSIONNAME,
	    "", ""
    };
    char
       *calledAPTitle = "DICOM_STORAGE",
       *callingAPTitle = "DEMO_SENDER_93",
        localHost[40],
       *node,			/* The node we are calling */
       *port;			/* ASCIIZ representation of TCP port */
    int
        scratch;		/* Used to check syntax of port number */
    unsigned long
        maxPDU = 16384;

    CTNBOOLEAN
	verboseDCM = FALSE,
	verboseDUL = FALSE,
	verboseSRV = FALSE;

    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'c':
	    argc--;
	    argv++;
	    if (argc <= 0)
		usageerror();
	    callingAPTitle = *argv;
	    break;
	case 'd':
	    argc--;
	    argv++;
	    if (argc <= 0)
		usageerror();
	    if (strcmp(*argv, "DCM") == 0)
		verboseDCM = TRUE;
	    else if (strcmp(*argv, "DUL") == 0)
		verboseDUL = TRUE;
	    else if (strcmp(*argv, "SRV") == 0)
		verboseSRV = TRUE;
	    else
		usageerror();
	    break;
	case 'm':
	    argc--;
	    argv++;
	    if (argc <= 0)
		usageerror();
	    if (sscanf(*argv, "%lu", &maxPDU) != 1)
		usageerror();
	    break;
	case 't':
	    argc--;
	    argv++;
	    if (argc <= 0)
		usageerror();
	    calledAPTitle = *argv;
	    break;
	case 'v':
	    verboseDUL = TRUE;
	    verboseSRV = TRUE;
	    break;
	default:
	    break;
	}
    }
    if (argc < 2)
	usageerror();

    THR_Init();
    DCM_Debug(verboseDCM);
    DUL_Debug(verboseDUL);
    SRV_Debug(verboseSRV);
    node = *argv++;
    argc--;
    port = *argv++;
    argc--;
    if (sscanf(port, "%d", &scratch) != 1)
	usageerror();

    cond = DUL_InitializeNetwork(DUL_NETWORK_TCP, DUL_AEREQUESTOR,
			   NULL, DUL_TIMEOUT, DUL_ORDERBIGENDIAN, &network);
    if (cond != DUL_NORMAL)
	myExit(NULL);

    (void) gethostname(localHost, sizeof(localHost));
    sprintf(params.calledPresentationAddress, "%s:%s", node, port);
    strcpy(params.callingPresentationAddress, localHost);
    strcpy(params.calledAPTitle, calledAPTitle);
    strcpy(params.callingAPTitle, callingAPTitle);
    params.maxPDU = maxPDU;

    reqReleaseAssociation(&network, &params);
    THR_Shutdown();
    exit(0);
}


/* reqReleaseAssociation
**
** Purpose:
**	Describe the purpose of the function
**
** Parameter Dictionary:
**	Define the parameters to the function
**
** Return Values:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static void
reqReleaseAssociation(DUL_NETWORKKEY ** network,
		      DUL_ASSOCIATESERVICEPARAMETERS * params)
{
    DUL_ASSOCIATIONKEY		/* Describes the Association with the
				 * Acceptor */
    * association = NULL;
    char
       *SOPClasses[] = {DICOM_SOPCLASSMR
    };

    if (requestAssociation(network, &association, params, SOPClasses, 1) == 0) {
	myExit(NULL);
    }
    (void) DUL_ReleaseAssociation(&association);
    (void) DUL_ClearServiceParameters(params);
    (void) DUL_DropNetwork(network);
}

/* usageerror
**
** Purpose:
**	Print the usage message for this program and exit.
**
** Parameter Dictionary:
**	Define the parameters to the function
**
** Return Values:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static void
usageerror()
{
    char msg[] = "\
    ex2_initiator [-c callingTitle][-d FAC][-m maxPDU][-t title][-v] node port\n\
    \n\
    -c    Set calling AE title to callingTitle in Association RQ\n\
    -d    Place one facility(DCM, DUL, SRV) in verbose mode\n\
    -m    Set maximum PDU in Association RQ to maxPDU\n\
    -t    Set called AE title to title in Association RQ\n\
    -v    Place DUL and SRV facilities in verbose mode\n\
    \n\
    node  Node name for network connection\n\
    port  TCP / IP port number of server application\n";

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



/* requestAssociation
**
** Purpose:
**	Describe the purpose of the function
**
** Parameter Dictionary:
**	Define the parameters to the function
**
** Return Values:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static int
requestAssociation(DUL_NETWORKKEY ** network,
		   DUL_ASSOCIATIONKEY ** association,
		 DUL_ASSOCIATESERVICEPARAMETERS * params, char **SOPClasses,
		   int classCount)
{
    CONDITION
    cond;
    DUL_PRESENTATIONCONTEXT
	* ctx;
    static DUL_PRESENTATIONCONTEXTID
        contextID = 1;

    if (params->requestedPresentationContext == NULL) {
	params->requestedPresentationContext = LST_Create();
	if (params->requestedPresentationContext == NULL) {
	    return 0;
	}
    }
    while (classCount-- > 0) {
	ctx = malloc(sizeof(*ctx));
	if (ctx == NULL)
	    return 0;

	cond = DUL_MakePresentationCtx(&ctx, DUL_SC_ROLE_DEFAULT,
				       DUL_SC_ROLE_DEFAULT,
				       contextID, 0, *SOPClasses++, "",
				       DICOM_TRANSFERLITTLEENDIAN,
				       DICOM_TRANSFERLITTLEENDIANEXPLICIT,
				       DICOM_TRANSFERBIGENDIANEXPLICIT,
				       NULL);
	if (cond != DUL_NORMAL)
	    return 0;
	else {
	    cond = LST_Enqueue(&params->requestedPresentationContext, ctx);
	    if (cond != LST_NORMAL)
		return 0;

	    contextID += 2;
	}
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
    (void) printf("Association accepted, parameters:\n");
    DUL_DumpParams(params);
    return 1;
}
