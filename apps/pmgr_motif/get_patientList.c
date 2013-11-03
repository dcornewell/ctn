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
**                              DICOM 93
**                   Electronic Radiology Laboratory
**                 Mallinckrodt Institute of Radiology
**              Washington University School of Medicine
**
** Module Name(s):
**                      SplitPName
**                      ConvertPNameToName
**                      selectStudy
**                      GetStudyList
**
** Author, Date:        Chander L. Sabharwal, November 7, 1994
**
** Intent:              To provide routines for searching
**                      throught the database and a list of
**                      available studies with images.
**                      Also to provide routines to display
**                      a list of studies in the database.
**
** Last Update:		$Author: smm $, $Date: 2002-10-31 23:36:06 $
** Source File:		$Source: /home/smm/ctn/ctn/apps/pmgr_motif/get_patientList.c,v $
** Revision:		$Revision: 1.12 $
** Status:		$State: Exp $
*/

#include "ctn_os.h"
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <Xm/DialogS.h>
#include <Xm/MenuShell.h>

#include "UxXt.h"

#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "tbl.h"
#include "dicom_uids.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_print.h"
#include "manage.h"
#include <X11/StringDefs.h>
#include <string.h>

Widget wMessageTxt;

#include "print_shared.h"
#include "print_client_app.h"

#include "dbquery.h"

#include "display.h"

static void ConvertPNameToName(char *PName, char *Name);
static void SplitPName(char *PName, char *lastname, char *firstname, char *middlename);
void load_list();
void loadStudyList();
void selected_study();


LST_HEAD
* lst_studylist = NULL;

extern char
   *db_file;

/* getStudyList
**
** Purpose:
**      Get the list of studies in the database.
**
** Parameter Dictionary:
**      TRUE
**	FALSE
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/

CTNBOOLEAN
getStudyList()
{
    short
        dbid;
    StudyLevel
	study;
    PatientLevel
	patient;
    CTNBOOLEAN
	got_a_patient = TRUE;
    CTNBOOLEAN
	got_a_study = TRUE;
    LIST_ITEM
	* item;
    int
        index = 0;


    static String
        text_studylist[30];

    LIST_ITEM l;
    PatientLevel *p;

    if (DB_Open(db_file, &dbid) != DB_NORMAL) {
	XmTextSetString(wMessageTxt, "Error: Failed to open = %s ", db_file);
	return (FALSE);
    }
    lst_studylist = LST_Create();
    if (lst_studylist == NULL) {
	XmTextSetString(wMessageTxt, "Error: LST_Create failed.");
	return (FALSE);
    }
    if (DB_GetPatient(dbid, &patient) != DB_NORMAL)
	got_a_patient = FALSE;

    while (got_a_patient) {
	got_a_study = TRUE;
	if (DB_GetStudy(dbid, patient.PatID, &study) != DB_NORMAL)
	    got_a_study = FALSE;
	while (got_a_study) {
	    item = (LIST_ITEM *) malloc(sizeof(LIST_ITEM));
	    if (item == NULL) {
		XmTextSetString(wMessageTxt, "Fatal Error: malloc failed.");
		exit(1);
	    }
	    ConvertPNameToName(patient.Name, item->PatientName);
	    strcpy(item->AccessionNumber, study.AccessionNumber);
	    strcpy(item->combo, study.AccessionNumber);
	    strcat(item->combo, ", ");
	    strcat(item->combo, item->PatientName);
	    text_studylist[index] = item->combo;
	    if (LST_Enqueue(&lst_studylist, item) != LST_NORMAL) {
		XmTextSetString(wMessageTxt, "Error: LST_Enqueue failed.");
		return (FALSE);
	    }
	    if (DB_GetNextStudy(dbid, &study) != DB_NORMAL)
		got_a_study = FALSE;
	}
	if (DB_GetNextPatient(dbid, &patient) != DB_NORMAL)
	    got_a_patient = FALSE;
	index++;
    }

    if (DB_Close(dbid) != DB_NORMAL) {
	XmTextSetString(wMessageTxt, "Error: DB_Close failed in GetStudyList.");
	return FALSE;
    }
    load_list();
    return TRUE;
}

/* ConvertPNameToName
**
** Purpose:
**      Decodes the DICOM patient name string.
**
** Parameter Dictionary:
**      PName           Patient's name
**      Name            Decoded name
**
** Return Values:
**      none
**
** Notes:
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/
static void
ConvertPNameToName(char *PName, char *Name)
{
    char
        first[40],
        last[40],
        middle[40];

    SplitPName(PName, last, first, middle);
    sprintf(Name, "%s %s %s", first, middle, last);
}
/* SplitPName
**
** Purpose:
**      This function splits a patient name encoded string into three parts.
**
** Parameter Dictionary:
**      PName           Patient's name
**      lastname        Patient's last name
**      firstname       Patient's first name
**      middlename      Patient's middle name
**
** Return Values:
**      None
**
** Notes:
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/

static void
SplitPName(char *PName, char *lastname, char *firstname, char *middlename)
{
    while (*PName != '\0' && *PName != '^') {
	*lastname++ = *PName++;
    }
    *lastname = '\0';


    if (*PName == '^')
	PName++;

    while (*PName != '\0' && *PName != '^') {
	*firstname++ = *PName++;
    }
    *firstname = '\0';


    if (*PName == '^')
	PName++;

    while (*PName != '\0' && *PName != '^') {
	*middlename++ = *PName++;
    }
    *middlename = '\0';
}

/* load_list
**
** Purpose:
**      This function is called to load the studylist in the window
**      to be selected.
**
** Parameter Dictionary:
**      None
**
** Return Values:
**      None.
**
** Notes:
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/

void
load_list()
{

    LIST_ITEM *item1;

    item1 = LST_Head(&lst_studylist);
    (void) LST_Position(&lst_studylist, item1);

    loadStudyList(lst_studylist);
}

/* selected_Study
**
** Purpose:
**      This function is called when the user selects a study from
**      the study list.
**
** Parameter Dictionary:
**      itemNo
**
** Return Values:
**      None.
**
** Notes:
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/

void
selected_study(int itemNo)
{
    int i;
    LIST_ITEM *se;

    se = LST_Head(&lst_studylist);
    (void) LST_Position(&lst_studylist, se);

    for (i = 2; i <= itemNo; i++)
	se = LST_Next(&lst_studylist);

}

void
freePatientResources()
{

    LIST_ITEM *se;

    if (lst_studylist != NULL) {
	while ((se = LST_Dequeue(&lst_studylist)) != NULL)
	    free(se);

	LST_Destroy(&lst_studylist);
	lst_studylist = NULL;
    }
}
