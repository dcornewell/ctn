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
** Module Name(s):	ctndisp (main())
** Author, Date:	David E. Beecher, 30-Jun-93
** Intent:		Display server for the imaging demonstration
** Last Update:		$Author: smm $, $Date: 1995-12-14 14:46:13 $
** Source File:		$RCSfile: ctnnetwork.h,v $
** Revision:		$Revision: 1.8 $
** Status:		$State: Exp $
*/

#ifndef _CTNNETWORK_IS_IN
#define _CTNNETWORK_IS_IN

#define		KILL_DISPLAY		"KILL_DISPLAY"

#define         MIN_DISPLAY_WIDTH   	100
#define         MIN_DISPLAY_HEIGHT	100
#define		MAX_STRING_LENGTH	256
/*
 * Max percetage of screen to use for image display
 */
#define		QUEUE_SIZE		256

/*
 * Error Conditions for functions
 */

#define	FAC_CTNNETWORK		4095

#define CTNNETWORK_NORMAL       FORM_COND (FAC_CTNNETWORK, SEV_SUCC, 1)
#define CTNNETWORK_NOXWINDOWS 	FORM_COND (FAC_CTNNETWORK, SEV_FATAL, 2)
#define CTNNETWORK_NOGQ		FORM_COND (FAC_CTNNETWORK, SEV_FATAL, 3)
#define CTNNETWORK_NOCOLORS	FORM_COND (FAC_CTNNETWORK, SEV_FATAL, 4)
#define CTNNETWORK_NOSERVER 	FORM_COND (FAC_CTNNETWORK, SEV_FATAL, 5)
#define CTNNETWORK_NOMEMORY	FORM_COND (FAC_CTNNETWORK, SEV_WARN, 6)
#define CTNNETWORK_NOFONT	FORM_COND (FAC_CTNNETWORK, SEV_WARN, 7)
#define CTNNETWORK_NOGC		FORM_COND (FAC_CTNNETWORK, SEV_FATAL, 8)
#define CTNNETWORK_NOGRAPHICS	FORM_COND (FAC_CTNNETWORK, SEV_FATAL, 9)
#define CTNNETWORK_CMDLINE	FORM_COND (FAC_CTNNETWORK, SEV_FATAL, 10)

/*
 * Function Prototypes
 */
CONDITION CTNNETWORK_InitializeX(void);
CONDITION CTNNETWORK_CreateGC(void);
void CTNNETWORK_CreateNetworkWindow();
void CTNNETWORK_CloseConnection();
void CTNNETWORK_ShowPercentage(int slot);
void CTNNETWORK_OpenConnection(int slot);
void CTNNETWORK_RefreshStatus(void);

#endif
