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
	
	// Temporary, reference implementation of ByteArray.
	
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
	
	
	class ByteArray : public std::vector<uint8_t, ByteArrayAllocator<uint8_t>>
	{
	public:
		typedef std::vector<uint8_t, ByteArrayAllocator<uint8_t>> parent_class;
		using parent_class::parent_class;
		
	};

	
} // cc7