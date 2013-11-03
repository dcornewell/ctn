#include <windows.h>
#include <string.h>
#include <stdio.h>

#include "dicom.h"
#include "ctnthread.h"
#include "lst.h"
#include "condition.h"
#include "dicom_objects.h"

#include "ctndisp.h"

static int dispPixelsX = 0;
static int dispPixelsY = 0;
static int dispBitsPixel = 0;
static int dispPlanes = 0;
static int dispColors = 0;

unsigned long graylut[256];
int G_ncolors;
int G_display_height;
int G_display_width;

ImageStruct currentImage;
static HPALETTE greyScalePalette;
static HBITMAP imageBitMap;
static int imageRows = 0;
static int imageCols = 0;

static void setPalette(HPALETTE* pal, int entries)
{
  static LPLOGPALETTE pLogPal;
  int i;
  int colorIntensity;

  pLogPal = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
					sizeof(LOGPALETTE) +
					(entries * sizeof(PALETTEENTRY)) );
  pLogPal->palVersion = 0x300;
  pLogPal->palNumEntries = entries;

  for (i = 0; i < entries; i++) {
	  colorIntensity = (255 * i) / (entries-1);
	  if (colorIntensity > 255)
		  colorIntensity = 255;
	  pLogPal->palPalEntry[i].peRed = colorIntensity;
	  pLogPal->palPalEntry[i].peGreen = colorIntensity;
	  pLogPal->palPalEntry[i].peBlue = colorIntensity;
	  pLogPal->palPalEntry[i].peFlags = PC_RESERVED;
	  //pLogPal->palPalEntry[i].peFlags = PC_NOCOLLAPSE;
  }
  *pal = CreatePalette(pLogPal);
}

void initializeDisplaySystem(HWND hwnd)
{
  HDC hdc;
  int i;

  THR_Init();

  hdc = GetDC(hwnd);
  dispPixelsX = GetDeviceCaps(hdc, HORZRES);
  dispPixelsY = GetDeviceCaps(hdc, VERTRES);
  dispBitsPixel = GetDeviceCaps(hdc, BITSPIXEL);
  dispPlanes = GetDeviceCaps(hdc, PLANES);
  dispColors = GetDeviceCaps(hdc, NUMCOLORS);

  G_display_height = dispPixelsY;
  G_display_width = dispPixelsX;
  G_ncolors = 255;

  for (i = 0; i < 256; i++)
	  graylut[i] = i;
}

static void dispImage8Bits(HWND hwnd, void* data, ImageStruct* imStruct)
{
  HBITMAP  hBitMap;
  HDC hDC, hMemDC;
  BITMAP bm;
  unsigned char* bits;
  int i, j, k;
  
  setPalette(&greyScalePalette, 64);

  bits = malloc(imStruct->ncols * imStruct->nrows);
  k = 0;
  for (i = 0; i < imStruct->nrows; i++) {
	  for (j = 0; j < imStruct->ncols; j++) {
		  bits[k] = ((unsigned char*)data)[k]/4 + 10;
		  k++;
	  }
  }

  bm.bmType		= 0;
  bm.bmWidth	= imStruct->ncols;
  bm.bmHeight	= imStruct->nrows;
  bm.bmWidthBytes = imStruct->ncols;
  bm.bmPlanes	= 1;
  bm.bmBitsPixel= 8;
  bm.bmBits = bits;

  hBitMap = CreateBitmapIndirect(&bm);
  hDC = GetDC(hwnd);
  hMemDC = CreateCompatibleDC(hDC);
  //SelectObject(hMemDC, hBitMap);
  SelectPalette(hDC, greyScalePalette, FALSE);
  RealizePalette(hDC);
  SelectObject(hMemDC, hBitMap);

  BitBlt(hDC,0, 0, imStruct->ncols, imStruct->nrows, hMemDC, 0, 0, SRCCOPY);
  ReleaseDC(hwnd, hDC);
  DeleteDC(hMemDC);
  DeleteObject(hBitMap);

}


static void dispImage16Bits(HWND hwnd, void* data, ImageStruct* imStruct)
{
  HDC hDC, hMemDC;
  BITMAP bm;
  unsigned short* bits;
  int i, j, k;
  HPALETTE lastPalette;
  UINT colorsMapped;
  int status;
  int sizePalette;
  int paletteReserved;
  int rasterCaps;
  unsigned short cValue;
  unsigned char* cData;

  //setPalette(&greyScalePalette, 128);

  bits = malloc(2*imStruct->ncols * imStruct->nrows);
  k = 0;
  cData = (unsigned char*)data;
  for (i = 0; i < imStruct->nrows; i++) {
	  for (j = 0; j < imStruct->ncols; j++) {
		  cValue = cData[k]/8;
		  cValue &= 0x1f;
		  cValue |= (cValue << 11) | (cValue << 6);
		  bits[k] = cValue;
		  //bits[k] = ((unsigned char*)data)[k]/2;
		  k++;
	  }
  }

  bm.bmType		= 0;
  bm.bmWidth	= imStruct->ncols;
  bm.bmHeight	= imStruct->nrows;
  bm.bmWidthBytes = imStruct->ncols * 2;
  bm.bmPlanes	= 1;
  bm.bmBitsPixel= 16;
  bm.bmBits = bits;

  imageBitMap = CreateBitmapIndirect(&bm);
  imageRows = imStruct->nrows;
  imageCols = imStruct->ncols;
  hDC = GetDC(hwnd);
  hMemDC = CreateCompatibleDC(hDC);
 
  SelectObject(hMemDC, imageBitMap);

  BitBlt(hDC,0, 0, imStruct->ncols, imStruct->nrows, hMemDC, 0, 0, SRCCOPY);
  ReleaseDC(hwnd, hDC);
  DeleteDC(hMemDC);
  //DeleteObject(hBitMap);

}

