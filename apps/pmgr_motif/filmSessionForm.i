! UIMX ascii 2.5 key: 8030                                                      

*filmSessionForm.class: formDialog
*filmSessionForm.gbldecl: /*\
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
** Module Name(s):	create_filmSessionForm\
**			makeFilmScreateSen\
** Author, Date:	Chander Sabharwal\
** Intent:		This module contains the user interface for\
**			managing a film session.  The user select\
**			parameters for a film session and can then\
**			issue a CREATE command to create a session on\
**			the printer.\
** Last Update:		$Author: smm $, $Date: 1995-04-10 17:30:44 $\
** Source File:		$RCSfile: filmSessionForm.i,v $\
** Revision:		$Revision: 1.16 $\
** Status:		$State: Exp $\
*/\
 \
static char rcsid[] = "$Revision: 1.16 $ $RCSfile: filmSessionForm.i,v $";\
\
\
#include <stdio.h>\
\
#include "dicom.h"\
#include "condition.h"\
#include "lst.h"\
#include "tbl.h"\
#include "dicom_uids.h"\
#include "dulprotocol.h"\
#include "dicom_objects.h"\
#include "dicom_print.h"\
#include "manage.h"\
#include "mut.h"\
\
#include "print_shared.h"\
#include "print_client_app.h"\
#define MAXLEN  5000\
\
extern DUL_ASSOCIATIONKEY *associationKey;\
extern DUL_ASSOCIATESERVICEPARAMETERS associationParameters;\
extern CTNBOOLEAN associationActive;\
\
extern Widget wFilmBoxForm;\
extern Widget wStudyListBoard;\
extern Widget  wImageAttribute;\
extern Widget  wFilmSessionForm;\
extern Widget wFilmInformationForm;\
\
static CONDITION readFilmSession(PRN_BASICFILMSESSION *f);\
\
void  server_changed_attribute_attribute_filmSessionForm();\
\
 char filmSessionSOPInstanceUID[DICOM_UI_LENGTH + 1];\
\
char   buf[MAXLEN];\
\
int    filmboxes;\
\
int filmboxcount = 1;\
\
void makeFilmScreateSen();\

*filmSessionForm.ispecdecl:
*filmSessionForm.funcdecl: swidget create_filmSessionForm(swidget UxParent)
*filmSessionForm.funcname: create_filmSessionForm
*filmSessionForm.funcdef: "swidget", "<create_filmSessionForm>(%)"
*filmSessionForm.argdecl: swidget UxParent;
*filmSessionForm.arglist: UxParent
*filmSessionForm.arglist.UxParent: "swidget", "%UxParent%"
*filmSessionForm.icode:
*filmSessionForm.fcode: return(rtrn);\

*filmSessionForm.auxdecl: /*readFilmSession\
**\
** Purpose:\
**	This function reads the attributes for filmSession\
**\
** Parameter Dictionary:\
**	f	pointer to the filmSession Attributes\
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
static CONDITION readFilmSession(PRN_BASICFILMSESSION *f)\
{\
  CONDITION  cond;\
  CTNBOOLEAN nullFlag;\
   int	memoryallocation;\
\
   \
    if (XmToggleButtonGetState(wCopiesOneToggle))\
	strcpy(f->copies, "1");\
    else\
	strcpy(f->copies, "2");\
\
    if (XmToggleButtonGetState(wPriorityLowToggle))\
	strcpy(f->priority, LOW);\
    else if (XmToggleButtonGetState(wPriorityMediumToggle))\
	strcpy(f->priority, MED);\
    else\
	strcpy(f->priority, HIGH);\
\
    if (XmToggleButtonGetState(wMediumPaperToggle))\
	strcpy(f->mediumType, PAPER);\
    else if (XmToggleButtonGetState(wMediumClearFilmToggle))\
	strcpy(f->mediumType, CLEAR_FILM);\
    else\
	strcpy(f->mediumType, BLUE_FILM);\
\
    if (XmToggleButtonGetState(wDestinationMagazineToggle))\
	strcpy(f->filmDestination, MAGAZINE);\
    else\
	strcpy(f->filmDestination, PROCESSOR);\
\
    f->bfsAttributeFlag = PRN_BFS_K_COPIES |\
	PRN_BFS_K_PRIORITY |\
	PRN_BFS_K_MEDIUMTYPE |\
	PRN_BFS_K_FILMDESTINATION ;\
\
    cond = MUT_ScanWidget(wFilmSessionLabelTxt, MUT_TEXT, &nullFlag,\
	f->filmSessionLabel);\
    if (cond == MUT_NORMAL && !nullFlag)\
	f->bfsAttributeFlag |= PRN_BFS_K_FILMSESSIONLABEL;\
\
    cond = MUT_ScanWidget(wMemoryAllocationTxt, MUT_INT, &nullFlag,\
			 &memoryallocation);\
    if (!nullFlag)\
   {\
    if (cond == MUT_NORMAL)\
    {\
       sprintf(f->memoryAllocation ,  "%-d", memoryallocation);\
	f->bfsAttributeFlag |= PRN_BFS_K_MEMORYALLOCATION;\
    }\
   else {\
	XmTextSetString(wErrorMesscrolledText1, \
		"Error reading Memory Allocation, not a proper Input\n");\
	return 0;\
  }\
}\
\
/* read number of filmboxes */\
    cond = MUT_ScanWidget(wNumberOfFilmBoxesTxt, MUT_INT, &nullFlag,\
	 &filmboxes);\
    if (cond != MUT_NORMAL ||  nullFlag)\
    {\
     XmTextSetString(wErrorMesscrolledText1," Error Obtaining number of filmboxes.");\
     return 0;\
    }   \
