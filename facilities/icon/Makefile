#	Makefile for ICON facility

OBJS = icon.o iconcond.o

include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)

library:	libicon.a
install:	libicon.a
	cp libicon.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libicon.a
	$(INSTALLINCLUDE) icon.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:
	$(INSTALLINCLUDE) icon.h $(DICOM_INCLUDE)

links:
	rm -f $(DICOM_LIBSRC)/icon.c $(DICOM_LIBSRC)/icon.h $(DICOM_LIBSRC)/iconcond.c
	ln -s ../facilities/icon/icon.c $(DICOM_LIBSRC)
	ln -s ../facilities/icon/icon.h $(DICOM_LIBSRC)
	ln -s ../facilities/icon/iconcond.c $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *BAK *% core

indent:
	$(INDENT) icon.c
#	$(INDENT) icon.h
	$(INDENT) iconcond.c

checkin:
	$(INDENT) icon.c;	$(CI) icon.c
				$(CI) icon.h
	$(INDENT) iconcond.c;	$(CI) iconcond.c
	rm -f *.a *.o *BAK *% core

checkout:
	$(CO) icon.c
	$(CO) icon.h
	$(CO) iconcond.c

libicon.a:	$(OBJS)
	ar cvr libicon.a $(OBJS); $(RANLIB) libicon.a

icon.o: icon.c icon.h

iconcond.o: iconcond.c icon.h

lint:
	flexelint $(LINTOPTIONS) icon.c
