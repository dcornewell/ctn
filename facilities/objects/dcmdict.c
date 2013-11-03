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
** Module Name(s):	DCM_LookupElement(DCM_ELEMENT *element)
** Author, Date:	Stephen M. Moore, 30-Apr-93
** Intent:		This module contains the routine and data which
**			define the DICOM data dictionary.  A number of
**			static objects are maintained which define how
**			elements in the DICOM V3.0 standard are to be
**			interpreted.
** Last Update:		$Author: smm $, $Date: 2008-03-02 03:48:36 $
** Source File:		$RCSfile: dcmdict.c,v $
** Revision:		$Revision: 1.104 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.104 $ $RCSfile: dcmdict.c,v $";

#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include "dicom.h"
#include "lst.h"
#include "condition.h"
#include "dicom_objects.h"
#include "dcmprivate.h"

/*  The DCM dictionary consists of a list of lists.  Each group (COMMAND,
**  IMAGE, ...) is defined in a list separately.  The object DCMDICT
**  below is used to define the entry for a single data element in a
**  known group.  We define the fields:
**	element
**	representation
**	english Description
**  The outer layer DCM dictionary consists of a list of groups.  The
**  group entries define the "group" number and give a pointer to the
**  DCMDICT list for that group.  The intent is to search the outer layer
**  dictionary to find the proper group, then search the particular group
**  list to find the proper element.
*/
typedef struct {
    DCM_TAG tag;
    DCM_VALUEREPRESENTATION representation;
    char englishDescription[48];
}   DCMDICT;

typedef struct {
    unsigned short group;
    unsigned long entries;
    DCMDICT *dict;
}   GROUPPTR;


/*  Define the entries for the COMMAND group
*/
static DCMDICT CMD_dictionary[] = {
    {DCM_CMDGROUPLENGTH, DCM_UL, "CMD Group Length"},
    {DCM_CMDAFFECTEDCLASSUID, DCM_UI, "CMD Affected SOP Class UID"},
    {DCM_CMDREQUESTEDCLASSUID, DCM_UI, "CMD Requested SOP Class UID"},
    {DCM_CMDCOMMANDFIELD, DCM_US, "CMD Command Field"},
    {DCM_CMDMSGID, DCM_US, "CMD Message ID"},
    {DCM_CMDMSGIDRESPOND, DCM_US, "CMD Message ID Responded to"},
    {DCM_CMDMOVEDESTINATION, DCM_AE, "CMD Move Destination"},
    {DCM_CMDPRIORITY, DCM_US, "CMD Priority"},
    {DCM_CMDDATASETTYPE, DCM_US, "CMD Data Set Type"},
    {DCM_CMDSTATUS, DCM_US, "CMD Status"},
    {DCM_CMDOFFENDINGELEMENT, DCM_AT, "CMD Offending Element"},
    {DCM_CMDERRORCOMMENT, DCM_LO, "CMD Error Comment"},
    {DCM_CMDERRORID, DCM_US, "CMD Error ID"},
    {DCM_CMDREQUESTEDINSTANCEUID, DCM_UI, "CMD SOP Requested Instance UID"},
    {DCM_CMDAFFECTEDINSTANCEUID, DCM_UI, "CMD SOP Affected Instance UID"},
    {DCM_CMDEVENTTYPEID, DCM_US, "CMD Event Type ID"},
    {DCM_CMDACTIONTYPEID, DCM_US, "CMD Action Type ID"},
    {DCM_CMDREMAININGSUBOPERATIONS, DCM_US, "CMD Remaining Suboperations"},
    {DCM_CMDCOMPLETEDSUBOPERATIONS, DCM_US, "CMD Completed Suboperations"},
    {DCM_CMDFAILEDSUBOPERATIONS, DCM_US, "CMD Failed Suboperations"},
    {DCM_CMDWARNINGSUBOPERATIONS, DCM_US, "CMD Warning Suboperations"},
    {DCM_CMDMOVEAETITLE, DCM_AE, "CMD AE Title"},
    {DCM_CMDMOVEMESSAGEID, DCM_US, "CMD Message ID"},
    {DCM_CMDATTRIBUTEIDLIST, DCM_AT, "CMD Attribute Identifier List"},

};

/* Define the entries for the file Meta Header group
*/

static DCMDICT META_dictionary[] = {
    {DCM_METAGROUPLENGTH, DCM_UL, "META Group Length"},
    {DCM_METAINFORMATIONVERSION, DCM_OB, "META File Meta Information Version"},
    {DCM_METAMEDIASTORAGESOPCLASS, DCM_UI, "META Media Stored SOP Class UID"},
    {DCM_METAMEDIASTORAGESOPINSTANCE, DCM_UI, "META Media Stored SOP Instance UID"},
    {DCM_METATRANSFERSYNTAX, DCM_UI, "META Transfer Syntax UID"},
    {DCM_METAIMPLEMENTATIONCLASS, DCM_UI, "META Implementation Class UID"},
    {DCM_METAIMPLEMENTATIONVERSION, DCM_SH, "META Implementation Version Name"},
    {DCM_METASOURCEAETITLE, DCM_AE, "META Source Application Entity Title"},
    {DCM_METAPRIVATEINFORMATIONCREATOR, DCM_UI, "META Private Information Creator"},
    {DCM_METAPRIVATEINFORMATION, DCM_OB, "META Private Information"}
};

/* Define the elements in the Basic Directory Information Group, 0x0004 */

static DCMDICT BASICDIR_dictionary[] = {
    {DCM_DIRFILESETID, DCM_CS, "DIR File-set ID"},
    {DCM_DIRFILESETDESCRFILEID, DCM_CS, "DIR File-set descriptor ID"},
    {DCM_DIRSPECIFICCHARACTER, DCM_CS, "DIR Specific character set"},
    {DCM_DIRFIRSTOFFSET, DCM_UL, "DIR Offset of the first dir of root dir entity"},
    {DCM_DIRLASTOFFSET, DCM_UL, "DIR Offset of the last dir of root dir entity"},
    {DCM_DIRFILESETCONSISTENCY, DCM_US, "DIR File-set consistency flag"},
    {DCM_DIRRECORDSEQUENCE, DCM_SQ, "DIR Directory record sequence"},
    {DCM_DIRNEXTRECORDOFFSET, DCM_UL, "DIR Offset of next directory record"},
    {DCM_DIRRECORDINUSE, DCM_US, "DIR Record in use flag"},
    {DCM_DIRLOWERLEVELOFFSET, DCM_UL, "DIR Offset of referenced lower-level dir entity"},
    {DCM_DIRRECORDTYPE, DCM_CS, "DIR Directory Record Type"},
    {DCM_DIRPRIVATERECORDUID, DCM_UI, "DIR Private Record UID"},
    {DCM_DIRREFERENCEDFILEID, DCM_CS, "DIR Referenced File ID"},
    {DCM_DIRMRDRRECORDOFFSET, DCM_UL, "DIR Directory Record Offset"},
    {DCM_DIRREFSOPCLASSUID, DCM_UI, "DIR Referenced SOP Class UID in File"},
    {DCM_DIRREFSOPINSTANCEUID, DCM_UI, "DIR Referenced SOP Instance UID in File"},
    {DCM_DIRREFTRANSFERSYNTAXUID, DCM_UI, "DIR Referenced Transfer Syntax in File"},
    {DCM_DIRNUMREFERENCES, DCM_UL, "DIR Number of References"}
};

/* Define the entries for the IDENTIFYING group, 0008
*/

/* Joe, start here */

static DCMDICT ID_dictionary[] = {
    /* 0000 0000 */
    {DCM_IDGROUPLENGTH, DCM_UL, "ID Group Length"},
/*    {DCM_IDLENGTHTOEND, DCM_RET, "ID Length to End (RET)"}, */
    /* 0008 0001 */
    {DCM_IDLENGTHTOEND, DCM_UL, "ID Length to End (RET)"},
    /* 0008 0005 */
    {DCM_IDSPECIFICCHARACTER, DCM_CS, "ID Specific Character Set"},
    /* 0008 0005 */
    {DCM_IDIMAGETYPE, DCM_CS, "ID Image Type"},
    /* 0008 0010 */
    {DCM_IDRECOGNITIONCODE, DCM_CS, "ID Recognition Code (RET)"},
    /* 0008 0012 */
    {DCM_IDINSTANCECREATEDATE, DCM_DA, "ID Instance Creation Date"},
    /* 0008 0013 */
    {DCM_IDINSTANCECREATETIME, DCM_TM, "ID Instance Creation Time"},
    /* 0008 0014 */
    {DCM_IDINSTANCECREATORUID, DCM_UI, "ID Instance Creator UID"},
    {DCM_MAKETAG(0x0008, 0x001A), DCM_UI, "ID Related General SOP Class UID"},
    {DCM_MAKETAG(0x0008, 0x001B), DCM_UI, "ID Original Specialized SOP Class UID"},
    /* 0008 0016 */
    {DCM_IDSOPCLASSUID, DCM_UI, "ID SOP Class UID"},
    /* 0008 0018 */
    {DCM_IDSOPINSTANCEUID, DCM_UI, "ID SOP Instance UID"},
    /* 0008 0020 */
    {DCM_IDSTUDYDATE, DCM_DA, "ID Study Date"},
    /* 0008 0021 */
    {DCM_IDSERIESDATE, DCM_DA, "ID Series Date"},
    /* 0008 0022 */
    {DCM_IDACQUISITIONDATE, DCM_DA, "ID Acquisition Date"},
    {DCM_IDIMAGEDATE, DCM_DA, "ID Content Date"},
    /* 0008 0024 */
    {DCM_IDOVERLAYDATE, DCM_DA, "ID Overlay Date (RET)"},
    /* 0008 0025 */
    {DCM_IDCURVEDATE, DCM_DA, "ID Curve Date (RET)"},
    /* 0008 002A */
    { DCM_MAKETAG(0x0008, 0x002a), DCM_DT, "ID Acquisition Datetime"}, /* 2005.08.26 */
    /* 0008 0030 */
    {DCM_IDSTUDYTIME, DCM_TM, "ID Study Time"},
    {DCM_MAKETAG(0x0008, 0x0031), DCM_TM, "ID Series Time"},
    {DCM_MAKETAG(0x0008, 0x0032), DCM_TM, "ID Acquisition Time"},
    {DCM_MAKETAG(0x0008, 0x0033), DCM_TM, "ID Content Time"},
    {DCM_MAKETAG(0x0008, 0x0034), DCM_TM, "ID Overlay Time (RET)"},
    {DCM_MAKETAG(0x0008, 0x0035), DCM_TM, "ID Curve Time (RET)"},
    {DCM_MAKETAG(0x0008, 0x0040), DCM_US, "ID Data Set Type (RET)"},
    {DCM_MAKETAG(0x0008, 0x0041), DCM_LO, "ID Data Set Subtype (RET)"},
    {DCM_MAKETAG(0x0008, 0x0042), DCM_CS, "ID Nuc Med Series Type (RET)"},
    {DCM_MAKETAG(0x0008, 0x0050), DCM_SH, "ID Accession Number"},
	{DCM_MAKETAG(0x0008, 0x0051), DCM_SQ, "ID Issuer of Accession Number Sequence"},
    {DCM_MAKETAG(0x0008, 0x0052), DCM_CS, "ID Query/Retrieve Level"},
    {DCM_MAKETAG(0x0008, 0x0054), DCM_AE, "ID Retrieve AE Title"},
    /* 0008 0056 */
    {DCM_IDINSTANCEAVAILABILITY, DCM_CS, "ID Instance Availability"},
    /* 0008 0058 */
    {DCM_IDFAILEDINSTANCEUIDLIST, DCM_UI, "ID Failed SOP Instances"},
    {DCM_MAKETAG(0x0008, 0x0060), DCM_CS, "ID Modality"},
    {DCM_MAKETAG(0x0008, 0x0061), DCM_CS, "ID Modalities in Study"},
    {DCM_MAKETAG(0x0008, 0x0062), DCM_UI, "SOP Classes in Study"},
    {DCM_MAKETAG(0x0008, 0x0064), DCM_CS, "Conversion Type"},
    /*{DCM_IDMODALITYSUBTYPE, DCM_SQ, "ID Modality Subtype"},*/	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x0008, 0x0068), DCM_CS, "ID Presentation Intent Type"},
    {DCM_MAKETAG(0x0008, 0x0070), DCM_LO, "ID Manufacturer"},
    {DCM_MAKETAG(0x0008, 0x0080), DCM_LO, "ID Institution Name"},
    /* 0008 0081 */
    {DCM_IDINSTITUTIONADDR, DCM_ST, "ID Institution Address"},
    /* 0008 0082 */
    {DCM_IDINSTITUTECODESEQUENCE, DCM_SQ, "ID Institution Code Sequence"},
    /* 0008 0090 */
    {DCM_IDREFERRINGPHYSICIAN, DCM_PN, "ID Referring Physician's Name"},
    /* 0008 0092 */
    {DCM_IDREFERRINGPHYSADDR, DCM_ST, "ID Referring Physician's Address"},
    /* 0008 0094 */
    {DCM_IDREFERRINGPHYSPHONE, DCM_SH, "ID Referring Physician's Telephone"},
    {DCM_MAKETAG(0x0008, 0x0096), DCM_SQ, "ID Referring Physician Identification Seq"},
    {DCM_MAKETAG(0x0008, 0x0100), DCM_SH, "ID Code Value"},
    {DCM_MAKETAG(0x0008, 0x0102), DCM_SH, "ID Coding Scheme Designator"},
    {DCM_MAKETAG(0x0008, 0x0103), DCM_SH, "ID Coding Scheme Version"},
    /* Sup 15, Version 1.2_interim_971226 */
    {DCM_MAKETAG(0x0008, 0x0104), DCM_LO, "ID Code Meaning"},
    {DCM_MAKETAG(0x0008, 0x0105), DCM_CS, "ID Mapping Resource"},	/* Sup 15, 1.1a */
    {DCM_MAKETAG(0x0008, 0x0106), DCM_DT, "ID Context Group Version"},
    /* Sup 15, Version 1.1a */
    {DCM_MAKETAG(0x0008, 0x0107), DCM_DT, "ID Context Group Local Version"},
    {DCM_MAKETAG(0x0008, 0x010B), DCM_CS, "ID Context Group Extension Flag"},
    {DCM_MAKETAG(0x0008, 0x010C), DCM_UI, "ID Coding Scheme UID"}, /* 0x010C: Sup 15, Version 1.1 */
    {DCM_MAKETAG(0x0008, 0x010D), DCM_UI, "ID Coding Group Extension Creator UID"}, /* 0x010D: Sup 15, Version 1.1 */
    {DCM_MAKETAG(0x0008, 0x010F), DCM_CS, "ID Context Identifier"},	/* Sup 15 */
    {DCM_MAKETAG(0x0008, 0x0110), DCM_SQ, "ID Coding Scheme Identification Sequence"},
    {DCM_MAKETAG(0x0008, 0x0112), DCM_LO, "ID Coding Scheme Registry"},
    {DCM_MAKETAG(0x0008, 0x0114), DCM_ST, "ID Coding Scheme External ID"},
    {DCM_MAKETAG(0x0008, 0x0115), DCM_ST, "ID Coding Scheme Name"},
    {DCM_MAKETAG(0x0008, 0x0116), DCM_ST, "ID Coding Scheme Responsible Organization"},
    {DCM_MAKETAG(0x0008, 0x0201), DCM_SH, "ID Timezone Offset From UTC"},
    {DCM_MAKETAG(0x0008, 0x1000), DCM_AE, "ID Network ID (RET)"},
    {DCM_MAKETAG(0x0008, 0x1010), DCM_SH, "ID Station Name"},
    {DCM_MAKETAG(0x0008, 0x1030), DCM_LO, "ID Study Description"},
    {DCM_MAKETAG(0x0008, 0x1032), DCM_SQ, "ID Procedure Code Sequence"},
    {DCM_MAKETAG(0x0008, 0x103E), DCM_LO, "ID Series Description"},
    {DCM_MAKETAG(0x0008, 0x1040), DCM_LO, "ID Institutional Department Name"},
    {DCM_MAKETAG(0x0008, 0x1048), DCM_PN, "ID Physician(s) of Record"},
    {DCM_MAKETAG(0x0008, 0x1049), DCM_SQ, "ID Physician(s) of Record ID Squence"},
    {DCM_MAKETAG(0x0008, 0x1050), DCM_PN, "ID Performing Physician's Name"},
    {DCM_MAKETAG(0x0008, 0x1052), DCM_SQ, "ID Performing Physician Identification Seq"},
    {DCM_MAKETAG(0x0008, 0x1060), DCM_PN, "ID Name of Physician(s) Reading Study"},
    {DCM_MAKETAG(0x0008, 0x1062), DCM_SQ, "ID Physician(s) Reading Study Ident Seq"},
    {DCM_MAKETAG(0x0008, 0x1070), DCM_PN, "ID Operator's Name"},
    {DCM_MAKETAG(0x0008, 0x1072), DCM_SQ, "ID Operator Identification Seq"},
    {DCM_MAKETAG(0x0008, 0x1080), DCM_LO, "ID Admitting Diagnoses Description"},
    {DCM_MAKETAG(0x0008, 0x1084), DCM_SQ, "ID Admitting Diagnoses Code Sequence"},
    {DCM_MAKETAG(0x0008, 0x1090), DCM_LO, "ID Manufacturer Model Name"},
    {DCM_MAKETAG(0x0008, 0x1100), DCM_SQ, "ID Referenced Results Sequence (RET)"},
    {DCM_MAKETAG(0x0008, 0x1110), DCM_SQ, "ID Referenced Study Sequence"},
    {DCM_MAKETAG(0x0008, 0x1111), DCM_SQ, "ID Referenced Performed Procedure Step Seq"},
    {DCM_MAKETAG(0x0008, 0x1115), DCM_SQ, "ID Referenced Series Sequence"},
    {DCM_MAKETAG(0x0008, 0x1120), DCM_SQ, "ID Referenced Patient Sequence"},
    {DCM_MAKETAG(0x0008, 0x1125), DCM_SQ, "ID Referenced Visit Sequence"},
    {DCM_MAKETAG(0x0008, 0x1130), DCM_SQ, "ID Referenced Overlay Sequence (RET)"},
    {DCM_MAKETAG(0x0008, 0x113A), DCM_SQ, "ID Referenced Waveform Sequence"},
    {DCM_MAKETAG(0x0008, 0x1140), DCM_SQ, "ID Referenced Image Sequence"},
    {DCM_MAKETAG(0x0008, 0x1145), DCM_SQ, "ID Referenced Curve Sequence (RET)"},
    {DCM_MAKETAG(0x0008, 0x114A), DCM_SQ, "ID Referenced Instance Sequence"},
    {DCM_MAKETAG(0x0008, 0x114B), DCM_SQ, "ID Ref'd Real World Value Mapping Instance Seq"},
/*    {DCM_IDREFERENCEDPREVIOUSWAVEFORM, DCM_SQ, "ID Referenced Previous Waveform"},*/	/* Sup 30 0.6 */
/*    {DCM_IDREFERENCEDSIMULTANEOUSWAVEFORMS, DCM_SQ, "ID Referenced Simultaneous Waveforms"},*/	/* Sup 30 0.6 */
/*    {DCM_IDREFERENCEDSUBSEQUENTWAVEFORM, DCM_SQ, "ID Referenced Subsequent Waveform"},*/	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x0008, 0x1150), DCM_UI, "ID Referenced SOP Class UID"},
    {DCM_MAKETAG(0x0008, 0x1155), DCM_UI, "ID Referenced SOP Instance UID"},
    {DCM_MAKETAG(0x0008, 0x115A), DCM_UI, "ID SOP Classes Supported"},
    {DCM_MAKETAG(0x0008, 0x1160), DCM_IS, "ID Referenced Frame Number"},
    {DCM_MAKETAG(0x0008, 0x1195), DCM_UI, "ID Transaction UID"},
    /* 0008 1197 */
    {DCM_IDFAILUREREASON, DCM_US, "ID Failure Reason"},
    /* 0008 1198 */
    {DCM_IDFAILEDSOPSEQUENCE, DCM_SQ, "ID Failed SOP Sequence"},
    {DCM_MAKETAG(0x0008, 0x1199), DCM_SQ, "ID Referenced SOP Sequence"},
    {DCM_MAKETAG(0x0008, 0x1200), DCM_SQ, "ID Studies Containing Other Ref'd Instances Seq"},
    {DCM_MAKETAG(0x0008, 0x1250), DCM_SQ, "ID Related Series Sequence"},
    {DCM_MAKETAG(0x0008, 0x2110), DCM_CS, "ID Lossy Image Compression (RET)"},
    /* 0008 2111 */
    {DCM_IDDERIVATIONDESCR, DCM_ST, "ID Derivation Description"},
    /* 0008 2112 */
    {DCM_IDSOURCEIMAGESEQ, DCM_SQ, "ID Source Image Sequence"},
    /* 0008 2120 */
    {DCM_IDSTAGENAME, DCM_SH, "ID Stage Name"},
    /* 0008 2122 */
    {DCM_IDSTAGENUMBER, DCM_IS, "ID Stage Number"},
    /* 0008 2124 */
    {DCM_IDNUMBEROFSTAGES, DCM_IS, "ID Number of Stages"},
    {DCM_MAKETAG(0x0008, 0x2127), DCM_SH, "ID View Name"},
    /* 0008 2128 */
    {DCM_IDVIEWNUMBER, DCM_IS, "ID View Number"},
    /* 0008 2129 */
    {DCM_IDNUMBEROFEVENTTIMERS, DCM_IS, "ID Number of Event Timers"},
    /* 0008 212A */
    {DCM_IDNUMBERVIEWSINSTAGE, DCM_IS, "ID Number of Views in Stage"},
    /* 0008 2130 */
    {DCM_IDEVENTELAPSEDTIME, DCM_DS, "ID Event Elapsed Time(s)"},
    /* 0008 2132 */
    {DCM_IDEVENTTIMERNAME, DCM_LO, "ID Event Timer Name(s)"},
    /* 0008 2142 */
    {DCM_IDSTARTTRIM, DCM_IS, "ID Start Trim"},
    /* 0008 2143 */
    {DCM_IDSTOPTRIM, DCM_IS, "ID Stop Trim"},
    /* 0008 2144 */
    {DCM_IDDISPLAYFRAMERATE, DCM_IS, "ID Recommended Display Frame Rate"},
    /* 0008 2200 */
    {DCM_IDTRANSDUCERPOSITION, DCM_CS, "ID Transducer Position (RET)"},
    /* 0008 2204 */
    {DCM_IDTRANSDUCERORIENTATION, DCM_CS, "ID Transducer Orientation (RET)"},
    /* 0008 2208 */
    {DCM_IDANATOMICSTRUCTURE, DCM_CS, "ID Anatomic Structure (RET)"},
    /* 0008 2218 */
    {DCM_IDANATOMICREGIONSEQUENCE, DCM_SQ, "ID Anatomic Region Sequence"},
    /* 0008 2200 */
    {DCM_IDANATOMICREGIONMODIFIERSEQ, DCM_SQ, "ID Anatomic Region Modifier Sequence"},
    {DCM_MAKETAG(0x0008, 0x2228), DCM_SQ, "ID Primary Anatomic Structure Sequence"},
    {DCM_MAKETAG(0x0008, 0x2229), DCM_SQ, "ID Anatomic Structure, Space or Region Sequence"},
    {DCM_MAKETAG(0x0008, 0x2230), DCM_SQ, "ID Primary Anatomic Structure Modifier Sequence"},
    {DCM_MAKETAG(0x0008, 0x2240), DCM_SQ, "ID Transducer Position Sequence (RET)"},
    {DCM_MAKETAG(0x0008, 0x2242), DCM_SQ, "ID Transducer Position Modifer Sequence (RET)"},
    /* 0008 2244 */
    {DCM_IDTRANSDUCERORIENTATIONSEQ, DCM_SQ, "ID Transducer Orientation Sequence (RET)"},
    /* 0008 2246 */
    {DCM_IDTRANSDUCERORIENTATIONMODIFIERSEQ, DCM_SQ, "ID Transducer Orientation Modifer Seq (RET)"},
    {DCM_MAKETAG(0x0008, 0x2251), DCM_SQ, "ID Anatomic Struct Space/Region Code Seq (RET)"},
    {DCM_MAKETAG(0x0008, 0x2253), DCM_SQ, "ID Anatomic Portal of Entrance Code Seq (RET)"},
    {DCM_MAKETAG(0x0008, 0x2255), DCM_SQ, "ID Anatomic Approach Direction Code Seq (RET)"},
    {DCM_MAKETAG(0x0008, 0x2256), DCM_ST, "ID Anatomic Perspective Description (RET)"},
    {DCM_MAKETAG(0x0008, 0x2257), DCM_SQ, "ID Anatomic Perspective Code Sequence (RET)"},
    {DCM_MAKETAG(0x0008, 0x2258), DCM_ST, "ID Anatomic Loc Examining Instrument Desc (RET)"},
    {DCM_MAKETAG(0x0008, 0x2259), DCM_SQ, "ID Anatomic Loc Examining Instru Code Seq (RET)"},
    {DCM_MAKETAG(0x0008, 0x225A), DCM_SQ, "ID Anatom Struct Space/Region Mod Cod Seq (RET)"},
    {DCM_MAKETAG(0x0008, 0x225C), DCM_SQ, "ID OnAxis Bkgrnd Anatomic Struct Code Seq (RET)"},
    {DCM_MAKETAG(0x0008, 0x3001), DCM_SQ, "ID Alternate Representation Sequence"},
    {DCM_MAKETAG(0x0008, 0x3010), DCM_UI, "ID Irradiation Event UID"},
    {DCM_MAKETAG(0x0008, 0x4000), DCM_LT, "ID Identifying Comments (RET)"},
    { DCM_MAKETAG(0x0008, 0x9007), DCM_CS, "ID Frame Type"}, /* 2005.05.17 */
    { DCM_MAKETAG(0x0008, 0x9092), DCM_SQ, "ID Referenced Image Evidence Sequence"}, /* 2005.05.17 */
    { DCM_MAKETAG(0x0008, 0x9121), DCM_SQ, "ID Referenced Raw Data Sequence"}, /* 2005.05.17 */
    { DCM_MAKETAG(0x0008, 0x9123), DCM_UI, "ID Creator-Version UID"}, /* 2005.05.17 */
    { DCM_MAKETAG(0x0008, 0x9124), DCM_SQ, "ID Derivation Image Sequence"}, /* 2005.05.17 */
    { DCM_MAKETAG(0x0008, 0x9154), DCM_SQ, "ID Source Image Evidence Sequence"}, /* 2005.05.17 */
    { DCM_MAKETAG(0x0008, 0x9205), DCM_CS, "ID Pixel Presentation"}, /* 2005.05.17 */
    { DCM_MAKETAG(0x0008, 0x9206), DCM_CS, "ID Volumetric Properties"}, /* 2005.05.17 */
    { DCM_MAKETAG(0x0008, 0x9207), DCM_CS, "ID Volume Based Calculation Technique"}, /* 2005.05.17 */
    { DCM_MAKETAG(0x0008, 0x9208), DCM_CS, "ID Complex Image Component"}, /* 2005.05.17 */
    { DCM_MAKETAG(0x0008, 0x9209), DCM_CS, "ID Acquisition Contrast"}, /* 2005.05.17 */
    { DCM_MAKETAG(0x0008, 0x9215), DCM_SQ, "ID Derivation Code Sequence"},
    { DCM_MAKETAG(0x0008, 0x9237), DCM_SQ, "ID Referenced Grayscale Presentation State Seq"},
    { DCM_MAKETAG(0x0008, 0x9410), DCM_SQ, "ID Referenced Other Plane Sequence"},
    { DCM_MAKETAG(0x0008, 0x9458), DCM_SQ, "ID Frame Display Sequence"},
    { DCM_MAKETAG(0x0008, 0x9459), DCM_FL, "ID Recommended Display Frame Rate in Float"},
    { DCM_MAKETAG(0x0008, 0x9460), DCM_CS, "ID Skip Frame Range Flag"},
};

/* Define the entries for the PATIENT INFORMATION group 0010
*/
static DCMDICT PAT_dictionary[] = {
    {DCM_PATGROUPLENGTH, DCM_UL, "PAT Group Length"},
    {DCM_PATNAME, DCM_PN, "PAT Patient Name"},
    {DCM_PATID, DCM_LO, "PAT Patient ID"},
    {DCM_ISSUERPATIENTID, DCM_LO, "PAT Issuer of Patient ID"},
    {DCM_MAKETAG(0x0010, 0x0022), DCM_CS, "PAT Type of Patient ID"},
    {DCM_PATBIRTHDATE, DCM_DA, "PAT Patient Birthdate"},
    {DCM_PATBIRTHTIME, DCM_TM, "PAT Patient Birth Time"},
    {DCM_PATSEX, DCM_CS, "PAT Patient Sex"},
    {DCM_PATINSURANCEPLANCODESEQ, DCM_SQ, "PAT Patient's Insurance Plan Code Sequence"},
    {DCM_MAKETAG(0x0010, 0x0101), DCM_SQ, "PAT Patient's Primary Language Code Seq"},
    {DCM_MAKETAG(0x0010, 0x0102), DCM_SQ, "PAT Patient's Primary Language Code Mod Seq"},
    {DCM_MAKETAG(0x0010, 0x1000), DCM_LO, "PAT Other Patient IDs"},
    {DCM_MAKETAG(0x0010, 0x1001), DCM_PN, "PAT Other Patient Names"},
    {DCM_MAKETAG(0x0010, 0x1002), DCM_SQ, "PAT Other Patient IDs Sequence"},
    {DCM_PATBIRTHNAME, DCM_PN, "PAT Patient's Birth Name "},
    {DCM_PATAGE, DCM_AS, "PAT Patient Age"},
    {DCM_PATSIZE, DCM_DS, "PAT Patient Size"},
    {DCM_PATWEIGHT, DCM_DS, "PAT Patient Weight"},
    {DCM_PATADDRESS, DCM_LO, "PAT Patient Address"},
    {DCM_MAKETAG(0x0010, 0x1050), DCM_LO, "PAT Insurance Plan Identifier"},
    {DCM_MAKETAG(0x0010, 0x1060), DCM_PN, "PAT Patient's Mother's Birth Name"},
    {DCM_MAKETAG(0x0010, 0x1080), DCM_LO, "PAT Military Rank"},
    {DCM_MAKETAG(0x0010, 0x1081), DCM_LO, "PAT Branch of Service"},
    {DCM_MAKETAG(0x0010, 0x1090), DCM_LO, "PAT Medical Record Locator"},
    {DCM_MAKETAG(0x0010, 0x2000), DCM_LO, "PAT Medical Alerts"},
    {DCM_MAKETAG(0x0010, 0x2110), DCM_LO, "PAT Allergies"},
    {DCM_MAKETAG(0x0010, 0x2150), DCM_LO, "PAT Country of Residence"},
    {DCM_MAKETAG(0x0010, 0x2152), DCM_LO, "PAT Region of Residence"},
    {DCM_MAKETAG(0x0010, 0x2154), DCM_SH, "PAT Patient's Telephone Numbers"},
    {DCM_MAKETAG(0x0010, 0x2160), DCM_SH, "PAT Ethnic Group"},
    {DCM_MAKETAG(0x0010, 0x2180), DCM_SH, "PAT Occupation"},
    {DCM_MAKETAG(0x0010, 0x21A0), DCM_CS, "PAT Smoking Status"},
    {DCM_MAKETAG(0x0010, 0x21B0), DCM_LT, "PAT Additional Patient History"},
    {DCM_MAKETAG(0x0010, 0x21C0), DCM_US, "PAT Pregnancy Status"},
    {DCM_MAKETAG(0x0010, 0x21D0), DCM_DA, "PAT Last Menstrual Date"},
    {DCM_MAKETAG(0x0010, 0x21F0), DCM_LO, "PAT Patient's Religious Preference"},
    {DCM_MAKETAG(0x0010, 0x2201), DCM_LO, "PAT Patient Species Description"},
    {DCM_MAKETAG(0x0010, 0x2202), DCM_SQ, "PAT Patient Species Code Sequence"},
    {DCM_MAKETAG(0x0010, 0x2203), DCM_CS, "PAT Patient's Sex Neutered"},
    {DCM_MAKETAG(0x0010, 0x2292), DCM_LO, "PAT Patient Breed Description"},
    {DCM_MAKETAG(0x0010, 0x2293), DCM_SQ, "PAT Patient Breed Code Sequence"},
    {DCM_MAKETAG(0x0010, 0x2294), DCM_SQ, "PAT Breed Registration Sequence"},
    {DCM_MAKETAG(0x0010, 0x2295), DCM_LO, "PAT Breed Registration Number"},
    {DCM_MAKETAG(0x0010, 0x2296), DCM_SQ, "PAT Breed Registry Code Sequence"},
    {DCM_MAKETAG(0x0010, 0x2297), DCM_PN, "PAT Responsible Person"},
    {DCM_MAKETAG(0x0010, 0x2298), DCM_CS, "PAT Responsible Person Role"},
    {DCM_MAKETAG(0x0010, 0x2299), DCM_LO, "PAT Responsible Organization"},
    {DCM_MAKETAG(0x0010, 0x4000), DCM_LT, "PAT Comments"}
};

