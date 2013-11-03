#	Makefile for DICOM facility (only defines an include file)

include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)

library:
install:
	$(INSTALLINCLUDE) ctn_os.h $(DICOM_INCLUDE)
	$(INSTALLINCLUDE) dicom.h $(DICOM_INCLUDE)
	$(INSTALLINCLUDE) dicom_platform.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:
	$(INSTALLINCLUDE) ctn_os.h $(DICOM_INCLUDE)
	$(INSTALLINCLUDE) dicom.h $(DICOM_INCLUDE)
	$(INSTALLINCLUDE) dicom_platform.h $(DICOM_INCLUDE)
links:
	rm -f $(DICOM_LIBSRC)/dicom.h $(DICOM_LIBSRC)/dicom_platform.h $(DICOM_LIBSRC)/ctn_os.h
	ln -s ../facilities/dicom/ctn_os.h $(DICOM_LIBSRC)
	ln -s ../facilities/dicom/dicom.h $(DICOM_LIBSRC)
	ln -s ../facilities/dicom/dicom_platform.h $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *.BAK *% core

indent:
#	$(INDENT) dicom.h

checkin:
	$(CI) dicom.h
	$(CI) dicom_platform_w32.h
	$(CI) dicom_platform.h
	rm -f *.a *.o *.BAK *% core

checkout:
	$(CO) dicom.h
	$(CO) dicom_platform_w32.h
	$(CO) dicom_platform.h

lint:

