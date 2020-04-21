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



BOOL encrypter(LPCTSTR, LPCTSTR, LPCTSTR);
