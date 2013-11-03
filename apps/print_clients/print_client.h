
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
** Author, Date:	Aniruddha Gokhale
**			Tze Lee
** Intent:
** Last Update:		$Author: tkl $, $Date: 1993-08-02 16:52:32 $
** Source File:		$Source: /home/smm/ctn/ctn/apps/print_clients/print_client.h,v $
** Revision:		$Revision: 1.2 $
** Status:		$State: Exp $
*/

CONDITION
initNetwork(DUL_ASSOCIATESERVICEPARAMETERS * params,
	    DUL_NETWORKKEY ** network);

CONDITION
establishAssociation(DUL_ASSOCIATESERVICEPARAMETERS * params,
		     DUL_NETWORKKEY ** network,
		     char *SOPClass,
		     DUL_ASSOCIATIONKEY ** association);

CONDITION
sendGetPrinterInstance(DUL_ASSOCIATIONKEY ** association,
		       DUL_ASSOCIATESERVICEPARAMETERS * params,
		       char *SOPClass,
		       PRINTER_ATTRIBUTES * printAttrib);

CONDITION
sendCreateFilmSession(DUL_ASSOCIATIONKEY ** association,
		      DUL_ASSOCIATESERVICEPARAMETERS * params,
		      char *SOPClass,
		      BFS_ATTRIBUTES * bfsAttributes);
CONDITION
sendCreateFilmBox(DUL_ASSOCIATIONKEY ** association,
		  DUL_ASSOCIATESERVICEPARAMETERS * params,
		  char *SOPClass,
		  BFB_ATTRIBUTES * bfbAttributes);
