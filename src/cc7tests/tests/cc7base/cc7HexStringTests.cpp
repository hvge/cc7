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
#include <cc7/CC7.h>
#include <algorithm>

namespace cc7
{
namespace tests
{
	class cc7HexStringTests : public UnitTest
	{
	public:
		cc7HexStringTests()
		{
			CC7_REGISTER_TEST_METHOD(testEncodeDecode);
		}
		
		// UNIT TESTS
		
		void testEncodeDecode()
		{
			// Write to HEX
			const byte data[] = {
				0x01, 0x11, 0x12, 0x23, 0x3f, 0x7f, 0x80, 0x93,
				0xf0, 0xff, 0xfe, 0x00, 0x01, 0xbd, 0xcc, 0xa8,
				0x01, 0x11, 0x12, 0x23, 0x3f, 0x7f, 0x80, 0x93,
				0xf0, 0xff, 0xfe, 0x00, 0x01, 0xbd, 0xcc, 0xa8,
				0x01, 0x11, 0x12, 0x23, 0x3f, 0x7f, 0x80, 0x93,
				0xf0, 0xff, 0xfe, 0x00, 0x01, 0xbd, 0xcc, 0xa8
			};
			
			const std::string expected("011112233F7F8093F0FFFE0001BDCCA8"
									   "011112233F7F8093F0FFFE0001BDCCA8"
									   "011112233F7F8093F0FFFE0001BDCCA8");
			for (size_t s = 0; s < sizeof(data); s++) {
				ByteRange d(data, s);
				std::string hex = ToHexString(d);
				std::string exp = expected.substr(0, s*2);
				ccstAssertEqual(hex, exp);
			}
			
			// Read from hex
			for (size_t s = 0; s < expected.length(); s += 2) {
				std::string hex = expected.substr(0, s);
				ByteArray d;
				bool result = HexString_Decode(hex, d);
				ccstAssertTrue(result);
				ccstAssertEqualMemSize(d.data(), data, s / 2);
				
				std::string lower = hex.c_str();
				std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
				hex = lower.c_str();
				result = HexString_Decode(hex, d);
				ccstAssertTrue(result);
				ccstAssertEqualMemSize(d.data(), data, s / 2);
				
				// Other forms
				ByteArray d2 = FromHexString(hex);
				ByteArray d3;
				result = d3.readFromHexString(hex);
				ccstAssertTrue(result);
				ccstAssertEqual(d, d2);
				ccstAssertEqual(d, d3);
				
				// back conversion
				ccstAssertEqual(hex, ToHexString(d, true));
				std::string hex3 = d.hexString(false);
				ccstAssertEqual(expected.substr(0, s), hex3);
			}
			
			// odd number of characters
			std::string hex(expected.c_str() + 1, expected.length() - 1);
			ByteArray d = FromHexString(hex);
			ccstAssertEqualMemArray(d.data(), data);
		
			// wrong strings
			hex = " ";
			bool result = HexString_Decode(hex, d);
			ccstAssertFalse(result);
			ccstAssertEqual(d.size(), 0);
			
			hex = "ABx";
			result = HexString_Decode(hex, d);
			ccstAssertFalse(result);
			ccstAssertEqual(d.size(), 0);
			
			hex = "ABCx";
			result = HexString_Decode(hex, d);
			ccstAssertFalse(result);
			ccstAssertEqual(d.size(), 0);
		}

	};
	
	CC7_CREATE_UNIT_TEST(cc7HexStringTests, "cc7")
	
} // cc7::tests
} // cc7