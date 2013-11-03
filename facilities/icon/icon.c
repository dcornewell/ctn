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
** @$=@$=@$=
*/
/*
**				DICOM 93
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	ICON_AppendStudyOffset
**			ICON_GetStudyOffsetLst
**			ICON_DumpStudyOffset
**			ICON_AppendStudyIcon
**			ICON_GetStudyIconLst
**			ICON_DumpStudyIcon
**
** Author, Date:	Tze Khiong Lee, Aug 3rd 1993
** Intent:		This facility provides routines to build
**			a simple database of image icons.
** Last Update:		$Author: smm $, $Date: 2000-01-19 22:07:35 $
** Source File:		$RCSfile: icon.c,v $
** Revision:		$Revision: 1.10 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.10 $ $RCSfile: icon.c,v $";
#include <stdio.h>
#ifndef MACOS
#include <stdlib.h>
#endif
#include <string.h>
#include <sys/types.h>
#ifdef MACOS
#include <stat.h>
#else
#include <sys/stat.h>
#endif
#include <fcntl.h>
#ifdef GCCSUNOS
#include <unistd.h>
#endif

#include "dicom.h"
#include "condition.h"
#include "lst.h"
#include "icon.h"


/* ICON_AppendStudyOffset
**
** Purpose:
**	Appends a new STUDYOFFSET record at the end of the file.
**	If the file does not exist, a new one will be created.
**
** Parameter Dictionary:
**	file		Pathname of the file.
**	studyoffset	The new STUDYOFFSET record.
**
** Return Values:
**	ICON_NORMAL		Success.
**	ICON_FILEOPENFAILED	Failed to open/create file.
**	ICON_APPENDFAILED	Failed to write to file.
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
ICON_AppendStudyOffset(char *file, ICON_STUDYOFFSET * studyoffset)
{
    int
        fd;

    if ((fd = open(file, O_WRONLY | O_APPEND)) < 0)
#ifdef MACOS
	fd = open(file, O_WRONLY | O_APPEND | O_CREAT);
#else
	fd = open(file, O_WRONLY | O_APPEND | O_CREAT, 0666);
#endif
    if (fd < 0)
	return COND_PushCondition(ICON_FILEOPENFAILED,
	 ICON_Message(ICON_FILEOPENFAILED), file, "ICON_AppendStudyOffset");
    if (write(fd, (char *) studyoffset, sizeof(ICON_STUDYOFFSET))
	!= sizeof(ICON_STUDYOFFSET))
	return COND_PushCondition(ICON_APPENDFAILED,
	   ICON_Message(ICON_APPENDFAILED), file, "ICON_AppendStudyOffset");
    close(fd);
    return (ICON_NORMAL);
}


/* ICON_GetStudyOffsetList
**
** Purpose:
**	Returns a list of STUDYOFFSET records contained in the file.
**
** Parameter Dictionary:
**	file			Pathname of the file.
**	studyoffsetlist		List of STUDYOFFSET records.
**
** Return Values:
**	ICON_NORMAL		Sucess
**	ICON_FILEOPENFAILED	Failed to open file.
**	ICON_LISTFAILURE	Failed to create/enqueue list.
**	ICON_MALLOCFAILURE	Failed to malloc memory space.
**	ICON_INCORRECTFILESIZE	Failed to parse file properly.
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
ICON_GetStudyOffsetLst(char *file, LST_HEAD ** studyoffsetlist)
{
    int
        fd,
        fdcond;
    LIST_ICON_STUDYOFFSET
	* listitem;
    ICON_STUDYOFFSET
	temp;
    CONDITION
	cond;

    fd = open(file, O_RDONLY);
    if (fd < 0)
	return COND_PushCondition(ICON_FILEOPENFAILED,
	ICON_Message(ICON_FILEOPENFAILED), file, "ICON_GetStudyOffsetList");
    *studyoffsetlist = LST_Create();
    if (*studyoffsetlist == NULL)
	return COND_PushCondition(ICON_LISTFAILURE,
		 ICON_Message(ICON_LISTFAILURE), "ICON_GetStudyOffsetList");
    while ((fdcond = read(fd, (char *) &temp, sizeof(ICON_STUDYOFFSET))) ==
	   sizeof(ICON_STUDYOFFSET)) {
	listitem = (LIST_ICON_STUDYOFFSET *) malloc(sizeof(LIST_ICON_STUDYOFFSET));
	if (listitem == NULL)
	    return COND_PushCondition(ICON_MALLOCFAILURE,
	       ICON_Message(ICON_MALLOCFAILURE), "ICON_GetStudyOffsetList");
	listitem->studyoffset = temp;
	cond = LST_Enqueue(studyoffsetlist, listitem);
	if (cond != LST_NORMAL)
	    return COND_PushCondition(ICON_LISTFAILURE,
		 ICON_Message(ICON_LISTFAILURE), "ICON_GetStudyOffsetList");
    }
    if ((fdcond != sizeof(ICON_STUDYOFFSET)) && (fdcond != 0))
	return COND_PushCondition(ICON_INCORRECTFILESIZE,
				  ICON_Message(ICON_INCORRECTFILESIZE), file, "ICON_GetStudyOffsetList");
    close(fd);
    return (ICON_NORMAL);
}


/* ICON_DumpStudyOffset
**
** Purpose:
**	Dumps an icon index file.
**
** Parameter Dictionary:
**	Define the parameters to the function
**
** Return Values:
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
ICON_DumpStudyOffset(char *file)
{
    LST_HEAD
	* list = NULL;
    CONDITION
	cond;
    LIST_ICON_STUDYOFFSET
	* item;
    int
        count = 0;

    cond = ICON_GetStudyOffsetLst(file, &list);
    if (cond != ICON_NORMAL)
	return (cond);
    while ((item = LST_Dequeue(&list)) != NULL) {
	printf("(%03d) Accession Number = %s \toffset = %d\n",
	       count++,
	       item->studyoffset.accessionNumber,
	       item->studyoffset.Offset);
	free(item);
    }
    return (ICON_NORMAL);
}


/* ICON_GetStudyOffset
**
** Purpose:
**	Describe the purpose of the function
**
** Parameter Dictionary:
**	Define the parameters to the function
**
** Return Values:
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
ICON_GetStudyOffset(char *file, char *accessionNumber, ICON_STUDYOFFSET * study)
{
    LST_HEAD
	* list = NULL;
    CONDITION
	cond;
    LIST_ICON_STUDYOFFSET
	* item;

    cond = ICON_GetStudyOffsetLst(file, &list);
    if (cond != ICON_NORMAL)
	return (cond);
    while (((item = LST_Dequeue(&list)) != NULL) &&
	   (strcmp(accessionNumber, item->studyoffset.accessionNumber)))
	free(item);
    if (item == NULL)
	return COND_PushCondition(ICON_ACCESSIONNOTFOUND,
	       ICON_Message(ICON_ACCESSIONNOTFOUND), accessionNumber, file);
    *study = item->studyoffset;
    free(item);
    while ((item = LST_Dequeue(&list)) != NULL)
	free(item);
    (void) LST_Destroy(&list);
    return (ICON_NORMAL);
}

/* ICON_AppendStudyIcon
**
** Purpose:
**	Appends a new STUDYICON record at the end of the file.
**	If the file does not exist, a new one will be created.
**
** Parameter Dictionary:
**	file		Pathname of the file.
**	studyoffset	The new STUDYICON record.
**
** Return Values:
**	ICON_NORMAL		Success.
**	ICON_FILEOPENFAILED	Failed to open/create file.
**	ICON_APPENDFAILED	Failed to write to file.
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
ICON_AppendStudyIcon(char *file, ICON_STUDYICON * studyicon)
{
    int
        fd;

    if ((fd = open(file, O_WRONLY | O_APPEND)) < 0)
#ifdef MACOS
	fd = open(file, O_WRONLY | O_APPEND | O_CREAT);
#else
	fd = open(file, O_WRONLY | O_APPEND | O_CREAT, 0666);
#endif
    if (fd < 0)
	return COND_PushCondition(ICON_FILEOPENFAILED,
	   ICON_Message(ICON_FILEOPENFAILED), file, "ICON_AppendStudyIcon");
    if (write(fd, (char *) studyicon, sizeof(ICON_STUDYICON))
	!= sizeof(ICON_STUDYICON))
	return COND_PushCondition(ICON_APPENDFAILED,
	     ICON_Message(ICON_APPENDFAILED), file, "ICON_AppendStudyIcon");
    close(fd);
    return (ICON_NORMAL);
}

/* ICON_GetStudyIconLst
**
** Purpose:
**      Reads the file and returns a list of STUDYICON records corresponding
**	to a study identified by an accession number.
**
** Parameter Dictionary:
**      file                    Pathname of the file.
**      studyiconlist         	List of STUDYICON records.
**
** Return Values:
**      ICON_NORMAL             Sucess
**      ICON_FILEOPENFAILED     Failed to open file.
**      ICON_LISTFAILURE        Failed to create/enqueue list.
**	ICON_OFFSETERROR	Failed to search file.
**      ICON_MALLOCFAILURE      Failed to malloc memory space.
**      ICON_INCORRECTFILESIZE  Failed to parse file properly.
**
** Notes:
**
** Algorithm:
**      Description of the algorithm (optional) and any other notes.
*/
CONDITION
ICON_GetStudyIconLst(char *file, ICON_STUDYOFFSET * studyoffset, LST_HEAD ** studyiconlist)
{
    int
        fd,
        fdcond;
    LIST_ICON_STUDYICON
	* listitem;
    ICON_STUDYICON
	temp;
    CONDITION
	cond;

    fd = open(file, O_RDONLY);
    if (fd < 0)
	return COND_PushCondition(ICON_FILEOPENFAILED,
	   ICON_Message(ICON_FILEOPENFAILED), file, "ICON_GetStudyIconLst");
    *studyiconlist = LST_Create();
    if (*studyiconlist == NULL)
	return COND_PushCondition(ICON_LISTFAILURE,
		    ICON_Message(ICON_LISTFAILURE), "ICON_GetStudyIconLst");
    if (lseek(fd, studyoffset->Offset, SEEK_SET) < 0)
	return COND_PushCondition(ICON_OFFSETERROR,
		    ICON_Message(ICON_OFFSETERROR), "ICON_GetStudyIconLst");
    while (((fdcond = read(fd, (char *) &temp, sizeof(ICON_STUDYICON))) ==
	    sizeof(ICON_STUDYICON)) &&
	   (!strcmp(temp.accessionNumber, studyoffset->accessionNumber))) {
	listitem = (LIST_ICON_STUDYICON *) malloc(sizeof(LIST_ICON_STUDYICON));
	if (listitem == NULL)
	    return COND_PushCondition(ICON_MALLOCFAILURE,
		  ICON_Message(ICON_MALLOCFAILURE), "ICON_GetStudyIconLst");
	listitem->studyicon = temp;
	cond = LST_Enqueue(studyiconlist, listitem);
	if (cond != LST_NORMAL)
	    return COND_PushCondition(ICON_LISTFAILURE,
		    ICON_Message(ICON_LISTFAILURE), "ICON_GetStudyIconLst");
    }
    if ((fdcond != sizeof(ICON_STUDYICON)) && (fdcond != 0))
	return COND_PushCondition(ICON_INCORRECTFILESIZE,
	      ICON_Message(ICON_INCORRECTFILESIZE), "ICON_GetStudyIconLst");

    close(fd);
    return (ICON_NORMAL);
}


/* ICON_DumpStudyIcon
**
** Purpose:
**	Dumps an icon table file.
**
** Parameter Dictionary:
**	Define the parameters to the function
**
** Return Values:
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/
CONDITION
ICON_DumpStudyIcon(char *file)
{
    int
        fd,
        fdcond,
        count = 0;
    ICON_STUDYICON
	studyicon;

    fd = open(file, O_RDONLY);
    if (fd < 0)
	return COND_PushCondition(ICON_FILEOPENFAILED,
	     ICON_Message(ICON_FILEOPENFAILED), file, "ICON_DumpStudyIcon");
    while ((fdcond = read(fd, (char *) &studyicon, sizeof(ICON_STUDYICON))) ==
	   sizeof(ICON_STUDYICON))
	printf("(%05d) Accession Number = %s\tUID = %s\n",
	       count++,
	       studyicon.accessionNumber,
	       studyicon.UID);
    if ((fdcond != sizeof(ICON_STUDYICON)) && (fdcond != 0))
	return COND_PushCondition(ICON_INCORRECTFILESIZE,
	      ICON_Message(ICON_INCORRECTFILESIZE), "ICON_GetStudyIconLst");
    close(fd);
    return (ICON_NORMAL);
}
