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
**				DICOM 93
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	PRN_BuildObject
**			PRN_ParseObject
**	Private Modules
**			buildObject
**			parseObject
**			dumpBasicPrinter
**			dumpBasicFilmSession
**			dumpBasicFilmBox
**			dumpBasicImageBox
** Author, Date:	Aniruddha S. Gokhale
**			Stephen M. Moore
** Intent:
** Last Update:		$Author: smm $, $Date: 1998-04-17 14:29:59 $
** Source File:		$RCSfile: print.c,v $
** Revision:		$Revision: 1.27 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.27 $ $RCSfile: print.c,v $";

#include <stdio.h>
#include <string.h>
#include <errno.h>
#ifndef MACOS
#include <stdlib.h>
#endif
#include <stdarg.h>
#include <sys/types.h>

#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "dicom_objects.h"
#include "dicom_uids.h"
#include "dicom_sq.h"
#include "dicom_print.h"

static void dumpBasicPrinter(void *attrib, FILE * f);
static void dumpBasicFilmSession(void *attrib, FILE * f);
static void dumpBasicImageBox(void *attrib, FILE * f);
static void dumpBasicFilmBox(void *attrib, FILE * f);

static PRN_BASICFILMSESSION
    PRNbfsAttrib;
static DCM_FLAGGED_ELEMENT PRNbfsAttribC[] = {	/* optional fields */
    {DCM_BFSCOPIES, DCM_IS, "#copies", 1, sizeof(PRNbfsAttrib.copies),
	(void *) &PRNbfsAttrib.copies[0],
    PRN_BFS_K_COPIES, &PRNbfsAttrib.bfsAttributeFlag},
    {DCM_BFSPRINTPRIORITY, DCM_CS, "priority", 1,
	sizeof(PRNbfsAttrib.priority), (void *) &PRNbfsAttrib.priority[0],
    PRN_BFS_K_PRIORITY, &PRNbfsAttrib.bfsAttributeFlag},

    {DCM_BFSMEDIUMTYPE, DCM_CS, "medium type", 1,
	sizeof(PRNbfsAttrib.mediumType), (void *) &PRNbfsAttrib.mediumType[0],
    PRN_BFS_K_MEDIUMTYPE, &PRNbfsAttrib.bfsAttributeFlag},

    {DCM_BFSFILMDESTINATION, DCM_CS, "film dest", 1,
	sizeof(PRNbfsAttrib.filmDestination),
	(void *) &PRNbfsAttrib.filmDestination[0],
    PRN_BFS_K_FILMDESTINATION, &PRNbfsAttrib.bfsAttributeFlag},

    {DCM_BFSFILMSESSIONLABEL, DCM_LO, "label", 1,
	sizeof(PRNbfsAttrib.filmSessionLabel),
	(void *) &PRNbfsAttrib.filmSessionLabel[0],
    PRN_BFS_K_FILMSESSIONLABEL, &PRNbfsAttrib.bfsAttributeFlag},

    {DCM_BFSMEMORYALLOCATION, DCM_IS, "mem alloc", 1,
	sizeof(PRNbfsAttrib.memoryAllocation),
	(void *) &PRNbfsAttrib.memoryAllocation[0],
    PRN_BFS_K_MEMORYALLOCATION, &PRNbfsAttrib.bfsAttributeFlag}
};

static PRN_BASICFILMBOX PRNbfbAttrib;

