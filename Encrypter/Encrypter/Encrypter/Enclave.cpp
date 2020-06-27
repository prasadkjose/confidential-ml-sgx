#include "Enclave.h"


sgx_enclave_id_t global_eid = 0;


bool testEnclave() {
	//sgx_enclave_id_t    eid;
	sgx_status_t ret = SGX_SUCCESS;
	sgx_launch_token_t token = { 0 };
	int updated = 0;
	BYTE buffer[MAX_BUF_LEN] = "Hello!";
	// Create the Enclave with above launch token.
	ret = sgx_create_enclave(ENCLAVE_FILE, SGX_DEBUG_FLAG, &token, &updated, &global_eid, NULL);
	if (ret != SGX_SUCCESS)
	{
		MessageBox(NULL, L"App: error, failed to create enclave.", L"Failure ", MB_OK);

	}
	// A bunch of Enclave calls (ECALL) will happen here. 
	int ptr;

	sgx_status_t status = generate_random_number(global_eid, &ptr, (char *)buffer, MAX_BUF_LEN);
	MessageBoxA(NULL, (char *)buffer, "Buffer Data  ", MB_OK);

	if (status != SGX_SUCCESS)
	{
		MessageBox(NULL, L"Random Number error", L"Failure ", MB_OK);

	}
	else {
		char c[MAX_BUF_LEN] = "";
		//sprintf_s(c, MAX_BUF_LEN, "%s", buffer);
	}

	// Seal the random number
	size_t sealed_size = sizeof(sgx_sealed_data_t) + sizeof(buffer);
	char* sealed_data = (char*)malloc(sealed_size);

	sgx_status_t ecall_status;
	status = seal(global_eid, &ecall_status,
		(uint8_t*)&buffer, sizeof(buffer),
		(sgx_sealed_data_t*)sealed_data, sealed_size);
	if (status != SGX_SUCCESS)
	{
		MessageBox(NULL, L"Seal Error error", L"Failure ", MB_OK);

	}
	else
	{
		char c[MAX_BUF_LEN] ="";
		sprintf_s(c, MAX_BUF_LEN, "%s", sealed_data);
		MessageBoxA(NULL, c, "Sealed Data ", MB_OK);

	}


	
	char unsealed[MAX_BUF_LEN]="";
	status = unseal(global_eid, &ecall_status,
		(sgx_sealed_data_t*)sealed_data, sealed_size,
		(uint8_t*)&unsealed, sizeof(unsealed));
	if (status != SGX_SUCCESS)
	{
		MessageBox(NULL, L"Unseal error", L"Failure ", MB_OK);

	}
	else {
		char c[MAX_BUF_LEN] = "";
		sprintf_s(c, MAX_BUF_LEN, "%s", unsealed);
		MessageBoxA(NULL, c, "Unsealed Data ", MB_OK);
	}
	

	MessageBoxA(NULL, (char *)buffer, "Success", MB_OK);


	// Destroy the enclave when all Enclave calls finished.
	if (SGX_SUCCESS != sgx_destroy_enclave(global_eid))
		return false;

	return true;
}

