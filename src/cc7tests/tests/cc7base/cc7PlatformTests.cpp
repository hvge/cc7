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

namespace cc7
{
namespace tests
{
	class cc7PlatformTests : public UnitTest
	{
	public:
		cc7PlatformTests()
		{
			CC7_REGISTER_TEST_METHOD(testPlatformBits)
			CC7_REGISTER_TEST_METHOD(testDebugFeatures)
			CC7_REGISTER_TEST_METHOD(testEndian16)
			CC7_REGISTER_TEST_METHOD(testEndian32)
			CC7_REGISTER_TEST_METHOD(testEndian64)
			CC7_REGISTER_TEST_METHOD(testEndianIntrinsics)
		}
		
		void testPlatformBits()
		{
#if defined(CC7_PLATFORM64)
			ccstAssertTrue(sizeof(size_t) == 8);
#elif defined(CC7_PLATFORM32)
			ccstAssertTrue(sizeof(size_t) == 4);
#else
			ccstFailure("CC7_PLATFORM32 or CC7_PLATFORM64 macro is not defined!");
#endif
			ccstAssertTrue(sizeof(cc7::byte) == 1);
			ccstAssertTrue(sizeof(cc7::U8)   == 1);
			ccstAssertTrue(sizeof(cc7::U16)  == 2);
			ccstAssertTrue(sizeof(cc7::U32)  == 4);
			ccstAssertTrue(sizeof(cc7::U64)  == 8);
		}
		
		void testDebugFeatures()
		{
#if defined(DEBUG)
			ccstAssertTrue(cc7::debug::HasDebugFeaturesTurnedOn());
#endif
#if defined(ENABLE_CC7_ASSERT)
			ccstAssertTrue(cc7::debug::HasDebugFeaturesTurnedOn());
#endif
#if defined(ENABLE_CC7_LOG)
			ccstAssertTrue(cc7::debug::HasDebugFeaturesTurnedOn());
#endif
			
#if !defined(DEBUG) && !defined(ENABLE_CC7_ASSERT) && !defined(ENABLE_CC7_LOG)
			ccstAssertFalse(cc7::error::HasDebugFeaturesTurnedOn());
#endif
		}
		
		void testEndianIntrinsics()
		{
#if !defined(CC7_BSWAP_16)
			ccstMessage("WARNING: This platform uses default impl. for 16 bit endian swap");
#endif
#if !defined(CC7_BSWAP_32)
			ccstMessage("WARNING: This platform uses default impl. for 32 bit endian swap");
#endif
#if !defined(CC7_BSWAP_64)
			ccstMessage("WARNING: This platform uses default impl. for 64 bit endian swap");
#endif
			
		}
		
		void testEndian16()
		{
			cc7::U16 u16src = 0x1122;
			
			cc7::byte u16BEBytes[2] = { 0x11, 0x22 };
			cc7::byte u16LEBytes[2] = { 0x22, 0x11 };
			
			cc7::U16 u16be = ToBigEndian(u16src);
			ccstAssertEqualMemArray(&u16be, u16BEBytes);
			cc7::U16 u16le = ToLittleEndian(u16src);
			ccstAssertEqualMemArray(&u16le, u16LEBytes);
			
			cc7::U16 u16dst = FromLittleEndian(u16le);
			ccstAssertEqual(u16src, u16dst);
			u16dst = FromBigEndian(u16be);
			ccstAssertEqual(u16src, u16dst);
		}
		
		void testEndian32()
		{
			cc7::U32 u32src = 0x11223344;
			
			cc7::byte u32BEBytes[4] = { 0x11, 0x22, 0x33, 0x44 };
			cc7::byte u32LEBytes[4] = { 0x44, 0x33, 0x22, 0x11 };
			
			cc7::U32 u32be = ToBigEndian(u32src);
			ccstAssertEqualMemArray(&u32be, u32BEBytes);
			cc7::U32 u32le = ToLittleEndian(u32src);
			ccstAssertEqualMemArray(&u32le, u32LEBytes);
			
			cc7::U32 u32dst = FromLittleEndian(u32le);
			ccstAssertEqual(u32src, u32dst);
			u32dst = FromBigEndian(u32be);
			ccstAssertEqual(u32src, u32dst);
		}
		
		void testEndian64()
		{
			cc7::U64 u64src = 0x1122334455667788LL;
			
			cc7::byte u64BEBytes[8] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88 };
			cc7::byte u64LEBytes[8] = { 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11 };
			
			cc7::U64 u64be = ToBigEndian(u64src);
			ccstAssertEqualMemArray(&u64be, u64BEBytes);
			cc7::U64 u64le = ToLittleEndian(u64src);
			ccstAssertEqualMemArray(&u64le, u64LEBytes);
			
			cc7::U64 u64dst = FromLittleEndian(u64le);
			ccstAssertEqual(u64src, u64dst);
			u64dst = FromBigEndian(u64be);
			ccstAssertEqual(u64src, u64dst);
		}
		
	};
	
	CC7_CREATE_UNIT_TEST(cc7PlatformTests, "cc7")
	
} // cc7::tests
} // cc7