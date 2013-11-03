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
** @$=@$=@$=
*/
/*
**                              DICOM 93
**                   Electronic Radiology Laboratory
**                 Mallinckrodt Institute of Radiology
**              Washington University School of Medicine
**
** Module Name(s):      CFG.H
** Author, Date:        Sheldon Hoffman, 24 Apr 93
** Intent:              Header file definitions for CFG routines
** Last Update:         $Author: smm $, $Date: 1996-08-04 03:38:11 $
** Source File:         $RCSfile: cfg.h,v $
** Revision:            $Revision: 1.9 $
** Status:              $State: Exp $
*/

/*  CFG.H - Structure definitions for access CFG configuration files
**
** USAGE
**      #include "condition.h"
**      #include "cfg.h"
**
** DESCRIPTION
**      Include this file for CFG prototypes and structure definitions.
*/
/* static char rcsid[] = "$Revision: 1.9 $ $RCSfile: cfg.h,v $"; */

#ifndef CFG_H
#define CFG_H			/* Prevent multiple includes */

#ifdef __cplusplus
extern "C" {
#endif

/* CONDITIONS RETURNED BY CFG FUNCTIONS */

#define CFG_NORMAL     \
    FORM_COND(FAC_CFG, SEV_SUCC,  1)	/* Operation Successful */
#define CFG_CANTOPEN   \
    FORM_COND(FAC_CFG, SEV_ERROR, 3)	/* Can't open file */
#define CFG_BADLINE    \
    FORM_COND(FAC_CFG, SEV_ERROR, 5)	/* Improperly formatted line */
#define CFG_NOENTRIES  \
    FORM_COND(FAC_CFG, SEV_ERROR, 7)	/* No entries in file */
#define CFG_NOMEMORY   \
    FORM_COND(FAC_CFG, SEV_ERROR, 9)	/* No memory for structure */

    typedef struct {
	char ctn_node[18];
	char title[18];
	char file[256];
	char imagepath[256];
	char quota[18];
	char read[18];
	char write[18];
    }   CFG_DATABASE;

    typedef struct {
	char ctn_node[18];
	char login[18];
	char title[18];
	char flag[18];
	char spool[18];
	char vendor_node[18];
	char port[18];
    }   CFG_CONNECT;

    CONDITION CFG_LoadDatabase(char *, char *, CFG_DATABASE **, short *);
    CONDITION CFG_LoadConnect(char *, char *, CFG_CONNECT **, short *);
    CONDITION CFG_LoadConnect1(char *, char *, CFG_CONNECT **, short *, char *);
    void CFG_free(void *);

#ifdef __cplusplus
}
#endif

#endif				/* CFG_H */
