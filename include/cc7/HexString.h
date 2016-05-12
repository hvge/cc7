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
	 Converts input byte range into hexadecimal string.
	 */
	bool HexString_Encode(const ByteRange & in_data, bool use_lowercase, std::string & out_string);
	
	/**
	 Converts hexadecimal encoded string into ByteArray.
	 */
	bool HexString_Decode(const std::string & in_string, ByteArray & out_data);
	
	
	inline std::string ToHexString(const ByteRange & data, bool use_lowercase = false)
	{
		std::string result;
		HexString_Encode(data, use_lowercase, result);
		return result;
	}
	
	inline ByteArray FromHexString(const std::string & string)
	{
		ByteArray result;
		HexString_Decode(string, result);
		return result;
	}
	
} // cc7
