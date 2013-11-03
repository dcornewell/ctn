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
** Module Name(s):
** Author, Date:	Aniruddha S. Gokhale, June 14, 1993
** Intent:
**	PRINT MANAGEMENT SOP CLASS header file for the following SOP classes
**
**	The following Service Object Pair classes are implemented :
**
**	Basic Film Session SOP class
**	Basic Film Box SOP class
**	Basic Image Box SOP class
**	Basic Annotation Box SOP class
**	Printer Job SOP class
**	Printer SOP class
**	Preformatted Greyscale Image SOP class
**	Preformatted Color Image SOP class
**
** Last Update:		$Author: smm $, $Date: 1998-04-17 14:29:30 $
** Source File:		$RCSfile: dicom_print.h,v $
** Revision:		$Revision: 1.20 $
** Status:		$State: Exp $
*/

#ifndef DICOM_PRN_IS_IN
#define DICOM_PRN_IS_IN 1

#ifdef  __cplusplus
extern "C" {
#endif

typedef enum {
    PRN_K_BASICFILMSESSION,
    PRN_K_BASICFILMBOX,
    PRN_K_BASICIMAGEBOX,
    PRN_K_BASICANNOTATIONBOX,
    PRN_K_BASICPRINTJOB,
    PRN_K_BASICPRINTER,
    PRN_K_BASICGREYSCALEIMAGEBOX,
    PRN_K_BASICCOLORIMAGEBOX
}   PRN_TYPE;

typedef struct {
    void *reserved[2];		/* For use by LST facility */
    PRN_TYPE type;
}   PRN_GENERAL;

/*
typedef struct {
    void *reserved[2];
    PRN_TYPE type;
    char classUID[DICOM_UI_LENGTH + 1];
    char instanceUID[DICOM_UI_LENGTH + 1];
}   PRN_REFERENCED_ITEM;
*/

CONDITION
PRN_BuildObject(void *structure, DCM_OBJECT ** object);

CONDITION
PRN_ParseObject(DCM_OBJECT ** object, PRN_TYPE type, void **structure);
CONDITION
PRN_FreeStructure(void **structure);

char *PRN_Message(CONDITION cond);

void
    PRN_DumpAttributes(void *attrib, FILE * f);

#define PRN_NORMAL		FORM_COND(FAC_PRN, SEV_SUCC, 1)
#define PRN_BUILDFAILED		FORM_COND(FAC_PRN, SEV_ERROR, 2)
#define PRN_PARSEFAILED		FORM_COND(FAC_PRN, SEV_ERROR, 3)
#define	PRN_LISTCREATEFAILURE	FORM_COND(FAC_PRN, SEV_ERROR, 4)
#define	PRN_MALLOCFAILURE	FORM_COND(FAC_PRN, SEV_ERROR, 5)
#define	PRN_ILLEGALUIDLENGTH	FORM_COND(FAC_PRN, SEV_ERROR, 6)
#define	PRN_LISTFAILURE		FORM_COND(FAC_PRN, SEV_ERROR, 7)

/*	BASIC FILM SESSION SOP CLASS
	----------------------------
*/

/* enumerated values for the film session priority */
#define	HIGH	"HIGH"
#define	MED	"MED"
#define	LOW	"LOW"

/* enumerated values for film session medium type */
#define	PAPER	"PAPER"
#define	CLEAR_FILM	"CLEAR FILM"
#define	BLUE_FILM	"BLUE FILM"

/* enumerated values for film session destination */
#define	MAGAZINE	"MAGAZINE"
#define	PROCESSOR	"PROCESSOR"

typedef struct {		/* basic film session attribute list */
    void *reserved[2];		/* For use by LST facility */
    PRN_TYPE type;
    long bfsAttributeFlag;
    char filmSessionSOPClassUID[DICOM_UI_LENGTH + 1];
    char filmSessionSOPInstanceUID[DICOM_UI_LENGTH + 1];
    char copies[DICOM_IS_LENGTH + 1];
    char priority[DICOM_CS_LENGTH + 1];
    char mediumType[DICOM_CS_LENGTH + 1];
    char filmDestination[DICOM_CS_LENGTH + 1];
    char filmSessionLabel[DICOM_LO_LENGTH + 1];
    char memoryAllocation[DICOM_IS_LENGTH + 1];
    LST_HEAD *referencedBFBSeq;	/* seq. of child referenced BFB UIDS */
}   PRN_BASICFILMSESSION;

/* PRN_BFS attribute flags */
#define PRN_BFS_K_COPIES			0x1
#define PRN_BFS_K_PRIORITY			0x2
#define PRN_BFS_K_MEDIUMTYPE			0x4
#define PRN_BFS_K_FILMDESTINATION		0x8
#define PRN_BFS_K_FILMSESSIONLABEL		0x10
#define PRN_BFS_K_MEMORYALLOCATION		0x20
#define PRN_BFS_K_REFERENCEDBFBSEQ		0x40

/* Mandatory attributes flag for SCU and SCP */
#define REQUIRED_BFS_SCU_CREATEATTRIB		0x0

#define REQUIRED_BFS_SCP_CREATEATTRIB (PRN_BFS_K_COPIES | PRN_BFS_K_PRIORITY |\
			PRN_BFS_K_MEDIUMTYPE | PRN_BFS_K_FILMDESTINATION)
