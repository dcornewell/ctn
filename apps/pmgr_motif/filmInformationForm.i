! UIMX ascii 2.5 key: 5167                                                      

*filmInformationForm.class: formDialog
*filmInformationForm.gbldecl: /*\
          Copyright (C) 1993, 1994, RSNA and Washington University\
 \
          The software and supporting documentation for the Radiological\
          Society of North America (RSNA) 1993, 1994 Digital Imaging and\
          Communications in Medicine (DICOM) Demonstration were developed\
          at the\
                  Electronic Radiology Laboratory\
                  Mallinckrodt Institute of Radiology\
                  Washington University School of Medicine\
                  510 S. Kingshighway Blvd.\
                  St. Louis, MO 63110\
          as part of the 1993, 1994 DICOM Central Test Node project for, and\
          under contract with, the Radiological Society of North America.\
 \
          THIS SOFTWARE IS MADE AVAILABLE, AS IS, AND NEITHER RSNA NOR\
          WASHINGTON UNIVERSITY MAKE ANY WARRANTY ABOUT THE SOFTWARE, ITS\
          PERFORMANCE, ITS MERCHANTABILITY OR FITNESS FOR ANY PARTICULAR\
          USE, FREEDOM FROM ANY COMPUTER DISEASES OR ITS CONFORMITY TO ANY\
          SPECIFICATION. THE ENTIRE RISK AS TO QUALITY AND PERFORMANCE OF\
          THE SOFTWARE IS WITH THE USER.\
 \
          Copyright of the software and supporting documentation is\
          jointly owned by RSNA and Washington University, and free access\
          is hereby granted as a license to use this software, copy this\
          software and prepare derivative works based upon this software.\
          However, any distribution of this software source code or\
          supporting documentation or derivative works (source code and\
          supporting documentation) must include the three paragraphs of\
          the copyright notice.\
*/\
/* Copyright marker.  Copyright will be inserted above.  Do not remove */\
/*\
**                   Electronic Radiology Laboratory\
**                 Mallinckrodt Institute of Radiology\
**              Washington University School of Medicine\
**\
** Module Name(s):	create_filmInformationForm\
** Author, Date:	Chander Sabharwal\
** Intent:		This module contains the widgets which display\
**			printer information, that is, information that\
**			can be retrieved from DICOM printers.\
** Last Update:		$Author: smm $, $Date: 1998-04-17 15:20:20 $\
** Source File:		$RCSfile: filmInformationForm.i,v $\
** Revision:		$Revision: 1.6 $\
** Status:		$State: Exp $\
*/\
 \
static char rcsid[] = "$Revision: 1.6 $ $RCSfile: filmInformationForm.i,v $";\
\
#include <stdio.h>\
\
#include "dicom.h"\
#include "condition.h"\
#include "lst.h"\
#include "dicom_uids.h"\
#include "dulprotocol.h"\
#include "tbl.h"\
#include "manage.h"\
#include "dicom_objects.h"\
#include "dicom_print.h"\
\
#include "print_client_app.h"\
#include "print_shared.h"\
\
extern DUL_ASSOCIATIONKEY *associationKey;\
extern DUL_ASSOCIATESERVICEPARAMETERS associationParameters;\
\
static void displayPrinterInformation(PRN_PRINTER *attributes);
*filmInformationForm.ispecdecl:
*filmInformationForm.funcdecl: swidget create_filmInformationForm(swidget UxParent)
*filmInformationForm.funcname: create_filmInformationForm
*filmInformationForm.funcdef: "swidget", "<create_filmInformationForm>(%)"
*filmInformationForm.argdecl: swidget UxParent;
*filmInformationForm.arglist: UxParent
*filmInformationForm.arglist.UxParent: "swidget", "%UxParent%"
*filmInformationForm.icode:
*filmInformationForm.fcode: return(rtrn);\

