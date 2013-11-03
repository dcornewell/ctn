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
** @$ = @$ = @$ =
*/
/*
**				DICOM 93
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	main
**			usageerror
**			myExit
**			sendImage
**			sendCallback
**			requestAssociation
** Author, Date:	Stephen M. Moore, 6-May-93
** Intent:		This program is an example and test program which
**			demonstrates the DICOM packages developed at MIR.
**			It establishes an Association with a server and sends
**			one or more images to the server.
**
** Last Update:		$Author: matt $, $Date: 2003-01-28 17:34:40 $
** Source File:		$RCSfile: spray_image.c,v $
** Revision:		$Revision: 1.4 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.4 $ $RCSfile: spray_image.c,v $";

#include "ctn_os.h"

#include "dicom.h"
#include "ctnthread.h"
#include "dicom_uids.h"
#include "lst.h"
#include "condition.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "utility.h"

#include <time.h>

#ifdef _MSC_VER
#include <iostream.h>
#include <winsock.h>
#else
#include <netdb.h>
#include <unistd.h>
#endif


static void usageerror();
static void myExit(DUL_ASSOCIATIONKEY ** association);
static void sprayImages(int argc, char **argv, DUL_NETWORKKEY ** network,
	     DUL_ASSOCIATESERVICEPARAMETERS * params, 
	     CTNBOOLEAN allowRepeatedElements, char** xferSyntaxes, 
         int xferSyntaxCount, int imageDelay, int studyDelay, int numStudies,
         char *patName);

static DCM_OBJECT** loadImageSet(int argc, char **argv, 
        CTNBOOLEAN allowRepeatedElements, int *numObjects, char *patName); 

static CONDITION sendImage(DUL_ASSOCIATIONKEY ** association,
	  DUL_ASSOCIATESERVICEPARAMETERS * params,
	  DCM_OBJECT ** object, char *SOPClass, char *instanceUID,
	  char *moveAETitle);

static void sendStudy(DUL_NETWORKKEY ** network, DUL_ASSOCIATESERVICEPARAMETERS * params, 
	     char** xferSyntaxes, int xferSyntaxCount,
             DCM_OBJECT** objects, int numObjects, int imageDelay);

static int requestAssociation(DUL_NETWORKKEY ** network,
		   DUL_ASSOCIATIONKEY ** association,
		   DUL_ASSOCIATESERVICEPARAMETERS * params,
		   char *SOPClass,
		   char** xferSyntaxes, int xferSyntaxCount);

static CONDITION sendCallback(MSG_C_STORE_REQ * request, MSG_C_STORE_RESP * response,
	     unsigned long transmitted, unsigned long total,
	     void *string);

static void doSleep(long msecs); 
int getHostIPAddress(char *ipaddr); 
long getTimemark();
int getUIDBase(char *uid, long timemark);
static void replaceStudyInstanceUID(DCM_OBJECT** iod, const char* studyUID);
static void replaceSeriesInstanceUID(DCM_OBJECT** iod, const char* seriesUID);
static void replacePatientName(DCM_OBJECT** iod, const char* patientName);
static void replaceAccessionNumber(DCM_OBJECT** iod, const char* accessionNum);
static void replacePatientId(DCM_OBJECT** iod, const char* patientId);
static void replacePatientBirthdate(DCM_OBJECT** iod, const char* patientBirthdate);

static CTNBOOLEAN responseSensitive = FALSE;
static CTNBOOLEAN silent = FALSE;
static CTNBOOLEAN timeTransfer = FALSE;
static long totBytesTransferred = 0;


static void
fillFileList(const char* f, LST_HEAD** lst)
{
    if (UTL_IsDirectory(f)) {
        LST_HEAD* l = 0;
        UTL_FILEITEM* item;

        UTL_ScanDirectory(f, &l);
        item = (UTL_FILEITEM*)LST_Dequeue(&l);
        while(item != NULL) {
            char p2[1024];
            if ((strcmp(item->path, ".") != 0) && (strcmp(item->path, "..") != 0)) {
                strcpy(p2, f);
                strcat(p2, "/");
                strcat(p2, item->path);
                fillFileList(p2, lst);
            }
            free(item);
            item = (UTL_FILEITEM*)LST_Dequeue(&l);
        }
    } else {
        UTL_FILEITEM * p;
        p = malloc(sizeof(*p));
        strcpy(p->path, f);
        LST_Enqueue(lst, p);
    }
}

main(int argc, char **argv)
{
    CONDITION			/* Return values from DUL and ACR routines */
	cond;
    DUL_NETWORKKEY		/* Used to initialize our network */
	* network = NULL;
    DUL_ASSOCIATESERVICEPARAMETERS	/* The items which describe this
					 * Association */
	params = {
	DICOM_STDAPPLICATIONCONTEXT, "DICOM_TEST", "DICOM_STORAGE",
	    "", 16384, 0, 0, 0,
	    "calling addr", "called addr", NULL, NULL, 0, 0,
	    MIR_IMPLEMENTATIONCLASSUID, MIR_IMPLEMENTATIONVERSIONNAME,
	    "", ""
    };
    char
       *calledAPTitle = "DICOM_STORAGE",
       *callingAPTitle = "DICOM_TEST",
       *patName = "DOE^JOHN",
        localHost[40],
       *node,			/* The node we are calling */
       *port,			/* ASCIIZ representation of TCP port */
       *SOPName = NULL;
    int
        scratch;		/* Used to check syntax of port number */
    unsigned int
        numStudies = 1;
    unsigned long
        maxPDU = 16384,
        imageDelay = 0,
        studyDelay = 0;

    CTNBOOLEAN
	verboseDCM = FALSE,
	verboseDUL = FALSE,
	verboseSRV = FALSE,
	allowRepeatedElements = FALSE;

    char* sopInstanceUID = 0;
    char** xferSyntaxes;
    int xferSyntaxCount = 0;
    char xferSyntaxBuf[1024] = "";

