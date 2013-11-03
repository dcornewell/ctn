#	Makefile for TBL facility

SYBASE_OBJS = tbl_sybase.o tblcond.o
MSQL_OBJS = tbl_msql.o tblcond.o
MYSQL_OBJS = tbl_mysql.o tblcond.o
PSQL_OBJS = tbl_psql.o tblcond.o
UFS_OBJS = ufs.o tblcond.o
include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS)

library:	libtbl_sybase.a libtbl_msql.a libtbl_mysql.a libtbl_psql.a libtbl_ufs.a
#library:	libtbl_sybase.a

install:	libtbl_sybase.a libtbl_msql.a libtbl_mysql.a libtbl_psql.a libtbl_ufs.a
#install:	libtbl_sybase.a
	cp libtbl_sybase.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libtbl_sybase.a
	cp libtbl_msql.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libtbl_msql.a
	cp libtbl_mysql.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libtbl_mysql.a
	cp libtbl_psql.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libtbl_psql.a
	cp libtbl_ufs.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libtbl_ufs.a
	$(INSTALLINCLUDE) tbl.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:
	$(INSTALLINCLUDE) tbl.h $(DICOM_INCLUDE)

links:
	rm -f $(DICOM_LIBSRC)/tbl.h $(DICOM_LIBSRC)/tbl_ufs.h
	rm -f $(DICOM_LIBSRC)/tblprivate.h $(DICOM_LIBSRC)/tblcond.c
	rm -f $(DICOM_LIBSRC)/tbl_sqlserver.c $(DICOM_LIBSRC)/tbl_sqlserver.h
	rm -f $(DICOM_LIBSRC)/tbl_sybase.c $(DICOM_LIBSRC)/tbl_sybase.h
	rm -f $(DICOM_LIBSRC)/tbl_msql.c $(DICOM_LIBSRC)/tbl_msql.h
	rm -f $(DICOM_LIBSRC)/tbl_mysql.c $(DICOM_LIBSRC)/tbl_mysql.h
	rm -f $(DICOM_LIBSRC)/tbl_psql.c $(DICOM_LIBSRC)/tbl_psql.h
	rm -f $(DICOM_LIBSRC)/ufs.c
	ln -s ../facilities/tbl/tbl.h $(DICOM_LIBSRC)
	ln -s ../facilities/tbl/tbl_ufs.h $(DICOM_LIBSRC)
	ln -s ../facilities/tbl/tblprivate.h $(DICOM_LIBSRC)
	ln -s ../facilities/tbl/tblcond.c $(DICOM_LIBSRC)
	ln -s ../facilities/tbl/tbl_sqlserver.c $(DICOM_LIBSRC)
	ln -s ../facilities/tbl/tbl_sqlserver.h $(DICOM_LIBSRC)
	ln -s ../facilities/tbl/tbl_sybase.c $(DICOM_LIBSRC)
	ln -s ../facilities/tbl/tbl_sybase.h $(DICOM_LIBSRC)
	ln -s ../facilities/tbl/tbl_msql.c $(DICOM_LIBSRC)
	ln -s ../facilities/tbl/tbl_msql.h $(DICOM_LIBSRC)
	ln -s ../facilities/tbl/tbl_mysql.c $(DICOM_LIBSRC)
	ln -s ../facilities/tbl/tbl_mysql.h $(DICOM_LIBSRC)
	ln -s ../facilities/tbl/tbl_psql.c $(DICOM_LIBSRC)
	ln -s ../facilities/tbl/tbl_psql.h $(DICOM_LIBSRC)
	ln -s ../facilities/tbl/ufs.c $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *.BAK *% core

indent:
#	$(INDENT) tbl.h
#	$(INDENT) tbl_ufs.h
#	$(INDENT) tblprivate.h
	$(INDENT) tblcond.c
	$(INDENT) tbl_sybase.c
#	$(INDENT) tbl_sybase.h
	$(INDENT) tbl_msql.c
	$(INDENT) tbl_mysql.c
	$(INDENT) tbl_psql.c
#	$(INDENT) tbl_msql.h
#	$(INDENT) tbl_mysql.h
	$(INDENT) ufs.c

checkin:
				$(CI) tbl.h
				$(CI) tbl_ufs.h
				$(CI) tblprivate.h
	$(INDENT) tblcond.c;	$(CI) tblcond.c
	$(INDENT) tbl_sybase.c;	$(CI) tbl_sybase.c
				$(CI) tbl_sybase.h
	$(INDENT) tbl_msql.c;	$(CI) tbl_msql.c
				$(CI) tbl_msql.h
	$(INDENT) tbl_mysql.c;	$(CI) tbl_mysql.c
				$(CI) tbl_mysql.h
	$(INDENT) tbl_psql.c;	$(CI) tbl_psql.c
				$(CI) tbl_psql.h
				$(CI) tbl_sqlserver.c
				$(CI) tbl_sqlserver.h
	$(INDENT) ufs.c;	$(CI) ufs.c
	rm -f *.a *.o *.BAK *% core

checkout:
	$(CO) tbl.h
	$(CO) tbl_ufs.h
	$(CO) tblprivate.h
	$(CO) tblcond.c
	$(CO) tbl_sybase.c
	$(CO) tbl_sybase.h
	$(CO) tbl_msql.c
	$(CO) tbl_msql.h
	$(CO) tbl_mysql.c
	$(CO) tbl_mysql.h
	$(CO) tbl_psql.c
	$(CO) tbl_psql.h
	$(CO) tbl_sqlserver.c
	$(CO) tbl_sqlserver.h
	$(CO) ufs.c

libtbl_sybase.a:	$(SYBASE_OBJS)
	ar cvr libtbl_sybase.a $(SYBASE_OBJS); $(RANLIB) libtbl_sybase.a
libtbl_msql.a:	$(MSQL_OBJS)
	ar cvr libtbl_msql.a $(MSQL_OBJS); $(RANLIB) libtbl_msql.a
libtbl_mysql.a:	$(MYSQL_OBJS)
	ar cvr libtbl_mysql.a $(MYSQL_OBJS); $(RANLIB) libtbl_mysql.a
libtbl_psql.a:	$(PSQL_OBJS)
	ar cvr libtbl_psql.a $(PSQL_OBJS); $(RANLIB) libtbl_psql.a
libtbl_ufs.a:	$(UFS_OBJS)
	ar cvr libtbl_ufs.a $(UFS_OBJS); $(RANLIB) libtbl_ufs.a

tblcond.o:	tbl.h tblcond.c
tbl_sybase.o:	tbl.h tbl_sybase.c tbl_sybase.h
tbl_msql.o:	tbl.h tbl_msql.c tbl_msql.h
tbl_mysql.o:	tbl.h tbl_mysql.c tbl_mysql.h
tbl_psql.o:	tbl.h tbl_psql.c tbl_psql.h
ufs.o:		tbl.h tbl_ufs.h ufs.c

lint:
	flexelint $(LINTOPTIONS)  tbl_sybase.c tblcond.c
	flexelint $(LINTOPTIONS)  tbl_msql.c tblcond.c
	flexelint $(LINTOPTIONS)  tbl_mysql.c tblcond.c
	flexelint $(LINTOPTIONS)  tbl_psql.c tblcond.c
	flexelint $(LINTOPTIONS)  ufs.c tblcond.c
