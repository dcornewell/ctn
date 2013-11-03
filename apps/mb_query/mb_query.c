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
**				MB CTN 1999
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	main
**			usageerror
**			myExit
**			mwlCallback
** Author, Date:	Stephen M. Moore, August 9, 1999
** Intent:		This program sends DICOM query requests to a
**			DICOM image server.  These are multi-byte requests.
**			the verification service class (ECHO command).
**  Usage:
**	mb_query [-a title] [-c title] [-f file] [-l level] [-p] [-v] node port
**  Options:
**	a	Application title of this application
**	c	Called AP title to use during Association setup
**	f	File containing a dcm object with a query specification
**	p	Dump service parameters after Association Request
**	v	Verbose mode for DUL facility
** Last Update:		$Author: smm $, $Date: 1999-12-09 19:55:35 $
** Source File:		$RCSfile: mb_query.c,v $
** Revision:		$Revision: 1.1 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.1 $ $RCSfile: mb_query.c,v $";

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
#include <io.h>
#include <winsock.h>
#else
#include <sys/file.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>

#include "dicom.h"
#include "ctnthread.h"
#include "dicom_uids.h"
#include "lst.h"
#include "condition.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"

typedef struct {
  void* reserved[2];
  DCM_OBJECT* obj;
  LST_HEAD* lst;
} ITEM;

static void usageerror();
static void myExit(DUL_ASSOCIATIONKEY ** association);

static char* getString(DCM_OBJECT* obj, DCM_TAG tag)
{
  DCM_ELEMENT e;
  CONDITION cond;

  memset(&e, 0, sizeof(e));
  e.tag = tag;
  cond = DCM_GetElement(&obj, tag, &e);
  if (cond != DCM_NORMAL)
    return 0;

  e.d.string = malloc(e.length + 1);
  cond = DCM_ParseObject(&obj, &e, 1, 0, 0, 0);
  if (cond != DCM_NORMAL) {
    COND_DumpConditions();
    exit(1);
  }

  return e.d.string;
}

static void  addString(DCM_OBJECT* obj, DCM_TAG tag, char* s)
{
  DCM_ELEMENT e;
  CONDITION cond;

  if (s == 0)
    return;

  memset(&e, 0, sizeof(e));
  e.tag = tag;

  cond = DCM_LookupElement(&e);
  if (cond != DCM_NORMAL) {
    COND_DumpConditions();
    exit(1);
  }

  e.d.string = s;
  e.length = strlen(s);
  cond = DCM_ModifyElements(&obj, &e, 1, 0, 0, 0);
  if (cond != DCM_NORMAL) {
    COND_DumpConditions();
    exit(1);
  }
}

static DCM_OBJECT *
createQueryObject(const char *fileName)
{
    DCM_OBJECT *obj;
    CONDITION cond;
    DCM_ELEMENT e;
    char txt[1024] = "";

    int group = 0;
    int element = 0;
    char textValue[1024];

    if (fileName != NULL) {
	cond = DCM_OpenFile(fileName, DCM_ORDERLITTLEENDIAN, &obj);
	if (cond != DCM_NORMAL) {
	    COND_DumpConditions();
	    exit(1);
	}
	return obj;
    }
    cond = DCM_CreateObject(&obj, 0);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
    while (fgets(txt, sizeof(txt), stdin) != NULL) {
	if (txt[0] == '#' || txt[0] == '\0')
	    continue;
	if (txt[0] == '\n' || txt[0] == '\r')
	    continue;

	if (sscanf(txt, "%x %x %[^\n]", &group, &element, textValue) != 3)
	    continue;

	e.tag = DCM_MAKETAG(group, element);
	DCM_LookupElement(&e);
	if (strncmp(textValue, "#", 1) == 0) {
	    e.length = 0;
	    e.d.string = NULL;
	} else {
	    e.length = strlen(textValue);
	    e.d.string = textValue;
	}

	cond = DCM_AddElement(&obj, &e);
	if (cond != DCM_NORMAL) {
	    COND_DumpConditions();
	    exit(1);
	}
    }

    return obj;
}

static CONDITION
queryCallback(MSG_C_FIND_REQ * request, MSG_C_FIND_RESP * response,
	    int responseCount, char *SOPClass, char *queryLevel, void *p)
{
  ITEM* item;
  LST_HEAD* l;

  if (response->status == MSG_K_SUCCESS)
    return SRV_NORMAL;

  MSG_DumpMessage(response, stdout);

  if (p == 0)
    return SRV_NORMAL;

  item = calloc(1, sizeof(*item));
  if (item == 0) {
    fprintf(stderr, "Could not allocate %d bytes\n", sizeof(*item));
    exit(1);
  }

  DCM_CopyObject(&response->identifier, &item->obj);
  l = (LST_HEAD*)p;
  LST_Enqueue(&l, item);

  return SRV_NORMAL;
}


