
#include "MainHelpers.h"
#include <vector>
#include <string>
#include <stdio.h>



using std::string;
using std::vector;

// Declare Window Handers
HWND a, b;

void AddControls(HWND hwnd)
{
	//-------------------------------------------------------------------------------------------------------
		// This function creates windows and coontorls to the main App.  
		// @PARAM HWND Window handler to the parent window. 
		// Returns - NULL.
	HWND hwnda;
		
	//Creates a label
	hwnda = CreateWindowW(L"Static", L"Confidential Computing with Machine Learning using Intel SGX.", WS_VISIBLE | WS_CHILD | SS_CENTER, 60, 100, 300, 309, hwnd, (HMENU)TITLE, NULL, NULL);
	HFONT hFont = CreateFont(30, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
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
	brush = CreateSolidBrush(RGB(50, 151, 151));
	FillRect(hdc, &rect, brush);
	DeleteObject(brush);


}

void addLoginContols(HWND hwnd)
{
	//-------------------------------------------------------------------------------------------------------
		// This function creates windows and controls for Login page.  
		// @PARAM HWND Window handler to the parent window. 
		// Returns - NULL.


	a = CreateWindowW(L"Static", L"Username : ", WS_VISIBLE | WS_CHILD | SS_CENTER, 572, 234, 70, 20, hwnd, NULL, NULL, NULL);
	CreateWindowW(L"Static", L"Password : ", WS_VISIBLE | WS_CHILD | SS_CENTER, 572, 308, 70, 20, hwnd, NULL, NULL, NULL);
	b = CreateWindowW(L"Edit", L"...", WS_VISIBLE | WS_CHILD | WS_BORDER, 718, 225, 200, 40, hwnd, NULL, NULL, NULL);
	CreateWindowW(L"Edit", L"...", WS_VISIBLE | WS_CHILD | WS_BORDER, 718, 300, 200, 40, hwnd, NULL, NULL, NULL);
	CreateWindowW(L"Button", L"Next", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 751, 381, 133, 36, hwnd, (HMENU)LOGIN_NEXT, NULL, NULL);
	ShowWindow(a, 1);

}

void removeLoginContols(HWND hwnd)
{	
	SetWindowText(b,TEXT("SHIT"));
	ShowWindow(a, 0);
}

void loadMainApp(HWND hwnd)
{
	AddControls(hwnd);
	addLoginContols(hwnd);
}




