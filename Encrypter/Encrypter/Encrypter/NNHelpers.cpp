#include "NNHelpers.h"
#include "NeuralCLI/network.hpp"
#include "EncrypterCLI/EncrypterCLI.h"

#include "PrintConsole.h"
#include <string> 


HWND trainUpldBtn;
HWND trainBtn;

void addTrainFileControls(HWND hwnd)
{
	//-------------------------------------------------------------------------------------------------------
	// This function creates Train  controls 
	// @PARAM HWND window handler of  the Main app

	trainUpldBtn = CreateWindowW(L"Button", L"Upload Train Set", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 751, 381, 133, 36, hwnd, (HMENU)OPEN_TRAIN_FILE_BUTTON, NULL, NULL);
	trainBtn = CreateWindowW(L"Button", L"Train", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 951, 381, 133, 36, hwnd, (HMENU)TRAIN_BUTTON, NULL, NULL);

}


BOOL copyFileToDest(PWSTR pszFilePath, LPCWSTR saveTo) {
	//-------------------------------------------------------------------------------------------------------
	// This function copies a file from a source to destination
	// @PARAM PWSTR poitner to existing file
	// @PARAM LPCWSTR poitner to destination file
	//Return - true if function succeeds. 

	if (CopyFile(pszFilePath, saveTo, TRUE))
		
		return 1;
	else
	{
		return 0;
	}

}

void listTrainFilesDir(HWND hwnd)
{
	//-------------------------------------------------------------------------------------------------------
	// This function lists the files in the "Data" Directory
	// @PARAM HWND Handle to the Window to display the list in 
	//
	//Return - true if function succeeds. 
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
	//-------------------------------------------------------------------------------------------------------
	// This function opens file selector and copies it to "data" directory
	// @PARAM HWND Handle to the Window to display the list in 

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
					PWSTR pszFileName;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
					hr = pItem->GetDisplayName(SIGDN_NORMALDISPLAY, &pszFileName);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{
						// concatenate the string with the Data directory path
						wchar_t saveTo[100] = L".tmp/.encrypted/";
						wcscat_s(saveTo, 100, pszFileName);


						//	//Call ReadFile with the buffer initialized above.
						//Copy the file to the Data Directory
						if(copyFileToDest(pszFilePath, (LPCWSTR)saveTo))
							MessageBox(NULL, L"File Successfully Added", L"File Copy", MB_OK);
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

void trainBtnClick()
{	
	//-------------------------------------------------------------------------------------------------------
		// This function is the event trigger to decrypt the datasets and the train Model function.  
		// NO PARAMS
	//-------------------------------------------------------------------------------------------------------
	
	//decrypt the datasets
	Crypto decrypt;

	//Take File names from directory - Same prefix file name for CT, BLOB and PT
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	int y = 160;
	hFind = FindFirstFile(L".tmp/.filekeys/*.BLOB", &FindFileData);
	//MessageBoxW(NULL, FindFileData.cFileName, L"First File", MB_OK);

	


	if (hFind == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, L"No file here", L"List Files", MB_OK);
		
		return;
	}
	else
	{

		while (FindNextFile(hFind, &FindFileData))
		{
			wchar_t * con;
			wchar_t * pwcFileName = new wchar_t[100];
			wchar_t CTname[100] = L".tmp/.encrypted/";
			wchar_t BLOBname[100] = L".tmp/.filekeys/";
			wchar_t PTname[100] = L".tmp/.decrypted/";
			DWORD file_size = 10;


			//MessageBox(NULL, FindFileData.cFileName, L"File Copy", MB_OK);
			pwcFileName = wcstok_s(FindFileData.cFileName, L".", &con); //get the file name without extension(hash)
			generateFileName(pwcFileName, CTname, CIPHERTEXT);
			generateFileName(pwcFileName, BLOBname, BLOB);
			generateFileName(pwcFileName, PTname, PLAINTEXT);

			//Decrypt the file
			fileSize(CTname, &file_size); // get file size
			if (decrypt.decrypt(CTname, file_size, BLOBname, PTname))
				MessageBox(NULL, pwcFileName, L"Decrypt Success",  MB_OK);
			else
				MessageBoxA(NULL, "Decrypt Failure", "File Enrypter", MB_OK);


		}

		FindClose(hFind);


	}

	
		
	

	/*if (mainNN())
	{
		MessageBox(NULL, L"Train Sucessful", L"Train", MB_OK);

	}
	else
	{
		MessageBox(NULL, L"Train Failed", L"Train", MB_OK);

	}*/
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
		260, 160, 215, 300,   // set size in WM_SIZE message 
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
