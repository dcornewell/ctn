#	Makefile for create_greyscale_module

NAME = create_greyscale_module
OBJ1 = $(NAME).o
include $(DICOM_MAKE)
VPATH=../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_DATABASE) $(LIBPATH_UCB)
LIBS = $(LIBS_CTN_NODB) $(LIBS_OS)

application:	$(NAME)

install: $(NAME)
	$(INSTALL) $(NAME) $(DICOM_BIN)

install-clean:	install
	make clean

clean:
	rm -f *.o $(NAME) *.BAK *.bak *% core

indent:
	$(INDENT) $(NAME).c

checkin:
	$(INDENT) $(NAME).c;	$(CI) $(NAME).c
	rm -f *.o $(NAME) *.BAK *.bak *% core

checkout:
	$(CO) $(NAME).c

$(NAME):	$(OBJ1)
	$(CC) -o $(NAME) -g $(LFLAGS) $(OBJ1) $(LIBS)

$(NAME).o: $(NAME).c dicom.h lst.h dicom_objects.h condition.h

lint:
	flexelint $(LINTOPTIONS) $(NAME).c
