#	Makefile for DICOM facility (only defines an include file)

include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)

library:
install:
	$(INSTALLINCLUDE) print_queue.h $(DICOM_INCLUDE)
	$(INSTALLINCLUDE) iqueues.h $(DICOM_INCLUDE)
	$(INSTALLINCLUDE) UxXt.h $(DICOM_INCLUDE)

install-clean:	install

headerexport: install

clean:
	rm -f *.a *.o *.BAK *.bak *% core

indent:
	$(INDENT) print_queue.h
	$(INDENT) iqueues.h
	$(INDENT) UxXt.h
checkin:
	$(INDENT) print_queue.h;     $(CI) print_queue.h
	$(INDENT) iqueues.h;     $(CI) iqueues.h
	$(INDENT) UxXt.h;     $(CI) UxXt.h
	rm -f *.a *.o *.BAK *.bak *% core

checkout:
	$(CO) print_queue.h
	$(CO) iqueues.h
	$(CO) UxXt.h

lint:

