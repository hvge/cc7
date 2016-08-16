#
# Copyright 2016 Juraj Durech <durech.juraj@gmail.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH:= $(call my-dir)

# -------------------------------------------------------------------------
# CC7 library
# -------------------------------------------------------------------------

include $(CLEAR_VARS)

NDK_TOOLCHAIN_VERSION := clang

# Library name
LOCAL_MODULE			:= libcc7
LOCAL_CFLAGS			:= $(EXTERN_CFLAGS)
LOCAL_CPPFLAGS			:= $(EXTERN_CFLAGS) -std=c++11

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../include \
	$(LOCAL_PATH)/../openssl/android/include \
	$(LOCAL_PATH)/cc7

# Android specific sources
LOCAL_SRC_FILES := \
	cc7/platform/android/PlatformAndroid.cpp \
	cc7/platform/android/JniHelper.cpp

# Multiplatform sources
LOCAL_SRC_FILES += \
	cc7/DebugFeatures.cpp \
	cc7/ByteRange.cpp \
	cc7/ByteArray.cpp \
	cc7/Base64.cpp \
	cc7/HexString.cpp

include $(BUILD_STATIC_LIBRARY)

# -------------------------------------------------------------------------
# CC7 Tests
# -------------------------------------------------------------------------

include $(CLEAR_VARS)

NDK_TOOLCHAIN_VERSION := clang

# Library name
LOCAL_MODULE			:= libcc7tests
LOCAL_CFLAGS			:= $(EXTERN_CFLAGS)
LOCAL_CPPFLAGS			:= $(EXTERN_CFLAGS) -std=c++11
LOCAL_CPP_FEATURES		+= exceptions
LOCAL_STATIC_LIBRARIES	:= cc7

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../include \
	$(LOCAL_PATH)/../openssl/android/include \
	$(LOCAL_PATH)/cc7tests

# Testing core
LOCAL_SRC_FILES := \
	cc7tests/TestManager.cpp \
	cc7tests/UnitTest.cpp \
	cc7tests/TestLog.cpp \
	cc7tests/TestFile.cpp \
	cc7tests/TestDirectory.cpp \
	cc7tests/TestResource.cpp \
	cc7tests/PerformanceTimer.cpp \
	cc7tests/JSONReader.cpp \
	cc7tests/JSONValue.cpp \
	cc7tests/detail/StringUtils.cpp

# Testing core (Android)
LOCAL_SRC_FILES += \
	cc7tests/platform/PerformanceTimerAndroid.cpp

# Unit tests (TestCore)
LOCAL_SRC_FILES += \
	cc7tests/tests/cc7base/tt7Testception.cpp \
	cc7tests/tests/cc7base/tt7JSONReaderTests.cpp


# Unit tests (CC7)
LOCAL_SRC_FILES += \
	cc7tests/tests/EmbeddedTestsList.cpp \
	cc7tests/tests/cc7base/cc7Base64Tests.cpp \
	cc7tests/tests/cc7base/cc7ByteArrayTests.cpp \
	cc7tests/tests/cc7base/cc7ByteRangeTests.cpp \
	cc7tests/tests/cc7base/cc7HexStringTests.cpp \
	cc7tests/tests/cc7base/cc7PlatformTests.cpp

# Generated files
LOCAL_SRC_FILES += \
	cc7tests/tests/test-data.generated/g_baseFiles.cpp


include $(BUILD_STATIC_LIBRARY)
