#	Makefile for DICOM print

OBJS = print.o printcond.o
include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)

library:	libprint.a
install:	libprint.a
	cp libprint.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libprint.a
	$(INSTALLINCLUDE) dicom_print.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:
	$(INSTALLINCLUDE) dicom_print.h $(DICOM_INCLUDE)

links:
	rm -f $(DICOM_LIBSRC)/dicom_print.h $(DICOM_LIBSRC)/print.c
	rm -f $(DICOM_LIBSRC)/printcond.c
	ln -s ../facilities/print/dicom_print.h $(DICOM_LIBSRC)
	ln -s ../facilities/print/print.c $(DICOM_LIBSRC)
	ln -s ../facilities/print/printcond.c $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *BAK *% core

indent:
#	$(INDENT) dicom_print.h
	$(INDENT) print.c
	$(INDENT) printcond.c
checkin:
				$(CI) dicom_print.h
	$(INDENT) print.c;	$(CI) print.c
	$(INDENT) printcond.c;	$(CI) printcond.c
	rm -f *.a *.o *BAK *% core

checkout:
	$(CO) dicom_print.h
	$(CO) print.c
	$(CO) printcond.c

libprint.a:	$(OBJS)
	ar cvr libprint.a $(OBJS); $(RANLIB) libprint.a

print.o:	print.c dicom.h condition.h dicom_print.h \
dicom_messages.h dicom_uids.h dicom_objects.h dicom_sq.h

printcond.o:	printcond.c dicom.h condition.h dicom_print.h \
dicom_messages.h dicom_uids.h dicom_objects.h

lint:
	flexelint $(LINTOPTIONS) print.c
