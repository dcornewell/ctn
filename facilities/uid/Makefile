#	Makefile for DICOM UID functions

OBJS = uid.o uidcond.o
include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)

library:	libuid.a
install:	libuid.a
	cp libuid.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libuid.a
	$(INSTALLINCLUDE) dicom_uids.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:
	$(INSTALLINCLUDE) dicom_uids.h $(DICOM_INCLUDE)

links:
	rm -f $(DICOM_LIBSRC)/uid.c $(DICOM_LIBSRC)/uidcond.c
	rm -f $(DICOM_LIBSRC)/dicom_uids.h
	ln -s ../facilities/uid/uid.c $(DICOM_LIBSRC)
	ln -s ../facilities/uid/uidcond.c $(DICOM_LIBSRC)
	ln -s ../facilities/uid/dicom_uids.h $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *BAK *% core

indent:
#	$(INDENT) dicom_uids.h
	$(INDENT) uid.c
	$(INDENT) uidcond.c

checkin:
				$(CI) dicom_uids.h
	$(INDENT) uid.c;	$(CI) uid.c
	$(INDENT) uidcond.c;	$(CI) uidcond.c
	rm -f *.a *.o *BAK *% core

checkout:
	$(CO) dicom_uids.h
	$(CO) uid.c
	$(CO) uidcond.c

libuid.a:	$(OBJS)
	ar cvr libuid.a $(OBJS); $(RANLIB) libuid.a

uid.o:	uid.c dicom.h dicom_uids.h
uidcond.o:	uidcond.c dicom.h dicom_uids.h

lint:
	flexelint $(LINTOPTIONS) uid.c uidcond.c
