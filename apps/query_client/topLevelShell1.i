! UIMX ascii 2.5 key: 2046                                                      

*topLevelShell1.class: topLevelShell
*topLevelShell1.gbldecl: /*\
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
\
/*\
**                   Electronic Radiology Laboratory\
**                 Mallinckrodt Institute of Radiology\
**              Washington University School of Medicine\
**\
** Module Name(s):	create_topLevelShell1\
** Author, Date:	Steve Moore\
** Intent:		This file contains the code the creates and\
**			implements the user interface for the query_client\
**			program.  This consists of functions that create\
**			the widgets that provide the user interface and\
**			the (private) callback functions that are executed\
**			to service an event (like a button push).\
** Last Update:         $Author: smm $, $Date: 1995-12-21 20:31:25 $\
** Source File:         $RCSfile: topLevelShell1.i,v $\
** Revision:            $Revision: 1.5 $\
** Status:              $State: Exp $\
*/\
 \
static char rcsid[] = "$Revision: 1.5 $ $RCSfile: topLevelShell1.i,v $";\
\
#include <Xm/Text.h>\
\
#include <stdio.h>\
\
#include "dicom.h"\
#include "lst.h"\
#include "condition.h"\
#include "mut.h"\
\
#include "query_interface.h"\
#include "format.h"\
\
static LST_HEAD *patientList = NULL;\
static LST_HEAD *studyList = NULL;\
static PATIENT_QUERY_MODULE selectedPatient;\
static STUDY_QUERY_MODULE selectedStudy;\
\
static void copyWidgetText(Widget w, char *txt);\
static void listClear(LST_HEAD **lst);\

*topLevelShell1.ispecdecl:
*topLevelShell1.funcdecl: swidget create_topLevelShell1(swidget UxParent)
*topLevelShell1.funcname: create_topLevelShell1
*topLevelShell1.funcdef: "swidget", "<create_topLevelShell1>(%)"
*topLevelShell1.argdecl: swidget UxParent;
*topLevelShell1.arglist: UxParent
*topLevelShell1.arglist.UxParent: "swidget", "%UxParent%"
*topLevelShell1.icode:
*topLevelShell1.fcode: return(rtrn);\

*topLevelShell1.auxdecl: static void copyWidgetText(Widget w, char *txt)\
{\
  char\
    *t;\
\
    *txt = '\0';\
    t = XmTextGetString(w);\
    if (t == NULL)\
	return;\
    (void)sscanf(t, "%s", txt);\
}\
\
static void listClear(LST_HEAD **lst)\
{\
  LST_NODE\
    *node;\
\
    while ((node = LST_Dequeue(lst)) != NULL)\
	free(node);\
}
*topLevelShell1.static: true
*topLevelShell1.name: topLevelShell1
*topLevelShell1.parent: NO_PARENT
*topLevelShell1.parentExpression: UxParent
*topLevelShell1.width: 980
*topLevelShell1.height: 470
*topLevelShell1.isCompound: "true"
*topLevelShell1.compoundIcon: "toplevelS.xpm"
*topLevelShell1.compoundName: "topLevel_Shell"
*topLevelShell1.x: 53
*topLevelShell1.y: 223

*mainWindow1.class: mainWindow
*mainWindow1.static: true
*mainWindow1.name: mainWindow1
*mainWindow1.parent: topLevelShell1
*mainWindow1.width: 980
*mainWindow1.height: 470
*mainWindow1.isCompound: "true"
*mainWindow1.compoundIcon: "main.xpm"
*mainWindow1.compoundName: "main_Window"
*mainWindow1.compoundEditor: {\
extern swidget UxGUIMePopup UXPROTO((swidget, swidget, int, int));\
UxGUIMwePopup(UxThisWidget, UxGetParent(UxThisWidget));\
}
*mainWindow1.x: 120
*mainWindow1.y: 20
*mainWindow1.unitType: "pixels"

