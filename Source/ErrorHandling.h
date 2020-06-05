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

#ifndef _errorhanding_h
#define _errorhanding_h

#include "avisynth.h"

void HandleAvisynthError(HWND wnd, AvisynthError e);

void HandleErrorClipAvisynthError(HWND wnd, AvisynthError e);

void HandleNotFoundError(HWND wnd);

void MakeAVSErrorString(HWND wnd, char *scriptBuffer, const char *errorMsg);

#endif