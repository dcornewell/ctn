#	Makefile for load_control

NAME = load_control
OBJ = $(NAME).o
include $(DICOM_MAKE)

VPATH = ../../include
CFLAGS = $(C_OPTS)
LIBS_OS += -ldl -lc
LFLAGS = -L$(DICOM_LIB) $(LIBPATH_DATABASE) $(LIBPATH_UCB) $(LIBS_OS) $(STATIC)
LIBS = $(LIBS_CTN) $(DEBUG_LIBS)

application:	$(NAME)

install: $(NAME)
	$(INSTALLUID) $(NAME) $(DICOM_BIN)

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
	$(CC) -o $(NAME) -g $(LFLAGS) $(OBJ) $(LIBS) -lm 

$(NAME).o: $(NAME).c tbl.h

lint:
	flexelint $(LINTOPTIONS) $(NAME).c
