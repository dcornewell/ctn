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
** Module Name(s):	DUL_Message
** Author, Date:	Stephen M. Moore, 14-Apr-1993
** Intent:		Define the ASCIZ messages that go with condition codes
**			and provide a function that returns a pointer to the
**			messages.
** Last Update:		$Author: smm $, $Date: 2001-12-21 15:18:44 $
** Source File:		$RCSfile: dulcond.c,v $
** Revision:		$Revision: 1.17 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.17 $ $RCSfile: dulcond.c,v $";

#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <time.h>
#endif

#include "dicom.h"
#include "lst.h"
#include "dulprotocol.h"
#include "dulstructures.h"

typedef struct vector {
    CONDITION cond;
    char *message;
}   VECTOR;

static VECTOR messageVector[] = {
    {DUL_NORMAL, "Normal return from DUL routine"},
    {DUL_NETWORKINITIALIZED, "Network already initialized"},
    {DUL_KEYCREATEFAILURE, "Failed to create key"},
    {DUL_UNSUPPORTEDNETWORK, "Unsupported network type: %s"},
    {DUL_UNRECOGNIZEDAE, "Unrecognized Application Entity: %s"},
    {DUL_TCPINITERROR, "TCP Initialization Error: %s"},
    {DUL_NULLKEY, "NULL key passed to routine: %s"},
    {DUL_ILLEGALKEY, "Illegal key passed to routine: %s"},
    {DUL_ILLEGALACCEPT, "Attempt to accept by requestor"},
    {DUL_ILLEGALREQUEST, "Attempt to request by acceptor"},
    {DUL_UNEXPECTEDPDU, "Received unexpected PDU"},
    {DUL_UNKNOWNREMOTENODE, "Connection attempt from unknown node: %s"},
    {DUL_UNKNOWNHOST, "Attempt to connect to unknown host: %s"},
    {DUL_MALLOCERROR,
    "Routine %s failed to malloc %d bytes"},
    {DUL_TCPIOERROR, "TCP I/O Error (%s) occurred in routine: %s"},
    {DUL_NOASSOCIATIONREQUEST, "No requests for associations for this server"},
    {DUL_UNRECOGNIZEDPDUTYPE, "Unrecognized PDU type: %2x"},
    {DUL_PEERABORTEDASSOCIATION,
    "Peer aborted Association (or never connected)"},
    {DUL_PEERDROPPEDASSOCIATION, "Peer dropped Association"},
    {DUL_WRONGASSOCIATIONSTATE, "Wrong Association state for operation"},
    {DUL_CODINGERROR, "Coding Error in DUL routine: %s"},
    {DUL_ILLEGALSERVICEPARAMETER, "Illegal service parameter: %s"},
    {DUL_REQUESTASSOCIATIONFAILED, "Failed to establish association"},
    {DUL_APABORT, "DUL A-P-ABORT"},
    {DUL_ASSOCIATIONREJECTED, "DUL Association Rejected"},
    {DUL_ILLEGALREJECTREASON,
    "DUL Illegal reason for rejecting Association: %x"},
    {DUL_ILLEGALREJECTRESULT,
    "DUL Illegal result for rejecting Association: %x"},
    {DUL_RELEASECONFIRMED, "DUL Release Confirmed"},
    {DUL_PDATAPDUARRIVED, "DUL P-Data PDU arrived"},
    {DUL_READTIMEOUT, "DUL network read timeout"},
    {DUL_NETWORKCLOSED, "DUL network closed"},
    {DUL_ILLEGALPDU, "DUL Illegal or ill-formed pdu of type: %02x"},
    {DUL_FSMERROR,
    "DUL Finite State Machine Error, State: %d Event %d"},
    {DUL_PEERREQUESTEDRELEASE, "DUL Peer Requested Release"},
    {DUL_WRONGDATATYPE, "DUL Wrong Data Type Specified for Request"},
    {DUL_INSUFFICIENTBUFFERLENGTH,
    "DUL Insufficient buffer space allocated for data"},
    {DUL_INCORRECTBUFFERLENGTH,
    "DUL Incorrect buffer space allocated for data"},
    {DUL_ASSOCIATIONPARAMETERFAILED, "DUL Failed to retrieve parameter: %s"},
    {DUL_NOPDVS, "DUL No PDVs available in current buffer"},
    {DUL_ILLEGALPARAMETER, "DUL Illegal parameter (%s) in function %s"},
    {DUL_LISTERROR, "DUL List error occurred in function %s"},
    {DUL_ILLEGALPDULENGTH, "DUL Illegal PDU Length %d.  Max expected %d in %s"},
    {DUL_LISTCREATEFAILED, "DUL Failed to create a new list in %s"},
    {DUL_UNSUPPORTEDPEERPROTOCOL, "DUL Unsupported peer protocol %04x; expected %04x in %s"},
    {DUL_PEERILLEGALXFERSYNTAXCOUNT,
    "DUL Peer supplied illegal number of transfer syntaxes (%d)"},
    {DUL_PCTRANSLATIONFAILURE,
    "DUL Presentation Context translation failure in %s"},
    {DUL_SNPFILEOPEN, "DUL Error opening file %s in %s"},
    {DUL_SNPCALLBACKUSE, "DUL Error using callback %s in %s"},
    {DUL_SNPCALLBACKREG, "DUL Error registering callback %s in %s"},
    {DUL_SNPINIT, "DUL SNP_Init failure in %s"},
    {DUL_SNPPREMATUREEOF, "DUL Expecting more data from files in function %s"},
    {DUL_SNPSTART, "DUL Failure of SNP_Start function in %s"},
    {DUL_SNPSTOP, "DUL Failure of SNP_Stop function in %s"},
    {DUL_SNPTERMINATE, "DUL Failure of SNP_Terminate function in %s"},
    {DUL_SNPNOTALLASSOC,
    "DUL SNP Error %s with %d associations to snoop on in %s"},
    {DUL_SNPBADSTATE,
    "DUL SNP facility in bad state %s in function %s"},
    {DUL_SNPBADASSOCSTATE,
    "DUL snooper transition into bad DUL association state"},
    {DUL_SNPUNIMPLEMENTED,
    "DUL snoop function %s unimplemented"},
    {DUL_PERROR, "DUL General perror (%s) in %s"},
    {0, NULL}
};


/* DUL_Message
**
** Purpose:
**	This function accepts a CONDITION as an input parameter and finds
**	the ASCIZ message that is defined for that CONDITION.  If the
**	CONDITION is defined for this facility, this function returns
**	a pointer to the ASCIZ string which describes the condition.
**	If the CONDITION is not found, the function returns NULL.
**
** Parameter Dictionary:
**	condition	The CONDITION used to search the dictionary.
**
** Return Values:
**	ASCIZ string which describes the condtion requested by the caller
**	NULL if the condition is not found
**
*/
char *
DUL_Message(CONDITION condition)
{
    int
        index;

    for (index = 0; messageVector[index].message != NULL; index++)
	if (condition == messageVector[index].cond)
	    return messageVector[index].message;

    return NULL;
}
