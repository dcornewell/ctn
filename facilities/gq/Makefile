#	Makefile for Generalized Queue (GQ) facility

OBJS = gq.o
include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)

library:	libgq.a
install:	libgq.a
	cp libgq.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libgq.a
	$(INSTALLINCLUDE) gq.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:
	$(INSTALLINCLUDE) gq.h $(DICOM_INCLUDE)

links:
	rm -f $(DICOM_LIBSRC)/gq.c $(DICOM_LIBSRC)/gq.h
	ln -s ../facilities/gq/gq.c $(DICOM_LIBSRC)
	ln -s ../facilities/gq/gq.h $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *BAK *% core

indent:
	$(INDENT) gq.c
#	$(INDENT) gq.h

checkin:
	$(INDENT) gq.c; $(CI) gq.c
			$(CI) gq.h
	rm -f *.a *.o *BAK *% core

checkout:
	$(CO) gq.c
	$(CO) gq.h

libgq.a:	$(OBJS)
	ar cvr libgq.a $(OBJS); $(RANLIB) libgq.a

gq.o: gq.c gq.h dicom.h condition.h 

lint:
#	flexelint $(LINTOPTIONS) gq.c
