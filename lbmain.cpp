//====================================================================
//  File: LbMain.cpp
//
//  Purpose: Main Function for Windows UI for LB Demo Sample
//
//  Functions:
//     WinMain      - Application entry point
//
//====================================================================
/*
	 Credits:
	 -------
	 Written by: Asmus Freytag
	 

	 Disclaimer and legal rights:
	 ---------------------------
	 Copyright (C) 1999-2007, ASMUS, Inc. All Rights Reserved. 
	 Distributed under the Terms of Use in http://www.unicode.org/copyright.html.

	 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
	 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
	 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF THIRD PARTY RIGHTS. 
	 IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS INCLUDED IN THIS NOTICE 
	 BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT OR CONSEQUENTIAL DAMAGES, 
	 OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, 
	 WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, 
	 ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE SOFTWARE.

	 The files linebrk.rc, and resource.h are distributed together with this file and are included 
	 in the above.

	 --------------------------------------------------------------------------

	 Last updated: 04/10/07 - asmusf - finalized
*/

// If no UI mode defined, set it here - default is commandline version
// which ignores this file
#ifndef WINDOWS_UI
#define WINDOWS_UI 0
#endif

// This file only needed for this build option
#if WINDOWS_UI == 1
#pragma message("Standalone Windows Demo")

// Sample dialogs work better if compiled for Unicode
#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef UNICODE
#define UNICODE
#endif



//====================  Header Files  ======================

#include "..\linebrk.h"

//====================================================================
//  Function: int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
//
//  Purpose: Application Entry Point
//
//  Comments: Registers window class, creates the dialog, and excecutes message loop.
//====================================================================

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pszCmdLine, int cmdShow )
{
	LPTSTR szAppName = TEXT("LineBreakDemo");
	WNDCLASS wc;
	MSG msg;

	wc.cbClsExtra = 0;
	wc.cbWndExtra = DLGWINDOWEXTRA;
	wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW );
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.lpszClassName = szAppName;
	wc.lpszMenuName = TEXT("NULL");
	wc.lpfnWndProc = (WNDPROC) LineBrkWndProc;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	// The IDD_LINEBRK resource for standalone use must have CLASS LineBreakDemo
	// in the linebrk.rc file and should not have an IDCANCEL button

	RegisterClass(&wc);

	HWND hwnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_LINEBRK), 0, NULL);
	ShowWindow(hwnd, cmdShow);

	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!IsDialogMessage(hwnd, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

#endif
