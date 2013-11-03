/*
+-+-+-+-+-+-+-+-+-
*/
/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	Steve Moore
** Intent:		This program finds a study in the CTN IDB
**			and clones it, making changes to some attributes
**			to make a new study.
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:37:26 $
** Source File:		$RCSfile: clone_study.c,v $
** Revision:		$Revision: 1.3 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.3 $ $RCSfile: clone_study.c,v $";

#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <stdlib.h>
#endif

#include "dicom.h"
#include "ctnthread.h"
#include "condition.h"
#include "tbl.h"
#include "lst.h"
#include "idb.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "manage.h"

CTNBOOLEAN silent = TRUE;

typedef struct {
    void *reserved[2];
    IDB_Query query;
}   QUERY_LIST_ITEM;


static CONDITION
selectCallback(IDB_Query * queryResponse,
	       long count, LST_HEAD * lst);
static char *callingAETitle = "CTN_INSTRUMENT";
static CTNBOOLEAN verbose = FALSE;

static void usageerror();
static void openDatabase(const char *dbname, IDB_HANDLE ** handle);
static void closeDatabase(IDB_HANDLE ** handle);
static void openControl(char *controlDatabase, DMAN_HANDLE ** dmanHandle);
static LST_HEAD *
selectImages(IDB_HANDLE ** h, const char *patientID,
	     const char *studyInstanceUID, const char *accessionNumber);

void remapItem(const IDB_Query * orig, IDB_Query * mapped);

static LST_HEAD *
cloneImages(LST_HEAD ** originalList)
{
    CONDITION cond;
    LST_HEAD *imageList = NULL;
    QUERY_LIST_ITEM *orig,
       *clone;
    IDB_InstanceListElement *e1,
       *e2;

    if (imageList == NULL) {
	imageList = LST_Create();
	if (imageList == NULL) {
	    fprintf(stderr, "Could not allocate image list in cloneImages\n");
	    exit(1);
	}
    }
    orig = LST_Head(originalList);
    if (orig != NULL)
	(void) LST_Position(originalList, orig);

    while (orig != NULL) {
	clone = malloc(sizeof(*clone));
	if (clone == NULL) {
	    fprintf(stderr, "Could not malloc an item in cloneImages\n");
	    exit(1);
	}
	*clone = *orig;

	if (orig->query.image.InstanceList != NULL) {
	    clone->query.image.InstanceList = LST_Create();
	    if (clone->query.image.InstanceList == NULL) {
		fprintf(stderr, "Could not create an instance list in cloneImages\n");
		exit(1);
	    }
	    e1 = LST_Head(&orig->query.image.InstanceList);
	    (void) LST_Position(&orig->query.image.InstanceList, e1);
	    while (e1 != NULL) {
		e2 = malloc(sizeof(*e2));
		*e2 = *e1;
		(void) LST_Enqueue(&clone->query.image.InstanceList, e2);
		e1 = LST_Next(&orig->query.image.InstanceList);
	    }
	}
	remapItem(&orig->query, &clone->query);

	(void) LST_Enqueue(&imageList, clone);
	orig = LST_Next(originalList);
    }
    return imageList;
}

