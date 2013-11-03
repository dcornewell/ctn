#	Makefile for DICOM hunks

OBJS = hunk_man.o
include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)

library:	libhunk_man.a
install:	libhunk_man.a
	cp libhunk_man.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libhunk_man.a
	$(INSTALLINCLUDE) hunk_man.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:

links:
	rm -f $(DICOM_LIBSRC)/hunk_man.c $(DICOM_LIBSRC)/hunk_man.h
	ln -s ../facilities/hunks/hunk_man.c $(DICOM_LIBSRC)
	ln -s ../facilities/hunks/hunk_man.h $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *BAK *% core

$(INDENT):
	$(INDENT) hunk_man.c
#	$(INDENT) hunk_man.h

checkin:
	$(INDENT) hunk_man.c;	$(CI) hunk_man.c
				$(CI) hunk_man.h
	rm -f *.a *.o *BAK *% core

checkout:
	$(CO) hunk_man.c
	$(CO) hunk_man.h

libhunk_man.a:	$(OBJS)
	ar cvr libhunk_man.a $(OBJS); $(RANLIB) libhunk_man.a

hunk_man.o:	hunk_man.c hunk_man.h condition.h dicom.h

lint:
#	flexelint minimal.lnt hunk_man.c
