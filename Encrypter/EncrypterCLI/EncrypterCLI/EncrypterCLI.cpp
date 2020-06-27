// Headers 
#include <windows.h>
#include <stdio.h>
#include <bcrypt.h>
#include "EncrypterCLI.h"
#include <assert.h>
#include <vector>
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

	BYTE rgbPlaintext[40] = { 0 };
	readFile(PlaintextPath, rgbPlaintext, 40);


	BCRYPT_ALG_HANDLE       hAesAlg = NULL;
	BCRYPT_KEY_HANDLE       hKey = NULL;
	NTSTATUS                status = STATUS_UNSUCCESSFUL;
	//Size in bytes of the buffers. 
	DWORD                   cbCipherText = 0,
		cbPlainText = 0,
		cbData = 0,
		cbKeyObject = 0,
		cbBlockLen = 0,
		cbIV = 12,
		cbBlob = 0;
	//Buffers
	PBYTE                   pbCipherText = NULL,
		pbPlainText = NULL,
		pbKeyObject = NULL,
		pbIV = NULL,
		pbBlob = NULL;

	NTSTATUS bcryptResult = 0;
	// To be clear, we're using an IV and a GCM nonce that are all
	// zeroes. This is only for testing. In a real application,
	// you MUST ensure the GCM nonce is never repeated. If you fail
	// to do so, your implementation will be inherently insecure.

	// Create an IV that is the same size as Botan's:
	const size_t AES_IV_SIZE = 12;
	const std::vector<BYTE> origIV = { 0,0,0,0,0,0,0,0,0,0,0,0 };

	// This must always be 96 bits (12 bytes):
	const size_t GCM_NONCE_SIZE = 12;
	const std::vector<BYTE> origNonce = { 0,0,0,0,0,0,0,0,0,0,0,0 };

		// We are going to do in-place encryption:

	


	// Open an algorithm handle.
	if (!NT_SUCCESS(status = BCryptOpenAlgorithmProvider(
		&hAesAlg,
		BCRYPT_AES_ALGORITHM,
		NULL,
		0)))
	{
		wprintf(L"**** Error 0x%x returned by BCryptOpenAlgorithmProvider\n", status);
		//goto Cleanup;
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
		//goto Cleanup;
	}

	// Allocate the key object on the heap.
	pbKeyObject = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbKeyObject);
	if (NULL == pbKeyObject)
	{
		wprintf(L"**** memory allocation failed\n");
		//goto Cleanup;
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
		//goto Cleanup;
	}

	//// Determine whether the cbBlockLen is not longer than the IV length.
	//if (cbBlockLen > sizeof(rgbIV))
	//{
	//	wprintf(L"**** block length is longer than the provided IV length\n");
	//	goto Cleanup;
	//}

	// Allocate a buffer for the IV. The buffer is consumed during the 
	// encrypt/decrypt process.
	pbIV = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbIV);
	if (NULL == pbIV)
	{
		wprintf(L"**** memory allocation failed\n");
		//goto Cleanup;
	}

	memcpy(pbIV, rgbIV, cbIV);

	if (!NT_SUCCESS(status = BCryptSetProperty(
		hAesAlg,
		BCRYPT_CHAINING_MODE,
		(PBYTE)BCRYPT_CHAIN_MODE_GCM,
		sizeof(BCRYPT_CHAIN_MODE_GCM),
		0)))
	{
		wprintf(L"**** Error 0x%x returned by BCryptSetProperty\n", status);
		//goto Cleanup;
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
		//goto Cleanup;
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
		//goto Cleanup;
	}


	// Allocate the buffer to hold the BLOB.
	pbBlob = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbBlob);
	if (NULL == pbBlob)
	{
		wprintf(L"**** memory allocation failed\n");
		//goto Cleanup;
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
		//goto Cleanup;
	}
	else {
		//writeFile(TEXT("TestOutKey.txt"), pbBlob, cbBlob);
		writeFile(KeyBlobPath, pbBlob, cbBlob);
		printf("\n%d Size of key blob = ", cbBlob);
		/*for (int i = 0; i < cbBlob; i++)
		{
			printf(" \n byte: %d Data : %X\n",i, pbBlob[i]);
		}*/

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
		//goto Cleanup;
	}

	memcpy(pbPlainText, rgbPlaintext, sizeof(rgbPlaintext));

	// This tells us the length of the authentication tag:
	BCRYPT_AUTH_TAG_LENGTHS_STRUCT authTagLengths;
	bcryptResult = BCryptGetProperty(hAesAlg, BCRYPT_AUTH_TAG_LENGTH, (BYTE*)&authTagLengths, sizeof(authTagLengths), &cbData, 0);
	assert(BCRYPT_SUCCESS(bcryptResult) || !"BCryptGetProperty(BCRYPT_AUTH_TAG_LENGTH)");

	std::vector<BYTE> authTag(authTagLengths.dwMaxLength);

	// This sets up our nonce and GCM authentication tag parameters:
	BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO authInfo;
	BCRYPT_INIT_AUTH_MODE_INFO(authInfo);
	authInfo.pbNonce = (PUCHAR)&origNonce[0]; // A nonce is required for GCM
	authInfo.cbNonce = origNonce.size(); // The size of the nonce is provided here
	authInfo.pbTag = &authTag[0]; // The buffer that will gain the authentication tag
	authInfo.cbTag = authTag.size(); // The size of the authentication tag


	//
	// Get the output buffer size.
	//
	if (!NT_SUCCESS(status = BCryptEncrypt(
		hKey,
		pbPlainText,
		cbPlainText,
		&authInfo,
		pbIV,
		cbIV,
		NULL,
		0,
		&cbCipherText,
		0)))
	{
		wprintf(L"**** Error 0x%x returned by BCryptEncrypt\n", status);
		//goto Cleanup;
	}

	pbCipherText = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbCipherText);
	if (NULL == pbCipherText)
	{
		wprintf(L"**** memory allocation failed\n");
		//goto Cleanup;
	}

	// Use the key to encrypt the plaintext buffer.
	// For block sized messages, block padding will add an extra block.
	if (!NT_SUCCESS(status = BCryptEncrypt(
		hKey,
		pbPlainText,
		cbPlainText,
		&authInfo,
		pbIV,
		cbIV,
		pbCipherText,
		cbCipherText,
		&cbData,
		0)))
	{
		wprintf(L"**** Error 0x%x returned by BCryptEncrypt\n", status);
		//goto Cleanup;
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
		//goto Cleanup;
	}
	hKey = 0;

	if (pbPlainText)
	{
		HeapFree(GetProcessHeap(), 0, pbPlainText);
	}

	pbPlainText = NULL;


