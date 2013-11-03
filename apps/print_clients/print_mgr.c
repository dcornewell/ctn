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
** Module Name(s):
**			Quit
**			UsageError
**			ParsePrintConfigFile
**			SelectPrinter
**			CreateBasicFilmSession
**			CreateBasicFilmSessionWindow
**			CreateBasicFilmBox
**			CreateBasicFilmBoxWindow
**			CreateImageAttributeWindow
**			main
**
** Author, Date:	Tze Khiong Lee, Jul 25th 1993
** Intent:		This is the main body of the print_mgr program.
**			It is to provide a graphical user interface for
**			printing DICOM images.
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:47:51 $
** Source File:		$RCSfile: print_mgr.c,v $
** Revision:		$Revision: 1.46 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.46 $ $RCSfile: print_mgr.c,v $";
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
#include "dicom_sq.h"
#include "dicom_print.h"
#include "print_shared.h"
#include "print_client_app.h"
#include "cfg.h"


CTNBOOLEAN
Send_BIB_when_print = TRUE;

PRN_PRINTER printAttrib = {
    NULL, NULL,
    PRN_K_BASICPRINTER
};

PRN_BASICIMAGEBOX bibAttrib = {
    NULL, NULL,			/* Reserved for LST */
    PRN_K_BASICGREYSCALEIMAGEBOX,	/* Type */
    0,				/* Flag for conditional fields */
    DICOM_SOPCLASSBASICGREYSCALEIMAGEBOX,
    "",				/* SOP Instance UID */
    0,				/* Image Position */
    "",				/* Polarity */
    "",				/* Magnification type */
    "",				/* Smoothing type */
#if STANDARD_VERSION < VERSION_AUG1993
    "",				/* magnification factor */
#else
    "",				/* Requested Image Size */
#endif
    NULL,			/* Preformatted Image sequence */
    NULL, NULL, NULL		/* Pointers to other sequences */
};

char
   *calledAPTitle = "MIR_PRINT_SCP",
   *callingAPTitle = "MIR_PRINT_SCU";

#ifdef ATHENA
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/List.h>
#include <X11/Xaw/Viewport.h>
#include "Buttons.h"
#include "MessageBox.h"
#include "Fields.h"
#include "print_mgr.h"
#include "print_shared.h"
#include "display_icon.h"
#include "display_study.h"
#include "display_error.h"

#define DEFAULT_PRINT_CONFIG		"pri_connect.cfg"

XtAppContext
app_context;

Widget
toplevel,
mainwin,
bfsshell,
bfbshell,
imageattshell,
studyshell,
studylist,
previewshell,
bibshell = NULL;
#endif

static DUL_NETWORKKEY
*   network = NULL;

DUL_ASSOCIATIONKEY
* association;
DUL_ASSOCIATESERVICEPARAMETERS
params;


static char
   *SOPClassList[DICOM_UI_LENGTH + 1] = {	/* list of SOP classes to be
						 * negotiated */
    DICOM_SOPCLASSGREYSCALEPRINTMGMTMETA,
/*    DICOM_SOPCLASSCOLORPRINTMGMTMETA, */
    "\0"
};
char
    SOPClass[DICOM_UI_LENGTH + 1];

char bfsInstanceUID[DICOM_UI_LENGTH + 1];	/* instance UID of the parent
						 * BASIC FILM SESSION SOP
						 * instance */
char bfbInstanceUID[DICOM_UI_LENGTH + 1];	/* instance UID of the last
						 * created BASIC FILM BOX SOP
						 * instance */
#ifdef SMM
static DCM_ELEMENT
    e[] = {			/* stores the chain of instance UIDS of the
				 * last created FILM BOX SOP instance */
    {DCM_BFSREFERENCEDFILMBOXSEQ, DCM_SQ, "", 1, DICOM_UI_LENGTH, ""}
};
#endif

CTNBOOLEAN silent = FALSE;

char *icon_file = "icon.file";
char *icon_index = "icon.index";
char *db_file = "icon.db";