static void handleStudyQueries(DUL_ASSOCIATIONKEY** association,
			       DUL_ASSOCIATESERVICEPARAMETERS* params,
			       MSG_C_FIND_REQ* patientRequest,
			       LST_HEAD* patientList,
			       const char* queryLevel)
{
  ITEM* item;
  CONDITION cond;
  MSG_C_FIND_RESP response;

  item = (ITEM*)LST_Head(&patientList);
  (void)LST_Position(&patientList, item);
  while (item != 0) {
    char* patientName;
    char* patientID;
    char* charSet;
    MSG_C_FIND_REQ studyRequest;
    DCM_OBJECT* q;

    patientName = getString(item->obj, DCM_PATNAME);
    patientID = getString(item->obj, DCM_PATID);
    charSet = getString(patientRequest->identifier, DCM_IDSPECIFICCHARACTER);

    printf("%s %s\n", patientID, patientName);

    studyRequest = *patientRequest;
    studyRequest.identifier = 0;
    DCM_CreateObject(&q, 0);

    addString(q, DCM_PATID, patientID);
    addString(q, DCM_IDSPECIFICCHARACTER, charSet);
    addString(q, DCM_IDQUERYLEVEL, "STUDY");
    addString(q, DCM_RELSTUDYINSTANCEUID, "");
    addString(q, DCM_IDSTUDYDATE, "");
    addString(q, DCM_IDSTUDYTIME, "");
    addString(q, DCM_IDACCESSIONNUMBER, "");
    addString(q, DCM_IDSTUDYDESCRIPTION, "");

    studyRequest.identifier = q;
    studyRequest.messageID = SRV_MessageIDOut();

    item->lst = LST_Create();

    cond = SRV_CFindRequest(association, params,
			    &studyRequest, &response,
			    queryCallback, item->lst, "");
    if (!(CTN_SUCCESS(cond))) {
      (void) printf("Verification Request unsuccessful\n");
      COND_DumpConditions();
    } else {

    }

    item = LST_Next(&patientList);
  }
}

static void handleSeriesQueries(DUL_ASSOCIATIONKEY** association,
				DUL_ASSOCIATESERVICEPARAMETERS* params,
				MSG_C_FIND_REQ* patientRequest,
				LST_HEAD* patientList,
				const char* queryLevel)
{
  ITEM* item;
  ITEM* studyItem;
  CONDITION cond;
  MSG_C_FIND_RESP response;

  item = (ITEM*)LST_Head(&patientList);
  (void)LST_Position(&patientList, item);
  while (item != 0) {
    char* patientName;
    char* patientID;
    char* studyInstanceUID;
    char* charSet;

    patientName = getString(item->obj, DCM_PATNAME);
    patientID = getString(item->obj, DCM_PATID);
    charSet = getString(patientRequest->identifier, DCM_IDSPECIFICCHARACTER);

    printf("%s %s\n", patientID, patientName);

    studyItem = (ITEM*)LST_Head(&item->lst);
    (void)LST_Position(&item->lst, studyItem);
    while (studyItem != NULL) {
      DCM_OBJECT* q;
      MSG_C_FIND_REQ seriesRequest;

      seriesRequest = *patientRequest;
      seriesRequest.identifier = 0;
      DCM_CreateObject(&q, 0);

      studyInstanceUID = getString(studyItem->obj, DCM_RELSTUDYINSTANCEUID);

      addString(q, DCM_PATID, patientID);
      addString(q, DCM_IDSPECIFICCHARACTER, charSet);
      addString(q, DCM_IDQUERYLEVEL, "SERIES");
      addString(q, DCM_RELSTUDYINSTANCEUID, studyInstanceUID);
      addString(q, DCM_IDMODALITY, "");
      addString(q, DCM_RELSERIESNUMBER, "");
      addString(q, DCM_ACQBODYPARTEXAMINED, "");
      addString(q, DCM_ACQVIEWPOSITION, "");

      seriesRequest.identifier = q;
      seriesRequest.messageID = SRV_MessageIDOut();

      studyItem->lst = LST_Create();

      cond = SRV_CFindRequest(association, params,
			      &seriesRequest, &response,
			      queryCallback, studyItem->lst, "");
      if (!(CTN_SUCCESS(cond))) {
	(void) printf("C-Find Request unsuccessful\n");
	COND_DumpConditions();
      } else {
	
      }

      studyItem = LST_Next(&item->lst);
    }
    item = LST_Next(&patientList);
  }
}

