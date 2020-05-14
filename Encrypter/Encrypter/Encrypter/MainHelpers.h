#include <windows.h>
#include "ShObjIdl_core.h"
#include "EncrypterHelpers.h"
#include "NNHelpers.h"

#define LOGIN_NEXT 1
#define TITLE 2
#define NO_BG_WHITE_TXT 3
#define NO_BG_BLACK_TXT 4


void AddControls(HWND);
void drawRect(HWND, HDC);
void addLoginContols(HWND);

void removeLoginContols(HWND);
void loadMainApp(HWND);



