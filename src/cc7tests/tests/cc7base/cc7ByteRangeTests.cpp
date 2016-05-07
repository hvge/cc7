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
#include <cc7/ByteArray.h>

namespace cc7
{
namespace tests
{
	typedef std::vector<cc7::byte> bvec;
	
	class cc7ByteRangeTests : public UnitTest
	{
	public:
		cc7ByteRangeTests()
		{
			CC7_REGISTER_TEST_METHOD(testCreation)
			CC7_REGISTER_TEST_METHOD(testAssign)
			CC7_REGISTER_TEST_METHOD(testSquareBracketOperator)
			CC7_REGISTER_TEST_METHOD(testRelationalOperators)
			CC7_REGISTER_TEST_METHOD(testIterators)
			CC7_REGISTER_TEST_METHOD(testCornerCases)
			CC7_REGISTER_TEST_METHOD(testSubRanges)
			CC7_REGISTER_TEST_METHOD(testOtherMethods)
		}
		
		// Helper methods
		
		std::string createSomeString()
		{
			return std::string("Don't panic!");
		}
		
		// Unit tests
		
		void testCreation()
		{
			const bvec v123   = {1, 2, 3};
			
			const std::string s1 = "Hello world";
			const char * chp1	 = "Hello world";
			const cc7::byte bp1[] = { 1, 2, 3, 4, 5, 6 };
			const void * some_ptr = bp1;
			
			ByteRange r1;
			ccstAssertTrue(r1.empty());
			ccstAssertEqual(r1.size(), 0);
			ccstAssertEqual(r1.begin(), r1.end());
			
			ByteRange r2(chp1);
			ccstAssertFalse(r2.empty());
			ccstAssertTrue(r2.size() == s1.size());
			ccstAssertTrue(memcmp(r2.data(), s1.data(), std::min(r2.size(), s1.size())) == 0);
			
			ByteRange r3(s1);
			ccstAssertFalse(r3.empty());
			ccstAssertTrue(r3.size() == strlen(chp1));
			ccstAssertTrue(memcmp(r3.data(), chp1, std::min(r3.size(), strlen(chp1))) == 0);

			// Keep alive s1...
			ccstAssertEqual(s1, chp1);
			
			ByteRange r4(v123.begin(), v123.end());
			ccstAssertTrue(r4.size() == v123.size());
			ccstAssertTrue(memcmp(r4.data(), v123.data(), std::min(r4.size(), v123.size())) == 0);
			
			ByteRange r5(bp1, sizeof(bp1));
			ccstAssertTrue(r5.size() == sizeof(bp1));
			ccstAssertTrue(memcmp(r5.data(), bp1, std::min(r5.size(), sizeof(bp1))) == 0);
			
			ByteRange r6(some_ptr, sizeof(bp1));
			ccstAssertTrue(r6.size() == sizeof(bp1));
			ccstAssertTrue(memcmp(r6.data(), some_ptr, std::min(r5.size(), sizeof(bp1))) == 0);

			
			/*
			 // This is an example of dangerous usage of ByteRange:

			 ByteRange r_dangerous(createSomeString());
			 
			 // At this point, ByteRange points to temporary created string,
			 // which may be destroyed immediately after the use.

			 const std::string s2 = "Don't panic!";
			 ccstAssertTrue(r_dangerous.size() == s2.size());
			 ccstAssertTrue(memcmp(r_dangerous.data(), s2.data(), std::min(r_dangerous.size(), s2.size())) == 0);
			*/

		}
		