static DCM_FLAGGED_ELEMENT PRNbfbAttribC[] = {
#if STANDARD_VERSION < VERSION_AUG1993
    {DCM_BFBIMAGEDISPLAYFORMAT, DCM_LT, "Image display format", 1,
	sizeof(PRNbfbAttrib.imageDisplayFormat),
	(void *) &PRNbfbAttrib.imageDisplayFormat[0],
    PRN_BFB_K_IMAGEDISPLAYFORMAT, &PRNbfbAttrib.bfbAttributeFlag},
#else
    {DCM_BFBIMAGEDISPLAYFORMAT, DCM_ST, "Image display format", 1,
	sizeof(PRNbfbAttrib.imageDisplayFormat),
	(void *) &PRNbfbAttrib.imageDisplayFormat[0],
    PRN_BFB_K_IMAGEDISPLAYFORMAT, &PRNbfbAttrib.bfbAttributeFlag},
#endif
    {DCM_BFBANNOTATIONDISPLAYFORMAT, DCM_CS, "Annotation display format", 1,
	sizeof(PRNbfbAttrib.annotationDisplayFormatID),
	(void *) &PRNbfbAttrib.annotationDisplayFormatID[0],
    PRN_BFB_K_ANNOTATIONDISPLAYFORMATID, &PRNbfbAttrib.bfbAttributeFlag},

    {DCM_BFBFILMORIENTATION, DCM_CS, "Film orientation", 1,
	sizeof(PRNbfbAttrib.filmOrientation),
	(void *) &PRNbfbAttrib.filmOrientation[0],
    PRN_BFB_K_FILMORIENTATION, &PRNbfbAttrib.bfbAttributeFlag},

    {DCM_BFBFILMSIZEID, DCM_CS, "Film size", 1,
	sizeof(PRNbfbAttrib.filmSizeID), (void *) &PRNbfbAttrib.filmSizeID[0],
    PRN_BFB_K_FILMSIZEID, &PRNbfbAttrib.bfbAttributeFlag},

    {DCM_BFBMAGNIFICATIONTYPE, DCM_CS, "Magnification type", 1,
	sizeof(PRNbfbAttrib.magnificationType),
	(void *) &PRNbfbAttrib.magnificationType[0],
    PRN_BFB_K_MAGNIFICATIONTYPE, &PRNbfbAttrib.bfbAttributeFlag},

    {DCM_BFBSMOOTHINGTYPE, DCM_CS, "Smoothing type", 1,
	sizeof(PRNbfbAttrib.smoothingType), (void *) &PRNbfbAttrib.smoothingType[0],
    PRN_BFB_K_SMOOTHINGTYPE, &PRNbfbAttrib.bfbAttributeFlag},

    {DCM_BFBBORDERDENSITY, DCM_CS, "Border density", 1,
	sizeof(PRNbfbAttrib.borderDensity), (void *) &PRNbfbAttrib.borderDensity[0],
    PRN_BFB_K_BORDERDENSITY, &PRNbfbAttrib.bfbAttributeFlag},

    {DCM_BFBEMPTYIMAGEDENSITY, DCM_CS, "Empty image density", 1,
	sizeof(PRNbfbAttrib.emptyImageDensity),
	(void *) &PRNbfbAttrib.emptyImageDensity[0],
    PRN_BFB_K_EMPTYIMAGEDENSITY, &PRNbfbAttrib.bfbAttributeFlag},

#if STANDARD_VERSION < VERSION_AUG1993
    {DCM_BFBMINDENSITY, DCM_IS, "min density", 1,
	sizeof(PRNbfbAttrib.minDensity), (void *) &PRNbfbAttrib.minDensity[0],
    PRN_BFB_K_MINDENSITY, &PRNbfbAttrib.bfbAttributeFlag},

    {DCM_BFBMAXDENSITY, DCM_IS, "Max density", 1,
	sizeof(PRNbfbAttrib.maxDensity), (void *) &PRNbfbAttrib.maxDensity[0],
    PRN_BFB_K_MAXDENSITY, &PRNbfbAttrib.bfbAttributeFlag},
#else
    {DCM_BFBMINDENSITY, DCM_US, "min density", 1,
	sizeof(PRNbfbAttrib.minDensity), (void *) &PRNbfbAttrib.minDensity,
    PRN_BFB_K_MINDENSITY, &PRNbfbAttrib.bfbAttributeFlag},

    {DCM_BFBMAXDENSITY, DCM_US, "Max density", 1,
	sizeof(PRNbfbAttrib.maxDensity), (void *) &PRNbfbAttrib.maxDensity,
    PRN_BFB_K_MAXDENSITY, &PRNbfbAttrib.bfbAttributeFlag},
#endif
    {DCM_BFBTRIM, DCM_CS, "Trim", 1, sizeof(PRNbfbAttrib.trim),
	(void *) &PRNbfbAttrib.trim[0],
    PRN_BFB_K_TRIM, &PRNbfbAttrib.bfbAttributeFlag},

    {DCM_BFBCONFIGURATIONINFO, DCM_ST, "configuration information", 1,
	sizeof(PRNbfbAttrib.configurationInfo),
	(void *) &PRNbfbAttrib.configurationInfo[0],
    PRN_BFB_K_CONFIGURATIONINFO, &PRNbfbAttrib.bfbAttributeFlag}
};

static PRN_BASICIMAGEBOX PRNbibAttrib;
static DCM_ELEMENT PRNbibAttribR[] = {
    {DCM_BIBIMAGEPOSITION, DCM_US, "Image position", 1,
	sizeof(PRNbibAttrib.imagePosition),
    (void *) &PRNbibAttrib.imagePosition}
};
static DCM_FLAGGED_ELEMENT PRNbibAttribC[] = {
    {DCM_BIBIMAGEPOSITION, DCM_US, "Image Position", 1,
	sizeof(PRNbibAttrib.imagePosition), (void *) &PRNbibAttrib.imagePosition,
    PRN_BIB_K_IMAGEPOSITION, &PRNbibAttrib.bibAttributeFlag},

    {DCM_BIBPOLARITY, DCM_CS, "polarity", 1,
	sizeof(PRNbibAttrib.polarity), (void *) &PRNbibAttrib.polarity[0],
    PRN_BIB_K_POLARITY, &PRNbibAttrib.bibAttributeFlag},

    {DCM_BFBMAGNIFICATIONTYPE, DCM_CS, "magnification type", 1,
	sizeof(PRNbibAttrib.magnificationType),
	(void *) &PRNbibAttrib.magnificationType[0],
    PRN_BIB_K_MAGNIFICATIONTYPE, &PRNbibAttrib.bibAttributeFlag},

    {DCM_BFBSMOOTHINGTYPE, DCM_CS, "Smoothing type", 1,
	sizeof(PRNbibAttrib.smoothingType), (void *) &PRNbibAttrib.smoothingType[0],
    PRN_BIB_K_SMOOTHINGTYPE, &PRNbibAttrib.bibAttributeFlag},

#if STANDARD_VERSION < VERSION_AUG1993
    {DCM_BIBMAGNIFICATIONFACTOR, DCM_DS, "BIB Magnification factor", 1,
	sizeof(PRNbibAttrib.magnificationFactor),
	(void *) &PRNbibAttrib.magnificationFactor[0],
    PRN_BIB_K_MAGNIFICATIONFACTOR, &PRNbibAttrib.bibAttributeFlag},
    {DCM_BIBIMAGESOPCLASSUID, DCM_UI, "BIB Image SOP Class UID", 1,
	sizeof(PRNbibAttrib.imageBoxSOPClassUID),
	(void *) &PRNbibAttrib.imageBoxSOPClassUID[0],
    PRN_BIB_K_IMAGEBOXSOPCLASSUID, &PRNbibAttrib.bibAttributeFlag},
#else
    {DCM_BIBREQUESTEDIMAGESIZE, DCM_DS, "requested image size", 1,
	sizeof(PRNbibAttrib.requestedImageSize),
	(void *) &PRNbibAttrib.requestedImageSize[0],
    PRN_BIB_K_REQUESTEDIMAGESIZE, &PRNbibAttrib.bibAttributeFlag}
#endif
};

