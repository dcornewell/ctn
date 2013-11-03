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
** Module Name(s):	MUT_LoadList
**			MUT_ScanWidget
** Author, Date:	Stephen Moore, summer 1994
** Intent:		Provide a set of general utility functions useful
**			in the Motif environment.
** Last Update:		$Author: smm $, $Date: 2000-01-20 17:27:52 $
** Source File:		$RCSfile: motifutil.c,v $
** Revision:		$Revision: 1.12 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.12 $ $RCSfile: motifutil.c,v $";

#include <stdio.h>
#ifdef MOTIF
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>

#include <Xm/Label.h>
#include <Xm/TextF.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/CascadeB.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>
#include <X11/Shell.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

#include <Xm/Text.h>

#include "dicom.h"
#include "lst.h"
#include "mut.h"

/* MUT_LoadList
**
** Purpose:
**	MUT_LoadList is used to provide a uniform mechanism for loading text
**	items into a Motif scrolled list widget.  For each item in the caller's
**	lst, MUT_LoadList invokes the caller's format function with the
**	arguments:
**		node	The node in the list
**		i	The index of the node in the list (0 ... N-1)
**		buffer	The ASCII buffer to be filled by format.
**	format is expected to examine the data in node and create a line of
**	text to be placed in the scrolled
**
** Parameter Dictionary:
**	Define the parameters to the function
**
** Return Values:
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
void MUT_LoadList(Widget w, LST_HEAD * lst, void (*format) (), char *buf) {
    int
        i,
        count;
    LST_NODE
	* node;
    XmString
	* xmstr = NULL;
    Arg
	args[2];

    if (lst == NULL)
	return;

    count = LST_Count(&lst);
    if (count != 0) {
	xmstr = (XmString *) XtMalloc(sizeof(XmString) * count);
	if (xmstr == NULL)
	    return;
    }
    node = LST_Head(&lst);
    if (node != NULL)
	(void) LST_Position(&lst, node);

    for (i = 0; i < count; i++) {
	format(node, i, buf);
	xmstr[i] = XmStringCreate(buf, XmSTRING_DEFAULT_CHARSET);
	node = LST_Next(&lst);
    }
    i = 0;
    XtSetArg(args[i], XmNitems, xmstr);
    i++;
    XtSetArg(args[i], XmNitemCount, count);
    i++;
    XtSetValues(w, args, 2);
    if (xmstr != NULL)
	XtFree((char *) xmstr);
    XmListDeselectAllItems(w);
}
CONDITION
MUT_ScanWidget(Widget w, MUT_DATATYPE type, CTNBOOLEAN * nullFlag, void *d)
{
    char
       *txt;
    CONDITION
	rValue = MUT_NORMAL;

    txt = XmTextGetString(w);
    if (strlen(txt) == 0)
	*nullFlag = TRUE;
    else
	*nullFlag = FALSE;

    if (*nullFlag == FALSE) {
	switch (type) {
	case MUT_TEXT:
	    strcpy(d, txt);
	    break;
	case MUT_INT:
	    if (sscanf(txt, "%d", d) != 1)
		rValue = MUT_SCANFAILURE;
	    break;
	case MUT_FLOAT:
	    if (sscanf(txt, "%f", d) != 1)
		rValue = MUT_SCANFAILURE;
	    break;
	case MUT_US:
	    if (sscanf(txt, "%hd", d) != 1)
		rValue = MUT_SCANFAILURE;
	    break;
	}
    } else {
	switch (type) {
	case MUT_TEXT:
	    *(char *) d = '\0';
	    break;
	case MUT_INT:
	    *(int *) d = 0;
	    break;
	case MUT_FLOAT:
	    *(float *) d = 0.;
	    break;
	}
    }

    XtFree(txt);
    return rValue;
}

CONDITION
MUT_DisplayConditionText(Widget w)
{
    char text[1024];

    COND_CopyText(text, sizeof(text));
    XmTextSetString(w, text);

    return MUT_NORMAL;
}

#endif

void
SgePreInitialize()
{
}
