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
** Module Name(s):	PRN_Message
** Author, Date:	Aniruddha S. Gokhale, September-1-1993.
** Intent:		This file defines the ASCII messages that go with
**			each PRN error number and provide a function for
**			looking up the error message.
** Last Update:		$Author: smm $, $Date: 1998-03-13 19:45:01 $
** Source File:		$RCSfile: printcond.c,v $
** Revision:		$Revision: 1.7 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.7 $ $RCSfile: printcond.c,v $";

#include <stdio.h>
#include <string.h>
#include <errno.h>
#ifndef MACOS
#include <stdlib.h>
#endif
#include <stdarg.h>
#include <sys/types.h>

#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "dicom_objects.h"
#include "dicom_sq.h"
#include "dicom_print.h"


typedef struct vector {
    CONDITION cond;
    char *message;
}   VECTOR;

static VECTOR messageVector[] = {
    {PRN_NORMAL, "PRN Normal return from PRN routine"},
    {PRN_BUILDFAILED, "PRN Build failed in routine %s"},
    {PRN_PARSEFAILED, "PRN Parse failed in routine %s"},
    {PRN_LISTCREATEFAILURE, "PRN Failed to create new list in %s"},
    {PRN_MALLOCFAILURE, "PRN Failed to allocate %d bytes in %s"},
    {PRN_ILLEGALUIDLENGTH, "PRN Illegal UID length (%d) in %s"},
    {PRN_LISTFAILURE, "PRN List failure in %s"},
    {0, NULL}
};


/* PRN_Message
**
** Purpose:
**      Find the ASCII message that goes with an PRN error number and
**      return a pointer to static memory containing that error message.
**
** Parameter Dictionary:
**	cond	The condition value that was returned as a result of
**		failure.
**
** Return Values:
**	The error message.
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
char *
PRN_Message(CONDITION condition)
{
    int
        index;

    for (index = 0; messageVector[index].message != NULL; index++)
	if (condition == messageVector[index].cond)
	    return messageVector[index].message;

    return NULL;		/* no message exists */
}
