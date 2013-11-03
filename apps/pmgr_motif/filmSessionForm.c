
/*******************************************************************************
	filmSessionForm.c

       Associated Header file: filmSessionForm.h
*******************************************************************************/

#include "ctn_os.h"
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/Text.h>
#include <Xm/ScrolledW.h>
#include <Xm/PushB.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>
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
** Module Name(s):	create_filmSessionForm
**			makeFilmScreateSen
** Author, Date:	Chander Sabharwal
** Intent:		This module contains the user interface for
**			managing a film session.  The user select
**			parameters for a film session and can then
**			issue a CREATE command to create a session on
**			the printer.
** Last Update:		$Author: smm $, $Date: 2002-10-31 23:36:06 $
** Source File:		$RCSfile: filmSessionForm.c,v $
** Revision:		$Revision: 1.18 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.18 $ $RCSfile: filmSessionForm.c,v $";



#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "tbl.h"
#include "dicom_uids.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_print.h"
#include "manage.h"
#include "mut.h"

#include "print_shared.h"
#include "print_client_app.h"
#define MAXLEN  5000

extern DUL_ASSOCIATIONKEY *associationKey;
extern DUL_ASSOCIATESERVICEPARAMETERS associationParameters;
extern CTNBOOLEAN associationActive;

extern Widget wFilmBoxForm;
extern Widget wStudyListBoard;
extern Widget wImageAttribute;
extern Widget wFilmSessionForm;
extern Widget wFilmInformationForm;

static CONDITION readFilmSession(PRN_BASICFILMSESSION * f);

void server_changed_attribute_attribute_filmSessionForm();

char filmSessionSOPInstanceUID[DICOM_UI_LENGTH + 1];

char buf[MAXLEN];

int filmboxes;

int filmboxcount = 1;

void makeFilmScreateSen();


/*******************************************************************************
       The following header file defines the context structure.
*******************************************************************************/

#define CONTEXT_MACRO_ACCESS 1
#include "filmSessionForm.h"
#undef CONTEXT_MACRO_ACCESS


/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

/*readFilmSession
**
** Purpose:
**	This function reads the attributes for filmSession
**
** Parameter Dictionary:
**	f	pointer to the filmSession Attributes
**
** Return Values:
**	None
**
** Notes:
**
**Algorithm:
**	Description of the algorithm (optional) and any other notes.
**
*/

static CONDITION
readFilmSession(PRN_BASICFILMSESSION * f)
{
    CONDITION cond;
    CTNBOOLEAN nullFlag;
    int memoryallocation;


    if (XmToggleButtonGetState(wCopiesOneToggle))
	strcpy(f->copies, "1");
    else
	strcpy(f->copies, "2");

    if (XmToggleButtonGetState(wPriorityLowToggle))
	strcpy(f->priority, LOW);
    else if (XmToggleButtonGetState(wPriorityMediumToggle))
	strcpy(f->priority, MED);
    else
	strcpy(f->priority, HIGH);

    if (XmToggleButtonGetState(wMediumPaperToggle))
	strcpy(f->mediumType, PAPER);
    else if (XmToggleButtonGetState(wMediumClearFilmToggle))
	strcpy(f->mediumType, CLEAR_FILM);
    else
	strcpy(f->mediumType, BLUE_FILM);

    if (XmToggleButtonGetState(wDestinationMagazineToggle))
	strcpy(f->filmDestination, MAGAZINE);
    else
	strcpy(f->filmDestination, PROCESSOR);

    f->bfsAttributeFlag = PRN_BFS_K_COPIES |
	PRN_BFS_K_PRIORITY |
	PRN_BFS_K_MEDIUMTYPE |
	PRN_BFS_K_FILMDESTINATION;

    cond = MUT_ScanWidget(wFilmSessionLabelTxt, MUT_TEXT, &nullFlag,
			  f->filmSessionLabel);
    if (cond == MUT_NORMAL && !nullFlag)
	f->bfsAttributeFlag |= PRN_BFS_K_FILMSESSIONLABEL;

    cond = MUT_ScanWidget(wMemoryAllocationTxt, MUT_INT, &nullFlag,
			  &memoryallocation);
    if (!nullFlag) {
	if (cond == MUT_NORMAL) {
	    sprintf(f->memoryAllocation, "%-d", memoryallocation);
	    f->bfsAttributeFlag |= PRN_BFS_K_MEMORYALLOCATION;
	} else {
	    XmTextSetString(wErrorMesscrolledText1,
		   "Error reading Memory Allocation, not a proper Input\n");
	    return 0;
	}
    }
/* read number of filmboxes */
    cond = MUT_ScanWidget(wNumberOfFilmBoxesTxt, MUT_INT, &nullFlag,
			  &filmboxes);
    if (cond != MUT_NORMAL || nullFlag) {
	XmTextSetString(wErrorMesscrolledText1, " Error Obtaining number of filmboxes.");
	return 0;
    }
    f->type = PRN_K_BASICFILMSESSION;
    return 1;
}

