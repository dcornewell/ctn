
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
** Author, Date:	Aniruddha S. Gokhale, August 4, 1993.
** Intent:
** Last Update:		$Author: asg $, $Date: 1993-08-05 13:57:07 $
** Source File:		$Source: /home/smm/ctn/ctn/apps/print_clients/print_mgr_private.h,v $
** Revision:		$Revision: 1.2 $
** Status:		$State: Exp $
*/

/* prototype declarations of private functions used */

CONDITION
ngetCallback(MSG_N_GET_REQ * ngetRequest,
	     MSG_N_GET_RESP * ngetResponse,
	     void *ctx);
CONDITION
ncreateBFSCallback(MSG_N_CREATE_REQ * createRequest,
		   MSG_N_CREATE_RESP * createResponse,
		   void *ctx);
CONDITION
ncreateBFBCallback(MSG_N_CREATE_REQ * createRequest,
		   MSG_N_CREATE_RESP * createResponse,
		   void *ctx);
BOOLEAN
sendSetImageBox(DUL_ASSOCIATIONKEY ** association,
		DUL_ASSOCIATESERVICEPARAMETERS * params,
		char *SOPClass,
		char *uid,
		int position,
		MSG_N_SET_REQ * request,
		MSG_N_SET_RESP ** response);
CONDITION
nsetBIBCallback(MSG_N_SET_REQ * setRequest,
		MSG_N_SET_RESP * setResponse,
		void *ctx);
