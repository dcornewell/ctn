# Microsoft Developer Studio Generated NMAKE File, Based on query_client.dsp
!IF "$(CFG)" == ""
CFG=query_client - Win32 Debug
!MESSAGE No configuration specified. Defaulting to query_client - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "query_client - Win32 Release" && "$(CFG)" != "query_client - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "query_client.mak" CFG="query_client - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "query_client - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "query_client - Win32 Debug" (based on "Win32 (x86) Application")
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

!IF  "$(CFG)" == "query_client - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\query_client.exe"


CLEAN :
	-@erase "$(INTDIR)\format.obj"
	-@erase "$(INTDIR)\query_client.obj"
	-@erase "$(INTDIR)\query_client.res"
	-@erase "$(INTDIR)\query_clientdlg.obj"
	-@erase "$(INTDIR)\query_interface.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\query_client.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\..\..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\query_client.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\query_client.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\query_client.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ctn_lib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\query_client.pdb" /machine:I386 /out:"$(OUTDIR)\query_client.exe" /libpath:"..\..\..\winctn\ctn_lib\Release" 
LINK32_OBJS= \
	"$(INTDIR)\format.obj" \
	"$(INTDIR)\query_client.obj" \
	"$(INTDIR)\query_clientdlg.obj" \
	"$(INTDIR)\query_interface.obj" \
	"$(INTDIR)\query_client.res"

"$(OUTDIR)\query_client.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "query_client - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\query_client.exe"


CLEAN :
	-@erase "$(INTDIR)\format.obj"
	-@erase "$(INTDIR)\query_client.obj"
	-@erase "$(INTDIR)\query_client.res"
	-@erase "$(INTDIR)\query_clientdlg.obj"
	-@erase "$(INTDIR)\query_interface.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\query_client.exe"
	-@erase "$(OUTDIR)\query_client.ilk"
	-@erase "$(OUTDIR)\query_client.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\query_client.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\query_client.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\query_client.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ctn_lib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\query_client.pdb" /debug /machine:I386 /out:"$(OUTDIR)\query_client.exe" /pdbtype:sept /libpath:"..\..\..\winctn\ctn_lib\Debug" 
LINK32_OBJS= \
	"$(INTDIR)\format.obj" \
	"$(INTDIR)\query_client.obj" \
	"$(INTDIR)\query_clientdlg.obj" \
	"$(INTDIR)\query_interface.obj" \
	"$(INTDIR)\query_client.res"

"$(OUTDIR)\query_client.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("query_client.dep")
!INCLUDE "query_client.dep"
!ELSE 
!MESSAGE Warning: cannot find "query_client.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "query_client - Win32 Release" || "$(CFG)" == "query_client - Win32 Debug"
SOURCE=..\..\..\apps\query_client\format.c

"$(INTDIR)\format.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\apps\query_client\query_client.cpp

"$(INTDIR)\query_client.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\apps\query_client\query_client.rc

!IF  "$(CFG)" == "query_client - Win32 Release"


"$(INTDIR)\query_client.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\query_client.res" /i "\prj\ctn\apps\query_client" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "query_client - Win32 Debug"


"$(INTDIR)\query_client.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\query_client.res" /i "\prj\ctn\apps\query_client" /d "_DEBUG" $(SOURCE)


!ENDIF 

SOURCE=..\..\..\apps\query_client\query_clientdlg.cpp

"$(INTDIR)\query_clientdlg.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\..\apps\query_client\query_interface.c

"$(INTDIR)\query_interface.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

