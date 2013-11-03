
/*******************************************************************************
	groupNamesForm.c

       Associated Header file: groupNamesForm.h
*******************************************************************************/

#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <X11/Shell.h>
#include <Xm/MenuShell.h>
#include "UxXt.h"

#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/BulletinB.h>
#include <Xm/PushB.h>
#include <Xm/TextF.h>
#include <Xm/Label.h>
#include <Xm/Form.h>

/*******************************************************************************
       Includes, Defines, and Global variables from the Declarations Editor:
*******************************************************************************/


#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "tbl.h"
#include "manage.h"
#include "mut.h"

#include "format.h"
#include "support.h"

extern DMAN_HANDLE *dmanHandle;
static LST_HEAD *groupNamesList = NULL;
static LST_HEAD *groupGroupList = NULL;
static LST_HEAD *groupTitleList = NULL;

void loadGroupGroupList();
static void loadGroupTitleList(char *g);
static void displayGroup(DMAN_GROUPNAMES * g);
static CONDITION readGroup(DMAN_GROUPNAMES * g);


/*******************************************************************************
       The following header file defines the context structure.
*******************************************************************************/

#define CONTEXT_MACRO_ACCESS 1
#include "groupNamesForm.h"
#undef CONTEXT_MACRO_ACCESS


/*******************************************************************************
Auxiliary code from the Declarations Editor:
*******************************************************************************/

static CONDITION
createLists()
{
    if (groupNamesList == NULL)
	groupNamesList = LST_Create();
    if (groupGroupList == NULL)
	groupGroupList = LST_Create();
    if (groupTitleList == NULL)
	groupTitleList = LST_Create();

    if (groupNamesList == NULL ||
	groupGroupList == NULL ||
	groupTitleList == NULL)
	return 0;
    else
	return 1;
}

void
loadGroupGroupList()
{
    CONDITION
    cond;
    DMAN_GROUPNAMES
	g,
	criteria;
    char
        b[256];

    if (createLists() == 0)
	return;

    memset(&g, 0, sizeof(g));
    g.Type = DMAN_K_GROUPNAMES;
    criteria.Type = DMAN_K_GROUPNAMES;
    criteria.Flag = 0;
    cond = DMAN_Select(&dmanHandle,
		       (DMAN_GENERICRECORD *) & g,
		       (DMAN_GENERICRECORD *) & criteria,
		       groupNamesList, NULL, NULL, NULL);
    if (cond != DMAN_NORMAL) {
	COND_DumpConditions();
	return;
    }
    cond = extractGroups(groupNamesList, groupGroupList);
    if (cond != 1)
	return;

    MUT_LoadList(wGroupGroupList, groupGroupList,
		 formatGroupGroup, b);


    (void) DMAN_ClearList(groupTitleList);
    MUT_LoadList(wGroupTitleList, groupTitleList,
		 formatGroupTitle, b);
}


static void
loadGroupTitleList(char *g)
{
    CONDITION
    cond;
    char b[256];

    (void) DMAN_ClearList(groupTitleList);
    cond = extractTitles(groupNamesList, g, groupTitleList);
    if (cond != 1)
	return;
    MUT_LoadList(wGroupTitleList, groupTitleList,
		 formatGroupTitle, b);
}


static void
displayGroup(DMAN_GROUPNAMES * g)
{
    if (g == NULL)
	return;

    XmTextSetString(wGroupTxt, g->GroupName);
    XmTextSetString(wApplicationTxt, g->Title);
}

static CONDITION
readGroup(DMAN_GROUPNAMES * g)
{
    CONDITION
    cond;
    CTNBOOLEAN
	nullFlag;

    memset(g, 0, sizeof(*g));
    g->Type = DMAN_K_GROUPNAMES;

    cond = MUT_ScanWidget(wGroupTxt, MUT_TEXT, &nullFlag,
			  g->GroupName);
    if (cond != MUT_NORMAL || nullFlag)
	return 0;
    g->Flag |= DMAN_K_GROUP_GROUP;

    cond = MUT_ScanWidget(wApplicationTxt, MUT_TEXT, &nullFlag,
			  g->Title);
    if (cond != MUT_NORMAL || nullFlag)
	return 0;
    g->Flag |= DMAN_K_GROUP_TITLE;

    return 1;
}

