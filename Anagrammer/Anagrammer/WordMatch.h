//
//  WordMatch.h
//  Anagrammer
//
//  Created by David Salmon on 1/3/16.
//  Copyright (c) 2016 Dave Salmon. All rights reserved.
//

#ifndef __Anagrammer__WordMatch__
#define __Anagrammer__WordMatch__

#include "StringMatch.h"
#include "Responder.h"

#include <vector>
#include <string>

class WordMatch
{
public:
	
	WordMatch(const std::string& wordListFile);
	
	// return a list of strings that are anagrams of word
	const StringArray&	matchWord(const std::string& word) const;
	
	void matchWords(const std::string& word, const std::vector<size_t>& lengths, bool & bail) const;
	
	size_t maxWordLength() const
				{ return words.back().getWordSize(); }
	
	void setResponder(const Responder& r) { resp = r; }
	
	// return true if the length vector changed.
	static bool getLengths(std::vector<size_t>& lens, const char* s);

	// return true if the length vector changed.
	static bool checkLengths(std::vector<size_t> &lens, size_t count, size_t total);
	
private:
	
	// each element of words is a list of strings of that length.
	std::vector<StringMatch> words;
	
	Responder resp;
};

#endif /* defined(__Anagrammer__WordMatch__) */
