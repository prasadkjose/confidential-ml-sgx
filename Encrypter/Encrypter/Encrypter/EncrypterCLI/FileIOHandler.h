#pragma once
//----------------------------------------------------------
// This seems to be the standard solution for preventing
// spurious "macro redefinition" warnings when you have
// to include both windows.h and ntstatus.h
#define WIN32_NO_STATUS
#include <windows.h>
#undef WIN32_NO_STATUS
#include <ntstatus.h>
//----------------------------------------------------------

#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <filesystem>


void DisplayError(LPCTSTR lpszFunction);

VOID CALLBACK FileIOCompletionRoutine(
	__in  DWORD dwErrorCode,
	__in  DWORD dwNumberOfBytesTransfered,
	__in  LPOVERLAPPED lpOverlapped
);

void readFile(LPCTSTR, PBYTE, DWORD);
void writeFile(LPCTSTR, PBYTE, DWORD);
bool fileSize(LPCTSTR, DWORD*);
