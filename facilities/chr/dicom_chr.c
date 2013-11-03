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
**				DICOM 93
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	
** Last Update:		$Author: smm $, $Date: 2004-01-15 22:35:44 $
** Source File:		$RCSfile: dicom_chr.c,v $
** Revision:		$Revision: 1.5 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.5 $ $RCSfile: dicom_chr.c,v $";

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#ifdef GCCSUNOS
#include <unistd.h>
#endif

#if defined _MSC_VER
#include <io.h>
#include <fcntl.h>
#else
#include <sys/file.h>
#endif

#if defined _MSC_VER
#include <fcntl.h>
#elif defined SOLARIS
#include <sys/fcntl.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include "dicom.h"
#include "condition.h"
#include "dicom_chr.h"

#define ABORT(s) { fprintf(stderr, "%s\n", (s)); exit(1); }
#define ABORT2(s1, s2) { fprintf(stderr, (s1), (s2)); exit(1); }
#define CHECK_LENGTH(t, l, msg) if (t < l) { \
 fprintf(stderr, "Length error (%d:%d) %s\n", (t), (l), (msg)); exit(1); }
#define CHECK_LENGTH2(t, l, msg1, msg2) if (t < l) { \
 fprintf(stderr, "Length error (%d:%d) %s %s\n", (t), (l), (msg1), (msg2)); exit(1); }
#define CHECK_NEW_MODE(m, s, c) { \
  CONDITION cond; \
  cond = validateMode((m), (s), (c)); \
  if (cond != CHR_NORMAL) \
    return cond; \
}

static CTNBOOLEAN debug = FALSE;/* Flag for debugging messages to stdout */

typedef struct {
  unsigned char c1;
  unsigned char c2;
  unsigned char c3;
  CHR_CHARACTER m;
} CHR_MODE_MAP;

static CHR_MODE_MAP modeMap[] = {
  { 0x28, 0x42, 0x00, CHR_ASCII },		/* Single byte */
  { 0x28, 0x49, 0x00, CHR_ISOIR13 },		/* Single byte JIS X 0201: Katakana */
  { 0x28, 0x4a, 0x00, CHR_ISOIR14 },		/* Single byte JIS X 0201-1976: Romaji */
  { 0x24, 0x42, 0x00, CHR_ISOIR87 },		/* Double byte JIS X 0208: Kanji */
  { 0x24, 0x28, 0x44, CHR_ISOIR159},		/* Double byte JIS X 0212: Supp Kanji*/
  { 0x24, 0x29, 0x43, CHR_ISOIR149}		/* Double byte KS X 1001: Hangul */
};

typedef struct {
  CHR_CHARACTER m;
  char* description;
} CHR_SET_NAME;

static CONDITION
validateMode(CHR_CHARACTER charSet, const CHR_CHARACTER* legalSets, int count)
{
  int i = 0;
  int j = 0;
  char buf[256];

  CHR_SET_NAME names[] = {
    { CHR_ASCII, "ASCII" },
    { CHR_ISOIR13, "ISO IR 13: JIS X 0201: Katakana"},
    { CHR_ISOIR14, "ISO IR 14: JIS X 0201-1976: Romaji"},
    { CHR_ISOIR87, "ISO IR 87: JIS X 0208: Kanji"},
    { CHR_ISOIR159, "ISO IR 159: JIS X 0212: Supplementary Kanji"}
  };


  for (i = 0; i < count; i++) {
    if (charSet == legalSets[i])
      return CHR_NORMAL;
  }

  sprintf(buf, "<%d>", charSet);

  for (j = 0; j < (int)DIM_OF(names); j++) {
    if (charSet == names[j].m) {
      strcpy(buf, names[j].description);
      break;
    }
  }
  return COND_PushCondition(CHR_ILLEGALCHARACTERSET,
			    CHR_Message(CHR_ILLEGALCHARACTERSET),
			    buf);
}

static void mapMode(unsigned char c1, unsigned char c2, unsigned char c3,
		    CHR_CHARACTER* newMode, int* escapeLength)
{
  int i = 0;

  for (; i < (int)DIM_OF(modeMap); i++) {
    if ((c1 == modeMap[i].c1) && (c2 == modeMap[i].c2)) {
      if (modeMap[i].c3 == 0x00) {
	*newMode = modeMap[i].m;
	*escapeLength = 2;
	return;
      } else if (c3 == modeMap[i].c3) {
	*newMode = modeMap[i].m;
	*escapeLength = 3;
	return;
      }
    }
  }
  fprintf(stderr, "Could not map to a new mode: %02x %02x %02x\n", c1, c2, c3);
  exit(1);
}

static CONDITION x_ISO2022JP_EUCJP(const void* src, int srcLength,
				   void* dest, int destLength,
				   int* encodedLength)
{
  const unsigned char *s;
  unsigned char *d;
  int l = 0;
  CHR_CHARACTER currentMode = CHR_ASCII;
  unsigned char c;
  unsigned char c1;
  unsigned char c2;
  unsigned char c3;
  CHR_CHARACTER legalCharSets[] = { CHR_ISOIR13,
				    CHR_ASCII,
				    CHR_ISOIR87,
				    CHR_ISOIR159 };

  s = src;
  d = dest;
  while (srcLength > 0) {
    if (destLength < 5) {
      fprintf(stderr,
	      "In ISO2022JP_EUCJP, ran out of output buffer\n");
      exit(1);
    }
    switch (currentMode) {
    case CHR_ASCII:

      c = *s++; srcLength--;

      if (c == 0)
	ABORT("Found a 0 character in SB mode in ISO2022JP_EUCJP");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ASCII mode in ISO2022JP_EUCJP");
	c1 = s[0];
	c2 = s[1];
	c3 = (srcLength > 2) ? s[2] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));
	s += skipLength;
	srcLength -= skipLength;

#if 0
	fprintf(stderr, "Switch to mode: %d\n", currentMode);
#endif
      } else {
	*d++ = c; destLength--; l++;
#if 0
	fprintf(stderr, "%c\n",(char) c);
#endif
      }
      break;

    case CHR_ISOIR13:	/* JIS X 0201: Katakana */

      c = *s++; srcLength--;

      if (c == 0)
	ABORT("Found a 0 character in ISOIR13 mode in ISO2022JP_EUCJP");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ISOIR13 mode in ISO2022JP_EUCJP");
	c1 = s[0];
	c2 = s[1];
	c3 = (srcLength > 2) ? s[2] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));
	s += skipLength;
	srcLength -= skipLength;

#if 0
	fprintf(stderr, "Switch to mode: %d\n", currentMode);
#endif
      } else {
	*d++ = CHR_SS2; destLength--; l++;
	*d++ = c | 0x80; destLength--; l++;
#if 0
	fprintf(stderr, "%c\n",(char) c);
#endif
      }
      break;

    case CHR_ISOIR87:	/* JIS X 0208: Kanji */

      c = *s++; srcLength--;

      if (c == 0)
	ABORT("Found a 0 character in ISOIR87 mode in ISO2022JP_EUCJP");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ISOIR87 mode in ISO2022JP_EUCJP");
	c1 = s[0];
	c2 = s[1];
	c3 = (srcLength > 2) ? s[2] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));
	s += skipLength;
	srcLength -= skipLength;

#if 0
	fprintf(stderr, "Switch to mode: %d\n", currentMode);
