
#pragma once

#include <windows.h>
#include <stdio.h>
#include <strsafe.h>
#include "FileIOHandler.h"
#include "../PrintConsole.h"
#include "EncrypterCLI.h"

namespace fs = std::filesystem;


DWORD g_BytesTransferred = 0;

VOID CALLBACK FileIOCompletionRoutine(
	__in  DWORD dwErrorCode,
	__in  DWORD dwNumberOfBytesTransfered,
	__in  LPOVERLAPPED lpOverlapped)
{
	_tprintf(TEXT("Error code:\t%x\n"), dwErrorCode);
	_tprintf(TEXT("FileIOHandler : Number of bytes:\t%x\n"), dwNumberOfBytesTransfered);
	g_BytesTransferred = dwNumberOfBytesTransfered;
}

void DisplayError(LPCTSTR  lpszFunction)
// Routine Description:
// Retrieve and output the system error message for the last-error code
{
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL);

	lpDisplayBuf =
		(LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf)
			+ lstrlen((LPCTSTR)lpszFunction)
			+ 40) // account for format string
			* sizeof(TCHAR));

	if (FAILED(StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error code %d as follows:\n%s"),
		lpszFunction,
		dw,
		lpMsgBuf)))
	{
		printf("FATAL ERROR: Unable to output error code.\n");
	}

	_tprintf(TEXT("ERROR: %s\n"), (LPCTSTR)lpDisplayBuf);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}

void readFile(LPCTSTR path, PBYTE ReadBuf, DWORD cbBuffer)
{
	//-------------------------------------------------------------------------
	// Function : Reads data from a file to a buffer.
	// NOTE : Buffer Size should be 1 more than the readData size. 
	// @PARAMS LPCTSTR pointer to File name to read the data from .
	// @PARAMS PBYTE Pointer to the buffer to write the data to.
	// @PARAMS DWORD size of buffer to write the data to.
	//-------------------------------------------------------------------------

	HANDLE hFile;
	DWORD  dwBytesRead = 0;
	BYTE* ReadBuffer = new BYTE[cbBuffer];
	//ReadBuffer = { 0 };
	OVERLAPPED ol = { 0 };
	//const TCHAR *path = TEXT("Text.txt");
	printf("\n");
	//if (argc != 2)
	//{
	//	printf("Usage Error: Incorrect number of arguments\n\n");
	//	_tprintf(TEXT("Usage:\n\t%s <text_file_name>\n"), argv[0]);
	//	return;
	//}

	hFile = CreateFile(path,               // file to open
		GENERIC_READ,          // open for reading
		FILE_SHARE_READ,       // share for reading
		NULL,                  // default security
		OPEN_EXISTING,         // existing file only
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // normal file
		NULL);                 // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		DisplayError(TEXT("CreateFile"));
		_tprintf(TEXT("Terminal failure: unable to open file \"%s\" for read.\n"), path);
		return;
	}

	// Read one character less than the buffer size to save room for
	// the terminating NULL character. 

	if (FALSE == ReadFileEx(hFile, ReadBuffer, cbBuffer, &ol, FileIOCompletionRoutine))
	{
		DisplayError(TEXT("ReadFile"));
		printf("Terminal failure: Unable to read from file.\n GetLastError=%08x\n", GetLastError());
		CloseHandle(hFile);
		return;
	}
	SleepEx(5000, TRUE);
	dwBytesRead = g_BytesTransferred;
	// This is the section of code that assumes the file is ANSI text. 
	// Modify this block for other data types if needed.

	if ( dwBytesRead <= cbBuffer)
	{
		//ReadBuffer[dwBytesRead] = '\0'; // NULL character

		_tprintf(TEXT("FileIOHandler : Data read from %s (%d bytes): \n"), path, dwBytesRead);
		printf("%x\n", ReadBuffer);

		//_tcscpy_s(ReadBuf, cbBuffer, ReadBuffer);
		memcpy(ReadBuf, ReadBuffer, cbBuffer);
	}
	else if (dwBytesRead == 0)
	{
		_tprintf(TEXT("No data read from file %s\n"), path);
	}
	else
	{
		printf("\n ** Unexpected value for dwBytesRead ** \n");
	}

	// It is always good practice to close the open file handles even though
	// the app will exit here and clean up open handles anyway.

	CloseHandle(hFile);
}

