! UIMX ascii 2.5 key: 9091                                                      

*groupNamesForm.class: form
*groupNamesForm.gbldecl: #include <stdio.h>\
\
#include "dicom.h"\
#include "condition.h"\
#include "lst.h"\
#include "tbl.h"\
#include "manage.h"\
#include "mut.h"\
\
#include "format.h"\
#include "support.h"\
\
extern DMAN_HANDLE *dmanHandle;\
static LST_HEAD *groupNamesList = NULL;\
static LST_HEAD *groupGroupList = NULL;\
static LST_HEAD *groupTitleList = NULL;\
\
void loadGroupGroupList();\
static void loadGroupTitleList(char *g);\
static void displayGroup(DMAN_GROUPNAMES *g);\
static CONDITION readGroup(DMAN_GROUPNAMES *g);
*groupNamesForm.ispecdecl:
*groupNamesForm.funcdecl: swidget create_groupNamesForm(swidget UxParent)
*groupNamesForm.funcname: create_groupNamesForm
*groupNamesForm.funcdef: "swidget", "<create_groupNamesForm>(%)"
*groupNamesForm.argdecl: swidget UxParent;
*groupNamesForm.arglist: UxParent
*groupNamesForm.arglist.UxParent: "swidget", "%UxParent%"
*groupNamesForm.icode:
*groupNamesForm.fcode: return(rtrn);\

*groupNamesForm.auxdecl: static CONDITION createLists()\
{\
    if (groupNamesList == NULL)\
	groupNamesList = LST_Create();\
    if (groupGroupList == NULL)\
	groupGroupList = LST_Create();\
    if (groupTitleList == NULL)\
	groupTitleList = LST_Create();\
\
    if (groupNamesList == NULL ||\
	groupGroupList == NULL ||\
	groupTitleList == NULL)\
	return 0;\
    else\
	return 1;\
}\
\
void loadGroupGroupList()\
{\
    CONDITION\
      cond;\
    DMAN_GROUPNAMES\
	g,\
	criteria;\
    char\
	b[256];\
\
    if (createLists() == 0)\
	return;\
\
    memset(&g, 0, sizeof(g));\
    g.Type = DMAN_K_GROUPNAMES;\
    criteria.Type = DMAN_K_GROUPNAMES;\
    criteria.Flag = 0;\
    cond = DMAN_Select(&dmanHandle,\
	(DMAN_GENERICRECORD *)&g,\
	(DMAN_GENERICRECORD *)&criteria,\
	groupNamesList, NULL, NULL, NULL);\
    if (cond != DMAN_NORMAL) {\
	COND_DumpConditions();\
	return;\
    }\
\
    cond = extractGroups(groupNamesList, groupGroupList);\
    if (cond != 1)\
	return;\
\
    MUT_LoadList(wGroupGroupList,  groupGroupList,\
	formatGroupGroup, b);\
\
\
    (void) DMAN_ClearList(groupTitleList);\
    MUT_LoadList(wGroupTitleList, groupTitleList,\
	formatGroupTitle, b);\
}\
\
\
static void loadGroupTitleList(char *g)\
{\
    CONDITION\
	cond;\
    char b[256];\
\
    (void) DMAN_ClearList(groupTitleList);\
    cond = extractTitles(groupNamesList, g, groupTitleList);\
    if (cond != 1)\
	return;\
    MUT_LoadList(wGroupTitleList, groupTitleList,\
	formatGroupTitle, b);\
}\
\
\
static void displayGroup(DMAN_GROUPNAMES *g)\
{\
    if (g == NULL)\
	return;\
\
    XmTextSetString(wGroupTxt, g->GroupName);\
    XmTextSetString(wApplicationTxt, g->Title);\
}\
\
static CONDITION readGroup(DMAN_GROUPNAMES *g)\
{\
  CONDITION\
    cond;\
  CTNBOOLEAN\
    nullFlag;\
\
    memset(g, 0, sizeof(*g));\
    g->Type = DMAN_K_GROUPNAMES;\
\
    cond = MUT_ScanWidget(wGroupTxt, MUT_TEXT, &nullFlag,\
	g->GroupName);\
    if (cond != MUT_NORMAL || nullFlag)\
	return 0;\
    g->Flag |= DMAN_K_GROUP_GROUP;\
\
    cond = MUT_ScanWidget(wApplicationTxt, MUT_TEXT, &nullFlag,\
	g->Title);\
    if (cond != MUT_NORMAL || nullFlag)\
	return 0;\
    g->Flag |= DMAN_K_GROUP_TITLE;\
\
    return 1;\
}
*groupNamesForm.static: true
*groupNamesForm.name: groupNamesForm
*groupNamesForm.parent: NO_PARENT
*groupNamesForm.parentExpression: UxParent
*groupNamesForm.defaultShell: topLevelShell
*groupNamesForm.width: 620
*groupNamesForm.height: 300
*groupNamesForm.resizePolicy: "resize_none"
*groupNamesForm.isCompound: "true"
*groupNamesForm.compoundIcon: "form.xpm"
*groupNamesForm.compoundName: "form_"
*groupNamesForm.x: 330
*groupNamesForm.y: 380
*groupNamesForm.unitType: "pixels"

