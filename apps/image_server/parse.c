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
**				DICOM 94
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	Stephen M. Moore, 27-Apr-94
** Intent:
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:37:44 $
** Source File:		$RCSfile: parse.c,v $
** Revision:		$Revision: 1.17 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.17 $ $RCSfile: parse.c,v $";

#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/types.h>
#endif


#include "dicom.h"
#include "tbl.h"
#include "lst.h"
#include "dulprotocol.h"
#include "dicom_uids.h"
#include "dicom_objects.h"
#include "tbl.h"
#include "manage.h"
#include "idb.h"

#include "image_server.h"

static void changeInsertionCase(IDB_Insertion * r);
static void changeQueryCase(IDB_Query * query);
static CONDITION queryCallback(DCM_ELEMENT * e, void *ctx);

typedef enum {
    DMAN_PATIENT, DMAN_STUDY, DMAN_SERIES, DMAN_IMAGE
}
    PRV_PARSE_TYPE;

/* The following section defines a set of static structures that
** provide memory for the Build and Parse routines which are implemented
** below.  The tables define the set of required and conditional elements
** for the messages which are supported by this facility.
*/

static IDB_Insertion
    Insert;
static U16
    SamplesPerPixel,
    Rows,
    Columns,
    BitsAllocated,
    BitsStored,
    PixelRepresentation;

static long
    PatientNodeFlags,
    StudyNodeFlags,
    SeriesNodeFlags,
    ImageNodeFlags;

