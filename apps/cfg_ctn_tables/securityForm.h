
/*******************************************************************************
       securityForm.h
       This header file is included by securityForm.c

*******************************************************************************/

#ifndef	_SECURITYFORM_INCLUDED
#define	_SECURITYFORM_INCLUDED


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
    Widget UxsecurityForm;
    Widget Uxlabel5;
    Widget Uxlabel6;
    Widget UxwRequestingApplicationTxt;
    Widget UxwRespondingApplicationTxt;
    Widget UxpushButton4;
    Widget UxpushButton5;
    Widget UxpushButton6;
    Widget Uxframe2;
    Widget UxscrolledWindowList2;
    Widget UxwSecurityMatrixList;
    swidget UxUxParent;
}   _UxCsecurityForm;

#ifdef CONTEXT_MACRO_ACCESS
static _UxCsecurityForm *UxSecurityFormContext;
#define securityForm            UxSecurityFormContext->UxsecurityForm
#define label5                  UxSecurityFormContext->Uxlabel5
#define label6                  UxSecurityFormContext->Uxlabel6
#define wRequestingApplicationTxt UxSecurityFormContext->UxwRequestingApplicationTxt
#define wRespondingApplicationTxt UxSecurityFormContext->UxwRespondingApplicationTxt
#define pushButton4             UxSecurityFormContext->UxpushButton4
#define pushButton5             UxSecurityFormContext->UxpushButton5
#define pushButton6             UxSecurityFormContext->UxpushButton6
#define frame2                  UxSecurityFormContext->Uxframe2
#define scrolledWindowList2     UxSecurityFormContext->UxscrolledWindowList2
#define wSecurityMatrixList     UxSecurityFormContext->UxwSecurityMatrixList
#define UxParent                UxSecurityFormContext->UxUxParent

#endif				/* CONTEXT_MACRO_ACCESS */


/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget create_securityForm(swidget _UxUxParent);

#endif				/* _SECURITYFORM_INCLUDED */
