
/*******************************************************************************
	filmInformationForm.c

       Associated Header file: filmInformationForm.h
*******************************************************************************/

#include "ctn_os.h"
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
** Module Name(s):	create_filmInformationForm
** Author, Date:	Chander Sabharwal
** Intent:		This module contains the widgets which display
**			printer information, that is, information that
**			can be retrieved from DICOM printers.
** Last Update:		$Author: smm $, $Date: 2002-10-31 23:36:06 $
** Source File:		$RCSfile: filmInformationForm.c,v $
** Revision:		$Revision: 1.4 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.4 $ $RCSfile: filmInformationForm.c,v $";


#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "dicom_uids.h"
#include "dulprotocol.h"
#include "tbl.h"
#include "manage.h"
#include "dicom_objects.h"
#include "dicom_print.h"

#include "print_client_app.h"
#include "print_shared.h"

extern DUL_ASSOCIATIONKEY *associationKey;
extern DUL_ASSOCIATESERVICEPARAMETERS associationParameters;

static void displayPrinterInformation(PRN_PRINTER * attributes);


/*******************************************************************************
       The following header file defines the context structure.
*******************************************************************************/

#define CONTEXT_MACRO_ACCESS 1
#include "filmInformationForm.h"
#undef CONTEXT_MACRO_ACCESS


/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