/*******************************************************************************
       The following are callback functions.
*******************************************************************************/

static void
activateCB_pushButton10(
			Widget wgt,
			XtPointer cd,
			XtPointer cb)
{
    _UxCgroupNamesForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxGroupNamesFormContext;
    UxGroupNamesFormContext = UxContext =
	(_UxCgroupNamesForm *) UxGetContext(UxWidget);
    {
	DMAN_GROUPNAMES
	    g;
	CONDITION
	    cond;

	cond = readGroup(&g);
	if (cond != 1) {
	    COND_DumpConditions();
	    return;
	}
	cond = DMAN_Insert(&dmanHandle, (DMAN_GENERICRECORD *) & g);
	if (cond != DMAN_NORMAL)
	    COND_DumpConditions();

	loadGroupGroupList();
    }
    UxGroupNamesFormContext = UxSaveCtx;
}

static void
activateCB_pushButton11(
			Widget wgt,
			XtPointer cd,
			XtPointer cb)
{
    _UxCgroupNamesForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxGroupNamesFormContext;
    UxGroupNamesFormContext = UxContext =
	(_UxCgroupNamesForm *) UxGetContext(UxWidget);
    {
	DMAN_GROUPNAMES g;
	CONDITION cond;

	cond = readGroup(&g);
	if (cond != 1) {
	    COND_DumpConditions();
	    return;
	}
	cond = DMAN_Delete(&dmanHandle, (DMAN_GENERICRECORD *) & g);
	if (cond != DMAN_NORMAL)
	    COND_DumpConditions();

	loadGroupGroupList();
	loadGroupTitleList(g.GroupName);
    }
    UxGroupNamesFormContext = UxSaveCtx;
}

static void
browseSelectionCB_wGroupGroupList(
				  Widget wgt,
				  XtPointer cd,
				  XtPointer cb)
{
    _UxCgroupNamesForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxGroupNamesFormContext;
    UxGroupNamesFormContext = UxContext =
	(_UxCgroupNamesForm *) UxGetContext(UxWidget);
    {
	XmListCallbackStruct *l;
	DMAN_GROUPNAMES *g;

	l = (XmListCallbackStruct *) UxCallbackArg;
	g = LST_Index(&groupGroupList, l->item_position);
	if (g == NULL)
	    return;

	loadGroupTitleList(g->GroupName);
    }
    UxGroupNamesFormContext = UxSaveCtx;
}

static void
browseSelectionCB_wGroupTitleList(
				  Widget wgt,
				  XtPointer cd,
				  XtPointer cb)
{
    _UxCgroupNamesForm *UxSaveCtx,
       *UxContext;
    Widget UxWidget = wgt;
    XtPointer UxClientData = cd;
    XtPointer UxCallbackArg = cb;

    UxSaveCtx = UxGroupNamesFormContext;
    UxGroupNamesFormContext = UxContext =
	(_UxCgroupNamesForm *) UxGetContext(UxWidget);
    {
	XmListCallbackStruct *l;
	DMAN_GROUPNAMES *g;

	l = (XmListCallbackStruct *) UxCallbackArg;
	g = LST_Index(&groupTitleList, l->item_position);
	displayGroup(g);
    }
    UxGroupNamesFormContext = UxSaveCtx;
}

/*******************************************************************************
       The 'build_' function creates all the widgets
       using the resource values specified in the Property Editor.
*******************************************************************************/

