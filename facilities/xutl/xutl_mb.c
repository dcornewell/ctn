/*
+-+-+-+-+-+-+-+-+-
*/
/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):
** Author, Date:
** Intent:
** Last Update:		$Author: smm $, $Date: 2000-01-04 19:56:06 $
** Source File:		$RCSfile: xutl_mb.c,v $
** Revision:		$Revision: 1.2 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.2 $ $RCSfile: xutl_mb.c,v $";

#include <stdio.h>
#include <stdlib.h>

#include "dicom.h"
#include "lst.h"
#include  "xutl.h"

#ifdef CTN_MULTIBYTE
static GC
mapCharacterToGC(XUTL_STRUCT** xStruct, CHR_CHARACTER mode)
{
  XUTL_STRUCT* x;
  x = *xStruct;

  if (mode == CHR_ASCII)
    return x->gcISOIR6;

  else if (mode == CHR_ISOIR13 && (x->gcISOIR13 != NULL))
     return x->gcISOIR13;

  else if (mode == CHR_ISOIR14 && (x->gcISOIR14 != NULL))
    return x->gcISOIR14;

  else if (mode == CHR_ISOIR87 && (x->gcISOIR87 != NULL))
    return x->gcISOIR87;

  else if (mode == CHR_ISOIR159 && (x->gcISOIR159 != NULL))
    return x->gcISOIR159;

  else if (mode == CHR_ISOIR149 && (x->gcISOIR149 != NULL))
    return x->gcISOIR149;

  return x->gcISOIR6;
}
#endif


#ifdef CTN_MULTIBYTE
static int
checkIfFontLoaded(XUTL_STRUCT** xStruct, CHR_CHARACTER mode)
{
  XUTL_STRUCT* x;
  x = *xStruct;

  if (mode == CHR_ASCII)
    return 1;

  else if (mode == CHR_ISOIR13 && (x->gcISOIR13 != NULL))
     return 1;

  else if (mode == CHR_ISOIR14 && (x->gcISOIR14 != NULL))
    return 1;

  else if (mode == CHR_ISOIR87 && (x->gcISOIR87 != NULL))
    return 1;

  else if (mode == CHR_ISOIR159 && (x->gcISOIR159 != NULL))
    return 1;

  else if (mode == CHR_ISOIR149 && (x->gcISOIR149 != NULL))
    return 1;

  return 1;
}

static void
xchars(const char* str, int count, CHR_CHARACTER mode,
       XUTL_STRUCT** xStruct, Window parentWindow,
       GC graphicsContext,
       int* xPosition, int* yPosition)
{
  XUTL_STRUCT* s;
  XChar2b theChar[256];
  int i = 0;
  XFontStruct* fontStruct = 0;
  int textWidth = 0;
  int fontFlag;

  s = *xStruct;

  fontFlag = checkIfFontLoaded(xStruct, mode);

  if (fontFlag == 0) {
    mode = CHR_ASCII;
    str = "Unable to load font for char set";
  }

  /* printf("%d %s\n", count, str); */

  for (i = 0; i < count; i++) {
    switch (mode) {
    case CHR_ASCII:
      fontStruct = s->fsISOIR6;
      theChar[i].byte1 = 0;
      theChar[i].byte2 = *str++;
      break;
    case CHR_ISOIR13:
      fontStruct = s->fsISOIR13;
      theChar[i].byte1 = 0;
      theChar[i].byte2 = *str++;
      break;
    case CHR_ISOIR14:
      fontStruct = s->fsISOIR14;
      theChar[i].byte1 = 0;
      theChar[i].byte2 = *str++;
      break;
    case CHR_ISOIR87:
      fontStruct = s->fsISOIR87;
      theChar[i].byte1 = *str++;
      theChar[i].byte2 = *str++;
      break;
    case CHR_ISOIR149:
      fontStruct = s->fsISOIR149;
      theChar[i].byte1 = *str++;
      theChar[i].byte2 = *str++;
      break;
    default:
     break;
    }
  }

  if (fontStruct == 0)
    return;

  XDrawString16(s->display, parentWindow,
		graphicsContext,
		*xPosition, *yPosition,
		theChar, count);

  textWidth = XTextWidth16(fontStruct, theChar, count);

  (*xPosition) += textWidth;

  XFlush(s->display);

}

