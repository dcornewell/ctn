#	Makefile for condition facility

OBJS = condition.o

include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)

library:	libcondition.a

install:	libcondition.a
	cp libcondition.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libcondition.a
	$(INSTALLINCLUDE) condition.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:
	$(INSTALLINCLUDE) condition.h $(DICOM_INCLUDE)

# softlinks to library source so we can edit one copy of the code
links:
	rm -f $(DICOM_LIBSRC)/condition.c $(DICOM_LIBSRC)/condition.h
	ln -s ../facilities/condition/condition.c $(DICOM_LIBSRC)
	ln -s ../facilities/condition/condition.h $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *BAK *% core

indent:
	$(INDENT) condition.c
#	$(INDENT) condition.h


checkin:
	$(INDENT) condition.c; $(CI) condition.c
	                       $(CI) condition.h
	rm -f *.a *.o *BAK *% core

checkout:
	$(CO) condition.c
	$(CO) condition.h

libcondition.a:	$(OBJS)
	ar cvr libcondition.a $(OBJS); $(RANLIB) libcondition.a


condition.o: condition.c dicom.h condition.h

lint:
	flexelint $(LINTOPTIONS) condition.c
