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

#if !defined(CC7_ANDROID)
// The file must be included after the <cc7/Platform.h>
#error "This file is for Android platform only"
#endif

/*
 The following CC7_JNI_FIELD_* macros returns jfieldID for named field of desired type.
 For example, CC7_JNI_FIELD_BOOL returns jfieldID for given clazz and named field of that class.
 
 The JNIEnv * env variable has to be available in the current scope.
 */
#define CC7_JNI_FIELD_BOOL(clazz, fieldName)				env->GetFieldID(clazz, fieldName, "Z")
#define CC7_JNI_FIELD_BYTE(clazz, fieldName)				env->GetFieldID(clazz, fieldName, "B")
#define CC7_JNI_FIELD_CHAR(clazz, fieldName)				env->GetFieldID(clazz, fieldName, "C")
#define CC7_JNI_FIELD_SHORT(clazz, fieldName)				env->GetFieldID(clazz, fieldName, "S")
#define CC7_JNI_FIELD_INT(clazz, fieldName)					env->GetFieldID(clazz, fieldName, "I")
#define CC7_JNI_FIELD_LONG(clazz, fieldName)				env->GetFieldID(clazz, fieldName, "J")
#define CC7_JNI_FIELD_FLOAT(clazz, fieldName)				env->GetFieldID(clazz, fieldName, "F")
#define CC7_JNI_FIELD_DOUBLE(clazz, fieldName)				env->GetFieldID(clazz, fieldName, "D")
#define CC7_JNI_FIELD_STRING(clazz, fieldName)				env->GetFieldID(clazz, fieldName, "Ljava/lang/String;")
#define CC7_JNI_FIELD_BYTEARRAY(clazz, fieldName)			env->GetFieldID(clazz, fieldName, "[B")
#define CC7_JNI_FIELD_OBJECT(clazz, fieldName, signature)	env->GetFieldID(clazz, fieldName, signature)

/*
 Field setters. You can simply set typed object to the instance of java object.
 Note that this macro internally performs jfieldID enumeration and that might cause some performance hit if you use it for multiple times
 or in the loop. If this is your case then it's better to acquire identifier before the loop and use env->SetXXXField() methods directly.
 
 The JNIEnv * env variable has to be available in the current scope.
 */
#define CC7_JNI_SET_FIELD_BOOL(object, clazz, field_name, value)				env->SetBooleanField(object, CC7_JNI_FIELD_BOOL(clazz, field_name), value)
#define CC7_JNI_SET_FIELD_BYTE(object, clazz, field_name, value)				env->SetByteField(object, CC7_JNI_FIELD_BYTE(clazz, field_name), value)
#define CC7_JNI_SET_FIELD_CHAR(object, clazz, field_name, value)				env->SetCharField(object, CC7_JNI_FIELD_CHAR(clazz, field_name), value)
#define CC7_JNI_SET_FIELD_SHORT(object, clazz, field_name, value)				env->SetShortField(object, CC7_JNI_FIELD_SHORT(clazz, field_name), value)
#define CC7_JNI_SET_FIELD_INT(object, clazz, field_name, value)					env->SetIntField(object, CC7_JNI_FIELD_INT(clazz, field_name), value)
#define CC7_JNI_SET_FIELD_LONG(object, clazz, field_name, value)				env->SetLongField(object, CC7_JNI_FIELD_LONG(clazz, field_name), value)
#define CC7_JNI_SET_FIELD_FLOAT(object, clazz, field_name, value)				env->SetFloatField(object, CC7_JNI_FIELD_FLOAT(clazz, field_name), value)
#define CC7_JNI_SET_FIELD_DOUBLE(object, clazz, field_name, value)				env->SetDoubleField(object, CC7_JNI_FIELD_DOUBLE(clazz, field_name), value)

#define CC7_JNI_SET_FIELD_OBJECT(object, clazz, field_name, signature, value)	env->SetObjectField(object, CC7_JNI_FIELD_OBJECT(clazz, field_name, signatgure), value)				
#define CC7_JNI_SET_FIELD_STRING(object, clazz, field_name, value)				env->SetObjectField(object, CC7_JNI_FIELD_STRING(clazz, field_name), value)
#define CC7_JNI_SET_FIELD_BYTEARRAY(object, clazz, field_name, value)			env->SetObjectField(object, CC7_JNI_FIELD_BYTEARRAY(clazz, field_name), value)

/*
 Field getters. You can simply get typed object from the instance of java object.
 Note that this macro internally performs jfieldID enumeration and that might cause some performance hit if you use it for multiple times
 or in the loop. If this is your case then it's better to acquire identifier before the loop and use env->GetXXXField() methods directly.
 
 The JNIEnv * env variable has to be available in the current scope.
 */
#define CC7_JNI_GET_FIELD_BOOL(object, clazz, field_name)				env->GetBooleanField(object, CC7_JNI_FIELD_BOOL(clazz, field_name))
#define CC7_JNI_GET_FIELD_BYTE(object, clazz, field_name)				env->GetByteField(object, CC7_JNI_FIELD_BYTE(clazz, field_name))
#define CC7_JNI_GET_FIELD_CHAR(object, clazz, field_name)				env->GetCharField(object, CC7_JNI_FIELD_CHAR(clazz, field_name))
#define CC7_JNI_GET_FIELD_SHORT(object, clazz, field_name)				env->GetShortField(object, CC7_JNI_FIELD_SHORT(clazz, field_name))
#define CC7_JNI_GET_FIELD_INT(object, clazz, field_name)				env->GetIntField(object, CC7_JNI_FIELD_INT(clazz, field_name))
#define CC7_JNI_GET_FIELD_LONG(object, clazz, field_name)				env->GetLongField(object, CC7_JNI_FIELD_LONG(clazz, field_name))
#define CC7_JNI_GET_FIELD_FLOAT(object, clazz, field_name)				env->GetFloatField(object, CC7_JNI_FIELD_FLOAT(clazz, field_name))
#define CC7_JNI_GET_FIELD_DOUBLE(object, clazz, field_name)				env->GetDoubleField(object, CC7_JNI_FIELD_DOUBLE(clazz, field_name))

#define CC7_JNI_GET_FIELD_OBJECT(object, clazz, field_name, signature)	env->GetObjectField(object, CC7_JNI_FIELD_OBJECT(clazz, field_name, signature))
#define CC7_JNI_GET_FIELD_STRING(object, clazz, field_name)				(jstring)env->GetObjectField(object, CC7_JNI_FIELD_STRING(clazz, field_name))
#define CC7_JNI_GET_FIELD_BYTEARRAY(object, clazz, field_name)			(jbyteArray)env->GetObjectField(object, CC7_JNI_FIELD_BYTEARRAY(clazz, field_name))
