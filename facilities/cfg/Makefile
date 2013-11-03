#	Makefile for CFG facility

OBJS = cfg.o

include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)

library:	libcfg.a
install:	libcfg.a
	cp libcfg.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libcfg.a
	$(INSTALLINCLUDE) cfg.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:
	$(INSTALLINCLUDE) cfg.h $(DICOM_INCLUDE)

links:
	rm -f $(DICOM_LIBSRC)/cfg.c $(DICOM_LIBSRC)/cfg.h
	ln -s ../facilities/cfg/cfg.c $(DICOM_LIBSRC)
	ln -s ../facilities/cfg/cfg.h $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *BAK *% core

indent:
	$(INDENT) cfg.c
	$(INDENT) cfg.h
checkin:
	$(INDENT) cfg.c;     $(CI) cfg.c
	$(INDENT) cfg.h;     $(CI) cfg.h
	rm -f *.a *.o *BAK *% core

checkout:
	$(CO) cfg.c
	$(CO) cfg.h

libcfg.a:	$(OBJS)
	ar cvr libcfg.a $(OBJS); $(RANLIB) libcfg.a

cfg.o: cfg.c cfg.h

lint:
	flexelint $(LINTOPTIONS) cfg.c
