#	Makefile for idb_dump

include $(DICOM_MAKE)

VPATH = ../../include
CFLAGS = $(C_OPTS)
LFLAGS = $(LINK_FLAGS) -L$(DICOM_LIB) $(LIBPATH_DATABASE) $(LIBPATH_UCB)
LIBS = $(LIBS_CTN) $(LIBS_OS_SECURE_UCB) $(DEBUG_LIBS) $(LIBS_OS)

EXES = idb_dump

application:	$(EXES)

install: $(EXES)
	$(INSTALL) idb_dump $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -f $(EXES) *.o *.BAK *.bak *% *~ core

indent:


checkin:

checkout:

OBJS = idb_dump.o

idb_dump: $(OBJS)
	$(CC) -o idb_dump -g $(LFLAGS) $(OBJS) $(LIBS) -lm

lint:


test:
	./idb_dump LTA_IDB SERIES
