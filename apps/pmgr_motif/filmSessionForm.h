
/*******************************************************************************
       filmSessionForm.h
       This header file is included by filmSessionForm.c

*******************************************************************************/

#ifndef	_FILMSESSIONFORM_INCLUDED
#define	_FILMSESSIONFORM_INCLUDED


#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/Text.h>
#include <Xm/ScrolledW.h>
#include <Xm/PushB.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>
#include <Xm/RowColumn.h>
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
    Widget UxfilmSessionForm;
    Widget Uxlabel10;
    Widget UxrowColumn1;
    Widget UxwCopiesOneToggle;
    Widget UxwCopiesTwoToggle;
    Widget Uxlabel11;
    Widget Uxlabel12;
    Widget UxrowColumn2;
    Widget UxwPriorityLowToggle;
    Widget UxwPriorityMediumToggle;
    Widget UxwPriorityHighToggle;
    Widget Uxlabel13;
    Widget UxrowColumn3;
    Widget UxwMediumPaperToggle;
    Widget UxwMediumClearFilmToggle;
    Widget UxwMediumBlueFilmToggle;
    Widget Uxlabel14;
    Widget UxrowColumn4;
    Widget UxwDestinationMagazineToggle;
    Widget UxwDestinationProcessorToggle;
    Widget Uxlabel15;
    Widget UxwNumberOfFilmBoxesTxt;
    Widget UxpushButton2;
    Widget UxpushButton3;
    Widget UxwFilmSessionLabelTxt;
    Widget UxwMemoryAllocationTxt;
    Widget Uxlabel26;
    Widget Uxlabel27;
    Widget UxscrolledWindowText2;
    Widget UxscrolledWindowText4;
    Widget UxwErrorMesscrolledText1;
    swidget UxUxParent;
}   _UxCfilmSessionForm;

#ifdef CONTEXT_MACRO_ACCESS
static _UxCfilmSessionForm *UxFilmSessionFormContext;
#define filmSessionForm         UxFilmSessionFormContext->UxfilmSessionForm
#define label10                 UxFilmSessionFormContext->Uxlabel10
#define rowColumn1              UxFilmSessionFormContext->UxrowColumn1
#define wCopiesOneToggle        UxFilmSessionFormContext->UxwCopiesOneToggle
#define wCopiesTwoToggle        UxFilmSessionFormContext->UxwCopiesTwoToggle
#define label11                 UxFilmSessionFormContext->Uxlabel11
#define label12                 UxFilmSessionFormContext->Uxlabel12
#define rowColumn2              UxFilmSessionFormContext->UxrowColumn2
#define wPriorityLowToggle      UxFilmSessionFormContext->UxwPriorityLowToggle
#define wPriorityMediumToggle   UxFilmSessionFormContext->UxwPriorityMediumToggle
#define wPriorityHighToggle     UxFilmSessionFormContext->UxwPriorityHighToggle
#define label13                 UxFilmSessionFormContext->Uxlabel13
#define rowColumn3              UxFilmSessionFormContext->UxrowColumn3
#define wMediumPaperToggle      UxFilmSessionFormContext->UxwMediumPaperToggle
#define wMediumClearFilmToggle  UxFilmSessionFormContext->UxwMediumClearFilmToggle
#define wMediumBlueFilmToggle   UxFilmSessionFormContext->UxwMediumBlueFilmToggle
#define label14                 UxFilmSessionFormContext->Uxlabel14
#define rowColumn4              UxFilmSessionFormContext->UxrowColumn4
#define wDestinationMagazineToggle UxFilmSessionFormContext->UxwDestinationMagazineToggle
#define wDestinationProcessorToggle UxFilmSessionFormContext->UxwDestinationProcessorToggle
#define label15                 UxFilmSessionFormContext->Uxlabel15
#define wNumberOfFilmBoxesTxt   UxFilmSessionFormContext->UxwNumberOfFilmBoxesTxt
#define pushButton2             UxFilmSessionFormContext->UxpushButton2
#define pushButton3             UxFilmSessionFormContext->UxpushButton3
#define wFilmSessionLabelTxt    UxFilmSessionFormContext->UxwFilmSessionLabelTxt
#define wMemoryAllocationTxt    UxFilmSessionFormContext->UxwMemoryAllocationTxt
#define label26                 UxFilmSessionFormContext->Uxlabel26
#define label27                 UxFilmSessionFormContext->Uxlabel27
#define scrolledWindowText2     UxFilmSessionFormContext->UxscrolledWindowText2
#define scrolledWindowText4     UxFilmSessionFormContext->UxscrolledWindowText4
#define wErrorMesscrolledText1  UxFilmSessionFormContext->UxwErrorMesscrolledText1
#define UxParent                UxFilmSessionFormContext->UxUxParent

#endif				/* CONTEXT_MACRO_ACCESS */


/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget create_filmSessionForm(swidget _UxUxParent);

#endif				/* _FILMSESSIONFORM_INCLUDED */
