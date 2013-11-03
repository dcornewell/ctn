# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101
# TARGTYPE "Win32 (x86) Console Application" 0x0103
# TARGTYPE "Win32 (x86) External Target" 0x0106

!IF "$(CFG)" == ""
CFG=mwlQuery - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to mwlQuery - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "win32apps - Win32 Release" && "$(CFG)" !=\
 "win32apps - Win32 Debug" && "$(CFG)" != "dcm_dump_element - Win32 Release" &&\
 "$(CFG)" != "dcm_dump_element - Win32 Debug" && "$(CFG)" !=\
 "dcm_dump_file - Win32 Release" && "$(CFG)" != "dcm_dump_file - Win32 Debug" &&\
 "$(CFG)" != "tbltest - Win32 Release" && "$(CFG)" != "tbltest - Win32 Debug" &&\
 "$(CFG)" != "ttinsert - Win32 Release" && "$(CFG)" != "ttinsert - Win32 Debug"\
 && "$(CFG)" != "ttselect - Win32 Release" && "$(CFG)" !=\
 "ttselect - Win32 Debug" && "$(CFG)" != "ttupdate - Win32 Release" && "$(CFG)"\
 != "ttupdate - Win32 Debug" && "$(CFG)" != "ttdelete - Win32 Release" &&\
 "$(CFG)" != "ttdelete - Win32 Debug" && "$(CFG)" != "ttunique - Win32 Release"\
 && "$(CFG)" != "ttunique - Win32 Debug" && "$(CFG)" !=\
 "ttlayout - Win32 Release" && "$(CFG)" != "ttlayout - Win32 Debug" && "$(CFG)"\
 != "load_control - Win32 Release" && "$(CFG)" != "load_control - Win32 Debug"\
 && "$(CFG)" != "archive_server - Win32 Release" && "$(CFG)" !=\
 "archive_server - Win32 Debug" && "$(CFG)" != "dcm_make_object - Win32 Release"\
 && "$(CFG)" != "dcm_make_object - Win32 Debug" && "$(CFG)" !=\
 "dcm_print_dictionary - Win32 Release" && "$(CFG)" !=\
 "dcm_print_dictionary - Win32 Debug" && "$(CFG)" !=\
 "dcm_modify_elements - Win32 Release" && "$(CFG)" !=\
 "dcm_modify_elements - Win32 Debug" && "$(CFG)" !=\
 "dcm_rm_element - Win32 Release" && "$(CFG)" != "dcm_rm_element - Win32 Debug"\
 && "$(CFG)" != "dicom_echo - Win32 Release" && "$(CFG)" !=\
 "dicom_echo - Win32 Debug" && "$(CFG)" != "dcm_rm_group - Win32 Release" &&\
 "$(CFG)" != "dcm_rm_group - Win32 Debug" && "$(CFG)" !=\
 "send_image - Win32 Release" && "$(CFG)" != "send_image - Win32 Debug" &&\
 "$(CFG)" != "simple_storage - Win32 Release" && "$(CFG)" !=\
 "simple_storage - Win32 Debug" && "$(CFG)" != "dcm_w_disp - Win32 Release" &&\
 "$(CFG)" != "dcm_w_disp - Win32 Debug" && "$(CFG)" !=\
 "query_client - Win32 Release" && "$(CFG)" != "query_client - Win32 Debug" &&\
 "$(CFG)" != "dcm_ctnto10 - Win32 Release" && "$(CFG)" !=\
 "dcm_ctnto10 - Win32 Debug" && "$(CFG)" != "dcm_diff - Win32 Release" &&\
 "$(CFG)" != "dcm_diff - Win32 Debug" && "$(CFG)" !=\
 "dcm_map_to_8 - Win32 Release" && "$(CFG)" != "dcm_map_to_8 - Win32 Debug" &&\
 "$(CFG)" != "dcm_modify_object - Win32 Release" && "$(CFG)" !=\
 "dcm_modify_object - Win32 Debug" && "$(CFG)" != "dcm_resize - Win32 Release"\
 && "$(CFG)" != "dcm_resize - Win32 Debug" && "$(CFG)" !=\
 "dcm_verify - Win32 Release" && "$(CFG)" != "dcm_verify - Win32 Debug" &&\
 "$(CFG)" != "print_server - Win32 Release" && "$(CFG)" !=\
 "print_server - Win32 Debug" && "$(CFG)" != "mwlQuery - Win32 Release" &&\
 "$(CFG)" != "mwlQuery - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "win32apps.mak" CFG="mwlQuery - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "win32apps - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "win32apps - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "dcm_dump_element - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "dcm_dump_element - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "dcm_dump_file - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "dcm_dump_file - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "tbltest - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "tbltest - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "ttinsert - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "ttinsert - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "ttselect - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "ttselect - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "ttupdate - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "ttupdate - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "ttdelete - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "ttdelete - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "ttunique - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "ttunique - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "ttlayout - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "ttlayout - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "load_control - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "load_control - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "archive_server - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "archive_server - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "dcm_make_object - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "dcm_make_object - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "dcm_print_dictionary - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "dcm_print_dictionary - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "dcm_modify_elements - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "dcm_modify_elements - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "dcm_rm_element - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "dcm_rm_element - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "dicom_echo - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "dicom_echo - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "dcm_rm_group - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "dcm_rm_group - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "send_image - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "send_image - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "simple_storage - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "simple_storage - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "dcm_w_disp - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "dcm_w_disp - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "query_client - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "query_client - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "dcm_ctnto10 - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "dcm_ctnto10 - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "dcm_diff - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "dcm_diff - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "dcm_map_to_8 - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "dcm_map_to_8 - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "dcm_modify_object - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "dcm_modify_object - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "dcm_resize - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "dcm_resize - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "dcm_verify - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "dcm_verify - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "print_server - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "print_server - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "mwlQuery - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "mwlQuery - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "send_image - Win32 Debug"

!IF  "$(CFG)" == "win32apps - Win32 Release"

# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP BASE Cmd_Line "NMAKE /f win32apps.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "win32apps.exe"
# PROP BASE Bsc_Name "win32apps.bsc"
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# PROP Cmd_Line "NMAKE /f win32apps.mak"
# PROP Rebuild_Opt "/a"
# PROP Target_File "win32apps.exe"
# PROP Bsc_Name "win32apps.bsc"
OUTDIR=.\Release
INTDIR=.\Release

ALL : "mwlQuery - Win32 Release" "print_server - Win32 Release"\
 "dcm_verify - Win32 Release" "dcm_resize - Win32 Release"\
 "dcm_modify_object - Win32 Release" "dcm_map_to_8 - Win32 Release"\
 "dcm_diff - Win32 Release" "dcm_ctnto10 - Win32 Release"\
 "query_client - Win32 Release" "dcm_w_disp - Win32 Release"\
 "simple_storage - Win32 Release" "send_image - Win32 Release"\
 "dcm_rm_group - Win32 Release" "dicom_echo - Win32 Release"\
 "dcm_rm_element - Win32 Release" "dcm_modify_elements - Win32 Release"\
 "dcm_print_dictionary - Win32 Release" "dcm_make_object - Win32 Release"\
 "archive_server - Win32 Release" "load_control - Win32 Release"\
 "tbltest - Win32 Release" "dcm_dump_file - Win32 Release"\
 "dcm_dump_element - Win32 Release" 

CLEAN : 
	-@erase 

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

!ELSEIF  "$(CFG)" == "win32apps - Win32 Debug"

# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP BASE Cmd_Line "NMAKE /f win32apps.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "win32apps.exe"
# PROP BASE Bsc_Name "win32apps.bsc"
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# PROP Cmd_Line "NMAKE /f win32apps.mak"
# PROP Rebuild_Opt "/a"
# PROP Target_File "win32apps.exe"
# PROP Bsc_Name "win32apps.bsc"
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "mwlQuery - Win32 Debug" "print_server - Win32 Debug"\
 "dcm_verify - Win32 Debug" "dcm_resize - Win32 Debug"\
 "dcm_modify_object - Win32 Debug" "dcm_map_to_8 - Win32 Debug"\
 "dcm_diff - Win32 Debug" "dcm_ctnto10 - Win32 Debug"\
 "query_client - Win32 Debug" "dcm_w_disp - Win32 Debug"\
 "simple_storage - Win32 Debug" "send_image - Win32 Debug"\
 "dcm_rm_group - Win32 Debug" "dicom_echo - Win32 Debug"\
 "dcm_rm_element - Win32 Debug" "dcm_modify_elements - Win32 Debug"\
 "dcm_print_dictionary - Win32 Debug" "dcm_make_object - Win32 Debug"\
 "archive_server - Win32 Debug" "load_control - Win32 Debug"\
 "tbltest - Win32 Debug" "dcm_dump_file - Win32 Debug"\
 "dcm_dump_element - Win32 Debug" 

CLEAN : 
	-@erase 

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

!ELSEIF  "$(CFG)" == "dcm_dump_element - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "dcm_dump_element\Release"
# PROP BASE Intermediate_Dir "dcm_dump_element\Release"
# PROP BASE Target_Dir "dcm_dump_element"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "dcm_dump_element\Release"
# PROP Intermediate_Dir "dcm_dump_element\Release"
# PROP Target_Dir "dcm_dump_element"
OUTDIR=.\dcm_dump_element\Release
INTDIR=.\dcm_dump_element\Release

ALL : "$(OUTDIR)\dcm_dump_element.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_dump_element.obj"
	-@erase "$(OUTDIR)\dcm_dump_element.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dcm_dump_element.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_dump_element\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_dump_element.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/dcm_dump_element.pdb" /machine:I386\
 /out:"$(OUTDIR)/dcm_dump_element.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_dump_element.obj"

"$(OUTDIR)\dcm_dump_element.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_dump_element - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "dcm_dump_element\dcm_dump"
# PROP BASE Intermediate_Dir "dcm_dump_element\dcm_dump"
# PROP BASE Target_Dir "dcm_dump_element"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "dcm_dump_element\debug"
# PROP Intermediate_Dir "dcm_dump_element\debug"
# PROP Target_Dir "dcm_dump_element"
OUTDIR=.\dcm_dump_element\debug
INTDIR=.\dcm_dump_element\debug

ALL : "$(OUTDIR)\dcm_dump_element.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_dump_element.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\dcm_dump_element.exe"
	-@erase "$(OUTDIR)\dcm_dump_element.ilk"
	-@erase "$(OUTDIR)\dcm_dump_element.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dcm_dump_element.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_dump_element\debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_dump_element.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/dcm_dump_element.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/dcm_dump_element.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_dump_element.obj"

"$(OUTDIR)\dcm_dump_element.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_dump_file - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "dcm_dump_file\Release"
# PROP BASE Intermediate_Dir "dcm_dump_file\Release"
# PROP BASE Target_Dir "dcm_dump_file"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "dcm_dump_file\Release"
# PROP Intermediate_Dir "dcm_dump_file\Release"
# PROP Target_Dir "dcm_dump_file"
OUTDIR=.\dcm_dump_file\Release
INTDIR=.\dcm_dump_file\Release

ALL : "$(OUTDIR)\dcm_dump_file.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_dump_file.obj"
	-@erase "$(OUTDIR)\dcm_dump_file.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dcm_dump_file.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_dump_file\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_dump_file.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/dcm_dump_file.pdb" /machine:I386\
 /out:"$(OUTDIR)/dcm_dump_file.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_dump_file.obj"

"$(OUTDIR)\dcm_dump_file.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_dump_file - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "dcm_dump_file\dcm_dump"
# PROP BASE Intermediate_Dir "dcm_dump_file\dcm_dump"
# PROP BASE Target_Dir "dcm_dump_file"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "dcm_dump_file\debug"
# PROP Intermediate_Dir "dcm_dump_file\debug"
# PROP Target_Dir "dcm_dump_file"
OUTDIR=.\dcm_dump_file\debug
INTDIR=.\dcm_dump_file\debug

ALL : "$(OUTDIR)\dcm_dump_file.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_dump_file.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\dcm_dump_file.exe"
	-@erase "$(OUTDIR)\dcm_dump_file.ilk"
	-@erase "$(OUTDIR)\dcm_dump_file.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dcm_dump_file.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_dump_file\debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_dump_file.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/dcm_dump_file.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/dcm_dump_file.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_dump_file.obj"

"$(OUTDIR)\dcm_dump_file.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "tbltest - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "tbltest\Release"
# PROP BASE Intermediate_Dir "tbltest\Release"
# PROP BASE Target_Dir "tbltest"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "tbltest\Release"
# PROP Intermediate_Dir "tbltest\Release"
# PROP Target_Dir "tbltest"
OUTDIR=.\tbltest\Release
INTDIR=.\tbltest\Release

ALL : "ttunique - Win32 Release" "ttdelete - Win32 Release"\
 "ttupdate - Win32 Release" "ttselect - Win32 Release"\
 "ttinsert - Win32 Release" 

CLEAN : 
	-@erase 

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/tbltest.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\tbltest\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/tbltest.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/tbltest.pdb" /machine:I386 /out:"$(OUTDIR)/tbltest.exe" 
LINK32_OBJS= \
	

