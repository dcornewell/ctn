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
+-+-+-+-+-+-+-+-+-
*/
/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	supportedAttributes
**	Private Routines
**			supportedMandatoryAttributes
**			supportedOptionalAttributes
**			supportedBasicFilmSessionValues
**			supportedBasicFilmBoxValues
**			supportedBasicGrayscaleImageBoxValues
**			applyNCreateBasicFilmSessionDefaultValues
**			applyNCreateBasicFilmBoxDefaultValues
**			applyNSetBasicGrayscaleImageBoxDefaultValues
**			applyNSetBasicColorImageBoxDefaultValues
**
** Author, Date:	Aniruddha S. Gokhale, 21-July-94
** Intent:		This module implements all the routines that verify
**			if the mandatory and optional attributes are
**			supported by the SCP and the SCU. In addition, it also
**			verifies if the  values for these attributes (requested
**			by the SCU) are supported by the SCP.
** Last Update:		$Author: smm $, $Date: 1995-12-21 17:23:26 $
** Source File:		$RCSfile: verify.c,v $
** Revision:		$Revision: 1.26 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.26 $ $RCSfile: verify.c,v $";

#include "print_server.h"
#include "private.h"

extern BFS_NODE bfsInstance;
extern BFB_NODE *bfbInstance;	/* last created Basic Film Box instance */

static CTNBOOLEAN
supportedMandatoryAttributes(PRN_TYPE, MSG_TYPE, PRN_ROLE, unsigned long,
			     char **, void **);
static CTNBOOLEAN
supportedOptionalAttributes(PRN_TYPE, MSG_TYPE, unsigned long *,
			    unsigned long, char **, void **);
static CONDITION
supportedBasicFilmSessionValues(PRN_BASICFILMSESSION *, SUPPORTEDATTRIBUTES *,
				int, char **);
static CONDITION
supportedBasicFilmBoxValues(PRN_BASICFILMBOX *, SUPPORTEDATTRIBUTES *,
			    int, char **);
static CONDITION
supportedBasicGrayscaleImageBoxValues(PRN_BASICIMAGEBOX *,
				      SUPPORTEDATTRIBUTES *, int, char **);
static void
applyNCreateBasicFilmSessionDefaultValues(PRN_BASICFILMSESSION * request,
				SUPPORTEDATTRIBUTES * supported, int count);
static void
applyNCreateBasicFilmBoxDefaultValues(PRN_BASICFILMBOX *,
				      SUPPORTEDATTRIBUTES *, int);
static void
applyNSetBasicGrayscaleImageBoxDefaultValues(PRN_BASICIMAGEBOX *,
					     SUPPORTEDATTRIBUTES *, int);
static void
applyNSetBasicColorImageBoxDefaultValues(PRN_BASICIMAGEBOX *,
					 SUPPORTEDATTRIBUTES *, int);

typedef struct {
    void *reserved[2];
    PRN_TYPE type;
    unsigned long attributeFlag;
}   REQUEST_GENERAL;

/* supportedAttributes
**
** Purpose:
** 	This function invokes a variety of internal functions that verify
**	if mandatory/optional attributes are supported and if yes, whether
**	the values sent by the requesting SCU are supported by the performing
**	SCU (SCP). In addition, for all the attributes with usage U/M, if
**	the attribute value is not sent by the SCU, the SCP applies a default.
**
** Parameter Dictionary:
** msgType	The MSG request type
** request	The PRN structure i.e. attributes of the SOP class sent by
**		requesting SCU
** supported	Pointer to a list of structures maintaing the attributes
**		supported by the performing SCU
** count	The total length of the list of supported attributes
** flagName	The list of attribute names that resulted in error
** response	Address of pointer to the MSG response message whose
**		optional fields may get updated in the event that some
**		attributes were not supported or the values were not
**		supported.
**
** Return Values:
**	APP_NORMAL if the verification process returns normal status else
**	some error CONDITION.
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
supportedAttributes(MSG_TYPE msgType, void *request,
SUPPORTEDATTRIBUTES * supported, int count, char **flagName, void **response)
{
    CONDITION
	rtnCond = APP_NORMAL,
	cond = 0;
    int
        index;
    REQUEST_GENERAL
	* general = (REQUEST_GENERAL *) request;
    unsigned long
        supportedFlag = 0;

    /* get the supported flag */
    for (index = 0; index < count; index++) {
	supportedFlag |= supported[index].flag;
    }
    /* now check if mandatory attributes are provided by the performing SCU */
    if (!supportedMandatoryAttributes(general->type, msgType, ROLE_SCP,
				      supportedFlag, flagName, response)) {
	return APP_UNSUPPORTEDMANDATORYATTRIBUTE;
    }
    /* now check if requesting SCU has provided all mandatory attributes */
    if (!supportedMandatoryAttributes(general->type, msgType, ROLE_SCU,
			      general->attributeFlag, flagName, response)) {
	return APP_MISSINGREQUIREDATTRIBUTE;
    }
    /*
     * now check if optional attributes sent by requesting SCU are supported
     * by performing SCU
     */
    if (!supportedOptionalAttributes(general->type, msgType,
	      &general->attributeFlag, supportedFlag, flagName, response)) {
	rtnCond = APP_UNSUPPORTEDOPTIONALATTRIBUTE;
    }
    /*
     * Now verify if the requested values are supported by the performing SCU
     */

    switch (general->type) {
    case PRN_K_BASICFILMSESSION:
	switch (msgType) {
	case MSG_K_N_CREATE_REQ:
	    cond = supportedBasicFilmSessionValues((PRN_BASICFILMSESSION *)
				       request, supported, count, flagName);
	    /*
	     * apply default values to all U/M attributes for which SCU has
	     * not sent any value
	     */
	    applyNCreateBasicFilmSessionDefaultValues(
			(PRN_BASICFILMSESSION *) request, supported, count);
	    break;
	case MSG_K_N_SET_REQ:
	    cond = supportedBasicFilmSessionValues((PRN_BASICFILMSESSION *)
				       request, supported, count, flagName);
	    /*
	     * we do not apply any default values for a SET request for Basic
	     * Film Session
	     */
	    break;
	case MSG_K_N_DELETE_REQ:
	case MSG_K_N_ACTION_REQ:
	default:
	    break;
	}
	break;
    case PRN_K_BASICFILMBOX:
	switch (msgType) {
	case MSG_K_N_CREATE_REQ:
	    cond = supportedBasicFilmBoxValues((PRN_BASICFILMBOX *) request,
					       supported, count, flagName);
	    /*
	     * apply default values to all U/M attributes for which SCU has
	     * not sent any value
	     */
	    applyNCreateBasicFilmBoxDefaultValues((PRN_BASICFILMBOX *) request,
						  supported, count);
	    break;
	case MSG_K_N_SET_REQ:
	    cond = supportedBasicFilmBoxValues((PRN_BASICFILMBOX *) request,
					       supported, count, flagName);
	    /*
	     * we do not apply any default values for NSet Request for Basic
	     * Film Box
	     */
	    break;
	case MSG_K_N_DELETE_REQ:
	case MSG_K_N_ACTION_REQ:
	default:
	    break;
	}
	break;
    case PRN_K_BASICGREYSCALEIMAGEBOX:
	switch (msgType) {
	case MSG_K_N_SET_REQ:
	    cond = supportedBasicGrayscaleImageBoxValues((PRN_BASICIMAGEBOX *)
				       request, supported, count, flagName);
	    applyNSetBasicGrayscaleImageBoxDefaultValues(
			   (PRN_BASICIMAGEBOX *) request, supported, count);
	    break;
	default:
	    break;
	}
	break;
    case PRN_K_BASICCOLORIMAGEBOX:
	switch (msgType) {
	case MSG_K_N_SET_REQ:
	    cond = supportedBasicGrayscaleImageBoxValues((PRN_BASICIMAGEBOX *)
				       request, supported, count, flagName);
	    applyNSetBasicColorImageBoxDefaultValues(
			   (PRN_BASICIMAGEBOX *) request, supported, count);
	    break;
	default:
	    break;
	}
	break;
    default:
	break;
    }

    if (rtnCond != APP_NORMAL)
	return rtnCond;
    else if (cond != APP_NORMAL)
	return cond;

    return APP_NORMAL;
}