/* Define the entries for the CLINICAL TRIALS group 0012
*/
static DCMDICT G0012_dictionary[] = {
    {DCM_MAKETAG(0x0012, 0x0000), DCM_UL, "CT Group Length"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0012, 0x0010), DCM_LO, "CT Clinical Trial Sponsor Name"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0012, 0x0020), DCM_LO, "CT Clinical Trial Protocol ID"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0012, 0x0021), DCM_LO, "CT Clinical Trial Protocol Name"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0012, 0x0030), DCM_LO, "CT Clinical Trial Site ID"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0012, 0x0031), DCM_LO, "CT Clinical Trial Site Name"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0012, 0x0040), DCM_LO, "CT Clinical Trial Subject ID"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0012, 0x0042), DCM_LO, "CT Clinical Trial Subject Reading ID"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0012, 0x0050), DCM_LO, "CT Clinical Trial Time Point ID"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0012, 0x0051), DCM_ST, "CT Clinical Trail Time Point Description"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0012, 0x0060), DCM_LO, "CT Clinical Trial Coordinating Center Name"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0012, 0x0062), DCM_CS, "CT Patient Identity Removed"},
    {DCM_MAKETAG(0x0012, 0x0063), DCM_LO, "CT De-identification Method"},
    {DCM_MAKETAG(0x0012, 0x0064), DCM_SQ, "CT De-identification Method Code Sequence"},
    {DCM_MAKETAG(0x0012, 0x0071), DCM_LO, "CT Clinical Trial Series ID"},
    {DCM_MAKETAG(0x0012, 0x0072), DCM_LO, "CT Clinical Trail Series Description"},
};
/* Define the entries for the ACQUISITION group, 0018
*/

static DCMDICT ACQ_dictionary[] = {
    {DCM_ACQGROUPLENGTH, DCM_UL, "ACQ Group Length"},
    {DCM_ACQCONTRASTBOLUSAGENT, DCM_LO, "ACQ Contrast/Bolus Agent"},
    {DCM_ACQCONTRASTBOLUSAGENTSEQ, DCM_SQ, "ACQ Contrast/Bolus Agent Sequence"},
    {DCM_ACQCONTRASTBOLUSADMINROUTESEQ, DCM_SQ, "ACQ Contrast/Bolus Administration Route Seq"},
    {DCM_ACQBODYPARTEXAMINED, DCM_CS, "ACQ Body Part Examined"},
    {DCM_ACQSCANNINGSEQUENCE, DCM_CS, "ACQ Scanning Sequence"},
    {DCM_ACQSEQUENCEVARIANT, DCM_CS, "ACQ Sequence Variant"},
    {DCM_ACQSCANOPTIONS, DCM_CS, "ACQ Scan Options"},
    {DCM_ACQMRACQUISITIONTYPE, DCM_CS, "ACQ MR Acquisition Type "},
    {DCM_ACQSEQUENCENAME, DCM_SH, "ACQ Sequence Name"},
    {DCM_ACQANGIOFLAG, DCM_CS, "ACQ Angio Flag"},
    {DCM_ACQINTERVENTIONDRUGINFOSEQ, DCM_SQ, "ACQ Intervention Drug Information Sequence"},
    {DCM_ACQINTERVENTIONDRUGSTOPTIME, DCM_TM, "ACQ Intervention Drug Stop Time"},
    {DCM_ACQINTERVENTIONDRUGDOSE, DCM_DS, "ACQ Intervention Drug Dose"},
    {DCM_ACQINTERVENTIONDRUGCODESEQ, DCM_SQ, "ACQ Intervention Drug Code Sequence"},
    {DCM_ACQADDITIONALDRUGSEQ, DCM_SQ, "ACQ Additional Drug Sequence"},
    {DCM_ACQRADIONUCLIDE, DCM_LO, "ACQ Radionuclide (RET)"},
    {DCM_ACQRADIOPHARMACEUTICAL, DCM_LO, "ACQ Radiopharmaceutical"},
    {DCM_ACQENERGYWCENTERLINE, DCM_DS, "ACQ Energy Window Centerline (RET)"},
    {DCM_ACQENERGYWTOTALWIDTH, DCM_DS, "ACQ Energy Window Total Width (RET)"},
    {DCM_ACQINTERVENTIONDRUGNAME, DCM_LO, "ACQ Intervention Drug Name"},
    {DCM_ACQINTERVENTIONDRUGSTART, DCM_TM, "ACQ Intervention Drug Start Time"},
    {DCM_ACQINTERVENTIONALTHERAPYSEQ, DCM_SQ, "ACQ Interventional Therapy Sequence"},
    {DCM_MAKETAG(0x0018, 0x0037), DCM_CS, "ACQ Therapy type (RET)"},
    {DCM_MAKETAG(0x0018, 0x0038), DCM_CS, "ACQ Interventional status"},
    {DCM_MAKETAG(0x0018, 0x0039), DCM_CS, "ACQ Therapy description (RET)"},
    {DCM_MAKETAG(0x0018, 0x003A), DCM_ST, "ACQ Intervention Description"},
    {DCM_MAKETAG(0x0018, 0x0040), DCM_IS, "ACQ Cine Rate"},
    {DCM_MAKETAG(0x0018, 0x0050), DCM_DS, "ACQ Slice Thickness"},
    {DCM_MAKETAG(0x0018, 0x0060), DCM_DS, "ACQ KVP"},
    {DCM_ACQCOUNTSACCUMULATED, DCM_IS, "ACQ Counts Accumulated"},
    {DCM_ACQTERMINATIONCONDITION, DCM_CS, "ACQ Acquisition Termination Condition"},
    {DCM_ACQEFFECTIVESERIESDURATION, DCM_DS, "ACQ Effective Series Duration"},
    {DCM_ACQSTARTCONDITION, DCM_CS, "ACQ Start Condition"},
    {DCM_ACQSTARTCONDITIONDATA, DCM_IS, "ACQ Start Condition Data"},
    {DCM_ACQTERMINATIONCONDITIONDATA, DCM_IS, "ACQ Termination Condition Data"},
    {DCM_ACQREPETITIONTIME, DCM_DS, "ACQ Repetition Time"},
    {DCM_ACQECHOTIME, DCM_DS, "ACQ Echo Time"},
    {DCM_ACQINVERSIONTIME, DCM_DS, "ACQ Inversion Time"},
    {DCM_ACQNUMBEROFAVERAGES, DCM_DS, "ACQ Number of Averages"},
    {DCM_ACQIMAGINGFREQUENCY, DCM_DS, "ACQ Imaging Frequency"},
    {DCM_ACQIMAGEDNUCLEUS, DCM_SH, "ACQ Imaged Nucleus"},
    {DCM_ACQECHONUMBER, DCM_IS, "ACQ Echo Number"},
    {DCM_ACQMAGNETICFIELDSTRENGTH, DCM_DS, "ACQ Magnetic Field Strength"},
    {DCM_ACQSLICESPACING, DCM_DS, "ACQ Spacing Between Slices"},
    {DCM_ACQPHASEENCODINGSTEPS, DCM_IS, "ACQ Number of Phase Encoding Steps"},
    {DCM_ACQDATACOLLECTIONDIAMETER, DCM_DS, "ACQ Data Collection Diameter"},
    {DCM_ACQECHOTRAINLENGTH, DCM_IS, "ACQ Echo Train Length"},
    {DCM_ACQPERCENTSAMPLING, DCM_DS, "ACQ Percent Sampling"},
    {DCM_ACQPERCENTPHASEFIELDVIEW, DCM_DS, "ACQ Percent Phase Field of View"},
    {DCM_ACQPIXELBANDWIDTH, DCM_DS, "ACQ Pixel Bandwidth"},
    {DCM_ACQDEVICESERIALNUM, DCM_LO, "ACQ Device Serial Number"},
    {DCM_MAKETAG(0x0018, 0x1002), DCM_UI, "ACQ Device UID"},
    {DCM_MAKETAG(0x0018, 0x1003), DCM_LO, "ACQ Device ID"},
    {DCM_MAKETAG(0x0018, 0x1004), DCM_LO, "ACQ ACQ Plate ID"},
    {DCM_MAKETAG(0x0018, 0x1005), DCM_LO, "ACQ Generator ID"},
    {DCM_MAKETAG(0x0018, 0x1006), DCM_LO, "ACQ Grid ID"},
    {DCM_MAKETAG(0x0018, 0x1007), DCM_LO, "ACQ Cassette ID"},
    {DCM_MAKETAG(0x0018, 0x1008), DCM_LO, "ACQ Gantry ID"},
    {DCM_MAKETAG(0x0018, 0x1010), DCM_LO, "ACQ Secondary Capture Device ID"},
    {DCM_MAKETAG(0x0018, 0x1011), DCM_LO, "ACQ Hardcopy Creation Device ID (RET)"},
    {DCM_ACQDATESECONDARYCAPTURE, DCM_DA, "ACQ Date of Secondary Capture"},
    {DCM_ACQTIMESECONDARYCAPTURE, DCM_TM, "ACQ Time of Secondary Capture"},
    {DCM_ACQSECONDARYCAPTMANUFACTURER, DCM_LO,
    "ACQ Secondary Capture Device Manufacturer"},
    {DCM_MAKETAG(0x0018, 0x1017), DCM_LO, "ACQ Hardcopy Device Manufacturer (RET)"},
    {DCM_MAKETAG(0x0018, 0x1018), DCM_LO, "ACQ Secondary Capture Device Model Name"},
    {DCM_MAKETAG(0x0018, 0x1019), DCM_LO, "ACQ Secondary Capture Device Software Version"},
    {DCM_MAKETAG(0x0018, 0x101A), DCM_LO, "ACQ Hardcopy Device Software Version (RET)"},
    {DCM_MAKETAG(0x0018, 0x101B), DCM_LO, "ACQ Hardcopy Device Mfrs Model Name (RET)"},
    {DCM_ACQSOFTWAREVERSION, DCM_LO, "ACQ Software Version"},
    {DCM_ACQVIDEOIMAGEFORMATACQ, DCM_SH, "ACQ Video Image Format Acquired"},
    {DCM_ACQDIGITALIMAGEFORMATACQ, DCM_LO, "ACQ Digital Image Format Acquired"},
    {DCM_ACQPROTOCOLNAME, DCM_LO, "ACQ Protocol Name"},
    {DCM_ACQCONTRASTBOLUSROUTE, DCM_LO, "ACQ Contrast/Bolus Route"},
    {DCM_ACQCONTRASTBOLUSVOL, DCM_DS, "ACQ Contrast/Bolus Volume"},
    {DCM_ACQCONTRASTBOLUSSTARTTIME, DCM_TM, "ACQ Contrast/Bolus Start Time"},
    {DCM_ACQCONTRASTBOLUSSTOPTIME, DCM_TM, "ACQ Contrast/Bolus Stop Time"},
    {DCM_ACQCONTRASTBOLUSTOTALDOSE, DCM_DS, "ACQ Contrast/Bolus Total Dose"},
    {DCM_ACQSYRINGECOUNTS, DCM_IS, "ACQ Syringe Counts"},
    {DCM_ACQCONTRASTFLOWRATE, DCM_DS, "ACQ Contrast Flow Rate (ml/sec)"},
    {DCM_ACQCONTRASTFLOWDURATION, DCM_DS, "ACQ Contrast Flow Duration (sec)"},
    {DCM_ACQCONTRASTBOLUSINGREDIENT, DCM_CS, "ACQ Contrast/Bolus Ingredient"},
    {DCM_ACQCONTRASTBOLUSINGREDIENTCONCENTRATION, DCM_DS, "ACQ Contrast/Bolus Ingredient Concentration"},
    {DCM_ACQSPATIALRESOLUTION, DCM_DS, "ACQ Spatial Resolution"},
    {DCM_ACQTRIGGERTIME, DCM_DS, "ACQ Trigger Time"},
    {DCM_ACQTRIGGERSRCTYPE, DCM_LO, "ACQ Trigger Source or Type"},
    {DCM_ACQNOMINALINTERVAL, DCM_IS, "ACQ Nominal Interval"},
    {DCM_ACQFRAMETIME, DCM_DS, "ACQ Frame Time"},
    {DCM_ACQFRAMINGTYPE, DCM_LO, "ACQ Cardiac Framing Type"},
    {DCM_ACQFRAMETIMEVECTOR, DCM_DS, "ACQ Frame Time Vector"},
    {DCM_ACQFRAMEDELAY, DCM_DS, "ACQ Frame Delay"},
    {DCM_ACQIMAGETRIGGERDELAY, DCM_DS, "ACQ Image Trigger Delay"},	/* Sup 30 0.6 */
    {DCM_ACQGROUPTIMEOFFSET, DCM_DS, "ACQ Group Time Offset"},	/* Sup 30 0.6 */
    {DCM_ACQTRIGGERTIMEOFFSET, DCM_DS, "ACQ Trigger Time Offset"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x0018, 0x106A), DCM_CS, "ACQ Synchronization Trigger"},	/* Sup 30 0.6 */
/*    {DCM_MAKETAG(0x0018, 0x106B), DCM_UI, "ACQ Synchronization Frame of Reference"},*//* Sup 30 0.6 */
    {DCM_MAKETAG(0x0018, 0x106C), DCM_US, "ACQ Synchronization Channel"},
    {DCM_MAKETAG(0x0018, 0x106E), DCM_UL, "ACQ Trigger Sample Position"},	/* Sup 30 0.6 */
    {DCM_ACQRADIOPHARMROUTE, DCM_LO, "ACQ Radiopharmaceutical Route"},
    {DCM_ACQRADIOPHARMVOLUME, DCM_DS, "ACQ Radiopharmaceutical Volume"},
    {DCM_ACQRADIOPHARMSTARTTIME, DCM_TM, "ACQ Radiopharmaceutical Start Time"},
    {DCM_ACQRADIOPHARMSTOPTIME, DCM_TM, "ACQ Radiopharmaceutical Stop Time"},
    {DCM_ACQRADIONUCLIDETOTALDOSE, DCM_DS, "ACQ Radionuclide Total Dose"},
    {DCM_ACQRADIONUCLIDEHALFLIFE, DCM_DS, "ACQ Radionuclide Half Life"},
    {DCM_ACQRADIONUCLIDEPOSITRONFRACTION, DCM_DS, "ACQ Radionuclide Positron Fraction"},
    {DCM_MAKETAG(0x0018, 0x1077), DCM_DS, "ACQ Radiopharmaceutical Specific Activity"},
    {DCM_MAKETAG(0x0018, 0x1078), DCM_DT, "ACQ Radiopharmaceutical Start DateTime"},
    {DCM_MAKETAG(0x0018, 0x1079), DCM_DT, "ACQ Radiopharmaceutical Stop DateTime"},
    {DCM_MAKETAG(0x0018, 0x1080), DCM_CS, "ACQ Beat Rejection Flag"},
    {DCM_ACQLOWRRVALUE, DCM_IS, "ACQ Low R-R Value"},
    {DCM_ACQHIGHRRVALUE, DCM_IS, "ACQ High R-R Value"},
    {DCM_ACQINTERVALSACQUIRED, DCM_IS, "ACQ Intervals Acquired"},
    {DCM_ACQINTERVALSREJECTED, DCM_IS, "ACQ Intervals Rejected"},
    {DCM_ACQPVCREJECTION, DCM_LO, "ACQ PVC Rejection"},
    {DCM_ACQSKIPBEATS, DCM_IS, "ACQ Skip Beats"},
    {DCM_ACQHEARTRATE, DCM_IS, "ACQ Heart Rate"},
    {DCM_ACQCARDIACNUMBEROFIMAGES, DCM_IS, "ACQ Cardiac Number of Images"},
    {DCM_ACQTRIGGERWINDOW, DCM_IS, "ACQ Trigger Window"},
    {DCM_ACQRECONSTRUCTIONDIAMETER, DCM_DS, "ACQ Reconstruction Diameter"},
    {DCM_ACQDISTANCESRCTODETECTOR, DCM_DS, "ACQ Distance Source-Detector"},
    {DCM_ACQDISTANCESRCTOPATIENT, DCM_DS, "ACQ Distance Source-Patient"},
    {DCM_ACQESTIMATEDRADIOGRAPHICMAGFACTOR, DCM_DS, "ACQ Estimated Radiographic Mag Factor"},
    {DCM_ACQGANTRYTILT, DCM_DS, "ACQ Gantry/Detector Tilt"},
    {DCM_ACQGANTRYSLEW, DCM_DS, "ACQ Gantry/Detector Slew"},
    {DCM_ACQTABLEHEIGHT, DCM_DS, "ACQ Table Height"},
    {DCM_ACQTABLETRAVERSE, DCM_DS, "ACQ Table Traverse"},
    {DCM_ACQTABLEMOTION, DCM_CS, "ACQ Table Motion (STATIC, DYNAMIC)"},
    {DCM_ACQTABLEVERTICALINCREMENT, DCM_DS, "ACQ Table Vertical Increment (mm)"},
    {DCM_ACQTABLELATERALINCREMENT, DCM_DS, "ACQ Table Lateral Increment (mm)"},
    {DCM_MAKETAG(0x0018, 0x1137), DCM_DS, "ACQ Table Longitudinal Increment (mm)"},
    {DCM_MAKETAG(0x0018, 0x1138), DCM_DS, "ACQ Table Angle (relative to horizontal: deg)"},
    {DCM_MAKETAG(0x0018, 0x113A), DCM_CS, "ACQ Table Type"},
    {DCM_MAKETAG(0x0018, 0x1140), DCM_CS, "ACQ Rotation Direction"},
    {DCM_MAKETAG(0x0018, 0x1141), DCM_DS, "ACQ Angular Position (RET)"},
    {DCM_ACQRADIALPOSITION, DCM_DS, "ACQ Radial Position"},
    {DCM_ACQSCANARC, DCM_DS, "ACQ Scan Arc"},
    {DCM_ACQANGULARSTEP, DCM_DS, "ACQ Angular Step"},
    {DCM_ACQCENTERROTATIONOFFSET, DCM_DS, "ACQ Center of Rotation Offset"},
    {DCM_ACQROTATIONOFFSET, DCM_DS, "ACQ Rotation Offset (RET)"},
    {DCM_ACQFIELDOFVIEWSHAPE, DCM_CS, "ACQ Field of View Shape"},
    {DCM_ACQFIELDOFVIEWDIMENSION, DCM_IS, "ACQ Field of View Dimension(s)"},
    {DCM_MAKETAG(0x0018, 0x1150), DCM_IS, "ACQ Exposure Time"},
    {DCM_MAKETAG(0x0018, 0x1151), DCM_IS, "ACQ X-ray Tube Current"},
    {DCM_MAKETAG(0x0018, 0x1152), DCM_IS, "ACQ Exposure"},
    {DCM_MAKETAG(0x0018, 0x1153), DCM_IS, "ACQ Exposure in micro-As"},
    {DCM_MAKETAG(0x0018, 0x1154), DCM_DS, "ACQ Average width of X-Ray pulse (ms)"},
    {DCM_MAKETAG(0x0018, 0x1155), DCM_CS, "ACQ Radiation Setting"},
    {DCM_MAKETAG(0x0018, 0x1156), DCM_CS, "ACQ Replication Type"},
    {DCM_ACQRADIATIONMODE, DCM_CS, "ACQ X-Ray radiation mode (CONTINUOUS, PULSED)"},
    {DCM_ACQIMAGEAREADOSEPRODUCT, DCM_DS, "ACQ X-Ray dose to which patient was exposed"},
    {DCM_ACQFILTERTYPE, DCM_SH, "ACQ Filter Type, extremity"},
    {DCM_ACQTYPEOFFILTERS, DCM_LO, "ACQ Type of filter(s) inserted into X-Ray beam"},
    {DCM_ACQINTENSIFIERSIZE, DCM_DS, "ACQ Intensifier Size (mm)"},
    {DCM_ACQIMAGERPIXELSPACING, DCM_DS, "ACQ Image Pixel Spacing"},
    {DCM_ACQGRID, DCM_CS, "ACQ Grid (IN, NONE)"},
    {DCM_ACQGENERATORPOWER, DCM_IS, "ACQ Generator Power"},
    {DCM_ACQCOLLIMATORGRIDNAME, DCM_SH, "ACQ Collimator/Grid Name"},
    {DCM_ACQCOLLIMATORTYPE, DCM_CS, "ACQ Collimator Type"},
    {DCM_ACQFOCALDISTANCE, DCM_IS, "ACQ Focal Distance"},
    {DCM_ACQXFOCUSCENTER, DCM_DS, "ACQ X Focus Center"},
    {DCM_ACQYFOCUSCENTER, DCM_DS, "ACQ Y Focus Center"},
    {DCM_MAKETAG(0x0018, 0x11a2), DCM_DS, "Compression Force"},
    {DCM_ACQFOCALSPOT, DCM_DS, "ACQ Focal Spot"},
    {DCM_MAKETAG(0x0018, 0x1191), DCM_CS, "ACQ Anode Target Material"},
    {DCM_MAKETAG(0x0018, 0x11A0), DCM_DS, "ACQ Body Part Thickness"},
    {DCM_MAKETAG(0x0018, 0x11A2), DCM_DS, "ACQ Compression Force"},
    {DCM_ACQDATELASTCALIBRATION, DCM_DA, "ACQ Date of Last Calibration"},
    {DCM_ACQTIMELASTCALIBRATION, DCM_TM, "ACQ Time of Last Calibration"},
    {DCM_ACQCONVOLUTIONKERNEL, DCM_SH, "ACQ Convolution Kernel"},
    {DCM_ACQUPPERLOWERPIXELVALUES, DCM_RET, "ACQ Upper/Lower Pixel Values (RET)"},
    {DCM_ACQACTUALFRAMEDURATION, DCM_IS, "ACQ Actual Frame Duration"},
    {DCM_ACQCOUNTRATE, DCM_IS, "ACQ Count Rate"},
    {DCM_ACQPREFPLAYBACKSEQUENCING, DCM_US, "ACQ Preferred Playback Sequencing"},
    {DCM_ACQRECEIVINGCOIL, DCM_SH, "ACQ Receiving Coil"},
    {DCM_ACQTRANSMITTINGCOIL, DCM_SH, "ACQ Transmitting Coil"},
    {DCM_ACQPLATETYPE, DCM_SH, "ACQ Plate Type"},
    {DCM_ACQPHOSPHORTYPE, DCM_LO, "ACQ Phosphor Type"},
#if STANDARD_VERSION < VERSION_APR1995
    {DCM_ACQSCANVELOCITY, DCM_IS, "ACQ Scan Velocity"},
#else
    {DCM_ACQSCANVELOCITY, DCM_DS, "ACQ Scan Velocity"},
