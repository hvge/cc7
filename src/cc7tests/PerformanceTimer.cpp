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
#include <cc7tests/detail/TestUtilities.h>
#include <math.h>

namespace cc7
{
namespace tests
{
	
	PerformanceTimer::PerformanceTimer() :
		_base(Platform_GetCurrentTime())
	{
	}
	
	
	void PerformanceTimer::start()
	{
		_base = Platform_GetCurrentTime();
	}
	
	
	double PerformanceTimer::elapsedTime()
	{
		return Platform_GetTimeDiff(_base, Platform_GetCurrentTime());
	}
	
	
	double PerformanceTimer::measureBlock(std::function<void()> block)
	{
		start();
		block();
		return elapsedTime();
	}
	
	
	std::string PerformanceTimer::humanReadableTime(double time)
	{
		double seconds		= floor(time / 1000.0);
		double minutes		= floor(seconds / 60.0);
		double milliseconds	= (time - seconds * 1000.0);
		if (minutes > 0) {
			return detail::FormattedString("%.0fm %.3fs", minutes, seconds);
		} else if (seconds > 0) {
			return detail::FormattedString("%.3fs", time / 1000);
		}
		return detail::FormattedString("%.3fms", milliseconds);
	}
	
	
} // cc7::tests
} // cc7
