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

#include <cc7tests/detail/StringUtils.h>
#include <sstream>
#include <memory>
#include <stdlib.h>

namespace cc7
{
namespace tests
{
namespace detail
{
	
	std::string FormattedString(const char * format, ...)
	{
		va_list ap;
		va_start(ap, format);
		int expected_size = vsnprintf(NULL, 0, format, ap);
		va_end(ap);
		if (CC7_CHECK(expected_size >= 0, "vsnprintf() encoding error occured: %d", expected_size)) {
			std::unique_ptr<char[]> buffer;
			buffer.reset(new char[expected_size + 1]);
			va_start(ap, format);
			int processed_size = vsnprintf(&buffer[0], expected_size + 1, format, ap);
			va_end(ap);
			if (CC7_CHECK(expected_size == processed_size, "vsnprintf() different exp. and processed size: %d vs %d", expected_size, processed_size)) {
				return std::string(buffer.get());
			}
		}
		return std::string();
	}
	

	std::vector<std::string> & SplitString(const std::string & str, char delim, std::vector<std::string> & elems)
	{
		std::stringstream ss(str);
		std::string item;
		while (std::getline(ss, item, delim)) {
			if (!item.empty()) {
				elems.push_back(item);
			}
		}
		return elems;
	}

	
	std::vector<std::string> SplitString(const std::string & str, char delimiter)
	{
		std::vector<std::string> elements;
		SplitString(str, delimiter, elements);
		return elements;
	}

	
} // cc7::tests::detail
} // cc7::tests
} // cc7
