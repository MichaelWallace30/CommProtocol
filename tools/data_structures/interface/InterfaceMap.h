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
  int32_t getSize() { return this->size; }
  
  /**
     Get the type of map being used.
   */
  DataStructureType getDataStructType() { return this->type; }

  /**
     Insert a value into the map.
   */
  virtual bool insert(const_key_reference key, const_value_reference value) = 0;
  /**
     Remove a value from the map.
     @param key 
     @return True, if value was removed, false otherwise.
   */
  virtual bool remove(const_key_reference key) = 0;

  /**
     Get the value using the key specified.
   */
  virtual const_value_reference get(const_key_reference key) = 0;
  /**
     Replace a value with the specified key.
   */
  virtual bool replace(const_key_reference key, const_value_reference value) = 0;

protected:
  /**
     The type of map.
   */
  MapType mapType;
};
} // Interface namespace
} // DataStructures namespace
} // Tools namespace
} // Comnet namespace

#endif // __INTERFACE_MAP_H
