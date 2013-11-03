
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
** Last Update:		$Author: smm $, $Date: 2004-11-15 01:50:13 $
** Source File:		$RCSfile: create.h,v $
** Revision:		$Revision: 1.9 $
** Status:		$State: Exp $
*/

#ifndef _CREATE_H_
#define _CREATE_H_

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

typedef struct {
    void *reserved[2];
    char str[4096];	/* allocate largest length in DICOM */
}   MULTVAL_ITEM;

extern FILE
*   yyin;			/* file parsed by the YACC parser */
extern DCM_OBJECT
*   object;			/* holds the created object */
extern CTNBOOLEAN
    verbose;
CONDITION
makeElement(int, int, void *, DCM_ELEMENT **);
void yyerror(const char *);

#endif
