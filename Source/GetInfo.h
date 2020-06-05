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

#ifndef _getinfo_h
#define _getinfo_h

#include "im-avisynth.h"

#include  "PrSDKimport.h"

enum ResizeFilter {
    Point = 0,
    Bilinear = 1,
    Bicubic = 2,
    Lanczos = 3
};

int GetIndFormat(imStdParms *stdparms, int index, imIndFormatRec *ifrec);

int GetInfo(imStdParms *stdparms, imFileAccessRec *fileInfo, imFileInfoRec *imFileInfo);
int GetInfo7(imStdParms *stdparms, imFileAccessRec *fileInfo, imFileInfoRec7 *imFileInfo);
int GetInfo8(imStdParms *stdparms, imFileAccessRec8 *fileInfo, imFileInfoRec8 *imFileInfo8);
ResizeFilter RetreiveRegistryAlgorithm(HWND wnd);
void AcquirePremiereAudioSuite(imStdParms *stdparms, FileInfo *pfi);

#endif
