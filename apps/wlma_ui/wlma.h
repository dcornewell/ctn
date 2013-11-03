/*
+-+-+-+-+-+-+-+-+-
*/
/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:	Stephen Moore, 21-Jul-94
** Intent:		Define typedefs and prototypes for wlma functions
** Last Update:		$Author: smm $, $Date: 1994-10-20 20:14:32 $
** Source File:		$RCSfile: wlma.h,v $
** Revision:		$Revision: 1.3 $
** Status:		$State: Exp $
*/

typedef struct {
    void *reserved[2];
    FIS_PATIENTRECORD Patient;
    FIS_STUDYRECORD Study;
}   WLMA_PATIENTSTUDY;

typedef struct {
    void *reserved[2];
    FIS_STUDYCOMPONENTRECORD StudyComponent;
    FIS_SCSERIESRECORD SCSeries;
}   WLMA_STUDYCOMPONENTSCSERIES;

void expandPatientStudy(LST_HEAD * patientList, LST_HEAD * patientStudyList);
void expandStudyComponentSCSeries(LST_HEAD * scList, LST_HEAD * scSeriesList);
CONDITION
openTables(char *databaseName, char *application, DMAN_HANDLE ** database,
	   FIS_HANDLE ** FIS);
CONDITION
loadDatabase(FIS_HANDLE ** handle);
