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

#include "ImportImage.h"

#include "im-avisynth.h"
#include "ErrorHandling.h"

//Init avisynth and popular the FileInfoRec
int ImportImage(imStdParms* stdparms, imFileRef ref, imImportImageRec* imageRec) {

	FileInfo* pfi = (FileInfo *)imageRec->privatedata;
	
	PClip clip;
	PVideoFrame f;

	//Calculate frame to get

	int frameToGet = imageRec->pos / imageRec->sampleSize;

	//Destination dimensions

	const unsigned int dst_width = imageRec->dstWidth, dst_height = imageRec->dstHeight;
	
	//If we have been unable to load the required file or produce an error clip, blit black frame to the output buffer
	
	if(pfi->errorCode == 1) {
	
		OutputBlackScreen(imageRec);
		return imNoErr;
	}

	//Output desired clip (or error clip) to the output buffer

	//Use avisynth

	//If the cache is OK for this frame use it
	if(pfi->lastWidth == dst_width && pfi->lastHeight == dst_height) {
		clip = pfi->clipCache;
	}
	else {

		//Otherwise create a new clip with the appropriate resize filter
		//Blank frames on Avisynth error here

		AVSValue args[3] = { pfi->procScript, imageRec->dstWidth, imageRec->dstHeight }; 
		AVSValue resizeRes;

		try {
			resizeRes = pfi->scriptEnvironment->Invoke(pfi->avisynthResizeFilter, AVSValue(args, 3));
		}
		catch(IScriptEnvironment::NotFound) {

			HWND wnd = (HWND) stdparms->piSuites->windFuncs->getMainWnd();
			HandleNotFoundError(wnd);
			OutputBlackScreen(imageRec);
			return imNoErr;
		}
		catch(AvisynthError e) {
			
			HWND wnd = (HWND) stdparms->piSuites->windFuncs->getMainWnd();
			HandleAvisynthError(wnd, e);
			OutputBlackScreen(imageRec);
			return imNoErr;
		}

		//Get new clip and cache
		clip = resizeRes.AsClip();
		pfi->clipCache = clip;
	}

	//Get frame

	f = clip->GetFrame(frameToGet, pfi->scriptEnvironment);

	//Copy from avisynth frame to Premiere buffer

	char *new_src = (char *)f->GetReadPtr();
	char *new_dst = (char *)imageRec->pix;

	int rowSize = f->GetRowSize();
	int pitch = f->GetPitch();
	int dstWidthBytes = imageRec->dstWidth * 4;
	char paddingBytes = (imageRec->rowbytes) - (dst_width * 4);

	unsigned int h = imageRec->dstHeight;
		
	do {
		memcpy(new_dst, new_src, rowSize);

		new_src = new_src + pitch;
		new_dst = new_dst + dstWidthBytes + paddingBytes;
			
	} while(--h);
	  
	return imNoErr;

}

//If working avisynth is not available just provide a blank screen
void OutputBlackScreen(imImportImageRec *imageRec) {

	char paddingBytes = (imageRec->rowbytes) - (imageRec->dstWidth * 4);

	char *new_dst = (char *)imageRec->pix;
	int dstWidthBytes = imageRec->dstWidth * 4;
	unsigned int h = imageRec->dstHeight;
	
	do {
		memset(new_dst, 0, dstWidthBytes);
		new_dst = new_dst + dstWidthBytes + paddingBytes;
		
	} while(--h);
}
