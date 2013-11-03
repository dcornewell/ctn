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
** Intent:		Define typedefs and prototypes for RSA functions
** Last Update:		$Author: smm $, $Date: 1995-12-21 20:45:59 $
** Source File:		$RCSfile: rsa.h,v $
** Revision:		$Revision: 1.2 $
** Status:		$State: Exp $
*/

typedef struct {
    void *reserved[2];
    FIS_PATIENTRECORD Patient;
    FIS_STUDYRECORD Study;
}   RSA_PATIENTSTUDY;

void expandPatientStudy(LST_HEAD * patientList, LST_HEAD * patientStudyList);
void loadPatientStudyList(void);
CONDITION
openTables(char *databaseName, char *application, DMAN_HANDLE ** database,
	   FIS_HANDLE ** FIS);
CONDITION
loadDatabase(FIS_HANDLE ** handle);
CONDITION
distributeEvent(FIS_HANDLE ** fis, DMAN_HANDLE ** dman,
       FIS_DATA_TYPE type, void *d, FIS_EVENT event, char *localApplication,
		char *dst);
