
/*******************************************************************************
       PrintMessage.h
       This header file is included by PrintMessage.c

*******************************************************************************/

#ifndef	_PRINTMESSAGE_INCLUDED
#define	_PRINTMESSAGE_INCLUDED


#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/MessageB.h>

/*******************************************************************************
       The definition of the context structure:
       If you create multiple copies of your interface, the context
       structure ensures that your callbacks use the variables for the
       correct copy.

       For each swidget in the interface, each argument to the Interface
       function, and each variable in the Interface Specific section of the
       Declarations Editor, there is an entry in the context structure.
       and a #define.  The #define makes the variable name refer to the
       corresponding entry in the context structure.
*******************************************************************************/

typedef struct {
    Widget UxPrintMessage;
    swidget UxUxParent;
}   _UxCPrintMessage;

#ifdef CONTEXT_MACRO_ACCESS
static _UxCPrintMessage *UxPrintMessageContext;
#define PrintMessage            UxPrintMessageContext->UxPrintMessage
#define UxParent                UxPrintMessageContext->UxUxParent

#endif				/* CONTEXT_MACRO_ACCESS */


/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget create_PrintMessage(swidget _UxUxParent);

#endif				/* _PRINTMESSAGE_INCLUDED */
