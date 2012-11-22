// LineBrk.cpp

// Line break sample implementation using pair tables

// Set WINDOWS_UI to 0 to get commandline UI	// compiles this file only
// Set WINDOWS_UI to 1 for windows UI			// requires all files in sample
// Set WINDOWS_UI to 2 for private build		

#ifndef WINDOWS_UI
#define WINDOWS_UI 0
#endif

// Sample dialogs work better if compiled for Unicode
#if WINDOWS_UI
#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef UNICODE
#define UNICODE
#endif
#endif

// Matches Version 5.1.0
// to getProposed Update: Version 5.0.1
// #define v501
// or, to get version 5.0.0 behavior
// #define v500


// Linebreak include file
#ifndef _LINEBRK_H_
#include "linebrk.h"
#endif

// Set to 0 for default
// Set to 1 to run in debug mode
// Enable this line for debugging, or set via makefile
// #define DEBUGGING 1				

#ifndef DEBUGGING
#ifdef _DEGUG
#define DEBUGGING 1
#endif
#endif
// Debug mode enables Table checks


#if DEBUGGING
// for Table verification, enable this line
// #define VERIFY_PAIR_TABLE  
#ifdef VERIFY_PAIR_TABLE

// change as needed to for table verification
#ifdef v500
#define VERIFICATION_FILE L"PairTableFull5.0.0.html"
#else
#ifdef v501  // this may be interim
#define VERIFICATION_FILE L"PairTableFull5.0.1.html"
#else
#define VERIFICATION_FILE L"PairTableFull5.1.0.html"
#endif
#endif

#pragma message("Table assertions enabled")
#endif
#endif

/*---------------------------------------------------------------------------
	 File: LineBrk.Cpp

	 This is sample code for the line breaking algorithm of
	 Unicode Standard Annex #14, Line Breaking Properties, Version 5.1.0
	 (and version 5.0.0 when using #define v500)

	 Conformance
	 -----------

	 This sample uses a pseudo-alphabet for ease of testing. To make the 
	 code work for regular Unicode, replace the function classifyLnBrk() with 
	 one that looks up the line break classes for Unicode characters from 
	 the file LineBreak.txt in the Unicode Character Database.

	 While every effort has been made to conform to the specifications
	 in UAX#14, no formal testing or verification has been carried out,
	 other than ensuring that the values in the pair table match those
	 in the HTML text of UAX#14.

	 Build Notes
	 -----------

	 To compile the sample implementation please set the #define 
	 directives above so the correct headers get included. 
	 
	 The Win32 version is provided as a dialog procedure. To create 
	 a full executable using VC++ set up a Win32 project and add all
	 the files to it. Add #define WINDOWS_UI=1 at the top of each file
	 or set /DWINDOWS_UI=1 on the compiler commandline. The
	 project definition file linebrk.vcproj can be used with MS Visual C++
	 and is preconfigured for compiling the Windows UI (debug build)
	 and the standalone version (release build).

	 To compile a standalone commandline version, use just the two
	 files linebrk.cpp and linebrk.h.

	 This code uses an extension to C++ that gives variables declared in
	 a for() statement function the same scope as the for() statement.
	 If your compiler does not support this extension, you may need to
	 move the declaration, e.g. int ich = 0; in front of the for statement.

	 Notation
	 --------
	 Pointer variables generally start with the letter p
	 Counter variables generally start with the letter c
	 Index variables generally start with the letter i
	 Boolean variables generally start with the letter f

	 The enumerated line break classes have the same name as in the
	 description for the Unicode Line Breaking Property

	 Update History:
	 --------------
	 Last Revised 08-03-04

	 Finalized, 5.1.0 version

	 This retracts rule 30 from Unicode 5.0.0, and adds rule 12a.
	 The code also fixes a UI bug that affected the use of the 
	 pseudo alphabet for NL, as well as display of its LB class

	 Last Revised 07-04-10

	 Finalized, 5.0.1 version
	 This is an interim version, never standardized, but matches a
	 proposed update for 5.1.0

	 Last Revised 07-02-14

	 Fixed a post 5.0.0 erratum where a leading space would assert
	 Support for modeless dialog if WINDOWS_UI==2
	 Support for dialog-only standarlon if WINDOWS_UI==1

	 Last Revised 06-06-19

	 Additional comments, minor bug in UI code

	 Last Revised 06-05-30

	 More explicit handling of NL in UI and sample driver.

	 Last Revised 06-04-18

	 Fixed a regression in the UI driver code that affected strings consisting
	 of two characters of class SA. Explicitly map NL to BK as they have the
	 same effect. Minor updates to some comments.

	 Last Revised 06-01-20

	 Updated the pair table to Unicode Version 5.0.0, and carried
	 out the rule-renumbering in comments and HTML generating
	 code.

	 Last Revised 05-03-30

	 Updated the pair table, improved handling of CM.
	 Sample can now produce HTML pair table for verification.
	 Changed to match Unicode Version 4.1
	 
	 Last Revised 04-06-03

	 Updated the pair table, improved handling of CM.
	 Removed commented out code. Added new classes NL and WJ.

	 Last Revised 23-08-02

	 Expanded sample to handle all classes, including BK, CR, LF and SG
	 Fixed the case of space at beginning of the line. Revised the
	 break pair table to match revised rules in Version 4.0.0 of UAX#14.
	 
	 Last Revised 03-08-01

	 Fixed regression in findLineBreak that made all characters
	 behave like combining marks when CMInTable was deselected.

	 Last Revised 04-25-01

	 Credits:
	 -------
	 Written by: Asmus Freytag
	 

	 Disclaimer and legal rights:
	 ---------------------------
	 Copyright (C) 1999-2008, ASMUS, Inc. All Rights Reserved. 
	 Distributed under the Terms of Use in http://www.unicode.org/copyright.html.

	 THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
	 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
	 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF THIRD PARTY RIGHTS. 
	 IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS INCLUDED IN THIS NOTICE 
	 BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT OR CONSEQUENTIAL DAMAGES, 
	 OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, 
	 WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, 
	 ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE SOFTWARE.

	 The files linebrk.vcproj, linebrk.rc, and resource.h are distributed together  with this file 
	 and are included in the above.
----------------------------------------------------------------------------------*/

// === LOCAL FUNCTION DECLARTIONS ===========================================
int classifyLnBrk(const LPTSTR pszText, enum break_class * pcls,  int cch);
int findLineBrk(enum break_class *pcls, enum break_action *pbrk, int cch, bool fTailorCMSP = false);
int findComplexBreak(enum break_class cls, enum break_class *pcls, enum break_action *pbrk, int cch);

void verifyTable();
enum break_class LBClassFromCh(TCHAR ch);

// === HELPER FUNCTIONS ==========================================================

// a stub to bypass assertions
bool assert(bool x)
{
	if (x)
		return true;
	else
		return false;
}

// === DEMO DISPLAY FUNCTIONS AND DECLARATIONS ====================================

// The demo code uses a pseudo classification which maps the ASCII character set to various line break
// classes. For a real implementation, use the values in LineBreak.txt in the  Unicode Character Database.
// The sample mapping is found further below together wiht the classification function.
//
// This section of the file contains additional mappings from various values that are used to make special 
// characters visible, sets of arrays that allow a mapping to the short name, and a help string for the demo.

// mapping of special characters to control codes for the pseudo alphabet
const int chFIRST = 1;
const int chZWSP = 1;
const int chZWNBSP = 2;
const int chNBHY = 3;
const int chSHY = 4;
const int chNBSP = 5;
const int chDummy1 = 6;	
const int chEM = 7;	// Em dash
const int chELLIPSIS = 8; // Ellipsis
const int chTB =  9;
const int chLFx = 10;
const int chOBJ = 11;
const int chDummy2 = 12;
const int chCRx = 13;
const int chNLx = 14;
const int chLAST = 14;

// characters in the above list are mapped *both* ways
// don't use regular ASCII characters

// mapping of special character codes to Unicode symbols for visualization
int chVisibleFromSpecial[] =
{
/* ZW  1 chZWSP */		0x2020,	// show as dagger
/* GL  2 chZWNBSP */	0x2021,	// show as double dagger
/* GL  3 chNBHY */		0x00AC,	// show as not sign
/* BA  4 chSHY */		0x00B7,	// show as dot
/* GL  5 chNBSP */		0x2017,	// show as low line
/* -- 6 chDummy1 */		0x203E,	// show as double low line
/* B2 7 chEM */			0x2014,	// show as em dash
/* IN 8 chELLIPSIS */	0x2026,	// show as ellipsis
/* CM 9 chTB */			0x2310,	// show as not sign
/* LF 10 chLFx */		0x2580,	// show as high square
/* CB 11 chOBJ */		0x2302,	// show as house (delete)
/* -- 12 chdummy2 */	0x2222,
/* CR 13 chCRx */		0x2584,	// show as low square
/* NL 14 chNLx */		0x258C,	// show as left half block
};

