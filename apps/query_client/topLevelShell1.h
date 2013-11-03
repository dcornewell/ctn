
/*******************************************************************************
       topLevelShell1.h
       This header file is included by topLevelShell1.c

*******************************************************************************/

#ifndef	_TOPLEVELSHELL1_INCLUDED
#define	_TOPLEVELSHELL1_INCLUDED


#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/CascadeB.h>
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>
#include <Xm/TextF.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/DrawingA.h>
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
    Widget UxtopLevelShell1;
    Widget UxmainWindow1;
    Widget UxdrawingArea1;
    Widget UxscrolledWindowList2;
    Widget UxstudyListWidget;
    Widget UxscrolledWindowList1;
    Widget UxpatientListWidget;
    Widget Uxframe3;
    Widget Uxform3;
    Widget Uxlabel11;
    Widget UxdestinationAPTitleTxt;
    Widget UxmoveButton;
    Widget Uxframe1;
    Widget Uxform2;
    Widget Uxlabel6;
    Widget Uxlabel7;
    Widget Uxlabel8;
    Widget UxpatientIDTxt;
    Widget UxlastNameTxt;
    Widget UxbirthDateTxt;
    Widget UxpushButton1;
    Widget Uxframe2;
    Widget Uxform1;
    Widget Uxlabel2;
    Widget Uxlabel3;
    Widget Uxlabel4;
    Widget Uxlabel5;
    Widget UxcallingAPTitleTxt;
    Widget UxcalledAPTitleTxt;
    Widget UximageServerPortTxt;
    Widget UximageServerNodeTxt;
    Widget Uxlabel1;
    Widget Uxmenu1;
    Widget Uxmenu1_p1;
    Widget Uxmenu1_p1_b1;
    Widget Uxmenu1_top_b1;
    swidget UxUxParent;
}   _UxCtopLevelShell1;

#ifdef CONTEXT_MACRO_ACCESS
static _UxCtopLevelShell1 *UxTopLevelShell1Context;
#define topLevelShell1          UxTopLevelShell1Context->UxtopLevelShell1
#define mainWindow1             UxTopLevelShell1Context->UxmainWindow1
#define drawingArea1            UxTopLevelShell1Context->UxdrawingArea1
#define scrolledWindowList2     UxTopLevelShell1Context->UxscrolledWindowList2
#define studyListWidget         UxTopLevelShell1Context->UxstudyListWidget
#define scrolledWindowList1     UxTopLevelShell1Context->UxscrolledWindowList1
#define patientListWidget       UxTopLevelShell1Context->UxpatientListWidget
#define frame3                  UxTopLevelShell1Context->Uxframe3
#define form3                   UxTopLevelShell1Context->Uxform3
#define label11                 UxTopLevelShell1Context->Uxlabel11
#define destinationAPTitleTxt   UxTopLevelShell1Context->UxdestinationAPTitleTxt
#define moveButton              UxTopLevelShell1Context->UxmoveButton
#define frame1                  UxTopLevelShell1Context->Uxframe1
#define form2                   UxTopLevelShell1Context->Uxform2
#define label6                  UxTopLevelShell1Context->Uxlabel6
#define label7                  UxTopLevelShell1Context->Uxlabel7
#define label8                  UxTopLevelShell1Context->Uxlabel8
#define patientIDTxt            UxTopLevelShell1Context->UxpatientIDTxt
#define lastNameTxt             UxTopLevelShell1Context->UxlastNameTxt
#define birthDateTxt            UxTopLevelShell1Context->UxbirthDateTxt
#define pushButton1             UxTopLevelShell1Context->UxpushButton1
#define frame2                  UxTopLevelShell1Context->Uxframe2
#define form1                   UxTopLevelShell1Context->Uxform1
#define label2                  UxTopLevelShell1Context->Uxlabel2
#define label3                  UxTopLevelShell1Context->Uxlabel3
#define label4                  UxTopLevelShell1Context->Uxlabel4
#define label5                  UxTopLevelShell1Context->Uxlabel5
#define callingAPTitleTxt       UxTopLevelShell1Context->UxcallingAPTitleTxt
#define calledAPTitleTxt        UxTopLevelShell1Context->UxcalledAPTitleTxt
#define imageServerPortTxt      UxTopLevelShell1Context->UximageServerPortTxt
#define imageServerNodeTxt      UxTopLevelShell1Context->UximageServerNodeTxt
#define label1                  UxTopLevelShell1Context->Uxlabel1
#define menu1                   UxTopLevelShell1Context->Uxmenu1
#define menu1_p1                UxTopLevelShell1Context->Uxmenu1_p1
#define menu1_p1_b1             UxTopLevelShell1Context->Uxmenu1_p1_b1
#define menu1_top_b1            UxTopLevelShell1Context->Uxmenu1_top_b1
#define UxParent                UxTopLevelShell1Context->UxUxParent

#endif				/* CONTEXT_MACRO_ACCESS */


/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget create_topLevelShell1(swidget _UxUxParent);

#endif				/* _TOPLEVELSHELL1_INCLUDED */
