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
**				DICOM 94
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	IRS_Message
** Author, Date:	Stephen M. Moore, 12-Jul-94
** Intent:		Define the ASCIZ messages that go with each IRS
**			error number and provide a function for looking up
**			the error message.
** Last Update:		$Author: smm $, $Date: 1998-10-22 17:47:16 $
** Source File:		$RCSfile: irscond.c,v $
** Revision:		$Revision: 1.1 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.1 $ $RCSfile: irscond.c,v $";

#include <stdio.h>
#include <sys/types.h>
#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "dicom_objects.h"
#include "tbl.h"
#include "dicom_irs.h"

typedef struct vector {
    CONDITION cond;
    char *message;
}   VECTOR;

static VECTOR messageVector[] = {
    {IRS_NORMAL, "IRS Normal return"},
    {IRS_ERROR, "IRS General error returned by %s"},
    {IRS_OPENFAILED, "IRS Failed to open table %s in database %s in %s"},
    {IRS_MALLOCFAILURE, "IRS Function failed to malloc %d bytes in %s"},
    {IRS_CLOSEFAILED, "IRS Failed to close database in %s"},
    {IRS_ILLEGALRECORDTYPE, "IRS Illegal record (expected %s) in %s"},
    {IRS_NEWRECORDFAILED, "IRS Failed to create a new IRS record in %s"},
    {IRS_INSERTFAILED, "IRS Failed to insert a new record (%s) in %s"},
    {IRS_PATIENTINSERTFAILED, "IRS Failed to insert patient record in %s"},
    {IRS_SELECTFAILED, "IRS Failed to select a new record in %s"},
    {IRS_PATIENTGETFAILED, "IRS Failed to get patient record in %s"},
    {IRS_DELETEFAILED, "IRS Failed to delete a record in %s"},
    {IRS_PATIENTDELETEFAILED, "IRS Failed to delete patient record in %s"},
    {IRS_NULLSTRING, "IRS Unexpected null string (%s) detected in %s"},
    {IRS_UPDATEFAILED, "IRS Failed to update a record in %s"},
    {IRS_PATIENTUPDATEFAILED, "IRS Failed to update patient record in %s"},
    {IRS_STUDYGETFAILED, "IRS Failed to get study record in %s"},
    {IRS_STUDYDELETEFAILED, "IRS Failed to delete study record in %s"},
    {IRS_STUDYUPDATEFAILED, "IRS Failed to update study record in %s"},
    {IRS_ACCESSIONNUMBERFAILED, "IRS Failed to create a new Accession Number in %s"},
    {IRS_MISSINGMANDATORYELEMENT,
    "IRS Record is missing mandatory element (%s) in %s"},
    {IRS_RESULTSUPDATEFAILED, "IRS Failed to update results record in %s"},
    {IRS_INTERPRETATIONUPDATEFAILED,
    "IRS Failed to update interpretation record in %s"},
    {IRS_IDFAILED, "IRS Failed to create a new %s ID in %s"},
    {IRS_UIDFAILED, "IRS Failed to create a new %s UID in %s"},
    {IRS_ILLEGALDELETECRITERIA,
    "IRS Illegal criteria specified for deleting record of type %s in %s"},
    {IRS_SINGLEGETFAILED,
    "IRS Failed to retrieve single record with uid %s as requested in %s"},
    {IRS_BUILDFAILED, "IRS Failed to build record of type %s in %s"},
    {IRS_SENDEVENTFAILED, "IRS Failed to send event notification to %s in %s"},
    {IRS_SENDSETFAILED, "IRS Failed to send set request to %s in %s"},
    {IRS_PARSEFAILED, "IRS Failed to parse object of type %s in %s"},
    {IRS_STORAGECOMMITREQUPDATEFAILED, "IRS Failed to update storage commit req record in %s"},
    {IRS_WORKQUEUERECORDDELETEFAILED, "IRS Failed to delete work queue record in %s"},
    {0, NULL}
};


/* IRS_Message
**
** Purpose:
**	Find the ASCIZ message that goes with an IRS error number and
**	return a pointer to static memory containing that error message.
**
** Parameter Dictionary:
**	condition	The error condition for which the message is to be
**			returned
**
** Return Values:
**	The error message if a valid error condition was reported else NULL.
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

char *
IRS_Message(CONDITION condition)
{
    int
        index;

    for (index = 0; messageVector[index].message != NULL; index++)
	if (condition == messageVector[index].cond)
	    return messageVector[index].message;

    return NULL;
}
