
#include "MainHelpers.h"
#include <vector>
#include <string>
#include <stdio.h>



using std::string;
using std::vector;

// Declare Window Handers
HWND btnLogin, b;
HWND hwnda, title, hwndEncrypter;
HWND s0, s1, s2, s3;
HBITMAP image, imageLogin;
HWND hwndImage, hwndImageLogin;
int xSteps = 60;
int ySteps = 300;


void drawRect(HWND hwnd, HDC hdc)
{
	//-------------------------------------------------------------------------------------------------------
		// This function creates a Colored Rectangle.  
		// @PARAM HWND Window handler to the parent window. 
		// @PARAM HDC Device Cpntext Handle of the paint. 
		// Returns - NULL.
	RECT parentRect;
	long width, height;
	if (GetWindowRect(hwnd, &parentRect))
	{
		width = parentRect.right - parentRect.left;
		height = parentRect.bottom - parentRect.top;
	}

	//White BG_ This helped in debugging the window resize render error
	RECT rect = { 0, 0, width,height };
	HBRUSH brush = CreateSolidBrush(RGB(255,255,255));
	FillRect(hdc, &rect, brush);
	DeleteObject(brush);

	rect = { 0, 0, (width/2)+50,height };
	brush = CreateSolidBrush(RGB(58, 179, 151));
	FillRect(hdc, &rect, brush);
	DeleteObject(brush);


}

void addLoginContols(HWND hwnd)
{
	//-------------------------------------------------------------------------------------------------------
		// This function creates windows and controls for Login page.  
		// @PARAM HWND Window handler to the parent window. 
		// Returns - NULL.
	//Creates a label
	title = CreateWindowW(L"Static", L"Confidential Computing using Intel SGX.", WS_VISIBLE | WS_CHILD | SS_CENTER, 80, 100, 400, 309, hwnd, (HMENU)NO_BG_WHITE_TXT, NULL, NULL);
	HFONT hFontTitle = CreateFont(36, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
	 
	HFONT hFontSteps = CreateFont(18, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
	s0 = CreateWindowW(L"Static", L"Steps to use:", WS_VISIBLE | WS_CHILD | SS_LEFT, xSteps, ySteps, 400, 30, hwnd, (HMENU)NO_BG_WHITE_TXT, NULL, NULL);
	s1 = CreateWindowW(L"Static", L"1. Login to File Encrypter", WS_VISIBLE | WS_CHILD | SS_LEFT, xSteps, ySteps+30, 400, 30, hwnd, (HMENU)NO_BG_WHITE_TXT, NULL, NULL);
	s2 = CreateWindowW(L"Static", L"2. Upload data sets to be encrypted", WS_VISIBLE | WS_CHILD | SS_LEFT, xSteps, ySteps+60, 400, 30, hwnd, (HMENU)NO_BG_WHITE_TXT, NULL, NULL);
	s3 = CreateWindowW(L"Static", L"3. Download encrypted files", WS_VISIBLE | WS_CHILD | SS_LEFT, xSteps, ySteps+90, 400, 30, hwnd, (HMENU)NO_BG_WHITE_TXT, NULL, NULL);

	btnLogin = CreateWindowW(L"Button", L"login", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 830, 381, 133, 36, hwnd, (HMENU)LOGIN_NEXT, NULL, NULL);

	image = (HBITMAP)LoadImage((HINSTANCE)NULL, L"\\MasterThesis\\Encrypter\\Encrypter\\Encrypter\\icons\\ml.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hwndImage = CreateWindowW(L"Static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, 860, 100, 50, 50, hwnd, (HMENU)LOGIN_NEXT, NULL, NULL);
	
	imageLogin = (HBITMAP)LoadImage((HINSTANCE)NULL, L"\\MasterThesis\\Encrypter\\Encrypter\\Encrypter\\icons\\login1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hwndImageLogin = CreateWindowW(L"Static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, 850, 250, 50, 30, hwnd, (HMENU)LOGIN_NEXT, NULL, NULL);


	//hwnda = CreateWindowW(L"Static", L"Login", WS_VISIBLE | WS_CHILD | SS_CENTER, 850, 230, 100, 36, hwnd, (HMENU)WHITE_BG_GREEN_TXT, NULL, NULL);
	HFONT hFont = CreateFont(36, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
	hwndEncrypter = CreateWindowW(L"Static", L"Encrypter", WS_VISIBLE | WS_CHILD | SS_CENTER, 800, 180, 200, 40, hwnd, (HMENU)WHITE_BG_GREEN_TXT, NULL, NULL);
	
	
	

	SendMessage(title, WM_SETFONT, WPARAM(hFontTitle), TRUE);
	SendMessage(s0, WM_SETFONT, WPARAM(hFontSteps), TRUE);
	SendMessage(s1, WM_SETFONT, WPARAM(hFontSteps), TRUE);
	SendMessage(s2, WM_SETFONT, WPARAM(hFontSteps), TRUE);
	SendMessage(s3, WM_SETFONT, WPARAM(hFontSteps), TRUE);
	SendMessage(hwnda, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(hwndEncrypter, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(hwndImage, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)image);
	SendMessage(hwndImageLogin, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imageLogin);



}

void removeLoginContols(HWND hwnd)
{	
	//SetWindowText(b,TEXT("SHIT"));
	ShowWindow(btnLogin, 0);
	ShowWindow(hwnda, 0);
	ShowWindow(title, 0);
	ShowWindow(s0, 0);
	ShowWindow(s1, 0);
	ShowWindow(s2, 0);
	ShowWindow(s3, 0);
	ShowWindow(hwndImageLogin, 0);
}

void loadMainApp(HWND hwnd)
{
	addLoginContols(hwnd);
}

HWND addConsoleControl(HWND hwnd)
{
	//-------------------------------------------------------------------------------------------------------
		// This function creates a console window to display print statements.  
		// @PARAM HWND Window handler to the parent window. 
		// Returns - the window handle of the console.

	//Edit box to the main window.
	HWND hwndConsole = CreateWindowEx(
		0, L"EDIT",   // predefined class 
		NULL,         // no window title 
		WS_CHILD | WS_VISIBLE | WS_VSCROLL |
		ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
		360, 160, 215, 300,   // set size in WM_SIZE message 
		hwnd,         // parent window 
		(HMENU)APPEND_CONSOLE,   // edit control ID 
		NULL,
		NULL);        // pointer not needed 


	return hwndConsole;

}

void printToConsoleControl(HWND hwnd, LPWSTR text)
{
	//-------------------------------------------------------------------------------------------------------
		// This function appends the input text to the console window.  
		// @PARAM HWND Window handler to the parent window. 
		// Returns - NULL.

	 // get edit control from dialog
	HWND hwndOutput = GetDlgItem(hwnd, APPEND_CONSOLE);

	// get the current selection
	DWORD StartPos, EndPos;
	SendMessage(hwndOutput, EM_GETSEL, reinterpret_cast<WPARAM>(&StartPos), reinterpret_cast<WPARAM>(&EndPos));

	// move the caret to the end of the text
	int outLength = GetWindowTextLength(hwndOutput);
	SendMessage(hwndOutput, EM_SETSEL, outLength, outLength);

	// insert the text at the new caret position

	SendMessage(hwndOutput, EM_REPLACESEL, TRUE, (LPARAM)text);

	// restore the previous selection
	SendMessage(hwndOutput, EM_SETSEL, StartPos, EndPos);


}




