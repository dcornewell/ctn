#	Makefile for storage_classes

NAME = storage_classes
OBJ = $(NAME).o

include $(DICOM_MAKE)
VPATH=../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_UCB) $(LIBS_OS) $(LIBPATH_DATABASE)
LIBS = $(LIBS_CTN_NODB)


application:	$(NAME)

install: $(NAME)
	$(INSTALL) $(NAME) $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -f $(NAME) *.o *.BAK *.bak *% core

indent:
	$(INDENT) $(NAME).c

checkin:
	$(INDENT) $(NAME).c; $(CI) $(NAME).c
	rm -f $(NAME) *.o *.BAK *.bak *% core

checkout:
	$(CO) $(NAME).c

$(NAME): $(OBJ)
	$(CC) -o $(NAME) -g $(LFLAGS) $(OBJ) $(LIBS)

$(NAME).o: $(NAME).c dicom.h dulprotocol.h
dump_error.o: dump_error.c condition.h

lint:
	flexelint $(LINTOPTIONS) $(NAME).c
