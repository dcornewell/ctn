! UIMX ascii 2.5 key: 5427                                                      

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
**                     Electronic Radiology Laboratory\
**                   Mallinckrodt Institute of Radiology\
**                Washington University School  of Medicine\
**\
** Module Name(s):		main\
**			activateCB_menu1_p1_b2\
**			browseSelectionCB_scrolledList1\
**			browseSelectionCB_scrolledList2	\
**			activateCB_pushButton1\
**			activateCB_pushButton2\
**			activateCB_pushButton3 \
**			activateCB_pushButton4\
**			network\
**			copyWtext\
**			copyWtext2\
**			load\
**			load_sopClasses\
**			addsop_scrolledList2\
**			selected_sop_fromList1\
**			update\
**			set_options\
**			set_options_toDefault\
**\
** Author:			Chander L. Sabharwal,  9-September-94\
**\
** Intent:			This program lets the user establish a  network\
**			 connection with a server and make an Association \
**			with it for the SOP service classes. It allows the \
**			user to select options as Service Class User SCU,\
**			SCP or SCU/SCP. It also lets the user select options\
**			for transfer syntaxes as LITTLEENDIAN, LITTLEENDIAN\
**			EXPLICIT and BIGENDIAN EXPLICIT.\
**\
** Last Update:		$Author: smm $, $Date: 1995-12-14 14:45:44 $\
**\
** Source File:		$Source: /home/smm/ctn/ctn/apps/assoc_tool/topLevelShell1.i,v $\
**\
** Revision:		$Revision: 1.5 $\
**\
** Status:			$State: Exp $\
**\
*/     \
static char rcsid[] = "$Revisin$ $RCSfile: topLevelShell1.i,v $";\
\
#include <stdio.h>\
#include <string.h>\
\
#include "dicom.h"\
#include "dicom_uids.h"\
#include "lst.h"\
#include "condition.h"\
#include "dulprotocol.h"\
#include "dicom_objects.h"\
#include "dicom_messages.h"\
#include "dicom_services.h"\
#include "dicom_ie.h"\
#include "mut.h"\
\
#include "structures.h"\
#include "prototypes.h"\
\
\
#define	MAXLEN	5000\
\
/* transfer syntaxes initialized to default */\
char	tSyntaxes[3][DUL_LEN_UID+1] = {\
			DICOM_TRANSFERLITTLEENDIAN,\
			"",\
			"",\
			};\
\
DUL_SC_ROLE		/* identifying the application as requestor,\
			 * acceptor or both, initialized to default */\
	role = DUL_SC_ROLE_DEFAULT;\
\
/* structure for linked list sopclassList  */\
typedef  struct{\
	void			*reserved[2];\
	char			sopclass[65];\
	char			sopclassW[65];\
}SOP_ELEMENT;\
\
LST_HEAD	*sopList = NULL;\
\
LST_HEAD	*selectedsopList = NULL;\
\
SELECTED_SOP	*se = NULL;\
\
#include "format.h"\
\
static int	itemNo_selected1;	/* item number selected fromlist1 */\
static int	itemNo_selected2;	/* item number selected fromlist2 */\
\
static char	*sopstr2;		/* character string containing sop classname from list2 */\
\
static char	*text_typed;	/* text typed in the textfield */\
\
static char	node[40];		/* the node we are calling*/\
\
static char	calledAPTitle[40];	/*  called application Title*/\
\
static char	callingAPTitle[40];	/*  calling application title */\
\
static char	port[40];		/* TCP port to establish Association */\
\
static char	buf_mes[65];
*topLevelShell1.ispecdecl:
*topLevelShell1.funcdecl: swidget create_topLevelShell1(swidget UxParent)
*topLevelShell1.funcname: create_topLevelShell1
*topLevelShell1.funcdef: "swidget", "<create_topLevelShell1>(%)"
*topLevelShell1.argdecl: swidget UxParent;
*topLevelShell1.arglist: UxParent
*topLevelShell1.arglist.UxParent: "swidget", "%UxParent%"
*topLevelShell1.icode:
*topLevelShell1.fcode: return(rtrn);\

