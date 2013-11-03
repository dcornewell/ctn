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
** Author, Date:	Stephen M. Moore, 11-Jun-93
** Intent:
** Last Update:		$Author: smm $, $Date: 1999-09-24 03:04:19 $
** Source File:		$RCSfile: hap.h,v $
** Revision:		$Revision: 1.15 $
** Status:		$State: Exp $
*/

#ifdef  __cplusplus
extern "C" {
#endif

typedef enum {
    HAP_PATIENT_CREATED = 0x1,
    HAP_PATIENT_UPDATED = 0x2,
    HAP_PATIENT_DELETED = 0x4
} HAP_PATIENT_EVENT;

#define HAP_P_VARIABLE_EVENT		HAP_PATIENT_UPDATED

/* mandatory attributes for each patient event */

#define HAP_REQUIRED_PCREATED_ATTRIB	HIS_K_PATICDATE | \
					HIS_K_PATICTIME | \
					HIS_K_PATICUID | \
					HIS_K_PATPNAME | \
					HIS_K_PATPID | \
					HIS_K_PATPSEX | \
					HIS_K_PATPBDATE
#define HAP_REQUIRED_PDELETED_ATTRIB	HIS_K_PATPNAME | \
                                        HIS_K_PATPID
#define HAP_REQUIRED_PUPDATED_ATTRIB	HIS_K_PATPNAME | \
					HIS_K_PATPID

/* events under the Detached Visit Management SOP Class */

typedef enum {
    HAP_VISIT_CREATED = 0x8,
    HAP_VISIT_SCHEDULED = 0x10,
    HAP_PATIENT_ADMITTED = 0x20,
    HAP_PATIENT_TRANSFERRED = 0x40,
    HAP_PATIENT_DISCHARGED = 0x80,
    HAP_VISIT_DELETED = 0x100,
    HAP_VISIT_UPDATED = 0x200
} HAP_VISIT_EVENT;

#define HAP_V_VARIABLE_EVENT	(HAP_VISIT_CREATED | HAP_VISIT_UPDATED)

/* mandatory attributes for each visit event */

#define HAP_REQUIRED_VCREATED_ATTRIB (	HIS_K_VISICDATE | \
					HIS_K_VISICTIME | \
					HIS_K_VISICUID |\
					HIS_K_VISRPSEQUENCE |\
					HIS_K_VISVSID | \
					HIS_K_VISAID )
#define HAP_REQUIRED_VSCHEDULED_ATTRIB (HIS_K_VISSADATE |  \
					HIS_K_VISSATIME | \
					HIS_K_VISSPIRESIDENCE | \
					HIS_K_VISRPSEQUENCE |\
					HIS_K_VISVSID )
#define HAP_REQUIRED_VADMITTED_ATTRIB (	HIS_K_VISADATE | \
					HIS_K_VISATIME | \
					HIS_K_VISROADMISSIONS | \
					HIS_K_VISVSID | \
					HIS_K_VISRPSEQUENCE |\
					HIS_K_VISRPHYSICIAN )
#define HAP_REQUIRED_VTRANSFERRED_ATTRIB (	HIS_K_VISPIRESIDENCE | \
					HIS_K_VISCPLOCATION | \
					HIS_K_VISRPSEQUENCE |\
					HIS_K_VISVSID )
#define HAP_REQUIRED_VDISCHARGED_ATTRIB	( HIS_K_VISDDATE | \
					HIS_K_VISDTIME | \
					HIS_K_VISRPSEQUENCE |\
					HIS_K_VISVSID )
#define HAP_REQUIRED_VDELETED_ATTRIB (	HIS_K_VISAID )
#define HAP_REQUIRED_VUPDATED_ATTRIB	0x0

/* events under the Detached Study Management SOP Class */

