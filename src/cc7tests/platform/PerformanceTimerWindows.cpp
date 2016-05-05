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

#if !defined(CC7_WINDOWS)
#error "This file is designed for Windows platforms only"
#endif

namespace cc7
{
namespace tests
{
	static int		s_timebase_initialized = 0;
	static double	s_inv_frequency;

	
	static void _TimerInitialization()
	{
		if (!s_timebase_initialized) {
			LARGE_INTEGER frequency;
			QueryPerformanceFrequency(&frequency);
			s_inv_frequency = 1000000.0 / (double)frequency.QuadPart;	// us
			s_timebase_initialized = 1;
		}
	}

	cc7::U64 Platform_GetCurrentTime()
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		return time.QuadPart;
	}
	
	double Platform_GetTimeDiff(cc7::U64 start, cc7::U64 future)
	{
		_TimerInitialization();
		return (future - start) * s_inv_frequency;
	}

} // cc7::tests
} // cc7