#define REQUIRED_BFS_SCP_SETATTRIB (PRN_BFS_K_COPIES | PRN_BFS_K_PRIORITY |\
	PRN_BFS_K_MEDIUMTYPE | PRN_BFS_K_FILMDESTINATION )

/*	BASIC FILM BOX SOP CLASS
	------------------------
*/

/* basic film box image display format enumerated values */
#define STANDARD "STANDARD"
#define ROW "ROW"
#define COL "COL"
#define SLIDE "SLIDE"
#define SUPERSLIDE "SUPERSLIDE"
#define CUSTOM "CUSTOM"

/* basic film box film orientation enumerated values */
#define	PORTRAIT	"PORTRAIT"
#define	LANDSCAPE	"LANDSCAPE"

/* defined film sizes */
#define	SIZE8INX10IN	"8INX10IN"
#define	SIZE10INX12IN	"10INX12IN"
#define	SIZE10INX14IN	"10INX14IN"
#define	SIZE11INX14IN	"11INX14IN"
#define	SIZE14INX14IN	"14INX14IN"
#define	SIZE14INX17IN	"14INX17IN"
#define	SIZE24CMX30CM	"24CMX30CM"
#define	SIZE24CMX24CM	"24CMX24CM"

/* magnification types */
#define	REPLICATE	"REPLICATE"
#define	BILINEAR	"BILINEAR"
#define	CUBIC		"CUBIC"
#define	NONE		"NONE"

/* smoothing type */
#define	SMOOTHINGTYPENONE	"NONE"

/* border density */
#define	BLACKBORDER	"BLACK"
#define	WHITEBORDER	"WHITE"

/* empty image density */
#define	BLACK		"BLACK"
#define	WHITE		"WHITE"

/*trim value */
#define	YES		"YES"
#define	NO		"NO"

typedef struct {
    void *reserved[2];		/* For use by LST facility */
    PRN_TYPE type;
    long bfbAttributeFlag;
    char filmBoxSOPClassUID[DICOM_UI_LENGTH + 1];
    char filmBoxSOPInstanceUID[DICOM_UI_LENGTH + 1];
    char referencedFilmSessionSOPClassUID[DICOM_UI_LENGTH + 1];
    char referencedFilmSessionSOPInstanceUID[DICOM_UI_LENGTH + 1];
#if STANDARD_VERSION < VERSION_AUG1993
    char imageDisplayFormat[DICOM_LT_LENGTH + 1];
#else
    char imageDisplayFormat[DICOM_ST_LENGTH + 1];
#endif
    char annotationDisplayFormatID[DICOM_CS_LENGTH + 1];
    char filmOrientation[DICOM_CS_LENGTH + 1];
    char filmSizeID[DICOM_CS_LENGTH + 1];
    char magnificationType[DICOM_CS_LENGTH + 1];
    char smoothingType[DICOM_CS_LENGTH + 1];
    char borderDensity[DICOM_CS_LENGTH + 1];
    char emptyImageDensity[DICOM_CS_LENGTH + 1];
#if STANDARD_VERSION < VERSION_AUG1993
    char minDensity[DICOM_IS_LENGTH + 1];
    char maxDensity[DICOM_IS_LENGTH + 1];
#else
    unsigned short minDensity;
    unsigned short maxDensity;
#endif
    char trim[DICOM_CS_LENGTH + 1];
    char configurationInfo[DICOM_ST_LENGTH + 1];
    LST_HEAD *referencedBIBSeq;	/* basic image box sequence */
    LST_HEAD *referencedBABSeq;	/* basic annotation box sequence */
    char refPrintJobClassUID[DICOM_UI_LENGTH + 1];
    char refPrintJobInstanceUID[DICOM_UI_LENGTH + 1];
}   PRN_BASICFILMBOX;

