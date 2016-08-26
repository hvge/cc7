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

#include <cc7tests/TestManager.h>
#include <cc7tests/PerformanceTimer.h>
#include <cc7tests/detail/StringUtils.h>

#include <cc7/DebugFeatures.h>
#include <algorithm>

namespace cc7
{
namespace tests
{
	// ------------------------------------------------------------------------------------
	// MARK: Instantiation
	
	TestManager::TestManager() :
		_test_manager_name("CC7"),
		_assertion_breakpoint_enabled(false),
		_log_capturig_enabled(false),
		_old_assertion_setup({nullptr, nullptr}),
		_old_log_setup({nullptr, nullptr})
	{
		
	}
	
	
	TestManager::~TestManager()
	{
	}
	
	extern const UnitTestCreationInfoList _GetDefaultUnitTestCreationInfoList();
	
	TestManager * TestManager::createDefaultManager()
	{
		TestManager * tm = new TestManager();
		tm->addUnitTestList(_GetDefaultUnitTestCreationInfoList());
		return tm;
	}
	
	
	TestManager * TestManager::createEmptyManager()
	{
		return new TestManager();
	}
	
	
	void TestManager::releaseManager(cc7::tests::TestManager *manager)
	{
		delete manager;
	}
	
	
	
	// ------------------------------------------------------------------------------------
	// MARK: Breakpoints configuration
	
	void TestManager::setAssertionBreakpointEnabled(bool enabled)
	{
		_assertion_breakpoint_enabled = enabled;
	}
	
	bool TestManager::assertionBreakpointEnabled() const
	{
		return _assertion_breakpoint_enabled;
	}
	
	void TestManager::setIncidentBreakpointEnabled(bool enabled)
	{
		tl().setIncidentBreakpointEnabled(enabled);
	}
	
	bool TestManager::incidentBreakpointEnabled() const
	{
		return tl().incidentBreakpointEnabled();
	}
	
	void TestManager::setLogCapturingEnabled(bool enabled)
	{
		_log_capturig_enabled = enabled;
	}
	
	bool TestManager::logCapturingEnabled() const
	{
		return _log_capturig_enabled;
	}
	
	
	
	// ------------------------------------------------------------------------------------
	// MARK: Tests registration
	
	void TestManager::addUnitTest(UnitTestCreationInfo test_info)
	{
		bool valid_info = test_info != nullptr &&
						  test_info->factory != nullptr &&
						  test_info->name != nullptr;
		if (CC7_CHECK(valid_info, "test_info is NULL or contains invalid information")) {
			if (std::find(_registered_tests.begin(), _registered_tests.end(), test_info) == _registered_tests.end()) {
				// Not added yet
				_registered_tests.push_back(test_info);
			} else {
				CC7_LOG("TestManager: '%s' is already registered in this manager.", test_info->name);
			}
		}
	}
	
	
	void TestManager::addUnitTestList(const UnitTestCreationInfoList & test_info_list)
	{
		_registered_tests.reserve(_registered_tests.size() + test_info_list.size());
		for (auto test_info : test_info_list) {
			addUnitTest(test_info);
		}
	}
	
	
	
	// ------------------------------------------------------------------------------------
	// MARK: Tests execution
	