*filmInformationForm.auxdecl: static void displayPrinterInformation(PRN_PRINTER *a)\
{\
    XmTextSetString(wPrinterNameTxt, "");\
    XmTextSetString(wManufacturerTxt, "");\
    XmTextSetString(wModelNameTxt, "");\
    XmTextSetString(wSerialNumberTxt, "");\
    XmTextSetString(wSoftwareVersionTxt, "");\
    XmTextSetString(wDateLastCalibrationTxt, "");\
    XmTextSetString(wTimeLastCalibrationTxt, "");\
    XmTextSetString(wPrinterStatusTxt, "");\
    XmTextSetString(wPrinterStatusInfoTxt, "");\
\
    if (a == NULL)\
	return;\
\
    if (a->attributeFlag & PRN_PRINTER_K_STATUS)\
	XmTextSetString(wPrinterStatusTxt, a->status);\
    if (a->attributeFlag & PRN_PRINTER_K_STATUSINFO)\
	XmTextSetString(wPrinterStatusInfoTxt, a->statusInfoCode);\
    if (a->attributeFlag & PRN_PRINTER_K_NAME)\
	XmTextSetString(wPrinterNameTxt, a->name);\
    if (a->attributeFlag & PRN_PRINTER_K_MANUFACTURER)\
	XmTextSetString(wManufacturerTxt, a->manufacturer);\
    if (a->attributeFlag & PRN_PRINTER_K_MODELNUMBER)\
	XmTextSetString(wModelNameTxt, a->manufacturerModelName);\
    if (a->attributeFlag & PRN_PRINTER_K_SERIALNUMBER)\
	XmTextSetString(wSerialNumberTxt, a->deviceSerialNumber);\
    if (a->attributeFlag & PRN_PRINTER_K_SOFTWAREVERSION)\
	XmTextSetString(wSoftwareVersionTxt, a->softwareVersion);\
    if (a->attributeFlag & PRN_PRINTER_K_DATEOFCALIBRATION)\
	XmTextSetString(wDateLastCalibrationTxt, a->dateOfLastCalibration);\
    if (a->attributeFlag & PRN_PRINTER_K_TIMEOFCALIBRATION)\
	XmTextSetString(wTimeLastCalibrationTxt, a->timeOfLastCalibration);\
}
*filmInformationForm.static: true
*filmInformationForm.name: filmInformationForm
*filmInformationForm.parent: NO_PARENT
*filmInformationForm.parentExpression: UxParent
*filmInformationForm.defaultShell: topLevelShell
*filmInformationForm.width: 646
*filmInformationForm.height: 533
*filmInformationForm.isCompound: "true"
*filmInformationForm.compoundIcon: "formD.xpm"
*filmInformationForm.compoundName: "form_Dialog"
*filmInformationForm.x: 17
*filmInformationForm.y: 325
*filmInformationForm.unitType: "pixels"

*form2.class: form
*form2.static: true
*form2.name: form2
*form2.parent: filmInformationForm
*form2.width: 610
*form2.height: 500
*form2.resizePolicy: "resize_none"
*form2.isCompound: "true"
*form2.compoundIcon: "form.xpm"
*form2.compoundName: "form_"
*form2.x: 30
*form2.y: 20

*label1.class: label
*label1.static: true
*label1.name: label1
*label1.parent: form2
*label1.isCompound: "true"
*label1.compoundIcon: "label.xpm"
*label1.compoundName: "label_"
*label1.x: 20
*label1.y: 10
*label1.width: 150
*label1.height: 30
*label1.labelString: "Printer Name"

*label2.class: label
*label2.static: true
*label2.name: label2
*label2.parent: form2
*label2.isCompound: "true"
*label2.compoundIcon: "label.xpm"
*label2.compoundName: "label_"
*label2.x: 20
*label2.y: 60
*label2.width: 150
*label2.height: 30
*label2.leftAttachment: "attach_opposite_widget"
*label2.leftOffset: 0
*label2.leftWidget: "label1"
*label2.topAttachment: "attach_widget"
*label2.topOffset: 15
*label2.topWidget: "label1"
*label2.labelString: "Manufacturer"

*label3.class: label
*label3.static: true
*label3.name: label3
*label3.parent: form2
*label3.isCompound: "true"
*label3.compoundIcon: "label.xpm"
*label3.compoundName: "label_"
*label3.x: 20
*label3.y: 100
*label3.width: 150
*label3.height: 30
*label3.leftAttachment: "attach_opposite_widget"
*label3.leftOffset: 0
*label3.leftWidget: "label1"
*label3.topAttachment: "attach_widget"
*label3.topOffset: 15
*label3.topWidget: "label2"
*label3.labelString: "Model Name"

*label4.class: label
*label4.static: true
*label4.name: label4
*label4.parent: form2
*label4.isCompound: "true"
*label4.compoundIcon: "label.xpm"
*label4.compoundName: "label_"
*label4.x: 20
*label4.y: 150
*label4.width: 150
*label4.height: 30
*label4.leftAttachment: "attach_opposite_widget"
*label4.leftOffset: 0
*label4.leftWidget: "label1"
*label4.topAttachment: "attach_widget"
*label4.topOffset: 15
*label4.topWidget: "label3"
*label4.labelString: "Serial Number"

