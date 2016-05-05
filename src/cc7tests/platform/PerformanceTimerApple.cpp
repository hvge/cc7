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

#include <cc7tests/PerformanceTimer.h>

#include <mach/mach.h>
#include <mach/mach_time.h>
#include <unistd.h>

#if !defined(CC7_APPLE)
#error "This file is designed for Apple platforms only"
#endif

namespace cc7
{
namespace tests
{
	static int		s_timebase_initalized = 0;
	static double	s_timebase_multiplier = 0;

	static void _TimerInitialization()
	{
		if (!s_timebase_initalized) {
			mach_timebase_info_data_t timebase;
			mach_timebase_info(&timebase);
			s_timebase_multiplier = (1e-6 * (double) timebase.numer / (double) timebase.denom);
			s_timebase_initalized = 0;
		}
	}
	
	cc7::U64 Platform_GetCurrentTime()
	{
		return mach_absolute_time();
	}
	
	double Platform_GetTimeDiff(cc7::U64 start, cc7::U64 future)
	{
		_TimerInitialization();
		return s_timebase_multiplier * (future - start);
	}
	
	
} // cc7::tests
} // cc7