static DCM_FLAGGED_ELEMENT InsertElements[] = {
    {DCM_PATNAME, DCM_PN, "", 1, sizeof(Insert.patient.PatNam),
    (void *) Insert.patient.PatNam, QF_PAT_PatNam, &PatientNodeFlags},
    {DCM_PATID, DCM_LO, "", 1, sizeof(Insert.patient.PatID),
    (void *) Insert.patient.PatID, QF_PAT_PatID, &PatientNodeFlags},
    {DCM_PATBIRTHDATE, DCM_DA, "", 1, sizeof(Insert.patient.PatBirDat),
    (void *) Insert.patient.PatBirDat, QF_PAT_PatBirDat, &PatientNodeFlags},
    {DCM_PATBIRTHTIME, DCM_TM, "", 1, sizeof(Insert.patient.PatBirTim),
    (void *) Insert.patient.PatBirTim, QF_PAT_PatBirTim, &PatientNodeFlags},
    {DCM_PATSEX, DCM_CS, "", 1, sizeof(Insert.patient.PatSex),
    (void *) Insert.patient.PatSex, QF_PAT_PatSex, &PatientNodeFlags},

    {DCM_IDSTUDYDATE, DCM_DA, "", 1, sizeof(Insert.study.StuDat),
    (void *) Insert.study.StuDat, QF_STU_StuDat, &StudyNodeFlags},
    {DCM_IDSTUDYTIME, DCM_TM, "", 1, sizeof(Insert.study.StuTim),
    (void *) Insert.study.StuTim, QF_STU_StuTim, &StudyNodeFlags},
    {DCM_IDACCESSIONNUMBER, DCM_SH, "", 1, sizeof(Insert.study.AccNum),
    (void *) Insert.study.AccNum, QF_STU_AccNum, &StudyNodeFlags},
    {DCM_RELSTUDYID, DCM_SH, "", 1, sizeof(Insert.study.StuID),
    (void *) Insert.study.StuID, QF_STU_StuID, &StudyNodeFlags},
    {DCM_RELSTUDYINSTANCEUID, DCM_UI, "", 1, sizeof(Insert.study.StuInsUID),
    (void *) Insert.study.StuInsUID, QF_STU_StuInsUID, &StudyNodeFlags},
    {DCM_IDREFERRINGPHYSICIAN, DCM_PN, "", 1, sizeof(Insert.study.RefPhyNam),
    (void *) Insert.study.RefPhyNam, QF_STU_RefPhyNam, &StudyNodeFlags},
    {DCM_IDSTUDYDESCRIPTION, DCM_LO, "", 1, sizeof(Insert.study.StuDes),
    (void *) Insert.study.StuDes, QF_STU_StuDes, &StudyNodeFlags},
    {DCM_PATAGE, DCM_AS, "", 1, sizeof(Insert.study.PatAge),
    (void *) Insert.study.PatAge, QF_STU_PatAge, &StudyNodeFlags},
    {DCM_PATSIZE, DCM_DS, "", 1, sizeof(Insert.study.PatSiz),
    (void *) Insert.study.PatSiz, QF_STU_PatSiz, &StudyNodeFlags},
    {DCM_PATWEIGHT, DCM_DS, "", 1, sizeof(Insert.study.PatWei),
    (void *) Insert.study.PatWei, QF_STU_PatWei, &StudyNodeFlags},

    {DCM_IDMODALITY, DCM_CS, "", 1, sizeof(Insert.series.Mod),
    (void *) Insert.series.Mod, QF_SER_Mod, &SeriesNodeFlags},
    {DCM_RELSERIESNUMBER, DCM_IS, "", 1, sizeof(Insert.series.SerNum),
    (void *) Insert.series.SerNum, QF_SER_SerNum, &SeriesNodeFlags},
    {DCM_RELSERIESINSTANCEUID, DCM_UI, "", 1, sizeof(Insert.series.SerInsUID),
    (void *) Insert.series.SerInsUID, QF_SER_SerInsUID, &SeriesNodeFlags},
    {DCM_ACQBODYPARTEXAMINED, DCM_CS, "", 1, sizeof(Insert.series.BodParExa),
    (void *) Insert.series.BodParExa, QF_SER_BodParExa, &SeriesNodeFlags},
    {DCM_ACQVIEWPOSITION, DCM_CS, "", 1, sizeof(Insert.series.ViePos),
    (void *) Insert.series.ViePos, QF_SER_ViePos, &SeriesNodeFlags},
    {DCM_IDSERIESDESCR, DCM_LO, "", 1, sizeof(Insert.series.SerDes),
    (void *) Insert.series.SerDes, QF_SER_SerDes, &SeriesNodeFlags},

    {DCM_RELIMAGENUMBER, DCM_IS, "", 1, sizeof(Insert.image.ImaNum),
    (void *) Insert.image.ImaNum, QF_IMA_ImaNum, &ImageNodeFlags},
    {DCM_IDSOPCLASSUID, DCM_UI, "", 1, sizeof(Insert.image.SOPClaUID),
    (void *) Insert.image.SOPClaUID, QF_IMA_SOPClaUID, &ImageNodeFlags},
    {DCM_IDSOPINSTANCEUID, DCM_UI, "", 1, sizeof(Insert.image.SOPInsUID),
    (void *) Insert.image.SOPInsUID, QF_IMA_SOPInsUID, &ImageNodeFlags},
    {DCM_IMGSAMPLESPERPIXEL, DCM_US, "", 1, sizeof(SamplesPerPixel),
    (void *) &SamplesPerPixel, QF_IMA_SamPerPix, &ImageNodeFlags},
    {DCM_IMGROWS, DCM_US, "", 1, sizeof(Rows),
    (void *) &Rows, QF_IMA_Row, &ImageNodeFlags},
    {DCM_IMGCOLUMNS, DCM_US, "", 1, sizeof(Columns),
    (void *) &Columns, QF_IMA_Col, &ImageNodeFlags},
    {DCM_IMGBITSALLOCATED, DCM_US, "", 1, sizeof(BitsAllocated),
    (void *) &BitsAllocated, QF_IMA_BitAll, &ImageNodeFlags},
    {DCM_IMGBITSSTORED, DCM_US, "", 1, sizeof(BitsStored),
    (void *) &BitsStored, QF_IMA_BitSto, &ImageNodeFlags},
    {DCM_IMGPIXELREPRESENTATION, DCM_US, "", 1, sizeof(PixelRepresentation),
    (void *) &PixelRepresentation, QF_IMA_PixRep, &ImageNodeFlags},
    {DCM_RELPATIENTORIENTATION, DCM_CS, "", 1, sizeof(Insert.image.PatOri),
    (void *) Insert.image.PatOri, QF_IMA_PatOri, &ImageNodeFlags}
};

#define	REQUIRED_PATIENT_FIELDS (QF_PAT_PatNam | QF_PAT_PatID | \
QF_PAT_PatSex | QF_PAT_PatBirDat)

