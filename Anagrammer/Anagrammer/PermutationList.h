//
//  PermutationList.h
//  Anagrammer
//
//  Created by David Salmon on 1/5/16.
//  Copyright (c) 2016 Dave Salmon. All rights reserved.
//

#ifndef __Anagrammer__PermutationList__
#define __Anagrammer__PermutationList__

#include "Permutation.h"
#include "StringMatch.h"

class PermutationList
{
public:
	
	PermutationList(const std::vector<const StringArray*>& wordLists);
	
	bool next();
	
	size_t index(size_t whichList) const
			{ return perms[whichList].index(); }
	
private:
	
	std::vector<Permutation> perms;
};

#endif /* defined(__Anagrammer__PermutationList__) */