	bool TestManager::runAllTests()
	{
		const std::string empty;
		return runTestsWithFilter(empty, empty);
	}

	
	bool TestManager::runTestsWithFilter(const std::string & incl, const std::string & excl)
	{
		// Keep current assertion handler and setup ours
		setupAssertionHandler();
		setupLogCapturingHandler();
		
		// Prepare tags for filtering
		auto included_tags = detail::SplitString(incl, ' ');
		auto excluded_tags = detail::SplitString(excl, ' ');
		
		_test_log.clearLogData();
		
		logHeader("== " + _test_manager_name + " Unit Tests Log");
		
		if (included_tags.size() > 0 || excluded_tags.size() > 0) {
			if (included_tags.size()) {
				logMessage(detail::FormattedString(" * included tags : %s", incl.c_str()));
			}
			if (excluded_tags.size()) {
				logMessage(detail::FormattedString(" * excluded tags : %s", excl.c_str()));
			}
			logSeparator();
		}

		bool tests_result = true;
		double elapsed_time = 0.0;
		
		if (_registered_tests.size() > 0) {
			//
			PerformanceTimer timer;
			tests_result = executeFilteredTests(included_tags, excluded_tags);
			elapsed_time = timer.elapsedTime();
			//
		} else {
			//
			logMessage("WARNING: There's no registered test!");
			//
		}

		// Keep elapsed time & report results to log
		tl().setElapsedTime(elapsed_time);
		TestLogData::Counters log_data_counters = tl().logDataCounters();
		logHeader(detail::FormattedString("== RESULTS:  %d passed,  %d failed,  %d skipped,  time %s",
										  log_data_counters.passed_tests,
										  log_data_counters.failed_tests,
										  log_data_counters.skipped_tests,
										  PerformanceTimer::humanReadableTime(elapsed_time).c_str()));
		
		// Set previous assertion handler back
		restoreLogCapturingHandler();
		restoreAssertionHandler();
		return tests_result;
	}
	
	
	static std::string BuildFullTestDescription(UnitTestCreationInfo ti, size_t index, size_t count)
	{
		return detail::FormattedString("Test [ %d / %d ] ::: %s", (int)index + 1, (int)count, ti->name);
	}
	
	bool TestManager::executeFilteredTests(const std::vector<std::string> & included_tags, const std::vector<std::string> & excluded_tags)
	{
		bool final_result = true;
		
		bool include_all = included_tags.size() == 0;
		bool no_excludes = excluded_tags.size() == 0;
		
		size_t test_index = 0;
		for (auto ti : _registered_tests) {
			// Build text for headers
			std::string full_test_desc = BuildFullTestDescription(ti, test_index++, _registered_tests.size());
			//
			// apply test filter
			//
			bool should_run = false;
			if (no_excludes && include_all) {
				// no filter
				should_run = true;
			} else if (ti->tags) {
				// has tags, split test tags by space
				std::vector<std::string> test_tags = detail::SplitString(std::string(ti->tags), ' ');
				if (test_tags.size() > 0) {
					bool is_included = false;
					if (!include_all) {
						for (std::string incl : included_tags) {
							if (std::find(test_tags.begin(), test_tags.end(), incl) != test_tags.end()) {
								is_included = true;		// found tag from included vector
								break;
							}
						}
					} else {
						is_included = true;
					}
					bool is_excluded = false;
					for (std::string excl : excluded_tags) {
						if (std::find(test_tags.begin(), test_tags.end(), excl) != test_tags.end()) {
							is_excluded = true;		// found tag from excluded vector
							break;
						}
					}
					should_run = is_included && !is_excluded;
					
				} else {
					// test tags string has wrong format, assume that there's no tag at all
					should_run = include_all;
				}
			} else {
				// test has no tags, if included is not present, then ignore this test
				should_run = include_all;
			}
			if (should_run) {
				bool test_result = executeTest(ti, full_test_desc);
				if (test_result) {
					tl().addPassedTest();
				} else {
					tl().addFailedTest();
				}
				final_result = final_result && test_result;
			} else {
				std::string skipped = full_test_desc + " ::: SKIPPED";
				logMessage(skipped);
				tl().addSkippedTest();
			}
		}
		
		return final_result;
	}
	
	bool TestManager::executeTest(UnitTestCreationInfo ti, const std::string & full_test_desc)
	{
		bool test_result = false;
		
		detail::UnitTestFactoryFunction test_factory = ti->factory;
		UnitTest * unit_test = test_factory ? test_factory() : nullptr;
		
		if (CC7_CHECK(unit_test != nullptr, "Unable to create test '%s'", unit_test)) {
			std::string begin_message = full_test_desc + " ::: START";
			logMessage(begin_message);
			
			// Set indentation and run test
			tl().setIndentationLevel(2);
			PerformanceTimer timer;
			double elapsed_time = 0.0;
			
			try {
				test_result = unit_test->runTest(this, &_test_log);
				elapsed_time = timer.elapsedTime();
			} catch (std::exception & exc) {
				std::string message("FAILED: Exception: ");
				message.append(exc.what());
				logMessage(message);
				test_result = false;
			} catch (...) {
				logMessage(std::string("FAILED: An unknown exception occured."));
				test_result = false;
			}
			
			// Clear indentation & dump result
			tl().setIndentationLevel(0);
			
			std::string end_message = full_test_desc;
			if (test_result) {
				end_message += " ::: OK ::: " + PerformanceTimer::humanReadableTime(elapsed_time);
			} else {
				end_message += " ::: FAILED";
			}
			logMessage(end_message);
			logSeparator();
			
			// destroy unit test object
			delete unit_test;
		}
		
		return test_result;
	}
	