static void
displayPrinterInformation(PRN_PRINTER * a)
{
    XmTextSetString(wPrinterNameTxt, "");
    XmTextSetString(wManufacturerTxt, "");
    XmTextSetString(wModelNameTxt, "");
    XmTextSetString(wSerialNumberTxt, "");
    XmTextSetString(wSoftwareVersionTxt, "");
    XmTextSetString(wDateLastCalibrationTxt, "");
    XmTextSetString(wTimeLastCalibrationTxt, "");
    XmTextSetString(wPrinterStatusTxt, "");
    XmTextSetString(wPrinterStatusInfoTxt, "");

    if (a == NULL)
	return;

    if (a->attributeFlag & PRN_PRINTER_K_STATUS)
	XmTextSetString(wPrinterStatusTxt, a->status);
    if (a->attributeFlag & PRN_PRINTER_K_STATUSINFO)
	XmTextSetString(wPrinterStatusInfoTxt, a->statusInfoCode);
    if (a->attributeFlag & PRN_PRINTER_K_NAME)
	XmTextSetString(wPrinterNameTxt, a->name);
    if (a->attributeFlag & PRN_PRINTER_K_MANUFACTURER)
	XmTextSetString(wManufacturerTxt, a->manufacturer);
    if (a->attributeFlag & PRN_PRINTER_K_MODELNUMBER)
	XmTextSetString(wModelNameTxt, a->manufacturerModelName);
    if (a->attributeFlag & PRN_PRINTER_K_SERIALNUMBER)
	XmTextSetString(wSerialNumberTxt, a->deviceSerialNumber);
    if (a->attributeFlag & PRN_PRINTER_K_SOFTWAREVERSION)
	XmTextSetString(wSoftwareVersionTxt, a->softwareVersion);
    if (a->attributeFlag & PRN_PRINTER_K_DATEOFCALIBRATION)
	XmTextSetString(wDateLastCalibrationTxt, a->dateOfLastCalibration);
    if (a->attributeFlag & PRN_PRINTER_K_TIMEOFCALIBRATION)
	XmTextSetString(wTimeLastCalibrationTxt, a->timeOfLastCalibration);
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void
activateCB_pushButton1(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    _UxCfilmInformationForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxFilmInformationFormContext;
    UxFilmInformationFormContext = UxContext =
	(_UxCfilmInformationForm *) UxGetContext(UxWidget);
    {
	CONDITION
	    cond;
	unsigned short
	    status;
	PRN_PRINTER *attributes = NULL;

	cond = getPrinterInstance(&associationKey, &associationParameters,
				  DICOM_SOPCLASSGREYSCALEPRINTMGMTMETA,
				  &attributes, &status);

	if (cond != APP_NORMAL) {
	    COND_DumpConditions();
	    if (attributes != NULL)
		PRN_FreeStructure(&attributes);
	}
	displayPrinterInformation(attributes);
	PRN_FreeStructure(&attributes);
    }
    UxFilmInformationFormContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget
_Uxbuild_filmInformationForm()
{
    Widget _UxParent;


    /* Creation of filmInformationForm */
    _UxParent = UxParent;
    if (_UxParent == NULL) {
	_UxParent = UxTopLevel;
    }
    _UxParent = XtVaCreatePopupShell("filmInformationForm_shell",
				     xmDialogShellWidgetClass, _UxParent,
				     XmNx, 17,
				     XmNy, 325,
				     XmNwidth, 646,
				     XmNheight, 533,
				     XmNshellUnitType, XmPIXELS,
				     XmNtitle, "filmInformationForm",
				     NULL);

    filmInformationForm = XtVaCreateWidget("filmInformationForm",
					   xmFormWidgetClass,
					   _UxParent,
					   XmNwidth, 646,
					   XmNheight, 533,
					   XmNunitType, XmPIXELS,
					   NULL);
    UxPutContext(filmInformationForm, (char *) UxFilmInformationFormContext);


    /* Creation of form2 */
    form2 = XtVaCreateManagedWidget("form2",
				    xmFormWidgetClass,
				    filmInformationForm,
				    XmNwidth, 610,
				    XmNheight, 500,
				    XmNresizePolicy, XmRESIZE_NONE,
				    XmNx, 30,
				    XmNy, 20,
				    NULL);
    UxPutContext(form2, (char *) UxFilmInformationFormContext);


    /* Creation of label1 */
    label1 = XtVaCreateManagedWidget("label1",
				     xmLabelWidgetClass,
				     form2,
				     XmNx, 20,
				     XmNy, 10,
				     XmNwidth, 150,
				     XmNheight, 30,
				RES_CONVERT(XmNlabelString, "Printer Name"),
				     NULL);
    UxPutContext(label1, (char *) UxFilmInformationFormContext);


    /* Creation of label2 */
    label2 = XtVaCreateManagedWidget("label2",
				     xmLabelWidgetClass,
				     form2,
				     XmNx, 20,
				     XmNy, 60,
				     XmNwidth, 150,
				     XmNheight, 30,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				     XmNleftOffset, 0,
				     XmNleftWidget, label1,
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 15,
				     XmNtopWidget, label1,
				RES_CONVERT(XmNlabelString, "Manufacturer"),
				     NULL);
    UxPutContext(label2, (char *) UxFilmInformationFormContext);


    /* Creation of label3 */
    label3 = XtVaCreateManagedWidget("label3",
				     xmLabelWidgetClass,
				     form2,
				     XmNx, 20,
				     XmNy, 100,
				     XmNwidth, 150,
				     XmNheight, 30,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				     XmNleftOffset, 0,
				     XmNleftWidget, label1,
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 15,
				     XmNtopWidget, label2,
				  RES_CONVERT(XmNlabelString, "Model Name"),
				     NULL);
    UxPutContext(label3, (char *) UxFilmInformationFormContext);


    /* Creation of label4 */
    label4 = XtVaCreateManagedWidget("label4",
				     xmLabelWidgetClass,
				     form2,
				     XmNx, 20,
				     XmNy, 150,
				     XmNwidth, 150,
				     XmNheight, 30,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				     XmNleftOffset, 0,
				     XmNleftWidget, label1,
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 15,
				     XmNtopWidget, label3,
			       RES_CONVERT(XmNlabelString, "Serial Number"),
				     NULL);
    UxPutContext(label4, (char *) UxFilmInformationFormContext);


    /* Creation of wPrinterNameTxt */
    wPrinterNameTxt = XtVaCreateManagedWidget("wPrinterNameTxt",
					      xmTextFieldWidgetClass,
					      form2,
					      XmNwidth, 350,
					      XmNx, 250,
					      XmNy, 20,
					      XmNheight, 40,
					 XmNleftAttachment, XmATTACH_WIDGET,
					      XmNleftOffset, 15,
					      XmNleftWidget, label1,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					      XmNtopOffset, 0,
					      XmNtopWidget, label1,
					      XmNeditable, FALSE,
					      NULL);
    UxPutContext(wPrinterNameTxt, (char *) UxFilmInformationFormContext);


    /* Creation of wManufacturerTxt */
    wManufacturerTxt = XtVaCreateManagedWidget("wManufacturerTxt",
					       xmTextFieldWidgetClass,
					       form2,
					       XmNwidth, 350,
					       XmNx, 250,
					       XmNy, 60,
					       XmNheight, 40,
					 XmNleftAttachment, XmATTACH_WIDGET,
					       XmNleftOffset, 15,
					       XmNleftWidget, label1,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					       XmNtopOffset, 0,
					       XmNtopWidget, label2,
					       XmNeditable, FALSE,
					       NULL);
    UxPutContext(wManufacturerTxt, (char *) UxFilmInformationFormContext);


    /* Creation of wModelNameTxt */
    wModelNameTxt = XtVaCreateManagedWidget("wModelNameTxt",
					    xmTextFieldWidgetClass,
					    form2,
					    XmNwidth, 350,
					    XmNx, 250,
					    XmNy, 100,
					    XmNheight, 40,
					 XmNleftAttachment, XmATTACH_WIDGET,
					    XmNleftOffset, 15,
					    XmNleftWidget, label1,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					    XmNtopOffset, 0,
					    XmNtopWidget, label3,
					    XmNeditable, FALSE,
					    NULL);
    UxPutContext(wModelNameTxt, (char *) UxFilmInformationFormContext);


    /* Creation of wSerialNumberTxt */
    wSerialNumberTxt = XtVaCreateManagedWidget("wSerialNumberTxt",
					       xmTextFieldWidgetClass,
					       form2,
					       XmNwidth, 350,
					       XmNx, 250,
					       XmNy, 140,
					       XmNheight, 40,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					       XmNtopOffset, 0,
					       XmNtopWidget, label4,
					 XmNleftAttachment, XmATTACH_WIDGET,
					       XmNleftOffset, 15,
					       XmNleftWidget, label1,
					       XmNeditable, FALSE,
					       NULL);
    UxPutContext(wSerialNumberTxt, (char *) UxFilmInformationFormContext);


    /* Creation of label5 */
    label5 = XtVaCreateManagedWidget("label5",
				     xmLabelWidgetClass,
				     form2,
				     XmNx, 30,
				     XmNy, 200,
				     XmNwidth, 150,
				     XmNheight, 30,
			    RES_CONVERT(XmNlabelString, "Software Version"),
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				     XmNleftOffset, 0,
				     XmNleftWidget, label1,
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 15,
				     XmNtopWidget, label4,
				     NULL);
    UxPutContext(label5, (char *) UxFilmInformationFormContext);


    /* Creation of label6 */
    label6 = XtVaCreateManagedWidget("label6",
				     xmLabelWidgetClass,
				     form2,
				     XmNx, 30,
				     XmNy, 240,
				     XmNwidth, 150,
				     XmNheight, 30,
		       RES_CONVERT(XmNlabelString, "Date Last Calibration"),
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				     XmNleftOffset, 0,
				     XmNleftWidget, label1,
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 15,
				     XmNtopWidget, label5,
				     NULL);
    UxPutContext(label6, (char *) UxFilmInformationFormContext);


    /* Creation of label7 */
    label7 = XtVaCreateManagedWidget("label7",
				     xmLabelWidgetClass,
				     form2,
				     XmNx, 30,
				     XmNy, 270,
				     XmNwidth, 150,
				     XmNheight, 30,
		       RES_CONVERT(XmNlabelString, "Time Last Calibration"),
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 15,
				     XmNtopWidget, label6,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				     XmNleftOffset, 0,
				     XmNleftWidget, label1,
				     NULL);
    UxPutContext(label7, (char *) UxFilmInformationFormContext);


    /* Creation of label8 */
    label8 = XtVaCreateManagedWidget("label8",
				     xmLabelWidgetClass,
				     form2,
				     XmNx, 30,
				     XmNy, 310,
				     XmNwidth, 150,
				     XmNheight, 30,
			      RES_CONVERT(XmNlabelString, "Printer Status"),
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				     XmNleftOffset, 0,
				     XmNleftWidget, label1,
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 15,
				     XmNtopWidget, label7,
				     NULL);
    UxPutContext(label8, (char *) UxFilmInformationFormContext);


    /* Creation of label9 */
    label9 = XtVaCreateManagedWidget("label9",
				     xmLabelWidgetClass,
				     form2,
				     XmNx, 30,
				     XmNy, 350,
				     XmNwidth, 150,
				     XmNheight, 30,
			 RES_CONVERT(XmNlabelString, "Printer Status Info"),
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				     XmNleftOffset, 0,
				     XmNleftWidget, label1,
				     XmNtopAttachment, XmATTACH_WIDGET,
				     XmNtopOffset, 15,
				     XmNtopWidget, label8,
				     NULL);
    UxPutContext(label9, (char *) UxFilmInformationFormContext);


    /* Creation of wSoftwareVersionTxt */
    wSoftwareVersionTxt = XtVaCreateManagedWidget("wSoftwareVersionTxt",
						  xmTextFieldWidgetClass,
						  form2,
						  XmNwidth, 350,
						  XmNx, 190,
						  XmNy, 200,
						  XmNheight, 40,
					 XmNleftAttachment, XmATTACH_WIDGET,
						  XmNleftOffset, 15,
						  XmNleftWidget, label1,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
						  XmNtopOffset, 0,
						  XmNtopWidget, label5,
						  XmNeditable, FALSE,
						  NULL);
    UxPutContext(wSoftwareVersionTxt, (char *) UxFilmInformationFormContext);


    /* Creation of wDateLastCalibrationTxt */
    wDateLastCalibrationTxt = XtVaCreateManagedWidget("wDateLastCalibrationTxt",
						      xmTextFieldWidgetClass,
						      form2,
						      XmNwidth, 350,
						      XmNx, 190,
						      XmNy, 240,
						      XmNheight, 40,
					 XmNleftAttachment, XmATTACH_WIDGET,
						      XmNleftOffset, 15,
						      XmNleftWidget, label1,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
						      XmNtopOffset, 0,
						      XmNtopWidget, label6,
						      XmNeditable, FALSE,
						      NULL);
    UxPutContext(wDateLastCalibrationTxt, (char *) UxFilmInformationFormContext);


    /* Creation of wTimeLastCalibrationTxt */
    wTimeLastCalibrationTxt = XtVaCreateManagedWidget("wTimeLastCalibrationTxt",
						      xmTextFieldWidgetClass,
						      form2,
						      XmNwidth, 350,
						      XmNx, 180,
						      XmNy, 290,
						      XmNheight, 40,
					 XmNleftAttachment, XmATTACH_WIDGET,
						      XmNleftOffset, 15,
						      XmNleftWidget, label1,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
						      XmNtopOffset, 0,
						      XmNtopWidget, label7,
						      XmNeditable, FALSE,
						      NULL);
    UxPutContext(wTimeLastCalibrationTxt, (char *) UxFilmInformationFormContext);


    /* Creation of wPrinterStatusTxt */
    wPrinterStatusTxt = XtVaCreateManagedWidget("wPrinterStatusTxt",
						xmTextFieldWidgetClass,
						form2,
						XmNwidth, 350,
						XmNx, 180,
						XmNy, 330,
						XmNheight, 40,
					 XmNleftAttachment, XmATTACH_WIDGET,
						XmNleftOffset, 15,
						XmNleftWidget, label1,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
						XmNtopOffset, 0,
						XmNtopWidget, label8,
						XmNeditable, FALSE,
						NULL);
    UxPutContext(wPrinterStatusTxt, (char *) UxFilmInformationFormContext);


    /* Creation of wPrinterStatusInfoTxt */
    wPrinterStatusInfoTxt = XtVaCreateManagedWidget("wPrinterStatusInfoTxt",
						    xmTextFieldWidgetClass,
						    form2,
						    XmNwidth, 350,
						    XmNx, 180,
						    XmNy, 390,
						    XmNheight, 40,
					 XmNleftAttachment, XmATTACH_WIDGET,
						    XmNleftOffset, 15,
						    XmNleftWidget, label1,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
						    XmNtopOffset, 0,
						    XmNtopWidget, label9,
						    XmNeditable, FALSE,
						    NULL);
    UxPutContext(wPrinterStatusInfoTxt, (char *) UxFilmInformationFormContext);


    /* Creation of pushButton1 */
    pushButton1 = XtVaCreateManagedWidget("pushButton1",
					  xmPushButtonWidgetClass,
					  form2,
					  XmNx, 190,
					  XmNy, 430,
					  XmNwidth, 220,
					  XmNheight, 50,
					  RES_CONVERT(XmNlabelString, "Get"),
					  NULL);
    XtAddCallback(pushButton1, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton1,
		  (XtPointer) UxFilmInformationFormContext);

    UxPutContext(pushButton1, (char *) UxFilmInformationFormContext);


    XtAddCallback(filmInformationForm, XmNdestroyCallback,
		  (XtCallbackProc) UxDestroyContextCB,
		  (XtPointer) UxFilmInformationFormContext);


    return (filmInformationForm);
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget
create_filmInformationForm(swidget _UxUxParent)
{
    Widget rtrn;
    _UxCfilmInformationForm *UxContext;

    UxFilmInformationFormContext = UxContext =
	(_UxCfilmInformationForm *) UxNewContext(sizeof(_UxCfilmInformationForm), False);

    UxParent = _UxUxParent;

    rtrn = _Uxbuild_filmInformationForm();

    return (rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/
