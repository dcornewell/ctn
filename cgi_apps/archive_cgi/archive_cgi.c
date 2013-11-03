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
#include "dicom_ddr.h"
#ifdef CTN_MULTIBYTE
#include "tblmb.h"
#include "idbmb.h"
#else
#include "tbl.h"
#include "idb.h"
#endif
#include "manage.h"
#include "utility.h"

#include "query_interface.h"
#include "queryCTN.h"

static const char* prettyDate(const char* d)
{
  char *monthString[12] = {"Jan", "Feb", "Mar",
			  "Apr", "May", "Jun",
			  "Jul", "Aug", "Sep",
			  "Oct", "Nov", "Dec"};

  static char dateString[20];
  int year = 0, month = 0, day = 0;

  strcpy(dateString, "");

  if (d[0] == '\0') {
    return dateString;
  }

  if (strcmp(d, "00000000") == 0) {
    return dateString;
  }
  if (sscanf(d, "%4d%2d%2d", &year, &month, &day) != 3)
    return dateString;

  if (month < 1 || month > 12) {
    strcpy(dateString, d);
    return dateString;
  }

  sprintf(dateString, "%2d-%3s-%4d",
	  day,
	  monthString[month-1],
	  year);
  return dateString;
}

static void
logMoveRequest(const char* dbKey, const char *studyUID, const char *seriesUID,
		const char *destinationApplication,
		long successes, long warnings, long failures)
{
  FILE *f;
  char *user;
  IDB_Query study;
  char theDate[40], theTime[40];
  CONDITION cond;

  f = fopen("/opt/ctn/agent.log", "a");
  if (f == NULL)
    return;

  user = getenv("REMOTE_USER");
  if (user == NULL)
    user = "Unknown user";

  cond = searchOneStudy(dbKey, "", studyUID, &study);

  UTL_GetDicomDate(theDate);
  UTL_GetDicomTime(theTime);

  fprintf(f, "%s/%s\n", theDate, theTime);

  fprintf(f, "  User (%s) requested move (%s %s %s) to (%s)\n",
	user,
	study.patient.PatNam,
	studyUID, seriesUID,
	destinationApplication);

  fprintf(f, "  Successes = %d, warnings = %d, failures = %d\n",
	successes, warnings, failures);

  fclose(f);
}

LST_HEAD* selectTargetDestinations()
{
  LST_HEAD *lst;
  CONDITION cond;
  DMAN_HANDLE *controlHandle;

  lst = LST_Create();
  if (lst == NULL)
    exit(1);        /* repair */

  cond = openCTN(&controlHandle);
  cond = listOfDestinations(&controlHandle, lst);
  cond = closeCTN(&controlHandle);

  return lst;
}

#if 0
static void patientSearch(llist entries)
{
  CONNECTION_MODULE connection = {"dicomctn", "2100", "RSNA96Medical",
				  "DINPACS"};

  PATIENT_QUERY_MODULE query;
  LST_HEAD *patientList;
  PATIENT_QUERY_MODULE *patientNode;

  memset(&query, 0, sizeof(query));

  strcpy(query.LastName, cgi_val(entries, "lastName"));

  patientList = LST_Create();

  html_header();
  html_begin_body_options("DICOM Patient Search", "bgcolor=#ffffff");

  patientQuery(&connection, &query, &patientList);
  patientNode = LST_Dequeue(&patientList);

  printf("<table cols=3>\n");
  while(patientNode != NULL) {
    printf("<tr><td><a href=""/cgi-bin/acc_agent.cgi?submitSearch=PatientSelect&patientID=%s"">%32s</a>\n",
	   patientNode->PatientID,
	   patientNode->PatientName);
    printf("<td>%s\n", prettyDate(patientNode->BirthDate));
    printf("<td>%s\n", patientNode->PatientID);
    printf("</tr>");

    free(patientNode);
    patientNode = LST_Dequeue(&patientList);
  }
  printf("</table>\n");
  html_end();
  LST_Destroy(&patientList);
}
#else
static void htmlHeaderBody(const char *codeSet, const char* title)
{
  printf("Content-type: text/html\n\n");

  printf("<HTML>");
  printf("<HEAD>\n");
#if 0
  if (strcmp(codeSet, "ISO2022JP") == 0)
#endif
  if (strcmp(codeSet, "EUCJP") == 0)
    printf("<META HTTP-EQUIV-Content-Type CONTENT=text/html, charset=iso-2022-jp>\n");
  else if (strcmp(codeSet, "EUCJPROMAJI") == 0)
    printf("<META HTTP-EQUIV-Content-Type CONTENT=text/html, charset=iso-2022-jp>\n");
  else if (strcmp(codeSet, "EUCKR") == 0)
    printf("<META HTTP-EQUIV-Content-Type CONTENT=text/html, charset=iso-2022-kr>\n");

  printf("<TITLE>%s</TITLE>\n", title);
  printf("</HEAD>\n");
  printf("<BODY BGCOLOR=#ffffff>\n");
}