/*
    char study[65], series[65], instance[65];
    long timemark = getTimemark();
    int l1, l2, l3;
    l1 = getStudyUID(study, timemark); 
    l2 = getSeriesUID(series, timemark, 1); 
    l3 = getInstanceUID(instance, timemark, 1, 1); 
    printf("study: %s (%i)\n", study, l1);
    printf("series: %s (%i)\n", series, l2);
    printf("instance: %s (%i)\n", instance, l3);
*/

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
	case 'd':
	    argc--;
	    argv++;
	    if (argc <= 0)
		usageerror();
	    if (sscanf(*argv, "%lu", &imageDelay) != 1)
		usageerror();
	    break;
	case 'D':
	    argc--;
	    argv++;
	    if (argc <= 0)
		usageerror();
	    if (sscanf(*argv, "%lu", &studyDelay) != 1)
		usageerror();
	    break;
	case 'm':
	    argc--;
	    argv++;
	    if (argc <= 0)
		usageerror();
	    if (sscanf(*argv, "%lu", &maxPDU) != 1)
		usageerror();
	    break;
	case 'n':
	    argc--;
	    argv++;
	    if (argc <= 0)
		usageerror();
	    patName = *argv;
	    break;
	case 'N':
	    argc--;
	    argv++;
	    if (argc <= 0)
		usageerror();
	    if (sscanf(*argv, "%u", &numStudies) != 1)
		usageerror();
	    break;
	case 'q':
	    silent = TRUE;
	    break;
	case 'r':
	    responseSensitive = TRUE;
	    break;
	case 's':
	    argc--;
	    argv++;
	    if (argc <= 0)
		usageerror();
	    SOPName = *argv;
	    break;
	case 't':
	    timeTransfer = TRUE;
	    break;
	case 'u':
	    argc--;
	    argv++;
	    if (argc <= 0)
		usageerror();
	    sopInstanceUID = *argv;
	    break;
	case 'v':
	    verboseDUL = TRUE;
	    verboseSRV = TRUE;
	    break;
	case 'w':
	    argc--; argv++;
	    if (argc < 1) {
		usageerror();
	    }
	    if (strcmp(*argv, "REPEAT") == 0)
		allowRepeatedElements = TRUE;
	    break;
	case 'x':
	    argc--;
	    argv++;
	    if (argc <= 0)
		usageerror();
	    if (strcmp(*argv, "DCM") == 0)
		verboseDCM = TRUE;
	    else if (strcmp(*argv, "DUL") == 0)
		verboseDUL = TRUE;
	    else if (strcmp(*argv, "SRV") == 0)
		verboseSRV = TRUE;
	    else
		usageerror();
	    break;
	case 'X':
	    argc--; argv++;
	    if (argc <= 0)
	      usageerror();
	    if (xferSyntaxCount++ != 0)
	      strcat(xferSyntaxBuf, ";");
	    strcat(xferSyntaxBuf, UID_Translate(*argv));
	    break;
	default:
	    break;
	}
    }
    if (argc < 3)
	usageerror();

    node = *argv++;
    argc--;
    port = *argv++;
    argc--;
    if (sscanf(port, "%d", &scratch) != 1)
	usageerror();

    THR_Init();
    DCM_Debug(verboseDCM);
    DUL_Debug(verboseDUL);
    SRV_Debug(verboseSRV);
    UTL_SetConfigFile(0);
    cond = DUL_InitializeNetwork(DUL_NETWORK_TCP, DUL_AEREQUESTOR,
			   NULL, DUL_TIMEOUT, DUL_ORDERBIGENDIAN, &network);
    if (cond != DUL_NORMAL)
	myExit(NULL);

    (void) gethostname(localHost, sizeof(localHost));
    sprintf(params.calledPresentationAddress, "%s:%s", node, port);
    strcpy(params.callingPresentationAddress, localHost);
    strcpy(params.calledAPTitle, calledAPTitle);
    strcpy(params.callingAPTitle, callingAPTitle);
    params.maxPDU = maxPDU;

    if (xferSyntaxCount != 0)
      xferSyntaxes = UTL_ExpandToPointerArray(xferSyntaxBuf, ";",
			&xferSyntaxCount);

    sprayImages(argc, argv, &network, &params, allowRepeatedElements,
	    xferSyntaxes, xferSyntaxCount, imageDelay, studyDelay, numStudies,
        patName);
    THR_Shutdown();
    return 0;
}

