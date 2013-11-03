#
#	Makefile for ctnnetwork
#

TARGETS = ctnnetwork kill_ctnnetwork pq_ctnnetwork enq_ctnnetwork

OBJ = ctnnetwork.o ctnnetwork_x_support.o

include $(DICOM_MAKE)
VPATH = ../../include
CFLAGS = $(C_OPTS)
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_DATABASE) $(LIBPATH_X11) $(LIBPATH_UCB)
LIBS = $(LIBS_CTN)

application:	$(TARGETS)

install: $(TARGETS)
	$(INSTALL) ctnnetwork $(DICOM_BIN)
	$(INSTALL) kill_ctnnetwork $(DICOM_BIN)
	$(INSTALL) pq_ctnnetwork $(DICOM_BIN)
	$(INSTALL) enq_ctnnetwork $(DICOM_BIN)
install-clean:	install
	make clean

clean:
	rm -f *.o *.BAK core $(TARGETS)

checkin:
	$(INDENT) ctnnetwork.c;		$(CI) ctnnetwork.c
	$(INDENT) ctnnetwork.h;		$(CI) ctnnetwork.h
	$(INDENT) ctnnetwork_x_support.c;$(CI) ctnnetwork_x_support.c
	$(INDENT) kill_ctnnetwork.c;	$(CI) kill_ctnnetwork.c
	$(INDENT) pq_ctnnetwork.c;	$(CI) pq_ctnnetwork.c
	$(INDENT) enq_ctnnetwork.c;	$(CI) enq_ctnnetwork.c
	rm -f *.o *.BAK core $(TARGETS)

indent:
	$(INDENT) ctnnetwork.c;
	$(INDENT) ctnnetwork.h;
	$(INDENT) ctnnetwork_x_support.c;
	$(INDENT) kill_ctnnetwork.c;
	$(INDENT) pq_ctnnetwork.c;
	$(INDENT) enq_ctnnetwork.c;

checkout:
	$(CO) ctnnetwork.c
	$(CO) ctnnetwork.h
	$(CO) ctnnetwork_x_support.c
	$(CO) kill_ctnnetwork.c
	$(CO) pq_ctnnetwork.c
	$(CO) enq_ctnnetwork.c

ctnnetwork: $(OBJ)
	$(XCC) -o ctnnetwork -g $(LFLAGS) $(OBJ) $(LIBS) $(LIBS_OS) $(LIBS_X11) -lm

pq_ctnnetwork: pq_ctnnetwork.o
	$(CC) -o pq_ctnnetwork pq_ctnnetwork.o -g $(LFLAGS) $(LIBS) -lm

kill_ctnnetwork: kill_ctnnetwork.o
	$(CC) -o kill_ctnnetwork kill_ctnnetwork.o -g $(LFLAGS) $(LIBS) -lm

enq_ctnnetwork: enq_ctnnetwork.o
	$(CC) -o enq_ctnnetwork enq_ctnnetwork.o -g $(LFLAGS) $(LIBS) -lm

ctnnetwork.o: ctnnetwork.c ctnnetwork.h dicom.h dicom.h
	$(XCC) -c $(CFLAGS) ctnnetwork.c

ctnnetwork_x_support.o: ctnnetwork_x_support.c ctnnetwork.h dicom.h
	$(XCC) -c $(CFLAGS) ctnnetwork_x_support.c

pq_ctnnetwork.o: pq_ctnnetwork.c iqueues.h ctnnetwork.h
	$(CC) -c $(CFLAGS) pq_ctnnetwork.c

kill_ctnnetwork.o: kill_ctnnetwork.c iqueues.h ctnnetwork.h
	$(CC) -c $(CFLAGS) kill_ctnnetwork.c

enq_ctnnetwork.o: enq_ctnnetwork.c iqueues.h ctnnetwork.h
	$(CC) -c $(CFLAGS) enq_ctnnetwork.c

lint:
	flexelint $(LINTOPTIONS) ctnnetwork.c ctnnetwork_x_support.c
	flexelint $(LINTOPTIONS) enq_ctnnetwork.c
	flexelint $(LINTOPTIONS) kill_ctnnetwork.c
	flexelint $(LINTOPTIONS) pq_ctnnetwork.c
