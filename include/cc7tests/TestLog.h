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
#include <string>
#include <mutex>
#include <set>

namespace cc7
{
namespace tests
{
	/**
	 The TestLogData structure contains all testing information
	 collected during the testing session.
	 */
	struct TestLogData
	{
		struct Counters
		{
			Counters() :
				incidents_count(0),
				current_test_incidents_count(0),
				executed_tests(0),
				passed_tests(0),
				failed_tests(0),
				skipped_tests(0),
				elapsed_time(0.0)
			{
			}
			
			void reset()
			{
				incidents_count = 0;
				current_test_incidents_count = 0;
				executed_tests = 0;
				passed_tests = 0;
				failed_tests = 0;
				skipped_tests = 0;
				elapsed_time = 0.f;
			}

			/**
			 Number of incidents valid for a whole testing session.
			 */
			int incidents_count;
			/**
			 Number of incidents valid for running test.
			 */
			int current_test_incidents_count;
			
			/**
			 Number of executed tests;
			 */
			int executed_tests;
			/**
			 Number of passed tests.
			 */
			int passed_tests;
			/**
			 Number of failed tests.
			 */
			int failed_tests;
			/**
			 Number of skipped tests.
			 */
			int skipped_tests;
			/**
			 Time spent for tests.
			 */
			double elapsed_time;
		};
		
		TestLogData()
		{
		}
		
		void reset()
		{
			log.clear();
			incidents.clear();
			c.reset();
		}
		
		/**
		 Full log from testing.
		 */
		std::string log;
		/**
		 Contains only incidents.
		 */
		std::string incidents;
		/**
		 Contains all counters.
		 */
		Counters c;
	};
	
	/**
	 The TestLog class collects all testing information during the testing.
	 Typically, this class collects all logs and incidents which occured 
	 during the tests.
	 
	 The TestLog implementation is thread-safe.
	 */
	class TestLog
	{
	public:
		
		// Construction / Destruction
		
		TestLog();
		~TestLog();
		
		// Messages
		
		/**
		 Adds |message| to the test log.
		 */
		void logMessage(const char * message);
		
		/**
		 Adds |message| to the test log.
		 */
		void logMessage(const std::string & message);
		
		/**
		 Adds formatted message to the test log. The formatted string depends on
		 |format| string and optional parameters.
		 */
		void logFormattedMessage(const char * format, ...);

		/**
		 Adds incident to the test log. The |file| and |line| parameter determines unique
		 location of the incident and repetitive events are ignored. Only first occurence of the
		 same incident is added to the test log and incidents log. The optional |condition| parameter
		 should represent a failed condition, which triggered the incident report. The |format| 
		 string and subsequent optional parameters allows construction of almost arbitrary 
		 formatted message.
		 
		 Unlike logMessage or logFormattedMessage methods, the method adds incident to both test and
		 incidents log.
		 */
		void logIncident(const char * file, int line, const char * condition, const char * format, ...);

		
		// Indentation
		
		/**
		 Sets current indentation level to |indentation_level| value.
		 */
		void setIndentationLevel(size_t indentation_level);
		
		/**
		 Returns current indentation level.
		 */
		size_t indentationLevel() const;
		
		/**
		 Sets indentation prefix to desired |prefix| value. The final indentation is
		 constructed as |prefix| + |' ' x indentation_level| + |suffix|.
		 */
		void setIndentationPrefix(const std::string & prefix);
		
		/**
		 Returns current identation prefix.
		 */
		std::string indentationPrefix() const;

		/**
		 Sets indentation suffix to desired |suffix| value. The final indentation is
		 constructed as |prefix| + |' ' x indentation_level| + |suffix|.
		 */
		void setIndentationSuffix(const std::string & suffix);
		
		/**
		 Returns current identation suffix.
		 */
		std::string indentationSuffix() const;
		
		
		// Log configuration
		
		/**
		 Enables or disables passing messages to system log. The logging also depends
		 on whether the CC7_LOG() is enabled.
		 */
		void setDumpToSystemLogEnabled(bool enabled);
		
		/**
		 Returns whether the dumping to system log is enabled or not.
		 */
		bool dumpToSystemLogEnabled() const;
		
		/**
		 Enables or disables software breakpoint triggered on any incident.
		 */
		void setIncidentBreakpointEnabled(bool enabled);
		/**
		 Returns whether the incidents breakpoint is enabled or not.
		 */
		bool incidentBreakpointEnabled() const;
		
		
		// MARK: Log results and control
		
		/**
		 Returns copy of internal TestLogData structure with actual and full 
		 content of the testing log.
		 */
		TestLogData logData() const;
		
		/**
		 Returns copy of internal TestLogData::Counters structure with actual counters.
		 */
		TestLogData::Counters logDataCounters() const;
		
		/**
		 Clears whole content of test log including current level indentation.
		 */
		void clearLogData();
		
		/**
		 Clears incidents counter for current test. The test framework must call this
		 method before start of each test.
		 */
		void clearCurrentTestIncidentsCount();
		
		
		
		// MARK: Passed / Failed counters
		
		void addPassedTest();
		void addFailedTest();
		void addSkippedTest();
		void setElapsedTime(double time);
		
	private:
		
		// Private methods & members
		
		/**
		 Updates internal _indentation string, based on desired level of indentation.
		 The final string is constructed as |prefix| + |' ' x indentation_level| + |suffix|
		 */
		void updateIndentationToLevel(size_t level);
		
		void appendMultilineString(const std::string & string);
		
		size_t indentationLevelImpl() const;
		
		std::mutex *	_lock;
		
		std::string		_indentation;
		std::string		_indentation_prefix;
		std::string		_indentation_suffix;
		TestLogData		_log_data;
		
		std::set<std::string> _incident_locations_set;
		
		// flags
		bool			_dump_to_system_log;
		bool			_incident_breakpoint;
	};
	
	
} // cc7::tests
} // cc7