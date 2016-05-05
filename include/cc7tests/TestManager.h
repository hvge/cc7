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

#include <cc7tests/UnitTest.h>
#include <cc7tests/TestLog.h>
#include <cc7tests/detail/TestTypes.h>

#include <cc7/Assert.h>

namespace cc7
{
namespace tests
{
	/**
	 The UnitTestCreationInfo is a type, representing information about
	 creation of one particular unit test. An actual implementation is using
	 constant pointer to detail::UnitTestDescriptor structure.
	 */
	typedef const detail::UnitTestDescriptor * UnitTestCreationInfo;
	
	/**
	 The UnitTestCreationInfoList is a vector type, representing list of unit tests
	 in the cc7tests framework.
	 */
	typedef std::vector<UnitTestCreationInfo> UnitTestCreationInfoList;
	
	/**
	 The TestManager class is responsible for managing whole process of the unit testing
	 in cc7tests framework.
	 */
	class TestManager
	{
	public:
		
		// Instantiation
		
		/**
		 Creates a new TestManager instance with default list of tests.
		 */
		static TestManager * createDefaultManager();
		
		/**
		 Creates a new empty TestManager instance with no tests added.
		 */
		static TestManager * createEmptyManager();
		
		/**
		 Releases previously created instance of TestManager class.
		 */
		static void releaseManager(TestManager * manager);
		
		
		// Breakpoints configuration
		
		void setAssertionBreakpointEnabled(bool enabled);
		bool assertionBreakpointEnabled() const;
		
		void setIncidentBreakpointEnabled(bool enabled);
		bool incidentBreakpointEnabled() const;

		
		// Tests registration
		
		/**
		 Adds one particular test, represented by UnitTestCreationInfo object
		 to the test manager. The method internally performs a basic sanity
		 checks and removes duplicate tests added to the manager.
		 */
		void addUnitTest(UnitTestCreationInfo test_info);
		
		/**
		 Adds multiple unit tests, represented by UnitTestCreationInfo object, 
		 to the test manager.
		 */
		void addUnitTestList(const UnitTestCreationInfoList & test_info_list);
		
		
		// Tests execution
		
		/**
		 Runs all registered tests at once.
		 */
		bool runAllTests();
		
		/**
		 Runs only tests which passed over the inclusion and exclusion filter.
		 If included_tags parameter is empty then all tests will be included for run.
		 If excluded_tags paramter is empty then no tests will be excluded for run.
		 */
		bool runTestsWithFilter(const std::string & included_tags, const std::string & excluded_tags);

		
		
		// Logging
		
		/**
		 Returns reference to TestLog object. The method is mostly used in
		 macros implementing testing assertions.
		 */
		TestLog & tl()
		{
			return _test_log;
		}
		
		/**
		 Returns constant reference to TestLog object. The method is mostly used in
		 macros implementing testing assertions.
		 */
		const TestLog & tl() const
		{
			return _test_log;
		}
				
		void logHeader(const std::string & message);
		void logMessage(const std::string & message);
		void logSeparator();
		
	private:

		// Construction / Destruction
		
		TestManager();
		~TestManager();
		
		// Test execution
		bool executeAllTests(const std::vector<std::string> & included_tags, const std::vector<std::string> & excluded_tags);
		bool executeTest(UnitTestCreationInfo ti, const std::string & full_test_desc);
		
		// Assert handler
		static void _AssertionHandler(void * handler_data, const char * file, int line, const char * message);
		void addAssertion(const char * message);
		void setupAssertionHandler();
		void restoreAssertionHandler();

		// Private members
		
		UnitTestCreationInfoList _registered_tests;
		TestLog _test_log;
		
		bool _assertion_breakpoint_enabled;
		
		cc7::error::AssertionHandlerSetup _old_assertion_setup;
		
	};
	
	
} // cc7::tests
} // cc7