#endif
      } else {
	*d++ = c | 0x80; destLength--; l++;
#if 0
	fprintf(stderr, "%02x", c);
#endif
	c = *s++; srcLength--;
	*d++ = c | 0x80; destLength--; l++;
#if 0
	fprintf(stderr, " %02x\n", c);
#endif
      }

      break;
    case CHR_ISOIR159:

      c = *s++; srcLength--;

      if (c == 0)
	ABORT("Found a 0 character in ISOIR87 mode in ISO2022JP_EUCJP");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ISOIR87 mode in ISO2022JP_EUCJP");
	c1 = s[0];
	c2 = s[1];
	c3 = (srcLength > 2) ? s[2] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));
	s += skipLength;
	srcLength -= skipLength;

#if 0
	fprintf(stderr, "Switch to mode: %d\n", currentMode);
#endif
      } else {
	*d++ = CHR_SS3; destLength--; l++;

	*d++ = c | 0x80; destLength--; l++;
#if 0
	fprintf(stderr, "%02x", c);
#endif
	c = *s++; srcLength--;
	*d++ = c | 0x80; destLength--; l++;
#if 0
	fprintf(stderr, " %02x\n", c);
#endif
      }

      break;
    default:
      fprintf(stderr,
	      "In ISO2022JP_EUCJP, mode should not be: %d\n",
	      currentMode);
      exit(1);
      break;
    }
  }
  *d = 0x00;
  *encodedLength = l;

  return CHR_NORMAL;
}


static CONDITION
x_ISO2022JP_EUCJPROMAJI(const void* src, int srcLength,
			void* dest, int destLength, int* encodedLength)
{
  CONDITION cond;
  const unsigned char *s;
  unsigned char *d;
  int l = 0;
  CHR_CHARACTER currentMode = CHR_ISOIR14;
  unsigned char c;
  unsigned char c1;
  unsigned char c2;
  unsigned char c3;
  CHR_CHARACTER legalCharSets[] = { CHR_ISOIR13,
				    CHR_ISOIR14,
				    CHR_ISOIR87,
				    CHR_ISOIR159 };
  s = src;
  d = dest;
  while (srcLength > 0) {
    if (destLength < 5) {
      fprintf(stderr,
	      "In ISO2022JP_EUCJP, ran out of output buffer\n");
      exit(1);
    }
    switch (currentMode) {
    case CHR_ISOIR14:	/* JIS X 0201-1976: Romaji */

      c = *s++; srcLength--;

      if (c == 0)
	ABORT("Found a 0 character in SB mode in ISO2022JP_EUCJP");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ASCII mode in ISO2022JP_EUCJP");
	c1 = s[0];
	c2 = s[1];
	c3 = (srcLength > 2) ? s[2] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));

	s += skipLength;
	srcLength -= skipLength;

#if 0
	fprintf(stderr, "Switch to mode: %d\n", currentMode);
#endif
      } else {
	if ((c & 0x80) != 0) {              /* G1 is Katakana */
	  *d++ = CHR_SS2; destLength--; l++;
	  *d++ = c; destLength--; l++;
	} else {
	  *d++ = c; destLength--; l++;
	}
#if 0
	fprintf(stderr, "%c\n",(char) c);
#endif
      }
      break;

    case CHR_ISOIR13:	/* JIS X 0201: Katakana */

      c = *s++; srcLength--;

      if (c == 0)
	ABORT("Found a 0 character in ISOIR13 mode in ISO2022JP_EUCJP");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ISOIR13 mode in ISO2022JP_EUCJP");
	c1 = s[0];
	c2 = s[1];
	c3 = (srcLength > 2) ? s[2] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));
	s += skipLength;
	srcLength -= skipLength;

#if 0
	fprintf(stderr, "Switch to mode: %d\n", currentMode);
#endif
      } else {
	*d++ = CHR_SS2; destLength--; l++;
	*d++ = c | 0x80; destLength--; l++;
#if 0
	fprintf(stderr, "%c\n",(char) c);
#endif
      }
      break;

    case CHR_ISOIR87:	/* JIS X 0208: Kanji */

      c = *s++; srcLength--;

      if (c == 0)
	ABORT("Found a 0 character in ISOIR87 mode in ISO2022JP_EUCJP");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ISOIR87 mode in ISO2022JP_EUCJP");
	c1 = s[0];
	c2 = s[1];
	c3 = (srcLength > 2) ? s[2] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));
	s += skipLength;
	srcLength -= skipLength;

#if 0
	fprintf(stderr, "Switch to mode: %d\n", currentMode);
#endif
      } else {
	*d++ = c | 0x80; destLength--; l++;
#if 0
	fprintf(stderr, "%02x", c);
#endif
	c = *s++; srcLength--;
	*d++ = c | 0x80; destLength--; l++;
#if 0
	fprintf(stderr, " %02x\n", c);
#endif
      }

      break;
    case CHR_ISOIR159:

      c = *s++; srcLength--;

      if (c == 0)
	ABORT("Found a 0 character in ISOIR87 mode in ISO2022JP_EUCJP");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ISOIR87 mode in ISO2022JP_EUCJP");
	c1 = s[0];
	c2 = s[1];
	c3 = (srcLength > 2) ? s[2] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));
	s += skipLength;
	srcLength -= skipLength;

#if 0
	fprintf(stderr, "Switch to mode: %d\n", currentMode);
#endif
      } else {
	*d++ = CHR_SS3; destLength--; l++;

	*d++ = c | 0x80; destLength--; l++;
#if 0
	fprintf(stderr, "%02x", c);
#endif
	c = *s++; srcLength--;
	*d++ = c | 0x80; destLength--; l++;
#if 0
	fprintf(stderr, " %02x\n", c);
#endif
      }

      break;
    default:
      fprintf(stderr,
	      "In ISO2022JP_EUCJP, mode should not be: %d\n",
	      currentMode);
      exit(1);
      break;
    }
  }
  *d = 0x00;
  *encodedLength = l;

  return CHR_NORMAL;
}

static CONDITION
x_DEFAULTISOIR13_EUCJPROMAJI(const void* src, int srcLength,
			void* dest, int destLength,
			int* encodedLength)
{
  const unsigned char *s;
  unsigned char *d;
  int l = 0;
  CHR_CHARACTER currentMode = CHR_ISOIR14;
  unsigned char c;
  unsigned char c1;
  unsigned char c2;
  unsigned char c3;
  CHR_CHARACTER legalCharSets[] = { CHR_ISOIR13,
				    CHR_ISOIR14,
				    CHR_ISOIR87,
				    CHR_ISOIR159 };

  char fname[] = "DEFAULTISOIR13_EUCJPROMAJI";

  s = src;
  d = dest;
  while (srcLength > 0) {
    if (destLength < 5) {
      fprintf(stderr,
	      "In %s, ran out of output buffer\n", fname);
      exit(1);
    }
    switch (currentMode) {
    case CHR_ISOIR14:
      c = *s++; srcLength--;

      if (c == 0)
	ABORT2("Found a 0 character in SB mode in %s\n", fname);

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH2(srcLength, 2, "ISOIR14 mode in ", fname);
	c1 = s[0];
	c2 = s[1];
	c3 = (srcLength > 2) ? s[2] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));
	s += skipLength;
	srcLength -= skipLength;

#if 0
	fprintf(stderr, "Switch to mode: %d\n", currentMode);
#endif
      } else {
	if ((c & 0x80) != 0) {
	  *d++ = CHR_SS2; destLength--; l++;
	  *d++ = c | 0x80; destLength--; l++;
	  currentMode = CHR_ISOIR13;
	} else {
	  *d++ = c; destLength--; l++;
	  currentMode = CHR_ISOIR14;
	}
#if 0
	fprintf(stderr, "%c\n",(char) c);
#endif
      }
      break;

    case CHR_ISOIR13:	/* JIS X 0201: Katakana */

      c = *s++; srcLength--;

      if (c == 0)
	ABORT("Found a 0 character in ISOIR13 mode in ISO2022JP_EUCJP");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ISOIR13 mode in ISO2022JP_EUCJP");
	c1 = s[0];
	c2 = s[1];
	c3 = (srcLength > 2) ? s[2] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));
	s += skipLength;
	srcLength -= skipLength;

