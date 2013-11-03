
/*******************************************************************************
       demonstrationForm.h
       This header file is included by demonstrationForm.c

*******************************************************************************/

#ifndef	_DEMONSTRATIONFORM_INCLUDED
#define	_DEMONSTRATIONFORM_INCLUDED


#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <X11/Shell.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/Frame.h>
#include <Xm/PushB.h>
#include <Xm/Label.h>
#include <Xm/TextF.h>
#include <Xm/Form.h>


/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget create_demonstrationForm(swidget _UxUxParent);

#endif				/* _DEMONSTRATIONFORM_INCLUDED */