// write the ip address into given buffer, return the length of 
// buffer.  return -1 on error.
int getHostIPAddress(char *ipaddr) {
    char ac[80];
    struct hostent *phe;
    
    if (gethostname(ac, sizeof(ac)) != 0) {
        printf("Error getting hostname.\n");
        return -1;
    }
    //printf("Hostname: %s\n", ac);

    phe = gethostbyname(ac);
    if (phe == 0) {
        printf("Bad hostname lookup\n");
        return -1;
    }

    // Might have more than one IP, but just take the first one.
    if (phe->h_addr_list[0] != 0) {
        struct in_addr addr;
        memcpy(&addr, phe->h_addr_list[0], sizeof(struct in_addr));
        //printf("Address: %s\n", inet_ntoa(addr));
        strcpy(ipaddr, (char*)inet_ntoa(addr));
        return strlen(ipaddr);
    } else {
        printf("Bad hostname lookup\n");
        return -1;
    }
}

  /*
   * Return a timemark accurate to one second.
   */
long getTimemark() {
#ifdef _MSC_VER
    struct _timeb timeNow;
    _ftime(&timeNow);
    return timeNow.time;
#else
    struct timeval timeNow;
#if (TIMEOFDAYARGS == 2)
    gettimeofday(&timeNow, NULL);
#else
    gettimeofday(&timeNow);
#endif
    return timeNow.tv_sec;
#endif
}