#if 0
	fprintf(stderr, "Switch to mode: %d\n", currentMode);
#endif
      } else {
	if ((c & 0x80) != 0) {
	  currentMode = CHR_ISOIR13;
	  *d++ = CHR_SS2; destLength--; l++;
	  *d++ = c | 0x80; destLength--; l++;
	} else {
	  *d++ = c; destLength--; l++;
	  currentMode = CHR_ISOIR14;
	}
#if 0
	fprintf(stderr, "%c\n",(char) c);
#endif
      }
      break;

    case CHR_ISOIR87:	/* JIS X 0208: Kanji */

      c = *s++; srcLength--;

      if (c == 0)
	ABORT("Found a 0 character in ISOIR87 mode in ISO2022JP_EUCJP");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ISOIR87 mode in ISO2022JP_EUCJP");
	c1 = s[0];
	c2 = s[1];
	c3 = (srcLength > 2) ? s[2] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));
	s += skipLength;
	srcLength -= skipLength;

#if 0
	fprintf(stderr, "Switch to mode: %d\n", currentMode);
#endif
      } else {
	*d++ = c | 0x80; destLength--; l++;
#if 0
	fprintf(stderr, "%02x", c);
#endif
	c = *s++; srcLength--;
	*d++ = c | 0x80; destLength--; l++;
#if 0
	fprintf(stderr, " %02x\n", c);
#endif
      }

      break;
    case CHR_ISOIR159:

      c = *s++; srcLength--;

      if (c == 0)
	ABORT("Found a 0 character in ISOIR87 mode in ISO2022JP_EUCJP");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ISOIR87 mode in ISO2022JP_EUCJP");
	c1 = s[0];
	c2 = s[1];
	c3 = (srcLength > 2) ? s[2] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));
	s += skipLength;
	srcLength -= skipLength;

#if 0
	fprintf(stderr, "Switch to mode: %d\n", currentMode);
#endif
      } else {
	*d++ = CHR_SS3; destLength--; l++;

	*d++ = c | 0x80; destLength--; l++;
#if 0
	fprintf(stderr, "%02x", c);
#endif
	c = *s++; srcLength--;
	*d++ = c | 0x80; destLength--; l++;
#if 0
	fprintf(stderr, " %02x\n", c);
#endif
      }

      break;
    default:
      fprintf(stderr,
	      "In ISO2022JP_EUCJP, mode should not be: %d\n",
	      currentMode);
      exit(1);
      break;
    }
  }
  *d = 0x00;
  *encodedLength = l;

  return CHR_NORMAL;
}

static CONDITION
x_DEFAULTISOIR13_ISO2022JP(const void* src, int srcLength,
			   void* dest, int destLength, int* encodedLength)
{
  CONDITION cond;
  const unsigned char *s;
  unsigned char *d;
  int l = 0;
  CHR_CHARACTER currentMode = CHR_ISOIR14;
  unsigned char c;
  unsigned char c1;
  unsigned char c2;
  unsigned char c3;
  CHR_CHARACTER legalCharSets[] = { CHR_ISOIR13,
				    CHR_ISOIR14,
				    CHR_ISOIR87,
				    CHR_ISOIR159 };

  FILE* f1;

  /*f1 = fopen("/tmp/xxx", "w");*/

  s = src;
  d = dest;
  *d++ = CHR_ESCAPE; destLength--;      /* Switch to ISO-IR 14 */
  *d++ = 0x28; destLength--;
  *d++ = 0x4a; destLength--;
  l += 3;

  while (srcLength > 0) {
    if (destLength < 5) {
      fprintf(stderr,
	      "In ISO2022JP_DEFAULTISOIR13, ran out of output buffer\n");
      exit(1);
    }
    switch (currentMode) {

    case CHR_ISOIR14:	/* JIS X 0201-1976: Romaji */

      c = *s++; srcLength--;

      if (c == 0)
	ABORT("Found a 0 character in SB mode in ISO2022JP_EUCJP");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ASCII mode in ISO2022JP_EUCJP");
	c1 = s[0];
	c2 = s[1];
	c3 = (srcLength > 2) ? s[2] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));

	*d++ = c; destLength--; l++;
	memcpy(d, s, skipLength); destLength -= skipLength; l += skipLength;
	d += skipLength;

	s += skipLength;
	srcLength -= skipLength;

#if 0
	fprintf(stderr, "Switch to mode: %d\n", currentMode);
#endif
      } else {

	if ((c & 0x80) == 0x80) {    /* We are in ISO-IR 13, G1 */
	  *d++ = CHR_ESCAPE; destLength--; l++;
	  /**d++ = 0x29; destLength--; l++;*/
	  *d++ = 0x28; destLength--; l++;
	  *d++ = 0x49; destLength--; l++;
	  *d++ = c & 0x7f; destLength--; l++;
	  currentMode = CHR_ISOIR13;
	} else {
	  *d++ = c; destLength--; l++;
	}

#if 0
	fprintf(stderr, "%c\n",(char) c);
#endif
      }
      break;

    case CHR_ISOIR13:	/* JIS X 0201: Katakana */

      c = *s++; srcLength--;

      if (c == 0)
	ABORT("Found a 0 character in ISOIR13 mode in ISO2022JP_EUCJP");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ISOIR13 mode in ISO2022JP_EUCJP");
	c1 = s[0];
	c2 = s[1];
	c3 = (srcLength > 2) ? s[2] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));

	*d++ = c; destLength--; l++;
	memcpy(d, s, skipLength); destLength -= skipLength; l += skipLength;
	d += skipLength;

	s += skipLength;
	srcLength -= skipLength;

#if 0
	fprintf(stderr, "Switch to mode: %d\n", currentMode);
#endif
      } else {
	if ((c & 0x80) == 0x80) {    /* We are in ISO-IR 13, G1 */
	  *d++ = c & 0x7f; destLength--; l++;
	} else {
	  *d++ = CHR_ESCAPE;  destLength--; l++;
	  *d++ = 0x28; destLength--; l++;
	  *d++ = 0x4a; destLength--; l++;
	  *d++ = c; destLength--; l++;
	  currentMode = CHR_ISOIR14;
	}
		    
#if 0
	fprintf(stderr, "%c\n",(char) c);
#endif
      }
      break;

    case CHR_ISOIR87:	/* JIS X 0208: Kanji */

      c = *s++; srcLength--;

      if (c == 0)
	ABORT("Found a 0 character in ISOIR87 mode in ISO2022JP_EUCJP");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ISOIR87 mode in ISO2022JP_EUCJP");
	c1 = s[0];
	c2 = s[1];
	c3 = (srcLength > 2) ? s[2] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));

	*d++ = c; destLength--; l++;
	memcpy(d, s, skipLength); destLength -= skipLength; l += skipLength;
	d += skipLength;

	s += skipLength;
	srcLength -= skipLength;

