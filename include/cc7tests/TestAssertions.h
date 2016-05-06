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

/**
 This file contains various macros for assertion evaluation in unit tests.
 All these macros should be used ony in test methods, in context of UnitTest,
 or derived classes. All macros expects that 'this' is the pointer to 
 UnitTest-like object.
 */

/**
 Triggers failure when condition is not true
 */
#define ccstAssertTrue(condition, ...)																	\
	try {																								\
		if ((condition) != true) {																		\
			this->tl().logIncident(__FILE__, __LINE__, "(" #condition ") != true", "" __VA_ARGS__);		\
		}																								\
	} catch (...) {																						\
		this->tl().logIncident(__FILE__, __LINE__, "Evaluation failed (" #condition ")", "");			\
	}

/**
 Triggers failure when condition is not false
 */
#define ccstAssertFalse(condition, ...)																	\
	try {																								\
		if ((condition) != false) {																		\
			this->tl().logIncident(__FILE__, __LINE__, "(" #condition ") != false", "" __VA_ARGS__);	\
		}																								\
	} catch (...) {																						\
		this->tl().logIncident(__FILE__, __LINE__, "Evaluation failed (" #condition ")", "");			\
	}

/**
 Triggers failure when pointer is not NULL
 */
#define ccstAssertNull(object, ...)																		\
	if (object != nullptr) {																			\
		this->tl().logIncident(__FILE__, __LINE__, "(" #object ") != NULL", "" __VA_ARGS__);			\
	}

/**
 Triggers failure when pointer is NULL
 */
#define ccstAssertNotNull(object, ...)																	\
	if (object == nullptr) {																			\
		this->tl().logIncident(__FILE__, __LINE__, "(" #object ") == NULL", "" __VA_ARGS__);			\
	}

/**
 Triggers failure when both parameters are not equal
 */
#define ccstAssertEqual(aa, bb, ...)																	\
	try {																								\
		if (!(aa == bb)) {																				\
			this->tl().logIncident(__FILE__, __LINE__, "(" #aa " != " #bb ")", "" __VA_ARGS__);			\
		}																								\
	} catch (...) {																						\
		this->tl().logIncident(__FILE__, __LINE__, "Evaluation failed (" #aa " == " #bb ")", "");		\
	}

/**
 Triggers failure when both parameters are equal
 */
#define ccstAssertNotEqual(aa, bb, ...)																	\
	try {																								\
		if (aa == bb) {																					\
			this->tl().logIncident(__FILE__, __LINE__, "(" #aa " == " #bb ")", "" __VA_ARGS__);			\
		}																								\
	} catch (...) {																						\
		this->tl().logIncident(__FILE__, __LINE__, "Evaluation failed (" #aa " != " #bb ")", "");		\
	}

/**
 Triggers failure when both memory regions are not equal
 */
#define ccstAssertEqualMemSize(p1, p2, size, ...)														\
	if (memcmp(p1, p2, size) != 0) {																	\
		this->tl().logIncident(__FILE__, __LINE__, "mem(" #p1 ") == mem(" #p2 ")", "" __VA_ARGS__);		\
	}

/**
 Triggers failure when both memory regions are not equal. The size of memory area 
 is determined by sizeof(expected_array) and therefore the ptr parameter must point
 to at least the same amount of bytes.
 */
#define ccstAssertEqualMemArray(ptr, expected_array, ...)												\
	if (memcmp(ptr, expected_array, sizeof(expected_array)) != 0) {										\
		this->tl().logIncident(__FILE__, __LINE__, "mem(" #ptr ") == mem(" #expected_array ")", "" __VA_ARGS__);	\
	}


/**
 Always triggers failure
 */
#define ccstFailure(...)																				\
	this->tl().logIncident(__FILE__, __LINE__, NULL, "" __VA_ARGS__);


/**
 Adds simple message to the test log
 */
#define ccstMessage(...)																				\
	this->tl().logFormattedMessage("" __VA_ARGS__);

