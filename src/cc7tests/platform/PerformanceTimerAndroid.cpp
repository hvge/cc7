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

#include <cc7/Platform.h>
#include <time.h>

#if !defined(CC7_ANDROID)
#error "This file is designed for Android platform only"
#endif

namespace cc7
{
namespace tests
{
	cc7::U64 Platform_GetCurrentTime()
	{
		struct timespec res;
		clock_gettime(CLOCK_REALTIME, &res);
		return (cc7::U64)(1000000.0*res.tv_sec + (double)res.tv_nsec * 1.0/1000.0);
	}
	
	double Platform_GetTimeDiff(cc7::U64 start, cc7::U64 future)
	{
		return (future - start) * 1.0/1000.0;
	}


} // cc7::tests
} // cc7
