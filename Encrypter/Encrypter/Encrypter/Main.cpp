#ifndef UNICODE
#define UNICODE
#endif 
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include "MainHelpers.h"




LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	//@PARAMS HINSTANCE hInstance - Instance of the Application.
	//@PARAMS HINSTANCE hPrevInst- Ignored for now.
	//@PARAMS PWSTR pCmdLine- Args passed from the command line. 

	//1. Register the window class.
	const wchar_t CLASS_NAME[] = L"EncrypterApplication";

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;  // Event Driven Programming 
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	//2.  Create the window.

	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Log in.",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	//3. Run the message loop. This is part of the Event driven programming. 

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		//loadMainApp(hwnd);
		addTrainFileUploadControls(hwnd);
		listTrainFilesDir(hwnd);

		

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
		//case LOGIN_NEXT :
		//	MessageBeep(MB_OK);
		//	//Authenticate User

		//	//Remove the LoginControls
		//	removeLoginContols(hwnd);
		//	//Add ClientArea controls
		//	break;

		case OPEN_TRAIN_FILE_BUTTON:
			uploadTrainFileProc(hwnd);
			listTrainFilesDir(hwnd);

			break;
		case TRAIN_BUTTON : 
			trainBtnClick();
		}
		break;
	/*case WM_CTLCOLORSTATIC:
		switch (wParam) {
			case TITLE:
				HDC hdcStatic = (HDC)wParam;
				SetTextColor(hdcStatic, RGB(255, 0, 0));
				break;
		}*/
	

		

	
	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