#endif
    {DCM_ACQWHOLEBODYTECHNIQUE, DCM_CS, "ACQ Whole Body Technique"},
    {DCM_ACQSCANLENGTH, DCM_IS, "ACQ Scan Length"},
    {DCM_ACQACQUISITIONMATRIX, DCM_US, "ACQ Acquisition Matrix"},
    {DCM_ACQPHASEENCODINGDIRECTION, DCM_CS, "ACQ Phase Encoding Direction"},
    {DCM_ACQFLIPANGLE, DCM_DS, "ACQ Flip Angle"},
    {DCM_ACQVARIABLEFLIPANGLE, DCM_CS, "ACQ Variable Flip Angle"},
    {DCM_ACQSAR, DCM_DS, "ACQ SAR"},
    {DCM_ACQDBDT, DCM_DS, "ACQ DB/DT"},
    {DCM_ACQDEVICEPROCESSINGDESCR, DCM_LO, "ACQ Acquisition Device Processing Description"},
    {DCM_ACQDEVICEPROCESSINGCODE, DCM_LO, "ACQ Acquisition Device Processing Code"},
    {DCM_ACQCASSETTEORIENTATION, DCM_CS, "ACQ Cassette Orientation"},
    {DCM_ACQCASSETTESIZE, DCM_CS, "ACQ Cassette Size"},
    {DCM_ACQEXPOSURESONPLATE, DCM_US, "ACQ Exposures on Plate"},
    {DCM_ACQRELATIVEXRAYEXPOSURE, DCM_IS, "ACQ Relative X-ray Exposure"},
	{DCM_MAKETAG(0x0018, 0x1411), DCM_DS, "ACQ Exposure Index"},
	{DCM_MAKETAG(0x0018, 0x1412), DCM_DS, "ACQ Target Exposure Index"},
	{DCM_MAKETAG(0x0018, 0x1413), DCM_DS, "ACQ Deviation Index"},
    {DCM_ACQCOLUMNANGULATION, DCM_DS, "ACQ Column Angulation"},
    {DCM_ACQTOMOLAYERHEIGHT, DCM_DS, "ACQ Tomo Layer Height (mm)"},
    {DCM_ACQTOMOANGLE, DCM_DS, "ACQ Tomo Angle"},
    {DCM_ACQTOMOTIME, DCM_DS, "ACQ Tomo Time"},
    {0x00181490, DCM_CS, "ACQ Tomo Type"},			/* 2002.04.26 */
    {0x00181491, DCM_CS, "ACQ Tomo Class"},			/* 2002.04.26 */
    {0x00181495, DCM_IS, "ACQ Number of Tomosynthesis Source Images"}, /* 2002.04.26 */
    {DCM_ACQPOSITIONERMOTION, DCM_CS, "ACQ Positioner Motion"},
    {0x00181508, DCM_CS, "ACQ Positioner Type"},		/* 2002.04.26 */
    {DCM_ACQPOSITIONERPRIMARYANGLE, DCM_DS, "ACQ Positioner Primary Angle"},
    {DCM_ACQPOSITIONERSECONDARYANGLE, DCM_DS, "ACQ Positioner Secondary Angle"},
    {DCM_ACQPOSITIONERPRIMARYANGLEINCR, DCM_DS, "ACQ Positioner Primary Angle Increment"},
    {DCM_ACQPOSITIONERSECONDARYANGLEINCR, DCM_DS, "ACQ Positioner Secondary Angle Increment"},
    {DCM_ACQDETECTORPRIMARYANGLE, DCM_DS, "ACQ Detector Primary Angle"},
    {DCM_ACQDETECTORSECONDARYANGLE, DCM_DS, "ACQ Detector Secondary Angle"},
    {DCM_ACQSHUTTERSHAPE, DCM_CS, "ACQ Shutter Shape"},
    {DCM_ACQSHUTTERLEFTVERTICALEDGE, DCM_IS, "ACQ Shutter Left Vertical Edge"},
    {DCM_ACQSHUTTERRIGHTVERTICALEDGE, DCM_IS, "ACQ Shutter Right Vertical Edge"},
    {DCM_ACQSHUTTERUPPERHORIZONTALEDGE, DCM_IS, "ACQ Shutter Upper Horizontal Edge"},
    {DCM_ACQSHUTTERLOWERHORIZONTALEDGE, DCM_IS, "ACQ Shutter Lower Horizontal Edge"},
    {DCM_ACQCENTEROFCIRCULARSHUTTER, DCM_IS, "ACQ Center of Circular Shutter"},
    {DCM_ACQRADIUSOFCIRCULARSHUTTER, DCM_IS, "ACQ Radius of Circular Shutter"},
    {DCM_ACQVERTICESOFPOLYGONALSHUTTER, DCM_IS, "ACQ Vertices of the Polygonal Shutter"},
    {DCM_MAKETAG(0x0018, 0x1622), DCM_US, "ACQ Shutter Presentation Value"},
    {DCM_MAKETAG(0x0018, 0x1624), DCM_US, "ACQ Shutter Presentation Color CIELab Value"}, /* Supp 100 */
    {DCM_ACQCOLLIMATORSHAPE, DCM_CS, "ACQ Collimator Shape"},
    {DCM_ACQCOLLIMATORLEFTVERTICALEDGE, DCM_IS, "ACQ Collimator Left Vertical Edge"},
    {DCM_ACQCOLLIMATORRIGHTVERTICALEDGE, DCM_IS, "ACQ Collimator Right Vertical Edge"},
    {DCM_ACQCOLLIMATORUPPERHORIZONTALEDGE, DCM_IS, "ACQ Collimator Upper Horizontal Edge"},
    {DCM_ACQCOLLIMATORLOWERHORIZONTALEDGE, DCM_IS, "ACQ Collimator Lower Horizontal Edge"},
    {DCM_ACQCENTEROFCIRCULARCOLLIMATOR, DCM_IS, "ACQ Center of Circular Collimator"},
    {DCM_ACQRADIUSOFCIRCULARCOLLIMATOR, DCM_IS, "ACQ Radius of Circular Collimator"},
    {DCM_ACQVERTICESOFPOLYGONALCOLLIMATOR, DCM_IS, "ACQ Vertices of the Polygonal Collimator"},
    {DCM_MAKETAG(0x0018, 0x1800), DCM_CS, "ACQ Acquisition Time Synchronized"},	/* Sup 30 0.7 */
    {DCM_MAKETAG(0x0018, 0x1801), DCM_SH, "ACQ Time Source"},	/* Sup 30 0.7 */
    {DCM_MAKETAG(0x0018, 0x1802), DCM_CS, "ACQ Time Distribution Protocol"},	/* Sup 30 0.7 */
    {DCM_MAKETAG(0x0018, 0x1803), DCM_LO, "ACQ NTP Source Address"},
    {DCM_MAKETAG(0x0018, 0x2001), DCM_IS, "ACQ Page Number Vector"},
    {DCM_MAKETAG(0x0018, 0x2002), DCM_SH, "ACQ Frame Label Vector"},
    {DCM_MAKETAG(0x0018, 0x2003), DCM_DS, "ACQ Frame Primary Angle Vector"},
    {DCM_MAKETAG(0x0018, 0x2004), DCM_DS, "ACQ Frame Secondary Angle Vector"},
    {DCM_MAKETAG(0x0018, 0x2005), DCM_DS, "ACQ Slice Location Vector"},
    {DCM_MAKETAG(0x0018, 0x2006), DCM_SH, "ACQ Display WIndow Label Vector"},
    {DCM_MAKETAG(0x0018, 0x2010), DCM_DS, "ACQ Nominal Scanned Pixel Spacing"},
    {DCM_MAKETAG(0x0018, 0x2020), DCM_CS, "ACQ Digitizing Device Transport Direction"},
    {DCM_MAKETAG(0x0018, 0x2030), DCM_DS, "ACQ Rotation of Scanned Film"},
    {DCM_MAKETAG(0x0018, 0x3100), DCM_CS, "ACQ IVUS Acquisition"},
    {DCM_MAKETAG(0x0018, 0x3101), DCM_DS, "ACQ IVUS Pullback Rate"},
    {DCM_MAKETAG(0x0018, 0x3102), DCM_DS, "ACQ IVUS Gated Rate"},
    {DCM_MAKETAG(0x0018, 0x3103), DCM_IS, "ACQ IVUS Pullback Start Frame Number"},
    {DCM_MAKETAG(0x0018, 0x3104), DCM_IS, "ACQ IVUS Pullback Stop Frame Number"},
    {DCM_MAKETAG(0x0018, 0x3105), DCM_IS, "ACQ Lesion Number"},

    {DCM_MAKETAG(0x0018, 0x4000), DCM_LT, "ACQ Comments (RET)"},
    {DCM_MAKETAG(0x0018, 0x5000), DCM_SH, "ACQ Output Power"},
    {DCM_MAKETAG(0x0018, 0x5010), DCM_LO, "ACQ Transducer Data"},
    {DCM_MAKETAG(0x0018, 0x5012), DCM_DS, "ACQ Focus Depth"},
    {DCM_MAKETAG(0x0018, 0x5020), DCM_LO, "ACQ Processing Function"},
    {DCM_MAKETAG(0x0018, 0x5021), DCM_LO, "ACQ Postprocessing Function (RET)"},
    {DCM_MAKETAG(0x0018, 0x5022), DCM_DS, "ACQ Mechanical Index"},
    {DCM_MAKETAG(0x0018, 0x5024), DCM_DS, "ACQ Thermal Index"},
    {DCM_MAKETAG(0x0018, 0x5026), DCM_DS, "ACQ Cranial Thermal Index"},
    {DCM_MAKETAG(0x0018, 0x5027), DCM_DS, "ACQ Soft Tissue Thermal Index"},
    {DCM_MAKETAG(0x0018, 0x5028), DCM_DS, "ACQ Soft Tissue-focus Thermal Index"},
    {DCM_MAKETAG(0x0018, 0x5029), DCM_CS, "ACQ Soft Tissue-surface Thermal Index"},
    {DCM_MAKETAG(0x0018, 0x5030), DCM_DS, "ACQ Dynamic Range (RET)"},
    {DCM_MAKETAG(0x0018, 0x5040), DCM_DS, "ACQ Total Gain (RET)"},
    {DCM_ACQDEPTHOFSCANFIELD, DCM_IS, "ACQ Depth of Scan Field"},
    {DCM_ACQPATIENTPOSITION, DCM_CS, "ACQ Patient Position"},
    {DCM_ACQVIEWPOSITION, DCM_CS, "ACQ View Position"},
    {DCM_MAKETAG(0x0018, 0x5104), DCM_SQ, "ACQ Projection Eponymous Name Code Sequence"},
    {DCM_MAKETAG(0x0018, 0x5210), DCM_DS, "ACQ Image Transformation Matrix (RET)"},
    {DCM_MAKETAG(0x0018, 0x5212), DCM_DS, "ACQ Image Translation Vector (RET)"},
    {DCM_ACQSENSITIVITY, DCM_DS, "ACQ Sensitivity"},
    {DCM_ACQUSREGIONSEQUENCE, DCM_SQ, "ACQ Ultrasound Region Sequence"},
    {DCM_ACQREGIONSPATIALFORMAT, DCM_US, "ACQ Region Spatial Format"},
    {DCM_ACQREGIONDATATYPE, DCM_US, "ACQ Region Data Type"},
    {DCM_ACQREGIONFLAGS, DCM_UL, "ACQ Region Flags"},
    {DCM_ACQREGIONLOCATIONMINX0, DCM_UL, "ACQ Region Location Min X(0)"},
    {DCM_ACQREGIONLOCATIONMINY0, DCM_UL, "ACQ Region Location Min Y(0)"},
    {DCM_ACQREGIONLOCATIONMAXX1, DCM_UL, "ACQ Region Location Max X(1)"},
    {DCM_ACQREGIONLOCATIONMAXY1, DCM_UL, "ACQ Region Location Max Y(1)"},
    {DCM_ACQREFERENCEPIXELX, DCM_SL, "ACQ Reference Pixel X"},
    {DCM_ACQREFERENCEPIXELY, DCM_SL, "ACQ Reference Pixel Y"},
    {DCM_ACQPHYSICALUNITSXDIRECTION, DCM_US, "ACQ Physical Units X Direction"},
    {DCM_ACQPHYSICALUNITSYDIRECTION, DCM_US, "ACQ Physical Units Y Direction"},
    {DCM_ACQREFPIXELPHYSICALVALUEX, DCM_FD, "ACQ Reference Pixel Physical Value X"},
    {DCM_ACQREFPIXELPHYSICALVALUEY, DCM_FD, "ACQ Reference Pixel Physical Value Y"},
    {DCM_ACQPHYSICALDELTAX, DCM_FD, "ACQ Physical Delta X"},
    {DCM_ACQPHYSICALDELTAY, DCM_FD, "ACQ Physical Delta Y"},
    {DCM_ACQTRANSDUCERFREQUENCY, DCM_UL, "ACQ Transducer Frequency"},
    {DCM_ACQTRANSDUCERTYPE, DCM_CS, "ACQ Transducer Type"},
    {DCM_ACQPULSEREPETITIONFREQ, DCM_UL, "ACQ Pulse Repetition Frequency"},
    {DCM_ACQDOPPLERCORRECTIONANGLE, DCM_FD, "ACQ Doppler Correction Angle"},
    {DCM_ACQSTERRINGANGLE, DCM_FD, "ACQ Steering Angle"},
    {DCM_MAKETAG(0x0018, 0x6038), DCM_UL, "ACQ Doppler Sample Volume X Position (RET)"},
    {DCM_MAKETAG(0x0018, 0x6039), DCM_SL, "ACQ Doppler Sample Volume X Position"},
    {DCM_MAKETAG(0x0018, 0x603A), DCM_UL, "ACQ Doppler Sample Volume Y Position (RET)"},
    {DCM_MAKETAG(0x0018, 0x603B), DCM_SL, "ACQ Doppler Sample Volume Y Position"},
    {DCM_MAKETAG(0x0018, 0x603C), DCM_UL, "ACQ TM-Line Position X(0) (RET)"},
    {DCM_MAKETAG(0x0018, 0x603D), DCM_SL, "ACQ TM-Line Position X(0)"},
    {DCM_MAKETAG(0x0018, 0x603E), DCM_UL, "ACQ TM-Line Position Y(0) (RET)"},
    {DCM_MAKETAG(0x0018, 0x603F), DCM_SL, "ACQ TM-Line Position Y(0)"},
    {DCM_MAKETAG(0x0018, 0x6040), DCM_UL, "ACQ TM-Line Position X(1) (RET)"},
    {DCM_MAKETAG(0x0018, 0x6041), DCM_SL, "ACQ TM-Line Position X(1)"},
    {DCM_MAKETAG(0x0018, 0x6042), DCM_UL, "ACQ TM-Line Position Y(1) (RET)"},
    {DCM_MAKETAG(0x0018, 0x6043), DCM_SL, "ACQ TM-Line Position Y(1)"},
    {DCM_MAKETAG(0x0018, 0x6044), DCM_US, "ACQ Pixel Component Organization"},
    {DCM_ACQPIXELCOMPMASK, DCM_UL, "ACQ Pixel Component Mask"},
    {DCM_ACQPIXELCOMPRANGESTART, DCM_UL, "ACQ Pixel Component Range Start"},
    {DCM_ACQPIXELCOMPRANGESTOP, DCM_UL, "ACQ Pixel Component Range Stop"},
    {DCM_ACQPIXELCOMPPHYSUNITS, DCM_US, "ACQ Pixel Component Physical Units"},
    {DCM_ACQPIXELCOMPDATATYPE, DCM_US, "ACQ Pixel Component Data Type"},
    {DCM_ACQNUMBERTABLEBREAKPOINTS, DCM_UL, "ACQ Number of Table Break Points"},
    {DCM_ACQTABLEXBREAKPOINTS, DCM_UL, "ACQ Table of X Break Points"},
    {DCM_ACQTABLEYBREAKPOINTS, DCM_FD, "ACQ Table of Y Break Points"},
    {DCM_ACQNUMBEROFTABLEENTRIES, DCM_UL, "ACQ Number of Table Entries"},
    {DCM_ACQTABLEOFPIXELVALUES, DCM_UL, "ACQ Table of Pixel Values"},
    {DCM_MAKETAG(0x0018, 0x605A), DCM_FL, "ACQ Table of Parameter Values"},
    {DCM_MAKETAG(0x0018, 0x6060), DCM_FL, "ACQ R Wave Time Vector"},

    {DCM_MAKETAG(0x0018,0x7000), DCM_CS, "ACQ Detector Conditions Nominal Flag"}, /* 2002.04.26 */
    {DCM_MAKETAG(0x0018,0x7001), DCM_DS, "ACQ Detector Temperature"},		/* 2002.04.26 */
    {DCM_MAKETAG(0x0018,0x7004), DCM_CS, "ACQ Detector Type"},			/* 2002.04.26 */
    {DCM_MAKETAG(0x0018,0x7005), DCM_CS, "ACQ Detector Configuration"},		/* 2002.04.26 */
    {DCM_MAKETAG(0x0018,0x7006), DCM_LT, "ACQ Detector Description"},		/* 2002.04.26 */
    {DCM_MAKETAG(0x0018,0x7008), DCM_LT, "ACQ Detector Mode"},			/* 2002.04.26 */
    {DCM_MAKETAG(0x0018,0x700A), DCM_SH, "ACQ Detector ID"},			/* 2002.04.26 */
    {DCM_MAKETAG(0x0018,0x700C), DCM_DA, "ACQ Date of Last Detector Calibration"},/* 2004.09.14 */
    {DCM_MAKETAG(0x0018,0x700E), DCM_TM, "ACQ Time of Last Detector Calibration"},/* 2004.09.14 */
    {DCM_MAKETAG(0x0018,0x7010), DCM_IS, "ACQ Exposures on Detector Since Last Calib"},
    {DCM_MAKETAG(0x0018,0x7011), DCM_IS, "ACQ Exposures on Detector Since Manufactured"},
    {DCM_MAKETAG(0x0018,0x7012), DCM_DS, "ACQ Detector Time Snce Last Exposure"},
    {DCM_MAKETAG(0x0018,0x7014), DCM_DS, "ACQ Detector Active Time"},
    {DCM_MAKETAG(0x0018,0x7016), DCM_DS, "ACQ Detector Activation Offset From Exposure"},
    {DCM_MAKETAG(0x0018,0x701A), DCM_DS, "ACQ Detector Binning"},
    {DCM_MAKETAG(0x0018,0x7020), DCM_DS, "ACQ Detector Element Physical Size"},
    {DCM_MAKETAG(0x0018,0x7022), DCM_DS, "ACQ Detector Element Spacing"},
    {DCM_MAKETAG(0x0018,0x7024), DCM_CS, "ACQ Detector Active Shape"},
    {DCM_MAKETAG(0x0018,0x7026), DCM_DS, "ACQ Detector Active Dimensions"},

    {DCM_MAKETAG(0x0018,0x7028), DCM_DS, "ACQ Detector Active Origin"},		/* 2002.04.26 */
    {DCM_MAKETAG(0x0018,0x702A), DCM_LO, "ACQ Detector Manufacturer Name"},
    {DCM_MAKETAG(0x0018,0x702B), DCM_LO, "ACQ Detector Manufacturer's Model Name"},
    {DCM_MAKETAG(0x0018,0x7030), DCM_DS, "ACQ Field of View Origin"},
    {DCM_MAKETAG(0x0018,0x7032), DCM_DS, "ACQ Field of View Rotation"},
    {DCM_MAKETAG(0x0018,0x7034), DCM_CS, "ACQ Field of View Horizontal Flip"},
    {DCM_MAKETAG(0x0018,0x7040), DCM_LT, "ACQ Grid Absorbing Material"},
    {DCM_MAKETAG(0x0018,0x7041), DCM_LT, "ACQ Grid Spacing Material"},
    {DCM_MAKETAG(0x0018,0x7042), DCM_DS, "ACQ Grid Thickness"},
    {DCM_MAKETAG(0x0018,0x7044), DCM_DS, "ACQ Grid Pitch"},
    {DCM_MAKETAG(0x0018,0x7046), DCM_IS, "ACQ Grid Aspect Ratio"},
    {DCM_MAKETAG(0x0018,0x7048), DCM_DS, "ACQ Grid Period"},
    {DCM_MAKETAG(0x0018,0x704C), DCM_DS, "ACQ Focal Distance"},

    {DCM_MAKETAG(0x0018,0x7050), DCM_CS, "ACQ Filter Material"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x7052), DCM_DS, "ACQ Filter Thickness Minimum"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x7054), DCM_DS, "ACQ Filter Thickness Maximum"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x7060), DCM_CS, "ACQ Exposure Control Mode"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x7062), DCM_LT, "ACQ Exposure Control Mode Description"},/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x7064), DCM_CS, "ACQ Exposure Status"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x7065), DCM_DS, "ACQ Phototimer Setting"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x8150), DCM_DS, "ACQ Exposure Time in uS"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x8151), DCM_DS, "ACQ Exposure X-Ray Tube Current in uA"},/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9004), DCM_CS, "ACQ Content Qualification"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9005), DCM_SH, "ACQ Pulse Sequence Name"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9006), DCM_SQ, "ACQ MR Imaging MOdifier Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9008), DCM_CS, "ACQ Echo Pulse Sequence"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9009), DCM_CS, "ACQ Inversion Recoverty"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9010), DCM_CS, "ACQ Flow Compensation"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9011), DCM_CS, "ACQ Multiple Spin Echo"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9012), DCM_CS, "ACQ Multi-planar Excitation"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9014), DCM_CS, "ACQ Phase Contrast"},			/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9015), DCM_CS, "ACQ Time of Flight Contrast"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9016), DCM_CS, "ACQ Spoiling"},			/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9017), DCM_CS, "ACQ Steady State Pulse Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9018), DCM_CS, "ACQ Echo Planar Pulse Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9019), DCM_FD, "ACQ Tag Angle First Axis"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9020), DCM_CS, "ACQ Magnetization Transfer"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9021), DCM_CS, "ACQ T2 Preparation"},			/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9022), DCM_CS, "ACQ Blood Signal Nulling"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9024), DCM_CS, "ACQ Saturation Recovery"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9025), DCM_CS, "ACQ Spectrally Selected Suppression"},/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9026), DCM_CS, "ACQ Spectrally Selected Excitation"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9027), DCM_CS, "ACQ Spatial Pre-saturation"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9028), DCM_CS, "ACQ Tagging"},			/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9029), DCM_CS, "ACQ Oversampling Phase"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9030), DCM_FD, "ACQ Tag Spacing First Dimension"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9032), DCM_CS, "ACQ Geometry of k-Space Traversal"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9033), DCM_CS, "ACQ Segmented k-Space Traversal"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9034), DCM_CS, "ACQ Rectilinear Phase Encode Recording"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9035), DCM_FD, "ACQ Tag Thickness"},			/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9036), DCM_CS, "ACQ Partial Fourier Direction"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9037), DCM_CS, "ACQ Cardiac Synchronization Technique"},/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9041), DCM_LO, "ACQ Receive Coil Manufacturer Name"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9042), DCM_SQ, "ACQ MR Receive Coil Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9043), DCM_CS, "ACQ Receive Coil Type"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9044), DCM_CS, "ACQ Quadrature Receive Coil"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9045), DCM_SQ, "ACQ Multi-Coil Definition Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9046), DCM_LO, "ACQ Multi-Coil Configuration"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9047), DCM_SH, "ACQ Multi-Coil Element Name"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9048), DCM_CS, "ACQ Multi-Coil ELement Used"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9049), DCM_SQ, "ACQ MR Transmit Coil Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9050), DCM_LO, "ACQ Transmit Coil Manufacturer Name"},/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9051), DCM_CS, "ACQ Transmit Coil Type"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9052), DCM_FD, "ACQ Coil Width"},			/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9053), DCM_FD, "ACQ Chemical Shift Reference"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9054), DCM_CS, "ACQ VOlume Localization Technique"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9058), DCM_US, "ACQ MR Acquisition Frequency Encoding Steps"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9059), DCM_CS, "ACQ De-coupling"},			/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9060), DCM_CS, "ACQ De-coupled Nucleus"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9061), DCM_FD, "ACQ De-coupling Frequency"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9062), DCM_CS, "ACQ De-coupling Method"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9063), DCM_FD, "ACQ De-coupling Chemical Shift Reference"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9064), DCM_CS, "ACQ k-space Filtering"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9065), DCM_CS, "ACQ Time Domain Filtering"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9066), DCM_US, "ACQ Number of Zero fills"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9067), DCM_CS, "ACQ Baseline Correction"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9069), DCM_FD, "ACQ Parallel Reduction Factor In-plane"},/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9070), DCM_FD, "ACQ Cardiac R-R Interval Specified"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9073), DCM_FD, "ACQ Acquisition Duration"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9074), DCM_DT, "ACQ Frame Acquisition Datetime"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9075), DCM_CS, "ACQ Diffusion Directionality"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9076), DCM_SQ, "ACQ Diffusion Gradient Direction Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9077), DCM_CS, "ACQ Parallel Acquisition"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9078), DCM_CS, "ACQ Parallel Acquisition Technique"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9079), DCM_FD, "ACQ Inversion Times"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9080), DCM_ST, "ACQ Metabolite Map Description"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9081), DCM_CS, "ACQ Partial Fourier"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9082), DCM_FD, "ACQ Effective Echo Time"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9083), DCM_SQ, "ACQ Metabolite Map Code Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9084), DCM_SQ, "ACQ Chemical Shift Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9085), DCM_CS, "ACQ Cardiac Signal Source"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9087), DCM_FD, "ACQ Diffusion b-value"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9089), DCM_FD, "ACQ Diffusion Gradient Orientation"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9090), DCM_FD, "ACQ Velocity Encoding Direction"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9091), DCM_FD, "ACQ Velocity Encoding Minimum Value"},/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9093), DCM_US, "ACQ Number of k-Space Trajectories"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9094), DCM_CS, "ACQ Coverage of k-Space"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9095), DCM_UL, "ACQ Spectoscopy Acquisiton Phase Rows"},/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9096), DCM_FD, "ACQ Parallel Reduction Factor in-plane (RET)"},
    {DCM_MAKETAG(0x0018,0x9098), DCM_FD, "ACQ Transmitter Frequency"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9100), DCM_CS, "ACQ Resonan Nucleus"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9101), DCM_CS, "ACQ Frequency Correction"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9103), DCM_SQ, "ACQ MR Spectroscopy FOV/Geometry Sequence"},/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9104), DCM_FD, "ACQ Slab Thickness"},			/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9105), DCM_FD, "ACQ Slab Orientation"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9106), DCM_FD, "ACQ Mid Slab Position"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9107), DCM_SQ, "ACQ MR Spatial Saturation Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9112), DCM_SQ, "ACQ MR Timing and Related Parameters Sequence"},/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9114), DCM_SQ, "ACQ MR Echo Sequence"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9115), DCM_SQ, "ACQ MR Modifier Sequence"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9117), DCM_SQ, "ACQ MR Diffusion Sequence"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9118), DCM_SQ, "ACQ Cardiac Trigger Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9119), DCM_SQ, "ACQ MR Averages Sequence"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9125), DCM_SQ, "ACQ MR FOV/Geometry Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9126), DCM_SQ, "ACQ Volume Localization Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9127), DCM_UL, "ACQ Spectroscopy Acquisiton Data Columns"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9147), DCM_CS, "ACQ Diffusion Anisotropy Type"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9151), DCM_DT, "ACQ Frame Reference Datetime"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9152), DCM_SQ, "ACQ MR Metabolite Map Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9155), DCM_FD, "ACQ Parallel Reduction Factor out-of-plane"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9159), DCM_UL, "ACQ Spectroscopy Acq Out-of-plane Phase Steps"},/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9166), DCM_CS, "ACQ Bulk Motion Status"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9168), DCM_FD, "ACQ Parallel Reduction Factor Second In-plane"},/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9169), DCM_CS, "ACQ Cardiace Beat Rejection Technique"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9170), DCM_CS, "ACQ Respiratory Motion Compensation Technique"},/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9171), DCM_CS, "ACQ Respiratory Signal Source"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9172), DCM_CS, "ACQ Bulk Motion Compensation Technique"},/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9173), DCM_CS, "ACQ Bulk Motion Signal Source"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9174), DCM_CS, "ACQ Applicable Safety Standard Agency"},/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9175), DCM_LO, "ACQ Application Safety Standard Description"},/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9176), DCM_SQ, "ACQ Operating Mode Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9177), DCM_CS, "ACQ Operating Mode Type"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9178), DCM_CS, "ACQ Operating Mode"},			/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9179), DCM_CS, "ACQ Specific Absorption Rate Definition"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9180), DCM_CS, "ACQ Gradient Output Type"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9181), DCM_FD, "ACQ Specific Absorption Rate Value"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9182), DCM_FD, "ACQ Gradient Output"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9183), DCM_CS, "ACQ Flow Compensation Direction"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9184), DCM_FD, "ACQ Tagging Delay"},			/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9185), DCM_ST, "ACQ Resp Motion Compensation Technique Descr"},
    {DCM_MAKETAG(0x0018,0x9186), DCM_SH, "ACQ Respiratory Signal Source ID"},
    {DCM_MAKETAG(0x0018,0x9195), DCM_FD, "ACQ Chem Shifts Min Integration Limit/Hz (RET)"},
    {DCM_MAKETAG(0x0018,0x9196), DCM_FD, "ACQ Chem Shifts Max Integration Limit/Hz (RET)"},

    {DCM_MAKETAG(0x0018,0x9197), DCM_SQ, "ACQ MR Velocity Encoding Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9198), DCM_CS, "ACQ First Order Phases Correction"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9199), DCM_CS, "ACQ Water Referenced Phase Correction"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9200), DCM_CS, "ACQ MR Spectroscopy Acquisition Type"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9214), DCM_CS, "ACQ Respiratory Cycle Position"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9217), DCM_FD, "ACQ Velocity Encoding Maximum Value"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9218), DCM_FD, "ACQ Tag Spacing Second Dimension"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9219), DCM_SS, "ACQ Tag Angle Second Axis"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9220), DCM_FD, "ACQ Frame Acquisition Duration"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9226), DCM_SQ, "ACQ MR Image Frame Type Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9227), DCM_SQ, "ACQ MR Spectroscopy Frame Type Sequence"},/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9231), DCM_US, "ACQ MR Acq Phase Encoding Steps in-plane"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9232), DCM_US, "ACQ MR Acq Phase Encoding Steps out-of-plane"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9234), DCM_UL, "ACQ Spectroscopy Acquisition Phase Columns"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9236), DCM_CS, "ACQ Cardiac Cycle Position"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9239), DCM_SQ, "ACQ Specific Absorption Rate Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9240), DCM_US, "ACQ RF Echo Train Length"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9241), DCM_US, "ACQ Gradient Echo Train Length"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9295), DCM_FD, "ACQ Chemical Shifts Min Integration Lim in ppm"},/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9296), DCM_FD, "ACQ Chemical Shifts Max Integration Lim in ppm"},/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9301), DCM_SQ, "ACQ CT Acquisition Type Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9302), DCM_CS, "ACQ Acquisition Type"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9303), DCM_FD, "ACQ Tube Angle"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9304), DCM_SQ, "ACQ CT Acquisition Details Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9305), DCM_FD, "ACQ Revolution Time"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9306), DCM_FD, "ACQ single Collimation Width"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9307), DCM_FD, "ACQ Total Collimation Width"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9308), DCM_SQ, "ACQ CT Table Dynamics Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9309), DCM_FD, "ACQ Table Speed"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9310), DCM_FD, "ACQ Table Feed per Rotation"},/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9311), DCM_FD, "ACQ Spiral Pitch Factor"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9312), DCM_SQ, "ACQ CT Geometry Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9313), DCM_FD, "ACQ Data Collection Center (Patient)"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9314), DCM_SQ, "ACQ CT Reconstruction Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9315), DCM_CS, "ACQ Reconstruction Algorithm"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9316), DCM_CS, "ACQ Convolution Kernel Group"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9317), DCM_FD, "ACQ Reconstruction Field of View"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9318), DCM_FD, "ACQ Reconstruction Target Center (Patient)"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9319), DCM_FD, "ACQ Reconstruction Angle"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9320), DCM_SH, "ACQ Image Filter"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9321), DCM_SQ, "ACQ CT Exposure Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9322), DCM_FD, "ACQ Reconstruction Pixel Spacing"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9323), DCM_CS, "ACQ Exposure Modulation Type"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9324), DCM_FD, "ACQ Estimated Dose Saving"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9325), DCM_SQ, "ACQ CT X-ray Details Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9326), DCM_SQ, "ACQ CT Position Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9327), DCM_FD, "ACQ Table Position"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9328), DCM_FD, "ACQ Exposure Time in ms"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9329), DCM_SQ, "ACQ CT Image Frame Type Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9330), DCM_FD, "ACQ X-Ray Tube Current in mA"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9332), DCM_FD, "ACQ Exposure in mAs"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9333), DCM_CS, "ACQ Constant Volume Flag"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9334), DCM_CS, "ACQ Fluoroscopy Flag"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9335), DCM_FD, "ACQ Distance Source to Data Collection Center"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9337), DCM_US, "ACQ Contrast/Bolus Agent Number"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9338), DCM_SQ, "ACQ Contrast/Bolus Ingredient Code Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9340), DCM_SQ, "ACQ Contrast Administration Profile Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9341), DCM_SQ, "ACQ Contrast/Bolus Usage Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9342), DCM_CS, "ACQ Contrast/Bolus Agent Administered"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9343), DCM_CS, "ACQ Contrast/Bolus Agent Detected"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9344), DCM_CS, "ACQ Contrast Agent Phase"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9345), DCM_FD, "ACQ CTDlvol"},		/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0x9346), DCM_SQ, "ACQ CTDI Phantom Type Code Sequence"},
    {DCM_MAKETAG(0x0018,0x9351), DCM_FL, "ACQ Calcium Scoring Mass Factor Patient"},
    {DCM_MAKETAG(0x0018,0x9352), DCM_FL, "ACQ Calcium Scoring Mass Factor Device"},
    {DCM_MAKETAG(0x0018,0x9360), DCM_SQ, "ACQ CT Additional X-Ray Source Sequence"},
    {DCM_MAKETAG(0x0018,0x9401), DCM_SQ, "ACQ Projection Pixel Calibration Seq"},
    {DCM_MAKETAG(0x0018,0x9402), DCM_FL, "ACQ Distance Source to Isocenter"},
    {DCM_MAKETAG(0x0018,0x9403), DCM_FL, "ACQ Distance Object to Table Top"},
    {DCM_MAKETAG(0x0018,0x9404), DCM_FL, "ACQ Object Pixel Spacing in Center of Beam"},
    {DCM_MAKETAG(0x0018,0x9405), DCM_SQ, "ACQ Positioner Position Sequence"},
    {DCM_MAKETAG(0x0018,0x9406), DCM_SQ, "ACQ Table Position Sequence"},
    {DCM_MAKETAG(0x0018,0x9407), DCM_SQ, "ACQ Collimator Shape Sequence"},
    {DCM_MAKETAG(0x0018,0x9412), DCM_SQ, "ACQ XA/XRF Frame Characteristics Sequence"},
    {DCM_MAKETAG(0x0018,0x9417), DCM_SQ, "ACQ Frame Acquisition Sequence"},
    {DCM_MAKETAG(0x0018,0x9420), DCM_CS, "ACQ X-Ray Receptor Type"},
    {DCM_MAKETAG(0x0018,0x9423), DCM_LO, "ACQ Acquisition Protocol Name"},
    {DCM_MAKETAG(0x0018,0x9424), DCM_LT, "ACQ Acquisition Protocol Description"},
    {DCM_MAKETAG(0x0018,0x9425), DCM_CS, "ACQ Contrast/Bolus Ingredient Opaque"},
    {DCM_MAKETAG(0x0018,0x9426), DCM_FL, "ACQ Distance Receptor Plane-Detector Housing"},
    {DCM_MAKETAG(0x0018,0x9427), DCM_CS, "ACQ Intensifier Active Shape"},
    {DCM_MAKETAG(0x0018,0x9428), DCM_FL, "ACQ Intensifier Active Shape Dimension(s)"},
    {DCM_MAKETAG(0x0018,0x9429), DCM_FL, "ACQ Physical Detector Size"},
    {DCM_MAKETAG(0x0018,0x9430), DCM_US, "ACQ Position of Isocenter Projection"},
    {DCM_MAKETAG(0x0018,0x9432), DCM_SQ, "ACQ Field of View Sequence"},
    {DCM_MAKETAG(0x0018,0x9433), DCM_LO, "ACQ Field of View Description"},
    {DCM_MAKETAG(0x0018,0x9434), DCM_SQ, "ACQ Exposure Control Sensing Regions Sequence"},
    {DCM_MAKETAG(0x0018,0x9435), DCM_CS, "ACQ Exposure Control Sensing Region Shape"},
    {DCM_MAKETAG(0x0018,0x9436), DCM_SS, "ACQ Exposure Ctrl Sensing Region LT Vert Edge"},
    {DCM_MAKETAG(0x0018,0x9437), DCM_SS, "ACQ Exposure Ctrl Sensing Region RT Vert Edge"},
    {DCM_MAKETAG(0x0018,0x9438), DCM_SS, "ACQ Exposure Ctrl Sensing Region Up Horiz Edge"},
    {DCM_MAKETAG(0x0018,0x9439), DCM_SS, "ACQ Exposure Ctrl Sensing Region Low Horiz Edge"},
    {DCM_MAKETAG(0x0018,0x9440), DCM_SS, "ACQ CTR of Circular Exposure Ctrl Sensing Reg"},
    {DCM_MAKETAG(0x0018,0x9441), DCM_US, "ACQ RAD of Circular Exposure Ctrl Sensing Reg"},
    {DCM_MAKETAG(0x0018,0x9442), DCM_SS, "ACQ Vertices Polygonal Exposure Ctrl Sensing .."},
    {DCM_MAKETAG(0x0018,0x9445), DCM_US, "ACQ Retired without a trace"},
    {DCM_MAKETAG(0x0018,0x9447), DCM_FL, "ACQ Column Angulation (Patient)"},
    {DCM_MAKETAG(0x0018,0x9449), DCM_FL, "ACQ Beam Angle"},
    {DCM_MAKETAG(0x0018,0x9451), DCM_SQ, "ACQ Frame Detector Parameters Sequence"},
    {DCM_MAKETAG(0x0018,0x9452), DCM_FL, "ACQ Calculated Anatomy Thickness"},
    {DCM_MAKETAG(0x0018,0x9455), DCM_SQ, "ACQ Calibration Sequence"},
    {DCM_MAKETAG(0x0018,0x9456), DCM_SQ, "ACQ Object Thickness Sequence"},
    {DCM_MAKETAG(0x0018,0x9457), DCM_CS, "ACQ Plane Identification"},
    {DCM_MAKETAG(0x0018,0x9461), DCM_FL, "ACQ Field of View Dim(s) in Float"},
    {DCM_MAKETAG(0x0018,0x9462), DCM_SQ, "ACQ Isocenter Reference System Seq"},
    {DCM_MAKETAG(0x0018,0x9463), DCM_FL, "ACQ Positioner Isocenter Primary Angle"},
    {DCM_MAKETAG(0x0018,0x9464), DCM_FL, "ACQ Positioner Isocenter Secondary Angle"},
    {DCM_MAKETAG(0x0018,0x9465), DCM_FL, "ACQ Positioner Isoctr Detector Rotation Angle"},
    {DCM_MAKETAG(0x0018,0x9466), DCM_FL, "ACQ Table X Position to Isocenter"},
    {DCM_MAKETAG(0x0018,0x9467), DCM_FL, "ACQ Table Y Position to Isocenter"},
    {DCM_MAKETAG(0x0018,0x9468), DCM_FL, "ACQ Table Z Position to Isocenter"},
    {DCM_MAKETAG(0x0018,0x9469), DCM_FL, "ACQ Table Horizontal Rotation Angle"},
    {DCM_MAKETAG(0x0018,0x9470), DCM_FL, "ACQ Table Head Tile Angle"},
    {DCM_MAKETAG(0x0018,0x9471), DCM_FL, "ACQ Table Cradle Tile Angle"},
    {DCM_MAKETAG(0x0018,0x9472), DCM_SQ, "ACQ Frame Display Shurrer Sequence"},
    {DCM_MAKETAG(0x0018,0x9473), DCM_FL, "ACQ Acquired Image Area Dose Product"},
    {DCM_MAKETAG(0x0018,0x9474), DCM_CS, "ACQ C-arm Posiiton Tabletop Relationship"},
    {DCM_MAKETAG(0x0018,0x9476), DCM_SQ, "ACQ X-Ray Geometry Sequence"},
    {DCM_MAKETAG(0x0018,0x9477), DCM_SQ, "ACQ Irradiation Event Identification Sequence"},
    {DCM_MAKETAG(0x0018,0x9504), DCM_SQ, "ACQ X-Ray 3D Frame Type Sequence"},
    {DCM_MAKETAG(0x0018,0x9506), DCM_SQ, "ACQ Contributing Sources Sequence"},
    {DCM_MAKETAG(0x0018,0x9507), DCM_SQ, "ACQ X-Ray 3D Acquisition Sequence"},
    {DCM_MAKETAG(0x0018,0x9508), DCM_SQ, "ACQ Primary Position Scan Arc"},
    {DCM_MAKETAG(0x0018,0x9509), DCM_FL, "ACQ Secondary Positioner Scan Arc"},
    {DCM_MAKETAG(0x0018,0x9510), DCM_FL, "ACQ Primary Positioner Scan Start Angle"},
    {DCM_MAKETAG(0x0018,0x9511), DCM_FL, "ACQ Secondary Positioner Scan Start Angle"},
    {DCM_MAKETAG(0x0018,0x9514), DCM_FL, "ACQ Primary Positioner Increment"},
    {DCM_MAKETAG(0x0018,0x9515), DCM_FL, "ACQ Tertiary Positioner Increment"},
    {DCM_MAKETAG(0x0018,0x9516), DCM_DT, "ACQ Start Acquisition Date Time"},
    {DCM_MAKETAG(0x0018,0x9517), DCM_TM, "ACQ End Acquisition Date Time"},
    {DCM_MAKETAG(0x0018,0x9524), DCM_LO, "ACQ Application Name"},
    {DCM_MAKETAG(0x0018,0x9525), DCM_LO, "ACQ Application Version"},
    {DCM_MAKETAG(0x0018,0x9526), DCM_LO, "ACQ Application Manufacturer"},
    {DCM_MAKETAG(0x0018,0x9527), DCM_CS, "ACQ Algorithm Type"},
    {DCM_MAKETAG(0x0018,0x9528), DCM_LO, "ACQ Algorithm Description"},
    {DCM_MAKETAG(0x0018,0x9530), DCM_SQ, "ACQ X-Ray 3D Reconstruction Sequence"},
    {DCM_MAKETAG(0x0018,0x9531), DCM_LO, "ACQ Reconstruction Description"},
    {DCM_MAKETAG(0x0018,0x9538), DCM_SQ, "ACQ Per Projection Acquisition Sequence"},
    {DCM_MAKETAG(0x0018,0x9601), DCM_SQ, "ACQ Diffusion b-matrix Sequence"},
    {DCM_MAKETAG(0x0018,0x9602), DCM_FD, "ACQ Diffusion b-value XX"},
    {DCM_MAKETAG(0x0018,0x9603), DCM_FD, "ACQ Diffusion b-value XY"},
    {DCM_MAKETAG(0x0018,0x9604), DCM_FD, "ACQ Diffusion b-value XZ"},
    {DCM_MAKETAG(0x0018,0x9605), DCM_FD, "ACQ Diffusion b-value YY"},
    {DCM_MAKETAG(0x0018,0x9606), DCM_FD, "ACQ Diffusion b-value YZ"},
    {DCM_MAKETAG(0x0018,0x9607), DCM_FD, "ACQ Diffusion b-value ZZ"},

    {DCM_MAKETAG(0x0018,0xA001), DCM_SQ, "ACQ Contributing Equipment Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0xA002), DCM_DT, "ACQ Contribution Date Time"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0018,0xA003), DCM_ST, "ACQ Contribution Description"},	/* 2005.05.17 */
};

