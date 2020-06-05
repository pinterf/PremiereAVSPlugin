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

#ifndef _wordwrap_h
#define _wordwrap_h

#include <windows.h>

#include <memory.h>
#include <string.h>

//Class suitable for Premiere plugin - tested and working
//Should really be static, but static memory is not safe in Premiere

class WordWrap {

 private:
	const char *SrcBegin;
	const char *SrcCur;
	char *DstPos;
	unsigned int LineLength;
	unsigned int TotalSize;
	unsigned int TotalLength;
	bool Finished;

	void OutputSource(unsigned int size) {
	
		if(TotalSize + size < TotalLength) {
			memcpy(DstPos, SrcBegin, size);
			DstPos += size;
			SrcBegin += size;
			TotalSize += size;
		}
		else {
			int bytesToCopy = TotalLength - TotalSize - 1;
			if(bytesToCopy) {
				memcpy(DstPos, SrcBegin, bytesToCopy);
				DstPos += bytesToCopy;
			}
			Finished = true;
		}
	}

	void IncreaseLineCounter(unsigned int size) {
		
		LineLength += size;
	}

	void SetLineCounter(unsigned int size) {
		
		LineLength = size;
	}
		
	void OutputExtra(const char *string) {

		unsigned int size = strlen(string);

		if(TotalSize + strlen(string) < TotalLength) {
			memcpy(DstPos, string, size);
			DstPos += size;
			TotalSize += size;
		}
		else {
			int bytesToCopy = TotalLength - TotalSize - 1;
			if(bytesToCopy) {
				memcpy(DstPos, string, bytesToCopy);
				DstPos += size;
			}
			Finished = true;
		}
	}


	void OutputNewline() {

		if(TotalSize)  //catches outputing a newline on the start of the first line itself
			OutputExtra("\n");

	}

	void OutputSpace() {

		OutputExtra(" ");
	}

	void SkipChar() {

		SrcBegin++;
	}

	void ResetLineCounter() {

		LineLength = 0;
	}

	void OutputLeadingWhitespace() {
		if(LineLength)
			OutputSpace();
		else
			OutputNewline();
	}

 public:
	
	char *DoWordWrap(HWND wnd, const char *src, unsigned int TotalLength, char *dst, unsigned int wrapNo);

};

#endif
