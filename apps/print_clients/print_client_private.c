/*
+-+-+-+-+-+-+-+-+-
*/
/*
**				DICOM 93
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	sendGetPrinterInstance
**			sendCreateFilmSession
**			sendCreateFilmBox
**			sendSetImageBox
**
** Author, Date:	Aniruddha S. Gokhale, August 9th, 1993.
** Intent:		This module contains routines that are  private
**			to the print_client program. These private routines
**			typically hard code various structures and then
**			call the routines in the print_shared.c file.
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:48:00 $
** Source File:		$Source: /home/smm/ctn/ctn/apps/print_clients/print_client_private.c,v $
** Revision:		$Revision: 1.7 $
** Status:		$State: Exp $
*/
static char rcsid[] = "$Revision: 1.7 $ $RCSfile: print_client_private.c,v $";
#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif


#include "dicom.h"
#include "lst.h"
#include "condition.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "dicom_uids.h"
#include "dicom_sq.h"
#include "dicom_n_printer.h"

/* Extern Declarations */
extern char bfsInstanceUID[DICOM_UI_LENGTH + 1];
extern char bfbInstanceUID[DICOM_UI_LENGTH + 1];
extern BOOLEAN silent;


/* sendGetPrinterInstance
**
** Purpose:
**	Send the N-GET  request to the SCP to get the attributes of the
**	PRINTER SOP instance.
** Parameter Dictionary:
**	association	the key which describes the association
**	params		parameters describing the association
**	SOPClass	negotiated meta SOP class UID
** Return Values:
**	success or failure
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
BOOLEAN
sendGetPrinterInstance(DUL_ASSOCIATIONKEY ** association,
		       DUL_ASSOCIATESERVICEPARAMETERS * params,
		       char *SOPClass)
{
    CONDITION
	cond;			/* condition code returned by various
				 * facilities */
    void
       *ctx = NULL;
    DCM_TAG
	tags[] = {		/* We request the server to provide us with
				 * the following attributes */
	DCM_PRINTERSTATUS,
	    DCM_PRINTERSTATUSINFOCODE,
	    DCM_PRINTERNAME,
	    DCM_IDMANUFACTURER,
	    DCM_IDMANUFACTURERMODEL,
	    DCM_ACQDEVICESERIALNUM,
	    DCM_ACQSOFTWAREVERSION,
	    DCM_ACQDATELASTCALIBRATION,
	    DCM_ACQTIMELASTCALIBRATION
    };				/* we ask for all the attributes of the
				 * printer */


    /*
     * invoke the function which builds a request message and sends it to the
     * SCP
     */
#ifdef SMM
    cond = sendPrinterNGetRequest(association, params, SOPClass, tags,
				  DIM_OF(tags));
    if (cond != SRV_NORMAL)
	return FALSE;
#endif

    return TRUE;		/* success */
}

/* sendCreateFilmSession
**
** Purpose:
**	used to send the N-CREATE request message to create a BASIC
**	FILM SESSION SOP instance.
**
** Parameter Dictionary:
**	association	key which describes the association
**	params		parameters describing the association
**	SOPClass	meta SOP class UID
**
** Return Values:
**	success or failure
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
BOOLEAN
sendCreateFilmSession(DUL_ASSOCIATIONKEY ** association,
		      DUL_ASSOCIATESERVICEPARAMETERS * params,
		      BFS_ATTRIBUTES * bfsAttributes,
		      char *SOPClass)
{
    CONDITION
	cond;

#ifdef SMM
    |BFS_K_FILMSESSIONLABEL |
	BFS_K_MEMORYALLOCATION;	/* these are the fields provided by us */
#endif

    /* invoke the function that builds the request message for N-CREATE */
#ifdef SMM
    cond = sendBFSNCreateRequest(association, params, SOPClass, bfsAttributes);
    if (cond != SRV_NORMAL)
	return FALSE;
#endif
    return TRUE;		/* success */
}

/* sendCreateFilmBox
**
** Purpose:
**	Describe the purpose of the function
**
** Parameter Dictionary:
**	association	key which describes the association
**	params		parameters of the association
**	SOPClass	negotiated meta SOP class
**	format		image display format
**	e		address of the array of DCM_ELEMENT. This
**			is necessary to store the instance UID of the
**			referenced parent film session.
**
** Return Values:
**	success or failure
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
BOOLEAN
sendCreateFilmBox(DUL_ASSOCIATIONKEY ** association,
		  DUL_ASSOCIATESERVICEPARAMETERS * params,
		  char *SOPClass,
		  char *format,
		  DCM_ELEMENT e[])
{
    /*
     * following attributes are provided by the SCU.
     * 
     * The user can change these attributes and set new values for them
     */
    static BFB_ATTRIBUTES bfbAttrib = {
	0,			/* Attribute Flag */
	DICOM_SOPCLASSBASICFILMSESSION,
	"SOP Instance UID goes here",
	"STANDARD\\1,1",	/* image display format */
	"",			/* annotation display */
	PORTRAIT,		/* film orientation */
	SIZE8INX10IN,		/* film size */
	"REPLICATE",		/* magnification type */
	"",			/* no smoothing type provided */
	BLACKBORDER,		/* border density */
	"",			/* no empty imaage density */
	0,			/* no minimum density provided */
	0,			/* no max density provided */
	NO,			/* trim value */
	"",			/* no configuration info */
	NULL, NULL		/* List heads for basic image box */
	/* and annotation box sequences */
    };

    CONDITION
	cond;

    /*
     * It is important to set the referenced parent film session UID of the
     * current film box
     */
/*    (void) strcpy(bfbAttrib.referencedParentFilmSessionUID,
		  bfsInstanceUID);
*/
    bfbAttrib.bfbAttribFlag = BFB_IMAGEDISPLAYFORMAT | BFB_FILMORIENTATION |
	BFB_FILMSIZEID | BFB_BORDERDENSITY | BFB_TRIM;
/*	| BFB_REFERENCEDPARENTFILMSESSION; */
    if (strlen(format) != 0)	/* user supplied FORMAT as command line
				 * argument */
	(void) strcpy(bfbAttrib.imageDisplayFormat, format);

    /*
     * invoke the function which builds the request message and sends it to
     * the SCP
     */
#ifdef SMM
    cond = sendBFBNCreateRequest(association, params, SOPClass,
				 &bfbAttrib, e);
    if (cond != SRV_NORMAL)
	return FALSE;
#endif

    return TRUE;		/* success */
}
