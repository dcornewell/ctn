#	Makefile for compress facility

COMPRESS_OBJS = compress.o
include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) -DDEBUG

library:	libdcmcompress.a

install:	libdcmcompress.a
	cp libdcmcompress.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libdcmcompress.a

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:

links:
	rm -f $(DICOM_LIBSRC)/dcm_compress_jpeg.c
	ln -s ../facilities/compress/dcm_compress_jpeg.c $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *.BAK *% core

libdcmcompress.a:	$(COMPRESS_OBJS)
	ar cvr libdcmcompress.a $(COMPRESS_OBJS); $(RANLIB) libdcmcompress.a