// fill in uid with the UID base characters.  Return length.
// return -1 on error.
int getUIDBase(char *uid, long timemark) {
    /*
     * The base consists of the string "1.2.840.113654.2.2003", plus the last two
     * values of the ip address (xx.xx.XX.XX), plus the timemark, which is to be
     * passed so it does not vary over the course of transmission.
     */
    const char prefix[] = "1.2.840.113654.2.2003";    
    char ipaddr[32], ts[50], *ip;
    int dot;

    int len = strlen(prefix);
    strcpy(uid, prefix);

    if (getHostIPAddress(ipaddr) == -1) {
        printf("Error getting IP address.\n");
        return -1;
    }

    // Get the second period in ip address.
    dot = 0;
    ip = ipaddr;
    while (dot++ < 2) {
        if ((ip = strstr(ip, ".")) == NULL) {
            printf("Error parsing ip address.\n");
            return -1;
        }
        ip++; // move past the .
    }

    strcpy(uid+len, (ip-1)); // -1 to stick the . back in there
    len += strlen(ip)+1;

    sprintf(ts, "%li", timemark);

    strcat(uid, ".");
    strcpy(uid+len+1, ts);

    return strlen(uid);
}

int getStudyUID(char *uid, long timemark, int studyNum) {
    if(getUIDBase(uid, timemark) == -1)
        return -1;
    return sprintf(uid, "%s.%d", uid, studyNum);
}

int getSeriesUID(char *uid, long timemark, int studyNum, int seriesNum) {
    if(getUIDBase(uid, timemark) == -1)
        return -1;
    return sprintf(uid, "%s.%d.%d", uid, studyNum, seriesNum);
}

int getInstanceUID(char *uid, long timemark, int studyNum, int seriesNum, int instanceNum) {
    if(getUIDBase(uid, timemark) == -1)
        return -1;
    return sprintf(uid, "%s.%d.%d.%d", uid, studyNum, seriesNum, instanceNum);
}

static void replaceSOPInstanceUID(DCM_OBJECT** iod,
				  const char* sopInstanceUID)
{
  char uid[DICOM_UI_LENGTH+1];
  DCM_ELEMENT e = { DCM_IDSOPINSTANCEUID, DCM_UI, "", 1, 0, 0 };

  strcpy(uid, sopInstanceUID);
  e.d.string = uid;

  DCM_ModifyElements(iod, &e, 1, 0, 0, 0);
}

static void replaceStudyInstanceUID(DCM_OBJECT** iod,
				  const char* studyUID)
{
  char uid[DICOM_UI_LENGTH+1];
  DCM_ELEMENT e = { DCM_RELSTUDYINSTANCEUID, DCM_UI, "", 1, 0, 0 };

  strcpy(uid, studyUID);
  e.d.string = uid;

  DCM_ModifyElements(iod, &e, 1, 0, 0, 0);
}

static void replaceSeriesInstanceUID(DCM_OBJECT** iod,
				  const char* seriesUID)
{
  char uid[DICOM_UI_LENGTH+1];
  DCM_ELEMENT e = { DCM_RELSERIESINSTANCEUID, DCM_UI, "", 1, 0, 0 };

  strcpy(uid, seriesUID);
  e.d.string = uid;

  DCM_ModifyElements(iod, &e, 1, 0, 0, 0);
}

static void replacePatientName(DCM_OBJECT** iod,
				  const char* patientName)
{
  char uid[DICOM_PN_LENGTH+1];
  DCM_ELEMENT e = { DCM_PATNAME, DCM_PN, "", 1, 0, 0 };

  strcpy(uid, patientName);
  e.d.string = uid;

  DCM_ModifyElements(iod, &e, 1, 0, 0, 0);
}

static void replaceAccessionNumber(DCM_OBJECT** iod,
				  const char* accessionNum)
{
  char uid[DICOM_SH_LENGTH+1];
  DCM_ELEMENT e = {DCM_IDACCESSIONNUMBER, DCM_SH, "", 1, 0, 0 };

  strcpy(uid, accessionNum);
  e.d.string = uid;

  DCM_ModifyElements(iod, &e, 1, 0, 0, 0);
}

