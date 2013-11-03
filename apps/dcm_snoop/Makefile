#	Makefile for dcm_snoop

NAME = dcm_snoop
OBJ = $(NAME).o
include $(DICOM_MAKE)

VPATH = ../../include:../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE) -I../include
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_DATABASE) $(LIBPATH_UCB)
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
	$(INDENT) $(NAME).c;	$(CI) $(NAME).c
	rm -f $(NAME) *.o *.BAK *.bak *% core

checkout:
	$(CO) $(NAME).c

$(NAME): $(OBJ)
	$(CC) -o $(NAME) -O $(LFLAGS) $(OBJ) $(LIBS) $(LIBS_OS) $(DEBUG_LIBS)

$(NAME).o: $(NAME).c 

lint:
	flexelint $(LINTOPTIONS) $(NAME).c