static void listArchives(llist entries)
{
  char msg[] = "\
The CTN on this system contains a number of virtual archives which\n\
are listed below.  Please select one of the virtual archives for\n\
viewing.  If you wish to have write access (for example, to delete\n\
a study in the archive), enter the password for that archive.\n\
<p>\n\
When you have selected the archive, press the <em>ArchiveSelect</em> button.\n\
<p>\n";

  CONDITION cond;
  DMAN_HANDLE* controlHandle;
  LST_HEAD *archiveList;
  DMAN_STORAGEACCESS *access;

  archiveList = LST_Create();
  TBL_SetOption("OPEN_SPEEDUP");
  cond = openCTN(&controlHandle);
  cond = listOfArchives(&controlHandle, archiveList);
  cond = closeCTN(&controlHandle);

  html_header();
  html_begin_body_options("CTN Archive: List of Archives", "bgcolor=#ffffff");

  printf("%s\n", msg);

#if 0
  printf("<form method=GET action=""/cgi-bin/archive_cgi.cgi"">\n");
#endif
  printf("<form method=GET action=""/cgi-bin/archive_cgi.cgi"">\n");

  access = LST_Dequeue(&archiveList);

  printf("<select name=DBKey size=10>\n");
  while(access != NULL) {
    char tmp[1024];
    sprintf(tmp, "%17s %s", access->Title, access->DbKey);
    printf("<option value=""%s"">%s\n",
	   access->DbKey,
	   access->Title);
    free(access);
    access = LST_Dequeue(&archiveList);
  }
  LST_Destroy(&archiveList);

  printf("</select>\n");
  printf("<br>\n");
  printf("Password (if requesting write access): <input type=text name=password size=15 maxlength=15><br>\n");
  printf("<input type=submit value=""ArchiveSelect"" name=submitSearch>\n");

  printf("</form>\n");
  html_end();
}
#endif

#if 0
static void selectArchive(llist entries)
{
  CONDITION cond;
  DMAN_HANDLE* controlHandle;
  LST_HEAD *studyList;
  char dbKey[64];
  QUERY_LIST_ITEM *item;

  studyList = LST_Create();
  cond = openCTN(&controlHandle);
  strcpy(dbKey, cgi_val(entries, "DBKey"));
  cond = listOfStudies(dbKey, studyList);
  cond = closeCTN(&controlHandle);

  html_header();
  html_begin_body_options("CTN Archive: List of Studies", "bgcolor=#ffffff");

  printf("Please select one archive to see a list of studies<p>\n");

#if 0
  printf("<form method=GET action=""/cgi-bin/archive_cgi.cgi"">\n");
#endif
  printf("<form method=GET action=""/cgi-bin/archive_cgi.cgi"">\n");
  printf("<select name=StudyUID size=10>\n");

  item = LST_Dequeue(&studyList);

  while(item != NULL) {
    printf("<option value=""%s"">%s\n",
	   item->query.study.StuInsUID,
	   item->query.patient.PatNam);
    free(item);
    item = LST_Dequeue(&studyList);
  }

  LST_Destroy(&studyList);

  printf("</select>\n");
  printf("<br>\n");
  printf("<input type=submit value=""ArchiveSelect"" name=submitSearch>\n");

  printf("</form>\n");
  html_end();
}