*drawingArea1.class: drawingArea
*drawingArea1.static: true
*drawingArea1.name: drawingArea1
*drawingArea1.parent: mainWindow1

*scrolledWindowList2.class: scrolledWindow
*scrolledWindowList2.static: true
*scrolledWindowList2.name: scrolledWindowList2
*scrolledWindowList2.parent: drawingArea1
*scrolledWindowList2.scrollingPolicy: "application_defined"
*scrolledWindowList2.visualPolicy: "variable"
*scrolledWindowList2.scrollBarDisplayPolicy: "static"
*scrolledWindowList2.shadowThickness: 0
*scrolledWindowList2.isCompound: "true"
*scrolledWindowList2.compoundIcon: "scrllist.xpm"
*scrolledWindowList2.compoundName: "scrolled_List"
*scrolledWindowList2.x: 520
*scrolledWindowList2.y: 220

*studyListWidget.class: scrolledList
*studyListWidget.static: true
*studyListWidget.name: studyListWidget
*studyListWidget.parent: scrolledWindowList2
*studyListWidget.width: 420
*studyListWidget.height: 180
*studyListWidget.listSizePolicy: "constant"
*studyListWidget.browseSelectionCallback: {\
XmListCallbackStruct *listStruct;\
STUDY_QUERY_MODULE\
  *s;\
int\
    i;\
\
listStruct = (XmListCallbackStruct *)UxCallbackArg;\
s = LST_Head(&studyList);\
(void)LST_Position(&studyList, s);\
\
for (i = 2; i <= listStruct->item_position; i++)\
    s = LST_Next(&studyList);\
\
selectedStudy = *s;\
}
*studyListWidget.fontList: "-misc-fixed-medium-r-normal--14-130-75-75-c-70-iso8859-1"

*scrolledWindowList1.class: scrolledWindow
*scrolledWindowList1.static: true
*scrolledWindowList1.name: scrolledWindowList1
*scrolledWindowList1.parent: drawingArea1
*scrolledWindowList1.scrollingPolicy: "application_defined"
*scrolledWindowList1.visualPolicy: "variable"
*scrolledWindowList1.scrollBarDisplayPolicy: "static"
*scrolledWindowList1.shadowThickness: 0
*scrolledWindowList1.isCompound: "true"
*scrolledWindowList1.compoundIcon: "scrllist.xpm"
*scrolledWindowList1.compoundName: "scrolled_List"
*scrolledWindowList1.x: 10
*scrolledWindowList1.y: 220

*patientListWidget.class: scrolledList
*patientListWidget.static: true
*patientListWidget.name: patientListWidget
*patientListWidget.parent: scrolledWindowList1
*patientListWidget.width: 420
*patientListWidget.height: 180
*patientListWidget.listSizePolicy: "constant"
*patientListWidget.browseSelectionCallback: {\
XmListCallbackStruct *listStruct;\
PATIENT_QUERY_MODULE\
  *p;\
int\
    i;\
CONDITION\
    cond;\
char\
    *txt;\
CONNECTION_MODULE\
    connection;\
char\
    buf[80];\
\
listStruct = (XmListCallbackStruct *)UxCallbackArg;\
p = LST_Head(&patientList);\
(void)LST_Position(&patientList, p);\
\
for (i = 2; i <= listStruct->item_position; i++)\
    p = LST_Next(&patientList);\
\
printf("%s\n", p->PatientID);\
\
copyWidgetText(imageServerNodeTxt, connection.NodeName);\
copyWidgetText(imageServerPortTxt, connection.PortNumber);\
copyWidgetText(calledAPTitleTxt,   connection.CalledAPTitle);\
copyWidgetText(callingAPTitleTxt,  connection.CallingAPTitle);\
\
if (studyList == NULL)\
    studyList = LST_Create();\
\
if (studyList == NULL)\
    return;\
\
listClear(&studyList);\
cond = studyQuery(&connection, p, &studyList);\
COND_DumpConditions();\
MUT_LoadList(studyListWidget, studyList, formatStudy, buf);\
\
selectedPatient = *p;\
}
*patientListWidget.fontList: "-misc-fixed-medium-r-normal--14-130-75-75-c-70-iso8859-1"

