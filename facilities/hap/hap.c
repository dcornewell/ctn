/*
          Copyright (C) 1993, RSNA and Washington University

          The software and supporting documentation for the Radiological
          Society of North America (RSNA) 1993 Digital Imaging and
          Communications in Medicine (DICOM) Demonstration were developed
          at the
                  Electronic Radiology Laboratory
                  Mallinckrodt Institute of Radiology
                  Washington University School of Medicine
                  510 S. Kingshighway Blvd.
                  St. Louis, MO 63110
          as part of the 1993 DICOM Central Test Node project for, and
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
**			HAP_SendImage
** Author, Date:	Stephen M. Moore, John T. O'Neill, 16-Jun-93
** Intent:
** Last Update:		$Author: smm $, $Date: 1994-01-28 15:16:39 $
** Source File:		$RCSfile: hap.c,v $
** Revision:		$Revision: 1.4 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.4 $ $RCSfile: hap.c,v $";

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "dicom.h"
#include "dicom_uids.h"
#include "lst.h"
#include "condition.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "dbquery.h"
#include "hap.h"

typedef struct {
    char *queryLevel;
    long flag;
}   QUERY_MAP;

static QUERY_MAP levelMap[] = {
    {DCM_QUERYLEVELPATIENT, DB_K_LEVELPAT},
    {DCM_QUERYLEVELSTUDY, DB_K_LEVELSTUDY},
    {DCM_QUERYLEVELSERIES, DB_K_LEVELSERIES},
    {DCM_QUERYLEVELIMAGE, DB_K_LEVELIMAGE}
};

typedef struct {
    char *SOPClass;
    long flag;
}   CLASS_MAP;

static CLASS_MAP classMap[] = {
    {DICOM_SOPPATIENTQUERY_FIND, DB_K_CLASSPAT},
    {DICOM_SOPSTUDYQUERY_FIND, DB_K_CLASSSTUDY},
    {DICOM_SOPPATIENTSTUDYQUERY_FIND, DB_K_CLASSPATSTUDY}
};

typedef struct {
    DCM_ELEMENT e;
    long *flagAddress;
    long flag;
}   ELEMENT_MAP;

static Query
    q;
static ELEMENT_MAP list[] = {
    {DCM_PATBIRTHDATE, DCM_DA, "", 1,
	sizeof(q.Patient.BirthDate), (void *) q.Patient.BirthDate,
    &q.Patient.Query_Flag, DB_K_QBIRTHDATE},
    {DCM_PATNAME, DCM_LO, "", 1,
	sizeof(q.Patient.Name), (void *) q.Patient.Name,
    &q.Patient.Query_Flag, DB_K_QNAME},
    {DCM_PATID, DCM_LO, "", 1,
	sizeof(q.Patient.PatID), (void *) q.Patient.PatID,
    &q.Patient.Query_Flag, DB_K_QID},

    {DCM_IDSTUDYDATE, DCM_DA, "", 1,
	sizeof(q.Study.StudyDate), (void *) q.Study.StudyDate,
    &q.Study.Query_Flag, DB_K_QSTUDYDATE},
    {DCM_IDSTUDYTIME, DCM_TM, "", 1,
	sizeof(q.Study.StudyTime), (void *) q.Study.StudyTime,
    &q.Study.Query_Flag, DB_K_QSTUDYTIME},
    {DCM_RELSTUDYID, DCM_CS, "", 1,
	sizeof(q.Study.StudyID), (void *) q.Study.StudyID,
    &q.Study.Query_Flag, DB_K_QSTUDYID},
    {DCM_RELACCESSIONNUMBER, DCM_SH, "", 1,
	sizeof(q.Study.AccessionNumber),
	(void *) q.Study.AccessionNumber,
    &q.Study.Query_Flag, DB_K_QACCESSIONNUMBER},
    {DCM_RELSTUDYINSTANCEUID, DCM_UI, "", 1,
	sizeof(q.Study.StudyUID), (void *) q.Study.StudyUID,
    &q.Study.Query_Flag, DB_K_QSTUDYUID},

    {DCM_IDMODALITY, DCM_CS, "", 1,
	sizeof(q.Series.Modality), (void *) q.Series.Modality,
    &q.Series.Query_Flag, DB_K_QMODALITY},
    {DCM_RELSERIESNUMBER, DCM_IS, "", 1,
	sizeof(q.Series.SeriesNumber), (void *) q.Series.SeriesNumber,
    &q.Series.Query_Flag, DB_K_QSERIESNUMBER},
    {DCM_RELSERIESINSTANCEUID, DCM_UI, "", 1,
	sizeof(q.Series.SeriesUID), (void *) q.Series.SeriesUID,
    &q.Series.Query_Flag, DB_K_QSERIESUID},

    {DCM_RELIMAGENUMBER, DCM_IS, "", 1,
	sizeof(q.Image.ImageNumber), (void *) q.Image.ImageNumber,
    &q.Image.Query_Flag, DB_K_QIMAGENUMBER},
    {DCM_IDSOPINSTANCEUID, DCM_UI, "", 1,
	sizeof(q.Image.ImageUID), (void *) q.Image.ImageUID,
    &q.Image.Query_Flag, DB_K_QIMAGEUID}
};

/* HAP_ObjectToQuery
**
** Purpose:
**	This function translates a DICOM object and an SOP Class into
**	the structure used by the DB query facility.  The DICOM object
**	is expected to contain a query level attribute and any number
**	of attributes that act as keys.  This function extracts only
**	the attributes that are supported as keys by the DB facility and
**	places those values in the query structure.  Bit flags are
**	also set which define the SOP class for the query and the
**	query level.
**
**	The function returns DCM_NORMAL upon success and DCM_ILLEGALOBJECT
**	upon failure.
**
** Parameter Dictionary:
**	object		Pointer to caller's object with keys stored
**			as attributes.
**	SOPClass	The SOP class of the requested query.  The
**			caller passes this string and the function sets
**			a bit in the query structure.
**	query		Pointer to caller's structure that this routine
**			will fill in.
**	elementCount	Pointer to caller's variable which holds the number
**			of keys that we extract from the object.
** Return Values:
**	DCM_NORMAL
**	DCM_ILLEGALOBJECT
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
HAP_ObjectToQuery(DCM_OBJECT ** object, char *SOPClass, Query * query,
		  int *elementCount)
{
    CONDITION
	cond,
	returnCondition;
    int
        index;
    void
       *ctx;
    unsigned long
        l;
    long
        flag;
    static char
        queryLevelString[48];
    DCM_ELEMENT
	queryLevelElement =
    {
	DCM_IDQUERYLEVEL, DCM_CS, "", 1,
	    sizeof(queryLevelString), (void *) queryLevelString
    };

    *elementCount = 0;
    q.QueryState = 0;
    q.Patient.Query_Flag = 0;
    q.Study.Query_Flag = 0;
    q.Series.Query_Flag = 0;
    q.Image.Query_Flag = 0;

    ctx = NULL;
    returnCondition = HAP_NORMAL;

    cond = DCM_GetElementValue(object, &queryLevelElement, &l, &ctx);
    if (cond != DCM_NORMAL) {
	if (cond == DCM_ILLEGALOBJECT)
	    return COND_PushCondition(HAP_ILLEGALOBJECT, "");
	else {
	    (void) COND_PopCondition(FALSE);
	    (void) COND_PushCondition(HAP_QUERYLEVELMISSING, "");
	    returnCondition = HAP_INCOMPLETEOBJECT;
	}
    } else {
	queryLevelString[l] = '\0';
	if (queryLevelString[l - 1] == ' ')
	    queryLevelString[l - 1] = '\0';

	for (index = 0; index < DIM_OF(levelMap); index++) {
	    if (strcmp(levelMap[index].queryLevel, queryLevelString) == 0)
		q.QueryState |= levelMap[index].flag;
	}
    }

    flag = 0;
    for (index = 0; index < DIM_OF(classMap); index++) {
	if (strcmp(classMap[index].SOPClass, SOPClass) == 0)
	    flag |= classMap[index].flag;
    }
    if (flag == 0)
	return HAP_ILLEGALSOPCLASS;

    q.QueryState |= flag;

    for (index = 0; index < (int) DIM_OF(list); index++) {
	ctx = NULL;
	cond = DCM_GetElementValue(object, &list[index].e, &l, &ctx);
	if (cond == DCM_NORMAL) {
	    (*elementCount)++;
	    list[index].e.d.string[l] = '\0';
	    *list[index].flagAddress |= list[index].flag;
	} else
	    (void) COND_PopCondition(FALSE);
    }
    *query = q;
    return returnCondition;
}

/* HAP_QueryToObject
**
** Purpose:
**	This function translates a DB query structure into a DICOM
**	object and an SOP Class UID.  The function looks at the query
**	level and attributes which are flagged in query structure
**	and creates a DICOM object to be returned to the caller.
**	The function also determines the UID of the SOP Class from the
**	query structure and returns the UID to the caller.
**
**	The function returns DCM_NORMAL upon success and other DCM conditions
**	upon failure.
**
** Parameter Dictionary:
**	query		Pointer to caller's structure.  This routine takes
**			data from that structure and creates a DICOM object.
**	object		Pointer to caller's object which will be created by
**			this routine.
**	SOPClass	Pointer to caller's area.  This function determines
**			the SOP Class from the query structure and returns
**			an ASCIZ string in the caller's area with the UID
**			of the SOP class.
**	elementCount	Pointer to caller's variable which holds the number
**			of keys that we place in the object.
** Return Values:
**	DCM_NORMAL
**	DCM_ILLEGALOBJECT
**	other values as returned by DCM_CreateObject
**
** Return Values:
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

CONDITION
HAP_QueryToObject(Query * query, DCM_OBJECT ** object, char *SOPClass,
		  int *elementCount)
{
    CONDITION
	cond,
	returnCondition;
    int
        index;
    DCM_ELEMENT
	e;
    long
        flag;
    static char
        queryLevelString[48];
    DCM_ELEMENT
	queryLevelElement =
    {
	DCM_IDQUERYLEVEL, DCM_CS, "", 1,
	    sizeof(queryLevelString), (void *) queryLevelString
    };

    q = *query;
    *elementCount = 0;

    if (*object == NULL) {
	cond = DCM_CreateObject(object);
	if (cond != DCM_NORMAL)
	    return HAP_OBJECTCREATEFAILED;
    }
    returnCondition = HAP_NORMAL;
    flag = q.QueryState &
	(DB_K_LEVELPAT | DB_K_LEVELSTUDY | DB_K_LEVELSERIES | DB_K_LEVELIMAGE);
    if (flag == 0)
	returnCondition = COND_PushCondition(HAP_INCOMPLETEQUERY, "");
    else {
	for (index = 0; index < DIM_OF(levelMap); index++) {
	    if ((q.QueryState & levelMap[index].flag) != 0) {
		(void) strcpy(queryLevelString, levelMap[index].queryLevel);
		queryLevelElement.length = strlen(queryLevelElement.d.string);
		cond = DCM_ModifyElements(object, &queryLevelElement,
					  1, NULL, 0, NULL);
		if (cond != DCM_NORMAL)
		    return cond;
	    }
	}
    }


    flag = q.QueryState &
	(DB_K_CLASSPAT | DB_K_CLASSSTUDY | DB_K_CLASSPATSTUDY);
    if (flag == 0)
	returnCondition = COND_PushCondition(HAP_INCOMPLETEQUERY, "");
    else {
	for (index = 0; index < DIM_OF(classMap); index++) {
	    if ((q.QueryState & classMap[index].flag) != 0) {
		(void) strcpy(SOPClass, classMap[index].SOPClass);
		cond = DCM_NORMAL;
	    }
	}
    }

    for (index = 0; index < (int) DIM_OF(list); index++) {
	flag = *list[index].flagAddress;
	if ((flag & list[index].flag) != 0) {
	    e = list[index].e;
	    e.length = strlen(e.d.string);
	    cond = DCM_AddElement(object, &e);
	    if (cond != DCM_NORMAL)
		return cond;
	    (*elementCount)++;
	}
    }
    return returnCondition;
}


/* HAP_NormalizedToObject
**
** Purpose:
**      Translates a fixed structure to a DICOM object.
**
** Parameter Dictionary:
**      Define the parameters to the function
**
** Return Values:
**
** Notes:
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/

CONDITION
HAP_NormalizedToObject(DCM_OBJECT ** object,
		       HAP_ELEMENT_MAP * map, int size)
{
    int
        index,
        temp;
    CONDITION
	cond;
    DCM_ELEMENT
	e;

    for (index = 0; index < size; index++)
	if ((*map[index].flagAddress & map[index].flag) != 0) {
	    e = map[index].e;
	    if (DCM_IsString(e.representation))
		e.length = strlen(e.d.string);
	    cond = DCM_AddElement(object, &e);
	    if (cond != DCM_NORMAL)
		return HAP_ILLEGALOBJECT;
	}
    return HAP_NORMAL;
}

CONDITION
IAP_InstanceArraytoElement(char *SOPArray, size_t SOPSpacing,
	      char *UIDArray, size_t UIDSpacing, int count, DCM_ELEMENT * e)
{
    CONDITION
	cond;
    DCM_ELEMENT
	classElement = {
	DCM_IDREFERENCEDSOPCLASSUID, DCM_UI, "", 1, 0, NULL
    },
	uidElement = {
	DCM_IDREFERENCEDSOPINSTUID, DCM_UI, "", 1, 0, NULL
    };
    DCM_OBJECT
	* object;
    DCM_SEQUENCE_ITEM
	* item;

    e->d.sq = LST_Create();
    if (e->d.sq == NULL)
	return 0;		/* repair */

    while (count-- > 0) {
	item = malloc(sizeof(*item));
	if (item == NULL)
	    return 0;		/* repair */
	cond = DCM_CreateObject(&item->object);
	if (cond != DCM_NORMAL)
	    return 0;		/* repair */
	classElement.d.string = SOPArray;
	classElement.length = strlen(SOPArray);
	cond = DCM_AddElement(&item->object, &classElement);
	if (cond != DCM_NORMAL)
	    return 0;		/* repair */
	uidElement.d.string = UIDArray;
	uidElement.length = strlen(UIDArray);
	cond = DCM_AddElement(&item->object, &uidElement);
	if (cond != DCM_NORMAL)
	    return 0;		/* repair */

	cond = LST_Enqueue(&e->d.sq, item);
	if (cond != LST_NORMAL)
	    return 0;		/* repair */

	SOPArray += SOPSpacing;
	UIDArray += UIDSpacing;
    }
    return HAP_NORMAL;
}
