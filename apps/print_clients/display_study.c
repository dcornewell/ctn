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
**			SplitPName
**			ConvertPNameToName
**			selectStudy
**			GetStudyList
**			CreateStudyListWindow
** Author, Date:	Tze Khiong Lee, SEPTEMBER 14th 1993
** Intent:		To provide routines for searching
**			throught the database and a list of
**			available studies with images.
**			Also to provide routines to display
**			a list of studies in the database.
** Last Update:		$Author: smm $, $Date: 2001-12-21 16:48:32 $
** Source File:		RCSfilee: /dicom1a/projects/dicom93/apps/RCS/display_study.c,v $
** Revision:		$Revision: 1.10 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.10 $ $RCSfile: display_study.c,v $";
#ifdef ATHENA
#include "ctn_os.h"

#if 0
#include <stdio.h>
#include <stdlib.h>
#endif

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Toggle.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/List.h>
#include "dicom.h"
#include "dicom_uids.h"
#include "lst.h"
#include "condition.h"
#include "dbquery.h"

#include "Buttons.h"
#include "icon.h"
#include "display_icon.h"

typedef struct {
    void
       *reserved[2];
    char
        PatientName[DICOM_PN_LENGTH + 1],
        AccessionNumber[DICOM_IS_LENGTH + 1];
    char
        combo[DICOM_PN_LENGTH + DICOM_IS_LENGTH + 6];
}   LIST_ITEM;

static LST_HEAD
*   lst_studylist;

static String
    text_studylist[30];

static Widget
    list,
    shell;

extern Widget
    toplevel,
    mainwin;
extern char
   *icon_file,
   *icon_index,
   *db_file;


/* SplitPName
**
** Purpose:
**	This function splits a patient name encoded string into three parts.
**
** Parameter Dictionary:
**	PName		Patient's name
**	lastname	Patient's last name
**	firstname	Patient's first name
**	middlename	Patient's middle name
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
SplitPName(char PName[], char lastname[], char firstname[], char middlename[])
{
    int
        i,
        first,
        second;

    memset(lastname, 0, 20);
    memset(firstname, 0, 24);
    memset(middlename, 0, 20);
    for (i = 0; (i < DICOM_PN_LENGTH) && (PName[i] != '^'); i++)
	lastname[i] = PName[i];
    lastname[i++] = '\0';
    first = i;
    for (; (i < DICOM_PN_LENGTH) && (PName[i] != '^'); i++)
	firstname[i - first] = PName[i];
    firstname[i++] = '\0';
    second = i;
    for (; (i < DICOM_PN_LENGTH) && (PName[i] != '^'); i++)
	middlename[i - second] = PName[i];
    middlename[i++] = '\0';
}


/* ConvertPNameToName
**
** Purpose:
**	Decodes the DICOM patient name string.
**
** Parameter Dictionary:
**	PName		Patient's name
**	Name		Decoded name
**
** Return Values:
**	none
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static void
ConvertPNameToName(char *PName, char *Name)
{
    char
        first[24],
        last[20],
        middle[20];

    SplitPName(PName, last, first, middle);
    sprintf(Name, "%s %s %s", first, middle, last);
}


/* selectStudy
**
** Purpose:
**	This function is called when the user selects a study from
**	the study list.  This will then call other functions
**	to display the images in that study.
**
** Parameter Dictionary:
**	w
**	client_data
**	call_data
**
** Return Values:
**	None.
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static void
selectStudy(Widget w, XtPointer client_data, XtPointer call_data)
{
    XawListReturnStruct
    * data;
    LIST_ITEM
	* item;

    data = (XawListReturnStruct *) call_data;
    printf("you have selected %s\n", data->string);
    item = LST_Position(&lst_studylist, LST_Head(&lst_studylist));
    if (item == NULL) {
	printf("Fatal Error: LST_Position failed in selectStudy \n");
	return;
    }
    printf("searching in list, getting \n");
    printf("%s\n", item->combo);
    while ((item != NULL) && (strcmp(item->combo, data->string) != 0)) {
	item = LST_Next(&lst_studylist);
	printf("searching in list, getting \n");
	printf("%s\n", item->combo);
    }
    if (item == NULL) {
	printf("Fatal Error: Major Logic Error in selectStudy\n");
	return;
    }
    if (PreviewStudy(icon_index, icon_file, item->AccessionNumber) == FALSE) {
	COND_DumpConditions();
	printf("Error: PreviewStudy failed in selectStudy\n");
    }
}


/* GetStudyList
**
** Purpose:
**	Get the list of studies in the database.
**
** Parameter Dictionary:
**	None
**
** Return Values:
**	TRUE
**	FALSE
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
Boolean
GetStudyList()
{
    short
        dbid;
    StudyLevel
	study;
    PatientLevel
	patient;
    Boolean
	got_a_patient = TRUE;
    Boolean
	got_a_study = TRUE;
    LIST_ITEM
	* item;
    int
        index = 0;

    if (DB_Open(db_file, &dbid) != DB_NORMAL) {
	printf("Error: Failed to open %s\n", db_file);
	return (FALSE);
    }
    lst_studylist = LST_Create();
    if (lst_studylist == NULL) {
	printf("Error: LST_Create failed\n");
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
		printf("Fatal Error: malloc failed\n");
		exit(1);
	    }
	    ConvertPNameToName(patient.Name, item->PatientName);
	    strcpy(item->AccessionNumber, study.AccessionNumber);
	    strcpy(item->combo, study.AccessionNumber);
	    strcat(item->combo, ", ");
	    strcat(item->combo, item->PatientName);
	    text_studylist[index] = item->combo;
	    if (LST_Enqueue(&lst_studylist, item) != LST_NORMAL) {
		printf("Error: LST_Enqueue failed\n");
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
	printf("Error: DB_Close failed in GetStudyList\n");
	return FALSE;
    }
    return FALSE;
}


/* CreateStudyListWindow
**
** Purpose:
**	Creates a window that will display the list of studies
**	in the database.
**
** Parameter Dictionary:
**	None
**
** Return Values:
**	Handle to a widget
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
Widget
CreateStudyListWindow()
{
    Widget
	form,
	label;
    unsigned int
        i;

    if (GetStudyList() == FALSE) {
	printf("Fatal Error: GetStudyList failed\n");
	return 0;
    }
    shell = XtVaCreatePopupShell("Study List",
				 topLevelShellWidgetClass, toplevel, NULL);
    form = XtVaCreateManagedWidget("study_form", formWidgetClass,
				   shell, NULL);
    label = XtVaCreateManagedWidget("study_label", labelWidgetClass,
		      form, XtNlabel, "List of Studies:", XtNborderWidth, 0,
				    XtNresizable, FALSE, NULL);
    list = CreateSimpleList(form, label, NULL, 300, 120, selectStudy, NULL,
			  text_studylist, LST_Count(&lst_studylist), FALSE);
    if (list == 0) {
	printf("Could not create list widget\n");
	return 0;
    }
    for (i = 0; i < LST_Count(&lst_studylist); i++)
	printf("%s\n", text_studylist[i]);
    return (shell);
}

#endif
