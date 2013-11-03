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
** Module Name(s):	APP_Message
** Author, Date:	Aniruddha S. Gokhale, 23-June-1994
** Intent:		Define the ASCII messages that go with condition codes
**			and provide a function that returns a pointer to the
**			messages.
** Last Update:		$Author: smm $
** Source File:		$RCSfile: psscond.c,v $
** Revision:		$Revision: 1.1 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.1 $ $RCSfile: psscond.c,v $";
#include "print_server.h"

typedef struct vector {
    CONDITION cond;
    char *message;
}   VECTOR;

static VECTOR messageVector[] = {
    {APP_NORMAL, "Normal return from application routine"},
    {APP_FAILURE, "APP Failure processing (%s) in (%s)"},
    {APP_ILLEGALSERVICEPARAMETER, "APP Illegal service parameter (name %s, value %s)"},
    {APP_MISSINGSERVICEPARAMETER, "APP The %s parameter is missing"},
    {APP_PARAMETERWARNINGS, "APP One or more service parameters were incorrect"},
    {APP_PARAMETERFAILURE, "APP One or more service parameters were incorrect"},
    {APP_ASSOCIATIONRQFAILED, "APP Unsuccessful association request"},
    {APP_LISTFAILURE, "APP Unsuccessful LST facility operation (%s) in (%s)"},
    {APP_MALLOCFAILURE, "APP Malloc failure in (%s)"},
    {APP_GQOPERATIONFAILED, "APP Generalized Queue operation (%s) failed in (%s)"},
    {APP_OPERATIONINTERRUPTED, "APP Application Interrupted"},
    {APP_MISSINGREQUIREDATTRIBUTE, "APP Missing Required Attribute(s) \n\t(%s) in (%s)"},
    {APP_UNSUPPORTEDMANDATORYATTRIBUTE, "APP Unsupported Mandatory Attribute(s) \n\t(%s) in (%s)"},
    {APP_UNSUPPORTEDOPTIONALATTRIBUTE, "APP Unsupported Optional Attribute(s) \n\t(%s) in (%s)"},
    {APP_UNSUPPORTEDATTRIBUTEVALUE, "APP Unsupported Attribute Value(s)\n\t(%s) in (%s)"},
    {APP_MISSINGIMAGES, "APP No Images in (%s)"},
    {APP_NOSUCHINSTANCE, "APP No such instance UID (%s) in (%s)"},
    {APP_MISSINGFILMBOX, "APP No Film Box OR No such Film Box  SOP instance in (%s)"},
};


/* APP_Message
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
APP_Message(CONDITION condition)
{
    int
        index;

    for (index = 0; index < (int) DIM_OF(messageVector); index++)
	if (condition == messageVector[index].cond)
	    return messageVector[index].message;

    return NULL;
}
