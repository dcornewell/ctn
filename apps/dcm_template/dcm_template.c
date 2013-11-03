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
**                              DICOM 93
**                   Electronic Radiology Laboratory
**                 Mallinckrodt Institute of Radiology
**              Washington University School of Medicine
**
** Module Name(s):	main
**			lookupUID
**			usageerror
** Author, Date:	Pei Weng, 29-June-1993
** Intent:              This program determines which IEs, Modules,
**                      and Attributes are required for an image file
**                      and print the above informatin to the standard
**			output. The user enters the SOP Class name
**			of the image file the user wants to examine in
**			the command line, then a list of required
**			Information Entities, required Modules and
**			mandatory Attributes will be printed to the
**			standard output.
**
** Last Update:         $Author: smm $, $Date: 1998-08-03 21:27:13 $
** Source File:         $RCSfile: dcm_template.c,v $
** Revision:            $Revision: 1.12 $
** Status:              $State: Exp $
*/

static char rcsid[] = "$Revision: 1.12 $ $RCSfile: dcm_template.c,v $";

#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

#include "dicom.h"
#include "ctnthread.h"
#include "condition.h"
#include "lst.h"
#include "dicom_objects.h"
#include "dicom_messages.h"
#include "dicom_ie.h"
#include "dicom_uids.h"

typedef struct {
    char *classUID;
    char *SOPClassName;
}   RULE;

/* table which has the SOP Class name and its corresponding
   SOP Class UID */
static RULE table[] = {
    {DICOM_SOPCLASSCOMPUTEDRADIOGRAPHY, "CR"},
    {DICOM_SOPCLASSCT, "CT"},
    {DICOM_SOPCLASSMR, "MR"},
    {DICOM_SOPCLASSNM, "NM"},
    {DICOM_SOPCLASSUS, "US"},
    {DICOM_SOPCLASSSECONDARYCAPTURE, "SC"}
};

/* Prototypes for internal functions
 */
static char *lookupUID(char *SOPClassName);
static void usageerror();

