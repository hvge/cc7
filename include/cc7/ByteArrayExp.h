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
#include <cc7/detail/BasicBuffer.h>

namespace cc7
{
#ifndef CC7_USE_EXPERIMENTAL_BYTEARRAY
#error Experimental feature, should not be used.
#endif
	
	/**
	 The ByteArray is a sequence container representing array of bytes that can
	 change in size. Just like std::vector, ByteArray uses contiguous storage 
	 for its bytes, which means that its content can also be accessed using
	 offsets on regular pointers to its content.
	 
	 Unlike the std::vector, the ByteArray doesn't leave traces of the content,
	 previously stored in the array. In other words, the ByteArray always clears
	 content of all dynamically allocated buffers, before the deallocation.
	 
	 
	 Note for developers
	 
	 The ByteArray is still under the heave development, but the goal is to
	 have a secure byte container, compatible as much as possible with the STL
	 infrastructure, easily exchangable with a regular std::vector<U8>. So, this 
	 means that every missing method, or an incompatibility with the STL, is 
	 treated as a bug and will be fixed as soon as possible.
	 
	 */
	class ByteArray
	{
	private:
		
		detail::BasicBuffer _bb;

	public:
		
		// STL container compatibility
		typedef detail::BasicBuffer::value_type			value_type;
		typedef detail::BasicBuffer::pointer			pointer;
		typedef detail::BasicBuffer::const_pointer		const_pointer;
		typedef detail::BasicBuffer::reference			reference;
		typedef detail::BasicBuffer::const_reference	const_reference;
		typedef detail::BasicBuffer::size_type			size_type;
		typedef detail::BasicBuffer::difference_type	difference_type;
		typedef detail::BasicBuffer::const_iterator		const_iterator;
		typedef detail::BasicBuffer::iterator			iterator;
		
		typedef detail::BasicBuffer::const_reverse_iterator		const_reverse_iterator;
		typedef detail::BasicBuffer::reverse_iterator			reverse_iterator;
		
		static const size_type	npos = static_cast<size_type>(-1);

		//
		// MARK: STL: Member functions -
		//
		
		// MARK: STL: Constructor -
		
		/**
		 Constructs an empty byte array, with no bytes.
		 */
		ByteArray() = default;
		/**
		 Constructs a byte array filled with |n| zero bytes.
		 */
		explicit ByteArray(size_type n)
		{
			_bb.resize(n, 0);
		}
		/**
		 Constructs a byte array filled with |n| bytes with value |val|
		 */
		explicit ByteArray(size_type n, const value_type& val)
		{
			_bb.resize(n, val);
		}
		
		// TODO: template <class InputIterator>  vector (InputIterator first, InputIterator last);
		
		/**
		 Constructs a byte array with a copy of all bytes in |x|, in the same order.
		 */
		ByteArray(const ByteArray& x) = default;
		/**
		 Constructs a byte array that acquires content of |x|.
		 The source |x| is left in an unspecified but valid state.
		 */
		ByteArray(ByteArray&& x) = default;
		/**
		 Constructs a byte array with a copy of all bytes in |il|, in the same order.
		 */
		ByteArray(std::initializer_list<value_type> il)
		{
			assignWithInitializerList(il);
		}
		
		
		// MARK: STL: Destructor -
		
		/**
		 Destroys the byte array object.
		 */
		~ByteArray() = default;
		
		
		
		// MARK: STL: Assign operator -
		
		/**
		 Copies all the bytes from |x| into the array with |x| preserving its contents.
		 */
		ByteArray& operator=(const ByteArray& x) = default;
		/**
		 Moves the content of |x| into the array. |x| is left in an unspecified 
		 but valid state.
		 */
		ByteArray& operator=(ByteArray&& x) = default;
		/**
		 Copies the content of |il| into the byte array.
		 */
		ByteArray& operator=(std::initializer_list<value_type> il)
		{
			assignWithInitializerList(il);
			return *this;
		}
		
		//
		// MARK: STL : Iterators -
		//

		/**
		 Returns an iterator pointing to the first byte in the byte array.
		 */
		iterator begin() noexcept
		{
			return _bb.begin();
		}
		/**
		 Returns a const iterator pointing to the first byte in the byte array.
		 */
		const_iterator begin() const noexcept
		{
			return _bb.cbegin();
		}
		/**
		 Returns a const iterator pointing to the first byte in the byte array.
		 */
		const_iterator cbegin() const noexcept
		{
			return _bb.cbegin();
		}
		
		/**
		 Returns an iterator referring to the past-the-end byte in the byte array.
		 */
		iterator end() noexcept
		{
			return _bb.end();
		}
		/**
		 Returns a const iterator referring to the past-the-end byte in the byte array.
		 */
		const_iterator end() const noexcept
		{
			return _bb.cend();
		}
		/**
		 Returns a const iterator referring to the past-the-end byte in the byte array.
		 */
		const_iterator cend() const noexcept
		{
			return _bb.cend();
		}
		
