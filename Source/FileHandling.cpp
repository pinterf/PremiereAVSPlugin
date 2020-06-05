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

#include "FileHandling.h"

#include "im-avisynth.h"

//Open the file using the P8 interface (Unicode filename)
int OpenFile8(imStdParms *stdparms, imFileRef *file, imFileOpenRec8 *fileOpenInfo) {

	int	result = imNoErr;
	
	HANDLE fileRef;

	//Open as read/write as requested by Premiere

	if (fileOpenInfo->inReadWrite == kPrOpenFileAccess_ReadOnly)
	{
		fileRef = CreateFileW(fileOpenInfo->fileinfo.filepath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if (fileOpenInfo->inReadWrite == kPrOpenFileAccess_ReadWrite)
	{
		fileRef = CreateFileW(fileOpenInfo->fileinfo.filepath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	
	//If we can't open for some reason, return failure

	if (fileRef == imInvalidHandleValue)
	{
		result = imBadFile;
	}
	else
	{
		*file = fileRef;
		fileOpenInfo->fileinfo.fileref = fileRef;
		fileOpenInfo->fileinfo.filetype = 'AVS_';
	}
	return result;
}

//Open the file (avisynth script)
int OpenFile(imStdParms *stdparms, imFileRef *file, imFileOpenRec *fileOpenInfo) {

	int	result = imNoErr;
	
	HANDLE fileRef;
	
	//Open as read/write as requested by Premiere

	if (fileOpenInfo->inReadWrite == kPrOpenFileAccess_ReadOnly)
	{
		fileRef = CreateFile(fileOpenInfo->fileinfo.filespec.name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if (fileOpenInfo->inReadWrite == kPrOpenFileAccess_ReadWrite)
	{
		fileRef = CreateFile(fileOpenInfo->fileinfo.filespec.name, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	
	//If we can't open for some reason, return failure

	if (fileRef == imInvalidHandleValue)
	{
		result = imBadFile;
	}
	else
	{
		*file = fileRef;
		fileOpenInfo->fileinfo.fileref = fileRef;
		fileOpenInfo->fileinfo.filetype = 'AVS_';
	}
	return result;
}


//Close the file (avisynth script) temporarily
int QuietFile(imStdParms *stdparms, imFileRef *file, FileInfo *privatedata) {

	//Close handle if valid
	if (file && *file)
	{
		CloseHandle (*file);
		*file = imInvalidHandleValue;
	}
	return imNoErr; 
}

//Close the file (avisynth script) permanently
int CloseFile(imStdParms *stdparms, imFileRef *fileRef, FileInfo *privateData) {

	FileInfo *fileptr = privateData;

	//Close file handle
	if(fileRef && *fileRef) {
		CloseHandle (fileRef);
	}

	if (fileptr) {

		if(fileptr->suiteBasic)
			fileptr->suiteBasic->ReleaseSuite(kPrSDKAudioSuite, kPrSDKAudioSuiteVersion);

		delete fileptr->scriptEnvironment;
		fileptr->procScript.~AVSValue(); //release the clip, only once
		//fileptr->clipCache.~PClip(); //for some reason the reference counting seems to be getting confused and will crash if we
		//kill both handles to the clip

		stdparms->piSuites->memFuncs->disposePtr(reinterpret_cast<char*>(fileptr));
	}
 
	return imNoErr;
}