static void
server_changed_attribute_filmSessionForm(PRN_BASICFILMSESSION filmSession,
					 PRN_BASICFILMSESSION filmSession0)
{

    int index;

    static long filmSessionFlagList[] = {
	PRN_BFS_K_COPIES,
	PRN_BFS_K_PRIORITY,
	PRN_BFS_K_MEDIUMTYPE,
	PRN_BFS_K_FILMDESTINATION,
	PRN_BFS_K_FILMSESSIONLABEL,
	PRN_BFS_K_MEMORYALLOCATION,
	PRN_BFS_K_REFERENCEDBFBSEQ
    };


    buf[0] = '\0';
/* If the Attribute is changed from SCP(Server) side, print a message */
/* and change the value in the form */

    for (index = 0; index < (int) DIM_OF(filmSessionFlagList); index++) {
	if (filmSession.bfsAttributeFlag & filmSessionFlagList[index]) {
	    switch (filmSessionFlagList[index]) {


/* Check number of copies */
	    case PRN_BFS_K_COPIES:
		if (strcmp(filmSession.copies, filmSession.copies) != 0) {
		    strcat(buf, " Server changed the attribute:  Copies\n");
		    if (strcmp(filmSession.copies, "1") == 0)
			XmToggleButtonSetState(wCopiesTwoToggle, TRUE, TRUE);
		    else if (strcmp(filmSession.copies, "2") == 0)
			XmToggleButtonSetState(wCopiesOneToggle, TRUE, TRUE);

		}
		break;
/* Check priority */
	    case PRN_BFS_K_PRIORITY:
		if (strcmp(filmSession0.priority, filmSession.priority) != 0) {
		    strcat(buf, " Server changed the attribute:  Priority\n");
		    if (strcmp(filmSession.priority, "LOW") == 0)
			XmToggleButtonSetState(wPriorityLowToggle, TRUE, TRUE);
		    else if (strcmp(filmSession.priority, "MED") == 0)
			XmToggleButtonSetState(wPriorityMediumToggle, TRUE, TRUE);
		    else if (strcmp(filmSession.priority, "HIGH") == 0)
			XmToggleButtonSetState(wPriorityHighToggle, TRUE, TRUE);
		}
		break;

/* Check mediumType */
	    case PRN_BFS_K_MEDIUMTYPE:

		if (strcmp(filmSession0.mediumType, filmSession.mediumType) != 0) {
		    strcat(buf, " Server changed the attribute:  Medium Type\n");
		    if (strcmp(filmSession.mediumType, PAPER) == 0)
			XmToggleButtonSetState(wMediumPaperToggle, TRUE, TRUE);
		    else if (strcmp(filmSession.mediumType, CLEAR_FILM) == 0)
			XmToggleButtonSetState(wMediumClearFilmToggle, TRUE, TRUE);
		    else if (strcmp(filmSession.mediumType, BLUE_FILM) == 0)
			XmToggleButtonSetState(wMediumBlueFilmToggle, TRUE, TRUE);
		}
		break;
/* Check filmDestination */
	    case PRN_BFS_K_FILMDESTINATION:
		if (strcmp(filmSession0.filmDestination, filmSession.filmDestination) != 0) {
		    strcat(buf, " Server changed the attribute:  Destination\n");
		    if (strcmp(filmSession.filmDestination, "MAGAZINE") == 0)
			XmToggleButtonSetState(wDestinationMagazineToggle, TRUE, TRUE);
		    else if (strcmp(filmSession.filmDestination, "PROCESSOR") == 0)
			XmToggleButtonSetState(wDestinationProcessorToggle, TRUE, TRUE);
		}
		break;

/* Check filmSessionLabel*/
	    case PRN_BFS_K_FILMSESSIONLABEL:
		if (strcmp(filmSession0.filmSessionLabel, filmSession.filmSessionLabel) != 0) {
		    strcat(buf, " Server changed the attribute:  Film Session Label\n");
		    XmTextSetString(wFilmSessionLabelTxt, filmSession.filmSessionLabel);
		}
		break;
/* Check memoryAllocation */
	    case PRN_BFS_K_MEMORYALLOCATION:
		if (strcmp(filmSession0.memoryAllocation, filmSession.memoryAllocation) != 0) {
		    strcat(buf, " Server changed the attribute:  Memory Allocation\n");
		    XmTextSetString(wMemoryAllocationTxt, filmSession.memoryAllocation);
		}
		break;
	    }
	}
    }
    XmTextSetString(wErrorMesscrolledText1, buf);
}