*topLevelShell1.auxdecl: /* load\
**\
** Purpose:\
**	This function loads the list of SOP class in scrolled List1\
**\
** Parameter Dictionary:\
**	sopList	list containing the SOP class names\
**\
** Return Values:\
**	None\
**\
** Notes:\
**\
**Algorithm:\
**	Description of the algorithm (optional) and any other notes.\
**\
*/\
void  load(LST_HEAD  *sops)\
{\
  char	buf[65];\
\
   MUT_LoadList(scrolledList1, sops, formatsop, buf);\
}\
\
\
/* addsop_scrolledList2\
**\
** Purpose:\
**	This function adds the name of SOP class selected to \
**	scrolledList2\
**\
** Parameter Dictionary:\
**	sopstr	input character string containg the SOP class name\
**\
** Return Values:\
**	None\
**\
** Notes:\
**\
**Algorithm:\
**	Description of the algorithm (optional) and any other notes.\
**\
*/\
void    addsop_scrolledList2(char *sopstr)\
{\
 XmString		item;\
\
 item = XmStringCreateSimple(sopstr);\
 \
  XmListAddItem(scrolledList2,item,0);\
  XtFree(item);\
}\
\
\
/* copyWtext2\
**\
** Purpose:\
**	This function writes the information and error messages \
**	in the text widget.\
**\
** Parameter Dictionary:\
**	buffer	input character string containing the message\
**\
** Return Values:\
**	None\
**\
** Notes:\
**\
**Algorithm:\
**	Description of the algorithm (optional) and any other notes.\
**\
*/\
\
void  copyWtext2(char *buffer)\
{\
 XmTextSetString(scrolledText1, buffer);\
}\
\
/* copyWtext\
**\
** Purpose:\
**	This function writes the name of the selected SOP class\
**	in the textField window1 inorder to update the options to\
**	to be applied to this class or to delete this class from the\
**	selected list.\
**\
** Parameter Dictionary:\
**	buffer	input character string containg the SOP class name\
**\
** Return Values:\
**	None\
**\
** Notes:\
**\
**Algorithm:\
**	Description of the algorithm (optional) and any other notes.\
**\
*/\
\
void copyWtext(char  *buffer)\
{\
 XmTextSetString(textField1, buffer);\
}\
\
\
/* update\
**\
** Purpose:\
**	This function updates the service class role and transfer\
**	syntaxes for the selected class by checking the state of\
**	the toggle buttons.\
**\
** Parameter Dictionary:\
**	None\
**\
** Return Values:\
**	None\
**\
** Notes:\
**\
**Algorithm:\
**	Description of the algorithm (optional) and any other notes.\
**\
*/\
\
void  update()\
{\
  int	i;\
  SELECTED_SOP *s;\
\
    s = LST_Index(&selectedsopList, itemNo_selected2);\
    if (s == NULL)\
	return;\
\
    s->tSyntaxes[0][0] = '\0';\
    s->tSyntaxes[1][0] = '\0';\
    s->tSyntaxes[2][0] = '\0';\
\
    if(XmToggleButtonGadgetGetState(toggleButtonGadget2))\
	s->role = DUL_SC_ROLE_SCU;\
    else if(XmToggleButtonGadgetGetState(toggleButtonGadget3))\
	s->role = DUL_SC_ROLE_SCP;\
    else if(XmToggleButtonGadgetGetState(toggleButtonGadget4))\
	s->role = DUL_SC_ROLE_SCUSCP;\
    else if(XmToggleButtonGadgetGetState(toggleButtonGadget1))\
	s->role = DUL_SC_ROLE_DEFAULT;\
\
/* set transfer syntaxes */\
\
    i = 0;\
    if (XmToggleButtonGetState(toggleButtonGadget5))\
	strcpy(s->tSyntaxes[i++], DICOM_TRANSFERLITTLEENDIAN);\
\
    if (XmToggleButtonGadgetGetState(toggleButtonGadget6))\
	strcpy(s->tSyntaxes[i++], DICOM_TRANSFERLITTLEENDIANEXPLICIT);\
\
    if (XmToggleButtonGadgetGetState(toggleButtonGadget7))\
	strcpy(s->tSyntaxes[i++], DICOM_TRANSFERBIGENDIANEXPLICIT);\
}\
\
/* set_options\
**\
** Purpose:\
**	This function sets up the state of the toggle buttons\
**	according to the the service class role and transfer\
**	syntaxes already set for the selected class by checking \
**	their options.\
**\
** Parameter Dictionary:\
**	None\
**\
** Return Values:\
**	None\
**\
** Notes:\
**\
**Algorithm:\
**	Description of the algorithm (optional) and any other notes.\
**\
*/\
\
void  set_options()\
{\
  int	i;\
\
  se = LST_Head(&selectedsopList);\
  (void) LST_Position(&selectedsopList, se);\
\
   for(i=2;  i<= itemNo_selected2;  i++)\
   {\
     se = LST_Next(&selectedsopList);\
   }\
\
\
  if(se->flag == 0){\
   set_options_toDefault();\
  } else if( se->flag == 1) {\
    if (se->role == DUL_SC_ROLE_DEFAULT) {\
      XmToggleButtonGadgetSetState(toggleButtonGadget1, TRUE, 0);\
    } else {\
      XmToggleButtonGadgetSetState(toggleButtonGadget1, FALSE, 0);\
    }\
\
    if(se->role == DUL_SC_ROLE_SCU) {\
      XmToggleButtonGadgetSetState(toggleButtonGadget2, TRUE, 0);\
    } else {\
      XmToggleButtonGadgetSetState(toggleButtonGadget2, FALSE, 0);\
    }\
\
    if(se->role == DUL_SC_ROLE_SCP) {\
      XmToggleButtonGadgetSetState(toggleButtonGadget3, TRUE, 0);\
    } else {\
      XmToggleButtonGadgetSetState(toggleButtonGadget3, FALSE, 0);\
    }\
\
    if(se->role == DUL_SC_ROLE_SCUSCP) {\
      XmToggleButtonGadgetSetState(toggleButtonGadget4, TRUE, 0);\
    } else {\
      XmToggleButtonGadgetSetState(toggleButtonGadget4, FALSE, 0);\
    }\
\
/* set syntaxes */\
\
    if( strlen(se->tSyntaxes[0]) != 0) {\
      XmToggleButtonGadgetSetState(toggleButtonGadget5, TRUE, 0);\
    } else {\
      XmToggleButtonGadgetSetState(toggleButtonGadget5, FALSE, 0);\
    }\
\
    if( strlen(se->tSyntaxes[1] ) !=  0) {\
      XmToggleButtonGadgetSetState(toggleButtonGadget6, TRUE, 0);\
    } else {\
      XmToggleButtonGadgetSetState(toggleButtonGadget6, FALSE, 0);\
    }\
\
    if( strlen(se->tSyntaxes[2] ) != 0){\
      XmToggleButtonGadgetSetState(toggleButtonGadget7, TRUE, 0);\
    } else {\
      XmToggleButtonGadgetSetState(toggleButtonGadget7, FALSE, 0);\
    }\
  }\
  update();\
}\
\
/*delete_sop_fromList2 \
**\
** Purpose:\
**	This function deletes the name of SOP class from \
**	scrolledList2, clears the textwindow from classname.	\
**\
** Parameter Dictionary:\
**	None\
**\
** Return Values:\
**	None\
**\
** Notes:\
**\
**Algorithm:\
**	Description of the algorithm (optional) and any other notes.\
**\
*/\
void delete_sop_fromList2()\
{\
  memset(buf_mes, 0, sizeof(buf_mes));\
  XmTextSetString(textField6, buf_mes);\
\
  if(itemNo_selected2 != 0) {\
    XmListDeletePos(scrolledList2,itemNo_selected2);\
    strcpy(buf_mes, "");\
    copyWtext(buf_mes);\
\
    update_selected_sop_inList2(itemNo_selected2);\
    itemNo_selected2 = 0;\
    free(sopstr2);\
\
    if(strcmp(XmTextGetString(textField6)," SELECT AN ITEM FROM SCROLLEDLIST2 BEFORE APPLYING DELETE") == 0) {\
      strcpy(buf_mes, "");\
      XmTextSetString(textField6, buf_mes);\
    }\
    set_options_toDefault();\
  } else {\
    memset(buf_mes, 0, sizeof(buf_mes));\
    strcpy(buf_mes, " SELECT AN ITEM FROM SCROLLEDLIST2 BEFORE APPLYING DELETE");\
\
    XmTextSetString(textField6, buf_mes);\
  }\
}\
/*set_options_toDefault\
**\
** Purpose:\
**	This function sets the state of the toggle buttons\
**	to default.\
**\
** Parameter Dictionary:\
**	None\
**\
** Return Values:\
**	None\
**\
** Notes:\
**\
**Algorithm:\
**	Description of the algorithm (optional) and any other notes.\
**\
*/  \
\
void  set_options_toDefault()\
{\
  XmToggleButtonGadgetSetState(toggleButtonGadget1, TRUE, 0);\
  XmToggleButtonGadgetSetState(toggleButtonGadget5, TRUE, 0);\
\
  XmToggleButtonGadgetSetState(toggleButtonGadget2, FALSE, 0);\
  XmToggleButtonGadgetSetState(toggleButtonGadget3, FALSE, 0);\
  XmToggleButtonGadgetSetState(toggleButtonGadget4, FALSE, 0);\
  XmToggleButtonGadgetSetState(toggleButtonGadget6, FALSE, 0);\
  XmToggleButtonGadgetSetState(toggleButtonGadget7, FALSE, 0);\
}\
\
  
