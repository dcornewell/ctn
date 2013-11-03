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
**			printMgmtSCU
**
** Author, Date:	Aniruddha S. Gokhale, June 9, 1993
**			Steve Moore, 6-Sep-94
** Intent: 		This module implements the SCU of the BASIC
**			Print Management SOP Class.
**			It requests an Association with a print server
**			and makes an N-GET Request to get printer information.
** Usage:
**	print_query [-c calledAPTitle] [-p] [-t callingAPTitle] [-v] node port
** Options:
**      c       Called AP title to use during Association setup
**	f	Number of films
**      p       Dump service parameters after Association Request
**      t       Calling AP title to use during Association setup
**      v       Verbose mode for DUL facility
**
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:47:31 $
** Source File:		$RCSfile: print_query.c,v $
** Revision:		$Revision: 1.7 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.7 $ $RCSfile: print_query.c,v $";

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
#include "lst.h"
#include "condition.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "dicom_uids.h"
#include "dicom_sq.h"
#include "dicom_print.h"
#include "print_shared.h"
#include "print_client_private.h"
#include "print_client_app.h"

/* prototype declarations of various functions used */
void
printMgmtSCU(DUL_NETWORKKEY ** network,
	     DUL_ASSOCIATESERVICEPARAMETERS * params,
	     DUL_ASSOCIATIONKEY ** association,
	     char *SOPClass,
	     int films,
	     char *format, int argc, char **argv);
static void
    usageerror();
static void
    myExit(DUL_ASSOCIATIONKEY ** association);

/* Global declarations */
CTNBOOLEAN silent = FALSE;

/* MAIN PROGRAM */
main(int argc, char **argv)
{
    CONDITION			/* Return values from DUL and ACR routines */
	cond;
    DUL_NETWORKKEY		/* Used to initialize our network */
	* network = NULL;
    DUL_ASSOCIATESERVICEPARAMETERS	/* The items which describe this
					 * Association */
	params;
    DUL_ASSOCIATIONKEY		/* Key which describes the association */
	* association = NULL;
    char
       *calledAPTitle = "PRINT_SERVER_SCP",
       *callingAPTitle = "PRINT_CLIENT_SCU",
        localHost[40],
       *node,			/* The node we are calling */
       *port;
    int
        scratch;		/* TCP port used to establish Association */
    char
        format[DICOM_LO_LENGTH + 1];	/* stores the image display format */
    char
        SOPClass[DICOM_UI_LENGTH + 1];	/* SOPClass to be negotiated */

    CTNBOOLEAN			/* for debugging purposes */
	verboseDUL = FALSE,
	verboseDCM = FALSE,
	verboseSRV = FALSE,
	paramsFlag = FALSE;

    /* initialize the image display format */
    (void) strcpy(format, "\0");

    /* parse the command line argument */
    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'c':
	    argc--;
	    argv++;
	    if (argc <= 0)
		usageerror();
	    calledAPTitle = *argv;
	    break;
	case 'd':
	    argc--;
	    argv++;
	    if (argc <= 0)
		usageerror();
	    if (strcmp(*argv, "DUL") == 0)
		verboseDUL = TRUE;
	    else if (strcmp(*argv, "DCM") == 0)
		verboseDCM = TRUE;
	    else if (strcmp(*argv, "SRV") == 0)
		verboseSRV = TRUE;
	    else
		usageerror();
	    break;
	case 'p':
	    paramsFlag = TRUE;
	    break;
	case 's':
	    silent = TRUE;
	    break;
	case 't':
	    argc--;
	    argv++;
	    if (argc <= 0)
		usageerror();
	    callingAPTitle = *argv;
	    break;
	case 'v':
	    verboseDCM = TRUE;
	    verboseDUL = TRUE;
	    verboseSRV = TRUE;
	    break;
	default:
	    break;
	}
    }
    if (argc < 2)
	usageerror();

    node = *argv++;
    argc--;
    port = *argv++;
    argc--;
    if (sscanf(port, "%d", &scratch) != 1)
	usageerror();

    THR_Init();
    DCM_Debug(verboseDCM);
    DUL_Debug(verboseDUL);
    SRV_Debug(verboseSRV);

    /* Network initialization and association establishment phase */
    cond = initNetwork(&params, &network);
    if (cond != DUL_NORMAL) {
	printf("In main : initNetwork failed\n");
	myExit(&association);
    }
    (void) gethostname(localHost, sizeof(localHost));
    strcpy(params.callingPresentationAddress, localHost);
    sprintf(params.calledPresentationAddress, "%s:%s", node, port);
    strcpy(params.calledAPTitle, calledAPTitle);
    strcpy(params.callingAPTitle, callingAPTitle);

    /* Negotiate for various SOP classes and establish an association */
    cond = establishAssociation(&params, &network, SOPClass,
				&association);
    if (cond != DUL_NORMAL) {
	printf("In main : establishAssociation failed \n");
	myExit(&association);
    }
    if (verboseDUL || paramsFlag) {
	(void) printf("Association accepted, parameters:\n");
	DUL_DumpParams(&params);
    }
    /* Perform the actions of the SCU of a print management service class */
    printMgmtSCU(&network, &params, &association, SOPClass, 1, format,
		 argc, argv);

    /* cleaning phase */
    cond = DUL_ReleaseAssociation(&association);
    if (cond != DUL_RELEASECONFIRMED) {
	(void) fprintf(stderr, "%x\n", cond);
	COND_DumpConditions();
    }
    (void) DUL_DropNetwork(&network);
    THR_Shutdown();
    exit(0);

}

/* printMgmtSCU
**
** Purpose:
**      Actual operation of an SCU of the Print Management SOP Class
** Parameter Dictionary:
**      network         key which describes the network connection
**      params          describes the association parameters
**      association     key which describes the association
**      SOPClass        SOP class UID of the meta SOP class for which the
**                      association is established.
**      films           number of films i.e. FILM BOX instances
**      format          image display format
**      argc            Number of remaining args to main (used to read
**                      files which contain preformatted images)
**      argv            Pointer to list of remaining arguments.  Should
**                      contain names of files with preformatted images.
**
** Return Values:
**      None.
** Algorithm:
**      If association established then
**         Get Printer attributes
*/

void
printMgmtSCU(DUL_NETWORKKEY ** network,
	     DUL_ASSOCIATESERVICEPARAMETERS * params,
	     DUL_ASSOCIATIONKEY ** association,
	     char *SOPClass,
	     int films,
	     char *format,
	     int argc, char **argv)
{
    CONDITION
    cond;

    static PRN_PRINTER
        printerAttributes = {	/* Printer attributes queried from printer */
	NULL, NULL,
	PRN_K_BASICPRINTER
    },
       *paPtr = &printerAttributes;

    /*
     * Get the SOP instance UID of the Printer SOP class from the SCP using
     * the DIMSE N_GET service
     */

    if (!silent) {
	printf("\n\n");
	printf("SCU : Sending N-GET Request to get printer attributes\n\n");
    }
    if (getPrinterInstance(association, params, SOPClass, &paPtr)
	!= APP_NORMAL) {
	printf("In printMgmtSCU : getPrinterInstance failed\n");
	myExit(association);
    }
    if (!silent)
	PRN_DumpAttributes(&printerAttributes, stdout);
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
    fprintf(stderr,
	    "Usage: print_client [-c calledAPTitle] [-f films] [-i imageFormat] [-p] [-s]  [-t callingAPTitle] [-v] node port\n");
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
