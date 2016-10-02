/*
  Map Interface for Commprotocol.
  
  Copyright (C) 2016  Mario Garcia.

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
#ifndef __INTERFACE_MAP_H
#define __INTERFACE_MAP_H

#include <CommProto/tools/data_structures/interface/generic.h>

namespace comnet {
namespace tools {
namespace datastructures {
namespace interface {

/**
   Map type to determine what type of map is being used.
*/
_COMNET_PRIVATE_API_
enum MapType {
  NONE,
  ORDERED_MAP,
  HASH_TABLE,
  HASH_MAP,
  DICTIONARY,
};

/**
   Map Interface, used for inheritance. Maps are used for fast storage and 
   retrieval, yet they suffer from space use. Best used if you have enough
   memory to hold maps, which shouldn't be a problem.
*/
_COMNET_PRIVATE_API_
template<typename _Ky,
	 typename _Ty>
class Map : public Generic {
  typedef const _Ty& const_value_reference;
  typedef const _Ky& const_key_reference;
  typedef const _Ty* const_value_pointer;
  typedef const _Ky* const_key_pointer;
public:
  /**
     Constructor method for the map.
   */
  Map() { this->type = MAP; }
  
  /**
     Polymorphic Destructor.
   */
  virtual ~Map() { }

  /**
     Get the current number of key-value pairs in the map.
   */
  virtual int32_t GetSize() { return this->size; }
  
  virtual bool IsEmpty() { return (comm_bool)(this->size == 0); }
  /**
     Get the type of map being used.
   */
  DataStructureType GetDataStructType() { return this->type; }

  /**
     Insert a value into the map.
   */
  virtual bool Insert(const_key_reference key, const_value_reference value) = 0;
  /**
     Remove a value from the map.
     @param key 
     @return True, if value was removed, false otherwise.
   */
  virtual bool Remove(const_key_reference key) = 0;

  /**
     Get the value using the key specified.
   */
  virtual const_value_reference Get(const_key_reference key) = 0;
  /**
     Replace a value with the specified key.
   */
  virtual bool Replace(const_key_reference key, const_value_reference value) = 0;

protected:
  /**
     The type of map.
   */
  MapType map_type;
};
} // Interface namespace
} // DataStructures namespace
} // Tools namespace
} // Comnet namespace

#endif // __INTERFACE_MAP_H
