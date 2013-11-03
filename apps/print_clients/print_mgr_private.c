
/*
+-+-+-+-+-+-+-+-+-
*/
/*
**				DICOM 93
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	Aniruddha S. Gokhale, August 4, 1993
** Intent:
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:51:25 $
** Source File:		$Source: /home/smm/ctn/ctn/apps/print_clients/print_mgr_private.c,v $
** Revision:		$Revision: 1.2 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.2 $ $RCSfile: print_mgr_private.c,v $";
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
#include "dicom_n_printer.h"

/* Extern Declarations */
extern char bfsInstanceUID[DICOM_UI_LENGTH + 1];
extern char bfbInstanceUID[DICOM_UI_LENGTH + 1];
extern PRINTER_ATTRIBUTES printAttrib;

/* ngetCallback
**
** Purpose:
**	The call back routine which should take the appropriate
**	actions on receiving the N-GET response message
**
** Parameter Dictionary:
**	ngetRequest	the N-GET request message
**	ngetResponse	the N-GET response message
**	ctx		the associated context variable
**
** Return Values:
**	should return SRV_NORMAL in normal conditions
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
ngetCallback(MSG_N_GET_REQ * ngetRequest,
	     MSG_N_GET_RESP * ngetResponse,
	     void *ctx)
{
    CONDITION
	cond;

    /* Now print the attributes of the printer that we have received */
    printf("SCU : N-GET response from the printer\n");
    printf("  Attributes of the printer are : - \n");
    cond = DCM_DumpElements(&ngetResponse->dataSet);
    if (cond != DCM_NORMAL) {
	printf(" In ngetCallback : DCM_DumpElements failed\n");
	return cond;
    }
    /* convert the response message received into a structure */
    cond = convertObjectToStruct(&ngetResponse->dataSet, &printAttrib);
    if (cond != DCM_NORMAL) {
	printf("In sendGetPrinterInstance, convertObjectToStruct failed\n");
	return cond;
    }
    if (freeAll((void **) &ngetResponse))
	printf("N-GET Response PROPERLY FREED\n\n");

    return SRV_NORMAL;
}

/* ncreateBFSCallback
**
** Purpose:
**	Call back function for the N-CREATE request for BAsic Film Session
**
** Parameter Dictionary:
**	createRequest	N-CREATE request message for Basic Film Session
**	createResponse	N-CREATE response message for Basic Film Session
**	ctx		context variable
**
** Return Values:
**	Should return SRV_NORMAL under normal circumstances.
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
ncreateBFSCallback(MSG_N_CREATE_REQ * createRequest,
		   MSG_N_CREATE_RESP * createResponse,
		   void *ctx)
{
    CONDITION
	cond;

    /* print attribute list, status and SOP instance UID */
    printf("SCU : Received N-CREATE Response message\n");
    printf("SCU : Received BASIC FILM SESSION instance UID : %s\n",
	   createResponse->instanceUID);
    printf("SCU : Received the following updated BFS attributes : \n");
    if (createResponse->dataSetType != DCM_CMDDATANULL) {
	cond = DCM_DumpElements(&createResponse->dataSet);
	if (cond != DCM_NORMAL) {
	    printf("In sendCreateFilmSession : DCM_DumpElements failed\n");
	    return cond;
	}
    } else
	printf("NO DATA SET RECEIVED\n");

    (void) strcpy(bfsInstanceUID, createResponse->instanceUID);
    return SRV_NORMAL;
}
/* ncreateBFBCallback
**
** Purpose:
**	Call back function for N-CREATE request for BASIC FILM BOX
**
** Parameter Dictionary:
**	createRequest	N-CREATE request for Basic Film Box
**	createResponse	N-CREATE response for Basic Film Box
**	ctx		context variable
**
** Return Values:
**	should return SRV_NORMAL in normal circumstances
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
ncreateBFBCallback(MSG_N_CREATE_REQ * createRequest,
		   MSG_N_CREATE_RESP * createResponse,
		   void *ctx)
{
    CONDITION
	cond;

    /* print attribute list, status and SOP instance UID */
    printf("SCU : Received N-CREATE Response message\n");
    printf("SCU : Received Basic Film Box instance UID : %s\n",
	   createResponse->instanceUID);
    printf("SCU : Attributes received via Response message:-\n");
    cond = DCM_DumpElements(&createResponse->dataSet);
    if (cond != DCM_NORMAL) {
	printf("In ncreateBFBCallback : DCM_DumpElements failed\n");
	return cond;
    }
    return SRV_NORMAL;
}
/* nsetBIBCallback

**
** Purpose:
**	Callback function for the N-SET request for the Basic Image Box
**
** Parameter Dictionary:
**	setRequest	N-SET request to set attributes for Image Box
**	setResponse	N-SET response received from SCP
**	ctx		context information
**
** Return Values:
**	should return SRV_NORMAL under normal conditions
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
nsetBIBCallback(MSG_N_SET_REQ * setRequest,
		MSG_N_SET_RESP * setResponse,
		void *ctx)
{
    CONDITION
	cond;

    /* print attribute list */
    printf("SCU : Received N-SET Response message\n");
    printf("SCU : For the Image box instance UID : %s\n",
	   setResponse->instanceUID);
    printf("SCU : Received updated Image Box attributes :- \n");
    cond = DCM_DumpElements(&setResponse->dataSet);
    if (cond != DCM_NORMAL) {
	printf("In nsetBIBCallback : DCM_DumpElements failed\n");
	return cond;
    }
    return SRV_NORMAL;
}
