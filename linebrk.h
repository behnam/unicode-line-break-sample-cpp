// File: LineBrk.h
//
// For use with LineBreak Sample Implementation
// For rights, disclaimers and description see associated LINEBREAK.CPP file
//
// Copyright (C) 1999-2005, ASMUS, Inc.     All Rights Reserved

#ifndef _LINEBRRK_H_
#define _LINEBRK_H_

// --- CONDITIONAL COMPILATION FOR DEMO --------------

#if WINDOWS_UI
	// disable some warnings generated by windows.h
	#pragma warning (disable : 4514 4201)
#endif

#if WINDOWS_UI > 1
#include "main.h"	    // private header, includes Windows,h
#include "window.h"     // private header, center
#include "cmapfile.h"	// private header, table_verify
#define DEMO 1			// force demo mode
#ifdef DEBUG_ENABLED
	#define DEBUGGING 1	// conditionally enable debug support 
#endif					// for private build
#elif WINDOWS_UI > 0
#define ASSERT(exp) (void)( (exp) || (MessageBoxA(NULL, "Warning:  Assert Failed: " #exp, __FILE__, MB_OK), 0) ) 
#define ASSERTSZ(exp, sz) (void)( (exp) || (MessageBoxA(NULL, "Warning: " #sz "\n" #exp, __FILE__, MB_OK), 0) ) 

#include <windows.h>    // standard include file for windows
#include <windowsx.h>   // extended include file for windows
#include "resource.h"	// resources for standalone LineBreak dialog
#include "stdio.h"		// for table_verify
#else
// commandline version, define printf based ASSERT
#include <stdio.h>		// ASSERT and table_verify
#define ASSERT(x)		if (!(x)) \
						fprintf(stdout, "assert failed: %s\n", #x); else ;
#define ASSERTSZ(x,y)	if (!(x)) \
						fprintf(stdout, "assert failed: %s\n%s\n", #x, #y); else ;
#endif

// duplicate some windows.h defines for commandline
#ifndef TEXT
#define TCHAR  char
#define LPTSTR char *
#endif

//--- DECLARATIONS FOR DEMO UI ------
#if WINDOWS_UI
#if WINDOWS_UI > 1
BOOL CALLBACK LineBrkDlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
#else
LRESULT CALLBACK LineBrkWndProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
#endif
#endif

#endif // ifndef _LINEBRK_H_