\
    f->type = PRN_K_BASICFILMSESSION;\
    return 1; \
}\
\
static void\
server_changed_attribute_filmSessionForm(PRN_BASICFILMSESSION filmSession,\
                                        PRN_BASICFILMSESSION filmSession0)\
{\
\
 int  index;\
\
     static long filmSessionFlagList[] = {\
        PRN_BFS_K_COPIES,\
        PRN_BFS_K_PRIORITY,\
        PRN_BFS_K_MEDIUMTYPE,\
        PRN_BFS_K_FILMDESTINATION,\
        PRN_BFS_K_FILMSESSIONLABEL,\
        PRN_BFS_K_MEMORYALLOCATION,\
        PRN_BFS_K_REFERENCEDBFBSEQ\
    };\
\
\
 buf[0] = '\0';\
/* If the Attribute is changed from SCP(Server) side, print a message */\
/* and change the value in the form */\
\
    for (index = 0; index < (int) DIM_OF(filmSessionFlagList); index++) {\
        if (filmSession.bfsAttributeFlag & filmSessionFlagList[index]) {\
                switch (filmSessionFlagList[index]) {\
\
\
/* Check number of copies */\
    case PRN_BFS_K_COPIES:\
    if(strcmp(filmSession.copies , filmSession.copies) != 0){\
     strcat(buf, " Server changed the attribute:  Copies\n");\
    if(strcmp(filmSession.copies, "1") == 0)\
     XmToggleButtonSetState(wCopiesTwoToggle, TRUE, TRUE);\
    else\
    if(strcmp(filmSession.copies, "2") == 0)\
     XmToggleButtonSetState(wCopiesOneToggle, TRUE, TRUE);  \
\
   } \
   break;\
/* Check priority */\
    case PRN_BFS_K_PRIORITY:\
     if(strcmp(filmSession0.priority, filmSession.priority) != 0){\
       strcat(buf, " Server changed the attribute:  Priority\n");\
     if(strcmp(filmSession.priority, "LOW") == 0)\
        XmToggleButtonSetState(wPriorityLowToggle, TRUE, TRUE); \
     else if (strcmp(filmSession.priority, "MED") == 0)\
        XmToggleButtonSetState(wPriorityMediumToggle, TRUE, TRUE); \
     else if (strcmp(filmSession.priority, "HIGH") == 0)\
        XmToggleButtonSetState(wPriorityHighToggle, TRUE, TRUE); \
   }\
   break;\
\
/* Check mediumType */\
   case PRN_BFS_K_MEDIUMTYPE:\
\
    if(strcmp(filmSession0.mediumType, filmSession.mediumType) != 0){\
      strcat(buf, " Server changed the attribute:  Medium Type\n");\
    if(strcmp(filmSession.mediumType, PAPER) == 0)\
      XmToggleButtonSetState(wMediumPaperToggle, TRUE, TRUE); \
    else if (strcmp(filmSession.mediumType, CLEAR_FILM) == 0)\
       XmToggleButtonSetState(wMediumClearFilmToggle, TRUE, TRUE); \
     else if (strcmp(filmSession.mediumType, BLUE_FILM) == 0)\
       XmToggleButtonSetState(wMediumBlueFilmToggle, TRUE, TRUE); \
 }\
   break;\
/* Check filmDestination */\
    case PRN_BFS_K_FILMDESTINATION:\
    if(strcmp(filmSession0.filmDestination, filmSession.filmDestination) != 0){\
     strcat(buf, " Server changed the attribute:  Destination\n");\
   if(strcmp(filmSession.filmDestination,"MAGAZINE") == 0)\
    XmToggleButtonSetState(wDestinationMagazineToggle, TRUE, TRUE);\
    else\
      if(strcmp(filmSession.filmDestination,"PROCESSOR") == 0)\
       XmToggleButtonSetState(wDestinationProcessorToggle, TRUE, TRUE); \
  } \
   break;\
\
/* Check filmSessionLabel*/\
    case PRN_BFS_K_FILMSESSIONLABEL:\
    if(strcmp(filmSession0.filmSessionLabel, filmSession.filmSessionLabel) != 0){\
         strcat(buf, " Server changed the attribute:  Film Session Label\n");\
      XmTextSetString(wFilmSessionLabelTxt,filmSession.filmSessionLabel);\
    }\
\
 break;                               \
/* Check memoryAllocation */\
    case PRN_BFS_K_MEMORYALLOCATION:\
     if(strcmp(filmSession0.memoryAllocation, filmSession.memoryAllocation) != 0){\
        strcat(buf, " Server changed the attribute:  Memory Allocation\n");\
        XmTextSetString(wMemoryAllocationTxt,filmSession.memoryAllocation);\
     }          \
  break;\
  }\
 }\
}\
  XmTextSetString(wErrorMesscrolledText1, buf); \
}\
\
\
void makeFilmScreateSen()\
{\
  XtVaSetValues(pushButton2, XmNsensitive, TRUE, NULL);\
}\

