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

namespace cc7
{
	//
	// Endian swap
	//
	
	namespace detail
	{
		// TODO: use some std implementations or intrinsics
		inline U16 SwapEndian(U16 n)
		{
			return	(n>>8) |
					(n<<8);
		}
		
		inline U32 SwapEndian(U32 n)
		{
			return	( n>>24) |
					((n<<8)  & 0x00FF0000) |
					((n>>8)  & 0x0000FF00) |
					( n<<24);
		}
		
		inline U64 SwapEndian(U64 n)
		{
			return  ( n>>56) |
					((n<<40) & 0x00FF000000000000LL) |
					((n<<24) & 0x0000FF0000000000LL) |
					((n<<8)  & 0x000000FF00000000LL) |
					((n>>8)  & 0x00000000FF000000LL) |
					((n>>24) & 0x0000000000FF0000LL) |
					((n>>40) & 0x000000000000FF00LL) |
					( n<<56);
		}
	}

	
#if defined(CC7_LITTLE_ENDIAN)
	
	template <typename T> T ToBigEndian(T n)		{ return detail::SwapEndian(n); }
	template <typename T> T ToLittleEndian(T n)		{ return n; }
	template <typename T> T FromBigEndian(T n)		{ return detail::SwapEndian(n); }
	template <typename T> T FromLittleEndian(T n)	{ return n; }
	
#elif defined(CC7_BIG_ENDIAN)
	
	template <typename T> T ToBigEndian(T n)		{ return n; }
	template <typename T> T ToLittleEndian(T n)		{ return detail::SwapEndian(n); }
	template <typename T> T FromBigEndian(T n)		{ return n; }
	template <typename T> T FromLittleEndian(T n)	{ return detail::SwapEndian(n); }

#else
	#error "Wrong ENDIAN setup in cc7/Platform.h"
#endif
	
} // cc7
