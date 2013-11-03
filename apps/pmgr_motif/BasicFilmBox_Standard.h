
/*******************************************************************************
       BasicFilmBox_Standard.h
       This header file is included by BasicFilmBox_Standard.c

*******************************************************************************/

#ifndef	_BASICFILMBOX_STANDARD_INCLUDED
#define	_BASICFILMBOX_STANDARD_INCLUDED


#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/Form.h>

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
    Widget UxBasicFilmBox_Standard;
    swidget UxUxParent;
}   _UxCBasicFilmBox_Standard;

#ifdef CONTEXT_MACRO_ACCESS
static _UxCBasicFilmBox_Standard *UxBasicFilmBox_StandardContext;
#define BasicFilmBox_Standard   UxBasicFilmBox_StandardContext->UxBasicFilmBox_Standard
#define UxParent                UxBasicFilmBox_StandardContext->UxUxParent

#endif				/* CONTEXT_MACRO_ACCESS */


/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget create_BasicFilmBox_Standard(swidget _UxUxParent);

#endif				/* _BASICFILMBOX_STANDARD_INCLUDED */
