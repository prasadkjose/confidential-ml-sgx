#include <windows.h>
#include "ShObjIdl_core.h"
#include "EncrypterHelpers.h"

#define LOGIN_NEXT 1
#define TITLE 2
#define NO_BG_WHITE_TXT 3
#define NO_BG_BLACK_TXT 4
#define APPEND_CONSOLE 5
#define WHITE_BG_GREEN_TXT 6


void drawRect(HWND, HDC);
void addLoginContols(HWND);

void removeLoginContols(HWND);
void loadMainApp(HWND);

HWND addConsoleControl(HWND hwnd);
void printToConsoleControl(HWND, LPWSTR);