/* Define tables that hold values of mandatory flags for various
   SOP classes under Print Management Meta SOP class in the roles of
   SCU and SCP
*/
static FLAGTABLE bfsSCPCreateMandatoryFlag[] = {
    {PRN_BFS_K_COPIES, DCM_BFSCOPIES, "copies"},
    {PRN_BFS_K_PRIORITY, DCM_BFSPRINTPRIORITY, "priority"},
    {PRN_BFS_K_MEDIUMTYPE, DCM_BFSMEDIUMTYPE, "medium type"},
    {PRN_BFS_K_FILMDESTINATION, DCM_BFSFILMDESTINATION, "film destination"}
};

static FLAGTABLE bfsSCPSetMandatoryFlag[] = {
    {PRN_BFS_K_COPIES, DCM_BFSCOPIES, "copies"},
    {PRN_BFS_K_PRIORITY, DCM_BFSPRINTPRIORITY, "priority"},
    {PRN_BFS_K_MEDIUMTYPE, DCM_BFSMEDIUMTYPE, "medium type"},
    {PRN_BFS_K_FILMDESTINATION, DCM_BFSFILMDESTINATION, "film destination"},
#ifdef ASG
    {PRN_BFS_K_FILMSESSIONLABEL, DCM_BFSFILMSESSIONLABEL, "film session label"},
    {PRN_BFS_K_MEMORYALLOCATION, DCM_BFSMEMORYALLOCATION, "memory allocation"}
#endif
};

static FLAGTABLE bfbSCUCreateMandatoryFlag[] = {
    {PRN_BFB_K_IMAGEDISPLAYFORMAT, DCM_BFBIMAGEDISPLAYFORMAT,
    "image display format"},
    {PRN_BFB_K_REFERENCEDBASICFILMSESSIONSEQ, DCM_BFBREFBASICFILMSESSIONSEQ,
    "referenced film session sequence"}
};

static FLAGTABLE bfbSCPCreateMandatoryFlag[] = {
    {PRN_BFB_K_IMAGEDISPLAYFORMAT, DCM_BFBIMAGEDISPLAYFORMAT,
    "image display format"},
    {PRN_BFB_K_REFERENCEDBASICFILMSESSIONSEQ,
    DCM_BFBREFBASICFILMSESSIONSEQ, "referenced film session sequence"},
    {PRN_BFB_K_REFERENCEDBASICIMAGEBOXSEQ,
    DCM_BFBREFBASICIMAGEBOXSEQ, "referenced image box sequence"},
    {PRN_BFB_K_FILMORIENTATION, DCM_BFBFILMORIENTATION, "film orientation"},
    {PRN_BFB_K_FILMSIZEID, DCM_BFBFILMSIZEID, "film size ID"},
    {PRN_BFB_K_MAGNIFICATIONTYPE, DCM_BFBMAGNIFICATIONTYPE,
    "magnification type"},
    {PRN_BFB_K_MAXDENSITY, DCM_BFBMAXDENSITY, "max density"},
    {PRN_BFB_K_CONFIGURATIONINFO, DCM_BFBCONFIGURATIONINFO,
    "configuration information"}
};

static FLAGTABLE bibSCUSetMandatoryFlag[] = {
    {PRN_BIB_K_IMAGEPOSITION, DCM_BIBIMAGEPOSITION, "image position"},
    {PRN_BIB_K_IMAGEMODULE, DCM_BIBPREFORMATGREYSCALEIMAGESEQ,
    "preformatted grayscale image sequence"}
};

static FLAGTABLE bibSCPSetMandatoryFlag[] = {
    {PRN_BIB_K_IMAGEPOSITION, DCM_BIBIMAGEPOSITION, "image position"},
    {PRN_BIB_K_IMAGEMODULE, DCM_BIBPREFORMATGREYSCALEIMAGESEQ,
    "preformatted grayscale image sequence"},
    {PRN_BIB_K_POLARITY, DCM_BIBPOLARITY, "image polarity"}
};

static FLAGTABLE bibColorSCUSetMandatoryFlag[] = {
    {PRN_BIB_K_IMAGEPOSITION, DCM_BIBIMAGEPOSITION, "image position"},
    {PRN_BIB_K_IMAGEMODULE, DCM_BIBPREFORMATCOLORIMAGESEQ,
    "preformatted color image sequence"}
};

static FLAGTABLE bibColorSCPSetMandatoryFlag[] = {
    {PRN_BIB_K_IMAGEPOSITION, DCM_BIBIMAGEPOSITION, "image position"},
    {PRN_BIB_K_IMAGEMODULE, DCM_BIBPREFORMATCOLORIMAGESEQ,
    "preformatted color image sequence"},
    {PRN_BIB_K_POLARITY, DCM_BIBPOLARITY, "image polarity"}
};

static FLAGTABLE printerGetSCPMandatoryFlag[] = {
    {PRN_PRINTER_K_STATUS, DCM_PRINTERSTATUS, "printer status"},
    {PRN_PRINTER_K_STATUSINFO, DCM_PRINTERSTATUSINFO,
    "printer status information"}
};

static PRN_FLAGMAP prnMandatoryFlagMap[] = {
    {PRN_K_BASICFILMSESSION, MSG_K_N_CREATE_REQ, ROLE_SCP,
	REQUIRED_BFS_SCP_CREATEATTRIB, bfsSCPCreateMandatoryFlag,
    (int) DIM_OF(bfsSCPCreateMandatoryFlag)},

    {PRN_K_BASICFILMSESSION, MSG_K_N_CREATE_REQ, ROLE_SCU, 0, NULL, 0},

    {PRN_K_BASICFILMSESSION, MSG_K_N_SET_REQ, ROLE_SCP,
	REQUIRED_BFS_SCP_SETATTRIB, bfsSCPSetMandatoryFlag,
    (int) DIM_OF(bfsSCPSetMandatoryFlag)},

    {PRN_K_BASICFILMSESSION, MSG_K_N_SET_REQ, ROLE_SCU, 0, NULL, 0},

    {PRN_K_BASICFILMBOX, MSG_K_N_CREATE_REQ, ROLE_SCP,
	REQUIRED_BFB_SCP_CREATEATTRIB, bfbSCPCreateMandatoryFlag,
    (int) DIM_OF(bfbSCPCreateMandatoryFlag)},

    {PRN_K_BASICFILMBOX, MSG_K_N_CREATE_REQ, ROLE_SCU,
	REQUIRED_BFB_SCU_CREATEATTRIB, bfbSCUCreateMandatoryFlag,
    (int) DIM_OF(bfbSCUCreateMandatoryFlag)},

    {PRN_K_BASICGREYSCALEIMAGEBOX, MSG_K_N_SET_REQ, ROLE_SCU,
	REQUIRED_BIB_SCU_NSETATTRIB, bibSCUSetMandatoryFlag,
    (int) DIM_OF(bibSCUSetMandatoryFlag)},

    {PRN_K_BASICGREYSCALEIMAGEBOX, MSG_K_N_SET_REQ, ROLE_SCP,
	REQUIRED_BIB_SCP_NSETATTRIB, bibSCPSetMandatoryFlag,
    (int) DIM_OF(bibSCPSetMandatoryFlag)},

    {PRN_K_BASICCOLORIMAGEBOX, MSG_K_N_SET_REQ, ROLE_SCU,
	REQUIRED_BIB_SCU_NSETATTRIB, bibColorSCUSetMandatoryFlag,
    (int) DIM_OF(bibColorSCUSetMandatoryFlag)},

    {PRN_K_BASICCOLORIMAGEBOX, MSG_K_N_SET_REQ, ROLE_SCP,
	REQUIRED_BIB_SCP_NSETATTRIB, bibColorSCPSetMandatoryFlag,
    (int) DIM_OF(bibColorSCPSetMandatoryFlag)},

    {PRN_K_BASICPRINTER, MSG_K_N_GET_REQ, ROLE_SCU, 0, NULL, 0},

    {PRN_K_BASICPRINTER, MSG_K_N_GET_REQ, ROLE_SCP,
	REQUIRED_PRINTER_NGETATTRIB, printerGetSCPMandatoryFlag,
    (int) DIM_OF(printerGetSCPMandatoryFlag)}
};