static void replacePatientId(DCM_OBJECT** iod,
				  const char* patientId)
{
  char uid[DICOM_LO_LENGTH+1];
  DCM_ELEMENT e = { DCM_PATID, DCM_LO, "", 1, 0, 0 };

  strcpy(uid, patientId);
  e.d.string = uid;

  DCM_ModifyElements(iod, &e, 1, 0, 0, 0);
}

static void replacePatientBirthdate(DCM_OBJECT** iod,
				  const char* patientBirthdate)
{
  char uid[DICOM_DA_LENGTH+1];
  DCM_ELEMENT e = { DCM_PATBIRTHDATE, DCM_DA, "", 1, 0, 0 };

  strcpy(uid, patientBirthdate);
  e.d.string = uid;

  DCM_ModifyElements(iod, &e, 1, 0, 0, 0);
}

static void getXferSyntax(DCM_OBJECT** iod, char* fileXferSyntax)
{
  CONDITION cond;
  DCM_ELEMENT elements[] = {
	{DCM_METATRANSFERSYNTAX, DCM_UI, "", 1, 65, 0}
    };

  elements[0].d.string = fileXferSyntax;
  cond = DCM_ParseObject(iod, elements, DIM_OF(elements), NULL, 0, NULL);
  if (cond != DCM_NORMAL) {
    printf("Unable to get Transfer Syntax from Part 10 file\n");
    exit(1);
  }
}

static int
testForEncapsulatedXferSyntax(const char* fileXferSyntax)
{
  char* xfer[] = {
    DICOM_TRANSFERLITTLEENDIAN,
    DICOM_TRANSFERLITTLEENDIANEXPLICIT,
    DICOM_TRANSFERBIGENDIANEXPLICIT
  };
  int index;

  if (fileXferSyntax == 0)
    return 0;

  for (index = 0; index < 3; index++) {
    if (strcmp(xfer[index], fileXferSyntax) == 0)
      return 0;
  }

  return 1;
}

static DCM_OBJECT** loadImageSet(int argc, char **argv, CTNBOOLEAN allowRepeatedElements, int *numObjects, char *patName) {

    DCM_OBJECT *iod;
    DCM_OBJECT **iods;
    LST_HEAD* fileNames = 0;
    UTL_FILEITEM* p = NULL;
    CTNBOOLEAN part10File;
    int n = 0;

    CONDITION
	cond;
    char fileXferSyntax[65] = "";
    int isEncapsulatedXferSyntax = 0;

    fileNames = LST_Create();
    while (argc-- > 0) {
        fillFileList(*argv, &fileNames);
        argv++;
    }

    if ((iods = calloc(LST_Count(&fileNames), sizeof(iod))) == NULL) {
        printf("Calloc Error.\n");
        myExit(NULL);
    }

    p = LST_Dequeue(&fileNames);
    while (p != NULL) {
        unsigned long options = DCM_ORDERLITTLEENDIAN;
        part10File = FALSE;

        if (allowRepeatedElements)
            options |= DCM_ALLOWREPEATELEMENTS;

        printf("%s\n", p->path);
        cond = DCM_OpenFile(p->path, options, &iod);
        if (cond != DCM_NORMAL) {
            part10File = TRUE;
            cond = DCM_OpenFile(p->path, DCM_ORDERLITTLEENDIAN | DCM_PART10FILE, &iod);
            if (cond != DCM_NORMAL)
                myExit(NULL);
        }
        (void) COND_PopCondition(TRUE);

        if (part10File) {
            getXferSyntax(&iod, fileXferSyntax);
            isEncapsulatedXferSyntax = testForEncapsulatedXferSyntax(fileXferSyntax);
            if (isEncapsulatedXferSyntax) {
                // won't support encapsulated, since we strip part-10 stuff.
                // could support it if kept reference to encapsulation some
                // other way.
                // Do error some better way.
                printf("Error processing %s.  Encapsulation not supported.\n", p->path);
                myExit(NULL); 
            }

            cond = DCM_RemoveGroup(&iod, DCM_GROUPFILEMETA);
            if (cond != DCM_NORMAL)
                myExit(NULL);
            (void) DCM_RemoveGroup(&iod, DCM_GROUPPAD);
            (void) COND_PopCondition(TRUE);
        }

        //replacePatientName(&iod, "DOE^JOHN");
        replacePatientName(&iod, patName);

        iods[n++] = iod;
        p = LST_Dequeue(&fileNames);
    }
    *numObjects = n; 
    return iods;
}

