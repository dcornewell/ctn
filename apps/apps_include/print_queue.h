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
**				DICOM 93
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	Aniruddha Gokhale
** Intent:		Header File defining the structure of the
**			element to be stored in the GQueue
** Last Update:		$Author: smm $, $Date: 1994-12-30 00:12:31 $
** Source File:		$RCSfile: print_queue.h,v $
** Revision:		$Revision: 1.2 $
** Status:		$State: Exp $
*/

typedef enum {
    PRN_NEWFILMSESSION,
    PRN_NEWFILMBOX,
    PRN_NEWIMAGEBOX,
    PRN_PRINTCOMMAND,
    PRN_ENDSESSION,
    PRN_TOTALFAILURE
}   PRN_MESSAGES;

typedef struct {
    PRN_MESSAGES prnMessage;
    char instanceUID[DICOM_UI_LENGTH + 1];
    char objFile[256];
}   GQ_ELEM;

#define KILL_DISPLAY "KILL_DISPLAY"