/* Define the entries for the RELATIONSHIP group (0020)
*/
static DCMDICT REL_dictionary[] = {
    {DCM_MAKETAG(0x0020, 0x0000), DCM_UL, "REL Group Length"},
    {DCM_MAKETAG(0x0020, 0x000d), DCM_UI, "REL Study Instance UID"},
    {DCM_MAKETAG(0x0020, 0x000e), DCM_UI, "REL Series Instance UID"},
    {DCM_MAKETAG(0x0020, 0x0010), DCM_SH, "REL Study ID"},
    {DCM_MAKETAG(0x0020, 0x0011), DCM_IS, "REL Series Number"},
    {DCM_MAKETAG(0x0020, 0x0012), DCM_IS, "REL Acquisition Number"},
    {DCM_MAKETAG(0x0020, 0x0013), DCM_IS, "REL Instance Number"},

    {DCM_RELISOTOPENUMBER, DCM_IS, "REL Isotope Number (RET)"},
    {DCM_RELPHASENUMBER, DCM_IS, "REL Phase Number (RET)"},
    {DCM_RELINTERVALNUMBER, DCM_IS, "REL Interval Number (RET)"},
    {DCM_RELTIMESLOTNUMBER, DCM_IS, "REL Time Slot Number (RET)"},
    {DCM_RELANGLENUMBER, DCM_IS, "REL Angle Number (RET)"},
    {DCM_MAKETAG(0x0020,0x0019), DCM_IS, "REL Item Number"},

    {DCM_MAKETAG(0x0020, 0x0020), DCM_CS, "REL Patient Orientation"},
    {DCM_RELOVERLAYNUMBER, DCM_IS, "REL Overlay Number"},
    {DCM_RELCURVENUMBER, DCM_IS, "REL Curve Number"},
    {DCM_RELLOOKUPTABLENUMBER, DCM_IS, "REL Looup Table Number"},
    {DCM_RELIMAGEPOSITION, DCM_RET, "REL Image Position (RET)"},
    {DCM_RELIMAGEPOSITIONPATIENT, DCM_DS, "REL Image Position Patient"},
    {DCM_RELIMAGEORIENTATION, DCM_RET, "REL Image Orientation"},
    {DCM_RELIMAGEORIENTATIONPATIENT, DCM_DS, "REL Image Orientation (Patient)"},
    {DCM_RELLOCATION, DCM_RET, "REL Location (RET)"},
    {DCM_RELFRAMEOFREFERENCEUID, DCM_UI, "REL Frame of Reference UID"},
    {DCM_RELLATERALITY, DCM_CS, "REL Laterality"},
    {DCM_MAKETAG(0x0020, 0x0062), DCM_CS, "REL Image Laterality"},
    {DCM_RELIMAGEGEOMETRYTYPE, DCM_RET, "REL Image Geometry Type (RET)"},
    {DCM_RELMASKINGIMAGE, DCM_RET, "REL Masking Image (RET)"},
    {DCM_RELTEMPORALPOSITIONID, DCM_IS, "REL Temporal Position Identifier"},
    {DCM_RELNUMBERTEMPORALPOSITIONS, DCM_IS, "REL Number of Temporal Positions"},
    {DCM_RELTEMPORALRESOLUTION, DCM_DS, "REL Temporal Resolution"},
    {DCM_MAKETAG(0x0020, 0x0200), DCM_UI, "REL Synchronization Frame of Reference UID"},
    {DCM_RELSERIESINSTUDY, DCM_IS, "REL Series in Study"},
    {DCM_RELACQUISITIONSINSERIES, DCM_RET, "REL Acquisitions in Series"},
    {DCM_RELIMAGESINACQUISITION, DCM_IS, "REL Images in Acquisition"},
    {DCM_MAKETAG(0x0020, 0x1003), DCM_IS, "REL Images in Series (RET)"},
    {DCM_MAKETAG(0x0020, 0x1004), DCM_IS, "REL Acquisitions in Study (RET)"},
    {DCM_MAKETAG(0x0020, 0x1005), DCM_IS, "REL Images in Study (RET)"},
    {DCM_RELREFERENCE, DCM_RET, "REL Reference (RET)"},
    {DCM_RELPOSITIONREFINDICATOR, DCM_LO, "REL Position Reference Indicator"},
    {DCM_RELSLICELOCATION, DCM_DS, "REL Slice Location"},
    {DCM_RELOTHERSTUDYNUMBERS, DCM_IS, "REL Other Study Numbers"},
    {DCM_RELNUMBERPATRELATEDSTUDIES, DCM_IS,
    "REL Number of Patient Related Studies"},
    {DCM_RELNUMBERPATRELATEDSERIES, DCM_IS, "REL Number of Patient Related Series"},
    {DCM_RELNUMBERPATRELATEDIMAGES, DCM_IS, "REL Number of Patient Related Instances"},
    {DCM_RELNUMBERSTUDYRELATEDSERIES, DCM_IS, "REL Number of Study Related Series"},
    {DCM_RELNUMBERSTUDYRELATEDIMAGES, DCM_IS, "REL Number of Study Related Instances"},
    {DCM_RELNUMBERSERIESRELATEDINST, DCM_IS, "REL Number of Series Related Instances"},
    {DCM_RELSOURCEIMAGEID, DCM_RET, "REL Source Image IDs (RET)"},
    {DCM_RELMODIFYINGDEVICEID, DCM_RET, "REL Modifying Device ID (RET)"},
    {DCM_RELMODIFIEDIMAGEID, DCM_RET, "REL Modified Image ID (RET)"},
    {DCM_RELMODIFIEDIMAGEDATE, DCM_RET, "REL Modified Image Date (RET)"},
    {DCM_RELMODIFYINGDEVICEMFR, DCM_RET, "REL Modifying Device Mfr (RET)"},
    {DCM_RELMODIFIEDIMAGETIME, DCM_RET, "REL Modified Image Time"},
    {DCM_RELMODIFIEDIMAGEDESCRIPTION, DCM_RET,
    "REL Modified Image Description (RET)"},
    {DCM_RELIMAGECOMMENTS, DCM_LT, "REL Image Comments"},
    {DCM_RELORIGINALIMAGEID, DCM_RET, "REL Original Image ID (RET)"},
    {DCM_RELORIGINALIMAGEIDNOMENCLATURE, DCM_RET,
    "REL Orig Image ID Nomenclature (RET)"},
    {0x00209056, DCM_SH, "REL Stack ID"},	/* 2005.05.17 */
    {0x00209057, DCM_UL, "REL In-Stack Position Number"},	/* 2005.05.17 */
    {0x00209071, DCM_SQ, "REL Frame Anatomy Sequence"},	/* 2005.05.17 */
    {0x00209072, DCM_CS, "REL Frame Laterality"},	/* 2005.05.17 */
    {0x00209111, DCM_SQ, "REL Frame Content Sequence"},	/* 2005.05.17 */
    {0x00209113, DCM_SQ, "REL Plane Position Sequence"},/* 2005.05.17 */
    {0x00209116, DCM_SQ, "REL Place Orientation Sequence"},	/* 2005.05.17 */
    {0x00209128, DCM_UL, "REL Temporal Position Index"},	/* 2005.05.17 */
    {0x00209153, DCM_FD, "REL Trigger Delay Time"},	/* 2005.05.17 */
    {0x00209156, DCM_US, "REL Frame Acquisition Number"},	/* 2005.05.17 */
    {0x00209157, DCM_UL, "REL Dimension Index Values"},	/* 2005.05.17 */
    {0x00209158, DCM_LT, "REL Frame Comments"},	/* 2005.05.17 */
    {0x00209161, DCM_UI, "REL Concatenation UID"},	/* 2005.05.17 */
    {0x00209162, DCM_US, "REL In-concatenation Number"},	/* 2005.05.17 */
    {0x00209163, DCM_US, "REL In-concatenation Total Number"},	/* 2005.05.17 */
    {0x00209164, DCM_UI, "REL Dimension Organization UID"},	/* 2005.05.17 */
    {0x00209165, DCM_AT, "REL Dimension Index Pointer"},	/* 2005.05.17 */
    {0x00209167, DCM_AT, "REL Functional Group Pointer"},	/* 2005.05.17 */
    {0x00209213, DCM_LO, "REL Dimension Index Private Creator"},	/* 2005.05.17 */
    {0x00209221, DCM_SQ, "REL Dimension Organization Sequence"},	/* 2005.05.17 */
    {0x00209222, DCM_SQ, "REL Dimension Index Sequence"},	/* 2005.05.17 */
    {0x00209228, DCM_UL, "REL Concatenation Frame Offset Number"},	/* 2005.05.17 */
    {0x00209238, DCM_LO, "REL Functional Group Private Creator"},	/* 2005.05.17 */

    {DCM_MAKETAG(0x0020, 0x9241), DCM_FL, "REL Nominal Percentage of Cardiac Phase"},
    {DCM_MAKETAG(0x0020, 0x9245), DCM_FL, "REL Nominal Percentage of Respiratory Phase"},
    {DCM_MAKETAG(0x0020, 0x9246), DCM_FL, "REL Starting Respiratory Amplitude"},
    {DCM_MAKETAG(0x0020, 0x9247), DCM_CS, "REL Starting Respiratory Phase"},
    {DCM_MAKETAG(0x0020, 0x9248), DCM_FL, "REL Ending Respiratory Amplidude"},
    {DCM_MAKETAG(0x0020, 0x9249), DCM_CS, "REL Ending Respiratory Phase"},
    {DCM_MAKETAG(0x0020, 0x9250), DCM_CS, "REL Respiratory Trigger Type"},
    {DCM_MAKETAG(0x0020, 0x9251), DCM_FD, "REL R-R Interval Time Nominal"},
    {DCM_MAKETAG(0x0020, 0x9252), DCM_FD, "REL Actual Cardiac Trigger Delay Time"},
    {DCM_MAKETAG(0x0020, 0x9253), DCM_SQ, "REL Respiratory Synchronization Sequence"},
    {DCM_MAKETAG(0x0020, 0x9254), DCM_FD, "REL Respiratory Interval Time"},
    {DCM_MAKETAG(0x0020, 0x9255), DCM_FD, "REL Nominal Respiratory Trigger Delay Time"},
    {DCM_MAKETAG(0x0020, 0x9256), DCM_FD, "REL Respiratory Trigger Delay Threshold"},
    {DCM_MAKETAG(0x0020, 0x9257), DCM_FD, "REL Actual Respiratory Trigger Delay Time"},
    {DCM_MAKETAG(0x0020, 0x9421), DCM_LO, "REL Dimension Description Label"},
    {DCM_MAKETAG(0x0020, 0x9450), DCM_SQ, "REL Patient Orientation in Frame Sequence"},
    {DCM_MAKETAG(0x0020, 0x9453), DCM_LO, "REL Frame Label"},
    {DCM_MAKETAG(0x0020, 0x9518), DCM_US, "REL Acquisition Index"},
    {DCM_MAKETAG(0x0020, 0x9529), DCM_SQ, "REL Contributing SOP Instances Reference Seq"},
    {DCM_MAKETAG(0x0020, 0x9536), DCM_US, "REL Reconstruction Index"}
};

/* Define the entries for the IMAGE group (0028)
*/
static DCMDICT IMG_dictionary[] = {
    {DCM_IMGGROUPLENGTH, DCM_UL, "IMG Group Length"},
    {DCM_IMGSAMPLESPERPIXEL, DCM_US, "IMG Samples Per Pixel"},
    {DCM_IMGPHOTOMETRICINTERP, DCM_CS, "IMG Photometric Interpretation"},
    {DCM_IMGIMAGEDIMENSIONS, DCM_RET, "IMG Image Dimensions (RET)"},
    {DCM_IMGPLANARCONFIGURATION, DCM_US, "IMG Planar Configuration"},
    {DCM_IMGNUMBEROFFRAMES, DCM_IS, "IMG Number of Frames"},
    {DCM_IMGFRAMEINCREMENTPOINTER, DCM_AT, "IMG Frame Increment Pointer"},
    {DCM_IMGROWS, DCM_US, "IMG Rows"},
    {DCM_IMGCOLUMNS, DCM_US, "IMG Columns"},
    {DCM_IMGPLANES, DCM_US, "IMG Planes"},
    {DCM_IMGUSOUNDCOLORDATAPRESENT, DCM_US, "IMG Ultrasound Color Data Present"},
    {DCM_IMGPIXELSPACING, DCM_DS, "IMG Pixel Spacing"},
    {DCM_IMGZOOMFACTOR, DCM_DS, "IMG Zoom Factor"},
    {DCM_IMGZOOMCENTER, DCM_DS, "IMG Zoom Center"},
    {DCM_IMGPIXELASPECTRATIO, DCM_IS, "IMG Pixel Aspect Ratio"},
    {DCM_IMGIMAGEFORMAT, DCM_RET, "IMG Image Format (RET)"},
    {DCM_IMGMANIPULATEDIMAGE, DCM_RET, "IMG Manipulated Image (RET)"},
    {DCM_IMGCORRECTEDIMAGE, DCM_CS, "IMG Corrected Image"},
    {DCM_IMGCOMPRESSIONCODE, DCM_RET, "IMG Compression Code"},
    {DCM_IMGBITSALLOCATED, DCM_US, "IMG Bits Allocated"},
    {DCM_IMGBITSSTORED, DCM_US, "IMG Bits Stored"},
    {DCM_IMGHIGHBIT, DCM_US, "IMG High Bit"},
    {DCM_IMGPIXELREPRESENTATION, DCM_US, "IMG Pixel Representation"},
    {DCM_IMGSMALLESTPIXELVALUE, DCM_RET, "IMG Smallest Pixel Value (RET)"},
    {DCM_IMGLARGESTPIXELVALUE, DCM_RET, "IMG Largest Pixel Vaue (RET)"},
    {DCM_IMGSMALLESTIMAGEPIXELVALUE, DCM_CTX, "IMG Smallest Image Pixel Value"},
    {DCM_IMGLARGESTIMAGEPIXELVALUE, DCM_CTX, "IMG Largest Image Pixel Value"},
    {DCM_IMGSMALLESTPIXELVALUESERIES, DCM_CTX, "IMG Smallest Pixel Value in Series"},
    {DCM_IMGLARGESTPIXELVALUESERIES, DCM_CTX, "IMG Largest Pixel Value in Series"},
    {DCM_IMGSMALLESTIMAGEPIXELVALUEPLANE, DCM_CTX, "IMG Smallest Pixel Value in Plane"},
    {DCM_IMGLARGESTIMAGEPIXELVALUEPLANE, DCM_CTX, "IMG Largest Pixel Value in Plane"},
    {DCM_IMGPIXELPADDINGVALUE, DCM_CTX, "IMG Pixel Padding Value"},
    {DCM_MAKETAG(0x0028, 0x0121), DCM_CTX, "IMG Pixel Padding Range Limit"},
    {DCM_IMGWAVEFORMPADDINGVALUE, DCM_CTX, "IMG Waveform Padding Value"},	/* Sup 30 0.6 */
    {DCM_IMGIMAGELOCATION, DCM_RET, "IMG Image Location"},
    {DCM_MAKETAG(0x0028, 0x0300), DCM_CS, "IMG Quality Control Image"},
    {DCM_MAKETAG(0x0028, 0x0301), DCM_CS, "IMG Burned In Annotation"},
    {DCM_IMGPIXELINTENSITYRELATIONSHIP, DCM_CS, "IMG Pixel Intensity Relationship"},
    {DCM_MAKETAG(0x0028, 0x1041), DCM_SS, "IMG Pixel Intensity Relationship Sign"},
    {DCM_IMGWINDOWCENTER, DCM_DS, "IMG Window Center"},
    {DCM_IMGWINDOWWIDTH, DCM_DS, "IMG Window Width"},
    {DCM_IMGRESCALEINTERCEPT, DCM_DS, "IMG Rescale Intercept"},
    {DCM_IMGRESCALESLOPE, DCM_DS, "IMG Rescale Slope"},
    {DCM_IMGRESCALETYPE, DCM_LO, "IMG Rescale Type"},
    {DCM_IMGWINDOWCWEXPLANATION, DCM_LO, "IMG Window Center & Width Explanation"},
    {DCM_IMGGRAYSCALE, DCM_RET, "IMG Gray Scale (RET)"},
    {DCM_IMGRECOMMENDEDVIEWINGMODE, DCM_CS, "IMG Recommended Viewing Mode"},
    {DCM_IMGLUTDESCRIPTGRAY, DCM_RET, "IMG Lookup Table Desc-Gray (RET)"},
    {DCM_IMGLUTDESCRIPTRED, DCM_US, "IMG Lookup Table Desc-Red"},
    {DCM_IMGLUTDESCRIPTGREEN, DCM_US, "IMG Lookup Table Desc-Green"},
    {DCM_IMGLUTDESCRIPTBLUE, DCM_US, "IMG Lookup Table Desc-Blue"},
    {DCM_IMGPALETTECOLORLUTUID, DCM_UI, "IMG Palette Color Lookup Table UID"},
    {DCM_IMGLOOKUPDATAGRAY, DCM_RET, "IMG Lookup Data-Gray"},
    {DCM_MAKETAG(0x0028, 0x1350), DCM_CS, "MAMMO IMG Partial View"},
    {DCM_MAKETAG(0x0028, 0x1352), DCM_SQ, "MAMMO IMG Partial View Code Sequence"},
    {DCM_MAKETAG(0x0028, 0x135A), DCM_CS, "Spatial Locations Preserved"},

#if 0
    /* As originally defined in 1993 */
    {DCM_IMGLOOKUPDATARED, DCM_US, "IMG Lookup Data-Red"},
    {DCM_IMGLOOKUPDATAGREEN, DCM_US, "IMG Lookup Data-Green"},
    {DCM_IMGLOOKUPDATABLUE, DCM_US, "IMG Lookup Data-Blue"},
#endif

    {DCM_IMGLOOKUPDATARED, DCM_CTX, "IMG Lookup Data-Red"},
    {DCM_IMGLOOKUPDATAGREEN, DCM_CTX, "IMG Lookup Data-Green"},
    {DCM_IMGLOOKUPDATABLUE, DCM_CTX, "IMG Lookup Data-Blue"},

    {DCM_IMGSEGMENTEDREDLUTDATA, DCM_OW, "IMG Segmented Red Palette Color LUT Data"},
    {DCM_IMGSEGMENTEDGREENLUTDATA, DCM_OW, "IMG Segmented Green Palette Color LUT Data"},
    {DCM_IMGSEGMENTEDBLUELUTDATA, DCM_OW, "IMG Segmented Blue Palette Color LUT Data"},
    {DCM_MAKETAG(0x0028, 0x1250), DCM_CS, "Partial View"},
    {DCM_MAKETAG(0x0028, 0x1251), DCM_ST, "Partial View Description"},
    {DCM_MAKETAG(0x0028, 0x1252), DCM_SQ, "Partial View Code Sequence"},
    {DCM_MAKETAG(0x0028, 0x2000), DCM_OB, "IMG ICC Profile"}, /* Supp 100 */
    {DCM_IMGLOSSYIMAGECOMPRESSION, DCM_CS, "IMG Lossy Image Compression"},
    {DCM_IMGMODALITYLUTSEQUENCE, DCM_SQ, "IMG Modality LUT Sequence"},
    {DCM_IMGLUTDESCRIPTOR, DCM_CTX, "IMG LUT Descriptor"},
    {DCM_IMGLUTEXPLANATION, DCM_LO, "IMG LUT Explanation"},
    {DCM_IMGMODALITYLUTTYPE, DCM_LO, "IMG Modality LUT Type"},
    {DCM_IMGLUTDATA, DCM_CTX, "IMG LUT Data"},
    {DCM_IMGVOILUTSEQUENCE, DCM_SQ, "IMG VOI LUT Sequence"},
    {DCM_IMGSOFTCOPYVOILUTSEQUENCE, DCM_SQ, "IMG Softcopy VOI LUT Sequence"},
    {DCM_IMGCOMMENTS, DCM_RET, "IMG Comments (RET)"},
    {DCM_IMGBIPLANEACQSEQUENCE, DCM_SQ, "IMG Bi-Plane Acquisition Sequence"},
    {DCM_IMGREPRESENTATIVEFRAMENUMBER, DCM_US, "IMG Representative Frame Number"},
    {DCM_IMGFRAMENUMBERSOFINTEREST, DCM_US, "IMG Frame Numbers of Interest"},
    {DCM_IMGFRAMEOFINTERESTDESCRIPTION, DCM_LO, "IMG Frame of Interest Description"},
    {DCM_IMGMASKPOINTER, DCM_US, "IMG Mask Pointer(s)"},
    {DCM_IMGRWAVEPOINTER, DCM_US, "IMG R Wave Pointer"},
    {DCM_IMGMASKSUBTRACTIONSEQ, DCM_SQ, "IMG Mask Subtraction Sequence"},
    {DCM_IMGMASKOPERATION, DCM_CS, "IMG Mask Operation"},
    {DCM_IMGAPPLICABLEFRAMERANGE, DCM_US, "IMG Applicable Frame Range"},
    {DCM_IMGMASKFRAMENUMBERS, DCM_US, "IMG Mask Frame Numbers"},
    {DCM_IMGCONTRASTFRAMEAVERAGING, DCM_US, "IMG Contrast Frame Averaging"},
    {DCM_IMGMASKSUBPIXELSHIFT, DCM_FL, "IMG Mask Sub-pixel shift"},
    {DCM_IMGTIDOFFSET, DCM_SS, "IMG TID Offset"},
    {DCM_MASKOPERATIONEXPLANATION, DCM_ST, "IMG Mask Operation Explanation"},
    {0x00289001, DCM_UL, "IMG Data Point Rows"},	/* 2005.05.17 */
    {0x00289002, DCM_UL, "IMG Data Point Columns"},	/* 2005.05.17 */
    {0x00289003, DCM_CS, "IM Signal Domain Columns"},	/* 2005.05.17 */
    {0x00289099, DCM_US, "IMG Lagent Monochrome Pixel Value - RET"},	/* 2005.05.17 */
    {0x00289108, DCM_CS, "Data Representation"},	/* 2005.05.17 */
    {0x00289110, DCM_SQ, "Pixel Measures Sequence"},	/* 2005.05.17 */
    {0x00289132, DCM_SQ, "IMG Frame VOI LUT Sequence"},	/* 2005.05.17 */
    {0x00289145, DCM_SQ, "IMG Pixel Value Transformation Sequence"},	/* 2005.05.17 */
    {0x00289235, DCM_CS, "IMG Signal Domain Rows"},	/* 2005.05.17 */
};

