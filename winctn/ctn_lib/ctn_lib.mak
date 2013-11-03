# Microsoft Developer Studio Generated NMAKE File, Based on ctn_lib.dsp
!IF "$(CFG)" == ""
CFG=ctn_lib - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ctn_lib - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ctn_lib - Win32 Release" && "$(CFG)" != "ctn_lib - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ctn_lib.mak" CFG="ctn_lib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ctn_lib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ctn_lib - Win32 Debug" (based on "Win32 (x86) Static Library")
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

!IF  "$(CFG)" == "ctn_lib - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\ctn_lib.lib"


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
	-@erase "$(INTDIR)\ddr.obj"
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
	-@erase "$(INTDIR)\lstcond.obj"
	-@erase "$(INTDIR)\mancond.obj"
	-@erase "$(INTDIR)\messages.obj"
	-@erase "$(INTDIR)\move.obj"
	-@erase "$(INTDIR)\msgcond.obj"
	-@erase "$(INTDIR)\naction.obj"
	-@erase "$(INTDIR)\ncreate.obj"
	-@erase "$(INTDIR)\ndelete.obj"
	-@erase "$(INTDIR)\neventreport.obj"
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
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\verify.obj"
	-@erase "$(OUTDIR)\ctn_lib.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\..\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\ctn_lib.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ctn_lib.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\ctn_lib.lib" 
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
	"$(INTDIR)\ddr.obj" \
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
	"$(INTDIR)\lstcond.obj" \
	"$(INTDIR)\mancond.obj" \
	"$(INTDIR)\messages.obj" \
	"$(INTDIR)\move.obj" \
	"$(INTDIR)\msgcond.obj" \
	"$(INTDIR)\naction.obj" \
	"$(INTDIR)\ncreate.obj" \
	"$(INTDIR)\ndelete.obj" \
	"$(INTDIR)\neventreport.obj" \
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

"$(OUTDIR)\ctn_lib.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ctn_lib - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\ctn_lib.lib"


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
	-@erase "$(INTDIR)\ddr.obj"
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
	-@erase "$(INTDIR)\lstcond.obj"
	-@erase "$(INTDIR)\mancond.obj"
	-@erase "$(INTDIR)\messages.obj"
	-@erase "$(INTDIR)\move.obj"
	-@erase "$(INTDIR)\msgcond.obj"
	-@erase "$(INTDIR)\naction.obj"
	-@erase "$(INTDIR)\ncreate.obj"
	-@erase "$(INTDIR)\ndelete.obj"
	-@erase "$(INTDIR)\neventreport.obj"
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
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\verify.obj"
	-@erase "$(OUTDIR)\ctn_lib.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\ctn_lib.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ctn_lib.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\ctn_lib.lib" 
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
	"$(INTDIR)\ddr.obj" \
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
	"$(INTDIR)\lstcond.obj" \
	"$(INTDIR)\mancond.obj" \
	"$(INTDIR)\messages.obj" \
	"$(INTDIR)\move.obj" \
	"$(INTDIR)\msgcond.obj" \
	"$(INTDIR)\naction.obj" \
	"$(INTDIR)\ncreate.obj" \
	"$(INTDIR)\ndelete.obj" \
	"$(INTDIR)\neventreport.obj" \
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

"$(OUTDIR)\ctn_lib.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
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
!IF EXISTS("ctn_lib.dep")
!INCLUDE "ctn_lib.dep"
!ELSE 
!MESSAGE Warning: cannot find "ctn_lib.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ctn_lib - Win32 Release" || "$(CFG)" == "ctn_lib - Win32 Debug"
SOURCE=..\..\facilities\fis\build.c

"$(INTDIR)\build.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\services\cmd_valid.c

"$(INTDIR)\cmd_valid.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\condition\condition.c

"$(INTDIR)\condition.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\manage\control.c

"$(INTDIR)\control.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\thread\ctnthread.c

"$(INTDIR)\ctnthread.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\objects\dcm.c

"$(INTDIR)\dcm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\objects\dcm1.c

