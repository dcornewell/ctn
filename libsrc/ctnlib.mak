# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

!IF "$(CFG)" == ""
CFG=ctnlib - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to ctnlib - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ctnlib - Win32 Release" && "$(CFG)" != "ctnlib - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "ctnlib.mak" CFG="ctnlib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ctnlib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ctnlib - Win32 Debug" (based on "Win32 (x86) Static Library")
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
# PROP Target_Last_Scanned "ctnlib - Win32 Debug"
CPP=cl.exe

!IF  "$(CFG)" == "ctnlib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\ctnlib.lib"

CLEAN : 
	-@erase "$(INTDIR)\build.obj"
	-@erase "$(INTDIR)\cmd_valid.obj"
	-@erase "$(INTDIR)\condition.obj"
	-@erase "$(INTDIR)\control.obj"
	-@erase "$(INTDIR)\ctnthread.obj"
	-@erase "$(INTDIR)\dcm.obj"
	-@erase "$(INTDIR)\dcm1.obj"
	-@erase "$(INTDIR)\dcmcond.obj"
	-@erase "$(INTDIR)\dcmdict.obj"
	-@erase "$(INTDIR)\dcmsupport.obj"
	-@erase "$(INTDIR)\delete.obj"
	-@erase "$(INTDIR)\dulcond.obj"
	-@erase "$(INTDIR)\dulconstruct.obj"
	-@erase "$(INTDIR)\dulfsm.obj"
	-@erase "$(INTDIR)\dulparse.obj"
	-@erase "$(INTDIR)\dulpresent.obj"
	-@erase "$(INTDIR)\dulprotocol.obj"
	-@erase "$(INTDIR)\dump.obj"
	-@erase "$(INTDIR)\event.obj"
	-@erase "$(INTDIR)\find.obj"
	-@erase "$(INTDIR)\fis.obj"
	-@erase "$(INTDIR)\fiscond.obj"
	-@erase "$(INTDIR)\fisdelete.obj"
	-@erase "$(INTDIR)\fisget.obj"
	-@erase "$(INTDIR)\fisinsert.obj"
	-@erase "$(INTDIR)\get.obj"
	-@erase "$(INTDIR)\gq.obj"
	-@erase "$(INTDIR)\iap.obj"
	-@erase "$(INTDIR)\iapcond.obj"
	-@erase "$(INTDIR)\idb.obj"
	-@erase "$(INTDIR)\idbcond.obj"
	-@erase "$(INTDIR)\ie.obj"
	-@erase "$(INTDIR)\iecond.obj"
	-@erase "$(INTDIR)\insert.obj"
	-@erase "$(INTDIR)\lst.obj"
	-@erase "$(INTDIR)\mancond.obj"
	-@erase "$(INTDIR)\messages.obj"
	-@erase "$(INTDIR)\move.obj"
	-@erase "$(INTDIR)\msgcond.obj"
	-@erase "$(INTDIR)\naction.obj"
	-@erase "$(INTDIR)\ncreate.obj"
	-@erase "$(INTDIR)\ndelete.obj"
	-@erase "$(INTDIR)\nget.obj"
	-@erase "$(INTDIR)\nset.obj"
	-@erase "$(INTDIR)\print.obj"
	-@erase "$(INTDIR)\printcond.obj"
	-@erase "$(INTDIR)\private.obj"
	-@erase "$(INTDIR)\record.obj"
	-@erase "$(INTDIR)\ref_item.obj"
	-@erase "$(INTDIR)\select.obj"
	-@erase "$(INTDIR)\send.obj"
	-@erase "$(INTDIR)\sequences.obj"
	-@erase "$(INTDIR)\set.obj"
	-@erase "$(INTDIR)\sqcond.obj"
	-@erase "$(INTDIR)\srv1.obj"
	-@erase "$(INTDIR)\srv2.obj"
	-@erase "$(INTDIR)\srvcond.obj"
	-@erase "$(INTDIR)\storage.obj"
	-@erase "$(INTDIR)\tbl_sqlserver.obj"
	-@erase "$(INTDIR)\tblcond.obj"
	-@erase "$(INTDIR)\thrcond.obj"
	-@erase "$(INTDIR)\uid.obj"
	-@erase "$(INTDIR)\uidcond.obj"
	-@erase "$(INTDIR)\update.obj"
	-@erase "$(INTDIR)\utility.obj"
	-@erase "$(INTDIR)\verify.obj"
	-@erase "$(OUTDIR)\ctnlib.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MTd /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/ctnlib.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ctnlib.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/ctnlib.lib" 
