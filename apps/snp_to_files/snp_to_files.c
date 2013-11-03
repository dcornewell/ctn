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
**                              DICOM 94
**                   Electronic Radiology Laboratory
**                 Mallinckrodt Institute of Radiology
**              Washington University School of Medicine
**
** Module Name(s):	main
**			callbackState
**			callbackITOA
**			callbackATOI
**			usageerror
** Author, Date:	Nilesh R. Gohel, 14-Sep-1994
** Intent:		This program allows the user to snoop on one or more
**			associations
**			at the TCP/IP level on a shared media network given the
**			name of the initiator, name of the acceptor, and the port
**			number on the acceptor. Additional information required
**			includes information on the network interface to be used
**			and size of buffers for temporary stroage of data. Data
**			from the initiator to the acceptor is stored in a file
**			and data from the acceptor to the initiator stored in
**			another. Names of these files are also required.
**			The application uses the SNP facility for the snooping.
**
** Last Update:         $Author: smm $, $Date: 1998-08-03 16:27:11 $
** Source File:         $RCSfile: snp_to_files.c,v $
** Revision:            $Revision: 1.9 $
** Status:              $State: Exp $
*/

static char rcsid[] = "$Revision: 1.9 $ $RCSfile: snp_to_files.c,v $";

#include <stdio.h>
#include <stdlib.h>
#ifdef SUNOS
#include <sys/types.h>
#endif
#include <sys/stat.h>
#include <fcntl.h>

#include "dicom.h"
#include "ctnthread.h"
#include "lst.h"
#include "condition.h"
#include "snp.h"

/*
	Prototypes for internal functions
*/
static void usageerror();
static CONDITION callbackState(int state, void *ctx);
static CONDITION callbackITOA(char *buffer, int bufsize, void *ctx);
static CONDITION callbackATOI(char *buffer, int bufsize, void *ctx);
static int write_end_assoc();

volatile int assoc;		/* Associations left to monitor */
static int current_state = NORMAL;	/* Current state reported by SNP
					 * facilities */
static int fd_atoi;		/* File descriptor for initiator -> acceptor
				 * data */
static int fd_itoa;		/* File descriptor for acceptor -> initiator
				 * data */
static TCP_BUF_HEAD buf_head;	/* Header information for each TCP data
				 * buffer */

