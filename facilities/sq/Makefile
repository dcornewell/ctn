#	Makefile for DICOM sequences

OBJS = sequences.o sqcond.o

include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)

library:	libsq.a
install:	libsq.a
	cp libsq.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libsq.a
	$(INSTALLINCLUDE) dicom_sq.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:
	$(INSTALLINCLUDE) dicom_sq.h $(DICOM_INCLUDE)

links:
	rm -f $(DICOM_LIBSRC)/dicom_sq.h $(DICOM_LIBSRC)/sequences.c
	rm -f $(DICOM_LIBSRC)/sqcond.c
	ln -s ../facilities/sq/dicom_sq.h $(DICOM_LIBSRC)
	ln -s ../facilities/sq/sequences.c $(DICOM_LIBSRC)
	ln -s ../facilities/sq/sqcond.c $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *BAK *% core

indent:
#	$(INDENT) dicom_sq.h
	$(INDENT) sequences.c
	$(INDENT) sqcond.c
checkin:
				$(CI) dicom_sq.h
	$(INDENT) sequences.c;	$(CI) sequences.c
	$(INDENT) sqcond.c;	$(CI) sqcond.c
	rm -f *.a *.o *BAK *% core

checkout:
	$(CO) dicom_sq.h
	$(CO) sequences.c
	$(CO) sqcond.c

libsq.a:	$(OBJS)
	ar cvr libsq.a $(OBJS); $(RANLIB) libsq.a

sequences.o:	sequences.c dicom.h condition.h dicom_sq.h \
dicom_uids.h dicom_objects.h
sqcond.o:	sqcond.c dicom.h condition.h dicom_sq.h \
dicom_uids.h dicom_objects.h

lint:
	flexelint $(LINTOPTIONS) sequences.c sqcond.c
