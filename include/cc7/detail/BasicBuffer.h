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
#include <cc7/Utilities.h>
#include <iterator>
#include <algorithm>

namespace cc7
{
namespace detail
{
	class BasicBuffer
	{
	public:
		
		typedef cc7::byte			value_type;
		typedef cc7::byte*			pointer;
		typedef const cc7::byte*	const_pointer;
		typedef cc7::byte&			reference;
		typedef const cc7::byte&	const_reference;
		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;
		
		typedef cc7::byte*			iterator;
		typedef const cc7::byte*	const_iterator;
		
		typedef std::reverse_iterator<const_iterator>	const_reverse_iterator;
		typedef std::reverse_iterator<iterator>			reverse_iterator;

		
		static const size_type	npos = static_cast<size_type>(-1);
		
		typedef cc7::detail::ExceptionsWrapper<value_type> _ValueTypeExceptions;
	
	private:
		
		pointer		_bytes;
		size_type	_size;
		size_type	_capacity;

	public:
		
		//
		// Constructors & destructor & move & copy
		//
		
		BasicBuffer() noexcept :
			_bytes(nullptr),
			_size(0),
			_capacity(0)
		{
		}
		
		// Move construct
		BasicBuffer(BasicBuffer&& x) noexcept :
			_bytes(x._bytes),
			_size(x._size),
			_capacity(x._capacity)
		{
			x._bytes = nullptr;
			x._size = 0;
			x._capacity = 0;
		}
		
		// Copy construct
		BasicBuffer(const BasicBuffer & x) :
			_bytes(nullptr),
			_size(0),
			_capacity(0)
		{
			assign(x.data(), x.size());
		}

		// Move assign
		BasicBuffer& operator=(BasicBuffer&& x)
		{
			if (this != &x) {
				// cleanup possible previously allocated resources
				_deallocBuffer(_bytes, _capacity);
				// move members
				_bytes = x._bytes;
				_size  = x._size;
				_capacity = x._capacity;
				// cleanup x
				x._bytes = nullptr;
				x._size  = 0;
				x._capacity = 0;
			}
			return *this;
		}
		
		// Copy assign
		BasicBuffer& operator=(const BasicBuffer& x)
		{
			assign(x.data(), x.size());
			return *this;
		}
		
		// Destruct
		~BasicBuffer() noexcept
		{
			_deallocBuffer(_bytes, _capacity);
		}
		
		
		//
		// Size, length, capacity...
		//
		
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
		
		//
		// MARK: STL : Iterators -
		//
		
		// BasicBuffer supports only (c)begin() & (c)end() iterators
		
		// begin, cbegin
		iterator begin() noexcept
		{
			return _bytes;
		}
		
		const_iterator cbegin() const noexcept
		{
			return _bytes;
		}
		
		// end, cend
		iterator end() noexcept
		{
			return _bytes != nullptr ? _bytes + _size : nullptr;
		}
		
		const_iterator cend() const noexcept
		{
			return _bytes != nullptr ? _bytes + _size : nullptr;
		}
		
		
		// buffer assigning
		
		void assign(const_pointer ptr, size_type size)
		{
			if ((ptr == nullptr) && (size > 0)) {
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
		
		void resize(size_type new_size, const value_type & val)
		{
			if (new_size <= _size) {
				// new size is smaller or equal than current one
				_size = new_size;
			} else {
				growForNewCapacity(new_size);
				for (size_type i = _size; i < new_size; ++i) {
					_bytes[i] = val;
				}
				_size = new_size;
			}
		}

		void resizeNoInit(size_type new_size)
		{
			if (new_size > _size) {
				growForNewCapacity(new_size);
				// keep the rest of memory not initialized
			}
			_size = new_size;
		}

		// Byte access
		
		reference atNoExc(size_type index) noexcept
		{
			if (index < size()) {
				return _bytes[index];
			}
			// Undefined in STL. We're returning reference to static buffer.
			return _ValueTypeExceptions::forbidden_value();
		}
		
		const_reference atNoExc(size_type index) const noexcept
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

		
		void swap(BasicBuffer & other)
		{
			std::swap(_bytes,	 other._bytes);
			std::swap(_size,	 other._size);
			std::swap(_capacity, other._capacity);
		}

		
		// Non-STL methods
		
		void secureClear() noexcept
		{
			_deallocBuffer(_bytes, _capacity);
			
			_bytes		= nullptr;
			_size		= 0;
			_capacity	= 0;
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
		
		static size_type _adjustNewCapacity(size_type old_cap, size_type new_cap) noexcept
		{
			if (old_cap > 0) {
				// new = 1.5 * old
				new_cap = std::max(old_cap + (old_cap >> 1), new_cap);
			}
			// align to 16
			return utilities::AlignValue<16>(new_cap);
		}
	};
	
} // cc7::detail
} // cc7