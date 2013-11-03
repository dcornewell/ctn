#	Makefile for send_study

OBJ = send_study.o association.o
include $(DICOM_MAKE)

VPATH = ../../include
CFLAGS = $(C_OPTS)
LIBS = $(LIBS_CTN) $(LIBS_OS)
LFLAGS = -L$(DICOM_LIB)

application:	send_study

install: send_study
	$(INSTALL) send_study $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -f send_study *.o *.BAK *% *~ core

indent:
	$(INDENT) association.c
	$(INDENT) send_study.c
	$(INDENT) send_study.h

checkin:
	$(INDENT) association.c;	$(CI) association.c
	$(INDENT) send_study.c;		$(CI) send_study.c
	$(INDENT) send_study.h;		$(CI) send_study.h
	rm -f send_study *.o *.BAK core *% *~

checkout:
	$(CO) association.c
	$(CO) send_study.c
	$(CO) send_study.h

send_study: $(OBJ)
	$(CC) -o send_study -g $(LFLAGS) $(OBJ) $(LIBS)

lint:
	flexelint $(LINTOPTIONS) send_study.c
