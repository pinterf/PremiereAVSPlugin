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

#include "ImportAudio.h"

#include "im-avisynth.h"

#include <stdio.h> //debug

int ImportAudio7(imStdParms* stdParms, imFileRef ref, imImportAudioRec7* audioRec) 
{
	FileInfo *pfi = (FileInfo *)audioRec->privateData;

	PrAudioSample startAudioPosition = 0;
	PrAudioSample numAudioFrames = 0;
 
	//If the clip is not loaded correctly, just return silence

	if(pfi->errorCode == 1 || pfi->errorCode == 4) {
	
		memset(audioRec->buffer, 0, audioRec->size);
		return imNoErr;
	}

	//A position less than zero is a sequential read and importer returns the next piece of audio
	if (audioRec->position < 0)
		startAudioPosition = pfi->audioPosition;
	else
		startAudioPosition = audioRec->position;

	//Check if request is too large
	if (startAudioPosition + audioRec->size > pfi->audioNoOfSamples)
	{
		numAudioFrames = pfi->audioNoOfSamples - startAudioPosition;
		pfi->audioPosition = pfi->audioNoOfSamples;
	}
	else
	{
		numAudioFrames = audioRec->size;
		pfi->audioPosition = startAudioPosition + audioRec->size;
	}
	
	PClip clip = pfi->procScript.AsClip();

	//Resize audioConversionBuffer

	int bufferSize = audioRec->size * pfi->audioBytesPerSample * pfi->numChannels;

	//Allocate a buffer first time
	if(pfi->audioConversionBuffer == 0)
	{
		pfi->audioConversionBuffer = stdParms->piSuites->memFuncs->newPtr(audioRec->size * pfi->audioBytesPerSample * pfi->numChannels);
	}
	//If a different size is requested, resize the buffer
	else if(pfi->audioConversionBufferSize != bufferSize) {
		stdParms->piSuites->memFuncs->setPtrSize((PrMemoryPtr *)(&(pfi->audioConversionBuffer)), bufferSize);
	}
	
	pfi->audioConversionBufferSize = bufferSize;

	//Get audio from the clip
	clip->GetAudio(pfi->audioConversionBuffer, startAudioPosition, numAudioFrames, pfi->scriptEnvironment);

	if(pfi->audioBytesPerSample == 1) //Convert 8 bit audio
	{
		pfi->suiteAudio->UninterleaveAndConvertFrom8BitInteger(pfi->audioConversionBuffer, audioRec->buffer, pfi->numChannels, audioRec->size);
	}
	else if(pfi->audioBytesPerSample == 2) //Convert 16 bit audio
	{
		pfi->suiteAudio->UninterleaveAndConvertFrom16BitInteger((short*)pfi->audioConversionBuffer, audioRec->buffer, pfi->numChannels, audioRec->size);
	}
	else if(pfi->audioBytesPerSample == 3) //Convert 24 bit audio
	{
		pfi->suiteAudio->UninterleaveAndConvertFrom24BitInteger(pfi->audioConversionBuffer, audioRec->buffer, pfi->numChannels, audioRec->size);
	}
	else if(pfi->audioBytesPerSample == 4) //Convert 32 bit audio
	{
		pfi->suiteAudio->UninterleaveAndConvertFrom32BitInteger((long*)pfi->audioConversionBuffer, audioRec->buffer, pfi->numChannels, audioRec->size);
	}

	return imNoErr;
}

int ImportAudio(imStdParms *stdParms, imFileRef ref, imImportAudioRec *audioRec) {

	FileInfo *pfi = (FileInfo *)audioRec->privatedata;

	//Only read audio if the desired file was opened

	if(!pfi->errorCode) {
		PClip clip = pfi->procScript.AsClip();

		int sampleToGet = audioRec->sample / (pfi->audioBytesPerSample * pfi->numChannels);
		int sizeToGet = audioRec->size / (pfi->audioBytesPerSample * pfi->numChannels);

		clip->GetAudio(audioRec->buffer, sampleToGet, sizeToGet, pfi->scriptEnvironment);
	}
	else {

		//On a black screen or error clip send blank audio
		memset(audioRec->buffer, 127, audioRec->size);
	}

	return imNoErr;
}