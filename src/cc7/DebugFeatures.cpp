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

#include <cc7/DebugFeatures.h>

namespace cc7
{
namespace debug
{
	//
	// Must be always implemented. Doesn't depend on assert.
	//
	bool HasDebugFeaturesTurnedOn()
	{
#if defined(DEBUG) || defined(ENABLE_CC7_LOG) || defined(ENABLE_CC7_ASSERT)
		return true;
#else
		return false;
#endif
	}
	
	
#if defined(ENABLE_CC7_ASSERT)
	//
	// Assertion handler
	//
	static AssertionHandlerSetup s_assert_setup = { nullptr, nullptr };
	
	void SetAssertionHandler(const AssertionHandlerSetup & new_setup)
	{
		AssertionHandlerSetup default_setup = Platform_GetDefaultAssertionHandler();
		if (!new_setup.handler) {
			s_assert_setup = default_setup;
		} else {
			s_assert_setup = new_setup;
		}
	}
	
	AssertionHandlerSetup GetAssertionHandler()
	{
		return s_assert_setup;
	}
#endif //ENABLE_CC7_ASSERT

	
#if defined(ENABLE_CC7_LOG)
	//
	// Log handler
	//
	static LogHandlerSetup s_log_setup = { nullptr, nullptr };
	
	void SetLogHandler(const LogHandlerSetup & new_setup)
	{
		LogHandlerSetup default_setup = Platform_GetDefaultLogHandler();
		if (!new_setup.handler) {
			s_log_setup = default_setup;
		} else {
			s_log_setup = new_setup;
		}
	}
	
	LogHandlerSetup GetLogHandler()
	{
		return s_log_setup;
	}
#endif //ENABLE_CC7_LOG


} // cc7::debug
} // cc7


#if defined(ENABLE_CC7_ASSERT)
//
// Real assert implementation, should not be wrapper in any namespace.
//
int CC7AssertImpl(int condition, const char * file, int line, const char * fmt, ...)
{
	if (condition) {
		// assertion did not fail, just return positive value
		return 1;
	}
	
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
	if (!cc7::debug::s_assert_setup.handler) {
		cc7::debug::s_assert_setup = cc7::debug::Platform_GetDefaultAssertionHandler();
		if (cc7::debug::s_assert_setup.handler) {
			cc7::debug::s_assert_setup.handler(cc7::debug::s_assert_setup.handler_data, file_name, line, message);
		}
	} else {
		cc7::debug::s_assert_setup.handler(cc7::debug::s_assert_setup.handler_data, file_name, line, message);
	}
	
	// Function must return 0 due to fact, that CC7AssertImpl() is also used in CC7_CHECK() macros.
	return condition;
}
#endif //ENABLE_CC7_ASSERT


#if defined(ENABLE_CC7_LOG)
//
// Real log implementation, should not be wrapper in any namespace.
//
void CC7LogImpl(const char * fmt, ...)
{
	char message[1024];
	va_list args;
	va_start(args, fmt);
	vsnprintf(message, 1024, fmt, args);
	message[1024 - 1] = 0;
	va_end(args);
	
	// Pass that message to the assert handler
	if (!cc7::debug::s_log_setup.handler) {
		cc7::debug::s_log_setup = cc7::debug::Platform_GetDefaultLogHandler();
		if (cc7::debug::s_log_setup.handler) {
			cc7::debug::s_log_setup.handler(cc7::debug::s_log_setup.handler_data, message);
		}
	} else {
		cc7::debug::s_log_setup.handler(cc7::debug::s_log_setup.handler_data, message);
	}
}
#endif //ENABLE_CC7_LOG
