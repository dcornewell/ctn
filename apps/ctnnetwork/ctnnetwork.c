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
** Module Name(s):	ctnnetwork (main()), UpdateSlot, ReleaseSlot,
**			FindSlot, GetUnusedSlot, CTNNETWORK_CheckStatus
** Author, Date:	David E. Beecher, 7-Jul-93
** Intent:		A simple network monitor for CTN's
** Last Update:		$Author: smm $, $Date: 1999-11-02 21:22:35 $
** Source File:		$RCSfile: ctnnetwork.c,v $
** Revision:		$Revision: 1.26 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.26 $ $RCSfile: ctnnetwork.c,v $";

#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#ifdef X11
#include        <X11/Xlib.h>
#include        <X11/Xutil.h>
#include        <X11/Xos.h>
#include        <X11/Xatom.h>

#include	"dicom.h"
#include	"ctnthread.h"
#include	"condition.h"
#include	"lst.h"
#include	"dicom_objects.h"
#include	"iqueues.h"
#include 	"gq.h"
#include 	"ctnnetwork.h"

#define 	SLEEP_TIME	((unsigned) 100000)

char *CTNNETWORK_usage = "CTNNETWORK Usage: ctnnetwork -q queue_number -n number_to_display [-w display_width -h display_height]\n";
/*
 * Globals (preceded with G_)...declared extern in other files
 */
Window G_big_window;

Display *G_display;		/* Xstuff used all over...	 */

Colormap G_cmap;

GC G_gclargeT,			/* Graphics contexts needed...	 */
G_gcgreen;

int G_display_width,
    G_display_height,		/* Well know screen stuff...	 */
    G_screen;

char G_nodename[MAX_STRING_LENGTH];
char G_dpnname[MAX_STRING_LENGTH];

CTNNETWORK_Queue
*G_Array;
int
    G_Number,
   *G_Used;

/*
 * End Globals...
 */

static void
    CTNNETWORK_CheckStatus();
static void
    ReleaseSlot(CTNNETWORK_Queue *element);
static void
    UpdateSlot(int index, CTNNETWORK_Queue *element);

/*******************************************************************/
main(int argc, char **argv)
{
    CONDITION
	retval;
    CTNNETWORK_Queue
        element;
    int
        i,
        QueueElementSize,
        diag_level,
        image_Q_id = 0;

    int
        slot;

    THR_Init();
    QueueElementSize = sizeof(CTNNETWORK_Queue);

/*
 * Get the input paramaters from command line
 */
    if (argc < 3) {
	COND_PushCondition(CTNNETWORK_CMDLINE, "%s", CTNNETWORK_usage);
	COND_DumpConditions();
	exit(-1);
    }
    G_display_width = G_display_height = -1;
    G_Number = 0;
    argc--;
    argv++;
    while (argc > 0) {
	if ((strcmp(*argv, "-q")) == 0) {
	    argc--;
	    argv++;
	    image_Q_id = atoi(*argv);
	    if (image_Q_id < 0) {
		COND_PushCondition(CTNNETWORK_CMDLINE,
				   "CTNNETWORK: Queue number must > 0");
		COND_DumpConditions();
		exit(-1);
	    }
	    argc--;
	    argv++;
	} else if ((strcmp(*argv, "-w")) == 0) {
	    argc--;
	    argv++;
	    G_display_width = atoi(*argv);
	    if (G_display_width < MIN_DISPLAY_WIDTH) {
		COND_PushCondition(CTNNETWORK_CMDLINE,
		   "CTNNETWORK: Display width must be > MIN_DISPLAY_WIDTH");
		COND_DumpConditions();
		exit(-1);
	    }
	    argc--;
	    argv++;
	} else if ((strcmp(*argv, "-h")) == 0) {
	    argc--;
	    argv++;
	    G_display_height = atoi(*argv);
	    if (G_display_height < MIN_DISPLAY_HEIGHT) {
		COND_PushCondition(CTNNETWORK_CMDLINE,
		 "CTNNETWORK: Display height must be > MIN_DISPLAY_HEIGHT");
		COND_DumpConditions();
		exit(-1);
	    }
	    argc--;
	    argv++;
	} else if ((strcmp(*argv, "-n")) == 0) {
	    argc--;
	    argv++;
	    strcpy(G_nodename, *argv);
	    G_Number = atoi(*argv);
	    argc--;
	    argv++;
	} else {
	    COND_PushCondition(CTNNETWORK_CMDLINE, CTNNETWORK_usage);
	    COND_DumpConditions();
	    exit(-1);
	}
    }
    if (G_Number < 1) {
	G_Number = 10;
    }
    if ((G_Array = (CTNNETWORK_Queue *) malloc(G_Number * QueueElementSize)) ==
	(CTNNETWORK_Queue *) NULL) {
	COND_PushCondition(CTNNETWORK_NOMEMORY,
			"CTNNETWORK: Can't allocate local memory for list");
	COND_DumpConditions();
	exit(-1);
    }
    if ((G_Used = (int *) malloc(G_Number * sizeof(int))) == (int *) NULL) {
	COND_PushCondition(CTNNETWORK_NOMEMORY,
			"CTNNETWORK: Can't allocate local memory for list");
	COND_DumpConditions();
	exit(-1);
    }
    for (i = 0; i < G_Number; i++) {	/* Set all the slots to open */
	G_Used[i] = 0;
    }

/*
 * Initialize the Generalized Queue for images and status messages
 */
    if (GQ_GetQueue(image_Q_id, QueueElementSize) != GQ_NORMAL) {
	if ((GQ_InitQueue(image_Q_id, QUEUE_SIZE, QueueElementSize)) != GQ_NORMAL) {
	    COND_PushCondition(CTNNETWORK_NOGQ,
			       "CTNNETWORK: Not able to acess Image Queue.");
	    COND_DumpConditions();
	    exit(-1);
	}
	if (GQ_GetQueue(image_Q_id, QueueElementSize) != GQ_NORMAL) {
	    COND_PushCondition(CTNNETWORK_NOGQ,
			       "CTNNETWORK: Not able to get Image Queue.");
	    COND_DumpConditions();
	    exit(-1);
	}
    }
/*
 * Initialize the X stuff, and create the graphics contexts
 */
    if ((CTNNETWORK_InitializeX()) != CTNNETWORK_NORMAL) {
	COND_PushCondition(CTNNETWORK_NOXWINDOWS,
			   "CTNNETWORK: No Connection to X-Server.");
	(void) GQ_KillQueue(image_Q_id);
	COND_DumpConditions();
	exit(-1);
    }
/*
 * Display node name and the status buttons...
 */
    CTNNETWORK_CreateNetworkWindow();

/*
 * Main Control loop for ctnnetwork
 */
    retval = GQ_Dequeue(image_Q_id, (void *) &element);
    strcpy(G_nodename, element.vendorid);
    strcpy(G_dpnname, element.dpnid);
    while (strcmp(element.vendorid, KILL_DISPLAY) != 0) {
	while (retval == GQ_NORMAL) {
	    switch (element.connection) {
	    case CONN_INOPEN:
	    case CONN_OUTOPEN:
		slot = GetUnusedSlot(&element);
		CTNNETWORK_OpenConnection(slot);
		break;
	    case CONN_INXFER:
	    case CONN_OUTXFER:
		slot = FindSlot(&element);
		UpdateSlot(slot, &element);
		CTNNETWORK_ShowPercentage(slot);
		break;
	    case CONN_INCLOSE:
	    case CONN_OUTCLOSE:
		slot = FindSlot(&element);
		UpdateSlot(slot, &element);
		CTNNETWORK_CloseConnection(slot);
		ReleaseSlot(&element);
		break;
	    case CONN_INDISPLAY:
	    case CONN_OUTDISPLAY:
	    default:
		break;
	    }
	    retval = GQ_Dequeue(image_Q_id, (void *) &element);
	}
#ifdef IRIX
	(void) sginap(6);
#elif defined(USLEEP)
	(void) usleep(SLEEP_TIME);
#else
	(void) sleep(1);
#endif
	retval = GQ_Dequeue(image_Q_id, (void *) &element);
	strcpy(G_dpnname, element.dpnid);
	strcpy(G_nodename, element.vendorid);

	CTNNETWORK_CheckStatus();
	(void)COND_PopCondition(TRUE);
    }
    THR_Shutdown();
    return 0;
}



