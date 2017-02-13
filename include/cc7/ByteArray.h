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
#include <cc7/detail/CleanupAllocator.h>

namespace cc7
{
	/**
	 The ByteArray class is a specialized version of std::vector<byte> which
	 implements a secure data cleanup when the object, or it's internal buffer
	 is being destroyed. The purpose of this functionality is a prevention 
	 against the sensitive information leakage.
	 
	 You can typically use this class in cooperation with the ByteRange, where 
	 the ByteRange can be used for data inputs and ByteArray for data outputs
	 in the interfaces.
	 */
	class ByteArray : public std::vector<cc7::byte, detail::CleanupAllocator<cc7::byte>>
	{
	public:
		
		// Make parent_class more accessible in the code.
		typedef std::vector<cc7::byte, detail::CleanupAllocator<cc7::byte>> parent_class;
		
		// Make constructors, assign & insert properly inherited from the vector.
		using parent_class::parent_class;
		using parent_class::assign;
		using parent_class::insert;
		
		/**
		 Constructs an empty ByteArray.
		 */
		ByteArray()
		{
		}
		
		//
		// Interaction with ByteRange class
		//
		
		/**
		 Constructs a byte array with all bytes copied from provided ByteRange |range|.
		 */
		ByteArray(const ByteRange & range) : ByteArray(range.begin(), range.end())
		{
		}
		
		/**
		 Assigns all bytes from provided |range| parameter to this array of bytes.
		 */
		ByteArray& operator=(const ByteRange& range)
		{
			parent_class::assign(range.begin(), range.end());
			return *this;
		}

		/**
		 Assigns all bytes from provided |range| parameter to this array of bytes.
		 */
		void assign(const ByteRange & range)
		{
			parent_class::assign(range.begin(), range.end());
		}
		
		/**
		 Appends all bytes from provided |range| parameter to the end of
		 this array of bytes. Returns a reference to this object.
		 */
		ByteArray & append(const ByteRange & range)
		{
			parent_class::insert(end(), range.begin(), range.end());
			return *this;
		}
		
		/**
		 Inserts all bytes from provided |range| parameter at specific |position|.
		 */
		iterator insert(const_iterator position, const ByteRange & range)
		{
			return parent_class::insert(position, range.begin(), range.end());
		}
		
		/**
		 Returns a new ByteRange object initialized for all bytes stored 
		 in this array.
		 */
		ByteRange byteRange() const
		{
			return ByteRange(data(), size());
		}

		/**
		 Implicit conversion to ByteRange class. The returned ByteRange is initialized
		 for all bytes stored in this array.
		 */
		operator ByteRange () const
		{
			return byteRange();
		}

		
		//
		// Appending
		//
		
		/**
		 Appends an one byte |val| to the end of byte array. The method performs
		 the same operation as push_back(). Returns a reference to this object.
		 */
		ByteArray & append(const value_type& val)
		{
			parent_class::push_back(val);
			return *this;
		}
		
		/**
		 Appends |n| number of bytes to the end of byte array. Each appended byte
		 is initialized to |val|. Returns a reference to this object.
		 */
		ByteArray & append(size_type n, const value_type& val)
		{
			parent_class::insert(end(), n, val);
			return *this;
		}

		// range
		
		/**
		 Appends a range of bytes between the |first| and |last| interator to the end
		 of byte array. Returns a reference to this object.
		 */
		template <class InputIterator>
		ByteArray & append(InputIterator first, InputIterator last)
		{
			parent_class::insert(end(), first, last);
			return *this;
		}

		/**
		 Appends a bytes from provided initializer list. 
		 Returns a reference to this object.
		 */
		ByteArray & append(std::initializer_list<value_type> il)
		{
			parent_class::insert(end(), il);
			return *this;
		}
		
		// append [pointer, size]
		
		/**
		 Appends a |size| bytes starting at pointer |p|, to this byte array.
		 Returns a reference to this object.
		 */
		ByteArray & append(const_pointer p, size_type size)
		{
			parent_class::insert(end(), p, p + size);
			return *this;
		}
		
		//
		// Other methods
		//
		
		/**
		 Just like vector::clear() method, but also securely clears a whole internal
		 allocated buffer of bytes.
		 
		 The method is useful for situations, when you need to wipe out sensitive
		 information from the memory, but don't want to destroy the ByteArray object.
		 */
		void secureClear()
		{
			CC7_SecureClean(data(), capacity());
			parent_class::clear();
		}

		/**
		 Sets an array of bytes encoded in Base64 |base64_string| into this byte array.
		 If |wrap_size| is greater than 0, then the multiline string is returned (check
		 Base64_Encode() for more details). Returns true if opperation succeded and false
		 if the provided string is not a valid Base64 string.
		 */
		bool readFromBase64String(const std::string & base64_string, size_t wrap_size = 0);
		
		/**
		 Sets an array of bytes encoded in hexadecimnal |hex_string| into this byte array.
		 Returns true if opperation succeded and false if the provided string is not a 
		 valid hexadecimal string.
		 */
		bool readFromHexString(const std::string & hex_string);
		
		/**
		 Returns a Base64 encoded string created from all bytes stored in the byte array.
		 If |wrap_size| is greater than 0, then the multiline string is returned (check
		 Base64_Encode() for more details).
		 */
		std::string base64String(size_t wrap_size = 0) const;
		
		/**
		 Returns a hexadecimal string created from all bytes stored in the byte array.
		 If |lower_case| parameter is true then the produced string will contain lower
		 case letters only.
		 */
		std::string hexString(bool lower_case = false) const;
	};
	
	/**
	 Copy conversion from ByteArray to the std::string object.
	 
	 This inline function returns a new instance of std::string object
	 which will be initialized with all bytes from the provided |array|
	 of bytes. The array of bytes is reinterpreted to characters, with
	 no additional conversion.
	 
	 This kind of function is typically useful in situations, when your
	 code is interacting with an another library (or some vintage code)
	 which is using std::string as a general data container. For example,
	 some protobuf implementations is doing this...
	 */
	inline std::string CopyToString(const ByteArray & array)
	{
		return std::string(reinterpret_cast<const char*>(array.data()), array.size());
	}
	
	/**
	 Creates a new ByteRange object from given ByteArray. The method
	 is here just for convenience and for increasing code readability in 
	 some specific situations. 
	 
	 You can also use automatic from ByteArray to ByteRange casting, 
	 ByteArray::byteRange() method or this conversion function to do the same task.
	 It's up to you, which form of conversion between the array and the range is 
	 more familiar with your coding style.
	 */
	inline ByteRange MakeRange(const ByteArray & array)
	{
		return array.byteRange();
	}

	
} // cc7
