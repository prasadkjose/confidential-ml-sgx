#include <tchar.h>
#include "sgx_urts.h"
#include "CryptoEnclave_u.h"
#define ENCLAVE_FILE _T("CryptoEnclave.signed.dll")
#define MAX_BUF_LEN 100



bool testEnclave();