#ifdef ATHENA
/* Quit
**
** Purpose:
**	This function is called when the user clicks on the Quit button.
**
** Parameter Dictionary:
**	w		For displaying the Quit button
**	client_data	Unused
**	call_data	Unused
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
Quit(Widget w, XtPointer client_data, XtPointer call_data)
{
    CONDITION
    cond;
    if (association != NULL) {
	cond = DUL_ReleaseAssociation(&association);
	if (cond != DUL_RELEASECONFIRMED) {
	    printf("Error: DUL_ReleaseAssociation failed\n");
	}
    }
    (void) DUL_DropNetwork(&network);
    exit(0);
}


/* UsageError
**
** Purpose:
**	This function will print out the command line format.
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
void
UsageError()
{
    static char help[] = "\
   print_mgr [SWITCHES] vendor\n\
\n\
       vendor       Vendor Login name. (Required)\n\
\n\
 SWITCHES:\n\
    -c connection   Set the printer configuration  Default:pri_connect.cfg)\n\
                    pathname.\n\
    -C CTN          Set the local CTN name.\n\
    -D database     Set the database pathname.     Default:icon.db\n\
    -I index        Set the icon index pathname.   Default:icon.index\n\
    -F file         Set the icon file pathname.    Default:icon.file\n\
    -v              Force verbose mode on all\n\
                    Facility function calls.\n\
    -z option       Sets the verbose mode for\n\
                    a facility.\n\
                    (option = srv or dcm or dul)\n\
    -s              Silent mode.\n\
    -S              When set, the BIB information\n\
                    will be sent to the print server\n\
                    as each BIB is filled with an image.\n";
    int i;

    printf("%s\n", help);
    exit(0);
}

/* Definition of a printer */
typedef struct {
    char
       *CalledApTitle,
       *Node,
       *Port;
}   DCMPrinter;


