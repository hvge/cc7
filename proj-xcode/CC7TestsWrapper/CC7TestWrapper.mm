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

#import <XCTest/XCTest.h>
#include <cc7tests/CC7Tests.h>

using namespace cc7;

@interface CC7TestWrapper : XCTestCase
@end

@implementation CC7TestWrapper

- (void)testRunCC7Tests
{
	// Create default TestManager with all embedded tests.
	tests::TestManager * manager = tests::TestManager::createDefaultManager();
	// Enable passing test log directly to the NSLog.
	manager->tl().setDumpToSystemLogEnabled(true);
	
	// Run tests and evaluate result
	bool result = manager->runAllTests();

	tests::TestLogData log_data = manager->tl().logData();
	tests::TestManager::releaseManager(manager);
	
	if (!result) {
		NSLog(@"Incidents:\n%@", [NSString stringWithUTF8String:log_data.incidents.c_str()]);
	}
	NSLog(@"Full test log:\n%@", [NSString stringWithUTF8String:log_data.log.c_str()]);
	
	// Finally, report result to Xcode
	XCTAssertTrue(result, @"Test failed. Check debug log for details.");
}

@end
