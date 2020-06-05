# Microsoft Developer Studio Project File - Name="im_avisynth" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=im_avisynth - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "im-avisynth.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "im-avisynth.mak" CFG="im_avisynth - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "im_avisynth - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "im_avisynth - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "im_avisynth - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "IM_Release"
# PROP BASE Intermediate_Dir "IM_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "IM_Release"
# PROP Intermediate_Dir "IM_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /Zp1 /MD /W3 /GX /O2 /Ob2 /I "premiere-headers" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "PRWIN_ENV" /FR /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "PRWIN_ENV"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib shell32.lib vfw32.lib msacm32.lib winmm.lib comctl32.lib advapi32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"IM_Release\IM-Avisynth.prm"

!ELSEIF  "$(CFG)" == "im_avisynth - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "IM_Debug"
# PROP BASE Intermediate_Dir "IM_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\IM_Debug"
# PROP Intermediate_Dir ".\IM_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /Zp1 /MDd /W3 /GX /ZI /Od /I "premiere-headers" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "PRWIN_ENV" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /fo".\IM_Debug/im_avisynth.res" /d "_DEBUG" /d "PRWIN_ENV"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib shell32.lib vfw32.lib msacm32.lib winmm.lib comctl32.lib advapi32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"IM_Debug\IM-Avisynth.prm"
# SUBTRACT LINK32 /verbose /incremental:no /map /force

!ENDIF 

# Begin Target

# Name "im_avisynth - Win32 Release"
# Name "im_avisynth - Win32 Debug"
# Begin Group "Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ErrorHandling.cpp
# End Source File
# Begin Source File

SOURCE=.\FileHandling.cpp
# End Source File
# Begin Source File

SOURCE=.\GetInfo.cpp
# End Source File
# Begin Source File

SOURCE=".\im-avisynth.cpp"
# End Source File
# Begin Source File

SOURCE=.\ImportAudio.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportImage.cpp
# End Source File
# Begin Source File

SOURCE=.\Init.cpp
# End Source File
# Begin Source File

SOURCE=.\WordWrap.cpp
# End Source File
# End Group
# Begin Group "Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ErrorHandling.h
# End Source File
# Begin Source File

SOURCE=.\FileHandling.h
# End Source File
# Begin Source File

SOURCE=.\GetInfo.h
# End Source File
# Begin Source File

SOURCE=".\im-avisynth.h"
# End Source File
# Begin Source File

SOURCE=.\ImportAudio.h
# End Source File
# Begin Source File

SOURCE=.\ImportImage.h
# End Source File
# Begin Source File

SOURCE=.\Init.h
# End Source File
# Begin Source File

SOURCE=.\WordWrap.h
# End Source File
# End Group
# Begin Group "Resource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\im-avisynth.def"
# End Source File
# Begin Source File

SOURCE=".\im-avisynth.rc"
# End Source File
# End Group
# End Target
# End Project
