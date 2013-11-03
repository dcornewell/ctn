#	Makefile for Image Copy facility

include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) -DDEBUG

library:

install:
	$(INSTALLINCLUDE) icpy.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:

links:

clean:
	rm -f *.a *.o *.BAK *% core

indent:
#	$(INDENT) icpy.h

checkin:
			$(CI) icpy.h
	rm -f *.a *.o *.BAK *% core

checkout:
	$(CO) icpy.h

lint:
