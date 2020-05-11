#include "EncrypterHelpers.h"
#include "EncrypterCLI/EncrypterCLI.h"
#include "EncrypterCLI/FileIOHandler.h"
#include "DRNG.h"
#include <Lmcons.h>
#include<windows.h>
#include <wincred.h>	



//-------------------------------------------------------------------------------------------------------
//Client Dashboard Helper functions

HWND upldBtn;
using namespace std;

void addFileEncrypterUploadControls(HWND hwnd)
{
	//-------------------------------------------------------------------------------------------------------
	// This function creates encrypter Upload controls and other UI elements
	// @PARAM HWND window handler of  the Main app

	CreateWindowW(L"static", L"Encrypted Files", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE, 148, 150, 205, 40, hwnd,(HMENU)NO_BG_COLOR, NULL, NULL);
	upldBtn = CreateWindowW(L"Button", L"Upload", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 751, 381, 133, 36, hwnd, (HMENU)OPEN_ENCRYPT_FILE_BUTTON, NULL, NULL);


}

void uploadFileEncrypterProc(HWND hwnd)
{
	//-------------------------------------------------------------------------------------------------------
	// This function take a file and Encrypts it to the crypto directory along with the KeyBlob
	// @PARAM HWND window handler of  the Main app

	wchar_t uhash[300] = L"";
	DWORD cbUhash = 0;
	authenticateWinCred(5, uhash, cbUhash);
	//MessageBoxW(NULL, uhash, L"Display Pass", MB_OK);

	

	OPENFILENAME ofn;

	Crypto encrypt;


	//The file name selected will be stored here
	char file_name[100];
	DWORD file_size = 10;
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
					PWSTR pszDirPath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);


					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{
						// concatenate the string with the Data directory path
						wchar_t saveTo[100] = L"Data/";
						wcscat_s(saveTo, 100, L"text.txt");
						
					//Get the size of the file to be encrypted. 
						if (fileSize(pszFilePath, &file_size))
						{

							//generate 32 bit Random File encryption key. 
							BYTE rgbAES128Key[32] = {};
							DRNG rand;
							rand.generate32bitRand((UINT32 *)rgbAES128Key, 32);
							
							//generate File Name for encrypted file and key blob
							wchar_t in[] = L"Jane Doe"; // temp Username or hash
							/*wchar_t username[UNLEN + 1];
							DWORD username_len = UNLEN + 1;
							GetUserName(username, &username_len);*/

							BYTE hash[32] = { 0 };
							encrypt.generateHash((PBYTE)uhash, cbUhash, hash);

							//Debug Statement for Hash
							wchar_t UTFhash[100] = L"";
							memcpy(UTFhash, hash, 32);
							//writeFile(TEXT("crypt/a.txt"), (PBYTE)UTFhash, 32);

							//Generate File Names
							wchar_t pbCTout[100] =L"";							
							generateFileName((LPWSTR)hash, pbCTout, CIPHERTEXT);							
							wchar_t pbCTPath[100] = L"crypt/";
							wcscat_s(pbCTPath, 100, pbCTout);
							//writeFile(TEXT("crypt/b.txt"), (PBYTE)pbCTout, 32);

							
							
							wchar_t pbBLOBout[100] = L"";
							generateFileName((LPWSTR)hash, pbBLOBout, BLOB);							
							wchar_t pbBLOBPath[100] = L".tmp/.filekeys/";
							wcscat_s(pbBLOBPath, 100, pbBLOBout);
							//wcscat_s(pbBLOBPath, 100, L".BLOB");

							
							//TODO: Get directory path of pszFilePath to save the encrypted file
							
							
							//Generate AES Key and Save the blob 
							char p[100] = "";
							memcpy(p, (LPCTSTR)pbBLOBPath, 100);

							if (encrypt.generateAESKey(rgbAES128Key, pbBLOBPath))
								MessageBox(NULL, L"Key Generrated", L"File Enrypter", MB_OK);
							else
								MessageBox(NULL, L"Key Generation Error", L"File Enrypter", MB_OK); 

							//Encrypt the file 
							
							/*if (encrypt.encrypt(pszFilePath, file_size, pbBLOBPath, pbCTPath))
								MessageBox(NULL, pszFilePath, L"File Enrypter", MB_OK);
							else
								MessageBox(NULL, L"Encryption Error", L"File Enrypter", MB_OK);*/
							

							/*wchar_t pbPTTout[100] = L"";
							generateFileName((LPWSTR)hash, pbPTTout, PLAINTEXT);
							wchar_t pbPTTPath[100] = L"crypt/";
							wcscat_s(pbPTTPath, 100, pbPTTout);

							if (encrypt.decrypt(pszFilePath, file_size, pbBLOBPath, pbPTTPath))
								MessageBoxA(NULL, (LPSTR)pbPTTPath, "File Enrypter", MB_OK);
							else
								MessageBoxA(NULL, "Decrypt Failure", "File Enrypter", MB_OK);*/


							//TODO 2 : Encrypt File Key BLOB


							//Delete the File Key Blob
							/*if (remove((LPSTR)pbBLOBPath))
								MessageBox(NULL, L"KeyBLOB Deleted", L"File Enrypter", MB_OK);
							else
								MessageBox(NULL, L"ERROR : Could not delete KeyBLOB", L"File Enrypter", MB_OK);*/

							//TODO 4 : CAB Compression Encrypted File Key Blob and Encrypted File


						}
						else 
							MessageBox(NULL, L"Get File Size Error", L"File Enrypter", MB_OK);


						//	//Call ReadFile with the buffer initialized above

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
	int y = 200;

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
			CreateWindowW(L"Static", FindFileData.cFileName, WS_VISIBLE | WS_CHILD | SS_CENTER | SS_NOTIFY | SS_CENTERIMAGE, 126, y, 250, 30, hwnd, (HMENU)SAVE_ENCRYPTED, NULL, NULL);
			y = y + 40;

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