static void printSeriesResults(LST_HEAD* lst)
{
  ITEM* item;

  if (lst == 0)
    return;

  item = (ITEM*)LST_Head(&lst);
  (void)LST_Position(&lst, item);
  while (item != 0) {
    char* s;
    DCM_TAG tags[] = { DCM_IDMODALITY, DCM_RELSERIESNUMBER,
		       DCM_ACQBODYPARTEXAMINED, DCM_ACQVIEWPOSITION
    };

    int i;

    for (i = 0; i < (int)DIM_OF(tags); i++) {
      s = getString(item->obj, tags[i]);
      if (s == 0) {
	printf("  %08x %s\n", tags[i], "");
      } else {
	printf("  %08x %s\n", tags[i], s);
	free(s);
      }
    }
    printf("\n");

    item = (ITEM*)LST_Next(&lst);
  }
}

static void printStudyResults(LST_HEAD* studyList)
{
  ITEM* studyItem;

  if (studyList == 0)
    return;

  studyItem = (ITEM*)LST_Head(&studyList);
  (void)LST_Position(&studyList, studyItem);
  while (studyItem != 0) {
    char* s;
    DCM_TAG tags[] = { DCM_RELSTUDYINSTANCEUID, DCM_IDSTUDYDATE,
		       DCM_IDSTUDYTIME, DCM_IDACCESSIONNUMBER,
		       DCM_IDSTUDYDESCRIPTION
    };

    int i;

    for (i = 0; i < (int)DIM_OF(tags); i++) {
      s = getString(studyItem->obj, tags[i]);
      if (s == 0) {
	printf(" %08x %s\n", tags[i], "");
      } else {
	printf(" %08x %s\n", tags[i], s);
	free(s);
      }
    }
    printSeriesResults(studyItem->lst);
    printf("\n");

    studyItem = (ITEM*)LST_Next(&studyList);
  }
}

static void printResults(LST_HEAD* patientList)
{
  ITEM* patientItem;

  patientItem = (ITEM*)LST_Head(&patientList);
  (void)LST_Position(&patientList, patientItem);
  while (patientItem != 0) {
    char* s;
    DCM_TAG pTags[] = { DCM_PATNAME, DCM_PATID, DCM_PATBIRTHDATE };
    int i;

    for (i = 0; i < (int)DIM_OF(pTags); i++) {
      s = getString(patientItem->obj, pTags[i]);
      if (s == 0) {
	printf("%08x %s\n", pTags[i], "");
      } else {
	printf("%08x %s\n", pTags[i], s);
	free(s);
      }
    }
    printStudyResults(patientItem->lst);
    printf("\n");

    patientItem = (ITEM*)LST_Next(&patientList);
  }
}