main(int argc, char *argv[])
{
    CONDITION cond;		/* Variable to track conditions returned */

    /*
     * Check for correct usage
     */
    if (argc < 10)
	usageerror();

    /*
     * Re-iterate user entries
     */
    printf("Device = %s\n", argv[1]);
    printf("PPA = %d\n", atoi(argv[2]));
    printf("Initiator = %s\n", argv[3]);
    printf("Acceptor = %s\n", argv[4]);
    printf("Acceptor port = %d\n", atoi(argv[5]));
    printf("Buffer size (longs) = %d\n", atoi(argv[6]));
    assoc = atoi(argv[7]);
    printf("Associations = %d\n", assoc);
    printf("Initiator -> Acceptor data file = %s\n", argv[8]);
    printf("Acceptor -> Initiator data file = %s\n", argv[9]);
    printf("\n\n");

    if (assoc < 1)
	fprintf(stderr, "Need to monitor at least one association\n");

    /*
     * Initialize sequence numbering for TCP data buffers
     */
    buf_head.seq = 0;

    /*
     * Place SNP facility in debug mode - turn off for now
     */
    THR_Init();
    SNP_Debug(FALSE);

    /*
     * Initialize SNP facilities
     */
    cond = SNP_Init();
    if (cond != SNP_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    /*
     * Register callback functions
     */
    cond = SNP_RegisterCallback(callbackState, SNP_CALLBACK_STATE, NULL);
    if (cond != SNP_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    cond = SNP_RegisterCallback(callbackITOA, SNP_CALLBACK_ITOA, NULL);
    if (cond != SNP_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    cond = SNP_RegisterCallback(callbackATOI, SNP_CALLBACK_ATOI, NULL);
    if (cond != SNP_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    /*
     * Open data files
     */
    if ((fd_itoa = open(argv[8], O_WRONLY | O_CREAT | O_TRUNC, 0666)) == NULL) {
	fprintf(stderr, "Problem opening file %s\n", argv[8]);
	THR_Shutdown();
	exit(1);
    }
    if ((fd_atoi = open(argv[9], O_WRONLY | O_CREAT | O_TRUNC, 0666)) == NULL) {
	fprintf(stderr, "Problem opening file %s\n", argv[9]);
	THR_Shutdown();
	exit(1);
    }
    /*
     * Commence the snooping with given arguements
     */
    cond = SNP_Start(argv[1], atoi(argv[2]), argv[3],
		     argv[4], atoi(argv[5]), 20, 5, atoi(argv[6]));
    if (cond != SNP_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    printf("\nInitialization complete .... ready to monitor communications\n");
    /*
     * Until have monitored correct number of associations or until something
     * goes wrong keep snooping - update user with number of associations to
     * go
     */
    while (assoc > 0) {
	sleep(1);
    }

    /*
     * If finished in a bad state .... something went wrong
     */
    if (current_state != NORMAL) {
	printf("\nError: %s\n", SNP_StateMsg(current_state));
	THR_Shutdown();
	exit(1);
    } else
	printf("\nCompleted monitoring associations normally\n");

    /*
     * Discontinue snooping operations
     */
    cond = SNP_Stop();
    if (cond != SNP_NORMAL)
	COND_DumpConditions();

    /*
     * Terminate activities with SNP facility
     */
    cond = SNP_Terminate();
    if (cond != SNP_NORMAL)
	COND_DumpConditions();

    /*
     * Close file descriptors
     */
    close(fd_itoa);
    close(fd_atoi);

    THR_Shutdown();
    exit(0);
}

/* callbackState
**
** Purpose:
**	Used to report the current state of the underlying SNP
**	facilities.
**
** Parameter Dictionary:
**	state	The state number
**	ctx	Context pointer
**
** Return Values:
**	SNP_NORMAL
**	SNP_DONE
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes
*/
CONDITION
callbackState(int state, void *ctx)
{

    current_state = state;
    switch (state) {
    case END_ASSOC:
	if (write_end_assoc()) {
	    fprintf(stderr, "Error writing End of Association\n");
	    return SNP_DONE;
	}
	current_state = NORMAL;
	assoc--;
	printf("\n%d associations to go\n", assoc);
	if (!assoc) {
	    return SNP_DONE;
	}
	break;
    case NORMAL:
	break;
    default:
	assoc = 0;
	return SNP_DONE;
    }
    return SNP_NORMAL;
}


/* callbackITOA
**
** Purpose:
**	Used to dump parsed data from underlying SNP facilities out to
**	appropriate file. Data handled by this function are from
**	the initiator to the acceptor.
**
** Parameter Dictionary:
**	fd_itoa (global)	File descriptor to write data to
**	buffer			Pointer to data being passed
**	bufsize 		Size of data buffer being passed
**	ctx			Context pointer
**
** Return Values:
**	SNP_NORMAL
**	SNP_CALLBACKFAIL
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
callbackITOA(char *buffer, int bufsize, void *ctx)
{
    /*
     * Construct header to add to each TCP data buffer placed in file. Header
     * provides direction, sequence number, and length of buffer. Write
     * buffer to file.
     */
    buf_head.type = ITOA;
    buf_head.len = bufsize;
    if ((write(fd_itoa, (void *) &buf_head, 12)) != 12)
	return SNP_CALLBACKFAIL;
    /*
     * Write data buffer to file
     */
    if ((write(fd_itoa, (void *) buffer, bufsize)) != bufsize)
	return SNP_CALLBACKFAIL;
    /*
     * Increment sequence number
     */
    buf_head.seq++;
    return SNP_NORMAL;
}


/* callbackATOI
**
** Purpose:
**	Used to dump parsed data from underlying SNP facilities out to
**	appropriate file. Data handled by this function are from
**	the acceptor to the initiator.
**
** Parameter Dictionary:
**	fd_atoi (global)	File descriptor to write data to
**	buffer			Pointer to data being passed
**	bufsize 		Size of data buffer being passed
**	ctx			Context pointer
**
** Return Values:
**	SNP_NORMAL
**	SNP_CALLBACKFAIL
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
callbackATOI(char *buffer, int bufsize, void *ctx)
{
    /*
     * Construct header to add to each TCP data buffer placed in file. Header
     * provides direction, sequence number, and length of buffer. Write
     * buffer to file.
     */
    buf_head.type = ATOI;
    buf_head.len = bufsize;
    if ((write(fd_atoi, (void *) &buf_head, 12)) != 12)
	return SNP_CALLBACKFAIL;
    /*
     * Write data buffer to file
     */
    if ((write(fd_atoi, (void *) buffer, bufsize)) != bufsize)
	return SNP_CALLBACKFAIL;
    /*
     * Increment sequence number
     */
    buf_head.seq++;
    return SNP_NORMAL;
}

/* write_end_assoc
**
** Purpose:
**	Writes a header to file for parsed ini->acc data indicating
**	an END OF ASSOCIATION at the TCP/IP level (socket closed).
**
** Parameter Dictionary:
**
**
** Return Values:
**	0			Success
**	1			Failure
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static int
write_end_assoc()
{
    buf_head.type = SNP_EOA;
    buf_head.len = 0;
    if ((write(fd_itoa, (void *) &buf_head, 12)) != 12)
	return 1;
    buf_head.seq++;
    return 0;
}


/* usageerror
**
** Purpose:
**	Print the usage string for this application and exit.
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

    fprintf(stderr, "Usage:\n\tsnp_to_files device ppa initiator acceptor port buffersize(longs) associations ini->acc_file acc->ini_file\n\n");
    exit(1);
}
