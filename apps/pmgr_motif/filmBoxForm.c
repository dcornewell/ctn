
/*******************************************************************************
	filmBoxForm.c

       Associated Header file: filmBoxForm.h
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
#include <Xm/ToggleB.h>
#include <Xm/RowColumn.h>
#include <Xm/TextF.h>
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
** Module Name(s):      create_filmBoxForm
**			makeCreateButtonSen
** Author, Date:        Chander Sabharwal
** Intent:              This module contains the user interface for the film
**			box (form).  It has a public function that allows the interface
**			to be displayed and several other public functions.
** Last Update:         $Author: smm $, $Date: 2002-10-31 23:36:06 $
** Source File:         $RCSfile: filmBoxForm.c,v $
** Revision:            $Revision: 1.27 $
** Status:              $State: Exp $
*/

static char rcsid[] = "$Revision: 1.27 $ $RCSfile: filmBoxForm.c,v $";



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

#define  MAXLEN  5000

extern DUL_ASSOCIATIONKEY *associationKey;
extern DUL_ASSOCIATESERVICEPARAMETERS associationParameters;

static CONDITION readFilmBoxForm(PRN_BASICFILMBOX * fb);

extern Widget wStudyListForm;
extern Widget wImageAttribute;
extern Widget wPreviewStudyForm;
extern Widget wPreviewPrintForm;
extern Widget wBasicFilmBox_Standard;
extern Widget wImageAttribute;
extern Widget wFilmBoxForm;

extern char filmSessionSOPInstanceUID[];

extern char buf[MAXLEN];

CTNBOOLEAN getStudyList();

static void server_changed_attribute_filmBoxForm();

static int r1,
    c1;


void makeCreateButtonSen();


PRN_BASICFILMBOX filmBox;


/*******************************************************************************
       The following header file defines the context structure.
*******************************************************************************/

#define CONTEXT_MACRO_ACCESS 1
#include "filmBoxForm.h"
#undef CONTEXT_MACRO_ACCESS


/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

/*readFilmBox
**
** Purpose:
**	This function reads the attributes for filmBox
**
** Parameter Dictionary:
**	fb	pointer to the filmBox Attributes
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
readFilmBoxForm(PRN_BASICFILMBOX * fb)
{
    CONDITION cond;
    CTNBOOLEAN nullFlag;
    Widget wid_hist;
    char *rc_typed;
    unsigned short us;
    float rminDensity,
        rmaxDensity,
        emptyimagedensity,
        borderdensity;


/* Reading Columns and Rows displayformat */
    cond = MUT_ScanWidget(wColumnsTxt, MUT_INT, &nullFlag,
			  &c1);
    if (cond != MUT_NORMAL || nullFlag) {
	XmTextSetString(wErrorMesscrolledText2, " Error Obtaining Col Value.");
	return 0;
    }
    if (c1 < 1 || c1 > 8) {
	XmTextSetString(wErrorMesscrolledText2, " Enter column between 1and 8");
	return 0;
    }
    cond = MUT_ScanWidget(wRowsTxt, MUT_INT, &nullFlag,
			  &r1);
    if (cond != MUT_NORMAL || nullFlag) {
	XmTextSetString(wErrorMesscrolledText2, " Error Obtaining Row Value.");
	return 0;
    }
    if (r1 < 1 || r1 > 8) {
	XmTextSetString(wErrorMesscrolledText2, " Enter row between 1and 8");
	return 0;
    }
    sprintf(fb->imageDisplayFormat, "%s\\%-d,%-d", STANDARD, c1, r1);


    /* Reading Orientation */
    if (XmToggleButtonGetState(wPortraitToggle))
	strcpy(fb->filmOrientation, PORTRAIT);
    else
	strcpy(fb->filmOrientation, LANDSCAPE);

/* Reading Film Size */
    XtVaGetValues(wFilmSizeOptMenu,
		  XmNmenuHistory, &wid_hist,
		  NULL);
    if (strcmp(XtName(wid_hist), XtName(w8inX10inButton)) == 0)
	strcpy(fb->filmSizeID, SIZE8INX10IN);
    else if (strcmp(XtName(wid_hist), XtName(w10inX12inButton)) == 0)
	strcpy(fb->filmSizeID, SIZE10INX12IN);
    else if (strcmp(XtName(wid_hist), XtName(w10inX14inButton)) == 0)
	strcpy(fb->filmSizeID, SIZE10INX14IN);
    else if (strcmp(XtName(wid_hist), XtName(w11inX14inButton)) == 0)
	strcpy(fb->filmSizeID, SIZE11INX14IN);
    else if (strcmp(XtName(wid_hist), XtName(w14inX14inButton)) == 0)
	strcpy(fb->filmSizeID, SIZE14INX14IN);
    else if (strcmp(XtName(wid_hist), XtName(w14inX17inButton)) == 0)
	strcpy(fb->filmSizeID, SIZE14INX17IN);
    else if (strcmp(XtName(wid_hist), XtName(w24cmX24cmButton)) == 0)
	strcpy(fb->filmSizeID, SIZE24CMX24CM);
    else if (strcmp(XtName(wid_hist), XtName(w24cmX30cmButton)) == 0)
	strcpy(fb->filmSizeID, SIZE24CMX30CM);


/* Reading Magnification */
    XtVaGetValues(wMagnificationOptMenu,
		  XmNmenuHistory, &wid_hist,
		  NULL);
    if (strcmp(XtName(wid_hist), XtName(wReplicateButton)) == 0)
	strcpy(fb->magnificationType, REPLICATE);
    else if (strcmp(XtName(wid_hist), XtName(wBilinearButton)) == 0)
	strcpy(fb->magnificationType, BILINEAR);
    else if (strcmp(XtName(wid_hist), XtName(wCubicButton)) == 0)
	strcpy(fb->magnificationType, CUBIC);
    else
	strcpy(fb->magnificationType, NONE);


    fb->bfbAttributeFlag = PRN_BFB_K_IMAGEDISPLAYFORMAT |
	PRN_BFB_K_FILMORIENTATION |
	PRN_BFB_K_FILMSIZEID |
	PRN_BFB_K_MAGNIFICATIONTYPE |
	PRN_BFB_K_REFERENCEDBASICFILMSESSIONSEQ;

/* Reading Trim */
    if (XmToggleButtonGetState(wTrimYesToggle)) {
	strcpy(fb->trim, YES);
	fb->bfbAttributeFlag |= PRN_BFB_K_TRIM;
    } else if (XmToggleButtonGetState(wTrimNoToggle)) {
	strcpy(fb->trim, NO);
	fb->bfbAttributeFlag |= PRN_BFB_K_TRIM;
    } else
	strcpy(fb->trim, "");


