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

// Check for clang builtin byte swap functions
#if defined(__clang__) && __has_builtin(__builtin_bswap16) \
					   && __has_builtin(__builtin_bswap32) \
					   && __has_builtin(__builtin_bswap64)
	#define CC7_BSWAP_16(n)	__builtin_bswap16(n)
	#define CC7_BSWAP_32(n)	__builtin_bswap32(n)
	#define CC7_BSWAP_64(n)	__builtin_bswap64(n)
#endif

namespace cc7
{
	//
	// Endian swap
	//
	namespace detail
	{
		inline cc7::U16 SwapEndian(cc7::U16 n)
		{
		#ifdef CC7_BSWAP_16
			return CC7_BSWAP_16(n);
		#else
			return	(n>>8) |
					(n<<8);
		#endif
		}
		
		inline cc7::U32 SwapEndian(cc7::U32 n)
		{
		#ifdef CC7_BSWAP_32
			return CC7_BSWAP_32(n);
		#else
			return	( n>>24) |
					((n<<8)  & 0x00FF0000) |
					((n>>8)  & 0x0000FF00) |
					( n<<24);
		#endif
		}
		
		inline cc7::U64 SwapEndian(cc7::U64 n)
		{
		#ifdef CC7_BSWAP_64
			return CC7_BSWAP_64(n);
		#else
			return  ( n>>56) |
					((n<<40) & 0x00FF000000000000LL) |
					((n<<24) & 0x0000FF0000000000LL) |
					((n<<8)  & 0x000000FF00000000LL) |
					((n>>8)  & 0x00000000FF000000LL) |
					((n>>24) & 0x0000000000FF0000LL) |
					((n>>40) & 0x000000000000FF00LL) |
					( n<<56);
		#endif
		}
	}

	/**
	 Converts integer |n|, which is in machine's byte order, into big endian representation.
	 Only cc7::U16, cc7::U32 and cc7::U64, or compatible types, are supported in
	 the template parameter T.
	 */
	template <typename T> T ToBigEndian(T n);
	
	/**
	 Converts integer |n|, which is in machine's byte order, into little endian representation
	 Only cc7::U16, cc7::U32 and cc7::U64, or compatible types, are supported in
	 the template parameter T.
	 */
	template <typename T> T ToLittleEndian(T n);
	
	/**
	 Converts big endian integer |n|, into machine's byte order representation.
	 Only cc7::U16, cc7::U32 and cc7::U64, or compatible types, are supported in
	 the template parameter T.
	 */
	template <typename T> T FromBigEndian(T n);
	/**
	 Converts little endian integer |n|, into machine's byte order representation.
	 Only cc7::U16, cc7::U32 and cc7::U64, or compatible types, are supported in
	 the template parameter T.
	 */
	template <typename T> T FromLittleEndian(T n);
	
#if defined(CC7_LITTLE_ENDIAN)
	
	template <typename T> inline T ToBigEndian(T n)      { return detail::SwapEndian(n); }
	template <typename T> inline T ToLittleEndian(T n)   { return n; }
	template <typename T> inline T FromBigEndian(T n)    { return detail::SwapEndian(n); }
	template <typename T> inline T FromLittleEndian(T n) { return n; }
	
#elif defined(CC7_BIG_ENDIAN)
	
	template <typename T> inline T ToBigEndian(T n)      { return n; }
	template <typename T> inline T ToLittleEndian(T n)   { return detail::SwapEndian(n); }
	template <typename T> inline T FromBigEndian(T n)    { return n; }
	template <typename T> inline T FromLittleEndian(T n) { return detail::SwapEndian(n); }

#else
	#error "Wrong ENDIAN setup in cc7/Platform.h"
#endif
	
} // cc7
