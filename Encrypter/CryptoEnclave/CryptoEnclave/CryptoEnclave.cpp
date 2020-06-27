#include "CryptoEnclave_t.h"
#include "sgx_trts.h"

#define ENCLAVE_FILE _T("CryptoEnclave.signed.dll")

#include <string.h>
int generate_random_number(char *buf, size_t len) {
	const char *secret = "Hello Enclave!"; 
	if (len > strlen(secret)) 
	{ 
		memcpy(buf, secret, strlen(secret) + 1);
	}
	ocall_print("Processing random number generation...");

	return 1;
}