#else

static void displayPatientList(llist entries, const char* name)
{
  CONDITION cond;
  LST_HEAD *patientList;
  char dbKey[64];
  QUERY_LIST_ITEM *item;
  IDB_Limits limits;
  char password[64];
  char codeSet[80] = "";
  int flag = 0;
  FILE *f1;

  /*f1 = fopen("/tmp/smm", "w");*/

  patientList = LST_Create();
  strcpy(dbKey, cgi_val(entries, "DBKey"));
  strcpy(password, cgi_val(entries, "password"));

  TBL_SetOption("OPEN_SPEEDUP");
  dbCodeSet(dbKey, codeSet);
  cond = listOfPatients(dbKey, codeSet, name, patientList);

  htmlHeaderBody(codeSet, "CTN Archive: List of Patients");

  printf("<TABLE BORDER>\n");
  printf("<TR><TH>Name<TH>ID<TH>Birth Date<TH>Sex</TR>\n");
  item = LST_Dequeue(&patientList);

  while(item != NULL) {
    printf("<TR><TD>"
	   "<a href=""/cgi-bin/archive_cgi.cgi?submitSearch=%s"
	   "&DBKey=%s&password=%s&PatID=%s"">%s</a>"
	   "<TD>%s<TD>%s<TD>%s</TR>\n",
	   "PatientSelect",
	   dbKey,
	   password,
#ifdef CTN_MULTIBYTE
	   item->query.patient.InternalID,
	   item->query.patient.patNam.personName,
	   item->query.patient.patID.text,
#else
	   item->query.patient.PatID,
	   item->query.patient.PatNam,
	   item->query.patient.PatID,
#endif

	   item->query.patient.PatBirDat,
	   item->query.patient.PatSex);

    /*fprintf(f1, "%s\n", item->query.patient.patNam.personName);*/

    free(item);
    item = LST_Dequeue(&patientList);
  }
  printf("</TABLE>\n");

  LST_Destroy(&patientList);

  html_end();
}

static void displayPatientStudies(llist entries)
{
  CONDITION cond;
  LST_HEAD *studyList;
  char dbKey[64];
  QUERY_LIST_ITEM *item;
  IDB_Query patient;
  IDB_Limits limits;
  char password[64];
  char patID[250] = "";
  char codeSet[80] = "";
  int flag = 0;

  studyList = LST_Create();
  strcpy(dbKey, cgi_val(entries, "DBKey"));
  strcpy(password, cgi_val(entries, "password"));
  strcpy(patID, cgi_val(entries, "PatID"));

  TBL_SetOption("OPEN_SPEEDUP");
  dbCodeSet(dbKey, codeSet);

  cond = searchOnePatient(dbKey, codeSet, patID, &patient);
  cond = listOfPatientStudies(dbKey, codeSet, patID, studyList);
  /*cond = listOfStudies(dbKey, studyList);*/

  htmlHeaderBody(codeSet, "CTN Archive: List of Studies");
  printf("<TABLE BORDER>\n");
#ifdef CTN_MULTIBYTE
  printf("<TR><TH>Name<TH>%s</TR>\n", patient.patient.patNam.personName);
  printf("<TR><TH>ID<TH>%s</TR>\n", patient.patient.patID.text);
  printf("<TR><TH>Birth Date<TH>%s</TR>\n", patient.patient.PatBirDat);
#else
  printf("<TR><TH>Name<TH>%s</TR>\n", patient.patient.PatNam);
  printf("<TR><TH>ID<TH>%s</TR>\n", patient.patient.PatID);
  printf("<TR><TH>Birth Date<TH>%s</TR>\n", patient.patient.PatBirDat);
#endif
  printf("<TR><TH>Sex<TH>%s</TR>\n", patient.patient.PatSex);
  printf("</TABLE>\n");
  printf("<P>\n");


  printf("<TABLE BORDER>\n");
  printf("<TR><TH>Date<TH>Time<TH>Accession<TH>Description</TR>\n");
  item = LST_Dequeue(&studyList);
  while(item != NULL) {
    printf("<TR><TD>"
	   "<a href=""/cgi-bin/archive_cgi.cgi?submitSearch=%s"
	   "&DBKey=%s&password=%s&StudyUID=%s"">%s</a>"
	   "<TD>%s<TD>%s<TD>%s</TR>\n",
	   "StudySelect",
	   dbKey,
	   password,
	   item->query.study.StuInsUID,
	   item->query.study.StuDat,

	   item->query.study.StuTim,
#ifdef CTN_MULTIBYTE
	   item->query.study.accNum.text,
	   item->query.study.stuDes.text);
#else
	   item->query.study.AccNum,
	   item->query.study.StuDes);
#endif

    free(item);
    item = LST_Dequeue(&studyList);
  }
  printf("</TABLE>\n");

  LST_Destroy(&studyList);

  html_end();
}