LIB32_OBJS= \
	"$(INTDIR)\build.obj" \
	"$(INTDIR)\cmd_valid.obj" \
	"$(INTDIR)\condition.obj" \
	"$(INTDIR)\control.obj" \
	"$(INTDIR)\ctnthread.obj" \
	"$(INTDIR)\dcm.obj" \
	"$(INTDIR)\dcm1.obj" \
	"$(INTDIR)\dcmcond.obj" \
	"$(INTDIR)\dcmdict.obj" \
	"$(INTDIR)\dcmsupport.obj" \
	"$(INTDIR)\delete.obj" \
	"$(INTDIR)\dulcond.obj" \
	"$(INTDIR)\dulconstruct.obj" \
	"$(INTDIR)\dulfsm.obj" \
	"$(INTDIR)\dulparse.obj" \
	"$(INTDIR)\dulpresent.obj" \
	"$(INTDIR)\dulprotocol.obj" \
	"$(INTDIR)\dump.obj" \
	"$(INTDIR)\event.obj" \
	"$(INTDIR)\find.obj" \
	"$(INTDIR)\fis.obj" \
	"$(INTDIR)\fiscond.obj" \
	"$(INTDIR)\fisdelete.obj" \
	"$(INTDIR)\fisget.obj" \
	"$(INTDIR)\fisinsert.obj" \
	"$(INTDIR)\get.obj" \
	"$(INTDIR)\gq.obj" \
	"$(INTDIR)\iap.obj" \
	"$(INTDIR)\iapcond.obj" \
	"$(INTDIR)\idb.obj" \
	"$(INTDIR)\idbcond.obj" \
	"$(INTDIR)\ie.obj" \
	"$(INTDIR)\iecond.obj" \
	"$(INTDIR)\insert.obj" \
	"$(INTDIR)\lst.obj" \
	"$(INTDIR)\mancond.obj" \
	"$(INTDIR)\messages.obj" \
	"$(INTDIR)\move.obj" \
	"$(INTDIR)\msgcond.obj" \
	"$(INTDIR)\naction.obj" \
	"$(INTDIR)\ncreate.obj" \
	"$(INTDIR)\ndelete.obj" \
	"$(INTDIR)\nget.obj" \
	"$(INTDIR)\nset.obj" \
	"$(INTDIR)\print.obj" \
	"$(INTDIR)\printcond.obj" \
	"$(INTDIR)\private.obj" \
	"$(INTDIR)\record.obj" \
	"$(INTDIR)\ref_item.obj" \
	"$(INTDIR)\select.obj" \
	"$(INTDIR)\send.obj" \
	"$(INTDIR)\sequences.obj" \
	"$(INTDIR)\set.obj" \
	"$(INTDIR)\sqcond.obj" \
	"$(INTDIR)\srv1.obj" \
	"$(INTDIR)\srv2.obj" \
	"$(INTDIR)\srvcond.obj" \
	"$(INTDIR)\storage.obj" \
	"$(INTDIR)\tbl_sqlserver.obj" \
	"$(INTDIR)\tblcond.obj" \
	"$(INTDIR)\thrcond.obj" \
	"$(INTDIR)\uid.obj" \
	"$(INTDIR)\uidcond.obj" \
	"$(INTDIR)\update.obj" \
	"$(INTDIR)\utility.obj" \
	"$(INTDIR)\verify.obj"

"$(OUTDIR)\ctnlib.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ctnlib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\ctnlib.lib"

