#	Makefile for fis_server

NAME = fis_server
OBJ = $(NAME).o association.o requests.o nget.o nset.o ncreate.o sscond.o
include $(DICOM_MAKE)

VPATH = ../../include
CFLAGS = $(C_OPTS)
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_UCB)
LIBS = $(LIBS_CTN) $(LIBS_OS_SECURE_UCB) $(LIBS_OS)

application:	$(NAME)

install: $(NAME)
	$(INSTALL) $(NAME) $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -f $(NAME) *.o *.BAK *.bak *% core

indent:
	$(INDENT) $(NAME).c
	$(INDENT) association.c
	$(INDENT) ncreate.c
	$(INDENT) nget.c
	$(INDENT) nset.c
	$(INDENT) requests.c
	$(INDENT) sscond.c
	$(INDENT) fis_server.h
	$(INDENT) services.h

checkin:
	$(INDENT) $(NAME).c; $(CI) $(NAME).c
	$(INDENT) association.c;	$(CI) association.c
	$(INDENT) ncreate.c;		$(CI) ncreate.c
	$(INDENT) nget.c;		$(CI) nget.c
	$(INDENT) nset.c;		$(CI) nset.c
	$(INDENT) requests.c;		$(CI) requests.c
	$(INDENT) sscond.c;		$(CI) sscond.c
	$(INDENT) fis_server.h;	$(CI) fis_server.h
	$(INDENT) services.h;		$(CI) services.h
	rm -f $(NAME) *.o *.BAK *.bak *% core

checkout:
	$(CO) $(NAME).c
	$(CO) association.c
	$(CO) ncreate.c
	$(CO) nget.c
	$(CO) nset.c
	$(CO) requests.c
	$(CO) sscond.c
	$(CO) fis_server.h
	$(CO) services.h

$(NAME): $(OBJ)
	$(CC) -o $(NAME) -g $(LFLAGS) $(OBJ) $(LIBS) -lm

$(NAME).o: $(NAME).c tbl.h fis_server.h
association.o: association.c fis_server.h
ncreate.o: ncreate.c fis_server.h
nget.o: nget.c fis_server.h
nset.o: nset.c fis_server.h
requests.o: requests.c fis_server.h
sscond.o: sscond.c fis_server.h

lint:
	flexelint $(LINTOPTIONS) $(NAME).c association.c requests.c nget.c nset.c ncreate.c sscond.c
