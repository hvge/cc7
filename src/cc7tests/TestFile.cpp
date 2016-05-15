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

#include <cc7tests/TestFile.h>
#include <cc7tests/TestResource.h>


namespace cc7
{
namespace tests
{
	TestFile::TestFile(const TestResource * r) :
		_resource(r),
		_offset(0)
	{
	}
	
	bool TestFile::isValid() const
	{
		return _resource != nullptr;
		
	}
	
	size_t TestFile::size() const
	{
		if (isValid()) {
			return _resource->size();
		}
		return 0;
	}
	
	size_t TestFile::offset() const
	{
		if (isValid()) {
			return _offset;
		}
		return 0;
	}
	
	size_t TestFile::remaining() const
	{
		if (isValid()) {
			return size() - _offset;
		}
		return 0;
	}
	
	//
	
	size_t TestFile::seekTo(size_t seek)
	{
		if (!isValid()) {
			return 0;
		}
		_offset = std::min(seek, size());
		return _offset;
	}
	
	cc7::ByteRange TestFile::readMemory(size_t size)
	{
		size = std::min(size, remaining());
		ByteRange range = _resource->range().subRange(_offset, size);
		_offset += size;
		return range;
	}
	
	cc7::U16 TestFile::readByte()
	{
		if (remaining() > 0) {
			return _resource->data()[_offset++];
		}
		return TestFile::EndOfFile;
	}
	
	std::string TestFile::readLine()
	{
		std::string result;
		result.reserve(16);
		cc7::byte bytes[2] = {0, 0};
		while (remaining() > 0) {
			bytes[0] = _resource->data()[_offset++];
			if (bytes[0] == '\n') {
				break;
			} else {
				result.append((const char*)bytes, 1);
			}
		}
		return result;
	}
	
} // cc7::tests
} // cc7
