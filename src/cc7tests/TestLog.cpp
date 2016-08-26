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

#include <cc7tests/TestLog.h>
#include <cc7tests/detail/StringUtils.h>
#include <memory>
#include <string>

namespace cc7
{
namespace tests
{
	// MARK: Macros & Declarations
	
#define GUARD_LOCK()	std::lock_guard<std::mutex> lock_guard(*_lock)
	
	static const std::string newline("\n");
	
	static const char * _LookForFileName(const char * path);
	static void			_AppendMultilineString(const std::string & str, const std::string & indentation, std::string & dest_string);
	
	
	
	// MARK: Construction / Destruction
	
	TestLog::TestLog() :
		_lock(new std::mutex()),
		_dump_to_system_log(false),
		_incident_breakpoint(false)
	{
		_log_data.log.reserve(2048);
		_log_data.incidents.reserve(1024);
		_indentation.reserve(64);
	}
	
	
	TestLog::~TestLog()
	{
		delete _lock;
	}
	
	
	
	// MARK: Logging
	
	void TestLog::appendMultilineString(const std::string & string)
	{
//		if (_dump_to_system_log) {
//			CC7_LOG("%s", string.c_str());
//		}
		_AppendMultilineString(string, _indentation, _log_data.log);
	}
	
	void TestLog::logMessage(const char * message)
	{
		GUARD_LOCK();
		appendMultilineString(std::string(message));
	}
	
	
	void TestLog::logMessage(const std::string & message)
	{
		GUARD_LOCK();
		appendMultilineString(message);
	}
	
	
	void TestLog::logFormattedMessage(const char * format, ...)
	{
		std::string message;
		
		va_list ap;
		va_start(ap, format);
		int expected_size = vsnprintf(NULL, 0, format, ap);
		va_end(ap);
		if (CC7_CHECK(expected_size >= 0, "vnsprintf() error occured. Result %d", expected_size)) {
			std::unique_ptr<char[]> buffer;
			buffer.reset(new char[expected_size + 1]);
			va_start(ap, format);
			int processed_size = vsnprintf(&buffer[0], expected_size + 1, format, ap);
			va_end(ap);
			if (CC7_CHECK(expected_size == processed_size, "vsnprintf() expected and processed size are different. %d vs %d", expected_size, processed_size)) {
				message.assign(buffer.get());
			}
		}
		// Lock now. All previous operations were local.
		GUARD_LOCK();
		appendMultilineString(message);
	}
	
	
	void TestLog::logIncident(const char * full_path, int line, const char * condition, const char * format, ...)
	{
#define BUF_COUNT(b) (sizeof(b)/sizeof(b[0]))
		// At first, we have to process formatted message.
		char formatted_message[1024];
		va_list args;
		va_start(args, format);
		vsnprintf(formatted_message, BUF_COUNT(formatted_message), format, args);
		va_end(args);
		
		// Crop the file path just to file name. If this is not possible then use a whole path.
		const char * file_name = _LookForFileName(full_path);
		
		// Build final string
		char message_buffer[1024+256];
		bool empty_formatted_message = !strlen(formatted_message);
		if (condition) {
			// This is incident with condition string (typically some assertion)
			if (!empty_formatted_message) {
				// Also contains message, dump  file, line, condition, buffer
				snprintf(message_buffer, BUF_COUNT(message_buffer), "FAIL: %s, %d: %s  =>  %s", file_name, line, condition, formatted_message);
			} else {
				// No message, just dump        file, line, condition
				snprintf(message_buffer, BUF_COUNT(message_buffer), "FAIL: %s, %d: %s", file_name, line, condition);
			}
		} else {
			// Incident without condition (typically direct failure)
			if (!empty_formatted_message) {
				// ...but still have some message
				snprintf(message_buffer, BUF_COUNT(message_buffer), "FAIL: %s, %d: %s", file_name, line, formatted_message);
			} else {
				// no condition, no message
				snprintf(message_buffer, BUF_COUNT(message_buffer), "FAIL: %s, %d", file_name, line);
			}
		}
#undef BUF_COUNT
		
		std::string message(message_buffer);
		bool dump_to_syslog;
		bool break_execution;
		
		_lock->lock();
		{
			// Look for already reported location
			std::string file_location_key(full_path);
			file_location_key.append(":");
			file_location_key.append(cc7::tests::detail::to_string(line));
			
			if (_incident_locations_set.find(file_location_key) == _incident_locations_set.end()) {
				// New incident, store key to locations set
				_incident_locations_set.insert(file_location_key);
				// Append message to log
				_AppendMultilineString(message, _indentation,  _log_data.log);
				_AppendMultilineString(message, std::string(), _log_data.incidents);
			}
			
			_log_data.c.incidents_count += 1;
			_log_data.c.current_test_incidents_count += 1;
			
			dump_to_syslog = _dump_to_system_log && !_incident_breakpoint;
			break_execution = _incident_breakpoint;
		}
		_lock->unlock();
		
		if (dump_to_syslog) {
			CC7_LOG("%s", message_buffer);
		}
		if (break_execution) {
			CC7_LOG("%s", message_buffer);
			CC7_BREAKPOINT();
		}
	}

	
	