static void displaySeries(llist entries)
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
  char codeSet[80] = "";


  strcpy(dbKey, cgi_val(entries, "DBKey"));
  strcpy(studyUID, cgi_val(entries, "StudyUID"));
  strcpy(password, cgi_val(entries, "password"));
  dbCodeSet(dbKey, codeSet);

  TBL_SetOption("OPEN_SPEEDUP");
  cond = searchOneStudy(dbKey, codeSet, studyUID, &study);

  imageList = LST_Create();
  seriesList = LST_Create();
  cond = searchImages(dbKey, studyUID, imageList);
  while ((item = LST_Dequeue(&imageList)) != NULL) {

    instancePtr = LST_Dequeue(&(item->query.image.InstanceList));

    if (instancePtr != NULL)
      byteCount += instancePtr->Size;
  }

  htmlHeaderBody(codeSet, "CTN Archive: Study Management");

  printf("<table>\n");
#ifdef CTN_MULTIBYTE
  printf("<TR><TH>Name: <TH>%s</TR>\n",
	 study.patient.patNam.personName);
  printf("<TR><TH>Date of Birth: <TH>%s</TR>\n",
	 study.patient.PatBirDat);
  printf("<TR><TH>Accession Number: <TH>%s</TR>\n",
	 study.study.accNum.text);
  printf("<TR><TH>Study Date: <TH>%s</TR>\n",
	 study.study.StuDat);
  printf("<TR><TH>Study Description: <TH>%s</TR>\n",
	 study.study.stuDes.text);
#else
  printf("<TR><TH>Name: <TH>%s</TR>\n",
	 study.patient.PatNam);
  printf("<TR><TH>Date of Birth: <TH>%s</TR>\n",
	 study.patient.PatBirDat);
  printf("<TR><TH>Accession Number: <TH>%s</TR>\n",
	 study.study.AccNum);
  printf("<TR><TH>Study Date: <TH>%s</TR>\n",
	 study.study.StuDat);
  printf("<TR><TH>Study Description: <TH>%s</TR>\n",
	 study.study.StuDes);
#endif
  printf("<TR><TH>Series: <TH>%d</TR>\n",
	 study.study.NumStuRelSer);
  printf("<TR><TH>Images: <TH>%d</TR>\n",
	 study.study.NumStuRelIma);
  printf("<TR><TH>Image Volume: <TH>%d bytes</TR>\n",
	 byteCount);
  printf("</table>\n");

  printf("<form method=GET action=""archive_cgi.cgi"">\n");
  printf("<input type=hidden name=DBKey value=%s>\n", dbKey);
  printf("<input type=hidden name=StudyUID value=%s>\n", studyUID);

