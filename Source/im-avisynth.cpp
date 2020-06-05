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
#include <new>

#include "im-avisynth.h"

#include  "PrSDKimport.h"

#include "resource.h"

#include "WordWrap.h"
#include "ErrorHandling.h"
#include "GetInfo.h"
#include "ImportImage.h"
#include "ImportAudio.h"
#include "Init.h"
#include "FileHandling.h"

int ImGetInfo8(imStdParms *stdparms, imFileAccessRec8 *fileInfo, imFileInfoRec8 *imFileInfo) {

	return GetInfo8(stdparms, fileInfo, imFileInfo);
}

int ImGetInfo7(imStdParms *stdparms, imFileAccessRec *fileInfo, imFileInfoRec7 *imFileInfo) {

	return GetInfo7(stdparms, fileInfo, imFileInfo);
}

int ImGetInfo(imStdParms *stdparms, imFileAccessRec *fileInfo, imFileInfoRec *imFileInfo) {

	return GetInfo(stdparms, fileInfo, imFileInfo);
}

int ImImportImage(imStdParms *stdparms, imFileRef ref, imImportImageRec *imageRec) {

	return ImportImage(stdparms, ref, imageRec);
}
	
int ImImportAudio7(imStdParms *stdParms, imFileRef ref, imImportAudioRec7 *audioRec) {
	
	return ImportAudio7(stdParms, ref, audioRec);
}

int ImImportAudio(imStdParms *stdparms, imFileRef ref, imImportAudioRec *audioRec) {

	return ImportAudio(stdparms, ref, audioRec);
}

int ImCloseFile(imStdParms *stdparms, imFileRef *file, FileInfo *privatedata) {
	
	return CloseFile(stdparms, file, privatedata);
}

int ImQuietFile(imStdParms *stdparms, imFileRef *file, FileInfo *privatedata) {
	
	return QuietFile(stdparms, file, privatedata);
}

int ImOpenFile(imStdParms *stdparms, imFileRef *file, imFileOpenRec *fileOpenInfo) {

	return OpenFile(stdparms, file, fileOpenInfo);
}

int ImOpenFile8(imStdParms *stdparms, imFileRef *file, imFileOpenRec8 *fileOpenInfo) {

	return OpenFile8(stdparms, file, fileOpenInfo);
}

int ImInit(imStdParms *stdparms, imImportInfoRec *importInfo) {
	
	return Init(stdparms, importInfo);
}

int ImGetIndFormat(imStdParms *stdparms, int index, imIndFormatRec *ifrec) {

	return GetIndFormat(stdparms, index, ifrec);
}

PREMPLUGENTRY xImportEntry(int selector, imStdParms *stdparms, long param1, long param2) {

	//char msg[80];
	//sprintf(msg, "sel: %i", selector);
	//MessageBox((HWND) stdparms->piSuites->windFuncs->getMainWnd(), msg, "Premiere AVS plugin", MB_OK | MB_ICONINFORMATION);	

	switch (selector) {

	//Premiere 8 (PPro 2) selectors
	case imGetSupports8:
		return malSupports8;
	case imGetInfo8:
		return ImGetInfo8(stdparms, (imFileAccessRec8 *)param1, (imFileInfoRec8 *)param2);
	case imOpenFile8:
		return ImOpenFile8(stdparms, (imFileRef *)param1, (imFileOpenRec8 *)param2);

	//Premiere 7 (PPro 1) selectors

	case imGetSupports7:
		return malSupports7;
	case imGetInfo7:
		return ImGetInfo7(stdparms, (imFileAccessRec *)param1, (imFileInfoRec7 *)param2);
	case imImportAudio7:	
		return ImImportAudio7(stdparms, (imFileRef)param1, (imImportAudioRec7 *)param2);

	//Original (Premiere 5,6) selectors
	
	case imInit:
		return ImInit(stdparms, (imImportInfoRec *)param1);
	case imImportImage:
		return ImImportImage(stdparms, (imFileRef)param1, (imImportImageRec *)param2);
	case imImportAudio:
		return ImImportAudio(stdparms, (imFileRef)param1, (imImportAudioRec *)param2);
	case imGetInfo:
		return ImGetInfo(stdparms, (imFileAccessRec *)param1, (imFileInfoRec *)param2);
	case imGetIndFormat:
		return ImGetIndFormat(stdparms, param1, (imIndFormatRec *)param2);
	case imOpenFile:
		return ImOpenFile(stdparms, (imFileRef *)param1, (imFileOpenRec *)param2);
	case imQuietFile:
		return ImQuietFile(stdparms, (imFileRef *)param1, (FileInfo *)param2); 
	case imCloseFile:
		return ImCloseFile(stdparms, (imFileRef *)param1, (FileInfo *)param2);
	
		
	default:

		return imUnsupported;
	}
}
