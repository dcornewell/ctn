! UIMX ascii 2.5 key: 99                                                        

*filmBoxForm.class: formDialog
*filmBoxForm.gbldecl: /*\
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
** Module Name(s):      create_filmBoxForm\
**			makeCreateButtonSen\
** Author, Date:        Chander Sabharwal\
** Intent:              This module contains the user interface for the film\
**			box (form).  It has a public function that allows the interface\
**			to be displayed and several other public functions.\
** Last Update:         $Author: smm $, $Date: 1996-08-07 19:32:36 $\
** Source File:         $RCSfile: filmBoxForm.i,v $\
** Revision:            $Revision: 1.22 $\
** Status:              $State: Exp $\
*/\
 \
static char rcsid[] = "$Revision: 1.22 $ $RCSfile: filmBoxForm.i,v $";\
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
\
#define  MAXLEN  5000\
\
extern DUL_ASSOCIATIONKEY *associationKey;\
extern DUL_ASSOCIATESERVICEPARAMETERS associationParameters;\
\
static CONDITION readFilmBoxForm(PRN_BASICFILMBOX *fb);\
\
extern Widget wStudyListForm;\
extern Widget  wImageAttribute;\
extern Widget wPreviewStudyForm;\
extern Widget wPreviewPrintForm;\
extern Widget wBasicFilmBox_Standard;\
extern Widget wImageAttribute;\
extern Widget wFilmBoxForm;\
\
extern char filmSessionSOPInstanceUID[];\
\
extern char buf[MAXLEN];\
\
CTNBOOLEAN  getStudyList();\
\
static void  server_changed_attribute_filmBoxForm();\
\
static int  r1,c1;\
\
\
void makeCreateButtonSen();\
\
\
 PRN_BASICFILMBOX  filmBox;
*filmBoxForm.ispecdecl:
*filmBoxForm.funcdecl: swidget create_filmBoxForm(swidget UxParent)\

*filmBoxForm.funcname: create_filmBoxForm
*filmBoxForm.funcdef: "swidget", "<create_filmBoxForm>(%)"
*filmBoxForm.argdecl: swidget UxParent;
*filmBoxForm.arglist: UxParent
*filmBoxForm.arglist.UxParent: "swidget", "%UxParent%"
*filmBoxForm.icode:
*filmBoxForm.fcode: return(rtrn);\

*filmBoxForm.auxdecl: /*readFilmBox\
**\
** Purpose:\
**	This function reads the attributes for filmBox\
**\
** Parameter Dictionary:\
**	fb	pointer to the filmBox Attributes\
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
static CONDITION readFilmBoxForm(PRN_BASICFILMBOX *fb)\
{\
  CONDITION  cond;\
  CTNBOOLEAN nullFlag;  \
  Widget  wid_hist;\
  char  *rc_typed;\
  unsigned short  us;\
  float   rminDensity, rmaxDensity,emptyimagedensity,borderdensity;\
\
  \
/* Reading Columns and Rows displayformat */\
    cond = MUT_ScanWidget(wColumnsTxt, MUT_INT, &nullFlag,\
	 &c1);\
    if (cond != MUT_NORMAL ||  nullFlag)\
    {\
     XmTextSetString(wErrorMesscrolledText2," Error Obtaining Col Value.");\
     return 0;\
    }   \
    if(c1 < 1 || c1 > 8)\
     {\
      XmTextSetString(wErrorMesscrolledText2," Enter column between 1and 8");\
     return 0;\
    }         \
    cond = MUT_ScanWidget(wRowsTxt, MUT_INT, &nullFlag,\
	 &r1);\
    if (cond != MUT_NORMAL ||  nullFlag)\
    {\
     XmTextSetString(wErrorMesscrolledText2," Error Obtaining Row Value.");\
     return  0;\
    }\
    if(r1 < 1 || r1 > 8)\
     {\
      XmTextSetString(wErrorMesscrolledText2," Enter row between 1and 8");\
     return 0;\
    }  \
   sprintf(fb->imageDisplayFormat, "%s\\%-d,%-d",STANDARD,c1,r1);  \
\
\
 /* Reading Orientation */\
    if (XmToggleButtonGetState(wPortraitToggle))\
	strcpy(fb->filmOrientation, PORTRAIT);\
    else\
	strcpy(fb->filmOrientation,LANDSCAPE); \
\
/* Reading Film Size */\
  XtVaGetValues(wFilmSizeOptMenu,\
		XmNmenuHistory, &wid_hist,\
		NULL);\
    if(strcmp(XtName(wid_hist),XtName(w8inX10inButton)) == 0)\
   	strcpy(fb->filmSizeID, SIZE8INX10IN); \
    else if (strcmp(XtName(wid_hist), XtName(w10inX12inButton) ) == 0)\
 	strcpy(fb->filmSizeID, SIZE10INX12IN);  \
   else if (strcmp(XtName(wid_hist),  XtName(w10inX14inButton)) == 0)\
	strcpy(fb->filmSizeID, SIZE10INX14IN); \
   else if (strcmp(XtName(wid_hist), XtName(w11inX14inButton) ) == 0)\
	strcpy(fb->filmSizeID, SIZE11INX14IN); \
   else if (strcmp(XtName(wid_hist) , XtName(w14inX14inButton)) == 0)\
	strcpy(fb->filmSizeID, SIZE14INX14IN); \
   else if (strcmp(XtName(wid_hist) ,XtName(w14inX17inButton)) == 0)\
	strcpy(fb->filmSizeID, SIZE14INX17IN); \
   else if (strcmp(XtName(wid_hist) , XtName(w24cmX24cmButton)) == 0)\
	strcpy(fb->filmSizeID, SIZE24CMX24CM); \
   else if (strcmp(XtName(wid_hist) ,XtName(w24cmX30cmButton)) == 0)\
	strcpy(fb->filmSizeID, SIZE24CMX30CM); \
\
\
/* Reading Magnification */\
   XtVaGetValues(wMagnificationOptMenu,\
		XmNmenuHistory, &wid_hist,\
		NULL);\
   if(strcmp(XtName(wid_hist) , XtName(wReplicateButton)) == 0)\
	strcpy(fb->magnificationType, REPLICATE);\
   else if (strcmp(XtName(wid_hist) , XtName(wBilinearButton)) == 0)\
 	strcpy(fb->magnificationType, BILINEAR);  \
   else if (strcmp(XtName(wid_hist) ,XtName(wCubicButton)) == 0)\
	strcpy(fb->magnificationType, CUBIC); \
   else\
	strcpy(fb->magnificationType, NONE);     \
