
/*******************************************************************************
       applicationShell1.h
       This header file is included by applicationShell1.c

*******************************************************************************/

#ifndef	_APPLICATIONSHELL1_INCLUDED
#define	_APPLICATIONSHELL1_INCLUDED


#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/BulletinB.h>
#include <Xm/PushB.h>
#include <Xm/CascadeB.h>
#include <Xm/RowColumn.h>
#include <Xm/MainW.h>
#include <X11/Shell.h>

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
    Widget UxapplicationShell1;
    Widget UxmainWindow1;
    Widget Uxmenu1;
    Widget Uxmenu1_p1;
    Widget Uxmenu1_p4;
    Widget Uxmenu1_p4_b2;
    Widget Uxmenu1_p4_b3;
    Widget Uxmenu1_p4_b1;
    Widget Uxmenu1_p1_b7;
    Widget Uxmenu1_p6;
    Widget Uxmenu1_p6_b1;
    Widget Uxmenu1_p6_b2;
    Widget Uxmenu1_p1_b5;
    Widget Uxmenu1_p2;
    Widget Uxmenu1_p2_b1;
    Widget Uxmenu1_p1_b9;
    Widget Uxmenu1_p5;
    Widget Uxmenu1_p5_b1;
    Widget Uxmenu1_p1_b6;
    Widget Uxmenu1_p7;
    Widget Uxmenu1_p7_b1;
    Widget Uxmenu1_p1_b8;
    Widget Uxmenu1_p1_b3;
    Widget Uxmenu1_top_b1;
    Widget UxbulletinBoard3;
    swidget UxUxParent;
}   _UxCapplicationShell1;

#ifdef CONTEXT_MACRO_ACCESS
static _UxCapplicationShell1 *UxApplicationShell1Context;
#define applicationShell1       UxApplicationShell1Context->UxapplicationShell1
#define mainWindow1             UxApplicationShell1Context->UxmainWindow1
#define menu1                   UxApplicationShell1Context->Uxmenu1
#define menu1_p1                UxApplicationShell1Context->Uxmenu1_p1
#define menu1_p4                UxApplicationShell1Context->Uxmenu1_p4
#define menu1_p4_b2             UxApplicationShell1Context->Uxmenu1_p4_b2
#define menu1_p4_b3             UxApplicationShell1Context->Uxmenu1_p4_b3
#define menu1_p4_b1             UxApplicationShell1Context->Uxmenu1_p4_b1
#define menu1_p1_b7             UxApplicationShell1Context->Uxmenu1_p1_b7
#define menu1_p6                UxApplicationShell1Context->Uxmenu1_p6
#define menu1_p6_b1             UxApplicationShell1Context->Uxmenu1_p6_b1
#define menu1_p6_b2             UxApplicationShell1Context->Uxmenu1_p6_b2
#define menu1_p1_b5             UxApplicationShell1Context->Uxmenu1_p1_b5
#define menu1_p2                UxApplicationShell1Context->Uxmenu1_p2
#define menu1_p2_b1             UxApplicationShell1Context->Uxmenu1_p2_b1
#define menu1_p1_b9             UxApplicationShell1Context->Uxmenu1_p1_b9
#define menu1_p5                UxApplicationShell1Context->Uxmenu1_p5
#define menu1_p5_b1             UxApplicationShell1Context->Uxmenu1_p5_b1
#define menu1_p1_b6             UxApplicationShell1Context->Uxmenu1_p1_b6
#define menu1_p7                UxApplicationShell1Context->Uxmenu1_p7
#define menu1_p7_b1             UxApplicationShell1Context->Uxmenu1_p7_b1
#define menu1_p1_b8             UxApplicationShell1Context->Uxmenu1_p1_b8
#define menu1_p1_b3             UxApplicationShell1Context->Uxmenu1_p1_b3
#define menu1_top_b1            UxApplicationShell1Context->Uxmenu1_top_b1
#define bulletinBoard3          UxApplicationShell1Context->UxbulletinBoard3
#define UxParent                UxApplicationShell1Context->UxUxParent

#endif				/* CONTEXT_MACRO_ACCESS */


/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget create_applicationShell1(swidget _UxUxParent);

#endif				/* _APPLICATIONSHELL1_INCLUDED */
