#	Makefile for TBL facility

SYBASE_OBJS = tblmb_sybase.o tblmbcond.o
MSQL_OBJS = tblmb_msql.o tblmbcond.o
PSQL_OBJS = tblmb_psql.o tblmbcond.o
UFS_OBJS = ufs.o tblmbcond.o
include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS)

library:	libtblmb_psql.a
#library:	libtblmb_sybase.a libtblmb_msql.a libtblmb_psql.a libtblmb_ufs.a
#library:	libtblmb_sybase.a

install:	library
#install:	libtblmb_sybase.a
	cp libtblmb_psql.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libtblmb_psql.a
	$(INSTALLINCLUDE) tblmb.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:

links:
	rm -f $(DICOM_LIBSRC)/tblmb.h $(DICOM_LIBSRC)/tblmb_ufs.h
	rm -f $(DICOM_LIBSRC)/tblmbprivate.h $(DICOM_LIBSRC)/tblmbcond.c
	rm -f $(DICOM_LIBSRC)/tblmb_sqlserver.c $(DICOM_LIBSRC)/tblmb_sqlserver.h
	rm -f $(DICOM_LIBSRC)/tblmb_sybase.c $(DICOM_LIBSRC)/tblmb_sybase.h
	rm -f $(DICOM_LIBSRC)/tblmb_msql.c $(DICOM_LIBSRC)/tblmb_msql.h
	rm -f $(DICOM_LIBSRC)/tblmb_psql.c $(DICOM_LIBSRC)/tblmb_psql.h
	rm -f $(DICOM_LIBSRC)/ufs.c
	ln -s ../facilities/tblmb/tblmb.h $(DICOM_LIBSRC)
#	ln -s ../facilities/tblmb/tblmb_ufs.h $(DICOM_LIBSRC)
	ln -s ../facilities/tblmb/tblmbprivate.h $(DICOM_LIBSRC)
	ln -s ../facilities/tblmb/tblmbcond.c $(DICOM_LIBSRC)
#	ln -s ../facilities/tblmb/tblmb_sqlserver.c $(DICOM_LIBSRC)
#	ln -s ../facilities/tblmb/tblmb_sqlserver.h $(DICOM_LIBSRC)
#	ln -s ../facilities/tblmb/tblmb_sybase.c $(DICOM_LIBSRC)
#	ln -s ../facilities/tblmb/tblmb_sybase.h $(DICOM_LIBSRC)
#	ln -s ../facilities/tblmb/tblmb_msql.c $(DICOM_LIBSRC)
#	ln -s ../facilities/tblmb/tblmb_msql.h $(DICOM_LIBSRC)
	ln -s ../facilities/tblmb/tblmb_psql.c $(DICOM_LIBSRC)
	ln -s ../facilities/tblmb/tblmb_psql.h $(DICOM_LIBSRC)
#	ln -s ../facilities/tblmb/ufs.c $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *.BAK *% core

indent:
#	$(INDENT) tblmb.h
#	$(INDENT) tblmb_ufs.h
#	$(INDENT) tblmbprivate.h
	$(INDENT) tblmbcond.c
	$(INDENT) tblmb_sybase.c
#	$(INDENT) tblmb_sybase.h
	$(INDENT) tblmb_msql.c
	$(INDENT) tblmb_psql.c
#	$(INDENT) tblmb_msql.h
	$(INDENT) ufs.c

checkin:
				$(CI) tblmb.h
				$(CI) tblmb_ufs.h
				$(CI) tblmbprivate.h
	$(INDENT) tblmbcond.c;	$(CI) tblmbcond.c
	$(INDENT) tblmb_sybase.c;	$(CI) tblmb_sybase.c
				$(CI) tblmb_sybase.h
	$(INDENT) tblmb_msql.c;	$(CI) tblmb_msql.c
				$(CI) tblmb_msql.h
	$(INDENT) tblmb_psql.c;	$(CI) tblmb_psql.c
				$(CI) tblmb_psql.h
				$(CI) tblmb_sqlserver.c
				$(CI) tblmb_sqlserver.h
	$(INDENT) ufs.c;	$(CI) ufs.c
	rm -f *.a *.o *.BAK *% core

checkout:
	$(CO) tblmb.h
	$(CO) tblmb_ufs.h
	$(CO) tblmbprivate.h
	$(CO) tblmbcond.c
	$(CO) tblmb_sybase.c
	$(CO) tblmb_sybase.h
	$(CO) tblmb_msql.c
	$(CO) tblmb_msql.h
	$(CO) tblmb_psql.c
	$(CO) tblmb_psql.h
	$(CO) tblmb_sqlserver.c
	$(CO) tblmb_sqlserver.h
	$(CO) ufs.c

libtblmb_sybase.a:	$(SYBASE_OBJS)
	ar cvr libtblmb_sybase.a $(SYBASE_OBJS); $(RANLIB) libtblmb_sybase.a
libtblmb_msql.a:	$(MSQL_OBJS)
	ar cvr libtblmb_msql.a $(MSQL_OBJS); $(RANLIB) libtblmb_msql.a
libtblmb_psql.a:	$(PSQL_OBJS)
	ar cvr libtblmb_psql.a $(PSQL_OBJS); $(RANLIB) libtblmb_psql.a
libtblmb_ufs.a:	$(UFS_OBJS)
	ar cvr libtblmb_ufs.a $(UFS_OBJS); $(RANLIB) libtblmb_ufs.a

tblmbcond.o:	tblmb.h tblmbcond.c
tblmb_sybase.o:	tblmb.h tblmb_sybase.c tblmb_sybase.h
tblmb_msql.o:	tblmb.h tblmb_msql.c tblmb_msql.h
tblmb_psql.o:	tblmb.h tblmb_psql.c tblmb_psql.h
ufs.o:		tblmb.h tblmb_ufs.h ufs.c

lint:
	flexelint $(LINTOPTIONS)  tblmb_sybase.c tblmbcond.c
	flexelint $(LINTOPTIONS)  tblmb_msql.c tblmbcond.c
	flexelint $(LINTOPTIONS)  tblmb_psql.c tblmbcond.c
	flexelint $(LINTOPTIONS)  ufs.c tblmbcond.c
