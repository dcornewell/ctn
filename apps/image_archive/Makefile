#	Makefile for image_archive

include $(DICOM_MAKE)

VPATH = ../../include
CFLAGS = $(C_OPTS)
LFLAGS = $(LINK_FLAGS) -L$(DICOM_LIB) $(LIBPATH_DATABASE) $(LIBPATH_UCB)
LIBS = $(LIBS_CTN) $(LIBS_OS_SECURE_UCB) $(DEBUG_LIBS) $(LIBS_OS)

EXES = archive_server archive_agent archive_cleaner dump_commit_requests
EXESMB = archive_server.mb archive_agent archive_cleaner dump_commit_requests

application:	$(EXES)
application.mb:	$(EXESM)

install: $(EXES)
	$(INSTALLUID) archive_server $(DICOM_BIN)
	$(INSTALL) archive_agent $(DICOM_BIN)
	$(INSTALL) archive_cleaner $(DICOM_BIN)
	$(INSTALL) dump_commit_requests $(DICOM_BIN)

install-clean:	install
	make clean

install.mb: $(EXESMB)
	mv archive_server.mb archive_server
	$(INSTALLUID) archive_server $(DICOM_BIN)
	$(INSTALL) archive_agent $(DICOM_BIN)
	$(INSTALL) archive_cleaner $(DICOM_BIN)
	$(INSTALL) dump_commit_requests $(DICOM_BIN)

clean:
	rm -f $(EXES) *.o *.BAK *.bak *% *~ core

indent:
	$(INDENT) archive_agent.c
	$(INDENT) archive_cleaner.c
	$(INDENT) archive_server.c
	$(INDENT) association.c
	$(INDENT) cget.c
	$(INDENT) cget.h
	$(INDENT) commit_result.c
	$(INDENT) commit_result.h
	$(INDENT) copy.c
	$(INDENT) copy.h
	$(INDENT) dump_commit_requests.c
	$(INDENT) find.c
	$(INDENT) find.h
	$(INDENT) image_archive.h
	$(INDENT) move.c
	$(INDENT) move.h
	$(INDENT) naction.c
	$(INDENT) naction.h
	$(INDENT) parse.c
	$(INDENT) requests.c
	$(INDENT) sscond.c


checkin:
	$(INDENT) archive_agent.c;	$(CI) archive_agent.c
	$(INDENT) archive_cleaner.c;	$(CI) archive_cleaner.c
	$(INDENT) archive_server.c;	$(CI) archive_server.c
	$(INDENT) association.c;	$(CI) association.c
	$(INDENT) cget.c;		$(CI) cget.c
	$(INDENT) cget.h;		$(CI) cget.h
	$(INDENT) commit_result.c;	$(CI) commit_result.c
	$(INDENT) commit_result.h;	$(CI) commit_result.h
	$(INDENT) copy.c;		$(CI) copy.c
	$(INDENT) copy.h;		$(CI) copy.h
	$(INDENT) dump_commit_requests.c; $(CI) dump_commit_requests.c
	$(INDENT) find.c;		$(CI) find.c
	$(INDENT) find.h;		$(CI) find.h
	$(INDENT) image_archive.h;	$(CI) image_archive.h
	$(INDENT) move.c;		$(CI) move.c
	$(INDENT) move.h;		$(CI) move.h
	$(INDENT) naction.c;		$(CI) naction.c
	$(INDENT) naction.h;		$(CI) naction.h
	$(INDENT) parse.c;		$(CI) parse.c
	$(INDENT) requests.c;		$(CI) requests.c
	$(INDENT) sscond.c;		$(CI) sscond.c
	rm -f $(EXES) *.o *.BAK *.bak *% core *~

checkout:
	$(CO) archive_agent.c
	$(CO) archive_cleaner.c
	$(CO) archive_server.c
	$(CO) association.c
	$(CO) cget.c
	$(CO) cget.h
	$(CO) commit_result.c
	$(CO) commit_result.h
	$(CO) copy.c
	$(CO) copy.h
	$(CO) dump_commit_requests.c
	$(CO) find.c
	$(CO) find.h
	$(CO) image_archive.h
	$(CO) move.c
	$(CO) move.h
	$(CO) naction.c
	$(CO) naction.h
	$(CO) parse.c
	$(CO) requests.c
	$(CO) sscond.c

#SERVER_OBJS = archive_server.o association.o copy.o requests.o find.o move.o
SERVER_OBJS = archive_server.o association.o requests.o find.o move.o \
cget.o naction.o queue.o parse.o sscond.o

SERVER_OBJS_MB = archive_server.o association.o requests.o find.o move.o \
cget.o naction.o queue.o parseMB.o sscond.o

archive_server: $(SERVER_OBJS)
	$(CC) -o archive_server -g $(LFLAGS) $(SERVER_OBJS) $(LIBS) -lm

relink:
	$(CC) -o archive_server -g $(LFLAGS) $(SERVER_OBJS) $(LIBS) -lm

archive_server.mb: $(SERVER_OBJS_MB)
	$(CC) -o archive_server.mb -g $(LFLAGS) $(SERVER_OBJS_MB) $(LIBS) -lm

archive_agent: archive_agent.o commit_result.o sscond.o
	$(CC) -o archive_agent -g $(LFLAGS) archive_agent.o commit_result.o sscond.o $(LIBS)

archive_cleaner: archive_cleaner.o commit_result.o sscond.o
	$(CC) -o archive_cleaner -g $(LFLAGS) archive_cleaner.o commit_result.o sscond.o $(LIBS)

dump_commit_requests: dump_commit_requests.o
	$(CC) -o dump_commit_requests -g $(LFLAGS) dump_commit_requests.o $(LIBS)

DUMMY_OBJS = archive_server.o association_dummy.o copy.o requests_dummy.o find.o move.o \
cget.o naction.o parse.o sscond.o

archive_dummy: $(DUMMY_OBJS)
	$(CC) -o archive_dummy -g $(LFLAGS) $(DUMMY_OBJS) $(LIBS) -lm

IMPORT_OBJS = importMB.o parseMB.o sscond.o

importMB: $(IMPORT_OBJS)
	$(CC) -o importMB -g $(LFLAGS) $(IMPORT_OBJS) $(LIBS) -lm


archive_server.o: archive_server.c tbl.h image_archive.h
association.o: association.c image_archive.h
cget.o: cget.c cget.h
copy.o: copy.c image_archive.h copy.h
find.o: find.c image_archive.h find.h
move.o: move.c image_archive.h move.h
naction.o: naction.c image_archive.h naction.h
parse.o: parse.c image_archive.h
parseMB.o: parseMB.c image_archive.h
requests.o: requests.c image_archive.h
sscond.o: sscond.c image_archive.h

archive_agent.o: archive_agent.c commit_result.h
commit_result.o: commit_result.c commit_result.h

lint:
	flexelint $(LINTOPTIONS) archive_server.c association.c copy.c find.c move.c parse.c requests.c sscond.c

server:
	./archive_server -g DINPACS -r -i -q 2100

drno:	archive_server
	./archive_server -C "" -n drno -r -i 2100

drno_silent:	archive_server
	./archive_server -C "" -s -n drno -r -i -q 2100
