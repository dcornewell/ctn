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
**			dumpCreatedImageBoxes
**
** Author, Date:	Aniruddha S. Gokhale, June 9, 1993
** Intent:
**			This module implements the SCU of the BASIC
**			Print Management SOP Class.
** Usage:
**	print_client [-c calledAPTitle] [-f films] [-i imageFormat]
**			[-p] [-s] [-t callingAPTitle] [-v] node port
** Options:
**      c       Called AP title to use during Association setup
**	f	Number of films
**	i	Image display format
**      p       Dump service parameters after Association Request
**	s	Silent, do not print messages.
**      t       Calling AP title to use during Association setup
**      v       Verbose mode for DUL facility
**
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:48:09 $
** Source File:		$RCSfile: print_client.c,v $
** Revision:		$Revision: 1.52 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.52 $ $RCSfile: print_client.c,v $";

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
#include "print_client_app.h"

/* prototype declarations of various functions used */
void
printMgmtSCU(DUL_NETWORKKEY ** network,
	     DUL_ASSOCIATESERVICEPARAMETERS * params,
	     DUL_ASSOCIATIONKEY ** association,
	     char **SOPClassList,
	     int films,
	     char *format, int argc, char **argv);
static void
    usageerror();
static void
    myExit(DUL_ASSOCIATIONKEY ** association);
static void
    dumpCreatedImageBoxes(LST_HEAD * l);

/* Global declarations */
char bfsInstanceUID[DICOM_UI_LENGTH + 1];
char bfbInstanceUID[DICOM_UI_LENGTH + 1];
PRN_PRINTER printAttrib;
CTNBOOLEAN silent = FALSE;