#define REQUIRED_STUDY_FIELDS  (QF_STU_StuDat | QF_STU_StuTim | \
QF_STU_AccNum | QF_STU_StuID | QF_STU_StuInsUID | QF_STU_RefPhyNam)

#define	REQUIRED_SERIES_FIELDS (QF_SER_Mod | QF_SER_SerNum | \
QF_SER_SerInsUID)

#define	REQUIRED_IMAGE_FIELDS	(QF_IMA_ImaNum | QF_IMA_SOPInsUID | \
QF_IMA_SamPerPix | QF_IMA_Row | QF_IMA_Col | QF_IMA_BitAll | QF_IMA_BitSto | \
QF_IMA_PixRep)

/* parseImageForInsert
**
** Purpose:
**
** Parameter Dictionary:
**	object
**
** Return Values:
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
parseImageForInsert(DCM_OBJECT ** object, IDB_Insertion * insertStructure)
{
    CONDITION
	cond;

    memset(&Insert, 0, sizeof(Insert));

    cond = DCM_ParseObject(object, NULL, 0,
			InsertElements, (int) DIM_OF(InsertElements), NULL);
    if (cond != DCM_NORMAL)
	return 0;

    *insertStructure = Insert;
    insertStructure->image.SamPerPix = SamplesPerPixel;
    insertStructure->image.Row = Rows;
    insertStructure->image.Col = Columns;
    insertStructure->image.BitAll = BitsAllocated;
    insertStructure->image.BitSto = BitsStored;
    insertStructure->image.PixRep = PixelRepresentation;

    if ((PatientNodeFlags & REQUIRED_PATIENT_FIELDS) != REQUIRED_PATIENT_FIELDS) {
	(void) COND_PushCondition(APP_ERROR(APP_GENERALMSG),
			"One or more attributes missing in patient fields");
	return COND_PushCondition(APP_ERROR(APP_IMAGEPARSEFAILED));
    }
    if ((StudyNodeFlags & REQUIRED_STUDY_FIELDS) != REQUIRED_STUDY_FIELDS) {
	(void) COND_PushCondition(APP_ERROR(APP_GENERALMSG),
			  "One or more attributes missing in study fields");
	return COND_PushCondition(APP_ERROR(APP_IMAGEPARSEFAILED));
    }
    if ((SeriesNodeFlags & REQUIRED_SERIES_FIELDS) != REQUIRED_SERIES_FIELDS) {
	(void) COND_PushCondition(APP_ERROR(APP_GENERALMSG),
			 "One or more attributes missing in series fields");
	return COND_PushCondition(APP_ERROR(APP_IMAGEPARSEFAILED));
    }
    if ((ImageNodeFlags & REQUIRED_IMAGE_FIELDS) != REQUIRED_IMAGE_FIELDS) {
	(void) COND_PushCondition(APP_ERROR(APP_GENERALMSG),
			  "One or more attributes missing in image fields");
	return COND_PushCondition(APP_ERROR(APP_IMAGEPARSEFAILED));
    }
    if (strlen(insertStructure->patient.PatNam) == 0) {
	(void) COND_PushCondition(APP_ERROR(APP_GENERALMSG),
				  "Zero length patient name rejected");
	return COND_PushCondition(APP_ERROR(APP_IMAGEPARSEFAILED));
    }
    if (strlen(insertStructure->patient.PatID) == 0) {
	(void) COND_PushCondition(APP_ERROR(APP_GENERALMSG),
				  "Zero length patient ID rejected");
	return COND_PushCondition(APP_ERROR(APP_IMAGEPARSEFAILED));
    }
    if (strlen(insertStructure->study.StuInsUID) == 0) {
	(void) COND_PushCondition(APP_ERROR(APP_GENERALMSG),
				  "Zero length Study Instance UID rejected");
	return COND_PushCondition(APP_ERROR(APP_IMAGEPARSEFAILED));
    }
    if (strlen(insertStructure->series.SerInsUID) == 0) {
	(void) COND_PushCondition(APP_ERROR(APP_GENERALMSG),
				"Zero length Series Instance UID rejected");
	return COND_PushCondition(APP_ERROR(APP_IMAGEPARSEFAILED));
    }
    if (strlen(insertStructure->image.SOPInsUID) == 0) {
	(void) COND_PushCondition(APP_ERROR(APP_GENERALMSG),
				  "Zero length SOP Instance UID rejected");
	return COND_PushCondition(APP_ERROR(APP_IMAGEPARSEFAILED));
    }
    changeInsertionCase(insertStructure);

    return APP_NORMAL;
}

void
extractTransferSyntax(DCM_OBJECT ** obj, char *transferSyntax)
{
    static DCM_ELEMENT e = {DCM_METATRANSFERSYNTAX, DCM_UI, "", 1, 0, NULL};

    e.length = DICOM_UI_LENGTH + 1;
    e.d.string = transferSyntax;

    DCM_ParseObject(obj, &e, 1, NULL, 0, NULL);
}


static void
changeInsertionCase(IDB_Insertion * r)
{
    int
        i;

    for (i = 0; r->patient.PatNam[i] != '\0'; i++) {
	if (islower(r->patient.PatNam[i]))
	    r->patient.PatNam[i] = toupper(r->patient.PatNam[i]);
    }
    for (i = 0; r->patient.PatID[i] != '\0'; i++) {
	if (islower(r->patient.PatID[i]))
	    r->patient.PatID[i] = toupper(r->patient.PatID[i]);
    }
    for (i = 0; r->study.AccNum[i] != '\0'; i++) {
	if (islower(r->study.AccNum[i]))
	    r->study.AccNum[i] = toupper(r->study.AccNum[i]);
    }
    for (i = 0; r->study.StuID[i] != '\0'; i++) {
	if (islower(r->study.StuID[i]))
	    r->study.StuID[i] = toupper(r->study.StuID[i]);
    }
}



static IDB_Query Query;

static DCM_FLAGGED_ELEMENT queryMap[] = {
    {DCM_PATNAME, DCM_PN, "", 1, sizeof(Query.patient.PatNam),
    (void *) Query.patient.PatNam, QF_PAT_PatNam, &Query.PatientQFlag},
    {DCM_PATID, DCM_LO, "", 1, sizeof(Query.patient.PatID),
    (void *) Query.patient.PatID, QF_PAT_PatID, &Query.PatientQFlag},
    {DCM_PATBIRTHDATE, DCM_DA, "", 1, sizeof(Query.patient.PatBirDat),
    (void *) Query.patient.PatBirDat, QF_PAT_PatBirDat, &Query.PatientQFlag},
    {DCM_PATBIRTHTIME, DCM_TM, "", 1, sizeof(Query.patient.PatBirTim),
    (void *) Query.patient.PatBirTim, QF_PAT_PatBirTim, &Query.PatientQFlag},
    {DCM_PATSEX, DCM_CS, "", 1, sizeof(Query.patient.PatSex),
    (void *) Query.patient.PatSex, QF_PAT_PatSex, &Query.PatientQFlag},

    {DCM_IDSTUDYDATE, DCM_DA, "", 1, sizeof(Query.study.StuDat),
    (void *) Query.study.StuDat, QF_STU_StuDat, &Query.StudyQFlag},
    {DCM_IDSTUDYTIME, DCM_TM, "", 1, sizeof(Query.study.StuTim),
    (void *) Query.study.StuTim, QF_STU_StuTim, &Query.StudyQFlag},
    {DCM_IDACCESSIONNUMBER, DCM_SH, "", 1, sizeof(Query.study.AccNum),
    (void *) Query.study.AccNum, QF_STU_AccNum, &Query.StudyQFlag},
    {DCM_RELSTUDYID, DCM_SH, "", 1, sizeof(Query.study.StuID),
    (void *) Query.study.StuID, QF_STU_StuID, &Query.StudyQFlag},
    {DCM_RELSTUDYINSTANCEUID, DCM_UI, "", 1, sizeof(Query.study.StuInsUID),
    (void *) Query.study.StuInsUID, QF_STU_StuInsUID, &Query.StudyQFlag},
    {DCM_IDREFERRINGPHYSICIAN, DCM_PN, "", 1, sizeof(Query.study.RefPhyNam),
    (void *) Query.study.RefPhyNam, QF_STU_RefPhyNam, &Query.StudyQFlag},
    {DCM_IDSTUDYDESCRIPTION, DCM_LO, "", 1, sizeof(Query.study.StuDes),
    (void *) Query.study.StuDes, QF_STU_StuDes, &Query.StudyQFlag},
    {DCM_PATAGE, DCM_AS, "", 1, sizeof(Query.study.PatAge),
    (void *) Query.study.PatAge, QF_STU_PatAge, &Query.StudyQFlag},
    {DCM_PATSIZE, DCM_DS, "", 1, sizeof(Query.study.PatSiz),
    (void *) Query.study.PatSiz, QF_STU_PatSiz, &Query.StudyQFlag},
    {DCM_PATWEIGHT, DCM_DS, "", 1, sizeof(Query.study.PatWei),
    (void *) Query.study.PatWei, QF_STU_PatWei, &Query.StudyQFlag},

    {DCM_IDMODALITY, DCM_CS, "", 1, sizeof(Query.series.Mod),
    (void *) Query.series.Mod, QF_SER_Mod, &Query.SeriesQFlag},
    {DCM_RELSERIESNUMBER, DCM_IS, "", 1, sizeof(Query.series.SerNum),
    (void *) Query.series.SerNum, QF_SER_SerNum, &Query.SeriesQFlag},
    {DCM_RELSERIESINSTANCEUID, DCM_UI, "", 1, sizeof(Query.series.SerInsUID),
    (void *) Query.series.SerInsUID, QF_SER_SerInsUID, &Query.SeriesQFlag},
    {DCM_ACQPROTOCOLNAME, DCM_LO, "", 1, sizeof(Query.series.ProNam),
    (void *) Query.series.ProNam, QF_SER_ProNam, &Query.SeriesQFlag},
    {DCM_ACQBODYPARTEXAMINED, DCM_CS, "", 1, sizeof(Query.series.BodParExa),
    (void *) Query.series.BodParExa, QF_SER_BodParExa, &Query.SeriesQFlag},

    {DCM_RELIMAGENUMBER, DCM_IS, "", 1, sizeof(Query.image.ImaNum),
    (void *) Query.image.ImaNum, QF_IMA_ImaNum, &Query.ImageQFlag},
    {DCM_IDSOPINSTANCEUID, DCM_UI, "", 1, sizeof(Query.image.SOPInsUID),
    (void *) Query.image.SOPInsUID, QF_IMA_SOPInsUID, &Query.ImageQFlag}
};

typedef struct {
    char *string;
    long flag;
    long *queryFlag;
    long *nullFlag;
}   NULL_MAP;

static NULL_MAP nullMap[] = {
    {Query.patient.PatNam, QF_PAT_PatNam, &Query.PatientQFlag,
    &Query.PatientNullFlag},
    {Query.patient.PatID, QF_PAT_PatID, &Query.PatientQFlag,
    &Query.PatientNullFlag},
    {Query.patient.PatBirDat, QF_PAT_PatBirDat, &Query.PatientQFlag,
    &Query.PatientNullFlag},
    {Query.patient.PatBirTim, QF_PAT_PatBirTim, &Query.PatientQFlag,
    &Query.PatientNullFlag},
    {Query.patient.PatSex, QF_PAT_PatSex, &Query.PatientQFlag,
    &Query.PatientNullFlag},

    {Query.study.StuDat, QF_STU_StuDat, &Query.StudyQFlag,
    &Query.StudyNullFlag},
    {Query.study.StuTim, QF_STU_StuTim, &Query.StudyQFlag,
    &Query.StudyNullFlag},
    {Query.study.AccNum, QF_STU_AccNum, &Query.StudyQFlag,
    &Query.StudyNullFlag},
    {Query.study.StuID, QF_STU_StuID, &Query.StudyQFlag,
    &Query.StudyNullFlag},
    {Query.study.StuInsUID, QF_STU_StuInsUID, &Query.StudyQFlag,
    &Query.StudyNullFlag},
    {Query.study.RefPhyNam, QF_STU_RefPhyNam, &Query.StudyQFlag,
    &Query.StudyNullFlag},
    {Query.study.StuDes, QF_STU_StuDes, &Query.StudyQFlag,
    &Query.StudyNullFlag},
    {Query.study.PatAge, QF_STU_PatAge, &Query.StudyQFlag,
    &Query.StudyNullFlag},
    {Query.study.PatSiz, QF_STU_PatSiz, &Query.StudyQFlag,
    &Query.StudyNullFlag},
    {Query.study.PatWei, QF_STU_PatWei, &Query.StudyQFlag,
    &Query.StudyNullFlag},

    {Query.series.Mod, QF_SER_Mod, &Query.SeriesQFlag,
    &Query.SeriesNullFlag},
    {Query.series.SerNum, QF_SER_SerNum, &Query.SeriesQFlag,
    &Query.SeriesNullFlag},
    {Query.series.SerInsUID, QF_SER_SerInsUID, &Query.SeriesQFlag,
    &Query.SeriesNullFlag},
    {Query.series.ProNam, QF_SER_ProNam, &Query.SeriesQFlag,
    &Query.SeriesNullFlag},
    {Query.series.BodParExa, QF_SER_BodParExa, &Query.SeriesQFlag,
    &Query.SeriesNullFlag},

    {Query.image.ImaNum, QF_IMA_ImaNum, &Query.ImageQFlag,
    &Query.ImageNullFlag},
    {Query.image.SOPInsUID, QF_IMA_SOPInsUID, &Query.ImageQFlag,
    &Query.ImageNullFlag}
};

CONDITION
parseQuery(DCM_OBJECT ** object, IDB_Query * queryStructure)
{
    CONDITION
	cond;
    char
        buf[1024];
    int
        i;

    memset(&Query, 0, sizeof(Query));

    cond = DCM_ScanParseObject(object, buf, sizeof(buf),
		     queryMap, (int) DIM_OF(queryMap), queryCallback, NULL);
    if (cond != DCM_NORMAL)
	return 0;		/* repair */

    Query.PatientNullFlag = 0;
    Query.StudyNullFlag = 0;
    Query.SeriesNullFlag = 0;
    Query.ImageNullFlag = 0;

    for (i = 0; i < (int) DIM_OF(nullMap); i++) {
	if (*nullMap[i].queryFlag & nullMap[i].flag) {
	    if (nullMap[i].string[0] == '\0')
		*nullMap[i].nullFlag |= nullMap[i].flag;
	}
    }

    *queryStructure = Query;
    changeQueryCase(queryStructure);
    return APP_NORMAL;
}

