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
** Last Update:		$Author: smm $, $Date: 2000-11-09 19:57:04 $
** Source File:		$RCSfile: ttupdate.c,v $
** Revision:		$Revision: 1.8 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.8 $ $RCSfile: ttupdate.c,v $";

#include <stdio.h>
#include <stdlib.h>
#include "dicom.h"
#include "ctnthread.h"
#include "condition.h"
#include "tbl.h"
char
    fname[50],
    lname[50],
    phone[20];

static CONDITION callback(TBL_FIELD * fields, int count, void *record);

main()
{
    CONDITION
	cond;
    TBL_HANDLE
	* handle;
    TBL_CRITERIA
	criteria[10];
    int
        age;

    static TBL_UPDATE
        fields[] = {
	{"LNAME", TBL_SET, TBL_STRING, 50, 0, 1, (void *) "WOODROW"},
	{NULL}
    };
#ifdef _MSC_VER
    static TBL_UPDATE
        fields1[] = {
	{"T_TEXT", TBL_SET, TBL_STRING, 50, 0, 1, (void *) "Updated Text Data"},
	{NULL}
    };
#endif

    THR_Init();
    cond = TBL_Open("TBLTest", "TBL_Persons", &handle);
    if (cond != TBL_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    criteria[0].FieldName = "AGE";
    criteria[0].Operator = TBL_EQUAL;
    criteria[0].Value.Type = TBL_SIGNED4;
    age = 50;
    criteria[0].Value.Value.Signed4 = &age;
    criteria[0].Value.IsNull = 0;
    criteria[0].Value.AllocatedSize = 4;
    criteria[1].FieldName = 0;

    cond = TBL_Update(&handle, criteria, fields);
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
    cond = TBL_Update(&handle, NULL, fields1);
    if (cond != TBL_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(3);
    }
    (void) TBL_Close(&handle);
#endif

    printf("\n\nUpdate operation succeeded\n\n");

    THR_Shutdown();
    exit(0);
}
