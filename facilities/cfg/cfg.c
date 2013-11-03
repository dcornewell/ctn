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
**                              DICOM 93
**                   Electronic Radiology Laboratory
**                 Mallinckrodt Institute of Radiology
**              Washington University School of Medicine
**
** Module Name(s):      CFG_LoadDatabase
**			CFG_LoadConnect
**			CFG_LoadConnect1
**			CFG_free
**			CFG_Message
**			span
**			environment
** Author, Date:        Sheldon Hoffman, 24 May 93
** Intent:              Provide standard common for configuration file processing
** Last Update:         $Author: smm $, $Date: 1995-02-24 04:21:42 $
** Source File:         $RCSfile: cfg.c,v $
** Revision:            $Revision: 1.13 $
** Status:              $State: Exp $
*/
static char rcsid[] = "$Revision: 1.13 $ $RCSfile: cfg.c,v $";

#include    <string.h>
#include    <stdio.h>
#include    <stdlib.h>
/*lint -e652 */
#include    <ctype.h>
/*lint +e652 */
#ifdef GCCSUNOS
#include    <unistd.h>
#endif

#include    "dicom.h"
#include    "condition.h"
#include    "cfg.h"

#define         ENV(v)  environment(v, sizeof(v))	/* A handy macro */
#define         RETURN(c)   return COND_PushCondition(c,CFG_Message(c));

static short lineno;		/* Current line number */
static char *filename;		/* Name of last file processed */

static char *CFG_Message(CONDITION);
static short span(char *);	/* Skip whitespace characters */
static void environment(char *, short);	/* Substitute environment variables */
static void sub_escape(char *s);
static int flags_match(char *required, char *flags);

/*
** REFERENCING ENVIRONMENT VARIABLES IN A CONFIGURATION FILE
**
**      Environment variables may be optionally mentioned in configuration files.
**      They are substituted with their value, as defined by the environment
**      in which the program using this package is run.
**
**      This facility makes convenient, for example, encapsulating long path
**      names.  To refer to an environment variable, the first character
**      of a field must be the dollar sign ($) followed by an alphanumeric
**      identifier (under-score (_) counts as an alphabetic character.)  The
**      variable is replaced with its value.  If a variable is not found in
**      the environment, the reference ($ + variable) is simply extracted
**      from the field.  Only one variable may be mentioned per field
**      and it must appear at the beginning of that field.  Environment
**      variables are case sensitive.
*/