// map character codes to visible symbol
int VisibleFromChar(int ch)
{
	if (ch >= chFIRST && ch <= chLAST)
	{
		// special char are one based enumeration
		return chVisibleFromSpecial[ch-1];
	}
	else
	{
		return ch;		
	}
}

// map visible symbol to character
int CharFromVisible(int ch)
{
	for (int ich = 0; ich < sizeof chVisibleFromSpecial / sizeof (int); ich ++)
	{
		if (ch == chVisibleFromSpecial[ich])
		{
			return ich + 1;
		}
	}
	return ch;
}

// This help string for the Windows UI, shows which sample characters
// from the pseudo alphabet get mapped to which line break class.
#if WINDOWS_UI
TCHAR * explain =
TEXT("This sample uses the following pseudo-alphabet as input\r\n")
TEXT("Alphabetic:  a-f   Ideograph:   A-F   Numeric:    0-9 \r\n")
TEXT("Combining:    `    Hangul 2:     h    Hangul 3:    H  \r\n")
TEXT("Jamo Lead:    L    Jamo Vowel:   V    Jamo Trail:  T  \r\n")
TEXT("Prefix:       $    Postfix:      %    Separator:   ,  \r\n")
TEXT("Exclamation:  !?   Non-Starter:  :    Syntax:      /  \r\n")
TEXT("Break after:  *    Break Before: &&    Hyphen:      -  \r\n")
TEXT("Quote:        \"    Glue:         G    Word Joiner: W  \r\n")
TEXT("Open         {[(   Close:       )]}   Leaders:     _  \r\n") 
TEXT("ZW-Space:     Z    Complex:      Y    Object:      @  \r\n") 
TEXT("Space:       ' '   Break opportunities are shown as | or \xA6");
#endif

// representative reverse mapping, i.e. mapping of line break class
// to a single specimen character from the pseudo alphabet.
TCHAR CharFromLnbkTypes[] =
{
	// OP,	CL,  QU,  GL,  NS,  EX,  SY,  IS,  PR,  PO,  NU,  AL,  ID,  IN,  HY,  BA,  BB,  B2,  ZW,  CM,  WJ,  SA,  SP,[ PS,  BK,  CR,  LF,  NL, CB, SG] = class 
	 0x28,0x29,0x27,0x3D,0x3a,0x21,0x2f,0x2c,0x24,0x25,0x30,0x61,0x4A,0x5f,0x2d,0x2a,0x26,0x07,0x01,0x6a,0x77, 0x7f,0x20,
    //  (    )    "   =    :    !    /     ,    $   %    0     a    I    _   -    *    &   bell ^A    i   DEL   ' '
};


// map line break class into single letter from the sequence 1-9,A...Y" 
// this is usefule for times when it is desired to show a string of 
// linebreak classes that has the same length as the input string in 
// characters, however, it's not very readable.
int CharFromLbcls[] =
{
// OP,	CL, QU, GL, NS, EX, SY, IS, PR, PO, NU, AL, ID, IN, HY, BA, BB, B2, ZW, CM, WJ, H2, H3, JL, JV, JT, SA, SP, PS, BK, CR, LF, NL, CB, 
   '1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y',
//....
};

// Line break classes are shown vertically in the demo dialog so that each class 
// fits underneath the current character in the input field. The first array gives the 
// character for the top row, the second the character for the bottom row.
// e.g. 'O' and 'P', when placed above one anohter and read down, read "OP".

int CharFromLbcls1[] =
{
// OP,	CL, QU, GL, NS, EX, SY, IS, PR, PO, NU, AL, ID, IN, HY, BA, BB, B2, ZW, CM, WJ, H2, H3, JL, JV, JT, SA, SP, PS, BK, CR, LF, NL, CB, 
   'O','C','Q','G','N','E','S','I','P','P','N','A','I','I','H','B','B','B','Z','C','W','H','H','J','J','J','S','S','P','B','C','L','N','C', //....
};
int CharFromLbcls2[] =
{
// OP, CL, QU, GL, NS, EX, SY, IS, PR, PO, NU, AL, ID, IN, HY, BA, BB, B2, ZW, CM, WJ, H2, H3, JL, JV, JT, SA, SP, PS, BK, CR, LF, NL, CB, 
   'P','L','U','L','S','X','Y','S','R','O','U','L','D','N','Y','A','B','2','W','M','J','2','3','L','V','T','A','P','S','K','R','F','L','B', //....
};

// Break actions are the types of break opportunities that may occur at a particular
// point in the input. Values for these are also needed in the UI portion of the code
// so they are already defined here - for explanation see below in the line break
// section.
enum break_action
{
	DIRECT_BRK,
	INDIRECT_BRK, 		
	COMBINING_INDIRECT_BRK, 	
	COMBINING_PROHIBITED_BRK, 	
	PROHIBITED_BRK,
	EXPLICIT_BRK,
	HANGUL_SPACE_BRK,
};

//=== DEMO DIALOG AND HELPER FUNCTIONS==============================================

#define MAX_CCH 256
int GetInputText(TCHAR * pszInput, int cch)
{
	static int ich[MAX_CCH]; 

	int max_ich = sizeof CharFromLnbkTypes / sizeof (TCHAR);

	for (int i = 0; i < cch; i++)
	{
		if (++ich[i] >= max_ich)
		{
			ich[i] = 0;
			continue;
		}
		break;
	}

	for (i = 0; i < cch; i++)
	{
		pszInput[i] = CharFromLnbkTypes[ich[i]];
	}
	pszInput[i] = 0;
	
	return i;
}

// === DISPLAY OPTIONS ======================================================

#if WINDOWS_UI > 0

//2 // === DISPLAY AND DIALOG FUNCTIONS ===================================================

void ShowLBClasses(HWND hwndDlg, int idc, enum break_class *lbcls, int cch)
{
	TCHAR pszTypes[MAX_CCH * 2];
	for (int ich = 0; ich < cch; ich++)
	{
		pszTypes[ich] = CharFromLbcls1[lbcls[ich]];//LBClassFromCh(pszInput[ich])];
	}
	pszTypes[ich++] = '\r';
	pszTypes[ich++] = '\n';
	for ( ; ich < cch * 2 + 2; ich++)
	{
		pszTypes[ich] = CharFromLbcls2[lbcls[ich - cch - 2]];//LBClassFromCh(pszInput[ich - cch - 2])];
	}
	pszTypes[ich] = 0;
	SetDlgItemText(hwndDlg, idc, pszTypes);
}							  

void ShowLineBreaks(HWND hwndDlg, int idc, LPTSTR pszInput, enum break_action *pbrk, int cch)
{
	TCHAR pszBrkText[2*MAX_CCH];
	for (int ichIn = 0, ichOut = 0; ichIn < cch; ichIn++)
	{
		// echo input character
		pszBrkText[ichOut++] = pszInput[ichIn];
		
		// echo break opportunity
		switch (pbrk[ichIn])
		{
		case EXPLICIT_BRK:					// '!' break required
#ifdef UNICODE
			pszBrkText[ichOut++] = 0x2551;	// double vertical line
#else
			pszBrkText[ichOut++] = '|';		// double vertical line
			pszBrkText[ichOut++] = '|';		// double vertical line
#endif
			break;
		case DIRECT_BRK:					// '_' break allowed
			pszBrkText[ichOut++] = '|';
			break;
		default:
		case INDIRECT_BRK:					// '%' only break across space (aka 'indirect break' below)
			pszBrkText[ichOut++] = (TCHAR) 0xa6;
			break;
		case COMBINING_INDIRECT_BRK:		// '#' indirect break for combining marks
			pszBrkText[ichOut++] = (TCHAR) 0xa6;
			break;
		case COMBINING_PROHIBITED_BRK:		// '@' indirect break for combining marks
			/* fall through */
		case PROHIBITED_BRK:				// '^' no break allowed
			break;
		case HANGUL_SPACE_BRK:				// break allowed, except when spaces are used with Hangul
			pszBrkText[ichOut++] = (TCHAR) 0xa6; // not yet used
		};
	}
	pszBrkText[ichOut] = 0;
	SetDlgItemText(hwndDlg, idc, pszBrkText);
}

/*---------------------------------------------------------------------------
	Function: DoLineBrkDlg
	
	Drives the line break function and displays the result

	Input: Handle to dialog

	Note: directly reads/writes to fields int the dialog, limit 256 chars
----------------------------------------------------------------------------*/

void DoLineBrkDlg(HWND hwndDlg)
{
	TCHAR pszInput[MAX_CCH];

	enum break_class lbcls[MAX_CCH];
	enum break_action lbrks[MAX_CCH];

	// read input string
	int cch = GetDlgItemText(hwndDlg, IDC_INPUT, pszInput, MAX_CCH); 

	// assign line breaking classes
	classifyLnBrk(pszInput, lbcls, cch);

	ShowLBClasses(hwndDlg, IDC_TYPES, lbcls /*pszInput*/, cch);

	// find the line breaks
	int ich = 0;
	enum break_action * lbrksTmp = lbrks;
	enum break_class * lbclsTmp = lbcls;
	int cchTmp = cch;

	if (cch)
	{
		do {
			ich += findLineBrk(lbclsTmp + ich, lbrksTmp + ich, cchTmp, 
				FALSE != IsDlgButtonChecked(hwndDlg, IDC_ALTERNATE));
			cchTmp = cch - ich;
		} while(cchTmp > 0);
	}

	// write display string
	ShowLineBreaks(hwndDlg, IDC_DISPL, pszInput, lbrks, cch);
}