/*********/
  printf("<TABLE BORDER>\n");
  printf("<TR><TH>Number<TH>Images<TH>Protocol<TH>Description<TH>Body Part</TR>\n");

  cond = searchSeries(dbKey, codeSet, studyUID, seriesList);
  orderSeriesListByNumber(seriesList);
  item = LST_Dequeue(&seriesList);
  while(item != NULL) {
    char tmp[1024];
    char* protocolName = "[]";
    char* seriesDescription = "[]";
    char* bodyPartExamined = "[]";

#ifdef CTN_MULTIBYTE
    if ((item->query.SeriesNullFlag & QF_SER_ProNam) == 0)
      protocolName = item->query.series.proNam.text;

    if ((item->query.SeriesNullFlag & QF_SER_SerDes) == 0)
      seriesDescription = item->query.series.serDes.text;
#else
    if ((item->query.SeriesNullFlag & QF_SER_ProNam) == 0)
      protocolName = item->query.series.ProNam;

    if ((item->query.SeriesNullFlag & QF_SER_SerDes) == 0)
      seriesDescription = item->query.series.SerDes;
#endif

    if ((item->query.SeriesNullFlag & QF_SER_BodParExa) == 0)
      bodyPartExamined = item->query.series.BodParExa;

    printf("<TR><TH>"
	   "<a href=""/cgi-bin/archive_cgi.cgi?submitSearch=%s&DBKey=%s"
	   "&password=%s&StudyUID=%s&SeriesUID=%s"">"
	   "%s<TH>%d<TH>%s<TH>%s<TH>%s</TR>\n",
	   "SeriesSelect",
	   dbKey,
	   password,
	   studyUID,
	   item->query.series.SerInsUID,

	   item->query.series.SerNum,
	   item->query.series.NumSerRelIma,
	   protocolName,
	   seriesDescription,
	   bodyPartExamined);

    free(item);
    item = LST_Dequeue(&seriesList);
  }
  printf("</TABLE>\n");
/**********/

  if (strcmp(password, "READONLY") != 0) {
    printf("<input type=submit value=""Delete_Study"" name=submitSearch>\n");
  }
  printf("</form>\n");

  html_end();
}

