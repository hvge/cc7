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
#include <cc7/detail/ExceptionsWrapper.h>

namespace cc7
{
namespace error
{
	/**
	 Defines assertion handler function. You can override behavior of the assertion handler
	 in DEBUG builds of the CC7 library and register your own handler. 
	 The handler will be notified about all failures, produced by CC7_ASSERT() macro.
	 */
	typedef void (*AssertionHandler)(void * handler_data, const char * file, int line, const char * formatted_string);
	
	/**
	 The AssertionHandlerSetup structure contains pointer to assertion handler and custom data
	 for the handler. You can change behavior of CC7_ASSERT() in debug builds of the library.
	 */
	struct AssertionHandlerSetup
	{
		AssertionHandler	handler;
		void *				handler_data;
	};
	
	/**
	 Returns true if the library was compiled with a debug features turned on.
	 It is highly recommended to NOT use this kind of build in the production environment.
	 */
	bool HasDebugFeaturesTurnedOn();
	
#if defined(ENABLE_CC7_ASSERT)

	/**
	 Sets a new setup to internal assertion handler. This function is available only
	 when CC7_ASSERT() macro is enabled and functional.
	 
	 Note that the function is not thread-safe. It is recommended to use this feature
	 only during the unit testing.
	 */
	void SetAssertionHandler(const AssertionHandlerSetup & new_setup);
	
	/**
	 Returns current assertion handler's setup.
	 
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

} // cc7::error
} // cc7
