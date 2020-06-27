#ifndef UNICODE
#define UNICODE
#endif 
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include <stdio.h>
#include "MainHelpers.h"
#include <gdiplus.h>
#include "Enclave.h"
#include "PrintConsole.h"

using namespace Gdiplus;

LRESULT CALLBACK ClientWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HINSTANCE   g_hInst;
HWND consoleControl;	//Window Handler for the console window
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	//@PARAMS HINSTANCE hInstance - Instance of the Application.
	//@PARAMS HINSTANCE hPrevInst- Ignored for now.
	//@PARAMS PWSTR pCmdLine- Args passed from the command line. 

	//1. Register the window class.
	const wchar_t CLASS_NAME[] = L"EncrypterApplication";

	WNDCLASS wc = { };

	wc.lpfnWndProc = ClientWindowProc;  // Event Driven Programming 
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	g_hInst = hInstance;
	RegisterClass(&wc);

	//2.  Create the window.

	HWND hwndClient = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Encrypter Application",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,


		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwndClient == NULL)
	{
		return 0;
	}

	ShowWindow(hwndClient, nCmdShow);

	//3. Run the message loop. This is part of the Event driven programming. 

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK ClientWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//-----------------------------------------------------------------
	//@PARAMS HWND hwnd - Window Handler
	//@PARAMS UINT uMsg - Unsigned Message ID
	//@PARAMS WPARAM wParam - Window Parameter
	//@PARAMS LPARAM lParam - 

	PAINTSTRUCT ps;
	HDC hdc;
	static HBRUSH hbrGreen, hbrDrkGreen, bhrWhite;
	HDC hdcMem;
	switch (uMsg)
	{

	case WM_DESTROY:
		//Quit the App when close button is clicked. 

		PostQuitMessage(0);
		return 0;


	case WM_CREATE:
	{
		//
		//testEnclave();

		
		loadMainApp(hwnd);
		hbrGreen = CreateSolidBrush(RGB(58, 179, 151));
		hbrDrkGreen = CreateSolidBrush(RGB(31, 117, 97));
		bhrWhite = CreateSolidBrush(RGB(255, 255, 255));

		break;

	}
	case WM_PAINT:

		hdc = BeginPaint(hwnd, &ps);
		
		drawRect(hwnd, hdc);

		//TextOut(hdc, 0, 0, L"Hello", 15);

		EndPaint(hwnd, &ps);


		return 0;
	
	case WM_CTLCOLORSTATIC:
	{
		//---------------------------------Color Schemes------------------------------------------------------
		DWORD CtrlID = GetDlgCtrlID((HWND)lParam); //Window Control ID
		if (CtrlID == SAVE_ENCRYPTED) //If desired control
		{
			HDC hdcStatic = (HDC)wParam;
			SetTextColor(hdcStatic, RGB(255, 255, 255));
			SetBkMode(hdcStatic, TRANSPARENT);
			return (LONG)hbrDrkGreen;
		}
		if (CtrlID == NO_BG_WHITE_TXT)
		{
			HDC hdcStatica = (HDC)wParam;
			SetTextColor(hdcStatica, RGB(255, 255, 255));
			SetBkMode(hdcStatica, TRANSPARENT);
			return (LONG)hbrGreen;
		}
		if (CtrlID == WHITE_BG_GREEN_TXT)
		{
			HDC hdcStatica = (HDC)wParam;
			SetTextColor(hdcStatica, RGB(31, 117, 97));
			SetBkMode(hdcStatica, TRANSPARENT);
			return (LONG)bhrWhite;
		}

		
		
		return (LRESULT)GetStockObject(HOLLOW_BRUSH);

		 
	}

	case WM_COMMAND:
		// This command is passed if any button/menu is clicked. 
		wchar_t uhash[300] = L"abc";
		DWORD cbUhash = 4;
		switch (wParam)
		{
			
		case OPEN_ENCRYPT_FILE_BUTTON:
		{
			uploadFileEncrypterProc(hwnd, uhash, cbUhash);
			listEncryptedFilesDir(hwnd);

			break;
		}
		case SAVE_ENCRYPTED:
		{
			if(HIWORD(wParam) == STN_CLICKED) 
			{
				wchar_t fileName[100];
				GetWindowTextW((HWND)lParam, (LPWSTR)fileName, 100);
				//MessageBox(NULL, (LPCWSTR)fileName, L"File Copy", MB_OK);

				saveEncrypted(hwnd, (LPWSTR)fileName);
				break;
			}
		}
		case LOGIN_NEXT :
			//Authenticate User
			
			//if (authenticateWinCred(5, uhash, cbUhash))
			if(1)
			{
				//Remove the LoginControls
				removeLoginContols(hwnd);
				//Add ClientArea controls
				addFileEncrypterUploadControls(hwnd);
				consoleControl = addConsoleControl(hwnd);
				listEncryptedFilesDir(hwnd);
				__printf((LPWSTR)L"User Authentication - OK.");


			}
			break;
		
		break;
		}
		
	break;
	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


