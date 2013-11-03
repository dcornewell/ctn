/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	netWork
**			request_association
**			request_release
**                      errorstackP
**                      copyWtext
**                      closeNetworkConnection
**
** Author, Date:	Chander L. Sabharwal, 9-September-94
**
** Intenet:		This program initializes network environment,
**			builds a presentation context to be transmitted
**			to an Association acceptor and requests an
**			Association with another node.
**			It also informs the user of any errors occuring
**			in this Association.
**
**
** Last Update:		$Author: smm $, $Date: 2002-10-31 23:17:53 $
** Source File:		$Source: /home/smm/ctn/ctn/apps/assoc_tool/netWork.c,v $
** Revision:		$Revision: 1.6 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.6 $ $RCSfile: netWork.c,v $";

#include "ctn_os.h"

#include "dicom.h"
#include "dicom_uids.h"
#include "lst.h"
#include "condition.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"

#include "structures.h"
#include "prototypes.h"


#define   MAXLEN  5000

extern LST_HEAD *sopList;
extern LST_HEAD *selectedsopList;

extern SELECTED_SOP *se;


static DUL_NETWORKKEY		/* Used to initialize our network */
*   network = NULL;

static DUL_ASSOCIATIONKEY	/* Describes the Association with the
				 * Acceptor */
*   association = NULL;

static DUL_ASSOCIATESERVICEPARAMETERS	/* The items which describe this
					 * Association */
    params =
{
    DICOM_STDAPPLICATIONCONTEXT, "DICOM_TEST", "DICOM_VERIFY",
    "", 16384, 0, 0, 0,
    "calling presentation addr", "called presentation addr",
    NULL, NULL, 0, 0,
    MIR_IMPLEMENTATIONCLASSUID, MIR_IMPLEMENTATIONVERSIONNAME,
    "", "", 0
};


static char *info;		/* character string contains error code and
				 * error message */