	// ------------------------------------------------------------------------------------
	// MARK: Logging
	
	void TestManager::setTestManagerName(const std::string &name)
	{
		_test_manager_name = name;
	}
	
	const std::string & TestManager::testManagerName() const
	{
		return _test_manager_name;
	}
	
	static const std::string s_normal_line("----------------------------------------------------------------------------");
	static const std::string s_header_line("============================================================================");
	static const size_t min_line = 12;
	
	void TestManager::logHeader(const std::string & message)
	{
		size_t indent = tl().indentationLevel();
		indent = std::min(indent, s_header_line.length() - min_line);
		const char * line_begin = s_header_line.c_str() + indent;
		tl().logFormattedMessage("%s\n%s\n%s", line_begin, message.c_str(), line_begin);
	}
	
	
	void TestManager::logMessage(const std::string & message)
	{
		tl().logMessage(message);
	}
	
	
	void TestManager::logSeparator()
	{
		size_t indent = tl().indentationLevel();
		indent = std::min(indent, s_normal_line.length() - min_line);
		const char * line_begin = s_normal_line.c_str() + indent;
		tl().logMessage(line_begin);
	}
	
	// Log capturing
	
	void TestManager::_LogHandler(void * handler_data, const char * message)
	{
		TestManager * manager = reinterpret_cast<TestManager*>(handler_data);
		if (manager && manager->_log_capturig_enabled) {
			std::string msg;
			size_t len = strlen(message);
			msg.reserve(len + 7);
			msg.assign("> CC7: ");
			msg.append(message, len);
			manager->tl().logMessage(msg);
		}
	}
	
	void TestManager::setupLogCapturingHandler()
	{
#ifdef ENABLE_CC7_LOG
		_old_log_setup = cc7::debug::GetLogHandler();
		cc7::debug::SetLogHandler({_LogHandler, this});
#endif
	}
	
	void TestManager::restoreLogCapturingHandler()
	{
#ifdef ENABLE_CC7_LOG
		cc7::debug::SetLogHandler(_old_log_setup);
		_old_log_setup = {nullptr, nullptr};
#endif
	}

	void TestManager::systemLog(const char * message)
	{
#ifdef ENABLE_CC7_LOG
		if (_log_capturig_enabled) {
			// Log capturing is enabled, we have to put message directly to the
			// previously used handler
			if (_old_log_setup.handler_data) {
				_old_log_setup.handler(_old_log_setup.handler_data, message);
			}
		} else {
			CC7_LOG("%s", message);
		}
#endif
	}

	
	// ------------------------------------------------------------------------------------
	// MARK: Private assertion handler
	
	void TestManager::_AssertionHandler(void * handler_data, const char * file, int line, const char * message)
	{
		TestManager * manager = reinterpret_cast<TestManager*>(handler_data);
		if (manager) {
			manager->tl().logMessage(message);
			manager->addAssertion(message);
		}
	}
	
	void TestManager::addAssertion(const char * message)
	{
		if (_assertion_breakpoint_enabled) {
			systemLog(message);
			CC7_BREAKPOINT();
		}
	}

	void TestManager::setupAssertionHandler()
	{
#ifdef ENABLE_CC7_ASSERT
		_old_assertion_setup = cc7::debug::GetAssertionHandler();
		cc7::debug::SetAssertionHandler({_AssertionHandler, this});
#endif
	}
	
	void TestManager::restoreAssertionHandler()
	{
#ifdef ENABLE_CC7_ASSERT
		cc7::debug::SetAssertionHandler(_old_assertion_setup);
		_old_assertion_setup = { nullptr, nullptr };
#endif
	}

}
}
