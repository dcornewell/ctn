#	Makefile for binary logging facility

OBJS = blg.o

include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)

library:

install:
	$(INSTALLINCLUDE) blg.h $(DICOM_INCLUDE)

headerexport:
	$(INSTALLINCLUDE) blg.h $(DICOM_INCLUDE)

links:
	rm -f $(DICOM_LIBSRC)/blg.h
	ln -s ../facilities/blg/blg.h $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *BAK *% core

indent:
	$(INDENT) blg.h


checkin:
	$(INDENT) blg.h; $(CI) blg.h
	rm -f *.a *.o *BAK *% core

checkout:
	$(CO) blg.h

#libblg.a:	$(OBJS)
#	ar cvr libblg.a $(OBJS); $(RANLIB) libblg.a


#blg.o: blg.c dicom.h blg.h

lint:
