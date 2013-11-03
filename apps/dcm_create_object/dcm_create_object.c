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
/*
+-+-+-+-+-+-+-+-+-
*/
/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	dcm_make_object (main)
**			makeElement
**			usageError
**			addPixels
**			fileSize
** Author, Date:	Aniruddha Gokhale, 24-May-1995
** Intent:		This program allows us to build a DICOM object.
**			The user will provide the input in the form of
**			triplets (Group, Element, Value) for "non" DCM_SQ
**			elements. Whenever an attribute is of the type,
**			SEQUENCE, the Value parameter can be recusively defined
**			using parentheses as shown:
**
**			group# element# (
**				group# element# value
**				....   ......   ....
**				)
**
**			other elements
**
** Last Update:		$Author: smm $, $Date: 2007-03-16 01:41:00 $
** Source File:		$RCSfile: dcm_create_object.c,v $
** Revision:		$Revision: 1.28 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.28 $ $RCSfile: dcm_create_object.c,v $";
#include "create.h"

DCM_OBJECT
* object;			/* stores the object that will be created and
				 * written onto the disk */
CTNBOOLEAN
verbose = FALSE;

static void usageerror();
static void addpixels(DCM_OBJECT ** object, char *pixelfile);
static void addEncapsulatedDocument(DCM_OBJECT ** object, char *encapsulatedDocument);
static void addBinaryTable(DCM_OBJECT ** object, char *binaryTable);
static int fileSize(int fd);
static void parseNumericString(char *, DCM_ELEMENT **);
static void parseNumericStringFloat(char *, DCM_ELEMENT **);

extern int yyparse(void);
DCM_VALUEREPRESENTATION contextVR = DCM_CTX;

main(int argc, char **argv)
{
  CONDITION cond;
  char
    *pixelfile = NULL,	/* file with pixel data */
    *encapsulatedDocument = NULL,	/* File with encapsulated document */
    *binaryTable = NULL,	/* File with binary Table */
    *inputfile = NULL;	/* input file with data that creates the
				 * object */

  yyin = stdin;		/* default input file */
  while (--argc > 0 && (*++argv)[0] == '-') {
    switch (*(argv[0] + 1)) {
    case 'b':		/* pixel data exists */
    case 'B':
      argc--;
      argv++;
      binaryTable = *argv;
      break;
    case 'c':		/* Set type for context values */
    case 'C':
     argc--; argv++;
     if (argc < 1)		/* must have an output file */
       usageerror();
     if (strcmp(*argv, "SS") == 0) {
	contextVR = DCM_SS;
     } else if (strcmp(*argv, "US") == 0) {
	contextVR = DCM_SS;
     } else {
       usageerror();
     }

     break;
    case 'e':		/* pixel data exists */
    case 'E':
      argc--;
      argv++;
      encapsulatedDocument = *argv;
      break;
    case 'i':		/* use input file */
    case 'I':
      argc--;
      argv++;
      inputfile = *argv;
      yyin = fopen(inputfile, "r");
      if (yyin == NULL) {
	perror("fopen failed opening input file");
	exit(1);
      }
      break;
    case 'p':		/* pixel data exists */
    case 'P':
      argc--;
      argv++;
      pixelfile = *argv;
      break;
    case 'v':		/* verbose option */
    case 'V':
      verbose = TRUE;
      break;
    default:
      fprintf(stderr, "Unrecognized option: %c\n", *(argv[0] + 1));
      break;
    }
  }
  if (argc < 1)		/* must have an output file */
    usageerror();

  THR_Init();
    /*
     * parse the input and build the list of elements to be added to an
     * object
     */
  (void) yyparse();

  if (pixelfile != NULL)
    addpixels(&object, pixelfile);
  if (encapsulatedDocument != NULL)
    addEncapsulatedDocument(&object, encapsulatedDocument);
  if (binaryTable != NULL)
    addBinaryTable(&object, binaryTable);

  if (verbose) {
    printf("*****In main program, after parse*****\n");
    (void) DCM_DumpElements(&object, 0);
  }
  cond = DCM_WriteFile(&object, DCM_ORDERLITTLEENDIAN, *argv);
  if (cond != DCM_NORMAL)
    COND_DumpConditions();
  (void) DCM_CloseObject(&object);
  THR_Shutdown();
  exit(0);
}


