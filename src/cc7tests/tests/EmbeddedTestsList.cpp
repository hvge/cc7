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

namespace cc7
{
namespace tests
{
	const UnitTestCreationInfoList _GetDefaultUnitTestCreationInfoList()
	{
		UnitTestCreationInfoList list;
		
		// cc7 framework tests
		CC7_ADD_UNIT_TEST(cc7PlatformTests, list);
		CC7_ADD_UNIT_TEST(cc7Testception, list);
		CC7_ADD_UNIT_TEST(cc7ByteArrayTests, list);
		CC7_ADD_UNIT_TEST(cc7ByteRangeTests, list);
		
		return list;
	}
	
} // cc7::tests
} // cc7