static void
sendStudy(DUL_NETWORKKEY ** network, DUL_ASSOCIATESERVICEPARAMETERS * params, 
	     char** xferSyntaxes, int xferSyntaxCount,
             DCM_OBJECT** objects, int numObjects, int imageDelay)
{
    DUL_ASSOCIATIONKEY		/* Describes the Association with the Acceptor */
        * association = NULL;
    char
        SOPClass[DICOM_UI_LENGTH + 1] = "",
        lastSOPClass[DICOM_UI_LENGTH + 1] = "";
    CONDITION
	cond;
    static char
        instanceUID[DICOM_UI_LENGTH + 1] = "";
    DCM_ELEMENT elements[] = {
	{DCM_IDSOPCLASSUID, DCM_UI, "", 1, sizeof(SOPClass), (void *) SOPClass},
	{DCM_IDSOPINSTANCEUID, DCM_UI, "", 1, sizeof(instanceUID), (void *) instanceUID}
    };
    unsigned long objectLength = 0;
    int status, i;
    DCM_OBJECT *iod;
    CTNBOOLEAN redoAssociation = TRUE;
    CTNBOOLEAN dropAssociation = FALSE;
    // the timemark is used as the basic element for uniqueness of this run.  one timemark
    // per execution of this program.
    static long timeMark = -1;
    static int studyNumber = 1;
    int imageNumber = 1, year, month, day;
    char date[10], uid[65];
    long base;

    if (timeMark == -1) {
        timeMark = getTimemark();
    }

    // Construct the birthdate for this patient, more or less unique based upon this execution
    // and the study.  We want the year to be 1900+y (0 < y < 100), month 1-12, day 1-30.  
    base = timeMark + (long) 5.14159 * studyNumber;
    year = base % 100 + 1900;
    month = base % 12 + 1;
    day = base % 30 + 1;  
    sprintf(date, "%04d%02d%02d", year, month, day);
    //printf("Birthdate = %s \n", date, base);

    for (i = 0; i < numObjects; i++) {

        iod = objects[i];
        // Modify the object's elements.

        getStudyUID(uid, timeMark, studyNumber);
        replaceStudyInstanceUID(&iod, uid);
        replaceSeriesInstanceUID(&iod, uid); // since 1 series per study, same UID for both.
        
        getInstanceUID(uid, timeMark, studyNumber, 1, imageNumber++);
        replaceSOPInstanceUID(&iod, uid);

        // Accession number is timeMark.studyNum
        sprintf(uid, "%li.%d", timeMark, studyNumber);
        replaceAccessionNumber(&iod, uid);

        // Patient ID is the same as the accession number -- why not?
        replacePatientId(&iod, uid);

        replacePatientBirthdate(&iod, date);
         
        cond = DCM_ParseObject(&iod, elements, DIM_OF(elements), NULL, 0, NULL);
        if (cond != DCM_NORMAL)
            myExit(&association);
        if (strcmp(SOPClass, lastSOPClass) != 0) 
            redoAssociation = TRUE;
        if (redoAssociation) {
            printf("Renegotiating Association\n");
            if (dropAssociation) {
                printf("Dropping old association\n");
                (void) DUL_ReleaseAssociation(&association);
                (void) DUL_DropAssociation(&association);
            }
            (void) DUL_ClearServiceParameters(params);
            status = requestAssociation(network, &association,
                    params, SOPClass, xferSyntaxes, xferSyntaxCount);
            dropAssociation = TRUE;
            if (status == 0) {
                myExit(NULL);
            }
        }

        cond = sendImage(&association, params, &iod, SOPClass,
                instanceUID, "");
        if (cond != SRV_NORMAL)
            myExit(&association);
        strcpy(lastSOPClass, SOPClass);
        redoAssociation = FALSE;

        if (timeTransfer) {
            (void) DCM_GetObjectSize(&iod, &objectLength);
            totBytesTransferred += objectLength;
        }

        if (i != numObjects-1) 
            doSleep(imageDelay);
    }

    strcpy(lastSOPClass, "");
    studyNumber++;

    (void) DUL_ReleaseAssociation(&association);
    (void) DUL_DropAssociation(&association);
}

