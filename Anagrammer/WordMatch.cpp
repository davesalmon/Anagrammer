/*+
 *
 *  WordMatch.cpp
 *
 *  Copyright © 2016 David C. Salmon. All Rights Reserved.
 *
 *  match a word using any of the word lists.
 *
-*/
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

#include "WordMatch.h"
#include "PermutationList.h"
#include <fstream>
#include <algorithm>
#include <exception>
#include <assert.h>
#include <numeric>

//----------------------------------------------------------------------------------------
//  WordMatch::WordMatch                                                      constructor
//
//      read in the list of words and construct word lists of each length found. Skip
//      capitalized words and words that have no vowels.
//
//  const std::string& wordListFile    -> path to file containing words to use for results.
//
//  returns nothing
//----------------------------------------------------------------------------------------
WordMatch::WordMatch(const std::string& wordListFile)
{
	std::ifstream s(wordListFile);
	
	std::string line;
	while(!s.eof()) {
		s >> line;
		
		size_t len = line.length();
		
		if (len > 0) {
		
			if (isupper(line[0]))
				continue;
			
			// if there is not a StringMatch for words with length len, create one.
			if (len >= words.size()) {
				words.resize(len + 1, StringMatch());
			}
			
			// and always to lower case
			std::transform(line.begin(), line.end(), line.begin(), ::tolower);

			words[len].addWord(line);
		}
	}
}

//----------------------------------------------------------------------------------------
//  WordMatch::matchWord
//
//      match a single word. Basically, find the list with the appropriate length and look
//      up the anagram list.
//
//  const std::string& word    -> the word to check.
//
//  returns const StringArray& <- 
//----------------------------------------------------------------------------------------
const StringArray&
WordMatch::matchWord(const std::string& word) const
{
	size_t len = word.length();
	if (len < words.size()) {
		return words[word.length()].matchWord(word);
	}
	return StringMatch::kEmpty;
}

//----------------------------------------------------------------------------------------
//  WordMatch::wordCount
//
//      return the number of words.
//
//  returns size_t <- 
//----------------------------------------------------------------------------------------
size_t
WordMatch::wordCount() const
{
	size_t sum = 0;
	for (const auto & l : words) {
		sum += l.size();
	}
	
	return sum;
}

// return the insert position for part.
//----------------------------------------------------------------------------------------
//  findInsertLocation                                                             static
//
//      return the insert location into dest of part to appends words in lexical order.
//
//  const std::string& dest -> the destination for part.
//  const std::string& part -> the part to insert.
//
//  returns std::size_t    <- the insert location for part to maintain word order.
//----------------------------------------------------------------------------------------
static std::size_t
findInsertLocation(const std::string& dest, const std::string& part)
{
	std::size_t pos = 0;
	std::size_t destlen = dest.length();
	
	while(pos < destlen) {
	
		if (dest.compare(pos, destlen - pos, part) >= 0) {
			return pos;
		}
		
		// move to the next space
		while (pos < dest.length() && dest[pos] != ' ')
			pos++;
		
		// and first char after space.
		pos++;
	}
	
	return destlen;
}

//----------------------------------------------------------------------------------------
//  WordMatch::matchWords
//
//      match multiple words to the input string. The lengths of the words are given by
//      the lengths vector.
//
//  const std::string& input              -> the input string
//  const std::vector<size_t>& lengths    -> the lengths (and count) of substrings.
//  bool & bail                           -> set true (by another thread) to cancel search.
//
//  returns StringArray    <- the result array.
//----------------------------------------------------------------------------------------
void
WordMatch::matchWords(const std::string& input, const std::vector<size_t>& lengths, bool & bail) const
{
	// only one word, just call the one word method
	if (lengths.size() <= 1) {
		// sort the word
		std::string cpy(input);
		std::sort(cpy.begin(), cpy.end());
		const StringArray& m = matchWord(cpy);
		for (auto & s : m) {
			resp.postResult(s.c_str());
		}
		
		return;
	}
	
	// check the lengths
	size_t total = 0;
	for (auto l : lengths) {
		total += l;
	}
	
	if (total != input.length()) {
		throw std::runtime_error("total length of output words must match string length");
	}
	
	StringArray outMatches;
	
	// make a copy of the input string and sort it so we can run through the permutations.
	// and always to lower case
	std::string word(input);
	std::transform(word.begin(), word.end(), word.begin(), ::tolower);
	std::sort(word.begin(), word.end());

	std::map<std::string, const StringArray*> cache;
	
	// loop over the permutations of the characters in the input string (word).
	do {
		std::vector<const StringArray*> matches;
		
		// find matches for each part of the string (lengths denoted by the lengths array)
		size_t pos = 0;
		for (auto l : lengths) {
			

			// get the appropriate substring of the input word.
			std::string part(word.substr(pos, l));

			// sort the part
			std::sort(part.begin(), part.end());
			
			// see if we already have a list.
			auto it = cache.find(part);
			
			// if not match that part to our word list.
			const StringArray* m = it != cache.end() ? it->second : &matchWord(part);
			
			if (it == cache.end()) {
				cache.emplace(std::make_pair(part, m));
			}

			// no matches for any given part and we are done.
			if (bail || m->size() == 0) {
				break;
			}
			
			// add the matches to the list.
			matches.push_back(m);
			
			pos += l;
		}
		
		// if each part generated a list, we have results.
		if (matches.size() == lengths.size()) {
			
			// combine the words found in each of the lists. The permutation list object
			// tracks the combinations of words from each list, such that each word
			// in each list is combined with all words from the subsequent list(s).
			PermutationList l(matches);
			
			do {
				std::string s;
				for (auto i = 0; i < matches.size(); i++) {

					std::string part((*matches[i])[l.index(i)]);
					
					// assemble the words in order. This allows us to
					// remove a lot of duplicates in the case of multiple word matches.
					std::size_t pos = findInsertLocation(s, part);
					
					if (pos > 0) {
						s += ' ';
						pos++;
					} else {
						if (s.length() != 0) {
							part += ' ';
						}
					}
					s.insert(pos, part);
				}
				
				const auto & it = std::lower_bound(outMatches.begin(), outMatches.end(), s);
				
				if (it == outMatches.end() || !(*it == s)) {
					outMatches.insert(it, s);
					resp.postResult(s.c_str());
				}
				
			} while(l.next());
		}
	}
	while(!bail && std::next_permutation(word.begin(), word.end()));

	fprintf(stderr, "match done\n");
}

