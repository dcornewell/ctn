# Microsoft Developer Studio Project File - Name="ctn_lib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ctn_lib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ctn_lib.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ctn_lib - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "ctn_lib - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "ctn_lib - Win32 Release"
# Name "ctn_lib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\facilities\fis\build.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\services\cmd_valid.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\condition\condition.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\manage\control.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\thread\ctnthread.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\objects\dcm.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\objects\dcm1.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\objects\dcmcond.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\objects\dcmdict.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\objects\dcmsupport.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\ddr\ddr.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\manage\delete.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\dulprotocol\dulcond.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\dulprotocol\dulconstruct.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\dulprotocol\dulfsm.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\dulprotocol\dulparse.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\dulprotocol\dulpresent.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\dulprotocol\dulprotocol.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\messages\dump.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\fis\event.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\services\find.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\fis\fis.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\fis\fiscond.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\fis\fisdelete.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\fis\fisget.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\fis\fisinsert.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\services\get.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\gq\gq.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\iap\iap.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\iap\iapcond.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\idb\idb.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\idb\idbcond.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\info_entity\ie.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\info_entity\iecond.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\manage\insert.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\lst\lst.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\lst\lstcond.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\manage\mancond.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\messages\messages.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\services\move.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\messages\msgcond.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\services\naction.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\services\ncreate.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\services\ndelete.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\services\neventreport.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\services\nget.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\services\nset.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\print\print.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\print\printcond.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\services\private.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\fis\record.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\messages\ref_item.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\manage\select.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\services\send.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\sq\sequences.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\manage\set.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\sq\sqcond.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\services\srv1.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\services\srv2.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\services\srvcond.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\services\storage.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\tbl\tbl_sqlserver.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\tbl\tblcond.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\thread\thrcond.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\uid\uid.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\uid\uidcond.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\fis\update.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\utility\utility.c
# End Source File
# Begin Source File

SOURCE=..\..\facilities\services\verify.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\facilities\condition\condition.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\thread\ctnthread.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\objects\dcmprivate.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\dicom\dicom.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\ddr\dicom_ddr.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\info_entity\dicom_ie.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\messages\dicom_messages.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\objects\dicom_objects.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\dicom\dicom_platform.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\print\dicom_print.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\services\dicom_services.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\sq\dicom_sq.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\uid\dicom_uids.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\manage\dmanprivate.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\dulprotocol\dulfsm.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\dulprotocol\dulprivate.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\dulprotocol\dulprotocol.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\dulprotocol\dulstructures.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\fis\fis.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\fis\fis_private.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\gq\gq.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\iap\iap.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\idb\idb.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\lst\lst.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\lst\lstprivate.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\manage\manage.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\messages\msgprivate.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\services\private.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\info_entity\tables.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\tbl\tbl.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\tbl\tbl_sqlserver.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\tbl\tblprivate.h
# End Source File
# Begin Source File

SOURCE=..\..\facilities\utility\utility.h
# End Source File
# End Group
# End Target
# End Project
