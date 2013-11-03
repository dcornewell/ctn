#	Makefile for FIS facility

OBJS = fis.o fiscond.o build.o fisdelete.o event.o fisget.o fisinsert.o record.o update.o
include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) -DDEBUG

library:	libfis.a

install:	libfis.a
	cp libfis.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libfis.a
	$(INSTALLINCLUDE) fis.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:
	$(INSTALLINCLUDE) fis.h $(DICOM_INCLUDE)

links:
	rm -f $(DICOM_LIBSRC)/build.c $(DICOM_LIBSRC)/fisdelete.c
	rm -f $(DICOM_LIBSRC)/event.c $(DICOM_LIBSRC)/fis.c
	rm -f $(DICOM_LIBSRC)/fiscond.c $(DICOM_LIBSRC)/fisget.c
	rm -f $(DICOM_LIBSRC)/fisinsert.c $(DICOM_LIBSRC)/record.c
	rm -f $(DICOM_LIBSRC)/update.c $(DICOM_LIBSRC)/fis.h
	rm -f $(DICOM_LIBSRC)/fis_private.h
	ln -s ../facilities/fis/build.c $(DICOM_LIBSRC)
	ln -s ../facilities/fis/fisdelete.c $(DICOM_LIBSRC)
	ln -s ../facilities/fis/event.c $(DICOM_LIBSRC)
	ln -s ../facilities/fis/fis.c $(DICOM_LIBSRC)
	ln -s ../facilities/fis/fiscond.c $(DICOM_LIBSRC)
	ln -s ../facilities/fis/fisget.c $(DICOM_LIBSRC)
	ln -s ../facilities/fis/fisinsert.c $(DICOM_LIBSRC)
	ln -s ../facilities/fis/record.c $(DICOM_LIBSRC)
	ln -s ../facilities/fis/update.c $(DICOM_LIBSRC)
	ln -s ../facilities/fis/fis.h $(DICOM_LIBSRC)
	ln -s ../facilities/fis/fis_private.h $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *.BAK *% core

indent:
	$(INDENT) build.c
	$(INDENT) fisdelete.c
	$(INDENT) event.c
	$(INDENT) fis.c
	$(INDENT) fiscond.c
	$(INDENT) fisget.c
	$(INDENT) fisinsert.c
	$(INDENT) record.c
	$(INDENT) update.c
#	$(INDENT) fis.h
#	$(INDENT) fis_private.h

checkin:
	$(INDENT) build.c;	$(CI) build.c
	$(INDENT) fisdelete.c;	$(CI) fisdelete.c
	$(INDENT) event.c;	$(CI) event.c
	$(INDENT) fis.c;	$(CI) fis.c
	$(INDENT) fiscond.c;	$(CI) fiscond.c
	$(INDENT) fisget.c;	$(CI) fisget.c
	$(INDENT) fisinsert.c;	$(CI) fisinsert.c
	$(INDENT) record.c;	$(CI) record.c
	$(INDENT) update.c;	$(CI) update.c
				$(CI) fis.h
				$(CI) fis_private.h
	rm -f *.a *.o *.BAK *% core

checkout:
	$(CO) build.c
	$(CO) fisdelete.c
	$(CO) event.c
	$(CO) fis.c
	$(CO) fiscond.c
	$(CO) fisget.c
	$(CO) fisinsert.c
	$(CO) record.c
	$(CO) update.c
	$(CO) fis.h
	$(CO) fis_private.h

libfis.a:	$(OBJS)
	ar cvr libfis.a $(OBJS); $(RANLIB) libfis.a

build.o:	build.c fis.h fis_private.h
fisdelete.o:	fisdelete.c fis.h fis_private.h
event.o:	event.c fis.h fis_private.h
fis.o:		fis.c fis.h fis_private.h
fiscond.o:	fiscond.c fis.h
fisget.o:	fisget.c fis.h fis_private.h
fisinsert.o:	fisinsert.c fis.h fis_private.h
record.o:	record.c fis.h fis_private.h
update.o:	update.c fis.h fis_private.h

lint:
	flexelint $(LINTOPTIONS) fis.c fiscond.c build.c fisdelete.c event.c fisget.c fisinsert.c record.c update.c