/*******************************************************************/
static void
CTNNETWORK_CheckStatus()
{

    CTNNETWORK_Queue
        element;

    XEvent
	report;

    unsigned long
        mask_event;

    CONDITION
	val;

/*
 * Check for X events that require redrawing the screen...
 */
    mask_event = StructureNotifyMask | SubstructureNotifyMask | ExposureMask;
    while (XCheckMaskEvent(G_display, mask_event, &report) == True) {

	CTNNETWORK_RefreshStatus();
    }

    return;
}

int
GetUnusedSlot(CTNNETWORK_Queue *element)
{

    int
        i;

    for (i = 0; i < G_Number; i++) {
	if (G_Used[i] == 0) {
	    G_Used[i] = 1;
	    strcpy(G_Array[i].vendorid, element->vendorid);
	    strcpy(G_Array[i].dpnid, element->dpnid);
	    G_Array[i].association_id = element->association_id;
	    G_Array[i].connection = element->connection;
	    G_Array[i].percentage = element->percentage;
	    return (i);
	}
    }
    return (-1);
}

int
FindSlot(CTNNETWORK_Queue *element)
{

    int
        i;

    for (i = 0; i < G_Number; i++) {
	if (G_Used[i] == 1) {
	    if (G_Array[i].association_id == element->association_id) {
		return (i);
	    }
	}
    }
    return (-1);
}

static void
ReleaseSlot(CTNNETWORK_Queue *element)
{

    int
        i;

    for (i = 0; i < G_Number; i++) {
	if (G_Used[i] == 1) {
	    if (G_Array[i].association_id == element->association_id) {
		G_Used[i] = 0;
		return;
	    }
	}
    }
    return;
}
/*lint -e578*/

static void
UpdateSlot(int index, CTNNETWORK_Queue *element)
{


    if (index < 0 || index > G_Number)
	return;
    strcpy(G_Array[index].vendorid, element->vendorid);
    strcpy(G_Array[index].dpnid, element->dpnid);
    G_Array[index].association_id = element->association_id;
    G_Array[index].connection = element->connection;
    G_Array[index].percentage = element->percentage;
    return;
}
/*lint +e578*/

#else

main()
{
    fprintf(stderr, "ctnnetwork requires the X Window System\n");
    fprintf(stderr, "This version was compiled without the proper MACRO\n");
    fprintf(stderr, "defined to use X11.  Please refer to the source code\n");
    fprintf(stderr, "(and recompile) to use this application\n");
}
#endif
