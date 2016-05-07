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

#include <cc7tests/detail/TestTypes.h>

/**
 This file contains various macros for unit test creation and registration.
 */


/**
 The CC7_CREATE_UNIT_TEST() macro creates an internal registration structure
 which helps with future unit test instantiation. You have to use this macro
 in cooperation with CC7_ADD_UNIT_TEST() for propper test registration and
 usage.
 
 The |TestClassName| parameter is a name of UnitTest derived class
 
 The |TestTagsgOrNULL| parameter defines tags for the test, which allows
 future categorization or filtering, during the test runs. You can use NULL
 if test has no tags, or string, with space separated tags.
 
 Note that the 'TestClassName' type must be fully declared. You can put
 the class to a different namespace, but the namespace where's the macro used
 should match the one, where you're adding test to the list of tests.
 */
#define CC7_CREATE_UNIT_TEST(TestClassName, TestTagsgOrNULL)					\
	static cc7::tests::UnitTest * _cc7t_factory_##TestClassName()				\
	{																			\
		return new TestClassName();												\
	}																			\
	cc7::tests::detail::UnitTestDescriptor _cc7t_cinfo_##TestClassName =		\
	{																			\
		&_cc7t_factory_##TestClassName,											\
		#TestClassName,															\
		TestTagsgOrNULL															\
	};																			\

/**
 The CC7_USE_UNIT_TEST() makes previously created internal structure visible
 in the current context. Note that namespace where's this macro used must match
 the one, where test structure was created.
 
 Normally you don't need to use this macro directly and is used in the
 implementation of CC7_ADD_UNIT_TEST().
 */
#define CC7_USE_UNIT_TEST(TestClassName)										\
	extern cc7::tests::detail::UnitTestDescriptor _cc7t_cinfo_##TestClassName;

/**
 The CC7_GET_UNIT_TEST() returns pointer to an internal registration structure.
 The structure must be visible in a current context.
 
 Normally you don't need to use this macro directly and is used in the
 implementation of CC7_ADD_UNIT_TEST().
 */
#define CC7_GET_UNIT_TEST(TestClassName)	&_cc7t_cinfo_##TestClassName


/**
 The CC7_ADD_UNIT_TEST() macto adds anunit test to the list of tests.
 The |TestClassName| parameter is a name of UnitTest derived class.
 The |list| parameter must be an instance of UnitTestCreationInfoList class.
 
 The direct visibility to the UnitTest derived class is not required as long
 as the class has the same namespace as the context in within you're performing
 the registration. For the simplification, it's recommended to put all test 
 classes and registration code to the cc7::tests namespace.
 */
#define CC7_ADD_UNIT_TEST(TestClassName, list)									\
	CC7_USE_UNIT_TEST(TestClassName);											\
	list.push_back(CC7_GET_UNIT_TEST(TestClassName));


/**
 The CC7_REGISTER_TEST_METHOD macro registers a test method in the context of 
 one particular unit test. It is recommended to reqister the test methods during
 the construction of UnitTest derived class.
 
 You should register at least one method in one particular unit test class.
 */
#define CC7_REGISTER_TEST_METHOD(method_name)									\
	this->registerTestMethod([this]() { this->method_name(); }, #method_name );
