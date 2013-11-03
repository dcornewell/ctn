#	Makefile for Image AP facility

OBJS = iap.o iapcond.o

include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)

library:	libiap.a
install:	libiap.a
	cp libiap.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libiap.a
	$(INSTALLINCLUDE) iap.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:
	$(INSTALLINCLUDE) iap.h $(DICOM_INCLUDE)

links:
	rm -f $(DICOM_LIBSRC)/iap.h $(DICOM_LIBSRC)/iap.c $(DICOM_LIBSRC)/iapcond.c
	ln -s ../facilities/iap/iap.h $(DICOM_LIBSRC)
	ln -s ../facilities/iap/iap.c $(DICOM_LIBSRC)
	ln -s ../facilities/iap/iapcond.c $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *.BAK *% core

indent:
#	$(INDENT) iap.h
	$(INDENT) iap.c
	$(INDENT) iapcond.c
checkin:
				$(CI) iap.h
	$(INDENT) iap.c;	$(CI) iap.c
	$(INDENT) iapcond.c;	$(CI) iapcond.c
	rm -f *.a *.o *.BAK *% core

checkout:
	$(CO) iap.h
	$(CO) iap.c
	$(CO) iapcond.c

libiap.a:	$(OBJS)
	ar cvr libiap.a $(OBJS); $(RANLIB) libiap.a

iap.o: iap.c iap.h
iapcond.o: iapcond.c iap.h

lint:
	flexelint $(LINTOPTIONS) iap.c iapcond.c
