#include "EncrypterHelpers.h"
#include "EncrypterCLI/EncrypterCLI.h"

//-------------------------------------------------------------------------------------------------------
//Client Dashboard Helper functions

HWND upldBtn;


void addFileEncrypterUploadControls(HWND hwnd)
{
	//-------------------------------------------------------------------------------------------------------
	// This function creates encrypter Upload controls 
	// @PARAM HWND window handler of  the Main app
	upldBtn = CreateWindowW(L"Button", L"Upload", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 751, 381, 133, 36, hwnd, (HMENU)OPEN_ENCRYPT_FILE_BUTTON, NULL, NULL);


}

void uploadFileEncrypterProc(HWND hwnd)
{
	//-------------------------------------------------------------------------------------------------------
	// This function take a file and copies it to the Data directory
	// @PARAM HWND window handler of  the Main app


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
						wcscat_s(saveTo, 100, L"text.txt");


						if (encrypter(pszFilePath, TEXT("crypt/CT.txt"), TEXT("crypt/keyOut.txt")))
							MessageBox(NULL, pszFilePath, L"File Enrypter", MB_OK);
						else
							MessageBox(NULL, L"Encryption Error", L"File Enrypter", MB_OK);


						//	//Call ReadFile with the buffer initialized above.
						//Copy the file to the Data Directory
						/*if(copyFileToDest(pszFilePath, (LPCWSTR)saveTo))
							MessageBox(NULL, pszFilePath, L"File Copy", MB_OK);
						else
							MessageBox(NULL, L"File Open Error", L"File Copy", MB_OK);*/

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

void listEncryptedFilesDir(HWND hwnd)
{
	//-------------------------------------------------------------------------------------------------------
	// This function lists the contents of the "crypt" directory 
	// @PARAM HWND window handler of  the Main app
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	BYTE names[] = { "0" };
	int y = 108;

	//_tprintf(TEXT("Target file is %s\n"), argv[1]);
	hFind = FindFirstFile(L"crypt/*", &FindFileData);
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
			CreateWindowW(L"Static", FindFileData.cFileName, WS_VISIBLE | WS_CHILD | SS_CENTER | SS_NOTIFY, 72, y, 200, 20, hwnd, NULL, NULL, NULL);
			y = y + 25;

		}

		FindClose(hFind);
		//MessageBox(NULL, (LPCWSTR)names[0], L"File Copy", MB_OK);

	}

}

bool saveEncrypted(HWND hwnd, LPWSTR fileName) {
	//-------------------------------------------------------------------------------------------------------
	// This function take a file and copies it to the Data directory
	// @PARAM HWND window handler of  the Main app
	// @PARAM LPWSTR pointer to the file name taht the user selects. 
	//Return - true if the function succeeds.


	//relative path to the encrypted dir
	wchar_t sourcePath[100] = L"crypt/";
	wcscat_s(sourcePath, 100, fileName); //append the file name selected



	//open destination to save and get the destination directory path
	OPENFILENAME ofn;

	
	PWSTR pszDestPath; //The destination path name selected will be stored here

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		IFileOpenDialog *pFileOpen;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
		DWORD dwOptions;
		if (SUCCEEDED(pFileOpen->GetOptions(&dwOptions)))
		{
			pFileOpen->SetOptions(dwOptions | FOS_PICKFOLDERS);
		}
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
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszDestPath);
					// Display the destination path name to the user.
					if (SUCCEEDED(hr))
					{						

						//append file name to destination path
						wchar_t destPath[100]= L"";

						wcscat_s(destPath, 100, pszDestPath);
						wcscat_s(destPath, 100, L"\\");
						wcscat_s(destPath, 100, fileName);
						//MessageBox(NULL, destPath, L"Success", MB_OK);

						if (CopyFile(sourcePath, destPath, TRUE)) 
						{
							MessageBox(NULL, destPath, L"Success", MB_OK);
						}
						else
						{
							MessageBox(NULL, L"Download Fail", L"Fail", MB_OK);
							return false;
						}

						CoTaskMemFree(pszDestPath);
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}
	return true;
}