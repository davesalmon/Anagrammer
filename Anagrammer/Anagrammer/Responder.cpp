//
//  Responder.cpp
//  Anagrammer
//
//  Created by David Salmon on 1/12/16.
//  Copyright (c) 2016 Dave Salmon. All rights reserved.
//

#include "Responder.h"

void
Responder::postResult(const char* result) const
{
	if (delegate) {
		[delegate performSelectorOnMainThread: @selector(addResults:)
								   withObject: [NSString stringWithCString:result encoding:NSUTF8StringEncoding]
								waitUntilDone: YES];
	}
}