\
\
   fb->bfbAttributeFlag = PRN_BFB_K_IMAGEDISPLAYFORMAT |\
		PRN_BFB_K_FILMORIENTATION |\
		PRN_BFB_K_FILMSIZEID |\
		PRN_BFB_K_MAGNIFICATIONTYPE |                                \
                                PRN_BFB_K_REFERENCEDBASICFILMSESSIONSEQ;\
\
/* Reading Trim */\
    if (XmToggleButtonGetState(wTrimYesToggle)){\
	strcpy(fb->trim,YES); \
       fb->bfbAttributeFlag |= PRN_BFB_K_TRIM;\
}\
      else if (XmToggleButtonGetState(wTrimNoToggle)){\
	strcpy(fb->trim,NO); \
      fb->bfbAttributeFlag |= PRN_BFB_K_TRIM;\
}\
    else\
      strcpy(fb->trim,""); \
\
\
/* Reading Maximum Density */\
    cond = MUT_ScanWidget(wMaxDensityTxt, MUT_FLOAT, &nullFlag,\
	  &rmaxDensity);\
    if (!nullFlag) {\
	if (cond == MUT_NORMAL) {\
	    rmaxDensity = rmaxDensity*100;\
	    fb->maxDensity = (unsigned short)  rmaxDensity ;     \
	    fb->bfbAttributeFlag |= PRN_BFB_K_MAXDENSITY;\
	} else {\
	    XmTextSetString(wErrorMesscrolledText2, \
		"Error reading Max Density, not a proper Input\n");\
	    return 0;\
	}\
    }\
 \
\
/* Reading Configuration Information */\
    cond = MUT_ScanWidget(wConfigurationInfoTxt, MUT_TEXT, &nullFlag,\
	fb->configurationInfo);\
\
  if (!nullFlag)\
  { \
    if (cond == MUT_NORMAL)\
    { \
\
      fb->bfbAttributeFlag |= PRN_BFB_K_CONFIGURATIONINFO ;\
    }\
 else {\
	XmTextSetString(wErrorMesscrolledText2, \
		"Error reading Configuration, not a proper Input\n");\
	return 0;\
  }\
}\
 \
\
/* Reading Minimum Density */\
    cond = MUT_ScanWidget(wMinDensityTxt, MUT_FLOAT, &nullFlag,\
	  &rminDensity);\
\
  if (!nullFlag)\
  { \
    if (cond == MUT_NORMAL)\
    { \
      if(rminDensity > 0.)\
     {\
      rminDensity = rminDensity*100;\
     fb->minDensity = (unsigned short)  rminDensity ;     \
     fb->bfbAttributeFlag |= PRN_BFB_K_MINDENSITY;\
    }\
  }\
 else {\
	XmTextSetString(wErrorMesscrolledText2, \
		"Error reading Min Density, not a proper Input\n");\
	return 0;\
  }\
}\
\
/* Reading Empty Image Density */\
  if (XmToggleButtonGetState(wEmptyImageBlackToggle)){\
	strcpy(fb->emptyImageDensity,BLACK);\
               fb->bfbAttributeFlag |= PRN_BFB_K_EMPTYIMAGEDENSITY; }\
  else if (XmToggleButtonGetState(wEmptyImageWhiteToggle)){\
	strcpy(fb->emptyImageDensity,WHITE);\
               fb->bfbAttributeFlag |= PRN_BFB_K_EMPTYIMAGEDENSITY; }\
  else\
   {\
     if(XmToggleButtonGetState(wEmptyImageOtherToggle))\
       cond = MUT_ScanWidget(wEmptyImageOtherTxt, MUT_FLOAT, &nullFlag,\
                   &emptyimagedensity);\
         if (!nullFlag)\
        { \
          if (cond == MUT_NORMAL)\
          { \
                     sprintf(fb->emptyImageDensity, "%-d", (int) emptyimagedensity*100);\
                     fb->bfbAttributeFlag |= PRN_BFB_K_EMPTYIMAGEDENSITY;\
           }\
 else {\
	XmTextSetString(wErrorMesscrolledText2, \
		"Error reading Empty Image from Other Txt, not a proper Input\n");\
	return 0;\
      }\
     }\
 } \
       \
  \
/* Reading Border Density */\
  if (XmToggleButtonGetState(wBorderBlackToggle))\
     {\
  	strcpy(fb->borderDensity,BLACK);\
                fb->bfbAttributeFlag |= PRN_BFB_K_BORDERDENSITY;}\
\
    else if (XmToggleButtonGetState(wBorderWhiteToggle)){\
	strcpy(fb->borderDensity,WHITE);\
               fb->bfbAttributeFlag |= PRN_BFB_K_BORDERDENSITY;}\
    else \
    {\
     if (XmToggleButtonGetState(wBorderOtherToggle))\
       cond = MUT_ScanWidget(wBorderOtherTxt, MUT_FLOAT, &nullFlag,\
                    &borderdensity);\
         if (!nullFlag)\
        { \
          if (cond == MUT_NORMAL)\
          { \
\
                    sprintf(fb->borderDensity, "%-d", (int) borderdensity*100);\
                   fb->bfbAttributeFlag |= PRN_BFB_K_BORDERDENSITY; \
         }\
          else {\
	XmTextSetString(wErrorMesscrolledText2, \
		"Error reading Border Density from Other Txt, not a proper Input\n");\
	return 0;\
      }\
    }               \
   }\
\
\
/* Reading Annotation Display Format */\
    cond = MUT_ScanWidget(wAnnotationDispFormatTxt, MUT_TEXT, &nullFlag,\
	fb->annotationDisplayFormatID);\
         if (!nullFlag)\
        { \
          if (cond == MUT_NORMAL)\
          { \
 \
                fb->bfbAttributeFlag |= PRN_BFB_K_ANNOTATIONDISPLAYFORMATID;\
}\
          else {\
	XmTextSetString(wErrorMesscrolledText2, \
		"Error reading Annotation Display Format ID, not a proper Input\n");\
	return 0;\
  }   \
}      \
\
  fb->type = PRN_K_BASICFILMBOX;\