/* ParsePrintConfigFile
**
** Purpose:
** 	This function will parse a configuration for print_mgr.
**	The configuration will consist of printers available
**	different the node/port information for each printers.
**
** Parameter Dictionary:
** 	file		pathname of the configuration file.
**	login		login name of the vendor.
**	printers	list of printers available.
**	size		number of printers available.
**
** Return Values:
**	TRUE		Configuration File successfully read.
**	FALSE		Failed.
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
Boolean
ParsePrintConfigFile(char *file, char *ctn, char *login, DCMPrinter ** printers, int *size)
{
    CFG_CONNECT
	* connections;
    short
        i,
        j,
        count;

    if (CFG_LoadConnect(file, ctn, &connections, &count) != CFG_NORMAL) {
	printf("Error: CFG_LoadConnect failed\n");
	(void) displayError("Could not load the printer configuration file.");
	return (FALSE);
    }
    for (i = 0, *size = 0; i < count; i++)
	if (!strcmp(login, connections[i].login))
	    (*size)++;
    *printers = (DCMPrinter *) malloc(sizeof(DCMPrinter) * (*size));
    if (*printers == NULL) {
	printf("Error: Malloc failed\n");
	return (FALSE);
    }
    for (i = 0, j = 0; i < count; i++)
	if (!strcmp(login, connections[i].login)) {
	    (*printers)[j].Node = strdup(connections[i].vendor_node);
	    (*printers)[j].Port = strdup(connections[i].port);
	    (*printers)[j].CalledApTitle = strdup(connections[i].title);
	    if (!(*printers)[j].Node || !(*printers)[j].Port ||
		!(*printers)[j].CalledApTitle) {
		printf("Error: strdup failed\n");
		return (FALSE);
	    }
	    j++;
	}
    CFG_free(connections);
    return (TRUE);
}


/* SelectPrinter
**
** Purpose:
**	This function is called after the user selects a printer from the
**	list.  It will make a network connection with the corresponding
**	print server to verify that the print service is avaible.  It will
** 	also query the print for printer attributes and display them.
**	It will then popup the Basic Film Session Window.
**
** Parameter Dictionary:
**
**	w			Widget displaying the names of various printers
**	client_data		List of printers
**	call_data		Xaw List Structure
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
SelectPrinter(Widget w, XtPointer client_data, XtPointer call_data)
{
    XawListReturnStruct
    * listdata;
    DCMPrinter
	* printers;
    char
        localHost[40];
    CONDITION
	cond;
    static DCM_TAG
        tags[] = {		/* We request the server to provide us with
				 * the following attributes */
	DCM_PRINTERSTATUS,
	DCM_PRINTERSTATUSINFO,
	DCM_PRINTERNAME,
	DCM_IDMANUFACTURER,
	DCM_IDMANUFACTURERMODEL,
	DCM_ACQDEVICESERIALNUM,
	DCM_ACQSOFTWAREVERSION,
	DCM_ACQDATELASTCALIBRATION,
	DCM_ACQTIMELASTCALIBRATION
    };				/* we ask for all the attributes of the
				 * printer */
    static PRN_PRINTER
    *   paptr;

    unsigned short
        retStat;

    XtPopdown(bfbshell);
    XtPopdown(bfsshell);
    XtPopdown(imageattshell);
    XtPopdown(previewshell);
    XtPopdown(studyshell);
    if (bibshell != NULL)
	XtPopdown(bibshell);
    listdata = (XawListReturnStruct *) call_data;
    printers = (DCMPrinter *) client_data;

    printf("Selected: %s\n", listdata->string);
    printf("Selected: printer=%s node=%s port=%s\n",
	   printers[listdata->list_index].CalledApTitle,
	   printers[listdata->list_index].Node,
	   printers[listdata->list_index].Port);
    if (association != NULL) {
	cond = DUL_ReleaseAssociation(&association);
	if (cond != DUL_RELEASECONFIRMED) {
	    printf("Error: DUL_ReleaseAssociation failed\n");
	    (void) displayError("Failed to release network connection properly.");
	}
    }
    association = NULL;
    (void) gethostname(localHost, sizeof(localHost));
    sprintf(params.calledPresentationAddress, "%s:%s",
	    printers[listdata->list_index].Node,
	    printers[listdata->list_index].Port);
    strcpy(params.callingPresentationAddress, localHost);
    strcpy(params.calledAPTitle, calledAPTitle);
    strcpy(params.callingAPTitle, callingAPTitle);
    printAttrib.attributeFlag = PRN_PRINTER_K_NAME |
	PRN_PRINTER_K_MANUFACTURER | PRN_PRINTER_K_MODELNUMBER |
	PRN_PRINTER_K_SERIALNUMBER;
    if (establishAssociation(&params, &network, SOPClassList,
			     &association) != DUL_NORMAL) {
	(void) displayError("Failed to create a network connection with print server.");
	return;
    }
/**** Temporary
    Needs to be made sophisticated
*/
    (void) strcpy(SOPClass, SOPClassList[0]);
    paptr = &printAttrib;
    if (getPrinterInstance(&association, &params, SOPClass, &paptr, &retStat)
	!= APP_NORMAL) {
	(void) displayError("Communications failure with print server.");
	return;
    }
    if (retStat != MSG_K_SUCCESS) {
	displayErrorStatus("PRINTER", "NGet", retStat);
	return;
    }
    UpdateAllEntries();
    XtPopup(bfsshell, XtGrabNone);
}

int filmboxes;
int filmboxcount;

