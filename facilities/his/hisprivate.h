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
** Module Name(s):	dbprivate.h
** Author, Date:	David E. Beecher, 12-May-93
** Intent:		Private data structure for the database access
**			routines.
** Last Update:		$Author: smm $, $Date: 1996-08-23 19:35:01 $
** Source File:		$RCSfile: hisprivate.h,v $
** Revision:		$Revision: 1.14 $
** Status:		$State: Exp $
*/
#ifndef HISPRIVATE_IS_IN
#define HISPRIVATE_IS_IN

#ifdef  __cplusplus
extern "C" {
#endif

#define HIS_MAXKEYLENGTH	1024
#define HIS_NULL		-1

#define HIS_HUNKRECORDSIZE		1000
#define HIS_NUMRECSPERHUNK		1000
#define HIS_HUNKINITIALALLOCATION	2

#define HIS_MATCH	1
#define HIS_NOMATCH	0

#define HIS_MAXVISITS	HIS_MAXPATIENTS*HIS_MAXVISITSPERPAT
#define HIS_MAXSTUDIES	HIS_MAXVISITS*HIS_MAXSTUDIESPERVIS
#define HIS_MAXRESULTS	HIS_MAXSTUDIES*HIS_MAXRESULTSPERSTUDY
#define HIS_MAXINTERPS	HIS_MAXRESULTS*HIS_MAXINTERPSPERRES

typedef struct _HIS_Index {
    char
        uid[DICOM_UI_LENGTH + 1];
    HunkBufAdd
        badd;
    int
        next;
} HIS_Index;

typedef struct _Root {
    char
        dbkey[HIS_MAXKEYLENGTH];

    int
        num_patients,
        num_visits,
        num_studies,
        num_results,
        num_interps,
        free_p,
        used_p,
        free_v,
        used_v,
        free_s,
        used_s,
        free_r,
        used_r,
        free_i,
        used_i;

    HIS_Index
        patient[HIS_MAXPATIENTS],
        visit[HIS_MAXVISITS],
        study[HIS_MAXSTUDIES],
        results[HIS_MAXRESULTS],
        interps[HIS_MAXINTERPS];
} Root;

typedef struct _HISidstruct {
    char dbkey[HIS_MAXKEYLENGTH];
    short dbid;
    int
        patient_context,
        visit_context,
        study_context,
        results_context,
        interpretation_context;
    struct _HISidstruct
       *next;
} HISidstruct;

CONDITION HIS_Init(char *dbkey);
CONDITION HIS_Findid(short dbid);
CONDITION HIS_Removeid(short dbid, char *dbkey);
CONDITION HIS_Addid(short *dbid);

CONDITION HIS_AddStudytoPatient(char *studyuid, char *patuid);
CONDITION HIS_AddStudytoVisit(char *studyuid, char *visituid);
CONDITION HIS_AddVisittoPatient(char *visituid, char *patuid);
CONDITION HIS_AddResultstoStudy(char *resultuid, char *studyuid);
CONDITION HIS_AddInterpretationtoResult(char *interpsuid, char *resultuid);

CONDITION HIS_RemoveILocal(int i);
CONDITION HIS_RemoveRLocal(int i);
CONDITION HIS_RemoveSLocal(int i);
CONDITION HIS_RemoveVLocal(int i);

CONDITION HIS_RemoveInterpretationfromResult(char *interpsuid, char *resultuid);
CONDITION HIS_RemoveResultsfromStudy(char *resultsuid, char *studyuid);
CONDITION HIS_RemoveStudyfromVisit(char *studyuid, char *visituid);
CONDITION HIS_RemoveStudyfromPatient(char *studyuid, char *patientuid);
CONDITION HIS_RemoveVisitfromPatient(char *visituid, char *patientuid);


CONDITION HIS_UnLock(CONDITION retval);

int HIS_FindPatient(char *uid);
int HIS_FindVisit(char *uid);
int HIS_FindStudy(char *uid);
int HIS_FindResults(char *uid);
int HIS_FindInterpretation(char *uid);

#ifdef  __cplusplus
}
#endif

#endif