/* supportedMandatoryAttributes
**
** Purpose:
**	Verifies if the mandatory attributes are provided for the given
**	PRN_TYPE, MSG_TYPE and the PRN_ROLE taken to-gether.
**
** Parameter Dictionary:
**	prnType		The SOP Class for which service is requested
**	msgType		The service requested
**	role		Whether SCU or SCP
**	supportedFlag	Attributes actually supported
**	flagName	List of attribute names that are unsupported
**	response	Address of pointer to response message that shall
**			be updated if any attribute is unsupported
**
** Return Values:
**	TRUE		on success
**	FALSE		if one of the attributes is not supported
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CTNBOOLEAN
supportedMandatoryAttributes(PRN_TYPE prnType, MSG_TYPE msgType, PRN_ROLE role,
	      unsigned long supportedFlag, char **flagName, void **response)
{
    int
        index;
    unsigned long
        result;
    PRN_FLAGMAP
	* map = 0;
    FLAGTABLE
	* table = 0;

    *flagName = 0;
    for (index = 0; index < (int) DIM_OF(prnMandatoryFlagMap); index++) {
	map = &prnMandatoryFlagMap[index];
	if ((map->prnType == prnType) && (map->msgType == msgType) &&
	    (map->prnRole == role))
	    break;
    }

    if (!map)
	return FALSE;
    /*
     * mask the supported flag with the mandatory flag. If the result is not
     * the same as the mandatory flag, some attributes were not supported
     */
    result = supportedFlag & map->flag;
    if (result != map->flag) {
	/*
	 * some attributes were not supported. Exclusive-OR the result of the
	 * previous masking step with the mandatory flag. This will remove
	 * all mandatory "bits" which were supported. Thus, the resulting
	 * value will have bits that stand for the attributes not supported
	 * by the application
	 */
	result = result ^ map->flag;

	/*
	 * now go thru the table of mandatory attribute flags and find out
	 * which ones were not supported
	 */
	table = map->flagTable;
	for (index = 0; index < map->flagCount; index++) {
	    if (result & table[index].flag) {
		updateFlagName(flagName, table[index].flagName, 0);
		updateAttributeIdentifierList(table[index].tag, response);
	    }
	}
	return FALSE;
    } else
	return TRUE;
}

/* Define tables for optional attributes of various SOP classes from the
point of view of the SCP */

static FLAGTABLE bfsOptionalFlagTable[] = {
    {PRN_BFS_K_FILMSESSIONLABEL, DCM_BFSFILMSESSIONLABEL, "Film Session Label"},
    {PRN_BFS_K_MEMORYALLOCATION, DCM_BFSMEMORYALLOCATION, "Memory Allocation"},
};

static FLAGTABLE bfbNCreateOptionalFlagTable[] = {
    {PRN_BFB_K_ANNOTATIONDISPLAYFORMATID, DCM_BFBANNOTATIONDISPLAYFORMAT,
    "Annotation Display Format ID"},
    {PRN_BFB_K_SMOOTHINGTYPE, DCM_BFBSMOOTHINGTYPE, "Smoothing Type"},
    {PRN_BFB_K_BORDERDENSITY, DCM_BFBBORDERDENSITY, "Border Density"},
    {PRN_BFB_K_EMPTYIMAGEDENSITY, DCM_BFBEMPTYIMAGEDENSITY,
    "Empty Image Density"},
    {PRN_BFB_K_MINDENSITY, DCM_BFBMINDENSITY, "Min Density"},
    {PRN_BFB_K_TRIM, DCM_BFBTRIM, "Trim"}
};

static FLAGTABLE bfbNSetOptionalFlagTable[] = {
    {PRN_BFB_K_SMOOTHINGTYPE, DCM_BFBSMOOTHINGTYPE, "Smoothing Type"},
    {PRN_BFB_K_BORDERDENSITY, DCM_BFBBORDERDENSITY, "Border Density"},
    {PRN_BFB_K_EMPTYIMAGEDENSITY, DCM_BFBEMPTYIMAGEDENSITY,
    "Empty Image Density"},
    {PRN_BFB_K_MINDENSITY, DCM_BFBMINDENSITY, "Min Density"},
    {PRN_BFB_K_TRIM, DCM_BFBTRIM, "Trim"}
};

static FLAGTABLE bibOptionalFlagTable[] = {
    {PRN_BIB_K_MAGNIFICATIONTYPE, DCM_BFBMAGNIFICATIONTYPE,
    "Magnification Type"},
    {PRN_BIB_K_SMOOTHINGTYPE, DCM_BFBSMOOTHINGTYPE, "Smoothing Type"},
    {PRN_BIB_K_REQUESTEDIMAGESIZE, DCM_BIBREQUESTEDIMAGESIZE,
    "Requested Image Size"}
};

static FLAGTABLE printerOptionalFlagTable[] = {
    {PRN_PRINTER_K_NAME, DCM_PRINTERNAME, "Printer name"},
    {PRN_PRINTER_K_MANUFACTURER, DCM_IDMANUFACTURER, "Manufacturer"},
    {PRN_PRINTER_K_MODELNUMBER, DCM_IDMANUFACTURERMODEL, "Model number"},
    {PRN_PRINTER_K_SERIALNUMBER, DCM_ACQDEVICESERIALNUM, "Device serial number"},
    {PRN_PRINTER_K_SOFTWAREVERSION, DCM_ACQSOFTWAREVERSION, "Software Version"},
    {PRN_PRINTER_K_DATEOFCALIBRATION, DCM_ACQDATELASTCALIBRATION,
    "Date of last Calibration"},
    {PRN_PRINTER_K_TIMEOFCALIBRATION, DCM_ACQTIMELASTCALIBRATION,
    "Time of last Calibration"}
};

