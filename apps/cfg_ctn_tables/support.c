/*
+-+-+-+-+-+-+-+-+-
*/
/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	extractGroups
**			extractTitles
** Author, Date:	Steve Moore, 29-Aug-94
** Intent:		Provide support functions for configuration application.
**			Functions are varied.
** Last Update:		$Author: smm $, $Date: 1994-09-05 15:42:49 $
** Source File:		$RCSfile: support.c,v $
** Revision:		$Revision: 1.1 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.1 $ $RCSfile: support.c,v $";

#include <stdio.h>
#include <stdlib.h>

#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "tbl.h"
#include "manage.h"

#include "support.h"

/* function name
**
** Purpose:
**	Describe the purpose of the function
**
** Parameter Dictionary:
**	Define the parameters to the function
**
** Return Values:
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
extractGroups(LST_HEAD * src, LST_HEAD * dst)
{
    DMAN_GROUPNAMES
	* s,
	*p;

    s = LST_Head(&src);
    if (s != NULL)
	(void) LST_Position(&src, s);

    while (s != NULL) {
	p = LST_Head(&dst);
	if (p != NULL)
	    (void) LST_Position(&dst, p);
	while (p != NULL) {
	    if (strcmp(s->GroupName, p->GroupName) == 0)
		break;
	    else
		p = LST_Next(&dst);
	}
	if (p == NULL) {
	    p = malloc(sizeof(*p));
	    if (p == NULL)
		return 0;
	    *p = *s;
	    (void) LST_Enqueue(&dst, p);
	}
	s = LST_Next(&src);
    }
    return 1;
}

/* function name
**
** Purpose:
**	Describe the purpose of the function
**
** Parameter Dictionary:
**	Define the parameters to the function
**
** Return Values:
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
extractTitles(LST_HEAD * src, char *group, LST_HEAD * dst)
{
    DMAN_GROUPNAMES
	* s,
	*p;

    s = LST_Head(&src);
    if (s != NULL)
	(void) LST_Position(&src, s);

    while (s != NULL) {
	if (strcmp(s->GroupName, group) == 0) {
	    p = malloc(sizeof(*p));
	    if (p == NULL)
		return 0;
	    *p = *s;
	    (void) LST_Enqueue(&dst, p);
	}
	s = LST_Next(&src);
    }
    return 1;
}
