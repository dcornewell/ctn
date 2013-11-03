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
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	David E. Beecher, January 1994
** Intent:		Private include file for tbl_sybase.c
** Last Update:		$Author: smm $, $Date: 1998-06-10 21:57:04 $
** Source File:		$RCSfile: tbl_sqlserver.h,v $
** Revision:		$Revision: 1.1 $
** Status:		$State: Exp $
*/


#ifndef _TBL_MSSQL_SERVER_IS_IN
#define _TBL_MSSQL_SERVER_IS_IN 1

#ifdef  __cplusplus
extern "C" {
#endif

/*#define	SYBASE_USER	"sybase"*/
/*#define SYBASE_PASSWORD	"sybase"*/
/*#define SYBASE_APP	"DICOM"*/

#define	BIG_2	32767
#define	BIG_4	999999999

/*
 * Internal function calls
 */

#if 0
static void
    Add_Field_to_Select(DBPROCESS *, TBL_FIELD *),
    Add_Criteria_to_Sybuf(DBPROCESS *, TBL_CRITERIA *),
    Add_Value_to_Sybuf(DBPROCESS *, TBL_FIELD *),
    Add_UpdateValue_to_Sybuf(DBPROCESS *, TBL_UPDATE *);

static int
    syb_err_handler(DBPROCESS *, int, int, int, char *, char *),
    syb_msg_handler(DBPROCESS *, DBINT, int, int, char *, char *, char *, DBUSMALLINT);
#endif

#define DATABASE "MS SQL Server"

#ifdef  __cplusplus
}
#endif

#endif