		/**
		 Returns a reverse iterator pointing to the last byte in the byte array.
		 */
		reverse_iterator rbegin() noexcept
		{
			return reverse_iterator(_bb.end());
		}
		/**
		 Returns a const reverse iterator pointing to the last byte in the byte array.
		 */
		const_reverse_iterator rbegin() const noexcept
		{
			return const_reverse_iterator(_bb.cend());
		}
		/**
		 Returns a const reverse iterator pointing to the byte in the byte array.
		 */
		const_reverse_iterator crbegin() const noexcept
		{
			return const_reverse_iterator(_bb.cend());
		}

		/**
		 Returns a reverse iterator pointing to the theoretical element preceding 
		 the first byte in the array (which is considered its reverse end).
		 */
		reverse_iterator rend() noexcept
		{
			return reverse_iterator(_bb.begin());
		}
		/**
		 Returns a const reverse iterator pointing to the theoretical element preceding
		 the first byte in the array (which is considered its reverse end).
		 */
		const_reverse_iterator rend() const noexcept
		{
			return const_reverse_iterator(_bb.cbegin());
		}
		/**
		 Returns a const reverse iterator pointing to the theoretical element preceding
		 the first byte in the array (which is considered its reverse end).
		 */
		const_reverse_iterator crend() const noexcept
		{
			return const_reverse_iterator(_bb.cbegin());
		}

		//
		// MARK: STL : Capacity -
		//
		
		/**
		 Returns the number of bytes in the array.
		 */
		size_type size() const noexcept
		{
			return _bb.size();
		}
		/**
		 Returns the number of bytes in the array.
		 */
		size_type length() const noexcept
		{
			return _bb.size();
		}
		/**
		 Returns the maximum number of bytes that the array can hold. The returned value 
		 is theoretical and is implemented only for compatibility reasons.
		 */
		size_type max_size() const noexcept
		{
			return SIZE_T_MAX / 2;
		}
		/**
		 Resizes the array so that it contains |n| bytes. If |n| is smaller than the current 
		 array size, the content is reduced to its first |n| bytes. If |n| is greater than the
		 current array size, the content is expanded by inserting at the end as many zero bytes
		 as needed to reach a size of |n|.
		 */
		void resize(size_type n)
		{
			_bb.resize(n, 0);
		}
		/**
		 Resizes the array so that it contains |n| bytes. If |n| is smaller than the current
		 array size, the content is reduced to its first |n| bytes. If |n| is greater than the
		 current array size, the content is expanded by inserting at the end as many |val| bytes
		 as needed to reach a size of |n|.
		 */
		void resize(size_type n, const value_type& val)
		{
			_bb.resize(n, val);
		}
		/**
		 Returns the size of the storage space currently allocated for the byte array.
		 */
		size_type capacity() const noexcept
		{
			return _bb.capacity();
		}
		/**
		 Returns whether the byte array is empty.
		 */
		bool empty() const noexcept
		{
			return size() == 0;
		}
		/**
		 Requests that the byte array capacity be at least enough to contain |n| bytes.
		 */
		void reserve(size_type n)
		{
			_bb.reserve(n);
		}
		/**
		 Requests the byte array to reduce its capacity to fit its size.
		 */
		void shrink_to_fit()
		{
			_bb.shrinkToFit();
		}
		
		//
		// MARK: STL : Element access -
		//
		
		/**
		 Returns a const reference to the byte at position |n| in the byte array.
		 The operator implements bounds checking and therefore is always safe. 
		 If |n| is greater than the current array size, then the reference to an
		 internal static byte is returned. In this case, the referenced byte has 
		 undefined value.
		 */
		const_reference operator[](size_type n) const noexcept
		{
			return _bb.atNoExc(n);
		}
		/**
		 Returns a reference to the byte at position |n| in the byte array.
		 The operator implements bounds checking and therefore is always safe.
		 If |n| is greater than the current array size, then the reference to an
		 internal static byte is returned. In this case, the referenced byte has
		 undefined value.
		 */
		reference operator[](size_type n) noexcept
		{
			return _bb.atNoExc(n);
		}
		/**
		 Returns a const reference to the byte at position |n| in the byte array.
		 The method has similar behavior to operator[], except that this method
		 throws an out_of_range exception, if |n| is greater that the current
		 array size.
		 */
		const_reference at(size_type n) const
		{
			return _bb.at(n);
		}
		/**
		 Returns a reference to the byte at position |n| in the byte array.
		 The method has similar behavior to operator[], except that this method
		 throws an out_of_range exception, if |n| is greater that the current
		 array size.
		 */
		reference at(size_type n)
		{
			return _bb.at(n);
		}
		/**
		 Returns a reference to the first byte in the array.
		 Calling this method on an empty array is always safe and the
		 reference to an internal static byte is returned.
		 */
		reference front() noexcept
		{
			return _bb.atNoExc(0);
		}
		/**
		 Returns a const reference to the first byte in the array.
		 Calling this method on an empty array is always safe and the
		 reference to an internal static byte is returned.
		 */
		const_reference front() const noexcept
		{
			return _bb.atNoExc(0);
		}
		/**
		 Returns a reference to the last byte in the array.
		 Calling this method on an empty array is always safe and the
		 reference to an internal static byte is returned.
		 */
		reference back() noexcept
		{
			return _bb.atNoExc(_bb.size() - 1);
		}
		/**
		 Returns a const reference to the last byte in the array.
		 Calling this method on an empty array is always safe and the
		 reference to an internal static byte is returned.
		 */
		const_reference back() const noexcept
		{
			return _bb.atNoExc(_bb.size() - 1);
		}
		/**
		 Returns a direct const pointer to the memory array used internally by
		 the byte array to store its owned bytes.
		 */
		const_pointer data() const noexcept
		{
			return _bb.data();
		}
		/**
		 Returns a direct pointer to the memory array used internally by
		 the byte array to store its owned bytes.
		 */
		pointer data() noexcept
		{
			return _bb.data();
		}
		
