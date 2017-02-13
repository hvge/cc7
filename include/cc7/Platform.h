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

/**
 cc7/Platform.h
 
 The purpose of Platform.h header is to clear several per-platform differences and set up
 a basic framework for muptiplatform development. Including this header you can count
 on a small set of default features, like debug logging, assertions, some important C/C++ 
 ssytem libraries, available on each supported platform.
 
 If your platform supports precompiled headers, then it's recommended include this header
 from PH. You can also include <cc7/CC7.7> if you want to make available ALL cc7's public
 interfaces.
 
 It is also recommended to include this header at first position from all your public 
 headers, or include any other header which includes Platform.h at first position. 
 This "Platform.h first" principle is mandatory for all cc7 library developers.
 
 
 Defined macros
 -----------------
 
 CC7_EXTERN_C, 
 CC7_EXTERN_C_BEGIN, 
 CC7_EXTERN_C_END
	- Defining extern "C" for C & C++ codes
 
 CC7_APPLE
	- Defined, if compiled on supported Apple platform in general (iOS, watchOS, OSX...)
 
 CC7_IOS
 CC7_OSX
 CC7_APPLE_WATCH
 CC7_APPLE_TV
	- Defined on specific Apple platform

 CC7_ANDROID
	- Defined, if compiled on Android platform
 
 CC7_PLATFORM32
 CC7_PLATFORM64
	- Defines whether the platform has 32 or 64 bits long word
 
 CC7_LITTLE_ENDIAN
 CC7_BIG_ENDIAN
	- Defines endianness on the platform
 
 CC7_SecureClean(void*, size_t size)
	- Defines secure memory cleanup function. The compiler doesn't omit
	  call to the function during its code optimization pass. You should
	  prefer this macro over the memset() to be sure that the memory cleanup
	  is really executed. The macro is using memset_s() if available, or
	  OPENSSL_cleanse() as an fallback implementation.
 
 CC7_BREAKPOINT()
	- A software breakpoint instruction, defined for DEBUG builds.
	- The sw breakpoint is supported only on some platforms, where
	  the native code debugging is well supported in the toolchains.
 
 CC7_LOG(format_string, args...)
	- Defines debug logging macro. The statement is removed for non DEBUG builds
 
 CC7_ASSERT(condition, format_string, args...)
	- Triggers assertion when condition is false and prints formatted message
	  about that to the debug console. If the software breakpoint is supported
	  on the platform, then breaks code execution in the assertion handling
	  routine.
	- For non DEBUG builds does nothing (e.g. whole statement is removed)
 
 CC7_CHECK(condition, format_string, args...)
	- Works just like CC7_ASSERT(), but can be used in conditional statements.
	  The result from the condition is used as the result of the macro.
	- For non DEBUG builds only the condition is expanded
    - For example:
	
		if (CC7_CHECK(can_process, "Can't process")) { 
			doSomething();
		}
	  
	  The Code will trigger assertion on DEBUG build if can_process is false,
	  but will execute doSomething() if can_process is true. For non-DEBUG
	  builds, the macro is expanded to:
 
		if (can_process) {
			doSomething();
		}
 
 
 Primitive types
 -----------------
 
 Following primitive types are defined for C++ code:
 
	cc7::byte, cc7:U8	as one usigned byte (8 bits)
	cc7::U16,			as 16 bit unsigned type
	cc7::U32			as 32 bit unsigned type
	cc7::U64			as 64 bit unsigned type
 
 
 Includes
 -----------------
 
 You can also count on that following headers (or similar replacement available on platform)
 are automatically included from Platform.h:
 
 c,c++
	<stdlib.h>
	<string.h>
 c++
	<string>
	<vector>
 
 
 Footnotes
 -----------------
 
 If you start digging into this header (or into the cc7 library in general), then you'll 
 see that some constructs are quite "clang centric". This is by purpose, becase we believe
 that the LLVM & clang is the future of native development. You can probably compile & use 
 the library with using other compilers (like GCC) with no problems, but clang is really
 our first choice.
 
 */