//----------------------------------------------------------------------------------------
//  WordMatch::getLengths
//
//      return true if the lengths vector is changed after parsing the (comma delimited)
//      string and comparing it with the original vector.
//
//  std::vector<size_t>& l   -> the lengths vector.
//  const char* s            -> the string (e.g., 2,3,4)
//
//  returns bool   <- true if the string caused changes to l.
//----------------------------------------------------------------------------------------
bool
WordMatch::getLengths(std::vector<size_t>& l, const char* s)
{
	// return true if the length vector changed.
	bool changed = false;
	size_t ofs = 0;
	while (true) {
		// find the delimit character
		size_t len = strcspn(s, " ,\t");
		if (len > 0) {
			std::string part(s, len);
			
			char* ePtr = nullptr;
			
			size_t val = strtoul(part.c_str(), &ePtr, 10);
			
			if (*ePtr != 0)
				throw std::runtime_error("failed to convert string");
			
			if (ofs >= l.size()) {
				l.push_back(val);
				changed = true;
			} else if (l[ofs] != val) {
				l[ofs] = val;
				changed = true;
			}
			
			ofs++;
		}
		
		if (s[len] == 0)
			break;
		s += len + 1;
	}

	if (ofs < l.size()) {
		l.resize(ofs);
		changed = true;
	}
	
	return changed;
}

//----------------------------------------------------------------------------------------
//  WordMatch::checkLengths
//
//      check the lengths in the supplied vector and retun true (and modify the vector) if
//      the vector required modification to match either the word count or the total
//      number of characters.
//
//  std::vector<size_t> &lens -> the vector of substring lengths.
//  size_t count              -> the number of substrings.
//  size_t total              -> the length of the input string.
//
//  returns bool   <- true if the lens vector needed changes.
//----------------------------------------------------------------------------------------
bool
WordMatch::checkLengths(std::vector<size_t> &lens, size_t count, size_t total)
{
	// check the size and total and return true if the list changes.
	bool changed = false;

	if (lens.size() > count) {
		lens.resize(count, 0);
		changed = true;
	} else if (lens.size() < count) {
		size_t val = std::accumulate(lens.begin(), lens.end(), 0);
		size_t each = val / count;
		
		if (each > 0) {
			std::fill(lens.begin(), lens.end(), each);
		}

		lens.resize(count, each);
		changed = true;
	}

	size_t sum = std::accumulate(lens.begin(), lens.end(), 0);
	
	if (total == 0 && total != sum) {
		changed = true;
		for (auto & v : lens)
			v = 0;
		return changed;
	}
	
	if (sum < total) {
		// distribute the excess over the elements.
		changed = true;
		size_t dist = total - sum;
		
		while(dist != 0) {
			for (auto& v : lens) {
				v += 1;
				if (--dist == 0)
					break;
			}
		}
	} else if (sum > total) {
		// distribute the deficit over positive values.
		changed = true;
		size_t dist = sum - total;
		
		while(dist != 0) {
			for (auto& v : lens) {
				if (v != 0) {
					v -= 1;
					if (--dist == 0)
					break;
				}
			}
		}
	}
	
	return changed;
}
