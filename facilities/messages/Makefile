#	Makefile for DICOM messages

OBJS = messages.o dump.o msgcond.o ref_item.o
include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)

library:	libmessages.a
install:	libmessages.a
	cp libmessages.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libmessages.a
	$(INSTALLINCLUDE) dicom_messages.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:
	$(INSTALLINCLUDE) dicom_messages.h $(DICOM_INCLUDE)

links:
	rm -f $(DICOM_LIBSRC)/dicom_messages.h $(DICOM_LIBSRC)/dump.c
	rm -f $(DICOM_LIBSRC)/msgcond.c $(DICOM_LIBSRC)/messages.c
	rm -f $(DICOM_LIBSRC)/msgprivate.h $(DICOM_LIBSRC)/ref_item.c
	ln -s ../facilities/messages/dicom_messages.h $(DICOM_LIBSRC)
	ln -s ../facilities/messages/dump.c $(DICOM_LIBSRC)
	ln -s ../facilities/messages/msgcond.c $(DICOM_LIBSRC)
	ln -s ../facilities/messages/messages.c $(DICOM_LIBSRC)
	ln -s ../facilities/messages/msgprivate.h $(DICOM_LIBSRC)
	ln -s ../facilities/messages/ref_item.c $(DICOM_LIBSRC)


clean:
	rm -f *.a *.o *.BAK *% core

indent:
#	$(INDENT) dicom_messages.h
	$(INDENT) dump.c
	$(INDENT) msgcond.c
	$(INDENT) messages.c
#	$(INDENT) msgprivate.h
	$(INDENT) ref_item.c
#	$(INDENT) cbuild.c	RETIRED
#	$(INDENT) msgsupport.c	RETIRED
#	$(INDENT) cparse.c	RETIRED
#	$(INDENT) nbuild.c	RETIRED
#	$(INDENT) nparse.c	RETIRED

checkin:
				$(CI) dicom_messages.h
	$(INDENT) dump.c;	$(CI) dump.c
	$(INDENT) msgcond.c;	$(CI) msgcond.c
	$(INDENT) messages.c;	$(CI) messages.c
				$(CI) msgprivate.h
	$(INDENT) ref_item.c; $(CI) ref_item.c
#	$(INDENT) cbuild.c;    $(CI) cbuild.c
#	$(INDENT) cparse.c;    $(CI) cparse.c
#	$(INDENT) msgsupport.c; $(CI) msgsupport.c
#	$(INDENT) nbuild.c;   $(CI) nbuild.c
#	$(INDENT) nparse.c;   $(CI) nparse.c
	rm -f *.a *.o *.BAK *% core

checkout:
	$(CO) dicom_messages.h
	$(CO) dump.c
	$(CO) messages.c
	$(CO) msgprivate.h
	$(CO) msgcond.c
	$(CO) ref_item.c
#	$(CO) cbuild.c
#	$(CO) cparse.c
#	$(CO) msgsupport.c
#	$(CO) nbuild.c
#	$(CO) nparse.c

libmessages.a:	$(OBJS)
	ar cvr libmessages.a $(OBJS); $(RANLIB) libmessages.a

dump.o:		dump.c dicom.h condition.h dicom_messages.h dicom_objects.h
messages.o:	messages.c dicom.h condition.h dicom_messages.h dicom_objects.h
msgcond.o:	msgcond.c dicom.h condition.h dicom_messages.h dicom_objects.h
ref_item.o:	ref_item.c dicom.h condition.h dicom_messages.h dicom_objects.h
#cbuild.o:	cbuild.c dicom.h condition.h dicom_messages.h dicom_objects.h
#cdump.o:	cdump.c dicom.h condition.h dicom_messages.h dicom_objects.h
#msgsupport.o:	msgsupport.c dicom.h condition.h dicom_messages.h dicom_objects.h msgprivate.h
#nbuild.o:	nbuild.c dicom.h condition.h dicom_messages.h dicom_objects.h
#nparse.o:	nparse.c dicom.h condition.h dicom_messages.h dicom_objects.h

lint:
	flexelint $(LINTOPTIONS) dump.c messages.c msgcond.c ref_item.c
