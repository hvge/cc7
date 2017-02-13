/*
 * Copyright 2017 Juraj Durech <durech.juraj@gmail.com>
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

namespace cc7
{
namespace detail
{
	/**
	 The CleanupAllocator is a special type of std::allocator, which only purpose
	 is to wipeout the content of the allocated memory before the deallocation.
	 
	 Important security notice
	 
	 Note that this allocator doesn't always guarantee the secure data cleanup on
	 all platforms. For example, if the container implements SBO (Small Buffer 
	 Optimization), then the internal buffer is typically not always allocated with
	 using the std::allocator. This behavior depends between the STL, so if you're going
	 to add support for a new platform, then you have to be very sure, how the containers
	 on that plaform behave for a small amounts of allocated memory.
	 
	 This is actually the main reason, why the cc7 developers prefers compiling
	 the library against LLVM's libc++, instead of another STL implementations. 
	 With minimizing the number of officially supported compilers & system libraries,
	 we can keep the library as simple as possible.
	 */
	template <class T> class CleanupAllocator : public std::allocator<T>
	{
	public :
		
		template <class U> struct rebind
		{
			typedef CleanupAllocator <U> other;
		};
		
		CleanupAllocator() throw()
		{
		}
		
		CleanupAllocator(const CleanupAllocator &) throw()
		{
		}
		
		template <class U> CleanupAllocator(const CleanupAllocator <U> &) throw()
		{
		}
		
		void deallocate(T * p,  size_t n)
		{
			CC7_SecureClean(p, n * sizeof(T));
			std::allocator <T>::deallocate(p, n);
		}
	};
	
} // cc7::detail
} // cc7
