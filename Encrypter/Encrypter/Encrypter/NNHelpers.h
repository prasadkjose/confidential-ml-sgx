
#pragma once
#include <windows.h>
#include "ShObjIdl_core.h"


// Defines start with 20*. Example 200, 201....
#define OPEN_TRAIN_FILE_BUTTON 200
#define TRAIN_BUTTON 201

void uploadTrainFileProc(HWND);
void addTrainFileUploadControls(HWND);
void listTrainFilesDir(HWND);

void trainBtnClick();

BOOL copyFileToDest(PWSTR, LPCWSTR);