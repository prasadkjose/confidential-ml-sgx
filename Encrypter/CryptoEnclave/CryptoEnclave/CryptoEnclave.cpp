#include "CryptoEnclave_t.h"

#include "sgx_trts.h"
#define ENCLAVE_FILE _T("CryptoEnclave.signed.dll")

#include <string.h>
void foo(char *buf, size_t len)
{ 
	const char *secret = "Hello Enclave Test!";
	if (len > strlen(secret))
	{ 
		memcpy(buf, secret, strlen(secret) + 1);
	}
}
