
#include "MainHelpers.h"
#include <vector>
#include <string>
#include <stdio.h>



using std::string;
using std::vector;

// Declare Window Handers
HWND title, b;
HWND hwnda;

void AddControls(HWND hwnd)
{
	//-------------------------------------------------------------------------------------------------------
		// This function creates windows and coontorls to the main App.  
		// @PARAM HWND Window handler to the parent window. 
		// Returns - NULL.
		
	//Creates a label
	hwnda = CreateWindowW(L"Static", L"Confidential Computing with Machine Learning using Intel SGX.", WS_VISIBLE | WS_CHILD | SS_CENTER, 60, 100, 400, 309, hwnd, (HMENU)NO_BG_WHITE_TXT, NULL, NULL);
	HFONT hFont = CreateFont(36, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
	SendMessage(hwnda, WM_SETFONT, WPARAM(hFont), TRUE);
	
	/*CreateWindowW(L"Edit", L"...", WS_VISIBLE | WS_CHILD, 200, 152, 100, 50, hwnd, NULL, NULL, NULL);
	CreateWindowW(L"Button", L"Click", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 200, 190, 100, 30, hwnd, NULL, NULL, NULL);*/
}

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

	rect = { 0, 0, (width/2)-50,height };
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


	title = CreateWindowW(L"Button", L"login", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 751, 381, 133, 36, hwnd, (HMENU)LOGIN_NEXT, NULL, NULL);

}

void removeLoginContols(HWND hwnd)
{	
	SetWindowText(b,TEXT("SHIT"));
	ShowWindow(title, 0);
	ShowWindow(hwnda, 0);
}

void loadMainApp(HWND hwnd)
{
	AddControls(hwnd);
	addLoginContols(hwnd);
}