CLEAN : 
	-@erase "$(INTDIR)\build.obj"
	-@erase "$(INTDIR)\cmd_valid.obj"
	-@erase "$(INTDIR)\condition.obj"
	-@erase "$(INTDIR)\control.obj"
	-@erase "$(INTDIR)\ctnthread.obj"
	-@erase "$(INTDIR)\dcm.obj"
	-@erase "$(INTDIR)\dcm1.obj"
	-@erase "$(INTDIR)\dcmcond.obj"
	-@erase "$(INTDIR)\dcmdict.obj"
	-@erase "$(INTDIR)\dcmsupport.obj"
	-@erase "$(INTDIR)\delete.obj"
	-@erase "$(INTDIR)\dulcond.obj"
	-@erase "$(INTDIR)\dulconstruct.obj"
	-@erase "$(INTDIR)\dulfsm.obj"
	-@erase "$(INTDIR)\dulparse.obj"
	-@erase "$(INTDIR)\dulpresent.obj"
	-@erase "$(INTDIR)\dulprotocol.obj"
	-@erase "$(INTDIR)\dump.obj"
	-@erase "$(INTDIR)\event.obj"
	-@erase "$(INTDIR)\find.obj"
	-@erase "$(INTDIR)\fis.obj"
	-@erase "$(INTDIR)\fiscond.obj"
	-@erase "$(INTDIR)\fisdelete.obj"
	-@erase "$(INTDIR)\fisget.obj"
	-@erase "$(INTDIR)\fisinsert.obj"
	-@erase "$(INTDIR)\get.obj"
	-@erase "$(INTDIR)\gq.obj"
	-@erase "$(INTDIR)\iap.obj"
	-@erase "$(INTDIR)\iapcond.obj"
	-@erase "$(INTDIR)\idb.obj"
	-@erase "$(INTDIR)\idbcond.obj"
	-@erase "$(INTDIR)\ie.obj"
	-@erase "$(INTDIR)\iecond.obj"
	-@erase "$(INTDIR)\insert.obj"
	-@erase "$(INTDIR)\lst.obj"
	-@erase "$(INTDIR)\mancond.obj"
	-@erase "$(INTDIR)\messages.obj"
	-@erase "$(INTDIR)\move.obj"
	-@erase "$(INTDIR)\msgcond.obj"
	-@erase "$(INTDIR)\naction.obj"
	-@erase "$(INTDIR)\ncreate.obj"
	-@erase "$(INTDIR)\ndelete.obj"
	-@erase "$(INTDIR)\nget.obj"
	-@erase "$(INTDIR)\nset.obj"
	-@erase "$(INTDIR)\print.obj"
	-@erase "$(INTDIR)\printcond.obj"
	-@erase "$(INTDIR)\private.obj"
	-@erase "$(INTDIR)\record.obj"
	-@erase "$(INTDIR)\ref_item.obj"
	-@erase "$(INTDIR)\select.obj"
	-@erase "$(INTDIR)\send.obj"
	-@erase "$(INTDIR)\sequences.obj"
	-@erase "$(INTDIR)\set.obj"
	-@erase "$(INTDIR)\sqcond.obj"
	-@erase "$(INTDIR)\srv1.obj"
	-@erase "$(INTDIR)\srv2.obj"
	-@erase "$(INTDIR)\srvcond.obj"
	-@erase "$(INTDIR)\storage.obj"
	-@erase "$(INTDIR)\tbl_sqlserver.obj"
	-@erase "$(INTDIR)\tblcond.obj"
	-@erase "$(INTDIR)\thrcond.obj"
	-@erase "$(INTDIR)\uid.obj"
	-@erase "$(INTDIR)\uidcond.obj"
	-@erase "$(INTDIR)\update.obj"
	-@erase "$(INTDIR)\utility.obj"
	-@erase "$(INTDIR)\verify.obj"
	-@erase "$(OUTDIR)\ctnlib.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/ctnlib.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ctnlib.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/ctnlib.lib" 
