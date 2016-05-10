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

#pragma once

#include <cc7/ByteArray.h>

namespace cc7
{
namespace tests
{
	/**
	 Generates a required amount of random bytes.
	 This function uses OpenSSL as a PRNG generator.
	 */
	inline cc7::ByteArray getTestRandomData(size_t size)
	{
		cc7::ByteArray result;
		result.reserve(size);
		while (size > 0) {
			result.push_back(random() & 0xFF);
			--size;
		}
		return result;
	}
	
	/**
	 The TestByteVector is useful as an reference data
	 storage
	 */
	typedef std::vector<cc7::byte> TestByteVector;
	
	/**
	 Similar to getRandomData(), but doesn't use OpenSSL
	 as PRNG.
	 */
	inline TestByteVector getTestRandomDataVector(size_t n)
	{
		TestByteVector result;
		result.reserve(n);
		while (n > 0) {
			result.push_back(random() & 0xFF);
			--n;
		}
		return result;
	}

	
} // cc7::tests
} // cc7