// helper function for dialog
void InsertChAtSelection(HWND hwndDlg, TCHAR chFormat, int ichStart, int ichEnd)
{
	TCHAR pszInput[MAX_CCH];
	TCHAR pszNew[MAX_CCH];

	// read input string
	int cch = GetDlgItemText(hwndDlg, IDC_INPUT, pszInput, MAX_CCH); 

	// no selection
	if (ichEnd < ichStart || ichStart > cch)
		return;

	// insert ZWSP, ZWNBSP, NBHY, SHY, NBSP,  etc
	lstrcpyn(pszNew, pszInput, ichStart + 1);
	pszNew[ichStart] = VisibleFromChar(chFormat);
	lstrcpyn(pszNew + ichStart + 1, pszInput + ichStart, cch - ichStart + 1);

	// write formatted string
	SetDlgItemText(hwndDlg, IDC_INPUT, pszNew);

	// get ready to accept more typed input
	SetFocus(GetDlgItem(hwndDlg, IDC_INPUT));
	ichStart++;
	SendDlgItemMessage(hwndDlg, IDC_INPUT, EM_SETSEL, 
                        (LPARAM) ichStart, (WPARAM) ichStart);
	}


//-------------------------------------------------------------------------
// Function: LineBrkDlgProc
//
// Implements user interaction with dialog controls for IDD_LINEBREAK
//-------------------------------------------------------------------------

#if WINDOWS_UI > 1
// For private build, this is an ordinary modeless dialog
BOOL CALLBACK LineBrkDlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int ichStart =0;
	static int ichEnd = 0;
	static PDWORD pcwndLineBrk = 0;

    switch (message)
    {
		case WM_INITDIALOG:
	   		{
				#ifdef _WINDOW_H_
				// center window (requires private header)
				CWindow winDlg(hwndDlg);
				winDlg.CenterAbove(GetWindow(hwndDlg,GW_OWNER));
				#endif

				// verify the table
				verifyTable();

				pcwndLineBrk = (PDWORD) lParam;
				if (pcwndLineBrk)
					(*pcwndLineBrk)++;


				// initialize dialog 
				SetDlgItemText(hwndDlg, IDC_EXPLAIN, explain);
				return TRUE;
			}
			// ... continued after #endif
#else
// For standalone (WINDOWS_UI == 1) the dialog is run as a main window
// requiring some difference in initialization code and message handling
#pragma message("Standalone Windows Demo")

// helper function to initialize the explanation window
BOOL CALLBACK SetExplainProc(HWND hwndChild, LPARAM lParam)
{
	LONG id = GetWindowLong(hwndChild, GWL_ID);
	if (id == IDC_EXPLAIN)
	{
		SendMessage(hwndChild, (UINT) WM_SETTEXT,  (WPARAM) 0,  (LPARAM) lParam); 
		return FALSE; // done
	}
	return TRUE; // continue looking
}

LRESULT CALLBACK LineBrkWndProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int ichStart =0;
	static int ichEnd = 0;

    switch (message)
    {
		case WM_SHOWWINDOW:
			// verify the table
			verifyTable();
			// initialize explanation window
			EnumChildWindows(hwndDlg, SetExplainProc,  (LPARAM) explain);
			return 0;
			break;
	   case WM_DESTROY:
		   PostQuitMessage(0);
		   return 0;
#endif
	   // Handling buttons and edit fields
       case WM_COMMAND:
            switch (GET_WM_COMMAND_ID(wParam, lParam)) //Command ID
            {
		// change to inpt text: run the algorithm
		case IDC_INPUT: 
			SendDlgItemMessage(hwndDlg, IDC_INPUT, EM_GETSEL, (LPARAM) &ichStart, (WPARAM) &ichEnd);
			DoLineBrkDlg(hwndDlg);
			break;

		case IDC_CMINTABLE:
			EnableWindow(GetDlgItem(hwndDlg, IDC_ALTERNATE), !IsDlgButtonChecked(hwndDlg, IDC_CMINTABLE));
		// fall through
		case IDC_ALTERNATE:
		case IDC_HANGULCLUSTER:
			DoLineBrkDlg(hwndDlg);
			break;				   

		// buttons to enter special character codes
		case IDC_TAB:
			InsertChAtSelection(hwndDlg, chTB, ichStart, ichEnd);
			break;
		case IDC_CR:
		   InsertChAtSelection(hwndDlg, chCRx, ichStart, ichEnd);
		   break;
		case IDC_NL:
		   InsertChAtSelection(hwndDlg, chNLx, ichStart, ichEnd);
		   break;
		case IDC_LF:
		   InsertChAtSelection(hwndDlg, chLFx, ichStart, ichEnd);
		   break;
		case IDC_ZWSP:
		   InsertChAtSelection(hwndDlg, chZWSP, ichStart, ichEnd);
		   break;
		case IDC_ZWNBSP:
		   InsertChAtSelection(hwndDlg, chZWNBSP, ichStart, ichEnd);
		   break;
		case IDC_NBSP:
		   InsertChAtSelection(hwndDlg, chNBSP, ichStart, ichEnd);
		   break;
		case IDC_EM:
		   InsertChAtSelection(hwndDlg, chEM, ichStart, ichEnd);
		   break;
		case IDC_ELLIPSIS:
		   InsertChAtSelection(hwndDlg, chELLIPSIS, ichStart, ichEnd);
		   break;
		case IDC_OBJ:
		   InsertChAtSelection(hwndDlg, chOBJ, ichStart, ichEnd);
		   break;

		case IDC_SHY:
			InsertChAtSelection(hwndDlg, chSHY, ichStart, ichEnd);
			break;
		case IDC_NBHY:
			InsertChAtSelection(hwndDlg, chNBHY, ichStart, ichEnd);
			break;
		#if WINDOWS_UI == 2
		// buttons to close the dialog
		case IDOK:
		case IDCANCEL:
			// pass either IDOK or IDCANCEL to ENDDIALOG
			EndDialog(hwndDlg, GET_WM_COMMAND_ID(wParam, lParam));
			CWindow::SetModelessDlg(0);
			if (pcwndLineBrk)
				(*pcwndLineBrk)--;
			return TRUE;
		#endif
		}
		break;
	}
#if WINDOWS_UI == 1
	return DefWindowProc(hwndDlg, message, wParam, lParam);
#else
    return FALSE ;
#endif
}

#else
#pragma message("Compiling linebrk.cpp for command line version")

// ===== FUNCTIONS FOR COMMAND LINE VERSION ==============================

#include <stdlib.h>
#include <string.h>

// An alternate CharFromTypes array may be needed to use the command 
// line version,

#define MAX_CCH 256

void ShowLBClasses(FILE *f, LPTSTR pszInput, int cch)
{
	TCHAR pszTypes[MAX_CCH * 2];
	for (int ich = 0; ich < cch; ich++)
	{
		pszTypes[ich] = CharFromLbcls1[LBClassFromCh(pszInput[ich])];
	}
	pszTypes[ich++] = '\r';
	pszTypes[ich++] = '\n';
	for ( ; ich < cch * 2 + 2; ich++)
	{
		pszTypes[ich] = CharFromLbcls2[LBClassFromCh(pszInput[ich - cch - 2])];
	}
	pszTypes[ich] = 0;

    fprintf(f, pszTypes);
}							  

void ShowLineBreaks(FILE * f, LPTSTR pszInput, break_action *pbrk, int cch)
{
	TCHAR pszBrkText[2*MAX_CCH];
	for (int ichIn = 0, ichOut = 0; ichIn < cch; ichIn++)
	{
		if (pbrk[ichIn])
		{
			if (pbrk[ichIn] > 1)
			{
				pszBrkText[ichOut++] = pszInput[ichIn];
				pszBrkText[ichOut++] = (TCHAR) 0xa6;
			}
			else
			{
				pszBrkText[ichOut++] = pszInput[ichIn];
			}
		}
		else
		{
			pszBrkText[ichOut++] = pszInput[ichIn];
			pszBrkText[ichOut++] = '|';
		}
	}
	pszBrkText[ichOut] = 0;
    fprintf(f, pszBrkText);
}

void usage(char *s) 
{
    printf("Usage: %s [-verbose] [-clean] strings...\n", s);
    printf("\t-verbose = verbose debugging output.\n");
    printf("\t-clean = clean up the result.\n");
    printf("\tOptions affect all subsequent arguments.\n");
    printf("\tAll other arguments are interpreted as strings to process.\n");
}

