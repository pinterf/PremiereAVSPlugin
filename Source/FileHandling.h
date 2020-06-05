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

#ifndef _filehandling_h
#define _filehandling_h

#include "PrSDKimport.h"

#include "im-avisynth.h"

int CloseFile(imStdParms *stdparms, imFileRef *fileRef, FileInfo *privateData);
int QuietFile(imStdParms *stdparms, imFileRef *file, FileInfo *privatedata);
int OpenFile(imStdParms *stdparms, imFileRef *file, imFileOpenRec *fileOpenInfo);
int OpenFile8(imStdParms *stdparms, imFileRef *file, imFileOpenRec8 *fileOpenInfo);

#endif