/* Define the entries for the STUDY group (0032)
*/
static DCMDICT SDY_dictionary[] = {
    {DCM_SDYGROUPLENGTH, DCM_UL, "SDY Study Group length"},
    {DCM_SDYSTATUSID, DCM_CS, "SDY Study Status ID"},
    {DCM_SDYPRIORITYID, DCM_CS, "SDY Study Priority ID"},
    {DCM_SDYIDISSUER, DCM_LO, "SDY Study ID Issuer"},
    {DCM_SDYVERIFIEDDATE, DCM_DA, "SDY Study Verified Date"},
    {DCM_SDYVERIFIEDTIME, DCM_TM, "SDY Study Verified Time"},
    {DCM_SDYREADDATE, DCM_DA, "SDY Study Read Date"},
    {DCM_SDYREADTIME, DCM_TM, "SDY Study Read Time"},
    {DCM_SDYSCHEDULEDSTARTDATE, DCM_DA, "SDY Scheduled Study Start Date"},
    {DCM_SDYSCHEDULEDSTARTTIME, DCM_TM, "SDY Scheduled Study Start Time"},
    {DCM_SDYSCHEDULEDSTOPDATE, DCM_DA, "SDY Scheduled Study Stop Date"},
    {DCM_SDYSCHEDULEDSTOPTIME, DCM_TM, "SDY Scheduled Study Stop Time"},
    {DCM_SDYSCHEDULEDLOCATION, DCM_LO, "SDY Scheduled Study Location"},
    {DCM_SDYSCHEDULEDLOCATIONAETITLE, DCM_AE,
    "SDY Scheduled Study Location AE Title(s)"},
    {DCM_SDYREASON, DCM_LO, "SDY Study Reason"},
    {DCM_SDYREQUESTINGPHYSICIAN, DCM_PN, "SDY Requesting Physician "},
    {DCM_SDYREQUESTINGSERVICE, DCM_LO, "SDY Requesting Service"},
    {DCM_SDYARRIVALDATE, DCM_DA, "SDY Study Arrival Date"},
    {DCM_SDYARRIVALTIME, DCM_TM, "SDY Study Arrival Time"},
    {DCM_SDYCOMPLETIONDATE, DCM_DA, "SDY Study Completion Date"},
    {DCM_SDYCOMPLETIONTIME, DCM_TM, "SDY Study Completion Time"},
    {DCM_SDYSTUDYCOMPONENTSTATUSID, DCM_CS, "SDY Study Component Status ID"},
    {DCM_SDYREQUESTEDPRODESCRIPTION, DCM_LO, "SDY Requested Procedure Description"},
    {DCM_SDYREQUESTEDPROCODESEQ, DCM_SQ, "SDY Requested Procedure Code Seq"},
    {DCM_SDYREQUESTEDCONTRASTAGENT, DCM_LO, "SDY Requested Contrast Agent"},
    {DCM_SDYCOMMENTS, DCM_LT, "SDY Comments"}
};

/* Define the entries for the VISIT group, 0038
*/
static DCMDICT VIS_dictionary[] = {
    {DCM_VISGROUPLENGTH, DCM_UL, "VIS Group Length"},
    {DCM_VISREFERENCEDPATALIASSEQ, DCM_SQ, "VIS Referenced Patient Alias Sequence"},
    {DCM_VISSTATUSID, DCM_CS, "VIS Visit Status ID"},
    {DCM_VISADMISSIONID, DCM_LO, "VIS Admission ID"},
    {DCM_VISISSUEROFADMISSIONID, DCM_LO, "VIS Issuer of Admission ID"},
    {DCM_VISROUTEOFADMISSION, DCM_LO, "VIS Route of Admission"},
    {DCM_VISSCHEDULEDADMISSIONDATE, DCM_DA, "VIS Scheduled Admission Date"},
    {DCM_VISSCHEDULEDADMISSIONTIME, DCM_TM, "VIS Scheduled Admission Time"},
    {DCM_VISSCHEDULEDDISCHARGEDATE, DCM_DA, "VIS Scheduled Discharge Date"},
    {DCM_VISSCHEDULEDDISCHARGETIME, DCM_TM, "VIS Scheduled Discharge Time"},
    {DCM_VISSCHEDULEDPATINSTRESIDENCE, DCM_LO, "VIS Scheduled Patient Institution Residence"},
    {DCM_VISADMITTINGDATE, DCM_DA, "VIS Admitting Date"},
    {DCM_VISADMITTINGTIME, DCM_TM, "VIS Admitting Time"},
    {DCM_VISDISCHARGEDATE, DCM_DA, "VIS Discharge Date"},
    {DCM_VISDISCHARGETIME, DCM_TM, "VIS Discharge Time"},
    {DCM_VISDISCHARGEDIAGDESCRIPTION, DCM_LO, "VIS Discharge Diagnosis Description"},
    {DCM_VISDISCHARGEDIAGNOSISCODESEQ, DCM_SQ, "VIS Discharge Diagnosis Code Sequence"},
    {DCM_VISSPECIALNEEDS, DCM_LO, "VIS Special Needs"},
    {DCM_VISCURRENTPATIENTLOCATION, DCM_LO, "VIS Current Patient Location"},
    {DCM_VISPATIENTSINSTRESIDENCE, DCM_LO, "VIS Patient's Institution Residence"},
    {DCM_VISPATIENTSTATE, DCM_LO, "VIS Patient State"},
    {DCM_VISCOMMENTS, DCM_LT, "VIS Comments"}
};

/* Define the entries for the Docuemnt group, 0042 */
static DCMDICT DOC_dictionary[] = {
    {0x00420000, DCM_UL, "DOC Document Group length"},
    {0x00420010, DCM_ST, "DOC Document Title"},
    {0x00420011, DCM_OB, "DOC Encapsulated Document "},
    {0x00420012, DCM_LO, "DOC MIME Type of Encapsulated Document"},
    {0x00420013, DCM_SQ, "DOC Source Instance Sequence"},
};

/* Define the entries for the Waveform group, 003a
*/
static DCMDICT WAV_dictionary[] = {
    {DCM_MAKETAG(0x003a, 0x0000), DCM_UL, "WAV Group Length"},
    {DCM_MAKETAG(0x003a, 0x0002), DCM_SQ, "WAV Waveform Sequence"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x0005), DCM_US, "WAV Number of Channels"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x0010), DCM_UL, "WAV Number of Samples"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x001a), DCM_DS, "WAV Sampling Frequency"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x0020), DCM_SH, "WAV Group Label"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x0103), DCM_CS, "WAV Data Value Representation"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x0200), DCM_SQ, "WAV Channel Definition"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x0202), DCM_IS, "WAV Channel Number"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x0203), DCM_SH, "WAV Channel Label"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x0205), DCM_CS, "WAV Channel Status"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x0208), DCM_SQ, "WAV Waveform Source"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x0209), DCM_SQ, "WAV Waveform Source Modifiers"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x020a), DCM_SQ, "WAV Differential Waveform Source"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x020b), DCM_SQ, "WAV Differential Waveform Source Modifiers"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x0210), DCM_DS, "WAV Channel Sensitivity"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x0211), DCM_SQ, "WAV Channel Sensitivity Units"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x0212), DCM_DS, "WAV Channel Sensitivity Correction Factor"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x0213), DCM_DS, "WAV Channel Baseline"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x0214), DCM_DS, "WAV Channel Time Skew"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x0215), DCM_DS, "WAV Channel Sample Skew"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x0218), DCM_DS, "WAV Channel Offset"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x021a), DCM_US, "WAV Bits Per Sample"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x0216), DCM_CTX, "WAV Channel Minimum Value"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x0217), DCM_CTX, "WAV Channel Maximum Value"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x0220), DCM_DS, "WAV Filter Low Frequency"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x0221), DCM_DS, "WAV Filter High Frequency"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x0222), DCM_DS, "WAV Notch Filter Frequency"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x0223), DCM_DS, "WAV Notch Filter Bandwidth"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x003a, 0x1000), DCM_CTX, "WAV Waveform Data"}	/* Sup 30 0.6 */
};

/* Define the entries for the Procedure Step group, 0040
*/

static DCMDICT PRC_dictionary[] = {
    {DCM_PRCGROUPLENGTH, DCM_UL, "PRC Group Length"},
    {DCM_PRCSCHEDULEDSTATIONAETITLE, DCM_AE, "PRC Scheduled Station AE Title"},
    {DCM_PRCSCHEDULEDPROCSTEPSTARTDATE, DCM_DA, "PRC Scheduled Procedure Step Start Date"},
    {DCM_PRCSCHEDULEDPROCSTEPSTARTTIME, DCM_TM, "PRC Scheduled Procedure Step Start Time"},
    {DCM_PRCSCHEDULEDPROCSTEPENDDATE, DCM_DA, "PRC Scheduled Procedure Step End Date"},
    {DCM_PRCSCHEDULEDPROCSTEPENDTIME, DCM_TM, "PRC Scheduled Procedure Step End Time"},
    {DCM_PRCSCHEDULEDPERFORMINGPHYSNAME, DCM_PN, "PRC Scheduled Performing Physician's Name"},
    {DCM_PRCSCHEDULEDPROCSTEPDESCRIPTION, DCM_LO, "PRC Scheduled Step Description"},
    {DCM_PRCSCHEDULEDACTIONITEMCODESEQ, DCM_SQ, "PRC Scheduled Action Item Code Sequence"},
    {DCM_PRCSCHEDULEDPROCSTEPID, DCM_SH, "PRC Scheduled Procedure Step ID"},
    {DCM_PRCSCHEDULEDSTATIONNAME, DCM_SH, "PRC Scheduled Station Name"},
    {DCM_PRCSCHEDULEDPROCSTEPLOCATION, DCM_SH, "PRC Scheduled Procedure Step Location"},
    {DCM_PRCPREMEDICATION, DCM_LO, "PRC Pre-Medication"},
    {DCM_PRCSTATUS, DCM_CS, "PRC SPStep Status"},
	{DCM_MAKETAG(0x0040, 0x0026), DCM_SQ, "PRC Order Placer Identifier Sequence"},
	{DCM_MAKETAG(0x0040, 0x0027), DCM_SQ, "PRC Order Filler Identifier Sequence"},
	{DCM_MAKETAG(0x0040, 0x0031), DCM_UT, "PRC Local Namespace Entity ID"},
	{DCM_MAKETAG(0x0040, 0x0032), DCM_UT, "PRC Universal Entity ID"},
	{DCM_MAKETAG(0x0040, 0x0033), DCM_CS, "PRC Universal Entity ID Type"},
	{DCM_MAKETAG(0x0040, 0x0035), DCM_CS, "PRC Identifier Type Code"},
	{DCM_MAKETAG(0x0040, 0x0036), DCM_SQ, "PRC Assigning Facility Sequence"},
	{DCM_MAKETAG(0x0040, 0x0039), DCM_SQ, "PRC Assigning Jurisdiction Code Sequence"},
	{DCM_MAKETAG(0x0040, 0x003A), DCM_SQ, "PRC Assigning Agency or Department Code Seq"},
	{DCM_MAKETAG(0x0040, 0x0100), DCM_SQ, "PRC Scheduled Procedure Step Sequence"},
    {DCM_PRCREFSTANDALONESOPSEQ, DCM_SQ, "PRC Ref Standalone SOP Inst Seq"},
    {DCM_PRCPERFORMEDSTATIONAET, DCM_AE, "PRC Performed Station AE Title"},
    {DCM_PRCPERFORMEDSTATIONNAME, DCM_SH, "PRC Performed Station Name"},
    {DCM_PRCPERFORMEDLOCATION, DCM_SH, "PRC Performed Location"},
    {DCM_PRCPPSSTARTDATE, DCM_DA, "PRC PPS Start Date"},
    {DCM_PRCPPSSTARTTIME, DCM_TM, "PRC PPS Start Time"},
    {DCM_PRCPPSENDDATE, DCM_DA, "PRC PPS End Date"},
    {DCM_PRCPPSENDTIME, DCM_TM, "PRC PPS End Time"},
    {DCM_PRCPPSSTATUS, DCM_CS, "PRC PPS Status"},
    {DCM_PRCPPSID, DCM_SH, "PRC PPS ID"},
    {DCM_PRCPPSDESCRIPTION, DCM_LO, "PRC PPS Description"},
    {DCM_PRCPPTYPEDESCRIPTION, DCM_LO, "PRC Perf Procedure Type Description"},
    {DCM_PRCPERFORMEDAISEQUENCE, DCM_SQ, "PRC Perf AI Sequence"},
    {DCM_PRCSCHEDSTEPATTRSEQ, DCM_SQ, "PRC Scheduled Step Attr Seq"},
    {DCM_PRCREQUESTATTRIBUTESSEQ, DCM_SQ, "PRC Request Attributes Seq"},
    {DCM_PRCCOMMENTSPPS, DCM_ST, "PRC Comments on PPS"},
    {DCM_MAKETAG(0x0040, 0x0281), DCM_SQ, "PRC PPS Discontinuation Reason"},
    {DCM_PRCQUANTITYSEQ, DCM_SQ, "PRC Quantity Sequence"},
    {DCM_PRCQUANTITY, DCM_DS, "PRC Quantity"},
    {DCM_PRCMEASURINGUNITSSEQ, DCM_SQ, "PRC Measuring Units Sequence"},
    {DCM_PRCBILLINGITEMSEQ, DCM_SQ, "PRC Billing Item Seq"},
    {DCM_PRCTOTALTIMEFLUOROSCOPY, DCM_US, "PRC Total Time Fluoroscopy"},
    {DCM_PRCTOTALNUMBEREXPOSURES, DCM_US, "PRC Total Number Exposures"},
    {DCM_PRCENTRANCEDOSE, DCM_US, "PRC Entrance Dose"},
    {DCM_PRCEXPOSEDAREA, DCM_US, "PRC Exposed Area"},
    {DCM_PRCDISTANCESOURCEENTRANCE, DCM_DS, "PRC Distance Source to Entrance"},
    {DCM_PRCCOMMENTSRADIATIONDOSE, DCM_ST, "PRC Comments on Radiation Dose"},

    {0x00400312, DCM_DS, "PRC X-Ray Output"},		/* 2002.04.26 */
    {0x00400314, DCM_DS, "PRC Half Value Layer"},	/* 2002.04.26 */
    {0x00400316, DCM_DS, "PRC Organ Dose"},		/* 2002.04.26 */
    {0x00400318, DCM_CS, "PRC Organ Exposed"},		/* 2002.04.26 */

    {DCM_PRCBILLINGPROCEDURESTEPSEQ, DCM_SQ, "PRC Billing Proc Step Seq"},
    {DCM_PRCFILMCONSUMPTIONSEQ, DCM_SQ, "PRC Film Consumption Seq"},
    {DCM_PRCBILLINGSUPPLIESDEVICESEQ, DCM_SQ, "PRC Billing Supplies/Devices Seq"},
    {DCM_PRCREFERENCEDPPS, DCM_SQ, "PRC Ref Procedure Step Seq"},
    {DCM_PRCPERFORMEDSERIESSEQ, DCM_SQ, "PRC Performed Series Seq"},
    {DCM_PRCSCHEDULEDPROCSTEPSEQ, DCM_SQ, "PRC Scheduled Procedure Step Sequence"},
    {DCM_PRCCOMMENTSONSCHEDULEDPROCSTEP, DCM_LT, "PRC Comments on the Scheduled Procedure Step"},
    {DCM_MAKETAG(0x0040, 0x050a), DCM_LO, "PRC Specimen Accession Number"},	/* Sup 15 */
    {DCM_MAKETAG(0x0040, 0x0550), DCM_SQ, "PRC Specimen Sequence"},	/* Sup 15 */
    {DCM_MAKETAG(0x0040, 0x0551), DCM_LO, "PRC Specimen Identifier"},	/* Sup 15 */
    {DCM_MAKETAG(0x0040, 0x0552), DCM_SQ, "PRC Specimen Description Sequence"},	/* Sup 15 */
    {DCM_MAKETAG(0x0040, 0x0553), DCM_ST, "PRC Specimen Description"},	/* Sup 15 */
    {DCM_MAKETAG(0x0040, 0x0555), DCM_SQ, "PRC Acquisition Context Sequence"},	/* Sup 15 */
    {DCM_MAKETAG(0x0040, 0x0556), DCM_ST, "PRC Acquisition Context Description"},	/* Sup 15 */
    {DCM_MAKETAG(0x0040, 0x059a), DCM_SQ, "PRC Specimen Type Code Sequence"},	/* Sup 15 */
    {DCM_MAKETAG(0x0040, 0x06fa), DCM_LO, "PRC Slide Identifier"},	/* Sup 15 */
    {DCM_MAKETAG(0x0040, 0x071a), DCM_SQ, "PRC Image Center Point Coordinates Sequence"},	/* Sup 15 */
    {DCM_MAKETAG(0x0040, 0x072a), DCM_DS, "PRC X offset in Slide Coordinate System"},	/* Sup 15 */
    {DCM_MAKETAG(0x0040, 0x073a), DCM_DS, "PRC Y offset in Slide Coordinate System"},	/* Sup 15 */
    {DCM_MAKETAG(0x0040, 0x074a), DCM_DS, "PRC Z offset in Slide Coordinate System"},	/* Sup 15 */
    {DCM_MAKETAG(0x0040, 0x08d8), DCM_SQ, "PRC Pixel Spacing Sequence"},	/* Sup 15 */
    {DCM_MAKETAG(0x0040, 0x08da), DCM_SQ, "PRC Coordinate System Axis Code Sequence"},	/* Sup 15 */
    {DCM_MAKETAG(0x0040, 0x08ea), DCM_SQ, "PRC Measurement Units Code Sequence"},	/* Sup 15 */
    {DCM_MAKETAG(0x0040, 0x09f8), DCM_SQ, "PRC Vital Stain Code Sequence"},	/* Sup 15 */
    {DCM_PRCREQUESTEDPROCEDUREID, DCM_SH, "PRC Requested Procedure ID"},
    {DCM_PRCREASONFORREQUESTEDPROC, DCM_LO, "PRC Reason for the Requested Procedure"},
    {DCM_PRCREQUESTEDPROCPRIORITY, DCM_SH, "PRC Patient Transport Arrangements"},
    {DCM_PRCPATIENTTRANSPORTARRANGEMENTS, DCM_LO, "PRC Patient Transport Arrangements"},
    {DCM_PRCREQUESTEDPROCLOCATION, DCM_LO, "PRC Requested Procedure Location"},
    {DCM_PRCPLACERORDERNUMBERPROC, DCM_SH, "PRC Placer Order Number / Procedure"},

    {DCM_PRCFILLERORDERNUMBERPROC, DCM_SH, "PRC Filler Order Number / Procedure"},
    {DCM_PRCCONFIDENTIALITYCODE, DCM_LO, "PRC Confidentiality Code"},
    {DCM_PRCREPORTINGPRIORITY, DCM_SH, "PRC  Reporting Priority"},
    {DCM_PRCNAMESINTENDEDRECIPIENTSRESULTS, DCM_PN, "PRC Names of Intended Recipients of Results"},
    {DCM_PRCREQUESTEDPROCCOMMENTS, DCM_LT, "PRC Requested Procedure Comments"},
    {DCM_PRCREASONFORIMAGINGSERVICEREQ, DCM_LO, "PRC Reason for teh Imaging Service Request"},
    {DCM_PRCISSUEDATEIMAGINGSERVICEREQ, DCM_DA, "PRC Issue Date of Imaging Service Request"},
    {DCM_PRCISSUETIMEIMAGINGSERVICEREQ, DCM_TM, "PRC Issue Time of Imaging Service Request"},
    {DCM_PRCPLACERORDERNUMBERIMAGINGSRVREQ, DCM_SH, "PRC Placer Order Number/Imaging Service Request"},
    {DCM_PRCFILLERORDERNUMBERIMAGINGSRVREQ, DCM_SH, "PRC Filler Order Number/Imaging Service Request"},
    {DCM_PRCORDERENTEREDBY, DCM_PN, "PRC Order Entered By"},
    {DCM_PRCORDERENTERERSLOCATION, DCM_SH, "PRC Order Enterer's Location"},
    {DCM_PRCORDERCALLBACKPHONENUMBER, DCM_SH, "PRC Order Callback Phone Number"},
    {DCM_MAKETAG(0x0040, 0x2016), DCM_LO, "PRC Placer Order Number/ISR"},
    {DCM_MAKETAG(0x0040, 0x2017), DCM_LO, "PRC Filler Order Number/ISR"},

    {DCM_PRCIMAGINGSERVICEREQCOMMENTS, DCM_LT, "PRC Imaging Service Request Comments"},
    {DCM_PRCCONFIDIENTIALITYCONSTRAINTPATIENTDATADES, DCM_LO, "PRC Confidientiality Constraint Patient Data..."},

    {DCM_PRCGPSPSSTATUS, DCM_CS, "PRC General Purpose Sched Procedure Step Status"},
    {DCM_PRCGPPPSSTATUS, DCM_CS, "PRC Gen. Purpose Perf Procedure Step Status"},
    {DCM_PRCGPSPSPRIORITY, DCM_CS, "PRC Gen. Purpose Sched Procedure Step Priority"},
    {DCM_PRCSCHEDULEDPROCAPPCODESEQ, DCM_SQ, "PRC Scheduled Proccessing Application Code Seq"},
    {DCM_PRCGPSPSSTARTDATETIME, DCM_DT, "PRC Sched Procedure Step Start Date and Time"},
    {DCM_PRCGPSPSMULTIPLECOPIESFLAG, DCM_CS, "PRC Multiple Copies Flag"},
    {DCM_PRCPERFORMEDPROCAPPCODESEQ, DCM_SQ, "PRC Performed Proccessing Applications Code Seq"},
    {DCM_PRCHUMANPERFORMERCODESEQ, DCM_SQ, "PRC Human Performer Code Sequence"},
    {DCM_MAKETAG(0x0040,0x4010), DCM_DT, "PRC SPS Modification Date/Time"},
    {DCM_PRCGPSPSEXPECTEDCOMPLETEDATETIME, DCM_DT, "PRC Expected Completion Date and Time"},
    {DCM_PRCRESULTINGGPPERFPROCSTEPSEQ, DCM_SQ, "PRC Resulting Gen Purpose Perf Proc Steps Seq"},
    {DCM_PRCREFERENCEDGPSCHEDPROCSTEPSEQ, DCM_SQ, "PRC Referenced Gen Purp Sched Proc Steps Seq"},
    {DCM_PRCSCHEDWORKITEMCODESEQ, DCM_SQ, "PRC Scheduled Workitem Code Sequence"},
    {DCM_PRCPERFORMEDWORKITEMCODESEQ, DCM_SQ, "PRC Performed Workitem Code Sequence"},
    {DCM_PRCINPUTAVAILFLAG, DCM_CS, "PRC Input Availability Flag"},
    {DCM_PRCINPUTINFOSEQ, DCM_SQ, "PRC Input Information Sequence"},
    {DCM_PRCRELEVANTINFOSEQ, DCM_SQ, "PRC Relevant Information Sequence"},
    {DCM_PRCREFERENCEDGPSPSTRANSACTIONUID, DCM_UI, "PRC Referenced Gen Purp SPS Transaction UID"},
    {DCM_PRCSCHEDSTATIONNAMECODESEQ, DCM_SQ, "PRC Scheduled Station Name Code Sequence"},
    {DCM_PRCSCHEDSTATIONCLASSCODESEQ, DCM_SQ, "PRC Scheduled Station Class Code Sequence"},
    {DCM_PRCSCHEDSTATIONLOCCODESEQ, DCM_SQ, "PRC Sched Station Geographic Location Code Seq"},
    {DCM_PRCPERFORMEDSTATIONNAMECODESEQ, DCM_SQ, "PRC Performed Station Name Code Seq"},
    {DCM_PRCPERFORMEDSTATIONCLASSCODESEQ, DCM_SQ, "PRC Performed Station Class Code Sequence"},
    {DCM_PRCPERFORMEDSTATIONLOCCODESEQ, DCM_SQ, "PRC Perf Station Geographic Location Code Seq"},
    {DCM_PRCREQSUBSWORKITEMCODESEQ, DCM_SQ, "PRC Requested Subsequent Workitem Code Sequence"},
    {DCM_PRCNONDICOMOUTPUTCODESEQ, DCM_SQ, "PRC Non-DICOM Output Code Sequence"},
    {DCM_PRCOUTPUTINFOSEQ, DCM_SQ, "PRC Output Information Sequence"},
    {DCM_PRCSCHEDHUMANPERFORMERSSEQ, DCM_SQ, "PRC Scheduled Human Performers Sequence"},
    {DCM_PRCACTUALHUMANPERFORMERSSEQ, DCM_SQ, "PRC Actual Human Performers Sequence"},
    {DCM_PRCHUMANPERFORMERSORG, DCM_LO, "PRC Human Performer's Organization"},
    {DCM_PRCHUMANPERFORMERSNAME, DCM_PN, "PRC Human Performer's Name"},

    {DCM_MAKETAG(0x0040, 0xa8302), DCM_DS, "PRC Entry Does in mGy"},

    {DCM_MAKETAG(0x0040, 0xa010), DCM_CS, "PRC Relationship Type"}, /* Sup 23*/
    {DCM_MAKETAG(0x0040, 0xa027), DCM_LO, "PRC Verifying Organization"}, /* Sup 23*/
    {DCM_MAKETAG(0x0040, 0xa030), DCM_DT, "PRC Verification DateTime"}, /* Sup 23*/
    {DCM_MAKETAG(0x0040, 0xa032), DCM_DT, "PRC Observation DateTime"}, /* Sup 23*/
    {DCM_MAKETAG(0x0040, 0xa040), DCM_CS, "PRC Value Type"}, /* Sup 23*/

    {DCM_MAKETAG(0x0040, 0xa043), DCM_SQ, "PRC Concept-name Code Sequence"},	/* Sup 23 */
    {DCM_MAKETAG(0x0040, 0xa050), DCM_CS, "PRC Continuity of Content"}, /* Sup 23*/
    {DCM_MAKETAG(0x0040, 0xa073), DCM_SQ, "PRC Verifying Observer Sequence"}, /* Sup 23*/
    {DCM_MAKETAG(0x0040, 0xa075), DCM_PN, "PRC Verifying Observer Name"}, /* Sup 23*/
    {DCM_MAKETAG(0x0040, 0xa088), DCM_SQ, "PRC Verifying Observer Identification Code Seq"}, /* Sup 23*/
    {DCM_MAKETAG(0x0040, 0xa0a0), DCM_CS, "PRC Referenced Type of Data"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x0040, 0xa0b0), DCM_US, "PRC Referenced Waveform Channels"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x0040, 0xa120), DCM_DT, "PRC Date Time"}, /* Sup 23*/
    {DCM_MAKETAG(0x0040, 0xa121), DCM_DA, "PRC Date"},	/* Sup 15 */
    {DCM_MAKETAG(0x0040, 0xa122), DCM_TM, "PRC Time"},	/* Sup 15 */
    {DCM_MAKETAG(0x0040, 0xa123), DCM_PN, "PRC Person Name"},	/* Sup 15 */
    {DCM_MAKETAG(0x0040, 0xa124), DCM_UI, "PRC UID"}, /* Sup 23*/
    {DCM_MAKETAG(0x0040, 0xa130), DCM_CS, "PRC Temporal Range Type"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x0040, 0xa132), DCM_UL, "PRC Referenced Sample Offsets"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x0040, 0xa138), DCM_DS, "PRC Referenced Time Offsets"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x0040, 0xa13a), DCM_DT, "PRC Referenced Datetime"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x0040, 0xa160), DCM_UT, "PRC Text Value"},	/* */
    {DCM_MAKETAG(0x0040, 0xa168), DCM_SQ, "PRC Concept Code Sequence"},	/* Sup 15 */
    {DCM_MAKETAG(0x0040, 0xa16a), DCM_ST, "PRC Bibliographics Citation"},	/* Sup 15 */
    {DCM_MAKETAG(0x0040, 0xa170), DCM_SQ, "PRC Purpose of Reference Code Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0040, 0xa180), DCM_US, "PRC Annotation Group Number"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x0040, 0xa195), DCM_SQ, "PRC Concept-name Code Sequence Modifier"},	/* Sup 15 */

    {DCM_MAKETAG(0x0040, 0xa300), DCM_SQ, "PRC Measured Value Sequence"}, /* Sup 23 */
    {DCM_MAKETAG(0x0040, 0xa301), DCM_SQ, "PRC Numeric Value Qualifier Code Sequence"},	/* 2005.05.17 */
    {DCM_MAKETAG(0x0040, 0xa30a), DCM_DS, "PRC Numeric Value"},	/* Sup 15 */
    /*{DCM_MAKETAG(0x0040, 0xa353), DCM_ST, "PRC Address"},	/* Sup 15 */
    /*{DCM_MAKETAG(0x0040, 0xa354), DCM_LO, "PRC Telephone Number"},	/* Sup 15 */
    {DCM_MAKETAG(0x0040, 0xa360), DCM_SQ, "PRC Predecessor Documents Sequence"}, /* Sup 23 */
    {DCM_MAKETAG(0x0040, 0xa370), DCM_SQ, "PRC Referenced Request Sequence"}, /* Sup 23 */
    {DCM_MAKETAG(0x0040, 0xa372), DCM_SQ, "PRC Performed Procedure Code Sequence"}, /* Sup 23 */
    {DCM_MAKETAG(0x0040, 0xa375), DCM_SQ, "PRC Current Reqeusted Procedure Evidence Seq"}, /* Sup 23 */
    {DCM_MAKETAG(0x0040, 0xa385), DCM_SQ, "PRC Pertinent Other Evidence Sequence"}, /* Sup 23 */
    {DCM_MAKETAG(0x0040, 0xa491), DCM_CS, "PRC Completion Flag"}, /* Sup 23 */
    {DCM_MAKETAG(0x0040, 0xa492), DCM_LO, "PRC Completion Flag Description"}, /* Sup 23 */
    {DCM_MAKETAG(0x0040, 0xa493), DCM_CS, "PRC Verification Flag"}, /* Sup 23 */
    {DCM_MAKETAG(0x0040, 0xa504), DCM_SQ, "PRC Content Template Sequence"}, /* Sup 23 */
    {DCM_MAKETAG(0x0040, 0xa525), DCM_SQ, "PRC Identical Documents Sequence"}, /* Sup 23 */
    {DCM_MAKETAG(0x0040, 0xa730), DCM_SQ, "PRC Content Sequence"}, /* Sup 23 */
    {DCM_MAKETAG(0x0040, 0xa992), DCM_ST, "PRC Uniform Resource Locator"},	/* Sup 15 */
    {DCM_MAKETAG(0x0040, 0xb020), DCM_SQ, "PRC Annotation Sequence"},	/* Sup 30 0.6 */
    {DCM_MAKETAG(0x0040, 0xadb00), DCM_CS, "PRC Template Identifier"}, /* Sup 23 */
    {DCM_MAKETAG(0x0040, 0xadb06), DCM_DT, "PRC Template Version"}, /* Sup 23 */
    {DCM_MAKETAG(0x0040, 0xadb07), DCM_DT, "PRC Template Local Version"}, /* Sup 23 */
    {DCM_MAKETAG(0x0040, 0xadb0b), DCM_CS, "PRC Template Extension Flag"}, /* Sup 23 */
    {DCM_MAKETAG(0x0040, 0xadb0c), DCM_UI, "PRC Template Extension Organization UID"}, /* Sup 23 */
    {DCM_MAKETAG(0x0040, 0xadb0d), DCM_UI, "PRC Template Extension Creator UID"}, /* Sup 23 */
    {DCM_MAKETAG(0x0040, 0xadb73), DCM_UL, "PRC Referenced Content Item Identifier"} /* Sup 23 */
};

