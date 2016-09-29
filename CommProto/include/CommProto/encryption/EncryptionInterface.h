#ifndef ENCRYPTION_INTERFACE_H
#define ENCRYPTION_INTERFACE_H

#include <stdint.h>
#define BLOCK_SIZE 16

//abstract class to use for aes encryption
class EncryptionInterface
{
/** input c string as the form of encrytion key*/
virtual uint8_t loadKey(char* key) = 0;
/** load file which contatins the encryption key by the file name*/
virtual uint8_t loadKeyFromFile(char*keyFileName) = 0;
/** Encrypt buffer for desired length of data stream and return any agumented legnth by reference
	A return value of - value is an error */
virtual int32_t encrypt(uint8_t* buffer, uint32_t& length, uint8_t IV[BLOCK_SIZE] = 0) = 0;
/** Decrypt buffer for desired length of data stream and return any agumented legnth by reference
	A return value of - value is an error */
virtual int32_t decrypt(uint8_t* buffer, uint32_t& length, uint8_t IV[BLOCK_SIZE] = 0) = 0;
/** Randome numbder generator which fills an array of size legnth*/
virtual uint8_t generateRandomIV(uint8_t * buffer, uint32_t length)=0;
};
#endif