!ELSEIF  "$(CFG)" == "tbltest - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "tbltest\Debug"
# PROP BASE Intermediate_Dir "tbltest\Debug"
# PROP BASE Target_Dir "tbltest"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "tbltest\Debug"
# PROP Intermediate_Dir "tbltest\Debug"
# PROP Target_Dir "tbltest"
OUTDIR=.\tbltest\Debug
INTDIR=.\tbltest\Debug

ALL : "ttunique - Win32 Debug" "ttdelete - Win32 Debug"\
 "ttupdate - Win32 Debug" "ttselect - Win32 Debug" "ttinsert - Win32 Debug" 

CLEAN : 
	-@erase 

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/tbltest.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\tbltest\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/tbltest.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/tbltest.pdb" /debug /machine:I386 /out:"$(OUTDIR)/tbltest.exe" 
LINK32_OBJS= \
	

!ELSEIF  "$(CFG)" == "ttinsert - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ttinsert\Release"
# PROP BASE Intermediate_Dir "ttinsert\Release"
# PROP BASE Target_Dir "ttinsert"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ttinsert\Release"
# PROP Intermediate_Dir "ttinsert\Release"
# PROP Target_Dir "ttinsert"
OUTDIR=.\ttinsert\Release
INTDIR=.\ttinsert\Release

ALL : "$(OUTDIR)\ttinsert.exe"

CLEAN : 
	-@erase "$(INTDIR)\ttinsert.obj"
	-@erase "$(OUTDIR)\ttinsert.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/ttinsert.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\ttinsert\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ttinsert.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/ttinsert.pdb" /machine:I386 /out:"$(OUTDIR)/ttinsert.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ttinsert.obj"

"$(OUTDIR)\ttinsert.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ttinsert - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ttinsert\Debug"
# PROP BASE Intermediate_Dir "ttinsert\Debug"
# PROP BASE Target_Dir "ttinsert"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ttinsert\Debug"
# PROP Intermediate_Dir "ttinsert\Debug"
# PROP Target_Dir "ttinsert"
OUTDIR=.\ttinsert\Debug
INTDIR=.\ttinsert\Debug

ALL : "$(OUTDIR)\ttinsert.exe"

CLEAN : 
	-@erase "$(INTDIR)\ttinsert.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\ttinsert.exe"
	-@erase "$(OUTDIR)\ttinsert.ilk"
	-@erase "$(OUTDIR)\ttinsert.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/ttinsert.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\ttinsert\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ttinsert.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/ttinsert.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/ttinsert.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ttinsert.obj"

"$(OUTDIR)\ttinsert.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ttselect - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ttselect\Release"
# PROP BASE Intermediate_Dir "ttselect\Release"
# PROP BASE Target_Dir "ttselect"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ttselect\Release"
# PROP Intermediate_Dir "ttselect\Release"
# PROP Target_Dir "ttselect"
OUTDIR=.\ttselect\Release
INTDIR=.\ttselect\Release

ALL : "$(OUTDIR)\ttselect.exe"

CLEAN : 
	-@erase "$(INTDIR)\ttselect.obj"
	-@erase "$(OUTDIR)\ttselect.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/ttselect.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\ttselect\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ttselect.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/ttselect.pdb" /machine:I386 /out:"$(OUTDIR)/ttselect.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ttselect.obj"

"$(OUTDIR)\ttselect.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ttselect - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ttselect\Debug"
# PROP BASE Intermediate_Dir "ttselect\Debug"
# PROP BASE Target_Dir "ttselect"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ttselect\Debug"
# PROP Intermediate_Dir "ttselect\Debug"
# PROP Target_Dir "ttselect"
OUTDIR=.\ttselect\Debug
INTDIR=.\ttselect\Debug

ALL : "$(OUTDIR)\ttselect.exe"

CLEAN : 
	-@erase "$(INTDIR)\ttselect.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\ttselect.exe"
	-@erase "$(OUTDIR)\ttselect.ilk"
	-@erase "$(OUTDIR)\ttselect.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/ttselect.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\ttselect\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ttselect.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/ttselect.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/ttselect.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ttselect.obj"

"$(OUTDIR)\ttselect.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ttupdate - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ttupdate\Release"
# PROP BASE Intermediate_Dir "ttupdate\Release"
# PROP BASE Target_Dir "ttupdate"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ttupdate\Release"
# PROP Intermediate_Dir "ttupdate\Release"
# PROP Target_Dir "ttupdate"
OUTDIR=.\ttupdate\Release
INTDIR=.\ttupdate\Release

ALL : "$(OUTDIR)\ttupdate.exe"

CLEAN : 
	-@erase "$(INTDIR)\ttupdate.obj"
	-@erase "$(OUTDIR)\ttupdate.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/ttupdate.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\ttupdate\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ttupdate.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/ttupdate.pdb" /machine:I386 /out:"$(OUTDIR)/ttupdate.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ttupdate.obj"

"$(OUTDIR)\ttupdate.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ttupdate - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ttupdate\Debug"
# PROP BASE Intermediate_Dir "ttupdate\Debug"
# PROP BASE Target_Dir "ttupdate"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ttupdate\Debug"
# PROP Intermediate_Dir "ttupdate\Debug"
# PROP Target_Dir "ttupdate"
OUTDIR=.\ttupdate\Debug
INTDIR=.\ttupdate\Debug

ALL : "$(OUTDIR)\ttupdate.exe"

CLEAN : 
	-@erase "$(INTDIR)\ttupdate.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\ttupdate.exe"
	-@erase "$(OUTDIR)\ttupdate.ilk"
	-@erase "$(OUTDIR)\ttupdate.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/ttupdate.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\ttupdate\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ttupdate.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/ttupdate.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/ttupdate.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ttupdate.obj"

"$(OUTDIR)\ttupdate.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ttdelete - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ttdelete\Release"
# PROP BASE Intermediate_Dir "ttdelete\Release"
# PROP BASE Target_Dir "ttdelete"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ttdelete\Release"
# PROP Intermediate_Dir "ttdelete\Release"
# PROP Target_Dir "ttdelete"
OUTDIR=.\ttdelete\Release
INTDIR=.\ttdelete\Release

ALL : "ttlayout - Win32 Release" "$(OUTDIR)\ttdelete.exe"

CLEAN : 
	-@erase "$(INTDIR)\ttdelete.obj"
	-@erase "$(OUTDIR)\ttdelete.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/ttdelete.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\ttdelete\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ttdelete.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/ttdelete.pdb" /machine:I386 /out:"$(OUTDIR)/ttdelete.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ttdelete.obj"

"$(OUTDIR)\ttdelete.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ttdelete - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ttdelete\Debug"
# PROP BASE Intermediate_Dir "ttdelete\Debug"
# PROP BASE Target_Dir "ttdelete"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ttdelete\Debug"
# PROP Intermediate_Dir "ttdelete\Debug"
# PROP Target_Dir "ttdelete"
OUTDIR=.\ttdelete\Debug
INTDIR=.\ttdelete\Debug

ALL : "ttlayout - Win32 Debug" "$(OUTDIR)\ttdelete.exe"

CLEAN : 
	-@erase "$(INTDIR)\ttdelete.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\ttdelete.exe"
	-@erase "$(OUTDIR)\ttdelete.ilk"
	-@erase "$(OUTDIR)\ttdelete.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/ttdelete.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\ttdelete\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ttdelete.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/ttdelete.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/ttdelete.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ttdelete.obj"

"$(OUTDIR)\ttdelete.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ttunique - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ttunique\Release"
# PROP BASE Intermediate_Dir "ttunique\Release"
# PROP BASE Target_Dir "ttunique"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ttunique\Release"
# PROP Intermediate_Dir "ttunique\Release"
# PROP Target_Dir "ttunique"
OUTDIR=.\ttunique\Release
INTDIR=.\ttunique\Release

ALL : "$(OUTDIR)\ttunique.exe"

CLEAN : 
	-@erase "$(INTDIR)\ttunique.obj"
	-@erase "$(OUTDIR)\ttunique.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/ttunique.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\ttunique\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ttunique.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/ttunique.pdb" /machine:I386 /out:"$(OUTDIR)/ttunique.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ttunique.obj"

"$(OUTDIR)\ttunique.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ttunique - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ttunique\Debug"
# PROP BASE Intermediate_Dir "ttunique\Debug"
# PROP BASE Target_Dir "ttunique"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ttunique\Debug"
# PROP Intermediate_Dir "ttunique\Debug"
# PROP Target_Dir "ttunique"
OUTDIR=.\ttunique\Debug
INTDIR=.\ttunique\Debug

ALL : "$(OUTDIR)\ttunique.exe"

CLEAN : 
	-@erase "$(INTDIR)\ttunique.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\ttunique.exe"
	-@erase "$(OUTDIR)\ttunique.ilk"
	-@erase "$(OUTDIR)\ttunique.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/ttunique.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\ttunique\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ttunique.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/ttunique.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/ttunique.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ttunique.obj"

"$(OUTDIR)\ttunique.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ttlayout - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ttlayout\Release"
# PROP BASE Intermediate_Dir "ttlayout\Release"
# PROP BASE Target_Dir "ttlayout"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ttlayout\Release"
# PROP Intermediate_Dir "ttlayout\Release"
# PROP Target_Dir "ttlayout"
OUTDIR=.\ttlayout\Release
INTDIR=.\ttlayout\Release

ALL : "$(OUTDIR)\ttlayout.exe"

CLEAN : 
	-@erase "$(INTDIR)\ttlayout.obj"
	-@erase "$(OUTDIR)\ttlayout.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/ttlayout.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\ttlayout\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ttlayout.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/ttlayout.pdb" /machine:I386 /out:"$(OUTDIR)/ttlayout.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ttlayout.obj"

"$(OUTDIR)\ttlayout.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ttlayout - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ttlayout\Debug"
# PROP BASE Intermediate_Dir "ttlayout\Debug"
# PROP BASE Target_Dir "ttlayout"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ttlayout\Debug"
# PROP Intermediate_Dir "ttlayout\Debug"
# PROP Target_Dir "ttlayout"
OUTDIR=.\ttlayout\Debug
INTDIR=.\ttlayout\Debug

ALL : "$(OUTDIR)\ttlayout.exe"

CLEAN : 
	-@erase "$(INTDIR)\ttlayout.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\ttlayout.exe"
	-@erase "$(OUTDIR)\ttlayout.ilk"
	-@erase "$(OUTDIR)\ttlayout.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/ttlayout.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\ttlayout\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ttlayout.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/ttlayout.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/ttlayout.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ttlayout.obj"

"$(OUTDIR)\ttlayout.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "load_control - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "load_control\Release"
# PROP BASE Intermediate_Dir "load_control\Release"
# PROP BASE Target_Dir "load_control"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "load_control\Release"
# PROP Intermediate_Dir "load_control\Release"
# PROP Target_Dir "load_control"
OUTDIR=.\load_control\Release
INTDIR=.\load_control\Release

ALL : "$(OUTDIR)\load_control.exe"

CLEAN : 
	-@erase "$(INTDIR)\load_control.obj"
	-@erase "$(OUTDIR)\load_control.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/load_control.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\load_control\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/load_control.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/load_control.pdb" /machine:I386\
 /out:"$(OUTDIR)/load_control.exe" 
LINK32_OBJS= \
	"$(INTDIR)\load_control.obj"

"$(OUTDIR)\load_control.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "load_control - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "load_control\Debug"
# PROP BASE Intermediate_Dir "load_control\Debug"
# PROP BASE Target_Dir "load_control"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "load_control\Debug"
# PROP Intermediate_Dir "load_control\Debug"
# PROP Target_Dir "load_control"
OUTDIR=.\load_control\Debug
INTDIR=.\load_control\Debug

ALL : "$(OUTDIR)\load_control.exe"

CLEAN : 
	-@erase "$(INTDIR)\load_control.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\load_control.exe"
	-@erase "$(OUTDIR)\load_control.ilk"
	-@erase "$(OUTDIR)\load_control.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/load_control.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\load_control\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/load_control.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/load_control.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/load_control.exe" 
LINK32_OBJS= \
	"$(INTDIR)\load_control.obj"

"$(OUTDIR)\load_control.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "archive_server - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "archive_server\Release"
# PROP BASE Intermediate_Dir "archive_server\Release"
# PROP BASE Target_Dir "archive_server"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "archive_server\Release"
# PROP Intermediate_Dir "archive_server\Release"
# PROP Target_Dir "archive_server"
OUTDIR=.\archive_server\Release
INTDIR=.\archive_server\Release

ALL : "$(OUTDIR)\archive_server.exe"

CLEAN : 
	-@erase "$(INTDIR)\archive_server.obj"
	-@erase "$(INTDIR)\association.obj"
	-@erase "$(INTDIR)\cget.obj"
	-@erase "$(INTDIR)\copy.obj"
	-@erase "$(INTDIR)\find.obj"
	-@erase "$(INTDIR)\move.obj"
	-@erase "$(INTDIR)\naction.obj"
	-@erase "$(INTDIR)\parse.obj"
	-@erase "$(INTDIR)\requests.obj"
	-@erase "$(INTDIR)\sscond.obj"
	-@erase "$(OUTDIR)\archive_server.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/archive_server.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\archive_server\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/archive_server.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/archive_server.pdb" /machine:I386\
 /out:"$(OUTDIR)/archive_server.exe" 
