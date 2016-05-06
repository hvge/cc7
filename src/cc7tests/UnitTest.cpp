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

#include <cc7tests/UnitTest.h>
#include <stdexcept>

namespace cc7
{
namespace tests
{
	UnitTest::UnitTest() :
		_log(nullptr),
		_manager(nullptr)
	{
	}
	
	
	UnitTest::~UnitTest()
	{
	}
	
	
	TestLog & UnitTest::tl()
	{
		if (!_log) {
			throw std::runtime_error("The TestLog is not set.");
		}
		return *_log;
	}
	
	
	TestLog & UnitTest::tl() const
	{
		if (!_log) {
			throw std::runtime_error("The TestLog is not set.");
		}
		return *_log;
	}
	
	
	TestManager & UnitTest::testManager()
	{
		if (!_manager) {
			throw std::runtime_error("The TestManager is not set.");
		}
		return *_manager;
	}

	
	TestManager & UnitTest::testManager() const
	{
		if (!_manager) {
			throw std::runtime_error("The TestManager is not set.");
		}
		return *_manager;
	}

	
	void UnitTest::registerTestMethod(std::function<void()> method, const char * description)
	{
		if (CC7_CHECK(method != nullptr && description != nullptr, "method & description must be set")) {
			_methods.push_back(make_tuple(method, std::string(description)));
		}
	}
	
	
	bool UnitTest::runTest(TestManager * manager, TestLog * log)
	{
		_log = log;
		_manager = manager;
		
		tl().clearCurrentTestIncidentsCount();
		
		instanceSetUp();
		
		if (_methods.size() == 0) {
			tl().logMessage("The test class is empty. Try to register some test methods.");
		}
		
		size_t indent_before = tl().indentationLevel();
		
		for (auto desc : _methods) {
			std::function<void()>	method_ptr;
			std::string				method_name;
			tie(method_ptr, method_name) = desc;
			
			tl().logFormattedMessage("[ %s ]", method_name.c_str());
			
			tl().setIndentationLevel(indent_before + 2);
			setUp();
			method_ptr();
			tearDown();
			tl().setIndentationLevel(indent_before);
		}
		
		instanceTearDown();
		
		bool result = tl().logDataCounters().current_test_incidents_count == 0;
		
		_log		= nullptr;
		_manager	= nullptr;
		
		return result;
	}
}
}