static void displaySeriesImages(llist entries)
{
  CONDITION cond;
  char dbKey[64];
  char studyUID[65];
  char seriesUID[65];
  IDB_Query study;
  LST_HEAD *imageList;
  LST_HEAD *seriesList;
  QUERY_LIST_ITEM *item;
  IDB_InstanceListElement *instancePtr;
  unsigned long byteCount = 0;
  char password[64];
  char codeSet[80] = "";
  int column = 0;


  strcpy(dbKey, cgi_val(entries, "DBKey"));
  strcpy(studyUID, cgi_val(entries, "StudyUID"));
  strcpy(seriesUID, cgi_val(entries, "SeriesUID"));
  strcpy(password, cgi_val(entries, "password"));
  dbCodeSet(dbKey, codeSet);

  TBL_SetOption("OPEN_SPEEDUP");
  cond = searchOneStudy(dbKey, codeSet, studyUID, &study);

  imageList = LST_Create();
  seriesList = LST_Create();
  cond = searchImagesBySeries(dbKey, codeSet, seriesUID, imageList);
  while ((item = LST_Dequeue(&imageList)) != NULL) {

    instancePtr = LST_Dequeue(&(item->query.image.InstanceList));

    if (instancePtr != NULL)
      byteCount += instancePtr->Size;
  }

  htmlHeaderBody(codeSet, "CTN Archive: Images in a Series");

  printf("<table>\n");
#ifdef CTN_MULTIBYTE
  printf("<TR><TH>Name: <TH>%s</TR>\n",
	 study.patient.patNam.personName);
  printf("<TR><TH>Date of Birth: <TH>%s</TR>\n",
	 study.patient.PatBirDat);
  printf("<TR><TH>Accession Number: <TH>%s</TR>\n",
	 study.study.accNum.text);
  printf("<TR><TH>Study Date: <TH>%s</TR>\n",
	 study.study.StuDat);
  printf("<TR><TH>Study Description: <TH>%s</TR>\n",
	 study.study.stuDes.text);
#else
  printf("<TR><TH>Name: <TH>%s</TR>\n",
	 study.patient.PatNam);
  printf("<TR><TH>Date of Birth: <TH>%s</TR>\n",
	 study.patient.PatBirDat);
  printf("<TR><TH>Accession Number: <TH>%s</TR>\n",
	 study.study.AccNum);
  printf("<TR><TH>Study Date: <TH>%s</TR>\n",
	 study.study.StuDat);
  printf("<TR><TH>Study Description: <TH>%s</TR>\n",
	 study.study.StuDes);
#endif

  printf("<TR><TH>Series: <TH>%d</TR>\n",
	 study.study.NumStuRelSer);
  printf("<TR><TH>Images: <TH>%d</TR>\n",
	 study.study.NumStuRelIma);
  printf("<TR><TH>Image Volume: <TH>%d bytes</TR>\n",
	 byteCount);
  printf("</table>\n");

  printf("<form method=GET action=""archive_cgi.cgi"">\n");
  printf("<input type=hidden name=DBKey value=%s>\n", dbKey);
  printf("<input type=hidden name=StudyUID value=%s>\n", studyUID);

/*********/
  printf("<TABLE BORDER>\n");
  printf("<TR><TH COLSPAN=8>Images Listed by Image Number</TR>\n");
  column = 1;
  cond = searchImagesBySeries(dbKey, codeSet, seriesUID, imageList);
  orderImageListByNumber(imageList);

  item = LST_Dequeue(&imageList);
  while(item != NULL) {

    if (column == 1)
      printf("<TR>");

    printf("<TH>"
	   "<a href=""/cgi-bin/archive_cgi.cgi?submitSearch=%s"
	   "&DBKey=%s"
	   "&password=%s"
	   "&SeriesUID=%s"
	   "&ImageUID=%s"">"
	   "%s</a>\n",
	   "ImageSelect",
	   dbKey,
	   password,
	   seriesUID,
	   item->query.image.SOPInsUID,
	   item->query.image.ImaNum);

    free(item);
    item = LST_Dequeue(&imageList);
    column++;
    if (column > 8) {
      printf("</TR>\n");
      column = 1;
    }
  }
  if (column != 1)
    printf("</TR>\n");

  printf("</TABLE>\n");
/**********/

  if (strcmp(password, "READONLY") != 0) {
    printf("<input type=submit value=""Delete_Study"" name=submitSearch>\n");
  }
  printf("</form>\n");

  html_end();
}

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

static void listAttributes(const char* path)
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
    COND_DumpConditions();
    return;
  }

  printf("<TABLE BORDER>\n");
  DCM_ScanParseObject(&obj,
		      NULL, 0,           /* Buffer/buffer size */
		      NULL, 0,           /* Vector of elements to scan */
		      callback, NULL);
  printf("</TABLE>\n");

  (void) DCM_CloseObject(&obj);
  THR_Shutdown();
}

