//
//  Permutation.h
//  Anagrammer
//
//  Created by David Salmon on 1/5/16.
//  Copyright (c) 2016 Dave Salmon. All rights reserved.
//

#ifndef __Anagrammer__Permutation__
#define __Anagrammer__Permutation__

#include <vector>

class Permutation {
public:
	Permutation(size_t n);
	
	bool next();
	
	// return the current index for this permutation. (between 1 and n)
	size_t index() const { return list[0]; }
	
private:
	std::vector<size_t>	list;
};

#endif /* defined(__Anagrammer__Permutation__) */
