/*
**                     Electronic Radiology Laboratory
**                   Mallinckrodt Institute of Radiology
**                Washington University School of Medicine
**
** Module Name(s):        format
**                        formatieList
**                        formatmodList
**                        formatattrList
**
** Author, Date:          Stephen M. Moore
**
** Intent:                This function converts binary string to ASCII
**
** Last Updates:          Chander L. Sabharwal, 27-June-94
**
** Source File:           $RCSfile: format.c,v $
**
** Revision:              $Revision: 1.4 $
**
** Status:                $State: Exp $
**
*/

static char rcsid[] = "$Revision: 1.4 $ $RCSfile: format.c,v $";

#include <stdio.h>
#ifdef GCCSUNOS
#include <stddef.h>
#endif

#include "dicom.h"
#include "condition.h"
#include "condition.h"
#include "tbl.h"
#include "lst.h"

#include "dicom.h"
#include "dicom_uids.h"
#include "dicom_objects.h"
#include "dicom_ie.h"


void copyWtext(char *t);
CTNBOOLEAN
errorstackP(unsigned long v, char *mes);

CTNBOOLEAN
clearStack;

/* Return value from DUL and ACR routines */
CONDITION cond;
char *info;


/* formatieList
** Purpose:
**      This function converts binary string of information entity(IE) list
**      to character string.
**
** Parameter Dictionary:
**      index           increments information entity list
**      buff            character array to store ASCII string
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
formatieList(IE_INFORMATIONENTITY * ieIE, int index, char *buff)
{
    (void) sprintf(buff, "%-32s", ieIE->ieDescription);
}
/* formatmodList
** Purpose:
**      This function converts binary string of module list
**      to character string.
**
** Parameter Dictionary:
**      index           increments module list
**      buff            character array to store ASCII string
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
formatmodieList(IE_MODULE * ieModule, int index, char *buff)
{
    (void) sprintf(buff, "%-48s", ieModule->moduleDescription);
}
/* formatattrList
** Purpose:
**      This function converts binary string of attribute list
**      to character string.
**
** Parameter Dictionary:
**      index           increments attribute list
**      buff            character array to store ASCII string
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
formatattrieList(IE_ATTRIBUTE * ieAttr, int index, char *buff)
{

    cond = DCM_LookupElement(&ieAttr->element);
    if (cond != DCM_NORMAL) {
/**	COND_DumpConditions();
	printf(" DCM_LookupElement not worked\n");**/
	(void) COND_ExtractConditions(errorstackP);
	copyWtext(info);
	(void) COND_PopCondition(clearStack);
	free(info);
    }
    (void) sprintf(buff, "%-64s", ieAttr->element.description);
}