static PRN_PRINTER PRNprinterAttrib;
static DCM_FLAGGED_ELEMENT PRNprinterAttribC[] = {
    {DCM_PRINTERSTATUS, DCM_CS, "Status Normal", 1,
	sizeof(PRNprinterAttrib.status), (void *) &PRNprinterAttrib.status[0],
    PRN_PRINTER_K_STATUS, &PRNprinterAttrib.attributeFlag},

    {DCM_PRINTERSTATUSINFO, DCM_CS, "Info code", 1,
	sizeof(PRNprinterAttrib.statusInfoCode),
	(void *) &PRNprinterAttrib.statusInfoCode[0],
    PRN_PRINTER_K_STATUSINFO, &PRNprinterAttrib.attributeFlag},

    {DCM_PRINTERNAME, DCM_LO, "Printer name", 1,
	sizeof(PRNprinterAttrib.name), (void *) &PRNprinterAttrib.name[0],
    PRN_PRINTER_K_NAME, &PRNprinterAttrib.attributeFlag},

    {DCM_IDMANUFACTURER, DCM_LO, "", 1,
	sizeof(PRNprinterAttrib.manufacturer),
	(void *) &PRNprinterAttrib.manufacturer[0],
    PRN_PRINTER_K_MANUFACTURER, &PRNprinterAttrib.attributeFlag},

    {DCM_IDMANUFACTURERMODEL, DCM_LO, "Manufacturer Model", 1,
	sizeof(PRNprinterAttrib.manufacturerModelName),
	(void *) &PRNprinterAttrib.manufacturerModelName[0],
    PRN_PRINTER_K_MODELNUMBER, &PRNprinterAttrib.attributeFlag},

    {DCM_ACQDEVICESERIALNUM, DCM_LO, "Device Serial #", 1,
	sizeof(PRNprinterAttrib.deviceSerialNumber),
	(void *) &PRNprinterAttrib.deviceSerialNumber[0],
    PRN_PRINTER_K_SERIALNUMBER, &PRNprinterAttrib.attributeFlag},

    {DCM_ACQSOFTWAREVERSION, DCM_LO, "Sofware version", 1,
	sizeof(PRNprinterAttrib.softwareVersion),
	(void *) &PRNprinterAttrib.softwareVersion[0],
    PRN_PRINTER_K_SOFTWAREVERSION, &PRNprinterAttrib.attributeFlag},

    {DCM_ACQDATELASTCALIBRATION, DCM_DA, "Calibration date", 1,
	sizeof(PRNprinterAttrib.dateOfLastCalibration),
	(void *) &PRNprinterAttrib.dateOfLastCalibration[0],
    PRN_PRINTER_K_DATEOFCALIBRATION, &PRNprinterAttrib.attributeFlag},

    {DCM_ACQTIMELASTCALIBRATION, DCM_TM, "Calibration time", 1,
	sizeof(PRNprinterAttrib.timeOfLastCalibration),
	(void *) &PRNprinterAttrib.timeOfLastCalibration[0],
    PRN_PRINTER_K_TIMEOFCALIBRATION, &PRNprinterAttrib.attributeFlag}
};

static PRN_BASICPRINTJOB printJob;

typedef struct {
    PRN_TYPE type;		/* One of the enumerated types */
    DCM_ELEMENT *required;	/* required elements */
    int requiredCount;		/* number of required elements */
    DCM_FLAGGED_ELEMENT *conditional;	/* optional/conditional elements */
    int conditionalCount;	/* number of optional/conditional elements */
    void *structure;		/* pointer to the structure */
    size_t structureSize;	/* size of the entire structure */
}   BASIC_FILM_TABLE;

static BASIC_FILM_TABLE filmTable[] = {
    {PRN_K_BASICFILMSESSION, NULL, 0, PRNbfsAttribC, (int) DIM_OF(PRNbfsAttribC),
    &PRNbfsAttrib, sizeof(PRNbfsAttrib)},

    {PRN_K_BASICFILMBOX, NULL, 0,
	PRNbfbAttribC, (int) DIM_OF(PRNbfbAttribC), &PRNbfbAttrib,
    sizeof(PRNbfbAttrib)},

#ifdef ASG
    {PRN_K_BASICGREYSCALEIMAGEBOX, PRNbibAttribR, (int) DIM_OF(PRNbibAttribR),
    }
#endif
    {PRN_K_BASICGREYSCALEIMAGEBOX, NULL, 0,
	PRNbibAttribC, (int) DIM_OF(PRNbibAttribC),
    &PRNbibAttrib, sizeof(PRNbibAttrib)},

    {PRN_K_BASICCOLORIMAGEBOX, PRNbibAttribR, (int) DIM_OF(PRNbibAttribR),
	PRNbibAttribC, (int) DIM_OF(PRNbibAttribC),
    &PRNbibAttrib, sizeof(PRNbibAttrib)},

    {PRN_K_BASICPRINTER, NULL, 0, PRNprinterAttribC, (int) DIM_OF(PRNprinterAttribC),
    &PRNprinterAttrib, sizeof(PRNprinterAttrib)},

    {PRN_K_BASICPRINTJOB, NULL, 0,
	NULL, 0,
    &printJob, sizeof(printJob)},
};

