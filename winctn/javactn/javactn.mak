# Microsoft Developer Studio Generated NMAKE File, Based on javactn.dsp
!IF "$(CFG)" == ""
CFG=javactn - Win32 Debug
!MESSAGE No configuration specified. Defaulting to javactn - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "javactn - Win32 Release" && "$(CFG)" != "javactn - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "javactn.mak" CFG="javactn - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "javactn - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "javactn - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "javactn - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\javactn.dll"


CLEAN :
	-@erase "$(INTDIR)\javactn.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\javactn.dll"
	-@erase "$(OUTDIR)\javactn.exp"
	-@erase "$(OUTDIR)\javactn.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "D:\jdk1.3\include" /I "D:\jdk1.3\include\win32" /I "..\..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAVACTN_EXPORTS" /Fp"$(INTDIR)\javactn.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\javactn.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ctn_lib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\javactn.pdb" /machine:I386 /out:"$(OUTDIR)\javactn.dll" /implib:"$(OUTDIR)\javactn.lib" /libpath:"..\..\winctn\ctn_lib\Release" 
LINK32_OBJS= \
	"$(INTDIR)\javactn.obj"

"$(OUTDIR)\javactn.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "javactn - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\javactn.dll"


CLEAN :
	-@erase "$(INTDIR)\javactn.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\javactn.dll"
	-@erase "$(OUTDIR)\javactn.exp"
	-@erase "$(OUTDIR)\javactn.ilk"
	-@erase "$(OUTDIR)\javactn.lib"
	-@erase "$(OUTDIR)\javactn.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "D:\jdk1.3\include" /I "D:\jdk1.3\include\win32" /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JAVACTN_EXPORTS" /Fp"$(INTDIR)\javactn.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\javactn.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ctn_lib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\javactn.pdb" /debug /machine:I386 /out:"$(OUTDIR)\javactn.dll" /implib:"$(OUTDIR)\javactn.lib" /pdbtype:sept /libpath:"..\..\winctn\ctn_lib\Debug" 
LINK32_OBJS= \
	"$(INTDIR)\javactn.obj"

"$(OUTDIR)\javactn.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("javactn.dep")
!INCLUDE "javactn.dep"
!ELSE 
!MESSAGE Warning: cannot find "javactn.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "javactn - Win32 Release" || "$(CFG)" == "javactn - Win32 Debug"
SOURCE=..\..\javactn\jni_src\javactn.c

"$(INTDIR)\javactn.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

