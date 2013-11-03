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
** Intent:		This file contains functions for remapping the
**			attributes in a set of images to make a new study.
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:37:27 $
** Source File:		$RCSfile: remap.c,v $
** Revision:		$Revision: 1.2 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.2 $ $RCSfile: remap.c,v $";

#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <stdlib.h>
#endif

#include "dicom.h"
#include "condition.h"
#include "tbl.h"
#include "lst.h"
#include "idb.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "manage.h"
#include "dicom_uids.h"


typedef struct {
    void *reserved[2];
    char *orig;
    char *mapped;
}   MAP;

static MAP *
newMap(const char *orig, const char *mapped)
{
    MAP *map;

    map = malloc(sizeof(*map));
    if (map == NULL)
	return map;

    map->orig = malloc(strlen(orig) + 1);
    map->mapped = malloc(strlen(mapped) + 1);
    if (map->orig == NULL || map->mapped == NULL)
	return NULL;

    strcpy(map->orig, orig);
    strcpy(map->mapped, mapped);
    return map;
}

static LST_HEAD *
nameMap()
{
    static LST_HEAD *m = NULL;

    if (m == NULL)
	m = LST_Create();

    return m;
}

static LST_HEAD *
patientIDMap()
{
    static LST_HEAD *m = NULL;

    if (m == NULL)
	m = LST_Create();

    return m;
}

static LST_HEAD *
accessionNumberMap()
{
    static LST_HEAD *m = NULL;

    if (m == NULL)
	m = LST_Create();

    return m;
}

static LST_HEAD *
studyUIDMap()
{
    static LST_HEAD *m = NULL;

    if (m == NULL)
	m = LST_Create();

    return m;
}

static LST_HEAD *
seriesUIDMap()
{
    static LST_HEAD *m = NULL;

    if (m == NULL)
	m = LST_Create();

    return m;
}

static LST_HEAD *
imageUIDMap()
{
    static LST_HEAD *m = NULL;

    if (m == NULL)
	m = LST_Create();

    return m;
}

const char *
newPatientName()
{
    static char name[128];
    unsigned long v;
    CONDITION cond;

    cond = UID_NewNumber(UID_PATIENT, &v);
    if (cond != UID_NORMAL) {
	fprintf(stderr, "Unable to create a new patient number to generate a name\n");
	exit(1);
    }
    sprintf(name, "LAST%05x^FIRST", v);
    return name;
}

const char *
newPatientID()
{
    static char string[128];
    unsigned long v;
    CONDITION cond;

    cond = UID_NewNumber(UID_PATIENT, &v);
    if (cond != UID_NORMAL) {
	fprintf(stderr, "Unable to create a new patient number to generate an ID\n");
	exit(1);
    }
    sprintf(string, "PID%08d", v);
    return string;
}

const char *
newAccessionNumber()
{
    static char string[128];
    unsigned long v;
    CONDITION cond;

    cond = UID_NewNumber(UID_STUDY, &v);
    if (cond != UID_NORMAL) {
	fprintf(stderr,
		"Unable to create a new study number to generate an Accession Number\n");
	exit(1);
    }
    sprintf(string, "ACC%08d", v);
    return string;
}

const char *
newStudyUID()
{
    static char string[128];
    CONDITION cond;

    cond = UID_NewUID(UID_STUDY, string);
    if (cond != UID_NORMAL) {
	fprintf(stderr,
		"Unable to create a new study UID\n");
	exit(1);
    }
    return string;
}

const char *
newSeriesUID()
{
    static char string[128];
    CONDITION cond;

    cond = UID_NewUID(UID_SERIES, string);
    if (cond != UID_NORMAL) {
	fprintf(stderr,
		"Unable to create a new series UID\n");
	exit(1);
    }
    return string;
}

const char *
newImageUID()
{
    static char string[128];
    CONDITION cond;

    cond = UID_NewUID(UID_IMAGE, string);
    if (cond != UID_NORMAL) {
	fprintf(stderr,
		"Unable to create a new image UID\n");
	exit(1);
    }
    return string;
}

