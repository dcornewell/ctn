#	Makefile for Image Management facility

OBJS = iman.o
include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) -DDEBUG

library:	libiman.a

install:	libiman.a
	cp libiman.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libiman.a
	$(INSTALLINCLUDE) iman.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:

links:

clean:
	rm -f *.a *.o *.BAK *% core

indent:
#	$(INDENT) iman.h
	$(INDENT) iman.c
	$(INDENT) mancond.c
checkin:
				$(CI) iman.h
	$(INDENT) iman.c;	$(CI) iman.c
	$(INDENT) mancond.c;	$(CI) mancond.c
	rm -f *.a *.o *.BAK *% core

checkout:
	$(CO) iman.h
	$(CO) iman.c
	$(CO) mancond.c

libiman.a:	$(OBJS)
	ar cvr libiman.a $(OBJS); $(RANLIB) libiman.a

iman.o:		iman.h iman.c
mancond.o:	iman.h mancond.c

lint:
	flexelint $(LINTOPTIONS) iman.c mancond.c
