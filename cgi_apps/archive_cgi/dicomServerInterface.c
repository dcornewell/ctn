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
+-+-+-+-+-+-+-+-+-
*/
/*
**				DICOM 93
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:
** Intent:
** Last Update:		$Author: smm $, $Date: 1999-11-03 16:56:05 $
** Source File:		$Source: /home/smm/ctn/ctn/cgi_apps/archive_cgi/dicomServerInterface.c,v $
** Revision:		$Revision: 1.1 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.1 $ $RCSfile: dicomServerInterface.c,v $";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dicom.h"
#include "dicom_uids.h"
#include "lst.h"
#include "condition.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "manage.h"

#include "dicomServerInterface.h"

/* These functions are private to this module */

static CONDITION moveCallback(MSG_C_MOVE_REQ *moveReq,
			MSG_C_MOVE_RESP *moveResp, int responseCount,
			char *abstractSyntax, char *queryLevel, void *ctx)
{
  return SRV_NORMAL;
}

CONDITION
connectToDICOMServer(const char *srcTitle,
			const char *dstTitle,
			DMAN_HANDLE **handle,
			DUL_NETWORKKEY **network,
			DUL_ASSOCIATIONKEY ** assoc,
			DUL_ASSOCIATESERVICEPARAMETERS * params,
			const char *SOPClass)
{
  CONDITION cond;
  DMAN_APPLICATIONENTITY ae;
  char localHost[1024];

  DUL_DefaultServiceParameters(params);

  memset(&ae, 0, sizeof(ae));
  ae.Type = DMAN_K_APPLICATIONENTITY;

  cond = DMAN_LookupApplication(handle, dstTitle, &ae);

  cond = DUL_InitializeNetwork(DUL_NETWORK_TCP, DUL_AEREQUESTOR,
			   NULL, DUL_TIMEOUT, DUL_ORDERBIGENDIAN, network);
  if (cond != DUL_NORMAL)
    return 0;

  (void) gethostname(localHost, sizeof(localHost));

  sprintf(params->calledPresentationAddress, "%s:%d", localHost, 2100);
  strcpy(params->callingPresentationAddress, localHost);
  strcpy(params->calledAPTitle, srcTitle);
  strcpy(params->callingAPTitle, srcTitle);

  cond = SRV_RequestServiceClass(SOPClass, DUL_SC_ROLE_SCU, params);
  if (cond != SRV_NORMAL) {
    COND_DumpConditions();
    return 0;
  }

  cond = DUL_RequestAssociation(network, params, assoc);
  if (cond != DUL_NORMAL) {
    if (cond == DUL_ASSOCIATIONREJECTED) {
    fprintf(stdout, "Association Rejected\n");
    fprintf(stdout, " Result: %2x Source %2x Reason %2x\n",
		 params->result, params->resultSource,
		 params->diagnostic);
    }
  return 0;
  }

  return 1;
}

CONDITION
releaseDICOMServer(DUL_NETWORKKEY **network, DUL_ASSOCIATIONKEY *assoc)
{
  (void) DUL_ReleaseAssociation(assoc);
  (void) DUL_DropAssociation(assoc);
  (void) DUL_DropNetwork(network);
  return 1;
}

CONDITION
sendMoveRequest(DUL_ASSOCIATIONKEY ** association,
          DUL_ASSOCIATESERVICEPARAMETERS * params,
	  const char *destinationAPTitle,
          DCM_OBJECT *moveObject, long *successCount, long *warningCount,
          long *failureCount)
{
  MSG_C_MOVE_REQ moveRequest;
  MSG_C_MOVE_RESP moveResponse;
  CONDITION cond;

  *successCount = 0;
  *warningCount = 0;
  *failureCount = 0;

  memset(&moveRequest, 0, sizeof(moveRequest));
  memset(&moveResponse, 0, sizeof(moveResponse));
  moveRequest.type = MSG_K_C_MOVE_REQ;
  moveRequest.dataSetType = DCM_CMDDATAIDENTIFIER;
  moveRequest.messageID = SRV_MessageIDOut();
  moveRequest.priority = 0;
  strcpy(moveRequest.classUID, DICOM_SOPSTUDYQUERY_MOVE);
  strcpy(moveRequest.moveDestination, destinationAPTitle);
  moveRequest.identifier = moveObject;

  cond = SRV_CMoveRequest(association, params, &moveRequest, &moveResponse,
			  moveCallback, NULL, "");

  *successCount = moveResponse.completedSubOperations;
  *warningCount = moveResponse.warningSubOperations;
  *failureCount = moveResponse.failedSubOperations;

  return 1;
}
