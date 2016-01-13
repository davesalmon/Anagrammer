/*+
 *
 *  PermutationList.cpp
 *
 *  Copyright © 2016 David C. Salmon. All Rights Reserved.
 *
 *  cycle through the combinations of a list of values.
 *
-*/

#include "PermutationList.h"

//----------------------------------------------------------------------------------------
//  PermutationList::PermutationList                                          constructor
//
//      construct a vector of Permutation objects.
//
//  const s    -> 
//
//  returns nothing
//----------------------------------------------------------------------------------------
PermutationList::PermutationList(const std::vector<const StringArray*>& wordLists)
{
	for(const auto& l : wordLists) {
		perms.push_back(Permutation(l->size()));
	}
}

//----------------------------------------------------------------------------------------
//  PermutationList::next
//
//      rotate each Permutation list object in turn until all rotations of all objects
//      have been accounted for. This allows the combination of multiple lists of strings
//      in each unique combination.
//
//  returns bool   <- true if there are more combination.
//----------------------------------------------------------------------------------------
bool
PermutationList::next()
{
	for (auto & p : perms) {
		if (p.next())
			return true;
	}
	
	return false;
}
