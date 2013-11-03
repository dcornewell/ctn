#	Makefile for DICOM services

OBJS = srv1.o srv2.o cmd_valid.o send.o verify.o storage.o find.o private.o srvcond.o move.o get.o \
nget.o neventreport.o ncreate.o nset.o naction.o ndelete.o
include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)

library:	libservices.a
install:	libservices.a
	cp libservices.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libservices.a
	$(INSTALLINCLUDE) dicom_services.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:
	$(INSTALLINCLUDE) dicom_services.h $(DICOM_INCLUDE)

links:
	rm -f $(DICOM_LIBSRC)/dicom_services.h $(DICOM_LIBSRC)/find.c
	rm -f $(DICOM_LIBSRC)/move.c $(DICOM_LIBSRC)/neventreport.c
	rm -f $(DICOM_LIBSRC)/nget.c $(DICOM_LIBSRC)/nset.c
	rm -f $(DICOM_LIBSRC)/ndelete.c $(DICOM_LIBSRC)/ncreate.c
	rm -f $(DICOM_LIBSRC)/naction.c $(DICOM_LIBSRC)/private.c
	rm -f $(DICOM_LIBSRC)/private.h $(DICOM_LIBSRC)/services.c
	rm -f $(DICOM_LIBSRC)/srvcond.c $(DICOM_LIBSRC)/storage.c
	rm -f $(DICOM_LIBSRC)/verify.c $(DICOM_LIBSRC)/get.c
	rm -f $(DICOM_LIBSRC)/cmd_valid.c $(DICOM_LIBSRC)/send.c
	rm -f $(DICOM_LIBSRC)/srv1.c $(DICOM_LIBSRC)/srv2.c
	ln -s ../facilities/services/dicom_services.h $(DICOM_LIBSRC)
	ln -s ../facilities/services/find.c $(DICOM_LIBSRC)
	ln -s ../facilities/services/move.c $(DICOM_LIBSRC)
	ln -s ../facilities/services/neventreport.c $(DICOM_LIBSRC)
	ln -s ../facilities/services/nget.c $(DICOM_LIBSRC)
	ln -s ../facilities/services/nset.c $(DICOM_LIBSRC)
	ln -s ../facilities/services/ndelete.c $(DICOM_LIBSRC)
	ln -s ../facilities/services/ncreate.c $(DICOM_LIBSRC)
	ln -s ../facilities/services/naction.c $(DICOM_LIBSRC)
	ln -s ../facilities/services/private.c $(DICOM_LIBSRC)
	ln -s ../facilities/services/private.h $(DICOM_LIBSRC)
	ln -s ../facilities/services/srvcond.c $(DICOM_LIBSRC)
	ln -s ../facilities/services/storage.c $(DICOM_LIBSRC)
	ln -s ../facilities/services/verify.c $(DICOM_LIBSRC)
	ln -s ../facilities/services/get.c $(DICOM_LIBSRC)
	ln -s ../facilities/services/cmd_valid.c $(DICOM_LIBSRC)
	ln -s ../facilities/services/send.c $(DICOM_LIBSRC)
	ln -s ../facilities/services/srv1.c $(DICOM_LIBSRC)
	ln -s ../facilities/services/srv2.c $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *BAK *% core

indent:
#	$(INDENT) dicom_services.h
	$(INDENT) find.c
	$(INDENT) move.c
	$(INDENT) neventreport.c
	$(INDENT) nget.c
	$(INDENT) nset.c
	$(INDENT) ndelete.c
	$(INDENT) ncreate.c
	$(INDENT) naction.c
	$(INDENT) private.c
#	$(INDENT) private.h
#	$(INDENT) services.c
	$(INDENT) srvcond.c
	$(INDENT) storage.c
	$(INDENT) verify.c
	$(INDENT) get.c
	$(INDENT) cmd_valid.c
	$(INDENT) send.c
	$(INDENT) srv1.c
	$(INDENT) srv2.c
