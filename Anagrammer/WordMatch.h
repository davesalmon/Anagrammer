//
//  WordMatch.h
//  Anagrammer
//
//  Created by David Salmon on 1/3/16.
//  Copyright (c) 2016 Dave Salmon. All rights reserved.
//
/*
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License along
 with this program; if not, write to the Free Software Foundation, Inc.,
 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

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
