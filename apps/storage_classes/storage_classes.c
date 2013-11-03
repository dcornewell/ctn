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
**			requestClasses
**			requestAssociation
** Author, Date:	Stephen M. Moore, 6-Sep-93
** Intent:		This program requests an Association with a
**			server and requests a number of SOP Classes for
**			various storage SOP Classes.
**  Usage:
**	storage_classes [-a calling title] [-c title] [-v] node port
**  Options:
**	a	Calling AP title to use during Association setup
**	c	Called AP title to use during Association setup
**	v	Verbose mode for DUL facility
**
** Last Update:		$Author: smm $, $Date: 2005-08-31 15:24:29 $
** Source File:		$RCSfile: storage_classes.c,v $
** Revision:		$Revision: 1.11 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.11 $ $RCSfile: storage_classes.c,v $";

#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
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
static void
requestClasses(DUL_NETWORKKEY ** network,
	       DUL_ASSOCIATESERVICEPARAMETERS * params);
static int
requestAssociation(DUL_NETWORKKEY ** network,
		   DUL_ASSOCIATIONKEY ** association,
		   DUL_ASSOCIATESERVICEPARAMETERS * params);

main(int argc, char **argv)
{
    CONDITION			/* Return values from DUL and ACR routines */
	cond;
    DUL_NETWORKKEY		/* Used to initialize our network */
	* network = NULL;
    DUL_ASSOCIATESERVICEPARAMETERS	/* The items which describe this
					 * Association */
	params = {
	DICOM_STDAPPLICATIONCONTEXT, "DICOM_TEST", "DICOM_STORAGE",
	    "", 16384, 0, 0, 0,
	    "calling addr", "called addr", NULL, NULL, 0, 0,
	    MIR_IMPLEMENTATIONCLASSUID, MIR_IMPLEMENTATIONVERSIONNAME,
	    "", ""
    };
    char
       *calledAPTitle = "DICOM_STORAGE",
       *callingAPTitle = "DICOM_TEST",
        localHost[40],
       *node,			/* The node we are calling */
       *port;			/* ASCIIZ representation of TCP port */
    int
        scratch;		/* Used to check syntax of port number */

    CTNBOOLEAN
	verboseDUL = FALSE,
	verboseSRV = FALSE;

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

    requestClasses(&network, &params);
    THR_Shutdown();
    exit(0);
}


/* requestClasses
**
** Purpose:
**	Request an Association and then release it and drop the network.
**
** Parameter Dictionary:
**	network		Key to the network connection
**	params		Service parameters describing the Association
**
** Return Values:
**	None
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static void
requestClasses(DUL_NETWORKKEY ** network,
	       DUL_ASSOCIATESERVICEPARAMETERS * params)
{
    DUL_ASSOCIATIONKEY		/* Describes the Association with the
				 * Acceptor */
    * association = NULL;

    if (requestAssociation(network, &association, params) == 0) {
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
    char msg[] =
    "Usage: storage_classes [-a calling Title] [-c calledTitle] node port\n";

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

/* requestAssociation
**
** Purpose:
**	request for an Association and also request service for specified
**	SOP Classes
**
** Parameter Dictionary:
**	network		Key describing the network connection
**	association	Handle to the Association (to be returned)
**	params		Service parameters describing the Association
**
** Return Values:
**	1 => success
**	0 => failure
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static int
requestAssociation(DUL_NETWORKKEY ** network,
		   DUL_ASSOCIATIONKEY ** association,
		   DUL_ASSOCIATESERVICEPARAMETERS * params)
{
    CONDITION
    cond;
    static char *SOPClasses[] = {
	DICOM_SOPCLASSVERIFICATION,
	DICOM_SOPCLASSCOMPUTEDRADIOGRAPHY,
	DICOM_SOPCLASSCT,
	DICOM_SOPCLASSMR,
	DICOM_SOPCLASSENCAPSULATEDPDF,
	DICOM_SOPCLASSNM,
	DICOM_SOPCLASSUS,
	DICOM_SOPCLASSSECONDARYCAPTURE,
    };
    int
        index;


    for (index = 0; index < (int) DIM_OF(SOPClasses); index++) {
	cond = SRV_RequestServiceClass(SOPClasses[index], DUL_SC_ROLE_SCU, params);
	if (cond != SRV_NORMAL) {
	    COND_DumpConditions();
	    return 0;
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