*wPrinterNameTxt.class: textField
*wPrinterNameTxt.static: true
*wPrinterNameTxt.name: wPrinterNameTxt
*wPrinterNameTxt.parent: form2
*wPrinterNameTxt.width: 350
*wPrinterNameTxt.isCompound: "true"
*wPrinterNameTxt.compoundIcon: "textfield.xpm"
*wPrinterNameTxt.compoundName: "text_Field"
*wPrinterNameTxt.x: 250
*wPrinterNameTxt.y: 20
*wPrinterNameTxt.height: 40
*wPrinterNameTxt.leftAttachment: "attach_widget"
*wPrinterNameTxt.leftOffset: 15
*wPrinterNameTxt.leftWidget: "label1"
*wPrinterNameTxt.topAttachment: "attach_opposite_widget"
*wPrinterNameTxt.topOffset: 0
*wPrinterNameTxt.topWidget: "label1"
*wPrinterNameTxt.editable: "false"

*wManufacturerTxt.class: textField
*wManufacturerTxt.static: true
*wManufacturerTxt.name: wManufacturerTxt
*wManufacturerTxt.parent: form2
*wManufacturerTxt.width: 350
*wManufacturerTxt.isCompound: "true"
*wManufacturerTxt.compoundIcon: "textfield.xpm"
*wManufacturerTxt.compoundName: "text_Field"
*wManufacturerTxt.x: 250
*wManufacturerTxt.y: 60
*wManufacturerTxt.height: 40
*wManufacturerTxt.leftAttachment: "attach_widget"
*wManufacturerTxt.leftOffset: 15
*wManufacturerTxt.leftWidget: "label1"
*wManufacturerTxt.topAttachment: "attach_opposite_widget"
*wManufacturerTxt.topOffset: 0
*wManufacturerTxt.topWidget: "label2"
*wManufacturerTxt.editable: "false"

*wModelNameTxt.class: textField
*wModelNameTxt.static: true
*wModelNameTxt.name: wModelNameTxt
*wModelNameTxt.parent: form2
*wModelNameTxt.width: 350
*wModelNameTxt.isCompound: "true"
*wModelNameTxt.compoundIcon: "textfield.xpm"
*wModelNameTxt.compoundName: "text_Field"
*wModelNameTxt.x: 250
*wModelNameTxt.y: 100
*wModelNameTxt.height: 40
*wModelNameTxt.leftAttachment: "attach_widget"
*wModelNameTxt.leftOffset: 15
*wModelNameTxt.leftWidget: "label1"
*wModelNameTxt.topAttachment: "attach_opposite_widget"
*wModelNameTxt.topOffset: 0
*wModelNameTxt.topWidget: "label3"
*wModelNameTxt.editable: "false"

*wSerialNumberTxt.class: textField
*wSerialNumberTxt.static: true
*wSerialNumberTxt.name: wSerialNumberTxt
*wSerialNumberTxt.parent: form2
*wSerialNumberTxt.width: 350
*wSerialNumberTxt.isCompound: "true"
*wSerialNumberTxt.compoundIcon: "textfield.xpm"
*wSerialNumberTxt.compoundName: "text_Field"
*wSerialNumberTxt.x: 250
*wSerialNumberTxt.y: 140
*wSerialNumberTxt.height: 40
*wSerialNumberTxt.topAttachment: "attach_opposite_widget"
*wSerialNumberTxt.topOffset: 0
*wSerialNumberTxt.topWidget: "label4"
*wSerialNumberTxt.leftAttachment: "attach_widget"
*wSerialNumberTxt.leftOffset: 15
*wSerialNumberTxt.leftWidget: "label1"
*wSerialNumberTxt.editable: "false"

*label5.class: label
*label5.static: true
*label5.name: label5
*label5.parent: form2
*label5.isCompound: "true"
*label5.compoundIcon: "label.xpm"
*label5.compoundName: "label_"
*label5.x: 30
*label5.y: 200
*label5.width: 150
*label5.height: 30
*label5.labelString: "Software Version"
*label5.leftAttachment: "attach_opposite_widget"
*label5.leftOffset: 0
*label5.leftWidget: "label1"
*label5.topAttachment: "attach_widget"
*label5.topOffset: 15
*label5.topWidget: "label4"

