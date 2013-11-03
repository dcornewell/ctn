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
** @$=@$=@$=
*/
/*
**				DICOM 93
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	Tze Khiong Lee
** Intent:		Print Manager Header File
** Last Update:		$Author: smm $, $Date: 1995-12-22 17:24:54 $
** Source File:		$RCSfile: print_mgr.h,v $
** Revision:		$Revision: 1.17 $
** Status:		$State: Exp $
*/


static FIELDwTEXT printer[] = {
    {"Name:", printAttrib.name, sizeof(printAttrib.name),
    FALSE, NULL},
    {"Manufacturer:", printAttrib.manufacturer,
    sizeof(printAttrib.manufacturer), FALSE, NULL},
    {"Model Name:", printAttrib.manufacturerModelName,
    sizeof(printAttrib.manufacturerModelName), FALSE, NULL},
    {"Serial#:", printAttrib.deviceSerialNumber,
    sizeof(printAttrib.deviceSerialNumber), FALSE, NULL}
};

static ENTRY printer_capabilities[] = {
    {FLD_FIELDwTEXT, OPTIONAL, {(void *) &printer[0]}},
    {FLD_FIELDwTEXT, OPTIONAL, {(void *) &printer[1]}},
    {FLD_FIELDwTEXT, OPTIONAL, {(void *) &printer[2]}},
    {FLD_FIELDwTEXT, OPTIONAL, {(void *) &printer[3]}}
};

static RBTNwLABEL CP[] = {
    {"1", "1"},
    {"2", "2"}
};

static ALLRBTN copies_rbtn[] = {
    {BTN_RBTNwLABEL, (void *) &CP[0], NULL},
    {BTN_RBTNwLABEL, (void *) &CP[1], NULL}
};

static RBTNwLABEL PP[] = {
    {"Low", LOW},
    {"Medium", MED},
    {"High", HIGH}
};

static ALLRBTN print_pri_rbtn[] = {
    {BTN_RBTNwLABEL, (void *) &PP[0], NULL},
    {BTN_RBTNwLABEL, (void *) &PP[1], NULL},
    {BTN_RBTNwLABEL, (void *) &PP[2], NULL}
};

static RBTNwLABEL MT[] = {
    {"Paper", PAPER},
    {"Clear Film", CLEAR_FILM},
    {"Blue Film", BLUE_FILM}
};

static ALLRBTN medium_type_rbtn[] = {
    {BTN_RBTNwLABEL, (void *) &MT[0], NULL},
    {BTN_RBTNwLABEL, (void *) &MT[1], NULL},
    {BTN_RBTNwLABEL, (void *) &MT[2], NULL}
};

static RBTNwLABEL FD[] = {
    {"Magazine", MAGAZINE},
    {"Processor", PROCESSOR}
};

static ALLRBTN film_dest_rbtn[] = {
    {BTN_RBTNwLABEL, (void *) &FD[0], NULL},
    {BTN_RBTNwLABEL, (void *) &FD[1], NULL}
};

static PRN_BASICFILMSESSION bfsAttrib = {
    NULL, NULL,
    PRN_K_BASICFILMSESSION,	/* This structure type */
    0,				/* Attribute Flag */
    DICOM_SOPCLASSBASICFILMSESSION,
    "",				/* Film session SOP Instance UID */
    "1",			/* 1 copies */
    HIGH,			/* high priority */
    PAPER,			/* medium is paper */
    MAGAZINE,			/* medium type */
    "",				/* session label (we're not doing this) */
    "",				/* kilo bytes requested  (not doing this) */
    NULL			/* no child film box UID known yet */
};


static FIELDwRBTN bfs[] = {
    {"Copies:", bfsAttrib.copies, copies_rbtn, DIM_OF(copies_rbtn)},
    {"Print Priority:", bfsAttrib.priority, print_pri_rbtn, DIM_OF(print_pri_rbtn)},
    {"Medium Type:", bfsAttrib.mediumType, medium_type_rbtn, DIM_OF(medium_type_rbtn)},
    {"Film Destination:", bfsAttrib.filmDestination, film_dest_rbtn, DIM_OF(film_dest_rbtn)}
};