"$(INTDIR)\dcm1.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\objects\dcmcond.c

"$(INTDIR)\dcmcond.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\objects\dcmdict.c

"$(INTDIR)\dcmdict.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\objects\dcmsupport.c

"$(INTDIR)\dcmsupport.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\ddr\ddr.c

"$(INTDIR)\ddr.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\manage\delete.c

"$(INTDIR)\delete.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\dulprotocol\dulcond.c

"$(INTDIR)\dulcond.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\dulprotocol\dulconstruct.c

"$(INTDIR)\dulconstruct.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\dulprotocol\dulfsm.c

"$(INTDIR)\dulfsm.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\dulprotocol\dulparse.c

"$(INTDIR)\dulparse.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\dulprotocol\dulpresent.c

"$(INTDIR)\dulpresent.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\dulprotocol\dulprotocol.c

"$(INTDIR)\dulprotocol.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\messages\dump.c

"$(INTDIR)\dump.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\fis\event.c

"$(INTDIR)\event.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\services\find.c

"$(INTDIR)\find.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\fis\fis.c

"$(INTDIR)\fis.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\fis\fiscond.c

"$(INTDIR)\fiscond.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\fis\fisdelete.c

"$(INTDIR)\fisdelete.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\fis\fisget.c

"$(INTDIR)\fisget.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\fis\fisinsert.c

"$(INTDIR)\fisinsert.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\services\get.c

"$(INTDIR)\get.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\gq\gq.c

"$(INTDIR)\gq.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\iap\iap.c

"$(INTDIR)\iap.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\iap\iapcond.c

"$(INTDIR)\iapcond.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\idb\idb.c

"$(INTDIR)\idb.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\idb\idbcond.c

"$(INTDIR)\idbcond.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\info_entity\ie.c

"$(INTDIR)\ie.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\info_entity\iecond.c

"$(INTDIR)\iecond.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\manage\insert.c

"$(INTDIR)\insert.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\lst\lst.c

"$(INTDIR)\lst.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\lst\lstcond.c

"$(INTDIR)\lstcond.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\manage\mancond.c

"$(INTDIR)\mancond.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\messages\messages.c

"$(INTDIR)\messages.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\services\move.c

"$(INTDIR)\move.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\messages\msgcond.c

"$(INTDIR)\msgcond.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\services\naction.c

"$(INTDIR)\naction.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\services\ncreate.c

"$(INTDIR)\ncreate.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\services\ndelete.c

"$(INTDIR)\ndelete.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\services\neventreport.c

"$(INTDIR)\neventreport.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\services\nget.c

"$(INTDIR)\nget.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\services\nset.c

"$(INTDIR)\nset.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\print\print.c

"$(INTDIR)\print.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\print\printcond.c

"$(INTDIR)\printcond.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\services\private.c

"$(INTDIR)\private.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\fis\record.c

"$(INTDIR)\record.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\messages\ref_item.c

"$(INTDIR)\ref_item.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\manage\select.c

"$(INTDIR)\select.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\services\send.c

"$(INTDIR)\send.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\sq\sequences.c

"$(INTDIR)\sequences.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\manage\set.c

"$(INTDIR)\set.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\sq\sqcond.c

"$(INTDIR)\sqcond.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\services\srv1.c

"$(INTDIR)\srv1.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\services\srv2.c

"$(INTDIR)\srv2.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\services\srvcond.c

"$(INTDIR)\srvcond.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\services\storage.c

"$(INTDIR)\storage.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\tbl\tbl_sqlserver.c

"$(INTDIR)\tbl_sqlserver.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\tbl\tblcond.c

"$(INTDIR)\tblcond.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\thread\thrcond.c

"$(INTDIR)\thrcond.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\uid\uid.c

"$(INTDIR)\uid.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\uid\uidcond.c

"$(INTDIR)\uidcond.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\fis\update.c

"$(INTDIR)\update.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\utility\utility.c

"$(INTDIR)\utility.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\facilities\services\verify.c

"$(INTDIR)\verify.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