\
 return 1;\
}\
\
\
\
/*server_changed_attribute_filmBoxForm\
**\
*/\
\
static void server_changed_attribute_filmBoxForm(PRN_BASICFILMBOX  filmBox,\
                                                             PRN_BASICFILMBOX  filmBox0)\
				\
{\
\
int    index;\
char  text[80];\
float   realMaxDen, realMinDen;\
 int     col0,col,row0,row;\
 Widget  wid_hist;\
 Arg  arg;\
\
\
    static filmBoxFlagList[] = {\
        PRN_BFB_K_IMAGEDISPLAYFORMAT,\
        PRN_BFB_K_ANNOTATIONDISPLAYFORMATID,\
        PRN_BFB_K_FILMORIENTATION,\
        PRN_BFB_K_FILMSIZEID,\
        PRN_BFB_K_MAGNIFICATIONTYPE,\
        PRN_BFB_K_SMOOTHINGTYPE,\
        PRN_BFB_K_BORDERDENSITY,\
        PRN_BFB_K_EMPTYIMAGEDENSITY,\
        PRN_BFB_K_MINDENSITY,\
        PRN_BFB_K_MAXDENSITY,\
        PRN_BFB_K_TRIM,\
        PRN_BFB_K_CONFIGURATIONINFO,\
        PRN_BFB_K_REFERENCEDBASICFILMSESSIONSEQ,\
        PRN_BFB_K_REFERENCEDBASICIMAGEBOXSEQ,\
        PRN_BFB_K_REFERENCEDBASICANNOTATIONBOXSEQ,\
        PRN_BFB_K_REFPRINTJOBSEQ\
    };\
\
/* If the Attribute is changed from SCP(Server) side, print a message */\
/* and change the value in the form */\
\
   buf[0] = '\0';\
\
   for (index = 0; index < (int) DIM_OF(filmBoxFlagList); index++) {\
        if (filmBox.bfbAttributeFlag & filmBoxFlagList[index]) {\
        switch (filmBoxFlagList[index]) {\
\
/* Check attri. filmOrientation */\
    case PRN_BFB_K_FILMORIENTATION:\
    if(strcmp(filmBox0.filmOrientation, filmBox.filmOrientation) != 0){\
       strcat(buf, " Server changed the attribute:  Film Orientation\n");                    \
       if(strcmp(filmBox.filmOrientation, "PORTRAIT") == 0)\
          XmToggleButtonSetState(wPortraitToggle, TRUE, TRUE);\
       else\
       if(strcmp(filmBox.filmOrientation, "LANDSCAPE") == 0)\
         XmToggleButtonSetState(wLandscapeToggle, TRUE, TRUE); \
 }\
 break;\
\
/* Check attri. trim */\
    case PRN_BFB_K_TRIM:\
    if(strcmp(filmBox0.trim, filmBox.trim) != 0){\
         strcat(buf, " Server changed the attribute:  Trim\n"); \
         if(strcmp(filmBox.trim,"YES") == 0)\
           XmToggleButtonSetState(wTrimYesToggle, TRUE, TRUE);\
         else\
         if(strcmp(filmBox.trim,"No") == 0)\
           XmToggleButtonSetState(wTrimNoToggle, TRUE, TRUE);\
 }\
 break;\
\
/* Check attri. maxDensity */\
    case PRN_BFB_K_MAXDENSITY:\
    if(filmBox0.maxDensity != filmBox.maxDensity){\
       strcat(buf, " Server changed the attribute:  Max Density\n"); \
        realMaxDen = ( (float) filmBox.maxDensity)/100.0;\
        sprintf(text, "%7.2f" ,  realMaxDen);\
        XmTextSetString(wMaxDensityTxt, text);\
   }\
  break;\
\
/* Check mindensity */\
    case PRN_BFB_K_MINDENSITY:\
    if(filmBox0.minDensity != filmBox.minDensity){\
       strcat(buf, " Server changed the attribute:  Min Density\n"); \
       realMinDen = ( (float) filmBox.minDensity)/100.0;\
       sprintf(text, "%7.2f" ,  realMinDen);\
       XmTextSetString(wMinDensityTxt, text);\
}\
break;\
\
/* Check emptyImageDensity */\
    case PRN_BFB_K_EMPTYIMAGEDENSITY:\
    if(strcmp(filmBox0.emptyImageDensity, filmBox.emptyImageDensity) != 0){\
         strcat(buf, " Server changed the attribute:  Empty Image Density\n"); \
          if(strcmp(filmBox.emptyImageDensity, "BLACK") == 0)\
           XmToggleButtonSetState(wEmptyImageBlackToggle, TRUE, TRUE);\
          else if(strcmp(filmBox.emptyImageDensity,"WHITE") == 0)\
           XmToggleButtonSetState(wEmptyImageWhiteToggle, TRUE, TRUE);\
          else {\
              XmToggleButtonSetState(wEmptyImageOtherToggle, TRUE, TRUE);\
              XmTextSetString(wEmptyImageOtherTxt,filmBox.emptyImageDensity);\
           } \
     }                      \
    break;\
\
/* Check borderDensity*/\
   case PRN_BFB_K_BORDERDENSITY:\
    if(strcmp(filmBox0.borderDensity, filmBox.borderDensity) != 0){\
          strcat(buf, " Server changed the attribute:  Border Density\n"); \
       if(strcmp(filmBox.borderDensity, "BLACK") == 0)\
         XmToggleButtonSetState(wBorderBlackToggle, TRUE, TRUE);\
      else if(strcmp(filmBox.borderDensity,"WHITE") == 0)\
           XmToggleButtonSetState(wBorderWhiteToggle, TRUE, TRUE);\
     else{\
        XmToggleButtonSetState(wBorderOtherToggle, TRUE, TRUE);\
        XmTextSetString(wBorderOtherTxt,filmBox.borderDensity);\
     }\
}\
break;\
\
/* Check annotationDisplayFormatID*/\
   case PRN_BFB_K_ANNOTATIONDISPLAYFORMATID:\
    if(strcmp(filmBox0.annotationDisplayFormatID,filmBox.annotationDisplayFormatID) != 0){\
        strcat(buf, " Server changed the attribute:  Annotation Display Format ID\n");\
        XmTextSetString(wAnnotationDispFormatTxt,filmBox.annotationDisplayFormatID);\
      }                        \
 break;\
\
/* check configurationInfo*/\
    case PRN_BFB_K_CONFIGURATIONINFO:\
    if(strcmp(filmBox0.configurationInfo,filmBox.configurationInfo) != 0){\
      strcat(buf, " Server changed the attribute:  Configuration Info\n");\
      XmTextSetString(wConfigurationInfoTxt,filmBox.configurationInfo);\
     }                      \
 break;\
\
/* check imageDisplayFormat */\
     case PRN_BFB_K_IMAGEDISPLAYFORMAT:\
     if(strcmp(filmBox0.imageDisplayFormat, filmBox.imageDisplayFormat) != 0){\
        strcat(buf, " Server changed the attribute:  Image Display Format\n");\
        sscanf(filmBox0.imageDisplayFormat, "STANDARD\\%d,%d", &col0,&row0);\
        sscanf(filmBox.imageDisplayFormat, "STANDARD\\%d,%d", &col,&row);\
        if(row != row0)\
        {\
         sprintf(text,"%d", row);\
         XmTextSetString(wRowsTxt,text);\
         }\
         if(col != col0)\
        {\
          sprintf(text,"%d", col);\
          XmTextSetString(wColumnsTxt,text);\
        }                     \
    }\
   break;\
\
/* Check filmSizeID */\
    case PRN_BFB_K_FILMSIZEID:\
     if(strcmp(filmBox0.filmSizeID ,   filmBox.filmSizeID) != 0){\
     strcat(buf, " Server changed the attribute:  Film Size ID\n"); \
#ifdef RUNTIME\
     if (strcmp(filmBox.filmSizeID, SIZE8INX10IN) == 0)\
        wid_hist = w8inX10inButton;\
     else if (strcmp(filmBox.filmSizeID,SIZE10INX12IN) == 0)\
       wid_hist = w10inX12inButton;\
     else if (strcmp(filmBox.filmSizeID,SIZE10INX14IN) == 0)\
       wid_hist = w10inX14inButton;\
     else if (strcmp(filmBox.filmSizeID,SIZE11INX14IN) == 0)\
       wid_hist = w11inX14inButton;\
     else if (strcmp(filmBox.filmSizeID, SIZE14INX14IN) == 0)\
       wid_hist = w14inX14inButton;\
     else if (strcmp(filmBox.filmSizeID,SIZE14INX17IN) == 0)		                                     \
         wid_hist = w14inX17inButton;\
     else if (strcmp(filmBox.filmSizeID,SIZE24CMX24CM) == 0)\
         wid_hist = w24cmX24cmButton;\
      else if (strcmp(filmBox.filmSizeID,SIZE24CMX30CM) == 0)\
         wid_hist = w24cmX30cmButton;\
#endif\
\
      XtSetArg(arg, XmNmenuHistory, wid_hist);\
      XtSetValues(wFilmSizeOptMenu, &arg, 1);                    \
  }\
 break;\
\
/* Check magnificationType */\
    case PRN_BFB_K_MAGNIFICATIONTYPE:\
    if(strcmp(filmBox0.magnificationType, filmBox.magnificationType) != 0){\
       strcat(buf, " Server changed the attribute:  Magnification Type\n"); \
#ifdef RUNTIME\
        if(strcmp(filmBox.magnificationType, "REPLICATE") == 0)\
           wid_hist = wReplicateButton;\
         else if (strcmp(filmBox.magnificationType, "BILINEAR") == 0)\
            wid_hist = wBilinearButton;\
         else if (strcmp(filmBox.magnificationType, "CUBIC") == 0)\
             wid_hist = wCubicButton;\
#endif\
\
         XtSetArg(arg, XmNmenuHistory, wid_hist);\
         XtSetValues(wMagnificationOptMenu, &arg, 1);\
   }\
   break;\
\
  }\
 }\
}\
     XmTextSetString(wErrorMesscrolledText2, buf); \
}\
\
\
\
void makeCreateButtonSen()\
{\
 XtVaSetValues(pushButton5, XmNsensitive, TRUE, NULL);\
}
*filmBoxForm.static: true
*filmBoxForm.name: filmBoxForm
*filmBoxForm.parent: NO_PARENT
*filmBoxForm.parentExpression: UxParent
*filmBoxForm.defaultShell: topLevelShell
*filmBoxForm.width: 750
*filmBoxForm.height: 680
*filmBoxForm.isCompound: "true"
*filmBoxForm.compoundIcon: "formD.xpm"
*filmBoxForm.compoundName: "form_Dialog"
*filmBoxForm.x: 270
*filmBoxForm.y: 197
*filmBoxForm.unitType: "pixels"
*filmBoxForm.autoUnmanage: "false"

