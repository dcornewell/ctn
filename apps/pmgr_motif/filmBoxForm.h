
/*******************************************************************************
       filmBoxForm.h
       This header file is included by filmBoxForm.c

*******************************************************************************/

#ifndef	_FILMBOXFORM_INCLUDED
#define	_FILMBOXFORM_INCLUDED


#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/Text.h>
#include <Xm/ScrolledW.h>
#include <Xm/PushB.h>
#include <Xm/ToggleB.h>
#include <Xm/RowColumn.h>
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
    Widget UxfilmBoxForm;
    Widget Uxlabel16;
    Widget Uxlabel17;
    Widget UxwColumnsTxt;
    Widget UxwRowsTxt;
    Widget Uxlabel18;
    Widget UxrowColumn5;
    Widget UxwTrimYesToggle;
    Widget UxwTrimNoToggle;
    Widget Uxlabel19;
    Widget UxwFilmSizeMenu;
    Widget Uxw8inX10inButton;
    Widget Uxw10inX12inButton;
    Widget Uxw10inX14inButton;
    Widget Uxw11inX14inButton;
    Widget Uxw14inX14inButton;
    Widget Uxw14inX17inButton;
    Widget Uxw24cmX24cmButton;
    Widget Uxw24cmX30cmButton;
    Widget UxwFilmSizeOptMenu;
    Widget Uxlabel20;
    Widget UxwMagnificationMenu;
    Widget UxwReplicateButton;
    Widget UxwBilinearButton;
    Widget UxwCubicButton;
    Widget UxwMagnificationOptMenu;
    Widget Uxlabel21;
    Widget UxrowColumn6;
    Widget UxwEmptyImageBlackToggle;
    Widget UxwEmptyImageWhiteToggle;
    Widget UxwEmptyImageOtherToggle;
    Widget Uxlabel22;
    Widget UxrowColumn7;
    Widget UxwBorderBlackToggle;
    Widget UxwBorderWhiteToggle;
    Widget UxwBorderOtherToggle;
    Widget Uxlabel23;
    Widget Uxlabel24;
    Widget UxwMaxDensityTxt;
    Widget UxwAnnotationDispFormatTxt;
    Widget Uxlabel25;
    Widget UxrowColumn8;
    Widget UxwPortraitToggle;
    Widget UxwLandscapeToggle;
    Widget UxpushButton4;
    Widget UxpushButton5;
    Widget Uxlabel28;
    Widget UxwMinDensityTxt;
    Widget Uxlabel29;
    Widget UxwConfigurationInfoTxt;
    Widget UxwBorderOtherTxt;
    Widget UxwEmptyImageOtherTxt;
    Widget UxscrolledWindowText3;
    Widget UxwErrorMesscrolledText2;
    swidget UxUxParent;
}   _UxCfilmBoxForm;

