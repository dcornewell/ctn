#	Makefile for AP facility

OBJS = hapcond.o hapbuildpatient.o hapbuildvisit.o hapbuildresults.o \
       hapbuildinterp.o hapbuildstudy.o

include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)

library:	libhap.a
install:	libhap.a
	cp libhap.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libhap.a
	$(INSTALLINCLUDE) hap.h $(DICOM_INCLUDE)

export:
	cp hap.h hapcond.c hapbuildpatient.c hapbuildvisit.c $(DICOM_LIBSRC)
	cp hapbuildstudy.c hapbuildresults.c hapbuildinterp.c  $(DICOM_LIBSRC)

headerexport:

links:
	rm -f $(DICOM_LIBSRC)/hap.h $(DICOM_LIBSRC)/hap.c $(DICOM_LIBSRC)/hapcond.c
	rm -f $(DICOM_LIBSRC)/hapbuildpatient.c $(DICOM_LIBSRC)/hapbuildvisit.c
	rm -f $(DICOM_LIBSRC)/hapbuildstudy.c $(DICOM_LIBSRC)/hapbuildresults.c
	rm -f $(DICOM_LIBSRC)/hapbuildinterp.c $(DICOM_LIBSRC)/hapbuildresults.c
	ln -s ../facilities/hap/hap.h $(DICOM_LIBSRC)
	ln -s ../facilities/hap/hapcond.c $(DICOM_LIBSRC)
	ln -s ../facilities/hap/hapbuildpatient.c $(DICOM_LIBSRC)
	ln -s ../facilities/hap/hapbuildvisit.c $(DICOM_LIBSRC)
	ln -s ../facilities/hap/hapbuildstudy.c $(DICOM_LIBSRC)
	ln -s ../facilities/hap/hapbuildresults.c $(DICOM_LIBSRC)
	ln -s ../facilities/hap/hapbuildinterp.c $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *.BAK *% core

indent:
#	$(INDENT) hap.h
	$(INDENT) hap.c
	$(INDENT) hapcond.c
	$(INDENT) hapbuildpatient.c
	$(INDENT) hapbuildvisit.c
	$(INDENT) hapbuildstudy.c
	$(INDENT) hapbuildresults.c
	$(INDENT) hapbuildinterp.c
checkin:
					$(CI) hap.h
	$(INDENT) hap.c;		$(CI) hap.c
	$(INDENT) hapcond.c;		$(CI) hapcond.c
	$(INDENT) hapbuildpatient.c;	$(CI) hapbuildpatient.c
	$(INDENT) hapbuildvisit.c;	$(CI) hapbuildvisit.c
	$(INDENT) hapbuildstudy.c;	$(CI) hapbuildstudy.c
	$(INDENT) hapbuildresults.c;	$(CI) hapbuildresults.c
	$(INDENT) hapbuildinterp.c;	$(CI) hapbuildinterp.c
	rm -f *.a *.o *.BAK *% core

checkout:
	$(CO) hap.h
	$(CO) hap.c
	$(CO) hapcond.c
	$(CO) hapbuildpatient.c
	$(CO) hapbuildvisit.c
	$(CO) hapbuildstudy.c
	$(CO) hapbuildresults.c
	$(CO) hapbuildinterp.c

libhap.a:	$(OBJS)
	ar cvr libhap.a $(OBJS); $(RANLIB) libhap.a

hap.o: hap.c hap.h
hapcond.o: hapcond.c hap.h
hapbuildpatient.o: hapbuildpatient.c hap.h
hapbuildvisit.o: hapbuildvisit.c hap.h
hapbuildstudy.o: hapbuildstudy.c hap.h
hapbuildresults.o: hapbuildresults.c hap.h
hapbuildinterp.o: hapbuildinterp.c hap.h

lint:
#	flexelint $(LINTOPTIONS) hap.c hapcond.c