#if 0
	fprintf(stderr, "Switch to mode: %d\n", currentMode);
#endif
      } else {
	*d++ = c; destLength--; l++;
	c = *s++; srcLength--;
	*d++ = c; destLength--; l++;
      }

      break;
    case CHR_ISOIR159:

      c = *s++; srcLength--;

      if (c == 0)
	ABORT("Found a 0 character in ISOIR87 mode in ISO2022JP_EUCJP");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ISOIR87 mode in ISO2022JP_EUCJP");
	c1 = s[0];
	c2 = s[1];
	c3 = (srcLength > 2) ? s[2] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));

	*d++ = c; destLength--; l++;
	memcpy(d, s, skipLength); destLength -= skipLength; l += skipLength;
	d += skipLength;

	s += skipLength;
	srcLength -= skipLength;

#if 0
	fprintf(stderr, "Switch to mode: %d\n", currentMode);
#endif
      } else {
	*d++ = c; destLength--; l++;
	c = *s++; srcLength--;
	*d++ = c; destLength--; l++;
      }

      break;
    default:
      fprintf(stderr,
	      "In ISO2022JP_EUCJP, mode should not be: %d\n",
	      currentMode);
      exit(1);
      break;
    }
  }

  if (currentMode != CHR_ASCII) {
    *d++ = CHR_ESCAPE; destLength--; l++;
    *d++ = 0x28; destLength--; l++;
    *d++ = 0x42; destLength--; l++;
    currentMode = CHR_ASCII;
  }

  *d = 0x00;
  *encodedLength = l;

  return CHR_NORMAL;
}

static CONDITION
x_NextCharacterISO2022JP(CHR_ITERATOR_CONTEXT* ctx, CHR_CHARACTER* charSet,
			    void* dst, int* length)
{
  const unsigned char *s = 0;
  unsigned char *d = 0;
  int l = 0;
  CHR_CHARACTER currentMode = CHR_ASCII;
  unsigned char c;
  unsigned char c1;
  unsigned char c2;
  unsigned char c3;
  int copyLength = 1;
  int srcLength = 0;
  CHR_CHARACTER legalCharSets[] = { CHR_ISOIR13,
				    CHR_ASCII,
				    CHR_ISOIR87,
				    CHR_ISOIR159 };

  currentMode = ctx->sMode;
  s = &ctx->s[ctx->sOffset];
  srcLength = ctx->sLength - ctx->sOffset;
  {
    switch (currentMode) {
    case CHR_ASCII:

      *charSet = CHR_ASCII;
      c = *s;

      if (c == 0)
	ABORT("Found a 0 character in SB mode in ISO2022JP_EUCJP");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ASCII mode in ISO2022JP_EUCJP");
	c1 = s[1];
	c2 = s[2];
	c3 = (srcLength > 3) ? s[3] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));
	copyLength = skipLength + 1;
	ctx->sMode = currentMode;

#if 0
	fprintf(stderr, "Switch to mode: %d\n", currentMode);
#endif
      } else {
	copyLength = 1;
#if 0
	fprintf(stderr, "%c\n",(char) c);
#endif
      }
      break;

    case CHR_ISOIR13:	/* JIS X 0201: Katakana */

      *charSet = CHR_ISOIR13;
      c = *s;

      if (c == 0)
	ABORT("Found a 0 character in ISOIR13 mode in ISO2022JP_EUCJP");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ISOIR13 mode in ISO2022JP_EUCJP");
	c1 = s[1];
	c2 = s[2];
	c3 = (srcLength > 3) ? s[3] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));
	s += skipLength;
	srcLength -= skipLength;

#if 0
	fprintf(stderr, "Switch to mode: %d\n", currentMode);
#endif
      } else {
#if REPAIR
	*d++ = CHR_SS2; destLength--; l++;
	*d++ = c | 0x80; destLength--; l++;
#endif
#if 0
	fprintf(stderr, "%c\n",(char) c);
#endif
      }
      break;

    case CHR_ISOIR87:	/* JIS X 0208: Kanji */

      *charSet = CHR_ISOIR87;
      c = *s;

      if (c == 0)
	ABORT("Found a 0 character in ISOIR87 mode in ISO2022JP_EUCJP");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ISOIR87 mode in ISO2022JP_EUCJP");
	c1 = s[1];
	c2 = s[2];
	c3 = (srcLength > 3) ? s[3] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));
	copyLength = skipLength + 1;
	ctx->sMode = currentMode;

#if 0
	fprintf(stderr, "Switch to mode: %d\n", currentMode);
#endif
      } else {
	copyLength = 2;
      }

      break;
    case CHR_ISOIR159:

      *charSet = CHR_ISOIR159;
      c = *s;

      if (c == 0)
	ABORT("Found a 0 character in ISOIR87 mode in ISO2022JP_EUCJP");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ISOIR87 mode in ISO2022JP_EUCJP");
	c1 = s[1];
	c2 = s[2];
	c3 = (srcLength > 3) ? s[3] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));
	s += skipLength;
	srcLength -= skipLength;

#if 0
	fprintf(stderr, "Switch to mode: %d\n", currentMode);
#endif
      } else {
#if REPAIR
	*d++ = CHR_SS3; destLength--; l++;

	*d++ = c | 0x80; destLength--; l++;
#endif
#if 0
	fprintf(stderr, "%02x", c);
#endif
#if REPAIR
	c = *s++; srcLength--;
	*d++ = c | 0x80; destLength--; l++;
#endif
#if 0
	fprintf(stderr, " %02x\n", c);
#endif
      }

      break;
    default:
      fprintf(stderr,
	      "In ISO2022JP_EUCJP, mode should not be: %d\n",
	      currentMode);
      exit(1);
      break;
    }
  }

  *length = copyLength;
  ctx->sOffset += copyLength;
  d = dst;
  while(copyLength-- > 0)
    *d++ = *s++;

  return CHR_NORMAL;
}

/* Translate from ISO 2022 (Korean) to EUC Korean */