/*---------------------------------------------------------------------*/
/* CFG_LoadDatabase
**
** Purpose:
**      Read DATABASE.CFG file into an array of CFG_DATABASE structures.
**
** Parameter Dictionary:
**      char *file;             Filespec of DATABASE.CFG
**      char *ctn_node;         TCP node name of local machine or NULL
**      CFG_DATABASE **db;      Place to leave pointer to returned structure
**      short *pcount;          Place to leave number of structures loaded
**
** Return Values:
**      A CONDITION code indicating result of operation.
**      A pointer to a Malloc'd array of CFG_DATABASE structures (db).
**      A count of the number of CFG_DATABASE structures loaded (pcount).
**
** Algorithm:
**      The file is scanned once to check its syntax and count the number
**      of entries.  Malloc is used to allocate an array of CFG_DATABASE
**      structures.  The file is scanned again and the fields are loaded.
**
**      If ctn_node is NULL, all entries are loaded.  Otherwise, only
**      entries for the ctn_node specified are loaded.
**
**      An extra, empty (all zero) CFG_DATABASE structure is appended
**      to the end to allow sequential access to the array of structures.
*/
CONDITION
CFG_LoadDatabase(char *file, char *ctn_node, CFG_DATABASE ** db, short *pcount)
{
    CFG_DATABASE temp;
    FILE *fid;			/* File handle */
    char buf[180];
    short count;
    short i;			/* Loop count */

    filename = file;
    if (db)
	*db = NULL;
    fid = fopen(filename, "r");
    if (!fid)
	RETURN(CFG_CANTOPEN);	/* Can't open file */

/* COUNT THE NUMBER OF ENTRIES IN THE FILE */

    count = 0;
    for (lineno = 1; fgets(buf, sizeof(buf) - 1, fid); lineno++) {
	char *p;		/* Pointer to line */
	short n;		/* Number of fields */

	p = strchr(buf, ';');
	if (p)
	    *p = '\0';		/* Strip comment */

	p = buf;
	p += span(p);
	if (!*p)
	    continue;

	n = sscanf(p, "%s %s %s %s %s %s %s",
		   temp.ctn_node, temp.title, temp.file, temp.imagepath,
		   temp.quota, temp.read, temp.write);

	if (n != 7) {		/* We require this many fields */
	    fclose(fid);
	    return COND_PushCondition(CFG_BADLINE, CFG_Message(CFG_BADLINE));
	}
	ENV(temp.ctn_node);	/* Substitute possible environment variable */

	if (!ctn_node || strcmp(ctn_node, temp.ctn_node) == 0)
	    count++;		/* Count one valid entry */
    }

/* ALLOCATE MEMORY NOW THAT WE KNOW HOW MANY WE HAVE.
 * BE SURE TO ALLOCATE ONE EXTRA ONE AT THE END.
 */

    if (count <= 0) {
	fclose(fid);
	return COND_PushCondition(CFG_NOENTRIES, CFG_Message(CFG_NOENTRIES));
    }
    *db = (CFG_DATABASE *) calloc(count + 1, sizeof(CFG_DATABASE));
    if (!*db) {
	fclose(fid);
	return COND_PushCondition(CFG_NOMEMORY, CFG_Message(CFG_NOMEMORY));
    }
/* SCAN THE DATABASE AFRESH AND LOAD UP MEMORY */

    (void) fseek(fid, 0L, SEEK_SET);	/* Back to beginning of file */

    for (lineno = 1, i = 0; i < count && fgets(buf, sizeof(buf) - 1, fid); lineno++) {
	CFG_DATABASE *pdb;
	char *p;		/* Pointer to line */

	p = strchr(buf, ';');
	if (p)
	    *p = '\0';		/* Strip comment */

	p = buf;
	p += span(buf);
	if (!*p)
	    continue;

	sscanf(p, "%s", temp.ctn_node);

	ENV(temp.ctn_node);	/* Substitute possible environment variable */

	if (!ctn_node || strcmp(ctn_node, temp.ctn_node) == 0) {
	    pdb = *db + i;	/* point to new entry */
	    sscanf(p, "%s %s %s %s %s %s %s",
		   pdb->ctn_node, pdb->title, pdb->file, pdb->imagepath,
		   pdb->quota, pdb->read, pdb->write);

	    ENV(pdb->ctn_node);	/* Substitute possible environment variable */
	    ENV(pdb->title);
	    ENV(pdb->file);
	    ENV(pdb->imagepath);
	    ENV(pdb->quota);
	    ENV(pdb->read);
	    ENV(pdb->write);

	    sub_escape(pdb->ctn_node);	/* Substitute escape characters */
	    sub_escape(pdb->title);
	    sub_escape(pdb->file);
	    sub_escape(pdb->imagepath);
	    sub_escape(pdb->quota);
	    sub_escape(pdb->read);
	    sub_escape(pdb->write);

	    i++;		/* Count one valid entry */
	}
    }

    fclose(fid);
    if (pcount)
	*pcount = count;
    return (CFG_NORMAL);
}

/*---------------------------------------------------------------------*/
/*
** Purpose:
**      Read CONNECT.CFG file into an array of CFG_CONNECT structures.
**
** Parameter Dictionary:
**      char *file;             Filespec of CONNECT.CFG
**      char *ctn_node;         TCP node name of local machine or NULL
**      CFG_CONNECT **cn;       Place to leave pointer to returned structure
**      short *pcount;          Place to leave number of structures loaded
**      char *flags;            List of required flags
**
** Return Values:
**      A CONDITION code indicating result of operation.
**      A pointer to a Malloc'd array of CFG_CONNECT structures (cn).
**      A count of the number of CFG_CONNECT structures loaded (pcount).
**
** Algorithm:
**      Call CFG_LoadConnect with flags argument set to NULL.
*/

CONDITION
CFG_LoadConnect(char *file, char *ctn_node, CFG_CONNECT ** cn, short *pcount)
{
    return (CFG_LoadConnect1(file, ctn_node, cn, pcount, NULL));
}

