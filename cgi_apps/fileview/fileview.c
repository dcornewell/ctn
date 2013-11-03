/* 
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "cgi-lib.h"
#include "html-lib.h"

#include "dicom.h"
#include "lst.h"
#include "condition.h"
#include "dicom_objects.h"

typedef struct {
    DCM_VALUEREPRESENTATION r;
    char *translation;
}   MAP;

static char *
translate(DCM_VALUEREPRESENTATION r)
{
    static MAP map[] = {
	{DCM_AE, "AE"},
	{DCM_AS, "AS"},
	{DCM_AT, "AT"},
	{DCM_CS, "CS"},
	{DCM_DA, "DA"},
	{DCM_DD, "DD"},
	{DCM_DS, "DS"},
	{DCM_FD, "FD"},
	{DCM_FL, "FL"},
	{DCM_IS, "IS"},
	{DCM_LO, "LO"},
	{DCM_LT, "LT"},
	{DCM_OT, "OT"},
	{DCM_SH, "SH"},
	{DCM_SL, "SL"},
	{DCM_SQ, "SQ"},
	{DCM_SS, "SS"},
	{DCM_ST, "ST"},
	{DCM_TM, "TM"},
	{DCM_UI, "UI"},
	{DCM_UL, "UL"},
	{DCM_US, "US"},
	{DCM_UNKNOWN, "UNKNOWN"},
	{DCM_RET, "RET"},
	{DCM_CTX, "CTX"},
	{DCM_PN, "PN"},
	{DCM_OB, "OB"},
	{DCM_OW, "OW"},
	{DCM_DT, "DT"}
    };
    int i;

    for (i = 0; i < (int) DIM_OF(map); i++) {
	if (r == map[i].r)
	    return map[i].translation;
    }
    return "??";
}

typedef struct {
  void* reserved[2];
  int isDirectory;
  char fileName[1024];
} FILE_ENTRY;

static void listDirectory(const char* directory, LST_HEAD** l)
{
  FILE_ENTRY* f;
  struct dirent* dp;
  DIR *dirp;

  dirp = opendir(directory);
  while ((dp = readdir(dirp)) != NULL) {
    if (strcmp(dp->d_name, ".") == 0)
      continue;
    if (strcmp(directory, "/opt/ctn/images") == 0) {
      if (strcmp(dp->d_name, ".") == 0)
	continue;
      if (strcmp(dp->d_name, "..") == 0)
	continue;
    }
    f = calloc(1, sizeof(*f));
    strcpy(f->fileName, dp->d_name);
    f->isDirectory = 0;
    LST_Enqueue(l, f);
  }
  close(dirp);
}

static void directoryToHTML(const char* path)
{
  LST_HEAD* l;
#if 0
  char topDirectory[1024];
  char currentDirectory[1024];
  char key[1024];
#endif
  FILE_ENTRY* f;
  char parentPath[1024];
  char *c;

  strcpy(parentPath, path);
  c = parentPath + strlen(parentPath);
  while (*(--c) != '/')
    ;
  *c = '\0';

  l = LST_Create();
  listDirectory(path, &l);

  html_header();
  html_begin_body_options("File View", "bgcolor=#ffffff");

  while ((f = LST_Dequeue(&l)) != NULL) {
    if (strcmp(f->fileName, "..") == 0) {
      printf("<br><a href=""/cgi-bin/fileview.cgi"
		"?submitSearch=FileSelect"
		"&Key=%s"
		"&CurrentDirectory=%s"
		"&>%s</a>\n",
	   ".",
	   parentPath,
	   f->fileName);
    } else {
      printf("<br><a href=""/cgi-bin/fileview.cgi"
		"?submitSearch=FileSelect"
		"&Key=%s"
		"&CurrentDirectory=%s"
		"&>%s</a>\n",
	   f->fileName,
	   path,
	   f->fileName);
    }
    free(f);
  }
  html_end();
  LST_Destroy(&l);
}

static CONDITION callback(const DCM_ELEMENT* e, void* ctx)
{
  char txt[512] = "";
  printf("<TR>\n");
  printf("<TD>%04x %04x</TD>\n", DCM_TAG_GROUP(e->tag), DCM_TAG_ELEMENT(e->tag));
  printf("<TD>%s\n", translate(e->representation));
  printf("<TD ALIGN=RIGHT>%6d</TD>\n", e->length);
  printf("<TD ALIGN=CENTER>%s</TD>\n", e->description);
  if (e->d.ot == NULL) {
    strcpy(txt, "No data");
  } else if (e->length == 0) {
    strcpy(txt, "0 length data");
  } else {
    if (DCM_IsString(e->representation)) {
      int stringLength;
      stringLength = MIN(sizeof(txt)-1, e->length);
      strncpy(txt, e->d.string, stringLength);
      txt[stringLength] = '\0';
    } else {
      switch (e->representation) {
      case DCM_AE:
      case DCM_AS:
	break;
      case DCM_AT:
	sprintf(txt, "%04x %04x",
		DCM_TAG_GROUP(*e->d.ul), DCM_TAG_ELEMENT(*e->d.ul));
	break;
      case DCM_CS:
      case DCM_DA:
	break;
      case DCM_DD:
	strcpy(txt, "Unimplemented");
	break;
      case DCM_DS:
	break;
      case DCM_FD:
      case DCM_FL:
	strcpy(txt, "Unimplemented");
	break;
      case DCM_IS:
      case DCM_LO:
      case DCM_LT:
      case DCM_OT:
      case DCM_SH:
	break;
      case DCM_SL:
	sprintf(txt, "%d", *e->d.sl);
	break;
      case DCM_SQ:
	strcpy(txt, "Unimplemented");
	break;
      case DCM_SS:
	sprintf(txt, "%d", *e->d.ss);
	break;
      case DCM_ST:
      case DCM_TM:
      case DCM_UI:
	break;
      case DCM_UL:
	sprintf(txt, "%d", *e->d.ul);
	break;
      case DCM_US:
	sprintf(txt, "%d", *e->d.us);
	break;
      case DCM_UNKNOWN:
	strcpy(txt, "Unknown");
	break;
      case DCM_RET:
	strcpy(txt, "Retired");
	break;
      case DCM_CTX:
	strcpy(txt, "Context sensitive");
	break;
      case DCM_PN:
	break;
      case DCM_OB:
      case DCM_OW:
	strcpy(txt, "OB or OW");
	break;
      case DCM_DT:
	break;
      case DCM_DLM:
	strcpy(txt, "Delimiter");
	break;
      default:
	break;
      }
    }
  }
  printf("<TD>%s</TD></TR>\n", txt);
  return DCM_NORMAL;
}

static void fileToHTML(const char* path)
{
  DCM_OBJECT* obj;
  CONDITION cond;

  THR_Init();
  cond = DCM_OpenFile(path, DCM_ORDERLITTLEENDIAN, &obj);
  if (cond != DCM_NORMAL) {
    (void) DCM_CloseObject(&obj);
    cond = DCM_OpenFile(path, DCM_PART10FILE, &obj);
  }
  if (cond != DCM_NORMAL) {
    exit(0);
  }
#if 1
    printf("Content-type: text/html\n\n");
    printf("<HTML>\n");
    printf("<HEAD>\n");
    printf("<META HTTP-EQUIV=""Content-Type"" CONTENT=""text/html; charset=iso-2022-jp"">\n");
    printf("<TITLE>dcm_to_html</TITLE>\n");
    printf("</HEAD>");

    printf("<BODY BGCOLOR=#ffffff FGCOLOR=#ff000000>\n");
    printf("<B>%s</B>\n", path);
    printf("<TABLE BORDER>\n");
    DCM_ScanParseObject(&obj,
			NULL, 0,           /* Buffer/buffer size */
			NULL, 0,           /* Vector of elements to scan */
			callback, NULL);
    printf("</TABLE>\n");
    printf("</BODY>\n");
    printf("</HTML>");

    (void) DCM_CloseObject(&obj);
    THR_Shutdown();