/* Define the entries for the DEVICE group, 0050
*/
static DCMDICT DEV_dictionary[] = {
    {DCM_DEVCALIBRATIONOBJECT, DCM_CS, "DEV Calibration Object"},
    {DCM_DEVDEVICESEQUENCE, DCM_SQ, "DEV Device Sequence"},
    {DCM_DEVDEVICELENGTH, DCM_DS, "DEV Device Length"},
    {DCM_DEVDEVICEDIAMETER, DCM_DS, "DEV Device Diameter"},
    {DCM_DEVDEVICEDIAMETERUNITS, DCM_CS, "DEV Device Diameter Units"},
    {DCM_DEVDEVICEVOLUME, DCM_DS, "DEV Device Volume"},
    {DCM_DEVINTERMARKERDISTANCE, DCM_DS, "DEV Inter-Marker Distance"},
    {DCM_DEVDEVICEDESCRIPTION, DCM_LO, "DEV Device Description"},
};

/* Define the entries for the RESULTS group, 4008
*/
static DCMDICT RES_dictionary[] = {
    {DCM_RESGROUPLENGTH, DCM_UL, "RES Group Length"},
    {DCM_RESID, DCM_SH, "RES Results ID"},
    {DCM_RESIDISSUER, DCM_LO, "RES Results ID Issuer"},
    {DCM_RESREFERENCEDINTERPSEQ, DCM_SQ, "RES Referenced Interpretation Sequence"},
    {DCM_RESINTERPRECORDEDDATE, DCM_DA, "RES Interpretation Recorded Date"},
    {DCM_RESINTERPRECORDEDTIME, DCM_TM, "RES Interpretation Recorded Time"},
    {DCM_RESINTERPRECORDER, DCM_PN, "RES Interpretation Recorder"},
    {DCM_RESREFERENCETORECORDEDSOUND, DCM_LO, "RES Reference to Recorded Sound"},
    {DCM_RESINTERPTRANSCRIPTIONDATE, DCM_DA, "RES Interpretation Transcription Date"},
    {DCM_RESINTERPTRANSCRIPTIONTIME, DCM_TM, "RES Interpretation Transcription Time"},
    {DCM_RESINTERPTRANSCRIBER, DCM_PN, "RES Interpretation Transcriber"},
    {DCM_RESINTERPTEXT, DCM_ST, "RES Interpretation Text"},
    {DCM_RESINTERPAUTHOR, DCM_PN, "RES Interpretation Author"},
    {DCM_RESINTERPAPPROVERSEQUENCE, DCM_SQ, "RES Interpretation Approver Sequence"},
    {DCM_RESINTERPAPPROVALDATE, DCM_DA, "RES Interpretation Approval Date"},
    {DCM_RESINTERPAPPROVALTIME, DCM_TM, "RES Interpretation Approval Time"},
    {DCM_RESPHYSICIANAPPROVINGINTERP, DCM_PN, "RES Physician Approving Interpretation"},
    {DCM_RESDIAGNOSIS, DCM_LT, "RES Diagnosis"},
    {DCM_RESDIAGNOSISCODESEQ, DCM_SQ, "RES Diagnosis Code Sequence"},
    {DCM_RESDISTRIBUTIIONLISTSEQUENCE, DCM_SQ, "RES Results Distribution List Sequence"},
    {DCM_RESDISTRIBUTIONNAME, DCM_PN, "RES Distribution Name"},
    {DCM_RESDISTRIBUTIONADDRESS, DCM_LO, "RES Distribution Address"},
    {DCM_RESINTERPID, DCM_SH, "RES Interpretation ID"},
    {DCM_RESINTERPIDISSUER, DCM_LO, "RES Interpretation ID Issuer"},
    {DCM_RESINTERPTYPEID, DCM_CS, "RES Interpretation Type ID"},
    {DCM_RESINTERPSTATUSID, DCM_CS, "RES Interpretation Status ID"},
    {DCM_RESIMPRESSIONS, DCM_ST, "RES Impressions"},
    {DCM_RESCOMMENTS, DCM_ST, "RES Comments"}
};

/* Define entries for the FUNCTIONAL group */
static DCMDICT G5200_dictionary[] = {
    {0x52000000, DCM_UL, "FUNC Group Length"},
    {0x52009229, DCM_SQ, "FUNC Shared Functional Group Sequence"},
    {0x52009230, DCM_SQ, "FUNC Per-frame Functional Groups Sequence"},
};

/* Define entries for the CURVE group */
static DCMDICT CRV_dictionary[] = {
    {DCM_CURVEGROUPLENGTH, DCM_UL, "CRV Group Length"},
    {DCM_CURVEDIMENSIONS, DCM_US, "CRV Curve Dimensions"},
    {DCM_CURVENUMBEROFPOINTS, DCM_US, "CRV Number of points"},
    {DCM_CURVETYPEOFDATA, DCM_CS, "CRV Type of Data"},
    {DCM_CURVEDESCRIPTION, DCM_LO, "CRV Curve Description"},
    {DCM_CURVEAXISUNITS, DCM_SH, "CRV Axis Units"},
    {DCM_CURVEAXISLABELS, DCM_SH, "CRV Axis Labels"},
    {DCM_CURVEDATAVALUEREPRESENTATION, DCM_US, "CRV Data Value Representation"},
    {DCM_CURVEMINCOORDINATEVALUE, DCM_US, "CRV Minimum Coordinate Value"},
    {DCM_CURVEMAXCOORDINATEVALUE, DCM_US, "CRV Maximum Coordinate Value"},
    {DCM_CURVERANGE, DCM_SH, "CRV Curve Range"},
    {DCM_CURVEDATADESCRIPTOR, DCM_US, "CRV Data Descriptor"},
    {DCM_CURVECOORDINATESTARTVALUE, DCM_US, "CRV Coordinate Start Value"},
    {DCM_CURVECOORDINATESTEPVALUE, DCM_US, "CRV Coordinate Step Value"},
    {DCM_CURVEAUDIOTYPE, DCM_US, "CRV Audio Type"},
    {DCM_CURVEAUDIOSAMPLEFORMAT, DCM_US, "CRV Audio Sample Format"},
    {DCM_CURVENUMBEROFCHANNELS, DCM_US, "CRV Number of Channels"},
    {DCM_CURVENUMBEROFSAMPLES, DCM_UL, "CRV Number of Samples"},
    {DCM_CURVESAMPLERATE, DCM_UL, "CRV Sample Rate"},
    {DCM_CURVETOTALTIME, DCM_UL, "CRV Total Time"},
    {DCM_CURVEAUDIOSAMPLEDATA, DCM_OW, "CRV Audio Sample Data"},
    {DCM_CURVEAUDIOCOMMENTS, DCM_LT, "CRV Audio Comments"},
    {DCM_CURVELABEL, DCM_LO, "CRV Curve Label"},
    {DCM_CURVEREFOVERLAYSEQUENCE, DCM_SQ, "CRV Referenced Overlay Sequence"},
    {DCM_CURVEREFOVERLAYGROUP, DCM_US, "CRV Referenced Overlay Group"},
    {DCM_CURVEDATA, DCM_OW, "CRV Curve Data"}
};


/* Define the entries for the NMI (nuclear medicine image) group, 0054 */
static DCMDICT NMI_dictionary[] = {
    {DCM_NMIGROUPLENGTH, DCM_UL, "NMI Group Length"},
    {DCM_NMIENERGYWINDOWVECTOR, DCM_US, "NMI Energy Window Vector"},
    {DCM_NMINUMBEROFENERGYWINDOWS, DCM_US, "NMI Number of Energy Windows"},
    {DCM_NMIENERGYWINDOWINFOSEQ, DCM_SQ, "NMI Energy Window Information Sequence"},
    {DCM_NMIENERGYWINDOWRANGESEQ, DCM_SQ, "NMI Energy Window Range Sequence"},
    {DCM_NMIENERGYWINDOWLOWERLIMIT, DCM_DS, "NMI Energy Window Lower Limit"},
    {DCM_NMIENERGYWINDOWUPPERLIMIT, DCM_DS, "NMI Energy Window Upper Limit"},
    {DCM_NMIRADIOPHARMINFOSEQ, DCM_SQ, "NMI Radiopharmaceutical Information Sequence"},
    {DCM_NMIRESIDUALSYRINGECOUNTS, DCM_IS, "NMI Residual Syringe Counts"},
    {DCM_NMIENERGYWINDOWNAME, DCM_SH, "NMI Energy Window Name"},
    {DCM_NMIDETECTORVECTOR, DCM_US, "NMI Detector Vector"},
    {DCM_NMINUMBEROFDETECTORS, DCM_US, "NMI Number of Detectors"},
    {DCM_NMIDETECTORINFOSEQUENCE, DCM_SQ, "NMI Detector Information Sequence"},
    {DCM_NMIPHASEVECTOR, DCM_US, "NMI Phase Vector"},
    {DCM_NMINUMBEROFPHASES, DCM_US, "NMI Number of Phases"},
    {DCM_NMIPHASEINFOSEQUENCE, DCM_SQ, "NMI Phase Information Sequence"},
    {DCM_NMINUMBEROFFRAMESINPHASE, DCM_US, "NMI Number of Frames in Phase"},
    {DCM_NMIPHASEDELAY, DCM_IS, "NMI Phase Delay"},
    {DCM_NMIPAUSEBETWEENFRAMES, DCM_IS, "NMI Pause between Frames"},
    {DCM_NMIROTATIONVECTOR, DCM_US, "NMI Rotation Vector"},
    {DCM_NMINUMBEROFROTATIONS, DCM_US, "NMI Number of rotations"},
    {DCM_NMIROTATIONINFOSEQUENCE, DCM_SQ, "NMI Rotation Information Sequence"},
    {DCM_NMINUMBEROFFRAMESINROTATION, DCM_US, "NMI Number of frames in rotation"},
    {DCM_NMIRRINTERVALVECTOR, DCM_US, "NMI R-R Interval Vector"},
    {DCM_NMINUMBEROFRRINTERVALS, DCM_US, "NMI Number of R-R Intervals"},
    {DCM_NMIGATEDINFOSEQUENCE, DCM_SQ, "NMI Gated Information Sequence"},
    {DCM_NMIDATAINFORMATIONSEQUENCE, DCM_SQ, "NMI Data Information Sequence"},
    {DCM_NMITIMESLOTVECTOR, DCM_US, "NMI Time Slot Vector"},
    {DCM_NMINUMBEROFTIMESLOTS, DCM_US, "NMI Number of Time Slots"},
    {DCM_NMITIMESLOTINFOSEQUENCE, DCM_SQ, "NMI Time Slot Information Sequence"},
    {DCM_NMITIMESLOTTIME, DCM_DS, "NMI Time Slot Time"},
    {DCM_NMISLICEVECTOR, DCM_US, "NMI Slice Vector"},
    {DCM_NMINUMBEROFSLICES, DCM_US, "NMI Number of Slices"},
    {DCM_NMIANGULARVIEWVECTOR, DCM_US, "NMI Angular View Vector"},
    {DCM_NMITIMESLICEVECTOR, DCM_US, "NMI Time Slice Vector"},
    {DCM_NMINUMBEROFTIMESLICES, DCM_US, "NMI Number of Time Slices"},
    {DCM_NMISTARTANGLE, DCM_DS, "NMI Start Angle"},
    {DCM_NMITYPEOFDETECTORMOTION, DCM_CS, "NMI Type of Detector Motion"},
    {DCM_NMITRIGGERVECTOR, DCM_IS, "NMI Trigger Vector"},
    {DCM_NMINUMBEROFTRIGGERSINPHASE, DCM_US, "NMI Number of Triggers in Phase"},
    {DCM_NMIVIEWCODESEQUENCE, DCM_SQ, "NMI View Code Sequence"},
    {DCM_NMIVIEWANGULATIONMODIFIERCODESEQ, DCM_SQ, "NMI View Angulation Modifer Code Sequence"},
    {DCM_NMIRADIONUCLIDECODESEQUENCE, DCM_SQ, "NMI Radionuclide Code Sequence"},
    {DCM_NMIRADIOPHARMROUTECODESEQUENCE, DCM_SQ, "NMI Radiopharmaceutical Route Code Sequence"},
    {DCM_NMIRADIOPHARMCODESEQUENCE, DCM_SQ, "NMI Radiopahrmaceutical Code Sequence"},
    {DCM_NMICALIBRATIONDATASEQUENCE, DCM_SQ, "NMI Calibration Data Sequence"},
    {DCM_NMIENERGYWINDOWNUMBER, DCM_US, "NMI Energy Window Number"},
    {DCM_NMIIMAGEID, DCM_SH, "NMI Image ID"},
    {DCM_NMIPATIENTORIENTATIONCODESEQ, DCM_SQ, "NMI Patient Orientation Code Sequence"},
    {DCM_NMIPATIENTORIENTATIONMODIFIERCODESEQ, DCM_SQ, "NMI Patient Orientation Modifier Code Sequence"},
    {DCM_NMIPATIENTGANTRYRELATIONSHIPCODESEQ, DCM_SQ, "NMI Patient Gantry Relationship Code Sequence"},
    {DCM_NMISERIESTYPE, DCM_CS, "NMI Series Type"},
    {DCM_NMIUNITS, DCM_CS, "NMI Units"},
    {DCM_NMICOUNTSSOURCE, DCM_CS, "NMI Counts Source"},	/* 1002 */
    {DCM_NMIREPROJECTIONMETHOD, DCM_CS, "NMI Reprojection Method"},	/* 1004 */
    {DCM_NMIRANDOMSCORRECTIONMETHOD, DCM_CS,
    "NMI Randoms Correction Method"},	/* 1100 */
    {DCM_NMIATTENUATIONCORRECTIONMETHOD, DCM_LO,
    "NMI Attenuation Correction Method"},	/* 1101 */
    {DCM_NMIDECAYCORRECTION, DCM_CS, "NMI Decay Correction"},	/* 1102 */
    {DCM_NMIRECONSTRUCTIONMETHOD, DCM_LO, "NMI Reconstruction Method"},	/* 1103 */
    {DCM_NMIDETECTORLINESRESPONSEUSED, DCM_LO,
    "NMI Detector Lines of Response Used"},	/* 1104 */
    {DCM_NMISCATTERCORRECTIONMETHOD, DCM_LO, "NMI Scatter Correction Method"},	/* 1105 */
    {DCM_NMIAXIALACCEPTANCE, DCM_DS, "NMI Axial Acceptance"},	/* 1200 */
    {DCM_NMIAXIALMASH, DCM_IS, "NMI Axial Mash"},	/* 1201 */
    {DCM_NMITRANSVERSEMASH, DCM_IS, "NMI Transverse Mash"},	/* 1202 */
    {DCM_NMIDETECTORELEMENTSIZE, DCM_DS, "NMI Detector Element Size"},	/* 1203 */
    {DCM_NMICOINCIDENCEWINDOWWIDTH, DCM_DS, "NMI Coincidence Window Width"},	/* 1210 */
    {DCM_NMISECONDARYCOUNTSTYPE, DCM_CS, "NMI Secondary Counts Type"},	/* 1220 */
    {DCM_NMIFRAMEREFERENCETIME, DCM_DS, "NMI Frame Reference Time"},	/* 1300 */
    {DCM_NMIPRIMARYCOUNTSACCUMULATED, DCM_IS,
    "NMI Primary (Prompts) Counts Accumulated"},	/* 1310 */
    {DCM_NMISECONDARYCOUNTSACCUMULATED, DCM_IS,
    "NMI Secondary Counts Accumulated"},	/* 1311 */
    {DCM_NMISLICESENSITIVITYFACTOR, DCM_DS, "NMI Slice Sensitivity Factor"},	/* 1320 */
    {DCM_NMIDECAYFACTOR, DCM_DS, "NMI Decay Factor"},	/* 1321 */
    {DCM_NMIDOSECALIBRATIONFACTOR, DCM_DS, "NMI Dose Calibration Factor"},	/* 1322 */
    {DCM_NMISCATTERFRACTIONFACTOR, DCM_DS, "NMI Scatter Fraction Factor"},	/* 1323 */
    {DCM_NMIDEADTIMEFACTOR, DCM_DS, "NMI Dead Time Factor"},	/* 1324 */
    {DCM_NMIIMAGEINDEX, DCM_US, "NMI Image Index"},	/* 1330 */
    {DCM_NMICOUNTSINCLUDED, DCM_CS, "NMI Counts Included"},	/* 1400 */
    {DCM_NMIDEADTIMECORRECTIONFLAG, DCM_CS,
    "NMI Dead Time Correction Flag"},	/* 1401 */
};

/* Define the entries for the Graphics group, 0070 */
static DCMDICT GRP_dictionary[] = {
    {DCM_MAKETAG(0x0070, 0x0000), DCM_UL, "GRP Group Length"},
    {DCM_MAKETAG(0x0070, 0x0001), DCM_SQ, "GRP Graphic Annotation Sequence"},
    {DCM_MAKETAG(0x0070, 0x0002), DCM_CS, "GRP Graphic Layer"},
    {DCM_MAKETAG(0x0070, 0x0003), DCM_CS, "GRP Bounding Box Annotation Units"},
    {DCM_MAKETAG(0x0070, 0x0004), DCM_CS, "GRP Anchor Point Annotation Units"},
    {DCM_MAKETAG(0x0070, 0x0005), DCM_CS, "GRP Graphic Annotation Units"},
    {DCM_MAKETAG(0x0070, 0x0006), DCM_ST, "GRP Unformatted Text Value"},
    {DCM_MAKETAG(0x0070, 0x0008), DCM_SQ, "GRP Text Object Sequence"},
    {DCM_MAKETAG(0x0070, 0x0009), DCM_SQ, "GRP Graphic Object Sequence"},
    {DCM_MAKETAG(0x0070, 0x0010), DCM_FL, "GRP Bounding Box Top LH Corner"},
    {DCM_MAKETAG(0x0070, 0x0011), DCM_FL, "GRP Bounding Box Bottom RH Corner"},
    {DCM_MAKETAG(0x0070, 0x0012), DCM_CS, "GRP Bounding Box Text Horiz Justification"},
    {DCM_MAKETAG(0x0070, 0x0014), DCM_FL, "GRP Anchor Point"},
    {DCM_MAKETAG(0x0070, 0x0015), DCM_CS, "GRP Anchor Point Visibility"},
    {DCM_MAKETAG(0x0070, 0x0020), DCM_US, "GRP Graphic Dimensions"},
    {DCM_MAKETAG(0x0070, 0x0021), DCM_US, "GRP Number of Graphic Points"},
    {DCM_MAKETAG(0x0070, 0x0022), DCM_FL, "GRP Graphic Data"}, /* Sup 33 */
    {DCM_MAKETAG(0x0070, 0x0023), DCM_CS, "GRP Graphic Type"}, /* Sup 33 */
    {DCM_MAKETAG(0x0070, 0x0024), DCM_CS, "GRP Graphic Filled"}, /* Sup 33 */
    {DCM_MAKETAG(0x0070, 0x0041), DCM_CS, "GRP Image Horizontal Flip"}, /* Sup 33*/
    {DCM_MAKETAG(0x0070, 0x0042), DCM_US, "GRP Image Rotation"}, /* Sup 33 */
    {DCM_MAKETAG(0x0070, 0x0052), DCM_SL, "GRP Displayed Area Top LH Corner"},
    {DCM_MAKETAG(0x0070, 0x0053), DCM_SL, "GRP Displayed Area Bottom RH Corner"},
    {DCM_MAKETAG(0x0070, 0x005a), DCM_SQ, "GRP Display Area Selection Seq"},
    {DCM_MAKETAG(0x0070, 0x0060), DCM_SQ, "GRP Graphic Layer Sequence"},
    {DCM_MAKETAG(0x0070, 0x0062), DCM_IS, "GRP Graphic Layer Order"},
    {DCM_MAKETAG(0x0070, 0x0066), DCM_US, "GRP Graphic Layer Rec Disp GS Val"},
    {DCM_MAKETAG(0x0070, 0x0067), DCM_US, "GRP Graphic Layer Rec Disp RGB Val"},
    {DCM_MAKETAG(0x0070, 0x0068), DCM_LO, "GRP Graphic Layer Description"},

    {DCM_MAKETAG(0x0070, 0x0080), DCM_CS, "GRP Presentation Label"}, /* Sup 33 */
    {DCM_MAKETAG(0x0070, 0x0081), DCM_LO, "GRP Presentation Description"}, /* Sup 33 */
    {DCM_MAKETAG(0x0070, 0x0082), DCM_DA, "GRP Presentation Creation Date"}, /* Sup 33 */
    {DCM_MAKETAG(0x0070, 0x0083), DCM_TM, "GRP Presentation Creation Time"}, /* Sup 33 */
    {DCM_MAKETAG(0x0070, 0x0084), DCM_PN, "GRP Presentation Creators Name"}, /* Sup 33 */
    {DCM_MAKETAG(0x0070, 0x0100), DCM_CS, "GRP Presentation Size Mode"}, /* Sup 33 */
    {DCM_MAKETAG(0x0070, 0x0101), DCM_DS, "GRP Presentation Pixel Spacing"}, /* Sup 33 */
    {DCM_MAKETAG(0x0070, 0x0102), DCM_IS, "GRP Presentation Pixel Aspect Ratio"}, /* Sup 33 */
    {DCM_MAKETAG(0x0070, 0x0103), DCM_FL, "GRP Presentation Pixel Magnification Ratio"}, /* Sup 33 */
    {DCM_MAKETAG(0x0070, 0x0308), DCM_SQ, "GRP Registration Sequence"}, /* PS 3.3 */
    {DCM_MAKETAG(0x0070, 0x0309), DCM_SQ, "GRP Referenced Matrix Registration Sequence"}, /* PS 3.3 */
    {DCM_MAKETAG(0x0070, 0x030a), DCM_SQ, "GRP FoR Transformation Matrix Sequence"}, /* PS 3.3 */
    {DCM_MAKETAG(0x0070, 0x030c), DCM_SH, "GRP FoR Transformation Matrix Type"}, /* PS 3.3 */
    {DCM_MAKETAG(0x0070, 0x030d), DCM_SQ, "GRP Registration Type Code Sequence"}, /* PS 3.3 */
    {DCM_MAKETAG(0x0070, 0x0401), DCM_US, "GRP Graphic Layer Recommended Disp CIELab Value"}, /* Sup 100 */
    {DCM_MAKETAG(0x0070, 0x0402), DCM_SQ, "GRP Blending Sequence"}, /* Sup 100 */
    {DCM_MAKETAG(0x0070, 0x0403), DCM_FL, "GRP Relative Opacity"}, /* Sup 100 */
    {DCM_MAKETAG(0x0070, 0x0404), DCM_SQ, "GRP Referenced Spatial Registration Sequence"}, /* Sup 100 */
    {DCM_MAKETAG(0x0070, 0x0405), DCM_CS, "GRP Blending Position"}, /* Sup 100 */
};

/* Define the entries for group 0074 */
static DCMDICT G0074_dictionary[] = {
    {DCM_MAKETAG(0x0074, 0x1000), DCM_CS, "Unified Procedure Step State"},
    {DCM_MAKETAG(0x0074, 0x1002), DCM_SQ, "UPS Progress Information Sequence"},
    {DCM_MAKETAG(0x0074, 0x1004), DCM_DS, "Unified Procedure Step Progress"},
    {DCM_MAKETAG(0x0074, 0x1006), DCM_ST, "Unified Procedure Step Progress Description"},
    {DCM_MAKETAG(0x0074, 0x1008), DCM_SQ, "Unified Procedure Step Communications URI Seq"},
    {DCM_MAKETAG(0x0074, 0x100a), DCM_ST, "Contact URI"},
    {DCM_MAKETAG(0x0074, 0x100c), DCM_LO, "Contact Display Name"},
    {DCM_MAKETAG(0x0074, 0x100e), DCM_SQ, "UPS Discontinuation Reason Code Sequence"},
    {DCM_MAKETAG(0x0074, 0x1200), DCM_CS, "Sched Proc Step Priority"},
    {DCM_MAKETAG(0x0074, 0x1202), DCM_LO, "Worklist Label"},
    {DCM_MAKETAG(0x0074, 0x1204), DCM_LO, "Proc Step Label"},
    {DCM_MAKETAG(0x0074, 0x1210), DCM_SQ, "Scheduled Processing Parameters Seq"},
    {DCM_MAKETAG(0x0074, 0x1212), DCM_SQ, "Performed Processing Parameters Seq"},
    {DCM_MAKETAG(0x0074, 0x1216), DCM_SQ, "UPS Performed Procedure Seq"},
    {DCM_MAKETAG(0x0074, 0x1220), DCM_SQ, "Related Procedure Step Seq"},
    {DCM_MAKETAG(0x0074, 0x1222), DCM_LO, "Procedure Step Relationship Type"},
    {DCM_MAKETAG(0x0074, 0x1230), DCM_LO, "Deletion Lock"},
    {DCM_MAKETAG(0x0074, 0x1234), DCM_AE, "Retrieving AE"},
    {DCM_MAKETAG(0x0074, 0x1236), DCM_AE, "Requesting AE"},
    {DCM_MAKETAG(0x0074, 0x1238), DCM_LT, "Reason for Cancellation"},
    {DCM_MAKETAG(0x0074, 0x1242), DCM_CS, "SCP Status"},
    {DCM_MAKETAG(0x0074, 0x1244), DCM_CS, "Supscription List Status"},
    {DCM_MAKETAG(0x0074, 0x1246), DCM_CS, "UPS List Status"},
};

/* Define the entries for the OLY (Overlay) group */
static DCMDICT OLY_dictionary[] = {
    {DCM_OLYGROUPLENGTH, DCM_UL, "OLY Group Length"},
    {DCM_OLYROWS, DCM_US, "OLY Rows"},
    {DCM_OLYCOLUMNS, DCM_US, "OLY Columns"},
    {DCM_OLYPLANES, DCM_US, "OLY Planes"},
    {DCM_OLYNUMBEROFFRAMESINOVERLAY, DCM_IS, "OLY Number of frames in Overlay"},
    {DCM_OLYOVERLAYDESCRIPTION, DCM_LO, "OLY Overlay Description"},
    {DCM_OLYTYPE, DCM_CS, "OLY Type"},
    {DCM_OLYSUBTYPE, DCM_LO, "OLY Subtype"},
    {DCM_OLYORIGIN, DCM_SS, "OLY Origin"},
    {DCM_OLYIMAGEFRAMEORIGIN, DCM_US, "OLY Image Frame Origin"},
    {DCM_OLYOVERLAYPLANEORIGIN, DCM_US, "OLY Overlay Plane Origin"},
    {DCM_OLYCOMPRESSIONCODE, DCM_LO, "OLY Compression Code (RET)"},
    {DCM_OLYBITSALLOCATED, DCM_US, "OLY Overlay Bits Allocated"},
    {DCM_OLYBITPOSITION, DCM_US, "OLY Overlay Bit Position"},
    {DCM_OLYOVERLAYFORMAT, DCM_LO, "OLY Overlay Format (RET)"},
    {DCM_OLYOVERLAYLOCATION, DCM_US, "OLY Overlay Location (RET)"},
    {DCM_OLYDESCRIPTORGRAY, DCM_US, "OLY Overlay Descriptor - Gray"},
    {DCM_OLYDESCRIPTORRED, DCM_US, "OLY Overlay Descriptor - Red"},
    {DCM_OLYDESCRIPTORGREEN, DCM_US, "OLY Overlay Descriptor - Green"},
    {DCM_OLYDESCRIPTORBLUE, DCM_US, "OLY Overlay Descriptor - Blue"},
    {DCM_OLYGRAY, DCM_US, "OLY Overlays - Gray"},
    {DCM_OLYRED, DCM_US, "OLY Overlays - Red"},
    {DCM_OLYGREEN, DCM_US, "OLY Overlays - Green"},
    {DCM_OLYBLUE, DCM_US, "OLY Overlays - Blue"},
    {DCM_OLYROIAREA, DCM_IS, "OLY ROI Area"},
    {DCM_OLYROIMEAN, DCM_DS, "OLY ROI Mean"},
    {DCM_OLYROISTANDARDDEVIATION, DCM_DS, "OLY ROI Standard Deviation"},
    {DCM_OLYOVERLAYLABEL, DCM_LO, "OLY Overlay Label"},
    {DCM_OLYDATA, DCM_OW, "OLY Data"},
    {DCM_OLYCOMMENTS, DCM_LO, "OLY Comments (RET)"}
};

/* Define the entries for the PIXEL group (7FE0)
*/
static DCMDICT PXL_dictionary[] = {
    {DCM_PXLGROUPLENGTH, DCM_UL, "PXL Group Length"},
    {DCM_PXLPIXELDATA, DCM_OT, "PXL Pixel Data"}
};

/* Define the elements for the MEDIA group (0088) */
static DCMDICT MED_dictionary[] = {
    {DCM_MEDIAGROUPLENGTH, DCM_UL, "MED Media Group Length "},
    {DCM_MEDIASTORAGEFILESETID, DCM_SH, "MED Storage Media File-set ID"},
    {DCM_MEDIASTORAGEFILESETUID, DCM_UI, "MED Storage Media File-setUID"},
    {DCM_MEDIAICONIMAGE, DCM_SQ, "MED Icon Image Sequence"},
    {DCM_MEDIATOPICTITLE, DCM_LO, "MED Topic Title"},
    {DCM_MEDIATOPICSUBJECT, DCM_ST, "MED Topic Subject"},
    {DCM_MEDIATOPICAUTHOR, DCM_LO, "MED Topic Author"},
    {DCM_MEDIATOPICKEYWORD, DCM_LO, "MED Topic Keywords"}
};

