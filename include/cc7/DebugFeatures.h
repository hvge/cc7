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
 cc7/DebugFeatures.h
 
 The cc7 library supports several features which helps with debugging & unit testing
 of your code, or the library itself. The main purpose of this header is to define
 all these interfaces at one, centralized place.
 
 Note that the availability of some functions depends on type of the build. For example,
 if library is compiled with DEBUG options, then the SetAssertionHandler() and similar
 functions are available, but these will not be visible for release builds.
 */

#include <cc7/Platform.h>

namespace cc7
{
namespace debug
{
	/**
	 Returns true if cc7 library is compiled with a DEBUG features turned on.
	 It is highly recommended to NOT use this kind of build in the production environment.
	 Your application should check this value in production build (e.g. appstore, final, etc.)
	 and abort its execution, if true is returned.
	 */
	bool HasDebugFeaturesTurnedOn();
	
	/**
	 Defines an assertion handler function. You can override behavior of the assertion handler
	 in DEBUG builds of the cc7 library and register your own handler. The handler will be notified
	 about all failures, reported from CC7_ASSERT & CC7_CHECK macros.
	 
	 The |handler_data| contains value from last AssertionHandlerSetup structure set in the SetAssertionHandler
	 function. The |file| and |line| is information about assertion location and |formatted_string| is a message
	 about the failure.
	 */
	typedef void (*AssertionHandler)(void * handler_data, const char * file, int line, const char * formatted_string);
	
	/**
	 The AssertionHandlerSetup structure contains pointer to the assertion handler and custom data
	 for the handler.
	 */
	struct AssertionHandlerSetup
	{
		/**
		 Log handler, called for each failed assertion.
		 */
		AssertionHandler	handler;
		/**
		 Assertion handler's data, you can put anything you need
		 for your custom assert processing.
		 */
		void *				handler_data;
	};
	
	/**
	 Defines log handler function. You can override behavior of the CC7_LOG() macro in DEBUG builds 
	 of cc7 library and register your own handler. The handler will be notified about all logs produced
	 by CC7_LOG() macro.
	 
	 The |handler_data| contains value from last LogHandlerSetup structure set in the SetLogHandler
	 function. The |formatted_string| is a final formatted message.
	 */
	typedef void (*LogHandler)(void * handler_data, const char * formatted_string);

	/**
	 The LogHandlerSetup structure contains pointer to the log handler and custom data
	 for the handler.
	 */
	struct LogHandlerSetup
	{
		/**
		 Log handler, called for every debug log produced
		 */
		LogHandler	handler;
		/**
		 Log handler's data, you can put anything you need 
		 for your custom log processing.
		 */
		void *		handler_data;
	};
	
#if defined(ENABLE_CC7_ASSERT)

	/**
	 Sets a new setup to internal assertion handler. This function is available only
	 when CC7_ASSERT() macro is enabled and functional.
	 
	 Note that the function is not thread-safe. It is recommended to use this feature
	 only during the unit testing.
	 */
	void SetAssertionHandler(const AssertionHandlerSetup & new_setup);
	
	/**
	 Returns current setup for assertion handler.
	 
	 Note that the function is not thread-safe. It is recommended to use this feature
	 only during the unit testing.
	 */
	AssertionHandlerSetup GetAssertionHandler();
	
	/**
	 Returns default assertion handler's setup.
	 Note that each platform supported by CC7 has its own implementation of this function.
	 */
	AssertionHandlerSetup Platform_GetDefaultAssertionHandler();
	
#endif // defined(ENABLE_CC7_ASSERT)

	
#if defined(ENABLE_CC7_LOG)
	
	/**
	 Sets a new setup to internal log handler. This function is available only
	 when CC7_LOG() macro is enabled and functional.
	 
	 Note that the function is not thread-safe. It is recommended to use this feature
	 only during the unit testing.
	 */
	void SetLogHandler(const LogHandlerSetup & new_setup);
	
	/**
	 Returns current setup for log handler.
	 
	 Note that the function is not thread-safe. It is recommended to use this feature
	 only during the unit testing.
	 */
	LogHandlerSetup GetLogHandler();
	
	/**
	 Returns default log handler's setup.
	 Note that each platform supported by CC7 has its own implementation of this function.
	 */
	LogHandlerSetup Platform_GetDefaultLogHandler();

#endif // defined(ENABLE_CC7_LOG)

} // cc7::debug
} // cc7