*label16.class: label
*label16.static: true
*label16.name: label16
*label16.parent: filmBoxForm
*label16.isCompound: "true"
*label16.compoundIcon: "label.xpm"
*label16.compoundName: "label_"
*label16.x: 40
*label16.y: 40
*label16.width: 100
*label16.height: 30
*label16.labelString: "Columns"

*label17.class: label
*label17.static: true
*label17.name: label17
*label17.parent: filmBoxForm
*label17.isCompound: "true"
*label17.compoundIcon: "label.xpm"
*label17.compoundName: "label_"
*label17.x: 40
*label17.y: 100
*label17.width: 100
*label17.height: 28
*label17.labelString: "Rows"

*wColumnsTxt.class: textField
*wColumnsTxt.static: true
*wColumnsTxt.name: wColumnsTxt
*wColumnsTxt.parent: filmBoxForm
*wColumnsTxt.width: 70
*wColumnsTxt.isCompound: "true"
*wColumnsTxt.compoundIcon: "textfield.xpm"
*wColumnsTxt.compoundName: "text_Field"
*wColumnsTxt.x: 150
*wColumnsTxt.y: 30
*wColumnsTxt.height: 40
*wColumnsTxt.activateCallback: {\
\
}
*wColumnsTxt.text: "2"

*wRowsTxt.class: textField
*wRowsTxt.static: true
*wRowsTxt.name: wRowsTxt
*wRowsTxt.parent: filmBoxForm
*wRowsTxt.width: 70
*wRowsTxt.isCompound: "true"
*wRowsTxt.compoundIcon: "textfield.xpm"
*wRowsTxt.compoundName: "text_Field"
*wRowsTxt.x: 150
*wRowsTxt.y: 90
*wRowsTxt.height: 40
*wRowsTxt.activateCallback: {\
\
}
*wRowsTxt.text: "2"

*label18.class: label
*label18.static: true
*label18.name: label18
*label18.parent: filmBoxForm
*label18.isCompound: "true"
*label18.compoundIcon: "label.xpm"
*label18.compoundName: "label_"
*label18.x: 470
*label18.y: 480
*label18.width: 120
*label18.height: 30
*label18.labelString: "Trim"

