#	Makefile for send_image

NAME = send_image
OBJ = $(NAME).o

include $(DICOM_MAKE)
VPATH=../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_UCB) $(LIBS_OS_SECURE_UCB) $(LIBPATH_DATABASE)
LIBS = $(LIBS_CTN_NODB)


application:	$(NAME)

install: $(NAME)
	$(INSTALL) $(NAME) $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -rf $(NAME) *.o *.BAK *.bak *% core debug

indent:
	$(INDENT) $(NAME).c

checkin:
	$(INDENT) $(NAME).c; $(CI) $(NAME).c
	rm -f $(NAME) *.o *.BAK *.bak *% core

checkout:
	$(CO) $(NAME).c

$(NAME): $(OBJ)
	$(CC) -o $(NAME) -g $(LFLAGS) $(OBJ) $(LIBS) $(LFLAGS)

relink:
	$(CC) -o $(NAME) -g $(LFLAGS) $(OBJ) $(LIBS) $(LFLAGS)

$(NAME).o: $(NAME).c dicom.h dulprotocol.h dicom_objects.h dicom_services.h
dump_error.o: dump_error.c condition.h

lint:
	flexelint $(LINTOPTIONS) $(NAME).c

drnotest:
	./send_image -q -r -a DRNO -c DRNO drno 2100 a.dcm