/*---------------------------------------------------------------------*/
/* CFG_LoadConnect1
**
** Purpose:
**      Read CONNECT.CFG file into an array of CFG_CONNECT structures.
**
** Parameter Dictionary:
**      char *file;             Filespec of CONNECT.CFG
**      char *ctn_node;         TCP node name of local machine or NULL
**      CFG_CONNECT **cn;       Place to leave pointer to returned structure
**      short *pcount;          Place to leave number of structures loaded
**      char *flags;            List of required flags
**
** Return Values:
**      A CONDITION code indicating result of operation.
**      A pointer to a Malloc'd array of CFG_CONNECT structures (cn).
**      A count of the number of CFG_CONNECT structures loaded (pcount).
**
** Algorithm:
**      The file is scanned once to check its syntax and count the number
**      of entries.  Malloc is used to allocate an array of CFG_CONNECT
**      structures.  The file is scanned again and the fields are loaded.
**
**      If ctn_node is NULL, all entries are loaded.  Otherwise, only
**      entries for the ctn_node specified are loaded.
**
**      If (required) flags argument is not NULL, only connections
**      whose "flag" field contains each character listed in the
**      "flags" argument are loaded.  The flags are compared
**      without respect to order or case.  Thus, the "flags" argument
**      is a REQUIRED list of flags which the connection must have
**      before it is loaded.  When the "flags" argument is NULL,
**      the connection flag field is ignored.
**
**      An extra, empty (all zero) CFG_CONNECT structure is appended
**      to the end to allow sequential access to the array of structures.
*/
CONDITION
CFG_LoadConnect1(char *file, char *ctn_node, CFG_CONNECT ** cn, short *pcount, char *flags)
{
    CFG_CONNECT temp;
    FILE *fid;			/* File handle */
    char buf[180];
    short count;
    short i;			/* Loop count */

    filename = file;
    if (cn)
	*cn = NULL;
    fid = fopen(filename, "r");
    if (!fid) {
	return COND_PushCondition(CFG_CANTOPEN, CFG_Message(CFG_CANTOPEN));
    }
    /* COUNT THE NUMBER OF ENTRIES IN THE FILE */

    count = 0;
    for (lineno = 1; fgets(buf, sizeof(buf) - 1, fid); lineno++) {
	char *p;		/* Pointer to line */
	short n;		/* Number of fields */

	p = strchr(buf, ';');
	if (p)
	    *p = '\0';		/* Strip comment */

	p = buf;
	p += span(buf);
	if (!*p)
	    continue;

	n = sscanf(p, "%s %s %s %s %s %s %s",
		   temp.ctn_node, temp.login, temp.title,
		   temp.flag, temp.spool,
		   temp.vendor_node, temp.port);

	if (n != 7) {		/* We require this many fields */
	    fclose(fid);
	    return COND_PushCondition(CFG_BADLINE, CFG_Message(CFG_BADLINE));
	}
	ENV(temp.ctn_node);
	ENV(temp.flag);

	if ((!ctn_node || strcmp(ctn_node, temp.ctn_node) == 0) &&
	    flags_match(flags, temp.flag))
	    count++;		/* Count one valid entry */
    }

/* ALLOCATE MEMORY NOW THAT WE KNOW HOW MANY WE HAVE.
 * BE SURE TO ALLOCATE ONE EXTRA ONE AT THE END.
 */

    if (count <= 0) {
	fclose(fid);
	return COND_PushCondition(CFG_NOENTRIES, CFG_Message(CFG_NOENTRIES));
    }
    *cn = (CFG_CONNECT *) calloc(count + 1, sizeof(CFG_DATABASE));
    if (!*cn) {
	fclose(fid);
	return COND_PushCondition(CFG_NOMEMORY, CFG_Message(CFG_NOMEMORY));
    }
/* SCAN THE DATABASE AFRESH AND LOAD UP MEMORY */

    (void) fseek(fid, 0L, SEEK_SET);	/* Back to beginning of file */

    for (lineno = 1, i = 0; i < count && fgets(buf, sizeof(buf) - 1, fid); lineno++) {
	CFG_CONNECT *pcn;
	char *p;		/* Pointer to line */

	p = strchr(buf, ';');
	if (p)
	    *p = '\0';		/* Strip comment */

	p = buf;
	p += span(buf);
	if (!*p)
	    continue;

	sscanf(p, "%s %s %s %s %s %s %s",
	       temp.ctn_node, temp.login, temp.title,
	       temp.flag, temp.spool,
	       temp.vendor_node, temp.port);

	ENV(temp.ctn_node);
	ENV(temp.flag);

	if ((!ctn_node || strcmp(ctn_node, temp.ctn_node) == 0) &&
	    flags_match(flags, temp.flag)) {
	    pcn = *cn + i;	/* point to new entry */
	    sscanf(p, "%s %s %s %s %s %s %s",
		   pcn->ctn_node, pcn->login, pcn->title,
		   pcn->flag, pcn->spool,
		   pcn->vendor_node, pcn->port);

	    ENV(pcn->ctn_node);	/* Substitute possible environment variable */
	    ENV(pcn->login);
	    ENV(pcn->title);
	    ENV(pcn->flag);
	    ENV(pcn->spool);
	    ENV(pcn->vendor_node);
	    ENV(pcn->port);

	    sub_escape(pcn->ctn_node);	/* Substitute escape characters */
	    sub_escape(pcn->login);
	    sub_escape(pcn->title);
	    sub_escape(pcn->flag);
	    sub_escape(pcn->spool);
	    sub_escape(pcn->vendor_node);
	    sub_escape(pcn->port);

	    i++;		/* Count one valid entry */
	}
    }

    fclose(fid);
    if (pcount)
	*pcount = count;
    return (CFG_NORMAL);
}

