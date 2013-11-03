#	Makefile for IRS facility

OBJS = irs.o irscond.o
include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) -DDEBUG

library:	libirs.a

install:	libirs.a
	cp libirs.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libirs.a
	$(INSTALLINCLUDE) dicom_irs.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:

links:
	rm -f $(DICOM_LIBSRC)/irs.c
	rm -f $(DICOM_LIBSRC)/irscond.c
	rm -f $(DICOM_LIBSRC)/irs_private.h
	rm -f $(DICOM_LIBSRC)/dicom_irs.h

	ln -s ../facilities/irs/irs.c $(DICOM_LIBSRC)
	ln -s ../facilities/irs/irscond.c $(DICOM_LIBSRC)
	ln -s ../facilities/irs/irs_private.h $(DICOM_LIBSRC)
	ln -s ../facilities/irs/dicom_irs.h $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *.BAK *% core *~

indent:
	$(INDENT) irs.c
	$(INDENT) irscond.c

checkin:
	$(INDENT) irs.c;	$(CI) irs.c
	$(INDENT) irscond.c;	$(CI) irscond.c
				$(CI) irs_private.h
				$(CI) dicom_irs.h
	rm -f *.a *.o *.BAK *% core *~

checkout:
	$(CO) irs.c
	$(CO) irscond.c
	$(CO) irs_private.h
	$(CO) dicom_irs.h

libirs.a:	$(OBJS)
	ar cvr libirs.a $(OBJS); $(RANLIB) libirs.a

irs.o:	irs.c dicom_irs.h irs_private.h

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
