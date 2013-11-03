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
** Module Name(s):	dcm_modify_object (main)
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
** Last Update:		$Author: smm $, $Date: 2007-03-16 02:18:15 $
** Source File:		$RCSfile: dcm_modify_object.c,v $
** Revision:		$Revision: 1.21 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.21 $ $RCSfile: dcm_modify_object.c,v $";
#include "modify.h"

LST_HEAD
* modlist;			/* stores the list of modifications */

DCM_OBJECT
* object;			/* stores the the object to which
				 * modifications need to be done. */
CTNBOOLEAN
verbose = FALSE;

static void usageerror();
static void addPixels(DCM_OBJECT ** object, char *pixelfile);
static int fileSize(int fd);
static void parseNumericString(char *, DCM_ELEMENT **);
void yyparse(void);
DCM_VALUEREPRESENTATION contextVR = DCM_CTX;

main(int argc, char **argv)
{
    CONDITION
	cond;
    char
       *pixelfile = NULL,	/* file with pixel data */
       *modfile = NULL;		/* input file with data that consists of
				 * elements that need to be modified in the
				 * input object */
    long
        options = DCM_ORDERLITTLEENDIAN;	/* default */
    long
        outputOptions = DCM_ORDERLITTLEENDIAN;	/* default */
    CTNBOOLEAN pixelsOnly = FALSE;

    yyin = stdin;		/* default input file */
    while (--argc > 0 && (*++argv)[0] == '-') {
	switch (*(argv[0] + 1)) {
	case 'b':		/* use BIG ENDIAN */
	    options &= ~DCM_ORDERMASK;
	    options |= DCM_ORDERBIGENDIAN;
	    break;
	case 'c':           /* Set type for context values */
	case 'C':
	 argc--; argv++;
	 if (argc < 1)              /* must have an output file */
	   usageerror();
	 if (strcmp(*argv, "SS") == 0) {
	    contextVR = DCM_SS;
	 } else if (strcmp(*argv, "US") == 0) {
	    contextVR = DCM_SS;
	 } else {
	   usageerror();
	 }

	 break;

	case 'i':		/* use input file */
	case 'I':
	    argc--;
	    argv++;
	    modfile = *argv;
	    yyin = fopen(modfile, "r");
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
	case 't':
	    options &= ~DCM_FILEFORMATMASK;
	    options |= DCM_PART10FILE;
	    break;
	case 'T':
	    outputOptions &= ~DCM_FILEFORMATMASK;
	    outputOptions |= DCM_PART10FILE;
	    break;
	case 'v':		/* verbose option */
	case 'V':
	    verbose = TRUE;
	    break;
	case 'x':
	    pixelsOnly = TRUE;
	    break;
	default:
	    fprintf(stderr, "Unrecognized option: %c\n", *(argv[0] + 1));
	    break;
	}
    }
    if (argc < 2)		/* must have an input and output file */
	usageerror();

    THR_Init();

    /* open the input file that needs to be modified */
    cond = DCM_OpenFile(*argv, options, &object);
    if (cond != DCM_NORMAL) {
	fprintf(stderr, "DCM_OpenFile: failed: %s\n", *argv);
	COND_DumpConditions();
	if ((options & DCM_FILEFORMATMASK) != DCM_PART10FILE) {
	    fprintf(stderr, "Now trying part 10 format\n");
	    options &= ~DCM_FILEFORMATMASK;
	    options |= DCM_PART10FILE;
	    cond = DCM_OpenFile(*argv, options, &object);
	    if (cond != DCM_NORMAL) {
		fprintf(stderr, "DCM_OpenFile: failed: %s\n", *argv);
		COND_DumpConditions();
	    }
	}
    }
    if (cond != DCM_NORMAL) {
	THR_Shutdown();
	exit(1);
    }
    argv++;
    /*
     * parse the input and build the list of elements to be added to an
     * object
     */
    if (!pixelsOnly) {
	yyparse();


	if (verbose) {
	    printf("*****In main program, after parse*****\n");
	    APP_DumpModList(&modlist, 0);
	}
	/* make modifications to the object as per the mod list */
	cond = makeModifications(&modlist, &object);
	if (cond != APP_NORMAL) {
	    printf("makeModifications: failed to make modifications\n");
	    COND_DumpConditions();
	    THR_Shutdown();
	    exit(1);
	}
    }
    /* make modifications to pixel data */
    if (pixelfile != NULL)
	addPixels(&object, pixelfile);

    if (verbose) {
	(void) DCM_DumpElements(&object, 0);
    }
    /* finally, write the modified object to output file */
    cond = DCM_WriteFile(&object, outputOptions, *argv);
    (void) DCM_CloseObject(&object);
    if (cond != DCM_NORMAL) {
	printf("DCM_WriteFile: failed to write modified object to file\n");
	COND_DumpConditions();
	THR_Shutdown();
	exit(1);
    }
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
Usage: dcm_modify_object [-b] [-i modfile] [-p pixels] [-t] [-T] [-v] [-x] filein fileout\n\
\n\
  -b    Use Big-Endian order to store the object (not Part 10) \n\
  -i    Use input file for the ASCII modifications instead of standard input\n\
  -p    Read raw pixel data from the file pixels \n\
  -t    Files are processed in DICOM Part 10 format \n\
  -T    Write output file in Part 10 format \n\
  -v    Verbose mode; place DCM facility in verbose mode \n\
  -x    Only process pixel data; there are no other modifications to make \n\
\n\
  filein   File that contains original DICOM object \n\
  fileout  FIle that stores the modified DICOM object \n";

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

    if (e->representation == DCM_CTX) {
      e->representation = contextVR;
    }

    switch (e->representation) {
    case DCM_DD:		/* Data set */
    case DCM_FD:		/* Floating double */
    case DCM_FL:		/* Float */
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
	if (strcmp(data, "#") == 0) {
	    e->length = 0;
	    e->d.string = "";
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
    case DCM_SQ:		/* Sequence of items */
    {
	char* xxx = "####";
	if (memcmp(vdata, xxx, 4) == 0) {
	    e->d.sq = LST_Create();
	} else {
	    e->d.sq = vdata;
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
addPixels(DCM_OBJECT ** obj, char *pixelfile)
{
    int
        fd,
        size,
	bytesRead,
	remainingBytes;
    unsigned char
       *p;

    DCM_ELEMENT e = {DCM_PXLPIXELDATA, DCM_OT, "", 0, 0, NULL};
    CONDITION
	cond;

    /* Remove pixel data if already there, ignoring errors */
    (void) DCM_RemoveElement(obj, DCM_PXLPIXELDATA);
    (void) COND_PopCondition(TRUE);

#ifdef _WIN32
    fd = open(pixelfile, O_RDONLY | O_BINARY);
#else
    fd = open(pixelfile, O_RDONLY);
#endif

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
    remainingBytes = size;
    while (remainingBytes > 0) {
      bytesRead = read(fd, p, remainingBytes);
      if (bytesRead <= 0) {
	perror(pixelfile);
	exit(1);
      }
      remainingBytes -= bytesRead;
      p += bytesRead;
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
APP_DumpModList(LST_HEAD ** lh, int level)
{
    APP_ELEMLIST
    * ael;
    APP_ITEMSEQ
	* ais;
    DCM_ELEMENT
	* e;
    CONDITION
	cond;

    ael = LST_Position(lh, LST_Head(lh));
    while (ael) {
	e = ael->e;
	cond = DCM_LookupElement(e);
	if (cond != DCM_NORMAL) {
	    printf("DCM_LookupElement: failed\n");
	    COND_DumpConditions();
	    exit(1);
	}
	printf("%*sTag = %x, Description = %s\n", level * 2, "", e->tag, e->description);
	switch (e->representation) {
	case DCM_SQ:
	    printf("%*s-----SEQUENCE---------\n", (level + 1) * 2, "");
	    ais = LST_Position(&e->d.sq, LST_Head(&e->d.sq));
	    while (ais) {
		printf("%*s===NEXT ITEM IN SEQUENCE===\n", (level + 1) * 2, "");
		APP_DumpModList(&ais->ael, level + 1);
		ais = LST_Next(&e->d.sq);
	    }
	    printf("%*s-----END SEQUENCE---------\n", (level + 1) * 2, "");
	    break;
	default:
	    break;
	}
	ael = LST_Next(lh);
    }
}

CONDITION
makeModifications(LST_HEAD ** lh, DCM_OBJECT ** obj)
{
    DCM_ELEMENT
	elem,
	*e;
    DCM_SEQUENCE_ITEM
	* dsi = NULL;
    APP_ELEMLIST
	* ael;
    APP_ITEMSEQ
	* ais;
    unsigned long
        rtnLength;
    void
       *ctx = NULL;
    CONDITION
	cond;
    LST_HEAD
	* l;
    LST_NODE* node;

    if (*lh == NULL) {
	/*
	 * no modifications were necessary in this object obj. We just
	 * return.
	 */
	return APP_NORMAL;
    }
    ael = LST_Position(lh, LST_Head(lh));
    while (ael) {
	e = ael->e;
	cond = DCM_LookupElement(e);
	if (cond != DCM_NORMAL) {
	    printf("makeModifications: DCM_LookupElement failed\n");
	    return cond;
	}
	switch (e->representation) {
	case DCM_SQ:
	    /* get the element from the main object */
	    memset(&elem, 0, sizeof(DCM_ELEMENT));
	    elem.tag = e->tag;
	    elem.representation = e->representation;
	    cond = DCM_GetSequenceList(obj, e->tag, &l);
	    if (cond == DCM_NORMAL) {
		/*
		 * l points to a list maintained in the object. A call to
		 * RemoveElement below will also indirectly screw up l that
		 * we maintain. Hence we must make a copy of that list
		 */
		elem.d.sq = LST_Create();
		if (elem.d.sq == NULL) {
		    printf("makeModifications: LST_Create failed\n");
		    return APP_FAILURE;
		}
		dsi = LST_Dequeue(&l);
		while (dsi != NULL) {
		    cond = LST_Enqueue(&elem.d.sq, dsi);
		    if (cond != LST_NORMAL) {
			printf("makeModifications: LST_Enqueue failed\n");
			return cond;
		    }
		    dsi = LST_Dequeue(&l);
		}
		/*
		 * now remove that element from the object so that we don't
		 * add it twice
		 */
		cond = DCM_RemoveElement(obj, e->tag);
		if (cond != DCM_NORMAL) {
		    printf("makeModifications: DCM_RemoveElement failed\n");
		    printf("\telement tag (%x, %x), decription (%s)\n",
			   DCM_TAG_GROUP(e->tag), DCM_TAG_ELEMENT(e->tag),
			   e->description);
		    return cond;
		}
#ifdef ASG
#endif
	    } else if (cond == DCM_ELEMENTNOTFOUND) {
		(void) COND_PopCondition(TRUE);
		elem.d.sq = LST_Create();
		if (elem.d.sq == NULL) {
		    printf("makeModifications: LST_Create failed\n");
		    return APP_FAILURE;
		}
	    } else {
		printf("makeModifications: DCM_GetElementValue failed\n");
		printf("\telement tag (%x, %x), decription (%s)\n",
		       DCM_TAG_GROUP(e->tag), DCM_TAG_ELEMENT(e->tag),
		       e->description);
		return cond;
	    }
	    node = LST_Head(&e->d.sq);
	    if (node != NULL) {
		ais = LST_Position(&e->d.sq, node);
	    } else {
		ais = NULL;
	    }
	    /*ais = LST_Position(&e->d.sq, LST_Head(&e->d.sq));*/
	    dsi = LST_Position(&elem.d.sq, LST_Head(&elem.d.sq));
	    while (ais) {
		if (dsi == NULL) {
		    /*
		     * no more sequence items in the original object. So the
		     * next list of mod items is just to be inserted in the
		     * object
		     */
		    dsi = (DCM_SEQUENCE_ITEM *) malloc(sizeof(DCM_SEQUENCE_ITEM));
		    if (dsi == NULL) {
			printf("makeModifications: Out of memory for SQ item\n");
			return APP_FAILURE;
		    }
		    cond = DCM_CreateObject(&dsi->object, 0);
		    if (cond != DCM_NORMAL) {
			printf("makeModifications: DCM_CreateObject failed\n");
			return cond;
		    }
		    cond = makeModifications(&ais->ael, &dsi->object);
		    if (cond != APP_NORMAL) {
			printf("makeModifications: failed to recursively modify\n");
			return cond;
		    }
		    cond = DCM_GetObjectSize(&dsi->object, &rtnLength);
		    if (rtnLength != 0) {
			elem.length += 8 + rtnLength;
			/* enqueue this seq item into the element sequence */
			cond = LST_Enqueue(&elem.d.sq, dsi);
			if (cond != LST_NORMAL) {
			    printf("makeModifications: LST_Enqueue failed\n");
			    return cond;
			}
		    }
		    dsi = NULL;
		} else {
		    /* modify elements in the current sequence */
		    cond = makeModifications(&ais->ael, &dsi->object);
		    if (cond != APP_NORMAL) {
			printf("makeModifications: failed to recursively modify\n");
			return cond;
		    }
		    cond = DCM_GetObjectSize(&dsi->object, &rtnLength);
		    elem.length += 8 + rtnLength;
		    dsi = LST_Next(&elem.d.sq);
		}
		ais = LST_Next(&e->d.sq);
	    }
	    /* Add the element to the object */
	    cond = DCM_AddElement(obj, &elem);
	    if (cond != DCM_NORMAL) {
		printf("makeModifications: DCM_AddElement failed\n");
		return cond;
	    }
#ifdef ASG
	    cond = DCM_ModifyElements(obj, &elem, 1, NULL, 0, NULL);
	    if (cond != DCM_NORMAL) {
		printf("makeModifications: DCM_ModifyElements failed\n");
		return cond;
	    }
#endif
	    break;
	default:
	    /*
	     * for any other element, simply modify the element in the object
	     * directly
	     */
	    cond = DCM_ModifyElements(obj, e, 1, NULL, 0, NULL);
	    if (cond != DCM_NORMAL) {
		printf("makeModifications: DCM_ModifyElements failed\n");
		printf("\telement tag (%x, %x), decription (%s)\n",
		       DCM_TAG_GROUP(e->tag), DCM_TAG_ELEMENT(e->tag),
		       e->description);
		return cond;
	    }
	    break;
	}
	ael = LST_Next(lh);
    }
    return APP_NORMAL;
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
    case DCM_AT:
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
