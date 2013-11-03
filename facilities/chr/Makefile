#	Makefile for dicom character set encoding routines

OBJS = dicom_chr.o

include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)

library:	libdcmchr.a
install:	libdcmchr.a
	cp libdcmchr.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libdcmchr.a
	$(INSTALLINCLUDE) dicom_chr.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:
	$(INSTALLINCLUDE) dicom_chr.h $(DICOM_INCLUDE)

links:
	rm -f $(DICOM_LIBSRC)/dicom_chr.h $(DICOM_LIBSRC)/dicom_chr.c
	ln -s ../facilities/chr/dicom_chr.h $(DICOM_LIBSRC)
	ln -s ../facilities/chr/dicom_chr.c $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *.BAK *% core

indent:
#	$(INDENT) dicom_objects.h
	$(INDENT) dcm.c
	$(INDENT) dcm1.c
	$(INDENT) dcmcond.c
	$(INDENT) dcmdict.c
#	$(INDENT) dcmprivate.h
	$(INDENT) dcmsupport.c

checkin:
				$(CI) dicom_objects.h
	$(INDENT) dcm.c;        $(CI) dcm.c
	$(INDENT) dcm1.c;       $(CI) dcm1.c
	$(INDENT) dcmcond.c;    $(CI) dcmcond.c
	$(INDENT) dcmdict.c;    $(CI) dcmdict.c
				$(CI) dcmprivate.h
	$(INDENT) dcmsupport.c; $(CI) dcmsupport.c
	rm -f *.a *.o *.BAK *% core

checkout:
	$(CO) dicom_objects.h
	$(CO) dcm.c
	$(CO) dcm1.c
	$(CO) dcmcond.c
	$(CO) dcmdict.c
	$(CO) dcmprivate.h
	$(CO) dcmsupport.c

libdcmchr.a:	$(OBJS)
	ar cvr libdcmchr.a $(OBJS); $(RANLIB) libdcmchr.a

dicom_chr.o: dicom_chr.c dicom_chr.h
lint:
	flexelint $(LINTOPTIONS) dcm.c dcm1.c dcmcond.c dcmsupport.c
