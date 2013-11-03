
/*******************************************************************************
       databaseForm.h
       This header file is included by databaseForm.c

*******************************************************************************/

#ifndef	_DATABASEFORM_INCLUDED
#define	_DATABASEFORM_INCLUDED


#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <X11/Shell.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/Text.h>
#include <Xm/ToggleB.h>
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>
#include <Xm/Text.h>
#include <Xm/Label.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/Frame.h>
#include <Xm/Form.h>


/*******************************************************************************
       Declarations of global functions.
*******************************************************************************/

Widget create_databaseForm(swidget _UxUxParent);

#endif				/* _DATABASEFORM_INCLUDED */