static PRN_FLAGMAP prnOptionalFlag[] = {
    {PRN_K_BASICFILMSESSION, MSG_K_N_CREATE_REQ, ROLE_SCP,
	(PRN_BFS_K_FILMSESSIONLABEL | PRN_BFS_K_MEMORYALLOCATION),
    bfsOptionalFlagTable, (int) DIM_OF(bfsOptionalFlagTable)},
    {PRN_K_BASICFILMSESSION, MSG_K_N_SET_REQ, ROLE_SCP,
	(PRN_BFS_K_FILMSESSIONLABEL | PRN_BFS_K_MEMORYALLOCATION),
    bfsOptionalFlagTable, (int) DIM_OF(bfsOptionalFlagTable)},
    {PRN_K_BASICFILMBOX, MSG_K_N_CREATE_REQ, ROLE_SCP,
	(PRN_BFB_K_ANNOTATIONDISPLAYFORMATID | PRN_BFB_K_SMOOTHINGTYPE |
	 PRN_BFB_K_BORDERDENSITY | PRN_BFB_K_EMPTYIMAGEDENSITY |
	 PRN_BFB_K_MINDENSITY | PRN_BFB_K_TRIM),
    bfbNCreateOptionalFlagTable, (int) DIM_OF(bfbNCreateOptionalFlagTable)},
    {PRN_K_BASICFILMBOX, MSG_K_N_SET_REQ, ROLE_SCP,
	(PRN_BFB_K_SMOOTHINGTYPE |
	 PRN_BFB_K_BORDERDENSITY | PRN_BFB_K_EMPTYIMAGEDENSITY |
	 PRN_BFB_K_MINDENSITY | PRN_BFB_K_TRIM),
    bfbNSetOptionalFlagTable, (int) DIM_OF(bfbNSetOptionalFlagTable)},
    {PRN_K_BASICGREYSCALEIMAGEBOX, MSG_K_N_SET_REQ, ROLE_SCP,
	(PRN_BIB_K_MAGNIFICATIONTYPE | PRN_BIB_K_SMOOTHINGTYPE |
	 PRN_BIB_K_REQUESTEDIMAGESIZE),
    bibOptionalFlagTable, (int) DIM_OF(bibOptionalFlagTable)},
    {PRN_K_BASICCOLORIMAGEBOX, MSG_K_N_SET_REQ, ROLE_SCP,
	(PRN_BIB_K_MAGNIFICATIONTYPE | PRN_BIB_K_SMOOTHINGTYPE |
	 PRN_BIB_K_REQUESTEDIMAGESIZE),
    bibOptionalFlagTable, (int) DIM_OF(bibOptionalFlagTable)},
    {PRN_K_BASICPRINTER, MSG_K_N_GET_REQ, ROLE_SCP,
	(PRN_PRINTER_K_NAME | PRN_PRINTER_K_MANUFACTURER |
	 PRN_PRINTER_K_MODELNUMBER | PRN_PRINTER_K_SERIALNUMBER |
	 PRN_PRINTER_K_SOFTWAREVERSION | PRN_PRINTER_K_DATEOFCALIBRATION |
	 PRN_PRINTER_K_TIMEOFCALIBRATION),
    printerOptionalFlagTable, (int) DIM_OF(printerOptionalFlagTable)}
};
/* supportedOptionalAttributes
**
** Purpose:
**	This function verifies if the optional attributes sent by the
**	requesting SCU are supported by the performing SCU
**
** Parameter Dictionary:
**	prnType		The SOP Class for which service is requested
**	msgType		The service requested
**	reqFlag		Attribute flags corresponding to attributes present
**			in the requesting message
**	supportedFlag	Attributes actually supported by SCP
**	flagName	List of attribute names that are unsupported
**	response	Address of pointer to response message that shall
**			be updated if any attribute is unsupported
**
** Return Values:
**	TRUE		on success
**	FALSE		if one of the attributes is not supported
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CTNBOOLEAN
supportedOptionalAttributes(PRN_TYPE prnType, MSG_TYPE msgType,
			unsigned long *reqFlag, unsigned long supportedFlag,
			    char **flagName, void **response)
{
    int
        index;
    unsigned long
        requestedFlag = *reqFlag,
        residueRequestedFlag,
        residueSupportedFlag,
        result;
    PRN_FLAGMAP
	* map = 0;
    FLAGTABLE
	* table = 0;

    *flagName = 0;
    for (index = 0; index < (int) DIM_OF(prnOptionalFlag); index++) {
	map = &prnOptionalFlag[index];
	if ((map->prnType == prnType) && (map->msgType == msgType))
	    break;
    }
    if (!map)
	return FALSE;

    /*
     * filter out mandatory attributes of the SCP from both the requested and
     * supported flags. The residue is the set of optional flags
     */
    residueRequestedFlag = requestedFlag & map->flag;
    residueSupportedFlag = supportedFlag & map->flag;
    /*
     * for the function to return true, the residues obtained in the above
     * steps must be equal or requested residue must be subset of supported
     * residue
     */
    result = residueRequestedFlag & residueSupportedFlag;
    if (result != residueRequestedFlag) {
	/*
	 * find which were not supported by filtering out those which were
	 * supported
	 */
	result = result ^ residueRequestedFlag;
	/*
	 * now go thru the table of optional attribute flags and find out
	 * which ones were not supported
	 */
	table = map->flagTable;
	for (index = 0; index < map->flagCount; index++) {
	    if (result & table[index].flag) {
		requestedFlag ^= table[index].flag;
		updateFlagName(flagName, table[index].flagName, 0);
		updateAttributeIdentifierList(table[index].tag, response);
	    }
	}
	*reqFlag = requestedFlag;
	return FALSE;
    } else
	return TRUE;
}



typedef struct {
    unsigned long flag;
}   APP_FLAGLIST;

static APP_FLAGLIST bfsFlagList[] = {
    PRN_BFS_K_COPIES,
    PRN_BFS_K_PRIORITY,
    PRN_BFS_K_MEDIUMTYPE,
    PRN_BFS_K_FILMDESTINATION,
    PRN_BFS_K_FILMSESSIONLABEL,
    PRN_BFS_K_MEMORYALLOCATION
};

/* supportedBasicFilmSessionValues
**
** Purpose:
**	This routine verifies if the attribute values sent by the SCU are
**	supported by the SCP or not for the N-CREATE request and the
**	Basic Film Session SOP class.
**
** Parameter Dictionary:
** 	request		Pointer to the request sent by the SCU.
**	list		List of supported attributes and their values.
**	count		The total number of attributes supported by SCP
**	flagName	The string that maintains names of attributes that
**			are unsupported.
**
** Return Values:
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
supportedBasicFilmSessionValues(PRN_BASICFILMSESSION * request,
		     SUPPORTEDATTRIBUTES * list, int count, char **flagName)
{
    int
        index,
        i,
        j;
    CONDITION
	cond;
    CTNBOOLEAN
	existsFlag = FALSE;
    BFS_SUPPORTED_COPIES
	* copies;
    BFS_SUPPORTED_PRIORITY
	* priority;
    BFS_SUPPORTED_MEDIUMTYPE
	* mediumType;
    BFS_SUPPORTED_FILMDESTINATION
	* filmDestination;
    BFS_SUPPORTED_FILMSESSIONLABEL
	* filmSessionLabel;
    BFS_SUPPORTED_MEMORYALLOCATION
	* memoryAllocation;

    /*
     * Go thru the list of defined attributes for this SOP class. If such an
     * attribute was requested by the SCU, proceed further to verify that the
     * value requested is supported by the performing SCU
     */
    for (index = 0; index < (int) DIM_OF(bfsFlagList); index++) {
	if (!(bfsFlagList[index].flag & request->bfsAttributeFlag)) {
	    /*
	     * that attribute was not requested by the SCU. Proceed to the
	     * next flag in the list
	     */
	    continue;
	}
	/*
	 * bfsFlagList[index].flag gives the flag value for which the
	 * attribute is requested. Go thru the list of values which we
	 * support for the requested attribute and verify if the requested
	 * value is supported
	 */
	for (i = 0; i < count; i++) {
	    if (bfsFlagList[index].flag == list[i].flag) {
		/* flag is supported */
		switch (bfsFlagList[index].flag) {
		case PRN_BFS_K_COPIES:
		    copies = (BFS_SUPPORTED_COPIES *) list[i].valueList;
		    existsFlag = FALSE;
		    for (j = 0; j < list[i].listCount; j++) {
			if (!strcmp(request->copies, copies[j].copies)) {
			    existsFlag = TRUE;
			    break;	/* value is supported, exit the loop */
			}
		    }
		    if (!existsFlag) {
			/* Value not supported */
			updateFlagName(flagName, list[i].listName, ": ",
				       request->copies, 0);
			/* we use the first value as default */
			strcpy(request->copies, copies[0].copies);
		    }
		    break;
		case PRN_BFS_K_PRIORITY:
		    priority = (BFS_SUPPORTED_PRIORITY *) list[i].valueList;
		    existsFlag = FALSE;
		    for (j = 0; j < list[i].listCount; j++) {
			if (!strcmp(request->priority, priority[j].priority)) {
			    existsFlag = TRUE;
			    break;	/* value is supported, exit the loop */
			}
		    }
		    if (!existsFlag) {
			/* Value not supported */
			updateFlagName(flagName, list[i].listName, ": ",
				       request->priority, 0);
			/* use the default value */
			strcpy(request->priority, priority[0].priority);
		    }
		    break;
		case PRN_BFS_K_MEDIUMTYPE:
		    mediumType = (BFS_SUPPORTED_MEDIUMTYPE *) list[i].valueList;
		    existsFlag = FALSE;
		    for (j = 0; j < list[i].listCount; j++) {
			if (!strcmp(request->mediumType,
				    mediumType[j].mediumType)) {
			    existsFlag = TRUE;
			    break;	/* value is supported, exit the loop */
			}
		    }
		    if (!existsFlag) {
			/* Value not supported */
			updateFlagName(flagName, list[i].listName, ": ",
				       request->mediumType, 0);
			strcpy(request->mediumType, mediumType[0].mediumType);
		    }
		    break;
		case PRN_BFS_K_FILMDESTINATION:
		    filmDestination = (BFS_SUPPORTED_FILMDESTINATION *)
			list[i].valueList;
		    existsFlag = FALSE;
		    for (j = 0; j < list[i].listCount; j++) {
			if (!strcmp(request->filmDestination,
				    filmDestination[j].filmDestination)) {
			    existsFlag = TRUE;
			    break;	/* value is supported, exit the loop */
			}
		    }
		    if (!existsFlag) {
			/* Value not supported */
			updateFlagName(flagName, list[i].listName, ": ",
				       request->filmDestination, 0);
			strcpy(request->filmDestination,
			       filmDestination[0].filmDestination);
		    }
		    break;
		case PRN_BFS_K_FILMSESSIONLABEL:
		    filmSessionLabel = (BFS_SUPPORTED_FILMSESSIONLABEL *)
			list[i].valueList;
		    existsFlag = FALSE;
		    for (j = 0; j < list[i].listCount; j++) {
			if (!strcmp(request->filmSessionLabel,
				    filmSessionLabel[j].filmSessionLabel)) {
			    existsFlag = TRUE;
			    break;	/* value is supported, exit the loop */
			}
		    }
		    if (!existsFlag) {
			/* Value not supported */
			updateFlagName(flagName, list[i].listName, ": ",
				       request->filmSessionLabel, 0);
			strcpy(request->filmSessionLabel,
			       filmSessionLabel[0].filmSessionLabel);
		    }
		    break;
		case PRN_BFS_K_MEMORYALLOCATION:
		    memoryAllocation = (BFS_SUPPORTED_MEMORYALLOCATION *)
			list[i].valueList;
		    existsFlag = FALSE;
		    for (j = 0; j < list[i].listCount; j++) {
			if (!strcmp(request->memoryAllocation,
				    memoryAllocation[j].memoryAllocation)) {
			    existsFlag = TRUE;
			    break;	/* value is supported, exit the loop */
			}
		    }
		    if (!existsFlag) {
			/* Value not supported */
			updateFlagName(flagName, list[i].listName, ": ",
				       request->memoryAllocation, 0);
			strcpy(request->memoryAllocation,
			       memoryAllocation[0].memoryAllocation);
		    }
		    break;
		default:
		    break;
		}
	    }
	}
    }
    if (*flagName)		/* implies some values were not supported */
	return APP_UNSUPPORTEDATTRIBUTEVALUE;

    return APP_NORMAL;
}

