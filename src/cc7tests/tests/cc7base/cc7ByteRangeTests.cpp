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
#include <cc7/ByteRange.h>

namespace cc7
{
namespace tests
{
	class cc7ByteRangeTests : public UnitTest
	{
	public:
		cc7ByteRangeTests()
		{
			CC7_REGISTER_TEST_METHOD(testCreation)
			CC7_REGISTER_TEST_METHOD(testSquareBracketOperator)
			CC7_REGISTER_TEST_METHOD(testRelationalOperators)
		}
		
		// Unit tests
		
		void testCreation()
		{

		}
		
		void testSquareBracketOperator()
		{
		}
		
		void testRelationalOperators()
		{
		}
	};
	
	CC7_CREATE_UNIT_TEST(cc7ByteRangeTests, "cc7")
	
} // cc7::tests
} // cc7