int main(int argc, char** argv) 
{
    int realArg = 0;
    int doCMInTable = 1;
    int beVerbose = 0;

    FILE* f = stdout;

	verifyTable();

    if (argc == 1) 
	{
		usage(argv[0]); exit(0);
    }
    for (int i = 1; i < argc; ++i) 
	{
		if (strcmp(argv[i], "-verbose") == 0) 
		{
			beVerbose = 1;
			continue;
		} 
		else if (strcmp(argv[i], "-cm") == 0) 
		{
			doCMInTable = 0;
			continue;
		} 
		else 
		{
			++realArg;
		}
    
		TCHAR pszInput[MAX_CCH+1];
    
		int cch = strlen(argv[i]);
		if (cch > MAX_CCH) cch = MAX_CCH;
		strncpy(pszInput, argv[i], cch);
    
		pszInput[cch] = 0;
		fprintf(f, "Input    %2d: %s\n", realArg, pszInput);
    

	
		break_class lbcls[MAX_CCH];
		break_action lbrks[MAX_CCH];

		
		// assign line breaking classes
		classifyLnBrk(pszInput, lbcls, cch);

		if (beVerbose) 
		{
			fprintf(f, "LB Classes  : ");
			ShowLBClasses(f, pszInput, cch); fprintf(f, "\n");
		}

		// find the line breaks
		findLineBrk(lbcls, lbrks, cch, false != doCMInTable);

		// write display string
 		fprintf(f, "Output   %2d:", realArg);
		ShowLineBreaks(f, pszInput, lbrks,  cch); fprintf(f, "\n");

    }

    return 0;
}
#endif // WINDOWS_UI

//1 === FIND LINE BREAKS ===================================================

//2 === LINE BREAK SAMPLE CLASSIFICATION =====================================

#define odd(x) ((x) & 1)

#undef IN

// Line Break Character Types

// These correspond to the line break class values defined in UAX#14, Version 
// 5.0.0. In a real implementation, there would be a mapping from character
// code to line break class value. In this demo version, the mapping is from
// a pseudo alphabet to these line break classes. The actual line break algorithm
// takes as input only line break classes, so, by changing the mapping from
// pseudo alphabet to actual Unicode Characters, this demo could be adapted 
// for use in actual line breaking.

enum break_class
{
	// input types
	OP = 0,	// open
	CL,	// close
	QU,	// quotation
	GL,	// glue
	NS,	// no-start
	EX,	// exclamation/interrogation
	SY,	// Syntax (slash)
	IS,	// infix (numeric) separator
	PR,	// prefix
	PO,	// postfix
	NU,	// numeric
	AL,	// alphabetic
	ID,	// ideograph (atomic)
	IN,	// inseparable
	HY,	// hyphen
	BA,	// break after
	BB,	// break before
	B2,	// break both
	ZW,	// ZW space
	CM,	// combining mark
	WJ, // word joiner

	// used for Korean Syllable Block pair table
	H2, // Hamgul 2 Jamo Syllable
	H3, // Hangul 3 Jamo Syllable
	JL, // Jamo leading consonant
	JV, // Jamo vowel
	JT, // Jamo trailing consonant

	// these are not handled in the pair tables
	SA, // South (East) Asian
	SP,	// space
	PS,	// paragraph and line separators
	BK,	// hard break (newline)
	CR, // carriage return
	LF, // line feed
	NL, // next line
	CB, // contingent break opportunity
	SG, // surrogate
	AI, // ambiguous
	XX, // unknown
}; 


enum break_class LnBrkClassFromChar[]  =
{		
					// treat CB as BB for demo purposes

//  0	1	2	3	4	5	6	7	8	9	a	b	c	d	e	f
	AL, ZW, GL, GL, BA, GL,	AL, B2, IN, BA, LF, CB, AL, CR, NL, AL, // 00-0f
	AL, AL, AL, AL, AL, AL, AL, AL, AL, AL, AL, AL, AL, AL, AL, AL, // 10-1f

//  ' '  !   "       $   %   &   '   (   )   *   +   ,   -   .    /  
	SP, EX, QU, IN, PR, PO, BB, QU, OP, CL, BA, PR, IN, HY, IN, SY, // 20-2f
//   0   1   2   3   4   5   6   7   8   9   :   ;   <   =   >   ?
	NU, NU, NU, NU, NU, NU, NU, NU, NU,	NU,	NS,	AL,	AL,	GL, AL,	EX,	// 30-3f

//   @,  A  B   C   D   E   F   G   H   I   J   K   L   M   N   O  
	CB, ID,	ID, ID,	ID, ID,	ID, GL,	H3, ID,	ID, ID,	JL, ID,	ID, ID,	// 40-4f
	ID, ID, ID,	ID, JT,	ID, JV,	WJ, XX,	SA, ZW,	OP,	AL,	CL,	AL,	IS,	// 50-5f
	CM, AL,	AL, AL,	AL, AL,	AL, AL,	H2, AL,	AL, AL,	AL, AL,	AL, AL,	// 60-6f
	AL, AL, AL,	AL, AL,	AL, AL,	AL, AL,	AL, AL,	OP,	AL,	CL,	AL,	SA,	// 70-7f
//   p  q   r   s   t    u   v   w  x   y   z 
};

enum break_class LBClassFromCh(TCHAR ch)
{
	ch = CharFromVisible(ch);
	if (ch >= 0x7f)
		return XX;
	return LnBrkClassFromChar[ch];
}
/*---------------------------------------------------------------------------
	Function: classify
	
    Determines the character classes for all following
	passes of the algorithm 

	This uses a pseudo alphabet as input - see the szExplain string
	above for a description. In a production version, this function
	would implement the line break property lookup for actual Unicode
	characters.

	Input: Text string
		   Character count

	Output: Array of linebreak classes	

----------------------------------------------------------------------------*/
int classifyLnBrk(const LPTSTR pszText, enum break_class * pcls,  int cch)
{
	for (int ich = 0; ich < cch; ich++)
	{
		pcls[ich] = LBClassFromCh(pszText[ich]);

		// map unknown, and ambiguous to AL by default
		if (pcls[ich] == XX || pcls[ich] == AI)
			pcls[ich] = AL;

		// map contingent break to B2 by default
		// this saves a row/col for CB in the table
		// but only approximates rule 20
		if (pcls[ich] == CB)
			pcls[ich] = B2;

		/* If the following remapping is enabled, all tests involving 
		   NL can be removed from the main loop below.
		   
		// map NL to BK as there's no difference
		if (pcls[ich] == NL)
			pcls[ich] = BK;
		*/
	}
	return ich;
}

//2 // === LINE BREAK DEFINITIONS ===================================================

// Define some short-cuts for the table
#define oo DIRECT_BRK				// '_' break allowed
#define SS INDIRECT_BRK				// '%' only break across space (aka 'indirect break' below)
#define cc COMBINING_INDIRECT_BRK	// '#' indirect break for combining marks
#define CC COMBINING_PROHIBITED_BRK	// '@' indirect break for combining marks
#define XX PROHIBITED_BRK			// '^' no break allowed_BRK
#define xS HANGUL_SPACE_BRK			// break allowed, except when spaces are used with Hangul (not used)

// xS not yet assigned in the table below

//2 // === LINE BREAK PAIR TABLE ===================================================

// Line Break Pair Table corresponding to Table 2 of UAX#14, Version 5.0.0 
// plus Korean Syllable Block extensions - for details see that document

