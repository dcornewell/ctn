/*
+-+-+-+-+-+-+-+-+-
*/
/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	Stephen Moore, 1-Sep-94
** Intent:		Provide some of the main driver functions for
**			the motif-based print client.
** Last Update:		$Author: smm $, $Date: 2002-10-31 23:36:06 $
** Source File:		$RCSfile: driver.c,v $
** Revision:		$Revision: 1.6 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.6 $ $RCSfile: driver.c,v $";

#include "ctn_os.h"

#include "dicom.h"
#include "lst.h"
#include "condition.h"
#include "tbl.h"
#include "manage.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "dbquery.h"
#include "dicom_uids.h"
#include "dicom_sq.h"
#include "dicom_print.h"
/*#include "icon.h"*/
#include "print_shared.h"

DMAN_HANDLE *dmanHandle;
CTNBOOLEAN silent = FALSE;

char imagePositionC[64];

char *serverGroup = NULL;

PRN_PRINTER printAttrib = {
    NULL, NULL,
    PRN_K_BASICPRINTER
};

PRN_BASICFILMSESSION bfsAttrib = {
    NULL, NULL,
    PRN_K_BASICFILMSESSION,	/* This structure type */
    0,				/* Attribute Flag */
    DICOM_SOPCLASSBASICFILMSESSION,
    "",				/* Film session SOP Instance UID */
    "1",			/* 1 copies */
    HIGH,			/* high priority */
    PAPER,			/* medium is paper */
    MAGAZINE,			/* medium type */
    "",				/* session label (we're not doing this) * /
				 * "",                         /* kilo bytes
				 * requested  (not doing this) */
    NULL			/* no child film box UID known yet */
};


PRN_BASICFILMBOX bfbAttrib = {
    NULL, NULL,
    PRN_K_BASICFILMBOX,		/* type */
    0,				/* Attribute Flag */
    DICOM_SOPCLASSBASICFILMBOX,	/* SOP class UID */
    "",				/* SOP instance UID, unknown yet */
    DICOM_SOPCLASSBASICFILMSESSION,	/* SOP class UID of parent film
					 * session */
    "",				/* instance UID of parent unknown yet */
    "STANDARD\\2,2",		/* image display format */
    "",				/* annotation display */
    PORTRAIT,			/* film orientation */
    SIZE8INX10IN,		/* film size */
    "REPLICATE",		/* magnification type */
    "",				/* no smoothing type provided */
    BLACKBORDER,		/* border density */
    "",				/* no empty imaage density */
#if STANDARD_VERSION < VERSION_AUG1993
    "",				/* no minimum density provided */
    "",				/* no max density provided */
#else
    0,				/* no minimum density provided */
    0,				/* no max density provided */
#endif
    NO,				/* trim value */
    "",				/* no configuration info */
    NULL, NULL			/* List heads for basic image box */
    /* and annotation box sequences */
};

PRN_BASICIMAGEBOX bibAttrib = {
    NULL, NULL,			/* Reserved for LST */
    PRN_K_BASICGREYSCALEIMAGEBOX,	/* Type */
    0,				/* Flag for conditional fields */
    DICOM_SOPCLASSBASICGREYSCALEIMAGEBOX,
    "",				/* SOP Instance UID */
    0,				/* Image Position */
    "",				/* Polarity */
    "",				/* Magnification type */
    "",				/* Smoothing type */
#if STANDARD_VERSION < VERSION_AUG1993
    "",				/* magnification factor */
#else
    "",				/* Requested Image Size */
#endif
    NULL,			/* Preformatted Image sequence */
    NULL, NULL, NULL		/* Pointers to other sequences */
};


DUL_NETWORKKEY *networkKey = NULL;
DUL_ASSOCIATIONKEY *associationKey = NULL;
DUL_ASSOCIATESERVICEPARAMETERS associationParameters;
