/*
  Decryptor implementations.

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

#include <CommProto/encryption/decryptor.h>
#include <CommProto/encryption/encryptor.h>
#include <CommProto/debug/comms_debug.h>
#include <CommProto/encryption/aes_encryption.h>

namespace comnet {
namespace encryption {


CommDecryptor::CommDecryptor()
: protocol(NONE)
, encryptor(nullptr)
, encryption(nullptr)
{
}


CommDecryptor::CommDecryptor(CryptProtocol proto)
: protocol(proto)
, encryptor(nullptr)
, encryption(std::shared_ptr<EncryptionInterface>(nullptr))
{
  Setup();
}


CommDecryptor::CommDecryptor(CryptProtocol proto, CommEncryptor* encryptor)
: protocol(proto)
, encryptor(encryptor)
{
  encryption = std::shared_ptr<EncryptionInterface>(this->encryptor->encryption);
  this->encryptor->LinkDecryptor(this);
  protocol = encryptor->protocol;
}


CommDecryptor::CommDecryptor(CommDecryptor&& decrypt)
: encryption(decrypt.encryption)
{
  std::swap(encryptor, decrypt.encryptor);
  std::swap(protocol, decrypt.protocol);
}


CommDecryptor& CommDecryptor::operator=(CommDecryptor&& decrypt) {
  encryption = decrypt.encryption;
  std::swap(encryptor, decrypt.encryptor);
  std::swap(protocol, decrypt.protocol);
  return *this;
}


CommDecryptor::~CommDecryptor() 
{
  if (encryptor != nullptr) {
    encryptor->decryptor = nullptr;
  }
}


uint8_t CommDecryptor::LoadKey(char* key) {
  uint8_t result = 0;
  if(encryption != nullptr) {
    result = encryption->LoadKey(key);
  }
  return result;
}


uint8_t CommDecryptor::LoadKeyFromFile(char* filename_key) {
  uint8_t key = 0;
  if(encryption != nullptr) {
    key = encryption->LoadKeyFromFile(filename_key);
  }
  return key;
}


int32_t CommDecryptor::Decrypt(comnet::serialization::ObjectStream* obj) {
  int32_t result = 0;
  if (!obj) {
    return -1;
  }
  if (obj->GetSize() <= 0) {
    return -2;
  }
  if (encryption != nullptr && KeyIsLoaded()) {
    // TODO(Garcia): Will require decryption of header!
    // It really doesn't matter, the length isn't returned.
    result = encryption->Decrypt(obj->GetBuffer() + sizeof(Header), 
                                 obj->GetHeaderPacket().msg_len, 
                                 obj->GetHeaderPacket().iv);
    return 1;
  }
  return result;
}


void CommDecryptor::Setup() {
  switch(protocol) {
  case AES:
  encryption = std::shared_ptr<EncryptionInterface>(new AesEncryption());
  break;
  default: // do nothing
  break;
  }
}


bool CommDecryptor::KeyIsLoaded() {
  if (encryption != nullptr) {
    return encryption->KeyIsLoaded();
  }
  return false;
}
} // encryption
} // comnet