/* Prototypes of private functions */
static CONDITION
buildObject(PRN_GENERAL * film, BASIC_FILM_TABLE * table,
	    DCM_OBJECT ** object);
static CONDITION
parseObject(DCM_OBJECT ** object, BASIC_FILM_TABLE * table,
	    void **structure);

/* PRN_BuildObject
**
** Purpose:
**	Build a DICOM object from the PRN structure
**
** Parameter Dictionary:
**	structure	One of the various PRN structures
**	object		The DICOM object to be built from the structure.
**
** Return Values:
**
**	PRN_BUILDFAILED
**	PRN_ILLEGALUIDLENGTH
**	PRN_LISTCREATEFAILURE
**	PRN_LISTFAILURE
**	PRN_MALLOCFAILURE
**	PRN_NORMAL
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
PRN_BuildObject(void *structure, DCM_OBJECT ** object)
{
    PRN_GENERAL
	* filmStruct;
    int
        index;
    CONDITION
	cond;

    filmStruct = (PRN_GENERAL *) structure;
    for (index = 0; index < (int) DIM_OF(filmTable); index++) {
	if (filmStruct->type == filmTable[index].type) {
	    cond = buildObject(filmStruct, &filmTable[index], object);
	    if (cond != PRN_NORMAL) {
		if (*object != NULL)
		    (void) DCM_CloseObject(object);
	    }
	    return cond;
	}
    }
    *object = NULL;
    return PRN_BUILDFAILED;	/* failure */
}

/* PRN_ParseObject
**
** Purpose:
**	Extract attributes of the PRN structure from the DICOM object.
**
** Parameter Dictionary:
**	object		Handle to the DICOM object to be parsed
**	type		The type of the PRN structure expected in the object
**	structure	The PRN structure to be filled  in.
**
** Return Values:
**
**	PRN_MALLOCFAILURE
**	PRN_NORMAL
**	PRN_PARSEFAILED
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
PRN_ParseObject(DCM_OBJECT ** object, PRN_TYPE type, void **structure)
{
    int
        index;

    for (index = 0; index < (int) DIM_OF(filmTable); index++) {
	if (type == filmTable[index].type)
	    return parseObject(object, &filmTable[index], structure);
    }
    return PRN_PARSEFAILED;	/* repair */
}

CONDITION
PRN_FreeStructure(void **structure)
{
    PRN_GENERAL *p;

    p = (PRN_GENERAL *) * structure;
    switch (p->type) {
    case PRN_K_BASICPRINTER:
    case PRN_K_BASICFILMBOX:
	free(p);
	break;
    default:
	free(p);
	break;
    }
    *structure = NULL;

    return PRN_NORMAL;
}

/* Private Functions */