*topLevelShell1.static: true
*topLevelShell1.name: topLevelShell1
*topLevelShell1.parent: NO_PARENT
*topLevelShell1.parentExpression: UxParent
*topLevelShell1.width: 1070
*topLevelShell1.height: 840
*topLevelShell1.isCompound: "true"
*topLevelShell1.compoundIcon: "toplevelS.xpm"
*topLevelShell1.compoundName: "topLevel_Shell"
*topLevelShell1.x: 90
*topLevelShell1.y: 50
*topLevelShell1.iconName: "ASSOC_TOOL"

*mainWindow1.class: mainWindow
*mainWindow1.static: true
*mainWindow1.name: mainWindow1
*mainWindow1.parent: topLevelShell1
*mainWindow1.width: 1070
*mainWindow1.height: 840
*mainWindow1.isCompound: "true"
*mainWindow1.compoundIcon: "main.xpm"
*mainWindow1.compoundName: "main_Window"
*mainWindow1.compoundEditor: {\
extern swidget UxGUIMePopup UXPROTO((swidget, swidget, int, int));\
UxGUIMwePopup(UxThisWidget, UxGetParent(UxThisWidget));\
}
*mainWindow1.x: 0
*mainWindow1.y: 0
*mainWindow1.unitType: "pixels"

*menu1.class: rowColumn
*menu1.static: true
*menu1.name: menu1
*menu1.parent: mainWindow1
*menu1.rowColumnType: "menu_bar"
*menu1.menuAccelerator: "<KeyUp>F10"

*menu1_p1.class: rowColumn
*menu1_p1.static: true
*menu1_p1.name: menu1_p1
*menu1_p1.parent: menu1
*menu1_p1.rowColumnType: "menu_pulldown"

*menu1_p1_b1.class: separatorGadget
*menu1_p1_b1.static: true
*menu1_p1_b1.name: menu1_p1_b1
*menu1_p1_b1.parent: menu1_p1

