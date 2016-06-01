#ifndef __INTERFACE_MAP_H
#define __INTERFACE_MAP_H

#include <tools/data_structures/interface/Generic.h>

namespace Comnet {
namespace Tools {
namespace DataStructures {
namespace Interface {

/**
   Map type to determine what type of map is being used.
*/
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
template<typename _Ky,
	 typename _Ty>
class Map : public Generic {
  typedef const _Ty& const_value_reference;
  typedef const _Ky& const_key_reference;
public:
  Map() { this->type = MAP; }
  
  virtual ~Map() { }

  int32_t getSize() { return this->size; }
  
  DataStructureType getDataStructType() { return this->type; }

  
  virtual bool insert(const_key_reference key, const_value_reference value) = 0;

  virtual bool remove(const_key_reference key) = 0;

  virtual const_value_reference get(const_key_reference key) = 0;

  virtual bool replace(const_key_reference key, const_value_reference value) = 0;

protected:
  MapType mapType;
};


} // Interface namespace
} // DataStructures namespace
} // Tools namespace
} // Comnet namespace

#endif // __INTERFACE_MAP_H