static void displayOneImage(llist entries)
{
  CONDITION cond;
  char dbKey[64];
  char seriesUID[65];
  char imageUID[65];
  IDB_Query image;
  QUERY_LIST_ITEM *item;
  IDB_InstanceListElement *instancePtr = NULL;
  unsigned long byteCount = 0;
  char password[64];
  char codeSet[80] = "";
  int column = 0;


  strcpy(dbKey, cgi_val(entries, "DBKey"));
  strcpy(seriesUID, cgi_val(entries, "SeriesUID"));
  strcpy(imageUID, cgi_val(entries, "ImageUID"));
  strcpy(password, cgi_val(entries, "password"));
  dbCodeSet(dbKey, codeSet);


  TBL_SetOption("OPEN_SPEEDUP");

  cond = searchOneImage(dbKey, codeSet, seriesUID, imageUID, &image);

  instancePtr = LST_Dequeue(&image.image.InstanceList);

  htmlHeaderBody(codeSet, "CTN Archive: Display One Image Attributes");


  if (instancePtr != NULL) {
    printf("<BOLD>%s</BOLD> <P>\n", instancePtr->Path);
    listAttributes(instancePtr->Path);
  }

#if 0
  printf("<table>\n");
  printf("<TR><TH>Name: <TH>%s</TR>\n",
	 study.patient.patNam.personName);
  printf("<TR><TH>Date of Birth: <TH>%s</TR>\n",
	 study.patient.PatBirDat);
  printf("<TR><TH>Accession Number: <TH>%s</TR>\n",
	 study.study.accNum.text);
  printf("<TR><TH>Study Date: <TH>%s</TR>\n",
	 study.study.StuDat);
  printf("<TR><TH>Study Description: <TH>%s</TR>\n",
	 study.study.stuDes.text);
  printf("<TR><TH>Series: <TH>%d</TR>\n",
	 study.study.NumStuRelSer);
  printf("<TR><TH>Images: <TH>%d</TR>\n",
	 study.study.NumStuRelIma);
  printf("<TR><TH>Image Volume: <TH>%d bytes</TR>\n",
	 byteCount);
  printf("</table>\n");

#endif

#if 0
  printf("<form method=GET action=""archive_cgi.cgi"">\n");
  printf("<input type=hidden name=DBKey value=%s>\n", dbKey);
  printf("<input type=hidden name=StudyUID value=%s>\n", studyUID);

/*********/
  printf("<TABLE BORDER>\n");
  column = 1;
  cond = searchImagesBySeries(dbKey, codeSet, seriesUID, imageList);
  orderImageListByNumber(imageList);

  item = LST_Dequeue(&imageList);
  while(item != NULL) {

    if (column == 1)
      printf("<TR>");

    printf("<TH>"
	   "<a href=""/cgi-bin/archive_cgi.cgi?submitSearch=%s"
	   "&DBKey=%s"
	   "&password=%s"
	   "&SeriesUID=%s"
	   "&ImageUID=%s"">"
	   "%s</a>",
	   "ImageSelect",
	   dbKey,
	   password,
	   seriesUID,
	   item->query.image.SOPInsUID,
	   item->query.image.ImaNum);

    free(item);
    item = LST_Dequeue(&imageList);
    column++;
    if (column > 8) {
      printf("</TR>\n");
      column = 1;
    }
  }
  if (column != 1)
    printf("</TR>\n");

  printf("</TABLE>\n");
/**********/
#endif

  if (strcmp(password, "READONLY") != 0) {
    printf("<input type=submit value=""Delete_Study"" name=submitSearch>\n");
  }
  printf("</form>\n");

  html_end();
}


