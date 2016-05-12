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

#include <cc7/ByteArray.h>

namespace cc7
{
	/**
	 Converts input byte range into Base64 encoded string.
	 */
	bool Base64_Encode(const ByteRange & in_data, size_t wrap_size, std::string & out_string);
	
	/**
	 Converts Base64 encoded string into ByteArray.
	 */
	bool Base64_Decode(const std::string & in_string, size_t wrap_size, ByteArray & out_data);
	
	
	inline std::string ToBase64String(const ByteRange & data, size_t wrap_size = 0)
	{
		std::string result;
		Base64_Encode(data, wrap_size, result);
		return result;
	}
	
	inline ByteArray FromBase64String(const std::string & string, size_t wrap_size = 0)
	{
		ByteArray result;
		Base64_Decode(string, wrap_size, result);
		return result;
	}
	
} // cc7