*label6.class: label
*label6.static: true
*label6.name: label6
*label6.parent: form2
*label6.isCompound: "true"
*label6.compoundIcon: "label.xpm"
*label6.compoundName: "label_"
*label6.x: 30
*label6.y: 240
*label6.width: 150
*label6.height: 30
*label6.labelString: "Date Last Calibration"
*label6.leftAttachment: "attach_opposite_widget"
*label6.leftOffset: 0
*label6.leftWidget: "label1"
*label6.topAttachment: "attach_widget"
*label6.topOffset: 15
*label6.topWidget: "label5"

*label7.class: label
*label7.static: true
*label7.name: label7
*label7.parent: form2
*label7.isCompound: "true"
*label7.compoundIcon: "label.xpm"
*label7.compoundName: "label_"
*label7.x: 30
*label7.y: 270
*label7.width: 150
*label7.height: 30
*label7.labelString: "Time Last Calibration"
*label7.topAttachment: "attach_widget"
*label7.topOffset: 15
*label7.topWidget: "label6"
*label7.leftAttachment: "attach_opposite_widget"
*label7.leftOffset: 0
*label7.leftWidget: "label1"

*label8.class: label
*label8.static: true
*label8.name: label8
*label8.parent: form2
*label8.isCompound: "true"
*label8.compoundIcon: "label.xpm"
*label8.compoundName: "label_"
*label8.x: 30
*label8.y: 310
*label8.width: 150
*label8.height: 30
*label8.labelString: "Printer Status"
*label8.leftAttachment: "attach_opposite_widget"
*label8.leftOffset: 0
*label8.leftWidget: "label1"
*label8.topAttachment: "attach_widget"
*label8.topOffset: 15
*label8.topWidget: "label7"

*label9.class: label
*label9.static: true
*label9.name: label9
*label9.parent: form2
*label9.isCompound: "true"
*label9.compoundIcon: "label.xpm"
*label9.compoundName: "label_"
*label9.x: 30
*label9.y: 350
*label9.width: 150
*label9.height: 30
*label9.labelString: "Printer Status Info"
*label9.leftAttachment: "attach_opposite_widget"
*label9.leftOffset: 0
*label9.leftWidget: "label1"
*label9.topAttachment: "attach_widget"
*label9.topOffset: 15
*label9.topWidget: "label8"

*wSoftwareVersionTxt.class: textField
*wSoftwareVersionTxt.static: true
*wSoftwareVersionTxt.name: wSoftwareVersionTxt
*wSoftwareVersionTxt.parent: form2
*wSoftwareVersionTxt.width: 350
*wSoftwareVersionTxt.isCompound: "true"
*wSoftwareVersionTxt.compoundIcon: "textfield.xpm"
*wSoftwareVersionTxt.compoundName: "text_Field"
*wSoftwareVersionTxt.x: 190
*wSoftwareVersionTxt.y: 200
*wSoftwareVersionTxt.height: 40
*wSoftwareVersionTxt.leftAttachment: "attach_widget"
*wSoftwareVersionTxt.leftOffset: 15
*wSoftwareVersionTxt.leftWidget: "label1"
*wSoftwareVersionTxt.topAttachment: "attach_opposite_widget"
*wSoftwareVersionTxt.topOffset: 0
*wSoftwareVersionTxt.topWidget: "label5"
*wSoftwareVersionTxt.editable: "false"

*wDateLastCalibrationTxt.class: textField
*wDateLastCalibrationTxt.static: true
*wDateLastCalibrationTxt.name: wDateLastCalibrationTxt
*wDateLastCalibrationTxt.parent: form2
*wDateLastCalibrationTxt.width: 350
*wDateLastCalibrationTxt.isCompound: "true"
*wDateLastCalibrationTxt.compoundIcon: "textfield.xpm"
*wDateLastCalibrationTxt.compoundName: "text_Field"
*wDateLastCalibrationTxt.x: 190
*wDateLastCalibrationTxt.y: 240
*wDateLastCalibrationTxt.height: 40
*wDateLastCalibrationTxt.leftAttachment: "attach_widget"
*wDateLastCalibrationTxt.leftOffset: 15
*wDateLastCalibrationTxt.leftWidget: "label1"
*wDateLastCalibrationTxt.topAttachment: "attach_opposite_widget"
*wDateLastCalibrationTxt.topOffset: 0
*wDateLastCalibrationTxt.topWidget: "label6"
*wDateLastCalibrationTxt.editable: "false"

