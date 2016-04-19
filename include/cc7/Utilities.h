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

#include <cc7/Platform.h>

namespace cc7
{
	template <typename T = size_t> T AlignValue(T value, size_t align)
	{
		CC7_ASSERT(align > 0, "align parameter must be greater than 0");
		if (!value) {
			return static_cast<T>(align);
		}
		return ((value + (align-1)) & ~(static_cast<T>(align)-1));
	}
	
} // cc7

#endif /* Utilities_h */