typedef enum {
    HAP_STUDY_CREATED = 0x400,
    HAP_STUDY_SCHEDULED = 0x800,
    HAP_PATIENT_ARRIVED = 0x1000,
    HAP_STUDY_STARTED = 0x2000,
    HAP_STUDY_COMPLETED = 0x4000,
    HAP_STUDY_VERIFIED = 0x8000,
    HAP_STUDY_READ = 0x10000,
    HAP_STUDY_DELETED = 0x20000,
    HAP_STUDY_UPDATED = 0x40000
} HAP_STUDY_EVENT;

#define HAP_S_VARIABLE_EVENT	(HAP_STUDY_CREATED | HAP_STUDY_UPDATED)

/* mandatory attributes for study events */

#define HAP_REQUIRED_SCREATED_ATTRIB	(HIS_K_STUICDATE |\
					HIS_K_STUICTIME |\
					HIS_K_STUICUID |\
					HIS_K_STURPSEQUENCE |\
					HIS_K_STURVSEQUENCE |\
					HIS_K_STUSSID )
#define HAP_REQUIRED_SSCHEDULED_ATTRIB  (HIS_K_STUSSSDATE |\
                                        HIS_K_STUSSSTIME |\
                                        HIS_K_STUSSLOCATION |\
                                        HIS_K_STUSSLATITLE |\
                                        HIS_K_STURPDESCRIPTION |\
					HIS_K_STURPCSEQUENCE |\
                                        HIS_K_STURPSEQUENCE |\
                                        HIS_K_STURVSEQUENCE )
#define HAP_REQUIRED_SARRIVED_ATTRIB	(HIS_K_STUSADATE |\
					HIS_K_STUSATIME )
#define HAP_REQUIRED_SSTARTED_ATTRIB	(HIS_K_STUSDATE |\
					HIS_K_STUSTIME )
#define HAP_REQUIRED_SCOMPLETED_ATTRIB	(HIS_K_STUSCDATE |\
					HIS_K_STUSCTIME |\
					HIS_K_STURSCSEQUENCE)
#define HAP_REQUIRED_SVERIFIED_ATTRIB	(HIS_K_STUSVDATE |\
					HIS_K_STUSVTIME |\
					HIS_K_STURSCSEQUENCE)
#define HAP_REQUIRED_SREAD_ATTRIB	(HIS_K_STUSRDATE |\
					HIS_K_STUSRTIME )
#define HAP_REQUIRED_SUPDATED_ATTRIB	0x0
#define HAP_REQUIRED_SDELETED_ATTRIB	0x0


/* events under the Detached Results Management SOP Class */

typedef enum {
    HAP_RESULTS_CREATED = 0x80000,
    HAP_RESULTS_UPDATED = 0x100000,
    HAP_RESULTS_DELETED = 0x200000
} HAP_RESULTS_EVENT;

#define HAP_R_VARIABLE_EVENT	(HAP_RESULTS_CREATED | HAP_RESULTS_UPDATED)

/* mandatory attributes of results events */

#define HAP_REQUIRED_RCREATED_ATTRIB	(HIS_K_RESICDATE |\
					HIS_K_RESICTIME |\
					HIS_K_RESRSSEQUENCE |\
					HIS_K_RESRISEQUENCE |\
					HIS_K_RESICUID |\
					HIS_K_RESRID )
#define HAP_REQUIRED_RDELETED_ATTRIB	HIS_K_RESRID
#define HAP_REQUIRED_RUPDATED_ATTRIB	0x0

/* events under the Detached Interpretation Management SOP Class */

typedef enum {
    HAP_INTERP_CREATED = 0x400000,
    HAP_INTERP_RECORDED = 0x800000,
    HAP_INTERP_TRANSCRIBED = 0x1000000,
    HAP_INTERP_APPROVED = 0x2000000,
    HAP_INTERP_UPDATED = 0x4000000,
    HAP_INTERP_DELETED = 0x8000000
} HAP_INTERP_EVENT;

#define HAP_I_VARIABLE_EVENT	(HAP_INTERP_CREATED | HAP_INTERP_UPDATED)

