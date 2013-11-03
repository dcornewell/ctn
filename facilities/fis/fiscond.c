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
** Module Name(s):	FIS_Message
** Author, Date:	Stephen M. Moore, 12-Jul-94
** Intent:		Define the ASCIZ messages that go with each FIS
**			error number and provide a function for looking up
**			the error message.
** Last Update:		$Author: smm $, $Date: 1998-03-13 19:43:07 $
** Source File:		$RCSfile: fiscond.c,v $
** Revision:		$Revision: 1.9 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.9 $ $RCSfile: fiscond.c,v $";

#include <stdio.h>
#include <sys/types.h>
#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "dicom_objects.h"
#include "tbl.h"
#include "manage.h"
#include "fis.h"

typedef struct vector {
    CONDITION cond;
    char *message;
}   VECTOR;

static VECTOR messageVector[] = {
    {FIS_NORMAL, "FIS Normal return"},
    {FIS_ERROR, "FIS General error returned by %s"},
    {FIS_OPENFAILED, "FIS Failed to open table %s in database %s in %s"},
    {FIS_MALLOCFAILURE, "FIS Function failed to malloc %d bytes in %s"},
    {FIS_CLOSEFAILED, "FIS Failed to close database in %s"},
    {FIS_ILLEGALRECORDTYPE, "FIS Illegal record (expected %s) in %s"},
    {FIS_NEWRECORDFAILED, "FIS Failed to create a new FIS record in %s"},
    {FIS_INSERTFAILED, "FIS Failed to insert a new record (%s) in %s"},
    {FIS_PATIENTINSERTFAILED, "FIS Failed to insert patient record in %s"},
    {FIS_GETFAILED, "FIS Failed to get a new record in %s"},
    {FIS_PATIENTGETFAILED, "FIS Failed to get patient record in %s"},
    {FIS_DELETEFAILED, "FIS Failed to delete a record in %s"},
    {FIS_PATIENTDELETEFAILED, "FIS Failed to delete patient record in %s"},
    {FIS_NULLSTRING, "FIS Unexpected null string (%s) detected in %s"},
    {FIS_UPDATEFAILED, "FIS Failed to update a record in %s"},
    {FIS_PATIENTUPDATEFAILED, "FIS Failed to update patient record in %s"},
    {FIS_STUDYGETFAILED, "FIS Failed to get study record in %s"},
    {FIS_STUDYDELETEFAILED, "FIS Failed to delete study record in %s"},
    {FIS_STUDYUPDATEFAILED, "FIS Failed to update study record in %s"},
    {FIS_ACCESSIONNUMBERFAILED, "FIS Failed to create a new Accession Number in %s"},
    {FIS_MISSINGMANDATORYELEMENT,
    "FIS Record is missing mandatory element (%s) in %s"},
    {FIS_RESULTSUPDATEFAILED, "FIS Failed to update results record in %s"},
    {FIS_INTERPRETATIONUPDATEFAILED,
    "FIS Failed to update interpretation record in %s"},
    {FIS_IDFAILED, "FIS Failed to create a new %s ID in %s"},
    {FIS_UIDFAILED, "FIS Failed to create a new %s UID in %s"},
    {FIS_ILLEGALDELETECRITERIA,
    "FIS Illegal criteria specified for deleting record of type %s in %s"},
    {FIS_SINGLEGETFAILED,
    "FIS Failed to retrieve single record with uid %s as requested in %s"},
    {FIS_BUILDFAILED, "FIS Failed to build record of type %s in %s"},
    {FIS_SENDEVENTFAILED, "FIS Failed to send event notification to %s in %s"},
    {FIS_SENDSETFAILED, "FIS Failed to send set request to %s in %s"},
    {FIS_PARSEFAILED, "FIS Failed to parse object of type %s in %s"},
    {FIS_STORAGECOMMITREQUPDATEFAILED, "FIS Failed to update storage commit req record in %s"},
    {FIS_WORKQUEUERECORDDELETEFAILED, "FIS Failed to delete work queue record in %s"},
    {0, NULL}
};


/* FIS_Message
**
** Purpose:
**	Find the ASCIZ message that goes with an FIS error number and
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
FIS_Message(CONDITION condition)
{
    int
        index;

    for (index = 0; messageVector[index].message != NULL; index++)
	if (condition == messageVector[index].cond)
	    return messageVector[index].message;

    return NULL;
}
