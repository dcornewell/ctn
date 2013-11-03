
/*******************************************************************************
       printServerCFGForm.h
       This header file is included by printServerCFGForm.c

*******************************************************************************/

#ifndef	_PRINTSERVERCFGFORM_INCLUDED
#define	_PRINTSERVERCFGFORM_INCLUDED


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
    Widget UxprintServerCFGForm;
    Widget Uxlabel7;
    Widget Uxlabel8;
    Widget Uxlabel9;
    Widget UxwRequestingTxt;
    Widget UxwRespondingTxt;
    Widget UxwQueueIDTxt;
    Widget UxpushButton7;
    Widget UxpushButton8;
    Widget UxpushButton9;
    Widget Uxframe3;
    Widget UxscrolledWindowList3;
    Widget UxwPrintServerCFGList;
    swidget UxUxParent;
}   _UxCprintServerCFGForm;

#ifdef CONTEXT_MACRO_ACCESS
static _UxCprintServerCFGForm *UxPrintServerCFGFormContext;
#define printServerCFGForm      UxPrintServerCFGFormContext->UxprintServerCFGForm
#define label7                  UxPrintServerCFGFormContext->Uxlabel7
#define label8                  UxPrintServerCFGFormContext->Uxlabel8
#define label9                  UxPrintServerCFGFormContext->Uxlabel9
#define wRequestingTxt          UxPrintServerCFGFormContext->UxwRequestingTxt
#define wRespondingTxt          UxPrintServerCFGFormContext->UxwRespondingTxt
#define wQueueIDTxt             UxPrintServerCFGFormContext->UxwQueueIDTxt
#define pushButton7             UxPrintServerCFGFormContext->UxpushButton7
#define pushButton8             UxPrintServerCFGFormContext->UxpushButton8
#define pushButton9             UxPrintServerCFGFormContext->UxpushButton9
#define frame3                  UxPrintServerCFGFormContext->Uxframe3
#define scrolledWindowList3     UxPrintServerCFGFormContext->UxscrolledWindowList3
#define wPrintServerCFGList     UxPrintServerCFGFormContext->UxwPrintServerCFGList
#define UxParent                UxPrintServerCFGFormContext->UxUxParent

#endif				/* CONTEXT_MACRO_ACCESS */


/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget create_printServerCFGForm(swidget _UxUxParent);

#endif				/* _PRINTSERVERCFGFORM_INCLUDED */