static CONDITION
x_ISO2022KR_EUCKR(const void* src, int srcLength,
		  void* dest, int destLength,
		  int* encodedLength)
{
  const unsigned char *s;
  unsigned char *d;
  int l = 0;
  CHR_CHARACTER currentMode = CHR_ASCII;
  unsigned char c;
  unsigned char c1;
  unsigned char c2;
  unsigned char c3;
  CHR_CHARACTER legalCharSets[] = { CHR_ISOIR149,
				    CHR_ASCII };

  s = src;
  d = dest;
  while (srcLength > 0) {
    if (destLength < 5) {
      fprintf(stderr,
	      "In x_ISO2022KR_EUCKR, ran out of output buffer\n");
      exit(1);
    }
    switch (currentMode) {
    case CHR_ASCII:

      c = *s++; srcLength--;

      if (c == 0)
	ABORT("Found a 0 character in SB mode in x_ISO2022KR_EUCKR");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ASCII mode in x_ISO2022KR_EUCKR");
	c1 = s[0];
	c2 = s[1];
	c3 = (srcLength > 2) ? s[2] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));
	s += skipLength;
	srcLength -= skipLength;

      } else {
	if ((c & 0x80) == 0x80) {
	  fprintf(stderr,
		  "In x_ISO2022KR_EUCKR, we are in ASCII mode and found a character with the MSB set (%02x)\n",
		  c);
	  exit(1);
	}
	*d++ = c; destLength--; l++;
      }
      break;

    case CHR_ISOIR149:	/* KS X 1001 */

      c = *s++; srcLength--;

      if (c == 0)
	ABORT("Found a 0 character in ISOIR149 mode in x_ISO2022KR_EUCKR");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ISOIR149 mode in x_ISO2022KR_EUCKR");
	c1 = s[0];
	c2 = s[1];
	c3 = (srcLength > 2) ? s[2] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));
	s += skipLength;
	srcLength -= skipLength;

      } else {
	if ((c & 0x80) == 0x00) {  /* This is one char in the G0 set */
	  *d++ = c; destLength--; l++;
	} else {
	  *d++ = c; destLength--; l++;
	  *d++ = *s++; srcLength--; destLength--; l++;
	}
      }
      break;

    default:
      fprintf(stderr,
	      "In x_ISO2022KR_EUCKR, mode should not be: %d\n",
	      currentMode);
      exit(1);
      break;
    }
  }
  *d = 0x00;
  *encodedLength = l;

  return CHR_NORMAL;
}


static CONDITION x_EUCJP_ISO2022JP(const void* src, int srcLength,
				   void* dest, int destLength, int* encodedLength)
{
  const unsigned char *s;
  unsigned char *d;
  int l = 0;
  CHR_CHARACTER currentMode = CHR_ASCII;
  unsigned char c;
  unsigned char c1;
  unsigned char c2;
  unsigned char c3;

  s = src;
  d = dest;

  while (srcLength > 0) {
    c = *s++; srcLength--;
    if (c == CHR_SS2) {   /* CHR_ISOIR13, JIS X 0201: Katakana */
      if (currentMode != CHR_ISOIR13) {
	*d++ = CHR_ESCAPE; destLength--; l++;
	*d++ = 0x28; destLength--; l++;
	*d++ = 0x49; destLength--; l++;
	currentMode = CHR_ISOIR13;
      }
      *d++ = c & 0x7F; destLength--; l++;

    } else if (c == CHR_SS3) { 	/* CHR_ISOIR159,  Double byte JIS X 0212: Supp Kanji*/
      if (currentMode != CHR_ISOIR159) {
	*d++ = CHR_ESCAPE; destLength--; l++;
	*d++ = 0x24; destLength--; l++;
	*d++ = 0x28; destLength--; l++;
	*d++ = 0x44; destLength--; l++;
	currentMode = CHR_ISOIR159;
      }
      *d++ = c & 0x7F; destLength--; l++;
      c = *s++; srcLength--;
      *d++ = c & 0x7F; destLength--; l++;

    } else if ((c & 0x80) != 0) { /* CHR_ISOIR87, JIS X 0208: Kanji */
      if (currentMode != CHR_ISOIR87) {
	*d++ = CHR_ESCAPE; destLength--; l++;
	*d++ = 0x24; destLength--; l++;
	*d++ = 0x42; destLength--; l++;
	currentMode = CHR_ISOIR87;
      }
      *d++ = c & 0x7F; destLength--; l++;
      c = *s++; srcLength--;
      *d++ = c & 0x7F; destLength--; l++;
    } else {
      if (currentMode != CHR_ASCII) {
	*d++ = CHR_ESCAPE; destLength--; l++;
	*d++ = 0x28; destLength--; l++;
	*d++ = 0x42; destLength--; l++;
	currentMode = CHR_ASCII;
      }
      *d++ = c; destLength--; l++;
    }
  }

  if (currentMode != CHR_ASCII) {
    *d++ = CHR_ESCAPE; destLength--; l++;
    *d++ = 0x28; destLength--; l++;
    *d++ = 0x42; destLength--; l++;
    currentMode = CHR_ASCII;
  }

  *d = 0x00;
  *encodedLength = l;

  return CHR_NORMAL;
}

static CONDITION
x_EUCJPROMAJI_ISO2022JP(const void* src, int srcLength,
				   void* dest, int destLength, int* encodedLength)
{
  const unsigned char *s;
  unsigned char *d;
  int l = 0;
  CHR_CHARACTER currentMode = CHR_ISOIR14;
  unsigned char c;
  unsigned char c1;
  unsigned char c2;
  unsigned char c3;

  s = src;
  d = dest;

  while (srcLength > 0) {
    c = *s++; srcLength--;
    if (c == CHR_SS2) {   /* CHR_ISOIR13, JIS X 0201: Katakana */
      if (currentMode != CHR_ISOIR13) {
	*d++ = CHR_ESCAPE; destLength--; l++;
	*d++ = 0x28; destLength--; l++;
	*d++ = 0x49; destLength--; l++;
	currentMode = CHR_ISOIR13;
      }
      *d++ = c & 0x7F; destLength--; l++;

    } else if (c == CHR_SS3) { 	/* CHR_ISOIR159,  Double byte JIS X 0212: Supp Kanji*/
      if (currentMode != CHR_ISOIR159) {
	*d++ = CHR_ESCAPE; destLength--; l++;
	*d++ = 0x24; destLength--; l++;
	*d++ = 0x28; destLength--; l++;
	*d++ = 0x44; destLength--; l++;
	currentMode = CHR_ISOIR159;
      }
      *d++ = c & 0x7F; destLength--; l++;
      c = *s++; srcLength--;
      *d++ = c & 0x7F; destLength--; l++;

    } else if ((c & 0x80) != 0) { /* CHR_ISOIR87, JIS X 0208: Kanji */
      if (currentMode != CHR_ISOIR87) {
	*d++ = CHR_ESCAPE; destLength--; l++;
	*d++ = 0x24; destLength--; l++;
	*d++ = 0x42; destLength--; l++;
	currentMode = CHR_ISOIR87;
      }
      *d++ = c & 0x7F; destLength--; l++;
      c = *s++; srcLength--;
      *d++ = c & 0x7F; destLength--; l++;
    } else {
      if (currentMode != CHR_ISOIR14) {
	*d++ = CHR_ESCAPE; destLength--; l++;
	*d++ = 0x28; destLength--; l++;
	*d++ = 0x4A; destLength--; l++;
	currentMode = CHR_ASCII;
      }
      *d++ = c; destLength--; l++;
    }
  }

  if (currentMode != CHR_ISOIR87) {
    *d++ = CHR_ESCAPE; destLength--; l++;
    *d++ = 0x28; destLength--; l++;
    *d++ = 0x4A; destLength--; l++;
    currentMode = CHR_ISOIR14;
  }

  *d = 0x00;
  *encodedLength = l;

  return CHR_NORMAL;
}