LIB32_OBJS= \
	"$(INTDIR)\build.obj" \
	"$(INTDIR)\cmd_valid.obj" \
	"$(INTDIR)\condition.obj" \
	"$(INTDIR)\control.obj" \
	"$(INTDIR)\ctnthread.obj" \
	"$(INTDIR)\dcm.obj" \
	"$(INTDIR)\dcm1.obj" \
	"$(INTDIR)\dcmcond.obj" \
	"$(INTDIR)\dcmdict.obj" \
	"$(INTDIR)\dcmsupport.obj" \
	"$(INTDIR)\delete.obj" \
	"$(INTDIR)\dulcond.obj" \
	"$(INTDIR)\dulconstruct.obj" \
	"$(INTDIR)\dulfsm.obj" \
	"$(INTDIR)\dulparse.obj" \
	"$(INTDIR)\dulpresent.obj" \
	"$(INTDIR)\dulprotocol.obj" \
	"$(INTDIR)\dump.obj" \
	"$(INTDIR)\event.obj" \
	"$(INTDIR)\find.obj" \
	"$(INTDIR)\fis.obj" \
	"$(INTDIR)\fiscond.obj" \
	"$(INTDIR)\fisdelete.obj" \
	"$(INTDIR)\fisget.obj" \
	"$(INTDIR)\fisinsert.obj" \
	"$(INTDIR)\get.obj" \
	"$(INTDIR)\gq.obj" \
	"$(INTDIR)\iap.obj" \
	"$(INTDIR)\iapcond.obj" \
	"$(INTDIR)\idb.obj" \
	"$(INTDIR)\idbcond.obj" \
	"$(INTDIR)\ie.obj" \
	"$(INTDIR)\iecond.obj" \
	"$(INTDIR)\insert.obj" \
	"$(INTDIR)\lst.obj" \
	"$(INTDIR)\mancond.obj" \
	"$(INTDIR)\messages.obj" \
	"$(INTDIR)\move.obj" \
	"$(INTDIR)\msgcond.obj" \
	"$(INTDIR)\naction.obj" \
	"$(INTDIR)\ncreate.obj" \
	"$(INTDIR)\ndelete.obj" \
	"$(INTDIR)\nget.obj" \
	"$(INTDIR)\nset.obj" \
	"$(INTDIR)\print.obj" \
	"$(INTDIR)\printcond.obj" \
	"$(INTDIR)\private.obj" \
	"$(INTDIR)\record.obj" \
	"$(INTDIR)\ref_item.obj" \
	"$(INTDIR)\select.obj" \
	"$(INTDIR)\send.obj" \
	"$(INTDIR)\sequences.obj" \
	"$(INTDIR)\set.obj" \
	"$(INTDIR)\sqcond.obj" \
	"$(INTDIR)\srv1.obj" \
	"$(INTDIR)\srv2.obj" \
	"$(INTDIR)\srvcond.obj" \
	"$(INTDIR)\storage.obj" \
	"$(INTDIR)\tbl_sqlserver.obj" \
	"$(INTDIR)\tblcond.obj" \
	"$(INTDIR)\thrcond.obj" \
	"$(INTDIR)\uid.obj" \
	"$(INTDIR)\uidcond.obj" \
	"$(INTDIR)\update.obj" \
	"$(INTDIR)\utility.obj" \
	"$(INTDIR)\verify.obj"

"$(OUTDIR)\ctnlib.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 

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

################################################################################
# Begin Target

# Name "ctnlib - Win32 Release"
# Name "ctnlib - Win32 Debug"

!IF  "$(CFG)" == "ctnlib - Win32 Release"

!ELSEIF  "$(CFG)" == "ctnlib - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\lst.c
DEP_CPP_LST_C=\
	".\dicom.h"\
	".\dicom_platform.h"\
	".\lst.h"\
	".\lstprivate.h"\
	