checkin:
				$(CI) dicom_services.h
	$(INDENT) find.c;	$(CI) find.c
	$(INDENT) move.c;	$(CI) move.c
	$(INDENT) neventreport.c;     $(CI) neventreport.c
	$(INDENT) nget.c;	$(CI) nget.c
	$(INDENT) nset.c;	$(CI) nset.c
	$(INDENT) ndelete.c;	$(CI) ndelete.c
	$(INDENT) ncreate.c;	$(CI) ncreate.c
	$(INDENT) naction.c;	$(CI) naction.c
	$(INDENT) private.c;	$(CI) private.c
				$(CI) private.h
#	$(INDENT) services.c;	$(CI) services.c
	$(INDENT) srvcond.c;	$(CI) srvcond.c
	$(INDENT) storage.c;	$(CI) storage.c
	$(INDENT) verify.c;	$(CI) verify.c
	$(INDENT) get.c;	$(CI) get.c
	$(INDENT) cmd_valid.c;	$(CI) cmd_valid.c
	$(INDENT) send.c;	$(CI) send.c
	$(INDENT) srv1.c;	$(CI) srv1.c
	$(INDENT) srv2.c;	$(CI) srv2.c
	rm -f *.a *.o *BAK *% core

checkout:
	$(CO) dicom_services.h
	$(CO) find.c
	$(CO) move.c
	$(CO) neventreport.c
	$(CO) nget.c
	$(CO) nset.c
	$(CO) ndelete.c
	$(CO) ncreate.c
	$(CO) naction.c
	$(CO) private.c
	$(CO) private.h
#	$(CO) services.c
	$(CO) srvcond.c
	$(CO) storage.c
	$(CO) verify.c
	$(CO) get.c
	$(CO) cmd_valid.c
	$(CO) send.c
	$(CO) srv1.c
	$(CO) srv2.c

libservices.a:	$(OBJS)
	ar cvr libservices.a $(OBJS); $(RANLIB) libservices.a

find.o:		find.c dicom.h condition.h dicom_services.h \
private.h dicom_messages.h dicom_uids.h dicom_objects.h
move.o:		move.c dicom.h condition.h dicom_services.h \
private.h dicom_messages.h dicom_uids.h dicom_objects.h
private.o:	private.c dicom.h condition.h dicom_services.h \
private.h dicom_messages.h dicom_uids.h dicom_objects.h
services.o:	services.c dicom.h condition.h dicom_services.h \
private.h dicom_messages.h dicom_uids.h dicom_objects.h
srvcond.o:	srvcond.c dicom.h condition.h dicom_services.h \
private.h dicom_messages.h dicom_uids.h dicom_objects.h
storage.o:	storage.c dicom.h condition.h dicom_services.h \
private.h dicom_messages.h dicom_uids.h dicom_objects.h
verify.o:	verify.c dicom.h condition.h dicom_services.h \
private.h dicom_messages.h dicom_uids.h dicom_objects.h
naction.o:	naction.c dicom.h condition.h dicom_services.h \
private.h dicom_messages.h dicom_uids.h dicom_objects.h
ncreate.o:	ncreate.c dicom.h condition.h dicom_services.h \
private.h dicom_messages.h dicom_uids.h dicom_objects.h
ndelete.o:	ndelete.c dicom.h condition.h dicom_services.h \
private.h dicom_messages.h dicom_uids.h dicom_objects.h
neventreport.o:	neventreport.c dicom.h condition.h dicom_services.h \
private.h dicom_messages.h dicom_uids.h dicom_objects.h
nget.o:	nget.c dicom.h condition.h dicom_services.h \
private.h dicom_messages.h dicom_uids.h dicom_objects.h
nset.o:	nset.c dicom.h condition.h dicom_services.h \
private.h dicom_messages.h dicom_uids.h dicom_objects.h

lint:
	flexelint $(LINTOPTIONS) srv1.c srv2.c find.c move.c private.c srvcond.c \
storage.c verify.c get.c naction.c ncreate.c ndelete.c neventreport.c \
nget.c nset.c
