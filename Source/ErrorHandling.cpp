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

#include <windows.h>
#include <stdio.h>

#include "ErrorHandling.h"
#include "im-avisynth.h"
#include "WordWrap.h"

void HandleAvisynthError(HWND wnd, AvisynthError e) {

	char errorBuffer[8192];
	strncpy(errorBuffer, "Avisynth Error:\n\n", 19);
	strncat(errorBuffer, e.msg, 8172);
	MessageBox(wnd, errorBuffer, "Premiere AVS plugin Error", MB_OK | MB_ICONERROR);  
}

void HandleErrorClipAvisynthError(HWND wnd, AvisynthError e) {

	char errorBuffer[8192];
	strncpy(errorBuffer, "Avisynth Error when trying to create error display:\n\n", 56);
	strncat(errorBuffer, e.msg, 8135);
	MessageBox(wnd, errorBuffer, "Premiere AVS plugin Error", MB_OK | MB_ICONERROR);  
}

void HandleNotFoundError(HWND wnd) {

	MessageBox(wnd, "Avisynth Error: Invoke not found. Please reinstall Avisynth.", "Premiere AVS plugin Error", MB_OK | MB_ICONERROR);
}

//Make an Avisynth script including a wrapped version of errorMsg and put it in scriptBuffer
void MakeAVSErrorString(HWND wnd, char *scriptBuffer, const char *errorMsg) {

	char wrappedError[8192];

	WordWrap wrapper;
	wrapper.DoWordWrap(wnd, errorMsg, 8192, wrappedError, 35);

	char errorTitle[8192];
	char errorLine1[8192];
	char errorLine2[8192];
	char errorLine3[8192];
	char singleLine[8192];
	char tempLine[8192];

	_snprintf(errorTitle, 8192, "Subtitle(\"AVS FILE ERROR!\",text_color=$FF3333,align=2,size=(Width()/10),spc=1,y=(Height()/2)-(Width()/5))");
	_snprintf(errorLine1, 8192, "ScriptClip(\"\"\"subtitle(\"Current clip frame: \" + string(current_frame),align=9,size=Width()/20)\"\"\")\n");

	const char *startchar = wrappedError;
	const char *cc = wrappedError;
	
	const char *subtitleStart = "Subtitle(\"\"\"";
	const char *subtitlePreNum = "\"\"\",text_color=$FF3333,align=2,size=(Width()/20),spc=1,y=(Height()/2)-(Width()/10)+(";
	const char *subtitlePostNum = "*Width()/20))\n";
	
	int lines = 0;

	strcpy(errorLine2, " ");

	//Create a new subtitle line on each newline in wrappedError

	while(*cc != '\0') {
		if(*cc == '\n') {
			strncpy(singleLine, startchar, cc - startchar);
			singleLine[cc - startchar] = '\0';
			_snprintf(tempLine, 8192, "%s%s%s %i %s", subtitleStart, singleLine, subtitlePreNum, lines, subtitlePostNum);
			strncat(errorLine2, tempLine, 8192);
			startchar = cc + 1;
			lines++;
		}
		++cc;
	}

	//Do last line
	if(startchar != '\0') {
		strncpy(singleLine, startchar, 8192);
		singleLine[cc - startchar + 1] = '\0';
		_snprintf(tempLine, 8192, "%s%s%s %i %s", subtitleStart, singleLine, subtitlePreNum, lines, subtitlePostNum);
		strncat(errorLine2, tempLine, 8192);
	}

	_snprintf(errorLine3, 8192, "ShowSMPTE()");
	_snprintf(scriptBuffer, 8192, "BlankClip(%i, 640, 480)\n%s %s %s %s", BLANK_CLIP_LENGTH_FRAMES, errorTitle, errorLine1, errorLine2, errorLine3);
}