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
**			callITOA_PDU
**			callATOI_PDU
**			sortDataPDU
**			parseCmdITOA
**			parseCmdATOI
**			parseDataITOA
**			parseDataATOI
**			parseAbort
**			usageerror
** Author, Date:	Nilesh R. Gohel, 14-Sep-1994
** Intent:		This program allows the user to snoop on one or more
**			associations at the DICOM level.
**			The snooping operation may be on a shared media network
**			in real-time given the name of the initiator, name of the
**			acceptor, and the TCP port number on the acceptor.
**			Additional information required includes information on
**			the network interface to be used and size of buffers for
**			temporary stroage of data.
**			Alternatively TCP/IP data files may be parsed for DICOM
**			protocol data units (PDUs). Two data files are provided.
**			One containing data from the initiator to the acceptor
**			and the other provided data in the other direction.
**			These files are generated using the snp_to_files application
**			which provides real-time monitoring of communications
**			at the TCP/IP level to data files.
**
** Last Update:         $Author: smm $, $Date: 2001-12-21 16:37:31 $
** Source File:         $RCSfile: dcm_snoop.c,v $
** Revision:            $Revision: 1.15 $
** Status:              $State: Exp $
*/

static char rcsid[] = "$Revision: 1.15 $ $RCSfile: dcm_snoop.c,v $";

#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef SUNOS
#include <sys/types.h>
#endif
#include <sys/stat.h>
#include <fcntl.h>
#endif

#include "dicom.h"
#include "ctnthread.h"
#include "lst.h"
#include "dicom_uids.h"
#include "condition.h"
#include "snp.h"
#include "dicom_objects.h"
#include "dulprotocol.h"

/*
	Prototypes for internal functions
*/
static void usageerror();
static void
callITOA_PDU(int ini_state, int acc_state, u_char pdu_type,
    char *buf, int len, DUL_ASSOCIATESERVICEPARAMETERS * params, void *ctx);

static void
callATOI_PDU(int ini_state, int acc_state, u_char pdu_type,
    char *buf, int len, DUL_ASSOCIATESERVICEPARAMETERS * params, void *ctx);

static void
    sortDataPDU(u_char * buf, int len, int dir_flag);

static void
    parseCmdITOA(u_char * buf, u_long pdvLength);

static void
    parseCmdATOI(u_char * buf, u_long pdvLength);

static void
    parseDataITOA(u_char * buf, u_long pdvLength);

static void
    parseDataATOI(u_char * buf, u_long pdvLength);

static void
    parseAbort(u_char * buf, int len);


int
main(int argc, char *argv[])
{
    CONDITION cond;		/* Variable to track conditions returned */

    /*
     * Check for correct usage
     */
    if ((argc != 3) && (argc != 5))
	if (argc < 8)
	    usageerror();

    /*
     * Place SNP and DUL facilities in debug mode - not for now
     */
    THR_Init();
    DUL_Debug(FALSE);
    SNP_Debug(FALSE);

    /*
     * Register callbacks for DICOM PDUs
     */
    if ((DUL_RegPDUCall(callITOA_PDU, CALLBACK_ITOA_PDU, NULL)) != DUL_NORMAL) {
	fprintf(stderr, "Problem registering callback for ITOA PDUs\n");
	COND_DumpConditions();
	THR_Shutdown();
	return (1);
    }
    if ((DUL_RegPDUCall(callATOI_PDU, CALLBACK_ATOI_PDU, NULL)) != DUL_NORMAL) {
	fprintf(stderr, "Problem registering callback for ATOI PDUs\n");
	COND_DumpConditions();
	THR_Shutdown();
	return (1);
    }
    if ((argc == 3) || (argc == 5)) {
	if (argc == 3) {
	    cond = DUL_FileSnoop(argv[1], argv[2], NULL, NULL);
	    if (cond != DUL_NORMAL) {
		COND_DumpConditions();
		THR_Shutdown();
		return (1);
	    }
	} else {
	    cond = DUL_FileSnoop(argv[1], argv[2], argv[3], argv[4]);
	    if (cond != DUL_NORMAL) {
		COND_DumpConditions();
		THR_Shutdown();
		return (1);
	    }
	}
    } else {
	cond = DUL_NetworkSnoop(argv[1], atoi(argv[2]), argv[3],
		      argv[4], atoi(argv[5]), atoi(argv[6]), atoi(argv[7]));
	if (cond != DUL_NORMAL) {
	    COND_DumpConditions();
	    THR_Shutdown();
	    return (1);
	}
    }

    THR_Shutdown();
    return (0);
}