static void selectArchive(llist entries)
{
  CONDITION cond;
  LST_HEAD *studyList;
  char dbKey[64];
  QUERY_LIST_ITEM *item;
  IDB_Limits limits;
  char password[64];
  char codeSet[80] = "";
  int flag = 0;

  studyList = LST_Create();
  strcpy(dbKey, cgi_val(entries, "DBKey"));
  strcpy(password, cgi_val(entries, "password"));

  TBL_SetOption("OPEN_SPEEDUP");
  dbCodeSet(dbKey, codeSet);
  cond = listOfStudies(dbKey, studyList);
  orderStudyListByName(studyList);

  flag = passwordCompare(dbKey, password);
  if (flag == 0)
    strcpy(password, "READONLY");

  memset(&limits, 0, sizeof(limits));
  cond = dbLimits(dbKey, &limits);

  htmlHeaderBody(codeSet, "CTN Archive: List of Studies");
/*
  html_header();
  html_begin_body_options("CTN Archive: List of Studies", "bgcolor=#ffffff");
*/

  if (flag == 0) {
    printf("Read only access granted <br>\n");
  }

  printf("CTN Archive: <BOLD>%s</BOLD> <br> \n", dbKey);
  { char *remoteUser;

    remoteUser = getenv("REMOTE_USER");
    if (remoteUser == NULL)
      remoteUser = "UNKNOWN";

    printf("Remote user: <BOLD>%s</BOLD><br>\n", remoteUser);
  }

#if 0
  printf("%d patients, %d studies, %d images <br>\n",
	 limits.PatientCount,
	 limits.StudyCount,
	 limits.ImageCount);
  printf("DB Size/Limits (MB): %d/%d<br>\n",
	 limits.DBSize/1000000,
	 limits.DBLimit);
#endif
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
    printf("<a href=""/cgi-bin/archive_cgi.cgi?submitSearch=%s&DBKey=%s&StudyUID=%s"">%s</a><br>\n",
#endif

    printf(
"<a href=""/cgi-bin/archive_cgi.cgi?submitSearch=%s&DBKey=%s&\
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

  printf("<form method=GET action=""archive_cgi.cgi"">\n");
  printf("<input type=hidden name=DBKey value=%s>\n", dbKey);
  printf("<input type=hidden name=StudyUID value=%s>\n", studyUID);

/*********/
  printf("<pre>\n");
  cond = searchSeries(dbKey, "", studyUID, seriesList);
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
"<a href=""/cgi-bin/archive_cgi.cgi?submitSearch=%s&DBKey=%s&\
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

  cond = searchOneStudy(dbKey, "", studyUID, &study);

  html_header();
  html_begin_body_options("CTN Archive: Study Management", "bgcolor=#ffffff");

  printf("<table>\n");
  printf("<TR><TH>Name: <TH>%s</TR>\n", study.patient.PatNam);
  printf("<TR><TH>Date of Birth: <TH>%s</TR>\n", study.patient.PatBirDat);
  printf("<TR><TH>Accession Number: <TH>%s</TR>\n", study.study.AccNum);
  printf("</table>\n");

#endif
}

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

  printf("<form method=GET action=""archive_cgi.cgi"">\n");
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


#if 0
static void patientSelect(llist entries)
{
  CONNECTION_MODULE connection = {"dicomctn", "2100", "RSNA96Medical",
				  "DINPACS"};

  PATIENT_QUERY_MODULE query;
  LST_HEAD *studyList;
  STUDY_QUERY_MODULE *studyNode;
  int i;

  memset(&query, 0, sizeof(query));

  strcpy(query.PatientID, cgi_val(entries, "patientID"));

  studyList = LST_Create();

  html_header();
  html_begin_body_options("DICOM Study Search", "bgcolor=#ffffff");

  studyQuery(&connection, &query, &studyList);
  studyNode = LST_Dequeue(&studyList);
  printf("<tt>\n");
  while(studyNode != NULL) {
    printf("%s %s %s %s",
	   studyNode->StudyDate,
	   studyNode->AccessionNumber,
	   studyNode->StudyID,
	   studyNode->StudyDescription);
    for (i = 0; i < 10; i++) {
      if (studyNode->SeriesModalities[i][0] != '\0')
	printf(" %s", studyNode->SeriesModalities[i]);
    }
    printf("<br>\n");

    free(studyNode);
    studyNode = LST_Dequeue(&studyList);
  }
  printf("</tt>\n");

  html_end();

}
#endif


static void retrieveStudies(llist entries)
{
  html_header();
  html_begin_body_options("DICOM Study Search", "bgcolor=#ffffff");
  printf("%s<p>\n", cgi_val(entries, "patientID"));

  html_end();

}

/* Entries for Local Directory Operations */

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


int main()
{
  llist entries;
  int status;
  char *form;

  putenv("PGUSER=postgres");
  status = read_cgi_input(&entries);
  form = cgi_val(entries, "submitSearch");
  if (strcmp(form, "ListArchives") == 0)
    listArchives(entries);
  else if (strcmp(form, "ArchiveSelect") == 0)
    displayPatientList(entries, "");
  else if (strcmp(form, "PatientSelect") == 0)
    displayPatientStudies(entries);
  else if (strcmp(form, "StudySelect") == 0)
    displaySeries(entries);
  else if (strcmp(form, "Delete_Study") == 0)
    deleteStudy(entries);
  else if (strcmp(form, "SeriesSelect") == 0)
    displaySeriesImages(entries);
  else if (strcmp(form, "ImageSelect") == 0)
    displayOneImage(entries);
  else if (strcmp(form, "MoveSeries") == 0)
    moveSeries(entries);
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