static CONDITION
x_EUCJPROMAJI_DEFAULTISOIR13(const void* src, int srcLength,
				   void* dest, int destLength, int* encodedLength)
{
  const unsigned char *s;
  unsigned char *d;
  int l = 0;
  /*CHR_CHARACTER currentMode = CHR_ISOIR14;*/
  CHR_CHARACTER g1Mode = CHR_ISOIR13;
  CHR_CHARACTER g0Mode = CHR_ISOIR14;
  unsigned char c;
  unsigned char c1;
  unsigned char c2;
  unsigned char c3;

  char fname[] = "EUCJPROMAJI_DEFAULTISOIR13";

  s = src;
  d = dest;

  while (srcLength > 0) {
    c = *s++; srcLength--;
    if (c == CHR_SS2) {   /* CHR_ISOIR13, JIS X 0201: Katakana */
      if (g1Mode != CHR_ISOIR13) {
	*d++ = CHR_ESCAPE; destLength--; l++;
	*d++ = 0x28; destLength--; l++;
	*d++ = 0x49; destLength--; l++;
	g1Mode = CHR_ISOIR13;
      }
      c = *s++; srcLength--;
      *d++ = c | 0x80; destLength--; l++;

    } else if (c == CHR_SS3) { 	/* CHR_ISOIR159,  Double byte JIS X 0212: Supp Kanji*/
      if (g0Mode != CHR_ISOIR159) {
	*d++ = CHR_ESCAPE; destLength--; l++;
	*d++ = 0x24; destLength--; l++;
	*d++ = 0x28; destLength--; l++;
	*d++ = 0x44; destLength--; l++;
	g0Mode = CHR_ISOIR159;
      }
      *d++ = c & 0x7F; destLength--; l++;
      c = *s++; srcLength--;
      *d++ = c & 0x7F; destLength--; l++;

    } else if ((c & 0x80) != 0) { /* CHR_ISOIR87, JIS X 0208: Kanji */
      if (g0Mode != CHR_ISOIR87) {
	*d++ = CHR_ESCAPE; destLength--; l++;
	*d++ = 0x24; destLength--; l++;
	*d++ = 0x42; destLength--; l++;
	g0Mode = CHR_ISOIR87;
      }
      *d++ = c & 0x7F; destLength--; l++;
      c = *s++; srcLength--;
      *d++ = c & 0x7F; destLength--; l++;
    } else {
      if (g0Mode != CHR_ISOIR14) {
	*d++ = CHR_ESCAPE; destLength--; l++;
	*d++ = 0x28; destLength--; l++;
	*d++ = 0x4a; destLength--; l++;
	g0Mode = CHR_ISOIR14;
      }
      *d++ = c; destLength--; l++;
    }
  }

  if (g0Mode != CHR_ISOIR14) {
    *d++ = CHR_ESCAPE; destLength--; l++;
    *d++ = 0x28; destLength--; l++;
    *d++ = 0x4A; destLength--; l++;
    g0Mode = CHR_ISOIR14;
  }

  *d = 0x00;
  *encodedLength = l;

  return CHR_NORMAL;
}

/* Translate from EUC Korean to ISO 2022 (Korean) */

static CONDITION
x_EUCKR_ISO2022KR(const void* src, int srcLength,
		  void* dest, int destLength, int* encodedLength)
{
  const unsigned char *s;
  unsigned char *d;
  int l = 0;
  CHR_CHARACTER currentMode = CHR_ASCII;
  unsigned char c;
  unsigned char c1;
  unsigned char c2;
  unsigned char c3;

  s = src;
  d = dest;

  while (srcLength > 0) {
    c = *s++; srcLength--;
    if ((c & 0x80) != 0) { /* CHR_ISOIR149, KS X 1001 */
      if (currentMode != CHR_ISOIR149) {
	*d++ = CHR_ESCAPE; destLength--; l++;
	*d++ = 0x24; destLength--; l++;
	*d++ = 0x29; destLength--; l++;
	*d++ = 0x43; destLength--; l++;
	currentMode = CHR_ISOIR149;
      }
      *d++ = c; destLength--; l++;
      c = *s++; srcLength--;
      *d++ = c; destLength--; l++;
    } else {
      if (currentMode != CHR_ASCII) {   /* Shift back to G0 */
	/* We don't need an escape sequence to go back */
	/* We do set the mode to ASCII so we can be ready to */
	/* shift back to ISORI 149 */
	currentMode = CHR_ASCII;
      }
      *d++ = c; destLength--; l++;
    }
  }

  *d = 0x00;
  *encodedLength = l;

  return CHR_NORMAL;
}



/* These are the private functions that validate characters in an
** input set (for translation to an output set)
*/

static void
formatHex(char* hex, int count, const unsigned char* c)
{
  int i;
  for (i = 0; i < count; i++, c++) {
    sprintf(hex, " %02x", *c);
    hex += 3;
  }
  for (i = count; i < 4; i++) {
    strcat(hex, "   ");
    hex += 3;
  }
}

static CONDITION validate_ISO2022JP_EUCJP(const void* src, int srcLength,
					  int verbose)
{
  const unsigned char *s;
  int l = 0;
  CHR_CHARACTER currentMode = CHR_ASCII;
  unsigned char c;
  unsigned char c1;
  unsigned char c2;
  unsigned char c3;
  CHR_CHARACTER legalCharSets[] = { CHR_ISOIR13,
				    CHR_ASCII,
				    CHR_ISOIR87,
				    CHR_ISOIR159 };

  FILE *f = stdout;
  char* modeString = "";
  s = src;

  if (verbose) {
    fprintf(f, "    Mode  Hex\n");
  }
  while (srcLength > 0) {
    char hexRepresentation[20] = "";
    char textRepresentation[20] = "";

    switch (currentMode) {
    case CHR_ASCII:
      modeString = "   ASCII";

      c = *s++; srcLength--;

      if (c == 0)
	ABORT("Found a 0 character in SB mode in ISO2022JP_EUCJP");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;

	CHECK_LENGTH(srcLength, 2, "ASCII mode in ISO2022JP_EUCJP");
	c1 = s[0];
	c2 = s[1];
	c3 = (srcLength > 2) ? s[2] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));
	formatHex(hexRepresentation, skipLength + 1, s-1);
	strcpy(textRepresentation, "<ESC> ...");
	s += skipLength;
	srcLength -= skipLength;

      } else {
	formatHex(hexRepresentation, 1, s-1);
	textRepresentation[0] = c;
	textRepresentation[1] = '\0';
      }
      fprintf(f, "%s %s %s\n",
	      modeString, hexRepresentation, textRepresentation);
      break;

    case CHR_ISOIR13:	/* JIS X 0201: Katakana */
      modeString = " ISOIR13";

      c = *s++; srcLength--;

      if (c == 0)
	ABORT("Found a 0 character in ISOIR13 mode in ISO2022JP_EUCJP");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ISOIR13 mode in ISO2022JP_EUCJP");
	c1 = s[0];
	c2 = s[1];
	c3 = (srcLength > 2) ? s[2] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));
	formatHex(hexRepresentation, skipLength + 1, s-1);
	strcpy(textRepresentation, "<ESC> ...");
	s += skipLength;
	srcLength -= skipLength;

      } else {
	formatHex(hexRepresentation, 1, s-1);
	textRepresentation[0] = CHR_SS2;
	textRepresentation[1] = c | 0x80;
	textRepresentation[2] = '\0';
      }
      fprintf(f, "%s %s %s\n",
	      modeString, hexRepresentation, textRepresentation);
      break;

    case CHR_ISOIR87:	/* JIS X 0208: Kanji */
      modeString = " ISOIR87";

      c = *s++; srcLength--;

      if (c == 0)
	ABORT("Found a 0 character in ISOIR87 mode in ISO2022JP_EUCJP");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ISOIR87 mode in ISO2022JP_EUCJP");
	c1 = s[0];
	c2 = s[1];
	c3 = (srcLength > 2) ? s[2] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));
	formatHex(hexRepresentation, skipLength + 1, s-1);
	strcpy(textRepresentation, "<ESC> ...");
	s += skipLength;
	srcLength -= skipLength;

      } else {
	formatHex(hexRepresentation, 2, s-1);
	textRepresentation[0] = c | 0x80;
	c = *s++; srcLength--;
	textRepresentation[1] = c | 0x80;
	textRepresentation[2] = '\0';
      }
      fprintf(f, "%s %s %s\n",
	      modeString, hexRepresentation, textRepresentation);

      break;
    case CHR_ISOIR159:
      modeString = "ISOIR159";

      c = *s++; srcLength--;

      if (c == 0)
	ABORT("Found a 0 character in ISOIR87 mode in ISO2022JP_EUCJP");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ISOIR87 mode in ISO2022JP_EUCJP");
	c1 = s[0];
	c2 = s[1];
	c3 = (srcLength > 2) ? s[2] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));
	formatHex(hexRepresentation, skipLength + 1, s-1);
	strcpy(textRepresentation, "<ESC> ...");
	s += skipLength;
	srcLength -= skipLength;

      } else {
	formatHex(hexRepresentation, 2, s-1);

	textRepresentation[0] = CHR_SS3;
	textRepresentation[1] = c | 0x80;
	c = *s++; srcLength--;
	textRepresentation[2] = c | 0x80;
	textRepresentation[3] = '\0';
      }
      fprintf(f, "%s %s %s\n",
	      modeString, hexRepresentation, textRepresentation);

      break;
    default:
      fprintf(stderr,
	      "In ISO2022JP_EUCJP, mode should not be: %d\n",
	      currentMode);
      exit(1);
      break;
    }
  }
