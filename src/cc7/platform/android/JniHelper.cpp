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

#include <cc7/jni/JniHelper.h>

#if !defined(CC7_ANDROID)
#error "This file is for Android platform only"
#endif

namespace cc7
{
namespace jni
{
	// Strings
	
	jstring CopyToJavaString(JNIEnv * env, const std::string & str)
	{
		return env->NewStringUTF(str.c_str());
	}
	
	
	jstring CopyToJavaString(JNIEnv * env, const char * str)
	{
		if (!str) {
			str = "";
		}
		return env->NewStringUTF(str);
	}
	
	std::string CopyFromJavaString(JNIEnv * env, jstring str)
	{
		std::string result;
		if (str && env) {
			const char * str_ptr = env->GetStringUTFChars(str, 0);
			if (str_ptr) {
				result.assign(str_ptr);
				env->ReleaseStringUTFChars(str, str_ptr);
			}
		}
		return result;
	}

	
	// Byte array
	
	jbyteArray CopyToJavaByteArray(JNIEnv * env, const cc7::ByteRange & range)
	{
		jbyteArray array = env->NewByteArray(range.size());
		env->SetByteArrayRegion (array, 0, range.size(), (const jbyte*)range.data());
		return array;
	}
	
	
	cc7::ByteArray CopyFromJavaByteArray(JNIEnv * env, jbyteArray array)
	{
		cc7::ByteArray result;
		if (env && array) {
			jsize length = env->GetArrayLength(array);
			if (length > 0) {
				jbyte * bytes = env->GetByteArrayElements(array, 0);
				if (CC7_CHECK(bytes != nullptr, "JNI: byteArray has no bytes but size greater than 0.")) {
					result.assign(bytes, bytes + length);
					env->ReleaseByteArrayElements(array, bytes, 0);
				}
			}
		}
		return result;
	}
	
	
	// Objects
	
	jobject CreateJavaObject(JNIEnv * env, const char * clazz, const char * ctr_signature, ...)
	{
		jclass result_class = env->FindClass(clazz);
		if (result_class == nullptr) {
			CC7_ASSERT(false, "JNI: Class: %s: Unknown class signature.", clazz);
			return nullptr;
		}
		
		jmethodID mid_constructor = env->GetMethodID(result_class, "<init>", ctr_signature);
		if (mid_constructor == nullptr) {
			CC7_ASSERT(false, "JNI: Class: %s: Unknown object constructor with signature '%s'", clazz, ctr_signature);
			return NULL;
		}
		
		va_list args;
		va_start(args, ctr_signature);
		CC7_LOG("JNI: Class: '%s': Creating a new object with arguments", clazz);
		jobject result_object = env->NewObjectV(result_class, mid_constructor, args);
		CC7_ASSERT(result_object != nullptr, "JNI: Class: %s: Object creation failed. Signature: '%s'", clazz, ctr_signature);
		va_end(args);
		return result_object;
	}
}
}