/* PRN_BFB attribute flags */
#define PRN_BFB_K_IMAGEDISPLAYFORMAT			0x1
#define PRN_BFB_K_ANNOTATIONDISPLAYFORMATID		0x2
#define PRN_BFB_K_FILMORIENTATION			0x4
#define PRN_BFB_K_FILMSIZEID				0x8
#define PRN_BFB_K_MAGNIFICATIONTYPE			0x10
#define PRN_BFB_K_SMOOTHINGTYPE				0x20
#define PRN_BFB_K_BORDERDENSITY				0x40
#define PRN_BFB_K_EMPTYIMAGEDENSITY			0x80
#define PRN_BFB_K_MINDENSITY				0x100
#define PRN_BFB_K_MAXDENSITY				0x200
#define PRN_BFB_K_TRIM					0x400
#define PRN_BFB_K_CONFIGURATIONINFO			0x800
#define PRN_BFB_K_REFERENCEDBASICFILMSESSIONSEQ 	0x1000
#define PRN_BFB_K_REFERENCEDBASICIMAGEBOXSEQ		0x2000
#define PRN_BFB_K_REFERENCEDBASICANNOTATIONBOXSEQ	0x4000
#define	PRN_BFB_K_REFPRINTJOBSEQ			0x8000

/* Mandatory attributes to be provided by SCU and SCP */
#define REQUIRED_BFB_SCU_CREATEATTRIB	(PRN_BFB_K_IMAGEDISPLAYFORMAT | \
				PRN_BFB_K_REFERENCEDBASICFILMSESSIONSEQ)
#define REQUIRED_BFB_SCP_CREATEATTRIB	(PRN_BFB_K_IMAGEDISPLAYFORMAT | \
	PRN_BFB_K_REFERENCEDBASICFILMSESSIONSEQ | PRN_BFB_K_FILMORIENTATION | \
	PRN_BFB_K_FILMSIZEID | PRN_BFB_K_MAGNIFICATIONTYPE | \
	PRN_BFB_K_MAXDENSITY | PRN_BFB_K_CONFIGURATIONINFO | \
	PRN_BFB_K_REFERENCEDBASICIMAGEBOXSEQ)

#define REQUIRED_BFB_SCP_SETATTRIB	(PRN_BFB_K_MAGNIFICATIONTYPE | \
		PRN_BFB_K_MAXDENSITY | PRN_BFB_K_CONFIGURATIONINFO )

#define	PRN_K_FILMBOX_ACTION_PRINT	1

/*	BASIC IMAGE BOX SOP CLASS
	-------------------------
*/

/* enumerated values for tthe attribute polarity */
#define	NORMAL	"NORMAL"
#define	REVERSE	"REVERSE"

typedef struct {
    void *reserved[2];		/* For use by LST facility */
    PRN_TYPE type;
    long bibAttributeFlag;
    char imageBoxSOPClassUID[DICOM_UI_LENGTH + 1];
    char imageBoxSOPInstanceUID[DICOM_UI_LENGTH + 1];
    unsigned short imagePosition;
    char polarity[DICOM_CS_LENGTH + 1];
    char magnificationType[DICOM_CS_LENGTH + 1];
    char smoothingType[DICOM_CS_LENGTH + 1];
#if STANDARD_VERSION < VERSION_AUG1993
    char magnificationFactor[DICOM_DS_LENGTH + 1];
#else
    char requestedImageSize[DICOM_DS_LENGTH + 1];
#endif
    DCM_OBJECT *object;		/* Preformatted image object */
    LST_HEAD *referencedImageSequence;
    LST_HEAD *referencedVOILUTSeq;
    LST_HEAD *referencedImageOverlayBoxSeq;
}   PRN_BASICIMAGEBOX;

