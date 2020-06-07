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
#include <new>

#include "im-avisynth.h"

#include "GetInfo.h"
#include "ErrorHandling.h"

#include <stdio.h> //debug

int GetIndFormat(imStdParms *stdparms, int index, imIndFormatRec *ifrec) {
	
	if (index != 0)
		return imBadFormatIndex;

	ifrec->filetype = ' SVA';
	ifrec->flags = xfCanOpen | xfCanImport | xfCanReplace | xfIsMovie;
	lstrcpy(ifrec->FormatName, "Avisynth file");
	lstrcpy(ifrec->FormatShortName, "Avisynth file");
	memcpy(ifrec->PlatformExtension, "AVS\0VDR\0", 9);
	
	return imNoErr;
}

int GetInfo8(imStdParms *stdparms, imFileAccessRec8 *fileInfo, imFileInfoRec8 *imFileInfo8) {

	//Use GetInfo7 to fill the common flags

	//Set up a P6 FileInfoRec struct

	imFileInfoRec p6FileInfo;
	
	p6FileInfo.prefs = imFileInfo8->prefs;
	p6FileInfo.privatedata = imFileInfo8->privatedata;

	//Set up a P6 imFileAccessRec

	//Need to convert the P8 UTF path into ASCII for avisynth
	//For some reason the output of wcstombs has 4 bad chars on the front and some on the end.
	//These are clipped in the copy and null terminate

	char filepathASCII[256];
	int stringLength = wcslen(reinterpret_cast<const wchar_t*>(fileInfo->filepath));
	wcstombs(reinterpret_cast<char*>(filepathASCII), fileInfo->filepath, stringLength);

	//Null terminate the ascii string at the end
	filepathASCII[stringLength] = '\0';
	
	imFileAccessRec p6FileAccessRec;
	
	strncpy(p6FileAccessRec.filespec.name, filepathASCII + 4, 255);

	int p6ReturnCode = GetInfo(stdparms, &p6FileAccessRec, &p6FileInfo);

	//If it failed, return failure

	if(p6ReturnCode == imBadFile)
		return imBadFile;

	//Copy relevant P6 data to imFileInfo7

	//Video

	imFileInfo8->hasVideo =							p6FileInfo.hasVideo;
	imFileInfo8->vidDuration =						p6FileInfo.vidDuration;
	imFileInfo8->vidScale =							p6FileInfo.vidScale;
	imFileInfo8->vidSampleSize =					p6FileInfo.vidSampleSize;
	
	imFileInfo8->vidInfo.imageWidth =				p6FileInfo.vidInfo.imageWidth;
	imFileInfo8->vidInfo.imageHeight =				p6FileInfo.vidInfo.imageHeight;
	imFileInfo8->vidInfo.depth =					p6FileInfo.vidInfo.depth;
	imFileInfo8->vidInfo.subType =					p6FileInfo.vidInfo.subType;
	imFileInfo8->vidInfo.fieldType =				p6FileInfo.vidInfo.fieldType;
	imFileInfo8->vidInfo.fieldsStacked =			p6FileInfo.vidInfo.fieldsStacked;
	imFileInfo8->vidInfo.hasPulldown =				p6FileInfo.vidInfo.hasPulldown;
	imFileInfo8->vidInfo.pulldownCadence =			p6FileInfo.vidInfo.pulldownCadence;
	imFileInfo8->vidInfo.alphaType =				p6FileInfo.vidInfo.alphaType;
	imFileInfo8->vidInfo.alphaInverted =			p6FileInfo.vidInfo.alphaInverted;
	imFileInfo8->vidInfo.isVectors =				p6FileInfo.vidInfo.isVectors;
	imFileInfo8->vidInfo.isStill =					p6FileInfo.vidInfo.isStill;
	imFileInfo8->vidInfo.noDuration =				p6FileInfo.vidInfo.noDuration;
	imFileInfo8->vidInfo.pixelAspectNum =			p6FileInfo.vidInfo.pixelAspectNum;
	imFileInfo8->vidInfo.pixelAspectDen =			p6FileInfo.vidInfo.pixelAspectDen;
	
	//New flags in the 7 interface

	imFileInfo8->accessModes =						kSeparateSequentialAudio;
	imFileInfo8->vidInfo.isRollCrawl	=			kPrFalse;
	imFileInfo8->vidInfo.supportsAsyncIO =			kPrFalse;
	imFileInfo8->vidInfo.supportsGetSourceVideo	=	kPrFalse;

	//New flags in the 8 interface

	imFileInfo8->alwaysUnquiet =					kPrFalse;
	//imFileInfo8->highMemUsage =						kPrFalse;

	//Audio

	imFileInfo8->hasAudio =							p6FileInfo.hasAudio;
	imFileInfo8->audDuration =						p6FileInfo.audDuration;
	
	//New flags in the 7 interface

	imFileInfo8->audInfo.numChannels =				((FileInfo *)(p6FileInfo.privatedata))->numChannels;
	imFileInfo8->audInfo.sampleRate =				((FileInfo *)(p6FileInfo.privatedata))->audioSampleRate;
	imFileInfo8->audInfo.sampleType =				((FileInfo *)(p6FileInfo.privatedata))->sampleType; //only for UI

	//Others

	imFileInfo8->prefs =							p6FileInfo.prefs;
	imFileInfo8->privatedata =						p6FileInfo.privatedata;

	//Acquire the Premiere audio suite for P7 conversions
	AcquirePremiereAudioSuite(stdparms, (FileInfo *)(imFileInfo8->privatedata));

	return imNoErr;
}


