/*
 * Copyright 2016 Juraj Durech <durech.juraj@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <cc7/Assert.h>
#include <iterator>
#include <algorithm>

namespace cc7
{
	class BasicBuffer
	{
	public:
		
		typedef cc7::byte			value_type;
		typedef cc7::byte*			pointer;
		typedef const cc7::byte*	const_pointer;
		typedef const cc7::byte&	reference;
		typedef const cc7::byte&	const_reference;
		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;
		typedef cc7::byte*			iterator;
		typedef const cc7::byte*	const_iterator;
		
		static const size_type	npos = static_cast<size_type>(-1);
		
		typedef error::Exceptions<value_type> _ValueTypeExceptions;
		
		
		// Constructors & destructor
		
		BasicBuffer() noexcept :
			_bytes(nullptr), _size(0), _capacity(0)
		{
		}
		
		~BasicBuffer() noexcept
		{
			_deallocBuffer(_bytes, _capacity);
		}
		
		// Size, length, capacity...
		
		pointer data() noexcept
		{
			return _bytes;
		}
		
		const_pointer data() const noexcept
		{
			return _bytes;
		}
		
		size_type size() const noexcept
		{
			return _size;
		}
		
		size_type capacity() const noexcept
		{
			return _capacity;
		}
		
		void clear() noexcept
		{
			_size = 0;
		}
		
		
		// buffer assigning
		
		void assign(const_pointer ptr, size_type size)
		{
			if (!ptr && size) {
				_ValueTypeExceptions::invalid_argument();
			} else {
				growForNewCapacity(size);
				memcpy(_bytes, ptr, size);
			}
		}
		
		void reserve(size_type new_size)
		{
			growForNewCapacity(new_size);
		}
		
		// Byte access
		
		reference operator[](size_type index) noexcept
		{
			if (index < size()) {
				return _bytes[index];
			}
			// Undefined in STL. We're returning reference to static buffer.
			return _ValueTypeExceptions::forbidden_value();
		}
		
		const_reference operator[](size_type index) const noexcept
		{
			if (index < size()) {
				return _bytes[index];
			}
			// Undefined in STL. We're returning reference to static buffer.
			return _ValueTypeExceptions::forbidden_value();
		}
		
		reference at(size_type index)
		{
			if (index < size()) {
				return _bytes[index];
			}
			return _ValueTypeExceptions::out_of_range();
		}
		
		const_reference at(size_type index) const
		{
			if (index < size()) {
				return _bytes[index];
			}
			return _ValueTypeExceptions::out_of_range();
		}

		
		
		// Non-STL methods
		
		void safeClear() noexcept
		{
			_deallocBuffer(_bytes, _capacity);
			
			_bytes = nullptr;
			_size = 0;
			_capacity = 0;
		}
		
		int compare(const BasicBuffer & other) const noexcept
		{
			const size_type ts = size();
			const size_type os = other.size();
			const size_type ms = std::min(ts, os);
			int res = memcmp(data(), other.data(), ms);
			if (res == 0) {
				// Converts difference between other and this size to -1 or 1.
				res = (static_cast<int>(
						(os - ts) >> (8 * sizeof(size_type) - 1)) << 1	// 0 or 2, based on signed bit
					   ) - 1;											// -1 or 1
			}
			return res;
		}
		
		pointer growForNewCapacity(size_type new_capacity) noexcept
		{
			if (new_capacity > _capacity) {
				new_capacity = _adjustNewCapacity(_capacity, new_capacity);
				pointer new_bytes = _allocBuffer(new_capacity);
				if (_size > 0) {
					memcpy(new_bytes, _bytes, _size);
				}
				_deallocBuffer(_bytes, _capacity);
				_bytes		= new_bytes;
				_capacity	= new_capacity;
			}
			return _bytes;
		}
		
		pointer shrinkToFit() noexcept
		{
			if (_capacity > _size) {
				size_type new_capacity = _adjustNewCapacity(0, _size);
				pointer new_bytes = _allocBuffer(new_capacity);
				if (_size > 0) {
					memcpy(new_bytes, _bytes, _size);
				}
				_deallocBuffer(_bytes, _capacity);
				_bytes		= new_bytes;
				_capacity	= new_capacity;
			}
			return _bytes;
		}
		
		void swap(BasicBuffer & other)
		{
			std::swap(_bytes,	 other._bytes);
			std::swap(_size,	 other._size);
			std::swap(_capacity, other._capacity);
		}
		
	private:
		
		static pointer _allocBuffer(size_type size)
		{
			return new value_type[size];
		}
		
		static void _deallocBuffer(pointer ptr, size_type size)
		{
			if (ptr) {
				CC7_SecureClean(ptr, size);
				delete [] ptr;
			}
		}
		
		static size_type _adjustNewCapacity(size_type old_cap, size_type new_cap)
		{
			return new_cap;
		}
		
		pointer		_bytes;
		size_type	_size;
		size_type	_capacity;
	};
	
} // cc7