*rowColumn5.class: rowColumn
*rowColumn5.static: true
*rowColumn5.name: rowColumn5
*rowColumn5.parent: filmBoxForm
*rowColumn5.width: 170
*rowColumn5.height: 80
*rowColumn5.isCompound: "true"
*rowColumn5.compoundIcon: "row.xpm"
*rowColumn5.compoundName: "row_Column"
*rowColumn5.x: 610
*rowColumn5.y: 460
*rowColumn5.radioBehavior: "true"

*wTrimYesToggle.class: toggleButton
*wTrimYesToggle.static: true
*wTrimYesToggle.name: wTrimYesToggle
*wTrimYesToggle.parent: rowColumn5
*wTrimYesToggle.isCompound: "true"
*wTrimYesToggle.compoundIcon: "toggle.xpm"
*wTrimYesToggle.compoundName: "toggle_Button"
*wTrimYesToggle.x: 0
*wTrimYesToggle.y: 10
*wTrimYesToggle.width: 170
*wTrimYesToggle.height: 30
*wTrimYesToggle.labelString: "Yes"
*wTrimYesToggle.set: "false"

*wTrimNoToggle.class: toggleButton
*wTrimNoToggle.static: true
*wTrimNoToggle.name: wTrimNoToggle
*wTrimNoToggle.parent: rowColumn5
*wTrimNoToggle.isCompound: "true"
*wTrimNoToggle.compoundIcon: "toggle.xpm"
*wTrimNoToggle.compoundName: "toggle_Button"
*wTrimNoToggle.x: 0
*wTrimNoToggle.y: 10
*wTrimNoToggle.width: 170
*wTrimNoToggle.height: 30
*wTrimNoToggle.labelString: "No"
*wTrimNoToggle.set: "false"

*label19.class: label
*label19.static: true
*label19.name: label19
*label19.parent: filmBoxForm
*label19.isCompound: "true"
*label19.compoundIcon: "label.xpm"
*label19.compoundName: "label_"
*label19.x: 20
*label19.y: 410
*label19.width: 160
*label19.height: 30
*label19.labelString: "Annotation Display Format"

*wFilmSizeOptMenu.class: rowColumn
*wFilmSizeOptMenu.static: true
*wFilmSizeOptMenu.name: wFilmSizeOptMenu
*wFilmSizeOptMenu.parent: filmBoxForm
*wFilmSizeOptMenu.rowColumnType: "menu_option"
*wFilmSizeOptMenu.subMenuId: "wFilmSizeMenu"
*wFilmSizeOptMenu.isCompound: "true"
*wFilmSizeOptMenu.compoundIcon: "optionM.xpm"
*wFilmSizeOptMenu.compoundName: "option_Menu"
*wFilmSizeOptMenu.compoundEditor: {\
extern swidget UxGUIMePopup UXPROTO((swidget, swidget, int, int));\
UxGUIMePopup(UxThisWidget, NULL, 2, 0);\
}
*wFilmSizeOptMenu.x: 170
*wFilmSizeOptMenu.y: 250
*wFilmSizeOptMenu.width: 200
*wFilmSizeOptMenu.height: 40

*wFilmSizeMenu.class: rowColumn
*wFilmSizeMenu.static: true
*wFilmSizeMenu.name: wFilmSizeMenu
*wFilmSizeMenu.parent: wFilmSizeOptMenu
*wFilmSizeMenu.rowColumnType: "menu_pulldown"

*w8inX10inButton.class: pushButton
*w8inX10inButton.static: true
*w8inX10inButton.name: w8inX10inButton
*w8inX10inButton.parent: wFilmSizeMenu
*w8inX10inButton.labelString: "8 in  x 10 in"

*w10inX12inButton.class: pushButton
*w10inX12inButton.static: true
*w10inX12inButton.name: w10inX12inButton
*w10inX12inButton.parent: wFilmSizeMenu
*w10inX12inButton.labelString: "10 in x 12 in"

*w10inX14inButton.class: pushButton
*w10inX14inButton.static: true
*w10inX14inButton.name: w10inX14inButton
*w10inX14inButton.parent: wFilmSizeMenu
*w10inX14inButton.labelString: "10 in x 14 in"

*w11inX14inButton.class: pushButton
*w11inX14inButton.static: true
*w11inX14inButton.name: w11inX14inButton
*w11inX14inButton.parent: wFilmSizeMenu
*w11inX14inButton.labelString: "11 in x 14 in"

*w14inX14inButton.class: pushButton
*w14inX14inButton.static: true
*w14inX14inButton.name: w14inX14inButton
*w14inX14inButton.parent: wFilmSizeMenu
*w14inX14inButton.labelString: "14 in x 14 in"

*w14inX17inButton.class: pushButton
*w14inX17inButton.static: true
*w14inX17inButton.name: w14inX17inButton
*w14inX17inButton.parent: wFilmSizeMenu
*w14inX17inButton.labelString: "14 in x 17 in"

*w24cmX24cmButton.class: pushButton
*w24cmX24cmButton.static: true
*w24cmX24cmButton.name: w24cmX24cmButton
*w24cmX24cmButton.parent: wFilmSizeMenu
*w24cmX24cmButton.labelString: "24 cm x 24 cm"

*w24cmX30cmButton.class: pushButton
*w24cmX30cmButton.static: true
*w24cmX30cmButton.name: w24cmX30cmButton
*w24cmX30cmButton.parent: wFilmSizeMenu
*w24cmX30cmButton.labelString: "24 cm x 30 cm"

*label20.class: label
*label20.static: true
*label20.name: label20
*label20.parent: filmBoxForm
*label20.isCompound: "true"
*label20.compoundIcon: "label.xpm"
*label20.compoundName: "label_"
*label20.x: 30
*label20.y: 250
*label20.width: 100
*label20.height: 30
*label20.labelString: "Film Size"

*wMagnificationOptMenu.class: rowColumn
*wMagnificationOptMenu.static: true
*wMagnificationOptMenu.name: wMagnificationOptMenu
*wMagnificationOptMenu.parent: filmBoxForm
*wMagnificationOptMenu.rowColumnType: "menu_option"
*wMagnificationOptMenu.subMenuId: "wMagnificationMenu"
*wMagnificationOptMenu.isCompound: "true"
*wMagnificationOptMenu.compoundIcon: "optionM.xpm"
*wMagnificationOptMenu.compoundName: "option_Menu"
*wMagnificationOptMenu.compoundEditor: {\
extern swidget UxGUIMePopup UXPROTO((swidget, swidget, int, int));\
UxGUIMePopup(UxThisWidget, NULL, 2, 0);\
}
*wMagnificationOptMenu.x: 180
*wMagnificationOptMenu.y: 300
*wMagnificationOptMenu.width: 120
*wMagnificationOptMenu.height: 30
*wMagnificationOptMenu.resizable: "true"
*wMagnificationOptMenu.resizeRecursion: "up"
*wMagnificationOptMenu.resizeWidth: "true"
*wMagnificationOptMenu.isResizable: "true"
*wMagnificationOptMenu.createCallback: {\
\
}

