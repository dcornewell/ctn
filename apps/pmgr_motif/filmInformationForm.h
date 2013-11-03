
/*******************************************************************************
       filmInformationForm.h
       This header file is included by filmInformationForm.c

*******************************************************************************/

#ifndef	_FILMINFORMATIONFORM_INCLUDED
#define	_FILMINFORMATIONFORM_INCLUDED


#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/PushB.h>
#include <Xm/TextF.h>
#include <Xm/Label.h>
#include <Xm/Form.h>
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
    Widget UxfilmInformationForm;
    Widget Uxform2;
    Widget Uxlabel1;
    Widget Uxlabel2;
    Widget Uxlabel3;
    Widget Uxlabel4;
    Widget UxwPrinterNameTxt;
    Widget UxwManufacturerTxt;
    Widget UxwModelNameTxt;
    Widget UxwSerialNumberTxt;
    Widget Uxlabel5;
    Widget Uxlabel6;
    Widget Uxlabel7;
    Widget Uxlabel8;
    Widget Uxlabel9;
    Widget UxwSoftwareVersionTxt;
    Widget UxwDateLastCalibrationTxt;
    Widget UxwTimeLastCalibrationTxt;
    Widget UxwPrinterStatusTxt;
    Widget UxwPrinterStatusInfoTxt;
    Widget UxpushButton1;
    swidget UxUxParent;
}   _UxCfilmInformationForm;

#ifdef CONTEXT_MACRO_ACCESS
static _UxCfilmInformationForm *UxFilmInformationFormContext;
#define filmInformationForm     UxFilmInformationFormContext->UxfilmInformationForm
#define form2                   UxFilmInformationFormContext->Uxform2
#define label1                  UxFilmInformationFormContext->Uxlabel1
#define label2                  UxFilmInformationFormContext->Uxlabel2
#define label3                  UxFilmInformationFormContext->Uxlabel3
#define label4                  UxFilmInformationFormContext->Uxlabel4
#define wPrinterNameTxt         UxFilmInformationFormContext->UxwPrinterNameTxt
#define wManufacturerTxt        UxFilmInformationFormContext->UxwManufacturerTxt
#define wModelNameTxt           UxFilmInformationFormContext->UxwModelNameTxt
#define wSerialNumberTxt        UxFilmInformationFormContext->UxwSerialNumberTxt
#define label5                  UxFilmInformationFormContext->Uxlabel5
#define label6                  UxFilmInformationFormContext->Uxlabel6
#define label7                  UxFilmInformationFormContext->Uxlabel7
#define label8                  UxFilmInformationFormContext->Uxlabel8
#define label9                  UxFilmInformationFormContext->Uxlabel9
#define wSoftwareVersionTxt     UxFilmInformationFormContext->UxwSoftwareVersionTxt
#define wDateLastCalibrationTxt UxFilmInformationFormContext->UxwDateLastCalibrationTxt
#define wTimeLastCalibrationTxt UxFilmInformationFormContext->UxwTimeLastCalibrationTxt
#define wPrinterStatusTxt       UxFilmInformationFormContext->UxwPrinterStatusTxt
#define wPrinterStatusInfoTxt   UxFilmInformationFormContext->UxwPrinterStatusInfoTxt
#define pushButton1             UxFilmInformationFormContext->UxpushButton1
#define UxParent                UxFilmInformationFormContext->UxUxParent

#endif				/* CONTEXT_MACRO_ACCESS */


/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget create_filmInformationForm(swidget _UxUxParent);

#endif				/* _FILMINFORMATIONFORM_INCLUDED */