/* Define the entries in the BASICFILMSESSION group (2000)
*/
static DCMDICT BFS_dictionary[] = {
    {DCM_BFSGROUPLENGTH, DCM_UL, "BFS Group Length"},
    {DCM_BFSCOPIES, DCM_IS, "BFS Number of copies printed for each film"},
    {DCM_BFSPRINTPRIORITY, DCM_CS, "BFS Specifies priority of print job"},
    {DCM_BFSMEDIUMTYPE, DCM_CS, "BFS Medium on which page will be printed"},
    {DCM_BFSFILMDESTINATION, DCM_CS, "BFS Film destination"},
    {DCM_BFSFILMSESSIONLABEL, DCM_LO, "BFS Human readable label to identify film"},
    {DCM_BFSMEMORYALLOCATION, DCM_IS, "BFS Amount of mem allocated for film session"},
    {DCM_BFSREFERENCEDFILMBOXSEQ, DCM_SQ, "BFS seq of UIDs of diff FILMBOX instances"}
};

/* Define the entries in the BASICFILMBOX group (2010)
*/
static DCMDICT BFB_dictionary[] = {
    {DCM_BFBGROUPLENGTH, DCM_UL, "BFB Group Length"},
    {DCM_BFBIMAGEDISPLAYFORMAT, DCM_ST, "BFB Type of image display format"},
    {DCM_BFBANNOTATIONDISPLAYFORMAT, DCM_CS, "BFB Id of annotation display format"},
    {DCM_BFBFILMORIENTATION, DCM_CS, "BFB Film orientation"},
    {DCM_BFBFILMSIZEID, DCM_CS, "BFB Film size identification"},
    {DCM_BFBMAGNIFICATIONTYPE, DCM_CS, "BFB Interpol. type by which printer mag image"},
    {DCM_BFBSMOOTHINGTYPE, DCM_CS, "BFB Specifies type of interpolation function"},
    {DCM_BFBBORDERDENSITY, DCM_CS, "BFB density of film areas around/between images"},
    {DCM_BFBEMPTYIMAGEDENSITY, DCM_CS, "BFB density of image box area having no image"},
    {DCM_BFBMINDENSITY, DCM_US, "BFB Minimum density of images on the film"},
    {DCM_BFBMAXDENSITY, DCM_US, "BFB Maximum density of images on the film"},
    {DCM_BFBTRIM, DCM_CS, "BFB specifies whether to trim or not"},
    {DCM_BFBCONFIGURATIONINFO, DCM_ST, "BFB ID of configuration table"},
    {DCM_BFBREFBASICFILMSESSIONSEQ, DCM_SQ, "BFB seq. of film session instance"},
    {DCM_BFBREFBASICIMAGEBOXSEQ, DCM_SQ, "BFB seq. of basic image box SOP instance"},
    {DCM_BFBREFBASICANNOTBOXSEQ, DCM_SQ, "BFB seq. of basic annotation box SOP instance"},
};

/* Defines the entries in the BASICIMAGEBOX (2020)
*/
static DCMDICT BIB_dictionary[] = {
    {DCM_BIBGROUPLENGTH, DCM_UL, "BIB Group Length"},
    {DCM_BIBIMAGEPOSITION, DCM_US, "BIB Specifies position of the image in the film"},
    {DCM_BIBPOLARITY, DCM_CS, "BIB Specifies image polarity"},
    {DCM_BIBREQUESTEDIMAGESIZE, DCM_DS, "BIB Requested image size"},
    {DCM_BIBPREFORMATGREYSCALEIMAGESEQ, DCM_SQ, "BIB Preformatted Greyscale image"},
    {DCM_BIBPREFORMATCOLORIMAGESEQ, DCM_SQ, "BIB Preformatted Color image"},
    {DCM_BIBREFIMAGEOVERLAYBOXSEQ, DCM_SQ, "BIB Referenced Image Overlay Box seq"},
    {DCM_BIBREFVOILUTSEQ, DCM_SQ, "BIB Referenced VOI LUT seq."}
};

/* Defines the entries in the BASICANNOTATIONBOX group (2030)
*/
static DCMDICT BAB_dictionary[] = {
    {DCM_BABGROUPLENGTH, DCM_UL, "BAB Group Length"},
    {DCM_BABANNOTATIONPOSITION, DCM_US, "BAB posn of the annot. box in parent film box"},
    {DCM_BABTEXTSTRING, DCM_LO, "BAB text string"}
};

/* Defines entries for BASICIMAGEOVERLAYBOX group (2040)
*/
static DCMDICT IOB_dictionary[] = {
    {DCM_IOBGROUPLENGTH, DCM_UL, "IOB Group Length"},
    {DCM_IOBREFOVERLAYPLANESEQ, DCM_SQ, "IOB Ref Overlay Plane Sequence"},
    {DCM_IOBREFOVERLAYPLANEGROUPS, DCM_US, "IOB Ref Overlay Plane Groups"},
    {DCM_IOBOVERLAYMAGNIFICATIONTYPE, DCM_CS, "IOB Overlay Magnification Type"},
    {DCM_IOBOVERLAYSMOOTHINGTYPE, DCM_CS, "IOB Overlay Smoothing Type"},
    {DCM_IOBOVERLAYFOREGROUNDDENSITY, DCM_CS, "IOB Overlay Foreground Density"},
    {DCM_IOBOVERLAYMODE, DCM_CS, "IOB Overlay Mode"},
    {DCM_IOBTHRESHOLDDENSITY, DCM_CS, "IOB Threshold Density"},
    {DCM_IOBREFIMAGEBOXSEQUENCE, DCM_SQ, "IOB Ref Image Box Sequence (RET)"}
};

/* Defines entries for Presentation LUT Group (2050)
*/
static DCMDICT PLUT_dictionary[] = {
    {DCM_MAKETAG(0x2050, 0x0000), DCM_UL, "PLUT Group Length"},
    {DCM_MAKETAG(0x2050, 0x0010), DCM_SQ, "PLUT Presentation LUT Sequence"},
    {DCM_MAKETAG(0x2050, 0x0020), DCM_CS, "PLUT Presentation LUT Shape"},
    {DCM_MAKETAG(0x2050, 0x0500), DCM_SQ, "PLUT Referenced Presentation LUT Sequence"}
};

/* Defines the entries in the PRINTJOB group (2100)
*/
static DCMDICT PJ_dictionary[] = {
    {DCM_PJGROUPLENGTH, DCM_UL, "PJ Group Length"},
    {DCM_PJEXECUTIONSTATUS, DCM_CS, "PJ execution status of print job"},
    {DCM_PJEXECUTIONSTATUSINFO, DCM_CS, "PJ additional information"},
    {DCM_PJCREATIONDATE, DCM_DA, "PJ date of print job creation"},
    {DCM_PJCREATIONTIME, DCM_TM, "PJ time of print job creation"},
    {DCM_PJORIGINATOR, DCM_AE, "PJ Appln entity title that issued the print opn"},
    {DCM_MAKETAG(0x2100, 0x0140), DCM_AE, "PJ Destination AE"},		/* 2002.06.30 */
    {DCM_MAKETAG(0x2100, 0x0160), DCM_SH, "PJ Owner ID"},		/* 2002.06.30 */
    {DCM_MAKETAG(0x2100, 0x0170), DCM_IS, "PJ Number of Films"},	/* 2002.06.30 */
    {DCM_PJREFPRINTJOBSEQ, DCM_SQ, "PJ Referenced print job seq."}
};

/* Defines the entries in the PRINTER group (2110)
*/
static DCMDICT PRN_dictionary[] = {
    {DCM_PRINTERGROUPLENGTH, DCM_UL, "PRINTER Group Length"},
    {DCM_PRINTERSTATUS, DCM_CS, "PRINTER printer device status"},
    {DCM_PRINTERSTATUSINFO, DCM_CS, "PRINTER additional information"},
    {DCM_PRINTERNAME, DCM_LO, "PRINTER printer name"},
    {DCM_PRINTERQUEUEID, DCM_SH, "Printer Queue ID"}
};

/* Define the entries in the 0x3002 group, used for RT planning
*/
static DCMDICT G3002_dictionary[] = {
    {DCM_MAKETAG(0x3002, 0x0000), DCM_UL, "RT Group Length"},
    {DCM_MAKETAG(0x3002, 0x0002), DCM_SH, "RT Image Label"},
    {DCM_MAKETAG(0x3002, 0x0003), DCM_LO, "RT Image Name"},
    {DCM_MAKETAG(0x3002, 0x0004), DCM_ST, "RT Image Description"},
    {DCM_MAKETAG(0x3002, 0x000a), DCM_CS, "RT Reported Values Origin"},
    {DCM_MAKETAG(0x3002, 0x000c), DCM_CS, "RT Image Plane"},
    {DCM_MAKETAG(0x3002, 0x000e), DCM_DS, "RT X-Ray Image Receptor Angle"},
    {DCM_MAKETAG(0x3002, 0x0010), DCM_DS, "RT Image Orientation"},
    {DCM_MAKETAG(0x3002, 0x0011), DCM_DS, "RT Image Plane Pixel Spacing"},
    {DCM_MAKETAG(0x3002, 0x0012), DCM_DS, "RT Image Position"},
    {DCM_MAKETAG(0x3002, 0x0020), DCM_SH, "RT Radiation Machine Name"},
    {DCM_MAKETAG(0x3002, 0x0022), DCM_DS, "RT Radiation Machine SAD"},
    {DCM_MAKETAG(0x3002, 0x0024), DCM_DS, "RT Radiation Machine SSD"},
    {DCM_MAKETAG(0x3002, 0x0026), DCM_DS, "RT Image SID"},
    {DCM_MAKETAG(0x3002, 0x0028), DCM_DS, "RT Source to Reference Object Distance"},
    {DCM_MAKETAG(0x3002, 0x0029), DCM_IS, "RT Fraction Number"},
    {DCM_MAKETAG(0x3002, 0x0030), DCM_SQ, "RT Exposure Sequence"},
    {DCM_MAKETAG(0x3002, 0x0032), DCM_DS, "RT Meterset Exposure"}
};

/* Define the entries in the 0x3004 group, Dose Volume Histogram (DVH),
** used in RT planning.
*/
static DCMDICT DVH_dictionary[] = {
    {DCM_MAKETAG(0x3004, 0x0000), DCM_UL, "DVH Group Length"},
    {DCM_MAKETAG(0x3004, 0x0001), DCM_CS, "DVH Type"},
    {DCM_MAKETAG(0x3004, 0x0002), DCM_CS, "DVH Dose Units"},
    {DCM_MAKETAG(0x3004, 0x0004), DCM_CS, "DVH Dose Type"},
    {DCM_MAKETAG(0x3004, 0x0006), DCM_LO, "DVH Dose Comment"},
    {DCM_MAKETAG(0x3004, 0x0008), DCM_DS, "DVH Normalization Point"},
    {DCM_MAKETAG(0x3004, 0x000a), DCM_CS, "DVH Dose Summation Type"},
    {DCM_MAKETAG(0x3004, 0x000c), DCM_DS, "DVH Grid Frame Offset Vector"},
    {DCM_MAKETAG(0x3004, 0x000e), DCM_DS, "DVH Dose Grid Scaling"},
    {DCM_MAKETAG(0x3004, 0x0010), DCM_SQ, "DVH RT Dose ROI Sequence"},
    {DCM_MAKETAG(0x3004, 0x0012), DCM_DS, "DVH Dose Value"},
    {DCM_MAKETAG(0x3004, 0x0040), DCM_DS, "DVH Normalization Point"},
    {DCM_MAKETAG(0x3004, 0x0042), DCM_DS, "DVH Normalization Dose Value"},
    {DCM_MAKETAG(0x3004, 0x0050), DCM_SQ, "DVH Sequence"},
    {DCM_MAKETAG(0x3004, 0x0052), DCM_DS, "DVH Dose Scaling"},
    {DCM_MAKETAG(0x3004, 0x0054), DCM_CS, "DVH Volume Units"},
    {DCM_MAKETAG(0x3004, 0x0056), DCM_IS, "DVH Number of Bins"},
    {DCM_MAKETAG(0x3004, 0x0058), DCM_DS, "DVH Data"},
    {DCM_MAKETAG(0x3004, 0x0060), DCM_SQ, "DVH Referenced ROI Sequence"},
    {DCM_MAKETAG(0x3004, 0x0062), DCM_CS, "DVH ROI Contribution Type"},
    {DCM_MAKETAG(0x3004, 0x0070), DCM_DS, "DVH Minimum Dose"},
    {DCM_MAKETAG(0x3004, 0x0072), DCM_DS, "DVH Maximum Dose"},
    {DCM_MAKETAG(0x3004, 0x0074), DCM_DS, "DVH Mean Dose"}
};

/* Define the entries in the 0x3006 group, Structure Set,
** used in RT planning.
*/
static DCMDICT SSET_dictionary[] = {
    {DCM_MAKETAG(0x3006, 0x0000), DCM_UL, "SSET Group Length"},
    {DCM_MAKETAG(0x3006, 0x0002), DCM_SH, "SSET Structure Set Label"},
    {DCM_MAKETAG(0x3006, 0x0004), DCM_LO, "SSET Structure Set Name"},
    {DCM_MAKETAG(0x3006, 0x0006), DCM_ST, "SSET Structure Set Description"},
    {DCM_MAKETAG(0x3006, 0x0008), DCM_DA, "SSET Structure Set Date"},
    {DCM_MAKETAG(0x3006, 0x0009), DCM_TM, "SSET Structure Set Time"},
    {DCM_MAKETAG(0x3006, 0x0010), DCM_SQ, "SSET Referenced Frame of Reference Sequence"},
    {DCM_MAKETAG(0x3006, 0x0012), DCM_SQ, "SSET RT Referenced Study Sequence"},
    {DCM_MAKETAG(0x3006, 0x0014), DCM_SQ, "SSET RT Referenced Series Sequence"},
    {DCM_MAKETAG(0x3006, 0x0016), DCM_SQ, "SSET Contour Image Sequence"},
    {DCM_MAKETAG(0x3006, 0x0020), DCM_SQ, "SSET Structure Set ROI Sequence"},
    {DCM_MAKETAG(0x3006, 0x0022), DCM_IS, "SSET ROI Number"},
    {DCM_MAKETAG(0x3006, 0x0024), DCM_UI, "SSET Referenced Frame of Reference UID"},
    {DCM_MAKETAG(0x3006, 0x0026), DCM_LO, "SSET ROI Name"},
    {DCM_MAKETAG(0x3006, 0x0028), DCM_ST, "SSET ROI Description"},
    {DCM_MAKETAG(0x3006, 0x002a), DCM_IS, "SSET ROI Display Color"},
    {DCM_MAKETAG(0x3006, 0x002c), DCM_DS, "SSET ROI Volume"},
    {DCM_MAKETAG(0x3006, 0x0030), DCM_SQ, "SSET RT Related ROI Sequence"},
    {DCM_MAKETAG(0x3006, 0x0033), DCM_CS, "SSET RT ROI Relationship"},
    {DCM_MAKETAG(0x3006, 0x0036), DCM_CS, "SSET ROI Generation Algorithm"},
    {DCM_MAKETAG(0x3006, 0x0038), DCM_LO, "SSET ROI Generation Description"},
    {DCM_MAKETAG(0x3006, 0x0039), DCM_SQ, "SSET ROI Contour Sequence"},
    {DCM_MAKETAG(0x3006, 0x0040), DCM_SQ, "SSET Contour Sequence"},
    {DCM_MAKETAG(0x3006, 0x0042), DCM_CS, "SSET Contour Geometric Type"},
    {DCM_MAKETAG(0x3006, 0x0044), DCM_DS, "SSET Contour Slab Thickness"},
    {DCM_MAKETAG(0x3006, 0x0045), DCM_DS, "SSET Contour Offset Vector"},
    {DCM_MAKETAG(0x3006, 0x0046), DCM_IS, "SSET Number of Contour Points"},
    {DCM_MAKETAG(0x3006, 0x0050), DCM_DS, "SSET Contour Data"},
    {DCM_MAKETAG(0x3006, 0x0080), DCM_SQ, "SSET RT ROI Observations Sequence"},
    {DCM_MAKETAG(0x3006, 0x0082), DCM_IS, "SSET Observation Number"},
    {DCM_MAKETAG(0x3006, 0x0084), DCM_IS, "SSET Referenced ROI Number"},
    {DCM_MAKETAG(0x3006, 0x0085), DCM_SH, "SSET ROI Observation Label"},
    {DCM_MAKETAG(0x3006, 0x0086), DCM_SQ, "SSET RT ROI Identification Code Sequence"},
    {DCM_MAKETAG(0x3006, 0x0088), DCM_ST, "SSET ROI Observation Description"},
    {DCM_MAKETAG(0x3006, 0x00a0), DCM_SQ, "SSET Relation RT ROI Observations Sequence"},
    {DCM_MAKETAG(0x3006, 0x00a4), DCM_CS, "SSET RT ROI Interpreted Type"},
    {DCM_MAKETAG(0x3006, 0x00a6), DCM_PN, "SSET ROI Interpreter"},
    {DCM_MAKETAG(0x3006, 0x00b0), DCM_SQ, "SSET ROI Physical Properties Sequence"},
    {DCM_MAKETAG(0x3006, 0x00b2), DCM_CS, "SSET ROI Physical Property"},
    {DCM_MAKETAG(0x3006, 0x00b4), DCM_DS, "SSET ROI Physical Property Value"},
    {DCM_MAKETAG(0x3006, 0x00c0), DCM_SQ, "SSET Frame of Referenced Relationship Sequence"},
    {DCM_MAKETAG(0x3006, 0x00c2), DCM_UI, "SSET Related Frame of Reference UID"},
    {DCM_MAKETAG(0x3006, 0x00c4), DCM_CS, "SSET Frame of Reference Transformation Type"},
    {DCM_MAKETAG(0x3006, 0x00c6), DCM_DS, "SSET Frame of Reference Transformation Matrix"},
    {DCM_MAKETAG(0x3006, 0x00c8), DCM_LO, "SSET Frame of Reference Transformation Comment"}
};

/* Define the entries in the 0x300A group, used in RT planning.
*/
static DCMDICT G300A_dictionary[] = {
    {DCM_MAKETAG(0x300a, 0x0000), DCM_UL, "     Group Length"},
    {DCM_MAKETAG(0x300a, 0x0002), DCM_SH, "     RT Plan Label"},
    {DCM_MAKETAG(0x300a, 0x0003), DCM_LO, "     RT Plan Name"},
    {DCM_MAKETAG(0x300a, 0x0004), DCM_ST, "     RT Plan Description"},
    {DCM_MAKETAG(0x300a, 0x0006), DCM_DA, "     RT Plan Date"},
    {DCM_MAKETAG(0x300a, 0x0007), DCM_TM, "     RT Plan Time"},
    {DCM_MAKETAG(0x300a, 0x0009), DCM_LO, "     RT Treatment Protocols"},
    {DCM_MAKETAG(0x300a, 0x000a), DCM_CS, "     Treatment Intent"},
    {DCM_MAKETAG(0x300a, 0x000b), DCM_LO, "     Treatment Sites"},
    {DCM_MAKETAG(0x300a, 0x000c), DCM_CS, "     RT Plan Geometry"},
    {DCM_MAKETAG(0x300a, 0x000e), DCM_ST, "     Prescription Description"},
    {DCM_MAKETAG(0x300a, 0x0010), DCM_SQ, "     Dose Reference Sequence"},
    {DCM_MAKETAG(0x300a, 0x0012), DCM_IS, "     Dose Reference Number"},
    {DCM_MAKETAG(0x300a, 0x0014), DCM_CS, "     Dose Reference Structure Type"},
    {DCM_MAKETAG(0x300a, 0x0016), DCM_LO, "     Dose Reference Description"},
    {DCM_MAKETAG(0x300a, 0x0018), DCM_DS, "     Dose Reference Point Coordinates"},
    {DCM_MAKETAG(0x300a, 0x001a), DCM_DS, "     Nominal Prior Dose"},
    {DCM_MAKETAG(0x300a, 0x0020), DCM_CS, "     Dose Reference Type"},
    {DCM_MAKETAG(0x300a, 0x0021), DCM_DS, "     Constraint Weight"},
    {DCM_MAKETAG(0x300a, 0x0022), DCM_DS, "     Delivery Warning Dose"},
    {DCM_MAKETAG(0x300a, 0x0023), DCM_DS, "     Delivery Maximum Dose"},
    {DCM_MAKETAG(0x300a, 0x0025), DCM_DS, "     Target Minimum Dose"},
    {DCM_MAKETAG(0x300a, 0x0026), DCM_DS, "     Target Prescription Dose"},
    {DCM_MAKETAG(0x300a, 0x0027), DCM_DS, "     Target Maximum Dose"},
    {DCM_MAKETAG(0x300a, 0x0028), DCM_DS, "     Target Underdose Volume Fraction"},
    {DCM_MAKETAG(0x300a, 0x002a), DCM_DS, "     Organ at Risk Full-volume Dose"},
    {DCM_MAKETAG(0x300a, 0x002b), DCM_DS, "     Organ at Risk Limit Dose"},
    {DCM_MAKETAG(0x300a, 0x002c), DCM_DS, "     Organ at Risk Maximum Dose"},
    {DCM_MAKETAG(0x300a, 0x002d), DCM_DS, "     Organ at Risk Overdose Volume Fraction"},
    {DCM_MAKETAG(0x300a, 0x0040), DCM_SQ, "     Tolerance Table Sequence"},
    {DCM_MAKETAG(0x300a, 0x0042), DCM_IS, "     Tolerance Table Number"},
    {DCM_MAKETAG(0x300a, 0x0043), DCM_SH, "     Tolerance Table Label"},
    {DCM_MAKETAG(0x300a, 0x0044), DCM_DS, "     Gantry Angle Tolerance"},
    {DCM_MAKETAG(0x300a, 0x0046), DCM_DS, "     Beam Limiting Device Angle Tolerance"},
    {DCM_MAKETAG(0x300a, 0x0048), DCM_SQ, "     Beam Limiting Device Tolerance Sequence"},
    {DCM_MAKETAG(0x300a, 0x004a), DCM_DS, "     Beam Limiting Device Position Tolerance"},
    {DCM_MAKETAG(0x300a, 0x004c), DCM_DS, "     Patient Support Angle Tolerance"},
    {DCM_MAKETAG(0x300a, 0x004e), DCM_DS, "     Table Top Eccentric Angle Tolerance"},
    {DCM_MAKETAG(0x300a, 0x0051), DCM_DS, "     Table Top Vertical Position Tolerance"},
    {DCM_MAKETAG(0x300a, 0x0052), DCM_DS, "     Table Top Longitudinal Position Tolerance"},
    {DCM_MAKETAG(0x300a, 0x0053), DCM_DS, "     Table Top Lateral Position Tolerance"},
    {DCM_MAKETAG(0x300a, 0x0055), DCM_CS, "     RT Plan Relationship"},
    {DCM_MAKETAG(0x300a, 0x0070), DCM_SQ, "     Fraction Group Sequence"},
    {DCM_MAKETAG(0x300a, 0x0071), DCM_IS, "     Fraction Group Number"},
    {DCM_MAKETAG(0x300a, 0x0078), DCM_IS, "     Number of Fractions Planned"},
    {DCM_MAKETAG(0x300a, 0x0079), DCM_IS, "     Number of Fractions Per Day"},
    {DCM_MAKETAG(0x300a, 0x007a), DCM_IS, "     Repeat Fraction Cycle Length"},
    {DCM_MAKETAG(0x300a, 0x007b), DCM_LT, "     Fraction Pattern"},
    {DCM_MAKETAG(0x300a, 0x0080), DCM_IS, "     Number of Beams"},
    {DCM_MAKETAG(0x300a, 0x0082), DCM_DS, "     Beam Dose Specification Point"},
    {DCM_MAKETAG(0x300a, 0x0084), DCM_DS, "     Beam Dose"},
    {DCM_MAKETAG(0x300a, 0x0086), DCM_DS, "     Beam Meterset"},
    {DCM_MAKETAG(0x300a, 0x00a0), DCM_IS, "     Number of Brachy Application Setups"},
    {DCM_MAKETAG(0x300a, 0x00a2), DCM_DS, "     Brachy App Setup Dose Specification Point"},
    {DCM_MAKETAG(0x300a, 0x00a4), DCM_DS, "     Brachy Application Setup Dose"},
    {DCM_MAKETAG(0x300a, 0x00b0), DCM_SQ, "     Beam Sequence"},
    {DCM_MAKETAG(0x300a, 0x00b2), DCM_SH, "     Treatment Machine Name"},
    {DCM_MAKETAG(0x300a, 0x00b3), DCM_CS, "     Primary Dosimeter Unit"},
    {DCM_MAKETAG(0x300a, 0x00b4), DCM_DS, "     Source-Axis Distance"},
    {DCM_MAKETAG(0x300a, 0x00b6), DCM_SQ, "     Beam Limiting Device Sequence"},
    {DCM_MAKETAG(0x300a, 0x00b8), DCM_CS, "     RT Beam Limiting Device Type"},
    {DCM_MAKETAG(0x300a, 0x00ba), DCM_DS, "     Source to Beam Limiting Device Distance"},
    {DCM_MAKETAG(0x300a, 0x00bc), DCM_IS, "     Number of Leaf/Jaw Pairs"},
    {DCM_MAKETAG(0x300a, 0x00be), DCM_DS, "     Leaf Position Boundaries"},
    {DCM_MAKETAG(0x300a, 0x00c0), DCM_IS, "     Beam Number"},
    {DCM_MAKETAG(0x300a, 0x00c2), DCM_LO, "     Beam Name"},
    {DCM_MAKETAG(0x300a, 0x00c3), DCM_ST, "     Beam Description"},
    {DCM_MAKETAG(0x300a, 0x00c4), DCM_CS, "     Beam Type"},
    {DCM_MAKETAG(0x300a, 0x00c6), DCM_CS, "     Radiation Type"},
    {DCM_MAKETAG(0x300a, 0x00c8), DCM_IS, "     Reference Image Number"},
    {DCM_MAKETAG(0x300a, 0x00ca), DCM_SQ, "     Planned Verification Image Sequence"},
    {DCM_MAKETAG(0x300a, 0x00cc), DCM_LO, "     Imaging Device-Specific Acq Parameters"},
    {DCM_MAKETAG(0x300a, 0x00ce), DCM_CS, "     Treatment Delivery Type"},
    {DCM_MAKETAG(0x300a, 0x00d0), DCM_IS, "     Number of Wedges"},
    {DCM_MAKETAG(0x300a, 0x00d1), DCM_SQ, "     Wedge Sequence"},
    {DCM_MAKETAG(0x300a, 0x00d2), DCM_IS, "     Wedge Number"},
    {DCM_MAKETAG(0x300a, 0x00d3), DCM_CS, "     Wedge Type"},
    {DCM_MAKETAG(0x300a, 0x00d4), DCM_SH, "     Wedge ID"},
    {DCM_MAKETAG(0x300a, 0x00d5), DCM_IS, "     Wedge Angle"},
    {DCM_MAKETAG(0x300a, 0x00d6), DCM_DS, "     Wedge Factor"},
    {DCM_MAKETAG(0x300a, 0x00d8), DCM_DS, "     Wedge Orientation"},
    {DCM_MAKETAG(0x300a, 0x00da), DCM_DS, "     Source to Wedge Tray Distance"},
    {DCM_MAKETAG(0x300a, 0x00e0), DCM_IS, "     Number of Compensators"},
    {DCM_MAKETAG(0x300a, 0x00e1), DCM_SH, "     Material ID"},
    {DCM_MAKETAG(0x300a, 0x00e2), DCM_DS, "     Total Compensator Tray Factor"},
    {DCM_MAKETAG(0x300a, 0x00e3), DCM_SQ, "     Compensator Sequence"},
    {DCM_MAKETAG(0x300a, 0x00e4), DCM_IS, "     Compensator Number"},
    {DCM_MAKETAG(0x300a, 0x00e5), DCM_SH, "     Compensator ID"},
    {DCM_MAKETAG(0x300a, 0x00e6), DCM_DS, "     Source to Compensator Tray Distance"},
    {DCM_MAKETAG(0x300a, 0x00e7), DCM_IS, "     Compensator Rows"},
    {DCM_MAKETAG(0x300a, 0x00e8), DCM_IS, "     Compensator Columns"},
    {DCM_MAKETAG(0x300a, 0x00e9), DCM_DS, "     Compensator Pixel Spacing"},
    {DCM_MAKETAG(0x300a, 0x00ea), DCM_DS, "     Compensator Position"},
    {DCM_MAKETAG(0x300a, 0x00eb), DCM_DS, "     Compensator Transmission Data"},
    {DCM_MAKETAG(0x300a, 0x00ec), DCM_DS, "     Compensator Thickness Data"},
    {DCM_MAKETAG(0x300a, 0x00ed), DCM_IS, "     Number of Boli"},
    {DCM_MAKETAG(0x300a, 0x00f0), DCM_IS, "     Number of Blocks"},
    {DCM_MAKETAG(0x300a, 0x00f2), DCM_DS, "     Total Block Tray Factor"},
    {DCM_MAKETAG(0x300a, 0x00f4), DCM_SQ, "     Block Sequence"},
    {DCM_MAKETAG(0x300a, 0x00f5), DCM_SH, "     Block Tray ID"},
    {DCM_MAKETAG(0x300a, 0x00f6), DCM_DS, "     Source to Block Tray Distance"},
    {DCM_MAKETAG(0x300a, 0x00f8), DCM_CS, "     Block Type"},
    {DCM_MAKETAG(0x300a, 0x00fa), DCM_CS, "     Block Divergence"},
    {DCM_MAKETAG(0x300a, 0x00fc), DCM_IS, "     Block Number"},
    {DCM_MAKETAG(0x300a, 0x00fe), DCM_LO, "     Block Name"},
    {DCM_MAKETAG(0x300a, 0x0100), DCM_DS, "     Block Thickness"},
    {DCM_MAKETAG(0x300a, 0x0102), DCM_DS, "     Block Transmission"},
    {DCM_MAKETAG(0x300a, 0x0104), DCM_IS, "     Block Number of Points"},
    {DCM_MAKETAG(0x300a, 0x0106), DCM_DS, "     Block Data"},
    {DCM_MAKETAG(0x300a, 0x0107), DCM_SQ, "     Applicator Sequence"},
    {DCM_MAKETAG(0x300a, 0x0108), DCM_SH, "     Applicator ID"},
    {DCM_MAKETAG(0x300a, 0x0109), DCM_CS, "     Applicator Type"},
    {DCM_MAKETAG(0x300a, 0x010a), DCM_LO, "     Applicator Description"},
    {DCM_MAKETAG(0x300a, 0x010c), DCM_DS, "     Cumulative Dose Reference COefficient"},
    {DCM_MAKETAG(0x300a, 0x010e), DCM_DS, "     Final Cumulative Meterset Weight"},
    {DCM_MAKETAG(0x300a, 0x0110), DCM_IS, "     Number of Control Points"},
    {DCM_MAKETAG(0x300a, 0x0111), DCM_SQ, "     Control Point Sequence"},
    {DCM_MAKETAG(0x300a, 0x0112), DCM_IS, "     Control Point Index"},
    {DCM_MAKETAG(0x300a, 0x0114), DCM_DS, "     Nominal Beam Energy"},
    {DCM_MAKETAG(0x300a, 0x0115), DCM_DS, "     Dose Rate Set"},
    {DCM_MAKETAG(0x300a, 0x0116), DCM_SQ, "     Wedge Position Sequence"},
    {DCM_MAKETAG(0x300a, 0x0118), DCM_CS, "     Wedge Position"},
    {DCM_MAKETAG(0x300a, 0x011a), DCM_SQ, "     Beam Limiting Device Position Sequence"},
    {DCM_MAKETAG(0x300a, 0x011c), DCM_DS, "     Leaf/Jaw Positions"},
    {DCM_MAKETAG(0x300a, 0x011e), DCM_DS, "     Gantry Angle"},
    {DCM_MAKETAG(0x300a, 0x011f), DCM_CS, "     Gantry Rotation Direction"},
    {DCM_MAKETAG(0x300a, 0x0120), DCM_DS, "     Beam Limiting Device Angle"},
    {DCM_MAKETAG(0x300a, 0x0121), DCM_CS, "     Beam Limiting Device Rotation Direction"},
    {DCM_MAKETAG(0x300a, 0x0122), DCM_DS, "     Patient Support Angle"},
    {DCM_MAKETAG(0x300a, 0x0123), DCM_CS, "     Patient Support Rotation Direction"},
    {DCM_MAKETAG(0x300a, 0x0124), DCM_DS, "     Table Top Eccentric Axis Distance"},
    {DCM_MAKETAG(0x300a, 0x0125), DCM_DS, "     Table Top Eccentric Angle"},
    {DCM_MAKETAG(0x300a, 0x0126), DCM_CS, "     Table Top Eccentric Rotation Direction"},
    {DCM_MAKETAG(0x300a, 0x0128), DCM_DS, "     Table Top Vertical Position"},
    {DCM_MAKETAG(0x300a, 0x0129), DCM_DS, "     Table Top Longitudinal Position"},
    {DCM_MAKETAG(0x300a, 0x012a), DCM_DS, "     Table Top Lateral Position"},
    {DCM_MAKETAG(0x300a, 0x012c), DCM_DS, "     Isocenter Position"},
    {DCM_MAKETAG(0x300a, 0x012e), DCM_DS, "     Surface Entry Point"},
    {DCM_MAKETAG(0x300a, 0x0130), DCM_DS, "     Source to Surface Distance"},
    {DCM_MAKETAG(0x300a, 0x0134), DCM_DS, "     Cumulative Meterset Weight"},
    {DCM_MAKETAG(0x300a, 0x0180), DCM_SQ, "     Patient Setup Sequence"},
    {DCM_MAKETAG(0x300a, 0x0182), DCM_IS, "     Patient Setup Number"},
    {DCM_MAKETAG(0x300a, 0x0184), DCM_LO, "     Patient Additional Position"},
    {DCM_MAKETAG(0x300a, 0x0190), DCM_SQ, "     Fixation Device Sequence"},
    {DCM_MAKETAG(0x300a, 0x0192), DCM_CS, "     Fixation Device Type"},
    {DCM_MAKETAG(0x300a, 0x0194), DCM_SH, "     Fixation Device Label"},
    {DCM_MAKETAG(0x300a, 0x0196), DCM_ST, "     Fixation Device Description"},
    {DCM_MAKETAG(0x300a, 0x0198), DCM_SH, "     Fixation Device Position"},
    {DCM_MAKETAG(0x300a, 0x01a0), DCM_SQ, "     Shielding Device Sequence"},
    {DCM_MAKETAG(0x300a, 0x01a2), DCM_CS, "     Shielding Device Type"},
    {DCM_MAKETAG(0x300a, 0x01a4), DCM_SH, "     Shielding Device Label"},
    {DCM_MAKETAG(0x300a, 0x01a6), DCM_ST, "     Shielding Device Description"},
    {DCM_MAKETAG(0x300a, 0x01a8), DCM_SH, "     Shielding Device Position"},
    {DCM_MAKETAG(0x300a, 0x01b0), DCM_CS, "     Setup Technique"},
    {DCM_MAKETAG(0x300a, 0x01b2), DCM_ST, "     Setup Technique Description"},
    {DCM_MAKETAG(0x300a, 0x01b4), DCM_SQ, "     Setup Device Sequence"},
    {DCM_MAKETAG(0x300a, 0x01b6), DCM_CS, "     Setup Device Type"},
    {DCM_MAKETAG(0x300a, 0x01b8), DCM_SH, "     Setup Device Label"},
    {DCM_MAKETAG(0x300a, 0x01ba), DCM_ST, "     Setup Device Description"},
    {DCM_MAKETAG(0x300a, 0x01bc), DCM_DS, "     Setup Device Parameter"},
    {DCM_MAKETAG(0x300a, 0x01d0), DCM_ST, "     Setup Reference Description"},
    {DCM_MAKETAG(0x300a, 0x01d2), DCM_DS, "     Table Top Vertical Setup Displacement"},
    {DCM_MAKETAG(0x300a, 0x01d4), DCM_DS, "     Table Top Longitudinal Setup Displacement"},
    {DCM_MAKETAG(0x300a, 0x01d6), DCM_DS, "     Table Top Lateral Setup Displacement"},
    {DCM_MAKETAG(0x300a, 0x0200), DCM_CS, "     Brachy Treatment Technique"},
    {DCM_MAKETAG(0x300a, 0x0202), DCM_CS, "     Brachy Treatment Type"},
    {DCM_MAKETAG(0x300a, 0x0206), DCM_SQ, "     Treatment Machine Sequence"},
    {DCM_MAKETAG(0x300a, 0x0210), DCM_SQ, "     Source Sequence"},
    {DCM_MAKETAG(0x300a, 0x0212), DCM_IS, "     Source Number"},
    {DCM_MAKETAG(0x300a, 0x0214), DCM_CS, "     Source Type"},
    {DCM_MAKETAG(0x300a, 0x0216), DCM_LO, "     Source Manufacturer"},
    {DCM_MAKETAG(0x300a, 0x0218), DCM_DS, "     Active Source Diameter"},
    {DCM_MAKETAG(0x300a, 0x021a), DCM_DS, "     Active Source Length"},
    {DCM_MAKETAG(0x300a, 0x0222), DCM_DS, "     Source Encapsulation Nominal Thickness"},
    {DCM_MAKETAG(0x300a, 0x0224), DCM_DS, "     Source Encapsulation Nominal Transmission"},
    {DCM_MAKETAG(0x300a, 0x0226), DCM_LO, "     Source Isotope Name"},
    {DCM_MAKETAG(0x300a, 0x0228), DCM_DS, "     Source Isotope Half Life"},
    {DCM_MAKETAG(0x300a, 0x022a), DCM_DS, "     Reference Air Kerma Rate"},
    {DCM_MAKETAG(0x300a, 0x022c), DCM_DA, "     Air Kerma Rate Reference Date"},
    {DCM_MAKETAG(0x300a, 0x022e), DCM_TM, "     Air Kerma Rate Reference Time"},
    {DCM_MAKETAG(0x300a, 0x0230), DCM_SQ, "     Application Setup Sequence"},
    {DCM_MAKETAG(0x300a, 0x0232), DCM_CS, "     Application Setup Type"},
    {DCM_MAKETAG(0x300a, 0x0234), DCM_IS, "     Application Setup Number"},
    {DCM_MAKETAG(0x300a, 0x0236), DCM_LO, "     Application Setup Name"},
    {DCM_MAKETAG(0x300a, 0x0238), DCM_LO, "     Application Setup Manufacturer"},
    {DCM_MAKETAG(0x300a, 0x0240), DCM_IS, "     Template Number"},
    {DCM_MAKETAG(0x300a, 0x0242), DCM_SH, "     Template Type"},
    {DCM_MAKETAG(0x300a, 0x0244), DCM_LO, "     Template Name"},
    {DCM_MAKETAG(0x300a, 0x0250), DCM_DS, "     Total Reference Air Kerma"},
    {DCM_MAKETAG(0x300a, 0x0260), DCM_SQ, "     Brachy Acessory Device Sequence"},
    {DCM_MAKETAG(0x300a, 0x0262), DCM_IS, "     Brachy Accessory Device Number"},
    {DCM_MAKETAG(0x300a, 0x0263), DCM_SH, "     Brachy Accessory Device ID"},
    {DCM_MAKETAG(0x300a, 0x0264), DCM_CS, "     Brachy Accessory Device Type"},
    {DCM_MAKETAG(0x300a, 0x0266), DCM_LO, "     Brachy Accessory Device Name"},
    {DCM_MAKETAG(0x300a, 0x026a), DCM_DS, "     Brachy Accessory Device Nominal Thickness"},
    {DCM_MAKETAG(0x300a, 0x026c), DCM_DS, "     Brachy Acc'ry Device Nominal Transmission"},
    {DCM_MAKETAG(0x300a, 0x0280), DCM_SQ, "     Channel Sequence"},
    {DCM_MAKETAG(0x300a, 0x0282), DCM_IS, "     Channel Number"},
    {DCM_MAKETAG(0x300a, 0x0284), DCM_DS, "     Channel Length"},
    {DCM_MAKETAG(0x300a, 0x0286), DCM_DS, "     Channel Total Time"},
    {DCM_MAKETAG(0x300a, 0x0288), DCM_CS, "     Source Movement Type"},
    {DCM_MAKETAG(0x300a, 0x028a), DCM_IS, "     Number of Pulses"},
    {DCM_MAKETAG(0x300a, 0x028c), DCM_DS, "     Pulse Repetition Interval"},
    {DCM_MAKETAG(0x300a, 0x0290), DCM_IS, "     Source Applicator Number"},
    {DCM_MAKETAG(0x300a, 0x0291), DCM_SH, "     Source Applicator ID"},
    {DCM_MAKETAG(0x300a, 0x0292), DCM_CS, "     Source Applicator Type"},
    {DCM_MAKETAG(0x300a, 0x0294), DCM_LO, "     Source Applicator Name"},
    {DCM_MAKETAG(0x300a, 0x0296), DCM_DS, "     Source Applicator Length"},
    {DCM_MAKETAG(0x300a, 0x0298), DCM_LO, "     Source Applicator Manufacturer"},
    {DCM_MAKETAG(0x300a, 0x029c), DCM_DS, "     Source Applicator Wall Nominal Thickness"},
    {DCM_MAKETAG(0x300a, 0x029e), DCM_DS, "     Src Applicator Wall Nominal Transmission"},
    {DCM_MAKETAG(0x300a, 0x02a0), DCM_DS, "     Source Applicator Step Size"},
    {DCM_MAKETAG(0x300a, 0x02a2), DCM_IS, "     Transfer Tube Number"},
    {DCM_MAKETAG(0x300a, 0x02a4), DCM_DS, "     Transfer Tube Length"},
    {DCM_MAKETAG(0x300a, 0x02b0), DCM_SQ, "     Channel Shield Sequence"},
    {DCM_MAKETAG(0x300a, 0x02b2), DCM_IS, "     Channel Shield Number"},
    {DCM_MAKETAG(0x300a, 0x02b3), DCM_SH, "     Channel Shield ID"},
    {DCM_MAKETAG(0x300a, 0x02b4), DCM_LO, "     Channel Shield Name"},
    {DCM_MAKETAG(0x300a, 0x02b8), DCM_DS, "     Channel Shield Nominal Thickness"},
    {DCM_MAKETAG(0x300a, 0x02ba), DCM_DS, "     Channel Shield Nominal Transmission"},
    {DCM_MAKETAG(0x300a, 0x02c8), DCM_DS, "     Final Cumulative Time Weight"},
    {DCM_MAKETAG(0x300a, 0x02d0), DCM_SQ, "     Brachy Control Point Sequence"},
    {DCM_MAKETAG(0x300a, 0x02d2), DCM_DS, "   Control Point Relative Position"},
    {DCM_MAKETAG(0x300a, 0x02d4), DCM_DS, "     Control Point 3D Position"},
    {DCM_MAKETAG(0x300a, 0x02d6), DCM_DS, "     Cumulative Time Weight"}
};

