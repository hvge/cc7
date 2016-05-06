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

#include <cc7/ByteRange.h>

namespace cc7
{
	//
	// Temporary, reference implementation of ByteArray.
	//
	
	template <class T> class ByteArrayAllocator : public std::allocator<T>
	{
	public :
		
		template <class U>  struct rebind
		{
			typedef  ByteArrayAllocator <U> other;
		};
		
		ByteArrayAllocator() throw()
		{
		}
		
		ByteArrayAllocator(const ByteArrayAllocator &) throw()
		{
		}
		
		template <class U> ByteArrayAllocator(const ByteArrayAllocator <U> &) throw()
		{
		}

		void deallocate(T * p,  size_t n)
		{
			CC7_SecureClean(p, n * sizeof(T));
			std::allocator <T>::deallocate(p, n);
		}
	};
	
	
	class ByteArray : public std::vector<cc7::byte, ByteArrayAllocator<cc7::byte>>
	{
	public:
		typedef std::vector<cc7::byte, ByteArrayAllocator<cc7::byte>> parent_class;
		using parent_class::parent_class;
		
		//
		// Interaction with ByteRange class
		//
		ByteArray(const ByteRange & range) : ByteArray(range.begin(), range.end())
		{
		}

		ByteArray& operator=(const ByteRange& range)
		{
			parent_class::assign(range.begin(), range.end());
			return *this;
		}
		
		void assign(const ByteRange & range)
		{
			parent_class::assign(range.begin(), range.end());
		}
		
		ByteArray & append(const ByteRange & range)
		{
			parent_class::insert(end(), range.begin(), range.end());
			return *this;
		}
		
		iterator insert(const_iterator position, const ByteRange & range)
		{
			return parent_class::insert(position, range.begin(), range.end());
		}
		
		ByteRange byteRange() const
		{
			return ByteRange(data(), size());
		}

		// dirty.. automatic casting to ByteRange
		operator ByteRange () const
		{
			return ByteRange(data(), size());
		}

		
		
		
		//
		// Appending
		//
		
		// single element, the same as push_back()
		ByteArray & append(const value_type& val)
		{
			parent_class::push_back(val);
			return *this;
		}
		
		// fill
		ByteArray & append(size_type n, const value_type& val)
		{
			parent_class::insert(end(), n, val);
			return *this;
		}

		// range
		template <class InputIterator>
		ByteArray & append(InputIterator first, InputIterator last)
		{
			parent_class::insert(end(), first, last);
			return *this;
		}

		// initializer list
		ByteArray & append(std::initializer_list<value_type> il)
		{
			parent_class::insert(end(), il);
			return *this;
		}
		
		//
		// Other custom methods
		//
		
		void secureClear()
		{
			CC7_SecureClean(data(), capacity());
			parent_class::clear();
		}
	
	};

	
} // cc7