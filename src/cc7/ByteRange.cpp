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

#include <cc7/ByteRange.h>
#include <cc7/Base64.h>
#include <cc7/HexString.h>

namespace cc7
{
	std::string ByteRange::base64String(size_t wrap_size) const
	{
		std::string result;
		Base64_Encode(*this, wrap_size, result);
		return result;
	}
	
	std::string ByteRange::hexString(bool lower_case) const
	{
		std::string result;
		HexString_Encode(*this, lower_case, result);
		return result;
	}

} // cc7