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
namespace utilities
{
	/**
	 Aligns |value| to required alignment, specified by the template parameter |Align|.
	 The returned value is higher or equal to |value|. If the |value| is zero then 
	 returns |Align|. The template parameter must be power of 2, otherwise the compilation
	 error is produced.
	 */
	template<size_t Align> size_t AlignValue(size_t value)
	{
		static_assert(Align > 0, "Align must be greater than 0");
		static_assert((Align & (~Align + 1)) == Align, "Align must be power of 2");
					  
		if (value) {
			return (value + (Align - 1)) & ~(Align - 1);
		}
		return Align;
	}

} // cc7::utilities
} // cc7

