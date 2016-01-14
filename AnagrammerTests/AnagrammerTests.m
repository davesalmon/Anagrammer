//
//  AnagrammerTests.m
//  AnagrammerTests
//
//  Created by David Salmon on 1/1/16.
//  Copyright (c) 2016 Dave Salmon. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <XCTest/XCTest.h>

#include "WordMatch.h"
#include "PermutationList.h"
#include <algorithm>
#include <set>

static WordMatch matchers("/usr/share/dict/words");

@interface AnagrammerTests : XCTestCase

@end

@implementation AnagrammerTests

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void) testMatch {

	std::string cpy("mereups");
	std::sort(cpy.begin(), cpy.end());

	StringArray list = matchers.matchWord(cpy);
	
	const std::set<std::string> matches = {"presume", "supreme"};
	
	XCTAssertEqual(list.size(), 2);
	XCTAssertTrue(matches.find(list[0]) != matches.end());
	XCTAssertTrue(matches.find(list[1]) != matches.end());
	
	for (auto i = 0; i < list.size(); i++) {
		NSLog(@"found match %s", list[i].c_str());
	}
}

void printVec(std::vector<size_t>& vals, bool tf)
{
	printf("vector: ");
	for (auto i = 0; i < vals.size(); i++) {
		printf("%ld ", vals[i]);
	}
	printf("%c\n", tf ? 't' : 'f');
}

- (void) testMorePerms {
	
	std::vector<StringArray> lists = {{"do", "you", "think"}, {"that", "this"}, {"is", "a", "valid", "test"}};
	
	std::vector<const StringArray*> lps = { &lists[0], &lists[1], &lists[2] };
	
	PermutationList l(lps);
	
	size_t count = 0;
	
	std::set<std::string> testSet;
	
	do {
		std::string s;
		for (auto i = 0; i < lists.size(); i++) {
			if (i != 0)
				s += ' ';
			s.append(lists[i][l.index(i)]);
		}
		
		// make sure the combinations are unique
		XCTAssertTrue(testSet.find(s) == testSet.end());
//		printf("%s\n", s.c_str());
		
		count++;
	} while(l.next());
	
	XCTAssertEqual(count, 3 * 2 * 4);
}

@end
