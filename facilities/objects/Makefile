#	Makefile for dicom information object libary files.

OBJS = dcm.o dcm1.o dcmdict.o dcmcond.o dcmsupport.o

include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)

library:	libdcmobjects.a
install:	libdcmobjects.a
	cp libdcmobjects.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libdcmobjects.a
	$(INSTALLINCLUDE) dicom_objects.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:
	$(INSTALLINCLUDE) dicom_objects.h $(DICOM_INCLUDE)

links:
	rm -f $(DICOM_LIBSRC)/dicom_objects.h $(DICOM_LIBSRC)/dcm.c
	rm -f $(DICOM_LIBSRC)/dcm1.c $(DICOM_LIBSRC)/dcmcond.c
	rm -f $(DICOM_LIBSRC)/dcmdict.c $(DICOM_LIBSRC)/dcmprivate.h
	rm -f $(DICOM_LIBSRC)/dcmsupport.c
	ln -s ../facilities/objects/dicom_objects.h $(DICOM_LIBSRC)
	ln -s ../facilities/objects/dcm.c $(DICOM_LIBSRC)
	ln -s ../facilities/objects/dcm1.c $(DICOM_LIBSRC)
	ln -s ../facilities/objects/dcmcond.c $(DICOM_LIBSRC)
	ln -s ../facilities/objects/dcmdict.c $(DICOM_LIBSRC)
	ln -s ../facilities/objects/dcmprivate.h $(DICOM_LIBSRC)
	ln -s ../facilities/objects/dcmsupport.c $(DICOM_LIBSRC)

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

libdcmobjects.a:	$(OBJS)
	ar cvr libdcmobjects.a $(OBJS); $(RANLIB) libdcmobjects.a

dcm.o: dcm.c dicom_objects.h dcmprivate.h lst.h
dcm1.o: dcm1.c dicom_objects.h dcmprivate.h lst.h
dcmcond.o: dcmcond.c dicom_objects.h dcmprivate.h lst.h
dcmdict.o: dcmdict.c dicom_objects.h dcmprivate.h lst.h
dcmsupport.o: dcmsupport.c dicom_objects.h dcmprivate.h lst.h
lint:
	flexelint $(LINTOPTIONS) dcm.c dcm1.c dcmcond.c dcmsupport.c
