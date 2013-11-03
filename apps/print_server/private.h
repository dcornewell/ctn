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
** Module Name(s):
** Author, Date:	Aniruddha S. Gokhale, 12-July-1994
** Intent:		Defines private structures for this application
** Last Update:		$Author: smm $, $Date: 1995-12-21 17:23:18 $
** Source File:		$RCSfile: private.h,v $
** Revision:		$Revision: 1.10 $
** Status:		$State: Exp $
*/

/* Define a structure that holds an attribute flag value and the
   corresponding name of the attribute */
typedef struct {
    unsigned long flag;
    DCM_TAG tag;
    char *flagName;
}   FLAGTABLE;

/* Define roles */
typedef enum {
    ROLE_SCU,			/* requesting SCU */
    ROLE_SCP			/* performing SCU */
}   PRN_ROLE;

typedef struct {
    PRN_TYPE prnType;
    MSG_TYPE msgType;
    PRN_ROLE prnRole;
    unsigned long flag;
    FLAGTABLE *flagTable;
    int flagCount;
}   PRN_FLAGMAP;

/* Define various structures for different SOP classes that hold values
   of a specific attribute */

/* All Basic Film Session attribute structures defined here */
typedef struct {
    char copies[DICOM_IS_LENGTH + 1];
}   BFS_SUPPORTED_COPIES;

typedef struct {
    char priority[DICOM_CS_LENGTH + 1];
}   BFS_SUPPORTED_PRIORITY;

typedef struct {
    char mediumType[DICOM_CS_LENGTH + 1];
}   BFS_SUPPORTED_MEDIUMTYPE;

typedef struct {
    char filmDestination[DICOM_CS_LENGTH + 1];
}   BFS_SUPPORTED_FILMDESTINATION;

typedef struct {
    char filmSessionLabel[DICOM_LO_LENGTH + 1];
}   BFS_SUPPORTED_FILMSESSIONLABEL;

typedef struct {
    char memoryAllocation[DICOM_IS_LENGTH + 1];
}   BFS_SUPPORTED_MEMORYALLOCATION;

/* All Basic Film Box SOP class attribute structures defined here */
typedef struct {
    char imageDisplayFormat[DICOM_ST_LENGTH + 1];
}   BFB_SUPPORTED_IMAGEDISPLAYFORMAT;

typedef struct {
    char annotationDisplayFormatID[DICOM_CS_LENGTH + 1];
}   BFB_SUPPORTED_ANNOTATIONDISPLAYFORMATID;

typedef struct {
    char filmOrientation[DICOM_CS_LENGTH + 1];
}   BFB_SUPPORTED_FILMORIENTATION;

typedef struct {
    char filmSizeID[DICOM_CS_LENGTH + 1];
}   BFB_SUPPORTED_FILMSIZEID;

typedef struct {
    char magnificationType[DICOM_CS_LENGTH + 1];
}   BFB_SUPPORTED_MAGNIFICATIONTYPE;

typedef struct {
    char smoothingType[DICOM_CS_LENGTH + 1];
}   BFB_SUPPORTED_SMOOTHINGTYPE;

typedef struct {
    char borderDensity[DICOM_CS_LENGTH + 1];
}   BFB_SUPPORTED_BORDERDENSITY;

typedef struct {
    char emptyImageDensity[DICOM_CS_LENGTH + 1];
}   BFB_SUPPORTED_EMPTYIMAGEDENSITY;

typedef struct {
    unsigned short minDensity;
}   BFB_SUPPORTED_MINDENSITY;

typedef struct {
    unsigned short maxDensity;
}   BFB_SUPPORTED_MAXDENSITY;

typedef struct {
    char trim[DICOM_CS_LENGTH + 1];
}   BFB_SUPPORTED_TRIM;

typedef struct {
    char configurationInfo[DICOM_ST_LENGTH + 1];
}   BFB_SUPPORTED_CONFIGURATIONINFO;

typedef struct {
    char referencedFilmSessionSOPClassUID[DICOM_UI_LENGTH + 1];
    char referencedFilmSessionSOPInstanceUID[DICOM_UI_LENGTH + 1];
}   BFB_SUPPORTED_REFFILMSESSIONSEQUENCE;

/* All the structures for Basic Grayscale Image Box */
typedef struct {
    unsigned short imagePosition;
}   BIB_SUPPORTED_IMAGEPOSITION;

typedef struct {
    DCM_OBJECT *preformattedImageBoxSequence;
}   BIB_SUPPORTED_PREFORMATTEDIMAGEBOXSEQUENCE;

typedef struct {
    char polarity[DICOM_CS_LENGTH + 1];
}   BIB_SUPPORTED_POLARITY;

