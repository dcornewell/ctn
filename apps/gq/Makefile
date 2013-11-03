#
#	Makefile for example database applications
#

TARGETS = gqinitq gqkillq

include $(DICOM_MAKE)

VPATH = ../../include:../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) -I../include
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_DATABASE)
LIBS = $(LIBS_CTN_NODB)

application:	$(TARGETS)

install:	$(TARGETS)
	$(INSTALL) gqinitq $(DICOM_BIN)
	$(INSTALL) gqkillq $(DICOM_BIN)
install-clean:	install
	make clean

clean:
	rm -f core *.o *.BAK *.bak *% $(TARGETS) core

checkin:
	$(INDENT) gqinitq.c;		$(CI) gqinitq.c
	$(INDENT) gqkillq.c;		$(CI) gqkillq.c
	rm -f core *.o *.BAK *.bak *% $(TARGETS) core

checkout:
	$(CO) gqinitq.c
	$(CO) gqkillq.c

indent:
	$(INDENT) gqinitq.c
	$(INDENT) gqkillq.c

gqinitq:		gqinitq.o
	$(CC) -o gqinitq gqinitq.o -g $(LFLAGS) $(LIBS)

gqkillq:	gqkillq.o
	$(CC) -o gqkillq gqkillq.o -g $(LFLAGS) $(LIBS)

lint:
	flexelint $(LINTOPTIONS) gqinitq.c
	flexelint $(LINTOPTIONS) gqkillq.c
