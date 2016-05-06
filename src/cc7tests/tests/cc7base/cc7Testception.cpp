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

#include <cc7tests/CC7Tests.h>
#include <cc7tests/detail/TestUtilities.h>

namespace cc7
{
namespace tests
{
	/**
	 The cc7Testception.cpp file implements unit tests for UnitTest framework itself.
	 What a magic!
	 */
	
	
	
	//
	// Always success tests
	//
	
	class UT_Success1 : public UnitTest
	{
	public:
		void setUp()
		{
			ccstFailure("No test methods, setUp must not be called");
		}
	};
	CC7_CREATE_UNIT_TEST(UT_Success1, "success")

	class UT_Success2 : public UnitTest
	{
	public:
		void tearDown()
		{
			ccstFailure("No test methods, tearDown must not be called");
		}
	};
	CC7_CREATE_UNIT_TEST(UT_Success2, "success group1")

	
	class UT_Success3 : public UnitTest
	{
	public:
		UT_Success3()
		{
			CC7_REGISTER_TEST_METHOD(successOnPurpose)
		}
		void successOnPurpose()
		{
			ccstAssertTrue(true, "Should not fail")
			ccstAssertTrue(true)
			ccstAssertFalse(false, "Should not fail")
			ccstAssertFalse(false)
			ccstAssertEqual(4, 4, "Should not fail")
			ccstAssertEqual(-1, -1)
			ccstAssertNotEqual(3, 4, "Should not fail")
			ccstAssertNotEqual(1, 2)
			ccstAssertNull(nullptr, "Should not fail")
			ccstAssertNull(nullptr)
			ccstAssertNotNull(this, "Should not fail")
			ccstAssertNotNull(this)
		}
	};
	CC7_CREATE_UNIT_TEST(UT_Success3, "success group2")
	
	// ------------
	
	UnitTestCreationInfoList GetPositiveList()
	{
		UnitTestCreationInfoList list;
		
		CC7_ADD_UNIT_TEST(UT_Success1, list)
		CC7_ADD_UNIT_TEST(UT_Success2, list)
		CC7_ADD_UNIT_TEST(UT_Success3, list)
		
		return list;
	}
	
	
	
	//
	// Always failure tests
	//
	
	class UT_Fail1 : public UnitTest
	{
	public:
		void instanceSetUp()
		{
			ccstFailure("Must be called and trigger failure");
		}
	};
	CC7_CREATE_UNIT_TEST(UT_Fail1, "failure")
	
	class UT_Fail2 : public UnitTest
	{
	public:
		void instanceTearDown()
		{
			ccstFailure("Must be called and trigger failure");
		}
	};
	CC7_CREATE_UNIT_TEST(UT_Fail2, "failure group1")
	
	class UT_Fail3 : public UnitTest
	{
	public:
		UT_Fail3()
		{
			CC7_REGISTER_TEST_METHOD(failureOnPurpose)
		}
		void failureOnPurpose()
		{
			ccstAssertTrue(false, "Must fail")
			ccstAssertTrue(false)
			ccstAssertFalse(true, "Must fail")
			ccstAssertFalse(true)
			ccstAssertEqual(5, 1, "Must fail")
			ccstAssertEqual(-1, 2)
			ccstAssertNotEqual(881, 881, "Must fail")
			ccstAssertNotEqual(882, 882)
			ccstAssertNull(this, "Must fail")
			ccstAssertNull(this)
			ccstAssertNotNull(nullptr, "Must fail")
			ccstAssertNotNull(nullptr)
			
		}
	};
	CC7_CREATE_UNIT_TEST(UT_Fail3, "failure group2")
	
	class UT_Fail4 : public UnitTest
	{
	public:
		UT_Fail4()
		{
			CC7_REGISTER_TEST_METHOD(forceException)
		}
		void forceException()
		{
			throw std::invalid_argument("Don't forget a towel.");
		}
	};
	CC7_CREATE_UNIT_TEST(UT_Fail4, "failure")
	