/* buildObject
**
** Purpose:
**	Build the object from the structure and the table entry
**
** Parameter Dictionary:
**	film		Handle to the structure
**	table		Handle to the table entry
**	object		Handle to the DICOM object to be built
**
** Return Values:
**
**	PRN_BUILDFAILED
**	PRN_ILLEGALUIDLENGTH
**	PRN_LISTCREATEFAILURE
**	PRN_LISTFAILURE
**	PRN_MALLOCFAILURE
**	PRN_NORMAL
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
buildObject(PRN_GENERAL * film, BASIC_FILM_TABLE * table,
	    DCM_OBJECT ** object)
{
    CONDITION
    cond;
    DCM_ELEMENT
	* e;
    int
        length;
    LST_HEAD
	* l;
    SQ_REFPRINT
	* refPrint;
    PRN_BASICIMAGEBOX
	* imageBox;

    cond = DCM_CreateObject(object, 0);
    if (cond != DCM_NORMAL)
	return COND_PushCondition(PRN_BUILDFAILED,
			       PRN_Message(PRN_BUILDFAILED), "buildObject");

    (void) memcpy(table->structure, film, table->structureSize);
    cond = DCM_ModifyElements(object, table->required, table->requiredCount,
			 table->conditional, table->conditionalCount, NULL);
    if (cond != DCM_NORMAL)
	return COND_PushCondition(PRN_BUILDFAILED,
			       PRN_Message(PRN_BUILDFAILED), "buildObject");

    switch (film->type) {
    case PRN_K_BASICFILMSESSION:
	break;
    case PRN_K_BASICFILMBOX:
	/*
	 * we need to add the referenced film session sequence to the dataset
	 */
	if (PRNbfbAttrib.bfbAttributeFlag & PRN_BFB_K_REFERENCEDBASICFILMSESSIONSEQ) {
	    l = LST_Create();
	    if (l == NULL)
		return COND_PushCondition(PRN_LISTCREATEFAILURE,
			 PRN_Message(PRN_LISTCREATEFAILURE), "buildObject");

	    refPrint = malloc(sizeof(*refPrint));
	    if (refPrint == NULL)
		return COND_PushCondition(PRN_MALLOCFAILURE,
			  PRN_Message(PRN_MALLOCFAILURE), sizeof(*refPrint),
					  "buildObject");

	    refPrint->type = SQ_K_REFFILMSESSION;
	    strcpy(refPrint->refSOPClassUID, DICOM_SOPCLASSBASICFILMSESSION);
	    length = strlen(PRNbfbAttrib.referencedFilmSessionSOPInstanceUID);
	    if ((length == 0) || (length > DICOM_UI_LENGTH))
		return COND_PushCondition(PRN_ILLEGALUIDLENGTH,
		  PRN_Message(PRN_ILLEGALUIDLENGTH), length, "buildObject");

	    strcpy(refPrint->refSOPInstanceUID,
		   PRNbfbAttrib.referencedFilmSessionSOPInstanceUID);

	    if (LST_Enqueue(&l, refPrint) != LST_NORMAL)
		return COND_PushCondition(PRN_LISTFAILURE,
			       PRN_Message(PRN_LISTFAILURE), "buildObject");

	    cond = SQ_BuildSequence(&l, SQ_K_REFFILMSESSION, &e);
	    if (cond != SQ_NORMAL)
		return COND_PushCondition(PRN_BUILDFAILED,
			       PRN_Message(PRN_BUILDFAILED), "buildObject");

	    cond = DCM_AddSequenceElement(object, e);
	    if (cond != DCM_NORMAL)
		return COND_PushCondition(PRN_BUILDFAILED,
			       PRN_Message(PRN_BUILDFAILED), "buildObject");
	    free(e);
	    {
		while ((refPrint = LST_Dequeue(&l)) != NULL) {
		    free(refPrint);
		}
		LST_Destroy(&l);
	    }
	}
	if (PRNbfbAttrib.bfbAttributeFlag & PRN_BFB_K_REFERENCEDBASICIMAGEBOXSEQ) {
	    cond = SQ_BuildSequence(&PRNbfbAttrib.referencedBIBSeq, SQ_K_REFBASICIMAGEBOX, &e);
	    if (cond != SQ_NORMAL)
		return COND_PushCondition(PRN_BUILDFAILED,
			       PRN_Message(PRN_BUILDFAILED), "buildObject");

	    cond = DCM_AddSequenceElement(object, e);
	    if (cond != DCM_NORMAL)
		return COND_PushCondition(PRN_BUILDFAILED,
			       PRN_Message(PRN_BUILDFAILED), "buildObject");
	    free(e);

	}
	if (PRNbfbAttrib.bfbAttributeFlag & PRN_BFB_K_REFPRINTJOBSEQ) {
	    l = LST_Create();
	    if (l == NULL)
		return COND_PushCondition(PRN_LISTCREATEFAILURE,
			 PRN_Message(PRN_LISTCREATEFAILURE), "buildObject");

	    refPrint = malloc(sizeof(*refPrint));
	    if (refPrint == NULL)
		return COND_PushCondition(PRN_MALLOCFAILURE,
			  PRN_Message(PRN_MALLOCFAILURE), sizeof(*refPrint),
					  "buildObject");

	    refPrint->type = SQ_K_REFPRINTJOB;
	    strcpy(refPrint->refSOPClassUID, DICOM_SOPCLASSPRINTJOB);
	    length = strlen(PRNbfbAttrib.refPrintJobInstanceUID);
	    if ((length == 0) || (length > DICOM_UI_LENGTH))
		return COND_PushCondition(PRN_ILLEGALUIDLENGTH,
		  PRN_Message(PRN_ILLEGALUIDLENGTH), length, "buildObject");

	    strcpy(refPrint->refSOPInstanceUID,
		   PRNbfbAttrib.refPrintJobInstanceUID);

	    if (LST_Enqueue(&l, refPrint) != LST_NORMAL)
		return COND_PushCondition(PRN_LISTFAILURE,
			       PRN_Message(PRN_LISTFAILURE), "buildObject");

	    cond = SQ_BuildSequence(&l, SQ_K_REFPRINTJOB, &e);
	    if (cond != SQ_NORMAL)
		return COND_PushCondition(PRN_BUILDFAILED,
			       PRN_Message(PRN_BUILDFAILED), "buildObject");

	    cond = DCM_AddSequenceElement(object, e);
	    if (cond != DCM_NORMAL)
		return COND_PushCondition(PRN_BUILDFAILED,
			       PRN_Message(PRN_BUILDFAILED), "buildObject");
	    free(e);
	}
#ifdef SMM
	e = malloc(sizeof(*e));
	if (e == NULL)
	    return COND_PushCondition(PRN_BUILDFAILED,
			       PRN_Message(PRN_BUILDFAILED), "buildObject");
	e->tag = DCM_BFBREFBASICFILMSESSIONSEQ;
	e->representation = DCM_SQ;
	e->multiplicity = 1;
	e->length = 0;
	e->d.sq = LST_Create();
	if (e->d.sq == NULL)
	    return COND_PushCondition(PRN_BUILDFAILED,
			       PRN_Message(PRN_BUILDFAILED), "buildObject");

	/* Allocate space for Sequence item */
	item = malloc(sizeof(DCM_SEQUENCE_ITEM));
	if (item == NULL)
	    return COND_PushCondition(PRN_BUILDFAILED,
			       PRN_Message(PRN_BUILDFAILED), "buildObject");
	cond = DCM_CreateObject(&item->object, 0);
	if (cond != DCM_NORMAL)
	    return COND_PushCondition(PRN_BUILDFAILED,
			       PRN_Message(PRN_BUILDFAILED), "buildObject");

	refElemPtr = malloc(sizeof(DCM_ELEMENT) * 2);
	refElemPtr[0] = refElem[0];
	(void) strcpy(refElemPtr[0].d.string,
		      ((PRN_BASICFILMBOX *) film)->filmBoxSOPClassUID);
	refElemPtr[1] = refElem[1];
	(void) strcpy(refElemPtr[1].d.string,
		      ((PRN_BASICFILMBOX *) film)->filmBoxSOPInstanceUID);

	/* add the 2 entries to the object */
	cond = DCM_ModifyElements(&item->object, refElemPtr, 2, NULL, 0, NULL);
	if (cond != DCM_NORMAL)
	    return COND_PushCondition(PRN_BUILDFAILED,
			       PRN_Message(PRN_BUILDFAILED), "buildObject");

	/* insert the sequence item in the sequence */
	cond = DCM_GetObjectSize(&item->object, &e->length);
	if (cond != DCM_NORMAL)
	    return COND_PushCondition(PRN_BUILDFAILED,
			       PRN_Message(PRN_BUILDFAILED), "buildObject");
	cond = LST_Enqueue(&e->d.sq, item);
	if (cond != LST_NORMAL)
	    return COND_PushCondition(PRN_BUILDFAILED,
			       PRN_Message(PRN_BUILDFAILED), "buildObject");
	/* now we add the element to the data set */
	cond = DCM_ModifyElements(object, e, 1, NULL, 0, NULL);
	if (cond != DCM_NORMAL)
	    return COND_PushCondition(PRN_BUILDFAILED,
			       PRN_Message(PRN_BUILDFAILED), "buildObject");
