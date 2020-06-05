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

#ifndef _im_avisynth_h
#define _im_avisynth_h

#include "avisynth.h"
#include  "PrSDKImport.h"

#define BLANK_CLIP_LENGTH_FRAMES 432000 //5 hours

struct FileInfo {
	
    HMODULE avisynthDll;
    IScriptEnvironment* scriptEnvironment;
    AVSValue procScript;
	
	PrAudioSample audioPosition;
    __int64 audioNoOfSamples;
	int audioBytesPerSample;
	int numChannels;
	float audioSampleRate;
	char *audioConversionBuffer;
	int audioConversionBufferSize;
	PrAudioSampleType sampleType;

	SPBasicSuite		*suiteBasic;
	PrSDKAudioSuite		*suiteAudio;

    int errorCode;

    PClip clipCache;
    unsigned int lastWidth;
    unsigned int lastHeight;

    char avisynthResizeFilter[80];
};

#endif
