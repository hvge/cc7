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
	 Converts input byte range into Base64 encoded string. The function returns false
	 only if you provide an invalid |wrap_size| parameter.
	 */
	bool Base64_Encode(const ByteRange & in_data, size_t wrap_size, std::string & out_string);
	
	/**
	 Converts Base64 encoded string into ByteArray. If the |wrap_size| parameter is greater than 0
	 then the multiline input string is expected. In this case, the size of wrapping is just a hint
	 and the decoder can process strings with a different size of lines.
	 
	 Returns false if the string is not a valid Base64 string.
	 
	 Note that unlike the other Base64 implementations, this decoder treats invalid characters in the string
	 as an error. Other implementations usually stops processing at first invalid character.
	 */
	bool Base64_Decode(const std::string & in_string, size_t wrap_size, ByteArray & out_data);
	
	/**
	 Converts input byte range into Base64 encoded string. This variant of encoding function may be
	 easier to use, but unlike the Base64_Encode(), you are not able to determine whether
	 the error occured or not.
	 */
	inline std::string ToBase64String(const ByteRange & data, size_t wrap_size = 0)
	{
		std::string result;
		Base64_Encode(data, wrap_size, result);
		return result;
	}
	
	/**
	 Converts Base64 encoded string into ByteArray. This variant of decoding function may be
	 easier to use, but unlike the Base64_Decode(), you are not able to determine whether
	 the error occured or not.
	 */
	inline ByteArray FromBase64String(const std::string & string, size_t wrap_size = 0)
	{
		ByteArray result;
		Base64_Decode(string, wrap_size, result);
		return result;
	}
	
} // cc7