*frame3.class: frame
*frame3.static: true
*frame3.name: frame3
*frame3.parent: drawingArea1
*frame3.width: 320
*frame3.height: 190
*frame3.isCompound: "true"
*frame3.compoundIcon: "frame.xpm"
*frame3.compoundName: "frame_"
*frame3.x: 650
*frame3.y: 0

*form3.class: form
*form3.static: true
*form3.name: form3
*form3.parent: frame3
*form3.width: 250
*form3.height: 140
*form3.resizePolicy: "resize_none"
*form3.isCompound: "true"
*form3.compoundIcon: "form.xpm"
*form3.compoundName: "form_"
*form3.x: 20
*form3.y: 20

*label11.class: label
*label11.static: true
*label11.name: label11
*label11.parent: form3
*label11.isCompound: "true"
*label11.compoundIcon: "label.xpm"
*label11.compoundName: "label_"
*label11.x: 20
*label11.y: 50
*label11.width: 150
*label11.height: 20
*label11.labelString: "Destination AP Title"

*destinationAPTitleTxt.class: textField
*destinationAPTitleTxt.static: true
*destinationAPTitleTxt.name: destinationAPTitleTxt
*destinationAPTitleTxt.parent: form3
*destinationAPTitleTxt.width: 130
*destinationAPTitleTxt.isCompound: "true"
*destinationAPTitleTxt.compoundIcon: "textfield.xpm"
*destinationAPTitleTxt.compoundName: "text_Field"
*destinationAPTitleTxt.x: 180
*destinationAPTitleTxt.y: 40
*destinationAPTitleTxt.height: 40
*destinationAPTitleTxt.fontList: "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1"

*moveButton.class: pushButton
*moveButton.static: true
*moveButton.name: moveButton
*moveButton.parent: form3
*moveButton.isCompound: "true"
*moveButton.compoundIcon: "push.xpm"
*moveButton.compoundName: "push_Button"
*moveButton.x: 30
*moveButton.y: 150
*moveButton.width: 280
*moveButton.height: 30
*moveButton.labelString: "Move"
*moveButton.activateCallback: {\
CONNECTION_MODULE\
    connection;\
char\
    moveDestination[20];\
CONDITION\
    cond;\
\
copyWidgetText(imageServerNodeTxt, connection.NodeName);\
copyWidgetText(imageServerPortTxt, connection.PortNumber);\
copyWidgetText(calledAPTitleTxt,   connection.CalledAPTitle);\
copyWidgetText(callingAPTitleTxt,  connection.CallingAPTitle);\
\
copyWidgetText(destinationAPTitleTxt, moveDestination);\
\
cond = moveRequest(&connection, moveDestination,\
	&selectedPatient, &selectedStudy);\
COND_DumpConditions();\
}

*frame1.class: frame
*frame1.static: true
*frame1.name: frame1
*frame1.parent: drawingArea1
*frame1.width: 320
*frame1.height: 190
*frame1.isCompound: "true"
*frame1.compoundIcon: "frame.xpm"
*frame1.compoundName: "frame_"
*frame1.x: 330
*frame1.y: 0

*form2.class: form
*form2.static: true
*form2.name: form2
*form2.parent: frame1
*form2.width: 180
*form2.height: 120
*form2.resizePolicy: "resize_none"
*form2.isCompound: "true"
*form2.compoundIcon: "form.xpm"
*form2.compoundName: "form_"
*form2.x: 40
*form2.y: 20

*label6.class: label
*label6.static: true
*label6.name: label6
*label6.parent: form2
*label6.isCompound: "true"
*label6.compoundIcon: "label.xpm"
*label6.compoundName: "label_"
*label6.x: 0
*label6.y: 30
*label6.width: 150
*label6.height: 20
*label6.labelString: "Patient ID"