*filmSessionForm.static: true
*filmSessionForm.name: filmSessionForm
*filmSessionForm.parent: NO_PARENT
*filmSessionForm.parentExpression: UxParent
*filmSessionForm.defaultShell: topLevelShell
*filmSessionForm.width: 824
*filmSessionForm.height: 723
*filmSessionForm.isCompound: "true"
*filmSessionForm.compoundIcon: "formD.xpm"
*filmSessionForm.compoundName: "form_Dialog"
*filmSessionForm.x: 433
*filmSessionForm.y: 85
*filmSessionForm.unitType: "pixels"
*filmSessionForm.autoUnmanage: "false"

*label10.class: label
*label10.static: true
*label10.name: label10
*label10.parent: filmSessionForm
*label10.isCompound: "true"
*label10.compoundIcon: "label.xpm"
*label10.compoundName: "label_"
*label10.x: 80
*label10.y: 20
*label10.width: 310
*label10.height: 40
*label10.labelString: "Basic Film Session"

*rowColumn1.class: rowColumn
*rowColumn1.static: true
*rowColumn1.name: rowColumn1
*rowColumn1.parent: filmSessionForm
*rowColumn1.width: 280
*rowColumn1.height: 61
*rowColumn1.isCompound: "true"
*rowColumn1.compoundIcon: "row.xpm"
*rowColumn1.compoundName: "row_Column"
*rowColumn1.x: 270
*rowColumn1.y: 70
*rowColumn1.radioBehavior: "true"
*rowColumn1.labelString: ""

*wCopiesOneToggle.class: toggleButton
*wCopiesOneToggle.static: true
*wCopiesOneToggle.name: wCopiesOneToggle
*wCopiesOneToggle.parent: rowColumn1
*wCopiesOneToggle.isCompound: "true"
*wCopiesOneToggle.compoundIcon: "toggle.xpm"
*wCopiesOneToggle.compoundName: "toggle_Button"
*wCopiesOneToggle.x: 3
*wCopiesOneToggle.y: -50
*wCopiesOneToggle.width: 109
*wCopiesOneToggle.height: 79
*wCopiesOneToggle.labelString: "1"
*wCopiesOneToggle.set: "true"