/* Reading Maximum Density */
    cond = MUT_ScanWidget(wMaxDensityTxt, MUT_FLOAT, &nullFlag,
			  &rmaxDensity);
    if (!nullFlag) {
	if (cond == MUT_NORMAL) {
	    rmaxDensity = rmaxDensity * 100;
	    fb->maxDensity = (unsigned short) rmaxDensity;
	    fb->bfbAttributeFlag |= PRN_BFB_K_MAXDENSITY;
	} else {
	    XmTextSetString(wErrorMesscrolledText2,
			 "Error reading Max Density, not a proper Input\n");
	    return 0;
	}
    }
/* Reading Configuration Information */
    cond = MUT_ScanWidget(wConfigurationInfoTxt, MUT_TEXT, &nullFlag,
			  fb->configurationInfo);

    if (!nullFlag) {
	if (cond == MUT_NORMAL) {

	    fb->bfbAttributeFlag |= PRN_BFB_K_CONFIGURATIONINFO;
	} else {
	    XmTextSetString(wErrorMesscrolledText2,
		       "Error reading Configuration, not a proper Input\n");
	    return 0;
	}
    }
/* Reading Minimum Density */
    cond = MUT_ScanWidget(wMinDensityTxt, MUT_FLOAT, &nullFlag,
			  &rminDensity);

    if (!nullFlag) {
	if (cond == MUT_NORMAL) {
	    if (rminDensity > 0.) {
		rminDensity = rminDensity * 100;
		fb->minDensity = (unsigned short) rminDensity;
		fb->bfbAttributeFlag |= PRN_BFB_K_MINDENSITY;
	    }
	} else {
	    XmTextSetString(wErrorMesscrolledText2,
			 "Error reading Min Density, not a proper Input\n");
	    return 0;
	}
    }
/* Reading Empty Image Density */
    if (XmToggleButtonGetState(wEmptyImageBlackToggle)) {
	strcpy(fb->emptyImageDensity, BLACK);
	fb->bfbAttributeFlag |= PRN_BFB_K_EMPTYIMAGEDENSITY;
    } else if (XmToggleButtonGetState(wEmptyImageWhiteToggle)) {
	strcpy(fb->emptyImageDensity, WHITE);
	fb->bfbAttributeFlag |= PRN_BFB_K_EMPTYIMAGEDENSITY;
    } else {
	if (XmToggleButtonGetState(wEmptyImageOtherToggle))
	    cond = MUT_ScanWidget(wEmptyImageOtherTxt, MUT_FLOAT, &nullFlag,
				  &emptyimagedensity);
	if (!nullFlag) {
	    if (cond == MUT_NORMAL) {
		sprintf(fb->emptyImageDensity, "%-d", (int) emptyimagedensity * 100);
		fb->bfbAttributeFlag |= PRN_BFB_K_EMPTYIMAGEDENSITY;
	    } else {
		XmTextSetString(wErrorMesscrolledText2,
				"Error reading Empty Image from Other Txt, not a proper Input\n");
		return 0;
	    }
	}
    }


/* Reading Border Density */
    if (XmToggleButtonGetState(wBorderBlackToggle)) {
	strcpy(fb->borderDensity, BLACK);
	fb->bfbAttributeFlag |= PRN_BFB_K_BORDERDENSITY;
    } else if (XmToggleButtonGetState(wBorderWhiteToggle)) {
	strcpy(fb->borderDensity, WHITE);
	fb->bfbAttributeFlag |= PRN_BFB_K_BORDERDENSITY;
    } else {
	if (XmToggleButtonGetState(wBorderOtherToggle))
	    cond = MUT_ScanWidget(wBorderOtherTxt, MUT_FLOAT, &nullFlag,
				  &borderdensity);
	if (!nullFlag) {
	    if (cond == MUT_NORMAL) {

		sprintf(fb->borderDensity, "%-d", (int) borderdensity * 100);
		fb->bfbAttributeFlag |= PRN_BFB_K_BORDERDENSITY;
	    } else {
		XmTextSetString(wErrorMesscrolledText2,
				"Error reading Border Density from Other Txt, not a proper Input\n");
		return 0;
	    }
	}
    }


/* Reading Annotation Display Format */
    cond = MUT_ScanWidget(wAnnotationDispFormatTxt, MUT_TEXT, &nullFlag,
			  fb->annotationDisplayFormatID);
    if (!nullFlag) {
	if (cond == MUT_NORMAL) {

	    fb->bfbAttributeFlag |= PRN_BFB_K_ANNOTATIONDISPLAYFORMATID;
	} else {
	    XmTextSetString(wErrorMesscrolledText2,
			    "Error reading Annotation Display Format ID, not a proper Input\n");
	    return 0;
	}
    }
    fb->type = PRN_K_BASICFILMBOX;

    return 1;
}



/*server_changed_attribute_filmBoxForm
**
*/