typedef struct {
    char magnificationType[DICOM_CS_LENGTH + 1];
}   BIB_SUPPORTED_MAGNIFICATIONTYPE;

typedef struct {
    char smoothingType[DICOM_CS_LENGTH + 1];
}   BIB_SUPPORTED_SMOOTHINGTYPE;

typedef struct {
    char requestedImageSize[DICOM_DS_LENGTH + 1];
}   BIB_SUPPORTED_REQUESTEDIMAGESIZE;

#ifdef SMM
/* Define all possible values of attributes supported by the performing SCU for
   the various supported SOP classes of the PRINT MANAGEMENT META SOP CLASS */

 /*
  * The callback defines all possible values of the attributes of the Basic
  * Film Session SOP Class that the performing SCU supports
  */
static BFS_SUPPORTED_COPIES	/* values for the attribute "copies" */
    bfsSupportedCopies[] = {
    "1", "2", "3", "4", "5", "6", "7", "8", "9", "10"
};
static BFS_SUPPORTED_PRIORITY	/* values for the attribute "priority" */
    bfsSupportedPriority[] = {
    HIGH, MED, LOW
};
static BFS_SUPPORTED_MEDIUMTYPE	/* values for the attribute "medium type" */
    bfsSupportedMediumType[] = {
    PAPER, CLEAR_FILM, BLUE_FILM
};
static BFS_SUPPORTED_FILMDESTINATION	/* values for "film destination */
    bfsSupportedFilmDestination[] = {
    MAGAZINE, PROCESSOR
};
static BFS_SUPPORTED_FILMSESSIONLABEL	/* values for "film label" */
    bfsSupportedFilmSessionLabel[] = {
    PAPER
};
 /*
  * define all the values supported by the performing SCU for various
  * attributes of Basic Film Box.
  * 
  */
static BFB_SUPPORTED_FILMORIENTATION
    bfbFilmOrientation[] = {	/* film orientation attribute values */
    PORTRAIT			/* , LANDSCAPE */
};
static BFB_SUPPORTED_FILMSIZEID
    bfbFilmSizeID[] = {		/* film size ID attribute values */
    SIZE8INX10IN,
    SIZE10INX12IN,
    SIZE10INX14IN,
    SIZE11INX14IN,
    SIZE14INX14IN,
    SIZE14INX17IN,
    SIZE24CMX30CM,
    SIZE24CMX24CM
};
static BFB_SUPPORTED_MAGNIFICATIONTYPE
 /* magnification type attribute values */
    bfbMagnificationType[] = {
    REPLICATE,
    BILINEAR,
    CUBIC
};
#ifdef ASG
static BFB_SUPPORTED_MAXDENSITY	/* Maximum density attribute values */
    bfbMaxDensity[] = {
    350
};
#endif
static BFB_SUPPORTED_CONFIGURATIONINFO	/* configuration information */
    bfbConfigurationInfo[] = {
    "ERL PRINTER"
};
static BFB_SUPPORTED_BORDERDENSITY	/* border density attribute values */
    bfbBorderDensity[] = {
    BLACKBORDER,
    WHITEBORDER
};
static BFB_SUPPORTED_EMPTYIMAGEDENSITY
 /* empty image density attribute values */
    bfbEmptyImageDensity[] = {
    BLACK,
    WHITE
};
#ifdef ASG
static BFB_SUPPORTED_MINDENSITY	/* minimum density attribute values */
    bfbMinDensity[] = {
    10
};
#endif
static BFB_SUPPORTED_TRIM	/* Trim attribute values */
    bfbTrim[] = {
    YES, NO
};
static BFB_SUPPORTED_REFFILMSESSIONSEQUENCE
 /* referenced film session sequence */
    bfbRefFilmSessionSequence[] = {
    {DICOM_SOPCLASSBASICFILMSESSION, NULL}
};

 /*
  * Now define all values for the Image Box attributes that are supported by
  * the SCP.
  * 
  */
static BIB_SUPPORTED_POLARITY	/* Image box polarity attribute values */
    bibPolarity[] = {
    NORMAL, REVERSE
};
static BIB_SUPPORTED_MAGNIFICATIONTYPE
 /* image box Magnification type values */
    bibMagnificationType[] = {
    REPLICATE, BILINEAR, CUBIC, NONE
};
static BIB_SUPPORTED_SMOOTHINGTYPE	/* image box Smoothing type values */
    bibSmoothingType[] = {
    CUBIC, NONE
};
static BIB_SUPPORTED_REQUESTEDIMAGESIZE
 /* Image Box requested image size values */
    bibRequestedImageSize[] = {
    10, 20			/* values in millimeters */
};
#endif
