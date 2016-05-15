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

#include <cc7/Platform.h>
#include <cc7tests/JSONValue.h>

namespace cc7
{
namespace tests
{
	//
	// Yet another JSON parser :)
	//
	
	class TestFile;
	class TestDirectory;
	
	bool JSON_ParseString(const std::string & str, JSONValue & out_value, std::string * out_error = nullptr);
	
	bool JSON_ParseData(const ByteRange & range, JSONValue & out_value, std::string * out_error = nullptr);
	
	JSONValue JSON_ParseFile(const TestDirectory & dir, const std::string & file_name);
	
} // cc7::tests
} // cc7