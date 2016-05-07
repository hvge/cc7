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
#include <cc7/ByteArray.h>

namespace cc7
{
namespace tests
{
	typedef std::vector<cc7::byte> bvec;
	
	class cc7ByteArrayTests : public UnitTest
	{
	public:
		cc7ByteArrayTests()
		{
			CC7_REGISTER_TEST_METHOD(testCreation)
			CC7_REGISTER_TEST_METHOD(testAppend)
			CC7_REGISTER_TEST_METHOD(testAssign)
			CC7_REGISTER_TEST_METHOD(testInsert)
			CC7_REGISTER_TEST_METHOD(testErase)
			CC7_REGISTER_TEST_METHOD(testSquareBracketOperator)
			CC7_REGISTER_TEST_METHOD(testRelationalOperators)
			CC7_REGISTER_TEST_METHOD(testOtherMethods)
		}
		
		// Helper methods
		
		ByteArray concatTwoArrays(const ByteArray & a1, const ByteArray & a2)
		{
			return ByteArray(a1).append(a2);
		}
		
		bvec randomData(size_t n)
		{
			bvec result;
			result.reserve(n);
			while (n > 0) {
				result.push_back(random() & 0xFF);
				--n;
			}
			return result;
		}
		
		// Unit tests
		
		void testCreation()
		{
			const bvec v123   = {1, 2, 3};
			const bvec v4zero = {0, 0, 0, 0};
			const bvec v5one  = {1, 1, 1, 1, 1};
			
			ByteArray a1;
			ccstAssertTrue(a1.empty());
			ccstAssertTrue(a1.size() == 0);
			ccstAssertTrue(a1.begin() == a1.end());

			ByteArray a2({1, 2, 3});
			ccstAssertTrue(v123.size() == a2.size());
			ccstAssertTrue(memcmp(a2.data(), v123.data(), a2.size()) == 0);
			
			ByteArray a3 = {1, 2, 3};
			ccstAssertTrue(v123.size() == a3.size());
			ccstAssertTrue(memcmp(v123.data(), a3.data(), std::min(a3.size(), v123.size())) == 0);

			ByteArray a4(v123.begin(), v123.end());
			ccstAssertTrue(v123.size() == a4.size());
			ccstAssertTrue(memcmp(v123.data(), a4.data(), std::min(a4.size(), v123.size())) == 0);

			ByteArray a5(4);
			ccstAssertTrue(v4zero.size() == a5.size());
			ccstAssertTrue(memcmp(v4zero.data(), a5.data(), std::min(a5.size(), v4zero.size())) == 0);

			ByteArray a6(5, 1);
			ccstAssertTrue(v5one.size() == a6.size());
			ccstAssertTrue(memcmp(v5one.data(), a6.data(), std::min(a6.size(), v5one.size())) == 0);
			
			ByteArray a7 = a1;
			ccstAssertTrue(a7.empty());
			ByteArray a8(a1);
			ccstAssertTrue(a8.empty());
			ByteArray a9 = a4;
			ccstAssertEqual(a9, a4);
			ByteArray a10(a5);
			ccstAssertEqual(a10, a5);
			
			// Tests for move or copy construction.
			
			ByteArray concat1 = concatTwoArrays({0}, {0, 0, 0});
			ccstAssertEqual(concat1, ByteArray({0,0,0,0}));
			ByteArray concat2(concatTwoArrays({1,2,3}, {8,8,8,8,8,8,8}));
			ccstAssertEqual(concat2, ByteArray({1,2,3,8,8,8,8,8,8,8}));

			// advanced test. The vector of arrays will reallocate its buffer
			// for several times and therefore must move or copy ByteArray objects.
			
			std::vector<ByteArray> vba({a1});
			size_t vba_capacity1 = vba.capacity();
			ccstAssertEqual(vba[0], a1);
			
			vba.insert(vba.end(), {
				a2, a3, a4, a5,
			});
			size_t vba_capacity2 = vba.capacity();
			
			ccstAssertEqual(vba[1], a2);
			ccstAssertEqual(vba[2], a3);
			ccstAssertEqual(vba[3], a4);
			ccstAssertEqual(vba[4], a5);
			
			vba.insert(vba.end(), {
				a6, a7, a8, a9, a10
			});
			size_t vba_capacity3 = vba.capacity();
			
			ccstAssertEqual(vba[0], a1);
			ccstAssertEqual(vba[1], a2);
			ccstAssertEqual(vba[2], a3);
			ccstAssertEqual(vba[3], a4);
			ccstAssertEqual(vba[4], a5);
			ccstAssertEqual(vba[5], a6);
			ccstAssertEqual(vba[6], a7);
			ccstAssertEqual(vba[7], a8);
			ccstAssertEqual(vba[8], a9);
			ccstAssertEqual(vba[9], a10);

			vba.push_back(concat1);
			vba.push_back(concat2);
			size_t vba_capacity4 = vba.capacity();
			
			ccstAssertEqual(vba[10], concat1);
			ccstAssertEqual(vba[11], concat2);
			ccstAssertNotEqual(vba[10].data(), concat1.data());
			ccstAssertNotEqual(vba[11].data(), concat2.data());
			
			// Check capacities. This is just a warning if vector
			// did not reallocate the buffer. If following assertions
			// fails then you should change test to achieve the reallocation.
			ccstAssertTrue(vba_capacity1 < vba_capacity2 || vba_capacity2 < vba_capacity3 || vba_capacity1 < vba_capacity3);
			ccstAssertTrue(vba_capacity2 < vba_capacity4 || vba_capacity3 < vba_capacity4);
		}

