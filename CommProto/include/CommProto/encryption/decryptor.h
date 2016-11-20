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
#ifndef __COMM_DECRYPTOR_H
#define __COMM_DECRYPTOR_H


#include <CommProto/encryption/encryptor.h>


namespace comnet {
namespace encryption {


/**
  Modular Decryptor that acts as a lightweight decryptor.
  Observer to Encryptor.
*/
class COMM_EXPORT CommDecryptor {
  COMM_DISALLOW_COPYING(CommDecryptor);
public:
  CommDecryptor();
  CommDecryptor(CryptProtocol proto);
  CommDecryptor(CryptProtocol proto, CommEncryptor* encryptor);
  CommDecryptor(CommDecryptor&& decrypt);
  CommDecryptor& operator=(CommDecryptor&& decrypt);
  ~CommDecryptor();

  uint8_t LoadKey(char* key);
  uint8_t LoadKeyFromFile(char* filename_key);
  int32_t Decrypt(comnet::serialization::ObjectStream* obj);

  CommEncryptor* GetEncryptor() { return encryptor; }
  void LinkEncryptor(CommEncryptor* encrypt) { encryptor = encrypt; }
  CryptProtocol GetEncryptionType() { return protocol; }
  bool KeyIsLoaded();
private:
  void Setup();
  std::shared_ptr<EncryptionInterface> encryption;
  CommEncryptor* encryptor;
  CryptProtocol protocol;

  friend class CommEncryptor;
};
} // encryption
} // comnet
#endif // __COMM_DECRYPTOR_H