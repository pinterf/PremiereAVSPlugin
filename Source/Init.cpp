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

#include "Init.h"

#include "im-avisynth.h"

int Init(imStdParms *stdparms, imImportInfoRec *importInfo) {
  
	importInfo->canOpen = true;
	importInfo->canSave = false;
	importInfo->canDelete = false;
	importInfo->canResize = false;
	importInfo->canDoSubsize = false;
	importInfo->canDoContinuousTime = false;
	importInfo->noFile = false;
	importInfo->addToMenu = imMenuNew;
	importInfo->hasSetup = false; //can't get this to give us imGetPrefs even if it's true
	importInfo->dontCache = false;
	importInfo->setupOnDblClk = false;
	importInfo->keepLoaded = false;
	importInfo->priority = 0;

	//Check whether to conform the audio

	HWND wnd = (HWND) stdparms->piSuites->windFuncs->getMainWnd();

	int audioConform;
	RetreiveRegistryInitData(wnd, &audioConform);

	if(audioConform)
		importInfo->avoidAudioConform = false;
	else
		importInfo->avoidAudioConform = true;

	return imNoErr;
}

// Read from the registry keys that define the ImInit behaviour
int RetreiveRegistryInitData(HWND wnd, int *audioConform) {

	char regkey[] = "SOFTWARE\\Premiere AVS Plugin";
	HKEY regroot;
	LONG regerror = RegOpenKeyEx(HKEY_LOCAL_MACHINE, regkey, 0, KEY_QUERY_VALUE, &regroot);

	if(regerror != ERROR_SUCCESS) {
		MessageBox(wnd, "Configuration GUI not run.\nDefault settings will be written.", "Premiere AVS plugin", MB_OK | MB_ICONINFORMATION);
		
		//Create key
		HKEY newKey;
		regerror = RegCreateKeyEx(HKEY_LOCAL_MACHINE, regkey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &newKey, NULL);
		
		//(Fail silently on error)
		if(regerror == ERROR_SUCCESS) {
			char valueData[2] = "1";
			regerror = RegSetValueEx(newKey, "Bilinear", 0, REG_SZ, (BYTE *)&valueData, 2);
			regerror = RegSetValueEx(newKey, "AudioConform", 0, REG_SZ, (BYTE *)&valueData, 2);
		}

		*audioConform = 1;

		return 0;
	}

	char keyvalue[80];
	DWORD buflen = 80;
	int keypos;

	//Query AudioConform

	regerror = RegQueryValueEx(regroot, "AudioConform", NULL, NULL, (LPBYTE) keyvalue, &buflen);

	//If this key has never been set (coming from an older version of the plugin)
	if(regerror != ERROR_SUCCESS) {
		MessageBox(wnd, "Please run configuration GUI to set if audio conforming should take place. Setting default on.", "Premiere AVS plugin", MB_OK | MB_ICONINFORMATION);
		
		//Create key
		HKEY newKey;
		regerror = RegCreateKeyEx(HKEY_LOCAL_MACHINE, regkey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &newKey, NULL);
		
		//(Fail silently on error)
		if(regerror == ERROR_SUCCESS) {
			char valueData[2] = "1";
			regerror = RegSetValueEx(newKey, "AudioConform", 0, REG_SZ, (BYTE *)&valueData, 2);
		}

		*audioConform = 1;

		return 0;
	}

	//Ensure NULL termination
	keyvalue[buflen - 1] = '\0';
	keypos = atoi(keyvalue);

	if(keypos)
		*audioConform = 1;
	else
		*audioConform = 0;
		
	return 0;
}