/* callITOA_PDU
**
** Purpose:
**	Callback for PDU in direction of ini->acc.
**
** Parameter Dictionary:
**	pdu_type 	Type of PDU
**	buf 		Pointer to data of PDU
**	len		Length of PDU
**	ctx		Context pointer for callback functions
**
** Return Values:
**
**	0		Success
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static void
callITOA_PDU(int ini_state, int acc_state, u_char pdu_type,
     char *buf, int len, DUL_ASSOCIATESERVICEPARAMETERS * params, void *ctx)
{

    switch (pdu_type) {
	case DUL_TYPEASSOCIATERQ:printf("ini->acc = A_ASSOC_RQ PDU\n");
	printf("\nAssociations params:\n\n");
	DUL_DumpParams(params);
	printf("\n");
	break;
    case DUL_TYPEASSOCIATEAC:
	printf("ini->acc = A_ASSOC_AC PDU\n");
	printf("\nAssociations params:\n\n");
	DUL_DumpParams(params);
	printf("\n");
	break;
    case DUL_TYPEASSOCIATERJ:
	printf("ini->acc = A_ASSOC_RJ PDU\n");
	printf("\nAssociations params:\n\n");
	DUL_DumpParams(params);
	printf("\n");
	break;
    case DUL_TYPEDATA:
	printf("ini->acc = P_DATA_TF PDU\n");
	sortDataPDU((u_char *) buf, len, ITOA);
	break;
    case DUL_TYPERELEASERQ:
	printf("ini->acc = A_REL_RQ PDU\n");
	break;
    case DUL_TYPERELEASERP:
	printf("ini->acc = A_REL_RP PDU\n");
	break;
    case DUL_TYPEABORT:
	printf("ini->acc = A_ABORT PDU\n");
	parseAbort((u_char *) buf, len);
	break;
    default:
	printf("ini->acc = UNKNOWN PDU\n");
	break;
    }
    printf("DUL states: Initiator = %d\tAcceptor = %d\n\n\n",
	   ini_state, acc_state);

}

/* callATOI_PDU
**
** Purpose:
**	Callback for PDU in direction of acc->ini.
**
** Parameter Dictionary:
**	pdu_type 	Type of PDU
**	buf 		Pointer to data of PDU
**	len		Length of PDU
**	ctx		Returned context pointer for callbacks
**
** Return Values:
**
**	0		Success
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static void
callATOI_PDU(int ini_state, int acc_state, u_char pdu_type,
     char *buf, int len, DUL_ASSOCIATESERVICEPARAMETERS * params, void *ctx)
{

    switch (pdu_type) {
	case DUL_TYPEASSOCIATERQ:printf("acc->ini = A_ASSOC_RQ PDU\n");
	printf("\nAssociations params:\n\n");
	DUL_DumpParams(params);
	printf("\n");
	break;
    case DUL_TYPEASSOCIATEAC:
	printf("acc->ini = A_ASSOC_AC PDU\n");
	printf("\nAssociations params:\n\n");
	DUL_DumpParams(params);
	printf("\n");
	break;
    case DUL_TYPEASSOCIATERJ:
	printf("acc->ini = A_ASSOC_RJ PDU\n");
	printf("\nAssociations params:\n\n");
	DUL_DumpParams(params);
	printf("\n");
	break;
    case DUL_TYPEDATA:
	printf("acc->ini = P_DATA_TF PDU\n");
	sortDataPDU((u_char *) buf, len, ATOI);
	break;
    case DUL_TYPERELEASERQ:
	printf("acc->ini = A_REL_RQ PDU\n");
	break;
    case DUL_TYPERELEASERP:
	printf("acc->ini = A_REL_RP PDU\n");
	break;
    case DUL_TYPEABORT:
	printf("acc->ini = A_ABORT PDU\n");
	parseAbort((u_char *) buf, len);
	break;
    default:
	printf("acc->ini = UNKNOWN PDU\n");
	break;
    }
    printf("DUL states: Initiator = %d\tAcceptor = %d\n\n\n",
	   ini_state, acc_state);
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

    fprintf(stderr, "Usage:\n\nFor reading data from files:\n\n");
    fprintf(stderr, "\tdcm_snoop ini->acc_file acc->ini_file");
    fprintf(stderr, " [intiator acceptor]\n\n\n");
    fprintf(stderr, "For real-time type operation:\n\n");
    fprintf(stderr, "\tdcm_snoop device ppa initiator acceptor port buffersize(longs) associations\n\n");
    exit(1);
}


/* sortDataPDU
**
** Purpose:
**	Clearing house for the parsing of P-Data PDUs containing
**	data or command fragments in either ini->acc or acc->ini
**	directions
**
** Parameter Dictionary:
**	buf	 	Pointer to data of PDU
**	len		Length of PDU
**	dir_flag 	Indicates direction of PDU as ITOA or ATOI
**
** Return Values:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static void
sortDataPDU(u_char * buf, int len, int dir_flag)
{
    u_long pdvLength;		/* Length of PDV fragment found in PDU */


    len -= 6;
    buf += 6;
    /* For all the PDVs in the PDU */
    do {
	memcpy((char *) &pdvLength, buf, 4);
	/* Check if command or data fragment - parse accordingly */
	if (buf[5] & 0x01) {
	    /* Command fragment */
	    printf("Command PDV fragment : Length = %u\n", pdvLength);
	    if (dir_flag == ITOA)
		parseCmdITOA(buf, pdvLength);
	    else
		parseCmdATOI(buf, pdvLength);
	} else {
	    /* Dataset fragment */
	    printf("Data set PDV fragment : Length = %u\n", pdvLength);
	    if (dir_flag == ITOA)
		parseDataITOA(buf, pdvLength);
	    else
		parseDataATOI(buf, pdvLength);
	}
	buf += (pdvLength + 4);
	len = len - (int) pdvLength - 4;
    } while (len > 0);

}


