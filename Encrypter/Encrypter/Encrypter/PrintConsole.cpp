#include "PrintConsole.h"
#include <cwchar>
void __printf(LPWSTR text)
{
	HWND hwnd = FindWindowA("EncrypterApplication", "Encrypter Application");
	wchar_t buff[500] = L"";
	wcscat_s(buff, 500, text);
	wcscat_s(buff, 500, L"\r\n");

	printToConsoleControl(hwnd, (LPWSTR)buff);

	//delete[] txt;


}


