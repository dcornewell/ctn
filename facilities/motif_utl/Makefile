#	Makefile for Motif Utility facility

OBJS = motifutil.o
include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) $(CFLAGS_MOTIF) $(CFLAGS_X11) -DDEBUG

library:	libmut.a

install:	libmut.a
	cp libmut.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libmut.a
	$(INSTALLINCLUDE) mut.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:

links:
	rm -f $(DICOM_LIBSRC)/mut.h $(DICOM_LIBSRC)/motifutil.c
	ln -s ../facilities/motif_utl/mut.h $(DICOM_LIBSRC)
	ln -s ../facilities/motif_utl/motifutil.c $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *.BAK *% core

indent:
#	$(INDENT) mut.h
	$(INDENT) motifutil.c

checkin:
				$(CI) mut.h
	$(INDENT) motifutil.c;	$(CI) motifutil.c
	rm -f *.a *.o *.BAK *% core

checkout:
	$(CO) mut.h
	$(CO) motifutil.c

libmut.a:	$(OBJS)
	ar cvr libmut.a $(OBJS); $(RANLIB) libmut.a

motifutil.o:	mut.h motifutil.c

lint:
	flexelint $(LINTOPTIONS) motifutil.c

