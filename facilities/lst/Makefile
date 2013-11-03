#	Makefile for LST facility

OBJS = lst.o lstcond.o

include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)

library:	liblst.a
install:	liblst.a
	cp liblst.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/liblst.a
	$(INSTALLINCLUDE) lst.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:
	$(INSTALLINCLUDE) lst.h $(DICOM_INCLUDE)

links:
	rm -f $(DICOM_LIBSRC)/lst.c $(DICOM_LIBSRC)/lst.h
	rm -f $(DICOM_LIBSRC)/lstprivate.h $(DICOM_LIBSRC)/lstcond.c
	ln -s ../facilities/lst/lst.c $(DICOM_LIBSRC)
	ln -s ../facilities/lst/lst.h $(DICOM_LIBSRC)
	ln -s ../facilities/lst/lstprivate.h $(DICOM_LIBSRC)
	ln -s ../facilities/lst/lstcond.c $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *.BAK *% core

indent:
	$(INDENT) lst.c
#	$(INDENT) lst.h
#	$(INDENT) lstprivate.h
	$(INDENT) lstcond.c
checkin:
	$(INDENT) lst.c;	$(CI) lst.c
				$(CI) lst.h
				$(CI) lstprivate.h
	$(INDENT) lstcond.c;	$(CI) lstcond.c
	rm -f *.a *.o *.BAK *% core

checkout:
	$(CO) lst.c
	$(CO) lst.h
	$(CO) lstprivate.h
	$(CO) lstcond.c

liblst.a:	$(OBJS)
	ar cvr liblst.a $(OBJS); $(RANLIB) liblst.a

lst.o: lst.c lst.h lstprivate.h
lstcond.o: lstcond.c lst.h lstprivate.h

lint:
	flexelint $(LINTOPTIONS) lst.c lstcond.c
