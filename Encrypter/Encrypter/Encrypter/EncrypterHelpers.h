#pragma once
#include <windows.h>
#include "ShObjIdl_core.h"

// Defines start with 10*. Example 100, 101....
#define OPEN_ENCRYPT_FILE_BUTTON 100

//Client Encrypter Dashboard Helper functions
void uploadFileEncrypterProc(HWND);
void addFileEncrypterUploadControls(HWND);
void listEncryptedFilesDir(HWND);