"$(INTDIR)\lst.obj" : $(SOURCE) $(DEP_CPP_LST_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\condition.c
DEP_CPP_CONDI=\
	".\condition.h"\
	".\ctnthread.h"\
	".\dicom.h"\
	".\dicom_platform.h"\
	".\lst.h"\
	

"$(INTDIR)\condition.obj" : $(SOURCE) $(DEP_CPP_CONDI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dcmsupport.c
DEP_CPP_DCMSU=\
	".\condition.h"\
	".\dcmprivate.h"\
	".\dicom.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\lst.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\dcmsupport.obj" : $(SOURCE) $(DEP_CPP_DCMSU) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dcm1.c
DEP_CPP_DCM1_=\
	".\condition.h"\
	".\dcmprivate.h"\
	".\dicom.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_uids.h"\
	".\lst.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\dcm1.obj" : $(SOURCE) $(DEP_CPP_DCM1_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dcmcond.c
DEP_CPP_DCMCO=\
	".\dcmprivate.h"\
	".\dicom.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\lst.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\dcmcond.obj" : $(SOURCE) $(DEP_CPP_DCMCO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dcmdict.c
DEP_CPP_DCMDI=\
	".\condition.h"\
	".\dcmprivate.h"\
	".\dicom.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\lst.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\dcmdict.obj" : $(SOURCE) $(DEP_CPP_DCMDI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dcm.c
DEP_CPP_DCM_C=\
	".\condition.h"\
	".\dcmprivate.h"\
	".\dicom.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_uids.h"\
	".\lst.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\dcm.obj" : $(SOURCE) $(DEP_CPP_DCM_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\iecond.c
DEP_CPP_IECON=\
	".\dicom.h"\
	".\dicom_ie.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\lst.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\iecond.obj" : $(SOURCE) $(DEP_CPP_IECON) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ie.c
DEP_CPP_IE_C10=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_ie.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_uids.h"\
	".\lst.h"\
	".\tables.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ie.obj" : $(SOURCE) $(DEP_CPP_IE_C10) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dump.c
DEP_CPP_DUMP_=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_messages.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dulprotocol.h"\
	".\lst.h"\
	".\msgprivate.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\dump.obj" : $(SOURCE) $(DEP_CPP_DUMP_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\messages.c
DEP_CPP_MESSA=\
	".\condition.h"\
	".\ctnthread.h"\
	".\dicom.h"\
	".\dicom_messages.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dulprotocol.h"\
	".\lst.h"\
	".\msgprivate.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\messages.obj" : $(SOURCE) $(DEP_CPP_MESSA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\msgcond.c
DEP_CPP_MSGCO=\
	".\dicom.h"\
	".\dicom_messages.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\lst.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\msgcond.obj" : $(SOURCE) $(DEP_CPP_MSGCO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ref_item.c
DEP_CPP_REF_I=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_messages.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dulprotocol.h"\
	".\lst.h"\
	".\msgprivate.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ref_item.obj" : $(SOURCE) $(DEP_CPP_REF_I) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\printcond.c
DEP_CPP_PRINT=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_print.h"\
	".\dicom_sq.h"\
	".\lst.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\printcond.obj" : $(SOURCE) $(DEP_CPP_PRINT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\print.c
DEP_CPP_PRINT_=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_print.h"\
	".\dicom_sq.h"\
	".\dicom_uids.h"\
	".\lst.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\print.obj" : $(SOURCE) $(DEP_CPP_PRINT_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\sequences.c
DEP_CPP_SEQUE=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_sq.h"\
	".\dulprotocol.h"\
	".\lst.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\sequences.obj" : $(SOURCE) $(DEP_CPP_SEQUE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\sqcond.c
DEP_CPP_SQCON=\
	".\dicom.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_sq.h"\
	".\lst.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\sqcond.obj" : $(SOURCE) $(DEP_CPP_SQCON) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\uidcond.c
DEP_CPP_UIDCO=\
	".\dicom.h"\
	".\dicom_platform.h"\
	".\dicom_uids.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\uidcond.obj" : $(SOURCE) $(DEP_CPP_UIDCO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\uid.c
DEP_CPP_UID_C=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_platform.h"\
	".\dicom_uids.h"\
	

"$(INTDIR)\uid.obj" : $(SOURCE) $(DEP_CPP_UID_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dulcond.c
DEP_CPP_DULCO=\
	".\dicom.h"\
	".\dicom_platform.h"\
	".\dulprotocol.h"\
	".\dulstructures.h"\
	".\lst.h"\
	

"$(INTDIR)\dulcond.obj" : $(SOURCE) $(DEP_CPP_DULCO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dulconstruct.c
DEP_CPP_DULCON=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_platform.h"\
	".\dulprivate.h"\
	".\dulprotocol.h"\
	".\dulstructures.h"\
	".\lst.h"\
	

"$(INTDIR)\dulconstruct.obj" : $(SOURCE) $(DEP_CPP_DULCON) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dulprotocol.c
DEP_CPP_DULPR=\
	".\blg.h"\
	".\condition.h"\
	".\ctnthread.h"\
	".\dicom.h"\
	".\dicom_platform.h"\
	".\dicom_uids.h"\
	".\dulfsm.h"\
	".\dulprivate.h"\
	".\dulprotocol.h"\
	".\dulstructures.h"\
	".\lst.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\dulprotocol.obj" : $(SOURCE) $(DEP_CPP_DULPR) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dulparse.c
DEP_CPP_DULPA=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_platform.h"\
	".\dulprivate.h"\
	".\dulprotocol.h"\
	".\dulstructures.h"\
	".\lst.h"\
	

"$(INTDIR)\dulparse.obj" : $(SOURCE) $(DEP_CPP_DULPA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dulpresent.c
DEP_CPP_DULPRE=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_platform.h"\
	".\dulfsm.h"\
	".\dulprivate.h"\
	".\dulprotocol.h"\
	".\dulstructures.h"\
	".\lst.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\dulpresent.obj" : $(SOURCE) $(DEP_CPP_DULPRE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dulfsm.c
DEP_CPP_DULFS=\
	".\blg.h"\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_platform.h"\
	".\dulfsm.h"\
	".\dulprivate.h"\
	".\dulprotocol.h"\
	".\dulstructures.h"\
	".\lst.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\dulfsm.obj" : $(SOURCE) $(DEP_CPP_DULFS) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\srv2.c
DEP_CPP_SRV2_=\
	".\condition.h"\
	".\ctnthread.h"\
	".\dicom.h"\
	".\dicom_messages.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_services.h"\
	".\dicom_uids.h"\
	".\dulprotocol.h"\
	".\lst.h"\
	".\private.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\srv2.obj" : $(SOURCE) $(DEP_CPP_SRV2_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\srv1.c
DEP_CPP_SRV1_=\
	".\condition.h"\
	".\ctnthread.h"\
	".\dicom.h"\
	".\dicom_messages.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_services.h"\
	".\dicom_uids.h"\
	".\dulprotocol.h"\
	".\lst.h"\
	".\private.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\srv1.obj" : $(SOURCE) $(DEP_CPP_SRV1_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\cmd_valid.c
DEP_CPP_CMD_V=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_messages.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_services.h"\
	".\dicom_uids.h"\
	".\dulprotocol.h"\
	".\lst.h"\
	".\private.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\cmd_valid.obj" : $(SOURCE) $(DEP_CPP_CMD_V) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\send.c
DEP_CPP_SEND_=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_messages.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_services.h"\
	".\dicom_uids.h"\
	".\dulprotocol.h"\
	".\lst.h"\
	".\private.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\send.obj" : $(SOURCE) $(DEP_CPP_SEND_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\verify.c
DEP_CPP_VERIF=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_messages.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_services.h"\
	".\dicom_uids.h"\
	".\dulprotocol.h"\
	".\lst.h"\
	".\private.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\verify.obj" : $(SOURCE) $(DEP_CPP_VERIF) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\storage.c
DEP_CPP_STORA=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_messages.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_services.h"\
	".\dicom_uids.h"\
	".\dulprotocol.h"\
	".\lst.h"\
	".\private.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\storage.obj" : $(SOURCE) $(DEP_CPP_STORA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\find.c
DEP_CPP_FIND_=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_messages.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_services.h"\
	".\dicom_uids.h"\
	".\dulprotocol.h"\
	".\lst.h"\
	".\private.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\find.obj" : $(SOURCE) $(DEP_CPP_FIND_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\private.c
DEP_CPP_PRIVA=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_messages.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_services.h"\
	".\dicom_uids.h"\
	".\dulprotocol.h"\
	".\lst.h"\
	

"$(INTDIR)\private.obj" : $(SOURCE) $(DEP_CPP_PRIVA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\srvcond.c
DEP_CPP_SRVCO=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_messages.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_services.h"\
	".\dicom_uids.h"\
	".\dulprotocol.h"\
	".\lst.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\srvcond.obj" : $(SOURCE) $(DEP_CPP_SRVCO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\get.c
DEP_CPP_GET_C=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_messages.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_services.h"\
	".\dicom_uids.h"\
	".\dulprotocol.h"\
	".\lst.h"\
	".\private.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\get.obj" : $(SOURCE) $(DEP_CPP_GET_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\move.c
DEP_CPP_MOVE_=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_messages.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_services.h"\
	".\dicom_uids.h"\
	".\dulprotocol.h"\
	".\lst.h"\
	".\private.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\move.obj" : $(SOURCE) $(DEP_CPP_MOVE_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\utility.c
DEP_CPP_UTILI=\
	".\dicom.h"\
	".\dicom_platform.h"\
	".\utility.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\timeb.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\utility.obj" : $(SOURCE) $(DEP_CPP_UTILI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\iapcond.c
DEP_CPP_IAPCO=\
	".\dbquery.h"\
	".\dicom.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dulprotocol.h"\
	".\iap.h"\
	".\lst.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\iapcond.obj" : $(SOURCE) $(DEP_CPP_IAPCO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\iap.c
DEP_CPP_IAP_C=\
	".\condition.h"\
	".\dbquery.h"\
	".\dicom.h"\
	".\dicom_messages.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_services.h"\
	".\dicom_uids.h"\
	".\dulprotocol.h"\
	".\iap.h"\
	".\lst.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\iap.obj" : $(SOURCE) $(DEP_CPP_IAP_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\tbl_sqlserver.c
DEP_CPP_TBL_S=\
	".\condition.h"\
	".\ctnthread.h"\
	".\dicom.h"\
	".\dicom_platform.h"\
	".\tbl.h"\
	".\tbl_sqlserver.h"\
	".\tblprivate.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\tbl_sqlserver.obj" : $(SOURCE) $(DEP_CPP_TBL_S) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\tblcond.c
DEP_CPP_TBLCO=\
	".\dicom.h"\
	".\dicom_platform.h"\
	".\tbl.h"\
	".\tblprivate.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\tblcond.obj" : $(SOURCE) $(DEP_CPP_TBLCO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\control.c
DEP_CPP_CONTR=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_platform.h"\
	".\dicom_uids.h"\
	".\dmanprivate.h"\
	".\lst.h"\
	".\manage.h"\
	".\tbl.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\control.obj" : $(SOURCE) $(DEP_CPP_CONTR) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\delete.c
DEP_CPP_DELET=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_platform.h"\
	".\dmanprivate.h"\
	".\lst.h"\
	".\manage.h"\
	".\tbl.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\delete.obj" : $(SOURCE) $(DEP_CPP_DELET) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\insert.c
DEP_CPP_INSER=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_platform.h"\
	".\dmanprivate.h"\
	".\lst.h"\
	".\manage.h"\
	".\tbl.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\insert.obj" : $(SOURCE) $(DEP_CPP_INSER) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\mancond.c
DEP_CPP_MANCO=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_platform.h"\
	".\lst.h"\
	".\manage.h"\
	".\tbl.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\mancond.obj" : $(SOURCE) $(DEP_CPP_MANCO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\select.c
DEP_CPP_SELEC=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_platform.h"\
	".\dmanprivate.h"\
	".\lst.h"\
	".\manage.h"\
	".\tbl.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\select.obj" : $(SOURCE) $(DEP_CPP_SELEC) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\set.c
DEP_CPP_SET_C=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_platform.h"\
	".\dmanprivate.h"\
	".\lst.h"\
	".\manage.h"\
	".\tbl.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\set.obj" : $(SOURCE) $(DEP_CPP_SET_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\idbcond.c
DEP_CPP_IDBCO=\
	".\dicom.h"\
	".\dicom_platform.h"\
	".\idb.h"\
	".\lst.h"\
	".\tbl.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\idbcond.obj" : $(SOURCE) $(DEP_CPP_IDBCO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\idb.c
DEP_CPP_IDB_C=\
	".\ctnthread.h"\
	".\dicom.h"\
	".\dicom_platform.h"\
	".\idb.h"\
	".\lst.h"\
	".\tbl.h"\
	".\utility.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\idb.obj" : $(SOURCE) $(DEP_CPP_IDB_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\build.c
DEP_CPP_BUILD=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_sq.h"\
	".\dicom_uids.h"\
	".\fis.h"\
	".\fis_private.h"\
	".\lst.h"\
	".\manage.h"\
	".\tbl.h"\
	".\utility.h"\
	

"$(INTDIR)\build.obj" : $(SOURCE) $(DEP_CPP_BUILD) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\event.c
DEP_CPP_EVENT=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_messages.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_services.h"\
	".\dicom_uids.h"\
	".\dulprotocol.h"\
	".\fis.h"\
	".\fis_private.h"\
	".\lst.h"\
	".\manage.h"\
	".\tbl.h"\
	".\utility.h"\
	

"$(INTDIR)\event.obj" : $(SOURCE) $(DEP_CPP_EVENT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\fisinsert.c
DEP_CPP_FISIN=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_uids.h"\
	".\fis.h"\
	".\fis_private.h"\
	".\lst.h"\
	".\manage.h"\
	".\tbl.h"\
	".\utility.h"\
	

"$(INTDIR)\fisinsert.obj" : $(SOURCE) $(DEP_CPP_FISIN) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\fiscond.c
DEP_CPP_FISCO=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\fis.h"\
	".\lst.h"\
	".\manage.h"\
	".\tbl.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\fiscond.obj" : $(SOURCE) $(DEP_CPP_FISCO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\fisdelete.c
DEP_CPP_FISDE=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_uids.h"\
	".\fis.h"\
	".\fis_private.h"\
	".\lst.h"\
	".\manage.h"\
	".\tbl.h"\
	".\utility.h"\
	

"$(INTDIR)\fisdelete.obj" : $(SOURCE) $(DEP_CPP_FISDE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\fisget.c
DEP_CPP_FISGE=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_uids.h"\
	".\fis.h"\
	".\fis_private.h"\
	".\lst.h"\
	".\manage.h"\
	".\tbl.h"\
	".\utility.h"\
	

"$(INTDIR)\fisget.obj" : $(SOURCE) $(DEP_CPP_FISGE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\fis.c
DEP_CPP_FIS_C=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\fis.h"\
	".\fis_private.h"\
	".\lst.h"\
	".\manage.h"\
	".\tbl.h"\
	

"$(INTDIR)\fis.obj" : $(SOURCE) $(DEP_CPP_FIS_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\record.c
DEP_CPP_RECOR=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_uids.h"\
	".\fis.h"\
	".\fis_private.h"\
	".\lst.h"\
	".\manage.h"\
	".\tbl.h"\
	

"$(INTDIR)\record.obj" : $(SOURCE) $(DEP_CPP_RECOR) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\update.c
DEP_CPP_UPDAT=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_uids.h"\
	".\fis.h"\
	".\fis_private.h"\
	".\lst.h"\
	".\manage.h"\
	".\tbl.h"\
	".\utility.h"\
	

"$(INTDIR)\update.obj" : $(SOURCE) $(DEP_CPP_UPDAT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\naction.c
DEP_CPP_NACTI=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_messages.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_services.h"\
	".\dicom_uids.h"\
	".\dulprotocol.h"\
	".\lst.h"\
	".\private.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\naction.obj" : $(SOURCE) $(DEP_CPP_NACTI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ctnthread.c
DEP_CPP_CTNTH=\
	".\ctnthread.h"\
	".\dicom.h"\
	".\dicom_platform.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ctnthread.obj" : $(SOURCE) $(DEP_CPP_CTNTH) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\thrcond.c
DEP_CPP_THRCO=\
	".\ctnthread.h"\
	".\dicom.h"\
	".\dicom_platform.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\thrcond.obj" : $(SOURCE) $(DEP_CPP_THRCO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\gq.c
DEP_CPP_GQ_C7a=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_platform.h"\
	".\gq.h"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\gq.obj" : $(SOURCE) $(DEP_CPP_GQ_C7a) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ndelete.c
DEP_CPP_NDELE=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_messages.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_services.h"\
	".\dicom_uids.h"\
	".\dulprotocol.h"\
	".\lst.h"\
	".\private.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ndelete.obj" : $(SOURCE) $(DEP_CPP_NDELE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\nset.c
DEP_CPP_NSET_=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_messages.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_services.h"\
	".\dicom_uids.h"\
	".\dulprotocol.h"\
	".\lst.h"\
	".\private.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\nset.obj" : $(SOURCE) $(DEP_CPP_NSET_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\nget.c
DEP_CPP_NGET_=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_messages.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_services.h"\
	".\dicom_uids.h"\
	".\dulprotocol.h"\
	".\lst.h"\
	".\private.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\nget.obj" : $(SOURCE) $(DEP_CPP_NGET_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ncreate.c
DEP_CPP_NCREA=\
	".\condition.h"\
	".\dicom.h"\
	".\dicom_messages.h"\
	".\dicom_objects.h"\
	".\dicom_platform.h"\
	".\dicom_services.h"\
	".\dicom_uids.h"\
	".\dulprotocol.h"\
	".\lst.h"\
	".\private.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\ncreate.obj" : $(SOURCE) $(DEP_CPP_NCREA) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