static CONDITION
queryCallback(DCM_ELEMENT * e, void *ctx)
{
    printf("%s\n", e->description);
    return DCM_NORMAL;
}

static void
changeQueryCase(IDB_Query * query)
{
    int
        i;

    if (query->PatientQFlag & QF_PAT_PatNam) {
	for (i = 0; query->patient.PatNam[i] != '\0'; i++) {
	    if (islower(query->patient.PatNam[i]))
		query->patient.PatNam[i] = toupper(query->patient.PatNam[i]);
	}
    }
    if (query->PatientQFlag & QF_PAT_PatID) {
	for (i = 0; query->patient.PatID[i] != '\0'; i++) {
	    if (islower(query->patient.PatID[i]))
		query->patient.PatID[i] = toupper(query->patient.PatID[i]);
	}
    }
    if (query->StudyQFlag & QF_STU_AccNum) {
	for (i = 0; query->study.AccNum[i] != '\0'; i++) {
	    if (islower(query->study.AccNum[i]))
		query->study.AccNum[i] = toupper(query->study.AccNum[i]);
	}
    }
    if (query->StudyQFlag & QF_STU_StuID) {
	for (i = 0; query->study.StuID[i] != '\0'; i++) {
	    if (islower(query->study.StuID[i]))
		query->study.StuID[i] = toupper(query->study.StuID[i]);
	}
    }
}

