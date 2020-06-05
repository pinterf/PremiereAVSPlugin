/*
 * Premiere AVS Import Filter
 *
 * Original code by Ben Rudiak-Gould (c) 2000
 *
 * This version fixed and extended by the Video Editors' Toolkit
 * Sourceforge Project
 * (http://sourceforge.net/projects/videoeditorskit/)
 *
 * Programming: Tom Ford <tom.ford@compsoc.net>
 *
 * (c) 2002-2009
 * 
 * Licensed under the GPL - see COPYING.txt
 *
 * Compiles under VC6SP5 and VC7
 *
 * Please see included readmes for building and usage instructions.
 */

//Class suitable for Premiere plugin

#include "WordWrap.h"

//Word wrap function
//Whitespace (newlines, spaces) are output just prior to text
//ie. if I come across \n, the newline is not output until the next bit of source text is ready
char *WordWrap::DoWordWrap(HWND wnd, const char *src, unsigned int length, char *dst, unsigned int wrapNo) {

	char curChar;
	unsigned int size;
	
	if(length < 1)
		return dst;

	TotalSize = 0;
	LineLength = 0;
	TotalLength = length;
	Finished = false;

	SrcCur = src;
	SrcBegin = src;
	DstPos = dst;

	while(*SrcCur != '\0' && !Finished) {
		
		curChar = *SrcCur;
		size = SrcCur - SrcBegin + 1;

		//If we have a space-type whitespace character
		//In this case in the buffer we have a word + a whitespace character (included in size)

		if(curChar == '\t' || curChar == ' ' || curChar == '\n') {

			//If we only have whitespace (or a newline) in the buffer ignore it
			if(size == 1) {

			}

			//If the word (including trailing whitespace) doesn't fit, we place it on the next line
			else if(LineLength + size > wrapNo + 1) {
				
				//---Case 1---

				OutputNewline();
				OutputSource(size - 1); //no whitespace
				SetLineCounter(size); //includes space, even though it's not output yet
			}

			//If the word fits but there is an extra space on the end, output the word then finish the line (ignore space)
			else if(LineLength + size > wrapNo) {
				
				//---Case 2---

				OutputLeadingWhitespace(); //space or newline as appropriate		
				OutputSource(size - 1);
				ResetLineCounter(); //start new line
			}
			//If the word fits, just output it and increment the line counter
			else {

				//---Case 3---

				OutputLeadingWhitespace();
				OutputSource(size - 1);
				IncreaseLineCounter(size);
			}

			//Skip the whitespace so it's not read into the buffer
			SkipChar();

			//If it was a newline, start a new row
			if(curChar == '\n' && size > 1) {
				
				//---Case 4---

				ResetLineCounter();
			}
		}

		//If we reach the end of the line without seeing a space, output what we have
		else if(size == wrapNo) {

			//---Case 5---

			OutputNewline();
			OutputSource(size);
			ResetLineCounter();
		}

		//Increment our position in the source
		++SrcCur;
	}

	//Deal with anything not yet output
	if(!Finished && SrcBegin != SrcCur) {
		
		//If the remainder fits onto the final line
		if(LineLength + size < wrapNo + 1) {
			
			//---Case 6---

			OutputLeadingWhitespace();
			OutputSource(size);
		}
		//Otherwise break the line and output the buffer on a new line
		else {

			//---Case 7---

			OutputNewline();
			OutputSource(size);
		}
	}

	//Terminate the string
	*DstPos = '\0';

	return dst;
}