/*---------------------------------------------------------------------*/
/* CFG_free
**
** Purpose:
**      Release resources (malloc'd memory) allocated by CFG_LoadDatabase() or
**      CFG_LoadConnect() routines.
**
** Parameter Dictionary:
**      void *p;                A pointer returned by CFG_LoadDatabase
**                              or CFG_LoadConnect
** Algorithm:
**      Call free() provide the argument is not NULL.
*/
void
CFG_free(void *p)
{
    if (p)
	free(p);		/* Assume we allocated this */
}


/*---------------------------------------------------------------------*/
/* CFG_Message
**
** Purpose:     Format an error message given a CFG condition
**
** Parameter Dictionary:
**              CONDITION cond;     A CFG condition
** Return Values:
**              A pointer to a static message string.
**
** Algorithm:
**      A static buffer is loaded with a message string based on the argument.
**      The previous contents of the static string is lost.  The message is
**      generally less than 80 chars but because some messages contain the
**      path and name of a configuration file, the returned string can be
**      longer.
*/
static char *
CFG_Message(CONDITION cond)
{
    static char buf[256];

    switch (cond) {
    case CFG_NORMAL:
	sprintf(buf, "CFG no errors in %s", filename);
	break;
    case CFG_CANTOPEN:
	sprintf(buf, "CFG Can't open %s", filename);
	break;
    case CFG_BADLINE:
	sprintf(buf, "CFG wrong number of field (line %d, file %s)", lineno, filename);
	break;
    case CFG_NOENTRIES:
	sprintf(buf, "CFG No entries in configuration %s", filename);
	break;
    case CFG_NOMEMORY:
	sprintf(buf, "CFG No memory for database using file %s", filename);
	break;
    default:
	sprintf(buf, "CFG Unknown error (%ld)", cond);
    }
    return (buf);
}


/*---------------------------------------------------------------------*/
/* SPAN - Span white space.  Return # of white characters.
 */
static short
span(char *q)
{
    char *p;

    p = q;
    if (!p)
	return (0);

    while (*p && isspace(*p))
	p++;

    return ((short) (p - q));
}


/*---------------------------------------------------------------------*/
/* ENVIRONMENT - Substitute environment variables, if found, in a string.
 */
static void
environment(char *s, short n)
{
    char *dst;
    char *p;
    char var[80];		/* Place to store variable */
    char value[256];

    dst = s;			/* Remember beginning of string */

    if (!s || *s++ != '$')
	return;			/* No environment variable here */

    p = var;			/* Construct variable name */
    while (*s == '_' || isalnum(*s))
	*p++ = *s++;		/* Copy variable name */

    *p = '\0';			/* Terminate variable name */

    p = getenv(var);		/* Look up environment variable */
    if (p)			/* If present.. */
	sprintf(value, "%s%s", p, s);	/* Insert its value into result */
    else
	strcpy(value, s);	/* Just copy remainder of string */

    sprintf(dst, "%.*s", n, value);	/* Copy back to user's buffer */
}

static void
sub_escape(char *s)
{
    while (*s != '\0') {
	if (*s == '^')
	    *s = ' ';
	s++;
    }
}

/*---------------------------------------------------------------------*/
/* FLAGS_MATCH - Compare required flags against connect flags to
 *      make sure connection supports each one.
 * Return 1 if the required flags are present, 0 if not.
 */
static int
flags_match(char *required, char *flags)
{
    char *f;

    if (!required)
	goto MATCHED;		/* Nothing required */

    if (!flags)
	goto MISMATCHED;	/* Something required, Nothing available */

    for (; *required; required++) {
	char r;
	short found;

	r = (char) toupper(*required);
	for (f = flags, found = 0; *f; f++) {
	    if (toupper(*f) == r) {
		found = 1;
		break;
	    }
	}
	if (!found)
	    goto MISMATCHED;
    }

MATCHED:
    return (1);			/* All required flags are present */

MISMATCHED:
    return (0);			/* There is at least one required flag
				 * missing */
}
