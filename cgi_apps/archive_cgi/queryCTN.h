
typedef struct {
  void *reserved[2];
  IDB_Query query;
} QUERY_LIST_ITEM;

CONDITION openCTN(DMAN_HANDLE** controlHandle);
CONDITION closeCTN(DMAN_HANDLE** controlHandle);
CONDITION listOfArchives(DMAN_HANDLE** controlHandle, LST_HEAD* lst);
CONDITION listOfDestinations(DMAN_HANDLE** controlHandle, LST_HEAD *lst);
CONDITION listOfStudies(const char* dbKey, LST_HEAD *studyList);
CONDITION listOfPatients(const char* dbKey, const char* codeSet,
			 const char* name, LST_HEAD* patientList);
CONDITION listOfPatientStudies(const char* dbKey, const char* codeSet,
			 const char* patientID, LST_HEAD* studyList);
CONDITION searchOnePatient(const char* dbKey, const char* codeSet,
			const char* internalID, IDB_Query *response);
CONDITION searchOneStudy(const char* dbKey, const char* codeSet,
			 const char* studyUID, IDB_Query *response);
CONDITION searchSeries(const char* dbKey, const char* codeSet,
		       const char* studyUID, LST_HEAD* seriesList);
CONDITION searchOneImage(const char* dbKey, const char* codeSet,
			 const char* seriesUID,
			 const char* imageUID, IDB_Query *response);
CONDITION searchOneSeries(const char* dbKey, const char* studyUID,
			 const char* seriesUID, IDB_Query *response);
CONDITION moveSeriesRequest(const char* dbKey, const char* studyUID,
			const char *seriesUID, const char* destinationTitle,
			long *successes, long *warnings, long *failures);
CONDITION deleteOneStudy(const char* dbKey, const char* studyUID);
CONDITION searchImages(const char* dbKey, const char* studyUID,
			LST_HEAD *imageList);
CONDITION searchImagesBySeries(const char* dbKey, const char* codeSet,
			       const char* seriesUID, LST_HEAD *imageList);
CONDITION dbLimits(const char* dbKey, IDB_Limits *limits);
int  passwordCompare(const char *dbKey, const char *password);
void orderStudyListByName(LST_HEAD *lst);
void orderSeriesListByNumber(LST_HEAD *lst);
void orderImageListByNumber(LST_HEAD *lst);
int dbCodeSet(const char* dbKey, char* codeSet);