*wCopiesTwoToggle.class: toggleButton
*wCopiesTwoToggle.static: true
*wCopiesTwoToggle.name: wCopiesTwoToggle
*wCopiesTwoToggle.parent: rowColumn1
*wCopiesTwoToggle.isCompound: "true"
*wCopiesTwoToggle.compoundIcon: "toggle.xpm"
*wCopiesTwoToggle.compoundName: "toggle_Button"
*wCopiesTwoToggle.x: 0
*wCopiesTwoToggle.y: 0
*wCopiesTwoToggle.width: 110
*wCopiesTwoToggle.height: 30
*wCopiesTwoToggle.labelString: "2"

*label11.class: label
*label11.static: true
*label11.name: label11
*label11.parent: filmSessionForm
*label11.isCompound: "true"
*label11.compoundIcon: "label.xpm"
*label11.compoundName: "label_"
*label11.x: 40
*label11.y: 80
*label11.width: 200
*label11.height: 40
*label11.labelString: "Copies"

*label12.class: label
*label12.static: true
*label12.name: label12
*label12.parent: filmSessionForm
*label12.isCompound: "true"
*label12.compoundIcon: "label.xpm"
*label12.compoundName: "label_"
*label12.x: 40
*label12.y: 170
*label12.width: 200
*label12.height: 40
*label12.labelString: "Print Priority"

*rowColumn2.class: rowColumn
*rowColumn2.static: true
*rowColumn2.name: rowColumn2
*rowColumn2.parent: filmSessionForm
*rowColumn2.width: 280
*rowColumn2.height: 61
*rowColumn2.isCompound: "true"
*rowColumn2.compoundIcon: "row.xpm"
*rowColumn2.compoundName: "row_Column"
*rowColumn2.x: 270
*rowColumn2.y: 140
*rowColumn2.radioBehavior: "true"
*rowColumn2.labelString: ""

*wPriorityLowToggle.class: toggleButton
*wPriorityLowToggle.static: true
*wPriorityLowToggle.name: wPriorityLowToggle
*wPriorityLowToggle.parent: rowColumn2
*wPriorityLowToggle.isCompound: "true"
*wPriorityLowToggle.compoundIcon: "toggle.xpm"
*wPriorityLowToggle.compoundName: "toggle_Button"
*wPriorityLowToggle.x: 40
*wPriorityLowToggle.y: 3
*wPriorityLowToggle.width: 35
*wPriorityLowToggle.height: 26
*wPriorityLowToggle.labelString: "Low"
*wPriorityLowToggle.set: "true"

*wPriorityMediumToggle.class: toggleButton
*wPriorityMediumToggle.static: true
*wPriorityMediumToggle.name: wPriorityMediumToggle
*wPriorityMediumToggle.parent: rowColumn2
*wPriorityMediumToggle.isCompound: "true"
*wPriorityMediumToggle.compoundIcon: "toggle.xpm"
*wPriorityMediumToggle.compoundName: "toggle_Button"
*wPriorityMediumToggle.x: 0
*wPriorityMediumToggle.y: 0
*wPriorityMediumToggle.width: 110
*wPriorityMediumToggle.height: 30
*wPriorityMediumToggle.labelString: "Medium"

*wPriorityHighToggle.class: toggleButton
*wPriorityHighToggle.static: true
*wPriorityHighToggle.name: wPriorityHighToggle
*wPriorityHighToggle.parent: rowColumn2
*wPriorityHighToggle.isCompound: "true"
*wPriorityHighToggle.compoundIcon: "toggle.xpm"
*wPriorityHighToggle.compoundName: "toggle_Button"
*wPriorityHighToggle.x: 3
*wPriorityHighToggle.y: -50
*wPriorityHighToggle.width: 109
*wPriorityHighToggle.height: 79
*wPriorityHighToggle.labelString: "High"

*label13.class: label
*label13.static: true
*label13.name: label13
*label13.parent: filmSessionForm
*label13.isCompound: "true"
*label13.compoundIcon: "label.xpm"
*label13.compoundName: "label_"
*label13.x: 40
*label13.y: 260
*label13.width: 200
*label13.height: 40
*label13.labelString: "Medium Type"

