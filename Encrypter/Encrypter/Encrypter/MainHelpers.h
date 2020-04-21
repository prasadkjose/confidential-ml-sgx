#include <windows.h>
#include "ShObjIdl_core.h"

#define OPEN_FILE_BUTTON 1
#define LOGIN_NEXT 2
#define TITLE 3


void AddControls(HWND);
void drawRect(HWND, HDC);
void addLoginContols(HWND);

void removeLoginContols(HWND);
void loadMainApp(HWND);


//Client Dashboard Helper functions
void uploadFileProc(HWND);
void addFileUploadControls(HWND);
void listFilesDir(HWND, TCHAR);
BOOL copyFileToDest(PWSTR, LPCWSTR) ;

