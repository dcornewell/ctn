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
** @$=@$=@$=
*/
/*
**				DICOM 93
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
**
** Author, Date:	Aniruddha S. Gokhale, August 4, 1993.
** Intent:		This module declares the prototypes of the functions
**			that are used by both the print_mgr and the print_client
** Last Update:		$Author: chander $, $Date: 1994-10-10 20:41:19 $
** Source File:		$RCSfile: print_shared.h,v $
** Revision:		$Revision: 1.1 $
** Status:		$State: Exp $
*/

CONDITION
initNetwork(DUL_ASSOCIATESERVICEPARAMETERS * params,
	    DUL_NETWORKKEY ** network);

CONDITION
establishAssociation(DUL_ASSOCIATESERVICEPARAMETERS * params,
		     DUL_NETWORKKEY ** network,
		     char **SOPClassList,
		     DUL_ASSOCIATIONKEY ** association);

IMG_DISPLAYFORMAT
* decodeImageDisplayFormat(char *displayFormat);

CONDITION
getImageData(char *dbkey, char *imageUID, DCM_OBJECT ** imageObject);

CONDITION
setImageBox(DUL_ASSOCIATIONKEY ** association,
	    DUL_ASSOCIATESERVICEPARAMETERS * params,
	    char *SOPClass,
	    PRN_BASICIMAGEBOX * imageBox,
	    unsigned short *stat);

CONDITION
sendFilmBoxPrint(DUL_ASSOCIATIONKEY ** association,
		 DUL_ASSOCIATESERVICEPARAMETERS * params,
		 char *SOPClass, char *classUID, PRN_BASICFILMBOX * filmBox,
		 unsigned short *stat);

CONDITION
sendDeleteRequest(DUL_ASSOCIATIONKEY ** association,
		  DUL_ASSOCIATESERVICEPARAMETERS * params,
		  char *SOPClass, char *classUID, char *instanceUID,
		  unsigned short *stat);

CONDITION
createFilmSession(DUL_ASSOCIATIONKEY ** association,
		  DUL_ASSOCIATESERVICEPARAMETERS * params,
		  PRN_BASICFILMSESSION * bfsAttributes, char *SOPClass,
		  unsigned short *stat);

CONDITION
createFilmBox(DUL_ASSOCIATIONKEY ** association,
	      DUL_ASSOCIATESERVICEPARAMETERS * params,
	      PRN_BASICFILMBOX * bfbAttributes,
	      char *SOPClass,
	      unsigned short *stat);

CONDITION
getPrinterInstance(DUL_ASSOCIATIONKEY ** association,
		   DUL_ASSOCIATESERVICEPARAMETERS * params, char *SOPClass,
		   PRN_PRINTER ** printerAttributes,
		   unsigned short *stat);
