
/*******************************************************************************
       imageStorageAccessForm.h
       This header file is included by imageStorageAccessForm.c

*******************************************************************************/

#ifndef	_IMAGESTORAGEACCESSFORM_INCLUDED
#define	_IMAGESTORAGEACCESSFORM_INCLUDED


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
    Widget UximageStorageAccessForm;
    Widget Uxframe5;
    Widget UxscrolledWindowList7;
    Widget UxwStorageAccessList;
    Widget Uxlabel16;
    Widget Uxlabel17;
    Widget Uxlabel18;
    Widget Uxlabel19;
    Widget Uxlabel20;
    Widget Uxlabel21;
    Widget UxwApplicationTitleTxt;
    Widget UxwDatabaseKeyTxt;
    Widget UxwOwnerTxt;
    Widget UxwGroupNameTxt;
    Widget UxwCommentTxt;
    Widget UxpushButton15;
    Widget UxpushButton16;
    Widget UxpushButton17;
    swidget UxUxParent;
}   _UxCimageStorageAccessForm;

#ifdef CONTEXT_MACRO_ACCESS
static _UxCimageStorageAccessForm *UxImageStorageAccessFormContext;
#define imageStorageAccessForm  UxImageStorageAccessFormContext->UximageStorageAccessForm
#define frame5                  UxImageStorageAccessFormContext->Uxframe5
#define scrolledWindowList7     UxImageStorageAccessFormContext->UxscrolledWindowList7
#define wStorageAccessList      UxImageStorageAccessFormContext->UxwStorageAccessList
#define label16                 UxImageStorageAccessFormContext->Uxlabel16
#define label17                 UxImageStorageAccessFormContext->Uxlabel17
#define label18                 UxImageStorageAccessFormContext->Uxlabel18
#define label19                 UxImageStorageAccessFormContext->Uxlabel19
#define label20                 UxImageStorageAccessFormContext->Uxlabel20
#define label21                 UxImageStorageAccessFormContext->Uxlabel21
#define wApplicationTitleTxt    UxImageStorageAccessFormContext->UxwApplicationTitleTxt
#define wDatabaseKeyTxt         UxImageStorageAccessFormContext->UxwDatabaseKeyTxt
#define wOwnerTxt               UxImageStorageAccessFormContext->UxwOwnerTxt
#define wGroupNameTxt           UxImageStorageAccessFormContext->UxwGroupNameTxt
#define wCommentTxt             UxImageStorageAccessFormContext->UxwCommentTxt
#define pushButton15            UxImageStorageAccessFormContext->UxpushButton15
#define pushButton16            UxImageStorageAccessFormContext->UxpushButton16
#define pushButton17            UxImageStorageAccessFormContext->UxpushButton17
#define UxParent                UxImageStorageAccessFormContext->UxUxParent

#endif				/* CONTEXT_MACRO_ACCESS */


/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget create_imageStorageAccessForm(swidget _UxUxParent);

#endif				/* _IMAGESTORAGEACCESSFORM_INCLUDED */
