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

#include <cc7tests/TestDirectory.h>
#include <cc7tests/TestResource.h>

namespace cc7
{
namespace tests
{

	TestDirectory::TestDirectory(std::initializer_list<TResource> il) :
		_resources(il)
	{
	}
	
	TestDirectory::~TestDirectory()
	{
	}
	
	TestFile TestDirectory::findFile(const std::string &path) const
	{
		for (auto&& resource : _resources) {
			if (path == resource->name()) {
				return TestFile(resource);
			}
		}
		throw std::invalid_argument("File '" + path + "' not found.");
	}
	
	const TestDirectory::TResourceList & TestDirectory::allResources() const
	{
		return _resources;
	}
	
	

} // cc7::tests
} // cc7
