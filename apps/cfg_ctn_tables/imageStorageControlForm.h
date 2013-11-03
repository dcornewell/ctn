
/*******************************************************************************
       imageStorageControlForm.h
       This header file is included by imageStorageControlForm.c

*******************************************************************************/

#ifndef	_IMAGESTORAGECONTROLFORM_INCLUDED
#define	_IMAGESTORAGECONTROLFORM_INCLUDED


#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <X11/Shell.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/PushB.h>
#include <Xm/TextF.h>
#include <Xm/Label.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/Frame.h>
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
    Widget UximageStorageControlForm;
    Widget Uxframe7;
    Widget UxscrolledWindowList8;
    Widget UxwStorageControlList;
    Widget Uxlabel22;
    Widget UxwRequestingApplicationTxt;
    Widget Uxlabel23;
    Widget Uxlabel24;
    Widget Uxlabel25;
    Widget UxwRespondingApplicationTxt;
    Widget UxwMediumTxt;
    Widget UxwRootTxt;
    Widget UxpushButton18;
    Widget UxpushButton19;
    Widget UxpushButton20;
    swidget UxUxParent;
}   _UxCimageStorageControlForm;

#ifdef CONTEXT_MACRO_ACCESS
static _UxCimageStorageControlForm *UxImageStorageControlFormContext;
#define imageStorageControlForm UxImageStorageControlFormContext->UximageStorageControlForm
#define frame7                  UxImageStorageControlFormContext->Uxframe7
#define scrolledWindowList8     UxImageStorageControlFormContext->UxscrolledWindowList8
#define wStorageControlList     UxImageStorageControlFormContext->UxwStorageControlList
#define label22                 UxImageStorageControlFormContext->Uxlabel22
#define wRequestingApplicationTxt UxImageStorageControlFormContext->UxwRequestingApplicationTxt
#define label23                 UxImageStorageControlFormContext->Uxlabel23
#define label24                 UxImageStorageControlFormContext->Uxlabel24
#define label25                 UxImageStorageControlFormContext->Uxlabel25
#define wRespondingApplicationTxt UxImageStorageControlFormContext->UxwRespondingApplicationTxt
#define wMediumTxt              UxImageStorageControlFormContext->UxwMediumTxt
#define wRootTxt                UxImageStorageControlFormContext->UxwRootTxt
#define pushButton18            UxImageStorageControlFormContext->UxpushButton18
#define pushButton19            UxImageStorageControlFormContext->UxpushButton19
#define pushButton20            UxImageStorageControlFormContext->UxpushButton20
#define UxParent                UxImageStorageControlFormContext->UxUxParent

#endif				/* CONTEXT_MACRO_ACCESS */


/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget create_imageStorageControlForm(swidget _UxUxParent);

#endif				/* _IMAGESTORAGECONTROLFORM_INCLUDED */