void
makeFilmScreateSen()
{
    XtVaSetValues(pushButton2, XmNsensitive, TRUE, NULL);
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void
activateCB_pushButton2(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    _UxCfilmSessionForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxFilmSessionFormContext;
    UxFilmSessionFormContext = UxContext =
	(_UxCfilmSessionForm *) UxGetContext(UxWidget);
    {
	CONDITION cond;
	unsigned short status;

	PRN_BASICFILMSESSION filmSession;
	PRN_BASICFILMSESSION filmSession0;	/* saving the copy of SCU
						 * Attributes */


	/* read FilmSession form */
	cond = readFilmSession(&filmSession);
	if (cond != 1) {
	    return;
	}
	/* Save a copy of attributes sent by the client(SCU) */
	filmSession0 = filmSession;

	strcpy(filmSession.filmSessionSOPInstanceUID, "");

	cond = createFilmSession(&associationKey, &associationParameters,
			 &filmSession, DICOM_SOPCLASSGREYSCALEPRINTMGMTMETA,
				 &status);

	if (cond != APP_NORMAL) {
	    MUT_DisplayConditionText(wErrorMesscrolledText1);
	    (void) COND_PopCondition(TRUE);
	}
	(void) strcpy(filmSessionSOPInstanceUID,
		      filmSession.filmSessionSOPInstanceUID);

	server_changed_attribute_filmSessionForm(filmSession, filmSession0);

#ifdef RUNTIME
	if (wFilmBoxForm == 0)
	    wFilmBoxForm = (Widget) create_filmBoxForm(NULL);

	XtManageChild(wFilmBoxForm);
#endif

	XtVaSetValues(pushButton2, XmNsensitive, FALSE, NULL);
    }
    UxFilmSessionFormContext = UxSaveCtx;
}

static void
activateCB_pushButton3(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    _UxCfilmSessionForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxFilmSessionFormContext;
    UxFilmSessionFormContext = UxContext =
	(_UxCfilmSessionForm *) UxGetContext(UxWidget);
    {
	CONDITION cond;

	if (associationActive) {
	    cond = DUL_ReleaseAssociation(&associationKey);
	    if (cond != DUL_RELEASECONFIRMED)
		(void) DUL_AbortAssociation(&associationKey);

	    (void) DUL_DropAssociation(&associationKey);
	    (void) DUL_ClearServiceParameters(&associationParameters);
	}
	associationActive = FALSE;

	XtUnmanageChild(wFilmSessionForm);
	if (wFilmInformationForm != 0)
	    XtUnmanageChild(wFilmInformationForm);

    }
    UxFilmSessionFormContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget
_Uxbuild_filmSessionForm()
{
    Widget _UxParent;


    /* Creation of filmSessionForm */
    _UxParent = UxParent;
    if (_UxParent == NULL) {
	_UxParent = UxTopLevel;
    }
    _UxParent = XtVaCreatePopupShell("filmSessionForm_shell",
				     xmDialogShellWidgetClass, _UxParent,
				     XmNx, 433,
				     XmNy, 85,
				     XmNwidth, 824,
				     XmNheight, 723,
				     XmNshellUnitType, XmPIXELS,
				     XmNtitle, "filmSessionForm",
				     NULL);

    filmSessionForm = XtVaCreateWidget("filmSessionForm",
				       xmFormWidgetClass,
				       _UxParent,
				       XmNwidth, 824,
				       XmNheight, 723,
				       XmNunitType, XmPIXELS,
				       XmNautoUnmanage, FALSE,
				       NULL);
    UxPutContext(filmSessionForm, (char *) UxFilmSessionFormContext);


    /* Creation of label10 */
    label10 = XtVaCreateManagedWidget("label10",
				      xmLabelWidgetClass,
				      filmSessionForm,
				      XmNx, 80,
				      XmNy, 20,
				      XmNwidth, 310,
				      XmNheight, 40,
			  RES_CONVERT(XmNlabelString, "Basic Film Session"),
				      NULL);
    UxPutContext(label10, (char *) UxFilmSessionFormContext);


    /* Creation of rowColumn1 */
    rowColumn1 = XtVaCreateManagedWidget("rowColumn1",
					 xmRowColumnWidgetClass,
					 filmSessionForm,
					 XmNwidth, 280,
					 XmNheight, 61,
					 XmNx, 270,
					 XmNy, 70,
					 XmNradioBehavior, TRUE,
					 RES_CONVERT(XmNlabelString, ""),
					 NULL);
    UxPutContext(rowColumn1, (char *) UxFilmSessionFormContext);


    /* Creation of wCopiesOneToggle */
    wCopiesOneToggle = XtVaCreateManagedWidget("wCopiesOneToggle",
					       xmToggleButtonWidgetClass,
					       rowColumn1,
					       XmNx, 3,
					       XmNy, -50,
					       XmNwidth, 109,
					       XmNheight, 79,
					   RES_CONVERT(XmNlabelString, "1"),
					       XmNset, TRUE,
					       NULL);
    UxPutContext(wCopiesOneToggle, (char *) UxFilmSessionFormContext);


    /* Creation of wCopiesTwoToggle */
    wCopiesTwoToggle = XtVaCreateManagedWidget("wCopiesTwoToggle",
					       xmToggleButtonWidgetClass,
					       rowColumn1,
					       XmNx, 0,
					       XmNy, 0,
					       XmNwidth, 110,
					       XmNheight, 30,
					   RES_CONVERT(XmNlabelString, "2"),
					       NULL);
    UxPutContext(wCopiesTwoToggle, (char *) UxFilmSessionFormContext);


    /* Creation of label11 */
    label11 = XtVaCreateManagedWidget("label11",
				      xmLabelWidgetClass,
				      filmSessionForm,
				      XmNx, 40,
				      XmNy, 80,
				      XmNwidth, 200,
				      XmNheight, 40,
				      RES_CONVERT(XmNlabelString, "Copies"),
				      NULL);
    UxPutContext(label11, (char *) UxFilmSessionFormContext);


    /* Creation of label12 */
    label12 = XtVaCreateManagedWidget("label12",
				      xmLabelWidgetClass,
				      filmSessionForm,
				      XmNx, 40,
				      XmNy, 170,
				      XmNwidth, 200,
				      XmNheight, 40,
			      RES_CONVERT(XmNlabelString, "Print Priority"),
				      NULL);
    UxPutContext(label12, (char *) UxFilmSessionFormContext);


    /* Creation of rowColumn2 */
    rowColumn2 = XtVaCreateManagedWidget("rowColumn2",
					 xmRowColumnWidgetClass,
					 filmSessionForm,
					 XmNwidth, 280,
					 XmNheight, 61,
					 XmNx, 270,
					 XmNy, 140,
					 XmNradioBehavior, TRUE,
					 RES_CONVERT(XmNlabelString, ""),
					 NULL);
    UxPutContext(rowColumn2, (char *) UxFilmSessionFormContext);


    /* Creation of wPriorityLowToggle */
    wPriorityLowToggle = XtVaCreateManagedWidget("wPriorityLowToggle",
						 xmToggleButtonWidgetClass,
						 rowColumn2,
						 XmNx, 40,
						 XmNy, 3,
						 XmNwidth, 35,
						 XmNheight, 26,
					 RES_CONVERT(XmNlabelString, "Low"),
						 XmNset, TRUE,
						 NULL);
    UxPutContext(wPriorityLowToggle, (char *) UxFilmSessionFormContext);


    /* Creation of wPriorityMediumToggle */
    wPriorityMediumToggle = XtVaCreateManagedWidget("wPriorityMediumToggle",
						  xmToggleButtonWidgetClass,
						    rowColumn2,
						    XmNx, 0,
						    XmNy, 0,
						    XmNwidth, 110,
						    XmNheight, 30,
				      RES_CONVERT(XmNlabelString, "Medium"),
						    NULL);
    UxPutContext(wPriorityMediumToggle, (char *) UxFilmSessionFormContext);


    /* Creation of wPriorityHighToggle */
    wPriorityHighToggle = XtVaCreateManagedWidget("wPriorityHighToggle",
						  xmToggleButtonWidgetClass,
						  rowColumn2,
						  XmNx, 3,
						  XmNy, -50,
						  XmNwidth, 109,
						  XmNheight, 79,
					RES_CONVERT(XmNlabelString, "High"),
						  NULL);
    UxPutContext(wPriorityHighToggle, (char *) UxFilmSessionFormContext);


    /* Creation of label13 */
    label13 = XtVaCreateManagedWidget("label13",
				      xmLabelWidgetClass,
				      filmSessionForm,
				      XmNx, 40,
				      XmNy, 260,
				      XmNwidth, 200,
				      XmNheight, 40,
				 RES_CONVERT(XmNlabelString, "Medium Type"),
				      NULL);
    UxPutContext(label13, (char *) UxFilmSessionFormContext);


    /* Creation of rowColumn3 */
    rowColumn3 = XtVaCreateManagedWidget("rowColumn3",
					 xmRowColumnWidgetClass,
					 filmSessionForm,
					 XmNwidth, 280,
					 XmNheight, 61,
					 XmNx, 267,
					 XmNy, 248,
					 XmNradioBehavior, TRUE,
					 RES_CONVERT(XmNlabelString, ""),
					 NULL);
    UxPutContext(rowColumn3, (char *) UxFilmSessionFormContext);


    /* Creation of wMediumPaperToggle */
    wMediumPaperToggle = XtVaCreateManagedWidget("wMediumPaperToggle",
						 xmToggleButtonWidgetClass,
						 rowColumn3,
						 XmNx, 3,
						 XmNy, 3,
						 XmNwidth, 77,
						 XmNheight, 79,
				       RES_CONVERT(XmNlabelString, "Paper"),
						 XmNset, TRUE,
						 NULL);
    UxPutContext(wMediumPaperToggle, (char *) UxFilmSessionFormContext);


    /* Creation of wMediumClearFilmToggle */
    wMediumClearFilmToggle = XtVaCreateManagedWidget("wMediumClearFilmToggle",
						  xmToggleButtonWidgetClass,
						     rowColumn3,
						     XmNx, 0,
						     XmNy, 0,
						     XmNwidth, 110,
						     XmNheight, 30,
				  RES_CONVERT(XmNlabelString, "Clear Film"),
						     XmNset, FALSE,
						     NULL);
    UxPutContext(wMediumClearFilmToggle, (char *) UxFilmSessionFormContext);


    /* Creation of wMediumBlueFilmToggle */
    wMediumBlueFilmToggle = XtVaCreateManagedWidget("wMediumBlueFilmToggle",
						  xmToggleButtonWidgetClass,
						    rowColumn3,
						    XmNx, 3,
						    XmNy, -50,
						    XmNwidth, 109,
						    XmNheight, 79,
				   RES_CONVERT(XmNlabelString, "Blue Film"),
						    NULL);
    UxPutContext(wMediumBlueFilmToggle, (char *) UxFilmSessionFormContext);


    /* Creation of label14 */
    label14 = XtVaCreateManagedWidget("label14",
				      xmLabelWidgetClass,
				      filmSessionForm,
				      XmNx, 40,
				      XmNy, 370,
				      XmNwidth, 200,
				      XmNheight, 40,
			    RES_CONVERT(XmNlabelString, "Film Destination"),
				      NULL);
    UxPutContext(label14, (char *) UxFilmSessionFormContext);


    /* Creation of rowColumn4 */
    rowColumn4 = XtVaCreateManagedWidget("rowColumn4",
					 xmRowColumnWidgetClass,
					 filmSessionForm,
					 XmNwidth, 280,
					 XmNheight, 61,
					 XmNx, 270,
					 XmNy, 360,
					 XmNradioBehavior, TRUE,
					 RES_CONVERT(XmNlabelString, ""),
					 NULL);
    UxPutContext(rowColumn4, (char *) UxFilmSessionFormContext);


    /* Creation of wDestinationMagazineToggle */
    wDestinationMagazineToggle = XtVaCreateManagedWidget("wDestinationMagazineToggle",
						  xmToggleButtonWidgetClass,
							 rowColumn4,
							 XmNx, 3,
							 XmNy, -50,
							 XmNwidth, 109,
							 XmNheight, 79,
				    RES_CONVERT(XmNlabelString, "Magazine"),
							 XmNset, TRUE,
							 NULL);
    UxPutContext(wDestinationMagazineToggle, (char *) UxFilmSessionFormContext);


    /* Creation of wDestinationProcessorToggle */
    wDestinationProcessorToggle = XtVaCreateManagedWidget("wDestinationProcessorToggle",
						  xmToggleButtonWidgetClass,
							  rowColumn4,
							  XmNx, 0,
							  XmNy, 0,
							  XmNwidth, 110,
							  XmNheight, 30,
				   RES_CONVERT(XmNlabelString, "Processor"),
							  NULL);
    UxPutContext(wDestinationProcessorToggle, (char *) UxFilmSessionFormContext);


    /* Creation of label15 */
    label15 = XtVaCreateManagedWidget("label15",
				      xmLabelWidgetClass,
				      filmSessionForm,
				      XmNx, 40,
				      XmNy, 440,
				      XmNwidth, 200,
				      XmNheight, 40,
			RES_CONVERT(XmNlabelString, "Number of Film Boxes"),
				      NULL);
    UxPutContext(label15, (char *) UxFilmSessionFormContext);


    /* Creation of wNumberOfFilmBoxesTxt */
    wNumberOfFilmBoxesTxt = XtVaCreateManagedWidget("wNumberOfFilmBoxesTxt",
						    xmTextFieldWidgetClass,
						    filmSessionForm,
						    XmNwidth, 120,
						    XmNx, 280,
						    XmNy, 440,
						    XmNheight, 40,
						    XmNvalue, "1",
						    NULL);
    UxPutContext(wNumberOfFilmBoxesTxt, (char *) UxFilmSessionFormContext);


    /* Creation of pushButton2 */
    pushButton2 = XtVaCreateManagedWidget("pushButton2",
					  xmPushButtonWidgetClass,
					  filmSessionForm,
					  XmNx, 150,
					  XmNy, 510,
					  XmNwidth, 150,
					  XmNheight, 50,
				      RES_CONVERT(XmNlabelString, "Create"),
					  NULL);
    XtAddCallback(pushButton2, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton2,
		  (XtPointer) UxFilmSessionFormContext);

    UxPutContext(pushButton2, (char *) UxFilmSessionFormContext);


    /* Creation of pushButton3 */
    pushButton3 = XtVaCreateManagedWidget("pushButton3",
					  xmPushButtonWidgetClass,
					  filmSessionForm,
					  XmNx, 550,
					  XmNy, 510,
					  XmNwidth, 150,
					  XmNheight, 50,
				      RES_CONVERT(XmNlabelString, "Cancel"),
					  NULL);
    XtAddCallback(pushButton3, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton3,
		  (XtPointer) UxFilmSessionFormContext);

    UxPutContext(pushButton3, (char *) UxFilmSessionFormContext);


    /* Creation of wFilmSessionLabelTxt */
    wFilmSessionLabelTxt = XtVaCreateManagedWidget("wFilmSessionLabelTxt",
						   xmTextFieldWidgetClass,
						   filmSessionForm,
						   XmNwidth, 250,
						   XmNx, 560,
						   XmNy, 70,
						   XmNheight, 40,
						   NULL);
    UxPutContext(wFilmSessionLabelTxt, (char *) UxFilmSessionFormContext);


    /* Creation of wMemoryAllocationTxt */
    wMemoryAllocationTxt = XtVaCreateManagedWidget("wMemoryAllocationTxt",
						   xmTextFieldWidgetClass,
						   filmSessionForm,
						   XmNwidth, 250,
						   XmNx, 560,
						   XmNy, 140,
						   XmNheight, 40,
						   NULL);
    UxPutContext(wMemoryAllocationTxt, (char *) UxFilmSessionFormContext);


    /* Creation of label26 */
    label26 = XtVaCreateManagedWidget("label26",
				      xmLabelWidgetClass,
				      filmSessionForm,
				      XmNx, 400,
				      XmNy, 70,
				      XmNwidth, 150,
				      XmNheight, 30,
			  RES_CONVERT(XmNlabelString, "Film Session Label"),
				      NULL);
    UxPutContext(label26, (char *) UxFilmSessionFormContext);


    /* Creation of label27 */
    label27 = XtVaCreateManagedWidget("label27",
				      xmLabelWidgetClass,
				      filmSessionForm,
				      XmNx, 400,
				      XmNy, 140,
				      XmNwidth, 150,
				      XmNheight, 30,
			   RES_CONVERT(XmNlabelString, "Memory Allocation"),
				      NULL);
    UxPutContext(label27, (char *) UxFilmSessionFormContext);


    /* Creation of scrolledWindowText2 */
    scrolledWindowText2 = XtVaCreateManagedWidget("scrolledWindowText2",
						xmScrolledWindowWidgetClass,
						  filmSessionForm,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNx, 140,
						  XmNy, 590,
						  NULL);
    UxPutContext(scrolledWindowText2, (char *) UxFilmSessionFormContext);


    /* Creation of scrolledWindowText4 */
    scrolledWindowText4 = XtVaCreateManagedWidget("scrolledWindowText4",
						xmScrolledWindowWidgetClass,
						  filmSessionForm,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNx, 240,
						  XmNy, 580,
						  NULL);
    UxPutContext(scrolledWindowText4, (char *) UxFilmSessionFormContext);


    /* Creation of wErrorMesscrolledText1 */
    wErrorMesscrolledText1 = XtVaCreateManagedWidget("wErrorMesscrolledText1",
						     xmTextWidgetClass,
						     scrolledWindowText4,
						     XmNwidth, 450,
						     XmNheight, 80,
					     XmNeditMode, XmMULTI_LINE_EDIT,
						     NULL);
    UxPutContext(wErrorMesscrolledText1, (char *) UxFilmSessionFormContext);


    XtAddCallback(filmSessionForm, XmNdestroyCallback,
		  (XtCallbackProc) UxDestroyContextCB,
		  (XtPointer) UxFilmSessionFormContext);


    return (filmSessionForm);
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget
create_filmSessionForm(swidget _UxUxParent)
{
    Widget rtrn;
    _UxCfilmSessionForm *UxContext;

    UxFilmSessionFormContext = UxContext =
	(_UxCfilmSessionForm *) UxNewContext(sizeof(_UxCfilmSessionForm), False);

    UxParent = _UxUxParent;

    rtrn = _Uxbuild_filmSessionForm();

    return (rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/
