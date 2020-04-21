// Headers 
#include <windows.h>
#include <stdio.h>
#include <bcrypt.h>
#include "EncrypterCLI.h"
#define NT_SUCCESS(Status)          (((NTSTATUS)(Status)) >= 0)

#define STATUS_UNSUCCESSFUL         ((NTSTATUS)0xC0000001L)


//const BYTE rgbPlaintext;
//{ 'P', 'A', 'S', 'S', 'W', 'O', 'R', 'D' };


static const BYTE rgbIV[] =
{
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};

//16 Byte key


void encrypter(LPCTSTR PlaintextPath, BYTE (&rgbAES128Key)[16], LPCTSTR EncryptedPath, LPCTSTR KeyBlobPath)
{
	/* --------------------------------------------------------------------	
	Encrypter Function 
	@PARAM LPCTSTR PlaintextPath
	@PARAM BYTE rgbAES128Key
	@PARAM LPCTSTR EncryptedPath
	@PARAM LPCTSTR KeyBlobPath
	-----------------------------------------------------------------------
	*/
	//printf("%d", sizeof(rgbPlaintext));

	BYTE rgbPlaintext[16] = { 0 };
	readFile(PlaintextPath, rgbPlaintext, 16);


	BCRYPT_ALG_HANDLE       hAesAlg = NULL;
	BCRYPT_KEY_HANDLE       hKey = NULL;
	NTSTATUS                status = STATUS_UNSUCCESSFUL;
	//Size in bytes of the buffers. 
	DWORD                   cbCipherText = 0,
		cbPlainText = 0,
		cbData = 0,
		cbKeyObject = 0,
		cbBlockLen = 0,
		cbBlob = 0;
	//Buffers
	PBYTE                   pbCipherText = NULL,
		pbPlainText = NULL,
		pbKeyObject = NULL,
		pbIV = NULL,
		pbBlob = NULL;


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

	// Generate the key from supplied input key bytes.
	if (!NT_SUCCESS(status = BCryptGenerateSymmetricKey(
		hAesAlg,
		&hKey,
		pbKeyObject,
		cbKeyObject,
		(PBYTE)rgbAES128Key,
		sizeof(rgbAES128Key),
		0)))
	{
		wprintf(L"**** Error 0x%x returned by BCryptGenerateSymmetricKey\n", status);
		goto Cleanup;
	}

	//-------------------------------------------------------------------------
	// Save another copy of the key for later.
	//-------------------------------------------------------------------------

	if (!NT_SUCCESS(status = BCryptExportKey(
		hKey,
		NULL,
		BCRYPT_OPAQUE_KEY_BLOB,
		NULL,
		0,
		&cbBlob,
		0)))
	{
		wprintf(L"**** Error 0x%x returned by BCryptExportKey\n", status);
		goto Cleanup;
	}


	// Allocate the buffer to hold the BLOB.
	pbBlob = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbBlob);
	if (NULL == pbBlob)
	{
		wprintf(L"**** memory allocation failed\n");
		goto Cleanup;
	}

	if (!NT_SUCCESS(status = BCryptExportKey(
		hKey,
		NULL,
		BCRYPT_OPAQUE_KEY_BLOB,
		pbBlob,
		cbBlob,
		&cbBlob,
		0)))
	{
		wprintf(L"**** Error 0x%x returned by BCryptExportKey\n", status);
		goto Cleanup;
	}
	else {
		//writeFile(TEXT("TestOutKey.txt"), pbBlob, cbBlob);
		writeFile(KeyBlobPath, pbBlob, cbBlob);
		printf("\n%d Size of key blob = ", cbBlob);
		for (int i = 0; i < cbBlob; i++)
		{
			printf(" \n byte: %d Data : %X\n",i, pbBlob[i]);
		}

	}
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Main Encryption
	//-------------------------------------------------------------------------

	cbPlainText = sizeof(rgbPlaintext);
	pbPlainText = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbPlainText);
	if (NULL == pbPlainText)
	{
		wprintf(L"**** memory allocation failed\n");
		goto Cleanup;
	}

	memcpy(pbPlainText, rgbPlaintext, sizeof(rgbPlaintext));

	//
	// Get the output buffer size.
	//
	if (!NT_SUCCESS(status = BCryptEncrypt(
		hKey,
		pbPlainText,
		cbPlainText,
		NULL,
		pbIV,
		cbBlockLen,
		NULL,
		0,
		&cbCipherText,
		BCRYPT_BLOCK_PADDING)))
	{
		wprintf(L"**** Error 0x%x returned by BCryptEncrypt\n", status);
		goto Cleanup;
	}

	pbCipherText = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbCipherText);
	if (NULL == pbCipherText)
	{
		wprintf(L"**** memory allocation failed\n");
		goto Cleanup;
	}

	// Use the key to encrypt the plaintext buffer.
	// For block sized messages, block padding will add an extra block.
	if (!NT_SUCCESS(status = BCryptEncrypt(
		hKey,
		pbPlainText,
		cbPlainText,
		NULL,
		pbIV,
		cbBlockLen,
		pbCipherText,
		cbCipherText,
		&cbData,
		BCRYPT_BLOCK_PADDING)))
	{
		wprintf(L"**** Error 0x%x returned by BCryptEncrypt\n", status);
		goto Cleanup;
	}
	else
	{
		// Print encryped Data to console
		printf("\nEncrypted text isof size %d \n", cbCipherText);

		//Print Encrypted data on console
		/*for (int i = 0; i < cbCipherText; i++)
		{
			printf("%X\n", pbCipherText[i]);
		}*/
		printf("\n %s", "Encryption Complete");

		//Write enrypted Data to a file
		//writeFile(TEXT("TestOut.txt"), pbCipherText, cbCipherText);
		writeFile(EncryptedPath, pbCipherText, cbCipherText);


	}
	// Destroy the key and reimport from saved BLOB.
	if (!NT_SUCCESS(status = BCryptDestroyKey(hKey)))
	{
		wprintf(L"**** Error 0x%x returned by BCryptDestroyKey\n", status);
		goto Cleanup;
	}
	hKey = 0;

	if (pbPlainText)
	{
		HeapFree(GetProcessHeap(), 0, pbPlainText);
	}

	pbPlainText = NULL;


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
	BYTE rgbAES128Key[16] =
	{ 'P', 'A', 'S', 'S', 'W', 'O', 'R', 'D', 'P', 'A', 'S', 'S', 'W', 'O', 'R', 'D' };
	encrypter(TEXT("Text.txt"), rgbAES128Key, TEXT("keyOut.txt"), TEXT("CT.txt"));

}