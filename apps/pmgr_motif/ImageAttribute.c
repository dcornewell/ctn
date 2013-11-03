
/*******************************************************************************
	ImageAttribute.c

       Associated Header file: ImageAttribute.h
*******************************************************************************/

#include "ctn_os.h"
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
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/

/*
          Copyright (C) 1993, 1994, RSNA and Washington University

          The software and supporting documentation for the Radiological
          Society of North America (RSNA) 1993, 1994 Digital Imaging and
          Communications in Medicine (DICOM) Demonstration were developed
          at the
                  Electronic Radiology Laboratory
                  Mallinckrodt Institute of Radiology
                  Washington University School of Medicine
                  510 S. Kingshighway Blvd.
                  St. Louis, MO 63110
          as part of the 1993, 1994 DICOM Central Test Node project for, and
          under contract with, the Radiological Society of North America.

          THIS SOFTWARE IS MADE AVAILABLE, AS IS, AND NEITHER RSNA NOR
          WASHINGTON UNIVERSITY MAKE ANY WARRANTY ABOUT THE SOFTWARE, ITS
          PERFORMANCE, ITS MERCHANTABILITY OR FITNESS FOR ANY PARTICULAR
          USE, FREEDOM FROM ANY COMPUTER DISEASES OR ITS CONFORMITY TO ANY
          SPECIFICATION. THE ENTIRE RISK AS TO QUALITY AND PERFORMANCE OF
          THE SOFTWARE IS WITH THE USER.

          Copyright of the software and supporting documentation is
          jointly owned by RSNA and Washington University, and free access
          is hereby granted as a license to use this software, copy this
          software and prepare derivative works based upon this software.
          However, any distribution of this software source code or
          supporting documentation or derivative works (source code and
          supporting documentation) must include the three paragraphs of
          the copyright notice.
*/
/* Copyright marker.  Copyright will be inserted above.  Do not remove */
/*
**                   Electronic Radiology Laboratory
**                 Mallinckrodt Institute of Radiology
**              Washington University School of Medicine
**
** Module Name(s):	create_ImageAttribute
**			UpdateEntries_in_ImageAttWindow
** Author, Date:	Chander Sabharwal
** Intent:		This module contains the widget that allows the
**			user to modify the attributes of a single image
**			on the print "film".  The user can modify
**			polarity and the algorithm for magnification.
** Last Update:		$Author: smm $, $Date: 2002-10-31 23:36:05 $
** Source File:		$RCSfile: ImageAttribute.c,v $
** Revision:		$Revision: 1.7 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.7 $ $RCSfile: ImageAttribute.c,v $";


#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "tbl.h"
#include "dicom_uids.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_print.h"
#include "manage.h"


#include "print_shared.h"
#include "print_client_app.h"

extern Widget wMessageTxt;

void UpdateEntries_in_ImageAttWindow();

extern char
    imagePositionC[64];

extern PRN_BASICIMAGEBOX
    bibAttrib;

extern int
    selected_bib;

typedef struct {
    Widget button;
    Pixmap pixmap;
    XImage *image;
    char UID[DICOM_UI_LENGTH + 1];
    PRN_BASICIMAGEBOX bib;
}   BASICIMAGEBOX;


BASICIMAGEBOX
bib[8][8];


/*******************************************************************************
       The following header file defines the context structure.
*******************************************************************************/

#define CONTEXT_MACRO_ACCESS 1
#include "ImageAttribute.h"
#undef CONTEXT_MACRO_ACCESS


/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

