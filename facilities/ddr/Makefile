#	Makefile for dicom directory libary files.

OBJS = ddr.o

include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)

library:	libddr.a
install:	libddr.a
	cp libddr.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libddr.a
	$(INSTALLINCLUDE) dicom_ddr.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:
	$(INSTALLINCLUDE) dicom_ddr.h $(DICOM_INCLUDE)

links:
	rm -f $(DICOM_LIBSRC)/dicom_ddr.h $(DICOM_LIBSRC)/ddr.c
	ln -s ../facilities/ddr/dicom_ddr.h $(DICOM_LIBSRC)
	ln -s ../facilities/ddr/ddr.c $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *.BAK *% *~ core

indent:
#	$(INDENT) dicom_ddr.h
	$(INDENT) ddr.c

checkin:
				$(CI) dicom_ddr.h
	$(INDENT) ddr.c;        $(CI) ddr.c
	rm -f *.a *.o *.BAK *% core

checkout:
	$(CO) dicom_ddr.h
	$(CO) ddr.c

libddr.a:	$(OBJS)
	ar cvr libddr.a $(OBJS); $(RANLIB) libddr.a

ddr.o: ddr.c dicom_objects.h dicom_ddr.h

lint:
	flexelint $(LINTOPTIONS) ddr.c