/* parseCmdITOA
**
** Purpose:
**	Gather command PDV fragments in ini->acc direction. When all
**	fragments have been gathered in buffer, create DICOM object
**	and dump elements.
**
** Parameter Dictionary:
**	buf	 	Pointer to data of PDV
**	pdvLength	Length of PDV fragment
**
** Return Values:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static void
parseCmdITOA(u_char * buf, u_long pdvLength)
{

    static u_char stream[8192];
    static u_long ptr;
    DCM_OBJECT *object;
    CONDITION cond;
    static DUL_PRESENTATIONCONTEXTID ctxID = 0;


    if (!ptr) {
	ctxID = buf[4];
    } else {
	if (ctxID != buf[4]) {
	    fprintf(stderr, "Unexpected presentation context ID\n");
	    exit(1);
	}
    }
    if (ptr + pdvLength - 2 > sizeof(stream)) {
	fprintf(stderr, "Command buffer overflow\n");
	exit(1);
    }
    (void) memcpy(&stream[ptr], &buf[6], pdvLength - 2);
    ptr = ptr + pdvLength - 2;
    if (buf[5] & 0x02) {
	/*
	 * Last fragment Dump elements
	 */
	cond = DCM_ImportStream(stream, ptr,
				DCM_ORDERLITTLEENDIAN, &object);
	if (cond != DCM_NORMAL) {
	    COND_DumpConditions();
	    fprintf(stderr, "DCM_ImportStream failed\n");
	    exit(1);
	}
	printf("\nCommand ini->acc :\n");
	(void) DCM_DumpElements(&object, 0);
	ptr = 0;
    }
}