static APP_FLAGLIST bfbFlagList[] = {
    PRN_BFB_K_IMAGEDISPLAYFORMAT,
    PRN_BFB_K_ANNOTATIONDISPLAYFORMATID,
    PRN_BFB_K_FILMORIENTATION,
    PRN_BFB_K_FILMSIZEID,
    PRN_BFB_K_MAGNIFICATIONTYPE,
    PRN_BFB_K_SMOOTHINGTYPE,
    PRN_BFB_K_BORDERDENSITY,
    PRN_BFB_K_EMPTYIMAGEDENSITY,
    PRN_BFB_K_MAXDENSITY,
    PRN_BFB_K_MINDENSITY,
    PRN_BFB_K_TRIM,
    PRN_BFB_K_CONFIGURATIONINFO,
    PRN_BFB_K_REFERENCEDBASICFILMSESSIONSEQ,
    PRN_BFB_K_REFERENCEDBASICIMAGEBOXSEQ,
    PRN_BFB_K_REFERENCEDBASICANNOTATIONBOXSEQ,
    PRN_BFB_K_REFPRINTJOBSEQ
};

static CONDITION
supportedBasicFilmBoxValues(PRN_BASICFILMBOX * request,
		     SUPPORTEDATTRIBUTES * list, int count, char **flagName)
{
    int
        i,
        j,
        index;
    CONDITION
	cond;
    CTNBOOLEAN
	existsFlag = FALSE;
    APP_BFBIMAGEDISPLAYFORMAT
	* displayFormat;
    BFB_SUPPORTED_ANNOTATIONDISPLAYFORMATID
	* annotationDisplayFormatID;
    BFB_SUPPORTED_FILMORIENTATION
	* filmOrientation;
    BFB_SUPPORTED_FILMSIZEID
	* filmSizeID;
    BFB_SUPPORTED_MAGNIFICATIONTYPE
	* magnificationType;
    BFB_SUPPORTED_SMOOTHINGTYPE
	* smoothingType;
    BFB_SUPPORTED_BORDERDENSITY
	* borderDensity;
    BFB_SUPPORTED_EMPTYIMAGEDENSITY
	* emptyImageDensity;
#ifdef ASG
    BFB_SUPPORTED_MINDENSITY
	* minDensity;
    BFB_SUPPORTED_MAXDENSITY
	* maxDensity;
#endif
    BFB_SUPPORTED_TRIM
	* trim;
    BFB_SUPPORTED_CONFIGURATIONINFO
	* configurationInfo;
    BFB_SUPPORTED_REFFILMSESSIONSEQUENCE
	* refFilmSessionSequence;

    /*
     * Go thru the list of defined attributes for this SOP class. If such an
     * attribute was requested by the SCU, proceed further to verify that the
     * value requested is supported by the performing SCU
     */
    for (index = 0; index < (int) DIM_OF(bfbFlagList); index++) {
	if (!(bfbFlagList[index].flag & request->bfbAttributeFlag)) {
	    /*
	     * that attribute was not requested by the SCU. Proceed to the
	     * next flag in the list
	     */
	    continue;
	}
	/*
	 * bfbFlagList[index].flag gives the flag value for which the
	 * attribute is requested. Go thru the list of values which we
	 * support for the requested attribute and verify if the requested
	 * value is supported
	 */
	for (i = 0; i < count; i++) {
	    if (bfbFlagList[index].flag == list[i].flag) {
		/* flag is supported */
		switch (bfbFlagList[index].flag) {
		case PRN_BFB_K_IMAGEDISPLAYFORMAT:
		    existsFlag = FALSE;
		    displayFormat = (APP_BFBIMAGEDISPLAYFORMAT *)
			list[i].valueList;
		    /*
		     * Here we achieve a side-effect. Along with verifying
		     * the attribute value, we also parse it and store the
		     * information in user supplied structure which can be
		     * used later by the user
		     */
		    existsFlag = decodeImageDisplayFormat(
				request->imageDisplayFormat, displayFormat);
		    if (!existsFlag) {
			/* value not supported */
			updateFlagName(flagName, list[i].listName, ": ",
				       request->imageDisplayFormat, 0);
			strcpy(request->imageDisplayFormat, "STANDARD\\2,2");
		    }
		    break;
		case PRN_BFB_K_ANNOTATIONDISPLAYFORMATID:
		    annotationDisplayFormatID =
			(BFB_SUPPORTED_ANNOTATIONDISPLAYFORMATID *)
			list[i].valueList;
		    existsFlag = FALSE;
		    for (j = 0; j < list[i].listCount; j++) {
			if (!strcmp(request->annotationDisplayFormatID,
				    annotationDisplayFormatID[j].
				    annotationDisplayFormatID)) {
			    existsFlag = TRUE;
			    break;
			}
		    }
		    if (!existsFlag) {
			/* value not supported */
			updateFlagName(flagName, list[i].listName, ": ",
				     request->annotationDisplayFormatID, 0);
			strcpy(request->annotationDisplayFormatID,
			       annotationDisplayFormatID[0].
			       annotationDisplayFormatID);
		    }
		    break;
		case PRN_BFB_K_FILMORIENTATION:
		    filmOrientation = (BFB_SUPPORTED_FILMORIENTATION *)
			list[i].valueList;
		    existsFlag = FALSE;
		    for (j = 0; j < list[i].listCount; j++) {
			if (!strcmp(request->filmOrientation,
				    filmOrientation[j].filmOrientation)) {
			    existsFlag = TRUE;
			    break;
			}
		    }
		    if (!existsFlag) {
			/* value not supported */
			updateFlagName(flagName, list[i].listName, ": ",
				       request->filmOrientation, 0);
			strcpy(request->filmOrientation,
			       filmOrientation[0].filmOrientation);
		    }
		    break;
		case PRN_BFB_K_FILMSIZEID:
		    filmSizeID = (BFB_SUPPORTED_FILMSIZEID *) list[i].valueList;
		    existsFlag = FALSE;
		    for (j = 0; j < list[i].listCount; j++) {
			if (!strcmp(request->filmSizeID,
				    filmSizeID[j].filmSizeID)) {
			    existsFlag = TRUE;
			    break;
			}
		    }
		    if (!existsFlag) {
			/* value not supported */
			updateFlagName(flagName, list[i].listName, ": ",
				       request->filmSizeID, 0);
			strcpy(request->filmSizeID, filmSizeID[0].filmSizeID);
		    }
		    break;
		case PRN_BFB_K_MAGNIFICATIONTYPE:
		    magnificationType = (BFB_SUPPORTED_MAGNIFICATIONTYPE *)
			list[i].valueList;
		    existsFlag = FALSE;
		    for (j = 0; j < list[i].listCount; j++) {
			if (!strcmp(request->magnificationType,
				  magnificationType[j].magnificationType)) {
			    existsFlag = TRUE;
			    break;
			}
		    }
		    if (!existsFlag) {
			/* value not supported */
			updateFlagName(flagName, list[i].listName, ": ",
				       request->magnificationType, 0);
			strcpy(request->magnificationType,
			       magnificationType[0].magnificationType);
		    }
		    break;
		case PRN_BFB_K_SMOOTHINGTYPE:
		    smoothingType = (BFB_SUPPORTED_SMOOTHINGTYPE *)
			list[i].valueList;
		    existsFlag = FALSE;
		    for (j = 0; j < list[i].listCount; j++) {
			if (!strcmp(request->smoothingType,
				    smoothingType[j].smoothingType)) {
			    existsFlag = TRUE;
			    break;
			}
		    }
		    if (!existsFlag) {
			/* value not supported */
			updateFlagName(flagName, list[i].listName, ": ",
				       request->smoothingType, 0);
			strcpy(request->smoothingType,
			       smoothingType[0].smoothingType);
		    }
		    break;
		case PRN_BFB_K_BORDERDENSITY:
		    borderDensity = (BFB_SUPPORTED_BORDERDENSITY *)
			list[i].valueList;
		    existsFlag = FALSE;
		    for (j = 0; j < list[i].listCount; j++) {
			if (!strcmp(request->borderDensity,
				    borderDensity[j].borderDensity)) {
			    existsFlag = TRUE;
			    break;
			}
		    }
		    if (!existsFlag) {
			/* value not supported */
			updateFlagName(flagName, list[i].listName, ": ",
				       request->borderDensity, 0);
			strcpy(request->borderDensity,
			       borderDensity[0].borderDensity);
		    }
		    break;
		case PRN_BFB_K_EMPTYIMAGEDENSITY:
		    emptyImageDensity = (BFB_SUPPORTED_EMPTYIMAGEDENSITY *)
			list[i].valueList;
		    existsFlag = FALSE;
		    for (j = 0; j < list[i].listCount; j++) {
			if (!strcmp(request->emptyImageDensity,
				  emptyImageDensity[j].emptyImageDensity)) {
			    existsFlag = TRUE;
			    break;
			}
		    }
		    if (!existsFlag) {
			/* value not supported */
			updateFlagName(flagName, list[i].listName, ": ",
				       request->emptyImageDensity, 0);
			strcpy(request->emptyImageDensity,
			       emptyImageDensity[0].emptyImageDensity);
		    }
		    break;
		case PRN_BFB_K_MINDENSITY:
		    existsFlag = FALSE;
		    /*
		     * we allow minimum density to be in the range 10-500,
		     * but we also check if it does not exceed the Max
		     * density
		     */
		    if ((request->minDensity >= 10) &&
			(request->minDensity <= 500)) {
			/* now verify that it is less than max density attrib */
			if (request->minDensity < request->maxDensity)
			    existsFlag = TRUE;
		    }
		    if (!existsFlag) {
			char temp[32];
			sprintf(temp, "%d", request->minDensity);
			/* value not supported */
			updateFlagName(flagName, list[i].listName, ": ",
				       temp, 0);
			request->minDensity = 10;
		    }
		    break;
		case PRN_BFB_K_MAXDENSITY:
		    existsFlag = FALSE;
		    /* we allow MAX DENSITY to be in the range 100-500 */
		    if ((request->maxDensity >= 100) &&
			(request->maxDensity <= 500)) {
			existsFlag = TRUE;
		    }
		    if (!existsFlag) {
			char temp[32];
			sprintf(temp, "%d", request->maxDensity);
			/* value not supported */
			updateFlagName(flagName, list[i].listName, ": ",
				       temp, 0);
			request->maxDensity = 500;
		    }
		    break;
		case PRN_BFB_K_TRIM:
		    trim = (BFB_SUPPORTED_TRIM *) list[i].valueList;
		    existsFlag = FALSE;
		    for (j = 0; j < list[i].listCount; j++) {
			if (!strcmp(request->trim, trim[j].trim)) {
			    existsFlag = TRUE;
			    break;
			}
		    }
		    if (!existsFlag) {
			/* value not supported */
			updateFlagName(flagName, list[i].listName, ": ",
				       request->trim, 0);
			strcpy(request->trim, trim[0].trim);
		    }
		    break;
		case PRN_BFB_K_CONFIGURATIONINFO:
		    configurationInfo = (BFB_SUPPORTED_CONFIGURATIONINFO *)
			list[i].valueList;
		    existsFlag = FALSE;
		    for (j = 0; j < list[i].listCount; j++) {
			if (!strcmp(request->configurationInfo,
				  configurationInfo[j].configurationInfo)) {
			    existsFlag = TRUE;
			    break;
			}
		    }
		    if (!existsFlag) {
			/* value not supported */
			updateFlagName(flagName, list[i].listName, ": ",
				       request->configurationInfo, 0);
			strcpy(request->configurationInfo,
			       configurationInfo[0].configurationInfo);
		    }
		    break;
		case PRN_BFB_K_REFERENCEDBASICFILMSESSIONSEQ:
		    refFilmSessionSequence =
			(BFB_SUPPORTED_REFFILMSESSIONSEQUENCE *)
			list[i].valueList;
		    existsFlag = FALSE;
		    for (j = 0; j < list[i].listCount; j++) {
			if (!strcmp(request->referencedFilmSessionSOPClassUID,
				    refFilmSessionSequence[j].
				    referencedFilmSessionSOPClassUID)) {
			    existsFlag = TRUE;
			    break;
			}
		    }
		    if (!existsFlag) {
			/* value not supported */
			updateFlagName(flagName, list[i].listName, ": ",
			      request->referencedFilmSessionSOPClassUID, 0);
		    }
		    break;
		case PRN_BFB_K_REFERENCEDBASICIMAGEBOXSEQ:
		case PRN_BFB_K_REFERENCEDBASICANNOTATIONBOXSEQ:
		case PRN_BFB_K_REFPRINTJOBSEQ:
		    break;
		}
	    }
	}
    }
    if (*flagName)		/* implies some values were unsupported */
	return APP_UNSUPPORTEDATTRIBUTEVALUE;

    return APP_NORMAL;
}