#endif

	/* we do exactly the similar process for referenced image boxes */
	break;
    case PRN_K_BASICGREYSCALEIMAGEBOX:
	imageBox = (PRN_BASICIMAGEBOX *) film;
	if (imageBox->bibAttributeFlag & PRN_BIB_K_IMAGEMODULE) {
	    cond = SQ_ObjectToSequence(DCM_BIBPREFORMATGREYSCALEIMAGESEQ,
				       &imageBox->object, &e);
	    if (cond != SQ_NORMAL)
		return COND_PushCondition(PRN_BUILDFAILED,
			       PRN_Message(PRN_BUILDFAILED), "buildObject");
	    cond = DCM_AddSequenceElement(object, e);
	    if (cond != DCM_NORMAL)
		return COND_PushCondition(PRN_BUILDFAILED,
			       PRN_Message(PRN_BUILDFAILED), "buildObject");
	    free(e);
	}
	break;
    case PRN_K_BASICPRINTJOB:
	break;
    }
    return PRN_NORMAL;
}


/* parseObject
**
** Purpose:
**	To parse a DCM_OBJECT and extract information about a specific
**	PRN structure and fill the structure with it.
**
** Parameter Dictionary:
**	object		Handle to the object to be parsed
**	table		Pointer to the table entry indicating the required
**			and optional attributes of the PRN structure to be
**			filled.
**	structure	The PRN structure to be filled.
**
** Return Values:
**
**	PRN_MALLOCFAILURE
**	PRN_NORMAL
**	PRN_PARSEFAILED
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
parseObject(DCM_OBJECT ** object, BASIC_FILM_TABLE * table,
	    void **structure)
{
    CONDITION
    cond;
    PRN_GENERAL
	* filmStruct;
    LST_HEAD
	* list;
    DCM_ELEMENT
	e;
    SQ_TYPE
	type;
    SQ_REFPRINT
	* refPrint;
    PRN_BASICIMAGEBOX
	* imageBox;

    if (*structure == NULL) {
	*structure = malloc(table->structureSize);
	if (*structure == NULL)
	    return COND_PushCondition(PRN_MALLOCFAILURE,
			     PRN_Message(PRN_MALLOCFAILURE), "parseObject");
    }
    (void) memset(table->structure, 0, table->structureSize);

    cond = DCM_ParseObject(object, table->required, table->requiredCount,
			 table->conditional, table->conditionalCount, NULL);
    if (cond != DCM_NORMAL)
	return COND_PushCondition(PRN_PARSEFAILED,
			       PRN_Message(PRN_PARSEFAILED), "parseObject");

    filmStruct = (PRN_GENERAL *) * structure;

    switch (table->type) {
    case PRN_K_BASICFILMSESSION:
	break;
    case PRN_K_BASICFILMBOX:
	/* here we need to parse the referenced film session seq */
	e.tag = DCM_BFBREFBASICFILMSESSIONSEQ;
	cond = DCM_GetSequenceList(object, e.tag, &e.d.sq);
	if (cond == DCM_ELEMENTNOTFOUND) {
	    (void) COND_PopCondition(FALSE);
	} else {
	    cond = SQ_ParseSequence(&e, &type, &list);
	    if (cond != SQ_NORMAL)
		return COND_PushCondition(PRN_PARSEFAILED,
			       PRN_Message(PRN_PARSEFAILED), "parseObject");

	    refPrint = LST_Head(&list);
	    if (refPrint == NULL)
		return COND_PushCondition(PRN_PARSEFAILED,
			       PRN_Message(PRN_PARSEFAILED), "parseObject");

	    strcpy(PRNbfbAttrib.referencedFilmSessionSOPClassUID,
		   refPrint->refSOPClassUID);
	    strcpy(PRNbfbAttrib.referencedFilmSessionSOPInstanceUID,
		   refPrint->refSOPInstanceUID);
	    PRNbfbAttrib.bfbAttributeFlag |=
		PRN_BFB_K_REFERENCEDBASICFILMSESSIONSEQ;
	    (void) SQ_ReleaseList(&list);
	}

	/* Parse the Referenced Basic Image Box Sequence */
	e.tag = DCM_BFBREFBASICIMAGEBOXSEQ;
	cond = DCM_GetSequenceList(object, e.tag, &e.d.sq);
	if (cond == DCM_ELEMENTNOTFOUND) {
	    (void) COND_PopCondition(FALSE);
	} else {
	    cond = SQ_ParseSequence(&e, &type, &list);
	    if (cond != SQ_NORMAL)
		return COND_PushCondition(PRN_PARSEFAILED,
			       PRN_Message(PRN_PARSEFAILED), "parseObject");

	    PRNbfbAttrib.referencedBIBSeq = list;
	    PRNbfbAttrib.bfbAttributeFlag |=
		PRN_BFB_K_REFERENCEDBASICIMAGEBOXSEQ;
	}