//Cleanup:
//
//	if (hAesAlg)
//	{
//		BCryptCloseAlgorithmProvider(hAesAlg, 0);
//	}
//
//	if (hKey)
//	{
//		BCryptDestroyKey(hKey);
//	}
//
//	if (pbCipherText)
//	{
//		HeapFree(GetProcessHeap(), 0, pbCipherText);
//	}
//
//	if (pbPlainText)
//	{
//		HeapFree(GetProcessHeap(), 0, pbPlainText);
//	}
//
//	if (pbKeyObject)
//	{
//		HeapFree(GetProcessHeap(), 0, pbKeyObject);
//	}
//
//	if (pbIV)
//	{
//		HeapFree(GetProcessHeap(), 0, pbIV);
//	}
	BCryptCloseAlgorithmProvider(hAesAlg, 0);
	BCryptDestroyKey(hKey);
	HeapFree(GetProcessHeap(), 0, pbCipherText);
	HeapFree(GetProcessHeap(), 0, pbPlainText);
	HeapFree(GetProcessHeap(), 0, pbKeyObject);
	HeapFree(GetProcessHeap(), 0, pbIV);
}

void __cdecl wmain(int argc, __in_ecount(argc) LPWSTR *wargv)
{
	
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(wargv);
	BYTE rgbAES128Key[16] =
	{ 'P', 'A', 'S', 'S', 'W', 'O', 'R', 'D', 'P', 'A', 'S', 'S', 'W', 'O', 'R', 'D' };
	encrypter(TEXT("Text.txt"), rgbAES128Key, TEXT("CT.txt"), TEXT("keyBlob.txt"));

}