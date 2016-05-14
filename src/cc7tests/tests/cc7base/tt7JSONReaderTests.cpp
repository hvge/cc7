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

#include <cc7tests/CC7Tests.h>
#include <cc7tests/JSONReader.h>

namespace cc7
{
namespace tests
{
	class tt7JSONReaderTests : public UnitTest
	{
	public:
		std::string _json1;
		
		tt7JSONReaderTests()
		{
			CC7_REGISTER_TEST_METHOD(testSimpleJson)
			
			loadJsonData();
		}
		
		void loadJsonData()
		{
			_json1 = "{"
					 " \"key1\" : \"value1\","
					 " \"true\" : true,"
					 " \"false\" : false,"
					 " \"empty\" : null,"
					 " \"object\" :"
					 " {"
					 "   \"xxx\" : \"this is xxx\","
					 "   \"yyy\" : false"
					 " },"
					 " \"array\" :"
					 " ["
					 "     \"a\", \"b\", \"c\", true"
					 " ]"
					 "}";
		}
		
		// UNIT TESTS
		
		void testSimpleJson()
		{
			JSONValue root;
			std::string error;
			bool result = JSON_ReadString(_json1, root, &error);
			if (!result) {
				ccstFailure("Parser failed with error: %s", error.c_str());
				return;
			}
			
			ccstAssertEqual(root.objectAtPath("key1").asString(), "value1");
			ccstAssertEqual(root.objectAtPath("true").asBoolean(), true);
			ccstAssertEqual(root.objectAtPath("false").asBoolean(), false);
			ccstAssertEqual(root.objectAtPath("empty").isNull(), true);
			ccstAssertEqual(root.objectAtPath("object").isType(JSONValue::Object), true);
			ccstAssertEqual(root.objectAtPath("array").isType(JSONValue::Array), true);
			
			ccstAssertEqual(root.objectAtPath("object.xxx").asString(), "this is xxx");
			ccstAssertEqual(root.objectAtPath("object.yyy").asBoolean(), false);
			
			auto & array = root.objectAtPath("array").asArray();
			ccstAssertEqual(array[0].asString(), "a");
			ccstAssertEqual(array[1].asString(), "b");
			ccstAssertEqual(array[2].asString(), "c");
			ccstAssertEqual(array[3].asBoolean(), true);
		}

	};
	
	CC7_CREATE_UNIT_TEST(tt7JSONReaderTests, "cc7 test")
	
} // cc7::tests
} // cc7