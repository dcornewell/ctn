#	Makefile for DICOM SNP functions

OBJS = snp.o snpcond.o decode.o dlroutines.o
include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)

library:	libsnp.a
install:	libsnp.a
	cp libsnp.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libsnp.a
	$(INSTALLINCLUDE) snp.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:

links:
	rm -f $(DICOM_LIBSRC)/snp.h $(DICOM_LIBSRC)/snp.c 
	rm -f $(DICOM_LIBSRC)/snpcond.c $(DICOM_LIBSRC)/decode.c
	rm -f $(DICOM_LIBSRC)/dlroutines.c $(DICOM_LIBSRC)/decode.h 
	ln -s ../facilities/snp/snp.h $(DICOM_LIBSRC)
	ln -s ../facilities/snp/snp.c $(DICOM_LIBSRC)
	ln -s ../facilities/snp/snpcond.c $(DICOM_LIBSRC)
	ln -s ../facilities/snp/decode.c  $(DICOM_LIBSRC)
	ln -s ../facilities/snp/dlroutines.c $(DICOM_LIBSRC)
	ln -s ../facilities/snp/decode.h  $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *BAK *% core

indent:
#	$(INDENT) snp.h
	$(INDENT) snp.c
	$(INDENT) snpcond.c
	$(INDENT) decode.c 
	$(INDENT) dlroutines.c
#	$(INDENT) decode.h 

checkin:
				$(CI) snp.h
	$(INDENT) snp.c;	$(CI) snp.c
	$(INDENT) snpcond.c;	$(CI) snpcond.c
	$(INDENT) decode.c;	$(CI) decode.c
	$(INDENT) dlroutines.c;	$(CI) dlroutines.c
				$(CI) decode.h 
	rm -f *.a *.o *BAK *% core

checkout:
	$(CO) snp.h
	$(CO) snp.c
	$(CO) snpcond.c
	$(CO) decode.c 
	$(CO) dlroutines.c
	$(CO) decode.h 

libsnp.a:	$(OBJS)
	ar cvr libsnp.a $(OBJS); $(RANLIB) libsnp.a

snp.o:		snp.c dicom.h snp.h	 
snpcond.o:	snpcond.c dicom.h snp.h
decode.o:	decode.c decode.h dicom.h snp.h
dlroutines.o: 	dlroutines.c dicom.h snp.h

lint:
	flexelint $(LINTOPTIONS) snp.c snpcond.c dlroutines.c
