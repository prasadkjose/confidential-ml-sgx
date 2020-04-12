#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include "ReadFile.h"

void writeFile(LPCTSTR path, PBYTE DataBuffer)
{
	//-------------------------------------------------------------------------
	// Function : Writes data from a buffer to the specified file. 
	// @PARAMS LPCTSTR pointer to File name to write to.
	// @PARAMS PBYTE Pointer to the buffer to read the data from.
	//-------------------------------------------------------------------------

	HANDLE hFile;
	//char DataBuffer[] = "This is some test data to write to the file.";
	DWORD dwBytesToWrite = BUFFERSIZE;
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

	_tprintf(TEXT("Writing %d bytes to %s.\n"), dwBytesToWrite, path);

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
			_tprintf(TEXT("Wrote %d bytes to %s successfully.\n"), dwBytesWritten, path);
		}
	}

	CloseHandle(hFile);

}

//void __cdecl _tmain()
//{
//	const LPCTSTR path = TEXT("TestOut.txt");
//	BYTE data[BUFFERSIZE] = { "Hello Write fu"};
//	writeFile(path, data);
//}
