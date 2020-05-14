#include "Enclave.h"



bool testEnclave() {
	sgx_enclave_id_t    eid;
	sgx_status_t ret = SGX_SUCCESS;
	sgx_launch_token_t token = { 0 };
	int updated = 0;
	char buffer[MAX_BUF_LEN] = "Hello World!";
	// Create the Enclave with above launch token.
	ret = sgx_create_enclave(ENCLAVE_FILE, SGX_DEBUG_FLAG, &token, &updated, &eid, NULL);
	if (ret != SGX_SUCCESS)
	{
		MessageBox(NULL, L"App: error, failed to create enclave.", L"Failure ", MB_OK);

	}
	// A bunch of Enclave calls (ECALL) will happen here. 
	foo(eid, buffer, MAX_BUF_LEN);
	MessageBoxA(NULL, buffer, "Success", MB_OK);


	// Destroy the enclave when all Enclave calls finished.
	if (SGX_SUCCESS != sgx_destroy_enclave(eid))
		return false;

	return true;
}
