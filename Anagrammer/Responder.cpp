//
//  Responder.cpp
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
