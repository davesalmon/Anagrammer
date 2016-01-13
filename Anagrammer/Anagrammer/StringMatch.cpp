/*+
 *
 *  StringMatch.cpp
 *
 *  Copyright © 2016 David C. Salmon. All Rights Reserved.
 *
 *  match strings to anagrams of a specific length.
 *
-*/

#include "StringMatch.h"
#include <assert.h>

const StringArray StringMatch::kEmpty;

//----------------------------------------------------------------------------------------
//  StringMatch::addWord
//
//      add a word to the map of anagrams.
//
//  const std::string& word    -> the word to add.
//
//  returns nothing
//----------------------------------------------------------------------------------------
void
StringMatch::addWord(const std::string& word)
{
	if (wordSize == 0)
		wordSize = word.length();

	assert(word.length() == wordSize);

	// check if there are no vowels in the string
	if (word.find_first_of(kVowels) == std::string::npos) {
		// and not in the list of OK words, the skip it
		if (kOKWords.find(word) == kOKWords.end()) {
#if DEBUG
			fprintf(stderr, "skipping vowellless string %s\n", word.c_str());
#endif
			return;
		}
	}
	
	// generate the map key
	std::string cpy(word);
	std::sort(cpy.begin(), cpy.end());
	
	map[cpy].push_back(word);
}

//----------------------------------------------------------------------------------------
//  StringMatch::matchWord
//
//      match the specified string. The string must be sorted.
//
//  const std::string& word    -> the word to anagram.
//
//  returns const StringArray& <- reference to anagram list for word.
//----------------------------------------------------------------------------------------
const StringArray&
StringMatch::matchWord(const std::string& word) const
{
	assert(word.length() == wordSize);
	assert(std::is_sorted(word.begin(), word.end()));
	
	// look up all variations that match the sorted character list.
	const auto& it = map.find(word);
	if (it == map.end())
		return kEmpty;
	return it->second;
}