static char numBFBC[10] = "1";

static FIELDwTEXT number_of_bfbs =
{"Number of Film Boxes:", numBFBC, sizeof(numBFBC),
TRUE, NULL};

static ENTRY basic_film_session[] = {
    {FLD_FIELDwRBTN, REQUIRED, &bfs[0]},
    {FLD_FIELDwRBTN, REQUIRED, &bfs[1]}
};

static ENTRY basic_film_session2[] = {
    {FLD_FIELDwRBTN, REQUIRED, &bfs[2]},
    {FLD_FIELDwRBTN, REQUIRED, &bfs[3]},
};

static ENTRY basic_film_session3[] = {
    {FLD_FIELDwTEXT, REQUIRED, (void *) &number_of_bfbs},
};


PRN_BASICFILMBOX bfbAttrib = {
    NULL, NULL,
    PRN_K_BASICFILMBOX,		/* type */
    0,				/* Attribute Flag */
    DICOM_SOPCLASSBASICFILMBOX,	/* SOP class UID */
    "",				/* SOP instance UID, unknown yet */
    DICOM_SOPCLASSBASICFILMSESSION,	/* SOP class UID of parent film
					 * session */
    "",				/* instance UID of parent unknown yet */
    "STANDARD\\2,2",		/* image display format */
    "",				/* annotation display */
    PORTRAIT,			/* film orientation */
    SIZE8INX10IN,		/* film size */
    "REPLICATE",		/* magnification type */
    "",				/* no smoothing type provided */
    BLACKBORDER,		/* border density */
    "",				/* no empty imaage density */
#if STANDARD_VERSION < VERSION_AUG1993
    "",				/* no minimum density provided */
    "",				/* no max density provided */
#else
    0,				/* no minimum density provided */
    0,				/* no max density provided */
#endif
    NO,				/* trim value */
    "",				/* no configuration info */
    NULL, NULL			/* List heads for basic image box */
    /* and annotation box sequences */
};
/*static char bfbstdC[30] = "STANDARD"; */
static char bfbstdCOL[2] = "2";
static char bfbstdROW[2] = "2";

static RBTNw2TEXTFIELD display_format_standard =
{"STANDARD", STANDARD, "COL", bfbstdCOL, sizeof(bfbstdCOL),
"ROW", bfbstdROW, sizeof(bfbstdROW), NULL, NULL};

/*static char bfbROW[20];*/
/*static char bfbCOL[20];*/

/*static RBTNwTEXTFIELD display_format_row_col[] = {
    {"ROW", NULL, ROW, bfbROW, sizeof(bfbROW), NULL},
    {"COL", NULL, COL, bfbCOL, sizeof(bfbCOL), NULL}
}; */

static ALLRBTN display_format_rbtn[] = {
    {BTN_RBTNw2TEXTFIELD, (void *) &display_format_standard, NULL},
/*
    {BTN_RBTNwTEXTFIELD, (void *) &display_format_row_col[0], NULL},
    {BTN_RBTNwTEXTFIELD, (void *) &display_format_row_col[1], NULL}
*/
};

static RBTNwLABEL orientation[] = {
    {"Portrait", PORTRAIT},
    {"Landscape", LANDSCAPE}
};

static ALLRBTN orientation_rbtn[] = {
    {BTN_RBTNwLABEL, (void *) &orientation[0], NULL},
    {BTN_RBTNwLABEL, (void *) &orientation[1], NULL}
};

static RBTNwLABEL film_size[] = {
    {"8in  x 10in", SIZE8INX10IN},
    {"10in x 12in", SIZE10INX12IN},
    {"10in x 14in", SIZE10INX14IN},
    {"11in x 14in", SIZE11INX14IN},
    {"14in x 14in", SIZE14INX14IN},
    {"14in x 17in", SIZE14INX17IN},
    {"24cm x 24cm", SIZE24CMX24CM},
    {"24cm x 30cm", SIZE24CMX30CM}
};

