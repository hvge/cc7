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
#include <functional>

namespace cc7
{
namespace tests
{
	
	class PerformanceTimer
	{
	public:
		PerformanceTimer();
		
		void	start();
		double	elapsedTime();
		double	measureBlock(std::function<void()> block);
		
		static std::string humanReadableTime(double time);
		
	private:
		
		cc7::U64 _base;
	};
	
	
	// Following two functions must have platform specific implementation.
	
	cc7::U64	Platform_GetCurrentTime();
	double		Platform_GetTimeDiff(cc7::U64 start, cc7::U64 future);
	
} // cc7::tests
} // cc7