	// MARK: Log configuration
	
	void TestLog::setDumpToSystemLogEnabled(bool enable)
	{
		GUARD_LOCK();
		_dump_to_system_log = enable;
	}
	
	
	bool TestLog::dumpToSystemLogEnabled() const
	{
		GUARD_LOCK();
		return _dump_to_system_log;
	}
	
	
	void TestLog::setIncidentBreakpointEnabled(bool enable)
	{
		GUARD_LOCK();
		_incident_breakpoint = enable;
	}
	
	
	bool TestLog::incidentBreakpointEnabled() const
	{
		GUARD_LOCK();
		return _incident_breakpoint;
	}

	
	// MARK: Log results and control

	TestLogData TestLog::logData() const
	{
		GUARD_LOCK();
		return _log_data;
	}
	
	
	TestLogData::Counters TestLog::logDataCounters() const
	{
		GUARD_LOCK();
		return _log_data.c;
	}
	
	
	void TestLog::clearLogData()
	{
		GUARD_LOCK();
		
		_log_data.reset();
		_incident_locations_set.clear();
		_indentation.clear();
		_indentation_prefix.clear();
		_indentation_suffix.clear();
	}
	
	
	void TestLog::clearCurrentTestIncidentsCount()
	{
		GUARD_LOCK();
		_log_data.c.current_test_incidents_count = 0;
	}
	
	
	
	// MARK: Indentation
	
	void TestLog::setIndentationLevel(size_t indentation_level)
	{
		GUARD_LOCK();
		updateIndentationToLevel(indentation_level);
	}
	
	
	size_t TestLog::indentationLevel() const
	{
		GUARD_LOCK();
		return indentationLevelImpl();
	}
	

	void TestLog::setIndentationPrefix(const std::string & prefix)
	{
		GUARD_LOCK();
		size_t current_level = indentationLevelImpl();
		_indentation_prefix = prefix;
		updateIndentationToLevel(current_level);
	}
	
	
	std::string TestLog::indentationPrefix() const
	{
		GUARD_LOCK();
		return _indentation_prefix;
	}
	
	
	void TestLog::setIndentationSuffix(const std::string & suffix)
	{
		GUARD_LOCK();
		size_t current_level = indentationLevelImpl();
		_indentation_suffix = suffix;
		updateIndentationToLevel(current_level);
	}
	
	
	std::string TestLog::indentationSuffix() const
	{
		GUARD_LOCK();
		return _indentation_suffix;
	}

	
	void TestLog::updateIndentationToLevel(size_t level)
	{
		_indentation = _indentation_prefix;
		_indentation.append(level, ' ');
		_indentation.append(_indentation_suffix);
		
	}
	
	size_t TestLog::indentationLevelImpl() const
	{
		return _indentation.length() - _indentation_prefix.length() - _indentation_suffix.length();
	}

	
	// MARK: Passed / Failed counters
	
	void TestLog::addPassedTest()
	{
		GUARD_LOCK();
		_log_data.c.passed_tests++;
		_log_data.c.executed_tests++;
	}
	
	
	void TestLog::addFailedTest()
	{
		GUARD_LOCK();
		_log_data.c.failed_tests++;
		_log_data.c.executed_tests++;
	}
	
	
	void TestLog::addSkippedTest()
	{
		GUARD_LOCK();
		_log_data.c.skipped_tests++;
	}
	
	
	void TestLog::setElapsedTime(double time)
	{
		GUARD_LOCK();
		_log_data.c.elapsed_time = time;
	}

	
	
	// MARK: Helper Functions
	
	static void _AppendMultilineString(const std::string & str, const std::string & indentation, std::string & dest_string)
	{
		size_t nl_pos = str.find(newline);
		if (nl_pos == str.npos) {
			// no newline
			dest_string.append(indentation);
			dest_string.append(str);
			dest_string.append(newline);
			return;
		}
		size_t offset = 0;
		while (nl_pos != str.npos && offset < str.length()) {
			dest_string.append(indentation);
			dest_string.append(str, offset, nl_pos - offset);
			dest_string.append(newline);
			offset = nl_pos + newline.length();
			nl_pos = str.find(newline, offset);
		}
		if (offset < str.length()) {
			dest_string.append(indentation);
			dest_string.append(str, offset, dest_string.npos);
			dest_string.append(newline);
		}
	}
	
	
	static const char * _LookForFileName(const char * path)
	{
#if defined(CC7_WINDOWS)
		const char kPlatformPathSeparator    = '\\';
		const char kPlatformPathSeparatorAlt = '/';
#else
		const char kPlatformPathSeparator    = '/';
		const char kPlatformPathSeparatorAlt = '\\';
#endif
		if (!path) {
			return "<<null-path>>";
		}
		const char * delimiter = strrchr(path, kPlatformPathSeparator);
		if (!delimiter) {
			delimiter = strrchr(path, kPlatformPathSeparatorAlt);
			if (!delimiter) {
				return path;
			}
		}
		return delimiter + 1;
	}
	
}
}
