#	Makefile for image_server

NAME = image_server
#OBJ = $(NAME).o association.o copy.o requests.o find.o move.o cget.o 
OBJ = $(NAME).o association.o requests.o find.o move.o cget.o \
parse.o sscond.o
include $(DICOM_MAKE)

VPATH = ../../include
CFLAGS = $(C_OPTS)
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_DATABASE) $(LIBPATH_UCB)
LIBS = $(LIBS_CTN) $(LIBS_OS_SECURE_UCB) $(DEBUG_LIBS) $(LIBS_OS)

application:	$(NAME) fillImageDB

install: $(NAME) fillImageDB
	$(INSTALLUID) $(NAME) $(DICOM_BIN)
	$(INSTALL) fillImageDB $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -f $(NAME) *.o *.BAK *.bak *% core test_storage fillImageDB

indent:
	$(INDENT) $(NAME).c
	$(INDENT) association.c
	$(INDENT) cget.c
	$(INDENT) cget.h
	$(INDENT) copy.c
	$(INDENT) fillImageDB.c
	$(INDENT) find.c
	$(INDENT) move.c
	$(INDENT) parse.c
	$(INDENT) requests.c
	$(INDENT) sscond.c
	$(INDENT) find.h
	$(INDENT) copy.h
	$(INDENT) image_server.h
	$(INDENT) move.h
	$(INDENT) test_storage.c

checkin:
	$(INDENT) $(NAME).c; $(CI) $(NAME).c
	$(INDENT) association.c;	$(CI) association.c
	$(INDENT) cget.c;		$(CI) cget.c
	$(INDENT) cget.h;		$(CI) cget.h
	$(INDENT) copy.c;		$(CI) copy.c
	$(INDENT) fillImageDB.c;	$(CI) fillImageDB.c
	$(INDENT) find.c;		$(CI) find.c
	$(INDENT) move.c;		$(CI) move.c
	$(INDENT) parse.c;		$(CI) parse.c
	$(INDENT) requests.c;		$(CI) requests.c
	$(INDENT) sscond.c;		$(CI) sscond.c
	$(INDENT) image_server.h;	$(CI) image_server.h
	$(INDENT) find.h;		$(CI) find.h
	$(INDENT) copy.h;		$(CI) copy.h
	$(INDENT) move.h;		$(CI) move.h
	$(INDENT) test_storage.c;	$(CI) test_storage.c
	rm -f $(NAME) *.o *.BAK *.bak *% core test_storage fillImageDB

checkout:
	$(CO) $(NAME).c
	$(CO) association.c
	$(CO) cget.c
	$(CO) cget.h
	$(CO) copy.c
	$(CO) fillImageDB.c
	$(CO) find.c
	$(CO) move.c
	$(CO) parse.c
	$(CO) requests.c
	$(CO) sscond.c
	$(CO) image_server.h
	$(CO) find.h
	$(CO) copy.h
	$(CO) move.h
	$(CO) test_storage.c

$(NAME): $(OBJ)
	$(CC) -o $(NAME) -g $(LFLAGS) $(OBJ) $(LIBS) -lm
test_storage: test_storage.o parse.o
	$(CC) -o test_storage -g $(LFLAGS) test_storage.o parse.o $(LIBS) -lm
fillImageDB: fillImageDB.o parse.o sscond.o
	$(CC) -o fillImageDB -g $(LFLAGS) fillImageDB.o parse.o sscond.o $(LIBS) -lm

$(NAME).o: $(NAME).c tbl.h image_server.h
association.o: association.c image_server.h
cget.o: cget.c cget.h
copy.o: copy.c image_server.h copy.h
find.o: find.c image_server.h find.h
move.o: move.c image_server.h move.h
parse.o: parse.c image_server.h
requests.o: requests.c image_server.h
sscond.o: sscond.c image_server.h

lint:
	flexelint $(LINTOPTIONS) $(NAME).c association.c copy.c find.c move.c parse.c requests.c sscond.c
