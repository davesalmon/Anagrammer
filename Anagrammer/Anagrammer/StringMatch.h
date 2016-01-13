//
//  StringMatch.h
//  Anagrammer
//
//  Created by David Salmon on 1/4/16.
//  Copyright (c) 2016 Dave Salmon. All rights reserved.
//

#ifndef __Anagrammer__StringMatch__
#define __Anagrammer__StringMatch__

#include <vector>
#include <string>
#include <set>
#include <map>

typedef std::vector<std::string> StringArray;

class StringMatch {

public:
	
	static const StringArray kEmpty;

	StringMatch() : wordSize(0) {}
	
	void addWord(const std::string& word);
	
	const StringArray&	matchWord(const std::string& word) const;
	
	size_t getWordSize() const { return wordSize; }
	
private:
	
	// map the sorted word into the list if matching words
	std::map<std::string, StringArray> map;
	size_t wordSize;
	
	const std::string kVowels = "aeiouy";
	const std::set<std::string> kOKWords = { "grr", "mrs", "mr", "nth" };
};

#endif /* defined(__Anagrammer__StringMatch__) */
