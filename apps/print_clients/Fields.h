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
** Module Name(s):
** Author, Date:	Tze Khiong Lee
** Intent:		Header File for Fields.c
** Last Update:		$Author: smm $, $Date: 1995-02-22 05:17:52 $
** Source File:		$RCSfile: Fields.h,v $
** Revision:		$Revision: 1.5 $
** Status:		$State: Exp $
*/

typedef struct {
    char *buttonlabel,
       *suffix,
       *data,
       *storage;
    int storagesize;
    Widget text;
}   RBTNwTEXTFIELD;

typedef struct {
    char *buttonlabel,
       *data,
       *label1,
       *storage1;
    int storagesize1;
    char *label2,
       *storage2;
    int storagesize2;
    Widget text1,
        text2;
}   RBTNw2TEXTFIELD;

typedef struct {
    char *buttonlabel,
       *data;
}   RBTNwLABEL;

typedef enum {
    BTN_RBTNwTEXTFIELD,
    BTN_RBTNw2TEXTFIELD,
    BTN_RBTNwLABEL
}   RADIOBUTTONTYPE;

typedef struct {
    RADIOBUTTONTYPE type;
    union {
	RBTNwTEXTFIELD *rbtf;
	RBTNw2TEXTFIELD *rb2tf;
	RBTNwLABEL *rblbl;
    }   btns;
    Widget button;
}   ALLRBTN;

typedef struct {
    char *fieldlabel,
       *storage;
    ALLRBTN *rbtns;
    int numrbtns;
}   FIELDwRBTN;

typedef struct {
    char *fieldlabel,
       *storage;
    int storagesize;
    Boolean editable;
    Widget text;
}   FIELDwTEXT;

typedef enum {
    FLD_FIELDwRBTN,
    FLD_FIELDwTEXT
}   FIELDTYPE;

typedef enum {
    REQUIRED,
    OPTIONAL
}   REQUIREMENT;

typedef struct {
    FIELDTYPE type;
    REQUIREMENT require;
    union {
	FIELDwRBTN *frbtn;
	FIELDwTEXT *ftext;
    }   field;
}   ENTRY;

#define GetRbtnForm(w)	XtParent(XtParent(w))

Widget
CreateEntryForm(Widget parent, Widget top, Widget left, ENTRY entry[], int num);
void
    UpdateAllEntries();