// =======================================================================
// Common macros
// =======================================================================

//
// Defines CC7_EXTERN_C, CC7_EXTERN_C_BEGIN & CC7_EXTERN_C_END macros
// which allows you define extern functions for both C and C++ codes.
//
#ifdef __cplusplus
	// C++
	#define CC7_EXTERN_C				extern "C"
	#define CC7_EXTERN_C_BEGIN          extern "C" {
	#define CC7_EXTERN_C_END			}
#else
	// C
	#define CC7_EXTERN_C				extern
	#define CC7_EXTERN_C_BEGIN
	#define CC7_EXTERN_C_END
#endif

//
// Compatibility with non-clang compilers.
//
#ifndef __has_builtin
	#define __has_builtin(x)	0
#endif
#ifndef __has_feature
	#define __has_feature(x)	0
#endif
#ifndef __has_extension
	#define __has_extension		__has_feature
#endif

// =======================================================================
//
// Main platform switch
//
// =======================================================================

#ifdef __APPLE__
	// -------------------------------------------------------------------
	// APPLE PLATFORMS (e.g. iOS, OSX, etc...)
	// -------------------------------------------------------------------
	#define __STDC_WANT_LIB_EXT1__ 1
	#ifdef __OBJC__
		// Objective-C specific
		#import <Foundation/Foundation.h>
	#else
		// Pure C
		#include <stdlib.h>
		#include <string.h>
	#endif
	#include "TargetConditionals.h"
	// Switch between iOS & OSX & Others...
	#if TARGET_OS_IPHONE == 1 && TARGET_OS_IOS == 1
		#define CC7_IOS
	#elif TARGET_OS_OSX == 1
		#define CC7_OSX
	#elif TARGET_OS_WATCH == 1
		#define CC7_APPLE_WATCH
	#elif TARGET_OS_TV == 1
		#define CC7_APPLE_TV
	#endif
	// Common defines for Apple platforms
	#define CC7_APPLE
	#define CC7_SecureClean(ptr, size)  memset_s(ptr, size, 0, size)
	// 64 bit
	#if TARGET_RT_64_BIT == 0
		#define CC7_PLATFORM32
	#else
		#define CC7_PLATFORM64
	#endif
	// Little / Big endian
	#if TARGET_RT_LITTLE_ENDIAN == 1
		#define CC7_LITTLE_ENDIAN
	#else
		#define CC7_BIG_ENDIAN
	#endif
	//
#elif __ANDROID__
	// -------------------------------------------------------------------
	// ANDROID PLATFORM
	// -------------------------------------------------------------------
	#include <stdlib.h>
	#include <string.h>
	//
	#define CC7_ANDROID
	CC7_EXTERN_C void OPENSSL_cleanse(void *ptr, size_t len);
	#define CC7_SecureClean(ptr, size)  OPENSSL_cleanse(ptr, size)
	// 64 bit
	#if __SIZEOF_POINTER__ == 8
		#define CC7_PLATFORM64
	#else
		#define CC7_PLATFORM32
	#endif
	// TODO: handle possible BE on Androids
	#define CC7_LITTLE_ENDIAN
	//
#elif defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
	// -------------------------------------------------------------------
	// Windows8+ Phone
	// -------------------------------------------------------------------
	#include <sdkddkver.h>
	#include <Windows.h>
	#include <stdlib.h>
	#include <string.h>
	//
	#define CC7_WINDOWS
	#define CC7_LITTLE_ENDIAN
	#define CC7_SecureClean(ptr, size)  RtlSecureZeroMemory(ptr, size)
	//
#elif defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
	// -------------------------------------------------------------------
	// Windows 10 Universal APP, now has different ifdef branch than WP8,
	// might be handy in future.
	// -------------------------------------------------------------------
	#include <sdkddkver.h>
	#include <Windows.h>
	#include <stdlib.h>
	#include <string.h>
	//
	#define CC7_WINDOWS
	#define CC7_LITTLE_ENDIAN
	#define CC7_SecureClean(ptr, size)  RtlSecureZeroMemory(ptr, size)
	//
