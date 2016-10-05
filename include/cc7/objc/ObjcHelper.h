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
#import <Foundation/Foundation.h>

namespace cc7
{
namespace objc
{
	/**
	 Function creates a new NSData object with bytes copied from
	 given range of bytes.
	 */
	NSData * CopyToNSData(const cc7::ByteRange & range);
	
	/**
	 Function creates a new NSString object with UTF8 string
	 copied from given string.
	 */
	NSString * CopyToNSString(const std::string & string);
	
	/**
	 Function creates a new ByteArray object with bytes copied
	 from given NSData object
	 */
	ByteArray CopyFromNSData(NSData * data);
	
	/**
	 Function creates a new std::string with UTF8 string
	 converted from given NSString object.
	 */
	std::string CopyFromNSString(NSString * string);
	
	/**
	 Function creates a new ByteArray object with bytes copied
	 from given NSString. The UTF8 encoding is used for conversion.
	 The function is very similar to 'CopyFromNSString' but the
	 destination C++ object is byte array instead of std::string. 
	 It's recommended to use this function for conversion from 
	 strings which contains sensitive information, like passphrases.
	 */	
	ByteArray CopyFromNSStringToByteArray(NSString * string);
	
} // cc7::objc
} // cc7