/* parseCmdATOI
**
** Purpose:
**	Gather command PDV fragments in acc->ini direction. When all
**	fragments have been gathered in buffer, create DICOM object
**	and dump elements.
**
** Parameter Dictionary:
**	buf	 	Pointer to data of PDV
**	pdvLength	Length of PDV fragment
**
** Return Values:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static void
parseCmdATOI(u_char * buf, u_long pdvLength)
{

    static u_char stream[8192];
    static u_long ptr;
    DCM_OBJECT *object;
    CONDITION cond;
    static DUL_PRESENTATIONCONTEXTID ctxID = 0;


    if (!ptr) {
	ctxID = buf[4];
    } else {
	if (ctxID != buf[4]) {
	    fprintf(stderr, "Unexpected presentation context ID\n");
	    exit(1);
	}
    }
    if (ptr + pdvLength - 2 > sizeof(stream)) {
	fprintf(stderr, "Command buffer overflow\n");
	exit(1);
    }
    (void) memcpy(&stream[ptr], &buf[6], pdvLength - 2);
    ptr = ptr + pdvLength - 2;
    if (buf[5] & 0x02) {
	/*
	 * Last fragment Dump elements
	 */
	cond = DCM_ImportStream(stream, ptr,
				DCM_ORDERLITTLEENDIAN, &object);
	if (cond != DCM_NORMAL) {
	    COND_DumpConditions();
	    fprintf(stderr, "DCM_ImportStream failed\n");
	    exit(1);
	}
	printf("\nCommand acc->ini :\n");
	(void) DCM_DumpElements(&object, 0);
	ptr = 0;
    }
}

/* parseDataITOA
**
** Purpose:
**	Gather data PDV fragments in ini->acc direction. When all
**	fragments have been gathered in buffer or file, create DICOM object
**	and dump elements. File called "for_dcm_snoopITOA" used if not
**	enough buffer space available. File created in the directory from
**	where the application is executed.
**
** Parameter Dictionary:
**	buf	 	Pointer to data of PDV
**	pdvLength	Length of PDV fragment
**
** Return Values:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static void
parseDataITOA(u_char * buf, u_long pdvLength)
{

    static u_char stream[16384];
    static u_long ptr = 0;
    DCM_OBJECT *object;
    CONDITION cond;
    static int fd = 0;
    static DUL_PRESENTATIONCONTEXTID ctxID = 0;


    if (!ptr) {
	ctxID = buf[4];
    } else {
	if (ctxID != buf[4]) {
	    fprintf(stderr, "Unexpected presentation context ID\n");
	    exit(1);
	}
    }
    if (ptr + pdvLength - 2 > sizeof(stream)) {
	if (fd == 0) {
	    printf("Opening file for_dcm_snoopITOA\n");
	    fd = open("for_dcm_snoopITOA", O_CREAT | O_WRONLY | O_TRUNC, 0666);
	    if (fd < 0) {
		fprintf(stderr, "Error: Opening file for_dcm_snoopITOA\n");
		exit(1);
	    }
	    if (ptr) {
		if (write(fd, stream, ptr) != ptr) {
		    fprintf(stderr,
			    "Error: Writing to file for_dcm_snoopITOA\n");
		    exit(1);
		}
	    }
	}
	if (write(fd, &buf[6], pdvLength - 2) != pdvLength - 2) {
	    fprintf(stderr, "Error: Writing to file for_dcm_snoop\n");
	    exit(1);
	}
    } else
	(void) memcpy(&stream[ptr], &buf[6], pdvLength - 2);
    ptr = ptr + pdvLength - 2;
    if (buf[5] & 0x02) {
	/*
	 * Last fragment Dump elements
	 */
	if (fd) {
	    (void) close(fd);
	    cond = DCM_OpenFile("for_dcm_snoopITOA", DCM_ORDERLITTLEENDIAN |
			 DCM_FORMATCONVERSION | DCM_DELETEONCLOSE, &object);
	} else {
	    cond = DCM_ImportStream(stream, ptr,
				    DCM_ORDERLITTLEENDIAN, &object);
	    if (cond != DCM_NORMAL) {
		COND_DumpConditions();
		fprintf(stderr, "DCM_ImportStream failed\n");
		exit(1);
	    }
	}
	printf("\nData set ini->acc :\n");
	(void) DCM_DumpElements(&object, 0);
	ptr = 0;
	fd = 0;
    }
}


