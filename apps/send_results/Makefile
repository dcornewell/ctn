#	Makefile for send_results

OBJ = send_results.o association.o
include $(DICOM_MAKE)

VPATH = ../../include
CFLAGS = $(C_OPTS)
LIBS = $(LIBS_CTN) $(LIBS_OS)
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_UCB)

application:	send_results

install: send_results
	$(INSTALL) send_results $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -f send_results *.o *.BAK *% *~ core

indent:
	$(INDENT) association.c
	$(INDENT) send_results.c
	$(INDENT) send_results.h

checkin:
	$(INDENT) association.c;	$(CI) association.c
	$(INDENT) send_results.c;	$(CI) send_results.c
	$(INDENT) send_results.h;	$(CI) send_results.h
	rm -f send_results *.o *.BAK core *% *~

checkout:
	$(CO) association.c
	$(CO) send_results.c
	$(CO) send_results.h

send_results: $(OBJ)
	$(CC) -o send_results -g $(LFLAGS) $(OBJ) $(LIBS)

lint:
	flexelint $(LINTOPTIONS) send_results.c