/* CreateBasicFilmSession
**
** Purpose:
**	This function is called when the user clicks on the Create
**	button in the Basic Film Session Window.  This will create
**	a basic film session with the print server.  It will popup
**	the basic film box window.
**
** Parameter Dictionary:
**	w
**	client_data
**	call_data
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
CreateBasicFilmSession(Widget w, XtPointer client_data, XtPointer call_data)
{
    CONDITION
    cond;
    unsigned short
        retStat;



    filmboxes = atoi(numBFBC);
    filmboxcount = 1;
    if (filmboxes < 1) {
	(void) displayError("Number of film boxes must be greater than 0");
	return;
    }
    bfsAttrib.bfsAttributeFlag = PRN_BFS_K_COPIES | PRN_BFS_K_PRIORITY |
	PRN_BFS_K_MEDIUMTYPE | PRN_BFS_K_FILMDESTINATION;
    strcpy(bfsAttrib.filmSessionSOPInstanceUID, "");
    cond = createFilmSession(&association, &params, &bfsAttrib, SOPClass,
			     &retStat);
    if (cond != SRV_NORMAL) {
	printf("sendCreateFilmSession failed\n");
	(void) displayError("Communications Failure with print server.");
	return;
    }
    if (retStat != MSG_K_SUCCESS) {
	displayErrorStatus("BFS", "NCreate", retStat);
	return;
    }
    XtPopdown(bfsshell);
    XtPopup(bfbshell, XtGrabNone);
}


/* CreateBasicFilmSessionWindow
**
** Purpose:
**	This function creates the Basic Film Session Window.
**
** Parameter Dictionary:
**	None
**
** Return Values:
**	TRUE	Success
**	FALSE	Failed
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
Boolean
CreateBasicFilmSessionWindow()
{
    Widget
	bigform,
	bfsform,
	bfsform2,
	bfsform3,
	label,
	pform;
    static String message[] = {
	"Printer Information:"
    };

    static MessageBox mb = {
	"Basic Film Session",
	message,
	DIM_OF(message),
	NULL, NULL, NULL, NULL, NULL,
	CreateBasicFilmSession,
	"Create",
    TRUE};
/*
	bfsshell = XtVaCreatePopupShell("Basic Film Session",
		transientShellWidgetClass, toplevel, NULL);
*/
    bfsshell = CreateMessageBoxPopup(&mb, toplevel, TRUE);
    pform = CreateEntryForm(mb.workform, mb.lastlabel, NULL,
			printer_capabilities, DIM_OF(printer_capabilities));
    label = CreateLabel(mb.workform, pform, NULL, "Basic Film Session:",
			5);
    bigform = XtVaCreateManagedWidget("big_form", formWidgetClass, mb.workform,
			       XtNfromVert, label, XtNborderWidth, 0, NULL);
    bfsform = CreateEntryForm(bigform, NULL, NULL,
			      basic_film_session,
			      DIM_OF(basic_film_session));
    bfsform2 = CreateEntryForm(bigform, NULL, bfsform,
			       basic_film_session2,
			       DIM_OF(basic_film_session2));
    bfsform3 = CreateEntryForm(mb.workform, bigform, NULL,
			       basic_film_session3,
			       DIM_OF(basic_film_session3));
    if (!bfsshell || !pform || !label || !bfsform || !bfsform2 || !bfsform3)
	return (FALSE);
    return (TRUE);
}




/* CreateBasicFilmBox
**
** Purpose:
**	This function is called when the user clicks on the Create
**	button in the Basic Film Box Window.  This will create
**	a basic film box with the print server.
**
** Parameter Dictionary:
**	w
**	client_data
**	call_data
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
CreateBasicFilmBox(Widget w, XtPointer client_data, XtPointer call_data)
{
    CONDITION
    cond;
    char
        temp[100 * (DICOM_UI_LENGTH + 1)];
    int
        col,
        row;
    double
        tempf;
    unsigned short
        retStat,
        tempDensity;

#ifdef SMM
    e[0].d.string = temp;
    e[0].length = 999 * (DICOM_UI_LENGTH + 2);	/* assign enough space */
#endif

    strcpy(bfbAttrib.referencedFilmSessionSOPClassUID,
	   bfsAttrib.filmSessionSOPClassUID);
    strcpy(bfbAttrib.referencedFilmSessionSOPInstanceUID,
	   bfsAttrib.filmSessionSOPInstanceUID);

