
#pragma once
#include <windows.h>
#include "ShObjIdl_core.h"
#include <fcntl.h>
#include <process.h>
#include <io.h>

// Defines start with 20*. Example 200, 201....
#define OPEN_TRAIN_FILE_BUTTON 200
#define TRAIN_BUTTON 201
#define ON_STD_OUT_CALL 202
#define APPEND_CONSOLE 203

//static HANDLE ioEvent; //an event for print statement 
//
// struct ConsoleMessage
//{
//	LPCWSTR msg;
//
//	~ConsoleMessage()
//	{
//		delete msg;
//	}
//};



void uploadTrainFileProc(HWND);
void addTrainFileControls(HWND);
void listTrainFilesDir(HWND);

void trainBtnClick();

BOOL copyFileToDest(PWSTR, LPCWSTR);

HWND addConsoleControl(HWND hwnd);
void printToConsoleControl(HWND, LPWSTR);
