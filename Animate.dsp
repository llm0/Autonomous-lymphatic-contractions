# Microsoft Developer Studio Project File - Name="Animate" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Animate - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Animate.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Animate.mak" CFG="Animate - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Animate - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Animate - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Animate - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /I "F:" /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 zlib.lib /nologo /subsystem:windows /machine:I386 /libpath:"C:\Program Files (x86)\GnuWin32\lib"

!ELSEIF  "$(CFG)" == "Animate - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 zlib.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"C:\Program Files (x86)\GnuWin32\lib"

!ENDIF 

# Begin Target

# Name "Animate - Win32 Release"
# Name "Animate - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Animate.cpp
# End Source File
# Begin Source File

SOURCE=.\Animate.rc
# End Source File
# Begin Source File

SOURCE=.\AnimateData.cpp
# End Source File
# Begin Source File

SOURCE=.\AnimateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorMap.cpp
# End Source File
# Begin Source File

SOURCE=.\Common.cpp
# End Source File
# Begin Source File

SOURCE=.\LatticeAnimate.cpp
# End Source File
# Begin Source File

SOURCE=.\Line.cpp
# End Source File
# Begin Source File

SOURCE=.\LymphAnimate.cpp
# End Source File
# Begin Source File

SOURCE=.\LymphaticVessle.cpp
# End Source File
# Begin Source File

SOURCE=.\MapWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\Membrane.cpp
# End Source File
# Begin Source File

SOURCE=.\MySliderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\PermeatAnimate.cpp
# End Source File
# Begin Source File

SOURCE=.\PermeatLine.cpp
# End Source File
# Begin Source File

SOURCE=.\Petal.cpp
# End Source File
# Begin Source File

SOURCE=.\PetalAnimate.cpp
# End Source File
# Begin Source File

SOURCE=.\Point.cpp
# End Source File
# Begin Source File

SOURCE=.\Segment.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Text.cpp
# End Source File
# Begin Source File

SOURCE=.\Valve.cpp
# End Source File
# Begin Source File

SOURCE=.\ValveAnimate.cpp
# End Source File
# Begin Source File

SOURCE=.\Vector.cpp
# End Source File
# Begin Source File

SOURCE=.\VectorAnimate.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Animate.h
# End Source File
# Begin Source File

SOURCE=.\AnimateData.h
# End Source File
# Begin Source File

SOURCE=.\AnimateDlg.h
# End Source File
# Begin Source File

SOURCE=.\ColorMap.h
# End Source File
# Begin Source File

SOURCE=.\Common.h
# End Source File
# Begin Source File

SOURCE=.\LatticeAnimate.h
# End Source File
# Begin Source File

SOURCE=.\Line.h
# End Source File
# Begin Source File

SOURCE=.\lymph_h.h
# End Source File
# Begin Source File

SOURCE=.\LymphAnimate.h
# End Source File
# Begin Source File

SOURCE=.\LymphaticVessle.h
# End Source File
# Begin Source File

SOURCE=.\MapWindow.h
# End Source File
# Begin Source File

SOURCE=.\Membrane.h
# End Source File
# Begin Source File

SOURCE=.\MySliderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\PermeatAnimate.h
# End Source File
# Begin Source File

SOURCE=.\PermeatLine.h
# End Source File
# Begin Source File

SOURCE=.\Petal.h
# End Source File
# Begin Source File

SOURCE=.\PetalAnimate.h
# End Source File
# Begin Source File

SOURCE=.\Point.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Segment.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Text.h
# End Source File
# Begin Source File

SOURCE=.\Valve.h
# End Source File
# Begin Source File

SOURCE=.\ValveAnimate.h
# End Source File
# Begin Source File

SOURCE=.\Vector.h
# End Source File
# Begin Source File

SOURCE=.\VectorAnimate.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Animate.ico
# End Source File
# Begin Source File

SOURCE=.\res\Animate.rc2
# End Source File
# Begin Source File

SOURCE=.\res\f.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