CONDITION
buildQuery(DCM_OBJECT ** object, IDB_Query * queryStructure)
{
    CONDITION
	cond;
    int i;

    Query = *queryStructure;
    for (i = 0; i < (int) DIM_OF(nullMap); i++) {
	if (*nullMap[i].nullFlag & nullMap[i].flag)
	    nullMap[i].string[0] = '\0';
    }

    cond = DCM_ModifyElements(object, NULL, 0,
			      queryMap, (int) DIM_OF(queryMap), NULL);
    if (cond != DCM_NORMAL)
	return 0;
    return APP_NORMAL;
}

static CONDITION
    verifyPatientRootQuery(IDB_Query * queryStructure, int queryLevel);
static CONDITION
    verifyStudyRootQuery(IDB_Query * queryStructure, int queryLevel);
static CONDITION
    verifyPatientStudyQuery(IDB_Query * queryStructure, int queryLevel);

CONDITION
verifyQuery(IDB_Query * queryStructure, char *SOPClass, int queryLevel)
{
    CONDITION cond = APP_NORMAL;

    if (strcmp(SOPClass, DICOM_SOPPATIENTQUERY_FIND) == 0)
	cond = verifyPatientRootQuery(queryStructure, queryLevel);
    else if (strcmp(SOPClass, DICOM_SOPSTUDYQUERY_FIND) == 0)
	cond = verifyStudyRootQuery(queryStructure, queryLevel);
    else if (strcmp(SOPClass, DICOM_SOPPATIENTSTUDYQUERY_FIND) == 0)
	cond = verifyPatientStudyQuery(queryStructure, queryLevel);
    else
	cond = 0;

    return cond;
}

