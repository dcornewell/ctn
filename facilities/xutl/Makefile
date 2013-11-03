#	Makefile for Motif Utility facility

OBJS = xutl.o
include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) $(CFLAGS_MOTIF) $(CFLAGS_X11) -DDEBUG

library:	libxutl.a

install:	libxutl.a
	cp libxutl.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libxutl.a
	$(INSTALLINCLUDE) xutl.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:
	$(INSTALLINCLUDE) xutl.h $(DICOM_INCLUDE)

links:
	rm -f $(DICOM_LIBSRC)/xutl.h $(DICOM_LIBSRC)/xutl.c $(DICOM_LIBSRC)/xutl_mb.c
	ln -s ../facilities/xutl/xutl.h $(DICOM_LIBSRC)
	ln -s ../facilities/xutl/xutl.c $(DICOM_LIBSRC)

links.mb:
	rm -f $(DICOM_LIBSRC)/xutl.h $(DICOM_LIBSRC)/xutl.c $(DICOM_LIBSRC)/xutl_mb.c
	ln -s ../facilities/xutl/xutl.h $(DICOM_LIBSRC)
	ln -s ../facilities/xutl/xutl.c $(DICOM_LIBSRC)
	ln -s ../facilities/xutl/xutl_mb.c $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *.BAK *% core

indent:
#	$(INDENT) xutl.h
	$(INDENT) xutl.c

checkin:
				$(CI) xutl.h
	$(INDENT) xutl.c;	$(CI) xutl.c
	rm -f *.a *.o *.BAK *% core

checkout:
	$(CO) xutl.h
	$(CO) xutl.c

libxutl.a:	$(OBJS)
	ar cvr libxutl.a $(OBJS); $(RANLIB) libxutl.a

xutl.o:	xutl.h xutl.c
xutl_mb.o:	xutl.h xutl_mb.c

lint:
	flexelint $(LINTOPTIONS) xutl.c