enum break_action brkPairs[][JT+1]=
{   //                ---     'after'  class  ------
	//		1	2	3	4	5	6	7	8	9  10  11  12  13  14  15  16  17  18  19  20  21   22  23  24  25  26  
	//     OP, CL, QU, GL, NS, EX, SY, IS, PR, PO, NU, AL, ID, IN, HY, BA, BB, B2, ZW, CM, WJ,  H2, H3, JL, JV, JT, = after class
	/*OP*/ XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, CC, XX,  XX, XX, XX, XX, XX, // OP open
#if defined(v500) || defined(v501)
// Version 5.0.0 and 5.0.1
	/*CL*/ oo, XX, SS, SS, XX, XX, XX, XX, SS, SS, SS, SS, oo, oo, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo, // CL close
#else
//  Version 5.1.0
	/*CL*/ oo, XX, SS, SS, XX, XX, XX, XX, SS, SS, oo, oo, oo, oo, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo, // CL close
#endif
	/*QU*/ XX, XX, SS, SS, SS, XX, XX, XX, SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, XX, cc, XX,  SS, SS, SS, SS, SS, // QU quotation
	/*GL*/ SS, XX, SS, SS, SS, XX, XX, XX, SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, XX, cc, XX,  SS, SS, SS, SS, SS, // GL glue
	/*NS*/ oo, XX, SS, SS, SS, XX, XX, XX, oo, oo, oo, oo, oo, oo, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo, // NS no-start
	/*EX*/ oo, XX, SS, SS, SS, XX, XX, XX, oo, oo, oo, oo, oo, oo, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo, // EX exclamation/interrogation
	/*SY*/ oo, XX, SS, SS, SS, XX, XX, XX, oo, oo, SS, oo, oo, oo, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo, // SY Syntax (slash)
	/*IS*/ oo, XX, SS, SS, SS, XX, XX, XX, oo, oo, SS, SS, oo, oo, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo, // IS infix (numeric) separator
	/*PR*/ SS, XX, SS, SS, SS, XX, XX, XX, oo, oo, SS, SS, SS, oo, SS, SS, oo, oo, XX, cc, XX,  SS, SS, SS, SS, SS, // PR prefix
	/*PO*/ SS, XX, SS, SS, SS, XX, XX, XX, oo, oo, SS, SS, oo, oo, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo, // NU numeric
#if defined(v500) || defined(v501)
// Version 5.0.0 and 5.0.1
	/*NU*/ SS, XX, SS, SS, SS, XX, XX, XX, SS, SS, SS, SS, oo, SS, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo, // AL alphabetic
	/*AL*/ SS, XX, SS, SS, SS, XX, XX, XX, oo, oo, SS, SS, oo, SS, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo, // AL alphabetic
#else
// Version 5.1.0
	/*NU*/ oo, XX, SS, SS, SS, XX, XX, XX, SS, SS, SS, SS, oo, SS, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo, // AL alphabetic
	/*AL*/ oo, XX, SS, SS, SS, XX, XX, XX, oo, oo, SS, SS, oo, SS, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo, // AL alphabetic
#endif
	/*ID*/ oo, XX, SS, SS, SS, XX, XX, XX, oo, SS, oo, oo, oo, SS, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo, // ID ideograph (atomic)
	/*IN*/ oo, XX, SS, SS, SS, XX, XX, XX, oo, oo, oo, oo, oo, SS, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo, // IN inseparable
#ifdef v500
// Version 5.0.0
	/*HY*/ oo, XX, SS, SS, SS, XX, XX, XX, oo, oo, SS, oo, oo, oo, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo, // HY hyphens and spaces
	/*BA*/ oo, XX, SS, SS, SS, XX, XX, XX, oo, oo, oo, oo, oo, oo, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo, // BA break after 
#else
// Version 5.0.1
	/*HY*/ oo, XX, SS, oo, SS, XX, XX, XX, oo, oo, SS, oo, oo, oo, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo, // HY hyphens and spaces
	/*BA*/ oo, XX, SS, oo, SS, XX, XX, XX, oo, oo, oo, oo, oo, oo, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo, // BA break after 
#endif
	/*BB*/ SS, XX, SS, SS, SS, XX, XX, XX, SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, XX, cc, XX,  SS, SS, SS, SS, SS, // BB break before 
	/*B2*/ oo, XX, SS, SS, SS, XX, XX, XX, oo, oo, oo, oo, oo, oo, SS, SS, oo, XX, XX, cc, XX,  oo, oo, oo, oo, oo, // B2 break either side, but not pair
	/*ZW*/ oo, oo, oo, oo, oo, oo, oo, oo, oo, oo, oo, oo, oo, oo, oo, oo, oo, oo, XX, oo, oo,  oo, oo, oo, oo, oo, // ZW zero width space
	/*CM*/ oo, XX, SS, SS, SS, XX, XX, XX, oo, oo, SS, SS, oo, SS, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo, // CM combining mark
	/*WJ*/ SS, XX, SS, SS, SS, XX, XX, XX, SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, XX, cc, XX,  SS, SS, SS, SS, SS, // WJ word joiner
																							    
	/*H2*/ oo, XX, SS, SS, SS, XX, XX, XX, oo, SS, oo, oo, oo, SS, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, SS, SS, // Hangul 2 Jamo syllable
	/*H3*/ oo, XX, SS, SS, SS, XX, XX, XX, oo, SS, oo, oo, oo, SS, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, SS, // Hangul 3 Jamo syllable
	/*JL*/ oo, XX, SS, SS, SS, XX, XX, XX, oo, SS, oo, oo, oo, SS, SS, SS, oo, oo, XX, cc, XX,  SS, SS, SS, SS, oo, // Jamo Leading Consonant
	/*JV*/ oo, XX, SS, SS, SS, XX, XX, XX, oo, SS, oo, oo, oo, SS, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, SS, SS, // Jamo Vowel
	/*JT*/ oo, XX, SS, SS, SS, XX, XX, XX, oo, SS, oo, oo, oo, SS, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, SS, // Jamo Trailing Consonant
	
};

//2 // === FIND LINE BREAKS =======================================================

// placeholder function for complex break analysis
// cls - last resolved line break class (this is !SA)
// pcls - pointer to array of line breaking classes with pcls[0] == SA (input)
// pbrk - pointer to array of line breaking opportunities (output)
//
int findComplexBreak(enum break_class cls, enum break_class *pcls, enum break_action *pbrk, int cch)
{
    if (!cch)
        return 0;

    for (int ich = 1; ich < cch; ich++) {

        // .. do complex break analysis here
        // and report any break opportunities in pbrk ..

        pbrk[ich-1] = PROHIBITED_BRK; // by default: no break

        if (pcls[ich] != SA)
                break;
    }
    return ich;
}

/* Line break actions
  * these are already declared above as they are needed for some of the UI functions
  * repeated here for ease of reference (the symbols used in the table in UAX#14 as 
  * well as the constants used in the brkPairs array are shown as well)
  
enum break_action {
       DIRECT_BRK = 0,             	// _ in table, 	oo in array
       INDIRECT_BRK,               	// % in table, 	SS in array
       COMBINING_INDIRECT_BRK,		// # in table, 	cc in array
       COMBINING_PROHIBITED_BRK,  	// @ in table 	CC in array
       PROHIBITED_BRK,             	// ^ in table, 	XX in array
       EXPLICIT_BRK };				// ! in rules
*/

// handle spaces separately, all others by table
// pcls - pointer to array of line breaking classes (input)
// pbrk - pointer to array of line break opportunities (output)
// cch - number of elements in the arrays (“count of characters”) (input)
// ich - current index into the arrays (variable) (returned value)
// cls - current resolved line break class for 'before' character (variable)
int findLineBrk(enum break_class *pcls, enum break_action *pbrk, int cch, bool fLEGACY_CM)
{
    if (cch <= 0) 
        return 0;

    enum break_class cls = pcls[0];

    // handle case where input starts with an LF
    if (cls == LF)
        cls = BK;

    // treat NL like BK
    if (cls == NL)
         cls = BK;

    // treat SP at start of input as if it followed WJ
    if (cls == SP)
         cls = WJ;

    // loop over all pairs in the string up to a hard break or CRLF pair
    for (int ich = 1; (ich < cch) && (cls != BK) && (cls != CR || pcls[ich] == LF); ich++) {

        // handle spaces explicitly
        if (pcls[ich] == SP) {
            pbrk[ich-1] = PROHIBITED_BRK;   // apply rule LB 7: × SP
            continue;                       // do not update cls
        }

        if (pcls[ich] == BK || pcls[ich] == NL || pcls[ich] == LF) {
            pbrk[ich-1] = PROHIBITED_BRK;
            cls = BK;
            continue;
        }

        if (pcls[ich] == CR)
        {
            pbrk[ich-1] = PROHIBITED_BRK;
            cls = CR;
            continue;
        }

        // handle complex scripts in a separate function
        if (cls == SA || pcls[ich] == SA) {
            ich += findComplexBreak(cls, &pcls[ich-1], &pbrk[ich-1], cch - (ich-1));
            if (ich < cch)
                cls = pcls[ich];
            continue;
        }

        ASSERT(cls < SP);
        ASSERT(pcls[ich] < SP);

        // lookup pair table information in brkPairs[before, after];
        enum break_action brk = brkPairs[cls][pcls[ich]];

        pbrk[ich-1] = brk;                              // save break action in output array

        if (brk == INDIRECT_BRK) {                      // resolve indirect break
            if (pcls[ich - 1] == SP)                    // if context is A SP * B
                pbrk[ich-1] = INDIRECT_BRK;             //       break opportunity
            else                                        // else
                pbrk[ich-1] = PROHIBITED_BRK;           //       no break opportunity
        } else if (brk == COMBINING_PROHIBITED_BRK) {   // this is the case OP SP* CM
            pbrk[ich-1] = COMBINING_PROHIBITED_BRK;     // no break allowed
            if (pcls[ich-1] != SP)
                continue;                               // apply rule 9: X CM* -> X
        } else if (brk == COMBINING_INDIRECT_BRK) {     // resolve combining mark break
            pbrk[ich-1] = PROHIBITED_BRK;               // don't break before CM
            if (pcls[ich-1] == SP){
                if (!fLEGACY_CM)                        // new: SP is not a base
                    pbrk[ich-1] = COMBINING_INDIRECT_BRK;    // apply rule SP ÷ 
                else                                    
                {
                    pbrk[ich-1] = PROHIBITED_BRK;		// legacy: keep SP CM together
                    if (ich > 1)
                        pbrk[ich-2] = ((pcls[ich - 2] == SP) ? INDIRECT_BRK : DIRECT_BRK);
                }
            } else                                      // apply rule 9: X CM * -> X
                continue;                               // don't update cls
        }
        cls = pcls[ich];                                // save cls of current character
    }
    // always break at the end
    pbrk[ich-1] = EXPLICIT_BRK;

    return ich;
}