static void
fillInsertion(IDB_Query * query, IDB_Insertion * pssi)
{
    IDB_InstanceListElement *e1;

    strcpy(pssi->patient.PatNam, query->patient.PatNam);
    strcpy(pssi->patient.PatID, query->patient.PatID);
    strcpy(pssi->patient.PatBirDat, query->patient.PatBirDat);
    strcpy(pssi->patient.PatBirTim, query->patient.PatBirTim);
    strcpy(pssi->patient.PatSex, query->patient.PatSex);

    strcpy(pssi->patient.Owner, query->patient.Owner);
    strcpy(pssi->patient.GroupName, query->patient.GroupName);
    strcpy(pssi->patient.Priv, query->patient.Priv);

    strcpy(pssi->study.StuDat, query->study.StuDat);
    strcpy(pssi->study.StuTim, query->study.StuTim);
    strcpy(pssi->study.AccNum, query->study.AccNum);
    strcpy(pssi->study.StuID, query->study.StuID);
    strcpy(pssi->study.StuInsUID, query->study.StuInsUID);
    strcpy(pssi->study.RefPhyNam, query->study.RefPhyNam);
    strcpy(pssi->study.StuDes, query->study.StuDes);
    strcpy(pssi->study.PatAge, query->study.PatAge);
    strcpy(pssi->study.PatSiz, query->study.PatSiz);
    strcpy(pssi->study.PatWei, query->study.PatWei);

    strcpy(pssi->study.Owner, query->study.Owner);
    strcpy(pssi->study.GroupName, query->study.GroupName);
    strcpy(pssi->study.Priv, query->study.Priv);

    strcpy(pssi->series.Mod, query->series.Mod);
    strcpy(pssi->series.SerNum, query->series.SerNum);
    strcpy(pssi->series.SerInsUID, query->series.SerInsUID);
    strcpy(pssi->series.ProNam, query->series.ProNam);
    strcpy(pssi->series.SerDes, query->series.SerDes);
    strcpy(pssi->series.BodParExa, query->series.BodParExa);
    strcpy(pssi->series.ViePos, query->series.ViePos);

    strcpy(pssi->series.Owner, query->series.Owner);
    strcpy(pssi->series.GroupName, query->series.GroupName);
    strcpy(pssi->series.Priv, query->series.Priv);

    strcpy(pssi->image.ImaNum, query->image.ImaNum);
    strcpy(pssi->image.SOPInsUID, query->image.SOPInsUID);
    strcpy(pssi->image.SOPClaUID, query->image.SOPClaUID);
    strcpy(pssi->image.PhoInt, query->image.PhoInt);
    strcpy(pssi->image.PatOri, query->image.PatOri);

    pssi->image.SamPerPix = query->image.SamPerPix;
    pssi->image.Row = query->image.Row;
    pssi->image.Col = query->image.Col;
    pssi->image.BitAll = query->image.BitAll;
    pssi->image.BitSto = query->image.BitSto;
    pssi->image.PixRep = query->image.PixRep;

    strcpy(pssi->image.Owner, query->image.Owner);
    strcpy(pssi->image.GroupName, query->image.GroupName);
    strcpy(pssi->image.Priv, query->image.Priv);

    e1 = LST_Head(&query->image.InstanceList);
    if (e1 == NULL) {
	fprintf(stderr, "No images associated with this instance\n");
	exit(1);
    }
    strcpy(pssi->image.RespondingTitle, e1->RespondingTitle);
    strcpy(pssi->image.Medium, e1->Medium);
    strcpy(pssi->image.Path, e1->Path);
    strcpy(pssi->image.Transfer, e1->Transfer);

    pssi->image.Size = e1->Size;
}

static void
insertImages(IDB_HANDLE ** handle, LST_HEAD ** imageList)
{
    CONDITION cond;
    QUERY_LIST_ITEM *q;
    IDB_InstanceListElement *e1;
    IDB_Insertion pssi;

    q = LST_Head(imageList);
    (void) LST_Position(imageList, q);
    while (q != NULL) {
	fillInsertion(&q->query, &pssi);
	cond = IDB_InsertImage(handle, &pssi);
	if (cond != IDB_NORMAL) {
	    COND_DumpConditions();
	    exit(1);
	}
	q = LST_Next(imageList);
    }
}


static void
dumpImageList(LST_HEAD ** originalList)
{
    CONDITION cond;
    QUERY_LIST_ITEM *orig;
    IDB_InstanceListElement *e1,
       *e2;

    orig = LST_Head(originalList);
    if (orig != NULL)
	(void) LST_Position(originalList, orig);

    while (orig != NULL) {
	printf("\n%-32s %s\n  %-32s\n  %-64s\n  %-64s\n  %-64s\n",
	       orig->query.patient.PatID,
	       orig->query.patient.PatNam,
	       orig->query.study.AccNum,
	       orig->query.study.StuInsUID,
	       orig->query.series.SerInsUID,
	       orig->query.image.SOPInsUID);
	e1 = LST_Head(&orig->query.image.InstanceList);
	(void) LST_Position(&orig->query.image.InstanceList, e1);
	while (e1 != NULL) {
	    printf("    %s\n", e1->Path);
	    e1 = LST_Next(&orig->query.image.InstanceList);
	}

	orig = LST_Next(originalList);
    }
}

main(int argc, char **argv)
{
    CONDITION
	cond;
    IDB_HANDLE
	* handle;
    IDB_Query
	recs;
    long
        i,
        count;
    char *dbname = "INSTRUMENT_IDB";
    char *patientID = "";
    char *studyInstanceUID = "";
    char *accessionNumber = "";
    LST_HEAD *imageList = NULL;
    LST_HEAD *cloneList = NULL;
    DMAN_HANDLE *dmanHandle = NULL;
    char *controlDatabase = "CTNControl";

    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'd':
	    argc--;
	    argv++;
	    if (argc < 1)
		usageerror();
	    dbname = *argv;
	    break;
	case 'f':
	    argc--;
	    argv++;
	    if (argc < 1)
		usageerror();
	    controlDatabase = *argv;
	    break;
	case 'h':
	    usageerror();
	    break;
	case 'p':
	    argc--;
	    argv++;
	    if (argc < 1)
		usageerror();
	    patientID = *argv;
	    break;
	case 'r':
	    argc--;
	    argv++;
	    if (argc < 1)
		usageerror();
	    accessionNumber = *argv;
	    break;
	case 'u':
	    argc--;
	    argv++;
	    if (argc < 1)
		usageerror();
	    studyInstanceUID = *argv;
	    break;
	case 'v':
	    verbose = TRUE;
	    break;
	default:
	    break;
	}
    }

    if (getenv("UIDFILE") == NULL) {
	fprintf(stderr, "clone_study needs UIDFILE environment variable set.\n");
	fprintf(stderr, "  This is part of the UID facility for making UIDs.\n");
	exit(1);
    }
    THR_Init();
    openDatabase(dbname, &handle);
    openControl(controlDatabase, &dmanHandle);
    imageList = selectImages(&handle, patientID, studyInstanceUID, accessionNumber);
    cloneList = cloneImages(&imageList);

    dumpImageList(&cloneList);

    insertImages(&handle, &cloneList);

    closeDatabase(&handle);

    THR_Shutdown();
    return 0;
}

static void
usageerror()
{
    char msg[] = "\
Usage: clone_study [-d <dbname>] [-f <control>] [-p <pat id>] [-r <req num>] [-u <study UID>]\n\
\n\
  -d  Set image database name (default INSTRUMENT_IDB)\n\
  -f  Set control database name (default CTNControl)\n\
  -p  Send study (or studies) for patient ID <pat id>\n\
  -r  Send study with accession number <req num>\n\
  -u  Send study with study instance UID <study UID>\n";

    fprintf(stderr, msg);
    exit(1);
}

static void
openDatabase(const char *dbname, IDB_HANDLE ** handle)
{
    CONDITION cond = 0;

    cond = IDB_Open(dbname, handle);
    if (cond != IDB_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}

static void
closeDatabase(IDB_HANDLE ** handle)
{
    CONDITION cond = 0;

    cond = IDB_Close(handle);
    if (cond != IDB_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}

static void
openControl(char *controlDatabase, DMAN_HANDLE ** dmanHandle)
{
    CONDITION cond = 0;

    cond = DMAN_Open(controlDatabase, "", "", dmanHandle);
    if (cond != DMAN_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}

static LST_HEAD *
selectImages(IDB_HANDLE ** h, const char *patientID,
	     const char *studyInstanceUID, const char *accessionNumber)
{
    CONDITION cond;
    IDB_HANDLE *handle;
    IDB_Query recs;
    long count;
    LST_HEAD *imageList = NULL;

    recs.PatientQFlag = recs.StudyQFlag = recs.SeriesQFlag = recs.ImageQFlag = 0;
    recs.PatientNullFlag = recs.StudyNullFlag = recs.SeriesNullFlag = recs.ImageNullFlag = 0;


    if (strlen(patientID) != 0) {
	recs.PatientQFlag |= QF_PAT_PatID;
	strcpy(recs.patient.PatID, patientID);
    }
    if (strlen(studyInstanceUID) != 0) {
	recs.StudyQFlag |= QF_STU_StuInsUID;
	strcpy(recs.study.StuInsUID, studyInstanceUID);
    }
    if (strlen(accessionNumber) != 0) {
	recs.StudyQFlag |= QF_STU_AccNum;
	strcpy(recs.study.AccNum, accessionNumber);
    }
    if (imageList == NULL) {
	imageList = LST_Create();
	if (imageList == NULL)
	    exit(1);
    }
    cond = IDB_Select(h, STUDY_ROOT, IDB_PATIENT_LEVEL,
		 IDB_IMAGE_LEVEL, &recs, &count, selectCallback, imageList);
    if (cond != IDB_NORMAL) {
	COND_DumpConditions();
    }
    printf("%d\n", LST_Count(&imageList));
    return imageList;
}


static CONDITION
selectCallback(IDB_Query * queryResponse, long count, LST_HEAD * lst)
{
    QUERY_LIST_ITEM
    * item;
    IDB_InstanceListElement
	* e1, *e2;

    item = malloc(sizeof(*item));
    if (item == NULL)
	return 0;

    item->query = *queryResponse;
    if (queryResponse->image.InstanceList != NULL) {
	item->query.image.InstanceList = LST_Create();
	if (item->query.image.InstanceList == NULL)
	    return 0;
	e1 = LST_Head(&queryResponse->image.InstanceList);
	(void) LST_Position(&queryResponse->image.InstanceList, e1);
	while (e1 != NULL) {
	    e2 = malloc(sizeof(*e2));
	    *e2 = *e1;
	    (void) LST_Enqueue(&item->query.image.InstanceList, e2);
	    e1 = LST_Next(&queryResponse->image.InstanceList);
	}
    }
    (void) LST_Enqueue(&lst, item);
    return IDB_NORMAL;
}