void writeFile(LPCTSTR path, PBYTE DataBuffer, DWORD cbBuffer)
{
	//-------------------------------------------------------------------------
	// Function : Writes data from a buffer to the specified file. 
	// @PARAMS LPCTSTR pointer to File name to write to.
	// @PARAMS PBYTE Pointer to the buffer to read the data from.
	// @PARAMS DWORD size of the buffer
	//-------------------------------------------------------------------------

	HANDLE hFile;
	//char DataBuffer[] = "This is some test data to write to the file.";
	DWORD dwBytesToWrite = cbBuffer;
	DWORD dwBytesWritten = 0;
	BOOL bErrorFlag = FALSE;
	//	const TCHAR *path = TEXT("TestOut.a");


	printf("\n");
	//if (argc != 2)
	//{
	//	printf("Usage Error:\tIncorrect number of arguments\n\n");
	//	_tprintf(TEXT("%s <file_name>\n"), argv[0]);
	//	return;
	//}

	hFile = CreateFile(path,                // name of the write
		GENERIC_WRITE,          // open for writing
		0,                      // do not share
		NULL,                   // default security
		CREATE_NEW,             // create new file only
		FILE_ATTRIBUTE_NORMAL,  // normal file
		NULL);                  // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		DisplayError(TEXT("CreateFile"));
		_tprintf(TEXT("Terminal failure: Unable to open file \"%s\" for write.\n"), path);
		return;
	}

	_tprintf(TEXT("FileIOHandler : Writing %d bytes to %s.\n"), dwBytesToWrite, path);

	bErrorFlag = WriteFile(
		hFile,           // open file handle
		DataBuffer,      // start of data to write
		dwBytesToWrite,  // number of bytes to write
		&dwBytesWritten, // number of bytes that were written
		NULL);            // no overlapped structure

	if (FALSE == bErrorFlag)
	{
		DisplayError(TEXT("WriteFile"));
		printf("Terminal failure: Unable to write to file.\n");
	}
	else
	{
		if (dwBytesWritten != dwBytesToWrite)
		{
			// This is an error because a synchronous write that results in
			// success (WriteFile returns TRUE) should write all data as
			// requested. This would not necessarily be the case for
			// asynchronous writes.
			printf("Error: dwBytesWritten != dwBytesToWrite\n");
		}
		else
		{
			_tprintf(TEXT("FileIOHandler : Wrote %d bytes to %s successfully.\n"), dwBytesWritten, path);
		}
	}

	CloseHandle(hFile);

}

bool fileSize(LPCTSTR path, DWORD* size)
{
	if (fs::file_size(path))
	{
		*size = fs::file_size(path);
		/*char szTest[10];
		sprintf_s(szTest, "%d", fs::file_size(path));
		MessageBoxA(NULL, szTest, "File Enrypter", MB_OK);*/


		return true;
	}
	else
		return false;

}

bool generateFileName(LPSTR pbInput, LPSTR pbfileName, int flag) {

	Crypto hash;
	PBYTE hashOut = (PBYTE)HeapAlloc(GetProcessHeap(), 0, 32);
	hash.generateHash((PBYTE)pbInput, sizeof(pbInput), hashOut);
	//PBYTE a = (PBYTE)HeapAlloc(GetProcessHeap(), 0, 100);
	//memcpy(a, hashOut, 32);


	switch (flag)
	{
		 
	case PLAINTEXT:
	{
		char a[] = ".PT" ;
		memcpy(hashOut+32, a, sizeof(a));
		/*sprintf_s(format, 10, "%s", ".PT");
		strcat_s(a, 50, format);*/
		break;
	}
	case CIPHERTEXT:
	{
		char a[] = ".CT";
		memcpy(hashOut + 32, a, sizeof(a));
		break;

	}
	case BLOB:
	{
		char a[] = ".BLOB";
		memcpy(hashOut + 32, a, sizeof(a));
		break;

	}
	}

	memcpy(pbfileName, hashOut, 100);


	
	HeapFree(GetProcessHeap(), 0, hashOut);

	return true;
}
//void __cdecl _tmain()
//{
//	//-------------------------------------------------------------------------
//	// Main Function to test and debug File IO Functions
//	//-------------------------------------------------------------------------
//
//	//Paths to read and write files
//	const LPCTSTR pathWrite = TEXT("TestOut.txt");
//	const LPCTSTR pathRead = TEXT("Text.txt");
//
//	BYTE data[DWORD(8)] = { 'P', 'A', 'S', 'S', 'W', 'O', 'R', 'D' };
//	//Function call Write to file
//	//writeFile(pathWrite, data, 8);
//
//	//initialize a buffer to copy the file contents to.
//	DWORD cbreadBuff = 6;
//	BYTE* out = new BYTE[cbreadBuff];
//	//Call ReadFile with the buffer initialized above.
//	readFile(pathRead, out, cbreadBuff);
//
//	//Print the Hex of the file in the data
//	for (int i = 0; i < cbreadBuff; i++)
//	{
//		printf("%x\n", out[i]);
//
//	}
//	// Print the data 
//	printf("%s", out);
//}