#if 0
  *d = 0x00;
  *encodedLength = l;
#endif

  return CHR_NORMAL;
}


static CONDITION
validate_ISO2022JP_EUCJPROMAJI(const void* src, int srcLength,
			       int verbose)
{
  CONDITION cond;
  const unsigned char *s;
  CHR_CHARACTER currentMode = CHR_ISOIR14;
  unsigned char c;
  unsigned char c1;
  unsigned char c2;
  unsigned char c3;
  CHR_CHARACTER legalCharSets[] = { CHR_ISOIR13,
				    CHR_ISOIR14,
				    CHR_ISOIR87,
				    CHR_ISOIR159 };

  FILE *f = stdout;
  char* modeString = "";
  s = src;

  if (verbose) {
    fprintf(f, "    Mode Hex\n");
  }

  while (srcLength > 0) {
    char hexRepresentation[20] = "";
    char textRepresentation[20] = "";

    switch (currentMode) {
    case CHR_ISOIR14:	/* JIS X 0201-1976: Romaji */
      modeString = " ISOIR14";
      c = *s++; srcLength--;

      if (c == 0)
	ABORT("Found a 0 character in SB mode in ISO2022JP_EUCJP");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ASCII mode in ISO2022JP_EUCJP");
	c1 = s[0];
	c2 = s[1];
	c3 = (srcLength > 2) ? s[2] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));
	formatHex(hexRepresentation, skipLength + 1, s-1);
	strcpy(textRepresentation, "<ESC> ...");
	s += skipLength;
	srcLength -= skipLength;

      } else {
	formatHex(hexRepresentation, 1, s-1);
	if ((c & 0x80) != 0) {         /* G1 is Katakana */
	  textRepresentation[0] = CHR_SS2;
	  textRepresentation[1] = c;
	  textRepresentation[2] = '\0';
	} else {
	  textRepresentation[0] = c;
	  textRepresentation[1] = '\0';
	}
      }
      fprintf(f, "%s %s %s\n",
	      modeString, hexRepresentation, textRepresentation);
      break;

    case CHR_ISOIR13:	/* JIS X 0201: Katakana */
      modeString = " ISOIR13";

      c = *s++; srcLength--;

      if (c == 0)
	ABORT("Found a 0 character in ISOIR13 mode in ISO2022JP_EUCJP");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ISOIR13 mode in ISO2022JP_EUCJP");
	c1 = s[0];
	c2 = s[1];
	c3 = (srcLength > 2) ? s[2] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));
	formatHex(hexRepresentation, skipLength + 1, s-1);
	strcpy(textRepresentation, "<ESC> ...");
	s += skipLength;
	srcLength -= skipLength;

      } else {
	formatHex(hexRepresentation, 1, s-1);
	textRepresentation[0] = CHR_SS2;
	textRepresentation[1] = c | 0x80;
	textRepresentation[2] = '\0';
      }
      fprintf(f, "%s %s %s\n",
	      modeString, hexRepresentation, textRepresentation);
      break;

    case CHR_ISOIR87:	/* JIS X 0208: Kanji */
      modeString = " ISOIR87";

      c = *s++; srcLength--;

      if (c == 0)
	ABORT("Found a 0 character in ISOIR87 mode in ISO2022JP_EUCJP");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ISOIR87 mode in ISO2022JP_EUCJP");
	c1 = s[0];
	c2 = s[1];
	c3 = (srcLength > 2) ? s[2] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));
	formatHex(hexRepresentation, skipLength + 1, s-1);
	strcpy(textRepresentation, "<ESC> ...");
	s += skipLength;
	srcLength -= skipLength;

      } else {
	formatHex(hexRepresentation, 2, s-1);
	textRepresentation[0] = c | 0x80;
	c = *s++; srcLength--;
	textRepresentation[1] = c | 0x80;
	textRepresentation[2] = '\0';
      }
      fprintf(f, "%s %s %s\n",
	      modeString, hexRepresentation, textRepresentation);
      break;
    case CHR_ISOIR159:
      modeString = "ISOIR159";

      c = *s++; srcLength--;

      if (c == 0)
	ABORT("Found a 0 character in ISOIR87 mode in ISO2022JP_EUCJP");

      if (c == CHR_ESCAPE) {
	int skipLength = 0;
	CHECK_LENGTH(srcLength, 2, "ISOIR87 mode in ISO2022JP_EUCJP");
	c1 = s[0];
	c2 = s[1];
	c3 = (srcLength > 2) ? s[2] : 0x00;

	mapMode(c1, c2, c3, &currentMode, &skipLength);
	CHECK_NEW_MODE(currentMode, legalCharSets, (int)DIM_OF(legalCharSets));
	formatHex(hexRepresentation, skipLength + 1, s-1);
	strcpy(textRepresentation, "<ESC> ...");
	s += skipLength;
	srcLength -= skipLength;

      } else {
	formatHex(hexRepresentation, 2, s-1);

	textRepresentation[0] = CHR_SS3;
	textRepresentation[1] = c | 0x80;
	c = *s++; srcLength--;
	textRepresentation[2] = c | 0x80;
	textRepresentation[3] = '\0';
      }
      fprintf(f, "%s %s %s\n",
	      modeString, hexRepresentation, textRepresentation);

      break;
    default:
      fprintf(stderr,
	      "In ISO2022JP_EUCJP, mode should not be: %d\n",
	      currentMode);
      exit(1);
      break;
    }
  }
#if 0
  *d = 0x00;
  *encodedLength = l;
#endif
  return CHR_NORMAL;
}












