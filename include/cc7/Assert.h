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

#if !defined(CC7_NO_EXCEPTIONS)
#include <stdexcept>
#include <new>
#endif

namespace cc7
{
namespace error
{
#if defined(CC7_NO_EXCEPTIONS)
	#define __CC7_THROW_EXCEPTION(exception, text)		CC7_ASSERT(false, text)
	#define __CC7_THROW_EXCEPTION_NOP(exception, text)	CC7_ASSERT(false, text)
#else
	#define __CC7_THROW_EXCEPTION(exception, text)		throw exception(text)
	#define __CC7_THROW_EXCEPTION_NOP(exception, text)	throw exception()
#endif

	template <class T> class Exceptions {
	public:
		
		static T & out_of_range()
		{
			__CC7_THROW_EXCEPTION(std::out_of_range, "out of range");
			return _foo;
		}
		
		static T & invalid_argument()
		{
			__CC7_THROW_EXCEPTION(std::invalid_argument, "invalid argument");
			return _foo;
		}
		
		static T & length_error()
		{
			__CC7_THROW_EXCEPTION(std::length_error, "length error");
			return _foo;
		}
		
		static T & allocation_error()
		{
			__CC7_THROW_EXCEPTION_NOP(std::bad_alloc, "not enough memory");
			return _foo;
		}
		
		static T & forbidden_value()
		{
			CC7_ASSERT(false, "Returning forbidden value.");
			return _foo;
		}
		
	private:
		static T _foo;
	};

} // cc7::error
} // cc7