/* request_association
**
** Purpose:
**	This program initializes network environment,
**	builds a presentation context to be transmitted
**      to an Association acceptor and requests an
**      Association with another node.
**	It also informs the user of any errors occuring
**	in this Association.
**
**
** Parameter Dictionary:
**	node		input node we are calling
**	port		input TCP port to establish association
**	calledAPTitle	input called application title
**	callingAPTitle	input caliing application title
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

void
request_association(char node[], char port[], char calledAPTitle[],
		    char callingAPTitle[])
{

    unsigned long i;

    CONDITION			/* Return values from DUL and ACR routines */
	cond;

    static CTNBOOLEAN
        networkInitialized = FALSE;

    static CTNBOOLEAN
        association_on = FALSE;

    char
        localHost[40];


    DUL_PRESENTATIONCONTEXT	/* contains DUL presentation context */
	* context;

    UID_DESCRIPTION		/* pointer to DUL presentation context
				 * description */
	d;

    unsigned short ctxresult;	/* character containing the code for
				 * acceptance or rejection of sop class */

    char *buf1,
       *buf2,
       *bufsyn;

    CTNBOOLEAN
	clearStack = TRUE;

    unsigned long numSop;	/* number of SOPCLASSES in the linkedList */


    static DUL_PRESENTATIONCONTEXTID	/* contains DUL presentation context
					 * ID */
        contextID = 1;

    CTNBOOLEAN			/* Function to obtain error messages from the
				 * stack */
	errorstackP(unsigned long v, char *mes);

    char *buffer,
       *buff;



    info = calloc(MAXLEN, sizeof(char));
    buf1 = calloc(100, sizeof(char));
    buf2 = calloc(100, sizeof(char));
    bufsyn = calloc(100, sizeof(char));

    memset(buf1, 0, sizeof(buf1));
    memset(buf2, 0, sizeof(buf2));
    memset(bufsyn, 0, sizeof(buf2));
    memset(info, 0, sizeof(info));
    copyWtext2(info);

    (void) DUL_ClearServiceParameters(&params);

    (void) gethostname(localHost, sizeof(localHost));

    if (!networkInitialized) {
	networkInitialized = TRUE;
	cond = DUL_InitializeNetwork(DUL_NETWORK_TCP, DUL_AEREQUESTOR,
				     NULL, 10, DUL_ORDERBIGENDIAN, &network);
	if (cond != DUL_NORMAL) {
	    networkInitialized = FALSE;
	    (void) COND_ExtractConditions(errorstackP);
	    copyWtext2(info);
	    (void) COND_PopCondition(clearStack);
	}
    }
    if (association_on) {
	request_release();
    }
    strcpy(params.calledPresentationAddress, node);
    strcat(params.calledPresentationAddress, ":");
    strcat(params.calledPresentationAddress, port);
    strcpy(params.callingPresentationAddress, localHost);
    strcpy(params.calledAPTitle, calledAPTitle);
    strcpy(params.callingAPTitle, callingAPTitle);

    numSop = LST_Count(&selectedsopList);

    if (numSop == 0) {
	strcpy(info, "PLEASE  SELECT  SERVICE  CLASS  AS  AN ASSOCIATION INITIATOR FROM SCROLLEDLIST 2 \n");
	copyWtext2(info);
    } else {


	if (params.requestedPresentationContext == NULL) {
	    params.requestedPresentationContext = LST_Create();
	    if (params.requestedPresentationContext == NULL) {
		return;
	    }
	}
	se = LST_Head(&selectedsopList);
	(void) LST_Position(&selectedsopList, se);

	for (i = 1; i <= numSop; i++) {
	    context = (DUL_PRESENTATIONCONTEXT *) malloc(sizeof(*context));
	    if (context == NULL) {
		printf(" Error in malloc context\n");
		return;
	    } else {
		cond = DUL_MakePresentationCtx(&context, se->role, se->role,
					       contextID, 0,
					       se->selectedsopclassW, "",
					       se->tSyntaxes[0],
					       se->tSyntaxes[1],
					       se->tSyntaxes[2],
					       NULL);

		if (cond != DUL_NORMAL) {
		    (void) COND_ExtractConditions(errorstackP);
		    copyWtext2(info);
		    (void) COND_PopCondition(clearStack);
		} else {

		    cond = LST_Enqueue(&params.requestedPresentationContext, context);
		    if (cond != LST_NORMAL)
			(void) COND_ExtractConditions(errorstackP);
		    copyWtext2(info);
		    (void) COND_PopCondition(clearStack);
		    contextID += 2;
		}
	    }
	    se = LST_Next(&selectedsopList);
	}


	cond = DUL_RequestAssociation(&network, &params, &association);
	association_on = TRUE;
	if (cond != DUL_NORMAL) {

	    if (cond == DUL_ASSOCIATIONREJECTED) {
		buffer = calloc(100, sizeof(char));
		memset(buffer, 0, sizeof(buffer));
		buff = calloc(20, sizeof(char));
		memset(buff, 0, sizeof(buff));

		strcat(buffer, "  Result : ");
		sprintf(buff, "%2x", params.result);
		strcat(buffer, buff);
		strcat(buffer, ",  resultSource : ");
		sprintf(buff, "%2x", params.resultSource);
		strcat(buffer, buff);
		strcat(buffer, ",  diagnostic : ");
		sprintf(buff, "%2x", params.diagnostic);
		strcat(buffer, buff);

		strcat(info, buffer);
		strcat(info, "\n");
		copyWtext2(info);

		free(buffer);
		free(buff);

	    }
	    association_on = FALSE;
	    (void) COND_ExtractConditions(errorstackP);
	    copyWtext2(info);
	    (void) COND_PopCondition(clearStack);

	} else {
	    context = LST_Head(&params.acceptedPresentationContext);

	    if (context != NULL) {
		context = LST_Position(&params.acceptedPresentationContext, context);
	    }
	    while (context != NULL) {
		ctxresult = (int) context->result;

		cond = UID_Lookup(context->acceptedTransferSyntax, &d);
		sprintf(bufsyn, "%30s", d.description);

		sprintf(buf1, "%30s", context->abstractSyntax);

		cond = UID_Lookup(context->abstractSyntax, &d);
		if (cond = UID_NORMAL) {
		    sprintf(buf2, "%s,%s ", d.description, d.originator);
		}
		context = LST_Next(&params.acceptedPresentationContext);

		strcat(info, buf1);
		strcat(info, "      ");
		strcat(info, buf2);
		strcat(info, "      ");
		strcat(info, bufsyn);


/**                buffer = calloc(100, sizeof(char));
		memset(buffer, 0, sizeof(buffer));
                buff = calloc(20, sizeof(char));
		memset(buff, 0, sizeof(buff));**/


		switch (ctxresult) {
		case 0:
		    strcat(info, "      (ACCEPTED)\n");
		    copyWtext2(info);
		    break;
		case 1:
		    strcat(info, "      (REJECTED_USER)\n");
/**	            strcat(buffer,"  Result : ");
                    sprintf(buff, "%2x", params.result);
	            strcat(buffer,buff);
	            strcat(buffer,",  resultSource : ");
                    sprintf(buff, "%2x", params.resultSource);
	            strcat(buffer,buff);
	            strcat(buffer,",  diagnostic : ");
                    sprintf(buff, "%2x", params.diagnostic);
	            strcat(buffer,buff);

                    strcat(info,buffer);
		    strcat(info,"\n");**/

		    copyWtext2(info);
		    break;
		case 2:
		    strcat(info, "      (REJECTED_NOREASON)\n");
/**	            strcat(buffer,"  Result : ");
                    sprintf(buff, "%2x", params.result);
	            strcat(buffer,buff);
	            strcat(buffer,",  resultSource : ");
                    sprintf(buff, "%2x", params.resultSource);
	            strcat(buffer,buff);
	            strcat(buffer,",  diagnostic : ");
                    sprintf(buff, "%2x", params.diagnostic);
	            strcat(buffer,buff);

                    strcat(info,buffer);
		    strcat(info,"\n");**/

		    copyWtext2(info);
		    break;
		case 3:
		    strcat(info, "      (REJECTED_ABSTRACT_SYNTAX)\n");
/**	            strcat(buffer,"  Result : ");
                    sprintf(buff, "%2x", params.result);
	            strcat(buffer,buff);
	            strcat(buffer,",  resultSource : ");
                    sprintf(buff, "%2x", params.resultSource);
	            strcat(buffer,buff);
	            strcat(buffer,",  diagnostic : ");
                    sprintf(buff, "%2x", params.diagnostic);
	            strcat(buffer,buff);

                    strcat(info,buffer);
		    strcat(info,"\n");**/
		    copyWtext2(info);
		    break;
		case 4:
		    strcat(info, "      (REJECTED_TRANSFER_SYNTAX)\n");
/**	            strcat(buffer,"  Result : ");
                    sprintf(buff, "%2x", params.result);
	            strcat(buffer,buff);
	            strcat(buffer,",  resultSource : ");
                    sprintf(buff, "%2x", params.resultSource);
	            strcat(buffer,buff);
	            strcat(buffer,",  diagnostic : ");
                    sprintf(buff, "%2x", params.diagnostic);
	            strcat(buffer,buff);

                    strcat(info,buffer);
                    strcat(info,"\n");**/
		    copyWtext2(info);
		    break;
		default:
		    break;
		}
/**		free(buffer);
		free(buff);**/
	    }
	}
    }
}


/* errorstackP
**
** Purpose:
**	This function is called for each condition on the stack.
**
** Parameter Dictionary:
**	v		condition value, identifies the facility which
**			generates error, severity of the error and the
**			actual error itself.
**	mes		character string, associated condition message
**                      which identifies the nature of the error.
**
** Return Values:
**      TRUE            to extract all the messages
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/


CTNBOOLEAN
errorstackP(unsigned long v, char *mes)
{

    char *buff;

    buff = calloc(20, sizeof(char));
    memset(buff, 0, sizeof(buff));

    sprintf(buff, "%8x", v);

    strcat(info, buff);
    strcat(info, "     ");
    if (CTN_ERROR(v))
	strcat(info, "  ERROR  ");
    if (CTN_WARNING(v))
	strcat(info, "  WARNING  ");
    if (CTN_FATAL(v))
	strcat(info, "  FATAL  ");
    if (CTN_INFORM(v))
	strcat(info, "  INFORMATION  ");

/*   strncat(info, mes, strlen(mes));*/
    strcat(info, mes);
    strcat(info, "\n");
    free(buff);
    return (TRUE);
}

/* request_release
**
** Purpose:
**	This subroutine requests an orderly release or abort of an
**	Association with a server. If fails to do so, then drops
**	an Association without notifying the peer application and
**	destroys the caller's reference to the Association.
**
** Parameter Dictionary:
**	None
**
** Return Values:
**      None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

void
request_release()
{
    CONDITION			/* Return values from DUL and ACR routines */
    cond = 0;


    (void) DUL_ClearServiceParameters(&params);

    if (association)
	cond = DUL_ReleaseAssociation(&association);
    if (cond != DUL_RELEASECONFIRMED) {
	cond = DUL_AbortAssociation(&association);
    }
    (void) DUL_DropAssociation(&association);
    strcpy(info, " Association has been released \n");
    copyWtext2(info);
    memset(info, 0, sizeof(info));
}

/*closeNetworkConnection
**
**
** Purpose:
**	This subroutine closes network connection with the server.
** Parameter Dictionary:
**	None
**
** Return Values:
**      None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

void
closeNetworkConnection()
{
    (void) DUL_ClearServiceParameters(&params);

    if (sopList != NULL)
	(void) LST_Destroy(&sopList);

    if (selectedsopList != NULL)
	(void) LST_Destroy(&selectedsopList);

    (void) DUL_DropNetwork(&network);

}
