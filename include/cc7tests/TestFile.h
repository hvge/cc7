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
	class TestResource;
	class JSONValue;
	
	class TestFile
	{
	public:
		TestFile(const TestFile & f) = default;
		TestFile(TestFile && f) = default;
		TestFile & operator=(const TestFile & t) = default;
		TestFile & operator=(TestFile && t) = default;
		
		bool isValid() const;
		
		// File size
		size_t size() const;
		// Current reading offset
		size_t offset() const;
		// Remaining bytes for reading
		size_t remaining() const;
		
		// Seeks to pos and returns new offset
		size_t seekTo(size_t pos);
		// Skips size bytes and returns how many bytes were skipped
		size_t skipSize(size_t size);
		
		// Returns byte range for required size. If the size is less than
		// remaining bytes then returns as much bytes as possible
		cc7::ByteRange	readMemory(size_t size);
		
		// Reads one byte from file. If there's no byte left, then
		// returns TestFile::EndOfFile
		cc7::U16		readByte();
		
		// Returns one line from file. If there's no bytes left, then
		// returns empty string. You should check remaining bytes.
		std::string		readLine();
		
		// Reads 
		void			readJsonValue(JSONValue & value);
		
		static const cc7::U16 EndOfFile = 0xFFFF;
		
		
	private:
		friend class TestDirectory;
		
		// Private constructor with a resource
		TestFile(const TestResource * resource);
		
		const TestResource *	_resource;
		size_t					_offset;
		
	};
	
} // cc7::tests
} // cc7