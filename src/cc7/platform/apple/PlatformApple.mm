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

#include <cc7/Assert.h>

#if !defined(CC7_APPLE)
#error "This file is for Apple platforms only"
#endif

#pragma mark - CC7_LOG

#if defined(ENABLE_CC7_LOG)

void CC7LogImpl(const char * fmt, ...)
{
	char buffer[1024];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, 1024, fmt, args);
	buffer[1024 - 1] = 0;
	va_end(args);
	
	NSLog(@"CC7: %s", buffer);
}

#endif //ENABLE_CC7_LOG


#pragma mark - Assertion log

#if defined(ENABLE_CC7_ASSERT)

namespace cc7
{
namespace error
{
	static void private_objcDumpToLog(void * foo, const char * file, int line, const char * message)
	{
		NSLog(@"%s", message);
		//
		// Break execution with using software breakpoint.
		//
		CC7_BREAKPOINT();
	}

	AssertionHandler GetDefaultAssertionHandler()
	{
		return private_objcDumpToLog;
	}
}
}

#endif //ENABLE_CC7_ASSERT
