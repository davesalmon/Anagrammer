/*+
 *
 *  Permutation.cpp
 *
 *  Copyright © 2016 David C. Salmon. All Rights Reserved.
 *
 *  handle index permutations for folding results.
 *
-*/

#include "Permutation.h"

#include <algorithm>
#include <assert.h>

//----------------------------------------------------------------------------------------
//  Permutation::Permutation                                                  constructor
//
//      construct a permutation object for the specified count. Note that this is not
//		exactly a permutation in that it only rotates through the list of its indices.
//		Not sure what to call it.
//
//		To combine all the strings in a given list, we need only to add the strings in
//		order, shifting the order one place each time.
//
//  size_t n   -> the number of values
//
//  returns nothing
//----------------------------------------------------------------------------------------
Permutation::Permutation(size_t n)
{
	assert (n != 0);
	
	for (size_t i = 0; i < n; i++) {
		list.push_back(i);
	}
}

//----------------------------------------------------------------------------------------
//  rotate                                                                         static
//
//      rotate the values one place
//
//  std::vector<size_t> & v     -> vector to rotate
//
//  returns bool  				<- true if there are more rotations to come.
//----------------------------------------------------------------------------------------
static bool
rotate(std::vector<size_t> & v) {
	std::rotate(v.begin(), v.begin() + 1, v.end());
	return v[0] != 0;
}

//----------------------------------------------------------------------------------------
//  Permutation::next
//
//      move to the next rotation and return false if there are no more.
//
//  returns bool   <- true if not finished.
//----------------------------------------------------------------------------------------
bool
Permutation::next()
{
	if (list.size() == 1)
		return false;
	
	return rotate(list);
}
