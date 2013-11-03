#	Makefile for Database facility

IDB_OBJS = idb.o idbcond.o
include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) -DDEBUG

library:	libidb.a

install:	libidb.a
	cp libidb.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libidb.a
	$(INSTALLINCLUDE) idb.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:
	$(INSTALLINCLUDE) idb.h $(DICOM_INCLUDE)

links:
	rm -f $(DICOM_LIBSRC)/idb.h $(DICOM_LIBSRC)/idb.c $(DICOM_LIBSRC)/idbcond.c
	ln -s ../facilities/idb/idb.h $(DICOM_LIBSRC)
	ln -s ../facilities/idb/idb.c $(DICOM_LIBSRC)
	ln -s ../facilities/idb/idbcond.c $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *.BAK *% core

indent:
#	$(INDENT) idb.h
	$(INDENT) idb.c
	$(INDENT) idbcond.c

checkin:
				$(CI) idb.h
	$(INDENT) idbcond.c;	$(CI) idbcond.c
	$(INDENT) idb.c;	$(CI) idb.c
	rm -f *.a *.o *.BAK *% core

checkout:
	$(CO) idb.h
	$(CO) idbcond.c
	$(CO) idb.c

libidb.a:	$(IDB_OBJS)
	ar cvr libidb.a $(IDB_OBJS); $(RANLIB) libidb.a

idbcond.o:	tbl.h idb.h idbcond.c
idb.o:		idb.h idb.c

lint:
#	flexelint $(LINTOPTIONS)  idbcond.c idb.c