CONDITION
XUTL_DisplayMBString(XUTL_STRUCT** xStruct,
		     Window parentWindow,
		     GC graphicsContext,
		     int xPosition, int yPosition,
		     const char* str, CHR_ENCODING encoding)
{
  char* dstString = 0;

  int l1 = 0;
  CONDITION cond;
  int dstLength = 0;
  CHR_ITERATOR_CONTEXT ctx;
  CHR_CHARACTER characterMode = CHR_ASCII;
  CHR_ENCODING startMode = CHR_ISO2022JP;
  int characterCount = 0;
  char subString[1024];
  GC localGC;

  l1 = strlen(str);

  dstString = (char*)malloc(2*l1 + 1);
  if (dstString == 0) {
    fprintf(stderr, "Could not malloc %d bytes in XUTL_DisplayMBString\n",
	    2*l1 + 1);
    exit(1);
  }

  switch (encoding) {
  case CHR_ISO2022JP:
    cond = CHR_Encode(str, l1, CHR_ISO2022JP,
		      dstString, 2*l1, CHR_EUC_JP, &dstLength);
    if (cond != CHR_NORMAL) {
      COND_DumpConditions();
      exit(1);
    }
    startMode = CHR_EUC_JP;
    break;

  case CHR_DEFAULTISOIR13:
    cond = CHR_Encode(str, l1, CHR_ISO2022JP,
		      dstString, 2*l1, CHR_EUC_JPROMAJI, &dstLength);
    if (cond != CHR_NORMAL) {
      COND_DumpConditions();
      exit(1);
    }
    startMode = CHR_EUC_JPROMAJI;
    break;
  case CHR_ISO2022KR:
    cond = CHR_Encode(str, l1, CHR_ISO2022KR,
		      dstString, 2*l1, CHR_EUC_KR, &dstLength);
    if (cond != CHR_NORMAL) {
      COND_DumpConditions();
      exit(1);
    }
    startMode = CHR_EUC_KR;
    break;

  default:
    fprintf(stderr, "Could not encode %d\n", encoding);
    exit(1);
  }

  cond = CHR_IterateBegin(dstString, dstLength,
			  startMode, &ctx);
  while (cond == CHR_NORMAL) {
    CHR_CHARACTER charSet;
    unsigned char mbChar[5];
    int mbLength = 0;

    cond = CHR_NextCharacter(&ctx, &charSet, mbChar, &mbLength);
    if (cond != CHR_NORMAL) {
      COND_DumpConditions();
      exit(1);
    }
    if (mbLength == 0)
      break;

    if ((charSet != characterMode) && (characterCount != 0)) {
      localGC = mapCharacterToGC(xStruct, characterMode);

      xchars(subString, characterCount, characterMode,
	     xStruct, parentWindow, localGC,
	     &xPosition, &yPosition);
      yPosition += 0;
      subString[0] = '\0';
      characterCount = 0;
    }

    characterMode = charSet;
    switch(characterMode) {
    case CHR_ASCII:
      subString[characterCount] = mbChar[0];
      subString[++characterCount] = '\0';
      break;
    case CHR_ISOIR13:
      subString[characterCount] = mbChar[1];  /* [0] is SS2 */
      subString[++characterCount] = '\0';
      break;
    case CHR_ISOIR14:
      subString[characterCount] = mbChar[0];
      subString[++characterCount] = '\0';
      break;
    case CHR_ISOIR87:
      subString[2*characterCount] = mbChar[0] & 0x7f;
      subString[2*characterCount + 1] = mbChar[1] & 0x7f;
      characterCount++;
      subString[2*characterCount] = '\0';
      break;
    case CHR_ISOIR159:
      subString[2*characterCount] = mbChar[1] & 0x7f;
      subString[2*characterCount + 1] = mbChar[2] & 0x7f;
      characterCount++;
      subString[2*characterCount] = '\0';
      break;
    case CHR_ISOIR149:
      subString[2*characterCount] = mbChar[0] & 0x7f;
      subString[2*characterCount + 1] = mbChar[1] & 0x7f;
      
      /*subString[2*characterCount] = mbChar[0] ;
      subString[2*characterCount + 1] = mbChar[1] ;
      */
      
      characterCount++;
      subString[2*characterCount] = '\0';
    default:
      break;
    }
  }

  if (characterCount != 0) {
    localGC = mapCharacterToGC(xStruct, characterMode);

    xchars(subString, characterCount, characterMode,
	   xStruct, parentWindow, localGC,
	   &xPosition, &yPosition);
    subString[0] = '\0';
    characterCount = 0;
  }

  return 1;
}
#endif
