# Microsoft Developer Studio Generated NMAKE File, Based on clone_study.dsp
!IF "$(CFG)" == ""
CFG=clone_study - Win32 Debug
!MESSAGE No configuration specified. Defaulting to clone_study - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "clone_study - Win32 Release" && "$(CFG)" != "clone_study - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "clone_study.mak" CFG="clone_study - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "clone_study - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "clone_study - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "clone_study - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\clone_study.exe"


CLEAN :
	-@erase "$(INTDIR)\clone_study.obj"
	-@erase "$(INTDIR)\remap.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\clone_study.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\..\..\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\clone_study.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\clone_study.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ctn_lib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\clone_study.pdb" /machine:I386 /out:"$(OUTDIR)\clone_study.exe" /libpath:"..\..\..\winctn\ctn_lib\Release" 
LINK32_OBJS= \
	"$(INTDIR)\clone_study.obj" \
	"$(INTDIR)\remap.obj"

"$(OUTDIR)\clone_study.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "clone_study - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\clone_study.exe"


CLEAN :
	-@erase "$(INTDIR)\clone_study.obj"
	-@erase "$(INTDIR)\remap.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\clone_study.exe"
	-@erase "$(OUTDIR)\clone_study.ilk"
	-@erase "$(OUTDIR)\clone_study.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\clone_study.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\clone_study.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ctn_lib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\clone_study.pdb" /debug /machine:I386 /out:"$(OUTDIR)\clone_study.exe" /pdbtype:sept /libpath:"..\..\..\winctn\ctn_lib\Debug" 
LINK32_OBJS= \
	"$(INTDIR)\clone_study.obj" \
	"$(INTDIR)\remap.obj"

"$(OUTDIR)\clone_study.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("clone_study.dep")
!INCLUDE "clone_study.dep"
!ELSE 
!MESSAGE Warning: cannot find "clone_study.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "clone_study - Win32 Release" || "$(CFG)" == "clone_study - Win32 Debug"
SOURCE=..\..\..\apps\clone_study\clone_study.c

"$(INTDIR)\clone_study.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\apps\clone_study\remap.c

"$(INTDIR)\remap.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

