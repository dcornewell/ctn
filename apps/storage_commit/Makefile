#	Makefile for storage_commit

OBJ = storage_commit.o association.o commit_request.o sscond.o
include $(DICOM_MAKE)

VPATH = ../../include
CFLAGS = $(C_OPTS)
LIBS = $(LIBS_CTN) $(LIBS_OS)
LFLAGS = -L$(DICOM_LIB)
EXES = storage_commit commit_agent

application:	$(EXES)

install: $(EXES)
	$(INSTALL) storage_commit $(DICOM_BIN)
	$(INSTALL) commit_agent $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -f $(EXES) *.o *.BAK core *% *~

indent:
	$(INDENT) association.c
	$(INDENT) commit_agent.c
	$(INDENT) commit_request.c
	$(INDENT) nevent.c
	$(INDENT) nevent.h
	$(INDENT) requests.c
	$(INDENT) sscond.c
	$(INDENT) storage_commit.c
	$(INDENT) storage_commit.h

checkin:
	$(INDENT) association.c;	$(CI) association.c
	$(INDENT) commit_agent.c;	$(CI) commit_agent.c
	$(INDENT) commit_request.c;	$(CI) commit_request.c
	$(INDENT) nevent.c;		$(CI) nevent.c
	$(INDENT) nevent.h;		$(CI) nevent.h
	$(INDENT) requests.c;		$(CI) requests.c
	$(INDENT) sscond.c;		$(CI) sscond.c
	$(INDENT) storage_commit.c;	$(CI) storage_commit.c
	$(INDENT) storage_commit.h;	$(CI) storage_commit.h
	rm -f $(EXES) *.o *.BAK core *% *~

checkout:
	$(CO) association.c
	$(CO) commit_agent.c
	$(CO) commit_request.c
	$(CO) nevent.c
	$(CO) nevent.h
	$(CO) requests.c
	$(CO) sscond.c
	$(CO) storage_commit.c
	$(CO) storage_commit.h

storage_commit: $(OBJ)
	$(CC) -o storage_commit -g $(LFLAGS) $(OBJ) $(LIBS)
commit_agent: commit_agent.o association.o requests.o nevent.o sscond.o
	$(CC) -o commit_agent -g $(LFLAGS) commit_agent.o association.o requests.o nevent.o sscond.o $(LIBS)

storage_commit.o: storage_commit.c tbl.h
commit_agent.o: commit_agent.c tbl.h
association.o: association.c
nevent.o: nevent.c
sscond.o: sscond.c

lint:
	flexelint $(LINTOPTIONS) storage_commit.c
