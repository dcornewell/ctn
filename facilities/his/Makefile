#	Makefile for DICOM hisdb

OBJS = hisdb.o
include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)

library:	libhisdb.a
install:	libhisdb.a
	cp libhisdb.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libhisdb.a
	$(INSTALLINCLUDE) hisdb.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:

links:
	rm -f $(DICOM_LIBSRC)/hisdb.c $(DICOM_LIBSRC)/hisdb.h
	rm -f $(DICOM_LIBSRC)/hisprivate.h
	ln -s ../facilities/his/hisdb.c $(DICOM_LIBSRC)
	ln -s ../facilities/his/hisdb.h $(DICOM_LIBSRC)
	ln -s ../facilities/his/hisprivate.h $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *BAK *% core

indent:
	$(INDENT) hisdb.c
#	$(INDENT) hisdb.h
#	$(INDENT) hisprivate.h
checkin:
	$(INDENT) hisdb.c;	$(CI) hisdb.c
				$(CI) hisdb.h
				$(CI) hisprivate.h
	rm -f *.a *.o *BAK *% core

checkout:
	$(CO) hisdb.c
	$(CO) hisdb.h
	$(CO) hisprivate.h

libhisdb.a:	$(OBJS)
	ar cvr libhisdb.a $(OBJS); $(RANLIB) libhisdb.a

hisdb.o:	hisdb.c hisdb.h hisprivate.h hunk_man.h condition.h dicom.h

lint:
#	flexelint $(LINTOPTIONS) hisdb.c
