
/*******************************************************************************
       ImageAttribute.h
       This header file is included by ImageAttribute.c

*******************************************************************************/

#ifndef	_IMAGEATTRIBUTE_INCLUDED
#define	_IMAGEATTRIBUTE_INCLUDED


#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/ToggleBG.h>
#include <Xm/RowColumn.h>
#include <Xm/TextF.h>
#include <Xm/LabelG.h>
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
    Widget UxImageAttribute;
    Widget UxlabelGadget2;
    Widget UxwImagePositionTxt;
    Widget UxlabelGadget3;
    Widget UxrowColumn9;
    Widget UxwNormalTBG;
    Widget UxwReverseTBG;
    Widget UxlabelGadget4;
    Widget UxrowColumn10;
    Widget UxwReplicateTBG;
    Widget UxwBilinearTBG;
    Widget UxwCubicTBG;
    swidget UxUxParent;
}   _UxCImageAttribute;

#ifdef CONTEXT_MACRO_ACCESS
static _UxCImageAttribute *UxImageAttributeContext;
#define ImageAttribute          UxImageAttributeContext->UxImageAttribute
#define labelGadget2            UxImageAttributeContext->UxlabelGadget2
#define wImagePositionTxt       UxImageAttributeContext->UxwImagePositionTxt
#define labelGadget3            UxImageAttributeContext->UxlabelGadget3
#define rowColumn9              UxImageAttributeContext->UxrowColumn9
#define wNormalTBG              UxImageAttributeContext->UxwNormalTBG
#define wReverseTBG             UxImageAttributeContext->UxwReverseTBG
#define labelGadget4            UxImageAttributeContext->UxlabelGadget4
#define rowColumn10             UxImageAttributeContext->UxrowColumn10
#define wReplicateTBG           UxImageAttributeContext->UxwReplicateTBG
#define wBilinearTBG            UxImageAttributeContext->UxwBilinearTBG
#define wCubicTBG               UxImageAttributeContext->UxwCubicTBG
#define UxParent                UxImageAttributeContext->UxUxParent

#endif				/* CONTEXT_MACRO_ACCESS */


/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget create_ImageAttribute(swidget _UxUxParent);

#endif				/* _IMAGEATTRIBUTE_INCLUDED */
