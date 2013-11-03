#	Makefile for mwlQuery

NAME = mwlQuery
OBJ = $(NAME).o

include $(DICOM_MAKE)
VPATH=../../include
CFLAGS = $(C_OPTS) -I$(DICOM_INCLUDE)
#LFLAGS = -L$(DICOM_LIB) $(LIBPATH_UCB) $(LIBS_OS) $(LIBPATH_DATABASE)
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_UCB) $(LIBS_OS)
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
	$(CC) -o $(NAME) -g $(LFLAGS) $(OBJ) $(LIBS)

$(NAME).o: $(NAME).c dicom.h dulprotocol.h

lint:
	flexelint $(LINTOPTIONS) $(NAME).c