static void
remapName(const IDB_Query * orig, IDB_Query * mapped)
{
    MAP *map;
    LST_HEAD *l = nameMap();

    map = LST_Head(&l);
    (void) LST_Position(&l, map);

    while (map != NULL) {
	if (strcmp(map->orig, orig->patient.PatNam) == 0) {
	    strcpy(mapped->patient.PatNam, map->mapped);
	    return;
	}
	map = LST_Next(&l);
    }

    map = newMap(orig->patient.PatNam, newPatientName());
    (void) LST_Enqueue(&l, map);
    strcpy(mapped->patient.PatNam, map->mapped);
}

static void
remapPatientID(const IDB_Query * orig, IDB_Query * mapped)
{
    MAP *map;
    LST_HEAD *l = patientIDMap();

    map = LST_Head(&l);
    (void) LST_Position(&l, map);

    while (map != NULL) {
	if (strcmp(map->orig, orig->patient.PatID) == 0) {
	    strcpy(mapped->patient.PatID, map->mapped);
	    return;
	}
	map = LST_Next(&l);
    }

    map = newMap(orig->patient.PatID, newPatientID());
    (void) LST_Enqueue(&l, map);
    strcpy(mapped->patient.PatID, map->mapped);
}

static void
remapAccessionNumber(const IDB_Query * orig, IDB_Query * mapped)
{
    MAP *map;
    LST_HEAD *l = accessionNumberMap();

    map = LST_Head(&l);
    (void) LST_Position(&l, map);

    while (map != NULL) {
	if (strcmp(map->orig, orig->study.AccNum) == 0) {
	    strcpy(mapped->study.AccNum, map->mapped);
	    return;
	}
	map = LST_Next(&l);
    }

    map = newMap(orig->study.AccNum, newAccessionNumber());
    (void) LST_Enqueue(&l, map);
    strcpy(mapped->study.AccNum, map->mapped);
}

static void
remapStudyInstanceUID(const IDB_Query * orig, IDB_Query * mapped)
{
    MAP *map;
    LST_HEAD *l = studyUIDMap();

    map = LST_Head(&l);
    (void) LST_Position(&l, map);

    while (map != NULL) {
	if (strcmp(map->orig, orig->study.StuInsUID) == 0) {
	    strcpy(mapped->study.StuInsUID, map->mapped);
	    return;
	}
	map = LST_Next(&l);
    }

    map = newMap(orig->study.StuInsUID, newStudyUID());
    (void) LST_Enqueue(&l, map);
    strcpy(mapped->study.StuInsUID, map->mapped);
}

static void
remapSeriesInstanceUID(const IDB_Query * orig, IDB_Query * mapped)
{
    MAP *map;
    LST_HEAD *l = seriesUIDMap();

    map = LST_Head(&l);
    (void) LST_Position(&l, map);

    while (map != NULL) {
	if (strcmp(map->orig, orig->series.SerInsUID) == 0) {
	    strcpy(mapped->series.SerInsUID, map->mapped);
	    return;
	}
	map = LST_Next(&l);
    }

    map = newMap(orig->series.SerInsUID, newSeriesUID());
    (void) LST_Enqueue(&l, map);
    strcpy(mapped->series.SerInsUID, map->mapped);
}
static void
remapSOPInstanceUID(const IDB_Query * orig, IDB_Query * mapped)
{
    MAP *map;
    LST_HEAD *l = imageUIDMap();

    map = LST_Head(&l);
    (void) LST_Position(&l, map);

    while (map != NULL) {
	if (strcmp(map->orig, orig->image.SOPInsUID) == 0) {
	    strcpy(mapped->image.SOPInsUID, map->mapped);
	    return;
	}
	map = LST_Next(&l);
    }

    map = newMap(orig->image.SOPInsUID, newImageUID());
    (void) LST_Enqueue(&l, map);
    strcpy(mapped->image.SOPInsUID, map->mapped);
}



void
remapItem(const IDB_Query * orig, IDB_Query * mapped)
{
    remapName(orig, mapped);
    remapPatientID(orig, mapped);
    remapAccessionNumber(orig, mapped);
    remapStudyInstanceUID(orig, mapped);
    remapSeriesInstanceUID(orig, mapped);
    remapSOPInstanceUID(orig, mapped);
}
