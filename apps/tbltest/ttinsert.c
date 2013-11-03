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
** Intent:		Test program for Sybase TBL Insert functions
** Last Update:		$Author: smm $, $Date: 2000-11-09 19:56:44 $
** Source File:		$RCSfile: ttinsert.c,v $
** Revision:		$Revision: 1.8 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.8 $ $RCSfile: ttinsert.c,v $";

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
main()
{
    CONDITION
    cond;
    TBL_HANDLE
	* handle;
    int
        ta1,
        ta2,
        ta3,
        ta4,
        tz1,
        tz2,
        tz3,
        tz4;
    float
        tw1,
        tw2,
        tw3,
        tw4;

    static TBL_FIELD
        field1[] = {
	{"FNAME", TBL_STRING, 50, 0, 0, (void *) "JOE"},
	{"LNAME", TBL_STRING, 50, 0, 0, (void *) "JONES"},
	{"AGE", TBL_SIGNED4, 4, 0, 0, 0},
	{"ZIP", TBL_SIGNED4, 4, 0, 0, 0},
	{"WEIGHT", TBL_FLOAT4, 4, 0, 0, 0},
	{NULL}
    };
    static TBL_FIELD
        field2[] = {
	{"FNAME", TBL_STRING, 50, 0, 0, (void *) "SMELDA"},
	{"LNAME", TBL_STRING, 50, 0, 0, (void *) "SMITH"},
	{"AGE", TBL_SIGNED4, 4, 0, 0, 0},
	{"ZIP", TBL_SIGNED4, 4, 0, 0, 0},
	{"WEIGHT", TBL_FLOAT4, 4, 0, 0, 0},
	{NULL}
    };
    static TBL_FIELD
        field3[] = {
	{"FNAME", TBL_STRING, 50, 0, 0, (void *) "JOHN"},
	{"LNAME", TBL_STRING, 50, 0, 0, (void *) "JONES"},
	{"AGE", TBL_SIGNED4, 4, 0, 0, 0},
	{"ZIP", TBL_SIGNED4, 4, 0, 0, 0},
	{"WEIGHT", TBL_FLOAT4, 4, 0, 0, 0},
	{NULL}
    };
    static TBL_FIELD
        field4[] = {
	{"FNAME", TBL_STRING, 50, 0, 0, (void *) "SMITHY"},
	{"LNAME", TBL_STRING, 50, 0, 0, (void *) "SMITH"},
	{"AGE", TBL_SIGNED4, 4, 0, 0, 0},
	{"ZIP", TBL_SIGNED4, 4, 0, 0, 0},
	{"WEIGHT", TBL_FLOAT4, 4, 0, 0, 0},
	{NULL}
    };

#ifdef _MSC_VER
    S32 txtNum = 10;
    static TBL_FIELD field5[] = {
	{"T_TEXT", TBL_TEXT, 50, 0, 0, (void *) "Text Insertion Test"},
	{"T_INT", TBL_SIGNED4, 4, 0, 0, 0},
	{NULL}
    };
#endif

    ta1 = 30;
    field1[2].Value.Value.Signed4 = &ta1;
    tz1 = 63100;
    field1[3].Value.Value.Signed4 = &tz1;
    tw1 = (float)150.1;
    field1[4].Value.Value.Float4 = &tw1;
    ta2 = 40;
    field2[2].Value.Value.Signed4 = &ta2;
    tz2 = 63200;
    field2[3].Value.Value.Signed4 = &tz2;
    tw2 = (float)160.2;
    field2[4].Value.Value.Float4 = &tw2;
    ta3 = 50;
    field3[2].Value.Value.Signed4 = &ta3;
    tz3 = 63300;
    field3[3].Value.Value.Signed4 = &tz3;
    tw3 = 170.5;
    field3[4].Value.Value.Float4 = &tw3;
    ta4 = 60;
    field4[2].Value.Value.Signed4 = &ta4;
    tz4 = 63400;
    field4[3].Value.Value.Signed4 = &tz4;
    tw4 = 180.25;
    field4[4].Value.Value.Float4 = &tw4;

#ifdef _MSC_VER
    field5[1].Value.Value.Signed4 = &txtNum;
#endif

    THR_Init();
    cond = TBL_Open("TBLTest", "TBL_Persons", &handle);
    if (cond != TBL_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    cond = TBL_Insert(&handle, field1);
    if (cond != TBL_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(2);
    }
    cond = TBL_Insert(&handle, field2);
    if (cond != TBL_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(2);
    }
    cond = TBL_Insert(&handle, field3);
    if (cond != TBL_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(2);
    }
    cond = TBL_Insert(&handle, field4);
    if (cond != TBL_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(2);
    }
    (void) TBL_Close(&handle);

#ifdef _MSC_VER
    cond = TBL_Open("TBLTest", "TBL_DataTypes", &handle);
    if (cond != TBL_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    cond = TBL_Insert(&handle, field5);
    if (cond != TBL_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(5);
    }
    (void) TBL_Close(&handle);
#endif

    printf("\n\nAll Inserts succeeded\n\n");

    THR_Shutdown();
    return 0;
}
