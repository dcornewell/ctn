#	Makefile for utility facility

UTILITY_OBJS = utility.o
include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) -DDEBUG

library:	libutility.a

install:	libutility.a
	cp libutility.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libutility.a
	$(INSTALLINCLUDE) utility.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:
	$(INSTALLINCLUDE) utility.h $(DICOM_INCLUDE)

links:
	rm -f $(DICOM_LIBSRC)/utility.h $(DICOM_LIBSRC)/utility.c
	ln -s ../facilities/utility/utility.h $(DICOM_LIBSRC)
	ln -s ../facilities/utility/utility.c $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *.BAK *% core

indent:
#	$(INDENT) utility.h
	$(INDENT) utility.c

checkin:
				$(CI) utility.h
	$(INDENT) utility.c;	$(CI) utility.c
	rm -f *.a *.o *.BAK *% core

checkout:
	$(CO) utility.h
	$(CO) utility.c

libutility.a:	$(UTILITY_OBJS)
	ar cvr libutility.a $(UTILITY_OBJS); $(RANLIB) libutility.a

utility.o:	utility.h utility.c

lint:
#	flexelint $(LINTOPTIONS)  utility.c
