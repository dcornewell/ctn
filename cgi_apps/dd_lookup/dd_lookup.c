/* 
*/

#include <stdio.h>
#include <stdlib.h>
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

static void search(llist entries)
{
  CONDITION cond;
  char groupText[10];
  char elementText[10];
  DCM_TAG tag;
  int group;
  int element;
  DCM_ELEMENT e;

  strcpy(groupText, cgi_val(entries, "group"));
  strcpy(elementText, cgi_val(entries, "element"));
  sscanf(groupText, "%x", &group);
  sscanf(elementText, "%x", &element);
  tag = DCM_MAKETAG(group, element);

  memset(&e, 0, sizeof(e));
  e.tag = tag;
  cond = DCM_LookupElement(&e);
  if (cond != DCM_NORMAL) {
    e.representation = DCM_UNKNOWN;
    strcpy(e.description, "Attribute not in dictionary");
  }

  html_header();
  html_begin_body_options("CTN Element Lookup", "bgcolor=#ffffff");

  printf("<B>\n");
  printf("%04x %04x %s %s <BR>\n",
	group,
	element,
	translate(e.representation),
	e.description);
  printf("</B>\n");

  html_end();
}

static void
elementCallback(DCM_TAG tag, char *description,
		DCM_VALUEREPRESENTATION representation, void *ctx)
{
    DCM_ELEMENT_NODE *n;
    LST_HEAD **l;

    l = (LST_HEAD **) ctx;
    n = calloc(1, sizeof(*n));
    if (n == NULL)
	return;

    n->e.tag = tag;
    strcpy(n->e.description, description);
    n->e.representation = representation;

    (void) LST_Enqueue(l, n);
}

static void
groupCallback(unsigned short group, char *description, void *ctx)
{
    DCM_ELEMENT_NODE *n;
    LST_HEAD **l;

    l = (LST_HEAD **) ctx;
    n = calloc(1, sizeof(*n));
    if (n == NULL)
	return;

    n->e.tag = DCM_MAKETAG(group, 0);
    strcpy(n->e.description, description);

    (void) LST_Enqueue(l, n);
}

static void printDictionary(llist entries)
{

  html_header();
  html_begin_body_options("DICOM CTN: Data Dictionary", "bgcolor=#ffffff");
  printf("<table>\n");
  printf("<TR><TH>Tag <TH>VR <TH ALIGN=LEFT>Description</TR>\n");

  {
    LST_HEAD *l;
    LST_HEAD *elementList;
    CONDITION cond;
    DCM_ELEMENT_NODE* n;
    DCM_ELEMENT_NODE* elementNode;
    int group = 0;

    l = LST_Create();
    if (l == NULL) {
	return;
    }
    elementList = LST_Create();
    if (elementList == NULL) {
	return;
    }
    cond = DCM_GroupDictionary(0xffff, &l, groupCallback);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
    }
    n = LST_Dequeue(&l);
    while (n != NULL) {
	group = DCM_TAG_GROUP(n->e.tag);
	/*printf("%04x  %s\n", group, n->e.description);*/

	cond = DCM_ElementDictionary(DCM_MAKETAG(group, 0xffff), &elementList,
				     elementCallback);
	if (cond != DCM_NORMAL) {
	    COND_DumpConditions();
	}
	elementNode = LST_Dequeue(&elementList);
	while (elementNode != NULL) {
	    printf("<TR><TH>%04x %04x<TH>%s<TH ALIGN=LEFT>%s</TR>\n",
		   DCM_TAG_GROUP(elementNode->e.tag),
		   DCM_TAG_ELEMENT(elementNode->e.tag),
		   translate(elementNode->e.representation),
		   elementNode->e.description);

	    free(elementNode);
	    elementNode = LST_Dequeue(&elementList);
	}

	free(n);
	n = LST_Dequeue(&l);
    }
    LST_Destroy(&l);
    LST_Destroy(&elementList);
  }

  printf("</table>\n");
  html_end();
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

  if (strcmp(form, "Search") == 0)
    search(entries);
  else if (strcmp(form, "Print") == 0)
    printDictionary(entries);
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
