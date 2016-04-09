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

#include <cc7/Platform.h>
#include <iterator>

namespace cc7
{
	template <class T> class Range
	{
	public:
		
		// STL container compatibility
		typedef T			value_type;
		typedef T*			pointer;
		typedef const T*	const_pointer;
		typedef const T&	reference;
		typedef const T&	const_reference;
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;
		typedef const T*	const_iterator;
		typedef const T*	iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		
		static const size_type	npos = static_cast<size_type>(-1);

		
		Range() :
			_begin(nullptr),
			_end(nullptr)
		{
		}
		
		Range(const_pointer ptr, size_type count) :
			_begin(ptr),
			_end(ptr != nullptr ? ptr + count : 0)
		{
		}
		
		Range(const_pointer begin, const_pointer end) :
			_begin(begin),
			_end(end)
		{
		}
		
		Range(const Range<T> & r) :
			_begin(r.begin()),
			_end(r.end())
		{
		}
	
		void set(const_pointer begin, const_pointer end)
		{
			_begin = begin;
			_end   = end;
		}
		
		void set(const_pointer ptr, size_type count)
		{
			_begin = ptr;
			_end   = ptr != nullptr ? ptr + count : 0;
		}
		
		void set(const Range<T> & r)
		{
			_begin = r.begin();
			_end   = r.end();
		}
		
		void clear()
		{
			_begin = _end = nullptr;
		}
		
		const T * data() const
		{
			return _begin;
		}
		
		size_type length() const
		{
			return _end - _begin;
		}
		
		size_type size() const
		{
			return _end - _begin;
		}
		
		bool empty() const
		{
			return _begin == _end;
		}
		
		//
		
		void removePrefix(size_t count)
		{
			if (count <= length()) {
				_begin += count;
			}
		}
		
		void removeSuffix(size_t count)
		{
			if (count <= length()) {
				_end -= count;
			}
		}
		
		// STL iterators
		
		const_iterator begin() const
		{
			return _begin;
		}
		
		const_iterator end() const
		{
			return _end;
		}
		
		const_reverse_iterator rbegin() const
		{
			return const_reverse_iterator(_end);
		}
		
		const_reverse_iterator rend() const
		{
			return const_reverse_iterator(_end);
		}
		
		size_type max_size() const
		{
			return size();
		}
		
		size_type capacity() const
		{
			return size();
		}

		
		
	private:
		
		// Private members
		const_pointer _begin;
		const_pointer _end;
	};
	
	typedef Range<uint8_t>	ByteRange;
	typedef Range<char>		StringPiece;
}
