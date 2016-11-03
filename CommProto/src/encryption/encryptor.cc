/*
  Encryptor implementations.

  Copyright (C) 2016  Mario Garcia, Michael Wallace.

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
#include <CommProto/encryption/encryptor.h>
#include <CommProto/encryption/decryptor.h>
#include <CommProto/debug/comms_debug.h>

#include "aes_encryption.h"

namespace comnet {
namespace encryption {


CommEncryptor::CommEncryptor(CryptProtocol proto)
: protocol(proto)
, decryptor(nullptr)
, encryption(std::shared_ptr<EncryptionInterface>(nullptr))
{
  Setup();
}


CommEncryptor::CommEncryptor(CryptProtocol proto, CommDecryptor* decryptor)
: protocol(proto)
, decryptor(decryptor)
{
  encryption = std::shared_ptr<EncryptionInterface>(this->decryptor->encryption);
  this->decryptor->LinkEncryptor(this);
  protocol = this->decryptor->protocol;
}


CommEncryptor::~CommEncryptor() 
{ 
  if (decryptor != nullptr) {
    decryptor->encryptor = nullptr;
  }
}


uint8_t CommEncryptor::LoadKey(char* key) {
  uint8_t result = 0;
  if (encryption != nullptr) {
    result = encryption->LoadKey(key);
  }
  return result;
}


uint8_t CommEncryptor::LoadKeyFromFile(char* filename_key) {
  uint8_t key = 0;
  if (encryption != nullptr) {
    key = encryption->LoadKeyFromFile(filename_key);
  }
  return key;
}


int32_t CommEncryptor::Encrypt(uint8_t* buffer, uint32_t& length, uint8_t iv[BLOCK_SIZE]) {
  int32_t result = 0;
  if (encryption != nullptr) {
    result = encryption->Encrypt(buffer, length, iv);
  } 
  return result;
}


void CommEncryptor::Setup() {
  switch (protocol) {
    case AES:
      encryption = std::shared_ptr<EncryptionInterface>(new AesEncryption());
      break;
    default: // do nothing
      break;
  }
}
} // encryption
} // comnet