static ALLRBTN film_size_rbtn[] = {
    {BTN_RBTNwLABEL, (void *) &film_size[0], NULL},
    {BTN_RBTNwLABEL, (void *) &film_size[1], NULL},
    {BTN_RBTNwLABEL, (void *) &film_size[2], NULL},
    {BTN_RBTNwLABEL, (void *) &film_size[3], NULL},
    {BTN_RBTNwLABEL, (void *) &film_size[4], NULL},
    {BTN_RBTNwLABEL, (void *) &film_size[5], NULL},
    {BTN_RBTNwLABEL, (void *) &film_size[6], NULL},
    {BTN_RBTNwLABEL, (void *) &film_size[7], NULL}
};

static RBTNwLABEL magnification[] = {
    {"Replicate", REPLICATE},
    {"Bilinear", BILINEAR},
    {"Cubic", CUBIC}
};

static ALLRBTN magnification_rbtn[] = {
    {BTN_RBTNwLABEL, (void *) &magnification[0], NULL},
    {BTN_RBTNwLABEL, (void *) &magnification[1], NULL},
    {BTN_RBTNwLABEL, (void *) &magnification[2], NULL}
};

static RBTNwLABEL border_density_1[] = {
    {"Black", BLACKBORDER},
    {"White", WHITEBORDER}
};

#define OTHER "Other"

static char border_density_other[5];

static RBTNwTEXTFIELD border_density_2 =
{NULL, "(OD)", OTHER, border_density_other, sizeof(border_density_other),
NULL};

static ALLRBTN border_density_rbtn[] = {
    {BTN_RBTNwLABEL, (void *) &border_density_1[0], NULL},
    {BTN_RBTNwLABEL, (void *) &border_density_1[1], NULL},
    {BTN_RBTNwTEXTFIELD, (void *) &border_density_2, NULL}
};

static RBTNwLABEL empty_image_density_1[] = {
    {"Black", BLACK},
    {"White", WHITE}
};

static char empty_image_density_other[5];

static RBTNwTEXTFIELD empty_image_density_2 =
{NULL, "(OD)", OTHER, empty_image_density_other,
sizeof(empty_image_density_other), NULL};

static ALLRBTN empty_image_density_rbtn[] = {
    {BTN_RBTNwLABEL, (void *) &empty_image_density_1[0], NULL},
    {BTN_RBTNwLABEL, (void *) &empty_image_density_1[1], NULL},
    {BTN_RBTNwTEXTFIELD, (void *) &empty_image_density_2, NULL},
};

static RBTNwLABEL trim[] = {
    {"Yes", YES},
    {"No", NO}
};

static ALLRBTN trim_rbtn[] = {
    {BTN_RBTNwLABEL, (void *) &trim[0], NULL},
    {BTN_RBTNwLABEL, (void *) &trim[1], NULL}
};

static char displayFormatC[30];
static char borderDensityC[10];
static char emptyImageDensityC[10];

static FIELDwRBTN basic_film_box_flds[] = {
/*
    {"Display Format:", bfbAttrib.imageDisplayFormat,
*/
    {"Display Format:", displayFormatC,
    display_format_rbtn, DIM_OF(display_format_rbtn)},
    {"Orientation:", bfbAttrib.filmOrientation,
    orientation_rbtn, DIM_OF(orientation_rbtn)},
    {"Film Size:", bfbAttrib.filmSizeID,
    film_size_rbtn, DIM_OF(film_size_rbtn)},
    {"Magnification:", bfbAttrib.magnificationType,
    magnification_rbtn, DIM_OF(magnification_rbtn)},
/*
    {"Border Density:", bfbAttrib.borderDensity,
*/
    {"Border Density:", borderDensityC,
    border_density_rbtn, DIM_OF(border_density_rbtn)},
/*
    {"Empty Image Density:", bfbAttrib.emptyImageDensity,
*/
    {"Empty Image Density:", emptyImageDensityC,
    empty_image_density_rbtn, DIM_OF(empty_image_density_rbtn)},
    {"Trim:", bfbAttrib.trim, trim_rbtn, DIM_OF(trim_rbtn)}
};

