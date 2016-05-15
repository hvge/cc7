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

#include <cc7tests/TestFile.h>

namespace cc7
{
namespace tests
{
	class TestResource;
	
	class TestDirectory
	{
	public:
		typedef const cc7::tests::TestResource* TResource;
		typedef std::vector<TResource> TResourceList;
		
		TestDirectory(std::initializer_list<TResource> il);
		~TestDirectory();
		
		TestFile findFile(const std::string & path) const;
		const TResourceList & allResources() const;
		
	private:
		TResourceList _resources;
	};
	
} // cc7::tests
} // cc7