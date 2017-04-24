/*
  AES Encryption description.

  Copyright (C) 2016  Michael Wallace.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (At your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <CommProto/architecture/macros.h>
#include <CommProto/debug/log.h>
#include <CommProto/headerpacket.h>
#include <fstream>
#include <CommProto/encryption/aes_encryption.h>

namespace comnet {
namespace encryption {

AesEncryption::AesEncryption() {
		
}

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
		debug::Log::Message(debug::LOG_ERROR, "Encryption Key length is too small. AesEncryption::LoadKey(char* key)\n");
		LOG_PRINTF(debug::LOG_ERROR, "Wanted key size %d read key size of %d\n", KEY_LENGTH, length);
		return 0;
	}
	return 1;//success
}


/** load file which contatins the encryption key by the file name*/
uint8_t AesEncryption::LoadKeyFromFile(char*keyFileName){
	std::ifstream keyFileInput(keyFileName);//open file
	std::string inputString;//string to read line from file
	char temp_key[KEY_LENGTH];//temp key to be inserted from file line

	if (keyFileInput.is_open()){
		std::getline(keyFileInput, inputString);//get line

		if (inputString.length() >= KEY_LENGTH){//translate line of file to key length as char*
			for (int x = 0; x < KEY_LENGTH; x++){
				temp_key[x] = inputString[x];
			}
		}
		else{//key length is too small
			LOG_PRINTF(debug::LOG_ERROR, "key.txt characters mismatch.\nCharacters found: %d\nCharacters needed: %d\n", inputString.length(), KEY_LENGTH);
			return 0;
		}
	}
	else{//file not open	
		LOG_PRINTF(debug::LOG_ERROR, "%s not found. \n", temp_key);
		return 0;
	}
	keyFileInput.close();
	return LoadKey(temp_key);//load key from temp key
}


/** Encrypt buffer for desired length of data stream and return any agumented legnth by reference
A return value of - value is an error */
int32_t AesEncryption::Encrypt(uint8_t* buffer, uint32_t length, uint8_t iv[BLOCK_SIZE]){
	CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption cfbEncryption(sec_key, sec_key.size(), iv);
	cfbEncryption.ProcessData((byte*)buffer, (byte*)buffer, length);
	return 1;
}


/** Decrypt buffer for desired length of data stream and return any agumented legnth by reference
A return value of - value is an error */
int32_t AesEncryption::Decrypt(uint8_t* buffer, uint32_t length, uint8_t iv[BLOCK_SIZE]){
	CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption cfbDecryption(sec_key, sec_key.size(), iv);
	cfbDecryption.ProcessData((byte*)buffer, (byte*)buffer, length);
	return 1; 
}


/** Random number generator which fills an array of size length*/
uint8_t AesEncryption::GenerateRandomIV(uint8_t * buffer, uint32_t length){	
		static THREAD_LOCAL CommRandom randomGen(RANDOM_GEN_MIN, RANDOM_GEN_MAX);
		for (uint32_t x = 0; x < length; x++){
		buffer[x] = randomGen.RandomUint8();
	}
	return 1;
}
} // encryption
} // comnet 
