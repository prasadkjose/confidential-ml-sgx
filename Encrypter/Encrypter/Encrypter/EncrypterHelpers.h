#pragma once
#include <windows.h>
#include "ShObjIdl_core.h"
#include "PrintConsole.h"
// Defines start with 10*. Example 100, 101....
#define OPEN_ENCRYPT_FILE_BUTTON 100
#define SAVE_ENCRYPTED 101
#define NO_BG_COLOR 102

//Client Encrypter Dashboard Helper functions
void uploadFileEncrypterProc(HWND, LPWSTR, DWORD);
void addFileEncrypterUploadControls(HWND);
void listEncryptedFilesDir(HWND);
bool saveEncrypted(HWND, LPWSTR);
bool authenticateWinCred(int, LPWSTR, DWORD);