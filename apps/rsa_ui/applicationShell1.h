
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

#include <Xm/List.h>
#include <Xm/Text.h>
#include <Xm/ScrolledW.h>
#include <Xm/Frame.h>
#include <Xm/TextF.h>
#include <Xm/Separator.h>
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
    Widget Uxmenu1_top_b1;
    Widget Uxform1;
    Widget UxwClearReportButton;
    Widget UxwUpdateReportButton;
    Widget UxpushButton3;
    Widget UxpushButton4;
    Widget UxpushButton5;
    Widget Uxseparator1;
    Widget UxwPatientStudyTxt;
    Widget Uxframe1;
    Widget UxscrolledWindowText1;
    Widget UxwReportTxt;
    Widget Uxframe2;
    Widget UxscrolledWindowList1;
    Widget UxwPatientStudyList;
    Widget Uxframe3;
    Widget UxscrolledWindowText2;
    Widget UxwImpressionsTxt;
    Widget UxwClearImpressionsButton;
    Widget UxwUpdateImpressionsButton;
    Widget Uxframe4;
    Widget UxscrolledWindowText3;
    Widget Uxtext1;
    swidget UxUxParent;
}   _UxCapplicationShell1;

#ifdef CONTEXT_MACRO_ACCESS
static _UxCapplicationShell1 *UxApplicationShell1Context;
#define applicationShell1       UxApplicationShell1Context->UxapplicationShell1
#define mainWindow1             UxApplicationShell1Context->UxmainWindow1
#define menu1                   UxApplicationShell1Context->Uxmenu1
#define menu1_p1                UxApplicationShell1Context->Uxmenu1_p1
#define menu1_p1_b1             UxApplicationShell1Context->Uxmenu1_p1_b1
#define menu1_top_b1            UxApplicationShell1Context->Uxmenu1_top_b1
#define form1                   UxApplicationShell1Context->Uxform1
#define wClearReportButton      UxApplicationShell1Context->UxwClearReportButton
#define wUpdateReportButton     UxApplicationShell1Context->UxwUpdateReportButton
#define pushButton3             UxApplicationShell1Context->UxpushButton3
#define pushButton4             UxApplicationShell1Context->UxpushButton4
#define pushButton5             UxApplicationShell1Context->UxpushButton5
#define separator1              UxApplicationShell1Context->Uxseparator1
#define wPatientStudyTxt        UxApplicationShell1Context->UxwPatientStudyTxt
#define frame1                  UxApplicationShell1Context->Uxframe1
#define scrolledWindowText1     UxApplicationShell1Context->UxscrolledWindowText1
#define wReportTxt              UxApplicationShell1Context->UxwReportTxt
#define frame2                  UxApplicationShell1Context->Uxframe2
#define scrolledWindowList1     UxApplicationShell1Context->UxscrolledWindowList1
#define wPatientStudyList       UxApplicationShell1Context->UxwPatientStudyList
#define frame3                  UxApplicationShell1Context->Uxframe3
#define scrolledWindowText2     UxApplicationShell1Context->UxscrolledWindowText2
#define wImpressionsTxt         UxApplicationShell1Context->UxwImpressionsTxt
#define wClearImpressionsButton UxApplicationShell1Context->UxwClearImpressionsButton
#define wUpdateImpressionsButton UxApplicationShell1Context->UxwUpdateImpressionsButton
#define frame4                  UxApplicationShell1Context->Uxframe4
#define scrolledWindowText3     UxApplicationShell1Context->UxscrolledWindowText3
#define text1                   UxApplicationShell1Context->Uxtext1
#define UxParent                UxApplicationShell1Context->UxUxParent

#endif				/* CONTEXT_MACRO_ACCESS */


/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget create_applicationShell1(swidget _UxUxParent);

#endif				/* _APPLICATIONSHELL1_INCLUDED */