*wTimeLastCalibrationTxt.class: textField
*wTimeLastCalibrationTxt.static: true
*wTimeLastCalibrationTxt.name: wTimeLastCalibrationTxt
*wTimeLastCalibrationTxt.parent: form2
*wTimeLastCalibrationTxt.width: 350
*wTimeLastCalibrationTxt.isCompound: "true"
*wTimeLastCalibrationTxt.compoundIcon: "textfield.xpm"
*wTimeLastCalibrationTxt.compoundName: "text_Field"
*wTimeLastCalibrationTxt.x: 180
*wTimeLastCalibrationTxt.y: 290
*wTimeLastCalibrationTxt.height: 40
*wTimeLastCalibrationTxt.leftAttachment: "attach_widget"
*wTimeLastCalibrationTxt.leftOffset: 15
*wTimeLastCalibrationTxt.leftWidget: "label1"
*wTimeLastCalibrationTxt.topAttachment: "attach_opposite_widget"
*wTimeLastCalibrationTxt.topOffset: 0
*wTimeLastCalibrationTxt.topWidget: "label7"
*wTimeLastCalibrationTxt.editable: "false"

*wPrinterStatusTxt.class: textField
*wPrinterStatusTxt.static: true
*wPrinterStatusTxt.name: wPrinterStatusTxt
*wPrinterStatusTxt.parent: form2
*wPrinterStatusTxt.width: 350
*wPrinterStatusTxt.isCompound: "true"
*wPrinterStatusTxt.compoundIcon: "textfield.xpm"
*wPrinterStatusTxt.compoundName: "text_Field"
*wPrinterStatusTxt.x: 180
*wPrinterStatusTxt.y: 330
*wPrinterStatusTxt.height: 40
*wPrinterStatusTxt.leftAttachment: "attach_widget"
*wPrinterStatusTxt.leftOffset: 15
*wPrinterStatusTxt.leftWidget: "label1"
*wPrinterStatusTxt.topAttachment: "attach_opposite_widget"
*wPrinterStatusTxt.topOffset: 0
*wPrinterStatusTxt.topWidget: "label8"
*wPrinterStatusTxt.editable: "false"

*wPrinterStatusInfoTxt.class: textField
*wPrinterStatusInfoTxt.static: true
*wPrinterStatusInfoTxt.name: wPrinterStatusInfoTxt
*wPrinterStatusInfoTxt.parent: form2
*wPrinterStatusInfoTxt.width: 350
*wPrinterStatusInfoTxt.isCompound: "true"
*wPrinterStatusInfoTxt.compoundIcon: "textfield.xpm"
*wPrinterStatusInfoTxt.compoundName: "text_Field"
*wPrinterStatusInfoTxt.x: 180
*wPrinterStatusInfoTxt.y: 390
*wPrinterStatusInfoTxt.height: 40
*wPrinterStatusInfoTxt.leftAttachment: "attach_widget"
*wPrinterStatusInfoTxt.leftOffset: 15
*wPrinterStatusInfoTxt.leftWidget: "label1"
*wPrinterStatusInfoTxt.topAttachment: "attach_opposite_widget"
*wPrinterStatusInfoTxt.topOffset: 0
*wPrinterStatusInfoTxt.topWidget: "label9"
*wPrinterStatusInfoTxt.editable: "false"

*pushButton1.class: pushButton
*pushButton1.static: true
*pushButton1.name: pushButton1
*pushButton1.parent: form2
*pushButton1.isCompound: "true"
*pushButton1.compoundIcon: "push.xpm"
*pushButton1.compoundName: "push_Button"
*pushButton1.x: 190
*pushButton1.y: 430
*pushButton1.width: 220
*pushButton1.height: 50
*pushButton1.labelString: "Get"
*pushButton1.activateCallback: {\
  CONDITION\
    cond;\
  unsigned short\
    status;\
  PRN_PRINTER *attributes = NULL;\
\
  cond = getPrinterInstance(&associationKey, &associationParameters,\
	DICOM_SOPCLASSGREYSCALEPRINTMGMTMETA,\
	&attributes, &status);\
\
    if (cond != APP_NORMAL) {\
	COND_DumpConditions();\
	if (attributes != NULL)\
	    PRN_FreeStructure(&attributes);\
    }\
    displayPrinterInformation(attributes);\
    PRN_FreeStructure(&attributes);\
}

