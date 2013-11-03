#	Makefile for Manage facility

OBJS = control.o delete.o insert.o select.o set.o mancond.o
include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) -DDEBUG

library:	libmanage.a

install:	libmanage.a
	cp libmanage.a $(DICOM_LIB); $(RANLIB) $(DICOM_LIB)/libmanage.a
	$(INSTALLINCLUDE) manage.h $(DICOM_INCLUDE)

export:
	cp *.[ch] $(DICOM_LIBSRC)

headerexport:
	$(INSTALLINCLUDE) manage.h $(DICOM_INCLUDE)

links:
	rm -f $(DICOM_LIBSRC)/manage.h $(DICOM_LIBSRC)/control.c
	rm -f $(DICOM_LIBSRC)/delete.c $(DICOM_LIBSRC)/insert.c
	rm -f $(DICOM_LIBSRC)/select.c $(DICOM_LIBSRC)/set.c
	rm -f $(DICOM_LIBSRC)/mancond.c $(DICOM_LIBSRC)/dmanprivate.h
	ln -s ../facilities/manage/manage.h $(DICOM_LIBSRC)
	ln -s ../facilities/manage/control.c $(DICOM_LIBSRC)
	ln -s ../facilities/manage/delete.c $(DICOM_LIBSRC)
	ln -s ../facilities/manage/insert.c $(DICOM_LIBSRC)
	ln -s ../facilities/manage/select.c $(DICOM_LIBSRC)
	ln -s ../facilities/manage/set.c $(DICOM_LIBSRC)
	ln -s ../facilities/manage/mancond.c $(DICOM_LIBSRC)
	ln -s ../facilities/manage/dmanprivate.h $(DICOM_LIBSRC)

clean:
	rm -f *.a *.o *.BAK *% core

indent:
#	$(INDENT) manage.h
	$(INDENT) control.c
	$(INDENT) delete.c
	$(INDENT) insert.c
	$(INDENT) select.c
	$(INDENT) set.c
	$(INDENT) mancond.c
#	$(INDENT) dmanprivate.h

checkin:
				$(CI) manage.h
	$(INDENT) control.c;	$(CI) control.c
	$(INDENT) delete.c;	$(CI) delete.c
	$(INDENT) insert.c;	$(CI) insert.c
	$(INDENT) select.c;	$(CI) select.c
	$(INDENT) set.c;	$(CI) set.c
	$(INDENT) mancond.c;	$(CI) mancond.c
				$(CI) dmanprivate.h
	rm -f *.a *.o *.BAK *% core

checkout:
	$(CO) manage.h
	$(CO) control.c
	$(CO) delete.c
	$(CO) insert.c
	$(CO) select.c
	$(CO) set.c
	$(CO) mancond.c
	$(CO) dmanprivate.h

libmanage.a:	$(OBJS)
	ar cvr libmanage.a $(OBJS); $(RANLIB) libmanage.a

control.o:	manage.h control.c tbl.h
delete.o:	manage.h delete.c tbl.h
insert.o:	manage.h insert.c tbl.h
select.o:	manage.h select.c tbl.h
set.o:		manage.h set.c tbl.h
mancond.o:	manage.h mancond.c

lint:
	flexelint $(LINTOPTIONS) control.c delete.c insert.c select.c set.c mancond.c
