#	Makefile for DICOM database

OBJS = database.o
include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)

library:	libdatabase.a
install:	libdatabase.a
	cp libdatabase.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libdatabase.a
	$(INSTALLINCLUDE) dbquery.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:
	$(INSTALLINCLUDE) dbquery.h $(DICOM_INCLUDE)

links:
	rm -f $(DICOM_LIBSRC)/database.c $(DICOM_LIBSRC)/dbquery.h
	rm -f $(DICOM_LIBSRC)/dbprivate.h
	ln -s ../facilities/database/database.c $(DICOM_LIBSRC)
	ln -s ../facilities/database/dbquery.h $(DICOM_LIBSRC)
	ln -s ../facilities/database/dbprivate.h $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *BAK *% core

indent:
	$(INDENT) database.c
	$(INDENT) dbquery.h
	$(INDENT) dbprivate.h

checkin:
	$(INDENT) database.c;  $(CI) database.c
	                       $(CI) dbquery.h
	                       $(CI) dbprivate.h
	rm -f *.a *.o *BAK *% core

checkout:
	$(CO) database.c
	$(CO) dbquery.h
	$(CO) dbprivate.h

libdatabase.a:	$(OBJS)
	ar cvr libdatabase.a $(OBJS); $(RANLIB) libdatabase.a

database.o:	database.c dbquery.h dbprivate.h hunk_man.h condition.h dicom.h

lint:
	flexelint $(LINTOPTIONS) database.c