		void testAssign()
		{
			const bvec v123   = {1, 2, 3};
			const bvec v4zero = {0, 0, 0, 0};
			const bvec v5one  = {1, 1, 1, 1, 1};
			
			ByteArray a1;
			a1.assign(ByteArray());
			ccstAssertTrue(a1.empty());
			ccstAssertTrue(a1.size() == 0);
			ccstAssertTrue(a1.begin() == a1.end());
			
			ByteArray a2;
			a2.assign({1, 2, 3});
			ccstAssertTrue(v123.size() == a2.size());
			ccstAssertTrue(memcmp(a2.data(), v123.data(), a2.size()) == 0);
			
			ByteArray a4;
			a4.assign(v123.begin(), v123.end());
			ccstAssertTrue(v123.size() == a4.size());
			ccstAssertTrue(memcmp(v123.data(), a4.data(), std::min(a4.size(), v123.size())) == 0);
			
			ByteArray a6;
			a6.assign(5, 1);
			ccstAssertTrue(v5one.size() == a6.size());
			ccstAssertTrue(memcmp(v5one.data(), a6.data(), std::min(a6.size(), v5one.size())) == 0);
		}

		
		void testAppend()
		{
			{
				// += [ begin, end ]
				bvec		bv;
				ByteArray	ba;
				for (size_t i = 0; i < 16; i++) {
					bvec rdata = randomData(random() & 0x1f + 1);
					bv.insert(bv.end(), rdata.begin(), rdata.end());
					ba.append(rdata.begin(), rdata.end());
				}
				ccstAssertEqual(bv.size(), ba.size());
				ccstAssertTrue(memcmp(bv.data(), ba.data(), std::min(bv.size(), ba.size())) == 0);
			}
			{
				// += [ ptr, size ]
				bvec		bv;
				ByteArray	ba;				
				for (size_t i = 0; i < 16; i++) {
					bvec rdata = randomData(random() & 0x1f + 1);
					bv.insert(bv.end(), rdata.begin(), rdata.end());
					ba.append(rdata.data(), rdata.size());
				}
				ccstAssertEqual(bv.size(), ba.size());
				ccstAssertTrue(memcmp(bv.data(), ba.data(), std::min(bv.size(), ba.size())) == 0);
			}
			{
				// += ByteArray
				bvec		bv;
				ByteArray	ba;
				for (size_t i = 0; i < 16; i++) {
					bvec rdata = randomData(random() & 0x1f + 1);
					ByteArray radata(rdata.begin(), rdata.end());
					bv.insert(bv.end(), rdata.begin(), rdata.end());
					ba.append(radata);
				}
				ccstAssertEqual(bv.size(), ba.size());
				ccstAssertTrue(memcmp(bv.data(), ba.data(), std::min(bv.size(), ba.size())) == 0);
			}
			{
				// += ByteRange
				bvec		bv;
				ByteArray	ba;
				for (size_t i = 0; i < 16; i++) {
					bvec rdata = randomData(random() & 0x1f + 1);
					ByteRange range(rdata.data(), rdata.size());
					bv.insert(bv.end(), rdata.begin(), rdata.end());
					ba.append(range);
				}
				ccstAssertEqual(bv.size(), ba.size());
				ccstAssertTrue(memcmp(bv.data(), ba.data(), std::min(bv.size(), ba.size())) == 0);
			}
			{
				// += fill [ n x 0 ]  or  [ n x value ]
				bvec		bv;
				ByteArray	ba;
				for (size_t i = 0; i < 16; i++) {
					size_t fill_count    = random() & 0x1F;
					cc7::byte fill_value = random() & 0xff;
					bv.insert(bv.end(), fill_count);
					ba.append(fill_count);
					fill_count = random() & 0x1F;
					bv.insert(bv.end(), fill_count, fill_value);
					ba.append(fill_count, fill_value);
				}
				ccstAssertEqual(bv.size(), ba.size());
				ccstAssertTrue(memcmp(bv.data(), ba.data(), std::min(bv.size(), ba.size())) == 0);
			}
		}
		
