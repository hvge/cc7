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

#include <cc7/BasicBuffer.h>
#include <cc7/ByteRange.h>

namespace cc7
{
	class ByteArray
	{
	private:
		
		BasicBuffer _bb;

	public:
		
		// STL container compatibility
		typedef BasicBuffer::value_type			value_type;
		typedef BasicBuffer::pointer			pointer;
		typedef BasicBuffer::const_pointer		const_pointer;
		typedef BasicBuffer::reference			reference;
		typedef BasicBuffer::const_reference	const_reference;
		typedef BasicBuffer::size_type			size_type;
		typedef BasicBuffer::difference_type	difference_type;
		typedef BasicBuffer::const_iterator		const_iterator;
		typedef BasicBuffer::iterator			iterator;
		
		typedef std::reverse_iterator<const_iterator>	const_reverse_iterator;
		typedef std::reverse_iterator<iterator>			reverse_iterator;
		
		static const size_type	npos = static_cast<size_type>(-1);

		//
		// MARK: STL: Member functions -
		//
		
		// MARK: STL: Constructor
		ByteArray()
		{
		}
		
		// MARK: STL: Assign operator
		

		//
		// MARK: STL : Iterators -
		//

		// TODO: begin
		// TODO: end
		// TODO: rbegin
		// TODO: rend
		// TODO: cbegin
		// TODO: cend
		// TODO: crbegin
		// TODO: crend
		
		//
		// MARK: STL : Capacity -
		//
		
		size_type size() const noexcept
		{
			return _bb.size();
		}
		
		size_type max_size() const noexcept
		{
			return SIZE_T_MAX / 2;		// TODO: this is stupid impl...
		}
		
		void resize (size_type n)
		{
			// TODO...
		}
		
		void resize (size_type n, const value_type& val)
		{
			// TODO...
		}
		
		size_type capacity() const noexcept
		{
			return _bb.capacity();
		}

		bool empty() const noexcept
		{
			return size() == 0;
		}
		
		void reserve (size_type n)
		{
			_bb.growForNewCapacity(n);
		}
		
		void shrink_to_fit()
		{
			_bb.shrinkToFit();
		}
		
		//
		// MARK: STL : Element access -
		//
		
		const_reference operator[](size_type index) const noexcept
		{
			return _bb.operator[](index);
		}
		
		reference operator[](size_type index) noexcept
		{
			return _bb.operator[](index);
		}
		
		const_reference at(size_type index) const
		{
			return _bb.at(index);
		}
		
		reference at(size_type index)
		{
			return _bb.at(index);
		}
		
		reference front() noexcept
		{
			return _bb.operator[](0);
		}
		
		const_reference front() const noexcept
		{
			return _bb.operator[](0);
		}
		
		reference back() noexcept
		{
			return _bb.operator[](_bb.size() - 1);
		}
		
		const_reference back() const noexcept
		{
			return _bb.operator[](_bb.size() - 1);
		}

		const_pointer data() const noexcept
		{
			return _bb.data();
		}
		
		pointer data() noexcept
		{
			return _bb.data();
		}
		
		//
		// MARK: STL: Modiffiers
		//
		
		// TODO: assign
		// TODO: push_back
		// TODO: pop_back
		// TODO: insert
		// TODO: erase
		
		void swap(ByteArray & x)
		{
			_bb.swap(x._bb);
		}
		
		void clear() noexcept
		{
			_bb.clear();
		}
		
		// TODO: emplace
		// TODO: emplace_back

		
		//
		// MARK: Non-STL methods -
		//
		
		// string-like length()
		size_type length() const noexcept
		{
			return _bb.size();
		}

		int compare(const ByteArray & other) const noexcept
		{
			return _bb.compare(other._bb);
		}
		
	};
	
	//
	// MARK: STL: Non-member function overloads -
	//
	
	void swap(ByteArray & x, ByteArray & y)
	{
		x.swap(y);
	}
	
	//
	// MARK: STL : ByteArray relational operators
	//
	
	bool operator==(const ByteArray & x, const ByteArray & y)
	{
		return x.compare(y) == 0;
	}
	bool operator!=(const ByteArray & x, const ByteArray & y)
	{
		return x.compare(y) != 0;
	}
	bool operator< (const ByteArray & x, const ByteArray & y)
	{
		return x.compare(y) > 0;
	}
	bool operator> (const ByteArray & x, const ByteArray & y)
	{
		return x.compare(y) < 0;
	}
	bool operator>=(const ByteArray & x, const ByteArray & y)
	{
		return x.compare(y) >= 0;
	}
	bool operator<=(const ByteArray & x, const ByteArray & y)
	{
		return x.compare(y) <= 0;
	}
	
} // cc7