LINK32_OBJS= \
	"$(INTDIR)\archive_server.obj" \
	"$(INTDIR)\association.obj" \
	"$(INTDIR)\cget.obj" \
	"$(INTDIR)\copy.obj" \
	"$(INTDIR)\find.obj" \
	"$(INTDIR)\move.obj" \
	"$(INTDIR)\naction.obj" \
	"$(INTDIR)\parse.obj" \
	"$(INTDIR)\requests.obj" \
	"$(INTDIR)\sscond.obj"

"$(OUTDIR)\archive_server.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "archive_server - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "archive_server\Debug"
# PROP BASE Intermediate_Dir "archive_server\Debug"
# PROP BASE Target_Dir "archive_server"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "archive_server\Debug"
# PROP Intermediate_Dir "archive_server\Debug"
# PROP Target_Dir "archive_server"
OUTDIR=.\archive_server\Debug
INTDIR=.\archive_server\Debug

ALL : "$(OUTDIR)\archive_server.exe"

CLEAN : 
	-@erase "$(INTDIR)\archive_server.obj"
	-@erase "$(INTDIR)\association.obj"
	-@erase "$(INTDIR)\cget.obj"
	-@erase "$(INTDIR)\copy.obj"
	-@erase "$(INTDIR)\find.obj"
	-@erase "$(INTDIR)\move.obj"
	-@erase "$(INTDIR)\naction.obj"
	-@erase "$(INTDIR)\parse.obj"
	-@erase "$(INTDIR)\requests.obj"
	-@erase "$(INTDIR)\sscond.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\archive_server.exe"
	-@erase "$(OUTDIR)\archive_server.ilk"
	-@erase "$(OUTDIR)\archive_server.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/archive_server.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\archive_server\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/archive_server.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/archive_server.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/archive_server.exe" 
LINK32_OBJS= \
	"$(INTDIR)\archive_server.obj" \
	"$(INTDIR)\association.obj" \
	"$(INTDIR)\cget.obj" \
	"$(INTDIR)\copy.obj" \
	"$(INTDIR)\find.obj" \
	"$(INTDIR)\move.obj" \
	"$(INTDIR)\naction.obj" \
	"$(INTDIR)\parse.obj" \
	"$(INTDIR)\requests.obj" \
	"$(INTDIR)\sscond.obj"

"$(OUTDIR)\archive_server.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_make_object - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "dcm_make_object\Release"
# PROP BASE Intermediate_Dir "dcm_make_object\Release"
# PROP BASE Target_Dir "dcm_make_object"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "dcm_make_object\Release"
# PROP Intermediate_Dir "dcm_make_object\Release"
# PROP Target_Dir "dcm_make_object"
OUTDIR=.\dcm_make_object\Release
INTDIR=.\dcm_make_object\Release

ALL : "$(OUTDIR)\dcm_make_object.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_make_object.obj"
	-@erase "$(OUTDIR)\dcm_make_object.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dcm_make_object.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_make_object\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_make_object.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/dcm_make_object.pdb" /machine:I386\
 /out:"$(OUTDIR)/dcm_make_object.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_make_object.obj"

"$(OUTDIR)\dcm_make_object.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_make_object - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "dcm_make_object\Debug"
# PROP BASE Intermediate_Dir "dcm_make_object\Debug"
# PROP BASE Target_Dir "dcm_make_object"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "dcm_make_object\Debug"
# PROP Intermediate_Dir "dcm_make_object\Debug"
# PROP Target_Dir "dcm_make_object"
OUTDIR=.\dcm_make_object\Debug
INTDIR=.\dcm_make_object\Debug

ALL : "$(OUTDIR)\dcm_make_object.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_make_object.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\dcm_make_object.exe"
	-@erase "$(OUTDIR)\dcm_make_object.ilk"
	-@erase "$(OUTDIR)\dcm_make_object.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dcm_make_object.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_make_object\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_make_object.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/dcm_make_object.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/dcm_make_object.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_make_object.obj"

"$(OUTDIR)\dcm_make_object.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_print_dictionary - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "dcm_print_dictionary\Release"
# PROP BASE Intermediate_Dir "dcm_print_dictionary\Release"
# PROP BASE Target_Dir "dcm_print_dictionary"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "dcm_print_dictionary\Release"
# PROP Intermediate_Dir "dcm_print_dictionary\Release"
# PROP Target_Dir "dcm_print_dictionary"
OUTDIR=.\dcm_print_dictionary\Release
INTDIR=.\dcm_print_dictionary\Release

ALL : "$(OUTDIR)\dcm_print_dictionary.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_print_dictionary.obj"
	-@erase "$(OUTDIR)\dcm_print_dictionary.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dcm_print_dictionary.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_print_dictionary\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_print_dictionary.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/dcm_print_dictionary.pdb" /machine:I386\
 /out:"$(OUTDIR)/dcm_print_dictionary.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_print_dictionary.obj"

"$(OUTDIR)\dcm_print_dictionary.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_print_dictionary - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "dcm_print_dictionary\Debug"
# PROP BASE Intermediate_Dir "dcm_print_dictionary\Debug"
# PROP BASE Target_Dir "dcm_print_dictionary"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "dcm_print_dictionary\Debug"
# PROP Intermediate_Dir "dcm_print_dictionary\Debug"
# PROP Target_Dir "dcm_print_dictionary"
OUTDIR=.\dcm_print_dictionary\Debug
INTDIR=.\dcm_print_dictionary\Debug

ALL : "$(OUTDIR)\dcm_print_dictionary.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_print_dictionary.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\dcm_print_dictionary.exe"
	-@erase "$(OUTDIR)\dcm_print_dictionary.ilk"
	-@erase "$(OUTDIR)\dcm_print_dictionary.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dcm_print_dictionary.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_print_dictionary\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_print_dictionary.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/dcm_print_dictionary.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/dcm_print_dictionary.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_print_dictionary.obj"

"$(OUTDIR)\dcm_print_dictionary.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_modify_elements - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "dcm_modify_elements\Release"
# PROP BASE Intermediate_Dir "dcm_modify_elements\Release"
# PROP BASE Target_Dir "dcm_modify_elements"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "dcm_modify_elements\Release"
# PROP Intermediate_Dir "dcm_modify_elements\Release"
# PROP Target_Dir "dcm_modify_elements"
OUTDIR=.\dcm_modify_elements\Release
INTDIR=.\dcm_modify_elements\Release

ALL : "$(OUTDIR)\dcm_modify_elements.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_modify_elements.obj"
	-@erase "$(OUTDIR)\dcm_modify_elements.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dcm_modify_elements.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_modify_elements\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_modify_elements.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/dcm_modify_elements.pdb" /machine:I386\
 /out:"$(OUTDIR)/dcm_modify_elements.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_modify_elements.obj"

"$(OUTDIR)\dcm_modify_elements.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_modify_elements - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "dcm_modify_elements\Debug"
# PROP BASE Intermediate_Dir "dcm_modify_elements\Debug"
# PROP BASE Target_Dir "dcm_modify_elements"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "dcm_modify_elements\Debug"
# PROP Intermediate_Dir "dcm_modify_elements\Debug"
# PROP Target_Dir "dcm_modify_elements"
OUTDIR=.\dcm_modify_elements\Debug
INTDIR=.\dcm_modify_elements\Debug

ALL : "$(OUTDIR)\dcm_modify_elements.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_modify_elements.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\dcm_modify_elements.exe"
	-@erase "$(OUTDIR)\dcm_modify_elements.ilk"
	-@erase "$(OUTDIR)\dcm_modify_elements.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dcm_modify_elements.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_modify_elements\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_modify_elements.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/dcm_modify_elements.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/dcm_modify_elements.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_modify_elements.obj"

"$(OUTDIR)\dcm_modify_elements.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_rm_element - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "dcm_rm_element\Release"
# PROP BASE Intermediate_Dir "dcm_rm_element\Release"
# PROP BASE Target_Dir "dcm_rm_element"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "dcm_rm_element\Release"
# PROP Intermediate_Dir "dcm_rm_element\Release"
# PROP Target_Dir "dcm_rm_element"
OUTDIR=.\dcm_rm_element\Release
INTDIR=.\dcm_rm_element\Release

ALL : "$(OUTDIR)\dcm_rm_element.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_rm_element.obj"
	-@erase "$(OUTDIR)\dcm_rm_element.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dcm_rm_element.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_rm_element\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_rm_element.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/dcm_rm_element.pdb" /machine:I386\
 /out:"$(OUTDIR)/dcm_rm_element.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_rm_element.obj"

"$(OUTDIR)\dcm_rm_element.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_rm_element - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "dcm_rm_element\Debug"
# PROP BASE Intermediate_Dir "dcm_rm_element\Debug"
# PROP BASE Target_Dir "dcm_rm_element"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "dcm_rm_element\Debug"
# PROP Intermediate_Dir "dcm_rm_element\Debug"
# PROP Target_Dir "dcm_rm_element"
OUTDIR=.\dcm_rm_element\Debug
INTDIR=.\dcm_rm_element\Debug

ALL : "$(OUTDIR)\dcm_rm_element.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_rm_element.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\dcm_rm_element.exe"
	-@erase "$(OUTDIR)\dcm_rm_element.ilk"
	-@erase "$(OUTDIR)\dcm_rm_element.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dcm_rm_element.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_rm_element\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_rm_element.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/dcm_rm_element.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/dcm_rm_element.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_rm_element.obj"

"$(OUTDIR)\dcm_rm_element.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dicom_echo - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "dicom_echo\Release"
# PROP BASE Intermediate_Dir "dicom_echo\Release"
# PROP BASE Target_Dir "dicom_echo"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "dicom_echo\Release"
# PROP Intermediate_Dir "dicom_echo\Release"
# PROP Target_Dir "dicom_echo"
OUTDIR=.\dicom_echo\Release
INTDIR=.\dicom_echo\Release

ALL : "$(OUTDIR)\dicom_echo.exe"

CLEAN : 
	-@erase "$(INTDIR)\dicom_echo.obj"
	-@erase "$(OUTDIR)\dicom_echo.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dicom_echo.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\dicom_echo\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dicom_echo.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/dicom_echo.pdb" /machine:I386 /out:"$(OUTDIR)/dicom_echo.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dicom_echo.obj"

"$(OUTDIR)\dicom_echo.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dicom_echo - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "dicom_echo\Debug"
# PROP BASE Intermediate_Dir "dicom_echo\Debug"
# PROP BASE Target_Dir "dicom_echo"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "dicom_echo\Debug"
# PROP Intermediate_Dir "dicom_echo\Debug"
# PROP Target_Dir "dicom_echo"
OUTDIR=.\dicom_echo\Debug
INTDIR=.\dicom_echo\Debug

ALL : "$(OUTDIR)\dicom_echo.exe"

CLEAN : 
	-@erase "$(INTDIR)\dicom_echo.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\dicom_echo.exe"
	-@erase "$(OUTDIR)\dicom_echo.ilk"
	-@erase "$(OUTDIR)\dicom_echo.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dicom_echo.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\dicom_echo\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dicom_echo.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/dicom_echo.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/dicom_echo.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dicom_echo.obj"

"$(OUTDIR)\dicom_echo.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_rm_group - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "dcm_rm_group\Release"
# PROP BASE Intermediate_Dir "dcm_rm_group\Release"
# PROP BASE Target_Dir "dcm_rm_group"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "dcm_rm_group\Release"
# PROP Intermediate_Dir "dcm_rm_group\Release"
# PROP Target_Dir "dcm_rm_group"
OUTDIR=.\dcm_rm_group\Release
INTDIR=.\dcm_rm_group\Release

ALL : "$(OUTDIR)\dcm_rm_group.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_rm_group.obj"
	-@erase "$(OUTDIR)\dcm_rm_group.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dcm_rm_group.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_rm_group\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_rm_group.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/dcm_rm_group.pdb" /machine:I386\
 /out:"$(OUTDIR)/dcm_rm_group.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_rm_group.obj"

"$(OUTDIR)\dcm_rm_group.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_rm_group - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "dcm_rm_group\Debug"
# PROP BASE Intermediate_Dir "dcm_rm_group\Debug"
# PROP BASE Target_Dir "dcm_rm_group"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "dcm_rm_group\Debug"
# PROP Intermediate_Dir "dcm_rm_group\Debug"
# PROP Target_Dir "dcm_rm_group"
OUTDIR=.\dcm_rm_group\Debug
INTDIR=.\dcm_rm_group\Debug

ALL : "$(OUTDIR)\dcm_rm_group.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_rm_group.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\dcm_rm_group.exe"
	-@erase "$(OUTDIR)\dcm_rm_group.ilk"
	-@erase "$(OUTDIR)\dcm_rm_group.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dcm_rm_group.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_rm_group\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_rm_group.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/dcm_rm_group.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/dcm_rm_group.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_rm_group.obj"

"$(OUTDIR)\dcm_rm_group.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "send_image - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "send_image\Release"
# PROP BASE Intermediate_Dir "send_image\Release"
# PROP BASE Target_Dir "send_image"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "send_image\Release"
# PROP Intermediate_Dir "send_image\Release"
# PROP Target_Dir "send_image"
OUTDIR=.\send_image\Release
INTDIR=.\send_image\Release

