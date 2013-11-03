
/*
+-+-+-+-+-+-+-+-+-
*/
/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	Aniruddha Gokhale, 24-May-1995
** Intent:		Common declarations shared between many files
** Last Update:		$Author: smm $, $Date: 1998-08-03 16:13:18 $
** Source File:		$RCSfile: modify.h,v $
** Revision:		$Revision: 1.4 $
** Status:		$State: Exp $
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _MSC_VER
#include <io.h>
#include <fcntl.h>
#else
#include <sys/file.h>
#endif
#ifdef SOLARIS
#include <sys/fcntl.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>

#include "dicom.h"
#include "ctnthread.h"
#include "condition.h"
#include "lst.h"
#include "dicom_objects.h"

#define APP_NORMAL		FORM_COND(FAC_APP, SEV_SUCC, 1)
#define APP_FAILURE		FORM_COND(FAC_APP, SEV_ERROR, 2)
typedef struct {		/* list of elements to be modified */
    void *reserved[2];
    DCM_ELEMENT *e;
}   APP_ELEMLIST;

typedef struct {
    void *reserved[2];
    LST_HEAD *ael;		/* sequence of a list of elements */
}   APP_ITEMSEQ;

typedef struct {
    void *reserved[2];
    char str[DICOM_LO_LENGTH + 1];	/* allocate largest length in DICOM */
}   MULTVAL_ITEM;

extern FILE
*   yyin;			/* file parsed by the YACC parser */
extern LST_HEAD
*   modlist;			/* holds the modifications to the object */
extern CTNBOOLEAN
    verbose;

CONDITION
makeElement(int, int, void *, DCM_ELEMENT **);
CONDITION
makeModifications(LST_HEAD **, DCM_OBJECT **);
void
    APP_DumpModList(LST_HEAD **, int);
void yyerror(const char *);
