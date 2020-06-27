//#include <windows.h>
//#include <assert.h>
//#include <vector>
//#include <Bcrypt.h>
//#include <iostream>
//#include <string>
//#include <string.h>
//#include <wincrypt.h>
//#include <ntstatus.h>
//#pragma comment(lib, "bcrypt.lib")
//
//// Base 64 library:
//// http://renenyffenegger.ch/notes/development/Base64/Encoding-and-decoding-base-64-with-cpp
//#include "base64.h"
//
//void printB64(BYTE *buffer, size_t length) {
//	std::string result = base64_encode(buffer, length);
//	std::cout << "Buffer (size: " << length << "): " << result << std::endl;
//}
//
//int main(int argc, CHAR* argv[]) {
//	NTSTATUS bcryptResult = 0;
//	DWORD bytesDone = 0;
//
//	// This gets us the AES algorithm:
//	BCRYPT_ALG_HANDLE algHandle = 0;
//	bcryptResult = BCryptOpenAlgorithmProvider(&algHandle, BCRYPT_AES_ALGORITHM, 0, 0);
//	assert(BCRYPT_SUCCESS(bcryptResult) || !"BCryptOpenAlgorithmProvider");
//
//	// This sets up the GCM chaining mode:
//	bcryptResult = BCryptSetProperty(algHandle, BCRYPT_CHAINING_MODE, (BYTE*)BCRYPT_CHAIN_MODE_GCM, sizeof(BCRYPT_CHAIN_MODE_GCM), 0);
//	assert(BCRYPT_SUCCESS(bcryptResult) || !"BCryptSetProperty(BCRYPT_CHAINING_MODE)");
//
//	// This tells us the length of the authentication tag:
//	BCRYPT_AUTH_TAG_LENGTHS_STRUCT authTagLengths;
//	bcryptResult = BCryptGetProperty(algHandle, BCRYPT_AUTH_TAG_LENGTH, (BYTE*)&authTagLengths, sizeof(authTagLengths), &bytesDone, 0);
//	assert(BCRYPT_SUCCESS(bcryptResult) || !"BCryptGetProperty(BCRYPT_AUTH_TAG_LENGTH)");
//
//	std::cout << "Auth Tag Length (min): " << authTagLengths.dwMinLength << std::endl;
//	std::cout << "Auth Tag Length (max): " << authTagLengths.dwMaxLength << std::endl;
//
//	// This tells us the length of the block:
//	DWORD blockLength = 0;
//	bcryptResult = BCryptGetProperty(algHandle, BCRYPT_BLOCK_LENGTH, (BYTE*)&blockLength, sizeof(blockLength), &bytesDone, 0);
//	assert(BCRYPT_SUCCESS(bcryptResult) || !"BCryptGetProperty(BCRYPT_BLOCK_LENGTH)");
//
//	std::cout << "Block Length: " << blockLength << std::endl;
//
//	BCRYPT_KEY_HANDLE keyHandle = 0;
//	std::string decodedKeyString = base64_decode("PASSWORDpasswordPASSWORDpassword");
//	const std::vector<BYTE> key(decodedKeyString.data(), decodedKeyString.data() + blockLength);
//
//	// This function takes the key buffer and directly uses it as a key. It does not manipulate
//	// it in a way that would cause it not to match on the Botan side.
//	bcryptResult = BCryptGenerateSymmetricKey(algHandle, &keyHandle, 0, 0, (PUCHAR)&key[0], key.size(), 0);
//	assert(BCRYPT_SUCCESS(bcryptResult) || !"BCryptGenerateSymmetricKey");
//
//	std::cout << "Key: ";
//	printB64((BYTE *)&key[0], key.size());
//
//	UCHAR namebuff[256];
//	ULONG tempSize = 256; // We ignore this because we know the buffer is big enough.
//	const WCHAR *name = (const WCHAR *)namebuff;
//	bcryptResult = BCryptGetProperty(algHandle, BCRYPT_ALGORITHM_NAME, namebuff, sizeof(namebuff), &tempSize, 0);
//	std::cout << "Obtained algorithm name: ";
//	std::wcout << name << std::endl; // Outputs: AES
//
//	bcryptResult = BCryptGetProperty(algHandle, BCRYPT_CHAINING_MODE, namebuff, sizeof(namebuff), &tempSize, 0);
//	std::cout << "Obtained chaining mode: ";
//	std::wcout << name << std::endl; // Outputs: ChainingModeGCM
//
//	// To be clear, we're using an IV and a GCM nonce that are all
//	// zeroes. This is only for testing. In a real application,
//	// you MUST ensure the GCM nonce is never repeated. If you fail
//	// to do so, your implementation will be inherently insecure.
//
//	// Create an IV that is the same size as Botan's:
//	const size_t AES_IV_SIZE = 12;
//	const std::vector<BYTE> origIV = { 0,0,0,0,0,0,0,0,0,0,0,0 };
//
//	std::cout << "Initialization Vector: ";
//	printB64((BYTE *)&origIV[0], origIV.size());
//
//	// This must always be 96 bits (12 bytes):
//	const size_t GCM_NONCE_SIZE = 12;
//	const std::vector<BYTE> origNonce = { 0,0,0,0,0,0,0,0,0,0,0,0 };
//
//	std::cout << "Nonce: ";
//	printB64((BYTE *)&origNonce[0], origNonce.size());
//
//	// This needs to be a multiple of the block size (128 bits/16 bytes) for Windows CNG. Our example is 128 bytes in ASCII (16 * 8 = 128)
//	std::string plaintext("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz01234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz01234567809AB");
//	const std::vector<BYTE> plaintextVector(plaintext.data(), plaintext.data() + plaintext.length());
//
//	// We are going to do in-place encryption:
//	std::vector<BYTE> encrypted = plaintextVector;
//	std::vector<BYTE> authTag(authTagLengths.dwMaxLength);
//
//	// This sets up our nonce and GCM authentication tag parameters:
//	BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO authInfo;
//	BCRYPT_INIT_AUTH_MODE_INFO(authInfo);
//	authInfo.pbNonce = (PUCHAR)&origNonce[0]; // A nonce is required for GCM
//	authInfo.cbNonce = origNonce.size(); // The size of the nonce is provided here
//	authInfo.pbTag = &authTag[0]; // The buffer that will gain the authentication tag
//	authInfo.cbTag = authTag.size(); // The size of the authentication tag
//
//	std::cout << "Plaintext (" << plaintext.length() << " bytes): " << plaintext << std::endl;
//
//	bcryptResult = BCryptEncrypt(
//		keyHandle,
//		&encrypted[0], encrypted.size(), // Plaintext and its associated size
//		&authInfo, // Must be a BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO for GCM
//		(PUCHAR)&origIV[0], 12, // No initialization vector provided. We'd want to change this.
//		&encrypted[0], encrypted.size(), // The buffer in which to store the encrypted output and its size
//		&bytesDone, 0
//	);
//
//	std::cout << "AES-128/GCM(16) ciphertext: ";
//	printB64((BYTE *)&encrypted[0], encrypted.size());
//
//	std::cout << "Auth Tag: ";
//	printB64((BYTE *)&authTag[0], authTag.size());
//
//	assert(BCRYPT_SUCCESS(bcryptResult) || !"BCryptEncrypt");
//	assert(bytesDone == encrypted.size());
//
//	// Cleanup
//	BCryptDestroyKey(keyHandle);
//	BCryptCloseAlgorithmProvider(algHandle, 0);
//
//	return 0;