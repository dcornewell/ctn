#include <windows.h>
#include <string.h>
#include <stdio.h>
#include "menu_windows.h"

extern void initializeDisplaySystem(HWND hwnd);
extern void displayDICOMImage(HWND hwnd, const char* fileName);
extern void displayQueryNewPalette(HWND hwnd);
extern void displayPaletteChanged(HWND hwnd);
extern void displayRepaint(HWND hwnd);

LRESULT CALLBACK WindowFunc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hPrevInst,
				LPSTR lpszArgs, int nWinMode)
{
  HWND hwnd;
  MSG msg;
  WNDCLASSEX wcl;

  /* Define a window class */
  wcl.cbSize = sizeof(WNDCLASSEX);
  wcl.hInstance = hThisInst;
  wcl.lpszClassName = "MyWin";
  wcl.lpfnWndProc = WindowFunc;
  wcl.style = 0;

  wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wcl.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
  wcl.hCursor = LoadCursor(NULL, IDC_ARROW);

  wcl.lpszMenuName = "smm";

  wcl.cbClsExtra = 0;
  wcl.cbWndExtra = 0;

  wcl.hbrBackground = GetStockObject(WHITE_BRUSH);

  if (!RegisterClassEx(&wcl))
	  return 0;

  hwnd = CreateWindow(
	  "MyWin",
	  "SMM Title",
	  WS_OVERLAPPEDWINDOW,
	  CW_USEDEFAULT,
	  CW_USEDEFAULT,
	  CW_USEDEFAULT,	/* width */
	  CW_USEDEFAULT,	/* height */
	  HWND_DESKTOP,		/* no parent */
	  NULL,
	  hThisInst,
	  NULL);

  ShowWindow(hwnd, nWinMode);
  UpdateWindow(hwnd);

  while(GetMessage(&msg, NULL, 0, 0)) {
	  TranslateMessage(&msg);
	  DispatchMessage(&msg);
  }

  return msg.wParam;
}

LRESULT CALLBACK WindowFunc(HWND hwnd, UINT msg,
							WPARAM wParam, LPARAM lParam)
{
  int response;
  char fileNameString[1024] = "";
  OPENFILENAME fname;

  switch(msg) {
  case WM_CREATE:
	  initializeDisplaySystem(hwnd);
	  break;
  case WM_QUERYNEWPALETTE:
	  //displayQueryNewPalette(hwnd);
	  break;
  case WM_PALETTECHANGED:
	  //displayPaletteChanged(hwnd);
	  break;
  case WM_PAINT:
	  displayRepaint(hwnd);
	  break;
  case WM_COMMAND:
	  switch (LOWORD(wParam)) {
	  case IDM_OPEN:
		  memset(&fname, 0, sizeof(fname));
		  fname.lStructSize = sizeof(fname);
		  fname.hwndOwner = hwnd;
		  fname.lpstrFile = fileNameString;
		  fname.nMaxFile = sizeof(fileNameString);
		  if (GetOpenFileName(&fname))
			  displayDICOMImage(hwnd, fileNameString);
		  break;
	  case IDM_EXIT:
		  PostQuitMessage(0);
		  break;
	  default:
		  return DefWindowProc(hwnd, msg, wParam, lParam);
	  }
	  break;
  case WM_DESTROY:
	  PostQuitMessage(0);
	  break;
  default:
	  return DefWindowProc(hwnd, msg, wParam, lParam);
  }

  return 0;
}