/* parseDataATOI
**
** Purpose:
**	Gather data PDV fragments in acc->ini direction. When all
**	fragments have been gathered in buffer or file, create DICOM object
**	and dump elements. File called "for_dcm_snoopATOI" used if not
**	enough buffer space available. File created in the directory from
**	from where the application is executed.
**
** Parameter Dictionary:
**	buf	 	Pointer to data of PDV
**	pdvLength	Length of PDV fragment
**
** Return Values:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static void
parseDataATOI(u_char * buf, u_long pdvLength)
{

    static u_char stream[16384];
    static u_long ptr = 0;
    DCM_OBJECT *object;
    CONDITION cond;
    static int fd = 0;
    static DUL_PRESENTATIONCONTEXTID ctxID = 0;


    if (!ptr) {
	ctxID = buf[4];
    } else {
	if (ctxID != buf[4]) {
	    fprintf(stderr, "Unexpected presentation context ID\n");
	    exit(1);
	}
    }
    if (ptr + pdvLength - 2 > sizeof(stream)) {
	if (fd == 0) {
	    printf("Opening file for_dcm_snoop\n");
	    fd = open("for_dcm_snoopATOI", O_CREAT | O_WRONLY | O_TRUNC, 0666);
	    if (fd < 0) {
		fprintf(stderr, "Error: Opening file for_dcm_snoopATOI\n");
		exit(1);
	    }
	    if (ptr) {
		if (write(fd, stream, ptr) != ptr) {
		    fprintf(stderr,
			    "Error: Writing to file for_dcm_snoopATOI\n");
		    exit(1);
		}
	    }
	}
	if (write(fd, &buf[6], pdvLength - 2) != pdvLength - 2) {
	    fprintf(stderr, "Error: Writing to file for_dcm_snoopATOI\n");
	    exit(1);
	}
    } else
	(void) memcpy(&stream[ptr], &buf[6], pdvLength - 2);
    ptr = ptr + pdvLength - 2;
    if (buf[5] & 0x02) {
	/*
	 * Last fragment Dump elements
	 */
	if (fd) {
	    (void) close(fd);
	    cond = DCM_OpenFile("for_dcm_snoopATOI", DCM_ORDERLITTLEENDIAN |
			 DCM_FORMATCONVERSION | DCM_DELETEONCLOSE, &object);
	} else {
	    cond = DCM_ImportStream(stream, ptr,
				    DCM_ORDERLITTLEENDIAN, &object);
	    if (cond != DCM_NORMAL) {
		COND_DumpConditions();
		fprintf(stderr, "DCM_ImportStream failed\n");
		exit(1);
	    }
	}
	printf("\nData set ini->acc :\n");
	(void) DCM_DumpElements(&object, 0);
	ptr = 0;
	fd = 0;
    }
}


/* parseAbort
**
** Purpose:
**	Dump information conatined in an ABORT PDU
**
** Parameter Dictionary:
**	buf	 	Pointer to data of PDU
**	pdvLength	Length of PDU
**
** Return Values:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static void
parseAbort(u_char * buf, int len)
{

    if (len != 10)
	printf("Incorrect size of A-ABORT PDU\n");
    switch (buf[8]) {
    case 0:
	printf("DICOM UL service-user intiated abort\n");
	break;
    case 2:
	printf("DICOM UL service-provider intiated abort\n");
	break;
    default:
	printf("Unknown source\n");
	break;
    }
    printf("Reason / Diag. = %u\n", buf[9]);
}
