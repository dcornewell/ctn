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
**				DICOM 93
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Author, Date:	Tze Khiong Lee, Aug 3rd 1993
** Intent:
** Last Update:		$Author: smm $, $Date: 1996-08-23 19:37:44 $
** Source File:		$RCSfile: icon.h,v $
** Revision:		$Revision: 1.9 $
** Status:		$State: Exp $
*/

#ifdef  __cplusplus
extern "C" {
#endif


#define ICON_SIZE 4096
#define ICON_XSIZE 64
#define ICON_YSIZE 64
#define ICON_DEPTH 8

typedef struct {
    char
        accessionNumber[DICOM_CS_LENGTH + 1];
        off_t
        Offset;
}   ICON_STUDYOFFSET;

typedef struct {
    void
       *reserved[2];
        ICON_STUDYOFFSET
        studyoffset;
}   LIST_ICON_STUDYOFFSET;

typedef struct {
    char
        accessionNumber[DICOM_CS_LENGTH + 1],
        UID[DICOM_UI_LENGTH + 1];
    unsigned char
        Icon[4096];
}   ICON_STUDYICON;

typedef struct {
    void *reserved[2];
        ICON_STUDYICON
        studyicon;
}   LIST_ICON_STUDYICON;

CONDITION ICON_AppendStudyOffset(char *file, ICON_STUDYOFFSET * studyoffset);
CONDITION ICON_GetStudyOffset(char *file, char *accessionNumber, ICON_STUDYOFFSET * studyoffset);
CONDITION ICON_GetStudyOffsetLst(char *file, LST_HEAD ** studyoffsetlist);
CONDITION ICON_DumpStudyOffset(char *file);

CONDITION ICON_AppendStudyIcon(char *file, ICON_STUDYICON * studyicon);
CONDITION ICON_GetStudyIconLst(char *file, ICON_STUDYOFFSET * studyoffset, LST_HEAD ** studyiconlist);
CONDITION ICON_DumpStudyIcon(char *file);

char *ICON_Message(CONDITION cond);

#define	ICON_NORMAL		FORM_COND(FAC_ICON, SEV_SUCC, 1)
#define	ICON_FILEOPENFAILED	FORM_COND(FAC_ICON, SEV_ERROR, 2)
#define	ICON_APPENDFAILED	FORM_COND(FAC_ICON, SEV_ERROR, 3)
#define	ICON_LISTFAILURE	FORM_COND(FAC_ICON, SEV_ERROR, 4)
#define	ICON_MALLOCFAILURE	FORM_COND(FAC_ICON, SEV_ERROR, 5)
#define	ICON_INCORRECTFILESIZE	FORM_COND(FAC_ICON, SEV_ERROR, 6)
#define	ICON_OFFSETERROR	FORM_COND(FAC_ICON, SEV_ERROR, 7)
#define ICON_ACCESSIONNOTFOUND  FORM_COND(FAC_ICON, SEV_ERROR, 8)

#ifdef  __cplusplus
}
#endif
