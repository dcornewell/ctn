#
#	Makefile for tbl tests...
#
NAME = ttunique ttinsert ttdelete ttselect ttlayout ttupdate
include $(DICOM_MAKE)

# smm had to change VPATH macro to get this to make under HPUX
# Kind of weird that make could not find tbl.h
#VPATH = $(DICOM_INCLUDE)
VPATH = ../../include

CFLAGS = $(C_OPTS)
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_DATABASE) $(LIBPATH_UCB) $(LIBS_OS)
LIBS = $(LIBS_CTN)

application: ttunique ttinsert ttdelete ttselect ttlayout ttupdate

install: ttinsert ttunique ttdelete ttselect ttlayout ttupdate
	$(INSTALL) ttinsert $(DICOM_BIN)
	$(INSTALL) ttunique $(DICOM_BIN)
	$(INSTALL) ttdelete $(DICOM_BIN)
	$(INSTALL) ttselect $(DICOM_BIN)
	$(INSTALL) ttlayout $(DICOM_BIN)
	$(INSTALL) ttupdate $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -rf $(NAME) *.o *.BAK *.bak *% core debug

indent:
	$(INDENT) ttunique.c
	$(INDENT) ttinsert.c
	$(INDENT) ttdelete.c
	$(INDENT) ttselect.c
	$(INDENT) ttlayout.c
	$(INDENT) ttupdate.c

checkin:
	$(INDENT) ttunique.c; $(CI) ttunique.c
	$(INDENT) ttinsert.c; $(CI) ttinsert.c
	$(INDENT) ttdelete.c; $(CI) ttdelete.c
	$(INDENT) ttselect.c; $(CI) ttselect.c
	$(INDENT) ttlayout.c; $(CI) ttlayout.c
	$(INDENT) ttupdate.c; $(CI) ttupdate.c
	rm -f $(NAME) *.o *.BAK *.bak *% core

checkout:
	$(CO) ttinsert.c
	$(CO) ttunique.c
	$(CO) ttdelete.c
	$(CO) ttselect.c
	$(CO) ttlayout.c
	$(CO) ttupdate.c

ttunique: ttunique.o
	$(CC) -o ttunique -g $(LFLAGS) ttunique.o $(LIBS)

ttinsert: ttinsert.o
	$(CC) -o ttinsert -g $(LFLAGS) ttinsert.o $(LIBS)

ttdelete: ttdelete.o
	$(CC) -o ttdelete -g $(LFLAGS) ttdelete.o $(LIBS)

ttselect: ttselect.o
	$(CC) -o ttselect -g $(LFLAGS) ttselect.o $(LIBS)

ttlayout: ttlayout.o
	$(CC) -o ttlayout -g $(LFLAGS) ttlayout.o $(LIBS)

ttupdate: ttupdate.o
	$(CC) -o ttupdate -g $(LFLAGS) ttupdate.o $(LIBS)

ttuinque.o: ttuinque.c tbl.h

ttinsert.o: ttinsert.c tbl.h

ttdelete.o: ttdelete.c tbl.h

ttselect.o: ttselect.c tbl.h

ttlayout.o: ttlayout.c tbl.h

ttupdate.o: ttupdate.c tbl.h

lint:
	flexelint $(LINTOPTIONS) ttunique.c
	flexelint $(LINTOPTIONS) ttinsert.c
	flexelint $(LINTOPTIONS) ttdelete.c
	flexelint $(LINTOPTIONS) ttselect.c
	flexelint $(LINTOPTIONS) ttlayout.c
	flexelint $(LINTOPTIONS) ttupdate.c
