//
//  StringMatch.h
//  Anagrammer
//
//  Created by David Salmon on 1/4/16.
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
	
	size_t size() const { return map.size(); }
	
private:
	
	// map the sorted word into the list if matching words
	std::map<std::string, StringArray> map;
	size_t wordSize;
	
	const std::string kVowels = "aeiouy";
	const std::set<std::string> kOKWords = { "grr", "mrs", "mr", "nth" };
};

#endif /* defined(__Anagrammer__StringMatch__) */