/* Define the entries in the 0x300C group, used in RT planning.
*/
static DCMDICT G300C_dictionary[] = {
    {DCM_MAKETAG(0x300c, 0x0000), DCM_UL, "     Group Length"},
    {DCM_MAKETAG(0x300c, 0x0002), DCM_SQ, "     Referenced RT Plan Sequence"},
    {DCM_MAKETAG(0x300c, 0x0004), DCM_SQ, "     Referenced Beam Sequence"},
    {DCM_MAKETAG(0x300c, 0x0006), DCM_IS, "     Referenced Beam Number"},
    {DCM_MAKETAG(0x300c, 0x0007), DCM_IS, "     Referenced Reference Image Number"},
    {DCM_MAKETAG(0x300c, 0x0008), DCM_DS, "     Start Cumulative Meterset Weight"},
    {DCM_MAKETAG(0x300c, 0x0009), DCM_DS, "     End Cumulative Meterset Weight"},
    {DCM_MAKETAG(0x300c, 0x000a), DCM_SQ, "     Referenced Brachy Application Setup Seq"},
    {DCM_MAKETAG(0x300c, 0x000c), DCM_IS, "     Referenced Brachy Application Setup Number"},
    {DCM_MAKETAG(0x300c, 0x000e), DCM_IS, "     Referenced Source Number"},
    {DCM_MAKETAG(0x300c, 0x0020), DCM_SQ, "     Referenced Fraction Group Sequence"},
    {DCM_MAKETAG(0x300c, 0x0022), DCM_IS, "     Referenced Fraction Group Number"},
    {DCM_MAKETAG(0x300c, 0x0040), DCM_SQ, "     Referenced Verification Image Sequence"},
    {DCM_MAKETAG(0x300c, 0x0042), DCM_SQ, "     Referenced Reference Image Sequence"},
    {DCM_MAKETAG(0x300c, 0x0050), DCM_SQ, "     Referenced Dose Reference Sequence"},
    {DCM_MAKETAG(0x300c, 0x0051), DCM_IS, "     Referenced Dose Reference Numer"},
    {DCM_MAKETAG(0x300c, 0x0055), DCM_SQ, "     Brachy Referenced Dose Reference Sequence"},
    {DCM_MAKETAG(0x300c, 0x0060), DCM_SQ, "     Referenced Structure Set Sequence"},
    {DCM_MAKETAG(0x300c, 0x006a), DCM_IS, "     Referenced Patient Setup Number"},
    {DCM_MAKETAG(0x300c, 0x0080), DCM_SQ, "     Referenced Dose Sequence"},
    {DCM_MAKETAG(0x300c, 0x00a0), DCM_IS, "     Referenced Tolerance Table Number"},
    {DCM_MAKETAG(0x300c, 0x00b0), DCM_SQ, "     Referenced Bolus Sequence"},
    {DCM_MAKETAG(0x300c, 0x00c0), DCM_IS, "     Referenced Wedge Number"},
    {DCM_MAKETAG(0x300c, 0x00d0), DCM_IS, "     Referenced Compensator Number"},
    {DCM_MAKETAG(0x300c, 0x00e0), DCM_IS, "     Referenced Block Number"},
    {DCM_MAKETAG(0x300c, 0x00f0), DCM_IS, "     Referenced Control Point Index"}
};


/* Define the entries in the 0x300E group, used in RT planning.
*/
static DCMDICT G300E_dictionary[] = {
    {DCM_MAKETAG(0x300e, 0x0000), DCM_UL, "     Group Length"},
    {DCM_MAKETAG(0x300e, 0x0002), DCM_CS, "     Approval Status"},
    {DCM_MAKETAG(0x300e, 0x0004), DCM_DA, "     Review Date"},
    {DCM_MAKETAG(0x300e, 0x0005), DCM_TM, "     Review Time"},
    {DCM_MAKETAG(0x300e, 0x0008), DCM_PN, "     Reviewer Name"}
};

/* Define the entries in the 0x0400 group
*/
static DCMDICT G0400_dictionary[] = {
    {DCM_MAKETAG(0x0400, 0x0000), DCM_UL, "     Group Length"},
    {DCM_MAKETAG(0x0400, 0x0561), DCM_SQ, "     Original Attributes Sequence"},
    {DCM_MAKETAG(0x0400, 0x0562), DCM_DT, "     Attribute Modification DateTime"},
    {DCM_MAKETAG(0x0400, 0x0563), DCM_LO, "     Modifying System"},
    {DCM_MAKETAG(0x0400, 0x0564), DCM_LO, "     Source of Previous Values"},
    {DCM_MAKETAG(0x0400, 0x0565), DCM_CS, "     Reseaon for Attr. Modification"},
    {DCM_MAKETAG(0x0400, 0x0550), DCM_SQ, "     Modified Attribute Sequence"}
};

/* Defines the entries in the Text group (4000)
*/
#if 0
static DCMDICT TXT_dictionary[] = {
};
#endif

/* Define the entries in the PAD group, 0xfffc
*/

static DCMDICT PAD_dictionary[] = {
    {DCM_PADITEM, DCM_OB, "Pad item"}
};

/* Define the entries in the DELIMITER group, 0xfffe
*/

static DCMDICT DLM_dictionary[] = {
    {DCM_DLMITEM, DCM_DLM, "DELIMITER Item"},
    {DCM_DLMITEMDELIMITATIONITEM, DCM_DLM, "DELIMITER Item Delimitation Item"},
    {DCM_DLMSEQUENCEDELIMITATIONITEM, DCM_DLM, "DELIMITER Sequence Delimitation Item"}
};

/* Define the outer layer dictionary which contains group numbers and
** pointers to each of the individual group lists.
*/

static GROUPPTR group_dictionary[] = {
    {DCM_GROUPCOMMAND, sizeof(CMD_dictionary) / sizeof(DCMDICT), CMD_dictionary},
    {DCM_GROUPFILEMETA, sizeof(META_dictionary) / sizeof(DCMDICT), META_dictionary},
    {DCM_GROUPBASICDIRINFO, sizeof(BASICDIR_dictionary) / sizeof(DCMDICT), BASICDIR_dictionary},
    {DCM_GROUPIDENTIFYING,
    sizeof(ID_dictionary) / sizeof(DCMDICT), ID_dictionary},
    {0x0012,
    sizeof(G0012_dictionary) / sizeof(DCMDICT), G0012_dictionary},
    {DCM_GROUPPATIENTINFO,
    sizeof(PAT_dictionary) / sizeof(DCMDICT), PAT_dictionary},
    {DCM_GROUPACQUISITION,
    sizeof(ACQ_dictionary) / sizeof(DCMDICT), ACQ_dictionary},
    {DCM_GROUPRELATIONSHIP,
    sizeof(REL_dictionary) / sizeof(DCMDICT), REL_dictionary},
    {DCM_GROUPIMAGE,
    sizeof(IMG_dictionary) / sizeof(DCMDICT), IMG_dictionary},
    {DCM_GROUPSTUDY,
    sizeof(SDY_dictionary) / sizeof(DCMDICT), SDY_dictionary},
    {0x0042,
    sizeof(DOC_dictionary) / sizeof(DCMDICT), DOC_dictionary},
    {DCM_GROUPVISIT,
    sizeof(VIS_dictionary) / sizeof(DCMDICT), VIS_dictionary},
    {DCM_GROUPWAVEFORM,
    sizeof(WAV_dictionary) / sizeof(DCMDICT), WAV_dictionary},
    {DCM_GRPPROCEDURE,
    sizeof(PRC_dictionary) / sizeof(DCMDICT), PRC_dictionary},
    {DCM_GROUPDEVICE,
    sizeof(DEV_dictionary) / sizeof(DCMDICT), DEV_dictionary},
    {DCM_GROUPNMIMAGE,
    sizeof(NMI_dictionary) / sizeof(DCMDICT), NMI_dictionary},
    {DCM_GROUPGRAPHICS,
    sizeof(GRP_dictionary) / sizeof(DCMDICT), GRP_dictionary},
    {0x0074,
    sizeof(G0074_dictionary) / sizeof(DCMDICT), G0074_dictionary},
    {DCM_GROUPMEDIA,
    sizeof(MED_dictionary) / sizeof(DCMDICT), MED_dictionary},
    {DCM_GROUPBASICFILMSESSION,
    sizeof(BFS_dictionary) / sizeof(DCMDICT), BFS_dictionary},
    {DCM_GROUPBASICFILMBOX,
    sizeof(BFB_dictionary) / sizeof(DCMDICT), BFB_dictionary},
    {DCM_GROUPBASICIMAGEBOX,
    sizeof(BIB_dictionary) / sizeof(DCMDICT), BIB_dictionary},
    {DCM_GROUPBASICANNOTATIONBOX,
    sizeof(BAB_dictionary) / sizeof(DCMDICT), BAB_dictionary},

    {DCM_GROUPBASICIMAGEOVERLAYBOX,
    sizeof(IOB_dictionary) / sizeof(DCMDICT), IOB_dictionary},

    {0x2050,
    sizeof(PLUT_dictionary) / sizeof(DCMDICT), PLUT_dictionary},

    {DCM_GROUPPRINTJOB,
    sizeof(PJ_dictionary) / sizeof(DCMDICT), PJ_dictionary},

    {DCM_GROUPPRINTER,
    sizeof(PRN_dictionary) / sizeof(DCMDICT), PRN_dictionary},
    {0x3002,
    sizeof(G3002_dictionary) / sizeof(DCMDICT), G3002_dictionary},
    {0x3004,
    sizeof(DVH_dictionary) / sizeof(DCMDICT), DVH_dictionary},
    {0x3006,
    sizeof(SSET_dictionary) / sizeof(DCMDICT), SSET_dictionary},
    {0x300a,
    sizeof(G300A_dictionary) / sizeof(DCMDICT), G300A_dictionary},
    {0x300c,
    sizeof(G300C_dictionary) / sizeof(DCMDICT), G300C_dictionary},
    {0x300e,
    sizeof(G300E_dictionary) / sizeof(DCMDICT), G300E_dictionary},
    {0x0400,
    sizeof(G0400_dictionary) / sizeof(DCMDICT), G0400_dictionary},
    

/*  Add this entry in when we define retired attributes
**  in text group.
*/
#if 0
    {DCM_GROUPTEXT,
    sizeof(TXT_dictionary) / sizeof(DCMDICT), TXT_dictionary},
#endif
    {DCM_GROUPRESULTS,
    sizeof(RES_dictionary) / sizeof(DCMDICT), RES_dictionary},
    {0x5200,
    sizeof(G5200_dictionary) / sizeof(DCMDICT), G5200_dictionary},
    {DCM_GROUPCURVE,
    sizeof(CRV_dictionary) / sizeof(DCMDICT), CRV_dictionary},
    {DCM_GROUPOVERLAY,
    sizeof(OLY_dictionary) / sizeof(DCMDICT), OLY_dictionary},
    {DCM_GROUPPIXEL,
    sizeof(PXL_dictionary) / sizeof(DCMDICT), PXL_dictionary},
    {DCM_GROUPPAD,
    sizeof(PAD_dictionary) / sizeof(DCMDICT), PAD_dictionary},
    {DCM_GROUPDELIMITER,
    sizeof(DLM_dictionary) / sizeof(DCMDICT), DLM_dictionary}
};


/* DCM_LookupElement
**
** Purpose:
**	Lookup an element in the DICOM dictionary and return information
**	about the element, including representation, type and english
**	description.
**
** Parameter Dictionary:
**	element		Pointer to an DCM element (group, element) to
**			be found in the dictionary.
**
** Return Values:
**	DCM_NORMAL
**	DCM_UNRECOGNIZEDGROUP
**	DCM_UNRECOGNIZEDELEMENT
**
** Algorithm:
**	Set representation, type, englishDescription fields of caller's
**	element to NULL values
**	Search group_dictionary to find caller's group.
**	If group not found,
**	    return DCM_UNRECOGNIZEDGROUP
**	Search particular group list to find caller's element.
**	If element not found,
**	    return DCM_UNRECOGNIZEDELEMENT
**	Else
**	    Copy representation, type, englishDescription from dictionary
**	    to caller's element
**	    return DCM_NORMAL
**	EndIf
*/

CONDITION
DCM_LookupElement(DCM_ELEMENT * element)
{
    int
        found;
    unsigned long
        index,
        entries;
    GROUPPTR
	* p;
    DCMDICT
	* dictionaryPtr;

    element->representation = DCM_UN;
    (void) strcpy(element->description, "");

    for (index = 0, p = NULL;
	 index < sizeof(group_dictionary) / sizeof(group_dictionary[0]) && p == NULL;
	 index++)
	if (DCM_TAG_GROUP(element->tag) == group_dictionary[index].group)
	    p = &group_dictionary[index];

    if (p == NULL) {
	if (DCM_TAG_ELEMENT(element->tag) == 0x0000) {
	    element->representation = DCM_UL;
	    (void) strcpy(element->description, "Unknown group length");
	    return DCM_NORMAL;
	}
	return COND_PushCondition(DCM_UNRECOGNIZEDGROUP,
				  DCM_Message(DCM_UNRECOGNIZEDGROUP),
				  DCM_TAG_GROUP(element->tag),
				  "DCM_LookupElement");
    }
    entries = p->entries;
    dictionaryPtr = p->dict;

    for (found = 0; !found && entries > 0; entries--)
	if (element->tag == dictionaryPtr->tag)
	    found++;
	else
	    dictionaryPtr++;

    if (!found)
	return COND_PushCondition(DCM_UNRECOGNIZEDELEMENT,
				  DCM_Message(DCM_UNRECOGNIZEDELEMENT),
				  DCM_TAG_GROUP(element->tag),
				  DCM_TAG_ELEMENT(element->tag),
				  "DCM_LookupElement");


    element->representation = dictionaryPtr->representation;
    (void) strcpy(element->description, dictionaryPtr->englishDescription);
    return DCM_NORMAL;
}

typedef struct {
    unsigned short group;
    char *description;
}   GROUP_DESCRIPTION;

static GROUP_DESCRIPTION groupTable[] = {
    {0x0000, "Command"},
    {0x0002, "File Meta"},
    {0x0004, "Basic Directory Information"},
    {0x0008, "Identifying"},
    {0x0010, "Patient Information"},
    {0x0012, "Clinical Trial"},
    {0x0018, "Acquisition"},
    {0x0020, "Relationship"},
    {0x0028, "Image"},
    {0x0032, "Study"},
    {0x0038, "Visit"},
    {0x003a, "Waveform"},
    {0x0040, "Procedure Step"},
    {0x0042, "Documents"},
    {0x0050, "Device"},
    {0x0054, "NM Image"},
    {0x0070, "Graphics"},
    {0x0074, "Procedures"},
    {0x0088, "Media"},
    {0x2000, "Basic Film Session"},
    {0x2010, "Basic Film Box"},
    {0x2020, "Basic Image Box"},
    {0x2030, "Basic Annotation Box"},
    {0x2040, "Basic Image Overlay Box"},
    {0x2050, "Presentation LUT"},
    {0x2100, "Print Job"},
    {0x2110, "Printer"},
    {0x3002, "RT"},
    {0x3004, "Dose Volume Histogram"},
    {0x3006, "Structure Set"},
    {0x300a, "300a"},
    {0x300c, "300c"},
    {0x300e, "300e"},
#if 0
    {0x4000, "Text"},
#endif
    {0x4008, "Results"},
    {0x5000, "Curve"},
    {0x5200, "Functional"},
    {0x6000, "Overlay"},
    {0x7fe0, "Pixel"}
};


/* DCM_GroupDictionary
**
** Purpose:
**	DCM_GroupDictionary is used to lookup descriptions of groups in
**	the internal DCM group dictionary.  Caller specifies one group
**	with a group number or all groups by passing 0xffff.  For each
**	group that matches (the one group or wildcard), this function
**	invokes the caller's callback function.
**	When the callback function is invoked, the arguments are the
**	group number, an ASCII description of the group and user context
**	information that was passed by the caller originally.
**
** Parameter Dictionary:
**	group		The number of the group to be found in the dictionary.
**	ctx		User context information to be passed to callback
**			function.
**	callback	The user's callback function, invoked once for each
**			group that is found during the dictionary lookup.
**
** Return Values:
**	DCM_NORMAL
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
DCM_GroupDictionary(unsigned short group, void *ctx,
	  void (*callback) (unsigned short g, char *description, void *ctx))
{
    int i;

    for (i = 0; i < (int) DIM_OF(groupTable); i++) {
	if ((group == 0xffff) || (group == groupTable[i].group)) {
	    callback(groupTable[i].group, groupTable[i].description, ctx);
	}
    }
    return DCM_NORMAL;
}

/* DCM_ElementDictionary
**
** Purpose:
**	DCM_ElementDictionary is used to lookup descriptions of elements in
**	the internal DCM element dictionary.  The caller can specify one
**	element to be found or a number of elements as follows:
**		(Group,  Element)	Description
**		GGGG,    EEEE		Lookup one particular element (GGGGEEEE)
**		GGGG,    0xffff		Lookup all elements in group GGGG
**		0xffff,  EEEE		Lookup all elements in all groups with
**					element number EEEE
**		0xffff,  0xffff		Lookup all elements in all groups
**	For each element that matches (the one element or wildcard), this
**	function invokes the caller's callback function.
**	When the callback function is invoked, the arguments are the
**	element tag, an ASCII description of the element, the element value
**	representation and user context information that was passed by
**	the caller originally.
**
** Parameter Dictionary:
**	tag		The tag of the element to be found in the dictionary.
**	ctx		User context information to be passed to callback
**			function.
**	callback	The user's callback function, invoked once for each
**			element that is found during the dictionary lookup.
**
** Return Values:
**	DCM_NORMAL
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
DCM_ElementDictionary(DCM_TAG tag, void *ctx,
  void (*callback) (DCM_TAG t, char *description, DCM_VALUEREPRESENTATION r,
		    void *ctx))
{
    int i;
    unsigned long j;
    GROUPPTR *p;
    DCMDICT *dictionaryPtr;

    for (i = 0; i < (int) DIM_OF(group_dictionary); i++) {
	if ((DCM_TAG_GROUP(tag) == group_dictionary[i].group) ||
	    (DCM_TAG_GROUP(tag) == 0xffff)) {
	    p = &group_dictionary[i];
	    dictionaryPtr = p->dict;
	    for (j = 0; j < p->entries; j++, dictionaryPtr++) {
		if ((DCM_TAG_ELEMENT(tag) == 0xffff) ||
		    (DCM_TAG_ELEMENT(tag) == DCM_TAG_ELEMENT(dictionaryPtr->tag))) {
		    callback(dictionaryPtr->tag,
			     dictionaryPtr->englishDescription,
			     dictionaryPtr->representation,
			     ctx);
		}
	    }
	}
    }
    return DCM_NORMAL;
}