//1 // === VERIFY PAIR TABLE =======================================================
#ifdef VERIFY_PAIR_TABLE 

// === MAP LB CLASSE TO ALIASES ===========================================================

// --- names -----
#define propID 0
#define propShort 1
#define propLong 2

struct LBAlias{
	int id;
	char * pszShort;
	char * pszLong;
	};

struct LBAlias LBAliases[] = {
//    AI , "AI", "Ambiguous",
    AL , "AL", "Alphabetic",
    B2, "B2", "BreakBoth",
    BA, "BA", "BreakAfter",
    BB, "BB", "BreakBefore",
    BK, "BK", "MandatoryBreak",
    CB, "CB", "ContingentBreak",
    CL, "CL", "ClosePunctuation",
    CM, "CM", "CombiningMark",
    CR, "CR", "CarriageReturn",
    EX, "EX", "Exclamation",
    GL, "GL", "Glue",
    H2, "H2", "H2",
    H3, "H3", "H3",
    HY, "HY", "Hyphen",
    ID, "ID", "Ideographic",
    IN, "IN", "Inseparable",
    IS, "IS", "InfixNumeric",
    JL, "JL", "JL",
    JT, "JT", "JT",
    JV, "JV", "JV",
    LF, "LF", "LineFeed",
    NL, "NL", "NextLine",
    NS, "NS", "Nonstarter",
    NU, "NU", "Numeric",
    OP, "OP", "OpenPunctuation",
    PO, "PO", "PostfixNumeric",
    PR, "PR", "PrefixNumeric",
    QU, "QU", "Quotation",
    SA, "SA", "ComplexContext",
    SG, "SG", "Surrogate",
    SP, "SP", "Space",
    SY, "SY", "BreakSymbols",
    WJ, "WJ", "WordJoiner",
    XX, "XX", "Unknown",
    ZW, "ZW", "ZWSpace",
};

char * pszSampleCharsFromLBClass[] = {
    /*OP*/   "U+0028 LEFT PARENTHESIS",
    /*CL*/   "U+0029 RIGHT PARENTHESIS",
    /*QU*/   "U+0022 QUOTATION MARK",
    /*GL*/   "U+00A0 NO-BREAK SPACE",
    /*NS*/   "U+30A1 KATAKANA LETTER SMALL A",
    /*EX*/   "U+0021 EXCLAMATION MARK",
    /*SY*/   "U+002F SOLIDUS",
    /*IS*/   "U+002C COMMA",
    /*PR*/   "U+0024 DOLLAR SIGN",
    /*PO*/   "U+0025 PERCENT SIGN",
    /*NU*/   "U+0030 DIGIT ZERO",
    /*AL*/   "U+0023 NUMBER SIGN",
    /*ID*/   "U+2E80 CJK RADICAL REPEAT",
    /*IN*/   "U+2024 ONE DOT LEADER",
    /*HY*/   "U+002D HYPHEN-MINUS",
    /*BA*/   "U+2010 HYPHEN",
    /*BB*/   "U+00B4 ACUTE ACCENT",
    /*B2*/   "U+2014 EM DASH",
    /*ZW*/   "U+200B ZERO WIDTH SPACE",
    /*CM*/   "U+0302 COMBINING ACUTE ACCENT",
    /*WJ*/   "U+2060 WORD JOINER",
    /*H2*/   "U+AC00 HANGUL SYLLABLE GA",
    /*H3*/   "U+AC01 HANGUL SYLLABLE GAG",
    /*JL*/   "U+1100 HANGUL CHOSEONG KIYEOK",
    /*JV*/   "U+1161 HANGUL JUNGSEONG A",
    /*JT*/   "U+11A8 HANGUL JONGSEONG KIYEOK",
};	 

// the above list is limited to LB classes shown in Tables 2 and 3 of UAX#14

char * pszLBAliasFromID(int id, bool fLong)
{
	for (int i = 0; i < sizeof LBAliases/ sizeof(struct LBAlias); i++)
	{
		if (LBAliases[i].id == id)
			if (fLong)
				return LBAliases[i].pszLong;
			else 
				return LBAliases[i].pszShort;
	}
	return ("");
}

char * pszShortFromLbclass(int id)
{
	return pszLBAliasFromID(id, false);
}
char * pszLongFromLbclass(int id)
{
	return pszLBAliasFromID(id, true);
};

// --- HELPER CLASS for PUBLIC BUILDS
#ifndef _CMAPFILE_H_
class CTextFile
{
	public:
		CTextFile(wchar_t *pszFilename, bool fIgnored)
		{
			_fp = _wfopen(pszFilename, L"w");
		}
		void PutString(char * psz)
		{
			fputs(psz, _fp);
		}
		void PutLine(char * psz)
		{
			fputs(psz, _fp);
			fputs("\n", _fp);
		}
		FILE * _fp;
};
#endif

//2 //=== TABLE VERIFICATION AND HTML GENERATION ===

// This section contains the code used to verify that the line break table in Section 7 of
// UAX# 14 matches the rules specified in Section 6. The verifyAndPrintTable method
// walks through all combinations of line break classes and handles each combination
// by a series of cascading rules that match those of UAX#14 as closely as possible.
//
// Whenever a rule handles a combination, the corresponding entry in the pair table
// array above is compared to the break action defined by the rule. An HTML fragment
// is emitted showing the rule and the resulting pair table entry. Together these 
// HTML fragments are used to provide the fully annotated version of the pair table 
// published in UAX#14. In case of discrepancies, the verification code asserts.
class table_verify
{
public: 
	table_verify()
		: out(CTextFile(VERIFICATION_FILE, true))
	{

	}
	void verifyAndPrintTable();
private:
	void no_break_pairs_with_space(enum break_class cb, enum break_class ca, char * pszRule = 0);
	void table_verify::no_break_pairs_with_space_for_combining(enum break_class cb, enum break_class ca,  char * pszRule);
	void no_break_without_space_for_combining(enum break_class cb, enum break_class ca, char * pszRule = 0);
	void no_break_pair(enum break_class cb, enum break_class ca, char * pszRule = 0);
	void break_pair(enum break_class cb, enum break_class ca, char * pszRule = 0);
	void init_table();
	void terminate_table();
	void init_row(char * pszHeader = 0, char * pszTitle = 0);
	void terminate_row();
	void init_col(char * pszTitle);
	void terminate_col();
	void dotitle(char * pszTitle);
	CTextFile out;
};

// worker functions to check particular values in the pair table array and to
// emit particular HTML fragments for the annotated HTML pair table
void table_verify::no_break_pairs_with_space(enum break_class cb, enum break_class ca, char * pszRule)
{
	ASSERT(brkPairs[cb][ca] == XX);
	if (pszRule)
	{
		init_col(pszRule); out.PutString("^"); terminate_col();
	}
}
void table_verify::no_break_pairs_with_space_for_combining(enum break_class cb, enum break_class ca,  char * pszRule)
{
	ASSERT(brkPairs[cb][ca] == CC);
	if (pszRule)
	{
		init_col(pszRule); out.PutString("@"); terminate_col();
	}
}
void table_verify::no_break_without_space_for_combining(enum break_class cb, enum break_class ca,  char * pszRule)
{
	ASSERT(brkPairs[cb][ca] == cc);
	if (pszRule)
	{
		init_col(pszRule); out.PutString("#"); terminate_col();
	}
}
void table_verify::no_break_pair(enum break_class cb, enum break_class ca,  char * pszRule)
{
	ASSERT(brkPairs[cb][ca] == SS);
	if (pszRule)
	{
		init_col(pszRule); out.PutString("%"); terminate_col();
	}
}
void table_verify::break_pair(enum break_class cb, enum break_class ca,  char * pszRule)
{
	ASSERT(brkPairs[cb][ca] == oo);
	if (pszRule)
	{
		init_col(pszRule); out.PutString("_"); terminate_col();
	}
}

void table_verify::init_table()
{
	out.PutLine("  <table class=\"pair\" cellSpacing=\"0\" width=\"88%\" border=\"1\">");
}

void table_verify::terminate_table()
{
	out.PutLine("  </table>");
}
void table_verify::init_row(char * pszHeader, char * pszTitle)
{
	out.PutLine("    <tr>");
	init_col(pszTitle);
	if (pszHeader)
		out.PutString(pszHeader);
	else
		out.PutString("&nbsp");
	terminate_col();
}
void table_verify::terminate_row()
{
	out.PutLine("    </tr>");
}

void table_verify::dotitle(char * pszTitle)
{
	out.PutString("title=\"");
	out.PutString(pszTitle);
	out.PutString("\"");
}

void table_verify::init_col(char * pszTitle)
{
	out.PutString("      <th ");
	if (pszTitle)
		dotitle(pszTitle);
	out.PutString(">");
}
void table_verify::terminate_col()
{
	out.PutLine("</th>");
}

#define nextclass(x) (x = (enum break_class)(x + 1))