#else
  html_header();
  html_begin_body_options("File View", "bgcolor=#ffffff");
  printf("%s<br>\n", path);
  html_end();
#endif
}


static void view(llist entries)
{
  directoryToHTML("/opt/ctn/images");
}

static void fileSelect(llist entries)
{
  LST_HEAD* l;
  char topDirectory[1024];
  char currentDirectory[1024];
  char key[1024];
  FILE_ENTRY* f;
  struct stat buf;
  int i;
  int flag = 0;

  strcpy(currentDirectory, cgi_val(entries, "CurrentDirectory"));
  strcpy(key, cgi_val(entries, "Key"));
  strcat(currentDirectory, "/");
  strcat(currentDirectory, key);

  memset(&buf, 0, sizeof(buf));

#ifdef WIN32
  i = _stat(currentDirectory, &buf);
  if (i == 0) {
    flag = ((buf.st_mode & _S_IFDIR) != 0);
  }
#else
  i = stat(currentDirectory, &buf);
  if (i == 0) {
    flag = (S_ISDIR(buf.st_mode));
  }
#endif

  if (flag == 0)
    fileToHTML(currentDirectory);
  else
    directoryToHTML(currentDirectory);
}

#if 0
static void selectArchive(llist entries)
{
  CONDITION cond;
  LST_HEAD *studyList;
  char dbKey[64];
  QUERY_LIST_ITEM *item;
  IDB_Limits limits;
  char password[64];
  int flag = 0;

  studyList = LST_Create();
  strcpy(dbKey, cgi_val(entries, "DBKey"));
  strcpy(password, cgi_val(entries, "password"));

  TBL_SetOption("OPEN_SPEEDUP");
  cond = listOfStudies(dbKey, studyList);
  orderStudyListByName(studyList);

  flag = passwordCompare(dbKey, password);
  if (flag == 0)
    strcpy(password, "READONLY");

  memset(&limits, 0, sizeof(limits));
  cond = dbLimits(dbKey, &limits);

  html_header();
  html_begin_body_options("CTN Archive: List of Studies", "bgcolor=#ffffff");

  if (flag == 0) {
    printf("Read only access granted <br>\n");
  }

  printf("CTN Archive: %s <br> \n", dbKey);
  { char *remoteUser;

    remoteUser = getenv("REMOTE_USER");
    if (remoteUser == NULL)
      remoteUser = "UNKNOWN";

    printf("Remote user: %s<br>\n", remoteUser);
  }

  printf("%d patients, %d studies, %d images <br>\n",
	 limits.PatientCount,
	 limits.StudyCount,
	 limits.ImageCount);
  printf("DB Size/Limits (MB): %d/%d<br>\n",
	 limits.DBSize/1000000,
	 limits.DBLimit);
#if 0
  printf("%d studies <br><br>\n", LST_Count(&studyList));
#endif

  printf("<pre>\n");
  item = LST_Dequeue(&studyList);
  while(item != NULL) {
    char tmp[1024];
    sprintf(tmp, "%-30s %8s (%2d series) %-32s",
	    item->query.patient.PatNam,
	    item->query.study.StuDat,
	    item->query.study.NumStuRelSer,
	    item->query.study.StuDes);

#if 0
    printf("<a href=""/cgi-bin/archive_agent.cgi?submitSearch=%s&DBKey=%s&StudyUID=%s"">%s</a><br>\n",
#endif

    printf(
"<a href=""/cgi-bin/archive_agent.cgi?submitSearch=%s&DBKey=%s&\
password=%s&StudyUID=%s"">%s</a>\n",
	   "StudySelect",
	   dbKey,
	   password,
	   item->query.study.StuInsUID,
	   tmp);

    free(item);
    item = LST_Dequeue(&studyList);
  }
  printf("</pre>\n");

  LST_Destroy(&studyList);

  html_end();
}

#endif


#if 0
static void selectStudy(llist entries)
{
  CONDITION cond;
  char dbKey[64];
  char studyUID[65];
  IDB_Query study;
  LST_HEAD *imageList;
  LST_HEAD *seriesList;
  QUERY_LIST_ITEM *item;
  IDB_InstanceListElement *instancePtr;
  unsigned long byteCount = 0;
  char password[64];


  strcpy(dbKey, cgi_val(entries, "DBKey"));
  strcpy(studyUID, cgi_val(entries, "StudyUID"));
  strcpy(password, cgi_val(entries, "password"));

  TBL_SetOption("OPEN_SPEEDUP");
  cond = searchOneStudy(dbKey, studyUID, &study);

  imageList = LST_Create();
  seriesList = LST_Create();
  cond = searchImages(dbKey, studyUID, imageList);
  while ((item = LST_Dequeue(&imageList)) != NULL) {

    instancePtr = LST_Dequeue(&(item->query.image.InstanceList));

    if (instancePtr != NULL)
      byteCount += instancePtr->Size;
#if 0
    while (instancePtr != NULL) {
      free(instancePtr);
      instancePtr = LST_Dequeue(&(item->query.image.InstanceList));
    }

    free(item);
#endif
  }

  html_header();
  html_begin_body_options("CTN Archive: Study Management", "bgcolor=#ffffff");

  printf("<table>\n");
  printf("<TR><TH>Name: <TH>%s</TR>\n", study.patient.PatNam);
  printf("<TR><TH>Date of Birth: <TH>%s</TR>\n", study.patient.PatBirDat);
  printf("<TR><TH>Accession Number: <TH>%s</TR>\n", study.study.AccNum);
  printf("<TR><TH>Study Date: <TH>%s</TR>\n", study.study.StuDat);
  printf("<TR><TH>Study Description: <TH>%s</TR>\n", study.study.StuDes);
  printf("<TR><TH>Series: <TH>%d</TR>\n", study.study.NumStuRelSer);
  printf("<TR><TH>Images: <TH>%d</TR>\n", study.study.NumStuRelIma);
  printf("<TR><TH>Image Volume: <TH>%d bytes</TR>\n", byteCount);
  printf("</table>\n");

  printf("<form method=GET action=""archive_agent.cgi"">\n");
  printf("<input type=hidden name=DBKey value=%s>\n", dbKey);
  printf("<input type=hidden name=StudyUID value=%s>\n", studyUID);

/*********/
  printf("<pre>\n");
  cond = searchSeries(dbKey, studyUID, seriesList);
  item = LST_Dequeue(&seriesList);
  printf("%-10s %6s %15s %15s %15s <br>",
	"Series #",
	"Images",
	"Proc Name",
	"Series Des",
	"Body Part");
  while(item != NULL) {
    char tmp[1024];
    sprintf(tmp, "%-10s %6d %15s %15s %15s",
	    item->query.series.SerNum,
	    item->query.series.NumSerRelIma,
	    item->query.series.ProNam,
	    item->query.series.SerDes,
	    item->query.series.BodParExa);

    printf(
"<a href=""/cgi-bin/archive_agent.cgi?submitSearch=%s&DBKey=%s&\
password=%s&StudyUID=%s&SeriesUID=%s"">%s</a>\n",
	   "SeriesSelect",
	   dbKey,
	   password,
	   studyUID,
	   item->query.series.SerInsUID,
	   tmp);

    free(item);
    item = LST_Dequeue(&seriesList);
  }
  printf("</pre>\n");
/**********/

  if (strcmp(password, "READONLY") != 0) {
    printf("<input type=submit value=""Delete_Study"" name=submitSearch>\n");
  }
  printf("</form>\n");

  html_end();
}
#endif

#if 0
static void deleteStudy(llist entries)
{
  CONDITION cond;
  char dbKey[64];
  char studyUID[65];
  IDB_Query study;

  strcpy(dbKey, cgi_val(entries, "DBKey"));
  strcpy(studyUID, cgi_val(entries, "StudyUID"));

  TBL_SetOption("OPEN_SPEEDUP");

  cond = deleteOneStudy(dbKey, studyUID);

  selectArchive(entries);

#if 0

  cond = searchOneStudy(dbKey, studyUID, &study);

  html_header();
  html_begin_body_options("CTN Archive: Study Management", "bgcolor=#ffffff");

  printf("<table>\n");
  printf("<TR><TH>Name: <TH>%s</TR>\n", study.patient.PatNam);
  printf("<TR><TH>Date of Birth: <TH>%s</TR>\n", study.patient.PatBirDat);
  printf("<TR><TH>Accession Number: <TH>%s</TR>\n", study.study.AccNum);
  printf("</table>\n");

#endif
}
#endif

#if 0
static void selectSeries(llist entries)
{
  CONDITION cond;
  char dbKey[64];
  char studyUID[65];
  char seriesUID[65];
  IDB_Query series;
  QUERY_LIST_ITEM *item;
  IDB_InstanceListElement *instancePtr;
  unsigned long byteCount = 0;
  char password[64];
  DMAN_HANDLE *controlHandle;

  LST_HEAD *destinationList;

  openCTN(&controlHandle);

  strcpy(dbKey, cgi_val(entries, "DBKey"));
  strcpy(studyUID, cgi_val(entries, "StudyUID"));
  strcpy(seriesUID, cgi_val(entries, "SeriesUID"));
  strcpy(password, cgi_val(entries, "password"));

  TBL_SetOption("OPEN_SPEEDUP");
  cond = searchOneSeries(dbKey, studyUID, seriesUID, &series);

  html_header();
  html_begin_body_options("CTN Archive: Series Management", "bgcolor=#ffffff");

  printf("<table>\n");

  printf("<TR><TH align=right>Name: <TH align=left>%s</TR>\n",
	 series.patient.PatNam);
  printf("<TR><TH align=right>Date of Birth: <TH align=left>%s</TR>\n",
	 series.patient.PatBirDat);
  printf("<TR><TH align=right>Series Number: <TH align=left>%s</TR>\n",
	 series.series.SerNum);
  printf("<TR><TH align=right>Image Count: <TH align=left>%d</TR>\n",
	 series.series.NumSerRelIma);

  printf("</table>\n");

  printf("<form method=GET action=""archive_agent.cgi"">\n");
  printf("<input type=hidden name=DBKey value=%s>\n", dbKey);
  printf("<input type=hidden name=StudyUID value=%s>\n", studyUID);
  printf("<input type=hidden name=SeriesUID value=%s>\n", seriesUID);
  printf("<input type=hidden name=password value=%s>\n", password);


  { DMAN_VIDEOIMAGEDEST *video;

    destinationList = LST_Create();

    cond = listOfDestinations(&controlHandle, destinationList);

    printf("<select name=DestinationApplication size=10>\n");

    video = LST_Dequeue(&destinationList);
    while (video != NULL) {
      /*char tmp[1024];*/
      DMAN_APPLICATIONENTITY ae;

      memset(&ae, 0, sizeof(ae));
      ae.Type = DMAN_K_APPLICATIONENTITY;
      DMAN_LookupApplication(&controlHandle, video->RespondingTitle, &ae);

      /*sprintf(tmp, "%17s %s", ae.Title, ae.Title, ae.Node, ae.Comment);*/
      printf("<option value=""%s"">%16s %15s %d %s\n",
/*	   ae.Title,*/
	   video->RespondingTitle,
	   ae.Title,
	   ae.Node,
	   ae.Port,
	   ae.Comment);

      free(video);
      video = LST_Dequeue(&destinationList);
    }
    printf("</select>\n");
  }
  LST_Destroy(&destinationList);

  printf("<br>\n");
  printf("<input type=submit value=""MoveSeries"" name=submitSearch>\n");

  printf("</form>\n");

  html_end();
}
#endif

#if 0
static void moveSeries(llist entries)
{
  CONDITION cond;
  char dbKey[64] = "";
  char studyUID[65] = "";
  char seriesUID[65] = "";
  char destinationApplication[17] = "";
  char password[64] = "";
  int i;
  long successes = 0;
  long warnings = 0;
  long failures = 0;
  void *timeStamp = 0;
  double deltaTime = 0;

  DMAN_HANDLE *controlHandle;

  strcpy(dbKey, cgi_val(entries, "DBKey"));
  strcpy(studyUID, cgi_val(entries, "StudyUID"));
  strcpy(seriesUID, cgi_val(entries, "SeriesUID"));
  strcpy(password, cgi_val(entries, "password"));
  strcpy(destinationApplication, cgi_val(entries, "DestinationApplication"));

  TBL_SetOption("OPEN_SPEEDUP");

  html_header();
  html_begin_body_options("CTN Archive: Move Series", "bgcolor=#ffffff");

  printf("<pre>\n");
  printf("DB Key:      %s<br>\n", dbKey);
  printf("Study UID:   %s<br>\n", studyUID);
  printf("Series UID:  %s<br>\n", seriesUID);
  /*printf("Password:    %s<br>\n", password);*/
  printf("Destination: %s<br>\n", destinationApplication);

  timeStamp = UTL_GetTimeStamp();

  cond = moveSeriesRequest(dbKey, studyUID, seriesUID, destinationApplication,
			   &successes, &warnings, &failures);

  deltaTime = UTL_DeltaTime(timeStamp);
  UTL_ReleaseTimeStamp(timeStamp);

  printf("Successes:   %d<br>\n", successes);
  printf("Warnings:    %d<br>\n", warnings);
  printf("Failures:    %d<br>\n", failures);
  printf("Xmit time:   %8.2f seconds <br>\n", deltaTime);

  printf("</pre>\n");

  logMoveRequest(dbKey, studyUID, seriesUID, destinationApplication,
		 successes, warnings, failures);

  html_end();
}
#endif


#if 0
static void retrieveStudies(llist entries)
{
  html_header();
  html_begin_body_options("DICOM Study Search", "bgcolor=#ffffff");
  printf("%s<p>\n", cgi_val(entries, "patientID"));

  html_end();

}
#endif

/* Entries for Local Directory Operations */

#if 0
static void localDirectory(llist entries)
{
  LST_HEAD *patientList;
  DCM_OBJECT *dirFile;
  DDR_PATIENT *patientNode;
  CONDITION cond;

  cond = DCM_OpenFile("/tmp/dicomdir", DCM_ORDERLITTLEENDIAN | DCM_PART10FILE,
		      &dirFile);
  patientList = LST_Create();

  cond = DDR_GetPatientList(&dirFile, &patientList);

  html_header();
  html_begin_body_options("DICOM Patient Search", "bgcolor=#ffffff");

  printf("Please select one patient to see a list of studies<p>\n");

  printf("<form method=GET action=""/cgi-bin/acc_agent.cgi"">\n");

  patientNode = LST_Dequeue(&patientList);
  printf("<select name=patientID size=10>\n");
  while(patientNode != NULL) {
    printf("<option value=""%s"">%s %s %s\n",
	   patientNode->PatientID,
	   patientNode->PatientID,
	   patientNode->PatientName,
	   patientNode->BirthDate);
    free(patientNode);
    patientNode = LST_Dequeue(&patientList);
  }
  printf("</select>\n");
  printf("<br>\n");
  printf("<input type=submit value=""LocalPatientSelect"" name=submitSearch>\n");
  printf("<input type=submit value=""Retrieve All Studies from Archive"" name=submitSearch>\n");

  printf("</form>\n");
  html_end();
  LST_Destroy(&patientList);

  DCM_CloseObject(&dirFile);
}

static void localPatientSelect(llist entries)
{
  LST_HEAD *studyList;
  DCM_OBJECT *dirFile;
  DDR_STUDY *studyNode;
  CONDITION cond;

  cond = DCM_OpenFile("/tmp/dicomdir", DCM_ORDERLITTLEENDIAN | DCM_PART10FILE,
		      &dirFile);
  studyList = LST_Create();

  cond = DDR_GetStudyList(&dirFile,
		cgi_val(entries, "patientID"),
		&studyList);

  html_header();
  html_begin_body_options("DICOM Study Search", "bgcolor=#ffffff");

  studyNode = LST_Dequeue(&studyList);
  printf("<tt>\n");
  while(studyNode != NULL) {
    printf("%s %s %s %s <br>\n",
	   studyNode->StudyDate,
	   studyNode->AccessionNumber,
	   studyNode->StudyID,
	   studyNode->StudyDescription);

    free(studyNode);
    studyNode = LST_Dequeue(&studyList);
  }
  printf("</tt>\n");
  html_end();


  DCM_CloseObject(&dirFile);
}
#endif


int main()
{
  llist entries;
  int status;
  char *form;

  status = read_cgi_input(&entries);
  form = cgi_val(entries, "submitSearch");

  if (strcmp(form, "View") == 0)
    view(entries);
  else if (strcmp(form, "FileSelect") == 0)
    fileSelect(entries);
  else {
    html_header();
    html_begin_body_options("Query Results", "bgcolor=#ffffff");
    printf("<h1>Status = %d</h1>\n",status);
    h1("Query results");
    print_entries(entries);
    html_end();
  }
  list_clear(&entries);
  return 0;
}
#if 0
====
/*
          Copyright (C) 1998, Washington University

	  This application was developed at the
                  Electronic Radiology Laboratory
                  Mallinckrodt Institute of Radiology
                  Washington University School of Medicine
                  510 S. Kingshighway Blvd.
                  St. Louis, MO 63110

          THIS SOFTWARE IS MADE AVAILABLE, AS IS, AND NEITHER RSNA NOR
          WASHINGTON UNIVERSITY MAKE ANY WARRANTY ABOUT THE SOFTWARE, ITS
          PERFORMANCE, ITS MERCHANTABILITY OR FITNESS FOR ANY PARTICULAR
          USE, FREEDOM FROM ANY COMPUTER DISEASES OR ITS CONFORMITY TO ANY
          SPECIFICATION. THE ENTIRE RISK AS TO QUALITY AND PERFORMANCE OF
          THE SOFTWARE IS WITH THE USER.

          Copyright of the software and supporting documentation is
          owned by and Washington University, and free access
          is hereby granted as a license to use this software, copy this
          software and prepare derivative works based upon this software.
          However, any distribution of this software source code or
          supporting documentation or derivative works (source code and
          supporting documentation) must include the three paragraphs of
          the copyright notice.
*/
/* Copyright marker.  Copyright will be inserted above.  Do not remove */
/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	main
**			usageerror
** Author, Date:	Stephen M. Moore, 4-Dec-1998
** Intent:		This program reads a DICOM file and creates an HTML
**			representation of the information in the file.
**   Usage:		dcm_to_html [-t] file
** Last Update:		$Author: smm $, $Date: 1999-11-03 16:57:36 $
** Source File:		$RCSfile: fileview.c,v $
** Revision:		$Revision: 1.1 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.1 $ $RCSfile: fileview.c,v $";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef GCCSUNOS
#include <sys/types.h>
#endif

#include "dicom.h"
#include "ctnthread.h"
#include "lst.h"
#include "condition.h"
#include "dicom_objects.h"

static CTNBOOLEAN verboseOutput = FALSE;

static void usageerror();

static void
compareTextAttribute(const DCM_ELEMENT * e1,
		     const DCM_ELEMENT * e2, void *ctx)
{
    const char *c1;
    const char *c2;

    c1 = e1->d.string;
    c2 = e2->d.string;

    while (*c1 == ' ')
	c1++;

    while (*c2 == ' ')
	c2++;

    if (strcmp(c1, c2) == 0) {
#if 0
	if (verboseOutput)
	    printf("       %04x %04x %32x\n",
		   DCM_TAG_GROUP(e1->tag),
		   DCM_TAG_GROUP(e2->tag),
		   e1->description);
#endif
	return;
    }
    printf("  TEXT %04x %04x %32s\n        %s\n        %s\n",
	   DCM_TAG_GROUP(e1->tag),
	   DCM_TAG_ELEMENT(e2->tag),
	   e1->description,
	   c1, c2);
    (*(long *) ctx)++;

}

static void
compareULAttribute(const DCM_ELEMENT * e1, const DCM_ELEMENT * e2, void *ctx)
{
    const U32 *p1,
       *p2;
    U32 l1;

    p1 = e1->d.ul;
    p2 = e2->d.ul;
    if (e1->length != e2->length) {
	printf("LENGTH %04x %04x %32s %d %d\n",
	       DCM_TAG_GROUP(e1->tag),
	       DCM_TAG_ELEMENT(e2->tag),
	       e1->description,
	       e1->length, e2->length);
	(*(long *) ctx)++;
	return;
    }
    l1 = e1->length;

    while (l1 > 0) {
	if (*p1 != *p2) {
	    printf("BINARY %04x %04x %32s %d %d\n",
		   DCM_TAG_GROUP(e1->tag),
		   DCM_TAG_ELEMENT(e2->tag),
		   e1->description,
		   *p1, *p2);
	    (*(long *) ctx)++;
	    return;
	}
	p1++;
	p2++;
	l1 -= sizeof(U32);
    }
}

static void
compareSLAttribute(const DCM_ELEMENT * e1, const DCM_ELEMENT * e2, void *ctx)
{
    const S32 *p1,
       *p2;
    U32 l1;

    p1 = e1->d.sl;
    p2 = e2->d.sl;
    if (e1->length != e2->length) {
	printf("LENGTH %04x %04x %32s %d %d\n",
	       DCM_TAG_GROUP(e1->tag),
	       DCM_TAG_ELEMENT(e2->tag),
	       e1->description,
	       e1->length, e2->length);
	(*(long *) ctx)++;
	return;
    }
    l1 = e1->length;

    while (l1 > 0) {
	if (*p1 != *p2) {
	    printf("BINARY %04x %04x %32s %d %d\n",
		   DCM_TAG_GROUP(e1->tag),
		   DCM_TAG_ELEMENT(e2->tag),
		   e1->description,
		   *p1, *p2);
	    (*(long *) ctx)++;
	    return;
	}
	p1++;
	p2++;
	l1 -= sizeof(S32);
    }
}

static void
compareUSAttribute(const DCM_ELEMENT * e1, const DCM_ELEMENT * e2, void *ctx)
{
    const U16 *p1,
       *p2;
    U32 l1;

    p1 = e1->d.us;
    p2 = e2->d.us;
    if (e1->length != e2->length) {
	printf("LENGTH %04x %04x %32s %d %d\n",
	       DCM_TAG_GROUP(e1->tag),
	       DCM_TAG_ELEMENT(e2->tag),
	       e1->description,
	       e1->length, e2->length);
	(*(long *) ctx)++;
	return;
    }
    l1 = e1->length;

    while (l1 > 0) {
	if (*p1 != *p2) {
	    printf("BINARY %04x %04x %32s %d %d\n",
		   DCM_TAG_GROUP(e1->tag),
		   DCM_TAG_ELEMENT(e2->tag),
		   e1->description,
		   *p1, *p2);
	    (*(long *) ctx)++;
	    return;
	}
	p1++;
	p2++;
	l1 -= sizeof(U16);
    }
}

static void
compareSSAttribute(const DCM_ELEMENT * e1, const DCM_ELEMENT * e2, void *ctx)
{
    const S16 *p1,
       *p2;
    S32 l1;

    p1 = e1->d.ss;
    p2 = e2->d.ss;
    if (e1->length != e2->length) {
	printf("LENGTH %04x %04x %32s %d %d\n",
	       DCM_TAG_GROUP(e1->tag),
	       DCM_TAG_ELEMENT(e2->tag),
	       e1->description,
	       e1->length, e2->length);
	(*(long *) ctx)++;
	return;
    }
    l1 = e1->length;

    while (l1 > 0) {
	if (*p1 != *p2) {
	    printf("BINARY %04x %04x %32s %d %d\n",
		   DCM_TAG_GROUP(e1->tag),
		   DCM_TAG_ELEMENT(e2->tag),
		   e1->description,
		   *p1, *p2);
	    (*(long *) ctx)++;
	    return;
	}
	p1++;
	p2++;
	l1 -= sizeof(S16);
    }
}




static void
compareAttribute(const DCM_ELEMENT * e1, const DCM_ELEMENT * e2,
		 void *ctx)
{

    if (verboseOutput)
	printf("       %04x %04x %32s\n", DCM_TAG_GROUP(e1->tag),
	       DCM_TAG_ELEMENT(e1->tag), e1->description);
    if (e1->representation != e2->representation) {
	printf(" VR    %04x %04x %32s\n",
	       DCM_TAG_GROUP(e1->tag),
	       DCM_TAG_ELEMENT(e1->tag), e1->description);
	return;
    }
    switch (e1->representation) {
    case DCM_AE:		/* Application Entity */
    case DCM_AS:		/* Age string */
	compareTextAttribute(e1, e2, ctx);
	break;
    case DCM_AT:		/* Attribute tag */
	break;
    case DCM_CS:		/* Control string */
    case DCM_DA:		/* Date */
	compareTextAttribute(e1, e2, ctx);
	break;
    case DCM_DD:		/* Data set */
	break;
    case DCM_DS:		/* Decimal string */
	compareTextAttribute(e1, e2, ctx);
	break;
    case DCM_FD:		/* Floating double */
    case DCM_FL:		/* Float */
	break;
    case DCM_IS:		/* Integer string */
    case DCM_LO:		/* Long string */
    case DCM_LT:		/* Long text */
	compareTextAttribute(e1, e2, ctx);
	break;
    case DCM_OT:		/* Other binary value */
	break;
    case DCM_SH:		/* Short string */
	compareTextAttribute(e1, e2, ctx);
	break;
    case DCM_SL:		/* Signed long */
	compareSLAttribute(e1, e2, ctx);
	break;
    case DCM_SQ:		/* Sequence of items */
	break;
    case DCM_SS:		/* Signed short */
	compareSSAttribute(e1, e2, ctx);
	break;
    case DCM_ST:		/* Short text */
    case DCM_TM:		/* Time */
    case DCM_UI:		/* Unique identifier (UID) */
	compareTextAttribute(e1, e2, ctx);
	break;
    case DCM_UL:		/* Unsigned long */
	compareULAttribute(e1, e2, ctx);
	break;
    case DCM_US:		/* Unsigned short */
	compareUSAttribute(e1, e2, ctx);
	break;
    case DCM_UNKNOWN:		/* Unknown/unspecified (non-standard) */
    case DCM_RET:		/* Retired */
    case DCM_CTX:		/* Context sensitive (non-standard) */
	break;
    case DCM_PN:		/* Person Name */
	compareTextAttribute(e1, e2, ctx);
	break;
    case DCM_OB:		/* Other, byte */
    case DCM_OW:		/* Other, word */
	break;
    case DCM_DT:		/* Date/Time */
	compareTextAttribute(e1, e2, ctx);
	break;
    case DCM_DLM:
	break;
    }
}

static void
compareCallback(const DCM_ELEMENT * e1, const DCM_ELEMENT * e2,
		void *ctx)
{
    if (e1 == NULL) {
	printf(" FILE2 %04x %04x %32s\n", DCM_TAG_GROUP(e2->tag),
	       DCM_TAG_ELEMENT(e2->tag), e2->description);
	(*(long *) ctx)++;
    } else if (e2 == NULL) {
	printf(" FILE1 %04x %04x %32s\n", DCM_TAG_GROUP(e1->tag),
	       DCM_TAG_ELEMENT(e1->tag), e1->description);
	(*(long *) ctx)++;
    } else {
	compareAttribute(e1, e2, ctx);
    }
}

static CONDITION callback(const DCM_ELEMENT* e, void* ctx)
{
  char txt[512] = "";
  printf("<TR>\n");
  printf("<TD>%04x %04x</TD>\n", DCM_TAG_GROUP(e->tag), DCM_TAG_ELEMENT(e->tag));
  printf("<TD ALIGN=RIGHT>%6d</TD>\n", e->length);
  printf("<TD ALIGN=CENTER>%s</TD>\n", e->description);
  if (e->d.ot == NULL) {
    strcpy(txt, "No data");
  } else if (e->length == 0) {
    strcpy(txt, "0 length data");
  } else {
    if (DCM_IsString(e->representation)) {
      int stringLength;
      stringLength = MIN(sizeof(txt)-1, e->length);
      strncpy(txt, e->d.string, stringLength);
      txt[stringLength] = '\0';
    } else {
      switch (e->representation) {
      case DCM_AE:
      case DCM_AS:
	break;
      case DCM_AT:
	sprintf(txt, "%04x %04x",
		DCM_TAG_GROUP(*e->d.ul), DCM_TAG_ELEMENT(*e->d.ul));
	break;
      case DCM_CS:
      case DCM_DA:
	break;
      case DCM_DD:
	strcpy(txt, "Unimplemented");
	break;
      case DCM_DS:
	break;
      case DCM_FD:
      case DCM_FL:
	strcpy(txt, "Unimplemented");
	break;
      case DCM_IS:
      case DCM_LO:
      case DCM_LT:
      case DCM_OT:
      case DCM_SH:
	break;
      case DCM_SL:
	sprintf(txt, "%d", *e->d.sl);
	break;
      case DCM_SQ:
	strcpy(txt, "Unimplemented");
	break;
      case DCM_SS:
	sprintf(txt, "%d", *e->d.ss);
	break;
      case DCM_ST:
      case DCM_TM:
      case DCM_UI:
	break;
      case DCM_UL:
	sprintf(txt, "%d", *e->d.ul);
	break;
      case DCM_US:
	sprintf(txt, "%d", *e->d.us);
	break;
      case DCM_UNKNOWN:
	strcpy(txt, "Unknown");
	break;
      case DCM_RET:
	strcpy(txt, "Retired");
	break;
      case DCM_CTX:
	strcpy(txt, "Context sensitive");
	break;
      case DCM_PN:
	break;
      case DCM_OB:
      case DCM_OW:
	strcpy(txt, "OB or OW");
	break;
      case DCM_DT:
	break;
      case DCM_DLM:
	strcpy(txt, "Delimiter");
	break;
      default:
	break;
      }
    }
  }
  printf("<TD>%s</TD></TR>\n", txt);
  return DCM_NORMAL;
}

main(int argc, char **argv)
{
    DCM_OBJECT *obj1,
       *obj2;
    CONDITION cond;
    CTNBOOLEAN
	verbose = FALSE,
	exitFlag = FALSE;
    unsigned long
        options = DCM_ORDERLITTLEENDIAN;
    long vmLimit = 0;
    long differences = 0;

    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'm':
	    argc--;
	    argv++;
	    vmLimit = atoi(*argv);
	    break;
	case 't':
	    options &= ~DCM_FILEFORMATMASK;
	    options |= DCM_PART10FILE;
	    break;
	case 'v':
	    verbose = TRUE;
	    break;
	case 'z':
	    options |= DCM_FORMATCONVERSION;
	    break;
	default:
	    break;
	}
    }

    THR_Init();
    DCM_Debug(verbose);
    if (argc != 1)
	usageerror();

    cond = DCM_OpenFile(*argv, options, &obj1);
    if (cond != DCM_NORMAL && ((options & DCM_PART10FILE) == 0)) {
	COND_DumpConditions();
	(void) DCM_CloseObject(&obj1);
	(void) COND_PopCondition(TRUE);
	cond = DCM_OpenFile(*argv, options | DCM_PART10FILE, &obj1);
    }
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }

    printf("<HTML>\n");
    printf("<HEAD>\n");
    printf("<META HTTP-EQUIV=""Content-Type"" CONTENT=""text/html; charset=iso-2022-jp"">\n");
    printf("<TITLE>dcm_to_html</TITLE>\n");
    printf("</HEAD>");

    printf("<BODY BGCOLOR=#ffffff FGCOLOR=#ff000000>\n");
    printf("<TABLE BORDER>\n");
    DCM_ScanParseObject(&obj1,
			NULL, 0,           /* Buffer/buffer size */
			NULL, 0,           /* Vector of elements to scan */
			callback, NULL);
    printf("</TABLE>\n");
    printf("</BODY>\n");
    printf("</HTML>");

    (void) DCM_CloseObject(&obj1);;
    THR_Shutdown();
}

#endif