/* usageError
**
** Purpose:
**	Displays the correct usage of the command
**
** Parameter Dictionary:
**	None
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static void
usageerror()
{
  char msg[] = "\
Usage: dcm_create_object [-b binaryfile] [-e doc] [-i inputfile] [-p pixels] [-v] fileout \n\
\n\
  -b   Read binary table from this file and add as 0028 2000\n\
  -e   Read encapsulated document from this file and add as 0042 0011\n\
  -i   Take input from this file rather than stdin \n\
  -p   Read raw pixel data from this file and add as 7FE0 0010 \n\
  -v   Verbose mode \n\
\n\
  fileout  Name the output file \n";

    fprintf(stdout, msg);
    exit(1);
}

/* makeElement
**
** Purpose:
**	Make an element out of the given group#, element# and value
**
** Parameter Dictionary:
**	group		Group number of the element to be added
**	element		Element number
**	vdata		Actual data to be added
**	elem		Handle to the element to be returned
**
** Return Values:
**
**	DCM_ILLEGALOBJECT
**	DCM_ILLEGALREPRESENTATION
**	DCM_INSERTFAILED
**	DCM_NORMAL
**	DCM_NULLOBJECT
**	DCM_UNRECOGNIZEDELEMENT
**	DCM_UNRECOGNIZEDGROUP
**
** Notes:
**
** Algorithm:
*/

CONDITION
makeElement(int group, int element, void *vdata,
	    DCM_ELEMENT ** elem)
{
    CONDITION
	cond = DCM_NORMAL;

    DCM_ELEMENT
	* e;
    char
       *data = (char *) vdata;

    e = (DCM_ELEMENT *) malloc(sizeof(DCM_ELEMENT));
    if (e == NULL) {
	printf("malloc: cannot allocate memory for DCM_ELEMENT\n");
	return DCM_ELEMENTCREATEFAILED;
    }
    e->tag = DCM_MAKETAG(group, element);
    cond = DCM_LookupElement(e);
    if (cond != DCM_NORMAL)
	return cond;

    if (e->tag == DCM_MAKETAG(0x0028, 0x3002)) {
      e->representation = DCM_US;
    }

    if (e->representation == DCM_CTX) {
      e->representation = contextVR;
    }

    switch (e->representation) {
    case DCM_DD:		/* Data set */
    case DCM_FD:		/* Floating double */
    case DCM_OT:		/* Other binary value */
    case DCM_UN:		/* Unknown/unspecified */
    case DCM_RET:		/* Retired */
    case DCM_CTX:		/* Context sensitive */
	printf("The representation for (%04x %04x) is unimplemented\n",
	       group, element);
	e->d.ot = NULL;
	break;
    case DCM_AE:		/* Application Entity */
    case DCM_AS:		/* Age String */
    case DCM_CS:		/* Control string */
    case DCM_DA:		/* Date */
    case DCM_DT:		/* Date/Time */
    case DCM_DS:		/* Decimal string */
    case DCM_IS:		/* Integer string */
    case DCM_LT:		/* Long text */
    case DCM_LO:		/* Long string */
    case DCM_PN:		/* Person Name */
    case DCM_SH:		/* Short string */
    case DCM_ST:		/* Short text */
    case DCM_TM:		/* Time */
    case DCM_UI:		/* Unique identifier (UID) */
    case DCM_UT:		/* Unlimited text */
	if (strcmp(data, "#") == 0) {
	    e->length = 0;
	    e->d.string = NULL;
	} else {
	    e->length = strlen(data);
	    e->d.string = data;
	}
	break;
    case DCM_AT:		/* Attribute tag */
    case DCM_SL:		/* Signed long */
    case DCM_SS:		/* Signed short */
    case DCM_UL:		/* Unsigned long */
    case DCM_US:		/* Unsigned short */
	if (strcmp(data, "#") == 0) {
	    e->length = 0;
	    e->d.string = NULL;
	} else {
	    parseNumericString(data, &e);
	}
	break;
    case DCM_FL:		/* Float */
	if (strcmp(data, "#") == 0) {
	    e->length = 0;
	    e->d.string = NULL;
	} else {
	    parseNumericStringFloat(data, &e);
	}
	break;
    case DCM_SQ:		/* Sequence of items */
	{
	    LST_HEAD *l;
	    DCM_SEQUENCE_ITEM *dsi;
	    unsigned long len = 0;
	    char* xxx = "####";

	    e->length = 0;
	    e->multiplicity = 1;
	    if (memcmp(vdata, xxx, 4) == 0) {
		e->d.sq = LST_Create();
	    } else {
	    e->d.sq = (LST_HEAD *) vdata;
	    l = e->d.sq;
	    dsi = LST_Position(&l, LST_Head(&l));
	    if (dsi == NULL) {
		printf("Empty sequence\n");
		exit(1);
	    }
	    while (dsi != NULL) {
		cond = DCM_GetObjectSize(&dsi->object, &len);
		if (cond != DCM_NORMAL) {
		    printf("DCM_GetObjectSize: failed to get object size\n");
		    COND_DumpConditions();
		    return cond;
		}
		e->length += 8 + len;
		dsi = LST_Next(&l);
	    }
	    }
	}
	break;
    default:
	fprintf(stderr,
		"Should not reach default for element (%04x %04x)\n",
		group, element);
	e->d.ot = NULL;
    }

    *elem = e;
    return cond;
}