static void sprayImages(int argc, char **argv, DUL_NETWORKKEY ** network,
	     DUL_ASSOCIATESERVICEPARAMETERS * params, 
	     CTNBOOLEAN allowRepeatedElements, char** xferSyntaxes, 
         int xferSyntaxCount, int imageDelay, int studyDelay, int numStudies,
         char *patName)
{
    void *timeStamp = NULL;
    double deltaTime = 0.;
    int l, numObjects;

    DCM_OBJECT** objects = loadImageSet(argc, argv, allowRepeatedElements, &numObjects,
            patName);

    printf("NumStudies = %d\n", numStudies);

    if (timeTransfer) {
        timeStamp = UTL_GetTimeStamp();
    }
    for (l = 0; l < numStudies; l++) {
        sendStudy(network, params, xferSyntaxes, xferSyntaxCount,
             objects, numObjects, imageDelay);
        if (l != numStudies - 1)
            doSleep(studyDelay);
    }

    if (timeTransfer) {
        deltaTime = UTL_DeltaTime(timeStamp);
        printf("%d Studies of %d Images, total %10d bytes transferred.\n", numStudies,
                numObjects, totBytesTransferred);
        printf("Total time (including user-defined delays) %7.3f seconds (%f kB/s)\n",
                deltaTime, (((float) totBytesTransferred) / 1000.) / deltaTime);
        UTL_ReleaseTimeStamp(timeStamp);
    }

    for (l = 0; l < numObjects; l++)
        (void) DCM_CloseObject(&(objects[l]));
    free(objects);
    (void) DUL_ClearServiceParameters(params);
    (void) DUL_DropNetwork(network);
}

