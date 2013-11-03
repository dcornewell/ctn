#ifndef XUTL_ISIN
#define XUTL_ISIN
/*
+-+-+-+-+-+-+-+-+-
*/
/*
**		     Electronic Radiology Laboratory
**		   Mallinckrodt Institute of Radiology
**		Washington University School of Medicine
**
** Module Name(s):	XUTL: X Windows utilities
** Author, Date:	Steve Moore, 16-Oct-1999
** Intent:
** Last Update:		$Author: smm $, $Date: 1999-12-29 16:27:38 $
** Source File:		$RCSfile: xutl.h,v $
** Revision:		$Revision: 1.4 $
** Status:		$State: Exp $
*/

/* static char rcsid[] = "$Revision: 1.4 $ $RCSfile: xutl.h,v $"; */

#include	<X11/Xlib.h>
#include	<X11/Xutil.h>
#include	<X11/Xos.h>
#include	<X11/Xatom.h>

#ifdef CTN_MULTIBYTE
#include "dicom_chr.h"
#endif

typedef struct {
  int		colors;
  Display	*display;
  Colormap	colormap;

  XFontStruct*  fsISOIR6;
  XFontStruct*  fsISOIR13;
  XFontStruct*  fsISOIR14;
  XFontStruct*  fsISOIR87;
  XFontStruct*  fsISOIR159;
  XFontStruct*  fsISOIR149;

  GC		gcISOIR6;     /* ASCII */
  GC		gcISOIR13;    /* Katakana */
  GC		gcISOIR14;    /* Romaji */
  GC		gcISOIR87;    /* JIS X 208 Kanji */
  GC		gcISOIR159;   /* JIS X 212, Sup Kanji */
  GC		gcISOIR149;   /* KS X 1001 */

  XFontSet	fontSet;

  LST_HEAD*	fontList;
  char		runtime[1024];  /* Runtime directory */

} XUTL_STRUCT;

typedef struct {
  void* reserved[2];
  char charset[1024];
  char fontname[1024];
} XUTL_FONTMAP;

CONDITION
XUTL_Open(int ncolors, XUTL_STRUCT** xStruct);

CONDITION
XUTL_DisplayString(XUTL_STRUCT** xStruct,
		   Window parentWindow,
		   GC graphicsContext,
		   int xPosition, int yPosition,
		   const char* str);


#ifdef CTN_MULTIBYTE
CONDITION
XUTL_DisplayMBString(XUTL_STRUCT** xStruct,
		     Window parentWindow,
		     GC graphicsContext,
		     int xPosition, int yPosition,
		     const char* str, CHR_ENCODING encoding);
#endif

#endif
