/*+
 *
 *  PermutationList.cpp
 *
 *  Copyright © 2016 David C. Salmon. All Rights Reserved.
 *
 *  cycle through the combinations of a list of values.
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