void table_verify::verifyAndPrintTable()
{
	// Running this code will stop excecution with an assert whenever
	// an entry in the pair table does not match the statement of the
	// rules of the line break algorithm below.

	// At the same time, the code produces an HTML version of the LB
	// pair table in a format that matches that of UAX#14, except it
	// includes the Hangul and Jamo rows and and columns.

	// Rules that are not handled in the pair table, are not verified
	// for example 1, 2, 3, 4, 5, 6. Rules 9 and 10 are handled as
	// described below.

	// The pair table implements rule 18 directly (by having two 
	// contexts, one for adjacency and one for adjacency across space).
	// In the cascading rule formulation, all rules above 18 are for
	// direct breaks (adjacent characters) and all rules below 18 are
	// for indirect breaks (adjacent across space). 

	// For that reason, SP does not exist as a row or column in the 
	// pair table.

	char szTitle[100];
	char szHeader[100];
	init_table();

	// write the header row, containing the column headers
	// (class after)
	init_row();
	for (enum break_class ca = OP; ca <=JT; nextclass(ca) )
	{
		// format column header
		strcpy(szTitle, pszSampleCharsFromLBClass[ca]);
		strcat(szTitle, "; ");
		strcat(szTitle, pszShortFromLbclass(ca));
		strcat(szTitle, "=");
		strcat(szTitle, pszLongFromLbclass(ca));
		strcpy(szHeader, "<a class=\"charclass\" href=\"#");
		strcat(szHeader, pszShortFromLbclass(ca));
		strcat(szHeader, "\">");
		strcat(szHeader, pszShortFromLbclass(ca));
		strcat(szHeader, "</a>");
		init_col(szTitle);
		out.PutString(szHeader);
		terminate_col();
	}
	terminate_row();

	// write each of the data frow
	// each row starts with a row header (class before)
	for (enum break_class cb = OP; cb <= JT; nextclass(cb) )
	{
		// format row header
		strcpy(szTitle, pszSampleCharsFromLBClass[cb]);
		strcpy(szHeader, "<a class=\"charclass\" href=\"#");
		strcat(szHeader, pszShortFromLbclass(cb));
		strcat(szHeader, "\">");
		strcat(szHeader, pszShortFromLbclass(cb));
		strcat(szHeader, "</a>");
		init_row(szHeader, szTitle);

		// evaluate and format each cell in the row (on col. position at a time)
		for (enum break_class ca = OP; ca <=JT; nextclass(ca) )
		{
			/**
			// LB 1  Assign a line breaking class to each code point of the input. Resolve AI, CB, SA, SG, XX
			// LB 2  Never break at the start of text.
			// 2: × sot
			// LB 3  Always break at the end of text.
			// 3: ! eot
			// LB 4  Always break after hard line breaks (but never between CR and LF).
			// 4: BK !
			// LB 5  Treat CR followed by LF, as well as CR, LF and NL as hard line breaks.
			if (cb == CR && cb == LF)
				no_break(cb, ca); // 3b: CR × LF
			else if (cb == CR || cb == LF || cb == NL)
				must_break_after(cb); // 3b: ( CR | LF | NL ) !
			//LB 6  Do not break before hard line breaks.
			else if (ca == BK || ca == CR || ca == LF || ca == NL)
				no_break_pair(ca); // 3c: × ( BK | CR | LF | NL )

			// LB 7  Do not break before spaces or zero-width space.
			else**/ if (ca == SP || ca == ZW)
				no_break_pairs_with_space(cb, ca, "7: × ( SP | ZW )"); // 7: × ( SP | ZW )
			// LB 8  Break after zero-width space.
			else if (cb == ZW)
				break_pair(cb, ca, "8: ZW ÷"); // 8: ZW ÷
			// LB 9  Do not break a combining character sequence;  treat it as if it has the LB class of the base character in all of the following rules.
			// Treat X CM* as if it were X.
			// Where X is any line break class except SP, BK, CR, LF, NL or ZW. 
			// For a pair table implementation LB 9 can be restated equivalently as: X CM* -> X +
			// This is handled by putting X × CM (which includes CM × CM) into the pair table, and
			// changing the break_action to account for the additional rule that
			// CM takes on the class of X for later line break
			else if ((cb == OP) && (ca == CM))
				no_break_pairs_with_space_for_combining(cb, ca, "9: X CM* -> X ; 14: OP SP * × ; 7: × ( SP | ZW )"); // 9: X CM* -> X ; 14: OP SP * x  ; 7: × ( SP | ZW )
			else if ((cb != SP && cb != BK && cb != CR && cb != LF && cb != NL && cb != ZW) && (ca == CM))
				no_break_without_space_for_combining(cb, ca, "9: X CM* -> X ; 18: SP ÷ (if X == SP)"); // 9: X CM* -> CM ; 31: ALL ÷ 
			//LB 10  Treat any remaining combining mark as AL.
			// carried out by rewriting all rules below that use AL
			// LB 11  Do not break before or after WORD JOINER and related characters.
			else if ( ca == WJ)
				no_break_pairs_with_space(cb, ca, "11: × WJ; ; 7: × ( SP | ZW )"); // 11: × WJ ; 7: × ( SP | ZW )
			// must exclude all later context starting in x, such as rule LB 8, which occur before rule LB 18
			else if ( cb == WJ && !(ca == CL || ca == EX || ca == IS || ca == SY))
				no_break_pair(cb, ca, "11: WJ × ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 11: WJ × ; 7: × ( SP | ZW ) ; 18: SP ÷
#ifdef v500
// Version 5.0.0
			// LB 12  Do not break before or after NBSP and related characters.
			// To account for [^SP] must exclude all later contexts ending in SP, such as rule LB 14, which occur before rule LB 18
			else if ( cb != OP && ca == GL)
				no_break_pair(cb, ca, "12: (!SP) × GL ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 12: [^SP] × GL ; 7: × ( SP | ZW )
			// must exclude all later context starting in x, such as rule LB 13, which occur before rule LB 18
			else if ( cb == GL && !(ca == CL || ca == EX || ca == IS || ca == SY))
				no_break_pair(cb, ca, "12: GL × ; 7: × ( SP | ZW )  ; 18: SP ÷"); // 12: GL × ; 7: × ( SP | ZW ) 
#else
// Version 5.0.1 and Version 5.1.0
			// LB 12a  Do not break after NBSP and related characters.
			// must exclude all later context starting in x, such as rule LB 13, which occur before rule LB 18
			else if ( cb == GL && !(ca == CL || ca == EX || ca == IS || ca == SY))
				no_break_pair(cb, ca, "12: GL × ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 12: GL × ; 7: × ( SP | ZW ); 18: SP ÷
			// LB 12b  Do not break before NBSP and related characters except after SP, BA and HY
			else if ((cb == BA || cb == HY) && ca == GL)
				break_pair(cb, ca, "12a: [^SP, BA, HY] × GL ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 12a [^SP, BA, HY] × GL ; 7: × ( SP | ZW ) ; 18: SP ÷
			// To account for [^SP] must exclude all later contexts ending in SP, such as rule LB 14, which occur before rule LB 18
			else if ( cb != OP && ca == GL)
				no_break_pair(cb, ca, "12a: [^SP, BA, HY] × GL ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 12a: [^SP, BA, HY] × GL ; 7: × ( SP | ZW ) ;  18: SP ÷
#endif
			// LB 13  Do not break before ‘]’ or ‘!’ or ‘;’ or ‘/’, even after spaces.
			else if(ca == CL || ca == EX || ca == IS || ca == SY )
				no_break_pairs_with_space(cb, ca, "13: × (CL | EX | IS | SY ) ; 7: × ( SP | ZW )"); // 13: × (CL | EX | IS | SY ) ; 7: × ( SP | ZW )
			// LB 14  Do not break after ‘[’, even after spaces.
			else if(cb == OP)
				no_break_pairs_with_space(cb, ca, "14: OP SP* × ; 7: × ( SP | ZW )"); // 14: OP SP* × ; 7: × ( SP | ZW )
			// LB 15  Do not break within ‘”[’, , even with intervening spaces.
			else if (cb == QU && ca == OP)
				no_break_pairs_with_space(cb, ca, "15: QU SP* × OP ;"); // 15: QU SP* × OP ; 7: × ( SP | ZW )
			// LB 16  Do not break within ‘]h’, even with intervening spaces.
			else if (cb == CL && ca == NS)
				no_break_pairs_with_space(cb, ca, "16: CL SP* × NS ;"); // 16: CL SP* × NS ; 7: × ( SP | ZW )
			// LB 17  Do not break within ‘——’, even with intervening spaces.
			else if (cb == B2 && ca == B2)
				no_break_pairs_with_space(cb, ca, "17: B2 SP* × B2;"); // 17: B2 × B2; ; 7: × ( SP | ZW )
			// LB 18  Break after spaces.
			//else if (cb == SP)
			// break_pair(cb, ca, "18: SP ÷"); // 18: SP ÷
			// ************************************************************************************
			// handled by allowing rule 18: below (no_break_pair, vs. no_break_pairs_with_space for earlier rules).
			// ***********************************************************************************
			// LB 19  Do not break before or after ‘”’.
			else if (ca == QU)
				no_break_pair(cb, ca, "19: × QU ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 19: × QU ; 7: × ( SP | ZW ) ; 18: SP ÷
			else if (cb == QU)
				no_break_pair(cb, ca, "19: QU × ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 19: QU × ; 7: × ( SP | ZW ) ; 18: SP ÷
			// LB 20  Break before/after unresolved CB
			// in the demo code, we map CB to B2, so we can avoid a redundant
			// row/col in the pair table, but the result is that CB CB doesn't break when otherwise it should
			else if (ca == CB)
				break_pair(cb, ca, "20: ÷ CB ; 7: × ( SP | ZW ); 18: SP ÷"); // 20: ÷ CB; 7: × ( SP | ZW ) ; 18: SP ÷  
			else if (cb == CB)
				break_pair(cb, ca, "20: CB ÷ ; 7: × ( SP | ZW ); 18: SP ÷"); // 20: CB ÷ ; 7: × ( SP | ZW ) ; 18: SP ÷
			// LB 21  Do not break before hyphen-minus, other hyphens, fixed-width spaces, small kana and other non-starters, or after acute accents.
			else if (ca == BA || ca == HY || ca == NS)
				no_break_pair(cb, ca, "21: × BA | HY | NS ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 15:× BA | HY | NS ; 7: × ( SP | ZW ) ; 18: SP ÷
			else if (cb == BB)
				no_break_pair(cb, ca, "21: BB × ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 21: BB × ; 7: × ( SP | ZW ) ; 18: SP ÷
			// LB 22  Do not break between two ellipses, or between letters or numbers and ellipsis.
			else if (cb == CM && ca == IN)
				no_break_pair(cb, ca, "10: CM->AL ; 22: CM * IN )  ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 10: CM->AL ; 22: CM * IN )  ; 7: × ( SP | ZW ) ; 18: SP ÷
			else if ((cb == AL || cb == ID || cb == IN || cb == NU) && ca == IN)
				no_break_pair(cb, ca, "22:( AL | ID | IN | NU )× IN  ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 22:( AL | ID | IN | NU )× IN  ; 7: × ( SP | ZW ) ; 18: SP ÷
			// LB 23  Do not break within ‘a9’, ‘3a’, or ‘H%’.
			else if (cb == ID && ca == PO)
				no_break_pair(cb, ca, "23: ID × PO ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 23: ID × PO ; 7: × ( SP | ZW ) ; 18: SP ÷
			else if (cb == AL && ca == NU)
				no_break_pair(cb, ca, "23: AL × NU ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 23: AL × NU ; 7: × ( SP | ZW ) ; 18: SP ÷
			else if (cb == NU && ca == AL)
				no_break_pair(cb, ca, "23: NU × AL ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 23: NU × AL ; 7: × ( SP | ZW ) ; 18: SP ÷
			else if (cb == CM && ca == NU)
				no_break_pair(cb, ca, "10: CM->AL ; 23: CM × NU ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 10: CM->AL ; 23: CM × NU ; 7: × ( SP | ZW ) ; 18: SP ÷
			else if (cb == NU && ca == CM)
				no_break_pair(cb, ca, "10: CM->AL ; 23: NU × CM ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 10: CM->AL ; 23: NU × CM ; 7: × ( SP | ZW ) ; 18: SP ÷
			// LB 24:
			else if ( cb == PR && (ca == AL || ca == ID) )
				no_break_pair(cb, ca, "24: PR × ( AL |  ID) ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 24: PR × ( AL | ID) ; 7: × ( SP | ZW ) ; 18: SP ÷
			else if ( cb == PO && ca == AL)
				no_break_pair(cb, ca, "24: PO × AL; 7: × ( SP | ZW ) ; 18: SP ÷"); // 24: PO × AL ; 7: × ( SP | ZW ) ; 18: SP ÷
			// LB 25  Do not break between the following pairs of classes.
			else if( (cb == CL || cb == NU) && (ca == PO || ca == PR) )
				no_break_pair(cb, ca, "25: ( CL | NU )× (PO | PR) ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 25:( CL | NU )× PO ; 7: × ( SP | ZW ) ; 18: SP ÷
			else if( (cb == HY || cb == IS || cb ==  NU || cb == SY ) && ca == NU)
				no_break_pair(cb, ca, "25: ( HY | IS | NU | SY )× NU ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 25:( HY | IS | NU | SY )× NU ; 7: × ( SP | ZW ) ; 18: SP ÷
			else if ( (cb == PR || cb == PO) && (ca == HY || ca == NU || ca == OP) )
				no_break_pair(cb, ca, "25: PR × ( HY | NU | OP ) ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 25: (PO | PR) × ( HY | NU | OP ) ; 7: × ( SP | ZW ) ; 18: SP ÷
			// should be redundant
			//else if(cb == SY && ca == NU)
			//	no_break_pair(cb, ca, "25: SY × NU ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 25: SY × NU ; 7: × ( SP | ZW ) ; 18: SP ÷
			else if (cb == PR && ca == CM)
				no_break_pair(cb, ca, "10: CM -> AL ; 25: PR × AL ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 10: CM->AL ; 25: PR × CM ; 7: × ( SP | ZW ) ; 18: SP ÷
			// LB 26 Do not break a Korean syllable.
			else if (cb == JL  && (ca == JL || ca == JV || ca == H2 || ca == H3 ))
				no_break_pair(cb, ca, "26: JL  × ( JL | JV | H2 | H3 ) ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 26: JL  × ( JL | JV | H2 | H3 ) ; 7: × ( SP | ZW ) ; 18: SP ÷
			else if ((cb == JV || cb == H2 ) && (ca == JV || ca == JT))
				no_break_pair(cb, ca, "26: ( JV | H2 ) × ( JV | JT ) ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 26: ( JV | H2 ) × ( JV | JT ) ; 7: × ( SP | ZW ) ; 18: SP ÷
			else if ((cb == JT || cb == H3 ) && ca == JT)
				no_break_pair(cb, ca, "26: ( JT | H3 ) × JT ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 26: ( JT | H3 ) × JT ; 7: × ( SP | ZW ) ; 18: SP ÷
			// LB 27 Treat a Korean Syllable Block the same as ID.
			else if ((cb == JL || cb == JV || cb == JT || cb == H2 || cb == H3 ) && (ca == IN || ca == PO))
				no_break_pair(cb, ca, "27: ( JL | JV | JT | H2 | H3 ) × (IN | PO) ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 27: ( JL | JV | JT | H2 | H3 ) × (IN | PO) ; 7: × ( SP | ZW ) ; 18: SP ÷
			else if ((cb == PR) && (ca == JL || ca == JV || ca == JT || ca == H2 || ca == H3))
				no_break_pair(cb, ca, "27: (PR × ( JL | JV | JT | H2 | H3 ) ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 27: (PR × ( JL | JV | JT | H2 | H3 ) ; 7: × ( SP | ZW ) ; 18: SP ÷
			// LB 28  Do not break between alphabetics (“at”).
			else if (cb == AL && ca == AL)
				no_break_pair(cb, ca, "28: AL × AL ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 28: AL × AL ; 7: × ( SP | ZW ) ; 18: SP ÷
			else if ((cb == CM && ca == AL) || (cb == AL && ca == CM))
				no_break_pair(cb, ca, "9: CM -> AL && 28: AL * AL ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 9: CM -> AL && 28: AL * AL ; 7: × ( SP | ZW ) ; 18: SP ÷
			// LB 29  Do not break between numeric punctuation and alphabetics ("e.g.").
			else if (cb == IS && ca == AL)
				no_break_pair(cb, ca, "29: IS × AL ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 29: IS × AL ; 7: × ( SP | ZW ) ; 18: SP ÷
#if defined(v500) || defined(v501)
// Version 5.0.0 and Version 5.0.1
			// LB 30 Do not break between letters, numbers or ordinary symbols and opening or closing punctuation 
			else if ( ca == OP && (cb == AL || cb == NU)) 
				no_break_pair(cb, ca, "30: (AL | NU)  × OP ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 30: (AL | NU) × OP ; 7: × ( SP | ZW ) ; 18: SP ÷
			else if ( cb == CL && (ca == AL || ca == NU)) 
				no_break_pair(cb, ca, "30: CL × (AL | NU) ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 30: CL × (AL | NU) ; 7: × ( SP | ZW ) ; 18: SP ÷
#endif
			// LB 31  Break everywhere else.
			else
				break_pair(cb, ca, "31: ALL ÷ ; ÷ ALL ; 7: × ( SP | ZW ) ; 18: SP ÷"); // 31: ALL ÷ ; ÷ ALL ;  7: × ( SP | ZW ) ; 18: SP ÷
		}
		terminate_row();
	}
	terminate_table();
}
#endif // ifdef VERIFY_PAIR TABLE

void verifyTable()
{
	#ifdef VERIFY_PAIR_TABLE
	class table_verify tv;
	tv.verifyAndPrintTable();
	#endif
}


//[EOF]