static APP_FLAGLIST bibFlagList[] = {
    PRN_BIB_K_IMAGEPOSITION,
    PRN_BIB_K_IMAGEMODULE,
    PRN_BIB_K_POLARITY,
    PRN_BIB_K_MAGNIFICATIONTYPE,
    PRN_BIB_K_SMOOTHINGTYPE,
    PRN_BIB_K_REQUESTEDIMAGESIZE
};

static CONDITION
supportedBasicGrayscaleImageBoxValues(PRN_BASICIMAGEBOX * request,
		     SUPPORTEDATTRIBUTES * list, int count, char **flagName)
{
    APP_BFBIMAGEDISPLAYFORMAT
    bfbImageDisplayFormat;	/* This structure is necessary since we have
				 * to make a runtime check on the value of
				 * the attribute image position */
    int
        index,
        i,
        j;			/* various loop indices */
    CTNBOOLEAN
	existsFlag = FALSE;	/* indicates if the attribute value exists or
				 * not */
    BIB_SUPPORTED_PREFORMATTEDIMAGEBOXSEQUENCE
	* imageBoxSequence;
    BIB_SUPPORTED_POLARITY
	* polarity;
    BIB_SUPPORTED_MAGNIFICATIONTYPE
	* magnificationType;
    BIB_SUPPORTED_SMOOTHINGTYPE
	* smoothingType;
    BIB_SUPPORTED_REQUESTEDIMAGESIZE
	* requestedImageSize;

    /*
     * Go thru the list of defined attributes for this SOP class. If such an
     * attribute was requested by the SCU, proceed further to verify that the
     * value requested is supported by the performing SCU
     */
    for (index = 0; index < (int) DIM_OF(bibFlagList); index++) {
	if (!(bibFlagList[index].flag & request->bibAttributeFlag)) {
	    /*
	     * That attribute was not requested by the SCU. Proceed to the
	     * next flag in the list
	     */
	    continue;
	}
	/*
	 * bibFlagList[index].flag gives the flag value for which the
	 * attribute is requested. Go thru the list of values which we
	 * support for the requested attribute and verify if the requested
	 * value is supported
	 */
	for (i = 0; i < count; i++) {
	    if (bibFlagList[index].flag == list[i].flag) {
		/* flag is supported */
		switch (bibFlagList[index].flag) {
		case PRN_BIB_K_IMAGEPOSITION:{
			APP_BFBIMAGEDISPLAYFORMAT
			    displayFormat;	/* stores image display
						 * format of last created
						 * Basic Film Box */
			PRN_BASICFILMBOX
			    bfbAttrib, *bfbPtr;	/* Basic Film Box structure
						 * that holds the attribute
						 * imageDisplayFormat */
			unsigned short
			    totalImages = 0;	/* total images defined by
						 * the image display format */

			existsFlag = FALSE;
			/*
			 * Here we must first obtain the Image Display Format
			 * from the attributes of the last created Basic Film
			 * Box instance.
			 */
			bfbPtr = &bfbAttrib;
			if (PRN_ParseObject(&bfbInstance->bfbAttrib,
					    PRN_K_BASICFILMBOX, (void **) &bfbPtr) == PRN_NORMAL) {
			    (void) decodeImageDisplayFormat(bfbPtr->imageDisplayFormat,
							    &displayFormat);

			    /*
			     * calculate the total number of images  and then
			     * verify if the imagePosition lies within the
			     * range 1 to totalImages
			     */
			    switch (displayFormat.type) {
			    case STANDARD_FORMAT:
				totalImages = displayFormat.format.standard.numRows *
				    displayFormat.format.standard.numColumns;
				break;
			    case ROW_FORMAT:
				for (j = 0; j < displayFormat.format.row.numRows; j++)
				    totalImages +=
					displayFormat.format.row.imagesPerRow[j];
				break;
			    case COL_FORMAT:
				for (j = 0; j < displayFormat.format.col.numColumns; j++)
				    totalImages +=
					displayFormat.format.col.imagesPerColumn[j];
				break;
			    }
			    if ((request->imagePosition >= 1) &&
				(request->imagePosition <= totalImages))
				existsFlag = TRUE;
			}
			if (!existsFlag) {
			    char temp[32];
			    sprintf(temp, "%d", request->imagePosition);
			    /* Value not supported */
			    updateFlagName(flagName, list[i].listName, ": ", temp);
			}
		    }
		case PRN_BIB_K_IMAGEMODULE:
		    existsFlag = TRUE;
		    break;
		case PRN_BIB_K_POLARITY:
		    polarity = (BIB_SUPPORTED_POLARITY *) list[i].valueList;
		    existsFlag = FALSE;
		    for (j = 0; j < list[i].listCount; j++) {
			if (!strcmp(request->polarity,
				    polarity[j].polarity)) {
			    existsFlag = TRUE;
			    break;
			}
		    }
		    if (!existsFlag) {
			/* value not supported */
			updateFlagName(flagName, list[i].listName, ": ",
				       request->polarity, 0);
			strcpy(request->polarity, polarity[0].polarity);
		    }
		    break;
		case PRN_BIB_K_MAGNIFICATIONTYPE:
		    magnificationType = (BIB_SUPPORTED_MAGNIFICATIONTYPE *)
			list[i].valueList;
		    existsFlag = FALSE;
		    for (j = 0; j < list[i].listCount; j++) {
			if (!strcmp(request->magnificationType,
				  magnificationType[j].magnificationType)) {
			    existsFlag = TRUE;
			    break;
			}
		    }
		    if (!existsFlag) {
			/* value not supported */
			updateFlagName(flagName, list[i].listName, ": ",
				       request->magnificationType, 0);
			strcpy(request->magnificationType,
			       magnificationType[0].magnificationType);
		    }
		    break;
		case PRN_BIB_K_SMOOTHINGTYPE:
		    smoothingType = (BIB_SUPPORTED_SMOOTHINGTYPE *)
			list[i].valueList;
		    existsFlag = FALSE;
		    for (j = 0; j < list[i].listCount; j++) {
			if (!strcmp(request->smoothingType,
				    smoothingType[j].smoothingType)) {
			    existsFlag = TRUE;
			    break;
			}
		    }
		    if (!existsFlag) {
			/* value not supported */
			updateFlagName(flagName, list[i].listName, ": ",
				       request->smoothingType, 0);
			strcpy(request->smoothingType,
			       smoothingType[0].smoothingType);
		    }
		    break;
		case PRN_BIB_K_REQUESTEDIMAGESIZE:
		    requestedImageSize = (BIB_SUPPORTED_REQUESTEDIMAGESIZE *)
			list[i].valueList;
		    existsFlag = FALSE;
		    for (j = 0; j < list[i].listCount; j++) {
			if (!strcmp(request->requestedImageSize,
				requestedImageSize[j].requestedImageSize)) {
			    existsFlag = TRUE;
			    break;
			}
		    }
		    if (!existsFlag) {
			/* value not supported */
			updateFlagName(flagName, list[i].listName, ": ",
				       request->requestedImageSize, 0);
			strcpy(request->requestedImageSize,
			       requestedImageSize[0].requestedImageSize);
		    }
		}
	    }
	}
    }
    if (*flagName)		/* some value was not supported */
	return APP_UNSUPPORTEDATTRIBUTEVALUE;
    return APP_NORMAL;
}