	// --------------------------------------------------------------------
	
	UnitTestCreationInfoList GetNegativeList()
	{
		UnitTestCreationInfoList list;
		
		CC7_ADD_UNIT_TEST(UT_Fail1, list)
		CC7_ADD_UNIT_TEST(UT_Fail2, list)
		CC7_ADD_UNIT_TEST(UT_Fail3, list)
		CC7_ADD_UNIT_TEST(UT_Fail4, list)
		
		return list;
	}
	
	/**
	 The cc7Testception is a globally registered class.
	 */
	class cc7Testception : public UnitTest
	{
		TestManager * _manager;
		
		size_t _positive_count;
		size_t _negative_count;

	public:
		
		cc7Testception() : _manager(nullptr)
		{
			CC7_REGISTER_TEST_METHOD(internalSplitStringTest);
			CC7_REGISTER_TEST_METHOD(allTests);
			CC7_REGISTER_TEST_METHOD(positiveTests);
			CC7_REGISTER_TEST_METHOD(negativeTests);
			CC7_REGISTER_TEST_METHOD(filterTests);
		}
		
		~cc7Testception()
		{
			TestManager::releaseManager(_manager);
		}
		
		
		// SETUP & TEAR DOWN
		
		void setUp()
		{
			TestManager::releaseManager(_manager);
			_manager = TestManager::createEmptyManager();
			ccstAssertNotNull(_manager, "Unable to create empty test manager");
			_manager->tl().setIndentationSuffix(">>  ");
			
			UnitTestCreationInfoList negative = GetNegativeList();
			UnitTestCreationInfoList positive = GetPositiveList();
			
			_manager->addUnitTestList(positive);
			_manager->addUnitTestList(negative);
			
			_positive_count = positive.size();
			_negative_count = negative.size();
		}
		
		void tearDown()
		{
		}
		
		void logLog()
		{
			ccstMessage("%s", _manager->tl().logData().log.c_str());
		}
		
		void simpleMethod()
		{
			ccstMessage("Hello world\nIn multiple\nlines");
		}
		
		
		// UNIT TESTS
		
		void internalSplitStringTest()
		{
			auto result = detail::SplitString("", ' ');
			ccstAssertTrue(result.size() == 0, "null must produce empty vector");
			
			result = detail::SplitString("tag1", ' ');
			ccstAssertTrue(result.size() == 1, "wrong result");
			ccstAssertEqual(result[0], "tag1", "wrong result");
			
			result = detail::SplitString("tag1 tag2", ' ');
			ccstAssertTrue(result.size() == 2, "wrong result");
			ccstAssertEqual(result[0], "tag1", "wrong result");
			ccstAssertEqual(result[1], "tag2", "wrong result");
			
			result = detail::SplitString("tag1  tag2  tag3", ' ');
			ccstAssertTrue(result.size() == 3, "wrong result");
			ccstAssertEqual(result[0], "tag1", "wrong result");
			ccstAssertEqual(result[1], "tag2", "wrong result");
			ccstAssertEqual(result[2], "tag3", "wrong result");
			
			result = detail::SplitString("  tag1  tag2  tag3  ", ' ');
			ccstAssertTrue(result.size() == 3, "wrong result");
			ccstAssertEqual(result[0], "tag1", "wrong result");
			ccstAssertEqual(result[1], "tag2", "wrong result");
			ccstAssertEqual(result[2], "tag3", "wrong result");
		}
		
		void allTests()
		{
			bool result = _manager->runAllTests();
			ccstAssertFalse(result);
			
			ccstAssertTrue(_manager->tl().logDataCounters().executed_tests == _positive_count + _negative_count);
			ccstAssertTrue(_manager->tl().logDataCounters().passed_tests == _positive_count);
			ccstAssertTrue(_manager->tl().logDataCounters().failed_tests  == _negative_count);
			ccstAssertTrue(_manager->tl().logDataCounters().skipped_tests == 0);
			
			if (result) {
				// must fail, something is wrong, dump log
				logLog();
			}
		}
		