//The P7 version of GetInfo
//Use what we can from the legacy GetInfo

int GetInfo7(imStdParms *stdparms, imFileAccessRec *fileInfo, imFileInfoRec7 *imFileInfo7) {

	//Use the legacy selector to do the real work

	//Set up a P6 FileInfoRec struct

	imFileInfoRec p6FileInfo;
	
	p6FileInfo.prefs = imFileInfo7->prefs;
	p6FileInfo.privatedata = imFileInfo7->privatedata;

	int p6ReturnCode = GetInfo(stdparms, fileInfo, &p6FileInfo);
	
	//If it failed, return failure

	if(p6ReturnCode == imBadFile)
		return imBadFile;

	//Copy relevant P6 data to imFileInfo7

	//Video

	imFileInfo7->hasVideo =							p6FileInfo.hasVideo;
	imFileInfo7->vidDuration =						p6FileInfo.vidDuration;
	imFileInfo7->vidScale =							p6FileInfo.vidScale;
	imFileInfo7->vidSampleSize =					p6FileInfo.vidSampleSize;
	
	imFileInfo7->vidInfo.imageWidth =				p6FileInfo.vidInfo.imageWidth;
	imFileInfo7->vidInfo.imageHeight =				p6FileInfo.vidInfo.imageHeight;
	imFileInfo7->vidInfo.depth =					p6FileInfo.vidInfo.depth;
	imFileInfo7->vidInfo.subType =					p6FileInfo.vidInfo.subType;
	imFileInfo7->vidInfo.fieldType =				p6FileInfo.vidInfo.fieldType;
	imFileInfo7->vidInfo.fieldsStacked =			p6FileInfo.vidInfo.fieldsStacked;
	imFileInfo7->vidInfo.hasPulldown =				p6FileInfo.vidInfo.hasPulldown;
	imFileInfo7->vidInfo.pulldownCadence =			p6FileInfo.vidInfo.pulldownCadence;
	imFileInfo7->vidInfo.alphaType =				p6FileInfo.vidInfo.alphaType;
	imFileInfo7->vidInfo.alphaInverted =			p6FileInfo.vidInfo.alphaInverted;
	imFileInfo7->vidInfo.isVectors =				p6FileInfo.vidInfo.isVectors;
	imFileInfo7->vidInfo.isStill =					p6FileInfo.vidInfo.isStill;
	imFileInfo7->vidInfo.noDuration =				p6FileInfo.vidInfo.noDuration;
	imFileInfo7->vidInfo.pixelAspectNum =			p6FileInfo.vidInfo.pixelAspectNum;
	imFileInfo7->vidInfo.pixelAspectDen =			p6FileInfo.vidInfo.pixelAspectDen;
	
	//New flags in the 7 interface

	imFileInfo7->accessModes =						kSeparateSequentialAudio;
	imFileInfo7->vidInfo.isRollCrawl	=			kPrFalse;
	imFileInfo7->vidInfo.supportsAsyncIO =			kPrFalse;
	imFileInfo7->vidInfo.supportsGetSourceVideo	=	kPrFalse;

	//Audio

	imFileInfo7->hasAudio =							p6FileInfo.hasAudio;
	imFileInfo7->audDuration =						p6FileInfo.audDuration;
	
	//New flags in the 7 interface

	imFileInfo7->audInfo.numChannels =				((FileInfo *)(p6FileInfo.privatedata))->numChannels;
	imFileInfo7->audInfo.sampleRate =				((FileInfo *)(p6FileInfo.privatedata))->audioSampleRate;
	imFileInfo7->audInfo.sampleType =				((FileInfo *)(p6FileInfo.privatedata))->sampleType; //only for UI

	//Others

	imFileInfo7->prefs =							p6FileInfo.prefs;
	imFileInfo7->privatedata =						p6FileInfo.privatedata;

	//Acquire the Premiere audio suite for P7 conversions
	AcquirePremiereAudioSuite(stdparms, (FileInfo *)(imFileInfo7->privatedata));

	return imNoErr;
}