*wMagnificationMenu.class: rowColumn
*wMagnificationMenu.static: true
*wMagnificationMenu.name: wMagnificationMenu
*wMagnificationMenu.parent: wMagnificationOptMenu
*wMagnificationMenu.rowColumnType: "menu_pulldown"

*wReplicateButton.class: pushButton
*wReplicateButton.static: true
*wReplicateButton.name: wReplicateButton
*wReplicateButton.parent: wMagnificationMenu
*wReplicateButton.labelString: "Replicate"
*wReplicateButton.activateCallback.source: public
*wReplicateButton.activateCallback: 

*wBilinearButton.class: pushButton
*wBilinearButton.static: true
*wBilinearButton.name: wBilinearButton
*wBilinearButton.parent: wMagnificationMenu
*wBilinearButton.labelString: "Bilinear"
*wBilinearButton.activateCallback.source: public
*wBilinearButton.activateCallback: 

*wCubicButton.class: pushButton
*wCubicButton.static: true
*wCubicButton.name: wCubicButton
*wCubicButton.parent: wMagnificationMenu
*wCubicButton.labelString: "Cubic"
*wCubicButton.activateCallback.source: public
*wCubicButton.activateCallback: 

*label21.class: label
*label21.static: true
*label21.name: label21
*label21.parent: filmBoxForm
*label21.isCompound: "true"
*label21.compoundIcon: "label.xpm"
*label21.compoundName: "label_"
*label21.x: 34
*label21.y: 174
*label21.width: 100
*label21.height: 30
*label21.labelString: "Orientation"

*rowColumn6.class: rowColumn
*rowColumn6.static: true
*rowColumn6.name: rowColumn6
*rowColumn6.parent: filmBoxForm
*rowColumn6.width: 170
*rowColumn6.height: 70
*rowColumn6.isCompound: "true"
*rowColumn6.compoundIcon: "row.xpm"
*rowColumn6.compoundName: "row_Column"
*rowColumn6.x: 530
*rowColumn6.y: 140
*rowColumn6.radioBehavior: "true"

*wEmptyImageBlackToggle.class: toggleButton
*wEmptyImageBlackToggle.static: true
*wEmptyImageBlackToggle.name: wEmptyImageBlackToggle
*wEmptyImageBlackToggle.parent: rowColumn6
*wEmptyImageBlackToggle.isCompound: "true"
*wEmptyImageBlackToggle.compoundIcon: "toggle.xpm"
*wEmptyImageBlackToggle.compoundName: "toggle_Button"
*wEmptyImageBlackToggle.x: 30
*wEmptyImageBlackToggle.y: 10
*wEmptyImageBlackToggle.width: 90
*wEmptyImageBlackToggle.height: 20
*wEmptyImageBlackToggle.labelString: "Black"
*wEmptyImageBlackToggle.set: "false"

*wEmptyImageWhiteToggle.class: toggleButton
*wEmptyImageWhiteToggle.static: true
*wEmptyImageWhiteToggle.name: wEmptyImageWhiteToggle
*wEmptyImageWhiteToggle.parent: rowColumn6
*wEmptyImageWhiteToggle.isCompound: "true"
*wEmptyImageWhiteToggle.compoundIcon: "toggle.xpm"
*wEmptyImageWhiteToggle.compoundName: "toggle_Button"
*wEmptyImageWhiteToggle.x: 0
*wEmptyImageWhiteToggle.y: 30
*wEmptyImageWhiteToggle.width: 90
*wEmptyImageWhiteToggle.height: 20
*wEmptyImageWhiteToggle.labelString: "White"

*wEmptyImageOtherToggle.class: toggleButton
*wEmptyImageOtherToggle.static: true
*wEmptyImageOtherToggle.name: wEmptyImageOtherToggle
*wEmptyImageOtherToggle.parent: rowColumn6
*wEmptyImageOtherToggle.isCompound: "true"
*wEmptyImageOtherToggle.compoundIcon: "toggle.xpm"
*wEmptyImageOtherToggle.compoundName: "toggle_Button"
*wEmptyImageOtherToggle.x: 30
*wEmptyImageOtherToggle.y: 10
*wEmptyImageOtherToggle.width: 90
*wEmptyImageOtherToggle.height: 20
*wEmptyImageOtherToggle.labelString: "Other"

*label22.class: label
*label22.static: true
*label22.name: label22
*label22.parent: filmBoxForm
*label22.isCompound: "true"
*label22.compoundIcon: "label.xpm"
*label22.compoundName: "label_"
*label22.x: 390
*label22.y: 60
*label22.width: 100
*label22.height: 30
*label22.labelString: "Border Density"

*rowColumn7.class: rowColumn
*rowColumn7.static: true
*rowColumn7.name: rowColumn7
*rowColumn7.parent: filmBoxForm
*rowColumn7.width: 170
*rowColumn7.height: 70
*rowColumn7.isCompound: "true"
*rowColumn7.compoundIcon: "row.xpm"
*rowColumn7.compoundName: "row_Column"
*rowColumn7.x: 530
*rowColumn7.y: 30
*rowColumn7.radioBehavior: "true"

*wBorderBlackToggle.class: toggleButton
*wBorderBlackToggle.static: true
*wBorderBlackToggle.name: wBorderBlackToggle
*wBorderBlackToggle.parent: rowColumn7
*wBorderBlackToggle.isCompound: "true"
*wBorderBlackToggle.compoundIcon: "toggle.xpm"
*wBorderBlackToggle.compoundName: "toggle_Button"
*wBorderBlackToggle.x: 30
*wBorderBlackToggle.y: 10
*wBorderBlackToggle.width: 90
*wBorderBlackToggle.height: 20
*wBorderBlackToggle.labelString: "Black"
*wBorderBlackToggle.set: "false"