		void positiveTests()
		{
			bool result = _manager->runTestsWithFilter("success", "");
			ccstAssertTrue(result);
			
			ccstAssertTrue(_manager->tl().logDataCounters().executed_tests == _positive_count);
			ccstAssertTrue(_manager->tl().logDataCounters().passed_tests  == _positive_count);
			ccstAssertTrue(_manager->tl().logDataCounters().failed_tests == 0);
			ccstAssertTrue(_manager->tl().logDataCounters().skipped_tests == _negative_count);
			
			if (!result) {
				// failed, something is wrong
				logLog();
			}
		}
		
		void negativeTests()
		{
			bool result = _manager->runTestsWithFilter("failure", "");
			ccstAssertFalse(result);
			
			ccstAssertTrue(_manager->tl().logDataCounters().executed_tests == _negative_count);
			ccstAssertTrue(_manager->tl().logDataCounters().passed_tests == 0);
			ccstAssertTrue(_manager->tl().logDataCounters().failed_tests == _negative_count);
			ccstAssertTrue(_manager->tl().logDataCounters().skipped_tests == _positive_count);
			
			if (result) {
				// must fail, dump log
				logLog();
			}
		}
		
		
		void filterTests()
		{
			bool result = _manager->runTestsWithFilter("", "failure");
			ccstAssertTrue(result);
			
			ccstAssertTrue(_manager->tl().logDataCounters().executed_tests == _positive_count);
			ccstAssertTrue(_manager->tl().logDataCounters().passed_tests  == _positive_count);
			ccstAssertTrue(_manager->tl().logDataCounters().failed_tests  == 0);
			ccstAssertTrue(_manager->tl().logDataCounters().skipped_tests == _negative_count);
			
			if (!result) {
				// should not fail, dump log
				logLog();
			}
			
			result = _manager->runTestsWithFilter("group1", "failure");
			ccstAssertTrue(result);
			
			ccstAssertTrue(_manager->tl().logDataCounters().executed_tests == 1);
			ccstAssertTrue(_manager->tl().logDataCounters().passed_tests == 1);
			ccstAssertTrue(_manager->tl().logDataCounters().failed_tests  == 0);
			ccstAssertTrue(_manager->tl().logDataCounters().skipped_tests == _negative_count + _positive_count - 1);
			
			if (!result) {
				// should not fail, dump log
				logLog();
			}
			
			result = _manager->runTestsWithFilter("group1", "");
			ccstAssertFalse(result);
			
			ccstAssertTrue(_manager->tl().logDataCounters().passed_tests == 2);
			ccstAssertTrue(_manager->tl().logDataCounters().failed_tests == 1);
			ccstAssertTrue(_manager->tl().logDataCounters().failed_tests == 1);
			ccstAssertTrue(_manager->tl().logDataCounters().skipped_tests == _negative_count + _positive_count - 2);
			
			if (result) {
				// should not fail, dump log
				logLog();
			}
			
			result = _manager->runTestsWithFilter("group1 group2", "");
			ccstAssertFalse(result);
			
			ccstAssertTrue(_manager->tl().logDataCounters().executed_tests == 4);
			ccstAssertTrue(_manager->tl().logDataCounters().passed_tests  == 2);
			ccstAssertTrue(_manager->tl().logDataCounters().failed_tests  == 2);
			ccstAssertTrue(_manager->tl().logDataCounters().skipped_tests == _negative_count + _positive_count - 4);
			
			if (result) {
				// should not fail, dump log
				logLog();
			}
		}
	};
	
	CC7_CREATE_UNIT_TEST(cc7Testception, "cc7")
	
} // cc7::tests
} // cc7