		void testAssign()
		{
			const bvec v123   = {1, 2, 3};
			
			const std::string s1 = "Hello world";
			const char * chp1	 = "Hello world";
			const cc7::byte bp1[] = { 1, 2, 3, 4, 5, 6 };
			const void * some_ptr = bp1;
			
			ByteRange r1;
			ccstAssertTrue(r1.empty());
			ccstAssertEqual(r1.size(), 0);
			ccstAssertEqual(r1.begin(), r1.end());
			
			r1.assign(chp1);
			ccstAssertFalse(r1.empty());
			ccstAssertTrue(r1.size() == s1.size());
			ccstAssertTrue(memcmp(r1.data(), s1.data(), std::min(r1.size(), s1.size())) == 0);
			
			r1.assign(chp1, 5);
			ccstAssertEqual(to_string(r1), "Hello");
			
			r1.assign(s1);
			ccstAssertFalse(r1.empty());
			ccstAssertTrue(r1.size() == strlen(chp1));
			ccstAssertTrue(memcmp(r1.data(), chp1, std::min(r1.size(), strlen(chp1))) == 0);
			
			// Keep alive s1...
			ccstAssertEqual(s1, chp1);
			
			r1.assign(v123.begin(), v123.end());
			ccstAssertTrue(r1.size() == v123.size());
			ccstAssertTrue(memcmp(r1.data(), v123.data(), std::min(r1.size(), v123.size())) == 0);
			
			r1.assign(bp1, sizeof(bp1));
			ccstAssertTrue(r1.size() == sizeof(bp1));
			ccstAssertTrue(memcmp(r1.data(), bp1, std::min(r1.size(), sizeof(bp1))) == 0);
			
			r1.assign(some_ptr, sizeof(bp1));
			ccstAssertTrue(r1.size() == sizeof(bp1));
			ccstAssertTrue(memcmp(r1.data(), some_ptr, std::min(r1.size(), sizeof(bp1))) == 0);
		}
		
		void testSquareBracketOperator()
		{
			const std::string s1 = "ABCDEFGH";
			ByteRange r1(s1);
			for (size_t i = 0; i < r1.length(); i++) {
				ccstAssertEqual(s1[i], r1[i]);
				ccstAssertEqual(s1[i], r1.at(i));
			}
			
			try {
				if (r1[99] == 77) {
					ccstMessage("What a fortune. We have seventy-seven!");
				} else {
					ccstMessage("Bad luck, no double sevens returned :(");
				}
			} catch (std::exception & exc) {
				ccstFailure("operator[] should not raise exception.");
			}
			
			try {
				if (r1.at(99) == 0) {
					ccstFailure("at() must raise exception.");
				} else {
					ccstFailure("at() must raise exception.");
				}
			} catch (std::exception & exc) {
				ccstMessage("Correct: %s", exc.what());
			}
		}
		
		void testRelationalOperators()
		{
			ByteArray aAAA1 = { 'A', 'A', 'A' };
			ByteArray aAAA2 = { 'A', 'A', 'A' };
			ByteArray aBBB1 = { 'B', 'B', 'B' };
			
			ByteArray aBBC1  = { 'B', 'B', 'C' };
			ByteArray aBBBC1 = { 'B', 'B', 'B', 'C' };
			
			ByteArray aEMPTY1;
			ByteArray aEMPTY2;
			
			ByteRange AAA1(aAAA1);
			ByteRange AAA2(aAAA2);
			ByteRange BBB1(aBBB1);
			
			ByteRange BBC1(aBBC1);
			ByteRange BBBC1(aBBBC1);
			ByteRange EMPTY1(aEMPTY1);
			ByteRange EMPTY2(aEMPTY2);
			
			ccstAssertTrue(AAA1 == AAA1);
			ccstAssertTrue(AAA1 == AAA2);
			ccstAssertTrue(AAA1 <= AAA2);
			ccstAssertTrue(AAA1 <= BBB1);
			ccstAssertTrue(AAA1 <  BBB1);
			ccstAssertTrue(BBB1 >  AAA1);
			ccstAssertTrue(BBB1 >= AAA1);
			ccstAssertTrue(BBB1 != AAA1);
			
			ccstAssertTrue(BBC1 >  BBB1);
			ccstAssertTrue(BBC1 >= BBB1);
			ccstAssertTrue(BBC1 != BBB1);
			ccstAssertTrue(BBB1 <  BBC1);
			ccstAssertTrue(BBB1 <= BBC1);
			
			ccstAssertTrue(AAA1 >  EMPTY1);
			ccstAssertTrue(AAA1 >= EMPTY1);
			ccstAssertTrue(AAA1 != EMPTY1);
			
			ccstAssertTrue(EMPTY1 <  AAA1);
			ccstAssertTrue(EMPTY1 <= AAA1);
			ccstAssertTrue(EMPTY1 == EMPTY2);
			
			ccstAssertTrue(BBB1  <  BBBC1);
			ccstAssertTrue(BBB1  <= BBBC1);
			ccstAssertTrue(BBB1  != BBBC1);
			ccstAssertTrue(BBBC1 >  BBB1);
			ccstAssertTrue(BBBC1 >= BBB1);
			ccstAssertTrue(BBBC1 != BBB1);
		}
		