#else
	//
	#error "CC7: Unsupported platform. Check Platform.h file"
	//
#endif // end of platform switch

// =======================================================================
// Setup for implicit features
// =======================================================================

#if defined (DEBUG)
	// DEBUG builds have CC7_LOG and CC7_ASSERT macros turned on by default
	#ifndef ENABLE_CC7_LOG
		#define ENABLE_CC7_LOG
	#endif
	#ifndef ENABLE_CC7_ASSERT
		#define ENABLE_CC7_ASSERT
	#endif
#endif

// =======================================================================
// Debug Features
// =======================================================================

#if defined(ENABLE_CC7_LOG)
	//
	// CC7_LOG is enabled
	//
	CC7_EXTERN_C void CC7LogImpl(const char * fmt, ...);
	#define CC7_LOG(...) CC7LogImpl(__VA_ARGS__)

#else
	//
	// CC7_LOG is disabled
	//
	#define CC7_LOG(...)

#endif


#if defined(ENABLE_CC7_ASSERT)
	//
	// CC7_ASSERT is enabled
	//
	CC7_EXTERN_C int CC7AssertImpl(int condition, const char * file, int line, const char * format, ...);

	#ifdef CC7_IOS
		#if	TARGET_CPU_ARM == 1
			// 32 bit ARM
			#define CC7_BREAKPOINT() __asm__ volatile ("bkpt 0");
			//
		#elif TARGET_CPU_ARM64 == 1
			// 64 bit ARM
			#define CC7_BREAKPOINT() __asm__ volatile ("brk 0");
			//
		#elif TARGET_CPU_X86 == 1
			// x86 (32 bit simulator)
			#define CC7_BREAKPOINT() __asm__ volatile ("int3");
			//
		#elif TARGET_CPU_X86_64 == 1
			// x86_64 (64 bit simulator)
			#define CC7_BREAKPOINT() __asm__ volatile ("int3");
			//
		#else
			// Undefined platform
			#warning "CC7_BREAKPOINT is not defined for this iOS platform"
			#define CC7_BREAKPOINT()
			//
		#endif
	#endif // CC7_IOS

	#ifdef CC7_ANDROID
		#define CC7_BREAKPOINT()
	#endif // CC7_ANDROID

	#ifdef CC7_WINDOWS
		#define CC7_BREAKPOINT()
	#endif // CC7_WINDOWS

	#define CC7_ASSERT(cond, ...)												\
		if (!(cond)) {															\
			CC7AssertImpl(0, __FILE__, __LINE__, "" __VA_ARGS__);				\
		}
	#define CC7_CHECK(cond, ...)												\
		CC7AssertImpl(cond, __FILE__, __LINE__, "" __VA_ARGS__)

#else
	//
	// CC7_ASSERT is disabled
	//
	#ifndef CC7_WINDOWS
		// Non-MSVC compilers (gcc, clang)
		#define CC7_BREAKPOINT()
		#define CC7_ASSERT(cond, ...)
		#define CC7_CHECK(cond, ...) (cond)
	#else
		// MSVC compiler
		#define CC7_BREAKPOINT()
		#define CC7_ASSERT
		#define CC7_CHECK(cond, ...) (cond)
	#endif

#endif // defined(ENABLE_CC7_ASSERT)


// =======================================================================
// Default includes & common types, valid for ALL platforms
// =======================================================================

#ifdef __cplusplus
	// C++
	#include <string>
	#include <vector>

	namespace cc7
	{
		typedef uint8_t		byte;
		typedef uint8_t		U8;
		typedef uint16_t	U16;
		typedef uint32_t	U32;
		typedef uint64_t	U64;
		
	} // cc7

#endif //__cplusplus
