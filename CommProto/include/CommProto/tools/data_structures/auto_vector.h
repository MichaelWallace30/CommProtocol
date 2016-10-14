/*
  Wrapper class for standard library vector.

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
#ifndef __AUTO_VECTOR_H
#define __AUTO_VECTOR_H

#include <CommProto/architecture/os/include_defines.h>
#include <CommProto/architecture/api.h>
#include <CommProto/architecture/macros.h>

#include <CommProto/tools/data_structures/interface/interface_list.h>
// Standard library vector.
#include <vector>
#include <algorithm>

namespace comnet {
namespace tools {
namespace datastructures {

/**
   AutoVector is a wrapper class for the standard library vector. The intentions 
   of this is so that the vector is not anymore complicated than it should be, since
   the standard library is not At all entirely supported on any platform, we must ensure
   that this wrapper class will garauntee that we are using the vector correctly, without
   having to look through lines of code figuring out why this vector function doesn't work.
   Not only this, AutoVector inherits from InterfaceList, so now vector can be related to all 
   other lists within this library.
*/
template<typename _Ty,
	 typename _Alloc = std::allocator<_Ty> >
class COMM_EXPORT AutoVector : public interface::List<_Ty> {
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
  /**
     Default constructor for the standard vector in c++.
   */
  AutoVector(const _Alloc& allocator = _Alloc() ) 
    : _vector(std::vector<_Ty, _Alloc>(allocator))
  { 
    this->list_type = interface::AUTO_VECTOR;  
    this->size = 0;
  }

  /**
     Set the size of the vector during construction.
   */
  AutoVector(size_type count)
    : _vector(std::vector<_Ty, _Alloc>(count))
  {
    this->list_type = interface::AUTO_VECTOR;
    this->size = 0;
  }

  /**
     Set Size and allocator for during construction of the vector.
   */
  AutoVector(size_type count, const _Alloc& allocator = _Alloc() ) 
    : _vector(std::vector<_Ty, _Alloc>(count, allocator) )
  {
    this->list_type = interface::AUTO_VECTOR;
    this->size = 0;
  }

  /**
     Copy constructor to this vector wrapper.
   */
  AutoVector(const AutoVector<_Ty, _Alloc>& other) 
    : _vector(std::vector<_Ty, _Alloc>(other._vector) )
  {
    this->list_type = interface::AUTO_VECTOR;
    this->size = 0;
  }

  /**
     Destructor for the Vector and AutoVector class.
   */
  ~AutoVector() { }

  /**
     Get the size of this vector.
   */
  int32_t GetSize() {
    return _vector.size();
  }

  /**
     Check if this Auto Vector is empty.
   */
  bool IsEmpty() { 
    return _vector.empty();
  }

  void Insert(const_reference value) {
    push_back(value);
  }
  /**
     Push a value At the End of this AutoVector.
   */
  void push_back(const_reference value) {
    _vector.push_back(value);
  }
  /**
     Move a value into the Back of this AutoVector.
   */
  void push_back(_Ty&& value) {
    _vector.push_back(value);
  }

  /**
     Pop a value from the Back of this Vector.
   */
  void pop_back() {
    _vector.pop_back();
  }
  
  /**
     Remove a value from the vector. This value is specified by the user.
   */
  bool Remove(const_reference value) {
    if (_vector.empty()) {
      return false;
    }
   
    bool result = false;
    iter pos = std::find(_vector.begin(), _vector.end(), value);

    if (pos != _vector.end()) {
      _vector.erase(pos);
      result = true;
    }

    return result;
  }
  /**
     Remove a value from the specified index. We should probably check if index goes over the size.
   */
  bool RemoveAt(const int32_t index) {
    bool result = false;
    iter pos = _vector.erase(_vector.begin() + index);
    if (pos != _vector.end()) {
      result = true;
    }
    return result;
  }

  /**
     Get the reverse End iterator for this vector.
   */
  reverse_iter rend() {
    return _vector.rend();
  }
  /**
     Get the reverse Begin iterator for this vector.
   */
  reverse_iter rbegin() {
    return _vector.rbegin();
  }
  /**
     Swap this vector with another.
   */
  void swap(AutoVector<_Ty>& other) {
    _vector.swap(other._vector);
  }

  /**
     Swap this vector with another vector that is not wrapped with AutoVector.
   */
  void swap(std::vector<_Ty, _Alloc> other) {
    _vector.swap(other);
  }
  
  /**
     Grab the Front value of this vector.
   */
  reference Front() {
    return _vector.front();
  }
  /**
     Grab the max size that this vector holds.
   */
  size_type MaxSize() const {
    return _vector.max_size();
  }
  /**
     Get the Back value of this vector.
   */
  reference Back() {
    return _vector.back();
  }
  /**
     Grab a value At the specified index. 
   */
  reference At(const int32_t index) {
    return _vector.at((size_type)index);
  }
  
  /**
     Check if this vector Contains the said value.
   */
  bool Contains(const _Ty& value) {
    bool result = false;
    if (_vector.empty()) {
      return result;
    }

    iter pos = std::find(_vector.begin(), _vector.end(), value);

    if (pos != _vector.end()) {
      result = true;
    }

    return result;
  }

  /**
     Grab the beginning iterator of this vector.
   */
  iter Begin() {
    return _vector.begin();
  }
  /**
     Grab the End iterator of this vector.
   */
  iter End() {
    return _vector.end();
  }

protected:
  /**
     The vector we are wrapping.
  */
  std::vector<_Ty, _Alloc> _vector;
};
} // DataStructures namespace
} // Tools namespace
} // Comnet namespace

#endif // __AUTO_VECTOR_H