static CONDITION
verifyPatientRootQuery(IDB_Query * queryStructure, int queryLevel)
{
    CONDITION cond = APP_NORMAL;

    switch (queryLevel) {
    case IDB_PATIENT_LEVEL:
	if (queryStructure->StudyQFlag != 0)
	    cond = 0;
	if (queryStructure->SeriesQFlag != 0)
	    cond = 0;
	if (queryStructure->ImageQFlag != 0)
	    cond = 0;
	break;
    case IDB_STUDY_LEVEL:
	if (queryStructure->PatientQFlag != QF_PAT_PatID)
	    cond = 0;
	if (queryStructure->SeriesQFlag != 0)
	    cond = 0;
	if (queryStructure->ImageQFlag != 0)
	    cond = 0;
	break;
    case IDB_SERIES_LEVEL:
	if (queryStructure->PatientQFlag != QF_PAT_PatID)
	    cond = 0;
	if (queryStructure->StudyQFlag != QF_STU_StuInsUID)
	    cond = 0;
	if (queryStructure->ImageQFlag != 0)
	    cond = 0;
	break;
    case IDB_IMAGE_LEVEL:
	if (queryStructure->PatientQFlag != QF_PAT_PatID)
	    cond = 0;
	if (queryStructure->StudyQFlag != QF_STU_StuInsUID)
	    cond = 0;
	if (queryStructure->SeriesQFlag != QF_SER_SerInsUID)
	    cond = 0;
	break;
    default:
	cond = 0;
	break;
    }

    return cond;
}