*wBorderWhiteToggle.class: toggleButton
*wBorderWhiteToggle.static: true
*wBorderWhiteToggle.name: wBorderWhiteToggle
*wBorderWhiteToggle.parent: rowColumn7
*wBorderWhiteToggle.isCompound: "true"
*wBorderWhiteToggle.compoundIcon: "toggle.xpm"
*wBorderWhiteToggle.compoundName: "toggle_Button"
*wBorderWhiteToggle.x: 30
*wBorderWhiteToggle.y: 10
*wBorderWhiteToggle.width: 90
*wBorderWhiteToggle.height: 20
*wBorderWhiteToggle.labelString: "White"

*wBorderOtherToggle.class: toggleButton
*wBorderOtherToggle.static: true
*wBorderOtherToggle.name: wBorderOtherToggle
*wBorderOtherToggle.parent: rowColumn7
*wBorderOtherToggle.isCompound: "true"
*wBorderOtherToggle.compoundIcon: "toggle.xpm"
*wBorderOtherToggle.compoundName: "toggle_Button"
*wBorderOtherToggle.x: 30
*wBorderOtherToggle.y: 10
*wBorderOtherToggle.width: 90
*wBorderOtherToggle.height: 20
*wBorderOtherToggle.labelString: "Other"

*label23.class: label
*label23.static: true
*label23.name: label23
*label23.parent: filmBoxForm
*label23.isCompound: "true"
*label23.compoundIcon: "label.xpm"
*label23.compoundName: "label_"
*label23.x: 350
*label23.y: 140
*label23.width: 150
*label23.height: 30
*label23.labelString: "Empty Image Density"

*label24.class: label
*label24.static: true
*label24.name: label24
*label24.parent: filmBoxForm
*label24.isCompound: "true"
*label24.compoundIcon: "label.xpm"
*label24.compoundName: "label_"
*label24.x: 350
*label24.y: 240
*label24.width: 150
*label24.height: 30
*label24.labelString: "Minimum Density"

*wMaxDensityTxt.class: textField
*wMaxDensityTxt.static: true
*wMaxDensityTxt.name: wMaxDensityTxt
*wMaxDensityTxt.parent: filmBoxForm
*wMaxDensityTxt.width: 170
*wMaxDensityTxt.isCompound: "true"
*wMaxDensityTxt.compoundIcon: "textfield.xpm"
*wMaxDensityTxt.compoundName: "text_Field"
*wMaxDensityTxt.x: 530
*wMaxDensityTxt.y: 310
*wMaxDensityTxt.height: 40
*wMaxDensityTxt.activateCallback: {\
\
}
*wMaxDensityTxt.text: "3.5"

*wAnnotationDispFormatTxt.class: textField
*wAnnotationDispFormatTxt.static: true
*wAnnotationDispFormatTxt.name: wAnnotationDispFormatTxt
*wAnnotationDispFormatTxt.parent: filmBoxForm
*wAnnotationDispFormatTxt.width: 490
*wAnnotationDispFormatTxt.isCompound: "true"
*wAnnotationDispFormatTxt.compoundIcon: "textfield.xpm"
*wAnnotationDispFormatTxt.compoundName: "text_Field"
*wAnnotationDispFormatTxt.x: 210
*wAnnotationDispFormatTxt.y: 410
*wAnnotationDispFormatTxt.height: 40

*label25.class: label
*label25.static: true
*label25.name: label25
*label25.parent: filmBoxForm
*label25.isCompound: "true"
*label25.compoundIcon: "label.xpm"
*label25.compoundName: "label_"
*label25.x: 350
*label25.y: 320
*label25.width: 150
*label25.height: 30
*label25.labelString: "Maximum Density"

*rowColumn8.class: rowColumn
*rowColumn8.static: true
*rowColumn8.name: rowColumn8
*rowColumn8.parent: filmBoxForm
*rowColumn8.width: 170
*rowColumn8.height: 80
*rowColumn8.isCompound: "true"
*rowColumn8.compoundIcon: "row.xpm"
*rowColumn8.compoundName: "row_Column"
*rowColumn8.x: 174
*rowColumn8.y: 154
*rowColumn8.radioBehavior: "true"

*wPortraitToggle.class: toggleButton
*wPortraitToggle.static: true
*wPortraitToggle.name: wPortraitToggle
*wPortraitToggle.parent: rowColumn8
*wPortraitToggle.isCompound: "true"
*wPortraitToggle.compoundIcon: "toggle.xpm"
*wPortraitToggle.compoundName: "toggle_Button"
*wPortraitToggle.x: 0
*wPortraitToggle.y: 10
*wPortraitToggle.width: 170
*wPortraitToggle.height: 30
*wPortraitToggle.labelString: "Portrait"
*wPortraitToggle.set: "true"

*wLandscapeToggle.class: toggleButton
*wLandscapeToggle.static: true
*wLandscapeToggle.name: wLandscapeToggle
*wLandscapeToggle.parent: rowColumn8
*wLandscapeToggle.isCompound: "true"
*wLandscapeToggle.compoundIcon: "toggle.xpm"
*wLandscapeToggle.compoundName: "toggle_Button"
*wLandscapeToggle.x: 0
*wLandscapeToggle.y: 10
*wLandscapeToggle.width: 170
*wLandscapeToggle.height: 30
*wLandscapeToggle.labelString: "Landscape"

*pushButton4.class: pushButton
*pushButton4.static: true
*pushButton4.name: pushButton4
*pushButton4.parent: filmBoxForm
*pushButton4.isCompound: "true"
*pushButton4.compoundIcon: "push.xpm"
*pushButton4.compoundName: "push_Button"
*pushButton4.x: 420
*pushButton4.y: 520
*pushButton4.width: 150
*pushButton4.height: 40
*pushButton4.labelString: "Cancel"
*pushButton4.activateCallback: {\
XtUnmanageChild(wFilmBoxForm);\
}