void
UpdateEntries_in_ImageAttWindow()
{
    int imgPos;
    CONDITION cond;
    CTNBOOLEAN nullFlag;
    char bufchar[64];

    printf(" in Update\n");

    printf("bib.imgP = %d\n", bibAttrib.imagePosition);

    sprintf(bufchar, "%d", bibAttrib.imagePosition);
    XmTextFieldSetString(wImagePositionTxt, bufchar);

    printf("bib.Pol = %s,%s\n", bibAttrib.polarity,
	   bib[selected_bib / 8][selected_bib % 8].bib.polarity);

    if (strcmp(bibAttrib.polarity, "NORMAL") == 0)
	XmToggleButtonGadgetSetState(wNormalTBG, TRUE, TRUE);
    else if (strcmp(bibAttrib.polarity, "REVERSE") == 0)
	XmToggleButtonGadgetSetState(wReverseTBG, TRUE, TRUE);


    printf("bib.mag = %s,%s\n", bibAttrib.magnificationType,
	   bib[selected_bib / 8][selected_bib % 8].bib.magnificationType);

    if (strcmp(bibAttrib.magnificationType, "REPLICATE") == 0)
	XmToggleButtonGadgetSetState(wReplicateTBG, TRUE, TRUE);
    else if (strcmp(bibAttrib.magnificationType, "BILINEAR") == 0)
	XmToggleButtonGadgetSetState(wBilinearTBG, TRUE, TRUE);
    else if (strcmp(bibAttrib.magnificationType, "CUBIC") == 0)
	XmToggleButtonGadgetSetState(wCubicTBG, TRUE, TRUE);
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget
_Uxbuild_ImageAttribute()
{
    Widget _UxParent;


    /* Creation of ImageAttribute */
    _UxParent = UxParent;
    if (_UxParent == NULL) {
	_UxParent = UxTopLevel;
    }
    _UxParent = XtVaCreatePopupShell("ImageAttribute_shell",
				     xmDialogShellWidgetClass, _UxParent,
				     XmNx, 510,
				     XmNy, 397,
				     XmNwidth, 270,
				     XmNheight, 313,
				     XmNshellUnitType, XmPIXELS,
				     XmNtitle, "ImageAttribute",
				     NULL);

    ImageAttribute = XtVaCreateWidget("ImageAttribute",
				      xmFormWidgetClass,
				      _UxParent,
				      XmNwidth, 270,
				      XmNheight, 313,
				      XmNunitType, XmPIXELS,
				      NULL);
    UxPutContext(ImageAttribute, (char *) UxImageAttributeContext);


    /* Creation of labelGadget2 */
    labelGadget2 = XtVaCreateManagedWidget("labelGadget2",
					   xmLabelGadgetClass,
					   ImageAttribute,
					   XmNx, 0,
					   XmNy, 0,
					   XmNwidth, 110,
					   XmNheight, 30,
			     RES_CONVERT(XmNlabelString, "Image Position:"),
					   NULL);
    UxPutContext(labelGadget2, (char *) UxImageAttributeContext);


    /* Creation of wImagePositionTxt */
    wImagePositionTxt = XtVaCreateManagedWidget("wImagePositionTxt",
						xmTextFieldWidgetClass,
						ImageAttribute,
						XmNwidth, 230,
						XmNx, 10,
						XmNy, 30,
						XmNheight, 30,
						XmNvalue, "1",
						NULL);
    UxPutContext(wImagePositionTxt, (char *) UxImageAttributeContext);


    /* Creation of labelGadget3 */
    labelGadget3 = XtVaCreateManagedWidget("labelGadget3",
					   xmLabelGadgetClass,
					   ImageAttribute,
					   XmNx, 0,
					   XmNy, 60,
					   XmNwidth, 90,
					   XmNheight, 30,
				   RES_CONVERT(XmNlabelString, "Polarity:"),
					   NULL);
    UxPutContext(labelGadget3, (char *) UxImageAttributeContext);


    /* Creation of rowColumn9 */
    rowColumn9 = XtVaCreateManagedWidget("rowColumn9",
					 xmRowColumnWidgetClass,
					 ImageAttribute,
					 XmNwidth, 80,
					 XmNheight, 80,
					 XmNx, 10,
					 XmNy, 90,
					 XmNradioBehavior, TRUE,
					 NULL);
    UxPutContext(rowColumn9, (char *) UxImageAttributeContext);


    /* Creation of wNormalTBG */
    wNormalTBG = XtVaCreateManagedWidget("wNormalTBG",
					 xmToggleButtonGadgetClass,
					 rowColumn9,
					 XmNx, 0,
					 XmNy, 0,
					 XmNwidth, 80,
					 XmNheight, 30,
				      RES_CONVERT(XmNlabelString, "Normal"),
					 XmNset, TRUE,
					 NULL);
    UxPutContext(wNormalTBG, (char *) UxImageAttributeContext);


    /* Creation of wReverseTBG */
    wReverseTBG = XtVaCreateManagedWidget("wReverseTBG",
					  xmToggleButtonGadgetClass,
					  rowColumn9,
					  XmNx, 13,
					  XmNy, 13,
					  XmNwidth, 80,
					  XmNheight, 30,
				     RES_CONVERT(XmNlabelString, "Reverse"),
					  NULL);
    UxPutContext(wReverseTBG, (char *) UxImageAttributeContext);


    /* Creation of labelGadget4 */
    labelGadget4 = XtVaCreateManagedWidget("labelGadget4",
					   xmLabelGadgetClass,
					   ImageAttribute,
					   XmNx, 0,
					   XmNy, 160,
					   XmNwidth, 140,
					   XmNheight, 30,
			 RES_CONVERT(XmNlabelString, "Magnification Type:"),
					   NULL);
    UxPutContext(labelGadget4, (char *) UxImageAttributeContext);


    /* Creation of rowColumn10 */
    rowColumn10 = XtVaCreateManagedWidget("rowColumn10",
					  xmRowColumnWidgetClass,
					  ImageAttribute,
					  XmNwidth, 130,
					  XmNheight, 90,
					  XmNx, 10,
					  XmNy, 200,
					  XmNradioBehavior, TRUE,
					  NULL);
    UxPutContext(rowColumn10, (char *) UxImageAttributeContext);


    /* Creation of wReplicateTBG */
    wReplicateTBG = XtVaCreateManagedWidget("wReplicateTBG",
					    xmToggleButtonGadgetClass,
					    rowColumn10,
					    XmNx, 0,
					    XmNy, 0,
					    XmNwidth, 130,
					    XmNheight, 30,
				   RES_CONVERT(XmNlabelString, "Replicate"),
					    XmNset, TRUE,
					    NULL);
    UxPutContext(wReplicateTBG, (char *) UxImageAttributeContext);


    /* Creation of wBilinearTBG */
    wBilinearTBG = XtVaCreateManagedWidget("wBilinearTBG",
					   xmToggleButtonGadgetClass,
					   rowColumn10,
					   XmNx, 13,
					   XmNy, 13,
					   XmNwidth, 130,
					   XmNheight, 30,
				    RES_CONVERT(XmNlabelString, "Bilinear"),
					   NULL);
    UxPutContext(wBilinearTBG, (char *) UxImageAttributeContext);


    /* Creation of wCubicTBG */
    wCubicTBG = XtVaCreateManagedWidget("wCubicTBG",
					xmToggleButtonGadgetClass,
					rowColumn10,
					XmNx, 13,
					XmNy, 42,
					XmNwidth, 130,
					XmNheight, 30,
					RES_CONVERT(XmNlabelString, "Cubic"),
					NULL);
    UxPutContext(wCubicTBG, (char *) UxImageAttributeContext);


    XtAddCallback(ImageAttribute, XmNdestroyCallback,
		  (XtCallbackProc) UxDestroyContextCB,
		  (XtPointer) UxImageAttributeContext);


    return (ImageAttribute);
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget
create_ImageAttribute(swidget _UxUxParent)
{
    Widget rtrn;
    _UxCImageAttribute *UxContext;

    UxImageAttributeContext = UxContext =
	(_UxCImageAttribute *) UxNewContext(sizeof(_UxCImageAttribute), False);

    UxParent = _UxUxParent;

    rtrn = _Uxbuild_ImageAttribute();

    return (rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/