static void dispImage24Bits(HWND hwnd, void* data, ImageStruct* imStruct)
{
  HBITMAP  hBitMap;
  HDC hDC, hMemDC;
  BITMAP bm;
  unsigned char* bits;
  int i, j, k;
  int errorCode;
  SIZE size;
  HPALETTE lastPalette;
  UINT colorsMapped;
  int status;
  int sizePalette;
  int paletteReserved;
  int rasterCaps;


  //setPalette(&greyScalePalette, 256);

  bits = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
	  imStruct->ncols * imStruct->nrows * 3);
  k = 0;
  for (i = 0; i < imStruct->nrows; i++) {
	  for (j = 0; j < imStruct->ncols; j++) {
		  bits[k*3] = ((unsigned char*)data)[k];
		  bits[k*3+1] = ((unsigned char*)data)[k];
		  bits[k*3+2] = ((unsigned char*)data)[k];
		  k++;
	  }
  }

  bm.bmType		= 0;
  bm.bmWidth	= imStruct->ncols;
  bm.bmHeight	= imStruct->nrows;
  bm.bmWidthBytes = 3*imStruct->ncols;
  bm.bmPlanes	= 1;
  bm.bmBitsPixel= 24;
  bm.bmBits = bits;

  hDC = GetDC(hwnd);
  hBitMap = CreateBitmapIndirect(&bm);
  if (hBitMap == 0) {
	  errorCode = GetLastError();
	  hBitMap = CreateCompatibleBitmap(hDC, imStruct->ncols, imStruct->nrows);
	  GetBitmapDimensionEx(hBitMap, &size);
  }
  
  hMemDC = CreateCompatibleDC(hDC);
  //SelectObject(hMemDC, hBitMap);

  sizePalette = GetDeviceCaps(hDC, SIZEPALETTE);
  paletteReserved = GetDeviceCaps(hDC, NUMRESERVED);
  rasterCaps = GetDeviceCaps(hDC, RASTERCAPS);

  //lastPalette = SelectPalette(hDC, greyScalePalette, FALSE);
  //colorsMapped = RealizePalette(hDC);
  SelectObject(hMemDC, hBitMap);

  BitBlt(hDC,0, 0, imStruct->ncols, imStruct->nrows, hMemDC, 0, 0, SRCCOPY);
  ReleaseDC(hwnd, hDC);
  DeleteDC(hMemDC);
  DeleteObject(hBitMap);
}

void displayDICOMImage(HWND hwnd, const char* fileName)
{
  void *data;
  //HDC hdc;

  //hdc = GetDC(hwnd);

  currentImage.error_condition = FALSE;
  data = GetDICOMData(fileName, DCM_ORDERLITTLEENDIAN, &currentImage, 0x80000, 0x80000);
  data = ScaleImageData(data, &currentImage);
#if 0
  if (currentImage.bitsallocated > 8) {
	  unsigned short *s;
	  unsigned char *c;
	  int i;
	  s = data;
	  c = data;
	  for (i = 0; i < currentImage.ncols * currentImage.nrows; i++)
		  *c++ = (unsigned char)*s++;
  }
#endif
  switch (dispBitsPixel) {
  case 8:
	  dispImage8Bits(hwnd, data, &currentImage);
	  break;
  case 16:
	  dispImage16Bits(hwnd, data, &currentImage);
	  break;
  case 24:
	  dispImage24Bits(hwnd, data, &currentImage);
	  break;
  case 32:
	  break;
  }

}

void displayQueryNewPalette(HWND hwnd)
{
  HDC hDC;

  hDC = GetDC(hwnd);
  SelectPalette(hDC, greyScalePalette, FALSE);
  RealizePalette(hDC);
  ReleaseDC(hwnd, hDC);
}

void displayPaletteChanged(HWND hwnd)
{
  HDC hDC;

  hDC = GetDC(hwnd);
  SelectPalette(hDC, greyScalePalette, FALSE);
  RealizePalette(hDC);
  UpdateColors(hDC);
  ReleaseDC(hwnd, hDC);
}

void displayRepaint(HWND hwnd)
{

  HDC hDC, hMemDC;
  PAINTSTRUCT paint;

  //hDC = GetDC(hwnd);
  hDC = BeginPaint(hwnd, &paint);
  hMemDC = CreateCompatibleDC(hDC);
 
  SelectObject(hMemDC, imageBitMap);

  //BitBlt(hDC,0, 0, imageCols, imageRows, hMemDC, 0, 0, SRCCOPY);
  BitBlt(hDC, paint.rcPaint.left, paint.rcPaint.top,
	  paint.rcPaint.right-paint.rcPaint.left,	/* width */
	  paint.rcPaint.bottom - paint.rcPaint.top,	/* height */
	  hMemDC,
	  paint.rcPaint.left, paint.rcPaint.top,
	  SRCCOPY);

  ReleaseDC(hwnd, hDC);
  DeleteDC(hMemDC);
  //DeleteObject(hBitMap);


}
