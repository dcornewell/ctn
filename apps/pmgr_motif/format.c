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
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	formatGroupTitle
**			formatStudyList
** Author, Date:	Chander Sabharwal, Summer, 1994
** Intent:
** Last Update:		$Author: smm $, $Date: 1995-03-28 23:02:55 $
** Source File:		$RCSfile: format.c,v $
** Revision:		$Revision: 1.4 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.4 $ $RCSfile: format.c,v $";

#include <stdio.h>
#ifdef GCCSUNOS
#include <stddef.h>
#endif

#include "dicom.h"
#include "condition.h"
#include "tbl.h"
#include "lst.h"
#include "manage.h"

#include "display.h"
#include "format.h"


/* formatGroupTitle
** Purpose:
**      This function converts binary string of groupname list
**      to character string.
**
** Parameter Dictionary:
**      index          increments groupname list
**      buf            character array to store ASCII string
**
** Return Values:
**      none
**
** Notes:
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/

void
formatGroupTitle(DMAN_GROUPNAMES * g, int index, char *buffer)
{
    strcpy(buffer, g->Title);
}

/* formatstudylist
** Purpose:
**      This function converts binary string of sopList list
**      to character string.
**
** Parameter Dictionary:
**      index          increments sopList list
**      buf            character array to store ASCII string
**
** Return Values:
**      none
**
** Notes:
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/

void
formatstudylist(LIST_ITEM * e, int index, char *buf)
{

    (void) sprintf(buf, "%-64s", e->combo);

}