/* BIB attribute flags */
#define PRN_BIB_K_IMAGEPOSITION		0x1
#define PRN_BIB_K_POLARITY		0x2
#define PRN_BIB_K_MAGNIFICATIONTYPE	0x4
#define PRN_BIB_K_SMOOTHINGTYPE		0x8
#define PRN_BIB_K_REQUESTEDIMAGESIZE	0x10
#define PRN_BIB_K_IMAGEMODULE		0x20
/* following two are needed for version less than Aug 1993 */
#define PRN_BIB_K_MAGNIFICATIONFACTOR	0x40
#define PRN_BIB_K_IMAGEBOXSOPCLASSUID	0x80

/* Mandatory attributes to be provided by SCU and SCP */

#define REQUIRED_BIB_SCU_NSETATTRIB	(PRN_BIB_K_IMAGEPOSITION | \
					PRN_BIB_K_IMAGEMODULE)
#define REQUIRED_BIB_SCP_NSETATTRIB	(PRN_BIB_K_IMAGEPOSITION | \
					PRN_BIB_K_POLARITY |\
					PRN_BIB_K_IMAGEMODULE)


/*	BASIC ANNOTATION BOX SOP CLASS
	------------------------------
*/

typedef struct {
    void *reserved[2];		/* For use by LST facility */
    PRN_TYPE type;
    long babAttributeFlag;
    char annotationBoxSOPClassUID[DICOM_UI_LENGTH + 1];
    char annotationBoxSOPInstanceUID[DICOM_UI_LENGTH + 1];
#if STANDARD_VERSION < VERSION_AUG1993
    char annotationPosition[DICOM_IS_LENGTH + 1];
#else
    unsigned short annotationPosition;
#endif
    char textString[DICOM_LO_LENGTH + 1];
}   PRN_BASICANNOTATIONBOX;

/* BAB attribute flags */
#define PRN_BAB_K_ANN0TATIONPOSITION	0x1
#define PRN_BAB_K_TEXTSTRING		0x2

/* Mandatory attributes to be provided by SCU and SCP */
#define REQUIRED_BAB_SCU_NSETATTRIB PRN_BAB_K_ANN0TATIONPOSITION
#define REQUIRED_BAB_SCP_NSETATTRIB (PRN_BAB_K_ANN0TATIONPOSITION | \
					PRN_BAB_K_TEXTSTRING)


/*	PRINT_JOB SOP CLASS
	-------------------
*/
/* enumerated values for execution status */
#define PENDING		"PENDING"
#define PRINTING	"PRINTING"
#define PRINTDONE	"DONE"
#define PRINTFAIL	"FAILURE"

/* enumerated values for execution status info */
#define PRINTQUEUED		"QUEUED"
#define PRINTNOSUPPLYMGZ	"NO_SUPPLY_MGZ"
#define PRINTNORECEIVEMGZ	"NO_RECEIVE_MGZ"
#define PRINTSUPPLYEMPTY	"SUPPLY EMPTY"
#define PRINTRECEIVERFULL	"RECEIVER FULL"
#define PRINTFILMJAM		"FILM JAM"

typedef struct {
    void *reserved[2];		/* For use by LST facility */
    PRN_TYPE type;
    long printJobAttributeFlag;
    char printJobSOPClassUID[DICOM_UI_LENGTH + 1];
    char printJobSOPInstanceUID[DICOM_UI_LENGTH + 1];
    char executionStatus[DICOM_CS_LENGTH + 1];
    char executionStatusInfo[DICOM_CS_LENGTH + 1];
    char creationDate[DICOM_DA_LENGTH + 1];
    char creationTime[DICOM_TM_LENGTH + 1];
    char printPriority[DICOM_CS_LENGTH + 1];
    char printerName[DICOM_SH_LENGTH + 1];
    char originator[DICOM_AE_LENGTH + 1];
}   PRN_BASICPRINTJOB;

