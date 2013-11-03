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
+-+-+-+-+-+-+-+-+-
*/
/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	David E. Beecher
** Intent:		Test program for Sybase TBL functions
** Last Update:		$Author: smm $, $Date: 1998-08-03 16:28:22 $
** Source File:		$RCSfile: ttselect.c,v $
** Revision:		$Revision: 1.7 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.7 $ $RCSfile: ttselect.c,v $";

#include <stdio.h>
#include <stdlib.h>
#include "dicom.h"
#include "ctnthread.h"
#include "condition.h"
#include "tbl.h"
char fname[50],
    lname[50],
    phone[20];
int
    zip,
    record,
    age;
float
    weight;

static char textTest[200];

static CONDITION callback(TBL_FIELD * fields, int count, void *foo);

main()
{
    CONDITION
	cond;
    TBL_HANDLE
	* handle;
/*
    TBL_CRITERIA
	criteria[10];
*/
    long
        count;
    void
       *foo = NULL;

    static TBL_FIELD
        fields[] = {
	{"FNAME", TBL_STRING, 50, 0, 1, (void *) fname},
	{"LNAME", TBL_STRING, 50, 0, 1, (void *) lname},
	{"AGE", TBL_SIGNED4, 4, 0, 1, &age},
	{"ZIP", TBL_SIGNED4, 4, 0, 1, &zip},
	{"WEIGHT", TBL_FLOAT4, 4, 0, 1, &weight},
	{NULL}
    };

    static TBL_FIELD
        fields1[] = {
	{"T_TEXT", TBL_TEXT, 50, 0, 1, (void *) textTest},
	{NULL}
    };

    THR_Init();
    cond = TBL_Open("TBLTest", "TBL_Persons", &handle);
    if (cond != TBL_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
/*
    criteria[0].FieldName = "LNAME";
    criteria[0].Operator = TBL_EQUAL;
    TBL_LOAD_STRING(&criteria[0].Value, "JONES");

    criteria[1].FieldName = 0;
*/

    cond = TBL_Select(&handle, NULL, fields, &count, callback, (void *) foo);
    if (cond != TBL_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(2);
    }
    printf("ALL DONE--Count: %d\n", count);

    (void) TBL_Close(&handle);

#ifdef _MSC_VER
    cond = TBL_Open("TBLTest", "TBL_DataTypes", &handle);
    if (cond != TBL_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    count = 0;
    cond = TBL_Select(&handle, NULL, fields1, &count, callback, (void *) foo);
    if (cond != TBL_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(3);
    }
    printf("ALL DONE--Count: %d\n", count);


#endif

    THR_Shutdown();
    exit(0);
}

static CONDITION
callback(TBL_FIELD * fields, int count, void *foo)
{
    TBL_FIELD *fp;

    printf("\n\nIn callback: Count is: %d\n", count);
    fp = fields;
    while (fp->FieldName != 0) {
	printf("Field Name: %s [%d]:", fp->FieldName, fp->Value.Type);
	if (fp->Value.Type == TBL_STRING)
	    printf(" %s\n\n", fp->Value.Value.String);
	else if (fp->Value.Type == TBL_TEXT)
	    printf(" %s\n\n", fp->Value.Value.String);
	else if (fp->Value.Type == TBL_SIGNED4)
	    printf(" %d\n\n", *(fp->Value.Value.Signed4));
	else if (fp->Value.Type == TBL_FLOAT4)
	    printf(" %.3f\n\n", *(fp->Value.Value.Float4));
	else
	    printf(" UNSUPPORTED TYPE FOR PRINT\n\n");
	fp++;
    }
    return TBL_NORMAL;
}