ALL : "$(OUTDIR)\send_image.exe"

CLEAN : 
	-@erase "$(INTDIR)\send_image.obj"
	-@erase "$(OUTDIR)\send_image.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/send_image.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\send_image\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/send_image.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/send_image.pdb" /machine:I386 /out:"$(OUTDIR)/send_image.exe" 
LINK32_OBJS= \
	"$(INTDIR)\send_image.obj"

"$(OUTDIR)\send_image.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "send_image - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "send_image\Debug"
# PROP BASE Intermediate_Dir "send_image\Debug"
# PROP BASE Target_Dir "send_image"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "send_image\Debug"
# PROP Intermediate_Dir "send_image\Debug"
# PROP Target_Dir "send_image"
OUTDIR=.\send_image\Debug
INTDIR=.\send_image\Debug

ALL : "$(OUTDIR)\send_image.exe"

CLEAN : 
	-@erase "$(INTDIR)\send_image.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\send_image.exe"
	-@erase "$(OUTDIR)\send_image.ilk"
	-@erase "$(OUTDIR)\send_image.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/send_image.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\send_image\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/send_image.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/send_image.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/send_image.exe" 
LINK32_OBJS= \
	"$(INTDIR)\send_image.obj"

"$(OUTDIR)\send_image.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "simple_storage - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "simple_storage\Release"
# PROP BASE Intermediate_Dir "simple_storage\Release"
# PROP BASE Target_Dir "simple_storage"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "simple_storage\Release"
# PROP Intermediate_Dir "simple_storage\Release"
# PROP Target_Dir "simple_storage"
OUTDIR=.\simple_storage\Release
INTDIR=.\simple_storage\Release

ALL : "$(OUTDIR)\simple_storage.exe"

CLEAN : 
	-@erase "$(INTDIR)\simple_storage.obj"
	-@erase "$(INTDIR)\sscond.obj"
	-@erase "$(OUTDIR)\simple_storage.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/simple_storage.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\simple_storage\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/simple_storage.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/simple_storage.pdb" /machine:I386\
 /out:"$(OUTDIR)/simple_storage.exe" 
LINK32_OBJS= \
	"$(INTDIR)\simple_storage.obj" \
	"$(INTDIR)\sscond.obj"

"$(OUTDIR)\simple_storage.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "simple_storage - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "simple_storage\Debug"
# PROP BASE Intermediate_Dir "simple_storage\Debug"
# PROP BASE Target_Dir "simple_storage"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "simple_storage/Debug"
# PROP Intermediate_Dir "simple_storage/Debug"
# PROP Target_Dir "simple_storage"
OUTDIR=.\simple_storage/Debug
INTDIR=.\simple_storage/Debug

ALL : "$(OUTDIR)\simple_storage.exe"

CLEAN : 
	-@erase "$(INTDIR)\simple_storage.obj"
	-@erase "$(INTDIR)\sscond.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\simple_storage.exe"
	-@erase "$(OUTDIR)\simple_storage.ilk"
	-@erase "$(OUTDIR)\simple_storage.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/simple_storage.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\simple_storage/Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/simple_storage.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/simple_storage.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/simple_storage.exe" 
LINK32_OBJS= \
	"$(INTDIR)\simple_storage.obj" \
	"$(INTDIR)\sscond.obj"

"$(OUTDIR)\simple_storage.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_w_disp - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "dcm_w_disp\Release"
# PROP BASE Intermediate_Dir "dcm_w_disp\Release"
# PROP BASE Target_Dir "dcm_w_disp"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "dcm_w_disp\Release"
# PROP Intermediate_Dir "dcm_w_disp\Release"
# PROP Target_Dir "dcm_w_disp"
OUTDIR=.\dcm_w_disp\Release
INTDIR=.\dcm_w_disp\Release

ALL : "$(OUTDIR)\dcm_w_disp.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_w_disp.obj"
	-@erase "$(INTDIR)\dcm_w_disp.res"
	-@erase "$(INTDIR)\image_utils.obj"
	-@erase "$(INTDIR)\windows_disp.obj"
	-@erase "$(OUTDIR)\dcm_w_disp.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/dcm_w_disp.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_w_disp\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/dcm_w_disp.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_w_disp.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/dcm_w_disp.pdb" /machine:I386 /out:"$(OUTDIR)/dcm_w_disp.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_w_disp.obj" \
	"$(INTDIR)\dcm_w_disp.res" \
	"$(INTDIR)\image_utils.obj" \
	"$(INTDIR)\windows_disp.obj"

"$(OUTDIR)\dcm_w_disp.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_w_disp - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "dcm_w_disp\Debug"
# PROP BASE Intermediate_Dir "dcm_w_disp\Debug"
# PROP BASE Target_Dir "dcm_w_disp"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "dcm_w_disp\Debug"
# PROP Intermediate_Dir "dcm_w_disp\Debug"
# PROP Target_Dir "dcm_w_disp"
OUTDIR=.\dcm_w_disp\Debug
INTDIR=.\dcm_w_disp\Debug

ALL : "$(OUTDIR)\dcm_w_disp.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_w_disp.obj"
	-@erase "$(INTDIR)\dcm_w_disp.res"
	-@erase "$(INTDIR)\image_utils.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\windows_disp.obj"
	-@erase "$(OUTDIR)\dcm_w_disp.exe"
	-@erase "$(OUTDIR)\dcm_w_disp.ilk"
	-@erase "$(OUTDIR)\dcm_w_disp.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/dcm_w_disp.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_w_disp\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/dcm_w_disp.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_w_disp.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/dcm_w_disp.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/dcm_w_disp.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_w_disp.obj" \
	"$(INTDIR)\dcm_w_disp.res" \
	"$(INTDIR)\image_utils.obj" \
	"$(INTDIR)\windows_disp.obj"

"$(OUTDIR)\dcm_w_disp.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "query_client - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "query_client\Release"
# PROP BASE Intermediate_Dir "query_client\Release"
# PROP BASE Target_Dir "query_client"
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "query_client\Release"
# PROP Intermediate_Dir "query_client\Release"
# PROP Target_Dir "query_client"
OUTDIR=.\query_client\Release
INTDIR=.\query_client\Release

ALL : "$(OUTDIR)\query_client.exe"

CLEAN : 
	-@erase "$(INTDIR)\format.obj"
	-@erase "$(INTDIR)\query_client.obj"
	-@erase "$(INTDIR)\query_client.pch"
	-@erase "$(INTDIR)\query_client.res"
	-@erase "$(INTDIR)\query_clientdlg.obj"
	-@erase "$(INTDIR)\query_interface.obj"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(OUTDIR)\query_client.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/query_client.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\query_client\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/query_client.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/query_client.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/query_client.pdb" /machine:I386\
 /out:"$(OUTDIR)/query_client.exe" 
LINK32_OBJS= \
	"$(INTDIR)\format.obj" \
	"$(INTDIR)\query_client.obj" \
	"$(INTDIR)\query_client.res" \
	"$(INTDIR)\query_clientdlg.obj" \
	"$(INTDIR)\query_interface.obj" \
	"$(INTDIR)\stdafx.obj"

"$(OUTDIR)\query_client.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "query_client - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "query_client\query_cl"
# PROP BASE Intermediate_Dir "query_client\query_cl"
# PROP BASE Target_Dir "query_client"
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "query_client\debug"
# PROP Intermediate_Dir "query_client\debug"
# PROP Target_Dir "query_client"
OUTDIR=.\query_client\debug
INTDIR=.\query_client\debug

ALL : "$(OUTDIR)\query_client.exe" "$(OUTDIR)\query_client.pch"

CLEAN : 
	-@erase "$(INTDIR)\format.obj"
	-@erase "$(INTDIR)\query_client.obj"
	-@erase "$(INTDIR)\query_client.pch"
	-@erase "$(INTDIR)\query_client.res"
	-@erase "$(INTDIR)\query_clientdlg.obj"
	-@erase "$(INTDIR)\query_interface.obj"
	-@erase "$(INTDIR)\stdafx.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\query_client.exe"
	-@erase "$(OUTDIR)\query_client.ilk"
	-@erase "$(OUTDIR)\query_client.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /c
# SUBTRACT CPP /YX /Yc /Yu
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\query_client\debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/query_client.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/query_client.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT LINK32 /incremental:no
LINK32_FLAGS=ctnlib.lib wsock32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/query_client.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/query_client.exe" 
LINK32_OBJS= \
	"$(INTDIR)\format.obj" \
	"$(INTDIR)\query_client.obj" \
	"$(INTDIR)\query_client.res" \
	"$(INTDIR)\query_clientdlg.obj" \
	"$(INTDIR)\query_interface.obj" \
	"$(INTDIR)\stdafx.obj"

"$(OUTDIR)\query_client.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_ctnto10 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "dcm_ctnto10\Release"
# PROP BASE Intermediate_Dir "dcm_ctnto10\Release"
# PROP BASE Target_Dir "dcm_ctnto10"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "dcm_ctnto10\Release"
# PROP Intermediate_Dir "dcm_ctnto10\Release"
# PROP Target_Dir "dcm_ctnto10"
OUTDIR=.\dcm_ctnto10\Release
INTDIR=.\dcm_ctnto10\Release

ALL : "$(OUTDIR)\dcm_ctnto10.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_ctnto10.obj"
	-@erase "$(OUTDIR)\dcm_ctnto10.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dcm_ctnto10.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_ctnto10\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_ctnto10.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/dcm_ctnto10.pdb" /machine:I386 /out:"$(OUTDIR)/dcm_ctnto10.exe"\
 
LINK32_OBJS= \
	"$(INTDIR)\dcm_ctnto10.obj"

"$(OUTDIR)\dcm_ctnto10.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_ctnto10 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "dcm_ctnto10\Debug"
# PROP BASE Intermediate_Dir "dcm_ctnto10\Debug"
# PROP BASE Target_Dir "dcm_ctnto10"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "dcm_ctnto10\Debug"
# PROP Intermediate_Dir "dcm_ctnto10\Debug"
# PROP Target_Dir "dcm_ctnto10"
OUTDIR=.\dcm_ctnto10\Debug
INTDIR=.\dcm_ctnto10\Debug

ALL : "$(OUTDIR)\dcm_ctnto10.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_ctnto10.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\dcm_ctnto10.exe"
	-@erase "$(OUTDIR)\dcm_ctnto10.ilk"
	-@erase "$(OUTDIR)\dcm_ctnto10.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dcm_ctnto10.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_ctnto10\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_ctnto10.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/dcm_ctnto10.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/dcm_ctnto10.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_ctnto10.obj"

"$(OUTDIR)\dcm_ctnto10.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_diff - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "dcm_diff\Release"
# PROP BASE Intermediate_Dir "dcm_diff\Release"
# PROP BASE Target_Dir "dcm_diff"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "dcm_diff\Release"
# PROP Intermediate_Dir "dcm_diff\Release"
# PROP Target_Dir "dcm_diff"
OUTDIR=.\dcm_diff\Release
INTDIR=.\dcm_diff\Release

ALL : "$(OUTDIR)\dcm_diff.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_diff.obj"
	-@erase "$(OUTDIR)\dcm_diff.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dcm_diff.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_diff\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_diff.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/dcm_diff.pdb" /machine:I386 /out:"$(OUTDIR)/dcm_diff.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_diff.obj"

"$(OUTDIR)\dcm_diff.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_diff - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "dcm_diff\Debug"
# PROP BASE Intermediate_Dir "dcm_diff\Debug"
# PROP BASE Target_Dir "dcm_diff"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "dcm_diff\Debug"
# PROP Intermediate_Dir "dcm_diff\Debug"
# PROP Target_Dir "dcm_diff"
OUTDIR=.\dcm_diff\Debug
INTDIR=.\dcm_diff\Debug

ALL : "$(OUTDIR)\dcm_diff.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_diff.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\dcm_diff.exe"
	-@erase "$(OUTDIR)\dcm_diff.ilk"
	-@erase "$(OUTDIR)\dcm_diff.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dcm_diff.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_diff\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_diff.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/dcm_diff.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/dcm_diff.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_diff.obj"

"$(OUTDIR)\dcm_diff.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_map_to_8 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "dcm_map_to_8\Release"
# PROP BASE Intermediate_Dir "dcm_map_to_8\Release"
# PROP BASE Target_Dir "dcm_map_to_8"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "dcm_map_to_8\Release"
# PROP Intermediate_Dir "dcm_map_to_8\Release"
# PROP Target_Dir "dcm_map_to_8"
OUTDIR=.\dcm_map_to_8\Release
INTDIR=.\dcm_map_to_8\Release

ALL : "$(OUTDIR)\dcm_map_to_8.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_map_to_8.obj"
	-@erase "$(OUTDIR)\dcm_map_to_8.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dcm_map_to_8.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_map_to_8\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_map_to_8.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/dcm_map_to_8.pdb" /machine:I386\
 /out:"$(OUTDIR)/dcm_map_to_8.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_map_to_8.obj"