*rowColumn3.class: rowColumn
*rowColumn3.static: true
*rowColumn3.name: rowColumn3
*rowColumn3.parent: filmSessionForm
*rowColumn3.width: 280
*rowColumn3.height: 61
*rowColumn3.isCompound: "true"
*rowColumn3.compoundIcon: "row.xpm"
*rowColumn3.compoundName: "row_Column"
*rowColumn3.x: 267
*rowColumn3.y: 248
*rowColumn3.radioBehavior: "true"
*rowColumn3.labelString: ""

*wMediumPaperToggle.class: toggleButton
*wMediumPaperToggle.static: true
*wMediumPaperToggle.name: wMediumPaperToggle
*wMediumPaperToggle.parent: rowColumn3
*wMediumPaperToggle.isCompound: "true"
*wMediumPaperToggle.compoundIcon: "toggle.xpm"
*wMediumPaperToggle.compoundName: "toggle_Button"
*wMediumPaperToggle.x: 3
*wMediumPaperToggle.y: 3
*wMediumPaperToggle.width: 77
*wMediumPaperToggle.height: 79
*wMediumPaperToggle.labelString: "Paper"
*wMediumPaperToggle.set: "true"

*wMediumClearFilmToggle.class: toggleButton
*wMediumClearFilmToggle.static: true
*wMediumClearFilmToggle.name: wMediumClearFilmToggle
*wMediumClearFilmToggle.parent: rowColumn3
*wMediumClearFilmToggle.isCompound: "true"
*wMediumClearFilmToggle.compoundIcon: "toggle.xpm"
*wMediumClearFilmToggle.compoundName: "toggle_Button"
*wMediumClearFilmToggle.x: 0
*wMediumClearFilmToggle.y: 0
*wMediumClearFilmToggle.width: 110
*wMediumClearFilmToggle.height: 30
*wMediumClearFilmToggle.labelString: "Clear Film"
*wMediumClearFilmToggle.set: "false"

*wMediumBlueFilmToggle.class: toggleButton
*wMediumBlueFilmToggle.static: true
*wMediumBlueFilmToggle.name: wMediumBlueFilmToggle
*wMediumBlueFilmToggle.parent: rowColumn3
*wMediumBlueFilmToggle.isCompound: "true"
*wMediumBlueFilmToggle.compoundIcon: "toggle.xpm"
*wMediumBlueFilmToggle.compoundName: "toggle_Button"
*wMediumBlueFilmToggle.x: 3
*wMediumBlueFilmToggle.y: -50
*wMediumBlueFilmToggle.width: 109
*wMediumBlueFilmToggle.height: 79
*wMediumBlueFilmToggle.labelString: "Blue Film"

*label14.class: label
*label14.static: true
*label14.name: label14
*label14.parent: filmSessionForm
*label14.isCompound: "true"
*label14.compoundIcon: "label.xpm"
*label14.compoundName: "label_"
*label14.x: 40
*label14.y: 370
*label14.width: 200
*label14.height: 40
*label14.labelString: "Film Destination"

*rowColumn4.class: rowColumn
*rowColumn4.static: true
*rowColumn4.name: rowColumn4
*rowColumn4.parent: filmSessionForm
*rowColumn4.width: 280
*rowColumn4.height: 61
*rowColumn4.isCompound: "true"
*rowColumn4.compoundIcon: "row.xpm"
*rowColumn4.compoundName: "row_Column"
*rowColumn4.x: 270
*rowColumn4.y: 360
*rowColumn4.radioBehavior: "true"
*rowColumn4.labelString: ""

*wDestinationMagazineToggle.class: toggleButton
*wDestinationMagazineToggle.static: true
*wDestinationMagazineToggle.name: wDestinationMagazineToggle
*wDestinationMagazineToggle.parent: rowColumn4
*wDestinationMagazineToggle.isCompound: "true"
*wDestinationMagazineToggle.compoundIcon: "toggle.xpm"
*wDestinationMagazineToggle.compoundName: "toggle_Button"
*wDestinationMagazineToggle.x: 3
*wDestinationMagazineToggle.y: -50
*wDestinationMagazineToggle.width: 109
*wDestinationMagazineToggle.height: 79
*wDestinationMagazineToggle.labelString: "Magazine"
*wDestinationMagazineToggle.set: "true"

*wDestinationProcessorToggle.class: toggleButton
*wDestinationProcessorToggle.static: true
*wDestinationProcessorToggle.name: wDestinationProcessorToggle
*wDestinationProcessorToggle.parent: rowColumn4
*wDestinationProcessorToggle.isCompound: "true"
*wDestinationProcessorToggle.compoundIcon: "toggle.xpm"
*wDestinationProcessorToggle.compoundName: "toggle_Button"
*wDestinationProcessorToggle.x: 0
*wDestinationProcessorToggle.y: 0
*wDestinationProcessorToggle.width: 110
*wDestinationProcessorToggle.height: 30
*wDestinationProcessorToggle.labelString: "Processor"

*label15.class: label
*label15.static: true
*label15.name: label15
*label15.parent: filmSessionForm
*label15.isCompound: "true"
*label15.compoundIcon: "label.xpm"
*label15.compoundName: "label_"
*label15.x: 40
*label15.y: 440
*label15.width: 200
*label15.height: 40
*label15.labelString: "Number of Film Boxes"

*wNumberOfFilmBoxesTxt.class: textField
*wNumberOfFilmBoxesTxt.static: true
*wNumberOfFilmBoxesTxt.name: wNumberOfFilmBoxesTxt
*wNumberOfFilmBoxesTxt.parent: filmSessionForm
*wNumberOfFilmBoxesTxt.width: 120
*wNumberOfFilmBoxesTxt.isCompound: "true"
*wNumberOfFilmBoxesTxt.compoundIcon: "textfield.xpm"
*wNumberOfFilmBoxesTxt.compoundName: "text_Field"
*wNumberOfFilmBoxesTxt.x: 280
*wNumberOfFilmBoxesTxt.y: 440
*wNumberOfFilmBoxesTxt.height: 40
*wNumberOfFilmBoxesTxt.text: "1"

*pushButton2.class: pushButton
*pushButton2.static: true
*pushButton2.name: pushButton2
*pushButton2.parent: filmSessionForm
*pushButton2.isCompound: "true"
*pushButton2.compoundIcon: "push.xpm"
*pushButton2.compoundName: "push_Button"
*pushButton2.x: 150
*pushButton2.y: 510
*pushButton2.width: 150
*pushButton2.height: 50
*pushButton2.labelString: "Create"
*pushButton2.activateCallback: {\
  CONDITION cond;\
  unsigned short status;\
\
  PRN_BASICFILMSESSION filmSession;\
  PRN_BASICFILMSESSION filmSession0;	/*saving the copy of SCU Attributes */\
\
\
/* read FilmSession form */\
    cond = readFilmSession(&filmSession);\
    if (cond != 1){\
	return;\
}\
/* Save a copy of attributes sent by the client(SCU) */\
    filmSession0 = filmSession;\
\
    strcpy(filmSession.filmSessionSOPInstanceUID, "");\
\
    cond = createFilmSession(&associationKey, &associationParameters,\
	&filmSession, DICOM_SOPCLASSGREYSCALEPRINTMGMTMETA,\
	&status);\
\
    if (cond != APP_NORMAL) {\
	MUT_DisplayConditionText(wErrorMesscrolledText1);\
	(void) COND_PopCondition(TRUE);\
    }\
\
   (void) strcpy(filmSessionSOPInstanceUID,\
	         filmSession.filmSessionSOPInstanceUID);\
\
server_changed_attribute_filmSessionForm(filmSession,filmSession0);\
 \
#ifdef RUNTIME\
    if (wFilmBoxForm == 0)\
	wFilmBoxForm = (Widget) create_filmBoxForm(NULL);\
\
    XtManageChild(wFilmBoxForm);\
#endif\
\
XtVaSetValues(pushButton2, XmNsensitive, FALSE, NULL);\
}

