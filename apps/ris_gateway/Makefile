#	Makefile for ris_gateway

NAME = ris_gateway
OBJ = $(NAME).o association.o event.o procevent.o requests.o sscond.o
include $(DICOM_MAKE)

VPATH = ../../include
CFLAGS = $(C_OPTS)
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_DATABASE) $(LIBPATH_UCB) $(LIBS_OS)
LIBS = $(LIBS_CTN) $(DEBUG_LIBS)

application:	$(NAME)

install: $(NAME)
	$(INSTALLUID) $(NAME) $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -f $(NAME) *.o *.BAK *.bak *% core

indent:
	$(INDENT) $(NAME).c
	$(INDENT) association.c
	$(INDENT) event.c
	$(INDENT) procevent.c
	$(INDENT) requests.c
	$(INDENT) sscond.c
	$(INDENT) ris_gateway.h

checkin:
	$(INDENT) $(NAME).c; $(CI) $(NAME).c
	$(INDENT) association.c;	$(CI) association.c
	$(INDENT) event.c;		$(CI) event.c
	$(INDENT) procevent.c;		$(CI) procevent.c
	$(INDENT) requests.c;		$(CI) requests.c
	$(INDENT) sscond.c;		$(CI) sscond.c
	$(INDENT) ris_gateway.h;	$(CI) ris_gateway.h
	rm -f $(NAME) *.o *.BAK *.bak *% core

checkout:
	$(CO) $(NAME).c
	$(CO) association.c
	$(CO) event.c
	$(CO) procevent.c
	$(CO) requests.c
	$(CO) sscond.c
	$(CO) ris_gateway.h

$(NAME): $(OBJ)
	$(CC) -o $(NAME) -g $(LFLAGS) $(OBJ) $(LIBS) -lm

$(NAME).o: $(NAME).c tbl.h ris_gateway.h
association.o: association.c ris_gateway.h
requests.o: requests.c ris_gateway.h
sscond.o: sscond.c ris_gateway.h

lint:
	flexelint $(LINTOPTIONS) $(NAME).c association.c
