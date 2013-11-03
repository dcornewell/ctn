#	Makefile for DICOM Upper Layer Protocol facility

OBJS = dulprotocol.o dulconstruct.o dulparse.o dulcond.o dulfsm.o dulpresent.o dulsnoop.o
include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) -DDEBUG -DDUMP_DATA_PDU

library:	libdulprotocol.a
install:	libdulprotocol.a
	cp libdulprotocol.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libdulprotocol.a
	$(INSTALLINCLUDE) dulprotocol.h $(DICOM_INCLUDE) 

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:
	$(INSTALLINCLUDE) dulprotocol.h $(DICOM_INCLUDE) 

links:
	rm -f $(DICOM_LIBSRC)/dul*[ch]
	ln -s ../facilities/dulprotocol/dulprotocol.h $(DICOM_LIBSRC)
	ln -s ../facilities/dulprotocol/dulcond.c $(DICOM_LIBSRC)
	ln -s ../facilities/dulprotocol/dulconstruct.c $(DICOM_LIBSRC)
	ln -s ../facilities/dulprotocol/dulfsm.c $(DICOM_LIBSRC)
	ln -s ../facilities/dulprotocol/dulfsm.h $(DICOM_LIBSRC)
	ln -s ../facilities/dulprotocol/dulparse.c $(DICOM_LIBSRC)
	ln -s ../facilities/dulprotocol/dulpresent.c $(DICOM_LIBSRC)
	ln -s ../facilities/dulprotocol/dulprivate.h $(DICOM_LIBSRC)
	ln -s ../facilities/dulprotocol/dulprotocol.c $(DICOM_LIBSRC)
	ln -s ../facilities/dulprotocol/dulstructures.h $(DICOM_LIBSRC)
	ln -s ../facilities/dulprotocol/dulsnoop.c $(DICOM_LIBSRC)
	ln -s ../facilities/dulprotocol/dulsnoop.h $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *.BAK *% core

indent:
#	$(INDENT) dulprotocol.h
	$(INDENT) dulcond.c
	$(INDENT) dulconstruct.c
	$(INDENT) dulfsm.c
#	$(INDENT) dulfsm.h
	$(INDENT) dulparse.c
	$(INDENT) dulpresent.c
#	$(INDENT) dulprivate.h
	$(INDENT) dulprotocol.c
#	$(INDENT) dulstructures.h
	$(INDENT) dulsnoop.c
#	$(INDENT) dulsnoop.h


checkin:
	                           $(CI) dulprotocol.h
	$(INDENT) dulcond.c;       $(CI) dulcond.c
	$(INDENT) dulconstruct.c;  $(CI) dulconstruct.c
	$(INDENT) dulfsm.c;        $(CI) dulfsm.c
	                           $(CI) dulfsm.h
	$(INDENT) dulparse.c;      $(CI) dulparse.c
	$(INDENT) dulpresent.c;    $(CI) dulpresent.c
	                           $(CI) dulprivate.h
	$(INDENT) dulprotocol.c;   $(CI) dulprotocol.c
	                           $(CI) dulstructures.h
	$(INDENT) dulsnoop.c; 	   $(CI) dulsnoop.c
	                           $(CI) dulsnoop.h
	rm -f *.a *.o *.BAK *% core

checkout:
	$(CO) dulprotocol.h
	$(CO) dulcond.c
	$(CO) dulconstruct.c
	$(CO) dulfsm.c
	$(CO) dulfsm.h
	$(CO) dulparse.c
	$(CO) dulpresent.c
	$(CO) dulprivate.h
	$(CO) dulprotocol.c
	$(CO) dulstructures.h
	$(CO) dulsnoop.c
	$(CO) dulsnoop.h



libdulprotocol.a:	$(OBJS)
	ar cvr libdulprotocol.a $(OBJS); $(RANLIB) libdulprotocol.a

dulprotocol.o:	dulprotocol.c dulprotocol.h dicom.h  dulprivate.h condition.h \
dulstructures.h dulfsm.h
dulconstruct.o: dulconstruct.c dulprotocol.h dicom.h dulprivate.h condition.h \
dulstructures.h
dulparse.o:	dulparse.c dulprotocol.h dicom.h dulprivate.h condition.h \
dulstructures.h
dulcond.o:	dulcond.c dulprotocol.h dicom.h dulprivate.h \
dulstructures.h
dulfsm.o:	dulfsm.c dicom.h dulprotocol.h dulfsm.h \
dulstructures.h dulfsm.h
dulpresent.o:	dulpresent.c dicom.h dulprotocol.h dulfsm.h \
dulstructures.h dulfsm.h
dulsnoop.o: 	dulsnoop.c dulsnoop.h dicom.h

lint:
	flexelint $(LINTOPTIONS)  dulcond.c dulconstruct.c dulfsm.c \
dulparse.c dulpresent.c dulprotocol.c dulsnoop.c
