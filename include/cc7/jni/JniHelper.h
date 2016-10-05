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
#include <cc7/jni/JniHelperMacros.h>
#include <jni.h>

namespace cc7
{
namespace jni
{
	/**
	 Function creates a new instance of jstring object with characters copied 
	 from given string object. The UTF8 encoding is expected on the input.
	 */
	jstring CopyToJavaString(JNIEnv * env, const std::string & str);
	
	/**
	 Function creates a new instance of jstring object with characters copied 
	 from given char pointer. The UTF8 encoding is expected on the input.
	 If the provided char pointer is NULL, then the empty, but valid java string 
	 object is returned.
	 */
	jstring CopyToJavaString(JNIEnv * env, const char * str);
	
	/**
	 Function creates a new instance of std::string with characters copied
	 from given java string. The UTF8 encoding is used for the conversion.
	 */
	std::string CopyFromJavaString(JNIEnv * env, jstring string);
	
	/**
	 Function creates a new instance of cc7::ByteArray, initialized with characters
	 from given java string. The UTF8 encoding is used for the conversion.
	 The function is very similar to 'CopyFromJavaString' but the destination C++
	 object is byte array instead of std::string. It's recommended to use this function 
	 for conversion from strings which contains sensitive information, like passphrases.
	 */
	cc7::ByteArray CopyFromJavaStringToByteArray(JNIEnv * env, jstring string);
	
	/**
	 Function creates a new instance of java byte array object with bytes copied from
	 given range of bytes.
	 */
	jbyteArray CopyToJavaByteArray(JNIEnv * env, const cc7::ByteRange & range);
	
	/**
	 Function creates a new instance of cc7::ByteArray object with bytes copied
	 from given java byte array object.
	 */
	cc7::ByteArray CopyFromJavaByteArray(JNIEnv * env, jbyteArray array);
	
	/**
	 Function creates a new instance of arbitrary java object.
	 */
	jobject CreateJavaObject(JNIEnv * env, const char * clazz, const char * ctr_signature, ...);
	
} // cc7::jni
} // cc7
