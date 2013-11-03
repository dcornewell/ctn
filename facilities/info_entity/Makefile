#	Makefile for dicom information entity facility

OBJS = ie.o iecond.o 

include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) 

library:	libie.a
install:	libie.a
	cp libie.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libie.a
	$(INSTALLINCLUDE) dicom_ie.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:
	$(INSTALLINCLUDE) dicom_ie.h $(DICOM_INCLUDE)
links:
	rm -f $(DICOM_LIBSRC)/dicom_ie.h $(DICOM_LIBSRC)/ie.c
	rm -f $(DICOM_LIBSRC)/iecond.c $(DICOM_LIBSRC)/tables.h
	ln -s ../facilities/info_entity/dicom_ie.h $(DICOM_LIBSRC)
	ln -s ../facilities/info_entity/ie.c $(DICOM_LIBSRC)
	ln -s ../facilities/info_entity/iecond.c $(DICOM_LIBSRC)
	ln -s ../facilities/info_entity/tables.h $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *.BAK *% core

indent:
#	$(INDENT) dicom_ie.h
	$(INDENT) ie.c
	$(INDENT) iecond.c
#	$(INDENT) tables.h
checkin:
				$(CI) dicom_ie.h
	$(INDENT) ie.c;		$(CI) ie.c
	$(INDENT) iecond.c;	$(CI) iecond.c
				$(CI) tables.h
	rm -f *.a *.o *.BAK *% core

checkout:
	$(CO) dicom_ie.h
	$(CO) ie.c
	$(CO) iecond.c
	$(CO) tables.h

libie.a:	$(OBJS)
	ar cvr libie.a $(OBJS); $(RANLIB) libie.a

ie.o: ie.c dicom_objects.h dicom_ie.h tables.h
iecond.o: iecond.c dicom_ie.h

lint:
	flexelint $(LINTOPTIONS) ie.c