/* sendImage
**
** Purpose:
**	Send a single image over the Association
**
** Parameter Dictionary:
**	assocition		Handle to the Association
**	params			Service parameters describing the Association
**	object			Handle to the DICOM object holding the image
**	SOPClass		SOP Class UID
**	instanceUID		SOP Instance UID of the image
**	moveAETitle		Name of the Application Entity
**
** Return Values:
**
**	SRV_ILLEGALPARAMETER
**	SRV_NOCALLBACK
**	SRV_NORMAL
**	SRV_NOSERVICEINASSOCIATION
**	SRV_OBJECTBUILDFAILED
**	SRV_REQUESTFAILED
**	SRV_UNEXPECTEDCOMMAND
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static CONDITION
sendImage(DUL_ASSOCIATIONKEY ** association,
	  DUL_ASSOCIATESERVICEPARAMETERS * params,
	  DCM_OBJECT ** object, char *SOPClass, char *instanceUID,
	  char *moveAETitle)
{
    MSG_C_STORE_REQ
    request;
    MSG_C_STORE_RESP
	response;
    CONDITION
	cond;

    request.type = MSG_K_C_STORE_REQ;
    request.messageID = SRV_MessageIDOut();
    request.priority = 0;
    request.dataSetType = DCM_CMDDATAIMAGE;
    request.dataSet = *object;
    request.fileName = NULL;
    strcpy(request.classUID, SOPClass);
    strcpy(request.instanceUID, instanceUID);
    strcpy(request.moveAETitle, moveAETitle);

    cond = SRV_CStoreRequest(association, params, &request, &response,
			     sendCallback, "context string", "foo");
    MSG_DumpMessage(&response, stdout);

    if (responseSensitive && response.status != MSG_K_SUCCESS)
	cond = 0;

    return cond;
}


/* sendCallback
**
** Purpose:
**	Callback routine
**
** Parameter Dictionary:
**	request		Pointer to request message
**	response	Pointer to response message
**	transmitted	Number of bytes sent
**	total		Total number of bytes sent
**	string		Context Information
**
** Return Values:
**	SRV_NORMAL
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
static CONDITION
sendCallback(MSG_C_STORE_REQ * request, MSG_C_STORE_RESP * response,
	     unsigned long transmitted, unsigned long total,
	     void *string)
{
    if (transmitted == 0 && !silent)
	printf("Initial call to sendCallback\n");

    if (!silent)
	printf("%8d bytes transmitted of %8d (%s)\n", transmitted, total, (char *) string);

    if (response != NULL && !silent)
	MSG_DumpMessage(response, stdout);

    return SRV_NORMAL;
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
send_image [-a application] [-c called] [-d imageDelay] [-D studyDelay] [-m maxPDU] [-N numStudies] [-q] [-r] [-s SOPName] [-t] [-x FAC] [-X xfer] [-v] [-w flag] node port image [image...]\n\
    \n\
    -a    Set application title of this (calling) application\n\
    -c    Set called AE title to title in Association RQ\n\
    -d    Set delay, in milliseconds, after each image transmitted.  Default=0 \n\
    -D    Set delay, in milliseconds, after each study transmitted.  Default=0 \n\
    -m    Set maximum PDU in Association RQ to maxPDU\n\
    -n    Set patient name for all images (default = \"DOE^JOHN\")\n\
    -N    Set number of times the study is transmitted.  Default=1\n\
    -q    Quiet mode.  Suppresses some messages to stdout\n\
    -r    Make program sensitve to response status.  If not success, stop\n\
    -s    Force an initial Association using one SOP Class based on SOPName\n\
          (CR, CT, MR, NM, SC, US)\n\
    -t    Time the image transfer.  Print elapsed time and transfer rate.\n\
    -v    Place DUL and SRV facilities in verbose mode\n\
    -x    Place one facility(DCM, DUL, SRV) in verbose mode\n\
    -X    Specify a transfer syntax to be proposed; may repeat this switch\n\
    -w    Set open options; flag can be REPEAT \n\
  \n\
    node  Node name for network connection\n\
    port  TCP / IP port number of server application\n\
    image A list of one or more images to send\n";

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



/* requestAssociation
**
** Purpose:
**	Request for a Association establishment
**
** Parameter Dictionary:
**	network		Key to the network connection
**	association	Handle to the Association (to be returned)
**	params		Service parameters describing the Association
**	SOPClass	SOPClass for which the service is to be requested
**
** Return Values:
**	1 => success
**	0 => failure
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static int
requestAssociation(DUL_NETWORKKEY ** network,
		   DUL_ASSOCIATIONKEY ** association,
		   DUL_ASSOCIATESERVICEPARAMETERS * params, char *SOPClass,
		   char** xferSyntaxes, int xferSyntaxCount)
{
  CONDITION cond;

  cond = SRV_ProposeSOPClassWithXfer(SOPClass, DUL_SC_ROLE_SCU,
			xferSyntaxes, xferSyntaxCount, 1, params);

  if (cond != SRV_NORMAL) {
    COND_DumpConditions();
    return 0;
  }

  cond = DUL_RequestAssociation(network, params, association);
  if (cond != DUL_NORMAL) {
    if (cond == DUL_ASSOCIATIONREJECTED) {
      fprintf(stderr, "Association Rejected\n");
      fprintf(stderr, " Result: %2x Source %2x Reason %2x\n",
	    params->result, params->resultSource,
		    params->diagnostic);
    }
    return 0;
  }
  if (!silent) {
    (void) printf("Association accepted, parameters:\n");
    DUL_DumpParams(params);
  }
  return 1;
}

#define BILLION 1000000000
static void doSleep(long msecs) {
#ifdef _MSC_VER
    Sleep(msecs);
#else
    struct timespec s = {msecs / 1000, msecs % BILLION};
    nanosleep(&s, NULL);
#endif
}
