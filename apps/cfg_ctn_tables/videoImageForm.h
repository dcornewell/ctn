
/*******************************************************************************
       videoImageForm.h
       This header file is included by videoImageForm.c

*******************************************************************************/

#ifndef	_VIDEOIMAGEFORM_INCLUDED
#define	_VIDEOIMAGEFORM_INCLUDED


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
    Widget UxvideoImageForm;
    Widget Uxframe6;
    Widget UxscrolledWindowList9;
    Widget UxwVideoImageList;
    Widget Uxlabel26;
    Widget Uxlabel27;
    Widget Uxlabel28;
    Widget UxwRequestingTitleTxt;
    Widget UxwRespondingTitleTxt;
    Widget UxwImageTypeTxt;
    Widget UxpushButton21;
    Widget UxpushButton22;
    swidget UxUxParent;
}   _UxCvideoImageForm;

#ifdef CONTEXT_MACRO_ACCESS
static _UxCvideoImageForm *UxVideoImageFormContext;
#define videoImageForm          UxVideoImageFormContext->UxvideoImageForm
#define frame6                  UxVideoImageFormContext->Uxframe6
#define scrolledWindowList9     UxVideoImageFormContext->UxscrolledWindowList9
#define wVideoImageList         UxVideoImageFormContext->UxwVideoImageList
#define label26                 UxVideoImageFormContext->Uxlabel26
#define label27                 UxVideoImageFormContext->Uxlabel27
#define label28                 UxVideoImageFormContext->Uxlabel28
#define wRequestingTitleTxt     UxVideoImageFormContext->UxwRequestingTitleTxt
#define wRespondingTitleTxt     UxVideoImageFormContext->UxwRespondingTitleTxt
#define wImageTypeTxt           UxVideoImageFormContext->UxwImageTypeTxt
#define pushButton21            UxVideoImageFormContext->UxpushButton21
#define pushButton22            UxVideoImageFormContext->UxpushButton22
#define UxParent                UxVideoImageFormContext->UxUxParent

#endif				/* CONTEXT_MACRO_ACCESS */


/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget create_videoImageForm(swidget _UxUxParent);

#endif				/* _VIDEOIMAGEFORM_INCLUDED */
