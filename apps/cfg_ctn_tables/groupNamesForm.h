
/*******************************************************************************
       groupNamesForm.h
       This header file is included by groupNamesForm.c

*******************************************************************************/

#ifndef	_GROUPNAMESFORM_INCLUDED
#define	_GROUPNAMESFORM_INCLUDED


#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <X11/Shell.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/BulletinB.h>
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
    Widget UxgroupNamesForm;
    Widget Uxlabel10;
    Widget Uxlabel11;
    Widget UxwGroupTxt;
    Widget UxwApplicationTxt;
    Widget UxpushButton10;
    Widget UxpushButton11;
    Widget UxbulletinBoard1;
    Widget UxscrolledWindowList4;
    Widget UxwGroupGroupList;
    Widget UxscrolledWindowList5;
    Widget UxwGroupTitleList;
    swidget UxUxParent;
}   _UxCgroupNamesForm;

#ifdef CONTEXT_MACRO_ACCESS
static _UxCgroupNamesForm *UxGroupNamesFormContext;
#define groupNamesForm          UxGroupNamesFormContext->UxgroupNamesForm
#define label10                 UxGroupNamesFormContext->Uxlabel10
#define label11                 UxGroupNamesFormContext->Uxlabel11
#define wGroupTxt               UxGroupNamesFormContext->UxwGroupTxt
#define wApplicationTxt         UxGroupNamesFormContext->UxwApplicationTxt
#define pushButton10            UxGroupNamesFormContext->UxpushButton10
#define pushButton11            UxGroupNamesFormContext->UxpushButton11
#define bulletinBoard1          UxGroupNamesFormContext->UxbulletinBoard1
#define scrolledWindowList4     UxGroupNamesFormContext->UxscrolledWindowList4
#define wGroupGroupList         UxGroupNamesFormContext->UxwGroupGroupList
#define scrolledWindowList5     UxGroupNamesFormContext->UxscrolledWindowList5
#define wGroupTitleList         UxGroupNamesFormContext->UxwGroupTitleList
#define UxParent                UxGroupNamesFormContext->UxUxParent

#endif				/* CONTEXT_MACRO_ACCESS */


/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget create_groupNamesForm(swidget _UxUxParent);

#endif				/* _GROUPNAMESFORM_INCLUDED */