bool sealFile(LPCTSTR PlaintextPath, DWORD cbPText, LPCTSTR SealedPath)
{
	/* --------------------------------------------------------------------
	Seals a File Function
	@PARAM LPCTSTR PlaintextPath
	@PARAM DWORD cbPText
	@PARAM LPCTSTR SealedPath
	-----------------------------------------------------------------------
	*/
	sgx_status_t ret = SGX_SUCCESS;
	sgx_status_t status;
	sgx_launch_token_t token = { 0 };
	int updated = 0;

	PBYTE rgbPlaintext= NULL,
		pbPlaintext = NULL,
		pbSealedText = NULL;

	DWORD cbPlaintext = cbPText,
		cbSealedText = 0;

	// Create the Enclave with above launch token.
	ret = sgx_create_enclave(ENCLAVE_FILE, SGX_DEBUG_FLAG, &token, &updated, &global_eid, NULL);
	if (ret != SGX_SUCCESS)
	{
		MessageBox(NULL, L"App: error, failed to create enclave.", L"Failure ", MB_OK);
		return false;
	}
	//Read File into buffer
	rgbPlaintext = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbPlaintext);
	pbPlaintext = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbPlaintext);
	readFile(PlaintextPath, rgbPlaintext, cbPlaintext);

	memcpy(pbPlaintext, rgbPlaintext, cbPlaintext);

	// Seal the Buffer
	size_t sealed_size = sizeof(sgx_sealed_data_t) + cbPlaintext;
	char* sealed_data = (char*)malloc(sealed_size);
	pbSealedText = (PBYTE)HeapAlloc(GetProcessHeap(), 0, sealed_size);


	sgx_status_t ecall_status;
	status = seal(global_eid, &ecall_status,
		(uint8_t*)pbPlaintext, (uint8_t)cbPlaintext,
		(sgx_sealed_data_t*)pbSealedText, sealed_size);
	if (status != SGX_SUCCESS)
	{
		MessageBox(NULL, L"Seal Error error", L"Failure ", MB_OK);
		return false;
	}
	else
	{
		char c[MAX_BUF_LEN] = "";
		//sprintf_s(c, MAX_BUF_LEN, "%s", sealed_data);
		//MessageBoxA(NULL, "Seal success", "Sealed Data Success ", MB_OK);

	}

	//write sealed data to file
	writeFile(SealedPath, pbSealedText, sealed_size);
	HeapFree(GetProcessHeap(), 0, pbPlaintext);
	HeapFree(GetProcessHeap(), 0, pbSealedText);
	HeapFree(GetProcessHeap(), 0, rgbPlaintext);

	return true;
}

bool unsealFile(LPCTSTR SealedPath, DWORD cbsealed, LPCTSTR PlaintextPath)
{
	/* --------------------------------------------------------------------
	Un-Seals a File Function
	@PARAM LPCTSTR SealedPath
	@PARAM DWORD cbsealed
	@PARAM LPCTSTR PlaintextPath
	-----------------------------------------------------------------------
	*/
	sgx_status_t ret = SGX_SUCCESS;
	sgx_status_t status = SGX_SUCCESS;
	sgx_launch_token_t token = { 0 };
	int updated = 0;
	PBYTE rgbSealedtext = NULL,
		pbSealedText = NULL;

	DWORD cbPlaintext = 0,
		cbSealedText = cbsealed;
	BYTE pbPlaintext[100]="";

	// Create the Enclave with above launch token.
	ret = sgx_create_enclave(ENCLAVE_FILE, SGX_DEBUG_FLAG, &token, &updated, &global_eid, NULL);
	if (ret != SGX_SUCCESS)
	{
		MessageBox(NULL, L"App: error, failed to create enclave.", L"Failure ", MB_OK);
		return false;
	}
	
	// Read sealed file to buffer
	rgbSealedtext = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbSealedText);
	pbSealedText = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbSealedText);

	readFile(SealedPath, rgbSealedtext, cbSealedText);

	memcpy(pbSealedText, rgbSealedtext, cbSealedText);

	//unseal File
	sgx_status_t ecall_status;

	status = unsealDataSize(global_eid, (int *)&cbPlaintext, (sgx_sealed_data_t*)pbSealedText, cbSealedText);
	if (status != SGX_SUCCESS)
	{
		MessageBox(NULL, L"Unseal error", L"Failure ", MB_OK);
		return false;

	}
	char unsealed[MAX_BUF_LEN] = "";

	status = unseal(global_eid, &ecall_status,
		(sgx_sealed_data_t*)pbSealedText, cbSealedText,
		(uint8_t*)pbPlaintext, cbPlaintext); //TODO calc PT Buffer size

	if (ecall_status != SGX_SUCCESS)
	{
		MessageBox(NULL, L"Unseal error: The message may have been tampered with. ", L"Failure ", MB_OK);
		return false;

	}
	
	else {
		char c[MAX_BUF_LEN] = "";
		/*sprintf_s(c, MAX_BUF_LEN, "%s", unsealed);
		MessageBoxA(NULL, c, "Unsealed Data ", MB_OK);*/
	}



	//Write to File
	writeFile(PlaintextPath, pbPlaintext, 32);
	return true;
}

/* Global EID shared by multiple threads */

// OCall implementations
void ocall_print(const char* str) {
	MessageBoxA(NULL, str, "Success OCall", MB_OK);

	
}
