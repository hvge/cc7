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

#include <cc7/DebugFeatures.h>

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
		
		/**
		 Sets software breakpoint, triggered on assertion enabled or disabled.
		 The TestManager changes default behavior of CC7_ASSERT() macro
		 during the testing and by default ignores all thesese assertions.
		 
		 Normally, the CC7's assertions are used as a runtime warnings and doesn't
		 exits application, like the normal assert() does. This behavior may be
		 iterfering with a regular testing and therefore all CC7_ASSERT's are
		 by default turned off.
		 */
		void setAssertionBreakpointEnabled(bool enabled);
		
		/**
		 Returns whether the assertion breakpoint is enabled or not.
		 */
		bool assertionBreakpointEnabled() const;
		
		/**
		 If turned on, then execution of test will break on software breakpoint,
		 when the test assertion fails.
		 By default is turned off.
		 */
		void setIncidentBreakpointEnabled(bool enabled);
		
		/**
		 Returns whether the incident breakpoint is turned on or not.
		 */
		bool incidentBreakpointEnabled() const;
		
		/**
		 If enabled, then all messages produced by CC7_LOG() macro will be
		 captured and stored to the test log. By default is disabled.
		 */
		void setLogCapturingEnabled(bool enabled);
		
		/**
		 Returns whether the log capturing is enabled or not.
		 */
		bool logCapturingEnabled() const;
		
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
		 Sets custom symbolic name for TestManager object. The name will be displayed at the beginning
		 of test log.
		 */
		void setTestManagerName(const std::string & name);
		/**
		 Returns current symbolic name of TestManager object.
		 */
		const std::string & testManagerName() const;
		
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
		
		/**
		 Puts visually identified message into the test log.
		 */
		void logHeader(const std::string & message);
		/**
		 Puts message into the test log. The method is equivalent to tl().logMessage().
		 */
		void logMessage(const std::string & message);
		/**
		 Puts dashed line into the test log.
		 */
		void logSeparator();
		
	private:

		// Construction / Destruction
		
		TestManager();
		~TestManager();
		
		/**
		 Private execution of selected unit tests.
		 */
		bool executeFilteredTests(const std::vector<std::string> & included_tags, const std::vector<std::string> & excluded_tags);
		/**
		 Private execution of one particular unit test.
		 */
		bool executeTest(UnitTestCreationInfo ti, const std::string & full_test_desc);
		
		// Assert handler
		
		/**
		 Assertion point
		 */
		static void _AssertionHandler(void * handler_data, const char * file, int line, const char * message);
		void addAssertion(const char * message);
		void setupAssertionHandler();
		void restoreAssertionHandler();
		
		/**
		 Log capturing
		 */
		static void _LogHandler(void * handler_data, const char * message);
		void setupLogCapturingHandler();
		void restoreLogCapturingHandler();
		
		void systemLog(const char * message);

		// Private members
		
		/**
		 Symbolic name
		 */
		std::string _test_manager_name;
		/**
		 List of registered tests.
		 */
		UnitTestCreationInfoList _registered_tests;
		/**
		 Test log
		 */
		TestLog _test_log;
		
		/**
		 Assertions / Log configuration
		 */
		bool _assertion_breakpoint_enabled;
		bool _log_capturig_enabled;
		debug::AssertionHandlerSetup _old_assertion_setup;
		debug::LogHandlerSetup _old_log_setup;
		
	};
	
	
} // cc7::tests
} // cc7