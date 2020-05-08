#ifndef UNICODE
#define UNICODE
#endif 
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include <stdio.h>
#include "MainHelpers.h"

//URGENT: Test with multiple hashes and users- encrypt and Decrypt

#define IDS_FIRSTCOLUMN 0
LRESULT CALLBACK ClientWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HINSTANCE   g_hInst;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	//@PARAMS HINSTANCE hInstance - Instance of the Application.
	//@PARAMS HINSTANCE hPrevInst- Ignored for now.
	//@PARAMS PWSTR pCmdLine- Args passed from the command line. 

	//1. Register the window class.
	const wchar_t CLASS_NAME[] = L"Dashboard";

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
		L"Dashboard.",    // Window text
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
	switch (uMsg)
	{
	case WM_DESTROY:
		//Quit the App when close button is clicked. 

		PostQuitMessage(0);
		return 0;


	case WM_CREATE:
	{
		
		addFileEncrypterUploadControls(hwnd);
		listEncryptedFilesDir(hwnd);
		


	}
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		drawRect(hwnd, hdc);

		//TextOut(hdc, 0, 0, L"Hello", 15);

		EndPaint(hwnd, &ps);


		return 0;
	
	case WM_COMMAND:
		// This command is passed if any button/menu is clicked. 
		switch (wParam)
		{
		case OPEN_ENCRYPT_FILE_BUTTON:
		{
			uploadFileEncrypterProc(hwnd);
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
			
		
		
		break;
		}
		
	break;
	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