static char
   *SOPClassList[DICOM_UI_LENGTH + 1] = {	/* list of SOP classes to be
						 * negotiated */
    DICOM_SOPCLASSGREYSCALEPRINTMGMTMETA,
#ifdef ASG
    DICOM_SOPCLASSCOLORPRINTMGMTMETA,
#endif
    "\0"
};

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
       *calledAPTitle = "MIR_PRINT_SCP",
       *callingAPTitle = "MIR_PRINT_SCU",
        localHost[40],
       *node,			/* The node we are calling */
       *port;
    int
        scratch;		/* TCP port used to establish Association */
    int
        films = 1;		/* number of films per film session */
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
	case 'f':
	    argc--;
	    argv++;
	    if (argc <= 0)
		usageerror();
	    films = atoi(*argv);
	    break;
	case 'i':
	    argc--;
	    argv++;
	    if (argc <= 0)
		usageerror();
	    strcpy(format, *argv);
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
	THR_Shutdown();
	myExit(&association);
    }
    (void) gethostname(localHost, sizeof(localHost));
    strcpy(params.callingPresentationAddress, localHost);
    sprintf(params.calledPresentationAddress, "%s:%s", node, port);
    strcpy(params.calledAPTitle, calledAPTitle);
    strcpy(params.callingAPTitle, callingAPTitle);

    /* Negotiate for various SOP classes and establish an association */
    cond = establishAssociation(&params, &network, SOPClassList,
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
    printMgmtSCU(&network, &params, &association, SOPClassList, films, format,
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
**      SOPClassArray   SOP class UID list of the meta SOP classes for which the
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
**         Create BASIC FILM SESSION SOP instance
**         For (each film of the film session)
**              Create a BASIC FILM BOX SOP instance
**              For (every image in the FILM BOX)
**                 Set attributes of the BASIC IMAGE BOX SOP instance
**              If (films not collated)
**                 Send action request
**                 Delete the film box instance
**         If (films are collated)
**              Send action request
**              Delete the film session instance
**         Service any event-report requests
*/

void
printMgmtSCU(DUL_NETWORKKEY ** network,
	     DUL_ASSOCIATESERVICEPARAMETERS * params,
	     DUL_ASSOCIATIONKEY ** association,
	     char **SOPClassArray,
	     int films,
	     char *format,
	     int argc, char **argv)
{
    CONDITION
    cond;
    int
        index;
    int
        position;
    IMG_DISPLAYFORMAT
	* imgDisplayFormat;
    char
        temp[100 * (DICOM_UI_LENGTH + 1)];
    char
        SOPClass[DICOM_UI_LENGTH + 1];

    static PRN_BASICFILMSESSION filmSession = {
	NULL, NULL,
	PRN_K_BASICFILMSESSION,	/* This structure type */
	0,			/* Attribute Flag */
	DICOM_SOPCLASSBASICFILMSESSION,
	"",			/* Film session SOP Instance UID */
	"1",			/* 1 copies */
	HIGH,			/* high priority */
	PAPER,			/* medium is paper */
	MAGAZINE,		/* medium type */
	"",			/* session label (we're not doing this) */
	"",			/* kilo bytes requested  (not doing this) */
	NULL			/* no child film box UID known yet */
    };

    static PRN_BASICFILMBOX filmBox = {
	NULL, NULL,
	PRN_K_BASICFILMBOX,	/* type */
	0,			/* Attribute Flag */
	DICOM_SOPCLASSBASICFILMBOX,	/* SOP class UID */
	"",			/* SOP instance UID, unknown yet */
	DICOM_SOPCLASSBASICFILMSESSION,	/* SOP class UID of parent film
					 * session */
	"",			/* instance UID of parent unknown yet */
	"STANDARD\\2,2",	/* image display format */
	"",			/* annotation display */
	PORTRAIT,		/* film orientation */
	SIZE14INX17IN,		/* film size */
	"REPLICATE",		/* magnification type */
	"",			/* no smoothing type provided */
	BLACKBORDER,		/* border density */
	"",			/* no empty imaage density */
#if STANDARD_VERSION < VERSION_AUG1993
	"",			/* no minimum density provided */
	"",			/* no max density provided */
#else
	0,			/* no minimum density provided */
	0,			/* no max density provided */
#endif
	NO,			/* trim value */
	"",			/* no configuration info */
	NULL, NULL		/* List heads for basic image box */
	/* and annotation box sequences */
    };
    static PRN_BASICIMAGEBOX imageBox = {
	NULL, NULL,		/* Reserved for LST */
	PRN_K_BASICGREYSCALEIMAGEBOX,	/* Type */
	0,			/* Flag for conditional fields */
	DICOM_SOPCLASSBASICGREYSCALEIMAGEBOX,
	"",			/* SOP Instance UID */
	0,			/* Image Position */
	"",			/* Polarity */
	"",			/* Magnification type */
	"",			/* Smoothing type */
#if STANDARD_VERSION < VERSION_AUG1993
	"",			/* Magnification Factor */
#else
	"",			/* Requested Image Size */
#endif
	NULL,			/* Preformatted Image sequence */
	NULL, NULL, NULL	/* Pointers to other sequences */
    };

    static PRN_PRINTER
        printerAttributes = {	/* Printer attributes queried from printer */
	NULL, NULL,
	PRN_K_BASICPRINTER
    },
       *paPtr = &printerAttributes;
    SQ_REFPRINT
	* refPrint;
    PRN_BASICPRINTJOB printJob = {
	NULL, NULL,		/* For LST */
	PRN_K_BASICPRINTJOB,	/* Type */
	0,			/* Attribute flag */
	"", "",			/* SOP Class & Instance UID */
	"", "",			/* Execution status & status info */
	"", "",			/* Creation Date and Time */
	"",			/* Printer name */
	""			/* Originator */
    };
    unsigned short
        retStat;

    /*
     * we now set the attribute flags for all the attributes provided by the
     * SCU
     */
    filmSession.bfsAttributeFlag = PRN_BFS_K_COPIES | PRN_BFS_K_PRIORITY |
	PRN_BFS_K_MEDIUMTYPE | PRN_BFS_K_FILMDESTINATION;


    /*
     * see if  the user has provided the image display format and if so,
     * override the default value
     */
    if (strlen(format) != 0)
	(void) strcpy(filmBox.imageDisplayFormat, format);

    /*
     * the next step is to get the SOP instance UID of the Printer SOP class
     * from the SCP using the DIMSE N_GET service
     */

    if (!silent) {
	printf("\n\n");
	printf("SCU : Sending N-GET Request to get printer attributes\n\n");
    }
    (void) strcpy(SOPClass, SOPClassArray[0]);
    /*
     * we get the printer attributes for both the classes but use all the
     * rest of the commands for only GREYSCALEPRINTMGMTMETA
     */
    while (strlen(*SOPClassArray) > 0) {
	printf("Send N-GET request to get printer attr for SOP class : %s\n",
	       *SOPClassArray);
	if (getPrinterInstance(association, params, *SOPClassArray,
			       &paPtr, &retStat)
	    != APP_NORMAL) {
	    printf("In printMgmtSCU : getPrinterInstance failed\n");
	    myExit(association);
	}
	if (retStat != MSG_K_SUCCESS) {
	    printf("Error Status = %d\n", retStat);
	    myExit(association);
	}
	if (!silent)
	    PRN_DumpAttributes(&printerAttributes, stdout);
	SOPClassArray++;
    }

    /* Now create a film session SOP instance */
    if (!silent)
	printf("SCU : Sending N-CREATE Request to create BASIC FILM SESSION\n");
    cond = createFilmSession(association, params, &filmSession, SOPClass,
			     &retStat);
    if (CTN_ERROR(cond)) {
	printf("IN printMgmtSCU, sendCreateFilmSession failed\n");
	myExit(association);
    }
    if (retStat != MSG_K_SUCCESS) {
	printf("Response Status = %d\n", retStat);
	myExit(association);
    }
    if (!silent)
	PRN_DumpAttributes(&filmSession, stdout);

    /* for every film in the film session, create a Film Box SOP instance */
    for (index = 0; index < films; index++) {
	/* create a Film Box SOP instance */
	if (!silent)
	    printf("SCU : Sending N-CREATE to create BASIC FILM BOX\n\n");

	(void) strcpy(filmBox.referencedFilmSessionSOPClassUID,
		      filmSession.filmSessionSOPClassUID);
	(void) strcpy(filmBox.referencedFilmSessionSOPInstanceUID,
		      filmSession.filmSessionSOPInstanceUID);
	filmBox.bfbAttributeFlag = PRN_BFB_K_IMAGEDISPLAYFORMAT |
	    PRN_BFB_K_FILMORIENTATION | PRN_BFB_K_FILMSIZEID |
	    PRN_BFB_K_MAGNIFICATIONTYPE | PRN_BFB_K_BORDERDENSITY;
	filmBox.bfbAttributeFlag |= PRN_BFB_K_REFERENCEDBASICFILMSESSIONSEQ;
	cond = createFilmBox(association, params, &filmBox, SOPClass, &retStat);
	if (CTN_ERROR(cond)) {
	    printf("In printMgmtSCU, createFilmBox failed\n");
	    myExit(association);
	}
	if (retStat != MSG_K_SUCCESS) {
	    printf("Error status = %d\n", retStat);
	    myExit(association);
	}
	if (!silent)
	    dumpCreatedImageBoxes(filmBox.referencedBIBSeq);

	/*
	 * now find the image display format from the command line argument
	 */
	imgDisplayFormat = decodeImageDisplayFormat(filmBox.imageDisplayFormat);
	if (imgDisplayFormat == NULL) {
	    printf("Incorrect or EMPTY image display format provided \n");
	    printf("In printMgmtSCU, decodeImageDisplayFormat failed\n");
	    myExit(association);
	}
	/*
	 * for every image box created by the film box, we set the attributes
	 * of the image box
	 */
	refPrint = LST_Head(&filmBox.referencedBIBSeq);
	if (refPrint != NULL)
	    (void) LST_Position(&filmBox.referencedBIBSeq, refPrint);

	for (position = 1; position <= imgDisplayFormat->count; position++) {
	    if (!silent) {
		printf("SCU : Sending N-SET Request to set attributes of ");
		printf("BASIC IMAGE BOX\n\n");
	    }
	    if (refPrint == NULL) {
		fprintf(stderr, "No BIB UID for this image box\n");
		myExit(association);
	    }
	    strcpy(imageBox.imageBoxSOPClassUID, refPrint->refSOPClassUID);
	    strcpy(imageBox.imageBoxSOPInstanceUID, refPrint->refSOPInstanceUID);
	    imageBox.imagePosition = position;
	    if (argc-- <= 0) {
		fprintf(stderr, "No preformatted image for this image box\n");
		myExit(association);
	    }
	    if (DCM_OpenFile(*argv++, DCM_ORDERLITTLEENDIAN,
			     &imageBox.object) != DCM_NORMAL) {
		fprintf(stderr, "Error opening preformatted image\n");
		myExit(association);
	    }
	    imageBox.bibAttributeFlag = REQUIRED_BIB_SCU_NSETATTRIB;
	    if (setImageBox(association, params, SOPClass, &imageBox, &retStat)
		!= SRV_NORMAL) {
		printf("In printMgmtSCU, sendBIBNSetRequest failed\n");
		myExit(association);
	    }
	    if (retStat != MSG_K_SUCCESS) {
		printf("Error status = %d\n", retStat);
		myExit(association);
	    }
	    refPrint = LST_Next(&filmBox.referencedBIBSeq);
	}
	/*
	 * Now we ask the SCP to print the images in the current film box
	 * instance
	 */

	if (!silent)
	    printf("SCU : Sending N-ACTION request to print the BASIC FILM BOX\n\n");
	if (sendFilmBoxPrint(association, params, SOPClass,
	    DICOM_SOPCLASSBASICFILMBOX, &filmBox, &retStat) != SRV_NORMAL) {
	    printf("In printMgmtSCU, sendActionRequest failed\n");
	    myExit(association);
	}
	if (retStat != MSG_K_SUCCESS) {
	    printf("Error Status : %d\n", retStat);
	    myExit(association);
	}
	/* now we ask the SCP to delete the current Film Box instance */
	if (!silent)
	    printf("SCU : Sending N-DELETE Request to delete current FILM BOX\n\n");
	if (sendDeleteRequest(association, params, SOPClass,
			      DICOM_SOPCLASSBASICFILMBOX,
		   filmBox.filmBoxSOPInstanceUID, &retStat) != SRV_NORMAL) {
	    printf("In printMgmtSCU, sendDeleteRequest failed\n");
	    printf("while deleting FILM BOX instance \n");
	    myExit(association);
	}
	if (retStat != MSG_K_SUCCESS) {
	    printf("Error Status : %d\n", retStat);
	    myExit(association);
	}
    }

    /* service any event-report requests asynchronously */
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
Usage: print_client [-c calledAPTitle] [-f films] [-i imageFormat] [-p] [-s]  [-t callingAPTitle] [-v] node port file\n\
\n\
    -c    Set the Called Application Entity Title\n\
    -f    Number of films to print\n\
    -i    Image format, for example STANDARD\2\2\n\
    -p    Dump association parameters after association established\n\
    -s    Silent mode; do not print results of all print commands\n\
    -t    Set the Calling Application Entity Title\n\
    -v    Use verbose mode for DUL and SRV facilities\n\
    node  The host name that is running a print server\n\
    port  TCP/IP port number of print server\n\
    file  One or more files that contain preformatted images for printing\n";

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

/* dumpCreatedImageBoxes
**
** Purpose:
**	Dump the contents of the list of image boxes
**
** Parameter Dictionary:
**	l	List of image boxes
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
dumpCreatedImageBoxes(LST_HEAD * l)
{
    SQ_REFPRINT
    * refPrint;

    printf("Dumping created Basic Image Box UIDs\n");
    refPrint = LST_Head(&l);
    if (refPrint != NULL)
	(void) LST_Position(&l, refPrint);
    while (refPrint != NULL) {
	printf("%s\n          %s\n", refPrint->refSOPClassUID,
	       refPrint->refSOPInstanceUID);
	refPrint = LST_Next(&l);
    }
}