/* This is the beginning of the public functions */
#if 0
CONDITION CHR_Translate(const void* src, int srcLength, CHR_ENCODING srcCode,
			void* dest, int destLength, CHR_ENCODING destCode,
			int* encodedLength)
{
  CONDITION rtnCode = CHR_UNSUPPORTEDTRANSLATION;

  switch (srcCode) {
  case CHR_ISO2022JP:
    if (destCode == CHR_EUC_JP)
      rtnCode = x_ISO2022JP_EUCJP(src, srcLength, dest, destLength, encodedLength);
    break;

  case CHR_EUC_JP:
    if (destCode == CHR_ISO2022JP)
      rtnCode = x_EUCJP_ISO2022JP(src, srcLength, dest, destLength, encodedLength);
    break;
  default:
    break;
  }

  return rtnCode;
}
#endif

CONDITION CHR_Encode(const void* src, int srcLength, CHR_ENCODING srcCode,
		     void* dest, int destLength, CHR_ENCODING destCode,
		     int* encodedLength)
{
  CONDITION rtnCode = CHR_UNSUPPORTEDTRANSLATION;

  switch (srcCode) {
  case CHR_ISO2022JP:
    if (destCode == CHR_EUC_JP)
      rtnCode = x_ISO2022JP_EUCJP(src, srcLength,
				  dest, destLength, encodedLength);
    else if (destCode == CHR_EUC_JPROMAJI)
      rtnCode = x_ISO2022JP_EUCJPROMAJI(src, srcLength,
					dest, destLength, encodedLength);

    break;

  case CHR_EUC_JP:
    if (destCode == CHR_ISO2022JP)
      rtnCode = x_EUCJP_ISO2022JP(src, srcLength,
				  dest, destLength, encodedLength);
    break;

  case CHR_EUC_JPROMAJI:
    if (destCode == CHR_ISO2022JP)
      rtnCode = x_EUCJPROMAJI_ISO2022JP(src, srcLength,
					dest, destLength, encodedLength);
    else if (destCode == CHR_DEFAULTISOIR13)
      rtnCode = x_EUCJPROMAJI_DEFAULTISOIR13(src, srcLength,
					dest, destLength, encodedLength);
    break;
  case CHR_DEFAULTISOIR13:
    if (destCode == CHR_ISO2022JP)
      rtnCode = x_DEFAULTISOIR13_ISO2022JP(src, srcLength,
					   dest, destLength, encodedLength);
    else if (destCode == CHR_EUC_JPROMAJI)
      rtnCode = x_DEFAULTISOIR13_EUCJPROMAJI(src, srcLength,
					   dest, destLength, encodedLength);
    break;
  case CHR_ISO2022KR:
    if (destCode == CHR_EUC_KR)
      rtnCode = x_ISO2022KR_EUCKR(src, srcLength,
				  dest, destLength, encodedLength);
    break;
  case CHR_EUC_KR:
    if (destCode == CHR_ISO2022KR)
      rtnCode = x_EUCKR_ISO2022KR(src, srcLength,
				  dest, destLength, encodedLength);
    break;
  default:
    break;
  }

  return rtnCode;
}


CONDITION CHR_IterateBegin(const void* src, int length,
			   CHR_ENCODING code, CHR_ITERATOR_CONTEXT* ctx)
{
  if ((code != CHR_EUC_JP) &&
      (code != CHR_EUC_JPROMAJI) &&
      (code != CHR_EUC_KR) &&
      (code != CHR_ISO2022JP) )
    return 0;  /* repair */

  ctx->s = src;
  ctx->sLength = length;
  ctx->sOffset = 0;
  ctx->sMode = 0;
  ctx->encoding = code;

  return CHR_NORMAL;

}
CONDITION CHR_NextCharacter(CHR_ITERATOR_CONTEXT* ctx, CHR_CHARACTER* charSet,
			    void* s, int* length)
{
  unsigned char c;
  unsigned char* dst;
  const unsigned char* src;
  int copyLength = 0;

  if (ctx->sOffset >= ctx->sLength) {
    *length = 0;
    return CHR_NORMAL;
  }

  src = &ctx->s[ctx->sOffset];
  dst = (unsigned char*) s;
  c = *src;

  if (ctx->encoding == CHR_ISO2022JP) {
    return x_NextCharacterISO2022JP(ctx, charSet, s, length);
  } else if (ctx->encoding == CHR_EUC_KR) {
    if ((c & 0x80) != 0) {  /* CHR_ISOIR149, KS X 1001 */
      *charSet = CHR_ISOIR149;
      copyLength = 2;
    } else {
      *charSet = CHR_ASCII;
      copyLength = 1;
    }
  } else {
    if (c == CHR_SS2) {   /* CHR_ISOIR13, JIS X 0201: Katakana */
      *charSet = CHR_ISOIR13;
      copyLength = 2;
    } else if (c == CHR_SS3) { 	/* CHR_ISOIR159,  Double byte JIS X 0212: Supp Kanji*/
      *charSet = CHR_ISOIR159;
      copyLength = 3;
    } else if ((c & 0x80) != 0) { /* CHR_ISOIR87, JIS X 0208: Kanji */
      *charSet = CHR_ISOIR87;
      copyLength = 2;
    } else {
      *charSet = CHR_ASCII;
      copyLength = 1;
    }
  }

  *length = copyLength;
  ctx->sOffset += copyLength;
  while(copyLength-- > 0)
    *dst++ = *src++;

  return CHR_NORMAL;
}

CONDITION CHR_ValidateEncoding(const void* src, int srcLength,
			       CHR_ENCODING srcCode, CHR_ENCODING destCode,
			       int verbose)
{
  CONDITION rtnCode = CHR_UNSUPPORTEDTRANSLATION;

  switch (srcCode) {
  case CHR_ISO2022JP:
    if (destCode == CHR_EUC_JP)
      rtnCode = validate_ISO2022JP_EUCJP(src, srcLength, verbose);
    else if (destCode == CHR_EUC_JPROMAJI)
      rtnCode = validate_ISO2022JP_EUCJPROMAJI(src, srcLength, verbose);

    break;

  case CHR_EUC_JP:
    break;
  }

  return rtnCode;
}


typedef struct vector {
    CONDITION cond;
    char *message;
}   VECTOR;

static VECTOR messageVector[] = {
    {CHR_NORMAL, "Normal return from CHR routine"},
    {CHR_UNSUPPORTEDTRANSLATION, "Unable to translate: (%s %s)"},
    {CHR_ILLEGALCHARACTERSET,
     "Character set: %s is not legal in this encoding"},
    {0, NULL}
};


/* CHR_Message
**
** Purpose:
**	This function accepts a CONDITION as an input parameter and finds
**	the ASCIZ message that is defined for that CONDITION.  If the
**	CONDITION is defined for this facility, this function returns
**	a pointer to the ASCIZ string which describes the condition.
**	If the CONDITION is not found, the function returns NULL.
**
** Parameter Dictionary:
**	condition	The CONDITION used to search the dictionary.
**
** Return Values:
**	ASCIZ string which describes the condtion requested by the caller
**	NULL if the condition is not found
**
*/
char *
CHR_Message(CONDITION condition)
{
    int
        index;

    for (index = 0; messageVector[index].message != NULL; index++)
	if (condition == messageVector[index].cond)
	    return messageVector[index].message;

    return NULL;
}
