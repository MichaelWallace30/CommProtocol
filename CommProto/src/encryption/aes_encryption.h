/*
  encryption protocol.

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
#ifndef AES_ENCRYPTION_H
#define AES_ENCRYPTION_H
#include <CommProto/encryption/encryption_interface.h>
#include <CommProto/architecture/api.h>
#include <CommProto/encryption/comm_random.h>
#include <aes.h>//cryptopp
#include <modes.h>//cryptopp


namespace comnet {
namespace encryption {


class COMM_EXPORT AesEncryption : public EncryptionInterface {
private:
  CryptoPP::SecByteBlock sec_key;
  static const int RANDOM_GEN_MIN = 0;
  static const int RANDOM_GEN_MAX = 255;
public:
  AesEncryption();
  ~AesEncryption();
  /** 
    input c string as the form of encrytion key
   */
  uint8_t LoadKey(char* key) override;
  /** 
    load file which contatins the encryption key by the file name
   */
  uint8_t LoadKeyFromFile(char*keyFileName) override;
  /** 
    Encrypt buffer for desired length of data stream and return any agumented legnth by reference
    A return value of - value is an error 
  */
  int32_t Encrypt(uint8_t* buffer, uint32_t length, uint8_t iv[BLOCK_SIZE]) override;
  /** 
    Decrypt buffer for desired length of data stream and return any agumented legnth by reference
    @returns A return value of - value is an error 
   */
  int32_t Decrypt(uint8_t* buffer, uint32_t length, uint8_t iv[BLOCK_SIZE]) override;
  /** 
    Random number generator which fills an array of size length
   */
  uint8_t GenerateRandomIV(uint8_t * buffer, uint32_t length) override;

  bool KeyIsLoaded() override { return !sec_key.empty(); }
};
} // encryption
} // comnet
#endif // AES_ENCRYPTION_H