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
** Module Name(s):	ICON_Message
**
** Author, Date:	Tze Khiong Lee, Aug 3rd 1993
** Intent:		This facility provides routines to build
**			a simple database of image icons.  This file contains
**			the function which tranlates condition values into
**			ASCII messages.
** Last Update:		$Author: smm $, $Date: 2000-01-19 22:07:20 $
** Source File:		$RCSfile: iconcond.c,v $
** Revision:		$Revision: 1.7 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.7 $ $RCSfile: iconcond.c,v $";
#include <stdio.h>
#include <sys/types.h>
#ifdef MACOS
#include <stat.h>
#else
#include <sys/stat.h>
#endif
#include <fcntl.h>
#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "icon.h"

typedef struct vector {
    CONDITION cond;
    char *message;
}   VECTOR;

static VECTOR messageVector[] = {
    {ICON_NORMAL, "Normal return from ICON routine"},
    {ICON_FILEOPENFAILED, "ICON Failed to open %s in %s"},
    {ICON_APPENDFAILED, "ICON Failed to append to %s in %s"},
    {ICON_LISTFAILURE, "ICON List Failure in %s"},
    {ICON_MALLOCFAILURE, "ICON Failed to malloc %d bytes in %s"},
    {ICON_INCORRECTFILESIZE, "ICON Failed to read Icon records from %s in %s"},
    {ICON_ACCESSIONNOTFOUND, "ICON Failed to find Accession Number %s in %s"},
    {0, NULL}
};

char *
ICON_Message(CONDITION condition)
{
    int
        index;

    for (index = 0; messageVector[index].message != NULL; index++)
	if (condition == messageVector[index].cond)
	    return messageVector[index].message;

    return NULL;
}
