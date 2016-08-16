Implementing JNI interfaces
-----------------------------


This brief document describes how to use "JniModule.inl" header for implementing
JNI wrappers. Note that this technique is complete optional and you can use your
own ways for JNI code implementation. CC7 for Android doesn't force this.


-----------------------------
Java code Java code Java code
-----------------------------

	package org.example.package

	public class YourJavaClass
	{
		static {
			System.loadLibrary("YourDynamicLibraryName");
		}

		// Field, where the instance pointer will be stored
		private long cppPointer;

		// Public construction
		public MyJavaClass()
		{
			this.cppPointer = init();
		}

		// Must be called to destroy an underlying C++ object
		public void destroy() {
			if (this.cppPointer != 0) {
				destroy(this.cppPointer);
				this.cppPointer = 0;
			}
		}

		// Some methods implemented in native code

		public native float calculateMagicValue(float value);
		public native float getLastMagicValue();
		public native string getVersionName();

		// Private init & destroy
		private native long init();
		private native void destroy(long handle);
	}


--------------------------
C++ code C++ code C++ code
--------------------------

	#include <string>

	class YourNativeClass
	{
		float _last_value;

	public:

		YourNativeClass() : _last_value(0.0F) {}
		float getLastValue() const { return _last_value; }
		//
		float doMagic(float v)
		{
			long i;
			float x2, y;
			const float threehalfs = 1.5F;

			x2 = number * 0.5F;
			y  = number;
			i  = * ( long * ) &y;
			i  = 0x5f3759df - ( i >> 1 );
			y  = * ( float * ) &i;
			y  = y * ( threehalfs - ( x2 * y * y ) );
			// y  = y * ( threehalfs - ( x2 * y * y ) );

			_last_value = y;
			return y;
		}

		std::string getVersion() const
		{
			return "Q3A";
		}
	};

--------------------------
JNI code JNI code JNI code
--------------------------

	// Next header includes our C++ class
	#include <MyProject/YourNativeClass.h>

	// JniHelper.h brings severa useful functions
	#include <cc7/jni/JniHelper.h>

	// Next section defines the package: org.example.package and the names
	// of all involved classes.

	#define CC7_JNI_CLASS_PATH		"org/example/package"
	#define CC7_JNI_CLASS_PACKAGE	org_example_package
	#define CC7_JNI_JAVA_CLASS		YourJavaClass
	#define CC7_JNI_CPP_CLASS		YourNativeClass

	// optional, you can use default value named "handle", but for example
	// we're going to use custom field name
	#define CC7_JNI_POINTER_FIELD	"cppPointer"

	// Now it's time for module's inline header
	#include <cc7/jni/JniModule.inl>

	// ------------------------------------
	CC7_JNI_MODULE_CLASS_BEGIN()
	// ------------------------------------

	// Java: private native long init();
	CC7_JNI_METHOD(jlong, init)
	{
		auto handle = new YourNativeClass();
		CC7_ASSERT(handle != nullptr, "Unable to create C++ object.");
		return (jlong)handle;
	}

	// Java: private native void destroy(long handle);
	CC7_JNI_METHOD_PARAM(void, destroy, jlong handle)
	{
		auto obj = CC7_THIS_OBJ();
		if (!obj || (jlong)obj != handle) {
			CC7_ASSERT(false, "Internal object is already destroyed, or provided handle is not ours.");
			return;
		}
		delete module;
	}

	// Java: public native float calculateMagicValue(float value);
	CC7_JNI_METHOD_PARAM(jfloat, calculateMagicValue, jfloat value)
	{
	auto obj = CC7_THIS_OBJ();
		if (!obj) {
			return NAN;
		}
		return obj->doMagic(value);
	}

	// Java: public native float getLastMagicValue();
	CC7_JNI_METHOD(jfloat, getLastMagicValue)
	{
		auto obj = CC7_THIS_OBJ();
		if (!obj) {
			return NAN;
		}
		return obj->getLastValue();
	}

	// Java: public native string getVersionName();
	CC7_JNI_METHOD(jstring, getMagicVersion)
	{
		auto obj = CC7_THIS_OBJ();
		if (!obj) {
			return NULL;
		}
		return cc7::jni::CopyToJavaString(env, obj->getVersion());
	}


	// ------------------------------------
	CC7_JNI_MODULE_CLASS_END()
	// ------------------------------------

	// end of the JNI wrapper
	// end of the file