/*
    if(!strcmp(displayFormatC, STANDARD)) {
*/

    if ((col = atoi(bfbstdCOL)) == 0) {
	(void) displayError("Please Enter a valid STANDARD Column number.");
	return;
    }
    if (col < 1 || col > 8) {
	(void) displayError
	    ("Please Enter a STANDARD column number between 1 and 8.");
	return;
    }
    if ((row = atoi(bfbstdROW)) == 0) {
	(void) displayError("Please Enter a valid STANDARD Row number.");
	return;
    }
    if (row < 1 || row > 8) {
	(void) displayError
	    ("Please Enter a STANDARD row number between 1 and 8.");
	return;
    }
/*
    if (!strncmp(bfbAttrib.imageDisplayFormat, STANDARD, strlen(STANDARD)))
*/
    sprintf(bfbAttrib.imageDisplayFormat, "%s\\%s,%s", STANDARD, bfbstdCOL, bfbstdROW);
/*
    }
*/
    if (!strcmp(borderDensityC, OTHER)) {
	tempf = atof(border_density_other);
	if (tempf < 0.1 || tempf > 5.0) {
	    (void) displayError("Please Enter a valid Border Density (0.1~5.0)");
	    return;
	}
	tempf = tempf * 100;
	sprintf(bfbAttrib.borderDensity, "%i", (int) tempf);
    } else
	strcpy(bfbAttrib.borderDensity, borderDensityC);
    if (!strcmp(emptyImageDensityC, OTHER)) {
	tempf = atof(empty_image_density_other);
	if (tempf < 0.1 || tempf > 5.0) {
	    (void) displayError("Please Enter a valid Empty Image Density (0.1~5.0)");
	    return;
	}
	tempf = tempf * 100;
	sprintf(bfbAttrib.emptyImageDensity, "%i", (int) tempf);
    } else
	strcpy(bfbAttrib.emptyImageDensity, emptyImageDensityC);

    tempDensity = (unsigned short) (atof(minDensityC) * 100.0);
    if (tempDensity < 10 || tempDensity > 500) {
	(void) displayError("Please Enter a valid Minimum Density (0.1~5.0)");
	return;
    }
#if STANDARD_VERSION < VERSION_AUG1993
    (void) strcpy(bfbAttrib.minDensity, minDensityC);
#else
    bfbAttrib.minDensity = tempDensity;
#endif

    tempDensity = (unsigned short) (atof(maxDensityC) * 100.0);
    if (tempDensity < 10 || tempDensity > 500) {
	(void) displayError("Please Enter a valid Maximum Density field (0.1~5.0)");
	return;
    }
#if STANDARD_VERSION < VERSION_AUG1993
    (void) strcpy(bfbAttrib.maxDensity, maxDensityC);
#else
    bfbAttrib.maxDensity = tempDensity;
#endif

    bfbAttrib.bfbAttributeFlag = PRN_BFB_K_IMAGEDISPLAYFORMAT |
	PRN_BFB_K_FILMORIENTATION | PRN_BFB_K_FILMSIZEID |
	PRN_BFB_K_MAGNIFICATIONTYPE | PRN_BFB_K_MINDENSITY |
	PRN_BFB_K_EMPTYIMAGEDENSITY | PRN_BFB_K_BORDERDENSITY |
	PRN_BFB_K_MAXDENSITY | PRN_BFB_K_TRIM |
	PRN_BFB_K_REFERENCEDBASICFILMSESSIONSEQ;

    cond = createFilmBox(&association, &params, &bfbAttrib, SOPClass, &retStat);
    if (cond != SRV_NORMAL) {
	printf("Error: sendCreateFilmBox failed\n");
	(void) displayError("Comunications Failure with Print Server.");
	return;
    }
    if (retStat != MSG_K_SUCCESS) {
	displayErrorStatus("BFB", "NCreate", retStat);
	return;
    }
    printf("Success\n");
    XtPopdown(bfbshell);
    XtPopup(imageattshell, XtGrabNone);
    XtPopup(previewshell, XtGrabNone);
    XtPopup(studyshell, XtGrabNone);
    if ((bibshell = CreateStandardBFBWindow(row, col)) == NULL) {
	printf("Error: CreateStandardBFBWindow failed\n");
	exit(0);
    }
    XtPopup(bibshell, XtGrabNone);
}


