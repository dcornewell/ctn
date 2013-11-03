/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	load_sopClasses
**			load_sopclassList
**			formatsop
**                      selected_sop_fromList1
**                      update_selected_sop_inList2
**
**
** Author, Date:	Chander L. Sabharwal, 9-September-94
**
** Intenet:		This program creates linked lists of SOP
**			classes loaded in scrolledList1 and
**			scrolledList2. It also updates the list
**			with options every time a class is selected
**			or deleted from scrolledList2.
**
**
** Last Update:		$Author: smm $, $Date: 2002-10-31 23:17:53 $
** Source File:		$Source: /home/smm/ctn/ctn/apps/assoc_tool/load_sopClasses.c,v $
** Revision:		$Revision: 1.4 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.4 $ $RCSfile: load_sopClasses.c,v $";


#include "ctn_os.h"

#include "dicom.h"
#include "dicom_uids.h"
#include "lst.h"
#include "condition.h"
#include "dulprotocol.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_services.h"
#include "dicom_ie.h"
#include "tbl.h"

#include  "sopclasslist.h"
#include  "prototypes.h"

extern char tSyntaxes[3][DUL_LEN_UID + 1];

static DUL_SC_ROLE role;

/* structure for creating linked list sopclassList*/
/* for scrolledWindow1 */
typedef struct {
    void *reserved[2];
    char sopclass[65];
    char sopclassW[65];
}   SOP_ELEMENT;

extern LST_HEAD *sopList;

/* structure for creating linked list selectedsopclassList*/
/* for scrolledWindow2 */

typedef struct {
    void *resreved[2];
    char selectedsopclass[65];
    char selectedsopclassW[65];
    DUL_SC_ROLE role;
    char tSyntaxes[3][DUL_LEN_UID + 1];
    int flag;
}   SELECTED_SOP;

extern LST_HEAD *selectedsopList;

extern SELECTED_SOP *se;

static char *info;		/* character string contains error code and
				 * error message */

CTNBOOLEAN			/* Function to obtain error messages from the
				 * stack */
errorstackP(unsigned long v, char *mes);

#include "format.h"


/* load_sopclassList
**
** Purpose:
**	This subroutine creates a linked list of SOP
**      classes to be loaded in scrolledList1.
**
** Parameter Dictionary:
**	None
**
** Return Values:
**      None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

void
load_sopclassList()
{
    int i;
    CONDITION cond;
    SOP_ELEMENT *e;

    sopList = LST_Create();
    if (sopList == NULL) {
	printf(" sopList not created\n");
	exit(1);
    }
/* also create selectedsopList */
    selectedsopList = LST_Create();
    if (selectedsopList == NULL) {
	printf(" selectedsopList not created\n");
	exit(1);
    }
    for (i = 0; i < DIM_OF(classes); i++) {
	if ((e = (SOP_ELEMENT *) malloc(sizeof(SOP_ELEMENT))) == NULL) {
	    printf(" malloc e failed\n");
	    exit(1);
	} else {
	    strcpy(e->sopclass, classes[i][0]);
	    strcpy(e->sopclassW, classes[i][1]);
	    cond = LST_Enqueue(&sopList, e);
	    if (cond != LST_NORMAL) {
		(void) COND_ExtractConditions(errorstackP);
		copyWtext2(info);
		(void) COND_PopCondition(TRUE);
	    }
	}
    }

    e = LST_Head(&sopList);
    (void) LST_Position(&sopList, e);

    load(sopList);
}

/* formatsop
** Purpose:
**      This function converts binary string of sopList list
**      to character string.
**
** Parameter Dictionary:
**      index          increments sopList list
**      buf            character array to store ASCII string
**
** Return Values:
**      none
**
** Notes:
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/

void
formatsop(SOP_ELEMENT * e, int index, char *buf)
{
    char *buffer[65];

    (void) sprintf(buf, "%-64s", e->sopclass);

}

/*selected_sop_fromList1
**
** Purpose:
**	This subroutine creates a linked list of SOP classes loaded
**      in scrolledList2.
**
** Parameter Dictionary:
**	item_number_list1  int input item number selected from
**			   scrolledList1 to added in scrolledList2.
**
** Return Values:
**      None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

void
selected_sop_fromList1(int item_number_list1)
{
    int i;
    CONDITION cond;
    SOP_ELEMENT *e;

    e = LST_Head(&sopList);
    (void) LST_Position(&sopList, e);

/* pointer to the next item  */
    for (i = 2; i <= item_number_list1; i++)
	e = LST_Next(&sopList);

    if ((se = (SELECTED_SOP *) malloc(sizeof(SELECTED_SOP))) == NULL) {
	printf(" malloc se failed\n");
	exit(0);
    } else {
	strcpy(se->selectedsopclass, e->sopclass);
	strcpy(se->selectedsopclassW, e->sopclassW);
	se->role = role;
	strcpy(se->tSyntaxes[0], DICOM_TRANSFERLITTLEENDIAN);
	strcpy(se->tSyntaxes[1], "\0");
	strcpy(se->tSyntaxes[2], "\0");
	se->flag = 0;

    }

    cond = LST_Enqueue(&selectedsopList, se);
    if (cond != LST_NORMAL) {
	(void) COND_ExtractConditions(errorstackP);
	copyWtext2(info);
	(void) COND_PopCondition(TRUE);
    }
}

/*update_selected_sop_inList2
**
** Purpose:
**	This subroutine updates the linked list created from
**      scrolledList2 by deleting an item from it.
**
** Parameter Dictionary:
**	itemNo_selected2   int input item number selected from
**			   scrolledList2 to be deleted
**
** Return Values:
**      None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
void
update_selected_sop_inList2(itemNo_selected2)
{
    int i;

    se = LST_Head(&selectedsopList);
    (void) LST_Position(&selectedsopList, se);


    for (i = 1; i < itemNo_selected2; i++) {
	se = LST_Next(&selectedsopList);
    }

    (void) LST_Remove(&selectedsopList, LST_K_BEFORE);
}