/* PJ attribute flags */
#define PRN_PJ_K_EXECUTIONSTATUS	0x1
#define PRN_PJ_K_EXECUTIONSTATUSINFO	0x2
#define PRN_PJ_K_CREATIONDATE		0x4
#define PRN_PJ_K_CREATIONTIME		0x8
#define PRN_PJ_K_PRINTPRIORITY		0x10
#define PRN_PJ_K_PRINTERNAME		0x20
#define PRN_PJ_K_ORIGINATOR		0x40

/* Mandatory attributes to be provided by SCU and SCP */
#define REQUIRED_PJ_SCU_NGETATTRIB	0x0
#define REQUIRED_PJ_SCP_NGETATTRIB (PRN_PJ_K_EXECUTIONSTATUS | \
			PRN_PJ_K_EXECUTIONSTATUSINFO | PRN_PJ_K_PRINTPRIORITY)

/*	PRINTER SOP CLASS
	-----------------
*/

/* enumerated values for PRINTER status */
#define	PRINTERNORMAL		"NORMAL"
#define	PRINTERWARNING		"WARNING"
#define	PRINTERFAILURE		"FAILURE"

/* enumerated values for printer status info code */
#define	NO_INFO		"NO-INFO"
#define	SUPPLY_EMPTY	"SUPPLY_EMPTY"
#define	SUPPLY_LOW	"SUPPLY_LOW"
#define	RECEIVER_FULL	"RECEIVER_FULL"
#define	FILM_JAM	"FILM_JAM"

typedef struct {
    void *reserved[2];		/* For use by LST facility */
    PRN_TYPE type;
    long attributeFlag;
    char printerSOPclassUID[DICOM_UI_LENGTH + 1];
    char printerSOPinstanceUID[DICOM_UI_LENGTH + 1];
    char status[DICOM_CS_LENGTH + 1];
    char statusInfoCode[DICOM_CS_LENGTH + 1];
    char name[DICOM_LO_LENGTH + 1];
    char manufacturer[DICOM_LO_LENGTH + 1];
    char manufacturerModelName[DICOM_LO_LENGTH + 1];
    char deviceSerialNumber[DICOM_LO_LENGTH + 1];
    char softwareVersion[DICOM_LO_LENGTH + 1];
    char dateOfLastCalibration[DICOM_DA_LENGTH + 1];
    char timeOfLastCalibration[DICOM_TM_LENGTH + 1];
}   PRN_PRINTER;

/* PRINTER attribute flags */
#define PRN_PRINTER_K_STATUS		0x1
#define PRN_PRINTER_K_STATUSINFO	0x2
#define PRN_PRINTER_K_NAME		0x4
#define PRN_PRINTER_K_MANUFACTURER	0x8
#define PRN_PRINTER_K_MODELNUMBER	0x10
#define PRN_PRINTER_K_SERIALNUMBER	0x20
#define PRN_PRINTER_K_SOFTWAREVERSION	0x40
#define PRN_PRINTER_K_DATEOFCALIBRATION	0x80
#define PRN_PRINTER_K_TIMEOFCALIBRATION	0x100

/* Mandatory flags to be provided bby SCU and SCP */
#define REQUIRED_PRINTER_SCU_NGETATTRIB	0x0
#define REQUIRED_PRINTER_NGETATTRIB	(PRN_PRINTER_K_STATUS | \
					PRN_PRINTER_K_STATUSINFO)


/*	PREFORMATTED GREYSCALE IMAGE SOP CLASS
	--------------------------------------
*/

/* enumerated values for bits allocated */
#define ALLOCATEDEIGHT  8
#define ALLOCATEDSIXTEEN 16

/* enumerated values for bits stored */
#define STOREDEIGHT 8
#define STOREDTWELVE 12

/* enumerated values for high bit */
#define HIGHBITSEVEN 7
#define HIGHBITELEVEN 11

typedef struct {
    void *reserved[2];		/* For use by LST facility */
    unsigned short rows;
    unsigned short cols;
    char pixelratio[DICOM_IS_LENGTH + 1];
    unsigned short bitsAllocated;
    unsigned short bitsStored;
    unsigned short highBit;
    unsigned short pixelRepresentation;
    /* structure for pixel data, defined as OB or OW */
}   GREYSCALE_IMAGE_ATTRIBUTES;