/* CreateBasicFilmBoxWindow
**
** Purpose:
**	This function creates the Basic Film Box Window.
**
** Parameter Dictionary:
**	None
**
** Return Values:
**	TRUE	Success
**	FALSE	Failure
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
Boolean
CreateBasicFilmBoxWindow()
{
    Widget
	workform,
	form1,
	form2;

    static String message[] = {
	"Basic Film Box Attributes:"
    };

    static MessageBox mb = {
	"BASIC FILM BOX ATTRIBUTES",
	message,
	DIM_OF(message),
	NULL, NULL, NULL, NULL, NULL,
	CreateBasicFilmBox,
	"Create",
    TRUE};

    bfbshell = CreateMessageBoxPopup(&mb, toplevel, TRUE);

    form1 = CreateEntryForm(mb.workform, mb.lastlabel, NULL,
			    basic_film_box,
			    DIM_OF(basic_film_box));
    form2 = CreateEntryForm(mb.workform, mb.lastlabel, form1,
			    basic_film_box2,
			    DIM_OF(basic_film_box2));
    if (!bfbshell || !form1 || !form2)
	return (FALSE);
    return (TRUE);
}


/* CreateImageAttributeWindow
**
** Purpose:
**	Describe the purpose of the function
**
** Parameter Dictionary:
**	None
**
** Return Values:
**	TRUE
**	FALSE
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
Boolean
CreateImageAttributeWindow()
{
    Widget
	workform,
	form1;

    static String message[] = {
	"Image Attribute:"
    };

    static MessageBox mb = {
	"IMAGE ATTRIBUTE",
	message,
	DIM_OF(message),
	NULL, NULL, NULL, NULL, NULL,
	NULL,
	NULL,
    FALSE};

    imageattshell = CreateMessageBoxPopup(&mb, toplevel, FALSE);
    form1 = CreateEntryForm(mb.workform, mb.lastlabel, NULL,
			    image_attributes,
			    DIM_OF(image_attributes));
    if (!imageattshell || !form1)
	return (FALSE);
    return (TRUE);
}

main(int argc, char *argv[])
{
    CONDITION
	cond;
    CTNBOOLEAN
	verbose = FALSE;

    static String firstmessage[] = {
	"Select a printer from the list:"
    };

    static MessageBox mb1 = {
	"Printer Selection",
	firstmessage,
	DIM_OF(firstmessage),
	NULL, NULL, NULL, NULL, NULL,
	Quit,
	"Quit",
    FALSE};
    DCMPrinter
	* printerlist;
    int
        numprinters;
    char
       *printerConfigFile = NULL;
    char
       *Login;
    char
       *ctn = NULL;
    Widget
	printerlistWidget;
    String
	printerlistlabels[40];
    int
        i;

    toplevel = XtAppInitialize(	/* create application context */
			       &app_context,
			       "Print Manager",
			       NULL, 0,
			       &argc, argv,
			       NULL,
			       NULL, 0);

    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'a':
	    argv++;
	    argc--;
	    callingAPTitle = strdup(argv[0]);
	    break;
	case 'v':
	    printf("Forcing all facilities into verbose mode.\n");
	    verbose = TRUE;
	    break;
	case 'C':
	    argv++;
	    argc--;
	    ctn = strdup(argv[0]);
	    break;
	case 'c':
	    argv++;
	    argc--;
	    printerConfigFile = strdup(argv[0]);
	    break;
	case 'D':
	    argv++;
	    argc--;
	    db_file = strdup(argv[0]);
	    break;
	case 'I':
	    argv++;
	    argc--;
	    icon_index = strdup(argv[0]);
	    break;
	case 'F':
	    argv++;
	    argc--;
	    icon_file = strdup(argv[0]);
	    break;
	case 'p':
	    argv++;
	    argc--;
	    calledAPTitle = strdup(argv[0]);
	    break;
	case 's':
	    silent = TRUE;
	    break;
	case 'S':
	    printf("BIBs sent as they are filled with an image.\n");
	    Send_BIB_when_print = FALSE;
	    break;
	case 'z':
	    argv++;
	    argc--;
	    if (!strcmp("dcm", argv[0])) {
		printf("Setting DCM Facility to verbose mode\n");
		DCM_Debug(TRUE);
	    } else if (!strcmp("srv", argv[0])) {
		printf("Setting SRV Facility to verbose mode\n");
		SRV_Debug(TRUE);
	    } else if (!strcmp("dul", argv[0])) {
		printf("Setting DUL Facility to verbose mode\n");
		DUL_Debug(TRUE);
	    } else
		printf("Unknown option following -z\n");
	    break;
	default:
	    break;
	}
    }

    if (argc != 1) {
	UsageError();
    }
    THR_Init();
    DUL_Debug(verbose);
    SRV_Debug(verbose);
    DCM_Debug(verbose);
    Login = strdup(argv[0]);
    if (ctn == NULL) {
	char temp[40];
	(void) gethostname(temp, 40);
	ctn = strdup(temp);
    }
    if (printerConfigFile == NULL)
	printerConfigFile = strdup(DEFAULT_PRINT_CONFIG);
    if (initNetwork(&params, &network) != DUL_NORMAL) {
	printf("Error: Failed to Initialize Network");
	THR_Shutdown();
	exit(0);
    }
    if (ParsePrintConfigFile(printerConfigFile, ctn, Login, &printerlist,
			     &numprinters) != TRUE) {
	printf("Error: Failed to Open %s\n\n", printerConfigFile);
	THR_Shutdown();
	exit(0);
    }
    if (numprinters == 0) {
	printf("Cannot find you Login Name in %s\n\n", printerConfigFile);
	THR_Shutdown();
	exit(0);
    }
    for (i = 0; i < numprinters; i++)
	printerlistlabels[i] = printerlist[i].CalledApTitle;
    mainwin = CreateMessageBox(&mb1, toplevel, TRUE);
    if (mainwin == 0) {
	printf("Error: Could not create main window widget\n");
	THR_Shutdown();
	exit(1);
    }
    printerlistWidget = CreateSimpleList(mb1.workform, mb1.lastlabel, NULL,
	100, 100, SelectPrinter, (XtPointer) printerlist, printerlistlabels,
					 numprinters, FALSE);
    if (printerlistWidget == 0) {
	printf("Error: Could not create simple list\n");
	THR_Shutdown();
	exit(1);
    }
    if (CreateBasicFilmSessionWindow() == FALSE) {
	printf("Error: CreateBasicFilmSessionWindow failed\n");
	THR_Shutdown();
	exit(0);
    }
    if (CreateBasicFilmBoxWindow() == FALSE) {
	printf("Error: CreateBasicBoxWindow failed\n");
	THR_Shutdown();
	exit(0);
    }
    if (CreateImageAttributeWindow() == FALSE) {
	printf("Error: CreateImageAttributeWindow failed\n");
	THR_Shutdown();
	exit(0);
    }
    if ((previewshell = CreatePreviewStudyWindow()) == NULL) {
	printf("Error: CreatePreviewStudyWindow failed\n");
	THR_Shutdown();
	exit(0);
    }
    if ((studyshell = CreateStudyListWindow()) == NULL) {
	printf("Error: CreateStudyListWindow failed\n");
	THR_Shutdown();
	exit(0);
    }
    XtRealizeWidget(toplevel);
    XtAppMainLoop(app_context);
    THR_Shutdown();
    return 0;
}
#endif
#ifndef ATHENA

void
main()
{
    printf("This Application cannot execute without the Athena Widget Set\n");
}
#endif
