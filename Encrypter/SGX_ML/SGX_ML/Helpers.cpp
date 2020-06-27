
#include "Helpers.h"
#include <vector>
#include <string>
#include <stdio.h>
#include <Lmcons.h>
#include <windows.h>
#include <wincred.h>		


using std::string;
using std::vector;

// Declare Window Handers
HWND title, b, btnLogin;
HWND hwnda, hwndML, hwndImage, hwndLogin, hwndImageLogin;
HWND s0, s1, s2, s3, s4;
HBITMAP image, imageLogin;
int xSteps = 60;
int ySteps = 300;


void AddControls(HWND hwnd)
{
	//-------------------------------------------------------------------------------------------------------
		// This function creates windows and coontorls to the main App.  
		// @PARAM HWND Window handler to the parent window. 
		// Returns - NULL.

	//Creates a label
	hwnda = CreateWindowW(L"Static", L"Confidential Computing using Intel SGX.", WS_VISIBLE | WS_CHILD | SS_CENTER, 80, 100, 400, 309, hwnd, (HMENU)NO_BG_WHITE_TXT, NULL, NULL);
	HFONT hFont = CreateFont(36, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
	

	HFONT hFontSteps = CreateFont(18, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
	s0 = CreateWindowW(L"Static", L"Steps to use:", WS_VISIBLE | WS_CHILD | SS_LEFT, xSteps, ySteps, 400, 30, hwnd, (HMENU)NO_BG_WHITE_TXT, NULL, NULL);
	s1 = CreateWindowW(L"Static", L"1. Login to ML Uploader", WS_VISIBLE | WS_CHILD | SS_LEFT, xSteps, ySteps + 30, 400, 30, hwnd, (HMENU)NO_BG_WHITE_TXT, NULL, NULL);
	s2 = CreateWindowW(L"Static", L"2. Upload data sets using the \"Upload Train Set\" button ", WS_VISIBLE | WS_CHILD | SS_LEFT, xSteps, ySteps + 60, 400, 30, hwnd, (HMENU)NO_BG_WHITE_TXT, NULL, NULL);
	s3 = CreateWindowW(L"Static", L"3. Train the Data using  the \"Train\" button", WS_VISIBLE | WS_CHILD | SS_LEFT, xSteps, ySteps + 90, 400, 30, hwnd, (HMENU)NO_BG_WHITE_TXT, NULL, NULL);
	s4 = CreateWindowW(L"Static", L"4. The result is saved to file.", WS_VISIBLE | WS_CHILD | SS_LEFT, xSteps, ySteps + 120, 400, 30, hwnd, (HMENU)NO_BG_WHITE_TXT, NULL, NULL);

	
	SendMessage(hwnda, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(s0, WM_SETFONT, WPARAM(hFontSteps), TRUE);
	SendMessage(s1, WM_SETFONT, WPARAM(hFontSteps), TRUE);
	SendMessage(s2, WM_SETFONT, WPARAM(hFontSteps), TRUE);
	SendMessage(s3, WM_SETFONT, WPARAM(hFontSteps), TRUE);
	SendMessage(s4, WM_SETFONT, WPARAM(hFontSteps), TRUE);
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
	HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
	FillRect(hdc, &rect, brush);
	DeleteObject(brush);

	rect = { 0, 0, (width / 2) + 50,height };
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


	btnLogin = CreateWindowW(L"Button", L"login", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 830, 381, 133, 36, hwnd, (HMENU)LOGIN_NEXT, NULL, NULL);
	image = (HBITMAP)LoadImage((HINSTANCE)NULL, L"\\MasterThesis\\Encrypter\\Encrypter\\Encrypter\\icons\\ml.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hwndImage = CreateWindowW(L"Static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, 860, 100, 100, 100, hwnd, (HMENU)LOGIN_NEXT, NULL, NULL);

	//hwndLogin = CreateWindowW(L"Static", L"Login", WS_VISIBLE | WS_CHILD | SS_CENTER, 850, 250, 100, 100, hwnd, (HMENU)WHITE_BG_GREEN_TXT, NULL, NULL);
	HFONT hFont = CreateFont(36, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
	hwndML = CreateWindowW(L"Static", L"ML Uploader", WS_VISIBLE | WS_CHILD | SS_CENTER, 800, 180, 200, 40, hwnd, (HMENU)WHITE_BG_GREEN_TXT, NULL, NULL);

	
	imageLogin = (HBITMAP)LoadImage((HINSTANCE)NULL, L"\\MasterThesis\\Encrypter\\Encrypter\\Encrypter\\icons\\login.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hwndImageLogin = CreateWindowW(L"Static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, 850, 250, 50, 30, hwnd, (HMENU)LOGIN_NEXT, NULL, NULL);


	
	SendMessage(hwndML, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(hwndLogin, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(hwndImage, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)image);
	SendMessage(hwndImageLogin, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imageLogin);


}

void removeLoginContols(HWND hwnd)
{
	ShowWindow(btnLogin, 0);
	ShowWindow(hwnda, 0);
	ShowWindow(title, 0);
	ShowWindow(hwndLogin, 0);
	ShowWindow(s0, 0);
	ShowWindow(s1, 0);
	ShowWindow(s2, 0);
	ShowWindow(s3, 0);
	ShowWindow(s4, 0);
	ShowWindow(hwndImageLogin, 0);

}

void loadMainApp(HWND hwnd)
{
	AddControls(hwnd);
	addLoginContols(hwnd);
}

bool authenticateWinCred(int iMaxTries, LPWSTR pszUname, DWORD cbUname)
{
	//-------------------------------------------------------------------------------------------------------
	// This function which handles user authentication
	// @PARAM int iMaxTries - Number of retries

	CREDUI_INFOW credui = {};
	credui.cbSize = sizeof(credui);
	credui.hwndParent = nullptr;
	credui.pszMessageText = L"Enter Credentials";
	credui.pszCaptionText = L"Credentials";
	credui.hbmBanner = nullptr;

	ULONG  authPackage = 0,
		outCredSize = 0;
	LPVOID outCredBuffer = nullptr;
	BOOL   save = false;

	DWORD err = 0;
	int   tries = 0;

	bool reAsk = false;

	do
	{
		tries++;

		if (CredUIPromptForWindowsCredentialsW(&credui,
			err,
			&authPackage,
			nullptr,
			0,
			&outCredBuffer,
			&outCredSize,
			&save,
			CREDUIWIN_ENUMERATE_ADMINS)

			!= ERROR_SUCCESS)
			return false;


		ULONG cchUserName = 0;
		ULONG cchPassword = 0;
		ULONG cchDomain = 0;
		ULONG cchNeed, cchAllocated = 0;

		static volatile UCHAR guz = 0;

		PWSTR stack = (PWSTR)alloca(guz);
		PWSTR szUserName = nullptr, szPassword = nullptr, szDomainName = nullptr;

		BOOL ret;

		do {
			if (cchAllocated < (cchNeed = cchUserName + cchPassword + cchDomain))
			{
				szUserName = (PWSTR)alloca((cchNeed - cchAllocated) * sizeof(WCHAR));
				cchAllocated = (ULONG)(stack - szUserName);
				szPassword = szUserName + cchUserName;
				szDomainName = szPassword + cchPassword;
			}

			ret = CredUnPackAuthenticationBuffer(
				CRED_PACK_PROTECTED_CREDENTIALS, outCredBuffer, outCredSize, szUserName, &cchUserName,
				szDomainName, &cchDomain, szPassword,
				&cchPassword);

		} while (!ret && GetLastError() == ERROR_INSUFFICIENT_BUFFER);


		SecureZeroMemory(outCredBuffer, outCredSize);
		CoTaskMemFree(outCredBuffer);

		HANDLE handle = nullptr;

		if (LogonUser(szUserName,
			szDomainName,
			szPassword,
			LOGON32_LOGON_INTERACTIVE,
			LOGON32_PROVIDER_DEFAULT,
			&handle))
		{
			//Debug Display
			/*MessageBox(NULL, szUserName, L"Display uname", MB_OK);
			MessageBox(NULL, szPassword, L"Display Pass", MB_OK);*/
			MessageBoxW(NULL, szUserName, L"Display Pass", MB_OK);
			memcpy(pszUname, szUserName, cchUserName * sizeof(WCHAR));
			cbUname = cchUserName * sizeof(WCHAR);
			CloseHandle(handle);
			return true;
		}

		else
		{
			err = ERROR_LOGON_FAILURE;
			reAsk = true;
		}


	} while (reAsk && tries < iMaxTries);
}
