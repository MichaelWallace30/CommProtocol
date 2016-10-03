#include <CommProto/encryption/aes_encryption.h>
#include <CommProto/architecture/macros.h>//str_length
#include <CommProto/debug/comms_debug.h>//COMMS_DEBUG
#include <modes.h>

AesEncryption::AesEncryption(){}
AesEncryption::~AesEncryption(){}
/** input c string as the form of encrytion key*/
uint8_t AesEncryption::LoadKey(char* key){	
	byte temp_key[KEY_LENGTH] = { 0 };
	uint16_t length = 0;
	str_length(key,length);
	if (length >= KEY_LENGTH){		
		memcpy(&temp_key, key, KEY_LENGTH);//copy key method want byte or constant *byte not char*
		sec_key = CryptoPP::SecByteBlock(temp_key, KEY_LENGTH);
	}else{//key lenth too small
		COMMS_DEBUG("Encryption Key length is too small. AesEncryption::LoadKey(char* key)\n");
		COMMS_DEBUG("Wanted key size %d read key size of %d\n", KEY_LENGTH, length);
		return 0;
	}
	return 1;//success
}
/** load file which contatins the encryption key by the file name*/
uint8_t AesEncryption::LoadKeyFromFile(char*keyFileName){
	//@TODO Michael Wallace
	return 1; }

/** Encrypt buffer for desired length of data stream and return any agumented legnth by reference
A return value of - value is an error */
int32_t AesEncryption::Encrypt(uint8_t* buffer, uint32_t& length, uint8_t iv[BLOCK_SIZE]){
	CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption cfbEncryption(sec_key, sec_key.size(), iv);
	cfbEncryption.ProcessData((byte*)buffer, (byte*)buffer, length);
	return 1;
}

/** Decrypt buffer for desired length of data stream and return any agumented legnth by reference
A return value of - value is an error */
int32_t AesEncryption::Decrypt(uint8_t* buffer, uint32_t& length, uint8_t iv[BLOCK_SIZE]){
	CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption cfbDecryption(sec_key, sec_key.size(), iv);
	cfbDecryption.ProcessData((byte*)buffer, (byte*)buffer, length);
	return 1; 
}

/** Randome numbder generator which fills an array of size legnth*/
uint8_t AesEncryption::GenerateRandomIV(uint8_t * buffer, uint32_t length){
	//@TODO Michael Wallace
	return 1;
}