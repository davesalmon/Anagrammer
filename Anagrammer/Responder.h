//
//  Responder.h
//  Anagrammer
//
//  Created by David Salmon on 1/12/16.
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
