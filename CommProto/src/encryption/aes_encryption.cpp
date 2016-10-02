#include <CommProto/encryption/aes_encryption.h>

AesEncryption::AesEncryption(){}
AesEncryption::~AesEncryption(){}
/** input c string as the form of encrytion key*/
uint8_t AesEncryption::LoadKey(char* key){ return 1; }
/** load file which contatins the encryption key by the file name*/
uint8_t AesEncryption::LoadKeyFromFile(char*keyFileName){ return 1; }
/** Encrypt buffer for desired length of data stream and return any agumented legnth by reference
A return value of - value is an error */
int32_t AesEncryption::Encrypt(uint8_t* buffer, uint32_t& length, uint8_t iv[BLOCK_SIZE]){ return 1; }
/** Decrypt buffer for desired length of data stream and return any agumented legnth by reference
A return value of - value is an error */
int32_t AesEncryption::Decrypt(uint8_t* buffer, uint32_t& length, uint8_t iv[BLOCK_SIZE]){ return 1; }
/** Randome numbder generator which fills an array of size legnth*/
uint8_t AesEncryption::GenerateRandomIV(uint8_t * buffer, uint32_t length){ return 1; }