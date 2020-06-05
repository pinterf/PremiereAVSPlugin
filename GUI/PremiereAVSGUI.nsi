# PremiereAVSPlugin GUI
#
# Video Editors' Toolkit Sourceforge Project
# (http://sourceforge.net/projects/videoeditorskit/)
# (c) 2004
#
# Licensed under the GPL - see COPYING.txt

#!packhdr "C:\program files\upx\upx.exe -best temp.dat"
!packhdr "c:\header.dat" "'C:\program files\upx\upx.exe' -best c:\header.dat"
!include "mui.nsh"

SetCompressor lzma
Page custom CustomPage1

!define VERSION "1.95"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "ADheader.bmp"
!define MUI_HEADERIMAGE_RIGHT
!define MUI_ICON "Avisynth.ico"
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"

ReserveFile "PremiereAVSGUI.ini"
InstallButtonText OK
BrandingText "Premiere AVS Plugin and GUI v${VERSION} - licensed under the GPL"

!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

Outfile "Premiere AVS GUI.exe"
showinstdetails nevershow

Function .onInit
        ClearErrors
        readregstr $8 HKEY_LOCAL_MACHINE "SOFTWARE\AviSynth" "plugindir2_5"
        IfErrors noplugindir plugindir
        noplugindir:
        strcpy $R0 "none"
        strcmp $R0 "none" message nomessage
        message:
        Messagebox MB_OK "No default Avisynth Plugin Folder detected - Set Memory Max will be ignored"
        writeregstr HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "Ignore" "1"
        strcpy $R6 "1"
        goto newstart
        nomessage:
        plugindir:
        ClearErrors
        FileOpen $0 "$8\setmemorymax.avsi" "r"
        IfErrors makeavsi nomakeavsi
        makeavsi:
        FileOpen $R1 "$8\setmemorymax.avsi" "w"
        FileWrite "$R1" "SetMemoryMax(64)"
        FileClose $R1
        nomakeavsi:
        ClearErrors
        StrCmp $0 "" opsend
        FileRead "$0" "$9"
        FileClose $0
        #Messagebox MB_OK $9
        strcpy $6 $9 1
        strcmp $6 "#" now14 now13
        now14:
        strcpy $8 $9 5 14
        #strcpy $7 $8 -1
        goto beforenewstart
        now13:
        strcpy $8 $9 5 13
        #strcpy $7 $8 -1

        beforenewstart:

        #check for ) at end of setmemorymax keep checking until all gone
        
        strcpy $9 $8 10 -1
        strcmp $9 ")" removebracket noremovebracket
        removebracket:
        strcpy $7 $8 -1
        strcpy $8 $7
        goto beforenewstart
        noremovebracket:
        strcpy $7 $8
        newstart:
        ClearErrors
        readregstr $0 HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "SetMemoryMax"
        IfErrors writit nowritit
        writit:
        
        ClearErrors
        StrCmp $8 "" opsend nextcmp
        nextcmp:
        writeregstr HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "SetMemoryMax" "$7"
        readregstr $0 HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "SetMemoryMax"
        goto nowritit

        opsend:
        #sets default setmemorymax of 64
        writeregstr HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "SetMemoryMax" "64"
        readregstr $0 HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "SetMemoryMax"
        
        nowritit:
        #use whatever was in the setmemorymax.avsi - ignores previous reg entry
        writeregstr HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "SetMemoryMax" "$7"
        readregstr $0 HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "SetMemoryMax"


        ClearErrors
        readregstr $1 HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "NearestNeighbour"
        IfErrors writit1 nowritit1
        writit1:
        writeregstr HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "NearestNeighbour" "1"
        readregstr $1 HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "NearestNeighbour"
        nowritit1:

        ClearErrors
        readregstr $2 HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "Bilinear"
        IfErrors writit2 nowritit2
        writit2:
        writeregstr HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "Bilinear" "0"
        readregstr $2 HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "Bilinear"
        nowritit2:

        ClearErrors
        readregstr $3 HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "Bicubic"
        IfErrors writit3 nowritit3
        writit3:
        writeregstr HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "Bicubic" "0"
        readregstr $3 HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "Bicubic"
        nowritit3:

        ClearErrors
        readregstr $4 HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "Lanczos3"
        IfErrors writit4 nowritit4
        writit4:
        writeregstr HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "Lanczos3" "0"
        readregstr $4 HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "Lanczos3"
        nowritit4:

        ClearErrors
        readregstr $R8 HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "Ignore"
        IfErrors writit5 nowritit5
        writit5:
        writeregstr HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "Ignore" "0"
        readregstr $R8 HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "Ignore"
        nowritit5:
        !insertmacro MUI_INSTALLOPTIONS_EXTRACT "PremiereAVSGUI.ini"
        
        ClearErrors
        readregstr $R9 HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "AudioConform"
        #Messagebox MB_OK $R9
        IfErrors writit6 nowritit6
        writit6:
        writeregstr HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "AudioConform" "1"
        nowritit6:
        !insertmacro MUI_INSTALLOPTIONS_EXTRACT "PremiereAVSGUI.ini"