int
main(int argc, char **argv)
{
    CONDITION			/* Return value from DUL and ACR routines */
    cond;
    IE_OBJECT			/* Handle to the IE_OBJECT object */
	* ieObject;
    IE_INFORMATIONENTITY	/* Handle to IE_INFORMATIONENTITY */
	* ieIE, *ie_node;
    LST_HEAD			/* Handle to the LST_HEAD */
	* ie_head, *mod_head, *attr_head;
    IE_MODULE			/* Handle to IE_MODULE */
	* ieModule, *mod_node;
    IE_ATTRIBUTE		/* Handle to IE_ATTRIBUTE */
	* attr_node;
    CTNBOOLEAN			/* For debugging purpose */
	verbose = FALSE;
    char			/* The UID of the image file */
       *UID,
       *SOPClassName;
    int				/* Iteration variables */
        ie_loop,
        mod_loop,
        attr_loop,
        i,
        k,
        j;


    if (argc < 1)
	usageerror();

    THR_Init();
    DCM_Debug(verbose);

    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'v':
	    verbose = TRUE;
	    break;
	default:
	    break;
	}
    }

    while (argc-- > 0) {
	SOPClassName = *argv;
	(void) *argv++;
	/*
	 * Find the SOP Class UID according to the SOP Class name.
	 */
	UID = lookupUID(SOPClassName);
	if (UID != NULL) {	/* The SOP Class name is legal */
	    printf("\nRequired IEs and Modules for %s image file:\n",
		   SOPClassName);
	    /*
	     * Find the required IEs.
	     */
	    cond = IE_ObjectRequirements(UID, &ieObject);
	    if (cond == IE_LISTFAILURE || cond == IE_MALLOCFAILURE ||
		cond == IE_ILLEGALDCMOBJECT) {
		COND_DumpConditions();
		THR_Shutdown();
		return (2);
	    }
	    ie_head = ieObject->ieList;
	    ie_loop = LST_Count(&ie_head);
	    for (i = 0; i < ie_loop; i++) {
		/*
		 * Find all the required Modules.
		 */
		ie_node = LST_Pop(&ie_head);
		printf("%s\n", ie_node->ieDescription);
		cond = IE_IERequirements(UID, ie_node->ieType, &ieIE);
		mod_head = ieIE->moduleList;
		mod_loop = LST_Count(&mod_head);
		for (k = 0; k < mod_loop; k++) {
		    mod_node = LST_Pop(&mod_head);
		    printf("  %s\n", mod_node->moduleDescription);
		}
		cond = IE_Free((void **) &ieIE);
	    }
	    cond = IE_Free((void **) &ieObject);

	    printf("\n");
	    printf("Required Modules and Attributes for %s image file:\n",
		   SOPClassName);
	    /*
	     * Find all the required IEs.
	     */
	    cond = IE_ObjectRequirements(UID, &ieObject);
	    if (cond == IE_LISTFAILURE || cond == IE_MALLOCFAILURE ||
		cond == IE_ILLEGALDCMOBJECT) {
		COND_DumpConditions();
		THR_Shutdown();
		return (2);
	    }
	    ie_head = ieObject->ieList;
	    ie_loop = LST_Count(&ie_head);
	    for (i = 0; i < ie_loop; i++) {
		/*
		 * Find all the required Modules.
		 */
		ie_node = LST_Pop(&ie_head);
		cond = IE_IERequirements(UID, ie_node->ieType, &ieIE);
		mod_head = ieIE->moduleList;
		mod_loop = LST_Count(&mod_head);
		for (k = 0; k < mod_loop; k++) {
		    mod_node = LST_Pop(&mod_head);
		    cond = IE_ModuleRequirements(UID,
				      ie_node->ieType, mod_node->moduleType,
						 &ieModule);
		    printf("%s\n", mod_node->moduleDescription);
		    attr_head = ieModule->attributeList;
		    attr_loop = LST_Count(&attr_head);
		    for (j = 0; j < attr_loop; j++) {
			/*
			 * Find all the mandatory attributes.
			 */
			attr_node = LST_Pop(&attr_head);
			cond = DCM_LookupElement
			    (&(attr_node->element));
			printf("  %s\n",
			       attr_node->element.description);
		    }
		}
	    }
	} else			/* Illegal SOP Class entered by the user */
	    printf("Illegal SOP Class.\n");
    }
    THR_Shutdown();
    return (0);
}

/* lookupUID
**
** Purpose:
**      Find the SOP Class UID according to the SOP Class Name.
**
** Parameter Dictionary:
**	SOPClassName: Caller's character string which identifies the
**		      SOP Class.
**
** Return Values:
**	DICOM_SOPCLASSCOMPUTEDRADIOGRAPHY
**	DICOM_SOPCLASSCT
**	DICOM_SOPCLASSMR
**	DICOM_SOPCLASSNM
**	DICOM_SOPCLASSUS
**	DICOM_SOPCLASSSECONDARYCAPTURE
**	NULL
**
** Algorithm:
**     	Find the SOP Class UID corresponding to its name.
**
*/

static char *
lookupUID(char *SOPClassName)
{
    int				/* Iteration variable */
        i = 0;

    /*
     * Use SOP Class Name as key to the table to find the SOP Class UID.
     */
    while ((i < (int) DIM_OF(table)) && strcmp(SOPClassName,
					       table[i].SOPClassName))
	i++;

    if (i < (int) DIM_OF(table))/* SOP Class UID is found */
	return table[i].classUID;
    else			/* Illegal SOP Class */
	return NULL;
}

/* usageerror
**
** Purpose:
**      Print the usage message for this program and exit.
**
** Parameter Dictionary:
**	None
**
** Return Values:
**	None
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/

static void
usageerror()
{
    fprintf(stderr,
	    "Usage: object_template [-v] SOPClassName\n");
    exit(1);
}
