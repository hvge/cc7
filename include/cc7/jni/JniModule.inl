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

#if !defined(CC7_ANDROID)
// The file must be included after the <cc7/Platform.h>
#error "This file is for Android platform only"
#endif

/**
 Inline header, you have to setup following macros before you include this file:

 #define CC7_JNI_CLASS_PATH		"org/example/package/path"	// Class path for code which you're going to implement
 #define CC7_JNI_CLASS_PACKAGE	org_example_package_path	// Class path prefix for functions which implements JNI methods. Without "Java" prefix.
 #define CC7_JNI_JAVA_CLASS		YourJavaClass				// Name of Java class
 #define CC7_JNI_CPP_CLASS		YourNativeClass				// Name of CPP class
 
 Optional defines:
 
 #define CC7_JNI_POINTER_FIELD	"cppPointer"				// Name of Java class field, where the C++ instance is stored (default value is "handle")
															// You have to add a private field with the same name into your Java class.

 Warning: It is not recommended to put implementation of multiple JNI class wrappers, into a single CPP file.
 
 Note for developers:

 I know that this is a very tricky and unclear method how to implement JNI wrappers, but I think that this method leads to more
 readable code than if you write everything manually. You don't need to use these helper macros at all. The decision is up to you :)
 
 Check README.jni.txt located in "src/cc7/platform/android" folder, for details about this technique. 
*/

#if !defined(CC7_JNI_CLASS_PATH) || !defined(CC7_JNI_CLASS_PACKAGE) || !defined(CC7_JNI_CPP_CLASS) || !defined(CC7_JNI_JAVA_CLASS)
#error "This is an inline header. You have to define CC7_JNI_CLASS_PATH & CC7_JNI_CLASS_PACKAGE & CC7_JNI_CPP_CLASS & CC7_JNI_JAVA_CLASS macros before include."
#endif

#define __CC7_JNI_XSTRINGIFY(a)			# a
#define __CC7_JNI_STRINGIFY(a)			__CC7_JNI_XSTRINGIFY(a)

#define __CC7_JNI_CONCAT3(a,b,c)		a ## b ## c
#define __CC7_JNI_CONCAT4(a,b,c,d)		a ## b ## c ## d
#define __CC7_JNI_CONCAT5(a,b,c,d,e)	a ## b ## c ## d ## e
#define __CC7_JNI_CONCAT6(a,b,c,d,e,f)	a ## b ## c ## d ## e ## f

#define __CC7_JNI_CONCAT_PATH(package, class, method)	__CC7_JNI_CONCAT6(Java_,package,_,class,_,method)

// Defaul value for CC7_JNI_POINTER_FIELD
#if !defined(CC7_JNI_POINTER_FIELD)
	#define CC7_JNI_POINTER_FIELD "handle"
#endif

/**
 The result of this macro is full class path for given object name. 
 For example, CC7_JNI_MODULE_CLASS_PATH("YourJavaClass") creates "org/example/package/path/YourJavaClass" string.
 */
#define CC7_JNI_MODULE_CLASS_PATH(object_name)			CC7_JNI_CLASS_PATH "/" object_name

/**
 Returns jclass object for given object name. The object name is
 */
#define CC7_JNI_MODULE_CLASS_FOR_OBJECT(object_name)	env->FindClass(CC7_JNI_MODULE_CLASS_PATH(object_name))

/**
 CC7_JNI_CLASS_BEGIN wraps subsequent code into the extern "C" block and creates a
 static method which returns jfieldID for handle in the native class.
 */
#define CC7_JNI_MODULE_CLASS_BEGIN()																\
																									\
	extern "C" {																					\
																									\
	static jfieldID GetHandleFieldID(JNIEnv* env)													\
	{																								\
		static jfieldID s_HandleFieldID = 0;														\
		if (s_HandleFieldID == 0) {																	\
			jclass clazz = CC7_JNI_MODULE_CLASS_FOR_OBJECT(__CC7_JNI_STRINGIFY(CC7_JNI_JAVA_CLASS));\
			s_HandleFieldID = CC7_JNI_FIELD_LONG(clazz, CC7_JNI_POINTER_FIELD);						\
		}																							\
		return s_HandleFieldID;																		\
	}

/**
 CC7_JNI_MODULE_CLASS_END closes previously opened extern "C" block.
 */
#define CC7_JNI_MODULE_CLASS_END()																			\
	} // extern "C"

/**
 Returns instance of CC7_JNI_CPP_CLASS retrieved from "thiz" object. You can typically use this macro at the beginning
 of JNI method to get a pointer to stored CC7_JNI_CPP_CLASS object. For example:
 
 YourNativeClass * obj = CC7_THIS_OBJ();
 if (!obj) {
	return;	// internal error, object was not initialized
 }
 */
#define CC7_THIS_OBJ()	reinterpret_cast<CC7_JNI_CPP_CLASS*>(env->GetLongField(thiz, GetHandleFieldID(env)))

/**
 Declares a prototype for method which has no parameters. You can also use this macro in the actual method
 implementation. For example:
 
 CC7_JNI_METHOD(jboolean, hasThisMightyFeature)
 {
	YourJavaClass * obj = CC7_THIS_OBJ();
	if (!obj) {
		return (jboolean)false;
	}
	return obj->hasThisMightyFeature();
 }
 
 CC7_JNI_METHOD(void, doVeryImportantStuff)
 {
	YourJavaClass * obj = CC7_THIS_OBJ();
	if (!obj) {
		return;
	}
	obj->doVeryImportantStuff();
 }
 */
#define CC7_JNI_METHOD(return_type, name)															\
	JNIEXPORT return_type JNICALL __CC7_JNI_CONCAT_PATH(CC7_JNI_CLASS_PACKAGE,CC7_JNI_JAVA_CLASS,name) (JNIEnv* env, jobject thiz)

/**
 Declares a prototype for method with parameters. You can also use this macro in the actual method
 implementation. For example:
 
 CC7_JNI_METHOD_PARAMS(jint, calculateVeryImportantValue, jint input)
 {
	YourJavaClass * obj = CC7_THIS_OBJ();
	if (!obj) {
		return -1;
	}
	return obj->calculateVeryImportantValue(input);
 }
 
 CC7_JNI_METHOD_PARAMS(void, setThatMagnificentFlag, jboolean value)
 {
	YourJavaClass * obj = CC7_THIS_OBJ();
	if (!obj) {
		return;
	}
	obj->setThatMagnificentFlag(value);
 }
 */
#define CC7_JNI_METHOD_PARAMS(return_type, name, paramz...)											\
	JNIEXPORT return_type JNICALL __CC7_JNI_CONCAT_PATH(CC7_JNI_CLASS_PACKAGE,CC7_JNI_JAVA_CLASS,name)(JNIEnv* env, jobject thiz, paramz)