		void testIterators()
		{
			ByteArray a1 = { 1, 2, 3, 4, 5, 6, 7, 8 };
			ByteRange r1(a1);
			ByteArray a1c(r1.begin(), r1.end());
			ccstAssertEqual(a1c, ByteArray({ 1, 2, 3, 4, 5, 6, 7, 8 }));
			
			// TODO: right iterator / r-iterator is needed...
			
//			ByteRange r2(a1);
//			ByteArray a2c(r2.rend(), r2.rbegin());
//			ccstAssertEqual(a2c, ByteArray({ 8, 7, 6, 5, 4, 3, 2, 1 }));
		}
		
		void testCornerCases()
		{
			// null pointer, but size
			ByteRange r1((void*)nullptr, 64);
			ccstAssertTrue(r1.empty());
			ccstAssertEqual(r1.size(), 0);
			ccstAssertEqual(r1.begin(), r1.end());
			
			// null char pointer
			const char * null_string = nullptr;
			ByteRange r2(null_string);
			ccstAssertTrue(r2.empty());
			ccstAssertEqual(r2.size(), 0);
			ccstAssertEqual(r2.begin(), r1.end());
		}
		
		void testSubRanges()
		{
			{
				std::string s1("Hello world!");
				ByteRange r1(s1);
				
				ccstAssertEqual(to_string(r1.subRangeTo(4)), "Hell");
				ccstAssertEqual(to_string(r1.subRangeTo(4).subRangeFrom(2)), "ll");
				ccstAssertEqual(to_string(r1.subRangeTo(0)), "");
				ccstAssertEqual(to_string(r1.subRangeTo(s1.size())), s1);
				ccstAssertEqual(to_string(r1.subRangeFrom(6)), "world!");
				ccstAssertEqual(to_string(r1.subRangeFrom(s1.size())), "");
				
				ccstAssertEqual(to_string(r1.subRange(2, 5)), "llo w");
				ccstAssertEqual(to_string(r1.subRange(2, 5)), "llo w");
				ccstAssertEqual(to_string(r1.subRange(0, 2)), "He");
				ccstAssertEqual(to_string(r1.subRange(r1.size()-3, 3)), "ld!");
				
				r1.removePrefix(1);
				ccstAssertEqual(to_string(r1), "ello world!");
				r1.removeSuffix(1);
				ccstAssertEqual(to_string(r1), "ello world");
				
				r1.assign(s1);
				ccstAssertEqual(to_string(r1.subRange(0, s1.size())), "Hello world!");
				
				// Keep s1 alive...
				ccstAssertEqual(s1, "Hello world!");
			}
			{
				// test corner cases
				std::string s1("Hello world!");
				ByteRange r1(s1);

				try {
					r1.subRange(0, s1.size() + 1);
					ccstFailure("Previous line must raise exception");
				} catch (std::exception & exc) {
				}
				try {
					r1.subRange(1, s1.size());
					ccstFailure("Previous line must raise exception");
				} catch (std::exception & exc) {
				}
				try {
					r1.subRangeFrom(s1.size() + 1);
					ccstFailure("Previous line must raise exception");
				} catch (std::exception & exc) {
				}
				try {
					r1.subRangeTo(s1.size() + 1);
					ccstFailure("Previous line must raise exception");
				} catch (std::exception & exc) {
				}

				// Keep s1 alive...
				ccstAssertEqual(s1, "Hello world!");
			}
		}
		
		void testOtherMethods()
		{
			std::string s1("Hello world!");
			ByteRange r1(s1);
			ccstAssertEqual(to_string(r1), s1);
			ByteRange r2;
			ccstAssertEqual(to_string(r2), "");
		}
	};
	
	CC7_CREATE_UNIT_TEST(cc7ByteRangeTests, "cc7")
	
} // cc7::tests
} // cc7