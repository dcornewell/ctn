#	Makefile for CTN threads library

OBJS = ctnthread.o thrcond.o
include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)

library:	libctnthread.a
install:	libctnthread.a
	cp libctnthread.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libctnthread.a
	$(INSTALLINCLUDE) ctnthread.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:
	$(INSTALLINCLUDE) ctnthread.h $(DICOM_INCLUDE)

links:
	rm -f $(DICOM_LIBSRC)/ctnthread.h $(DICOM_LIBSRC)/ctnthread.c
	rm -f $(DICOM_LIBSRC)/thrcond.c 
	ln -s ../facilities/thread/ctnthread.h $(DICOM_LIBSRC)
	ln -s ../facilities/thread/ctnthread.c $(DICOM_LIBSRC)
	ln -s ../facilities/thread/thrcond.c $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *BAK *% core

indent:
#	$(INDENT) ctnthread.h
	$(INDENT) ctnthread.c
	$(INDENT) thrcond.c
checkin:
				$(CI) ctnthread.h
	$(INDENT) ctnthread.c;	$(CI) ctnthread.c
	$(INDENT) thrcond.c;	$(CI) thrcond.c
	rm -f *.a *.o *BAK *% core

checkout:
	$(CO) ctnthread.h
	$(CO) ctnthread.c
	$(CO) thrcond.c

libctnthread.a:	$(OBJS)
	ar cvr libctnthread.a $(OBJS); $(RANLIB) libctnthread.a

ctnthread.o:	ctnthread.c ctnthread.h
thrcond.o:	thrcond.c ctnthread.h

lint:
	flexelint $(LINTOPTIONS) ctnthread.c