*label10.class: label
*label10.static: true
*label10.name: label10
*label10.parent: groupNamesForm
*label10.isCompound: "true"
*label10.compoundIcon: "label.xpm"
*label10.compoundName: "label_"
*label10.x: 30
*label10.y: 140
*label10.width: 190
*label10.height: 30
*label10.labelString: "Group"

*label11.class: label
*label11.static: true
*label11.name: label11
*label11.parent: groupNamesForm
*label11.isCompound: "true"
*label11.compoundIcon: "label.xpm"
*label11.compoundName: "label_"
*label11.x: 30
*label11.y: 180
*label11.width: 190
*label11.height: 30
*label11.leftAttachment: "attach_opposite_widget"
*label11.leftOffset: 0
*label11.leftWidget: "label10"
*label11.topAttachment: "attach_widget"
*label11.topOffset: 15
*label11.topWidget: "label10"
*label11.labelString: "Application (Title)"

*wGroupTxt.class: textField
*wGroupTxt.static: true
*wGroupTxt.name: wGroupTxt
*wGroupTxt.parent: groupNamesForm
*wGroupTxt.width: 340
*wGroupTxt.isCompound: "true"
*wGroupTxt.compoundIcon: "textfield.xpm"
*wGroupTxt.compoundName: "text_Field"
*wGroupTxt.x: 260
*wGroupTxt.y: 120
*wGroupTxt.height: 40
*wGroupTxt.leftAttachment: "attach_widget"
*wGroupTxt.leftOffset: 15
*wGroupTxt.leftWidget: "label10"
*wGroupTxt.topAttachment: "attach_opposite_widget"
*wGroupTxt.topOffset: 0
*wGroupTxt.topWidget: "label10"

*wApplicationTxt.class: textField
*wApplicationTxt.static: true
*wApplicationTxt.name: wApplicationTxt
*wApplicationTxt.parent: groupNamesForm
*wApplicationTxt.width: 340
*wApplicationTxt.isCompound: "true"
*wApplicationTxt.compoundIcon: "textfield.xpm"
*wApplicationTxt.compoundName: "text_Field"
*wApplicationTxt.x: 260
*wApplicationTxt.y: 180
*wApplicationTxt.height: 40
*wApplicationTxt.leftAttachment: "attach_widget"
*wApplicationTxt.leftOffset: 15
*wApplicationTxt.leftWidget: "label10"
*wApplicationTxt.topAttachment: "attach_opposite_widget"
*wApplicationTxt.topOffset: 0
*wApplicationTxt.topWidget: "label11"

*pushButton10.class: pushButton
*pushButton10.static: true
*pushButton10.name: pushButton10
*pushButton10.parent: groupNamesForm
*pushButton10.isCompound: "true"
*pushButton10.compoundIcon: "push.xpm"
*pushButton10.compoundName: "push_Button"
*pushButton10.x: 110
*pushButton10.y: 240
*pushButton10.width: 130
*pushButton10.height: 50
*pushButton10.labelString: "Add"
*pushButton10.activateCallback: {\
  DMAN_GROUPNAMES\
    g;\
  CONDITION\
    cond;\
\
    cond = readGroup(&g);\
    if (cond != 1) {\
	COND_DumpConditions();\
	return;\
    }\
\
    cond = DMAN_Insert(&dmanHandle, (DMAN_GENERICRECORD *)&g);\
    if (cond != DMAN_NORMAL)\
	COND_DumpConditions();\
\
    loadGroupGroupList();\
}