*menu1_p1_b2.class: pushButton
*menu1_p1_b2.static: true
*menu1_p1_b2.name: menu1_p1_b2
*menu1_p1_b2.parent: menu1_p1
*menu1_p1_b2.labelString: "Quit"
*menu1_p1_b2.mnemonic: "Q"
*menu1_p1_b2.activateCallback: /* activateCB_menu1_p1_b2\
**\
** Purpose:\
**	This routine calls a function to close the network connection\
**\
** Parameter Dictionary:\
**	None\
**\
** Return Values:\
**	None\
**\
** Notes:\
**\
**Algorithm:\
**	Description of the algorithm (optional) and any other notes.\
**\
*/\
{\
  closeNetworkConnection();\
  exit(0);\
}

*menu1_top_b1.class: cascadeButton
*menu1_top_b1.static: true
*menu1_top_b1.name: menu1_top_b1
*menu1_top_b1.parent: menu1
*menu1_top_b1.labelString: "Control"
*menu1_top_b1.mnemonic: "C"
*menu1_top_b1.subMenuId: "menu1_p1"

*bulletinBoard1.class: bulletinBoard
*bulletinBoard1.static: true
*bulletinBoard1.name: bulletinBoard1
*bulletinBoard1.parent: mainWindow1

*scrolledWindowList1.class: scrolledWindow
*scrolledWindowList1.static: true
*scrolledWindowList1.name: scrolledWindowList1
*scrolledWindowList1.parent: bulletinBoard1
*scrolledWindowList1.scrollingPolicy: "application_defined"
*scrolledWindowList1.visualPolicy: "variable"
*scrolledWindowList1.scrollBarDisplayPolicy: "static"
*scrolledWindowList1.shadowThickness: 0
*scrolledWindowList1.isCompound: "true"
*scrolledWindowList1.compoundIcon: "scrllist.xpm"
*scrolledWindowList1.compoundName: "scrolled_List"
*scrolledWindowList1.x: 60
*scrolledWindowList1.y: 30

*scrolledList1.class: scrolledList
*scrolledList1.static: true
*scrolledList1.name: scrolledList1
*scrolledList1.parent: scrolledWindowList1
*scrolledList1.width: 440
*scrolledList1.height: 150
*scrolledList1.x: 0
*scrolledList1.y: 30
*scrolledList1.browseSelectionCallback: {\
/* browseSelectionCB_scrolledList1\
**\
** Purpose:\
**	This function selects an SOP class from scrolled List1\
**	and calls functions to add it in scrolledList2, and \
**	selectedsopList.\
**\
** Parameter Dictionary:\
**	None\
**\
** Return Values:\
**	None\
**\
** Notes:\
**\
**Algorithm:\
**	Description of the algorithm (optional) and any other notes.\
**\
*/\
\
XmListCallbackStruct	*cbs;\
char			*sopstr1;\
int			one;\
\
  cbs = (XmListCallbackStruct *)UxCallbackArg;\
\
  strcpy(buf_mes, "");\
\
  if( (sopstr1 = (char *) malloc(65)) == NULL) {\
    strcpy(buf_mes, "malloc sopstr1 failed in scrolledList1");\
    copyWtext2(buf_mes);\
    strcpy(buf_mes, "");\
  }\
\
  if(!XmStringGetLtoR(cbs->item, XmSTRING_DEFAULT_CHARSET, &sopstr1)) {\
    strcpy(buf_mes, "No SOP class selected from scrolledList1");\
    copyWtext2(buf_mes);\
    strcpy(buf_mes, "");\
  }\
\
  itemNo_selected1 = cbs->item_position;\
  one = itemNo_selected1;\
\
  addsop_scrolledList2(sopstr1);\
  selected_sop_fromList1(one);\
  free(sopstr1);\
}
*scrolledList1.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"
*scrolledList1.highlightOnEnter: "false"
*scrolledList1.selectionPolicy: "browse_select"

*scrolledWindowList2.class: scrolledWindow
*scrolledWindowList2.static: true
*scrolledWindowList2.name: scrolledWindowList2
*scrolledWindowList2.parent: bulletinBoard1
*scrolledWindowList2.scrollingPolicy: "application_defined"
*scrolledWindowList2.visualPolicy: "variable"
*scrolledWindowList2.scrollBarDisplayPolicy: "static"
*scrolledWindowList2.shadowThickness: 0
*scrolledWindowList2.isCompound: "true"
*scrolledWindowList2.compoundIcon: "scrllist.xpm"
*scrolledWindowList2.compoundName: "scrolled_List"
*scrolledWindowList2.x: 610
*scrolledWindowList2.y: 30

*scrolledWindowList4.class: scrolledWindow
*scrolledWindowList4.static: true
*scrolledWindowList4.name: scrolledWindowList4
*scrolledWindowList4.parent: scrolledWindowList2
*scrolledWindowList4.scrollingPolicy: "application_defined"
*scrolledWindowList4.visualPolicy: "variable"
*scrolledWindowList4.scrollBarDisplayPolicy: "static"
*scrolledWindowList4.shadowThickness: 0
*scrolledWindowList4.isCompound: "true"
*scrolledWindowList4.compoundIcon: "scrllist.xpm"
*scrolledWindowList4.compoundName: "scrolled_List"
*scrolledWindowList4.x: 0
*scrolledWindowList4.y: 0

