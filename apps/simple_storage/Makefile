#	Makefile for simple_storage

NAME = simple_storage
OBJ = $(NAME).o sscond.o
include $(DICOM_MAKE)

VPATH = ../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) -I../include
LFLAGS = $(LINK_FLAGS) $(LIBPATH_UCB) -L$(DICOM_LIB) $(LIBPATH_DATABASE)
LIBS = $(LIBS_CTN_NODB) $(LIBS_OS_SECURE_UCB)

application:	$(NAME)

install: $(NAME)
	$(INSTALLUID) $(NAME) $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -rf $(NAME) *.o *.BAK *.bak *% core debug

indent:
	$(INDENT) simple_storage.c
	$(INDENT) sscond.c
	$(INDENT) simple_storage.h

checkin:
	$(INDENT) simple_storage.c; $(CI) $(NAME).c
	$(INDENT) sscond.c; $(CI) sscond.c
	$(INDENT) simple_storage.h; $(CI) $(NAME).h
	rm -f $(NAME) *.o *.BAK *.bak *% core

checkout:
	$(CO) simple_storage.c
	$(CO) sscond.c
	$(CO) simple_storage.h

$(NAME): $(OBJ)
	$(CC) -o $(NAME) -g $(LFLAGS) $(OBJ) $(LIBS)

relink:
	$(CC) -o $(NAME) -g $(LFLAGS) $(OBJ) $(LIBS)

$(NAME).o: $(NAME).c dicom.h dulprotocol.h lst.h condition.h dicom_services.h \
dicom_messages.h dicom_uids.h simple_storage.h
sscond.o: sscond.c dicom.h simple_storage.h
lint:
	flexelint $(LINTOPTIONS) $(NAME).c sscond.c

local:
	./simple_storage -c LOCAL -s 2000

t1-1:	$(NAME)
	./simple_storage -b 1 -s 2000