*label7.class: label
*label7.static: true
*label7.name: label7
*label7.parent: form2
*label7.isCompound: "true"
*label7.compoundIcon: "label.xpm"
*label7.compoundName: "label_"
*label7.x: 22
*label7.y: 82
*label7.width: 150
*label7.height: 20
*label7.labelString: "Last Name"
*label7.topAttachment: "attach_widget"
*label7.topOffset: 20
*label7.topWidget: "label6"
*label7.leftAttachment: "attach_opposite_widget"
*label7.leftOffset: 0
*label7.leftWidget: "label6"

*label8.class: label
*label8.static: true
*label8.name: label8
*label8.parent: form2
*label8.isCompound: "true"
*label8.compoundIcon: "label.xpm"
*label8.compoundName: "label_"
*label8.x: 20
*label8.y: 110
*label8.width: 150
*label8.height: 20
*label8.labelString: "Birth Date"
*label8.topAttachment: "attach_widget"
*label8.topOffset: 20
*label8.topWidget: "label7"
*label8.leftAttachment: "attach_opposite_widget"
*label8.leftOffset: 0
*label8.leftWidget: "label6"

*patientIDTxt.class: textField
*patientIDTxt.static: true
*patientIDTxt.name: patientIDTxt
*patientIDTxt.parent: form2
*patientIDTxt.width: 140
*patientIDTxt.isCompound: "true"
*patientIDTxt.compoundIcon: "textfield.xpm"
*patientIDTxt.compoundName: "text_Field"
*patientIDTxt.x: 150
*patientIDTxt.y: 30
*patientIDTxt.height: 35
*patientIDTxt.topAttachment: "attach_opposite_widget"
*patientIDTxt.topOffset: 0
*patientIDTxt.topWidget: "label6"
*patientIDTxt.fontList: "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1"

*lastNameTxt.class: textField
*lastNameTxt.static: true
*lastNameTxt.name: lastNameTxt
*lastNameTxt.parent: form2
*lastNameTxt.width: 140
*lastNameTxt.isCompound: "true"
*lastNameTxt.compoundIcon: "textfield.xpm"
*lastNameTxt.compoundName: "text_Field"
*lastNameTxt.x: 150
*lastNameTxt.y: 79
*lastNameTxt.height: 35
*lastNameTxt.topAttachment: "attach_opposite_widget"
*lastNameTxt.topOffset: 0
*lastNameTxt.topWidget: "label7"
*lastNameTxt.fontList: "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1"

*birthDateTxt.class: textField
*birthDateTxt.static: true
*birthDateTxt.name: birthDateTxt
*birthDateTxt.parent: form2
*birthDateTxt.width: 140
*birthDateTxt.isCompound: "true"
*birthDateTxt.compoundIcon: "textfield.xpm"
*birthDateTxt.compoundName: "text_Field"
*birthDateTxt.x: 150
*birthDateTxt.y: 128
*birthDateTxt.height: 35
*birthDateTxt.topAttachment: "attach_opposite_widget"
*birthDateTxt.topOffset: 0
*birthDateTxt.topWidget: "label8"
*birthDateTxt.fontList: "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1"