#define GREYSCALE_ROWS			0x1
#define GREYSCALE_COLS			0x2
#define GREYSCALE_PIXELRATIO		0x4
#define GREYSCALE_BITSALLOCATED		0x8
#define GREYSCALE_BITSSTORED		0x10
#define GREYSCALE_HIGHBIT		0x20
#define GREYSCALE_PIXELREPRESENTATION	0x40
#define GREYSCALE_PIXELDATA		0x80

#define GREYSCALE_NGETATTRIB	(GREYSCALE_ROWS | GREYSCALE_COLS | \
				GREYSCALE_PIXELRATIO | \
				GREYSCALE_BITSALLOCATED | \
				GREYSCALE_BITSSTORED | GREYSCALE_HIGHBIT | \
				GREYSCALE_PIXELREPRESENTATION | \
				GREYSCALE_PIXELDATA)

/*	PREFORMATTED COLOR IMAGE SOP CLASS
	----------------------------------
*/
typedef struct {
    void *reserved[2];		/* For use by LST facility */
    unsigned short samplesPerPixel /* = 3 */ ;
    unsigned short photometricInterpretation /* has value RGB */ ;
    unsigned short planarConfiguration /* = 0x0001 */ ;
    unsigned short rows;
    unsigned short cols;
    char pixelratio[DICOM_IS_LENGTH + 1];
    unsigned short bitsAllocated;
    unsigned short bitsStored;
    unsigned short highBit;
    unsigned short pixelRepresentation /* = 0x0000 */ ;
    /* structure for pixel data, defined as OB or OW */
}   COLOR_IMAGE_ATTRIBUTES;

#define COLOR_SAMPLESPERPIXEL		0x1
#define COLOR_PHOTOMETRICINTERPRETATION	0x2
#define COLOR_PLANARCONFIGURATION	0x4
#define COLOR_ROWS			0x8
#define COLOR_COLS			0x10
#define COLOR_PIXELRATIO		0x20
#define COLOR_BITSALLOCATED		0x40
#define COLOR_BITSSTORED		0x80
#define COLOR_HIGHBIT			0x100
#define COLOR_PIXELREPRESENTATION	0x200
#define COLOR_PIXELDATA			0x400

#define COLOR_NGETATTRIB	(COLOR_SAMPLESPERPIXE | \
		COLOR_PHOTOMETRICINTERPRETATION | COLOR_PLANARCONFIGURATION| \
		COLOR_ROWS | COLOR_COLS | COLOR_PIXELRATIO | \
		COLOR_BITSALLOCATED | COLOR_BITSSTORED | COLOR_HIGHBIT | \
		COLOR_PIXELREPRESENTATION | COLOR_PIXELDATA)

/* data structure to store the image display format and the row/column
   major order of images
*/
typedef struct {
    char displayFormat[20];	/* stores the image display format e.g.
				 * STANDARD */
    int count;			/* stores total number of image boxes */
    int *numList;
}   IMG_DISPLAYFORMAT;

/*	Data Structure to store the PRINT MANAGEMENT HIERARCHY
	------------------------------------------------------
*/

typedef struct {
    void *reserved[2];
    char bibInstanceUID[DICOM_UI_LENGTH + 1];
    DCM_OBJECT *bibAttrib;
}   BIB_NODE;
typedef struct {
    void *reserved[2];
    char babInstanceUID[DICOM_UI_LENGTH + 1];
    DCM_OBJECT *babAttrib;
}   BAB_NODE;
typedef struct {
    void *reserved[2];
    char bfbInstanceUID[DICOM_UI_LENGTH + 1];
    DCM_OBJECT *bfbAttrib;
    LST_HEAD *bibList;
    LST_HEAD *babList;
}   BFB_NODE;
typedef struct {
    void *reserved[2];		/* For use by LST facility */
    char bfsInstanceUID[DICOM_UI_LENGTH + 1];
    DCM_OBJECT *bfsAttrib;
    LST_HEAD *bfbList;
}   BFS_NODE;

/* Various status codes */

#define BFSCREATESUCCESS		0x0000
#define BFSMEMORYALLOCUNSUPPORTED	0xB600


#ifdef  __cplusplus
}
#endif

#endif
