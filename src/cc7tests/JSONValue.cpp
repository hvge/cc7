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

#include <cc7tests/JSONReader.h>
#include <cc7tests/detail/TestUtilities.h>
#include <cc7/HexString.h>
#include <cc7/Base64.h>

namespace cc7
{
namespace tests
{
	const JSONValue & JSONValue::valueAtPath(const std::string & path, Type expected_type) const
	{
		auto path_components = detail::SplitString(path, '.');
		if (path_components.empty()) {
			throw std::invalid_argument("The provided path is wrong or empty.");
		}
		if (!isType(Object)) {
			throw std::invalid_argument("JSONValue is not an Object. Key: *this*");
		}
		const JSONValue * selected_obj = this;
		for (auto&& key : path_components) {
			if (selected_obj->isType(JSONValue::Object)) {
				auto&& object_map = selected_obj->asObject();
				auto value = object_map.find(key);
				if (value != object_map.end()) {
					selected_obj = &value->second;
				}
			} else {
				throw std::invalid_argument("JSONValue is not an Object. Key: '" + key + "'");
			}
		}
		if (expected_type != NaT) {
			if (!selected_obj->isType(expected_type)) {
				throw std::invalid_argument("The selected JSONValue has unexpected type.");
			}
		}
		return *selected_obj;
	}
	
	cc7::ByteArray JSONValue::dataFromBase64StringAtPath(const std::string & path) const
	{
		ByteArray result;
		if (!Base64_Decode(stringAtPath(path), 0, result)) {
			throw std::invalid_argument("The selected string is not a Base64 string.");
		}
		return result;
	}
	
	cc7::ByteArray JSONValue::dataFromHexStringAtPath(const std::string & path) const
	{
		ByteArray result;
		if (!HexString_Decode(stringAtPath(path), result)) {
			throw std::invalid_argument("The selected string is not a hexadecimal string.");
		}
		return result;
	}

	
} // cc7::tests
} // cc7
