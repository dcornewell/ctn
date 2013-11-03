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
** Last Update:		$Author: smm $, $Date: 1997-11-17 22:29:34 $
** Source File:		$RCSfile: ctndisp.h,v $
** Revision:		$Revision: 1.8 $
** Status:		$State: Exp $
*/

#ifndef _CTNDISP_IS_IN
#define _CTNDISP_IS_IN

#include "common_disp.h"

#define		KILL_DISPLAY		"KILL_DISPLAY"

#define         MIN_DISPLAY_WIDTH   	100
#define         MIN_DISPLAY_HEIGHT	100
/*
 * Max percetage of screen to use for image display
 */
#define		MAX_PERCT		.90
#define		TEXT_POSITION		5
#define		QUEUE_SIZE		256

/*
 * Error Conditions for functions
 */

#define	FAC_CTNDISP		4095

#define CTNDISP_NORMAL       	FORM_COND (FAC_CTNDISP, SEV_SUCC,   1)
#define CTNDISP_NOXWINDOWS 	FORM_COND (FAC_CTNDISP, SEV_FATAL,  2)
#define CTNDISP_NOGQ		FORM_COND (FAC_CTNDISP, SEV_FATAL,  3)
#define CTNDISP_NOKILLQUEUE	FORM_COND (FAC_CTNDISP, SEV_WARN,   4)
#define CTNDISP_IDXOPENFAIL	FORM_COND (FAC_CTNDISP, SEV_WARN,   5)
#define CTNDISP_IDXLOCKFAIL	FORM_COND (FAC_CTNDISP, SEV_WARN,   6)
#define CTNDISP_IDXNONODE	FORM_COND (FAC_CTNDISP, SEV_WARN,   7)
#define CTNDISP_NOIMAGEYET 	FORM_COND (FAC_CTNDISP, SEV_INFORM, 8)
#define CTNDISP_GETHEADERFAIL	FORM_COND (FAC_CTNDISP, SEV_INFORM, 9)
#define CTNDISP_NOCOLORS	FORM_COND (FAC_CTNDISP, SEV_FATAL,  10)
#define CTNDISP_NOSERVER 	FORM_COND (FAC_CTNDISP, SEV_FATAL,  11)
#define CTNDISP_NOMEMORY	FORM_COND (FAC_CTNDISP, SEV_WARN,   12)
#define CTNDISP_NOSCALEDATA	FORM_COND (FAC_CTNDISP, SEV_WARN,   13)
#define CTNDISP_CANTGETDATA	FORM_COND (FAC_CTNDISP, SEV_WARN,   14)
#define CTNDISP_NODOWNSAMPLEDATA FORM_COND (FAC_CTNDISP, SEV_WARN,  15)
#define CTNDISP_NOFONT		FORM_COND (FAC_CTNDISP, SEV_WARN,   16)
#define CTNDISP_NOGC		FORM_COND (FAC_CTNDISP, SEV_FATAL,  17)
#define CTNDISP_NOGRAPHICS	FORM_COND (FAC_CTNDISP, SEV_FATAL,  18)
#define CTNDISP_ACROPENFAIL	FORM_COND (FAC_CTNDISP, SEV_WARN,   19)
#define CTNDISP_ACRGETPIXELS	FORM_COND (FAC_CTNDISP, SEV_WARN,   20)
#define CTNDISP_NOACRELEMENT	FORM_COND (FAC_CTNDISP, SEV_WARN,   20)
#define CTNDISP_NOPIXELDATASIZE FORM_COND (FAC_CTNDISP, SEV_WARN,   21)
#define CTNDISP_NOPIXELDATA	FORM_COND (FAC_CTNDISP, SEV_WARN,   22)
#define CTNDISP_NOGETDATA	FORM_COND (FAC_CTNDISP, SEV_WARN,   23)
#define CTNDISP_NOPARTITION 	FORM_COND (FAC_CTNDISP, SEV_WARN,   24)
#define CTNDISP_ACRCLOSEFAIL	FORM_COND (FAC_CTNDISP, SEV_WARN,   25)
#define CTNDISP_CMDLINE		FORM_COND (FAC_CTNDISP, SEV_FATAL,  26)
#define CTNDISP_INFORM		FORM_COND (FAC_CTNDISP, SEV_INFORM, 27)
#define CTNDISP_NO8BIT 	        FORM_COND (FAC_CTNDISP, SEV_FATAL,  64)
/*
 * This is an informational message
 */
#define DICOMXDISP		FORM_COND (FAC_CTNDISP, SEV_INFORM,27)

/*
 * Function Prototypes
 */
void CTNDISP_DestroyImage(int NumImages);
CONDITION CTNDISP_LoadGrayMap(int n);
CONDITION CTNDISP_InitializeX(int ncolors);
/*void		CTNDISP_DisplayImageData( ImageStruct *img, XImageStruct *ximg );*/
/*void		CTNDISP_RefreshImageData( ImageStruct *img, XImageStruct *ximg );*/
CONDITION CTNDISP_CreateGC(void);

void CTNDISP_CreateIncomingOutgoing(void);
void CTNDISP_ProcessIncoming(short flag, short num);
void CTNDISP_ProcessOutgoing(short flag, short num);
void CTNDISP_CheckStatus(void);
void CTNDISP_RefreshStatus(void);
/*void CTNDISP_UnDisplayImageData(ImageStruct * img, XImageStruct * ximg);*/

#endif