/* addPixels
**
** Purpose:
**	Add pixel data to the DICOM object
**
** Parameter Dictionary:
**	object		Handle to object to which pixel data is to be added
**	pixelfile	Name of file containing pixel data
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static void
addpixels(DCM_OBJECT ** obj, char *pixelfile)
{
    int
        fd,
        size;
    unsigned char
       *p;

    DCM_ELEMENT e = {DCM_PXLPIXELDATA, DCM_OT, "", 0, 0, NULL};
    CONDITION
	cond;

    fd = open(pixelfile, O_RDONLY);
    if (fd < 0) {
	perror(pixelfile);
	exit(1);
    }
    size = fileSize(fd);
    if (size < 0) {
	perror(pixelfile);
	exit(1);
    }
    p = (unsigned char *) malloc(size);
    if (p == NULL) {
	perror("");
	exit(1);
    }
    e.length = (unsigned long) size;
    e.d.ot = p;
    if (read(fd, p, size) != size) {
	perror(pixelfile);
	exit(1);
    }
    cond = DCM_AddElement(obj, &e);
    if (cond != DCM_NORMAL) {
	COND_DumpConditions();
	exit(1);
    }
}

/* addEncapsulatedDocument
**
** Purpose:
**	Add an Encapsulated Document to the DICOM object
**
** Parameter Dictionary:
**	object		Handle to object to which pixel data is to be added
**	encapsulatedDocument	Name of file containing pixel data
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static void
addEncapsulatedDocument(DCM_OBJECT ** obj, char *encapsulatedDocument)
{
  int
    fd,
    size;
  unsigned char *p;

  DCM_ELEMENT e = {DCM_MAKETAG(0x0042, 0x0011), DCM_OB, "", 0, 0, NULL};
  CONDITION cond;

  fd = open(encapsulatedDocument, O_RDONLY);
  if (fd < 0) {
    perror(encapsulatedDocument);
    exit(1);
  }
  size = fileSize(fd);
  if (size < 0) {
    perror(encapsulatedDocument);
    exit(1);
  }
  p = (unsigned char *) malloc(size+1);
  if (p == NULL) {
    perror("");
    exit(1);
  }
  e.length = (unsigned long) size;
  e.d.ot = p;
  if (read(fd, p, size) != size) {
    perror(encapsulatedDocument);
    exit(1);
  }
  if(size&1 == 1) {
    p[size] = '\0';
    e.length++;
  }
  cond = DCM_AddElement(obj, &e);
  if (cond != DCM_NORMAL) {
    COND_DumpConditions();
    exit(1);
  }
}

/* addBinaryTable
**
** Purpose:
**	Add a binary table to the DICOM object
**
** Parameter Dictionary:
**	object		Handle to object to which pixel data is to be added
**	encapsulatedDocument	Name of file containing pixel data
**
** Return Values:
**	None
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static void
addBinaryTable (DCM_OBJECT ** obj, char *binaryTable)
{
  int
    fd,
    size;
  unsigned char *p;

  DCM_ELEMENT e = {DCM_MAKETAG(0x0028, 0x2000), DCM_OT, "", 0, 0, NULL};
  CONDITION cond;

  fd = open(binaryTable, O_RDONLY);
  if (fd < 0) {
    perror(binaryTable);
    exit(1);
  }
  size = fileSize(fd);
  if (size < 0) {
    perror(binaryTable);
    exit(1);
  }
  p = (unsigned char *) malloc(size+1);
  if (p == NULL) {
    perror("");
    exit(1);
  }
  e.length = (unsigned long) size;
  e.d.ot = p;
  if (read(fd, p, size) != size) {
    perror(binaryTable);
    exit(1);
  }
  if(size&1 == 1) {
    p[size] = '\0';
    e.length++;
  }
  cond = DCM_AddElement(obj, &e);
  if (cond != DCM_NORMAL) {
    COND_DumpConditions();
    exit(1);
  }
}


/* fileSize
**
** Purpose:
**	Returns file size on success
**
** Parameter Dictionary:
**	fd	File descriptor
**
** Return Values:
**	file size on success, else error status code
**
** Notes:
**
** Algorithm:
**	Description of the algorithm (optional) and any other notes.
*/

static int
fileSize(int fd)
{
    int
        status;
    struct stat
        im_stat;

    status = fstat(fd, &im_stat);
    if (status < 0) {
	return status;
    } else
	return im_stat.st_size;
}