static CONDITION
verifyStudyRootQuery(IDB_Query * queryStructure, int queryLevel)
{
    CONDITION cond = APP_NORMAL;

    switch (queryLevel) {
    case IDB_PATIENT_LEVEL:
	cond = 0;
	break;
    case IDB_STUDY_LEVEL:
	if (queryStructure->SeriesQFlag != 0)
	    cond = 0;
	if (queryStructure->ImageQFlag != 0)
	    cond = 0;
	break;
    case IDB_SERIES_LEVEL:
	if (queryStructure->StudyQFlag != QF_STU_StuInsUID)
	    cond = 0;
	if (queryStructure->ImageQFlag != 0)
	    cond = 0;
	break;
    case IDB_IMAGE_LEVEL:
	if (queryStructure->StudyQFlag != QF_STU_StuInsUID)
	    cond = 0;
	if (queryStructure->SeriesQFlag != QF_SER_SerInsUID)
	    cond = 0;
	break;
    default:
	cond = 0;
	break;
    }

    return cond;
}

static CONDITION
verifyPatientStudyQuery(IDB_Query * queryStructure, int queryLevel)
{
    CONDITION cond = APP_NORMAL;

    switch (queryLevel) {
    case IDB_PATIENT_LEVEL:
	if (queryStructure->StudyQFlag != 0)
	    cond = 0;
	if (queryStructure->SeriesQFlag != 0)
	    cond = 0;
	if (queryStructure->ImageQFlag != 0)
	    cond = 0;
	break;
    case IDB_STUDY_LEVEL:
	if (queryStructure->PatientQFlag != QF_PAT_PatID)
	    cond = 0;
	if (queryStructure->SeriesQFlag != 0)
	    cond = 0;
	if (queryStructure->ImageQFlag != 0)
	    cond = 0;
	break;
    case IDB_SERIES_LEVEL:
	cond = 0;
	break;
    case IDB_IMAGE_LEVEL:
	cond = 0;
	break;
    default:
	cond = 0;
	break;
    }

    return cond;
}
