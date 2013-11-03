#	Makefile for simple_pacs

NAME = simple_pacs
OBJ = $(NAME).o sscond.o
include $(DICOM_MAKE)

VPATH = ../../include:../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) -I../include
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_UCB) $(LIBPATH_DATABASE)
LIBS = $(LIBS_CTN) $(LIBS_OS_SECURE_UCB)

application:	$(NAME)

install: $(NAME)
	$(INSTALLUID) $(NAME) $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -f $(NAME) *.o *.BAK *.bak *% core

indent:
	$(INDENT) simple_pacs.c
	$(INDENT) sscond.c
	$(INDENT) simple_pacs.h

checkin:
	$(INDENT) simple_pacs.c; $(CI) $(NAME).c
	$(INDENT) sscond.c; $(CI) sscond.c
	$(INDENT) simple_pacs.h; $(CI) $(NAME).h
	rm -f $(NAME) *.o *.BAK *.bak *% core

checkout:
	$(CO) simple_pacs.c
	$(CO) sscond.c
	$(CO) simple_pacs.h

$(NAME): $(OBJ)
	$(CC) -o $(NAME) -g $(LFLAGS) $(OBJ) $(LIBS)

$(NAME).o: $(NAME).c dicom.h dulprotocol.h lst.h condition.h dicom_services.h \
dicom_messages.h dicom_uids.h simple_pacs.h
sscond.o: $(NAME).c dicom.h simple_pacs.h
lint:
	flexelint $(LINTOPTIONS) $(NAME).c sscond.c
