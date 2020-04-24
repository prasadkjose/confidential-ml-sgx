#include "NNHelpers.h"
#include "NeuralCLI/network.hpp"
#include "PrintConsole.h"
#include <string> 

//TODO : Comment all the new functions added today
HWND trainUpldBtn;
HWND trainBtn;


BOOL copyFileToDest(PWSTR pszFilePath, LPCWSTR saveTo) {
	if (CopyFile(pszFilePath, saveTo, TRUE))
		return 1;
	else
	{
		return 0;
	}

}

void listTrainFilesDir(HWND hwnd)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	BYTE names[] = { "0" };
	int y = 160;

	//_tprintf(TEXT("Target file is %s\n"), argv[1]);
	hFind = FindFirstFile(L"Data/*", &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		//printf("FindFirstFile failed (%d)\n", GetLastError());
		MessageBox(NULL, L"No file here", L"List Files", MB_OK);

		return;
	}
	else
	{

		while (FindNextFile(hFind, &FindFileData))
		{
			//MessageBox(NULL, FindFileData.cFileName, L"File Copy", MB_OK);
			CreateWindowW(L"Static", FindFileData.cFileName, WS_VISIBLE | WS_CHILD | SS_CENTER, 32, y, 200, 20, hwnd, NULL, NULL, NULL);
			y = y + 25;

		}

		FindClose(hFind);
		//MessageBox(NULL, (LPCWSTR)names[0], L"File Copy", MB_OK);

	}

}

void uploadTrainFileProc(HWND hwnd)
{
	// This function take a file path and copies it to the Data directory

	OPENFILENAME ofn;

	//The file name selected will be stored here
	char file_name[100];

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		IFileOpenDialog *pFileOpen;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
		
		if (SUCCEEDED(hr))
		{
			// Show the Open dialog box.
			hr = pFileOpen->Show(NULL);

			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				IShellItem *pItem;
				hr = pFileOpen->GetResult(&pItem);

				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{
						// concatenate the string with the Data directory path
						wchar_t saveTo[100] = L"Data/";
						wcscat_s(saveTo, 100, L"Train.txt");


						//	//Call ReadFile with the buffer initialized above.
						//Copy the file to the Data Directory
						if(copyFileToDest(pszFilePath, (LPCWSTR)saveTo))
							MessageBox(NULL, pszFilePath, L"File Copy", MB_OK);
						else
							MessageBox(NULL, L"File Open Error", L"File Copy", MB_OK);

						CoTaskMemFree(pszFilePath);
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}
}

void addTrainFileUploadControls(HWND hwnd)
{
	trainUpldBtn = CreateWindowW(L"Button", L"Upload Train Set", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 751, 381, 133, 36, hwnd, (HMENU)OPEN_TRAIN_FILE_BUTTON, NULL, NULL);
	trainBtn = CreateWindowW(L"Button", L"Train", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 951, 381, 133, 36, hwnd, (HMENU)TRAIN_BUTTON, NULL, NULL);

}

void trainBtnClick()
{	

	//for (int i = 0; i < 4; i++)
	//{
	//	wchar_t buf[100];
	//	int len = swprintf_s(buf, 100, L"%s%d", L"adjaso oasdoasiod apjdp9asdpasj 9adipasjdipp prasadj okjkjaskjd-", i);
	//__printf((LPWSTR)buf);

	//
	//}

	if (mainNN())
	{
		MessageBox(NULL, L"Train Sucessful", L"Train", MB_OK);

	}
	else
	{
		MessageBox(NULL, L"Train Failed", L"Train", MB_OK);

	}
}

HWND addConsoleControl(HWND hwnd)
{
	//Edit box to the main window.
	HWND hwndConsole = CreateWindowEx(
		0, L"EDIT",   // predefined class 
		NULL,         // no window title 
		WS_CHILD | WS_VISIBLE | WS_VSCROLL |
		ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
		260, 160, 215, 300,   // set size in WM_SIZE message 
		hwnd,         // parent window 
		(HMENU)APPEND_CONSOLE,   // edit control ID 
		NULL,
		NULL);        // pointer not needed 

	
	return hwndConsole;

}




void printToConsoleControl(HWND hwnd, LPWSTR text)
{
	
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
