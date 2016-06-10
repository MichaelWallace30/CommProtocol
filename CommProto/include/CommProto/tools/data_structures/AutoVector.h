/*
  Wrapper class for standard library vector.

  Copyright (C) 2016  Mario Garcia.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __AUTO_VECTOR_H
#define __AUTO_VECTOR_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>
#include <CommProto/architecture/macros.h>

#include <CommProto/tools/data_structures/interface/InterfaceList.h>
// Standard library vector.
#include <vector>

namespace Comnet {
namespace Tools {
namespace DataStructures {

/**
   AutoVector is a wrapper class for the standard library vector. The intentions 
   of this is so that the vector is not anymore complicated than it should be, since
   the standard library is not at all entirely supported on any platform, we must ensure
   that this wrapper class will garauntee that we are using the vector correctly, without
   having to look through lines of code figuring out why this vector function doesn't work.
*/
template<typename _Ty,
	 typename _Alloc = std::Allocator<_Ty> >
class AutoVector : public Interface::List<_Ty> {
  typedef const _Ty&                                        const_reference;
  typedef const _Ty*                                        const_pointer;
  typedef _Ty*                                              pointer;
  typedef _Ty&                                              reference;
  typedef typename std::vector<_Ty>::iterator               iter;
  typedef typename std::vector<_Ty>::reverse_iterator       reverse_iter;
  typedef typename std::vector<_Ty>::const_reverse_iterator const_reverse_iter;
  typedef typename std::vector<_Ty>::const_iterator         const_iter;
  typedef std::size_t                                       size_type;
public:
  AutoVector(const _Alloc& allocator = Allocator() ) { 
    this->listType = Interface::AUTO_VECTOR;  
    this->size = 0;
    _vector(allocator);
  }
  
  ~AutoVector() { }

  int32_t getSize() {
    return _vector.size();
  }

  bool isEmpty() { 
    return _vector.empty();
  }

  void insert(const_reference value) {
    push_back(value);
  }

  void push_back(const_reference value) {
    _vector.pop_back(value);
  }
  
  void push_back(_Ty&& value) {
    _vector.pop_back(value);
  }

  void pop_back() {
    _vector.pop_back();
  }
  
  bool remove(const_reference value) {
  }
  
  bool removeAt(const int32_t index) {
    bool result = false;
    
    
  }

  reverse_iter rend() {
    return _vector.rend();
  }

  reverse_item rbegin() {
    return _vector.rbegin();
  }

  void swap(AutoVector<_Ty>& other) {
    _vector.swap(other);
  }
  
  reference_type front() {
    return _vector.front();
  }

  size_type max_size() const {
    return _vector.max_size();
  }

  reference_type back() {
    return _vector.back();
  }

  reference_type at(const int32_t index) {
    return _vector.at((size_type)index);
  }
  
  bool contains(const _Ty& value) {
  }

  iter begin() {
    return _vector.begin();
  }
  
  iter end() {
    return _vector.end();
  }
private:
  /**
     The vector we are wrapping.
  */
  std::vector<_Ty> _vector;
};
} // DataStructures namespace
} // Tools namespace
} // Comnet namespace

#endif // __AUTO_VECTOR_H
