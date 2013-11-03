
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

#include <Xm/Text.h>
#include <Xm/ScrolledW.h>
#include <Xm/Frame.h>
#include <Xm/Form.h>
#include <Xm/CascadeB.h>
#include <Xm/PushB.h>
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
    Widget Uxmenu1_p1_b1;
    Widget Uxmenu1_p1_b2;
    Widget Uxmenu1_top_b1;
    Widget Uxform1;
    Widget Uxframe1;
    Widget UxscrolledWindowText1;
    Widget UxwMessageTxt;
    swidget UxUxParent;
}   _UxCapplicationShell1;

#ifdef CONTEXT_MACRO_ACCESS
static _UxCapplicationShell1 *UxApplicationShell1Context;
#define applicationShell1       UxApplicationShell1Context->UxapplicationShell1
#define mainWindow1             UxApplicationShell1Context->UxmainWindow1
#define menu1                   UxApplicationShell1Context->Uxmenu1
#define menu1_p1                UxApplicationShell1Context->Uxmenu1_p1
#define menu1_p1_b1             UxApplicationShell1Context->Uxmenu1_p1_b1
#define menu1_p1_b2             UxApplicationShell1Context->Uxmenu1_p1_b2
#define menu1_top_b1            UxApplicationShell1Context->Uxmenu1_top_b1
#define form1                   UxApplicationShell1Context->Uxform1
#define frame1                  UxApplicationShell1Context->Uxframe1
#define scrolledWindowText1     UxApplicationShell1Context->UxscrolledWindowText1
#define wMessageTxt             UxApplicationShell1Context->UxwMessageTxt
#define UxParent                UxApplicationShell1Context->UxUxParent

#endif				/* CONTEXT_MACRO_ACCESS */


/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget create_applicationShell1(swidget _UxUxParent);

#endif				/* _APPLICATIONSHELL1_INCLUDED */
