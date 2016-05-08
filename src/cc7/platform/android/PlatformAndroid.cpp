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

#include <cc7/Platform.h>

#if !defined(CC7_ANDROID)
#error "This file is for Android platform only"
#endif

#include <android/log.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#if defined(ENABLE_CC7_LOG)
//
// Debug log is enabled, we have to provide CC7LogImpl() function
//
void CC7LogImpl(const char * fmt, ...)
{
	char buffer[1024];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, 1024, fmt, args);
	buffer[1024 - 1] = 0;
	va_end(args);
	
	__android_log_write(ANDROID_LOG_INFO, "CC7", buffer);
}

#endif //ENABLE_CC7_LOG



#if defined(ENABLE_CC7_ASSERT)

namespace cc7
{
namespace error
{
	static void private_androidDumpToLog(void * foo, const char * file, int line, const char * message)
	{
		__android_log_write(ANDROID_LOG_ERROR, "CC7", message);
		
		// ...unlike ios, android has no SW breakpoint defined.
	}
	
	AssertionHandler Platform_GetDefaultAssertionHandler()
	{
		return private_androidDumpToLog;
	}
	
} // cc7::error
} // cc7

#endif //ENABLE_CC7_ASSERT