//Init avisynth and popular the FileInfoRec

int GetInfo(imStdParms* stdparms, imFileAccessRec* fileInfo, imFileInfoRec* imFileInfo) {

	//Use Premier memory handling for FileInfo saved data struct
	FileInfo *fi = (FileInfo *)stdparms->piSuites->memFuncs->newPtr(sizeof(FileInfo));
	new (fi) FileInfo();

	//Use blank/error screens rather than sending BadFile to Premiere
	bool blankScreenError = true;

	//Initialise FileInfo

	fi->errorCode = 0;
	fi->audioPosition = 0;
	fi->scriptEnvironment = 0;
	fi->suiteBasic = 0;
	fi->suiteAudio = 0;
	fi->audioConversionBufferSize = 0;

	//Get HWND
	HWND wnd = (HWND) stdparms->piSuites->windFuncs->getMainWnd();

	//Get version number
	int premiereVersion = stdparms->piSuites->piInterfaceVer;

	//Init Avisynth
	fi->avisynthDll = LoadLibrary("avisynth.dll");
	if(!fi->avisynthDll) {

		MessageBox(wnd, "Failed to load avisynth.dll. Please reinstall Avisynth.", "Premiere AVS plugin Error", MB_OK | MB_ICONERROR);
		fi->errorCode = 1;
		if(!blankScreenError)
			return imBadFile;
	}

	//Create script environment
	IScriptEnvironment* (__stdcall *CreateScriptEnvironment)(int version);
	
	if(!fi->errorCode) {
		CreateScriptEnvironment = (IScriptEnvironment* (__stdcall *)(int)) GetProcAddress(fi->avisynthDll, "CreateScriptEnvironment");
		if(!CreateScriptEnvironment) 
			{ 
				MessageBox(wnd, "Can't find CreateScriptEnvironment. Please reinstall Avisynth.", "Premiere AVS plugin Error", MB_OK | MB_ICONERROR);
				fi->errorCode = 1;
				if(!blankScreenError)
					return imBadFile;
			}
	}

	if(!fi->errorCode) {
		
		try {
			fi->scriptEnvironment = CreateScriptEnvironment(AVISYNTH_CLASSIC_INTERFACE_VERSION); // V6 is enough
		}
		catch(AvisynthError) {
			
			//The exception message is non-zero terminated and cannot be strcated
			fi->errorCode = 1;
		}

		if (fi->errorCode || fi->scriptEnvironment == NULL) 
			{
				//Old avisynth version or avisynth config error
				MessageBox(wnd, "Avisynth configuration error or old Avisynth version.\nPlease do a fresh install of a recent version of Avisynth.", "Premiere AVS plugin Error", MB_OK | MB_ICONERROR);
				fi->errorCode = 1;
				if(!blankScreenError)
					return imBadFile;
			}

	}

  AVS_linkage = fi->scriptEnvironment->GetAVSLinkage(); // AviSynth Interface V6+

	AVSValue res;
	const char *errorMsg;

	if(!fi->errorCode) {

		//Import the clip, if Avisynth is working so far
		
		const char *param = fileInfo->filespec.name;

		AVSValue arg(param);

		try {
			res = fi->scriptEnvironment->Invoke("Import", AVSValue(&arg, 1));
		}
		catch(IScriptEnvironment::NotFound) {
			
			HandleNotFoundError(wnd);
			errorMsg = "Import function not found";
			if(!blankScreenError)
				return imBadFile;
			fi->errorCode = 2;
		}
		catch(AvisynthError e) {
			
			HandleAvisynthError(wnd, e);
			errorMsg = e.msg;
			if(!blankScreenError)
				return imBadFile;
			fi->errorCode = 2;
		}
	}

	//Setup premiere flags

	//Default values

	imFileInfo->hasVideo = false;
	imFileInfo->hasAudio = false;
	imFileInfo->vidDuration = 0;
	imFileInfo->vidInfo.imageWidth = 0;
	imFileInfo->vidInfo.imageHeight = 0;

	//Video info

	imFileInfo->vidInfo.pixelAspectNum = 1;
	imFileInfo->vidInfo.pixelAspectDen = 1;
	imFileInfo->vidInfo.depth = 32; //required by Premiere
	imFileInfo->vidInfo.subType = imUncompressed;
		
	imFileInfo->canDraw = false; //don't handle drawing ourselves
	imFileInfo->prefs = 0; //no pref dialog
				
	//Time info

	imFileInfo->vidInfo.noDuration = false;
//	imFileInfo->vidInfo.isFramestore = false; //deprecated?
	imFileInfo->hasDataRate = false; //no data rate info

	//Format info

	imFileInfo->vidInfo.alphaInverted = false;

	//Draw info

	imFileInfo->vidInfo.fieldType = prFieldsNone; //prFieldsNone prFieldsUpperFirst prFieldsLowerFirst prFieldsUnknown
	imFileInfo->vidInfo.fieldsStacked = false; //true = Fields are present, and not interlaced.
	imFileInfo->vidInfo.hasPulldown = false; //Set this to true if the clip contains NTSC film footage with 3:2 pulldown.
	imFileInfo->vidInfo.pulldownCadence = importer_PulldownPhase_NO_PULLDOWN; //in PP 2 = pulldownCadence
	imFileInfo->vidInfo.isVectors = false; //Vector format, no resolution
	imFileInfo->vidInfo.isStill = false; //Only 1 frame in video
	//imFileInfo->vidInfo.drawsExternal = false; //Draw on external device only
	//imFileInfo->vidInfo.dontObscure = false; 

	// Read from the registry what scaling algorithm to use and save in FileInfo

	ResizeFilter fmode = RetreiveRegistryAlgorithm(wnd);

	switch(fmode) {
	case Bicubic:
		strncpy(fi->avisynthResizeFilter, "BicubicResize", 80);
		break;
	case Bilinear:
		strncpy(fi->avisynthResizeFilter, "BilinearResize", 80);
		break;
	case Lanczos:
		strncpy(fi->avisynthResizeFilter, "LanczosResize", 80);
		break;
	case Point:
		strncpy(fi->avisynthResizeFilter, "PointResize", 80);
		break;
	default:
		//Not strictly necessary, but I don't trust strings
		strncpy(fi->avisynthResizeFilter, "BilinearResize", 80);
	}

	//Set file-specific information

	//If we failed to open the real file give fake information for fps, width, height and audio
	if(fi->errorCode) {
			
		imFileInfo->hasVideo = true;
		imFileInfo->vidInfo.imageWidth = 640;
		imFileInfo->vidInfo.imageHeight = 480;
		imFileInfo->vidDuration = BLANK_CLIP_LENGTH_FRAMES;
		imFileInfo->vidScale = 24;
		imFileInfo->vidSampleSize = 1;

		//Ensure no alpha channel for the error code, for Premiere Pro 2
		imFileInfo->vidInfo.alphaType = alphaNone;

		//This must be set to true (at least if the project files have audio - untested). Otherwise loading a project will crash
		//Works even with projects that include [broken] clips without audio
		imFileInfo->hasAudio = true;
		imFileInfo->audScale = 22050;
		imFileInfo->audSampleSize = 1;
		imFileInfo->audDuration = 22050 / 24 * BLANK_CLIP_LENGTH_FRAMES;
		imFileInfo->audInfo.audStereo = 2;
		imFileInfo->audInfo.aud16 = 0;
		imFileInfo->audInfo.twosComp = 0;
		imFileInfo->audInfo.subType = imUncompressed;
			
	}

	//If import was a success extract info from media file

	else {

		PClip clip = res.AsClip();

		//Save the result
		fi->procScript = res;

		//Get video info
		VideoInfo inf  = clip->GetVideoInfo();

		//Video
		if (inf.HasVideo())	   {
		
			//Set premiere video parameters

			imFileInfo->hasVideo = true;
			
			//Check sourcce film type and set Premiere fps rates accordingly

			double fps = inf.fps_numerator / (double)inf.fps_denominator;

			//Look for NTSC or FILM from avisynth and set Premiere up accordingly
			//Avisynth currently passes 30000/1001 and 24000/1001 as numerator/denominator
			//which is suitable for Premiere. However, the user may pass more approximate
			//values so looking for ranges

			//NTSC

			if(fps > 29.96 && fps < 29.98) {
				//30000 / 1001 = 29.9700299700...
				imFileInfo->vidScale = 30000;
				imFileInfo->vidSampleSize = 1001;
				imFileInfo->vidDuration = inf.num_frames * 1001;
			}
		
			//FILM

			else if(fps > 23.96 && fps < 23.98) {
				//24000 / 1001 = 23.9760239760...
				imFileInfo->vidScale = 24000;
				imFileInfo->vidSampleSize = 1001;
				imFileInfo->vidDuration = inf.num_frames * 1001;
			}

			//all others

			else {
				imFileInfo->vidScale = inf.fps_numerator;
				imFileInfo->vidSampleSize = inf.fps_denominator;
				imFileInfo->vidDuration = inf.num_frames * inf.fps_denominator;
			}

			//Frame size

			imFileInfo->vidInfo.imageWidth = inf.width;
			imFileInfo->vidInfo.imageHeight = inf.height;
				
			//Check colour space and depth
			//If we're not 32-bit RGB, get AviSynth to do this conversion

			if (!inf.IsRGB32())	 {

				try {
					AVSValue colRes;
					colRes = fi->scriptEnvironment->Invoke("ConvertToRGB32", AVSValue(&res, 1));
					PClip clip = colRes.AsClip();
				
					VideoInfo infh = clip->GetVideoInfo();

					if(!infh.IsRGB32()) {
						MessageBox(wnd, "Failed to convert to RGB32 colour space.", "Premiere AVS plugin Error", MB_OK | MB_ICONINFORMATION);
						errorMsg = "Failed to convert to RGB32";
						fi->errorCode = 2;
						if(!blankScreenError)
							return imBadFile;
					}

					//Update the finished script
					fi->procScript = colRes;
				}
				catch(IScriptEnvironment::NotFound) {
			
					HandleNotFoundError(wnd);
					MessageBox(wnd, "Failed to convert to RGB32 colour space.", "Premiere AVS plugin Error", MB_OK | MB_ICONINFORMATION);
					if(!blankScreenError)
						return imBadFile;
					errorMsg = "Failed to convert to RGB32";
					fi->errorCode = 2;
				}
				catch(AvisynthError e) {
					HandleAvisynthError(wnd, e);
					MessageBox(wnd, "Failed to convert to RGB32 colour space.", "Premiere AVS plugin Error", MB_OK | MB_ICONINFORMATION);
					fi->errorCode = 2;
					errorMsg = "Failed to convert to RGB32";
					if(!blankScreenError)
						return imBadFile;
				}
			}
		}

		//Handle audio

		imFileInfo->hasAudio = false;

		if(inf.HasAudio()) {
		
			imFileInfo->hasAudio = true;

			//For premiere versions prior to Premiere Pro, 16-bit audio is the longest that can be past,
			//so we convert any longer types to 16bit

			if(premiereVersion == PR_PISUITES_VERSION_1 ||
				premiereVersion == PR_PISUITES_VERSION_2 ||
				premiereVersion == PR_PISUITES_VERSION_3) {

				//8 bit sound
				if(inf.BytesPerChannelSample() == 1) {
					imFileInfo->audInfo.aud16 = 0; //8bit
				}
				//16 bit sound
				else if(inf.BytesPerChannelSample() == 2) {
					imFileInfo->audInfo.aud16 = 1; //16bit
				}
	
				//If higher (24 bit etc), use Avisynth to convert sound to 16 bit
				else {
					//Set to 8-bit for the failure case
					imFileInfo->audInfo.aud16 = 0;

					try {
						AVSValue audioRes = fi->scriptEnvironment->Invoke("ConvertAudioTo16bit", AVSValue(&fi->procScript, 1));
						fi->procScript = audioRes;
						clip = audioRes.AsClip();
						
						//Get new info about 16bit sound
						inf = clip->GetVideoInfo();
						imFileInfo->audInfo.aud16 = 1; //16bit
					}
					catch(IScriptEnvironment::NotFound) {
				
						HandleNotFoundError(wnd);
						MessageBox(wnd, "Failed to convert audio to 16 bit.\nNo audio will play.", "Premiere AVS plugin Error", MB_OK | MB_ICONINFORMATION);
						fi->errorCode = 4;
					}
					catch(AvisynthError e) {
						HandleAvisynthError(wnd, e);
						MessageBox(wnd, "Failed to convert audio to 16 bit.\nNo audio will play", "Premiere AVS plugin Error", MB_OK | MB_ICONINFORMATION);
						fi->errorCode = 4;
						if(!blankScreenError)
							return imBadFile;
					}
				}
			}

			//For Premiere Pro and later, pass up to 32bit audio

			else {

				//8 bit sound
				if(inf.BytesPerChannelSample() == 1) {
					fi->sampleType = kPrAudioSampleType_8BitInt;
				}
				//16 bit sound
				else if(inf.BytesPerChannelSample() == 2) {
					fi->sampleType = kPrAudioSampleType_16BitInt;
				}
				//24 bit sound
				else if(inf.BytesPerChannelSample() == 3) {
					fi->sampleType = kPrAudioSampleType_24BitInt;
				}
				//32 bit sound
				else if(inf.BytesPerChannelSample() == 4) {
					fi->sampleType = kPrAudioSampleType_32BitInt;
				}
				//If higher, use Avisynth to convert sound to 16 bit
				else {

					try {
						AVSValue audioRes = fi->scriptEnvironment->Invoke("ConvertAudioTo16bit", AVSValue(&fi->procScript, 1));
						fi->procScript = audioRes;
						clip = audioRes.AsClip();
						
						//Get new info about 16bit sound
						inf = clip->GetVideoInfo();
						fi->sampleType = kPrAudioSampleType_16BitInt;
					}
					catch(IScriptEnvironment::NotFound) {
				
						HandleNotFoundError(wnd);
						MessageBox(wnd, "Failed to convert audio to 16 bit.\nNo audio will play.", "Premiere AVS plugin Error", MB_OK | MB_ICONINFORMATION);
						fi->errorCode = 4;
					}
					catch(AvisynthError e) {
						HandleAvisynthError(wnd, e);
						MessageBox(wnd, "Failed to convert audio to 16 bit.\nNo audio will play", "Premiere AVS plugin Error", MB_OK | MB_ICONINFORMATION);
						fi->errorCode = 4;
						if(!blankScreenError)
							return imBadFile;
					}
				}
			}

			//Maybe check for how many bytes per channel here rather than just failing silently in imImportAudio7

			imFileInfo->audScale = inf.SamplesPerSecond();
			imFileInfo->audSampleSize = 1;
			imFileInfo->audDuration = (long)inf.num_audio_samples;

			imFileInfo->audInfo.audStereo = inf.AudioChannels();
			imFileInfo->audInfo.twosComp = 0;
			imFileInfo->audInfo.subType = imUncompressed;

			//Store audio parameters needed for P7 and P8 interfaces in privateData

			fi->audioBytesPerSample = inf.BytesPerChannelSample();
			fi->audioSampleRate = (float)inf.SamplesPerSecond();
			fi->numChannels = inf.AudioChannels();
			fi->audioNoOfSamples = inf.num_audio_samples;
			
		}
	}

	//Handle recoverable video errors
	
	//For a recoverable error, attempt to put up an Avisynth error display in the frame
	if(fi->errorCode == 2) {

		//Format the error message into a script
		
		char scriptBuffer[8192];
		MakeAVSErrorString(wnd, scriptBuffer, errorMsg);

		//Try to run the error script
		
		AVSValue arg = AVSValue(scriptBuffer);

		try {
			res = fi->scriptEnvironment->Invoke("Eval", AVSValue(&arg, 1));

			//Save the result
			PClip clip = res.AsClip();
			fi->procScript = res;
		}
		catch(IScriptEnvironment::NotFound) {
		
			HandleNotFoundError(wnd);
			fi->errorCode = 1;
		}
		catch(AvisynthError e) {
	   
			HandleErrorClipAvisynthError(wnd, e);
			fi->errorCode = 1;
		}
	}

	//Set last frame width and height to a null value
	fi->lastWidth = 0;
	fi->lastHeight = 0;

	//Store the private info
	imFileInfo->privatedata = (long)fi;
	
	return imNoErr;
}

