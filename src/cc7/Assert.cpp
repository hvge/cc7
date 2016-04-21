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

#if defined(ENABLE_CC7_ASSERT)

namespace cc7
{
namespace error
{
	//
	// Assertion handler
	//
	
	static AssertionHandler	s_Handler = nullptr;
	static void *			s_HandlerData = nullptr;
	
	void SetAssertionHandler(AssertionHandler handler, void * data)
	{
		AssertionHandler default_handler = GetDefaultAssertionHandler();
		if (!handler) {
			handler = default_handler;
		}
		s_Handler = handler;
		if (handler == default_handler) {
			s_HandlerData = nullptr;
		} else {
			s_HandlerData = data;
		}
	}
	
	void GetAssertionHandler(AssertionHandler& handler, void*& handler_data)
	{
		handler			= s_Handler;
		handler_data	= s_HandlerData;
	}
	
} // cc7::error
} // cc7


//
// Real assert implementation, should not be wrapper in any namespace.
//
int CC7AssertImpl(const char * file, int line, const char * fmt, ...)
{
	// Format input string
	char buffer[1024];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, 1024, fmt, args);
	buffer[1024 - 1] = 0;
	va_end(args);
	
	// Look for file name component from "file"
	// Printing just file name component from the path increases readability (IMO)
	const char * separator = strrchr(file, '/');
	if (!separator) {
		separator = strrchr(file, '\\');
	}
	const char * file_name = separator ? separator + 1 : file;
	
	// Build final string
	char message[1024];
	snprintf(message, 1024, "CC7_ASSERT: %s, %d: %s", file_name, line, buffer);
	message[1024 - 1] = 0;
	
	// Pass that message to the assert handler
	if (!cc7::error::s_Handler) {
		cc7::error::s_Handler = cc7::error::GetDefaultAssertionHandler();
		if (cc7::error::s_Handler) {
			cc7::error::s_Handler(cc7::error::s_HandlerData, file_name, line, message);
		}
	} else {
		cc7::error::s_Handler(cc7::error::s_HandlerData, file_name, line, message);
	}
	
	// Function must return 0 due to fact, that CC7AssertImpl() is also used in CC7_CHECK() macros.
	return 0;
}


#endif //ENABLE_CC7_ASSERT
