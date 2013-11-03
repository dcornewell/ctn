
/*******************************************************************************
       studyListForm.h
       This header file is included by studyListForm.c

*******************************************************************************/

#ifndef	_STUDYLISTFORM_INCLUDED
#define	_STUDYLISTFORM_INCLUDED


#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/LabelG.h>
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
    Widget UxstudyListForm;
    Widget UxlabelGadget1;
    Widget UxscrolledWindowList1;
    Widget UxwStudyList;
    swidget UxUxParent;
}   _UxCstudyListForm;

#ifdef CONTEXT_MACRO_ACCESS
static _UxCstudyListForm *UxStudyListFormContext;
#define studyListForm           UxStudyListFormContext->UxstudyListForm
#define labelGadget1            UxStudyListFormContext->UxlabelGadget1
#define scrolledWindowList1     UxStudyListFormContext->UxscrolledWindowList1
#define wStudyList              UxStudyListFormContext->UxwStudyList
#define UxParent                UxStudyListFormContext->UxUxParent

#endif				/* CONTEXT_MACRO_ACCESS */


/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget create_studyListForm(swidget _UxUxParent);

#endif				/* _STUDYLISTFORM_INCLUDED */
