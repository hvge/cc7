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
#include <cc7tests/TestDirectory.h>

namespace cc7
{
namespace tests
{
	extern TestDirectory g_baseFiles;
	
	class tt7JSONReaderTests : public UnitTest
	{
	public:
		std::string _json1;
		
		tt7JSONReaderTests()
		{
			CC7_REGISTER_TEST_METHOD(testSimpleJsonString)
			CC7_REGISTER_TEST_METHOD(testSimpleJsonFile)
			CC7_REGISTER_TEST_METHOD(testComplexJson)
			
			loadJsonData();
		}
		
		void loadJsonData()
		{
			_json1 =  u8"{"
						" \"key1\" : \"value1\","
						" \"true\" : true,"
						" \"false\" : false,"
						" \"empty\" : null,"
						" \"object\" :"
						" { "
						"   \"xxx\" : \"this is xxx\","
						"   \"yyy\" : false,"
						"	 \"zzz\":{ "
						"     \"integer\" : 64,"
						"     \"double\"  : 6.4,"
						"     \"unicode1\":\"Švárny ščípäk\","
						"     \"unicode2\":\"\\u0160v\\u00E1rny \\u0161\\u010d\\u00eDp\\u00E4k\""
						"   }"
						" },"
						" \"array\" :"
						" ["
						"		\"a\", \"b\", \"c\", true,"
						"		{\"sub-array\":[1,2,3,4]},"
						"		{\"sub-array\":[1.1,2.2,3.3,4.4]},"
						"		{\"sub-array\":[-1,-1.1,1e3,3.2e-1]}"
						" ]"
						"}";
		}
		
		// UNIT TESTS
		
		void testSimpleJsonString()
		{
			JSONValue root;
			std::string error;
			bool result = JSON_ParseString(_json1, root, &error);
			if (!result) {
				ccstFailure("Parser failed with error: %s", error.c_str());
				return;
			}
			simpleJsonValidation(root);
		}
		
		void testSimpleJsonFile()
		{
			JSONValue root = JSON_ParseFile(g_baseFiles, "test-data/cc7base/json-simple.json");
			simpleJsonValidation(root);
		}
		
		void simpleJsonValidation(const JSONValue & root)
		{
			ccstAssertEqual(root.valueAtPath("key1").asString(), "value1");
			ccstAssertEqual(root.booleanAtPath("true"), true);
			ccstAssertEqual(root.booleanAtPath("false"), false);
			ccstAssertEqual(root.valueAtPath("empty").isNull(), true);
			ccstAssertEqual(root.valueAtPath("object").isType(JSONValue::Object), true);
			ccstAssertEqual(root.valueAtPath("array").isType(JSONValue::Array), true);
			
			ccstAssertEqual(root.stringAtPath("object.xxx"), "this is xxx");
			ccstAssertEqual(root.booleanAtPath("object.yyy"), false);
			ccstAssertEqual(root.integerAtPath("object.zzz.integer"), 64);
			ccstAssertEqual(root.doubleAtPath("object.zzz.double"), 6.4);
			ccstAssertEqual(root.stringAtPath("object.zzz.unicode1"), u8"Švárny ščípäk");
			ccstAssertEqual(root.stringAtPath("object.zzz.unicode2"), u8"Švárny ščípäk");
			
			auto&& array = root.arrayAtPath("array");
			ccstAssertEqual(array[0].asString(), "a");
			ccstAssertEqual(array[1].asString(), "b");
			ccstAssertEqual(array[2].asString(), "c");
			ccstAssertEqual(array[3].asBoolean(), true);
			ccstAssertEqual(array[4].isType(JSONValue::Object), true);
			ccstAssertEqual(array[5].isType(JSONValue::Object), true);
			ccstAssertEqual(array[6].isType(JSONValue::Object), true);
			
			auto&& array2 = array[4].arrayAtPath("sub-array");
			ccstAssertEqual(array2[0].asInteger(), 1);
			ccstAssertEqual(array2[1].asInteger(), 2);
			ccstAssertEqual(array2[2].asInteger(), 3);
			ccstAssertEqual(array2[3].asInteger(), 4);
			auto&& array3 = array[5].arrayAtPath("sub-array");
			ccstAssertEqual(array3[0].asDouble(), 1.1);
			ccstAssertEqual(array3[1].asDouble(), 2.2);
			ccstAssertEqual(array3[2].asDouble(), 3.3);
			ccstAssertEqual(array3[3].asDouble(), 4.4);
			auto&& array4 = array[6].arrayAtPath("sub-array");
			ccstAssertEqual(array4[0].asInteger(), -1);
			ccstAssertEqual(array4[1].asDouble(), -1.1);
			ccstAssertEqual(array4[2].asDouble(), 1e3);
			ccstAssertEqual(array4[3].asDouble(), 3.2e-1);
		}

		
		void testComplexJson()
		{
			JSONValue complex = JSON_ParseFile(g_baseFiles, "test-data/cc7base/json-complex.json");
			ccstAssertTrue(complex.isType(JSONValue::Object));
		}

	};
	
	CC7_CREATE_UNIT_TEST(tt7JSONReaderTests, "cc7 test")
	
} // cc7::tests
} // cc7