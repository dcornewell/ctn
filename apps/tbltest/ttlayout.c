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
** Last Update:		$Author: smm $, $Date: 1998-08-03 16:28:25 $
** Source File:		$RCSfile: ttlayout.c,v $
** Revision:		$Revision: 1.6 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.6 $ $RCSfile: ttlayout.c,v $";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

static CONDITION callback(TBL_FIELD * fields, void *foo);

main()
{
    CONDITION
	cond;
    char
        tab[50],
        db[50];
    TBL_HANDLE
	* handle;

    void
       *foo = NULL;


    printf("Input database: ");
    scanf("%s", db);
    printf("Input table: ");
    scanf("%s", tab);

    THR_Init();
    cond = TBL_Open(db, tab, &handle);
    if (cond != TBL_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
    cond = TBL_Layout(db, tab, callback, (void *) foo);
    if (cond != TBL_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(2);
    }
    THR_Shutdown();
    exit(0);
}

static CONDITION
callback(TBL_FIELD * fields, void *foo)
{
    static int count = 0;

    count++;
    printf("Column #: %3d  Length: %6d  Type: ", count, fields->Value.AllocatedSize);
    switch (fields->Value.Type) {
    case TBL_OTHER:
	printf("Other      ");
	break;
    case TBL_TEXT:
	printf("Text       ");
	break;
    case TBL_STRING:
	printf("String     ");
	break;
    case TBL_BINARYDATA:
	printf("BinaryData ");
	break;
    case TBL_UNSIGNED2:
	printf("Unsigned2  ");
	break;
    case TBL_UNSIGNED4:
	printf("Unsigned4  ");
	break;
    case TBL_SIGNED2:
	printf("Signed2    ");
	break;
    case TBL_SIGNED4:
	printf("Signed4    ");
	break;
    case TBL_FLOAT4:
	printf("Float4     ");
	break;
    case TBL_FLOAT8:
	printf("Float8     ");
	break;
    }
    printf(" Name: %s\n", fields->FieldName);
    return TBL_NORMAL;
}
