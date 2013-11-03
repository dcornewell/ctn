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
** Module Name(s):	hisdb.h
** Author, Date:	David E. Beecher, 14-May-93
** Intent:		Include file for his database queries.
** Last Update:		$Author: smm $, $Date: 1996-08-23 19:34:59 $
** Source File:		$RCSfile: hisdb.h,v $
** Revision:		$Revision: 1.22 $
** Status:		$State: Exp $
*/

#ifndef HISDB_IS_IN
#define HISDB_IS_IN 1

#ifdef  __cplusplus
extern "C" {
#endif

#define HIS_MAXPATIENTS		15
#define HIS_MAXVISITSPERPAT	5
#define HIS_MAXSTUDIESPERPAT	25
#define HIS_MAXSTUDIESPERVIS	5
#define HIS_MAXPATIENTSPERVIS	2
#define HIS_MAXPATIENTSPERSTUDY	2
#define HIS_MAXVISITSPERSTUDY	2
#define HIS_MAXRESULTSPERSTUDY	5
#define HIS_MAXSTUDIESPERRES	1
#define HIS_MAXINTERPSPERRES	5
#define HIS_MAXRESULTSPERINT	1
#define HIS_MAXVISADCSEQUENCE	5
#define HIS_MAXVISDDCSEQUENCE	5
#define HIS_MAXSTUPCSEQUENCE	5
#define HIS_MAXSTURPCSEQUENCE	5
#define HIS_MAXSTURSCSEQUENCE 	5
#define HIS_MAXINTERPDCSEQUENCE 5
#define HIS_MAXINTERPAPPROVSEQUENCE 5
/*
 * Query flags for database searches and updates...
 */
/* Patient Level */
#define HIS_K_PATICDATE		0x00000001
#define HIS_K_PATICTIME		0x00000002
#define HIS_K_PATICUID		0x00000004
#define HIS_K_PATRVSEQUENCE	0x00008000
#define HIS_K_PATRSSEQUENCE	0x00010000
#define	HIS_K_PATPNAME		0x00000008
#define	HIS_K_PATPID		0x00000010
#define	HIS_K_PATIOPID		0x00000020
#define	HIS_K_PATOPIDS		0x00000040
#define	HIS_K_PATOPNAMES	0x00000080
#define	HIS_K_PATPADDRESS	0x00000100
#define	HIS_K_PATPTNUMBERS	0x00000200
#define	HIS_K_PATPBDATE		0x00000400
#define	HIS_K_PATPSEX		0x00000800
#define	HIS_K_PATEGROUP		0x00001000
#define	HIS_K_PATRPREFERENCE	0x00002000
#define	HIS_K_PATCALLERGIES	0x00004000

/* Visit Level */
#define HIS_K_VISICDATE		0x00000001
#define HIS_K_VISICTIME		0x00000002
#define HIS_K_VISICUID		0x00000004
#define HIS_K_VISRPSEQUENCE	0x01000000
#define HIS_K_VISRSSEQUENCE	0x02000000
#define HIS_K_VISIIDNAME	0x00000008
#define HIS_K_VISIOIID		0x00000010
#define HIS_K_VISAID		0x00000020
#define HIS_K_VISSADATE		0x00000040
#define HIS_K_VISSATIME		0x00000080
#define HIS_K_VISADATE		0x00000100
#define HIS_K_VISATIME		0x00000200
#define HIS_K_VISROADMISSIONS	0x00000400
#define HIS_K_VISRPHYSICIAN	0x00000800
#define HIS_K_VISADDESCRIPTION	0x00001000
/*
#define HIS_K_VISADCSUID	0x00002000
#define HIS_K_VISADCODE		0x00004000
*/
#define HIS_K_VISADCSEQUENCE	0x00004000
#define HIS_K_VISVSID		0x00008000
#define HIS_K_VISCPLOCATION	0x00010000
#define HIS_K_VISSPIRESIDENCE	0x00020000
#define HIS_K_VISPIRESIDENCE	0x00040000
#define HIS_K_VISDDATE		0x00080000
#define HIS_K_VISDTIME		0x00100000
#define HIS_K_VISDDDESCRIPTION	0x00200000
/*
#define HIS_K_VISDDCSUID	0x00400000
#define HIS_K_VISDDCODE		0x00800000
*/
#define HIS_K_VISDDCSEQUENCE	0x00800000

/* Study Level */
#define HIS_K_STUICDATE		0x00000001
#define HIS_K_STUICTIME		0x00000002
#define HIS_K_STUICUID		0x00000004
#define HIS_K_STUANUMBER	0x00000008
#define HIS_K_STURPSEQUENCE	0x40000000
#define HIS_K_STURVSEQUENCE	0x80000000
/*
 * This is defined for Update_Flag2 only!
 */
#define HIS_K_STURRSEQUENCE	0x00000001

#define HIS_K_STUSID		0x00000010
#define HIS_K_STUSIISSUER	0x00000020
#define HIS_K_STUDESCRIPTION	0x00000040
/*
#define HIS_K_STUPCSUID		0x00000080
#define HIS_K_STUPCODE		0x00000100
*/
#define HIS_K_STUPCSEQUENCE	0x00000100
#define HIS_K_STUSSID		0x00000200
#define HIS_K_STUSPID		0x00000400
#define HIS_K_STUSSSDATE	0x00000800
#define HIS_K_STUSSSTIME	0x00001000
#define HIS_K_STUSSLOCATION	0x00002000
#define HIS_K_STUSSLATITLE	0x00004000
#define HIS_K_STURFSTUDY	0x00008000
#define HIS_K_STURPHYSICIAN	0x00010000
#define HIS_K_STURPDESCRIPTION	0x00020000
/* removed from the standard Aug 27
#define HIS_K_STURPCSUID	0x00040000
#define HIS_K_STURPCODE		0x00080000
*/
#define HIS_K_STURPCSEQUENCE	0x00080000
#define HIS_K_STUSADATE		0x00100000
#define HIS_K_STUSATIME		0x00200000
#define HIS_K_STUSDATE		0x00400000
#define HIS_K_STUSTIME		0x00800000
#define HIS_K_STURSCSEQUENCE	0x00040000	/* added in Aug 27 standard */
#define HIS_K_STUSCDATE		0x01000000
#define HIS_K_STUSCTIME		0x02000000
#define HIS_K_STUSVDATE		0x04000000
#define HIS_K_STUSVTIME		0x08000000
#define HIS_K_STUSRDATE		0x10000000
#define HIS_K_STUSRTIME		0x20000000

/* Results Level */
#define HIS_K_RESICDATE		0x00000001
#define HIS_K_RESICTIME		0x00000002
#define HIS_K_RESICUID		0x00000004
#define HIS_K_RESRSSEQUENCE	0x00000080
#define HIS_K_RESRISEQUENCE	0x00000100
#define HIS_K_RESRID		0x00000008
#define HIS_K_RESRIISSUER	0x00000010
#define HIS_K_RESIMPRESSIONS	0x00000020
#define HIS_K_RESCOMMENTS	0x00000040


/* Interpretation Level */
#define HIS_K_INTICDATE		0x00000001
#define HIS_K_INTICTIME		0x00000002
#define HIS_K_INTICUID		0x00000004
#define HIS_K_INTRRSEQUENCE	0x00800000
#define HIS_K_INTIID		0x00000008
#define HIS_K_INTIIISSUER	0x00000010
#define HIS_K_INTITID		0x00000020
#define HIS_K_INTISID		0x00000040
#define HIS_K_INTIRDATE		0x00000080
#define HIS_K_INTIRTIME		0x00000100
#define HIS_K_INTIRECORDER	0x00000200
#define HIS_K_INTITRANSCRIBER	0x00000400
#define HIS_K_INTITDATE		0x00000800
#define HIS_K_INTITTIME		0x00001000
#define HIS_K_INTIAUTHOR	0x00002000
#define HIS_K_INTIAPPSEQUENCE	0x00004000
/*
#define HIS_K_INTIADATES	0x00008000
#define HIS_K_INTIATIMES	0x00010000
#define HIS_K_INTPAINTERPRETATION 0x00020000
*/
#define HIS_K_INTIDIAGNOSIS	0x00040000
/*
#define HIS_K_INTIDCSUID	0x00080000
#define HIS_K_INTIDCODES	0x00100000
*/
#define HIS_K_INTIDCSEQUENCE	0x00100000
#define HIS_K_INTITEXT		0x00200000
#define HIS_K_INTRDLSEQUENCE	0x00400000

typedef struct {
    char
        CodeValue[DICOM_SH_LENGTH + 1],
        CodingSchemeDesignator[DICOM_SH_LENGTH + 1],
        CodeMeaning[DICOM_LO_LENGTH + 1];
}   CODE_SEQUENCE;

typedef struct {
    char
        PIUID[DICOM_UI_LENGTH + 1],	/* Unique	 */
        ICDate[DICOM_DA_LENGTH + 1],	/* Required	 */
        ICTime[DICOM_TM_LENGTH + 1],	/* Required	 */
        ICUID[DICOM_UI_LENGTH + 1];	/* Required	 */
    long
        RVSequenceCount;	/* Required	 */
    char
        RVSequence[HIS_MAXVISITSPERPAT][DICOM_UI_LENGTH + 1];
    /* Required	 */
    long
        RSSequenceCount;	/* Required	 */
    char
        RSSequence[HIS_MAXSTUDIESPERPAT][DICOM_UI_LENGTH + 1];
    /* Required	 */
    char
        PName[DICOM_PN_LENGTH + 1],	/* Required	 */
        PID[DICOM_LO_LENGTH + 1],	/* Required	 */
        IoPID[DICOM_LO_LENGTH + 1],	/* Required	 */
        OPIDs[DICOM_LO_LENGTH + 1],	/* Required	 */
        OPNames[DICOM_LO_LENGTH + 1],	/* Required	 */
        PAddress[DICOM_LO_LENGTH + 1],	/* Required	 */
        PTNumbers[DICOM_SH_LENGTH + 1],	/* Required	 */
        PBDate[DICOM_DA_LENGTH + 1],	/* Required	 */
        PSex[DICOM_CS_LENGTH + 1],	/* Required	 */
        EGroup[DICOM_SH_LENGTH + 1],	/* Required	 */
        RPreference[DICOM_LO_LENGTH + 1],	/* Required	 */
        CAllergies[DICOM_LO_LENGTH + 1];	/* Required	 */
    long
        Update_Flag;
} HIS_PatientLevel;

typedef struct {
    char
        PVIUID[DICOM_UI_LENGTH + 1],	/* Unique	 */
        ICDate[DICOM_DA_LENGTH + 1],	/* Required	 */
        ICTime[DICOM_TM_LENGTH + 1],	/* Required	 */
        ICUID[DICOM_UI_LENGTH + 1];	/* Required	 */
    long
        RPSequenceCount;	/* Required	 */
    char
        RPSequence[HIS_MAXPATIENTSPERVIS][DICOM_UI_LENGTH + 1];
    /* Required	 */
    long
        RSSequenceCount;	/* Required	 */
    char
        RSSequence[HIS_MAXSTUDIESPERVIS][DICOM_UI_LENGTH + 1];
    /* Required	 */
    char
        IIDName[DICOM_LO_LENGTH + 1],	/* Required	 */
        IoIID[DICOM_LO_LENGTH + 1],	/* Required	 */
        AID[DICOM_LO_LENGTH + 1],	/* Required	 */
        SADate[DICOM_DA_LENGTH + 1],	/* Required	 */
        SATime[DICOM_TM_LENGTH + 1],	/* Required	 */
        ADate[DICOM_DA_LENGTH + 1],	/* Required	 */
        ATime[DICOM_TM_LENGTH + 1],	/* Required	 */
        RoAdmissions[DICOM_LO_LENGTH + 1],	/* Required	 */
        RPhysician[DICOM_PN_LENGTH + 1],	/* Required	 */
        ADDescription[DICOM_LO_LENGTH + 1];	/* Required	 */
/*
        ADCSUID[DICOM_UI_LENGTH + 1],	 Required
        ADcode[DICOM_SH_LENGTH + 1],	 Required
*/
    long
        ADCSequenceCount;
        CODE_SEQUENCE
        ADCSequence[HIS_MAXVISADCSEQUENCE];
    char
        VSID[DICOM_CS_LENGTH + 1],	/* Required	 */
        CPLocation[DICOM_LO_LENGTH + 1],	/* Required	 */
        SPIResidence[DICOM_LO_LENGTH + 1],	/* Required	 */
        PIResidence[DICOM_LO_LENGTH + 1],	/* Required	 */
        DDate[DICOM_DA_LENGTH + 1],	/* Required	 */
        DTime[DICOM_TM_LENGTH + 1],	/* Required	 */
        DDDescription[DICOM_LO_LENGTH + 1];	/* Required	 */
/*
        DDCSUID[DICOM_UI_LENGTH + 1],	Required
        DDCode[DICOM_SH_LENGTH + 1];	Required
*/
    long
        DDCSequenceCount;
        CODE_SEQUENCE
        DDCSequence[HIS_MAXVISDDCSEQUENCE];
    long
        Update_Flag;
} HIS_VisitLevel;

typedef struct {
    char
        SIUID[DICOM_UI_LENGTH + 1],	/* Unique	 */
        ANumber[DICOM_SH_LENGTH + 1],	/* Required	 */
        ICDate[DICOM_DA_LENGTH + 1],	/* Required	 */
        ICTime[DICOM_TM_LENGTH + 1],	/* Required	 */
        ICUID[DICOM_UI_LENGTH + 1];	/* Required	 */
    long
        RPSequenceCount;	/* Required	 */
    char
        RPSequence[HIS_MAXPATIENTSPERSTUDY][DICOM_UI_LENGTH + 1];
    /* Required	 */
    long
        RVSequenceCount;	/* Required	 */
    char
        RVSequence[HIS_MAXVISITSPERSTUDY][DICOM_UI_LENGTH + 1];
    /* Required	 */
    long
        RRSequenceCount;	/* Required	 */
    char
        RRSequence[HIS_MAXRESULTSPERSTUDY][DICOM_UI_LENGTH + 1];
    char
        SID[DICOM_CS_LENGTH + 1],	/* Required	 */
        SIIssuer[DICOM_LO_LENGTH + 1],	/* Required	 */
        Description[DICOM_LO_LENGTH + 1];	/* Required	 */
/*
        PCSUID[DICOM_UI_LENGTH + 1],	Required
        PCode[DICOM_SH_LENGTH + 1],	Required
*/
    long
        PCSequenceCount;
        CODE_SEQUENCE
        PCSequence[HIS_MAXSTUPCSEQUENCE];
    char
        StudyStatusID[DICOM_CS_LENGTH + 1],	/* Required */
        StudyPriorityID[DICOM_CS_LENGTH + 1],	/* Required */
        SSSDate[DICOM_DA_LENGTH + 1],	/* Required	 */
        SSSTime[DICOM_TM_LENGTH + 1],	/* Required	 */
        SSLocation[DICOM_LO_LENGTH + 1],	/* Required	 */
        SSLATitle[DICOM_AE_LENGTH + 1],	/* Required	 */
        RfStudy[DICOM_LO_LENGTH + 1],	/* Required	 */
        RPhysician[DICOM_PN_LENGTH + 1],	/* Required	 */
        RPDescription[DICOM_LO_LENGTH + 1];	/* Required	 */
/*
        RPCSUID[DICOM_UI_LENGTH + 1],	 Required
        RPCode[DICOM_SH_LENGTH + 1],	 Required
*/
    long
        RPCSequenceCount;
        CODE_SEQUENCE
        RPCSequence[HIS_MAXSTURPCSEQUENCE];
    char
        SADate[DICOM_DA_LENGTH + 1],	/* Required	 */
        SATime[DICOM_TM_LENGTH + 1],	/* Required	 */
        SDate[DICOM_DA_LENGTH + 1],	/* Required	 */
        STime[DICOM_TM_LENGTH + 1];	/* Required	 */
    long
        RSCSequenceCount;	/* Required	 */
    char
        RSCRSCUIDSequence[HIS_MAXSTURSCSEQUENCE][DICOM_UI_LENGTH + 1],
        RSCRSIUIDSequence[HIS_MAXSTURSCSEQUENCE][DICOM_UI_LENGTH + 1],
    /* Required	 */
        SCDate[DICOM_DA_LENGTH + 1],	/* Required	 */
        SCTime[DICOM_TM_LENGTH + 1],	/* Required	 */
        SVDate[DICOM_DA_LENGTH + 1],	/* Required	 */
        SVTime[DICOM_TM_LENGTH + 1],	/* Required	 */
        SRDate[DICOM_DA_LENGTH + 1],	/* Required	 */
        SRTime[DICOM_TM_LENGTH + 1];	/* Required	 */
    long
        Update_Flag,
        Update_Flag2;
} HIS_StudyLevel;

typedef struct {
    char
        RIUID[DICOM_UI_LENGTH + 1],	/* Unique	 */
        ICDate[DICOM_DA_LENGTH + 1],	/* Required	 */
        ICTime[DICOM_TM_LENGTH + 1],	/* Required	 */
        ICUID[DICOM_UI_LENGTH + 1];	/* Required	 */
    long
        RSSequenceCount;	/* Required	 */
    char
        RSSequence[HIS_MAXSTUDIESPERRES][DICOM_UI_LENGTH + 1];
    /* Required	 */
    long
        RISequenceCount;	/* Required	 */
    char
        RISequence[HIS_MAXINTERPSPERRES][DICOM_UI_LENGTH + 1];
    /* Required	 */
    char
        RID[DICOM_SH_LENGTH + 1],	/* Required	 */
        RIIssuer[DICOM_LO_LENGTH + 1],	/* Required	 */
        Impressions[DICOM_ST_LENGTH + 1],	/* Required	 */
        Comments[DICOM_ST_LENGTH + 1];	/* Required	 */
    long
        Update_Flag;
} HIS_ResultsLevel;

typedef struct {
    char
        IIUID[DICOM_UI_LENGTH + 1],	/* Unique	 */
        ICDate[DICOM_DA_LENGTH + 1],	/* Required	 */
        ICTime[DICOM_TM_LENGTH + 1],	/* Required	 */
        ICUID[DICOM_UI_LENGTH + 1];	/* Required	 */
    long
        RRSequenceCount;	/* Required	 */
    char
        RRSequence[HIS_MAXRESULTSPERINT][DICOM_UI_LENGTH + 1];
    /* Required	 */
    char
        IID[DICOM_SH_LENGTH + 1],	/* Required	 */
        IIIssuer[DICOM_LO_LENGTH + 1],	/* Required	 */
        ITID[DICOM_CS_LENGTH + 1],	/* Required	 */
        ISID[DICOM_CS_LENGTH + 1],	/* Required	 */
        IRDate[DICOM_DA_LENGTH + 1],	/* Required	 */
        IRTime[DICOM_TM_LENGTH + 1],	/* Required	 */
        IRecorder[DICOM_PN_LENGTH + 1],	/* Required	 */
        ITranscriber[DICOM_PN_LENGTH + 1],	/* Required	 */
        ITDate[DICOM_DA_LENGTH + 1],	/* Required	 */
        ITTime[DICOM_TM_LENGTH + 1],	/* Required	 */
        IAuthor[DICOM_PN_LENGTH + 1];	/* Required	 */
    long
        IApprovalSequenceCount;
    char
        IApprover[HIS_MAXINTERPAPPROVSEQUENCE][DICOM_PN_LENGTH + 1],	/* Required	 */
        IADates[HIS_MAXINTERPAPPROVSEQUENCE][DICOM_DA_LENGTH + 1],	/* Required	 */
        IATimes[HIS_MAXINTERPAPPROVSEQUENCE][DICOM_TM_LENGTH + 1],	/* Required	 */
/*
        PAInterpretation[DICOM_PN_LENGTH + 1],	Required
*/
        IDiagnosis[DICOM_LT_LENGTH + 1];	/* Required	 */
/*
        IDCSUID[DICOM_UI_LENGTH + 1],	 Required
        IDCodes[DICOM_SH_LENGTH + 1],	 Required
*/
    long
        IDCSequenceCount;
        CODE_SEQUENCE
        IDCSequence[HIS_MAXINTERPDCSEQUENCE];
    char
        IText[DICOM_ST_LENGTH + 1],	/* Required	 */
        RDLSequence[10];	/* Required	 */
    long
        Update_Flag;
} HIS_InterpretationLevel;

/*
 * Function Prototypes...
 */

CONDITION HIS_CreateDB(char *dbkey);
CONDITION HIS_DeleteDB(char *dbkey);
CONDITION HIS_Open(char *dbkey, short *dbid);
CONDITION HIS_Close(short dbid);

/*
 * The add functions...
 */
CONDITION HIS_AddPatient(short dbid, HIS_PatientLevel *pat);
CONDITION HIS_AddVisit(short dbid, HIS_VisitLevel *visit);
CONDITION HIS_AddStudy(short dbid, HIS_StudyLevel *study);
CONDITION HIS_AddResults(short dbid, HIS_ResultsLevel *results);
CONDITION HIS_AddInterpretation(short dbid, HIS_InterpretationLevel *interp);

/*
 * The delete functions...
 */
CONDITION HIS_DelPatient(short dbid, char *patuid);
CONDITION HIS_DelVisit(short dbid, char *visituid);
CONDITION HIS_DelStudy(short dbid, char *studyuid);
CONDITION HIS_DelResults(short dbid, char *resultsuid);
CONDITION HIS_DelInterpretation(short dbid, char *interpuid);

/*
 * The Get/GetNext functions...
 *	if the uid field in the structure is not null, the Getxxx
 *	function will return the structure pointed to by that uid...
 */
CONDITION
HIS_GetPatient(short dbid, HIS_PatientLevel *patient);
CONDITION
HIS_GetNextPatient(short dbid, HIS_PatientLevel *patient);

CONDITION
HIS_GetVisit(short dbid, HIS_VisitLevel *visit);
CONDITION
HIS_GetNextVisit(short dbid, HIS_VisitLevel *visit);

CONDITION
HIS_GetStudy(short dbid, HIS_StudyLevel *study);
CONDITION
HIS_GetNextStudy(short dbid, HIS_StudyLevel *study);

CONDITION
HIS_GetResults(short dbid, HIS_ResultsLevel *results);
CONDITION
HIS_GetNextResults(short dbid, HIS_ResultsLevel *results);

CONDITION
HIS_GetInterpretation(short dbid, HIS_InterpretationLevel *interp);
CONDITION HIS_GetNextInterpretation(short dbid, HIS_InterpretationLevel *interp);

/*
 * The update functions...
 *	These functions use the Update_Flag bit field to determine
 *	which fields to update in the record selected by the uid.
 *	Note that the HIS_StudyLevel structure contains 2 such flags because
 *	it contains more than 32 elements.
 */
CONDITION HIS_UpdatePatient(short dbid, HIS_PatientLevel *patient);
CONDITION HIS_UpdateVisit(short dbid, HIS_VisitLevel *visit);
CONDITION HIS_UpdateStudy(short dbid, HIS_StudyLevel *study);
CONDITION HIS_UpdateResults(short dbid, HIS_ResultsLevel *results);
CONDITION HIS_UpdateInterpretation(short dbid, HIS_InterpretationLevel *interp);

/*
 * The error return codes
 */

#define	HIS_NORMAL		FORM_COND(FAC_DB,SEV_SUCC,1)
#define	HIS_OK			FORM_COND(FAC_DB,SEV_SUCC,1)
#define	HIS_CREATERROR		FORM_COND(FAC_DB,SEV_ERROR,2)
#define	HIS_OPENERROR		FORM_COND(FAC_DB,SEV_ERROR,3)
#define	HIS_ALLOCATERROR	FORM_COND(FAC_DB,SEV_ERROR,4)
#define	HIS_WRITERROR		FORM_COND(FAC_DB,SEV_ERROR,5)
#define	HIS_CLOSERROR		FORM_COND(FAC_DB,SEV_ERROR,6)
#define	HIS_IDREMERROR		FORM_COND(FAC_DB,SEV_ERROR,7)
#define	HIS_READERROR		FORM_COND(FAC_DB,SEV_ERROR,8)
#define	HIS_NOMEMORY		FORM_COND(FAC_DB,SEV_ERROR,9)
#define	HIS_TOOMANYPATS		FORM_COND(FAC_DB,SEV_ERROR,10)
#define	HIS_IDNOTHERE		FORM_COND(FAC_DB,SEV_ERROR,11)
#define HIS_NOTOPENED		FORM_COND(FAC_DB,SEV_ERROR,12)
#define HIS_BADPATIENT		FORM_COND(FAC_DB,SEV_ERROR,13)
#define HIS_TOOMANYSTUDS	FORM_COND(FAC_DB,SEV_ERROR,14)
#define HIS_BADSTUDY		FORM_COND(FAC_DB,SEV_ERROR,15)
#define HIS_NOPATIENTS		FORM_COND(FAC_DB,SEV_ERROR,16)
#define HIS_NOMORE		FORM_COND(FAC_DB,SEV_ERROR,17)
#define HIS_NOSTUDIES		FORM_COND(FAC_DB,SEV_ERROR,18)
#define HIS_EXISTS		FORM_COND(FAC_DB,SEV_ERROR,19)
#define HIS_LOCKERROR		FORM_COND(FAC_DB,SEV_ERROR,20)
#define HIS_DUPIMAGE		FORM_COND(FAC_DB,SEV_ERROR,21)
#define HIS_NOFREENODES 	FORM_COND(FAC_DB,SEV_ERROR,22)
#define HIS_BADSEQCOUNT 	FORM_COND(FAC_DB,SEV_ERROR,23)
#define HIS_TOOMANYVISITS 	FORM_COND(FAC_DB,SEV_ERROR,24)
#define HIS_BADVISIT 		FORM_COND(FAC_DB,SEV_ERROR,25)
#define HIS_TOOMANYINTERPS 	FORM_COND(FAC_DB,SEV_ERROR,26)
#define HIS_BADRESULT 		FORM_COND(FAC_DB,SEV_ERROR,27)
#define HIS_TOOMANYRESULTS 	FORM_COND(FAC_DB,SEV_ERROR,28)
#define HIS_PATNOTFOUND 	FORM_COND(FAC_DB,SEV_ERROR,29)
#define HIS_BADCONTEXT 		FORM_COND(FAC_DB,SEV_ERROR,30)
#define HIS_NOVISITS 		FORM_COND(FAC_DB,SEV_ERROR,31)
#define HIS_VISITNOTFOUND 	FORM_COND(FAC_DB,SEV_ERROR,32)
#define HIS_STUDYNOTFOUND 	FORM_COND(FAC_DB,SEV_ERROR,33)
#define HIS_NORESULTS 		FORM_COND(FAC_DB,SEV_ERROR,34)
#define HIS_RESULTSNOTFOUND 	FORM_COND(FAC_DB,SEV_ERROR,35)
#define HIS_NOINTERPS 		FORM_COND(FAC_DB,SEV_ERROR,36)
#define HIS_INTERPSNOTFOUND 	FORM_COND(FAC_DB,SEV_ERROR,37)
#define HIS_BADSEQUENCECOUNT	FORM_COND(FAC_DB,SEV_ERROR,38)

#ifdef  __cplusplus
}
#endif

#endif
