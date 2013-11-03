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
** Last Update:		$Author: smm $, $Date: 2001-01-03 21:39:07 $
** Source File:		$RCSfile: xutl.c,v $
** Revision:		$Revision: 1.8 $
** Status:		$State: Exp $
*/

static char rcsid[] = "$Revision: 1.8 $ $RCSfile: xutl.c,v $";

#include <stdio.h>
#include <stdlib.h>

#include "dicom.h"
#include "lst.h"
#include  "xutl.h"

static void
findRuntime(XUTL_STRUCT* s)
{
  char* c;

  c = getenv("CTN_TARGET");
  if (c == NULL)
    c = "/opt/ctn";

  sprintf(s->runtime, "%s/runtime/fontmap", c);
}

static void
addFont(XUTL_STRUCT* s, const char* charSet, const char* fontName)
{
  XUTL_FONTMAP* m;

  m = malloc(sizeof(*m));
  if (m == NULL) {
    fprintf(stderr, "Could not allocated %d bytes for fontmap \n",
	    sizeof(*m));
    exit(1);
  }

  strcpy(m->charset, charSet);
  strcpy(m->fontname, fontName);
  LST_Enqueue(&s->fontList, m);
}

static void
readFontMap(XUTL_STRUCT* s)
{
  FILE* f;
  char str[1024];
  XUTL_FONTMAP m;

  s->fontList = LST_Create();

  f = fopen(s->runtime, "r");
  if (f != 0) {

    while (fgets(str, sizeof(str), f) != 0) {
      if (str[0] == '#') continue;
      if (str[0] == '\r') continue;
      if (str[0] == '\n') continue;
      
      sscanf(str, "%s %s", m.charset, m.fontname);
      addFont(s, m.charset, m.fontname);
    }
    fclose(f);
  }

  addFont(s, "ISOIR6", "-adobe-helvetica-bold-r-normal--24-*");
  addFont(s, "ISOIR13", "-sony-fixed-medium-r-normal--16-*jisx0201*");
  addFont(s, "ISOIR14", "-sony-fixed-medium-r-normal--16-*jisx0201*");
  addFont(s, "ISOIR87",	"-jis-fixed-medium-r-normal--16-*jisx0208*");
  addFont(s, "ISOIR159", "-jis-fixed-medium-r-normal--16-*jisx0208*");
  addFont(s, "ISOIR149", "-daweoo-gothic-medium-r-normal--16-*ksc5601*");
}

static void
mapCharSetToFont(XUTL_STRUCT* s,
		 const char* charSet,
		 char* fontName)
{
  XUTL_FONTMAP* m;

  m = LST_Head(&s->fontList);
  LST_Position(&s->fontList, m);

  while (m != NULL) {
    if (strcmp(m->charset, charSet) == 0) {
      strcpy(fontName, m->fontname);
      return;
    }

    m = LST_Next(&s->fontList);
  }
  strcpy(fontName, "");
}