static void
server_changed_attribute_filmBoxForm(PRN_BASICFILMBOX filmBox,
				     PRN_BASICFILMBOX filmBox0)
{

    int index;
    char text[80];
    float realMaxDen,
        realMinDen;
    int col0,
        col,
        row0,
        row;
    Widget wid_hist;
    Arg arg;


    static filmBoxFlagList[] = {
	PRN_BFB_K_IMAGEDISPLAYFORMAT,
	PRN_BFB_K_ANNOTATIONDISPLAYFORMATID,
	PRN_BFB_K_FILMORIENTATION,
	PRN_BFB_K_FILMSIZEID,
	PRN_BFB_K_MAGNIFICATIONTYPE,
	PRN_BFB_K_SMOOTHINGTYPE,
	PRN_BFB_K_BORDERDENSITY,
	PRN_BFB_K_EMPTYIMAGEDENSITY,
	PRN_BFB_K_MINDENSITY,
	PRN_BFB_K_MAXDENSITY,
	PRN_BFB_K_TRIM,
	PRN_BFB_K_CONFIGURATIONINFO,
	PRN_BFB_K_REFERENCEDBASICFILMSESSIONSEQ,
	PRN_BFB_K_REFERENCEDBASICIMAGEBOXSEQ,
	PRN_BFB_K_REFERENCEDBASICANNOTATIONBOXSEQ,
	PRN_BFB_K_REFPRINTJOBSEQ
    };

/* If the Attribute is changed from SCP(Server) side, print a message */
/* and change the value in the form */

    buf[0] = '\0';

    for (index = 0; index < (int) DIM_OF(filmBoxFlagList); index++) {
	if (filmBox.bfbAttributeFlag & filmBoxFlagList[index]) {
	    switch (filmBoxFlagList[index]) {

/* Check attri. filmOrientation */
	    case PRN_BFB_K_FILMORIENTATION:
		if (strcmp(filmBox0.filmOrientation, filmBox.filmOrientation) != 0) {
		    strcat(buf, " Server changed the attribute:  Film Orientation\n");
		    if (strcmp(filmBox.filmOrientation, "PORTRAIT") == 0)
			XmToggleButtonSetState(wPortraitToggle, TRUE, TRUE);
		    else if (strcmp(filmBox.filmOrientation, "LANDSCAPE") == 0)
			XmToggleButtonSetState(wLandscapeToggle, TRUE, TRUE);
		}
		break;

/* Check attri. trim */
	    case PRN_BFB_K_TRIM:
		if (strcmp(filmBox0.trim, filmBox.trim) != 0) {
		    strcat(buf, " Server changed the attribute:  Trim\n");
		    if (strcmp(filmBox.trim, "YES") == 0)
			XmToggleButtonSetState(wTrimYesToggle, TRUE, TRUE);
		    else if (strcmp(filmBox.trim, "No") == 0)
			XmToggleButtonSetState(wTrimNoToggle, TRUE, TRUE);
		}
		break;

/* Check attri. maxDensity */
	    case PRN_BFB_K_MAXDENSITY:
		if (filmBox0.maxDensity != filmBox.maxDensity) {
		    strcat(buf, " Server changed the attribute:  Max Density\n");
		    realMaxDen = ((float) filmBox.maxDensity) / 100.0;
		    sprintf(text, "%7.2f", realMaxDen);
		    XmTextSetString(wMaxDensityTxt, text);
		}
		break;

/* Check mindensity */
	    case PRN_BFB_K_MINDENSITY:
		if (filmBox0.minDensity != filmBox.minDensity) {
		    strcat(buf, " Server changed the attribute:  Min Density\n");
		    realMinDen = ((float) filmBox.minDensity) / 100.0;
		    sprintf(text, "%7.2f", realMinDen);
		    XmTextSetString(wMinDensityTxt, text);
		}
		break;

/* Check emptyImageDensity */
	    case PRN_BFB_K_EMPTYIMAGEDENSITY:
		if (strcmp(filmBox0.emptyImageDensity, filmBox.emptyImageDensity) != 0) {
		    strcat(buf, " Server changed the attribute:  Empty Image Density\n");
		    if (strcmp(filmBox.emptyImageDensity, "BLACK") == 0)
			XmToggleButtonSetState(wEmptyImageBlackToggle, TRUE, TRUE);
		    else if (strcmp(filmBox.emptyImageDensity, "WHITE") == 0)
			XmToggleButtonSetState(wEmptyImageWhiteToggle, TRUE, TRUE);
		    else {
			XmToggleButtonSetState(wEmptyImageOtherToggle, TRUE, TRUE);
			XmTextSetString(wEmptyImageOtherTxt, filmBox.emptyImageDensity);
		    }
		}
		break;

/* Check borderDensity*/
	    case PRN_BFB_K_BORDERDENSITY:
		if (strcmp(filmBox0.borderDensity, filmBox.borderDensity) != 0) {
		    strcat(buf, " Server changed the attribute:  Border Density\n");
		    if (strcmp(filmBox.borderDensity, "BLACK") == 0)
			XmToggleButtonSetState(wBorderBlackToggle, TRUE, TRUE);
		    else if (strcmp(filmBox.borderDensity, "WHITE") == 0)
			XmToggleButtonSetState(wBorderWhiteToggle, TRUE, TRUE);
		    else {
			XmToggleButtonSetState(wBorderOtherToggle, TRUE, TRUE);
			XmTextSetString(wBorderOtherTxt, filmBox.borderDensity);
		    }
		}
		break;

/* Check annotationDisplayFormatID*/
	    case PRN_BFB_K_ANNOTATIONDISPLAYFORMATID:
		if (strcmp(filmBox0.annotationDisplayFormatID, filmBox.annotationDisplayFormatID) != 0) {
		    strcat(buf, " Server changed the attribute:  Annotation Display Format ID\n");
		    XmTextSetString(wAnnotationDispFormatTxt, filmBox.annotationDisplayFormatID);
		}
		break;

/* check configurationInfo*/
	    case PRN_BFB_K_CONFIGURATIONINFO:
		if (strcmp(filmBox0.configurationInfo, filmBox.configurationInfo) != 0) {
		    strcat(buf, " Server changed the attribute:  Configuration Info\n");
		    XmTextSetString(wConfigurationInfoTxt, filmBox.configurationInfo);
		}
		break;

/* check imageDisplayFormat */
	    case PRN_BFB_K_IMAGEDISPLAYFORMAT:
		if (strcmp(filmBox0.imageDisplayFormat, filmBox.imageDisplayFormat) != 0) {
		    strcat(buf, " Server changed the attribute:  Image Display Format\n");
		    sscanf(filmBox0.imageDisplayFormat, "STANDARD\\%d,%d", &col0, &row0);
		    sscanf(filmBox.imageDisplayFormat, "STANDARD\\%d,%d", &col, &row);
		    if (row != row0) {
			sprintf(text, "%d", row);
			XmTextSetString(wRowsTxt, text);
		    }
		    if (col != col0) {
			sprintf(text, "%d", col);
			XmTextSetString(wColumnsTxt, text);
		    }
		}
		break;

/* Check filmSizeID */
	    case PRN_BFB_K_FILMSIZEID:
		if (strcmp(filmBox0.filmSizeID, filmBox.filmSizeID) != 0) {
		    strcat(buf, " Server changed the attribute:  Film Size ID\n");
#ifdef RUNTIME
		    if (strcmp(filmBox.filmSizeID, SIZE8INX10IN) == 0)
			wid_hist = w8inX10inButton;
		    else if (strcmp(filmBox.filmSizeID, SIZE10INX12IN) == 0)
			wid_hist = w10inX12inButton;
		    else if (strcmp(filmBox.filmSizeID, SIZE10INX14IN) == 0)
			wid_hist = w10inX14inButton;
		    else if (strcmp(filmBox.filmSizeID, SIZE11INX14IN) == 0)
			wid_hist = w11inX14inButton;
		    else if (strcmp(filmBox.filmSizeID, SIZE14INX14IN) == 0)
			wid_hist = w14inX14inButton;
		    else if (strcmp(filmBox.filmSizeID, SIZE14INX17IN) == 0)
			wid_hist = w14inX17inButton;
		    else if (strcmp(filmBox.filmSizeID, SIZE24CMX24CM) == 0)
			wid_hist = w24cmX24cmButton;
		    else if (strcmp(filmBox.filmSizeID, SIZE24CMX30CM) == 0)
			wid_hist = w24cmX30cmButton;
#endif

		    XtSetArg(arg, XmNmenuHistory, wid_hist);
		    XtSetValues(wFilmSizeOptMenu, &arg, 1);
		}
		break;

/* Check magnificationType */
	    case PRN_BFB_K_MAGNIFICATIONTYPE:
		if (strcmp(filmBox0.magnificationType, filmBox.magnificationType) != 0) {
		    strcat(buf, " Server changed the attribute:  Magnification Type\n");
#ifdef RUNTIME
		    if (strcmp(filmBox.magnificationType, "REPLICATE") == 0)
			wid_hist = wReplicateButton;
		    else if (strcmp(filmBox.magnificationType, "BILINEAR") == 0)
			wid_hist = wBilinearButton;
		    else if (strcmp(filmBox.magnificationType, "CUBIC") == 0)
			wid_hist = wCubicButton;
#endif

		    XtSetArg(arg, XmNmenuHistory, wid_hist);
		    XtSetValues(wMagnificationOptMenu, &arg, 1);
		}
		break;

	    }
	}
    }
    XmTextSetString(wErrorMesscrolledText2, buf);
}