"$(OUTDIR)\dcm_map_to_8.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_map_to_8 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "dcm_map_to_8\Debug"
# PROP BASE Intermediate_Dir "dcm_map_to_8\Debug"
# PROP BASE Target_Dir "dcm_map_to_8"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "dcm_map_to_8\Debug"
# PROP Intermediate_Dir "dcm_map_to_8\Debug"
# PROP Target_Dir "dcm_map_to_8"
OUTDIR=.\dcm_map_to_8\Debug
INTDIR=.\dcm_map_to_8\Debug

ALL : "$(OUTDIR)\dcm_map_to_8.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_map_to_8.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\dcm_map_to_8.exe"
	-@erase "$(OUTDIR)\dcm_map_to_8.ilk"
	-@erase "$(OUTDIR)\dcm_map_to_8.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dcm_map_to_8.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_map_to_8\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_map_to_8.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/dcm_map_to_8.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/dcm_map_to_8.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_map_to_8.obj"

"$(OUTDIR)\dcm_map_to_8.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_modify_object - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "dcm_modify_object\Release"
# PROP BASE Intermediate_Dir "dcm_modify_object\Release"
# PROP BASE Target_Dir "dcm_modify_object"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "dcm_modify_object\Release"
# PROP Intermediate_Dir "dcm_modify_object\Release"
# PROP Target_Dir "dcm_modify_object"
OUTDIR=.\dcm_modify_object\Release
INTDIR=.\dcm_modify_object\Release

ALL : "$(OUTDIR)\dcm_modify_object.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_modify_object.obj"
	-@erase "$(INTDIR)\gram_mod_w32.obj"
	-@erase "$(INTDIR)\lex_mod.obj"
	-@erase "$(OUTDIR)\dcm_modify_object.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dcm_modify_object.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_modify_object\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_modify_object.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/dcm_modify_object.pdb" /machine:I386\
 /out:"$(OUTDIR)/dcm_modify_object.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_modify_object.obj" \
	"$(INTDIR)\gram_mod_w32.obj" \
	"$(INTDIR)\lex_mod.obj"

"$(OUTDIR)\dcm_modify_object.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_modify_object - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "dcm_modify_object\Debug"
# PROP BASE Intermediate_Dir "dcm_modify_object\Debug"
# PROP BASE Target_Dir "dcm_modify_object"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "dcm_modify_object\Debug"
# PROP Intermediate_Dir "dcm_modify_object\Debug"
# PROP Target_Dir "dcm_modify_object"
OUTDIR=.\dcm_modify_object\Debug
INTDIR=.\dcm_modify_object\Debug

ALL : "$(OUTDIR)\dcm_modify_object.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_modify_object.obj"
	-@erase "$(INTDIR)\gram_mod_w32.obj"
	-@erase "$(INTDIR)\lex_mod.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\dcm_modify_object.exe"
	-@erase "$(OUTDIR)\dcm_modify_object.ilk"
	-@erase "$(OUTDIR)\dcm_modify_object.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dcm_modify_object.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_modify_object\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_modify_object.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/dcm_modify_object.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/dcm_modify_object.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_modify_object.obj" \
	"$(INTDIR)\gram_mod_w32.obj" \
	"$(INTDIR)\lex_mod.obj"

"$(OUTDIR)\dcm_modify_object.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_resize - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "dcm_resize\Release"
# PROP BASE Intermediate_Dir "dcm_resize\Release"
# PROP BASE Target_Dir "dcm_resize"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "dcm_resize\Release"
# PROP Intermediate_Dir "dcm_resize\Release"
# PROP Target_Dir "dcm_resize"
OUTDIR=.\dcm_resize\Release
INTDIR=.\dcm_resize\Release

ALL : "$(OUTDIR)\dcm_resize.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_resize.obj"
	-@erase "$(INTDIR)\pixavg.obj"
	-@erase "$(OUTDIR)\dcm_resize.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dcm_resize.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_resize\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_resize.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/dcm_resize.pdb" /machine:I386 /out:"$(OUTDIR)/dcm_resize.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_resize.obj" \
	"$(INTDIR)\pixavg.obj"

"$(OUTDIR)\dcm_resize.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_resize - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "dcm_resize\Debug"
# PROP BASE Intermediate_Dir "dcm_resize\Debug"
# PROP BASE Target_Dir "dcm_resize"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "dcm_resize\Debug"
# PROP Intermediate_Dir "dcm_resize\Debug"
# PROP Target_Dir "dcm_resize"
OUTDIR=.\dcm_resize\Debug
INTDIR=.\dcm_resize\Debug

ALL : "$(OUTDIR)\dcm_resize.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_resize.obj"
	-@erase "$(INTDIR)\pixavg.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\dcm_resize.exe"
	-@erase "$(OUTDIR)\dcm_resize.ilk"
	-@erase "$(OUTDIR)\dcm_resize.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dcm_resize.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_resize\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_resize.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/dcm_resize.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/dcm_resize.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_resize.obj" \
	"$(INTDIR)\pixavg.obj"

"$(OUTDIR)\dcm_resize.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_verify - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "dcm_verify\Release"
# PROP BASE Intermediate_Dir "dcm_verify\Release"
# PROP BASE Target_Dir "dcm_verify"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "dcm_verify\Release"
# PROP Intermediate_Dir "dcm_verify\Release"
# PROP Target_Dir "dcm_verify"
OUTDIR=.\dcm_verify\Release
INTDIR=.\dcm_verify\Release

ALL : "$(OUTDIR)\dcm_verify.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_verify.obj"
	-@erase "$(OUTDIR)\dcm_verify.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dcm_verify.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_verify\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_verify.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/dcm_verify.pdb" /machine:I386 /out:"$(OUTDIR)/dcm_verify.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_verify.obj"

"$(OUTDIR)\dcm_verify.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dcm_verify - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "dcm_verify\Debug"
# PROP BASE Intermediate_Dir "dcm_verify\Debug"
# PROP BASE Target_Dir "dcm_verify"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "dcm_verify\Debug"
# PROP Intermediate_Dir "dcm_verify\Debug"
# PROP Target_Dir "dcm_verify"
OUTDIR=.\dcm_verify\Debug
INTDIR=.\dcm_verify\Debug

ALL : "$(OUTDIR)\dcm_verify.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcm_verify.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\dcm_verify.exe"
	-@erase "$(OUTDIR)\dcm_verify.ilk"
	-@erase "$(OUTDIR)\dcm_verify.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/dcm_verify.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\dcm_verify\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dcm_verify.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/dcm_verify.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/dcm_verify.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcm_verify.obj"

"$(OUTDIR)\dcm_verify.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "print_server - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "print_server\Release"
# PROP BASE Intermediate_Dir "print_server\Release"
# PROP BASE Target_Dir "print_server"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "print_server\Release"
# PROP Intermediate_Dir "print_server\Release"
# PROP Target_Dir "print_server"
OUTDIR=.\print_server\Release
INTDIR=.\print_server\Release

ALL : "$(OUTDIR)\print_server.exe"

CLEAN : 
	-@erase "$(INTDIR)\action.obj"
	-@erase "$(INTDIR)\create.obj"
	-@erase "$(INTDIR)\delete.obj"
	-@erase "$(INTDIR)\get.obj"
	-@erase "$(INTDIR)\passociation.obj"
	-@erase "$(INTDIR)\prequests.obj"
	-@erase "$(INTDIR)\print_server.obj"
	-@erase "$(INTDIR)\psscond.obj"
	-@erase "$(INTDIR)\set.obj"
	-@erase "$(INTDIR)\utils.obj"
	-@erase "$(INTDIR)\verify.obj"
	-@erase "$(OUTDIR)\print_server.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/print_server.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\print_server\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/print_server.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/print_server.pdb" /machine:I386\
 /out:"$(OUTDIR)/print_server.exe" 
LINK32_OBJS= \
	"$(INTDIR)\action.obj" \
	"$(INTDIR)\create.obj" \
	"$(INTDIR)\delete.obj" \
	"$(INTDIR)\get.obj" \
	"$(INTDIR)\passociation.obj" \
	"$(INTDIR)\prequests.obj" \
	"$(INTDIR)\print_server.obj" \
	"$(INTDIR)\psscond.obj" \
	"$(INTDIR)\set.obj" \
	"$(INTDIR)\utils.obj" \
	"$(INTDIR)\verify.obj"

"$(OUTDIR)\print_server.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "print_server - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "print_server\print_se"
# PROP BASE Intermediate_Dir "print_server\print_se"
# PROP BASE Target_Dir "print_server"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "print_server\print_se"
# PROP Intermediate_Dir "print_server\print_se"
# PROP Target_Dir "print_server"
OUTDIR=.\print_server\print_se
INTDIR=.\print_server\print_se

ALL : ".\print_server\debug\print_server.exe"

CLEAN : 
	-@erase "$(INTDIR)\action.obj"
	-@erase "$(INTDIR)\create.obj"
	-@erase "$(INTDIR)\delete.obj"
	-@erase "$(INTDIR)\get.obj"
	-@erase "$(INTDIR)\passociation.obj"
	-@erase "$(INTDIR)\prequests.obj"
	-@erase "$(INTDIR)\print_server.obj"
	-@erase "$(INTDIR)\psscond.obj"
	-@erase "$(INTDIR)\set.obj"
	-@erase "$(INTDIR)\utils.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\verify.obj"
	-@erase "$(OUTDIR)\print_server.pdb"
	-@erase ".\print_server\debug\print_server.exe"
	-@erase ".\print_server\debug\print_server.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/print_server.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\print_server\print_se/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/print_server.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"print_server\debug/print_server.exe"
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/print_server.pdb" /debug /machine:I386\
 /out:"print_server\debug/print_server.exe" 
LINK32_OBJS= \
	"$(INTDIR)\action.obj" \
	"$(INTDIR)\create.obj" \
	"$(INTDIR)\delete.obj" \
	"$(INTDIR)\get.obj" \
	"$(INTDIR)\passociation.obj" \
	"$(INTDIR)\prequests.obj" \
	"$(INTDIR)\print_server.obj" \
	"$(INTDIR)\psscond.obj" \
	"$(INTDIR)\set.obj" \
	"$(INTDIR)\utils.obj" \
	"$(INTDIR)\verify.obj"

".\print_server\debug\print_server.exe" : "$(OUTDIR)" $(DEF_FILE)\
 $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "mwlQuery - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "mwlQuery\Release"
# PROP BASE Intermediate_Dir "mwlQuery\Release"
# PROP BASE Target_Dir "mwlQuery"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "mwlQuery\Release"
# PROP Intermediate_Dir "mwlQuery\Release"
# PROP Target_Dir "mwlQuery"
OUTDIR=.\mwlQuery\Release
INTDIR=.\mwlQuery\Release

ALL : "$(OUTDIR)\mwlQuery.exe"

CLEAN : 
	-@erase "$(INTDIR)\mwlQuery.obj"
	-@erase "$(OUTDIR)\mwlQuery.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/mwlQuery.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\mwlQuery\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/mwlQuery.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/mwlQuery.pdb" /machine:I386 /out:"$(OUTDIR)/mwlQuery.exe" 
LINK32_OBJS= \
	"$(INTDIR)\mwlQuery.obj"

"$(OUTDIR)\mwlQuery.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "mwlQuery - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "mwlQuery\Debug"
# PROP BASE Intermediate_Dir "mwlQuery\Debug"
# PROP BASE Target_Dir "mwlQuery"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "mwlQuery\Debug"
# PROP Intermediate_Dir "mwlQuery\Debug"
# PROP Target_Dir "mwlQuery"
OUTDIR=.\mwlQuery\Debug
INTDIR=.\mwlQuery\Debug

ALL : "$(OUTDIR)\mwlQuery.exe"

CLEAN : 
	-@erase "$(INTDIR)\mwlQuery.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\mwlQuery.exe"
	-@erase "$(OUTDIR)\mwlQuery.ilk"
	-@erase "$(OUTDIR)\mwlQuery.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/mwlQuery.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\mwlQuery\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/mwlQuery.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=ctnlib.lib wsock32.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/mwlQuery.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/mwlQuery.exe" 
LINK32_OBJS= \
	"$(INTDIR)\mwlQuery.obj"

"$(OUTDIR)\mwlQuery.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

################################################################################
# Begin Target

# Name "win32apps - Win32 Release"
# Name "win32apps - Win32 Debug"

!IF  "$(CFG)" == "win32apps - Win32 Release"

".\win32apps.exe" : 
   CD E:\prj\ctn\testRelease\apps
   NMAKE /f win32apps.mak

!ELSEIF  "$(CFG)" == "win32apps - Win32 Debug"

".\win32apps.exe" : 
   CD E:\prj\ctn\testRelease\apps
   NMAKE /f win32apps.mak

!ENDIF 

################################################################################
# Begin Project Dependency

# Project_Dep_Name "dcm_dump_element"

!IF  "$(CFG)" == "win32apps - Win32 Release"

"dcm_dump_element - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak"\
 CFG="dcm_dump_element - Win32 Release" 

!ELSEIF  "$(CFG)" == "win32apps - Win32 Debug"

"dcm_dump_element - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak"\
 CFG="dcm_dump_element - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "dcm_dump_file"

!IF  "$(CFG)" == "win32apps - Win32 Release"

"dcm_dump_file - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak"\
 CFG="dcm_dump_file - Win32 Release" 

!ELSEIF  "$(CFG)" == "win32apps - Win32 Debug"

