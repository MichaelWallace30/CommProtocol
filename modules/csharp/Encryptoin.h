#include <CommProto/encryption/aes_encryption.h>


using namespace System;
using namespace System::Runtime::InteropServices;

#pragma once
namespace Comnet{
#pragma managed	

	public ref class Encryption sealed
	{
	internal:
		//unmanged pointer to encryption class
		comnet::AesEncryption *aesEncryption;
	public:
		/** input c string as the form of encrytion key*/
		Boolean LoadKey(String key);
		/** load file which contatins the encryption key by the file name*/
		Boolean LoadKeyFromFile(String keyFileName);
		/** Encrypt buffer for desired length of data stream and return any agumented legnth by reference
		A return value of - value is an error */
		int32_t Encrypt(Byte^ buffer, UINT32% length, Byte iv[BLOCK_SIZE]);
		/** Decrypt buffer for desired length of data stream and return any agumented legnth by reference
		A return value of - value is an error */
		int32_t Decrypt(Byte^ buffer, UINT32% length, Byte iv[BLOCK_SIZE]);
		/** Randome numbder generator which fills an array of size legnth*/
		Boolean GenerateRandomIV(Byte^ buffer, UINT32 length);

	};

}