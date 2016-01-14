/*+
 *
 *  AnagramDelegate.m
 *
 *  Copyright © 2016 David C. Salmon. All Rights Reserved.
 *
 *  handle UI for Anagrammer program
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

#import  "AnagramDelegate.h"
#include "WordMatch.h"
#include <vector>
#include <cstdlib>
#include <exception>

@interface AnagramDelegate () <NSTableViewDataSource, NSTableViewDelegate>
{
	uint					wordCount;
	NSString*				wordLengths;
	NSString* 				inputString;
	std::vector<size_t>*	lengths;
	NSMutableArray*			outputStrings;
	WordMatch*				matcher;
	
	bool					cancel;
	bool					finding;
}

@property (weak) IBOutlet NSWindow* 	window;
@property (weak) IBOutlet NSTableView*	outputTable;
@property (weak) IBOutlet NSTextField*	inputBox;
@property (weak) IBOutlet NSTextField*	lengthBox;
@property (weak) IBOutlet NSComboBox*	countBox;
@property (weak) IBOutlet NSButton*		findButton;

@property (copy) NSString* message;

@property NSString* 					wordListLocation;

- (void) setInputString: (NSString*) s;
- (NSString*) getInputString;

- (void) setWordCount: (int) newCount;
- (int) getWordCount;

- (void) setWordLengths: (NSString*) s;
- (NSString*) getWordLengths;

- (IBAction)findAnagrams:(id)sender;	// the button was pressed

@end

//----------------------------------------------------------------------------------------
//  split
//
//      split the string into a vector of substrings using command and space as
//      delimeters.
//
//  s              -> 
//
//  returns size_t <- 
//----------------------------------------------------------------------------------------
size_t
split(std::vector<std::string>& list, const char* str)
{
	size_t count = 0;
	while (true) {
		// find the delimit character
		size_t len = strcspn(str, " ,\t");
		if (len > 0) {
			list.push_back(std::string(str, len));
			count++;
		}
		if (str[len] == 0)
			break;
		str += len + 1;
	}
	return count;
}

//----------------------------------------------------------------------------------------
//  makeString                                                                     static
//
//      make a lengths string from the lengths
//
//  const s            -> 
//
//  returns NSString*  <- 
//----------------------------------------------------------------------------------------
static
NSString*
makeString(const std::vector<size_t>& list)
{
	std::string s;
	
	for (auto v : list) {
		
		char buf[32];
		
		snprintf(buf, 31, "%lu", v);
		
		if (s.length() != 0)
			s += ", ";
		s.append(buf);
	}
	
	return [NSString stringWithCString: s.c_str() encoding: NSUTF8StringEncoding];
}

@implementation AnagramDelegate

//----------------------------------------------------------------------------------------
//  applicationDidFinishLaunching:
//
//      called when the app is about to go live. Initialize UI
//
//  applicationDidFinishLaunching: NSNotification * aNotification  -> 
//
//  returns nothing
//----------------------------------------------------------------------------------------
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	// Insert code here to initialize your application
	lengths = new std::vector<size_t> ();
	matcher = new WordMatch("/usr/share/dict/words");
	matcher->setResponder(Responder(self));
	
	self.wordListLocation = @"/usr/share/dict/words";
	self.wordCount = 1;
	self.message = @"";
	finding = false;
	
	_findButton.enabled = NO;

}

//----------------------------------------------------------------------------------------
//  applicationWillTerminate:
//
//      called when app is about to quit.
//
//  applicationWillTerminate: NSNotification * aNotification   -> 
//
//  returns nothing
//----------------------------------------------------------------------------------------
- (void)applicationWillTerminate:(NSNotification *)aNotification {
	// Insert code here to tear down your application
	delete lengths;
	delete matcher;
}

//----------------------------------------------------------------------------------------
//  numberOfRowsInTableView:
//
//      return the number of rows in the output table.
//
//  numberOfRowsInTableView: NSTableView * tableView   -> 
//
//  returns NSInteger                                  <- 
//----------------------------------------------------------------------------------------
- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView
{
	return [outputStrings count];
}

//----------------------------------------------------------------------------------------
//  tableView:objectValueForTableColumn:row:
//
//      return the object for the specified row.
//
//  tableView: NSTableView * tableView                     -> 
//  objectValueForTableColumn: NSTableColumn * tableColumn -> 
//  row: NSInteger row                                     -> 
//
//  returns id                                             <- 
//----------------------------------------------------------------------------------------
- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row
{
	return [outputStrings objectAtIndex: row];
}

//----------------------------------------------------------------------------------------
//  setInputString:
//
//      the user has typed a new input string.
//
//  setInputString: NSString* s    -> 
//
//  returns nothing
//----------------------------------------------------------------------------------------
- (void) setInputString: (NSString*) s
{
#if DEBUG
	NSLog(@"set input string to %@", s);
#endif
	inputString = s;
	
	if (WordMatch::checkLengths(*lengths, wordCount, [s length])) {
		[self setWordLengths: makeString(*lengths)];
	}
	
	_findButton.enabled = [inputString length] > 0;
}

//----------------------------------------------------------------------------------------
//  getInputString
//
//      return the current input string.
//
//  returns NSString*  <- 
//----------------------------------------------------------------------------------------
- (NSString*) getInputString
{
	return inputString;
}

-(BOOL)validateInputString: (id *)ioValue error:(NSError * __autoreleasing *)outError
{
#if DEBUG
	NSLog(@"in inputString validate");
#endif
	
	NSDictionary *userInfoDict = nil;
	
	if (ioValue != nil) {
		NSString* val = *ioValue;
		NSInteger l = [val length];
		if (l >= 256) {
			userInfoDict = @{ NSLocalizedDescriptionKey : @"Input string is too long" };
		} else {
			unichar chars[256];
			
			[val getCharacters: chars range: NSMakeRange(0, l)];
			for (auto i = 0; i < l; i++) {
				if (!isalpha(chars[i])) {
					userInfoDict = @{ NSLocalizedDescriptionKey : @"Only letters are allowed in input string." };
					break;
				}
			}
		}
		
	} else {
		userInfoDict = @{ NSLocalizedDescriptionKey : @"String may not be empty" };
	}
	
	if (userInfoDict != nil && outError != nil) {
		*outError = [[NSError alloc] initWithDomain: @"AnagramError"
											   code: 0
										   userInfo: userInfoDict];
		return NO;
	}
	
	return YES;
}

//----------------------------------------------------------------------------------------
//  setWordCount:
//
//      the user has specified a new word count.
//
//  setWordCount: int newCount -> 
//
//  returns nothing
//----------------------------------------------------------------------------------------
- (void) setWordCount: (int) newCount
{
	if (newCount != wordCount) {
#if DEBUG
		NSLog(@"set word count to %u", newCount);
#endif
		wordCount = newCount;
		
		// change the size of the lengths array
		if (WordMatch::checkLengths(*lengths, newCount, [inputString length])) {
			[self setWordLengths: makeString(*lengths)];
		}
	}
}

//----------------------------------------------------------------------------------------
//  getWordCount
//
//      return the current word count.
//
//  returns int    <- 
//----------------------------------------------------------------------------------------
- (int) getWordCount
{
	return wordCount;
}

//----------------------------------------------------------------------------------------
//  setWordLengths:
//
//      the user has entered a new word lengths string.
//
//  setWordLengths: NSString* s    -> 
//
//  returns nothing
//----------------------------------------------------------------------------------------
- (void) setWordLengths: (NSString*) s
{
#if DEBUG
	NSLog(@"set word length string to %@", s);
#endif
	
	wordLengths = s;
	if (WordMatch::getLengths(*lengths, [s cStringUsingEncoding: NSUTF8StringEncoding])) {
		if (lengths->size() != wordCount) {
			[self setWordCount: (int)lengths->size()];
		}
	}
}

-(BOOL) validateWordLengths: (id *)ioValue error:(NSError * __autoreleasing *)outError
{
#if DEBUG
	NSLog(@"in wordLengths validate");
#endif
	NSDictionary *userInfoDict = nil;
	
	if (ioValue != nil) {
		
		NSString* val = *ioValue;
		
		{
			NSInteger l = [val length];
			unichar chars[256];
			
			[val getCharacters: chars range: NSMakeRange(0, l)];
			for (auto i = 0; i < l; i++) {
				if (!(isdigit(chars[i]) || chars[i] == ',' || chars[i] == ' ')) {
					userInfoDict = @{
							NSLocalizedDescriptionKey :
								@"word lengths must be a comma separated list of integers." };
					break;
				}
			}
		}
		
		if (userInfoDict == nil) {
			
			std::vector<size_t> tryLengths = *lengths;
			
			if (WordMatch::getLengths(tryLengths, [val cStringUsingEncoding: NSUTF8StringEncoding])) {
				if (WordMatch::checkLengths(tryLengths, tryLengths.size(), [inputString length])) {
					*ioValue = makeString(tryLengths);
				}
			}
		}
		
	} else {
		userInfoDict = @{ NSLocalizedDescriptionKey : @"String may not be empty" };
	}
	
	if (userInfoDict != nil && outError != nil) {
		*outError = [[NSError alloc] initWithDomain: @"AnagramError"
											   code: 0
										   userInfo: userInfoDict];
		return NO;
	}
	
	return YES;
}

//----------------------------------------------------------------------------------------
//  getWordLengths
//
//      return the word lengths string.
//
//  returns NSString*  <- 
//----------------------------------------------------------------------------------------
- (NSString*) getWordLengths
{
	return wordLengths;
}

//----------------------------------------------------------------------------------------
//  endAnagramThread:
//
//      the anagram thread ended, possibly with an error.
//
//  endAnagramThread: NSString* error   -> any error to display
//
//  returns nothing
//----------------------------------------------------------------------------------------
- (void) endAnagramThread: (NSString*) error
{
	if (error) {
#if DEBUG
		NSLog(@"error from anagram thread");
#endif
		// update the UI with an error sheet
		NSBeginCriticalAlertSheet(
				@"An error ocurred", @"OK", nil, nil, _window, nil, nil, nil, nil, @"%@", error);
	}
	
	NSLog(@"finished anagram thread");

	finding = false;
	[_inputBox setEnabled: YES];
	[_lengthBox setEnabled: YES];
	[_countBox setEnabled: YES];
	_findButton.title = @"Find Anagrams";
}

//----------------------------------------------------------------------------------------
//  addResults:
//
//      update the UI with the specified error.
//
//  addResults: NSString* results   -> the string to add.
//
//  returns nothing
//----------------------------------------------------------------------------------------
- (void) addResults: (NSString*) results
{
	if (outputStrings == nil) {
		outputStrings = [NSMutableArray array];
	}
	
	if (results) {
		
		NSUInteger loc = [outputStrings indexOfObject: results
										inSortedRange: NSMakeRange(0, [outputStrings count])
											  options: NSBinarySearchingInsertionIndex
									  usingComparator: ^(id obj1, id obj2)
						  {
							  return [obj1 compare:obj2];
						  }];
		
		[outputStrings insertObject: results atIndex: loc];
	}
	
	[_outputTable noteNumberOfRowsChanged];
	self.message = [NSString stringWithFormat: @"%ld anagrams found", [outputStrings count]];
}

//----------------------------------------------------------------------------------------
//  doAnagrams:
//
//      anagram generation thread.
//
//  doAnagrams: id sender  -> 
//
//  returns nothing
//----------------------------------------------------------------------------------------
- (void) doAnagrams: (id) sender
{
	@autoreleasepool {
		
		NSLog(@"started anagram thread");

		[outputStrings removeAllObjects];
		
		// initialize results
		[self performSelectorOnMainThread: @selector(addResults:)
							   withObject: nil
							waitUntilDone: NO];

		cancel = false;
		
		try {
		
			size_t maxLen = matcher->maxWordLength();
			for (auto l : *lengths) {
				if (l > maxLen) {
					char buf[256];
					sprintf(buf, "string is too long. Maximum match length is %ld", maxLen);
					throw std::runtime_error(buf);
				}
			}
			
			matcher->matchWords(
							[inputString cStringUsingEncoding: NSUTF8StringEncoding], *lengths, cancel);
			
			[self performSelectorOnMainThread: @selector(endAnagramThread:)
								   withObject: nil
								waitUntilDone: NO];

		} catch(const std::exception& ex) {
			
			// here we should show the error.
			
			fprintf(stderr, "failed with error %s\n", ex.what());
			
			[self performSelectorOnMainThread: @selector(endAnagramThread:)
								   withObject: [NSString stringWithCString: ex.what()
																  encoding: NSUTF8StringEncoding]
								waitUntilDone: NO];

		}
	}
}

//----------------------------------------------------------------------------------------
//  findAnagrams:
//
//      the user hit the findAnagrams button.
//
//  findAnagrams: id sender    -> 
//
//  returns nothing
//----------------------------------------------------------------------------------------
- (IBAction)findAnagrams:(id)sender {
	
	if (finding) {
		NSLog(@"cancelling find anagram thread");
		cancel = true;
		return;
	}

	// update all values that might be being edited.
	[[_inputBox window] makeFirstResponder: nil];
	[[_lengthBox window] makeFirstResponder: nil];
	[[_countBox window] makeFirstResponder: nil];

	finding = true;
	[_inputBox setEnabled: NO];
	[_lengthBox setEnabled: NO];
	[_countBox setEnabled: NO];
	_findButton.title = @"Cancel";
	
	self.message = @"Searching...";
	
#if DEBUG
	NSLog(@"starting find anagram thread");
#endif
	
	[self performSelectorInBackground: @selector(doAnagrams:) withObject: nil];
	
}


@end