		void testInsert()
		{
			{
				ByteArray a1;
				a1.insert(a1.begin(), 0xFF);
				ccstAssertEqual(a1[0], 0xFF);
			}
			{
				ByteArray a1;
				a1.insert(a1.end(), 0xFF);
				ccstAssertEqual(a1[0], 0xFF);
			}
			{
				ByteArray a1;
				a1.insert(a1.begin(), 5, 0xCC);
				ccstAssertEqual(a1, ByteArray(5, 0xCC));
			}
			{
				ByteArray a1 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
				a1.insert(a1.begin(), 0x55);
				ccstAssertEqual(a1, ByteArray({0x55, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
			}
			{
				ByteArray a1 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
				a1.insert(a1.end(), 0xEE);
				ccstAssertEqual(a1, ByteArray({1, 2, 3, 4, 5, 6, 7, 8, 9, 0xEE}));
			}
			{
				ByteArray a1 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
				a1.insert(a1.begin() + 4, 0xCC);
				ccstAssertEqual(a1, ByteArray({1, 2, 3, 4, 0xCC, 5, 6, 7, 8, 9,}));
			}
			{
				ByteArray a1 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
				a1.insert(a1.begin() + 5, 4, 0xDD);
				ccstAssertEqual(a1, ByteArray({1, 2, 3, 4, 5, 0xDD, 0xDD, 0xDD, 0xDD, 6, 7, 8, 9,}));
			}
		}
		
		void testErase()
		{
			{
				// erase at specific position
				ByteArray a1 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
				a1.erase(a1.begin());
				ccstAssertEqual(a1, ByteArray({2, 3, 4, 5, 6, 7, 8, 9}));
				a1.erase(a1.end() - 1);
				ccstAssertEqual(a1, ByteArray({2, 3, 4, 5, 6, 7, 8}));
				a1.erase(a1.begin() + 1);
				ccstAssertEqual(a1, ByteArray({2, 4, 5, 6, 7, 8}));
			}
			{
				// erase range
				ByteArray a1 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
				a1.erase(a1.begin(), a1.begin() + 2);
				ccstAssertEqual(a1, ByteArray({3, 4, 5, 6, 7, 8, 9}));
				a1.erase(a1.end() - 2, a1.end());
				ccstAssertEqual(a1, ByteArray({3, 4, 5, 6, 7}));
				a1.erase(a1.begin() + 1, a1.end() - 1);
				ccstAssertEqual(a1, ByteArray({3, 7}));
				a1.erase(a1.begin(), a1.end());
				ccstAssertTrue(a1.empty());
			}
		}

		void testSquareBracketOperator()
		{
			ByteArray a1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xff };
			ccstAssertEqual(a1[0], 1);
			ccstAssertEqual(a1[1], 2);
			ccstAssertEqual(a1[2], 3);
			ccstAssertEqual(a1[3], 4);
			a1[4] = 33;
			ccstAssertEqual(a1[4], 33);
			ccstAssertEqual(a1[5], 6);
			ccstAssertEqual(a1[6], 7);
			ccstAssertEqual(a1[7], 8);
			ccstAssertEqual(a1[8], 9);
			ccstAssertEqual(a1[9], 0xff);
		}
		
		void testRelationalOperators()
		{
			ByteArray AAA1 = { 'A', 'A', 'A' };
			ByteArray AAA2 = { 'A', 'A', 'A' };
			ByteArray BBB1 = { 'B', 'B', 'B' };
			ByteArray CCC1 = { 'C', 'C', 'C' };
			
			ByteArray BBC1  = { 'B', 'B', 'C' };
			ByteArray BBBC1 = { 'B', 'B', 'B', 'C' };
			ByteArray EMPTY1;
			ByteArray EMPTY2;
			
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
		
		void testOtherMethods()
		{
			{
				ByteArray a1({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
				a1.reserve(128);
				ccstAssertTrue(a1.capacity() >= 128);
				size_t capacity1 = a1.capacity();
				a1.clear();
				ccstAssertTrue(a1.empty());
				ccstAssertEqual(capacity1, a1.capacity());
			}
			{
				ByteArray a1({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
				a1.reserve(128);
				ccstAssertTrue(a1.capacity() >= 128);
				size_t capacity1 = a1.capacity();
				a1.secureClear();
				ccstAssertTrue(a1.empty());
				ccstAssertEqual(capacity1, a1.capacity());
			}
		}
		
	};
	
	CC7_CREATE_UNIT_TEST(cc7ByteArrayTests, "cc7")
	
} // cc7::tests
} // cc7