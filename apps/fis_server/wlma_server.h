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
** Intent:		Define typedefs and function prototypes for
**			TBL facility (for handling table operations).
** Last Update:		$Author: smm $, $Date: 1994-09-15 17:49:42 $
** Source File:		$RCSfile: wlma_server.h,v $
** Revision:		$Revision: 1.1 $
** Status:		$State: Exp $
*/

#ifndef _APP_IS_IN
#define _APP_IS_IN 1

typedef struct {
    char callingAPTitle[DUL_LEN_TITLE + 1];
    char calledAPTitle[DUL_LEN_TITLE + 1];
    int pid;
}   PROCESS_ELEMENT;

#define	K_APPLICATION_ENTITY	0
#define	K_GROUP_NAMES		1
#define	K_DATABASE_ACCESS	2

#define	MAXIMUM_TABLES 10

#define	SIMPLE_STORAGE_AE_TITLE	"DICOM_STORAGE"
CONDITION
associationCheck(char *node, DUL_ASSOCIATESERVICEPARAMETERS * params,
       DMAN_HANDLE ** handle, BOOLEAN forgive, DUL_ABORTITEMS * abortItems);
char *APP_Message(CONDITION cond);
CONDITION
serviceRequests(DUL_NETWORKKEY ** network, DUL_ASSOCIATIONKEY ** association,
		DUL_ASSOCIATESERVICEPARAMETERS * service);
CONDITION
nextAssociationRequest(char *node, DUL_NETWORKKEY ** network,
	     DUL_ASSOCIATESERVICEPARAMETERS * service, unsigned long maxPDU,
		    BOOLEAN forgiveFlag, DUL_ASSOCIATIONKEY ** association);


#define	APP_NORMAL	FORM_COND(FAC_APP, SEV_SUCC, 1)
#define	APP_ILLEGALSERVICEPARAMETER	FORM_COND(FAC_APP, SEV_ERROR, 2)
#define	APP_MISSINGSERVICEPARAMETER	FORM_COND(FAC_APP, SEV_ERROR, 3)
#define	APP_PARAMETERWARNINGS		FORM_COND(FAC_APP, SEV_WARN,  4)
#define	APP_PARAMETERFAILURE		FORM_COND(FAC_APP, SEV_ERROR, 5)
#define	APP_FAILURE			FORM_COND(FAC_APP, SEV_ERROR, 6)
#define	APP_ASSOCIATIONRQFAILED		FORM_COND(FAC_APP, SEV_WARN, 7)

#define APP_ERROR(a) (a), APP_Message((a))

#endif
