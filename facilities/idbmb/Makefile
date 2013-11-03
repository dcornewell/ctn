#	Makefile for Database facility

IDB_OBJS = idbmb.o idbmbcond.o
include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) -DDEBUG

library:	libidbmb.a

install:	libidbmb.a
	cp libidbmb.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libidbmb.a
	$(INSTALLINCLUDE) idbmb.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:

links:
	rm -f $(DICOM_LIBSRC)/idbmb.h $(DICOM_LIBSRC)/idbmb.c $(DICOM_LIBSRC)/idbmbcond.c
	ln -s ../facilities/idbmb/idbmb.h $(DICOM_LIBSRC)
	ln -s ../facilities/idbmb/idbmb.c $(DICOM_LIBSRC)
	ln -s ../facilities/idbmb/idbmbcond.c $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *.BAK *% core

indent:
#	$(INDENT) idbmb.h
	$(INDENT) idbmb.c
	$(INDENT) idbmbcond.c

checkin:
				$(CI) idbmb.h
	$(INDENT) idbmbcond.c;	$(CI) idbmbcond.c
	$(INDENT) idbmb.c;	$(CI) idbmb.c
	rm -f *.a *.o *.BAK *% core

checkout:
	$(CO) idbmb.h
	$(CO) idbmbcond.c
	$(CO) idbmb.c

libidbmb.a:	$(IDB_OBJS)
	ar cvr libidbmb.a $(IDB_OBJS); $(RANLIB) libidbmb.a

idbmbcond.o:	tbl.h idbmb.h idbmbcond.c
idbmb.o:		idbmb.h idbmb.c

lint:
#	flexelint $(LINTOPTIONS)  idbmbcond.c idbmb.c