void
parseNumericString(char *data, DCM_ELEMENT ** elem)
{
    DCM_ELEMENT
    * e = *elem;
    char *token,
       *sep = "\\";
    int
        i[100];			/* assume that multiplicity can atmost be 100 */
    int
        j,
        count = 0;

    /* assume that the incoming data is of multiplicity >= 1 */
    token = strtok(data, sep);
    while (token != NULL) {
	if (sscanf(token, "%d", &i[count]) != 1) {
	    fprintf(stderr, "Error parsing %s as a numeric quantity\n", token);
	    exit(1);
	}
	count++;
	/* get next token */
	token = strtok(NULL, sep);
    }
    if (count == 0) {
	e->length = 0;
	e->d.string = NULL;
	return;
    }
    /* now fill the data in the element */
    switch (e->representation) {
    case DCM_AT:		/* Attribute tag */
	e->d.at = (DCM_TAG *) malloc(sizeof(DCM_TAG) * count);
	if (e->d.at == NULL) {
	    fprintf(stderr, "malloc: Out of memory\n");
	    exit(1);
	}
	e->length = sizeof(DCM_TAG) * count;
	for (j = 0; j < count; j++) {
	    e->d.at[j] = (DCM_TAG) i[j];
	}
	break;
    case DCM_SL:
	e->d.sl = (S32 *) malloc(sizeof(S32) * count);
	if (e->d.sl == NULL) {
	    fprintf(stderr, "malloc: Out of memory\n");
	    exit(1);
	}
	e->length = sizeof(S32) * count;
	for (j = 0; j < count; j++) {
	    e->d.sl[j] = (S32) i[j];
	}
	break;
    case DCM_SS:
	e->d.ss = (S16 *) malloc(sizeof(S16) * count);
	if (e->d.ss == NULL) {
	    fprintf(stderr, "malloc: Out of memory\n");
	    exit(1);
	}
	e->length = sizeof(S16) * count;
	for (j = 0; j < count; j++) {
	    e->d.ss[j] = (S16) i[j];
	}
	break;
    case DCM_UL:
	e->d.ul = (U32 *) malloc(sizeof(U32) * count);
	if (e->d.ul == NULL) {
	    fprintf(stderr, "malloc: Out of memory\n");
	    exit(1);
	}
	e->length = sizeof(U32) * count;
	for (j = 0; j < count; j++) {
	    e->d.ul[j] = (U32) i[j];
	}
	break;
    case DCM_US:
	e->d.us = (U16 *) malloc(sizeof(U16) * count);
	if (e->d.us == NULL) {
	    fprintf(stderr, "malloc: Out of memory\n");
	    exit(1);
	}
	e->length = sizeof(U16) * count;
	for (j = 0; j < count; j++) {
	    e->d.us[j] = (U16) i[j];
	}
	break;
    case DCM_FL:
	e->d.ot = malloc(sizeof(U32) * count);
	if (e->d.ot == NULL) {
	    fprintf(stderr, "malloc: Out of memory\n");
	    exit(1);
	}
	e->length = sizeof(U32) * count;
	memset(e->d.ot, 0, sizeof(U32) * count);
#if 0
	for (j = 0; j < count; j++) {
	    e->d.ul[j] = (U32) i[j];
	}
#endif
	break;
    default:
	return;			/* unknown. Better not come here because we
				 * invoked this proc only for the obove 4
				 * cases */
    }
}

void
parseNumericStringFloat(char *data, DCM_ELEMENT ** elem)
{
    DCM_ELEMENT
    * e = *elem;
    char *token,
       *sep = "\\";
    float
        f[100];			/* assume that multiplicity can atmost be 100 */
    int
        j,
        count = 0;

    /* assume that the incoming data is of multiplicity >= 1 */
    token = strtok(data, sep);
    while (token != NULL) {
	if (sscanf(token, "%f", &f[count]) != 1) {
	    fprintf(stderr, "Error parsing %s as a numeric quantity\n", token);
	    exit(1);
	}
	count++;
	/* get next token */
	token = strtok(NULL, sep);
    }
    if (count == 0) {
	e->length = 0;
	e->d.string = NULL;
	return;
    }
    /* now fill the data in the element */
    switch (e->representation) {
    case DCM_FL:
	e->d.ot = malloc(sizeof(U32) * count);
	if (e->d.ot == NULL) {
	    fprintf(stderr, "malloc: Out of memory\n");
	    exit(1);
	}
	e->length = sizeof(U32) * count;
	memcpy(e->d.ot, f, sizeof(U32) * count);
/*	memset(e->d.ot, 0, sizeof(U32) * count);*/
#if 0
	for (j = 0; j < count; j++) {
	    e->d.ul[j] = (U32) i[j];
	}
#endif
	break;
    default:
	return;			/* unknown. Better not come here because we
				 * invoked this proc only for the obove 4
				 * cases */
    }
}

void
yyerror(const char *err)
{
    printf("Parsing Error: (msg = %s)\n", err);
    printf("Exiting\n");
    exit(1);
}