		//
		// MARK: STL: Modiffiers
		//
		
		// TODO: template <class InputIterator>  void assign (InputIterator first, InputIterator last);
		
		/**
		 Assigns new contents to the array, replacing its current contents, and modifying
		 its size accordingly. In the fill version, the array is filled with |n| bytes, each
		 initialized to |val|.
		 */
		void assign(size_type n, const value_type& val)
		{
			_bb.resize(n, val);
		}
		/**
		 Assigns new contents to the array, replacing its current contents, and modifying
		 its size accordingly. In the initializer list version, the new bytes are copies
		 of the values passed as initializer list |il|, in the same order.
		 */
		void assign(std::initializer_list<value_type> il)
		{
			assignWithInitializerList(il);
		}
		
		/**
		 Adds a new byte at the end of the array, after its current last byte.
		 */
		void push_back(const value_type& val)
		{
			const size_type old_size = _bb.size();
			_bb.resizeNoInit(old_size + 1);
			_bb.atNoExc(old_size) = val;
		}
		/**
		 Adds a new byte at the end of the array, after its current last byte.
		 */
		void push_back(const value_type&& val)
		{
			const size_type old_size = _bb.size();
			_bb.resizeNoInit(old_size + 1);
			_bb.atNoExc(old_size) = val;
		}
		/**
		 Removes the last byte in the array, effectively reducing the container size by one.
		 */
		void pop_back()
		{
			if (_bb.size() > 0) {
				_bb.resizeNoInit(_bb.size() - 1);
			}
		}
		
		// TODO: insert
		// TODO: erase
		
		/**
		 Exchanges the content of the byte array by the content of |x|. Sizes may differ.
		 */
		void swap(ByteArray & x)
		{
			_bb.swap(x._bb);
		}
		/**
		 Removes all bytes from the array, leaving the array with a size of 0.
		 The method doesn't perform secure clear of stored bytes.
		 */
		void clear() noexcept
		{
			_bb.clear();
		}
		/**
		 Removes all bytes from the array, leaving the array with a size of 0.
		 The method also performs a secure clear of stored bytes.
		 */
		void secureClear() noexcept
		{
			_bb.secureClear();
		}

		
		// TODO: emplace
		// TODO: emplace_back

		
		//
		// MARK: Non-STL methods -
		//
		
		/**
		 Comapares the content of this byte array to |other| array. If |other| is smaller than
		 this array, returns integer greater than 0. If |other| is greater, then returns negative
		 integer. If both arrays are identical, then returns zero.
		 */
		int compare(const ByteArray & other) const noexcept
		{
			return _bb.compare(other._bb);
		}
		
		
	private:
		
		void assignWithInitializerList(const std::initializer_list<value_type> & il)
		{
			_bb.resizeNoInit(il.size());
			auto it = il.begin();
			auto ptr = _bb.data();
			while (it != il.end()) {
				*ptr = *it;
				++it;
				++ptr;
			}
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
	
	inline bool operator==(const ByteArray & x, const ByteArray & y)
	{
		return x.compare(y) == 0;
	}
	inline bool operator!=(const ByteArray & x, const ByteArray & y)
	{
		return x.compare(y) != 0;
	}
	inline bool operator< (const ByteArray & x, const ByteArray & y)
	{
		return x.compare(y) > 0;
	}
	inline bool operator> (const ByteArray & x, const ByteArray & y)
	{
		return x.compare(y) < 0;
	}
	inline bool operator>=(const ByteArray & x, const ByteArray & y)
	{
		return x.compare(y) >= 0;
	}
	inline bool operator<=(const ByteArray & x, const ByteArray & y)
	{
		return x.compare(y) <= 0;
	}
	
} // cc7