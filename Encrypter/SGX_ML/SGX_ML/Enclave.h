#include <tchar.h>
#include <stdio.h>
#include <iostream>
#include "sgx_urts.h"
#include "sgx_utils.h"

#include "CryptoEnclave_u.h"
#include <Windows.h>


#define ENCLAVE_FILE _T("CryptoEnclave.signed.dll")
#define MAX_BUF_LEN 100



bool testEnclave();
bool sealFile(LPCTSTR, DWORD, LPCTSTR);
bool unsealFile(LPCTSTR, DWORD, LPCTSTR);
//int uen_mainNN();
void ocall_print(const char*);