#if STANDARD_VERSION >= VERSION_AUG1993
	e.tag = DCM_PJREFPRINTJOBSEQ;
	cond = DCM_GetSequenceList(object, e.tag, &e.d.sq);
	if (cond == DCM_ELEMENTNOTFOUND) {
	    (void) COND_PopCondition(FALSE);
	} else {
	    cond = SQ_ParseSequence(&e, &type, &list);
	    if (cond != SQ_NORMAL)
		return COND_PushCondition(PRN_PARSEFAILED,
			       PRN_Message(PRN_PARSEFAILED), "parseObject");

	    refPrint = LST_Head(&list);
	    if (refPrint == NULL)
		return COND_PushCondition(PRN_PARSEFAILED,
			       PRN_Message(PRN_PARSEFAILED), "parseObject");

	    strcpy(PRNbfbAttrib.refPrintJobClassUID, refPrint->refSOPClassUID);
	    strcpy(PRNbfbAttrib.refPrintJobInstanceUID,
		   refPrint->refSOPInstanceUID);
	    PRNbfbAttrib.bfbAttributeFlag |=
		PRN_BFB_K_REFPRINTJOBSEQ;
	}
#endif
#ifdef SMM
	list = LST_Create();
	if (list == NULL)
	    return COND_PushCondition(PRN_PARSEFAILED,
			       PRN_Message(PRN_PARSEFAILED), "parseObject");

	cond = DCM_GetSequenceList(object, DCM_BFBREFBASICFILMSESSIONSEQ,
				   &list);
	if (cond != DCM_NORMAL)
	    return COND_PushCondition(PRN_PARSEFAILED,
			       PRN_Message(PRN_PARSEFAILED), "parseObject");
	item = LST_Head(&list);
	if (item == NULL)
	    return COND_PushCondition(PRN_PARSEFAILED,
			       PRN_Message(PRN_PARSEFAILED), "parseObject");
	/* get the elements from the object */
	cond = DCM_ParseObject(object, refElem, 2, NULL, 0, NULL);
	if (cond != DCM_NORMAL)
	    return COND_PushCondition(PRN_PARSEFAILED,
			       PRN_Message(PRN_PARSEFAILED), "parseObject");

	(void) strcpy(((PRN_BASICFILMBOX *) filmStruct)->filmBoxSOPClassUID,
		      refElem[0].d.string);
	(void) strcpy(((PRN_BASICFILMBOX *) filmStruct)->filmBoxSOPInstanceUID,
		      refElem[1].d.string);
	((PRN_BASICFILMBOX *) filmStruct)->bfbAttributeFlag |=
	    PRN_BFB_K_REFERENCEDBASICFILMSESSIONSEQ;
#endif
	break;
    case PRN_K_BASICGREYSCALEIMAGEBOX:
	imageBox = (PRN_BASICIMAGEBOX *) table->structure;
	e.tag = DCM_BIBPREFORMATGREYSCALEIMAGESEQ;
	cond = DCM_GetSequenceList(object, e.tag, &e.d.sq);
	if (cond == DCM_ELEMENTNOTFOUND) {
	    (void) COND_PopCondition(FALSE);
	} else {
	    cond = SQ_SequenceToObject(&e.d.sq, &imageBox->object);
	    if (cond != SQ_NORMAL)
		return COND_PushCondition(PRN_PARSEFAILED,
			       PRN_Message(PRN_PARSEFAILED), "parseObject");
	    PRNbibAttrib.bibAttributeFlag |= PRN_BIB_K_IMAGEMODULE;
	}
	break;
    case PRN_K_BASICPRINTJOB:
	break;
    }
    (void) memcpy(filmStruct, table->structure, table->structureSize);
    filmStruct->type = table->type;
    return PRN_NORMAL;
}

void
PRN_DumpAttributes(void *attrib, FILE * f)
{
    PRN_GENERAL
    * p;

    p = (PRN_GENERAL *) attrib;
    switch (p->type) {
    case PRN_K_BASICPRINTER:
	dumpBasicPrinter(attrib, f);
	break;
    case PRN_K_BASICFILMSESSION:
	dumpBasicFilmSession(attrib, f);
	break;
    case PRN_K_BASICGREYSCALEIMAGEBOX:
    case PRN_K_BASICCOLORIMAGEBOX:
	dumpBasicImageBox(attrib, f);
	break;

    case PRN_K_BASICFILMBOX:
	dumpBasicFilmBox(attrib, f);
	break;

    default:
	break;
    }

}

