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
namespace tests
{
namespace detail
{
	/**
	 The UnitTestFactoryFunction is a type for factory function, responsible
	 for creation of instance of one particular test.
	 
	 This is an internal type and should not be used outside of the cc7tests
	 library.
	 */
	typedef UnitTest* (*UnitTestFactoryFunction)();
	
	/**
	 The UnitTestDescriptor structure keeps information about one particular
	 unit test. The type is internal and should not be used outside of the
	 cc7tests library.
	 */
	struct UnitTestDescriptor
	{
		UnitTestFactoryFunction		factory;
		const char *				name;
		const char *				tags;
	};
	
} // cc7::tests::detail
} // cc7::tests
} // cc7