*scrolledList2.class: scrolledList
*scrolledList2.static: true
*scrolledList2.name: scrolledList2
*scrolledList2.parent: scrolledWindowList4
*scrolledList2.width: 430
*scrolledList2.height: 150
*scrolledList2.x: 0
*scrolledList2.y: 30
*scrolledList2.browseSelectionCallback: {\
/* browseSelectionCB_scrolledList2\
**\
** Purpose:\
**	This function selects an SOP class from scrolledList2\
**	and calls function update to update the options on\
**	the toggleButtons.\
**\
** Parameter Dictionary:\
**	None\
**\
** Return Values:\
**	None\
**\
** Notes:\
**\
**Algorithm:\
**	Description of the algorithm (optional) and any other notes.\
**\
*/\
XmListCallbackStruct	*cbs;\
\
  cbs = (XmListCallbackStruct *)UxCallbackArg;\
\
  strcpy(buf_mes, "");\
\
  if( (sopstr2 = (char *) malloc(65)) == NULL) {\
    strcpy(buf_mes, " malloc sopstr2 failed in scrolledList2");\
    copyWtext2(buf_mes);\
    strcpy(buf_mes, "");\
  }\
\
  if(!XmStringGetLtoR(cbs->item, XmSTRING_DEFAULT_CHARSET, &sopstr2)) {\
    strcpy(buf_mes, "No sopclass selected from scrolledList2");\
    copyWtext2(buf_mes);\
    strcpy(buf_mes, "");\
  }\
\
  itemNo_selected2 = cbs->item_position;\
\
  set_options();\
\
  strcpy(buf_mes,sopstr2);\
  copyWtext(buf_mes);\
  strcpy(buf_mes, "");\
\
}
*scrolledList2.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"
*scrolledList2.highlightOnEnter: "false"
*scrolledList2.selectionPolicy: "browse_select"

*separatorGadget1.class: separatorGadget
*separatorGadget1.static: true
*separatorGadget1.name: separatorGadget1
*separatorGadget1.parent: bulletinBoard1
*separatorGadget1.width: 1030
*separatorGadget1.isCompound: "true"
*separatorGadget1.compoundIcon: "sepG.xpm"
*separatorGadget1.compoundName: "sepG_"
*separatorGadget1.x: 20
*separatorGadget1.y: 200
*separatorGadget1.height: 20

*labelGadget1.class: labelGadget
*labelGadget1.static: true
*labelGadget1.name: labelGadget1
*labelGadget1.parent: bulletinBoard1
*labelGadget1.isCompound: "true"
*labelGadget1.compoundIcon: "labelG.xpm"
*labelGadget1.compoundName: "labelG_"
*labelGadget1.x: 160
*labelGadget1.y: 230
*labelGadget1.width: 180
*labelGadget1.height: 30
*labelGadget1.labelString: "Selected SopClass:"
*labelGadget1.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"

*textField1.class: textField
*textField1.static: true
*textField1.name: textField1
*textField1.parent: bulletinBoard1
*textField1.width: 490
*textField1.isCompound: "true"
*textField1.compoundIcon: "textfield.xpm"
*textField1.compoundName: "text_Field"
*textField1.x: 330
*textField1.y: 230
*textField1.height: 40
*textField1.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"

*rowColumn1.class: rowColumn
*rowColumn1.static: true
*rowColumn1.name: rowColumn1
*rowColumn1.parent: bulletinBoard1
*rowColumn1.width: 228
*rowColumn1.height: 147
*rowColumn1.isCompound: "true"
*rowColumn1.compoundIcon: "row.xpm"
*rowColumn1.compoundName: "row_Column"
*rowColumn1.x: 200
*rowColumn1.y: 280
*rowColumn1.borderWidth: 0
*rowColumn1.radioBehavior: "true"

*toggleButtonGadget1.class: toggleButtonGadget
*toggleButtonGadget1.static: true
*toggleButtonGadget1.name: toggleButtonGadget1
*toggleButtonGadget1.parent: rowColumn1
*toggleButtonGadget1.isCompound: "true"
*toggleButtonGadget1.compoundIcon: "toggleG.xpm"
*toggleButtonGadget1.compoundName: "toggleB_Gadget"
*toggleButtonGadget1.x: 0
*toggleButtonGadget1.y: 0
*toggleButtonGadget1.width: 222
*toggleButtonGadget1.height: 33
*toggleButtonGadget1.labelString: "Default SCU"
*toggleButtonGadget1.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"
*toggleButtonGadget1.set: "true"

*toggleButtonGadget2.class: toggleButtonGadget
*toggleButtonGadget2.static: true
*toggleButtonGadget2.name: toggleButtonGadget2
*toggleButtonGadget2.parent: rowColumn1
*toggleButtonGadget2.isCompound: "true"
*toggleButtonGadget2.compoundIcon: "toggleG.xpm"
*toggleButtonGadget2.compoundName: "toggleB_Gadget"
*toggleButtonGadget2.x: 0
*toggleButtonGadget2.y: 10
*toggleButtonGadget2.width: 222
*toggleButtonGadget2.height: 33
*toggleButtonGadget2.labelString: "Service Class User SCU"
*toggleButtonGadget2.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"