*pushButton5.class: pushButton
*pushButton5.static: true
*pushButton5.name: pushButton5
*pushButton5.parent: filmBoxForm
*pushButton5.isCompound: "true"
*pushButton5.compoundIcon: "push.xpm"
*pushButton5.compoundName: "push_Button"
*pushButton5.x: 150
*pushButton5.y: 520
*pushButton5.width: 150
*pushButton5.height: 40
*pushButton5.labelString: "Create"
*pushButton5.activateCallback: {\
CONDITION  cond;\
unsigned short  status;\
\
\
 PRN_BASICFILMBOX  filmBox0; /* save a copy of SCU Attri. */\
\
/* read Film Box Form */\
    cond  = readFilmBoxForm(&filmBox);\
    if (cond != 1){\
         return;\
   }\
   else\
  {\
 \
/* save a copy of the attributes provided by client(SCU) */    \
    filmBox0 = filmBox;\
\
    strcpy(filmBox.filmBoxSOPInstanceUID, "");\
    strcpy(filmBox.referencedFilmSessionSOPClassUID,DICOM_SOPCLASSBASICFILMSESSION);\
    strcpy(filmBox.referencedFilmSessionSOPInstanceUID, filmSessionSOPInstanceUID);\
\
     cond = createFilmBox(&associationKey, &associationParameters,\
	 &filmBox, DICOM_SOPCLASSGREYSCALEPRINTMGMTMETA,\
	 &status);\
\
     if (cond != APP_NORMAL) {\
	 MUT_DisplayConditionText(wErrorMesscrolledText2);\
	 (void) COND_PopCondition(TRUE);\
                  return;\
     }\
     server_changed_attribute_filmBoxForm(filmBox,filmBox0);\
\
#ifdef RUNTIME\
    if (wStudyListForm == 0)\
	wStudyListForm = (Widget) create_studyListForm(NULL);\
    if (wPreviewStudyForm == 0)\
	wPreviewStudyForm = (Widget) create_previewStudyForm(NULL);\
\
    if (wBasicFilmBox_Standard == 0)\
	wBasicFilmBox_Standard = (Widget) create_BasicFilmBox_Standard(NULL);\
\
/*    if (wImageAttribute == 0)\
	wImageAttribute = (Widget) create_ImageAttribute(NULL);  */\
\
/*	XtManageChild(wImageAttribute);  */\
	XtManageChild(wStudyListForm);\
	XtManageChild(wPreviewStudyForm);\
	XtManageChild(wBasicFilmBox_Standard);\
\
\
    loadPreviewStudyForm(8, 8);\
 \
    loadBasicFilmBox_Standard(r1,c1);\
  \
#endif\
\
if(!getStudyList())\
    XmTextSetString(wErrorMesscrolledText2, " Error getting Study List from function getStudyList\n");\
}\
\
XtVaSetValues(pushButton5, XmNsensitive, FALSE, NULL);\
}

*label28.class: label
*label28.static: true
*label28.name: label28
*label28.parent: filmBoxForm
*label28.isCompound: "true"
*label28.compoundIcon: "label.xpm"
*label28.compoundName: "label_"
*label28.x: 30
*label28.y: 300
*label28.width: 100
*label28.height: 30
*label28.labelString: "Magnification"

*wMinDensityTxt.class: textField
*wMinDensityTxt.static: true
*wMinDensityTxt.name: wMinDensityTxt
*wMinDensityTxt.parent: filmBoxForm
*wMinDensityTxt.width: 170
*wMinDensityTxt.isCompound: "true"
*wMinDensityTxt.compoundIcon: "textfield.xpm"
*wMinDensityTxt.compoundName: "text_Field"
*wMinDensityTxt.x: 530
*wMinDensityTxt.y: 240
*wMinDensityTxt.height: 40
*wMinDensityTxt.activateCallback: {\
\
}
*wMinDensityTxt.text: ""

*label29.class: label
*label29.static: true
*label29.name: label29
*label29.parent: filmBoxForm
*label29.isCompound: "true"
*label29.compoundIcon: "label.xpm"
*label29.compoundName: "label_"
*label29.x: 20
*label29.y: 370
*label29.width: 160
*label29.height: 30
*label29.labelString: "Configuration Information"

*wConfigurationInfoTxt.class: textField
*wConfigurationInfoTxt.static: true
*wConfigurationInfoTxt.name: wConfigurationInfoTxt
*wConfigurationInfoTxt.parent: filmBoxForm
*wConfigurationInfoTxt.width: 490
*wConfigurationInfoTxt.isCompound: "true"
*wConfigurationInfoTxt.compoundIcon: "textfield.xpm"
*wConfigurationInfoTxt.compoundName: "text_Field"
*wConfigurationInfoTxt.x: 210
*wConfigurationInfoTxt.y: 360
*wConfigurationInfoTxt.height: 40

*wBorderOtherTxt.class: textField
*wBorderOtherTxt.static: true
*wBorderOtherTxt.name: wBorderOtherTxt
*wBorderOtherTxt.parent: filmBoxForm
*wBorderOtherTxt.width: 110
*wBorderOtherTxt.isCompound: "true"
*wBorderOtherTxt.compoundIcon: "textfield.xpm"
*wBorderOtherTxt.compoundName: "text_Field"
*wBorderOtherTxt.x: 610
*wBorderOtherTxt.y: 90
*wBorderOtherTxt.height: 30

*wEmptyImageOtherTxt.class: textField
*wEmptyImageOtherTxt.static: true
*wEmptyImageOtherTxt.name: wEmptyImageOtherTxt
*wEmptyImageOtherTxt.parent: filmBoxForm
*wEmptyImageOtherTxt.width: 110
*wEmptyImageOtherTxt.isCompound: "true"
*wEmptyImageOtherTxt.compoundIcon: "textfield.xpm"
*wEmptyImageOtherTxt.compoundName: "text_Field"
*wEmptyImageOtherTxt.x: 610
*wEmptyImageOtherTxt.y: 200
*wEmptyImageOtherTxt.height: 30

*scrolledWindowText3.class: scrolledWindow
*scrolledWindowText3.static: true
*scrolledWindowText3.name: scrolledWindowText3
*scrolledWindowText3.parent: filmBoxForm
*scrolledWindowText3.scrollingPolicy: "application_defined"
*scrolledWindowText3.visualPolicy: "variable"
*scrolledWindowText3.scrollBarDisplayPolicy: "static"
*scrolledWindowText3.isCompound: "true"
*scrolledWindowText3.compoundIcon: "scrltext.xpm"
*scrolledWindowText3.compoundName: "scrolled_Text"
*scrolledWindowText3.x: 190
*scrolledWindowText3.y: 570

*wErrorMesscrolledText2.class: scrolledText
*wErrorMesscrolledText2.static: true
*wErrorMesscrolledText2.name: wErrorMesscrolledText2
*wErrorMesscrolledText2.parent: scrolledWindowText3
*wErrorMesscrolledText2.width: 470
*wErrorMesscrolledText2.height: 80
*wErrorMesscrolledText2.editMode: "multi_line_edit"

