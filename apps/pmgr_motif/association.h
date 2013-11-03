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
** Intent:		Provide typedefs and prototypes for functions
**			for establishing associations with DICOM film
**			printers.
** Last Update:		$Author: chander $, $Date: 1994-10-10 20:40:45 $
** Source File:		$Source: /home/smm/ctn/ctn/apps/pmgr_motif/association.h,v $
** Revision:		$Revision: 1.1 $
** Status:		$State: Exp $
*/

#ifndef _ASSOC_IS_IN
#define _ASSOC_IS_IN

CONDITION
requestPrintAssociation(char *callingApplication,
			char *calledApplication, DMAN_HANDLE ** h,
	       DUL_NETWORKKEY ** network, DUL_ASSOCIATIONKEY ** association,
			DUL_ASSOCIATESERVICEPARAMETERS * params);
#endif
