/*
          Copyright (C) 1993, RSNA and Washington University

          The software and supporting documentation for the Radiological
          Society of North America (RSNA) 1993 Digital Imaging and
          Communications in Medicine (DICOM) Demonstration were developed
          at the
                  Electronic Radiology Laboratory
                  Mallinckrodt Institute of Radiology
                  Washington University School of Medicine
                  510 S. Kingshighway Blvd.
                  St. Louis, MO 63110
          as part of the 1993 DICOM Central Test Node project for, and
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
/*
+-+-+-+-+-+-+-+-+-
*/
/*
**				DICOM 93
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	requestPrintAssociation
** Author, Date:	Stephen Moore, 1-Sep-94
** Intent:		Functions which establish and release associations
**			with DICOM film printers.
** Last Update:		$Author: smm $, $Date: 2002-10-31 23:36:05 $
** Source File:		$Source: /home/smm/ctn/ctn/apps/pmgr_motif/association.c,v $
** Revision:		$Revision: 1.4 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.4 $ $RCSfile: association.c,v $";

#include "ctn_os.h"

#include "dicom.h"
#include "dicom_uids.h"
#include "lst.h"
#include "condition.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "tbl.h"
#include "manage.h"


static CONDITION
requestAssociation(DUL_NETWORKKEY ** network,
		   DUL_ASSOCIATIONKEY ** association,
		   DUL_ASSOCIATESERVICEPARAMETERS * params,
		   char *SOPClass);



/* requestPrintAssociation
**
** Purpose:
**	requestPrintAssociation is used to establish an association
**	with an application that is presumed to be an SCP of the basic
**	print SOP classes.  The caller provides application titles
**	(calling and called) and a database interface (DMAN_HANDLE) that
**	allows this function to resolve the called application to
**	a host name and port number.  This function requests the
**	appropriate print meta SOP class with the single transfer syntax
**	that is supported.
**
** Parameter Dictionary:
**	callingApplication	The DICOM application entity title of this
**				application.  Used to identify one's self to
**				the server application.
**	calledApplication	The DICOM application entity title of the
**				print server application.
**	h			A handle to the management facility/database
**				that has previously been opened.  Used to
**				lookup the called AE title.
**	network			A network key used by the DUL facility.  If
**				the network has not already been opened, this
**				function will open it and place the key in
**				this variable.
**	association		The association handle returned by this
**				function.
**	params			The parameters used to describe this
**				association.
** Return Values:
**	0	failure
**	1	success
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
requestPrintAssociation(char *callingApplication,
			char *calledApplication, DMAN_HANDLE ** h,
	       DUL_NETWORKKEY ** network, DUL_ASSOCIATIONKEY ** association,
			DUL_ASSOCIATESERVICEPARAMETERS * params)
{
    CONDITION
	cond;
    DMAN_APPLICATIONENTITY
	ae;


    if (*network == NULL) {
	cond = DUL_InitializeNetwork(DUL_NETWORK_TCP, DUL_AEREQUESTOR,
			    NULL, DUL_TIMEOUT, DUL_ORDERBIGENDIAN, network);
	if (cond != DUL_NORMAL) {
	    return 0;
	}
    }
    cond = DMAN_LookupApplication(h, calledApplication, &ae);
    if (cond != DMAN_NORMAL) {
	return 0;

    }
    DUL_DefaultServiceParameters(params);

    (void) gethostname(params->callingPresentationAddress,
		       sizeof(params->callingPresentationAddress));
    sprintf(params->calledPresentationAddress, "%s:%-d", ae.Node,
	    ae.Port);
    strcpy(params->calledAPTitle, ae.Title);
    strcpy(params->callingAPTitle, callingApplication);

    if (requestAssociation(network, association, params,
			   DICOM_SOPCLASSGREYSCALEPRINTMGMTMETA) == 0) {
	return 0;
    }
    return 1;
}

/* requestAssociation
**
** Purpose:
**      This subroutine implements association request
**
** Parameter Dictionary:
**      network         the handle key of the network
**      association     the association key of the communication
**      params          the parameters for the association request
**      SOPClass        the SOP Class for specifying the association reqeust
**
** Return Values:
**      0               association request failed
**      1               association request successed
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/

static CONDITION
requestAssociation(DUL_NETWORKKEY ** network,
		   DUL_ASSOCIATIONKEY ** association,
		   DUL_ASSOCIATESERVICEPARAMETERS * params,
		   char *SOPClass)
{
    CONDITION
    cond;


    cond = SRV_RequestServiceClass(SOPClass, DUL_SC_ROLE_SCU, params);
    if (cond != SRV_NORMAL) {
	return 0;
    }
    cond = DUL_RequestAssociation(network, params, association);
    if (cond != DUL_NORMAL) {
	if (cond == DUL_ASSOCIATIONREJECTED) {
	    fprintf(stderr, "Association Rejected\n");
	    fprintf(stderr, " Result: %2x Source %2x Reason %2x\n",
		    params->result, params->resultSource,
		    params->diagnostic);
	}
	return 0;
    }
    (void) printf("Association accepted, parameters:-------------\n");
    DUL_DumpParams(params);
    return 1;
}