FunctionEnd



Function Custompage1

        !insertmacro MUI_HEADER_TEXT "For use with the Premiere AVS Plugin and Avisynth 2.5+" "This GUI allows you to change settings that will alter the operation of AVS files in Adobe Premiere."

        #Uses registry settings to configure GUI on opening

        !insertmacro MUI_INSTALLOPTIONS_WRITE  "PremiereAVSGUI.ini" "Field 2" "State" $0
        !insertmacro MUI_INSTALLOPTIONS_WRITE  "PremiereAVSGUI.ini" "Field 10" "State" 1
        !insertmacro MUI_INSTALLOPTIONS_WRITE  "PremiereAVSGUI.ini" "Field 6" "State" $1
        !insertmacro MUI_INSTALLOPTIONS_WRITE  "PremiereAVSGUI.ini" "Field 5" "State" $2
        !insertmacro MUI_INSTALLOPTIONS_WRITE  "PremiereAVSGUI.ini" "Field 4" "State" $3
        !insertmacro MUI_INSTALLOPTIONS_WRITE  "PremiereAVSGUI.ini" "Field 11" "State" $4
        !insertmacro MUI_INSTALLOPTIONS_WRITE  "PremiereAVSGUI.ini" "Field 10" "State" $R8
        !insertmacro MUI_INSTALLOPTIONS_WRITE  "PremiereAVSGUI.ini" "Field 12" "State" $R9

        !insertmacro MUI_INSTALLOPTIONS_INITDIALOG "PremiereAVSGUI.ini"
        !insertmacro MUI_INSTALLOPTIONS_SHOW

        #Reads the GUI for new registry settings

        !insertmacro MUI_INSTALLOPTIONS_READ $0 "PremiereAVSGUI.ini" "Field 2" "State"
        !insertmacro MUI_INSTALLOPTIONS_READ $2 "PremiereAVSGUI.ini" "Field 6" "State"
        !insertmacro MUI_INSTALLOPTIONS_READ $3 "PremiereAVSGUI.ini" "Field 5" "State"
        !insertmacro MUI_INSTALLOPTIONS_READ $4 "PremiereAVSGUI.ini" "Field 4" "State"
        !insertmacro MUI_INSTALLOPTIONS_READ $5 "PremiereAVSGUI.ini" "Field 11" "State"
        !insertmacro MUI_INSTALLOPTIONS_READ $R7 "PremiereAVSGUI.ini" "Field 10" "State"
        !insertmacro MUI_INSTALLOPTIONS_READ $R9 "PremiereAVSGUI.ini" "Field 12" "State"

FunctionEnd

Section -1 #post-gui sequence

        Writeregstr HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "SetMemoryMax" $0
        Writeregstr HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "NearestNeighbour" $2
        Writeregstr HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "Bilinear" $3
        Writeregstr HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "Bicubic" $4
        Writeregstr HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "Lanczos3" $5
        writeregstr HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "Ignore" $R7
        #Messagebox MB_OK $R9
        writeregstr HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "AudioConform" $R9

        #If no plugin-dir SetMemMax stuff is skipped
        strcmp $R0 "none" skipwrite +1
        strcmp $R7 "1" commentout nocommentout

        nocommentout:
        strcmp $R6 "1" commentout continue
        continue:
        readregstr $9 HKEY_LOCAL_MACHINE "SOFTWARE\AviSynth" "plugindir2_5"
        FileOpen $8 "$9\setmemorymax.avsi" "w"
        #messagebox MB_OK "$9 $4"
        FileWrite "$8" "SetMemoryMax($0)"
        FileClose $8
        goto skipwrite

        commentout:
        Writeregstr HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "SetMemoryMax" $0
        readregstr $9 HKEY_LOCAL_MACHINE "SOFTWARE\AviSynth" "plugindir2_5"
        FileOpen $8 "$9\setmemorymax.avsi" "w"
        #messagebox MB_OK "$9 $4"
        FileWrite "$8" "#SetMemoryMax($0)"
        FileClose $8
        strcmp $R7 "1" make1 register
        make1:
        writeregstr HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "Ignore" "1"

        goto skipwrite
        register:
        writeregstr HKEY_LOCAL_MACHINE "SOFTWARE\Premiere AVS Plugin" "Ignore" $R7

        skipwrite:
        setautoclose true
        
SectionEnd
