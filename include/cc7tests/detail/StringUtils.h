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
	 Returns formatted std::string like regular sprintf() does.
	 */
	std::string FormattedString(const char * format, ...);
	
	/**
	 Helper split function.
	 */
	std::vector<std::string> & SplitString(const std::string & str, char delim, std::vector<std::string> & elems);
	
	/**
	 Returns a vector containing substrings from the input string that have been divided by a given delimiter.
	 */
	std::vector<std::string>   SplitString(const std::string & str, char delimiter);
	
	/**
	 Workarounds for missing STL conversion routines on Android/GNU-STL.
	 */
	std::string to_string(int value);
	double to_double(const std::string & str);
	int64_t to_longint(const std::string & str);
	

	
} // cc7::tests::detail
} // cc7::tests
} // cc7