static Widget
_Uxbuild_groupNamesForm()
{
    Widget _UxParent;


    /* Creation of groupNamesForm */
    _UxParent = UxParent;
    if (_UxParent == NULL) {
	_UxParent = XtVaCreatePopupShell("groupNamesForm_shell",
				       topLevelShellWidgetClass, UxTopLevel,
					 XmNx, 330,
					 XmNy, 380,
					 XmNwidth, 620,
					 XmNheight, 300,
					 XmNshellUnitType, XmPIXELS,
					 XmNtitle, "groupNamesForm",
					 XmNiconName, "groupNamesForm",
					 NULL);

    }
    groupNamesForm = XtVaCreateManagedWidget("groupNamesForm",
					     xmFormWidgetClass,
					     _UxParent,
					     XmNwidth, 620,
					     XmNheight, 300,
					     XmNresizePolicy, XmRESIZE_NONE,
					     XmNunitType, XmPIXELS,
					     NULL);
    UxPutContext(groupNamesForm, (char *) UxGroupNamesFormContext);


    /* Creation of label10 */
    label10 = XtVaCreateManagedWidget("label10",
				      xmLabelWidgetClass,
				      groupNamesForm,
				      XmNx, 30,
				      XmNy, 140,
				      XmNwidth, 190,
				      XmNheight, 30,
				      RES_CONVERT(XmNlabelString, "Group"),
				      NULL);
    UxPutContext(label10, (char *) UxGroupNamesFormContext);


    /* Creation of label11 */
    label11 = XtVaCreateManagedWidget("label11",
				      xmLabelWidgetClass,
				      groupNamesForm,
				      XmNx, 30,
				      XmNy, 180,
				      XmNwidth, 190,
				      XmNheight, 30,
				XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
				      XmNleftOffset, 0,
				      XmNleftWidget, label10,
				      XmNtopAttachment, XmATTACH_WIDGET,
				      XmNtopOffset, 15,
				      XmNtopWidget, label10,
			 RES_CONVERT(XmNlabelString, "Application (Title)"),
				      NULL);
    UxPutContext(label11, (char *) UxGroupNamesFormContext);


    /* Creation of wGroupTxt */
    wGroupTxt = XtVaCreateManagedWidget("wGroupTxt",
					xmTextFieldWidgetClass,
					groupNamesForm,
					XmNwidth, 340,
					XmNx, 260,
					XmNy, 120,
					XmNheight, 40,
					XmNleftAttachment, XmATTACH_WIDGET,
					XmNleftOffset, 15,
					XmNleftWidget, label10,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					XmNtopOffset, 0,
					XmNtopWidget, label10,
					NULL);
    UxPutContext(wGroupTxt, (char *) UxGroupNamesFormContext);


    /* Creation of wApplicationTxt */
    wApplicationTxt = XtVaCreateManagedWidget("wApplicationTxt",
					      xmTextFieldWidgetClass,
					      groupNamesForm,
					      XmNwidth, 340,
					      XmNx, 260,
					      XmNy, 180,
					      XmNheight, 40,
					 XmNleftAttachment, XmATTACH_WIDGET,
					      XmNleftOffset, 15,
					      XmNleftWidget, label10,
				 XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
					      XmNtopOffset, 0,
					      XmNtopWidget, label11,
					      NULL);
    UxPutContext(wApplicationTxt, (char *) UxGroupNamesFormContext);


    /* Creation of pushButton10 */
    pushButton10 = XtVaCreateManagedWidget("pushButton10",
					   xmPushButtonWidgetClass,
					   groupNamesForm,
					   XmNx, 110,
					   XmNy, 240,
					   XmNwidth, 130,
					   XmNheight, 50,
					 RES_CONVERT(XmNlabelString, "Add"),
					   NULL);
    XtAddCallback(pushButton10, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton10,
		  (XtPointer) UxGroupNamesFormContext);

    UxPutContext(pushButton10, (char *) UxGroupNamesFormContext);


    /* Creation of pushButton11 */
    pushButton11 = XtVaCreateManagedWidget("pushButton11",
					   xmPushButtonWidgetClass,
					   groupNamesForm,
					   XmNx, 370,
					   XmNy, 240,
					   XmNwidth, 130,
					   XmNheight, 50,
				      RES_CONVERT(XmNlabelString, "Delete"),
					   NULL);
    XtAddCallback(pushButton11, XmNactivateCallback,
		  (XtCallbackProc) activateCB_pushButton11,
		  (XtPointer) UxGroupNamesFormContext);

    UxPutContext(pushButton11, (char *) UxGroupNamesFormContext);


    /* Creation of bulletinBoard1 */
    bulletinBoard1 = XtVaCreateManagedWidget("bulletinBoard1",
					     xmBulletinBoardWidgetClass,
					     groupNamesForm,
					     XmNresizePolicy, XmRESIZE_NONE,
					     XmNwidth, 600,
					     XmNheight, 110,
					     XmNx, 10,
					     XmNy, 10,
					     XmNnoResize, TRUE,
					     NULL);
    UxPutContext(bulletinBoard1, (char *) UxGroupNamesFormContext);


    /* Creation of scrolledWindowList4 */
    scrolledWindowList4 = XtVaCreateManagedWidget("scrolledWindowList4",
						xmScrolledWindowWidgetClass,
						  bulletinBoard1,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNshadowThickness, 0,
						  XmNx, 10,
						  XmNy, 10,
						  NULL);
    UxPutContext(scrolledWindowList4, (char *) UxGroupNamesFormContext);


    /* Creation of wGroupGroupList */
    wGroupGroupList = XtVaCreateManagedWidget("wGroupGroupList",
					      xmListWidgetClass,
					      scrolledWindowList4,
					      XmNwidth, 280,
					      XmNheight, 90,
					      XmNlistSizePolicy, XmCONSTANT,
					      NULL);
    XtAddCallback(wGroupGroupList, XmNbrowseSelectionCallback,
		  (XtCallbackProc) browseSelectionCB_wGroupGroupList,
		  (XtPointer) UxGroupNamesFormContext);

    UxPutContext(wGroupGroupList, (char *) UxGroupNamesFormContext);


    /* Creation of scrolledWindowList5 */
    scrolledWindowList5 = XtVaCreateManagedWidget("scrolledWindowList5",
						xmScrolledWindowWidgetClass,
						  bulletinBoard1,
				  XmNscrollingPolicy, XmAPPLICATION_DEFINED,
						XmNvisualPolicy, XmVARIABLE,
					XmNscrollBarDisplayPolicy, XmSTATIC,
						  XmNshadowThickness, 0,
						  XmNx, 300,
						  XmNy, 10,
						  NULL);
    UxPutContext(scrolledWindowList5, (char *) UxGroupNamesFormContext);


    /* Creation of wGroupTitleList */
    wGroupTitleList = XtVaCreateManagedWidget("wGroupTitleList",
					      xmListWidgetClass,
					      scrolledWindowList5,
					      XmNwidth, 280,
					      XmNheight, 90,
					      XmNlistSizePolicy, XmCONSTANT,
					      NULL);
    XtAddCallback(wGroupTitleList, XmNbrowseSelectionCallback,
		  (XtCallbackProc) browseSelectionCB_wGroupTitleList,
		  (XtPointer) UxGroupNamesFormContext);

    UxPutContext(wGroupTitleList, (char *) UxGroupNamesFormContext);


    XtAddCallback(groupNamesForm, XmNdestroyCallback,
		  (XtCallbackProc) UxDestroyContextCB,
		  (XtPointer) UxGroupNamesFormContext);


    return (groupNamesForm);
}

/*******************************************************************************
       The following is the 'Interface function' which is the
       external entry point for creating this interface.
       This function should be called from your application or from
       a callback function.
*******************************************************************************/

Widget
create_groupNamesForm(swidget _UxUxParent)
{
    Widget rtrn;
    _UxCgroupNamesForm *UxContext;

    UxGroupNamesFormContext = UxContext =
	(_UxCgroupNamesForm *) UxNewContext(sizeof(_UxCgroupNamesForm), False);

    UxParent = _UxUxParent;

    rtrn = _Uxbuild_groupNamesForm();

    return (rtrn);
}

/*******************************************************************************
       END OF FILE
*******************************************************************************/
