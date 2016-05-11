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

#include <cc7/ByteArray.h>
#include <cc7/Base64.h>
#include <cc7/HexString.h>

namespace cc7
{	
	bool ByteArray::readFromBase64String(const std::string & base64_string, size_t wrap_size)
	{
		return Base64_Decode(base64_string, wrap_size, *this);
	}
	
	bool ByteArray::readFromHexString(const std::string & hex_string)
	{
		return HexString_Decode(hex_string, *this);
	}
	
	std::string ByteArray::base64String(size_t wrap_size) const
	{
		std::string result;
		Base64_Encode(this->byteRange(), wrap_size, result);
		return result;
	}
	
	std::string ByteArray::hexString(bool lower_case) const
	{
		std::string result;
		HexString_Encode(this->byteRange(), lower_case, result);
		return result;
	}
	
} // cc7