/* dumpBasicPrinter
**
** Purpose:
**	Dump the attributes of the Printer SOP class
**
** Parameter Dictionary:
**	attrib		Handle to the attributes
**	f		File pointer
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static void
dumpBasicPrinter(void *attrib, FILE * f)
{
    PRN_PRINTER
    * p;

    p = (PRN_PRINTER *) attrib;
    if (p->attributeFlag & PRN_PRINTER_K_STATUS)
	fprintf(f, "Status              %s\n", p->status);
    if (p->attributeFlag & PRN_PRINTER_K_STATUSINFO)
	fprintf(f, "Status Info         %s\n", p->statusInfoCode);
    if (p->attributeFlag & PRN_PRINTER_K_NAME)
	fprintf(f, "Name                %s\n", p->name);
    if (p->attributeFlag & PRN_PRINTER_K_MANUFACTURER)
	fprintf(f, "Manufacturer        %s\n", p->manufacturer);
    if (p->attributeFlag & PRN_PRINTER_K_MODELNUMBER)
	fprintf(f, "Model Number        %s\n", p->manufacturerModelName);
    if (p->attributeFlag & PRN_PRINTER_K_SERIALNUMBER)
	fprintf(f, "Serial Number       %s\n", p->deviceSerialNumber);
    if (p->attributeFlag & PRN_PRINTER_K_SOFTWAREVERSION)
	fprintf(f, "Software Version    %s\n", p->softwareVersion);
    if (p->attributeFlag & PRN_PRINTER_K_DATEOFCALIBRATION)
	fprintf(f, "Date of Calibration %s\n", p->dateOfLastCalibration);
    if (p->attributeFlag & PRN_PRINTER_K_TIMEOFCALIBRATION)
	fprintf(f, "Time of Calibration %s\n", p->timeOfLastCalibration);
}

/* dumpBasicFilmSession
**
** Purpose:
**	Dump the attributes of the Basic Film Session
**
** Parameter Dictionary:
**	attrib		Handle to the attributes
**	f		File pointer
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static void
dumpBasicFilmSession(void *attrib, FILE * f)
{
    PRN_BASICFILMSESSION
    * p;

    p = (PRN_BASICFILMSESSION *) attrib;

    if (p->bfsAttributeFlag & PRN_BFS_K_COPIES)
	fprintf(f, "Copies              %s\n", p->copies);
    if (p->bfsAttributeFlag & PRN_BFS_K_PRIORITY)
	fprintf(f, "Priority            %s\n", p->priority);
    if (p->bfsAttributeFlag & PRN_BFS_K_MEDIUMTYPE)
	fprintf(f, "Medium Type         %s\n", p->mediumType);
    if (p->bfsAttributeFlag & PRN_BFS_K_FILMDESTINATION)
	fprintf(f, "Film Destination    %s\n", p->filmDestination);
    if (p->bfsAttributeFlag & PRN_BFS_K_FILMSESSIONLABEL)
	fprintf(f, "Film Session Label %s\n", p->filmSessionLabel);
    if (p->bfsAttributeFlag & PRN_BFS_K_MEMORYALLOCATION)
	fprintf(f, "Memory Allocation   %s\n", p->memoryAllocation);
}

/* dumpBasicFilmBox
**
** Purpose:
**	Dump the attributes of the Basic Film Box
**
** Parameter Dictionary:
**	attrib		Handle to the attributes
**	f		File pointer
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static void
dumpBasicFilmBox(void *attrib, FILE * f)
{
    PRN_BASICFILMBOX
    * p;

    p = (PRN_BASICFILMBOX *) attrib;

    if (p->bfbAttributeFlag & PRN_BFB_K_IMAGEDISPLAYFORMAT)
	fprintf(f, "imageDisplayFormat		%s\n", p->imageDisplayFormat);

    if (p->bfbAttributeFlag & PRN_BFB_K_ANNOTATIONDISPLAYFORMATID)
	fprintf(f, "annotationDisplayFormat	%s\n", p->annotationDisplayFormatID);

    if (p->bfbAttributeFlag & PRN_BFB_K_FILMORIENTATION)
	fprintf(f, "filmOrientation		%s\n", p->filmOrientation);

    if (p->bfbAttributeFlag & PRN_BFB_K_FILMSIZEID)
	fprintf(f, "filmSizeID		%s\n", p->filmSizeID);

    if (p->bfbAttributeFlag & PRN_BFB_K_MAGNIFICATIONTYPE)
	fprintf(f, "magnificationType		%s\n", p->magnificationType);

    if (p->bfbAttributeFlag & PRN_BFB_K_BORDERDENSITY)
	fprintf(f, "borderDensity		%s\n", p->borderDensity);
    if (p->bfbAttributeFlag & PRN_BFB_K_EMPTYIMAGEDENSITY)
	fprintf(f, "emptyImageDensity		%s\n", p->emptyImageDensity);

    if (p->bfbAttributeFlag & PRN_BFB_K_MINDENSITY)
	fprintf(f, "minDensity		%d\n", p->minDensity);

    if (p->bfbAttributeFlag & PRN_BFB_K_MAXDENSITY)
	fprintf(f, "maxDensity		%d\n", p->maxDensity);


    if (p->bfbAttributeFlag & PRN_BFB_K_TRIM)
	fprintf(f, "trim		%s\n", p->trim);


    if (p->bfbAttributeFlag & PRN_BFB_K_CONFIGURATIONINFO)
	fprintf(f, "configurationInfo		%s\n", p->configurationInfo);
}

/* dumpBasicImageBox
**
** Purpose:
**	Dump the attributes of the Image Box
**
** Parameter Dictionary:
**	attrib		Handle to the attributes
**	f		File pointer
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static void
dumpBasicImageBox(void *attrib, FILE * f)
{
    PRN_BASICIMAGEBOX
    * p;

    p = (PRN_BASICIMAGEBOX *) attrib;

    fprintf(f, "Image Position      %d\n", p->imagePosition);
    (void) DCM_DumpElements(&p->object, 0);
}