static void
createGC(XUTL_STRUCT* s)
{
  unsigned long valuemask;
  XColor color;
  XGCValues values;
  int screen = 0;

  Font fontISOIR6;
  Font fontISOIR13;
  Font fontISOIR14;
  Font fontISOIR87;
  Font fontISOIR159;
  Font fontISOIR149;

  char fontName[1024];

  screen = DefaultScreen(s->display);

  mapCharSetToFont(s, "ISOIR6", fontName);

  s->fsISOIR6 = XLoadQueryFont(s->display, fontName);
  fontISOIR6 = s->fsISOIR6->fid;

  if (fontISOIR6 == 0) {
    fprintf(stderr, "Could not load font: %s\n", fontName);
    exit(1);
  }


  mapCharSetToFont(s, "ISOIR13", fontName);

  s->fsISOIR13 = XLoadQueryFont(s->display, fontName);
  fontISOIR13 = s->fsISOIR13->fid;

  if (fontISOIR13 == 0) {
    fprintf(stderr, "Could not load font: %s\n", fontName);
  }

  mapCharSetToFont(s, "ISOIR14", fontName);

  s->fsISOIR14 = XLoadQueryFont(s->display, fontName);
  fontISOIR14 = s->fsISOIR14->fid;

  if (fontISOIR14 == 0) {
    fprintf(stderr, "Could not load font: %s\n", fontName);
  }

  mapCharSetToFont(s, "ISOIR87", fontName);
  s->fsISOIR87 = XLoadQueryFont(s->display, fontName);
  fontISOIR87 = s->fsISOIR87->fid;

  if (fontISOIR87 == 0) {
    fprintf(stderr, "Could not load font: %s\n", fontName);
  }

  mapCharSetToFont(s, "ISOIR159", fontName);
  s->fsISOIR159 = XLoadQueryFont(s->display, fontName);

  if (s->fsISOIR159 == 0) {
    fprintf(stderr, "Could not load font: %s\n", fontName);
    fontISOIR159 = 0;
  } else {
    fontISOIR159 = s->fsISOIR159->fid;
  }

  mapCharSetToFont(s, "ISOIR149", fontName);

  s->fsISOIR149 = XLoadQueryFont(s->display, fontName);
  if (s->fsISOIR149 == 0) {
    fprintf(stderr, "Could not load font: %s\n", fontName);
    fontISOIR149 = 0;
  } else {
    fontISOIR149 = s->fsISOIR149->fid;
  }

  valuemask = GCForeground | GCBackground | GCFont | GCFillStyle;

  XParseColor(s->display, s->colormap, "white", &color);
  XAllocColor(s->display, s->colormap, &color);
  values.foreground = color.pixel;

  XParseColor(s->display, s->colormap, "black", &color);
  XAllocColor(s->display, s->colormap, &color);
  values.background = color.pixel;

  values.fill_style = FillSolid;

  values.font = fontISOIR6;
  s->gcISOIR6 = XCreateGC(s->display,
			  RootWindow(s->display, screen),
			  valuemask,
			  &values);

  if (s->gcISOIR6 == NULL) {
    fprintf(stderr, "Could not create graphics context for ISOIR6.\n");
    exit(1);
  }

  if (fontISOIR13 != 0) {
    values.font = fontISOIR13;
    s->gcISOIR13 = XCreateGC(s->display,
			     RootWindow(s->display, screen),
			     valuemask,
			     &values);

    if (s->gcISOIR13 == NULL) {
      fprintf(stderr, "Could not create graphics context for ISOIR13.\n");
      exit(1);
    }
  } else {
    s->gcISOIR13 = NULL;
  }

  if (fontISOIR14 != 0) {
    values.font = fontISOIR14;
    s->gcISOIR14 = XCreateGC(s->display,
			     RootWindow(s->display, screen),
			     valuemask,
			     &values);

    if (s->gcISOIR14 == NULL) {
      fprintf(stderr, "Could not create graphics context for ISOIR14.\n");
      exit(1);
    }
  } else {
    s->gcISOIR14 = NULL;
  }

  if (fontISOIR87 != 0) {
    values.font = fontISOIR87;
    s->gcISOIR87 = XCreateGC(s->display,
			     RootWindow(s->display, screen),
			     valuemask,
			     &values);

    if (s->gcISOIR87 == NULL) {
      fprintf(stderr, "Could not create graphics context for ISOIR87.\n");
      exit(1);
    }
  } else {
    s->gcISOIR87 = NULL;
  }

  if (fontISOIR159 != 0) {
    values.font = fontISOIR159;
    s->gcISOIR159 = XCreateGC(s->display,
			      RootWindow(s->display, screen),
			      valuemask,
			      &values);

    if (s->gcISOIR159 == NULL) {
      fprintf(stderr, "Could not create graphics context for ISOIR159.\n");
      exit(1);
    }
  } else {
    s->gcISOIR159 = NULL;
  }

  if (fontISOIR149 != 0) {
    values.font = fontISOIR149;
    s->gcISOIR149 = XCreateGC(s->display,
			      RootWindow(s->display, screen),
			      valuemask,
			      &values);

    if (s->gcISOIR149 == NULL) {
      fprintf(stderr, "Could not create graphics context for ISOIR149.\n");
      exit(1);
    }
  } else {
    s->gcISOIR149 = NULL;
  }
}

CONDITION
XUTL_Open(int ncolors, XUTL_STRUCT** xStruct)
{
  XUTL_STRUCT* s = 0;
  int screen = 0;

  s = malloc(sizeof(XUTL_STRUCT));
  if (s == 0) {
    fprintf(stderr, "Could not allocate space for XUTL_STRUCT \n");
    exit(1);
  }

  s->display = XOpenDisplay(NULL);
  if (s->display == NULL) {
    fprintf(stderr, "Could not open X Display\n");
    exit(1);
  }

  screen = DefaultScreen(s->display);
  s->colormap = DefaultColormap(s->display, screen);

  findRuntime(s);
  readFontMap(s);
  createGC(s);

  *xStruct = s;

  return 0;
}


CONDITION
XUTL_DisplayString(XUTL_STRUCT** xStruct,
		   Window parentWindow,
		   GC graphicsContext,
		   int xPosition, int yPosition,
		   const char* str)
{
  XUTL_STRUCT *s;

  s = *xStruct;

  if (graphicsContext == 0)
    graphicsContext = s->gcISOIR6;

  XDrawString(s->display, parentWindow, graphicsContext,
	      xPosition, yPosition,
	      str, strlen(str));

  XFlush(s->display);

}