*toggleButtonGadget3.class: toggleButtonGadget
*toggleButtonGadget3.static: true
*toggleButtonGadget3.name: toggleButtonGadget3
*toggleButtonGadget3.parent: rowColumn1
*toggleButtonGadget3.isCompound: "true"
*toggleButtonGadget3.compoundIcon: "toggleG.xpm"
*toggleButtonGadget3.compoundName: "toggleB_Gadget"
*toggleButtonGadget3.x: 0
*toggleButtonGadget3.y: 41
*toggleButtonGadget3.width: 222
*toggleButtonGadget3.height: 33
*toggleButtonGadget3.labelString: "Service Class Provider SCP"
*toggleButtonGadget3.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"

*toggleButtonGadget4.class: toggleButtonGadget
*toggleButtonGadget4.static: true
*toggleButtonGadget4.name: toggleButtonGadget4
*toggleButtonGadget4.parent: rowColumn1
*toggleButtonGadget4.isCompound: "true"
*toggleButtonGadget4.compoundIcon: "toggleG.xpm"
*toggleButtonGadget4.compoundName: "toggleB_Gadget"
*toggleButtonGadget4.x: 0
*toggleButtonGadget4.y: 72
*toggleButtonGadget4.width: 222
*toggleButtonGadget4.height: 33
*toggleButtonGadget4.labelString: "SCU/SCP"
*toggleButtonGadget4.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"

*toggleButtonGadget5.class: toggleButtonGadget
*toggleButtonGadget5.static: true
*toggleButtonGadget5.name: toggleButtonGadget5
*toggleButtonGadget5.parent: bulletinBoard1
*toggleButtonGadget5.isCompound: "true"
*toggleButtonGadget5.compoundIcon: "toggleG.xpm"
*toggleButtonGadget5.compoundName: "toggleB_Gadget"
*toggleButtonGadget5.x: 620
*toggleButtonGadget5.y: 280
*toggleButtonGadget5.width: 210
*toggleButtonGadget5.height: 30
*toggleButtonGadget5.labelString: "Implicit LITTLE ENDIAN"
*toggleButtonGadget5.set: "true"
*toggleButtonGadget5.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"

*toggleButtonGadget6.class: toggleButtonGadget
*toggleButtonGadget6.static: true
*toggleButtonGadget6.name: toggleButtonGadget6
*toggleButtonGadget6.parent: bulletinBoard1
*toggleButtonGadget6.isCompound: "true"
*toggleButtonGadget6.compoundIcon: "toggleG.xpm"
*toggleButtonGadget6.compoundName: "toggleB_Gadget"
*toggleButtonGadget6.x: 620
*toggleButtonGadget6.y: 330
*toggleButtonGadget6.width: 210
*toggleButtonGadget6.height: 30
*toggleButtonGadget6.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"
*toggleButtonGadget6.labelString: "Explicit LITTLE ENDIAN"

*toggleButtonGadget7.class: toggleButtonGadget
*toggleButtonGadget7.static: true
*toggleButtonGadget7.name: toggleButtonGadget7
*toggleButtonGadget7.parent: bulletinBoard1
*toggleButtonGadget7.isCompound: "true"
*toggleButtonGadget7.compoundIcon: "toggleG.xpm"
*toggleButtonGadget7.compoundName: "toggleB_Gadget"
*toggleButtonGadget7.x: 620
*toggleButtonGadget7.y: 370
*toggleButtonGadget7.width: 210
*toggleButtonGadget7.height: 30
*toggleButtonGadget7.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"
*toggleButtonGadget7.labelString: "Explicit BIG ENDIAN"

*separatorGadget2.class: separatorGadget
*separatorGadget2.static: true
*separatorGadget2.name: separatorGadget2
*separatorGadget2.parent: bulletinBoard1
*separatorGadget2.width: 1030
*separatorGadget2.isCompound: "true"
*separatorGadget2.compoundIcon: "sepG.xpm"
*separatorGadget2.compoundName: "sepG_"
*separatorGadget2.x: 20
*separatorGadget2.y: 470
*separatorGadget2.height: 10

*scrolledWindowText1.class: scrolledWindow
*scrolledWindowText1.static: true
*scrolledWindowText1.name: scrolledWindowText1
*scrolledWindowText1.parent: bulletinBoard1
*scrolledWindowText1.scrollingPolicy: "application_defined"
*scrolledWindowText1.visualPolicy: "variable"
*scrolledWindowText1.scrollBarDisplayPolicy: "static"
*scrolledWindowText1.isCompound: "true"
*scrolledWindowText1.compoundIcon: "scrltext.xpm"
*scrolledWindowText1.compoundName: "scrolled_Text"
*scrolledWindowText1.x: 90
*scrolledWindowText1.y: 640

*scrolledText1.class: scrolledText
*scrolledText1.static: true
*scrolledText1.name: scrolledText1
*scrolledText1.parent: scrolledWindowText1
*scrolledText1.width: 900
*scrolledText1.height: 100
*scrolledText1.activateCallback: {\
\
}
*scrolledText1.fontList: "-adobe-helvetica-bold-r-normal--12-120-75-75-p-70-iso8859-1"
*scrolledText1.editMode: "multi_line_edit"

