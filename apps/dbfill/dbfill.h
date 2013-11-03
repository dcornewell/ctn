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
**                              DICOM 93
**                   Electronic Radiology Laboratory
**                 Mallinckrodt Institute of Radiology
**              Washington University School of Medicine
**
** Module Name(s):
** Author, Date:        Sheldon Hoffman
** Intent:              Definitions for DBUTIL DB utility
**
** Last Update:         $Author: smm $, $Date: 1994-12-30 18:23:34 $
** Source File:         $RCSfile: dbfill.h,v $
** Revision:            $Revision: 1.4 $
** Status:              $State: Exp $
*/

#ifndef DBUTIL_H
#define DBUTIL_H		/* Include only once            */

#include    "dicom.h"
#include    "lst.h"
#include    "cfg.h"
#include    "condition.h"
#include    "dulprotocol.h"
#include    "dicom_objects.h"
#include    "dicom_messages.h"
#include    "dicom_services.h"
#include    "dicom_uids.h"
#include    "dbquery.h"

typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned long ULONG;

/* DIAGNOSTIC MESSAGE LEVELS                                            */

#define DIAG_SEVERE     0
#define DIAG_WARNING    1
#define DIAG_INFORM     2
#define DIAG_VERBOSE    3
#define DIAG_ALWAYS     4


#endif				/* #ifndef DBUTIL_H    */
