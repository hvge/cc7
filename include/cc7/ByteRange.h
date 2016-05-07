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
#include <string>
#include <vector>

namespace cc7
{	
	class ByteRange
	{
	public:
		
		// STL container compatibility
		typedef cc7::byte			value_type;
		typedef cc7::byte*			pointer;
		typedef const cc7::byte*	const_pointer;
		typedef const cc7::byte&	reference;
		typedef const cc7::byte&	const_reference;
		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;
		typedef cc7::byte*			iterator;
		typedef const cc7::byte*	const_iterator;		
		typedef std::reverse_iterator<const_iterator>	const_reverse_iterator;
		typedef std::reverse_iterator<iterator>			reverse_iterator;
		
		static const size_type	npos = static_cast<size_type>(-1);
		
		typedef cc7::detail::ExceptionsWrapper<value_type> _ValueTypeExceptions;
		typedef cc7::detail::ExceptionsWrapper<ByteRange>  _ByteRangeExceptions;
		
	private:
		
		// Private members
		const_pointer _begin;
		const_pointer _end;
		
	public:
		
		// Constructors
		
		ByteRange() noexcept :
			_begin (nullptr),
			_end   (nullptr)
		{
		}
		
		ByteRange(const_pointer ptr, size_type count) noexcept :
			_begin (ptr),
			_end   (ptr != nullptr ? ptr + count : 0)
		{
		}
		
		ByteRange(const_pointer begin, const_pointer end) :
			_begin (begin),
			_end   (end)
		{
			_validateBeginEnd(begin, end);
		}
		
		ByteRange(const ByteRange & r) noexcept :
			_begin (r.begin()),
			_end   (r.end())
		{
		}
		
		ByteRange(const void * ptr, size_type size) noexcept :
			_begin (reinterpret_cast<const_pointer>(ptr)),
			_end   (_begin ? _begin + size : nullptr)
		{
		}
		
		ByteRange(const std::string & str) noexcept :
			_begin (reinterpret_cast<const_pointer>(str.data())),
			_end   (reinterpret_cast<const_pointer>(str.data()) + str.length())
		{
		}
		
		ByteRange(const char * c_str) noexcept :
			_begin (reinterpret_cast<const_pointer>(c_str)),
			_end   (_begin ? _begin + strlen(c_str) : nullptr)
		{
		}
		
		// assign methods
	
		void assign(const_pointer begin, const_pointer end)
		{
			_validateBeginEnd(begin, end);
			_begin = begin;
			_end   = end;
		}
		
		void assign(const_pointer ptr, size_type count) noexcept
		{
			_begin = ptr;
			_end   = _begin != nullptr ? _begin + count : nullptr;
		}
		
		void assign(const ByteRange & r) noexcept
		{
			_begin = r.begin();
			_end   = r.end();
		}
		
		void assign(const void * ptr, size_type size) noexcept
		{
			_begin = reinterpret_cast<const_pointer>(ptr);
			_end   = _begin ? _begin + size : nullptr;
		}
		
		void assign(const std::string & str) noexcept
		{
			_begin = reinterpret_cast<const_pointer>(str.data());
			_end   = _begin + str.size();
		}
		
		void assign(const char * c_str) noexcept
		{
			_begin = reinterpret_cast<const_pointer>(c_str);
			_end  = _begin ? _begin + strlen(c_str) : nullptr;
		}
		
		void assign(const char * c_str, size_type size) noexcept
		{
			_begin = reinterpret_cast<const_pointer>(c_str);
			_end   = _begin ? _begin + size : nullptr;
		}
		
		
		// other methods
		
		void clear() noexcept
		{
			_begin = nullptr;
			_end   = nullptr;
		}
		
		const_pointer data() const noexcept
		{
			return _begin;
		}
		
		size_type length() const noexcept
		{
			return _end - _begin;
		}
		
		size_type size() const noexcept
		{
			return _end - _begin;
		}
		
		size_type capacity() const noexcept
		{
			return size();
		}

		size_type max_size() const noexcept
		{
			return size();
		}
		
		bool empty() const noexcept
		{
			return _begin == _end;
		}
		
		// Getting elements
			
		const_reference operator[](size_type index) const noexcept
		{
			if (index < size()) {
				return _begin[index];
			}
			// Accessing element which is out of range has undefined behavior in STL.
			// We can return reference to some static buffer.
			return _ValueTypeExceptions::forbidden_value();
		}
		
		const_reference at(size_type index) const
		{
			if (index < size()) {
				return _begin[index];
			} else {
				return _ValueTypeExceptions::out_of_range();
			}
		}
		
		// STL iterators
		
		const_iterator begin() const
		{
			return cbegin();
		}
		
		const_iterator end() const
		{
			return cend();
		}
		
		const_iterator cbegin() const
		{
			return _begin;
		}
		
		const_iterator cend() const
		{
			return _end;
		}
		
		const_reverse_iterator rbegin() const
		{
			return crbegin();
		}
		
		const_reverse_iterator rend() const
		{
			return crend();
		}
		
		const_reverse_iterator crbegin() const
		{
			return const_reverse_iterator(_end);
		}
		
		const_reverse_iterator crend() const
		{
			return const_reverse_iterator(_end);
		}
		
		// Non-STL methods
		
		// Prefix / Suffix remove, SubRange
		void removePrefix(size_t count)
		{
			if (count <= length()) {
				_begin += count;
			} else {
				_ValueTypeExceptions::out_of_range();
			}
		}
			
		void removeSuffix(size_t count)
		{
			if (count <= length()) {
				_end -= count;
			} else {
				_ValueTypeExceptions::out_of_range();
			}
		}
		
		ByteRange subRangeFrom(size_type from) const
		{
			if (from <= size()) {
				return ByteRange(begin() + from, end());
			}
			return _ByteRangeExceptions::out_of_range();
		}

		ByteRange subRangeTo(size_type to) const
		{
			if (to <= size()) {
				return ByteRange(begin(), begin() + to);
			}
			return _ByteRangeExceptions::out_of_range();
		}
		
		ByteRange subRange(size_type from, size_type count)
		{
			if ((from <= size()) && (from + count <= size())) {
				return ByteRange(begin() + from, count);
			}
			return _ByteRangeExceptions::out_of_range();
		}
			
		int compare(const ByteRange & other) const noexcept
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
		
	protected:
			
		void _validateBeginEnd(const_pointer begin, const_pointer end)
		{
			if ((begin > end) || (!begin && end)) {
				_ValueTypeExceptions::invalid_argument();
			}
		}
			
	};
		
	// ByteRange comparation operators
	
	inline bool operator==(const ByteRange & x, const ByteRange & y)
	{
		return x.compare(y) == 0;
	}
	inline bool operator!=(const ByteRange & x, const ByteRange & y)
	{
		return x.compare(y) != 0;
	}
	inline bool operator< (const ByteRange & x, const ByteRange & y)
	{
		return x.compare(y) > 0;
	}
	inline bool operator> (const ByteRange & x, const ByteRange & y)
	{
		return x.compare(y) < 0;
	}
	inline bool operator>=(const ByteRange & x, const ByteRange & y)
	{
		return x.compare(y) >= 0;
	}
	inline bool operator<=(const ByteRange & x, const ByteRange & y)
	{
		return x.compare(y) <= 0;
	}
	
} // cc7