// Read from the registry what scaling algorithm to use and return
ResizeFilter RetreiveRegistryAlgorithm(HWND wnd) {

	char regkey[] = "SOFTWARE\\Premiere AVS Plugin";
	HKEY regroot;
	LONG regerror = RegOpenKeyEx(HKEY_LOCAL_MACHINE, regkey, 0, KEY_QUERY_VALUE, &regroot);

	if(regerror != ERROR_SUCCESS) {
		MessageBox(wnd, "Configuration GUI not run.\nResize set to: Bilinear rescaling.", "Premiere AVS plugin", MB_OK | MB_ICONINFORMATION);
		
		//Set bilinear filtering from now on
		HKEY newKey;
		regerror = RegCreateKeyEx(HKEY_LOCAL_MACHINE, regkey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &newKey, NULL);
		
		//(Fail silently on error)
		if(regerror == ERROR_SUCCESS) {
			char valueData[2] = "1";
			regerror = RegSetValueEx(newKey, "Bilinear", 0, REG_SZ, (BYTE *)&valueData, 2);
		}

		return Bilinear;
	}

	char keyvalue[80];
	DWORD buflen = 80;
	int keypos;

	//Query each scaling algorithm

	//Bicubic

	RegQueryValueEx(regroot, "Bicubic", NULL, NULL, (LPBYTE) keyvalue, &buflen);
	//Ensure NULL termination
	keyvalue[buflen - 1] = '\0';
	keypos = atoi(keyvalue);

	if(keypos)
		return Bicubic;

	//Bilinear

	RegQueryValueEx(regroot, "Bilinear", NULL, NULL, (LPBYTE) keyvalue, &buflen);
	keyvalue[buflen - 1] = '\0';
	keypos = atoi(keyvalue);

	if(keypos)
		return Bilinear;

	//Lanczos3

	RegQueryValueEx(regroot, "Lanczos3", NULL, NULL, (LPBYTE) keyvalue, &buflen);
	keyvalue[buflen - 1] = '\0';
	keypos = atoi(keyvalue);

	if(keypos)
		return Lanczos;

	//Nearest Neighbour

	RegQueryValueEx(regroot, "NearestNeighbour", NULL, NULL, (LPBYTE) keyvalue, &buflen);
	keyvalue[buflen - 1] = '\0';
	keypos = atoi(keyvalue);
	
	if(keypos)
		return Point;

	//If the registry is really broken, return bilinear

	MessageBox(wnd, "Configuration error. Please rerun GUI.\nResize set to: Bilinear rescaling.", "Premiere AVS plugin", MB_OK | MB_ICONINFORMATION);
	return Bilinear;
}

void AcquirePremiereAudioSuite(imStdParms *stdParms, FileInfo *pfi) {

	pfi->suiteBasic = stdParms->piSuites->utilFuncs->getSPBasicSuite(); //Allocate an audio suite

	if (pfi->suiteBasic)
	{
		pfi->suiteBasic->AcquireSuite (kPrSDKAudioSuite, kPrSDKAudioSuiteVersion, (const void **)&pfi->suiteAudio);
	}
}

const AVS_Linkage* AVS_linkage = 0; // Since AviSynth V6 interface