*pushButton3.class: pushButton
*pushButton3.static: true
*pushButton3.name: pushButton3
*pushButton3.parent: filmSessionForm
*pushButton3.isCompound: "true"
*pushButton3.compoundIcon: "push.xpm"
*pushButton3.compoundName: "push_Button"
*pushButton3.x: 550
*pushButton3.y: 510
*pushButton3.width: 150
*pushButton3.height: 50
*pushButton3.labelString: "Cancel"
*pushButton3.activateCallback: {\
  CONDITION cond;\
\
    if (associationActive) {\
	cond = DUL_ReleaseAssociation(&associationKey);\
	if (cond != DUL_RELEASECONFIRMED)\
	    (void) DUL_AbortAssociation(&associationKey);\
\
	(void) DUL_DropAssociation(&associationKey);\
	(void) DUL_ClearServiceParameters(&associationParameters);\
    }\
    associationActive = FALSE;\
\
    XtUnmanageChild(wFilmSessionForm);\
    if (wFilmInformationForm != 0)\
	XtUnmanageChild(wFilmInformationForm);\
\
}

*wFilmSessionLabelTxt.class: textField
*wFilmSessionLabelTxt.static: true
*wFilmSessionLabelTxt.name: wFilmSessionLabelTxt
*wFilmSessionLabelTxt.parent: filmSessionForm
*wFilmSessionLabelTxt.width: 250
*wFilmSessionLabelTxt.isCompound: "true"
*wFilmSessionLabelTxt.compoundIcon: "textfield.xpm"
*wFilmSessionLabelTxt.compoundName: "text_Field"
*wFilmSessionLabelTxt.x: 560
*wFilmSessionLabelTxt.y: 70
*wFilmSessionLabelTxt.height: 40

*wMemoryAllocationTxt.class: textField
*wMemoryAllocationTxt.static: true
*wMemoryAllocationTxt.name: wMemoryAllocationTxt
*wMemoryAllocationTxt.parent: filmSessionForm
*wMemoryAllocationTxt.width: 250
*wMemoryAllocationTxt.isCompound: "true"
*wMemoryAllocationTxt.compoundIcon: "textfield.xpm"
*wMemoryAllocationTxt.compoundName: "text_Field"
*wMemoryAllocationTxt.x: 560
*wMemoryAllocationTxt.y: 140
*wMemoryAllocationTxt.height: 40

*label26.class: label
*label26.static: true
*label26.name: label26
*label26.parent: filmSessionForm
*label26.isCompound: "true"
*label26.compoundIcon: "label.xpm"
*label26.compoundName: "label_"
*label26.x: 400
*label26.y: 70
*label26.width: 150
*label26.height: 30
*label26.labelString: "Film Session Label"

*label27.class: label
*label27.static: true
*label27.name: label27
*label27.parent: filmSessionForm
*label27.isCompound: "true"
*label27.compoundIcon: "label.xpm"
*label27.compoundName: "label_"
*label27.x: 400
*label27.y: 140
*label27.width: 150
*label27.height: 30
*label27.labelString: "Memory Allocation"

*scrolledWindowText2.class: scrolledWindow
*scrolledWindowText2.static: true
*scrolledWindowText2.name: scrolledWindowText2
*scrolledWindowText2.parent: filmSessionForm
*scrolledWindowText2.scrollingPolicy: "application_defined"
*scrolledWindowText2.visualPolicy: "variable"
*scrolledWindowText2.scrollBarDisplayPolicy: "static"
*scrolledWindowText2.isCompound: "true"
*scrolledWindowText2.compoundIcon: "scrltext.xpm"
*scrolledWindowText2.compoundName: "scrolled_Text"
*scrolledWindowText2.x: 140
*scrolledWindowText2.y: 590

*scrolledWindowText4.class: scrolledWindow
*scrolledWindowText4.static: true
*scrolledWindowText4.name: scrolledWindowText4
*scrolledWindowText4.parent: filmSessionForm
*scrolledWindowText4.scrollingPolicy: "application_defined"
*scrolledWindowText4.visualPolicy: "variable"
*scrolledWindowText4.scrollBarDisplayPolicy: "static"
*scrolledWindowText4.isCompound: "true"
*scrolledWindowText4.compoundIcon: "scrltext.xpm"
*scrolledWindowText4.compoundName: "scrolled_Text"
*scrolledWindowText4.x: 240
*scrolledWindowText4.y: 580

*wErrorMesscrolledText1.class: scrolledText
*wErrorMesscrolledText1.static: true
*wErrorMesscrolledText1.name: wErrorMesscrolledText1
*wErrorMesscrolledText1.parent: scrolledWindowText4
*wErrorMesscrolledText1.width: 450
*wErrorMesscrolledText1.height: 80
*wErrorMesscrolledText1.editMode: "multi_line_edit"