*pushButton1.class: pushButton
*pushButton1.static: true
*pushButton1.name: pushButton1
*pushButton1.parent: form2
*pushButton1.isCompound: "true"
*pushButton1.compoundIcon: "push.xpm"
*pushButton1.compoundName: "push_Button"
*pushButton1.x: 38
*pushButton1.y: 148
*pushButton1.width: 250
*pushButton1.height: 30
*pushButton1.labelString: "Search"
*pushButton1.activateCallback: {\
CONDITION\
    cond;\
char\
    *txt;\
CONNECTION_MODULE\
    connection;\
PATIENT_QUERY_MODULE\
    query;\
char\
    buf[80];\
\
    copyWidgetText(imageServerNodeTxt, connection.NodeName);\
    copyWidgetText(imageServerPortTxt, connection.PortNumber);\
    copyWidgetText(calledAPTitleTxt,   connection.CalledAPTitle);\
    copyWidgetText(callingAPTitleTxt,  connection.CallingAPTitle);\
\
    copyWidgetText(patientIDTxt, query.PatientID);\
    copyWidgetText(lastNameTxt, query.LastName);\
    copyWidgetText(birthDateTxt, query.BirthDate);\
\
    if (patientList == NULL)\
	patientList = LST_Create();\
    if (studyList == NULL)\
	studyList = LST_Create();\
\
    listClear(&patientList);\
    cond = patientQuery(&connection, &query, &patientList);\
    COND_DumpConditions();\
    MUT_LoadList(patientListWidget, patientList, formatPatient, buf);\
\
    listClear(&studyList);\
    MUT_LoadList(studyListWidget, studyList, formatStudy, buf);\
}

*frame2.class: frame
*frame2.static: true
*frame2.name: frame2
*frame2.parent: drawingArea1
*frame2.width: 320
*frame2.height: 190
*frame2.isCompound: "true"
*frame2.compoundIcon: "frame.xpm"
*frame2.compoundName: "frame_"
*frame2.x: 10
*frame2.y: 0

*form1.class: form
*form1.static: true
*form1.name: form1
*form1.parent: frame2
*form1.width: 290
*form1.height: 166
*form1.resizePolicy: "resize_none"
*form1.isCompound: "true"
*form1.compoundIcon: "form.xpm"
*form1.compoundName: "form_"
*form1.x: 10
*form1.y: 20

*label2.class: label
*label2.static: true
*label2.name: label2
*label2.parent: form1
*label2.isCompound: "true"
*label2.compoundIcon: "label.xpm"
*label2.compoundName: "label_"
*label2.x: 10
*label2.y: 20
*label2.width: 120
*label2.height: 20
*label2.labelString: "Image Server Node:"

*label3.class: label
*label3.static: true
*label3.name: label3
*label3.parent: form1
*label3.isCompound: "true"
*label3.compoundIcon: "label.xpm"
*label3.compoundName: "label_"
*label3.x: 10
*label3.y: 55
*label3.width: 120
*label3.height: 20
*label3.labelString: "Image Server Port:"
*label3.leftAttachment: "attach_opposite_widget"
*label3.leftOffset: 0
*label3.leftWidget: "label2"
*label3.topAttachment: "attach_widget"
*label3.topOffset: 20
*label3.topWidget: "label2"

*label4.class: label
*label4.static: true
*label4.name: label4
*label4.parent: form1
*label4.isCompound: "true"
*label4.compoundIcon: "label.xpm"
*label4.compoundName: "label_"
*label4.x: 10
*label4.y: 90
*label4.width: 120
*label4.height: 20
*label4.labelString: "Called AP Title:"
*label4.leftAttachment: "attach_opposite_widget"
*label4.leftOffset: 0
*label4.leftWidget: "label2"
*label4.topAttachment: "attach_widget"
*label4.topOffset: 20
*label4.topWidget: "label3"

*label5.class: label
*label5.static: true
*label5.name: label5
*label5.parent: form1
*label5.isCompound: "true"
*label5.compoundIcon: "label.xpm"
*label5.compoundName: "label_"
*label5.x: 10
*label5.y: 125
*label5.width: 120
*label5.height: 20
*label5.labelString: "Calling AP Title:"
*label5.leftAttachment: "attach_opposite_widget"
*label5.leftOffset: 0
*label5.leftWidget: "label2"
*label5.topAttachment: "attach_widget"
*label5.topOffset: 20
*label5.topWidget: "label4"

