// Headers 
#include <windows.h>

#include "EncrypterCLI.h"

//const BYTE rgbPlaintext;
//{ 'P', 'A', 'S', 'S', 'W', 'O', 'R', 'D' };

BYTE rgbIV[] =
{
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};
//16 Byte key

Crypto::Crypto(){
	
};


bool Crypto::encrypt(LPCTSTR PlaintextPath, DWORD cbPText, LPCTSTR EncryptedPath, LPCTSTR KeyBlobPath)
{
	/* --------------------------------------------------------------------	
	Encrypter Function 
	@PARAM LPCTSTR PlaintextPath
	@PARAM DWORD cbPText
	@PARAM BYTE rgbAES128Key
	@PARAM LPCTSTR EncryptedPath
	@PARAM LPCTSTR KeyBlobPath
	-----------------------------------------------------------------------
	*/
	PBYTE rgbPlaintext = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbPText);
	readFile(PlaintextPath, rgbPlaintext, cbPText);

	//Print the size of Plaintext
	//printf("%d", sizeof(rgbPlaintext));
	/*char size[10];
	sprintf_s(size, "%d", sizeof(rgbPlaintext));
	MessageBoxA(NULL, (LPCSTR)size, "File Read", MB_OK);*/

	BYTE rgbAES128Key[] =
	{ 'P', 'A', 'S', 'S', 'W', 'O', 'R', 'D', 'P', 'A', 'S', 'S', 'W', 'O', 'R', 'D' };

	static const BYTE rgbIV[] =
	{
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
	};


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
		return false;

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
		return false;

	}

	// Allocate the key object on the heap.
	pbKeyObject = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbKeyObject);
	if (NULL == pbKeyObject)
	{
		wprintf(L"**** memory allocation failed\n");
		goto Cleanup;
		return false;

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
		return false;

	}

	// Determine whether the cbBlockLen is not longer than the IV length.
	if (cbBlockLen > sizeof(rgbIV))
	{
		wprintf(L"**** block length is longer than the provided IV length\n");
		goto Cleanup;
		return false;

	}

	// Allocate a buffer for the IV. The buffer is consumed during the 
	// encrypt/decrypt process.
	pbIV = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbBlockLen);
	if (NULL == pbIV)
	{
		wprintf(L"**** memory allocation failed\n");
		goto Cleanup;
		return false;

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
		return false;

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
		return false;

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
		return false;

	}


	// Allocate the buffer to hold the BLOB.
	pbBlob = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbBlob);
	if (NULL == pbBlob)
	{
		wprintf(L"**** memory allocation failed\n");
		goto Cleanup;
		return false;

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
		return false;

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

	cbPlainText = cbPText;
	pbPlainText = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbPlainText);
	if (NULL == pbPlainText)
	{
		wprintf(L"**** memory allocation failed\n");
		goto Cleanup;
		return false;

	}

	memcpy(pbPlainText, rgbPlaintext, cbPText);

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
		return false;

	}

	pbCipherText = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbCipherText);
	if (NULL == pbCipherText)
	{
		wprintf(L"**** memory allocation failed\n");
		goto Cleanup;
		return false;

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
		return false;

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
		writeFile(EncryptedPath, pbCipherText, cbCipherText);

		//Print Encrypted data size on messagebox
		/*char size[10];
		sprintf_s(size, "%d", cbCipherText);
		MessageBoxA(NULL, (LPCSTR)size, "File Read", MB_OK);*/
		return true;


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

bool Crypto::decrypt(LPCTSTR CipherTextPath, DWORD cbCText, LPCTSTR KeyBlobPath, LPCTSTR PlainTextPath)
{
	/* --------------------------------------------------------------------
	Decrypter Function
	@PARAM LPCTSTR CipherTextPath
	@PARAM DWORD cbCText
	@PARAM LPCTSTR KeyBlobPath
	@PARAM LPCTSTR PlainTextPath

	-----------------------------------------------------------------------
	*/


	/*BYTE rgbPlaintext[40] = { 0 };
	readFile(TEXT("Text.txt"), rgbPlaintext, 40);
	printf("****PlainText data : \n");*/
	//printf("%d", sizeof(rgbPlaintext));



	BCRYPT_ALG_HANDLE       hAesAlg = NULL;
	BCRYPT_KEY_HANDLE       hKey = NULL;
	NTSTATUS                status = STATUS_UNSUCCESSFUL;

	//Size in bytes of the buffers. 
	//TODO: Dynamically allocate cbCipherText size
	DWORD                   cbCipherText = cbCText,
		cbPlainText = 0,
		cbData = 0,
		cbKeyObject = 0,
		cbBlockLen = 0,
		cbBlob = 560;
	//Buffers
	PBYTE                   pbCipherText = NULL,
		pbPlainText = NULL,
		pbKeyObject = NULL,
		pbIV = NULL,
		pbBlob = NULL;


	//Read CipherText from File
	wprintf(L"*****Reading Cipher Text from file******************\n");

	// Allocate the buffer to Cipher Text the BLOB.
	PBYTE rgbCrypto = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbCipherText);
	readFile(CipherTextPath, rgbCrypto, cbCipherText);
	//for (int i = 0; i <= cbCipherText ; i++)
	//{
	//	wprintf(L"\n byte: %d Data : %x\n", i, rgbCrypto[i]);
	//}
	wprintf(L"*****Done Reading Cipher Text from file******************\n\n\n");


	//Read key from File
	// Allocate the buffer to hold the BLOB.
	pbBlob = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbBlob);
	//BYTE rgbKeyBlob[560] = { 0 };

	wprintf(L"*****Reading Key Blob from file******************\n");

	//readFile(TEXT("keyBlob.txt"), pbBlob, cbBlob);
	readFile(KeyBlobPath, pbBlob, cbBlob);
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
	else
	{
		wprintf(L"cbBlock length : %d \n", cbBlockLen);
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
	/*wprintf(L"IV is :\n");
	for (int i = 0; i < cbBlockLen; i++)
		printf("%x \n", pbIV[i]);
*/
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
	//memset(pbKeyObject, 0, cbKeyObject);





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
	//printf("Plaintext Size : %d\n", cbPlainText);

	if (!NT_SUCCESS(status = BCryptDecrypt(
		hKey,
		rgbCrypto,
		cbCipherText,
		NULL,
		pbIV,
		cbBlockLen,
		NULL,
		0,
		&cbPlainText,
		BCRYPT_BLOCK_PADDING)))
	{
		wprintf(L"**** Error 0x%x returned by BCryptDecrypt of size \n", status);

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
	else
	{
		/*wprintf(L"Plain Text is :\n");
	for (int i = 0; i < cbPlainText; i++)
		printf("%x \n", pbPlainText[i]);*/
	}


	if (!NT_SUCCESS(status = BCryptDecrypt(
		hKey,
		rgbCrypto,
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
		/*wprintf(L"Plaintext Data :\n");
		for (int i = 0; i < 40; i++)
			printf("%x \n", rgbPlaintext[i]);*/

		wprintf(L"Decrypted Data :\n");
		for (int i = 0; i < cbPlainText; i++)
			printf("%x \n", pbPlainText[i]);
		//Write to File after decryption. 
		writeFile(PlainTextPath, pbPlainText, cbPlainText);

	}

	/*if (0 != memcmp(pbPlainText, (PBYTE)rgbPlaintext, sizeof(rgbPlaintext)))
	{
		wprintf(L"Expected decrypted text comparison failed.\n");
		goto Cleanup;
	}*/

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

	if (rgbCrypto)
	{
		HeapFree(GetProcessHeap(), 0, rgbCrypto);
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

	return true;
}



//void __cdecl wmain(int argc, __in_ecount(argc) LPWSTR *wargv)
//{
//	
//	UNREFERENCED_PARAMETER(argc);
//	UNREFERENCED_PARAMETER(wargv);
//	BYTE rgbAES128Key[16] =
//	{ 'P', 'A', 'S', 'S', 'W', 'O', 'R', 'D', 'P', 'A', 'S', 'S', 'W', 'O', 'R', 'D' };
//	encrypter(TEXT("Text.txt"), rgbAES128Key, TEXT("keyOut.txt"), TEXT("CT.txt"));
//	decrypt(TEXT("CT.txt"), TEXT("keyBlob.txt"), TEXT("PT.txt"));
//
//}

