#include "NNHelpers.h"
#include "NeuralCLI/network.hpp"
#include "EncrypterCLI.h"
#include "Enclave.h"

#include "PrintConsole.h"
#include <string> 


HWND trainUpldBtn;
HWND trainBtn;

void addTrainFileControls(HWND hwnd)
{
	//-------------------------------------------------------------------------------------------------------
	// This function creates Train  controls 
	// @PARAM HWND window handler of  the Main app
	HWND hwndEncTitle = CreateWindowW(L"static", L"Encrypted Files", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 80, 150, 205, 40, hwnd, (HMENU)NO_BG_WHITE_TXT, NULL, NULL);
	HFONT hFont = CreateFont(26, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");


	trainUpldBtn = CreateWindowW(L"Button", L"Upload Train Set", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 751, 381, 133, 36, hwnd, (HMENU)OPEN_TRAIN_FILE_BUTTON, NULL, NULL);
	trainBtn = CreateWindowW(L"Button", L"Train", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 951, 381, 133, 36, hwnd, (HMENU)TRAIN_BUTTON, NULL, NULL);

	HBITMAP imageUpload = (HBITMAP)LoadImage((HINSTANCE)NULL, L"\\MasterThesis\\Encrypter\\Encrypter\\Encrypter\\icons\\uploadSmall.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HWND hwndUpload = CreateWindowW(L"Static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, 790, 300, 10, 50, hwnd, NULL, NULL, NULL);
	
	HBITMAP imageTrain = (HBITMAP)LoadImage((HINSTANCE)NULL, L"\\MasterThesis\\Encrypter\\Encrypter\\Encrypter\\icons\\learn.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HWND hwndTrain = CreateWindowW(L"Static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, 990, 295, 10, 50, hwnd, NULL, NULL, NULL);

	SendMessage(hwndEncTitle, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(hwndTrain, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imageTrain);
	SendMessage(hwndUpload, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imageUpload);

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
	int y = 200;

	//_tprintf(TEXT("Target file is %s\n"), argv[1]);
	hFind = FindFirstFile(L"/MasterThesis/Encrypter/Encrypter/Encrypter/.tmp/.encrypted/*", &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		//printf("FindFirstFile failed (%d)\n", GetLastError());
		MessageBox(NULL, L"No file here", L"List Files", MB_OK);

		return;
	}
	else
	{
		FindNextFile(hFind, &FindFileData);
		FindNextFile(hFind, &FindFileData);
		while (FindNextFile(hFind, &FindFileData))
		{
			//MessageBox(NULL, FindFileData.cFileName, L"File Copy", MB_OK);
			CreateWindowW(L"Static", FindFileData.cFileName, WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 40, y, 270, 30, hwnd, (HMENU)LIST_ENCRYPTED, NULL, NULL);
			y = y + 35;

		}

		FindClose(hFind);
		//MessageBox(NULL, (LPCWSTR)names[0], L"File Copy", MB_OK);
		HWND hComment = CreateWindowW(L"Static", L"Files cannot be downloaded.", WS_VISIBLE | WS_CHILD, 40, y, 270, 30, hwnd, (HMENU)NO_BG_WHITE_TXT, NULL, NULL);
		HFONT hFont = CreateFont(14, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
		SendMessage(hComment, WM_SETFONT, WPARAM(hFont), TRUE);
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
						wchar_t saveTo[100] = L"/MasterThesis/Encrypter/Encrypter/Encrypter/.tmp/.encrypted/";
						wcscat_s(saveTo, 100, pszFileName);


						//	//Call ReadFile with the buffer initialized above.
						//Copy the file to the Data Directory
						if (copyFileToDest(pszFilePath, (LPCWSTR)saveTo))
						{
							MessageBox(NULL, L"File Successfully Added", L"File Copy", MB_OK);

							__printf((LPWSTR)L"File Upload - OK.");

						}
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
	
	////decrypt the datasets
	Crypto decrypt;

	//Take File names from directory - Same prefix file name for CT, BLOB and PT
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	int y = 160;
	hFind = FindFirstFile(L"/MasterThesis/Encrypter/Encrypter/Encrypter/.tmp/.encrypted/*.CT", &FindFileData);
	//MessageBoxW(NULL, FindFileData.cFileName, L"First File", MB_OK);

	//Create Command 
	wchar_t a[100] = L"/MasterThesis/Encrypter/Encrypter/Encrypter/.tmp/.encrypted/";
	

	char command[800] = "cd D:/MasterThesis/Encrypter/SGX_ML/SGX_ML/NeuralCLI/ && main ";

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
			wchar_t CTname[100] = L"/MasterThesis/Encrypter/Encrypter/Encrypter/.tmp/.encrypted/";
			wchar_t BLOBname[100] = L"/MasterThesis/Encrypter/Encrypter/Encrypter/.tmp/.filekeys/";
			wchar_t PTname[100] = L"/MasterThesis/Encrypter/Encrypter/Encrypter/.tmp/.decrypted/";
			DWORD file_size = 10;


			//MessageBox(NULL, FindFileData.cFileName, L"File Copy", MB_OK);
			pwcFileName = wcstok_s(FindFileData.cFileName, L".", &con); //get the file name without extension(hash)
			/*generateFileName(pwcFileName, CTname, CIPHERTEXT);
			generateFileName(pwcFileName, BLOBname, BLOB);
			generateFileName(pwcFileName, PTname, PLAINTEXT);*/
			wcscat_s(CTname, 100, pwcFileName);
			wcscat_s(CTname, 100, L".CT");
			
			wcscat_s(PTname, 100, pwcFileName);
			wcscat_s(PTname, 100, L".data");


			//Decrypt the file
			fileSize(CTname, &file_size); // get file size
			//if(unsealFile(CTname, file_size, PTname))
			//
			if (decrypt.decrypt(CTname, file_size, BLOBname, PTname))
			{	// Add file name to command string
				char b[100] = "";
				WideCharToMultiByte(CP_ACP, 0, pwcFileName, 100, b, 100, NULL, NULL);
				strcat_s(command, sizeof(command), b);
				strcat_s(command, sizeof(command), ".data");
				strcat_s(command, sizeof(command), " ");
				//MessageBoxA(NULL, command, "Decrypt Success", MB_OK);

			}
			else
				MessageBoxA(NULL, "Data Files Decryption : Failure", "File Decrypter", MB_OK);


		}
		__printf((LPWSTR)L"Data Files Decryption : OK");

		FindClose(hFind);


	}

	
		
		if(system(NULL))
			//MessageBox(NULL, L"system works", L"Train", MB_OK);
		//system("curl dir > NeuralCLI/file.html");
		//system("cd NeuralCLI");
		//system("dir > D:/MasterThesis/Encrypter/SGX_ML/SGX_ML/NeuralCLI/");

		system(command);
		//MessageBox(NULL, L"Train Sucessful", L"Train", MB_OK);
		__printf((LPWSTR)L"Training Status : Success");







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

