#ifndef __WORKINGDIALOG_HEADER__
#define __WORKINGDIALOG_HEADER__

static char rcsID_WorkingDialog_h[] = "$Id: WorkingDialog.h,v 1.1 1994-08-09 17:47:25 chander Exp $";


#include <Xm/Xm.h>
#include "SutProto.h"

#ifdef __cplusplus
extern "C" {
#endif

/**** request modes ***/
#define NO_USE 0
#define USE_IT 1
#define NO_WP 2
#define INTERNAL_USE_ONLY 10100

/**** cache usage ****/
#define NO_CACHE 0
#define TO_CACHE 1
#define FROM_CACHE 2

/**** index types ****/
#define ROW_INDEX 0
#define CLEAR_INDEX 1
#define WIDGET_INDEX 2
#define NEXT_INDEX 3
#define PREV_INDEX 4
#define PAGEUP_INDEX 5
#define PAGEDW_INDEX 6
#define HOME_INDEX 7
#define LAST_INDEX 8
#define REFRESH_INDEX 9

/**** queue file flags ****/
#define NO_QUEUE_FILE 0
#define QUEUE_TO_FILE 1

    typedef struct {
	char ***data;		/* 2D array of string pointers, nRows by
				 * nCols */
	long nRows;		/* number of rows in this results group */
	int nCols;		/* number of columns in this results group */
	int *colWidth;		/* actual max col width based on return data */
	int *maxColWidth;	/* column width based on database tables */
	int *colCodes;		/* hook for future growth */
	int *colFormat;		/* hook for future growth */
	char **columnTitles;
	void *options;		/* hook for future growth */
    }   SaDataStruct;

    typedef struct {
	int reason;
	XEvent *event;
	SaDataStruct *groups;	/* array of nGroups SaDataStruct, */
	/* one for each result group */
	int nGroups;		/* number of results groups */
    }   SaCallbackStruct;

/*** Widget, client data, pointer to SaCallbackStruct ***/
    typedef void (*PopulationCallback) SUTPROTO((Widget, XtPointer, SaCallbackStruct *));

    typedef struct SgeWorkCallData *pSgeWorkCallData;

/****** Function Declarations *****************/
    int SgePreInitialize SUTPROTO((int *argc, char **argv));

    pSgeWorkCallData getSgeWorkCallData SUTPROTO((Widget w, long max,
			     char *serv, char *db, char *sproc, int usage));

    void SgeSetCallDetail SUTPROTO((pSgeWorkCallData p, int cache_flag,
		           void *cache_key, int index_type, void *index_val,
				     void *index_key, int queue_file_flag));

    void SgeSetColBind SUTPROTO((pSgeWorkCallData p, XtPointer dest,
				     int group, int order,
			     int size, XtPointer cbfunc, XtPointer client));

    void SgeSetColDetail SUTPROTO((pSgeWorkCallData p, int typeid,
				       int compute_id, int assoc_col,
		       int format_code, char *typename, char *column_name));

    void DoWorkProc SUTPROTO((pSgeWorkCallData p, int OrbReqType));

    void SgeSetArgBind SUTPROTO((pSgeWorkCallData p,
			          int typeid, char *argname, char *argval));

/*** free a cache given a stored procedure triplet. If the cache id ***/
/*** was not NULL or the triplet doesn't match any cache, nothing happens */
/*** WARNING: valid callback structures may point to a cache. ***/
/*** DO NOT free a cache if any references to a cbs exist ***/
    void SgeDestroyCacheByName SUTPROTO((char *server, char *database,
					     char *sproc));

/*** free a cache by its unique id. If the id is not a cache, nothing happens */
/*** WARNING: valid callback structures may point to a cache. ***/
/*** DO NOT free a cache if any references to a cbs exist ***/
    void SgeDestroyCacheById SUTPROTO((void *id));

/**** returns a row number. Only cbs made from cache data will work ***/
/**** any copies of valid cbs will also NOT work ****/
/**** row 0, the first row is returned for any error conditions ***/
    int SgeGetIndexValue SUTPROTO((SaCallbackStruct * cbs, int mode));

/****** Unregisters the client's collection service from the portmapper ****/
/**** and calls exit with the status value. *****/
    void SaExit SUTPROTO((int status));

/****** Unregisters the client's collection service from the portmapper ****/
    void SaUnreg SUTPROTO(());

/****** Reregisters the client's collection service from the portmapper ****/
    void SaRereg SUTPROTO(());

#ifdef __cplusplus
}
#endif

#endif