/* mandatory attributes of interpretation events */

#define HAP_REQUIRED_ICREATED_ATTRIB	( HIS_K_INTICDATE |\
					HIS_K_INTICTIME |\
					HIS_K_INTICUID |\
					HIS_K_INTRRSEQUENCE |\
					HIS_K_INTITID |\
					HIS_K_INTISID )
#define HAP_REQUIRED_IRECORDED_ATTRIB	( HIS_K_INTIRDATE |\
					HIS_K_INTIRTIME |\
					HIS_K_INTIRECORDER |\
					HIS_K_INTITID |\
					HIS_K_INTISID )
#define HAP_REQUIRED_ITRANSCRIBED_ATTRIB	( HIS_K_INTITDATE |\
					HIS_K_INTITTIME |\
					HIS_K_INTIAUTHOR |\
					HIS_K_INTITID |\
					HIS_K_INTISID )
#define HAP_REQUIRED_IAPPROVED_ATTRIB	( HIS_K_INTIDIAGNOSIS |\
					HIS_K_INTIAPPSEQUENCE |\
					HIS_K_INTIDCSEQUENCE |\
					HIS_K_INTISID)
#define HAP_REQUIRED_IUPDATED_ATTRIB	0x0
#define HAP_REQUIRED_IDELETED_ATTRIB	0x0

typedef struct {
    DCM_TAG tag;
    unsigned long flag;
}   HAP_TAG_TO_FLAG;

CONDITION
HAP_BuildPatient(DCM_OBJECT ** o, HIS_PatientLevel * xl,
		 HAP_PATIENT_EVENT event);
CONDITION
HAP_BuildVisit(DCM_OBJECT ** object, HIS_VisitLevel * xl,
	       HAP_VISIT_EVENT event);
CONDITION
HAP_BuildStudy(DCM_OBJECT ** object, HIS_StudyLevel * xl,
	       HAP_STUDY_EVENT event);
CONDITION
HAP_BuildResults(DCM_OBJECT ** object, HIS_ResultsLevel * xl,
		 HAP_RESULTS_EVENT event);
CONDITION
HAP_BuildInterp(DCM_OBJECT ** object, HIS_InterpretationLevel * xl,
		HAP_INTERP_EVENT event);

CONDITION HAP_ParseInterp(DCM_OBJECT ** object, HIS_InterpretationLevel * xl);
CONDITION HAP_ParsePatient(DCM_OBJECT ** object, HIS_PatientLevel * xl);
CONDITION HAP_ParseResults(DCM_OBJECT ** object, HIS_ResultsLevel * xl);
CONDITION HAP_ParseStudy(DCM_OBJECT ** object, HIS_StudyLevel * xl);
CONDITION HAP_ParseVisit(DCM_OBJECT ** object, HIS_VisitLevel * xl);

#define	HAP_NORMAL	FORM_COND(FAC_HAP, SEV_SUCC, 1)
#define	HAP_ILLEGALOBJECT	FORM_COND(FAC_HAP, SEV_ERROR, 2)
#define	HAP_QUERYLEVELMISSING	FORM_COND(FAC_HAP, SEV_WARN, 3)
#define	HAP_INCOMPLETEOBJECT	FORM_COND(FAC_HAP, SEV_WARN, 4)
#define	HAP_ILLEGALSOPCLASS	FORM_COND(FAC_HAP, SEV_ERROR, 5)
#define	HAP_OBJECTCREATEFAILED	FORM_COND(FAC_HAP, SEV_ERROR, 6)
#define	HAP_INCOMPLETEQUERY	FORM_COND(FAC_HAP, SEV_WARN, 7)
#define HAP_ILLEGALEVENT	FORM_COND(FAC_HAP, SEV_ERROR, 8)
#define HAP_INCOMPLETEEVENT	FORM_COND(FAC_HAP, SEV_ERROR, 9)

#ifdef  __cplusplus
}
#endif
