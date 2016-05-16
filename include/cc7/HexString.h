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
	 Converts input byte range into hexadecimal upper, or lowercase string. 
	 The function always returns true.
	 */
	bool HexString_Encode(const ByteRange & in_data, bool use_lowercase, std::string & out_string);
	
	/**
	 Converts hexadecimal encoded string into ByteArray. Returns false if the
	 input string is not a valid hexadecimal string.
	 */
	bool HexString_Decode(const std::string & in_string, ByteArray & out_data);
	
	/**
	 Converts input byte range into hexadecimal upper, or lowercase string. 
	 This variant of encoding function may be easier to use, but unlike 
	 the HexString_Encode(), you are not able to determine whether the error
	 occured or not.
	 */
	inline std::string ToHexString(const ByteRange & data, bool use_lowercase = false)
	{
		std::string result;
		HexString_Encode(data, use_lowercase, result);
		return result;
	}
	
	/**
	 Converts hexadecimal encoded string into ByteArray. This variant of decoding
	 function may be easier to use, but unlike the HexString_Decode(), you are not
	 able to determine whether the error occured or not.
	 */
	inline ByteArray FromHexString(const std::string & string)
	{
		ByteArray result;
		HexString_Decode(string, result);
		return result;
	}
	
} // cc7