static char minDensityC[10] = "0.1";
static char maxDensityC[10] = "3.5";

static FIELDwTEXT basic_film_box_txts[] = {
    {"Min Density:", minDensityC, sizeof(minDensityC),
    TRUE, NULL},
    {"Max Density:", maxDensityC, sizeof(maxDensityC),
    TRUE, NULL}
};

static ENTRY basic_film_box[] = {
    {FLD_FIELDwRBTN, REQUIRED, (void *) &basic_film_box_flds[0]},
    {FLD_FIELDwRBTN, REQUIRED, (void *) &basic_film_box_flds[1]},
    {FLD_FIELDwRBTN, REQUIRED, (void *) &basic_film_box_flds[2]}
};

static ENTRY basic_film_box2[] = {
    {FLD_FIELDwRBTN, REQUIRED, (void *) &basic_film_box_flds[3]},
    {FLD_FIELDwRBTN, REQUIRED, (void *) &basic_film_box_flds[4]},
    {FLD_FIELDwRBTN, REQUIRED, (void *) &basic_film_box_flds[5]},
    {FLD_FIELDwTEXT, REQUIRED, (void *) &basic_film_box_txts[0]},
    {FLD_FIELDwTEXT, REQUIRED, (void *) &basic_film_box_txts[1]},
    {FLD_FIELDwRBTN, REQUIRED, (void *) &basic_film_box_flds[6]}
};

char imagePositionC[64];

static FIELDwTEXT image_attributes_txts[] = {
    {"Image Position:", imagePositionC, sizeof(imagePositionC),
    FALSE, NULL},
/*
    {"Requested Image Size:", bibAttrib.requestedImageSize,
	sizeof(bibAttrib.requestedImageSize),
    TRUE, NULL}
*/
};

/*static ALLRBTN polarity_rbtn[] = {
    {BTN_RBTNwLABEL, (void *) &border_density_1[0], NULL},
    {BTN_RBTNwLABEL, (void *) &border_density_1[1], NULL},
    {BTN_RBTNwTEXTFIELD, (void *) &border_density_2, NULL}
};*/

static RBTNwLABEL polarity2[] = {
/*
    {"None", "None"},
*/
    {"Normal", NORMAL},
    {"Reverse", REVERSE}
};

static ALLRBTN polarity2_rbtn[] = {
    {BTN_RBTNwLABEL, (void *) &polarity2[0], NULL},
    {BTN_RBTNwLABEL, (void *) &polarity2[1], NULL},
/*
    {BTN_RBTNwLABEL, (void *) &polarity2[2], NULL}
*/
};

static RBTNwLABEL magnification_type[] = {
    {"Replicate", REPLICATE},
    {"Bilinear", BILINEAR},
    {"Cubic", CUBIC}
};

static ALLRBTN magnification_type_rbtn[] = {
    {BTN_RBTNwLABEL, (void *) &magnification_type[0], NULL},
    {BTN_RBTNwLABEL, (void *) &magnification_type[1], NULL},
    {BTN_RBTNwLABEL, (void *) &magnification_type[2], NULL}
};

static FIELDwRBTN image_attributes_fld[] = {
    {"Polarity:", bibAttrib.polarity, polarity2_rbtn, DIM_OF(polarity2_rbtn)},
    {"Magnification Type:", bibAttrib.magnificationType,
    magnification_type_rbtn, DIM_OF(magnification_type_rbtn)}
};

static ENTRY image_attributes[] = {
    {FLD_FIELDwTEXT, REQUIRED, (void *) &image_attributes_txts[0]},
    {FLD_FIELDwRBTN, REQUIRED, (void *) &image_attributes_fld[0]},
    {FLD_FIELDwRBTN, REQUIRED, (void *) &image_attributes_fld[1]},
/*
    {FLD_FIELDwTEXT, REQUIRED, (void *) &image_attributes_txts[1]}
*/
};
