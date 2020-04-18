#include <windows.h>
#include <stdio.h>
#include <bcrypt.h>
#include "FileIOHandler.h"

#define NT_SUCCESS(Status)          (((NTSTATUS)(Status)) >= 0)

#define STATUS_UNSUCCESSFUL         ((NTSTATUS)0xC0000001L)

#define DATA_TO_ENCRYPT  "Test Data"


//const BYTE rgbPlaintext;
//{ 'P', 'A', 'S', 'S', 'W', 'O', 'R', 'D' };

static const BYTE rgbIV[] =
{
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};
static const BYTE rgbAES128Key[] =
{ 'P', 'A', 'S', 'S', 'W', 'O', 'R', 'D', 'P', 'A', 'S', 'S', 'W', 'O', 'R', 'D' };


//void PrintBytes(
//	IN BYTE     *pbPrintData,
//	IN DWORD    cbDataLen)
//{
//	DWORD dwCount = 0;
//
//	for (dwCount = 0; dwCount < cbDataLen; dwCount++)
//	{
//		printf("0x%02x, ", pbPrintData[dwCount]);
//
//		if (0 == (dwCount + 1) % 10) putchar('\n');
//	}
//
//}

void decrypter()
{
	//printf("%d", sizeof(rgbPlaintext));

	BYTE rgbPlaintext[7] = { 0 };
	readFile(TEXT("Text.txt"), rgbPlaintext, 7);
	printf("****PlainText data : \n");
	printf("%d", sizeof(rgbPlaintext));

	for (int i = 0; i < 10; i++)
		printf("%x \n", rgbPlaintext[i]);

	BCRYPT_ALG_HANDLE       hAesAlg = NULL;
	BCRYPT_KEY_HANDLE       hKey = NULL;
	NTSTATUS                status = STATUS_UNSUCCESSFUL;
	//Size in bytes of the buffers. 
	DWORD                   cbCipherText = 33,
		cbPlainText = 0,
		cbData = 0,
		cbKeyObject = 0,
		cbBlockLen = 0,
		cbBlob = 561;
	//Buffers
	PBYTE                   pbCipherText = NULL,
		pbPlainText = NULL,
		pbKeyObject = NULL,
		pbIV = NULL,
		pbBlob = NULL;

	//Read CipherText from File
	wprintf(L"*****Reading Cipher Text from file******************\n");

	// Allocate the buffer to Cipher Text the BLOB.
	PBYTE rgbCrypto = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbBlob);
	readFile(TEXT("TestOut.txt"), rgbCrypto, cbCipherText);
	for (int i = 0; i < cbCipherText +1; i++)
	{
		printf("\n byte: %d Data : %X\n",i, rgbCrypto[i]);
	}
	wprintf(L"*****Done Reading Cipher Text from file******************\n\n\n");


	//Read key from File
	// Allocate the buffer to hold the BLOB.
	pbBlob = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbBlob);
	//BYTE rgbKeyBlob[560] = { 0 };

	wprintf(L"*****Reading Key Blob from file******************\n");

	readFile(TEXT("TestOutKey.txt"), pbBlob, cbBlob);
	/*for (int i = 0; i < cbBlob; i++)
	{
		printf(" \n byte: %d Data : %x\n",i, pbBlob[i]);
	}*/
	wprintf(L"*****Done Reading Key Blob from file******************\n\n\n");     


	// Open an algorithm handle.
	if (!NT_SUCCESS(status = BCryptOpenAlgorithmProvider(
		&hAesAlg,
		BCRYPT_AES_ALGORITHM,
		NULL,
		0)))
	{
		wprintf(L"**** Error 0x%x returned by BCryptOpenAlgorithmProvider\n", status);
		goto Cleanup;
	}

	// Calculate the size of the buffer to hold the KeyObject.
	if (!NT_SUCCESS(status = BCryptGetProperty(
		hAesAlg,
		BCRYPT_OBJECT_LENGTH,
		(PBYTE)&cbKeyObject,
		sizeof(DWORD),
		&cbData,
		0)))
	{
		wprintf(L"**** Error 0x%x returned by BCryptGetProperty\n", status);
		goto Cleanup;
	}

	// Allocate the key object on the heap.
	pbKeyObject = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbKeyObject);
	if (NULL == pbKeyObject)
	{
		wprintf(L"**** memory allocation failed\n");
		goto Cleanup;
	}

	// Calculate the block length for the IV.
	if (!NT_SUCCESS(status = BCryptGetProperty(
		hAesAlg,
		BCRYPT_BLOCK_LENGTH,
		(PBYTE)&cbBlockLen,
		sizeof(DWORD),
		&cbData,
		0)))
	{
		wprintf(L"**** Error 0x%x returned by BCryptGetProperty\n", status);
		goto Cleanup;
	}

	// Determine whether the cbBlockLen is not longer than the IV length.
	if (cbBlockLen > sizeof(rgbIV))
	{
		wprintf(L"**** block length is longer than the provided IV length\n");
		goto Cleanup;
	}

	// Allocate a buffer for the IV. The buffer is consumed during the 
	// encrypt/decrypt process.
	pbIV = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbBlockLen);
	if (NULL == pbIV)
	{
		wprintf(L"**** memory allocation failed\n");
		goto Cleanup;
	}

	memcpy(pbIV, rgbIV, cbBlockLen);

	if (!NT_SUCCESS(status = BCryptSetProperty(
		hAesAlg,
		BCRYPT_CHAINING_MODE,
		(PBYTE)BCRYPT_CHAIN_MODE_CBC,
		sizeof(BCRYPT_CHAIN_MODE_CBC),
		0)))
	{
		wprintf(L"**** Error 0x%x returned by BCryptSetProperty\n", status);
		goto Cleanup;
	}



	hKey = 0;

	if (pbPlainText)
	{
		HeapFree(GetProcessHeap(), 0, pbPlainText);
	}

	pbPlainText = NULL;

	// We can reuse the key object.
	memset(pbKeyObject, 0, cbKeyObject);


	// Reinitialize the IV because encryption would have modified it.
	memcpy(pbIV, rgbIV, cbBlockLen);

	
	if (NULL == pbBlob)
	{
		wprintf(L"**** memory allocation failed\n");
		goto Cleanup;
	}

	if (!NT_SUCCESS(status = BCryptImportKey(
		hAesAlg,
		NULL,
		BCRYPT_OPAQUE_KEY_BLOB,
		&hKey,
		pbKeyObject,
		cbKeyObject,
		pbBlob,
		cbBlob,
		0)))
	{
		wprintf(L"**** Error 0x%x returned by BCryptImportSymmetricKey\n", status);
		goto Cleanup;
	}

	

	//
	// Get the output buffer size.
	//
	if (!NT_SUCCESS(status = BCryptDecrypt(
		hKey,
		pbCipherText,
		cbCipherText,
		NULL,
		pbIV,
		cbBlockLen,
		NULL,
		0,
		&cbPlainText,
		BCRYPT_BLOCK_PADDING)))
	{
		wprintf(L"**** Error 0x%x returned by BCryptDecrypt\n", status);

		goto Cleanup;
	}
	else {
		printf("Plaintext Size : %d\n", cbPlainText);

	}

	pbPlainText = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbPlainText);
	if (NULL == pbPlainText)
	{
		wprintf(L"**** memory allocation failed\n");
		goto Cleanup;
	}

	if (!NT_SUCCESS(status = BCryptDecrypt(
		hKey,
		pbCipherText,
		cbCipherText,
		NULL,
		pbIV,
		cbBlockLen,
		pbPlainText,
		cbPlainText,
		&cbPlainText,
		BCRYPT_BLOCK_PADDING)))
	{
		wprintf(L"**** Error 0x%x returned by BCryptDecrypt\n", status);
		goto Cleanup;
	}
	else {
		wprintf(L"Decrypted Data :\n");
		for (int i = 0; i < cbPlainText; i++)
			printf("%x \n", pbPlainText[i]);
	}

	if (0 != memcmp(pbPlainText, (PBYTE)rgbPlaintext, sizeof(rgbPlaintext)))
	{
		wprintf(L"Expected decrypted text comparison failed.\n");
		goto Cleanup;
	}

	wprintf(L"Success!\n");


Cleanup:

	if (hAesAlg)
	{
		BCryptCloseAlgorithmProvider(hAesAlg, 0);
	}

	if (hKey)
	{
		BCryptDestroyKey(hKey);
	}

	if (pbCipherText)
	{
		HeapFree(GetProcessHeap(), 0, pbCipherText);
	}

	if (pbPlainText)
	{
		HeapFree(GetProcessHeap(), 0, pbPlainText);
	}

	if (pbKeyObject)
	{
		HeapFree(GetProcessHeap(), 0, pbKeyObject);
	}

	if (pbIV)
	{
		HeapFree(GetProcessHeap(), 0, pbIV);
	}

}

void __cdecl wmain(int argc, __in_ecount(argc) LPWSTR *wargv)
{

	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(wargv);
	decrypter();
}