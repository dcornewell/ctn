/*
+-+-+-+-+-+-+-+-+-
*/
/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	Stephen Moore
** Intent:		Test program for TBL functions
** Last Update:		$Author: beech $, $Date: 1994-07-07 17:44:52 $
** Source File:		$RCSfile: tbltest.c,v $
** Revision:		$Revision: 1.1 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.1 $ $RCSfile: tbltest.c,v $";

#include <stdio.h>
#include "facilities.h"
#include "sitemacros.h"
#include "condition.h"
#include "tbl.h"

typedef struct {
    char title[128];
    char node[128];
    char port[128];
}   RECORD;
static CONDITION callback(TBL_FIELD * fields, long count, RECORD * record);

main()
{
    CONDITION
	cond;
    TBL_HANDLE
	* handle;
    TBL_CRITERIA
	criteria[2];
    static RECORD record;

    static TBL_FIELD
        fields[] = {{TBL_FIELD_STRING("TITLE", record.title)},
    {TBL_FIELD_STRING("NODE", record.node)},
    {TBL_FIELD_STRING("PORT", record.port)},
    {NULL}
    };

    cond = TBL_Open("", "APPLICATION_ENTITY", &handle);
    if (cond != TBL_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
    criteria[0].FieldName = "NODE";
    criteria[0].Operator = TBL_EQUAL;
    TBL_LOAD_STRING(&criteria[0].Value, "dicom1");

    cond = TBL_Select(&handle, criteria, fields, NULL, callback, &record);
    if (cond != TBL_NORMAL) {
	COND_DumpConditions();
	exit(2);
    }
    exit(0);
}

static CONDITION 
callback(TBL_FIELD * fields, long count, RECORD * record)
{
    printf("%3d %s %s %s\n", count, record->title, record->node, record->port);
}
