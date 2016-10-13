#ifndef AES_ENCRYPTION_H
#define AES_ENCRYPTION_H
#include <CommProto/encryption/encryption_interface.h>
#include <CommProto/headerpacket.h>
#include <CommProto/encryption/comm_random.h>
#include <aes.h>
#include <modes.h>


namespace comnet {
	class AesEncryption : public EncryptionInterface
	{
	private:
		CryptoPP::SecByteBlock sec_key;
		CommRandom randomGen;
	public:
		AesEncryption();
		~AesEncryption();
		/** input c string as the form of encrytion key*/
		 uint8_t LoadKey(char* key);
		/** load file which contatins the encryption key by the file name*/
		uint8_t LoadKeyFromFile(char*keyFileName);
		/** Encrypt buffer for desired length of data stream and return any agumented legnth by reference
		A return value of - value is an error */
		int32_t Encrypt(uint8_t* buffer, uint32_t& length, uint8_t iv[BLOCK_SIZE]);
		/** Decrypt buffer for desired length of data stream and return any agumented legnth by reference
		A return value of - value is an error */
		int32_t Decrypt(uint8_t* buffer, uint32_t& length, uint8_t iv[BLOCK_SIZE]);
		/** Randome numbder generator which fills an array of size legnth*/
		uint8_t GenerateRandomIV(uint8_t * buffer, uint32_t length);
	};

}//end namespace comnet
#endif