"dcm_dump_file - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="dcm_dump_file - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Source File

SOURCE=.\win32apps\win32apps.c

!IF  "$(CFG)" == "win32apps - Win32 Release"

!ELSEIF  "$(CFG)" == "win32apps - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Project Dependency

# Project_Dep_Name "tbltest"

!IF  "$(CFG)" == "win32apps - Win32 Release"

"tbltest - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="tbltest - Win32 Release" 

!ELSEIF  "$(CFG)" == "win32apps - Win32 Debug"

"tbltest - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="tbltest - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "load_control"

!IF  "$(CFG)" == "win32apps - Win32 Release"

"load_control - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak"\
 CFG="load_control - Win32 Release" 

!ELSEIF  "$(CFG)" == "win32apps - Win32 Debug"

"load_control - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="load_control - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "archive_server"

!IF  "$(CFG)" == "win32apps - Win32 Release"

"archive_server - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak"\
 CFG="archive_server - Win32 Release" 

!ELSEIF  "$(CFG)" == "win32apps - Win32 Debug"

"archive_server - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak"\
 CFG="archive_server - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "dcm_make_object"

!IF  "$(CFG)" == "win32apps - Win32 Release"

"dcm_make_object - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak"\
 CFG="dcm_make_object - Win32 Release" 

!ELSEIF  "$(CFG)" == "win32apps - Win32 Debug"

"dcm_make_object - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak"\
 CFG="dcm_make_object - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "dcm_print_dictionary"

!IF  "$(CFG)" == "win32apps - Win32 Release"

"dcm_print_dictionary - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak"\
 CFG="dcm_print_dictionary - Win32 Release" 

!ELSEIF  "$(CFG)" == "win32apps - Win32 Debug"

"dcm_print_dictionary - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak"\
 CFG="dcm_print_dictionary - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "dcm_modify_elements"

!IF  "$(CFG)" == "win32apps - Win32 Release"

"dcm_modify_elements - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak"\
 CFG="dcm_modify_elements - Win32 Release" 

!ELSEIF  "$(CFG)" == "win32apps - Win32 Debug"

"dcm_modify_elements - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak"\
 CFG="dcm_modify_elements - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "dcm_rm_element"

!IF  "$(CFG)" == "win32apps - Win32 Release"

"dcm_rm_element - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak"\
 CFG="dcm_rm_element - Win32 Release" 

!ELSEIF  "$(CFG)" == "win32apps - Win32 Debug"

"dcm_rm_element - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak"\
 CFG="dcm_rm_element - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "dicom_echo"

!IF  "$(CFG)" == "win32apps - Win32 Release"

"dicom_echo - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="dicom_echo - Win32 Release" 

!ELSEIF  "$(CFG)" == "win32apps - Win32 Debug"

"dicom_echo - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="dicom_echo - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "dcm_rm_group"

!IF  "$(CFG)" == "win32apps - Win32 Release"

"dcm_rm_group - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak"\
 CFG="dcm_rm_group - Win32 Release" 

!ELSEIF  "$(CFG)" == "win32apps - Win32 Debug"

"dcm_rm_group - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="dcm_rm_group - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "send_image"

!IF  "$(CFG)" == "win32apps - Win32 Release"

"send_image - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="send_image - Win32 Release" 

!ELSEIF  "$(CFG)" == "win32apps - Win32 Debug"

"send_image - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="send_image - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "simple_storage"

!IF  "$(CFG)" == "win32apps - Win32 Release"

"simple_storage - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak"\
 CFG="simple_storage - Win32 Release" 

!ELSEIF  "$(CFG)" == "win32apps - Win32 Debug"

"simple_storage - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak"\
 CFG="simple_storage - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "dcm_w_disp"

!IF  "$(CFG)" == "win32apps - Win32 Release"

"dcm_w_disp - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="dcm_w_disp - Win32 Release" 

!ELSEIF  "$(CFG)" == "win32apps - Win32 Debug"

"dcm_w_disp - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="dcm_w_disp - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "query_client"

!IF  "$(CFG)" == "win32apps - Win32 Release"

"query_client - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak"\
 CFG="query_client - Win32 Release" 

!ELSEIF  "$(CFG)" == "win32apps - Win32 Debug"

"query_client - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="query_client - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "dcm_ctnto10"

!IF  "$(CFG)" == "win32apps - Win32 Release"

"dcm_ctnto10 - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="dcm_ctnto10 - Win32 Release" 

!ELSEIF  "$(CFG)" == "win32apps - Win32 Debug"

"dcm_ctnto10 - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="dcm_ctnto10 - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "dcm_diff"

!IF  "$(CFG)" == "win32apps - Win32 Release"

"dcm_diff - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="dcm_diff - Win32 Release" 

!ELSEIF  "$(CFG)" == "win32apps - Win32 Debug"

"dcm_diff - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="dcm_diff - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "dcm_map_to_8"

!IF  "$(CFG)" == "win32apps - Win32 Release"

"dcm_map_to_8 - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak"\
 CFG="dcm_map_to_8 - Win32 Release" 

!ELSEIF  "$(CFG)" == "win32apps - Win32 Debug"

"dcm_map_to_8 - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="dcm_map_to_8 - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "dcm_modify_object"

!IF  "$(CFG)" == "win32apps - Win32 Release"

"dcm_modify_object - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak"\
 CFG="dcm_modify_object - Win32 Release" 

!ELSEIF  "$(CFG)" == "win32apps - Win32 Debug"

"dcm_modify_object - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak"\
 CFG="dcm_modify_object - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "dcm_resize"

!IF  "$(CFG)" == "win32apps - Win32 Release"

"dcm_resize - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="dcm_resize - Win32 Release" 

!ELSEIF  "$(CFG)" == "win32apps - Win32 Debug"

"dcm_resize - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="dcm_resize - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "dcm_verify"

!IF  "$(CFG)" == "win32apps - Win32 Release"

"dcm_verify - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="dcm_verify - Win32 Release" 

!ELSEIF  "$(CFG)" == "win32apps - Win32 Debug"

"dcm_verify - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="dcm_verify - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "print_server"

!IF  "$(CFG)" == "win32apps - Win32 Release"

"print_server - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak"\
 CFG="print_server - Win32 Release" 

!ELSEIF  "$(CFG)" == "win32apps - Win32 Debug"

"print_server - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="print_server - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "mwlQuery"

!IF  "$(CFG)" == "win32apps - Win32 Release"

"mwlQuery - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="mwlQuery - Win32 Release" 

!ELSEIF  "$(CFG)" == "win32apps - Win32 Debug"

"mwlQuery - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="mwlQuery - Win32 Debug" 

!ENDIF 

# End Project Dependency
# End Target
################################################################################
# Begin Target

# Name "dcm_dump_element - Win32 Release"
# Name "dcm_dump_element - Win32 Debug"

!IF  "$(CFG)" == "dcm_dump_element - Win32 Release"

!ELSEIF  "$(CFG)" == "dcm_dump_element - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\dcm_dump_element\dcm_dump_element.c
DEP_CPP_DCM_D=\
	"..\include\dicom_platform.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dcm_dump_element.obj" : $(SOURCE) $(DEP_CPP_DCM_D) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
################################################################################
# Begin Target

# Name "dcm_dump_file - Win32 Release"
# Name "dcm_dump_file - Win32 Debug"

!IF  "$(CFG)" == "dcm_dump_file - Win32 Release"

!ELSEIF  "$(CFG)" == "dcm_dump_file - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\dcm_dump_file\dcm_dump_file.c
DEP_CPP_DCM_DU=\
	"..\include\dicom_platform.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dcm_dump_file.obj" : $(SOURCE) $(DEP_CPP_DCM_DU) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
################################################################################
# Begin Target

# Name "tbltest - Win32 Release"
# Name "tbltest - Win32 Debug"

!IF  "$(CFG)" == "tbltest - Win32 Release"

!ELSEIF  "$(CFG)" == "tbltest - Win32 Debug"

!ENDIF 

################################################################################
# Begin Project Dependency

# Project_Dep_Name "ttinsert"

!IF  "$(CFG)" == "tbltest - Win32 Release"

"ttinsert - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="ttinsert - Win32 Release" 

!ELSEIF  "$(CFG)" == "tbltest - Win32 Debug"

"ttinsert - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="ttinsert - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "ttselect"

!IF  "$(CFG)" == "tbltest - Win32 Release"

"ttselect - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="ttselect - Win32 Release" 

!ELSEIF  "$(CFG)" == "tbltest - Win32 Debug"

"ttselect - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="ttselect - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "ttupdate"

!IF  "$(CFG)" == "tbltest - Win32 Release"

"ttupdate - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="ttupdate - Win32 Release" 

!ELSEIF  "$(CFG)" == "tbltest - Win32 Debug"

"ttupdate - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="ttupdate - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "ttdelete"

!IF  "$(CFG)" == "tbltest - Win32 Release"

"ttdelete - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="ttdelete - Win32 Release" 

!ELSEIF  "$(CFG)" == "tbltest - Win32 Debug"

"ttdelete - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="ttdelete - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "ttunique"

!IF  "$(CFG)" == "tbltest - Win32 Release"

"ttunique - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="ttunique - Win32 Release" 

!ELSEIF  "$(CFG)" == "tbltest - Win32 Debug"

"ttunique - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="ttunique - Win32 Debug" 

!ENDIF 

# End Project Dependency
# End Target
################################################################################
# Begin Target

# Name "ttinsert - Win32 Release"
# Name "ttinsert - Win32 Debug"

!IF  "$(CFG)" == "ttinsert - Win32 Release"

!ELSEIF  "$(CFG)" == "ttinsert - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\tbltest\ttinsert.c
DEP_CPP_TTINS=\
	"..\include\dicom_platform.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\tbl.h"\
	

"$(INTDIR)\ttinsert.obj" : $(SOURCE) $(DEP_CPP_TTINS) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
################################################################################
# Begin Target

# Name "ttselect - Win32 Release"
# Name "ttselect - Win32 Debug"

!IF  "$(CFG)" == "ttselect - Win32 Release"

!ELSEIF  "$(CFG)" == "ttselect - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\tbltest\ttselect.c
DEP_CPP_TTSEL=\
	"..\include\dicom_platform.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\tbl.h"\
	

"$(INTDIR)\ttselect.obj" : $(SOURCE) $(DEP_CPP_TTSEL) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
################################################################################
# Begin Target

# Name "ttupdate - Win32 Release"
# Name "ttupdate - Win32 Debug"

!IF  "$(CFG)" == "ttupdate - Win32 Release"

!ELSEIF  "$(CFG)" == "ttupdate - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\tbltest\ttupdate.c
DEP_CPP_TTUPD=\
	"..\include\dicom_platform.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\tbl.h"\
	

"$(INTDIR)\ttupdate.obj" : $(SOURCE) $(DEP_CPP_TTUPD) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
################################################################################
# Begin Target

# Name "ttdelete - Win32 Release"
# Name "ttdelete - Win32 Debug"

!IF  "$(CFG)" == "ttdelete - Win32 Release"

!ELSEIF  "$(CFG)" == "ttdelete - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\tbltest\ttdelete.c
DEP_CPP_TTDEL=\
	"..\include\dicom_platform.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\tbl.h"\
	

"$(INTDIR)\ttdelete.obj" : $(SOURCE) $(DEP_CPP_TTDEL) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Project Dependency

# Project_Dep_Name "ttlayout"

!IF  "$(CFG)" == "ttdelete - Win32 Release"

"ttlayout - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="ttlayout - Win32 Release" 

!ELSEIF  "$(CFG)" == "ttdelete - Win32 Debug"

"ttlayout - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\win32apps.mak" CFG="ttlayout - Win32 Debug" 

!ENDIF 

# End Project Dependency
# End Target
################################################################################
# Begin Target

# Name "ttunique - Win32 Release"
# Name "ttunique - Win32 Debug"

!IF  "$(CFG)" == "ttunique - Win32 Release"

!ELSEIF  "$(CFG)" == "ttunique - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\tbltest\ttunique.c
DEP_CPP_TTUNI=\
	"..\include\dicom_platform.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\tbl.h"\
	