main(int argc, char **argv)
{

  CONDITION cond;

  DUL_NETWORKKEY* network = NULL;
  DUL_ASSOCIATIONKEY* association = NULL;
  DUL_ASSOCIATESERVICEPARAMETERS params;
  char
    *calledAPTitle = "QUERY_SCP",
    *callingAPTitle = "QUERY_SCU";

  char localHost[40];

  int port;			/* TCP port used to establish Association */
  char* node;			/* The node we are calling */

  CTNBOOLEAN
    verbose = FALSE,
    abortFlag = FALSE,
    paramsFlag = FALSE;
  char *fileName = NULL;
  DUL_SC_ROLE role = DUL_SC_ROLE_DEFAULT;
  MSG_C_FIND_RESP response;
  MSG_C_FIND_REQ findRequest = {MSG_K_C_FIND_REQ, 0, 0,
				DCM_CMDDATAIDENTIFIER,
				0, NULL, DICOM_SOPPATIENTQUERY_FIND};
  char* queryLevel = "patient";
  LST_HEAD* patientList = 0;
  LST_HEAD* studyList = 0;

  while (--argc > 0 && (*++argv)[0] == '-') {
    switch (*(argv[0] + 1)) {
    case 'a':
      argc--;
      argv++;
      if (argc <= 0)
	usageerror();
      callingAPTitle = *argv;
      break;
    case 'c':
      argc--;
      argv++;
      if (argc <= 0)
	usageerror();
      calledAPTitle = *argv;
      break;
    case 'f':
      argc--;
      argv++;
      if (argc <= 0)
	usageerror();
      fileName = *argv;
      break;
    case 'l':
      argc--;
      argv++;
      if (argc <= 0)
	usageerror();
      queryLevel = *argv;
      break;
    case 'p':
      paramsFlag = TRUE;
      break;
    case 'v':
      verbose = TRUE;
      break;
    default:
      break;
    }
  }

  if (argc < 2)
    usageerror();

  THR_Init();
  DUL_Debug(verbose);
  SRV_Debug(verbose);
  
  node = *argv;
  if (sscanf(*++argv, "%d", &port) != 1)
    usageerror();
  (void) gethostname(localHost, sizeof(localHost));

  cond = DUL_InitializeNetwork(DUL_NETWORK_TCP, DUL_AEREQUESTOR,
			       NULL, 10, DUL_ORDERBIGENDIAN, &network);
  if (cond != DUL_NORMAL)
    myExit(&association);

  DUL_DefaultServiceParameters(&params);
  sprintf(params.calledPresentationAddress, "%s:%s", node, *argv);
  strcpy(params.callingPresentationAddress, localHost);
  strcpy(params.calledAPTitle, calledAPTitle);
  strcpy(params.callingAPTitle, callingAPTitle);
  cond = SRV_RequestServiceClass(DICOM_SOPPATIENTQUERY_FIND, role, &params);
  if (cond != SRV_NORMAL) {
    COND_DumpConditions();
    THR_Shutdown();
    exit(1);
  }
  cond = DUL_RequestAssociation(&network, &params, &association);
  if (cond != DUL_NORMAL) {
    if (cond == DUL_ASSOCIATIONREJECTED) {
      fprintf(stderr, "Association Rejected\n");
      fprintf(stderr, " Result: %2x Source %2x Reason %2x\n",
	      params.result, params.resultSource,
	      params.diagnostic);
    }
    myExit(&association);
  }
  if (verbose || paramsFlag) {
    (void) printf("Association accepted, parameters:\n");
    DUL_DumpParams(&params);
  }
  findRequest.messageID = SRV_MessageIDOut();
  findRequest.identifier = createQueryObject(fileName);


  patientList = LST_Create();
  studyList = LST_Create();

  cond = SRV_CFindRequest(&association, &params,
			  &findRequest, &response,
			  queryCallback, patientList, "");
  if (!(CTN_SUCCESS(cond))) {
    (void) printf("Verification Request unsuccessful\n");
    COND_DumpConditions();
  } else {
    MSG_DumpMessage(&response, stdout);
  }
  SRV_MessageIDIn(findRequest.messageID);

  if (strcmp(queryLevel, "patient") != 0) {
    handleStudyQueries(&association, &params, &findRequest,
		       patientList, queryLevel);
  }

  if (strcmp(queryLevel, "series") == 0) {
    handleSeriesQueries(&association, &params, &findRequest,
			patientList, queryLevel);
  }

  cond = DUL_ReleaseAssociation(&association);
  if (cond != DUL_RELEASECONFIRMED) {
    (void) fprintf(stderr, "%x\n", cond);
    COND_DumpConditions();
  }
  (void) DUL_ClearServiceParameters(&params);

  printResults(patientList);

  (void) DUL_DropNetwork(&network);
  THR_Shutdown();
  return 0;
}


/* usageerror
**
** Purpose:
**	Print the usage message for this program and exit.
**
** Parameter Dictionary:
**	None
**
** Return Values:
**	None
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static void
usageerror()
{
    char msg[] = "\
mb_query [-a title] [-c title] [-f file] [-p] [-v] node port\n\
\n\
    a     Application title of this application\n\
    c     Called AP title to use during Association setup\n\
    f     File containing a dcm object with query\n\
    p     Dump service parameters after Association Request\n\
    v     Verbose mode for DUL/SRV facilities\n\
\n\
    node  Node name of server\n\
    port  Port number of server\n";

    fprintf(stderr, msg);
    exit(1);
}


/* myExit
**
** Purpose:
**	Exit routines which closes network connections, dumps error
**	messages and exits.
**
** Parameter Dictionary:
**	association	A handle for an association which is possibly open.
**
** Return Values:
**	None
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static void
myExit(DUL_ASSOCIATIONKEY ** association)
{
    fprintf(stderr, "Abnormal exit\n");
    COND_DumpConditions();

    if (association != NULL)
	if (*association != NULL)
	    (void) DUL_DropAssociation(association);
    THR_Shutdown();
    exit(1);
}