*pushButton1.class: pushButton
*pushButton1.static: true
*pushButton1.name: pushButton1
*pushButton1.parent: bulletinBoard1
*pushButton1.isCompound: "true"
*pushButton1.compoundIcon: "push.xpm"
*pushButton1.compoundName: "push_Button"
*pushButton1.x: 270
*pushButton1.y: 770
*pushButton1.width: 130
*pushButton1.height: 30
*pushButton1.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"
*pushButton1.labelString: "Request"
*pushButton1.activateCallback: {\
/*activateCB_pushButton1 \
**\
** Purpose:\
**	This function reads association request parameters and \
**	requests an association\
**\
** Parameter Dictionary:\
**	None\
**\
** Return Values:\
**	None\
**\
** Notes:\
**\
**Algorithm:\
**	Description of the algorithm (optional) and any other notes.\
**\
*/\
\
  char buffer[512] = "";\
\
  text_typed = XmTextFieldGetString(textField2);\
  if(strlen(text_typed) != 0) {\
    strcpy(node,text_typed);\
    XtFree(text_typed);\
  } else {\
    strcat(buffer, "Association Request Parameters,  Node  not supplied !\n");\
  }\
\
  text_typed = XmTextFieldGetString(textField3);\
  if(strlen(text_typed) != 0) {\
    strcpy(port, text_typed);\
    XtFree(text_typed);\
  } else {\
    strcat(buffer, "Association Request Parameter,  PORT not supplied !\n");\
  }\
\
  text_typed = XmTextFieldGetString(textField4);\
  if(strlen(text_typed) != 0) {\
    strcpy(callingAPTitle, text_typed);\
    XtFree(text_typed);\
  } else {\
    strcat(buffer, "Association Request Parameter,  Calling App Title not supplied !\n");\
  }\
\
  text_typed = XmTextFieldGetString(textField5);\
  if(strlen(text_typed) != 0) {\
    strcpy(calledAPTitle, text_typed);\
    XtFree(text_typed);\
  } else {\
    strcat(buffer, "Association Request Parameter,  called App Title not supplied !\n");\
  }\
\
  if(strlen(buffer) != 0) {\
    copyWtext2(buffer);\
  } else {		\
    request_association(node,port,calledAPTitle,callingAPTitle);\
  }\
}

*pushButton2.class: pushButton
*pushButton2.static: true
*pushButton2.name: pushButton2
*pushButton2.parent: bulletinBoard1
*pushButton2.isCompound: "true"
*pushButton2.compoundIcon: "push.xpm"
*pushButton2.compoundName: "push_Button"
*pushButton2.x: 600
*pushButton2.y: 770
*pushButton2.width: 130
*pushButton2.height: 30
*pushButton2.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"
*pushButton2.labelString: "Release"
*pushButton2.activateCallback: {\
/*activateCB_pushButton2\
**\
** Purpose:\
**	This function requests for release of association\
**\
** Parameter Dictionary:\
**	None\
**\
** Return Values:\
**	None\
**\
** Notes:\
**\
**Algorithm:\
**	Description of the algorithm (optional) and any other notes.\
**\
*/\
request_release();\
}

*labelGadget2.class: labelGadget
*labelGadget2.static: true
*labelGadget2.name: labelGadget2
*labelGadget2.parent: bulletinBoard1
*labelGadget2.isCompound: "true"
*labelGadget2.compoundIcon: "labelG.xpm"
*labelGadget2.compoundName: "labelG_"
*labelGadget2.x: 180
*labelGadget2.y: 480
*labelGadget2.width: 270
*labelGadget2.height: 30
*labelGadget2.labelString: "Association Request Parameters"
*labelGadget2.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"

*labelGadget3.class: labelGadget
*labelGadget3.static: true
*labelGadget3.name: labelGadget3
*labelGadget3.parent: bulletinBoard1
*labelGadget3.isCompound: "true"
*labelGadget3.compoundIcon: "labelG.xpm"
*labelGadget3.compoundName: "labelG_"
*labelGadget3.x: 200
*labelGadget3.y: 530
*labelGadget3.width: 130
*labelGadget3.height: 30
*labelGadget3.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"
*labelGadget3.labelString: "Remote Node"

*labelGadget4.class: labelGadget
*labelGadget4.static: true
*labelGadget4.name: labelGadget4
*labelGadget4.parent: bulletinBoard1
*labelGadget4.isCompound: "true"
*labelGadget4.compoundIcon: "labelG.xpm"
*labelGadget4.compoundName: "labelG_"
*labelGadget4.x: 590
*labelGadget4.y: 530
*labelGadget4.width: 140
*labelGadget4.height: 30
*labelGadget4.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"
*labelGadget4.labelString: "Remote Port"

*labelGadget5.class: labelGadget
*labelGadget5.static: true
*labelGadget5.name: labelGadget5
*labelGadget5.parent: bulletinBoard1
*labelGadget5.isCompound: "true"
*labelGadget5.compoundIcon: "labelG.xpm"
*labelGadget5.compoundName: "labelG_"
*labelGadget5.x: 200
*labelGadget5.y: 580
*labelGadget5.width: 130
*labelGadget5.height: 30
*labelGadget5.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"
*labelGadget5.labelString: "Calling App Title"

