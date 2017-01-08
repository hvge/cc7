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

#import <cc7/objc/ObjcHelper.h>

namespace cc7
{
namespace objc
{
	NSData * CopyToNSData(const cc7::ByteRange & range)
	{
		return [[NSData alloc] initWithBytes:range.data() length:range.size()];
	}
	
	NSString * CopyToNSString(const std::string & string)
	{
		return [[NSString alloc] initWithUTF8String:string.c_str()];
	}
	
	NSData * CopyToNullableNSData(const cc7::ByteRange & range)
	{
		if (!range.empty()) {
			return [[NSData alloc] initWithBytes:range.data() length:range.size()];
		}
		return nil;
	}
	
	NSString * CopyToNullableNSString(const std::string & string)
	{
		if (!string.empty()) {
			return [[NSString alloc] initWithUTF8String:string.c_str()];
		}
		return nil;
	}
	
	ByteArray CopyFromNSData(NSData * data)
	{
		ByteArray result;
		if (data) {
			result.assign(ByteRange(data.bytes, data.length));
		}
		return result;
	}
	
	std::string CopyFromNSString(NSString * string)
	{
		std::string result;
		if (string) {
			result.assign(string.UTF8String);
		}
		return result;
	}
	
	ByteArray CopyFromNSStringToByteArray(NSString * string)
	{
		ByteArray result;
		if (string) {
			result.assign(cc7::MakeRange(string.UTF8String));
		}
		return result;
	}
	
} // cc7::objc
} // cc7
