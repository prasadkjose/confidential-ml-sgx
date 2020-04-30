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
#include <bcrypt.h> 
#include "FileIOHandler.h"
#include <stdio.h>
#define NT_SUCCESS(Status)          (((NTSTATUS)(Status)) >= 0)

#define STATUS_UNSUCCESSFUL         ((NTSTATUS)0xC0000001L)

//const BYTE rgbPlaintext;
//{ 'P', 'A', 'S', 'S', 'W', 'O', 'R', 'D' };


class Crypto {
public:
	Crypto();
	//~Crypto();

	bool generateAESKey(PBYTE, LPCTSTR);
	bool encrypt(LPCTSTR, DWORD, LPCTSTR, LPCTSTR);
	bool decrypt(LPCTSTR, DWORD, LPCTSTR, LPCTSTR);

};