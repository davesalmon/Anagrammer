//
//  Responder.h
//  Anagrammer
//
//  Created by David Salmon on 1/12/16.
//  Copyright (c) 2016 Dave Salmon. All rights reserved.
//

#ifndef __Anagrammer__Responder__
#define __Anagrammer__Responder__

#import "AnagramDelegate.h"
#include "StringMatch.h"

class Responder {
	
public:
	
	Responder() : delegate(0) {}

	Responder(AnagramDelegate* d) : delegate(d) {}
	
	void postResult(const char* result) const;
	
private:
	AnagramDelegate* delegate;
};

#endif /* defined(__Anagrammer__Responder__) */
