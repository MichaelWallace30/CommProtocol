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
#include <CommProto/encryption/comm_random.h>
#include <CommProto/debug/comms_debug.h>
#include <CommProto/encryption/aes_encryption.h>

namespace comnet {
namespace encryption {


CommEncryptor::CommEncryptor()
: protocol(NONE)
, decryptor(nullptr)
, encryption(nullptr)
{
}


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


CommEncryptor::CommEncryptor(CommEncryptor&& encrypt) 
: encryption(encrypt.encryption)
{
  std::swap(decryptor, encrypt.decryptor);
  std::swap(protocol, encrypt.protocol);
}


CommEncryptor& CommEncryptor::operator=(CommEncryptor&& encrypt) {
  encryption = encrypt.encryption;
  std::swap(decryptor, encrypt.decryptor);
  std::swap(protocol, encrypt.protocol);
  return *this;
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


int32_t CommEncryptor::Encrypt(comnet::serialization::ObjectStream* obj) {
  int32_t result = 0;
  if (encryption != nullptr && KeyIsLoaded()) {
    // TODO(Garcia): Will require encryption of the header!
    encryption->GenerateRandomIV(obj->GetHeaderPacket().iv, BLOCK_SIZE);
    result = encryption->Encrypt(obj->GetBuffer() + sizeof(Header), 
                                 obj->GetHeaderPacket().msg_len, 
                                 obj->GetHeaderPacket().iv);
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


bool CommEncryptor::KeyIsLoaded() {
  if (encryption != nullptr) {
    return encryption->KeyIsLoaded();
  }
  return false;
}
} // encryption
} // comnet