/* applyNCreateBasicFilmSessionDefaultValues
**
** Purpose:
**	This routine is meant to deal with the U/M case of attribute support.
** In the event that the SCU does not provide any value, the SCP applies a
** default value to the attribute and updates the flag in the request structure.
** We can be assured that the SCP supports the attribute because it is
** mandatory for the SCP to support it and such a verification has been already
** made in the routine "supportedAttributes" before this routine is invoked
**
** Parameter Dictionary:
**	request		Pointer to the Basic Film Session request structure
**	supported	Pointer to an array of supported attributes by SCP
**	count		The number of entries in the list of supported
**			attributes
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
applyNCreateBasicFilmSessionDefaultValues(PRN_BASICFILMSESSION * request,
				      SUPPORTEDATTRIBUTES * list, int count)
{
    int
        index,
        i;
    BFS_SUPPORTED_COPIES
	* copies;
    BFS_SUPPORTED_PRIORITY
	* priority;
    BFS_SUPPORTED_MEDIUMTYPE
	* mediumType;
    BFS_SUPPORTED_FILMDESTINATION
	* filmDestination;

    /*
     * Go thru the list of defined attributes for this SOP class. If such an
     * attribute was requested by the SCU, proceed further to verify that the
     * value requested is supported by the performing SCU
     */
    for (index = 0; index < (int) DIM_OF(bfsSCPCreateMandatoryFlag); index++) {
	if ((bfsSCPCreateMandatoryFlag[index].flag &
	     request->bfsAttributeFlag)) {
	    /*
	     * that attribute was already present. Proceed to the next one.
	     */
	    continue;
	}
	for (i = 0; i < count; i++) {
	    if (bfsSCPCreateMandatoryFlag[index].flag == list[i].flag) {
		switch (bfsSCPCreateMandatoryFlag[index].flag) {
		case PRN_BFS_K_COPIES:
		    copies = (BFS_SUPPORTED_COPIES *) list[i].valueList;
		    strcpy(request->copies, copies[0].copies);
		    request->bfsAttributeFlag |=
			bfsSCPCreateMandatoryFlag[index].flag;
		    break;
		case PRN_BFS_K_PRIORITY:
		    priority = (BFS_SUPPORTED_PRIORITY *) list[i].valueList;
		    strcpy(request->priority, priority[0].priority);
		    request->bfsAttributeFlag |=
			bfsSCPCreateMandatoryFlag[index].flag;
		    break;
		case PRN_BFS_K_MEDIUMTYPE:
		    mediumType = (BFS_SUPPORTED_MEDIUMTYPE *)
			list[i].valueList;
		    strcpy(request->mediumType, mediumType[0].mediumType);
		    request->bfsAttributeFlag |=
			bfsSCPCreateMandatoryFlag[index].flag;
		    break;
		case PRN_BFS_K_FILMDESTINATION:
		    filmDestination = (BFS_SUPPORTED_FILMDESTINATION *)
			list[i].valueList;
		    strcpy(request->filmDestination,
			   filmDestination[0].filmDestination);
		    request->bfsAttributeFlag |=
			bfsSCPCreateMandatoryFlag[index].flag;
		    break;
		default:
		    break;
		}
	    }
	}
    }
}