*callingAPTitleTxt.class: textField
*callingAPTitleTxt.static: true
*callingAPTitleTxt.name: callingAPTitleTxt
*callingAPTitleTxt.parent: form1
*callingAPTitleTxt.width: 160
*callingAPTitleTxt.isCompound: "true"
*callingAPTitleTxt.compoundIcon: "textfield.xpm"
*callingAPTitleTxt.compoundName: "text_Field"
*callingAPTitleTxt.x: 150
*callingAPTitleTxt.y: 140
*callingAPTitleTxt.height: 40
*callingAPTitleTxt.topAttachment: "attach_opposite_widget"
*callingAPTitleTxt.topOffset: 0
*callingAPTitleTxt.topWidget: "label5"
*callingAPTitleTxt.fontList: "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1"

*calledAPTitleTxt.class: textField
*calledAPTitleTxt.static: true
*calledAPTitleTxt.name: calledAPTitleTxt
*calledAPTitleTxt.parent: form1
*calledAPTitleTxt.width: 160
*calledAPTitleTxt.isCompound: "true"
*calledAPTitleTxt.compoundIcon: "textfield.xpm"
*calledAPTitleTxt.compoundName: "text_Field"
*calledAPTitleTxt.x: 150
*calledAPTitleTxt.y: 100
*calledAPTitleTxt.height: 40
*calledAPTitleTxt.topAttachment: "attach_opposite_widget"
*calledAPTitleTxt.topOffset: 0
*calledAPTitleTxt.topWidget: "label4"
*calledAPTitleTxt.fontList: "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1"

*imageServerPortTxt.class: textField
*imageServerPortTxt.static: true
*imageServerPortTxt.name: imageServerPortTxt
*imageServerPortTxt.parent: form1
*imageServerPortTxt.width: 160
*imageServerPortTxt.isCompound: "true"
*imageServerPortTxt.compoundIcon: "textfield.xpm"
*imageServerPortTxt.compoundName: "text_Field"
*imageServerPortTxt.x: 150
*imageServerPortTxt.y: 60
*imageServerPortTxt.height: 40
*imageServerPortTxt.topAttachment: "attach_opposite_widget"
*imageServerPortTxt.topOffset: 0
*imageServerPortTxt.topWidget: "label3"
*imageServerPortTxt.fontList: "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1"

*imageServerNodeTxt.class: textField
*imageServerNodeTxt.static: true
*imageServerNodeTxt.name: imageServerNodeTxt
*imageServerNodeTxt.parent: form1
*imageServerNodeTxt.width: 160
*imageServerNodeTxt.isCompound: "true"
*imageServerNodeTxt.compoundIcon: "textfield.xpm"
*imageServerNodeTxt.compoundName: "text_Field"
*imageServerNodeTxt.x: 150
*imageServerNodeTxt.y: 20
*imageServerNodeTxt.height: 40
*imageServerNodeTxt.topAttachment: "attach_opposite_widget"
*imageServerNodeTxt.topOffset: 0
*imageServerNodeTxt.topWidget: "label2"
*imageServerNodeTxt.fontList: "-adobe-helvetica-medium-r-normal--14-100-100-100-p-76-iso8859-1"

*label1.class: label
*label1.static: true
*label1.name: label1
*label1.parent: mainWindow1

*menu1.class: rowColumn
*menu1.static: true
*menu1.name: menu1
*menu1.parent: mainWindow1
*menu1.rowColumnType: "menu_bar"

*menu1_p1.class: rowColumn
*menu1_p1.static: true
*menu1_p1.name: menu1_p1
*menu1_p1.parent: menu1
*menu1_p1.rowColumnType: "menu_pulldown"

*menu1_p1_b1.class: pushButton
*menu1_p1_b1.static: true
*menu1_p1_b1.name: menu1_p1_b1
*menu1_p1_b1.parent: menu1_p1
*menu1_p1_b1.labelString: "Quit"
*menu1_p1_b1.activateCallback: {\
    exit(0);\
}

*menu1_top_b1.class: cascadeButton
*menu1_top_b1.static: true
*menu1_top_b1.name: menu1_top_b1
*menu1_top_b1.parent: menu1
*menu1_top_b1.labelString: "File"
*menu1_top_b1.subMenuId: "menu1_p1"

