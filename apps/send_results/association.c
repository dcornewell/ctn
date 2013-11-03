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
**				DINPACS 97
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	Stephen Moore, 13-Feb-97
** Intent:
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:45:57 $
** Source File:		$RCSfile: association.c,v $
** Revision:		$Revision: 1.2 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.2 $ $RCSfile: association.c,v $";

#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#ifdef MACH
#include <unistd.h>
#endif
#endif

#include "dicom.h"
#include "condition.h"
#include "tbl.h"
#include "lst.h"
#include "dicom_uids.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_ie.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "tbl.h"
#include "manage.h"
#include "idb.h"
#include "iap.h"

#include "send_results.h"

extern CTNBOOLEAN silent;

typedef struct {
    char *levelChar;
    int levelInt;
}   QUERY_MAP;

typedef struct {
    void *reserved[2];
    IDB_Query query;
}   QUERY_LIST_ITEM;


/* sendCallback
**
** Purpose:
**	Callback routine for the C-SEND Response primitive
**
** Parameter Dictionary:
**	request		Pointer to request message
**	response	Pointer to response message
**	transmitted	Number of bytes sent
**	total		Total number of bytes to be sent
**	string		Context Information
**
** Return Values:
**	SRV_NORMAL
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
sendCallback(MSG_C_STORE_REQ * request, MSG_C_STORE_RESP * response,
	     unsigned long transmitted, unsigned long total,
	     char *string)
{
    if (transmitted == 0) {
	if (!silent)
	    printf("Initial call to sendCallback\n");
    }
    if (!silent)
	printf("%8d bytes transmitted of %8d (%s)\n", transmitted, total, string);

    if (response != NULL) {
	if (!silent)
	    MSG_DumpMessage(response, stdout);
    }
    return SRV_NORMAL;
}

/* establishSendAssociation
**
** Purpose:
**	Request for the specific service class and then establish an
**	Association
**
** Parameter Dictionary:
**	networkKey		Key describing the network connection
**	queryList		Handle to list of queries
**	moveDestination		Name of destination where images are to be moved
**	sendAssociation		Key describing the Association
**	params			Service parameters describing the Association
**
** Return Values:
**	DUL_NORMAL	on success
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
establishSendAssociation(DUL_NETWORKKEY ** networkKey,
			 LST_HEAD * queryList, char *moveDestination,
			 DMAN_HANDLE ** handle,
			 DUL_ASSOCIATIONKEY ** sendAssociation,
			 DUL_ASSOCIATESERVICEPARAMETERS * params)
{
    QUERY_LIST_ITEM
	* q;
    CONDITION
	cond;
    DMAN_APPLICATIONENTITY
	ae,
	criteria;
    long
        count;

    if (queryList == NULL)
	return 0;
    memset(&criteria, 0, sizeof(criteria));
    criteria.Type = ae.Type = DMAN_K_APPLICATIONENTITY;
    criteria.Flag = DMAN_K_APPLICATION_TITLE;
    strcpy(criteria.Title, moveDestination);
    cond = DMAN_Select(handle, (DMAN_GENERICRECORD *) & ae,
	       (DMAN_GENERICRECORD *) & criteria, NULL, NULL, &count, NULL);
    if (cond != DMAN_NORMAL)
	return 0;
    if (count != 1)
	return 0;

    sprintf(params->calledPresentationAddress, "%s:%-d", ae.Node, ae.Port);

    q = LST_Head(&queryList);
    if (q == NULL)
	return 0;
    (void) LST_Position(&queryList, q);
    while (q != NULL) {
	cond = SRV_RequestServiceClass(q->query.image.SOPClaUID,
				       DUL_SC_ROLE_DEFAULT, params);
	if (CTN_INFORM(cond))
	    (void) COND_PopCondition(FALSE);
	else if (cond != SRV_NORMAL)
	    return 0;		/* repair */
	q = LST_Next(&queryList);
    }
    cond = DUL_RequestAssociation(networkKey, params, sendAssociation);
    if (cond != DUL_NORMAL) {
	printf("Could not establish Association with %s %s %s\n",
	       params->callingAPTitle,
	       params->calledAPTitle,
	       params->calledPresentationAddress);
	COND_DumpConditions();
	return 0;		/* repair */
    }
    return APP_NORMAL;
}