#ifdef CONTEXT_MACRO_ACCESS
static _UxCfilmBoxForm *UxFilmBoxFormContext;
#define filmBoxForm             UxFilmBoxFormContext->UxfilmBoxForm
#define label16                 UxFilmBoxFormContext->Uxlabel16
#define label17                 UxFilmBoxFormContext->Uxlabel17
#define wColumnsTxt             UxFilmBoxFormContext->UxwColumnsTxt
#define wRowsTxt                UxFilmBoxFormContext->UxwRowsTxt
#define label18                 UxFilmBoxFormContext->Uxlabel18
#define rowColumn5              UxFilmBoxFormContext->UxrowColumn5
#define wTrimYesToggle          UxFilmBoxFormContext->UxwTrimYesToggle
#define wTrimNoToggle           UxFilmBoxFormContext->UxwTrimNoToggle
#define label19                 UxFilmBoxFormContext->Uxlabel19
#define wFilmSizeMenu           UxFilmBoxFormContext->UxwFilmSizeMenu
#define w8inX10inButton         UxFilmBoxFormContext->Uxw8inX10inButton
#define w10inX12inButton        UxFilmBoxFormContext->Uxw10inX12inButton
#define w10inX14inButton        UxFilmBoxFormContext->Uxw10inX14inButton
#define w11inX14inButton        UxFilmBoxFormContext->Uxw11inX14inButton
#define w14inX14inButton        UxFilmBoxFormContext->Uxw14inX14inButton
#define w14inX17inButton        UxFilmBoxFormContext->Uxw14inX17inButton
#define w24cmX24cmButton        UxFilmBoxFormContext->Uxw24cmX24cmButton
#define w24cmX30cmButton        UxFilmBoxFormContext->Uxw24cmX30cmButton
#define wFilmSizeOptMenu        UxFilmBoxFormContext->UxwFilmSizeOptMenu
#define label20                 UxFilmBoxFormContext->Uxlabel20
#define wMagnificationMenu      UxFilmBoxFormContext->UxwMagnificationMenu
#define wReplicateButton        UxFilmBoxFormContext->UxwReplicateButton
#define wBilinearButton         UxFilmBoxFormContext->UxwBilinearButton
#define wCubicButton            UxFilmBoxFormContext->UxwCubicButton
#define wMagnificationOptMenu   UxFilmBoxFormContext->UxwMagnificationOptMenu
#define label21                 UxFilmBoxFormContext->Uxlabel21
#define rowColumn6              UxFilmBoxFormContext->UxrowColumn6
#define wEmptyImageBlackToggle  UxFilmBoxFormContext->UxwEmptyImageBlackToggle
#define wEmptyImageWhiteToggle  UxFilmBoxFormContext->UxwEmptyImageWhiteToggle
#define wEmptyImageOtherToggle  UxFilmBoxFormContext->UxwEmptyImageOtherToggle
#define label22                 UxFilmBoxFormContext->Uxlabel22
#define rowColumn7              UxFilmBoxFormContext->UxrowColumn7
#define wBorderBlackToggle      UxFilmBoxFormContext->UxwBorderBlackToggle
#define wBorderWhiteToggle      UxFilmBoxFormContext->UxwBorderWhiteToggle
#define wBorderOtherToggle      UxFilmBoxFormContext->UxwBorderOtherToggle
#define label23                 UxFilmBoxFormContext->Uxlabel23
#define label24                 UxFilmBoxFormContext->Uxlabel24
#define wMaxDensityTxt          UxFilmBoxFormContext->UxwMaxDensityTxt
#define wAnnotationDispFormatTxt UxFilmBoxFormContext->UxwAnnotationDispFormatTxt
#define label25                 UxFilmBoxFormContext->Uxlabel25
#define rowColumn8              UxFilmBoxFormContext->UxrowColumn8
#define wPortraitToggle         UxFilmBoxFormContext->UxwPortraitToggle
#define wLandscapeToggle        UxFilmBoxFormContext->UxwLandscapeToggle
#define pushButton4             UxFilmBoxFormContext->UxpushButton4
#define pushButton5             UxFilmBoxFormContext->UxpushButton5
#define label28                 UxFilmBoxFormContext->Uxlabel28
#define wMinDensityTxt          UxFilmBoxFormContext->UxwMinDensityTxt
#define label29                 UxFilmBoxFormContext->Uxlabel29
#define wConfigurationInfoTxt   UxFilmBoxFormContext->UxwConfigurationInfoTxt
#define wBorderOtherTxt         UxFilmBoxFormContext->UxwBorderOtherTxt
#define wEmptyImageOtherTxt     UxFilmBoxFormContext->UxwEmptyImageOtherTxt
#define scrolledWindowText3     UxFilmBoxFormContext->UxscrolledWindowText3
#define wErrorMesscrolledText2  UxFilmBoxFormContext->UxwErrorMesscrolledText2
#define UxParent                UxFilmBoxFormContext->UxUxParent

#endif				/* CONTEXT_MACRO_ACCESS */


/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget create_filmBoxForm(swidget _UxUxParent);

#endif				/* _FILMBOXFORM_INCLUDED */