void
makeCreateButtonSen()
{
    XtVaSetValues(pushButton5, XmNsensitive, TRUE, NULL);
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void
activateCB_wColumnsTxt(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    _UxCfilmBoxForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxFilmBoxFormContext;
    UxFilmBoxFormContext = UxContext =
	(_UxCfilmBoxForm *) UxGetContext(UxWidget);
    {

    }
    UxFilmBoxFormContext = UxSaveCtx;
}

static void
activateCB_wRowsTxt(
		    Widget wgt,
		    XtPointer cd,
		    XtPointer cb)
{
    _UxCfilmBoxForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxFilmBoxFormContext;
    UxFilmBoxFormContext = UxContext =
	(_UxCfilmBoxForm *) UxGetContext(UxWidget);
    {

    }
    UxFilmBoxFormContext = UxSaveCtx;
}

static void
createCB_wMagnificationOptMenu(
			       Widget wgt,
			       XtPointer cd,
			       XtPointer cb)
{
    _UxCfilmBoxForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxFilmBoxFormContext;
    UxContext = UxFilmBoxFormContext;
    {

    }
    UxFilmBoxFormContext = UxSaveCtx;
}

static void
activateCB_wMaxDensityTxt(
			  Widget wgt,
			  XtPointer cd,
			  XtPointer cb)
{
    _UxCfilmBoxForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxFilmBoxFormContext;
    UxFilmBoxFormContext = UxContext =
	(_UxCfilmBoxForm *) UxGetContext(UxWidget);
    {

    }
    UxFilmBoxFormContext = UxSaveCtx;
}

static void
activateCB_pushButton4(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    _UxCfilmBoxForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxFilmBoxFormContext;
    UxFilmBoxFormContext = UxContext =
	(_UxCfilmBoxForm *) UxGetContext(UxWidget);
    {
	XtUnmanageChild(wFilmBoxForm);
    }
    UxFilmBoxFormContext = UxSaveCtx;
}

static void
activateCB_pushButton5(
		       Widget wgt,
		       XtPointer cd,
		       XtPointer cb)
{
    _UxCfilmBoxForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxFilmBoxFormContext;
    UxFilmBoxFormContext = UxContext =
	(_UxCfilmBoxForm *) UxGetContext(UxWidget);
    {
	CONDITION cond;
	unsigned short status;


	PRN_BASICFILMBOX filmBox0;	/* save a copy of SCU Attri. */

	/* read Film Box Form */
	cond = readFilmBoxForm(&filmBox);
	if (cond != 1) {
	    return;
	} else {

	    /* save a copy of the attributes provided by client(SCU) */
	    filmBox0 = filmBox;

	    strcpy(filmBox.filmBoxSOPInstanceUID, "");
	    strcpy(filmBox.referencedFilmSessionSOPClassUID, DICOM_SOPCLASSBASICFILMSESSION);
	    strcpy(filmBox.referencedFilmSessionSOPInstanceUID, filmSessionSOPInstanceUID);

	    cond = createFilmBox(&associationKey, &associationParameters,
			     &filmBox, DICOM_SOPCLASSGREYSCALEPRINTMGMTMETA,
				 &status);

	    if (cond != APP_NORMAL) {
		MUT_DisplayConditionText(wErrorMesscrolledText2);
		(void) COND_PopCondition(TRUE);
		return;
	    }
	    server_changed_attribute_filmBoxForm(filmBox, filmBox0);

#ifdef RUNTIME
	    if (wStudyListForm == 0)
		wStudyListForm = (Widget) create_studyListForm(NULL);
	    if (wPreviewStudyForm == 0)
		wPreviewStudyForm = (Widget) create_previewStudyForm(NULL);

	    if (wBasicFilmBox_Standard == 0)
		wBasicFilmBox_Standard = (Widget) create_BasicFilmBox_Standard(NULL);

	    /*
	     * if (wImageAttribute == 0) wImageAttribute = (Widget)
	     * create_ImageAttribute(NULL);
	     */

	    /* XtManageChild(wImageAttribute);  */
	    XtManageChild(wStudyListForm);
	    XtManageChild(wPreviewStudyForm);
	    XtManageChild(wBasicFilmBox_Standard);


	    loadPreviewStudyForm(8, 8);

	    loadBasicFilmBox_Standard(r1, c1);

#endif

	    if (!getStudyList())
		XmTextSetString(wErrorMesscrolledText2, " Error getting Study List from function getStudyList\n");
	}

	XtVaSetValues(pushButton5, XmNsensitive, FALSE, NULL);
    }
    UxFilmBoxFormContext = UxSaveCtx;
}

static void
activateCB_wMinDensityTxt(
			  Widget wgt,
			  XtPointer cd,
			  XtPointer cb)
{
    _UxCfilmBoxForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxFilmBoxFormContext;
    UxFilmBoxFormContext = UxContext =
	(_UxCfilmBoxForm *) UxGetContext(UxWidget);
    {

    }
    UxFilmBoxFormContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget
_Uxbuild_filmBoxForm()
{
    Widget _UxParent;
    Widget wFilmSizeMenu_shell;
    Widget wMagnificationMenu_shell;


    /* Creation of filmBoxForm */
    _UxParent = UxParent;
    if (_UxParent == NULL) {
	_UxParent = UxTopLevel;
    }
    _UxParent = XtVaCreatePopupShell("filmBoxForm_shell",
				     xmDialogShellWidgetClass, _UxParent,
				     XmNx, 270,
				     XmNy, 197,
				     XmNwidth, 750,
				     XmNheight, 680,
				     XmNshellUnitType, XmPIXELS,
				     XmNtitle, "filmBoxForm",
				     NULL);

    filmBoxForm = XtVaCreateWidget("filmBoxForm",
				   xmFormWidgetClass,
				   _UxParent,
				   XmNwidth, 750,
				   XmNheight, 680,
				   XmNunitType, XmPIXELS,
				   XmNautoUnmanage, FALSE,
				   NULL);
    UxPutContext(filmBoxForm, (char *) UxFilmBoxFormContext);


    /* Creation of label16 */
    label16 = XtVaCreateManagedWidget("label16",
				      xmLabelWidgetClass,
				      filmBoxForm,
				      XmNx, 40,
				      XmNy, 40,
				      XmNwidth, 100,
				      XmNheight, 30,
				      RES_CONVERT(XmNlabelString, "Columns"),
				      NULL);
    UxPutContext(label16, (char *) UxFilmBoxFormContext);


    /* Creation of label17 */
    label17 = XtVaCreateManagedWidget("label17",
				      xmLabelWidgetClass,
				      filmBoxForm,
				      XmNx, 40,
				      XmNy, 100,
				      XmNwidth, 100,
				      XmNheight, 28,
				      RES_CONVERT(XmNlabelString, "Rows"),
				      NULL);
    UxPutContext(label17, (char *) UxFilmBoxFormContext);


    /* Creation of wColumnsTxt */
    wColumnsTxt = XtVaCreateManagedWidget("wColumnsTxt",
					  xmTextFieldWidgetClass,
					  filmBoxForm,
					  XmNwidth, 70,
					  XmNx, 150,
					  XmNy, 30,
					  XmNheight, 40,
					  XmNvalue, "2",
					  NULL);
    XtAddCallback(wColumnsTxt, XmNactivateCallback,
		  (XtCallbackProc) activateCB_wColumnsTxt,
		  (XtPointer) UxFilmBoxFormContext);

    UxPutContext(wColumnsTxt, (char *) UxFilmBoxFormContext);


    /* Creation of wRowsTxt */
    wRowsTxt = XtVaCreateManagedWidget("wRowsTxt",
				       xmTextFieldWidgetClass,
				       filmBoxForm,
				       XmNwidth, 70,
				       XmNx, 150,
				       XmNy, 90,
				       XmNheight, 40,
				       XmNvalue, "2",
				       NULL);
    XtAddCallback(wRowsTxt, XmNactivateCallback,
		  (XtCallbackProc) activateCB_wRowsTxt,
		  (XtPointer) UxFilmBoxFormContext);

    UxPutContext(wRowsTxt, (char *) UxFilmBoxFormContext);


    /* Creation of label18 */
    label18 = XtVaCreateManagedWidget("label18",
				      xmLabelWidgetClass,
				      filmBoxForm,
				      XmNx, 470,
				      XmNy, 480,
				      XmNwidth, 120,
				      XmNheight, 30,
				      RES_CONVERT(XmNlabelString, "Trim"),
				      NULL);
    UxPutContext(label18, (char *) UxFilmBoxFormContext);


    /* Creation of rowColumn5 */
    rowColumn5 = XtVaCreateManagedWidget("rowColumn5",
					 xmRowColumnWidgetClass,
					 filmBoxForm,
					 XmNwidth, 170,
					 XmNheight, 80,
					 XmNx, 610,
					 XmNy, 460,
					 XmNradioBehavior, TRUE,
					 NULL);
    UxPutContext(rowColumn5, (char *) UxFilmBoxFormContext);


    /* Creation of wTrimYesToggle */
    wTrimYesToggle = XtVaCreateManagedWidget("wTrimYesToggle",
					     xmToggleButtonWidgetClass,
					     rowColumn5,
					     XmNx, 0,
					     XmNy, 10,
					     XmNwidth, 170,
					     XmNheight, 30,
					 RES_CONVERT(XmNlabelString, "Yes"),
					     XmNset, FALSE,
					     NULL);
    UxPutContext(wTrimYesToggle, (char *) UxFilmBoxFormContext);


    /* Creation of wTrimNoToggle */
    wTrimNoToggle = XtVaCreateManagedWidget("wTrimNoToggle",
					    xmToggleButtonWidgetClass,
					    rowColumn5,
					    XmNx, 0,
					    XmNy, 10,
					    XmNwidth, 170,
					    XmNheight, 30,
					  RES_CONVERT(XmNlabelString, "No"),
					    XmNset, FALSE,
					    NULL);
    UxPutContext(wTrimNoToggle, (char *) UxFilmBoxFormContext);


    /* Creation of label19 */
    label19 = XtVaCreateManagedWidget("label19",
				      xmLabelWidgetClass,
				      filmBoxForm,
				      XmNx, 20,
				      XmNy, 410,
				      XmNwidth, 160,
				      XmNheight, 30,
		   RES_CONVERT(XmNlabelString, "Annotation Display Format"),
				      NULL);
    UxPutContext(label19, (char *) UxFilmBoxFormContext);


    /* Creation of wFilmSizeMenu */
    wFilmSizeMenu_shell = XtVaCreatePopupShell("wFilmSizeMenu_shell",
					xmMenuShellWidgetClass, filmBoxForm,
					       XmNwidth, 1,
					       XmNheight, 1,
					       XmNallowShellResize, TRUE,
					       XmNoverrideRedirect, TRUE,
					       NULL);

    wFilmSizeMenu = XtVaCreateWidget("wFilmSizeMenu",
				     xmRowColumnWidgetClass,
				     wFilmSizeMenu_shell,
				     XmNrowColumnType, XmMENU_PULLDOWN,
				     NULL);
    UxPutContext(wFilmSizeMenu, (char *) UxFilmBoxFormContext);


    /* Creation of w8inX10inButton */
    w8inX10inButton = XtVaCreateManagedWidget("w8inX10inButton",
					      xmPushButtonWidgetClass,
					      wFilmSizeMenu,
			       RES_CONVERT(XmNlabelString, "8 in  x 10 in"),
					      NULL);
    UxPutContext(w8inX10inButton, (char *) UxFilmBoxFormContext);


    /* Creation of w10inX12inButton */
    w10inX12inButton = XtVaCreateManagedWidget("w10inX12inButton",
					       xmPushButtonWidgetClass,
					       wFilmSizeMenu,
			       RES_CONVERT(XmNlabelString, "10 in x 12 in"),
					       NULL);
    UxPutContext(w10inX12inButton, (char *) UxFilmBoxFormContext);


    /* Creation of w10inX14inButton */
    w10inX14inButton = XtVaCreateManagedWidget("w10inX14inButton",
					       xmPushButtonWidgetClass,
					       wFilmSizeMenu,
			       RES_CONVERT(XmNlabelString, "10 in x 14 in"),
					       NULL);
    UxPutContext(w10inX14inButton, (char *) UxFilmBoxFormContext);


    /* Creation of w11inX14inButton */
    w11inX14inButton = XtVaCreateManagedWidget("w11inX14inButton",
					       xmPushButtonWidgetClass,
					       wFilmSizeMenu,
			       RES_CONVERT(XmNlabelString, "11 in x 14 in"),
					       NULL);
    UxPutContext(w11inX14inButton, (char *) UxFilmBoxFormContext);


    /* Creation of w14inX14inButton */
    w14inX14inButton = XtVaCreateManagedWidget("w14inX14inButton",
					       xmPushButtonWidgetClass,
					       wFilmSizeMenu,
			       RES_CONVERT(XmNlabelString, "14 in x 14 in"),
					       NULL);
    UxPutContext(w14inX14inButton, (char *) UxFilmBoxFormContext);


    /* Creation of w14inX17inButton */
    w14inX17inButton = XtVaCreateManagedWidget("w14inX17inButton",
					       xmPushButtonWidgetClass,
					       wFilmSizeMenu,
			       RES_CONVERT(XmNlabelString, "14 in x 17 in"),
					       NULL);
    UxPutContext(w14inX17inButton, (char *) UxFilmBoxFormContext);


    /* Creation of w24cmX24cmButton */
    w24cmX24cmButton = XtVaCreateManagedWidget("w24cmX24cmButton",
					       xmPushButtonWidgetClass,
					       wFilmSizeMenu,
			       RES_CONVERT(XmNlabelString, "24 cm x 24 cm"),
					       NULL);
    UxPutContext(w24cmX24cmButton, (char *) UxFilmBoxFormContext);


    /* Creation of w24cmX30cmButton */
    w24cmX30cmButton = XtVaCreateManagedWidget("w24cmX30cmButton",
					       xmPushButtonWidgetClass,
					       wFilmSizeMenu,
			       RES_CONVERT(XmNlabelString, "24 cm x 30 cm"),
					       NULL);
    UxPutContext(w24cmX30cmButton, (char *) UxFilmBoxFormContext);


    /* Creation of wFilmSizeOptMenu */
    wFilmSizeOptMenu = XtVaCreateManagedWidget("wFilmSizeOptMenu",
					       xmRowColumnWidgetClass,
					       filmBoxForm,
					    XmNrowColumnType, XmMENU_OPTION,
					       XmNsubMenuId, wFilmSizeMenu,
					       XmNx, 170,
					       XmNy, 250,
					       XmNwidth, 200,
					       XmNheight, 40,
					       NULL);
    UxPutContext(wFilmSizeOptMenu, (char *) UxFilmBoxFormContext);


    /* Creation of label20 */
    label20 = XtVaCreateManagedWidget("label20",
				      xmLabelWidgetClass,
				      filmBoxForm,
				      XmNx, 30,
				      XmNy, 250,
				      XmNwidth, 100,
				      XmNheight, 30,
				   RES_CONVERT(XmNlabelString, "Film Size"),
				      NULL);
    UxPutContext(label20, (char *) UxFilmBoxFormContext);


    /* Creation of wMagnificationMenu */
    wMagnificationMenu_shell = XtVaCreatePopupShell("wMagnificationMenu_shell",
					xmMenuShellWidgetClass, filmBoxForm,
						    XmNwidth, 1,
						    XmNheight, 1,
						  XmNallowShellResize, TRUE,
						  XmNoverrideRedirect, TRUE,
						    NULL);

    wMagnificationMenu = XtVaCreateWidget("wMagnificationMenu",
					  xmRowColumnWidgetClass,
					  wMagnificationMenu_shell,
					  XmNrowColumnType, XmMENU_PULLDOWN,
					  NULL);
    UxPutContext(wMagnificationMenu, (char *) UxFilmBoxFormContext);


    /* Creation of wReplicateButton */
    wReplicateButton = XtVaCreateManagedWidget("wReplicateButton",
					       xmPushButtonWidgetClass,
					       wMagnificationMenu,
				   RES_CONVERT(XmNlabelString, "Replicate"),
					       NULL);
    UxPutContext(wReplicateButton, (char *) UxFilmBoxFormContext);


    /* Creation of wBilinearButton */
    wBilinearButton = XtVaCreateManagedWidget("wBilinearButton",
					      xmPushButtonWidgetClass,
					      wMagnificationMenu,
				    RES_CONVERT(XmNlabelString, "Bilinear"),
					      NULL);
    UxPutContext(wBilinearButton, (char *) UxFilmBoxFormContext);


    /* Creation of wCubicButton */
    wCubicButton = XtVaCreateManagedWidget("wCubicButton",
					   xmPushButtonWidgetClass,
					   wMagnificationMenu,
				       RES_CONVERT(XmNlabelString, "Cubic"),
					   NULL);
    UxPutContext(wCubicButton, (char *) UxFilmBoxFormContext);


    /* Creation of wMagnificationOptMenu */
    wMagnificationOptMenu = XtVaCreateManagedWidget("wMagnificationOptMenu",
						    xmRowColumnWidgetClass,
						    filmBoxForm,
					    XmNrowColumnType, XmMENU_OPTION,
					   XmNsubMenuId, wMagnificationMenu,
						    XmNx, 180,
						    XmNy, 300,
						    XmNwidth, 120,
						    XmNheight, 30,
						    XmNresizable, TRUE,
						    XmNresizeWidth, TRUE,
						    NULL);
    UxPutContext(wMagnificationOptMenu, (char *) UxFilmBoxFormContext);

    createCB_wMagnificationOptMenu(wMagnificationOptMenu,
			(XtPointer) UxFilmBoxFormContext, (XtPointer) NULL);


    /* Creation of label21 */
    label21 = XtVaCreateManagedWidget("label21",
				      xmLabelWidgetClass,
				      filmBoxForm,
				      XmNx, 34,
				      XmNy, 174,
				      XmNwidth, 100,
				      XmNheight, 30,
				 RES_CONVERT(XmNlabelString, "Orientation"),
				      NULL);
    UxPutContext(label21, (char *) UxFilmBoxFormContext);


    /* Creation of rowColumn6 */
    rowColumn6 = XtVaCreateManagedWidget("rowColumn6",
					 xmRowColumnWidgetClass,
					 filmBoxForm,
					 XmNwidth, 170,
					 XmNheight, 70,
					 XmNx, 530,
					 XmNy, 140,
					 XmNradioBehavior, TRUE,
					 NULL);
    UxPutContext(rowColumn6, (char *) UxFilmBoxFormContext);


    /* Creation of wEmptyImageBlackToggle */
    wEmptyImageBlackToggle = XtVaCreateManagedWidget("wEmptyImageBlackToggle",
						  xmToggleButtonWidgetClass,
						     rowColumn6,
						     XmNx, 30,
						     XmNy, 10,
						     XmNwidth, 90,
						     XmNheight, 20,
				       RES_CONVERT(XmNlabelString, "Black"),
						     XmNset, FALSE,
						     NULL);
    UxPutContext(wEmptyImageBlackToggle, (char *) UxFilmBoxFormContext);


    /* Creation of wEmptyImageWhiteToggle */
    wEmptyImageWhiteToggle = XtVaCreateManagedWidget("wEmptyImageWhiteToggle",
						  xmToggleButtonWidgetClass,
						     rowColumn6,
						     XmNx, 0,
						     XmNy, 30,
						     XmNwidth, 90,
						     XmNheight, 20,
				       RES_CONVERT(XmNlabelString, "White"),
						     NULL);
    UxPutContext(wEmptyImageWhiteToggle, (char *) UxFilmBoxFormContext);


    /* Creation of wEmptyImageOtherToggle */
    wEmptyImageOtherToggle = XtVaCreateManagedWidget("wEmptyImageOtherToggle",
						  xmToggleButtonWidgetClass,
						     rowColumn6,
						     XmNx, 30,
						     XmNy, 10,
						     XmNwidth, 90,
						     XmNheight, 20,
				       RES_CONVERT(XmNlabelString, "Other"),
						     NULL);
    UxPutContext(wEmptyImageOtherToggle, (char *) UxFilmBoxFormContext);


    /* Creation of label22 */
    label22 = XtVaCreateManagedWidget("label22",
				      xmLabelWidgetClass,
				      filmBoxForm,
				      XmNx, 390,
				      XmNy, 60,
				      XmNwidth, 100,
				      XmNheight, 30,
			      RES_CONVERT(XmNlabelString, "Border Density"),
				      NULL);
    UxPutContext(label22, (char *) UxFilmBoxFormContext);


    /* Creation of rowColumn7 */
    rowColumn7 = XtVaCreateManagedWidget("rowColumn7",
					 xmRowColumnWidgetClass,
					 filmBoxForm,
					 XmNwidth, 170,
					 XmNheight, 70,
					 XmNx, 530,
					 XmNy, 30,
					 XmNradioBehavior, TRUE,
					 NULL);
    UxPutContext(rowColumn7, (char *) UxFilmBoxFormContext);


    /* Creation of wBorderBlackToggle */
    wBorderBlackToggle = XtVaCreateManagedWidget("wBorderBlackToggle",
						 xmToggleButtonWidgetClass,
						 rowColumn7,
						 XmNx, 30,
						 XmNy, 10,
						 XmNwidth, 90,
						 XmNheight, 20,
				       RES_CONVERT(XmNlabelString, "Black"),
						 XmNset, FALSE,
						 NULL);
    UxPutContext(wBorderBlackToggle, (char *) UxFilmBoxFormContext);


    /* Creation of wBorderWhiteToggle */
    wBorderWhiteToggle = XtVaCreateManagedWidget("wBorderWhiteToggle",
						 xmToggleButtonWidgetClass,
						 rowColumn7,
						 XmNx, 30,
						 XmNy, 10,
						 XmNwidth, 90,
						 XmNheight, 20,
				       RES_CONVERT(XmNlabelString, "White"),
						 NULL);
    UxPutContext(wBorderWhiteToggle, (char *) UxFilmBoxFormContext);


    /* Creation of wBorderOtherToggle */
    wBorderOtherToggle = XtVaCreateManagedWidget("wBorderOtherToggle",
						 xmToggleButtonWidgetClass,
						 rowColumn7,
						 XmNx, 30,
						 XmNy, 10,
						 XmNwidth, 90,
						 XmNheight, 20,
				       RES_CONVERT(XmNlabelString, "Other"),
						 NULL);
    UxPutContext(wBorderOtherToggle, (char *) UxFilmBoxFormContext);


    /* Creation of label23 */
    label23 = XtVaCreateManagedWidget("label23",
				      xmLabelWidgetClass,
				      filmBoxForm,
				      XmNx, 350,
				      XmNy, 140,
				      XmNwidth, 150,
				      XmNheight, 30,
			 RES_CONVERT(XmNlabelString, "Empty Image Density"),
				      NULL);
    UxPutContext(label23, (char *) UxFilmBoxFormContext);


    /* Creation of label24 */
    label24 = XtVaCreateManagedWidget("label24",
				      xmLabelWidgetClass,
				      filmBoxForm,
				      XmNx, 350,
				      XmNy, 240,
				      XmNwidth, 150,
				      XmNheight, 30,
			     RES_CONVERT(XmNlabelString, "Minimum Density"),
				      NULL);
    UxPutContext(label24, (char *) UxFilmBoxFormContext);


    /* Creation of wMaxDensityTxt */
    wMaxDensityTxt = XtVaCreateManagedWidget("wMaxDensityTxt",
					     xmTextFieldWidgetClass,
					     filmBoxForm,
					     XmNwidth, 170,
					     XmNx, 530,
					     XmNy, 310,
					     XmNheight, 40,
					     XmNvalue, "3.5",
					     NULL);
    XtAddCallback(wMaxDensityTxt, XmNactivateCallback,
		  (XtCallbackProc) activateCB_wMaxDensityTxt,
		  (XtPointer) UxFilmBoxFormContext);

    UxPutContext(wMaxDensityTxt, (char *) UxFilmBoxFormContext);


    /* Creation of wAnnotationDispFormatTxt */
    wAnnotationDispFormatTxt = XtVaCreateManagedWidget("wAnnotationDispFormatTxt",
						     xmTextFieldWidgetClass,
						       filmBoxForm,
						       XmNwidth, 490,
						       XmNx, 210,
						       XmNy, 410,
						       XmNheight, 40,
						       NULL);
    UxPutContext(wAnnotationDispFormatTxt, (char *) UxFilmBoxFormContext);


    /* Creation of label25 */
    label25 = XtVaCreateManagedWidget("label25",
				      xmLabelWidgetClass,
				      filmBoxForm,
				      XmNx, 350,
				      XmNy, 320,
				      XmNwidth, 150,
				      XmNheight, 30,
			     RES_CONVERT(XmNlabelString, "Maximum Density"),
				      NULL);
    UxPutContext(label25, (char *) UxFilmBoxFormContext);


    /* Creation of rowColumn8 */
    rowColumn8 = XtVaCreateManagedWidget("rowColumn8",
					 xmRowColumnWidgetClass,
					 filmBoxForm,
					 XmNwidth, 170,
					 XmNheight, 80,
					 XmNx, 174,
					 XmNy, 154,
					 XmNradioBehavior, TRUE,
					 NULL);
    UxPutContext(rowColumn8, (char *) UxFilmBoxFormContext);


    /* Creation of wPortraitToggle */
    wPortraitToggle = XtVaCreateManagedWidget("wPortraitToggle",
					      xmToggleButtonWidgetClass,
					      rowColumn8,
					      XmNx, 0,
					      XmNy, 10,
					      XmNwidth, 170,
					      XmNheight, 30,
				    RES_CONVERT(XmNlabelString, "Portrait"),
					      XmNset, TRUE,
					      NULL);
    UxPutContext(wPortraitToggle, (char *) UxFilmBoxFormContext);


    /* Creation of wLandscapeToggle */
    wLandscapeToggle = XtVaCreateManagedWidget("wLandscapeToggle",
					       xmToggleButtonWidgetClass,
					       rowColumn8,
					       XmNx, 0,
					       XmNy, 10,
					       XmNwidth, 170,
					       XmNheight, 30,
				   RES_CONVERT(XmNlabelString, "Landscape"),
					       NULL);
    UxPutContext(wLandscapeToggle, (char *) UxFilmBoxFormContext);


    /* Creation of pushButton4 */
    pushButton4 = XtVaCreateManagedWidget("pushButton4",
					  xmPushButtonWidgetClass,
					  filmBoxForm,
					  XmNx, 420,
					  XmNy, 520,
					  XmNwidth, 150,
					  XmNheight, 40,
				      RES_CONVERT(XmNlabelString, "Cancel"),
					  NULL);
    XtAddCallback(pushButton4, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton4,
		  (XtPointer) UxFilmBoxFormContext);

    UxPutContext(pushButton4, (char *) UxFilmBoxFormContext);


    /* Creation of pushButton5 */
    pushButton5 = XtVaCreateManagedWidget("pushButton5",
					  xmPushButtonWidgetClass,
					  filmBoxForm,
					  XmNx, 150,
					  XmNy, 520,
					  XmNwidth, 150,
					  XmNheight, 40,
				      RES_CONVERT(XmNlabelString, "Create"),
					  NULL);
    XtAddCallback(pushButton5, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton5,
		  (XtPointer) UxFilmBoxFormContext);

    UxPutContext(pushButton5, (char *) UxFilmBoxFormContext);


    /* Creation of label28 */
    label28 = XtVaCreateManagedWidget("label28",
				      xmLabelWidgetClass,
				      filmBoxForm,
				      XmNx, 30,
				      XmNy, 300,
				      XmNwidth, 100,
				      XmNheight, 30,
			       RES_CONVERT(XmNlabelString, "Magnification"),
				      NULL);
    UxPutContext(label28, (char *) UxFilmBoxFormContext);


    /* Creation of wMinDensityTxt */
    wMinDensityTxt = XtVaCreateManagedWidget("wMinDensityTxt",
					     xmTextFieldWidgetClass,
					     filmBoxForm,
					     XmNwidth, 170,
					     XmNx, 530,
					     XmNy, 240,
					     XmNheight, 40,
					     XmNvalue, "",
					     NULL);
    XtAddCallback(wMinDensityTxt, XmNactivateCallback,
		  (XtCallbackProc) activateCB_wMinDensityTxt,
		  (XtPointer) UxFilmBoxFormContext);

    UxPutContext(wMinDensityTxt, (char *) UxFilmBoxFormContext);


    /* Creation of label29 */
    label29 = XtVaCreateManagedWidget("label29",
				      xmLabelWidgetClass,
				      filmBoxForm,
				      XmNx, 20,
				      XmNy, 370,
				      XmNwidth, 160,
				      XmNheight, 30,
		   RES_CONVERT(XmNlabelString, "Configuration Information"),
				      NULL);
    UxPutContext(label29, (char *) UxFilmBoxFormContext);


    /* Creation of wConfigurationInfoTxt */
    wConfigurationInfoTxt = XtVaCreateManagedWidget("wConfigurationInfoTxt",
						    xmTextFieldWidgetClass,
						    filmBoxForm,
						    XmNwidth, 490,
						    XmNx, 210,
						    XmNy, 360,
						    XmNheight, 40,
						    NULL);
    UxPutContext(wConfigurationInfoTxt, (char *) UxFilmBoxFormContext);


    /* Creation of wBorderOtherTxt */
    wBorderOtherTxt = XtVaCreateManagedWidget("wBorderOtherTxt",
					      xmTextFieldWidgetClass,
					      filmBoxForm,
					      XmNwidth, 110,
					      XmNx, 610,
					      XmNy, 90,
					      XmNheight, 30,
					      NULL);
    UxPutContext(wBorderOtherTxt, (char *) UxFilmBoxFormContext);


    /* Creation of wEmptyImageOtherTxt */
    wEmptyImageOtherTxt = XtVaCreateManagedWidget("wEmptyImageOtherTxt",
						  xmTextFieldWidgetClass,
						  filmBoxForm,
						  XmNwidth, 110,
						  XmNx, 610,
						  XmNy, 200,
						  XmNheight, 30,
						  NULL);
    UxPutContext(wEmptyImageOtherTxt, (char *) UxFilmBoxFormContext);


    /* Creation of scrolledWindowText3 */
    scrolledWindowText3 = XtVaCreateManagedWidget("scrolledWindowText3",
						xmScrolledWindowWidgetClass,
						  filmBoxForm,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNx, 190,
						  XmNy, 570,
						  NULL);
    UxPutContext(scrolledWindowText3, (char *) UxFilmBoxFormContext);


    /* Creation of wErrorMesscrolledText2 */
    wErrorMesscrolledText2 = XtVaCreateManagedWidget("wErrorMesscrolledText2",
						     xmTextWidgetClass,
						     scrolledWindowText3,
						     XmNwidth, 470,
						     XmNheight, 80,
					     XmNeditMode, XmMULTI_LINE_EDIT,
						     NULL);
    UxPutContext(wErrorMesscrolledText2, (char *) UxFilmBoxFormContext);


    XtAddCallback(filmBoxForm, XmNdestroyCallback,
		  (XtCallbackProc) UxDestroyContextCB,
		  (XtPointer) UxFilmBoxFormContext);


    return (filmBoxForm);
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget
create_filmBoxForm(swidget _UxUxParent)
{
    Widget rtrn;
    _UxCfilmBoxForm *UxContext;

    UxFilmBoxFormContext = UxContext =
	(_UxCfilmBoxForm *) UxNewContext(sizeof(_UxCfilmBoxForm), False);

    UxParent = _UxUxParent;

    rtrn = _Uxbuild_filmBoxForm();

    return (rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/
