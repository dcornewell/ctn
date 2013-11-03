
/*******************************************************************************
       applicationForm.h
       This header file is included by applicationForm.c

*******************************************************************************/

#ifndef	_APPLICATIONFORM_INCLUDED
#define	_APPLICATIONFORM_INCLUDED


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
    Widget UxapplicationForm;
    Widget Uxlabel1;
    Widget Uxlabel2;
    Widget UxwApplicationTitleTxt;
    Widget UxwNodeTxt;
    Widget UxpushButton1;
    Widget UxpushButton2;
    Widget UxpushButton3;
    Widget Uxframe1;
    Widget UxscrolledWindowList1;
    Widget UxwApplicationList;
    Widget Uxlabel29;
    Widget Uxlabel3;
    Widget Uxlabel4;
    Widget UxwPortTxt;
    Widget UxwCommentTxt;
    Widget UxwOrganizationTxt;
    swidget UxUxParent;
}   _UxCapplicationForm;

#ifdef CONTEXT_MACRO_ACCESS
static _UxCapplicationForm *UxApplicationFormContext;
#define applicationForm         UxApplicationFormContext->UxapplicationForm
#define label1                  UxApplicationFormContext->Uxlabel1
#define label2                  UxApplicationFormContext->Uxlabel2
#define wApplicationTitleTxt    UxApplicationFormContext->UxwApplicationTitleTxt
#define wNodeTxt                UxApplicationFormContext->UxwNodeTxt
#define pushButton1             UxApplicationFormContext->UxpushButton1
#define pushButton2             UxApplicationFormContext->UxpushButton2
#define pushButton3             UxApplicationFormContext->UxpushButton3
#define frame1                  UxApplicationFormContext->Uxframe1
#define scrolledWindowList1     UxApplicationFormContext->UxscrolledWindowList1
#define wApplicationList        UxApplicationFormContext->UxwApplicationList
#define label29                 UxApplicationFormContext->Uxlabel29
#define label3                  UxApplicationFormContext->Uxlabel3
#define label4                  UxApplicationFormContext->Uxlabel4
#define wPortTxt                UxApplicationFormContext->UxwPortTxt
#define wCommentTxt             UxApplicationFormContext->UxwCommentTxt
#define wOrganizationTxt        UxApplicationFormContext->UxwOrganizationTxt
#define UxParent                UxApplicationFormContext->UxUxParent

#endif				/* CONTEXT_MACRO_ACCESS */


/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget create_applicationForm(swidget _UxUxParent);

#endif				/* _APPLICATIONFORM_INCLUDED */
