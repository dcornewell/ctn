#ifndef __SAPOPCALLS_HEADER__
#define __SAPOPCALLS_HEADER__

static char rcsID_SaPopCalls_h[] = "$Id: SaPopCalls.h,v 1.1 1994-08-10 19:41:09 smm Exp $";


#include "WorkingDialog.h"

#define PROCESSBRSREQUEST_DSQL 300

#ifdef __cplusplus
extern "C" {
#endif

/************************ DATA RETRIEVAL CALLS **************************/

/*** returns a malloc'd string which must be free'd with free ***/
/*** the string is created from the row'th row ***/
    char *SaMakeStringFromThisRow SUTPROTO((SaCallbackStruct * cbs, int row));

/*** returns a malloc'd string which must be free'd with free ***/
/*** the string is created from the indexed row in the cache ***/
    char *SaMakeStringFromIndexRow SUTPROTO((SaCallbackStruct * cbs));

/*** returns a malloc'd string which must be free'd with free ***/
/*** the string is created from all rows with a return char ****/
/*** separating the rows in the string ****/
    char *SaMakeStringFromCBS SUTPROTO((SaCallbackStruct * cbs));

/*** returns a malloc'd XmString which must be free'd with XmStringFree ***/
/*** the XmString is created from the indexed row in the cache ***/
    XmString SaMakeXmStringFromIndexRow SUTPROTO((SaCallbackStruct * cbs));


/************************ WIDGET POPULATION CALLS **************************/

/*** This populates a List Widget or subclass thereof. If used with ***/
/*** any other, the call returns and does nothing ***/
    void SaPopulateList SUTPROTO((Widget list, XtPointer client_data,
				      SaCallbackStruct * cbs));

/*** This populates a scrollBar Widget. If used with ***/
/*** any other, the call returns and does nothing ***/
    void SaPopulateIntValue SUTPROTO((Widget wid, XtPointer client_data,
				          SaCallbackStruct * cbs));

/*** This populates a File Selection Box Widget or Dialog. If used with ***/
/*** any other, the call returns and does nothing ***/
    void SaPopulateFileSelectionBox SUTPROTO((Widget box, XtPointer client_data,
					          SaCallbackStruct * cbs));

/*** This populates a Selection Box  or Prompt Dialog. If used with ***/
/*** any other, the call returns and does nothing ***/
    void SaPopulateSelectionBox SUTPROTO((Widget box, XtPointer client_data,
					      SaCallbackStruct * cbs));

/*** This populates a Message Box  or a derived Dialog. If used with ***/
/*** any other, the call returns and does nothing ***/
    void SaPopulateMessageBox SUTPROTO((Widget mbox, XtPointer client_data,
					    SaCallbackStruct * cbs));

/*** This populates a Scale Widget or subclass thereof. If used with ***/
/*** any other, the call returns and does nothing ***/
    void SaPopulateScale SUTPROTO((Widget scale, XtPointer client_data,
				       SaCallbackStruct * cbs));

/*** This populates a Text Widget or subclass thereof. If used with ***/
/*** any other, the call returns and does nothing ***/
    void SaPopulateTextWithIndexRow SUTPROTO((Widget text, XtPointer client_data,
					          SaCallbackStruct * cbs));

/*** This populates a Text Widget or subclass thereof. If used with ***/
/*** any other, the call returns and does nothing ***/
    void SaPopulateTextAppendIndexRow SUTPROTO((Widget text, XtPointer client_data,
						    SaCallbackStruct * cbs));

/*** This populates a Text Widget or subclass thereof. If used with ***/
/*** any other, the call returns and does nothing ***/
    void SaPopulateTextAppendCBS SUTPROTO((Widget text, XtPointer client_data,
					       SaCallbackStruct * cbs));

/*** This populates a Text Widget or subclass thereof. If used with ***/
/*** any other, the call returns and does nothing ***/
    void SaPopulateText SUTPROTO((Widget text, XtPointer client_data,
				      SaCallbackStruct * cbs));

/*** This populates a TextField Widget or subclass thereof. If used with ***/
/*** any other, the call returns and does nothing ***/
    void SaPopulateTextField SUTPROTO((Widget text, XtPointer client_data,
				           SaCallbackStruct * cbs));

/*** This populates a Shell Widget by altering its Title. If used with ***/
/*** any other, the call returns and does nothing ***/
    void SaPopulateTitle SUTPROTO((Widget text, XtPointer client_data,
				       SaCallbackStruct * cbs));

/*** This populates a topLevelShell and applicationShell by altering its ***/
/*** Title and iconName. If used with ***/
/*** any other, the call returns and does nothing ***/
    void SaPopulateTitleAndIcon SUTPROTO((Widget text, XtPointer client_data,
					      SaCallbackStruct * cbs));

/*** This populates a BulletinBoard Widget or Form Widget. If used with ***/
/*** any other, the call returns and does nothing ***/
    void SaPopulateDialogTitle SUTPROTO((Widget text, XtPointer client_data,
					     SaCallbackStruct * cbs));

/*** This populates several Widget classes. They are : ***/
/*** PushButton, PushButtonGadget, ToggleButton, ToggleButtonGadget, ***/
/*** Label, and LabelGadget. If used with any other, the call returns ***/
/*** and does nothing ***/
    void SaPopulateLabel SUTPROTO((Widget label, XtPointer client_data,
				       SaCallbackStruct * cbs));

/*** This populates an option menu when the rowcolumn of the option menu ***/
/*** is specified.  New widgets of type xmPushButtonWidgetClass are      ***/
/*** created or deleted as necessary to accomodate data.  Each pushButton***/
/*** labelString will contain one occurance of the bound column.  If     ***/
/*** more than one column is bound, they are ignored. The number of rows ***/
/*** and columns will be determined dynamically based upon the number of ***/
/*** pushButtons to manage.  An option menu should be used for choosing  ***/
/*** one of a few options.  Large or unknown numbers of choices should   ***/
/*** implement a scrolled list instead.                                  ***/
    void SaPopulateMenu SUTPROTO((Widget prc, XtPointer client_data,
				      SaCallbackStruct * cbs));

    void SaPopulateMatrix SUTPROTO((Widget matrix, XtPointer client_data,
				        SaCallbackStruct * cbs));

    void SaPopulateXRTgraph SUTPROTO((Widget xrt, XtPointer client_data,
				          SaCallbackStruct * cbs));

    void SaPopulateXRT3d SUTPROTO((Widget xrt3d, XtPointer client_data,
				       SaCallbackStruct * cbs));

/*** prints a cbs to standard out ***/
    void SaPrintOutCBS SUTPROTO((Widget w, XtPointer client_data,
				     SaCallbackStruct * cbs));

/*** prints a cbs to standardd error ***/
    void SaPrintOutCBStoStdErr SUTPROTO((Widget w, XtPointer client_data,
					     SaCallbackStruct * cbs));

/*** appends a cbs to the Sa Console interface *****/
    void SaPrintCBStoSaConsole SUTPROTO((Widget w, XtPointer client_data,
					     SaCallbackStruct * cbs));

/*** assigns to the int pointed to by client_data the total rows returned ***/
    void SaAssignNumRowsToInt SUTPROTO((Widget w, XtPointer client_data,
					    SaCallbackStruct * cbs));

/*** appends the cbs data to a file. The client_data holds the file name. ***/
/*** if the file does not exist, it is created. *****/
    void SaPrintCBStoFile SUTPROTO((Widget w, XtPointer client_data,
				        SaCallbackStruct * cbs));

/*** This function returns a pointer to a default population function ***/
/*** for a specified Widget class. If there is no default, NULL is ***/
/*** returned. A default population callback allows a user to not ***/
/*** have to specify any in the Results Detail editor of the Object ***/
/*** Bind Editor of dbuimx. If one is specified, that is used instead. ***/
    PopulationCallback SaGetDefaultPopulateCallback SUTPROTO((Widget w));



/************************ ARGUMENT EXTRACTION CALLS ************************/

/*** This function returns a pointer to the text in a Text or ***/
/*** TextField Widget.  This must not be FREED. This will be overwritten. ***/
/*** If you need a permanent copy, you must copy it ***/
    char *SaGetText SUTPROTO((Widget text));

/*** This function returns a pointer to String representing the value ***/
/*** resource for scales and scrollBars.  This must not be FREED. This ***/
/*** will be overwritten. ***/
/*** If you need a permanent copy, you must copy it ***/
    char *SaGetValue SUTPROTO((Widget text));

/*** This function returns the string in the rox x column cell. ****/
/*** If the cell is not valid then the selected cell's contents ****/
/*** is returned; if no cell is selected, an empty string is returned ***/
    char *SaGetMatrixCell SUTPROTO((Widget matrix, int row, int col));

/*** This function returns a pointer to the text in a Text or ***/
/*** TextField Widget.  This must not be FREED. This will be overwritten. ***/
/*** If you need a permanent copy, you must copy it ***/
    char *SaGetLabelString SUTPROTO((Widget text));

/*** This function returns a pointer to the Title in a Shell.  ***/
/*** This must not be FREED. This will ***/
/*** be overwritten.  If you need a permanent copy, you must copy it ***/
    char *SaGetTitle SUTPROTO((Widget wid));

/*** This function returns a pointer to the DialogTitle in a Form or ***/
/*** BulletinBoard Manager Widget.  This must not be FREED. This will ***/
/*** be overwritten.  If you need a permanent copy, you must copy it ***/
    char *SaGetDialogTitle SUTPROTO((Widget wid));

/*** This function returns a pointer to the message in a message box. ***/
/*** This must not be FREED. This will be overwritten. ***/
/*** If you need a permanent copy, you must copy it ***/
    char *SaGetMessageString SUTPROTO((Widget message));

/*** This function returns a pointer to the first item in a list's ***/
/***  text in a Text or ***/
/*** TextField Widget.  This must not be FREED. This will be overwritten. ***/
/*** If you need a permanent copy, you must copy it ***/
    char *SaGetListSelection SUTPROTO((Widget text));

/*** This function returns a pointer to a SelectionBox's XmNtextString ***/
/*** resource. ***/
/*** This must not be FREED. This will be overwritten. ***/
/*** If you need a permanent copy, you must copy it ***/
    char *SaGetSelectionBoxChoice SUTPROTO((Widget box));

/*** This function returns a pointer to the OptionLabelGadget's text  ***/
/*** Which holds an Option Menu's selection.  This must not be FREED. ***/
/*** This will be overwritten. If you need a permanent copy, you must ***/
/*** copy it. ***/
    char *SaGetOMenuChoice SUTPROTO((Widget text));

    int SaGetWidgetIndex SUTPROTO((Widget w));

    int SaGetIndexFromList SUTPROTO((Widget w));

    int SaGetIndexFromScale SUTPROTO((Widget w));

    int SaGetIndexFromMenu SUTPROTO((Widget w));

    char *SaGetDefaultCallbackName SUTPROTO((Widget w));

/**
***  Extend Function: ***************************************************
***
***  The purpose of SaGetArgBindCode is to provide string which will be
***  inserted into the Argument Value textField in the Object Bind Editor when
***  the given widget class is dragged and dropped into the same Argument
***  Value textField.  You are responsible for making sure the specified
***  code fragment is available and correct.
**/
    char *SaGetArgBindCode SUTPROTO((Widget w));

#ifdef __cplusplus
}
#endif

#endif
