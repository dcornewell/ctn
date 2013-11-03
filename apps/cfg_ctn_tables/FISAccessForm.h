
/*******************************************************************************
       FISAccessForm.h
       This header file is included by FISAccessForm.c

*******************************************************************************/

#ifndef	_FISACCESSFORM_INCLUDED
#define	_FISACCESSFORM_INCLUDED


#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <X11/Shell.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/Frame.h>
#include <Xm/PushB.h>
#include <Xm/TextF.h>
#include <Xm/Label.h>
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
    Widget UxFISAccessForm;
    Widget Uxlabel12;
    Widget Uxlabel13;
    Widget Uxlabel14;
    Widget Uxlabel15;
    Widget UxwGroupTxt;
    Widget UxwCommentTxt;
    Widget UxwTitleTxt;
    Widget UxwDatabaseKeyTxt;
    Widget UxpushButton12;
    Widget UxpushButton13;
    Widget UxpushButton14;
    Widget Uxframe4;
    Widget UxscrolledWindowList6;
    Widget UxwFISAccessList;
    swidget UxUxParent;
}   _UxCFISAccessForm;

#ifdef CONTEXT_MACRO_ACCESS
static _UxCFISAccessForm *UxFISAccessFormContext;
#define FISAccessForm           UxFISAccessFormContext->UxFISAccessForm
#define label12                 UxFISAccessFormContext->Uxlabel12
#define label13                 UxFISAccessFormContext->Uxlabel13
#define label14                 UxFISAccessFormContext->Uxlabel14
#define label15                 UxFISAccessFormContext->Uxlabel15
#define wGroupTxt               UxFISAccessFormContext->UxwGroupTxt
#define wCommentTxt             UxFISAccessFormContext->UxwCommentTxt
#define wTitleTxt               UxFISAccessFormContext->UxwTitleTxt
#define wDatabaseKeyTxt         UxFISAccessFormContext->UxwDatabaseKeyTxt
#define pushButton12            UxFISAccessFormContext->UxpushButton12
#define pushButton13            UxFISAccessFormContext->UxpushButton13
#define pushButton14            UxFISAccessFormContext->UxpushButton14
#define frame4                  UxFISAccessFormContext->Uxframe4
#define scrolledWindowList6     UxFISAccessFormContext->UxscrolledWindowList6
#define wFISAccessList          UxFISAccessFormContext->UxwFISAccessList
#define UxParent                UxFISAccessFormContext->UxUxParent

#endif				/* CONTEXT_MACRO_ACCESS */


/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget create_FISAccessForm(swidget _UxUxParent);

#endif				/* _FISACCESSFORM_INCLUDED */