/* applyNCreateBasicFilmBoxDefaultValues
**
** Purpose:
**	For all those U/M attributes for which SCU doesn't send any values,
**	this routine makes the SCP apply default values to the requested
**	structure and updates the flag
**
** Parameter Dictionary:
**	request		Pointer to Basic Film Box attributes to be verified
**	list		Pointer to a list of attributes and their values
**			supported by the SCP
**	count		Size of list
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
applyNCreateBasicFilmBoxDefaultValues(PRN_BASICFILMBOX * request,
				      SUPPORTEDATTRIBUTES * list, int count)
{
    int
        index,
        i;
    BFB_SUPPORTED_FILMORIENTATION
	* filmOrientation;
    BFB_SUPPORTED_FILMSIZEID
	* filmSizeID;
    BFB_SUPPORTED_MAGNIFICATIONTYPE
	* magnificationType;
    BFB_SUPPORTED_CONFIGURATIONINFO
	* configurationInfo;
    /*
     * Go thru the list of defined attributes for this SOP class. If such an
     * attribute was requested by the SCU, proceed further to verify that the
     * value requested is supported by the performing SCU
     */
    for (index = 0; index < (int) DIM_OF(bfbSCPCreateMandatoryFlag); index++) {
	if ((bfbSCPCreateMandatoryFlag[index].flag &
	     request->bfbAttributeFlag)) {
	    /*
	     * that attribute was already present. Proceed to the next one.
	     */
	    continue;
	}
	for (i = 0; i < count; i++) {
	    if (bfbSCPCreateMandatoryFlag[index].flag == list[i].flag) {
		switch (bfbSCPCreateMandatoryFlag[index].flag) {
		case PRN_BFB_K_FILMORIENTATION:
		    filmOrientation =
			(BFB_SUPPORTED_FILMORIENTATION *) list[i].valueList;
		    strcpy(request->filmOrientation,
			   filmOrientation[0].filmOrientation);
		    request->bfbAttributeFlag |=
			bfbSCPCreateMandatoryFlag[index].flag;
		    break;
		case PRN_BFB_K_FILMSIZEID:
		    filmSizeID = (BFB_SUPPORTED_FILMSIZEID *)
			list[i].valueList;
		    strcpy(request->filmSizeID, filmSizeID[0].filmSizeID);
		    request->bfbAttributeFlag |=
			bfbSCPCreateMandatoryFlag[index].flag;
		    break;
		case PRN_BFB_K_MAGNIFICATIONTYPE:
		    magnificationType =
			(BFB_SUPPORTED_MAGNIFICATIONTYPE *) list[i].valueList;
		    strcpy(request->magnificationType,
			   magnificationType[0].magnificationType);
		    request->bfbAttributeFlag |=
			bfbSCPCreateMandatoryFlag[index].flag;
		    break;
		case PRN_BFB_K_CONFIGURATIONINFO:
		    configurationInfo = (BFB_SUPPORTED_CONFIGURATIONINFO *)
			list[i].valueList;
		    strcpy(request->configurationInfo,
			   configurationInfo[0].configurationInfo);
		    request->bfbAttributeFlag |=
			bfbSCPCreateMandatoryFlag[index].flag;
		    break;
		case PRN_BFB_K_MAXDENSITY:
		    request->maxDensity = 500;
		    request->bfbAttributeFlag |=
			bfbSCPCreateMandatoryFlag[index].flag;
		    break;
		default:
		    break;
		}
	    }
	}
    }
}
static void
applyNSetBasicGrayscaleImageBoxDefaultValues(PRN_BASICIMAGEBOX * request,
				      SUPPORTEDATTRIBUTES * list, int count)
{
    int
        index,
        i;
    BIB_SUPPORTED_POLARITY
	* polarity;
    BIB_SUPPORTED_MAGNIFICATIONTYPE
	* magnificationType;
    BIB_SUPPORTED_SMOOTHINGTYPE
	* smoothingType;
    BIB_SUPPORTED_REQUESTEDIMAGESIZE
	* requestedImageSize;
    /*
     * Go thru the list of defined attributes for this SOP class. If such an
     * attribute was requested by the SCU, proceed further to verify that the
     * value requested is supported by the performing SCU
     */
    for (index = 0; index < (int) DIM_OF(bibSCPSetMandatoryFlag); index++) {
	if ((bibSCPSetMandatoryFlag[index].flag &
	     request->bibAttributeFlag)) {
	    /*
	     * that attribute was already present. Proceed to the next one.
	     */
	    continue;
	}
	for (i = 0; i < count; i++) {
	    if (bibSCPSetMandatoryFlag[index].flag == list[i].flag) {
		switch (bibSCPSetMandatoryFlag[index].flag) {
		case PRN_BIB_K_POLARITY:
		    polarity =
			(BIB_SUPPORTED_POLARITY *) list[i].valueList;
		    strcpy(request->polarity,
			   polarity[0].polarity);
		    request->bibAttributeFlag |=
			bibSCPSetMandatoryFlag[index].flag;
		    break;
		default:
		    break;
		}
	    }
	}
    }
}
static void
applyNSetBasicColorImageBoxDefaultValues(PRN_BASICIMAGEBOX * request,
				      SUPPORTEDATTRIBUTES * list, int count)
{
    int
        index,
        i;
    BIB_SUPPORTED_POLARITY
	* polarity;
    BIB_SUPPORTED_MAGNIFICATIONTYPE
	* magnificationType;
    BIB_SUPPORTED_SMOOTHINGTYPE
	* smoothingType;
    BIB_SUPPORTED_REQUESTEDIMAGESIZE
	* requestedImageSize;
    /*
     * Go thru the list of defined attributes for this SOP class. If such an
     * attribute was requested by the SCU, proceed further to verify that the
     * value requested is supported by the performing SCU
     */
    for (index = 0; index < (int) DIM_OF(bibSCPSetMandatoryFlag); index++) {
	if ((bibSCPSetMandatoryFlag[index].flag &
	     request->bibAttributeFlag)) {
	    /*
	     * that attribute was already present. Proceed to the next one.
	     */
	    continue;
	}
	for (i = 0; i < count; i++) {
	    if (bibSCPSetMandatoryFlag[index].flag == list[i].flag) {
		switch (bibSCPSetMandatoryFlag[index].flag) {
		case PRN_BIB_K_POLARITY:
		    polarity =
			(BIB_SUPPORTED_POLARITY *) list[i].valueList;
		    strcpy(request->polarity,
			   polarity[0].polarity);
		    request->bibAttributeFlag |=
			bibSCPSetMandatoryFlag[index].flag;
		    break;
		default:
		    break;
		}
	    }
	}
    }
}
