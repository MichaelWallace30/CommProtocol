#ifndef AES_ENCRYPTION_H
#define AES_ENCRYPTION_H
#include <CommProto/encryption/encryption_interface.h>


class AesEncryption : public EncryptionInterface
{
AesEncryption();
~AesEncryption();
/** input c string as the form of encrytion key*/
virtual uint8_t LoadKey(char* key);
/** load file which contatins the encryption key by the file name*/
virtual uint8_t LoadKeyFromFile(char*keyFileName);
/** Encrypt buffer for desired length of data stream and return any agumented legnth by reference
A return value of - value is an error */
virtual int32_t Encrypt(uint8_t* buffer, uint32_t& length, uint8_t iv[BLOCK_SIZE] = 0);
/** Decrypt buffer for desired length of data stream and return any agumented legnth by reference
A return value of - value is an error */
virtual int32_t Decrypt(uint8_t* buffer, uint32_t& length, uint8_t iv[BLOCK_SIZE] = 0);
/** Randome numbder generator which fills an array of size legnth*/
virtual uint8_t GenerateRandomIV(uint8_t * buffer, uint32_t length);
};
#endif