*labelGadget6.class: labelGadget
*labelGadget6.static: true
*labelGadget6.name: labelGadget6
*labelGadget6.parent: bulletinBoard1
*labelGadget6.isCompound: "true"
*labelGadget6.compoundIcon: "labelG.xpm"
*labelGadget6.compoundName: "labelG_"
*labelGadget6.x: 590
*labelGadget6.y: 580
*labelGadget6.width: 140
*labelGadget6.height: 30
*labelGadget6.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"
*labelGadget6.labelString: "Called App Title"

*separatorGadget3.class: separatorGadget
*separatorGadget3.static: true
*separatorGadget3.name: separatorGadget3
*separatorGadget3.parent: bulletinBoard1
*separatorGadget3.width: 1050
*separatorGadget3.isCompound: "true"
*separatorGadget3.compoundIcon: "sepG.xpm"
*separatorGadget3.compoundName: "sepG_"
*separatorGadget3.x: 20
*separatorGadget3.y: 620
*separatorGadget3.height: 10

*textField2.class: textField
*textField2.static: true
*textField2.name: textField2
*textField2.parent: bulletinBoard1
*textField2.width: 160
*textField2.isCompound: "true"
*textField2.compoundIcon: "textfield.xpm"
*textField2.compoundName: "text_Field"
*textField2.x: 360
*textField2.y: 520
*textField2.height: 40
*textField2.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"

*textField3.class: textField
*textField3.static: true
*textField3.name: textField3
*textField3.parent: bulletinBoard1
*textField3.width: 160
*textField3.isCompound: "true"
*textField3.compoundIcon: "textfield.xpm"
*textField3.compoundName: "text_Field"
*textField3.x: 740
*textField3.y: 520
*textField3.height: 40
*textField3.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"

*textField4.class: textField
*textField4.static: true
*textField4.name: textField4
*textField4.parent: bulletinBoard1
*textField4.width: 160
*textField4.isCompound: "true"
*textField4.compoundIcon: "textfield.xpm"
*textField4.compoundName: "text_Field"
*textField4.x: 360
*textField4.y: 570
*textField4.height: 40
*textField4.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"

*textField5.class: textField
*textField5.static: true
*textField5.name: textField5
*textField5.parent: bulletinBoard1
*textField5.width: 160
*textField5.isCompound: "true"
*textField5.compoundIcon: "textfield.xpm"
*textField5.compoundName: "text_Field"
*textField5.x: 740
*textField5.y: 570
*textField5.height: 40
*textField5.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"

*pushButton3.class: pushButton
*pushButton3.static: true
*pushButton3.name: pushButton3
*pushButton3.parent: bulletinBoard1
*pushButton3.isCompound: "true"
*pushButton3.compoundIcon: "push.xpm"
*pushButton3.compoundName: "push_Button"
*pushButton3.x: 320
*pushButton3.y: 410
*pushButton3.width: 110
*pushButton3.height: 30
*pushButton3.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"
*pushButton3.labelString: "Update"
*pushButton3.activateCallback: {\
/*activateCB_pushButton3 \
**\
** Purpose:\
**	This function writes message in textField6 to select an \
**	SOP class from scrolledList2 before applying update, if\
**	the class is not already selected\
**\
** Parameter Dictionary:\
**	None\
**\
** Return Values:\
**	None\
**\
** Notes:\
**\
**Algorithm:\
**	Description of the algorithm (optional) and any other notes.\
**\
*/\
\
if(strlen(XmTextGetString(textField1)) != 0) {\
    XmTextSetString(textField6, "");\
    update();\
} else {\
    XmTextSetString(textField6,\
	"Select an item from list of requested SOP classes before applying UPDATE");\
}\
\
\
}

*pushButton4.class: pushButton
*pushButton4.static: true
*pushButton4.name: pushButton4
*pushButton4.parent: bulletinBoard1
*pushButton4.isCompound: "true"
*pushButton4.compoundIcon: "push.xpm"
*pushButton4.compoundName: "push_Button"
*pushButton4.x: 600
*pushButton4.y: 410
*pushButton4.width: 110
*pushButton4.height: 30
*pushButton4.fontList: "-adobe-helvetica-bold-r-normal--14-140-75-75-p-82-iso8859-1"
*pushButton4.labelString: "Delete"
*pushButton4.defaultButtonShadowThickness: 0
*pushButton4.activateCallback: {\
/*activateCB_pushButton4 \
**\
** Purpose:\
**	This function calls routine to delete selected sop class\
**	from scrolledList2\
**\
** Parameter Dictionary:\
**	None\
**\
** Return Values:\
**	None\
**\
** Notes:\
**\
**Algorithm:\
**	Description of the algorithm (optional) and any other notes.\
**\
*/\
\
delete_sop_fromList2();\
}

*textField6.class: textField
*textField6.static: true
*textField6.name: textField6
*textField6.parent: bulletinBoard1
*textField6.width: 600
*textField6.isCompound: "true"
*textField6.compoundIcon: "textfield.xpm"
*textField6.compoundName: "text_Field"
*textField6.x: 250
*textField6.y: 440
*textField6.height: 30
*textField6.fontList: "-adobe-helvetica-bold-r-normal--12-120-75-75-p-70-iso8859-1"