"$(INTDIR)\ttunique.obj" : $(SOURCE) $(DEP_CPP_TTUNI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
################################################################################
# Begin Target

# Name "ttlayout - Win32 Release"
# Name "ttlayout - Win32 Debug"

!IF  "$(CFG)" == "ttlayout - Win32 Release"

!ELSEIF  "$(CFG)" == "ttlayout - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\tbltest\ttlayout.c

!IF  "$(CFG)" == "ttlayout - Win32 Release"

DEP_CPP_TTLAY=\
	"..\include\dicom_platform.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\tbl.h"\
	

"$(INTDIR)\ttlayout.obj" : $(SOURCE) $(DEP_CPP_TTLAY) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ttlayout - Win32 Debug"

DEP_CPP_TTLAY=\
	"..\include\dicom_platform.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\tbl.h"\
	

"$(INTDIR)\ttlayout.obj" : $(SOURCE) $(DEP_CPP_TTLAY) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "load_control - Win32 Release"
# Name "load_control - Win32 Debug"

!IF  "$(CFG)" == "load_control - Win32 Release"

!ELSEIF  "$(CFG)" == "load_control - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\load_control\load_control.c
DEP_CPP_LOAD_=\
	"..\include\dicom_platform.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\idb.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\manage.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\tbl.h"\
	

"$(INTDIR)\load_control.obj" : $(SOURCE) $(DEP_CPP_LOAD_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
################################################################################
# Begin Target

# Name "archive_server - Win32 Release"
# Name "archive_server - Win32 Debug"

!IF  "$(CFG)" == "archive_server - Win32 Release"

!ELSEIF  "$(CFG)" == "archive_server - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\image_archive\cget.c
DEP_CPP_CGET_=\
	"..\include\dicom_platform.h"\
	".\image_archive\cget.h"\
	".\image_archive\image_archive.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_ie.h"\
	{$(INCLUDE)}"\dicom_messages.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_services.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\dulprotocol.h"\
	{$(INCLUDE)}"\idb.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\manage.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\tbl.h"\
	

"$(INTDIR)\cget.obj" : $(SOURCE) $(DEP_CPP_CGET_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\image_archive\association.c
DEP_CPP_ASSOC=\
	"..\include\dicom_platform.h"\
	".\image_archive\image_archive.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_messages.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_services.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\dulprotocol.h"\
	{$(INCLUDE)}"\idb.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\manage.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\tbl.h"\
	

"$(INTDIR)\association.obj" : $(SOURCE) $(DEP_CPP_ASSOC) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\image_archive\archive_server.c
DEP_CPP_ARCHI=\
	"..\include\dicom_platform.h"\
	".\image_archive\image_archive.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_messages.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_services.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\dulprotocol.h"\
	{$(INCLUDE)}"\idb.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\manage.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\tbl.h"\
	{$(INCLUDE)}"\utility.h"\
	

"$(INTDIR)\archive_server.obj" : $(SOURCE) $(DEP_CPP_ARCHI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\image_archive\copy.c
DEP_CPP_COPY_=\
	"..\include\dicom_platform.h"\
	".\image_archive\copy.h"\
	".\image_archive\image_archive.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dulprotocol.h"\
	{$(INCLUDE)}"\icpy.h"\
	{$(INCLUDE)}"\idb.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\manage.h"\
	{$(INCLUDE)}"\tbl.h"\
	

"$(INTDIR)\copy.obj" : $(SOURCE) $(DEP_CPP_COPY_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\image_archive\sscond.c
DEP_CPP_SSCON=\
	"..\include\dicom_platform.h"\
	".\image_archive\image_archive.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_messages.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_services.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\dulprotocol.h"\
	{$(INCLUDE)}"\idb.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\manage.h"\
	{$(INCLUDE)}"\tbl.h"\
	

"$(INTDIR)\sscond.obj" : $(SOURCE) $(DEP_CPP_SSCON) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\image_archive\move.c
DEP_CPP_MOVE_=\
	"..\include\dbquery.h"\
	"..\include\dicom_platform.h"\
	".\image_archive\image_archive.h"\
	".\image_archive\move.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_ie.h"\
	{$(INCLUDE)}"\dicom_messages.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_services.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\dulprotocol.h"\
	{$(INCLUDE)}"\iap.h"\
	{$(INCLUDE)}"\idb.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\manage.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\tbl.h"\
	

"$(INTDIR)\move.obj" : $(SOURCE) $(DEP_CPP_MOVE_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\image_archive\naction.c
DEP_CPP_NACTI=\
	"..\include\dicom_platform.h"\
	".\image_archive\image_archive.h"\
	".\image_archive\naction.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_messages.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_services.h"\
	{$(INCLUDE)}"\dicom_sq.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\dulprotocol.h"\
	{$(INCLUDE)}"\fis.h"\
	{$(INCLUDE)}"\idb.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\manage.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\tbl.h"\
	{$(INCLUDE)}"\utility.h"\
	

"$(INTDIR)\naction.obj" : $(SOURCE) $(DEP_CPP_NACTI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\image_archive\parse.c
DEP_CPP_PARSE=\
	"..\include\dicom_platform.h"\
	".\image_archive\image_archive.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\dulprotocol.h"\
	{$(INCLUDE)}"\idb.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\manage.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\tbl.h"\
	

"$(INTDIR)\parse.obj" : $(SOURCE) $(DEP_CPP_PARSE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\image_archive\requests.c
DEP_CPP_REQUE=\
	"..\include\dicom_platform.h"\
	".\image_archive\cget.h"\
	".\image_archive\copy.h"\
	".\image_archive\find.h"\
	".\image_archive\image_archive.h"\
	".\image_archive\move.h"\
	".\image_archive\naction.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_ie.h"\
	{$(INCLUDE)}"\dicom_messages.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_services.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\dulprotocol.h"\
	{$(INCLUDE)}"\fis.h"\
	{$(INCLUDE)}"\idb.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\manage.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\tbl.h"\
	

"$(INTDIR)\requests.obj" : $(SOURCE) $(DEP_CPP_REQUE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\image_archive\find.c
DEP_CPP_FIND_=\
	"..\include\dicom_platform.h"\
	".\image_archive\find.h"\
	".\image_archive\image_archive.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_ie.h"\
	{$(INCLUDE)}"\dicom_messages.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_services.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\dulprotocol.h"\
	{$(INCLUDE)}"\idb.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\manage.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\tbl.h"\
	

"$(INTDIR)\find.obj" : $(SOURCE) $(DEP_CPP_FIND_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
################################################################################
# Begin Target

# Name "dcm_make_object - Win32 Release"
# Name "dcm_make_object - Win32 Debug"

!IF  "$(CFG)" == "dcm_make_object - Win32 Release"

!ELSEIF  "$(CFG)" == "dcm_make_object - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\dcm_make_object\dcm_make_object.c
DEP_CPP_DCM_M=\
	"..\include\dicom_platform.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dcm_make_object.obj" : $(SOURCE) $(DEP_CPP_DCM_M) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
################################################################################
# Begin Target

# Name "dcm_print_dictionary - Win32 Release"
# Name "dcm_print_dictionary - Win32 Debug"

!IF  "$(CFG)" == "dcm_print_dictionary - Win32 Release"

!ELSEIF  "$(CFG)" == "dcm_print_dictionary - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\dcm_print_dictionary\dcm_print_dictionary.c
DEP_CPP_DCM_P=\
	"..\include\dicom_platform.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dcm_print_dictionary.obj" : $(SOURCE) $(DEP_CPP_DCM_P) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
################################################################################
# Begin Target

# Name "dcm_modify_elements - Win32 Release"
# Name "dcm_modify_elements - Win32 Debug"

!IF  "$(CFG)" == "dcm_modify_elements - Win32 Release"

!ELSEIF  "$(CFG)" == "dcm_modify_elements - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\dcm_modify_elements\dcm_modify_elements.c
DEP_CPP_DCM_MO=\
	"..\include\dicom_platform.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dcm_modify_elements.obj" : $(SOURCE) $(DEP_CPP_DCM_MO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
################################################################################
# Begin Target

# Name "dcm_rm_element - Win32 Release"
# Name "dcm_rm_element - Win32 Debug"

!IF  "$(CFG)" == "dcm_rm_element - Win32 Release"

!ELSEIF  "$(CFG)" == "dcm_rm_element - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\dcm_rm_element\dcm_rm_element.c
DEP_CPP_DCM_R=\
	"..\include\dicom_platform.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dcm_rm_element.obj" : $(SOURCE) $(DEP_CPP_DCM_R) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
################################################################################
# Begin Target

# Name "dicom_echo - Win32 Release"
# Name "dicom_echo - Win32 Debug"

!IF  "$(CFG)" == "dicom_echo - Win32 Release"

!ELSEIF  "$(CFG)" == "dicom_echo - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\dicom_echo\dicom_echo.c
DEP_CPP_DICOM=\
	"..\include\dicom_platform.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_messages.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_services.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\dulprotocol.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dicom_echo.obj" : $(SOURCE) $(DEP_CPP_DICOM) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
################################################################################
# Begin Target

# Name "dcm_rm_group - Win32 Release"
# Name "dcm_rm_group - Win32 Debug"

!IF  "$(CFG)" == "dcm_rm_group - Win32 Release"

!ELSEIF  "$(CFG)" == "dcm_rm_group - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\dcm_rm_group\dcm_rm_group.c
DEP_CPP_DCM_RM=\
	"..\include\dicom_platform.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dcm_rm_group.obj" : $(SOURCE) $(DEP_CPP_DCM_RM) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
################################################################################
# Begin Target

# Name "send_image - Win32 Release"
# Name "send_image - Win32 Debug"

!IF  "$(CFG)" == "send_image - Win32 Release"

!ELSEIF  "$(CFG)" == "send_image - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\send_image\send_image.c
DEP_CPP_SEND_=\
	"..\include\dicom_platform.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_messages.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_services.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\dulprotocol.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\utility.h"\
	

"$(INTDIR)\send_image.obj" : $(SOURCE) $(DEP_CPP_SEND_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
################################################################################
# Begin Target

# Name "simple_storage - Win32 Release"
# Name "simple_storage - Win32 Debug"

!IF  "$(CFG)" == "simple_storage - Win32 Release"

!ELSEIF  "$(CFG)" == "simple_storage - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\simple_storage\simple_storage.c
DEP_CPP_SIMPL=\
	"..\include\dbquery.h"\
	"..\include\dicom_platform.h"\
	".\simple_storage\simple_storage.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_messages.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_services.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\dulprotocol.h"\
	{$(INCLUDE)}"\iap.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\utility.h"\
	

"$(INTDIR)\simple_storage.obj" : $(SOURCE) $(DEP_CPP_SIMPL) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\simple_storage\sscond.c
DEP_CPP_SSCON=\
	"..\include\dicom_platform.h"\
	".\simple_storage\simple_storage.h"\
	{$(INCLUDE)}"\dicom.h"\
	

"$(INTDIR)\sscond.obj" : $(SOURCE) $(DEP_CPP_SSCON) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
################################################################################
# Begin Target

# Name "dcm_w_disp - Win32 Release"
# Name "dcm_w_disp - Win32 Debug"

!IF  "$(CFG)" == "dcm_w_disp - Win32 Release"

!ELSEIF  "$(CFG)" == "dcm_w_disp - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\dcm_w_disp\dcm_w_disp.rc
DEP_RSC_DCM_W=\
	".\dcm_w_disp\menu_windows.h"\
	

!IF  "$(CFG)" == "dcm_w_disp - Win32 Release"


"$(INTDIR)\dcm_w_disp.res" : $(SOURCE) $(DEP_RSC_DCM_W) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/dcm_w_disp.res" /i "dcm_w_disp" /d "NDEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "dcm_w_disp - Win32 Debug"


"$(INTDIR)\dcm_w_disp.res" : $(SOURCE) $(DEP_RSC_DCM_W) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/dcm_w_disp.res" /i "dcm_w_disp" /d "_DEBUG"\
 $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dcm_w_disp\image_utils.c
DEP_CPP_IMAGE=\
	"..\include\dicom_platform.h"\
	".\dcm_w_disp\common_disp.h"\
	".\dcm_w_disp\ctndisp.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\image_utils.obj" : $(SOURCE) $(DEP_CPP_IMAGE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dcm_w_disp\dcm_w_disp.c
DEP_CPP_DCM_W_=\
	".\dcm_w_disp\menu_windows.h"\
	

"$(INTDIR)\dcm_w_disp.obj" : $(SOURCE) $(DEP_CPP_DCM_W_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dcm_w_disp\windows_disp.c
DEP_CPP_WINDO=\
	"..\include\dicom_platform.h"\
	".\dcm_w_disp\common_disp.h"\
	".\dcm_w_disp\ctndisp.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\lst.h"\
	

"$(INTDIR)\windows_disp.obj" : $(SOURCE) $(DEP_CPP_WINDO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
################################################################################
# Begin Target

# Name "query_client - Win32 Release"
# Name "query_client - Win32 Debug"

!IF  "$(CFG)" == "query_client - Win32 Release"

!ELSEIF  "$(CFG)" == "query_client - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\query_client\readme.txt

!IF  "$(CFG)" == "query_client - Win32 Release"

!ELSEIF  "$(CFG)" == "query_client - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\query_client\query_client.cpp
DEP_CPP_QUERY=\
	".\query_client\query_client.h"\
	".\query_client\query_clientDlg.h"\
	".\query_client\StdAfx.h"\
	

"$(INTDIR)\query_client.obj" : $(SOURCE) $(DEP_CPP_QUERY) "$(INTDIR)"\
 "$(INTDIR)\query_client.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\query_client\query_clientdlg.cpp
DEP_CPP_QUERY_=\
	"..\include\dicom_platform.h"\
	".\query_client\format.h"\
	".\query_client\query_client.h"\
	".\query_client\query_clientDlg.h"\
	".\query_client\query_interface.h"\
	".\query_client\StdAfx.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\lst.h"\
	

"$(INTDIR)\query_clientdlg.obj" : $(SOURCE) $(DEP_CPP_QUERY_) "$(INTDIR)"\
 "$(INTDIR)\query_client.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\query_client\stdafx.cpp
DEP_CPP_STDAF=\
	".\query_client\StdAfx.h"\
	

!IF  "$(CFG)" == "query_client - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/query_client.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\query_client.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "query_client - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)/query_client.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\query_client.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\query_client\query_client.rc
DEP_RSC_QUERY_C=\
	".\query_client\res\query_client.ico"\
	".\query_client\res\query_client.rc2"\
	

!IF  "$(CFG)" == "query_client - Win32 Release"


"$(INTDIR)\query_client.res" : $(SOURCE) $(DEP_RSC_QUERY_C) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/query_client.res" /i "query_client" /d\
 "NDEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "query_client - Win32 Debug"


"$(INTDIR)\query_client.res" : $(SOURCE) $(DEP_RSC_QUERY_C) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/query_client.res" /i "query_client" /d\
 "_DEBUG" $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\query_client\query_interface.c
DEP_CPP_QUERY_I=\
	"..\include\dicom_platform.h"\
	".\query_client\format.h"\
	".\query_client\query_interface.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_messages.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_services.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\dulprotocol.h"\
	{$(INCLUDE)}"\lst.h"\
	

"$(INTDIR)\query_interface.obj" : $(SOURCE) $(DEP_CPP_QUERY_I) "$(INTDIR)"\
 "$(INTDIR)\query_client.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\query_client\format.c
DEP_CPP_FORMA=\
	"..\include\dicom_platform.h"\
	".\query_client\query_interface.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\lst.h"\
	

"$(INTDIR)\format.obj" : $(SOURCE) $(DEP_CPP_FORMA) "$(INTDIR)"\
 "$(INTDIR)\query_client.pch"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
################################################################################
# Begin Target

# Name "dcm_ctnto10 - Win32 Release"
# Name "dcm_ctnto10 - Win32 Debug"

!IF  "$(CFG)" == "dcm_ctnto10 - Win32 Release"

!ELSEIF  "$(CFG)" == "dcm_ctnto10 - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\dcm_ctnto10\dcm_ctnto10.c
DEP_CPP_DCM_C=\
	"..\include\dicom_platform.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dcm_ctnto10.obj" : $(SOURCE) $(DEP_CPP_DCM_C) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
################################################################################
# Begin Target

# Name "dcm_diff - Win32 Release"
# Name "dcm_diff - Win32 Debug"

!IF  "$(CFG)" == "dcm_diff - Win32 Release"

!ELSEIF  "$(CFG)" == "dcm_diff - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\dcm_diff\dcm_diff.c
DEP_CPP_DCM_DI=\
	"..\include\dicom_platform.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dcm_diff.obj" : $(SOURCE) $(DEP_CPP_DCM_DI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
################################################################################
# Begin Target

# Name "dcm_map_to_8 - Win32 Release"
# Name "dcm_map_to_8 - Win32 Debug"

!IF  "$(CFG)" == "dcm_map_to_8 - Win32 Release"

!ELSEIF  "$(CFG)" == "dcm_map_to_8 - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\dcm_map_to_8\dcm_map_to_8.c
DEP_CPP_DCM_MA=\
	"..\include\dicom_platform.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\lst.h"\
	

"$(INTDIR)\dcm_map_to_8.obj" : $(SOURCE) $(DEP_CPP_DCM_MA) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
################################################################################
# Begin Target

# Name "dcm_modify_object - Win32 Release"
# Name "dcm_modify_object - Win32 Debug"

!IF  "$(CFG)" == "dcm_modify_object - Win32 Release"

!ELSEIF  "$(CFG)" == "dcm_modify_object - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\dcm_modify_object\dcm_modify_object.c
DEP_CPP_DCM_MOD=\
	"..\include\dicom_platform.h"\
	".\dcm_modify_object\modify.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dcm_modify_object.obj" : $(SOURCE) $(DEP_CPP_DCM_MOD) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dcm_modify_object\lex_mod.c
DEP_CPP_LEX_M=\
	"..\include\dicom_platform.h"\
	".\dcm_modify_object\gram.h"\
	".\dcm_modify_object\modify.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\lex_mod.obj" : $(SOURCE) $(DEP_CPP_LEX_M) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dcm_modify_object\gram_mod_w32.c
DEP_CPP_GRAM_=\
	"..\include\dicom_platform.h"\
	".\dcm_modify_object\modify.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\gram_mod_w32.obj" : $(SOURCE) $(DEP_CPP_GRAM_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
################################################################################
# Begin Target

# Name "dcm_resize - Win32 Release"
# Name "dcm_resize - Win32 Debug"

!IF  "$(CFG)" == "dcm_resize - Win32 Release"

!ELSEIF  "$(CFG)" == "dcm_resize - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\dcm_resize\pixavg.c
DEP_CPP_PIXAV=\
	"..\include\dicom_platform.h"\
	".\dcm_resize\pixavg.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\pixavg.obj" : $(SOURCE) $(DEP_CPP_PIXAV) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dcm_resize\dcm_resize.c
DEP_CPP_DCM_RE=\
	"..\include\dicom_platform.h"\
	".\dcm_resize\pixavg.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\lst.h"\
	

"$(INTDIR)\dcm_resize.obj" : $(SOURCE) $(DEP_CPP_DCM_RE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
################################################################################
# Begin Target

# Name "dcm_verify - Win32 Release"
# Name "dcm_verify - Win32 Debug"

!IF  "$(CFG)" == "dcm_verify - Win32 Release"

!ELSEIF  "$(CFG)" == "dcm_verify - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\dcm_verify\dcm_verify.c
DEP_CPP_DCM_V=\
	"..\include\dicom_platform.h"\
	".\dcm_verify\dcm_verify.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_ie.h"\
	{$(INCLUDE)}"\dicom_messages.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\dcm_verify.obj" : $(SOURCE) $(DEP_CPP_DCM_V) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
################################################################################
# Begin Target

# Name "print_server - Win32 Release"
# Name "print_server - Win32 Debug"

!IF  "$(CFG)" == "print_server - Win32 Release"

!ELSEIF  "$(CFG)" == "print_server - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\print_server\print_server.c
DEP_CPP_PRINT=\
	"..\include\dicom_platform.h"\
	".\print_server\print_server.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_messages.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_print.h"\
	{$(INCLUDE)}"\dicom_services.h"\
	{$(INCLUDE)}"\dicom_sq.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\dulprotocol.h"\
	{$(INCLUDE)}"\gq.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\manage.h"\
	{$(INCLUDE)}"\print_queue.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\tbl.h"\
	

"$(INTDIR)\print_server.obj" : $(SOURCE) $(DEP_CPP_PRINT) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\print_server\create.c
DEP_CPP_CREAT=\
	"..\include\dicom_platform.h"\
	".\print_server\print_server.h"\
	".\print_server\private.h"\
	".\print_server\static_variables.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_messages.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_print.h"\
	{$(INCLUDE)}"\dicom_services.h"\
	{$(INCLUDE)}"\dicom_sq.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\dulprotocol.h"\
	{$(INCLUDE)}"\gq.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\manage.h"\
	{$(INCLUDE)}"\print_queue.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\tbl.h"\
	

"$(INTDIR)\create.obj" : $(SOURCE) $(DEP_CPP_CREAT) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\print_server\delete.c
DEP_CPP_DELET=\
	"..\include\dicom_platform.h"\
	".\print_server\print_server.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_messages.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_print.h"\
	{$(INCLUDE)}"\dicom_services.h"\
	{$(INCLUDE)}"\dicom_sq.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\dulprotocol.h"\
	{$(INCLUDE)}"\gq.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\manage.h"\
	{$(INCLUDE)}"\print_queue.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\tbl.h"\
	

"$(INTDIR)\delete.obj" : $(SOURCE) $(DEP_CPP_DELET) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\print_server\get.c
DEP_CPP_GET_C=\
	"..\include\dicom_platform.h"\
	".\print_server\print_server.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_messages.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_print.h"\
	{$(INCLUDE)}"\dicom_services.h"\
	{$(INCLUDE)}"\dicom_sq.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\dulprotocol.h"\
	{$(INCLUDE)}"\gq.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\manage.h"\
	{$(INCLUDE)}"\print_queue.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\tbl.h"\
	

"$(INTDIR)\get.obj" : $(SOURCE) $(DEP_CPP_GET_C) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\print_server\action.c
DEP_CPP_ACTIO=\
	"..\include\dicom_platform.h"\
	".\print_server\print_server.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_messages.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_print.h"\
	{$(INCLUDE)}"\dicom_services.h"\
	{$(INCLUDE)}"\dicom_sq.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\dulprotocol.h"\
	{$(INCLUDE)}"\gq.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\manage.h"\
	{$(INCLUDE)}"\print_queue.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\tbl.h"\
	

"$(INTDIR)\action.obj" : $(SOURCE) $(DEP_CPP_ACTIO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\print_server\verify.c
DEP_CPP_VERIF=\
	"..\include\dicom_platform.h"\
	".\print_server\print_server.h"\
	".\print_server\private.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_messages.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_print.h"\
	{$(INCLUDE)}"\dicom_services.h"\
	{$(INCLUDE)}"\dicom_sq.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\dulprotocol.h"\
	{$(INCLUDE)}"\gq.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\manage.h"\
	{$(INCLUDE)}"\print_queue.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\tbl.h"\
	

"$(INTDIR)\verify.obj" : $(SOURCE) $(DEP_CPP_VERIF) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\print_server\set.c
DEP_CPP_SET_C=\
	"..\include\dicom_platform.h"\
	".\print_server\print_server.h"\
	".\print_server\private.h"\
	".\print_server\static_variables.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_messages.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_print.h"\
	{$(INCLUDE)}"\dicom_services.h"\
	{$(INCLUDE)}"\dicom_sq.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\dulprotocol.h"\
	{$(INCLUDE)}"\gq.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\manage.h"\
	{$(INCLUDE)}"\print_queue.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\tbl.h"\
	

"$(INTDIR)\set.obj" : $(SOURCE) $(DEP_CPP_SET_C) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\print_server\utils.c
DEP_CPP_UTILS=\
	"..\include\dicom_platform.h"\
	".\print_server\print_server.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_messages.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_print.h"\
	{$(INCLUDE)}"\dicom_services.h"\
	{$(INCLUDE)}"\dicom_sq.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\dulprotocol.h"\
	{$(INCLUDE)}"\gq.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\manage.h"\
	{$(INCLUDE)}"\print_queue.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\tbl.h"\
	

"$(INTDIR)\utils.obj" : $(SOURCE) $(DEP_CPP_UTILS) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\print_server\prequests.c
DEP_CPP_PREQU=\
	"..\include\dicom_platform.h"\
	".\print_server\print_server.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_messages.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_print.h"\
	{$(INCLUDE)}"\dicom_services.h"\
	{$(INCLUDE)}"\dicom_sq.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\dulprotocol.h"\
	{$(INCLUDE)}"\gq.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\manage.h"\
	{$(INCLUDE)}"\print_queue.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\tbl.h"\
	

"$(INTDIR)\prequests.obj" : $(SOURCE) $(DEP_CPP_PREQU) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\print_server\passociation.c
DEP_CPP_PASSO=\
	"..\include\dicom_platform.h"\
	".\print_server\print_server.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_messages.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_print.h"\
	{$(INCLUDE)}"\dicom_services.h"\
	{$(INCLUDE)}"\dicom_sq.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\dulprotocol.h"\
	{$(INCLUDE)}"\gq.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\manage.h"\
	{$(INCLUDE)}"\print_queue.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\tbl.h"\
	

"$(INTDIR)\passociation.obj" : $(SOURCE) $(DEP_CPP_PASSO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\print_server\psscond.c
DEP_CPP_PSSCO=\
	"..\include\dicom_platform.h"\
	".\print_server\print_server.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_messages.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_print.h"\
	{$(INCLUDE)}"\dicom_services.h"\
	{$(INCLUDE)}"\dicom_sq.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\dulprotocol.h"\
	{$(INCLUDE)}"\gq.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\manage.h"\
	{$(INCLUDE)}"\print_queue.h"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	{$(INCLUDE)}"\tbl.h"\
	

"$(INTDIR)\psscond.obj" : $(SOURCE) $(DEP_CPP_PSSCO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
################################################################################
# Begin Target

# Name "mwlQuery - Win32 Release"
# Name "mwlQuery - Win32 Debug"

!IF  "$(CFG)" == "mwlQuery - Win32 Release"

!ELSEIF  "$(CFG)" == "mwlQuery - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\mwlQuery\mwlQuery.c
DEP_CPP_MWLQU=\
	"..\include\dicom_platform.h"\
	{$(INCLUDE)}"\condition.h"\
	{$(INCLUDE)}"\ctnthread.h"\
	{$(INCLUDE)}"\dicom.h"\
	{$(INCLUDE)}"\dicom_messages.h"\
	{$(INCLUDE)}"\dicom_objects.h"\
	{$(INCLUDE)}"\dicom_services.h"\
	{$(INCLUDE)}"\dicom_uids.h"\
	{$(INCLUDE)}"\dulprotocol.h"\
	{$(INCLUDE)}"\lst.h"\
	{$(INCLUDE)}"\sys\STAT.H"\
	{$(INCLUDE)}"\sys\TYPES.H"\
	

"$(INTDIR)\mwlQuery.obj" : $(SOURCE) $(DEP_CPP_MWLQU) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
# End Project
################################################################################