*pushButton11.class: pushButton
*pushButton11.static: true
*pushButton11.name: pushButton11
*pushButton11.parent: groupNamesForm
*pushButton11.isCompound: "true"
*pushButton11.compoundIcon: "push.xpm"
*pushButton11.compoundName: "push_Button"
*pushButton11.x: 370
*pushButton11.y: 240
*pushButton11.width: 130
*pushButton11.height: 50
*pushButton11.labelString: "Delete"
*pushButton11.activateCallback: {\
  DMAN_GROUPNAMES g;\
  CONDITION cond;\
\
    cond = readGroup(&g);\
    if (cond != 1) {\
	COND_DumpConditions();\
	return;\
    }\
\
    cond = DMAN_Delete(&dmanHandle, (DMAN_GENERICRECORD *)&g);\
    if (cond != DMAN_NORMAL)\
	COND_DumpConditions();\
\
    loadGroupGroupList();\
    loadGroupTitleList(g.GroupName);\
}

*bulletinBoard1.class: bulletinBoard
*bulletinBoard1.static: true
*bulletinBoard1.name: bulletinBoard1
*bulletinBoard1.parent: groupNamesForm
*bulletinBoard1.resizePolicy: "resize_none"
*bulletinBoard1.width: 600
*bulletinBoard1.height: 110
*bulletinBoard1.isCompound: "true"
*bulletinBoard1.compoundIcon: "bboard.xpm"
*bulletinBoard1.compoundName: "bulletin_Board"
*bulletinBoard1.x: 10
*bulletinBoard1.y: 10
*bulletinBoard1.noResize: "true"

*scrolledWindowList4.class: scrolledWindow
*scrolledWindowList4.static: true
*scrolledWindowList4.name: scrolledWindowList4
*scrolledWindowList4.parent: bulletinBoard1
*scrolledWindowList4.scrollingPolicy: "application_defined"
*scrolledWindowList4.visualPolicy: "variable"
*scrolledWindowList4.scrollBarDisplayPolicy: "static"
*scrolledWindowList4.shadowThickness: 0
*scrolledWindowList4.isCompound: "true"
*scrolledWindowList4.compoundIcon: "scrllist.xpm"
*scrolledWindowList4.compoundName: "scrolled_List"
*scrolledWindowList4.x: 10
*scrolledWindowList4.y: 10
*scrolledWindowList4.isResizable: "false"

*wGroupGroupList.class: scrolledList
*wGroupGroupList.static: true
*wGroupGroupList.name: wGroupGroupList
*wGroupGroupList.parent: scrolledWindowList4
*wGroupGroupList.width: 280
*wGroupGroupList.height: 90
*wGroupGroupList.isResizable: "false"
*wGroupGroupList.listSizePolicy: "constant"
*wGroupGroupList.browseSelectionCallback: {\
  XmListCallbackStruct *l;\
  DMAN_GROUPNAMES *g;\
\
    l = (XmListCallbackStruct *)UxCallbackArg;\
    g = LST_Index(&groupGroupList, l->item_position);\
    if (g == NULL)\
	return;\
\
    loadGroupTitleList(g->GroupName);\
}

*scrolledWindowList5.class: scrolledWindow
*scrolledWindowList5.static: true
*scrolledWindowList5.name: scrolledWindowList5
*scrolledWindowList5.parent: bulletinBoard1
*scrolledWindowList5.scrollingPolicy: "application_defined"
*scrolledWindowList5.visualPolicy: "variable"
*scrolledWindowList5.scrollBarDisplayPolicy: "static"
*scrolledWindowList5.shadowThickness: 0
*scrolledWindowList5.isCompound: "true"
*scrolledWindowList5.compoundIcon: "scrllist.xpm"
*scrolledWindowList5.compoundName: "scrolled_List"
*scrolledWindowList5.x: 300
*scrolledWindowList5.y: 10
*scrolledWindowList5.isResizable: "false"
*scrolledWindowList5.resizeRecursion: "none"

*wGroupTitleList.class: scrolledList
*wGroupTitleList.static: true
*wGroupTitleList.name: wGroupTitleList
*wGroupTitleList.parent: scrolledWindowList5
*wGroupTitleList.width: 280
*wGroupTitleList.height: 90
*wGroupTitleList.resizeRecursion: "none"
*wGroupTitleList.isResizable: "false"
*wGroupTitleList.listSizePolicy: "constant"
*wGroupTitleList.browseSelectionCallback: {\
  XmListCallbackStruct *l;\
  DMAN_GROUPNAMES *g;\
\
    l = (XmListCallbackStruct *)UxCallbackArg;\
    g = LST_Index(&groupTitleList, l->item_position);\
    displayGroup(g);\
}

